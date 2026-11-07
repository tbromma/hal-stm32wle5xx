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

#include "perAdc.h"
#include "regAdc.h"
#include "sysclk.h"
#include "perGpio.h"
#include "mcuCore.h"
#include "irqAdc.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::adc {

#undef CODE_STATUS
#undef ACTION_ON_ERR
#define CODE_STATUS     ctx.status
#define ACTION_ON_ERR   ctx.status = Status::ERR; ctx.mode = MODE_ERR

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec ADC::Setup(const adcCfg_t& config)
{
    ctx.status = Status ::INIT;
    ctx.config = config;

    ASSERT(ctx.config.clkSrc != hal::clk::ADC_CLKSRC_NONE, "CLOCK SOURCE UNDEFINED");
    ASSERT(ctx.config.presc != PRESC_none, "PRESCALER VALUE UNDEFINED");
    ASSERT(ctx.config.oversamp != OVERSAMP_none, "OVERSAMPLING VALUE UNDEFINED");
    ASSERT(ctx.config.sampTime != SAMPTIME_none, "SAMPLING TIME UNDEFINED");
    ASSERT(ctx.config.label_p != nullptr, "CODE_LABEL IS A NULLPOINTER");

    hal::clk::PerClkEnable(hal::PER_ADC);
    hal::clk::AdcClkSrcSelect(ctx.config.clkSrc);

    REG_ADC->CR.bit.ADVREGEN = 1;                                       // ADC Voltage Regulator Enable
    bareOs::Timer::DelayMs(10);
    REG_ADC->CCR.bit.PRESC = ctx.config.presc & 0b1111;
    REG_ADC->CFGR1.bit.ALIGN = 0;                                       // ADC data result right alignment
    REG_ADC->CFGR1.bit.RES = ctx.config.resolution & 0b11;              // ADC measurement resolution
    REG_ADC->CFGR1.bit.OVRMOD = 1;                                      // Data register is overwritten on overrun
    REG_ADC->CFGR2.bit.CKMODE = ctx.config.clkSrc & 0b11;               // ADC clock source and mode
    REG_ADC->CFGR2.bit.OVSR = (ctx.config.oversamp > OVERSAMP_OFF) ? (ctx.config.oversamp - 1) & 0b111 : 0;
    REG_ADC->CFGR2.bit.OVSE = (ctx.config.oversamp > OVERSAMP_OFF) ? 1 & 0b111 : 0;
    REG_ADC->SMPR.bit.SMP1 = ctx.config.sampTime & 0b111;
    REG_ADC->SMPR.bit.SMP2 = ctx.config.sampTime & 0b111;
    REG_ADC->CFGR2.bit.OVSS = ctx.config.oversamp & 0b1111;

    REG_ADC->CR.bit.ADCAL = true;
    while (REG_ADC->ISR.bit.EOCAL == false);

    hal::irqAdc::SetIrqClbk(ConversionDoneIrqClbk);

    ctx.status = Status::OK;
    return Exec::OK;
}

Exec ADC::SetupChannel(const chCfg_t& config)
{
    CHECK_STATUS();
    ASSERT(config.chNum != CHNUM_none, "CHANNEL NUMBER UNDEFINED");
    ASSERT(config.pinNum != PIN_none, "PIN NUMBER UNDEFINED");
    ASSERT(config.pinPort != PORT_none, "PIN PORT UNDEFINED");
    ASSERT(config.label_p != nullptr, "CODE_LABEL IS A NULLPOINTER");

    GPIO::SetPinMode(config.pinPort, config.pinNum, GPIO::PinMode::Analog);
    GPIO::SetPinPullDir(config.pinPort, config.pinNum, GPIO::PullDir::None);
    GPIO::SetPinInUse(config.pinPort, config.pinNum, config.label_p);

    ctx.channel[config.chNum].config = config;
    ctx.channel[config.chNum].triggConv = false;
    ctx.channel[config.chNum].clbk_t = nullptr;
    ctx.channel[config.chNum].valid = true;
    return Exec::OK;
}

void ADC::EnableAdc()
{
    CHECK_STATUS();
    REG_ADC->CR.bit.ADEN = true;
    while (REG_ADC->ISR.bit.ADRDY == false);
}

void ADC::DisableAdc()
{
    CHECK_STATUS();
    REG_ADC->CR.bit.ADEN = false;
}

Exec ADC::TriggerConvPoll(chNum_t chNum, uint16& result)
{
    CHECK_STATUS();
    ASSERT(chNum != CHNUM_none, "Invalid channel number!");
    ASSERT(ctx.channel[chNum].valid, "Selected channel not configured!");

    EnableAdc();
    REG_ADC->CHSELR.val = 1 << (uint8)chNum;
    while (REG_ADC->ISR.bit.CCRDY == false);
    REG_ADC->CR.bit.ADSTART = true;
    while (REG_ADC->ISR.bit.EOC == false);
    result = REG_ADC->DR.bit.DATA;
    DisableAdc();
    return Exec::OK;
}

Exec ADC::TriggerConvClbk(chNum_t chNum, ConvReadyClbk_t Clbk)
{
    CHECK_STATUS();
    ASSERT(chNum != CHNUM_none, "Invalid channel number!");
    ASSERT(ctx.channel[chNum].valid, "Selected channel not configured!");

    REG_ADC->CHSELR.val = 1 << (uint8)chNum;
    while (REG_ADC->ISR.bit.CCRDY == false);

    ctx.Clbk = Clbk;
    ctx.channelRun = chNum;
    REG_ADC->CR.bit.ADSTART = true;
    EnableIrq();
    return Exec::OK;
}

Status ADC::GetStatus()
{
    return ctx.status;
}

/******************************************************************************************************************************************
                                                         PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

void ADC::EnableIrq()
{
    REG_ADC->ISR.bit.EOC = 1;
    REG_ADC->IER.bit.EOCIE = 1;
    hal::core::IrqEnable(IRQ_ADC);
}

void ADC::DisableIrq()
{
    REG_ADC->IER.bit.EOCIE = 0;
    hal::core::IrqDisable(IRQ_ADC);
}

void ADC::ConversionDoneIrqClbk()
{
    adc_p->DisableIrq();
    adc_p->ctx.Clbk(adc_p->ctx.channelRun, REG_ADC->DR.bit.DATA);
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::adc'
