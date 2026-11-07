/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: DMAMUX
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union DMAMUX_CxCR {
    struct {
        uint32 DMAREQ_ID    : 8;    // DMA request identification
        uint32 SOIE         : 1;    // Synchronization overrun interrupt enable
        uint32 EGE          : 1;    // Event generation enable
        uint32              : 6;
        uint32 SE           : 1;    // Synchronization enable
        uint32 SPOL         : 2;    // Synchronization polarity
        uint32 NBREQ        : 5;    // Number of DMA requests minus 1 to forward
        uint32 SYNC_ID      : 5;    // Synchronization identification
        uint32              : 3;
    } bit;
    uint32 val;
};

union DMAMUX_CSR {
    struct {
        uint32 SOF0     : 1;    // Synchronization overrun event flag channel 0
        uint32 SOF1     : 1;    // Synchronization overrun event flag channel 1
        uint32 SOF2     : 1;    // Synchronization overrun event flag channel 2
        uint32 SOF3     : 1;    // Synchronization overrun event flag channel 3
        uint32 SOF4     : 1;    // Synchronization overrun event flag channel 4
        uint32 SOF5     : 1;    // Synchronization overrun event flag channel 5
        uint32 SOF6     : 1;    // Synchronization overrun event flag channel 6
        uint32 SOF7     : 1;    // Synchronization overrun event flag channel 7
        uint32 SOF8     : 1;    // Synchronization overrun event flag channel 8
        uint32 SOF9     : 1;    // Synchronization overrun event flag channel 9
        uint32 SOF10    : 1;    // Synchronization overrun event flag channel 10
        uint32 SOF11    : 1;    // Synchronization overrun event flag channel 11
        uint32 SOF12    : 1;    // Synchronization overrun event flag channel 12
        uint32 SOF13    : 1;    // Synchronization overrun event flag channel 13
        uint32          : 18;
    } bit;
    uint32 val;
};

union DMAMUX_CCFR {
    struct {
        uint32 CSOF0    : 1;    // Clear synchronization overrun event flag channel 0
        uint32 CSOF1    : 1;    // Clear synchronization overrun event flag channel 1
        uint32 CSOF2    : 1;    // Clear synchronization overrun event flag channel 2
        uint32 CSOF3    : 1;    // Clear synchronization overrun event flag channel 3
        uint32 CSOF4    : 1;    // Clear synchronization overrun event flag channel 4
        uint32 CSOF5    : 1;    // Clear synchronization overrun event flag channel 5
        uint32 CSOF6    : 1;    // Clear synchronization overrun event flag channel 6
        uint32 CSOF7    : 1;    // Clear synchronization overrun event flag channel 7
        uint32 CSOF8    : 1;    // Clear synchronization overrun event flag channel 8
        uint32 CSOF9    : 1;    // Clear synchronization overrun event flag channel 9
        uint32 CSOF10   : 1;    // Clear synchronization overrun event flag channel 10
        uint32 CSOF11   : 1;    // Clear synchronization overrun event flag channel 11
        uint32 CSOF12   : 1;    // Clear synchronization overrun event flag channel 12
        uint32 CSOF13   : 1;    // Clear synchronization overrun event flag channel 13
        uint32          : 18;
    } bit;
    uint32 val;
};

union DMAMUX_RGxCR {
    struct {
        uint32 SIG_ID   : 5;    // Signal identification
        uint32          : 3;
        uint32 OIE      : 1;    // Trigger overrun interrupt enable
        uint32          : 7;
        uint32 GE       : 1;    // DMA request generator channel enable
        uint32 GPOL     : 2;    // DMA request generator trigger polarity
        uint32 GNBREQ   : 5;    // Number of DMA requests to be generated minus 1
        uint32          : 8;
    } bit;
    uint32 val;
};

union DMAMUX_RGSR {
    struct {
        uint32 OF0  : 1;    // Trigger overrun event flag generator 0
        uint32 OF1  : 1;    // Trigger overrun event flag generator 1
        uint32 OF2  : 1;    // Trigger overrun event flag generator 2
        uint32 OF3  : 1;    // Trigger overrun event flag generator 3
        uint32      : 28;
    } bit;
    uint32 val;
};

union DMAMUX_RGCFR {
    struct {
        uint32 COF0 : 1;    // Clear trigger overrun event flag generator 0
        uint32 COF1 : 1;    // Clear trigger overrun event flag generator 1
        uint32 COF2 : 1;    // Clear trigger overrun event flag generator 2
        uint32 COF3 : 1;    // Clear trigger overrun event flag generator 3
        uint32      : 28;
    } bit;
    uint32 val;
};

struct REG_DMAMUX_
{
    union {
        struct {
            DMAMUX_CxCR     C0CR;           // Address offset: 0x000
            DMAMUX_CxCR     C1CR;           // Address offset: 0x004
            DMAMUX_CxCR     C2CR;           // Address offset: 0x008
            DMAMUX_CxCR     C3CR;           // Address offset: 0x00C
            DMAMUX_CxCR     C4CR;           // Address offset: 0x010
            DMAMUX_CxCR     C5CR;           // Address offset: 0x014
            DMAMUX_CxCR     C6CR;           // Address offset: 0x018
            DMAMUX_CxCR     C7CR;           // Address offset: 0x01C
            DMAMUX_CxCR     C8CR;           // Address offset: 0x020
            DMAMUX_CxCR     C9CR;           // Address offset: 0x024
            DMAMUX_CxCR     C10CR;          // Address offset: 0x028
            DMAMUX_CxCR     C11CR;          // Address offset: 0x02C
            DMAMUX_CxCR     C12CR;          // Address offset: 0x030
            DMAMUX_CxCR     C13CR;          // Address offset: 0x034
        };
        DMAMUX_CxCR CxCR[14];
    };
    uint32          RESERVED0[18];  // 0x038 - 0x07C
    DMAMUX_CSR      CSR;            // Address offset: 0x080
    DMAMUX_CCFR     CCFR;           // Address offset: 0x084
    uint32          RESERVED1[30];  // 0x088 - 0x0FC
    DMAMUX_RGxCR    RG0CR;          // Address offset: 0x100
    DMAMUX_RGxCR    RG1CR;          // Address offset: 0x104
    DMAMUX_RGxCR    RG2CR;          // Address offset: 0x108
    DMAMUX_RGxCR    RG3CR;          // Address offset: 0x10C
    uint32          RESERVED2[12];  // 0x110 - 0x13C
    DMAMUX_RGSR     RGSR;           // Address offset: 0x140
    DMAMUX_RGCFR    RGCFR;          // Address offset: 0x144
};

#define REG_DMAMUX ((volatile REG_DMAMUX_*)0x40020800u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
