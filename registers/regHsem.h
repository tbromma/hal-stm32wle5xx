/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: HSEM
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

union HSEM_RX {
    struct {
        uint32 PROCID   : 8;    // Semaphore PROCID
        uint32 MASTERID : 4;    // Semaphore MASTERID
        uint32          : 19;
        uint32 LOCK     : 1;    // Lock indication
    } bit;
    uint32 val;
};

union HSEM_RLRX {
    struct {
        uint32 PROCID   : 8;    // Semaphore processor ID
        uint32 MASTERID : 4;    // Semaphore MASTERID
        uint32          : 19;
        uint32 LOCK     : 1;    // Lock indication
    } bit;
    uint32 val;
};

union HSEM_IER {
    struct {
        uint32 ISE  : 16;   // Interrupt semaphore x enable bit (x = 0 to 15)
        uint32      : 16;
    } bit;
    uint32 val;
};

union HSEM_ICR {
    struct {
        uint32 ISC  : 16;   // Interrupt semaphore x clear bit (x = 0 to 15)
        uint32      : 16;
    } bit;
    uint32 val;
};

union HSEM_ISR {
    struct {
        uint32 ISF  : 16;   // Interrupt semaphore x status bit before enable (mask) (x = 0 to 15)
        uint32      : 16;
    } bit;
    uint32 val;
};

union HSEM_MISR {
    struct {
        uint32 MISF : 16;   // Masked interrupt semaphore x status bit after enable (mask) (x = 0 to 15)
        uint32      : 16;
    } bit;
    uint32 val;
};

union HSEM_CR {
    struct {
        uint32          : 8;
        uint32 MASTERID : 4;    // MASTERID of semaphores to be cleared
        uint32          : 4;
        uint32 KEY      : 16;   // Semaphore clear key
    } bit;
    uint32 val;
};

union HSEM_KEYR {
    struct {
        uint32 OTY  : 16;   // Port Pxy output type configuration (y = 15 to 0)
        uint32 KEY  : 16;   // Semaphore clear key
    } bit;
    uint32 val;
};

struct REG_HSEM
{
    uint32      RESERVED0[1];   // 0x000 - 0x000
    HSEM_RX     RX;             // Address offset: 0x004
    uint32      RESERVED1[62];  // 0x008 - 0x0FC
    HSEM_IER    IER;            // Address offset: 0x100
    HSEM_ICR    ICR;            // Address offset: 0x104
    HSEM_ISR    ISR;            // Address offset: 0x108
    HSEM_MISR   MISR;           // Address offset: 0x10C
    uint32      RESERVED2[12];  // 0x110 - 0x13C
    HSEM_CR     CR;             // Address offset: 0x140
    HSEM_KEYR   KEYR;           // Address offset: 0x144
};

#define REG_HSEM ((volatile REG_HSEM*)0x58001400u)

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
