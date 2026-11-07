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

#pragma once
#include "common.h"
#include "regUsart.h"
#include "stm32wle5xx.h"
#include "stm32wleTypes.h"

/******************************************************************************************************************************************
                                                                PUBLIC TYPES
******************************************************************************************************************************************/
namespace hal::irqUsart {

using TxfnfClbk_t = void (*)(hal::usartNum_t usartNum);          // Tx fifo not full interrupt callback
using TxcmpClbk_t = void (*)(hal::usartNum_t usartNum);          // Tx complete interrupt callback
using RxfneClbk_t = void (*)(uint8 rxByte);                      // Rx data fifo not empty interrupt callback
using ParityClbk_t = void (*)(hal::usartNum_t usartNum);         // Parity error interrupt callback
using FrameClbk_t = void (*)(hal::usartNum_t usartNum);          // Framing error interrupt callback
using OreClbk_t = void (*)(hal::usartNum_t usartNum);            // Overrun error interrupt callback
using RtofClbk_t = void (*)(hal::usartNum_t usartNum);           // Receiver timeout interrupt callback

/******************************************************************************************************************************************
                                                                    API
******************************************************************************************************************************************/

Exec SetTxfnfClbk(hal::usartNum_t usartNum, TxfnfClbk_t Clbk);
Exec SetTxcmpClbk(hal::usartNum_t usartNum, TxcmpClbk_t Clbk);
Exec SetRxfneClbk(hal::usartNum_t usartNum, RxfneClbk_t Clbk);
Exec SetParityClbk(hal::usartNum_t usartNum, ParityClbk_t Clbk);
Exec SetFrameClbk(hal::usartNum_t usartNum, FrameClbk_t Clbk);
Exec SetOreClbk(hal::usartNum_t usartNum, OreClbk_t Clbk);
Exec SetRtofClbk(hal::usartNum_t usartNum, RtofClbk_t Clbk);

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // End of namespace 'hal::irqUsart'

