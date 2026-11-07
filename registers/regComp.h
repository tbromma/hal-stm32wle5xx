/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: COMP
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union COMP1_CSR {
    struct {
        uint32 EN       : 1;    // COMP1 enable
        uint32          : 1;
        uint32 PWRMODE  : 2;    // COMP1 power mode
        uint32 INMSEL   : 3;    // COMP1 input minus selection
        uint32 INPSEL   : 2;    // COMP1 input plus selection
        uint32          : 6;
        uint32 POLARITY : 1;    // COMP1 polarity selection
        uint32 HYST     : 2;    // COMP1 hysteresis selection
        uint32 BLANKING : 3;    // COMP1 blanking source selection
        uint32          : 1;
        uint32 BRGEN    : 1;    // scaler bridge enable
        uint32 SCALEN   : 1;    // voltage scaler enable
        uint32          : 1;
        uint32 INMESEL  : 2;    // COMP1 input minus extended selection
        uint32          : 3;
        uint32 VALUE    : 1;    // COMP1 output status bit
        uint32 LOCK     : 1;    // locks the whole content of the register, COMP1_CSR[31:0]
    } bit;
    uint32 val;
};

union COMP2_CSR {
    struct {
        uint32 EN       : 1;    // COMP2 enable
        uint32          : 1;
        uint32 PWRMODE  : 2;    // COMP2 power mode
        uint32 INMSEL   : 3;    // COMP2 input minus selection
        uint32 INPSEL   : 2;    // COMP2 input plus selection
        uint32 WINMODE  : 1;    // window mode selection
        uint32          : 5;
        uint32 POLARITY : 1;    // COMP2 polarity selection
        uint32 HYST     : 2;    // COMP2 hysteresis selection
        uint32 BLANKING : 3;    // COMP2 blanking source selection
        uint32          : 1;
        uint32 BRGEN    : 1;    // scaler bridge enable
        uint32 SCALEN   : 1;    // voltage scaler enable
        uint32          : 1;
        uint32 INMESEL  : 2;    // COMP2 input minus extended selection
        uint32          : 3;
        uint32 VALUE    : 1;    // COMP2 output status bit
        uint32 LOCK     : 1;    // locks the whole content of the register, COMP2_CSR[31:0]
    } bit;
    uint32 val;
};

struct REG_COMP
{
    COMP1_CSR   CSR1;  // Address offset: 0x00
    COMP2_CSR   CSR2;  // Address offset: 0x04
};

#define REG_COMP ((volatile REG_COMP*)0x40010200u)
#define REG_COMP1_CSR ((volatile COMP1_CSR*)0x40010200u)
#define REG_COMP2_CSR ((volatile COMP2_CSR*)0x40010204u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
