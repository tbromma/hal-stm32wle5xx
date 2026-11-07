/******************************************************************************************************************************************
 *
 * CREATED BY: Tobiasz Bromma (tobiasz.bromma@thoriumspace.com)
 *
 ******************************************************************************************************************************************/

#include "mcuCore.h"
#include "regSystick.h"
#include "regRcc.h"

namespace hal::core {

#undef CODE_STATUS
#define CODE_STATUS systickStat

static constexpr uint32 SYSTICK_MAX_VAL = 16777215;     // 24Bit
Status systickStat;

void SystemReset()
{
    NVIC_SystemReset();
}

void GetResetReason(rstReason_t& rstReason)
{
    rstReason.val = (uint8)(REG_RCC->CSR.val >> 24);
}

void ClearResetFlags()
{
    REG_RCC->CSR.bit.RMVF = true;
}

Exec SystickConfig(systickClkSrc_t clkSrc, uint32 reloadVal, bool irqEnable, bool timerEnable)
{
    ASSERT(reloadVal <= SYSTICK_MAX_VAL, "VARIABLE 'reloadVal'");

    REG_SYSTICK->RVR.val 	        = reloadVal;        // Set systick reload value
    REG_SYSTICK->CVR.bit.CURRENT 	= 0;                // Set to 0 the counter value
    REG_SYSTICK->CSR.bit.CLKSOURCE  = clkSrc & 0b1;     // Set the clock source
    REG_SYSTICK->CSR.bit.TICKINT    = irqEnable;        // Enable-Disable the systick interrupt
    REG_SYSTICK->CSR.bit.ENABLE     = timerEnable;

    systickStat = Status::OK;
    return Exec::OK;
}

void SystickEnable()
{
    CHECK_STATUS();
    REG_SYSTICK->CSR.bit.ENABLE = true;
}

void SystickDisable()
{
    CHECK_STATUS();
    REG_SYSTICK->CSR.bit.ENABLE = false;
}

void SystickIrqEnable()
{
    CHECK_STATUS();
    REG_SYSTICK->CSR.bit.TICKINT = true;
}

void SystickIrqDisable()
{
    CHECK_STATUS();
    REG_SYSTICK->CSR.bit.TICKINT = false;
}

void SystickSetReloadVal(uint32 reloadVal, bool resetTimer)
{
    CHECK_STATUS();
    REG_SYSTICK->RVR.val = reloadVal;
    if (resetTimer) REG_SYSTICK->CVR.bit.CURRENT = 0;
}

Status SystickGetStatus()
{
    return systickStat;
}

void IrqGlobalEnable()
{
    asm volatile ("CPSIE i");
}

void IrqGlobalDisable()
{
    asm volatile ("CPSID i");
}

bool IrqGlobalStatus()
{
    return (bool)(__get_PRIMASK() ^ 1);     // If the first bit of the PRIMASK register is 0 then global interrupts are enbaled
}

__attribute__((always_inline)) inline void IrqFaultEnable()
{
    asm volatile ("CPSIE f");
}

__attribute__((always_inline)) inline void IrqFaultDisable()
{
    asm volatile ("CPSID f");
}

void IrqEnable(irqId_t irqId)
{
    NVIC_EnableIRQ((IRQn_Type)irqId);
}

void IrqDisable(irqId_t irqId)
{
    NVIC_DisableIRQ((IRQn_Type)irqId);
}

void IrqSetPrio(irqId_t irqId, irqPrio_t prio)
{
    NVIC_SetPriority((IRQn_Type)irqId, prio);
}

irqPrio_t IrqGetPrio(irqId_t irqId)
{
    return (irqPrio_t)NVIC_GetPriority((IRQn_Type)irqId);
}

bool IrqGetEnable(irqId_t irqId)
{
    return NVIC_GetEnableIRQ((IRQn_Type)irqId);
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::core'