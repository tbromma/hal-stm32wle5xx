/****************************************************************************************************************************************
 *
 * Interrupt callback implementation for STM32WLE5xx
 * Peripheral: TIM / LPTIM
 *
 ****************************************************************************************************************************************/

#include "irqTim.h"

namespace hal::irqTim {

static IrqClbk_t Tim1BrkClbk = nullptr;
static IrqClbk_t Tim1UpClbk = nullptr;
static IrqClbk_t Tim1TrgComClbk = nullptr;
static IrqClbk_t Tim1CcClbk = nullptr;
static IrqClbk_t Tim2Clbk = nullptr;
static IrqClbk_t Tim16Clbk = nullptr;
static IrqClbk_t Tim17Clbk = nullptr;
static IrqClbk_t Lptim1Clbk = nullptr;
static IrqClbk_t Lptim2Clbk = nullptr;
static IrqClbk_t Lptim3Clbk = nullptr;

Exec SetTim1BrkIrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "TIM1 BRK IRQ CALLBACK");
    Tim1BrkClbk = Clbk;
    return Exec::OK;
}

Exec SetTim1UpIrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "TIM1 UP IRQ CALLBACK");
    Tim1UpClbk = Clbk;
    return Exec::OK;
}

Exec SetTim1TrgComIrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "TIM1 TRG/COM IRQ CALLBACK");
    Tim1TrgComClbk = Clbk;
    return Exec::OK;
}

Exec SetTim1CcIrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "TIM1 CC IRQ CALLBACK");
    Tim1CcClbk = Clbk;
    return Exec::OK;
}

Exec SetTim2IrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "TIM2 IRQ CALLBACK");
    Tim2Clbk = Clbk;
    return Exec::OK;
}

Exec SetTim16IrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "TIM16 IRQ CALLBACK");
    Tim16Clbk = Clbk;
    return Exec::OK;
}

Exec SetTim17IrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "TIM17 IRQ CALLBACK");
    Tim17Clbk = Clbk;
    return Exec::OK;
}

Exec SetLptim1IrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "LPTIM1 IRQ CALLBACK");
    Lptim1Clbk = Clbk;
    return Exec::OK;
}

Exec SetLptim2IrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "LPTIM2 IRQ CALLBACK");
    Lptim2Clbk = Clbk;
    return Exec::OK;
}

Exec SetLptim3IrqClbk(IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "LPTIM3 IRQ CALLBACK");
    Lptim3Clbk = Clbk;
    return Exec::OK;
}

} // End of namespace 'hal::irqTim'

extern "C" {
    void TIM1_BRK_IRQHandler();
    void TIM1_UP_IRQHandler();
    void TIM1_TRG_COM_IRQHandler();
    void TIM1_CC_IRQHandler();
    void TIM2_IRQHandler();
    void TIM16_IRQHandler();
    void TIM17_IRQHandler();
    void LPTIM1_IRQHandler();
    void LPTIM2_IRQHandler();
    void LPTIM3_IRQHandler();
}

void TIM1_BRK_IRQHandler()
{
    if (hal::irqTim::Tim1BrkClbk) {
        hal::irqTim::Tim1BrkClbk();
    }
    else {
        REPORT_FAT("TIM1 BRK IRQ CALLBACK");
    }
}

void TIM1_UP_IRQHandler()
{
    if (hal::irqTim::Tim1UpClbk) {
        hal::irqTim::Tim1UpClbk();
    }
    else {
        REPORT_FAT("TIM1 UP IRQ CALLBACK");
    }
}

void TIM1_TRG_COM_IRQHandler()
{
    if (hal::irqTim::Tim1TrgComClbk) {
        hal::irqTim::Tim1TrgComClbk();
    }
    else {
        REPORT_FAT("TIM1 TRG/COM IRQ CALLBACK");
    }
}

void TIM1_CC_IRQHandler()
{
    if (hal::irqTim::Tim1CcClbk) {
        hal::irqTim::Tim1CcClbk();
    }
    else {
        REPORT_FAT("TIM1 CC IRQ CALLBACK");
    }
}

void TIM2_IRQHandler()
{
    if (hal::irqTim::Tim2Clbk) {
        hal::irqTim::Tim2Clbk();
    }
    else {
        REPORT_FAT("TIM2 IRQ CALLBACK");
    }
}

void TIM16_IRQHandler()
{
    if (hal::irqTim::Tim16Clbk) {
        hal::irqTim::Tim16Clbk();
    }
    else {
        REPORT_FAT("TIM16 IRQ CALLBACK");
    }
}

void TIM17_IRQHandler()
{
    if (hal::irqTim::Tim17Clbk) {
        hal::irqTim::Tim17Clbk();
    }
    else {
        REPORT_FAT("TIM17 IRQ CALLBACK");
    }
}

void LPTIM1_IRQHandler()
{
    if (hal::irqTim::Lptim1Clbk) {
        hal::irqTim::Lptim1Clbk();
    }
    else {
        REPORT_FAT("LPTIM1 IRQ CALLBACK");
    }
}

void LPTIM2_IRQHandler()
{
    if (hal::irqTim::Lptim2Clbk) {
        hal::irqTim::Lptim2Clbk();
    }
    else {
        REPORT_FAT("LPTIM2 IRQ CALLBACK");
    }
}

void LPTIM3_IRQHandler()
{
    if (hal::irqTim::Lptim3Clbk) {
        hal::irqTim::Lptim3Clbk();
    }
    else {
        REPORT_FAT("LPTIM3 IRQ CALLBACK");
    }
}

/****************************************************************************************************************************************
                                                        END OF FILE
****************************************************************************************************************************************/
