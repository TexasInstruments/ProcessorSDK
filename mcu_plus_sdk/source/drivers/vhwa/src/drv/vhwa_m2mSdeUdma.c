/**
 *   Copyright (c) Texas Instruments Incorporated 2019
 *   All rights reserved.
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
 *
 */

/**
 *  \file vhwa_m2mSdeUdma.c
 *
 *  \brief Utility APIs for UDMA configuration
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "src/drv/vhwa_m2mSdePriv.h"


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* TODO - Cross check with Smoke test */


#define VHWA_SDE_WAIT_CYCLES                              (500)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Vhwa_sdeSetupPacketInfo(const Vhwa_M2mSdeChParams *chPrms,
                                    uint32_t complRingNum,
                                    uint32_t chType);
static void Vhwa_sdeSetupInputTR(Vhwa_M2mSdeHandleObj *hObj,
                                 CSL_UdmapTR9 *pTr, uint32_t chType);
static void Vhwa_sdeSetupOutputTR(const Vhwa_M2mSdeHandleObj *hObj,
                                  const Vhwa_M2mSdeChParams *chPrms,
                                  CSL_UdmapTR9 *pTr);
static void Vhwa_sdeSetOutTRAddr(const Vhwa_M2mSdeChParams *chPrms,
                                 CSL_UdmapTR9 *pTr, uint64_t outAddr);


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/**
 * \brief TRPD memory for each handle and for each channel in handle
 */
/**
 * \brief Ring memory, one ring for each output channel.
 */
static uint8_t gSdeRingMem[VHWA_M2M_SDE_MAX_DMA_CH]
    [VHWA_M2M_SDE_UDMA_RING_MEM_SIZE]
    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#if !defined(MCU_PLUS_SDK)
/**
 * \brief Completion Ring memory, one ring for each output channel.
 */
static uint8_t gSdeCompRingMem[VHWA_M2M_SDE_MAX_DMA_CH]
    [VHWA_M2M_SDE_UDMA_RING_MEM_SIZE]
    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/**
 * \brief Completion Ring memory, one ring for each output channel.
 */
static uint8_t gSdeTdCompRingMem[VHWA_M2M_SDE_MAX_DMA_CH]
    [VHWA_M2M_SDE_UDMA_RING_MEM_SIZE]
    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
#endif
/**
 * \brief TR memroy for input channel
 */
static uint8_t gSdeTxTprdMem[VHWA_M2M_SDE_MAX_HANDLES]
    [VHWA_M2M_SDE_TX_DMA_CH][VHWA_M2M_SDE_UDMA_TX_TRPD_SIZE]
    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/**
 * \brief TR memroy for input channel
 */
static uint8_t gSdeRxTprdMem[VHWA_M2M_SDE_MAX_HANDLES]
    [VHWA_M2M_SDE_RX_DMA_CH][VHWA_M2M_SDE_UDMA_RX_TRPD_SIZE]
    __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Vhwa_m2mSdeUdmaInit(Vhwa_M2mSdeInstObj *instObj,
    Vhwa_M2mSdeInitParams *initPrms)
{
    int32_t                 status = UDMA_SOK;
    uint32_t                cnt;
    uint32_t                utcChId;
    uint32_t                chType;
    Udma_ChHandle           chHndl;
    Udma_ChPrms             chPrms;
    Udma_ChUtcPrms          utcPrms;

    /* Check for Null pointer,
       Internal function so checking with assert */
    GT_assert(VhwaSdeTrace, (NULL != instObj));
    GT_assert(VhwaSdeTrace, (NULL != initPrms));

    utcChId = VHWA_SDE_UTC_CH_START;

    for (cnt = 0U; cnt < VHWA_M2M_SDE_MAX_DMA_CH; cnt ++)
    {
        chHndl = &instObj->utcChObj[cnt];
        instObj->utcChHndl[cnt] = chHndl;

        /* Initialize channel parameters */
        chType = UDMA_CH_TYPE_UTC;
        UdmaChPrms_init(&chPrms, chType);

        chPrms.chNum                = utcChId;
        chPrms.utcId                = VHWA_M2M_SDE_UTC_ID;
        chPrms.fqRingPrms.ringMem   = &gSdeRingMem[cnt][0U];
        chPrms.fqRingPrms.elemCnt   = VHWA_M2M_SDE_UDMA_RING_ENTRIES;
        #if defined(MCU_PLUS_SDK)
            chPrms.cqRingPrms.ringMem   = NULL;
            chPrms.tdCqRingPrms.ringMem = NULL;
        #else

            chPrms.cqRingPrms.ringMem   = &gSdeCompRingMem[cnt][0];
            chPrms.tdCqRingPrms.ringMem = &gSdeTdCompRingMem[cnt][0U];
            chPrms.cqRingPrms.elemCnt   = VHWA_M2M_SDE_UDMA_RING_ENTRIES;
            chPrms.tdCqRingPrms.elemCnt = VHWA_M2M_SDE_UDMA_RING_ENTRIES;
        #endif
        /* Open channel,
           since chNum is set to ANY in chPrms, it also allocates
           an external channel and completion channel */
        status = Udma_chOpen(initPrms->udmaDrvHndl, chHndl,
            chType, &chPrms);
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */ 
        if(UDMA_SOK != status)
        {
            GT_0trace(VhwaSdeTrace, GT_ERR, "UDMA channel open failed!!\n");
        }
        /* LDRA_JUSTIFY_END */
        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if(UDMA_SOK == status)
        /* LDRA_JUSTIFY_END */
        {
            /* Intialize UTC channel Paramenters */
            UdmaChUtcPrms_init(&utcPrms);


            #if defined (MCU_PLUS_SDK)
				utcPrms.chanType = (uint8_t)CSL_BCDMA_CHAN_TYPE_REF_TR_RING;
                utcPrms.druOwner = (uint8_t)CSL_BCDMA_CHAN_TYPE_REF_TR_RING;
            #else
				utcPrms.chanType = (uint8_t)CSL_UDMAP_CHAN_TYPE_REF_TR_RING;
                utcPrms.busOrderId = 3;
                utcPrms.busPriority = 4;
            #endif

            /* Configure UTC channel,
               used to configure other channel specific parameters like
               priority, orderid, queue id etc */
            status = Udma_chConfigUtc(chHndl, &utcPrms);
            /* LDRA_JUSTIFY_START
            <metric start> statement branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            This failure case is out of scope for the imaging test framework.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if(UDMA_SOK != status)
            {
                GT_0trace(VhwaSdeTrace, GT_ERR,
                            "UDMA UTC channel config failed!!\n");
            }
            /* LDRA_JUSTIFY_END */
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if(UDMA_SOK != status)
        {
            break;
        }
        /* LDRA_JUSTIFY_END */
        /* Locally store completion queue number, forward ring handle
           and completion ring handle */
        instObj->complRingNum[cnt] = Udma_chGetCqRingNum(chHndl);
        instObj->fqRingHndl[cnt] = Udma_chGetFqRingHandle(chHndl);
        instObj->cqRingHndl[cnt] = Udma_chGetCqRingHandle(chHndl);
        instObj->utcCh[cnt] = Udma_chGetNum(chHndl);

        utcChId++;
    }

    /* Convert UDMA status to FVID2 status,
       so that the caller of this function always uses FVID2 status only */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(UDMA_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;
    }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    The break statement in a previous makes control exit the loop before it reaches here.
    Status setting should be with break statemen. This statement is redundant. This can be added in a future release.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    else
    {
        status = FVID2_EFAIL;
    }
    /* LDRA_JUSTIFY_END */
    return (status);
}

int32_t Vhwa_m2mSdeUdmaDeInit(const Vhwa_M2mSdeInstObj *instObj)
{
    int32_t             status = FVID2_SOK;
    uint32_t            cnt;
    Udma_ChHandle       chHndl;

    /* Check for Null pointer */
    GT_assert(VhwaSdeTrace, (NULL != instObj));

    for (cnt = 0U; cnt < VHWA_M2M_SDE_MAX_DMA_CH; cnt ++)
    {
        chHndl = instObj->utcChHndl[cnt];

        /* Close UDMA Channel */
        status = Udma_chClose(chHndl);
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        A break statement can be added to exit the loop to avoid status overwriting in a future release.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if(UDMA_SOK != status)
        {
            GT_0trace(VhwaSdeTrace, GT_ERR,
                        "[Error] UDMA channel close failed!!\n");
        }
        /* LDRA_JUSTIFY_END */
    }

    /* Convert UDMA status to FVID2 status,
       so that the caller of this function always uses FVID2 status only */
    /* LDRA_JUSTIFY_START_DISABLED
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(UDMA_SOK == status)
    {
        status = FVID2_SOK;
    }
    /* LDRA_JUSTIFY_END_DISABLED */
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    else
    {
        status = FVID2_EFAIL;
    }
    /* LDRA_JUSTIFY_END */
    return (status);
}

int32_t Vhwa_m2mSdeStartCh(const Vhwa_M2mSdeInstObj *instObj)
{
    int32_t         status = FVID2_EBADARGS;
    uint32_t        cnt;
    Udma_ChHandle   chHndl;

    /* Check for Null pointer */
    GT_assert(VhwaSdeTrace, (NULL != instObj));

    /* By default, all channels are enabled */
    for (cnt = 0u; cnt < VHWA_M2M_SDE_MAX_DMA_CH; cnt ++)
    {
        chHndl = instObj->utcChHndl[cnt];

        /* UDMA Channel enable */
        status = Udma_chEnable(chHndl);
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if (UDMA_SOK != status)
        {
            GT_0trace(VhwaSdeTrace, GT_ERR,
                "UDMA channel enable failed!!\n");
            status = FVID2_EFAIL;
            break;
        }
        /* LDRA_JUSTIFY_END */
    }

    /* Convert UDMA status to FVID2 status,
       so that the caller of this function always uses FVID2 status only */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(UDMA_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;
    }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    The break statement in a previous makes control exit the loop before it reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested. This is redundant and can be removed in a future release.
    <justification end> */
    else
    {
        status = FVID2_EFAIL;
    }
    /* LDRA_JUSTIFY_END */
    return (status);
}

int32_t Vhwa_m2mSdeStopCh(const Vhwa_M2mSdeInstObj *instObj)
{
    int32_t         status = FVID2_EBADARGS;
    uint32_t        cnt;
    Udma_ChHandle   chHndl;

    /* Check for Null pointer */
    GT_assert(VhwaSdeTrace, (NULL != instObj));

    /* By default, all channels are disabled, on the last handle close */
    for (cnt = 0u; cnt < VHWA_M2M_SDE_MAX_DMA_CH; cnt ++)
    {
        chHndl = instObj->utcChHndl[cnt];

        /* UDMA Channel disable */
        status = Udma_chDisable(chHndl, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if(UDMA_SOK != status)
        {
            GT_0trace(VhwaSdeTrace, GT_ERR,
                "UDMA channel disable failed!!\n");
            status = FVID2_EFAIL;
            break;
        }
        /* LDRA_JUSTIFY_END */
    }

    /* Convert UDMA status to FVID2 status,
       so that the caller of this function always uses FVID2 status only */
    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    Since no error is accumulated due to previously stated rationales, some branches for this branch statement cannot be reached.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested.
    <justification end> */
    if(UDMA_SOK == status)
    /* LDRA_JUSTIFY_END */
    {
        status = FVID2_SOK;
    }
    /* LDRA_JUSTIFY_START
    <metric start> statement branch <metric end>
    <justification start>
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    This branch statement checks for errors accumulated in previous steps.
    The break statement in a previous makes control exit the loop before it reaches here.
    Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
    However, due to the stated rationale, this is not tested. This is redundant and can be removed in a future release.
    <justification end> */
    else
    {
        status = FVID2_EFAIL;
    }
    /* LDRA_JUSTIFY_END */
    return (status);
}

int32_t Vhwa_m2mSdeSubmitRing(Vhwa_M2mSdeInstObj *instObj,
                              const Vhwa_M2mSdeHandleObj *hObj)
{
    int32_t              status = UDMA_SOK;
    uint32_t             chCnt;

    /* Check for Null pointer */
    GT_assert(VhwaSdeTrace, (NULL != instObj));
    GT_assert(VhwaSdeTrace, (NULL != hObj));

    /* Enable the channel which are required */
    for (chCnt = 0; chCnt < VHWA_M2M_SDE_MAX_DMA_CH; chCnt ++)
    {
        /* Submit TRPD to ring */
        #if defined (MCU_PLUS_SDK)
        status = Udma_chRingQueueRaw(instObj->utcChHndl[chCnt],
                                            hObj->chPrms.trMem[chCnt],
                                            1U);
        #else
        status = Udma_ringQueueRaw(instObj->fqRingHndl[chCnt],
            (uint64_t) hObj->chPrms.trMem[chCnt]);
        #endif
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        if(UDMA_SOK != status)
        {
            GT_0trace(VhwaSdeTrace, GT_ERR, "Ring queue failed\n");
        }
        /* LDRA_JUSTIFY_END */
         #if defined (MCU_PLUS_SDK)
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion. The test framework does not support the configuration required to trigger this error scenario. 
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application. However, due to the stated rationale, this is not tested.
            <justification end> */
            if (UDMA_SOK == status)
            /* LDRA_JUSTIFY_END */
            {
                status = Udma_chRingRingDbRaw(instObj->utcChHndl[chCnt], 1U);
            }
        #endif /* SOC_J722S */
    }

    return (status);
}
#if !defined (MCU_PLUS_SDK)
int32_t Vhwa_m2mSdePopRings(Vhwa_M2mSdeInstObj *instObj,
    Vhwa_M2mSdeHandleObj *hObj)
{
    int32_t              status = FVID2_SOK;
    uint32_t             chCnt;
    uint64_t             ringPopVal;
#if !defined(MCU_PLUS_SDK)
    uint32_t repeatCnt;
    Vhwa_M2mSdeChParams *chPrms = NULL;
    Vhwa_M2mSdeChParams *chPrms2 = NULL;
    Vhwa_M2mSdeHandleObj *hObj2 = NULL;
#endif

    /* Check for Null pointer */
    GT_assert(VhwaSdeTrace, (NULL != instObj));
    GT_assert(VhwaSdeTrace, (NULL != hObj));

    /* Enable the channel which are required */
    for (chCnt = 0u; chCnt < VHWA_M2M_SDE_MAX_DMA_CH; chCnt ++)
    {
        #if defined (MCU_PLUS_SDK)
        status = Udma_chRingDeQueueRaw(instObj->utcChHndl[chCnt], 1U, &ringPopVal);
        if(UDMA_SOK != status)
        {
            GT_1trace(VhwaSdeTrace, GT_ERR,
                "UDMA Failed to pop Ring for ch%d!!\n", chCnt);
            break;
        }
        if (UDMA_SOK == status)
        {
            status = Udma_chRingRingRvrDbRaw(instObj->utcChHndl[chCnt], 1U);
        }
        #else
        repeatCnt = 0u;

        chPrms = &hObj->chPrms;

        do
        {
            status = Udma_ringDequeueRaw(instObj->cqRingHndl[chCnt],
                        &ringPopVal);
            
            /* LDRA_JUSTIFY_START
            <metric start> branch <metric end>
            <justification start>
            Rationale: The component level negative test framework and test applications cannot reach this portion.
            This failure case is out of scope for the imaging test framework.
            Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
            <justification end> */
            if(UDMA_SOK == status)
            /* LDRA_JUSTIFY_END */
            {
                break;
            }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        This failure case is out of scope for the imaging test framework.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
        However, due to the stated rationale, this is not tested.
        <justification end> */
            repeatCnt ++;
        } while (repeatCnt < VHWA_SDE_MAX_WAIT_LOOP_CNT);
        /* LDRA_JUSTIFY_END */

        /* LDRA_JUSTIFY_START
        <metric start> branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
        <justification end> */
        if ((repeatCnt < VHWA_SDE_MAX_WAIT_LOOP_CNT) &&
            ((uint32_t)ringPopVal == (uint32_t)chPrms->trMem[chCnt]))
        /* LDRA_JUSTIFY_END */
        {
            status = FVID2_SOK;
        }
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start>
        Rationale: The test framework and test apps cannot reach this portion since triggering out of order UDMA interrupts is not possible from the current test configs.
        Effect on this unit: If the control reaches here, the code base is expected to accumulate and return the error.
        However, due to the stated rationale, this is not tested.
        <justification end> */
        else if ((repeatCnt < VHWA_SDE_MAX_WAIT_LOOP_CNT) &&
            ((uint32_t)ringPopVal != (uint32_t)chPrms->trMem[chCnt]))
        {
            uint32_t hcnt = 0U;
            for (hcnt = 0U; hcnt < VHWA_M2M_SDE_MAX_HANDLES; hcnt ++)
            {
                hObj2 = Vhwa_m2mSdeGetHandleObj(hcnt);
                chPrms2 = &hObj2->chPrms;

                if (((uint32_t)UTRUE == hObj2->isUsed) && 
                ((uint32_t)ringPopVal == (uint32_t)chPrms2->trMem[chCnt]))
                {
                    status = FVID2_SOK;
                    break;
                }
            }
            if(hcnt >= VHWA_M2M_SDE_MAX_HANDLES)
            {
                status = FVID2_EFAIL;
                GT_1trace(VhwaSdeTrace, GT_ERR, "UDMA Ring dequeue mismatch for ch%d!!\n", chCnt);
            }
        }
        /* LDRA_JUSTIFY_END */
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
            However, due to the stated rationale, this is not tested.
        <justification end> */
        else
        {
            if (repeatCnt >= VHWA_SDE_MAX_WAIT_LOOP_CNT)
            {
                status = FVID2_EFAIL;

                GT_1trace(VhwaSdeTrace, GT_ERR, "UDMA Ring dequeue failed for ch%d!!\n", chCnt);
            }
        }
        /* LDRA_JUSTIFY_END */
    #endif
    }

    return (status);
}
#endif
int32_t Vhwa_m2mSdeAllocUdmaMem(Vhwa_M2mSdeHandleObj *hObj)
{
    int32_t     status = FVID2_SOK;
    uint32_t    hIdx;
    uint32_t    chCnt = 0u;

    /* Check for Null pointer */
    GT_assert(VhwaSdeTrace, (NULL != hObj));

    hIdx = hObj->hIdx;

    for (chCnt = 0u; chCnt < VHWA_M2M_SDE_TX_DMA_CH; chCnt ++)
    {
        hObj->chPrms.trMem[chCnt] =
                                &gSdeTxTprdMem[hIdx][chCnt][0U];
        Fvid2Utils_memset(hObj->chPrms.trMem[chCnt], 0x0,
                        VHWA_M2M_SDE_UDMA_TX_TRPD_SIZE);
    }

    hObj->chPrms.trMem[SDE_OUTPUT] = &gSdeRxTprdMem[hIdx][0U][0U];
    Fvid2Utils_memset(hObj->chPrms.trMem[SDE_OUTPUT], 0x0,
                        VHWA_M2M_SDE_UDMA_RX_TRPD_SIZE);

    return (status);
}

void Vhwa_m2mSdeSetTrDesc(const Vhwa_M2mSdeInstObj *instObj,
                          Vhwa_M2mSdeHandleObj *hObj)
{
    CSL_UdmapTR9          *pTr9;
    uint32_t               packetInfoSize;
    Vhwa_M2mSdeChParams   *chPrms;

    packetInfoSize = sizeof(CSL_UdmapTR15);

    chPrms = &hObj->chPrms;

    /* Invalidate cache before changing contents */
    VhwaAl_Cache_Inv(chPrms->trMem[SDE_INPUT_REFERENCE_IMG],
               VHWA_M2M_SDE_UDMA_TX_TRPD_SIZE, CacheP_TYPE_L1D);

    /* Set TR for Reference */
    Vhwa_sdeSetupPacketInfo(chPrms,
                            instObj->complRingNum[SDE_INPUT_REFERENCE_IMG],
                            SDE_INPUT_REFERENCE_IMG);
    pTr9 = (CSL_UdmapTR9 *)(&chPrms->trMem[SDE_INPUT_REFERENCE_IMG]
                                                            [packetInfoSize]);
    Vhwa_sdeSetupInputTR(hObj, pTr9,SDE_INPUT_REFERENCE_IMG );

    /* Write back contents into memory after changing */
    VhwaAl_Cache_Wb(chPrms->trMem[SDE_INPUT_REFERENCE_IMG],
              VHWA_M2M_SDE_UDMA_TX_TRPD_SIZE, CacheP_TYPE_L1D);

    /* Invalidate cache before changing contents */
    VhwaAl_Cache_Inv(chPrms->trMem[SDE_INPUT_BASE_IMG],
               VHWA_M2M_SDE_UDMA_TX_TRPD_SIZE, CacheP_TYPE_L1D);

    /* Set TR for Base */
    Vhwa_sdeSetupPacketInfo(chPrms,
                            instObj->complRingNum[SDE_INPUT_BASE_IMG],
                            SDE_INPUT_BASE_IMG);
    pTr9 = (CSL_UdmapTR9 *)(&chPrms->trMem[SDE_INPUT_BASE_IMG][packetInfoSize]);
    Vhwa_sdeSetupInputTR(hObj, pTr9, SDE_INPUT_BASE_IMG);

    /* Write back contents into memory after changing */
    VhwaAl_Cache_Wb(chPrms->trMem[SDE_INPUT_BASE_IMG],
              VHWA_M2M_SDE_UDMA_TX_TRPD_SIZE, CacheP_TYPE_L1D);

    /* Invalidate cache before changing contents */
    VhwaAl_Cache_Inv(chPrms->trMem[SDE_OUTPUT],
               VHWA_M2M_SDE_UDMA_RX_TRPD_SIZE, CacheP_TYPE_L1D);

    /* Set TR for Output */
    Vhwa_sdeSetupPacketInfo(chPrms,
                            instObj->complRingNum[SDE_OUTPUT],
                            SDE_OUTPUT);
    pTr9 = (CSL_UdmapTR9 *)(&chPrms->trMem[SDE_OUTPUT][packetInfoSize]);
    Vhwa_sdeSetupOutputTR(hObj, chPrms, pTr9);

    /* Write back contents into memory after changing */
    VhwaAl_Cache_Wb(chPrms->trMem[SDE_OUTPUT],
              VHWA_M2M_SDE_UDMA_RX_TRPD_SIZE, CacheP_TYPE_L1D);
}


void Vhwa_m2mSdeSetAddress(Vhwa_M2mSdeHandleObj *hObj,
                           const Fvid2_FrameList *inFrmList,
                           const Fvid2_FrameList *outFrmList)
{
    uint32_t             packetInfoSize, chCnt;
    CSL_UdmapTR9        *pTr = NULL;
    Vhwa_M2mSdeChParams *chPrms = NULL;

    packetInfoSize = sizeof(CSL_UdmapTR15);

    /* Enable the channel which are required */

    chPrms = &hObj->chPrms;
    for (chCnt = 0; chCnt < VHWA_M2M_SDE_MAX_DMA_CH; chCnt ++)
    {
        pTr = (CSL_UdmapTR9 *)(&chPrms->trMem[chCnt][packetInfoSize]);

        if (chCnt == SDE_OUTPUT)
        {
            VhwaAl_Cache_Inv(chPrms->trMem[chCnt],
                       VHWA_M2M_SDE_UDMA_RX_TRPD_SIZE, CacheP_TYPE_L1D);

            Vhwa_sdeSetOutTRAddr(chPrms, pTr,
                                 outFrmList->frames[0]->addr[0]);

            VhwaAl_Cache_Wb(chPrms->trMem[chCnt],
                      VHWA_M2M_SDE_UDMA_RX_TRPD_SIZE, CacheP_TYPE_L1D);
        }
        else
        {
            VhwaAl_Cache_Inv(pTr, packetInfoSize, CacheP_TYPE_L1D);
            pTr->addr = (uint64_t)inFrmList->frames[chCnt]->addr[0];
            VhwaAl_Cache_Wb(pTr, packetInfoSize, CacheP_TYPE_L1D);
        }
    }
}

/* ========================================================================== */
/*                             Local Functions                                */
/* ========================================================================== */

static void Vhwa_sdeSetupPacketInfo(const Vhwa_M2mSdeChParams *chPrms,
                                    uint32_t complRingNum,
                                    uint32_t chType)
{
    uint32_t             numTr;
    uint32_t             descType = CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_TR;
    CSL_UdmapCppi5TRPD  *pTrpd;

    /* Check for Null pointer */
    GT_assert(VhwaSdeTrace, (NULL != chPrms));

    pTrpd = (CSL_UdmapCppi5TRPD *) chPrms->trMem[chType];

    if(SDE_OUTPUT == chType)
    {
        numTr = chPrms->outPrams.numTr;
    }
    else
    {
        numTr = 1u;
    }

    CSL_udmapCppi5SetDescType(pTrpd, descType);
    CSL_udmapCppi5TrSetReload(pTrpd, UFALSE, 0U);
    CSL_udmapCppi5SetPktLen(pTrpd, descType, numTr);
    /* Flow ID and Packet ID */
    CSL_udmapCppi5SetIds(pTrpd, descType, 0U, VHWA_SDE_FLOW_ID);
    CSL_udmapCppi5SetSrcTag(pTrpd, 0x0025);             /* Not used */
    CSL_udmapCppi5SetDstTag(pTrpd, 0x1234);             /* Not used */
    CSL_udmapCppi5TrSetEntryStride(pTrpd,
        CSL_UDMAP_CPPI5_TRPD_PKTINFO_RECSIZE_VAL_64B);
    CSL_udmapCppi5SetReturnPolicy(pTrpd, descType,
        /* Don't care for TR */
        CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
        /* Early return */
        UFALSE, CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
        complRingNum);
}

static void Vhwa_sdeSetOutTRAddr(const Vhwa_M2mSdeChParams *chPrms,
                                 CSL_UdmapTR9 *pTr, uint64_t outAddr)
{
    uint32_t cnt;
    uint64_t addr = outAddr;
    CSL_UdmapTR9 *pTrLocal = pTr;

    /* Set address for TR's */
    for (cnt = 0u; cnt < chPrms->outPrams.numTr; cnt ++)
    {
        pTrLocal->daddr = (uint64_t)(addr + chPrms->outPrams.bufAddrOff[cnt]);
        pTrLocal ++;
    }
}

static void Vhwa_sdeSetupInputTR(Vhwa_M2mSdeHandleObj *hObj,
                                 CSL_UdmapTR9 *pTr, uint32_t chType)
{
    uint32_t widthInBytes, imageHeight;
    uint32_t ddim1;
    Vhwa_M2mSdePrms *sdePrms;

    sdePrms = &hObj->sdePrms;

    widthInBytes = Vhwa_calcHorzSizeInBytes(sdePrms->sdeCfg.width,
                                    sdePrms->inOutImgFmt[chType].ccsFormat);

    imageHeight = sdePrms->sdeCfg.height;

    /* Initialize TR packet: Start */
    /* Setup TR */
    /* Bit fields::
       Bit No.->Info
       3:0->Type: 4D transfer
       7:6->Event Size: Event is generated every time ICNT1 is decremented by 1
       9:8->Trigger0: Local Event
       10:11->Trigger0 Type:
                    The second inner most loop (ICNT1) will be decremented by 1
       13:12->Trigger1: No Trigger
       15:14->Trigger1 Type:
                    The second inner most loop (ICNT1) will be decremented by 1
       23:16->Command ID: The Command ID will come back in TR response
       31:24->Conf Flags: Configuration Specific Flags
       */
    if(hObj->isFocoUsed == UTRUE)
    {
        pTr->flags =
            CSL_FMK(UDMAP_TR_FLAGS_TYPE,
                CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING) |
            CSL_FMK(UDMAP_TR_FLAGS_STATIC, UFALSE) |
            CSL_FMK(UDMAP_TR_FLAGS_EOL, (uint32_t)UFALSE) |   /* NA */
            CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE,
                CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT1_DEC) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0,
                CSL_UDMAP_TR_FLAGS_TRIGGER_LOCAL_EVENT) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE,
                CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC) |
                        /* Cerify this config */
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1,
                CSL_UDMAP_TR_FLAGS_TRIGGER_NONE) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE,
                CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC) |
            /* CmdId: This will come back in TR response */
            CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, (uint32_t)0x25U) |
            CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, (uint32_t)0U) |
            CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, (uint32_t)0U) |
            CSL_FMK(UDMAP_TR_FLAGS_EOP, (uint32_t)1U);

        pTr->icnt0    = (uint16_t)widthInBytes;
        pTr->icnt1    = (uint16_t)imageHeight;
        pTr->icnt2    = 1U;
        pTr->icnt3    = 1U;
        pTr->dim1     = (int32_t)sdePrms->inOutImgFmt[chType].pitch[0U];
        pTr->dim2     = 0;
        pTr->dim3     = 0;

        pTr->dicnt0   = (uint16_t)widthInBytes;

        if(chType == SDE_INPUT_REFERENCE_IMG)
        {
            pTr->daddr    = hObj->sl2Prms.sl2Addr[SDE_INPUT_FOCO_REF_IMG];
            pTr->dicnt1   = SDE_FOCO_SL2_BUFF_DEPTH;
        }
        else
        {
            pTr->daddr    = hObj->sl2Prms.sl2Addr[SDE_INPUT_FOCO_BASE_IMG];
            pTr->dicnt1   = SDE_FOCO_SL2_BUFF_DEPTH;
        }

        pTr->dicnt2   = (uint16_t)imageHeight/pTr->dicnt1;
        pTr->dicnt3   = 1U;

        ddim1 = (widthInBytes + 63u) & 0xFFFFFFC0u;
        pTr->ddim1    = (int32_t)ddim1;
        pTr->ddim2    = 0;
        pTr->ddim3    = 0;
    }
    else
    {
        pTr->flags =
            CSL_FMK(UDMAP_TR_FLAGS_TYPE,
                CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING) |
            CSL_FMK(UDMAP_TR_FLAGS_STATIC, UFALSE) |
            CSL_FMK(UDMAP_TR_FLAGS_EOL, (uint32_t)UFALSE) |   /* NA */
            CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE,
                CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT2_DEC) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0,
                CSL_UDMAP_TR_FLAGS_TRIGGER_LOCAL_EVENT) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE,
                CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC) |
                        /* Cerify this config */
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1,
                CSL_UDMAP_TR_FLAGS_TRIGGER_NONE) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE,
                CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC) |
            /* CmdId: This will come back in TR response */
            CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, (uint32_t)0x25U) |
            CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, (uint32_t)0U) |
            CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, (uint32_t)0U) |
            CSL_FMK(UDMAP_TR_FLAGS_EOP, (uint32_t)1U);

        pTr->icnt0    = (uint16_t)widthInBytes;
        pTr->icnt1    = SDE_SL2_REF_BASE_IMG_BUFF_HEIGHT;
        pTr->icnt2    = (uint16_t)imageHeight/SDE_SL2_REF_BASE_IMG_BUFF_HEIGHT;
        pTr->icnt3    = 1U;
        pTr->dim1     = (int32_t)sdePrms->inOutImgFmt[chType].pitch[0U];
        pTr->dim2     = pTr->dim1 * (int32_t)pTr->icnt1;
        pTr->dim3     = 0;

        pTr->dicnt0   = (uint16_t)widthInBytes;
        pTr->dicnt1   = pTr->icnt1;

        pTr->dicnt2   = SDE_SL2_REF_BASE_IMG_BUFF_COUNT;

        pTr->dicnt3   = (uint16_t)imageHeight/(pTr->dicnt2 * pTr->dicnt1);
        if((pTr->dicnt3 * pTr->dicnt2 * pTr->dicnt1) != imageHeight)
        {
            pTr->dicnt3++;
        }
        pTr->ddim1    = (int32_t)hObj->sl2Prms.sl2Pitch[chType];
        pTr->ddim2    = pTr->ddim1 * (int32_t)pTr->dicnt1;
        pTr->ddim3    = 0;
        pTr->daddr    = hObj->sl2Prms.sl2Addr[chType];
    }

    pTr->addr     = 0x0U;
    pTr->fmtflags = 0x0U |
            CSL_FMK(UDMAP_TR_FMTFLAGS_AMODE,
                CSL_UDMAP_TR_FMTFLAGS_AMODE_LINEAR)                         |
            CSL_FMK(UDMAP_TR_FMTFLAGS_DIR,
                CSL_UDMAP_TR_FMTFLAGS_DIR_SRC_USES_AMODE)                   |
            CSL_FMK(UDMAP_TR_FMTFLAGS_ELYPE, CSL_UDMAP_TR_FMTFLAGS_ELYPE_1);

}


static void Vhwa_sdeSetupOutputTR(const Vhwa_M2mSdeHandleObj *hObj,
                                  const Vhwa_M2mSdeChParams *chPrms,
                                  CSL_UdmapTR9 *pTr)
{
    uint32_t  trCnt;
    uint64_t  sl2BuffAddr;
    CSL_UdmapTR9 *pTrLocal = pTr;

    sl2BuffAddr = hObj->sl2Prms.sl2Addr[SDE_OUTPUT];

    for (trCnt = 0u; trCnt < chPrms->outPrams.numTr; trCnt ++)
    {
        /* Initialize TR packet: Start */
        /* Setup TR */
        /* Bit fields::
           Bit No.->Info
           3:0->Type: 4D transfer
           7:6->Event Size: Event is generated every time ICNT1 is decremented by 1
           9:8->Trigger0: Local Event
           10:11->Trigger0 Type:
                        The second inner most loop (ICNT1) will be decremented by 1
           13:12->Trigger1: No Trigger
           15:14->Trigger1 Type:
                        The second inner most loop (ICNT1) will be decremented by 1
           23:16->Command ID: The Command ID will come back in TR response
           31:24->Conf Flags: Configuration Specific Flags
           */
        pTrLocal->flags =
            CSL_FMK(UDMAP_TR_FLAGS_TYPE,
                CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING) |
            CSL_FMK(UDMAP_TR_FLAGS_STATIC, UFALSE) |
            CSL_FMK(UDMAP_TR_FLAGS_EOL, (uint32_t)UFALSE) |   /* NA */
            CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE,
                CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT2_DEC) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0,
                CSL_UDMAP_TR_FLAGS_TRIGGER_LOCAL_EVENT) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE,
                CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC) |
                        /* Check this */
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1,
                CSL_UDMAP_TR_FLAGS_TRIGGER_NONE) |
            CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE,
                CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC) |
            /* CmdId: This will come back in TR response */
            CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, (uint32_t)0x25U) |
            CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, (uint32_t)0U) |
            CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, (uint32_t)0U) |
            CSL_FMK(UDMAP_TR_FLAGS_EOP, (uint32_t)1U);

        pTrLocal->fmtflags = 0x0U |
            CSL_FMK(UDMAP_TR_FMTFLAGS_AMODE,
                CSL_UDMAP_TR_FMTFLAGS_AMODE_LINEAR)                         |
            CSL_FMK(UDMAP_TR_FMTFLAGS_DIR,
                CSL_UDMAP_TR_FMTFLAGS_DIR_SRC_USES_AMODE)                   |
            CSL_FMK(UDMAP_TR_FMTFLAGS_ELYPE, CSL_UDMAP_TR_FMTFLAGS_ELYPE_1);


        pTrLocal->icnt0 = SDE_OUTPUT_BLOCK_WIDTH_IN_BYTE;
        pTrLocal->icnt1 = (uint16_t)chPrms->outPrams.sicnt1[trCnt];

        pTrLocal->icnt2 = (uint16_t)chPrms->outPrams.sicnt2[trCnt];
        pTrLocal->icnt3 = (uint16_t)chPrms->outPrams.sicnt3[trCnt];
        pTrLocal->dim1 = (int32_t)pTrLocal->icnt0;
        pTrLocal->dim2 = pTrLocal->dim1 * (int32_t)SDE_OUTPUT_BLOCK_HEIGHT;

        pTrLocal->dim3 = 0;

        pTrLocal->dicnt0 = pTrLocal->icnt0;
        pTrLocal->dicnt1 = pTrLocal->icnt1;

        pTrLocal->dicnt2 = (uint16_t)chPrms->outPrams.dicnt2[trCnt];
        pTrLocal->dicnt3 = (uint16_t)chPrms->outPrams.dicnt3[trCnt];

        pTrLocal->ddim1 = (int32_t)hObj->sdePrms.inOutImgFmt[SDE_OUTPUT].pitch[0U];
        pTrLocal->ddim2 = (int32_t)SDE_OUTPUT_BLOCK_WIDTH_IN_BYTE;

        /* Effective only for TR's for which dicnt3 is not 1 */
        pTrLocal->ddim3 = (int32_t)hObj->sdePrms.inOutImgFmt[SDE_OUTPUT].pitch[0U] *
                      (int32_t)pTrLocal->dicnt1;

        pTrLocal->addr  = sl2BuffAddr + chPrms->outPrams.sl2AddrOff[trCnt];
        pTrLocal->daddr = 0U;

        pTrLocal++;
    }

}
