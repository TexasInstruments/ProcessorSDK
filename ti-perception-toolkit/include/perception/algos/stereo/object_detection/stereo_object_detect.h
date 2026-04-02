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
#ifndef _STEREO_OBJECT_DETECT_H_
#define _STEREO_OBJECT_DETECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <perception/perception.h>
#include <perception/algos/alg_common.h>
#include <perception/algos/stereo/common_detection/stereo_algo.h>
#include <perception/utils/api_memory_if.h>
#include <perception/base/matrix.h>

/** Max number of detected obstacles */
#define MAX_DETECTIONS               40

/**
 * \brief Stereo algorithm - four corners of a detected obstacle bounding box
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct {
    /** Depth of an obstacle */ 
    float   depth;

    /** Bottom-left x pixel pos of an obstacle bounding box */
    int32_t p1x;

    /** Bottom-left y pixel pos of an obstacle bounding box */
    int32_t p1y;

    /** Bottom-right x pixel pos of an obstacle bounding box */
    int32_t p2x;
    
    /** Bottom-right y pixel pos of an obstacle bounding box */
    int32_t p2y;

    /** Top-left x pixel pos of an obstacle bounding box */
    int32_t p3x;

    /** Top-left y pixel pos of an obstacle bounding box */
    int32_t p3y;

    /** Top-right x pixel pos of an obstacle bounding box */
    int32_t p4x;

    /** Top-right y pixel pos of an obstacle bounding box */
    int32_t p4y;
} PTK_Alg_StereoAlgo_obsBox;


/**
 * \brief Stereo algorithm - drivable space structure
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct {
    /** Maximum drivable depth in front of ego vehicle */
    float   drivableDepth;

    /** Bottom-left x pixel pos of drivable space */
    int32_t p1x;

    /** Bottom-left y pixel pos of drivable space */
    int32_t p1y;

    /** Bottom-right x pixel pos of drivable space */
    int32_t p2x;

    /** Bottom-right y pixel pos of drivable space */
    int32_t p2y;

    /** Top-left x pixel pos of drivable space */
    int32_t p3x;

    /** Top-left y pixel pos of drivable space */
    int32_t p3y;

    /** Top-right x pixel pos of drivable space */
    int32_t p4x;

    /** Top-right y pixel pos of drivable space */
    int32_t p4y;
} PTK_Alg_StereoAlgo_driveSpace;


/**
 * \brief Stereo algorithm - line model for detected obstacle bounding box
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct{
    /** Linear line model y-intercept */
    float offset;

    /** Linear line model slope */
    float dx;
} PTK_Alg_StereoAlgo_lineModel;


/**
 * \brief Stereo algorithm - properties of a detected obstacles
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Disparities of two horizontal ends of an obstacle */
    float                                disparities[2];

    /** Depth of two horizontal ends of an obstacle */
    float                                depths[2];

    /** Bounding box heights corresponding to two horizontal ends of an obstacle */
    int32_t                              windowHeights[2];

    /** Lower y positions of two horizontal ends of an obstacle */
    int32_t                              bottomYs[2];

    /** x positions of two horizontal ends of an obstacle */
    int32_t                              xLocations[2];

    /** Area of an obstacle */
    int32_t                              originalDetectedArea;

    /** Flat that indicates whether it is an valid detection */
    int8_t                               isValid;

    /** Bounding box line model */
    PTK_Alg_StereoAlgo_lineModel         detectionLineModel;
} PTK_Alg_StereoAlgo_obsDetection;


/**
 * \brief Stereo algorithm - array of detected obstacels in a frame
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Pointer to the all detected obstacles */
    PTK_Alg_StereoAlgo_obsDetection * detections;

    /** The number of detected obstacles */
    int32_t numDetections;

} PTK_Alg_StereoAlgo_obsDetectionArray;


/**
 * \brief Stereo algorithm - closest obstacle parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Box height to find the closest obstacle */
    int32_t statisticsBoxHeight;

    /** Minimum box size to find the closest obstacle */
    int32_t minSearchBoxSize;

    /** Tuning parameter to find the closest obstacle */
    float   statisticsNearestPercent;

} PTK_Alg_StereoAlgo_closestObsParams;


/**
 * \brief Stereo algorithm - detected obstacle parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Tuning parameter 1 for obstacle detection */
    uint8_t maxHorizontalToSkip;

    /** Tuning parameter 2 for obstacle detection */
    uint8_t minHorizontalLength;

    /** Tuning parameter 3 for obstacle detection */
    float   windowHeightRatioRange[2];

    /** Tuning parameter 4 for obstacle detection */
    float   adaptiveTerminationPercentage;

    /** Tuning parameter 5 for obstacle detection 
      * Disparity with difference bigger than it is declared as new obstacle 
      */
    float   maxDisparityDifference;

    /** Tuning parameter 6 for obstacle detection */
    float   obsProbabilityMinDisparityToAccept;

} PTK_Alg_StereoAlgo_detectObsParams;


/**
 * \brief Stereo algorithm - obstacle merge parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Tuning parameter 1 for obstacle merge */ 
    int32_t depthThreshold;

    /** Tuning parameter 2 for obstacle merge */ 
    uint8_t maxHorizontalSkip;

    /** Tuning parameter 3 for obstacle merge */ 
    float   tiltThreshold;

    /** Tuning parameter 4 for obstacle merge */ 
    float   adaptiveTerminationPercentage;

} PTK_Alg_StereoAlgo_mergeObsParams;

/**
 * \brief Stereo algorithm - obstacle split parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Tuning parameter 1 for obstacle split */
    uint8_t minSplitComponentWidth;

    /** Tuning parameter 2 for obstacle split */
    float   splitInlierThreshold;

    /** Tuning parameter 3 for obstacle split */
    float   adaptiveTerminationPercentage;

} PTK_Alg_StereoAlgo_splitObsParams;


/**
 * \brief Stereo algorithm - all required parameters structure for obstacle detection
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Common stereo algorithm config parameters */
    PTK_Alg_StereoAlgo_configParams      odConfig;

    /** Camera parameters */
    PTK_Alg_StereoAlgo_camParams         camParams;

    /** Road parameters */
    PTK_Alg_StereoAlgo_roadParams        roadParams;

    /** Disparity parameters */
    PTK_Alg_StereoAlgo_dispParams        dispParams;
    
    /** Obstacle probability parameters */
    PTK_Alg_StereoAlgo_obsProbParams     copppParams;    

    /** Bottom of obstacle parameter */
    PTK_Alg_StereoAlgo_bottomObsParams   fbooParams;

    /** Closest obstacle parameters */
    PTK_Alg_StereoAlgo_closestObsParams  fcdParams;

    /** Detected obstacle parameters */
    PTK_Alg_StereoAlgo_detectObsParams   doParams;
   
    /** Obstacle merge parameters */
    PTK_Alg_StereoAlgo_mergeObsParams    moParams;

    /** Obstacle split parameters */
    PTK_Alg_StereoAlgo_splitObsParams    soParams;


} PTK_Alg_StereoAlgo_ObjectDetect_allParams;



/**
 * \brief Stereo algorithm - obstacle detection algorithm context
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** All configuation parameter for obstacle detection */
    PTK_Alg_StereoAlgo_ObjectDetect_allParams    allParams;

    /** Data mems needed for obstacle detection */
    PTK_Alg_StereoAlgo_obstacleDetetionMems      algoMemData;

    /** Pointer to ground model parameters array */ 
    PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr;

    /** Pointer to obstacle detection array */
    PTK_Alg_StereoAlgo_obsDetectionArray       * oda1;

    /** Pointer to obstacle detection array */
    PTK_Alg_StereoAlgo_obsDetectionArray       * oda2;

    /** Pointer to input image */
    uint8_t                                    * inputImage;

    /** Pointer to disparity confidence map */
    uint8_t                                    * dispConfidence;

    /** Pointer to disparity map. */
    int16_t                                    * disparity;

    /** Pointer to closest obstalce disparity in each column of an image */
    float                                      * closestDisparity;

    /** Initial guess of detected obtacle's height */
    int32_t                                    * closestHeightPrior;

    /** Refined detected obtacle's height */
    int32_t                                    * closestHeightComputed;

    /** Sorted disparit to find closest obstacle disparity  */
    float                                      * sortedDisparity;

    /** Data buffer to solve linear equation Ax + b */
    float                                      * dataBuffer[2];

    /** A matrix of linear equation Ax + b  */
    float                                      * A;

    /** A transpose matrix */
    float                                      * At;

    /** b vector of linear equation Ax + b  */
    float                                      * b;

    /** Pointer to inlier index */
    int16_t                                    * inlierIdx;

    /** Pointer to best inlier index */
    int16_t                                    * bestInlierIdx;

    /** Free space for each row of an image */
    int16_t                                    * freeDriveSpace;

} PTK_Alg_StereoAlgo_ObjectDetectObj;


/**
 * \brief Initialize obstacle detection parameters
 * 
 * \param [in]  allPararms      all configuration parameters for obstacle detection
 * \param [in]  cntxt           obstacle detection algorithm context
 *
 * \return 
 *        depth for an input disparity
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void PTK_Alg_StereoAlgo_ObjectDetectSetParams(PTK_Alg_StereoAlgo_ObjectDetect_allParams *  allPararms,
                                              PTK_Alg_StereoAlgo_ObjectDetectObj *         cntxt);


/**
 * \brief Computes the memory requirements based on the given algorithm
 *        configuration parameters.
 *
 * \param [in] cntxt        Algorithm configuration parameters.
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
int32_t PTK_Alg_StereoAlgo_ObjectDetectConfig(PTK_Alg_StereoAlgo_ObjectDetect_allParams  * cntxt,
                                              PTK_Api_MemoryReq                          * memReq);


/**
 * \brief Initializes the library based on the given algorithm configuration
 *        parameters and allocated memory blocks.
 *
 * \param [in] allParams Algorithm configuration parameters.
 * \param [in] memRsp    Memory blocks as specified below
 *
 * \return
 *        - valid handle, if successful
 *        - NULL, otherwise
 *
 * \ingroup group_ptk_algos_stereo_od
 */
PTK_AlgHandle PTK_Alg_StereoAlgo_ObjectDetectInit(PTK_Alg_StereoAlgo_ObjectDetect_allParams * allParams,
                                                  const PTK_Api_MemoryRsp                   * memRsp);



/**
 * \brief Get memory size
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memory block pointer
 *
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_ObjectDetectGetMem(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt,  uint8_t *memPtr);



/* PRIVATE APIS (not visible to OVX node) */

#ifdef __cplusplus
}
#endif

#endif
