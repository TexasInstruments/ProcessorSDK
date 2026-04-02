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
*  @file        sfm_ti_alg.c
*  @author      Deepak Poddar
*  @brief       This file defines the XDAIS/iVISION functions for Feature classification applet
*  @version     0.0 (Sep 2014) : Base version.
*/

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#pragma RESET_MISRA ("required")
//#include "edma_utils_memcpy.h"
//#include "edma_utils.h"

#include "sfm_ti_alg_int.h"

#include "sfm_ti_3dPruning.h"
#include "sfm_ti_cam.h"
#include "sfm_ti_matrixOps.h"
#include "sfm_ti_track.h"
#include "sfm_ti_math.h"
#include "VLIB_triangulatePoints.h"
#include "VLIB_triangulatePoints_cn.h"
#include "sfm_ti_3dPruning.h"
#include "sfm_ti_device_utils.h"
#include "sfm_ti_vis.h"
#include "sfm_ti_occpGrid.h"

#include <assert.h>

TI_DSP_PrfInfo* prfInfo;

#define TIADALG_SFM_DEBUG

#if defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/csl/arch/c7x/Cache.h>
#endif

/**
* IVISION interface of the algorithm
*/
const IVISION_Fxns SFM_TI_VISION_FXNS =
{
  {
    (void *)&SFM_TI_VISION_FXNS,           /* module ID              */
      SFM_TI_activate,                     /* activate               */
      SFM_TI_alloc,                        /* algAlloc               */
      NULL,                                /* control (not supported)*/
      SFM_TI_deactivate,                   /* deactivate             */
      SFM_TI_free,                         /* free                   */
      SFM_TI_init,                         /* init                   */
      NULL,                                /* moved (not supported)  */
      SFM_TI_numAlloc                      /* algNumAlloc            */
  },
  SFM_TI_process,
  SFM_TI_control
};

/**
* Other exposed functions
*/
int32_t SFM_TI_numAlloc(void)
{
  return NUM_MEMRECS;
}
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
  )
{
  int32_t status = IALG_EOK;

  int32_t profileInfoSize = 0;
  int32_t fMatL2Size      = 0;
  int32_t occupancyGridSize = 0;

  SFM_TI_CreateParams * createParams = (SFM_TI_CreateParams*)params;

  /*-----------------------------------------------------------------------------*/
  /* 0th index should be alg handle memory                                       */
  /*-----------------------------------------------------------------------------*/
  memRec[ALG_HANDLE_MEMREC].size          = sizeof(SFM_TI_Obj);
  memRec[ALG_HANDLE_MEMREC].space         = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[ALG_HANDLE_MEMREC].attrs         = IALG_PERSIST;
  memRec[ALG_HANDLE_MEMREC].alignment     = 8;

  memRec[ALG_HANDLE_INT_MEMREC].size          = sizeof(SFM_TI_Obj);
  memRec[ALG_HANDLE_INT_MEMREC].space         = (IALG_MemSpace)IALG_DARAM2;
  memRec[ALG_HANDLE_INT_MEMREC].attrs         = IALG_SCRATCH;
  memRec[ALG_HANDLE_INT_MEMREC].alignment     = 8;

  fMatL2Size += createParams->maxNumTracks * sizeof(uint8_t);   /*isInlier*/
  fMatL2Size += createParams->maxNumTracks * 4 * sizeof(float); /*curPoints & prevPoints */
  fMatL2Size += createParams->maxNumTracks * sizeof(uint16_t);  /*indexMap*/

  if(createParams->rsvd1 != 0x0)
  {
    profileInfoSize = sizeof(TI_DSP_PrfData)*TI_DSP_PROFILE_MAX;
  }

  if(createParams->enAlgOcpGrdVis != 0x0)
  {
    /*Size = ocpGrdSizeInPixels x ocpGrdSizeInPixels is maintained in L3 and DDR*/
    occupancyGridSize = createParams->ocpGrdSizeInPixels * createParams->ocpGrdSizeInPixels;
  }

  memRec[ALG_EXT_MEMREC].size             = sizeof(sSfm_TI_ExtMem) +
#ifdef SFM_TI_FMAT_EST_EN
    sizeof(SFM_TI_FMatrixScratchExt) +
    sizeof(SFM_TI_FMatrixParams) +
#endif
    profileInfoSize +
    ((createParams->imgWidth*createParams->imgHeight*3)>>1) + // space to store default luma/chroma value of WxH luma, WxH/2 chroma
    1024;

  memRec[ALG_EXT_MEMREC].space            = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[ALG_EXT_MEMREC].attrs            = IALG_PERSIST;
  memRec[ALG_EXT_MEMREC].alignment        = 8;

  memRec[ALG_TRACK_MEMREC].size           = createParams->maxNumTracks*sizeof(SFM_TI_trackInfo) + occupancyGridSize;
  memRec[ALG_TRACK_MEMREC].space          = (IALG_MemSpace)IALG_EXTERNAL;
  memRec[ALG_TRACK_MEMREC].attrs          = IALG_PERSIST;
  memRec[ALG_TRACK_MEMREC].alignment      = 8;

  memRec[ALG_TRACK_INT_MEMREC].size       = createParams->maxNumTracks*sizeof(SFM_TI_trackInfo) + occupancyGridSize;
  memRec[ALG_TRACK_INT_MEMREC].space      = (IALG_MemSpace)IALG_DARAM2;
  memRec[ALG_TRACK_INT_MEMREC].attrs      = IALG_SCRATCH;
  memRec[ALG_TRACK_INT_MEMREC].alignment  = 8;

  memRec[ALG_L3D_MEMREC].size              = createParams->maxNumTracks*sizeof(SFM_TI_output) + 1024;
  memRec[ALG_L3D_MEMREC].space             = (IALG_MemSpace)IALG_DARAM2;
  memRec[ALG_L3D_MEMREC].attrs             = IALG_SCRATCH;
  memRec[ALG_L3D_MEMREC].alignment         = 8;

  memRec[ALG_L2D_MEMREC].size              = __mmax(__mmax(sizeof(sSfm_TI_L2DMem),fMatL2Size),L2_SCRATCH_BUF_SIZE); /* + sizeof(SFM_TI_FMatrixScratchInt)*/
  memRec[ALG_L2D_MEMREC].space             = (IALG_MemSpace)IALG_DARAM1;
  memRec[ALG_L2D_MEMREC].attrs             = IALG_SCRATCH;
  memRec[ALG_L2D_MEMREC].alignment         = 8;

  int32_t dmaContextSize = DmaUtilsAutoInc3d_getContextSize((uint32_t)TIADALG_DMA_CHANNEL_MAX);

  memRec[ALG_L1D_MEMREC].size             = sizeof(sSfm_TI_L1DMem) + dmaContextSize;
  memRec[ALG_L1D_MEMREC].space            = (IALG_MemSpace)IALG_DARAM0;
  memRec[ALG_L1D_MEMREC].attrs            = IALG_SCRATCH;
  memRec[ALG_L1D_MEMREC].alignment        = 8;

  return (status);
}

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
  )
{
  int32_t status = IALG_EOK;
  sSfm_TI_ExtMem* sfmPrm;
#ifdef SFM_TI_FMAT_EST_EN
  SFM_TI_FMatrixParams* fMatParams;
#endif
  size_t temp;
  float* pf32temp2;
  float* pf32temp1;
  float* pf32temp0;
  uint8_t* scratch;
  int32_t i,j;

  SFM_TI_CreateParams * createParams = (SFM_TI_CreateParams*)params;
  SFM_TI_Handle algHandle =  (SFM_TI_Handle)memRec[ALG_HANDLE_MEMREC].base;
  algHandle->intAlgHandle = (uint8_t*)memRec[ALG_HANDLE_INT_MEMREC].base;

  algHandle->numMemRecs  = NUM_MEMRECS;
  memcpy(algHandle->memRec,memRec, sizeof(IALG_MemRec)*algHandle->numMemRecs);
  memcpy(&algHandle->createParams, createParams, sizeof(SFM_TI_CreateParams));

  algHandle->ivision        = (IVISION_Fxns *)&SFM_TI_VISION_FXNS;
  algHandle->udmaDrvObj     = createParams->udmaDrvObj;

  /*-----------------------------------------------------------------------*/
  /* State to indicate that internal context of algorithm is not active    */
  /*-----------------------------------------------------------------------*/
  algHandle->algState       = ALG_NOT_ACTIVE;
  sfmPrm                    = (sSfm_TI_ExtMem*)algHandle->memRec[ALG_EXT_MEMREC].base;
  temp                      = (size_t)sfmPrm;
  sfmPrm->curLargeBufIndx   = 0;
  sfmPrm->num3DPoints       = 0;
  sfmPrm->curFrameNum       = 0;

  sfmPrm->maxNumTracks      = createParams->maxNumTracks;

  sfmPrm->profileEn = createParams->rsvd1;

  temp += sizeof(sSfm_TI_ExtMem);
  temp  = (temp + 0x7) & ~(0x7);

  if(createParams->rsvd1 != 0x0) {
    sfmPrm->prfInfo.prfData = (TI_DSP_PrfData*)(temp);
  }else{
    sfmPrm->prfInfo.prfData = NULL;
  }

  if(algHandle->createParams.enAlgOcpGrdVis != 0x0)
  {
    temp += sizeof(TI_DSP_PrfData)*TI_DSP_PROFILE_MAX;
    temp  = (temp + 0x7) & ~(0x7);
    sfmPrm->constYBuf = (uint8_t*)temp;

    temp += algHandle->createParams.imgWidth * algHandle->createParams.imgHeight;
    temp  = (temp + 0x7) & ~(0x7);
    sfmPrm->constCbCrBuf = (uint8_t*)temp;

    for(i =0; i < algHandle->createParams.imgWidth*algHandle->createParams.imgHeight; i++)
    {
      sfmPrm->constYBuf[i] = 0;
    }

    for(i =0; i < algHandle->createParams.imgHeight>>1; i++)
    {
      for(j = 0; j < algHandle->createParams.imgWidth >>1; j++)
      {
        sfmPrm->constCbCrBuf[i*algHandle->createParams.imgWidth + 2*j + 0] = 128;
        sfmPrm->constCbCrBuf[i*algHandle->createParams.imgWidth + 2*j + 1] = 128;
      }
    }

  }
  else
  {
    sfmPrm->constYBuf = NULL;
    sfmPrm->constCbCrBuf = NULL;
  }

  if((algHandle->createParams.flowCtrl & TIADALG_FLOW_CTRL_REF_ONLY) == TIADALG_FLOW_CTRL_REF_ONLY)
  {
    /*In refrence flow dont enable DMA*/
    algHandle->dmaUtilsContext = NULL;
  }
  else
  {
    algHandle->dmaUtilsContext  = (void*)(((size_t)memRec[ALG_L1D_MEMREC].base) + sizeof(sSfm_TI_L1DMem));
  }

  sfmPrm->sfmL3Scratch      = (uint8_t*) memRec[ALG_L3D_MEMREC].base;

  /* Copy Only when it is first frame.
  */
  {
    /* Copying the user given Int camera parameters into persistent area.
    This is NOT maintained in circular buffer, as Intrinsic parameter
    does not change frame to frame.
    */
    pf32temp2 = (float*)createParams->camIntPrm;
    pf32temp1 = (float*)sfmPrm->camIntPrm;
    pf32temp0 = (float*)sfmPrm->camInvIntPrm;

    for(i = 0; i < (VLIB_TRIANG_CAMERA_INTRINSIC_PARAM_SIZE); i++) {
      pf32temp1[i] = pf32temp2[i];
      pf32temp0[i] = pf32temp2[i];
    }

    scratch = sfmPrm->scratchBuf;

#ifdef DSP_INTRINSICS
    /* Inverse of K matrix
    */
    SFM_TI_gaussJordanElimination_ci(pf32temp0,
      3,
      (float*)scratch,
      3,
      scratch+16);
#else
    SFM_TI_gaussJordanElimination_cn(pf32temp0,
      3,
      (float*)scratch,
      3,
      scratch+16);
#endif
  }

  sfmPrm->keyPointBitMapSize  = (((createParams->imgWidth / createParams->keyPointStep) + 7)>>3) *
                                 ((createParams->imgHeight)/createParams->keyPointStep);

  sfmPrm->trackIndxBufSize    = createParams->maxNumTracks*sizeof(uint16_t);// 2 for in and out index buffer

  sfmPrm->dofBlockBufSize     = (L2_SCRATCH_BUF_SIZE - sfmPrm->keyPointBitMapSize - 2*sfmPrm->trackIndxBufSize) >> 1; // by two for pingPong.

  sfmPrm->numRowsInDofBlock   = floor(floor(sfmPrm->dofBlockBufSize/sizeof(int32_t))/createParams->imgWidth); // Nr
  sfmPrm->numDofBlocks        = floor((createParams->imgHeight + sfmPrm->numRowsInDofBlock - 1)/sfmPrm->numRowsInDofBlock); // Nb

  sfmPrm->dofBlockBufSize     = sfmPrm->numRowsInDofBlock * createParams->imgWidth * sizeof(uint32_t);

  SFM_TI_updatePtrs((sSfm_TI_L1DMem*)memRec[ALG_L1D_MEMREC].base,
                    (sSfm_TI_L2DMem*)memRec[ALG_L2D_MEMREC].base,
                    sfmPrm,
                    createParams);

  SFM_TI_trackInfo* track = (SFM_TI_trackInfo*)memRec[ALG_TRACK_MEMREC].base;

  for(i = 0; i < createParams->maxNumTracks; i++)
  {
    track[i].age = 255;
    track[i].idx = 255;
  }

  sfmPrm->track              = (SFM_TI_trackInfo*) memRec[ALG_TRACK_MEMREC].base;
  sfmPrm->trackInt           = (SFM_TI_trackInfo*) memRec[ALG_TRACK_INT_MEMREC].base;

  sfmPrm->ogBuf              = ((int8_t*)memRec[ALG_TRACK_MEMREC].base) + createParams->maxNumTracks*sizeof(SFM_TI_trackInfo);
  sfmPrm->ogBufInt           = ((int8_t*)memRec[ALG_TRACK_INT_MEMREC].base) + createParams->maxNumTracks*sizeof(SFM_TI_trackInfo);
  sfmPrm->isLocalOg          = 0x1;

  if(createParams->enAlgOcpGrdVis != 0x0)
  {
    for(i = 0; i < createParams->ocpGrdSizeInPixels*createParams->ocpGrdSizeInPixels; i++)
    {
      sfmPrm->ogBuf[i] = 0;
    }
  }

  return (status);
}

/*-----------------------------------------------------------------------*/
/* Function to accept any run time updates with recreation of object     */
/* Refer algControl function for details in ivision.h file               */
/* Not implemented for this Algorithm                                     */
/*-----------------------------------------------------------------------*/
int32_t SFM_TI_control
  (
  IVISION_Handle     handle,
  IALG_Cmd           cmd,
  const IALG_Params *inParams,
  IALG_Params       *outParams
  )
{
  int32_t status = IALG_EOK;

  return (status);
}

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
*    - NA
*
*/

int32_t SFM_TI_process
  (
  IVISION_Handle   handle,
  IVISION_InBufs  *inBufs,
  IVISION_OutBufs *outBufs,
  IVISION_InArgs  *inArgs,
  IVISION_OutArgs *outArgs
  )
{
  SFM_TI_Handle   algHandle   = (SFM_TI_Handle)(handle);
  SFM_TI_InArgs  *inArgsSFM   = (SFM_TI_InArgs  *)inArgs;
  SFM_TI_OutArgs *outArgsSFM  = (SFM_TI_OutArgs *)outArgs;
  SFM_TI_Handle intAlgHandle  ;

  int32_t status = IALG_EOK;
  int32_t kk;
  int32_t i,j,l;

  int16_t           maxNumInliers;
  float             Fmat[9];
  SFM_TI_trackInfo* restrict curTrack;
  SFM_TI_trackInfo* restrict baseTrack;
  int32_t           lastIndx,scndLastIndx;
  SFM_TI_output*    psfmOPut;
  uint32_t          numOut3DPoints;
  int32_t           infeatQfmt = 16; // This is driven by dof Q format
  uint8_t           validOut[NUM_TRACKS_TOGATHER];

  /*-----------------------------------------------------------------------*/
  /* Activate the algorithm to make sure that now onwards internal memory  */
  /* handle can be utilized                                                */
  /*-----------------------------------------------------------------------*/
  if (status == IALG_EOK)
  {
    SFM_TI_activate((IALG_Handle)(void*)algHandle);
    intAlgHandle = (SFM_TI_Handle)(void *)algHandle->intAlgHandle;
  }

  SFM_TI_CreateParams * createParams = (SFM_TI_CreateParams*)&intAlgHandle->createParams;
  sSfm_TI_ExtMem* sfmPrm     = (sSfm_TI_ExtMem*)intAlgHandle->memRec[ALG_EXT_MEMREC].base;
  sSfm_TI_L1DMem* sfmL1Prm   = (sSfm_TI_L1DMem*)intAlgHandle->memRec[ALG_L1D_MEMREC].base;


  if(inArgsSFM->reset != 0)
  {
    sfmPrm->curFrameNum = 0;
  }

#ifdef ENABLE_PROFILE
  prfInfo = &sfmPrm->prfInfo;

  if (inArgsSFM->reserved0)
  {
    printf("profile initialization \n");
  }


  TI_DSP_ProfileInit(&sfmPrm->prfInfo);

  if (inArgsSFM->reserved0)
  {
    printf("profile finished \n");
  }

  /*Extra compensation*/
  sfmPrm->prfInfo.rsvd1                 = 0;
  sfmPrm->prfInfo.rsvd2                 = 0;

  sfmPrm->prfInfo.rsvd3                 = 0;
  sfmPrm->prfInfo.rsvd4                 = 0;


  TI_DSP_ProfileModule(&sfmPrm->prfInfo,
                        TI_DSP_PROFILE_MODULE0,
                        TI_DSP_PROFILE_START,
                        TI_DSP_PROFILE_CUMULATIVE);

#endif

  /* Initializing outArgs parameters
  */
  outArgsSFM->sfmStats.numCur3DPnts       = 0;
  outArgsSFM->sfmStats.numInlierFmat      = 0;
  outArgsSFM->sfmStats.numIterFMat        = 0;
  outArgsSFM->outNumPoints                = 0;


  /* Error checks for few of the input parameters.
     Only one plane of input data is needed, which is DOF data.
  */
  if(inBufs->numBufs < 1){
    status = IVISION_ERRORTYPE_NUM_INBUF_EXCEEDED;
  }

  if(((int32_t)outBufs->bufDesc[SFM_TI_OUT_BUFDESC_FEATURE_PLANES]->bufPlanes[0].width) <
    sfmPrm->maxNumTracks){
      status = IVISION_ERRORTYPE_NUM_INBUF_EXCEEDED;
  }
#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,
                        TI_DSP_PROFILE_MODULE0,
                        TI_DSP_PROFILE_END,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif

//#define DUMP_FLOW_BIN
#ifdef DUMP_FLOW_BIN
  char file_name1[200];
  static int frame_num1 = 0;
  sprintf(file_name1,"dof_%d.flow",frame_num1++);
  FILE* fp1 = fopen(file_name1,"wb");
  fwrite(inBufs->bufDesc[SFM_TI_IN_BUFDESC_IN_DOF_BUFFER]->bufPlanes[0].buf,4,createParams->imgWidth*createParams->imgHeight,fp1);
  fclose(fp1);
#endif

//#define DUMP_FLOW_TXT
#ifdef DUMP_FLOW_TXT
  char file_name[200];
  static int frame_num = 0;
  sprintf(file_name,"/ti/j7/workarea/tiadalg/tiadalg_structure_from_motion/test/dof_%d.h",frame_num);
  FILE* fp = fopen(file_name,"w");

  uint32_t* dof_ptr = (uint32_t*) inBufs->bufDesc[SFM_TI_IN_BUFDESC_IN_DOF_BUFFER]->bufPlanes[0].buf;
  //sprintf(file_name,"#pragma DATA_SECTION (dofTable_%d, \".ioBufs\") \n",frame_num);
  //fprintf(fp,"%s",file_name);

  sprintf(file_name," const uint32_t dofTable_%d[%d] = {",frame_num++, createParams->imgWidth*createParams->imgHeight);
  fprintf(fp,"%s",file_name);

  for(i = 0; i < createParams->imgHeight; i++)
  {
    for(j = 0; j < createParams->imgWidth; j++)
    {
      fprintf(fp,"0x%08x, ", dof_ptr[i*createParams->imgWidth + j]);
    }
    fprintf(fp,"\n");
  }
  fprintf(fp,"}; \n \n");

  fclose(fp);
#endif

  if (inArgsSFM->reserved0)
  {
    if(intAlgHandle->dmaUtilsContext == NULL)
    {
      printf("DRU is not used \n");
    }
    else
    {
      printf("DRU is used \n");
    }
    printf("update track started \n");
  }

  if((algHandle->createParams.flowCtrl & TIADALG_FLOW_CTRL_REF_ONLY) == TIADALG_FLOW_CTRL_REF_ONLY)
  {
    SFM_TI_updateTrackRef(sfmPrm->trackInt,(int32_t*)inBufs->bufDesc[SFM_TI_IN_BUFDESC_IN_DOF_BUFFER]->bufPlanes[0].buf,
                        sfmPrm->curInTrackIdx,sfmPrm->keyPointBitMap,
                        createParams->imgWidth, createParams->imgHeight,
                        createParams->keyPointStep, createParams->maxNumTracks,
                        inArgsSFM->flowInvalidPadX, inArgsSFM->flowInvalidPadY,
                        sfmPrm->curFrameNum, inArgsSFM->flowConfThr, &sfmPrm->prfInfo
                      );
  }
  else
  {
    SFM_TI_updateTrack(sfmPrm->trackInt,(int32_t*)inBufs->bufDesc[SFM_TI_IN_BUFDESC_IN_DOF_BUFFER]->bufPlanes[0].buf,
                        sfmPrm->curInTrackIdx,sfmPrm->curOutTrackIdx,(int32_t*)sfmPrm->dofBufInt, sfmPrm->keyPointBitMap,
                        createParams->imgWidth, createParams->imgHeight,createParams->keyPointStep,
                        createParams->maxNumTracks,inArgsSFM->flowInvalidPadX, inArgsSFM->flowInvalidPadY,
                        sfmPrm->curFrameNum, inArgsSFM->flowConfThr,sfmPrm->keyPointBitMapSize,
                        sfmPrm->dofBlockBufSize, sfmPrm->numDofBlocks, sfmPrm->numRowsInDofBlock,
                        intAlgHandle->pingPongDofTr,intAlgHandle->dmaUtilsContext, &sfmPrm->prfInfo
                      );
  }


  /*If it is first frame or reset frame then clear out the occupancy grid data*/
  if(((sfmPrm->curFrameNum == 0) || (sfmPrm->isLocalOg == 0x1)) && (createParams->enAlgOcpGrdVis != 0))
  {
    TIADALG_memcpy2D(sfmPrm->ogBufInt,
              sfmPrm->ogBuf,
              createParams->ocpGrdSizeInPixels,
              createParams->ocpGrdSizeInPixels,
              createParams->ocpGrdSizeInPixels,
              createParams->ocpGrdSizeInPixels,
              TIADALG_DMA_CHANNEL_MEMCPY_2,
              intAlgHandle->dmaUtilsContext,
              intAlgHandle->memcpyTr,
              0x0);
  }


  if (inArgsSFM->reserved0)
  {
    printf("update track finished \n");
  }

  if((createParams->enAlgPtCldVis != 0))
  {
    TIADALG_memcpy2D(outBufs->bufDesc[SFM_TI_OUT_BUFDESC_LUMA_PTCLD_BUFFER]->bufPlanes[0].buf,
              inBufs->bufDesc[SFM_TI_IN_BUFDESC_IN_LUMA_IMG_BUFFER]->bufPlanes[0].buf,
              createParams->imgWidth,
              createParams->imgHeight,
              createParams->imgWidth,
              createParams->imgWidth,
              TIADALG_DMA_CHANNEL_MEMCPY,
              intAlgHandle->dmaUtilsContext,
              intAlgHandle->memcpyTr,
              0x0);
  }

  if(createParams->enAlgOcpGrdVis != 0)
  {
    /*reusing the ping pong channel used in dof*/
    /*copying the constant luma buffer, Cb Cr will be filled later in visualization routine*/
    TIADALG_memcpy2D(outBufs->bufDesc[SFM_TI_OUT_BUFDESC_LUMA_OCPGD_BUFFER]->bufPlanes[0].buf, // luma buffer in occupancy grid luma buffer in ddr
              sfmPrm->constYBuf, // lumar buffer in DDR
              createParams->imgWidth,
              createParams->imgHeight,
              createParams->imgWidth,
              createParams->imgWidth,
              TIADALG_DMA_CHANNEL_PING_PONG,
              intAlgHandle->dmaUtilsContext,
              intAlgHandle->memcpyTr,
              0x0);

    TIADALG_memcpy2D(outBufs->bufDesc[SFM_TI_OUT_BUFDESC_CHROMA_OCPGD_BUFFER]->bufPlanes[0].buf, // chroma buffer in occupancy grid luma buffer in ddr
              sfmPrm->constCbCrBuf, // lumar buffer in DDR
              createParams->imgWidth,
              createParams->imgHeight>>1,
              createParams->imgWidth,
              createParams->imgWidth,
              TIADALG_DMA_CHANNEL_CBCR_PING_PONG,
              intAlgHandle->dmaUtilsContext,
              intAlgHandle->memcpyTr,
              0x0);
  }

  /* Copy the intrinsic and extrinsic camera parameters to persistent
     circular buffer (DDR). Extrinsic parameters needs to be buffered
     every frame. whereas intrinsic parameter needs to be buffered only
     in first frame. Current frame Extrinsic parameters are copied in
     circular buffer.
  */
  if (inArgsSFM->reserved0)
  {
    printf("SFM_TI_copyCamPrmToCirCBuf started \n");
  }

  SFM_TI_copyCamPrmToCirCBuf(inArgsSFM->camExtPrm,
    sfmPrm->curLargeBufIndx,
    (float*)sfmPrm->camExtPrm);

  if(sfmPrm->curFrameNum == (VLIB_TRIANG_MAX_POINTS_IN_TRACK - 1))
  {
    memcpy((void*)sfmPrm->firstCamExtPrm, (void*)inArgsSFM->camExtPrm, VLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE*sizeof(float));

    sfmPrm->firstCamExtPrm[12] = 0.0f;
    sfmPrm->firstCamExtPrm[13] = 0.0f;
    sfmPrm->firstCamExtPrm[14] = 0.0f;
    sfmPrm->firstCamExtPrm[15] = 1.0f;
  }

  /*Get the normalized matrices, where the matrix corresponding to
  first frame (5 frame back from current) is inversed and multiplied
  with other next matrices. First matrix turns to be identity matrix
  after multiplying with its own inverse.
  Lets assume if matrices are P0 P1 P2 P3 P4 P5, then normalized
  matrices will be P0*inv(P0), P1*inv(P0), P2*inv(P0), P3*inv(P0),
  P4*inv(P0),P5*inv(P0).

  Please note that in default configuration, no normalization happens.
  In default configuration, projection matrices are used as it is.

  Inverse of incoming P matrices are stored in persistent area, to
  avoid multiple inverse calculation in a process call. So every
  process call for one incoming P matrix , its inverse is calculated
  and stored in persistent area (DDR)

  Normalized matrices are directly stored in L1D.

  Inverse of intrinsic parameter matrix (K) also needs to be calculated,
  which is size of 3x3. Inverse of K matrix should happen only once and
  should be stored in persistent area (currently DDR) for future frame
  usages. Later in every frame just copy this inverse of K matrix from DDR to
  L1D in every frame to have better performance.
  */
  if (inArgsSFM->reserved0)
  {
    printf("SFM_TI_getInvExtPrmMatrix started \n");
  }

  SFM_TI_getInvExtPrmMatrix((float*)sfmPrm->camExtPrm,
    sfmPrm->curLargeBufIndx,
    (float*)&sfmPrm->camInvExtPrm[0][0],
    sfmPrm->scratchBuf);

  /* Copy Camera parameters from external memory to internal memory L1D.
  This has been done for optimization purpose, because cache miss will
  be avoided. Currently this module usages CPU to do copy, later this
  can be converted into EDMA transfer.
  */
  if (inArgsSFM->reserved0)
  {
    printf("SFM_TI_copyCamPrmsToIntMem started \n");
  }

  SFM_TI_copyCamPrmsToIntMem(sfmL1Prm,sfmPrm);

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,
                        TI_DSP_PROFILE_MODULE3,
                        TI_DSP_PROFILE_END,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif

  if (inArgsSFM->reserved0)
  {
    printf("tringulation init started \n");
  }

#ifdef DSP_INTRINSICS

  //if(sfmPrm->curFrameNum == 0) // do the streaming engine initialization every frame, as pBlock in SL2 is scratch
  {
    // first frame for traingulation
    VXLIB_triangulatePoints_i32f_o32f_init((float*)sfmPrm->curTrack,
                                                (float*)sfmPrm->arrangedCamExtPrmL1,
                                                sfmPrm->curFeatInTrack,
                                                inArgsSFM->maxTriangItr,
                                                (float*)sfmPrm->matData,
                                                NUM_VECTORS_TOGATHER*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR,
                                                inArgsSFM->reserved0,
                                                (float*)sfmPrm->Xcam,
                                                validOut,
                                                sfmPrm->pBlock);
  }
#endif

  if (inArgsSFM->reserved0)
  {
    printf("tringulation init finished \n");
  }

  baseTrack = sfmPrm->trackInt;

  if(sfmPrm->curFrameNum > 0) {
#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,
                        TI_DSP_PROFILE_MODULE4,
                        TI_DSP_PROFILE_START,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif
    maxNumInliers = 0;
    j = 0;

    if (inArgsSFM->reserved0)
    {
      printf("Current previous point collection started started \n");
    }

    float invInfeatQfmt = my_OneByX((float)infeatQfmt);

    //printf("address of lastIndx = %lx, scndLastIndx = %lx \n",&lastIndx,&scndLastIndx);

    for(i=0; i < sfmPrm->maxNumTracks; i++) {

      curTrack = baseTrack + i;

      if(((curTrack->age) >= 0x2U) && ((curTrack->age) <= MAX_NUM_FRAMES_TO_TRACK)){
        lastIndx     = curTrack->idx;
        scndLastIndx = lastIndx - 1;

        if(scndLastIndx < 0)
        {
          scndLastIndx = scndLastIndx + MAX_NUM_FRAMES_TO_TRACK;
        }

        sfmPrm->curPoints[2*j]     =  (float)((float)curTrack->x[lastIndx])*invInfeatQfmt;
        sfmPrm->curPoints[2*j + 1] =  (float)((float)curTrack->y[lastIndx])*invInfeatQfmt;

        sfmPrm->prevPoints[2*j]     = (float)((float)curTrack->x[scndLastIndx])*invInfeatQfmt;
        sfmPrm->prevPoints[2*j + 1] = (float)((float)curTrack->y[scndLastIndx])*invInfeatQfmt;

        sfmPrm->indexMap[j] = (int16_t)i;

        j++;
      }
    }
#if defined(ENABLE_PROFILE) && defined(TIADALG_PROFILE_PRINTS)
    uint32_t numValidTracks = j;
#endif

#ifdef ENABLE_PROFILE
    TI_DSP_ProfileModule(&sfmPrm->prfInfo,
                          TI_DSP_PROFILE_MODULE4,
                          TI_DSP_PROFILE_END,
                          TI_DSP_PROFILE_CUMULATIVE);
#endif

    /* F matrix Computation or Estimation
    */
    if(inArgsSFM->fMatrixPrunEn) {
#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE5,
        TI_DSP_PROFILE_START,
        TI_DSP_PROFILE_CUMULATIVE);
#endif
      /*if(inArgsSFM->fMatrixCalcMethod == SFM_TI_FMAT_FROM_RT_PARAMS)*/
      /* Currently Essential matrix (R & T) is decomposed to get F matrix.
       * Accentual estimation code is disabled because of SVD license issue
       */
      {
        if (inArgsSFM->reserved0)
        {
          printf("SFM_TI_computeFmatFrmRT started \n");
        }
        SFM_TI_computeFmatFrmRT(
          &sfmPrm->camExtPrmL1[(CAM_EXT_PRM_BUF_DEPTH - 1)][0],
          &sfmPrm->camInvExtPrmL1[(CAM_EXT_PRM_BUF_DEPTH - 2)][0],
          sfmPrm->camInvIntPrmL1,
          sfmPrm->camIntPrmL1,
          Fmat);
        outArgsSFM->sfmStats.numIterFMat   = 0x0;
      }
      /*else{
        SFM_TI_findFundamentalMat(sfmPrm->prevPoints,
                           sfmPrm->curPoints,
                           j,
                           inArgsSFM->fMatrixInTh,
                           inArgsSFM->maxRansacItr,
                           Fmat,
                           &numIter,
                           (SFM_TI_FMatrixParams*)sfmPrm->fMatParams);

        outArgsSFM->sfmStats.numIterFMat   = numIter;

      }*/
#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE5,
        TI_DSP_PROFILE_END,
        TI_DSP_PROFILE_CUMULATIVE);

      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE6,
        TI_DSP_PROFILE_START,
        TI_DSP_PROFILE_CUMULATIVE);
#endif

      if (inArgsSFM->reserved0)
      {
        printf("Total number of points for VXLIB_FMAT_mapPoints_ci API is %d \n", j);
      }

      /* Find all the inliers after estimating F matrix.
      */
#ifdef DSP_INTRINSICS
      VXLIB_FMAT_mapPoints_ci(sfmPrm->curPoints,
        sfmPrm->prevPoints,
        (int16_t)j,
        Fmat,
        1,
        (float)inArgsSFM->fMatrixInTh,
        &maxNumInliers,
        sfmPrm->isInlier,
        sfmPrm->pBlock);
#else
      VLIB_FMAT_AR_mapPoints(sfmPrm->curPoints,
        sfmPrm->prevPoints,
        (int16_t)j,
        Fmat,
        (float)inArgsSFM->fMatrixInTh,
        &maxNumInliers,
        sfmPrm->isInlier);
#endif
      /* Updation of local origin, based on inlier outlier
      information
      */
      for(i = 0; i < j; i++) {

        l = sfmPrm->indexMap[i];

        curTrack = baseTrack + l;

        /* If out-lier, reset the track. As it doesnt have any kp from latest frame
        */
        if(sfmPrm->isInlier[i] == 0x00)
        {
          curTrack->age = 255;
          curTrack->idx = 255;
        }
      }

#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE6,
        TI_DSP_PROFILE_END,
        TI_DSP_PROFILE_CUMULATIVE);
#endif

    }else{

      for(i = 0; i < sfmPrm->maxNumTracks; i++) {
        sfmPrm->isInlier[i] = 0x1;
      }

      maxNumInliers = (int16_t)sfmPrm->maxNumTracks;

    }

    if (inArgsSFM->reserved0)
    {
      printf("maxNumInliers is %d \n", maxNumInliers);
    }

    outArgsSFM->sfmStats.numInlierFmat = maxNumInliers;
  }

  if(sfmPrm->curFrameNum >= (VLIB_TRIANG_MAX_POINTS_IN_TRACK - 1)) {

#ifdef ENABLE_PROFILE
    TI_DSP_ProfileModule(&sfmPrm->prfInfo,
      TI_DSP_PROFILE_MODULE7,
      TI_DSP_PROFILE_START,
      TI_DSP_PROFILE_CUMULATIVE);
#endif
    /* Multiply normalized transformation matrix (extrinsic prm) with
    Intrinsic parameters.
    camExtPrm --> 3x4 [in]
    camIntPrm     --> 3x3 [in]
    normCamIntExtPrm --> 3x4 [out]
    */
    SFM_TI_mulIntExtPrm(sfmPrm->camIntPrmL1,
      (float*)sfmPrm->camExtPrmL1,
      (float*)sfmPrm->camIntXExtPrmL1);

    /* reformatting of normalized camera parameter, for better access in
    later kernels
    */
    for(l = 0; l < (VLIB_TRIANG_MAX_POINTS_IN_TRACK); l++){
      sfmPrm->arrangedCamExtPrmL1[l][0]   = sfmPrm->camExtPrmL1[l][8];
      sfmPrm->arrangedCamExtPrmL1[l][1]   = sfmPrm->camExtPrmL1[l][0];
      sfmPrm->arrangedCamExtPrmL1[l][2]   = sfmPrm->camExtPrmL1[l][4];
      sfmPrm->arrangedCamExtPrmL1[l][3]   = sfmPrm->camExtPrmL1[l][9];
      sfmPrm->arrangedCamExtPrmL1[l][4]   = sfmPrm->camExtPrmL1[l][1];
      sfmPrm->arrangedCamExtPrmL1[l][5]   = sfmPrm->camExtPrmL1[l][5];
      sfmPrm->arrangedCamExtPrmL1[l][6]   = sfmPrm->camExtPrmL1[l][10];
      sfmPrm->arrangedCamExtPrmL1[l][7]   = sfmPrm->camExtPrmL1[l][11];
      sfmPrm->arrangedCamExtPrmL1[l][8]   = sfmPrm->camExtPrmL1[l][2];
      sfmPrm->arrangedCamExtPrmL1[l][9]   = sfmPrm->camExtPrmL1[l][6];
      sfmPrm->arrangedCamExtPrmL1[l][10]  = sfmPrm->camExtPrmL1[l][3];
      sfmPrm->arrangedCamExtPrmL1[l][11]  = sfmPrm->camExtPrmL1[l][7];

      sfmPrm->arrangedCamExtPrmL1[l][12]  = sfmPrm->camExtPrmL1[l][8];
      sfmPrm->arrangedCamExtPrmL1[l][13]  = sfmPrm->camExtPrmL1[l][9];
      sfmPrm->arrangedCamExtPrmL1[l][14]  = sfmPrm->camExtPrmL1[l][10];
      sfmPrm->arrangedCamExtPrmL1[l][15]  = sfmPrm->camExtPrmL1[l][11];

    }
#ifdef ENABLE_PROFILE
    TI_DSP_ProfileModule(&sfmPrm->prfInfo,
      TI_DSP_PROFILE_MODULE7,
      TI_DSP_PROFILE_END,
      TI_DSP_PROFILE_CUMULATIVE);
#endif

    j = 0;
    numOut3DPoints = 0;

    /* Below FOR loop mostly does computation on data present in L1D.
    This loop ideally is consuming most of the Mhz , second to
    F-matrix based pruning.
    */
    // track loop

    if (inArgsSFM->reserved0)
    {
      printf("Track Loop started \n");
    }

    if((((sfmPrm->curFrameNum == 0) || (sfmPrm->isLocalOg == 0x1)) && (createParams->enAlgOcpGrdVis != 0)) &&
      (intAlgHandle->dmaUtilsContext!= NULL)
      )
    {
      DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_MEMCPY_2);
    }

    for(i = 0; i < sfmPrm->maxNumTracks;) {

      j = 0;
#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE8,
        TI_DSP_PROFILE_START,
        TI_DSP_PROFILE_CUMULATIVE);
#endif
      /* Collect valid track ids and their size to be processed together.
      */

      do
      {
        curTrack = baseTrack + i;

        if(((curTrack->age) >= 2) && ((curTrack->age) <= VLIB_TRIANG_MAX_POINTS_IN_TRACK)){
          sfmPrm->trackId[j] = (uint16_t)i;
          sfmPrm->curFeatInTrack[j] = curTrack->age;
          j++;
        }

        i++;
      }while( (j < NUM_TRACKS_TOGATHER) && (i < sfmPrm->maxNumTracks) );

      /* Taking care of last chunk of tracks.
      */
      for(;j < NUM_TRACKS_TOGATHER;j++){
        sfmPrm->trackId[j]        = (uint16_t)(i - 1);
        sfmPrm->curFeatInTrack[j] = 0;
      }
#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE8,
        TI_DSP_PROFILE_END,
        TI_DSP_PROFILE_CUMULATIVE);

      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE9,
        TI_DSP_PROFILE_START,
        TI_DSP_PROFILE_CUMULATIVE);
#endif
      /* Copying the track data from external memory to internal memory.
      */

#ifdef DSP_INTRINSICS
      SFM_TI_getCurTrack_ci(baseTrack,
        sfmPrm->camInvIntPrmL1,
        sfmPrm->trackId,
        (float*)sfmPrm->curTrack,
        (float*)sfmPrm->curTrackRaw,
        sfmPrm->curFeatInTrack);
#else
      SFM_TI_getCurTrack(baseTrack,
        sfmPrm->camInvIntPrmL1,
        sfmPrm->trackId,
        (float*)sfmPrm->curTrack,
        (float*)sfmPrm->curTrackRaw,
        sfmPrm->curFeatInTrack);
#endif
#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE9,
        TI_DSP_PROFILE_END,
        TI_DSP_PROFILE_CUMULATIVE);
#endif
      if(j == NUM_TRACKS_TOGATHER)
      {
        /* curFeatInTrack >= 2 indicates that it is a valid track
        */
        /* Main Triangulation processing to generate the 3D point from
        track of image co-ordinates and camera parameters.
        */
#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE10,
        TI_DSP_PROFILE_START,
        TI_DSP_PROFILE_CUMULATIVE);
#endif
#ifdef DSP_INTRINSICS
      VXLIB_triangulatePoints_i32f_o32f_kernel((float*)sfmPrm->curTrack,
                                                  (float*)sfmPrm->arrangedCamExtPrmL1,
                                                  sfmPrm->curFeatInTrack,
                                                  inArgsSFM->maxTriangItr,
                                                  (float*)sfmPrm->matData,
                                                  NUM_VECTORS_TOGATHER*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR,
                                                  inArgsSFM->reserved0,
                                                  (float*)sfmPrm->Xcam,
                                                  validOut,
                                                  sfmPrm->pBlock);
#else
        VLIB_triangulatePoints_cn((float*)sfmPrm->curTrack,
          (float*)sfmPrm->arrangedCamExtPrmL1,
          sfmPrm->curFeatInTrack,
          inArgsSFM->maxTriangItr,
          (float*)sfmPrm->Xcam,
          (float*)sfmPrm->matData,
          NUM_VECTORS_TOGATHER*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR,
          validOut,
          inArgsSFM->reserved0);
#endif

        /* Calculating the re projection error for all the 3D points
        generated in current frame.
        */
#ifdef ENABLE_PROFILE
        TI_DSP_ProfileModule(&sfmPrm->prfInfo,
          TI_DSP_PROFILE_MODULE10,
          TI_DSP_PROFILE_END,
          TI_DSP_PROFILE_CUMULATIVE);

        TI_DSP_ProfileModule(&sfmPrm->prfInfo,
          TI_DSP_PROFILE_MODULE11,
          TI_DSP_PROFILE_START,
          TI_DSP_PROFILE_CUMULATIVE);
#endif
        /* project back each generated 3D points on each image plane,
        and calculate the average error. This average error is called
        as re projection error.
        */
#ifdef DSP_INTRINSICS
        SFM_TI_reprojErrorCalc_ci((float*)sfmPrm->camIntXExtPrmL1,
          (float*)&sfmPrm->Xcam[0][0],
          (float*)sfmPrm->curTrackRaw,
          sfmPrm->curFeatInTrack,
          sfmPrm->matData->prunPrm.reprojErr
          );
#else
        SFM_TI_reprojErrorCalc((float*)sfmPrm->camIntXExtPrmL1,
          (float*)&sfmPrm->Xcam[0][0],
          (float*)sfmPrm->curTrackRaw,
          sfmPrm->curFeatInTrack,
          sfmPrm->matData->prunPrm.reprojErr
          );
#endif
#ifdef ENABLE_PROFILE
        TI_DSP_ProfileModule(&sfmPrm->prfInfo,
          TI_DSP_PROFILE_MODULE11,
          TI_DSP_PROFILE_END,
          TI_DSP_PROFILE_CUMULATIVE);

        TI_DSP_ProfileModule(&sfmPrm->prfInfo,
          TI_DSP_PROFILE_MODULE12,
          TI_DSP_PROFILE_START,
          TI_DSP_PROFILE_CUMULATIVE);
#endif
        /* Calculate angle subtended for each generated 3D points
        */
#ifdef DSP_INTRINSICS
        SFM_TI_subtendedAnglePrun_ci((float*)sfmPrm->camInvExtPrmL1,
          (float*)&sfmPrm->Xcam[0][0],
          sfmPrm->curFeatInTrack,
          sfmPrm->matData->prunPrm.subAngle);
#else
        SFM_TI_subtendedAnglePrun((float*)sfmPrm->camInvExtPrmL1,
          (float*)&sfmPrm->Xcam[0][0],
          sfmPrm->curFeatInTrack,
          sfmPrm->matData->prunPrm.subAngle);
#endif

#ifdef ENABLE_PROFILE
        TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE12,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);

        TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE13,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif
        /* Utilizing the re projection error and angle subtended
        check the validity of the generated 3D points
        */
        SFM_TI_combinePrun(sfmPrm->matData->prunPrm.reprojErr,sfmPrm->matData->prunPrm.subAngle,
          4.0,inArgsSFM->pointPruneAngle,sfmPrm->matData->prunPrm.valid);

#ifdef ENABLE_PROFILE
        TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE13,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);


        TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE14,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif

        float* Xcam = (float*)&sfmPrm->Xcam[0][0];
#ifdef DSP_INTRINSICS
        SFM_TI_chiralityPruning_ci((float*)sfmPrm->camExtPrmL1,Xcam,
        sfmPrm->matData->prunPrm.valid,NUM_TRACKS_TOGATHER);
#else
        SFM_TI_chiralityPruning((float*)sfmPrm->camExtPrmL1,Xcam,
        sfmPrm->matData->prunPrm.valid,NUM_TRACKS_TOGATHER);
#endif

#ifdef ENABLE_PROFILE
        TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE14,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);

        TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE15,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif
      /* Transform generated 3D points, which are in world co-ordinate system,
      into current camera co-ordinate system.
      */
#ifdef DSP_INTRINSICS
      SFM_TI_matrixMultiply_3x4By4x1_ci(&sfmPrm->camExtPrmL1[CAM_EXT_PRM_BUF_DEPTH - 1][0],
        &sfmPrm->Xcam[0][0],
        (float*)sfmPrm->curTrack, // buffer reused for storing the current camera co-ordinates points
        NUM_TRACKS_TOGATHER,
        (uint32_t*)sfmPrm->pBlock);
#else
      SFM_TI_matrixMultiply_3x4By4x1(&sfmPrm->camExtPrmL1[CAM_EXT_PRM_BUF_DEPTH - 1][0],
        &sfmPrm->Xcam[0][0],
        (float*)sfmPrm->curTrack,
        NUM_TRACKS_TOGATHER);
#endif
      if(createParams->enAlgOcpGrdVis != 0x0)
      {
        if(sfmPrm->isLocalOg == 0)
        {
          //sfmPrm->curTrack --> has points in current camera co-ordinate system
          //sfmPrm->curTrackRaw --> has points in first camera co-ordinate system
#ifdef DSP_INTRINSICS
          SFM_TI_matrixMultiply_3x4By4x1_ci(&sfmPrm->firstCamExtPrmL1[0],
            &sfmPrm->Xcam[0][0],
            (float*)sfmPrm->curTrackRaw, // buffer reused for storing the first frame camera co-ordinates points
            NUM_TRACKS_TOGATHER,
            (uint32_t*)sfmPrm->pBlock);

          SFM_TI_genOccpGrid_ci((float*)sfmPrm->curTrackRaw,
                            (float*)sfmPrm->curTrack,
                            sfmPrm->matData->prunPrm.valid,
                            createParams->ocpGrdSizeInPixels,
                            sfmPrm->ogBufInt,
                            (uint32_t*)sfmPrm->matData->prunPrm.reprojErr,
                            (int8_t*)sfmPrm->matData->prunPrm.subAngle,
                            sfmPrm->pBlock);

#else
          SFM_TI_matrixMultiply_3x4By4x1(&sfmPrm->firstCamExtPrmL1[0],
            &sfmPrm->Xcam[0][0],
            (float*)sfmPrm->curTrackRaw,
            NUM_TRACKS_TOGATHER);

          SFM_TI_genOccpGrid((float*)sfmPrm->curTrackRaw,
                            (float*)sfmPrm->curTrack,
                            sfmPrm->matData->prunPrm.valid,
                            createParams->ocpGrdSizeInPixels,
                            sfmPrm->ogBufInt);
#endif
        }
        else
        {
#ifdef DSP_INTRINSICS
          SFM_TI_genOccpGrid_ci((float*)sfmPrm->curTrack,
                            (float*)sfmPrm->curTrack,
                            sfmPrm->matData->prunPrm.valid,
                            createParams->ocpGrdSizeInPixels,
                            sfmPrm->ogBufInt,
                            (uint32_t*)sfmPrm->matData->prunPrm.reprojErr,
                            (int8_t*)sfmPrm->matData->prunPrm.subAngle,
                            sfmPrm->pBlock);
#else
          SFM_TI_genOccpGrid((float*)sfmPrm->curTrack,
                            (float*)sfmPrm->curTrack,
                            sfmPrm->matData->prunPrm.valid,
                            createParams->ocpGrdSizeInPixels,
                            sfmPrm->ogBufInt);

#endif
        }

      }

#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo, TI_DSP_PROFILE_MODULE15, TI_DSP_PROFILE_END, TI_DSP_PROFILE_CUMULATIVE);

      TI_DSP_ProfileModule(&sfmPrm->prfInfo, TI_DSP_PROFILE_MODULE16, TI_DSP_PROFILE_START, TI_DSP_PROFILE_CUMULATIVE);
#endif
      /* Getting the image pixel location for each 3D points
      */
#ifdef DSP_INTRINSICS
      SFM_TI_matrixMultiply_3x3By3x1_ci(&sfmPrm->camIntPrmL1[0],
        (float*)sfmPrm->curTrack,
        (float*)sfmPrm->curTrackRaw, // buffer reused for storing image co-ordinates
        NUM_TRACKS_TOGATHER,
        (uint32_t*)sfmPrm->pBlock);
#else
      SFM_TI_matrixMultiply_3x3By3x1(&sfmPrm->camIntPrmL1[0],
        (float*)sfmPrm->curTrack,
        (float*)sfmPrm->curTrackRaw,
        NUM_TRACKS_TOGATHER);
#endif

#ifdef ENABLE_PROFILE

      TI_DSP_ProfileModule(&sfmPrm->prfInfo, TI_DSP_PROFILE_MODULE16, TI_DSP_PROFILE_END, TI_DSP_PROFILE_CUMULATIVE);


      TI_DSP_ProfileModule(&sfmPrm->prfInfo, TI_DSP_PROFILE_MODULE17, TI_DSP_PROFILE_START, TI_DSP_PROFILE_CUMULATIVE);
#endif

      float* out3dPoints  = (float*)sfmPrm->curTrack;
      float* out2dPoints  = (float*)sfmPrm->curTrackRaw;

      psfmOPut = (SFM_TI_output*)outBufs->bufDesc[SFM_TI_OUT_BUFDESC_FEATURE_PLANES]->bufPlanes[0].buf;

      /* Queuing the generated 3D points into another buffer along with other
      information
      */
      for(l = 0; l < NUM_TRACKS_TOGATHER; l+=VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR) {
        for(kk = 0; kk < VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; kk++){
          if(sfmPrm->matData->prunPrm.valid[l+kk]) {

            psfmOPut[numOut3DPoints].point3dX  = out3dPoints[0*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];
            psfmOPut[numOut3DPoints].point3dY  = out3dPoints[1*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];
            psfmOPut[numOut3DPoints].point3dZ  = out3dPoints[2*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];

            float onebyz = my_OneByX(out2dPoints[2*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk]);

            psfmOPut[numOut3DPoints].point2dX  = out2dPoints[0*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk] * onebyz;
            psfmOPut[numOut3DPoints].point2dY  = out2dPoints[1*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk] * onebyz;

#if 1//def TIADALG_SFM_DEBUG
            psfmOPut[numOut3DPoints].trackId  = sfmPrm->trackId[l+kk];
#endif
            psfmOPut[numOut3DPoints].trackAge = sfmPrm->trackInt[sfmPrm->trackId[l+kk]].age;

            numOut3DPoints ++;
          }
        }
        out3dPoints += (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*3);
        out2dPoints += (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*3);
      }

#ifdef ENABLE_PROFILE
      TI_DSP_ProfileModule(&sfmPrm->prfInfo,
        TI_DSP_PROFILE_MODULE17,
        TI_DSP_PROFILE_END,
        TI_DSP_PROFILE_CUMULATIVE);
#endif
      }
    }/*End of the track loop*/

    if (inArgsSFM->reserved0)
    {
      printf("numOut3DPoints = %d \n", numOut3DPoints);
    }

    outArgsSFM->outNumPoints = numOut3DPoints;

//#define DUMP_OUTPUT_TEXT

#ifdef DUMP_OUTPUT_TEXT
    char file_name2[200];
    static int frame_num2 = 5;
    sprintf(file_name2,"/ti/j7/workarea/tiadalg/tiadalg_structure_from_motion/test/out_%d.h",frame_num2);
    FILE* fp = fopen(file_name2, "w");

    SFM_TI_output* out_ptr = (SFM_TI_output*) outBufs->bufDesc[SFM_TI_OUT_BUFDESC_FEATURE_PLANES]->bufPlanes[0].buf;

    //sprintf(file_name2,"#pragma DATA_SECTION (outTable_%d, \".ioBufs\") \n",frame_num2);
    //fprintf(fp,"%s",file_name2);

    sprintf(file_name2,"const SFM_TI_output outTable_%d[%d] = {",frame_num2++, outArgsSFM->outNumPoints);

    fprintf(fp,"%s",file_name2);

    for(int32_t i =0 ; i < outArgsSFM->outNumPoints; i++)
    {
      fprintf(fp,"{");
      fprintf(fp,"%05.1f, %05.1f, %05.1f, %05.1f, %05.1f, %d, %d",
              out_ptr[i].point2dX, out_ptr[i].point2dY,
              out_ptr[i].point3dX, out_ptr[i].point3dY, out_ptr[i].point3dZ,
              out_ptr[i].trackAge,out_ptr[i].trackId
              );
      fprintf(fp,"},");
    }

    fprintf(fp,"}; \n\n\n");
#endif

#ifdef EN_SFM_STATIC_FRAME
    /* Static frame decision*/
    f32temp2 = 0.0f;

    f32temp1 = sfmPrm->arrangedCamExtPrmL1[VLIB_TRIANG_MAX_POINTS_IN_TRACK - 1][9] -
               sfmPrm->arrangedCamExtPrmL1[VLIB_TRIANG_MAX_POINTS_IN_TRACK - 2][9];
    f32temp2+= f32temp1*f32temp1;

    f32temp1 = sfmPrm->arrangedCamExtPrmL1[VLIB_TRIANG_MAX_POINTS_IN_TRACK - 1][11] -
               sfmPrm->arrangedCamExtPrmL1[VLIB_TRIANG_MAX_POINTS_IN_TRACK - 2][11];
    f32temp2+= f32temp1*f32temp1;

    f32temp1 = sfmPrm->arrangedCamExtPrmL1[VLIB_TRIANG_MAX_POINTS_IN_TRACK - 1][7] -
               sfmPrm->arrangedCamExtPrmL1[VLIB_TRIANG_MAX_POINTS_IN_TRACK - 2][7];
    f32temp2+= f32temp1*f32temp1;

    f32temp2 = f32temp2 * my_OneBySqrtX(f32temp2);

    if(f32temp2 > inArgsSFM->staticFrmTh){
      outArgsSFM->sfmStats.isStatic       = 0;
    }else{
      outArgsSFM->sfmStats.isStatic       = 1;
    }
#endif

  } else {

    /* Initial few frames no 3D point will be given out
    */
    outArgsSFM->outNumPoints            = 0;
    outArgsSFM->sfmStats.isStatic       = 0;

  }

  sfmPrm->num3DPoints = (uint16_t)outArgsSFM->outNumPoints;

  /*In reference flow dmaUtilsContext will be null hence no need to wait for it.
    in reference flow memory copy would have happened instead of DMA transfer
  */
  if((createParams->enAlgPtCldVis != 0) && (intAlgHandle->dmaUtilsContext!= NULL))
  {
    DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_MEMCPY);
  }

  if((createParams->enAlgOcpGrdVis != 0) && (intAlgHandle->dmaUtilsContext!= NULL))
  {
    DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_PING_PONG);
    DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);
  }

  /*In refflow no visualization drawing is done as we dont have refrence flow for this portion of the code*/
  if(((algHandle->createParams.flowCtrl & TIADALG_FLOW_CTRL_REF_ONLY) != TIADALG_FLOW_CTRL_REF_ONLY))
  {
#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE18,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif

    if((sfmPrm->num3DPoints > 0) && (createParams->enAlgPtCldVis != 0))
    {
      TIADALG_memcpy2D(sfmPrm->sfmOutputL2, // l2 temprory buffer for point cloud
            outBufs->bufDesc[SFM_TI_OUT_BUFDESC_FEATURE_PLANES]->bufPlanes[0].buf, // point cloud buffer in ddr
            sizeof(SFM_TI_output),
            sfmPrm->num3DPoints,
            sizeof(SFM_TI_output),
            sizeof(SFM_TI_output),
            TIADALG_DMA_CHANNEL_MEMCPY,
            intAlgHandle->dmaUtilsContext,
            intAlgHandle->memcpyTr,
            0x1);
    }
#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE18,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE19,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif

    /* In reference flow DMA is not enabled by having context as NULL, hence ping pong DMA can not work*/
    /* so in reference flow no visualization will be generated for now*/
    SFM_TI_visPtCldOutput(intAlgHandle,
                      createParams,
                      sfmPrm,
                      NULL,//(uint8_t*)inBufs->bufDesc[SFM_TI_IN_BUFDESC_IN_LUMA_IMG_BUFFER]->bufPlanes[0].buf,// luma plane is not needed
                      outBufs->bufDesc[SFM_TI_OUT_BUFDESC_FEATURE_PLANES]->bufPlanes[0].buf,
                      (uint8_t*)outBufs->bufDesc[SFM_TI_OUT_BUFDESC_CHROMA_PTCLD_BUFFER]->bufPlanes[0].buf);

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE19,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);
#endif

    if(createParams->enAlgOcpGrdVis != 0)
    {

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE20,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif

      uint8_t* outImgLoc = (uint8_t*)outBufs->bufDesc[SFM_TI_OUT_BUFDESC_CHROMA_OCPGD_BUFFER]->bufPlanes[0].buf;
      /*Assuming square display and double the size of occupancy grid visualization resolution, as we are working on
        YUV nv12 format, hence to represent each pixel distinctly axb resolution of occupancy grid
        needs to be mapped on 2ax2b visualization drawing.
      */
      int32_t ogVizImageSizeInPixelsW = __mmin(2*createParams->ocpGrdSizeInPixels,createParams->imgWidth);
      int32_t ogVizImageSizeInPixelsH = __mmin(2*createParams->ocpGrdSizeInPixels,createParams->imgHeight);

      /*one divide by 2 for making it center, and another one because it is chroma plane*/
      if(createParams->imgHeight > ogVizImageSizeInPixelsH)
      {
        outImgLoc += (((createParams->imgHeight>>1) - (ogVizImageSizeInPixelsH>>1)) >> 1)*createParams->imgWidth;
      }

      if(createParams->imgWidth > ogVizImageSizeInPixelsW)
      {
        outImgLoc += ((((createParams->imgWidth >> 1) - (ogVizImageSizeInPixelsW>>1))>>1)<<1);
      }

      SFM_TI_visOgOutput(intAlgHandle,
                        createParams,
                        sfmPrm,
                        outImgLoc); /*Output image location in DDR*/

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE20,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE21,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif

      SFM_TI_visEgoVehicle(intAlgHandle,
                           createParams,
                           sfmPrm,
                           sfmPrm->isLocalOg,
                           outImgLoc);
#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE21,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);
#endif

    }

  }

  /* Free the input and output buffer
  */

  if(sfmPrm->profileEn) {
    outArgsSFM->sfmStats.rsvd1 = (size_t)&sfmPrm->prfInfo;
  } else {
    outArgsSFM->sfmStats.rsvd1 = (size_t)NULL;
  }

  outArgs->inFreeBufIDs[SFM_TI_IN_BUFDESC_IN_DOF_BUFFER] =
    inBufs->bufDesc[SFM_TI_IN_BUFDESC_IN_DOF_BUFFER]->bufferId ;

  outArgs->inFreeBufIDs[SFM_TI_IN_BUFDESC_TOTAL] = 0 ;

  outArgs->outFreeBufIDs[SFM_TI_OUT_BUFDESC_FEATURE_PLANES] =
    outBufs->bufDesc[SFM_TI_OUT_BUFDESC_FEATURE_PLANES]->bufferId ;

  outArgs->outFreeBufIDs[SFM_TI_OUT_BUFDESC_TOTAL] = 0 ;

  outBufs->bufDesc[SFM_TI_OUT_BUFDESC_FEATURE_PLANES]->bufPlanes[0].accessMask = IVISION_ACCESSMODE_CPU;

  sfmPrm->curLargeBufIndx++;

  if(sfmPrm->curFrameNum < VLIB_TRIANG_MAX_POINTS_IN_TRACK){
    sfmPrm->curFrameNum++; // used for just detecting the first fram
  }

  if(sfmPrm->curLargeBufIndx == CAM_EXT_PRM_BUF_DEPTH)
    sfmPrm->curLargeBufIndx = 0x0;

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE22,TI_DSP_PROFILE_START,TI_DSP_PROFILE_CUMULATIVE);
#endif
  /*put back track data from internal memory /L3 to DDR. DDR has to be always up to date*/
  TIADALG_memcpy2D(sfmPrm->track,
                sfmPrm->trackInt,
                sizeof(SFM_TI_trackInfo),
                createParams->maxNumTracks,
                sizeof(SFM_TI_trackInfo),
                sizeof(SFM_TI_trackInfo),
                TIADALG_DMA_CHANNEL_MEMCPY,
                algHandle->dmaUtilsContext,
                algHandle->memcpyTr,
                0x1);

  if((createParams->enAlgOcpGrdVis != 0x0) && (sfmPrm->isLocalOg != 0x1))
  {
    TIADALG_memcpy2D(sfmPrm->ogBuf,
                  sfmPrm->ogBufInt,
                  createParams->ocpGrdSizeInPixels,
                  createParams->ocpGrdSizeInPixels,
                  createParams->ocpGrdSizeInPixels,
                  createParams->ocpGrdSizeInPixels,
                  TIADALG_DMA_CHANNEL_MEMCPY,
                  intAlgHandle->dmaUtilsContext,
                  intAlgHandle->memcpyTr,
                  0x1);
  }

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(&sfmPrm->prfInfo,TI_DSP_PROFILE_MODULE22,TI_DSP_PROFILE_END,TI_DSP_PROFILE_CUMULATIVE);
#endif

#if defined(ENABLE_PROFILE) && defined(TIADALG_PROFILE_PRINTS)
  if(createParams->rsvd1 != 0x0){

    prfInfo = (TI_DSP_PrfInfo*)outArgsSFM->sfmStats.rsvd1;

    printf("  Sub-Modules Profile Info for %d tracks and %d 3d points\n",numValidTracks, numOut3DPoints);
    int32_t md;

    for(md = 0; md < TI_DSP_PROFILE_MAX; md ++){
      if((prfInfo->prfData[md].moduleId != -1))
        printf("    MODULE%3d TotalCycle = %10lld TotalCount = %10d avgCycle = %5lld\n",\
        prfInfo->prfData[md].moduleId,
        prfInfo->prfData[md].totalTime,
        prfInfo->prfData[md].count,
        (prfInfo->prfData[md].totalTime/prfInfo->prfData[md].count));
    }
  }
#endif

  //SFM_TI_deactivate((IALG_Handle)(void*)algHandle);

  return status;
}

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
#if (defined(SOC_J784S4)  || defined(SOC_J742S2)) && !defined(HOST_EMULATION)
  void TIADALG_7xSetL2WBINV(uint64_t param);
  uint64_t TIADALG_7xGetL2WBINV(void);
__asm__ __volatile__("TIADALG_7xSetL2WBINV: \n"
	                 "    MVC	.S1	A4,		ECR387  ; \n"
	                 "    RET	.B1\n"
          );
__asm__ __volatile__("TIADALG_7xGetL2WBINV: \n"
	      " MVC	.S1	ECR387,	A4	; \n"
	      " RET	.B1\n;"
       );
#define TIDL_UMC_L2WBINV_WBINV_MASK                              (0x00000001ULL)
#define TIDL_UMC_L2WBINV_WBINV_SHIFT                             (0ULL)
#define TIDL_UMC_L2WBINV_WBINV_RESETVAL                          (0x00000000ULL)
#define TIDL_UMC_L2WBINV_WBINV_MAX                               (0x00000001ULL)
void TIADALG_7xCleaninvalidateL2Cache()
{
    volatile uint64_t       wbinv;
    wbinv = TIADALG_7xGetL2WBINV() & ~TIDL_UMC_L2WBINV_WBINV_MASK;
    wbinv |= (0x1U << TIDL_UMC_L2WBINV_WBINV_SHIFT) & TIDL_UMC_L2WBINV_WBINV_MASK;
    TIADALG_7xSetL2WBINV(wbinv);
    return;
}
#endif

void  SFM_TI_activate(IALG_Handle handle)
{
  SFM_TI_Handle algHandle = (SFM_TI_Handle)(handle);

  /*----------------------------------------------------------------*/
  /* Do Activation only if it is deactivated or just after creation */
  /*----------------------------------------------------------------*/
  if(algHandle->algState == (uint8_t)ALG_NOT_ACTIVE)
  {
    /* Initialize the DMA Utils Handle */
    if((algHandle->createParams.flowCtrl & TIADALG_FLOW_CTRL_REF_ONLY) == TIADALG_FLOW_CTRL_REF_ONLY)
    {
      algHandle->dmaUtilsContext = NULL;
    }
    else
    {
      TIADALG_initDmaUtils((SFM_TI_Handle)algHandle);
    }

#if (defined(SOC_J784S4)  || defined(SOC_J742S2)) && !defined(HOST_EMULATION)
    Cache_wbInvL1dAll();
    TIADALG_7xCleaninvalidateL2Cache();
#endif

    SFM_TI_Handle intAlgHandle = (SFM_TI_Handle)algHandle->intAlgHandle;

    TIADALG_memcpy2D(intAlgHandle,
                  (const void  *)algHandle->memRec[ALG_HANDLE_MEMREC].base,
                  algHandle->memRec[ALG_HANDLE_MEMREC].size,
                  1U,
                  0,
                  0,
                  TIADALG_DMA_CHANNEL_MEMCPY,
                  algHandle->dmaUtilsContext,
                  algHandle->memcpyTr,
                  0x1);

    SFM_TI_CreateParams * createParams = (SFM_TI_CreateParams*)&intAlgHandle->createParams;
    sSfm_TI_ExtMem* sfmPrm     = (sSfm_TI_ExtMem*)intAlgHandle->memRec[ALG_EXT_MEMREC].base;
    sfmPrm->track              = (SFM_TI_trackInfo*) intAlgHandle->memRec[ALG_TRACK_MEMREC].base;
    sfmPrm->trackInt           = (SFM_TI_trackInfo*) intAlgHandle->memRec[ALG_TRACK_INT_MEMREC].base;

    /*Bring track data from DDR to internal memory /L3. */
    TIADALG_memcpy2D(sfmPrm->trackInt,
                  sfmPrm->track,
                  sizeof(SFM_TI_trackInfo),
                  createParams->maxNumTracks,
                  sizeof(SFM_TI_trackInfo),
                  sizeof(SFM_TI_trackInfo),
                  TIADALG_DMA_CHANNEL_MEMCPY,
                  intAlgHandle->dmaUtilsContext,
                  intAlgHandle->memcpyTr,
                  0x1);

    if((createParams->enAlgOcpGrdVis != 0x0)&&(sfmPrm->isLocalOg != 0x1))
    {
      TIADALG_memcpy2D(sfmPrm->ogBufInt,
                    sfmPrm->ogBuf,
                    createParams->ocpGrdSizeInPixels,
                    createParams->ocpGrdSizeInPixels,
                    createParams->ocpGrdSizeInPixels,
                    createParams->ocpGrdSizeInPixels,
                    TIADALG_DMA_CHANNEL_MEMCPY,
                    intAlgHandle->dmaUtilsContext,
                    intAlgHandle->memcpyTr,
                    0x1);
    }

    algHandle->algState = (uint8_t)ALG_ACTIVE;
  }


  return;
}

/**
* @func SFM_TI_deactivate
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
void  SFM_TI_deactivate(IALG_Handle handle)
{
  SFM_TI_Handle algHandle = (SFM_TI_Handle)(void*)(handle);
  SFM_TI_Handle intAlgHandle = (SFM_TI_Handle)(void*)(algHandle->intAlgHandle);

  if(algHandle->algState == (uint8_t)ALG_ACTIVE)
  {
    if(NULL != intAlgHandle)
    {

      /*algHandle in internal memeory is not updated by algorithm so whatever was in DDR is up to date only so no need to
        put back in DDR.
      */

      /*TIADALG_memcpy2D(intAlgHandle->memRec[ALG_HANDLE_MEMREC].base,
                  intAlgHandle,
                  intAlgHandle->memRec[ALG_HANDLE_MEMREC].size,
                  1U,
                  0,
                  0,
                  TIADALG_DMA_CHANNEL_MEMCPY,
                  intAlgHandle->dmaUtilsContext,
                  intAlgHandle->memcpyTr);*/

      /* Release the dma resources*/
      if (intAlgHandle->dmaUtilsContext != NULL )
      {
        (void)DmaUtilsAutoInc3d_deinit(intAlgHandle->dmaUtilsContext);
      }
    }
    algHandle->algState = (uint8_t)ALG_NOT_ACTIVE;
  }
}

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
int32_t SFM_TI_free(IALG_Handle handle, IALG_MemRec *memRec)
{
  int32_t status            = IALG_EOK;
  SFM_TI_Handle algHandle = (SFM_TI_Handle)(handle);

  memcpy(memRec, algHandle->memRec, sizeof(IALG_MemRec)*algHandle->numMemRecs);

  return status;
}

void SFM_TI_updatePtrs(sSfm_TI_L1DMem* sfmL1Prm, sSfm_TI_L2DMem* sfmL2Prm, sSfm_TI_ExtMem* sfmPrm, SFM_TI_CreateParams * createParams)
{
  size_t endPtrFmat;
  size_t endPtrTrack;

  endPtrFmat                 = (size_t)sfmL2Prm;
  endPtrFmat                 = (endPtrFmat + 0x7) & (~0x7);

  sfmPrm->isInlier           = (uint8_t*)endPtrFmat;
  endPtrFmat                += sfmPrm->maxNumTracks;
  endPtrFmat                 = (endPtrFmat + 0x7) & (~0x7);

  sfmPrm->indexMap           = (uint16_t *)endPtrFmat;
  endPtrFmat                += sfmPrm->maxNumTracks * sizeof(uint16_t);
  endPtrFmat                 = (endPtrFmat + 0x7) & ~(0x7);

  sfmPrm->curPoints          = (float *)endPtrFmat;
  endPtrFmat                += sfmPrm->maxNumTracks * sizeof(float)*2;
  endPtrFmat                 = (endPtrFmat + 0x7) & ~(0x7);

  sfmPrm->prevPoints         = (float *)endPtrFmat;
  endPtrFmat                += sfmPrm->maxNumTracks * sizeof(float)*2;
  endPtrFmat                 = (endPtrFmat + 0x7) & ~(0x7);

  if(endPtrFmat > (((size_t)sfmL2Prm) + L2_SCRATCH_BUF_SIZE))
  {
    assert(0);
  }

  if(sizeof(sSfm_TI_L2DMem) > L2_SCRATCH_BUF_SIZE)
  {
    assert(0);
  }

  /*track maintainace related L2 alloaction. Assumed that this size is less than Fmat pruning L2 requirement*/
  endPtrTrack                 = (size_t)sfmL2Prm;

  sfmPrm->keyPointBitMap      = (uint8_t*)endPtrTrack;
  endPtrTrack                += sfmPrm->keyPointBitMapSize;
  endPtrTrack                 = (endPtrTrack + 0x7) & (~0x7);

  sfmPrm->curInTrackIdx       = (uint16_t*)endPtrTrack;
  endPtrTrack                += sfmPrm->trackIndxBufSize;
  endPtrTrack                 = (endPtrTrack + 0x7) & (~0x7);

  sfmPrm->curOutTrackIdx      = (uint16_t*)endPtrTrack;
  endPtrTrack                += sfmPrm->trackIndxBufSize;
  endPtrTrack                 = (endPtrTrack + 0x7) & (~0x7);

  sfmPrm->dofBufInt           = (uint8_t*)endPtrTrack;
  endPtrTrack                += sfmPrm->dofBlockBufSize*2; // 2 for ping pong
  endPtrTrack                 = (endPtrTrack + 0x7) & (~0x7);

  if(endPtrTrack > (((size_t)sfmL2Prm) + L2_SCRATCH_BUF_SIZE))
  {
    assert(0);
  }

  /*Visualization related*/
  int32_t l2PtCldSize;
  if(createParams->enAlgPtCldVis != 0)
  {
    endPtrTrack                 = (size_t)sfmL2Prm;

    if(createParams->maxNumTracks <= 15000)
    {
      sfmPrm->sfmOutputL2         = (SFM_TI_output*)endPtrTrack;
      endPtrTrack                += createParams->maxNumTracks * sizeof(SFM_TI_output);
      l2PtCldSize = sizeof(SFM_TI_output);
    }
    else
    {
      sfmPrm->sfmOutputL2         = (SFM_TI_output*)sfmPrm->sfmL3Scratch;
      l2PtCldSize = 0;
    }

    sfmPrm->colorIndx           = (uint8_t*)endPtrTrack;
    endPtrTrack                += createParams->maxNumTracks;
    sfmPrm->blkIndx             = (uint16_t*)endPtrTrack;
    endPtrTrack                += 2*createParams->maxNumTracks;
    sfmPrm->curBlkIndx          = (uint16_t*)endPtrTrack;
    endPtrTrack                += 2*createParams->maxNumTracks;

    int32_t availbleL2Size      = L2_SCRATCH_BUF_SIZE - (createParams->maxNumTracks * (l2PtCldSize + 5)); // 3 for colorIndx,blkIndx,curBlkIndx
    int32_t oneRowSizeImg1;
    int32_t oneRowSizeImg2;

    if(createParams->imgColorFmt == 0)// yuvn12
    {
      oneRowSizeImg1 = 0; // no luma ia needed in drawing function, as only chroma plane is generated
      oneRowSizeImg2 = createParams->imgWidth; // cbcr interleaved of  size = wxh/2
    }
    else
    {
      oneRowSizeImg1 = (createParams->imgWidth)*3; // 3 for BGR channels interleaved
      oneRowSizeImg2 = 0;
    }

    if(availbleL2Size < (oneRowSizeImg1 + oneRowSizeImg2)*2) // 2 ping pong
    {
      /*if not a single row of image can not be accomodated in ping pong*/
      assert(0);
    }

    sfmPrm->numRowInPtCldVisBuf = (int32_t)(((int32_t)(availbleL2Size/(oneRowSizeImg1 + oneRowSizeImg2)))/2); // 2 for ping pong

    /*ping pong dim has to be 32b singed so no need of below code*/
    /*while((sfmPrm->numRowInPtCldVisBuf*createParams->imgWidth) > (int32_t)(0xFFFF))
    {
      sfmPrm->numRowInPtCldVisBuf--;
    }*/

    /* chroma visualization writes two pixel. So  one line blow get written outside ping pong block.
       instead of checking last row of pong, one exatra line is left so that two lines writing at the
       last row of png block will not overwrite next data.
    */
    sfmPrm->numRowInPtCldVisBuf -= 1;

    sfmPrm->tempImgPtCldL2       = (uint8_t*)endPtrTrack;
  }
  else
  {
    sfmPrm->tempImgPtCldL2 = NULL;
  }

  if((createParams->enAlgOcpGrdVis != 0))
  {

   /*Assuming square display and double the size of occupancy grid visualization resolution, as we are working on
     YUV nv12 format, hence to represent each pixel distinctly axb resolution of occupancy grid
     needs to be mapped on 2ax2b visualization drawing.
   */

    int32_t ogVizImageSizeInPixelsW = __mmin(2*createParams->ocpGrdSizeInPixels,createParams->imgWidth);
    int32_t ogVizImageSizeInPixelsH = __mmin(2*createParams->ocpGrdSizeInPixels,createParams->imgHeight);

    /* Divide by two for ping pong*/

    sfmPrm->numRowInOgVisBuf = (L2_SCRATCH_BUF_SIZE >> 1)/(ogVizImageSizeInPixelsW);

    /* divide by 2 because of cbcr plane will be of w X (h/2)
    */
    if(sfmPrm->numRowInOgVisBuf > ((ogVizImageSizeInPixelsH >> 1)))
    {
      sfmPrm->numRowInOgVisBuf = (ogVizImageSizeInPixelsH >> 1);
    }

    // complete L2 can be used in this scenario
    sfmPrm->tempImgOgL2 = (uint8_t*)sfmL2Prm;

  }
  else
  {
    sfmPrm->tempImgOgL2 = NULL;
  }

  // l2 static buffer pointers
  sfmPrm->curTrack         = sfmL2Prm->curTrack;
  sfmPrm->curTrackRaw      = sfmL2Prm->curTrackRaw;
  sfmPrm->trackId          = sfmL2Prm->trackId;
  sfmPrm->curFeatInTrack   = sfmL2Prm->curFeatInTrack;
  sfmPrm->Xcam             = sfmL2Prm->Xcam;

  //l1 static buffer pointers
  sfmPrm->camExtPrmL1         = sfmL1Prm->camExtPrm;
  sfmPrm->camInvExtPrmL1      = sfmL1Prm->camInvExtPrm;
  sfmPrm->arrangedCamExtPrmL1 = sfmL1Prm->arrangedCamExtPrm;
  sfmPrm->camIntXExtPrmL1     = sfmL1Prm->camIntXExtPrm;
  sfmPrm->camInvIntPrmL1      = sfmL1Prm->camInvIntPrm;
  sfmPrm->camIntPrmL1         = sfmL1Prm->camIntPrm;
  sfmPrm->firstCamExtPrmL1    = sfmL1Prm->firstCamExtPrm;

  sfmPrm->matData = &sfmL2Prm->matData;
  sfmPrm->pBlock  = sfmL2Prm->pBlock;

  sfmPrm->sfmL1Prm = sfmL1Prm;
  sfmPrm->sfmL2Prm = sfmL2Prm;

}

