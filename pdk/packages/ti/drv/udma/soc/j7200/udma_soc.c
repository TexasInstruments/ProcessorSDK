/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2025
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
 *  \file udma_soc.c
 *
 *  \brief File containing the UDMA driver SOC related configuration functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/src/udma_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

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

#define UDMA_MCU_NAVSS0_UDMASS_UDMAP0_CFG_GCFG_BASE             (CSL_MCU_NAVSS0_UDMASS_UDMAP0_CFG_GCFG_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW_BASE            (CSL_MCU_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_UDMAP0_TCHAN_BASE                (CSL_MCU_NAVSS0_UDMASS_UDMAP0_TCHAN_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_UDMAP0_RCHAN_BASE                (CSL_MCU_NAVSS0_UDMASS_UDMAP0_RCHAN_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_UDMAP_TCHANRT_BASE               (CSL_MCU_NAVSS0_UDMASS_UDMAP_TCHANRT_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_UDMAP_RCHANRT_BASE               (CSL_MCU_NAVSS0_UDMASS_UDMAP_RCHANRT_BASE)

#define UDMA_NAVSS0_UDMASS_UDMAP0_CFG_BASE                      (CSL_NAVSS0_UDMASS_UDMAP0_CFG_BASE)
#define UDMA_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW_BASE                (CSL_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW_BASE)
#define UDMA_NAVSS0_UDMASS_UDMAP0_CFG_TCHAN_BASE                (CSL_NAVSS0_UDMASS_UDMAP0_CFG_TCHAN_BASE)
#define UDMA_NAVSS0_UDMASS_UDMAP0_CFG_RCHAN_BASE                (CSL_NAVSS0_UDMASS_UDMAP0_CFG_RCHAN_BASE)
#define UDMA_NAVSS0_UDMASS_UDMAP0_CFG_TCHANRT_BASE              (CSL_NAVSS0_UDMASS_UDMAP0_CFG_TCHANRT_BASE)
#define UDMA_NAVSS0_UDMASS_UDMAP0_CFG_RCHANRT_BASE              (CSL_NAVSS0_UDMASS_UDMAP0_CFG_RCHANRT_BASE)

#define UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_GCFG_BASE           (CSL_MCU_NAVSS0_UDMASS_RINGACC0_CFG_GCFG_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_BASE                (CSL_MCU_NAVSS0_UDMASS_RINGACC0_CFG_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_RT_BASE             (CSL_MCU_NAVSS0_UDMASS_RINGACC0_CFG_RT_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_MON_BASE            (CSL_MCU_NAVSS0_UDMASS_RINGACC0_CFG_MON_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_RINGACC0_FIFOS_BASE              (CSL_MCU_NAVSS0_UDMASS_RINGACC0_FIFOS_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_RINGACC0_ISC_ISC_BASE            (CSL_MCU_NAVSS0_UDMASS_RINGACC0_ISC_BASE)

#define UDMA_NAVSS0_UDMASS_RINGACC0_GCFG_BASE                   (CSL_NAVSS0_UDMASS_RINGACC0_GCFG_BASE)
#define UDMA_NAVSS0_UDMASS_RINGACC0_CFG_BASE                    (CSL_NAVSS0_UDMASS_RINGACC0_CFG_BASE)
#define UDMA_NAVSS0_UDMASS_RINGACC0_CFG_RT_BASE                 (CSL_NAVSS0_UDMASS_RINGACC0_CFG_RT_BASE)
#define UDMA_NAVSS0_UDMASS_RINGACC0_CFG_MON_BASE                (CSL_NAVSS0_UDMASS_RINGACC0_CFG_MON_BASE)
#define UDMA_NAVSS0_UDMASS_RINGACC0_SRC_FIFOS_BASE              (CSL_NAVSS0_UDMASS_RINGACC0_SRC_FIFOS_BASE)
#define UDMA_NAVSS0_UDMASS_RINGACC0_ISC_ISC_BASE                (CSL_NAVSS0_UDMASS_RINGACC0_ISC_BASE)

#define UDMA_MCU_NAVSS0_UDMASS_PSILSS_CFG0_PROXY_BASE           (CSL_MCU_NAVSS0_UDMASS_PSILSS_CFG0_PROXY_BASE)
#define UDMA_NAVSS0_UDMASS_PSILCFG0_CFG_PROXY_BASE              (CSL_NAVSS0_UDMASS_PSILCFG0_CFG_PROXY_BASE)

#define UDMA_MCU_NAVSS0_UDMASS_INTA0_CFG_BASE                   (CSL_MCU_NAVSS0_UDMASS_INTA0_CFG_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_INTA0_IMAP_BASE                  (CSL_MCU_NAVSS0_UDMASS_INTA0_IMAP_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_INTA0_INTR_BASE                  (CSL_MCU_NAVSS0_UDMASS_INTA0_INTR_BASE)
#define UDMA_MCU_NAVSS0_PAR_UDMASS_UDMASS_INTA0_CFG_L2G_BASE    (CSL_MCU_NAVSS0_UDMASS_INTA0_I2G_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_INTA0_MCAST_BASE                 (CSL_MCU_NAVSS0_UDMASS_INTA0_MCAST_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_INTA0_GCNT_BASE                  (CSL_MCU_NAVSS0_UDMASS_INTA0_GCNT_BASE)
#define UDMA_MCU_NAVSS0_UDMASS_INTA0_GCNTRTI_BASE               (CSL_MCU_NAVSS0_UDMASS_INTA0_GCNTRTI_BASE)

#define UDMA_MCU_NAVSS0_INTR0_CFG_BASE                          (CSL_MCU_NAVSS0_INTR0_CFG_BASE)

#define UDMA_NAVSS0_UDMASS_INTA0_CFG_BASE                       (CSL_NAVSS0_UDMASS_INTA0_CFG_BASE)
#define UDMA_NAVSS0_UDMASS_INTA0_IMAP_BASE                      (CSL_NAVSS0_UDMASS_INTA0_IMAP_BASE)
#define UDMA_NAVSS0_UDMASS_INTA0_CFG_INTR_BASE                  (CSL_NAVSS0_UDMASS_INTA0_CFG_INTR_BASE)
#define UDMA_NAVSS0_UDMASS_INTA0_CFG_L2G_BASE                   (CSL_NAVSS0_UDMASS_INTA0_CFG_L2G_BASE)
#define UDMA_NAVSS0_UDMASS_INTA0_CFG_MCAST_BASE                 (CSL_NAVSS0_UDMASS_INTA0_CFG_MCAST_BASE)
#define UDMA_NAVSS0_UDMASS_INTA0_CFG_GCNTCFG_BASE               (CSL_NAVSS0_UDMASS_INTA0_CFG_GCNTCFG_BASE)
#define UDMA_NAVSS0_UDMASS_INTA0_CFG_GCNTRTI_BASE               (CSL_NAVSS0_UDMASS_INTA0_CFG_GCNTRTI_BASE)

#define UDMA_NAVSS0_INTR0_INTR_ROUTER_CFG_BASE                  (CSL_NAVSS0_INTR0_INTR_ROUTER_CFG_BASE)

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Udma_initDrvHandle(Udma_DrvHandle drvHandle)
{
    uint32_t            instId;
    CSL_UdmapCfg       *pUdmapRegs;
    CSL_RingAccCfg     *pRaRegs;
    CSL_IntaggrCfg     *pIaRegs;
    CSL_ProxyCfg       *pProxyCfg;
    CSL_ProxyTargetParams *pProxyTargetRing;

    instId = drvHandle->initPrms.instId;
    
    drvHandle->instType = UDMA_INST_TYPE_NORMAL;
    
    /*
     * UDMA config init
     */
    /* Init the config structure - one time step */
    pUdmapRegs = &drvHandle->udmapRegs;
    if(UDMA_INST_ID_MCU_0 == instId)
    {
        pUdmapRegs->pGenCfgRegs     = ((CSL_udmap_gcfgRegs *) UDMA_MCU_NAVSS0_UDMASS_UDMAP0_CFG_GCFG_BASE);
        pUdmapRegs->pRxFlowCfgRegs  = ((CSL_udmap_rxfcfgRegs *) UDMA_MCU_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW_BASE);
        pUdmapRegs->pTxChanCfgRegs  = ((CSL_udmap_txccfgRegs *) UDMA_MCU_NAVSS0_UDMASS_UDMAP0_TCHAN_BASE);
        pUdmapRegs->pRxChanCfgRegs  = ((CSL_udmap_rxccfgRegs *) UDMA_MCU_NAVSS0_UDMASS_UDMAP0_RCHAN_BASE);
        pUdmapRegs->pTxChanRtRegs   = ((CSL_udmap_txcrtRegs *) UDMA_MCU_NAVSS0_UDMASS_UDMAP_TCHANRT_BASE);
        pUdmapRegs->pRxChanRtRegs   = ((CSL_udmap_rxcrtRegs *) UDMA_MCU_NAVSS0_UDMASS_UDMAP_RCHANRT_BASE);
        drvHandle->trigGemOffset    = CSL_NAVSS_GEM_MCU_UDMA_TRIGGER_OFFSET;
    }
    else
    {
        pUdmapRegs->pGenCfgRegs     = ((CSL_udmap_gcfgRegs *) UDMA_NAVSS0_UDMASS_UDMAP0_CFG_BASE);
        pUdmapRegs->pRxFlowCfgRegs  = ((CSL_udmap_rxfcfgRegs *) UDMA_NAVSS0_UDMASS_UDMAP0_CFG_RFLOW_BASE);
        pUdmapRegs->pTxChanCfgRegs  = ((CSL_udmap_txccfgRegs *) UDMA_NAVSS0_UDMASS_UDMAP0_CFG_TCHAN_BASE);
        pUdmapRegs->pRxChanCfgRegs  = ((CSL_udmap_rxccfgRegs *) UDMA_NAVSS0_UDMASS_UDMAP0_CFG_RCHAN_BASE);
        pUdmapRegs->pTxChanRtRegs   = ((CSL_udmap_txcrtRegs *) UDMA_NAVSS0_UDMASS_UDMAP0_CFG_TCHANRT_BASE);
        pUdmapRegs->pRxChanRtRegs   = ((CSL_udmap_rxcrtRegs *) UDMA_NAVSS0_UDMASS_UDMAP0_CFG_RCHANRT_BASE);
        drvHandle->trigGemOffset    = CSL_NAVSS_GEM_MAIN_UDMA_TRIGGER_OFFSET;
    }
    /* Fill other SOC specific parameters by reading from UDMA config
     * registers */
    CSL_udmapGetCfg(pUdmapRegs);

    /*
     * RA config init
     */
    drvHandle->raType = UDMA_RA_TYPE_NORMAL;
    pRaRegs = &drvHandle->raRegs;
    if(UDMA_INST_ID_MCU_0 == instId)
    {
        pRaRegs->pGlbRegs   = (CSL_ringacc_gcfgRegs *) UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_GCFG_BASE;
        pRaRegs->pCfgRegs   = (CSL_ringacc_cfgRegs *) UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_BASE;
        pRaRegs->pRtRegs    = (CSL_ringacc_rtRegs *) UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_RT_BASE;
        pRaRegs->pMonRegs   = (CSL_ringacc_monitorRegs *) UDMA_MCU_NAVSS0_UDMASS_RINGACC0_CFG_MON_BASE;
        pRaRegs->pFifoRegs  = (CSL_ringacc_fifosRegs *) UDMA_MCU_NAVSS0_UDMASS_RINGACC0_FIFOS_BASE;
        pRaRegs->pIscRegs   = (CSL_ringacc_iscRegs *) UDMA_MCU_NAVSS0_UDMASS_RINGACC0_ISC_ISC_BASE;
        pRaRegs->maxRings   = CSL_NAVSS_MCU_RINGACC_RING_CNT;
    }
    else
    {
        pRaRegs->pGlbRegs   = (CSL_ringacc_gcfgRegs *) UDMA_NAVSS0_UDMASS_RINGACC0_GCFG_BASE;
        pRaRegs->pCfgRegs   = (CSL_ringacc_cfgRegs *) UDMA_NAVSS0_UDMASS_RINGACC0_CFG_BASE;
        pRaRegs->pRtRegs    = (CSL_ringacc_rtRegs *) UDMA_NAVSS0_UDMASS_RINGACC0_CFG_RT_BASE;
        pRaRegs->pMonRegs   = (CSL_ringacc_monitorRegs *) UDMA_NAVSS0_UDMASS_RINGACC0_CFG_MON_BASE;
        pRaRegs->pFifoRegs  = (CSL_ringacc_fifosRegs *) UDMA_NAVSS0_UDMASS_RINGACC0_SRC_FIFOS_BASE;
        pRaRegs->pIscRegs   = (CSL_ringacc_iscRegs *) UDMA_NAVSS0_UDMASS_RINGACC0_ISC_ISC_BASE;
        pRaRegs->maxRings   = CSL_NAVSS_MAIN_RINGACC_RING_CNT;
    }
    pRaRegs->maxMonitors     = CSL_RINGACC_MAX_MONITORS;
    pRaRegs->bTraceSupported = BTRUE;

    drvHandle->ringDequeueRaw           = &Udma_ringDequeueRawNormal;
    drvHandle->ringQueueRaw             = &Udma_ringQueueRawNormal;
    drvHandle->ringFlushRaw             = &Udma_ringFlushRawNormal;
    drvHandle->ringGetElementCnt        = &Udma_ringGetElementCntNormal;
    drvHandle->ringGetMemPtr            = &Udma_ringGetMemPtrNormal;
    drvHandle->ringGetMode              = &Udma_ringGetModeNormal;
    drvHandle->ringGetForwardRingOcc    = &Udma_ringGetRingOccNormal;
    drvHandle->ringGetReverseRingOcc    = &Udma_ringGetRingOccNormal;
    drvHandle->ringGetWrIdx             = &Udma_ringGetWrIdxNormal;
    drvHandle->ringGetRdIdx             = &Udma_ringGetRdIdxNormal;
    drvHandle->ringPrime                = &Udma_ringPrimeNormal;
    drvHandle->ringPrimeRead            = &Udma_ringPrimeReadNormal;
    drvHandle->ringSetDoorBell          = &Udma_ringSetDoorBellNormal;
    drvHandle->ringSetCfg               = &Udma_ringSetCfgNormal;
    drvHandle->ringHandleClearRegs      = &Udma_ringHandleClearRegsNormal;

    /*
     * All interrupt related config should be based on core and not
     * based on NAVSS instance
     */
#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);

    /* If MCU_1_0 or MCU_1_1 execute these */
    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* IA config init */
        pIaRegs = &drvHandle->iaRegs;
        pIaRegs->pCfgRegs       = (CSL_intaggr_cfgRegs *) UDMA_MCU_NAVSS0_UDMASS_INTA0_CFG_BASE;
        pIaRegs->pImapRegs      = (CSL_intaggr_imapRegs *) UDMA_MCU_NAVSS0_UDMASS_INTA0_IMAP_BASE;
        pIaRegs->pIntrRegs      = (CSL_intaggr_intrRegs *) UDMA_MCU_NAVSS0_UDMASS_INTA0_INTR_BASE;
        pIaRegs->pL2gRegs       = (CSL_intaggr_l2gRegs *) UDMA_MCU_NAVSS0_PAR_UDMASS_UDMASS_INTA0_CFG_L2G_BASE;
        pIaRegs->pMcastRegs     = (CSL_intaggr_mcastRegs *) UDMA_MCU_NAVSS0_UDMASS_INTA0_MCAST_BASE;
        pIaRegs->pGcntCfgRegs   = (CSL_intaggr_gcntcfgRegs *) UDMA_MCU_NAVSS0_UDMASS_INTA0_GCNT_BASE;
        pIaRegs->pGcntRtiRegs   = (CSL_intaggr_gcntrtiRegs *) UDMA_MCU_NAVSS0_UDMASS_INTA0_GCNTRTI_BASE;
        CSL_intaggrGetCfg(pIaRegs);

        drvHandle->devIdIa      = TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0;
        drvHandle->devIdIr      = TISCI_DEV_MCU_NAVSS0_INTR_0;
    }
    else
    {
        /* IA config init */
        pIaRegs = &drvHandle->iaRegs;
        pIaRegs->pCfgRegs       = (CSL_intaggr_cfgRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_BASE;
        pIaRegs->pImapRegs      = (CSL_intaggr_imapRegs *) UDMA_NAVSS0_UDMASS_INTA0_IMAP_BASE;
        pIaRegs->pIntrRegs      = (CSL_intaggr_intrRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_INTR_BASE;
        pIaRegs->pL2gRegs       = (CSL_intaggr_l2gRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_L2G_BASE;
        pIaRegs->pMcastRegs     = (CSL_intaggr_mcastRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_MCAST_BASE;
        pIaRegs->pGcntCfgRegs   = (CSL_intaggr_gcntcfgRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_GCNTCFG_BASE;
        pIaRegs->pGcntRtiRegs   = (CSL_intaggr_gcntrtiRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_GCNTRTI_BASE;
        CSL_intaggrGetCfg(pIaRegs);

        drvHandle->devIdIa      = TISCI_DEV_NAVSS0_UDMASS_INTA_0;
        drvHandle->devIdIr      = TISCI_DEV_NAVSS0_INTR_ROUTER_0;
    #if (UDMA_SOC_CFG_CLEC_PRESENT == 1)
        drvHandle->clecRtMap    = CSL_CLEC_RTMAP_DISABLE;
        drvHandle->clecOffset   = 0U;
    #endif
    }
#else
    /* IA config init */
    pIaRegs = &drvHandle->iaRegs;
    pIaRegs->pCfgRegs       = (CSL_intaggr_cfgRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_BASE;
    pIaRegs->pImapRegs      = (CSL_intaggr_imapRegs *) UDMA_NAVSS0_UDMASS_INTA0_IMAP_BASE;
    pIaRegs->pIntrRegs      = (CSL_intaggr_intrRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_INTR_BASE;
    pIaRegs->pL2gRegs       = (CSL_intaggr_l2gRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_L2G_BASE;
    pIaRegs->pMcastRegs     = (CSL_intaggr_mcastRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_MCAST_BASE;
    pIaRegs->pGcntCfgRegs   = (CSL_intaggr_gcntcfgRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_GCNTCFG_BASE;
    pIaRegs->pGcntRtiRegs   = (CSL_intaggr_gcntrtiRegs *) UDMA_NAVSS0_UDMASS_INTA0_CFG_GCNTRTI_BASE;
    CSL_intaggrGetCfg(pIaRegs);

    drvHandle->devIdIa      = TISCI_DEV_NAVSS0_UDMASS_INTA_0;
    drvHandle->devIdIr      = TISCI_DEV_NAVSS0_INTR_ROUTER_0;
#if (UDMA_SOC_CFG_CLEC_PRESENT == 1)
    drvHandle->clecRtMap    = CSL_CLEC_RTMAP_DISABLE;
    drvHandle->clecOffset   = 0U;
#endif
#endif

    drvHandle->devIdCore    = Udma_getCoreSciDevId();
    drvHandle->iaGemOffset  = Udma_getGlobalEventOffset();
    
    /*
     * Proxy init
     */
    pProxyCfg           = &drvHandle->proxyCfg;
    pProxyTargetRing    = &drvHandle->proxyTargetRing;
    if(UDMA_INST_ID_MCU_0 == instId)
    {
        pProxyTargetRing->pTargetRegs   = (CSL_proxy_target0Regs *) CSL_MCU_NAVSS0_PROXY0_TARGET0_DATA_BASE;
        pProxyTargetRing->numChns       = CSL_NAVSS_MCU_PROXY_TARGET_RINGACC0_NUM_CHANNELS;
        pProxyTargetRing->chnSizeBytes  = CSL_NAVSS_MCU_PROXY_TARGET_RINGACC0_NUM_CHANNEL_SIZE_BYTES;

        pProxyCfg->pGlbRegs             = (CSL_proxyRegs *) CSL_MCU_NAVSS0_PROXY_CFG_GCFG_BASE;
        pProxyCfg->pCfgRegs             = (CSL_proxy_cfgRegs *) CSL_MCU_NAVSS0_PROXY0_BUF_CFG_BASE;
        pProxyCfg->bufferSizeBytes      = CSL_NAVSS_MCU_PROXY_BUFFER_SIZE_BYTES;
        pProxyCfg->numTargets           = 1U;
        pProxyCfg->pProxyTargetParams   = pProxyTargetRing;

        drvHandle->proxyTargetNumRing   = CSL_NAVSS_MCU_PROXY_TARGET_NUM_RINGACC0;
    }
    else
    {
        pProxyTargetRing->pTargetRegs   = (CSL_proxy_target0Regs *) CSL_NAVSS0_PROXY_TARGET0_DATA_BASE;
        pProxyTargetRing->numChns       = CSL_NAVSS_MAIN_PROXY_TARGET_RINGACC0_NUM_CHANNELS;
        pProxyTargetRing->chnSizeBytes  = CSL_NAVSS_MAIN_PROXY_TARGET_RINGACC0_NUM_CHANNEL_SIZE_BYTES;

        pProxyCfg->pGlbRegs             = (CSL_proxyRegs *) CSL_NAVSS0_PROXY0_CFG_BUF_CFG_BASE;
        pProxyCfg->pCfgRegs             = (CSL_proxy_cfgRegs *) CSL_NAVSS0_PROXY0_BUF_CFG_BASE;
        pProxyCfg->bufferSizeBytes      = CSL_NAVSS_MAIN_PROXY_BUFFER_SIZE_BYTES;
        pProxyCfg->numTargets           = 1U;
        pProxyCfg->pProxyTargetParams   = pProxyTargetRing;

        drvHandle->proxyTargetNumRing   = CSL_NAVSS_MAIN_PROXY_TARGET_NUM_RINGACC0;
    }

    /* Init other variables */
    if(UDMA_INST_ID_MCU_0 == instId)
    {
        drvHandle->udmapSrcThreadOffset = CSL_PSILCFG_NAVSS_MCU_UDMAP0_TSTRM_THREAD_OFFSET;
        drvHandle->udmapDestThreadOffset= CSL_PSILCFG_NAVSS_MCU_UDMAP0_RSTRM_THREAD_OFFSET;
        drvHandle->maxRings             = CSL_NAVSS_MCU_RINGACC_RING_CNT;
        drvHandle->maxProxy             = CSL_NAVSS_MCU_PROXY_NUM_PROXIES;
        drvHandle->maxRingMon           = CSL_NAVSS_MCU_RINGACC_NUM_MONITORS;
        drvHandle->devIdRing            = TISCI_DEV_MCU_NAVSS0_RINGACC0;
        drvHandle->devIdProxy           = TISCI_DEV_MCU_NAVSS0_PROXY0;
        drvHandle->devIdUdma            = TISCI_DEV_MCU_NAVSS0_UDMAP_0;
        drvHandle->devIdPsil            = TISCI_DEV_MCU_NAVSS0;
    }
    else
    {
        drvHandle->udmapSrcThreadOffset = CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TSTRM_THREAD_OFFSET;
        drvHandle->udmapDestThreadOffset= CSL_PSILCFG_NAVSS_MAIN_UDMAP0_RSTRM_THREAD_OFFSET;
        drvHandle->maxRings             = CSL_NAVSS_MAIN_RINGACC_RING_CNT;
        drvHandle->maxProxy             = CSL_NAVSS_MAIN_PROXY_NUM_PROXIES;
        drvHandle->maxRingMon           = CSL_NAVSS_MAIN_RINGACC_NUM_MONITORS;
        drvHandle->devIdRing            = TISCI_DEV_NAVSS0_RINGACC_0;
        drvHandle->devIdProxy           = TISCI_DEV_NAVSS0_PROXY_0;
        drvHandle->devIdUdma            = TISCI_DEV_NAVSS0_UDMAP_0;
        drvHandle->devIdPsil            = TISCI_DEV_NAVSS0;
    }
    /* `TISCI_DEV_NAVSS0_TIMERMGR_0` is a part of IA Event Sources and does not contain any OES registers */
    drvHandle->devIdDummyEvtSrc      = TISCI_DEV_NAVSS0_TIMERMGR_0;
    drvHandle->srcIdRingIrq          = drvHandle->devIdRing;
    drvHandle->blkCopyRingIrqOffset  = TISCI_RINGACC0_OES_IRQ_SRC_IDX_START; 
    drvHandle->txRingIrqOffset       = TISCI_RINGACC0_OES_IRQ_SRC_IDX_START;
    drvHandle->rxRingIrqOffset       = TISCI_RINGACC0_OES_IRQ_SRC_IDX_START;
    drvHandle->srcIdTrIrq            = drvHandle->devIdUdma;
    drvHandle->blkCopyTrIrqOffset    = TISCI_UDMAP0_RX_OES_IRQ_SRC_IDX_START; 
    drvHandle->txTrIrqOffset         = TISCI_UDMAP0_TX_OES_IRQ_SRC_IDX_START;
    drvHandle->rxTrIrqOffset         = TISCI_UDMAP0_RX_OES_IRQ_SRC_IDX_START;
    drvHandle->txChOffset            = 0U;
    drvHandle->extChOffset           = drvHandle->txChOffset + pUdmapRegs->txChanCnt;
    drvHandle->rxChOffset            =
        drvHandle->extChOffset + pUdmapRegs->txExtUtcChanCnt;

    return;
}

uint32_t Udma_getGlobalEventOffset(void)
{
    uint32_t globalEventOffset = 0U;

    /* Global Events(SEVI) Offset - Tied to cores and not based on NAVSS instance */
#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);

    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        globalEventOffset = CSL_NAVSS_GEM_MCU_UDMA_INTA0_SEVI_OFFSET;
    }
    else
    {
        globalEventOffset = CSL_NAVSS_GEM_MAIN_UDMA_INTA0_SEVI_OFFSET;
    }
#else
    globalEventOffset = CSL_NAVSS_GEM_MAIN_UDMA_INTA0_SEVI_OFFSET;
#endif

    return (globalEventOffset);
}

uint32_t Udma_getCoreId(void)
{
    uint32_t coreId = UDMA_CORE_ID_INVALID;

#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);

    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            coreId = UDMA_CORE_ID_MCU1_0;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            coreId = UDMA_CORE_ID_MCU1_1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_1)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            coreId = UDMA_CORE_ID_MCU2_0;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            coreId = UDMA_CORE_ID_MCU2_1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        /* Do Nothing */
    }
#endif
#if defined (BUILD_MPU1_0)
    coreId = UDMA_CORE_ID_MPU1_0;
#endif

    return (coreId);
}

uint16_t Udma_getCoreSciDevId(void)
{
    uint16_t coreSciDevId = 0U;

#if defined (BUILD_MCU)
    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);

    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            coreSciDevId = TISCI_DEV_MCU_R5FSS0_CORE0;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            coreSciDevId = TISCI_DEV_MCU_R5FSS0_CORE1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_1)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            coreSciDevId = TISCI_DEV_R5FSS0_CORE0;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            coreSciDevId = TISCI_DEV_R5FSS0_CORE1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        /* Do Nothing */
    }
#endif
#if defined (BUILD_MPU1_0)
    coreSciDevId = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS;
#endif

    return (coreSciDevId);
}

uint32_t Udma_isCacheCoherent(void)
{
    uint32_t isCacheCoherent;

#if defined (BUILD_MPU1_0)
    isCacheCoherent = UTRUE;
#else
    isCacheCoherent = UFALSE;
#endif

    return (isCacheCoherent);
}
