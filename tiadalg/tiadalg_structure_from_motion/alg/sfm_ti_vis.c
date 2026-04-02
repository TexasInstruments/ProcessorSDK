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
*  @file         sfm_ti_vis.c
*  @author       Deepak Poddar
*  @brief        This file defines the functions related to visualization of SFM outout
*  @version 0.1 (Feb 2021) : Base version.
***************************************************************************
*/
#include "sfm_ti_vis.h"
#include "sfm_ti_device_utils.h"

void SFM_TI_visPtCldOutput(SFM_TI_Handle intAlgHandle,
                      SFM_TI_CreateParams* createParams,
                      sSfm_TI_ExtMem* sfmPrm,
                      uint8_t* inImg, /* chroma plane only*/
                      void * pointCld,
                      uint8_t* outImg)
{

#ifdef TIADALG_VIS_PROFILE
  long long t0, t1, t2, t3;
  long long acc0=0;
  long long acc1=0;
  long long acc2=0;
  long long acc3=0;
  long long acc4=0;
  long long acc5=0;
#endif
   int32_t lastBlkHeight;
   float oneByNumRowInVisBuf = 1.0f/sfmPrm->numRowInPtCldVisBuf;

   /*Visualization drawing*/
   if(createParams->enAlgPtCldVis != 0)
   {

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
      t2 = __TSC;
#endif

      if(sfmPrm->num3DPoints > 0)
      {
        SFM_TI_segregate3DPoints((SFM_TI_output*)pointCld, createParams->imgWidth, createParams->imgHeight,
                                 0.0f, 192.0f, sfmPrm->numRowInPtCldVisBuf,
                                 sfmPrm->colorIndx, sfmPrm->blkIndx,
                                 sfmPrm->num3DPoints);
      }

#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc0 += (t1-t0);
#endif

      int32_t numImgBlocks = (createParams->imgHeight >> 1)*oneByNumRowInVisBuf; // last block is processed seperately
      int32_t pingPongOffset; // luma ping pong buffer offset

      /*one line extra buffer between ping pong boundry as we write one extra line of chroma*/
      pingPongOffset = createParams->imgWidth*sfmPrm->numRowInPtCldVisBuf + createParams->imgWidth;

      /*output DMA configuration. Input dma is not needed for producing just the chroma plane*/
      TIADALG_updatePingPong3DTr((uint8_t*)sfmPrm->tempImgPtCldL2, outImg,
                                intAlgHandle->pingPongCbCrTr,
                                createParams->imgWidth, sfmPrm->numRowInPtCldVisBuf*numImgBlocks, createParams->imgWidth,
                                createParams->imgWidth, sfmPrm->numRowInPtCldVisBuf, createParams->imgWidth,
                                pingPongOffset, 0x1, sizeof(uint8_t), // 2 for ping pong
                                TIADALG_DMA_CHANNEL_CBCR_PING_PONG, intAlgHandle->dmaUtilsContext);

      DmaUtilsAutoInc3d_configure(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG, intAlgHandle->pingPongCbCrTr, 1U);


      uint8_t* outImgCbCrL2; // chroma plane or RGB plane

      int32_t l,i;
      int32_t curBlkNumPoints;

      for(l = 0; l < numImgBlocks; l++)
      {
        outImgCbCrL2       = sfmPrm->tempImgPtCldL2 + ((l&0x1)*pingPongOffset);

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif

        // memset the chroma plane to 128;
        for(i = 0; i < pingPongOffset; i++)
        {
          outImgCbCrL2[i] = 0x80;
        }
        //visualization drawing starts here
        if(sfmPrm->num3DPoints > 0)
        {
          curBlkNumPoints = 0;
          for(i = 0; i < sfmPrm->num3DPoints; i++)
          {
            if(sfmPrm->blkIndx[i] == l)
            {
              sfmPrm->curBlkIndx[curBlkNumPoints] = i;
              curBlkNumPoints++;
            }
          }
          SFM_TI_drawSparse3DPoints(outImgCbCrL2, (SFM_TI_output*)pointCld,createParams->imgWidth,
                                    sfmPrm->colorIndx, sfmPrm->curBlkIndx, l*sfmPrm->numRowInPtCldVisBuf*createParams->imgWidth,
                                    curBlkNumPoints);
        }

#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc1 += (t1-t0);
#endif

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif


      if(l!=0) // wait for previous out transfer triggered before trigerring new one
      {
        DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);
      }

      DmaUtilsAutoInc3d_trigger(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);

#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc2 += (t1-t0);
#endif

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif
#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc3 += (t1-t0);
#endif

      }

      DmaUtilsAutoInc3d_deconfigure(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG, intAlgHandle->pingPongCbCrTr, 1U);

      lastBlkHeight  = (createParams->imgHeight >> 1) - l*sfmPrm->numRowInPtCldVisBuf;

      if(lastBlkHeight > 0)
      {
      outImgCbCrL2       = sfmPrm->tempImgPtCldL2 + ((l&0x1)*pingPongOffset);

      uint8_t* outPtrLastBlk = outImg + l*sfmPrm->numRowInPtCldVisBuf*createParams->imgWidth;
#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif
      for(i = 0; i < pingPongOffset; i++)
      {
        outImgCbCrL2[i] = 0x80;
      }

      if(sfmPrm->num3DPoints > 0)
      {

          curBlkNumPoints = 0;
          for(i = 0; i < sfmPrm->num3DPoints; i++)
          {
            if(sfmPrm->blkIndx[i] == l)
            {
              sfmPrm->curBlkIndx[curBlkNumPoints] = i;
              curBlkNumPoints++;
            }
          }
          SFM_TI_drawSparse3DPoints(outImgCbCrL2, (SFM_TI_output*)pointCld,
                                    createParams->imgWidth,sfmPrm->colorIndx,
                                    sfmPrm->curBlkIndx, l*sfmPrm->numRowInPtCldVisBuf*createParams->imgWidth,
                                    curBlkNumPoints);
      }
#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc1+= (t1-t0);
#endif

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif
      DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);

#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc4 += (t1-t0);
#endif

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif
      /*last chunk may have partial block, hence using memcopy instead of ping-pong auto increment trigger*/
      TIADALG_memcpy2D(outPtrLastBlk,
                        outImgCbCrL2,
                        createParams->imgWidth,
                        lastBlkHeight,
                        createParams->imgWidth,
                        createParams->imgWidth,
                        TIADALG_DMA_CHANNEL_MEMCPY,
                        intAlgHandle->dmaUtilsContext,
                        intAlgHandle->memcpyTr,
                        0x1);
#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc5 += (t1-t0);
      t3 = __TSC;
#endif

      }
   }

#ifdef TIADALG_VIS_PROFILE
   printf("Total cycle consumed in 3D points ( %d bytes) cloud transfer = %lld \n", sizeof(SFM_TI_output)*sfmPrm->num3DPoints, acc0);
   printf(" Total cycle in real compute is %lld \n", acc1);
   printf(" Total wait cycle is %lld \n", acc2);
   printf(" Total trigger cycle is %lld \n", acc3);
   printf(" Last wait cycles %lld \n", acc4);
   printf(" Last transfer of %d bytes takes total cycle %lld \n", lastBlkHeight*createParams->imgWidth, acc5);
   printf(" Total visualization Cycle is :: %lld \n", (t3-t2));
#endif

}

int32_t SFM_TI_segregate3DPoints(SFM_TI_output* restrict ptCld, int32_t imgWidth,int32_t imgHeight,
                               float dMin, float dMax, int32_t numRowInBlk,
                               uint8_t* restrict colorIndx, uint16_t* restrict blkIndx,
                               int32_t outNumPoints)
{
  int32_t i;
  float maxZ4Visulization = 100.0;
  int32_t  colorMax = 64;
  int32_t  colorMaxToUse = colorMax * 3 / 4;
  float colorMaxToUseFact = colorMaxToUse / (maxZ4Visulization - dMin);
  int32_t colorIndex;
  int32_t isValidPt;
  int32_t validCnt = 0;
  float oneByNumRowInBlk = 1.0f/numRowInBlk;

  float curImgx, curImgy;
  int32_t  int_x, int_y;

  float curX, curY, curZ, curDist;

  for(i = 0; i < outNumPoints; i++)
  {
    curX = ptCld[i].point3dX;
    curY = ptCld[i].point3dY;
    curZ = ptCld[i].point3dZ;

    curImgx = ptCld[i].point2dX;
    curImgy = ptCld[i].point2dY;

    int_x = curImgx;
    int_y = curImgy;

    int_x &= 0x7FFFFFFE;
    int_y &= 0x7FFFFFFE;

    curDist = curX*curX + curY*curY + curZ*curZ;

    curDist = (curDist* __recip_sqrt(curDist));

    colorIndex = (curDist - dMin) * colorMaxToUseFact;

    if(colorIndex < 0)
    {
      colorIndex = 0;
    }

    if(colorIndex >= colorMaxToUse)
    {
      colorIndex = colorMaxToUse - 1;
    }

    isValidPt = 1;

    /*as 4x4 chroma point is colored*/
    if(int_x < 4)
    {
      isValidPt = 0;
    }

    if(int_y < 4)
    {
      isValidPt = 0;
    }

    if(int_x > (imgWidth - 4))
    {
      isValidPt = 0;
    }

    if(int_y > (imgHeight - 4))
    {
      isValidPt = 0;
    }

    if(curDist <= dMin)
    {
      isValidPt = 0;
    }

    if(curDist >= dMax)
    {
      isValidPt = 0;
    }

    if(isValidPt)
    {
      blkIndx[i]   = (int_y >> 1)*oneByNumRowInBlk;
      colorIndx[i] = colorIndex;
      validCnt++;
    }else{
      blkIndx[i]   = 0xFF;
      colorIndx[i] = 0x0;
    }

  }
  return validCnt;
}

void SFM_TI_drawSparse3DPoints(uint8_t* restrict imgPtr2, SFM_TI_output* restrict ptCld, int32_t imgWidth,
                               uint8_t* restrict colorIndx, uint16_t* restrict curBlkIndx,int32_t blockOffset,
                               int32_t curBlkNumPoints)
{
  int32_t i;
  float curImgx, curImgy;
  int32_t  int_x, int_y;

  float HSV_COLOR_MAP[64][3] =
  {
    1.0000,         0,         0, 1.0000,    0.0938,         0, 1.0000,    0.1875,         0,
    1.0000,    0.2813,         0, 1.0000,    0.3750,         0, 1.0000,    0.4688,         0,
    1.0000,    0.5625,         0, 1.0000,    0.6563,         0, 1.0000,    0.7500,         0,
    1.0000,    0.8438,         0, 1.0000,    0.9375,         0, 0.9688,    1.0000,         0,
    0.8750,    1.0000,         0, 0.7813,    1.0000,         0, 0.6875,    1.0000,         0,
    0.5938,    1.0000,         0, 0.5000,    1.0000,         0, 0.4063,    1.0000,         0,
    0.3125,    1.0000,         0, 0.2188,    1.0000,         0, 0.1250,    1.0000,         0,
    0.0313,    1.0000,         0, 0,    1.0000,    0.0625, 0,    1.0000,    0.1563,
    0,    1.0000,    0.2500, 0,    1.0000,    0.3438, 0,    1.0000,    0.4375,
    0,    1.0000,    0.5313, 0,    1.0000,    0.6250, 0,    1.0000,    0.7188,
    0,    1.0000,    0.8125, 0,    1.0000,    0.9063, 0,    1.0000,    1.0000,
    0,    0.9063,    1.0000, 0,    0.8125,    1.0000, 0,    0.7188,    1.0000,
    0,    0.6250,    1.0000, 0,    0.5313,    1.0000, 0,    0.4375,    1.0000,
    0,    0.3438,    1.0000, 0,    0.2500,    1.0000, 0,    0.1563,    1.0000,
    0,    0.0625,    1.0000, 0.0313,         0,    1.0000, 0.1250,         0,    1.0000,
    0.2188,         0,    1.0000, 0.3125,         0,    1.0000, 0.4063,         0,    1.0000,
    0.5000,         0,    1.0000, 0.5938,         0,    1.0000, 0.6875,         0,    1.0000,
    0.7813,         0,    1.0000, 0.8750,         0,    1.0000, 0.9688,         0,    1.0000,
    1.0000,         0,    0.9375, 1.0000,         0,    0.8438, 1.0000,         0,    0.7500,
    1.0000,         0,    0.6563, 1.0000,         0,    0.5625, 1.0000,         0,    0.4688,
    1.0000,         0,    0.3750, 1.0000,         0,    0.2813, 1.0000,         0,    0.1875,
    1.0000,         0,    0.0938,
  };

  for(i = 0; i < curBlkNumPoints; i++)
  {

    int32_t curIdx = curBlkIndx[i];
    int32_t colorIndex = colorIndx[curIdx];

    uint8_t r = HSV_COLOR_MAP[colorIndex][0]*255;
    uint8_t g = HSV_COLOR_MAP[colorIndex][1]*255;
    uint8_t b = HSV_COLOR_MAP[colorIndex][2]*255;

    uint8_t cb, cr;

    //y  = (( (66 * r)  + (129 * g) + (25 * b)  + 128) >> 8) + 16;
    cb = ((-(38 * r)  - (74 * g)  + (112 * b) + 128) >> 8) + 128;
    cr = (( (112 * r) - (94 * g)  - (18 * b)  + 128) >> 8) + 128;

    curImgx = ptCld[curIdx].point2dX;
    curImgy = ptCld[curIdx].point2dY;

    int_x = curImgx;
    int_y = curImgy;

    int_x &= 0x7FFFFFFE;
    int_y &= 0x7FFFFFFE;

    int_y = int_y >> 1;// as only chroma plane is in drawing
    int_x = int_x >> 1;

    int32_t index  = int_y * imgWidth; // assuming Nv12 format wx(h/2)
    index -= blockOffset;

    /*if(((int_y*imgWidth) >= blockOffset) &&
      ((int_y*imgWidth) < (blockOffset + numRowInBlk*imgWidth))
    )*/
    imgPtr2[index + 2*int_x + 0] = cb;
    imgPtr2[index + 2*int_x + 1] = cr;

    /*for last row it may write into outside ping/pong boundary*/
    imgPtr2[index + imgWidth + 2*int_x + 0] = cb;
    imgPtr2[index + imgWidth + 2*int_x + 1] = cr;

    imgPtr2[index + 2*int_x + 2] = cb;
    imgPtr2[index + 2*int_x + 3] = cr;

    /*for last row it may write into outside ping/pong boundary*/
    imgPtr2[index + imgWidth + 2*int_x + 2] = cb;
    imgPtr2[index + imgWidth + 2*int_x + 3] = cr;
  }
}

void SFM_TI_visOgOutput(SFM_TI_Handle intAlgHandle,
                        SFM_TI_CreateParams* createParams,
                        sSfm_TI_ExtMem* sfmPrm,
                        uint8_t* outImg) /*Output image location in DDR*/
{

#ifdef TIADALG_VIS_PROFILE
  long long t0, t1, t2, t3;
  long long acc0=0;
  long long acc1=0;
  long long acc2=0;
  long long acc3=0;
  long long acc4=0;
  long long acc5=0;
#endif

   /*Visualization drawing*/
   if((createParams->enAlgOcpGrdVis != 0))
   {

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
      t2 = __TSC;
#endif

#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc0 += (t1-t0);
#endif

      float oneByNumRowInVisBuf = 1.0f/sfmPrm->numRowInOgVisBuf;

      /*Assuming square display and double the size of occupancy grid visualization resolution, as we are working on
        YUV nv12 format, hence to represent each pixel distinctly axb resolution of occupancy grid
        needs to be mapped on 2ax2b visualization drawing.
      */
      // for 500x500, visualization chroma buffer is 1024x512 (UV nv12)
      int32_t ogVizImageSizeInPixelsW = __mmin(2*createParams->ocpGrdSizeInPixels,createParams->imgWidth); // 1000 // display width
      int32_t ogVizImageSizeInPixelsH = __mmin(2*createParams->ocpGrdSizeInPixels,createParams->imgHeight); // 512 //display height
      int32_t ogVizValidHeight        = ogVizImageSizeInPixelsH * 0.5f; // 256. valid disaplay height

      /*startYLoc and startOff are in luma domain*/
      int32_t starYLocInOcg; // only 50% of height is visualized from bottom to just fit in L2
      int32_t starYLocInChromaBuf;
      int32_t numImgBlocks;
      int32_t numRowInOgVisBuf;
      int32_t blockOffsetOg;
      int32_t blockOffsetChromaBuf;

      /*one line in occupancy buffer produces one line of chroma, which results in 2 lines in display*/
      starYLocInOcg = createParams->ocpGrdSizeInPixels - (ogVizValidHeight >> 1); // 500 - 128 = 372
      starYLocInChromaBuf = (ogVizImageSizeInPixelsH >> 1) - (ogVizValidHeight >> 1); // 256 - 128 = 128

      if((ogVizImageSizeInPixelsH >>1) <= sfmPrm->numRowInOgVisBuf*2)
      {
        numImgBlocks = 1;// no need of ping pong
        numRowInOgVisBuf = ogVizImageSizeInPixelsH>>1; //256
      }
      else
      {
        /*this flow is not varified*/
        numImgBlocks = ogVizImageSizeInPixelsH * oneByNumRowInVisBuf; // last block is processed seperately
        numRowInOgVisBuf = sfmPrm->numRowInOgVisBuf;
      }

      blockOffsetChromaBuf = ogVizImageSizeInPixelsW*numRowInOgVisBuf;
      blockOffsetOg        = createParams->ocpGrdSizeInPixels*numRowInOgVisBuf;

      /*output DMA configuration. Input dma is not needed for producing just the chroma plane*/
      TIADALG_updatePingPong3DTr((uint8_t*)sfmPrm->tempImgOgL2, outImg,
                                intAlgHandle->pingPongCbCrTr,
                                ogVizImageSizeInPixelsW, numRowInOgVisBuf*numImgBlocks, createParams->imgWidth,
                                ogVizImageSizeInPixelsW, numRowInOgVisBuf, ogVizImageSizeInPixelsW,
                                blockOffsetChromaBuf, 0x1, sizeof(uint8_t), // 2 for ping pong
                                TIADALG_DMA_CHANNEL_CBCR_PING_PONG, intAlgHandle->dmaUtilsContext);

      DmaUtilsAutoInc3d_configure(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG, intAlgHandle->pingPongCbCrTr, 1U);


      uint8_t* outImgCbCrL2; // chroma plane for occupancy grid
      int8_t* inImgCbCrL3; // persistant occupancy grid

      int32_t l,i;
      int32_t cb, cr; // luma is fixed to 82
      /*x and y is in final chroma buffer domain*/
      int32_t x, y;

      for(l = 0; l < numImgBlocks; l++)
      {
        x = 0; y = starYLocInChromaBuf;

        /*outImgCbCrL2 buffer is in CbCr domain*/
        outImgCbCrL2       = sfmPrm->tempImgOgL2 + ((l&0x1)*blockOffsetChromaBuf);
        uchar64 constCbCr   = (uchar64)128;

        for(i = 0; i < starYLocInChromaBuf*ogVizImageSizeInPixelsW; i+= 64) // will write some of the bytes outside the expected boundary
        {
         *stov_ptr(uchar64,outImgCbCrL2) = constCbCr;
         outImgCbCrL2 += 64;
        }

        outImgCbCrL2       = sfmPrm->tempImgOgL2 + ((l&0x1)*blockOffsetChromaBuf);
        //soutImgCbCrL2      += (starYLocInChromaBuf*ogVizImageSizeInPixelsW);

        /* As chroma block is half of the size of luma block. Ann l is indexed for
           chroma block. occupancy grid buffer in L3 is for for full image resolution.
        */

        /* ogVizImageSizeInPixelsW is the pitch of chroma buffer
           createParams->ocpGrdSizeInPixels is the pitch for occupancy grid buffer
        */
        /*inImgCbCrL3 buffer is the occupancy buffer*/
        inImgCbCrL3        = (int8_t*)sfmPrm->ogBufInt + l*blockOffsetOg;
        //inImgCbCrL3       += (starYLocInOcg*createParams->ocpGrdSizeInPixels);

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif
        /*
          Bring the data from L3 (using CPU read) and prepeare the colored image in L2.
          For L2 size of of 448KB, image size of 667x667 can be prepared in one go.
          Since it is ping pong so 333x667 buffer can be prepared in one go.
        */
        int32_t cmpRes1;
        int32_t cmpRes2;
        int8_t  curImg;

        /* loop parses input occupancy grid data, and places in output buffer by expanding 2x in x dir*/
        for(i = starYLocInOcg*createParams->ocpGrdSizeInPixels; i < (createParams->ocpGrdSizeInPixels*createParams->ocpGrdSizeInPixels); i++)
        {
          /*for better optimization, there can be another loop inside this loop which collects the
            the valid locations from the list "inImgCbCrL3" and places the valid location in L2.
            another loop which just executes for the valid lcoations.
            Buffer required to hold the valid locations may be large hence it can be furhter broken into multiple blocks.
          */
          cb = 128;
          cr = 128;
          curImg = inImgCbCrL3[i];

          cmpRes1 = curImg > 0; // red
          cmpRes2 = curImg < 0; // green

          if(cmpRes1) //red
          {
            cb = 80;
          }

          if(cmpRes1) //red
          {
            cr = 255;
          }

          if(cmpRes2) // green
          {
            cb = 0;
          }

          if(cmpRes2) // green
          {
            cr = 0;
          }

          outImgCbCrL2[y*ogVizImageSizeInPixelsW + 2*x] = cb;
          outImgCbCrL2[y*ogVizImageSizeInPixelsW + 2*x + 1] = cr;

          x++;

          if(x == createParams->ocpGrdSizeInPixels)
          {
            x = 0;
            y++;
          }
        }

#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc1 += (t1-t0);
#endif

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif


      if(l!=0) // wait for previous out transfer triggered before trigerring new one
      {
        DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);
      }

      DmaUtilsAutoInc3d_trigger(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);

#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc2 += (t1-t0);
#endif

#ifdef TIADALG_VIS_PROFILE
      t0 = __TSC;
#endif
#ifdef TIADALG_VIS_PROFILE
      t1 = __TSC;
      acc3 += (t1-t0);
#endif

      }

      DmaUtilsAutoInc3d_wait(intAlgHandle->dmaUtilsContext, TIADALG_DMA_CHANNEL_CBCR_PING_PONG);

#ifdef TIADALG_VIS_PROFILE
   printf("Total cycle consumed in 3D points ( %d bytes) cloud transfer = %lld \n", sizeof(SFM_TI_output)*sfmPrm->num3DPoints, acc0);
   printf(" Total cycle in real compute is %lld \n", acc1);
   printf(" Total wait cycle is %lld \n", acc2);
   printf(" Total trigger cycle is %lld \n", acc3);
   printf(" Last wait cycles %lld \n", acc4);
   printf(" Last transfer of %d bytes takes total cycle %lld \n", lastBlkHeight*createParams->imgWidth, acc5);
   printf(" Total visualization Cycle is :: %lld \n", (t3-t2));
#endif

 }
}
void SFM_TI_visEgoVehicle(SFM_TI_Handle intAlgHandle,
                        SFM_TI_CreateParams* createParams,
                        sSfm_TI_ExtMem* sfmPrm,
                        int32_t isLocal,
                        uint8_t* outImg) /*Output image location in DDR*/
{
    float * cur_inv_cam_pose = &sfmPrm->camInvExtPrmL1[(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 1)][0];
    float * first_cam_pose   = &sfmPrm->firstCamExtPrmL1[0];
    int32_t og_row_idx, og_col_idx;
    float grid_scale = 10.0f;
    int32_t isValid = 1;

    if(isLocal)
    {
      og_row_idx = createParams->ocpGrdSizeInPixels - 4;
      og_col_idx = createParams->ocpGrdSizeInPixels;
    }
    else
    {
      float w_x = first_cam_pose[0*4+0]*cur_inv_cam_pose[0*4+3] +
                  first_cam_pose[0*4+1]*cur_inv_cam_pose[1*4+3] +
                  first_cam_pose[0*4+2]*cur_inv_cam_pose[2*4+3] +
                  first_cam_pose[0*4+3]*cur_inv_cam_pose[3*4+3];

      float w_z = first_cam_pose[2*4+0]*cur_inv_cam_pose[0*4+3] +
                  first_cam_pose[2*4+1]*cur_inv_cam_pose[1*4+3] +
                  first_cam_pose[2*4+2]*cur_inv_cam_pose[2*4+3] +
                  first_cam_pose[2*4+3]*cur_inv_cam_pose[3*4+3];

      og_row_idx = (int32_t)(round(createParams->ocpGrdSizeInPixels- (w_z*grid_scale)));
      og_col_idx = (int32_t)(round((w_x*grid_scale) + (createParams->ocpGrdSizeInPixels>>1)));

      if(og_row_idx < 0)
      {
        isValid = 0;
      }

      if(og_col_idx < 0)
      {
        isValid = 0;
      }

      if(og_row_idx > (createParams->ocpGrdSizeInPixels - 1))
      {
        isValid = 0;
      }

      if(og_col_idx > (createParams->ocpGrdSizeInPixels - 1))
      {
        isValid = 0;
      }
    }

    og_row_idx = og_row_idx >> 1; // as it is chroma plane
    og_col_idx = og_col_idx >> 1; // as it is chroma plane

    int32_t thickness   = 2; // thickness in chroma plane
    int32_t line_length = 8; // length in chroma plane
    int32_t i,j;

    if(isValid > 0)
    {
      // draw horizontal line
      for(i = -(thickness>>1); i < (thickness>>1); i++)
      {
        for(j = -(line_length>>1); j < (line_length>>1); j++)
        {
          outImg[(og_row_idx + i)*createParams->imgWidth + og_col_idx*2 + 0 + j*2] = 255; // blue color
          outImg[(og_row_idx + i)*createParams->imgWidth + og_col_idx*2 + 1 + j*2] = 80;
        }
      }

      // draw vertical line
      for(i = -(line_length>>1); i < (line_length>>1); i++)
      {
        for(j = -(thickness>>1); j < (thickness>>1); j++)
        {
          outImg[(og_row_idx + i)*createParams->imgWidth + og_col_idx*2 + 0 + j*2] = 255; // blue color
          outImg[(og_row_idx + i)*createParams->imgWidth + og_col_idx*2 + 1 + j*2] = 80;
        }
      }
    }

}
