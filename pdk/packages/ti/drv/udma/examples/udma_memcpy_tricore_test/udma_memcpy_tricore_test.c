/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file udma_memcpy_tricore_test.c
 *
 *  \brief UDMA memory copy sample application performing block copy using
 *  Type 15 Transfer Record (TR15) using Transfer Record Packet
 *  Descriptor (TRPD) running in three cores simultaneously.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/examples/udma_apputils/udma_apputils.h>
#include <ti/csl/arch/r5/csl_arm_r5.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Application test parameters
 */
/** \brief Number of bytes to copy and buffer allocation */
#define UDMA_TEST_APP_NUM_BYTES         (1024U)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_NUM_BYTES_ALIGN    ((UDMA_TEST_APP_NUM_BYTES + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/* Enable benchmark profiling - can be used only for baremetal */
//#define UDMA_TEST_APP_PROFILE

#ifdef UDMA_TEST_APP_PROFILE

/* Enable PMU Cycle measurement */
#if defined (BUILD_MCU)
#define UDMA_TEST_APP_PMU_PROFILE
#endif

/** \brief In case of profiling, Set Number of times to perform the memcpy operation as 1  */
#define UDMA_TEST_APP_LOOP_CNT                      (1U)
/** \brief In case of profiling, Set Number of times to perform the channel open close operation as 1 */
#define UDMA_TEST_APP_OPEN_CLOSE_TEST_LOOP_CNT      (1U)
#else
/** \brief Number of times to perform the memcpy operation*/
#define UDMA_TEST_APP_LOOP_CNT                      (100U)
/** \brief Number of times to perform the channel open close operation */
#define UDMA_TEST_APP_OPEN_CLOSE_TEST_LOOP_CNT      (10U)
#endif

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define UDMA_TEST_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_TEST_APP_RING_MEM_SIZE     (UDMA_TEST_APP_RING_ENTRIES * UDMA_TEST_APP_RING_ENTRY_SIZE)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_RING_MEM_SIZE_ALIGN ((UDMA_TEST_APP_RING_MEM_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))
/**
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 */
#define UDMA_TEST_APP_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * 2U) + 4U)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_TRPD_SIZE_ALIGN   ((UDMA_TEST_APP_TRPD_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/* Enable interrupt instead of polling mode */
#define UDMA_TEST_INTR

/* Override default RM Shared Resource parameters */
//#define UDMA_OVERRIDE_DEF_RM_SHARED_RES_PRMS

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t App_memcpyTest(Udma_ChHandle chHandle);
static int32_t App_udmaMemcpy(Udma_ChHandle chHandle,
                              void *destBuf,
                              void *srcBuf,
                              uint32_t length);
#if defined (UDMA_TEST_INTR)
static void App_udmaEventDmaCb(Udma_EventHandle eventHandle,
                               uint32_t eventType,
                               void *appData);
#endif
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static void App_udmaEventTdCb(Udma_EventHandle eventHandle,
                              uint32_t eventType,
                              void *appData);
#endif

static int32_t App_init(Udma_DrvHandle drvHandle);
static int32_t App_deinit(Udma_DrvHandle drvHandle);

static int32_t App_create(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);
static int32_t App_delete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);

static void App_udmaTrpdInit(Udma_ChHandle chHandle,
                             uint8_t *pTrpdMem,
                             const void *destBuf,
                             const void *srcBuf,
                             uint32_t length);

static void App_print(const char *str);
static void App_printNum(const char *str, uint32_t num);

#ifdef UDMA_TEST_APP_PMU_PROFILE
#include <ti/csl/arch/csl_arch.h>
extern uint32_t osal_TimestampProvider_getOverflowCCNT(void);

static uint64_t App_getPmuCycleCnt (uint32_t hiCycleCnt);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver objects
 */
struct Udma_DrvObj      gUdmaDrvObj;
struct Udma_ChObj       gUdmaChObj;
#if defined (UDMA_TEST_INTR)
struct Udma_EventObj    gUdmaCqEventObj;
#endif
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
struct Udma_EventObj    gUdmaTdCqEventObj;
#endif

/*
 * UDMA Memories
 */
static uint8_t gTxRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static uint8_t gTxCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gTxTdCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#endif
static uint8_t gUdmaTrpdMem[UDMA_TEST_APP_TRPD_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/*
 * Application Buffers
 */
static uint8_t gUdmaTestSrcBuf[UDMA_TEST_APP_NUM_BYTES_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmaTestDestBuf[UDMA_TEST_APP_NUM_BYTES_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

#if defined (UDMA_TEST_INTR)
/* Semaphore to indicate transfer completion */
static SemaphoreP_Handle gUdmaAppDoneSem = NULL;
#endif

#ifdef UDMA_TEST_APP_PROFILE
/*
 * Profiling Params
 */
static uint64_t gUdmaTotalTime;
#ifdef UDMA_TEST_APP_PMU_PROFILE     
static uint64_t gUdmaTotalCycle;
#endif
#endif

/* Global test pass/fail flag */
static volatile int32_t gUdmaAppResult = UDMA_SOK;

/* Used for assigning buffers to cores */
CSL_ArmR5CPUInfo cpuInfo;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * UDMA memcpy test
 */
int32_t Udma_memcpyTest(void)
{
    int32_t         retVal;
    uint32_t        loopCnt = 0U;
    Udma_DrvHandle  drvHandle = &gUdmaDrvObj;
    Udma_ChHandle   chHandle = &gUdmaChObj;

    CSL_armR5GetCpuID(&cpuInfo);

    App_print("UDMA memcpy application started...\n");
    
    retVal = App_init(drvHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA App init failed!!\n");
    }

    while(loopCnt < UDMA_TEST_APP_OPEN_CLOSE_TEST_LOOP_CNT)
    {
        if(UDMA_SOK == retVal)
        {
            retVal = App_create(drvHandle, chHandle);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA App create failed!!\n");
            }
        }

        if(UDMA_SOK == retVal)
        {
            retVal = App_memcpyTest(chHandle);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA App memcpy test failed!!\n");
            }
            else
            {
                if (UTRUE == Udma_appIsUdmapStatsSupported())
                {
                    Udma_ChStats    chStats;
                    retVal = Udma_chGetStats(chHandle, &chStats);
                    if(UDMA_SOK == retVal)
                    {
                        App_print("UDMA App memcpy test statistics:\n");
                        App_printNum("Completed packet count       : %d\n", chStats.packetCnt);
                        App_printNum("Completed payload byte count : %d\n", chStats.completedByteCnt);
                        App_printNum("Started byte count           : %d\n", chStats.startedByteCnt);
                    }
                }
#ifdef UDMA_TEST_APP_PROFILE
             App_printNum("Copied %d bytes in ", UDMA_TEST_APP_NUM_BYTES);
#ifdef UDMA_TEST_APP_PMU_PROFILE
             App_printNum("%lu PMU cycles, ",gUdmaTotalCycle);
#endif
             App_printNum("%d usec.\n",gUdmaTotalTime);
#endif
            }
        }

        retVal += App_delete(drvHandle, chHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA App delete failed!!\n");
        }

        loopCnt++;
    }

    retVal += App_deinit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA App deinit failed!!\n");
    }

    #if defined LDRA_DYN_COVERAGE_EXIT
    App_print("\nLDRA ENTRY: UDMA MEMCPY!!\n");
    upload_execution_history();
    App_print("\nLDRA EXIT: UDMA MEMCPY!!\n");
    #endif
    
    if((UDMA_SOK == retVal) && (UDMA_SOK == gUdmaAppResult))
    {
        App_print("UDMA memcpy using TR15 block copy Passed!!\n");
        App_print("All tests have passed!!\n");
    }
    else
    {
        App_print("UDMA memcpy using TR15 block copy Failed!!\n");
        App_print("Some tests have failed!!\n");
    }

    return (0);
}

static int32_t App_memcpyTest(Udma_ChHandle chHandle)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            loopCnt = 0U;
    uint8_t            *srcBuf = &gUdmaTestSrcBuf[0U];
    uint8_t            *destBuf = &gUdmaTestDestBuf[0U];

    // App_print("SrcBuf at 0x");
    // App_printNum("%x and DestBuf at 0x", (uint32_t) srcBuf);
    // App_printNum("%x\n", (uint32_t) destBuf);

#ifndef UDMA_TEST_APP_PROFILE    
    uint32_t            i;
    /* Init buffers */
    for(i = 0U; i < UDMA_TEST_APP_NUM_BYTES; i++)
    {
        srcBuf[i] = i;
        destBuf[i] = 0U;
    }
    /* Writeback source and destination buffer */
    Udma_appUtilsCacheWb(&gUdmaTestSrcBuf[0U], UDMA_TEST_APP_NUM_BYTES);
    Udma_appUtilsCacheWb(&gUdmaTestDestBuf[0U], UDMA_TEST_APP_NUM_BYTES);
#endif

    while(loopCnt < UDMA_TEST_APP_LOOP_CNT)
    {
        /* Perform UDMA memcpy */
        retVal = App_udmaMemcpy(
                     chHandle,
                     destBuf,
                     srcBuf,
                     UDMA_TEST_APP_NUM_BYTES);
#ifndef UDMA_TEST_APP_PROFILE        
        if(UDMA_SOK == retVal)
        {
            /* Compare data */
            /* Invalidate destination buffer */
            Udma_appUtilsCacheInv(&gUdmaTestDestBuf[0U], UDMA_TEST_APP_NUM_BYTES);
            for(i = 0U; i < UDMA_TEST_APP_NUM_BYTES; i++)
            {
                if(srcBuf[i] != destBuf[i])
                {
                    App_print("[Error] Data mismatch!!\n");
                    retVal = UDMA_EFAIL;
                    break;
                }
            }
        }
#endif

        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    return (retVal);
}

static int32_t App_udmaMemcpy(Udma_ChHandle chHandle,
                              void *destBuf,
                              void *srcBuf,
                              uint32_t length)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t   *pTrResp, trRespStatus;
    uint64_t    pDesc = 0;
    uint8_t    *trpdMem = &gUdmaTrpdMem[0U];

    /* Update TR packet descriptor */
    App_udmaTrpdInit(chHandle, trpdMem, destBuf, srcBuf, length);

#ifdef UDMA_TEST_APP_PROFILE
    /* Initialize the profiling time counters and start */
    gUdmaTotalTime  = TimerP_getTimeInUsecs();
#ifdef UDMA_TEST_APP_PMU_PROFILE
    gUdmaTotalCycle = App_getPmuCycleCnt((uint64_t)0U>>32U);
#endif
#endif
    /* Submit TRPD to channel */
    retVal = Udma_ringQueueRaw(
                 Udma_chGetFqRingHandle(chHandle), (uint64_t) Udma_appVirtToPhyFxn(trpdMem, 0U, NULL));
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] Channel queue failed!!\n");
    }

#if defined (UDMA_TEST_INTR)
    if(UDMA_SOK == retVal)
    {
        /* Wait for return descriptor in completion ring - this marks the
         * transfer completion */
        SemaphoreP_pend(gUdmaAppDoneSem, SemaphoreP_WAIT_FOREVER);

        /* Response received in completion queue */
        retVal = Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] No descriptor after callback!!\n");
            retVal = UDMA_EFAIL;
        }
    }
#else
    if(UDMA_SOK == retVal)
    {
        /* Wait for return descriptor in completion ring - this marks the
         * transfer completion */
        while(1)
        {
            /* Response received in completion queue */
            retVal = Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);
            if(UDMA_SOK == retVal)
            {
                break;
            }
        }
    }
#endif
#ifdef UDMA_TEST_APP_PROFILE
    /* Stop Timer */
#ifdef UDMA_TEST_APP_PMU_PROFILE
    gUdmaTotalCycle = App_getPmuCycleCnt(gUdmaTotalCycle>>32U) - gUdmaTotalCycle;
#endif
    gUdmaTotalTime  = TimerP_getTimeInUsecs() - gUdmaTotalTime;
#endif

    if(UDMA_SOK == retVal)
    {
        /*
         * Sanity check
         */
        /* Check returned descriptor pointer */
        if(((uint64_t) Udma_appPhyToVirtFxn(pDesc, 0U, NULL)) != ((uint64_t) trpdMem))
        {
            App_print("[Error] TR descriptor pointer returned doesn't "
                   "match the submitted address!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Invalidate cache */
        Udma_appUtilsCacheInv(&gUdmaTrpdMem[0U], UDMA_TEST_APP_TRPD_SIZE);

        /* check TR response status */
        pTrResp = (uint32_t *) (trpdMem + (sizeof(CSL_UdmapTR15) * 2U));
        trRespStatus = CSL_FEXT(*pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
        if(trRespStatus != CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE)
        {
            App_print("[Error] TR Response not completed!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return (retVal);
}

#if defined (UDMA_TEST_INTR)
static void App_udmaEventDmaCb(Udma_EventHandle eventHandle,
                               uint32_t eventType,
                               void *appData)
{
    if(UDMA_EVENT_TYPE_DMA_COMPLETION == eventType)
    {
        SemaphoreP_post(gUdmaAppDoneSem);
    }
    else
    {
        gUdmaAppResult = UDMA_EFAIL;
    }

    return;
}
#endif
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static void App_udmaEventTdCb(Udma_EventHandle eventHandle,
                              uint32_t eventType,
                              void *appData)
{
    int32_t             retVal;
    CSL_UdmapTdResponse tdResp;

    if(UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
    {
        /* Response received in Teardown completion queue */
        retVal = Udma_chDequeueTdResponse(&gUdmaChObj, &tdResp);
        if(UDMA_SOK != retVal)
        {
            /* [Error] No TD response after callback!! */
            gUdmaAppResult = UDMA_EFAIL;
        }
    }
    else
    {
        gUdmaAppResult = UDMA_EFAIL;
    }

    return;
}
#endif

static int32_t App_init(Udma_DrvHandle drvHandle)
{
    int32_t         retVal;
    Udma_InitPrms   initPrms;
    uint32_t        instId;
#if defined (SOC_AM64X)
    /* Use Block Copy DMA for AM64x */
    instId = UDMA_INST_ID_BCDMA_0;
#else
    /* Use MCU NAVSS for MCU domain cores. Rest cores all uses Main NAVSS */
#if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
    instId = UDMA_INST_ID_MCU_0;
#else
    instId = UDMA_INST_ID_MAIN_0;
#endif
#endif

#if defined UDMA_OVERRIDE_DEF_RM_SHARED_RES_PRMS
    /* Override the default RM Shared Resource parameters. 
    *  For example, using maximum no.of available resources of Global Events
    *  for current instance and updating mimium requirement to 10U */
    Udma_RmSharedResPrms *rmSharedResPrms;
    
    rmSharedResPrms = Udma_rmGetSharedResPrms(UDMA_RM_RES_ID_GLOBAL_EVENT);
    if(NULL_PTR != rmSharedResPrms)
    {
        rmSharedResPrms->instShare[instId] = UDMA_RM_SHARED_RES_CNT_REST;
        rmSharedResPrms->minReq            = 10U;
        /* #UdmaInitPrms_init makes use of this information, 
         * in initilaizing #Udma_RmInitPrms */
    }
    else
    {
        App_print("Global Event is not a shared resource!!\n");
    }
#endif

    /* UDMA driver init */
    retVal = UdmaInitPrms_init(instId, &initPrms);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA init prms init failed!!\n");
    }
    else
    {
        initPrms.virtToPhyFxn   = &Udma_appVirtToPhyFxn;
        initPrms.phyToVirtFxn   = &Udma_appPhyToVirtFxn;
        initPrms.printFxn       = &App_print;
        retVal = Udma_init(drvHandle, &initPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA init failed!!\n");
        }
    }

    return (retVal);
}

static int32_t App_deinit(Udma_DrvHandle drvHandle)
{
    int32_t     retVal;

    retVal = Udma_deinit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA deinit failed!!\n");
    }

    return (retVal);
}

static int32_t App_create(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    Udma_EventHandle    tdCqEventHandle;
    Udma_EventPrms      tdCqEventPrms;
#endif
#if defined (UDMA_TEST_INTR)
    Udma_EventHandle    cqEventHandle;
    Udma_EventPrms      cqEventPrms;
    SemaphoreP_Params   semPrms;

    SemaphoreP_Params_init(&semPrms);
    gUdmaAppDoneSem = SemaphoreP_create(0, &semPrms);
    if(NULL == gUdmaAppDoneSem)
    {
        App_print("[Error] Sem create failed!!\n");
        retVal = UDMA_EFAIL;
    }
#endif

    if(UDMA_SOK == retVal)
    {
        /* Init channel parameters */
        chType = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.ringMem       = &gTxRingMem[0U];
        chPrms.fqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.fqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
        chPrms.cqRingPrms.ringMem       = &gTxCompRingMem[0U];
        chPrms.cqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.cqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.tdCqRingPrms.ringMem     = &gTxTdCompRingMem[0U];
        chPrms.tdCqRingPrms.ringMemSize = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.tdCqRingPrms.elemCnt     = UDMA_TEST_APP_RING_ENTRIES;
#endif

        /* Open channel for block copy */
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA channel open failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Config TX channel */
        UdmaChTxPrms_init(&txPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA TX channel config failed!!\n");
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
            App_print("[Error] UDMA RX channel config failed!!\n");
        }
    }

#if defined (UDMA_TEST_INTR)
    if(UDMA_SOK == retVal)
    {
        /* Register ring completion callback */
        cqEventHandle = &gUdmaCqEventObj;
        UdmaEventPrms_init(&cqEventPrms);
        cqEventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
        cqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        cqEventPrms.chHandle          = chHandle;
        cqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        cqEventPrms.eventCb           = &App_udmaEventDmaCb;
        retVal = Udma_eventRegister(drvHandle, cqEventHandle, &cqEventPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA CQ event register failed!!\n");
        }
    }
#endif
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    if(UDMA_SOK == retVal)
    {
        /* Register teardown ring completion callback */
        tdCqEventHandle = &gUdmaTdCqEventObj;
        UdmaEventPrms_init(&tdCqEventPrms);
        tdCqEventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
        tdCqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        tdCqEventPrms.chHandle          = chHandle;
        tdCqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        tdCqEventPrms.eventCb           = &App_udmaEventTdCb;
        retVal = Udma_eventRegister(drvHandle, tdCqEventHandle, &tdCqEventPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA Teardown CQ event register failed!!\n");
        }
    }
#endif

    if(UDMA_SOK == retVal)
    {
        /* Channel enable */
        retVal = Udma_chEnable(chHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA channel enable failed!!\n");
        }
    }

    return (retVal);
}

static int32_t App_delete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{
    int32_t             retVal, tempRetVal;
    uint64_t            pDesc;
#if defined (UDMA_TEST_INTR)
    Udma_EventHandle    cqEventHandle;
#endif
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    Udma_EventHandle    tdCqEventHandle;
#endif

    retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA channel disable failed!!\n");
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
#if defined (UDMA_TEST_INTR)
    cqEventHandle = &gUdmaCqEventObj;
    retVal += Udma_eventUnRegister(cqEventHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA event unregister failed!!\n");
    }
#endif
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    tdCqEventHandle = &gUdmaTdCqEventObj;
    retVal += Udma_eventUnRegister(tdCqEventHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA event unregister failed!!\n");
    }
#endif

    retVal += Udma_chClose(chHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA channel close failed!!\n");
    }

#if defined (UDMA_TEST_INTR)
    if(gUdmaAppDoneSem != NULL)
    {
        SemaphoreP_delete(gUdmaAppDoneSem);
        gUdmaAppDoneSem = NULL;
    }
#endif

    return (retVal);
}

static void App_udmaTrpdInit(Udma_ChHandle chHandle,
                             uint8_t *pTrpdMem,
                             const void *destBuf,
                             const void *srcBuf,
                             uint32_t length)
{
    CSL_UdmapCppi5TRPD *pTrpd = (CSL_UdmapCppi5TRPD *) pTrpdMem;
    CSL_UdmapTR15 *pTr = (CSL_UdmapTR15 *)UdmaUtils_getTrpdTr1Pointer(pTrpdMem, 0U);
    uint32_t *pTrResp = (uint32_t *) (pTrpdMem + (sizeof(CSL_UdmapTR15) * 2U));
    uint32_t cqRingNum = Udma_chGetCqRingNum(chHandle);

    /* Make TRPD */
    UdmaUtils_makeTrpd(pTrpd, UDMA_TR_TYPE_15, 1U, cqRingNum);

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
    pTr->icnt0    = length;
    pTr->icnt1    = 1U;
    pTr->icnt2    = 1U;
    pTr->icnt3    = 1U;
    pTr->dim1     = pTr->icnt0;
    pTr->dim2     = (pTr->icnt0 * pTr->icnt1);
    pTr->dim3     = (pTr->icnt0 * pTr->icnt1 * pTr->icnt2);
    pTr->addr     = (uint64_t) Udma_appVirtToPhyFxn(srcBuf, 0U, NULL);
    pTr->fmtflags = 0x00000000U;        /* Linear addressing, 1 byte per elem.
                                           Replace with CSL-FL API */
    pTr->dicnt0   = length;
    pTr->dicnt1   = 1U;
    pTr->dicnt2   = 1U;
    pTr->dicnt3   = 1U;
    pTr->ddim1    = pTr->dicnt0;
    pTr->ddim2    = (pTr->dicnt0 * pTr->dicnt1);
    pTr->ddim3    = (pTr->dicnt0 * pTr->dicnt1 * pTr->dicnt2);
    pTr->daddr    = (uint64_t) Udma_appVirtToPhyFxn(destBuf, 0U, NULL);

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;

    /* Writeback cache */
    Udma_appUtilsCacheWb(pTrpdMem, UDMA_TEST_APP_TRPD_SIZE);

    return;
}

static void App_print(const char *str)
{
    UART_printf("[CORE %d_%d] %s" ,cpuInfo.grpId+1 , cpuInfo.cpuID, str);
    return;
}
static void App_printNum(const char *str, uint32_t num)
{
    static char printBuf[200U];

    snprintf(printBuf, 200U, str, num);
    UART_printf("%s", printBuf);

    if(UTRUE == Udma_appIsPrintSupported())
    {
        printf("%s", printBuf);
    }

    return;
}

#ifdef UDMA_TEST_APP_PMU_PROFILE
static uint64_t App_getPmuCycleCnt (uint32_t hiCycleCnt)
{
    uint32_t    ovsrStatus;
    uint32_t    hiCnt = hiCycleCnt;
    uint64_t    cycleCnt;
    

    cycleCnt = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
    ovsrStatus = osal_TimestampProvider_getOverflowCCNT();
    if (ovsrStatus != 0U)
    {
        hiCnt++;
    }
    cycleCnt |= (uint64_t)hiCnt << 32U;

    return (cycleCnt);
}
#endif
