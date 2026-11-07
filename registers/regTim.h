/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: LPTIM1, LPTIM2, LPTIM3, TIM1, TIM2, TIM16, TIM17
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                          LPTIM1 / LPTIM2 / LPTIM3
******************************************************************************************************************************************/

union LPTIM_ISR {
    struct {
        uint32 CMPM        : 1; // Compare match flag
        uint32 ARRM        : 1; // Auto-reload match flag
        uint32 EXTTRIG     : 1; // External trigger edge event flag
        uint32 CMPOK       : 1; // Compare register update OK flag
        uint32 ARROK       : 1; // Auto-reload register update OK flag
        uint32 UP          : 1; // Counter direction change up flag
        uint32 DOWN        : 1; // Counter direction change down flag
        uint32 UE          : 1; // Update event flag
        uint32 REPOK       : 1; // Repetition register update OK flag
        uint32             : 23;
    } bit;
    uint32 val;
};

union LPTIM_ICR {
    struct {
        uint32 CMPMCF          : 1; // Clear compare match flag
        uint32 ARRMCF          : 1; // Clear auto-reload match flag
        uint32 EXTTRIGCF       : 1; // Clear external trigger flag
        uint32 CMPOKCF         : 1; // Clear compare update OK flag
        uint32 ARROKCF         : 1; // Clear auto-reload update OK flag
        uint32 UPCF            : 1; // Clear up direction flag
        uint32 DOWNCF          : 1; // Clear down direction flag
        uint32 UECF            : 1; // Clear update event flag
        uint32 REPOKCF         : 1; // Clear repetition update OK flag
        uint32                 : 23;
    } bit;
    uint32 val;
};

union LPTIM_IER {
    struct {
        uint32 CMPMIE          : 1; // Compare match interrupt enable
        uint32 ARRMIE          : 1; // Auto-reload match interrupt enable
        uint32 EXTTRIGIE       : 1; // External trigger interrupt enable
        uint32 CMPOKIE         : 1; // Compare update OK interrupt enable
        uint32 ARROKIE         : 1; // Auto-reload update OK interrupt enable
        uint32 UPIE            : 1; // Up direction interrupt enable
        uint32 DOWNIE          : 1; // Down direction interrupt enable
        uint32 UEIE            : 1; // Update event interrupt enable
        uint32 REPOKIE         : 1; // Repetition update OK interrupt enable
        uint32                 : 23;
    } bit;
    uint32 val;
};

union LPTIM_CFGR {
    struct {
        uint32 CKSEL           : 1; // Clock selector
        uint32 CKPOL           : 2; // Clock polarity
        uint32 CKFLT           : 2; // Clock filter
        uint32                 : 1;
        uint32 TRGFLT          : 2; // Trigger filter
        uint32                 : 1;
        uint32 PRESC           : 3; // Clock prescaler
        uint32                 : 1;
        uint32 TRIGSEL         : 3; // Trigger source selection
        uint32                 : 1;
        uint32 TRIGEN          : 2; // Trigger enable and polarity
        uint32 TIMOUT          : 1; // Timeout enable
        uint32 WAVE            : 1; // Waveform shape
        uint32 WAVPOL          : 1; // Waveform polarity
        uint32 PRELOAD         : 1; // Register update mode
        uint32 COUNTMODE       : 1; // Counter mode
        uint32 ENC             : 1; // Encoder mode enable
        uint32 RSTARE          : 1; // Reset after read enable
        uint32 RSTEXT          : 1; // External reset enable
        uint32                 : 5;
    } bit;
    uint32 val;
};

union LPTIM_CR {
    struct {
        uint32 ENABLE      : 1; // LPTIM enable
        uint32 SNGSTRT     : 1; // Single mode start trigger
        uint32 CNTSTRT     : 1; // Continuous mode start trigger
        uint32 COUNTRST    : 1; // Counter reset
        uint32 RSTARE      : 1; // Reset after read enable
        uint32             : 27;
    } bit;
    uint32 val;
};

union LPTIM_CMP {
    struct {
        uint32 CMP     : 16; // Compare value
        uint32         : 16;
    } bit;
    uint32 val;
};

union LPTIM_ARR {
    struct {
        uint32 ARR     : 16; // Auto-reload value
        uint32         : 16;
    } bit;
    uint32 val;
};

union LPTIM_CNT {
    struct {
        uint32 CNT     : 16; // Counter value
        uint32         : 16;
    } bit;
    uint32 val;
};

// Option bits differ in meaning between LPTIM instances.
// Keep a raw value to avoid exposing invalid per-instance bit names.
union LPTIM_OR {
    uint32 val;
};

union LPTIM_RCR {
    struct {
        uint32 REP     : 8; // Repetition counter value
        uint32         : 24;
    } bit;
    uint32 val;
};

struct REG_LPTIM
{
    LPTIM_ISR       ISR;            // Address offset: 0x000
    LPTIM_ICR       ICR;            // Address offset: 0x004
    LPTIM_IER       IER;            // Address offset: 0x008
    LPTIM_CFGR      CFGR;           // Address offset: 0x00C
    LPTIM_CR        CR;             // Address offset: 0x010
    LPTIM_CMP       CMP;            // Address offset: 0x014
    LPTIM_ARR       ARR;            // Address offset: 0x018
    LPTIM_CNT       CNT;            // Address offset: 0x01C
    LPTIM_OR        OR;             // Address offset: 0x020
    uint32          RESERVED0;      // Address offset: 0x024
    LPTIM_RCR       RCR;            // Address offset: 0x028
};

#define REG_LPTIM1 ((volatile REG_LPTIM*)0x40007C00u)
#define REG_LPTIM2 ((volatile REG_LPTIM*)0x40009400u)
#define REG_LPTIM3 ((volatile REG_LPTIM*)0x40009800u)

/******************************************************************************************************************************************
                                                                    TIM1
******************************************************************************************************************************************/

union TIM1_CR1 {
    struct {
        uint32 CEN         : 1; // Counter enable
        uint32 UDIS        : 1; // Update disable
        uint32 URS         : 1; // Update request source
        uint32 OPM         : 1; // One-pulse mode
        uint32 DIR         : 1; // Direction
        uint32 CMS         : 2; // Center-aligned mode selection
        uint32 ARPE        : 1; // Auto-reload preload enable
        uint32 CKD         : 2; // Clock division
        uint32             : 1;
        uint32 UIFREMAP    : 1; // Update interrupt flag remapping
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM1_CR2 {
    struct {
        uint32 CCPC        : 1; // Capture/compare preloaded control
        uint32             : 1;
        uint32 CCUS        : 1; // Capture/compare control update selection
        uint32 CCDS        : 1; // Capture/compare DMA selection
        uint32 MMS         : 3; // Master mode selection
        uint32 TI1S        : 1; // TI1 selection
        uint32 OIS1        : 1; // Output idle state for channel 1
        uint32 OIS1N       : 1; // Output idle state for channel 1
        uint32 OIS2        : 1; // Output idle state for channel 2
        uint32 OIS2N       : 1; // Output idle state for channel 2
        uint32 OIS3        : 1; // Output idle state for channel 3
        uint32 OIS3N       : 1; // Output idle state for channel 3
        uint32 OIS4        : 1; // Output idle state for channel 4
        uint32             : 1;
        uint32 OIS5        : 1; // Output idle state for channel 5
        uint32             : 1;
        uint32 OIS6        : 1; // Output idle state for channel 6
        uint32             : 1;
        uint32 MMS2        : 4; // Master mode selection 2
        uint32             : 8;
    } bit;
    uint32 val;
};

union TIM1_SMCR {
    struct {
        uint32 SMS     : 3; // Slave mode selection
        uint32         : 1;
        uint32 TS      : 3; // Trigger selection
        uint32 MSM     : 1; // Master/slave mode
        uint32 ETF     : 4; // External trigger filter
        uint32 ETPS    : 2; // External trigger prescaler
        uint32 ECE     : 1; // External clock enable
        uint32 ETP     : 1; // External trigger polarity
        uint32 SMS3    : 1; // Slave mode selection bit 3
        uint32         : 3;
        uint32 TS3     : 1; // Trigger selection bit 3
        uint32         : 11;
    } bit;
    uint32 val;
};

union TIM1_DIER {
    struct {
        uint32 UIE         : 1; // Update interrupt enable
        uint32 CC1IE       : 1; // Capture/compare 1 interrupt enable
        uint32 CC2IE       : 1; // Capture/compare 2 interrupt enable
        uint32 CC3IE       : 1; // Capture/compare 3 interrupt enable
        uint32 CC4IE       : 1; // Capture/compare 4 interrupt enable
        uint32 COMIE       : 1; // COM interrupt enable
        uint32 TIE         : 1; // Trigger interrupt enable
        uint32 BIE         : 1; // Break interrupt enable
        uint32 UDE         : 1; // Update DMA request enable
        uint32 CC1DE       : 1; // Capture/compare 1 DMA request enable
        uint32 CC2DE       : 1; // Capture/compare 2 DMA request enable
        uint32 CC3DE       : 1; // Capture/compare 3 DMA request enable
        uint32 CC4DE       : 1; // Capture/compare 4 DMA request enable
        uint32 COMDE       : 1; // COM DMA request enable
        uint32 TDE         : 1; // Trigger DMA request enable
        uint32             : 17;
    } bit;
    uint32 val;
};

union TIM1_SR {
    struct {
        uint32 UIF         : 1; // Update interrupt flag
        uint32 CC1IF       : 1; // Capture/compare 1 interrupt flag
        uint32 CC2IF       : 1; // Capture/compare 2 interrupt flag
        uint32 CC3IF       : 1; // Capture/compare 3 interrupt flag
        uint32 CC4IF       : 1; // Capture/compare 4 interrupt flag
        uint32 COMIF       : 1; // COM interrupt flag
        uint32 TIF         : 1; // Trigger interrupt flag
        uint32 BIF         : 1; // Break interrupt flag
        uint32 B2IF        : 1; // Break 2 interrupt flag
        uint32 CC1OF       : 1; // Capture/compare 1 overcapture flag
        uint32 CC2OF       : 1; // Capture/compare 2 overcapture flag
        uint32 CC3OF       : 1; // Capture/compare 3 overcapture flag
        uint32 CC4OF       : 1; // Capture/compare 4 overcapture flag
        uint32 SBIF        : 1; // System break interrupt flag
        uint32             : 2;
        uint32 CC5IF       : 1; // Capture/compare 5 interrupt flag
        uint32 CC6IF       : 1; // Capture/compare 6 interrupt flag
        uint32             : 14;
    } bit;
    uint32 val;
};

union TIM1_EGR {
    struct {
        uint32 UG      : 1; // Update generation
        uint32 CC1G    : 1; // Capture/compare 1 generation
        uint32 CC2G    : 1; // Capture/compare 2 generation
        uint32 CC3G    : 1; // Capture/compare 3 generation
        uint32 CC4G    : 1; // Capture/compare 4 generation
        uint32 COMG    : 1; // Capture/compare control update generation
        uint32 TG      : 1; // Trigger generation
        uint32 BG      : 1; // Break generation
        uint32 B2G     : 1; // Break 2 generation
        uint32         : 23;
    } bit;
    uint32 val;
};

union TIM1_CCMR1 {
    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 OC1FE       : 1; // Output compare 1 fast enable
        uint32 OC1PE       : 1; // Output compare 1 preload enable
        uint32 OC1M        : 3; // Output compare 1 mode
        uint32 OC1CE       : 1; // Output compare 1 clear enable
        uint32 CC2S        : 2; // Capture/compare 2 selection
        uint32 OC2FE       : 1; // Output compare 2 fast enable
        uint32 OC2PE       : 1; // Output compare 2 preload enable
        uint32 OC2M        : 3; // Output compare 2 mode
        uint32 OC2CE       : 1; // Output compare 2 clear enable
        uint32 OC1M3       : 1; // Output compare 1 mode bit 3
        uint32             : 7;
        uint32 OC2M3       : 1; // Output compare 2 mode bit 3
        uint32             : 7;
    } bitOut;
    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 IC1PSC      : 2; // Input capture 1 prescaler
        uint32 IC1F        : 4; // Input capture 1 filter
        uint32 CC2S        : 2; // Capture/compare 2 selection
        uint32 IC2PSC      : 2; // Input capture 2 prescaler
        uint32 IC2F        : 4; // Input capture 2 filter
        uint32             : 16;
    } bitIn;
    uint32 val;
};

union TIM1_CCMR2 {
    struct {
        uint32 CC3S        : 2; // Capture/compare 3 selection
        uint32 OC3FE       : 1; // Output compare 3 fast enable
        uint32 OC3PE       : 1; // Output compare 3 preload enable
        uint32 OC3M        : 3; // Output compare 3 mode
        uint32 OC3CE       : 1; // Output compare 3 clear enable
        uint32 CC4S        : 2; // Capture/compare 4 selection
        uint32 OC4FE       : 1; // Output compare 4 fast enable
        uint32 OC4PE       : 1; // Output compare 4 preload enable
        uint32 OC4M        : 3; // Output compare 4 mode
        uint32 OC4CE       : 1; // Output compare 4 clear enable
        uint32 OC3M3       : 1; // Output compare 3 mode bit 3
        uint32             : 7;
        uint32 OC4M3       : 1; // Output compare 4 mode bit 3
        uint32             : 7;
    } bitOut;
    struct {
        uint32 CC3S        : 2; // Capture/compare 3 selection
        uint32 IC3PSC      : 2; // Input capture 3 prescaler
        uint32 IC3F        : 4; // Input capture 3 filter
        uint32 CC4S        : 2; // Capture/compare 4 selection
        uint32 IC4PSC      : 2; // Input capture 4 prescaler
        uint32 IC4F        : 4; // Input capture 4 filter
        uint32             : 16;
    } bitIn;
    uint32 val;
};

union TIM1_CCER {
    struct {
        uint32 CC1E        : 1; // Capture/compare 1 output enable
        uint32 CC1P        : 1; // Capture/compare 1 output polarity
        uint32 CC1NE       : 1; // Capture/compare 1 complementary output enable
        uint32 CC1NP       : 1; // Capture/compare 1 complementary output polarity
        uint32 CC2E        : 1; // Capture/compare 2 output enable
        uint32 CC2P        : 1; // Capture/compare 2 output polarity
        uint32 CC2NE       : 1; // Capture/compare 2 complementary output enable
        uint32 CC2NP       : 1; // Capture/compare 2 complementary output polarity
        uint32 CC3E        : 1; // Capture/compare 3 output enable
        uint32 CC3P        : 1; // Capture/compare 3 output polarity
        uint32 CC3NE       : 1; // Capture/compare 3 complementary output enable
        uint32 CC3NP       : 1; // Capture/compare 3 complementary output polarity
        uint32 CC4E        : 1; // Capture/compare 4 output enable
        uint32 CC4P        : 1; // Capture/compare 4 output polarity
        uint32             : 2;
        uint32 CC5E        : 1; // Capture/compare 5 output enable
        uint32 CC5P        : 1; // Capture/compare 5 output polarity
        uint32             : 2;
        uint32 CC6E        : 1; // Capture/compare 6 output enable
        uint32 CC6P        : 1; // Capture/compare 6 output polarity
        uint32             : 10;
    } bit;
    uint32 val;
};

union TIM1_CNT {
    struct {
        uint32 CNT         : 16; // Counter value
        uint32             : 15;
        uint32 UIFCPY      : 1; // Update interrupt flag copy
    } bit;
    uint32 val;
};

union TIM1_PSC {
    struct {
        uint32 PSC     : 16; // Prescaler value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM1_ARR {
    struct {
        uint32 ARR     : 16; // Auto-reload value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM1_RCR {
    struct {
        uint32 REP     : 16; // Repetition counter value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM1_CCR {
    struct {
        uint32 CCR     : 16; // Capture/compare value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM1_BDTR {
    struct {
        uint32 DTG     : 8; // Dead-time generator setup
        uint32 LOCK    : 2; // Lock configuration
        uint32 OSSI    : 1; // Off-state selection for Idle mode
        uint32 OSSR    : 1; // Off-state selection for Run mode
        uint32 BKE     : 1; // Break input enable
        uint32 BKP     : 1; // Break input polarity
        uint32 AOE     : 1; // Automatic output enable
        uint32 MOE     : 1; // Main output enable
        uint32 BKF     : 4; // Break input filter
        uint32 BK2F    : 4; // Break 2 input filter
        uint32 BK2E    : 1; // Break 2 input enable
        uint32 BK2P    : 1; // Break 2 input polarity
        uint32         : 6;
    } bit;
    uint32 val;
};

union TIM1_DCR {
    struct {
        uint32 DBA     : 5; // DMA base address
        uint32         : 3;
        uint32 DBL     : 5; // DMA burst length
        uint32         : 19;
    } bit;
    uint32 val;
};

union TIM1_DMAR {
    struct {
        uint32 DMAB    : 16; // DMA register for burst accesses
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM1_OR1 {
    uint32 val;
};
union TIM1_CCMR3 {
    uint32 val;
};
union TIM1_AF1 {
    uint32 val;
};
union TIM1_AF2 {
    uint32 val;
};

struct REG_TIM1
{
    TIM1_CR1        CR1;        // Address offset: 0x000
    TIM1_CR2        CR2;        // Address offset: 0x004
    TIM1_SMCR       SMCR;       // Address offset: 0x008
    TIM1_DIER       DIER;       // Address offset: 0x00C
    TIM1_SR         SR;         // Address offset: 0x010
    TIM1_EGR        EGR;        // Address offset: 0x014
    TIM1_CCMR1      CCMR1;      // Address offset: 0x018
    TIM1_CCMR2      CCMR2;      // Address offset: 0x01C
    TIM1_CCER       CCER;       // Address offset: 0x020
    TIM1_CNT        CNT;        // Address offset: 0x024
    TIM1_PSC        PSC;        // Address offset: 0x028
    TIM1_ARR        ARR;        // Address offset: 0x02C
    TIM1_RCR        RCR;        // Address offset: 0x030
    TIM1_CCR        CCR1;       // Address offset: 0x034
    TIM1_CCR        CCR2;       // Address offset: 0x038
    TIM1_CCR        CCR3;       // Address offset: 0x03C
    TIM1_CCR        CCR4;       // Address offset: 0x040
    TIM1_BDTR       BDTR;       // Address offset: 0x044
    TIM1_DCR        DCR;        // Address offset: 0x048
    TIM1_DMAR       DMAR;       // Address offset: 0x04C
    TIM1_OR1        OR1;        // Address offset: 0x050
    TIM1_CCMR3      CCMR3;      // Address offset: 0x054
    TIM1_CCR        CCR5;       // Address offset: 0x058
    TIM1_CCR        CCR6;       // Address offset: 0x05C
    TIM1_AF1        AF1;        // Address offset: 0x060
    TIM1_AF2        AF2;        // Address offset: 0x064
};

#define REG_TIM1 ((volatile REG_TIM1*)0x40012C00u)

/******************************************************************************************************************************************
                                                                    TIM2
******************************************************************************************************************************************/

union TIM2_CR1 {
    struct {
        uint32 CEN         : 1; // Counter enable
        uint32 UDIS        : 1; // Update disable
        uint32 URS         : 1; // Update request source
        uint32 OPM         : 1; // One-pulse mode
        uint32 DIR         : 1; // Direction
        uint32 CMS         : 2; // Center-aligned mode selection
        uint32 ARPE        : 1; // Auto-reload preload enable
        uint32 CKD         : 2; // Clock division
        uint32             : 1;
        uint32 UIFREMAP    : 1; // Update interrupt flag remapping
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM2_CR2 {
    struct {
        uint32         : 3;
        uint32 CCDS    : 1; // Capture/compare DMA selection
        uint32 MMS     : 3; // Master mode selection
        uint32 TI1S    : 1; // TI1 selection
        uint32         : 12;
        uint32 MMS2    : 4; // Master mode selection 2
        uint32         : 8;
    } bit;
    uint32 val;
};

union TIM2_SMCR {
    struct {
        uint32 SMS     : 3; // Slave mode selection
        uint32         : 1;
        uint32 TS      : 3; // Trigger selection
        uint32 MSM     : 1; // Master/slave mode
        uint32 ETF     : 4; // External trigger filter
        uint32 ETPS    : 2; // External trigger prescaler
        uint32 ECE     : 1; // External clock enable
        uint32 ETP     : 1; // External trigger polarity
        uint32 SMS3    : 1; // Slave mode selection bit 3
        uint32         : 3;
        uint32 TS3     : 1; // Trigger selection bit 3
        uint32         : 11;
    } bit;
    uint32 val;
};

union TIM2_DIER {
    struct {
        uint32 UIE         : 1; // Update interrupt enable
        uint32 CC1IE       : 1; // Capture/compare 1 interrupt enable
        uint32 CC2IE       : 1; // Capture/compare 2 interrupt enable
        uint32 CC3IE       : 1; // Capture/compare 3 interrupt enable
        uint32 CC4IE       : 1; // Capture/compare 4 interrupt enable
        uint32             : 1;
        uint32 TIE         : 1; // Trigger interrupt enable
        uint32             : 1;
        uint32 UDE         : 1; // Update DMA request enable
        uint32 CC1DE       : 1; // Capture/compare 1 DMA request enable
        uint32 CC2DE       : 1; // Capture/compare 2 DMA request enable
        uint32 CC3DE       : 1; // Capture/compare 3 DMA request enable
        uint32 CC4DE       : 1; // Capture/compare 4 DMA request enable
        uint32             : 1;
        uint32 TDE         : 1; // Trigger DMA request enable
        uint32             : 17;
    } bit;
    uint32 val;
};

union TIM2_SR {
    struct {
        uint32 UIF         : 1; // Update interrupt flag
        uint32 CC1IF       : 1; // Capture/compare 1 interrupt flag
        uint32 CC2IF       : 1; // Capture/compare 2 interrupt flag
        uint32 CC3IF       : 1; // Capture/compare 3 interrupt flag
        uint32 CC4IF       : 1; // Capture/compare 4 interrupt flag
        uint32             : 1;
        uint32 TIF         : 1; // Trigger interrupt flag
        uint32             : 2;
        uint32 CC1OF       : 1; // Capture/compare 1 overcapture flag
        uint32 CC2OF       : 1; // Capture/compare 2 overcapture flag
        uint32 CC3OF       : 1; // Capture/compare 3 overcapture flag
        uint32 CC4OF       : 1; // Capture/compare 4 overcapture flag
        uint32             : 19;
    } bit;
    uint32 val;
};

union TIM2_EGR {
    struct {
        uint32 UG      : 1; // Update generation
        uint32 CC1G    : 1; // Capture/compare 1 generation
        uint32 CC2G    : 1; // Capture/compare 2 generation
        uint32 CC3G    : 1; // Capture/compare 3 generation
        uint32 CC4G    : 1; // Capture/compare 4 generation
        uint32         : 1;
        uint32 TG      : 1; // Trigger generation
        uint32         : 25;
    } bit;
    uint32 val;
};

union TIM2_CCMR1 {
    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 OC1FE       : 1; // Output compare 1 fast enable
        uint32 OC1PE       : 1; // Output compare 1 preload enable
        uint32 OC1M        : 3; // Output compare 1 mode
        uint32 OC1CE       : 1; // Output compare 1 clear enable
        uint32 CC2S        : 2; // Capture/compare 2 selection
        uint32 OC2FE       : 1; // Output compare 2 fast enable
        uint32 OC2PE       : 1; // Output compare 2 preload enable
        uint32 OC2M        : 3; // Output compare 2 mode
        uint32 OC2CE       : 1; // Output compare 2 clear enable
        uint32 OC1M3       : 1; // Output compare 1 mode bit 3
        uint32             : 7;
        uint32 OC2M3       : 1; // Output compare 2 mode bit 3
        uint32             : 7;
    } bitOut;
    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 IC1PSC      : 2; // Input capture 1 prescaler
        uint32 IC1F        : 4; // Input capture 1 filter
        uint32 CC2S        : 2; // Capture/compare 2 selection
        uint32 IC2PSC      : 2; // Input capture 2 prescaler
        uint32 IC2F        : 4; // Input capture 2 filter
        uint32             : 16;
    } bitIn;
    uint32 val;
};

union TIM2_CCMR2 {
    struct {
        uint32 CC3S        : 2; // Capture/compare 3 selection
        uint32 OC3FE       : 1; // Output compare 3 fast enable
        uint32 OC3PE       : 1; // Output compare 3 preload enable
        uint32 OC3M        : 3; // Output compare 3 mode
        uint32 OC3CE       : 1; // Output compare 3 clear enable
        uint32 CC4S        : 2; // Capture/compare 4 selection
        uint32 OC4FE       : 1; // Output compare 4 fast enable
        uint32 OC4PE       : 1; // Output compare 4 preload enable
        uint32 OC4M        : 3; // Output compare 4 mode
        uint32 OC4CE       : 1; // Output compare 4 clear enable
        uint32 OC3M3       : 1; // Output compare 3 mode bit 3
        uint32             : 7;
        uint32 OC4M3       : 1; // Output compare 4 mode bit 3
        uint32             : 7;
    } bitOut;
    struct {
        uint32 CC3S        : 2; // Capture/compare 3 selection
        uint32 IC3PSC      : 2; // Input capture 3 prescaler
        uint32 IC3F        : 4; // Input capture 3 filter
        uint32 CC4S        : 2; // Capture/compare 4 selection
        uint32 IC4PSC      : 2; // Input capture 4 prescaler
        uint32 IC4F        : 4; // Input capture 4 filter
        uint32             : 16;
    } bitIn;
    uint32 val;
};

union TIM2_CCER {
    struct {
        uint32 CC1E        : 1; // Capture/compare 1 output enable
        uint32 CC1P        : 1; // Capture/compare 1 output polarity
        uint32             : 1;
        uint32 CC1NP       : 1; // Capture/compare 1 complementary output polarity
        uint32 CC2E        : 1; // Capture/compare 2 output enable
        uint32 CC2P        : 1; // Capture/compare 2 output polarity
        uint32             : 1;
        uint32 CC2NP       : 1; // Capture/compare 2 complementary output polarity
        uint32 CC3E        : 1; // Capture/compare 3 output enable
        uint32 CC3P        : 1; // Capture/compare 3 output polarity
        uint32             : 1;
        uint32 CC3NP       : 1; // Capture/compare 3 complementary output polarity
        uint32 CC4E        : 1; // Capture/compare 4 output enable
        uint32 CC4P        : 1; // Capture/compare 4 output polarity
        uint32             : 1;
        uint32 CC4NP       : 1; // Capture/compare 4 complementary output polarity
        uint32             : 16;
    } bit;
    uint32 val;
};

union TIM2_CNT {
    struct {
        uint32 CNT         : 31; // Counter value
        uint32 UIFCPY      : 1; // Update interrupt flag copy
    } bit;
    uint32 val;
};

union TIM2_PSC {
    struct {
        uint32 PSC     : 16; // Prescaler value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM2_ARR {
    uint32 val;
};
union TIM2_CCR {
    uint32 val;
};

union TIM2_DCR {
    struct {
        uint32 DBA     : 5; // DMA base address
        uint32         : 3;
        uint32 DBL     : 5; // DMA burst length
        uint32         : 19;
    } bit;
    uint32 val;
};

union TIM2_DMAR {
    uint32 val;
};
union TIM2_OR1 {
    uint32 val;
};
union TIM2_AF1 {
    uint32 val;
};
union TIM2_AF2 {
    uint32 val;
};

struct REG_TIM2
{
    TIM2_CR1        CR1;            // Address offset: 0x000
    TIM2_CR2        CR2;            // Address offset: 0x004
    TIM2_SMCR       SMCR;           // Address offset: 0x008
    TIM2_DIER       DIER;           // Address offset: 0x00C
    TIM2_SR         SR;             // Address offset: 0x010
    TIM2_EGR        EGR;            // Address offset: 0x014
    TIM2_CCMR1      CCMR1;          // Address offset: 0x018
    TIM2_CCMR2      CCMR2;          // Address offset: 0x01C
    TIM2_CCER       CCER;           // Address offset: 0x020
    TIM2_CNT        CNT;            // Address offset: 0x024
    TIM2_PSC        PSC;            // Address offset: 0x028
    TIM2_ARR        ARR;            // Address offset: 0x02C
    uint32          RESERVED0;      // Address offset: 0x030
    TIM2_CCR        CCR1;           // Address offset: 0x034
    TIM2_CCR        CCR2;           // Address offset: 0x038
    TIM2_CCR        CCR3;           // Address offset: 0x03C
    TIM2_CCR        CCR4;           // Address offset: 0x040
    uint32          RESERVED1;      // Address offset: 0x044
    TIM2_DCR        DCR;            // Address offset: 0x048
    TIM2_DMAR       DMAR;           // Address offset: 0x04C
    TIM2_OR1        OR1;            // Address offset: 0x050
    uint32          RESERVED2;      // Address offset: 0x054
    uint32          RESERVED3;      // Address offset: 0x058
    uint32          RESERVED4;      // Address offset: 0x05C
    TIM2_AF1        AF1;            // Address offset: 0x060
    TIM2_AF2        AF2;            // Address offset: 0x064
};

#define REG_TIM2 ((volatile REG_TIM2*)0x40000000u)

/******************************************************************************************************************************************
                                                                    TIM16
******************************************************************************************************************************************/

union TIM16_CR1 {
    struct {
        uint32 CEN         : 1; // Counter enable
        uint32 UDIS        : 1; // Update disable
        uint32 URS         : 1; // Update request source
        uint32 OPM         : 1; // One-pulse mode
        uint32             : 3;
        uint32 ARPE        : 1; // Auto-reload preload enable
        uint32 CKD         : 2; // Clock division
        uint32             : 1;
        uint32 UIFREMAP    : 1; // Update interrupt flag remapping
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM16_CR2 {
    struct {
        uint32 CCPC        : 1; // Capture/compare preloaded control
        uint32             : 1;
        uint32 CCUS        : 1; // Capture/compare control update selection
        uint32 CCDS        : 1; // Capture/compare DMA selection
        uint32 MMS         : 3; // Master mode selection
        uint32 TI1S        : 1; // TI1 selection
        uint32 OIS1        : 1; // Output idle state for channel 1
        uint32 OIS1N       : 1; // Output idle state for channel 1
        uint32             : 22;
    } bit;
    uint32 val;
};

union TIM16_DIER {
    struct {
        uint32 UIE         : 1; // Update interrupt enable
        uint32 CC1IE       : 1; // Capture/compare 1 interrupt enable
        uint32             : 3;
        uint32 COMIE       : 1; // COM interrupt enable
        uint32 TIE         : 1; // Trigger interrupt enable
        uint32 BIE         : 1; // Break interrupt enable
        uint32 UDE         : 1; // Update DMA request enable
        uint32 CC1DE       : 1; // Capture/compare 1 DMA request enable
        uint32             : 4;
        uint32 COMDE       : 1; // COM DMA request enable
        uint32             : 17;
    } bit;
    uint32 val;
};

union TIM16_SR {
    struct {
        uint32 UIF         : 1; // Update interrupt flag
        uint32 CC1IF       : 1; // Capture/compare 1 interrupt flag
        uint32             : 3;
        uint32 COMIF       : 1; // COM interrupt flag
        uint32 TIF         : 1; // Trigger interrupt flag
        uint32 BIF         : 1; // Break interrupt flag
        uint32 B2IF        : 1; // Break 2 interrupt flag
        uint32 CC1OF       : 1; // Capture/compare 1 overcapture flag
        uint32 SBIF        : 1; // System break interrupt flag
        uint32 CC1IFC      : 1; // Capture/compare 1 interrupt flag clear
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM16_EGR {
    struct {
        uint32 UG      : 1; // Update generation
        uint32 CC1G    : 1; // Capture/compare 1 generation
        uint32         : 3;
        uint32 COMG    : 1; // Capture/compare control update generation
        uint32 TG      : 1; // Trigger generation
        uint32 BG      : 1; // Break generation
        uint32 B2G     : 1; // Break 2 generation
        uint32         : 23;
    } bit;
    uint32 val;
};

union TIM16_CCMR1 {
    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 OC1FE       : 1; // Output compare 1 fast enable
        uint32 OC1PE       : 1; // Output compare 1 preload enable
        uint32 OC1M        : 3; // Output compare 1 mode
        uint32 OC1CE       : 1; // Output compare 1 clear enable
        uint32             : 8;
        uint32 OC1M3       : 1; // Output compare 1 mode bit 3
        uint32             : 15;
    } bitOut;

    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 IC1PSC      : 2; // Input capture 1 prescaler
        uint32 IC1F        : 4; // Input capture 1 filter
        uint32             : 24;
    } bitIn;

    uint32 val;
};

union TIM16_CCER {
    struct {
        uint32 CC1E        : 1; // Capture/compare 1 output enable
        uint32 CC1P        : 1; // Capture/compare 1 output polarity
        uint32 CC1NE       : 1; // Capture/compare 1 complementary output enable
        uint32 CC1NP       : 1; // Capture/compare 1 complementary output polarity
        uint32             : 28;
    } bit;
    uint32 val;
};

union TIM16_CNT {
    struct {
        uint32 CNT         : 16; // Counter value
        uint32 UIFCPY      : 1; // Update interrupt flag copy
        uint32             : 15;
    } bit;
    uint32 val;
};

union TIM16_PSC {
    struct {
        uint32 PSC     : 16; // Prescaler value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM16_ARR {
    struct {
        uint32 ARR     : 16; // Auto-reload value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM16_RCR {
    struct {
        uint32 REP     : 8; // Repetition counter value
        uint32         : 24;
    } bit;
    uint32 val;
};

union TIM16_CCR1 {
    struct {
        uint32 CCR1    : 16; // Capture/compare 1 value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM16_BDTR {
    struct {
        uint32 DTG     : 8; // Dead-time generator setup
        uint32 LOCK    : 2; // Lock configuration
        uint32 OSSI    : 1; // Off-state selection for Idle mode
        uint32 OSSR    : 1; // Off-state selection for Run mode
        uint32 BKE     : 1; // Break input enable
        uint32 BKP     : 1; // Break input polarity
        uint32 AOE     : 1; // Automatic output enable
        uint32 MOE     : 1; // Main output enable
        uint32 BKF     : 4; // Break input filter
        uint32 BK2F    : 4; // Break 2 input filter
        uint32 BK2E    : 1; // Break 2 input enable
        uint32 BK2P    : 1; // Break 2 input polarity
        uint32         : 6;
    } bit;
    uint32 val;
};

union TIM16_DCR {
    struct {
        uint32 DBA     : 5; // DMA base address
        uint32         : 3;
        uint32 DBL     : 5; // DMA burst length
        uint32         : 19;
    } bit;
    uint32 val;
};

union TIM16_DMAR {
    struct {
        uint32 DMAB    : 16; // DMA register for burst accesses
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM16_OR1 {
    struct {
        uint32 TI1_RMP     : 2; // TI1 remapping
        uint32             : 30;
    } bit;
    uint32 val;
};

union TIM16_AF1 {
    struct {
        uint32 BKINE       : 1; // Break input enable source
        uint32 BKCMP1E     : 1; // COMP1 break input enable
        uint32 BKCMP2E     : 1; // COMP2 break input enable
        uint32             : 6;
        uint32 BKINP       : 1; // Break input polarity
        uint32 BKCMP1P     : 1; // COMP1 break input polarity
        uint32 BKCMP2P     : 1; // COMP2 break input polarity
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM16_TISEL {
    struct {
        uint32 TI1SEL      : 4; // TI1 input selection
        uint32             : 28;
    } bit;
    uint32 val;
};

struct REG_TIM16
{
    TIM16_CR1       CR1;            // Address offset: 0x000
    TIM16_CR2       CR2;            // Address offset: 0x004
    uint32          RESERVED0;      // Address offset: 0x008
    TIM16_DIER      DIER;           // Address offset: 0x00C
    TIM16_SR        SR;             // Address offset: 0x010
    TIM16_EGR       EGR;            // Address offset: 0x014
    TIM16_CCMR1     CCMR1;          // Address offset: 0x018
    uint32          RESERVED1;      // Address offset: 0x01C
    TIM16_CCER      CCER;           // Address offset: 0x020
    TIM16_CNT       CNT;            // Address offset: 0x024
    TIM16_PSC       PSC;            // Address offset: 0x028
    TIM16_ARR       ARR;            // Address offset: 0x02C
    TIM16_RCR       RCR;            // Address offset: 0x030
    TIM16_CCR1      CCR1;           // Address offset: 0x034
    uint32          RESERVED2;      // Address offset: 0x038
    uint32          RESERVED3;      // Address offset: 0x03C
    uint32          RESERVED4;      // Address offset: 0x040
    TIM16_BDTR      BDTR;           // Address offset: 0x044
    TIM16_DCR       DCR;            // Address offset: 0x048
    TIM16_DMAR      DMAR;           // Address offset: 0x04C
    TIM16_OR1       OR1;            // Address offset: 0x050
    uint32          RESERVED5;      // Address offset: 0x054
    uint32          RESERVED6;      // Address offset: 0x058
    uint32          RESERVED7;      // Address offset: 0x05C
    TIM16_AF1       AF1;            // Address offset: 0x060
    uint32          RESERVED8;      // Address offset: 0x064
    TIM16_TISEL     TISEL;          // Address offset: 0x068
};

#define REG_TIM16 ((volatile REG_TIM16*)0x40014400u)

/******************************************************************************************************************************************
                                                                    TIM17
******************************************************************************************************************************************/

union TIM17_CR1 {
    struct {
        uint32 CEN         : 1; // Counter enable
        uint32 UDIS        : 1; // Update disable
        uint32 URS         : 1; // Update request source
        uint32 OPM         : 1; // One-pulse mode
        uint32             : 3;
        uint32 ARPE        : 1; // Auto-reload preload enable
        uint32 CKD         : 2; // Clock division
        uint32             : 1;
        uint32 UIFREMAP    : 1; // Update interrupt flag remapping
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM17_CR2 {
    struct {
        uint32 CCPC        : 1; // Capture/compare preloaded control
        uint32             : 1;
        uint32 CCUS        : 1; // Capture/compare control update selection
        uint32 CCDS        : 1; // Capture/compare DMA selection
        uint32 MMS         : 3; // Master mode selection
        uint32 TI1S        : 1; // TI1 selection
        uint32 OIS1        : 1; // Output idle state for channel 1
        uint32 OIS1N       : 1; // Output idle state for channel 1
        uint32             : 22;
    } bit;
    uint32 val;
};

union TIM17_DIER {
    struct {
        uint32 UIE         : 1; // Update interrupt enable
        uint32 CC1IE       : 1; // Capture/compare 1 interrupt enable
        uint32             : 3;
        uint32 COMIE       : 1; // COM interrupt enable
        uint32 TIE         : 1; // Trigger interrupt enable
        uint32 BIE         : 1; // Break interrupt enable
        uint32 UDE         : 1; // Update DMA request enable
        uint32 CC1DE       : 1; // Capture/compare 1 DMA request enable
        uint32             : 4;
        uint32 COMDE       : 1; // COM DMA request enable
        uint32             : 17;
    } bit;
    uint32 val;
};

union TIM17_SR {
    struct {
        uint32 UIF         : 1; // Update interrupt flag
        uint32 CC1IF       : 1; // Capture/compare 1 interrupt flag
        uint32             : 3;
        uint32 COMIF       : 1; // COM interrupt flag
        uint32 TIF         : 1; // Trigger interrupt flag
        uint32 BIF         : 1; // Break interrupt flag
        uint32 B2IF        : 1; // Break 2 interrupt flag
        uint32 CC1OF       : 1; // Capture/compare 1 overcapture flag
        uint32 SBIF        : 1; // System break interrupt flag
        uint32 CC1IFC      : 1; // Capture/compare 1 interrupt flag clear
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM17_EGR {
    struct {
        uint32 UG      : 1; // Update generation
        uint32 CC1G    : 1; // Capture/compare 1 generation
        uint32         : 3;
        uint32 COMG    : 1; // Capture/compare control update generation
        uint32 TG      : 1; // Trigger generation
        uint32 BG      : 1; // Break generation
        uint32 B2G     : 1; // Break 2 generation
        uint32         : 23;
    } bit;
    uint32 val;
};

union TIM17_CCMR1 {
    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 OC1FE       : 1; // Output compare 1 fast enable
        uint32 OC1PE       : 1; // Output compare 1 preload enable
        uint32 OC1M        : 3; // Output compare 1 mode
        uint32 OC1CE       : 1; // Output compare 1 clear enable
        uint32             : 8;
        uint32 OC1M3       : 1; // Output compare 1 mode bit 3
        uint32             : 15;
    } bitOut;

    struct {
        uint32 CC1S        : 2; // Capture/compare 1 selection
        uint32 IC1PSC      : 2; // Input capture 1 prescaler
        uint32 IC1F        : 4; // Input capture 1 filter
        uint32             : 24;
    } bitIn;

    uint32 val;
};

union TIM17_CCER {
    struct {
        uint32 CC1E        : 1; // Capture/compare 1 output enable
        uint32 CC1P        : 1; // Capture/compare 1 output polarity
        uint32 CC1NE       : 1; // Capture/compare 1 complementary output enable
        uint32 CC1NP       : 1; // Capture/compare 1 complementary output polarity
        uint32             : 28;
    } bit;
    uint32 val;
};

union TIM17_CNT {
    struct {
        uint32 CNT         : 16; // Counter value
        uint32 UIFCPY      : 1; // Update interrupt flag copy
        uint32             : 15;
    } bit;
    uint32 val;
};

union TIM17_PSC {
    struct {
        uint32 PSC     : 16; // Prescaler value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM17_ARR {
    struct {
        uint32 ARR     : 16; // Auto-reload value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM17_RCR {
    struct {
        uint32 REP     : 8; // Repetition counter value
        uint32         : 24;
    } bit;
    uint32 val;
};

union TIM17_CCR1 {
    struct {
        uint32 CCR1    : 16; // Capture/compare 1 value
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM17_BDTR {
    struct {
        uint32 DTG     : 8; // Dead-time generator setup
        uint32 LOCK    : 2; // Lock configuration
        uint32 OSSI    : 1; // Off-state selection for Idle mode
        uint32 OSSR    : 1; // Off-state selection for Run mode
        uint32 BKE     : 1; // Break input enable
        uint32 BKP     : 1; // Break input polarity
        uint32 AOE     : 1; // Automatic output enable
        uint32 MOE     : 1; // Main output enable
        uint32 BKF     : 4; // Break input filter
        uint32 BK2F    : 4; // Break 2 input filter
        uint32 BK2E    : 1; // Break 2 input enable
        uint32 BK2P    : 1; // Break 2 input polarity
        uint32         : 6;
    } bit;
    uint32 val;
};

union TIM17_DCR {
    struct {
        uint32 DBA     : 5; // DMA base address
        uint32         : 3;
        uint32 DBL     : 5; // DMA burst length
        uint32         : 19;
    } bit;
    uint32 val;
};

union TIM17_DMAR {
    struct {
        uint32 DMAB    : 16; // DMA register for burst accesses
        uint32         : 16;
    } bit;
    uint32 val;
};

union TIM17_OR1 {
    struct {
        uint32 TI1_RMP     : 2; // TI1 remapping
        uint32             : 30;
    } bit;
    uint32 val;
};

union TIM17_AF1 {
    struct {
        uint32 BKINE       : 1; // Break input enable source
        uint32 BKCMP1E     : 1; // COMP1 break input enable
        uint32 BKCMP2E     : 1; // COMP2 break input enable
        uint32             : 6;
        uint32 BKINP       : 1; // Break input polarity
        uint32 BKCMP1P     : 1; // COMP1 break input polarity
        uint32 BKCMP2P     : 1; // COMP2 break input polarity
        uint32             : 20;
    } bit;
    uint32 val;
};

union TIM17_TISEL {
    struct {
        uint32 TI1SEL      : 4; // TI1 input selection
        uint32             : 28;
    } bit;
    uint32 val;
};

struct REG_TIM17
{
    TIM17_CR1       CR1;            // Address offset: 0x000
    TIM17_CR2       CR2;            // Address offset: 0x004
    uint32          RESERVED0;      // Address offset: 0x008
    TIM17_DIER      DIER;           // Address offset: 0x00C
    TIM17_SR        SR;             // Address offset: 0x010
    TIM17_EGR       EGR;            // Address offset: 0x014
    TIM17_CCMR1     CCMR1;          // Address offset: 0x018
    uint32          RESERVED1;      // Address offset: 0x01C
    TIM17_CCER      CCER;           // Address offset: 0x020
    TIM17_CNT       CNT;            // Address offset: 0x024
    TIM17_PSC       PSC;            // Address offset: 0x028
    TIM17_ARR       ARR;            // Address offset: 0x02C
    TIM17_RCR       RCR;            // Address offset: 0x030
    TIM17_CCR1      CCR1;           // Address offset: 0x034
    uint32          RESERVED2;      // Address offset: 0x038
    uint32          RESERVED3;      // Address offset: 0x03C
    uint32          RESERVED4;      // Address offset: 0x040
    TIM17_BDTR      BDTR;           // Address offset: 0x044
    TIM17_DCR       DCR;            // Address offset: 0x048
    TIM17_DMAR      DMAR;           // Address offset: 0x04C
    TIM17_OR1       OR1;            // Address offset: 0x050
    uint32          RESERVED5;      // Address offset: 0x054
    uint32          RESERVED6;      // Address offset: 0x058
    uint32          RESERVED7;      // Address offset: 0x05C
    TIM17_AF1       AF1;            // Address offset: 0x060
    uint32          RESERVED8;      // Address offset: 0x064
    TIM17_TISEL     TISEL;          // Address offset: 0x068
};

#define REG_TIM17 ((volatile REG_TIM17*)0x40014800u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
