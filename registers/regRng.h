/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: RNG
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                              TYPES & VARIABLES
******************************************************************************************************************************************/

union RNG_CR {
    struct {
        uint32              : 2;
        uint32 RNGEN        : 1;    // Random number generator enable
        uint32 IE           : 1;    // Interrupt enable
        uint32              : 1;
        uint32 CED          : 1;    // Clock error detection disable
        uint32              : 2;
        uint32 RNG_CONFIG3  : 4;    // RNG CONFIG3 field
        uint32 NISTC        : 1;    // NIST compliance mode
        uint32 RNG_CONFIG2  : 3;    // RNG CONFIG2 field
        uint32 CLKDIV       : 4;    // Clock divider
        uint32 RNG_CONFIG1  : 6;    // RNG CONFIG1 field
        uint32              : 4;
        uint32 CONDRST      : 1;    // Conditioning soft reset
        uint32 CONFIGLOCK   : 1;    // Configuration lock
    } bit;
    uint32 val;
};

union RNG_SR {
    struct {
        uint32 DRDY : 1;    // Data ready
        uint32 CECS : 1;    // Current clock error status
        uint32 SECS : 1;    // Current seed error status
        uint32      : 2;
        uint32 CEIS : 1;    // Clock error interrupt status
        uint32 SEIS : 1;    // Seed error interrupt status
        uint32      : 25;
    } bit;
    uint32 val;
};

union RNG_DR {
    struct {
        uint32 RNDATA   : 32;   // Random number
    } bit;
    uint32 val;
};

union RNG_HTCR {
    struct {
        uint32 HTCFG    : 32;   // Health test configuration
    } bit;
    uint32 val;
};


struct REG_RNG
{
    RNG_CR      CR;         // RNG control register              Address offset: 0x00
    RNG_SR      SR;         // RNG status register               Address offset: 0x04
    RNG_DR      DR;         // RNG data register                 Address offset: 0x08
    uint32      RESERVED0;  // Reserved                          Address offset: 0x0C
    RNG_HTCR    HTCR;       // RNG health test control register  Address offset: 0x10
};

#define REG_RNG ((volatile REG_RNG*)0x58001000u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
