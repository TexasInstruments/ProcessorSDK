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


#include <perception/algos/stereo/object_detection/obstacle_from_disparity.h>
#include <perception/algos/stereo/common_detection/stereo_algo.h>

static int32_t compare(const void * a, const void * b)
{
    return ( *(float*)a > *(float*)b );
}

/**
 * \brief Create a new deteted obstacle object
 * 
 * \param [in] cntxt                  slope of V-disparity ground model
 * \param [in] componentBegin         starting horizontal position of obstacle
 * \param [in] componentEnd           last horizontal position of obstacle
 * \param [in] windowHeightRatioRange min/max ratio of window heights at componentBegin and componentEnd
 * \param [in] minHorizontalLength    minimum horizontal length of obstacle
 * 
 * \return 
 *        detcted obstacle object
 *
 * \ingroup group_ptk_algos_stereo_od
 */
static void PTK_Alg_StereoAlgo_newDetection(
        PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
        const int32_t                       componentBegin,
        const int32_t                       componentEnd,
        const float                         windowHeightRatioRange[2],
        const int32_t                       minHorizontalLength,
        PTK_Alg_StereoAlgo_obsDetection    *outObj)
{
    PTK_Alg_StereoAlgo_obsProbParams  * copppParams = &cntxt->allParams.copppParams;
    PTK_Alg_StereoAlgo_camParams      * camParams   = &cntxt->allParams.camParams;

    int8_t  dsf = copppParams->dsFactor;

    int32_t componentBeginScaled;
    int32_t componentEndScaled;
    int32_t meanX;
    int32_t windowBottom;
    int32_t originalDetectedArea;

    float   windowHeightRatio;
    float   meanDisparity = 0.0;
    int32_t meanBottomLine = 0;
    int32_t meanClosestPriorHeights = 0;
    int32_t meanClosestComputedHeights = 0;
    int32_t i, k, cd = 0, cb = 0, cph = 0, cch = 0;

    outObj->isValid = 0;

    if((componentEnd-componentBegin+1) * dsf < minHorizontalLength)
    {
        return;
    }

    for(i = componentBegin; i <= componentEnd; i++)
    {
        float curDisparity    = cntxt->closestDisparity[i];
        int curBottom         = cntxt->algoMemData.obtacleBottomLine[i];
        int curPriorHeight    = cntxt->closestHeightPrior[i];
        int curComputedHeight = cntxt->closestHeightComputed[i];

        //TODO: It may or may not be necessary to check each of these three independently.
        //      One option is that bottomLineMean and closestHeightsMean maybe should be calculated based on meanDisparity
        //      Alternately, they may all be -1 at the same locations (but I think not)
        if(curDisparity > 0)
        {
            meanDisparity += curDisparity;
            cd++;
        }
        if(curBottom > 0)
        {
            meanBottomLine += curBottom;
            cb++;
        }
        if(curPriorHeight > 0)
        {
            meanClosestPriorHeights += curPriorHeight;
            cph++;
        }
        if(curComputedHeight > 0)
        {
            meanClosestComputedHeights += curComputedHeight;
            cch++;
        }
    }

    if(cd > 0 && cb > 0 && cph > 0 && cch > 0)
    {
        meanDisparity              /= cd;
        meanBottomLine             /= cb;
        meanClosestPriorHeights    /= cph;
        meanClosestComputedHeights /= cch;
    }
    else
    {
        return;
    }

    windowHeightRatio = (float) meanClosestComputedHeights / (float)meanClosestPriorHeights;
    if(windowHeightRatio < windowHeightRatioRange[0] || windowHeightRatio > windowHeightRatioRange[1])
    {
        return;
    }

    componentBeginScaled = componentBegin * dsf;
    componentEndScaled   = componentEnd * dsf;

    //#where should the bottom of the window be, based on the ground model ?
    meanX = (componentBeginScaled + componentEndScaled) / 2;
    k = cntxt->algoMemData.gpModelIdx[meanX];
    windowBottom = PTK_Alg_StereoAlgo_findYforDisparity(&cntxt->gmParamsArr->gmParams[k], camParams, meanX, meanDisparity);
    originalDetectedArea = (componentEnd - componentBegin) * meanClosestPriorHeights;

    outObj->isValid              = 1;
    outObj->bottomYs[0]          = windowBottom;
    outObj->bottomYs[1]          = windowBottom;
    outObj->depths[0]            = PTK_Alg_StereoAlgo_disparity2Depth(camParams, meanDisparity, outObj->bottomYs[0]);
    outObj->depths[1]            = outObj->depths[0];
    outObj->disparities[0]       = meanDisparity;
    outObj->disparities[1]       = meanDisparity;
    outObj->originalDetectedArea = originalDetectedArea;
    outObj->windowHeights[0]     = meanClosestComputedHeights * dsf;
    outObj->windowHeights[1]     = outObj->windowHeights[0];
    outObj->xLocations[0]        = componentBeginScaled;
    outObj->xLocations[1]        = componentEndScaled;
    outObj->detectionLineModel   = PTK_Alg_StereoAlgo_getlineModel(meanDisparity, 0);

    return;
}

int32_t PTK_Alg_StereoAlgo_findObstacleFromDisparity(PTK_Alg_StereoAlgo_ObjectDetectObj *cntxt,
                                                  PTK_Alg_StereoAlgo_obsDetectionArray * oda)
{
    int32_t horizon;
    int32_t drivableYPos;

    horizon = PTK_Alg_StereoAlgo_computeObstacleProbalilityPerPixel(
        &cntxt->allParams.copppParams, &cntxt->allParams.camParams, cntxt->gmParamsArr, &cntxt->algoMemData);

    PTK_Alg_StereoAlgo_findBottomOfObstacle(
        &cntxt->allParams.copppParams, &cntxt->allParams.fbooParams, &cntxt->algoMemData);

    PTK_Alg_StereoAlgo_findClosestDisparity(cntxt);

    PTK_Alg_StereoAlgo_detectObstacle(cntxt, oda);

    // For freespace detection
    PTK_Alg_StereoAlgo_searchFreeSpace(cntxt, horizon);

    drivableYPos = PTK_Alg_StereoAlgo_searchFreeDriveDistance(cntxt);

    return drivableYPos;
}

void  PTK_Alg_StereoAlgo_findClosestDisparity(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt)
{
    PTK_Alg_StereoAlgo_obsProbParams    * copppParams = &cntxt->allParams.copppParams;
    PTK_Alg_StereoAlgo_camParams        * camParams   = &cntxt->allParams.camParams;
    PTK_Alg_StereoAlgo_closestObsParams * fcdParams   = &cntxt->allParams.fcdParams;

    int32_t * obtacleBottomLine = cntxt->algoMemData.obtacleBottomLine;
    float   * dsDisparity       = cntxt->algoMemData.dsDisparity;

    int8_t  dsf      = copppParams->dsFactor;
    int16_t dsWidth  = copppParams->dsWidth;

    int32_t x, y, yy, k;
    int32_t ci, ind;
    int32_t limitsY[2];
    int32_t numElements;

    int32_t heightHere;
    float   expectedDisparityHere;
    float   expectedDepthHere;

    float   curDisp;
    float   closestDisparity;
    float   computedDepth;


    // init closest disparity
    for(x = 0; x < dsWidth; x++)
    {
        cntxt->closestDisparity[x]      = -1;
        cntxt->closestHeightPrior[x]    = -1;
        cntxt->closestHeightComputed[x] = -1;
    }


    for(x = 0; x < dsWidth; x++)
    {
        y = obtacleBottomLine[x];

        //If the current bottom line value is invalid
        if(y < 0)
        {
            continue;
        }

        k = cntxt->algoMemData.gpModelIdx[x*dsf];

        expectedDisparityHere = PTK_Alg_StereoAlgo_getDisparityFromModel(&cntxt->gmParamsArr->gmParams[k], (x+0.5)*dsf, (y+0.5)*dsf);
        expectedDepthHere     = PTK_Alg_StereoAlgo_disparity2Depth(camParams, expectedDisparityHere, (y+0.5)*dsf);

        heightHere = MMAX(fcdParams->minSearchBoxSize,
                (int32_t)(camParams->f * fcdParams->statisticsBoxHeight / (expectedDepthHere * dsf)));

        if(heightHere > 0)
        {
            ci = 0;

            limitsY[0] = MMAX(0 , y - heightHere);
            limitsY[1] = y;

            //    values = disparityImageDownsampled[limits[0]:limits[1],x]
            //    values = values[nonzero(values > 0)]
            //    values.sort()
            numElements = limitsY[1] - limitsY[0];
            for(yy = 0; yy < numElements; yy++)
            {
                cntxt->sortedDisparity[yy] = 0;
            }

            for(yy = limitsY[0]; yy < limitsY[1]; yy++)
            {
                curDisp = dsDisparity[yy * dsWidth + x];
                if(curDisp > 0)
                {
                    cntxt->sortedDisparity[ci] = dsDisparity[yy * dsWidth + x];
                    ci++;
                }
            }

            if(ci > 0)
            {
                // TO REVISIt: sort array in ascending order
                qsort(cntxt->sortedDisparity, numElements, sizeof(float), compare); 

                //        nearestVal = values[int(len(values)*(1-nearestPercent))] #the absolute maximum may be erroneous
                //        closestDisparity[x] = nearestVal
                //        closestHeights[x] = curHeight
                //int ind = (int)(heightHere*(1.0-statisticsNearestPercent));

                ind = (int32_t)(ci * (1.0 - fcdParams->statisticsNearestPercent));
                closestDisparity = cntxt->sortedDisparity[MMAX(0, ind)];

                if(closestDisparity > 0)
                {
                    cntxt->closestDisparity[x]   = closestDisparity;
                    cntxt->closestHeightPrior[x] = heightHere;

                    computedDepth = PTK_Alg_StereoAlgo_disparity2Depth(camParams, closestDisparity, (y+0.5)*dsf);
                    cntxt->closestHeightComputed[x] = (int)(camParams->f * fcdParams->statisticsBoxHeight/(computedDepth * dsf));
                }
            }

        }   //if(heightHere > 0)
    }
}

void  PTK_Alg_StereoAlgo_detectObstacle(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt, 
                                        PTK_Alg_StereoAlgo_obsDetectionArray * oda)
{
    PTK_Alg_StereoAlgo_obsProbParams   * copppParams = &cntxt->allParams.copppParams;
    PTK_Alg_StereoAlgo_detectObsParams * doParams    = &cntxt->allParams.doParams;


    int32_t x;
    int32_t dsWidth                   = copppParams->dsWidth;

    uint8_t onComponent               = 0;
    int32_t componentBegin            = -1;
    int32_t horizontalSkipped         = 0;
    int32_t numCloseEnoughDisparities = 0;

    float   MIN_DISP                  = -10000.0;
    float   lastDisparity             = MIN_DISP;
    float   curClosestDisparity;


    // initialize the number of detections
    oda->numDetections = 0;

    for(x = 0; x < dsWidth; x++)
    {
        curClosestDisparity = cntxt->closestDisparity[x];
        if(onComponent)
        {
            if(curClosestDisparity < 0)
            {
                horizontalSkipped++;
                if(horizontalSkipped < doParams->maxHorizontalToSkip)
                {
                    continue;
                }

                //Need at least 2 points to draw a line
                if(numCloseEnoughDisparities > 2)
                {
                    PTK_Alg_StereoAlgo_newDetection(cntxt,
                                                    componentBegin,
                                                    x-1-horizontalSkipped,
                                                    doParams->windowHeightRatioRange,
                                                    doParams->minHorizontalLength,
                                                    &oda->detections[oda->numDetections]);

                    if(oda->detections[oda->numDetections].isValid)
                    {
                        oda->numDetections++;
                    }
                }

                onComponent               = 0;
                componentBegin            = -1;
                lastDisparity             = MIN_DISP;
                horizontalSkipped         = 0;
                numCloseEnoughDisparities = 0;

            }
            else if(fabs(curClosestDisparity-lastDisparity) > doParams->maxDisparityDifference)
            {
                //Need at least 2 points to draw a line
                if(numCloseEnoughDisparities > 2)
                {
                    PTK_Alg_StereoAlgo_newDetection(cntxt, 
                                                    componentBegin,
                                                    x-1-horizontalSkipped,
                                                    doParams->windowHeightRatioRange,
                                                    doParams->minHorizontalLength,
                                                    &oda->detections[oda->numDetections]);

                    if(oda->detections[oda->numDetections].isValid)
                    {
                        oda->numDetections++;
                    }
                }

                onComponent               = 1;
                componentBegin            = x;
                lastDisparity             = curClosestDisparity;
                horizontalSkipped         = 0;
                numCloseEnoughDisparities = 0;

                if(curClosestDisparity >= doParams->obsProbabilityMinDisparityToAccept)
                {
                    numCloseEnoughDisparities++;
                }
            }
            else
            {
                // Otherwise, do nothing
                lastDisparity     = curClosestDisparity;
                horizontalSkipped = 0;

                if(curClosestDisparity >= doParams->obsProbabilityMinDisparityToAccept)
                {
                    numCloseEnoughDisparities++;
                }
            }
        }
        else
        {
            if(curClosestDisparity > 0){
                onComponent       = 1;
                componentBegin    = x;
                lastDisparity     = curClosestDisparity;
                horizontalSkipped = 0;

                if(curClosestDisparity >= doParams->obsProbabilityMinDisparityToAccept)
                {
                    numCloseEnoughDisparities++;
                }
            }
        }
    }

    if(onComponent)
    {
        if(numCloseEnoughDisparities > 2)
        {
            //Need at least 2 points to draw a line
            PTK_Alg_StereoAlgo_newDetection(cntxt, 
                                            componentBegin,
                                            dsWidth-1-horizontalSkipped,
                                            doParams->windowHeightRatioRange,
                                            doParams->minHorizontalLength,
                                            &oda->detections[oda->numDetections]);

            if(oda->detections[oda->numDetections].isValid)
            {
                oda->numDetections++;
            }
        }
    }

}

PTK_Alg_StereoAlgo_lineModel PTK_Alg_StereoAlgo_getlineModel(float offset, float dx)
{
    PTK_Alg_StereoAlgo_lineModel model;

    model.offset = offset;
    model.dx     = dx;

    return model;
}


int32_t  PTK_Alg_StereoAlgo_searchFreeDriveDistance(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt)
{
    PTK_Alg_StereoAlgo_roadParams        * roadParams  = &cntxt->allParams.roadParams;
    PTK_Alg_StereoAlgo_obsProbParams     * copppParams = &cntxt->allParams.copppParams;
    PTK_Alg_StereoAlgo_camParams         * camParams   = &cntxt->allParams.camParams;

    int32_t j, i, modelIdx, roadCenterBaseDx;
    int32_t dsf, dsWidth, dsHeight;
    int32_t segmentWidth, segmentFreeSpaceBoundary;

    int32_t minDrivableYPos;

    float   expectedDisparity;
    float   expectedDepth;
    float   egoWidth;
    float   halfEgoWidth;


    dsf      = copppParams->dsFactor;
    dsWidth  = copppParams->dsWidth;
    dsHeight = copppParams->dsHeight;

    roadCenterBaseDx = roadParams->roadCenterBase[0] / dsf;
    modelIdx         = cntxt->algoMemData.gpModelIdx[roadCenterBaseDx*dsf];


    minDrivableYPos = -1.0;
    for (j = dsHeight-1; j >= 0; j--)
    {
        // init freeDriveSpace
        cntxt->freeDriveSpace[j*2]     = -1;
        cntxt->freeDriveSpace[j*2 + 1] = -1;

        expectedDisparity = PTK_Alg_StereoAlgo_getDisparityFromModel(&cntxt->gmParamsArr->gmParams[modelIdx], (roadCenterBaseDx + 0.5f)*dsf, (j + 0.5f)*dsf);
        expectedDepth     = PTK_Alg_StereoAlgo_disparity2Depth(camParams, expectedDisparity, (j+0.5f)*dsf);

        egoWidth = floor(camParams->f * roadParams->egoSize / (expectedDepth * dsf));
        halfEgoWidth = MMIN(roadCenterBaseDx - 1, floor((egoWidth + 1) / 2));

        if (halfEgoWidth > 0)
        {
            cntxt->freeDriveSpace[j*2]     = MMAX(0, roadCenterBaseDx - halfEgoWidth);
            cntxt->freeDriveSpace[j*2 + 1] = MMIN(dsWidth - 1, roadCenterBaseDx + halfEgoWidth);

            segmentWidth =  cntxt->freeDriveSpace[j*2 + 1] - cntxt->freeDriveSpace[2*j] + 1;
            segmentFreeSpaceBoundary = 0.0f;
            for (i =  cntxt->freeDriveSpace[j*2]; i <= cntxt->freeDriveSpace[j*2 + 1]; i++)
            {
                if ( cntxt->algoMemData.obtacleBottomLine[i] >= j)
                {
                    segmentFreeSpaceBoundary += cntxt->algoMemData.obtacleBottomLine[i];
                }
            }

            if (segmentFreeSpaceBoundary > (float)segmentWidth * 0.1f)
            {
                break;
            } else
            {
                minDrivableYPos = j;
            }
        }
    }

    return minDrivableYPos;

}


void  PTK_Alg_StereoAlgo_searchFreeSpace(PTK_Alg_StereoAlgo_ObjectDetectObj * cntxt, int32_t horizon)
{
    PTK_Alg_StereoAlgo_obsProbParams     * copppParams = &cntxt->allParams.copppParams;

    int32_t i, x;
    int32_t dsWidth  = copppParams->dsWidth;

    int32_t validLeftBottom      = -1;
    int32_t validRightBottom     = -1;
    int32_t validRightBottomXPos = -1;

    int32_t * obtacleBottomLine = cntxt->algoMemData.obtacleBottomLine;

    for (x = 0; x < dsWidth-1; x++)
    {
        if (obtacleBottomLine[x] == -1)
        {
            if (validLeftBottom != -1)
            {
                if (validRightBottomXPos > x)
                {
                    obtacleBottomLine[x] = MMIN(validLeftBottom, validRightBottom);
                } else
                {
                    for (i = x + 1; i < dsWidth-1; i++)
                    {
                        if (obtacleBottomLine[i] != -1)
                        {
                            validRightBottomXPos = i;
                            validRightBottom = obtacleBottomLine[i];

                            break;
                        }
                    }

                    if (validRightBottomXPos > x)
                    {
                        obtacleBottomLine[x] = MMIN(validLeftBottom, validRightBottom);
                    } else
                    {
                        obtacleBottomLine[x] = horizon;
                        validRightBottom     = horizon;
                    }
                }
            } else
            {
                obtacleBottomLine[x] = horizon;
                validLeftBottom = obtacleBottomLine[x];
            }

        } else
        {
            validLeftBottom = obtacleBottomLine[x];
            //obtacleBottomLine[x] = obtacleBottomLine[x];
        }
    }

}



