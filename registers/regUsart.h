/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: USART / LPUART
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                              TYPES & VARIABLES
******************************************************************************************************************************************/

union USART_CR1 {
    struct {
        uint32 UE       : 1;    // USART enable
        uint32 UESM     : 1;    // USART enable in low-power mode
        uint32 RE       : 1;    // Receiver enable
        uint32 TE       : 1;    // Transmitter enable
        uint32 IDLEIE   : 1;    // IDLE interrupt enable
        uint32 RXFNEIE  : 1;    // RXFIFO not empty interrupt enable
        uint32 TCIE     : 1;    // Transmission complete interrupt enable
        uint32 TXFNFIE  : 1;    // TXFIFO not-full interrupt enable
        uint32 PEIE     : 1;    // PE interrupt enable
        uint32 PS       : 1;    // Parity selection
        uint32 PCE      : 1;    // Parity control enable
        uint32 WAKE     : 1;    // Receiver wake-up method
        uint32 M0       : 1;    // Word length
        uint32 MME      : 1;    // Mute mode enable
        uint32 CMIE     : 1;    // Character match interrupt enable
        uint32 OVER8    : 1;    // Oversampling mode
        uint32 DEDT     : 5;    // Driver enable deassertion time
        uint32 DEAT     : 5;    // Driver enable assertion time
        uint32 RTOIE    : 1;    // Receiver timeout interrupt enable
        uint32 EOBIE    : 1;    // End-of-block interrupt enable
        uint32 M1       : 1;    // Word length
        uint32 FIFOEN   : 1;    // FIFO mode enable
        uint32 TXFEIE   : 1;    // TXFIFO empty interrupt enable
        uint32 RXFFIE   : 1;    // RXFIFO full interrupt enable

    } bit;
    uint32 val;
};

union USART_CR2 {
    struct {
        uint32 SLVEN    : 1;    // Synchronous Slave mode enable
        uint32          : 2;
        uint32 DIS_NSS  : 1;    // NSS pin enable
        uint32 ADDM7    : 1;    // 7-bit address detection/4-bit address detection
        uint32 LBDL     : 1;    // LIN break detection length
        uint32 LBDIE    : 1;    // LIN break detection interrupt enable
        uint32          : 1;
        uint32 LBCL     : 1;    // Last bit clock pulse
        uint32 CPHA     : 1;    // Clock phase
        uint32 CPOL     : 1;    // Clock polarity
        uint32 CLKEN    : 1;    // Clock enable
        uint32 STOP     : 2;    // Stop bits
        uint32 LINEN    : 1;    // LIN mode enable
        uint32 SWAP     : 1;    // Swap TX/RX pins
        uint32 RXINV    : 1;    // RX pin active level inversion
        uint32 TXINV    : 1;    // TX pin active level inversion
        uint32 DATAINV  : 1;    // Binary data inversion
        uint32 MSBFIRST : 1;    // Most significant bit first
        uint32 ABREN    : 1;    // Auto baud rate enable
        uint32 ABRMOD   : 2;    // Auto baud rate mode
        uint32 RTOEN    : 1;    // Receiver timeout enable
        uint32 ADD      : 8;    // Address of the USART node
    } bit;
    uint32 val;
};

union USART_CR3 {
    struct {
        uint32 EIE      : 1;    // Error interrupt enable
        uint32 IREN     : 1;    // IrDA mode enable
        uint32 IRLP     : 1;    // IrDA low-power
        uint32 HDSEL    : 1;    // Half-duplex selection
        uint32 NACK     : 1;    // Smartcard NACK enable
        uint32 SCEN     : 1;    // Smartcard mode enable
        uint32 DMAR     : 1;    // DMA enable receiver
        uint32 DMAT     : 1;    // DMA enable transmitter
        uint32 RTSE     : 1;    // RTS enable
        uint32 CTSE     : 1;    // CTS enable
        uint32 CTSIE    : 1;    // CTS interrupt enable
        uint32 ONEBIT   : 1;    // One sample bit method enable
        uint32 OVRDIS   : 1;    // Overrun disable
        uint32 DDRE     : 1;    // DMA Disable on reception error
        uint32 DEM      : 1;    // Driver enable mode
        uint32 DEP      : 1;    // Driver enable polarity selection
        uint32          : 1;
        uint32 SCARCNT  : 3;    // Smartcard auto-retry count
        uint32 WUS      : 2;    // Wake-up from low-power mode interrupt flag selection
        uint32 WUFIE    : 1;    // Wake-up from low-power mode interrupt enable
        uint32 TXFTIE   : 1;    // TXFIFO threshold interrupt enable
        uint32 TCBGTIE  : 1;    // Transmission complete before guard time, interrupt enable
        uint32 RXFTCFG  : 3;    // Receive FIFO threshold configuration
        uint32 RXFTIE   : 1;    // RXFIFO threshold interrupt enable
        uint32 TXFTCFG  : 3;    // TXFIFO threshold configuration
    } bit;
    uint32 val;
};

union USART_BRR {
    struct {
        uint32 BRR  : 16;   // USART baud rate
        uint32      : 16;
    } bit;
    uint32 val;
};

union USART_GTPR {
    struct {
        uint32 PSC  : 8;    // Prescaler value
        uint32 GT   : 8;    // Guard time value
        uint32      : 16;
    } bit;
    uint32 val;
};

union USART_RTOR {
    struct {
        uint32 RTO  : 24;   // Receiver timeout value
        uint32 BLEN : 8;    // Block length
    } bit;
    uint32 val;
};

union USART_RQR {
    struct {
        uint32 ABRRQ    : 1;    // Auto baud rate request
        uint32 SBKRQ    : 1;    // Send break request
        uint32 MMRQ     : 1;    // Mute mode request
        uint32 RXFRQ    : 1;    // Receive data flush request
        uint32 TXFRQ    : 1;    // Transmit data flush request
        uint32          : 27;
    } bit;
    uint32 val;
};

union USART_ISR {
    struct {
        uint32 PE       : 1;    // Parity error
        uint32 FE       : 1;    // Framing error
        uint32 NE       : 1;    // Noise detection flag
        uint32 ORE      : 1;    // Overrun error
        uint32 IDLE     : 1;    // Idle line detected
        uint32 RXFNE    : 1;    // RXFIFO not empty
        uint32 TC       : 1;    // Transmission complete
        uint32 TXFNF    : 1;    // TXFIFO not full
        uint32 LBDF     : 1;    // LIN break detection flag
        uint32 CTS      : 1;    // CTS flag
        uint32 CTSIF    : 1;    // CTS interrupt flag
        uint32 RTOF     : 1;    // Receiver timeout
        uint32 EOBF     : 1;    // End of block flag
        uint32 UDR      : 1;    // SPI slave underrun error flag
        uint32 ABRE     : 1;    // Auto baud rate error
        uint32 ABRF     : 1;    // Auto baud rate flag
        uint32 BUSY     : 1;    // Busy flag
        uint32 CMF      : 1;    // Character match flag
        uint32 SBKF     : 1;    // Send break flag
        uint32 RWU      : 1;    // Receiver wake-up from Mute mode
        uint32 WUF      : 1;    // Wake-up from low-power mode flag
        uint32 TEACK    : 1;    // Transmit enable acknowledge flag
        uint32 REACK    : 1;    // Receive enable acknowledge flag
        uint32 TXFE     : 1;    // TXFIFO empty
        uint32 RXFF     : 1;    // RXFIFO full
        uint32 TCBGT    : 1;    // Transmission complete before guard time flag
        uint32 RXFT     : 1;    // RXFIFO threshold flag
        uint32 TXFT     : 1;    // TXFIFO threshold flag
        uint32          : 4;
    } bit;
    uint32 val;
};

union USART_ICR {
    struct {
        uint32 PECF     : 1;    // Parity error clear flag
        uint32 FECF     : 1;    // Framing error clear flag
        uint32 NECF     : 1;    // Noise detected clear flag
        uint32 ORECF    : 1;    // Overrun error clear flag
        uint32 IDLECF   : 1;    // Idle line detected clear flag
        uint32 TXFECF   : 1;    // TXFIFO empty clear flag
        uint32 TCCF     : 1;    // Transmission complete clear flag
        uint32 TCBGTCF  : 1;    // Transmission complete before Guard time clear flag
        uint32 LBDCF    : 1;    // LIN break detection clear flag
        uint32 CTSCF    : 1;    // CTS clear flag
        uint32          : 1;
        uint32 RTOCF    : 1;    // Receiver timeout clear flag
        uint32 EOBCF    : 1;    // End of block clear flag
        uint32 UDRCF    : 1;    // SPI slave underrun clear flag
        uint32          : 3;
        uint32 CMCF     : 1;    // Character match clear flag
        uint32          : 2;
        uint32 WUCF     : 1;    // Wake-up from low-power mode clear flag
        uint32          : 11;
    } bit;
    uint32 val;
};

union USART_RDR {
    struct {
        uint32 RDR  : 8;    // Receive data value
        uint32      : 24;
    } bit;
    uint32 val;
};

union USART_TDR {
    struct {
        uint32 TDR  : 8;    // Transmit data value
        uint32      : 24;
    } bit;
    uint32 val;
};

union USART_PRESC {
    struct {
        uint32 PRESCALER    : 4;    // Transmit data value
        uint32              : 28;
    } bit;
    uint32 val;
};

struct REG_USART
{
    USART_CR1   CR1;    // USART Control register 1                     Address offset: 0x00
    USART_CR2   CR2;    // USART Control register 2                     Address offset: 0x04
    USART_CR3   CR3;    // USART Control register 3                     Address offset: 0x08
    USART_BRR   BRR;    // USART Baud rate register                     Address offset: 0x0C
    USART_GTPR  GTPR;   // USART Guard time and prescaler register      Address offset: 0x10
    USART_RTOR  RTOR;   // USART Receiver Time Out register             Address offset: 0x14
    USART_RQR   RQR;    // USART Request register                       Address offset: 0x18
    USART_ISR   ISR;    // USART Interrupt and status register          Address offset: 0x1C
    USART_ICR   ICR;    // USART Interrupt flag Clear register          Address offset: 0x20
    USART_RDR   RDR;    // USART Receive Data register                  Address offset: 0x24
    USART_TDR   TDR;    // USART Transmit Data register                 Address offset: 0x28
    USART_PRESC PRESC;  // USART Prescaler register                     Address offset: 0x2C
};

#define REG_USART1 ((volatile REG_USART*)0x40013800u)
#define REG_LPUART1 ((volatile REG_USART*)0x40008000u)
#define REG_USART2 ((volatile REG_USART*)0x40004400u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
