/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: CRC
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                              TYPES & VARIABLES
******************************************************************************************************************************************/

union CRC_DR {
    struct {
        uint32 DR   : 32;   // Data register value
    } bit;
    uint32 val;
};

union CRC_IDR {
    struct {
        uint32 IDR  : 32;   // Independent data register value
    } bit;
    uint32 val;
};

union CRC_CR {
    struct {
        uint32 RESET    : 1;    // Reset CRC calculation unit
        uint32          : 2;
        uint32 POLYSIZE : 2;    // Polynomial size
        uint32 REV_IN   : 2;    // Input data bit reversal mode
        uint32 REV_OUT  : 1;    // Output data bit reversal enable
    } bit;
    uint32 val;
};

union CRC_INIT {
    struct {
        uint32 INIT : 32;   // Initial CRC value
    } bit;
    uint32 val;
};

union CRC_POL {
    struct {
        uint32 POL  : 32;   // CRC polynomial value
    } bit;
    uint32 val;
};

struct REG_CRC
{
    CRC_DR      DR;         // CRC Data register                    Address offset: 0x00
    CRC_IDR     IDR;        // CRC Independent data register        Address offset: 0x04
    CRC_CR      CR;         // CRC Control register                 Address offset: 0x08
    uint32      RESERVED;   // Reserved                                             0x0C
    CRC_INIT    INIT;       // Initial CRC value register           Address offset: 0x10
    CRC_POL     POL;        // CRC polynomial register              Address offset: 0x14
};

#define REG_CRC ((volatile REG_CRC*)0x40023000u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
