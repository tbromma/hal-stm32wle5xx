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
// CREATION DATE:    November 2023
// DESCRIPTION:      Low level layer for SPI3 peripheral, communicating with the integrated Semtech SX126x radio

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include "common.h"
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"
#include "regSpi.h"
#include "timer.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::spiRf {

enum mode_t {
    MODE_NONE,
    MODE_OFF,
    MODE_IDLE,
    MODE_BUSY,
    MODE_ERR,
};

enum slaveId_t {
    SLAVE_ID0,
    SLAVE_ID1,
    SLAVE_ID2,
    SLAVE_ID3,
    SLAVE_count,
};

enum baudrate_t {
    BAUD_PCLK_DIV2,     // SPI baudrate = peripheral_clock_freq / 2
    BAUD_PCLK_DIV4,
    BAUD_PCLK_DIV8,
    BAUD_PCLK_DIV16,
    BAUD_PCLK_DIV32,
    BAUD_PCLK_DIV64,
    BAUD_PCLK_DIV128,
    BAUD_PCLK_DIV256,
};

struct config_t {
    bool            clkPol;                         // Clock polarity
    bool            clkPhase;                       // Clock phase
    baudrate_t      baudrate;                       // Baudrate clock speed
    uint16          txBuffSize = 10;                // Transmit buffer size in bytes
    uint8           txBuffCnt = 1;                  // Transmit buffer count
    hal::irqPrio_t  irqPrio = hal::IRQ_PRIO_none;
    bool            debugEn = false;                // If set 'true' all SPI signals will be outputted on PORTA for debug
                                                    // MOSI -> PA7
                                                    // MISO -> PA6
                                                    // SCK  -> PA5
                                                    // SS   -> PA4
    const char*     label_p = nullptr;              // Optional
};

/******************************************************************************************************************************************
                                                              PUBLIC API
******************************************************************************************************************************************/

Exec  Setup(config_t config);
Exec  Handler();
Exec  TxData(uint8* txSrc, uint16 txSize, bool keepSsLow, uint64* trId, bool wait);
Exec  RxData(uint8* rxDest, uint16 rxSize, bool keepSsLow, uint64* trId, bool wait);
Exec  TxRxData(uint8* txSrc, uint16 txSize, uint8* rxDest, uint16 rxSize, bool keepSsLow, uint64* trId, bool wait);
uint32  GetBaudrateHz();
Status  GetStatus();
mode_t  GetMode();

} // END OF NAMESPACE 'hal::SPI_M'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
