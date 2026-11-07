/****************************************************************************************************************************************
 *
 * Interrupt callback implementation for STM32WLE5xx
 * Peripheral: RTC
 *
 ****************************************************************************************************************************************/

#include "irqRtc.h"

namespace hal::irqRtc {

static IrqClbk_t WakeupClbk = nullptr;
static IrqClbk_t AlarmClbk  = nullptr;

Exec SetWakeupIrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "RTC WAKEUP IRQ CALLBACK");
    WakeupClbk = Clbk;
    return Exec::OK;
}

Exec SetAlarmIrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "RTC ALARM IRQ CALLBACK");
    AlarmClbk = Clbk;
    return Exec::OK;
}

} // End of namespace 'hal::irqRtc'

extern "C" {
    void RTC_WKUP_IRQHandler();
    void RTC_Alarm_IRQHandler();
}

void RTC_WKUP_IRQHandler()
{
    if (hal::irqRtc::WakeupClbk) {
        hal::irqRtc::WakeupClbk();
    }
    else {
        REPORT_FAT("RTC WAKEUP IRQ CALLBACK");
    }
}

void RTC_Alarm_IRQHandler()
{
    if (hal::irqRtc::AlarmClbk) {
        hal::irqRtc::AlarmClbk();
    }
    else {
        REPORT_FAT("RTC ALARM IRQ CALLBACK");
    }
}

/****************************************************************************************************************************************
                                                        END OF FILE
****************************************************************************************************************************************/
