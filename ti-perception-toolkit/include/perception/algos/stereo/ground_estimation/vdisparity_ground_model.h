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
#ifndef _VDISPARITY_GROUND_MODEL_H
#define _VDISPARITY_GROUND_MODEL_H

#include <perception/perception.h>
#include "perception/algos/stereo/ground_estimation/stereo_ground_estimation.h"

/**
 * \brief Main function to estimat v-disparity based ground plane models
 * 
 * \param [in]  cntxt            algorithm handle
 * \param [out] gmParamsArr      ground plane model array
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_computeVDisparityGroundModel(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt,
                                                        PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr);



    // estimate ground plane model from V disparity

/**
 * \brief Get disparities corresponding to ground plane
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] numGPDisparity    the number of disparities corresponding to ground
 * \param [in] totalNumPixels    the number of pixels corresponding to ground
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_updateCandidateGPDisparity(PTK_Alg_StereoAlgo_GroundEstimationObj * cntxt,
                                                      int32_t * numGPDisparity, 
                                                      int32_t * totalNumPixels);


/**
 * \brief Get disparities corresponding to ground plane
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] gmParamsArr       ground plane model array
 * \param [in] numGPDisparity    the number of disparities corresponding to ground
 * \param [in] totalNumPixels    the number of pixels corresponding to ground
 * 
 * \return 
 *        - 0, if successful
 *        - < error code, otherwise
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_modelGroundPlane(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt, 
                                            PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
                                            int32_t                                    * numGPDisparity, 
                                            int32_t                                    * totalNumPixels);

/**
 * \brief Update ground model history 
 * 
 * \param [in]  cntxt            algorithm handle
 * \param [in] gmParamsArr      ground plane model array
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_updateGroundPlaneModelHistory(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt,
                                                         PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr);


/**
 * \brief Comput the final ground plane model by taking account for previous models 
 * 
 * \param [in]  cntxt            algorithm handle
 * \param [in] gmParamsArr      ground plane model array
 * \param [in] numGPDisparity    the number of disparities corresponding to ground
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_averageGrounPlaneModel(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt, 
                                                  PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
                                                  int32_t * numGPDisparity);



#endif // #ifndef _VDISPARITY_GROUND_MODEL_H
