/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the
* distribution.
*
* Neither the name of Texas Instruments Incorporated nor the names of
* its contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

/**
 *  \file   cpsw_eth_test.h
 *
 *  \brief  This file contains all Local definitions for CPSW Ethernet test
 *          application.
 *
 */

#ifndef __CPSW_ETH_TEST_H__
#define __CPSW_ETH_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/board/board.h>
#include <ti/board/src/tpr12_evm/include/board_ethernet_config.h>
#include <ti/board/src/tpr12_evm/include/board_utils.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/core/enet_dma.h>
#include <ti/drv/enet/include/per/cpsw.h>

#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appboardutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils_rtos.h>
#include <ti/drv/enet/examples/utils/include/enet_board_tpr12evm.h>

/* Loopback test iteration count */
#define BOARD_DIAG_ENETLPBK_NUM_ITERATION          (1U)
/* Loopback test Number of packets count */
#define BOARD_DIAG_ENETLPBK_TEST_PKT_NUM           (20U)
/* Loopback test packet length */
#define BOARD_DIAG_ENETLPBK_TEST_PKT_LEN           (500U)
/* Status check */
#define BOARD_DIAG_SUCCESS                          (0U)

extern void EnetAppUtils_timerInit(void);
/** 
  * @brief Enet device object structure 
  *
  */
typedef struct BoardDiag_EnetLpbkObj_s
{
    /* Enet handle */
    Enet_Handle hEnet;
    /* CPSW instance type */
    Enet_Type enetType;
    /* Instant Id */
    uint32_t instId;
    /* Core Id */
    uint32_t coreId;
    /* Core Key */
    uint32_t coreKey;
    /* Board Id */
    uint32_t boardId;
    /* MAC port number */
    Enet_MacPort macPort;
    /* MAC mode (defined in board library) */
    emac_mode macMode;
    /* CPSW driver handle */
    EnetDma_RxChHandle hRxCh;

    EnetDma_PktQ rxFreeQ;
    EnetDma_PktQ rxReadyQ;
    EnetDma_TxChHandle hTxCh;
    /* TX DMA packet info memory */
    EnetDma_PktQ txFreePktInfoQ;

    /* Rx channel number */
    uint32_t rxChNum;
    /* Tx channel number */
    uint32_t txChNum;
    /* Host mac address */
    uint8_t hostMacAddr[ENET_MAC_ADDR_LEN];

    /* Test config params */
    /* external LoopBack enable */
    bool testExtLoopback;
    /* PHY loopback */
    bool testPhyLoopback;
    /* Print received Ethernet frames */
    bool printFrame;

    /* Packet transmission */
    uint32_t totalTxCnt;

    /* Packet reception */
    uint32_t totalRxCnt;

} BoardDiag_EnetLpbkObj_t;

/**
 *  \brief    This function runs CPSW2G ethernet test
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_cpswEthRunTest(void);

#ifdef __cplusplus
}
#endif
#endif /* __CPSW_ETH_TEST_H__ */
