/*
 *
 * Copyright (c) 2021 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <string.h>

#include <ti/drv/uart/UART_stdio.h>

#include "sbl_dma.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define UDMA_COPY_CACHELINE_ALIGNMENT       (UDMA_CACHELINE_ALIGNMENT)

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define SBL_RING_ENTRIES                (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define SBL_RING_ENTRY_SIZE             (sizeof(uint64_t))
/** \brief Total ring memory */
#define SBL_RING_MEM_SIZE               (SBL_RING_ENTRIES * SBL_RING_ENTRY_SIZE)
/** \brief This ensures every channel memory is aligned */
#define SBL_RING_MEM_SIZE_ALIGN ((SBL_RING_MEM_SIZE + UDMA_COPY_CACHELINE_ALIGNMENT) & ~(UDMA_COPY_CACHELINE_ALIGNMENT - 1U))

#define SBL_MAX_TR              (2u)
/**
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 *  At max 2 TRs
 */
#define SBL_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * (SBL_MAX_TR + 1u)) + 4U)
/** \brief This ensures every channel memory is aligned */
#define SBL_TRPD_SIZE_ALIGN   ((SBL_TRPD_SIZE + UDMA_COPY_CACHELINE_ALIGNMENT) & ~(UDMA_COPY_CACHELINE_ALIGNMENT - 1U))

#define SBL_UDMA_XFER_SIZE    (0x00008000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    struct Udma_ChObj       chObj;
    struct Udma_EventObj    cqEventObj;
    struct Udma_EventObj    tdCqEventObj;

    Udma_ChHandle           chHandle;
    Udma_EventHandle        cqEventHandle;
    Udma_EventHandle        tdCqEventHandle;

    Udma_DrvHandle          drvHandle;
    SemaphoreP_Handle       transferDoneSem;
    /**< Semaphore to indicate transfer completion */

    uint8_t                 *txRingMem;
    uint8_t                 *txCompRingMem;
    uint8_t                 *txTdCompRingMem;
    uint8_t                 *trpdMem;

    /**< Allocated TRPD memory physical pointer */
} Sbl_UdmaChObj;

typedef struct
{
    uint8_t                 initDone;
    Udma_DrvHandle          drvHandle;
    Sbl_UdmaChObj           chObj;
} Sbl_UdmaObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void sblEventDmaCb(Udma_EventHandle eventHandle,
                           uint32_t eventType,
                           void *appData);
static void sblEventTdCb(Udma_EventHandle eventHandle,
                          uint32_t eventType,
                          void *appData);

static void sblUdmaInit(Sbl_UdmaObj *udmaObj, Udma_DrvHandle drvHndl);
static int32_t sblUdmaDeInit(Sbl_UdmaObj *udmaObj);

static int32_t sblUdmaCreate(Sbl_UdmaObj *udmaObj);
static int32_t sblUdmaDelete(Sbl_UdmaObj *udmaObj);

static void sblTrpdinit(Sbl_UdmaChObj *udmaChObj, void *srcAddr, void *dstAddr, uint32_t length);

static void SBL_udmaCacheWb(const void *addr, uint32_t size);


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver and channel objects
 */
Sbl_UdmaObj gSblUdmaObj;

/*
 * UDMA Memories
 */
uint8_t gSblTxRingMem[SBL_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_COPY_CACHELINE_ALIGNMENT)));
uint8_t gSblTxCompRingMem[SBL_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_COPY_CACHELINE_ALIGNMENT)));
uint8_t gSblTxTdCompRingMem[SBL_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_COPY_CACHELINE_ALIGNMENT)));
uint8_t gSblUdmaTrpdMem[SBL_TRPD_SIZE_ALIGN] __attribute__((aligned(UDMA_COPY_CACHELINE_ALIGNMENT)));

/* Offset into app image that is being processed */
static uint32_t sblMemOffset = 0;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t SBL_udmaInit(Udma_DrvHandle drvHndl)
{
    int32_t         retVal = 0;
    Sbl_UdmaObj    *udmaObj = &gSblUdmaObj;

    memset(&gSblUdmaObj, 0x0, sizeof(gSblUdmaObj));

    sblUdmaInit(udmaObj, drvHndl);

    retVal = sblUdmaCreate(udmaObj);
    if(UDMA_SOK != retVal)
    {
        SBL_log(SBL_LOG_ERR, "[DMA] UDMA create failed!!\n");
        retVal = -1;
    }
    else
    {
        udmaObj->initDone = 1;
        retVal = 0;
    }

    return (retVal);
}

uint8_t SBL_isUdmaInitDone(void)
{
    return (gSblUdmaObj.initDone);
}

void SBL_udmaDeInit(void)
{
    int32_t         retVal;
    Sbl_UdmaObj    *udmaObj = &gSblUdmaObj;

    retVal = sblUdmaDelete(udmaObj);
    if(UDMA_SOK != retVal)
    {
        SBL_log(SBL_LOG_ERR, "[DMA] UDMA App delete failed!!\n");
    }

    retVal = sblUdmaDeInit(udmaObj);
    if(UDMA_SOK != retVal)
    {
        SBL_log(SBL_LOG_ERR, "[DMA] UDMA App deinit failed!!\n");
    }
    udmaObj->initDone = 0;
}

int32_t SBL_udmaReadData(void *dstAddr, void *srcAddr, uint32_t length)
{
    int32_t         retVal = UDMA_SOK;
    uint64_t        pDesc = 0;
    Sbl_UdmaObj    *udmaObj = &gSblUdmaObj;
    Sbl_UdmaChObj  *udmaChObj;
    Udma_ChHandle   chHandle;
    void           *tempPtr;

    udmaChObj = &udmaObj->chObj;
    chHandle = udmaChObj->chHandle;

    tempPtr = (void*)((uint32_t)srcAddr + sblMemOffset);

    /* DMA transfer is currently supported only for DDR and
     * only if the transfer size is more than 1K bytes. */
    if ((length > 1024) &&
        (((uint32_t)dstAddr & 0x80000000u) == 0x80000000u))
    {
        /* Updated TRPD */
        sblTrpdinit(udmaChObj, tempPtr, dstAddr, length);

        /* Submit TRPD to channel */
        retVal = Udma_ringQueueRaw(
                     Udma_chGetFqRingHandle(chHandle),
                     (uint64_t) udmaChObj->trpdMem);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] Channel queue failed!!\n");
        }

        /* Wait for return descriptor in completion ring for the last channel
         * This marks the entire transfer completion */
        SemaphoreP_pend(udmaChObj->transferDoneSem, SemaphoreP_WAIT_FOREVER);

        /* Response received in completion queue */
        retVal =
            Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);

        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] No descriptor after callback!!\n");
            retVal = UDMA_EFAIL;
        }

        /*
         * Sanity check
         */
        /* Check returned descriptor pointer */
        if(pDesc != ((uint64_t) udmaChObj->trpdMem))
        {
            SBL_log(SBL_LOG_ERR, "[DMA] TR descriptor pointer returned doesn't "
                   "match the submitted address!!\n");
            retVal = UDMA_EFAIL;
        }

        sblMemOffset += length;
    }
    else
    {
        memcpy((void *)dstAddr, tempPtr, length);
        sblMemOffset += length;
    }

    if (UDMA_SOK != retVal)
    {
        retVal = -1;
    }
    else
    {
        retVal = 0;
    }
    return (retVal);
}

/* move the buffer pointer */
void SBL_udmaSeekMem(void *srcAddr, uint32_t location)
{
    sblMemOffset = location;
}


static void sblEventDmaCb(Udma_EventHandle eventHandle,
                               uint32_t eventType,
                               void *appData)
{
    Sbl_UdmaChObj *udmaChObj = (Sbl_UdmaChObj *) appData;

    if(udmaChObj != NULL)
    {
        SemaphoreP_post(udmaChObj->transferDoneSem);
    }

    return;
}

static void sblEventTdCb(Udma_EventHandle eventHandle,
                         uint32_t eventType,
                         void *appData)
{
    CSL_UdmapTdResponse tdResp;
    Sbl_UdmaChObj      *udmaChObj = (Sbl_UdmaChObj *) appData;

    if(udmaChObj != NULL)
    {
        /* Response received in Teardown completion queue */
        Udma_chDequeueTdResponse(udmaChObj->chHandle, &tdResp);
    }

    return;
}

static void sblUdmaInit(Sbl_UdmaObj *udmaObj, Udma_DrvHandle drvHndl)
{
    Sbl_UdmaChObj   *udmaChObj;

    udmaObj->drvHandle           = drvHndl;

    udmaChObj                    = &udmaObj->chObj;

    udmaChObj->chHandle          = &udmaChObj->chObj;
    udmaChObj->cqEventHandle     = NULL;
    udmaChObj->tdCqEventHandle   = NULL;
    udmaChObj->drvHandle         = drvHndl;
    udmaChObj->transferDoneSem   = NULL;
    udmaChObj->txRingMem         = &gSblTxRingMem[0u];
    udmaChObj->txCompRingMem     = &gSblTxCompRingMem[0u];
    udmaChObj->txTdCompRingMem   = &gSblTxTdCompRingMem[0u];
    udmaChObj->trpdMem           = &gSblUdmaTrpdMem[0u];
}

static int32_t sblUdmaDeInit(Sbl_UdmaObj *udmaObj)
{
    /* Release Driver Handle */
    udmaObj->drvHandle = NULL;

    return (0);
}

static int32_t sblUdmaCreate(Sbl_UdmaObj *udmaObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_EventHandle    eventHandle;
    Udma_EventPrms      eventPrms;
    SemaphoreP_Params   semPrms;
    Sbl_UdmaChObj      *udmaChObj;
    Udma_ChHandle       chHandle;
    Udma_DrvHandle      drvHandle = udmaObj->drvHandle;

    udmaChObj = &udmaObj->chObj;
    chHandle = udmaChObj->chHandle;

    SemaphoreP_Params_init(&semPrms);
    udmaChObj->transferDoneSem = SemaphoreP_create(0, &semPrms);
    if(NULL == udmaChObj->transferDoneSem)
    {
        SBL_log(SBL_LOG_ERR, "[DMA] Sem create failed!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Init channel parameters */
        chType = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.ringMem   = udmaChObj->txRingMem;
        chPrms.cqRingPrms.ringMem   = udmaChObj->txCompRingMem;
        chPrms.tdCqRingPrms.ringMem = udmaChObj->txTdCompRingMem;
        chPrms.fqRingPrms.ringMemSize   = SBL_RING_MEM_SIZE;
        chPrms.cqRingPrms.ringMemSize   = SBL_RING_MEM_SIZE;
        chPrms.tdCqRingPrms.ringMemSize = SBL_RING_MEM_SIZE;

        chPrms.fqRingPrms.elemCnt   = SBL_RING_ENTRIES;
        chPrms.cqRingPrms.elemCnt   = SBL_RING_ENTRIES;
        chPrms.tdCqRingPrms.elemCnt = SBL_RING_ENTRIES;

        /* Open channel for block copy */
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA channel open failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Config TX channel */
        UdmaChTxPrms_init(&txPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txPrms);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA TX channel config failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Config RX channel - which is implicitly paired to TX channel in
         * block copy mode */
        UdmaChRxPrms_init(&rxPrms, chType);
        retVal = Udma_chConfigRx(chHandle, &rxPrms);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA RX channel config failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Register ring completion callback - for the last channel only */
        eventHandle = &udmaChObj->cqEventObj;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.chHandle          = chHandle;
        eventPrms.masterEventHandle = NULL;
        eventPrms.eventCb           = &sblEventDmaCb;
        eventPrms.appData           = udmaChObj;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA CQ event register failed!!\n");
        }
        else
        {
            udmaChObj->cqEventHandle = eventHandle;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Register teardown ring completion callback */
        eventHandle = &udmaChObj->tdCqEventObj;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.chHandle          = chHandle;
        eventPrms.masterEventHandle = NULL;
        eventPrms.eventCb           = &sblEventTdCb;
        eventPrms.appData           = udmaChObj;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA Teardown CQ event register failed!!\n");
        }
        else
        {
            udmaChObj->tdCqEventHandle = eventHandle;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Channel enable */
        retVal = Udma_chEnable(chHandle);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA channel enable failed!!\n");
        }
    }

    if(UDMA_SOK != retVal)
    {
        retVal = -1;
    }
    else
    {
        retVal = 0;
    }

    return (retVal);
}

static int32_t sblUdmaDelete(Sbl_UdmaObj *udmaObj)
{
    int32_t         retVal = UDMA_SOK, tempRetVal;
    uint64_t        pDesc;
    Sbl_UdmaChObj  *udmaChObj;
    Udma_ChHandle   chHandle;

    udmaChObj = &udmaObj->chObj;
    chHandle = udmaChObj->chHandle;

    retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
    if(UDMA_SOK != retVal)
    {
        SBL_log(SBL_LOG_ERR, "[DMA] UDMA channel disable failed!!\n");
    }

    /* Flush any pending request from the free queue */
    while(1)
    {
        tempRetVal = Udma_ringFlushRaw(
                         Udma_chGetFqRingHandle(chHandle), &pDesc);
        if(UDMA_ETIMEOUT == tempRetVal)
        {
            break;
        }
    }

    /* Unregister all events */
    if(NULL != udmaChObj->cqEventHandle)
    {
        retVal += Udma_eventUnRegister(udmaChObj->cqEventHandle);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA event unregister failed!!\n");
        }
        udmaChObj->cqEventHandle = NULL;
    }
    if(NULL != udmaChObj->tdCqEventHandle)
    {
        retVal += Udma_eventUnRegister(udmaChObj->tdCqEventHandle);
        if(UDMA_SOK != retVal)
        {
            SBL_log(SBL_LOG_ERR, "[DMA] UDMA event unregister failed!!\n");
        }
        udmaChObj->tdCqEventHandle = NULL;
    }

    retVal += Udma_chClose(chHandle);
    if(UDMA_SOK != retVal)
    {
        SBL_log(SBL_LOG_ERR, "[DMA] UDMA channel close failed!!\n");
    }

    if(udmaChObj->transferDoneSem != NULL)
    {
        SemaphoreP_delete(udmaChObj->transferDoneSem);
        udmaChObj->transferDoneSem = NULL;
    }

    if(UDMA_SOK != retVal)
    {
        retVal = -1;
    }
    else
    {
        retVal = 0;
    }

    return (retVal);
}

static void sblTrpdinit(Sbl_UdmaChObj *udmaChObj, void *srcAddr, void *dstAddr, uint32_t length)
{
    uint32_t               num_tr = 1, cnt;
    uint16_t               icnt[SBL_MAX_TR][4];
    uint32_t               addr_offset[SBL_MAX_TR];
    CSL_UdmapCppi5TRPD *pTrpd = (CSL_UdmapCppi5TRPD *) udmaChObj->trpdMem;
    CSL_UdmapTR15 *pTr = (CSL_UdmapTR15 *)(udmaChObj->trpdMem + sizeof(CSL_UdmapTR15));
    uint32_t               *pTrResp;
    uint32_t cqRingNum = Udma_chGetCqRingNum(udmaChObj->chHandle);
    uint32_t descType = CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_TR;

    memset(icnt, 0, sizeof(icnt));
    memset(addr_offset, 0, sizeof(addr_offset));

    if (length < SBL_UDMA_XFER_SIZE)
    {
        num_tr = 1;
    }
    else if ((length > SBL_UDMA_XFER_SIZE) &&
             (0u != (length & (SBL_UDMA_XFER_SIZE - 1u))))
    {
        num_tr = 2u;
    }
    else
    {
        num_tr = 1u;
    }

    pTrResp = (uint32_t *)(udmaChObj->trpdMem + (sizeof(CSL_UdmapTR15) * (num_tr + 1u)));

    /* Make TRPD */
    /* Setup descriptor */
    CSL_udmapCppi5SetDescType(pTrpd, descType);
    CSL_udmapCppi5TrSetReload(pTrpd, 0U, 0U);
    CSL_udmapCppi5SetPktLen(pTrpd, descType, num_tr);       /* Only one TR in TRPD */
    CSL_udmapCppi5SetIds(pTrpd, descType, 0U, 0x3FFFU); /* Flow ID and Packet ID */
    CSL_udmapCppi5SetSrcTag(pTrpd, 0x0000);     /* Not used */
    CSL_udmapCppi5SetDstTag(pTrpd, 0x0000);     /* Not used */
    CSL_udmapCppi5TrSetEntryStride(
        pTrpd, CSL_UDMAP_CPPI5_TRPD_PKTINFO_RECSIZE_VAL_64B);
    CSL_udmapCppi5SetReturnPolicy(
        pTrpd,
        descType,
        CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,   /* Don't care for TR */
        CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
        CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
        cqRingNum);

    if (length < SBL_UDMA_XFER_SIZE)
    {
        icnt[0u][0u] = (uint16_t)length;
        icnt[0u][1u] = (uint16_t)1U;
        icnt[0u][2u] = (uint16_t)1U;
        icnt[0u][3u] = (uint16_t)1U;
    }
    else
    {
        icnt[0u][0u] = (uint16_t)SBL_UDMA_XFER_SIZE;
        icnt[0u][1u] = (uint16_t)(length / SBL_UDMA_XFER_SIZE);
        icnt[0u][2u] = (uint16_t)1U;
        icnt[0u][3u] = (uint16_t)1U;
    }
    if (num_tr > 1u)
    {
        icnt[1u][0u] = (uint16_t)(length & (SBL_UDMA_XFER_SIZE - 1u));
        icnt[1u][1u] = (uint16_t)1U;
        icnt[1u][2u] = (uint16_t)1U;
        icnt[1u][3u] = (uint16_t)1U;
        addr_offset[1u] = length - (length & (SBL_UDMA_XFER_SIZE - 1u));
    }

    for (cnt = 0u; cnt < num_tr; cnt ++)
    {
        /* Setup TR */
        pTr->flags    = CSL_FMK(UDMAP_TR_FLAGS_TYPE, 15)                                            |
                        CSL_FMK(UDMAP_TR_FLAGS_STATIC, 0U)                                          |
                        CSL_FMK(UDMAP_TR_FLAGS_EOL, 0U)                                             |   /* NA */
                        CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE, CSL_UDMAP_TR_FLAGS_EVENT_SIZE_COMPLETION)|
                        CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0, CSL_UDMAP_TR_FLAGS_TRIGGER_NONE)           |
                        CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE, CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL)  |
                        CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1, CSL_UDMAP_TR_FLAGS_TRIGGER_NONE)           |
                        CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE, CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL)  |
                        CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, 0x35U)                                       |   /* This will come back in TR response */
                        CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, 0U)                                     |
                        CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, 0U)                                     |
                        CSL_FMK(UDMAP_TR_FLAGS_EOP, 1U);

        pTr->icnt0    = icnt[cnt][0];
        pTr->icnt1    = icnt[cnt][1];
        pTr->icnt2    = icnt[cnt][2];
        pTr->icnt3    = icnt[cnt][3];
        pTr->dim1     = (int32_t)pTr->icnt0;
        pTr->dim2     = ((int32_t)pTr->icnt0 * (int32_t)pTr->icnt1);
        pTr->dim3     = ((int32_t)pTr->icnt0 * (int32_t)pTr->icnt1 * (int32_t)pTr->icnt2);
        pTr->addr     = ((uint64_t)srcAddr) + addr_offset[cnt];
        pTr->fmtflags = 0x00000000U;

        pTr->dicnt0   = icnt[cnt][0];
        pTr->dicnt1   = icnt[cnt][1];
        pTr->dicnt2   = icnt[cnt][2];
        pTr->dicnt3   = icnt[cnt][3];
        pTr->ddim1    = (int32_t)pTr->dicnt0;
        pTr->ddim2    = (int32_t)pTr->dicnt0 * (int32_t)pTr->dicnt1;
        pTr->ddim3    = (int32_t)pTr->dicnt0 * (int32_t)pTr->dicnt1 * (int32_t)pTr->dicnt2;
        pTr->daddr    = ((uint64_t)dstAddr) + addr_offset[cnt];

        pTr ++;
    }

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;

    /* Writeback cache */
    SBL_udmaCacheWb(udmaChObj->trpdMem, SBL_TRPD_SIZE_ALIGN);

    return;
}


static void SBL_udmaCacheWb(const void *addr, uint32_t size)
{
    CacheP_wb(addr, size);

    return;
}
