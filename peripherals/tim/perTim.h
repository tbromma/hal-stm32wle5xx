/******************************************************************************************************************************************
                                                               START OF FILE
******************************************************************************************************************************************/

// DESCRIPTION: Generic HAL layer for STM32WLE5 TIM1, TIM2, TIM16 and TIM17 peripherals

#pragma once
#include "common.h"
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"
#include "perGpio.h"

namespace hal {

class TIM {
public:

    enum class Num : int8 {
        none = -1,
        TIM_1 = 0,
        TIM_2,
        TIM_16,
        TIM_17,
        count,
    };

    enum class Mode : uint8 {
        NONE,
        OFF,
        RUN,
        ERR,
    };

    enum class Channel : int8 {
        none = -1,
        CH1 = 0,
        CH2,
        CH3,
        CH4,
        count,
    };

    enum class Polarity : uint8 {
        High,
        Low,
    };

    using UpdateClbk_t = void (*)(Num timNum);

    struct irqConfig_t {
        bool            enable = false;
        hal::irqPrio_t  priority = hal::IRQ_PRIO_none;
        UpdateClbk_t    Clbk = nullptr;
    };

    struct config_t {
        Num             timNum = Num::none;
        uint32          freqHz = 1000;                // Counter update/PWM frequency
        bool            autoStart = false;
        irqConfig_t     updateIrq = {};
        const char*     label_p = nullptr;
    };

    struct pwmConfig_t {
        Channel             channel = Channel::none;
        uint16              dutyPermille = 0;         // 0...1000 = 0.0...100.0%
        Polarity            polarity = Polarity::High;
        hal::pinPort_t      pinPort = hal::PORT_none;
        hal::pinNum_t       pinNum = hal::PIN_none;
        hal::pinFunc_t      pinFunc = hal::FUNC_none;
        hal::GPIO::OutSpeed speed = hal::GPIO::OutSpeed::High;
        const char*         label_p = nullptr;
    };

    Exec    Setup(config_t config);
    Exec    AddPwmChannel(pwmConfig_t config);

    void    Enable();
    void    Disable();
    Exec    SetFrequency(uint32 freqHz);
    Exec    SetDuty(Channel channel, uint16 dutyPermille);

    uint32  GetFrequencyHz();
    uint32  GetTimerClockHz();
    uint32  GetCounter();
    void    SetCounter(uint32 value);
    Status  GetStatus();
    Mode    GetMode();
    config_t GetConfig();

private:

    struct channelCtx_t {
        bool        configured = false;
        uint16      dutyPermille = 0;
        pwmConfig_t config = {};
    };

    void        ChangeMode(Mode newMode);
    uint32      CalcTimerClockHz();
    Exec        CalcTimebase(uint32 freqHz, uint16* psc_p, uint32* arr_p, uint32* actualFreqHz_p);
    void        ApplyDuty(Channel channel);
    bool        IsChannelSupported(Channel channel);
    volatile uint32* GetCcrReg(Channel channel);
    void        MainOutputEnable();

    static void IrqHandlerClbk(Num timNum);

    struct {
        config_t            config = {};
        Status              status = Status::NONE;
        Mode                mode = Mode::NONE;
        Mode                modeLast = Mode::NONE;
        hal::perId_t        perId = hal::PER_none;
        hal::irqId_t        irqId = hal::IRQ_none;
        TIM_TypeDef*        timReg_p = nullptr;
        uint32              timerClkHz = 0;
        uint32              freqHz = 0;
        uint16              prescaler = 0;
        uint32              autoReload = 0;
        channelCtx_t        channel[(uint8)Channel::count] = {};
    } ctx = {};

    static inline TIM* timer_p[(uint8)Num::count] = {};
};

} // END OF NAMESPACE 'hal'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
