/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: PWR
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union PWR_CR1 {
    struct {
        uint32 LPMS             : 3; // Low-power mode selection
        uint32 SUBGHZSPINSSSEL  : 1; // sub-GHz SPI NSS source select
        uint32 FPDR             : 1; // Flash memory power-down mode during LPRun
        uint32 FPDS             : 1; // Flash memory power-down mode during LPSleep
        uint32                  : 2;
        uint32 DBP              : 1; // Disable backup domain write protection
        uint32 VOS              : 2; // Voltage scaling range selection
        uint32                  : 3;
        uint32 LPR              : 1; // LPRun
        uint32                  : 17;
    } bit;
    uint32 val;
};

union PWR_CR2 {
    struct {
        uint32 PVDE          : 1; // Programmable voltage detector enable
        uint32 PLS           : 3; // Programmable voltage detector level selection.
        uint32               : 2;
        uint32 PVME3         : 1; // Peripheral voltage monitoring 3 enable: VDDA versus 1.62 V
        uint32               : 25;
    } bit;
    uint32 val;
};

union PWR_CR3 {
    struct {
        uint32 EWUP1         : 1; // Wake-up pin WKUP1 for CPU enable
        uint32 EWUP2         : 1; // Wake-up pin WKUP2 for CPUenable
        uint32 EWUP3         : 1; // Wake-up pin WKUP3 for CPUenable
        uint32               : 4;
        uint32 ULPEN         : 1; // Ultra-low-power enable
        uint32 EWPVD         : 1; // PVD and wake-up for CPU enable (when sub-GHz radio in active state)
        uint32 RRS           : 1; // SRAM2 retention in Standby mode
        uint32 APC           : 1; // Apply pull-up and pull-down configuration from CPU
        uint32 EWRFBUSY      : 1; // radio busy wake-up from Standby for CPU enable
        uint32               : 1;
        uint32 EWRFIRQ       : 1; // radio IRQ[2:0] wake-up for CPU enable
        uint32               : 1;
        uint32 EIWUL         : 1; // internal wake-up line for CPU enable
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_CR4 {
    struct {
        uint32 WP1           : 1; // Wake-up pin WKUP1 polarity
        uint32 WP2           : 1; // Wake-up pin WKUP2 polarity
        uint32 WP3           : 1; // Wake-up pin WKUP3 polarity
        uint32               : 5;
        uint32 VBE           : 1; // VBAT battery charging enable
        uint32 VBRS          : 1; // VBAT battery charging resistor selection
        uint32               : 1;
        uint32 WRFBUSYP      : 1; // Wake-up radio busy polarity
        uint32               : 20;
    } bit;
    uint32 val;
};

union PWR_SR1 {
    struct {
        uint32 WUF1          : 1; // Wake-up flag 1
        uint32 WUF2          : 1; // Wake-up flag 2
        uint32 WUF3          : 1; // Wake-up flag 3
        uint32               : 5;
        uint32 WPVDF         : 1; // Wake-up PVD flag
        uint32               : 2;
        uint32 WRFBUSYF      : 1; // Radio busy wake-up flag
        uint32               : 3;
        uint32 WUFI          : 1; // Internal wake-up interrupt flag
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_SR2 {
    struct {
        uint32               : 1;
        uint32 RFBUSYS       : 1; // Radio busy signal status
        uint32 RFBUSYMS      : 1; // Radio busy masked signal status
        uint32 SMPSRDY       : 1; // SMPS ready flag
        uint32 LDORDY        : 1; // LDO ready flag
        uint32 RFEOLF        : 1; // Radio end-of-life flag
        uint32 REGMRS        : 1; // Main regulator status
        uint32 FLASHRDY      : 1; // Flash memory ready
        uint32 REGLPS        : 1; // low-power regulator started (ready)
        uint32 REGLPF        : 1; // low-power regulator flag
        uint32 VOSF          : 1; // Voltage scaling flag
        uint32 PVDO          : 1; // Programmable voltage detector output
        uint32               : 2;
        uint32 PVMO3         : 1; // Peripheral voltage monitoring output: VDDA versus 1.62 V
        uint32               : 17;
    } bit;
    uint32 val;
};

union PWR_SCR {
    struct {
        uint32 CWUF1         : 1; // Clear wake-up flag 1
        uint32 CWUF2         : 1; // Clear wake-up flag 2
        uint32 CWUF3         : 1; // Clear wake-up flag 3
        uint32               : 5;
        uint32 CWPVDF        : 1; // Clear wake-up PVD interrupt flag
        uint32               : 2;
        uint32 CWRFBUSYF     : 1; // Clear wake-up radio busy flag
        uint32               : 20;
    } bit;
    uint32 val;
};

union PWR_CR5 {
    struct {
        uint32               : 14;
        uint32 RFEOLEN       : 1; // sub-GHz radio end-of-life detector enable
        uint32 SMPSEN        : 1; // SMPS step-down converter enable
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_PUCRA {
    struct {
        uint32 PU            : 16; // Port PA[y] pull-up bit y (y = 0 to 15)
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_PDCRA {
    struct {
        uint32 PD            : 16; // Port PA[y] pull-down (y = 0 to 15)
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_PUCRB {
    struct {
        uint32 PU            : 16; // Port PB[y] pull-up (y = 0 to 15)
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_PDCRB {
    struct {
        uint32 PD            : 16; // Port PB[y] pull-down (y = 0 to 15)
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_PUCRC {
    struct {
        uint32 PU            : 7; // Port PC[y] pull-up (y = 0 to 6)
        uint32               : 6;
        uint32 PU_2          : 3; // Port PC[y] pull-up (y = 13 to 15)
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_PDCRC {
    struct {
        uint32 PD            : 7; // Port PC[y] pull-down bit y (y = 0 to 6)
        uint32               : 6;
        uint32 PD_2          : 3; // Port PC[y] pull-down (y = 13 to 15)
        uint32               : 16;
    } bit;
    uint32 val;
};

union PWR_PUCRH {
    struct {
        uint32               : 3;
        uint32 PU3           : 1; // Port PH[3] pull-up
        uint32               : 28;
    } bit;
    uint32 val;
};

union PWR_PDCRH {
    struct {
        uint32               : 3;
        uint32 PD3           : 1; // Port PH[3] pull-down
        uint32               : 28;
    } bit;
    uint32 val;
};

union PWR_EXTSCR {
    struct {
        uint32 C1CSSF        : 1; // Clear CPU Stop Standby flags
        uint32               : 7;
        uint32 C1SBF         : 1; // System Standby flag for CPU (no core states retained)
        uint32 C1STOP2F      : 1; // System Stop 2 flag for CPU (partial core states retained)
        uint32 C1STOPF       : 1; // System Stop 0, 1 flag for CPU (all core states retained)
        uint32               : 3;
        uint32 C1DS          : 1; // CPU Deep-Sleep mode
        uint32               : 17;
    } bit;
    uint32 val;
};

union PWR_SUBGHZSPICR {
    struct {
        uint32               : 15;
        uint32 NSS           : 1; // sub-GHz SPI NSS control
        uint32               : 16;
    } bit;
    uint32 val;
};

struct REG_PWR
{
    PWR_CR1             CR1; // Address offset: 0x000
    PWR_CR2             CR2; // Address offset: 0x004
    PWR_CR3             CR3; // Address offset: 0x008
    PWR_CR4             CR4; // Address offset: 0x00C
    PWR_SR1             SR1; // Address offset: 0x010
    PWR_SR2             SR2; // Address offset: 0x014
    PWR_SCR             SCR; // Address offset: 0x018
    PWR_CR5             CR5; // Address offset: 0x01C
    PWR_PUCRA           PUCRA; // Address offset: 0x020
    PWR_PDCRA           PDCRA; // Address offset: 0x024
    PWR_PUCRB           PUCRB; // Address offset: 0x028
    PWR_PDCRB           PDCRB; // Address offset: 0x02C
    PWR_PUCRC           PUCRC; // Address offset: 0x030
    PWR_PDCRC           PDCRC; // Address offset: 0x034
    uint32              RESERVED0[8]; // 0x038 - 0x054
    PWR_PUCRH           PUCRH; // Address offset: 0x058
    PWR_PDCRH           PDCRH; // Address offset: 0x05C
    uint32              RESERVED1[10]; // 0x060 - 0x084
    PWR_EXTSCR          EXTSCR; // Address offset: 0x088
    uint32              RESERVED2[1]; // 0x08C - 0x08C
    PWR_SUBGHZSPICR     SUBGHZSPICR; // Address offset: 0x090
};

#define REG_PWR ((volatile REG_PWR*)0x58000400u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
