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

// CREATED BY:       T.Bromma (tobiasz.bromma@thoriumspace.com)
// CREATION DATE:    January 2024
// DESCRIPTION:      Low level layer for FLASH peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "perRng.h"
#include "regRng.h"
#include "halTime.h"

/******************************************************************************************************************************************
                                                           PRIVATE DEFINES & TYPES
******************************************************************************************************************************************/
namespace hal::rng {

#undef CODE_STATUS
#define CODE_STATUS ctx.status

static constexpr uint8  PERCLK_RNGCLK_RATIO = 32;
static constexpr uint32 REG_HTCR_ACCESS_KEY = 0x17590ABC;
static constexpr uint32 REG_HTCR_VALUE = 0x0000AA74;
static constexpr uint8  ENABLE_WAIT_ms = 100;

static struct {
    Status      status;
    RNG_CR      regCr;
    RNG_HTCR    regHtcr;
} ctx = {};

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec Setup(hal::clk::rngClkSrc_t clkSrc)
{
    //-----------------------------------------------------------------------------------
    // Enable RNG clock's

    hal::clk::PerClkEnable(hal::PER_RNG);
    hal::clk::RngClkSrcSelect(clkSrc);
    if (clkSrc == hal::clk::RNG_CLKSRC_LSE) {
        CHECK_EXEC(hal::clk::LseClkSysEnable());
    }

    //-----------------------------------------------------------------------------------
    // Check clock speed ratio. The RNG source clock can be at maximum PERCLK_RNGCLK_RATIO
    // slower than the RNG peripheral clock

    uint32 perClkHz = hal::clk::PerClkGetFreqHz(hal::PER_RNG);
    uint32 rngClkHz = hal::clk::RngClkGetFreqHz();
    ASSERT((perClkHz / rngClkHz) <= PERCLK_RNGCLK_RATIO, "RNG CLOCK SOURCE TOO SLOW");

    //-----------------------------------------------------------------------------------
    // Setup configuration register (register values derived from datasheet)

    ctx.regCr.bit.CONDRST       = 1;
    ctx.regCr.bit.NISTC         = 0;
    ctx.regCr.bit.RNG_CONFIG1   = 0x0F;
    ctx.regCr.bit.CLKDIV        = 0;
    ctx.regCr.bit.RNG_CONFIG2   = 0;
    ctx.regCr.bit.RNG_CONFIG3   = 0x0D;
    ctx.regCr.bit.CED           = 0;
    ctx.regCr.bit.RNGEN         = 0;
    REG_RNG->CR.val = ctx.regCr.val;

    //-----------------------------------------------------------------------------------
    // Set and check RNG healt check register value

    REG_RNG->HTCR.val = REG_HTCR_ACCESS_KEY;
    REG_RNG->HTCR.val = REG_HTCR_VALUE;

    REG_RNG->HTCR.val = REG_HTCR_ACCESS_KEY;
    ctx.regHtcr.val = REG_RNG->HTCR.val;

    ASSERT(ctx.regHtcr.val == REG_HTCR_VALUE, "RNG HTCR REGISTER");

    //-----------------------------------------------------------------------------------
    // Enable RNG

    ctx.regCr.bit.CONDRST = 0;
    ctx.regCr.bit.RNGEN = 1;
    REG_RNG->CR.val = ctx.regCr.val;

    //-----------------------------------------------------------------------------------
    // Wait till RNG enable OK

    uint8 timeout = ENABLE_WAIT_ms;

    while (REG_RNG->CR.bit.CONDRST) {
        DelayMs(1);
        ASSERT(--timeout > 0, "RNG PERIPHERAL ENABLE");
    }

    //-----------------------------------------------------------------------------------
    // Check error flags

    ASSERT(REG_RNG->SR.bit.SECS == false, "RNG SEED ERROR FLAG SET");
    ASSERT(REG_RNG->SR.bit.CECS == false, "CLOCK ERROR FLAG SET");

    //-----------------------------------------------------------------------------------

    ctx.status = Status::OK;
    return Exec::OK;
}

uint64 Get64BitNumber()
{
    CHECK_STATUS();
    uint64 randomNumber;
    while (REG_RNG->SR.bit.DRDY == false);
    randomNumber = (uint64)REG_RNG->DR.val << 32;
    while (REG_RNG->SR.bit.DRDY == false);
    randomNumber |= REG_RNG->DR.val;
    return randomNumber;
}

uint32 Get32BitNumber()
{
    while (REG_RNG->SR.bit.DRDY == false);
    return REG_RNG->DR.val;
}

uint16 Get16BitNumber()
{
    while (REG_RNG->SR.bit.DRDY == false);
    return (uint16)REG_RNG->DR.val;
}

uint8 Get8BitNumber()
{
    while (REG_RNG->SR.bit.DRDY == false);
    return (uint8)REG_RNG->DR.val;
}

Status GetStatus()
{
    return ctx.status;
}

} // END OF NAMESPACE 'hal::rng'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
