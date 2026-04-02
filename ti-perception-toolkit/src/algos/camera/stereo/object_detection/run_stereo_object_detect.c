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
#include <math.h>
#include <string.h>
#include <stdio.h>

#include <perception/algos/stereo/ground_estimation/vdisparity_ground_model.h>
#include <perception/algos/stereo/object_detection/run_stereo_object_detect.h>
#include <perception/algos/stereo/object_detection/obstacle_from_disparity.h>
#include <perception/algos/stereo/object_detection/highlight_obstacle.h>
#include <perception/algos/stereo/common_detection/stereo_algo.h>


int32_t PTK_Alg_StereoAlgo_runObjectDetect(PTK_Alg_StereoAlgo_ObjectDetectObj         * cntxt,
                                           PTK_Alg_StereoAlgo_groundModelParams_Array * gmParamsArr,
                                           PTK_Alg_StereoAlgo_obsBox                  * obsBox,
                                           uint32_t                                   * numDetetions,
                                           int32_t                                    * freeSpaceBoundary,
                                           PTK_Alg_StereoAlgo_driveSpace              * drivableSpace,
                                           uint8_t                                    * inputImageBuffer,
                                           int16_t                                    * disparityBuffer)
{
    /* 
    PTK_Alg_StereoAlgo_dispParams        * dispParams = &cntxt->allParams.dispParams;
    PTK_Alg_StereoAlgo_camParams         * camParams  = &cntxt->allParams.camParams;
    */
    PTK_Alg_StereoAlgo_camParams         * camParams   = &cntxt->allParams.camParams;
    PTK_Alg_StereoAlgo_roadParams        * roadParams  = &cntxt->allParams.roadParams;
    PTK_Alg_StereoAlgo_obsProbParams     * copppParams = &cntxt->allParams.copppParams;

    PTK_Alg_StereoAlgo_obsDetectionArray * curOda;
    PTK_Alg_StereoAlgo_obsDetectionArray * newOda;
    PTK_Alg_StereoAlgo_obsDetectionArray * tmpOda;

    int32_t i, numDifference;
    int32_t mergeIterations = 20;

    int32_t dsf, dsWidth, dsHeight;
    int32_t drivableYPos;
    int32_t modelIdx, roadCenterBaseDx;
    float   expectedDisparity;
    float   expectedDepth;

    cntxt->inputImage     = inputImageBuffer;
    cntxt->disparity      = disparityBuffer;

    dsf              = copppParams->dsFactor;
    dsWidth          = copppParams->dsWidth;
    dsHeight         = copppParams->dsHeight;

    roadCenterBaseDx = roadParams->roadCenterBase[0] / dsf;
    modelIdx         = cntxt->algoMemData.gpModelIdx[roadCenterBaseDx*dsf];

    cntxt->gmParamsArr = gmParamsArr;

    if (cntxt->gmParamsArr->gmParams[0].validGM) // to revisit
    {
        // run object detection
        PTK_Alg_StereoAlgo_downsampleDisparity(&cntxt->allParams.odConfig, 
                &cntxt->allParams.copppParams, cntxt->disparity, cntxt->algoMemData.dsDisparity);

        // update cntxt->oda1
        drivableYPos = PTK_Alg_StereoAlgo_findObstacleFromDisparity(cntxt, cntxt->oda1);

        // update cntxt->oda2
        PTK_Alg_StereoAlgo_computePerspectiveForDetection(cntxt, cntxt->oda1, cntxt->oda2);

        curOda = cntxt->oda2;
        newOda = cntxt->oda1;

        for (i = 0; i < mergeIterations; i++)
        {
            PTK_Alg_StereoAlgo_mergeDetection(cntxt, curOda, newOda);

            numDifference = newOda->numDetections - curOda->numDetections;

            // swap pointers
            tmpOda = curOda;
            curOda = newOda;
            newOda = tmpOda;

            if (numDifference == 0)
            {
                break;
            }
        }

        for (i = 0; i < mergeIterations; i++)
        {
            PTK_Alg_StereoAlgo_splitDetection(cntxt, curOda, newOda);

            numDifference = newOda->numDetections - curOda->numDetections;

            // swap pointers
            tmpOda = curOda;
            curOda = newOda;
            newOda = tmpOda;

            if (numDifference == 0)
            {
                break;
            }
        }

        PTK_Alg_StereoAlgo_removeFarDetection(cntxt, curOda, newOda);

        if (numDetetions != NULL)
        {
            *numDetetions = newOda->numDetections;

            // p3  p4
            // p1  p2
            for (i = 0; i < newOda->numDetections; i++)
            {
                obsBox[i].p1x   = newOda->detections[i].xLocations[0];
                obsBox[i].p1y   = newOda->detections[i].bottomYs[0];
                obsBox[i].p3x   = newOda->detections[i].xLocations[0];
                obsBox[i].p3y   = newOda->detections[i].bottomYs[0] - newOda->detections[i].windowHeights[0];

                obsBox[i].p2x   = newOda->detections[i].xLocations[1];
                obsBox[i].p2y   = newOda->detections[i].bottomYs[1];
                obsBox[i].p4x   = newOda->detections[i].xLocations[1];
                obsBox[i].p4y   = newOda->detections[i].bottomYs[1] - newOda->detections[i].windowHeights[1];

                obsBox[i].depth =  PTK_Alg_StereoAlgo_disparity2Depth(&cntxt->allParams.camParams,
                                       (newOda->detections[i].disparities[0] + newOda->detections[i].disparities[1])/2,
                                       (newOda->detections[i].bottomYs[0] + newOda->detections[i].bottomYs[1])/2);
            }
        }


        // for freespace detection
        /*
        PTK_Alg_StereoAlgo_searchFreeSpace(cntxt, horizon);
        drivableYPos = PTK_Alg_StereoAlgo_searchFreeDriveDistance(cntxt);
        */

        /* update output */
        expectedDisparity = PTK_Alg_StereoAlgo_getDisparityFromModel(&cntxt->gmParamsArr->gmParams[modelIdx], (roadCenterBaseDx + 0.5f)*dsf, (drivableYPos + 0.5f)*dsf);
        expectedDepth     = PTK_Alg_StereoAlgo_disparity2Depth(camParams, expectedDisparity, (drivableYPos + 0.5f)*dsf);

        // P3 P4
        // P1 P2
        drivableSpace->drivableDepth = expectedDepth;
        drivableSpace->p1x = cntxt->freeDriveSpace[(dsHeight - 1) * 2] * dsf;
        drivableSpace->p1y = (dsHeight - 1) * dsf;
        drivableSpace->p2x = cntxt->freeDriveSpace[(dsHeight - 1) * 2 + 1] * dsf;
        drivableSpace->p2y = (dsHeight - 1) * dsf;
        drivableSpace->p3x = cntxt->freeDriveSpace[drivableYPos * 2] * dsf;
        drivableSpace->p3y = drivableYPos * dsf;
        drivableSpace->p4x = cntxt->freeDriveSpace[drivableYPos * 2 + 1] * dsf;
        drivableSpace->p4y = drivableYPos * dsf;

        for (i = 0; i < dsWidth; i++)
        {
            freeSpaceBoundary[i] = cntxt->algoMemData.obtacleBottomLine[i];
        }
    }

    return dsWidth;
}


