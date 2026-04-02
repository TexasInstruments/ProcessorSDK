/*
 * Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * \file  lwip2enet.h
 *
 * \brief Header file for the LwIP to Enet helper functions.
 */

#ifndef LWIP2ENET_H_
#define LWIP2ENET_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* Standard language headers */
#include <stdint.h>
#include <assert.h>

/* OS/Posix headers */
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/ClockP.h>
#include <ti/osal/DebugP.h>

/* Project dependency headers */
#include "lwipif2enet_appif.h"
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/enet_cfg.h>
#include "lwip2enet_queue.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*! \brief RX packet task stack size */
#define LWIPIF_RX_PACKET_TASK_STACK         (16U * 1024U)

/*! \brief TX packet task stack size */
#define LWIPIF_TX_PACKET_TASK_STACK         (16U * 1024U)

/*! \brief Links status poll task stack size */
#define LWIPIF_POLL_TASK_STACK              (16U * 1024U)

/*! \brief RX packet task stack size */
#define LWIPIF_RX_PACKET_TASK_STACK_ALIGN    LWIPIF_RX_PACKET_TASK_STACK

/*! \brief TX packet task stack size */
#define LWIPIF_TX_PACKET_TASK_STACK_ALIGN    LWIPIF_TX_PACKET_TASK_STACK

/*! \brief Links status poll task stack size */
#define LWIPIF_POLL_TASK_STACK_ALIGN         LWIPIF_POLL_TASK_STACK

/*
 * Split the number of pbufs for TX channel and as many enabled RX channels/flows:
 * - TX channel requires LWIP2ENET_TX_PACKETS pbufs
 * - Each RX channel/flows requires (2 x LWIP2ENET_RX_PACKETS) to avoid running out of
 *   free pbufs to new packets
 *
 * Total pbufs (PBUF_POOL_SIZE) should be:
 *   total = tx + ((2 * rx) * n)
 *
 * If we assume number of buffers per channel to be same, then:
 *   tx = rx = total / (1 + 2 * n)
 */
#define LWIP2ENET_PACKET_BUDGET          (PBUF_POOL_SIZE / ENET_CFG_LWIP_IFACE_MAX)
#define LWIP2ENET_TX_PACKETS             (LWIP2ENET_PACKET_BUDGET / (1U + (2U * LWIP2ENET_RX_NUM)))
#define LWIP2ENET_RX_PACKETS             (LWIP2ENET_TX_PACKETS)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

#define HISTORY_CNT ((uint32_t)256U)

typedef LwipifEnetAppIf_GetHandleOutArgs Lwip2Enet_AppInfo;

typedef struct Lwip2Enet_PktTaskStats_s
{
    uint32_t rawNotifyCnt;
    uint32_t dataNotifyCnt;
    uint32_t zeroNotifyCnt;
    uint32_t totalPktCnt;
    uint32_t totalCycleCnt;

    uint32_t pktsPerNotifyMax;
    uint32_t pktsPerNotify[HISTORY_CNT];
    uint32_t cycleCntPerNotifyMax;
    uint32_t cycleCntPerNotify[HISTORY_CNT];
    uint32_t cycleCntPerPktMax;
    uint32_t cycleCntPerPkt[HISTORY_CNT];
    uint32_t taskLoad[HISTORY_CNT];
} Lwip2Enet_PktTaskStats;

/*!
 * \brief lwIP interface layer's RX statistics.
 */
typedef struct Lwip2Enet_RxStats_s
{
    Lwip2Enet_PktTaskStats pktStats;
    uint32_t freePbufPktEnq;
    uint32_t freePbufPktDeq;
    uint32_t freeAppPktEnq;
    uint32_t freeAppPktDeq;
    uint32_t errPktCnt;
    uint32_t chkSumErr;
    uint32_t stackNotifyCnt;
} Lwip2Enet_RxStats;

/*!
 * \brief lwIP interface layer's TX statistics.
 */
typedef struct Lwip2Enet_TxStats_s
{
    Lwip2Enet_PktTaskStats pktStats;
    uint32_t readyPbufPktEnq;
    uint32_t readyPbufPktDeq;
    uint32_t freeAppPktEnq;
    uint32_t freeAppPktDeq;
} Lwip2Enet_TxStats;

typedef struct Lwip2Enet_Stats_s
{
    uint32_t cpuLoad[HISTORY_CNT];
    uint32_t hwiLoad[HISTORY_CNT];
} Lwip2Enet_Stats;

struct Lwip2Enet_Obj_s;

/*!
 * \brief RX object which groups variables related to a particular RX channel/flow.
 */
typedef struct Lwip2Enet_RxObj_s
{
    /*! Pointer to parent Lwip2Enet object */
    struct Lwip2Enet_Obj_s *hLwip2Enet;

    /*! Whether this RX object is being used or not */
    bool enabled;

    /*! Enet DMA receive channel (flow) */
    EnetDma_RxChHandle hFlow;

    /*! Start index for RX flow */
    uint32_t flowStartIdx;

    /*! Flow index for RX flow */
    uint32_t flowIdx;

    /*! MAC address allocated for the flow */
    uint8_t macAddr[ENET_MAC_ADDR_LEN];

#if defined(LWIPIF_APP_RX_PKT_HANDLING)
    /*! Pointer for function that lets application handle packet locally */
    LwipifEnetAppIf_HandleRxPktFxn handlePktFxn;
#endif

    /*! Pointer to function for error packet handling */
    LwipifEnetAppIf_HandleRxErrPktFxn handleErrPktFxn;

    /*! Number of DMA packets allocated for this RX flow */
    uint32_t numPkts;

    /*! Packet info memory */
    EnetDma_Pkt pktInfoMem[LWIP2ENET_RX_PACKETS];

    /*! Queue that holds packets ready to be given to the hardware */
    Lwip2EnetQ_Queue freePbufQ;

    /*! DMA Rx free packet info queue (holds packets returned from the hardware) */
    EnetDma_PktQ freePktInfoQ;

    /*! Handle to Rx task, whose job it is to receive packets used by the hardware
     *  and give them to the stack, and return freed packets back to the hardware */
    TaskP_Handle hPktTask;

    /*! Handle to Rx semaphore, on which the hRxPacketTask awaits for notification
     *  of used packets available */
    SemaphoreP_Handle hPktSem;

    /*! lwIP interface statistics */
    Lwip2Enet_RxStats stats;

    /*! RX packet task stack */
    uint8_t pktTaskStack[LWIPIF_RX_PACKET_TASK_STACK] __attribute__ ((aligned(LWIPIF_RX_PACKET_TASK_STACK_ALIGN)));

    /*! Whether RX event should be disabled or not. When disabled, it relies on pacing timer
     *  to retrieve packets from RX channel/flow */
    bool disableEvent;
} Lwip2Enet_RxObj;

/*!
 * \brief TX object which groups variables related to a particular TX channel.
 */
typedef struct Lwip2Enet_TxObj_s
{
    /*! Pointer to parent Lwip2Enet object */
    struct Lwip2Enet_Obj_s *hLwip2Enet;

    /*! Enet DMA transmit channel */
    EnetDma_TxChHandle hCh;

    /*! TX channel peer id */
    uint32_t chNum;

    /*! Directed port number. Set to \ref ENET_MAC_PORT_INV for non-directed packets */
    Enet_MacPort portNum;

    /*! Number of DMA packets allocated for this TX channel */
    uint32_t numPkts;

    /*! Packet info memory */
    EnetDma_Pkt pktInfoMem[LWIP2ENET_TX_PACKETS];

    /*! DMA free queue (holds free hardware packets awaiting) */
    EnetDma_PktQ freePktInfoQ;

    /*! Queue that holds packets ready to be sent to the hardware */
    Lwip2EnetQ_Queue readyPbufQ;

    /*! Queue that holds packets that were not sent to the hardware in previous submit */
    Lwip2EnetQ_Queue unusedPbufQ;

    /*! Handle to Tx task whose job is to retrieve packets consumed by the hardware and
     *  give them to the stack */
    TaskP_Handle hPktTask;

    /*! Handle to Tx semaphore, on which the hTxPacketTask awaits for notification
     *  of used packets available */
    SemaphoreP_Handle hPktSem;

    /*! lwIP interface statistics */
    Lwip2Enet_TxStats stats;

    /*! TX packet task stack */
    uint8_t pktTaskStack[LWIPIF_TX_PACKET_TASK_STACK] __attribute__ ((aligned(LWIPIF_TX_PACKET_TASK_STACK_ALIGN)));

    /*! Whether TX event should be disabled or not. When disabled, "lazy" descriptor recycle
     *  is used instead, which defers retrieval till none is available */
    bool disableEvent;
} Lwip2Enet_TxObj;

/*!
 * \brief Enet-based lwIP object
 */
typedef struct Lwip2Enet_Obj_s
{
    /*! Whether this Enet lwIP object is being used or not */
    bool inUse;

    /*! lwIP network interface */
    struct netif *netif;

    /*! RX object */
    Lwip2Enet_RxObj rx[LWIP2ENET_RX_NUM];

    /*! TX object */
    Lwip2Enet_TxObj tx;

    /*! Total number of allocated PktInfo elements */
    uint32_t allocPktInfo;

    /*! Container of parameters passed from application */
    Lwip2Enet_AppInfo appInfo;

    /*! Initialization flag */
    uint32_t initDone;

    /*! Link is up flag */
    uint32_t linkIsUp;

    /*! Clock handle for periodic poll status query */
    ClockP_Handle hPollTimer;

    /*! Clock handle for triggering the packet Rx notify */
    ClockP_Handle hPacingTimer;

    /*! Handle to counting shutdown semaphore, which all subtasks created in the
     *  open function must post before the close operation can complete */
    SemaphoreP_Handle hShutDownSem;

    /*! Handle to input task that sends polls the link status */
    TaskP_Handle hLinkPollTask;

    /*! Semaphore used by timer to trigger periodic link poll */
    SemaphoreP_Handle hLinkPollSem;

    /*! Boolean to indicate shutdown status of translation layer */
    volatile bool shutDownFlag;

    /*! Print buffer */
    char printBuf[ENET_CFG_PRINT_BUF_LEN];

    /*! Print Function */
    Enet_Print print;

#if defined(LWIPIF_INSTRUMENTATION_LOAD_ENABLED)
    /*! CPU load stats */
    Lwip2Enet_Stats stats;
#endif

    /*! Link status poll task stack */
    uint8_t pollTaskStack[LWIPIF_POLL_TASK_STACK] __attribute__ ((aligned(LWIPIF_POLL_TASK_STACK_ALIGN)));
}
Lwip2Enet_Obj, *Lwip2Enet_Handle;

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*
 * Functions Provided by our translation layer code
 */
extern Lwip2Enet_Handle Lwip2Enet_open(struct netif *netif);

extern void Lwip2Enet_close(Lwip2Enet_Handle hlwip2enet);

extern void Lwip2Enet_sendTxPackets(Lwip2Enet_TxObj *tx);

extern void Lwip2Enet_poll(Lwip2Enet_Handle hlwip2enet,
                          uint32_t fTimerTick);

extern void Lwip2Enet_periodicFxn(Lwip2Enet_Handle hLwip2Enet);

void Lwip2Enet_openDma(Lwip2Enet_Handle hLwip2Enet);

void Lwip2Enet_closeDma(Lwip2Enet_Handle hLwip2Enet);

/* ========================================================================== */
/*                        Deprecated Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static inline void Lwip2EnetStats_addOne(uint32_t *statCnt)
{
#if defined(LWIPIF_INSTRUMENTATION_ENABLED)
    *statCnt += 1U;
#endif
}

static inline void Lwip2EnetStats_addNum(uint32_t *statCnt,
                                         uint32_t addCnt)
{
#if defined(LWIPIF_INSTRUMENTATION_ENABLED)
    *statCnt += addCnt;
#endif
}

static inline void Lwip2Enet_assert(bool cond)
{
    assert(cond);
}

#ifdef __cplusplus
}
#endif

#endif /* LWIP2ENET_H_ */
