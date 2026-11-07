/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: DAC
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union DAC_CR {
    struct {
        uint32 EN1          : 1;    // DAC channel1 enable
        uint32 TEN1         : 1;    // DAC channel1 trigger enable
        uint32 TSEL1        : 4;    // DAC channel1 trigger selection
        uint32 WAVE1        : 2;    // DAC channel1 noise/triangle wave generation enable
        uint32 MAMP1        : 4;    // DAC channel1 mask/amplitude selector
        uint32 DMAEN1       : 1;    // DAC channel1 DMA enable
        uint32 DMAUDRIE1    : 1;    // DAC channel1 DMA Underrun Interrupt enable
        uint32 CEN1         : 1;    // DAC channel1 calibration enable
        uint32              : 17;
    } bit;
    uint32 val;
};

union DAC_SWTRGR {
    struct {
        uint32 SWTRIG1  : 1;    // DAC channel1 software trigger
        uint32          : 31;
    } bit;
    uint32 val;
};

union DAC_DHR12R1 {
    struct {
        uint32 DACC1DHR : 12;   // DAC channel1 12-bit right-aligned data
        uint32          : 20;
    } bit;
    uint32 val;
};

union DAC_DHR12L1 {
    struct {
        uint32          : 4;
        uint32 DACC1DHR : 12;   // DAC channel1 12-bit left-aligned data
        uint32          : 16;
    } bit;
    uint32 val;
};

union DAC_DHR8R1 {
    struct {
        uint32 DACC1DHR : 8;    // DAC channel1 8-bit right-aligned data
        uint32          : 24;
    } bit;
    uint32 val;
};

union DAC_DOR1 {
    struct {
        uint32 DACC1DOR : 12;   // DAC channel1 data output
        uint32          : 20;
    } bit;
    uint32 val;
};

union DAC_SR {
    struct {
        uint32              : 13;
        uint32 DMAUDR1      : 1;    // DAC channel1 DMA underrun flag
        uint32 CAL_FLAG1    : 1;    // DAC channel1 calibration offset status
        uint32 BWST1        : 1;    // DAC channel1 busy writing sample time flag
        uint32              : 16;
    } bit;
    uint32 val;
};

union DAC_CCR {
    struct {
        uint32 OTRIM1   : 5;    // DAC channel1 offset trimming value
        uint32          : 27;
    } bit;
    uint32 val;
};

union DAC_MCR {
    struct {
        uint32 MODE1    : 3;    // DAC channel1 mode
        uint32          : 29;
    } bit;
    uint32 val;
};

union DAC_SHSR1 {
    struct {
        uint32 TSAMPLE1 : 10;   // DAC channel1 sample time (only valid in Sample and hold mode)
        uint32          : 22;
    } bit;
    uint32 val;
};

union DAC_SHHR {
    struct {
        uint32 THOLD1   : 10;   // DAC channel1 hold time (only valid in Sample and hold mode)
        uint32          : 22;
    } bit;
    uint32 val;
};

union DAC_SHRR {
    struct {
        uint32 TREFRESH1    : 8;    // DAC channel1 refresh time (only valid in Sample and hold mode)
        uint32              : 24;
    } bit;
    uint32 val;
};

struct REG_DAC
{
    DAC_CR      CR;             // Address offset: 0x000
    DAC_SWTRGR  SWTRGR;         // Address offset: 0x004
    DAC_DHR12R1 DHR12R1;        // Address offset: 0x008
    DAC_DHR12L1 DHR12L1;        // Address offset: 0x00C
    DAC_DHR8R1  DHR8R1;         // Address offset: 0x010
    uint32      RESERVED0[6];   // 0x014 - 0x028
    DAC_DOR1    DOR1;           // Address offset: 0x02C
    uint32      RESERVED1[1];   // 0x030 - 0x030
    DAC_SR      SR;             // Address offset: 0x034
    DAC_CCR     CCR;            // Address offset: 0x038
    DAC_MCR     MCR;            // Address offset: 0x03C
    DAC_SHSR1   SHSR1;          // Address offset: 0x040
    uint32      RESERVED2[1];   // 0x044 - 0x044
    DAC_SHHR    SHHR;           // Address offset: 0x048
    DAC_SHRR    SHRR;           // Address offset: 0x04C
};

#define REG_DAC ((volatile REG_DAC*)0x40007400u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
