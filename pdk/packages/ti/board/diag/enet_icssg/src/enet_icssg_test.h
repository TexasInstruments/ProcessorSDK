/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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

 /*!
 * \file  enet_icssg_test.h
 *
 * \brief This file contains the type definitions and helper macros for ICSSG
 *        TimeSync.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <ti/csl/soc.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/board/board.h>
#include <ti/drv/udma/udma.h>
#include <ti/osal/SemaphoreP.h>
#include "diag_common_cfg.h"
#include "board_cfg.h"

#if defined(am65xx_evm)
#include <ti/board/src/am65xx_evm/include/board_ethernet_config.h>
#else
#include <ti/board/src/am65xx_idk/include/board_ethernet_config.h>
#endif

/* ENET Driver Header Files */
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_dma.h>
#include <ti/drv/enet/include/per/icssg.h>

/* ENET Util Files */
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils_baremetal.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appboardutils.h>
#include <ti/drv/enet/examples/utils/include/enet_board_am65xevm.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Max number of ports supported per context */
#define ENETIG_PORT_MAX                 (2U)

/* Max number of ports that can be tested with this app */
#define ENETIG_ICSSG_INSTANCE_MAX       (2U)
#define ENETIG_ICSSG_MAX_PORTS          (6U)

/* Loopback test Number of packets count */
#define BOARD_DIAG_ENETLPBK_TEST_PKT_NUM        (100U)
/* Loopback Tx/Rx Timeout */
#define BOARD_DIAG_ENETLPBK_TIMEOUT             (500U)
/* Loopback test packet length */
#define BOARD_DIAG_ENETLPBK_TEST_PKT_LEN        (100U)
/* Link Up Timeout */
#define BOARD_DIAG_ICSSG_LINKUP_TIMEOUT         (100000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/* Test parameters for each port in the multiport test */
typedef struct EnetIg_TestParams_s
{
    /* Peripheral type */
    Enet_Type enetType;

    /* Peripheral instance */
    uint32_t instId;

    /* Peripheral's MAC ports to use */
    Enet_MacPort macPort[ENETIG_PORT_MAX];

    /* Number of MAC ports in macPorts array */
    uint32_t macPortNum;

    /* Name of this port to be used for logging */
    char *name;

}EnetIg_TestParams;

/* Context of a peripheral/port */
typedef struct EnetIg_PerCtxt_s
{
    /* Peripheral type */
    Enet_Type enetType;

    /* Peripheral instance */
    uint32_t instId;

    /* Peripheral's MAC ports to use */
    Enet_MacPort macPort[ENETIG_PORT_MAX];

    /* Number of MAC ports in macPorts array */
    uint32_t macPortNum;

    /* Name of this port to be used for logging */
    char *name;

    /* Enet driver handle for this peripheral type/instance */
    Enet_Handle hEnet;

    /* ICSSG configuration */
    Icssg_Cfg icssgCfg;

    /* MAC address. It's port's MAC address in Dual-MAC or
     * host port's MAC addres in Switch */
    uint8_t macAddr[ENET_MAC_ADDR_LEN];

    /* UDMA driver configuration */
    EnetDma_Cfg dmaCfg;

    /* TX channel number */
    uint32_t txChNum;

    /* TX channel handle */
    EnetDma_TxChHandle hTxCh;

    /* Start flow index */
    uint32_t rxStartFlowIdx;

    /* Flow index */
    uint32_t rxFlowIdx;

    /* RX channel handle */
    EnetDma_RxChHandle hRxCh;

    /* Semaphore posted from RX callback when packets have arrived */
    SemaphoreP_Handle hRxSem;

    /* Semaphore posted from TX callback when packets are transmitted */
    SemaphoreP_Handle hTxSem;

    /* Flag updated from event callback upon asynchronous IOCTL completion */
    bool hAsyncIoctlSem;

    /* Timestamp of the last received packets */
    uint64_t rxTs[ENET_MEM_NUM_RX_PKTS];

    /* Timestamp of the last transmitted packets */
    uint64_t txTs[ENET_MEM_NUM_RX_PKTS];

    /* Sequence number used as cookie for timestamp events. This value is passed
     * to the DMA packet when submitting a packet for transmission. Driver will
     * pass this same value when timestamp for that packet is ready. */
    uint32_t txTsSeqId;

    /* Flag updated from event callback upon tx timestamp completion */
    bool hTxTsSem;

} EnetIg_PerCtxt;

typedef struct EnetIg_Obj_s
{
    /* This core's id */
    uint32_t coreId;

    /* Core key returned by Enet RM after attaching this core */
    uint32_t coreKey;

    /* Main UDMA driver handle */
    Udma_DrvHandle hMainUdmaDrv;

    /* MCU UDMA driver handle */
    Udma_DrvHandle hMcuUdmaDrv;

    /* Queue of free TX packets */
    EnetDma_PktQ txFreePktInfoQ;

    /* Array of all peripheral/port contexts used in the test */
    EnetIg_PerCtxt perCtxt[ENETIG_ICSSG_INSTANCE_MAX];

    EthFrame *txFrame;
    EthFrame *rxFrame;

    /* Number of active contexts being used */
    uint32_t numPerCtxts;

    uint32_t totalTxCnt;

    uint32_t totalRxCnt;

    uint32_t loopCnt;

    uint32_t passIterations;

    uint32_t failIterations;

} EnetIg_Obj;
