/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: SYSCFG
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                             TYPES & VARIABLES
******************************************************************************************************************************************/

union SYSCFG_EXTICR1 {
    struct {
        uint32 EXT0 : 3;    // EXTI line 0 port selection
        uint32      : 1;
        uint32 EXT1 : 3;    // EXTI line 1 port selection
        uint32      : 1;
        uint32 EXT2 : 3;    // EXTI line 2 port selection
        uint32      : 1;
        uint32 EXT3 : 3;    // EXTI line 3 port selection
        uint32      : 17;
    } bit;
    uint32 val;
};

union SYSCFG_EXTICR2 {
    struct {
        uint32 EXT4 : 3;    // EXTI line 4 port selection
        uint32      : 1;
        uint32 EXT5 : 3;    // EXTI line 5 port selection
        uint32      : 1;
        uint32 EXT6 : 3;    // EXTI line 6 port selection
        uint32      : 1;
        uint32 EXT7 : 3;    // EXTI line 7 port selection
        uint32      : 17;
    } bit;
    uint32 val;
};

union SYSCFG_EXTICR3 {
    struct {
        uint32 EXT8     : 3;    // EXTI line 8 port selection
        uint32          : 1;
        uint32 EXT9     : 3;    // EXTI line 9 port selection
        uint32          : 1;
        uint32 EXT10    : 3;    // EXTI line 10 port selection
        uint32          : 1;
        uint32 EXT11    : 3;    // EXTI line 11 port selection
        uint32          : 17;
    } bit;
    uint32 val;
};

union SYSCFG_EXTICR4 {
    struct {
        uint32 EXT12    : 3;    // EXTI line 12 port selection
        uint32          : 1;
        uint32 EXT13    : 3;    // EXTI line 13 port selection
        uint32          : 1;
        uint32 EXT14    : 3;    // EXTI line 14 port selection
        uint32          : 1;
        uint32 EXT15    : 3;    // EXTI line 15 port selection
        uint32          : 17;
    } bit;
    uint32 val;
};

struct REG_SYSCFG
{
    uint32          MEMRMP;         // SYSCFG memory remap register                             Address offset: 0x00
    uint32          CFGR1;          // SYSCFG configuration register 1                          Address offset: 0x04
    SYSCFG_EXTICR1  EXTICR1;        // SYSCFG external interrupt configuration register 1       Address offset: 0x08
    SYSCFG_EXTICR2  EXTICR2;        // SYSCFG external interrupt configuration register 2       Address offset: 0x0C
    SYSCFG_EXTICR3  EXTICR3;        // SYSCFG external interrupt configuration register 3       Address offset: 0x10
    SYSCFG_EXTICR4  EXTICR4;        // SYSCFG external interrupt configuration register 4       Address offset: 0x14
    uint32          SCSR;           // SYSCFG SRAM2 control and status register                 Address offset: 0x18
    uint32          CFGR2;          // SYSCFG configuration register 2                          Address offset: 0x1C
    uint32          SWPR;           // SYSCFG SRAM2 write protection register part              Address offset: 0x20
    uint32          SKR;            // SYSCFG SRAM2 key register                                Address offset: 0x24
    uint32          RESERVED1[120]; // Reserved                                                 Address offset: 0x28-0x204
    uint32          RFDCR;          // SYSCFG CPU2 radio debug control register                 Address offset: 0x208
};

#define REG_SYSCFG ((volatile REG_SYSCFG*)0x40010000u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
