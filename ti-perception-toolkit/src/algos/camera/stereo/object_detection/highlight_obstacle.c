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

#include <perception/algos/stereo/ground_estimation/vdisparity_ground_model.h>
#include <perception/algos/stereo/object_detection/highlight_obstacle.h>
#include <perception/algos/stereo/object_detection/obstacle_from_disparity.h>
#include <perception/algos/stereo/common_detection/stereo_algo.h>



void PTK_Alg_StereoAlgo_computePerspectiveForDetection(PTK_Alg_StereoAlgo_ObjectDetectObj         * cntxt,                                                       
                                                       PTK_Alg_StereoAlgo_obsDetectionArray       * inOda,
                                                       PTK_Alg_StereoAlgo_obsDetectionArray       * outOda)
{
    PTK_Alg_StereoAlgo_obsProbParams     * copppParams = &cntxt->allParams.copppParams;
    PTK_Alg_StereoAlgo_detectObsParams   * doParams    = &cntxt->allParams.doParams;
    PTK_Alg_StereoAlgo_camParams         * camParams   = &cntxt->allParams.camParams;
    PTK_Alg_StereoAlgo_closestObsParams  * fcdParams   = &cntxt->allParams.fcdParams;

    PTK_Alg_StereoAlgo_lineModel           model;

    int32_t i, k;
    int32_t bottomYBegin, bottomYEnd;
    int32_t componentBeginD, componentEndD;
    int32_t windowHeightBegin, windowHeightEnd;
    float   disparityBegin, disparityEnd;
    float   depthBegin, depthEnd;
    float   bottomBegin, bottomEnd;
    float   inlierTh = 0.25;

    int8_t dsf = copppParams->dsFactor;

    outOda->numDetections = inOda->numDetections;

    for (i = 0; i < outOda->numDetections; i++)
    {

        componentBeginD = inOda->detections[i].xLocations[0] / dsf;
        componentEndD   = inOda->detections[i].xLocations[1] / dsf;

        // estimate perspective line model
        model = PTK_Alg_StereoAlgo_computeALineModel(cntxt, componentBeginD, componentEndD, dsf, inlierTh, doParams->adaptiveTerminationPercentage);

        disparityBegin    = inOda->detections[i].disparities[0];
        disparityEnd      = inOda->detections[i].disparities[1];
        depthBegin        = inOda->detections[i].depths[0];
        depthEnd          = inOda->detections[i].depths[1];
        windowHeightBegin = inOda->detections[i].windowHeights[0];
        windowHeightEnd   = inOda->detections[i].windowHeights[1];
        bottomBegin       = inOda->detections[i].bottomYs[0];
        bottomEnd         = inOda->detections[i].bottomYs[1];

        if(model.offset != 0.0f)
        {

            // Ransac could find a solution
            //    disparityBegin = model[0]
            //    disparityEnd = model[0] + model[1]*len(curClosest)
            //    depthBegin = FindObstacles.disparityToDepth(disparityBegin)
            //    depthEnd = FindObstacles.disparityToDepth(disparityEnd)
            disparityBegin = model.offset + model.dx*(componentBeginD)*dsf;
            disparityEnd   = model.offset + model.dx*(componentEndD)*dsf;

            depthBegin     = PTK_Alg_StereoAlgo_disparity2Depth(camParams, disparityBegin, bottomBegin);
            depthEnd       = PTK_Alg_StereoAlgo_disparity2Depth(camParams, disparityEnd, bottomEnd);

         
            //    if depthBegin > .001:
            //        windowHeightBegin = int(round(focalLength*obstacleHeight/depthBegin))
            //    else:
            //        windowHeightBegin = 0
            //    if depthEnd > .001:
            //        windowHeightEnd = int(round(focalLength*obstacleHeight/depthEnd))
            //    else:
            //        windowHeightEnd = 0
            if(depthBegin > .001 && depthEnd > .001)
            {
                windowHeightBegin = (int32_t)(camParams->f * fcdParams->statisticsBoxHeight / depthBegin);
                windowHeightEnd   = (int32_t)(camParams->f * fcdParams->statisticsBoxHeight / depthEnd);
            } else
            {
                //windowHeightBegin = 0;
                //windowHeightEnd = 0;
            }
        }

        k = cntxt->algoMemData.gpModelIdx[(inOda->detections[i].xLocations[0])];
        bottomYBegin = PTK_Alg_StereoAlgo_findYforDisparity(&cntxt->gmParamsArr->gmParams[k], camParams, inOda->detections[i].xLocations[0], disparityBegin);
        k = cntxt->algoMemData.gpModelIdx[(inOda->detections[i].xLocations[1])];
        bottomYEnd   = PTK_Alg_StereoAlgo_findYforDisparity(&cntxt->gmParamsArr->gmParams[k], camParams, inOda->detections[i].xLocations[1], disparityEnd);

        outOda->detections[i].bottomYs[0]          = bottomYBegin;
        outOda->detections[i].bottomYs[1]          = bottomYEnd;
        outOda->detections[i].depths[0]            = depthBegin;
        outOda->detections[i].depths[1]            = depthEnd;
        outOda->detections[i].detectionLineModel   = model;
        outOda->detections[i].disparities[0]       = disparityBegin;
        outOda->detections[i].disparities[1]       = disparityEnd;
        outOda->detections[i].isValid              = 1;
        outOda->detections[i].originalDetectedArea = inOda->detections[i].originalDetectedArea;
        outOda->detections[i].windowHeights[0]     = windowHeightBegin;
        outOda->detections[i].windowHeights[1]     = windowHeightEnd;
        outOda->detections[i].xLocations[0]        = inOda->detections[i].xLocations[0];
        outOda->detections[i].xLocations[1]        = inOda->detections[i].xLocations[1];
    }
}


void PTK_Alg_StereoAlgo_mergeDetection(PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
                                       PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
                                       PTK_Alg_StereoAlgo_obsDetectionArray * newOda)
{
    PTK_Alg_StereoAlgo_mergeObsParams *moParams = &cntxt->allParams.moParams;

    int32_t i;
    int32_t startInd = 0;

    int32_t xLocationDiff;
    float   dxDiff, depthDiff;

    // reset the number of merged detections to 0
    newOda->numDetections = 0;

    if(curOda->numDetections == 0)
    {
        return;
    }

    for(i = 0; i < curOda->numDetections-1; i++)
    {
        PTK_Alg_StereoAlgo_obsDetection curDet  = curOda->detections[i];
        PTK_Alg_StereoAlgo_obsDetection nextDet = curOda->detections[i+1];

        dxDiff        = fabs(curDet.detectionLineModel.dx - nextDet.detectionLineModel.dx);
        depthDiff     = fabs(curDet.depths[1]-nextDet.depths[0]);
        xLocationDiff = (nextDet.xLocations[0]-curDet.xLocations[1]);

        //If the next component still matches, just keep on going
        if(dxDiff < moParams->tiltThreshold  &&  depthDiff < moParams->depthThreshold  &&
                xLocationDiff < moParams->maxHorizontalSkip)
        {
            continue;
        } else
        {
            //Otherwise, make a new component
            newOda->detections[newOda->numDetections] = PTK_Alg_StereoAlgo_createMergedDetection(cntxt, curOda, startInd, i);
            newOda->numDetections++;

            startInd = i+1;
        }
    }

    //Add the last detection(s)
    newOda->detections[newOda->numDetections] = PTK_Alg_StereoAlgo_createMergedDetection(cntxt, curOda, startInd, curOda->numDetections-1);
    newOda->numDetections++;
}


void PTK_Alg_StereoAlgo_splitDetection(PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
                                       PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
                                       PTK_Alg_StereoAlgo_obsDetectionArray * newOda)
{
    PTK_Alg_StereoAlgo_obsProbParams  * copppParams = &cntxt->allParams.copppParams;
    PTK_Alg_StereoAlgo_splitObsParams * soParams    = &cntxt->allParams.soParams;
    
    PTK_Alg_StereoAlgo_lineModel    leftModel, rightModel;
    PTK_Alg_StereoAlgo_obsDetection leftDetection, rightDetection;

    int32_t i, x;
    int32_t inliersInARow, outliersInARow;
    int32_t xBeginD, xEndD;
    int8_t  splitPerformed;
    int8_t  beginningBreakDetected;
    int8_t  makeABreak;

    int32_t xL0, xL1, xR0, xR1;
    float   predicted, curDistance, curDisparity;


    float dsf = copppParams->dsFactor;

    // reset the number of merged detections to 0
    newOda->numDetections = 0;

    // For each component, go along and check how many outliers there are in a row.
    // If there are closer and above minComponentWidth, make a new component.
    for(i = 0; i < curOda->numDetections; i++)
    {
        inliersInARow  = 0;
        outliersInARow = 0;

        xBeginD = curOda->detections[i].xLocations[0] / dsf;
        xEndD   = curOda->detections[i].xLocations[1] / dsf;

        splitPerformed         = 0;
        beginningBreakDetected = 0;
        makeABreak = 0;

        for(x = xBeginD; x <= xEndD; x++)
        {
            xL0 = xL1 = xR0 = xR1 = 0;
            curDisparity = cntxt->closestDisparity[x];
            if(curDisparity < 0)
            {
                //don't count this as an inlier or an outlier, just keep checking
                continue;
            }

            predicted = curOda->detections[i].detectionLineModel.offset + curOda->detections[i].detectionLineModel.dx * x * dsf;
            curDistance = (float)fabs(predicted - curDisparity);

            if(curDistance > soParams->splitInlierThreshold)
            {
                outliersInARow++;
                inliersInARow = 0;
            } else
            {
                outliersInARow = 0;
                inliersInARow++;
            }

            /*
             *  To REVISIT :
             * Seems unnecessary
             */
            if(beginningBreakDetected)
            {
                if(inliersInARow > soParams->minSplitComponentWidth)
                {
                    //Split this component here, if this is a beginning break
                    xL0 = xBeginD;
                    xL1 = x-inliersInARow;
                    xR0 = x-inliersInARow+1;
                    xR1 = xEndD;

                    makeABreak = 1;
                }
            } else
            {
                if(outliersInARow > soParams->minSplitComponentWidth){
                    //Split this component here, if this is not a beginning break
                    //If the break is near the beginning, keep checking until we get a bunch of inliers
                    if((x-outliersInARow) < (xBeginD + soParams->minSplitComponentWidth))
                    {
                        beginningBreakDetected = 1;
                        continue;
                    } else
                    {
                        //Otherwise
                        xL0 = xBeginD;
                        xL1 = x-outliersInARow;
                        xR0 = x-outliersInARow+1;
                        xR1 = xEndD;
                    }

                    //TODO: set the values, then the below code will be the same for both +-beginningBreakDetected
                    makeABreak = 1;
                }
            }

            if(makeABreak){
                leftModel  = PTK_Alg_StereoAlgo_computeALineModel(cntxt, xL0, xL1, dsf, soParams->splitInlierThreshold, soParams->adaptiveTerminationPercentage);
                rightModel = PTK_Alg_StereoAlgo_computeALineModel(cntxt, xR0, xR1, dsf, soParams->splitInlierThreshold, soParams->adaptiveTerminationPercentage);


                //leftModel.dx /= dLine->downsampledBy;
                //leftModel.offset -= leftModel.dx*(xBeginD*dLine->downsampledBy);

                //rightModel.dx /= dLine->downsampledBy;
                //rightModel.offset -= rightModel.dx*(x*dLine->downsampledBy);
                leftDetection  = PTK_Alg_StereoAlgo_getNewDetection(cntxt, leftModel, xL0*dsf, xL1*dsf);
                rightDetection = PTK_Alg_StereoAlgo_getNewDetection(cntxt, rightModel, xR0*dsf, xR1*dsf);

                newOda->detections[newOda->numDetections]   = leftDetection;
                newOda->detections[newOda->numDetections+1] = rightDetection;

                newOda->numDetections += 2;

                splitPerformed = 1;
                break;
            }
        }

        if(!splitPerformed)
        {
            newOda->detections[newOda->numDetections] = curOda->detections[i];
            newOda->numDetections++;
        }
    }
}


void PTK_Alg_StereoAlgo_removeFarDetection(PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
                                           PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
                                           PTK_Alg_StereoAlgo_obsDetectionArray * newOda)
{
    PTK_Alg_StereoAlgo_obsDetection     curDetection;
    PTK_Alg_StereoAlgo_detectObsParams *doParams = &cntxt->allParams.doParams;

    int32_t io;
    int32_t newX0, newX1;
    float farIntersection;

    // reset the number of detections to 0
    newOda->numDetections = 0;

    //Remove components that are too far away
    for(io = 0; io < curOda->numDetections; io++)
    {
        curDetection = curOda->detections[io];

        farIntersection = (doParams->obsProbabilityMinDisparityToAccept - curDetection.detectionLineModel.offset) / curDetection.detectionLineModel.dx;

        //If the whole component is too far, don't use it at all
        if((farIntersection < curDetection.xLocations[0] && curDetection.detectionLineModel.dx < 0.0f) ||
           (farIntersection > curDetection.xLocations[1] && curDetection.detectionLineModel.dx > 0.0f))
        {
            continue;
        }

        // If part of the component too far, trim the component
        if(farIntersection >= curDetection.xLocations[0] && farIntersection <= curDetection.xLocations[1])
        {
            newX0 = curDetection.xLocations[0];
            newX1 = curDetection.xLocations[1];

            if(curDetection.detectionLineModel.dx > 0.0f)
            {
                newX0 = (int32_t) floor(farIntersection + 0.5f);
            } else
            {
                newX1 = (int32_t) floor(farIntersection + 0.5f);
            }

            // If the cutting didn't make the component too small, use it
            if((newX1 - newX0) >= doParams->minHorizontalLength)
            {
                newOda->detections[newOda->numDetections] =
                        PTK_Alg_StereoAlgo_getNewDetection(cntxt, curDetection.detectionLineModel, newX0, newX1);
            } else {
                newOda->numDetections--; //to counteract the ++ in a few lines
            }
        } else
        {
            //Otherwise, just copy it
            newOda->detections[newOda->numDetections] = curDetection;
        }

        newOda->numDetections++;
    }

}




PTK_Alg_StereoAlgo_obsDetection PTK_Alg_StereoAlgo_createMergedDetection(
        PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
        PTK_Alg_StereoAlgo_obsDetectionArray * curOda,
        int32_t startIdx, int32_t endIdx)
{
    PTK_Alg_StereoAlgo_obsDetection odNew;
    PTK_Alg_StereoAlgo_lineModel newModel;

    int32_t i;
    float nDx, nOffset, curAlpha;
    float totalArea = 0.0f;


    for(i = startIdx; i <= endIdx; i++)
    {
        totalArea += (float )curOda->detections[i].originalDetectedArea;
    }

    //Create the line model by weighted averaging, based on the original detected area
    nDx     = 0.0f;
    nOffset = 0.0f;
    for(i = startIdx; i<= endIdx; i++)
    {
        curAlpha = ((float )curOda->detections[i].originalDetectedArea) / totalArea;

        nDx     += curAlpha * curOda->detections[i].detectionLineModel.dx;
        nOffset += curAlpha * curOda->detections[i].detectionLineModel.offset;
    }

    

    newModel.dx = nDx;
    newModel.offset = nOffset;

    odNew = PTK_Alg_StereoAlgo_getNewDetection(cntxt, newModel, curOda->detections[startIdx].xLocations[0], curOda->detections[endIdx].xLocations[1]);


    return odNew;

}

PTK_Alg_StereoAlgo_obsDetection PTK_Alg_StereoAlgo_getNewDetection(PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
        PTK_Alg_StereoAlgo_lineModel model, int xBeginIdx, int xEndIdx)
{
    PTK_Alg_StereoAlgo_camParams         * camParams = &cntxt->allParams.camParams;
    PTK_Alg_StereoAlgo_closestObsParams  * fcdParams = &cntxt->allParams.fcdParams;

    PTK_Alg_StereoAlgo_obsDetection newDetection;
    int32_t k;

    newDetection.detectionLineModel   = model;
    newDetection.xLocations[0]        = xBeginIdx;
    newDetection.xLocations[1]        = xEndIdx;
    newDetection.disparities[0]       = newDetection.detectionLineModel.offset + newDetection.xLocations[0]*newDetection.detectionLineModel.dx;
    newDetection.disparities[1]       = newDetection.detectionLineModel.offset + newDetection.xLocations[1]*newDetection.detectionLineModel.dx;

    k = cntxt->algoMemData.gpModelIdx[newDetection.xLocations[0]];
    newDetection.bottomYs[0]          = PTK_Alg_StereoAlgo_findYforDisparity(&cntxt->gmParamsArr->gmParams[k], camParams, newDetection.xLocations[0], newDetection.disparities[0]);
    k = cntxt->algoMemData.gpModelIdx[newDetection.xLocations[1]];
    newDetection.bottomYs[1]          = PTK_Alg_StereoAlgo_findYforDisparity(&cntxt->gmParamsArr->gmParams[k], camParams, newDetection.xLocations[1], newDetection.disparities[1]);

    newDetection.depths[0]            = PTK_Alg_StereoAlgo_disparity2Depth(camParams, newDetection.disparities[0], newDetection.bottomYs[0]);
    newDetection.depths[1]            = PTK_Alg_StereoAlgo_disparity2Depth(camParams, newDetection.disparities[1], newDetection.bottomYs[1]);
    newDetection.isValid              = 1;
    newDetection.originalDetectedArea = 1; //TODO: make this the actual detected area
    newDetection.windowHeights[0]     = (int32_t)(camParams->f * fcdParams->statisticsBoxHeight/newDetection.depths[0]);
    newDetection.windowHeights[1]     = (int32_t)(camParams->f * fcdParams->statisticsBoxHeight/newDetection.depths[1]);


    if (newDetection.windowHeights[0] > newDetection.bottomYs[0])
    {
        newDetection.windowHeights[0] = newDetection.bottomYs[0];
    }

    if (newDetection.windowHeights[1] > newDetection.bottomYs[1])
    {
        newDetection.windowHeights[1] = newDetection.bottomYs[1];
    }

    return newDetection;
}


PTK_Alg_StereoAlgo_lineModel PTK_Alg_StereoAlgo_computeALineModel(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt,
        int32_t startInd, int32_t endInd, int8_t dsf, float inlierTh, float adaptiveTerminationPercentage)
{
    PTK_Alg_StereoAlgo_lineModel model;

    int32_t x, iter, iS;
    //int32_t numData = endInd - startInd + 1;

    int32_t numInliers, maxInliers = 0;
    int32_t validNum  = 0;


    int32_t whichSamples[2];
    int32_t requiredSamples = 2;
    int32_t maxIterations   = 100;

    int32_t estimatedRequiredIterations;
    float   minInlierPercentage;
    float   requiredSamplesF;



    float * A = cntxt->A;
    float * b = cntxt->b;

    for(x = startInd; x <= endInd; x++)
    {
        if(cntxt->closestDisparity[x] > 0)
        {
            cntxt->dataBuffer[0][validNum] = (float)(x * dsf);
            cntxt->dataBuffer[1][validNum] = cntxt->closestDisparity[x];
            validNum++;
        }
    }

    if (validNum < requiredSamples)
    {
        model.offset = 0.0;
        model.dx     = 0.0;

        return model;
    }

    // run RANSAC
    for (iter = 0; iter < maxIterations; iter++)
    {
        //Not the best way to get random elements
        for(iS = 0; iS < requiredSamples; iS++)
        {
            whichSamples[iS] = PTK_Alg_StereoAlgo_randRange(0, validNum-1);
        }

        // Ax = b
        A[0] = 1.0;
        A[1] = cntxt->dataBuffer[0][whichSamples[0]];
        A[2] = 1.0;
        A[3] = cntxt->dataBuffer[0][whichSamples[1]];
        b[0] = cntxt->dataBuffer[1][whichSamples[0]];
        b[1] = cntxt->dataBuffer[1][whichSamples[1]];

        model = PTK_Alg_StereoAlgo_fitModelOneLine(cntxt, A, b, 2, 0);

        numInliers = PTK_Alg_StereoAlgo_getInliersOneLine(cntxt->dataBuffer, cntxt->inlierIdx, model,  validNum, inlierTh);

        if (numInliers > maxInliers)
        {
            maxInliers = numInliers;
            memcpy(cntxt->bestInlierIdx, cntxt->inlierIdx, numInliers*sizeof(int16_t));
        }

        // update maxInterations
        if(adaptiveTerminationPercentage < 1.0f && maxInliers >= requiredSamples){
            minInlierPercentage = (float)maxInliers / (float)validNum;

            if(minInlierPercentage > .01f) //To prevent overflow
            {
                requiredSamplesF = (float) requiredSamples;
                estimatedRequiredIterations = (int32_t)floor(logf(1.0f-adaptiveTerminationPercentage) / logf(1.0f-powf(minInlierPercentage, requiredSamplesF))+0.5f);
                maxIterations = MMIN(maxIterations, estimatedRequiredIterations);

            }
        }
    }

    if (maxInliers < requiredSamples)
    {
        model.offset = 0.0;
        model.dx     = 0.0;

        return model;
    }

    // get the final model
    for (iS = 0; iS < maxInliers; iS++)
    {
        A[iS*2]     = 1.0;
        A[iS*2 + 1] = cntxt->dataBuffer[0][cntxt->inlierIdx[iS]];
        b[iS]       = cntxt->dataBuffer[1][cntxt->inlierIdx[iS]];
	}

    model = PTK_Alg_StereoAlgo_fitModelOneLine(cntxt, A, b, maxInliers, 1);


    return model;
}

void printMatrix(float *matrix, int32_t width, int32_t height)
{
    int32_t i, j;

    for(j = 0; j < height; j++)
    {
        for (i = 0 ; i <width; i++)
        {
            printf("%.2f ", matrix[j * width + i]);
        }
        printf("\n");
    }
    printf("\n");

}


PTK_Alg_StereoAlgo_lineModel PTK_Alg_StereoAlgo_fitModelOneLine(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt,
        float *A, float *b, int32_t numData, int8_t computeGram)
{
    PTK_Alg_StereoAlgo_lineModel model;

    int32_t permutedVal;
    float AtA[4];
    float Atb[2];

    if (computeGram)
    {
        PTK_Alg_StereoAlgo_transpose(A, 2, numData, cntxt->At);

        PTK_Alg_StereoAlgo_multiplyMatrix(cntxt->At, numData, 2, A, 2, numData, AtA, 2, 2);
        PTK_Alg_StereoAlgo_multiplyMatrix(cntxt->At, numData, 2, b, 1, numData, Atb, 1, 2);

        permutedVal = PTK_Alg_StereoAlgo_LUdecomposition(AtA, 2, 2, Atb, 1, 1);

        if (permutedVal != 0)
        {
            model.offset = Atb[0];
            model.dx     = Atb[1];
        } else
        {
            model.offset = 0.0;
            model.dx     = 0.0;
        }

    } else
    {
        permutedVal = PTK_Alg_StereoAlgo_LUdecomposition(A, 2, numData, b, 1, 1);

        if (permutedVal != 0)
        {
            model.offset = b[0];
            model.dx     = b[1];
        } else
        {
            model.offset = 0.0;
            model.dx     = 0.0;
        }
    }


    return model;
}


int32_t PTK_Alg_StereoAlgo_getInliersOneLine(float *dataBuffer[2], int16_t *inlierIdx,
        PTK_Alg_StereoAlgo_lineModel model, int32_t numData, float inlierTh)
{
    int32_t i, numInliers = 0;
    float prediction, difference;

    for (i = 0; i < numData; i++)
    {
        prediction = dataBuffer[0][i] * model.dx + model.offset;
        difference = (prediction - dataBuffer[1][i]);
        difference = difference * difference;

        if (difference < inlierTh)
        {
            inlierIdx[numInliers] = i;
            numInliers++;
        }
    }

    return numInliers;
}


