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
#ifndef _MEDIAN_FILTER_H
#define _MEDIAN_FILTER_H

#include <perception/perception.h>
#include <perception/algos/alg_common.h>
#include <perception/utils/api_memory_if.h>

/**
 * \defgroup group_ptk_algos_stereo_pp_mf PTK Stereo Disparity Median Filter Postprocess
 * \ingroup group_ptk_algos
 *
*/


/**
 * \brief Stereo Post-Processing 2-D median filter algorithm - config Parameters
 * \ingroup group_ptk_algos_stereo_pp_mf
 */
typedef struct
{
    /** Width of input image */
    int16_t       width;

    /** Height of input image */
    int16_t       height;

    /** (square) median filter size */
    int16_t       filterSize;

} PTK_Alg_StereoPP_MedianFilter_configParams;


/**
 * \brief Stereo algorithm - 2-D median filter context
 * \ingroup group_ptk_algos_stereo_pp_mf
 */
typedef struct
{
    /** All configuation parameter for median filter */
    PTK_Alg_StereoPP_MedianFilter_configParams    cfgParams;

    int16_t                                     * buffer;

    int16_t                                     * tmpBuffer;

} PTK_Alg_StereoPP_MedianFilterObj;

/**
 * \brief Initialize median filter parameters
 *
 * \param [in]  cfg      configuration parameters for median filter
 * \param [in]  cntxt    median filter algorithm context
 *
 * \return
 *
 *
 * \ingroup group_ptk_algos_stereo_pp_mf
 */
void PTK_Alg_StereoPP_MedianFilterSetParams(PTK_Alg_StereoPP_MedianFilter_configParams * cfg,
                                            PTK_Alg_StereoPP_MedianFilterObj           * cntxt);

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
 * \ingroup group_ptk_algos_stereo_pp_mf
 */
int32_t PTK_Alg_StereoPP_MedianFilterConfig(PTK_Alg_StereoPP_MedianFilter_configParams * cfg,
                                            PTK_Api_MemoryReq                          * memReq);

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
 * \ingroup group_ptk_algos_stereo_pp_mf
 */
PTK_AlgHandle PTK_Alg_StereoPP_MedianFilterInit(PTK_Alg_StereoPP_MedianFilter_configParams * cfg,
                                                const PTK_Api_MemoryRsp                   * memRsp);

/**
 * \brief Get memory size
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memory block pointer
 *
 * \return
 *        - required memory size
 *
 * \ingroup group_ptk_algos_stereo_pp_mf
 */
int32_t PTK_Alg_StereoPP_MedianFilterGetMem(PTK_Alg_StereoPP_MedianFilterObj * cntxt,  uint8_t *memPtr);

/**
 * \brief Run 2-D median filter
 *
 * \param [in]  cntxt               algorithm handle
 * \param [in]  disparityBuffer     input disparity buffer
 * \param [out] outDisparityBuffer  output disparity buffer
 * \param [int] stride              stride of disparity buffer
 *
 * \return 
 *
 * \ingroup group_ptk_algos_stereo_pp_mf
 */
void    PTK_Alg_StereoPP_runMedianFilter(PTK_Alg_StereoPP_MedianFilterObj * cntxt,
                                        int16_t * disparityBuffer,
                                        int16_t * outDisparityBuffer,
                                        int16_t   stride);



#endif // #ifndef _MEDIAN_FILTER_H
