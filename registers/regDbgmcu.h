/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: DBGMCU
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union DBGMCU_IDCODE { struct {
uint32 DEV_ID       : 12;   // Device identifier
uint32              : 4;
uint32 REV_ID       : 16;   // Revision identifier
} bit;
    uint32 val;
};
union DBGMCU_CR { struct {
uint32 DBG_SLEEP    : 1;    // Debug support in Sleep mode
uint32 DBG_STOP     : 1;    // Debug support in Stop mode
uint32 DBG_STANDBY  : 1;    // Debug support in Standby mode
uint32              : 29;
} bit;
    uint32 val;
};

struct REG_DBGMCU
{
    DBGMCU_IDCODE   IDCODE;         // 0x00
    DBGMCU_CR       CR;             // 0x04
    uint32          RESERVED1[13];  // 0x08..0x38
    uint32          APB1FZR1;       // 0x3C
    uint32          RESERVED2;      // 0x40
    uint32          APB1FZR2;       // 0x44
    uint32          RESERVED3;      // 0x48
    uint32          APB2FZR;        // 0x4C
};
#define REG_DBGMCU ((volatile REG_DBGMCU*)0xE0042000u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
