/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: SYSTICK
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union SYSTICK_CSR {
    struct {
        uint32 ENABLE       : 1;    // Counter enable
        uint32 TICKINT      : 1;    // SysTick interrupt enable
        uint32 CLKSOURCE    : 1;    // Clock source selection
        uint32              : 13;
        uint32 COUNTFLAG    : 1;    // Counter reached zero flag
        uint32              : 15;
    } bit;
    uint32 val;
};

union SYSTICK_RVR {
    struct {
        uint32 RELOAD   : 24;   // Reload value
        uint32          : 8;
    } bit;
    uint32 val;
};

union SYSTICK_CVR {
    struct {
        uint32 CURRENT  : 24;   // Current counter value
        uint32          : 8;
    } bit;
    uint32 val;
};

union SYSTICK_CALIB {
    struct {
        uint32 TENMS    : 24;   // Calibration value for 10 ms
        uint32          : 6;
        uint32 SKEW     : 1;    // Calibration value is inexact
        uint32 NOREF    : 1;    // Reference clock not provided
    } bit;
    uint32 val;
};

struct REG_SYSTICK
{
    SYSTICK_CSR     CSR;
    SYSTICK_RVR     RVR;
    SYSTICK_CVR     CVR;
    SYSTICK_CALIB   CALIB;
};

#define REG_SYSTICK ((volatile REG_SYSTICK*)0xE000E010UL)    // Systick Timer Register access

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
