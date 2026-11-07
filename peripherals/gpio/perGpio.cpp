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
// CREATION DATE:    March 2023
// DESCRIPTION:      HAL layer for STM32WLE5 GPIO peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "perGpio.h"
#include "regSyscfg.h"
#include "regExti.h"
#include "sysclk.h"
#include "mcuCore.h"
#include "utils.h"
#include "irqList.h"
#include "perExti.h"

/******************************************************************************************************************************************
                                                      PRIVATE DEFINES, TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal {

#undef ACTION_ON_ERR
#undef CODE_LABEL
#undef CODE_STATUS
#define ACTION_ON_ERR   ctx.status = Status::ERR
#define CODE_LABEL      ctx.config.label_p
#define CODE_STATUS     ctx.status

struct {
    struct {
        struct __packed {
            bool inUse = false;
            const char* label_p = nullptr;
        } pin[hal::PIN_count];
    } port[hal::PORT_count];
} pinStat;

static constexpr const char* portStr[] = {
    "GPIOA",
    "GPIOB",
    "GPIOC",
    "GPIOH",
};

static constexpr uint8 EXTI_CFG_CNT = 4;    // Possible options to assign the EXTI line to a hardware pin

static constexpr int8 extiPinoutTab[hal::irqExti::EXTI_count][EXTI_CFG_CNT][2] = {
    { {hal::PORT_A, hal::PIN_0}, {hal::PORT_B, hal::PIN_0}, {hal::PORT_C, hal::PIN_0}, {hal::PORT_none, hal::PIN_none} },           // EXTI0
    { {hal::PORT_A, hal::PIN_1}, {hal::PORT_B, hal::PIN_1}, {hal::PORT_C, hal::PIN_1}, {hal::PORT_none, hal::PIN_none} },           // EXTI1
    { {hal::PORT_A, hal::PIN_2}, {hal::PORT_B, hal::PIN_2}, {hal::PORT_C, hal::PIN_2}, {hal::PORT_none, hal::PIN_none} },           // EXTI2
    { {hal::PORT_A, hal::PIN_3}, {hal::PORT_B, hal::PIN_3}, {hal::PORT_C, hal::PIN_3}, {hal::PORT_none, hal::PIN_none} },           // EXTI3

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
                                                    PUBLIC FUNCTION DEFINITIONS - GENERAL
******************************************************************************************************************************************/

void GPIO::EnablePortClk(hal::pinPort_t pinPort)
{
    hal::perId_t gpioPerId = (hal::perId_t)((int8)hal::PER_GPIOA + (int8)pinPort);

    if (hal::clk::PerClkState(gpioPerId) == State::OFF) {
        hal::clk::PerClkEnable(gpioPerId);
    }
}

void GPIO::DisablePortClk(hal::pinPort_t pinPort)
{
    hal::perId_t gpioPerId = (hal::perId_t)((int8)hal::PER_GPIOA + (int8)pinPort);

    if (hal::clk::PerClkState(gpioPerId) == State::ON) {
        hal::clk::PerClkDisable(gpioPerId);
    }
}

void GPIO::SetPinMode(hal::pinPort_t pinPort, hal::pinNum_t pinNum, PinMode pinMode)
{
    volatile REG_GPIO* gpioAddr_p = GetGpioRegBaseAddr(pinPort);
    gpioAddr_p->MODER.val &= ~(0b11 << (pinNum * 2));
    gpioAddr_p->MODER.val |= (uint8)pinMode << (pinNum * 2);
}

void GPIO::SetPinAltFunc(hal::pinPort_t pinPort, hal::pinNum_t pinNum, hal::pinFunc_t pinFunc)
{
    volatile REG_GPIO* gpioAddr_p = GetGpioRegBaseAddr(pinPort);

    if (pinFunc == hal::FUNC_none) pinFunc = hal::FUNC_AF0;

    if (pinNum <= hal::PIN_7) {
        gpioAddr_p->AFR[0].val &= ~(0b1111 << (pinNum * 4));
        gpioAddr_p->AFR[0].val |= pinFunc << (pinNum * 4);
    }
    else {
        gpioAddr_p->AFR[1].val &= ~(0b1111 << ((pinNum - 8) * 4));
        gpioAddr_p->AFR[1].val |= pinFunc << ((pinNum - 8) * 4);
    }
}

void GPIO::SetPinOutLevel(hal::pinPort_t pinPort, hal::pinNum_t pinNum, bool level)
{
    volatile REG_GPIO* gpioAddr_p = GetGpioRegBaseAddr(pinPort);
    if (level)  gpioAddr_p->BSRR.val = 1 << pinNum;
    else        gpioAddr_p->BSRR.val = 1 << (pinNum + 16);
}

void GPIO::SetPinOutType(hal::pinPort_t pinPort, hal::pinNum_t pinNum, OutType outType)
{
    volatile REG_GPIO* gpioAddr_p = GetGpioRegBaseAddr(pinPort);
    if (outType == OutType::PushPull)   gpioAddr_p->OTYPER.val &= ~(1 << pinNum);
    else                                gpioAddr_p->OTYPER.val |= 1 << pinNum;
}

void GPIO::SetPinOutSpeed(hal::pinPort_t pinPort, hal::pinNum_t pinNum, OutSpeed outSpeed)
{
    volatile REG_GPIO* gpioAddr_p = GetGpioRegBaseAddr(pinPort);
    gpioAddr_p->OSPEEDR.val &= ~(0b11 << (pinNum * 2));
    gpioAddr_p->OSPEEDR.val |= (uint8)outSpeed << (pinNum * 2);
}

void GPIO::SetPinPullDir(hal::pinPort_t pinPort, hal::pinNum_t pinNum, PullDir pullDir)
{
    volatile REG_GPIO* gpioAddr_p = GetGpioRegBaseAddr(pinPort);
    gpioAddr_p->PUPDR.val &= ~(0b11 << (pinNum * 2));
    gpioAddr_p->PUPDR.val |= (uint8)pullDir << (pinNum * 2);
}

Exec GPIO::SetPinInUse(hal::pinPort_t pinPort, hal::pinNum_t pinNum, const char* pinLabelArgs...)
{
    //-----------------------------------------------------------------------------------
    // Get pin label

    char pinLabel[50] = {};
    va_list args;
    va_start(args, pinLabelArgs);
    vsprintf((char*)pinLabel, pinLabelArgs, args);
    va_end(args);

    //-----------------------------------------------------------------------------------
    // Check if pin not already in use

    if (pinStat.port[pinPort].pin[pinNum].inUse) {
        struct { struct { const char* label_p = "GPIO"; } config; } ctx;
        REPORT_FAT("PORT: %s, PIN: %d, NEW FUNC: %s, OLD FUNC: %s",
                    portStr[pinPort], pinNum, pinLabel, pinStat.port[pinPort].pin[pinNum].label_p);
        return Exec::ERR;
    }

    //-----------------------------------------------------------------------------------
    // Save pin usage info

    pinStat.port[pinPort].pin[pinNum].label_p = pinLabel;
    pinStat.port[pinPort].pin[pinNum].inUse = true;
    return Exec::OK;
}

GPIO::pinStatus_t GPIO::GetPinStatus(hal::pinPort_t pinPort, hal::pinNum_t pinNum)
{
    pinStatus_t pin = {};
    pin.inUse = pinStat.port[pinPort].pin[pinNum].inUse;

    if (pinStat.port[pinPort].pin[pinNum].inUse) {
        pin.inUse = true;
        pin.label_p = pinStat.port[pinPort].pin[pinNum].label_p;
        pin.mode = (PinMode)((GetGpioRegBaseAddr(pinPort)->MODER.val >> (pinNum * 2)) & 0b11);
    }

    return pin;
}

/******************************************************************************************************************************************
                                                             GPIO-OUT
******************************************************************************************************************************************/

Exec GPIO::OUT::Setup(config_t config)
{
    ASSERT(ctx.status == Status::NONE, "PROBABLY GPIO ALLREADY INITIALIZED");
    ASSERT(config.pinNum != hal::PIN_none, "PIN NUMBER");
    ASSERT(config.pinPort != hal::PORT_none, "PORT NUMBER");
    ASSERT(config.activeLvl != ActiveLvl::None, "ACTIVE LEVEL");
    ASSERT(config.outType != OutType::None, "OUTPUT TYPE");

    ctx.status = Status ::INIT;
    ctx.config = config;

    //-----------------------------------------------------------------------------------
    // Check it GPIO peripheral clock & get GPIO peripheral base address

    EnablePortClk(ctx.config.pinPort);
    ctx.gpioRegAddr_p = GetGpioRegBaseAddr(ctx.config.pinPort);

    //-----------------------------------------------------------------------------------
    // Config pin pull direction

    SetPinPullDir(ctx.config.pinPort, ctx.config.pinNum, ctx.config.pullDir);

    //-----------------------------------------------------------------------------------
    // Config pin output mode

    SetPinOutType(ctx.config.pinPort, ctx.config.pinNum, ctx.config.outType);

    //-----------------------------------------------------------------------------------
    // Config pin speed mode

    SetPinOutSpeed(ctx.config.pinPort, ctx.config.pinNum, ctx.config.speed);

    //-----------------------------------------------------------------------------------
    // Mark pin usage

    SetPinInUse(ctx.config.pinPort, ctx.config.pinNum, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Clear and enable pin

    Clear();
    if (ctx.config.enable) Enable();

    //-----------------------------------------------------------------------------------

    ctx.status = Status::OK;
    return Exec::OK;
}

void GPIO::OUT::Enable()
{
    CHECK_STATUS();
    ctx.gpioRegAddr_p->MODER.val &= ~(0b11 << (ctx.config.pinNum * 2));
    ctx.gpioRegAddr_p->MODER.val |= 0b01 << (ctx.config.pinNum * 2);
}

void GPIO::OUT::Disable()
{
    CHECK_STATUS();
    ctx.gpioRegAddr_p->MODER.val &= ~(0b11 << (ctx.config.pinNum * 2));
}

void GPIO::OUT::Set()
{
    CHECK_STATUS();

    //-----------------------------------------------------------------------------------
    // Set output

    if (ctx.config.activeLvl == ActiveLvl::High)    ctx.gpioRegAddr_p->BSRR.val = 1 << ctx.config.pinNum;
    else                                            ctx.gpioRegAddr_p->BSRR.val = 1 << (ctx.config.pinNum + 16);

    //-----------------------------------------------------------------------------------
    // Update pin hardware and software state

    ctx.pinHardwareState = (bool)(ctx.gpioRegAddr_p->ODR.val >> ctx.config.pinNum);
    ctx.pinSoftwareState = 1;
}

void GPIO::OUT::Clear()
{
    CHECK_STATUS();

    //-----------------------------------------------------------------------------------
    // Set output

    if (ctx.config.activeLvl == ActiveLvl::High)    ctx.gpioRegAddr_p->BSRR.val = 1 << (ctx.config.pinNum + 16);
    else                                            ctx.gpioRegAddr_p->BSRR.val = 1 << ctx.config.pinNum;

    //-----------------------------------------------------------------------------------
    // Update pin hardware and software state

    ctx.pinHardwareState = (bool)(ctx.gpioRegAddr_p->ODR.val >> ctx.config.pinNum);
    ctx.pinSoftwareState = 0;
}

void GPIO::OUT::Toggle()
{
    CHECK_STATUS();
    if (ctx.pinSoftwareState == 0)  Set();
    else                            Clear();
}

bool GPIO::OUT::GetSoftwareState()
{
    CHECK_STATUS();
    return ctx.pinSoftwareState;
}

bool GPIO::OUT::GetHardwareState()
{
    CHECK_STATUS();
    return ctx.pinHardwareState;
}

Status GPIO::OUT::GetStatus()
{
    return ctx.status;
}

GPIO::OUT::config_t GPIO::OUT::GetConfig()
{
    return ctx.config;
}

/******************************************************************************************************************************************
                                                              GPIO-INPUT
******************************************************************************************************************************************/

Exec GPIO::IN::Setup(config_t config)
{
    ctx.status = Status ::INIT;

    //-----------------------------------------------------------------------------------
    // Perform config check

    ASSERT(config.pinNum != hal::PIN_none, "PIN NUMBER");
    ASSERT(config.pinPort != hal::PORT_none, "PORT NUMBER");

    if (config.irq.irqNum != hal::irqExti::EXTI_none) {
        ASSERT(config.irq.irqNum != hal::irqExti::EXTI_none, "EXTI LINE NUMBER");
        ASSERT(config.irq.irqType != IRQ_none, "IRQ TYPE");
        ASSERT(config.irq.irqPrio != hal::IRQ_PRIO_none, "IRQ PRIORITY");
        ASSERT(config.irq.IrqClbk != nullptr, "IRQ CALLBACK");
        ASSERT(config.label_p != nullptr, "CODE_LABEL STRING");
    }

    ctx.config = config;

    //-----------------------------------------------------------------------------------
    // Check it GPIO peripheral clock & get GPIO peripheral base address

    EnablePortClk(ctx.config.pinPort);
    ctx.gpioRegAddr_p = GetGpioRegBaseAddr(ctx.config.pinPort);

    //-----------------------------------------------------------------------------------
    // Set pin as input

    ctx.gpioRegAddr_p->MODER.val &= ~(0b11 << (ctx.config.pinNum * 2));

    //-----------------------------------------------------------------------------------
    // Config pin pull direction

    SetPullDir(ctx.config.pullDir);

    //-----------------------------------------------------------------------------------
    // If pin mode 'MODE_ALTFUNC' set pin alt. function type

    hal::GPIO::SetPinAltFunc(ctx.config.pinPort, ctx.config.pinNum, ctx.config.pinFunc);

    //-----------------------------------------------------------------------------------
    // Config irq line

    if (ctx.config.irq.irqType != IRQ_none) SetupPinIrq();

    //-----------------------------------------------------------------------------------
    // Mark pin usage

    SetPinInUse(ctx.config.pinPort, ctx.config.pinNum, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Setup debounce timer

    if (ctx.config.debTimeMs) {
        ctx.timerDebounce_p = (bareOs::Timer*)calloc(1, sizeof(bareOs::Timer));
        ASSERT(ctx.timerDebounce_p != nullptr, "Memory allocation failed!");
    }

    ctx.timerDebounce_p->Config(bareOs::Timer::ModeEndless, bareOs::Timer::UnitUs, ctx.config.debTimeMs);
    ctx.timerDebounce_p->Enable(true, false);

    //-----------------------------------------------------------------------------------

    ctx.status = Status::OK;
    return Exec::OK;
}

void GPIO::IN::SetPullDir(PullDir pullDir)
{
    CHECK_STATUS();
    ctx.gpioRegAddr_p->PUPDR.val &= ~(0b11 << (ctx.config.pinNum * 2));
    if (pullDir != PullDir::None) {
        ctx.gpioRegAddr_p->PUPDR.val |= (uint8)ctx.config.pullDir << (ctx.config.pinNum * 2);
    }
}

void GPIO::IN::SetDebouceTime(uint8 timeMs)
{
    CHECK_STATUS();
    if (ctx.timerDebounce_p == nullptr && timeMs) {
        ctx.timerDebounce_p = (bareOs::Timer*)calloc(1, sizeof(bareOs::Timer));
        ASSERT(ctx.timerDebounce_p != nullptr, "Memory allocation failed!");
        ctx.timerDebounce_p->Config(bareOs::Timer::ModeEndless, bareOs::Timer::UnitUs);
        ctx.timerDebounce_p->Enable(true, false);
    }
    ctx.config.debTimeMs = timeMs;
    ctx.timerDebounce_p->ChangeInterval(ctx.config.debTimeMs);
}

void GPIO::IN::IrqEnable()
{
    CHECK_STATUS();
    REG_EXTI->PR1.val |= 1 << ctx.config.irq.irqNum;       // Clear IRQ flag
    REG_EXTI->IMR1.val |= 1 << ctx.config.irq.irqNum;      // Unmask IRQ flag
}

void GPIO::IN::IrqDisable()
{
    CHECK_STATUS();
    REG_EXTI->IMR1.val &= ~(1 << ctx.config.irq.irqNum);   // Mask IRQ flag
}

Exec GPIO::IN::ChangeIrqClbk(IrqClbk_t Clbk)
{
    CHECK_STATUS();
    ASSERT(Clbk != nullptr, "EXTI IRQ CALLBACK");
    CHECK_EXEC(hal::irqExti::SetIrqClbk(ctx.config.irq.irqNum, Clbk));
    ctx.config.irq.IrqClbk = Clbk;
    return Exec::OK;
}

bool GPIO::IN::GetHardwareState()
{
    CHECK_STATUS();
    UpdateState();
    return ctx.hardwareState;
}

bool GPIO::IN::GetSoftwareState()
{
    CHECK_STATUS();
    UpdateState();
    return ctx.softwareState;
}

Status GPIO::IN::GetStatus()
{
    return ctx.status;
}

GPIO::IN::config_t GPIO::IN::GetConfig()
{
    return ctx.config;
}

void GPIO::IN::UpdateState()
{
    //-----------------------------------------------------------------------------------
    // Get pin hardware state

    ctx.hardwareState = ctx.gpioRegAddr_p->IDR.val & (1 << ctx.config.pinNum);

    //-----------------------------------------------------------------------------------
    // Update pin software state

    if (ctx.hardwareState == (bool)ctx.config.activeLvl) {
        if (ctx.config.debTimeMs > 0) {
             if (ctx.timerDebounce_p->Check())  ctx.softwareState = 1;
             else                               ctx.softwareState = 0;
        }
        else ctx.softwareState = 1;
    }
    else {
        ctx.softwareState = 0;
        ctx.timerDebounce_p->Reset();
    }
}

/******************************************************************************************************************************************
                                                         PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

volatile REG_GPIO* GPIO::GetGpioRegBaseAddr(hal::pinPort_t portNum)
{
    if      (portNum == hal::PORT_A)    return REG_GPIOA;
    else if (portNum == hal::PORT_B)    return REG_GPIOB;
    else if (portNum == hal::PORT_C)    return REG_GPIOC;
    else if (portNum == hal::PORT_H)    return REG_GPIOH;
    else                                return nullptr;
}

Exec GPIO::IN::SetupPinIrq()
{
    //-----------------------------------------------------------------------------------
    // Verify pin port and pin number

    bool matchFound = false;

    for (uint8 extiCfgOpt = 0; extiCfgOpt < EXTI_CFG_CNT; extiCfgOpt++) {
        if ((extiPinoutTab[ctx.config.irq.irqNum][extiCfgOpt][0] == ctx.config.pinPort) &&
            (extiPinoutTab[ctx.config.irq.irqNum][extiCfgOpt][1] == ctx.config.pinNum)) {
            matchFound = true;
            break;
        }
    }

    ASSERT(matchFound == true, "INVALID PORT OR PIN NUMBER");

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

    if (ctx.config.irq.irqNum <= hal::irqExti::EXTI_4) {
        extiIrqId = (hal::irqId_t)((int8)hal::IRQ_EXTI0 + (int8)ctx.config.irq.irqNum);
    }
    else if (ctx.config.irq.irqNum <= hal::irqExti::EXTI_9) {
        extiIrqId = hal::IRQ_EXTI9_5;
    }
    else {
        extiIrqId = hal::IRQ_EXTI15_10;
    }

    SetIntType(ctx.config.irq.irqType);
    hal::core::IrqSetPrio(extiIrqId, ctx.config.irq.irqPrio);
    hal::core::IrqEnable(extiIrqId);
    hal::irq::SetIrqInUse(extiIrqId, ctx.config.label_p);

    //-----------------------------------------------------------------------------------
    // Assign pin interrupt handler callback

    CHECK_EXEC(hal::irqExti::SetIrqClbk(ctx.config.irq.irqNum, ctx.config.irq.IrqClbk));

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

void GPIO::IN::SetIntType(irqType_t intType) {
    if (intType == IRQ_EDGE_BOTH) {
        REG_EXTI->FTSR1.val |= 1 << ctx.config.irq.irqNum;         // Enable falling edge detection
        REG_EXTI->RTSR1.val |= 1 << ctx.config.irq.irqNum;         // Enable rising edge detection
    }
    else if (intType == IRQ_EDGE_FALLING) {
        REG_EXTI->FTSR1.val |= 1 << ctx.config.irq.irqNum;         // Enable falling edge detection
        REG_EXTI->RTSR1.val &= ~(1 << ctx.config.irq.irqNum);      // Disable rising edge detection
    } else {
        REG_EXTI->RTSR1.val |= 1 << ctx.config.irq.irqNum;         // Enable rising edge detection
        REG_EXTI->FTSR1.val &= ~(1 << ctx.config.irq.irqNum);      // Disable falling edge detection
    }
}

} // END OF NAMESPACE 'hal::gpio'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
