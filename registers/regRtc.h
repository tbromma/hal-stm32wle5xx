/******************************************************************************************************************************************
 *
 * Register definitions for STM32WLE5xx
 * Peripheral: RTC
 *
 ******************************************************************************************************************************************/

#pragma once
#include "common.h"

/******************************************************************************************************************************************
                                                              TYPES & VARIABLES
******************************************************************************************************************************************/

union RTC_TR {
    struct {
        uint32 SU   : 4;    // SU field
        uint32 ST   : 3;    // ST field
        uint32      : 1;
        uint32 MNU  : 4;    // MNU field
        uint32 MNT  : 3;    // MNT field
        uint32      : 1;
        uint32 HU   : 4;    // HU field
        uint32 HT   : 2;    // HT field
        uint32 PM   : 1;    // PM field
        uint32      : 9;
    } bit;
    uint32 val;
};

union RTC_DR {
    struct {
        uint32 DU   : 4;    // DU field
        uint32 DT   : 2;    // DT field
        uint32      : 2;
        uint32 MU   : 4;    // MU field
        uint32 MT   : 1;    // MT field
        uint32 WDU  : 3;    // WDU field
        uint32 YU   : 4;    // YU field
        uint32 YT   : 4;    // YT field
        uint32      : 8;

    } bit;
    uint32 val;
};

union RTC_SSR {
    struct {
        uint32 SSL  : 16;   // SSL field
        uint32 SSH  : 16;   // SSH field
    } bit;
    uint32 val;
};

union RTC_ICSR {
    struct {
        uint32          : 2;
        uint32 WUTWF    : 1;    // WUTWF field
        uint32 SHPF     : 1;    // SHPF field
        uint32 INITS    : 1;    // INITS field
        uint32 RSF      : 1;    // RSF field
        uint32 INITF    : 1;    // INITF field
        uint32 INIT     : 1;    // INIT field
        uint32 BIN      : 2;    // BIN field
        uint32 BCDU     : 3;    // BCDU field
        uint32          : 3;
        uint32 RECALPF  : 1;    // RECALPF field
        uint32          : 15;
    } bit;
    uint32 val;
};

union RTC_PRER {
    struct {
        uint32 PREDIVS  : 16;   // PREDIVS field
        uint32 PREDIVA  : 7;    // PREDIVA field
        uint32          : 9;
    } bit;
    uint32 val;
};

union RTC_CR {
    struct {
        uint32 WUCKSEL          : 3;    // WUCKSEL field
        uint32 TSEDGE           : 1;    // TSEDGE field
        uint32 REFCKON          : 1;    // REFCKON field
        uint32 BYPSHAD          : 1;    // BYPSHAD field
        uint32 FMT              : 1;    // FMT field
        uint32 SSRUIE           : 1;    // SSRUIE field
        uint32 ALRAE            : 1;    // ALRAE field
        uint32 ALRBE            : 1;    // ALRBE field
        uint32 WUTE             : 1;    // WUTE field
        uint32 TSE              : 1;    // TSE field
        uint32 ALRAIE           : 1;    // ALRAIE field
        uint32 ALRBIE           : 1;    // ALRBIE field
        uint32 WUTIE            : 1;    // WUTIE field
        uint32 TSIE             : 1;    // TSIE field
        uint32 ADD1H            : 1;    // ADD1H field
        uint32 SUB1H            : 1;    // SUB1H field
        uint32 BKP              : 1;    // BKP field
        uint32 COSEL            : 1;    // COSEL field
        uint32 POL              : 1;    // POL field
        uint32 OSEL             : 2;    // OSEL field
        uint32 COE              : 1;    // COE field
        uint32 ITSE             : 1;    // ITSE field
        uint32 TAMPTS           : 1;    // TAMPTS field
        uint32 TAMPOE           : 1;    // TAMPOE field
        uint32                  : 2;
        uint32 TAMPALRM_PU      : 1;    // TAMPALRM PU field
        uint32 TAMPALRM_TYPE    : 1;    // TAMPALRM TYPE field
        uint32 OUT2EN           : 1;    // OUT2EN field
    } bit;
    uint32 val;
};

union RTC_WUTR {
    struct {
        uint32 WUT      : 16;   // WUT field
        uint32 WUTOCLR  : 16;   // WUTOCLR field
    } bit;
    uint32 val;
};

union RTC_WPR {
    struct {
        uint32 KEY  : 8;    // KEY field
        uint32      : 24;
    } bit;
    uint32 val;
};

union RTC_CALR {
    struct {
        uint32 CALM     : 9;    // CALM field
        uint32          : 3;
        uint32 LPCAL    : 1;    // LPCAL field
        uint32 CALW16   : 1;    // CALW16 field
        uint32 CALW8    : 1;    // CALW8 field
        uint32 CALP     : 1;    // CALP field
        uint32          : 16;
    } bit;
    uint32 val;
};

union RTC_SHIFTR {
    struct {
        uint32 SUBFS    : 15;   // SUBFS field
        uint32          : 16;
        uint32 ADD1S    : 1;    // ADD1S field
    } bit;
    uint32 val;
};

union RTC_TSTR {
    struct {
        uint32 SU   : 4;    // SU field
        uint32 ST   : 3;    // ST field
        uint32      : 1;
        uint32 MNU  : 4;    // MNU field
        uint32 MNT  : 3;    // MNT field
        uint32      : 1;
        uint32 HU   : 4;    // HU field
        uint32 HT   : 2;    // HT field
        uint32 PM   : 1;    // PM field
        uint32      : 9;
    } bit;
    uint32 val;
};

union RTC_TSDR {
    struct {
        uint32 DU   : 4;    // DU field
        uint32 DT   : 2;    // DT field
        uint32      : 2;
        uint32 MU   : 4;    // MU field
        uint32 MT   : 1;    // MT field
        uint32 WDU  : 3;    // WDU field
        uint32 YU   : 4;    // YU field
        uint32 YT   : 4;    // YT field
        uint32      : 8;

    } bit;
    uint32 val;
};

union RTC_TSSSR {
    struct {
        uint32 SS   : 32;   // SS field
    } bit;
    uint32 val;
};

union RTC_ALRMAR {
    struct {
        uint32 SU       : 4;    // SU field
        uint32 ST       : 3;    // ST field
        uint32 MSK1     : 1;    // MSK1 field
        uint32 MNU      : 4;    // MNU field
        uint32 MNT      : 3;    // MNT field
        uint32 MSK2     : 1;    // MSK2 field
        uint32 HU       : 4;    // HU field
        uint32 HT       : 2;    // HT field
        uint32 PM       : 1;    // PM field
        uint32 MSK3     : 1;    // MSK3 field
        uint32 DU       : 4;    // DU field
        uint32 DT       : 2;    // DT field
        uint32 WDSEL    : 1;    // WDSEL field
        uint32 MSK4     : 1;    // MSK4 field
    } bit;
    uint32 val;
};

union RTC_ALRMASSR {
    struct {
        uint32 SS       : 15;   // SS field
        uint32          : 9;
        uint32 MASKSS   : 6;    // MASKSS field
        uint32          : 1;
        uint32 SSCLR    : 1;    // SSCLR field
    } bit;
    uint32 val;
};

union RTC_ALRMBR {
    struct {
        uint32 SU       : 4;    // SU field
        uint32 ST       : 3;    // ST field
        uint32 MSK1     : 1;    // MSK1 field
        uint32 MNU      : 4;    // MNU field
        uint32 MNT      : 3;    // MNT field
        uint32 MSK2     : 1;    // MSK2 field
        uint32 HU       : 4;    // HU field
        uint32 HT       : 2;    // HT field
        uint32 PM       : 1;    // PM field
        uint32 MSK3     : 1;    // MSK3 field
        uint32 DU       : 4;    // DU field
        uint32 DT       : 2;    // DT field
        uint32 WDSEL    : 1;    // WDSEL field
        uint32 MSK4     : 1;    // MSK4 field
    } bit;
    uint32 val;
};

union RTC_ALRMBSSR {
    struct {
        uint32 SS       : 15;   // SS field
        uint32          : 9;
        uint32 MASKSS   : 6;    // MASKSS field
        uint32          : 1;
        uint32 SSCLR    : 1;    // SSCLR field
    } bit;
    uint32 val;
};

union RTC_SR {
    struct {
        uint32 ALRAF    : 1;    // ALRAF field
        uint32 ALRBF    : 1;    // ALRBF field
        uint32 WUTF     : 1;    // WUTF field
        uint32 TSF      : 1;    // TSF field
        uint32 TSOVF    : 1;    // TSOVF field
        uint32 ITSF     : 1;    // ITSF field
        uint32 SSRUF    : 1;    // SSRUF field
        uint32          : 25;
    } bit;
    uint32 val;
};

union RTC_MISR {
    struct {
        uint32 ALRAMF   : 1;    // ALRAMF field
        uint32 ALRBMF   : 1;    // ALRBMF field
        uint32 WUTMF    : 1;    // WUTMF field
        uint32 TSMF     : 1;    // TSMF field
        uint32 TSOVMF   : 1;    // TSOVMF field
        uint32 ITSMF    : 1;    // ITSMF field
        uint32 SSRUMF   : 1;    // SSRUMF field
        uint32          : 25;
    } bit;
    uint32 val;
};

union RTC_SCR {
    struct {
        uint32 CALRAF   : 1;    // CALRAF field
        uint32 CALRBF   : 1;    // CALRBF field
        uint32 CWUTF    : 1;    // CWUTF field
        uint32 CTSF     : 1;    // CTSF field
        uint32 CTSOVF   : 1;    // CTSOVF field
        uint32 CITSF    : 1;    // CITSF field
        uint32 CSSRUF   : 1;    // CSSRUF field
        uint32          : 25;
    } bit;
    uint32 val;
};

union RTC_ALRABINR {
    struct {
        uint32 SS   : 32;   // SS field
    } bit;
    uint32 val;
};

union RTC_ALRBBINR {
    struct {
        uint32 SS   : 32;   // SS field
    } bit;
    uint32 val;
};

struct REG_RTC
{
    RTC_TR          TR;             // RTC time register                                Address offset: 0x00
    RTC_DR          DR;             // RTC date register                                Address offset: 0x04
    RTC_SSR         SSR;            // RTC sub second register                          Address offset: 0x08
    RTC_ICSR        ICSR;           // RTC initialization control and status register   Address offset: 0x0C
    RTC_PRER        PRER;           // RTC prescaler register                           Address offset: 0x10
    RTC_WUTR        WUTR;           // RTC wakeup timer register                        Address offset: 0x14
    RTC_CR          CR;             // RTC control register                             Address offset: 0x18
    uint32          RESERVED0;      // Reserved                                         Address offset: 0x1C
    uint32          RESERVED1;      // Reserved                                         Address offset: 0x20
    RTC_WPR         WPR;            // RTC write protection register                    Address offset: 0x24
    RTC_CALR        CALR;           // RTC calibration register                         Address offset: 0x28
    RTC_SHIFTR      SHIFTR;         // RTC shift control register                       Address offset: 0x2C
    RTC_TSTR        TSTR;           // RTC time stamp time register                     Address offset: 0x30
    RTC_TSDR        TSDR;           // RTC time stamp date register                     Address offset: 0x34
    RTC_TSSSR       TSSSR;          // RTC time-stamp sub second register               Address offset: 0x38
    uint32          RESERVED2;      // Reserved                                         Address offset: 0x3C
    RTC_ALRMAR      ALRMAR;         // RTC alarm A register                             Address offset: 0x40
    RTC_ALRMASSR    ALRMASSR;       // RTC alarm A sub second register                  Address offset: 0x44
    RTC_ALRMBR      ALRMBR;         // RTC alarm B register                             Address offset: 0x48
    RTC_ALRMBSSR    ALRMBSSR;       // RTC alarm B sub second register                  Address offset: 0x4C
    RTC_SR          SR;             // RTC Status register                              Address offset: 0x50
    RTC_MISR        MISR;           // RTC masked interrupt status register             Address offset: 0x54
    uint32          RESERVED3;      // Reserved                                         Address offset: 0x58
    RTC_SCR         SCR;            // RTC status Clear register                        Address offset: 0x5C
    uint32          RESERVED4[4];   // Reserved                                         Address offset: 0x58
    RTC_ALRABINR    ALRABINR;       // RTC alarm A binary mode register                 Address offset: 0x70
    RTC_ALRBBINR    ALRBBINR;       // RTC alarm B binary mode register                 Address offset: 0x74
};

#define REG_RTC ((volatile REG_RTC*)0x40002800u)

/******************************************************************************************************************************************
                                                                 END OF FILE
******************************************************************************************************************************************/
