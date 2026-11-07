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
// CREATION DATE:    April 2024
// DESCRIPTION:      List of in use IRQ's

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "stm32wleTypes.h"
#include "irqList.h"
#include "mcuCore.h"
#include "utils.h"

/******************************************************************************************************************************************
                                                      PRIVATE DEFINES, TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::irq {

#undef CODE_LABEL
#define CODE_LABEL  "IRQ LIST"

static constexpr uint8 LABEL_MAX_LEN = 25;

struct {
    irqInfo_t*  irqList_p = nullptr;
    uint8       irqInUseCnt = 0;
} static ctx = {};

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITION's
******************************************************************************************************************************************/

void SetIrqInUse(hal::irqId_t irqId, const char* label_p)
{
    ASSERT(label_p != nullptr, "IRQ CODE_LABEL STRING");
    ASSERT(*label_p > 0, "IRQ CODE_LABEL STRING STARTS WITH ZERO");

    uint8 labelLen = (uint8)utils::CalcStrLength(label_p, true);
    ASSERT(labelLen <= LABEL_MAX_LEN, "IRQ CODE_LABEL TOO LONG");

    irqInfo_t** irqInfo_pp = &ctx.irqList_p;

    if (ctx.irqList_p != nullptr) {
        while ((*irqInfo_pp)->nextIrq_p != nullptr) {
            irqInfo_pp = &((*irqInfo_pp)->nextIrq_p);
        }
        irqInfo_pp = &((*irqInfo_pp)->nextIrq_p);
    }

    *irqInfo_pp = (irqInfo_t*)calloc(1, sizeof(irqInfo_t));
    ASSERT(*irqInfo_pp != nullptr, "MEM ALLOC FOR LIST ELEMENT FAILED");
    (*irqInfo_pp)->irqId = irqId;
    (*irqInfo_pp)->irqPrio = hal::core::IrqGetPrio(irqId);
    (*irqInfo_pp)->irqEn = hal::core::IrqGetEnable(irqId);
    (*irqInfo_pp)->label_p = label_p;
    (*irqInfo_pp)->nextIrq_p = nullptr;
    ctx.irqInUseCnt++;
}

const irqInfo_t* GetIrqList()
{
    if (ctx.irqList_p == nullptr) return nullptr;
    irqInfo_t* irqInfo_p = ctx.irqList_p;
    while(1) {
        irqInfo_p->irqPrio = hal::core::IrqGetPrio(irqInfo_p->irqId);
        irqInfo_p->irqEn = hal::core::IrqGetEnable(irqInfo_p->irqId);
        if (irqInfo_p->nextIrq_p == nullptr) break;
        irqInfo_p = irqInfo_p->nextIrq_p;
    }
    return ctx.irqList_p;
}

} // End of namespace 'hal::irq'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
