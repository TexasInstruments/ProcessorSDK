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
 *  \defgroup DRV_UDMA_MODULE UDMA Driver
 *
 *  @{
 */
/* @} */

/**
 *  \ingroup DRV_UDMA_MODULE
 *  \defgroup DRV_UDMA_API_MODULE UDMA Driver API
 *            This is UDMA driver init, deinit and common API.
 *
 *  @{
 */

/**
 *  \file udma.h
 *
 *  \brief UDMA Driver API/interface file.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2494)
 */

#ifndef UDMA_H_
#define UDMA_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>

/* UDMA_SOC_CFG_* macros are defined udma_soc.h. 
 * So including this first
 */ 
#include <ti/drv/udma/soc/udma_soc.h>

#include <ti/csl/soc.h>
#include <ti/csl/csl_psilcfg.h>
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
#include <ti/drv/udma/src/ringacc/csl_ringacc.h>
#endif 
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1)
#include <ti/drv/udma/src/lcdma_ringacc/csl_lcdma_ringacc.h>
#endif
#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
#include <ti/drv/udma/src/udmap/csl_udmap.h>
#endif
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
#include <ti/drv/udma/src/bcdma/csl_bcdma.h>
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1)
#include <ti/drv/udma/csl_pktdma.h>
#include <ti/drv/udma/src/ip/udmap/V0/csl_udmap_cppi5.h>
#endif
#endif
#include <ti/drv/udma/src/intaggr/csl_intaggr.h>
#include <ti/csl/csl_intr_router.h>
#if (UDMA_NUM_UTC_INSTANCE > 0)
#include <ti/drv/udma/src/dru/csl_dru.h>
#endif
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
#include <ti/drv/udma/src/proxy/csl_proxy.h>
#endif
#if (UDMA_SOC_CFG_CLEC_PRESENT == 1)
#include <ti/csl/arch/c7x/src/clec/csl_clec.h>
#endif

#include <ti/osal/osal.h>
#include <ti/drv/sciclient/sciclient.h>

#include <ti/drv/udma/include/udma_cfg.h>
#include <ti/drv/udma/include/udma_types.h>
#include <ti/drv/udma/include/udma_osal.h>
#include <ti/drv/udma/include/udma_ring.h>
#include <ti/drv/udma/include/udma_proxy.h>
#include <ti/drv/udma/include/udma_flow.h>
#include <ti/drv/udma/include/udma_event.h>
#include <ti/drv/udma/include/udma_rm.h>
#include <ti/drv/udma/include/udma_ch.h>
#if (UDMA_NUM_UTC_INSTANCE > 0)
#include <ti/drv/udma/include/udma_dru.h>
#endif
#include <ti/drv/udma/include/udma_utils.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief UDMA Virtual to Physical address translation callback function.
 *
 *  This function is used by the driver to convert virtual address to physical
 *  address.
 *
 *  \param virtAddr [IN] Virtual address
 *  \param chNum    [IN] Channel number passed during channel open
 *  \param appData  [IN] Callback pointer passed during channel open
 *
 *  \return Corresponding physical address
 */
typedef uint64_t (*Udma_VirtToPhyFxn)(const void *virtAddr,
                                      uint32_t chNum,
                                      void *appData);
/**
 *  \brief UDMA Physical to Virtual address translation callback function.
 *
 *  This function is used by the driver to convert physical address to virtual
 *  address.
 *
 *  \param phyAddr  [IN] Physical address
 *  \param chNum    [IN] Channel number passed during channel open
 *  \param appData  [IN] Callback pointer passed during channel open
 *
 *  \return Corresponding virtual address
 */
typedef void *(*Udma_PhyToVirtFxn)(uint64_t phyAddr,
                                   uint32_t chNum,
                                   void *appData);

/**
 *  \brief UDMA info/debug print function prototype.
 *
 *  This function is used by the driver to print info/debug messages.
 *
 *  \param str      [OUT] Info string to print.
 */
typedef void (*Udma_PrintFxn)(const char *str);

/**
 *  \anchor Udma_RingLocalApiPrototypes
 *  \name UDMA Ring Local API's function prototypes
 * 
 *  Function prototypes for various local UDMA Ring API's.
 *  For Normal RA / LCDMA RA, function pointers will be used
 *  to call the appropriate function.
 *
 *  @{
 */
/** \brief UDMA Ring handle clear register function prototype */
typedef void (*Udma_ringHandleClearRegsFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring set doorbell function prototype */
typedef void (*Udma_ringSetDoorBellFxn)(Udma_RingHandle ringHandle, 
                                        int32_t count);
/** \brief UDMA Ring prime function prototype */
typedef void (*Udma_ringPrimeFxn)(Udma_RingHandle ringHandle,
                                  uint64_t phyDescMem);
/** \brief UDMA Ring prime read function prototype */
typedef void (*Udma_ringPrimeReadFxn)(Udma_RingHandle ringHandle, 
                                      uint64_t *phyDescMem);
/** \brief UDMA Ring get mem pointer function prototype */
typedef void *(*Udma_ringGetMemPtrFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring get ring mode function prototype */
typedef uint32_t (*Udma_ringGetModeFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring get element count function prototype */
typedef uint32_t (*Udma_ringGetElementCntFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring get forward ring occupancy function prototype */
typedef uint32_t (*Udma_ringGetForwardRingOccFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring get reverse ring occupancy function prototype */
typedef uint32_t (*Udma_ringGetReverseRingOccFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring get write index value function prototype */
typedef uint32_t (*Udma_ringGetWrIdxFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring get read index value function prototype */
typedef uint32_t (*Udma_ringGetRdIdxFxn)(Udma_RingHandle ringHandle);
/** \brief UDMA Ring dequeue raw function prototype */
typedef int32_t (*Udma_ringDequeueRawFxn)(Udma_DrvHandle  drvHandle, 
                                          Udma_RingHandle ringHandle, 
                                          uint64_t *phyDescMem);
/** \brief UDMA Ring queue raw function prototype */
typedef int32_t (*Udma_ringQueueRawFxn)(Udma_DrvHandle  drvHandle, 
                                        Udma_RingHandle ringHandle, 
                                        uint64_t phyDescMem); 
/** \brief UDMA Ring flush raw function prototype */
typedef int32_t (*Udma_ringFlushRawFxn)(Udma_DrvHandle  drvHandle, 
                                        Udma_RingHandle ringHandle, 
                                        uint64_t *phyDescMem); 
/** \brief UDMA Ring set Cfg function prototype */
typedef void (*Udma_ringSetCfgFxn)(Udma_DrvHandle drvHandle,
                                   Udma_RingHandle ringHandle,
                                   const Udma_RingPrms *ringPrms);
/* @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief UDMA initialization parameters.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2631)
 */
typedef struct
{
    uint32_t                instId;
    /**< [IN] \ref Udma_InstanceIdSoc */

    Udma_RmInitPrms         rmInitPrms;
    /**< RM init parameters */
    uint32_t                skipRmOverlapCheck;
    /**< Skips the resource overlap check - useful when running from pre-silicon
     *   environment as well as in production code when the resource overlap
     *   is already checked */
    uint32_t                skipGlobalEventReg;
    /**< Skips the global event registeration for the handle. By default this
     *   is set to UFALSE and application can use this common handle to set the
     *   master event to limit the number of IA/IR registration per core
     *   This can be set to UTRUE to skip this registration as in the case
     *   of having multiple handles per core in usecases */
    Udma_VirtToPhyFxn       virtToPhyFxn;
    /**< If not NULL, this function will be called to convert virtual address
     *   to physical address to be provided to UDMA.
     *   If NULL, the driver will assume a one-one mapping.
     *
     *   Note: The init fxn will initialize this to the default one-one map
     *   function #Udma_defaultVirtToPhyFxn
     */
    Udma_PhyToVirtFxn       phyToVirtFxn;
    /**< If not NULL, this function will be called to convert physical address
     *   to virtual address to access the pointer returned by the UDMA.
     *   If NULL, the driver will assume a one-one mapping.
     *
     *   Note: The init fxn will initialize this to the default one-one map
     *   function #Udma_defaultPhyToVirtFxn
     */
    Udma_PrintFxn           printFxn;
    /**< If not NULL, this function will be called to print debug/info message
     *   with appropriate string. */

    Udma_OsalPrms           osalPrms;
    /**< OSAL callback  parameters */
} Udma_InitPrms;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief UDMA init function.
 *
 *  Initializes the UDMA drivers.
 *  This function should be called before calling any of driver API's and
 *  should be called only once.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2576)
 *
 *  \param drvHandle    [IN] UDMA driver handle - static memory needs to
 *                           allocated by caller. This is used by the driver to
 *                           maintain the driver states.
 *                           This cannot be NULL.
 *  \param initPrms     [IN] UDMA Initialization parameters.
 *                           If NULL is passed, the default parameters will be
 *                           assumed - address translation disabled.
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t Udma_init(Udma_DrvHandle drvHandle, const Udma_InitPrms *initPrms);

/**
 *  \brief UDMA deinit function.
 *
 *  Uninitializes the drivers and the hardware and should be called during
 *  system shutdown. Should not be called if Udma_init() is not called.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2577)
 *
 *  \param drvHandle    [IN] UDMA driver handle pointer passed during
 *                           #Udma_init
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t Udma_deinit(Udma_DrvHandle drvHandle);

/*
 * Structure Init functions
 *
 * Requirement: DOX_REQ_TAG(PDK-2600)
 */
/**
 *  \brief Udma_InitPrms structure init function.
 *  
 *  Note: API returns error when there is a failure in intilaizing
 *  \ref Udma_RmInitPrms. This can be due to the following reasons:
 *  - Wrong entry for resources in Default Board Cfg 
 *    (Sciclient_defaultBoardCfg_rm.c)
 *  - Number of resources reserved in Default Board Cfg is less
 *    than the 'minumum requirement' specified as per 
 *    UDMA RM Shared resource parameters \ref Udma_RmSharedResPrms.
 *    In this case, user should reserve more resources in Default Board Cfg 
 *    OR override the default UDMA RM Shared resource parameters.
 *    (Use \ref Udma_rmGetSharedResPrms API to get default 
 *     UDMA RM Shared resource parameters)
 *  - Total number of resources requested for each instance
 *    as per UDMA RM Shared resource parameters
 *    is greater than the number of resorurces reserved in Default 
 *    Board Cfg.
 *    In this case, user should reduce the requested share for each 
 *    instance in UDMA RM Shared resource parameters.
 * 
 *  \param instId       [IN] \ref Udma_InstanceIdSoc
 *  \param initPrms     [IN] Pointer to #Udma_InitPrms structure.
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t UdmaInitPrms_init(uint32_t instId, Udma_InitPrms *initPrms);

/* ========================================================================== */
/*      Internal Function Declarations (Needed for other static inlines)      */
/* ========================================================================== */

/**
 *  \brief Default virtual to physical translation function.
 *
 *  \param virtAddr [IN] Virtual address
 *  \param chNum    [IN] Channel number passed during channel open.
 *                       Note: When called for functions which is not channel
 *                       dependent (like ring alloc), this parameter will
 *                       be set to #UDMA_DMA_CH_INVALID.
 *  \param appData  [IN] Callback pointer passed during channel open.
 *                       Note: When called for functions which is not channel
 *                       dependent (like ring alloc), this parameter will
 *                       be set to NULL.
 *
 *  \return Corresponding physical address
 */
static inline uint64_t Udma_defaultVirtToPhyFxn(const void *virtAddr,
                                                uint32_t chNum,
                                                void *appData);

/**
 *  \brief Default physical to virtual translation function.
 *
 *  \param phyAddr  [IN] Physical address
 *  \param chNum    [IN] Channel number passed during channel open.
 *                       Note: When called for functions which is not channel
 *                       dependent (like ring alloc), this parameter will
 *                       be set to #UDMA_DMA_CH_INVALID.
 *  \param appData  [IN] Callback pointer passed during channel open.
 *                       Note: When called for functions which is not channel
 *                       dependent (like ring alloc), this parameter will
 *                       be set to NULL.
 *
 *  \return Corresponding virtual address
 */
static inline void *Udma_defaultPhyToVirtFxn(uint64_t phyAddr,
                                             uint32_t chNum,
                                             void *appData);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static uint64_t Udma_defaultVirtToPhyFxn(const void *virtAddr,
                                         uint32_t chNum,
                                         void *appData)
{
    return ((uint64_t) virtAddr);
}

static void *Udma_defaultPhyToVirtFxn(uint64_t phyAddr,
                                      uint32_t chNum,
                                      void *appData)
{
#if defined (__aarch64__)
    uint64_t temp = phyAddr;
#else
    /* R5 is 32-bit machine, need to truncate to avoid void * typecast error */
    uint32_t temp = (uint32_t) phyAddr;
#endif

    return ((void *) temp);
}

/* ========================================================================== */
/*                  Internal/Private Structure Declarations                   */
/* ========================================================================== */

/**
 *  \brief UDMA driver object.
 *
 *  Note: This is an internal/private driver structure and should not be
 *  used or modified by caller.
 */
struct Udma_DrvObj
{   
    uint32_t                instType;
    /**< Udma Instance Type */
    uint32_t                raType;
    /**< Udma Ring Accelerator Type */

#if (UDMA_SOC_CFG_UDMAP_PRESENT == 1)
    /*
     * NAVSS instance parameters
     */
    CSL_UdmapCfg            udmapRegs;
    /**< UDMAP register configuration */ 
#endif 
#if (UDMA_SOC_CFG_BCDMA_PRESENT == 1)
    CSL_BcdmaCfg             bcdmaRegs;
    /**< BCDMA register configuration */
#endif
#if (UDMA_SOC_CFG_PKTDMA_PRESENT == 1) 
    CSL_PktdmaCfg            pktdmaRegs; 
    /**< PKTDMA register configuration */ 
#endif 
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)  
    CSL_RingAccCfg          raRegs;
#endif
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1)  
    CSL_LcdmaRingaccCfg     lcdmaRaRegs;
#endif
    /**< RA register configuration */
    CSL_IntaggrCfg          iaRegs;
    /**< Interrupt Aggregator configuration */
#if (UDMA_NUM_UTC_INSTANCE > 0)
    Udma_UtcInstInfo        utcInfo[UDMA_NUM_UTC_INSTANCE];
    /**< UTC instance information */
#endif
    uint32_t                udmapSrcThreadOffset;
    /**< UDMAP Source/TX thread offset */
    uint32_t                udmapDestThreadOffset;
    /**< UDMAP Dest/RX thread offset */
    uint32_t                maxRings;
    /**< Maximun number of rings present in the NAVSS instance */
    uint32_t                maxProxy;
    /**< Maximun number of proxy present in the NAVSS instance */
    uint32_t                maxRingMon;
    /**< Maximun number of ring monitors present in the NAVSS instance */   

#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
    /*
     * Proxy parameters
     */
    CSL_ProxyCfg            proxyCfg;
    /*< Proxy register configuration */
    CSL_ProxyTargetParams   proxyTargetRing;
    /*< Proxy ring target register configuration */
    uint32_t                proxyTargetNumRing;
    /*< Proxy ring target index */
#endif    

#if (UDMA_SOC_CFG_CLEC_PRESENT == 1)
    /*
     * Clec parameters
     */
    CSL_CLEC_EVTRegs       *clecRegs;
    /**< CLEC baseaddress. */
    uint32_t                clecRtMap;
    /**< Route map bit field - differs from core to core.
     *   Refer \ref CSL_ClecRouteMap. */
    uint32_t                clecOffset;
    /**< GIC SPI to CLEC interrupt offset (they are not directly connected */
#endif

    /*
     * TISCI RM parameters
     */
    uint16_t                devIdRing;
    /**< Ring RM ID */
    uint16_t                devIdUdma;
    /**< UDMA RM ID */
    uint16_t                devIdPsil;
    /**< PSIL RM ID */
    uint16_t                devIdIa;
    /**< IA RM ID */
    uint16_t                devIdIr;
    /**< IR RM ID */
    uint16_t                devIdDummyEvtSrc;
    /**< RM ID of Event Source not having OES registers */
#if (UDMA_SOC_CFG_PROXY_PRESENT == 1)
    uint16_t                devIdProxy;
    /**< Proxy RM ID */
#endif
    uint16_t                devIdCore;
    /**< Core RM ID */
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t                druCoreId;
    /**< DRU core ID register to use for direct TR submission.
     *   Each CPU should have a unique submit register to avoid corrupting
     *   submit word when SW is running from multiple CPU at the same time.
     *   Refer \ref Udma_DruSubmitCoreId */
#endif  
    /*
     * TISCI Ring event IRQ params
     * 
     * These IRQ offsets should be corresponding TISCI offset - ringNum Offset
     */
    uint16_t                srcIdRingIrq;
    /**< Ring completion event IRQ Source ID. */
    uint16_t                blkCopyRingIrqOffset;
    /**< Block Copy channel ring completion event IRQ offset. */
    uint16_t                txRingIrqOffset;
    /**< TX channel ring completion event IRQ offset. */
    uint16_t                rxRingIrqOffset;
    /**< RX channel ring completion event IRQ offset. */
    /*
     * TISCI TR event IRQ params
     * 
     * These IRQ offsets should be corresponding TISCI offset - chNum Offset
     */
    uint16_t                srcIdTrIrq;
    /**< TR event IRQ Source ID. */
    uint16_t                blkCopyTrIrqOffset;
    /**< Block Copy channel TR event IRQ offset. */
    uint16_t                txTrIrqOffset;
    /**< TX channel TR event IRQ offset. */
    uint16_t                rxTrIrqOffset;
    /**< RX channel TR event IRQ offset. */
    /*
     * Channel Offsets
     */
    uint32_t                txChOffset;
    /**< TX channel offset. */
    uint32_t                extChOffset;
    /**< External channel offset. */
    uint32_t                rxChOffset;
    /**< RX channel offset. */
    /*
     *  The driver allocates ringNum = chNum (for BlkCpoy)
                                     = chNum + txChOffset (for SplitTR Tx)
                                     = chNum + rxChOffset (for SplitTR Rx)

        For CSL_bcdma* API's passed param ->channel_num = txChNum (for BlkCopy)
                                                        = txChNum + txChOffset (for SplitTR Tx)
                                                        = rxChNum + rxChOffset (for SplitTR Rx)
    */
    /*
     * Other Offsets
     */
    uint32_t                iaGemOffset;
    /**< IA global event map offset to differentiate between main and MCU NAVSS */
    uint32_t                trigGemOffset;
    /**< UDMAP trigger global event map offset to differentiate between main
     *   and MCU NAVSS */

    struct Udma_EventObj    globalEventObj;
    /**< Object to store global event. */
    Udma_EventHandle        globalEventHandle;
    /**< Global event handle. */

    Udma_InitPrms           initPrms;
    /**< Object to store the init params. */
    uint32_t                drvInitDone;
    /**< Flag to check if the driver object is init properly or not. */

    /*
     * RM objects.
     * This is a bitwise flag
     * 1 - free, 0 - allocated
     */
    uint32_t                blkCopyChFlag[UDMA_RM_BLK_COPY_CH_ARR_SIZE];
    /**< UDMA Block copy channel allocation flag */
    uint32_t                blkCopyHcChFlag[UDMA_RM_BLK_COPY_HC_CH_ARR_SIZE];
    /**< UDMA high capacity Block copy channel allocation flag */
    uint32_t                blkCopyUhcChFlag[UDMA_RM_BLK_COPY_UHC_CH_ARR_SIZE];
    /**< UDMA ultra high capacity Block copy channel allocation flag */

    uint32_t                txChFlag[UDMA_RM_TX_CH_ARR_SIZE];
    /**< UDMA TX channel allocation flag */
    uint32_t                txHcChFlag[UDMA_RM_TX_HC_CH_ARR_SIZE];
    /**< UDMA high capacity TX channel allocation flag */
    uint32_t                txUhcChFlag[UDMA_RM_TX_UHC_CH_ARR_SIZE];

    /**< UDMA ultra high capacity TX channel allocation flag */
    uint32_t                rxChFlag[UDMA_RM_RX_CH_ARR_SIZE];
    /**< UDMA RX channel allocation flag */
    uint32_t                rxHcChFlag[UDMA_RM_RX_HC_CH_ARR_SIZE];
    /**< UDMA high capacity RX channel allocation flag */
    uint32_t                rxUhcChFlag[UDMA_RM_RX_UHC_CH_ARR_SIZE];
    /**< UDMA ultra high capacity RX channel allocation flag */
    
#if (UDMA_NUM_UTC_INSTANCE > 0)
    uint32_t                utcChFlag[UDMA_NUM_UTC_INSTANCE][UDMA_RM_UTC_CH_ARR_SIZE];
    /**< UDMA external UTC channel allocation flag */
#endif
#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
    uint32_t                mappedTxChFlag[UDMA_NUM_MAPPED_TX_GROUP][UDMA_RM_MAPPED_TX_CH_ARR_SIZE];
    /**< UDMA mapped TX channel allocation flag */
#endif
#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
    uint32_t                mappedRxChFlag[UDMA_NUM_MAPPED_RX_GROUP][UDMA_RM_MAPPED_RX_CH_ARR_SIZE];
    /**< UDMA mapped RX channel allocation flag */
#endif
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
    uint32_t                mappedRingFlag[UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP][UDMA_RM_MAPPED_RING_ARR_SIZE];
    /**< UDMA mapped ring allocation flag */
#endif

    uint32_t                freeRingFlag[UDMA_RM_FREE_RING_ARR_SIZE];
    /**< UDMA free ring allocation flag */
    uint32_t                freeFlowFlag[UDMA_RM_FREE_FLOW_ARR_SIZE];
    /**< UDMA free flow allocation flag */
    uint32_t                globalEventFlag[UDMA_RM_GLOBAL_EVENT_ARR_SIZE];
    /**< IA global event allocation flag */
    uint32_t                vintrFlag[UDMA_RM_VINTR_ARR_SIZE];
    /**< IA VINTR allocation flag */
    uint32_t                irIntrFlag[UDMA_RM_IR_INTR_ARR_SIZE];
    /**< IR interrupt allocation flag */
    uint32_t                proxyFlag[UDMA_RM_PROXY_ARR_SIZE];
    /**< UDMA proxy allocation flag */
    uint32_t                ringMonFlag[UDMA_RM_RING_MON_ARR_SIZE];
    /**< UDMA ring monitor allocation flag */

    void                   *rmLock;
    /**< Mutex to protect RM allocation. */
    void                   *printLock;
    /**< Mutex to protect print buffer. */
    char                    printBuf[UDMA_CFG_PRINT_BUF_LEN];
    /**< Print buffer */

    /*
     * UDMA Ring Local API's function pointers
     * For Normal RA / LCDMA RA, these function pointers are used
     * to call the appropriate function.
     */
    Udma_ringDequeueRawFxn            ringDequeueRaw;
    /**< UDMA Ring dequeue raw function pointer */
    Udma_ringQueueRawFxn              ringQueueRaw;
    /**< UDMA Ring queue raw function pointer */
    Udma_ringFlushRawFxn              ringFlushRaw;
    /**< UDMA Ring flush raw function pointer */
    Udma_ringGetElementCntFxn         ringGetElementCnt;
    /**< UDMA Ring get element count function pointer */
    Udma_ringGetMemPtrFxn             ringGetMemPtr;
    /**< UDMA Ring get mem pointer function pointer */
    Udma_ringGetModeFxn               ringGetMode;
    /**< UDMA Ring get ring mode function pointer */
    Udma_ringGetForwardRingOccFxn     ringGetForwardRingOcc;
    /**< UDMA Ring get forward ring occupancy function pointer */
    Udma_ringGetReverseRingOccFxn     ringGetReverseRingOcc;
    /**< UDMA Ring get reverse ring occupancy function pointer */
    Udma_ringGetWrIdxFxn              ringGetWrIdx;
    /**< UDMA Ring get write index value function pointer */
    Udma_ringGetRdIdxFxn              ringGetRdIdx;
    /**< UDMA Ring get read index value function pointer */
    Udma_ringPrimeFxn                 ringPrime;
    /**< UDMA Ring prime function pointer */
    Udma_ringPrimeReadFxn             ringPrimeRead;
    /**< UDMA Ring prime read function pointer */
    Udma_ringSetDoorBellFxn           ringSetDoorBell;
    /**< UDMA Ring set doorbell function pointer */
    Udma_ringSetCfgFxn                ringSetCfg;
    /**< UDMA Ring set Cfg function pointer */
    Udma_ringHandleClearRegsFxn       ringHandleClearRegs;
    /**< UDMA Ring handle clear register function pointer */
};

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UDMA_H_ */

/* @} */
