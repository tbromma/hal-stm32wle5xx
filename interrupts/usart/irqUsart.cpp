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

#include "irqUsart.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::irqUsart {

static TxfnfClbk_t TxfnfClbkTab[hal::USART_count];
static TxcmpClbk_t TxcmpClbkTab[hal::USART_count];
static RxfneClbk_t RxfneClbkTab[hal::USART_count];
static ParityClbk_t ParityClbkTab[hal::USART_count];
static FrameClbk_t FrameClbkTab[hal::USART_count];
static OreClbk_t OreClbkTab[hal::USART_count];
static RtofClbk_t RtofClbkTab[hal::USART_count];

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/

Exec SetTxfnfClbk(hal::usartNum_t usartNum, TxfnfClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "USART%d CALLBACK", usartNum + 1);
    TxfnfClbkTab[usartNum] = Clbk;
    return Exec::OK;
}

Exec SetTxcmpClbk(hal::usartNum_t usartNum, TxcmpClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "USART%d CALLBACK", usartNum + 1);
    TxcmpClbkTab[usartNum] = Clbk;
    return Exec::OK;
}

Exec SetRxfneClbk(hal::usartNum_t usartNum, RxfneClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "USART%d CALLBACK", usartNum + 1);
    RxfneClbkTab[usartNum] = Clbk;
    return Exec::OK;
}

Exec SetParityClbk(hal::usartNum_t usartNum, ParityClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "USART%d CALLBACK", usartNum + 1);
    ParityClbkTab[usartNum] = Clbk;
    return Exec::OK;
}

Exec SetFrameClbk(hal::usartNum_t usartNum, FrameClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "USART%d CALLBACK", usartNum + 1);
    FrameClbkTab[usartNum] = Clbk;
    return Exec::OK;
}

Exec SetOreClbk(hal::usartNum_t usartNum, OreClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "USART%d CALLBACK", usartNum + 1);
    OreClbkTab[usartNum] = Clbk;
    return Exec::OK;
}

Exec SetRtofClbk(hal::usartNum_t usartNum, RtofClbk_t Clbk)
{
    ASSERT(Clbk != nullptr, "USART%d CALLBACK", usartNum + 1);
    RtofClbkTab[usartNum] = Clbk;
    return Exec::OK;
}

} // End of namespace 'irq::usart'
/******************************************************************************************************************************************
                                                             INTERRUPT HANDLERS
******************************************************************************************************************************************/

extern "C" {
    void USART1_IRQHandler();
    void USART2_IRQHandler();
    void LPUART1_IRQHandler();
}

void USART1_IRQHandler()
{
    USART_ISR ISR_REG;
    USART_CR1 CR1_REG;
    USART_CR3 CR3_REG;
    ISR_REG.val = REG_USART1->ISR.val;
    CR1_REG.val = REG_USART1->CR1.val;
    CR3_REG.val = REG_USART1->CR3.val;
    REG_USART1->ICR.val = 0xFF;
    if (CR1_REG.bit.RXFNEIE & ISR_REG.bit.RXFNE)                    hal::irqUsart::RxfneClbkTab[hal::USART_1](REG_USART1->RDR.bit.RDR);
    if (CR1_REG.bit.TXFNFIE & ISR_REG.bit.TXFNF)                    hal::irqUsart::TxfnfClbkTab[hal::USART_1](hal::USART_1);
    if (CR1_REG.bit.TCIE & ISR_REG.bit.TC)                          hal::irqUsart::TxcmpClbkTab[hal::USART_1](hal::USART_1);
    if (CR1_REG.bit.PEIE & ISR_REG.bit.PE)                          hal::irqUsart::ParityClbkTab[hal::USART_1](hal::USART_1);
    if (CR3_REG.bit.EIE & ISR_REG.bit.FE)                           hal::irqUsart::FrameClbkTab[hal::USART_1](hal::USART_1);
    if (CR1_REG.bit.RTOIE & ISR_REG.bit.RTOF)                       hal::irqUsart::RtofClbkTab[hal::USART_1](hal::USART_1);
}

void USART2_IRQHandler()
{
    USART_ISR ISR_REG;
    USART_CR1 CR1_REG;
    USART_CR3 CR3_REG;
    ISR_REG.val = REG_USART2->ISR.val;
    CR1_REG.val = REG_USART2->CR1.val;
    CR3_REG.val = REG_USART2->CR3.val;
    REG_USART2->ICR.val = 0xFF;
    if (CR1_REG.bit.RXFNEIE & ISR_REG.bit.RXFNE)                    hal::irqUsart::RxfneClbkTab[hal::USART_2](REG_USART2->RDR.bit.RDR);
    if (CR1_REG.bit.TXFNFIE & ISR_REG.bit.TXFNF)                    hal::irqUsart::TxfnfClbkTab[hal::USART_2](hal::USART_2);
    if (CR1_REG.bit.TCIE & ISR_REG.bit.TC)                          hal::irqUsart::TxcmpClbkTab[hal::USART_2](hal::USART_2);
    if (CR1_REG.bit.PEIE & ISR_REG.bit.PE)                          hal::irqUsart::ParityClbkTab[hal::USART_2](hal::USART_2);
    if (CR3_REG.bit.EIE & ISR_REG.bit.FE)                           hal::irqUsart::FrameClbkTab[hal::USART_2](hal::USART_2);
    if (CR1_REG.bit.RTOIE & ISR_REG.bit.RTOF)                       hal::irqUsart::RtofClbkTab[hal::USART_2](hal::USART_2);
}

void LPUART1_IRQHandler()
{
    USART_ISR ISR_REG;
    USART_CR1 CR1_REG;
    USART_CR3 CR3_REG;
    ISR_REG.val = REG_LPUART1->ISR.val;
    CR1_REG.val = REG_LPUART1->CR1.val;
    CR3_REG.val = REG_LPUART1->CR3.val;
    REG_LPUART1->ICR.val = 0xFF;
    if (CR1_REG.bit.RXFNEIE & ISR_REG.bit.RXFNE)                    hal::irqUsart::RxfneClbkTab[hal::LPUART_1](REG_LPUART1->RDR.bit.RDR);
    if (CR1_REG.bit.TXFNFIE & ISR_REG.bit.TXFNF)                    hal::irqUsart::TxfnfClbkTab[hal::LPUART_1](hal::LPUART_1);
    if (CR1_REG.bit.TCIE & ISR_REG.bit.TC)                          hal::irqUsart::TxcmpClbkTab[hal::LPUART_1](hal::LPUART_1);
    if (CR1_REG.bit.PEIE & ISR_REG.bit.PE)                          hal::irqUsart::ParityClbkTab[hal::LPUART_1](hal::LPUART_1);
    if (CR3_REG.bit.EIE & ISR_REG.bit.FE)                           hal::irqUsart::FrameClbkTab[hal::LPUART_1](hal::LPUART_1);
    if (CR1_REG.bit.RTOIE & ISR_REG.bit.RTOF)                       hal::irqUsart::RtofClbkTab[hal::LPUART_1](hal::LPUART_1);
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
