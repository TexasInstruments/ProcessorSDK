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

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Udma_ringSetCfgNormal(Udma_DrvHandle drvHandle,
                           Udma_RingHandle ringHandle,
                           const Udma_RingPrms *ringPrms)
{
    uint32_t            addrHi, addrLo, elemSize;
    CSL_RingAccRingCfg *ringCfg;

    /* Configure ring object */
    Udma_assert(drvHandle, (NULL_PTR != drvHandle->raRegs.pCfgRegs));
    Udma_assert(drvHandle, (NULL_PTR != drvHandle->raRegs.pRtRegs));
    Udma_assert(drvHandle, (ringHandle->ringNum < 1024U));
    ringHandle->pCfgRegs =
        &drvHandle->raRegs.pCfgRegs->RING[ringHandle->ringNum];
    ringHandle->pRtRegs         =
        &drvHandle->raRegs.pRtRegs->RINGRT[ringHandle->ringNum];

    ringCfg = &ringHandle->cfg;
    if(NULL_PTR != ringPrms)
    {
        ringCfg->physBase    =
            Udma_virtToPhyFxn(ringPrms->ringMem, drvHandle, (Udma_ChHandle) NULL_PTR);
        ringCfg->virtBase    = (void *) ringPrms->ringMem;
        ringCfg->mode        = ringPrms->mode;
        ringCfg->elCnt       = ringPrms->elemCnt;
        /* CSL expects ring size in bytes */
        ringCfg->elSz        = ((uint32_t) 1U << (ringPrms->elemSize + 2U));
    }
    else
    {
        /* Init CSL ring object */
        addrHi = CSL_REG32_FEXT(&ringHandle->pCfgRegs->BA_HI, RINGACC_CFG_RING_BA_HI_ADDR_HI);
        addrLo = CSL_REG32_FEXT(&ringHandle->pCfgRegs->BA_LO, RINGACC_CFG_RING_BA_LO_ADDR_LO);
        ringCfg->physBase    = (uint64_t)((((uint64_t) addrHi) << 32UL) |
                                            ((uint64_t) addrLo));
        ringCfg->virtBase    = Udma_phyToVirtFxn(ringCfg->physBase, drvHandle, (Udma_ChHandle) NULL_PTR);
        ringCfg->mode        = CSL_REG32_FEXT(&ringHandle->pCfgRegs->SIZE, RINGACC_CFG_RING_SIZE_QMODE);
        ringCfg->elCnt       = CSL_REG32_FEXT(&ringHandle->pCfgRegs->SIZE, RINGACC_CFG_RING_SIZE_ELCNT);
        elemSize             = CSL_REG32_FEXT(&ringHandle->pCfgRegs->SIZE, RINGACC_CFG_RING_SIZE_ELSIZE);
        /* CSL expects ring size in bytes */
        ringCfg->elSz        = ((uint32_t) 1U << (elemSize + 2U));
    }
    
    ringCfg->evtNum      = UDMA_EVENT_INVALID;
    ringCfg->credSecure  = 0U;
    ringCfg->credPriv    = 0U;
    ringCfg->credPrivId  = CSL_RINGACC_CRED_PASSTHRU;
    ringCfg->credVirtId  = CSL_RINGACC_CRED_PASSTHRU;
    CSL_ringaccInitRingObj(ringHandle->ringNum, ringCfg);

#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
    ringHandle->proxyAddr =
        CSL_proxyGetDataAddr(
            &drvHandle->proxyCfg,
            drvHandle->proxyTargetNumRing,
            drvHandle->initPrms.rmInitPrms.proxyThreadNum,
            ringCfg->elSz);
#endif

    return;
}

void Udma_ringHandleClearRegsNormal(Udma_RingHandle ringHandle)
{
    ringHandle->pCfgRegs        = (volatile CSL_ringacc_cfgRegs_RING *) NULL_PTR;
    ringHandle->pRtRegs         = (volatile CSL_ringacc_rtRegs_RINGRT *) NULL_PTR;
}

int32_t Udma_ringQueueRawNormal(Udma_DrvHandle  drvHandle, Udma_RingHandle ringHandle, uint64_t phyDescMem)
{
    int32_t         retVal = UDMA_SOK;

    if(TISCI_MSG_VALUE_RM_RING_MODE_RING == ringHandle->cfg.mode)
    {
        /* Use direct memory access for RING mode */
        retVal = CSL_ringaccPush64(
            &ringHandle->drvHandle->raRegs,
            &ringHandle->cfg,
            (uint64_t) phyDescMem,
            &Udma_ringaccMemOps);
    }
    else
    {
        /* Use proxy for other modes */
        retVal = Udma_ringProxyQueueRaw(ringHandle, drvHandle, phyDescMem);
    }

    return (retVal);
}

int32_t Udma_ringDequeueRawNormal(Udma_DrvHandle drvHandle, Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    int32_t         retVal = UDMA_SOK, cslRetVal;

    if(TISCI_MSG_VALUE_RM_RING_MODE_RING == ringHandle->cfg.mode)
    {
        /* Use direct memory access for RING mode */
        cslRetVal = CSL_ringaccPop64(
            &ringHandle->drvHandle->raRegs,
            &ringHandle->cfg,
            phyDescMem,
            &Udma_ringaccMemOps);
        if(CSL_PASS != cslRetVal)
        {
            retVal = UDMA_ETIMEOUT;
        }
    }
    else
    {
        /* Use proxy for other modes */
        retVal = Udma_ringProxyDequeueRaw(ringHandle, drvHandle, phyDescMem);
    }

    return (retVal);
}

int32_t Udma_ringFlushRawNormal(Udma_DrvHandle drvHandle, Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    int32_t         retVal = UDMA_SOK;

    /* Same as proxy dequeue API as proxy can be used in all modes of ring */
    retVal = Udma_ringProxyDequeueRaw(ringHandle, drvHandle, phyDescMem);

    return (retVal);
}

void Udma_ringPrimeNormal(Udma_RingHandle ringHandle, uint64_t phyDescMem)
{
    volatile uint64_t        *ringPtr;
    CSL_RingAccRingCfg       *pRing;
    uintptr_t                 tempPtr;

    pRing = &ringHandle->cfg;
    tempPtr = (((uintptr_t)pRing->rwIdx * (uintptr_t)pRing->elSz) +
              (uintptr_t)pRing->virtBase);
    ringPtr = (volatile uint64_t *)(tempPtr);
    *ringPtr = phyDescMem;

    /* Book keeping */
    pRing->waiting++;
    pRing->rwIdx++;
    if(pRing->rwIdx >= pRing->elCnt)
    {
        pRing->rwIdx = 0U;
    }
    pRing->occ++;

    return;
}

void Udma_ringPrimeReadNormal(Udma_RingHandle ringHandle, uint64_t *phyDescMem)
{
    volatile uint64_t        *ringPtr;
    CSL_RingAccRingCfg       *pRing;
    uintptr_t                 tempPtr;

    pRing = &ringHandle->cfg;
    tempPtr = (((uintptr_t)pRing->rwIdx * (uintptr_t)pRing->elSz) +
              (uintptr_t)pRing->virtBase);
    ringPtr = (volatile uint64_t *)(tempPtr);
    *phyDescMem = *ringPtr;

    if (0U != *phyDescMem)
    {
        /* Book keeping */
        pRing->waiting++;
        pRing->rwIdx++;
        if(pRing->rwIdx >= pRing->elCnt)
        {
            pRing->rwIdx = 0U;
        }
        pRing->occ--;
    }
}

void Udma_ringSetDoorBellNormal(Udma_RingHandle ringHandle, int32_t count)
{
    uint32_t    regVal;
    int32_t     thisDbRingCnt;
    int32_t     dbRingCnt = count;
    CSL_RingAccRingCfg       *pRing;

    pRing = &ringHandle->cfg;

    /* count will be positive when ring elements are queued into the ring */
    if (0 <= dbRingCnt)
    {
        while(0 != dbRingCnt)
        {
            if(dbRingCnt < (int32_t)UDMA_RING_MAX_DB_RING_CNT)
            {
                thisDbRingCnt = dbRingCnt;
                regVal = (uint32_t)CSL_FMK(RINGACC_RT_RINGRT_DB_CNT, (uint32_t)thisDbRingCnt);
            }
            else
            {
                thisDbRingCnt = (int32_t)UDMA_RING_MAX_DB_RING_CNT;
                regVal = (uint32_t)CSL_FMK(RINGACC_RT_RINGRT_DB_CNT, (uint32_t)thisDbRingCnt);
            }
            CSL_REG32_WR(&ringHandle->pRtRegs->DB, regVal);
            pRing->waiting -= thisDbRingCnt;
            dbRingCnt -= thisDbRingCnt;
        }
    }

    /* count will be negative when ring elements are dequeued from the ring */
    else
    {
        while(0 != dbRingCnt)
        {
            if(dbRingCnt > (-1 * (int32_t)UDMA_RING_MAX_DB_RING_CNT))
            {
                thisDbRingCnt = dbRingCnt;
                regVal = (uint32_t)CSL_FMK(RINGACC_RT_RINGRT_DB_CNT, (uint32_t)thisDbRingCnt);
            }
            else
            {
                thisDbRingCnt = (-1 * (int32_t)UDMA_RING_MAX_DB_RING_CNT);
                regVal = (uint32_t)CSL_FMK(RINGACC_RT_RINGRT_DB_CNT, (uint32_t)thisDbRingCnt);
            }
            CSL_REG32_WR(&ringHandle->pRtRegs->DB, regVal);
            pRing->waiting += thisDbRingCnt;
            dbRingCnt -= thisDbRingCnt;
        }
    }
}

void *Udma_ringGetMemPtrNormal(Udma_RingHandle ringHandle)
{
    void   *ringMem = NULL_PTR;

    if((NULL_PTR != ringHandle) && (UDMA_INIT_DONE == ringHandle->ringInitDone))
    {
        ringMem = ringHandle->cfg.virtBase;
    }

    return (ringMem);
}
uint32_t Udma_ringGetModeNormal(Udma_RingHandle ringHandle)
{
    uint32_t ringMode = CSL_RINGACC_RING_MODE_INVALID;

    if((NULL_PTR != ringHandle) && (UDMA_INIT_DONE == ringHandle->ringInitDone))
    {
        ringMode = ringHandle->cfg.mode;
    }

    return (ringMode);
}

uint32_t Udma_ringGetElementCntNormal(Udma_RingHandle ringHandle)
{
    uint32_t size = 0U;

    if((NULL_PTR != ringHandle) && (UDMA_INIT_DONE == ringHandle->ringInitDone))
    {
        size = ringHandle->cfg.elCnt;
    }

    return (size);
}

uint32_t Udma_ringGetRingOccNormal(Udma_RingHandle ringHandle)
{
    uint32_t occ = 0U;

    if((NULL_PTR != ringHandle) && (UDMA_INIT_DONE == ringHandle->ringInitDone))
    {
        occ = CSL_ringaccGetRingHwOcc(&ringHandle->drvHandle->raRegs,
                                      ringHandle->ringNum);
        /* Update cfg->occ */  
        ringHandle->cfg.occ = occ;                      
    }

    return (occ);
}

uint32_t Udma_ringGetWrIdxNormal(Udma_RingHandle ringHandle)
{
    uint32_t idx = 0U;

    if((NULL_PTR != ringHandle) && (UDMA_INIT_DONE == ringHandle->ringInitDone))
    {
        idx = ringHandle->cfg.rwIdx;
    }

    return (idx);
}

uint32_t Udma_ringGetRdIdxNormal(Udma_RingHandle ringHandle)
{
    uint32_t idx = 0U;

    if((NULL_PTR != ringHandle) && (UDMA_INIT_DONE == ringHandle->ringInitDone))
    {
        idx = ringHandle->cfg.rwIdx;
    }

    return (idx);
}
