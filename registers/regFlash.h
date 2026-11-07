/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: FLASH
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

#define FLASH_KEY1  0x45670123
#define FLASH_KEY2  0xCDEF89AB

union FLASH_ACR {
    struct {
        uint32 LATENCY  : 3;    // Flash memory access latency
        uint32          : 5;
        uint32 PRFTEN   : 1;    // Prefetch enable
        uint32 ICEN     : 1;    // Instruction cache enable
        uint32 DCEN     : 1;    // Data cache enable
        uint32 ICRST    : 1;    // Instruction cache reset
        uint32 DCRST    : 1;    // Data cache reset
        uint32          : 2;
        uint32 PES      : 1;    // Program/erase suspend
        uint32 EMPTY    : 1;    // Flash user area empty
        uint32          : 15;
    } bit;
    uint32 val;
};

union FLASH_KEYR {
    struct {
        uint32 KEYR     : 32;   // Flash key value
    } bit;
    uint32 val;
};

union FLASH_OPTKEYR {
    struct {
        uint32 OPTKEY   : 32;   // Option byte key value
    } bit;
    uint32 val;
};

union FLASH_SR {
    struct {
        uint32 EOP      : 1;    // End of operation flag
        uint32 OPERR    : 1;    // Operation error flag
        uint32          : 1;
        uint32 PROGERR  : 1;    // Programming error flag
        uint32 WRPERR   : 1;    // Write protection error flag
        uint32 PGAERR   : 1;    // Programming alignment error flag
        uint32 SIZERR   : 1;    // Size error flag
        uint32 PGSERR   : 1;    // Programming sequence error flag
        uint32 MISSERR  : 1;    // Fast programming data miss error flag
        uint32 FASTERR  : 1;    // Fast programming error flag
        uint32          : 3;
        uint32 OPTNV    : 1;    // Option validity error flag
        uint32 RDERR    : 1;    // PCROP read error flag
        uint32 OPTVERR  : 1;    // Option validity error flag
        uint32 BSY      : 1;    // Busy flag
        uint32          : 1;
        uint32 CFGBSY   : 1;    // Programming or erase configuration busy flag
        uint32 PESD     : 1;    // Programming or erase operation suspended
        uint32          : 12;
    } bit;
    uint32 val;
};

union FLASH_CR {
    struct {
        uint32 PG           : 1;    // Programming enable
        uint32 PER          : 1;    // Page erase enable
        uint32 MER          : 1;    // Mass erase enable
        uint32 PNB          : 7;    // Page number selection
        uint32              : 6;
        uint32 STRT         : 1;    // Start erase operation
        uint32 OPTSTRT      : 1;    // Start option modification
        uint32 FSTPG        : 1;    // Fast programming enable
        uint32              : 5;
        uint32 EOPIE        : 1;    // End of operation interrupt enable
        uint32 ERRIE        : 1;    // Error interrupt enable
        uint32 RDERRIE      : 1;    // PCROP read error interrupt enable
        uint32 OBL_LAUNCH   : 1;    // Force option byte loading
        uint32              : 2;
        uint32 OPTLOCK      : 1;    // Option lock
        uint32 LOCK         : 1;    // Flash control register lock
    } bit;
    uint32 val;
};

union FLASH_ECCR {
    struct {
        uint32 ADDR_ECC : 1;    // ECC failing address
        uint32          : 3;
        uint32 SYSF_ECC : 1;    // ECC error in system Flash
        uint32          : 3;
        uint32 ECCCIE   : 1;    // ECC correction interrupt enable
        uint32          : 5;
        uint32 ECCC     : 1;    // ECC correction flag
        uint32 ECCD     : 1;    // ECC detection flag
    } bit;
    uint32 val;
};

union FLASH_OPTR {
    struct {
        uint32 RDP          : 8;    // Readout protection level
        uint32 ESE          : 1;    // Security enable
        uint32 BOR_LEV      : 1;    // Brown-out reset level
        uint32 nRST_STOP    : 1;    // Reset generation in Stop mode
        uint32 nRST_STDBY   : 1;    // Reset generation in Standby mode
        uint32 nRST_SHDW    : 1;    // Reset generation in Shutdown mode
        uint32              : 1;
        uint32 IWDG_SW      : 1;    // Independent watchdog mode
        uint32 IWDG_STOP    : 1;    // Independent watchdog counter in Stop mode
        uint32 IWDG_STDBY   : 1;    // Independent watchdog counter in Standby mode
        uint32 WWDG_SW      : 1;    // Window watchdog mode
        uint32              : 3;
        uint32 nBOOT1       : 1;    // Boot configuration bit 1
        uint32 SRAM2_PE     : 1;    // SRAM2 parity check enable
        uint32 SRAM_RST     : 1;    // SRAM2 erase on system reset
        uint32 nSWBOOT0     : 1;    // Software BOOT0 selection
        uint32 nBOOT0       : 1;    // BOOT0 selection
        uint32              : 2;
        uint32 BOOT_LOCK    : 1;    // Boot lock
        uint32              : 1;
    } bit;
    uint32 val;
};

union FLASH_PCROP1ASR {
    struct {
        uint32 PCROP1A_STRT : 8;    // Bank 1 PCROP area A start address
        uint32              : 24;
    } bit;
    uint32 val;
};

union FLASH_PCROP1AER {
    struct {
        uint32 PCROP1A_END  : 8;    // Bank 1 PCROP area A end address
        uint32              : 23;
        uint32 PCROP_RDP    : 1;    // PCROP area preserved when RDP level decreases
    } bit;
    uint32 val;
};

union FLASH_WRP1AR {
    struct {
        uint32 WRP1A_STRT   : 7;    // Bank 1 WRP area A start address
        uint32              : 9;
        uint32 WRP1A_END    : 7;    // Bank 1 WRP area A end address
        uint32              : 9;
    } bit;
    uint32 val;
};

union FLASH_WRP1BR {
    struct {
        uint32 WRP1B_STRT   : 7;    // Bank 1 WRP area B start address
        uint32              : 9;
        uint32 WRP1B_END    : 7;    // Bank 1 WRP area B end address
        uint32              : 9;
    } bit;
    uint32 val;
};

union FLASH_PCROP1BSR {
    struct {
        uint32 PCROP1B_END  : 8;    // Bank 1 PCROP area B end address
        uint32              : 24;
    } bit;
    uint32 val;
};

union FLASH_PCROP1BER {
    struct {
        uint32 PCROP1B_END  : 8;    // Bank 1 PCROP area B end address
        uint32              : 24;
    } bit;
    uint32 val;
};

struct REG_FLASH
{
    FLASH_ACR       ACR;        // FLASH Access control register                       Address offset: 0x00
    uint32          RESERVED0;  // Reserved                                            Address offset: 0x04
    FLASH_KEYR      KEYR;       // FLASH Key register                                  Address offset: 0x08
    FLASH_OPTKEYR   OPTKEYR;    // FLASH Option Key register                           Address offset: 0x0C
    FLASH_SR        SR;         // FLASH Status register                               Address offset: 0x10
    FLASH_CR        CR;         // FLASH Control register                              Address offset: 0x14
    FLASH_ECCR      ECCR;       // FLASH ECC register                                  Address offset: 0x18
    uint32          RESERVED1;  // Reserved                                            Address offset: 0x1C
    FLASH_OPTR      OPTR;       // FLASH Option register                               Address offset: 0x20
    FLASH_PCROP1ASR PCROP1ASR;  // FLASH Bank 1 PCROP area A Start address register    Address offset: 0x24
    FLASH_PCROP1AER PCROP1AER;  // FLASH Bank 1 PCROP area A End address register      Address offset: 0x28
    FLASH_WRP1AR    WRP1AR;     // FLASH Bank 1 WRP area A address register            Address offset: 0x2C
    FLASH_WRP1BR    WRP1BR;     // FLASH Bank 1 WRP area B address register            Address offset: 0x30
    FLASH_PCROP1BSR PCROP1BSR;  // FLASH Bank 1 PCROP area B Start address register    Address offset: 0x34
    FLASH_PCROP1BER PCROP1BER;  // FLASH Bank 1 PCROP area B End address register      Address offset: 0x38
};

#define REG_FLASH ((volatile REG_FLASH*)0x58004000u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
