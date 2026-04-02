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

#if !defined (MCU_PLUS_SDK)
#include <ti/csl/csl_dru.h>

#include <ti/drv/udma/dmautils/udma_standalone/include/udma_types.h>
#include <ti/drv/udma/dmautils/udma_standalone/include/udma_ch.h>
#include <ti/drv/udma/dmautils/udma_standalone/include/udma_event.h>
#else
#include <drivers/dmautils/csl/csl_dru.h>

#include <drivers/dmautils/udma_standalone/include/udma_types.h>
#include <drivers/dmautils/udma_standalone/include/udma_ch.h>
#include <drivers/dmautils/udma_standalone/include/udma_event.h>

#endif


#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */



#define UDMA_UTC_ID_MSMC_DRU0           (0)
#define UDMA_INST_ID_MAIN_0             (0)
#define UDMA_INST_ID_MAIN_1             (1)
#define UDMA_DEFAULT_CH_DISABLE_TIMEOUT     (100U)

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
 *  \brief UDMA resource manager init parameters.
 *
 *  This assumes contiguos allocation of 'N' resources from a start offset
 *  to keep the interface simple.
 *
 *  Note: This is applicable for the driver handle as given during init call.
 *  The init call doesn't (can't rather) check for resource overlap across
 *  handles and across cores. It is the callers responsibility to ensure that
 *  resources overlaps are not present.
 */
typedef struct
{
    uint32_t                druStartId;
    /**< Start channel number for DRU */
    uint32_t                druEndId;
    /**< End channel number for DRU */
}Udma_RmInitPrms;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */




typedef struct
{
    uint32_t                instId;
    /**< [IN] \ref Udma_InstanceIdSoc */
    Udma_RmInitPrms         rmInitPrms;
    /**< RM init parameters */
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

void Udma_chDruSubmitTr(Udma_ChHandle chHandle, const CSL_UdmapTR *tr);
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
    Udma_UtcInstInfo utcInfo[1];
    /**< DRU base address register */
    Udma_InitPrms           initPrms;
    /**< Object to store the init params. */
    uint32_t                drvInitDone;

    uint32_t                druCoreId;
    /**< DRU core ID register to use for direct TR submission.
     *   Each CPU should have a unique submit register to avoid corrupting
     *   submit word when SW is running from multiple CPU at the same time.
     *   Refer \ref Udma_DruSubmitCoreId */
     uint64_t druChannelStatus;
    /**< Each bit here represents the status of corresponding channel*/
};



#ifdef __cplusplus
}
#endif

#endif /* #ifndef UDMA_H_ */

/* @} */
