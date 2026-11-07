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

#include "perFlash.h"
#include "regFlash.h"
#include "sysclk.h"


/******************************************************************************************************************************************
                                                           PRIVATE DEFINES & TYPES
******************************************************************************************************************************************/
namespace hal::flash {

static constexpr uint32 _0_WAIT_STATES_MAX_FREQ_Hz = 18000000;
static constexpr uint32 _1_WAIT_STATES_MAX_FREQ_Hz = 36000000;
static constexpr uint32 _2_WAIT_STATES_MAX_FREQ_Hz = 48000000;

uint32 tabWsMaxFreqHz[] = { _0_WAIT_STATES_MAX_FREQ_Hz,
                            _1_WAIT_STATES_MAX_FREQ_Hz,
                            _2_WAIT_STATES_MAX_FREQ_Hz  };

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec ChangeWaitStates(waitStates_t waitStates)
{
    uint32 sysClkFreqHz = hal::clk::SysClkGetFreqHz(hal::clk::SYSCLK_HCLK1);
    ASSERT(sysClkFreqHz <= tabWsMaxFreqHz[waitStates], "CLOCK TO FAST FOR %d WAIT STATES", waitStates);
    REG_FLASH->ACR.bit.LATENCY = (uint8)waitStates & 0b111;
    return Exec::OK;
}

Exec ChangeWaitStates(uint32 systemClkFreqHz)
{
    ASSERT(systemClkFreqHz <= _2_WAIT_STATES_MAX_FREQ_Hz, "CLOCK SPEED TOO FAST");

    waitStates_t waitStates;

    if (systemClkFreqHz <= _0_WAIT_STATES_MAX_FREQ_Hz)      waitStates = WAIT_STATES_0;
    else if (systemClkFreqHz <= _1_WAIT_STATES_MAX_FREQ_Hz) waitStates = WAIT_STATES_1;
    else                                                    waitStates = WAIT_STATES_2;

    REG_FLASH->ACR.bit.LATENCY = waitStates & 0b111;
    return Exec::OK;
}

} // END OF NAMESPACE 'hal::flash'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
