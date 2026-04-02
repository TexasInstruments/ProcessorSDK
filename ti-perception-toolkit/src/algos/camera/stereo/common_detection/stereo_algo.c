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
#include <float.h>


#include <perception/algos/stereo/common_detection/stereo_algo.h>


int32_t PTK_Alg_StereoAlgo_computeObstacleProbalilityPerPixel(
        PTK_Alg_StereoAlgo_obsProbParams           * copppParams,
        PTK_Alg_StereoAlgo_camParams               * camParams,
        PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
        PTK_Alg_StereoAlgo_obstacleDetetionMems    * algoMemData)
{
    PTK_Alg_StereoAlgo_groundModelParams * gmParams;

    int32_t i, x, y, xx, k;
    int32_t dsf, dsWidth, dsHeight;
    int32_t detectedHorizon, horizon;

    int32_t widthHere, halfWidthHere;
    int32_t numHere, numValid;
    int32_t limitsX[2];
    float   expectedDisparityHere;
    float   expectedDepthHere;

    dsf      = copppParams->dsFactor;
    dsWidth  = copppParams->dsWidth;
    dsHeight = copppParams->dsHeight;


    detectedHorizon = dsHeight;
    for (k = 0; k < NUM_GP_MODELS; k++)
    {
        // horizon disparity = 1.0
        gmParams = &(gmParamsArr->gmParams[k]);
        y = (1.0 - gmParams->intercept) / gmParams->slope;
        horizon = ceil(y / copppParams->dsFactor);

        if (horizon < detectedHorizon)
        {
            detectedHorizon = horizon;
        }
    }

    // compute obstacle probability vertically
    PTK_Alg_StereoAlgo_computeObstacleProbabilityPerPixel_verticalLoop(copppParams, camParams, gmParamsArr, algoMemData, detectedHorizon);

    // sum up the vertical obstacle candidate horizontally
    for(y = dsHeight - 1; y >= 0; y--)
    {
        for(x = 0; x < dsWidth; x++)
        {
            // to reduce complexity, better keep gpModelIdx for down-sampled resoultion
            k = algoMemData->gpModelIdx[x*dsf];

            expectedDisparityHere = PTK_Alg_StereoAlgo_getDisparityFromModel(&(gmParamsArr->gmParams[k]), (x+0.5f)*dsf, (y+0.5f)*dsf);
            expectedDepthHere = PTK_Alg_StereoAlgo_disparity2Depth(camParams, expectedDisparityHere, (y+0.5)*dsf);

            widthHere = copppParams->minSearchBoxSize;
            if(expectedDepthHere > 0.0f){
                widthHere = MMAX(copppParams->minSearchBoxSize,
                        (int32_t)(camParams->f * copppParams->obsProbWindowWidth / (expectedDepthHere * dsf)));
            }

            if(widthHere > 0)
            {
                numHere = 0;
                numValid = 0;

                halfWidthHere = widthHere / 2;
                widthHere = 2 * halfWidthHere + 1;

                if(x < halfWidthHere){
                    limitsX[0] = 0;
                    limitsX[1] = MMIN(widthHere, dsWidth);     // to use always widthHere columns
                    //limitsX[1] = MIN(x + halfWidthHere + 1, dsWidth);
                } else if(x > (dsWidth - halfWidthHere - 1)){
                    limitsX[0] = MMAX(dsWidth - widthHere, 0); // to use always widthHere columns
                    //limitsX[0] = MAX(x - halfWidthHere, 0);
                    limitsX[1] = dsWidth;
                } else {
                    limitsX[0] = x - halfWidthHere;
                    limitsX[1] = x + halfWidthHere + 1;
                }

                for(xx = limitsX[0]; xx < limitsX[1]; xx++){
                    numHere  += algoMemData->candidateFoundVertical[y * dsWidth + xx];
                    numValid += algoMemData->numValidFoundVertical[y * dsWidth + xx];
                }

                algoMemData->candidateFound[y * dsWidth + x] = numHere;
                algoMemData->numValidFound[y * dsWidth + x]  = numValid;
            }
        }
    }


    for (i = 0; i < dsWidth*dsHeight; i++)
    {
        if(algoMemData->numValidFound[i] > 0)
        {
            algoMemData->obstacleProbability[i] = algoMemData->candidateFound[i]/(float)algoMemData->numValidFound[i];
        } else
        {
            algoMemData->obstacleProbability[i] = 0;
        }
    }

    return detectedHorizon;
}


void PTK_Alg_StereoAlgo_computeObstacleProbabilityPerPixel_verticalLoop(
        PTK_Alg_StereoAlgo_obsProbParams           * copppParams,
        PTK_Alg_StereoAlgo_camParams               * camParams,
        PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
        PTK_Alg_StereoAlgo_obstacleDetetionMems    * algoMemData, 
        int32_t horizon)
{

    int32_t x, y, k;
    int32_t yy;
    int32_t outIndex;

    int32_t heightHere;
    int32_t numHere, numValid;

    float   disparityHere, expectedDisparityHere;
    float   expectedDepthHere;

    int32_t * candidateFoundVertical = algoMemData->candidateFoundVertical;
    int32_t * numValidFoundVertical  = algoMemData->numValidFoundVertical;

    int8_t  dsf      = copppParams->dsFactor;
    int16_t dsWidth  = copppParams->dsWidth;
    int16_t dsHeight = copppParams->dsHeight;

    // initialize data
    for (x=0; x < dsWidth * dsHeight; x++)
    {
        candidateFoundVertical[x] = 0;
        numValidFoundVertical[x]  = 0;
    }


    for (x = 0; x < dsWidth; x++)
    {
        for (y = dsHeight - 1; y >= 0; y--)
        {
            k = algoMemData->gpModelIdx[x*dsf];

            // based on V-disparity ground model
            expectedDisparityHere = PTK_Alg_StereoAlgo_getDisparityFromModel(&gmParamsArr->gmParams[k], (x+0.5)*dsf, (y+0.5)*dsf);

            heightHere = 0;
            expectedDepthHere = -1.0f;

            if(expectedDisparityHere > 1.0f){
                expectedDepthHere = PTK_Alg_StereoAlgo_disparity2Depth(camParams, expectedDisparityHere, (y+0.5)*dsf);

                if(expectedDepthHere > 0.0f){
                    heightHere = MMAX(copppParams->minSearchBoxSize,
                            (int32_t)(camParams->f * copppParams->obsProbWindowHeight / (expectedDepthHere * dsf)));
                }
            }

            if(heightHere <= 0){
                if((horizon - y) > copppParams->pixelAboveHorizonToCheck){
                    continue;
                }
            }

            heightHere = MMAX(heightHere, copppParams->minSearchBoxSize);

            numHere = 0;
            numValid = 0;
            for(yy = MMAX(0, y-heightHere); yy < MMIN(dsHeight, y); yy++){
                disparityHere = algoMemData->dsDisparity[yy * dsWidth + x];
                if(disparityHere > 0){
                    numValid++;

                    if(disparityHere > (expectedDisparityHere + copppParams->minDisparityDifferenceFromModel)){
                        numHere++;
                    }
                }
            }

            outIndex = y * dsWidth + x;
            candidateFoundVertical[outIndex] = numHere;
            numValidFoundVertical[outIndex]  = numValid;
        }
    }

}


void  PTK_Alg_StereoAlgo_findBottomOfObstacle(PTK_Alg_StereoAlgo_obsProbParams        * copppParams,
                                              PTK_Alg_StereoAlgo_bottomObsParams      * fbooParams,
                                              PTK_Alg_StereoAlgo_obstacleDetetionMems * algoMemData)
{
 
    int32_t dsWidth  = copppParams->dsWidth;
    int32_t dsHeight = copppParams->dsHeight;

    int32_t numDetected, detectionStart;
    int32_t x, y;

    // init obstacleBottomLine
    for(x = 0; x < dsWidth; x++)
    {
        algoMemData->obtacleBottomLine[x] = -1;
    }

    for(x = 0; x < dsWidth; x++)
    {
        numDetected    = 0;
        detectionStart = 0;

        for(y = dsHeight - 1; y >= 0; y--)
        {
            if(algoMemData->obstacleProbability[y * dsWidth + x] > fbooParams->minProbability)
            {
                numDetected++;

                if(numDetected == 1)
                {
                    detectionStart = y;
                } else if(numDetected == fbooParams->minConsecutiveVertical)
                {
                    algoMemData->obtacleBottomLine[x] = detectionStart;
                    break;
                }
            } else
            {
                numDetected = 0;
                detectionStart = 0;
            }
        }
    }
}


void  PTK_Alg_StereoAlgo_downsampleDisparity(PTK_Alg_StereoAlgo_configParams  * config,
                                             PTK_Alg_StereoAlgo_obsProbParams * copppParams,
                                             int16_t * disparity,
                                             float   * dsDisparity)
{

    int16_t dsFactor = copppParams->dsFactor;

    if (dsFactor <= 1)
    {
        int16_t i;
        float scale = 1.0 / (1 << NUM_FRAC_BITS);
        for (i = 0; i < config->width * config->height; i++)
        {
            dsDisparity[i] = (disparity[i] * scale);
        }
    } else
    {
        int16_t x, y, xx, yy, cx, cy, ci;
        int16_t minNonZero;

        int16_t newSize[2];
        int16_t limitsX[2];
        int16_t limitsY[2];

        float curImVal, curSum = 0.0;

        newSize[0] = floor(config->height / dsFactor);
        newSize[1] = floor(config->width / dsFactor);
        limitsX[0] = config->width - newSize[1] * dsFactor;
        limitsX[1] = config->width - dsFactor;
        limitsY[0] = config->height - newSize[0] * dsFactor;
        limitsY[1] = config->height - dsFactor;

        minNonZero = 1;

        cy = 0;
        for(y = limitsY[0]; y < limitsY[1]; y += dsFactor)
        {
            cx = 0;
            for(x = limitsX[0]; x < limitsX[1]; x += dsFactor)
            {
                curSum = 0.0;
                ci = 0;

                for(yy = y; yy < (y + dsFactor); yy++)
                {
                    for(xx = x; xx < (x+dsFactor); xx++)
                    {
                        curImVal = disparity[yy * config->width + xx];
                        if (curImVal > 0)
                        {
                            curSum += curImVal;
                            ci++;
                        }
                    }
                }

                if (ci >= minNonZero){
                    curSum /= (ci << NUM_FRAC_BITS);
                } else {
                    curSum = 0;
                }

                dsDisparity[cy * newSize[1] + cx] = curSum;
                cx++;
            }
            cy++;
        }
	}
}


void PTK_Alg_StereoAlgo_filterLowConfDisparity(int32_t width, int32_t height,
                                               int8_t  existConf, int8_t threshold,
                                               int16_t *disparity, int16_t* outDisparity)
{
    // filter low-confident disparity
    int32_t i;
    int8_t  confidence;

    if (existConf)
    {
        for (i = 0; i < width * height; i++)
        {
            outDisparity[i] = ((disparity[i] & 0x7FFF) >> 3);
            confidence = disparity[i] & 0x7;

            if (confidence < threshold)
            {
                outDisparity[i] = 0;
            }
        }
    }
    else
    {
        memcpy(outDisparity, disparity, width*height*sizeof(int16_t));
    }
}

/** Get disparity from the V-disparity model for the given image position (x, y) */
float PTK_Alg_StereoAlgo_getDisparityFromModel(PTK_Alg_StereoAlgo_groundModelParams *gmParams, float x, float y)
{
    // V-disparity based disparity
    float disparity;

    disparity = gmParams->slope * y + gmParams->intercept;
    disparity = MMAX(disparity, 0);

    return disparity;
}

/** Get the y position from the V-disparity model for the given image disparity */
int32_t PTK_Alg_StereoAlgo_findYforDisparity(PTK_Alg_StereoAlgo_groundModelParams * gmParams,
        PTK_Alg_StereoAlgo_camParams * camParams, int32_t x, float disparity)
{
    int32_t y;

    y = (disparity - gmParams->intercept) / gmParams->slope;
    y = floor(y);
    
    if (y < 0)
    {
   	    y = 0;
    }
    else if (y > camParams->height - 1)
    {
        y = camParams->height - 1;
    }

    return y;
}

float PTK_Alg_StereoAlgo_disparity2Depth(PTK_Alg_StereoAlgo_camParams * camParams, float disparity, float ypos)
{
    float depth;
    depth = (camParams->baseline / disparity) *
            (camParams->f *camParams->cosPitch - (ypos - camParams->dcy)*camParams->sinPitch);

    return depth;
}


int32_t PTK_Alg_StereoAlgo_randRange(int32_t min, int32_t max)
{
    if (min == max)
    {
        return min;
    } else
    {
        return (rand() % (max-min)) + min;
    }
}


void PTK_Alg_StereoAlgo_transpose(float* M, int32_t M_width, int32_t M_height, float* Mt)
{
    int32_t x, y;

    for(y=0; y < M_height; y++)
    {
        int32_t ci = y * M_width;
        int32_t co = y;
        for(x = 0; x < M_width; x++)
        {
            Mt[co] = M[ci];

            co += M_height;
            ci++;
        }
    }
}

void PTK_Alg_StereoAlgo_multiplyMatrix(float* iM1, int32_t iM1_width, int32_t iM1_height,
        float *iM2, int32_t iM2_width, int32_t iM2_height,
        float* oM, int32_t oM_width, int32_t oM_height)
{
    int32_t x, y, l;

    for(y = 0; y < oM_height; y++)
    {
    	int32_t co = y * oM_width;

        for(x = 0; x < oM_width; x++)
        {
            float total = 0.0f;

            int32_t ca = y * iM1_width;
            int32_t cb = x;

            for(l = 0; l < iM1_width; l++)
            {
                total += iM1[ca] * iM2[cb];
                ca++;
                cb += iM2_width;
            }

            oM[co] = total;
            co++;
        }
    }
}


int32_t PTK_Alg_StereoAlgo_LUdecomposition(float* A, int32_t A_width, int32_t A_height, float* b, int32_t b_width, int32_t n_toCompute)
{
    int32_t i, j, k, permutedVal = 1;
    int32_t i1, i2;

    float tmp, alpha, s;

    for( i = 0; i < A_height; i++ )
    {
        float d;

        k = i;

        //Find the largest value for the current column
        for( j = i+1; j < A_height; j++ )
        {
            if( fabs(A[j*A_width + i]) > fabs(A[k*A_width + i]) )
                k = j;
        }

        // Check if we're within numerical precision limits
        if( fabs(A[k*A_width + i]) < FLT_EPSILON)
            return 0;

        //For the current column,
        //if the row we're checking is not in order, permute the input and output matrices
        if( k != i )
        {
            for( j = i; j < A_height; j++ )
            {
                i1 = i*A_width + j;
                i2 = k*A_width + j;
                tmp = A[i1];
                A[i1] = A[i2];
                A[i2] = tmp;
            }

            for( j = 0; j < n_toCompute; j++ )
            {
                i1 = i*b_width + j;
                i2 = k*b_width + j;
                tmp = b[i1];
                b[i1] = b[i2];
                b[i2] = tmp;
            }

            permutedVal = -permutedVal;
        }

        d = -1/A[i*A_width + i];

        for( j = i+1; j < A_height; j++ )
        {
            alpha = A[j*A_width + i]*d;

            for( k = i+1; k < A_height; k++ )
                A[j*A_width + k] += alpha*A[i*A_width + k];

            for( k = 0; k < n_toCompute; k++ )
                b[j*b_width + k] += alpha*b[i*b_width + k];
        }

        A[i*A_width + i] = -d;
    }

    for( i = A_height-1; i >= 0; i-- )
    {
        for( j = 0; j < n_toCompute; j++ )
        {
            s = b[i*b_width + j];
            for( k = i+1; k < A_height; k++ )
                s -= A[i*A_width + k]*b[k*b_width + j];

            b[i*b_width + j] = s*A[i*A_width + i];
        }
    }

    return permutedVal;
}


