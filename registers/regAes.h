/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: AES
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union AES_CR {
    struct {
        uint32 EN       : 1;    // AES enable
        uint32 DATATYPE : 2;    // Data type selection
        uint32 MODE     : 2;    // AES operating mode
        uint32          : 2;
        uint32 CCFC     : 1;    // Computation complete flag clear
        uint32 ERRC     : 1;    // Error flag clear
        uint32 CCFIE    : 1;    // CCF interrupt enable
        uint32 ERRIE    : 1;    // Error interrupt enable
        uint32 DMAINEN  : 1;    // DMA input enable
        uint32 DMAOUTEN : 1;    // DMA output enable
        uint32 GCMPH    : 2;    // GCM or CCM phase selection
        uint32          : 3;
        uint32 KEYSIZE  : 1;    // Key size selection
        uint32          : 1;
        uint32 NPBLB    : 4;    // Number of padding bytes in last block
        uint32          : 8;
    } bit;
    uint32 val;
};

union AES_SR {
    struct {
        uint32 CCF      : 1;    // Computation completed flag
        uint32 RDERR    : 1;    // Read error flag
        uint32 WRERR    : 1;    // Write error
        uint32 BUSY     : 1;    // Busy
        uint32          : 28;
    } bit;
    uint32 val;
};

union AES_SUSPXR {
    struct {
        uint32 SUSP : 32;   // AES suspend
    } bit;
    uint32 val;
};

union AES_DINR {
    struct {
        uint32 DIN  : 32;   // Input data word
    } bit;
    uint32 val;
};

union AES_DOUTR {
    struct {
        uint32 DOUT : 32;   // Output data word
    } bit;
    uint32 val;
};

union AES_KEYR0 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [31:0]
    } bit;
    uint32 val;
};

union AES_KEYR1 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [63:32]
    } bit;
    uint32 val;
};

union AES_KEYR2 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [95:64]
    } bit;
    uint32 val;
};

union AES_KEYR3 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [127:96]
    } bit;
    uint32 val;
};

union AES_IVR0 {
    struct {
        uint32 IVI  : 32;   // Initialization vector input, bits [31:0]
    } bit;
    uint32 val;
};

union AES_IVR1 {
    struct {
        uint32 IVI  : 32;   // Initialization vector input, bits [63:32]
    } bit;
    uint32 val;
};

union AES_IVR2 {
    struct {
        uint32 IVI  : 32;   // Initialization vector input, bits [95:64]
    } bit;
    uint32 val;
};

union AES_IVR3 {
    struct {
        uint32 IVI  : 32;   // Initialization vector input, bits [127:96]
    } bit;
    uint32 val;
};

union AES_KEYR4 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [159:128]
    } bit;
    uint32 val;
};

union AES_KEYR5 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [191:160]
    } bit;
    uint32 val;
};

union AES_KEYR6 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [223:192]
    } bit;
    uint32 val;
};

union AES_KEYR7 {
    struct {
        uint32 KEY  : 32;   // Cryptographic key, bits [255:224]
    } bit;
    uint32 val;
};

struct REG_AES
{
    AES_CR      CR;     // Address offset: 0x000
    AES_SR      SR;     // Address offset: 0x004
    AES_DINR    DINR;   // Address offset: 0x008
    AES_DOUTR   DOUTR;  // Address offset: 0x00C
    AES_KEYR0   KEYR0;  // Address offset: 0x010
    AES_KEYR1   KEYR1;  // Address offset: 0x014
    AES_KEYR2   KEYR2;  // Address offset: 0x018
    AES_KEYR3   KEYR3;  // Address offset: 0x01C
    AES_IVR0    IVR0;   // Address offset: 0x020
    AES_IVR1    IVR1;   // Address offset: 0x024
    AES_IVR2    IVR2;   // Address offset: 0x028
    AES_IVR3    IVR3;   // Address offset: 0x02C
    AES_KEYR4   KEYR4;  // Address offset: 0x030
    AES_KEYR5   KEYR5;  // Address offset: 0x034
    AES_KEYR6   KEYR6;  // Address offset: 0x038
    AES_KEYR7   KEYR7;  // Address offset: 0x03C
};

#define REG_AES ((volatile REG_AES*)0x58001800u)

/******************************************************************************************************************************************
                                                        END OF FILE
******************************************************************************************************************************************/
