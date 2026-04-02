/*
 *  Copyright (c) Texas Instruments Incorporated 2022
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/**
 *  \file   enet_main.h
 *
 *  \brief  This file contains all Local definitions for CPSW 9G Ethernet test
 *          application.
 *
 */
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/ClockP.h>

#include <ti/board/board.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_dma.h>
#include <ti/drv/enet/include/per/cpsw.h>

#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils_rtos.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils_rtos.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>


#include <ti/board/src/j784s4_evm/include/board_ethernet_config.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define BOARD_DIAG_ENET_SGMII_P0P1_TEST            (1U)
#define BOARD_DIAG_ENET_SGMII_P2P3_TEST            (2U)

#define ENETLPBK_NUM_ITERATION                     (1U)

#define ENETLPBK_TEST_PKT_NUM                      (100U)

#define ENETLPBK_TEST_PKT_LEN                      (500U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct BoardDiag_EnetObj_s
{
    /* Enet driver */
    Enet_Handle hEnet;
    
    /* ENET type */
    Enet_Type enetType;
    
    /* Instance ID */
    uint32_t instId;
    
    /* Core ID */
    uint32_t coreId;
    
    /* Core Key */
    uint32_t coreKey;
    
    /* Board ID */
    uint32_t boardId;

    /* Expansion Port*/
    uint32_t expPort;
    
    /* MAC port numbers */
    Enet_MacPort macPort0;
    Enet_MacPort macPort1;
    
    emac_mode macMode;
    
    /* MII interface type */
    EnetMacPort_Interface mii;
    
    /* UDMA driver handle */
    Udma_DrvHandle hUdmaDrv;
    uint32_t rxFlowIdx;
    uint32_t rxStartFlowIdx;
    EnetDma_RxChHandle hRxCh;
    EnetDma_PktQ rxFreeQ;
    EnetDma_PktQ rxReadyQ;
    EnetDma_TxChHandle hTxCh;
    EnetDma_PktQ txFreePktInfoQ;
    uint32_t txChNum;
    
    uint8_t hostMacAddr0[ENET_MAC_ADDR_LEN];
    uint8_t hostMacAddr1[ENET_MAC_ADDR_LEN];
    
    /* Print received Ethernet frames */
    bool printFrame; 

} BoardDiag_EnetObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t BoardDiag_enetLoopbackTest(void);

void BoardDiag_enetInitCpswCfg(Cpsw_Cfg *cpswCfg);

int32_t BoardDiag_enetSetupCpswAle(void);

int32_t BoardDiag_enetOpenEnet(void);

void BoardDiag_enetCloseEnet(void);

int32_t BoardDiag_enetShowAlivePhys(void);

int32_t BoardDiag_enetWaitForLinkUp(void);

int32_t BoardDiag_enetOpenDma();

int32_t BoardDiag_enetMacMode2PhyMii(emac_mode macMode,
                                EnetPhy_Mii *mii);
                             
void BoardDiag_enetCloseDma();

void BoardDiag_enetGetTestConfig(Enet_Type *enetType,
                           uint32_t *instId,
                           Enet_MacPort *macPort0,
                           Enet_MacPort *macPort1,
                           EnetMacPort_Interface *mii,
                           uint32_t *boardId,
                           uint32_t *expPort);

void BoardDiag_enetMacMode2MacMii(emac_mode macMode,
                             EnetMacPort_Interface *mii);