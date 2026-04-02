/*
*
* Copyright (c) 2020 Texas Instruments Incorporated
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

#include <perception/algos/stereo/ogmap/create_pc.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))
#define MAX(a,b) ((a > b) ? (a) : (b))

// 0: Road, 1: Sidewalk
// 11: pedestrian, 12: person on bike, 13: car, 14: truck, 15: bike
int8_t PTK_Alg_StereoOG_classToPC(uint8_t classId)
{
    if (classId == 0 || classId == 1 || classId == 11 || classId == 13)
    {
        return 1;
    }

    return 0;
}

int8_t PTK_Alg_StereoOG_classToDetect(uint8_t classId)
{
    if (classId == 11 || classId == 13)
    {
        return 1;
    }

    return 0;
}

void PTK_Alg_StereoOG_CreatePCSetParams(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                        PTK_Alg_StereoOG_CreatePCObj       * cntxt)
{
    /* initialize config parameters */
    cntxt->params.cfgParams.width        = params->cfgParams.width;
    cntxt->params.cfgParams.height       = params->cfgParams.height;
    cntxt->params.cfgParams.tensorWidth  = params->cfgParams.tensorWidth;
    cntxt->params.cfgParams.tensorHeight = params->cfgParams.tensorHeight;
    cntxt->params.cfgParams.dsFactor     = params->cfgParams.dsFactor;
    cntxt->params.cfgParams.confidenceTh = params->cfgParams.confidenceTh;
    cntxt->params.cfgParams.dsWidth      = cntxt->params.cfgParams.width / cntxt->params.cfgParams.dsFactor;
    cntxt->params.cfgParams.dsHeight     = cntxt->params.cfgParams.height / cntxt->params.cfgParams.dsFactor;

    /* initialize camera parameters */
    cntxt->params.camParams.camHeight    = params->camParams.camHeight;
    cntxt->params.camParams.camRoll      = params->camParams.camRoll;
    cntxt->params.camParams.camPitch     = params->camParams.camPitch;
    cntxt->params.camParams.camYaw       = params->camParams.camYaw;
    cntxt->params.camParams.sinPitch     = params->camParams.sinPitch;
    cntxt->params.camParams.cosPitch     = params->camParams.cosPitch;
    cntxt->params.camParams.baseline     = params->camParams.baseline;
    cntxt->params.camParams.dcx          = params->camParams.dcx;
    cntxt->params.camParams.dcy          = params->camParams.dcy;
    cntxt->params.camParams.f            = params->camParams.f;

}


int32_t PTK_Alg_StereoOG_CreatePCConfig(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                        PTK_Api_MemoryReq                  * memReq)
{
    uint32_t memSize;

    PTK_Alg_StereoOG_CreatePCObj cntxt;
    cntxt.params = *params;

    memReq->numBlks = 2;

    /* Block 0 is for the internal context. */
    memReq->blks[0].size    = sizeof(PTK_Alg_StereoOG_CreatePCObj);
    memReq->blks[0].memType = PTK_Api_MemoryType_LVL4;

    /* Block1 is for the memory needed by the algorithm */
    memSize = PTK_Alg_StereoOG_CreatePCGetMem(&cntxt, 0);
    memReq->blks[1].size    = memSize;
    memReq->blks[1].memType = PTK_Api_MemoryType_LVL4;

    return PTK_ALG_RET_SUCCESS;
}


PTK_AlgHandle PTK_Alg_StereoOG_CreatePCInit(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                            const PTK_Api_MemoryRsp            * memRsp)
{
    PTK_Alg_StereoOG_CreatePCObj  * cntxt;

    /* Validate the input parameters. */
    if (!params               ||
        !memRsp               ||
        (memRsp->numBlks < 2) ||
        !memRsp->blks[0].mem  ||
        !memRsp->blks[1].mem)
    {
        return NULL;
    }

    /* Initialize the context. */
    cntxt = (PTK_Alg_StereoOG_CreatePCObj *)memRsp->blks[0].mem;
    cntxt->params = *params;

    PTK_Alg_StereoOG_CreatePCGetMem(cntxt, memRsp->blks[1].mem);

    return cntxt;
}


int32_t PTK_Alg_StereoOG_CreatePCGetMem(PTK_Alg_StereoOG_CreatePCObj * cntxt,  uint8_t *memPtr)
{
    int32_t memSize = 0;
    uint8_t *pMem    = (uint8_t *) memPtr;

    PTK_Alg_StereoOG_CreatePCConfigParams * cfg = &cntxt->params.cfgParams;

    if (pMem)
    {
        cntxt->dsDisparity = (float *)(pMem + memSize);
    }
    memSize += sizeof(float)*(cfg->dsWidth * cfg->dsHeight);
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->dsSSMap = (uint8_t *)(pMem + memSize);
    }
    memSize += sizeof(uint8_t)*(cfg->dsWidth * cfg->dsHeight);
    memSize = align(memSize, 8);

    return memSize;
}

/**
 *  Root function to creat point cloud 
 */
void PTK_Alg_StereoOG_createPC(PTK_Alg_StereoOG_CreatePCObj * cntxt,
                               int16_t                      * disparity,
                               uint8_t                      * ssmap,
                               PTK_PointCloud               * pc)
{
    float   * dsDisparity  = cntxt->dsDisparity;
    uint8_t * dsSSMap      = cntxt->dsSSMap;

    PTK_Alg_StereoOG_downsampleDisparity(&cntxt->params.cfgParams, disparity, dsDisparity);

    PTK_Alg_StereoOG_downsampleSSMap(&cntxt->params.cfgParams, ssmap, dsSSMap);

    PTK_Alg_StereoOG_triangulate(&cntxt->params, cntxt->dsDisparity, cntxt->dsSSMap, pc);

    return;
}

/**
 *  Downsample dispariyt map 
 */
void PTK_Alg_StereoOG_downsampleDisparity(PTK_Alg_StereoOG_CreatePCConfigParams * config,
                                          int16_t                               * disparity,
                                          float                                 * dsDisparity)
{
    int16_t   dsFactor     = config->dsFactor;
    int16_t   width        = config->width;
    int16_t   height       = config->height;
    int16_t   dsWidth      = config->dsWidth;
    int16_t   dsHeight     = config->dsHeight;
    uint8_t   confidenceTh = config->confidenceTh;

    uint8_t   confidence;
    float     scale = 1.0 / (1 << NUM_FRAC_BITS);

    if (dsFactor <= 1)
    {
        int16_t i;

        for (i = 0; i < width * height; i++)
        {
            confidence = disparity[i] & 0x07;

            if (confidence < confidenceTh)
            {
                dsDisparity[i] = 0;
            } else
            {
                dsDisparity[i] = ((disparity[i] & 0x7FFF) >> 3) * scale;
            }
        }
    } else
    {
        int16_t x, y, xx, yy, cx, cy, ci;
        int16_t minNonZero;

        int16_t limitsX[2];
        int16_t limitsY[2];

        float curImVal, curSum = 0.0;

        limitsX[0] = width  - dsWidth * dsFactor;
        limitsX[1] = width  - dsFactor;
        limitsY[0] = height - dsHeight * dsFactor;
        limitsY[1] = height - dsFactor;

        minNonZero = 1;

        cy = 0;
        for(y = limitsY[0]; y <= limitsY[1]; y += dsFactor)
        {
            cx = 0;
            for(x = limitsX[0]; x <= limitsX[1]; x += dsFactor)
            {
                curSum = 0.0;
                ci = 0;

                for(yy = y; yy < (y + dsFactor); yy++)
                {
                    for(xx = x; xx < (x+dsFactor); xx++)
                    {
                        confidence = disparity[yy * width + xx] & 0x07;
                        if (confidence >= confidenceTh)
                        {
                            curImVal = ((disparity[yy * width + xx] & 0x7FFF) >> 3);
                            curSum  += curImVal;
                            ci++;
                        }
                    }
                }

                if (ci >= minNonZero){
                    curSum /= (ci << NUM_FRAC_BITS);
                } else {
                    curSum = 0;
                }

                dsDisparity[cy * dsWidth + cx] = curSum;
                cx++;
            }
            cy++;
        }
    }
}


/**
 *  Downsample semantic segmentation map to the same size of downsampled disparity map
 */
void    PTK_Alg_StereoOG_downsampleSSMap(PTK_Alg_StereoOG_CreatePCConfigParams  * config,
                                         uint8_t                                * ssmap,
                                         uint8_t                                * dsSSMap)
{
    int16_t   width        = config->tensorWidth;
    int16_t   height       = config->tensorHeight;
    int16_t   dsWidth      = config->dsWidth;
    int16_t   dsHeight     = config->dsHeight;

    float     hScale       = width *1.0 / dsWidth;
    float     vScale       = height*1.0 / dsHeight;

    int16_t   i, j, x, y;

    for (j = 0; j < dsHeight; j++)
    {
        for (i = 0; i < dsWidth; i++)
        {
            y = floor(j*vScale + 0.5);
            x = floor(i*hScale + 0.5);
            dsSSMap[j * dsWidth + i] = ssmap[y* width + x];

        }
    }
}

/**
 *  Triangulate points 
 */
int32_t PTK_Alg_StereoOG_triangulate(PTK_Alg_StereoOG_CreatePCAllParams * params,
                                     float                              * dsDisparity,
                                     uint8_t                            * dsSSMap,
                                     PTK_PointCloud                     * pc)
{
    int32_t i, j, p;
    int16_t dsWidth, dsHeight, dsFactor;
    uint8_t classId;

    int32_t baseline  = params->camParams.baseline;
    int32_t camHeight = params->camParams.camHeight;

    float   dcx       = params->camParams.dcx;
    float   dcy       = params->camParams.dcy;
    float   f         = params->camParams.f;
    float   cosPitch  = params->camParams.cosPitch;
    float   sinPitch  = params->camParams.sinPitch;

    float d, X, Y, Z;

    PTK_Point  * points;

    dsWidth  = params->cfgParams.dsWidth;
    dsHeight = params->cfgParams.dsHeight;
    dsFactor = params->cfgParams.dsFactor;

    // clear point cloud
    p = 0;
    PTK_PointCloud_clear(pc);

    points = PTK_PointCloud_getPoints(pc);

    for (j = 0; j < dsHeight; j++)
    {
        for (i = 0; i < dsWidth; i++)
        {
            // In operation mode, minimum disparity should be non-negative
            d = dsDisparity[j*dsWidth + i];
            if (d == 0)
            {
                continue;
            }

            if (dsSSMap)
            {
                classId = dsSSMap[j*dsWidth + i];

                // Treat 12 (person on bike) and 18 (bike) as pedestrian
                if (classId == 12 || classId == 18)
                {
                    classId = 11;
                }
            } else
            {
                classId = 11;
            }

            // check if classId should be one for point cloud
            if (!PTK_Alg_StereoOG_classToPC(classId))
            {
                continue;
            }

            /*
            Z = bf / d;
            X = (i - dcx) * baseline / d;
            Y = (j - dcy) * baseline / d;
            */
            Z = baseline * (f * cosPitch - (j*dsFactor - dcy)*sinPitch) / d;
            Y = baseline * ((j*dsFactor - dcy)*cosPitch + f * sinPitch) / d;
            Y = -Y + camHeight;
            X = baseline * (i*dsFactor - dcx) / d;

            // add a point into PCL
            // based on PointCloud_addPoint()
            points[p].x = X;
            points[p].y = Z;
            points[p].z = Y;
            points[p].meta.f = classId;

            pc->numPoints += 1;
            p++;
        }
    }

    // make sure numPoints is less than maxPoints
    if (pc->numPoints == pc->config.maxPoints)
        return -1;

    return 0;
}

