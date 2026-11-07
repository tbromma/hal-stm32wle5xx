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
// CREATION DATE:    October 2023
// DESCRIPTION:      EXTI peripheral interrupt assignment

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "irqExti.h"
#include "regExti.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::irqExti {

#undef CODE_LABEL
#define CODE_LABEL  "IRQ EXTI"

static IrqClbk_t ExtiClbkTab[EXTI_count];

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/

Exec SetIrqClbk(extiNum_t extiNum, IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "EXTI IRQ CALLBACK");
    ExtiClbkTab[extiNum] = Clbk;
    return Exec::OK;
}

} // End of namespace 'hal::irqExti'
/******************************************************************************************************************************************
                                                             INTERRUPT HANDLERS
******************************************************************************************************************************************/

#undef CODE_LABEL
#define CODE_LABEL   "IRQ EXTI"

extern "C" {
    void EXTI0_IRQHandler();
    void EXTI1_IRQHandler();
    void EXTI2_IRQHandler();
    void EXTI3_IRQHandler();
    void EXTI4_IRQHandler();
    void EXTI9_5_IRQHandler();
    void EXTI15_10_IRQHandler();
}

static void IrqErrHandler(int8 extiNum)
{
    REPORT_FAT("EXTI%d IRQ CALLBACK", extiNum);
}

void EXTI0_IRQHandler()
{
    REG_EXTI->PR1.val |= 1 << hal::irqExti::EXTI_0;
    if (hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_0]) {
        hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_0]();
    }
    else IrqErrHandler(hal::irqExti::EXTI_0);
}

void EXTI1_IRQHandler()
{
    REG_EXTI->PR1.val |= 1 << hal::irqExti::EXTI_1;
    if (hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_1]) {
        hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_1]();
    }
    else IrqErrHandler(hal::irqExti::EXTI_1);
}

void EXTI2_IRQHandler()
{
    REG_EXTI->PR1.val |= 1 << hal::irqExti::EXTI_2;
    if (hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_2]) {
        hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_2]();
    }
    else IrqErrHandler(hal::irqExti::EXTI_2);
}

void EXTI3_IRQHandler()
{
    REG_EXTI->PR1.val |= 1 << hal::irqExti::EXTI_3;
    if (hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_3]) {
        hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_3]();
    }
    else IrqErrHandler(hal::irqExti::EXTI_3);
}

void EXTI4_IRQHandler()
{
    REG_EXTI->PR1.val |= 1 << hal::irqExti::EXTI_4;
    if (hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_4]) {
        hal::irqExti::ExtiClbkTab[hal::irqExti::EXTI_4]();
    }
    else IrqErrHandler(hal::irqExti::EXTI_4);
}

void EXTI9_5_IRQHandler()
{
    for (int8 extiNum = hal::irqExti::EXTI_5; extiNum <= hal::irqExti::EXTI_9; extiNum++) {
        if (REG_EXTI->PR1.val & (1 << extiNum)) {
            REG_EXTI->PR1.val |= 1 << extiNum;
            if (hal::irqExti::ExtiClbkTab[extiNum]) {
                hal::irqExti::ExtiClbkTab[extiNum]();
            }
            else IrqErrHandler(extiNum);
        }
    }
}

void EXTI15_10_IRQHandler()
{
    for (int8 extiNum = hal::irqExti::EXTI_10; extiNum <= hal::irqExti::EXTI_15; extiNum++) {
        if (REG_EXTI->PR1.val & (1 << extiNum)) {
            REG_EXTI->PR1.val |= 1 << extiNum;
            if (hal::irqExti::ExtiClbkTab[extiNum]) {
                hal::irqExti::ExtiClbkTab[extiNum]();
            }
            else IrqErrHandler(extiNum);
        }
    }
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
