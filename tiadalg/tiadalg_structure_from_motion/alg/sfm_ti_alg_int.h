/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2009-2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

/**
***************************************************************************
*  @file         sfm_ti_alg_int.h
*  @author       Deepak Poddar
*  @brief        This file defines the private functions for Feature Plane
*                classification
*  @version 0.1 (Sep 2014) : Base version.
***************************************************************************
*/


#ifndef TI_SFM_ALG_INT_H
#define TI_SFM_ALG_INT_H
#include "c7x.h"
#include <./../common/tiadalg_alg_int_interface.h>
#include <./../common/tiadalg_common_utils.h>
#include "tiadalg_interface.h"
#include <math.h>
/*
* float.h is required for some preprocessors like DBL_MIN, FLT_MIN etc.
*/
#include <float.h>
#include <string.h>
#include <./../common/profile.h>

#include "sfm_ti_device_utils.h"
#include "VLIB_triangulatePoints_types.h"

/*
* Maximum number of input points or tracks
*/
#define MAX_NUM_IN_POINTS            (20000)

/*
* Maximum number of output 3D points
*/
#define MAX_NUM_OUT_POINTS           MAX_NUM_IN_POINTS

/*
* Depth of the circular buffer which holds user provided camera extrinsic parameters
*/
#define CAM_EXT_PRM_BUF_DEPTH (VLIB_TRIANG_MAX_POINTS_IN_TRACK)

/*
* Maximum number of 3D points maintained inside the algorithm
*/
#define MAX_3D_POINTS (MAX_NUM_OUT_POINTS)

/*
* Total number of vector processed in one call of a kernel. This will controlled by local memory size.
*/
#define NUM_VECTORS_TOGATHER (48)

/*
* Total number of tracks processed in single call of kernel
*/
#define NUM_TRACKS_TOGATHER (NUM_VECTORS_TOGATHER * VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR)

/*
* Size of requested scratch area in L2 of DSP. request all the scratch leaving 64 KB of cache
*/
//#define L2_SCRATCH_BUF_SIZE  (480*1024) //480KB sram and 64 kb cache. total size of 512 + 32.
#define L2_SCRATCH_BUF_SIZE  (448*1024)

/*
* Size of requested scratch area in DDR
*/
#define DDR_SCRATCH_BUF_SIZE (8*1024)



#define SIMD_WIDTH (16)

#define TIADALG_DMA_CHANNEL_PING_PONG  ((int32_t) 0)
#define TIADALG_DMA_CHANNEL_CBCR_PING_PONG  ((int32_t) 1)
#define TIADALG_DMA_CHANNEL_MEMCPY_2   ((int32_t) 2)
#define TIADALG_DMA_CHANNEL_MEMCPY     ((int32_t) 3)
#define TIADALG_DMA_CHANNEL_MAX        ((int32_t) 4)

#define TIADALG_FLOW_CTRL_OPT_ONLY  (0)
#define TIADALG_FLOW_CTRL_REF_ONLY  (1)
#define TIADALG_FLOW_CTRL_NATC_ONLY (2)

extern TI_DSP_PrfInfo* prfInfo;

/**
*  @enum   eMemrecs
*  @brief  Memory records for Feature Plane classification applet
*
*/
typedef enum
{
  /* Memory records for handle */
  /* Alg Handle should be first entry in this enum table
  as test app is expecting it to be first entry*/
  ALG_HANDLE_MEMREC,
  ALG_HANDLE_INT_MEMREC,
  ALG_EXT_MEMREC,
  ALG_L3D_MEMREC,
  ALG_L2D_MEMREC,
  ALG_L1D_MEMREC,
  ALG_TRACK_MEMREC,
  ALG_TRACK_INT_MEMREC,
  NUM_MEMRECS
} eMemrecs;

/**
*  @enum   eAlgState
*  @brief  State of Feature Plane computation applet
*/
typedef enum
{
  ALG_NOT_ACTIVE,
  ALG_ACTIVE
} eAlgState;



/**
*******************************************************************************
*  @struct  TI_SFM_Handle
*  @brief   This structure is the main handle of feature plane classification
*
*  @params ivision         : All public function pointers
*  @params algState        : State of algorithm to indicate
*  @params numMemRecs      : Number of memory records
*  @params memRec          : Array of memory records
*  @params maxNumFeature   : Maximum number of features
*******************************************************************************
*/
typedef struct
{
  IVISION_Fxns      * ivision;

  uint8_t             algState;
  uint32_t            numMemRecs;
  IALG_MemRec         memRec[NUM_MEMRECS];
  SFM_TI_CreateParams createParams;

  uint8_t           * intAlgHandle;
  void              * udmaDrvObj;
  void              * dmaUtilsContext;

  uint8_t           memcpyTr[64U];
  uint8_t           pingPongDofTr[64U];
  uint8_t           pingPongCbCrTr[64U];
} SFM_TI_Obj;

typedef SFM_TI_Obj * SFM_TI_Handle;

/*--------------------------------------------------------*/
/* IALG functions                                         */
/* Refer XDAIS ialg.h file for details on these functions */
/*--------------------------------------------------------*/
int32_t SFM_TI_numAlloc(void);
/**
* @func SFM_TI_alloc
*
* @par Description:
*   Function to supply memory requirement to user refer algAlloc
*   function for details in XADIS (ialg.h file)
*
* @par
*   @param [in]  params      Create time parameters                   (IALG_Params )
*   @param [in]  parentFxns  IALG function pointers                   (IALG_Fxns )
*   @param [in]  memRec      IALG requested memory handle             (IALG_MemRec )
*
* @par Assumptions:
*
*    - Not Any
*
* @par Implementation Notes:
*    - Not Any
*
*/
int32_t SFM_TI_alloc
  (
  const IALG_Params  *params,
  struct IALG_Fxns  **parentFxns,
  IALG_MemRec        *memRec
  );
/**
* @func SFM_TI_init
*
* @par Description:
*   Does initialization of algorithm at create time.
*
* @par
*   @param [in]  handle      SFM Algorithm handle                     (IVISION_Handle )
*   @param [in]  memRec      IALG requested memory handle             (IALG_MemRec )
*   @param [in]  params      Create time parameters                   (IALG_Params )
*
* @par Assumptions:
*
*    - Not Any
*
* @par Implementation Notes:
*    - Not Any
*
*/
int32_t SFM_TI_init
  (
  IALG_Handle        handle,
  const IALG_MemRec *memRec,
  IALG_Handle        parent,
  const IALG_Params *params
  );
/**
* @func SFM_TI_activate
*
* @par Description:
*   Activation of the algorithm, like persistent memory copy
*   back to internal memory etc. Currently not in use
*
* @par
*   @param [in]  handle      Algorithm Handle                   (IALG_Handle )
*
* @par Assumptions:
*
*    - Not Any
*
* @par Implementation Notes:
*    - Not Any
*
*/
void  SFM_TI_activate(IALG_Handle handle);
/**
* @func SFM_TI_activate
*
* @par Description:
*   DEactivation of the algorithm, like persistent memory copy
*   from internal memory to external memory. Currently not in use.
*
* @par
*   @param [in]  handle      Algorithm Handle                   (IALG_Handle )
*
* @par Assumptions:
*
*    - Not Any
*
* @par Implementation Notes:
*    - Not Any
*
*/
void  SFM_TI_deactivate(IALG_Handle handle);
/**
* @func SFM_TI_free
*
* @par Description:
*   Releases all the memory requested for algorithm. Currently Not in use
*
* @par
*   @param [in]  handle      Algorithm Handle                  (IALG_Handle )
*   @param [in]  memRec      IALG memory request handle        (IALG_MemRec )
*
* @par Assumptions:
*
*    - Not Any
*
* @par Implementation Notes:
*    - Not Any
*
*/
int32_t SFM_TI_free(IALG_Handle handle, IALG_MemRec *memRec);

int32_t SFM_TI_control
  (
  IVISION_Handle     handle,
  IALG_Cmd           cmd,
  const IALG_Params *inParams,
  IALG_Params       *outParams
  );
/**
* @func SFM_TI_process
*
* @par Description:
*   This is the main processing API for SFM and should be called for each frame
*   from application.
*   Refer process function for details in ivision.h file.
*
* @par
*   @param [in]  handle      SFM Algorithm handle                     (IVISION_Handle )
*   @param [in]  inBufs      Input Buffer to be processed             (IVISION_InBufs )
*   @param [in]  outBufs     Output Buffer to be filled up            (IVISION_OutBufs)
*   @param [in]  inArgs      Input Arguments to control the behavior  (IVISION_InArgs )
*   @param [in]  outArgs     Generated discrete information           (IVISION_OutArgs)
*
* @par Assumptions:
*
*    - Input buffer is not image buffer. Input buffer is set of objects of
*      'SFM_TI_trackInfo' placed in linear memory. inBufs ::
*      bufDesc[SFM_TI_IN_BUFDESC_FEATURE_PLANE]:: bufPlanes[0] :: width
*      should be greater than or equal to input tracks which is equal to
*      inArgs :: numTracks
*
*    - Output buffer also is not image buffer, it is set of objects of
*      'SFM_TI_output' placed in linear memory. Since number of output
*      buffer is not known at the time of process call, hence out buf
*      should be allocated assuming maximum possible number of output
*      points. Maximum number of out 3D points can be equal to
*      maxNumTracks*6. Hence user should set outBufs :: bufDesc
*      [SFM_TI_OUT_BUFDESC_FEATURE_PLANES] :: bufPlanes[0] ::width
*      greater than or equal to maxNumTracks*VLIB_TRIANG_MAX_POINTS_IN_TRACK. maxNumTracks
*      was set at the time of create.
*
*    - inArgs :: numTracks should  be less than or equal to maxNumTracks
*      provided at create time.
*
*
* @par Implementation Notes:
*    - There are two optimization flows have been implemented for this kernel.
*      These two flow can be selected by:
*
*/
int32_t SFM_TI_process
  (
  IVISION_Handle   handle,
  IVISION_InBufs  *inBufs,
  IVISION_OutBufs *outBufs,
  IVISION_InArgs  *inArgs,
  IVISION_OutArgs *outArgs
  );
#define TIADALG_DOF_CONFIDENCE_TO_UINT8_SCALING (17)
/**

  @struct SFM_TI_trackInfo

  @brief  This structure defines the format in which SFM module expects a
          particular feature point's tracked location in image plane.

  @param  age : Age of the current key points that is being tracked.
                If the 255 is zero then the key point is assumed to be
                not valid. Maximum value of age is MAX_NUM_FRAMES_TO_TRACK

  @param  idx : The last index where latest kp is available.
                If it is 255 then this track is not valid.
                Maximum value of idx if MAX_NUM_FRAMES_TO_TRACK - 1.

  @param  x :   Array of horizontal image co-ordinates in Q format for
                MAX_NUM_FRAMES_TO_TRACK frames. Value of Q format has
                to be provided through SFM_TI_InArgs :: trackPtQfmt.

  @param  y :   Array of vertical image co-ordinates in Q format for
                MAX_NUM_FRAMES_TO_TRACK frames. Value of Q format has
                to be provided through SFM_TI_InArgs :: trackPtQfmt.

*/
typedef struct
{
  uint8_t   age;
  uint8_t   idx;
  uint16_t x[MAX_NUM_FRAMES_TO_TRACK];
  uint16_t y[MAX_NUM_FRAMES_TO_TRACK];
}SFM_TI_trackInfo;

/**
*******************************************************************************
*  @struct  sSFM_TI_EqSolvePrm
*  @brief   Buffers required for equation solving (AtA * x = Atb) in triangulation
*           Since total NUM_TRACKS_TOGATHER number of tracks are worked together
*           hence for all the tracks these matrix are formed before calling the
*           equation solve kernel.
*
*  @params matrixAtA    : Data matrix multiplied with its transpose, each matrix
*                         of size 3x3.
*  @params matrixAtb    : Right Column matrix multiplied by transpose of A, Size
*                         of each matrix is 3x1
*  @params weights      : Weights for each point of a track from which matrix
*                         A is formed. Before forming matrix A, matrix A is
*                         weighted with corresponding weights.
*******************************************************************************
*/
typedef struct{

  float   matrixAtA[VLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA][NUM_TRACKS_TOGATHER]; //[][6]
  float   matrixAtb[VLIB_TRIANG_MAT_AROW][NUM_TRACKS_TOGATHER];     //[][3 + 1]
  float   weights[VLIB_TRIANG_MAX_POINTS_IN_TRACK][NUM_TRACKS_TOGATHER]; // [][6]
}sSFM_TI_EqSolvePrm;

/**
*******************************************************************************
*  @struct  sSFM_TI_EqInitMat
*  @brief   Place holder for initial data matrices. From this data matrix
*           Pseudo AtA and Pseudo Atb matrices are formed. After forming
*           Pseudo matrices this data is not used.
*
*  @params matrixA      : Initial data matrix 'A' formed from track data and projection
*                         matrices
*  @params matrixb      : Initial data matrix 'b' formed from track data and projection
*                         matrices
*  @param  matrixP_AtA  : Pseudo matrix to calculate final AtA
*  @param  matrixP_Atb  : Pseudo matrix to calculate final Atb
*******************************************************************************
*/
typedef struct{
  float   matrixA[VLIB_TRIANG_MAT_ROW][VLIB_TRIANG_MAT_COL][NUM_TRACKS_TOGATHER];//[][3][12]
  float   matrixb[VLIB_TRIANG_MAT_COL][NUM_TRACKS_TOGATHER]; //[][12]
  float   matrixP_AtA[NUM_TRACKS_TOGATHER][VLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA*VLIB_TRIANG_MAX_POINTS_IN_TRACK];
  float   matrixP_Atb[NUM_TRACKS_TOGATHER][VLIB_TRIANG_MAT_ROW*VLIB_TRIANG_MAX_POINTS_IN_TRACK];
}sSFM_TI_EqInitMat;

/**
*******************************************************************************
*  @struct  sSFM_TI_PrunPrm
*  @brief   3D pruning related parameters
*
*  @params reprojErr   : Re-projection error calculated for 3D point track which are
*                        worked together
*  @params subAngle    : Subtended Angle for each 3D point which are worked together
*******************************************************************************
*/
typedef struct{
  float reprojErr[NUM_TRACKS_TOGATHER];
  float subAngle[NUM_TRACKS_TOGATHER];
  uint8_t valid[NUM_TRACKS_TOGATHER];
}sSFM_TI_PrunPrm;

/**
*******************************************************************************
*  @union  uSFM_TI_MatData
*  @brief  Triangulation related mutually exclusive scratch data
*
*  @params eqInitMat    : Init scratch data for triangulation, used before triangulation loop
*  @params eqSolvePrm   : Equation solving scratch data, used in triangulation iteration
*  @param  prunPrm      : 3D Pruning related scratch
*
*******************************************************************************
*/
typedef union {
  sSFM_TI_EqSolvePrm eqSolvePrm;
  sSFM_TI_EqInitMat  eqInitMat;
  sSFM_TI_PrunPrm    prunPrm;
}uSFM_TI_MatData;

/**
*******************************************************************************
*  @struct  sSfm_TI_L1DMem
*  @brief   L1D scratch memory definition. Base address of ever element in below
*           structure is double word aligned if base address of the structure is
*           double word aligned
*
*  @params camExtPrm         : Extrinsic Camera parameters, ordered form oldest to latest
*  @params camInvExtPrm      : Inverse of Extrinsic Camera parameters, ordered form oldest to latest
*  @param  camExtPrm     : Normalized Extrinsic Camera parameters, ordered form oldest to latest
*  @param  arrangedCamExtPrmL1    : Re-ordered Extrinsic Camera parameters, ordered form oldest to latest
*  @param  camIntXExtPrmL1  : Normalized Extrinsic Camera parameters multiplied with intrinsic,
*                              ordered form oldest to latest
*  @param  camIntPrm         : Camera Intrinsic parameters
*  @param  camInvIntPrm      : Inverse of camera Intrinsic parameters
*  @param  curTrack          : Current track data after multiplying with intrinsic parameters.
*                              Two track data are clubbed together. x/y-coordinates of two
*                              tracks are placed together. Used in forming initial data matrices
*  @param  curTrackRaw       : Current track raw data. Two track data are clubbed together.
*                              x/y-coordinates of two tracks are placed together. Used in re
*                              projection error calculation. e.g. x1x0y1y0 so on for each feature
*                              point.
*  @param  trackId           : Original track Id of selected tracks in single call of each kernel
*  @param  curFeatInTrack    : number of feature points in selected tracks in single call of
*                              each kernel
*  @param  Xcam              : 3-D output generated after triangulation API. two 3D outputs are
*                              clubbed together. X(or Y or Z) co-ordinates are clubbed together.
*                              e.g X1X0Y1Y0Z1Z0X3X2Y3Y2Z3Z2....so on
*  @param  matData           : Other scratch area. Refer @uSFM_TI_MatData
*
*******************************************************************************
*/
#define SE_PARAM_BASE    (0x0000)
#define SE0_PARAM_OFFSET (SE_PARAM_BASE)
#define SE1_PARAM_OFFSET (SE0_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE2_PARAM_OFFSET (SE1_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE3_PARAM_OFFSET (SE2_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE4_PARAM_OFFSET (SE3_PARAM_OFFSET + SE_PARAM_SIZE)
#define SE5_PARAM_OFFSET (SE4_PARAM_OFFSET + SE_PARAM_SIZE)
#define MAX_NUM_STRM_TEMPLATE (6)

#define SE_PARAM_WORDS ((uint32_t)SE_PARAM_SIZE/sizeof(uint32_t))

typedef struct
{
  /* Below two elements are used in F matrix computation from R & T parameters,
  Also inverse one is used in subtended angle calculation
  */
  float     camExtPrm[CAM_EXT_PRM_BUF_DEPTH][VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  float     camInvExtPrm[CAM_EXT_PRM_BUF_DEPTH][VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];

  /* Used in data matrix formation for triangulation
  */
  float     arrangedCamExtPrm[VLIB_TRIANG_MAX_POINTS_IN_TRACK][VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];

  /* Used in re projection error calculation
  */
  float     camIntXExtPrm[VLIB_TRIANG_MAX_POINTS_IN_TRACK][VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];

  /* Used in image co-ordinate calculation
  */
  float     camIntPrm[VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_ASIZE];

  /* Used in F matrix calculation from R &T
  */
  float     camInvIntPrm[VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_ASIZE];

  /*First frame camera intrinsic parameter
   */
  float     firstCamExtPrm[VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];

}sSfm_TI_L1DMem;

/**
*******************************************************************************
*  @struct  sSfm_TI_L2DMem
*  @brief   L2 scratch memory definition. Base address of every element in below
*           structure is double word aligned if base address of the structure is
*           double word aligned
*
*  @params isInlier       : In or out lier after F matrix pruning
*  @params isValid3DPoint    : Is valid or not valid 3D points after triangulation
*  @params scratchBuf        : Scratch Buffer
*******************************************************************************
*/
typedef struct
{
  /*Block based track processing. based on available L2 size below structure
    can be enlarged by increasing NUM_VECTORS_TOGATHER*/
  /*This structure is used in track loop, not dependent of image reoslution.
    size of structure is fix for given resolution and given algorithm.
  */
  float          curTrack[NUM_VECTORS_TOGATHER][VLIB_TRIANG_MAX_POINTS_IN_TRACK][2][VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR];
  float          curTrackRaw[NUM_VECTORS_TOGATHER][VLIB_TRIANG_MAX_POINTS_IN_TRACK][2][VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR];
  uint16_t       trackId[NUM_TRACKS_TOGATHER];
  uint8_t        curFeatInTrack[NUM_TRACKS_TOGATHER];
  float          Xcam[NUM_TRACKS_TOGATHER][3];
  uSFM_TI_MatData matData;
  uint32_t       pBlock[MAX_NUM_STRM_TEMPLATE][SE_PARAM_WORDS];

}sSfm_TI_L2DMem;

/**
*******************************************************************************
*  @struct  sSfm_TI_ExtMem
*  @brief   L1D scratch memory definition. Base address of ever element in below
*           structure is double word aligned if base address of the structure is
*           double word aligned
*
*  @params curPoints3D        : Place holder of all the 3D points generated from
*                               current frame and also from previous frames. If there
*                               are no previous points to be preserved then this area is
*                               not used, and L2 area is used.
*  @params isValid3DPoint     : To indicate a point in /c curPoints3D is valid or not.
*                               If there are no previous points to be preserved then this area is
*                               not used, and L2 are is used.
*  @params origFrmId          : For each point in /c curPoints3D, this field tells the origin
*                               frame in which it got generated. Here inBufId provided
*                               through process call is used.
*  @params origTrackId        : For each point in /c curPoints3D, this field tells the original
*                               track id from which it got generated. Here track id is the memory
*                               location of trackInfo in full track buffer.
*                               If there are no previous points to be preserved then this area is
*                               not used, and L2 are is used.
*  @params point3Dindex       : Hold the indexes of valid 3D points in /c curPoints3D
*  @params camExtPrm          : Extrinsic camera parameters placed as circular buffer
*  @params camInvExtPrm       : Inverse extrinsic camera parameters placed as circular buffer
*  @params camIntPrm          : Intrinsic camera parameters
*  @params camInvIntPrm       : Inverse of intrinsic camera parameters
*  @params localTrkOrg        : Local origin maintained for each track.
*  @params scratchBuf         : scratch buffer
*  @params num3DPoints        : Total valid current points in /c curPoints3D
*  @params curFrameNum        : Current frame number. Just incremented till VLIB_TRIANG_MAX_POINTS_IN_TRACK
*  @params curBufIndx         : Circular buffer index, with max value as (VLIB_TRIANG_MAX_POINTS_IN_TRACK-1)
*  @params cur3DPointBufIndx  : Current buffer index in curPoints3D[0 .. (VLIB_TRIANG_MAX_POINTS_IN_TRACK-1)]
*  @params curLargeBufIndx    : Circular buffer index with max value as (CAM_EXT_PRM_BUF_DEPTH - 1)
*  @params maxNumTracks       : Maximum number of tracks possible for current instance
*  @params fMatParams         : F-matrix related scratch buffer
*  @params profileEn          : Flag to indicate profile is enabled or not
*  @params prfInfo            : Profile related information
*******************************************************************************
*/

typedef struct
{
  // track data is persistant in DDR, and brought to L3/L2 at start.
  // Currently it is allocated in L3
  SFM_TI_trackInfo *track    ;
  SFM_TI_trackInfo *trackInt ;

  // occupancy grid buffer data is persistant in DDR, and brought to L3/L2 at start.
  // Currently it is allocated in L3
  int8_t *ogBuf    ;
  int8_t *ogBufInt ;
  uint8_t *constYBuf;
  uint8_t *constCbCrBuf;
  int32_t isLocalOg;

  /*pointers of l2 memory. Pointers can be overlapping based on where/when it is used in flow*/
  /*Any ping-pong buffer pointers will come here*/

  /*Needed in track maintainance code*/
  uint8_t        *keyPointBitMap;
  uint16_t       *curInTrackIdx;
  uint16_t       *curOutTrackIdx;
  uint8_t        *dofBufInt;

  /*used in F matrix estimation and pruning. Total size is 19*max_tracks */
  uint8_t        *isInlier;  //[MAX_NUM_IN_POINTS]
  uint16_t       *indexMap;
  float          *curPoints; // x and y
  float          *prevPoints; // x and y

  /*used in visualization module*/
  SFM_TI_output  *sfmOutputL2;
  uint8_t        *tempImgPtCldL2;
  uint8_t        *tempImgOgL2;
  uint8_t        *colorIndx;
  uint16_t       *blkIndx;
  uint16_t       *curBlkIndx;

  /*l2 static buffers pointers related to track loop*/
  float          (*curTrack)[VLIB_TRIANG_MAX_POINTS_IN_TRACK][2][VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR];
  float          (*curTrackRaw)[VLIB_TRIANG_MAX_POINTS_IN_TRACK][2][VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR];
  uint16_t       *trackId;
  uint8_t        *curFeatInTrack;
  float          (*Xcam)[3];
  uSFM_TI_MatData* matData;
  uint32_t       (*pBlock)[SE_PARAM_WORDS];

  /*l1 static buffers pointers*/
  /*Important matrices which are used in track loop is placed inL1D*/
  /*used in chirality pruning*/
  float     (*camExtPrmL1)[VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  /*used in SFM_TI_subtendedAnglePrun*/
  float     (*camInvExtPrmL1)[VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  /*used in triangulation*/
  float     (*arrangedCamExtPrmL1)[VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  /*used in reprojection error*/
  float     (*camIntXExtPrmL1)[VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  /*used in transforming camera points into image points*/
  float     *camIntPrmL1;
  /*used in SFM_TI_getCurTrack API*/
  float     *camInvIntPrmL1;
  /*First camera intrinsic parameter*/
  float     *firstCamExtPrmL1;

  /*Camera parameters*/
  float    camExtPrm[CAM_EXT_PRM_BUF_DEPTH][VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  float    camInvExtPrm[CAM_EXT_PRM_BUF_DEPTH][VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  float    camIntPrm[VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_ASIZE];
  float    camInvIntPrm[VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_ASIZE];
  float    firstCamExtPrm[VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE];
  uint8_t  scratchBuf[DDR_SCRATCH_BUF_SIZE];

  uint16_t       num3DPoints;
  uint32_t       curFrameNum;
  uint8_t        curLargeBufIndx;
  int32_t        maxNumTracks;
  void*          fMatParams;
  uint16_t       profileEn;

  int32_t trackIndxBufSize;
  int32_t keyPointBitMapSize;
  int32_t dofBlockBufSize;
  int32_t numRowsInDofBlock;
  int32_t numDofBlocks;

  int32_t numRowInPtCldVisBuf;
  int32_t numRowInOgVisBuf;

  TI_DSP_PrfInfo prfInfo;

  sSfm_TI_L1DMem * sfmL1Prm;
  sSfm_TI_L2DMem * sfmL2Prm;
  uint8_t* sfmL3Scratch;

}sSfm_TI_ExtMem;

void SFM_TI_updatePtrs(sSfm_TI_L1DMem* sfmL1Prm, sSfm_TI_L2DMem* sfmL2Prm, sSfm_TI_ExtMem* sfmPrm, SFM_TI_CreateParams * createParams);
#endif /* TI_SFM_ALG_INT_H */

