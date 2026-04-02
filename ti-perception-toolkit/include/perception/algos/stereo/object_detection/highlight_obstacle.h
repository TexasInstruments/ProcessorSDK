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
#ifndef _HIGHLIGHT_OBSTACLE_H
#define _HIGHLIGHT_OBSTACLE_H

#include <perception/perception.h>
#include "perception/algos/stereo/object_detection/stereo_object_detect.h"

/**
 * \brief Correct perspectives of detected obstacles
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] inOda             detected obstacle array
 * \param [in] outOda            detected obstacle array after correcting perspective
 *
 * \return 
 *        Merged obstacles
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_computePerspectiveForDetection(PTK_Alg_StereoAlgo_ObjectDetectObj   * cntxt,        
                                                          PTK_Alg_StereoAlgo_obsDetectionArray * inOda,
                                                          PTK_Alg_StereoAlgo_obsDetectionArray * outOda);

/**
 * \brief Compute a line model
 * 
 * \param [in] cntxt                          algorithm handle
 * \param [in] startInd                       first horizontal pos of a line
 * \param [in] endInd                         last horizontal pos of a line           
 * \param [in] dsf                            downsampling factor
 * \param [in] inlierTh                       inlier threshold for RANSAC 
 * \param [in] adaptiveTerminationPercentage  detected obstacle array after correcting perspective
 *
 * \return 
 *        A line model
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
PTK_Alg_StereoAlgo_lineModel PTK_Alg_StereoAlgo_computeALineModel(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt,
                                                                  int32_t startInd, 
                                                                  int32_t endInd, 
                                                                  int8_t dsf, 
                                                                  float inlierTh, 
                                                                  float adaptiveTerminationPercentage);

/**
 * \brief Fit a line to compute a line model
 * 
 * \param [in] cntxt                          algorithm handle
 * \param [in] A                              A matrix in a linear equation Ax = b
 * \param [in] b                              b vector in a linear equation Ax = b           
 * \param [in] numData                        the number of data
 * \param [in] computeGram                    flag
 *
 * \return 
 *        A line model
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
PTK_Alg_StereoAlgo_lineModel PTK_Alg_StereoAlgo_fitModelOneLine(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt,
                                                                float *A, float *b, 
                                                                int32_t numData, int8_t computeGram);

/**
 * \brief Compute the number of inlier after fitting a line
 * 
 * \param [in] dataBuffer                     data used to fit a line
 * \param [in] inlierIdx                      inlier data index
 * \param [in] model                          line model
 * \param [in] numData                        number of data
 * \param [in] inlierTh                       inlier threshold
 *
 * \return 
 *        the number of inlier
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_getInliersOneLine(float *dataBuffer[2], 
                                             int16_t *inlierIdx,
                                             PTK_Alg_StereoAlgo_lineModel model, 
                                             int32_t numData, 
                                             float inlierTh);

/**
 * \brief split detected obstacles 
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] curOda            detected obstacle array
 * \param [in] newOda            detected obstacle array after merging obstacles
 *
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_mergeDetection(
        PTK_Alg_StereoAlgo_ObjectDetectObj   * cntxt,
        PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
        PTK_Alg_StereoAlgo_obsDetectionArray * newOda);

/**
 * \brief split detected obstacles 
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] curOda            detected obstacle array
 * \param [in] newOda            detected obstacle array after spliting obstacles
 *
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_splitDetection(
        PTK_Alg_StereoAlgo_ObjectDetectObj   * cntxt,
        PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
        PTK_Alg_StereoAlgo_obsDetectionArray * newOda);

/**
 * \brief Remove too far obstacles
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] curOda            detected obstacle array
 * \param [in] newOda            detected obstacle array after removing far obstacles
 *
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_removeFarDetection(
        PTK_Alg_StereoAlgo_ObjectDetectObj   * cntxt,
        PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
        PTK_Alg_StereoAlgo_obsDetectionArray * newOda);


/**
 * \brief Merge detected obstacles
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] curOda            detected obstacle array
 * \param [in] startIdx          index of the first obstacle to merge
 * \param [in] endIdx            index of the last obstacle to merge
 *
 * \return 
 *        Merged obstacles
 *  
 * \ingroup group_ptk_algos_stereo_od
 */
PTK_Alg_StereoAlgo_obsDetection PTK_Alg_StereoAlgo_createMergedDetection( 
        PTK_Alg_StereoAlgo_ObjectDetectObj   * cntxt,
        PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
        int32_t                                startIdx,
        int32_t                                endIdx);

/**
 * \brief Create a new detection object after spliting or merging obstacle 
 * 
 * \param [in] cntxt             algorithm handle
 * \param [in] model             line model 
 * \param [in] xBeginIdx         starting x position of an obstacle 
 * \param [in] xEndIdx           last x position of an obstacle
 *
 * \return 
 *        a new obstacle object
 *
 */
PTK_Alg_StereoAlgo_obsDetection PTK_Alg_StereoAlgo_getNewDetection(
        PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
        PTK_Alg_StereoAlgo_lineModel        model, 
        int                                 xBeginIdx, 
        int                                 xEndIdx);


#endif // #ifndef _HIGHLIGHT_OBSTACLE_H
