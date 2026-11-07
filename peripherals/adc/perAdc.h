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

// CREATED BY:       Tobiasz Bromma (tobiasz.bromma@thoriumspace.com)
// CREATION DATE:
// DESCRIPTION:

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include "common.h"
#include "sysclk.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::adc {

enum clkSrc_t : int8 {
    CLKSRC_none = -1,
    CLKSRC_ASYNC = 0,
    CLKSRC_PCLK_DIV1 = 3,       // PCLK synchronus clock
    CLKSRC_PCLK_DIV2 = 1,       // PCLK synchronus clock divided by 2
    CLKSRC_PCLK_DIV4 = 2,       // PCLK synchronus clock divided by 4
};

enum chNum_t : int8 {
    CHNUM_none = -1,
    CHNUM_0 = 0,
    CHNUM_1,
    CHNUM_2,
    CHNUM_3,
    CHNUM_4,
    CHNUM_5,
    CHNUM_6,
    CHNUM_7,
    CHNUM_8,
    CHNUM_9,
    CHNUM_10,
    CHNUM_11,
    CHNUM_12,
    CHNUM_13,
    CHNUM_14,
    CHNUM_15,
    CHNUM_16,
    CHNUM_17,
    CHNUM_count,
};

enum resolution_t : int8 {
    RESOLUTION_none = -1,
    RESOLUTION_12bit,
    RESOLUTION_10bit,
    RESOLUTION_8bit,
    RESOLUTION_6bit,
};

enum presc_t : int8 {
    PRESC_none = -1,
    PRESC_1 = 0b0000,           // Input clock not divided
    PRESC_2 = 0b0001,           // Input clock divided by 2
    PRESC_4 = 0b0010,           // Input clock divided by 4
    PRESC_6 = 0b0011,           // Input clock divided by 6
    PRESC_8 = 0b0100,           // Input clock divided by 8
    PRESC_10 = 0b0101,          // Input clock divided by 10
    PRESC_12 = 0b0110,          // Input clock divided by 12
    PRESC_16 = 0b0111,          // Input clock divided by 16
    PRESC_32 = 0b1000,          // Input clock divided by 32
    PRESC_64 = 0b1001,          // Input clock divided by 64
    PRESC_128 = 0b1010,         // Input clock divided by 128
    PRESC_256 = 0b1011,         // Input clock divided by 256
};

enum oversamp_t : int8 {
    OVERSAMP_none = -1,         // Oversampling disabled
    OVERSAMP_OFF = 0,
    OVERSAMP_2x,
    OVERSAMP_4x,
    OVERSAMP_8x,
    OVERSAMP_16x,
    OVERSAMP_32x,
    OVERSAMP_64x,
    OVERSAMP_128x,
    OVERSAMP_256x,
};

enum sampTime_t : int8 {
    SAMPTIME_none = -1,
    SAMPTIME_1p5  = 0b000,      // Sampling time 1.5 ADC clock cycles
    SAMPTIME_3p5  = 0b001,      // Sampling time 3.5 ADC clock cycles
    SAMPTIME_7p5  = 0b010,      // Sampling time 7.5 ADC clock cycles
    SAMPTIME_12p5 = 0b011,      // Sampling time 12.5 ADC clock cycles
    SAMPTIME_19p5 = 0b100,      // Sampling time 19.5 ADC clock cycles
    SAMPTIME_39p5 = 0b101,      // Sampling time 39.5 ADC clock cycles
    SAMPTIME_79p5 = 0b110,      // Sampling time 79.5 ADC clock cycles
    SAMPTIME_160p5 = 0b111,     // Sampling time 160.5 ADC clock cycles
};

struct adcCfg_t {

    clk::adcClkSrc_t    clkSrc = clk::ADC_CLKSRC_NONE;
    presc_t             presc = PRESC_none;
    sampTime_t          sampTime = SAMPTIME_none;
    resolution_t        resolution = RESOLUTION_none;
    oversamp_t          oversamp = OVERSAMP_none;
    const char*         label_p = nullptr;
};

struct chCfg_t {
    chNum_t         chNum = CHNUM_none;
    hal::pinPort_t  pinPort = hal::PORT_none;
    hal::pinNum_t   pinNum = hal::PIN_none;
    const char*     label_p = nullptr;
};

using ConvReadyClbk_t = void (*)(chNum_t chNum, uint16 result);

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/

class ADC {
public:

    Exec    Setup(const adcCfg_t& config);
    Exec    SetupChannel(const chCfg_t& config);
    void    EnableAdc();
    void    DisableAdc();
    Exec    TriggerConvPoll(chNum_t chNum, uint16& result);
    Exec    TriggerConvClbk(chNum_t chNum, ConvReadyClbk_t Clbk);
    Status  GetStatus();

    ADC() { adc_p = this; }

private:

    struct channel_t {
        chCfg_t config;
        bool valid;
        bool triggConv;
        uint16 resultBin;
        ConvReadyClbk_t clbk_t;
    };

    void EnableIrq();
    void DisableIrq();
    static void ConversionDoneIrqClbk();

    struct {
        adcCfg_t config;
        Status status = Status::NONE;
        channel_t channel[CHNUM_count];
        ConvReadyClbk_t Clbk;
        chNum_t channelRun;
    } ctx = {};

    static inline ADC* adc_p = nullptr;
};

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // End of namespace 'hal::adc'