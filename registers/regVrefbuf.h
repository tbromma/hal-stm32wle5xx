/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: VREFBUF
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union VREFBUF_CSR {
    struct {
        uint32 ENVR : 1;    // Voltage reference buffer mode enable
        uint32 HIZ  : 1;    // High impedance mode
        uint32 VRS  : 1;    // Voltage reference scale
        uint32 VRR  : 1;    // Voltage reference buffer ready
        uint32      : 28;
    } bit;
    uint32 val;
};

union VREFBUF_CCR {
    struct {
        uint32 TRIM : 6;    // Trimming code
        uint32      : 26;
    } bit;
    uint32 val;
};

struct REG_VREFBUF
{
    VREFBUF_CSR CSR;    // Address offset: 0x000
    VREFBUF_CCR CCR;    // Address offset: 0x004
};

#define REG_VREFBUF ((volatile REG_VREFBUF*)0x40010030u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
