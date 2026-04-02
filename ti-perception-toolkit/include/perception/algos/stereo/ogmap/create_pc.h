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
#ifndef _CREATE_PC_H
#define _CREATE_PC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <perception/perception.h>
#include <perception/algos/alg_common.h>
#include <perception/utils/api_memory_if.h>
#include <perception/base/matrix.h>

/** SDE disparity fractional bits */
#define NUM_FRAC_BITS               4

/**
 * \defgroup group_ptk_algos_stereo_og_cpc PTK Stereo PC creation process
 * \ingroup group_ptk_algos
 *
*/

/**
 * \brief Stereo PC creation process - config Parameters
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
typedef struct
{
    /** Width of input */
    int16_t       width;

    /** Height of input */
    int16_t       height;

    /** Width of tensor */
    int16_t       tensorWidth;

    /** Height of tensor */
    int16_t       tensorHeight;

    /** down-sampled width of input */
    int16_t       dsWidth;

    /** down-sampled height of input */
    int16_t       dsHeight;

    /** down-sampling factor */
    int16_t       dsFactor;

    /** disparity confidence theshold */
    uint8_t       confidenceTh;

} PTK_Alg_StereoOG_CreatePCConfigParams;


/**
 * \brief Stereo PC creation process - camera Parameters
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
typedef struct
{
    /** Camera height from ground */
    int32_t        camHeight;

    /** Camera roll angle */
    float          camRoll;

    /** Camera pitch angle */
    float          camPitch;

    /** Camera yaw angle */
    float          camYaw;

    /** sine of camera pitch */
    float          sinPitch;

    /** cosine of camera pitch */
    float          cosPitch;

    /** Baseline distance in mm between left and right cameras */
    int32_t        baseline;

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

} PTK_Alg_StereoOG_CreatePCCamParams;


/**
 * \brief Stereo PC creation process - All algorithm parameters
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
typedef struct
{
    /** Algorithm config params */
    PTK_Alg_StereoOG_CreatePCConfigParams cfgParams;

    /** Camera params */
    PTK_Alg_StereoOG_CreatePCCamParams    camParams;

} PTK_Alg_StereoOG_CreatePCAllParams;

/**
 * \brief Stereo PC creation process  -  PC creation context
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
typedef struct
{
    /** Configuation parameter for Stereo PC creaetion */
    PTK_Alg_StereoOG_CreatePCAllParams     params;

    /** Pointer to disparity map. */
    int16_t                               * disparity;

    /** Downsampled disparity */
    float                                 * dsDisparity;

    /** Downsampled semantic segmentation map */
    uint8_t                               * dsSSMap;

} PTK_Alg_StereoOG_CreatePCObj;

/**
 * \brief Initialize PC creation parameters
 *
 * \param [in]  params   all algorithm parameters for PC creation
 * \param [in]  cntxt    PC creation algorithm context
 *
  * \ingroup group_ptk_algos_stereo_og_cpc
 */
void PTK_Alg_StereoOG_CreatePCSetParams(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                        PTK_Alg_StereoOG_CreatePCObj       * cntxt);

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
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
int32_t PTK_Alg_StereoOG_CreatePCConfig(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                        PTK_Api_MemoryReq                  * memReq);

/**
 * \brief Initializes the library based on the given algorithm configuration
 *        parameters and allocated memory blocks.
 *
 * \param [in] params    All algorithm configuration parameters.
 *
 * \param [in] memRsp    Memory blocks as specified below
 *
 * \return
 *        - valid handle, if successful
 *        - NULL, otherwise
 *
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
PTK_AlgHandle PTK_Alg_StereoOG_CreatePCInit(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                            const PTK_Api_MemoryRsp             * memRsp);

/**
 * \brief Get memory size
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memory block pointer
 *
 * \return
 *        - required memory size
 *
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
int32_t PTK_Alg_StereoOG_CreatePCGetMem(PTK_Alg_StereoOG_CreatePCObj * cntxt,  uint8_t *memPtr);

/**
 * \brief Root function to creaet point clouds from disparity map and semantic segmantation map
 *
 * \param [in]  cntxt            algorithm handle
 * \param [in]  disparity        disparity buffer
 * \param [in]  ssmap            semantic segmentaion buffer
 * \param [out] pc               Point cloud
 *
 *
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
void    PTK_Alg_StereoOG_createPC(PTK_Alg_StereoOG_CreatePCObj * cntxt,
                                  int16_t                      * disparity,
                                  uint8_t                      * ssmap,
                                  PTK_PointCloud               * pc);

/**
 * \brief Downsample input disparity map
 * 
 * \param [in]  config      algorihtm configuration params
 * \param [in]  disparity   input disparity map
 * \param [out] dsDisparity downsampled disparity map
 *
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
void    PTK_Alg_StereoOG_downsampleDisparity(PTK_Alg_StereoOG_CreatePCConfigParams  * config,
                                             int16_t                                * disparity,
                                             float                                  * dsDisparity);

/**
 * \brief Downsample input semantic segmentation map
 * 
 * \param [in]  config      algorihtm configuration params
 * \param [in]  ssmap       input semantic segmentation map
 * \param [out] dsSSMap     downsampled semantic segmentation map
 *
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
void    PTK_Alg_StereoOG_downsampleSSMap(PTK_Alg_StereoOG_CreatePCConfigParams * config,
                                         uint8_t                               * ssmap,
                                         uint8_t                               * dsSSMap);
                                             

/**
 * \brief Triangulate points using downsampled disparity and semantic segmentation
 *        to create point cloud
 * 
 * \param [in]  params       All algorihtm configuration params
 * \param [in]  dsDisparity  Down-sampled disparity map
 * \param [out] dsSSMap      downsampled semantic segmentation map
 *
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
int32_t PTK_Alg_StereoOG_triangulate(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                     float                              * dsDisparity,
                                     uint8_t                            * dsSSMap,
                                     PTK_PointCloud                     * pc);


/**
 * \brief Check if the point with a particular class should be triangulated
 * 
 * \param [in]  classId     class id of a point
 * 
 * \return 
 *         1 if a point should be triangualted
 *  
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
int8_t PTK_Alg_StereoOG_classToPC(uint8_t classId);

/**
 * \brief Check if a particular class of object should be detected
 * 
 * \param [in]  classId     class id
 * 
 * \return 
 *        1 if a input class id one that should be detected
 *  
 * \ingroup group_ptk_algos_stereo_og_cpc
 */
int8_t PTK_Alg_StereoOG_classToDetect(uint8_t classId);



#ifdef __cplusplus
}
#endif

#endif // #ifndef _CREATE_PC_H
