/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: SPI
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                              TYPES & VARIABLES
******************************************************************************************************************************************/

union SPI_CR1 {
    struct {
        uint32 CPHA     : 1;    // Clock phase
        uint32 CPOL     : 1;    // Clock polarity
        uint32 MSTR     : 1;    // Master selection
        uint32 BR       : 3;    // Baud rate control
        uint32 SPE      : 1;    // SPI enable
        uint32 LSBFIRST : 1;    // Frame format
        uint32 SSI      : 1;    // Internal slave select
        uint32 SSM      : 1;    // Software slave management
        uint32 RXONLY   : 1;    // Receive only mode
        uint32 CRCL     : 1;    // CRC length
        uint32 CRCNEXT  : 1;    // CRC transfer next
        uint32 CRCEN    : 1;    // Hardware CRC calculation enable
        uint32 BIDIOE   : 1;    // Output enable in bidirectional mode
        uint32 BIDIMODE : 1;    // Bidirectional data mode enable
    } bit;
    uint32 val;
};

union SPI_CR2 {
    struct {
        uint32 RXDMAEN  : 1;    // RX DMA request enable
        uint32 TXDMAEN  : 1;    // TX DMA request enable
        uint32 SSOE     : 1;    // SS output enable
        uint32 NSSP     : 1;    // NSS pulse management
        uint32 FRF      : 1;    // Frame format
        uint32 ERRIE    : 1;    // Error interrupt enable
        uint32 RXNEIE   : 1;    // RX buffer not empty interrupt enable
        uint32 TXEIE    : 1;    // TX buffer empty interrupt enable
        uint32 DS       : 4;    // Data size
        uint32 FRXTH    : 1;    // FIFO reception threshold
        uint32 LDMA_RX  : 1;    // Last DMA transfer for reception
        uint32 LDMA_TX  : 1;    // Last DMA transfer for transmission
        uint32          : 1;
    } bit;
    uint32 val;
};

union SPI_SR {
    struct {
        uint32 RXNE     : 1;    // Receive buffer not empty
        uint32 TXE      : 1;    // Transmit data register empty
        uint32 CHSIDE   : 1;    // Channel side
        uint32 UDR      : 1;    // Underrun flag
        uint32 CRCERR   : 1;    // CRC error flag
        uint32 MODF     : 1;    // Mode fault
        uint32 OVR      : 1;    // Overrun/underrun flag
        uint32 BSY      : 1;    // Busy flag
        uint32 FRE      : 1;    // Frame format error
        uint32 FRLVL    : 2;    // FIFO reception level
        uint32 FTLVL    : 2;    // FIFO transmission level
        uint32          : 3;
    } bit;
    uint32 val;
};

union SPI_DR {
    struct {
        uint32 DR   : 16;   // Data register
    } bit;
    uint8 val8;
    uint16 val16;
};

union SPI_CRCPR {
    struct {
        uint32 CRCPOLY  : 16;   // CRC polynomial
    } bit;
    uint32 val;
};

union SPI_RXCRCR {
    struct {
        uint32 RXCRC    : 16;   // RX CRC register
    } bit;
    uint32 val;
};

union SPI_TXCRCR {
    struct {
        uint32 TXCRC    : 16;   // TX CRC register
    } bit;
    uint32 val;
};

struct REG_SPI
{
    SPI_CR1     CR1;        // SPI Control register 1 (not used in I2S mode)        Address offset: 0x00
    SPI_CR2     CR2;        // SPI Control register 2                               Address offset: 0x04
    SPI_SR      SR;         // SPI Status register                                  Address offset: 0x08
    SPI_DR      DR;         // SPI data register                                    Address offset: 0x0C
    SPI_CRCPR   CRCPR;      // SPI CRC polynomial register (not used in I2S mode)   Address offset: 0x10
    SPI_RXCRCR  RXCRCR;     // SPI Rx CRC register (not used in I2S mode)           Address offset: 0x14
    SPI_TXCRCR  TXCRCR;     // SPI Tx CRC register (not used in I2S mode)           Address offset: 0x18
    uint32      I2SCFGR;    // SPI_I2S configuration register                       Address offset: 0x1C
    uint32      I2SPR;      // SPI_I2S prescaler register                           Address offset: 0x20
};

#define REG_SPI1 ((volatile REG_SPI*)0x40013000u)
#define REG_SPI2 ((volatile REG_SPI*)0x40003800u)
#define REG_SPI3_RF ((volatile REG_SPI*)0x58010000u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
