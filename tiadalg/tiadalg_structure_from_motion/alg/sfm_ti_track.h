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
*  @file         sfm_ti_track.h
*  @author       Deepak Poddar
*  @brief        This file defines the functions related track extraction
*                from input buffer
*  @version 0.1 (Oct 2014) : Base version.
***************************************************************************
*/

#ifndef TI_SFM_TRACK_H
#define TI_SFM_TRACK_H

//#define TIADALG_TRACK_DEBUG
//#define TIADALG_UPDATE_TARCK_DEBUG
//#define TIADALG_TRACK_KP_INSERTION_DEBUG
//#define TIADALG_TRACK_DUMP
//#define TIADALG_TRACK_PROFILE

void SFM_TI_getCurTrack(SFM_TI_trackInfo* baseTrackInfo,
                        float*            camInvIntPrm,
                        uint16_t*         trackId,
                        float*            curTrack,
                        float*            curTrackRaw,
                        uint8_t*          curFeatInTrack);

void SFM_TI_getCurTrack_ci(SFM_TI_trackInfo* restrict baseTrackInfo,
                        float*            restrict camInvIntPrm,
                        uint16_t*         restrict trackId,
                        float*            restrict curTrack,
                        float*            restrict curTrackRaw,
                        uint8_t*          restrict curFeatInTrack);

void SFM_TI_updateTrackRef(SFM_TI_trackInfo *track, int32_t* dofPlane, uint16_t* ageZeroTrackIdx, uint8_t* keyPointBitMap,
                        int32_t imgWidth, int32_t imgHeight, int32_t kpStep, int32_t maxNumTracks,
                        int32_t flowInvalidPadX, int32_t flowInvalidPadY, int32_t curFrameNum,
                        int32_t flowConfThr, TI_DSP_PrfInfo* prfInfo);

void SFM_TI_updateTrack(SFM_TI_trackInfo * restrict track, int32_t* restrict dofPlane, uint16_t* restrict curInTrackIdx,
                        uint16_t* restrict curOutTrackIdx, int32_t* restrict dofBufInt, uint8_t* restrict keyPointBitMap,
                        int32_t imgWidth, int32_t imgHeight, int32_t kpStep,
                        int32_t maxNumTracks, int32_t flowInvalidPadX, int32_t flowInvalidPadY,
                        int32_t curFrameNum,int32_t flowConfThr, int32_t keyPointBitMapSize,
                        int32_t dofBlockBufSize, int32_t numDofBlocks, int32_t numRowInDofBlk,
                        uint8_t* restrict inTrMem, void * dmaUtilsContext, TI_DSP_PrfInfo* restrict prfInfo);

#ifdef TIADALG_TRACK_DUMP
void SFM_TI_dumpTrackData(SFM_TI_trackInfo *trackL,int32_t maxNumTracks);
#endif

#endif
