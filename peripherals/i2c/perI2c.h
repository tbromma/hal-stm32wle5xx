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

#pragma once
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"
#include "common.h"
#include "regI2c.h"
#include "mcuCore.h"
#include "sysclk.h"
#include "timer.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::i2c {

enum mode_t {
    MODE_NONE,
    MODE_IDLE,
    MODE_BUSY,
    MODE_ERR,
};

using slaveAddr_t = uint8;       // I2C 7-bit slave address -> 0b0xxxxxxx

struct config_t {
    hal::i2cNum_t           i2cNum;
    hal::clk::i2cClkSrc_t   clkSrc;
    bool                    fastModePlus;                   // Setting to 1 allows baudrates up to 1MHz
    uint32                  baudrate;                       // Expected baudrate. Real baudrate can differ depending on pullup resistor values
    uint16                  txBuffSize = 5;                 // Transmision buffer size in bytes
    uint8                   txBuffCount = 1;                // Transmision buffer count
    hal::pinPort_t          sdaPinPort = hal::PORT_none;
    hal::pinNum_t           sdaPinNum = hal::PIN_none;
    hal::pinFunc_t          sdaPinFunc = hal::FUNC_none;
    hal::pinPort_t          sclPinPort = hal::PORT_none;
    hal::pinNum_t           sclPinNum = hal::PIN_none;
    hal::pinFunc_t          sclPinFunc = hal::FUNC_none;
    hal::irqPrio_t          irqPrio = hal::IRQ_PRIO_none;
    const char*             label_p = nullptr;              // Description label
};

/******************************************************************************************************************************************
                                                              PUBLIC CLASSES
******************************************************************************************************************************************/

class MASTER {

public:

    Exec  Handler();
    Exec  Setup(config_t config);
    Exec  TxData(slaveAddr_t addr, uint8* txSrc, uint8 txSize, bool wait, uint64* trId);
    Exec  RxData(slaveAddr_t addr, uint8* rxDest_p, uint8 rxSize, bool wait, uint64* trId);
    Exec  TxRxData(slaveAddr_t addr, uint8* txSrc, uint8 txSize, uint8* rxDest_p, uint8 rxSize, bool wait, uint64* trId);
    uint32  GetBaudrateHz();
    Status  GetStatus();
    mode_t  GetMode();

private:

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

    enum irqErr_t {
        IRQERR_NONE,
        IRQERR_NACK,
        IRQERR_ARLO,
        IRQERR_BERR,
        IRQERR_OVR,
    };

    // Transaction buffer
    struct trBuff_t {
        slaveAddr_t slaveAddr;
        trType_t    trType;
        trStat_t    trStatus = TR_STAT_NONE;
        uint64      trId;
        uint8       txSize;
        uint8       txPos;
        uint8       rxSize;
        uint8       rxPos;
        uint8_t*    rxDest_p;
        uint32      timeoutUs;
        uint8*      buff_p;
    };

    void        IrqEnable();
    void        IrqDisable();
    Exec        CheckPioPeripheralClock(hal::pinPort_t pinPort_f);
    Exec        SetupInPin(hal::pinPort_t pinPort, hal::pinNum_t pinNum);
    Exec        SetupOutPin(hal::pinPort_t pinPort, hal::pinNum_t pinNum, hal::pinFunc_t pinFunc);
    Exec        SetupTransaction(slaveAddr_t slaveAddr, uint8* txSrc, uint8 txSize, uint8* rxDest_p, uint8 rxSize, bool waitTillCmp, uint64* trId);
    void        TriggerTransaction(trBuff_t* trBuff);
    trBuff_t*   FindFreeTrBuff();
    trBuff_t*   FindWaitingTrBuff();
    bool        CheckForTimeout();

    struct {
        config_t            config = {};
        Status              status = Status::NONE;
        mode_t              mode = MODE_NONE;
        uint64              trId = 0;
        uint32              byteTxRxTimeNs;
        bareOs::Timer       timerTimeout;
        volatile bool       startTimerTimeout = false;
        hal::perId_t        i2cPerId;
        hal::irqId_t        i2cIrqEvId;
        hal::irqId_t        i2cIrqErId;
        volatile REG_I2C*   i2cReg_p = nullptr;
        trBuff_t*           trBuff_p = nullptr;
        trBuff_t*           trBuffRun_p = nullptr;
        trBuff_t*           trBuffWaiting_p = nullptr;
        volatile uint8      buffsBusy = 0;
        bool                irqErrFlag = false;
        irqErr_t            irqErrType = IRQERR_NONE;
        const char*         irqErrStr = nullptr;
    } ctx = {};

    static inline MASTER* i2cBusClass_p[4] = {nullptr};
    static void IrqEvtCallback(hal::i2cNum_t i2cNum);
    static void IrqErrCallback(hal::i2cNum_t i2cNum);
    static void IrqHandlerTxe(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p);
    static void IrqHandlerTc(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p);
    static void IrqHandlerRxne(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p);
    static void IrqErrHandler(MASTER* i2cBus_p, volatile REG_I2C* i2cReg_p, irqErr_t irqErr);
};

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::i2c'
