/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2022
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
 *  \file udma_priv.h
 *
 *  \brief UDMA private header file.
 */

#ifndef UDMA_PRIV_H_
#define UDMA_PRIV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* This is needed for memset/memcpy */
#include <string.h>
#include <ti/drv/udma/udma.h>

#if defined (BUILD_MCU)
#include <ti/csl/arch/r5/csl_arm_r5.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/**
 *  \anchor Udma_InstanceType
 *  \name UDMA Instance Type
 *
 *  UDMA instance Type - UDMA/LCDMA_BCDMA/LCDMA_PKTDMA
 *
 *  @{
 */
/** \brief UDMA instance type*/
#define UDMA_INST_TYPE_NORMAL            (0U)
/** \brief LCDMA - Block Copy DMA instance type*/
#define UDMA_INST_TYPE_LCDMA_BCDMA     (1U)
/** \brief LCDMA - Packet DMA instance type*/
#define UDMA_INST_TYPE_LCDMA_PKTDMA    (2U)
/** \brief Maximum number of UDMA instance types */
#define UDMA_INST_TYPE_MAX             (3U)
/* @} */

/**
 *  \anchor Udma_RingAccType
 *  \name UDMA Ring Accelerator Type
 *
 *  UDMA ring accelerator Type - Normal RA/Lcdma RA
 *
 *  @{
 */
/** \brief Normal RA type*/
#define UDMA_RA_TYPE_NORMAL            (0U)
/** \brief Lcdma RA type*/
#define UDMA_RA_TYPE_LCDMA             (1U)
/** \brief Maximum number of RA types */
#define UDMA_RA_TYPE_MAX               (2U)
/* @} */

#if defined (UDMA_CFG_ASSERT_ENABLE)
#define Udma_assert(drvHandle, cond)                                     \
    (Udma_assertLocal((drvHandle), (bool) (cond), (const char *) # cond, \
                    (const char *) __FILE__, (int32_t) __LINE__))
#else
#define Udma_assert(drvHandle, cond)
#endif

/** \brief Macro used to specify that the thread ID is invalid. */
#define UDMA_THREAD_ID_INVALID          ((uint32_t) 0xFFFF0004U)

/** \brief Macro used to specify that the Sciclient RM resource assignment type is invalid. */
#define UDMA_RM_SCI_REQ_TYPE_INVALID       ((uint16_t) 0xFFFFU)

/** \brief Macro used to specify that the Sciclient RM resource assignment subtype is invalid. */
#define UDMA_RM_SCI_REQ_SUBTYPE_INVALID    ((uint8_t) 0xFFU)

/** \brief Macro used to specify shift value for RX flow threshold before passing to SysFw */
#define UDMA_RFLOW_RX_SIZE_THRESH_VAL_SHIFT      ((uint32_t) 0x00000005U)

/** Invalid Core ID */
#define UDMA_CORE_ID_INVALID               ((uint32_t) 0xFFFF0008U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief UDMA Sciclient Default BoardCfg RM parameters.
 */
typedef struct
{
    uint32_t                resId;
    /**< UDMA Resource Id \ref Udma_RmResId macros. */
    uint16_t                sciclientReqType;
    /**< Sciclient RM resource assignment type */
    uint8_t                 sciclientReqSubtype;
    /**< Sciclient RM resource assignment subtype */
    uint8_t                 sciclientSecHost;
    /**< Sciclient Secondary host */
} Udma_RmDefBoardCfgPrms;


/**
 *  \brief UDMA Sciclient Default BoardCfg RM Response.
 */
typedef struct
{
    uint32_t                resId;
    /**< UDMA Resource Id \ref Udma_RmResId macros. */
    uint16_t                rangeStart;
    /**< Sciclient RM resource reservation start */
    uint16_t                rangeNum;
    /**< Sciclient RM no. of resources reserverd */
    uint16_t                rangeStartSec;
    /**< Sciclient RM resource secondary reservation start */
    uint16_t                rangeNumSec;
    /**< Sciclient RM no. of resources reserverd secondary*/
} Udma_RmDefBoardCfgResp;

/* ========================================================================== */
/*                         Global Variables                                   */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* SOC APIs */
void Udma_initDrvHandle(Udma_DrvHandle drvHandle);
uint32_t Udma_getGlobalEventOffset(void);
const Udma_RmDefBoardCfgPrms *Udma_rmGetDefBoardCfgPrms(uint32_t instId);
const Udma_RmDefBoardCfgResp *Udma_rmGetLocalBoardCfgResp(uint32_t resId);
#if ((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
int32_t Udma_getMappedChRingAttributes(Udma_DrvHandle drvHandle,
                                       uint32_t mappedGrp,
                                       uint32_t chNum,  
                                       Udma_MappedChRingAttributes *chAttr);
#endif
/* Private APIs */
#if (UDMA_NUM_UTC_INSTANCE > 0)
const Udma_UtcInstInfo *Udma_chGetUtcInst(Udma_DrvHandle drvHandle,
                                          uint32_t utcId);
#endif

/**
 *  \brief Default Normal RA memory fence API used for CSL-FL to perform cache ops
 *
 *  \param pVirtAddr        [IN]    The virtual memory address written to
 *  \param size             [IN]    Number of bytes to writeback
 *  \param opsType          [IN]    \ref CSL_RingAccMemoryOpsType
 */
void Udma_ringaccMemOps(void *pVirtAddr, uint32_t size, uint32_t opsType);

/**
 *  \brief Default Lcdma RA memory fence API used for CSL-FL to perform cache ops
 *
 *  \param pVirtAddr        [IN]    The virtual memory address written to
 *  \param size             [IN]    Number of bytes to writeback
 *  \param opsType          [IN]    \ref CSL_LcdmaRingAccMemoryOpsType
 */
void Udma_lcdmaRingaccMemOps(void *pVirtAddr, uint32_t size, uint32_t opsType);

/*
 * Ring APIs
 */
int32_t Udma_ringCheckParams(Udma_DrvHandle drvHandle,
                             const Udma_RingPrms *ringPrms);

#if (UDMA_SOC_CFG_APPLY_RING_WORKAROUND == 1)
int32_t Udma_ringReset(Udma_DrvHandle drvHandle,
                       Udma_RingHandle ringHandle);
#endif

#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
/* Normal RA APIs*/
void Udma_ringHandleClearRegsNormal(Udma_RingHandle ringHandle);
void Udma_ringSetDoorBellNormal(Udma_RingHandle ringHandle, int32_t count);
void Udma_ringPrimeNormal(Udma_RingHandle ringHandle, uint64_t phyDescMem);
void Udma_ringPrimeReadNormal(Udma_RingHandle ringHandle, uint64_t *phyDescMem);
void *Udma_ringGetMemPtrNormal(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetModeNormal(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetElementCntNormal(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetRingOccNormal(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetWrIdxNormal(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetRdIdxNormal(Udma_RingHandle ringHandle);
int32_t Udma_ringDequeueRawNormal(Udma_DrvHandle  drvHandle, 
                                  Udma_RingHandle ringHandle, 
                                  uint64_t *phyDescMem);
int32_t Udma_ringQueueRawNormal(Udma_DrvHandle  drvHandle, 
                                Udma_RingHandle ringHandle, 
                                uint64_t phyDescMem); 
int32_t Udma_ringFlushRawNormal(Udma_DrvHandle  drvHandle, 
                                Udma_RingHandle ringHandle, 
                                uint64_t *phyDescMem); 
void Udma_ringSetCfgNormal(Udma_DrvHandle drvHandle,
                           Udma_RingHandle ringHandle,
                           const Udma_RingPrms *ringPrms);
#endif
#if (UDMA_SOC_CFG_RA_LCDMA_PRESENT == 1)
/* LCDMA RA APIs*/
void Udma_ringHandleClearRegsLcdma(Udma_RingHandle ringHandle);
void Udma_ringSetDoorBellLcdma(Udma_RingHandle ringHandle, int32_t count);
void Udma_ringPrimeLcdma(Udma_RingHandle ringHandle, uint64_t phyDescMem);
void Udma_ringPrimeReadLcdma(Udma_RingHandle ringHandle, uint64_t *phyDescMem);
void *Udma_ringGetMemPtrLcdma(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetModeLcdma(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetElementCntLcdma(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetForwardRingOccLcdma(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetReverseRingOccLcdma(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetWrIdxLcdma(Udma_RingHandle ringHandle);
uint32_t Udma_ringGetRdIdxLcdma(Udma_RingHandle ringHandle);
int32_t Udma_ringDequeueRawLcdma(Udma_DrvHandle  drvHandle, 
                                 Udma_RingHandle ringHandle, 
                                 uint64_t *phyDescMem);
int32_t Udma_ringQueueRawLcdma(Udma_DrvHandle  drvHandle, 
                               Udma_RingHandle ringHandle, 
                               uint64_t phyDescMem); 
int32_t Udma_ringFlushRawLcdma(Udma_DrvHandle  drvHandle, 
                               Udma_RingHandle ringHandle, 
                               uint64_t *phyDescMem); 
void Udma_ringSetCfgLcdma(Udma_DrvHandle drvHandle,
                          Udma_RingHandle ringHandle,
                          const Udma_RingPrms *ringPrms);
#endif
/* Proxy APIs*/
int32_t Udma_ringProxyQueueRaw(Udma_RingHandle ringHandle,
                               Udma_DrvHandle drvHandle,
                               uint64_t phyDescMem);
int32_t Udma_ringProxyDequeueRaw(Udma_RingHandle ringHandle,
                                 Udma_DrvHandle drvHandle,
                                 uint64_t *phyDescMem);
/*
 * RM APIs
 */
int32_t Udma_rmInit(Udma_DrvHandle drvHandle);
int32_t Udma_rmDeinit(Udma_DrvHandle drvHandle);
int32_t UdmaRmInitPrms_init(uint32_t instId, Udma_RmInitPrms *rmInitPrms);
/* Channel RM APIs */
uint32_t Udma_rmAllocBlkCopyCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeBlkCopyCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocBlkCopyHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeBlkCopyHcCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocBlkCopyUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeBlkCopyUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocTxCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeTxCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocRxCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeRxCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocTxHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeTxHcCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocRxHcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeRxHcCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocTxUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeTxUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocRxUhcCh(uint32_t preferredChNum, Udma_DrvHandle drvHandle);
void Udma_rmFreeRxUhcCh(uint32_t chNum, Udma_DrvHandle drvHandle);
#if (UDMA_NUM_UTC_INSTANCE > 0)
uint32_t Udma_rmAllocExtCh(uint32_t preferredChNum,
                           Udma_DrvHandle drvHandle,
                           const Udma_UtcInstInfo *utcInfo);
void Udma_rmFreeExtCh(uint32_t chNum,
                      Udma_DrvHandle drvHandle,
                      const Udma_UtcInstInfo *utcInfo);
#endif
#if (UDMA_NUM_MAPPED_TX_GROUP > 0)
uint32_t Udma_rmAllocMappedTxCh(uint32_t preferredChNum,
                                Udma_DrvHandle drvHandle,
                                const uint32_t mappedChGrp);
void Udma_rmFreeMappedTxCh(uint32_t chNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedChGrp);
#endif
#if (UDMA_NUM_MAPPED_RX_GROUP > 0)
uint32_t Udma_rmAllocMappedRxCh(uint32_t preferredChNum,
                                Udma_DrvHandle drvHandle,
                                const uint32_t mappedChGrp);
void Udma_rmFreeMappedRxCh(uint32_t chNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedChGrp);
#endif
/* Ring RM APIs */
#if((UDMA_NUM_MAPPED_TX_GROUP + UDMA_NUM_MAPPED_RX_GROUP) > 0)
uint32_t Udma_rmAllocMappedRing(Udma_DrvHandle drvHandle,
                                const uint32_t mappedRingGrp,
                                const uint32_t mappedChNum);
void Udma_rmFreeMappedRing(uint32_t ringNum,
                           Udma_DrvHandle drvHandle,
                           const uint32_t mappedRingGrp,
                           const uint32_t mappedChNum);
#endif
uint16_t Udma_rmAllocFreeRing(Udma_DrvHandle drvHandle);
void Udma_rmFreeFreeRing(uint16_t ringNum, Udma_DrvHandle drvHandle);
uint16_t Udma_rmAllocRingMon(Udma_DrvHandle drvHandle);
void Udma_rmFreeRingMon(uint16_t ringMonNum, Udma_DrvHandle drvHandle);
/* Proxy RM APIs */
uint16_t Udma_rmAllocProxy(Udma_DrvHandle drvHandle);
void Udma_rmFreeProxy(uint16_t proxyNum, Udma_DrvHandle drvHandle);
/* Event RM APIs */
uint32_t Udma_rmAllocEvent(Udma_DrvHandle drvHandle);
void Udma_rmFreeEvent(uint32_t globalEvent, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocVintr(Udma_DrvHandle drvHandle);
void Udma_rmFreeVintr(uint32_t vintrNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmAllocVintrBit(Udma_EventHandle eventHandle);
void Udma_rmFreeVintrBit(uint32_t vintrBitNum,
                         Udma_DrvHandle drvHandle,
                         Udma_EventHandle eventHandle);
uint32_t Udma_rmAllocIrIntr(uint32_t preferredIrIntrNum,
                              Udma_DrvHandle drvHandle);
void Udma_rmFreeIrIntr(uint32_t irIntrNum, Udma_DrvHandle drvHandle);
uint32_t Udma_rmTranslateIrOutput(Udma_DrvHandle drvHandle, uint32_t irIntrNum);
uint32_t Udma_rmTranslateCoreIntrInput(Udma_DrvHandle drvHandle, uint32_t coreIntrNum);

/* Utils APIs */
uint64_t Udma_virtToPhyFxn(const void *virtAddr,
                           Udma_DrvHandle drvHandle,
                           Udma_ChHandle chHandle);
void *Udma_phyToVirtFxn(uint64_t phyAddr,
                        Udma_DrvHandle drvHandle,
                        Udma_ChHandle chHandle);
/**
 *  \brief Prints to Shared memory and CCS console
 *
 *  This function prints the provided formatted string to shared memory and CCS
 *  console
 *
 *  \param drvHandle    [IN] Driver handle
 *  \param format       [IN] Formatted string followed by variable arguments
 */
void Udma_printf(Udma_DrvHandle drvHandle, const char *format, ...);

#if defined (UDMA_CFG_ASSERT_ENABLE)
static inline void Udma_assertLocal(Udma_DrvHandle drvHandle,
                                    bool           condition,
                                    const char    *str,
                                    const char    *fileName,
                                    int32_t        lineNum);
#endif  /* if defined (UDMA_CFG_ASSERT_ENABLE) */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

#if defined (UDMA_CFG_ASSERT_ENABLE)
static inline void Udma_assertLocal(Udma_DrvHandle drvHandle,
                                    bool           condition,
                                    const char    *str,
                                    const char    *fileName,
                                    int32_t        lineNum)
{
    if(!(condition))
    {
        Udma_printf(drvHandle,
                       "Assertion @ Line: %d in %s: %s : failed !!!\n",
                       lineNum, fileName, str);
    }

    return;
}
#endif  /* if defined (UDMA_CFG_ASSERT_ENABLE) */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef UDMA_PRIV_H_ */
