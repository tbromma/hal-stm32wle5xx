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
// DESCRIPTION:      Low level layer for USART peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"
#include "common.h"
#include "regUsart.h"
#include "irqUsart.h"
#include "sysclk.h"
#include "timer.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::usart {

enum mode_t {
    MODE_NONE,
    MODE_OFF,
    MODE_IDLE,
    MODE_BUSY,
    MODE_ERR,
};

enum baudrate_t : int32 {
    BAUD_none = -1,
    BAUD_4800 = 4800,
    BAUD_9600 = 9600,
    BAUD_19200 = 19200,
    BAUD_28800 = 28800,
    BAUD_38400 = 38400,
    BAUD_57600 = 57600,
    BAUD_76800 = 76800,
    BAUD_115200 = 115200,
    BAUD_230400 = 230400,
    BAUD_460800 = 460800,
    BAUD_576000 = 576000,
    BAUD_921600 = 921600,
};

enum stopBits_t {
    STOPBITS_1 = 0b00,      // 1 stop bit
    STOPBITS_0d5 = 0b01,    // 0.5 stop bit.
    STOPBITS_2 = 0b10,      // 2 stop bits
    STOPBITS_1d5 = 0b11,    // 1.5 stop bits
};

enum parity_t : int8 {
    PARITY_NONE = -1,
    PARITY_EVEN = 0,
    PARITY_ODD = 1,
};

enum oversampling_t {
    OVERSAMPLING_8 = 8,
    OVERSAMPLING_16 = 16,
};

using trId_t = uint64;  // UART transaction ID

struct config_t {
    hal::usartNum_t             usartNum = hal::USART_none;
    hal::clk::usartClkSrc_t     clkSrc;
    baudrate_t                  baudrate = BAUD_none;
    stopBits_t                  stopBits = STOPBITS_1;
    parity_t                    parity = PARITY_NONE;
    oversampling_t              oversampling = OVERSAMPLING_16;
    bool                        useTxdLine = false;
    bool                        useRxdLine = false;
    uint16                      txBuffSize = 10;
    uint8                       txBuffCount = 1;
    hal::pinPort_t              txdPinPort = hal::PORT_none;
    hal::pinNum_t               txdPinNum = hal::PIN_none;
    hal::pinFunc_t              txdPinFunc = hal::FUNC_none;
    hal::pinPort_t              rxdPinPort = hal::PORT_none;
    hal::pinNum_t               rxdPinNum = hal::PIN_none;
    hal::pinFunc_t              rxdPinFunc = hal::FUNC_none;
    hal::irqUsart::RxfneClbk_t  RxDataClbk = nullptr;
    hal::irqPrio_t              irqPrio = hal::IRQ_PRIO_none;
    const char*                 label_p = nullptr;
};

/******************************************************************************************************************************************
                                                              PUBLIC CLASSES
******************************************************************************************************************************************/

class USART {

public:

    Exec  Setup(config_t config);
    Exec  Handler();
    Exec  TxData(uint8* txSrc_p, uint16 txSize, bool wait, trId_t* trId);
    Exec  RxEnable();
    Exec  RxDisable();
    Exec  TxEnable();
    Exec  TxDisable();
    Exec  ChangeRxClbk(hal::irqUsart::RxfneClbk_t Clbk);
    uint32  GetBaudrate();
    Status  GetStatus();

private:

    enum trStat_t {
        TR_STAT_NONE,
        TR_STAT_NEW,
        TR_STAT_TXRUN,
        TR_STAT_TXDONE,
    };

    struct trBuff_t {   // Transaction buffer
        trStat_t    trStatus = TR_STAT_NONE;
        trId_t      trId;
        uint16      txDataSize;
        uint32      txTimeoutUs;
        uint16      buffPos;
        uint8*      buff_p;
    };

    void        UsartEnable();
    void        UsartDisable();
    void        IrqSetPrio(hal::irqPrio_t irqPrio);
    void        IrqEnable();
    void        IrqDisable();
    void        TriggerTx(trBuff_t* trBuff);
    trBuff_t*   FindFreeTrBuff();
    trBuff_t*   FindWaitingTrBuff();

    struct {
        config_t                config = {};
        Status                  status = Status::NONE;
        mode_t                  mode = MODE_NONE;
        bareOs::Timer           timerTxTimeout;
        bareOs::Timer           timerTemp;
        bool                    txdPinCfgOk = false;
        bool                    rxdPinCfgOk = false;
        bool                    txEnabled = false;
        bool                    rxEnabled = false;
        bool                    usartEnabled = false;
        bool                    irqEnabled = false;
        uint32                  byteTxTimeNs;
        hal::perId_t            usartPerId;
        hal::irqId_t            usartIrqId;
        volatile REG_USART*     usartReg_p = nullptr;
        trBuff_t*               trBuff_p = nullptr;
        trBuff_t*               trBuffRun_p = nullptr;
        trBuff_t*               trBuffWaiting = nullptr;
        trId_t                  trId = 0;
        volatile uint8          buffsBusy = 0;
    } ctx = {};

    friend void IrqTxReadyHandler(hal::usartNum_t);
    friend void IrqTxEmptyHandler(hal::usartNum_t);
};

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::usart'
