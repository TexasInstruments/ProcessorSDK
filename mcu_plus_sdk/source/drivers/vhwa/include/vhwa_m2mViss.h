/*
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
 *  \ingroup  DRV_VHWA_MODULE
 *  \defgroup DRV_VISS_MODULE VISS Module
 *            VISS Module
 *
 */
/**
 *  \ingroup  DRV_VISS_MODULE
 *  \defgroup DRV_VISS_MODULE_INTERFACE VISS Interface
 *            Interface file for VISS M2M FVID2 driver
 *
 *  @{
 */
/**
 *  \file vhwa_m2mViss.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *              configure / control VISS M2M driver
 *
 *          Typical Application for the VISS M2M driver is as shown below.
 *
 *          Vhwa_m2mVissInit
 *               ||
 *          Vhwa_m2mVissAllocSl2 (Optional, if not called, first open allocates)
 *               ||
 *          FVID2_Create
 *               ||
 *          sets VISS configuration
 *               ||
 *          FVID2_ProcessReq
 *               ||
 *          Wait for completion callback
 *               ||
 *          FVID2_GetProcessedReq
 *               ||
 *          FVID2_close
 *               ||
 *          FVID2_deInit
 */

#ifndef VHWA_M2M_VISS_H_
#define VHWA_M2M_VISS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <soc/vhwa_soc.h>
#include "include/rawfe_cfg.h"
#include "include/fcp_cfg.h"
#include "include/nsf4_cfg.h"
#include "include/h3a_cfg.h"
#include "include/glbce_cfg.h"
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
#include "include/cac_cfg.h"
#endif
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
#include <include/pcid_cfg.h>
#endif
#include <include/vhwa_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor VhwaM2mViss_Ioctls
 *  \name   Ioctls for the VISS memory to memory driver
 *  \brief  Input/Output control MACRO's for VISS memory to memory module
 *
 *  @{
 */
/**
 * \brief IOCTL for setting VISS configuration.
 *        This IOCTL is used to select the number input, outputs and
 *        any internal path related configuration, it has flags
 *        to enable/disable, which alters blanking requirement.
 *        It is not used for confiugring individual VISS sub-modules.
 *        Typically used just after the create to select inputs, outputs
 *        and paths. Refer to #Vhwa_M2mVissParams
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_VHWA_M2M_VISS_SET_PARAMS          (VHWA_IOCTL_M2M_VISS_IOCTL_BASE)

/**
 * \brief IOCTL for enabling error events and registering callbacks for the
 *        same. This IOCTL pointer to #Viss_ErrEventParams as input
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_VHWA_M2M_VISS_REGISTER_ERR_CB     (                              \
    IOCTL_VHWA_M2M_VISS_SET_PARAMS + 1U)

/**
 * \brief IOCTL for enabling and setting HTS limiter.
 *        This is used to slowed down the HTS by introducing clock cycles
 *        between internal start signals. Refer to #Vhwa_HtsLimiter
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_VHWA_M2M_VISS_SET_HTS_LIMIT       (                              \
    IOCTL_VHWA_M2M_VISS_REGISTER_ERR_CB + 1U)

/**
 * \brief IOCTL to sync start each module
 *        This IOCTL doesn't configure any register and only enable pipeline
 *        to start processing
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_VHWA_M2M_VISS_SYNC_START       (                              \
                IOCTL_VHWA_M2M_VISS_SET_HTS_LIMIT + 1U)


/**
 * \brief IOCTL for getting module's performance numbers
 *        for the last frame submitted.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_VHWA_M2M_VISS_GET_PERFORMANCE     (                           \
                IOCTL_VHWA_M2M_VISS_SYNC_START + 1U)

/**
 * \brief Get Config UDMA buffer information.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_VHWA_M2M_VISS_GET_BUFF_INFO     (                           \
				IOCTL_VHWA_M2M_VISS_GET_PERFORMANCE + 1U)

/**
 * \brief Set Config UDMA buffer information.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_VHWA_M2M_VISS_SET_BUFF_INFO     (                           \
				IOCTL_VHWA_M2M_VISS_GET_BUFF_INFO + 1U)

/**
 * \brief IOCTL for enabling Watchdog timer error events and registering
 *         callbacks for the same.
 */
#define IOCTL_VHWA_M2M_VISS_REGISTER_WDTIMER_ERR_CB                        \
                (IOCTL_VHWA_M2M_VISS_SET_BUFF_INFO + 1U)

/** \brief IOCTL to get PSA signature for all enabled outputs 
 *  \return  FVID2_SOK on success, else error code.
*/
#define VHWA_M2M_IOCTL_VISS_GET_PSA_SIGN         \
                                        (IOCTL_VHWA_M2M_VISS_REGISTER_WDTIMER_ERR_CB + 1U)

/**
 * \brief IOCTL for enabling/disabling periodic reconfiguration of VISS registers.
 *
 * This control command takes a single pointer to a uint32_t as its argument.
 * The value pointed to by the uint32_t should be set to enable (1) or disable (0)
 * the periodic reconfiguration of VISS registers.
 */
/** \brief IOCTL for enabling/disabling periodic reconfiguration of VISS regs */
#define VHWA_M2M_IOCTL_VISS_ENABLE_RECONFIG_REINIT_REG        \
                                    (VHWA_M2M_IOCTL_VISS_GET_PSA_SIGN + 1U)

/**
 * \brief IOCTL for enabling/disabling status register validation.
 *
 * This control command takes a single pointer to a uint32_t as its argument.
 * The value pointed to by the uint32_t should be set to enable (1) or disable (0)
 * the status register validation feature.
 */
/** \brief IOCTL for validating the status register */
#define VHWA_M2M_IOCTL_VISS_ENABLE_STATUS_REG_VALIDATE        \
                                    (VHWA_M2M_IOCTL_VISS_ENABLE_RECONFIG_REINIT_REG + 1U)

/**
 * \brief IOCTL command to retrieve the validation status of the status register.
 *
 * This IOCTL can be used to trigger the after frame process completion and fetch
 * the register validation current state or outcome.
 *
 * Usage:
 * - Enable status register validation using VHWA_M2M_IOCTL_VISS_ENABLE_STATUS_REG_VALIDATE
 * - Submit a frame processing request
 * - Call this IOCTL to validate status registers and get the result
 * - The IOCTL will accumulate the validation status in the instance object
 * - After validation, the enableStatusRegValidate flag is reset for the current frame
 *
 * Important Notes:
 * - This IOCTL MUST be called after frame processing if status validation is enabled
 * - Failure to call this IOCTL may result in continuous invocation of status update functions
 */
/** \brief IOCTL for getting the status of status register validation */
#define VHWA_M2M_IOCTL_VISS_VALIDATE_REG    \
                                    (VHWA_M2M_IOCTL_VISS_ENABLE_STATUS_REG_VALIDATE + 1U)

/**
 * \brief IOCTL for enabling/disabling configuration register validation.
 *
 * This control command takes a single pointer to a uint32_t as its argument.
 * The value pointed to by the uint32_t should be set to enable (1) or disable (0)
 * the configuration register validation feature.
 */
/** \brief IOCTL for validating the configuration register */
#define VHWA_M2M_IOCTL_VISS_ENABLE_CONFIG_REG_READBACK        \
                                    (VHWA_M2M_IOCTL_VISS_VALIDATE_REG + 1U)

/**
 * \brief IOCTL command to get the size required for configuration register readback.
 *
 * This IOCTL returns the size (in bytes) needed to store the configuration register readback data.
 */
/** \brief IOCTL for getting the size of configuration register readback */
#define VHWA_M2M_IOCTL_VISS_GET_READBACK_SIZE    \
                                    (VHWA_M2M_IOCTL_VISS_ENABLE_CONFIG_REG_READBACK + 1U)

/**
 * \brief IOCTL for setting the configuration register readback and golden pointers, and callback.
 *
 * This IOCTL allows the application to provide pointers to the configuration register readback buffer,
 * the golden reference buffer, and a callback function. The driver will use these pointers to store
 * the configuration register readback data and compare it with the golden reference after processing.
 * The argument should be a pointer to a Vhwa_M2mVissConfigRegMemParams structure, where:
 *   - readback_ptr: Pointer to the buffer for configuration register readback data (must not be NULL).
 *   - golden_ptr: Pointer to the buffer containing the golden reference data (must not be NULL).
 *   - cbFxn: Callback function to be called after comparison (must not be NULL).
 * The buffer size for readback_ptr should be at least as large as returned by VHWA_M2M_IOCTL_VISS_GET_READBACK_SIZE.
 */
/** \brief IOCTL for setting configuration register readback pointer */
#define VHWA_M2M_IOCTL_VISS_SET_CONFIG_REG_PTRS    \
                                    (VHWA_M2M_IOCTL_VISS_GET_READBACK_SIZE + 1U)
                                        
/**
 * \brief IOCTL for getting VISS SL2 allocation information.
 *        This IOCTL retrieves the start address and size of allocated SL2 memory.
 *        Takes pointer to Vhwa_M2mVissSl2Info as output parameter.
 * \return  FVID2_SOK on success, else error code.
 */
#define VHWA_M2M_IOCTL_VISS_GET_SL2_INFO      \
                                                    (VHWA_M2M_IOCTL_VISS_SET_CONFIG_REG_PTRS + 1U)
/** @} */

/**
 *  \anchor VhwaM2mViss_InstanceId
 *  \name VHWA VISS Instance ID 
 *
 *  VHWA VPAC VISS Instance ID's
 *
 *  @{
 */
#if defined (SOC_J784S4) || defined (SOC_J742S2)
    /** \brief VPAC 0 VISS Instance 0  */
    #define VHWA_M2M_VPAC_0_VISS_DRV_INST_ID_0             (0U)
    /** \brief VPAC 1 VISS Instance 0  */
    #define VHWA_M2M_VPAC_1_VISS_DRV_INST_ID_0             (1U)
    /** \brief Total number of VISS instances */
    #define VHWA_M2M_VISS_DRV_NUM_INST                     (VHWA_M2M_VPAC_1_VISS_DRV_INST_ID_0 + 1U)
#else
    /** \brief VPAC 0 VISS Instance 0  */
    #define VHWA_M2M_VPAC_0_VISS_DRV_INST_ID_0             (0U)
    /** \brief Total number of VISS instances */
    #define VHWA_M2M_VISS_DRV_NUM_INST                     (VHWA_M2M_VPAC_0_VISS_DRV_INST_ID_0 + 1U)
#endif
/* @} */

/**
 *  \brief Default VISS Driver Instance ID [For backward compatibility]
 */
#define VHWA_M2M_VISS_DRV_INST0                 (VHWA_M2M_VPAC_0_VISS_DRV_INST_ID_0)

/**
 *  \brief Maximum VISS driver Instances [For backward compatibility]
 */
#define VHWA_M2M_VISS_DRV_MAX_INST              (VHWA_M2M_VISS_DRV_NUM_INST)

/**
 * \brief Max Number of handles supported  for each instance of VISS M2M Driver
 */
#define VHWA_M2M_VISS_MAX_HANDLES               (8u)

/*
 *  \brief Index for input channel0, ie companded/V short input
 */
#define VHWA_M2M_VISS_INPUT_IDX0                (0U)
/*
 *  \brief Index for input channel1, ie short input
 */
#define VHWA_M2M_VISS_INPUT_IDX1                (1U)
/*
 *  \brief Index for input channel2, ie long input
 */
#define VHWA_M2M_VISS_INPUT_IDX2                (2U)
/*
 *  \brief Max Input Channels
 */
#define VHWA_M2M_VISS_MAX_INPUTS                (3U)


/**
 *  \anchor VhwaM2mViss_OutputIdx
 *  \name   Ioctls for the VISS memory to memory driver
 *  \brief  Input/Output control MACRO's for VISS memory to memory module
 *
 *  @{
 */
#define VHWA_M2M_VISS_OUT_YUV420_12B_IDX        (0U)
#define VHWA_M2M_VISS_OUT_GREY_12B_IDX          (0U)
#define VHWA_M2M_VISS_OUT_COLOR_12B_IDX         (0U)
#define VHWA_M2M_VISS_OUT_LUMA_12B_IDX          (0U)
#define VHWA_M2M_VISS_OUT_YUV422_12B_IDX        (0U)
#define VHWA_M2M_VISS_OUT_RAW_IR_8B_OPT0        (0U)
#define VHWA_M2M_VISS_OUT_RAW_IR_12B_OPT0       (0U)
#define VHWA_M2M_VISS_OUT_CFA_C1_12B_IDX        (1U)
#define VHWA_M2M_VISS_OUT_CHROMA_12B_IDX        (1U)
#define VHWA_M2M_VISS_OUT_RAW_IR_8B_OPT1        (1U)
#define VHWA_M2M_VISS_OUT_RAW_IR_12B_OPT1       (1U)
#define VHWA_M2M_VISS_OUT_YUV420_8B_IDX         (2U)
#define VHWA_M2M_VISS_OUT_YUV422_8B_IDX         (2U)
#define VHWA_M2M_VISS_OUT_RGB888_8B_IDX         (2U)
#define VHWA_M2M_VISS_OUT_GREY_8B_IDX           (2U)
#define VHWA_M2M_VISS_OUT_COLOR_8B_IDX          (2U)
#define VHWA_M2M_VISS_OUT_CFA_C2_12B_IDX        (2U)
#define VHWA_M2M_VISS_OUT_LUMA_8B_IDX           (2U)
#define VHWA_M2M_VISS_OUT_RED_8B_IDX            (2U)
#define VHWA_M2M_VISS_OUT_RAW_IR_8B_OPT2        (2U)
#define VHWA_M2M_VISS_OUT_RAW_IR_12B_OPT2       (2U)
#define VHWA_M2M_VISS_OUT_RGI_8B_IDX            (2U)
#define VHWA_M2M_VISS_OUT_CFA_C3_12B_IDX        (3U)
#define VHWA_M2M_VISS_OUT_CHROMA_8B_IDX         (3U)
#define VHWA_M2M_VISS_OUT_GREEN_8B_IDX          (3U)
#define VHWA_M2M_VISS_OUT_RAW_IR_8B_OPT3        (3U)
#define VHWA_M2M_VISS_OUT_RAW_IR_12B_OPT3       (3U)
#define VHWA_M2M_VISS_OUT_SATURA_8B_IDX         (4U)
#define VHWA_M2M_VISS_OUT_CFA_C4_12B_IDX        (4U)
#define VHWA_M2M_VISS_OUT_BLUE_8B_IDX           (4U)
#define VHWA_M2M_VISS_OUT_RAW_IR_8B_OPT4        (4U)
#define VHWA_M2M_VISS_OUT_RAW_IR_12B_OPT4       (4U)
#define VHWA_M2M_VISS_OUT_H3A_IDX               (5U)
#define VHWA_M2M_VISS_OUT_RAW_IR_8B_OPT5        (5U)
#define VHWA_M2M_VISS_OUT_RAW_IR_12B_OPT5       (5U)
/*
 *  \brief Max Output Channels
 */
#define VHWA_M2M_VISS_MAX_OUTPUTS               (6U)
/** @} */

/**
 *  \anchor Vhwa_M2mVissInputMode
 *  \name   VISS input mode
 *  \brief  Decides the number of input channels required,
 *          essentially decides the merge mode.
 *
 *          Caution: This macro values are used for internal calculations.
 *
 *  @{
 */
/**< VISS Single Frame Input Mode */
#define VHWA_M2M_VISS_MODE_SINGLE_FRAME_INPUT   (1U)
/**< VISS Two Frame Merge Mode */
#define VHWA_M2M_VISS_MODE_TWO_FRAME_MERGE      (2U)
/**< VISS Three Frame Merge Mode */
#define VHWA_M2M_VISS_MODE_THREE_FRAME_MERGE    (3U)
/**< Max VISS Mode */
#define VHWA_M2M_VISS_MODE_MAX                  (4U)
/** @} */

/**
 *  \anchor Vhwa_M2mVissEdgeEnhancerMode
 *  \name   VISS Luma Interpolation type
 *  \brief  Enum to define Luma interpolation type. For all other components,
 *         i.e. chroma or bayer, bilinear interpolation is used.
 *         The following combinations are invalid:
 *         - Enabling EE on both Luma12 and Luma8 channels of a single FCP
 *         - Enabling EE on Luma12 channel of an FCP when output is NV12/Luma8
 *         - Enabling EE on Luma8 channel of an FCP when output is NV12_P12/Luma12
 *
 *  @{
 */
/**< Edge Enhancer is disabled */
#define VHWA_M2M_VISS_EE_DISABLE                (0x0U)
/**< Edge Enhancer is enabled on Luma12 bit output for FCP0 */
#define VHWA_M2M_VISS_EE_ON_FCP0_LUMA12         (0x1U)
/**< Edge Enhancer is enabled on Luma8 bit output for FCP0 */
#define VHWA_M2M_VISS_EE_ON_FCP0_LUMA8          (0x2U)
/**< VHWA_M2M_VISS_EE_ON_LUMA12 has been replaced by VHWA_M2M_VISS_EE_ON_FCP0_LUMA12 */
#define VHWA_M2M_VISS_EE_ON_LUMA12              VHWA_M2M_VISS_EE_ON_FCP0_LUMA12
/**< VHWA_M2M_VISS_EE_ON_LUMA8 has been replaced by VHWA_M2M_VISS_EE_ON_FCP0_LUMA8 */
#define VHWA_M2M_VISS_EE_ON_LUMA8               VHWA_M2M_VISS_EE_ON_FCP0_LUMA8
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
/**< Edge Enhancer is enabled on Luma12 bit output for FCP1 */
#define VHWA_M2M_VISS_EE_ON_FCP1_LUMA12         (0x4U)
/**< Edge Enhancer is enabled on Luma8 bit output for FCP1 */
#define VHWA_M2M_VISS_EE_ON_FCP1_LUMA8          (0x8U)
/**< VHWA_M2M_VISS_EE_ON_MV_LUMA12 has been replaced by VHWA_M2M_VISS_EE_ON_FCP1_LUMA12 */
#define VHWA_M2M_VISS_EE_ON_MV_LUMA12           VHWA_M2M_VISS_EE_ON_FCP1_LUMA12
/**< VHWA_M2M_VISS_EE_ON_MV_LUMA8 has been replaced by VHWA_M2M_VISS_EE_ON_FCP1_LUMA8 */
#define VHWA_M2M_VISS_EE_ON_MV_LUMA8            VHWA_M2M_VISS_EE_ON_FCP1_LUMA8
#endif
/**< Max value, used for error checking */
#define VHWA_M2M_VISS_EE_MAX                    (0xFFFFU)
/** @} */

#if defined (VHWA_VPAC_IP_REV_VPAC3L)
/**
 *  \anchor Vhwa_M2mVissIrOut
 *  \name   IR Out enabled/disable
 *  \brief  Enum to define possible values for IR out control
 *
 *  @{
 */
/**< Is NOT an IR output channel configuration */
#define VHWA_VISS_IROUT_DISABLED                (0u)
/**< Is NOT an IR output channel configuration */
#define VHWA_VISS_IROUT_ENABLED                 (1u)
/** @} */
#endif

#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
/**
 *  \anchor Vhwa_M2mVissPipeMode
 *  \name   VISS HV/MV Pipeline
 *  \brief  MACRO to use the MV pipeline and select input for MV pipeline
 *
 *  @{
 */
 /**< HV output pipeline */
#define VHWA_VISS_PIPE_HV                       (0u)

/**< MV output pipeline */
#define VHWA_VISS_PIPE_MV                       (1u)
#endif
#if defined VHWA_VPAC_IP_REV_VPAC3
/**< Use RFE output for MV pipeline */
#define VHWA_VISS_MV_PIPE_INPUT_RFE             (0u)
/**< Use NSF4 output for MV pipeline */
#define VHWA_VISS_MV_PIPE_INPUT_NSF4            (1u)
/**< Use GLBC output for MV pipeline */
#define VHWA_VISS_MV_PIPE_INPUT_GLBC            (2u)
/**< Use CAC output for MV pipeline */
#define VHWA_VISS_MV_PIPE_INPUT_CAC             (3u)
/** @} */
#endif
/**
 *  \anchor Viss_ErrorEvents
 *  \name   VISS Error Events
 *  \brief  Macros to define VISS Error events, for which VISS
 *          callback could be registered.
 *
 *          Caution: These macro values are directly used by driver
 *          for enabling events.
 *  @{
 */
/**<\brief Config read or write memory acccess occurred during functional operation and
 *        likely corrupted functional opertion. VISS merges all config error sources from
 *        RawFE and refer to RawFE spec for the entire error source list */
#define VHWA_VISS_RAWFE_CFG_ERR_INTR                 (0x1U)
/**<brief H3A AEW interrupt */
#define VHWA_VISS_RAWFE_AEW_PULSE_INTR               (0x2U)
/**<brief H3A AF interrupt */
#define VHWA_VISS_RAWFE_AF_PULSE_INTR                (0x4U)
/**<brief H3A interrupt */
#define VHWA_VISS_RAWFE_H3A_PULSE_INTR               (0x8U)
/**<brief H3A output buffer overflow */
#define VHWA_VISS_RAWFE_H3A_BUF_OVRFLOW_PULSE_INTR   (0x10U)
/**<brief VBUSP diagnostic read access of, while NSF data using RAM for functional purpose */
#define VHWA_VISS_NSF4V_LINEMEM_CFG_ERR_INTR         (0x20U)
/**<brief Horzontal Blanking too short between lines */
#define VHWA_VISS_NSF4V_HBLANK_ERR_INTR              (0x40U)
/**<brief Vertical Blanking too short between frames */
#define VHWA_VISS_NSF4V_VBLANK_ERR_INTR              (0x80U)
/**<brief Either non-shadowed registers written or statastic memories are accessed during active window */
#define VHWA_VISS_GLBCE_CFG_ERR_INTR                 (0x100U)
/**<brief GLBCE started filtering. This interrupt is issued at the rising edge of filtering signal */
#define VHWA_VISS_GLBCE_FILT_START_INTR              (0x200U)
/**<brief GLBCE ended filtering. This interrupt is issued at the falling edge of filtering signal */
#define VHWA_VISS_GLBCE_FILT_DONE_INTR               (0x400U)
/**<brief Generated when delayed HS/HE signals does not match with derived signals from GLBCE core.*/
#define VHWA_VISS_GLBCE_HSYNC_ERR_INTR               (0x800U)
/**<brief Generated when delayed VS/VE signals does not match with derived signals from GLBCE core */
#define VHWA_VISS_GLBCE_VSYNC_ERR_INTR               (0x1000U)
/**<brief This interrupt is issued, if there is a data input while filtering is high */
#define VHWA_VISS_GLBCE_VP_ERR_INTR                  (0x2000U)
/**<brief Either non-shadowed registers written or line memories are accessed during active window */
#define VHWA_VISS_FCFA_CFG_ERR_INTR                  (0x4000U)
/**<brief Configuration access to registers/memories has corrupted functional operation. 
 * Configuration read or write memory acccess occurred during functional operation.
 * Merged independent error sources at VISS. Refer to Section VISS Flexible Color
   Processing (FCP) for all sources. */
#define VHWA_VISS_FCC_CFG_ERR_INTR                   (0x8000U)
/**<brief FIFO overflow on FIFO for Y12 LSE I/F. Merged all FCC output overflow error
 * sources at VISS. Refer to Section VISS Flexible Color Processing (FCP) for all sources. */
#define VHWA_VISS_FCC_OUTIF_OVF_ERR_INTR             (0x10000U)
/**<brief Host was not able to read the entire histogram mem between VS-VE window
 * (triggered when the first access to histogram has been performed but the last has
 * not been performed). */
#define VHWA_VISS_FCC_HIST_READ_ERR_INTR             (0x20000U)
/**<brief Configuration happened to EE regions causing corruption during frame processing */
#define VHWA_VISS_EE_CFG_ERR                         (0x40000U)
/**<brief EE horizontal synchronization FIFO overflow interrupt */
#define VHWA_VISS_EE_SYNCOVF_ERR                     (0x80000U)
/**<brief LSE frame done interrupt */
#define VHWA_VISS_LSE_FR_DONE_EVT_INTR               (0x100000U)
/**<brief Set whenever there is an error response on VBUSM read command for any input channel. */
#define VHWA_VISS_LSE_SL2_RD_ERR_INTR                (0x200000U)
/**<brief Set whenever there is an error response on VBUSM write command for any output channel. */
#define VHWA_VISS_LSE_SL2_WR_ERR_INTR                (0x400000U)
/**<brief Set whenever one of the following input frame errors is detected at VPORT_INPUT */
#define VHWA_VISS_LSE_CAL_VP_ERR_INTR                (0x800000U)
/**<brief Output Frame Start (from VISS top level) */
#define VHWA_VISS_LSE_OUT_FR_START_EVT_INTR          (0x1000000U)
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined VHWA_VPAC_IP_REV_VPAC3L
/**<brief x,y pixel position has reached the start of rawfe pipeline interrupt*/
#define VHWA_VISS_RAWFE_X_Y_POINTER                  (0x2000000U)
/**<brief Config read/write access happened to CAC MeshLUT when CAC data is active 
 * (active window - IN_VS to OUT_VE)*/
#define VHWA_VISS_CR_CFG_ERR                         (0x4000000U)
/**<brief Triggered when the first access to statastics memory has been performed
 * but the last has not been performed*/
#endif
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
#define VHWA_VISS_RAWFE_DPC_STATS_READ_ERR           (0x8000000U)
#endif
/**< \brief Watchdog Timeout Error for VISS */
#define VHWA_VISS_WDTIMER_ERR                       (0x100000U)

/**< \brief Mask for VISS Stalling Error Interrupts
 *   Stalling errors cause the IP to stop processing and require CPU intervention.
 *   This mask includes only the error interrupts that result in IP stall:
 *   - ee_syncovf_err (bit 19)
 *   - fcc_outif_ovf_err_intr (bit 16)
 *   - glbce_vsync_err_intr (bit 12)
 *   - glbce_hsync_err_intr (bit 11)
 *   - nsf4v_vblank_err_intr (bit 7)
 *   - nsf4v_hblank_err_intr (bit 6)
 *   Note: Watchdog timer errors are handled separately and not included here.
 */
#define VHWA_VISS_STALLING_ERR_MASK                 ( \
    VHWA_VISS_EE_SYNCOVF_ERR | \
    VHWA_VISS_FCC_OUTIF_OVF_ERR_INTR | \
    VHWA_VISS_GLBCE_VSYNC_ERR_INTR | \
    VHWA_VISS_GLBCE_HSYNC_ERR_INTR | \
    VHWA_VISS_NSF4V_VBLANK_ERR_INTR | \
    VHWA_VISS_NSF4V_HBLANK_ERR_INTR)

/**< \brief Bitmask for all VISS Error Events */
#if defined(VHWA_VPAC_IP_REV_VPAC)
#define VHWA_VISS_ALL_ERR_EVENTS ( \
    VHWA_VISS_RAWFE_CFG_ERR_INTR | \
    VHWA_VISS_RAWFE_H3A_BUF_OVRFLOW_PULSE_INTR | \
    VHWA_VISS_NSF4V_LINEMEM_CFG_ERR_INTR | \
    VHWA_VISS_NSF4V_HBLANK_ERR_INTR | \
    VHWA_VISS_NSF4V_VBLANK_ERR_INTR | \
    VHWA_VISS_GLBCE_CFG_ERR_INTR | \
    VHWA_VISS_GLBCE_HSYNC_ERR_INTR | \
    VHWA_VISS_GLBCE_VSYNC_ERR_INTR | \
    VHWA_VISS_GLBCE_VP_ERR_INTR | \
    VHWA_VISS_FCFA_CFG_ERR_INTR | \
    VHWA_VISS_FCC_CFG_ERR_INTR | \
    VHWA_VISS_FCC_OUTIF_OVF_ERR_INTR | \
    VHWA_VISS_FCC_HIST_READ_ERR_INTR | \
    VHWA_VISS_EE_CFG_ERR | \
    VHWA_VISS_EE_SYNCOVF_ERR | \
    VHWA_VISS_LSE_SL2_RD_ERR_INTR | \
    VHWA_VISS_LSE_SL2_WR_ERR_INTR | \
    VHWA_VISS_LSE_CAL_VP_ERR_INTR \
)
#define VHWA_VISS_ALL_ERR_AND_INFO_EVENTS ( \
    VHWA_VISS_ALL_ERR_EVENTS | \
    VHWA_VISS_RAWFE_AEW_PULSE_INTR | \
    VHWA_VISS_RAWFE_AF_PULSE_INTR | \
    VHWA_VISS_RAWFE_H3A_PULSE_INTR | \
    VHWA_VISS_GLBCE_FILT_START_INTR | \
    VHWA_VISS_GLBCE_FILT_DONE_INTR | \
    VHWA_VISS_LSE_FR_DONE_EVT_INTR | \
    VHWA_VISS_LSE_OUT_FR_START_EVT_INTR \
)
#elif defined(VHWA_VPAC_IP_REV_VPAC3)
#define VHWA_VISS_ALL_ERR_EVENTS ( \
    VHWA_VISS_RAWFE_CFG_ERR_INTR | \
    VHWA_VISS_RAWFE_H3A_BUF_OVRFLOW_PULSE_INTR | \
    VHWA_VISS_NSF4V_LINEMEM_CFG_ERR_INTR | \
    VHWA_VISS_NSF4V_HBLANK_ERR_INTR | \
    VHWA_VISS_NSF4V_VBLANK_ERR_INTR | \
    VHWA_VISS_GLBCE_CFG_ERR_INTR | \
    VHWA_VISS_GLBCE_HSYNC_ERR_INTR | \
    VHWA_VISS_GLBCE_VSYNC_ERR_INTR | \
    VHWA_VISS_GLBCE_VP_ERR_INTR | \
    VHWA_VISS_FCFA_CFG_ERR_INTR | \
    VHWA_VISS_FCC_CFG_ERR_INTR | \
    VHWA_VISS_FCC_OUTIF_OVF_ERR_INTR | \
    VHWA_VISS_FCC_HIST_READ_ERR_INTR | \
    VHWA_VISS_EE_CFG_ERR | \
    VHWA_VISS_EE_SYNCOVF_ERR | \
    VHWA_VISS_LSE_SL2_RD_ERR_INTR | \
    VHWA_VISS_LSE_SL2_WR_ERR_INTR | \
    VHWA_VISS_LSE_CAL_VP_ERR_INTR | \
    VHWA_VISS_CR_CFG_ERR \
)
#define VHWA_VISS_ALL_ERR_AND_INFO_EVENTS ( \
    VHWA_VISS_ALL_ERR_EVENTS | \
    VHWA_VISS_RAWFE_AEW_PULSE_INTR | \
    VHWA_VISS_RAWFE_AF_PULSE_INTR | \
    VHWA_VISS_RAWFE_H3A_PULSE_INTR | \
    VHWA_VISS_GLBCE_FILT_START_INTR | \
    VHWA_VISS_GLBCE_FILT_DONE_INTR | \
    VHWA_VISS_LSE_FR_DONE_EVT_INTR | \
    VHWA_VISS_LSE_OUT_FR_START_EVT_INTR | \
    VHWA_VISS_RAWFE_X_Y_POINTER \
)
#elif defined(VHWA_VPAC_IP_REV_VPAC3L)
#define VHWA_VISS_ALL_ERR_EVENTS ( \
    VHWA_VISS_RAWFE_CFG_ERR_INTR | \
    VHWA_VISS_RAWFE_H3A_BUF_OVRFLOW_PULSE_INTR | \
    VHWA_VISS_NSF4V_LINEMEM_CFG_ERR_INTR | \
    VHWA_VISS_NSF4V_HBLANK_ERR_INTR | \
    VHWA_VISS_NSF4V_VBLANK_ERR_INTR | \
    VHWA_VISS_GLBCE_CFG_ERR_INTR | \
    VHWA_VISS_GLBCE_HSYNC_ERR_INTR | \
    VHWA_VISS_GLBCE_VSYNC_ERR_INTR | \
    VHWA_VISS_GLBCE_VP_ERR_INTR | \
    VHWA_VISS_FCFA_CFG_ERR_INTR | \
    VHWA_VISS_FCC_CFG_ERR_INTR | \
    VHWA_VISS_FCC_OUTIF_OVF_ERR_INTR | \
    VHWA_VISS_FCC_HIST_READ_ERR_INTR | \
    VHWA_VISS_EE_CFG_ERR | \
    VHWA_VISS_EE_SYNCOVF_ERR | \
    VHWA_VISS_LSE_SL2_RD_ERR_INTR | \
    VHWA_VISS_LSE_SL2_WR_ERR_INTR | \
    VHWA_VISS_LSE_CAL_VP_ERR_INTR | \
    VHWA_VISS_CR_CFG_ERR | \
    VHWA_VISS_RAWFE_DPC_STATS_READ_ERR \
)
#define VHWA_VISS_ALL_ERR_AND_INFO_EVENTS ( \
    VHWA_VISS_ALL_ERR_EVENTS | \
    VHWA_VISS_RAWFE_AEW_PULSE_INTR | \
    VHWA_VISS_RAWFE_AF_PULSE_INTR | \
    VHWA_VISS_RAWFE_H3A_PULSE_INTR | \
    VHWA_VISS_GLBCE_FILT_START_INTR | \
    VHWA_VISS_GLBCE_FILT_DONE_INTR | \
    VHWA_VISS_LSE_FR_DONE_EVT_INTR | \
    VHWA_VISS_LSE_OUT_FR_START_EVT_INTR | \
    VHWA_VISS_RAWFE_X_Y_POINTER \
)
#endif

/** @} */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 * \brief Prototype for the Error Event for VISS.
 *        The callback for the VISS error events can be registered
 *        using #Viss_ErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Viss_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Viss_ErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the Watchdog Timeout Error Event for VISS.
 *        The callback for the VISS Watchdog Timeout error events can be 
 *        registered using #Viss_WdTimerErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured,
 *                              Refer to \ref Viss_ErrorEvents for valid values.
 *
 * \return None.
 */
typedef void (*Viss_WdTimerErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 * \brief Prototype for the config Register Memory Compare Event for VISS.
 *        The callback for the VISS register memory compare events can be registered
 *        using #Vhwa_M2mVissConfigRegMemParams. One of the parameters in this
 *        is the callback. Driver calls this callback when the event occurs.
 *
 * \param handle                FVID2 driver handle, for which the event
 *                              has occurred.
 * \param configRegPrms         Pointer to configuration register parameters.
 * \param appData               Application-specific data pointer.
 *
 * \return                      Return FVID2_SOK on success, else error code.
 */
typedef int32_t (*Viss_ConfigRegCompareCbFxn)(Fvid2_Handle handle, void *configRegPrms);

/**
 *  struct Viss_ErrEventParams
 *  \brief Structure for error event parameters
 *         Used to register callback for the given set of events.
 */
typedef struct
{
    uint32_t                     errEvents;
    /**< bitmask of error events,
     *   Multiple error events can be registered with the same callback,
     *   Driver returns bitmask of events, which has occured,
     *   Refer to \ref Viss_ErrorEvents for valid values. */
    Viss_ErrEventCbFxn           cbFxn;
    /**< Callback function to be call on error events */
    void                         *appData;
    /**< private data */
} Viss_ErrEventParams;
/**
 *  struct Viss_WdTimerErrEventParams
 *  \brief WD Timer parameters
 *         Used to configure WD Timer parameters
 *         Must be called after SET_PARAMS ioctl
 *         Used with IOCTL_VHWA_M2M_VISS_CONFIG_WDTIMER ioctl
 */
typedef struct
{
    uint32_t                    wdTimeoutCycles;
    /**< Used to select timeout cycles
     *   Refer to #VHWA_WdTimeOutCycles for valid values */
    uint32_t                    wdTimerErrEvents;
    /**< Watchdog timer error event value,
     *   VHWA_VISS_WDTIMER_ERR registered with this callback
     *   Refer to \ref Viss_ErrorEvents for valid values. */
    Viss_WdTimerErrEventCbFxn   cbFxn;
    /**< Callback function to be call on error events */
    void                       *appData;
    /**< private data */
} Viss_WdTimerErrEventParams;
/**
 *  struct Vhwa_m2mVissInitParams
 *  \brief Init Parameters for VISS M2M Driver
 */
typedef struct
{
    Udma_DrvHandle              udmaDrvHndl;
    /**< UDMA driver handle, used for configuring UTC */
    Udma_DrvHandle              udmaDrvHndlBcdma;
    #if defined(MCU_PLUS_SDK)
    /**< UDMA driver handle, used for configuring BCDMA */
    Udma_DrvHandle              udmaDrvHndlVissCfgBcdma;
    #endif
    /**< UDMA driver handle, used for configuring BCDMA for VISS config */
    Vhwa_IrqInfo                irqInfo;
    /**< IRQ Information */
    bool 						configThroughUdmaFlag;
    /**< this should be enabled when configuration through UDMA is enabled */
    bool 						copyGlbceCtxThroughBcdmaFlag;
    /**< this should be enabled when copying GLBCE ctontext suing BCDMA */
} Vhwa_M2mVissInitParams;

typedef struct
{
    Vhwa_GetTimeStamp  getTimeStamp;
    /**< Function pointer to get timestamp */

    uint32_t                    enablePsa;
    /**< Flag to enable/Disble LSE PSA Module */
} Vhwa_M2mVissCreateArgs;

typedef struct
{
    /* Following parameters are used by the driver for the
       SL2 memory allocation */
    uint32_t                    maxInWidth[VHWA_M2M_VISS_MAX_INPUTS];
    /**< Maximum input image width,
     *   If set to 0, memory for that channel is not allocated.
     *   Depth and CCSF is common for all three input channels,
     *   But interface for max Input width is provided, so that
     *   internal memory can be allocated for less than 3 channels */
    uint32_t                    inCcsf;
    /**< Input image storage format,
     *   It is common for all input channels */
    uint32_t                    inDepth;
    /**< Buffer Depth
     *   It is common for all input channels */

    uint32_t                    maxOutWidth[VHWA_M2M_VISS_MAX_OUTPUTS];
    /**< Maximum input image width */
    uint32_t                    outCcsf[VHWA_M2M_VISS_MAX_OUTPUTS];
    /**< Output image storage format */
    uint32_t                    outDepth[VHWA_M2M_VISS_MAX_OUTPUTS];
    /**< Buffer Depth */
    uint32_t                    enable16KBAlignment;
    /**< Enable 16KB alignment for SL2 allocation
     *   TRUE: Allocate with 16KB address and size alignment (required for firewall)
     *   FALSE: Use default alignment without firewall support */
} Vhwa_M2mVissSl2Params;

typedef struct
{
    uint32_t                    enable;
    /**< Flag to enable/disble H3A output,
     *   Used it here, so that driver allocates/reserves resources for H3A
     *   AF/AEWB config can be set later on using SET_CONFIG ioctl.
     *   Request can be submitted only if H3A Set config is called
     *   when this flag is set. ie driver does not allow submitting request
     *   without setting h3a config, in case it is enabled here. */
    Fvid2_Format                fmt;
    /**< Output Format */
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined VHWA_VPAC_IP_REV_VPAC3L
    uint32_t                    vPipeline;
    /**< Vision pipeline: Human vision or Machine vision
            VHWA_VISS_PIPE_HV
            VHWA_VISS_PIPE_MV
     */
#endif
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
    uint32_t                    isIrOut;
    /**< Is this an IR output channel, check Vhwa_M2mVissIrOut */
#endif
} Vhwa_M2mVissOutputParams;

/**
 *  struct Vhwa_M2mVissParams
 *  \brief Connfiguration parameters of VISS.
 */
typedef struct
{
    uint32_t                    inputMode;
    /**< VISS Mode, decides the number of inputs,
     *   Refer \ref Vhwa_M2mVissInputMode for valid values */

    Fvid2_Format                inFmt;
    /**< Input Frame format, describing input frames storage format
     *   Used for configuring input frame format for all three inputs
     *   Frame size for all three inputs are used from this format
     *   Used in all VISS Mode \ref Vhwa_M2mVissInputMode
     *
     *   Following parametes are used from this
     *   width      = width of the input frame,
     *                max width supports is 4096 pixels
     *   height     = height of the input frame
     *   pitch      = line offset for the input data,
     *                used based on the data format
     *   dataFormat = Input DataFormat, only Bayer formats supports as of now
     *   sfcc       = Storage format
     *
     *   In case of multi-exposure input, the only parameter that can
     *   change among exposure is pitch/line offset. In this case
     *   pitch[n] is used to specify pitch of n exposures. */

    Vhwa_M2mVissOutputParams    outPrms[VHWA_M2M_VISS_MAX_OUTPUTS];
    /**< Output Frame Parameters, used to enable output and also to describe
     *   output frames storage format.
     *   Output format can be indexede using index \ref VhwaM2mViss_OutputIdx
     *   and be enabled in the output outPrms structure. the same instance has
     *   fvid2_format which describes the storage format for the
     *   corresponding output.
     *
     *   Following parametes are used from this
     *   width      = width of the output frame,
     *                must be same as input frame width
     *   height     = height of the input frame
     *                must be same as input frame height
     *   pitch      = line offset for the input data, used
     *                based on the data format
     *   dataFormat = DataFormat,
     *                Supported data formats
     *                  Output0: YUV420, Grey, RAW, Luma Only
     *                  Output1: RAW, Chroma Only
     *                  Output2: YUV420, YUV422, RGB, Grey, RAW, Luma only, RED
     *                  Output3: RAW, Chroma Only, Green
     *                  Output4: Saturation, RAW, BLUE
     *   sfcc       = Storage format
     *
     *   Not used for describing frame format for H3A output, H3A outputs
     *   8bit linear data. Driver internally ignores H3A output format */

    uint32_t                    enableGlbce;
    /**< Flag to include GLBCE in the pipeline */
    uint32_t                    enableNsf4;
    /**< Flag to include NSF4 in the pipeline */
    uint32_t                    edgeEnhancerMode;
    /**< Enables Edge Enhancer on Luma12 output,
     *   Mainly used in calculating vertical blanking,
     *   Refer \ref Vhwa_M2mVissEdgeEnhancerMode For valid values */
    uint32_t                    enableDpc;
    /**< Enable DPC
     *   Mainly used in calculating vertical blanking */
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
    uint32_t                    enableCac;
    /**< Flag to include CAC in the pipeline */
#endif
#if defined VHWA_VPAC_IP_REV_VPAC3
    uint32_t                    enableMVPipe;

    uint32_t                    mvPipeInSel;
    /**< MV pipe input select
     *      VHWA_VISS_MV_PIPE_INPUT_RFE
     *      VHWA_VISS_MV_PIPE_INPUT_NSF4
     *      VHWA_VISS_MV_PIPE_INPUT_GLBC
     *      VHWA_VISS_MV_PIPE_INPUT_GLBC */
    uint32_t                    fcp1ChromaMode;
#endif
    uint32_t                    fcp0ChromaMode;
    /**< Chroma Mode, 0: 420 1: 422
     *   Used only when CHROMA_ONLY output format selected.
     *   to determine chroma downsampling,
     *   Refer \ref Fcp_ChromaMode for valid values */
#if defined (VHWA_VPAC_IP_REV_VPAC3L)
    uint32_t                    enablePcid;
    /**< Flag to include PCID in the pipeline, TRUE includes it */
    uint32_t                    cfa_mode;
    /**< CFA pattern mode
     *   Refer \ref RFE_CFA_CFG_MODE for valid values */
#endif
    uint32_t                    enableGlbceSaveRestoreCtx;
    /**< Flag to include GLBCE Context save/restore in the pipeline */

} Vhwa_M2mVissParams;

/**
 * Structure contains the application buffer information to store
 * configuration, which will be written through UDMA.
 */
typedef struct
{
	/**< Buffer pointer passed from the application */
	uint32_t	*bufferPtr;
	/**< length of application buffer in number of bytes bytes */
	uint32_t	length;
	/**< configuration through UDMA is enabled or not */
	bool        configThroughUdmaFlag;
	/**< set to TRUE if cache policy of MPU for allocated buffer is cache write back */
	bool        isCacheWriteBackPolicy;
} Vhwa_M2mVissConfigAppBuff;

/**
 *  struct Vhwa_m2mVissPsaSign
 *  \brief Structure for storing PSA signatures
 */
typedef struct
{
    uint32_t psaSign[VHWA_M2M_VISS_MAX_OUTPUTS];
    /**< LSE PSA signature for all output channels */
} Vhwa_M2mVissPsaSign;

/**
 *  struct Vhwa_M2mVissSl2Info
 *  \brief Structure to hold VISS SL2 memory information
 */
typedef struct
{
    uint64_t    sl2Addr;
    /**< Start address of allocated SL2 memory */
    uint32_t    sl2Size;
    /**< Size of allocated SL2 memory in bytes */
} Vhwa_M2mVissSl2Info;

/**
 * \brief Configuration register memory parameters for ISO 26262 safety mechanism (VISS).
 *        Interface for providing golden and readback buffer pointers for register validation.
 */
typedef struct
{
    void *golden_ptr;
    /**< Pointer to the buffer containing the golden reference data (must not be NULL) */

    void *readback_ptr;
    /**< Pointer to the buffer for configuration register readback data (must not be NULL) */

    Viss_ConfigRegCompareCbFxn cbFxn;
    /**< Callback function to be called after comparison (must not be NULL) */

    uint32_t readback_mem_size;
    /**< Size of the readback buffer in bytes */

    void *appData;
    /**< Application-specific data pointer, passed to the callback */
} Vhwa_M2mVissConfigRegMemParams;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief This function should be used to initialize variable of type
 *          #Vhwa_M2mVissInitParams.
 *
 *  \param prms     A pointer of type Vhwa_M2mVissInitParams
 *
 *  \return         None
 */
static inline void Vhwa_m2mVissInitParamsInit(Vhwa_M2mVissInitParams *prms);

/**
 *  \brief This function should be used to initialize variable of type
 *          #Vhwa_M2mVissCreateArgs.
 *
 *  \param prms     A pointer of type #Vhwa_M2mVissCreateArgs
 *
 *  \return         None
 */
static inline void Vhwa_m2mVissCreateArgsInit(Vhwa_M2mVissCreateArgs *prms);

/**
 *  \brief This function should be used to initialize variable of type
 *          #Vhwa_M2mVissParams.
 *
 *  \param prms     A pointer of type #Vhwa_M2mVissParams
 *
 *  \return         None
 */
static inline void Vhwa_m2mVissParamsInit(Vhwa_M2mVissParams *prms);

/**
 *  \brief This function should be used to initialize variable of type
 *          #Vhwa_M2mVissSl2Params.
 *
 *  \param prms     A pointer of type #Vhwa_M2mVissSl2Params
 *
 *  \return         None
 */
static inline void Vhwa_m2mVissSl2ParamsInit(Vhwa_M2mVissSl2Params *prms);

/**
 *  \brief Function to initialize VISS with provided init parameters
 *
 *  \param initPrms         Init Parameters containing base address
 *                          and utc channel parameters
 *                          This parameter should not be 0.
 *  \return                 Returns 0 on success else returns error value
 */
int32_t Vhwa_m2mVissInit(Vhwa_M2mVissInitParams *initPrms);

/**
 *  \brief Function to deinitialize, cleans up the internal data structures
 */
void Vhwa_m2mVissDeInit(void);

/**
 *  \brief Function to allocate Sl2 memory for the output buffers.
 *
 *  \param sl2AllocPrms Pointer to a #Vhwa_M2mVissSl2Params structure
 *                      containing the SL2 allocation parameters
 *
 *  \return FVID2_SOK if successful, else suitable error code
 */
int32_t Vhwa_m2mVissAllocSl2(const Vhwa_M2mVissSl2Params *sl2AllocPrms);

/**
 *  \brief Function to free allocated SL2.
 *
 */
void Vhwa_m2mVissFreeSl2(void);

/**
 * \brief API for getting VISS SL2 allocation information.
 *        This API retrieves the start address and size of allocated SL2 memory.
 *        Takes pointer to Vhwa_M2mVissSl2Info as output parameter.
 *
 * \param sl2Info [OUT] Pointer to structure to store SL2 info
 *
 * \return  FVID2_SOK if successful
 *          FVID2_EBADARGS if sl2Info is NULL
 *          FVID2_EFAIL if SL2 memory is not allocated
 */
int32_t Vhwa_m2mVissGetSl2Info(Vhwa_M2mVissSl2Info *sl2Info);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static inline void Vhwa_m2mVissInitParamsInit(Vhwa_M2mVissInitParams *prms)
{
    if (NULL != prms)
    {
        prms->udmaDrvHndl = NULL;
        Viss_getIrqInfo(&prms->irqInfo);
        prms->configThroughUdmaFlag = false;
        prms->copyGlbceCtxThroughBcdmaFlag = false;
    }
}

static inline void Vhwa_m2mVissCreateArgsInit(Vhwa_M2mVissCreateArgs *prms)
{
    if (NULL != prms)
    {
        prms->getTimeStamp = NULL;
        prms->enablePsa = (uint32_t)UFALSE;
    }
}

static inline void Vhwa_m2mVissParamsInit(Vhwa_M2mVissParams *prms)
{
    if (NULL != prms)
    {
        prms->inputMode = VHWA_M2M_VISS_MODE_SINGLE_FRAME_INPUT;
        prms->inFmt.width = 1280U;
        prms->inFmt.height = 720U;
        prms->inFmt.pitch[0] = 1280U * 2U;
        prms->inFmt.dataFormat = FVID2_DF_RAW;
        prms->inFmt.ccsFormat = FVID2_CCSF_BITS12_UNPACKED16;

        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].enable = (uint32_t)UTRUE;
        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].fmt.width = 1280U;
        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].fmt.height = 720U;
        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].fmt.pitch[0U] = 1280U;
        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].fmt.pitch[1U] = 1280U;
        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].fmt.dataFormat =
            FVID2_DF_YUV420SP_UV;
        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].fmt.ccsFormat =
            FVID2_CCSF_BITS8_PACKED;
        #if defined (VHWA_VPAC_IP_REV_VPAC3L)
        prms->outPrms[VHWA_M2M_VISS_OUT_YUV420_8B_IDX].isIrOut = VHWA_VISS_IROUT_DISABLED;
        #endif
        prms->enableGlbce = (uint32_t)UFALSE;
        prms->enableNsf4 = (uint32_t)UFALSE;
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined (VHWA_VPAC_IP_REV_VPAC3L)
        prms->enableCac = (uint32_t)UFALSE;
#endif

#if defined (VHWA_VPAC_IP_REV_VPAC3L)
        prms->enablePcid = (uint32_t)FALSE;
#endif

        prms->edgeEnhancerMode = VHWA_M2M_VISS_EE_DISABLE;
        prms->enableDpc = (uint32_t)UFALSE;
        prms->fcp0ChromaMode = FCP_CHROMA_MODE_420;
#if defined (VHWA_VPAC_IP_REV_VPAC3)
        prms->fcp1ChromaMode = FCP_CHROMA_MODE_420;
#endif
    #if defined (VHWA_VPAC_IP_REV_VPAC3L)
        prms->cfa_mode = RFE_CFA_CFG_MODE_4;
    #endif
        prms->enableGlbceSaveRestoreCtx = (uint32_t)UFALSE;
    }
}

static inline void Vhwa_m2mVissSl2ParamsInit(Vhwa_M2mVissSl2Params *prms)
{
    uint32_t cnt;

    if (NULL != prms)
    {
        for (cnt = 0U; cnt < VHWA_M2M_VISS_MAX_INPUTS; cnt ++)
        {
            prms->maxInWidth[cnt] = 1920U;
        }
        prms->inCcsf = FVID2_CCSF_BITS12_UNPACKED16;
        prms->inDepth = 3U;
        for (cnt = 0U; cnt < VHWA_M2M_VISS_MAX_OUTPUTS; cnt ++)
        {
            if (VHWA_M2M_VISS_OUT_H3A_IDX == cnt)
            {
                prms->maxOutWidth[cnt] = 1024U;
            }
            else if (cnt == 4U)
            {
                prms->maxOutWidth[cnt] = 4096U;
            }
            else
            {
                prms->maxOutWidth[cnt] = 1920U;
            }

#if defined VHWA_VPAC_IP_REV_VPAC || defined (VHWA_VPAC_IP_REV_VPAC3L)
            if (cnt < VHWA_M2M_VISS_OUT_YUV420_8B_IDX)
            {
                prms->outCcsf[cnt] = FVID2_CCSF_BITS12_UNPACKED16;
            }
            else
            {
                prms->outCcsf[cnt] = FVID2_CCSF_BITS8_PACKED;
            }
#elif defined VHWA_VPAC_IP_REV_VPAC3
            prms->outCcsf[cnt] = FVID2_CCSF_BITS12_UNPACKED16;
#endif
            prms->outDepth[cnt] = 2U;
        }
    #if defined(VHWA_VPAC_IP_REV_VPAC) || defined(VHWA_VPAC_IP_REV_VPAC3)
    prms->enable16KBAlignment = (uint32_t)UTRUE;  /* 16KB alignment enabled by default (required for firewall support) */
    #else
    prms->enable16KBAlignment = (uint32_t)UFALSE;  /* VPAC3L does not support firewall */
    #endif
    }
}

#ifdef __cplusplus
}
#endif

#endif /* VHWA_M2M_VISS_H_ */

/* @} */
