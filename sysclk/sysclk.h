/******************************************************************************************************************************************
                                                               START OF FILE
*******************************************************************************************************************************************
                                                @
                                                @
                                                @@@ @@@@@@@@@@
                @@@@@@@@@@@@@  @@@        @@@   @ @@          @    @@@@@@@@@@@    @@@   @@@        @@@   @@@@@       @@@@@
                     @@@       @@@        @@@   @@@            @@  @@@      @@@   @@@   @@@        @@@   @@@@@@     @@@@@@
                     @@@       @@@        @@@  @  @             @  @@@      @@@   @@@   @@@        @@@   @@@ @@     @@ @@@
                     @@@       @@@@@@@@@@@@@@  @  @             @  @@@@@@@@@@@    @@@   @@@        @@@   @@@ @@@   @@@ @@@
                     @@@       @@@        @@@  @                @  @@@@@@@@@      @@@   @@@        @@@   @@@  @@@ @@@  @@@
                     @@@       @@@        @@@  @@              @@  @@@    @@@     @@@   @@@        @@@   @@@   @@@@@   @@@
                     @@@       @@@        @@@    @            @    @@@     @@@    @@@    @@@@@@@@@@@@    @@@    @@@    @@@
                     @@@       @@@        @@@     @@@@@@@@@@@@     @@@      @@@   @@@      @@@@@@@@      @@@     @     @@@

                     S      P      A      C      E        T      E      C      H      N      O      L      O      G      Y

*******************************************************************************************************************************************
                                                                 FILE INFO
******************************************************************************************************************************************/

// CREATED BY:       Tobiasz Bromma (tobiasz.bromma@thoriumspace.com)
// CREATION DATE:    February 2023
// DESCRIPTION:      Low level layer for general purpose digital input's and output's

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include "common.h"
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::clk {

enum sysClk_t {
    SYSCLK_LSI,
    SYSCLK_LSE,
    SYSCLK_HSI16,
    SYSCLK_HSE32,
    SYSCLK_MSI,
    SYSCLK_PLLP,
    SYSCLK_PLLQ,
    SYSCLK_PLLR,
    SYSCLK_HCLK1,
    SYSCLK_HCLK3,
    SYSCLK_PCLK1,
    SYSCLK_PCLK2,
    SYSCLK_PCLK3,
    SYSCLK_SYSTICK,
};

enum slckSrc_t : bool {     // Slow clock source
    SLCK_SRC_LSI,           // Internal RC oscillator
    SLCK_SRC_LSE,           // External crystal oscillator
};

enum mainckSrc_t {          // Mainck clock source
    MAINCK_SRC_HSI16,       // Internal RC 16MHz RC oscillator
    MAINCK_SRC_MIS,         // Internal Multi Speed (100kHz - 48Mh=Hz) RC oscillator
    MAINCK_SRC_HSE32,       // External 32MHz crystal oscillator
};

enum sysClkSrc_t {
    SYSCLK_SRC_MSI,
    SYSCLK_SRC_HSI16,
    SYSCLK_SRC_HSE32,
    SYSCLK_SRC_PLLRCLK,
};

// Hse32 clock prescaler for system clock and PLL
enum hse32Presc_t {
    HSE32_PRESC1,
    HSE32_PRESC2,
};

// PLL clock output
enum pllClk_t {
    PLL_CLK_P,
    PLL_CLK_Q,
    PLL_CLK_R,
};

// PLL clock source
enum pllClkSrc_t {
    PLL_CLKSRC_MSI,
    PLL_CLKSRC_HSI16,
    PLL_CLKSRC_HSE32PRES1,  // HSE32 divided by 1
    PLL_CLKSRC_HSE32PRES2,  // HSE32 divided by 2
};

// USART clock source
enum usartClkSrc_t {
    USART_CLKSRC_PCLK = 0b00,
    USART_CLKSRC_SYSCLK = 0b01,
    USART_CLKSRC_HSI16 = 0b10,
    USART_CLKSRC_LSE = 0b11,
};

// RTC clock source
enum rtcClkSrc_t {
    RTC_CLKSRC_NONE = 0b00,
    RTC_CLKSRC_LSE = 0b01,
    RTC_CLKSRC_LSI = 0b10,
    RTC_CLKSRC_HSE32DIV32 = 0b11,
};

// I2C clock source
enum i2cClkSrc_t {
    I2C_CLKSRC_NONE = 0b00,
    I2C_CLKSRC_PCLK = 0b00,
    I2C_CLKSRC_SYSCLK = 0b01,
    I2C_CLKSRC_HSI16 = 0b10,
};

// SPI2 clock source
enum spi2ClkSrc_t {
    SPI2_CLKSRC_NONE = 0b00,
    SPI2_CLKSRC_PLLQ = 0b01,
    SPI2_CLKSRC_HSI16 = 0b10,
    SPI2_CLKSRC_SYSCLK = 0b11,
};

// ADC clock source
enum adcClkSrc_t {
    ADC_CLKSRC_NONE = 0b00,
    ADC_CLKSRC_HSI16 = 0b01,
    ADC_CLKSRC_PLLP = 0b10,
    ADC_CLKSRC_SYSCLK = 0b11,
};

// RNG clock source
enum rngClkSrc_t {
    RNG_CLKSRC_PLLQ = 0b00,
    RNG_CLKSRC_LSI = 0b01,
    RNG_CLKSRC_LSE = 0b10,
    RNG_CLKSRC_MSI = 0b11,
};

// PLL configuration
struct pllCfg_t {
    pllClkSrc_t clkSrc = PLL_CLKSRC_HSI16;
    uint8       divM = 1;                   // PLL input division factor. Allowed range 1-8
    uint8       mulN = 6;                   // PLL input multiplication factor. Allowed range 6-127
    uint8       divP = 0;                   // PLL output P division factor. Allowed range 2-32. Set 0 to disable P output
    uint8       divQ = 0;                   // PLL output Q division factor. Allowed range 2-8. Set 0 to disable Q output
    uint8       divR = 0;                   // PLL output R division factor. Allowed range 2-8. Set 0 to disable R output
};

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/

Exec  SysClkSetSrc(sysClkSrc_t clkSrc);
uint32  SysClkGetFreqHz(sysClk_t sysClk);

Exec  LsiClkEnable();
Exec  LsiClkDisable();
State   LsiClkGetState();

Exec  LseClkEnable();
Exec  LseClkDisable();
Exec  LseClkSysEnable();
State   LseClkGetState();

Exec  Hse32ClkEnable();
Exec  Hse32ClkDisable();
void    Hse32ClkSetPresc(hse32Presc_t presc);
State   Hse32ClkGetState();

Exec  MsiClkEnable();
Exec  MsiClkDisable();
State   MsiClkGetState();

Exec  Hsi16ClkEnable();
Exec  Hsi16ClkDisable();
State   Hsi16ClkGetState();

Exec  PllClkEnable(pllCfg_t& config);
State   PllClkGetState(pllClk_t clk);

void    RtcClkSrcSelect(rtcClkSrc_t clkSrc);

void    UsartClkSrcSelect(hal::usartNum_t usartNum, usartClkSrc_t clkSrc);
uint32  UsartClkGetFreqHz(hal::usartNum_t usartNum);

void    I2cClkSrcSelect(hal::i2cNum_t i2cNum, i2cClkSrc_t clkSrc);
uint32  I2cClkGetFreqHz(hal::i2cNum_t i2cNum);

void    AdcClkSrcSelect(adcClkSrc_t clkSrc);
uint32  AdcClkGetFreqHz();

Exec  RngClkSrcSelect(rngClkSrc_t clkSrc);
uint32  RngClkGetFreqHz();

void    PerClkEnable(hal::perId_t perId);
void    PerClkDisable(hal::perId_t perId);
State   PerClkState(hal::perId_t perId);
uint32  PerClkGetFreqHz(hal::perId_t perId);

} // END OF NAMESPACE 'hal::clk'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
