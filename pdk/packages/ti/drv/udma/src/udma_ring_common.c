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
 *  \file udma_ring.c
 *
 *  \brief File containing the UDMA driver ring related APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/src/udma_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Max number of door bell ring that can be performed at one go */
#define UDMA_RING_MAX_DB_RING_CNT       (127U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static inline void Udma_ringAssertFnPointers(Udma_DrvHandle drvHandle);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Udma_ringAlloc(Udma_DrvHandle drvHandle,
                       Udma_RingHandle ringHandle,
                       uint16_t ringNum,
                       const Udma_RingPrms *ringPrms)
{
    int32_t             retVal = UDMA_SOK;
    uint64_t            physBase;
    uint32_t            allocDone = UFALSE;
    struct tisci_msg_rm_ring_cfg_req    rmRingReq;
    struct tisci_msg_rm_ring_cfg_resp   rmRingResp;

    /* Error check */
    if((NULL_PTR == drvHandle) || (NULL_PTR == ringHandle) || (NULL_PTR == ringPrms))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        if(UDMA_INIT_DONE != drvHandle->drvInitDone)
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        retVal = Udma_ringCheckParams(drvHandle, ringPrms);
    }

    if(UDMA_SOK == retVal)
    {
        if(UDMA_RING_ANY == ringNum)
        {
            /* Alloc free ring */
            if(UDMA_MAPPED_GROUP_INVALID == ringPrms->mappedRingGrp)
            {
                ringHandle->ringNum = Udma_rmAllocFreeRing(drvHandle);
            }
#if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
            else
            {
                ringHandle->ringNum = Udma_rmAllocMappedRing(drvHandle, ringPrms->mappedRingGrp, ringPrms->mappedChNum);
            }
#endif
            if(UDMA_RING_INVALID == ringHandle->ringNum)
            {
                retVal = UDMA_EALLOC;
            }
            else
            {
                allocDone = UTRUE;
            }
        }
        else
        {
            if(ringNum >= drvHandle->maxRings)
            {
                Udma_printf(drvHandle, "[Error] Out of range ring index!!!\n");
                retVal = UDMA_EINVALID_PARAMS;
            }
            else
            {
                ringHandle->ringNum = ringNum;
            }
        }
    }

    if(UDMA_SOK == retVal)
    {
        Udma_ringAssertFnPointers(drvHandle);
        ringHandle->drvHandle = drvHandle;
        /* Set the mapped group in ringHandle, since only ringHandle is passed to rmFreeMappedRing() and
         * the mapped group parameter is required to reset the appropriate flag */
        ringHandle->mappedRingGrp   = ringPrms->mappedRingGrp;
        ringHandle->mappedChNum     = ringPrms->mappedChNum;
        drvHandle->ringSetCfg(drvHandle, ringHandle, ringPrms);

#if (UDMA_SOC_CFG_APPLY_RING_WORKAROUND == 1)
        /* Perform ring reset */
        retVal = Udma_ringReset(drvHandle, ringHandle);
        if(UDMA_SOK != retVal)
        {
            Udma_printf(drvHandle, "[Error] Ring reset failed!!!\n");
        }
#endif
    }

    if(UDMA_SOK == retVal)
    {
        /* Configure ring */
        rmRingReq.valid_params  = TISCI_MSG_VALUE_RM_RING_ADDR_LO_VALID |
                                  TISCI_MSG_VALUE_RM_RING_ADDR_HI_VALID |
                                  TISCI_MSG_VALUE_RM_RING_COUNT_VALID |
                                  TISCI_MSG_VALUE_RM_RING_MODE_VALID |
                                  TISCI_MSG_VALUE_RM_RING_SIZE_VALID |
                                  TISCI_MSG_VALUE_RM_RING_ORDER_ID_VALID |
                                  TISCI_MSG_VALUE_RM_RING_ASEL_VALID;
        rmRingReq.nav_id        = drvHandle->devIdRing;
        rmRingReq.index         = ringHandle->ringNum;
        physBase = Udma_virtToPhyFxn(ringPrms->ringMem, drvHandle, (Udma_ChHandle) NULL_PTR);
        rmRingReq.addr_lo       = (uint32_t)physBase;
        rmRingReq.addr_hi       = (uint32_t)(physBase >> 32UL);
        rmRingReq.count         = ringPrms->elemCnt;
        rmRingReq.mode          = ringPrms->mode;
        rmRingReq.size          = ringPrms->elemSize;
        rmRingReq.order_id      = ringPrms->orderId;
        rmRingReq.asel          = ringPrms->asel;

        if(UDMA_RING_VIRTID_INVALID != ringPrms->virtId)
        {
            rmRingReq.valid_params |= TISCI_MSG_VALUE_RM_RING_VIRTID_VALID;
            rmRingReq.virtid        = ringPrms->virtId;
        }

        retVal = Sciclient_rmRingCfg(
                     &rmRingReq, &rmRingResp, UDMA_SCICLIENT_TIMEOUT);
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle, "[Error] Ring config failed!!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        ringHandle->ringInitDone = UDMA_INIT_DONE;
    }
    else
    {
        /* Error. Free-up resource if allocated */
        if(UTRUE == allocDone)
        {
            if(UDMA_MAPPED_GROUP_INVALID == ringPrms->mappedRingGrp)
            {
                Udma_rmFreeFreeRing(ringHandle->ringNum, drvHandle);
            }
#if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
            else
            {
                Udma_rmFreeMappedRing(ringHandle->ringNum, drvHandle, ringHandle->mappedRingGrp, ringHandle->mappedChNum);
            }
#endif
        }
    }

    return (retVal);
}

int32_t Udma_ringFree(Udma_RingHandle ringHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if(NULL_PTR == ringHandle)
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        if(UDMA_INIT_DONE != ringHandle->ringInitDone)
        {
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = ringHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Free-up event resources */
        Udma_assert(drvHandle, (UDMA_RING_INVALID != ringHandle->ringNum));
        if(UDMA_MAPPED_GROUP_INVALID == ringHandle->mappedRingGrp)
        {
            Udma_rmFreeFreeRing(ringHandle->ringNum, drvHandle);
        }
#if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
        else
        {
            Udma_rmFreeMappedRing(ringHandle->ringNum, drvHandle, ringHandle->mappedRingGrp, ringHandle->mappedChNum);
        }
#endif
        ringHandle->ringNum         = UDMA_RING_INVALID;
        ringHandle->ringInitDone    = UDMA_DEINIT_DONE;

        drvHandle->ringHandleClearRegs(ringHandle);

        ringHandle->drvHandle       = (Udma_DrvHandle) NULL_PTR;
    }

    return (retVal);
}

int32_t Udma_ringAttach(Udma_DrvHandle drvHandle,
                        Udma_RingHandle ringHandle,
                        uint16_t ringNum)
{
    int32_t             retVal = UDMA_SOK;

    /* Error check */
    if((NULL_PTR == drvHandle) || (NULL_PTR == ringHandle))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        if(UDMA_INIT_DONE != drvHandle->drvInitDone)
        {
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        if(ringNum >= drvHandle->maxRings)
        {
            Udma_printf(drvHandle, "[Error] Out of range ring index!!!\n");
            retVal = UDMA_EINVALID_PARAMS;
        }
    }

    if(UDMA_SOK == retVal)
    {
        ringHandle->ringNum = ringNum;
        ringHandle->drvHandle = drvHandle;
        drvHandle->ringSetCfg(drvHandle, ringHandle, (Udma_RingPrms *) NULL_PTR);

        ringHandle->ringInitDone = UDMA_INIT_DONE;
    }

    return (retVal);
}

int32_t Udma_ringDetach(Udma_RingHandle ringHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if(NULL_PTR == ringHandle)
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        if(UDMA_INIT_DONE != ringHandle->ringInitDone)
        {
            retVal = UDMA_EFAIL;
        }
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = ringHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Clear handle object */
        Udma_assert(drvHandle, (UDMA_RING_INVALID != ringHandle->ringNum));
        ringHandle->ringInitDone    = UDMA_DEINIT_DONE;

        drvHandle->ringHandleClearRegs(ringHandle);

        ringHandle->drvHandle       = (Udma_DrvHandle) NULL_PTR;

    }

    return (retVal);
}

int32_t Udma_ringQueueRaw(Udma_RingHandle ringHandle, uint64_t phyDescMem)
{
    int32_t         retVal = UDMA_SOK;
    uintptr_t       cookie;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR          == ringHandle) ||
       (UDMA_INIT_DONE    != ringHandle->ringInitDone) ||
       (UDMA_RING_INVALID == ringHandle->ringNum))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = ringHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        cookie = drvHandle->initPrms.osalPrms.disableAllIntr();

        retVal = drvHandle->ringQueueRaw(drvHandle,ringHandle,phyDescMem);

        drvHandle->initPrms.osalPrms.restoreAllIntr(cookie);
    }

    return (retVal);
}

int32_t Udma_ringDequeueRaw(Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    int32_t         retVal = UDMA_SOK;
    uintptr_t       cookie;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR          == ringHandle) ||
       (UDMA_INIT_DONE    != ringHandle->ringInitDone) ||
       (UDMA_RING_INVALID == ringHandle->ringNum))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = ringHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        cookie = drvHandle->initPrms.osalPrms.disableAllIntr();

        retVal = drvHandle->ringDequeueRaw(drvHandle,ringHandle,phyDescMem);

        drvHandle->initPrms.osalPrms.restoreAllIntr(cookie);
    }

    return (retVal);
}

int32_t Udma_ringFlushRaw(Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == ringHandle) ||
       (UDMA_INIT_DONE != ringHandle->ringInitDone) ||
       (UDMA_RING_INVALID == ringHandle->ringNum))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = ringHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        retVal = drvHandle->ringFlushRaw(drvHandle,ringHandle,phyDescMem);
    }

    return (retVal);
}

int32_t Udma_ringFlushAll(Udma_RingHandle ringHandle, uint64_t *phyDescMem, uint64_t retryCnt)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;
    uint64_t        curRetryCnt = 0U;

    /* Error check */
    if((NULL_PTR == ringHandle) ||
       (UDMA_INIT_DONE != ringHandle->ringInitDone) ||
       (UDMA_RING_INVALID == ringHandle->ringNum))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = ringHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        while ((retVal != UDMA_ETIMEOUT) && (retryCnt > curRetryCnt))
        {
            retVal = drvHandle->ringFlushRaw(drvHandle,ringHandle,phyDescMem);
            curRetryCnt++;
        }
        /* ringFlushRaw will return UDMA_ETIMEOUT if ring is empty */
        if(retVal == UDMA_ETIMEOUT)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            retVal = UDMA_EFAIL;
        }
    }

    return (retVal);
}

void Udma_ringPrime(Udma_RingHandle ringHandle, uint64_t phyDescMem)
{
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    drvHandle->ringPrime(ringHandle,phyDescMem);

    return;
}

void Udma_ringPrimeRead(Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    drvHandle->ringPrimeRead(ringHandle,phyDescMem);

    return;
}

void Udma_ringSetDoorBell(Udma_RingHandle ringHandle, int32_t count)
{
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    drvHandle->ringSetDoorBell(ringHandle,count);

    return;
}

uint16_t Udma_ringGetNum(Udma_RingHandle ringHandle)
{
    uint16_t        ringNum = UDMA_RING_INVALID;

    if((NULL_PTR != ringHandle) && (UDMA_INIT_DONE == ringHandle->ringInitDone))
    {
        ringNum = ringHandle->ringNum;
    }

    return (ringNum);
}

void *Udma_ringGetMemPtr(Udma_RingHandle ringHandle)
{
    void   *ringMem = NULL_PTR;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    ringMem = drvHandle->ringGetMemPtr(ringHandle);

    return (ringMem);
}

uint32_t Udma_ringGetMode(Udma_RingHandle ringHandle)
{
    uint32_t ringMode = UDMA_RING_MODE_INVALID;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    ringMode = drvHandle->ringGetMode(ringHandle);

    return (ringMode);
}

uint32_t Udma_ringGetElementCnt(Udma_RingHandle ringHandle)
{
    uint32_t size = 0U;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    size = drvHandle->ringGetElementCnt(ringHandle);

    return (size);
}

uint32_t Udma_ringGetForwardRingOcc(Udma_RingHandle ringHandle)
{
    uint32_t occ = 0U;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    occ = drvHandle->ringGetForwardRingOcc(ringHandle);

    return (occ);
}

uint32_t Udma_ringGetReverseRingOcc(Udma_RingHandle ringHandle)
{
    uint32_t occ = 0U;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    occ = drvHandle->ringGetReverseRingOcc(ringHandle);

    return (occ);
}

uint32_t Udma_ringGetWrIdx(Udma_RingHandle ringHandle)
{
    uint32_t idx = 0U;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    idx = drvHandle->ringGetWrIdx(ringHandle);

    return (idx);
}

uint32_t Udma_ringGetRdIdx(Udma_RingHandle ringHandle)
{
    uint32_t idx = 0U;
    Udma_DrvHandle  drvHandle = ringHandle->drvHandle;

    idx = drvHandle->ringGetRdIdx(ringHandle);

    return (idx);
}

int32_t Udma_ringMonAlloc(Udma_DrvHandle drvHandle,
                          Udma_RingMonHandle monHandle,
                          uint16_t ringMonNum)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    allocDone = UFALSE;
    uint32_t    instType;

    /* Error check */
    if((NULL_PTR == monHandle) || (NULL_PTR == drvHandle))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        instType = drvHandle->instType;

        if(UDMA_INST_TYPE_NORMAL == instType)
        {
#if (UDMA_SOC_CFG_RING_MON_PRESENT == 1)
            if(UDMA_RING_MON_ANY == ringMonNum)
            {
                /* Alloc free ring MONITOR */
                monHandle->ringMonNum = Udma_rmAllocRingMon(drvHandle);
                if(UDMA_RING_MON_INVALID == monHandle->ringMonNum)
                {
                    retVal = UDMA_EALLOC;
                }
                else
                {
                    allocDone = UTRUE;
                }
            }
            else
            {
                if(ringMonNum >= drvHandle->maxRingMon)
                {
                    Udma_printf(drvHandle, "[Error] Out of range ring monitor index!!!\n");
                    retVal = UDMA_EINVALID_PARAMS;
                }
                else
                {
                    monHandle->ringMonNum = ringMonNum;
                }
            }

            if(UDMA_SOK == retVal)
            {
                monHandle->drvHandle = drvHandle;
                Udma_assert(drvHandle, (NULL_PTR != drvHandle->raRegs.pMonRegs));
                monHandle->pMonRegs =
                    &drvHandle->raRegs.pMonRegs->MON[monHandle->ringMonNum];
                monHandle->ringMonInitDone = UDMA_INIT_DONE;
            }
            else
            {
                /* Error. Free-up resource if allocated */
                if(UTRUE == allocDone)
                {
                    Udma_rmFreeRingMon(monHandle->ringMonNum, drvHandle);
                }
            }
#endif
        }
        else
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "[Error] Ring Monitor only supported for Main/MCU Navss instances; Event Config failed!!!\n");
        }
    }

    return (retVal);
}

int32_t Udma_ringMonFree(Udma_RingMonHandle monHandle)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR           == monHandle) ||
    (UDMA_INIT_DONE        != monHandle->ringMonInitDone) ||
    (UDMA_RING_MON_INVALID == monHandle->ringMonNum))
    {
        retVal = UDMA_EBADARGS;
    }

    if(UDMA_SOK == retVal)
    {
        drvHandle = monHandle->drvHandle;

        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        uint32_t instType = drvHandle->instType;

        if(UDMA_INST_TYPE_NORMAL == instType)
        {

#if (UDMA_SOC_CFG_RING_MON_PRESENT == 1)
            /* Free-up event resources */
            Udma_assert(drvHandle, (UDMA_RING_MON_INVALID != monHandle->ringMonNum));
            Udma_rmFreeRingMon(monHandle->ringMonNum, drvHandle);
            monHandle->drvHandle        = (Udma_DrvHandle) NULL_PTR;
            monHandle->ringMonNum       = UDMA_RING_MON_INVALID;
            monHandle->pMonRegs         = (volatile CSL_ringacc_monitorRegs_mon *) NULL_PTR;
            monHandle->ringMonInitDone  = UDMA_DEINIT_DONE;
#endif
        }
        else
        {
            retVal = UDMA_EFAIL;
        }
    }

    return (retVal);
}

int32_t Udma_ringMonConfig(Udma_RingMonHandle monHandle,
                           const Udma_RingMonPrms *monPrms)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == monHandle) ||
    (NULL_PTR    == monPrms)   ||
    (UDMA_INIT_DONE        != monHandle->ringMonInitDone) ||
    (UDMA_RING_MON_INVALID == monHandle->ringMonNum))
    {
        retVal = UDMA_EBADARGS;
    }

    if(UDMA_SOK == retVal)
    {
        drvHandle = monHandle->drvHandle;

        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }


    if(UDMA_SOK == retVal)
    {
        uint32_t instType = drvHandle->instType;

        if(UDMA_INST_TYPE_NORMAL == instType)
        {
#if (UDMA_SOC_CFG_RING_MON_PRESENT == 1)
            struct tisci_msg_rm_ring_mon_cfg_req    rmRingMonReq;
            struct tisci_msg_rm_ring_mon_cfg_resp   rmRingMonResp;
            memset(&rmRingMonResp, 0, sizeof(rmRingMonResp));
            rmRingMonReq.valid_params   = TISCI_MSG_VALUE_RM_MON_SOURCE_VALID |
                                        TISCI_MSG_VALUE_RM_MON_MODE_VALID |
                                        TISCI_MSG_VALUE_RM_MON_QUEUE_VALID;
            if((monPrms->mode != TISCI_MSG_VALUE_RM_MON_MODE_WATERMARK) &&
               (monPrms->mode != TISCI_MSG_VALUE_RM_MON_MODE_STARVATION))
            {
                rmRingMonReq.valid_params |= TISCI_MSG_VALUE_RM_MON_DATA0_VAL_VALID |
                                             TISCI_MSG_VALUE_RM_MON_DATA1_VAL_VALID;
            }
            rmRingMonReq.nav_id         = drvHandle->devIdRing;
            rmRingMonReq.index          = monHandle->ringMonNum;
            rmRingMonReq.source         = monPrms->source;
            rmRingMonReq.mode           = monPrms->mode;
            rmRingMonReq.queue          = monPrms->ringNum;
            if((monPrms->mode != TISCI_MSG_VALUE_RM_MON_MODE_WATERMARK) &&
               (monPrms->mode != TISCI_MSG_VALUE_RM_MON_MODE_STARVATION))
            {
                rmRingMonReq.data0_val      = monPrms->data0;
                rmRingMonReq.data1_val      = monPrms->data1;
            }
            retVal = Sciclient_rmRingMonCfg(
                        &rmRingMonReq, &rmRingMonResp, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] Ring monitor config failed!!!\n");
            }
#endif
        }
        else
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "[Error] Ring Monitor only supported for Main/MCU Navss instances; Event Config failed!!!\n");
        }
    }

    return (retVal);
}

int32_t Udma_ringMonGetData(Udma_RingMonHandle monHandle,
                            Udma_RingMonData *monData)
{
    int32_t         retVal = UDMA_SOK;
    Udma_DrvHandle  drvHandle;

    /* Error check */
    if((NULL_PTR == monHandle) ||
    (NULL_PTR == monData) ||
    (UDMA_INIT_DONE        != monHandle->ringMonInitDone) ||
    (UDMA_RING_MON_INVALID == monHandle->ringMonNum))
    {
        retVal = UDMA_EBADARGS;
    }
    if(UDMA_SOK == retVal)
    {
        drvHandle = monHandle->drvHandle;
        if((NULL_PTR == drvHandle) || (UDMA_INIT_DONE != drvHandle->drvInitDone))
        {
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        uint32_t instType = drvHandle->instType;

        if(UDMA_INST_TYPE_NORMAL == instType)
        {
#if (UDMA_SOC_CFG_RING_MON_PRESENT == 1)
            Udma_assert(drvHandle, (NULL_PTR != monHandle->pMonRegs));
            monData->data0 = CSL_REG32_RD(&monHandle->pMonRegs->DATA0);
            monData->data1 = CSL_REG32_RD(&monHandle->pMonRegs->DATA1);
#endif
        }
        else
        {
            retVal = UDMA_EFAIL;
            Udma_printf(drvHandle, "[Error] Ring Monitor only supported for Main/MCU Navss instances; Event Config failed!!!\n");
        }
    }

    return (retVal);
}

uint16_t Udma_ringMonGetNum(Udma_RingMonHandle monHandle)
{
    uint16_t        ringMonNum = UDMA_RING_MON_INVALID;

#if (UDMA_SOC_CFG_RING_MON_PRESENT == 1)
    if((NULL_PTR != monHandle) && (UDMA_INIT_DONE == monHandle->ringMonInitDone))
    {
        ringMonNum = monHandle->ringMonNum;
    }
#endif

    return (ringMonNum);
}

int32_t UdmaRingPrms_init(Udma_RingPrms *ringPrms)
{
    int32_t             retVal = UDMA_SOK;

    if(NULL_PTR != ringPrms)
    {
        ringPrms->ringMem       = NULL_PTR;
        ringPrms->ringMemSize   = UDMA_RING_SIZE_CHECK_SKIP;
        ringPrms->mode          = TISCI_MSG_VALUE_RM_RING_MODE_RING;
        ringPrms->virtId        = UDMA_RING_VIRTID_INVALID;
        ringPrms->elemCnt       = 0U;
        ringPrms->elemSize      = UDMA_RING_ES_8BYTES;
        ringPrms->orderId       = UDMA_DEFAULT_RING_ORDER_ID;
        ringPrms->asel          = UDMA_RINGACC_ASEL_ENDPOINT_PHYSADDR;
        ringPrms->mappedRingGrp = UDMA_MAPPED_GROUP_INVALID;
        ringPrms->mappedChNum   = UDMA_DMA_CH_INVALID;
    }
    else
    {
        retVal = UDMA_EINVALID_PARAMS;
    }

    return retVal;
}

int32_t UdmaRingMonPrms_init(Udma_RingMonPrms *monPrms)
{
    int32_t             retVal = UDMA_SOK;

    if(NULL_PTR != monPrms)
    {
        monPrms->source     = TISCI_MSG_VALUE_RM_MON_SRC_ELEM_CNT;
        monPrms->mode       = TISCI_MSG_VALUE_RM_MON_MODE_DISABLED;
        monPrms->ringNum    = UDMA_RING_INVALID;
        monPrms->data0      = 0U;
        monPrms->data1      = 0U;
    }
    else
    {
        retVal = UDMA_EINVALID_PARAMS;
    }

    return retVal;
}

int32_t Udma_ringProxyQueueRaw(Udma_RingHandle ringHandle,
                               Udma_DrvHandle drvHandle,
                               uint64_t phyDescMem)
{
    int32_t             retVal = UDMA_SOK;
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
        uint32_t            ringHwOcc;
        CSL_ProxyThreadCfg  threadCfg;

        /* Get ring occupancy. We should write to FIFO (through proxy) only
        * when there is room available. Otherwise ring will overflow */
        ringHwOcc = CSL_ringaccGetRingHwOcc(
                        &ringHandle->drvHandle->raRegs, ringHandle->ringNum);
        if(ringHwOcc >= ringHandle->cfg.elCnt)
        {
            /* Ring full */
            retVal = UDMA_EFAIL;
        }
        else
        {
            threadCfg.mode      = CSL_PROXY_QUEUE_ACCESS_MODE_TAIL;
            threadCfg.elSz      = (uint32_t)sizeof(uint64_t);
            threadCfg.queueNum  = ringHandle->ringNum;
            threadCfg.errEvtNum = UDMA_EVENT_INVALID;
            retVal = CSL_proxyCfgThread(
                        &drvHandle->proxyCfg,
                        drvHandle->proxyTargetNumRing,
                        drvHandle->initPrms.rmInitPrms.proxyThreadNum,
                        &threadCfg);
            if(UDMA_SOK == retVal)
            {
                Udma_proxyWrite64(ringHandle->proxyAddr, phyDescMem);
            }
        }
#endif
    }
    else
    {
        retVal = UDMA_EFAIL;
        Udma_printf(drvHandle, "[Error] Proxy not present!!!\n");
    }

    return (retVal);
}

int32_t Udma_ringProxyDequeueRaw(Udma_RingHandle ringHandle,
                                        Udma_DrvHandle drvHandle,
                                        uint64_t *phyDescMem)
{
    int32_t             retVal = UDMA_SOK;
    if(UDMA_INST_TYPE_NORMAL == drvHandle->instType)
    {
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
        uint32_t            ringHwOcc;
        CSL_ProxyThreadCfg  threadCfg;

        /* Get ring occupancy. We should read from FIFO (through proxy) only
        * when something is available. Otherwise ring will underflow */
        ringHwOcc = CSL_ringaccGetRingHwOcc(
                        &ringHandle->drvHandle->raRegs, ringHandle->ringNum);
        if(0U == ringHwOcc)
        {
            /* Nothing to flush */
            retVal = UDMA_ETIMEOUT;
        }
        else
        {
            threadCfg.mode      = CSL_PROXY_QUEUE_ACCESS_MODE_HEAD;
            threadCfg.elSz      = (uint32_t)sizeof(uint64_t);
            threadCfg.queueNum  = ringHandle->ringNum;
            threadCfg.errEvtNum = UDMA_EVENT_INVALID;
            retVal = CSL_proxyCfgThread(
                        &drvHandle->proxyCfg,
                        drvHandle->proxyTargetNumRing,
                        drvHandle->initPrms.rmInitPrms.proxyThreadNum,
                        &threadCfg);
            if(UDMA_SOK == retVal)
            {
                Udma_proxyRead64(ringHandle->proxyAddr, phyDescMem);
            }
        }
#endif
    }
    else
    {
        retVal = UDMA_EFAIL;
        Udma_printf(drvHandle, "[Error] Proxy not present!!!\n");
    }

    return (retVal);
}

int32_t Udma_ringCheckParams(Udma_DrvHandle drvHandle,
                                    const Udma_RingPrms *ringPrms)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    ringMemSize;

    Udma_assert(drvHandle, (NULL_PTR != ringPrms));

    if(NULL_PTR == ringPrms->ringMem)
    {
        retVal = UDMA_EINVALID_PARAMS;
        Udma_printf(drvHandle, "[Error] Ring memory should not be NULL_PTR!!!\n");
    }
    else
    {
        /* Check 128 byte alignment */
        if(0U != ((uintptr_t)ringPrms->ringMem & (UDMA_CACHELINE_ALIGNMENT - 1U)))
        {
            retVal = UDMA_EINVALID_PARAMS;
            Udma_printf(drvHandle, "[Error] Ring memory not aligned!!!\n");
        }
    }

    if(0U == ringPrms->elemCnt)
    {
        retVal = UDMA_EINVALID_PARAMS;
        Udma_printf(drvHandle,
            "[Error] Ring element count should not be zero!!!\n");
    }

    if(UDMA_INST_TYPE_NORMAL != drvHandle->instType)
    {
        if(TISCI_MSG_VALUE_RM_RING_MODE_RING != ringPrms->mode)
        {
            retVal = UDMA_EINVALID_PARAMS;
            Udma_printf(drvHandle, "[Error] Invalid Ring Mode for LCDMA!!!\n");
        }
    }

    if(UDMA_RING_SIZE_CHECK_SKIP != ringPrms->ringMemSize)
    {
        /* Get ring memory size */
        ringMemSize = UdmaUtils_getRingMemSize(
                          ringPrms->mode,
                          ringPrms->elemCnt,
                          ringPrms->elemSize);
        if(ringPrms->ringMemSize < ringMemSize)
        {
            retVal = UDMA_EALLOC;
            Udma_printf(drvHandle, "[Error] Ring memory not sufficient!!!\n");
        }
    }

    if (UDMA_RING_ORDERID_MAX < ringPrms->orderId)
    {
        retVal = UDMA_EINVALID_PARAMS;
        Udma_printf(drvHandle, "[Error] Ring orderId out of range (%u)!!!\n",
                    ringPrms->orderId);
    }

#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    if((UDMA_MAPPED_GROUP_INVALID != ringPrms->mappedRingGrp) &&
       ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) <= ringPrms->mappedRingGrp))
    {
        retVal = UDMA_EINVALID_PARAMS;
        Udma_printf(drvHandle, "[Error] Incorrect Mapped Ring Group!!!\n");
    }
#endif

    return (retVal);
}

static inline void Udma_ringAssertFnPointers(Udma_DrvHandle drvHandle)
{
    Udma_assert(drvHandle, drvHandle->ringDequeueRaw        != (Udma_ringDequeueRawFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringQueueRaw          != (Udma_ringQueueRawFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringFlushRaw          != (Udma_ringFlushRawFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringGetElementCnt     != (Udma_ringGetElementCntFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringGetMemPtr         != (Udma_ringGetMemPtrFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringGetMode           != (Udma_ringGetModeFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringGetForwardRingOcc != (Udma_ringGetForwardRingOccFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringGetReverseRingOcc != (Udma_ringGetReverseRingOccFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringGetWrIdx          != (Udma_ringGetWrIdxFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringGetRdIdx          != (Udma_ringGetRdIdxFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringPrime             != (Udma_ringPrimeFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringPrimeRead         != (Udma_ringPrimeReadFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringSetDoorBell       != (Udma_ringSetDoorBellFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringSetCfg            != (Udma_ringSetCfgFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->ringHandleClearRegs   != (Udma_ringHandleClearRegsFxn) NULL_PTR);

    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.disableAllIntr != (Udma_OsalDisableAllIntrFxn) NULL_PTR);
    Udma_assert(drvHandle, drvHandle->initPrms.osalPrms.restoreAllIntr != (Udma_OsalRestoreAllIntrFxn) NULL_PTR);
}

#if (UDMA_SOC_CFG_APPLY_RING_WORKAROUND == 1)
int32_t Udma_ringReset(Udma_DrvHandle drvHandle,
                              Udma_RingHandle ringHandle)
{
    int32_t     retVal = UDMA_SOK;
    uint32_t    regVal;
    uint32_t    ringOcc, mode, elemSize;
    uint32_t    applyWorkaround = 0U, freeRingStart;
    uint32_t    dbRingCnt, thisDbRingCnt;
    struct tisci_msg_rm_ring_cfg_req    rmRingReq;
    struct tisci_msg_rm_ring_cfg_resp   rmRingResp;

    Udma_assert(drvHandle, (NULL_PTR != drvHandle->raRegs.pGlbRegs));
    Udma_assert(drvHandle, (NULL_PTR != ringHandle->pCfgRegs));
    Udma_assert(drvHandle, (NULL_PTR != ringHandle->pRtRegs));

    /*-------------------------------------------------------------------------
     *  ringacc revision 1.0.19 and earlier has an issue where the UDMAP ring
     *  state (ring occupancy) will be out of sync with the ringacc ring state
     *  when a ring is reset.
     *
     *  Perform the software workaround to correct this.
     *-----------------------------------------------------------------------*/
    regVal = CSL_REG32_RD(&drvHandle->raRegs.pGlbRegs->REVISION);
    if(!((1U  < CSL_FEXT(regVal, RINGACC_GCFG_REVISION_REVMAJ)) ||
         (0U  < CSL_FEXT(regVal, RINGACC_GCFG_REVISION_REVMIN)) ||
         (19U < CSL_FEXT(regVal, RINGACC_GCFG_REVISION_REVRTL))))
    {
        /* Workaround required only for FQ ring */
        freeRingStart = drvHandle->udmapRegs.txChanCnt +
                        drvHandle->udmapRegs.txExtUtcChanCnt +
                        drvHandle->udmapRegs.rxChanCnt;
        if(ringHandle->ringNum < freeRingStart)
        {
            applyWorkaround = 1U;
        }
    }

    if(1U == applyWorkaround)
    {
        /* Call SCICLIENT with dummy cfg to get read access to the ring RT
         * By default this is is firewalled even for read access */
        rmRingReq.valid_params  = 0U;
        rmRingReq.nav_id        = drvHandle->devIdRing;
        rmRingReq.index         = ringHandle->ringNum;
        /* Not used */
        rmRingReq.mode          = 0U;
        rmRingReq.size          = 0U;
        rmRingReq.addr_lo       = 0U;
        rmRingReq.addr_hi       = 0U;
        rmRingReq.count         = 0U;
        rmRingReq.order_id      = UDMA_DEFAULT_RING_ORDER_ID;
        retVal = Sciclient_rmRingCfg(
                     &rmRingReq, &rmRingResp, UDMA_SCICLIENT_TIMEOUT);
        if(CSL_PASS != retVal)
        {
            Udma_printf(drvHandle,
                "[Error] Ring config for read access failed!!!\n");
        }
    }

    if((1U == applyWorkaround) && (UDMA_SOK == retVal))
    {
        /*-------------------------------------------------------------------------
         *  1. Read the ring occupancy.
         *
         *  In the case of a credentials mode or qm mode ring, each ring write
         *  results in the ring occupancy increasing by 2 elements (one entry for
         *  the credentials, one entry for the data). However, the internal UDMAP
         *  ring state occupancy counter only records the number of writes, so we
         *  divide the ring occupancy by 2 to get back to the number of doorbell
         *  rings needed.
         *
         *  If the ring occupancy is not 0, then we need to execute the workaround.
         *-----------------------------------------------------------------------*/
        ringOcc = CSL_REG32_FEXT(&ringHandle->pRtRegs->OCC, RINGACC_RT_RINGRT_OCC_CNT);
        mode = CSL_REG32_FEXT(&ringHandle->pCfgRegs->SIZE, RINGACC_CFG_RING_SIZE_QMODE);
        elemSize = CSL_REG32_FEXT(&ringHandle->pCfgRegs->SIZE, RINGACC_CFG_RING_SIZE_ELSIZE);
        if((CSL_RINGACC_RING_MODE_CREDENTIALS == mode) ||
           (CSL_RINGACC_RING_MODE_QM == mode))
        {
            ringOcc >>= 1U;
        }
        if(0U != ringOcc)
        {
            /*---------------------------------------------------------------------
             *  2. Reset the ring by writing to size register
             *  3. At the same time setup the ring in ring/doorbell mode
             *-------------------------------------------------------------------*/
            rmRingReq.valid_params  = TISCI_MSG_VALUE_RM_RING_MODE_VALID |
                                      TISCI_MSG_VALUE_RM_RING_SIZE_VALID;
            rmRingReq.nav_id        = drvHandle->devIdRing;
            rmRingReq.index         = ringHandle->ringNum;
            rmRingReq.mode          = TISCI_MSG_VALUE_RM_RING_MODE_RING;
            rmRingReq.size          = elemSize;
            /* Not used */
            rmRingReq.addr_lo       = 0U;
            rmRingReq.addr_hi       = 0U;
            rmRingReq.count         = 0U;
            rmRingReq.order_id      = UDMA_DEFAULT_RING_ORDER_ID;
            retVal = Sciclient_rmRingCfg(
                         &rmRingReq, &rmRingResp, UDMA_SCICLIENT_TIMEOUT);
            if(CSL_PASS != retVal)
            {
                Udma_printf(drvHandle, "[Error] Ring reset failed!!!\n");
            }
            else
            {
                /*--------------------------------------------------------------
                 *  4. Ring the doorbell 2**22 - ringOcc times. This will wrap
                 *     the internal UDMAP ring state occupancy counter
                 *     (which is 21-bits wide) to 0.
                 *------------------------------------------------------------*/
                dbRingCnt = ((uint32_t)1U << 22) - ringOcc;
                thisDbRingCnt = UDMA_RING_MAX_DB_RING_CNT;
                regVal = (uint32_t)CSL_FMK(RINGACC_RT_RINGRT_DB_CNT, thisDbRingCnt);
                while(0U != dbRingCnt)
                {
                    /*----------------------------------------------------------
                     *  Ring the doorbell with the maximum count each iteration
                     *  if possible to minimize the total # of writes
                     *--------------------------------------------------------*/
                    if(dbRingCnt < UDMA_RING_MAX_DB_RING_CNT)
                    {
                        thisDbRingCnt = dbRingCnt;
                        regVal = (uint32_t)CSL_FMK(RINGACC_RT_RINGRT_DB_CNT, thisDbRingCnt);
                    }
                    CSL_REG32_WR(&ringHandle->pRtRegs->DB, regVal);
                    dbRingCnt -= thisDbRingCnt;
                }

                /*--------------------------------------------------------------
                 *  5. Restore the original ring mode (if not ring mode)
                 *------------------------------------------------------------*/
                /* Not done as we will reconfigure the ring after reset step */
            }
        }
    }

    return (retVal);
}
#endif
