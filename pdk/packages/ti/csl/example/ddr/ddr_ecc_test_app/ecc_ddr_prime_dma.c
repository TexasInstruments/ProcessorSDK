/*
 *  Copyright (c) Texas Instruments Incorporated 2025
 *  All rights reserved.
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
 *  \file     ecc_ddr_prime_dma.c
 *
 *  \brief    This file implements DDR ECC UDMA priming functions
 */

#include <stdio.h>
#include <string.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/osal.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/udma.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Buffer Size in bytes */
#define ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES  (UDMA_CACHELINE_ALIGNMENT)

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define ECC_DDR_RING_ENTRIES                (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define ECC_DDR_RING_ENTRY_SIZE             (sizeof(uint64_t))
/** \brief Total ring memory */
#define ECC_DDR_RING_MEM_SIZE               (ECC_DDR_RING_ENTRIES * ECC_DDR_RING_ENTRY_SIZE)
/** \brief This ensures every channel memory is aligned */
#define ECC_DDR_RING_MEM_SIZE_ALIGN ((ECC_DDR_RING_MEM_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/** 
 * \brief Maximum no.of TR's. 
 * Size to prime is of 32 bytes. TR ICNT is 16 bytes and hence use max of 2 TR's to 
 * effectively split all cases, without causing any left over.
 *  */
#define ECC_DDR_MAX_TR                    (2U)
/* ICNT Split Size. This will be used to split to multiple TR's if required.
 * Set this to half of 0xFFFF(max of 16 bytes ICNT) */
#define ECC_DDR_UDMA_ICNT_SPLIT_SIZE      (0x8000U)

/**
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 */
#define ECC_DDR_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * (ECC_DDR_MAX_TR + 1U)) + 4U)
/** \brief This ensures every channel memory is aligned */
#define ECC_DDR_TRPD_SIZE_ALIGN   ((ECC_DDR_TRPD_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t ECC_DDR_udmaPrime(Udma_ChHandle chHandle, 
                        uint64_t startAddr,
                        uint32_t size);

static void ECC_DDR_udmaEventDmaCb(Udma_EventHandle eventHandle,
                                 uint32_t eventType,
                                 void *appData);
static void ECC_DDR_udmaEventTdCb(Udma_EventHandle eventHandle,
                                uint32_t eventType,
                                void *appData);

int32_t ECC_DDR_udmaInit(Udma_DrvHandle drvHandle);
int32_t ECC_DDR_udmaDeInit(Udma_DrvHandle drvHandle);

static int32_t ECC_DDR_udmaCreate(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);
static int32_t ECC_DDR_udmaDelete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);

static uint32_t ECC_DDR_udmaTrpdinit(Udma_ChHandle chHandle,
                                   uint8_t *pTrpdMem,
                                   uint64_t startAddr,
                                   uint32_t totalSize,
                                   const void *srcAddr,
                                   uint32_t srcBufSize);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver objects
 */
struct Udma_DrvObj      gECCDDRUdmaDrvObj;
struct Udma_ChObj       gECCDDRUdmaChObj;
struct Udma_EventObj    gECCDDRUdmaCqEventObj;
struct Udma_EventObj    gECCDDRUdmaTdCqEventObj;

/*
 * UDMA Memories
 */
uint8_t gECCDDRTxRingMem[ECC_DDR_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
uint8_t gECCDDRTxCompRingMem[ECC_DDR_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
uint8_t gECCDDRTxTdCompRingMem[ECC_DDR_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
uint8_t gECCDDRUdmaTrpdMem[ECC_DDR_TRPD_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

uint32_t gECCDDRUdmaPrimeSrcBuffer[ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES/4U] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/* Semaphore to indicate priming completion */
static SemaphoreP_Handle gECCDDRUdmaDDRPrimeDoneSem = NULL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t ECC_DDR_udmaPrimeDDR(uint64_t startAddr, uint32_t size)
{
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &gECCDDRUdmaDrvObj;
    Udma_ChHandle   chHandle = &gECCDDRUdmaChObj;
 
    retVal = ECC_DDR_udmaCreate(drvHandle, chHandle);
    if(retVal != CSL_PASS )
    {
        UART_printf("ECC DDR UDMA create failed!!\n");
    }

    if(retVal == CSL_PASS )
    {
        retVal = ECC_DDR_udmaPrime(chHandle, startAddr, size);
        if(retVal != CSL_PASS )
        {
            UART_printf("ECC DDR UDMA prime failed!!\n");
        }
    }

    retVal += ECC_DDR_udmaDelete(drvHandle, chHandle);
    if(retVal != CSL_PASS)
    {
        UART_printf("ECC DDR UDMA delete failed!!\n");
    }

    return (retVal);
}

int32_t ECC_DDR_udmaPrime(Udma_ChHandle chHandle, uint64_t startAddr, uint32_t size)
{
    int32_t         retVal = CSL_PASS;
    uint32_t       *pTrResp, trRespStatus;
    uint64_t        pDesc = 0;
    uint32_t       *srcBuf = &gECCDDRUdmaPrimeSrcBuffer[0U];
    uint8_t        *trpdMem = &gECCDDRUdmaTrpdMem[0U];
    uintptr_t       memPtr;
    uint32_t        offset;
    uint32_t        numTR;
    
    /* Init Src Buffer */
    for (offset = 0U; offset < ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES; offset += 4)
    {
        *((volatile uint32_t *) srcBuf) = 0x00000000U;
        srcBuf++;
    }
    /* Writeback buffer */
    CacheP_wb(&gECCDDRUdmaPrimeSrcBuffer[0U], ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES);

    /* Update TRPD and get no. of TR's used */
    numTR = ECC_DDR_udmaTrpdinit(chHandle, trpdMem, startAddr, size, &gECCDDRUdmaPrimeSrcBuffer[0U], ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES);

    /* Submit TRPD to channel */
    retVal = Udma_ringQueueRaw(
                    Udma_chGetFqRingHandle(chHandle),
                    (uint64_t) trpdMem);
    if(CSL_PASS != retVal)
    {
        UART_printf("[DMA] Channel queue failed!!\n");
    }

    if(CSL_PASS == retVal)
    {
        /* Wait for return descriptor in completion ring for the last channel
         * This marks the entire transfer completion */
        SemaphoreP_pend(gECCDDRUdmaDDRPrimeDoneSem, SemaphoreP_WAIT_FOREVER);

        /* Response received in completion queue */
        retVal =
            Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);

        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] No descriptor after callback!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(CSL_PASS == retVal)
    {
        /*
         * Sanity check
         */
        /* Check returned descriptor pointer */
        if(pDesc != ((uint64_t) trpdMem))
        {
            UART_printf("[DMA] TR descriptor pointer returned doesn't "
                    "match the submitted address!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(CSL_PASS == retVal)
    {
        /* Invalidate cache */
        CacheP_Inv(&gECCDDRUdmaTrpdMem[0U], ECC_DDR_TRPD_SIZE);

        /* check TR response status */
        pTrResp = (uint32_t *) (trpdMem + (sizeof(CSL_UdmapTR15) * (numTR + 1U)));
        trRespStatus = CSL_FEXT(*pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
        if(trRespStatus != CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE)
        {
            UART_printf("[DMA] TR Response not completed!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    /* Prime any chunk left over towards end, in case when total size is not multiple of ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES */
    if(size % ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES)
    {
        memPtr = (uintptr_t)startAddr + size;
        for (offset = 0U; offset <= (size % ECC_DDR_DDR_PRIME_BUFFER_NUM_BYTES); offset += 4)
        {
            memPtr -= 4;
            *((volatile uint32_t *) memPtr) = 0x00000000U;
        }
    }

    return (retVal);
}

static void ECC_DDR_udmaEventDmaCb(Udma_EventHandle eventHandle,
                                 uint32_t eventType,
                                 void *appData)
{
    if(UDMA_EVENT_TYPE_DMA_COMPLETION == eventType)
    {
        SemaphoreP_post(gECCDDRUdmaDDRPrimeDoneSem);
    }

    return;
}

static void ECC_DDR_udmaEventTdCb(Udma_EventHandle eventHandle,
                         uint32_t eventType,
                         void *appData)
{
    CSL_UdmapTdResponse tdResp;

    if(UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
    {
        /* Response received in Teardown completion queue */
        Udma_chDequeueTdResponse(&gECCDDRUdmaChObj, &tdResp);
    }

    return;
}

int32_t ECC_DDR_udmaInit(Udma_DrvHandle drvHandle)
{
    int32_t         retVal;
    Udma_InitPrms   initPrms;
    uint32_t        instId;

     /* Use MCU NAVSS for MCU domain cores. Rest cores all uses Main NAVSS */
#if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
    instId = UDMA_INST_ID_MCU_0;
#else
    instId = UDMA_INST_ID_MAIN_0;
#endif

    /* UDMA driver init */
    retVal = UdmaInitPrms_init(instId, &initPrms);
    if(CSL_PASS != retVal)
    {
        UART_printf("[DMA] UDMA init prms init failed!!\n");
    }
    else
    {
        retVal = Udma_init(drvHandle, &initPrms);
        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] UDMA init failed!!\n");
        }
    }
    return (retVal);
}

int32_t ECC_DDR_udmaDeInit(Udma_DrvHandle drvHandle)
{
    int32_t         retVal;
    
    retVal = Udma_deinit(drvHandle);
    if(CSL_PASS != retVal)
    {
        UART_printf("[DMA] UDMA deinit failed!!\n");
    }

    return (retVal);
}

static int32_t ECC_DDR_udmaCreate(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{

    int32_t             retVal = CSL_PASS;
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
    gECCDDRUdmaDDRPrimeDoneSem = SemaphoreP_create(0, &semPrms);
    if(NULL == gECCDDRUdmaDDRPrimeDoneSem)
    {
        UART_printf("[DMA] Sem create failed!!\n");
        retVal = UDMA_EFAIL;
    }

    if(CSL_PASS == retVal)
    {
        /* Init channel parameters */
        chType = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.ringMem       = &gECCDDRTxRingMem[0U];
        chPrms.cqRingPrms.ringMem       = &gECCDDRTxCompRingMem[0U];
        chPrms.tdCqRingPrms.ringMem     = &gECCDDRTxTdCompRingMem[0U];
        chPrms.fqRingPrms.ringMemSize   = ECC_DDR_RING_MEM_SIZE;
        chPrms.cqRingPrms.ringMemSize   = ECC_DDR_RING_MEM_SIZE;
        chPrms.tdCqRingPrms.ringMemSize = ECC_DDR_RING_MEM_SIZE;

        chPrms.fqRingPrms.elemCnt   = ECC_DDR_RING_ENTRIES;
        chPrms.cqRingPrms.elemCnt   = ECC_DDR_RING_ENTRIES;
        chPrms.tdCqRingPrms.elemCnt = ECC_DDR_RING_ENTRIES;

        /* Open channel for block copy */
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] UDMA channel open failed!!\n");
        }
    }

    if(CSL_PASS == retVal)
    {
        /* Config TX channel */
        UdmaChTxPrms_init(&txPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txPrms);
        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] UDMA TX channel config failed!!\n");
        }
    }

    if(CSL_PASS == retVal)
    {
        /* Config RX channel - which is implicitly paired to TX channel in
         * block copy mode */
        UdmaChRxPrms_init(&rxPrms, chType);
        retVal = Udma_chConfigRx(chHandle, &rxPrms);
        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] UDMA RX channel config failed!!\n");
        }
    }

    if(CSL_PASS == retVal)
    {
        /* Register ring completion callback */
        cqEventHandle = &gECCDDRUdmaCqEventObj;
        UdmaEventPrms_init(&cqEventPrms);
        cqEventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
        cqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        cqEventPrms.chHandle          = chHandle;
        cqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        cqEventPrms.eventCb           = &ECC_DDR_udmaEventDmaCb;
        retVal = Udma_eventRegister(drvHandle, cqEventHandle, &cqEventPrms);
        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] UDMA CQ event register failed!!\n");
        }
    }

    if(CSL_PASS == retVal)
    {
        /* Register teardown ring completion callback */
        tdCqEventHandle = &gECCDDRUdmaTdCqEventObj;
        UdmaEventPrms_init(&tdCqEventPrms);
        tdCqEventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
        tdCqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        tdCqEventPrms.chHandle          = chHandle;
        tdCqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        tdCqEventPrms.eventCb           = &ECC_DDR_udmaEventTdCb;
        retVal = Udma_eventRegister(drvHandle, tdCqEventHandle, &tdCqEventPrms);
        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] UDMA Teardown CQ event register failed!!\n");
        }
    }

    if(CSL_PASS == retVal)
    {
        /* Channel enable */
        retVal = Udma_chEnable(chHandle);
        if(CSL_PASS != retVal)
        {
            UART_printf("[DMA] UDMA channel enable failed!!\n");
        }
    }

    return (retVal);
}

static int32_t ECC_DDR_udmaDelete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{
    int32_t         retVal = CSL_PASS, tempRetVal;
    uint64_t        pDesc;
    Udma_EventHandle    cqEventHandle;
    Udma_EventHandle    tdCqEventHandle;

    retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
    if(CSL_PASS != retVal)
    {
        UART_printf("[DMA] UDMA channel disable failed!!\n");
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
    cqEventHandle = &gECCDDRUdmaCqEventObj;
    retVal += Udma_eventUnRegister(cqEventHandle);
    if(CSL_PASS != retVal)
    {
        UART_printf("[DMA] UDMA event unregister failed!!\n");
    }
    tdCqEventHandle = &gECCDDRUdmaTdCqEventObj;
    retVal += Udma_eventUnRegister(tdCqEventHandle);
    if(CSL_PASS != retVal)
    {
        UART_printf("[DMA] UDMA event unregister failed!!\n");
    }

    retVal += Udma_chClose(chHandle);
    if(CSL_PASS != retVal)
    {
        UART_printf("[DMA] UDMA channel close failed!!\n");
    }

    if(gECCDDRUdmaDDRPrimeDoneSem != NULL)
    {
        SemaphoreP_delete(gECCDDRUdmaDDRPrimeDoneSem);
        gECCDDRUdmaDDRPrimeDoneSem = NULL;
    }

    return (retVal);
}

static uint32_t ECC_DDR_udmaTrpdinit(Udma_ChHandle chHandle,
                                   uint8_t *pTrpdMem,
                                   uint64_t startAddr,
                                   uint32_t totalSize,
                                   const void *srcAddr,
                                   uint32_t srcBufSize)
{
    CSL_UdmapCppi5TRPD  *pTrpd  = (CSL_UdmapCppi5TRPD *) pTrpdMem;
    CSL_UdmapTR15       *pTr    = (CSL_UdmapTR15 *)(pTrpdMem + sizeof(CSL_UdmapTR15));

    uint32_t *pTrResp;
    uint32_t cqRingNum = Udma_chGetCqRingNum(chHandle);

    uint32_t numTR = 1, trIdx;
    uint16_t icnt[ECC_DDR_MAX_TR][4] = {0};
    uint32_t addrOffset[ECC_DDR_MAX_TR] = {0};

    uint32_t remainder    = totalSize/srcBufSize;
    uint32_t residue      = remainder & (ECC_DDR_UDMA_ICNT_SPLIT_SIZE - 1U);

    /* Calculate number of TR's */
    if (remainder < ECC_DDR_UDMA_ICNT_SPLIT_SIZE)
    {
        numTR = 1U;
    }
    else if ((remainder > ECC_DDR_UDMA_ICNT_SPLIT_SIZE) && (0U != residue))
    {
        numTR = 2U;
    }
    else
    {
        numTR = 1U;
    }

    pTrResp = (uint32_t *)(pTrpdMem + (sizeof(CSL_UdmapTR15) * (numTR + 1U)));
    
    if (remainder < ECC_DDR_UDMA_ICNT_SPLIT_SIZE)
    {
        icnt[0][0] = (uint16_t)srcBufSize;
        icnt[0][1] = (uint16_t)remainder;
        icnt[0][2] = (uint16_t)1U;
        icnt[0][3] = (uint16_t)1U;
    }
    else
    {
        icnt[0][0] = (uint16_t)srcBufSize;
        icnt[0][1] = (uint16_t)ECC_DDR_UDMA_ICNT_SPLIT_SIZE;
        icnt[0][2] = (uint16_t)(remainder / ECC_DDR_UDMA_ICNT_SPLIT_SIZE);
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
    CacheP_wb(pTrpdMem, ECC_DDR_TRPD_SIZE);

    return (numTR);
}