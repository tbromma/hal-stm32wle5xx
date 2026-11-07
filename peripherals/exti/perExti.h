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

// CREATED BY:       T.Bromma (tobiasz.bromma@thoriumspace.com)
// CREATION DATE:    Oktober 2023
// DESCRIPTION:      Low level layer for EXTI (External Interrupt) peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"
#include "common.h"
#include "regGpio.h"
#include "perGpio.h"
#include "irqExti.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::exti {

enum intType_t : int8 {
    INT_none = -1,
    INT_EDGE = 0,
    INT_RISING_EDGE,
    INT_FALLING_EDGE,
};

using IrqClbk_t = void (*)();       // Exti interrupt callback

struct config_t {
    hal::pinPort_t          pinPort = hal::PORT_none;
    hal::pinNum_t           pinNum = hal::PIN_none;
    hal::GPIO::PinMode      pinMode = hal::GPIO::PinMode::None;
    hal::pinFunc_t          pinFunc = hal::FUNC_none;
    hal::irqExti::extiNum_t extiNum = hal::irqExti::EXTI_none;      // External interrupt line number
    hal::GPIO::PullDir      pullDir = hal::GPIO::PullDir::None;
    intType_t               intType = INT_none;
    hal::irqPrio_t          irqPrio = hal::IRQ_PRIO_none;
    IrqClbk_t               IrqClbk = nullptr;                      // Interrupt callback function
    const char*             label_p = nullptr;                      // Optional description label
    bool                    enable = false;                         // Enable EXTI line right after config?
};

/******************************************************************************************************************************************
                                                              PUBLIC CLASSES
******************************************************************************************************************************************/

// DIGITAL OUTPUT
class EXTI {

public:

    Exec    Setup(config_t config);
    void    Enable();
    void    Disable();
    Exec    ChangeClbk(IrqClbk_t Clbk);
    void    ChangeIntType(intType_t intType);
    void    SetPullDir(hal::GPIO::PullDir pullDir);
    bool    GetLogicLevel();
    Status  GetStatus();

private:

    void SetIntType(intType_t intType);
    void CheckGpioClk(hal::pinPort_t pinPort);

    struct {
        config_t config = {};
        Status status = Status::NONE;
        volatile REG_GPIO* gpioRegAddr_p;
    } ctx = {};
};

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::exti'
