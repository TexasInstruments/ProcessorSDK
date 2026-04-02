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
 * *       No reverse engineering, de-compilation, or disassembly of this software is
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
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/udma.h>

#include "board_ddr.h"

#ifdef BOARD_DDR_ENABLE_DDR_MEM_PRIME

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Buffer Size in bytes */
#define BOARD_DDR_PRIME_BUFFER_NUM_BYTES  (UDMA_CACHELINE_ALIGNMENT)

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define BOARD_RING_ENTRIES                (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define BOARD_RING_ENTRY_SIZE             (sizeof(uint64_t))
/** \brief Total ring memory */
#define BOARD_RING_MEM_SIZE               (BOARD_RING_ENTRIES * BOARD_RING_ENTRY_SIZE)
/** \brief This ensures every channel memory is aligned */
#define BOARD_RING_MEM_SIZE_ALIGN ((BOARD_RING_MEM_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/** 
 * \brief Maximum no.of TR's. 
 * Size to prime is of 32 bytes. TR ICNT is 16 bytes and hence use max of 2 TR's to 
 * effectively split all cases, without causing any left over.
 *  */
#define BOARD_MAX_TR                    (2U)
/* ICNT Split Size. This will be used to split to multiple TR's if required.
 * Set this to half of 0xFFFF(max of 16 bytes ICNT) */
#define BOARD_UDMA_ICNT_SPLIT_SIZE      (0x8000U)

/**
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 */
#define BOARD_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * (BOARD_MAX_TR + 1U)) + 4U)
/** \brief This ensures every channel memory is aligned */
#define BOARD_TRPD_SIZE_ALIGN   ((BOARD_TRPD_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t BOARD_udmaPrime(Udma_ChHandle chHandle, 
                        const void *startAddr, 
                        uint32_t size);

static void BOARD_udmaEventDmaCb(Udma_EventHandle eventHandle,
                                 uint32_t eventType,
                                 void *appData);
static void BOARD_udmaEventTdCb(Udma_EventHandle eventHandle,
                                uint32_t eventType,
                                void *appData);

static int32_t BOARD_udmaInit(Udma_DrvHandle drvHandle);
static int32_t BOARD_udmaDeInit(Udma_DrvHandle drvHandle);

static int32_t BOARD_udmaCreate(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);
static int32_t BOARD_udmaDelete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);

static uint32_t BOARD_udmaTrpdinit(Udma_ChHandle chHandle,
                                   uint8_t *pTrpdMem,
                                   const void *startAddr, 
                                   uint32_t totalSize,
                                   const void *srcAddr,
                                   uint32_t srcBufSize);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver objects
 */
struct Udma_DrvObj      gBoardUdmaDrvObj;
struct Udma_ChObj       gBoardUdmaChObj;
struct Udma_EventObj    gBoardUdmaCqEventObj;
struct Udma_EventObj    gBoardUdmaTdCqEventObj;

/*
 * UDMA Memories
 */
uint8_t gBoardTxRingMem[BOARD_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
uint8_t gBoardTxCompRingMem[BOARD_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
uint8_t gBoardTxTdCompRingMem[BOARD_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
uint8_t gBoardUdmaTrpdMem[BOARD_TRPD_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

uint32_t gBoardUdmaPrimeSrcBuffer[BOARD_DDR_PRIME_BUFFER_NUM_BYTES/4U] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/* Semaphore to indicate priming completion */
static SemaphoreP_Handle gBoardUdmaDDRPrimeDoneSem = NULL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

Board_STATUS BOARD_udmaPrimeDDR(void *startAddr, uint32_t size)
{
    Board_STATUS    status    = BOARD_SOK;
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &gBoardUdmaDrvObj;
    Udma_ChHandle   chHandle = &gBoardUdmaChObj;

    retVal = BOARD_udmaInit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] BOARD UDMA init failed!!\n");
    }

    if(UDMA_SOK == retVal)
    {
        retVal = BOARD_udmaCreate(drvHandle, chHandle);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] BOARD UDMA create failed!!\n");
        }

        if(UDMA_SOK == retVal)
        {
            retVal = BOARD_udmaPrime(chHandle, startAddr, size);
            if(UDMA_SOK != retVal)
            {
                BOARD_DEBUG_LOG("[DMA] BOARD UDMA prime failed!!\n");
            }
        }

        retVal += BOARD_udmaDelete(drvHandle, chHandle);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] BOARD UDMA delete failed!!\n");
        }
    }

    retVal += BOARD_udmaDeInit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] BOARD UDMA deinit failed!!\n");
    }

    return (status);
}

int32_t BOARD_udmaPrime(Udma_ChHandle chHandle, const void *startAddr, uint32_t size)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t       *pTrResp, trRespStatus;
    uint64_t        pDesc = 0;
    uint32_t       *srcBuf = &gBoardUdmaPrimeSrcBuffer[0U];
    uint8_t        *trpdMem = &gBoardUdmaTrpdMem[0U];
    uintptr_t       memPtr;
    uint32_t        offset;
    uint32_t        numTR;
    
    /* Init Src Buffer */
    for (offset = 0U; offset < BOARD_DDR_PRIME_BUFFER_NUM_BYTES; offset += 4)
    {
        *((volatile uint32_t *) srcBuf) = 0xA5A5A5A5;
        srcBuf++;
    }
    /* Writeback buffer */
    CacheP_wb(&gBoardUdmaPrimeSrcBuffer[0U], BOARD_DDR_PRIME_BUFFER_NUM_BYTES);

    /* Update TRPD and get no. of TR's used */
    numTR = BOARD_udmaTrpdinit(chHandle, trpdMem, startAddr, size, &gBoardUdmaPrimeSrcBuffer[0U], BOARD_DDR_PRIME_BUFFER_NUM_BYTES);

    /* Submit TRPD to channel */
    retVal = Udma_ringQueueRaw(
                    Udma_chGetFqRingHandle(chHandle),
                    (uint64_t) trpdMem);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] Channel queue failed!!\n");
    }

    if(UDMA_SOK == retVal)
    {
        /* Wait for return descriptor in completion ring for the last channel
         * This marks the entire transfer completion */
        SemaphoreP_pend(gBoardUdmaDDRPrimeDoneSem, SemaphoreP_WAIT_FOREVER);

        /* Response received in completion queue */
        retVal =
            Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);

        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] No descriptor after callback!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /*
         * Sanity check
         */
        /* Check returned descriptor pointer */
        if(pDesc != ((uint64_t) trpdMem))
        {
            BOARD_DEBUG_LOG("[DMA] TR descriptor pointer returned doesn't "
                    "match the submitted address!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Invalidate cache */
        CacheP_Inv(&gBoardUdmaTrpdMem[0U], BOARD_TRPD_SIZE);

        /* check TR response status */
        pTrResp = (uint32_t *) (trpdMem + (sizeof(CSL_UdmapTR15) * (numTR + 1U)));
        trRespStatus = CSL_FEXT(*pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
        if(trRespStatus != CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE)
        {
            BOARD_DEBUG_LOG("[DMA] TR Response not completed!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    /* Prime any chunk left over towards end, in case when total size is not multiple of BOARD_DDR_PRIME_BUFFER_NUM_BYTES */
    if(size % BOARD_DDR_PRIME_BUFFER_NUM_BYTES)
    {
        memPtr = (uintptr_t)startAddr + size;
        for (offset = 0U; offset <= (size % BOARD_DDR_PRIME_BUFFER_NUM_BYTES); offset += 4)
        {
            memPtr -= 4;
            *((volatile uint32_t *) memPtr) = 0xA5A5A5A5;
        }
    }

    return (retVal);
}

static void BOARD_udmaEventDmaCb(Udma_EventHandle eventHandle,
                                 uint32_t eventType,
                                 void *appData)
{
    if(UDMA_EVENT_TYPE_DMA_COMPLETION == eventType)
    {
        SemaphoreP_post(gBoardUdmaDDRPrimeDoneSem);
    }

    return;
}

static void BOARD_udmaEventTdCb(Udma_EventHandle eventHandle,
                         uint32_t eventType,
                         void *appData)
{
    CSL_UdmapTdResponse tdResp;

    if(UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
    {
        /* Response received in Teardown completion queue */
        Udma_chDequeueTdResponse(&gBoardUdmaChObj, &tdResp);
    }

    return;
}

static int32_t BOARD_udmaInit(Udma_DrvHandle drvHandle)
{
    int32_t         retVal;
    Udma_InitPrms   initPrms;
    uint32_t        instId;
    uint32_t        socDomain;

    /* Use MCU NAVSS for MCU domain cores. Rest cores all uses Main NAVSS */
    socDomain = Board_getSocDomain();
    if(socDomain == BOARD_SOC_DOMAIN_MCU)
    {
        instId = UDMA_INST_ID_MCU_0;
    }
    else
    {
        instId = UDMA_INST_ID_MAIN_0;
    }

    /* UDMA driver init */
    retVal = UdmaInitPrms_init(instId, &initPrms);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] UDMA init prms init failed!!\n");
    }
    else
    {
        retVal = Udma_init(drvHandle, &initPrms);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] UDMA init failed!!\n");
        }
    }
    return (retVal);
}

static int32_t BOARD_udmaDeInit(Udma_DrvHandle drvHandle)
{
    int32_t         retVal;
    
    retVal = Udma_deinit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] UDMA deinit failed!!\n");
    }

    return (retVal);
}

static int32_t BOARD_udmaCreate(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{

    int32_t             retVal = UDMA_SOK;
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_EventHandle    tdCqEventHandle;
    Udma_EventPrms      tdCqEventPrms;
    Udma_EventHandle    cqEventHandle;
    Udma_EventPrms      cqEventPrms;
    SemaphoreP_Params   semPrms;

    SemaphoreP_Params_init(&semPrms);
    gBoardUdmaDDRPrimeDoneSem = SemaphoreP_create(0, &semPrms);
    if(NULL == gBoardUdmaDDRPrimeDoneSem)
    {
        BOARD_DEBUG_LOG("[DMA] Sem create failed!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Init channel parameters */
        chType = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.ringMem       = &gBoardTxRingMem[0U];
        chPrms.cqRingPrms.ringMem       = &gBoardTxCompRingMem[0U];
        chPrms.tdCqRingPrms.ringMem     = &gBoardTxTdCompRingMem[0U];
        chPrms.fqRingPrms.ringMemSize   = BOARD_RING_MEM_SIZE;
        chPrms.cqRingPrms.ringMemSize   = BOARD_RING_MEM_SIZE;
        chPrms.tdCqRingPrms.ringMemSize = BOARD_RING_MEM_SIZE;

        chPrms.fqRingPrms.elemCnt   = BOARD_RING_ENTRIES;
        chPrms.cqRingPrms.elemCnt   = BOARD_RING_ENTRIES;
        chPrms.tdCqRingPrms.elemCnt = BOARD_RING_ENTRIES;

        /* Open channel for block copy */
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] UDMA channel open failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Config TX channel */
        UdmaChTxPrms_init(&txPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txPrms);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] UDMA TX channel config failed!!\n");
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
            BOARD_DEBUG_LOG("[DMA] UDMA RX channel config failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Register ring completion callback */
        cqEventHandle = &gBoardUdmaCqEventObj;
        UdmaEventPrms_init(&cqEventPrms);
        cqEventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
        cqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        cqEventPrms.chHandle          = chHandle;
        cqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        cqEventPrms.eventCb           = &BOARD_udmaEventDmaCb;
        retVal = Udma_eventRegister(drvHandle, cqEventHandle, &cqEventPrms);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] UDMA CQ event register failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Register teardown ring completion callback */
        tdCqEventHandle = &gBoardUdmaTdCqEventObj;
        UdmaEventPrms_init(&tdCqEventPrms);
        tdCqEventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
        tdCqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        tdCqEventPrms.chHandle          = chHandle;
        tdCqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        tdCqEventPrms.eventCb           = &BOARD_udmaEventTdCb;
        retVal = Udma_eventRegister(drvHandle, tdCqEventHandle, &tdCqEventPrms);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] UDMA Teardown CQ event register failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Channel enable */
        retVal = Udma_chEnable(chHandle);
        if(UDMA_SOK != retVal)
        {
            BOARD_DEBUG_LOG("[DMA] UDMA channel enable failed!!\n");
        }
    }

    return (retVal);
}

static int32_t BOARD_udmaDelete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{
    int32_t         retVal = UDMA_SOK, tempRetVal;
    uint64_t        pDesc;
    Udma_EventHandle    cqEventHandle;
    Udma_EventHandle    tdCqEventHandle;

    retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] UDMA channel disable failed!!\n");
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
    cqEventHandle = &gBoardUdmaCqEventObj;
    retVal += Udma_eventUnRegister(cqEventHandle);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] UDMA event unregister failed!!\n");
    }
    tdCqEventHandle = &gBoardUdmaTdCqEventObj;
    retVal += Udma_eventUnRegister(tdCqEventHandle);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] UDMA event unregister failed!!\n");
    }

    retVal += Udma_chClose(chHandle);
    if(UDMA_SOK != retVal)
    {
        BOARD_DEBUG_LOG("[DMA] UDMA channel close failed!!\n");
    }

    if(gBoardUdmaDDRPrimeDoneSem != NULL)
    {
        SemaphoreP_delete(gBoardUdmaDDRPrimeDoneSem);
        gBoardUdmaDDRPrimeDoneSem = NULL;
    }

    return (retVal);
}

static uint32_t BOARD_udmaTrpdinit(Udma_ChHandle chHandle,
                                   uint8_t *pTrpdMem,
                                   const void *startAddr, 
                                   uint32_t totalSize,
                                   const void *srcAddr,
                                   uint32_t srcBufSize)
{
    CSL_UdmapCppi5TRPD  *pTrpd  = (CSL_UdmapCppi5TRPD *) pTrpdMem;
    CSL_UdmapTR15       *pTr    = (CSL_UdmapTR15 *)(pTrpdMem + sizeof(CSL_UdmapTR15));

    uint32_t *pTrResp;
    uint32_t cqRingNum = Udma_chGetCqRingNum(chHandle);

    uint32_t numTR = 1, trIdx;
    uint16_t icnt[BOARD_MAX_TR][4] = {0};
    uint32_t addrOffset[BOARD_MAX_TR] = {0};

    uint32_t remainder    = totalSize/srcBufSize;
    uint32_t residue      = remainder & (BOARD_UDMA_ICNT_SPLIT_SIZE - 1U);

    /* Calculate number of TR's */
    if (remainder < BOARD_UDMA_ICNT_SPLIT_SIZE)
    {
        numTR = 1U;
    }
    else if ((remainder > BOARD_UDMA_ICNT_SPLIT_SIZE) && (0U != residue))
    {
        numTR = 2U;
    }
    else
    {
        numTR = 1U;
    }

    pTrResp = (uint32_t *)(pTrpdMem + (sizeof(CSL_UdmapTR15) * (numTR + 1U)));
    
    if (remainder < BOARD_UDMA_ICNT_SPLIT_SIZE)
    {
        icnt[0][0] = (uint16_t)srcBufSize;
        icnt[0][1] = (uint16_t)remainder;
        icnt[0][2] = (uint16_t)1U;
        icnt[0][3] = (uint16_t)1U;
    }
    else
    {
        icnt[0][0] = (uint16_t)srcBufSize;
        icnt[0][1] = (uint16_t)BOARD_UDMA_ICNT_SPLIT_SIZE;
        icnt[0][2] = (uint16_t)(remainder / BOARD_UDMA_ICNT_SPLIT_SIZE);
        icnt[0][3] = (uint16_t)1U;
    }
    if (numTR > 1)
    {
        icnt[1][0] = (uint16_t)srcBufSize;
        icnt[1][1] = (uint16_t)residue;
        icnt[1][2] = (uint16_t)1U;
        icnt[1][3] = (uint16_t)1U;
        addrOffset[1] = srcBufSize * (remainder - residue);
    }

    /* Make TRPD */
    UdmaUtils_makeTrpd(pTrpd, UDMA_TR_TYPE_15, numTR, cqRingNum);

    for (trIdx = 0u; trIdx < numTR; trIdx ++)
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
                        CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, 0x25U)                                       |   /* This will come back in TR response */
                        CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, 0U)                                     |
                        CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, 0U)                                     |
                        CSL_FMK(UDMAP_TR_FLAGS_EOP, 1U);

        pTr->icnt0    = icnt[trIdx][0];
        pTr->icnt1    = icnt[trIdx][1];
        pTr->icnt2    = icnt[trIdx][2];
        pTr->icnt3    = icnt[trIdx][3];
        pTr->dim1     = 0U;
        pTr->dim2     = 0U;
        pTr->dim3     = 0U;
        pTr->addr     = (uint64_t)srcAddr;
        pTr->fmtflags = 0x00000000U;        /* Linear addressing, 1 byte per elem.
                                            Replace with CSL-FL API */
        pTr->dicnt0   = icnt[trIdx][0];
        pTr->dicnt1   = icnt[trIdx][1];
        pTr->dicnt2   = icnt[trIdx][2];
        pTr->dicnt3   = icnt[trIdx][3];
        pTr->ddim1    = pTr->dicnt0;
        pTr->ddim2    = (pTr->dicnt0 * pTr->dicnt1);
        pTr->ddim3    = (pTr->dicnt0 * pTr->dicnt1 * pTr->dicnt2);
        pTr->daddr    = (uint64_t)startAddr + addrOffset[trIdx];

        pTr ++;
    }

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;

    /* Writeback cache */
    CacheP_wb(pTrpdMem, BOARD_TRPD_SIZE);

    return (numTR);
}

#endif /* #ifdef BOARD_DDR_ENABLE_DDR_MEM_PRIME */

