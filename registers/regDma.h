/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: DMA
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union DMA_ISR {
    struct {
        uint32 GIF1     : 1;    // Global interrupt flag for channel 1
        uint32 TCIF1    : 1;    // Transfer complete (TC) flag for channel 1
        uint32 HTIF1    : 1;    // Half transfer (HT) flag for channel 1
        uint32 TEIF1    : 1;    // Transfer error (TE) flag for channel 1
        uint32 GIF2     : 1;    // Global interrupt flag for channel 2
        uint32 TCIF2    : 1;    // Transfer complete (TC) flag for channel 2
        uint32 HTIF2    : 1;    // Half transfer (HT) flag for channel 2
        uint32 TEIF2    : 1;    // Transfer error (TE) flag for channel 2
        uint32 GIF3     : 1;    // Global interrupt flag for channel 3
        uint32 TCIF3    : 1;    // Transfer complete (TC) flag for channel 3
        uint32 HTIF3    : 1;    // Half transfer (HT) flag for channel 3
        uint32 TEIF3    : 1;    // Transfer error (TE) flag for channel 3
        uint32 GIF4     : 1;    // Global interrupt flag for channel 4
        uint32 TCIF4    : 1;    // Transfer complete (TC) flag for channel 4
        uint32 HTIF4    : 1;    // Half transfer (HT) flag for channel 4
        uint32 TEIF4    : 1;    // Transfer error (TE) flag for channel 4
        uint32 GIF5     : 1;    // Global interrupt flag for channel 5
        uint32 TCIF5    : 1;    // Transfer complete (TC) flag for channel 5
        uint32 HTIF5    : 1;    // Half transfer (HT) flag for channel 5
        uint32 TEIF5    : 1;    // Transfer error (TE) flag for channel 5
        uint32 GIF6     : 1;    // Global interrupt flag for channel 6
        uint32 TCIF6    : 1;    // Transfer complete (TC) flag for channel 6
        uint32 HTIF6    : 1;    // Half transfer (HT) flag for channel 6
        uint32 TEIF6    : 1;    // Transfer error (TE) flag for channel 6
        uint32 GIF7     : 1;    // Global interrupt flag for channel 7
        uint32 TCIF7    : 1;    // Transfer complete (TC) flag for channel 7
        uint32 HTIF7    : 1;    // Half transfer (HT) flag for channel 7
        uint32 TEIF7    : 1;    // Transfer error (TE) flag for channel 7
        uint32          : 4;
    } bit;
    uint32 val;
};

union DMA_IFCR {
    struct {
        uint32 CGIF1    : 1;    // Global interrupt flag clear for channel 1
        uint32 CTCIF1   : 1;    // Transfer complete flag clear for channel 1
        uint32 CHTIF1   : 1;    // Half transfer flag clear for channel 1
        uint32 CTEIF1   : 1;    // Transfer error flag clear for channel 1
        uint32 CGIF2    : 1;    // Global interrupt flag clear for channel 2
        uint32 CTCIF2   : 1;    // Transfer complete flag clear for channel 2
        uint32 CHTIF2   : 1;    // Half transfer flag clear for channel 2
        uint32 CTEIF2   : 1;    // Transfer error flag clear for channel 2
        uint32 CGIF3    : 1;    // Global interrupt flag clear for channel 3
        uint32 CTCIF3   : 1;    // Transfer complete flag clear for channel 3
        uint32 CHTIF3   : 1;    // Half transfer flag clear for channel 3
        uint32 CTEIF3   : 1;    // Transfer error flag clear for channel 3
        uint32 CGIF4    : 1;    // Global interrupt flag clear for channel 4
        uint32 CTCIF4   : 1;    // Transfer complete flag clear for channel 4
        uint32 CHTIF4   : 1;    // Half transfer flag clear for channel 4
        uint32 CTEIF4   : 1;    // Transfer error flag clear for channel 4
        uint32 CGIF5    : 1;    // Global interrupt flag clear for channel 5
        uint32 CTCIF5   : 1;    // Transfer complete flag clear for channel 5
        uint32 CHTIF5   : 1;    // Half transfer flag clear for channel 5
        uint32 CTEIF5   : 1;    // Transfer error flag clear for channel 5
        uint32 CGIF6    : 1;    // Global interrupt flag clear for channel 6
        uint32 CTCIF6   : 1;    // Transfer complete flag clear for channel 6
        uint32 CHTIF6   : 1;    // Half transfer flag clear for channel 6
        uint32 CTEIF6   : 1;    // Transfer error flag clear for channel 6
        uint32 CGIF7    : 1;    // Global interrupt flag clear for channel 7
        uint32 CTCIF7   : 1;    // Transfer complete flag clear for channel 7
        uint32 CHTIF7   : 1;    // Half transfer flag clear for channel 7
        uint32 CTEIF7   : 1;    // Transfer error flag clear for channel 7
        uint32          : 4;
    } bit;
    uint32 val;
};

union DMA_CCRX {
    struct {
        uint32 EN       : 1;    // Channel enable
        uint32 TCIE     : 1;    // Transfer complete interrupt enable
        uint32 HTIE     : 1;    // Half transfer interrupt enable
        uint32 TEIE     : 1;    // Transfer error interrupt enable
        uint32 DIR      : 1;    // Data transfer direction
        uint32 CIRC     : 1;    // Circular mode
        uint32 PINC     : 1;    // Peripheral increment mode
        uint32 MINC     : 1;    // Memory increment mode
        uint32 PSIZE    : 2;    // Peripheral size
        uint32 MSIZE    : 2;    // Memory size
        uint32 PL       : 2;    // Priority level
        uint32 MEM2MEM  : 1;    // Memory-to-memory mode
        uint32          : 5;
        uint32 PRIV     : 1;    // Privileged mode
        uint32          : 11;
    } bit;
    uint32 val;
};

union DMA_CNDTRX {
    struct {
        uint32 NDT  : 18;   // Number of data to transfer (0 to 218 - 1)
        uint32      : 14;
    } bit;
    uint32 val;
};

union DMA_CPARX {
    struct {
        uint32 PA   : 32;   // Peripheral address
    } bit;
    uint32 val;
};

union DMA_CMARX {
    struct {
        uint32 MA   : 32;   // Memory address
    } bit;
    uint32 val;
};


union DMA_CCR {
    struct {
        uint32 EN       : 1;    // Channel enable
        uint32 TCIE     : 1;    // Transfer complete interrupt enable
        uint32 HTIE     : 1;    // Half transfer interrupt enable
        uint32 TEIE     : 1;    // Transfer error interrupt enable
        uint32 DIR      : 1;    // Data transfer direction
        uint32 CIRC     : 1;    // Circular mode
        uint32 PINC     : 1;    // Peripheral increment mode
        uint32 MINC     : 1;    // Memory increment mode
        uint32 PSIZE    : 2;    // Peripheral size
        uint32 MSIZE    : 2;    // Memory size
        uint32 PL       : 2;    // Priority level
        uint32 MEM2MEM  : 1;    // Memory-to-memory mode
        uint32          : 5;
        uint32 PRIV     : 1;    // Privileged mode
        uint32          : 11;
    } bit;
    uint32 val;
};
union DMA_CNDTR {
    struct {
        uint32 NDT  : 18;   // Number of data to transfer
        uint32      : 14;
    } bit;
    uint32 val;
};
union DMA_CPAR {
    struct {
        uint32 PA   : 32;   // Peripheral address
    } bit;
    uint32 val;
};
union DMA_CMAR {
    struct {
        uint32 MA   : 32;   // Memory address
    } bit;
    uint32 val;
};

struct REG_DMA_CHANNEL
{
    DMA_CCR     CCR;
    DMA_CNDTR   CNDTR;
    DMA_CPAR    CPAR;
    DMA_CMAR    CMAR;
    uint32      RESERVED;
};

struct REG_DMA
{
    DMA_ISR         ISR;    // Address offset: 0x000
    DMA_IFCR        IFCR;   // Address offset: 0x004
    REG_DMA_CHANNEL CH[7];  // Address offset: 0x008, channel stride: 0x014
};

#define REG_DMA1 ((volatile REG_DMA*)0x40020000u)
#define REG_DMA2 ((volatile REG_DMA*)0x40020400u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
