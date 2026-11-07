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
// DESCRIPTION:      Low level layer for the SPI1 & SPI2 peripheral

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "mcuCore.h"
#include "perSpi.h"
#include "sysclk.h"
#include "irqList.h"
#include "regDmaMux.h"
#include "irqDma.h"

/******************************************************************************************************************************************
                                                         PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/

#undef CODE_LABEL
#undef CODE_STATUS
#undef ACTION_ON_ERR
#define CODE_LABEL      ctx.config.label_p
#define CODE_STATUS     ctx.status
#define ACTION_ON_ERR   ctx.status = Status::ERR; ChangeMode(Mode::ERR)

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/
namespace hal {

void SPI_M::SetupDma()
{
    ctx.dmaReg_p = (ctx.config.dmaNum == hal::DMA_1) ? REG_DMA1 : REG_DMA2;

    uint8 chTx = ctx.config.dmaTxChannel;
    uint8 chRx = ctx.config.dmaRxChannel;
    uint8 muxOffset = (ctx.config.dmaNum == hal::DMA_1) ? 0 : 7;
    uint8 muxTx = muxOffset + chTx;
    uint8 muxRx = muxOffset + chRx;

    hal::clk::PerClkEnable((ctx.config.dmaNum == hal::DMA_1) ? hal::PER_DMA1 : hal::PER_DMA2);
    hal::clk::PerClkEnable(hal::PER_DMA_MUX);

    // Disable DMA channels before configuration
    ctx.dmaReg_p->CH[chTx].CCR.bit.EN = 0;
    ctx.dmaReg_p->CH[chRx].CCR.bit.EN = 0;

    // Set DMA channel request source
    REG_DMAMUX->CxCR[muxTx].val = 0;
    REG_DMAMUX->CxCR[muxTx].bit.DMAREQ_ID = DMA_REQ_SPI2_TX;

    REG_DMAMUX->CxCR[muxRx].val = 0;
    REG_DMAMUX->CxCR[muxRx].bit.DMAREQ_ID = DMA_REQ_SPI2_RX;

    // Configure TX channel: memory -> peripheral
    ctx.dmaReg_p->CH[chTx].CCR.val = 0;
    ctx.dmaReg_p->CH[chTx].CCR.bit.DIR   = 1;  // Memory to peripheral
    ctx.dmaReg_p->CH[chTx].CCR.bit.PINC  = 0;  // Peripheral address fixed
    ctx.dmaReg_p->CH[chTx].CCR.bit.MINC  = 1;  // Increment memory address
    ctx.dmaReg_p->CH[chTx].CCR.bit.PSIZE = 0;  // 8-bit peripheral data
    ctx.dmaReg_p->CH[chTx].CCR.bit.MSIZE = 0;  // 8-bit memory data
    ctx.dmaReg_p->CH[chTx].CCR.bit.PL    = 2;  // High priority
    ctx.dmaReg_p->CH[chTx].CCR.bit.TEIE  = 1;  // Transfer error interrupt enable

    // Configure RX channel: peripheral -> memory
    ctx.dmaReg_p->CH[chRx].CCR.val = 0;
    ctx.dmaReg_p->CH[chRx].CCR.bit.DIR   = 0;  // Peripheral to memory
    ctx.dmaReg_p->CH[chRx].CCR.bit.PINC  = 0;  // Peripheral address fixed
    ctx.dmaReg_p->CH[chRx].CCR.bit.MINC  = 1;  // Increment memory address
    ctx.dmaReg_p->CH[chRx].CCR.bit.PSIZE = 0;  // 8-bit peripheral data
    ctx.dmaReg_p->CH[chRx].CCR.bit.MSIZE = 0;  // 8-bit memory data
    ctx.dmaReg_p->CH[chRx].CCR.bit.PL    = 2;  // High priority
    ctx.dmaReg_p->CH[chRx].CCR.bit.TEIE  = 1;  // Transfer error interrupt enable

    irqDma::SetIrqClbk(ctx.config.dmaNum, ctx.config.dmaTxChannel, ctx.spiPerId, IrqDmaTxTransfer);
    irqDma::SetIrqClbk(ctx.config.dmaNum, ctx.config.dmaRxChannel, ctx.spiPerId, IrqDmaRxTransfer);

    uint8 dmaIrqIdBase = (ctx.config.dmaNum == hal::DMA_1) ? hal::IRQ_DMA1_CH1 : hal::IRQ_DMA2_CH1;
    hal::irq::SetIrqInUse((irqId_t)(dmaIrqIdBase + ctx.config.dmaTxChannel), "DMA SPI TX");
    hal::irq::SetIrqInUse((irqId_t)(dmaIrqIdBase + ctx.config.dmaRxChannel), "DMA SPI RX");

    ClearDmaIrqFlags();
    EnableDmaIrq();
}

void SPI_M::StartDmaTxTransfer(trBuff_t* trBuff_p)
{
    static uint8 dummyRxByte;

    uint8 chTx = ctx.config.dmaTxChannel;
    uint8 chRx = ctx.config.dmaRxChannel;

    // Disable channels
    ctx.dmaReg_p->CH[ctx.config.dmaTxChannel].CCR.bit.EN = false;
    ctx.dmaReg_p->CH[ctx.config.dmaRxChannel].CCR.bit.EN = false;

    // Clear all interrupt flags
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaTxChannel * 4);
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaRxChannel * 4);

    // Configure TX DMA
    ctx.dmaReg_p->CH[chTx].CPAR.val = (uint32)&ctx.spiReg_p->DR.val8;
    ctx.dmaReg_p->CH[chTx].CMAR.val = (uint32)trBuff_p->buff_p;
    ctx.dmaReg_p->CH[chTx].CNDTR.bit.NDT = trBuff_p->txSize;
    ctx.dmaReg_p->CH[chTx].CCR.bit.MINC = 1;
    ctx.dmaReg_p->CH[chTx].CCR.bit.TCIE = 1;

    // Configure RX DMA as dummy sink
    ctx.dmaReg_p->CH[chRx].CPAR.val = (uint32)&ctx.spiReg_p->DR.val8;
    ctx.dmaReg_p->CH[chRx].CMAR.val = (uint32)&dummyRxByte;
    ctx.dmaReg_p->CH[chRx].CNDTR.bit.NDT = trBuff_p->txSize;

    // RX memory address must remain fixed
    ctx.dmaReg_p->CH[chRx].CCR.bit.MINC = 0;

    // RX transfer complete interrupt is not needed for dummy RX
    ctx.dmaReg_p->CH[chRx].CCR.bit.TCIE = 0;

    // Enable DMA channels
    ctx.dmaReg_p->CH[chRx].CCR.bit.EN = 1;
    ctx.dmaReg_p->CH[chTx].CCR.bit.EN = 1;

    // Enable SPI DMA requests
    ctx.spiReg_p->CR2.bit.RXDMAEN = 1;
    ctx.spiReg_p->CR2.bit.TXDMAEN = 1;
}

void SPI_M::StartDmaRxTransfer(trBuff_t* trBuff_p)
{
    static uint8 dummyTxByte = 0;

    uint8 chTx = ctx.config.dmaTxChannel;
    uint8 chRx = ctx.config.dmaRxChannel;

    // Disable channels
    ctx.dmaReg_p->CH[ctx.config.dmaTxChannel].CCR.bit.EN = false;
    ctx.dmaReg_p->CH[ctx.config.dmaRxChannel].CCR.bit.EN = false;

    // Clear all interrupt flags
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaTxChannel * 4);
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaRxChannel * 4);

    // Configure RX DMA
    ctx.dmaReg_p->CH[chRx].CPAR.val = (uint32)&ctx.spiReg_p->DR.val8;
    ctx.dmaReg_p->CH[chRx].CMAR.val = (uint32)trBuff_p->rxDest_p;
    ctx.dmaReg_p->CH[chRx].CNDTR.bit.NDT = trBuff_p->rxSize;
    ctx.dmaReg_p->CH[chRx].CCR.bit.MINC = 1;
    ctx.dmaReg_p->CH[chRx].CCR.bit.TCIE = 1;

    // Configure TX DMA as clock generator
    ctx.dmaReg_p->CH[chTx].CPAR.val = (uint32)&ctx.spiReg_p->DR.val8;
    ctx.dmaReg_p->CH[chTx].CMAR.val = (uint32)&dummyTxByte;
    ctx.dmaReg_p->CH[chTx].CNDTR.bit.NDT = trBuff_p->rxSize;
    ctx.dmaReg_p->CH[chTx].CCR.bit.MINC = 0;
    ctx.dmaReg_p->CH[chTx].CCR.bit.TCIE = 0;

    // Enable DMA channels
    ctx.dmaReg_p->CH[chRx].CCR.bit.EN = 1;
    ctx.dmaReg_p->CH[chTx].CCR.bit.EN = 1;

    // Enable SPI DMA requests
    ctx.spiReg_p->CR2.bit.RXDMAEN = 1;
    ctx.spiReg_p->CR2.bit.TXDMAEN = 1;
}

void SPI_M::DisableDmaChannels()
{
    // Disable channels
    ctx.dmaReg_p->CH[ctx.config.dmaTxChannel].CCR.bit.EN = false;
    ctx.dmaReg_p->CH[ctx.config.dmaRxChannel].CCR.bit.EN = false;

    // Disable SPI DMA requests
    ctx.spiReg_p->CR2.bit.RXDMAEN = 0;
    ctx.spiReg_p->CR2.bit.TXDMAEN = 0;
}

void SPI_M::DisableDmaIrq()
{
    uint8 dmaIrqIdBase = (ctx.config.dmaNum == hal::DMA_1) ? hal::IRQ_DMA1_CH1 : hal::IRQ_DMA2_CH1;
    hal::core::IrqDisable((irqId_t)(dmaIrqIdBase + ctx.config.dmaTxChannel));
    hal::core::IrqDisable((irqId_t)(dmaIrqIdBase + ctx.config.dmaRxChannel));
}

void SPI_M::EnableDmaIrq()
{
    uint8 dmaIrqIdBase = (ctx.config.dmaNum == hal::DMA_1) ? hal::IRQ_DMA1_CH1 : hal::IRQ_DMA2_CH1;
    hal::core::IrqEnable((irqId_t)(dmaIrqIdBase + ctx.config.dmaTxChannel));
    hal::core::IrqEnable((irqId_t)(dmaIrqIdBase + ctx.config.dmaRxChannel));
}

void SPI_M::ClearDmaIrqFlags()
{
    // Clear all interrupt flags
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaTxChannel * 4);
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaRxChannel * 4);
}

void SPI_M::ResetDma()
{
    // Clear all interrupt flags
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaTxChannel * 4);
    ctx.dmaReg_p->IFCR.val |= 0b1 << (ctx.config.dmaRxChannel * 4);

    // Disable channels
    ctx.dmaReg_p->CH[ctx.config.dmaTxChannel].CCR.bit.EN = false;
    ctx.dmaReg_p->CH[ctx.config.dmaRxChannel].CCR.bit.EN = false;
}

void SPI_M::IrqDmaTxTransfer(hal::dmaNum_t dmaNum, hal::dmaCh_t dmaCh, hal::perId_t perID)
{
    SPI_M* spi_p = perID == hal::PER_SPI1 ? spiMaster_p[hal::SPI1] : spiMaster_p[hal::SPI2];
    SPI_M::trBuff_t* trBuff_p = spi_p->ctx.trBuffRun_p;
    while (spi_p->ctx.spiReg_p->SR.bit.BSY);

    spi_p->ClearDmaIrqFlags();
    spi_p->DisableDmaChannels();
    spi_p->DisableMosi();

    if (trBuff_p->trType == TrType::TXRX) {
        spi_p->StartDmaRxTransfer(trBuff_p);
    }
    else {
        if ((trBuff_p->ssMode == SsMode::AUTO) && (trBuff_p->keepSsLow == false)) {
            spi_p->ClearSsLine(trBuff_p->slaveId);
        }

        if (trBuff_p->Clbk) trBuff_p->Clbk();
        trBuff_p->trStatus = TrStat::DONE;
        spi_p->ctx.buffsBusy--;

        //-----------------------------------------------------------------------------------
        // Check if there is a new transaction waiting

        SPI_M::trBuff_t* trBuffWaiting_p = spi_p->FindWaitingTrBuff();
        if (trBuffWaiting_p != nullptr) {
            spi_p->TriggerTransaction(trBuffWaiting_p);
        }
        else {
            spi_p->ChangeState(State::IDLE);
        }
    }
}

void SPI_M::IrqDmaRxTransfer(hal::dmaNum_t dmaNum, hal::dmaCh_t dmaCh, hal::perId_t perID)
{
    SPI_M* spi_p = perID == hal::PER_SPI1 ? spiMaster_p[hal::SPI1] : spiMaster_p[hal::SPI2];
    SPI_M::trBuff_t* trBuff_p = spi_p->ctx.trBuffRun_p;
    while (spi_p->ctx.spiReg_p->SR.bit.BSY);
    spi_p->ClearDmaIrqFlags();
    spi_p->DisableDmaChannels();

    if ((trBuff_p->ssMode == SsMode::AUTO) && (trBuff_p->keepSsLow == false)) {
        spi_p->ClearSsLine(trBuff_p->slaveId);
    }

    if (trBuff_p->Clbk) trBuff_p->Clbk();
    trBuff_p->trStatus = TrStat::DONE;
    spi_p->ctx.buffsBusy--;

    //-----------------------------------------------------------------------------------
    // Check if there is a new transaction waiting

    SPI_M::trBuff_t* trBuffWaiting_p = spi_p->FindWaitingTrBuff();
    if (trBuffWaiting_p != nullptr) {
        spi_p->TriggerTransaction(trBuffWaiting_p);
    }
    else {
        spi_p->ChangeState(State::IDLE);
    }
}

} // END OF NAMESPACE 'hal'
/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
