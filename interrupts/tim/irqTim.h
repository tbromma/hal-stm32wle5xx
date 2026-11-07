/****************************************************************************************************************************************
 *
 * Interrupt callback definitions for STM32WLE5xx
 * Peripheral: TIM1, TIM2, TIM16, TIM17, LPTIM1, LPTIM2, LPTIM3
 *
 ****************************************************************************************************************************************/

#pragma once
#include "common.h"
#include "stm32wleTypes.h"

namespace hal::irqTim {

using IrqClbk_t = void (*)();

Exec SetTim1BrkIrqClbk(IrqClbk_t Clbk);
Exec SetTim1UpIrqClbk(IrqClbk_t Clbk);
Exec SetTim1TrgComIrqClbk(IrqClbk_t Clbk);
Exec SetTim1CcIrqClbk(IrqClbk_t Clbk);
Exec SetTim2IrqClbk(IrqClbk_t Clbk);
Exec SetTim16IrqClbk(IrqClbk_t Clbk);
Exec SetTim17IrqClbk(IrqClbk_t Clbk);
Exec SetLptim1IrqClbk(IrqClbk_t Clbk);
Exec SetLptim2IrqClbk(IrqClbk_t Clbk);
Exec SetLptim3IrqClbk(IrqClbk_t Clbk);

} // End of namespace 'hal::irqTim'

/****************************************************************************************************************************************
                                                        END OF FILE
****************************************************************************************************************************************/
