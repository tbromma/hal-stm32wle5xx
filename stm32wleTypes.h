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

// CREATED BY:       T. Bromma (tobiasz.bromma@thoriumspace.com)
// CREATION DATE:    March 2023
// DESCRIPTION:      Low level layer for general purpose digital input's and output's

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************************************************************************
                                                                   TYPES
******************************************************************************************************************************************/
namespace hal {

enum irqPrio_t : int8_t {
    IRQ_PRIO_none   = -1,
    IRQ_PRIO_0      = 0,
    IRQ_PRIO_1      = 1,
    IRQ_PRIO_2      = 2,
    IRQ_PRIO_3      = 3,
    IRQ_PRIO_4      = 4,
    IRQ_PRIO_5      = 5,
    IRQ_PRIO_6      = 6,
    IRQ_PRIO_7      = 7,
};

enum irqId_t : int8_t {
    IRQ_none            = -1,
    IRQ_WWDG            = 0,    // Window WatchDog Interrupt
    IRQ_PVD_PVM         = 1,    // PVD and PVM detector
    IRQ_RTC             = 2,    // RTC Tamper, RTC TimeStamp, LSECSS and RTC SSRU Interrupts
    IRQ_RTC_WKUP        = 3,    // RTC Wakeup Interrupt
    IRQ_FLASH           = 4,    // FLASH (CFI)  global Interrupt
    IRQ_RCC             = 5,    // RCC Interrupt
    IRQ_EXTI0           = 6,    // EXTI Line 0 Interrupt
    IRQ_EXTI1           = 7,    // EXTI Line 1 Interrupt
    IRQ_EXTI2           = 8,    // EXTI Line 2 Interrupt
    IRQ_EXTI3           = 9,    // EXTI Line 3 Interrupt
    IRQ_EXTI4           = 10,   // EXTI Line 4 Interrupt
    IRQ_DMA1_CH1        = 11,   // DMA1 Channel 1 Interrupt
    IRQ_DMA1_CH2        = 12,   // DMA1 Channel 2 Interrupt
    IRQ_DMA1_CH3        = 13,   // DMA1 Channel 3 Interrupt
    IRQ_DMA1_CH4        = 14,   // DMA1 Channel 4 Interrupt
    IRQ_DMA1_CH5        = 15,   // DMA1 Channel 5 Interrupt
    IRQ_DMA1_CH6        = 16,   // DMA1 Channel 6 Interrupt
    IRQ_DMA1_CH7        = 17,   // DMA1 Channel 7 Interrupt
    IRQ_ADC             = 18,   // ADC Interrupt
    IRQ_DAC             = 19,   // DAC Interrupt
    IRQ_COMP            = 21,   // COMP1 and COMP2 Interrupts
    IRQ_EXTI9_5         = 22,   // EXTI Lines [9:5] Interrupt
    IRQ_TIM1_BRK        = 23,   // TIM1 Break Interrupt
    IRQ_TIM1_UP         = 24,   // TIM1 Update Interrupt
    IRQ_TIM1_TRG_COM    = 25,   // TIM1 Trigger and Communication Interrupts
    IRQ_TIM1_CC         = 26,   // TIM1 Capture Compare Interrupt
    IRQ_TIM2            = 27,   // TIM2 Global Interrupt
    IRQ_TIM16           = 28,   // TIM16 Global Interrupt
    IRQ_TIM17           = 29,   // TIM17 Global Interrupt
    IRQ_I2C1_EV         = 30,   // I2C1 Event Interrupt
    IRQ_I2C1_ER         = 31,   // I2C1 Error Interrupt
    IRQ_I2C2_EV         = 32,   // I2C2 Event Interrupt
    IRQ_I2C2_ER         = 33,   // I2C2 Error Interrupt
    IRQ_SPI1            = 34,   // SPI1 Interrupt
    IRQ_SPI2            = 35,   // SPI2 Interrupt
    IRQ_USART1          = 36,   // USART1 Interrupt
    IRQ_USART2          = 37,   // USART2 Interrupt
    IRQ_LPUART1         = 38,   // LPUART1 Interrupt
    IRQ_LPTIM1          = 39,   // LPTIM1 Global Interrupt
    IRQ_LPTIM2          = 40,   // LPTIM2 Global Interrupt
    IRQ_EXTI15_10       = 41,   // EXTI Lines [15:10] Interrupt
    IRQ_RTC_Alarm       = 42,   // RTC Alarms (A and B) Interrupt
    IRQ_LPTIM3          = 43,   // LPTIM3 Global Interrupt
    IRQ_SPI3_RF         = 44,   // SUBGHZSPI Interrupt
    IRQ_HSEM            = 47,   // HSEM Interrupt
    IRQ_I2C3_EV         = 48,   // I2C3 Event Interrupt
    IRQ_I2C3_ER         = 49,   // I2C3 Error Interrupt
    IRQ_SUBGHZ_Radio    = 50,   // SUBGHZ Radio Interrupt
    IRQ_AES             = 51,   // AES Interrupt
    IRQ_RNG             = 52,   // RNG Interrupt
    IRQ_PKA             = 53,   // PKA Interrupt
    IRQ_DMA2_CH1        = 54,   // DMA2 Channel 1 Interrupt
    IRQ_DMA2_CH2        = 55,   // DMA2 Channel 2 Interrupt
    IRQ_DMA2_CH3        = 56,   // DMA2 Channel 3 Interrupt
    IRQ_DMA2_CH4        = 57,   // DMA2 Channel 4 Interrupt
    IRQ_DMA2_CH5        = 58,   // DMA2 Channel 5 Interrupt
    IRQ_DMA2_CH6        = 59,   // DMA2 Channel 6 Interrupt
    IRQ_DMA2_CH7        = 60,   // DMA2 Channel 7 Interrupt
    IRQ_DMAMUX1_OVR     = 61,   // DMAMUX1 overrun Interrupt
};

//---------------------------------------------------------------------------------------
// Peripheral ID

enum perId_t : int8_t {
    PER_none = -1,
    PER_DMA1 = 0,
    PER_DMA2,
    PER_DMA_MUX,
    PER_CRC,
    PER_GPIOA,
    PER_GPIOB,
    PER_GPIOC,
    PER_GPIOH,
    PER_PKA,
    PER_AES,
    PER_RNG,
    PER_FLASH,
    PER_TIM2,
    PER_RTCAPB,
    PER_WWDG,
    PER_SPI2,
    PER_USART2,
    PER_I2C1,
    PER_I2C2,
    PER_I2C3,
    PER_DAC,
    PER_LPTIM1,
    PER_LPUART1,
    PER_LPTIM2,
    PER_LPTIM3,
    PER_ADC,
    PER_TIM1,
    PER_SPI1,
    PER_USART1,
    PER_TIM16,
    PER_TIM17,
    PER_SPI3_RF,
    PER_count,
};

//---------------------------------------------------------------------------------------
// GPIO peripheral

// Pin port
enum pinPort_t : int8_t {
    PORT_none = -1,
    PORT_A = 0,
    PORT_B,
    PORT_C,
    PORT_H,
    PORT_count,
};

// Pin number
enum pinNum_t : int8_t {
    PIN_none = -1,
    PIN_0 = 0,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
    PIN_9,
    PIN_10,
    PIN_11,
    PIN_12,
    PIN_13,
    PIN_14,
    PIN_15,
    PIN_count,
};

// Pin peripheral function
enum pinFunc_t : int8_t {
    FUNC_none = -1,
    FUNC_AF0 = 0,
    FUNC_AF1,
    FUNC_AF2,
    FUNC_AF3,
    FUNC_AF4,
    FUNC_AF5,
    FUNC_AF6,
    FUNC_AF7,
    FUNC_AF8,
    FUNC_AF9,
    FUNC_AF10,
    FUNC_AF11,
    FUNC_AF12,
    FUNC_AF13,
    FUNC_AF14,
    FUNC_AF15,
    FUNC_count,
};

//---------------------------------------------------------------------------------------
// USART peripheral

enum usartNum_t : int8_t {
    USART_none = -1,
    USART_1 = 0,
    LPUART_1,
    USART_2,
    USART_count,
};

//---------------------------------------------------------------------------------------
// I2C peripheral

enum i2cNum_t : int8_t {
    I2C_none = -1,
    I2C_1 = 0,
    I2C_2,
    I2C_3,
    I2C_count,
};

//---------------------------------------------------------------------------------------
// SPI peripheral

enum spiNum_t : int8_t {
    SPI_none = -1,
    SPI1 = 0,
    SPI2,
    SPI3_RF,
    SPI_count,
};

//---------------------------------------------------------------------------------------
// TIMER peripheral

enum timNum_t : int8_t {
    TIM_none = -1,
    TIM_1 = 0,
    TIM_2,
    TIM_16,
    TIM_17,
    LPTIM_1,
    LPTIM_2,
    TIM_count,
};

//---------------------------------------------------------------------------------------
// DMA peripheral

enum dmaNum_t : int8_t {
    DMA_none = -1,
    DMA_1 = 0,
    DMA_2,
    DMA_count,
};

// DMA channels
enum dmaCh_t : int8_t {
    DMA_CH_none = -1,
    DMA_CH1 = 0,
    DMA_CH2,
    DMA_CH3,
    DMA_CH4,
    DMA_CH5,
    DMA_CH6,
    DMA_CH7,
    DMA_CH_count,
};

enum dmaReq_t : uint8_t {

    DMA_REQ_NONE            = 0,

    DMA_REQ_DMAMUX_REQ_GEN0 = 1,
    DMA_REQ_DMAMUX_REQ_GEN1 = 2,
    DMA_REQ_DMAMUX_REQ_GEN2 = 3,
    DMA_REQ_DMAMUX_REQ_GEN3 = 4,

    DMA_REQ_ADC             = 5,
    DMA_REQ_DAC_OUT1        = 6,

    DMA_REQ_SPI1_RX         = 7,
    DMA_REQ_SPI1_TX         = 8,
    DMA_REQ_SPI2_RX         = 9,
    DMA_REQ_SPI2_TX         = 10,

    DMA_REQ_I2C1_RX         = 11,
    DMA_REQ_I2C1_TX         = 12,
    DMA_REQ_I2C2_RX         = 13,
    DMA_REQ_I2C2_TX         = 14,
    DMA_REQ_I2C3_RX         = 15,
    DMA_REQ_I2C3_TX         = 16,

    DMA_REQ_USART1_RX       = 17,
    DMA_REQ_USART1_TX       = 18,
    DMA_REQ_USART2_RX       = 19,
    DMA_REQ_USART2_TX       = 20,

    DMA_REQ_LPUART1_RX      = 21,
    DMA_REQ_LPUART1_TX      = 22,

    DMA_REQ_TIM1_CH1        = 23,
    DMA_REQ_TIM1_CH2        = 24,
    DMA_REQ_TIM1_CH3        = 25,
    DMA_REQ_TIM1_CH4        = 26,
    DMA_REQ_TIM1_UP         = 27,
    DMA_REQ_TIM1_TRIG       = 28,
    DMA_REQ_TIM1_COM        = 29,

    DMA_REQ_TIM2_CH1        = 30,
    DMA_REQ_TIM2_CH2        = 31,
    DMA_REQ_TIM2_CH3        = 32,
    DMA_REQ_TIM2_CH4        = 33,
    DMA_REQ_TIM2_UP         = 34,

    DMA_REQ_TIM16_CH1       = 35,
    DMA_REQ_TIM16_UP        = 36,

    DMA_REQ_TIM17_CH1       = 37,
    DMA_REQ_TIM17_UP        = 38,

    DMA_REQ_AES_IN          = 39,
    DMA_REQ_AES_OUT         = 40,

    DMA_REQ_SUBGHZSPI_RX    = 41,
    DMA_REQ_SUBGHZSPI_TX    = 42,
};

} // END OF NAMESPACE 'hal'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/