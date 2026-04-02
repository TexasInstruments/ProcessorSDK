/*
*
* Copyright (c) 2018 Texas Instruments Incorporated
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
#include <math.h>
#include <string.h>
#include <stdio.h>

#include <perception/algos/stereo/object_detection/stereo_object_detect.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))



void PTK_Alg_StereoAlgo_ObjectDetectSetParams(PTK_Alg_StereoAlgo_ObjectDetect_allParams* allParams,
                                              PTK_Alg_StereoAlgo_ObjectDetectObj   * cntxt)
{
    int32_t i;
    //int32_t numDisparity;
    //float   invF;
    //float   d0;

    int32_t numColsPerModel = allParams->camParams.width / NUM_GP_MODELS;

    /* initialize camera parameters */
    cntxt->allParams.camParams.width     = allParams->camParams.width;
    cntxt->allParams.camParams.height    = allParams->camParams.height;

    // for KITTI Sequences
    // To change for TI stereo camera
    cntxt->allParams.camParams.camHeight = allParams->camParams.camHeight;
    cntxt->allParams.camParams.baseline  = allParams->camParams.baseline;
    cntxt->allParams.camParams.dcx       = allParams->camParams.dcx;
    cntxt->allParams.camParams.dcy       = allParams->camParams.dcy;
    cntxt->allParams.camParams.f         = allParams->camParams.f;

    cntxt->allParams.camParams.K.M[0]    = allParams->camParams.K.M[0];
    cntxt->allParams.camParams.K.M[1]    = allParams->camParams.K.M[1];
    cntxt->allParams.camParams.K.M[2]    = allParams->camParams.K.M[2];
    cntxt->allParams.camParams.K.M[3]    = allParams->camParams.K.M[3];
    cntxt->allParams.camParams.K.M[4]    = allParams->camParams.K.M[4];
    cntxt->allParams.camParams.K.M[5]    = allParams->camParams.K.M[5];
    cntxt->allParams.camParams.K.M[6]    = allParams->camParams.K.M[6];
    cntxt->allParams.camParams.K.M[7]    = allParams->camParams.K.M[7];
    cntxt->allParams.camParams.K.M[8]    = allParams->camParams.K.M[8];

    //invF                       = 1.0f / cntxt->camParams.f;
    cntxt->allParams.camParams.invK.M[0] = allParams->camParams.invK.M[0];
    cntxt->allParams.camParams.invK.M[1] = allParams->camParams.invK.M[1];
    cntxt->allParams.camParams.invK.M[2] = allParams->camParams.invK.M[2];
    cntxt->allParams.camParams.invK.M[3] = allParams->camParams.invK.M[3];
    cntxt->allParams.camParams.invK.M[4] = allParams->camParams.invK.M[4];
    cntxt->allParams.camParams.invK.M[5] = allParams->camParams.invK.M[5];
    cntxt->allParams.camParams.invK.M[6] = allParams->camParams.invK.M[6];
    cntxt->allParams.camParams.invK.M[7] = allParams->camParams.invK.M[7];
    cntxt->allParams.camParams.invK.M[8] = allParams->camParams.invK.M[8];

    /* initialize road params */
    // for KITTI Sequences
    cntxt->allParams.roadParams.roadCenterBase[0] = allParams->roadParams.roadCenterBase[0];
    cntxt->allParams.roadParams.roadCenterBase[1] = allParams->roadParams.roadCenterBase[1];
    cntxt->allParams.roadParams.roadCenterTop[0]  = allParams->roadParams.roadCenterTop[0];
    cntxt->allParams.roadParams.roadCenterTop[1]  = allParams->roadParams.roadCenterTop[1];

    cntxt->allParams.roadParams.roadSizeBase      = allParams->roadParams.roadSizeBase;
    cntxt->allParams.roadParams.roadSizeTop       = allParams->roadParams.roadSizeTop;

    cntxt->allParams.roadParams.maxDepth          = allParams->roadParams.maxDepth;
    cntxt->allParams.roadParams.egoSize           = allParams->roadParams.egoSize;

    /* initialize disparity params */
    cntxt->allParams.dispParams.confTh            = allParams->dispParams.confTh;
    cntxt->allParams.dispParams.minDisparity      = allParams->dispParams.minDisparity;
    cntxt->allParams.dispParams.maxDisparity      = allParams->dispParams.maxDisparity;
    cntxt->allParams.dispParams.numDisparity      = allParams->dispParams.numDisparity;
    cntxt->allParams.dispParams.fracBits          = allParams->dispParams.fracBits;
    cntxt->allParams.dispParams.farZ              = allParams->dispParams.farZ;

    //numDisparity = cntxt->allParams.dispParams.numDisparity;

    /* create gpModelIdx */
    for (i = 0;  i < allParams->camParams.width; i++)
    {
    	cntxt->algoMemData.gpModelIdx[i] = floor(i / numColsPerModel);
    }

    /* compute obstacle probability per pixel params */
    cntxt->allParams.copppParams.dsFactor = allParams->copppParams.dsFactor;
    cntxt->allParams.copppParams.dsWidth  = allParams->copppParams.dsWidth;
    cntxt->allParams.copppParams.dsHeight = allParams->copppParams.dsHeight; //floor(cntxt->odConfig.height / cntxt->copppParams.dsFactor);


    cntxt->allParams.copppParams.obsProbWindowHeight             = allParams->copppParams.obsProbWindowHeight;   // 2m
    cntxt->allParams.copppParams.obsProbWindowWidth              = allParams->copppParams.obsProbWindowWidth;    // 0.5m
    cntxt->allParams.copppParams.minSearchBoxSize                = allParams->copppParams.minSearchBoxSize;
    cntxt->allParams.copppParams.pixelAboveHorizonToCheck        = allParams->copppParams.pixelAboveHorizonToCheck;
    cntxt->allParams.copppParams.minDisparityDifferenceFromModel = allParams->copppParams.minDisparityDifferenceFromModel;
    //cntxt->allParams.copppParams.ransacIter                      = allParams->copppParams.ransacIter;
    //cntxt->allParams.copppParams.ransacErrTh                     = allParams->copppParams.ransacErrTh;

    /* find bottom of obstacles params */
    cntxt->allParams.fbooParams.minProbability                   = allParams->fbooParams.minProbability;
    cntxt->allParams.fbooParams.minConsecutiveVertical           = allParams->fbooParams.minConsecutiveVertical;

    /* find closest obstacles params */
    cntxt->allParams.fcdParams.statisticsBoxHeight               = allParams->fcdParams.statisticsBoxHeight;  // 1.5m
    cntxt->allParams.fcdParams.statisticsNearestPercent          = allParams->fcdParams.statisticsNearestPercent;
    cntxt->allParams.fcdParams.minSearchBoxSize                  = allParams->copppParams.minSearchBoxSize;

    /* detect obstacles params */
    cntxt->allParams.doParams.maxHorizontalToSkip                = allParams->doParams.maxHorizontalToSkip;
    cntxt->allParams.doParams.minHorizontalLength                = allParams->doParams.minHorizontalLength;

    cntxt->allParams.doParams.windowHeightRatioRange[0]          = allParams->doParams.windowHeightRatioRange[0];
    cntxt->allParams.doParams.windowHeightRatioRange[1]          = allParams->doParams.windowHeightRatioRange[1];
    cntxt->allParams.doParams.adaptiveTerminationPercentage      = allParams->doParams.adaptiveTerminationPercentage;
    cntxt->allParams.doParams.maxDisparityDifference             = allParams->doParams.maxDisparityDifference;
    cntxt->allParams.doParams.obsProbabilityMinDisparityToAccept = allParams->doParams.obsProbabilityMinDisparityToAccept;

    /* merge obstacles params */
    cntxt->allParams.moParams.depthThreshold                     = allParams->moParams.depthThreshold;
    cntxt->allParams.moParams.maxHorizontalSkip                  = allParams->moParams.maxHorizontalSkip;
    cntxt->allParams.moParams.tiltThreshold                      = allParams->moParams.tiltThreshold ;
    cntxt->allParams.moParams.adaptiveTerminationPercentage      = allParams->moParams.adaptiveTerminationPercentage;

    /* split obstacles params */
    cntxt->allParams.soParams.minSplitComponentWidth             = allParams->soParams.minSplitComponentWidth;
    cntxt->allParams.soParams.splitInlierThreshold               = allParams->soParams.splitInlierThreshold;
    cntxt->allParams.soParams.adaptiveTerminationPercentage      = allParams->soParams.adaptiveTerminationPercentage;

}



int32_t PTK_Alg_StereoAlgo_ObjectDetectConfig(PTK_Alg_StereoAlgo_ObjectDetect_allParams  * cfg,
                                              PTK_Api_MemoryReq                     * memReq)
{
    uint32_t memSize;

    PTK_Alg_StereoAlgo_ObjectDetectObj cntxt;
    cntxt.allParams = *cfg;

    memReq->numBlks = 2;

    /* Block 0 is for the internal context. */
    memReq->blks[0].size    = sizeof(PTK_Alg_StereoAlgo_ObjectDetectObj);
    memReq->blks[0].memType = PTK_Api_MemoryType_LVL4;

    /* Block1 is for the memory needed by the algorithm */
    memSize = PTK_Alg_StereoAlgo_ObjectDetectGetMem(&cntxt, 0);
    memReq->blks[1].size    = memSize;
    memReq->blks[1].memType = PTK_Api_MemoryType_LVL4;

    //printf("PTK_Alg_StereoAlgo_ObjectDetectConfig memSize:%d, %d\n", memReq->blks[0].size, memReq->blks[1].size);


    /* Block2 is for the output data */
    /*
    memSize = 0;
    memReq->blks[2].size    = memSize;
    memReq->blks[2].memType = PTK_Api_MemoryType_LVL4;
    */

    return PTK_ALG_RET_SUCCESS;
}


PTK_AlgHandle PTK_Alg_StereoAlgo_ObjectDetectInit(PTK_Alg_StereoAlgo_ObjectDetect_allParams * allParams,
                                                  const PTK_Api_MemoryRsp                   * memRsp)
{
	PTK_Alg_StereoAlgo_ObjectDetectObj   *cntxt;

    /* Validate the input parameters. */
    if (!allParams            ||
        !memRsp               ||
        (memRsp->numBlks < 2) ||
        !memRsp->blks[0].mem  ||
        !memRsp->blks[1].mem)
    {
        return NULL;
    }

    /* Initialize the context. */
    cntxt = (PTK_Alg_StereoAlgo_ObjectDetectObj *)memRsp->blks[0].mem;
    cntxt->allParams      = *allParams;

    PTK_Alg_StereoAlgo_ObjectDetectGetMem(cntxt, memRsp->blks[1].mem);

    return cntxt;
}


int32_t PTK_Alg_StereoAlgo_ObjectDetectGetMem(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt,  uint8_t *memPtr)
{
    /*
     * Too much memory allocation?
     * Cannot be compiled on C6x
     */

    //int32_t k;
    int32_t memSize;

    //int32_t i;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoAlgo_configParams      * cfg           = &cntxt->allParams.odConfig;

    //PTK_Alg_StereoAlgo_dispParams        * dispParams    = &cntxt->allParams.dispParams;
    PTK_Alg_StereoAlgo_obsProbParams     * copppParams   = &cntxt->allParams.copppParams;

        
    //PTK_Alg_StereoAlgo_groundModelParams * gmParams;

    //int32_t numDisparity = cfg->maxDisparity - cfg->minDisparity + 1;


    memSize = 0;

#if AUX_CODE_REMOVE
    if (pMem)
    {
    	uvdParams->groundErrArray[0] = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*numDisparity;
    memSize = align(memSize, 8);

    if (pMem)
    {
    	uvdParams->groundErrArray[1] = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*numDisparity;
    memSize = align(memSize, 8);
#endif

    // GP model index for multiple models
    if (pMem)
    {
        cntxt->algoMemData.gpModelIdx = (uint8_t *)(pMem + memSize);
    }
    memSize += sizeof(uint8_t)*(cfg->width);
    memSize = align(memSize, 8);


    // ground plane model
#if AUX_CODE_REMOVE
    if (pMem)
    {
    	gmParams->gpModelLine[0] = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*cfg->height;
    memSize = align(memSize, 8);

    if (pMem)
    {
    	gmParams->gpModelLine[1] = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*cfg->height;
    memSize = align(memSize, 8);

    if (pMem)
    {
    	gmParams->gpModelLine[2] = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*cfg->height;
    memSize = align(memSize, 8);

    if (pMem)
    {
    	gmParams->gpModelLine[3] = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*cfg->height;
    memSize = align(memSize, 8);

    for (k = 0; k < NUM_GP_MODELS;k++)
    {
        gmParams = &cntxt->algoMemData.gmParams[k];
        if (pMem)
        {
            gmParams->igpModel = (int32_t*)(pMem + memSize);
        }
        memSize += sizeof(int32_t)*dispParams->numDisparity;
        memSize = align(memSize, 8);
    }
#endif

    // down-sampled disparity map for obstacle detection
    if (copppParams->dsFactor > 1)
    {
        if (pMem)
        {
        	cntxt->algoMemData.dsDisparity = (float*)(pMem + memSize);
        }
        memSize += sizeof(float)*floor(cfg->height/copppParams->dsFactor)*floor(cfg->width/copppParams->dsFactor);
        memSize = align(memSize, 8);
    }

    // for obstacle detection
    if (pMem)
    {
        cntxt->algoMemData.candidateFound = (int32_t*)(pMem + memSize);
    }
    memSize += sizeof(int32_t)*copppParams->dsHeight*copppParams->dsWidth;
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->algoMemData.numValidFound = (int32_t*)(pMem + memSize);
    }
    memSize += sizeof(int32_t)*copppParams->dsHeight*copppParams->dsWidth;
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->algoMemData.candidateFoundVertical = (int32_t*)(pMem + memSize);
    }
    memSize += sizeof(int32_t)*copppParams->dsHeight*copppParams->dsWidth;
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->algoMemData.numValidFoundVertical = (int32_t*)(pMem + memSize);
    }
    memSize += sizeof(int32_t)*copppParams->dsHeight*copppParams->dsWidth;
    memSize = align(memSize, 8);

    // obstacle probability
    if (pMem)
    {
        cntxt->algoMemData.obstacleProbability = (float*)(pMem + memSize);
    }
    memSize += sizeof(float)*copppParams->dsHeight*copppParams->dsWidth;
    memSize = align(memSize, 8);

    // obstacle bottom line
    if (pMem)
    {
        cntxt->algoMemData.obtacleBottomLine = (int32_t*)(pMem + memSize);
    }
    memSize += sizeof(int32_t)*copppParams->dsWidth;
    memSize = align(memSize, 8);

    // closest disparity
    if (pMem)
    {
        cntxt->closestDisparity = (float*)(pMem + memSize);
    }
    memSize += sizeof(float)*copppParams->dsWidth;
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->closestHeightPrior = (int32_t*)(pMem + memSize);
    }
    memSize += sizeof(int32_t)*copppParams->dsWidth;
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->closestHeightComputed = (int32_t*)(pMem + memSize);
    }
    memSize += sizeof(int32_t)*copppParams->dsWidth;
    memSize = align(memSize, 8);

    // sorted disparity for a column
    if (pMem)
    {
        cntxt->sortedDisparity = (float*)(pMem + memSize);
    }
    memSize += sizeof(float)*copppParams->dsHeight;
    memSize = align(memSize, 8);

    // obstacle detection array 1
    if (pMem)
    {
        cntxt->oda1 = (PTK_Alg_StereoAlgo_obsDetectionArray*)(pMem + memSize);
    }
    memSize += sizeof(PTK_Alg_StereoAlgo_obsDetectionArray);
    memSize = align(memSize, 8);


    if (pMem)
    {
    	cntxt->oda1->detections = (PTK_Alg_StereoAlgo_obsDetection*)(pMem + memSize);
    }
    memSize += sizeof(PTK_Alg_StereoAlgo_obsDetection) * MAX_DETECTIONS;
    memSize = align(memSize, 8);


    // obstacle detection array 2
    if (pMem)
    {
    	cntxt->oda2 = (PTK_Alg_StereoAlgo_obsDetectionArray*)(pMem + memSize);
    }
    memSize += sizeof(PTK_Alg_StereoAlgo_obsDetectionArray);
    memSize = align(memSize, 8);


    if (pMem)
    {
    	cntxt->oda2->detections = (PTK_Alg_StereoAlgo_obsDetection*)(pMem + memSize);
    }
    memSize += sizeof(PTK_Alg_StereoAlgo_obsDetection) * MAX_DETECTIONS;
    memSize = align(memSize, 8);

    // data buffer
    if (pMem)
    {
        cntxt->dataBuffer[0] = (float*)(pMem + memSize);
    }
    memSize += sizeof(float) * copppParams->dsWidth;
    memSize = align(memSize, 8);


    if (pMem)
    {
    	cntxt->dataBuffer[1] = (float*)(pMem + memSize);
    }
    memSize += sizeof(float) * copppParams->dsWidth;
    memSize = align(memSize, 8);


    // to solve linear equation
    if (pMem)
    {
    	cntxt->A = (float*)(pMem + memSize);
    }
    memSize += sizeof(float) * copppParams->dsWidth * 2;
    memSize = align(memSize, 8);

    if (pMem)
    {
    	cntxt->At = (float*)(pMem + memSize);
    }
    memSize += sizeof(float) * copppParams->dsWidth * 2;
    memSize = align(memSize, 8);


    if (pMem)
    {
    	cntxt->b = (float*)(pMem + memSize);
    }
    memSize += sizeof(float) * copppParams->dsWidth;
    memSize = align(memSize, 8);


    // inlierIdx
    if (pMem)
    {
    	cntxt->inlierIdx = (int16_t*)(pMem + memSize);
    }
    memSize += sizeof(int16_t) * copppParams->dsWidth;
    memSize = align(memSize, 8);


    // best inlier idx
    if (pMem)
    {
    	cntxt->bestInlierIdx = (int16_t*)(pMem + memSize);
    }
    memSize += sizeof(int16_t) * copppParams->dsWidth;
    memSize = align(memSize, 8);


    // free drive space
    if (pMem)
    {
        cntxt->freeDriveSpace = (int16_t*)(pMem + memSize);
    }
    memSize += sizeof(int16_t)*2*copppParams->dsHeight;
    memSize = align(memSize, 8);


    return memSize;
}


