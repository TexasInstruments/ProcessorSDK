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
#ifndef _CREATE_OG_H
#define _CREATE_OG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <perception/perception.h>
#include <perception/algos/alg_common.h>
#include <perception/utils/api_memory_if.h>
#include <perception/base/matrix.h>
#include <stdbool.h>

#define GRID_MAX_HEIGHT        2000 //  2m
#define GRID_MIN_HEIGHT       -1000 // -1m

#define MAX_CLASS_NUM_IN_GRID  5

/** On/Off test codes to enable any quadrilateral bounding box shape
 *  To be cleaned up after more tests
 */
#define QUAD_LATERAL_BB        0


/** Enum for correspondence search direction:
 *  DIR_FORWARD  - from previous to current
 *  DIR_BACKWARD - from current to previous
 */
typedef enum
{
    DIR_FORWARD = 0,
    DIR_BACKWARD
} OBJECT_SEARCH_DIRECTION;

/** Enum for correspondence search result:
 *  CORRESPONDENCE_INIT  - not searched yet
 *  CORRESPONDENCE_FOUND - correspondence found
 */  
typedef enum
{
    CORRESPONDENCE_INIT  = -1,
    CORRESPONDENCE_FOUND = -2
} OBJECT_CORRESPONDENCE;


/**
 * \defgroup group_ptk_algos_stereo_og_cog PTK Stereo OG Map creation process
 * \ingroup group_ptk_algos
 *
*/


/**
 * \brief Stereo OG Map creation process - 3D bounding box property
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct {
    /** Object class */
    uint8_t     classId;

    // pf4 pf3
    // pf1 pf2
    /** X pos of the 1st point of front bounding box */
    float        pf1x;
    /** Y pos of the 1st point of front bounding box */
    float        pf1y;
    /** X pos of the 2nd point of front bounding box */
    float        pf2x;
    /** Y pos of the 2nd point of front bounding box */
    float        pf2y;
    /** X pos of the 3rd point of front bounding box */
    float        pf3x;
    /** Y pos of the 3rd point of front bounding box */
    float        pf3y;
    /** X pos of the 4th point of front bounding box */
    float        pf4x;
    /** Y pos of the 4th point of front bounding box */
    float        pf4y;

    // pr4 pr3
    // pr1 pr2
    /** X pos of the 1st point of rear bounding box */
    float        pr1x;
    /** Y pos of the 1st point of rear bounding box */
    float        pr1y;
    /** X pos of the 2nd point of rear bounding box */
    float        pr2x;
    /** Y pos of the 2nd point of rear bounding box */
    float        pr2y;
    /** X pos of the 3rd point of rear bounding box */
    float        pr3x;
    /** Y pos of the 3rd point of rear bounding box */
    float        pr3y;
    /** X pos of the 4th point of rear bounding box */
    float        pr4x;
    /** Y pos of the 4th point of rear bounding box */
    float        pr4y;

    /** Depth of front bounding box */ 
    float        frontDepth;
    /** Depth of rear bounding box */ 
    float        rearDepth;
    
    /** Top-left X pos of occupied grid cells in mm */
    float        topLeftGridX;
    /** Top-left Y pos of occupied grid cells in mm */
    float        topLeftGridY;
    /** Bottom-right X pos of occupied grid cells in mm */
    float        bottomRightGridX;
    /** Bottom-right Y pos of occupied grid cells in mm */
    float        bottomRightGridY;

} PTK_Alg_StereoOG_BoxProp;


/**
 * \brief Stereo OG Map creation process - 3D bounding boxes
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct {
    /** number of object */
    int16_t  numObject;

    /** bounding box propery offset */
    uint32_t bbOffset;
} PTK_Alg_StereoOG_obs3DBox;

/**
 * \brief Stereo OG Map creation process - config Parameters
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** Width of input image */
    int16_t       width;

    /** Height of input image */
    int16_t       height;

    /** Enabling flag of spatial object merge */
    uint8_t       enableSpatialObjMerge;

    /** Enable flag of temporal object merge */
    uint8_t       enableTemporalObjMerge;

    /** Enable flag of temporal object smoothing */
    uint8_t       enableTemporalObjSmoothing;

    /** Method to compute distance between objects
     *  0: distance between centers
     *  1: distacne between corners
     */
    uint8_t       objectDistanceMode;

} PTK_Alg_StereoOG_CreateOGConfigParams;

/**
 * \brief Stereo algorithm - camera Parameters
 * \ingroup group_ptk_algos_stereo_og_cog
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

} PTK_Alg_StereoOG_CreateOGCamParams;


/**
 * \brief Stereo OG Map creation process - OG Parameters
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** x grid size */
    int32_t       xGridSize;

    /** y grid size */
    int32_t       yGridSize;

    /** Min X range in mm */
    int32_t       xMinRange;

    /** Max X range in mm */
    int32_t       xMaxRange;

    /** Min Y range in mm */
    int32_t       yMinRange;

    /** Max Y range in mm */
    int32_t       yMaxRange;

    /* Number of grids in X dimension */
    int32_t       xGridNum;

    /* Number of grids in Y dimension */
    int32_t       yGridNum;

    /** grid threshold for occupied/non-occupied decision */
    int16_t       thCnt;

    /** object grid threshold for occupied/non-occupied decision */
    int16_t       thObjCnt;

    /** Maximum number of objects to be detected */
    int16_t       maxNumObject;

    /** Number of neighboring grids to check for connected component analysis */
    int16_t       cNeighNum;

} PTK_Alg_StereoOG_CreateOGParams;


/**
 * \brief Stereo OG Map creation process - All algorithm Parameters
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** Algorithm config params */
    PTK_Alg_StereoOG_CreateOGConfigParams cfgParams;

    /** Algorithm camera params */
    PTK_Alg_StereoOG_CreateOGCamParams    camParams;

    /** Occupancy grid params */
    PTK_Alg_StereoOG_CreateOGParams       ogParams;

} PTK_Alg_StereoOG_CreateOGAllParams;


/**
 * \brief Stereo OG Map creation process - Grid Property struct
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** the number of classes that a grid has */
    int8_t        * numClass;    // xGridNum * yGridNum

    /** Max height of points in a grid blonging to each class */
    int16_t       * maxHeight;   // xGridNum * yGridNum * MAX_CLASS_NUM_IN_GRID

    /** Min height of points in a grid blonging to each class */
    int16_t       * minHeight;   // xGridNum * yGridNum * MAX_CLASS_NUM_IN_GRID
    
    /** Class id of each class of points in a grid */
    uint8_t       * classId;      // xGridNum * yGridNum * MAX_CLASS_NUM_IN_GRID

    /** the number of points beloning to each class ins a grid */
    int16_t       * count;       // xGridNum * yGridNum * MAX_CLASS_NUM_IN_GRID

} PTK_Alg_StereoOG_GridProp;

/**
 * \brief Stereo OG Map creation process - OG struct
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** whether a grid is occupied or not */
    int8_t        * occupied;    // xGridNum * yGridNum

    /** Max height of points in a grid */
    int16_t       * maxHeight;   // xGridNum * yGridNum

    /** Min height of points in a grid */
    int16_t       * minHeight;   // xGridNum * yGridNum

    /** Class of a grid */
    uint8_t       * classId;     // xGridNum * yGridNum

    /** the number of points in a grid */
    int16_t       * count;       // xGridNum * yGridNum

} PTK_Alg_StereoOG_OccupancyGrid;


/**
 * \brief Stereo OG Map creation process - Object struct
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** the Number of objects */
    int16_t        numObject;

    /** Class Id of an object */
    uint8_t       * classId;         // MAX_NUM_OBJECT

    /** Number of points belonging to an object */
    int16_t       * count;           // MAX_NUM_OBJECT

    /** Max height of points */
    int16_t       * maxHeight;       // MAX_NUM_OBJECT

    /** Min height of points */
    int16_t       * minHeight;       // MAX_NUM_OBJECT

    /** Min X grid index that an object occupies */
    int16_t       * maxObjXGrid;     // MAX_NUM_OBJECT

    /** Max X grid index that an object occupies */
    int16_t       * minObjXGrid;     // MAX_NUM_OBJECT

    /** Min Y grid index that an object occupies */
    int16_t       * maxObjYGrid;     // MAX_NUM_OBJECT

    /** Max Y grid index that an object occupies */
    int16_t       * minObjYGrid;     // MAX_NUM_OBJECT

    /** 1 if an object is merged to another */
    uint8_t       * merged;          // MAX_NUM_OBJECT

#if QUAD_LATERAL_BB
    /**  Y index for a grid whose X index is maxObjXGrid */
    int16_t       * maxObjXGridYPos; // MAX_NUM_OBJECT

    /**  Y index for a grid whose X index is minObjXGrid */
    int16_t       * minObjXGridYPos; // MAX_NUM_OBJECT

    /**  X index for a grid whose Y index is maxObjXGrid */
    int16_t       * maxObjYGridXPos; // MAX_NUM_OBJECT

    /**  X index for a grid whose Y index is minObjXGrid */
    int16_t       * minObjYGridXPos; // MAX_NUM_OBJECT
#endif

    /** Width of object's on OG map in the unit of grid */
    int16_t       * width;

    /** Height of object's on OG map in the unit of grid */
    int16_t       * height;

    /** Center X of object's on OG map in the unit of grid */
    float        * centerX;

    /** Center Y of object's on OG map in the unit of grid */
    float        * centerY;

    /** Object's age, i.e. the number of frames for which object has been observed
     *  It should be larger than a threshold for an object to be a real object
     */
    int16_t       * age;

    /** 1 if an object is visible in the current frame */
    uint8_t       * visible;

    /** the number of frames for which object hasn't been observed
     *  if it is larger than a threshold, the object is removed
     */
    uint8_t       * invisibleSpan;

    /** Corresponing object index in previoue/current frame */
    int16_t       * correspondence;

    /** Object's horizontal movement between frames */
    float         * dx;

    /** Object's vertical movement between frames */
    float         * dy;
} PTK_Alg_StereoOG_Object;


/**
 * \brief Stereo OG Map creation process - OGConn struct
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** The number of grids in a queue to be checked if 
     *  conected to an input grid
     */
    int32_t        numGridInQueue;

    /** 1 if a grid is visited for CCA */
    int8_t       * visited;        // xGridNum * yGridNum

    /** index of connected grid to an grid */
    int32_t      * neighborQueue;  // xGridNum * yGridNum 

    /** index of 8-connected or 24-connected grid */
    int32_t      * neighborIndex;  // xGridNum * yGridNum

} PTK_Alg_StereoOG_OGConn;


/**
 * \brief Stereo algorithm -  OG Map creation context
 * \ingroup group_ptk_algos_stereo_og_cog
 */
typedef struct
{
    /** Configuation parameter for Stereo OG Map creation */
    PTK_Alg_StereoOG_CreateOGAllParams   params;

    /** Property of each grid */
    PTK_Alg_StereoOG_GridProp            gridProp;

    /** A whole OG map */
    PTK_Alg_StereoOG_OccupancyGrid       og;

    /** Object struct */
    PTK_Alg_StereoOG_Object              object1;

    /** Additional object struct for merging process */
    PTK_Alg_StereoOG_Object              object2;

    /** Previous frame object struct for temeporal filtering */
    PTK_Alg_StereoOG_Object              object3;

    /** Pointer to a current object */
    PTK_Alg_StereoOG_Object            * curObject;

    /** Pointer to an aux object */
    PTK_Alg_StereoOG_Object            * auxObject;

    /** Pointer to a previous object */
    PTK_Alg_StereoOG_Object            * prevObject;

    /** Connected component analysis struct */
    PTK_Alg_StereoOG_OGConn              ogConn;

} PTK_Alg_StereoOG_CreateOGObj;


/**
 * \brief Get bounding box pointer
 *
 * \param [inout] obsBB object bounding box
 *
 * \return
 *        pointer to bounding boxes
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
PTK_Alg_StereoOG_BoxProp * PTK_Alg_StereOG_get3DBB(PTK_Alg_StereoOG_obs3DBox* obsBB);


/**
 * \brief Initialize OG Map creation parameters
 *
 * \param [in]  params   All algorithm configuration parameters for OG Map creation
 * \param [in]  cntxt    OG Map creation algorithm context
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_CreateOGSetParams(PTK_Alg_StereoOG_CreateOGAllParams * params,
                                        PTK_Alg_StereoOG_CreateOGObj       * cntxt);

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
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int32_t PTK_Alg_StereoOG_CreateOGConfig(PTK_Alg_StereoOG_CreateOGAllParams * params,
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
 * \ingroup group_ptk_algos_stereo_og_cog
 */
PTK_AlgHandle PTK_Alg_StereoOG_CreateOGInit(PTK_Alg_StereoOG_CreateOGAllParams * params,
                                            const PTK_Api_MemoryRsp            * memRsp);

/**
 * \brief Root function to create OG map from point clouds 
 *
 * \param [in] cntxt            algorithm handle
 * \param [in] pc               point cloud
 * \param [in] obsBB            detected object's bounding box info
 * 
  * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_createOG(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                               PTK_PointCloud               * pc,
                               PTK_Alg_StereoOG_obs3DBox    * obsBB);

/**
 * \brief Get memory size
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memPtr block pointer
 *
 * \return
 *        - required memory size
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int32_t PTK_Alg_StereoOG_CreateOGGetMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr);


/**
 * \brief Get memory size for GridProp mem
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memPtr block pointer
 *
 * \return
 *        - required memory size for OG map
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int32_t PTK_Alg_StereoOG_getGridPropMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr);

/**
 * \brief Get memory size for OG map
 *
 * \param [inout] cntxt  Algorithm handle.
 * \param [in]    memPtr block pointer
 *
 * \return
 *        - required memory size for OG map
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int32_t PTK_Alg_StereoOG_getOGMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr);


/**
 * \brief Get memory size for OG objects
 *
 * \param [inout] cntxt  Algorithm handle.
 * \param [inout] obj    Object structure
 * \param [in]    memPtr block pointer
 *
 * \return
 *        - required memory size for OG map
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int32_t PTK_Alg_StereoOG_getObjMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  PTK_Alg_StereoOG_Object *obj, uint8_t *memPtr);


/**
 * \brief Get memory size for connected component analysis
 *
 * \param [inout] cntxt Algorithm handle.
 * \param [in]    memPtr block pointer
 *
 * \return
 *        - required memory size for OG map
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int32_t PTK_Alg_StereoOG_getOGConnMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr);


/**
 * \brief Get memory size for object 3D bounding box on image without allocating memory
 *
 * \param [in] ogParams OG params.
 *
 * \return
 *        - required memory size for OG map
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
uint32_t PTK_Alg_StereoOG_getObsBBSize(const PTK_Alg_StereoOG_CreateOGParams *ogParams);

/**
 * \brief Initialize bouning box structure using the given chunk of memory
 *
 * \param [in] memPtr chunk of memory
 *
 * \return
 *        - pointer to object bounding box structure
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
PTK_Alg_StereoOG_obs3DBox * PTK_Alg_StereoOG_initObsBB(uint8_t * memPtr);

/**
 * \brief Add point clouds to OG map
 *
 * \param [in] cntxt            algorithm handle
 * \param [in] points           points
 * \param [in] numPoints        number of points
 *
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_addPoints(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                                PTK_Point                    * points,
                                int32_t                        numPoints);

/**
 * \brief Connected component analysis
 *
 * \param [in] cntxt       algorithm handle
 *
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_CCA(PTK_Alg_StereoOG_CreateOGObj * cntxt);

/**
 * \brief Find 8 neighboring grids
 *
 * \param [in] ogParams            OG params
 * \param [in] ogConn              OGConn struct 
 * \param [in] idx                 2D grid index
 * \param [in] xIdx                X grid index
 * \param [in] yIdx                Y grid index
 *
 * \return
 *          number of neighboring grids
 * 
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int16_t getNeighbor8Idx(PTK_Alg_StereoOG_CreateOGParams * ogParams, 
                        PTK_Alg_StereoOG_OGConn         * ogConn,
                        int32_t                           idx,
                        int32_t                           xIdx,
                        int32_t                           yIdx);

/**
 * \brief Find 24 neighboring grids
 *
 * \param [in] ogParams            OG params
 * \param [in] ogConn              OGConn struct 
 * \param [in] idx                 2D grid index
 * \param [in] xIdx                X grid index
 * \param [in] yIdx                Y grid index
 *
 * \return
 *          number of neighboring grids
 * 
 * \ingroup group_ptk_algos_stereo_og_cog
 */
int16_t getNeighbor24Idx(PTK_Alg_StereoOG_CreateOGParams * ogParams, 
                         PTK_Alg_StereoOG_OGConn         * ogConn,
                         int32_t                           idx,
                         int32_t                           xIdx,
                         int32_t                           yIdx);

/**
 * \brief Merge neighboring objects
 *
 * \param [inout] cntxt Algorithm handle.
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_mergeObjects(PTK_Alg_StereoOG_CreateOGObj * cntxt);

/**
 * \brief Merge neighboring objects
 *
 * \param [in]  curObj   input object
 *        [out] newObj   output merged object
 *        [in]  gridSize grid size 
 *        [in]  index    input index to merge
 *
 * \return
 *          true if any objects are merged, false otherwise
 * 
 * \ingroup group_ptk_algos_stereo_og_cog
 */
bool PTK_Alg_StereoOG_mergeTwoObjects(PTK_Alg_StereoOG_Object *curObj,
                                      PTK_Alg_StereoOG_Object *newObj,
                                      int32_t                  gridSize,
                                      int16_t                  index);

/**
 * \brief Apply temporal filtering of objects
 *
 * \param [inout] cntxt   Algorithm handle.
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_applyTemporalFilter(PTK_Alg_StereoOG_CreateOGObj * cntxt);

/**
 * \brief Find corresponding objects in two frames
 *
 * \param [in]    cntxt      Algorithm handle
 *        [in]    srcObj     source objects for which correspondences need to be found 
 *        [in]    refObj     reference objects to be searched
 *        [in]    direction  search direction
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_findCorrespondence(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                                         PTK_Alg_StereoOG_Object *srcObj,
                                         PTK_Alg_StereoOG_Object *refObj,
                                         uint8_t                  direction);

/**
 * \brief Associate the objects in the current frame to the previous frame's object.
 *        The assocaited objects shuold have the matched forward serach and backward search.
 *
 * \param [in]    cntxt      Algorithm handle
 *        [inout] curObj     current frame objects
 *        [in]    prevObj    previous frame objects
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_associateCorrespondence(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                                              PTK_Alg_StereoOG_Object *curObj,
                                              PTK_Alg_StereoOG_Object *prevObj);

/**
 * \brief Merge the current (or previous) frame's object, which does not have  
 *        correspondences in the prvious (or current ) frame,
 *        with a close object in the same frame
 *
 * \param [inout]    cntxt      Algorithm handle
 *        [in]       direction  0: backward, 1: forward
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_mergeObjectsTemporal(PTK_Alg_StereoOG_CreateOGObj * cntxt, int8_t direction);

/**
 * \brief Merge neighboring objects
 *
 * \param [in]  mode     distance compute mode
 *        [in]  obj1     input object struct 1
 *        [in]  obj2     input object struct 2
 *        [in]  idx1     object index in obj1, with which distance is computed
 *        [in]  idx2     object index in obj2, with which distance is computed
 *
 * \return
 *        distance between two objects
 * 
 * \ingroup group_ptk_algos_stereo_og_cog
 */
float PTK_Alg_StereoOG_computeDistanceObjects(uint8_t                  mode,
                                              PTK_Alg_StereoOG_Object *obj1,
                                              PTK_Alg_StereoOG_Object *obj2,
                                              int16_t                  idx1,
                                              int16_t                  idx2);

/**
 * \brief Create 3D bounding boxes for detected objects
 *
 * \param [inout] cntxt  Algorithm handle.
 *        [in]    object pointer to the detected objects.
 *        [out]   obsBB  pointer to objects' bounding boxes on image
 *
  * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_createBB(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                               PTK_Alg_StereoOG_Object      * object,
                               PTK_Alg_StereoOG_obs3DBox    * obsBB);

/**
 * \brief Merge neighboring objects
 *
 * \param [in]  srcObj   src object struct, from which an object is copied
 *        [in]  dstObj   dst object struct, to which an object is copied
 *        [in]  srcIdx   object index in srcObj that will be copied to dstObj
 *        [in]  dstIdx   object index in dstObj that an object will be copied to
 *
 * \return
 *        distance between two objects
 * 
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_copyObjectInfo(PTK_Alg_StereoOG_Object *srcObj,
                                     PTK_Alg_StereoOG_Object *dstObj,
                                     int16_t                  srcIdx,
                                     int16_t                  dstIdx);

/**
 * \brief Reset OG
 *
 * \param [in] cntxt  Algorithm handle
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_resetOG(PTK_Alg_StereoOG_CreateOGObj * cntxt);

/**
 * \brief Reset temporal object reference
 *
 * \param [in] cntxt  Algorithm handle
 *
 * \ingroup group_ptk_algos_stereo_og_cog
 */
void PTK_Alg_StereoOG_resetTemporalReference(PTK_Alg_StereoOG_CreateOGObj * cntxt);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _CREATE_OG_H
