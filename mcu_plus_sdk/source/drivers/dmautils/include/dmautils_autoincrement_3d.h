/*
 *  Copyright (c) Texas Instruments Incorporated 2022-2023
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
 *  \ingroup DRV_UDMA_MODULE
 *  \defgroup DRV_UDMA_DMAUTILS_MODULE  DMA Utils API
 *            This is DMA Utils related configuration parameters and
 *            API
 *
 *  @{
 */

/**
 *  \file dmautils_autoincrement_3d.h
 *
 *  \brief This file contains the data types and util function prototype for
 *              configuring DMA Utility autoincrement 3D usecase.
 *
 *  Typical sequence of operation :
 *      DmaUtilsAutoInc3d_getContextSize(numChannels) : Request memory required based on the number of
 *                                                                  channels required for a usecase
 *     DmaUtilsAutoInc3d_init : To allocate hardware resources required for for a usecase ( Need to be called only
 *                                             once per frame
 *     DmaUtilsAutoInc3d_getTrMemReq : Request memory required to prepare TR/TR descriptor based on the
 *                                             number of transfer records that needs to be submitted on a particular channel
 *    DmaUtilsAutoInc3d_prepareTr : Populate the tr based on the actual transfer property. This can be computed
 *                                              upfront to avoid cycles consumed to prepare it
 *    DmaUtilsAutoInc3d_configure : Configure a a particular channel by submitting the TR memory prepared earlier.
 *                                               This can be called multiple times within a frame
 *    DmaUtilsAutoInc3d_trigger : Trigger a particular channel
 *    DmaUtilsAutoInc3d_wait : wait for transfer completion of  particular channel
 *
 *    Trigger and wait can be called multiple times till we finish requrested number of transfers
 *
 *    DmaUtilsAutoInc3d_deconfigure :
 *    DmaUtilsAutoInc3d_deint :
 *
 *  Requirement: DOX_REQ_TAG(PDK-2644), DOX_REQ_TAG(PDK-2643), DOX_REQ_TAG(PDK-2642),
 *                         DOX_REQ_TAG(PDK-2645), DOX_REQ_TAG(PDK-2646), DOX_REQ_TAG(PDK-2650),
 *                         DOX_REQ_TAG(PDK-2652), DOX_REQ_TAG(PDK-3241)
 */


#ifndef DMAUTILS_AUTOINCREMENT_3D_H_
#define DMAUTILS_AUTOINCREMENT_3D_H_

#include <stdint.h>
#include <stdarg.h>
#include <drivers/hw_include/csl_types.h>

#ifdef __cplusplus
extern "C" {
#endif

//:TODO: Actual value on SOC is 3 but currently VLAB supports only on
/** \brief Number of TR's that can be submitted back to back channel  */
#define DMAUTILS_MAX_NUM_TR_DIRECT_TR_MODE (1U)
#define DMAUTILS_INTERFACE_WITH_NO_UDMA  (1U)

typedef enum{
  DMAUTILS_SOK = CSL_PASS,
  DMAUTILS_EFAIL = CSL_EFAIL,
  DMAUTILS_EBADARGS = CSL_EBADARGS,
  DMAUTILS_EINVALID_PARAMS = CSL_EINVALID_PARAMS
} DmaUtilsAutoInc3d_ReturnType;

/** \brief Size of the programmable part of the secondary TR Size. This determines the starting offset of the CDB table during compression.*/
#define DMAUTILS_COMP_SECTR_PROG_SIZE (32)
/**
 *  @enum   DmaUtilsAutoInc3d_SyncType
 *
 *  @brief   Describes the boundary at which the DMA is suppose to get
 *          synced
 *
 */
typedef enum{
  DMAUTILSAUTOINC3D_SYNC_1D = 0,/*!< DMA is synced at each 1D transfer i.e. it is synced
                                                                            whenever icnt1 is decremented by 1 */
  DMAUTILSAUTOINC3D_SYNC_2D = 1,/*!< DMA is synced at each 2D transfer i.e. it is synced
                                                                            whenever icnt2 is decremented by 1 */
  DMAUTILSAUTOINC3D_SYNC_3D = 2,/*!< DMA is synced at each 3D transfer i.e. it is synced
                                                                            whenever icnt3 is decremented by 1 */
  DMAUTILSAUTOINC3D_SYNC_4D = 3/*!< DMA is synced at each 4D transfer i.e. it is synced
                                                                            whenever one TR is completed */
}DmaUtilsAutoInc3d_SyncType;


/**
 *  @enum    DmaUtilsAutoInc3d_DfmtType
 *
 *  @brief    Describes the formatting option selected for the DMA
 *
  */
typedef enum{
  DMAUTILSAUTOINC3D_DFMT_NONE      = 0U, /*!< No formatting options */
  DMAUTILSAUTOINC3D_DFMT_COMP      = 5U, /*!< DMA will compress data into 1-D blocks */
  DMAUTILSAUTOINC3D_DFMT_DECOMP    = 6U  /*!< DMA will decompress data from 1-D blocks into decompressed structure */
}DmaUtilsAutoInc3d_DfmtType;


/**
 *  @enum    DmaUtilsAutoInc3d_CmprsType
 *
 *  @brief    Describes the compression type to be employed by the DMA
 *
  */
typedef enum{
  DMAUTILSAUTOINC3D_CMPRS_ZERO      = 0, /*!< Zero compression algorithm */
  DMAUTILSAUTOINC3D_CMPRS_UEG       = 1, /*!< Unsigned Exponential Golomb algorithm */
  DMAUTILSAUTOINC3D_CMPRS_SEG       = 2, /*!< Signed Exponential Golomb algorithm */
  DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K = 3  /*!< Signed Exponenetial Golomb algorithm with variable-k */
}DmaUtilsAutoInc3d_CmprsType;


/**
 *  @enum    DmaUtilsAutoInc3d_AddrType
 *
 *  @brief    Describes the boundary at which the DMA is suppose to get
 *          synced
 *
  */
typedef enum{
  DMAUTILSAUTOINC3D_ADDR_LINEAR = 0 , /*!< Linear addressing, addresses will be updated linearly as
                                                                                        per the dim's and icnt's*/
  DMAUTILSAUTOINC3D_ADDR_CIRC1 = 1,/*!< Circular addressing, address will hold the upper bits of the
                                                                                       addresses to be constant. This enum is to use circSize1 for
                                                                                       circularity */
  DMAUTILSAUTOINC3D_ADDR_CIRC2 = 2 /*!< Circular addressing, address will hold the upper bits of the
                                                                                       addresses to be constant. This enum is to use circSize2 for
                                                                                       circularity*/
}DmaUtilsAutoInc3d_AddrType;


/**
 *  @enum    DmaUtilsAutoInc3d_UpdateKType
 *
 *  @brief    Describes how the DRU will update the k-value for variable-k signed exponential golomb
 *
  */
typedef enum{
  DMAUTILSAUTOINC3D_UPDATEK_NONE = 0, /*!< DRU will not change the k-value between superblocks */
  DMAUTILSAUTOINC3D_UPDATEK_INC  = 1, /*!< DRU will move the k-value by 1 toward the current superblock's
                                             optimial k-value for the next superblock */
  DMAUTILSAUTOINC3D_UPDATEK_JUMP = 2  /*!< DRU will set the k-value to the current superblock's optimal
                                             k-value for the next superblock */
}DmaUtilsAutoInc3d_UpdateKType;


/**
 *  @enum    DmaUtilsAutoInc3d_CmpElemType
 *
 *  @brief    Describes the valid element sizes for variable-k signed exponential golomb compression
 *
  */
typedef enum{
  DMAUTILSAUTOINC3D_CMPELEM_8BIT  = 0, /*!< 8-bit elements */
  DMAUTILSAUTOINC3D_CMPELEM_12BIT = 1, /*!< 12-bit elements */
  DMAUTILSAUTOINC3D_CMPELEM_16BIT = 2  /*!< 16-bit elements */
}DmaUtilsAutoInc3d_CmpElemType;


/**
 *  @enum    DmaUtilsAutoInc3d_CmpSubType
 *
 *  @brief    Describes the pattern of pixel subtraction scheme usages
 *
  */
typedef enum{
  DMAUTILSAUTOINC3D_CMPSUB_NONE = 0, /*!< No Inter-pixel subtraction before compression */
  DMAUTILSAUTOINC3D_CMPSUB_SUB0 = 1, /*!< All pixels use subSel0 scheme */
  DMAUTILSAUTOINC3D_CMPSUB_SUB1 = 2, /*!< Even pixels use subSel0 and odd pixels use subSel1 scheme */
  DMAUTILSAUTOINC3D_CMPSUB_SUB2 = 3, /*!< Pattern of subSel0, subSel1, and subSel2 schemes */
  DMAUTILSAUTOINC3D_CMPSUB_SUB3 = 4  /*!< Pattern of subSel0, subSel1, subSel2, and subSel3 schemes */
}DmaUtilsAutoInc3d_CmpSubType;


/**
 *  @enum    DmaUtilsAutoInc3d_CmpSubSelType
 *
 *  @brief    Describes a pixel subtraction scheme
 *
  */
typedef enum{
  DMAUTILSAUTOINC3D_CMPSUBSEL_SUB1 = 0, /*!< Subtract previous pixel from current pixel */
  DMAUTILSAUTOINC3D_CMPSUBSEL_SUB2 = 1, /*!< Subtract pixel two before from current pixel */
  DMAUTILSAUTOINC3D_CMPSUBSEL_SUB3 = 2, /*!< Subtract pixel three before from current pixel */
  DMAUTILSAUTOINC3D_CMPSUBSEL_SUB4 = 3  /*!< Subtract pixel four before from current pixel */
}DmaUtilsAutoInc3d_CmpSubSelType;


/**
 *  @enum    DmaUtilsAutoInc3d_CircDirType
 *
 *  @brief    Describes the direction in which circular addressing is applied
 *
 */
typedef enum{
  DMAUTILSAUTOINC3D_CIRCDIR_SRC = 0,/*!< Circular addressing if enabled is applied on source side */
  DMAUTILSAUTOINC3D_CIRCDIR_DST = 1/*!<  Circular addressing if enabled is applied on desination side */
}DmaUtilsAutoInc3d_CircDirType;

/**
 *  @enum    DmaUtilsAutoInc3d_CircDirType
 *
 *  @brief    Describes the direction in which circular addressing is applied
 *
 */
typedef enum{
  DMAUTILSAUTOINC3D_DRUOWNER_DIRECT_TR = 0,/*!< DRU channel is used in Direct TR mode */
  DMAUTILSAUTOINC3D_DRUOWNER_UDMA= 1/*!<  Dru channel is used via ring based mechanism*/
}DmaUtilsAutoInc3d_druOwner;

/**
 *  @enum    DmaUtilsAutoInc3d_addrConvertMask
 *
 *  @brief   Describes the addresses that needs to be converted while using
 *           DmaUtilsAutoInc3d_convertToPhysicalAddr update API.User can create a mask
 *           with more than one fields of this enumby "OR"ing these fields
 *           to update more than one parameters.
 *           e.g. if user wants to convert both src and destination addresses
 *           then they can create this mask as follows :
 *           mask = DMAUTILSAUTOINC3D_ADDRCONVERTMASK_SRCADDR | DMAUTILSAUTOINC3D_ADDRCONVERTMASK_DSTADDR
 *
 */
typedef enum{
  DMAUTILSAUTOINC3D_ADDRCONVERTMASK_SRCADDR = (uint32_t)1U << 0,/*!< Convert Source Address Pointer */
  DMAUTILSAUTOINC3D_ADDRCONVERTMASK_DSTADDR = (uint32_t)1U << 1,/*!< Convert Destination Address Pointer */
}DmaUtilsAutoInc3d_addrConvertMask;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief This structure specifies the properties of the transfer for
 *          auto increment usecase.
 *
 */

typedef struct
{
    /** Total loop iteration count for level 0 (innermost) for source*/
    uint16_t    sicnt0;
    /**  Total loop iteration count for level 1 for source*/
    uint16_t    sicnt1;
     /** Total loop iteration count for level 2 for source*/
    uint16_t    sicnt2;
    /** Total loop iteration count for level 3  for source*/
    uint16_t    sicnt3;
    /** Jump in bytes when moving from from icnt0 to icnt1 for source*/
    int32_t     sdim1;
    /** Jump in bytes when moving from from icnt1 to icnt2 for source*/
    int32_t     sdim2;
    /** Jump in bytes when moving from from icnt2 to icnt3 for source*/
    int32_t     sdim3;
    /** Total loop iteration count for level 0 (innermost) for destination*/
    uint16_t    dicnt0;
    /** Total loop iteration count for level 1 for destination*/
    uint16_t    dicnt1;
    /** Total loop iteration count for level 2 for destination*/
    uint16_t    dicnt2;
    /** Total loop iteration count for level 3  for destination*/
    uint16_t    dicnt3;
    /** Jump in bytes when moving from from icnt0 to icnt1 for destination*/
    int32_t     ddim1;
    /** Jump in bytes when moving from from icnt1 to icnt2 for destination*/
    int32_t     ddim2;
    /** Jump in bytes when moving from from icnt2 to icnt3 for destination*/
    int32_t     ddim3;
}DmaUtilsAutoInc3d_TransferDim;


/**
 *
 *  \brief   This structure specifies the circular properties for each level of
 *          the transfer count. User can specify 2 different sizes for each level
 *
 */

typedef struct
{
  /** Size in Bytes of the first circular buffer. The minimum value is
        512 and it should be a power of 2. Maximum value can be 16M */
  uint64_t    circSize1;
  /** Size in Bytes of the first circular buffer. The minimum value is
      512 and it should be a power of 2. Maximum value can be 16M */
  uint64_t    circSize2;
  /**  Which addressing to be used for icnt0. Please refer DmaUtilsAutoInc3d_AddrType
          for the valid values*/
  uint8_t     addrModeIcnt0;
    /**  Which addressing to be used for icnt1. Please refer DmaUtilsAutoInc3d_AddrType
          for the valid values*/
  uint8_t     addrModeIcnt1;
  /**  Which addressing to be used for icnt2. Please refer DmaUtilsAutoInc3d_AddrType
      for the valid values*/
  uint8_t     addrModeIcnt2;
  /**  Which addressing to be used for icnt3. Please refer DmaUtilsAutoInc3d_AddrType
        for the valid values*/
  uint8_t     addrModeIcnt3;
  /**  Direction in which circular addressing needs to be applied, i.e. whether to apply
         circular addressing on source side or destination side. Please refer
         DmaUtilsAutoInc3d_CircDirType for the valid values. Circular addressing can only
             be applied to one side (src/dst) of transfer other side will use linear addressing*/
  uint8_t     circDir;
}DmaUtilsAutoInc3d_TransferCirc;

/**
 *  \brief   This structure specifies the input and output pointers for the
 *          transfer
 */
typedef struct
{
  /** Pointer to memory buffer for the source */
  uint8_t    *srcPtr;
  /** Pointer to memory buffer for the destination */
  uint8_t    *dstPtr;
  /** Optional pointer to memory buffer for the secondary TR */
  uint8_t    *strPtr;
  /** Optional pointer to CDB table first entry for decompression */
  uint8_t    *cdbPtr;
}DmaUtilsAutoInc3d_IOPointers;



/**
 *  \brief   The structure specifies the compression parameters for the transfer
 */
typedef struct
{
  /** Type of compression to use. Please refer to DmaUtilsAutoInc3d_CmprsType */
  uint8_t     cmpAlg;
  /** 8-bit bias value for all compression types other than variable-k signed exponential golomb */
  uint8_t     cmpBias;
  /** initial k value for variable-k signed exponential golomb */
  uint8_t     varKStartK;
  /** k value update method for variable-k signed exponential golomb.
        Please refer to DmaUtilsAutoInc3d_UpdateKType for valid values */
  uint8_t     varKUpdateK;
  /** Element size for variable-k signed exponential golomb.
        Please refer to DmaUtilsAutoInc3d_CmpElemType for valid values */
  uint8_t     varKElemSize;
  /** Pixel subtraction scheme for variable-k signed exponenetial golomb.
        Please refer to DmaUtilsAutoInc3d_CmpSubType for valid values */
  uint8_t     varKSubType;
  /** Pixel subtraction scheme 0 for variable-k signed exponenential golomb.
       Please refer to DmaUtilsAutoInc3d_CmpSubSelType for valid values */
  uint8_t     varKSubSel0;
  /** Pixel subtraction scheme 1 for variable-k signed exponenential golomb.
       Please refer to DmaUtilsAutoInc3d_CmpSubSelType for valid values */
  uint8_t     varKSubSel1;
  /** Pixel subtraction scheme 2 for variable-k signed exponenential golomb.
       Please refer to DmaUtilsAutoInc3d_CmpSubSelType for valid values */
  uint8_t     varKSubSel2;
  /** Pixel subtraction scheme 3 for variable-k signed exponenential golomb.
       Please refer to DmaUtilsAutoInc3d_CmpSubSelType for valid values */
  uint8_t     varKSubSel3;
  /** Total loop iteration count for level 0 (innermost) for a superblock*/
  uint16_t    sbIcnt0;
  /** Total loop iteration count for level 1 for a superblock*/
  uint16_t    sbIcnt1;
  /** Jump in bytes when moving from sbIcnt0 to sbIcnt1 must be multiple of 16*/
  uint32_t    sbDim1;
  /** Jump in bytes when moving from sbIcnt1 to sicnt0 (completing a superblock) */
  uint32_t    sDim0;

  //Changes for 1.0.6 start here:
  /** The destination dimension to use for DICNT0 in the original TR after each super block */
  uint32_t    dDim0;
  /** The addressing mode to use for the super block during ICNT0 is decrmenting */
  uint32_t    sbAM0;
  /** The addressing mode to use for the super block when SB_ICNT1 is decremented.*/
  uint32_t    sbAM1;

}DmaUtilsAutoInc3d_TransferCompression;


/**
 *  \brief   This structure specifies the properties of the transfer for
 *          auto increment usecase.
 *
 *  ===============================================================
 */
typedef struct
{
    /** Trasnfer sync boundary, Refer DmaUtilsAutoInc3d_SyncType for valid values */
    uint32_t syncType;
    /**  Formatting options for DMA transfer (compression, decompression, transposed, etc.)
           Please refer to DmaUtilsAutoInc3d_DfmtType for valid values */
    uint32_t dmaDfmt;
    /** Properties to describe transfer dimensions in terms of icnt's
          and dim's */
    DmaUtilsAutoInc3d_TransferDim transferDim;
    /** Properties describing circularity */
    DmaUtilsAutoInc3d_TransferCirc circProp;
    /** Optional properties describing compression */
    DmaUtilsAutoInc3d_TransferCompression cmpProp;
    /** Input and output pointers */
    DmaUtilsAutoInc3d_IOPointers ioPointers;
}DmaUtilsAutoInc3d_TransferProp;

/** ========================================================
 *
 *  \brief   This structure specifies the parameter to initialize
 *          auto increment related properties.
 *
 *  ===============================================================
 */

typedef struct
{
    /**  DRU queue number to which a particular channel should submit its
         transfers */
    uint8_t dmaQueNo;
    /**  Owner of the DRU. Refer DmaUtilsAutoInc3d_druOwner for valid values*/
    uint8_t druOwner;

}DmaUtilsAutoInc3d_ChannelInitParam;

typedef struct
{
    int32_t coreId;
    /**  DRU queue number to which a particular channel should submit its
         transfers */
    int32_t numChannels;
    /**  Owner of the DRU. Refer DmaUtilsAutoInc3d_druOwner for valid values*/
    int32_t contextSize;
    /** Level for debug messages */
    int32_t traceLogLevel;
    /** Call back Function pointer to Write Log*/
    int32_t(*DmaUtilsVprintf)(const char * format, va_list arg);
    /** Handle to the UDMA driver to be used for the utility. If user sets
    it to NULL then utility will use a default udma driver handle */
    void *                    udmaDrvHandle;
}DmaUtilsAutoInc3d_InitParam;


typedef struct
{
    /**  Allocated memory for TR descriptor, Use DmaUtilsAutoInc3d_getTrMemReq AP
    to know the actual size required for this memory*/
    uint8_t * trMem;
    /**  Size of the memory allocated for trMem*/
    int32_t trMemSize;
    /** Number of Transfer Records (TR's) that will be submitted to a particular channel */
    int32_t numTRs;
    /** channelId for which TR is prepared It is important to note
    that this id is a virtual id for channel and is not related to the actual channel
    used internally to do the transfer  */
    int32_t channelId;
}DmaUtilsAutoInc3d_TrPrepareParam;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Returns the size of dmautils context.
 *
 *  This function returns the size of the DMA UTILS autoincrement 3D handle. User is
 *  then expected to allocate this memory and provide it to DMA UTILS autoincrement 3D
 *  function as an input.
 *
 *  \param numChannels [IN] Number of channels required for a usecase
 *
 *  \return  Size of the DMA UTILS context in bytes
 */

int32_t DmaUtilsAutoInc3d_getContextSize(int32_t numChannels);
/**
 *  \brief     This function initializes the DMA UTILS context for autoincrement usecase.
 *
 *  This function initializes the DMA UTILS context for autoincrement usecase. Internally it allocates
 *   numChannels DRU channels for the use case
 *
 *  \param    autoIncrementContext [IN] Memory allocated by the user as per DmaUtilsAutoInc3d_getContextSize
 *                                                      API. Try to allocate this memory in the fastest available memory for optimal performance
 *
 *  \param    initParams [IN] Init params for the dmautils
 *
 *  \param    chInitParams [IN] Init parameter for each channel. This is an array and number of enteries should be same as numChannels
 *
 *
 *  \return \ref Udma_ErrorCodes
 *
 *  =======================================================
 */

 int32_t DmaUtilsAutoInc3d_init(void * autoIncrementContext , DmaUtilsAutoInc3d_InitParam * initParams, DmaUtilsAutoInc3d_ChannelInitParam chInitParams[]);


/**
 *
 *  \brief     This function returns the size of the TR descriptor required for the transfer configuration
 *            given by the user.
 *
 * \param numTRs [IN] Number of Transfer Records (TR's) that will be submitted to a particular channel
 *
 *  \return   Size of the TR descriptor in bytes.
 *
 *  =======================================================
 */
 int32_t DmaUtilsAutoInc3d_getTrMemReq(int32_t numTRs);

/**
 *
 *  \brief     This function will prepare a TR/ TR descriptor depending on the number of Tr's to be submitted
 *             on a particular channel.
 *             Note this function will use the addresses as given by the user without any conversion.
 *             If user wants to convert addresses from virtual to physical address they should use
 *             DmaUtilsAutoInc3d_convertTrVirtToPhyAddr API.
 *             Other way to do the same  is to call DmaUtilsAutoInc3d_convertToPhysicalAddr to
 *             explicitly convert the address in TR to physical address
 *
 *  \param trPrepParam [IN] Parameters required for preparing TR
 *
 *  \param transferProp [IN] Transfer properties of all the TR'sr which needs to be submitted to a single channel
 *
 *  \return \ref Udma_ErrorCodes
 *
 */
 int32_t DmaUtilsAutoInc3d_prepareTr(DmaUtilsAutoInc3d_TrPrepareParam * trPrepParam ,
                                            DmaUtilsAutoInc3d_TransferProp transferProp[]);

/**
 *
 *  \brief     This function will prepare a TR/ TR descriptor depending on the number of Tr's to be submitted
 *             on a particular channel. The only difference between this function and DmaUtilsAutoInc3d_prepareTr
 *             function is that this function will call UDMA driver's Udma_VirtToPhyFxn function if its not NULL
 *             to convert virtual address to physical address.
 *             Note that this function will convert the virtual addresses given to physical addresses if
 *             UDMA driver Udma_VirtToPhyFxn function is not NULL using the same function.
 *             If user is preparing TR without using this API then it is user's responsibity to provide
 *             the physical addresses or call DmaUtilsAutoInc3d_convertToPhysicalAddr API to covert the
 *             addresses given in TR to physical addresses.
 *
 *  \param autoIncrementContext [IN] Context allocated for dmautils usecase
 *
 *  \param trPrepParam [IN] Parameters required for preparing TR
 *
 *  \param transferProp [IN] Transfer properties of all the TR'sr which needs to be submitted to a single channel
 *
 *  \return \ref Udma_ErrorCodes
 *
 */
 int32_t DmaUtilsAutoInc3d_prepareTrWithPhysicalAddress(void * autoIncrementContext,
                                            DmaUtilsAutoInc3d_TrPrepareParam * trPrepParam ,
                                            DmaUtilsAutoInc3d_TransferProp transferProp[]);


/**
 *
 *  \brief     This function should be called whenever provides TR with virtual addresses of
 *             src and dst pointer. This function will convert the virtual addresses given to
 *             physical addresses using UDMA driver initialized function  Udma_VirtToPhyFxn.
 *             If UDMA driver Udma_VirtToPhyFxn function is NULL then this API results into
 *             no change
 *
 *  \param autoIncrementContext [IN] Context allocated for dmautils usecase
 *
 *  \param trPrepParam [IN] Parameters required for preparing TR
 *
 *  \param convertMask   [IN] Mask to inform which fields address to be converted. Refer DmaUtilsAutoInc3d_addrConvertMask
 *                           for its usage.
 *
 *  \return \ref Udma_ErrorCodes
 *
 */
int32_t DmaUtilsAutoInc3d_convertTrVirtToPhyAddr(void * autoIncrementContext,
                                            const DmaUtilsAutoInc3d_TrPrepareParam * trPrepParam ,
                                            uint32_t convertMask);


/**
 *
 *  \brief   This function configures autoincrement for a particular channel
 *
 *  \param autoIncrementContext [IN] Context allocated for dmautils usecase
 *
 *  \param channelId  [IN] Channel Id which needs to be configured. It is important to note
 *                      that this id is a virtual id for channel and is not related to the actual channel
 *                      used internally to do the transfer
 *
 *  \param trMem [IN] Populated TrMem after calling DmaUtilsAutoInc3d_prepareTr function. This API expects
 *                    all addresses to be physical memory addresses. This is automatically taken care if
 *                    user prepares TR using DmaUtilsAutoInc3d_prepareTr or user can call
 *                    DmaUtilsAutoInc3d_convertToPhysicalAddr to convert addresses to physical addresses.
 *
 *  \param numTr [IN] Number of Transfer Records (TR's) that will be submitted to a particular channel
 *
 *  \return \ref Udma_ErrorCodes
 *
 */
 int32_t DmaUtilsAutoInc3d_configure(void * autoIncrementContext, int32_t channelId, uint8_t * trMem, int32_t numTr);


/**
 *
 *  \brief   This function triggers transfer on a particular channel
 *
 *  \param autoIncrementContext [IN] Context allocated for dmautils usecase
 *
 *  \param  channelId [IN] Channel Id which needs to be triggered. It is important to note
 *                      that this id is a virtual id for channel and is not related to the actual channel
 *                      used internally to do the transfer
 *
 *  \return Number of triggered required to complete the full transfer
 *
 */
int32_t DmaUtilsAutoInc3d_trigger(void * autoIncrementContext, int32_t channelId);


/**
 *
 *  \brief   This function waits for completion transfer on a particular channel
 *
 *  \param autoIncrementContext [IN] Context allocated for dmautils usecase
 *
 *  \param channelId  [IN] Channel Id for which we need to wait for transfer completion. It is important to note
 *                      that this id is a virtual id for channel and is not related to the actual channel
 *                      used internally to do the transfer
 *
 *  \return Number of triggered required to complete the full transfer
 *
 */
void  DmaUtilsAutoInc3d_wait(void * autoIncrementContext, int32_t channelId);


/**
 *
 *  \brief   This function deconfigures autoincrement for a particular channel
 *
 *  \param autoIncrementContext [IN] Context allocated for dmautils usecase
 *
 *  \param channelId [IN] Channel Id which needs to be de-configured. It is important to note
 *                      that this id is a virtual id for channel and is not related to the actual channel
 *                      used internally to do the transfer
 *
 *  \param trMem [IN] Populated TrMem after calling DmaUtilsAutoInc3d_prepareTr function
 *
  * \param numTr [IN] Number of Transfer Records (TR's) that will be submitted to a particular channel
 *
 *  \return \ref Udma_ErrorCodes
 *
 */
int32_t DmaUtilsAutoInc3d_deconfigure(void * autoIncrementContext, int32_t channelId, const uint8_t * trMem, int32_t numTr);

/**
 *
 *  \brief   This function deinit autoincrement by releasing all the resorces allocated
 *
 *  \param autoIncrementContext [IN] Context allocated for dmautils usecase
 *
 *  \return   \ref  Udma_ErrorCodes
 *
 */

int32_t DmaUtilsAutoInc3d_deinit(void * autoIncrementContext);

int32_t DmaUitlsAutoInc3d_CompressSW(void* trMem);

#ifdef __cplusplus
}
#endif

/* @} */

#endif /*#define DMAUTILS_AUTOINCREMENT_3D_H_*/
