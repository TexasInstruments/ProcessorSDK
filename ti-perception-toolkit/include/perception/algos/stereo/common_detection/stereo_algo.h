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
#ifndef _STEREO_ALGO_H_
#define _STEREO_ALGO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <perception/perception.h>
#include <perception/algos/alg_common.h>
#include <perception/utils/api_memory_if.h>
#include <perception/base/matrix.h>


/**
 * \defgroup group_ptk_algos_stereo_od PTK Streo Algorithm for Obstacle / Freespace detection
 * \ingroup group_ptk_algos
 *
 */


#define AUX_CODE_REMOVE              0

#define TEST_MAX_FILE_PATH           (256u)
#define MAX_FRAME_WIDTH              (1920u)
#define MAX_FRAME_HEIGHT             (1080u)

#define ALGORITHM_PROCESS_OK         0
#define ALGORITHM_PROCESS_FAIL      -1

#define PI                           3.14159265358979f


#define MMAX(a,b) ((a > b) ? (a) : (b))
#define MMIN(a,b) ((a < b) ? (a) : (b))

/** The number of ground plane model for a frame.
 *  1: A whole image uses the same ground plane model
 *  2: Left half and right half of image has a separte model.
 */
#define NUM_GP_MODELS                1

#define NUM_FRAC_BITS                4


/**
 * \brief Stereo algorithm - config Parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Width of input image */
    int32_t       width;

    /** Height of input image */
    int32_t       height;

    /** Minimum disparity value */
    int32_t       minDisparity;
    
    /** Maximum disparity value */
    int32_t       maxDisparity;

    /** Disparity confidence threshold value. 
      * Disparity whose confience less than sde_confidence_threshold 
      * is not used in the algorihtm
      * Allowed: {0, 1, 2, ..., 7} 
      */
    int8_t        sde_confidence_threshold;

    /** Horizontal distortion center */
    float         distCenterX;

    /** Vertical distortion center */
    float         distCenterY;

    /** Camera focal length */
    float         focalLength;

} PTK_Alg_StereoAlgo_configParams;



/**
 * \brief Stereo algorithm - road Parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Bottom road center position (x, y) in an image */
    int32_t roadCenterBase[2];

    /** Top road center position (x, y) in an image */
    int32_t roadCenterTop[2];

    /** Width (in pixel) of road centered at roadCenterBase */
    int32_t roadSizeBase;

    /** Width (in pixel) of road centered at roadCenterTop */
    int32_t roadSizeTop;

    /** Max distance for detection in mm */
    int32_t maxDepth;

    /** ego vehicle's width in mm */
    int32_t egoSize;

} PTK_Alg_StereoAlgo_roadParams;


/**
 * \brief Stereo algorithm - road Parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Camera height from ground */
    int32_t        camHeight;

    /** Width of image */ 
    int32_t        width;

    /** Height of image */ 
    int32_t        height;

    /* Camera roll angle */
    float          camRoll;

    /* Camera pitch angle */
    float          camPitch;

    /* Camera yaw angle */
    float          camYaw;

    /* sine of camera pitch */
    float          sinPitch;

    /* cosine of camera pitch */
    float          cosPitch;

    /** Baseline distance in mm between left and right cameras */
    float          baseline;

    /** Horizontal distortion center */
    float          dcx;

    /** Vertical distortion center */
    float          dcy;

    /** Camera focal length */
    float          f;

    /** Camera instrinsic parameters */
    PTK_3D_Matrix  K;

    /** Inverse of camera intrinsic parameters */
    PTK_3D_Matrix  invK;

} PTK_Alg_StereoAlgo_camParams;


/**
 * \brief Stereo algorithm - disparity parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Disparity confidence threshold value
      * It is the same to PTK_Alg_StereoAlgo_configParams.sde_confidence_threshold
      */
    int8_t  confTh;

    /** Whether confidence data exist in 16-bit disparity data 
     */
    int8_t  existConf;

    /** Minimum disparity value 
      * It is the same to  PTK_Alg_StereoAlgo_configParams.minDisparity
      */
    int32_t minDisparity;

    /** Minimum disparity value 
      * It is the same to  PTK_Alg_StereoAlgo_configParams.maxDisparity
      */
    int32_t maxDisparity;

    /** The number of disparity, which is (maxDisparity - minDisparity + 1) */
    int32_t numDisparity;

    /** The number of bits used for fractional disparity */
    int32_t fracBits;

    /** Max distance for detection in mm 
      * It is the same to  PTK_Alg_StereoAlgo_roadParams.maxDepth
      */
    int32_t farZ;

} PTK_Alg_StereoAlgo_dispParams;

/**
 * \brief Stereo algorithm - uvDisparity parameters 
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Downsampling factor of disparity map
      * Downsampled disparity map is an input to ground plane estimation 
      */
    int8_t  dsFactor;

    /** Flag that indicates whether to use road params in estimating ground plane */
    uint8_t useRoadParams;

    /** Flat that indicates whether to use adaptive ground error for non-flat ground */
    uint8_t adaptiveGroundError;

    /** Minimum road width. 
      * Ground plane disparity should have more pixels than as many pixels as roadWidth
      */
    int32_t roadWidth;

    /** V-disparity map width */
    int32_t vdWidth;

    /** V-disparity map width */
    int32_t vdHeight;

    /** Starting horizontal pixel pos that is used for ground model estimation */
    int32_t startX;

    /** Last horizontal pixel pos that is used for ground model estimation */
    int32_t lastX;

    /** Starting vertical pixel pos that is used for ground model estimation */
    int32_t startY;

    /** Last vertical pixel pos that is used for ground model estimation */
    int32_t lastY;

    /** The number of RANSAC iteration for V-disparity modeling */
    int32_t ransacIter;

    /** RANSAC error threshold */
    float   ransacErrTh;

} PTK_Alg_StereoAlgo_uvDispParams;


/**
 * \brief Stereo algorithm - obstacle probability (per pixel) parameters 
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Downsampling factor of disparity map
      * Downsampled disparity map is an input to obstacle / freespace detection
      */
    uint8_t dsFactor;

    /** Dowsampled disparity width */
    int32_t dsWidth;

    /** Downsampled disparity height */
    int32_t dsHeight;

    /** search window height in mm to calculate obstacle probability */
    int32_t obsProbWindowHeight;

    /** search window width in mm to calculate obstacle probability */
    int32_t obsProbWindowWidth;

    /** Minimum size of search window in pixel to calculate obstacle probability */
    int32_t minSearchBoxSize;

    /** Pixel to check above horizon */
    int32_t pixelAboveHorizonToCheck;

    /** allowed ground disparity difference from model */
    float   minDisparityDifferenceFromModel;

} PTK_Alg_StereoAlgo_obsProbParams;

/**
 * \brief Stereo algorithm -  bottom of obstacle parameter
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** minimum probability of a pixel to be an obstacle pixel */
    float   minProbability;

    /** minimum consecutive number of obstacle pixels for a pixel 
      * to be deteremined as the starting pixel of an obstacle in a column
      */
    int32_t minConsecutiveVertical;

} PTK_Alg_StereoAlgo_bottomObsParams;


/**
 * \brief Stereo algorithm -  ground plane parameters
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** Pointer to the minimum number of groudn pixels at each disparity */
    float * gpThArray;

#if AUX_CODE_REMOVE
    float * groundErrArray[2];
#endif

    /** Pointer to y offset (variation) of ground plane at each disparity */
    float * yOffsetArray;

    /** Pointer to minimum obstacle height at each disparity */
    float * objHeightArray;

} PTK_Alg_StereoAlgo_groundParams;



/**
 * \brief Stereo algorithm -  V-disparity ground plane model histroy
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** History of slopes in the last 5 frames */
    float     slopeArray[5];

    /** History of slopes in the last 5 frames */
    float     interceptArray[5];
 
    /** The current frame number */
    int32_t   frameNo;

    /** Max number of previous frames to use 
      * Allowed: {0, 1, 2, 3, 4 5}
      */ 
    int32_t   maxFrameNo;

} PTK_Alg_StereoAlgo_groundModelHistory;


/**
 * \brief Stereo algorithm -  V-disparity ground plane model histroy array for muliple models
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** History of the ground plane models */
    PTK_Alg_StereoAlgo_groundModelHistory gmHistory[NUM_GP_MODELS];

    /** The number of ground plane model used */
    int8_t                                numModels;

} PTK_Alg_StereoAlgo_groundModelHistory_Array;


/**
 * \brief  Stereo algorithm -  V-disparity ground plane model
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct
{
    /** slope of the ground plaane model */
    float     slope;

    /** y-intercept of the ground plane model */
    float     intercept;

#if AUX_CODE_REMOVE
    float   * gpModelLine[4];
    int32_t * igpModel;
#endif

    /** Flag that indicates whether GP model is valid or not */
    int8_t    validGM;      

} PTK_Alg_StereoAlgo_groundModelParams;


/**
 * \brief  Stereo algorithm -  V-disparity ground plane model array for multiple models
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct {
    /** ground plane models */
    PTK_Alg_StereoAlgo_groundModelParams  gmParams[NUM_GP_MODELS];

    /** the number of ground plane models used */
    int8_t                                numModels;
} PTK_Alg_StereoAlgo_groundModelParams_Array;



/**
 * \brief  Stereo algorithm -  Data mems needed for algorithms
 * \ingroup group_ptk_algos_stereo_od
 */
typedef struct 
{
    /** Pointer to ground plane model index for each column of image */
    uint8_t                                 * gpModelIdx;

    /** Pointer to Downsampled disparity */
    float                                   * dsDisparity;

    /** The number of obstacle pixels in a search window centered at each pixel */
    int32_t                                 * candidateFound;

    /** The number of valid pixels in a search window centered at each pixel */
    int32_t                                 * numValidFound;

    /** The number of obstacle pixels in a vertical search range centered at each pixel */
    int32_t                                 * candidateFoundVertical;

    /** The number of obstacle pixels in a vertical search range centered at each pixel */
    int32_t                                 * numValidFoundVertical;

    /** Pointer to the probability for each pixel to be obstacle */
    float                                   * obstacleProbability;

    /** Pointer to the starting y pos of obstacle for each column */
    int32_t                                 * obtacleBottomLine;

} PTK_Alg_StereoAlgo_obstacleDetetionMems;


/**
 * \brief Compute probabilities of pixels to be obstacle 
 *        configuration parameters.
 *
 * \param [in]     copppParams obstacle probability params
 * \param [in]     camParams   camera params
 * \param [in]     gmParamsArr camera params
 * \param [in/out] algoMemData algorihtm data mem
 *
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_computeObstacleProbalilityPerPixel(
        PTK_Alg_StereoAlgo_obsProbParams           * copppParams,
        PTK_Alg_StereoAlgo_camParams               * camParams,
        PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
        PTK_Alg_StereoAlgo_obstacleDetetionMems    * algoMemData);

/**
 * \brief Compute probabilities of pixels to be obstacle 
 *        configuration parameters.
 *
 * \param [in]     copppParams obstacle probability params
 * \param [in]     camParams   camera params
 * \param [in]     gmParamsArr camera params 
 * \param [in/out] algoMemData algorihtm data mem
 * \param [in]     horizon     y position of horizon in an input image
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void PTK_Alg_StereoAlgo_computeObstacleProbabilityPerPixel_verticalLoop(
        PTK_Alg_StereoAlgo_obsProbParams           * copppParams,
        PTK_Alg_StereoAlgo_camParams               * camParams,     
        PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,   
        PTK_Alg_StereoAlgo_obstacleDetetionMems    * algoMemData, 
        int32_t horizon);

/**
 * \brief Find the closest y positions where obstacles are on every column of an image
 *
 * \param [in]     copppParams obstacle probability params
 * \param [in]     fbooParams  camera params 
 * \param [in/out] algoMemData algorihtm data mem 
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void  PTK_Alg_StereoAlgo_findBottomOfObstacle(
        PTK_Alg_StereoAlgo_obsProbParams        * copppParams,
        PTK_Alg_StereoAlgo_bottomObsParams      * fbooParams,
        PTK_Alg_StereoAlgo_obstacleDetetionMems * algoMemData);

/**
 * \brief Downsample input disparity map
 * 
 * \param [in]  config      algorihtm configuration params
 * \param [in]  copppParams obstacle probability params
 * \param [in]  disparity   input disparity map
 * \param [out] dsDisparity downsampled disparity map
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_downsampleDisparity(PTK_Alg_StereoAlgo_configParams  * config, 
                                               PTK_Alg_StereoAlgo_obsProbParams * copppParams,
                                               int16_t * disparity, 
                                               float   * dsDisparity);

/**
 * \brief Filter out low-confidence disparities
 * 
 * \param [in]  width        width of depth map
 * \param [in]  height       height of depth map
 * \param [in]  threshold    confidence threshold
 * \param [in]  disparity    input disparity map
 * \param [in]  confidence   confidence map
 * \param [out] outDisparity filtered disparity map
 *
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_filterLowConfDisparity(int32_t  width,
                                                  int32_t  height,
                                                  int8_t   existConf,
                                                  int8_t   threshold,
                                                  int16_t *disparity,
                                                  int16_t *outDisparity);
 
                                                  
/**
 * \brief Get disparity for input position (X, y) from a ground plane model
 * 
 * \param [in]  gmParams    ground model params
 * \param [in]  x           x position 
 * \param [in]  y           y position
 *
 * \return 
 *        disparity for input position (x, y)
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
float   PTK_Alg_StereoAlgo_getDisparityFromModel(PTK_Alg_StereoAlgo_groundModelParams *gmParams, 
                                                float x, 
                                                float y);

/**
 * \brief Get y position of a pixel for the given horizontal position and disparity
 * 
 * \param [in]  gmParams        ground model params
 * \param [in]  camParams       camera params
 * \param [in]  x               x position of a pixel
 * \param [in]  disparity       disparity for a pixel
 *
 * \return 
 *        Y position of pixel whose horizontal position and disparity value are x and disparity.
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_findYforDisparity(PTK_Alg_StereoAlgo_groundModelParams * gmParams,
                                             PTK_Alg_StereoAlgo_camParams         * camParams,  
                                             int32_t x, 
                                             float disparity);

/**
 * \brief Convert disparity to depth
 * 
 * \param [in]  camParams       camera params 
 * \param [in]  disparity       disparity for a pixel
 *
 * \return 
 *        depth for an input disparity
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
float   PTK_Alg_StereoAlgo_disparity2Depth(PTK_Alg_StereoAlgo_camParams * camParams, float disparity, float ypos);


/**
 * \brief Generate random number between two numbers
 * 
 * \param [in]  min       Low limit 
 * \param [in]  max       High limit
 *
 * \return 
 *        random number generated
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_randRange(int32_t min, int32_t max);

/**
 * \brief Generate random number between two numbers
 * 
 * \param [in]  M            input matrix
 * \param [in]  M_width       input matrix width
 * \param [in]  M_height      input matrix height
 * \param [out] Mt            transposed output matrix
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_transpose(float* M, int32_t M_width, int32_t M_height, float* Mt);


/**
 * \brief Multiply two matrices
 * 
 * \param [in] iM1           input matrix 1
 * \param [in] iM1_width     input matrix 1 width
 * \param [in] iM1_height    input matrix 1 height
 * \param [in] iM2           input matrix 2
 * \param [in] iM2_width     input matrix 2 width
 * \param [in] iM2_height    input matrix 2 height
 * \param [out] oM           output matrix
 * \param [out] oM_width     output matrix width
 * \param [out] oM_height    output matrix height
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
void    PTK_Alg_StereoAlgo_multiplyMatrix(float* iM1, int32_t iM1_width, int32_t iM1_height,
                                          float *iM2, int32_t iM2_width, int32_t iM2_height,
                                          float* oM, int32_t oM_width, int32_t oM_height);

/**
 * \brief LU decomposition
 * 
 * \param [in]  A           input matrix 
 * \param [in]  A_width     input matrix width
 * \param [in]  A_height    input matrix  height
 * \param [out] b           output matrix after LU decomposition
 * \param [out] b_width     output matrix width
 * \param [out] n_toCompute output matrix height
 * 
 * \return 
 *        permute value for validation
 * 
 * \ingroup group_ptk_algos_stereo_od
 */
int32_t PTK_Alg_StereoAlgo_LUdecomposition(float* A, int32_t A_width, int32_t A_height, float* b, int32_t b_width, int32_t n_toCompute);


/* PRIVATE APIS (not visible to OVX node) */

#ifdef __cplusplus
}
#endif

#endif // _STEREO_ALGO_H_
