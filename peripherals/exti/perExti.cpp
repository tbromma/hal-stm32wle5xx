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
// CREATION DATE:    Oktober 2023
// DESCRIPTION:      Low level layer for EXTI (External Interrupt) peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "perExti.h"
#include "irqExti.h"
#include "sysclk.h"
#include "regSyscfg.h"
#include "regExti.h"
#include "mcuCore.h"
#include "irqList.h"
#include "perGpio.h"

/******************************************************************************************************************************************
                                                           PRIVATE DEFINES & TYPES
******************************************************************************************************************************************/
namespace hal::exti {

#undef ACTION_ON_ERR
#undef CODE_LABEL
#undef CODE_STATUS
#define ACTION_ON_ERR   ctx.status = Status::ERR
#define CODE_LABEL      ctx.config.label_p
#define CODE_STATUS     ctx.status

static constexpr uint8 EXTI_CFG_CNT = 4;    // Possible options to assign the EXTI line to a hardware pin

static constexpr int8 extiPinoutTab[hal::irqExti::EXTI_count][EXTI_CFG_CNT][2] = {
    { {hal::PORT_A, hal::PIN_0}, {hal::PORT_B, hal::PIN_0}, {hal::PORT_C, hal::PIN_0}, {hal::PORT_none, hal::PIN_none} },           // EXTI0
    { {hal::PORT_A, hal::PIN_1}, {hal::PORT_B, hal::PIN_1}, {hal::PORT_C, hal::PIN_1}, {hal::PORT_none, hal::PIN_none} },           // EXTI1
    { {hal::PORT_A, hal::PIN_2}, {hal::PORT_B, hal::PIN_2}, {hal::PORT_C, hal::PIN_2}, {hal::PORT_none, hal::PIN_none} },           // EXTI2
    { {hal::PORT_A, hal::PIN_3}, {hal::PORT_B, hal::PIN_3}, {hal::PORT_C, hal::PIN_3}, {hal::PORT_none, hal::PIN_none} },                 // EXTI3

    { {hal::PORT_A, hal::PIN_4}, {hal::PORT_B, hal::PIN_4}, {hal::PORT_C, hal::PIN_4}, {hal::PORT_none, hal::PIN_none} },           // EXTI4
    { {hal::PORT_A, hal::PIN_5}, {hal::PORT_B, hal::PIN_5}, {hal::PORT_C, hal::PIN_5}, {hal::PORT_none, hal::PIN_none} },           // EXTI5
    { {hal::PORT_A, hal::PIN_6}, {hal::PORT_B, hal::PIN_6}, {hal::PORT_C, hal::PIN_6}, {hal::PORT_none, hal::PIN_none} },           // EXTI6
    { {hal::PORT_A, hal::PIN_7}, {hal::PORT_B, hal::PIN_7}, {hal::PORT_none, hal::PIN_none}, {hal::PORT_none, hal::PIN_none} },     // EXTI7

    { {hal::PORT_A, hal::PIN_8}, {hal::PORT_B, hal::PIN_8}, {hal::PORT_none, hal::PIN_none}, {hal::PORT_none, hal::PIN_none} },     // EXTI8
    { {hal::PORT_A, hal::PIN_9}, {hal::PORT_B, hal::PIN_9}, {hal::PORT_none, hal::PIN_none}, {hal::PORT_none, hal::PIN_none} },     // EXTI9
    { {hal::PORT_A, hal::PIN_10}, {hal::PORT_B, hal::PIN_10}, {hal::PORT_none, hal::PIN_none}, {hal::PORT_none, hal::PIN_none} },   // EXTI10
    { {hal::PORT_A, hal::PIN_11}, {hal::PORT_B, hal::PIN_11}, {hal::PORT_none, hal::PIN_none}, {hal::PORT_none, hal::PIN_none} },   // EXTI11

    { {hal::PORT_A, hal::PIN_12}, {hal::PORT_B, hal::PIN_12}, {hal::PORT_none, hal::PIN_none}, {hal::PORT_none, hal::PIN_none} },   // EXTI12
    { {hal::PORT_A, hal::PIN_13}, {hal::PORT_B, hal::PIN_13}, {hal::PORT_C, hal::PIN_13}, {hal::PORT_none, hal::PIN_none} },        // EXTI13
    { {hal::PORT_A, hal::PIN_14}, {hal::PORT_B, hal::PIN_14}, {hal::PORT_C, hal::PIN_14}, {hal::PORT_none, hal::PIN_none} },        // EXTI14
    { {hal::PORT_A, hal::PIN_15}, {hal::PORT_B, hal::PIN_15}, {hal::PORT_C, hal::PIN_15}, {hal::PORT_none, hal::PIN_none} },        // EXTI15
};

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec EXTI::Setup(config_t config)
{
    ctx.status = Status ::INIT;
    ctx.config = config;

    //-----------------------------------------------------------------------------------
    // Perform config check

    ASSERT(ctx.config.pinNum != hal::PIN_none, "PIN NUMBER");
    ASSERT(ctx.config.pinPort != hal::PORT_none, "PORT NUMBER");
    ASSERT(ctx.config.pinMode != hal::GPIO::PinMode::None, "PIN MODE");
    ASSERT_COND(ctx.config.pinMode == hal::GPIO::PinMode::AltFunc,
                ctx.config.pinFunc != hal::FUNC_none, "PIN ALT. FUNC.");
    ASSERT(ctx.config.extiNum != hal::irqExti::EXTI_none, "EXTI LINE NUMBER");
    ASSERT(ctx.config.intType != INT_none, "IRQ TYPE");
    ASSERT(ctx.config.irqPrio != hal::IRQ_PRIO_none, "IRQ PRIORITY");
    ASSERT(ctx.config.IrqClbk != nullptr, "IRQ CALLBACK");
    ASSERT(ctx.config.label_p != nullptr, "CODE_LABEL STRING");
    ASSERT(*ctx.config.label_p > 0, "CODE_LABEL STRING STARTS WITH ZERO");

    //-----------------------------------------------------------------------------------
    // Verify pin port and pin number

    bool matchFound = false;

    for (uint8 extiCfgOpt = 0; extiCfgOpt < EXTI_CFG_CNT; extiCfgOpt++) {
        if ((extiPinoutTab[ctx.config.extiNum][extiCfgOpt][0] == ctx.config.pinPort) &&
            (extiPinoutTab[ctx.config.extiNum][extiCfgOpt][1] == ctx.config.pinNum)) {
            matchFound = true;
            break;
        }
    }

    ASSERT(matchFound == true, "INVALID PORT OR PIN NUMBER");

    //-----------------------------------------------------------------------------------
    // Check it GPIO peripheral clock

    CheckGpioClk(ctx.config.pinPort);

    //-----------------------------------------------------------------------------------
    // Get GPIO port address

    ctx.gpioRegAddr_p = GPIO::GetGpioRegBaseAddr(ctx.config.pinPort);

    //-----------------------------------------------------------------------------------
    // Set pin mode & pin alt. function

    hal::GPIO::SetPinMode(ctx.config.pinPort, ctx.config.pinNum, ctx.config.pinMode);

    //-----------------------------------------------------------------------------------
    // If pin mode 'MODE_ALTFUNC' set pin alt. function type

    hal::GPIO::SetPinAltFunc(ctx.config.pinPort, ctx.config.pinNum, ctx.config.pinFunc);

    //-----------------------------------------------------------------------------------
    // Config pin pull direction

    hal::GPIO::SetPinPullDir(ctx.config.pinPort, ctx.config.pinNum, ctx.config.pullDir);

    //-----------------------------------------------------------------------------------
    // Mark pin usage

    hal::GPIO::SetPinInUse(ctx.config.pinPort, ctx.config.pinNum, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Config irq line

    irqId_t extiIrqId;

    if (ctx.config.pinNum < hal::PIN_4) {
        REG_SYSCFG->EXTICR1.val &= ~(0b11 << ((ctx.config.pinNum % 4) * 4));
        REG_SYSCFG->EXTICR1.val |= (ctx.config.pinPort << ((ctx.config.pinNum % 4) * 4));
    }
    else if (ctx.config.pinNum < hal::PIN_8) {
        REG_SYSCFG->EXTICR2.val &= ~(0b11 << ((ctx.config.pinNum % 4) * 4));
        REG_SYSCFG->EXTICR2.val |= (ctx.config.pinPort << ((ctx.config.pinNum % 4) * 4));
    }
    else if (ctx.config.pinNum < hal::PIN_12) {
        REG_SYSCFG->EXTICR3.val &= ~(0b11 << ((ctx.config.pinNum % 4) * 4));
        REG_SYSCFG->EXTICR3.val |= (ctx.config.pinPort << ((ctx.config.pinNum % 4) * 4));
    }
    else {
        REG_SYSCFG->EXTICR4.val &= ~(0b11 << ((ctx.config.pinNum % 4) * 4));
        REG_SYSCFG->EXTICR4.val |= (ctx.config.pinPort << ((ctx.config.pinNum % 4) * 4));
    }

    if (ctx.config.extiNum <= hal::irqExti::EXTI_4) {
        extiIrqId = (hal::irqId_t)((int8)hal::IRQ_EXTI0 + (int8)ctx.config.extiNum);
    }
    else if (ctx.config.extiNum <= hal::irqExti::EXTI_9) {
        extiIrqId = hal::IRQ_EXTI9_5;
    }
    else {
        extiIrqId = hal::IRQ_EXTI15_10;
    }

    SetIntType(ctx.config.intType);
    hal::core::IrqSetPrio(extiIrqId, ctx.config.irqPrio);
    hal::core::IrqEnable(extiIrqId);
    hal::irq::SetIrqInUse(extiIrqId, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Assign pin interrupt handler callback

    CHECK_EXEC(hal::irqExti::SetIrqClbk(ctx.config.extiNum, ctx.config.IrqClbk));

    //-----------------------------------------------------------------------------------

    if (ctx.config.enable)  Enable();
    else                    Disable();
    ctx.status = Status::OK;
    return Exec::OK;
}

void EXTI::Enable()
{
    CHECK_STATUS();
    REG_EXTI->PR1.val |= 1 << ctx.config.extiNum;       // Clear IRQ flag
    REG_EXTI->IMR1.val |= 1 << ctx.config.extiNum;      // Unmask IRQ flag
    ctx.config.enable = true;
}

void EXTI::Disable()
{
    CHECK_STATUS();
    REG_EXTI->IMR1.val &= ~(1 << ctx.config.extiNum);   // Mask IRQ flag
    ctx.config.enable = false;
}

Exec EXTI::ChangeClbk(IrqClbk_t Clbk)
{
    CHECK_STATUS();
    ASSERT(Clbk != nullptr, "EXTI IRQ CALLBACK");
    CHECK_EXEC(hal::irqExti::SetIrqClbk(ctx.config.extiNum, Clbk));
    ctx.config.IrqClbk = Clbk;
    return Exec::OK;
}

void EXTI::ChangeIntType(intType_t intType)
{
    CHECK_STATUS();
    SetIntType(intType);
}

void EXTI::SetPullDir(hal::GPIO::PullDir pullDir)
{
    CHECK_STATUS();
    hal::GPIO::SetPinPullDir(ctx.config.pinPort, ctx.config.pinNum, pullDir);
    ctx.config.pullDir = pullDir;
}

bool EXTI::GetLogicLevel()
{
    return ctx.gpioRegAddr_p->IDR.val & (1 << ctx.config.pinNum);
}

Status EXTI::GetStatus()
{
    return ctx.status;
}

/******************************************************************************************************************************************
                                                        PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

void EXTI::SetIntType(intType_t intType) {
    if (intType == INT_EDGE) {
        REG_EXTI->FTSR1.val |= 1 << ctx.config.extiNum;         // Enable falling edge detection
        REG_EXTI->RTSR1.val |= 1 << ctx.config.extiNum;         // Enable rising edge detection
    }
    else if (intType == INT_FALLING_EDGE) {
        REG_EXTI->FTSR1.val |= 1 << ctx.config.extiNum;         // Enable falling edge detection
        REG_EXTI->RTSR1.val &= ~(1 << ctx.config.extiNum);      // Disable rising edge detection
    } else {
        REG_EXTI->RTSR1.val |= 1 << ctx.config.extiNum;         // Enable rising edge detection
        REG_EXTI->FTSR1.val &= ~(1 << ctx.config.extiNum);      // Disable falling edge detection
    }
}

void EXTI::CheckGpioClk(hal::pinPort_t pinPort)
{
    hal::perId_t gpioPerId = (hal::perId_t)((int8)hal::PER_GPIOA + (int8)pinPort);

    if (hal::clk::PerClkState(gpioPerId) == State::OFF) {
        hal::clk::PerClkEnable(gpioPerId);
    }
}

} // END OF NAMESPACE 'hal::exti'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
