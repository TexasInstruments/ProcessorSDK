/*
 *  Copyright (c) Texas Instruments Incorporated 2026
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
 *  \file udma_dru_test.c
 *
 *  \brief DRU Readback Example initializes the UDMA driver and DRU queues,
 *         and channel, then performs periodic readback and comparison
 *         of DRU registers.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/examples/udma_apputils/udma_apputils.h>

#include <ip_fma_dru.h>
#include <string.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
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

/**< Number of periodic readbacks */
#define PERIODIC_CHECK_NUM                        ((uint8_t)10U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief Performs a register check for the DRU registers.
 *
 * This function reads the expected values from the DRU Queue and CHNRT registers,
 * periodically reads the actual register values, and compares them with the expected values.
 * It prints the comparison results to the console.
 *
 * \param   None.
 *
 * \retval  status          REGCHECK_OK on successful
 *                          else otherwise..
 */
static int32_t DruApp_RegisterCheck(void);

/**
 *  \brief UDMA event callback function used to handle channel events.
 *
 *  \param   eventHandle     Handle of the UDMA event that triggered the callback.
 *  \param   eventType       Type of the event received from the UDMA driver.
 *  \param   appData         User-defined data pointer passed during callback registration.
 *
 *  \retval     None.
 */
static void DruApp_UdmaEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData);
/**
 * \brief Initializes the UDMA driver and configures all DRU queues.
 *
 * This function initializes the UDMA driver for the MAIN NAVSS instance and
 * applies queue configuration parameters (priority, orderId, etc.) to all
 * DRU queues of the selected UTC module. It prints error messages in case of
 * initialization or configuration failures.
 *
 * \param   drvHandle       Pointer to the UDMA driver handle.
 *
 * \retval  status          UDMA_SOK on successful
 *                          else otherwise..
 */
static int32_t DruApp_Init(Udma_DrvHandle drvHandle);

/**
 * \brief   De-initializes the UDMA driver.
 *
 * This function de-initializes the UDMA driver associated with the provided
 * driver handle. It prints an error message if the de-initialization fails.
 *
 * \param   drvHandle       Pointer to the UDMA driver handle.
 *
 * \retval  status          UDMA_SOK on successful
 *                          else otherwise..
 */
static int32_t DruApp_Deinit(Udma_DrvHandle drvHandle);

/**
 * \brief   Creates and initializes a UDMA UTC channel, including ring setup and
 *          event registration.
 *
 * This function configures and opens a UDMA UTC (Unified Transfer Controller)
 * channel, initializes ring memories for FQ, CQ and teardown CQ rings, sets
 * UTC-specific parameters (DRU owner and queue), registers completion and
 * teardown events, and finally enables the channel.
 *
 * Errors during any stage are reported through DruApp_Print().
 *
 * \param   drvHandle       Handle to an initialized UDMA driver instance.
 * \param   chHandle        Handle to a UDMA channel object that will be opened
 *                          and configured by this function.
 *
 * \retval  status          UDMA_SOK on successful
 *                          else otherwise..
 */
static int32_t DruApp_Create(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);

/**
 * \brief   Deletes a UDMA channel by disabling it, flushing pending requests,
 *          unregistering events, and closing the channel.
 *
 * This function first disables the specified UDMA channel, then flushes all
 * remaining descriptors from the free queue ring. After that, it unregisters
 * all associated UDMA events (completion and teardown events) and finally
 * closes the channel.
 *
 * Any errors during the procedure are reported via DruApp_Print().
 * The function accumulates error codes from multiple cleanup steps.
 *
 * \param   drvHandle       Handle to the UDMA driver instance (unused but kept
 *                          for interface consistency).
 * \param   chHandle        Handle to the UDMA channel to be disabled and closed.
 *
 * \retval  status          UDMA_SOK on successful
 *                          else otherwise..
 */
static int32_t DruApp_Delete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle);

/**
 * \brief   Prints a string using UART and optionally standard output.
 *
 * This function prints the given string through UART using UART_printf().
 * If printing to standard output is supported (as determined by
 * Udma_appIsPrintSupported()), the same string is also printed using printf().
 *
 * \param   str     Pointer to a null-terminated string to be printed.
 *
 * \return  None.
 */
static void DruApp_Print(const char *str);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver objects
 */
struct Udma_DrvObj      gUdmaDrvObj;
struct Udma_ChObj       gUdmaChObj;
struct Udma_EventObj    gUdmaCqEventObj;
struct Udma_EventObj    gUdmaTdCqEventObj;

/*
 * UDMA Memories
 */
static uint8_t gDruRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gDruCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gDruTdCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Application main
 */
int32_t DruApp_ReadbackTest(void)
{
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &gUdmaDrvObj;
    Udma_ChHandle   chHandle = &gUdmaChObj;

    DruApp_Print("UDMA DRU application started...\n");

    /* Initializes the UDMA driver and configures all DRU queues */
    retVal = DruApp_Init(drvHandle);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA App init failed!!\n");
    }

    if (UDMA_SOK == retVal)
    {
        /* Creates and initializes a UDMA UTC channel, including ring setup and
           event registration */
        retVal = DruApp_Create(drvHandle, chHandle);
        if (UDMA_SOK != retVal)
        {
            DruApp_Print("[Error] UDMA App create failed!!\n");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Performs a register check for the DRU registers */
        retVal = DruApp_RegisterCheck();
        if (IPFMA_OK != retVal)
        {
            DruApp_Print("[Error] Dru register check failed!!\n");
        }
    }

    /* Deletes a UDMA channel by disabling it, flushing pending requests,
       unregistering events, and closing the channel */
    retVal = DruApp_Delete(drvHandle, chHandle);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA App delete failed!!\n");
    }

    /* De-initializes the UDMA driver */
    retVal = DruApp_Deinit(drvHandle);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA App deinit failed!!\n");
    }

    return (0);
}

static int32_t DruApp_RegisterCheck(void)
{
    DruApp_Print("Register check starts ..\r\n");

    IpFma_Status status = IPFMA_OK;

    // Get expected values from dru_queue registers
    IpFma_DruQueueRegs druQueueRegsExpValues;
    status = IpFma_Dru_GetQueueRegs(CSL_COMPUTE_CLUSTER0_MMR_DRU0_MMR_CFG_DRU_QUEUE_BASE, &druQueueRegsExpValues);
    DruApp_Print("Comparing expected-actual dru queue register values...\r\n");

    if (IPFMA_OK == status)
    {
        int8_t i = PERIODIC_CHECK_NUM;
        while (i > 0)
        {
            // Read the actual values from registers
            IpFma_DruQueueRegs druQueueRegsActualValues;
            status = IpFma_Dru_GetQueueRegs(CSL_COMPUTE_CLUSTER0_MMR_DRU0_MMR_CFG_DRU_QUEUE_BASE, &druQueueRegsActualValues);

            if (IPFMA_OK == status)
            {
                // Compare expected and actual values periodically
                status = IpFma_Dru_CompareQueueRegs(&druQueueRegsExpValues, &druQueueRegsActualValues);
                if (IPFMA_OK == status)
                {
                    DruApp_Print("Expected - Actual dru queue register values match!\n");
                }
                else
                {
                    DruApp_Print("Expected - Actual dru queue values mismatch!\n");
                }
                i--;
            }
        }
    }

    // Get expected values from dru_chnrt registers
    IpFma_DruChnrtRegs chnrtRegsExpValues;
    status = IpFma_Dru_GetChnrtRegs(CSL_COMPUTE_CLUSTER0_MMR_DRU0_MMR_CFG_DRU_CHNRT_BASE, &chnrtRegsExpValues);
    //memset(&chnrtRegsExpValues, 0, sizeof(chnrtRegsExpValues));
    DruApp_Print("Comparing expected-actual dru chnrt register values...\r\n");

    if (IPFMA_OK == status)
    {
        int8_t i = PERIODIC_CHECK_NUM;
        while (i > 0)
        {
            // Read the actual values from registers
            IpFma_DruChnrtRegs chnrtRegsActualValues;
            status = IpFma_Dru_GetChnrtRegs(CSL_COMPUTE_CLUSTER0_MMR_DRU0_MMR_CFG_DRU_CHNRT_BASE, &chnrtRegsActualValues);

            if (IPFMA_OK == status)
            {
                // Compare expected and actual values periodically
                status = IpFma_Dru_CompareChnrtRegs(&chnrtRegsExpValues, &chnrtRegsActualValues);
                if (IPFMA_OK == status)
                {
                    DruApp_Print("Expected - Actual dru chnrt register values match!\n");
                }
                else
                {
                    DruApp_Print("Expected - Actual dru chnrt values mismatch!\n");
                }
                i--;
            }
        }
    }

    if (IPFMA_OK == status)
    {
        DruApp_Print("Register check done ...\r\n");
        DruApp_Print("All tests have passed!!\n");
    }
    else
    {
        DruApp_Print("Register check fail ...\r\n");
        DruApp_Print("Some tests have failed!!\n");
    }

    return status;
}

static void DruApp_UdmaEventCb(Udma_EventHandle eventHandle,
                            uint32_t eventType,
                            void *appData)
{
    int32_t         retVal;
    CSL_UdmapTdResponse tdResp;

    if (UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
    {
        /* Response received in Teardown completion queue */
        retVal = Udma_chDequeueTdResponse(&gUdmaChObj, &tdResp);
        if (UDMA_SOK != retVal)
        {
            /* [Error] No TD response after callback!! */
        }
    }

    return;
}

static int32_t DruApp_Init(Udma_DrvHandle drvHandle)
{
    int32_t             retVal;
    Udma_InitPrms       initPrms;
    uint32_t            instId;
    uint32_t            utcId;
    uint32_t            numQueue, queId;
    CSL_DruQueueConfig  queueCfg;

    /* Note: There is no external channel support in MCU NAVSS. So always use
     * main NAVSS even for MCU builds */
    /* UDMA driver init */
    instId = UDMA_INST_ID_MAIN_0;
    UdmaInitPrms_init(instId, &initPrms);
    initPrms.printFxn = &DruApp_Print;
    retVal = Udma_init(drvHandle, &initPrms);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA init failed!!\n");
    }

    /* Init all DRU queue */
    utcId = UDMA_UTC_ID_MSMC_DRU0;
    numQueue = Udma_druGetNumQueue(drvHandle, utcId);
    if (0U == numQueue)
    {
        DruApp_Print("[Error] Invalid queue number!!\n");
    }
    UdmaDruQueueConfig_init(&queueCfg);

    /* Config DRU queue */
    for (queId = CSL_DRU_QUEUE_ID_0; queId < numQueue; queId++)
    {
        retVal = Udma_druQueueConfig(drvHandle, utcId, queId, &queueCfg);
        if (UDMA_SOK != retVal)
        {
            DruApp_Print("[Error] DRU queue config failed!!\n");
            break;
        }
    }
    return (retVal);
}

static int32_t DruApp_Deinit(Udma_DrvHandle drvHandle)
{
    int32_t             retVal;

    retVal = Udma_deinit(drvHandle);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA deinit failed!!\n");
    }

    return (retVal);
}

static int32_t DruApp_Create(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChUtcPrms      utcPrms;
    Udma_EventHandle    eventHandle;
    Udma_EventPrms      eventPrms;

    if (UDMA_SOK == retVal)
    {
        /* Init channel parameters */
        chType = UDMA_CH_TYPE_UTC;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.utcId                = UDMA_UTC_ID_MSMC_DRU0;
        chPrms.fqRingPrms.ringMem   = &gDruRingMem[0U];
        chPrms.cqRingPrms.ringMem   = &gDruCompRingMem[0U];
        chPrms.tdCqRingPrms.ringMem = &gDruTdCompRingMem[0U];
        chPrms.fqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.cqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.tdCqRingPrms.ringMemSize = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.fqRingPrms.elemCnt   = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.cqRingPrms.elemCnt   = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.tdCqRingPrms.elemCnt = UDMA_TEST_APP_RING_ENTRIES;

        /* Open channel for DRU */
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if (UDMA_SOK != retVal)
        {
            DruApp_Print("[Error] UDMA channel open failed!!\n");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Config UTC channel */
        UdmaChUtcPrms_init(&utcPrms);
        utcPrms.druOwner    = CSL_DRU_OWNER_UDMAC_TR;
        utcPrms.druQueueId  = CSL_DRU_QUEUE_ID_3;
        retVal = Udma_chConfigUtc(chHandle, &utcPrms);
        if (UDMA_SOK != retVal)
        {
            DruApp_Print("[Error] UDMA UTC channel config failed!!\n");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Register ring completion callback */
        eventHandle = &gUdmaCqEventObj;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_DMA_COMPLETION;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.chHandle          = chHandle;
        eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.eventCb           = &DruApp_UdmaEventCb;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if (UDMA_SOK != retVal)
        {
            DruApp_Print("[Error] UDMA CQ event register failed!!\n");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Register teardown ring completion callback */
        eventHandle = &gUdmaTdCqEventObj;
        UdmaEventPrms_init(&eventPrms);
        eventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
        eventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        eventPrms.chHandle          = chHandle;
        eventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        eventPrms.eventCb           = &DruApp_UdmaEventCb;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &eventPrms);
        if (UDMA_SOK != retVal)
        {
            DruApp_Print("[Error] UDMA Teardown CQ event register failed!!\n");
        }
    }

    if (UDMA_SOK == retVal)
    {
        /* Channel enable */
        retVal = Udma_chEnable(chHandle);
        if (UDMA_SOK != retVal)
        {
            DruApp_Print("[Error] UDMA channel enable failed!!\n");
        }
    }

    return (retVal);
}

static int32_t DruApp_Delete(Udma_DrvHandle drvHandle, Udma_ChHandle chHandle)
{
    int32_t             retVal, tempRetVal;
    uint64_t            pDesc;
    Udma_EventHandle    eventHandle;

    retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA channel disable failed!!\n");
    }

    /* Flush any pending request from the free queue */
    while (1)
    {
        tempRetVal = Udma_ringFlushRaw(
                         Udma_chGetFqRingHandle(chHandle), &pDesc);
        if (UDMA_ETIMEOUT == tempRetVal)
        {
            break;
        }
    }

    /* Unregister all events */
    eventHandle = &gUdmaTdCqEventObj;
    retVal += Udma_eventUnRegister(eventHandle);
    eventHandle = &gUdmaCqEventObj;
    retVal += Udma_eventUnRegister(eventHandle);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA event unregister failed!!\n");
    }

    retVal += Udma_chClose(chHandle);
    if (UDMA_SOK != retVal)
    {
        DruApp_Print("[Error] UDMA channel close failed!!\n");
    }

    return (retVal);
}

static void DruApp_Print(const char *str)
{
    UART_printf("%s", str);

    if (UTRUE == Udma_appIsPrintSupported())
    {
        printf("%s", str);
    }

    return;
}
