/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: ADC
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union ADC_ISR {
    struct {
        uint32 ADRDY    : 1;    // ADC ready
        uint32 EOSMP    : 1;    // End of sampling flag
        uint32 EOC      : 1;    // End of conversion flag
        uint32 EOS      : 1;    // End of sequence flag
        uint32 OVR      : 1;    // ADC overrun
        uint32          : 2;
        uint32 AWD1     : 1;    // Analog watchdog 1 flag
        uint32 AWD2     : 1;    // Analog watchdog 2 flag
        uint32 AWD3     : 1;    // Analog watchdog 3 flag
        uint32          : 1;
        uint32 EOCAL    : 1;    // End Of Calibration flag
        uint32          : 1;
        uint32 CCRDY    : 1;    // Channel Configuration Ready flag
        uint32          : 18;
    } bit;
    uint32 val;
};

union ADC_IER {
    struct {
        uint32 ADRDYIE  : 1;    // ADC ready interrupt enable
        uint32 EOSMPIE  : 1;    // End of sampling flag interrupt enable
        uint32 EOCIE    : 1;    // End of conversion interrupt enable
        uint32 EOSIE    : 1;    // End of conversion sequence interrupt enable
        uint32 OVRIE    : 1;    // Overrun interrupt enable
        uint32          : 2;
        uint32 AWD1IE   : 1;    // Analog watchdog 1 interrupt enable
        uint32 AWD2IE   : 1;    // Analog watchdog 2 interrupt enable
        uint32 AWD3IE   : 1;    // Analog watchdog 3 interrupt enable
        uint32          : 1;
        uint32 EOCALIE  : 1;    // End of calibration interrupt enable
        uint32          : 1;
        uint32 CCRDYIE  : 1;    // Channel Configuration Ready Interrupt enable
        uint32          : 18;
    } bit;
    uint32 val;
};

union ADC_CR {
    struct {
        uint32 ADEN     : 1;    // ADC enable command
        uint32 ADDIS    : 1;    // ADC disable command
        uint32 ADSTART  : 1;    // ADC start conversion command
        uint32          : 1;
        uint32 ADSTP    : 1;    // ADC stop conversion command
        uint32          : 23;
        uint32 ADVREGEN : 1;    // ADC Voltage Regulator Enable
        uint32          : 2;
        uint32 ADCAL    : 1;    // ADC calibration
    } bit;
    uint32 val;
};

union ADC_CFGR1 {
    struct {
        uint32 DMAEN        : 1;    // Direct memory access enable
        uint32 DMACFG       : 1;    // Direct memory access configuration
        uint32 SCANDIR      : 1;    // Scan sequence direction
        uint32 RES          : 2;    // Data resolution
        uint32 ALIGN        : 1;    // Data alignment
        uint32 EXTSEL       : 3;    // External trigger selection
        uint32              : 1;
        uint32 EXTEN        : 2;    // External trigger enable and polarity selection
        uint32 OVRMOD       : 1;    // Overrun management mode
        uint32 CONT         : 1;    // Single / continuous conversion mode
        uint32 WAIT         : 1;    // Wait conversion mode
        uint32 AUTOFF       : 1;    // Auto-off mode
        uint32 DISCEN       : 1;    // Discontinuous mode
        uint32              : 4;
        uint32 CHSELRMOD    : 1;    // Mode selection of the ADC_CHSELR register
        uint32 AWD1SGL      : 1;    // Enable the watchdog on a single channel or on all channels
        uint32 AWD1EN       : 1;    // Analog watchdog enable
        uint32              : 2;
        uint32 AWD1CH       : 5;    // Analog watchdog channel selection
        uint32              : 1;
    } bit;
    uint32 val;
};

union ADC_CFGR2 {
    struct {
        uint32 OVSE     : 1;    // Oversampler Enable
        uint32          : 1;
        uint32 OVSR     : 3;    // Oversampling ratio
        uint32 OVSS     : 4;    // Oversampling shift
        uint32 TOVS     : 1;    // Triggered Oversampling
        uint32          : 19;
        uint32 LFTRIG   : 1;    // Low frequency trigger mode enable
        uint32 CKMODE   : 2;    // ADC clock mode
    } bit;
    uint32 val;
};

union ADC_SMPR {
    struct {
        uint32 SMP1     : 3;    // Sampling time selection 1
        uint32          : 1;
        uint32 SMP2     : 3;    // Sampling time selection 2
        uint32          : 1;
        uint32 SMPSELX  : 18;   // Channel-x sampling time selection (x = 17 to 0)
        uint32          : 6;
    } bit;
    uint32 val;
};

union ADC_AWD1TR {
    struct {
        uint32 LT1  : 12;   // Analog watchdog 1 lower threshold
        uint32      : 4;
        uint32 HT1  : 12;   // Analog watchdog 1 higher threshold
        uint32      : 4;
    } bit;
    uint32 val;
};

union ADC_AWD2TR {
    struct {
        uint32 LT2  : 12;   // Analog watchdog 2 lower threshold
        uint32      : 4;
        uint32 HT2  : 12;   // Analog watchdog 2 higher threshold
        uint32      : 4;
    } bit;
    uint32 val;
};

union ADC_CHSELR {
    struct {
        uint32 CHSEL    : 18;   // Channel-x selection
        uint32          : 14;
    } bit;
    uint32 val;
};

union ADC_AWD3TR {
    struct {
        uint32 LT3  : 12;   // Analog watchdog 3lower threshold
        uint32      : 4;
        uint32 HT3  : 12;   // Analog watchdog 3 higher threshold
        uint32      : 4;
    } bit;
    uint32 val;
};

union ADC_DR {
    struct {
        uint32 DATA : 16;   // Converted data
        uint32      : 16;
    } bit;
    uint32 val;
};

union ADC_AWD2CR {
    struct {
        uint32 AWD2CH   : 18;   // Analog watchdog channel selection
        uint32          : 14;
    } bit;
    uint32 val;
};

union ADC_AWD3CR {
    struct {
        uint32 AWD3CH   : 18;   // Analog watchdog channel selection
        uint32          : 14;
    } bit;
    uint32 val;
};

union ADC_CALFACT {
    struct {
        uint32 CALFACT  : 7;    // Calibration factor
        uint32          : 25;
    } bit;
    uint32 val;
};

union ADC_CCR {
    struct {
        uint32          : 18;
        uint32 PRESC    : 4;    // ADC prescaler
        uint32 VREFEN   : 1;    // VREFINT enable
        uint32 TSEN     : 1;    // Temperature sensor enable
        uint32 VBATEN   : 1;    // VBAT enable
        uint32          : 7;
    } bit;
    uint32 val;
};

struct REG_ADC
{
    ADC_ISR     ISR;            // Address offset: 0x000
    ADC_IER     IER;            // Address offset: 0x004
    ADC_CR      CR;             // Address offset: 0x008
    ADC_CFGR1   CFGR1;          // Address offset: 0x00C
    ADC_CFGR2   CFGR2;          // Address offset: 0x010
    ADC_SMPR    SMPR;           // Address offset: 0x014
    uint32      RESERVED0[2];   // 0x018 - 0x01C
    ADC_AWD1TR  AWD1TR;         // Address offset: 0x020
    ADC_AWD2TR  AWD2TR;         // Address offset: 0x024
    ADC_CHSELR  CHSELR;         // Address offset: 0x028
    ADC_AWD3TR  AWD3TR;         // Address offset: 0x02C
    uint32      RESERVED1[4];   // 0x030 - 0x03C
    ADC_DR      DR;             // Address offset: 0x040
    uint32      RESERVED2[23];  // 0x044 - 0x09C
    ADC_AWD2CR  AWD2CR;         // Address offset: 0x0A0
    ADC_AWD3CR  AWD3CR;         // Address offset: 0x0A4
    uint32      RESERVED3[3];   // 0x0A8 - 0x0B0
    ADC_CALFACT CALFACT;        // Address offset: 0x0B4
    uint32      RESERVED4[148]; // 0x0B8 - 0x304
    ADC_CCR     CCR;            // Address offset: 0x308
};

#define REG_ADC ((volatile REG_ADC*)0x40012400u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
