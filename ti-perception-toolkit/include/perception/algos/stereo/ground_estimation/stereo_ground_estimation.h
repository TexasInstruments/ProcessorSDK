/*
 *
 * Copyright (c) 2019 Texas Instruments Incorporated
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
#ifndef _STEREO_GROUND_ESTIMATION_H_
#define _STEREO_GROUND_ESTIMATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <perception/perception.h>
#include <perception/algos/alg_common.h>
#include <perception/algos/stereo/common_detection/stereo_algo.h>
#include <perception/utils/api_memory_if.h>
#include <perception/base/matrix.h>


/**
 * \brief Stereo algorithm - all required parameters structure for ground plane estimation
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Common stereo algorithm config parameters */
    PTK_Alg_StereoAlgo_configParams      config;

    /** Camera parameters */
    PTK_Alg_StereoAlgo_camParams         camParams;

    /** Road parameters */
    PTK_Alg_StereoAlgo_roadParams        roadParams;

    /** Disparity parameters */
    PTK_Alg_StereoAlgo_dispParams        dispParams;

    /** U-disparity parameters */
    PTK_Alg_StereoAlgo_uvDispParams      uvDispParams;

} PTK_Alg_StereoAlgo_GroundEstimation_allParams;


/**
 * \brief Stereo algorithm - ground plane estimation context
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** All configuation parameter for ground plane estimation */
    PTK_Alg_StereoAlgo_GroundEstimation_allParams allParams;

    /** Ground parameters */
    PTK_Alg_StereoAlgo_groundParams               groundParams;

    /** Ground model history array */
    PTK_Alg_StereoAlgo_groundModelHistory_Array   gmHistoryArray;

    /** Pointer to input image */
    uint8_t                                     * inputImage;

    /** Pointer to disparity confidence map */
    uint8_t                                     * dispConfidence;

    /** Pointer to disparity map. */
    int16_t                                     * disparity;

    /** Pointer to output disparity map after removing low-confidence disparities */
    int16_t                                     * outDisparity;

    /** down-sampled disparity pointer for ground plane estimation */
    float                                       * dsDisparity;

    /** Pointer to V-disparity map */
    int32_t                                     * vDisparity[NUM_GP_MODELS];

    /** Candidate GP disparity for every row */
    float                                       * candidateGP[NUM_GP_MODELS];

    /** Pointer to the ground plane model index for each column of an image */
    uint8_t                                     * gpModelIdx;

} PTK_Alg_StereoAlgo_GroundEstimationObj;


/**
 * \brief Initialize ground plane estimation parameters
 * 
 * \param [in]  allPararms      all configuration parameters for ground plane estimation
 * \param [in]  cntxt           ground plane estimation algorithm context
 *
 * \return 
 *        depth for an input disparity
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void PTK_Alg_StereoAlgo_GroundEstimationSetParams(
        PTK_Alg_StereoAlgo_GroundEstimation_allParams* allPararms,
        PTK_Alg_StereoAlgo_GroundEstimationObj       * cntxt);


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
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_GroundEstimationConfig(PTK_Alg_StereoAlgo_GroundEstimation_allParams * cfg,
                                                  PTK_Api_MemoryReq                             * memReq);


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
 * \ingroup group_ptk_algos_stereo_od
 */
PTK_AlgHandle PTK_Alg_StereoAlgo_GroundEstimationInit(PTK_Alg_StereoAlgo_GroundEstimation_allParams * allParams,
                                                     const PTK_Api_MemoryRsp                        * memRsp);



/**
 * \brief Get memory size
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memory block pointer
 *
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_GroundEstimationGetMem(PTK_Alg_StereoAlgo_GroundEstimationObj * cntxt,  uint8_t *memPtr);


/**
 * \brief Reset ground plane estimation parameters
 *
 * \param [in] cntxt Algorithm handle.
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_GroundEstimationReset(PTK_Alg_StereoAlgo_GroundEstimationObj * cntxt);


/* PRIVATE APIS (not visible to OVX node) */

#ifdef __cplusplus
}
#endif

#endif  // _STEREO_GROUND_ESTIMATION_H_
