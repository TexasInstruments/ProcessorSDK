/**
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 */

/**
 *  \file csl_lse.h
 *
 *  \brief CSL interface file for LSE module
 *
 */

#ifndef CSL_LSE_H_
#define CSL_LSE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/csl/csl_fvid2_dataTypes.h>
#include <ti/csl/cslr_lse.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Maximum number of input channels supported in LSE */
#define CSL_LSE_MAX_INPUT_CH            (2U)

/** \brief Maximum number of input channels supported in LSE */
#define CSL_LSE_MAX_OUTPUT_CH           (10U)

/** \brief Maximum number of input Buffers for each channel
           For VISS 3 exposure merge mode, three buffers are provided as
           inputs for all other cases, only singal buffer input is used. */
#define CSL_LSE_MAX_INPUT_BUF           (3U)

/** \brief Maximum number of region supported for BPr region */
#define CSL_LSE_BPR_MAX_REGIONS         (9U)

/** \brief LSE align mask */
#define CSL_LSE_ALIGN_MASK              (0x3FU)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 *  \anchor CSL_LseThreadId
 *  \name   LSE Thread ID
 *  \brief  Thread ID's for LSE modules
 *
 *  @{
 */
#define CSL_LSE_THREAD_ID_0             (0U)

#define CSL_LSE_THREAD_ID_1             (1U)
 /** @} */

/**
 *  struct CSL_LseInChConfig
 *  \brief Structure containing input channel configuration parameters
 */
typedef struct
{
    uint32_t                        enable;
    /**< Flag to enable/disable this channel */

    uint32_t                        bypass;
    /**< UTRUE: Do not perform enable/disable for the input channel
         UFALSE: Perform enable/disable for the input channel,
         Set to UFALSE for most of the HWA,
         For MSC, it set to UFALSE depending on which input is enabled */

    uint32_t                        vpHBlankCnt;
    /**< Number of HBlank Pixels to insert between active lines
         for internal vport interface to core.  (Max= 1023)
         Must be set to 0 if hblank insertion is not needed by accelerator */

    uint32_t                        knTopPadding;
    /**< Input kernel top padding lines
         Valid=0..4 for LSE,
         programmed in kern_tpad_sz */
    uint32_t                        knBottomPadding;
    /**< Input kernel bottom padding lines
         Valid=0..4 for LSE,
         programmed in kern_bpad_sz */
    uint32_t                        knLineOffset;
    /**< Input kernel starting line position
         Valid=0..4 for LSE,
         programmed in kern_ln_offset */
    uint32_t                        knHeight;
    /**< Actual number of input kernel lines (height)
         Valid=1..5 for LSE,
         Programmed in kern_sz_height */

    uint32_t                        enableAddrIncrBy2;
    /**< Source Line address Increment by 2 when enabled
         Otherwise incremented by 1 */

    Fvid2_ColorCompStorageFmt       ccsf;
    /**< Size of the pixel and its storage format,
         FVID2_BPP_BITS8, FVID2_BPP_BITS12, FVID2_BPP_BITS14,
         FVID2_BPP_BITS16
        0:  8-bit, 1: 12-bit, 2: 14-bit, 3: 16-bit */

    uint32_t                        frameWidth;
    /**< SL2 - Source Buffer Width (number of pixels) */
    uint32_t                        frameHeight;
    /**< SL2 - Source Buffer Height (number of lines) */

    uint32_t                        startOffset;
    /**< Buffer Line start offset within the first SL2 data word -
         in half-byte (nibble) resolution */
    uint32_t                        lineOffset;
    /**< Input buffer line offset, must be 64byte aligned,
         Lower 6bits must be 0 */
    uint32_t                        circBufSize;
    /**< SL2 Circular Buffer Size (number of line buffers) */
    uint32_t                        bufAddr[CSL_LSE_MAX_INPUT_BUF];
    /**< Input Buffer Address,
         Must be 64byte aligned */
    uint32_t                        numInBuff;
    /**< Number of output buffers, essentially, valid entries in bufAddr
         Maximum value is 3 and minimum is 1,
         2 and 3 are mainly used for VISS for 2 and 3 exposure read */

     uint32_t                      enableMultiTapKnlConfig;
     /* Enable to control the kernel tap parameter configuration */
} CSL_LseInChConfig;

/**
 *  struct CSL_LseOutChConfig
 *  \brief Structure containing output channel configuration parameters
 */
typedef struct
{
    uint32_t                        enable;
    /**< Flag to enable/disable this channel */

    uint32_t                        bypass;
    /**< UTRUE: Do not perform enable/disable for the input channel
         UFALSE: Perform enable/disable for the input channel,
         Set to UFALSE for most of the HWA,
         For MSC, it set to UFALSE depending on which input is enabled */

    uint32_t                        thrId;
    /**< Id of the input thread to which this output channel is mapped */

    Fvid2_ColorCompStorageFmt       ccsf;
    /**< Size of the pixel and its storage format,
         FVID2_BPP_BITS8, FVID2_BPP_BITS12, FVID2_BPP_BITS16 */

    uint32_t                        numInitOutSkip;
    /**< Line Out Initial Skip Count
         The number of initial HTS tstart/tdone cycles with no output
         from the core on this output channel.  The LSE will
         auto-generate the channel done status during these cycles. */

    uint32_t                        circBufSize;
    /**< SL2 Circular Buffer Size (number of line buffers) */
    uint32_t                        lineOffset;
    /**< Input buffer line offset, must be 64byte aligned,
         Lower 6bits must be 0 */

    uint32_t                        bufAddr;
    /**< output buffer address
         must be 64byte aligned */

    uint32_t                        enableYuv422Out;
    /**< enables YUV422 Interleaved Output Merge Enable
         UTRUE: YUV422 output enabled
         UFALSE: Disabled */
    Fvid2_DataFormat                yuv422DataFmt;
    /**< YUYV or UYUV format */

    uint32_t                        enableVertWrap;
    /**< Flag to enable Vertical wrap */

    uint32_t                        useMultiBprParams;
    /**< BPR Selection mode
         UTRUE: use regBpr for individual regions
         UFALSE: use bpr, commong blocks per row for all regions */
    uint32_t                        enableTDoneEoBPR;
    /**< Enable Tdone at end of BPR or for at the end of every block
         UTRUE: Generate Tdone only at the end of CBUF_BPR
         UFALSE: Generate Tdone on every 2D block completion */
    uint32_t                        bpr;
    /**< BPR value when useMultiBprParams is set to UFALSE
         otherwise ignored   */
    uint32_t                        regBpr[CSL_LSE_BPR_MAX_REGIONS];
    /**< Individual BPR value for each region
         Used only when useMultiBprParams is set to UTRUE
         */
} CSL_LseOutChConfig;

/**
 *  struct CSL_LseVportConfig
 *  \brief Structure containing V-Port parameters
 */
typedef struct
{
    uint32_t                        enable;
    /**< Flag to enable/disable VPORT input on LSE */
    uint32_t                        numPixels;
    /**< Number of pixels per vport cycles
         0: 1 Pixel
         1: 2 pixels */
    Fvid2_VideoIfWidth              vifw;
    /**< Vport Pixel Data Width Sel
         FVID2_VIFW_8BIT, FVID2_VIFW_12BIT, FVID2_VIFW_14BIT,
         FVID2_VIFW_16BIT
         Vport Pixels are always LSB aligned in 16-bit halfword */
    uint32_t                        enableProtocolFix;
    /**< Flag to enable/disable protocol fixer */
    uint32_t                        width, height;
} CSL_LseVportConfig;

/**
 *  struct CSL_LseH3aConfig
 *  \brief Structure containing H3A parameters
 */
typedef struct
{
    uint32_t                        enable;
    /**< Flag to enable/disable H3a output */
    uint32_t                        circBufSize;
    /**< SL2 Circular Buffer Size (number of H3A line buffers) */
    uint32_t                        lineOffset;
    /**< Size of H3A output line size in bytes for HTS Done generation
         (64 byte multiple) */
    uint32_t                        bufAddr;
    /**< output buffer address
         must be 64byte aligned */
} CSL_LseH3aConfig;

/**
 *  struct CSL_LseLpbkConfig
 *  \brief Structure containing paramters to configure Loopback
 */
typedef struct
{
    uint32_t                        enable;
    /**< Flag to enable/disable Loopback mode */
    uint32_t                        coreEnable;
    /**< Flag to enable/disable Core in loopback mode
         When enabled, the loopback-enabled input channel is used
         also for CORE data input. Otherwise, it is strictly used
         for the loopback path. */
    uint32_t                        inCh;
    /**< Input Channel Select for VISS loopback mode
         0:Ch0/Vport_in
         1:Ch1
         2:Ch2 */
} CSL_LseLpbkConfig;

/**
 *  struct CSL_LseConfig
 *  \brief VHWA LSE configuration,
 *         This structure is used for configuring entire LSE module,
 */
typedef struct
{
    uint32_t                     enableInChSyncOnFrame;
    /**< Input Channel Transfer Sync Mode (applicable only for VISS)
         UTRUE: Frame Mode
         UFALSE: Line Mode */
    uint32_t                     enableFixedArbMode;
    /**< VBUSM Arbitration Fixed Mode select
         UTRUE: Fixed-mode Arbitration
         UFALSE: Round-Robin Arbitration (default) */
    uint32_t                     enablePsa;
    /**< Enables PSA module
         UTRUE: Enable PSA
         UFALSE: Disable PSA */

    CSL_LseVportConfig           vpCfg;
    /**< Video Input Port Configuration,
         Used only when LSE is receiving data from CAL */
    CSL_LseH3aConfig             h3aCfg;
    /**< Output H3a Channel configuration */
    uint32_t                     numInCh;
    /**< Number of input channels,
         Valid Enteries in inChCfg
         Fixed for each HWA */
    CSL_LseInChConfig            inChCfg[CSL_LSE_MAX_INPUT_CH];
    /**< Input channel configuration */
    uint32_t                     numOutCh;
    /**< Number of output  channels,
         Valid Enteries in outChCfg
         Fixed for each HWA */
    CSL_LseOutChConfig           outChCfg[CSL_LSE_MAX_OUTPUT_CH];
    /**< Output Channel configuration */
    CSL_LseLpbkConfig            lpbkCfg;
    /**< Loopback configuration */
} CSL_LseConfig;

/**
 *  struct CSL_LseStatus
 *  \brief Struacture to get LSE status parameters
 */
typedef struct
{
    uint32_t                        numLpbkCh;
    /**< Number of available input channel selection for loopback mode */
    uint32_t                        isOutSkipEn;
    /**< Output Auto-Skip Enable */
    uint32_t                        is2DSupported;
    /**< 1D or 2D output addressing mode(2D if 1) */
    uint32_t                        outDataWidth;
    /**< Core Output Channel Data Width */
    uint32_t                        isLineSkipEn;
    /**< Source Line Inc by 2 Supported (if 1) */
    uint32_t                        isSrcNibOffsetEn;
    /**< Source nibble offset address Supported (if 1) */
    uint32_t                        isBlankInsertSupported;
    /**< H/VBLANK Insertion Supported (if 1) */
    uint32_t                        maxPixMatrixHeight;
    /**< _Input Pixel Matrix Height */
    uint32_t                        inputDataBusWidth;
    /**< Core Input Data Bus Width */
    uint32_t                        numOutH3aCh;
    /**< Number of SL2 H3A Output Channels */
    uint32_t                        numOutCh;
    /**< Number of SL2 Output Channels */
    uint32_t                        numInChPerThr;
    /**< Number of input channels per thread */
    uint32_t                        numVportInCh;
    /**< Number of VPORT input enabled */
    uint32_t                        numThrsSupported;
    /**< Number of Threads supported */
} CSL_LseStatus;

/**
 * \brief Holds LSE status registers.
 */
typedef struct {
    uint32_t DST_BUF_CFG_CH_STATUS[4U];
} CSL_DofLseStatusRead;

typedef struct {
     uint32_t DST_BUF_CFG_CH_STATUS[4U];
} CSL_SdeLseStatusRead;

typedef struct {
     uint32_t DST_BUF_CFG_CH_STATUS[2U][10U];
} CSL_MscLseStatusRead;

/**
 * \brief Holds LSE status registers for LDC safety mechanism.
 */
typedef struct {
    uint32_t DST_BUF_CFG_CH_STATUS[4U];
    /**< DST buffer configuration status for 4 channels */
} CSL_LdcLseStatusRead;

typedef struct {
    uint32_t DST_BUF_CFG_CH_STATUS[5U];
    /**< DST buffer configuration status for 5 channels */
} CSL_VissLseStatusRead;

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/**
 *  \brief Function to check the validity of the configuration
 *
 *  \param cfg              Pointer to structure containing the LSE register
 *                          configurations. Refer to #CSL_LseConfig
 *                          This parameter should be non-NULL.
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_lseCheckConfig(const CSL_LseConfig  *cfg);

/**
 *  \brief Funtion to set the entire LSE configuration to the LSE registers.
 *
 *  \param lseRegs          Pointer to structure containing LSE Base Address
 *  \param cfg              Pointer to structure containing the LSE register
 *                          configurations. Refer to #CSL_LseConfig
 *                          This parameter should be non-NULL.
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_lseSetConfig(CSL_lseRegs          *lseRegs,
                         const CSL_LseConfig  *cfg);

/**
 *  \brief Function to get the PSA singnature for the outputs
 *
 *  \param lseRegs          Pointer to structure containing LSE Base Address
 *  \param cfg              Pointer to structure containing the LSE register
 *                          configurations. Refer to #CSL_LseConfig
 *                          This parameter should be non-NULL.
 *  \param psaSign          Variable to store PSA signature
 *
 *  \return                 Returns 0 on success else returns error value
 */
void CSL_lseGetPsaSign(const CSL_lseRegs *lseRegs,
                       const CSL_LseConfig *cfg,
                       uint32_t psaSign[CSL_LSE_MAX_OUTPUT_CH]);

/**
 *  \brief Function to get the LSE core status
 *
 *  \param lseRegs          Pointer to structure containing LSE Base Address
 *  \param stats            Pointer to structure in which output
 *                          stats will be stored. Refer to #CSL_LseStatus
 *                          This parameter should be non-NULL.
 *
 *  \return                 NULL
 */
void CSL_lseGetCoreStatus(const CSL_lseRegs         *lseRegs,
                          CSL_LseStatus       *stat);

/**
 *  \brief Funtion to set Frame Size in the LSE registers.
 *         This is typically used in the YUV420 processing, where
 *         chroma height is half of luma height. In this case,
 *         Only frame size changes between two iterations of MSC operation.
 *
 *  \param lseRegs          Pointer to structure containing LSE Base Address
 *  \param cfg              Pointer to structure containing the LSE register
 *                          configurations. Refer to #CSL_LseConfig
 *                          This parameter should be non-NULL.
 *
 *  \return                 Returns 0 on success else returns error value
 */
int32_t CSL_lseSetUpdateConfig(CSL_lseRegs          *lseRegs,
                               CSL_LseConfig  *cfg);

void CSL_lseStopChannels(CSL_lseRegs *lseRegs, const CSL_LseConfig *cfg);

void CSL_lseStartChannels(CSL_lseRegs *lseRegs, const CSL_LseConfig *cfg);

/**
 *  \brief Function to initialize LSE configuration
 *
 *  \param cfg              LSE Configuration
 */
static inline void CSL_lseConfigInit(CSL_LseConfig *cfg);

/**
 *  \brief Function to align the pitch in the VHWA memory
 *
 *  \param pitch            Pitch to be aligned
 *
 *  \return                 Aligned pitch value
 */
static inline uint32_t CSL_lseMakePitchAligned(uint32_t pitch);

/* ========================================================================== */
/*                           Function Definition                              */
/* ========================================================================== */

static inline void CSL_lseConfigInit(CSL_LseConfig *cfg)
{
    uint32_t cnt;

    if (NULL != cfg)
    {
        (void)memset(cfg, 0x0, sizeof(CSL_LseConfig));
        cfg->enableInChSyncOnFrame = (uint32_t)UFALSE;
        cfg->enableFixedArbMode = (uint32_t)UFALSE;
        cfg->enablePsa = (uint32_t)UFALSE;

        cfg->vpCfg.enable = (uint32_t)UFALSE;
        cfg->h3aCfg.enable = (uint32_t)UFALSE;
        for (cnt = 0U; cnt < CSL_LSE_MAX_INPUT_CH; cnt ++)
        {
            cfg->inChCfg[cnt].enable = (uint32_t)UFALSE;
            cfg->inChCfg[cnt].bypass = UFALSE;
        }
        for (cnt = 0U; cnt < CSL_LSE_MAX_OUTPUT_CH; cnt ++)
        {
            cfg->outChCfg[cnt].enable = (uint32_t)UFALSE;
            cfg->outChCfg[cnt].bypass = UFALSE;
        }
        cfg->lpbkCfg.enable = (uint32_t)UFALSE;
    }
}

static inline uint32_t CSL_lseMakePitchAligned(uint32_t pitch)
{
    uint32_t tPitch;

    tPitch = (pitch + 63u) & 0xFFFFFFC0u;
    if(0u == (tPitch%256u))
    {
        tPitch += 64u;
    }
    return (tPitch);
}

#ifdef __cplusplus
}
#endif

#endif  /* CSL_LSE_H_ */
