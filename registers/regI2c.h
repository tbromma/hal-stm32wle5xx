/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: I2C
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                             TYPES & VARIABLES
******************************************************************************************************************************************/

union I2C_CR1 {
    struct {
        uint32 PE           : 1;    // Parity error
        uint32 TXIE         : 1;    // TX interrupt enable
        uint32 RXIE         : 1;    // RX interrupt enable
        uint32 ADDRIE       : 1;    // Address match interrupt enable
        uint32 NACKIE       : 1;    // NACK received interrupt enable
        uint32 STOPIE       : 1;    // STOP detection interrupt enable
        uint32 TCIE         : 1;    // Transfer complete interrupt enable
        uint32 ERRIE        : 1;    // Error interrupt enable
        uint32 DNF          : 4;    // Digital noise filter
        uint32 ANFOFF       : 1;    // Analog noise filter off
        uint32              : 1;
        uint32 TXDMAEN      : 1;    // TX DMA request enable
        uint32 RXDMAEN      : 1;    // RX DMA request enable
        uint32 SBC          : 1;    // Slave byte control
        uint32 NOSTRETCH    : 1;    // Clock stretching disable
        uint32 WUPEN        : 1;    // Wake-up from Stop mode enable
        uint32 GCEN         : 1;    // General call enable
        uint32 SMBHEN       : 1;    // SMBus host address enable
        uint32 SMBDEN       : 1;    // SMBus device default address enable
        uint32 ALERTEN      : 1;    // SMBus alert enable
        uint32 PECEN        : 1;    // Packet error checking enable
        uint32              : 8;
    } bit;
    uint32 val;
};

union I2C_CR2 {
    struct {
        uint32 SADD         : 10;       // Slave address
        uint32 RDWRN        : 1;        // Transfer direction
        uint32 ADD10        : 1;        // 10-bit addressing mode
        uint32 HEAD10R      : 1;        // 10-bit address header only read direction
        uint32 START        : 1;        // Start generation
        uint32 STOP         : 1;        // Stop generation
        uint32 NACK         : 1;        // Smartcard NACK enable
        uint32 NBYTES       : 8;        // Number of bytes
        uint32 RELOAD       : 1;        // Reload mode
        uint32 AUTOEND      : 1;        // Automatic end mode
        uint32 PECBYTE      : 1;        // Packet error checking byte
        uint32              : 5;
    } bit;
    uint32 val;
};

union I2C_OAR1 {
    struct {
        uint32 OA1          : 10;       // Own address 1
        uint32 OA1MODE      : 1;        // Own address 1 mode
        uint32              : 4;
        uint32 OA1EN        : 1;        // Own address 1 enable
        uint32              : 16;
    } bit;
    uint32 val;
};

union I2C_OAR2 {
    struct {
        uint32              : 1;
        uint32 OA2          : 7;        // Own address 2
        uint32 OA2MSK       : 3;        // Own address 2 masks
        uint32              : 4;
        uint32 OA2EN        : 1;        // Own address 2 enable
        uint32              : 16;
    } bit;
    uint32 val;
};

union I2C_TIMINGR {
    struct {
        uint32 SCLL         : 8;    // SCL low period
        uint32 SCLH         : 8;    // SCL high period
        uint32 SDADEL       : 4;    // Data hold time
        uint32 SCLDEL       : 4;    // Data setup time
        uint32              : 4;
        uint32 PRESC        : 4;    // Timing prescaler
    } bit;
    uint32 val;
};

union I2C_TIMEOUTR {
    struct {
        uint32 TIMEOUTA     : 12;       // Bus timeout A
        uint32 TIDLE        : 1;        // Idle clock timeout detection
        uint32              : 2;
        uint32 TIMOUTEN     : 1;        // Clock timeout enable
        uint32 TIMEOUTB     : 12;       // Bus timeout B
        uint32              : 3;
        uint32 TEXTEN       : 1;        // Extended clock timeout enable
    } bit;
    uint32 val;
};

union I2C_ISR {
    struct {
        uint32 TXE          : 1;    // Transmit data register empty
        uint32 TXIS         : 1;    // Transmit interrupt status
        uint32 RXNE         : 1;    // Receive buffer not empty
        uint32 ADDR         : 1;    // Address matched flag
        uint32 NACKF        : 1;    // NACK received flag
        uint32 STOPF        : 1;    // STOP detection flag
        uint32 TC           : 1;    // Transfer complete flag
        uint32 TCR          : 1;    // Transfer complete reload flag
        uint32 BERR         : 1;    // Bus error flag
        uint32 ARLO         : 1;    // Arbitration lost flag
        uint32 OVR          : 1;    // Overrun/underrun flag
        uint32 PECERR       : 1;    // PEC error flag
        uint32 TIMEOUT      : 1;    // Timeout flag
        uint32 ALERT        : 1;    // SMBus alert flag
        uint32              : 1;
        uint32 BUSY         : 1;    // Bus busy
        uint32 DIR          : 1;    // Transfer direction
        uint32 ADDCODE      : 7;    // Address match code
        uint32              : 8;
    } bit;
    uint32 val;
};

union I2C_ICR {
    struct {
        uint32              : 3;
        uint32 ADDRCF       : 1;        // Clear address matched flag
        uint32 NACKCF       : 1;        // Clear NACK flag
        uint32 STOPCF       : 1;        // Clear STOP flag
        uint32              : 2;
        uint32 BERRCF       : 1;        // Clear bus error flag
        uint32 ARLOCF       : 1;        // Clear arbitration lost flag
        uint32 OVRCF        : 1;        // Clear overrun/underrun flag
        uint32 PECCF        : 1;        // Clear PEC error flag
        uint32 TIMOUTCF     : 1;        // Clear timeout flag
        uint32 ALERTCF      : 1;        // Clear SMBus alert flag
        uint32              : 18;
    } bit;
    uint32 val;
};

union I2C_PECR {
    struct {
        uint32 PEC      : 8;        // Packet error checking register
        uint32          : 24;
    } bit;
    uint32 val;
};

union I2C_RXDR {
    struct {
        uint32 RXDATA   : 8;        // Receive data
        uint32          : 24;
    } bit;
    uint32 val;
};

union I2C_TXDR {
    struct {
        uint32 TXDATA       : 8;        // Transmit data
        uint32              : 24;
    } bit;
    uint32 val;
};

struct REG_I2C
{
    I2C_CR1         CR1;            // Control register 1             Address offset: 0x00
    I2C_CR2         CR2;            // Control register 2             Address offset: 0x04
    I2C_OAR1        OAR1;           // Own address 1 register         Address offset: 0x08
    I2C_OAR2        OAR2;           // Own address 2 register         Address offset: 0x0C
    I2C_TIMINGR     TIMINGR;        // Timing register                Address offset: 0x10
    I2C_TIMEOUTR    TIMEOUTR;       // Timeout register               Address offset: 0x14
    I2C_ISR         ISR;            // Interrupt and status register  Address offset: 0x18
    I2C_ICR         ICR;            // Interrupt clear register       Address offset: 0x1C
    I2C_PECR        PECR;           // PEC register                   Address offset: 0x20
    I2C_RXDR        RXDR;           // Receive data register          Address offset: 0x24
    I2C_TXDR        TXDR;           // Transmit data register         Address offset: 0x28
};

#define REG_I2C1 ((volatile REG_I2C*)0x40005400u)
#define REG_I2C2 ((volatile REG_I2C*)0x40005800u)
#define REG_I2C3 ((volatile REG_I2C*)0x40005C00u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
