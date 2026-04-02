/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file udma_sw_trigger_test.c
 *
 *  \brief UDMA SW trigger sample application performs 3D transfer using
 *  SW trigger method as below
 *
 *  Loop N times (icnt2)
 *      - SW trigger CH 0 -> Triggers DDR to IRAM
 *      - Wait for CH 0 icnt0 x icnt1 to complete
 *      - SW trigger Channel 1 -> Triggers IRAM to DDR
 *      - Wait for CH 1 icnt0 x icnt1 to complete
 *
 *  Each loop transfers M (icnt0 x icnt1) bytes of data
 *  DDR size is M x N and IRAM size is just M bytes.
 *  IRAM memory set to wrap around after M bytes of transfer
 *
 *  Where,
 *      - M is icnt0 = UDMA_TEST_1D_SIZE x icnt1 = UDMA_TEST_2D_SIZE
 *      - N is icnt2 = UDMA_TEST_3D_SIZE
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/osal/TaskP.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/examples/udma_apputils/udma_apputils.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Application test config parameters
 */
/** \brief 1D Size */
#define UDMA_TEST_1D_SIZE                   (32U)
/** \brief 2D Size */
#define UDMA_TEST_2D_SIZE                   (4U)
/** \brief 3D Size */
#define UDMA_TEST_3D_SIZE                   (10U)
/* Enable interrupt instead of polling mode */
#define UDMA_TEST_INTR

/*
 * Application other test parameters
 */
/** \brief Number of bytes to copy and buffer allocation */
#define UDMA_TEST_APP_NUM_BYTES             (UDMA_TEST_1D_SIZE * UDMA_TEST_2D_SIZE * UDMA_TEST_3D_SIZE)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_BUF_SIZE_ALIGN        ((UDMA_TEST_APP_NUM_BYTES + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))
/** \brief Number of bytes to alloc is IRAM */
#define UDMA_TEST_APP_NUM_BYTES_IRAM        (UDMA_TEST_1D_SIZE * UDMA_TEST_2D_SIZE)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_BUF_SIZE_IRAM_ALIGN   ((UDMA_TEST_APP_NUM_BYTES_IRAM + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))
/** \brief Number of times to perform the memcpy operation */
#define UDMA_TEST_APP_LOOP_CNT              (1U)
/** \brief Number of channels */
#define UDMA_TEST_APP_NUM_CH                (2U)

/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define UDMA_TEST_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_TEST_APP_RING_MEM_SIZE     (UDMA_TEST_APP_RING_ENTRIES * \
                                         UDMA_TEST_APP_RING_ENTRY_SIZE)
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

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    int32_t                 chIdx;
    struct Udma_ChObj       chObj;
    struct Udma_EventObj    trEventObj;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    struct Udma_EventObj    tdCqEventObj;
#endif

    Udma_ChHandle           chHandle;
    Udma_EventHandle        trEventHandle;
    Udma_EventPrms          trEventPrms;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    Udma_EventHandle        tdCqEventHandle;
    Udma_EventPrms          tdCqEventPrms;
#endif

    uint32_t                trigger;
    /**< Global0 or Global 1 Trigger - refer \ref CSL_UdmapTrFlagsTrigger. */
    uint32_t                eventSize;
    /**< Refer \ref CSL_UdmapTrFlagsEventSize. */
    uint32_t                triggerType;
    /**< Refer \ref CSL_UdmapTrFlagsTriggerType. */
    uint32_t                eolType;
    /**< Refer \ref CSL_UdmapTrFlagsEol. */
    volatile uint32_t      *pSwTriggerReg;
    uint32_t                triggerMask;

    Udma_DrvHandle          drvHandle;
    SemaphoreP_Handle       transferDoneSem;

    uint8_t                 *txRingMem;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    uint8_t                 *txCompRingMem;
    uint8_t                 *txTdCompRingMem;
#endif
    uint8_t                 *trpdMem;

    uint8_t                 *srcBuf;
    uint8_t                 *destBuf;
} App_UdmaChObj;

typedef struct
{
    struct Udma_DrvObj      drvObj;
    App_UdmaChObj           appChObj[UDMA_TEST_APP_NUM_CH];
} App_UdmaObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t App_swTriggerTest(App_UdmaObj *appObj);
static int32_t App_udmaSwTrigger(App_UdmaObj *appObj);

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

static int32_t App_init(App_UdmaObj *appObj);
static int32_t App_deinit(App_UdmaObj *appObj);

static int32_t App_create(App_UdmaObj *appObj);
static int32_t App_delete(App_UdmaObj *appObj);

static void App_udmaTrpdInit(App_UdmaChObj *appChObj);

static void App_print(const char *str);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver and channel objects
 */
App_UdmaObj gUdmaAppObj;

/*
 * UDMA Memories
 */
static uint8_t gTxRingMem[UDMA_TEST_APP_NUM_CH][UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static uint8_t gTxCompRingMem[UDMA_TEST_APP_NUM_CH][UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gTxTdCompRingMem[UDMA_TEST_APP_NUM_CH][UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#endif
static uint8_t gUdmaTrpdMem[UDMA_TEST_APP_NUM_CH][UDMA_TEST_APP_TRPD_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/*
 * Application Buffers
 */
static uint8_t gUdmaTestSrcBuf[UDMA_TEST_APP_BUF_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmaTestDestBuf[UDMA_TEST_APP_BUF_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmaTestIramBuf[UDMA_TEST_APP_BUF_SIZE_IRAM_ALIGN] __attribute__(( aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_msmc") ));

/* Global test pass/fail flag */
static volatile int32_t gUdmaAppResult = UDMA_SOK;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * UDMA SW Trigger test
 */
int32_t Udma_swTriggerTest(void)
{
    int32_t         retVal;
    App_UdmaObj    *appObj = &gUdmaAppObj;

    retVal = App_init(appObj);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA App init failed!!\n");
    }

    App_print("UDMA SW trigger application started...\n");

    if(UDMA_SOK == retVal)
    {
        retVal = App_create(appObj);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA App create failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        retVal = App_swTriggerTest(appObj);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA App SW trigger test failed!!\n");
        }
    }

    retVal += App_delete(appObj);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA App delete failed!!\n");
    }

    retVal += App_deinit(appObj);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA App deinit failed!!\n");
    }

    #if defined LDRA_DYN_COVERAGE_EXIT
    App_print("\nLDRA ENTRY: UDMA SW TRIGGER!!\n");
    upload_execution_history();
    App_print("\nLDRA EXIT: UDMA SW TRIGGER!!\n");
    #endif
    
    if((UDMA_SOK == retVal) && (UDMA_SOK == gUdmaAppResult))
    {
        App_print("UDMA SW trigger Passed!!\n");
        App_print("All tests have passed!!\n");
    }
    else
    {
        App_print("UDMA SW trigger Failed!!\n");
        App_print("Some tests have failed!!\n");
    }

    return (0);
}

static int32_t App_swTriggerTest(App_UdmaObj *appObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t        loopCnt = 0U;
    uint32_t        i;
    uint8_t        *destBuf;

    while(loopCnt < UDMA_TEST_APP_LOOP_CNT)
    {
        /* Reset dest buffers */
        destBuf  = &gUdmaTestDestBuf[0U];
        for(i = 0U; i < UDMA_TEST_APP_NUM_BYTES; i++)
        {
            destBuf[i] = 0U;
        }
        /* Writeback destination buffer */
        Udma_appUtilsCacheWb(destBuf, UDMA_TEST_APP_NUM_BYTES);

        /* Perform UDMA memcpy */
        retVal = App_udmaSwTrigger(appObj);
        if(UDMA_SOK != retVal)
        {
            break;
        }

        loopCnt++;
    }

    return (retVal);
}

static int32_t App_udmaSwTrigger(App_UdmaObj *appObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t       *pTrResp, trRespStatus;
    uint8_t        *trpdMem;
    uint64_t        pDesc = 0;
    int32_t         chIdx;
    uint32_t        i;
    App_UdmaChObj  *appChObj;
    Udma_ChHandle   chHandle;
    uint32_t        triggerCnt, tCnt;
    uint8_t        *srcBuf, *destBuf;

    for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
    {
        appChObj = &appObj->appChObj[chIdx];
        chHandle = appChObj->chHandle;

        /* Get SW trigger register for easy access */
        appChObj->triggerMask = ((uint32_t)1U << (appChObj->trigger - 1U));
        appChObj->pSwTriggerReg = (volatile uint32_t *) Udma_chGetSwTriggerRegister(chHandle);
        if(NULL == appChObj->pSwTriggerReg)
        {
            App_print("[Error] Channel trigger register get failed!!\n");
            break;
        }

        /* Submit TRPD to channels */
        App_udmaTrpdInit(appChObj);
        retVal = Udma_ringQueueRaw(
                     Udma_chGetFqRingHandle(chHandle),
                     (uint64_t) Udma_appVirtToPhyFxn(appChObj->trpdMem, 0U, NULL));
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] Channel queue failed!!\n");
            break;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Set number of times to trigger based on event size */
        triggerCnt = UDMA_TEST_3D_SIZE;
        for(tCnt = 0U; tCnt < triggerCnt; tCnt++)
        {
            /* Set channel 0 trigger and wait for completion */
            appChObj = &appObj->appChObj[0U];
            CSL_REG32_WR(appChObj->pSwTriggerReg, appChObj->triggerMask);
#if defined (UDMA_TEST_INTR)
            SemaphoreP_pend(appChObj->transferDoneSem, SemaphoreP_WAIT_FOREVER);
#else
            while(1U)
            {
                volatile uint64_t   intrStatusReg;
                intrStatusReg = CSL_REG64_RD(appChObj->trEventPrms.intrStatusReg);
                if(intrStatusReg & appChObj->trEventPrms.intrMask)
                {
                    /* Clear interrupt */
                    CSL_REG64_WR(appChObj->trEventPrms.intrClearReg, appChObj->trEventPrms.intrMask);
                    break;
                }
                TaskP_yield();
            }
#endif

            /* Do application specific processing here!! */

            /* Set channel 1 trigger and wait for completion */
            appChObj = &appObj->appChObj[1U];
            CSL_REG32_WR(appChObj->pSwTriggerReg, appChObj->triggerMask);
#if defined (UDMA_TEST_INTR)
            SemaphoreP_pend(appChObj->transferDoneSem, SemaphoreP_WAIT_FOREVER);
#else
            while(1U)
            {
                volatile uint64_t   intrStatusReg;
                intrStatusReg = CSL_REG64_RD(appChObj->trEventPrms.intrStatusReg);
                if(intrStatusReg & appChObj->trEventPrms.intrMask)
                {
                    /* Clear interrupt */
                    CSL_REG64_WR(appChObj->trEventPrms.intrClearReg, appChObj->trEventPrms.intrMask);
                    break;
                }
                TaskP_yield();
            }
#endif
        }
    }

    if(UDMA_SOK == retVal)
    {
        for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
        {
            appChObj = &appObj->appChObj[chIdx];
            chHandle = appChObj->chHandle;
            trpdMem  = appChObj->trpdMem;

            /* wait for response to be received in completion queue */
            while(1)
            {
                retVal =
                    Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);
                if(UDMA_SOK == retVal)
                {
                    TaskP_yield();
                    break;
                }
            }

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

            if(UDMA_SOK == retVal)
            {
                /* Invalidate cache */
                Udma_appUtilsCacheInv(trpdMem, UDMA_TEST_APP_TRPD_SIZE_ALIGN);

                /* check TR response status */
                pTrResp = (uint32_t *) (trpdMem + (sizeof(CSL_UdmapTR15) * 2U));
                trRespStatus = CSL_FEXT(*pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
                if(trRespStatus != CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE)
                {
                    App_print("[Error] TR Response not completed!!\n");
                    retVal = UDMA_EFAIL;
                }
            }

            if(UDMA_SOK != retVal)
            {
                break;
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Compare data */
        /* Invalidate destination buffer */
        srcBuf  = &gUdmaTestSrcBuf[0U];
        destBuf  = &gUdmaTestDestBuf[0U];
        Udma_appUtilsCacheInv(destBuf, UDMA_TEST_APP_NUM_BYTES);
        for(i = 0U; i < UDMA_TEST_APP_NUM_BYTES; i++)
        {
            if(srcBuf[i] != destBuf[i])
            {
                App_print("[Error] Data mismatch!!\n");
                retVal = UDMA_EFAIL;
            }
        }
    }

    return (retVal);
}

#if defined (UDMA_TEST_INTR)
static void App_udmaEventDmaCb(Udma_EventHandle eventHandle,
                               uint32_t eventType,
                               void *appData)
{
    App_UdmaChObj *appChObj = (App_UdmaChObj *) appData;

    if(appChObj != NULL)
    {
        if(UDMA_EVENT_TYPE_TR == eventType)
        {
            SemaphoreP_post(appChObj->transferDoneSem);
        }
        else
        {
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

#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static void App_udmaEventTdCb(Udma_EventHandle eventHandle,
                              uint32_t eventType,
                              void *appData)
{
    int32_t             retVal;
    CSL_UdmapTdResponse tdResp;
    App_UdmaChObj      *appChObj = (App_UdmaChObj *) appData;

    if(appChObj != NULL)
    {
        if(UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
        {
            /* Response received in Teardown completion queue */
            retVal = Udma_chDequeueTdResponse(appChObj->chHandle, &tdResp);
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
    }
    else
    {
        gUdmaAppResult = UDMA_EFAIL;
    }

    return;
}
#endif

static int32_t App_init(App_UdmaObj *appObj)
{
    int32_t         retVal;
    Udma_InitPrms   initPrms;
    uint32_t        instId;
    int32_t         chIdx;
    App_UdmaChObj  *appChObj;
    Udma_DrvHandle  drvHandle = &appObj->drvObj;
    uint32_t        i;
    uint8_t        *srcBuf;

#if defined (SOC_AM64X)
    /* Use Block Copy DMA for AM64x */
    instId = UDMA_INST_ID_BCDMA_0;
#else
    /* Use MCU NAVSS for MCU domain cores. Rest all cores uses Main NAVSS */
#if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
    instId = UDMA_INST_ID_MCU_0;
#else
    instId = UDMA_INST_ID_MAIN_0;
#endif
#endif
    /* UDMA driver init */
    UdmaInitPrms_init(instId, &initPrms);
    initPrms.virtToPhyFxn   = &Udma_appVirtToPhyFxn;
    initPrms.phyToVirtFxn   = &Udma_appPhyToVirtFxn;
    initPrms.printFxn       = &App_print;
    retVal = Udma_init(drvHandle, &initPrms);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA init failed!!\n");
    }

    /* Init channel parameters */
    for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
    {
        appChObj                    = &appObj->appChObj[chIdx];
        appChObj->chIdx             = chIdx;
        appChObj->chHandle          = &appChObj->chObj;
        appChObj->drvHandle         = drvHandle;
        appChObj->transferDoneSem   = NULL;
        appChObj->trEventHandle     = NULL;
        appChObj->trigger           = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL0;
        appChObj->eventSize         = CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT2_DEC;
        appChObj->triggerType       = CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC;
        appChObj->eolType           = CSL_UDMAP_TR_FLAGS_EOL_ICNT0_ICNT1;
        appChObj->txRingMem         = &gTxRingMem[chIdx][0U];
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
        appChObj->tdCqEventHandle   = NULL;
        appChObj->txCompRingMem     = &gTxCompRingMem[chIdx][0U];
        appChObj->txTdCompRingMem   = &gTxTdCompRingMem[chIdx][0U];
#endif
        appChObj->trpdMem           = &gUdmaTrpdMem[chIdx][0U];
        if(0U == chIdx)
        {
            appChObj->srcBuf        = &gUdmaTestSrcBuf[0U];
            appChObj->destBuf       = &gUdmaTestIramBuf[0U];
            /* Init buffers */
            srcBuf = appChObj->srcBuf;
            for(i = 0U; i < UDMA_TEST_APP_NUM_BYTES; i++)
            {
                srcBuf[i] = i;
            }
            /* Writeback source destination buffer */
            Udma_appUtilsCacheWb(appChObj->srcBuf, UDMA_TEST_APP_NUM_BYTES);
        }
        else
        {
            appChObj->srcBuf        = &gUdmaTestIramBuf[0U];
            appChObj->destBuf       = &gUdmaTestDestBuf[0U];
        }
    }

    return (retVal);
}

static int32_t App_deinit(App_UdmaObj *appObj)
{
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &appObj->drvObj;

    retVal = Udma_deinit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("[Error] UDMA deinit failed!!\n");
    }

    return (retVal);
}
static int32_t App_create(App_UdmaObj *appObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_EventHandle    eventHandle;
    SemaphoreP_Params   semPrms;
    int32_t             chIdx;
    App_UdmaChObj      *appChObj;
    Udma_ChHandle       chHandle;
    Udma_DrvHandle      drvHandle = &appObj->drvObj;

    for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
    {
        appChObj = &appObj->appChObj[chIdx];
        chHandle = appChObj->chHandle;

        SemaphoreP_Params_init(&semPrms);
        appChObj->transferDoneSem = SemaphoreP_create(0, &semPrms);
        if(NULL == appChObj->transferDoneSem)
        {
            App_print("[Error] Sem create failed!!\n");
            retVal = UDMA_EFAIL;
        }

        if(UDMA_SOK == retVal)
        {
            /* Init channel parameters */
            chType = UDMA_CH_TYPE_TR_BLK_COPY;
            UdmaChPrms_init(&chPrms, chType);
            chPrms.fqRingPrms.ringMem       = appChObj->txRingMem;
            chPrms.fqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
            chPrms.fqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
            chPrms.cqRingPrms.ringMem       = appChObj->txCompRingMem;
            chPrms.cqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
            chPrms.cqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
            chPrms.tdCqRingPrms.ringMem     = appChObj->txTdCompRingMem;
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

        if(UDMA_SOK == retVal)
        {
            /* Register TR event */
            eventHandle = &appChObj->trEventObj;
            UdmaEventPrms_init(&appChObj->trEventPrms);
            appChObj->trEventPrms.eventType         = UDMA_EVENT_TYPE_TR;
            appChObj->trEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            appChObj->trEventPrms.chHandle          = chHandle;
#if defined (UDMA_TEST_INTR)
            appChObj->trEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
            appChObj->trEventPrms.eventCb           = &App_udmaEventDmaCb;
#else
            /* For polling mode we can't use the existing master event as that is meant only for interrupt event - 
             *  we can't mix interrupt and poll mode in same master handle. Set the parameter to NULL 
             *  so that the driver creates a new master event. */
            appChObj->trEventPrms.masterEventHandle = NULL;
            appChObj->trEventPrms.eventCb           = NULL;
#endif
            appChObj->trEventPrms.appData           = appChObj;
            retVal = Udma_eventRegister(drvHandle, eventHandle, &appChObj->trEventPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA TR event register failed!!\n");
            }
            else
            {
                appChObj->trEventHandle = eventHandle;
            }
        }

#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
        if(UDMA_SOK == retVal)
        {
            /* Register teardown ring completion callback */
            eventHandle = &appChObj->tdCqEventObj;
            UdmaEventPrms_init(&appChObj->tdCqEventPrms);
            appChObj->tdCqEventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
            appChObj->tdCqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
            appChObj->tdCqEventPrms.chHandle          = chHandle;
            appChObj->tdCqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
            appChObj->tdCqEventPrms.eventCb           = &App_udmaEventTdCb;
            appChObj->tdCqEventPrms.appData           = appChObj;
            retVal = Udma_eventRegister(drvHandle, eventHandle, &appChObj->tdCqEventPrms);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA Teardown CQ event register failed!!\n");
            }
            else
            {
                appChObj->tdCqEventHandle = eventHandle;
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

        if(UDMA_SOK != retVal)
        {
            break;
        }
    }

    return (retVal);
}

static int32_t App_delete(App_UdmaObj *appObj)
{
    int32_t         retVal, tempRetVal;
    uint64_t        pDesc;
    int32_t         chIdx;
    App_UdmaChObj  *appChObj;
    Udma_ChHandle   chHandle;

    for(chIdx = 0U; chIdx < UDMA_TEST_APP_NUM_CH; chIdx++)
    {
        appChObj = &appObj->appChObj[chIdx];
        chHandle = appChObj->chHandle;

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
    }

    for(chIdx = UDMA_TEST_APP_NUM_CH - 1U; chIdx >=0 ; chIdx--)
    {
        appChObj = &appObj->appChObj[chIdx];
        chHandle = appChObj->chHandle;

        /* Unregister all events */
        if(NULL != appChObj->trEventHandle)
        {
            retVal += Udma_eventUnRegister(appChObj->trEventHandle);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA event unregister failed!!\n");
            }
            appChObj->trEventHandle = NULL;
        }
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
        if(NULL != appChObj->tdCqEventHandle)
        {
            retVal += Udma_eventUnRegister(appChObj->tdCqEventHandle);
            if(UDMA_SOK != retVal)
            {
                App_print("[Error] UDMA event unregister failed!!\n");
            }
            appChObj->tdCqEventHandle = NULL;
        }
#endif

        retVal += Udma_chClose(chHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA channel close failed!!\n");
        }

        if(appChObj->transferDoneSem != NULL)
        {
            SemaphoreP_delete(appChObj->transferDoneSem);
            appChObj->transferDoneSem = NULL;
        }
    }

    return (retVal);
}

static void App_udmaTrpdInit(App_UdmaChObj *appChObj)
{
    CSL_UdmapCppi5TRPD *pTrpd = (CSL_UdmapCppi5TRPD *) appChObj->trpdMem;
    CSL_UdmapTR15 *pTr = (CSL_UdmapTR15 *)(appChObj->trpdMem + sizeof(CSL_UdmapTR15));
    uint32_t *pTrResp = (uint32_t *) (appChObj->trpdMem + (sizeof(CSL_UdmapTR15) * 2U));
    uint32_t cqRingNum = Udma_chGetCqRingNum(appChObj->chHandle);

    /* Make TRPD */
    UdmaUtils_makeTrpd(pTrpd, UDMA_TR_TYPE_15, 1U, cqRingNum);

    /* Setup TR */
    pTr->flags  = CSL_FMK(UDMAP_TR_FLAGS_TYPE, CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING_INDIRECTION);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_STATIC, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EOL, appChObj->eolType);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE, appChObj->eventSize);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0, appChObj->trigger);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1, CSL_UDMAP_TR_FLAGS_TRIGGER_NONE);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE, appChObj->triggerType);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE, CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, 0x25U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EOP, 1U);

    pTr->icnt0    = UDMA_TEST_1D_SIZE;
    pTr->icnt1    = UDMA_TEST_2D_SIZE;
    pTr->icnt2    = UDMA_TEST_3D_SIZE;
    pTr->icnt3    = 1U;
    pTr->addr     = (uint64_t) Udma_appVirtToPhyFxn(appChObj->srcBuf, 0U, NULL);
    pTr->fmtflags = 0x00000000U;        /* Linear addressing, 1 byte per elem.
                                           Replace with CSL-FL API */
    pTr->dicnt0   = UDMA_TEST_1D_SIZE;
    pTr->dicnt1   = UDMA_TEST_2D_SIZE;
    pTr->dicnt2   = UDMA_TEST_3D_SIZE;
    pTr->dicnt3   = 1U;
    pTr->daddr    = (uint64_t) Udma_appVirtToPhyFxn(appChObj->destBuf, 0U, NULL);

    if(0U == appChObj->chIdx)
    {
        pTr->dim1     = pTr->icnt0;
        pTr->dim2     = (pTr->icnt0 * pTr->icnt1);
        pTr->dim3     = (pTr->icnt0 * pTr->icnt1 * pTr->icnt2);
        pTr->ddim1    = pTr->dicnt0;
        pTr->ddim2    = 0U;
        pTr->ddim3    = 0U;
    }
    else
    {
        pTr->dim1     = pTr->icnt0;
        pTr->dim2     = 0U;
        pTr->dim3     = 0U;
        pTr->ddim1    = pTr->dicnt0;
        pTr->ddim2    = (pTr->dicnt0 * pTr->dicnt1);
        pTr->ddim3    = (pTr->dicnt0 * pTr->dicnt1 * pTr->dicnt2);
    }

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;

    /* Writeback cache */
    Udma_appUtilsCacheWb(appChObj->trpdMem, UDMA_TEST_APP_TRPD_SIZE_ALIGN);

    return;
}

static void App_print(const char *str)
{
    UART_printf("%s", str);
    if(UTRUE == Udma_appIsPrintSupported())
    {
        printf("%s", str);
    }

    return;
}
