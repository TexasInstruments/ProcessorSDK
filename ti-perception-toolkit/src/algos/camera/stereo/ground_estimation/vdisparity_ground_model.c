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


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include <perception/base/point.h>
#include <perception/algos/stereo/ground_estimation/vdisparity_ground_model.h>



void PTK_Alg_StereoAlgo_computeVDisparityGroundModel(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt,
                                                     PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr)
{
    PTK_Alg_StereoAlgo_camParams    *camParams  = &cntxt->allParams.camParams;
    PTK_Alg_StereoAlgo_uvDispParams *uvdParams  = &cntxt->allParams.uvDispParams;
    PTK_Alg_StereoAlgo_roadParams   *roadParams = &cntxt->allParams.roadParams;

    int32_t status;
    int32_t i, j, k;

    int32_t  startY         = uvdParams->startY;
    int32_t  lastY          = uvdParams->lastY;
    int32_t  startX         = uvdParams->startX;
    int32_t  lastX          = uvdParams->lastX;
    int32_t  vdWidth        = uvdParams->vdWidth;

    int32_t  roadSizeBase   = roadParams->roadSizeBase;
    int32_t  roadSizeTop    = roadParams->roadSizeTop;
    int32_t *roadCenterBase = roadParams->roadCenterBase;

    int16_t *disparity      = cntxt->outDisparity;
    float    alpha, curRoadSize;
    int16_t  disp;
    int16_t  halfPelDisp = (1 << (NUM_FRAC_BITS >> 1));

    int32_t  numGPDisparity[NUM_GP_MODELS], totalNumPixels[NUM_GP_MODELS];


    // construct V-disparity map
    for (k = 0; k < NUM_GP_MODELS; k++)
    {
        numGPDisparity[k] = 0;
        totalNumPixels[k] = 0;
        memset(cntxt->vDisparity[k], 0, sizeof(int32_t) * uvdParams->vdWidth * uvdParams->vdHeight);
    }


    if (uvdParams->useRoadParams)
    {
        for (j = startY; j < lastY; j++)
        {
            alpha       = (float)(j - startY) / (lastY - startY);
            curRoadSize = alpha * roadSizeBase + (1 - alpha) * roadSizeTop;
            curRoadSize = floor(curRoadSize / 2.0f + 0.5f);

            for (i = roadCenterBase[0] - curRoadSize; i < roadCenterBase[0] + curRoadSize; i++)
            {
                disp = (disparity[j * camParams->width + i] + halfPelDisp) >> NUM_FRAC_BITS;
                if (disp != 0)
                {
                    k = cntxt->gpModelIdx[i];
                    cntxt->vDisparity[k][j * vdWidth + disp] += 1;
                }
            }
        }
    } else
    {
        for (j = startY; j < lastY; j++)
        {
            for (i = startX; i < lastX; i++)
            {
                disp = disparity[j * camParams->width + i] >> NUM_FRAC_BITS;
                if (disp != 0)
                {
                    k = cntxt->gpModelIdx[i];
                    cntxt->vDisparity[k][j * vdWidth + disp] += 1;
                }
            }
        }
    }


    // get disparities corresponding to ground plane
    PTK_Alg_StereoAlgo_updateCandidateGPDisparity(cntxt, numGPDisparity, totalNumPixels);

    // estimate ground plane model from V disparity
    status = PTK_Alg_StereoAlgo_modelGroundPlane(cntxt, gmParamsArr, numGPDisparity, totalNumPixels);


#if 1
    // update GP model history
    // not working with pipelining
    if (status == ALGORITHM_PROCESS_OK)
    {
        PTK_Alg_StereoAlgo_updateGroundPlaneModelHistory(cntxt, gmParamsArr);
    }

    // get the final average GP model
    PTK_Alg_StereoAlgo_averageGrounPlaneModel(cntxt, gmParamsArr, numGPDisparity); 
#else
    if (status == ALGORITHM_PROCESS_OK)
    {
        PTK_Alg_StereoAlgo_groundModelParams * gmParams;

        for (k = 0; k < NUM_GP_MODELS; k++)
        {
            gmParams = &gmParamsArr->gmParams[k];
            gmParams->validGM = 1;
        }

    } else
    {
        PTK_Alg_StereoAlgo_groundModelParams * gmParams;
        for (k = 0; k < NUM_GP_MODELS; k++)
        {
            gmParams = &gmParamsArr->gmParams[k];
            gmParams->validGM = 1;
        }
    }

#endif
}



void PTK_Alg_StereoAlgo_updateCandidateGPDisparity(PTK_Alg_StereoAlgo_GroundEstimationObj *cntxt, int32_t * numGPDisparity, int32_t * totalNumPixels)
{
    PTK_Alg_StereoAlgo_uvDispParams * uvdParams    = &cntxt->allParams.uvDispParams;
    PTK_Alg_StereoAlgo_groundParams * groundParams = &cntxt->groundParams;

    float   * gpThArray    = groundParams->gpThArray;
    float   * yOffsetArray = groundParams->yOffsetArray;

    int32_t   vdHeight     = uvdParams->vdHeight;
    int32_t   vdWidth      = uvdParams->vdWidth;

    int32_t i, j, k;
    int32_t count;

    for (k = 0; k < NUM_GP_MODELS; k++)
    {
    	count = 0;
        for (j = 0; j < vdHeight; j++)
        {
            for (i = 0; i < vdWidth; i++)
            {
                if (cntxt->vDisparity[k][j * vdWidth + i] > gpThArray[i])
                {
                    cntxt->candidateGP[k][count]     = i;                             // disparity index
                    cntxt->candidateGP[k][count + 1] = j - yOffsetArray[i];           // row (height) index, negative because Y is flipped
                    cntxt->candidateGP[k][count + 2] = j;
                    cntxt->candidateGP[k][count + 3] = cntxt->vDisparity[k][j * vdWidth + i];  // # of pixels
                    totalNumPixels[k] += cntxt->candidateGP[k][count + 3];

                    count += 4;
                    break;
               }
            }
        }

        numGPDisparity[k] = (count/4);
    }
}

int32_t PTK_Alg_StereoAlgo_modelGroundPlane(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt, 
                                            PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
                                            int32_t                                    * numGPDisparity, 
                                            int32_t                                    * totalNumPixels)
{
    PTK_Alg_StereoAlgo_uvDispParams      * uvdParams = &cntxt->allParams.uvDispParams;
    PTK_Alg_StereoAlgo_groundModelParams * gmParams  = gmParamsArr->gmParams;

    //float * candidateGP = cntxt->candidateGP;
    
    float maxAngle, minAngle;
    float slope, icpt, degree;
    float optSlope = 0.0f;
    float optIcpt  = 0.0f;

    float x, y;
    float esty, error;

    int32_t i, k;
    int32_t minOutlierIdx;   // = numGPDisparity;
    int32_t minOutlierPixel; // = INT_MAX;

    int32_t numOutlierIdx;
    int32_t numOutlierPixel;

    int32_t maxIter = uvdParams->ransacIter;
    int32_t errTh   = uvdParams->ransacErrTh;

    int32_t iter;
    int32_t ptIdxA, ptIdxB;
    int32_t halfNumGPDisparity; // = numGPDisparity / 2;

    PTK_2D_Point ptA, ptB;

    // set angle limits
    maxAngle = 90.0f;
    minAngle = 1.0f;

    for (k = 0; k < NUM_GP_MODELS; k++)
    {
        minOutlierIdx = numGPDisparity[k];
        minOutlierPixel = INT_MAX;

        halfNumGPDisparity = numGPDisparity[k] / 2;

        // run RANSAC
        iter  = 0;
        while (iter < maxIter)
        {
            ptIdxA = PTK_Alg_StereoAlgo_randRange(0, numGPDisparity[k]);
            if (ptIdxA > halfNumGPDisparity)
            {
                ptIdxB = PTK_Alg_StereoAlgo_randRange(0, ptIdxA);
            } else
            {
                ptIdxB = PTK_Alg_StereoAlgo_randRange(ptIdxA+1, numGPDisparity[k]);
            }

            iter = iter + 1;

            assert(ptIdxA != ptIdxB);

            ptA.x = cntxt->candidateGP[k][ptIdxA*4 + 1];
            ptA.y = cntxt->candidateGP[k][ptIdxA*4 + 0];
            ptB.x = cntxt->candidateGP[k][ptIdxB*4 + 1];
            ptB.y = cntxt->candidateGP[k][ptIdxB*4 + 0];


            slope = (ptB.y - ptA.y) / (ptB.x - ptA.x);
            icpt = ptA.y - (slope * ptA.x);
            if (slope < 0){
                continue;
            }

            degree = slope * 180 / PI;
            if (degree > maxAngle || degree < minAngle){
                continue;
            }

            numOutlierIdx   = 0;
            numOutlierPixel = 0;

            for (i = 0; i < numGPDisparity[k]; i++)
            {
                x = cntxt->candidateGP[k][i*4 + 1];
                y = cntxt->candidateGP[k][i*4 + 0];

                esty  = slope * x + icpt;
                error = fabs(esty - y);

                if (error > errTh)
                {
                    numOutlierIdx   += 1;
                    numOutlierPixel += cntxt->candidateGP[k][i*4 + 3];
                }
            }

            if (numOutlierIdx < minOutlierIdx)
            {
                minOutlierIdx = numOutlierIdx;
            }

            if (numOutlierPixel < minOutlierPixel)
            {
                optSlope = slope;
                optIcpt = icpt;
                minOutlierPixel = numOutlierPixel;
            }

            // early stop RANSAC
            if (minOutlierPixel < 0.1f * totalNumPixels[k])
            {
                break;
            }

        }

        if ((minOutlierPixel > 0.9f * totalNumPixels[k]) || (minOutlierIdx > 0.9f * numGPDisparity[k]))
        {
            return ALGORITHM_PROCESS_FAIL;
        }

        gmParams[k].slope     = optSlope;
        gmParams[k].intercept = optIcpt;

        //printf("model_id:%d) slope:%f,  intercept:%f\n", k, optSlope, optIcpt);

    }

    return ALGORITHM_PROCESS_OK;

}

void PTK_Alg_StereoAlgo_updateGroundPlaneModelHistory(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt,
                                                      PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr)
{
    PTK_Alg_StereoAlgo_groundModelParams  * gmParams;
    PTK_Alg_StereoAlgo_groundModelHistory * gmHistory;

    int32_t i, k;


    /* Temporary model estimation error handling 
     * Model is so different from the previous frame, ignore it
     */
    for (k = 0; k < NUM_GP_MODELS; k++)
    {
        gmParams = &gmParamsArr->gmParams[k];
        gmHistory = &cntxt->gmHistoryArray.gmHistory[k];

        if (gmHistory->frameNo > 0)
        {
            if (fabs(gmParams->intercept -  gmHistory->interceptArray[gmHistory->frameNo - 1]) > 20)
            {
                return;
            }
        }
    }

    for (k = 0; k < NUM_GP_MODELS; k++)
    {
        gmParams = &gmParamsArr->gmParams[k];
        /*
        gmParams = &gmParamsArr->gmParams[k];
        if (gmParams->frameNo == gmParams->maxFrameNo - 1)
        {
            for (i = 0; i < gmParams->frameNo - 1; i++)
            {
                gmParams->slopeArray[i]     = gmParams->slopeArray[i + 1];
                gmParams->interceptArray[i] = gmParams->interceptArray[i + 1];
            }
        }

        gmParams->slopeArray[gmParams->frameNo]     = gmParams->slope;
        gmParams->interceptArray[gmParams->frameNo] = gmParams->intercept;

        gmParams->frameNo = gmParams->frameNo + 1;
        if (gmParams->frameNo > gmParams->maxFrameNo - 1)
        {
            gmParams->frameNo = gmParams->maxFrameNo - 1;
        }
        */


        gmHistory = &cntxt->gmHistoryArray.gmHistory[k];
        if (gmHistory->frameNo == gmHistory->maxFrameNo - 1)
        {
            for (i = 0; i < gmHistory->frameNo - 1; i++)
            {
                gmHistory->slopeArray[i]     = gmHistory->slopeArray[i + 1];
                gmHistory->interceptArray[i] = gmHistory->interceptArray[i + 1];
            }
        }

        gmHistory->slopeArray[gmHistory->frameNo]   = gmParams->slope;
        gmHistory->interceptArray[gmHistory->frameNo] = gmParams->intercept;

        gmHistory->frameNo = gmHistory->frameNo + 1;
        if (gmHistory->frameNo > gmHistory->maxFrameNo - 1)
        {
            gmHistory->frameNo = gmHistory->maxFrameNo - 1;
        }       
    }
}


void PTK_Alg_StereoAlgo_averageGrounPlaneModel(PTK_Alg_StereoAlgo_GroundEstimationObj     * cntxt,
                                               PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
                                               int32_t                                    * numGPDisparity)
{
    //PTK_Alg_StereoAlgo_dispParams        * dispParams = &cntxt->allParams.dispParams;
    PTK_Alg_StereoAlgo_groundModelParams  * gmParams;
    PTK_Alg_StereoAlgo_groundModelHistory * gmHistory;

    int32_t i, k;
    
    float sumSlope, sumIntercept;
    
    for (k = 0; k < NUM_GP_MODELS; k++)
    {
        sumSlope     = 0.0f;
        sumIntercept = 0.0f;
        gmParams  = &gmParamsArr->gmParams[k];
        gmHistory = &cntxt->gmHistoryArray.gmHistory[k];

        if (gmHistory->frameNo > 0)
        {
            // GM is vaide now
            gmParams->validGM = 1;

            for (i = 0; i < gmHistory->frameNo; i++)
            {
                sumSlope     += gmHistory->slopeArray[i];
                sumIntercept += gmHistory->interceptArray[i];
            }

            gmParams->slope     = sumSlope / gmHistory->frameNo;
            gmParams->intercept = sumIntercept / gmHistory->frameNo;

#if AUX_CODE_REMOVE
            // construct GP model
            for (j = 0; j < height; j++)
            {
                disp = j * gmParams->slope + gmParams->intercept;

                gmParams->gpModelLine[0][j] = -1;
                gmParams->gpModelLine[1][j] = -1;
                if (disp >= 0)
                {
                    gmParams->gpModelLine[0][j] = disp;
                    gmParams->gpModelLine[1][j] = j;
                }
            }

            if (uvdParams->adaptiveGroundError == 1)
            {
                for (j = 0; j < height; j++)
                {
                    gmParams->gpModelLine[2][j] = uvdParams->groundErrArray[0][(int32_t)MIN(floor(gmParams->gpModelLine[0][j]), dispParams->maxDisparity)];
                    gmParams->gpModelLine[3][j] = uvdParams->groundErrArray[1][(int32_t)MIN(floor(gmParams->gpModelLine[0][j]), dispParams->maxDisparity)];
                }
            } else
            {
                for (j = 0; j < height; j++)
                {
                    gmParams->gpModelLine[2][j] = disp;
                    gmParams->gpModelLine[3][j] = j;
                }
            }

            // inverse GP model
            for (i = 0; i < dispParams->numDisparity; i++)
            {
                v = floor((i - gmParams->intercept)/ gmParams->slope);
                gmParams->igpModel[i] = -1;
                if (v >= 0)
                {
                    gmParams->igpModel[i] = v;
                }
            }
#endif
        }
	}
}
