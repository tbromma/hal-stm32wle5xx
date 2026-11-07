/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: RCC
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                              TYPES & VARIABLES
******************************************************************************************************************************************/

union RCC_CR {
    struct {
        uint32 MSION        : 1;    // MSI clock enable
        uint32 MSIRDY       : 1;    // MSI clock ready flag
        uint32 MSIPLLEN     : 1;    // MSI PLL mode enable
        uint32 MSIRGSEL     : 1;    // MSI clock range selection
        uint32 MSIRANGE     : 4;    // MSI clock range
        uint32 HSION        : 1;    // HSI16 clock enable
        uint32 HSIKERON     : 1;    // HSI16 kernel clock enable
        uint32 HSIRDY       : 1;    // HSI16 clock ready flag
        uint32 HSIASFS      : 1;    // HSI16 automatic start from Stop
        uint32 HSIKERDY     : 1;    // HSI16 kernel clock ready flag
        uint32              : 3;
        uint32 HSEON        : 1;    // HSE clock enable
        uint32 HSERDY       : 1;    // HSE clock ready flag
        uint32              : 1;
        uint32 CSSON        : 1;    // Clock security system enable
        uint32 HSEPRE       : 1;    // HSE prescaler
        uint32 HSEBYPPWR    : 1;    // HSE external clock source power enable
        uint32              : 2;
        uint32 PLLON        : 1;    // Main PLL enable
        uint32 PLLRDY       : 1;    // Main PLL ready flag
        uint32              : 6;
    } bit;
    uint32 val;
};

union RCC_CFGR {
    struct {
        uint32 SW       : 2;    // System clock switch
        uint32 SWS      : 2;    // System clock switch status
        uint32 HPRE     : 4;    // AHB prescaler
        uint32 PRE1     : 3;    // APB1 prescaler
        uint32 PRE2     : 3;    // APB2 prescaler
        uint32 STOPWUCK : 1;    // Wake-up from Stop clock selection
        uint32 HPREF    : 1;    // AHB prescaler flag
        uint32 PPRE1F   : 1;    // APB1 prescaler flag
        uint32 PPRE2F   : 1;    // APB2 prescaler flag
        uint32          : 5;
        uint32 MCOSEL   : 4;    // Microcontroller clock output selection
        uint32 MCOPRE   : 1;    // Microcontroller clock output prescaler
        uint32          : 1;
    } bit;
    uint32 val;
};

union RCC_PLLCFGR {
    struct {
        uint32 PLLSRC   : 2;    // PLL clock source
        uint32          : 2;
        uint32 PLLM     : 3;    // PLL input clock divider
        uint32          : 1;
        uint32 PLLN     : 7;    // PLL multiplication factor
        uint32          : 1;
        uint32 PLLPEN   : 1;    // PLL P output enable
        uint32 PLLP     : 5;    // PLL P output divider
        uint32          : 2;
        uint32 PLLQEN   : 1;    // PLL Q output enable
        uint32 PLLQ     : 3;    // PLL Q output divider
        uint32 PLLREN   : 1;    // PLL R output enable
        uint32 PLLR     : 3;    // PLL R output divider
    } bit;
    uint32 val;
};

union RCC_AHB1ENR {
    struct {
        uint32 DMA1EN       : 1;    // DMA1 clock enable
        uint32 DMA2EN       : 1;    // DMA2 clock enable
        uint32 DMAMUX1EN    : 1;    // DMAMUX1 clock enable
        uint32              : 9;
        uint32 CRCEN        : 1;    // CRC clock enable
        uint32              : 19;
    } bit;
    uint32 val;
};

static uint8 RCC_AHB1ENR_bitpos[] = {0, 1, 2, 12};

union RCC_AHB2ENR {
    struct {
        uint32 GPIOAEN  : 1;    // GPIOA clock enable
        uint32 GPIOBEN  : 1;    // GPIOB clock enable
        uint32 GPIOCEN  : 1;    // GPIOC clock enable
        uint32          : 4;
        uint32 GPIOHEN  : 1;    // GPIOH clock enable
        uint32          : 24;
    } bit;
    uint32 val;
};

static uint8 RCC_AHB2ENR_bitpos[] = {0, 1, 2, 7};

union RCC_AHB3ENR {
    struct {
        uint32          : 8;
        uint32 PKAEN    : 1;    // PKA clock enable
        uint32 AESEN    : 1;    // AES clock enable
        uint32 RNGEN    : 1;    // RNG clock enable
        uint32 HSEMEN   : 1;    // HSEM clock enable
        uint32          : 5;
        uint32 FLASHEN  : 1;    // Flash interface clock enable
        uint32          : 6;
    } bit;
    uint32 val;
};

static uint8 RCC_AHB3ENR_bitpos[] = {16, 17, 18, 19, 25};

union RCC_APB1ENR1 {
    struct {
        uint32 TIM2EN   : 1;    // TIM2 clock enable
        uint32          : 9;
        uint32 RTCAPBEN : 1;    // RTC APB clock enable
        uint32 WWDGEN   : 1;    // WWDG clock enable
        uint32          : 2;
        uint32 SPI2EN   : 1;    // SPI2 clock enable
        uint32          : 2;
        uint32 USART2EN : 1;    // USART2 clock enable
        uint32          : 3;
        uint32 I2C1EN   : 1;    // I2C1 clock enable
        uint32 I2C2EN   : 1;    // I2C2 clock enable
        uint32 I2C3EN   : 1;    // I2C3 clock enable
        uint32          : 5;
        uint32 DACEN    : 1;    // DAC clock enable
        uint32          : 1;
        uint32 LPTIM1EN : 1;    // LPTIM1 clock enable
    } bit;
    uint32 val;
};

static uint8 RCC_APB1ENR1_bitpos[] = {0, 10, 11, 14, 17, 21, 22, 23, 29, 31};

union RCC_APB1ENR2 {
    struct {
        uint32 LPUART1EN    : 1;    // LPUART1 clock enable
        uint32              : 4;
        uint32 LPTIM2EN     : 1;    // LPTIM2 clock enable
        uint32 LPTIM3EN     : 1;    // LPTIM3 clock enable
        uint32              : 25;

    } bit;
    uint32 val;
};

static uint8 RCC_APB1ENR2_bitpos[] = {0, 5, 6};

union RCC_APB2ENR {
    struct {
        uint32          : 9;
        uint32 ADCEN    : 1;    // ADC clock enable
        uint32          : 1;
        uint32 TIM1EN   : 1;    // TIM1 clock enable
        uint32 SPI1EN   : 1;    // SPI1 clock enable
        uint32          : 1;
        uint32 USART1EN : 1;    // USART1 clock enable
        uint32          : 2;
        uint32 TIM16EN  : 1;    // TIM16 clock enable
        uint32 TIM17EN  : 1;    // TIM17 clock enable
        uint32          : 13;
    } bit;
    uint32 val;
};

static uint8 RCC_APB2ENR_bitpos[] = {9, 11, 12, 14, 17, 18};

union RCC_APB3ENR {
    struct {
        uint32 SUBGHZSPIEN  : 1;    // Sub-GHz SPI clock enable
        uint32              : 31;
    } bit;
    uint32 val;
};

static uint8 RCC_APB3ENR_bitpos[] = {0};

union RCC_CCIPR {
    struct {
        uint32 USART1SEL    : 2;    // USART1 clock source selection
        uint32 USART2SEL    : 2;    // USART2 clock source selection
        uint32              : 4;
        uint32 SPI2SEL      : 2;    // SPI2 clock source selection
        uint32 LPUARTSEL    : 2;    // LPUART1 clock source selection
        uint32 I2C1SEL      : 2;    // I2C1 clock source selection
        uint32 I2C2SEL      : 2;    // I2C2 clock source selection
        uint32 I2C3SEL      : 2;    // I2C3 clock source selection
        uint32 LPTIM1SEL    : 2;    // LPTIM1 clock source selection
        uint32 LPTIM2SEL    : 2;    // LPTIM2 clock source selection
        uint32 LPTIM3SEL    : 2;    // LPTIM3 clock source selection
        uint32              : 4;
        uint32 ADCSEL       : 2;    // ADC clock source selection
        uint32 RNGSEL       : 2;    // RNG clock source selection
    } bit;
    uint32 val;
};

union RCC_BDCR {
    struct {
        uint32 LSEON        : 1;    // LSE oscillator enable
        uint32 LSERDY       : 1;    // LSE oscillator ready flag
        uint32 LSEBYP       : 1;    // LSE oscillator bypass
        uint32 LSEDRV       : 2;    // LSE oscillator drive capability
        uint32 LSECSSON     : 1;    // LSE clock security system enable
        uint32 LSECSSD      : 1;    // LSE clock security system failure flag
        uint32 LSESYSEN     : 1;    // LSE system clock enable
        uint32 RTCSEL       : 2;    // RTC clock source selection
        uint32              : 1;
        uint32 LSESYSRDY    : 1;    // LSE system clock ready flag
        uint32              : 3;
        uint32 RTCEN        : 1;    // RTC clock enable
        uint32 BDRST        : 1;    // Backup domain software reset
        uint32              : 7;
        uint32 LSCOEN       : 1;    // Low-speed clock output enable
        uint32 LSCOSEL      : 1;    // Low-speed clock output selection
        uint32              : 6;
    } bit;
    uint32 val;
};

union RCC_CSR {
    struct {
        uint32 LSION        : 1;    // LSI oscillator enable
        uint32 LSIRDY       : 1;    // LSI oscillator ready flag
        uint32              : 2;
        uint32 LSIPRE       : 1;    // LSI prescaler
        uint32              : 3;
        uint32 MSISRANGE    : 4;    // MSI range after Standby
        uint32              : 2;
        uint32 RFRSTF       : 1;    // RF reset flag
        uint32 RFRST        : 1;    // RF reset
        uint32              : 7;
        uint32 RMVF         : 1;    // Remove reset flags
        uint32 RFILARSTF    : 1;    // RF illegal access reset flag
        uint32 OBLRSTF      : 1;    // Option byte loader reset flag
        uint32 PINRSTF      : 1;    // Pin reset flag
        uint32 BORRSTF      : 1;    // Brown-out reset flag
        uint32 SFTRSTF      : 1;    // Software reset flag
        uint32 IWDGRSTF     : 1;    // Independent watchdog reset flag
        uint32 WWDGRSTF     : 1;    // Window watchdog reset flag
        uint32 LPWRRSTF     : 1;    // Low-power reset flag
    } bit;
    uint32 val;
};

union RCC_EXTCFGR {
    struct {
        uint32 SHDHPRE  : 4;    // Shared AHB prescaler
        uint32          : 12;
        uint32 SHDHPREF : 1;    // Shared AHB prescaler flag
        uint32          : 15;
    } bit;
    uint32 val;
};

struct REG_RCC
{
    RCC_CR          CR;             // RCC clock  Control Register                                                     Address offset: 0x00
    uint32          ICSCR;          // RCC Internal Clock Sources Calibration Register                                 Address offset: 0x04
    RCC_CFGR        CFGR;           // RCC Clocks Configuration Register                                               Address offset: 0x08
    RCC_PLLCFGR     PLLCFGR;        // RCC System PLL configuration Register                                           Address offset: 0x0C
    uint32          RESERVED0;      // Reserved                                                                        Address offset: 0x10
    uint32          RESERVED1;      // Reserved                                                                        Address offset: 0x14
    uint32          CIER;           // RCC Clock Interrupt Enable Register                                             Address offset: 0x18
    uint32          CIFR;           // RCC Clock Interrupt Flag Register                                               Address offset: 0x1C
    uint32          CICR;           // RCC Clock Interrupt Clear Register                                              Address offset: 0x20
    uint32          RESERVED2;      // Reserved                                                                        Address offset: 0x24
    uint32          AHB1RSTR;       // RCC AHB1 peripheral reset register                                              Address offset: 0x28
    uint32          AHB2RSTR;       // RCC AHB2 peripheral reset register                                              Address offset: 0x2C
    uint32          AHB3RSTR;       // RCC AHB3 peripheral reset register                                              Address offset: 0x30
    uint32          RESERVED3;      // Reserved                                                                        Address offset: 0x34
    uint32          APB1RSTR1;      // RCC APB1 peripheral reset register 1                                            Address offset: 0x38
    uint32          APB1RSTR2;      // RCC APB1 peripheral reset register 2                                            Address offset: 0x3C
    uint32          APB2RSTR;       // RCC APB2 peripheral reset register                                              Address offset: 0x40
    uint32          APB3RSTR;       // RCC APB3 peripheral reset register                                              Address offset: 0x44
    RCC_AHB1ENR     AHB1ENR;        // RCC AHB1 peripheral clocks enable register                                      Address offset: 0x48
    RCC_AHB2ENR     AHB2ENR;        // RCC AHB2 peripheral clocks enable register                                      Address offset: 0x4C
    RCC_AHB3ENR     AHB3ENR;        // RCC AHB3 peripheral clocks enable register                                      Address offset: 0x50
    uint32          RESERVED4;      // Reserved                                                                        Address offset: 0x54
    RCC_APB1ENR1    APB1ENR1;       // RCC APB1 peripheral clocks enable register 1                                    Address offset: 0x58
    RCC_APB1ENR2    APB1ENR2;       // RCC APB1 peripheral clocks enable register 2                                    Address offset: 0x5C
    RCC_APB2ENR     APB2ENR;        // RCC APB2 peripheral clocks enable register                                      Address offset: 0x60
    RCC_APB3ENR     APB3ENR;        // RCC APB3 peripheral clocks enable register                                      Address offset: 0x64
    uint32          AHB1SMENR;      // RCC AHB1 peripheral clocks enable in sleep and stop modes register              Address offset: 0x68
    uint32          AHB2SMENR;      // RCC AHB2 peripheral clocks enable in sleep and stop modes register              Address offset: 0x6C
    uint32          AHB3SMENR;      // RCC AHB3 & AHB4 peripheral clocks enable in sleep and stop modes register       Address offset: 0x70
    uint32          RESERVED5;      // Reserved                                                                        Address offset: 0x74
    uint32          APB1SMENR1;     // RCC APB1 peripheral clocks enable in sleep mode and stop modes register 1       Address offset: 0x78
    uint32          APB1SMENR2;     // RCC APB1 peripheral clocks enable in sleep mode and stop modes register 2       Address offset: 0x7C
    uint32          APB2SMENR;      // RCC APB2 peripheral clocks enable in sleep mode and stop modes register         Address offset: 0x80
    uint32          APB3SMENR;      // RCC APB3 peripheral clocks enable in sleep mode and stop modes register         Address offset: 0x84
    RCC_CCIPR       CCIPR;          // RCC Peripherals Clock Configuration Independent Register                        Address offset: 0x88
    uint32          RESERVED6;      // Reserved                                                                        Address offset: 0x8C
    RCC_BDCR        BDCR;           // RCC Backup Domain Control Register                                              Address offset: 0x90
    RCC_CSR         CSR;            // RCC Control and Status Register                                                 Address offset: 0x94
    uint32          RESERVED7[28];  // Reserved                                                                        Address offset: 0x98-0x104
    RCC_EXTCFGR     EXTCFGR;        // RCC Extended Clock Recovery Register                                            Address offset: 0x108
};

#define REG_RCC ((volatile REG_RCC*)0x58000000u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
