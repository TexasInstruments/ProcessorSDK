/*
 *
 * Copyright (c) 2020 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _DISPARITY_MERGE_H
#define _DISPARITY_MERGE_H

#include <perception/perception.h>
#include <perception/algos/alg_common.h>
#include <perception/utils/api_memory_if.h>

/**
 * \defgroup group_ptk_algos_stereo_pp_dispmerge PTK Stereo Disparity Merge Postprocess
 * \ingroup group_ptk_algos
 *
 */

/**
 * \brief Stereo Post-Processing Disparity Merge algorithm - config Parameters
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */

typedef struct
{
    /** Width of low-resol disparity */
    int16_t       loWidth;

    /** Height of low-resol disparity */
    int16_t       loHeight;

    /** Width of high-resol disparity */
    int16_t       hiWidth;

    /** Height of high-resol disparity */
    int16_t       hiHeight;

    /** threshold of disparity difference for merge */
    int16_t       diffDispTh;

    /** Indicate whether low-resolution disparity has only disparity info without confidence */
    int8_t        loDispOnlyFlag;


} PTK_Alg_StereoPP_DisparityMerge_configParams;


/**
 * \brief Stereo algorithm - 1-D hole filling context
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */
typedef struct
{
    /** All configuation parameter for 1-D hole filling */
    PTK_Alg_StereoPP_DisparityMerge_configParams     cfgParams;

    /** upsampled disparity map from low-resol disparity map: */
    int16_t                                        * upDisparity;

    
} PTK_Alg_StereoPP_DisparityMergeObj;

/**
 * \brief Initialize disparity merge parameters
 *
 * \param [in]  cfg      configuration parameters for disparity merge
 * \param [in]  cntxt    hole filling algorithm context
 *
 * \return
 *
 *
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */
void PTK_Alg_StereoPP_DisparityMergeSetParams(PTK_Alg_StereoPP_DisparityMerge_configParams * cfg,
                                              PTK_Alg_StereoPP_DisparityMergeObj           * cntxt);

/**
 * \brief Computes the memory requirements based on the given algorithm
 *        configuration parameters.
 *
 * \param [in] cntxt Algorithm configuration parameters.
 *
 * \param [in] memReq Memory requirements as specified below
 *             - entry [0]: Memory for internal context. Best allocated from
 *                          the fastest memory.
 *             - entry [1]: Memory for internal point cloud. Best allocated from
 *                          the fastest memory.
 *             - entry [2]: Memory for the output buffer. The output buffer is
 *                          allocated by the Application so this entry is just
 *                          to communicate the output buffer size. No need to
 *                          pass the memory to the library in the init call
 *                          PTK_Alg_SfmOgmapInit().
 *
 * \return
 *        - PTK_ALG_RET_SUCCESS, if successful
 *        - < error code, otherwise
 *
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */
int32_t PTK_Alg_StereoPP_DisparityMergeConfig(PTK_Alg_StereoPP_DisparityMerge_configParams * cfg,
                                              PTK_Api_MemoryReq                            * memReq);

/**
 * \brief Initializes the library based on the given algorithm configuration
 *        parameters and allocated memory blocks.
 *
 * \param [in] allParams Algorithm configuration parameters.
 *
 * \param [in] memRsp    Memory blocks as specified below
 *
 * \return
 *        - valid handle, if successful
 *        - NULL, otherwise
 *
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */
PTK_AlgHandle PTK_Alg_StereoPP_DisparityMergeInit(PTK_Alg_StereoPP_DisparityMerge_configParams * cfg,
                                                  const PTK_Api_MemoryRsp                      * memRsp);

/**
 * \brief Get memory size
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memory block pointer
 *
 * \return
 *        - required memory size
 *
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */
int32_t PTK_Alg_StereoPP_DisparityMergeGetMem(PTK_Alg_StereoPP_DisparityMergeObj * cntxt,  uint8_t *memPtr);

/**
 * \brief Merge two disparity maps
 *
 * \param [in]  cntxt               algorithm handle
 * \param [in]  disparityLowBuffer  input lower-resolution disparity buffer
 * \param [in]  disparityHighBuffer input higher-resolution disparity buffer
 * \param [out] outDisparityBuffer  output disparity buffer
 * \param [in]  lowBufferStride     stride of lower-resolution disparity buffer
 * \param [in]  highBufferStride    stride of higher-resolution disparity buffer
 *
 * \return
 *
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */
void    PTK_Alg_StereoPP_mergeDisparity(PTK_Alg_StereoPP_DisparityMergeObj * cntxt,
                                        int16_t                            * disparityLowBuffer,
                                        int16_t                            * disparityHighBuffer,
                                        int16_t                            * outDisparityBuffer,
                                        int16_t                              lowBufferStride,
                                        int16_t                              highBufferStride);


/**
 * \brief Upsample low-resolution disparity
 *
 * \param [in]  lowDisparity    low-disparity disparity map
 * \param [out] upDisparity     upsampled disparity map
 * \param [in]  loWidth         low-resolution disparity width
 * \param [in]  loStride        low-resolution disparity stride
 * \param [in]  loHeight        low-resolution disparity height
 * \param [in]  hiWidth         upsampled disparity width
 * \param [in]  hiStride        upsampled disparity stride
 * \param [in]  hiHeight        upsampled disparity height
 * 
 * \return
 *
 * \ingroup group_ptk_algos_stereo_pp_dispmerge
 */
void    PTK_Alg_StereoPP_upsampleDisparity(int16_t * lowDisparity, 
                                           int16_t * upDisparity, 
                                           int16_t   loWidth, 
                                           int16_t   loStride,
                                           int16_t   loHeight,
                                           int16_t   hiWidth, 
                                           int16_t   hiStride,
                                           int16_t   hiHeight,
                                           int8_t    loDispOnlyFlag);


#endif // #ifndef _DISPARITY_MERGE_H
