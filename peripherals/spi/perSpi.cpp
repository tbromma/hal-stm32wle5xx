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
// DESCRIPTION:      Low level layer for the SPI1 & SPI2 peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "mcuCore.h"
#include "perSpi.h"
#include "irqSpi.h"
#include "sysclk.h"
#include "irqList.h"
#include "utils.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal {

#undef CODE_LABEL
#undef CODE_STATUS
#undef ACTION_ON_ERR
#define CODE_LABEL      ctx.config.label_p
#define CODE_STATUS     ctx.status
#define ACTION_ON_ERR   ctx.status = Status::ERR; ChangeState(State::ERR)

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

// GPIO-OUT: Digital OUT3
constexpr hal::GPIO::OUT::config_t digOut3 = {
    .pinPort    = hal::PORT_A,
    .pinNum     = hal::PIN_2,
    .activeLvl  = hal::GPIO::ActiveLvl::High,
    .outType    = hal::GPIO::OutType::PushPull,
    .speed      = hal::GPIO::OutSpeed::Slow,
    .pullDir    = hal::GPIO::PullDir::None,
    .label_p    = "GPIO DIGOUT3",
    .enable     = true,
};

Exec SPI_M::Setup(config_t config)
{
    ctx.status = Status ::INIT;
    ctx.config = config;

    //-----------------------------------------------------------------------------------
    // Check config

    ASSERT(ctx.config.spiNum != hal::SPI_none, "SPI PERIPHERAL NUMBER");
    ASSERT(ctx.config.spiNum != hal::SPI3_RF, "SPI RF HANDELED BY A SEPERATE HAL LIB");
    ASSERT(ctx.config.baudrate != Baudrate::none, "SPI BAUDRATE");

    ASSERT(ctx.config.misoPinPort != hal::PORT_none, "MISO PIN PORT");
    ASSERT(ctx.config.misoPinNum != hal::PIN_none, "MISO PIN NUMBER");
    ASSERT(ctx.config.misoPinFunc != hal::FUNC_none, "MISO PIN ALT. FUNC.");

    ASSERT(ctx.config.mosiPinPort != hal::PORT_none, "MOSI PIN PORT");
    ASSERT(ctx.config.mosiPinNum != hal::PIN_none, "MOSI PIN NUMBER");
    ASSERT(ctx.config.mosiPinFunc != hal::FUNC_none, "MOSI PIN ALT. FUNC.");

    ASSERT(ctx.config.sckPinPort != hal::PORT_none, "SCK PIN PORT");
    ASSERT(ctx.config.sckPinNum != hal::PIN_none, "SCK PIN NUMBER");
    ASSERT(ctx.config.sckPinFunc != hal::FUNC_none, "SCK PIN ALT. FUNC.");

    ASSERT(ctx.config.irqPrio != hal::IRQ_PRIO_none, "IRQ PRIORITY");

    //-----------------------------------------------------------------------------------
    // Get SPI peripheral base address

    if (config.spiNum == hal::SPI1) {
        ctx.spiReg_p = REG_SPI1;
        ctx.spiPerId = hal::PER_SPI1;
        ctx.spiIrqId = hal::IRQ_SPI1;
    }
    else {
        ctx.spiReg_p = REG_SPI2;
        ctx.spiPerId = hal::PER_SPI2;
        ctx.spiIrqId = hal::IRQ_SPI2;
    }

    //-----------------------------------------------------------------------------------
    // Enable peripheral clock

    hal::clk::PerClkEnable(ctx.spiPerId);

    //-----------------------------------------------------------------------------------
    // Config SPI peripheral registers & set baudrate

    ctx.spiReg_p->CR1.bit.MSTR = true;                              // Enable SPI master mode
    ctx.spiReg_p->CR1.bit.CPOL = ctx.config.clkPol;                 // Set clock polarity
    ctx.spiReg_p->CR1.bit.CPHA = ctx.config.clkPhase;               // Set clock phase
    ctx.spiReg_p->CR1.bit.LSBFIRST = false;                         // Data is transmitted-received with the MSB first
    ctx.spiReg_p->CR1.bit.SSM = true;                               // Slave select pin managed by software
    ctx.spiReg_p->CR1.bit.SSI = true;                               // Set slave select pin state high (deselect slave)
    ctx.spiReg_p->CR1.bit.RXONLY = false;                           // Full-duplex mode (transmit and receive)
    ctx.spiReg_p->CR1.bit.CRCEN = false;                            // CRC calculation disabled
    ctx.spiReg_p->CR1.bit.BIDIMODE = false;                         // 2-line unidirectional data mode selected

    ctx.spiReg_p->CR2.bit.NSSP = false;                             // No NSS pulse
    ctx.spiReg_p->CR2.bit.SSOE = true;                              // Slave select output enabled
    ctx.spiReg_p->CR2.bit.FRF = false;                              // SPI Motorola mode
    ctx.spiReg_p->CR2.bit.ERRIE = true;                             // Error interrupt is enabled
    ctx.spiReg_p->CR2.bit.DS = 0b0111;                              // Data size 8-bit
    ctx.spiReg_p->CR2.bit.FRXTH = true;                             // RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)

    SetBaudrate(ctx.config.baudrate);

    //-----------------------------------------------------------------------------------
    // Config SPI pins

    // MOSI pin
    hal::GPIO::EnablePortClk(ctx.config.mosiPinPort);
    hal::GPIO::SetPinOutSpeed(ctx.config.mosiPinPort, ctx.config.mosiPinNum, hal::GPIO::OutSpeed::High);
    hal::GPIO::SetPinMode(ctx.config.mosiPinPort, ctx.config.mosiPinNum, hal::GPIO::PinMode::AltFunc);
    hal::GPIO::SetPinAltFunc(ctx.config.mosiPinPort, ctx.config.mosiPinNum, ctx.config.mosiPinFunc);
    hal::GPIO::SetPinOutType(ctx.config.mosiPinPort, ctx.config.mosiPinNum, hal::GPIO::OutType::PushPull);
    hal::GPIO::SetPinOutLevel(ctx.config.mosiPinPort, ctx.config.mosiPinNum, true);

    // MISO pin
    hal::GPIO::EnablePortClk(ctx.config.misoPinPort);
    hal::GPIO::SetPinMode(ctx.config.misoPinPort, ctx.config.misoPinNum, hal::GPIO::PinMode::AltFunc);
    hal::GPIO::SetPinAltFunc(ctx.config.misoPinPort, ctx.config.misoPinNum, ctx.config.misoPinFunc);
    hal::GPIO::SetPinPullDir(ctx.config.misoPinPort, ctx.config.misoPinNum, hal::GPIO::PullDir::Gnd);

    // SCK pin
    hal::GPIO::EnablePortClk(ctx.config.sckPinPort);
    hal::GPIO::SetPinOutSpeed(ctx.config.sckPinPort, ctx.config.sckPinNum, hal::GPIO::OutSpeed::High);
    hal::GPIO::SetPinMode(ctx.config.sckPinPort, ctx.config.sckPinNum, hal::GPIO::PinMode::AltFunc);
    hal::GPIO::SetPinAltFunc(ctx.config.sckPinPort, ctx.config.sckPinNum, ctx.config.sckPinFunc);
    hal::GPIO::SetPinOutType(ctx.config.sckPinPort, ctx.config.sckPinNum, hal::GPIO::OutType::PushPull);

    // Mark pin usage
    hal::GPIO::SetPinInUse(ctx.config.sckPinPort, ctx.config.sckPinNum, "%s %s", ctx.config.label_p, "SCK");
    hal::GPIO::SetPinInUse(ctx.config.mosiPinPort, ctx.config.mosiPinNum, "%s %s", ctx.config.label_p, "MOSI");
    hal::GPIO::SetPinInUse(ctx.config.misoPinPort, ctx.config.misoPinNum, "%s %s", ctx.config.label_p, "MISO");

    //-----------------------------------------------------------------------------------
    // Config DMA

    if (ctx.config.useDma) SetupDma();

    //-----------------------------------------------------------------------------------
    // Config SPI interrupts

    CHECK_EXEC(hal::irqSpi::SetIrqClbk(ctx.config.spiNum, IrqHandlerClbk));
    hal::core::IrqSetPrio(ctx.spiIrqId, ctx.config.irqPrio);
    hal::irq::SetIrqInUse(ctx.spiIrqId, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Allocate memory for UART transaction buffers

    ctx.trBuff_p = (trBuff_t*)calloc(ctx.config.txBuffCnt, sizeof(trBuff_t));
    ASSERT(ctx.trBuff_p != nullptr, "TX BUFF");

    uint8* pointer;

    for (uint8 buffNum = 0; buffNum < ctx.config.txBuffCnt; buffNum++) {
        pointer = (uint8*)calloc(config.txBuffSize, sizeof(uint8));
        ctx.trBuff_p[buffNum].buff_p = pointer;
        ASSERT(ctx.trBuff_p[buffNum].buff_p != nullptr, "TX BUFF");
    };

    ctx.gpio3.Setup(digOut3);

    //-----------------------------------------------------------------------------------

    ctx.timerTimeout.Config(bareOs::Timer::ModeAutoOff, bareOs::Timer::UnitUs);
    spiMaster_p[ctx.config.spiNum] = this;
    DisableMosi();
    EnableSpi();
    ctx.status = Status::OK;
    return Exec::OK;
}

Exec SPI_M::Handler(void)
{
    switch(ctx.state) {
        case State::NONE: {
            REPORT_FAT("MODE NONE");
        }

        case State::IDLE: {
            if (ctx.timerTimeout.GetState() == State::ON) {
                ctx.timerTimeout.Disable();
            }
            auto trBuffWaiting_p = FindWaitingTrBuff();
            if (trBuffWaiting_p) TriggerTransaction(trBuffWaiting_p);

        } break;

        case State::BUSY: {
            if (CheckForTimeout()) {
                DisableSpi();
                RET_ERR("SPI transaction timeout! Slave ID: %d", ctx.trBuffRun_p->slaveId);
            }

        } break;

        case State::ERR:
        default: {
            return Exec::ERR;
        }
    }

    return Exec::OK;
}

Exec SPI_M::AddSlave(slave_t config)
{
    CHECK_STATUS();
    ASSERT(config.slaveId != SlaveId::none, "SLAVE ID");
    ASSERT(config.mode != SsMode::none, "SS LINE MODE");
    ASSERT(ctx.ssPin[(uint8)config.slaveId].GetStatus() == Status::NONE, "SLAVE ID%d", config.slaveId);
    hal::GPIO::OUT::config_t ssPinCfg = {};
    ssPinCfg.pinPort = config.pinPort;
    ssPinCfg.pinNum = config.pinNum;
    ssPinCfg.activeLvl = hal::GPIO::ActiveLvl::Low;
    ssPinCfg.outType = hal::GPIO::OutType::PushPull;
    ssPinCfg.speed = hal::GPIO::OutSpeed::High;
    ssPinCfg.pullDir = hal::GPIO::PullDir::None;
    ssPinCfg.label_p = config.label_p;
    ssPinCfg.enable = true;
    CHECK_EXEC(ctx.ssPin[(uint8)config.slaveId].Setup(ssPinCfg));
    ctx.slave[(uint8)config.slaveId] = config;
    return Exec::OK;
}

void SPI_M::EnableSpi()
{
    hal::clk::PerClkEnable(ctx.spiPerId);
    ctx.spiReg_p->CR1.bit.SPE = true;
    ctx.dummyRead16 = (uint16)ctx.spiReg_p->SR.val;
    ChangeState(State::IDLE);
    hal::core::IrqEnable(ctx.spiIrqId);
}

void SPI_M::DisableSpi()
{
    hal::core::IrqDisable(ctx.spiIrqId);
    ctx.spiReg_p->CR1.bit.SPE = false;
    hal::clk::PerClkDisable(ctx.spiPerId);
    ChangeState(State::OFF);
}

Exec SPI_M::TxData(SlaveId slaveId, uint8* txSrc_p, uint16 txSize, bool keepSsLow, uint64* trId_p, bool wait)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(slaveId, txSrc_p, txSize, nullptr, 0, keepSsLow, trId_p, nullptr, wait));
}

Exec SPI_M::RxData(SlaveId slaveId, uint8* rxDest_p, uint16 rxSize, bool keepSsLow, uint64* trId_p, bool wait)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(slaveId, nullptr, 0, rxDest_p, rxSize, keepSsLow, trId_p, nullptr, wait));
}

Exec SPI_M::TxRxData(SlaveId slaveId, uint8* txSrc_p, uint16 txSize, uint8* rxDest_p, uint16 rxSize, bool keepSsLow, uint64* trId_p, bool wait)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(slaveId, txSrc_p, txSize, rxDest_p, rxSize, keepSsLow, trId_p, nullptr, wait));
}

Exec SPI_M::TxDataClbk(SlaveId slaveId, uint8* txSrc_p, uint16 txSize, bool keepSsLow, Clbk_t Clbk)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(slaveId, txSrc_p, txSize, nullptr, 0, keepSsLow, nullptr, Clbk, false));
}

Exec SPI_M::RxDataClbk(SlaveId slaveId, uint8* rxDest_p, uint16 rxSize, bool keepSsLow, Clbk_t Clbk)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(slaveId, nullptr, 0, rxDest_p, rxSize, keepSsLow, nullptr, Clbk, false));
}

Exec SPI_M::TxRxDataClbk(SlaveId slaveId, uint8* txSrc_p, uint16 txSize, uint8* rxDest_p, uint16 rxSize, bool keepSsLow, Clbk_t Clbk)
{
    CHECK_STATUS();
    RET_EXEC(SetupTransaction(slaveId, txSrc_p, txSize, rxDest_p, rxSize, keepSsLow, nullptr, Clbk, false));
}

void SPI_M::SetSsLine(SlaveId slaveId)
{
    CHECK_STATUS();
    ctx.ssPin[(uint8)slaveId].Set();
}

void SPI_M::ClearSsLine(SlaveId slaveId)
{
    CHECK_STATUS();
    ctx.ssPin[(uint8)slaveId].Clear();
}

void SPI_M::ChangeBaudrate(Baudrate baud)
{
    CHECK_STATUS();
    ASSERT(baud != Baudrate::none, "BAUDRATE");
    if (baud !=ctx.config.baudrate) ctx.baudNew = baud;
}

uint32 SPI_M::GetBaudrateHz()
{
    CHECK_STATUS();
    return ctx.baudHz;
}

Status SPI_M::GetStatus()
{
    return ctx.status;
}

State SPI_M::GetState()
{
    return ctx.state;
}

/******************************************************************************************************************************************
                                                         PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

void SPI_M::ChangeState(State state)
{
    ctx.state = state;
}

void SPI_M::SetBaudrate(Baudrate baud)
{
    ctx.spiReg_p->CR1.bit.BR = (uint8)baud & 0b111;
    ctx.baudHz = hal::clk::PerClkGetFreqHz(ctx.spiPerId) / (uint32)utils::Exp(2, (uint8)baud + 1);
    ctx.byteTxRxTimeNs = (uint32)((NS_PER_SEC * BITS_PER_BYTE) / ctx.baudHz);
    ctx.config.baudrate = baud;
}

void SPI_M::EnableMosi()
{
    hal::GPIO::SetPinMode(ctx.config.mosiPinPort, ctx.config.mosiPinNum, hal::GPIO::PinMode::AltFunc);
}

void SPI_M::DisableMosi()
{
    hal::GPIO::SetPinMode(ctx.config.mosiPinPort, ctx.config.mosiPinNum, hal::GPIO::PinMode::Output);
}

void SPI_M::EnableTxIrq()
{
    ctx.spiReg_p->CR2.bit.TXEIE = true;
}

void SPI_M::DisableTxIrq()
{
    ctx.spiReg_p->CR2.bit.TXEIE = false;
}

void SPI_M::EnableRxIrq()
{
    ctx.spiReg_p->CR2.bit.RXNEIE = true;
}

void SPI_M::DisableRxIrq()
{
    ctx.spiReg_p->CR2.bit.RXNEIE = false;
}

void SPI_M::ClearRxFifo()
{
    while (ctx.spiReg_p->SR.bit.FRLVL) ctx.dummyRead16 = ctx.spiReg_p->DR.val16;
}

Exec SPI_M::SetupTransaction(SlaveId slaveId, uint8* txSrc_p, uint16 txSize, uint8* rxDest_p, uint16 rxSize, bool keepSsLow, uint64* trId_p, Clbk_t Clbk, bool wait)
{
    ASSERT(ctx.ssPin[(uint8)slaveId].GetStatus() == Status::OK, "SLAVE ID%d", slaveId);
    ASSERT(txSize <= ctx.config.txBuffSize, "TX DATA SIZE TO BIG");
    ASSERT((txSize > 0) || (rxSize > 0), "TX AND RX DATA SIZE IS ZERO");
    ASSERT_COND(txSize > 0, txSrc_p != nullptr, "TX DATA SOURCE ADDR.");
    ASSERT_COND(rxSize > 0, rxDest_p != nullptr, "RX DATA DEST. ADDR.");

    //-----------------------------------------------------------------------------------
    // Find free transaction buff

    ctx.trBuffFree_p = FindFreeTrBuff();

    if (ctx.trBuffFree_p == nullptr) {
        REPORT_WAR("TX FIFO FULL");
        do {
            CHECK_EXEC(Handler());
            ctx.trBuffFree_p = FindFreeTrBuff();
        } while (ctx.trBuffFree_p == nullptr);
    }

    //-----------------------------------------------------------------------------------
    // Setup transaction details

    if ((txSize > 0) && (rxSize == 0))      ctx.trBuffFree_p->trType = TrType::TX;
    else if ((rxSize > 0) && (txSize == 0)) ctx.trBuffFree_p->trType = TrType::RX;
    else                                    ctx.trBuffFree_p->trType = TrType::TXRX;

    ctx.trBuffFree_p->slaveId = slaveId;
    ctx.trBuffFree_p->slaveLabel_p = ctx.ssPin[(uint8)slaveId].GetConfig().label_p;
    ctx.trBuffFree_p->txPos = 0;
    ctx.trBuffFree_p->rxPos = 0;
    ctx.trBuffFree_p->txSize = txSize;
    ctx.trBuffFree_p->rxSize = rxSize;
    ctx.trBuffFree_p->rxDest_p = rxDest_p;
    memcpy(ctx.trBuffFree_p->buff_p, txSrc_p, txSize);
    ctx.trBuffFree_p->trId = ++ctx.trId;
    ctx.trBuffFree_p->ssMode = ctx.slave[(uint8)slaveId].mode;
    ctx.trBuffFree_p->keepSsLow = keepSsLow;
    ctx.trBuffFree_p->baud = ctx.config.baudrate;
    ctx.trBuffFree_p->Clbk = Clbk;
    ctx.trBuffFree_p->wait = wait;
    ctx.trBuffFree_p->timeoutUs = CalcTimeout(txSize + rxSize);
    ctx.trBuffFree_p->trStatus = TrStat::NEW;
    ctx.buffsBusy++;

    if (trId_p) *trId_p = ctx.trBuffFree_p->trId;

    //-----------------------------------------------------------------------------------
    // Trigger instant RX or wait till idle state

    if ((ctx.state == State::OFF) || (ctx.state == State::IDLE)) {
        TriggerTransaction(ctx.trBuffFree_p);
    }

    if (wait == true) {
        do {
            CHECK_EXEC(Handler());
        } while (ctx.state == State::BUSY);
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

void SPI_M::TriggerTransaction(trBuff_t* trBuff_p)
{
    //-----------------------------------------------------------------------------------
    // Setup and trigger transaction

    ctx.trBuffRun_p = trBuff_p;
    ctx.startTimerTimeout = true;

    ChangeState(State::BUSY);

    if (ctx.trBuffRun_p->ssMode == SsMode::AUTO) {
        SetSsLine(ctx.trBuffRun_p->slaveId);
    }

    if ((ctx.trBuffRun_p->trType == TrType::TX) || (ctx.trBuffRun_p->trType == TrType::TXRX)) {
        EnableMosi();
        ctx.trBuffRun_p->trStatus = TrStat::TXRUN;
        if (ctx.config.useDma && ((ctx.trBuffRun_p->txSize + ctx.trBuffRun_p->rxSize) >= ctx.config.dmaMinBytes)) {
            StartDmaTxTransfer(ctx.trBuffRun_p);
        }
        else EnableTxIrq();
    }
    else {
        ctx.trBuffRun_p->trStatus = TrStat::RXRUN;

        if (ctx.config.useDma && ctx.trBuffRun_p->rxSize >= ctx.config.dmaMinBytes) {
            StartDmaRxTransfer(ctx.trBuffRun_p);
        }
        else {
            ctx.spiReg_p->DR.val8 = 0xFF;
            EnableRxIrq();
        }
    }
}

SPI_M::trBuff_t* SPI_M::FindFreeTrBuff()
{
    uint8 buffNum;
    uint8 buffEnd;
    trBuff_t* trBuffFree_p = nullptr;
    trBuff_t* trBuffRun_p = nullptr;
    trBuff_t* trBuff_p = nullptr;

    hal::core::IrqDisable(ctx.spiIrqId);
    trBuffRun_p = ctx.trBuffRun_p;
    trBuff_p = ctx.trBuff_p;
    hal::core::IrqEnable(ctx.spiIrqId);

    if (trBuffRun_p == nullptr) {
        buffNum = 0;
        buffEnd = ctx.config.txBuffCnt - 1;
    }
    else {
        buffEnd = (uint8)(trBuffRun_p - trBuff_p);
        buffNum = (uint8)((buffEnd + 1) % ctx.config.txBuffCnt);
    }

    do {
        if ((trBuff_p[buffNum].trStatus == TrStat::NONE) ||
            (trBuff_p[buffNum].trStatus == TrStat::DONE)) {
                trBuffFree_p = &trBuff_p[buffNum];
                break;
            }
        buffNum += 1;
        buffNum %= ctx.config.txBuffCnt;
    } while (buffNum != buffEnd);

    return trBuffFree_p;
}

SPI_M::trBuff_t* SPI_M::FindWaitingTrBuff()
{
    uint8 buffNum;
    uint8 buffEnd;
    trBuff_t* trBuffWaiting_p = nullptr;
    trBuff_t* trBuffRun_p = nullptr;
    trBuff_t* trBuff_p = nullptr;

    hal::core::IrqDisable(ctx.spiIrqId);
    trBuffRun_p = ctx.trBuffRun_p;
    trBuff_p = ctx.trBuff_p;
    hal::core::IrqEnable(ctx.spiIrqId);

    if (trBuffRun_p == nullptr) {
        buffNum = 0;
        buffEnd = ctx.config.txBuffCnt - 1;
    }
    else {
        buffEnd = (uint8)(trBuffRun_p - trBuff_p);
        buffNum = (uint8)((buffEnd + 1) % ctx.config.txBuffCnt);
    }

    do {
        if (trBuff_p[buffNum].trStatus == TrStat::NEW) {
            trBuffWaiting_p = &trBuff_p[buffNum];
            break;
        }
        buffNum += 1;
        buffNum %= ctx.config.txBuffCnt;
    } while (buffNum != buffEnd);

    return trBuffWaiting_p;
}

uint32 SPI_M::CalcTimeout(uint16 byteCnt)
{
    return ((byteCnt * ctx.byteTxRxTimeNs * TXRX_TIMEOUT_FACTOR) / NS_PER_US) + US_PER_MS;
}

bool SPI_M::CheckForTimeout()
{
    if ((ctx.trBuffRun_p->trStatus == TrStat::TXRUN) ||
        (ctx.trBuffRun_p->trStatus == TrStat::RXRUN)) {
            if (ctx.startTimerTimeout) {
                ctx.startTimerTimeout = false;
                ctx.timerTimeout.ChangeInterval(ctx.trBuffRun_p->timeoutUs);
                ctx.timerTimeout.Enable(false, false);
            }
        return ctx.timerTimeout.Check();
    }
    return false;
}

void SPI_M::IrqHandlerClbk(hal::spiNum_t spiNum)
{
    volatile SPI_CR2 CR2_REG;
    volatile SPI_SR SR_REG;
    CR2_REG.val = spiMaster_p[spiNum]->ctx.spiReg_p->CR2.val;
    SR_REG.val = spiMaster_p[spiNum]->ctx.spiReg_p->SR.val;

    if (CR2_REG.bit.RXNEIE && SR_REG.bit.RXNE) {
        IrqRxHandler(spiMaster_p[spiNum]);
    }
    if (CR2_REG.bit.TXEIE && SR_REG.bit.TXE) {
        IrqTxHandler(spiMaster_p[spiNum]);
    }
    if (CR2_REG.bit.ERRIE &&
        (   SR_REG.bit.MODF ||
            SR_REG.bit.OVR  ||
            SR_REG.bit.UDR )) {
        IrqErrHandler(spiMaster_p[spiNum]);
    }
}

void SPI_M::IrqTxHandler(SPI_M* spiBus_p)
{
    SPI_M::trBuff_t* trBuff_p = spiBus_p->ctx.trBuffRun_p;
    volatile REG_SPI* spiReg_p = spiBus_p->ctx.spiReg_p;

    //-----------------------------------------------------------------------------------
    // Push tx data into fifo

    while (spiReg_p->SR.bit.TXE && (trBuff_p->txPos < trBuff_p->txSize)) {
        if ((uint16)(trBuff_p->txSize - trBuff_p->txPos) >= sizeof(uint16)) {
            spiReg_p->DR.val16 = *((uint16*)&trBuff_p->buff_p[trBuff_p->txPos]);
            trBuff_p->txPos += sizeof(uint16);
        }
        else {
            spiReg_p->DR.val8 = trBuff_p->buff_p[trBuff_p->txPos];
            trBuff_p->txPos += sizeof(uint8);
        }
    }

    //-----------------------------------------------------------------------------------
    // Clear rx fifo to prevent OVR (overrun) error IRQ flag

    spiBus_p->ClearRxFifo();

    //-----------------------------------------------------------------------------------
    // Chedck if all bytes have been sent

    if ((trBuff_p->txPos == trBuff_p->txSize) && (spiReg_p->SR.bit.BSY == false)) {
        spiBus_p->ClearRxFifo();
        spiBus_p->DisableTxIrq();
        spiBus_p->DisableMosi();

        if ((trBuff_p->ssMode == SsMode::AUTO)  &&
            (trBuff_p->trType == TrType::TX)    &&
            (trBuff_p->keepSsLow == false)) {
            spiBus_p->ClearSsLine(trBuff_p->slaveId);
        }

        if (trBuff_p->trType == TrType::TXRX) {
            trBuff_p->trStatus = TrStat::RXRUN;
            spiBus_p->ctx.dummyRead8 = spiReg_p->DR.val8;
            spiBus_p->EnableRxIrq();
            spiReg_p->DR.val8 = 0;        // Push first dummy byte into tx register to start rx
            return;
        }
        else {
            if (trBuff_p->Clbk) trBuff_p->Clbk();
            trBuff_p->trStatus = TrStat::DONE;
            spiBus_p->ctx.buffsBusy--;
            trBuff_p = spiBus_p->FindWaitingTrBuff();

            if (trBuff_p != nullptr) {
                spiBus_p->TriggerTransaction(trBuff_p);
            }
            else {
                spiBus_p->ChangeState(State::IDLE);
            }
        }
    }
}

void SPI_M::IrqRxHandler(SPI_M* spiBus_p)
{
    SPI_M::trBuff_t* trBuff_p = spiBus_p->ctx.trBuffRun_p;
    volatile REG_SPI* spiReg_p = spiBus_p->ctx.spiReg_p;

    //-----------------------------------------------------------------------------------
    // Read rx byte

    trBuff_p->rxDest_p[trBuff_p->rxPos++] = (uint8)spiReg_p->DR.val8;

    //-----------------------------------------------------------------------------------
    // All bytes have been received?

    if (trBuff_p->rxPos < trBuff_p->rxSize) {
        spiReg_p->DR.val8 = 0;
        return;
    }
    else {
        spiBus_p->DisableRxIrq();
        if ((trBuff_p->ssMode == SsMode::AUTO) && (trBuff_p->keepSsLow == false)) {
            spiBus_p->ClearSsLine(trBuff_p->slaveId);
        }
        if (trBuff_p->Clbk) trBuff_p->Clbk();
        trBuff_p->trStatus = TrStat::DONE;
        spiBus_p->ctx.buffsBusy--;
    }

    //-----------------------------------------------------------------------------------
    // Check if there is a new transaction waiting

    SPI_M::trBuff_t* trBuffWaiting_p = spiBus_p->FindWaitingTrBuff();
    if (trBuffWaiting_p != nullptr) {
        spiBus_p->TriggerTransaction(trBuffWaiting_p);
    }
    else {
        spiBus_p->ChangeState(State::IDLE);
    }
}

void SPI_M::IrqErrHandler(SPI_M* spiBus_p)
{
    // TODO
}

} // END OF NAMESPACE 'hal'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
