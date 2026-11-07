/******************************************************************************************************************************************
                                                               START OF FILE
*******************************************************************************************************************************************
                                                @
                                                @
                                                @@@ @@@@@@@@@@
                @@@@@@@@@@@@@  @@@        @@@   @ @@          @    @@@@@@@@@@@    @@@   @@@        @@@   @@@@@       @@@@@
                     @@@       @@@        @@@   @@@            @@  @@@      @@@   @@@   @@@        @@@   @@@@@@     @@@@@@
                     @@@       @@@        @@@  @  @             @  @@@      @@@   @@@   @@@        @@@   @@@ @@     @@ @@@
                     @@@       @@@@@@@@@@@@@@  @  @             @  @@@@@@@@@@@    @@@   @@@        @@@   @@@ @@@   @@@ @@@
                     @@@       @@@        @@@  @                @  @@@@@@@@@      @@@   @@@        @@@   @@@  @@@ @@@  @@@
                     @@@       @@@        @@@  @@              @@  @@@    @@@     @@@   @@@        @@@   @@@   @@@@@   @@@
                     @@@       @@@        @@@    @            @    @@@     @@@    @@@    @@@@@@@@@@@@    @@@    @@@    @@@
                     @@@       @@@        @@@     @@@@@@@@@@@@     @@@      @@@   @@@      @@@@@@@@      @@@     @     @@@

                     S      P      A      C      E        T      E      C      H      N      O      L      O      G      Y

*******************************************************************************************************************************************
                                                                 FILE INFO
******************************************************************************************************************************************/

// CREATED BY:       Tobiasz Bromma (tobiasz.bromma@thoriumspace.com)
// CREATION DATE:    October 2023
// DESCRIPTION:      Low level layer for SPI3 peripheral, communicating with the integrated Semtech SX126x radio

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "perSpiRf.h"
#include "irqSpi.h"
#include "perGpio.h"
#include "regPwr.h"
#include "sysclk.h"
#include "mcuCore.h"
#include "utils.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::spiRf {

#undef CODE_LABEL
#undef CODE_STATUS
#undef ACTION_ON_ERR
#define CODE_LABEL      ctx.config.label_p
#define CODE_STATUS     ctx.status
#define ACTION_ON_ERR   ctx.status = Status::ERR; ChangeMode(MODE_ERR)

static constexpr uint8 TXRX_TIMEOUT_FACTOR = 20;    // TX-RX timeout safety factor
static constexpr uint32 NS_PER_SEC = 1000000000;
static constexpr uint32 NS_PER_US = 1000;
static constexpr uint8 BITS_PER_BYTE = 8;

// Transaction status
enum trStat_t {
    TR_STAT_NONE,
    TR_STAT_NEW,
    TR_STAT_TXRUN,
    TR_STAT_RXRUN,
    TR_STAT_DONE,
    TR_STAT_ERR,
};

// Transaction type
enum trType_t {
    TR_TYPE_NONE,
    TR_TYPE_TX,
    TR_TYPE_RX,
    TR_TYPE_TXRX,
};

// Transaction buffer
struct trBuff_t {
    trType_t    trType;
    trStat_t    trStatus = TR_STAT_NONE;
    uint64      trId;
    bool        wait;
    bool        keepSsLow;
    uint16      txSize;
    uint16      txPos;
    uint16      rxSize;
    uint16      rxPos;
    uint8*      rxDest_p;
    uint32      timeoutUs;
    uint8*      buff_p;
};

struct {
    config_t            config = {};
    Status              status = Status::NONE;
    mode_t              mode = MODE_NONE;
    mode_t              modeLast = MODE_NONE;
    bool                enabled = false;
    bareOs::Timer       timerTimeout;
    volatile bool       startTimerTimeout = false;
    uint32              baudHz;
    uint32              byteTxRxTimeNs;
    hal::perId_t        spiPerId;
    hal::irqId_t        spiIrqId;
    volatile REG_SPI*   spiReg_p = nullptr;
    trBuff_t*           trBuff_p = nullptr;
    trBuff_t*           trBuffRun_p = nullptr;
    uint64              trId = 0;
    volatile uint8      buffsBusy = 0;
    volatile uint8      dummyRead8;
    volatile uint16     dummyRead16;
    uint32              handlerExecCnt;
} ctx = {};

/******************************************************************************************************************************************
                                                        PRIVATE FUNCTION DECLARATIONS
******************************************************************************************************************************************/

static void         ChangeMode(mode_t newMode);
static void         EnableSpi();
static void         DisableSpi();
static void         EnableTxIrq();
static void         DisableTxIrq();
static void         EnableRxIrq();
static void         DisableRxIrq();
static void         SetSsLine();
static void         ClearSsLine();
static void         ClearRxFifo();
static Exec         SetupTransaction(uint8* txSrc, uint16 txSize, uint8* rxDest, uint16 rxSize, bool keepSsLow, uint64* trId, bool wait);
static void         TriggerTransaction(trBuff_t* trBuff);
static trBuff_t*    FindFreeTrBuff();
static trBuff_t*    FindWaitingTrBuff();
static bool         CheckForTimeout();
static void         IrqHandlerClbk(hal::spiNum_t spiNum);
static void         IrqTxHandler();
static void         IrqRxHandler();
static void         IrqErrHandler();

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec Setup(config_t config)
{
    ctx.status = Status ::INIT;
    ctx.config = config;

    //-----------------------------------------------------------------------------------
    // Check config

    ASSERT(ctx.config.irqPrio != hal::IRQ_PRIO_none, "IRQ PRIORITY");

    //-----------------------------------------------------------------------------------
    // Get SPI peripheral base address

    ctx.spiReg_p = REG_SPI3_RF;
    ctx.spiPerId = hal::PER_SPI3_RF;
    ctx.spiIrqId = hal::IRQ_SPI3_RF;

    //-----------------------------------------------------------------------------------
    // Enable peripheral clock

    hal::clk::PerClkEnable(ctx.spiPerId);

    //-----------------------------------------------------------------------------------
    // Config SPI peripheral registers

    ctx.spiReg_p->CR1.bit.MSTR = true;                          // Enable SPI master mode
    ctx.spiReg_p->CR1.bit.BR = ctx.config.baudrate & 0b111;     // Set bus baudrate
    ctx.spiReg_p->CR1.bit.CPOL = ctx.config.clkPol;             // Set clock polarity
    ctx.spiReg_p->CR1.bit.CPHA = ctx.config.clkPhase;           // Set clock phase
    ctx.spiReg_p->CR1.bit.LSBFIRST = false;                     // Data is transmitted-received with the MSB first
    ctx.spiReg_p->CR1.bit.SSM = false;                          // Slave select pin managed by hardware
    ctx.spiReg_p->CR1.bit.SSI = true;                           // Set slave select pin state high (deselect slave)
    ctx.spiReg_p->CR1.bit.RXONLY = false;                       // Full-duplex mode (transmit and receive)
    ctx.spiReg_p->CR1.bit.CRCEN = false;                        // CRC calculation disabled
    ctx.spiReg_p->CR1.bit.BIDIMODE = false;                     // 2-line unidirectional data mode selected

    ctx.spiReg_p->CR2.bit.NSSP = false;                         // No NSS pulse
    ctx.spiReg_p->CR2.bit.SSOE = true;                          // Slave select output enabled
    ctx.spiReg_p->CR2.bit.FRF = false;                          // SPI Motorola mode
    ctx.spiReg_p->CR2.bit.ERRIE = true;                         // Error interrupt is enabled
    ctx.spiReg_p->CR2.bit.DS = 0b0111;                          // Data size 8-bit
    ctx.spiReg_p->CR2.bit.FRXTH = true;                         // RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)

    //-----------------------------------------------------------------------------------
    // Calculate baudrate in Hz and byte transmision time in ns

    ctx.baudHz = hal::clk::SysClkGetFreqHz(hal::clk::SYSCLK_PCLK3) / (uint32)utils::Exp(2, ctx.config.baudrate + 1);
    ctx.byteTxRxTimeNs = (uint32)(((uint64)BITS_PER_BYTE * NS_PER_SEC) / ctx.baudHz);

    //-----------------------------------------------------------------------------------
    // Config SPI interrupts

    CHECK_EXEC(hal::irqSpi::SetIrqClbk(hal::SPI3_RF, IrqHandlerClbk));
    hal::core::IrqSetPrio(ctx.spiIrqId, ctx.config.irqPrio);

    //-----------------------------------------------------------------------------------
    // Allocate memory for UART transaction buffers

    ctx.trBuff_p = (trBuff_t*)calloc(ctx.config.txBuffCnt, sizeof(trBuff_t));
    ASSERT(ctx.trBuff_p != nullptr, "MEM ALLOC FOR TX BUFF");

    uint8* pointer;

    for (uint8 buffNum = 0; buffNum < ctx.config.txBuffCnt; buffNum++) {
        pointer = (uint8*)calloc(config.txBuffSize, sizeof(uint8));
        ctx.trBuff_p[buffNum].buff_p = pointer;
        ASSERT(ctx.trBuff_p[buffNum].buff_p != nullptr, "MEM ALLOC FOR TX BUFF");
    }

    //-----------------------------------------------------------------------------------
    // Enable debug?

    if (ctx.config.debugEn == true) {
        // MOSI
        hal::GPIO::SetPinOutSpeed(hal::PORT_A, hal::PIN_7, hal::GPIO::OutSpeed::High);
        hal::GPIO::SetPinMode(hal::PORT_A, hal::PIN_7, hal::GPIO::PinMode::AltFunc);
        hal::GPIO::SetPinAltFunc(hal::PORT_A, hal::PIN_7, hal::FUNC_AF13);
        hal::GPIO::SetPinOutType(hal::PORT_A, hal::PIN_7, hal::GPIO::OutType::PushPull);

        // MISO
        hal::GPIO::SetPinMode(hal::PORT_A, hal::PIN_6, hal::GPIO::PinMode::AltFunc);
        hal::GPIO::SetPinAltFunc(hal::PORT_A, hal::PIN_6, hal::FUNC_AF13);
        hal::GPIO::SetPinPullDir(hal::PORT_A, hal::PIN_6, hal::GPIO::PullDir::Gnd);

        // SCK
        hal::GPIO::SetPinOutSpeed(hal::PORT_A, hal::PIN_5, hal::GPIO::OutSpeed::High);
        hal::GPIO::SetPinMode(hal::PORT_A, hal::PIN_5, hal::GPIO::PinMode::AltFunc);
        hal::GPIO::SetPinAltFunc(hal::PORT_A, hal::PIN_5, hal::FUNC_AF13);
        hal::GPIO::SetPinOutType(hal::PORT_A, hal::PIN_5, hal::GPIO::OutType::PushPull);

        // SS
        hal::GPIO::SetPinOutSpeed(hal::PORT_A, hal::PIN_4, hal::GPIO::OutSpeed::High);
        hal::GPIO::SetPinMode(hal::PORT_A, hal::PIN_4, hal::GPIO::PinMode::AltFunc);
        hal::GPIO::SetPinAltFunc(hal::PORT_A, hal::PIN_4, hal::FUNC_AF13);
        hal::GPIO::SetPinOutType(hal::PORT_A, hal::PIN_4, hal::GPIO::OutType::PushPull);

        // Mark pin usage
        hal::GPIO::SetPinInUse(hal::PORT_A, hal::PIN_7, "%s %s", ctx.config.label_p, "MOSI");
        hal::GPIO::SetPinInUse(hal::PORT_A, hal::PIN_6, "%s %s", ctx.config.label_p, "MISO");
        hal::GPIO::SetPinInUse(hal::PORT_A, hal::PIN_5, "%s %s", ctx.config.label_p, "SCK");
        hal::GPIO::SetPinInUse(hal::PORT_A, hal::PIN_4, "%s %s", ctx.config.label_p, "SS");
    }

    //-----------------------------------------------------------------------------------

    ctx.timerTimeout.Config(bareOs::Timer::ModeAutoOff, bareOs::Timer::UnitUs);
    ctx.status = Status::OK;
    DisableSpi();
    return Exec::OK;
}

Exec Handler(void)
{
    CHECK_STATUS();

    ctx.handlerExecCnt++;

    switch(ctx.mode) {
        case MODE_NONE:
        {
            REPORT_FAT("MODE NONE");
        }
        case MODE_OFF:
        {
            break;
        }
        case MODE_IDLE:
        {
            if (ctx.timerTimeout.GetState() == State::ON) ctx.timerTimeout.Disable();
            trBuff_t* waitingTrBuff;
            waitingTrBuff = FindWaitingTrBuff();
            if (waitingTrBuff) TriggerTransaction(waitingTrBuff);
            break;
        }
        case MODE_BUSY:
        {
            if (CheckForTimeout()) {
                DisableSpi();
                RET_ERR("SPI TRANSACTION TIMEOUT");
            }
            break;
        }
        case MODE_ERR:
        {
            return Exec::ERR;
        }
    }

    return Exec::OK;
}

Exec TxData(uint8* txSrc_p, uint16 txSize, bool keepSsLow, uint64* trId_p, bool wait)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(txSrc_p, txSize, nullptr, 0, keepSsLow, trId_p, wait));
}

Exec RxData(uint8* rxDest_p, uint16 rxSize, bool keepSsLow, uint64* trId_p, bool wait)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(nullptr, 0, rxDest_p, rxSize, keepSsLow, trId_p, wait));
}

Exec TxRxData(uint8* txSrc_p, uint16 txSize, uint8* rxDest_p, uint16 rxSize, bool keepSsLow, uint64* trId_p, bool wait)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(txSrc_p, txSize, rxDest_p, rxSize, keepSsLow, trId_p, wait));
}

uint32 GetBaudrateHz()
{
    return ctx.baudHz;
}

Status GetStatus()
{
    return ctx.status;
}

mode_t GetMode()
{
    return ctx.mode;
}

/******************************************************************************************************************************************
                                                         PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

void ChangeMode(mode_t newMode)
{
    ctx.modeLast = ctx.mode;
    ctx.mode = newMode;
}

void EnableSpi()
{
    ctx.dummyRead16 = (uint16)ctx.spiReg_p->SR.val;
    hal::clk::PerClkEnable(ctx.spiPerId);
    hal::core::IrqEnable(ctx.spiIrqId);
    ctx.spiReg_p->CR1.bit.SPE = true;
    ChangeMode(MODE_IDLE);
}

void DisableSpi()
{
    hal::core::IrqDisable(ctx.spiIrqId);
    ctx.spiReg_p->CR1.bit.SPE = false;
    hal::clk::PerClkDisable(ctx.spiPerId);
    ChangeMode(MODE_OFF);
}

void EnableTxIrq()
{
    ctx.spiReg_p->CR2.bit.TXEIE = true;
}

void DisableTxIrq()
{
    ctx.spiReg_p->CR2.bit.TXEIE = false;
}

void EnableRxIrq()
{
    ctx.spiReg_p->CR2.bit.RXNEIE = true;
}

void DisableRxIrq()
{
    ctx.spiReg_p->CR2.bit.RXNEIE = false;
}

void SetSsLine()
{
   REG_PWR->SUBGHZSPICR.bit.NSS = 0;
}

void ClearSsLine()
{
   REG_PWR->SUBGHZSPICR.bit.NSS = 1;
}

void ClearRxFifo()
{
    while (ctx.spiReg_p->SR.bit.FRLVL) ctx.dummyRead16 = ctx.spiReg_p->DR.val16;
}

Exec SetupTransaction(uint8* txSrc_p, uint16 txSize, uint8* rxDest_p, uint16 rxSize, bool keepSsLow, uint64* trId_p, bool wait)
{
    ASSERT(txSize || rxSize, "TX AND RX DATA SIZE IS ZERO");
    ASSERT(txSize <= ctx.config.txBuffSize, "TX DATA SIZE TO BIG");
    ASSERT_COND(txSize, txSrc_p != nullptr, "TX DATA SOURCE");
    ASSERT_COND(rxSize, rxDest_p != nullptr, "RX DATA DEST.");

    //-----------------------------------------------------------------------------------
    // Find free transaction buff

    trBuff_t* trBuffFree_p;
    trBuffFree_p = FindFreeTrBuff();

    if (trBuffFree_p == nullptr) {
        REPORT_WAR("TX FIFO FULL");
        do {
            CHECK_EXEC(Handler());
            trBuffFree_p = FindFreeTrBuff();
        } while (trBuffFree_p == nullptr);
    }

    //-----------------------------------------------------------------------------------
    // Setup transaction details

    if ((txSize > 0) && (rxSize == 0))      trBuffFree_p->trType = TR_TYPE_TX;
    else if ((rxSize > 0) && (txSize == 0)) trBuffFree_p->trType = TR_TYPE_RX;
    else                                    trBuffFree_p->trType = TR_TYPE_TXRX;

    trBuffFree_p->txPos = 0;
    trBuffFree_p->rxPos = 0;
    trBuffFree_p->txSize = txSize;
    trBuffFree_p->rxSize = rxSize;
    trBuffFree_p->rxDest_p = rxDest_p;
    memcpy(trBuffFree_p->buff_p, txSrc_p, txSize);
    trBuffFree_p->trId = ++ctx.trId;
    trBuffFree_p->wait = wait;
    trBuffFree_p->keepSsLow = keepSsLow;
    trBuffFree_p->timeoutUs = ((txSize + rxSize) * ctx.byteTxRxTimeNs * TXRX_TIMEOUT_FACTOR) / NS_PER_US;
    trBuffFree_p->trStatus = TR_STAT_NEW;
    ctx.buffsBusy++;

    if (trId_p) *trId_p = trBuffFree_p->trId;

    //-----------------------------------------------------------------------------------
    // Trigger instant RX or wait till idle state

    if ((ctx.mode == MODE_OFF) || (ctx.mode == MODE_IDLE)) TriggerTransaction(trBuffFree_p);

    if (wait == true) {
        do {
            CHECK_EXEC(Handler());
        } while (ctx.mode == MODE_BUSY);
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

void TriggerTransaction(trBuff_t* trBuff_p)
{
    ctx.trBuffRun_p = trBuff_p;

    //-----------------------------------------------------------------------------------
    // Trigger transaction

    if (ctx.mode == MODE_OFF) EnableSpi();
    ctx.startTimerTimeout = true;
    ChangeMode(MODE_BUSY);
    SetSsLine();

    if ((ctx.trBuffRun_p->trType == TR_TYPE_TX) || (ctx.trBuffRun_p->trType == TR_TYPE_TXRX)) {
        ctx.trBuffRun_p->trStatus = TR_STAT_TXRUN;
        EnableTxIrq();
    }
    else {
        ctx.trBuffRun_p->trStatus = TR_STAT_RXRUN;
        ctx.spiReg_p->DR.val8 = 0;                  // Push first dummy byte into tx register to start rx
        EnableRxIrq();
    }
}

trBuff_t* FindFreeTrBuff()
{
    uint8 buffNum;
    uint8 buffEnd;

    if (ctx.trBuffRun_p == nullptr) {
        buffNum = 0;
        buffEnd = ctx.config.txBuffCnt - 1;
    }
    else {
        buffEnd = (uint8)(ctx.trBuffRun_p - ctx.trBuff_p);
        buffNum = (uint8)((buffEnd + 1) % ctx.config.txBuffCnt);
    }

    do {
        if ((ctx.trBuff_p[buffNum].trStatus == TR_STAT_NONE) ||
            (ctx.trBuff_p[buffNum].trStatus == TR_STAT_DONE)) return &ctx.trBuff_p[buffNum];
        buffNum += 1;
        buffNum %= ctx.config.txBuffCnt;
    } while (buffNum != buffEnd);

    return nullptr;
}

trBuff_t* FindWaitingTrBuff()
{
    uint8 buffNum;
    uint8 buffEnd;

    if (ctx.trBuffRun_p == nullptr) {
        buffNum = 0;
        buffEnd = ctx.config.txBuffCnt - 1;
    }
    else {
        buffEnd = (uint8)(ctx.trBuffRun_p - ctx.trBuff_p);
        buffNum = (uint8)((buffEnd + 1) % ctx.config.txBuffCnt);
    }

    do {
        if (ctx.trBuff_p[buffNum].trStatus == TR_STAT_NEW) return &ctx.trBuff_p[buffNum];
        buffNum += 1;
        buffNum %= ctx.config.txBuffCnt;
    } while (buffNum != buffEnd);

    return nullptr;
}

bool CheckForTimeout()
{
    if ((ctx.trBuffRun_p->trStatus == TR_STAT_TXRUN) ||
        (ctx.trBuffRun_p->trStatus == TR_STAT_RXRUN)) {
            if (ctx.startTimerTimeout) {
                ctx.startTimerTimeout = false;
                ctx.timerTimeout.ChangeInterval(ctx.trBuffRun_p->timeoutUs);
                ctx.timerTimeout.Enable(false, false);
            }
        return ctx.timerTimeout.Check();
    }
    return false;
}

void IrqHandlerClbk(hal::spiNum_t spiNum)
{
    volatile SPI_CR2 CR2_REG;
    volatile SPI_SR SR_REG;
    CR2_REG.val = ctx.spiReg_p->CR2.val;
    SR_REG.val = ctx.spiReg_p->SR.val;

    if (CR2_REG.bit.RXNEIE && SR_REG.bit.RXNE) {
        IrqRxHandler();
    }
    if (CR2_REG.bit.TXEIE && SR_REG.bit.TXE) {
        IrqTxHandler();
    }
    if (CR2_REG.bit.ERRIE &&
        (   SR_REG.bit.MODF ||
            SR_REG.bit.OVR  ||
            SR_REG.bit.UDR )) {
        IrqErrHandler();
    }
}

void IrqTxHandler()
{
    //-----------------------------------------------------------------------------------
    // Push tx data into fifo

    while (ctx.spiReg_p->SR.bit.TXE && (ctx.trBuffRun_p->txPos < ctx.trBuffRun_p->txSize)) {
        if ((uint16)(ctx.trBuffRun_p->txSize - ctx.trBuffRun_p->txPos) >= sizeof(uint16)) {
            ctx.spiReg_p->DR.val16 = *((uint16*)&ctx.trBuffRun_p->buff_p[ctx.trBuffRun_p->txPos]);
            ctx.trBuffRun_p->txPos += sizeof(uint16);
        }
        else {
            ctx.spiReg_p->DR.val8 = ctx.trBuffRun_p->buff_p[ctx.trBuffRun_p->txPos];
            ctx.trBuffRun_p->txPos += sizeof(uint8);
        }
    }

    //-----------------------------------------------------------------------------------
    // Clear rx fifo to prevent OVR (overrun) error IRQ flag

    ClearRxFifo();

    //-----------------------------------------------------------------------------------
    // Chedck if all bytes have been sent

    if ((ctx.trBuffRun_p->txPos == ctx.trBuffRun_p->txSize) && (ctx.spiReg_p->SR.bit.BSY == false)) {
        ClearRxFifo();
        DisableTxIrq();

        if ((ctx.trBuffRun_p->trType == TR_TYPE_TX) && (ctx.trBuffRun_p->keepSsLow == false)) {
            ClearSsLine();
        }

        if (ctx.trBuffRun_p->trType == TR_TYPE_TXRX) {
            ctx.trBuffRun_p->trStatus = TR_STAT_RXRUN;
            ctx.dummyRead8 = (uint8)ctx.spiReg_p->DR.bit.DR;
            EnableRxIrq();
            ctx.spiReg_p->DR.bit.DR = 0;        // Push first dummy byte into tx register to start rx
            return;
        }
        else {
            ctx.trBuffRun_p->trStatus = TR_STAT_DONE;
            ctx.buffsBusy--;

            trBuff_t* trBuffWaiting_p = FindWaitingTrBuff();

            if (trBuffWaiting_p != nullptr) {
                TriggerTransaction(trBuffWaiting_p);
            }
            else {
                ChangeMode(MODE_IDLE);
            }
        }
    }
}

void IrqRxHandler()
{
    //-----------------------------------------------------------------------------------
    // Read rx byte

    ctx.trBuffRun_p->rxDest_p[ctx.trBuffRun_p->rxPos++] = (uint8)ctx.spiReg_p->DR.val8;

    //-----------------------------------------------------------------------------------
    // All bytes have been received?

    if (ctx.trBuffRun_p->rxPos < ctx.trBuffRun_p->rxSize) {
        ctx.spiReg_p->DR.val8 = 0;
        return;
    }
    else {
        DisableRxIrq();
        if (ctx.trBuffRun_p->keepSsLow == false) ClearSsLine();
        ctx.trBuffRun_p->trStatus = TR_STAT_DONE;
        ctx.buffsBusy--;
    }

    //-----------------------------------------------------------------------------------
    // Check if there is a new transaction waiting

    trBuff_t* trBuffWaiting_p = FindWaitingTrBuff();
    if (trBuffWaiting_p != nullptr) {
        TriggerTransaction(trBuffWaiting_p);
    }
    else {
        ChangeMode(MODE_IDLE);
    }
}

void IrqErrHandler()
{
    while(1);
}

} // END OF NAMESPACE 'hal::SPI_M'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
