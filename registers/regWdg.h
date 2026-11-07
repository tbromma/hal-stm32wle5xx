/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: WDG, IWDG
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
 * IWDG
 ******************************************************************************************************************************************/

union IWDG_KR {
    struct {
        uint32 KEY  : 16; // Key value (write only, read 0x0000)
        uint32      : 16;
    } bit;
    uint32 val;
};

union IWDG_PR {
    struct {
        uint32 PR   : 3; // Prescaler divider
        uint32      : 29;
    } bit;
    uint32 val;
};

union IWDG_RLR {
    struct {
        uint32 RL   : 12; // Watchdog counter reload value
        uint32      : 20;
    } bit;
    uint32 val;
};

union IWDG_SR {
    struct {
        uint32 PVU  : 1; // Watchdog prescaler value update
        uint32 RVU  : 1; // Watchdog counter reload value update
        uint32 WVU  : 1; // Watchdog counter window value update
        uint32      : 29;
    } bit;
    uint32 val;
};

union IWDG_WINR {
    struct {
        uint32 WIN  : 12; // Watchdog counter window value
        uint32      : 20;
    } bit;
    uint32 val;
};

struct REG_IWDG
{
    IWDG_KR     KR; // Address offset: 0x000
    IWDG_PR     PR; // Address offset: 0x004
    IWDG_RLR    RLR; // Address offset: 0x008
    IWDG_SR     SR; // Address offset: 0x00C
    IWDG_WINR   WINR; // Address offset: 0x010
};

#define REG_IWDG ((volatile REG_IWDG*)0x40003000u)

/******************************************************************************************************************************************
 * WWDG
 ******************************************************************************************************************************************/

union WWDG_CR {
    struct {
        uint32 T        : 7; // 7-bit counter (MSB to LSB)
        uint32 WDGA     : 1; // Activation bit
        uint32          : 24;
    } bit;
    uint32 val;
};

union WWDG_CFR {
    struct {
        uint32 W        : 7; // 7-bit window value
        uint32          : 2;
        uint32 EWI      : 1; // Early wake-up interrupt enable
        uint32          : 1;
        uint32 WDGTB    : 3; // Timer base
        uint32          : 18;
    } bit;
    uint32 val;
};

union WWDG_SR {
    struct {
        uint32 EWIF     : 1; // Early wake-up interrupt flag
        uint32          : 31;
    } bit;
    uint32 val;
};

struct REG_WWDG
{
    WWDG_CR     CR; // Address offset: 0x000
    WWDG_CFR    CFR; // Address offset: 0x004
    WWDG_SR     SR; // Address offset: 0x008
};

#define REG_WWDG ((volatile REG_WWDG*)0x40002C00u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
