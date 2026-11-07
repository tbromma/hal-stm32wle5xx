/*******************************************************************************************************************************************
                                                                 FILE INFO
*******************************************************************************************************************************************/

// DESCRIPTION:      DMA peripheral interrupt assignment

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "irqDma.h"
#include "stm32wleTypes.h"

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/
namespace hal::irqDma {

static IrqClbk_t DmaClbkTab[hal::DMA_count][hal::DMA_CH_count];
static hal::perId_t perIDTab[hal::DMA_count][hal::DMA_CH_count];

Exec SetIrqClbk(hal::dmaNum_t dmaNum, hal::dmaCh_t dmaCh, hal::perId_t perID, IrqClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "DMA%d CH%d invalid callback!", dmaNum + 1, dmaCh + 1);
    ASSERT(perID != hal::PER_none, "DMA%d CH%d invalid peripheral ID!", dmaNum + 1, dmaCh + 1);
    DmaClbkTab[dmaNum][dmaCh] = Clbk;
    perIDTab[dmaNum][dmaCh] = perID;
    return Exec::OK;
}

} // End of namespace 'hal::irqDma'

/******************************************************************************************************************************************
                                                             INTERRUPT HANDLERS
******************************************************************************************************************************************/

#undef CODE_LABEL
#define CODE_LABEL  "IRQ DMA"

extern "C" {
    void DMA1_Channel1_IRQHandler();
    void DMA1_Channel2_IRQHandler();
    void DMA1_Channel3_IRQHandler();
    void DMA1_Channel4_IRQHandler();
    void DMA1_Channel5_IRQHandler();
    void DMA1_Channel6_IRQHandler();
    void DMA1_Channel7_IRQHandler();

    void DMA2_Channel1_IRQHandler();
    void DMA2_Channel2_IRQHandler();
    void DMA2_Channel3_IRQHandler();
    void DMA2_Channel4_IRQHandler();
    void DMA2_Channel5_IRQHandler();
    void DMA2_Channel6_IRQHandler();
    void DMA2_Channel7_IRQHandler();
}

static void IrqErrHandler(int8 dmaNum, int8 dmaCh)
{
    REPORT_FAT("DMA%d CH%d IRQ callback is nullpointer!", dmaNum + 1, dmaCh + 1);
}

static void IrqHandler(hal::dmaNum_t dmaNum, hal::dmaCh_t dmaCh)
{
    if (hal::irqDma::DmaClbkTab[dmaNum][dmaCh]) {
        hal::perId_t perID = hal::irqDma::perIDTab[hal::DMA_1][hal::DMA_CH1];
        hal::irqDma::DmaClbkTab[dmaNum][dmaCh](dmaNum, dmaCh, perID);
    }
    else IrqErrHandler(dmaNum, dmaCh);
}

void DMA1_Channel1_IRQHandler() { IrqHandler(hal::DMA_1, hal::DMA_CH1); }
void DMA1_Channel2_IRQHandler() { IrqHandler(hal::DMA_1, hal::DMA_CH2); }
void DMA1_Channel3_IRQHandler() { IrqHandler(hal::DMA_1, hal::DMA_CH3); }
void DMA1_Channel4_IRQHandler() { IrqHandler(hal::DMA_1, hal::DMA_CH4); }
void DMA1_Channel5_IRQHandler() { IrqHandler(hal::DMA_1, hal::DMA_CH5); }
void DMA1_Channel6_IRQHandler() { IrqHandler(hal::DMA_1, hal::DMA_CH6); }
void DMA1_Channel7_IRQHandler() { IrqHandler(hal::DMA_1, hal::DMA_CH7); }

void DMA2_Channel1_IRQHandler() { IrqHandler(hal::DMA_2, hal::DMA_CH1); }
void DMA2_Channel2_IRQHandler() { IrqHandler(hal::DMA_2, hal::DMA_CH2); }
void DMA2_Channel3_IRQHandler() { IrqHandler(hal::DMA_2, hal::DMA_CH3); }
void DMA2_Channel4_IRQHandler() { IrqHandler(hal::DMA_2, hal::DMA_CH4); }
void DMA2_Channel5_IRQHandler() { IrqHandler(hal::DMA_2, hal::DMA_CH5); }
void DMA2_Channel6_IRQHandler() { IrqHandler(hal::DMA_2, hal::DMA_CH6); }
void DMA2_Channel7_IRQHandler() { IrqHandler(hal::DMA_2, hal::DMA_CH7); }

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
