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
*  @file        sfm_ti_track.c
*  @author      Deepak Poddar
*  @brief       This file defines the XDAIS/iVISION functions for Feature classification applet
*  @version     0.0 (Jan 2014) : Base version.
*/

#pragma CHECK_MISRA ("none")
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#pragma RESET_MISRA ("required")

#include "tiadalg_interface.h"
#include "sfm_ti_alg_int.h"
#include "sfm_ti_track.h"
#include "sfm_ti_math.h"
#include "sfm_ti_device_utils.h"
#include <assert.h>

/* returns current track length if track is valid.
Otherwise return zero.
*/

void SFM_TI_getCurTrack(SFM_TI_trackInfo* baseTrackInfo,
                        float*            camInvIntPrm,
                        uint16_t*         trackId,
                        float*            curTrack,
                        float*            curTrackRaw,
                        uint8_t*          curFeatInTrack)
{
  uint16_t  j,l,k;
  int16_t   curTrackIndex;
  float x0,y0;
  SFM_TI_trackInfo* curTrackInfo;
  uint16_t trackId0,trackVecAge;

  float ftemp1 = 0.0625f; // one by 16

  for(l = 0; l < NUM_TRACKS_TOGATHER; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR){

    trackVecAge = 0;
    for(k = 0; k < VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++){
      trackVecAge          = __mmax(trackVecAge, curFeatInTrack[l+k]);
    }

    /* Oldest key point index (j). Oldest key point is kept at '0' index of
    * curTrack
    *                     j = 0                                               j = 6
    * Format is [x_15(-6) .. x_0(-6) y_15(-6) .. y_0(-6)] ...........[x_15(-0) .. x_0(-0) y_15(-0) .. y_0(-0)]
    * oldest point index = cur_idx - age + 1
    */
    for(j = (VXLIB_TRIANG_MAX_POINTS_IN_TRACK - trackVecAge); j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK ; j++){
      for(k = 0; k < VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++){
        trackId0             = trackId[l + k];
        curTrackInfo         = baseTrackInfo + trackId0;
        curTrackIndex        = curTrackInfo->idx  - (VXLIB_TRIANG_MAX_POINTS_IN_TRACK - j) + 1; // indx - age + 1

        curTrackIndex        = (curTrackIndex + VXLIB_TRIANG_MAX_POINTS_IN_TRACK) % VXLIB_TRIANG_MAX_POINTS_IN_TRACK;

        x0 = (float)curTrackInfo->x[curTrackIndex] * ftemp1;
        y0 = (float)curTrackInfo->y[curTrackIndex] * ftemp1;

        curTrackRaw[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k] = x0;

        curTrack[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k] =
          camInvIntPrm[0]*x0 + camInvIntPrm[1]*y0 + camInvIntPrm[2]*1;


        curTrackRaw[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k] = y0;

        curTrack[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + k] =
          camInvIntPrm[3]*x0 + camInvIntPrm[4]*y0 + camInvIntPrm[5]*1;
      }
    }

    curTrack     += 2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
    curTrackRaw  += 2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
  }

  return;
}

void SFM_TI_getCurTrack_ci(SFM_TI_trackInfo* restrict baseTrackInfo,
                        float*            restrict camInvIntPrm,
                        uint16_t*         restrict trackId,
                        float*            restrict curTrack,
                        float*            restrict curTrackRaw,
                        uint8_t*          restrict curFeatInTrack)
{
  uint16_t  j,l,k;
  int16_t   curTrackIndex;
  float x0,y0;
  SFM_TI_trackInfo* curTrackInfo;
  uint16_t trackId0,trackVecAge;

  float ftemp1 = 0.0625f; // one by 16

  for(l = 0; l < NUM_TRACKS_TOGATHER; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR){

    trackVecAge = 0;
    for(k = 0; k < VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++){
      trackVecAge          = __mmax(trackVecAge, curFeatInTrack[l+k]);
    }

    /* Oldest key point index (j). Oldest key point is kept at '0' index of
    * curTrack
    *                     j = 0                                               j = 6
    * Format is [x_15(-6) .. x_0(-6) y_15(-6) .. y_0(-6)] ...........[x_15(-0) .. x_0(-0) y_15(-0) .. y_0(-0)]
    * oldest point index = cur_idx - age + 1
    */
    for(j = (VXLIB_TRIANG_MAX_POINTS_IN_TRACK - trackVecAge); j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK ; j++){
      float* restrict curTrack_x = &curTrack[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + 0];
      float* restrict curTrack_y = &curTrack[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + 0];
      float* restrict curTrackRaw_x = &curTrackRaw[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + 0];
      float* restrict curTrackRaw_y = &curTrackRaw[j*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + 0];

      for(k = 0; k < VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++){
        trackId0             = trackId[l + k];
        curTrackInfo         = baseTrackInfo + trackId0;
        curTrackIndex        = curTrackInfo->idx  - (VXLIB_TRIANG_MAX_POINTS_IN_TRACK - j) + 1; // indx - age + 1

        if(curTrackIndex < 0 ){
          curTrackIndex = curTrackIndex + VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
        }

        x0 = (float)curTrackInfo->x[curTrackIndex] * ftemp1;
        y0 = (float)curTrackInfo->y[curTrackIndex] * ftemp1;

        *curTrackRaw_x++ = x0;
        *curTrackRaw_y++ = y0;

        *curTrack_x++ = camInvIntPrm[0]*x0 + camInvIntPrm[1]*y0 + camInvIntPrm[2];
        *curTrack_y++ = camInvIntPrm[3]*x0 + camInvIntPrm[4]*y0 + camInvIntPrm[5];

      }
    }

    curTrack     += 2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
    curTrackRaw  += 2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
  }

  return;
}

void SFM_TI_updateTrackRef(SFM_TI_trackInfo *track, int32_t* dofPlane, uint16_t* curInTrackIdx, uint8_t* keyPointBitMap,
                        int32_t imgWidth, int32_t imgHeight, int32_t kpStep, int32_t maxNumTracks,
                        int32_t flowInvalidPadX, int32_t flowInvalidPadY, int32_t curFrameNum,
                        int32_t flowConfThr, TI_DSP_PrfInfo* prfInfo)
{
  int32_t i, j, k, l, m, n;

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE1,
                        TI_DSP_PROFILE_START,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif
  /***************************************************************************/
  /**************** Main Processing starts Here ******************************/
  /***************************************************************************/

  /*keypoint map creation, each bit at one locatoin of keypoint*/
  int32_t imgHeightByKpStep = (imgHeight + 1 - 1)/kpStep;
  //int32_t imgHeightByKpStep = (imgHeight + kpStep - 1)/kpStep;
  int32_t imgWidthByKpStep  = imgWidth/kpStep;
  int32_t numKpInRow        = (imgWidthByKpStep + 7) & 0xFFFFFFF8U; // number of kp in a row shuold be multiple of 8 or bytes
  int32_t kpBytesInRow      = numKpInRow >> 3;
  int32_t numDetKp          = 0;

  /*generating valid bit field for all key points placed at gird of kpStep x kpStep*/
  for(i = 0; i < imgHeightByKpStep; i++)
  {
    for(j = 0; j < kpBytesInRow; j++)
    {
      if(j == (kpBytesInRow - 1))
      {
        keyPointBitMap[i*kpBytesInRow + j] = (0x1 << (imgWidthByKpStep - j*8)) - 1;
        numDetKp += (imgWidthByKpStep - j*8);
      }
      else
      {
        keyPointBitMap[i*kpBytesInRow + j] = 0xffu;
        numDetKp += 8;
      }
    }
  }

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE1,
                        TI_DSP_PROFILE_END,
                        TI_DSP_PROFILE_CUMULATIVE);

  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE2,
                        TI_DSP_PROFILE_START,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif

  SFM_TI_trackInfo * trackL = track;

  if(curFrameNum == 0)
  {
    for(i =0; i < maxNumTracks; i++)
    {
      trackL[i].age = 255;
      trackL[i].idx = 255;
    }
    for(i = 0; i < imgHeightByKpStep; i++)
    {
      for(j = 0; j < imgWidthByKpStep; j++)
      {
        trackL->age = 1; // age and idx packed. 4 bit each. age tells how many points are valid, and idx tells latest point index.
        trackL->idx = 0;
        trackL->x[trackL->idx] = (uint16_t)((j*kpStep) << 4); // i and j in Q4 format
        trackL->y[trackL->idx] = (uint16_t)((i*kpStep) << 4);
        trackL++;
      }
    }
  }
  else
  {
    m = 0;
    n = 0;
    for(i = 0; i < maxNumTracks; i++)
    {
      if(trackL->idx != 0xFF)
      {
        // ageIdxPkd == 0xFF means this track is empty.
        // Do below processing for non empty tracks, even if age = 1

        int32_t curIdx     = trackL->idx;
        int32_t age        = trackL->age;

        if(curIdx >= MAX_NUM_FRAMES_TO_TRACK)
        {
          curIdx = 0;
        }

        int32_t xOldInt    = (trackL->x[curIdx] >> 4);
        int32_t yOldInt    = (trackL->y[curIdx] >> 4);
        int32_t curPkdFlow = dofPlane[(yOldInt*imgWidth + xOldInt)];

        /* |upper 16 bit horizontal flow|12 bit vertical flow| lowest 4 bit confidence|*/
        int32_t hFlow = (int32_t)((int32_t)((int32_t)curPkdFlow + 0) >> 16);
        int32_t vFlow = (int32_t)((((int32_t)((int32_t)curPkdFlow + 0) << 16) + 0)>> 20);
        int32_t conf  = (uint32_t)(curPkdFlow << 28) >> 28;

        int32_t xNew     = trackL->x[curIdx] + hFlow; // addition in Q4 format
        int32_t yNew     = trackL->y[curIdx] + vFlow; // addition in Q4 format
        int32_t xNewInt  = xNew >> 4;
        int32_t yNewInt  = yNew >> 4;

        /* For boundary region DOF gnerated data will  not be of good quality hence ignoring it*/
        if((xOldInt < flowInvalidPadX) || (xOldInt > (imgWidth - 1 - flowInvalidPadX)) ||
           (yOldInt < flowInvalidPadY) || (yOldInt > (imgHeight - 1 - flowInvalidPadY)))
        {
             conf = 0;
        }

        /*If the new location is still in the picture boundary and confidence is higher than threshold*/
        if((xNewInt >= 0) && (xNewInt <= (imgWidth - 1)) && (yNewInt >= 0) && (yNewInt <= (imgHeight - 1))
           && ((conf * TIADALG_DOF_CONFIDENCE_TO_UINT8_SCALING) > flowConfThr))
        {
          /*need to find the static frame, and stop updating the tracks. Also wrap around of tracks needs to be taken care*/
          curIdx++;
          age++;

          if(curIdx >= MAX_NUM_FRAMES_TO_TRACK){
            curIdx = 0;
          }

          if(age >= MAX_NUM_FRAMES_TO_TRACK){
            age = MAX_NUM_FRAMES_TO_TRACK;
          }

          trackL->age       = age;
          trackL->idx       = curIdx;
          trackL->x[curIdx] = (uint16_t)xNew;
          trackL->y[curIdx] = (uint16_t)yNew;

          // Get new flow and update the tracks
          // Check the validity of new locations. This may create holes
          // update the key point binary map
          int32_t nearestKpx  = ((xNewInt + (kpStep >> 1))/kpStep)*kpStep;
          int32_t nearestKpy  = ((yNewInt + (kpStep >> 1))/kpStep)*kpStep;

          if((abs(nearestKpx - xNewInt) < 3) && (abs(nearestKpy - yNewInt) < 3)){
            // nearst kp needs to be droped
            int32_t keyPointBitMapLoc = (nearestKpy/kpStep)*numKpInRow + nearestKpx/kpStep; // location of kp in the domain of w/step, h/step
            uint8_t kpByte = keyPointBitMap[keyPointBitMapLoc>>3];
            kpByte = kpByte & (~(0x1 << (keyPointBitMapLoc % 8)));// same nearest kp may be dropped multiple times, hence kPByte might not change always after this operation
            keyPointBitMap[keyPointBitMapLoc>>3] = kpByte;
          }
          m++;
        }else{
          trackL->age = 255;
          trackL->idx = 255;
          n++;
        }
      }
      trackL++;
    }
#ifdef TIADALG_TRACK_DEBUG
    printf("\n");
    printf("Previous Total Tracks (age >=1)     :: %05d \n", (m+n));
    printf("Current Total Tracks                :: %05d \n",m);
    printf("Current Total Tracks dropped        :: %05d \n",n);
#endif
    // get the location and count of age zero locations, basically free tracks
    trackL = track;
    int32_t numFreeTrack = 0;

    for(i = 0; i < maxNumTracks; i++){
      if(trackL->idx == 255){
        curInTrackIdx[numFreeTrack] = (uint16_t)i;
        numFreeTrack++;
      }
      trackL++;
    }

    // Count the number of valid key points
    // get count of active key points
    int32_t validNumKp = 0;//numDetKp - droppedKpCnt;

    for(i = 0, k=0; i < imgHeightByKpStep; i++, k++){ // kpStepFact will skip horizontal lines
      for(j = 0; j <  kpBytesInRow; j++){
        uint8_t curKpMap = keyPointBitMap[k*kpBytesInRow + j];
        for(l = 0; l < 8; l++){
          if((((curKpMap >> l) & 0x1) == 0x1))
          {
            validNumKp++;
          }
        }
      }
    }


    float skipCnt = ((float)(validNumKp > numFreeTrack ? (validNumKp - numFreeTrack) : 0))/validNumKp; // fraction of key points to be dropped

#ifdef TIADALG_TRACK_DEBUG
    printf("***********************************\n");
    printf("Total empty Tracks         :: %05d \n",numFreeTrack);
    printf("Total non empty Tracks     :: %05d \n",maxNumTracks - numFreeTrack);
    printf("Total valid kps            :: %05d \n",validNumKp);
#endif

    m = 0;
    n = 0;
    for(i = 0, k=0; i < imgHeightByKpStep; i++, k++){ // kpStepFact will skip horizontal lines
      for(j = 0; j <  kpBytesInRow; j++){
        uint8_t curKpMap = keyPointBitMap[k*kpBytesInRow + j];
        for(l = 0; l < 8; l++){
          if((((curKpMap >> l) & 0x1) == 0x1))
          {
            if(((int32_t)(n*skipCnt)) == ((int32_t)((n+1)*skipCnt))) // similar to if((n%skipCntFreq) !=0 )
            {
              track[curInTrackIdx[m]].age  = 1;
              track[curInTrackIdx[m]].idx  = 0;
              track[curInTrackIdx[m]].x[0] = (uint16_t)(((j*8 + l)*kpStep) << 4);
              track[curInTrackIdx[m]].y[0] = (uint16_t)((i*kpStep) << 4);
              m++;
              if(m > numFreeTrack){
                break;
                }
            }
            n++;
          }
        }
      }
    }

#ifdef TIADALG_SFM_DEBUG
  printf("value of m =%d, n=%d \n",m,n);
#endif

  }

#ifdef TIADALG_TRACK_DUMP
  SFM_TI_dumpTrackData(track, maxNumTracks);
#endif
#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE2,
                        TI_DSP_PROFILE_END,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif

}

void SFM_TI_updateTrack(SFM_TI_trackInfo * restrict track, int32_t* restrict dofPlane, uint16_t* restrict curInTrackIdx,
                        uint16_t* restrict curOutTrackIdx, int32_t* restrict dofBufInt, uint8_t* restrict keyPointBitMap,
                        int32_t imgWidth, int32_t imgHeight, int32_t kpStep,
                        int32_t maxNumTracks, int32_t flowInvalidPadX, int32_t flowInvalidPadY,
                        int32_t curFrameNum,int32_t flowConfThr, int32_t keyPointBitMapSize,
                        int32_t dofBlockBufSize, int32_t numDofBlocks, int32_t numRowInDofBlk,
                        uint8_t* restrict inTrMem, void * dmaUtilsContext, TI_DSP_PrfInfo* restrict prfInfo)
{
  int32_t i, j, k, l, m, n;
  float oneByKpStep = 1.0f/kpStep;

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE1,
                        TI_DSP_PROFILE_START,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif

  /***************************************************************************/
  /**************** Main Processing starts Here ******************************/
  /***************************************************************************/
#ifdef TIADALG_TRACK_PROFILE
  long long t0, t1, t2, t3;
  long long acc0=0;
  long long acc1=0;
  long long acc2=0;
  long long acc3=0;
  long long acc4=0;
  long long acc5=0;
  long long acc6=0;
#endif

  /*keypoint map creation, each bit at one locatoin of keypoint*/
  int32_t imgHeightByKpStep = (imgHeight + 1 - 1)*oneByKpStep;
  //int32_t imgHeightByKpStep = (imgHeight + kpStep - 1)/kpStep;
  int32_t imgWidthByKpStep  = imgWidth*oneByKpStep;
  int32_t numKpInRow        = (imgWidthByKpStep + 7) & 0xFFFFFFF8U; // number of kp in a row shuold be multiple of 8 or bytes
  int32_t kpBytesInRow      = numKpInRow >> 3;
           //dofPlane        += 4; // TIFC header are 16 elements
  int32_t numDetKp          = 0;

#ifdef TIADALG_TRACK_DEBUG

  printf("imgHeightByKpStep = %d \n", imgHeightByKpStep);
  printf("imgWidthByKpStep = %d \n", imgWidthByKpStep);
  printf("numKpInRow = %d \n", numKpInRow);
  printf("kpBytesInRow = %d \n", kpBytesInRow);
  printf("imgWidth = %d \n", imgWidth);
  printf("imgHeight = %d \n", imgHeight);
  printf("curFrameNum = %d \n", curFrameNum);
  printf("flowInvalidPadX = %d \n", flowInvalidPadX);
  printf("flowInvalidPadY = %d \n", flowInvalidPadY);

  printf("dofPlane[16] = %d \n", dofPlane[16]);
  printf("dofPlane[160] = %d \n", dofPlane[160]);
  printf("dofPlane[320] = %d \n", dofPlane[320]);
  printf("dofPlane[1024] = %d \n", dofPlane[1024]);

  printf("track[16] = %d \n", track[16].x[0]);
  printf("track[160] = %d \n", track[160].x[0]);
  printf("track[320] = %d \n", track[320].x[0]);
  printf("track[1024] = %d \n", track[1024].x[0]);
#endif

#ifdef TIADALG_TRACK_PROFILE
  t0 = __TSC;
  t2 = __TSC;
#endif

  /*generating valid bit field for all key points placed at gird of kpStep x kpStep*/
  for(i = 0; i < imgHeightByKpStep; i++)
  {
    for(j = 0; j < kpBytesInRow; j++)
    {
      if(j == (kpBytesInRow - 1))
      {
        keyPointBitMap[i*kpBytesInRow + j] = (0x1 << (imgWidthByKpStep - j*8)) - 1;
        numDetKp += (imgWidthByKpStep - j*8);
      }
      else
      {
        keyPointBitMap[i*kpBytesInRow + j] = 0xffu;
        numDetKp += 8;
      }
    }
  }

#ifdef TIADALG_TRACK_PROFILE
  t1 = __TSC;
  acc0 += (t1-t0);
#endif

  assert((imgHeightByKpStep*kpBytesInRow) == keyPointBitMapSize);

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE1,
                        TI_DSP_PROFILE_END,
                        TI_DSP_PROFILE_CUMULATIVE);

  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE2,
                        TI_DSP_PROFILE_START,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif

  SFM_TI_trackInfo * restrict trackL = track;
  j = 0;

  if(curFrameNum == 0)
  {

    for(i =0; i < maxNumTracks; i++)
    {
      trackL[i].age = 255;
      trackL[i].idx = 255;
    }

    for(i = 0; i < imgHeightByKpStep; i++)
    {
      for(j = 0; j < imgWidthByKpStep; j++)
      {
        trackL->age = 1; // age and idx packed. 4 bit each. age tells how many points are valid, and idx tells latest point index.
        trackL->idx = 0;
        trackL->x[trackL->idx] = (uint16_t)((j*kpStep) << 4); // i and j in Q4 format
        trackL->y[trackL->idx] = (uint16_t)((i*kpStep) << 4);
        trackL++;
      }
    }
  }
  else
  {

    TIADALG_updatePingPong3DTr((uint8_t*)dofPlane, (uint8_t*)dofBufInt, inTrMem,
                              imgWidth, imgHeight, imgWidth,
                              imgWidth, numRowInDofBlk, imgWidth,
                              imgWidth*numRowInDofBlk, 0x0, sizeof(uint32_t),
                              TIADALG_DMA_CHANNEL_PING_PONG,dmaUtilsContext);

    DmaUtilsAutoInc3d_configure(dmaUtilsContext, TIADALG_DMA_CHANNEL_PING_PONG, inTrMem, 1U);


    m = 0;
    n = 0;

    trackL = track;

#ifdef TIADALG_TRACK_PROFILE
  t0 = __TSC;
#endif
    float oneByNumRowsInDofBlk = 1.0f/numRowInDofBlk;

    for(i = 0; i < maxNumTracks; i++)
    {
      int32_t curIdx       = trackL->idx;
      int32_t xOldInt      = (trackL->x[curIdx] >> 4);
      int32_t yOldInt      = (trackL->y[curIdx] >> 4);
      int32_t isValidTrack = 1;
      int32_t blockId      = yOldInt * oneByNumRowsInDofBlk;

      /* For boundary region DOF gnerated data will  not be of good quality hence ignoring it*/
      if(xOldInt < flowInvalidPadX)
      {
        isValidTrack = 0;
      }

      if(yOldInt < flowInvalidPadY)
      {
        isValidTrack = 0;
      }

      if(xOldInt > (imgWidth - 1 - flowInvalidPadX))
      {
        isValidTrack = 0;
      }

      if(yOldInt > (imgHeight - 1 - flowInvalidPadY))
      {
        isValidTrack = 0;
      }

      if(curIdx == 0xFF)
      {
        isValidTrack = 0;
      }

      if(isValidTrack == 1)
      {
        curInTrackIdx[i] = blockId;
        j++;
      }
      else
      {
        curInTrackIdx[i] = 0xffff;
      }

      trackL++;
    }

#ifdef TIADALG_TRACK_PROFILE
  t1 = __TSC;
  acc1 += (t1-t0);
#endif

#ifdef TIADALG_TRACK_DEBUG
    printf(" toatal valid tracks are %d \n",j);
    printf(" curInTrackIdx[100] %d \n", curInTrackIdx[100]);
    printf(" curInTrackIdx[1000] %d \n", curInTrackIdx[1000]);
#endif

    DmaUtilsAutoInc3d_trigger(dmaUtilsContext, TIADALG_DMA_CHANNEL_PING_PONG);
    DmaUtilsAutoInc3d_wait(dmaUtilsContext, TIADALG_DMA_CHANNEL_PING_PONG);

    for(l = 0; l < numDofBlocks; l++)
    {
      int32_t curDofBlkOffset = l*numRowInDofBlk*imgWidth;

#ifdef TIADALG_TRACK_PROFILE
  t0 = __TSC;
#endif

      if( l < (numDofBlocks-1))
      {
        DmaUtilsAutoInc3d_trigger(dmaUtilsContext, TIADALG_DMA_CHANNEL_PING_PONG);
      }

      j = 0; // number of valid track to be updated from current block of dof

      for(i = 0; i < maxNumTracks; i++)
      {
        if(curInTrackIdx[i] == l)
        {
          curOutTrackIdx[j] = i;
          j++;
        }
      }

#ifdef TIADALG_TRACK_PROFILE
  t1 = __TSC;
  acc2 += (t1-t0);
#endif

#ifdef TIADALG_TRACK_DEBUG
      printf("l = %d total tracks in this block are %d \n", l, j);
#endif

#ifdef TIADALG_TRACK_PROFILE
  t0 = __TSC;
#endif

      for(i = 0; i < j; i++)
      {

#ifdef TIADALG_UPDATE_TARCK_DEBUG
        printf("trackIndx processed used is %d \n",i, curOutTrackIdx[i]);
#endif
        trackL = track + curOutTrackIdx[i];
        // ageIdxPkd == 0xFF means this track is empty.
        // Do below processing for non empty tracks, even if age = 1
        int32_t curIdx     = trackL->idx;
        int32_t age        = trackL->age;

        int32_t xOldInt    = (trackL->x[curIdx] >> 4);
        int32_t yOldInt    = (trackL->y[curIdx] >> 4);
        int32_t flowIdx    = (yOldInt*imgWidth + xOldInt);

        int32_t curPkdFlow = dofBufInt[(flowIdx - curDofBlkOffset) + (l & 0x1)*numRowInDofBlk*imgWidth];
        //int32_t curPkdFlow = dofPlane[flowIdx];

#ifdef TIADALG_UPDATE_TARCK_DEBUG
        printf("curIdx = %d, age = %d \n",curIdx,age);
        printf("curPkdFlow = %d \n",curPkdFlow);
        if((flowIdx - curDofBlkOffset) < 0)
        {
          printf("flowIdx = %d and curDofBlkOffset = %d \n",flowIdx,curDofBlkOffset);
        }
        assert((flowIdx - curDofBlkOffset) >= 0);
#endif

        /* |upper 16 bit horizontal flow|12 bit vertical flow| lowest 4 bit confidence|*/
        int32_t hFlow = (int32_t)((int32_t)((int32_t)curPkdFlow + 0) >> 16);
        int32_t vFlow = (int32_t)((((int32_t)((int32_t)curPkdFlow + 0) << 16) + 0)>> 20);
        int32_t conf  = (uint32_t)(curPkdFlow << 28) >> 28;

        int32_t xNew     = trackL->x[curIdx] + hFlow; // addition in Q4 format
        int32_t yNew     = trackL->y[curIdx] + vFlow; // addition in Q4 format
        int32_t xNewInt  = xNew >> 4;
        int32_t yNewInt  = yNew >> 4;

#ifdef TIADALG_UPDATE_TARCK_DEBUG
        printf("value of hFlow = %d, vFlow = %d, conf=%d \n",hFlow,vFlow,conf);
        printf("value of xNew = %d, yNew = %d, xNewInt=%d, yNewInt=%d \n",xNew,yNew,xNewInt,yNewInt);
#endif
#if 1
        /*If the new location is still in the picture boundary and confidence is higher than threshold*/
        if((xNewInt >= 0) && (xNewInt <= (imgWidth - 1)) && (yNewInt >= 0) && (yNewInt <= (imgHeight - 1))
           && ((conf * TIADALG_DOF_CONFIDENCE_TO_UINT8_SCALING) > flowConfThr))
#endif
        {
          /*need to find the static frame, and stop updating the tracks. Also wrap around of tracks needs to be taken care*/
          curIdx++;
          age++;

          if(curIdx >= MAX_NUM_FRAMES_TO_TRACK){
            curIdx = 0;
          }

          if(age >= MAX_NUM_FRAMES_TO_TRACK){
            age = MAX_NUM_FRAMES_TO_TRACK;
          }

          trackL->age       = age;
          trackL->idx       = curIdx;
          trackL->x[curIdx] = (uint16_t)xNew;
          trackL->y[curIdx] = (uint16_t)yNew;
#ifdef TIADALG_UPDATE_TARCK_DEBUG
          printf("curIdx = %d, xNew = %d, yNew= %d \n",curIdx, trackL->x[curIdx],trackL->y[curIdx]);
#endif
          int32_t nearestKpx  = ((int32_t)((xNewInt + (kpStep >> 1))*oneByKpStep))*kpStep;
          int32_t nearestKpy  = ((int32_t)((yNewInt + (kpStep >> 1))*oneByKpStep))*kpStep;

          int32_t keyPointBitMapLoc = ((int32_t)(nearestKpy*oneByKpStep))*numKpInRow + (int32_t)(nearestKpx*oneByKpStep); // location of kp in the domain of w/step, h/step
          int32_t byteLoc  = keyPointBitMapLoc>>3;
          uint8_t kpByte   = keyPointBitMap[byteLoc];
          uint8_t maskByte = (~(0x1 << (keyPointBitMapLoc & 0x7)));

          kpByte = kpByte & maskByte;

          if((__abs(nearestKpx - xNewInt) < 3) && (__abs(nearestKpy - yNewInt) < 3)){
            keyPointBitMap[byteLoc] = kpByte;
          }

          m++;
        }
        else
        {
          trackL->age = 255;
          trackL->idx = 255;
          n++;
        }
      }

#ifdef TIADALG_TRACK_PROFILE
  t1 = __TSC;
  acc3 += (t1-t0);
#endif

      if( l < (numDofBlocks-1))
      {
        DmaUtilsAutoInc3d_wait(dmaUtilsContext, TIADALG_DMA_CHANNEL_PING_PONG);
      }
    }

#ifdef TIADALG_TRACK_DEBUG
    printf("\n");
    printf("Previous Total Tracks (age >=1)     :: %05d \n", (m+n));
    printf("Current Total Tracks                :: %05d \n",m);
    printf("Current Total Tracks dropped        :: %05d \n",n);
#endif
    // get the location and count of age zero locations, basically free tracks
    trackL = track;
    int32_t numFreeTrack = 0;

#ifdef TIADALG_TRACK_PROFILE
  t0 = __TSC;
#endif

    for(i = 0; i < maxNumTracks; i++){

      int32_t idx = trackL->idx;

      if(idx == 255)
      {
        curInTrackIdx[numFreeTrack] = (uint16_t)i;
        numFreeTrack++;
      }

      trackL++;
    }

#if 0
    // potential oprtimization. May reduce 400k cycles. If thekey point masking is done in seperate loop like below
    // right now there is mismatch if it is seperate like below instead of merged with main loop
    // Atleast below loop can be software pipelined without SIMD
    trackL = track;
    for(i = 0; i < maxNumTracks; i++){

      int32_t idx = trackL->idx;

      if(idx != 0xFF){
        // Get new flow and update the tracks
        // Check the validity of new locations. This may create holes
        // update the key point binary map
        int32_t xNewInt = (trackL->x[idx]>>4);
        int32_t yNewInt = (trackL->y[idx]>>4);
        int32_t nearestKpx  = ((int32_t)((xNewInt + (kpStep >> 1))*oneByKpStep))*kpStep;
        int32_t nearestKpy  = ((int32_t)((yNewInt + (kpStep >> 1))*oneByKpStep))*kpStep;

        int32_t keyPointBitMapLoc = ((int32_t)(nearestKpy*oneByKpStep))*numKpInRow + (int32_t)(nearestKpx*oneByKpStep); // location of kp in the domain of w/step, h/step
        int32_t byteLoc  = keyPointBitMapLoc>>3;
        uint8_t kpByte   = keyPointBitMap[byteLoc];
        uint8_t maskByte = (~(0x1 << (keyPointBitMapLoc & 0x7)));

        kpByte = kpByte & maskByte;

        if((__abs(nearestKpx - xNewInt) < 3) && (__abs(nearestKpy - yNewInt) < 3)){
          keyPointBitMap[byteLoc] = kpByte;
        }
      }

      trackL++;
    }
#endif

#ifdef TIADALG_TRACK_PROFILE
  t1 = __TSC;
  acc4 += (t1-t0);
#endif
#ifdef TIADALG_TRACK_PROFILE
  t0 = __TSC;
#endif
    // Count the number of valid key points
    // get count of active key points
    int32_t validNumKp = 0;//numDetKp - droppedKpCnt;

    for(i = 0, k=0; i < imgHeightByKpStep; i++, k++){ // kpStepFact will skip horizontal lines
      for(j = 0; j <  kpBytesInRow; j++){
        uint8_t curKpMap = keyPointBitMap[k*kpBytesInRow + j];
        for(l = 0; l < 8; l++){
          if((((curKpMap >> l) & 0x1) == 0x1))
          {
            validNumKp++;
          }
        }
      }
    }

#ifdef TIADALG_TRACK_PROFILE
  t1 = __TSC;
  acc5+= (t1-t0);
#endif

    float skipCnt = ((float)(validNumKp > numFreeTrack ? (validNumKp - numFreeTrack) : 0))/validNumKp; // fraction of key points to be dropped

#ifdef TIADALG_TRACK_DEBUG
    printf("***********************************\n");
    printf("Total empty Tracks         :: %05d \n",numFreeTrack);
    printf("Total non empty Tracks     :: %05d \n",maxNumTracks - numFreeTrack);
    printf("Total valid kps            :: %05d \n",validNumKp);
    printf("skip count                 :: %f \n", skipCnt);
#endif
#ifdef TIADALG_TRACK_PROFILE
  t0 = __TSC;
#endif
    m = 0;
    n = 0;
    for(i = 0, k=0; i < imgHeightByKpStep; i++, k++){ // kpStepFact will skip horizontal lines
      for(j = 0; j <  kpBytesInRow; j++){
        uint8_t curKpMap = keyPointBitMap[k*kpBytesInRow + j];
        for(l = 0; l < 8; l++){
          if((((curKpMap >> l) & 0x1) == 0x1))
          {
            if(((int32_t)(n*skipCnt)) == ((int32_t)((n+1)*skipCnt))) // similar to if((n%skipCntFreq) !=0 )
            {
              track[curInTrackIdx[m]].age  = 1;
              track[curInTrackIdx[m]].idx  = 0;
              track[curInTrackIdx[m]].x[0] = (uint16_t)(((j*8 + l)*kpStep) << 4);
              track[curInTrackIdx[m]].y[0] = (uint16_t)((i*kpStep) << 4);
              m++;
              if(m > numFreeTrack){
                break;
              }
            }
            n++;
          }
        }
      }
    }

#ifdef TIADALG_TRACK_PROFILE
  t1 = __TSC;
  acc6 += (t1-t0);
  t3 = __TSC;

  printf("Total cycle consumed in track segragation is      = %lld \n", acc0);
  printf("Total cycle consumed in key points creation is    = %lld \n", acc1);
  printf("cycle consumed in collecting tracks are           = %lld \n", acc2);
  printf("Cycle consumed in current block tracks are        = %lld \n", acc3);
  printf("total cycle consumed in finding free tracks are   = %lld \n", acc4);
  printf("total cycle consumed in finding validkp are       = %lld \n", acc5);
  printf("Total cycle consumed in new track insertion       = %lld \n", acc6);
  printf("Total cycle consumed is :: %lld \n",(t3-t2));
#endif


#ifdef TIADALG_TRACK_DEBUG
  printf("value of m =%d, n=%d \n",m,n);
#endif
  DmaUtilsAutoInc3d_deconfigure(dmaUtilsContext,
                TIADALG_DMA_CHANNEL_PING_PONG,
                inTrMem,
                0x1);
  }

#ifdef ENABLE_PROFILE
  TI_DSP_ProfileModule(prfInfo,
                        TI_DSP_PROFILE_MODULE2,
                        TI_DSP_PROFILE_END,
                        TI_DSP_PROFILE_CUMULATIVE);
#endif

#ifdef TIADALG_TRACK_DUMP
  SFM_TI_dumpTrackData(track,maxNumTracks);
#endif

}

#ifdef TIADALG_TRACK_DUMP

void SFM_TI_dumpTrackData(SFM_TI_trackInfo *trackL,int32_t maxNumTracks)
{
    int32_t ageCnt[MAX_NUM_FRAMES_TO_TRACK];

    FILE* fp;
    static int32_t curFrameNum = 0;
    int32_t lastIndx;
    int32_t i,j,k;
    int32_t totNumTracks = 0;

    if(curFrameNum == 0){
      fp = fopen("track.txt","w");
    }else{
      fp = fopen("track.txt","a");
    }

    if(fp == NULL)
      printf("txt file to dump track could not be opened \n");

    for(i = 0; i < MAX_NUM_FRAMES_TO_TRACK; i++){
      ageCnt[i] = 0;
    }

    fprintf(fp,"Frame # %010d",curFrameNum++);

    for(i = 0; i < maxNumTracks; i++){
      ageCnt[trackL->idx]++;
      lastIndx     = ((trackL->idx) - (trackL->age) + 1 + MAX_NUM_FRAMES_TO_TRACK) % MAX_NUM_FRAMES_TO_TRACK;

      if(((trackL->age) > 0)&&((trackL->age) <= MAX_NUM_FRAMES_TO_TRACK)){
        fprintf(fp,"\n %05d :: %02d :: ", i, (trackL->age));

        for(j = 0; j < (trackL->age); j++)
        {
          k = (lastIndx + j + MAX_NUM_FRAMES_TO_TRACK)%MAX_NUM_FRAMES_TO_TRACK;
          fprintf(fp, " (%10.2f , %10.2f), ", ((float)trackL->x[k])/16.0f, ((float)trackL->y[k])/16.0f);
        }
      }
      trackL++;
    }
    fprintf(fp,"\n");

    fclose(fp);

    printf("Final track stats \n");
    for(i = 0; i < MAX_NUM_FRAMES_TO_TRACK; i++){
      printf("ageCnt[%d] = %04d \n", i, ageCnt[i]);
      totNumTracks += ageCnt[i];
    }

    printf("Total number of tracks are %d \n", totNumTracks);
}
#endif

