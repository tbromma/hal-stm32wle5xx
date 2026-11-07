/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: GPIO
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union GPIO_MODER {
    struct {
        uint32 MODE0    : 2;    // Port x configuration bits for pin 0
        uint32 MODE1    : 2;    // Port x configuration bits for pin 1
        uint32 MODE2    : 2;    // Port x configuration bits for pin 2
        uint32 MODE3    : 2;    // Port x configuration bits for pin 3
        uint32 MODE4    : 2;    // Port x configuration bits for pin 4
        uint32 MODE5    : 2;    // Port x configuration bits for pin 5
        uint32 MODE6    : 2;    // Port x configuration bits for pin 6
        uint32 MODE7    : 2;    // Port x configuration bits for pin 7
        uint32 MODE8    : 2;    // Port x configuration bits for pin 8
        uint32 MODE9    : 2;    // Port x configuration bits for pin 9
        uint32 MODE10   : 2;    // Port x configuration bits for pin 10
        uint32 MODE11   : 2;    // Port x configuration bits for pin 11
        uint32 MODE12   : 2;    // Port x configuration bits for pin 12
        uint32 MODE13   : 2;    // Port x configuration bits for pin 13
        uint32 MODE14   : 2;    // Port x configuration bits for pin 14
        uint32 MODE15   : 2;    // Port x configuration bits for pin 15
    } bit;
    uint32 val;
};

union GPIO_OTYPER {
    struct {
        uint32 OT0  : 1;    // Port x output type for pin 0
        uint32 OT1  : 1;    // Port x output type for pin 1
        uint32 OT2  : 1;    // Port x output type for pin 2
        uint32 OT3  : 1;    // Port x output type for pin 3
        uint32 OT4  : 1;    // Port x output type for pin 4
        uint32 OT5  : 1;    // Port x output type for pin 5
        uint32 OT6  : 1;    // Port x output type for pin 6
        uint32 OT7  : 1;    // Port x output type for pin 7
        uint32 OT8  : 1;    // Port x output type for pin 8
        uint32 OT9  : 1;    // Port x output type for pin 9
        uint32 OT10 : 1;    // Port x output type for pin 10
        uint32 OT11 : 1;    // Port x output type for pin 11
        uint32 OT12 : 1;    // Port x output type for pin 12
        uint32 OT13 : 1;    // Port x output type for pin 13
        uint32 OT14 : 1;    // Port x output type for pin 14
        uint32 OT15 : 1;    // Port x output type for pin 15
        uint32      : 16;
    } bit;
    uint32 val;
};

union GPIO_OSPEEDR {
    struct {
        uint32 OSPEED0  : 2;    // Port x output speed for pin 0
        uint32 OSPEED1  : 2;    // Port x output speed for pin 1
        uint32 OSPEED2  : 2;    // Port x output speed for pin 2
        uint32 OSPEED3  : 2;    // Port x output speed for pin 3
        uint32 OSPEED4  : 2;    // Port x output speed for pin 4
        uint32 OSPEED5  : 2;    // Port x output speed for pin 5
        uint32 OSPEED6  : 2;    // Port x output speed for pin 6
        uint32 OSPEED7  : 2;    // Port x output speed for pin 7
        uint32 OSPEED8  : 2;    // Port x output speed for pin 8
        uint32 OSPEED9  : 2;    // Port x output speed for pin 9
        uint32 OSPEED10 : 2;    // Port x output speed for pin 10
        uint32 OSPEED11 : 2;    // Port x output speed for pin 11
        uint32 OSPEED12 : 2;    // Port x output speed for pin 12
        uint32 OSPEED13 : 2;    // Port x output speed for pin 13
        uint32 OSPEED14 : 2;    // Port x output speed for pin 14
        uint32 OSPEED15 : 2;    // Port x output speed for pin 15
    } bit;
    uint32 val;
};

union GPIO_PUPDR {
    struct {
        uint32 PUPD0    : 2;    // Port x pull-up/pull-down configuration for pin 0
        uint32 PUPD1    : 2;    // Port x pull-up/pull-down configuration for pin 1
        uint32 PUPD2    : 2;    // Port x pull-up/pull-down configuration for pin 2
        uint32 PUPD3    : 2;    // Port x pull-up/pull-down configuration for pin 3
        uint32 PUPD4    : 2;    // Port x pull-up/pull-down configuration for pin 4
        uint32 PUPD5    : 2;    // Port x pull-up/pull-down configuration for pin 5
        uint32 PUPD6    : 2;    // Port x pull-up/pull-down configuration for pin 6
        uint32 PUPD7    : 2;    // Port x pull-up/pull-down configuration for pin 7
        uint32 PUPD8    : 2;    // Port x pull-up/pull-down configuration for pin 8
        uint32 PUPD9    : 2;    // Port x pull-up/pull-down configuration for pin 9
        uint32 PUPD10   : 2;    // Port x pull-up/pull-down configuration for pin 10
        uint32 PUPD11   : 2;    // Port x pull-up/pull-down configuration for pin 11
        uint32 PUPD12   : 2;    // Port x pull-up/pull-down configuration for pin 12
        uint32 PUPD13   : 2;    // Port x pull-up/pull-down configuration for pin 13
        uint32 PUPD14   : 2;    // Port x pull-up/pull-down configuration for pin 14
        uint32 PUPD15   : 2;    // Port x pull-up/pull-down configuration for pin 15
    } bit;
    uint32 val;
};

union GPIO_IDR {
    struct {
        uint32 ID0  : 1;    // Port x input data for pin 0
        uint32 ID1  : 1;    // Port x input data for pin 1
        uint32 ID2  : 1;    // Port x input data for pin 2
        uint32 ID3  : 1;    // Port x input data for pin 3
        uint32 ID4  : 1;    // Port x input data for pin 4
        uint32 ID5  : 1;    // Port x input data for pin 5
        uint32 ID6  : 1;    // Port x input data for pin 6
        uint32 ID7  : 1;    // Port x input data for pin 7
        uint32 ID8  : 1;    // Port x input data for pin 8
        uint32 ID9  : 1;    // Port x input data for pin 9
        uint32 ID10 : 1;    // Port x input data for pin 10
        uint32 ID11 : 1;    // Port x input data for pin 11
        uint32 ID12 : 1;    // Port x input data for pin 12
        uint32 ID13 : 1;    // Port x input data for pin 13
        uint32 ID14 : 1;    // Port x input data for pin 14
        uint32 ID15 : 1;    // Port x input data for pin 15
        uint32      : 16;
    } bit;
    uint32 val;
};

union GPIO_ODR {
    struct {
        uint32 OD0  : 1;    // Port x output data for pin 0
        uint32 OD1  : 1;    // Port x output data for pin 1
        uint32 OD2  : 1;    // Port x output data for pin 2
        uint32 OD3  : 1;    // Port x output data for pin 3
        uint32 OD4  : 1;    // Port x output data for pin 4
        uint32 OD5  : 1;    // Port x output data for pin 5
        uint32 OD6  : 1;    // Port x output data for pin 6
        uint32 OD7  : 1;    // Port x output data for pin 7
        uint32 OD8  : 1;    // Port x output data for pin 8
        uint32 OD9  : 1;    // Port x output data for pin 9
        uint32 OD10 : 1;    // Port x output data for pin 10
        uint32 OD11 : 1;    // Port x output data for pin 11
        uint32 OD12 : 1;    // Port x output data for pin 12
        uint32 OD13 : 1;    // Port x output data for pin 13
        uint32 OD14 : 1;    // Port x output data for pin 14
        uint32 OD15 : 1;    // Port x output data for pin 15
        uint32      : 16;
    } bit;
    uint32 val;
};

union GPIO_BSRR {
    struct {
        uint32 BS0  : 1;    // Set port x output bit 0
        uint32 BS1  : 1;    // Set port x output bit 1
        uint32 BS2  : 1;    // Set port x output bit 2
        uint32 BS3  : 1;    // Set port x output bit 3
        uint32 BS4  : 1;    // Set port x output bit 4
        uint32 BS5  : 1;    // Set port x output bit 5
        uint32 BS6  : 1;    // Set port x output bit 6
        uint32 BS7  : 1;    // Set port x output bit 7
        uint32 BS8  : 1;    // Set port x output bit 8
        uint32 BS9  : 1;    // Set port x output bit 9
        uint32 BS10 : 1;    // Set port x output bit 10
        uint32 BS11 : 1;    // Set port x output bit 11
        uint32 BS12 : 1;    // Set port x output bit 12
        uint32 BS13 : 1;    // Set port x output bit 13
        uint32 BS14 : 1;    // Set port x output bit 14
        uint32 BS15 : 1;    // Set port x output bit 15
        uint32 BR0  : 1;    // Reset port x output bit 0
        uint32 BR1  : 1;    // Reset port x output bit 1
        uint32 BR2  : 1;    // Reset port x output bit 2
        uint32 BR3  : 1;    // Reset port x output bit 3
        uint32 BR4  : 1;    // Reset port x output bit 4
        uint32 BR5  : 1;    // Reset port x output bit 5
        uint32 BR6  : 1;    // Reset port x output bit 6
        uint32 BR7  : 1;    // Reset port x output bit 7
        uint32 BR8  : 1;    // Reset port x output bit 8
        uint32 BR9  : 1;    // Reset port x output bit 9
        uint32 BR10 : 1;    // Reset port x output bit 10
        uint32 BR11 : 1;    // Reset port x output bit 11
        uint32 BR12 : 1;    // Reset port x output bit 12
        uint32 BR13 : 1;    // Reset port x output bit 13
        uint32 BR14 : 1;    // Reset port x output bit 14
        uint32 BR15 : 1;    // Reset port x output bit 15
    } bit;
    uint32 val;
};

union GPIO_LCKR {
    struct {
        uint32 LCK0     : 1;    // Port x lock bit for pin 0
        uint32 LCK1     : 1;    // Port x lock bit for pin 1
        uint32 LCK2     : 1;    // Port x lock bit for pin 2
        uint32 LCK3     : 1;    // Port x lock bit for pin 3
        uint32 LCK4     : 1;    // Port x lock bit for pin 4
        uint32 LCK5     : 1;    // Port x lock bit for pin 5
        uint32 LCK6     : 1;    // Port x lock bit for pin 6
        uint32 LCK7     : 1;    // Port x lock bit for pin 7
        uint32 LCK8     : 1;    // Port x lock bit for pin 8
        uint32 LCK9     : 1;    // Port x lock bit for pin 9
        uint32 LCK10    : 1;    // Port x lock bit for pin 10
        uint32 LCK11    : 1;    // Port x lock bit for pin 11
        uint32 LCK12    : 1;    // Port x lock bit for pin 12
        uint32 LCK13    : 1;    // Port x lock bit for pin 13
        uint32 LCK14    : 1;    // Port x lock bit for pin 14
        uint32 LCK15    : 1;    // Port x lock bit for pin 15
        uint32 LCKK     : 1;    // Lock key
        uint32          : 15;
    } bit;
    uint32 val;
};

union GPIO_AFR {
    struct {
        uint32 AFSEL0   : 4;    // Alternate function selection for pin 0
        uint32 AFSEL1   : 4;    // Alternate function selection for pin 1
        uint32 AFSEL2   : 4;    // Alternate function selection for pin 2
        uint32 AFSEL3   : 4;    // Alternate function selection for pin 3
        uint32 AFSEL4   : 4;    // Alternate function selection for pin 4
        uint32 AFSEL5   : 4;    // Alternate function selection for pin 5
        uint32 AFSEL6   : 4;    // Alternate function selection for pin 6
        uint32 AFSEL7   : 4;    // Alternate function selection for pin 7
    } bit;
    uint32 val;
};

union GPIO_BRR {
    struct {
        uint32 BR0  : 1;    // Reset port x output bit 0
        uint32 BR1  : 1;    // Reset port x output bit 1
        uint32 BR2  : 1;    // Reset port x output bit 2
        uint32 BR3  : 1;    // Reset port x output bit 3
        uint32 BR4  : 1;    // Reset port x output bit 4
        uint32 BR5  : 1;    // Reset port x output bit 5
        uint32 BR6  : 1;    // Reset port x output bit 6
        uint32 BR7  : 1;    // Reset port x output bit 7
        uint32 BR8  : 1;    // Reset port x output bit 8
        uint32 BR9  : 1;    // Reset port x output bit 9
        uint32 BR10 : 1;    // Reset port x output bit 10
        uint32 BR11 : 1;    // Reset port x output bit 11
        uint32 BR12 : 1;    // Reset port x output bit 12
        uint32 BR13 : 1;    // Reset port x output bit 13
        uint32 BR14 : 1;    // Reset port x output bit 14
        uint32 BR15 : 1;    // Reset port x output bit 15
        uint32      : 16;
    } bit;
    uint32 val;
};

struct REG_GPIO
{
    GPIO_MODER      MODER;      // GPIO port mode register                Address offset: 0x00
    GPIO_OTYPER     OTYPER;     // GPIO port output type register         Address offset: 0x04
    GPIO_OSPEEDR    OSPEEDR;    // GPIO port output speed register        Address offset: 0x08
    GPIO_PUPDR      PUPDR;      // GPIO port pull-up/pull-down register   Address offset: 0x0C
    GPIO_IDR        IDR;        // GPIO port input data register          Address offset: 0x10
    GPIO_ODR        ODR;        // GPIO port output data register         Address offset: 0x14
    GPIO_BSRR       BSRR;       // GPIO port bit set/reset  register      Address offset: 0x18
    GPIO_LCKR       LCKR;       // GPIO port configuration lock register  Address offset: 0x1C
    GPIO_AFR        AFR[2];     // GPIO alternate function registers      Address offset: 0x20-0x24
    GPIO_BRR        BRR;        // GPIO Bit Reset register                Address offset: 0x28
};

#define REG_GPIOA ((volatile REG_GPIO*)0x48000000u)
#define REG_GPIOB ((volatile REG_GPIO*)0x48000400u)
#define REG_GPIOC ((volatile REG_GPIO*)0x48000800u)
#define REG_GPIOH ((volatile REG_GPIO*)0x48001C00u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
