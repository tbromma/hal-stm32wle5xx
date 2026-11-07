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
// CREATION DATE:    March 2023
// DESCRIPTION:      HAL layer for STM32WLE5 GPIO peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#pragma once
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"
#include "common.h"
#include "regGpio.h"
#include "timer.h"
#include "irqExti.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal {

class GPIO {
public:

    enum class PinMode : int8 {
        None = -1,
        Input = 0,
        Output,
        AltFunc,
        Analog,
    };

    enum class ActiveLvl : int8 {
        None = -1,
        Low = 0,
        High = 1,
    };

    enum class OutType : int8 {
        None = -1,
        PushPull = 0,
        OpenDrain = 1,
    };

    enum class OutSpeed : int8 {
        None = -1,
        Slow = 0b00,
        Medium = 0b01,
        Fast = 0b10,
        High = 0b11,
    };

    enum class PullDir {
        None = 0b00,
        Vcc = 0b01,
        Gnd = 0b10,
    };

    enum irqType_t : int8 {
        IRQ_none = -1,
        IRQ_EDGE_BOTH = 0,
        IRQ_EDGE_RISING,
        IRQ_EDGE_FALLING,
    };

    using IrqClbk_t = void (*)();   // GPIO IRQ callback

    struct pinStatus_t {
        bool inUse = false;
        PinMode mode = PinMode::None;
        const char* label_p = nullptr;
    };

    static void         EnablePortClk(hal::pinPort_t pinPort);
    static void         DisablePortClk(hal::pinPort_t pinPort);
    static void         SetPinMode(hal::pinPort_t pinPort, hal::pinNum_t pinNum, PinMode pinMode);
    static void         SetPinAltFunc(hal::pinPort_t pinPort, hal::pinNum_t pinNum, hal::pinFunc_t pinFunc);
    static void         SetPinOutLevel(hal::pinPort_t pinPort, hal::pinNum_t pinNum, bool level);
    static void         SetPinOutType(hal::pinPort_t pinPort, hal::pinNum_t pinNum, OutType outType);
    static void         SetPinOutSpeed(hal::pinPort_t pinPort, hal::pinNum_t pinNum, OutSpeed outSpeed);
    static void         SetPinPullDir(hal::pinPort_t pinPort, hal::pinNum_t pinNum, PullDir pullDir);
    static Exec         SetPinInUse(hal::pinPort_t pinPort, hal::pinNum_t pinNum, const char* pinLabel...);
    static pinStatus_t  GetPinStatus(hal::pinPort_t pinPort, hal::pinNum_t pinNum);
    static volatile REG_GPIO* GetGpioRegBaseAddr(hal::pinPort_t portNum);

    // DIGITAL OUTPUT
    class OUT {
    public:

        struct config_t {
            hal::pinPort_t  pinPort = hal:: PORT_none;
            hal::pinNum_t   pinNum = hal::PIN_none;
            ActiveLvl       activeLvl = ActiveLvl::None;
            OutType         outType = OutType::None;
            OutSpeed        speed = OutSpeed::None;
            PullDir         pullDir = PullDir::None;
            const char*     label_p = nullptr;
            bool            enable = false;
        };

        Exec        Setup(config_t config);
        void        Enable();
        void        Disable();
        void        Set(void);
        void        Clear(void);
        void        Toggle(void);
        bool        GetSoftwareState();
        bool        GetHardwareState();
        Status      GetStatus();
        config_t    GetConfig();

    private:

        struct {
            config_t            config;
            Status              status = Status::NONE;
            bool                pinHardwareState;
            bool                pinSoftwareState;
            volatile REG_GPIO*  gpioRegAddr_p;
        } ctx = {};
    };

    // DIGITAL INPUT
    class IN {
    public:

        struct config_t {
            hal::pinPort_t  pinPort = hal:: PORT_none;
            hal::pinNum_t   pinNum = hal::PIN_none;
            ActiveLvl       activeLvl = ActiveLvl::None;
            PullDir         pullDir = PullDir::None;
            hal::pinFunc_t  pinFunc = hal::FUNC_none;
            uint32          debTimeMs = 0;                                      // Input debounce time in miliseconds
            struct {
                irqType_t               irqType = IRQ_none;
                hal::irqExti::extiNum_t irqNum = hal::irqExti::EXTI_none;       // External interrupt line number
                hal::irqPrio_t          irqPrio = hal::IRQ_PRIO_none;
                IrqClbk_t               IrqClbk = nullptr;                      // Interrupt callback function
            } irq = {};
            const char*     label_p = nullptr;                                  // Optional description label
        };

        Exec        Setup(config_t config);
        void        IrqEnable();
        void        IrqDisable();
        Exec        ChangeIrqClbk(IrqClbk_t Clbk);
        void        SetPullDir(PullDir pullDir);
        void        SetDebouceTime(uint8 timeMs);
        bool        GetSoftwareState();
        bool        GetHardwareState();
        Status      GetStatus();
        config_t    GetConfig();

    private:

        Exec SetupPinIrq();
        void SetIntType(irqType_t intType);
        void UpdateState();

        struct {
            config_t            config;
            Status              status = Status::NONE;
            bool                hardwareState;
            bool                softwareState;
            bareOs::Timer*      timerDebounce_p = nullptr;
            volatile REG_GPIO*  gpioRegAddr_p;
        } ctx = {};
    };
};

} // END OF NAMESPACE 'hal::gpio'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
