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
// CREATION DATE:    March 2023
// DESCRIPTION:      Hardware abstraction layer for USART peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "perUsart.h"
#include "irqUsart.h"
#include "perGpio.h"
#include "mcuCore.h"
#include "sysclk.h"
#include "irqList.h"
#include "utils.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::usart {

#undef ACTION_ON_ERR
#undef CODE_LABEL
#undef CODE_STATUS
#define ACTION_ON_ERR       ctx.status = Status::ERR; ctx.mode = MODE_ERR
#define CODE_LABEL          ctx.config.label_p
#define CODE_STATUS         ctx.status
#define CHECK_PINCFG(cfgOk) ASSERT(cfgOk == true, "PIN CONFIG INVALID")

static constexpr uint8 USART_SAMPLING = 16;
static constexpr uint8 UART_BAUD_MAX_ERR = 25;          // 2.5%
static constexpr uint8 TX_TIMEOUT_FACTOR = 10;          // TX timeout safety factor
static constexpr uint16 MS_PER_SEC = 1000;
static constexpr uint32 NS_PER_SEC = 1000000000;
static constexpr uint32 NS_PER_US = 1000;
static constexpr uint8 BITS_PER_TX_BYTE = 10;           // 10 -> 1 Start bit + 1 stop bit + 8 data bits
static constexpr uint16 BRR_REG_MAX_VAL = 65535;
static constexpr uint8 PERSCALER_MAX_REGVAL = 0b1011;

static USART* usartBus_p[hal::USART_count];

/******************************************************************************************************************************************
                                                        PRIVATE FUNCTION DECLARATIONS
******************************************************************************************************************************************/

void IrqTxReadyHandler(hal::usartNum_t usartNum);
void IrqTxEmptyHandler(hal::usartNum_t usartNum);

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec USART::Setup(config_t config)
{
    ctx.status = Status ::INIT;
    ctx.config = config;

    //-----------------------------------------------------------------------------------
    // Check config

    ASSERT(ctx.config.usartNum != hal::USART_none, "USART NUMBER");
    ASSERT(ctx.config.baudrate != BAUD_none, "BAUDRATE");
    ASSERT(ctx.config.irqPrio != hal::IRQ_PRIO_none, "IRQ PRIORITY");
    ASSERT_COND(ctx.config.useTxdLine, ctx.config.txdPinPort != hal::PORT_none, "TXD PIN PORT");
    ASSERT_COND(ctx.config.useTxdLine, ctx.config.txdPinNum != hal::PIN_none, "TXD PIN NUMBER");
    ASSERT_COND(ctx.config.useTxdLine, ctx.config.txdPinFunc != hal::FUNC_none, "TXD PIN ALT. FUNC.");
    ASSERT_COND(ctx.config.useRxdLine, ctx.config.rxdPinPort != hal::PORT_none, "RXD PIN PORT");
    ASSERT_COND(ctx.config.useRxdLine, ctx.config.rxdPinNum != hal::PIN_none, "RXD PIN NUMBER");
    ASSERT_COND(ctx.config.useRxdLine, ctx.config.rxdPinFunc != hal::FUNC_none, "RXD PIN ALT. FUNC.");

    //-----------------------------------------------------------------------------------
    // Get USART peripheral base address

    if (ctx.config.usartNum == hal::USART_1) {
        ctx.usartReg_p = REG_USART1;
        ctx.usartPerId = hal::PER_USART1;
        ctx.usartIrqId = hal::IRQ_USART1;
    }
    else if (ctx.config.usartNum == hal::LPUART_1) {
        ctx.usartReg_p = REG_LPUART1;
        ctx.usartPerId = hal::PER_LPUART1;
        ctx.usartIrqId = hal::IRQ_LPUART1;
    }
    else if (ctx.config.usartNum == hal::USART_2) {
        ctx.usartReg_p = REG_USART2;
        ctx.usartPerId = hal::PER_USART2;
        ctx.usartIrqId = hal::IRQ_USART2;
    }

    //-----------------------------------------------------------------------------------
    // Setup clock's

    hal::clk::PerClkEnable(ctx.usartPerId);
    hal::clk::UsartClkSrcSelect(ctx.config.usartNum, ctx.config.clkSrc);

    //-----------------------------------------------------------------------------------
    // Calculate baudrate

    uint32 tempBaudRegVal = {};
    uint32 baudRegVal = {};
    uint32 baudReal = {};
    int32 baudErr = {};
    int32 minBaudErr = {};
    uint32 clkSrcFreqHz = {};
    uint8 tempPerscRegVal = {};
    uint8 perscRegVal = {};
    uint16 perscVal = {};

    clkSrcFreqHz = hal::clk::UsartClkGetFreqHz(ctx.config.usartNum);
    ASSERT(clkSrcFreqHz > 0, "SELECTED USART CLOCK SOURCE IS DISABLED");
    ASSERT((clkSrcFreqHz / ctx.config.oversampling) >= (uint32)ctx.config.baudrate, "BAUDRATE TO HIGH");
    minBaudErr = INT32_MAX;

    for (tempPerscRegVal = 0; tempPerscRegVal < PERSCALER_MAX_REGVAL; tempPerscRegVal++) {
        perscVal = (uint16)utils::Exp(2, tempPerscRegVal);
        tempBaudRegVal = clkSrcFreqHz / (perscVal * ctx.config.baudrate);
        if (tempBaudRegVal > BRR_REG_MAX_VAL) continue;
        baudReal = clkSrcFreqHz / (perscVal * tempBaudRegVal);
        baudErr = ((int32)baudReal - ctx.config.baudrate) * 1000;
        baudErr /= ctx.config.baudrate;
        if (baudErr < 0) baudErr *= -1;
        if (baudErr < minBaudErr) {
            baudRegVal = tempBaudRegVal;
            perscRegVal = tempPerscRegVal;
            minBaudErr = baudErr;
        }
    }

    ASSERT(minBaudErr <= UART_BAUD_MAX_ERR, "BAUDRATE ERROR TO BIG");

    ctx.usartReg_p->PRESC.bit.PRESCALER = perscRegVal & 0b1111;
    ctx.byteTxTimeNs = (uint32)(((uint64)BITS_PER_TX_BYTE * NS_PER_SEC) / ctx.config.baudrate);

    if (ctx.config.oversampling == OVERSAMPLING_16) {
        ctx.usartReg_p->BRR.bit.BRR = (uint16)baudRegVal;
    }
    else {
        ctx.usartReg_p->BRR.bit.BRR = (uint16)((baudRegVal & 0xFFF0) | ((baudRegVal & 0xF) >> 1));
    }

    //-----------------------------------------------------------------------------------
    // Config UART peripheral registers

    ctx.usartReg_p->CR1.bit.UESM = 1;                               // USART able to wake up the MCU from low-power mode

    if (ctx.config.parity != PARITY_NONE) {
        ctx.usartReg_p->CR1.bit.PCE = true;                        // Parity control enabled
        ctx.usartReg_p->CR1.bit.PS = ctx.config.parity & 0b1;      // Parity selection
    }
    else ctx.usartReg_p->CR1.bit.PCE = false;                       // Parity control disabled

    ctx.usartReg_p->CR1.bit.M0 = 0;                                 // 1 start bit, 8 Data bits, n Stop bit
    ctx.usartReg_p->CR1.bit.M1 = 0;                                 // 1 start bit, 8 Data bits, n Stop bit
    ctx.usartReg_p->CR1.bit.OVER8 = ctx.config.oversampling & 0b1;  // Oversampling mode
    ctx.usartReg_p->CR1.bit.FIFOEN = true;                          // Fifo enable
    ctx.usartReg_p->CR2.bit.STOP = ctx.config.stopBits & 0b11;      // Stop bits count
    ctx.usartReg_p->CR3.bit.WUS = 0b10;                             // Wake-up from low-power mode active on start bit detection

    //-----------------------------------------------------------------------------------
    // Config UART pins

    if (ctx.config.useTxdLine) {
        hal::GPIO::SetPinMode(ctx.config.txdPinPort, ctx.config.txdPinNum, hal::GPIO::PinMode::AltFunc);
        hal::GPIO::SetPinAltFunc(ctx.config.txdPinPort, ctx.config.txdPinNum, ctx.config.txdPinFunc);
        hal::GPIO::SetPinInUse(ctx.config.txdPinPort, ctx.config.txdPinNum, "%s %s", ctx.config.label_p, "TXD PIN");
        ctx.txdPinCfgOk = true;
    }

    if (ctx.config.useRxdLine) {
        hal::GPIO::SetPinMode(ctx.config.rxdPinPort, ctx.config.rxdPinNum, hal::GPIO::PinMode::AltFunc);
        hal::GPIO::SetPinAltFunc(ctx.config.rxdPinPort, ctx.config.rxdPinNum, ctx.config.rxdPinFunc);
        hal::GPIO::SetPinInUse(ctx.config.rxdPinPort, ctx.config.rxdPinNum, "%s %s", ctx.config.label_p, "RXD PIN");
        ctx.rxdPinCfgOk = true;
    }

    //-----------------------------------------------------------------------------------
    // Config UART interrupts

    if (ctx.config.useTxdLine) {
        CHECK_EXEC(hal::irqUsart::SetTxfnfClbk(ctx.config.usartNum, IrqTxEmptyHandler));
        CHECK_EXEC(hal::irqUsart::SetTxcmpClbk(ctx.config.usartNum, IrqTxReadyHandler));
    }

    if (ctx.config.useRxdLine) {
        CHECK_EXEC(hal::irqUsart::SetRxfneClbk(ctx.config.usartNum, ctx.config.RxDataClbk));
    }

    hal::core::IrqSetPrio(ctx.usartIrqId, ctx.config.irqPrio);
    hal::core::IrqEnable(ctx.usartIrqId);
    hal::irq::SetIrqInUse(ctx.usartIrqId, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Allocate memory for UART transaction buffers

    ctx.trBuff_p = (trBuff_t*)calloc(ctx.config.txBuffCount, sizeof(trBuff_t));
    ASSERT(ctx.trBuff_p != nullptr, "TX BUFF MEM. ALLOC.");

    uint8* pointer;

    for (uint8 buffNum = 0; buffNum < ctx.config.txBuffCount; buffNum++) {
        pointer = (uint8*)calloc(ctx.config.txBuffSize, sizeof(uint8));
        ctx.trBuff_p[buffNum].buff_p = pointer;
        ASSERT(ctx.trBuff_p[buffNum].buff_p != nullptr, "TX BUFF MEM. ALLOC.");
    };

    //-----------------------------------------------------------------------------------

    ctx.timerTxTimeout.Config(bareOs::Timer::ModeAutoOff, bareOs::Timer::UnitUs, 0);
    usartBus_p[ctx.config.usartNum] = this;
    ctx.status = Status::OK;
    ctx.mode = MODE_OFF;
    return Exec::OK;
}

Exec USART::Handler()
{
    CHECK_STATUS();

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
            if (ctx.timerTxTimeout.GetState() == State::ON) {
                ctx.timerTxTimeout.Disable();
            }
            if (ctx.txEnabled == false && ctx.rxEnabled == false) {
                UsartDisable();
                break;
            }

            ctx.trBuffWaiting = FindWaitingTrBuff();
            if (ctx.trBuffWaiting) TriggerTx(ctx.trBuffWaiting);
            break;
        }
        case MODE_BUSY:
        {
            ctx.timerTemp = ctx.timerTxTimeout;
            if (ctx.timerTemp.Check()) {
                TxDisable();
                UsartDisable();
                RET_ERR("TX DATA TIMEOUT");
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

Exec USART::TxEnable()
{
    CHECK_STATUS();
    CHECK_PINCFG(ctx.txdPinCfgOk);
    if (ctx.usartEnabled == false) UsartEnable();
    ctx.usartReg_p->CR1.bit.TE = 1;
    ctx.txEnabled = true;
    return Exec::OK;
}

Exec USART::TxDisable()
{
    CHECK_STATUS();
    CHECK_PINCFG(ctx.txdPinCfgOk);
    ctx.usartReg_p->CR1.bit.TE = 0;
    ctx.txEnabled = false;
    return Exec::OK;
}

Exec USART::RxEnable()
{
    CHECK_STATUS();
    CHECK_PINCFG(ctx.rxdPinCfgOk);
    if (ctx.usartEnabled == false) UsartEnable();
    ctx.usartReg_p->CR1.bit.RE = true;          // Enable receiver
    ctx.rxEnabled = true;
    ctx.usartReg_p->CR1.bit.RXFNEIE = true;     // Enable rx fifo not empty interrupt
    return Exec::OK;
}

Exec USART::RxDisable()
{
    CHECK_STATUS();
    CHECK_PINCFG(ctx.rxdPinCfgOk);
    ctx.usartReg_p->CR1.bit.RXFNEIE = false;    // Disable rx fifo not empty interrupt
    ctx.usartReg_p->CR1.bit.RE = false;         // Disable receiver
    ctx.rxEnabled = false;
    return Exec::OK;
}

Exec USART::ChangeRxClbk(hal::irqUsart::RxfneClbk_t Clbk)
{
    CHECK_STATUS();
    ASSERT(ctx.config.useRxdLine, "RX LINE NOT IN USE");
    ASSERT(ctx.rxEnabled == false, "RX ENABLED");
    CHECK_EXEC(hal::irqUsart::SetRxfneClbk(ctx.config.usartNum, Clbk));
    ctx.config.RxDataClbk = Clbk;
    return Exec::OK;
}

uint32 USART::GetBaudrate()
{
    return ctx.config.baudrate;
}

Status USART::GetStatus()
{
    return ctx.status;
}

Exec USART::TxData(uint8* txSrc_p, uint16 txSize, bool waitTillCmp, trId_t* trId_p)
{
    CHECK_STATUS();
    ASSERT(ctx.txEnabled != false, "TXD LINE DISABLED");
    ASSERT(txSrc_p != nullptr, "TX DATA SRC POINTER");
    ASSERT(txSize > 0, "TX DATA SIZE IS ZERO");
    ASSERT(txSize <= ctx.config.txBuffSize, "TX DATA TO BIG FOR TX BUFF");

    //-----------------------------------------------------------------------------------
    // Find free transaction buff

    trBuff_t* trBuffFree_p;
    trBuffFree_p = FindFreeTrBuff();

    if (trBuffFree_p == nullptr) {
        do {
            CHECK_EXEC(Handler());
            trBuffFree_p = FindFreeTrBuff();
        } while (trBuffFree_p == nullptr);
    }

    //-----------------------------------------------------------------------------------
    // Setup transaction details

    trBuffFree_p->buffPos = 0;
    trBuffFree_p->txDataSize = txSize;
    trBuffFree_p->trId = ++ctx.trId;
    memcpy(trBuffFree_p->buff_p, txSrc_p, txSize);
    trBuffFree_p->txTimeoutUs = (uint32)(((uint64)ctx.byteTxTimeNs * txSize * TX_TIMEOUT_FACTOR) / NS_PER_US);
    trBuffFree_p->trStatus = TR_STAT_NEW;
    ctx.buffsBusy++;

    if (trId_p) *trId_p = trBuffFree_p->trId;

    //-----------------------------------------------------------------------------------
    // Trigger instant TX or wait till idle state

    if (ctx.mode == MODE_IDLE) TriggerTx(trBuffFree_p);

    if (waitTillCmp == true) {
        do {
            CHECK_EXEC(Handler());
        } while (ctx.mode == MODE_BUSY);
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

/******************************************************************************************************************************************
                                                         PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

void USART::IrqSetPrio(hal::irqPrio_t irqPrio)
{
    hal::core::IrqSetPrio(ctx.usartIrqId, irqPrio);
}

void USART::IrqEnable()
{
    ctx.irqEnabled = true;
    hal::core::IrqEnable(ctx.usartIrqId);
}

void USART::IrqDisable()
{
    hal::core::IrqDisable(ctx.usartIrqId);
    ctx.irqEnabled = false;
}

void USART::UsartEnable()
{
    ctx.usartReg_p->CR1.bit.UE = 1;     // Usart enable
    IrqEnable();
    ctx.mode = MODE_IDLE;
    ctx.usartEnabled = true;
}

void USART::UsartDisable()
{
    ctx.usartReg_p->CR1.bit.UE = 0;     // Usart disable
    ctx.mode = MODE_OFF;
    ctx.usartEnabled = false;
}

void USART::TriggerTx(trBuff_t* trBuff_fp)
{
	//-----------------------------------------------------------------------------------
	// Setup TX transaction

    ctx.trBuffRun_p = trBuff_fp;
    ctx.mode = MODE_BUSY;
    ctx.trBuffRun_p->trStatus = TR_STAT_TXRUN;
    ctx.timerTxTimeout.ChangeInterval(ctx.trBuffRun_p->txTimeoutUs);
    ctx.timerTxTimeout.Enable(true, false);

	//-----------------------------------------------------------------------------------
	// Trigger TX

    ctx.usartReg_p->CR1.bit.TXFNFIE = 1;    // Enable TX fifo not full interrupt
}

USART::trBuff_t* USART::FindFreeTrBuff()
{
    uint8 buffNum;
    uint8 buffEnd;

    if (ctx.trBuffRun_p == nullptr) {
        buffNum = 0;
        buffEnd = ctx.config.txBuffCount - 1;
    }
    else {
        buffEnd = (uint8)(ctx.trBuffRun_p - ctx.trBuff_p);
        buffNum = (uint8)((buffEnd + 1) % ctx.config.txBuffCount);
    }

    do {
        if ((ctx.trBuff_p[buffNum].trStatus == TR_STAT_NONE) ||
            (ctx.trBuff_p[buffNum].trStatus == TR_STAT_TXDONE)) return &ctx.trBuff_p[buffNum];
        buffNum += 1;
        buffNum %= ctx.config.txBuffCount;
    } while (buffNum != buffEnd);

    return nullptr;
}

USART::trBuff_t* USART::FindWaitingTrBuff()
{
    uint8 buffNum;
    uint8 buffEnd;

    if (ctx.trBuffRun_p == nullptr) {
        buffNum = 0;
        buffEnd = ctx.config.txBuffCount - 1;
    }
    else {
        buffEnd = (uint8)(ctx.trBuffRun_p - ctx.trBuff_p);
        buffNum = (uint8)((buffEnd + 1) % ctx.config.txBuffCount);
    }

    do {
        if (ctx.trBuff_p[buffNum].trStatus == TR_STAT_NEW) return &ctx.trBuff_p[buffNum];
        buffNum += 1;
        buffNum %= ctx.config.txBuffCount;
    } while (buffNum != buffEnd);

    return nullptr;
}

void IrqTxEmptyHandler(hal::usartNum_t usartNum)
{
    //-----------------------------------------------------------------------------------
    // Get USART class and UART register pointers

    USART::trBuff_t* trBuff_p = usartBus_p[usartNum]->ctx.trBuffRun_p;
    volatile REG_USART* usartReg_p = usartBus_p[usartNum]->ctx.usartReg_p;

    //-----------------------------------------------------------------------------------
    // Tx data

    do {
        // Load tx data byte into tx register
        usartReg_p->TDR.bit.TDR = trBuff_p->buff_p[trBuff_p->buffPos++];

        // All bytes have been sent? -> switch to tx ready interrupt
        if ((trBuff_p->buffPos) == trBuff_p->txDataSize) {
            usartReg_p->CR1.bit.TXFNFIE = 0;
            usartReg_p->CR1.bit.TCIE = 1;
            return;
        }
    } while(usartReg_p->ISR.bit.TXFNF);
}

void IrqTxReadyHandler(hal::usartNum_t usartNum)
{
    //-----------------------------------------------------------------------------------
    // Get USART class and UART register pointers

    USART::trBuff_t* trBuff_p = usartBus_p[usartNum]->ctx.trBuffRun_p;
    volatile REG_USART* usartReg_p = usartBus_p[usartNum]->ctx.usartReg_p;

    //-----------------------------------------------------------------------------------
    // Finish current transaction

    usartReg_p->CR1.bit.TCIE = 0;
    trBuff_p->trStatus = USART::TR_STAT_TXDONE;
    usartBus_p[usartNum]->ctx.buffsBusy--;

    //-----------------------------------------------------------------------------------
    // Check if there is a new transaction waiting

    trBuff_p = usartBus_p[usartNum]->FindWaitingTrBuff();

    if (trBuff_p != nullptr) {
        usartBus_p[usartNum]->TriggerTx(trBuff_p);
    }
    else {
        usartBus_p[usartNum]->ctx.mode = MODE_IDLE;
    }
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::usart'
