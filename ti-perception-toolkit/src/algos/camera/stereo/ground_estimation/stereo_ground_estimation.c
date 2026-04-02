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

#include <perception/algos/stereo/ground_estimation/stereo_ground_estimation.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))



void PTK_Alg_StereoAlgo_GroundEstimationSetParams(PTK_Alg_StereoAlgo_GroundEstimation_allParams* allParams,
                                                  PTK_Alg_StereoAlgo_GroundEstimationObj   * cntxt)
{
    int32_t i;
    int32_t numDisparity;
    //float   invF;
    float   d0;

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

/*
    printf("================= Road Params ==================\n");
    printf("roadCenterBase: (%d, %d)\n", cntxt->allParams.roadParams.roadCenterBase[0], cntxt->allParams.roadParams.roadCenterBase[1]);
    printf("roadCenterTop:  (%d, %d)\n", cntxt->allParams.roadParams.roadCenterTop[0], cntxt->allParams.roadParams.roadCenterTop[1]);
    printf("roadSizeBase: %d, roadSizeTop: %d\n", cntxt->allParams.roadParams.roadSizeBase, cntxt->allParams.roadParams.roadSizeTop);
    printf("maxDepth: %d, egoSize: %d\n", cntxt->allParams.roadParams.maxDepth, cntxt->allParams.roadParams.egoSize);
*/
    /* initialize disparity params */
    cntxt->allParams.dispParams.confTh            = allParams->dispParams.confTh;
    cntxt->allParams.dispParams.minDisparity      = allParams->dispParams.minDisparity;
    cntxt->allParams.dispParams.maxDisparity      = allParams->dispParams.maxDisparity;
    cntxt->allParams.dispParams.numDisparity      = allParams->dispParams.numDisparity;
    cntxt->allParams.dispParams.fracBits          = allParams->dispParams.fracBits;
    cntxt->allParams.dispParams.farZ              = allParams->dispParams.farZ;

    /* initialize uv-Disparity params */
    cntxt->allParams.uvDispParams.dsFactor              = allParams->uvDispParams.dsFactor;
    cntxt->allParams.uvDispParams.useRoadParams         = allParams->uvDispParams.useRoadParams;
    cntxt->allParams.uvDispParams.adaptiveGroundError   = allParams->uvDispParams.adaptiveGroundError;
    cntxt->allParams.uvDispParams.roadWidth             = allParams->uvDispParams.roadWidth;
    cntxt->allParams.uvDispParams.ransacIter            = allParams->uvDispParams.ransacIter;
    cntxt->allParams.uvDispParams.ransacErrTh           = allParams->uvDispParams.ransacErrTh;
    cntxt->allParams.uvDispParams.vdWidth               = allParams->uvDispParams.vdWidth;
    cntxt->allParams.uvDispParams.vdHeight              = allParams->uvDispParams.vdHeight;

    if (cntxt->allParams.uvDispParams.useRoadParams == 1)
    {
        // startX and lastX are refined later
        cntxt->allParams.uvDispParams.startX = allParams->uvDispParams.startX;
        cntxt->allParams.uvDispParams.lastX  = allParams->uvDispParams.lastX;

        cntxt->allParams.uvDispParams.startY = allParams->uvDispParams.startY;
        cntxt->allParams.uvDispParams.lastY  = allParams->uvDispParams.lastY;
    } else
    {
        cntxt->allParams.uvDispParams.startX = allParams->uvDispParams.startX;
        cntxt->allParams.uvDispParams.lastX  = allParams->uvDispParams.lastX ;

        cntxt->allParams.uvDispParams.startY = allParams->uvDispParams.startY;
        cntxt->allParams.uvDispParams.lastY  = allParams->uvDispParams.lastY;
    }

    /*
    printf("===================== UVDisp Params ==================\n");
    printf("dsFactor:%d\n", cntxt->allParams.uvDispParams.dsFactor);
    printf("useRoadParams:%d\n", cntxt->allParams.uvDispParams.useRoadParams);
    printf("adaptiveGroundError:%d\n", cntxt->allParams.uvDispParams.adaptiveGroundError);
    printf("roadWidth:%d\n", cntxt->allParams.uvDispParams.roadWidth);
    printf("ransacIter:%d\n", cntxt->allParams.uvDispParams.ransacIter);
    printf("ransacErrTh:%.4f\n", cntxt->allParams.uvDispParams.ransacErrTh);
    printf("vdWidth:%d\n", cntxt->allParams.uvDispParams.vdWidth);
    printf("vdHeight:%d\n", cntxt->allParams.uvDispParams.vdHeight);
    printf("startX, Y: (%d, %d)\n", cntxt->allParams.uvDispParams.startX, cntxt->allParams.uvDispParams.startY);
    printf("last, Y: (%d, %d)\n", cntxt->allParams.uvDispParams.lastX, cntxt->allParams.uvDispParams.lastY);
    printf("======================================================\n");
    /////////////////////////////////////////////////////////////////////////////////////////////////
    */
    numDisparity = cntxt->allParams.dispParams.numDisparity;

    //printf("==== baseline:%.4f, dsFactor:%d, roadWidth:%d\n", cntxt->allParams.camParams.baseline,
    //  cntxt->allParams.uvDispParams.dsFactor,  cntxt->allParams.uvDispParams.roadWidth );

    for (i = 0; i < numDisparity; i++)
    {
        cntxt->groundParams.gpThArray[i] = cntxt->allParams.uvDispParams.roadWidth * i/*(i + 1)*/ /
                (cntxt->allParams.uvDispParams.dsFactor * cntxt->allParams.camParams.baseline);

        //printf("%d: cntxt->allParams.uvDispParams.gpThArray[i]:%.4f\n", i, cntxt->allParams.uvDispParams.gpThArray[i]);
    }

    if (cntxt->allParams.uvDispParams.adaptiveGroundError == 1)
    {
        for (i = 0; i < numDisparity; i++)
        {
            d0 = i + 0.5;
            //z0 = cntxt->camParams.f * cntxt->camParams.baseline / d0;

            // set possible error in GP model
#if AUX_CODE_REMOVE
            dmax = cntxt->camParams.f * cntxt->camParams.baseline / (z0 + 5000); // 5m farther
            cntxt->uvDispParams.groundErrArray[0][i] = MIN(-1, MAX(-10, (dmax - d0)));

            dmin = cntxt->camParams.f * cntxt->camParams.baseline / (z0 - 2000); // 2m closer
            cntxt->uvDispParams.groundErrArray[1][i] = MAX(1, MIN(3, (dmin - d0)));
#endif

            cntxt->groundParams.yOffsetArray[i] =
                    50 * d0 / (cntxt->allParams.uvDispParams.dsFactor * cntxt->allParams.camParams.baseline);  // 5cm
            cntxt->groundParams.objHeightArray[i] =
                    500 * d0 / (cntxt->allParams.uvDispParams.dsFactor * cntxt->allParams.camParams.baseline); // 50cm
        }
    }

    /* create gpModelIdx */
    for (i = 0;  i < allParams->camParams.width; i++)
    {
        cntxt->gpModelIdx[i] = floor(i / numColsPerModel);
    }

    for ( i = 0; i < NUM_GP_MODELS; i++)
    {
        cntxt->gmHistoryArray.gmHistory[i].frameNo    = 0;
        cntxt->gmHistoryArray.gmHistory[i].maxFrameNo = 5;         
    }

}



int32_t PTK_Alg_StereoAlgo_GroundEstimationConfig(PTK_Alg_StereoAlgo_GroundEstimation_allParams * cfg,
                                                  PTK_Api_MemoryReq                             * memReq)
{
    uint32_t memSize;

    PTK_Alg_StereoAlgo_GroundEstimationObj cntxt;
    cntxt.allParams = *cfg;

/*
    printf("******* ===================== UVDisp Params ==================\n");
      printf("dsFactor:%d\n", cntxt.allParams.uvDispParams.dsFactor);
      printf("useRoadParams:%d\n", cntxt.allParams.uvDispParams.useRoadParams);
      printf("adaptiveGroundError:%d\n", cntxt.allParams.uvDispParams.adaptiveGroundError);
      printf("roadWidth:%d\n", cntxt.allParams.uvDispParams.roadWidth);
      printf("ransacIter:%d\n", cntxt.allParams.uvDispParams.ransacIter);
      printf("ransacErrTh:%.4f\n", cntxt.allParams.uvDispParams.ransacErrTh);
      printf("vdWidth:%d\n", cntxt.allParams.uvDispParams.vdWidth);
      printf("vdHeight:%d\n", cntxt.allParams.uvDispParams.vdHeight);
      printf("startX, Y: (%d, %d)\n", cntxt.allParams.uvDispParams.startX, cntxt.allParams.uvDispParams.startY);
      printf("last, Y: (%d, %d)\n", cntxt.allParams.uvDispParams.lastX, cntxt.allParams.uvDispParams.lastY);
      printf("======================================================\n");
*/
    memReq->numBlks = 2;

    /* Block 0 is for the internal context. */
    memReq->blks[0].size    = sizeof(PTK_Alg_StereoAlgo_GroundEstimationObj);
    memReq->blks[0].memType = PTK_Api_MemoryType_LVL4;

    /* Block1 is for the memory needed by the algorithm */
    memSize = PTK_Alg_StereoAlgo_GroundEstimationGetMem(&cntxt, 0);
    memReq->blks[1].size    = memSize;
    memReq->blks[1].memType = PTK_Api_MemoryType_LVL4;

    //printf("PTK_Alg_StereoAlgo_GroundEstimationGetMem memSize:%d, %d\n", memReq->blks[0].size, memReq->blks[1].size);


    /* Block2 is for the output data */
    /*
    memSize = 0;
    memReq->blks[2].size    = memSize;
    memReq->blks[2].memType = PTK_Api_MemoryType_LVL4;
    */

    return PTK_ALG_RET_SUCCESS;
}


PTK_AlgHandle PTK_Alg_StereoAlgo_GroundEstimationInit(PTK_Alg_StereoAlgo_GroundEstimation_allParams  * allParams,
                                                      const PTK_Api_MemoryRsp                        * memRsp)
{
	PTK_Alg_StereoAlgo_GroundEstimationObj   *cntxt;

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
    cntxt = (PTK_Alg_StereoAlgo_GroundEstimationObj *)memRsp->blks[0].mem;
    cntxt->allParams      = *allParams;


    PTK_Alg_StereoAlgo_GroundEstimationGetMem(cntxt, memRsp->blks[1].mem);

    return cntxt;
}


int32_t PTK_Alg_StereoAlgo_GroundEstimationGetMem(PTK_Alg_StereoAlgo_GroundEstimationObj * cntxt,  uint8_t *memPtr)
{
    /*
     * Too much memory allocation?
     * Cannot be compiled on C6x
     */


    int32_t k;
    int32_t memSize;

    //int32_t i;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoAlgo_configParams      * cfg           = &cntxt->allParams.config;
    PTK_Alg_StereoAlgo_uvDispParams      * uvdParams     = &cntxt->allParams.uvDispParams;
    //PTK_Alg_StereoAlgo_dispParams        * dispParams    = &cntxt->allParams.dispParams;
    PTK_Alg_StereoAlgo_groundParams      * groundParams  = &cntxt->groundParams;
        
    //PTK_Alg_StereoAlgo_groundModelParams * gmParams;

    int32_t numDisparity = cfg->maxDisparity - cfg->minDisparity + 1;


    memSize = 0;
    if (pMem)
    {
    	groundParams->gpThArray = (float *)pMem;
    }
    memSize += sizeof(float)*numDisparity;
    memSize = align(memSize, 8);


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

    if (pMem)
    {
    	groundParams->yOffsetArray = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*numDisparity;
    memSize = align(memSize, 8);


    if (pMem)
    {
    	groundParams->objHeightArray = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*numDisparity;
    memSize = align(memSize, 8);

    // GP model index for multiple models
    if (pMem)
    {
    	cntxt->gpModelIdx = (uint8_t *)(pMem + memSize);
    }
    memSize += sizeof(uint8_t)*(cfg->width);
    memSize = align(memSize, 8);


    for (k = 0; k < NUM_GP_MODELS;k++)
    {
        if (pMem)
        {
            cntxt->vDisparity[k] = (int32_t *)(pMem + memSize);
        }
        memSize += sizeof(int32_t)*uvdParams->vdWidth*uvdParams->vdHeight;
        memSize = align(memSize, 8);
    }

    for (k = 0; k < NUM_GP_MODELS;k++)
    {
        if (pMem)
        {
            cntxt->candidateGP[k] = (float *)(pMem + memSize);
        }
        memSize += sizeof(float)*uvdParams->vdHeight*uvdParams->vdWidth*4;
        memSize = align(memSize, 8);
    }

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
        gmParams = &cntxt->gmParams[k];
        if (pMem)
        {
            gmParams->igpModel = (int32_t*)(pMem + memSize);
        }
        memSize += sizeof(int32_t)*dispParams->numDisparity;
        memSize = align(memSize, 8);
    }
#endif  

    return memSize;
}



void PTK_Alg_StereoAlgo_GroundEstimationReset(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt)
{
    int32_t i;
    for (i = 0; i < NUM_GP_MODELS; i++)
    {
        cntxt->gmHistoryArray.gmHistory[i].frameNo    = 0;
        cntxt->gmHistoryArray.gmHistory[i].maxFrameNo = 5; 
    }
}
