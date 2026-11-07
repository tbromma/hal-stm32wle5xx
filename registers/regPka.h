/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: PKA
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union PKA_CR {
    struct {
        uint32 EN           : 1;    // PKA enable.
        uint32 START        : 1;    // start the operation
        uint32              : 6;
        uint32 MODE         : 6;    // PKA operation code
        uint32              : 3;
        uint32 PROCENDIE    : 1;    // End of operation interrupt enable
        uint32              : 1;
        uint32 RAMERRIE     : 1;    // RAM error interrupt enable
        uint32 ADDRERRIE    : 1;    // Address error interrupt enable
        uint32              : 11;
    } bit;
    uint32 val;
};

union PKA_SR {
    struct {
        uint32          : 16;
        uint32 BUSY     : 1;    // PKA operation is in progress
        uint32 PROCENDF : 1;    // PKA End of Operation flag
        uint32          : 1;
        uint32 RAMERRF  : 1;    // PKA RAM error flag
        uint32 ADDRERRF : 1;    // Address error flag
        uint32          : 11;
    } bit;
    uint32 val;
};

union PKA_CLRFR {
    struct {
        uint32              : 17;
        uint32 PROCENDFC    : 1;    // Clear PKA End of Operation flag
        uint32              : 1;
        uint32 RAMERRFC     : 1;    // Clear PKA RAM error flag
        uint32 ADDRERRFC    : 1;    // Clear Address error flag
        uint32              : 11;
    } bit;
    uint32 val;
};

struct REG_PKA
{
    PKA_CR      CR;     // Address offset: 0x000
    PKA_SR      SR;     // Address offset: 0x004
    PKA_CLRFR   CLRFR;  // Address offset: 0x008
};

#define REG_PKA ((volatile REG_PKA*)0x58002000u)

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
