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
// DESCRIPTION:      UART peripheral interrupt assignment

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "irqI2c.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::irqI2c {

#undef CODE_LABEL
#define CODE_LABEL "IRQ I2C"

enum irqType_t {
    IRQ_EVT,
    IRQ_ERR,
};

inline const uint8* irqTypeStr[] = {
    (uint8*)"EVT",
    (uint8*)"ERR",
};

static IrqClbk_t EvtClbkTab[hal::I2C_count];
static IrqClbk_t ErrClbkTab[hal::I2C_count];

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/

Exec SetEvtIrqClbk(hal::i2cNum_t i2cNum, IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "I2C%d EVENT IRQ CALLBACK", (uint8)i2cNum);
    EvtClbkTab[i2cNum] = Clbk;
    return Exec::OK;
}

Exec SetErrIrqClbk(hal::i2cNum_t i2cNum, IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "I2C%d ERROR IRQ CALLBACK", (uint8)i2cNum);
    ErrClbkTab[i2cNum] = Clbk;
    return Exec::OK;
}

} // End of namespace 'hal::irqI2c'
/******************************************************************************************************************************************
                                                             INTERRUPT HANDLERS
******************************************************************************************************************************************/

extern "C" {
    void I2C1_EV_IRQHandler();
    void I2C1_ER_IRQHandler();
    void I2C2_EV_IRQHandler();
    void I2C2_ER_IRQHandler();
    void I2C3_EV_IRQHandler();
    void I2C3_ER_IRQHandler();
}

void ErrorHandler(hal::i2cNum_t i2cNum, hal::irqI2c::irqType_t irqType)
{
    REPORT_FAT("I2C%d %s IRQ CALLBACK", i2cNum + 1, hal::irqI2c::irqTypeStr[irqType]);
}

void I2C1_EV_IRQHandler()
{
    if (hal::irqI2c::EvtClbkTab[hal::I2C_1])    hal::irqI2c::EvtClbkTab[hal::I2C_1](hal::I2C_1);
    else                                        ErrorHandler(hal::I2C_1, hal::irqI2c::IRQ_EVT);
}

void I2C1_ER_IRQHandler()
{
    if (hal::irqI2c::ErrClbkTab[hal::I2C_1])    hal::irqI2c::ErrClbkTab[hal::I2C_1](hal::I2C_1);
    else                                        ErrorHandler(hal::I2C_1, hal::irqI2c::IRQ_ERR);
}

void I2C2_EV_IRQHandler()
{
    if (hal::irqI2c::EvtClbkTab[hal::I2C_2])    hal::irqI2c::EvtClbkTab[hal::I2C_2](hal::I2C_2);
    else                                        ErrorHandler(hal::I2C_2, hal::irqI2c::IRQ_EVT);
}

void I2C2_ER_IRQHandler()
{
    if (hal::irqI2c::ErrClbkTab[hal::I2C_2])    hal::irqI2c::ErrClbkTab[hal::I2C_2](hal::I2C_2);
    else                                        ErrorHandler(hal::I2C_2, hal::irqI2c::IRQ_ERR);
}

void I2C3_EV_IRQHandler()
{
    if (hal::irqI2c::EvtClbkTab[hal::I2C_3])    hal::irqI2c::EvtClbkTab[hal::I2C_3](hal::I2C_3);
    else                                        ErrorHandler(hal::I2C_3, hal::irqI2c::IRQ_EVT);
}

void I2C3_ER_IRQHandler()
{
    if (hal::irqI2c::ErrClbkTab[hal::I2C_3])    hal::irqI2c::ErrClbkTab[hal::I2C_3](hal::I2C_3);
    else                                        ErrorHandler(hal::I2C_3, hal::irqI2c::IRQ_ERR);
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
