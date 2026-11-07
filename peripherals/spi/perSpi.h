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

#pragma once
#include "common.h"
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"
#include "regSpi.h"
#include "regDma.h"
#include "perGpio.h"
#include "timer.h"

/******************************************************************************************************************************************
                                                              PUBLIC CLASSES
******************************************************************************************************************************************/



namespace hal {

// SPI master class
class SPI_M {
public:

    enum class SlaveId : int8 {
        none = -1,
        ID0,
        ID1,
        ID2,
        ID3,
        count,
    };

    enum class Baudrate : int8 {
        none = -1,
        PCLK_DIV2,              // SPI baudrate = peripheral_clock_freq / 2
        PCLK_DIV4,
        PCLK_DIV8,
        PCLK_DIV16,
        PCLK_DIV32,
        PCLK_DIV64,
        PCLK_DIV128,
        PCLK_DIV256,
    };

    enum class SsMode : int8 {
        none = -1,              // Invalid option
        AUTO,                   // Slave select line in auto mode -> no user action needed
        MANUAL,                 // Slave select line in manual mode -> user must set/clear ss line
    };

    enum CsMode : int8 {
        CSMODE_none = -1,       // Invalid option
        CSMODE_AUTO,            // Slave select line in auto mode -> no user action needed
        CSMODE_MANUAL,          // Slave select line in manual mode -> user must set/clear ss line
    };

    using Clbk_t = void(*)();

    struct config_t {
        hal::spiNum_t   spiNum = hal::SPI_none;
        bool            clkPol;                         // Clock polarity
        bool            clkPhase;                       // Clock phase
        Baudrate        baudrate = Baudrate::none;      // Baudrate clock speed
        uint16          txBuffSize = 10;                // Transmit buffer size in bytes
        uint8           txBuffCnt = 1;                  // Transmit buffer count
        bool            useDma = false;
        dmaNum_t        dmaNum = hal::DMA_none;
        dmaCh_t         dmaTxChannel = hal::DMA_CH_none;
        dmaCh_t         dmaRxChannel = hal::DMA_CH_none;
        uint8           dmaMinBytes = 8;

        hal::pinPort_t  misoPinPort = hal::PORT_none;
        hal::pinNum_t   misoPinNum = hal::PIN_none;
        hal::pinFunc_t  misoPinFunc = hal::FUNC_none;

        hal::pinPort_t  mosiPinPort = hal::PORT_none;
        hal::pinNum_t   mosiPinNum = hal::PIN_none;
        hal::pinFunc_t  mosiPinFunc = hal::FUNC_none;

        hal::pinPort_t  sckPinPort = hal::PORT_none;
        hal::pinNum_t   sckPinNum = hal::PIN_none;
        hal::pinFunc_t  sckPinFunc = hal::FUNC_none;

        hal::irqPrio_t  irqPrio = hal::IRQ_PRIO_none;
        const char*     label_p = nullptr;              // Optional
    };

    struct slave_t {
        SlaveId         slaveId = SlaveId::none;
        hal::pinPort_t  pinPort = hal::PORT_none;
        hal::pinNum_t   pinNum = hal::PIN_none;
        SsMode          mode = SsMode::none;
        const char*     label_p;
    };

    Exec    Setup(config_t config);
    Exec    Handler();
    Exec    AddSlave(slave_t config);
    void    EnableSpi();
    void    DisableSpi();
    Exec    TxData(SlaveId slaveId, uint8* txSrc, uint16 txSize, bool keepSsLow, uint64* trId, bool wait);
    Exec    RxData(SlaveId slaveId, uint8* rxDest, uint16 rxSize, bool keepSsLow, uint64* trId, bool wait);
    Exec    TxRxData(SlaveId slaveId, uint8* txSrc, uint16 txSize, uint8* rxDest, uint16 rxSize, bool keepSsLow, uint64* trId, bool wait);
    Exec    TxDataClbk(SlaveId slaveId, uint8* txSrc, uint16 txSize, bool keepSsLow, Clbk_t Clbk);
    Exec    RxDataClbk(SlaveId slaveId, uint8* rxDest, uint16 rxSize, bool keepSsLow, Clbk_t Clbk);
    Exec    TxRxDataClbk(SlaveId slaveId, uint8* txSrc, uint16 txSize, uint8* rxDest, uint16 rxSize, bool keepSsLow, Clbk_t Clbk);
    void    SetSsLine(SlaveId slaveId);
    void    ClearSsLine(SlaveId slaveId);
    void    ChangeBaudrate(Baudrate baud);
    uint32  GetBaudrateHz();
    Status  GetStatus();
    State   GetState();

private:

    static constexpr uint8 TXRX_TIMEOUT_FACTOR = 5;     // TX-RX timeout safety factor
    static constexpr uint64 NS_PER_SEC = 1000000000;
    static constexpr uint32 NS_PER_US = 1000;           // Nanoseconds per microsecond
    static constexpr uint32 US_PER_MS = 1000;           // Microseconds per milisecond
    static constexpr uint8 BITS_PER_BYTE = 8;

    // Transaction status
    enum class TrStat : uint8 {
        NONE,
        NEW,
        TXRUN,
        RXRUN,
        DONE,
        ERR,
    };

    // Transaction type
    enum class TrType : uint8 {
        NONE,
        TX,
        RX,
        TXRX,
    };

    // Transaction buffer
    struct trBuff_t {
        SlaveId     slaveId;
        const char* slaveLabel_p;
        TrType      trType = TrType::NONE;
        TrStat      trStatus = TrStat::NONE;
        uint64      trId;
        SsMode      ssMode;
        bool        keepSsLow;
        Baudrate    baud;
        Clbk_t      Clbk;
        uint16      txSize;
        uint16      txPos;
        uint16      rxSize;
        uint16      rxPos;
        uint8*      rxDest_p;
        bool        wait;
        uint32      timeoutUs;
        uint8*      buff_p;
    };

    void        SetupDma();
    void        ChangeState(State state);
    void        SetBaudrate(Baudrate baud);
    void        EnableMosi();
    void        DisableMosi();
    void        EnableTxIrq();
    void        DisableTxIrq();
    void        EnableRxIrq();
    void        DisableRxIrq();
    void        ClearRxFifo();
    Exec        SetupTransaction(SlaveId slaveId, uint8* txSrc, uint16 txSize, uint8* rxDest, uint16 rxSize, bool keepSsLow, uint64* trId, Clbk_t Clbk, bool wait);
    void        TriggerTransaction(trBuff_t* trBuff);
    void        StartDmaTxTransfer(trBuff_t* trBuff);
    void        StartDmaRxTransfer(trBuff_t* trBuff_p);
    void        DisableDmaChannels();
    void        DisableDmaIrq();
    void        EnableDmaIrq();
    void        ClearDmaIrqFlags();
    void        ResetDma();
    trBuff_t*   FindFreeTrBuff();
    trBuff_t*   FindWaitingTrBuff();
    uint32      CalcTimeout(uint16 byteCnt);
    bool        CheckForTimeout();

    struct {
        config_t            config = {};
        Status              status = Status::NONE;
        State               state = State::NONE;
        bareOs::Timer       timerTimeout;
        volatile bool       startTimerTimeout = false;
        Baudrate            baudNew;
        uint32              baudHz;
        uint32              byteTxRxTimeNs;
        slave_t             slave[(uint8)SlaveId::count] = {};
        hal::GPIO::OUT      ssPin[(uint8)SlaveId::count] = {};
        hal::perId_t        spiPerId;
        hal::irqId_t        spiIrqId;
        volatile REG_SPI*   spiReg_p = nullptr;
        volatile REG_DMA*   dmaReg_p = nullptr;
        trBuff_t*           trBuff_p = {};
        trBuff_t*           trBuffRun_p = nullptr;
        trBuff_t*           trBuffFree_p = nullptr;
        uint64              trId = 0;
        uint8               rxByteCnt;
        volatile uint8      buffsBusy = 0;
        volatile uint8      dummyRead8;
        volatile uint16     dummyRead16;
        GPIO::OUT           gpio3;
    } ctx = {};

    static inline SPI_M* spiMaster_p[2];
    static void IrqHandlerClbk(hal::spiNum_t spiNum);
    static void IrqTxHandler(SPI_M* spiBus_p);
    static void IrqRxHandler(SPI_M* spiBus_p);
    static void IrqErrHandler(SPI_M* spiBus_p);
    static void IrqDmaTxTransfer(hal::dmaNum_t dmaNum, hal::dmaCh_t dmaCh, hal::perId_t perID);
    static void IrqDmaRxTransfer(hal::dmaNum_t dmaNum, hal::dmaCh_t dmaCh, hal::perId_t perID);
};

} // END OF NAMESPACE 'hal'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
