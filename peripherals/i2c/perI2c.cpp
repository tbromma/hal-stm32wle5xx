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
// CREATION DATE:    May 2023
// DESCRIPTION:      Low level layer for the I2C peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "perI2c.h"
#include "irqI2c.h"
#include "perGpio.h"
#include "irqList.h"
#include "halDelay.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/

namespace hal::i2c {

#undef CODE_LABEL
#undef CODE_STATUS
#define CODE_LABEL  ctx.config.label_p
#define CODE_STATUS ctx.status

static constexpr uint8 I2C_BAUD_MAX_ERR = 5;            // 5%
static constexpr uint32 I2C_MIN_BAUD_Hz = 10000;        // 10 kHz
static constexpr uint32 I2C_MAX_BAUD_FM_Hz = 400000;    // I2C fast mode max baudrate (400kHz)
static constexpr uint32 I2C_MAX_BAUD_FMP_Hz = 1000000;  // I2C fast mode plus max baudrate (1MHz)
static constexpr uint8 PRESC_MAX_VAL = 16;
static constexpr uint16 SCLH_MAX_VAL = 256;
static constexpr uint16 SCLL_MAX_VAL = 256;
static constexpr uint8 TXRX_TIMEOUT_FACTOR = 10;        // TX-RX timeout safety factor
static constexpr uint32 MS_PER_SEC = 1000;              // Miliseconds per second
static constexpr uint32 US_PER_SEC = 1000000;           // Microseconds per second
static constexpr uint32 NS_PER_SEC = 1000000000;        // Nanoseconds per second
static constexpr uint8 BITS_PER_BYTE = 10;              // 1 start bit + 8 data bits + 1 stop bit

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec MASTER::Setup(config_t config)
{
    ctx.status = Status ::INIT;
    ctx.config = config;

    //-----------------------------------------------------------------------------------
    // Check config

    ASSERT(ctx.config.sclPinPort != hal::PORT_none, "SCL PORT NUMBER");
    ASSERT(ctx.config.sclPinNum != hal::PIN_none, "SCL PIN NUMBER");
    ASSERT(ctx.config.sclPinFunc != hal::FUNC_none, "SCL PIN ALT. FUNC.");

    ASSERT(ctx.config.sdaPinPort != hal::PORT_none, "SDA PORT NUMBER");
    ASSERT(ctx.config.sdaPinNum != hal::PIN_none, "SDA PIN NUMBER");
    ASSERT(ctx.config.sdaPinFunc != hal::FUNC_none, "SDA PIN ALT. FUNC.");

    ASSERT(ctx.config.txBuffCount > 0, "TX BUFF COUNT IS ZERO");
    ASSERT(config.irqPrio != hal::IRQ_PRIO_none, "IRQ PRIO");
    ASSERT(ctx.config.baudrate >= I2C_MIN_BAUD_Hz, "I2C BAUDRATE TO LOW (MIN SPEED %dHz)", I2C_MIN_BAUD_Hz);
    ASSERT_COND(ctx.config.fastModePlus == false, ctx.config.baudrate <= I2C_MAX_BAUD_FM_Hz,
                "I2C BAUDRATE TO HIGH (MAX SPEED IN FAST MODE %dHz)", I2C_MAX_BAUD_FM_Hz);
    ASSERT_COND(ctx.config.fastModePlus == true, ctx.config.baudrate <= I2C_MAX_BAUD_FMP_Hz,
                "I2C BAUDRATE TO HIGH (MAX SPEED IN FAST MODE PLUS %dHz)", I2C_MAX_BAUD_FMP_Hz);

    //-----------------------------------------------------------------------------------
    // Get I2C peripheral base address

    if (config.i2cNum == hal::I2C_1) {
        ctx.i2cReg_p = REG_I2C1;
        ctx.i2cPerId = hal::PER_I2C1;
        ctx.i2cIrqEvId = hal::IRQ_I2C1_EV;
        ctx.i2cIrqErId = hal::IRQ_I2C1_ER;
    }
    else if (config.i2cNum == hal::I2C_2) {
        ctx.i2cReg_p = REG_I2C2;
        ctx.i2cPerId = hal::PER_I2C2;
        ctx.i2cIrqEvId = hal::IRQ_I2C2_EV;
        ctx.i2cIrqErId = hal::IRQ_I2C2_ER;
    }
    else if (config.i2cNum == hal::I2C_3) {
        ctx.i2cReg_p = REG_I2C3;
        ctx.i2cPerId = hal::PER_I2C3;
        ctx.i2cIrqEvId = hal::IRQ_I2C3_EV;
        ctx.i2cIrqErId = hal::IRQ_I2C3_ER;
    }

    //-----------------------------------------------------------------------------------
    // Setup clock's

    hal::clk::PerClkEnable(ctx.i2cPerId);
    hal::clk::I2cClkSrcSelect(ctx.config.i2cNum, ctx.config.clkSrc);

    //-----------------------------------------------------------------------------------
    // Calculate prescaler value for selected baudrate

    uint8 prescCheck;
    uint8 presc = 0;
    uint16 scllCheck;
    uint16 sclhCheck;
    uint16 scll = 0;
    uint16 sclh = 0;
    uint32 clkSrcFreqHz;
    uint32 baud;
    uint32 baudReal;
    uint32 baudErr;
    uint32 baudErrMin = UINT32_MAX;
    uint16 baudErrPercent;

    clkSrcFreqHz = hal::clk::I2cClkGetFreqHz(ctx.config.i2cNum);
    ASSERT(clkSrcFreqHz > 0, "SELECTED I2C CLOCK SOURCE IS DISABLED");

    for (prescCheck = 1; prescCheck < PRESC_MAX_VAL; prescCheck++) {
        scllCheck = 1;
        sclhCheck = 1;
        while ((scllCheck <= SCLL_MAX_VAL) && (sclhCheck <= SCLH_MAX_VAL)) {
            baud = clkSrcFreqHz / (prescCheck * (scllCheck + sclhCheck));
            baudErr = abs((int32)baud - (int32)ctx.config.baudrate);

            if (baudErr < baudErrMin) {
                presc = prescCheck;
                scll = scllCheck;
                sclh = sclhCheck;
                baudErrMin = baudErr;
                baudErrPercent = (uint16)((baudErr * 100) / ctx.config.baudrate);
            }
            scllCheck++;
            sclhCheck++;
        }
    }

    ASSERT(baudErrPercent <= I2C_BAUD_MAX_ERR, "BAUDRATE ERROR TO HIGH");

    baudReal = clkSrcFreqHz / (presc * (scll + sclh));
    ctx.byteTxRxTimeNs = (NS_PER_SEC / baudReal) * BITS_PER_BYTE;

    ctx.i2cReg_p->TIMINGR.bit.PRESC = (presc - 1) & 0b1111;
    ctx.i2cReg_p->TIMINGR.bit.SCLL = (uint8)(scll - 1);
    ctx.i2cReg_p->TIMINGR.bit.SCLH = (uint8)(sclh - 1);
    ctx.i2cReg_p->TIMINGR.bit.SDADEL = 0b1111;
    ctx.i2cReg_p->TIMINGR.bit.SCLDEL = 0b1111;

    //-----------------------------------------------------------------------------------
    // Config I2C peripheral registers

    I2C_CR1 REG_CR1 = {};
    REG_CR1.bit.PE = true;                      // Peripheral enable
    REG_CR1.bit.NACKIE = true;                  // Not acknowledge (NACKF) received interrupts enabled
    REG_CR1.bit.ERRIE = true;                   // Error detection interrupts enabled
    REG_CR1.bit.ANFOFF = false;                 // Analog noise filter enabled
    ctx.i2cReg_p->CR1.val = REG_CR1.val;

    ctx.i2cReg_p->CR2.bit.AUTOEND = false;      // Automatic end mode: a STOP condition is automatically sent when NBYTES data are transferred.

    //-----------------------------------------------------------------------------------
    // Config I2C pins

    hal::GPIO::EnablePortClk(ctx.config.sclPinPort);
    hal::GPIO::SetPinOutType(ctx.config.sclPinPort, ctx.config.sclPinNum, hal::GPIO::OutType::OpenDrain);
    hal::GPIO::SetPinMode(ctx.config.sclPinPort, ctx.config.sclPinNum, hal::GPIO::PinMode::AltFunc);
    hal::GPIO::SetPinAltFunc(ctx.config.sclPinPort, ctx.config.sclPinNum, ctx.config.sclPinFunc);

    hal::GPIO::EnablePortClk(ctx.config.sdaPinPort);
    hal::GPIO::SetPinOutType(ctx.config.sdaPinPort, ctx.config.sdaPinNum, hal::GPIO::OutType::OpenDrain);
    hal::GPIO::SetPinMode(ctx.config.sdaPinPort, ctx.config.sdaPinNum, hal::GPIO::PinMode::AltFunc);
    hal::GPIO::SetPinAltFunc(ctx.config.sdaPinPort, ctx.config.sdaPinNum, ctx.config.sdaPinFunc);

    // Mark pin usage
    hal::GPIO::SetPinInUse(ctx.config.sclPinPort, ctx.config.sclPinNum, "%s %s", ctx.config.label_p, "SCL");
    hal::GPIO::SetPinInUse(ctx.config.sdaPinPort, ctx.config.sdaPinNum, "%s %s", ctx.config.label_p, "SDA");

    //-----------------------------------------------------------------------------------
    // Config I2C interrupts

    CHECK_EXEC(hal::irqI2c::SetEvtIrqClbk(ctx.config.i2cNum, IrqEvtCallback));
    CHECK_EXEC(hal::irqI2c::SetErrIrqClbk(ctx.config.i2cNum, IrqErrCallback));

    hal::core::IrqSetPrio(ctx.i2cIrqEvId, ctx.config.irqPrio);
    hal::core::IrqSetPrio(ctx.i2cIrqErId, ctx.config.irqPrio);

    hal::irq::SetIrqInUse(ctx.i2cIrqEvId, ctx.config.label_p);
    hal::irq::SetIrqInUse(ctx.i2cIrqErId, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Allocate memory for I2C transaction buffers

    ctx.trBuff_p = (trBuff_t*)calloc(config.txBuffCount, sizeof(trBuff_t));
    ASSERT(ctx.trBuff_p != nullptr, "TX BUFF");

    for (uint8 buffNum = 0; buffNum < config.txBuffCount; buffNum++) {
        uint8* pointer = (uint8*)calloc(config.txBuffSize, sizeof(uint8));
        ctx.trBuff_p[buffNum].buff_p = pointer;
        ASSERT(ctx.trBuff_p[buffNum].buff_p != nullptr, "TX BUFF");
    };

    //-----------------------------------------------------------------------------------

    ctx.timerTimeout.Config(bareOs::Timer::ModeAutoOff, bareOs::Timer::UnitUs);
    i2cBusClass_p[config.i2cNum] = this;
    ctx.status = Status::OK;
    ctx.mode = MODE_IDLE;
    return Exec::OK;
}

Exec MASTER::Handler(void)
{
    switch(ctx.mode) {
        case MODE_NONE: {
            REPORT_FAT("MODE NONE");
        } break;

        case MODE_IDLE: {
            if (ctx.timerTimeout.GetState() == State::ON) {
                ctx.timerTimeout.Disable();
            }
            auto trBuffWaiting_p = FindWaitingTrBuff();
            if (trBuffWaiting_p) TriggerTransaction(trBuffWaiting_p);
        } break;

        case MODE_BUSY: {
            if (CheckForTimeout()) {
                IrqDisable();
                REPORT_ERR("I2C transaction timeout! Slave Address -> 0x%02X", ctx.trBuffRun_p->slaveAddr);
                ctx.mode = MODE_ERR;
            }
        } break;

        case MODE_ERR: {
            if (ctx.irqErrFlag) {
                ctx.irqErrFlag = false;
                REPORT_ERR(ctx.irqErrStr);
            }
            ctx.i2cReg_p->CR1.bit.PE = 0;
            DelayMs(1);
            ctx.i2cReg_p->CR1.bit.PE = 1;
            ctx.trBuffRun_p->trStatus = TR_STAT_ERR;
            ctx.mode = MODE_IDLE;
            return Exec::ERR;
        } break;
    }
    return Exec::OK;
}

Exec MASTER::TxData(slaveAddr_t addr, uint8* txSrc_p, uint8 txSize, bool wait, uint64* trId_p)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(addr, txSrc_p, txSize, nullptr, 0, wait, trId_p));
}

Exec MASTER::RxData(slaveAddr_t addr, uint8* rxDest_p, uint8 rxSize, bool wait, uint64* trId_p)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(addr, nullptr, 0, rxDest_p, rxSize, wait, trId_p));
}

Exec MASTER::TxRxData(slaveAddr_t addr, uint8* txSrc_p, uint8 txSize, uint8* rxDest_p, uint8 rxSize, bool wait, uint64* trId_p)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(addr, txSrc_p, txSize, rxDest_p, rxSize, wait, trId_p));
}

uint32 MASTER::GetBaudrateHz()
{
    return ctx.config.baudrate;
}

Status MASTER::GetStatus()
{
    return ctx.status;
}

mode_t MASTER::GetMode()
{
    return ctx.mode;
}

/******************************************************************************************************************************************
                                                         PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

void MASTER::IrqEnable()
{
    hal::core::IrqEnable(ctx.i2cIrqEvId);
    hal::core::IrqEnable(ctx.i2cIrqErId);
}

void MASTER::IrqDisable()
{
    hal::core::IrqDisable(ctx.i2cIrqEvId);
    hal::core::IrqDisable(ctx.i2cIrqErId);
}

Exec MASTER::SetupTransaction(slaveAddr_t addr, uint8* txSrc_p, uint8 txSize, uint8* rxDest_p, uint8 rxSize, bool wait, uint64* trId_p)
{
    ASSERT((txSize > 0) || (rxSize > 0), "TX & RX DATA SIZE IS ZERO");
    ASSERT_COND(txSize > 0, txSrc_p != nullptr, "VARIABLE 'txSrc_p'");
    ASSERT_COND(rxSize > 0, rxDest_p != nullptr, "VARIABLE 'rxDest_p'");

    //-----------------------------------------------------------------------------------
    // Find free transaction buff

    trBuff_t* trBuffFree_p;
    trBuffFree_p = FindFreeTrBuff();

    if (trBuffFree_p == nullptr) {
        REPORT_WAR("NO FREE BUFF");
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

    trBuffFree_p->slaveAddr = addr;
    trBuffFree_p->txPos = 0;
    trBuffFree_p->rxPos = 0;
    trBuffFree_p->txSize = txSize;
    trBuffFree_p->rxSize = rxSize;
    trBuffFree_p->rxDest_p = rxDest_p;
    memcpy(trBuffFree_p->buff_p, txSrc_p, txSize);
    trBuffFree_p->trId = ++ctx.trId;
    if (txSize > 0) trBuffFree_p->timeoutUs = ((txSize + 1) * ctx.byteTxRxTimeNs * TXRX_TIMEOUT_FACTOR) / MS_PER_SEC;   // + 1 for I2C address
    if (rxSize > 0) trBuffFree_p->timeoutUs += ((rxSize + 1) * ctx.byteTxRxTimeNs * TXRX_TIMEOUT_FACTOR) / MS_PER_SEC;  // + 1 for I2C address

    trBuffFree_p->trStatus = TR_STAT_NEW;
    if (trId_p) *trId_p = trBuffFree_p->trId;
    ctx.buffsBusy++;

    //-----------------------------------------------------------------------------------
    // Trigger instant RX or wait till idle state

    if (ctx.mode == MODE_IDLE) TriggerTransaction(trBuffFree_p);

    if (wait == true) {
        do {
            CHECK_EXEC(Handler());
        } while ((ctx.mode == MODE_BUSY) || (ctx.mode == MODE_ERR));
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

void MASTER::TriggerTransaction(trBuff_t* trBuff_fp)
{
    ctx.trBuffRun_p = trBuff_fp;
    ctx.startTimerTimeout = true;
    ctx.i2cReg_p->CR2.bit.SADD = (ctx.trBuffRun_p->slaveAddr << 1) & 0b1111111111;      // Set slave address
    ctx.mode = MODE_BUSY;

    if ((ctx.trBuffRun_p->trType == TR_TYPE_TX) || (ctx.trBuffRun_p->trType == TR_TYPE_TXRX)) {
        ctx.i2cReg_p->CR2.bit.RDWRN = 0;                                                // Set write operation type
        ctx.i2cReg_p->CR2.bit.NBYTES = ctx.trBuffRun_p->txSize;
        ctx.trBuffRun_p->trStatus = TR_STAT_TXRUN;
        ctx.i2cReg_p->CR1.bit.TXIE = 1;                                                 // Enable TX interrupt

    }
    else {
        ctx.i2cReg_p->CR2.bit.RDWRN = 1;                                                // Set read operation type
        ctx.i2cReg_p->CR2.bit.NBYTES = ctx.trBuffRun_p->rxSize;
        ctx.trBuffRun_p->trStatus = TR_STAT_RXRUN;
        ctx.i2cReg_p->CR1.bit.RXIE = 1;                                                 // Enable  RX Ready interrupt
    }

    ctx.i2cReg_p->CR2.bit.START = 1;                                                    // Generate start condition
    IrqEnable();
}

MASTER::trBuff_t* MASTER::FindFreeTrBuff()
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
            (ctx.trBuff_p[buffNum].trStatus == TR_STAT_DONE) ||
            (ctx.trBuff_p[buffNum].trStatus == TR_STAT_ERR)) return &ctx.trBuff_p[buffNum];
        buffNum += 1;
        buffNum %= ctx.config.txBuffCount;
    } while (buffNum != buffEnd);

    return nullptr;
}

MASTER::trBuff_t* MASTER::FindWaitingTrBuff()
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

bool MASTER::CheckForTimeout()
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

void MASTER::IrqEvtCallback(hal::i2cNum_t i2cNum)
{
    MASTER* i2cBus_p = i2cBusClass_p[i2cNum];
    volatile REG_I2C* i2cReg_p = i2cBusClass_p[i2cNum]->ctx.i2cReg_p;

    I2C_ISR ISR_REG;
    I2C_CR1 CR1_REG;
    ISR_REG.val = i2cReg_p->ISR.val;
    CR1_REG.val = i2cReg_p->CR1.val;
    if (CR1_REG.bit.ERRIE & ISR_REG.bit.NACKF)  IrqErrHandler(i2cBus_p, i2cReg_p, MASTER::IRQERR_NACK);
    if (CR1_REG.bit.RXIE & ISR_REG.bit.RXNE)    IrqHandlerRxne(i2cBus_p, i2cReg_p);
    if (CR1_REG.bit.TCIE & ISR_REG.bit.TC)      IrqHandlerTc(i2cBus_p, i2cReg_p);
    if (CR1_REG.bit.TXIE & ISR_REG.bit.TXE)     IrqHandlerTxe(i2cBus_p, i2cReg_p);
}

void MASTER::IrqErrCallback(hal::i2cNum_t i2cNum)
{
    MASTER* i2cBus_p = i2cBusClass_p[i2cNum];
    volatile REG_I2C* i2cReg_p = i2cBusClass_p[i2cNum]->ctx.i2cReg_p;

    I2C_ISR ISR_REG;
    I2C_CR1 CR1_REG;
    ISR_REG.val = i2cReg_p->ISR.val;
    CR1_REG.val = i2cReg_p->CR1.val;
    if (CR1_REG.bit.ERRIE & ISR_REG.bit.BERR)   IrqErrHandler(i2cBus_p, i2cReg_p, MASTER::IRQERR_BERR);
    if (CR1_REG.bit.ERRIE & ISR_REG.bit.ARLO)   IrqErrHandler(i2cBus_p, i2cReg_p, MASTER::IRQERR_ARLO);
    if (CR1_REG.bit.ERRIE & ISR_REG.bit.OVR)    IrqErrHandler(i2cBus_p, i2cReg_p, MASTER::IRQERR_OVR);
}

void MASTER::IrqHandlerTxe(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p)
{
    //-----------------------------------------------------------------------------------
    // Load tx data byte into tx register

    MASTER::trBuff_t* trBuff_p = i2cBus_p->ctx.trBuffRun_p;

    i2cReg_p->TXDR.bit.TXDATA = trBuff_p->buff_p[trBuff_p->txPos++];

    if (trBuff_p->txPos == trBuff_p->txSize) {
        i2cReg_p->CR1.bit.TXIE = 0;
        i2cReg_p->CR1.bit.TCIE = 1;
    }
}

void MASTER::IrqHandlerTc(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p)
{
    MASTER::trBuff_t* trBuff_p = i2cBus_p->ctx.trBuffRun_p;

    //-----------------------------------------------------------------------------------
    // Finish current tx transaction

    i2cReg_p->CR1.bit.TCIE = 0;                                         // Disable Transmit Complete Interrupt

    if (trBuff_p->trType == MASTER::TR_TYPE_TX) {
        i2cReg_p->CR2.bit.STOP = 1;                                     // Send stop
        trBuff_p->trStatus = MASTER::TR_STAT_DONE;
        i2cBus_p->ctx.buffsBusy--;
    }
    else if (trBuff_p->trType == MASTER::TR_TYPE_TXRX) {
        i2cReg_p->CR2.bit.RDWRN = 1;                                    // Set read operation type
        i2cReg_p->CR2.bit.NBYTES = trBuff_p->rxSize;                    // Set read operation byte count
        i2cReg_p->CR2.bit.START = 1;
        trBuff_p->trStatus = MASTER::TR_STAT_RXRUN;
        i2cReg_p->CR1.bit.RXIE = 1;                                     // Enable  Rx Ready interrupt
        return;
    }

    //-----------------------------------------------------------------------------------
    // Check if there is a new transaction waiting

    trBuff_p = i2cBus_p->FindWaitingTrBuff();

    if (trBuff_p != nullptr) {
        i2cBus_p->TriggerTransaction(trBuff_p);
    }
    else {
        i2cBus_p->IrqDisable();
        i2cBus_p->ctx.mode = MODE_IDLE;
    }
}

void MASTER::IrqHandlerRxne(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p)
{
    MASTER::trBuff_t* trBuff_p = i2cBus_p->ctx.trBuffRun_p;

    //-----------------------------------------------------------------------------------
    // Generate stop condition?

    if (trBuff_p->rxSize == (trBuff_p->rxPos + 1)) {
        i2cReg_p->CR2.bit.STOP = 1;
    }

    //-----------------------------------------------------------------------------------
    // Read rx byte

    trBuff_p->rxDest_p[trBuff_p->rxPos++] = (uint8)i2cReg_p->RXDR.bit.RXDATA;

    //-----------------------------------------------------------------------------------
    // All bytes received?

    if (trBuff_p->rxPos == trBuff_p->rxSize) {
        i2cReg_p->CR1.bit.RXIE = 0;
        trBuff_p->trStatus = MASTER::TR_STAT_DONE;
        i2cBus_p->ctx.buffsBusy--;
    }

    //-----------------------------------------------------------------------------------
    // Check if there is a new transaction waiting

    if (trBuff_p->trStatus == MASTER::TR_STAT_DONE) {
        trBuff_p = i2cBus_p->FindWaitingTrBuff();
        if (trBuff_p != nullptr)    i2cBus_p->TriggerTransaction(trBuff_p);
        else {
            i2cBus_p->IrqDisable();
            i2cBus_p->ctx.mode = MODE_IDLE;
        }
    }
}

void MASTER::IrqErrHandler(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p, MASTER::irqErr_t irqErr)
{
    i2cReg_p->CR1.bit.RXIE = 0;
    i2cReg_p->CR1.bit.TXIE = 0;
    i2cReg_p->CR1.bit.TCIE = 0;
    i2cBus_p->ctx.trBuffRun_p->trStatus = MASTER::TR_STAT_ERR;
    i2cBus_p->ctx.buffsBusy--;

    if (irqErr == MASTER::IRQERR_NACK) {
        i2cReg_p->ICR.bit.NACKCF = 1;
        i2cBus_p->ctx.irqErrStr = "NACK RECEIVED";
    }
    else if (irqErr == MASTER::IRQERR_ARLO) {
        i2cReg_p->ICR.bit.ARLOCF = 1;
        i2cBus_p->ctx.irqErrStr = "ARBITRATION LOST";
    }
    else if (irqErr == MASTER::IRQERR_BERR) {
        i2cReg_p->ICR.bit.BERRCF = 1;
        i2cBus_p->ctx.irqErrStr = "BUS ERROR DETECTED";
    }
    else if (irqErr == MASTER::IRQERR_OVR) {
        i2cReg_p->ICR.bit.OVRCF = 1;
        i2cBus_p->ctx.irqErrStr = "OVERRUN";
    }

    i2cBus_p->ctx.irqErrFlag = true;
    i2cBus_p->ctx.irqErrType = irqErr;
    i2cBus_p->ctx.mode = MODE_ERR;
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::i2c'
