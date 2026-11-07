/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: TAMP
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union TAMP_CR1 {
    struct {
        uint32 TAMP1E        : 1; // Tamper detection on TAMP_IN1 enable(1)
        uint32 TAMP2E        : 1; // Tamper detection on TAMP_IN2 enable(1)
        uint32 TAMP3E        : 1; // Tamper detection on TAMP_IN3 enable(1)
        uint32               : 15;
        uint32 ITAMP3E       : 1; // Internal tamper 3 enable
        uint32               : 1;
        uint32 ITAMP5E       : 1; // Internal tamper 5 enable
        uint32 ITAMP6E       : 1; // Internal tamper 6 enable
        uint32               : 1;
        uint32 ITAMP8E       : 1; // Internal tamper 8 enable
        uint32               : 8;
    } bit;
    uint32 reg;
};

union TAMP_CR2 {
    struct {
        uint32 TAMP1NOER     : 1; // Tamper 1 no erase
        uint32 TAMP2NOER     : 1; // Tamper 2 no erase
        uint32 TAMP3NOER     : 1; // Tamper 3 no erase
        uint32               : 13;
        uint32 TAMP1MSK      : 1; // Tamper 1 mask
        uint32 TAMP2MSK      : 1; // Tamper 2 mask
        uint32 TAMP3MSK      : 1; // Tamper 3 mask
        uint32               : 4;
        uint32 BKERASE       : 1; // Backup registers(1) erase
        uint32 TAMP1TRG      : 1; // Active level for tamper 1 input
        uint32 TAMP2TRG      : 1; // Active level for tamper 2 input
        uint32 TAMP3TRG      : 1; // Active level for tamper 3 input
        uint32               : 5;
    } bit;
    uint32 reg;
};

union TAMP_BKPXR {
    struct {
        uint32 BKP           : 32;
    } bit;
    uint32 reg;
};

union TAMP_CR3 {
    struct {
        uint32               : 2;
        uint32 ITAMP3NOER    : 1; // Internal Tamper 3 no erase
        uint32               : 1;
        uint32 ITAMP5NOER    : 1; // Internal Tamper 5 no erase
        uint32 ITAMP6NOER    : 1; // Internal Tamper 6 no erase
        uint32               : 1;
        uint32 ITAMP8NOER    : 1; // Internal Tamper 8 no erase
        uint32               : 24;
    } bit;
    uint32 reg;
};

union TAMP_FLTCR {
    struct {
        uint32 TAMPFREQ      : 3; // Tamper sampling frequency
        uint32 TAMPFLT       : 2; // TAMP_INx filter count
        uint32 TAMPPRCH      : 2; // TAMP_INx precharge duration
        uint32 TAMPPUDIS     : 1; // TAMP_INx pull-up disable
        uint32               : 24;
    } bit;
    uint32 reg;
};

union TAMP_IER {
    struct {
        uint32 TAMP1IE       : 1; // Tamper 1 interrupt enable
        uint32 TAMP2IE       : 1; // Tamper 2 interrupt enable
        uint32 TAMP3IE       : 1; // Tamper 3 interrupt enable
        uint32               : 15;
        uint32 ITAMP3IE      : 1; // Internal tamper 3 interrupt enable
        uint32               : 1;
        uint32 ITAMP5IE      : 1; // Internal tamper 5 interrupt enable
        uint32 ITAMP6IE      : 1; // Internal tamper 6 interrupt enable
        uint32               : 1;
        uint32 ITAMP8IE      : 1; // Internal tamper 8 interrupt enable
        uint32               : 8;
    } bit;
    uint32 reg;
};

union TAMP_SR {
    struct {
        uint32 TAMP1F        : 1; // TAMP1 detection flag
        uint32 TAMP2F        : 1; // TAMP2 detection flag
        uint32 TAMP3F        : 1; // TAMP3 detection flag
        uint32               : 15;
        uint32 ITAMP3F       : 1; // Internal tamper 3 flag
        uint32               : 1;
        uint32 ITAMP5F       : 1; // Internal tamper 5 flag
        uint32 ITAMP6F       : 1; // Internal tamper 6 flag
        uint32               : 1;
        uint32 ITAMP8F       : 1; // Internal tamper 8 flag
        uint32               : 8;
    } bit;
    uint32 reg;
};

union TAMP_MISR {
    struct {
        uint32 TAMP1MF       : 1; // TAMP1 interrupt masked flag
        uint32 TAMP2MF       : 1; // TAMP2 interrupt masked flag
        uint32 TAMP3MF       : 1; // TAMP3 interrupt masked flag
        uint32               : 15;
        uint32 ITAMP3MF      : 1; // Internal tamper 3 interrupt masked flag
        uint32               : 1;
        uint32 ITAMP5MF      : 1; // Internal tamper 5 interrupt masked flag
        uint32 ITAMP6MF      : 1; // Internal tamper 6 interrupt masked flag
        uint32               : 1;
        uint32 ITAMP8MF      : 1; // Internal tamper 8 interrupt masked flag
        uint32               : 8;
    } bit;
    uint32 reg;
};

union TAMP_SCR {
    struct {
        uint32 CTAMP1F       : 1; // Clear TAMP1 detection flag
        uint32 CTAMP2F       : 1; // Clear TAMP2 detection flag
        uint32 CTAMP3F       : 1; // Clear TAMP3 detection flag
        uint32               : 15;
        uint32 CITAMP3F      : 1; // Clear ITAMP3 detection flag
        uint32               : 1;
        uint32 CITAMP5F      : 1; // Clear ITAMP5 detection flag
        uint32 CITAMP6F      : 1; // Clear ITAMP6 detection flag
        uint32               : 1;
        uint32 CITAMP8F      : 1; // Clear ITAMP8 detection flag
        uint32               : 8;
    } bit;
    uint32 reg;
};

union TAMP_COUNTR {
    struct {
        uint32 COUNT         : 32;
    } bit;
    uint32 reg;
};

struct REG_TAMP
{
    TAMP_CR1            CR1; // Address offset: 0x000
    TAMP_CR2            CR2; // Address offset: 0x004
    TAMP_CR3            CR3; // Address offset: 0x008
    TAMP_FLTCR          FLTCR; // Address offset: 0x00C
    uint32              RESERVED0[7]; // 0x010 - 0x028
    TAMP_IER            IER; // Address offset: 0x02C
    TAMP_SR             SR; // Address offset: 0x030
    TAMP_MISR           MISR; // Address offset: 0x034
    uint32              RESERVED1[1]; // 0x038 - 0x038
    TAMP_SCR            SCR; // Address offset: 0x03C
    TAMP_COUNTR         COUNTR; // Address offset: 0x040
};

#define REG_TAMP ((volatile REG_TAMP*)0x4000B000u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
