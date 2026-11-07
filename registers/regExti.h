/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: EXTI
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                             TYPES & VARIABLES
******************************************************************************************************************************************/

union EXTI_RTSR1 {
    struct {
        uint32 RT0  : 1;    // Rising trigger selection for EXTI line 0
        uint32 RT1  : 1;    // Rising trigger selection for EXTI line 1
        uint32 RT2  : 1;    // Rising trigger selection for EXTI line 2
        uint32 RT3  : 1;    // Rising trigger selection for EXTI line 3
        uint32 RT4  : 1;    // Rising trigger selection for EXTI line 4
        uint32 RT5  : 1;    // Rising trigger selection for EXTI line 5
        uint32 RT6  : 1;    // Rising trigger selection for EXTI line 6
        uint32 RT7  : 1;    // Rising trigger selection for EXTI line 7
        uint32 RT8  : 1;    // Rising trigger selection for EXTI line 8
        uint32 RT9  : 1;    // Rising trigger selection for EXTI line 9
        uint32 RT10 : 1;    // Rising trigger selection for EXTI line 10
        uint32 RT11 : 1;    // Rising trigger selection for EXTI line 11
        uint32 RT12 : 1;    // Rising trigger selection for EXTI line 12
        uint32 RT13 : 1;    // Rising trigger selection for EXTI line 13
        uint32 RT14 : 1;    // Rising trigger selection for EXTI line 14
        uint32 RT15 : 1;    // Rising trigger selection for EXTI line 15
        uint32 RT16 : 1;    // Rising trigger selection for EXTI line 16
        uint32      : 4;
        uint32 RT21 : 1;    // Rising trigger selection for EXTI line 21
        uint32 RT22 : 1;    // Rising trigger selection for EXTI line 22
        uint32      : 9;
    } bit;
    uint32 val;
};

union EXTI_FTSR1 {
    struct {
        uint32 FT0  : 1;    // Falling trigger selection for EXTI line 0
        uint32 FT1  : 1;    // Falling trigger selection for EXTI line 1
        uint32 FT2  : 1;    // Falling trigger selection for EXTI line 2
        uint32 FT3  : 1;    // Falling trigger selection for EXTI line 3
        uint32 FT4  : 1;    // Falling trigger selection for EXTI line 4
        uint32 FT5  : 1;    // Falling trigger selection for EXTI line 5
        uint32 FT6  : 1;    // Falling trigger selection for EXTI line 6
        uint32 FT7  : 1;    // Falling trigger selection for EXTI line 7
        uint32 FT8  : 1;    // Falling trigger selection for EXTI line 8
        uint32 FT9  : 1;    // Falling trigger selection for EXTI line 9
        uint32 FT10 : 1;    // Falling trigger selection for EXTI line 10
        uint32 FT11 : 1;    // Falling trigger selection for EXTI line 11
        uint32 FT12 : 1;    // Falling trigger selection for EXTI line 12
        uint32 FT13 : 1;    // Falling trigger selection for EXTI line 13
        uint32 FT14 : 1;    // Falling trigger selection for EXTI line 14
        uint32 FT15 : 1;    // Falling trigger selection for EXTI line 15
        uint32 FT16 : 1;    // Falling trigger selection for EXTI line 16
        uint32      : 4;
        uint32 FT21 : 1;    // Falling trigger selection for EXTI line 21
        uint32 FT22 : 1;    // Falling trigger selection for EXTI line 22
        uint32      : 9;
    } bit;
    uint32 val;
};

union EXTI_PR1 {
    struct {
        uint32 PIF0     : 1;    // Pending interrupt flag for EXTI line 0
        uint32 PIF1     : 1;    // Pending interrupt flag for EXTI line 1
        uint32 PIF2     : 1;    // Pending interrupt flag for EXTI line 2
        uint32 PIF3     : 1;    // Pending interrupt flag for EXTI line 3
        uint32 PIF4     : 1;    // Pending interrupt flag for EXTI line 4
        uint32 PIF5     : 1;    // Pending interrupt flag for EXTI line 5
        uint32 PIF6     : 1;    // Pending interrupt flag for EXTI line 6
        uint32 PIF7     : 1;    // Pending interrupt flag for EXTI line 7
        uint32 PIF8     : 1;    // Pending interrupt flag for EXTI line 8
        uint32 PIF9     : 1;    // Pending interrupt flag for EXTI line 9
        uint32 PIF10    : 1;    // Pending interrupt flag for EXTI line 10
        uint32 PIF11    : 1;    // Pending interrupt flag for EXTI line 11
        uint32 PIF12    : 1;    // Pending interrupt flag for EXTI line 12
        uint32 PIF13    : 1;    // Pending interrupt flag for EXTI line 13
        uint32 PIF14    : 1;    // Pending interrupt flag for EXTI line 14
        uint32 PIF15    : 1;    // Pending interrupt flag for EXTI line 15
        uint32 PIF16    : 1;    // Pending interrupt flag for EXTI line 16
        uint32          : 4;
        uint32 PIF21    : 1;    // Pending interrupt flag for EXTI line 21
        uint32 PIF22    : 1;    // Pending interrupt flag for EXTI line 22
        uint32          : 9;
    } bit;
    uint32 val;
};

union EXTI_IMR1 {
    struct {
        uint32 IM   : 32;   // IM field
    } bit;
    uint32 val;
};

struct REG_EXTI
{
    EXTI_RTSR1  RTSR1;          // EXTI rising trigger selection register [31:0]             Address offset: 0x00
    EXTI_FTSR1  FTSR1;          // EXTI falling trigger selection register [31:0]            Address offset: 0x04
    uint32      SWIER1;         // EXTI software interrupt event register [31:0]             Address offset: 0x08
    EXTI_PR1    PR1;            // EXTI pending register [31:0]                              Address offset: 0x0C
    uint32      RESERVED1[4];   // Reserved                                                  Address offset: 0x10 - 0x1C
    uint32      RTSR2;          // EXTI rising trigger selection register [31:0]             Address offset: 0x20
    uint32      FTSR2;          // EXTI falling trigger selection register [31:0]            Address offset: 0x24
    uint32      SWIER2;         // EXTI software interrupt event register [31:0]             Address offset: 0x28
    uint32      PR2;            // EXTI pending register [31:0]                              Address offset: 0x2C
    uint32      RESERVED2[4];   // Reserved                                                  Address offset: 0x30 - 0x3C
    uint32      RESERVED3[8];   // Reserved                                                  Address offset: 0x40 - 0x5C
    uint32      RESERVED4[8];   // Reserved                                                  Address offset: 0x60 - 0x7C
    EXTI_IMR1   IMR1;           // EXTI wakeup with interrupt mask register for cpu1 [31:0]  Address offset: 0x80
    uint32      EMR1;           // EXTI wakeup with event mask register for cpu1 [31:0]      Address offset: 0x84
    uint32      RESERVED5[2];   // Reserved                                                  Address offset: 0x88 - 0x8C
    uint32      IMR2;           // EXTI wakeup with interrupt mask register for cpu1 [31:0]  Address offset: 0x90
    uint32      EMR2;           // EXTI wakeup with event mask register for cpu1 [31:0]      Address offset: 0x94
};

#define REG_EXTI ((volatile REG_EXTI*)0x58000800u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
