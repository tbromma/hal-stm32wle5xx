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
// DESCRIPTION:      Low level layer for handling the Cortex M7 core

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include "common.h"
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::core {

enum systickClkSrc_t : uint8 {
    SYSTICK_CLK_EXT,    // External clock source
    SYSTICK_CLK_MCU,    // Procesor clock as clock source
};

union rstReason_t {
    struct {
        uint8 RFILA     : 1;    // Sub-GHz radio illegal command reset
        uint8 OBLRS     : 1;    // Option byte loader reset, occurs due to option byte loading
        uint8 PIN       : 1;    // Pin reset, occurs when pin NRST is set low
        uint8 BOR       : 1;    // Brownout reset
        uint8 SFT       : 1;    // Software reset
        uint8 IWDT      : 1;    // Independent window watchdog reset
        uint8 WWDT      : 1;    // Window watchdog reset
        uint8 LPWR      : 1;    // Low-power reset, occurs due to illegal Stop, Standby or Shutdown mode entry
    } bit;
    uint8 val;
};

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/

    // System reset
    void    SystemReset();
    void    GetResetReason(rstReason_t& rstReason);
    void    ClearResetFlags();

    // Systick timer
    Exec    SystickConfig(systickClkSrc_t clkSrc, uint32 reloadVal, bool irqEnable, bool timerEnable);
    void    SystickEnable();
    void    SystickDisable();
    void    SystickIrqEnable();
    void    SystickIrqDisable();
    void    SystickSetReloadVal(uint32 reloadVal, bool resetTimer);
    Status  SystickGetStatus();

    // Global interrupts
    void    IrqGlobalEnable();
    void    IrqGlobalDisable();
    bool    IrqGlobalStatus();

    // Fault interrupts
    void    IrqFaultEnable();
    void    IrqFaultDisable();

    // Peripheral interrupts
    void        IrqEnable(irqId_t irqId);
    void        IrqDisable(irqId_t irqId);
    void        IrqSetPrio(irqId_t irqId, irqPrio_t prio);
    irqPrio_t   IrqGetPrio(irqId_t irqId);
    bool        IrqGetEnable(irqId_t irqId);

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::core'