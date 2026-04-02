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
#include <perception/algos/stereo/ogmap/create_og.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))
#define MAX(a,b) ((a > b) ? (a) : (b))
#define MIN(a,b) ((a < b) ? (a) : (b))


PTK_Alg_StereoOG_BoxProp * PTK_Alg_StereOG_get3DBB(PTK_Alg_StereoOG_obs3DBox* obsBB)
{
    PTK_Alg_StereoOG_BoxProp * boxProp =
        (PTK_Alg_StereoOG_BoxProp*)((uint8_t*)obsBB + obsBB->bbOffset);

    return boxProp;
}

void PTK_Alg_StereoOG_CreateOGSetParams(PTK_Alg_StereoOG_CreateOGAllParams * params,
                                        PTK_Alg_StereoOG_CreateOGObj       * cntxt)
{
    /* initialize config parameters */
    cntxt->params.cfgParams.width                      = params->cfgParams.width;
    cntxt->params.cfgParams.height                     = params->cfgParams.height;
    cntxt->params.cfgParams.enableSpatialObjMerge      = params->cfgParams.enableSpatialObjMerge;
    cntxt->params.cfgParams.enableTemporalObjMerge     = params->cfgParams.enableTemporalObjMerge;
    cntxt->params.cfgParams.enableTemporalObjSmoothing = params->cfgParams.enableTemporalObjSmoothing;
    cntxt->params.cfgParams.objectDistanceMode         = params->cfgParams.objectDistanceMode;

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

    /* initialize OG parameters */
    cntxt->params.ogParams.xGridSize     = params->ogParams.xGridSize;
    cntxt->params.ogParams.yGridSize     = params->ogParams.yGridSize;
    cntxt->params.ogParams.xMinRange     = params->ogParams.xMinRange;
    cntxt->params.ogParams.xMaxRange     = params->ogParams.yMaxRange;
    cntxt->params.ogParams.yMinRange     = params->ogParams.yMinRange;
    cntxt->params.ogParams.yMaxRange     = params->ogParams.yMaxRange;
    cntxt->params.ogParams.xGridNum      =
        (cntxt->params.ogParams.xMaxRange - cntxt->params.ogParams.xMinRange) / params->ogParams.xGridSize;
    cntxt->params.ogParams.yGridNum      =
        (cntxt->params.ogParams.yMaxRange - cntxt->params.ogParams.yMinRange) / params->ogParams.yGridSize;
    cntxt->params.ogParams.thCnt         = params->ogParams.thCnt;
    cntxt->params.ogParams.thObjCnt      = params->ogParams.thObjCnt;
    cntxt->params.ogParams.maxNumObject  = params->ogParams.maxNumObject;
    cntxt->params.ogParams.cNeighNum     = params->ogParams.cNeighNum;
}


int32_t PTK_Alg_StereoOG_CreateOGConfig(PTK_Alg_StereoOG_CreateOGAllParams * params,
                                        PTK_Api_MemoryReq                  * memReq)
{
    uint32_t memSize;

    PTK_Alg_StereoOG_CreateOGObj cntxt;
    cntxt.params = *params;

    memReq->numBlks = 2;

    /* Block 0 is for the internal context. */
    memReq->blks[0].size    = sizeof(PTK_Alg_StereoOG_CreateOGObj);
    memReq->blks[0].memType = PTK_Api_MemoryType_LVL4;

    /* Block1 is for the memory needed by the algorithm */
    memSize = PTK_Alg_StereoOG_CreateOGGetMem(&cntxt, 0);
    memReq->blks[1].size    = memSize;
    memReq->blks[1].memType = PTK_Api_MemoryType_LVL4;


    return PTK_ALG_RET_SUCCESS;
}


PTK_AlgHandle PTK_Alg_StereoOG_CreateOGInit(PTK_Alg_StereoOG_CreateOGAllParams * params,
                                            const PTK_Api_MemoryRsp            * memRsp)
{
	PTK_Alg_StereoOG_CreateOGObj  * cntxt;

    /* Validate the input parameters. */
    if (!params                  ||
        !memRsp               ||
        (memRsp->numBlks < 2) ||
        !memRsp->blks[0].mem  ||
        !memRsp->blks[1].mem)
    {
        return NULL;
    }

    /* Initialize the context. */
    cntxt = (PTK_Alg_StereoOG_CreateOGObj *)memRsp->blks[0].mem;
    cntxt->params = *params;

    PTK_Alg_StereoOG_CreateOGGetMem(cntxt, memRsp->blks[1].mem);

    cntxt->curObject  = &cntxt->object1;
    cntxt->auxObject  = &cntxt->object2;

    cntxt->prevObject = &cntxt->object3;

    // init prevObject.numObject = -1, 
    // which means that no frame is not processed.
    PTK_Alg_StereoOG_resetTemporalReference(cntxt);

    return cntxt;
}

void PTK_Alg_StereoOG_createOG(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                               PTK_PointCloud               * pc,
                               PTK_Alg_StereoOG_obs3DBox    * obsBB)
{
    PTK_Point               * points;
    PTK_Alg_StereoOG_Object * tmpObj;

    int32_t                   numPoints = (int32_t) pc->numPoints;

    // reset OG maps
    PTK_Alg_StereoOG_resetOG(cntxt);

    // construct OG map
    points = PTK_PointCloud_getPoints(pc);
    PTK_Alg_StereoOG_addPoints(cntxt, points, numPoints);

    // do connected component analysis
    PTK_Alg_StereoOG_CCA(cntxt);

    // Merge neigboring objects
    if (cntxt->params.cfgParams.enableSpatialObjMerge)
    {
        PTK_Alg_StereoOG_mergeObjects(cntxt);
    }

    // Apply temporal filtering
    if (cntxt->params.cfgParams.enableTemporalObjMerge)
    {
        PTK_Alg_StereoOG_applyTemporalFilter(cntxt);
    }

    // get 3D object bounding boxes
    PTK_Alg_StereoOG_createBB(cntxt, cntxt->curObject, obsBB);

    // swap object and prevObject
    if (cntxt->params.cfgParams.enableTemporalObjMerge)
    {
        tmpObj            = cntxt->curObject;
        cntxt->curObject  = cntxt->prevObject;
        cntxt->prevObject = tmpObj;
    }

    return;
}


int32_t PTK_Alg_StereoOG_CreateOGGetMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr)
{
    int32_t  memSize = 0;

    int32_t  gridPropSize, ogSize, objSize, ogConnSize;
    uint8_t *pMem    = (uint8_t *) memPtr;

    gridPropSize = PTK_Alg_StereoOG_getGridPropMem(cntxt, (pMem?(pMem + memSize): pMem));
    memSize += gridPropSize;
    memSize = align(memSize, 8);

    ogSize = PTK_Alg_StereoOG_getOGMem(cntxt, (pMem?(pMem + memSize): pMem));
    memSize += ogSize;
    memSize = align(memSize, 8);

    objSize = PTK_Alg_StereoOG_getObjMem(cntxt, &cntxt->object1, (pMem?(pMem + memSize): pMem));
    memSize += objSize;
    memSize = align(memSize, 8);

    objSize = PTK_Alg_StereoOG_getObjMem(cntxt, &cntxt->object2, (pMem?(pMem + memSize): pMem));
    memSize += objSize;
    memSize = align(memSize, 8);

    objSize = PTK_Alg_StereoOG_getObjMem(cntxt, &cntxt->object3, (pMem?(pMem + memSize): pMem));
    memSize += objSize;
    memSize = align(memSize, 8);

    ogConnSize = PTK_Alg_StereoOG_getOGConnMem(cntxt, (pMem?(pMem + memSize): pMem));
    memSize += ogConnSize;
    memSize = align(memSize, 8);

    return memSize;
}

int32_t PTK_Alg_StereoOG_getGridPropMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr)
{
    int32_t gridPropSize = 0;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoOG_GridProp        * gridProp = &cntxt->gridProp;
    PTK_Alg_StereoOG_CreateOGParams  * ogParams = &cntxt->params.ogParams;

    if (pMem)
    {
        gridProp->numClass = (int8_t *) (pMem + gridPropSize);
    }
    gridPropSize += sizeof(int8_t) * (ogParams->xGridNum * ogParams->yGridNum);
    gridPropSize = align(gridPropSize, 8);

    if (pMem)
    {
        gridProp->maxHeight = (int16_t *) (pMem + gridPropSize);
    }
    gridPropSize += sizeof(int16_t) * (ogParams->xGridNum * ogParams->yGridNum * MAX_CLASS_NUM_IN_GRID);
    gridPropSize = align(gridPropSize, 8);

    if (pMem)
    {
        gridProp->minHeight = (int16_t *) (pMem + gridPropSize);
    }
    gridPropSize += sizeof(int16_t) * (ogParams->xGridNum * ogParams->yGridNum * MAX_CLASS_NUM_IN_GRID);
    gridPropSize = align(gridPropSize, 8);

    if (pMem)
    {
        gridProp->classId = (uint8_t *) (pMem + gridPropSize);
    }
    gridPropSize += sizeof(uint8_t) * (ogParams->xGridNum * ogParams->yGridNum * MAX_CLASS_NUM_IN_GRID);
    gridPropSize = align(gridPropSize, 8);

    if (pMem)
    {
        gridProp->count = (int16_t *) (pMem + gridPropSize);
    }
    gridPropSize += sizeof(int16_t) * (ogParams->xGridNum * ogParams->yGridNum * MAX_CLASS_NUM_IN_GRID);
    gridPropSize = align(gridPropSize, 8);

    return gridPropSize;
}

int32_t PTK_Alg_StereoOG_getOGMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr)
{
    int32_t ogSize = 0;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoOG_OccupancyGrid   * og       = &cntxt->og;
    PTK_Alg_StereoOG_CreateOGParams  * ogParams = &cntxt->params.ogParams;

    if (pMem)
    {
        og->occupied = (int8_t *) (pMem + ogSize);
    }
    ogSize += sizeof(int8_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogSize = align(ogSize, 8);

    if (pMem)
    {
        og->maxHeight = (int16_t *) (pMem + ogSize);
    }
    ogSize += sizeof(int16_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogSize = align(ogSize, 8);

    if (pMem)
    {
        og->minHeight = (int16_t *) (pMem + ogSize);
    }
    ogSize += sizeof(int16_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogSize = align(ogSize, 8);

    if (pMem)
    {
        og->classId = (uint8_t *) (pMem + ogSize);
    }
    ogSize += sizeof(uint8_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogSize = align(ogSize, 8);

    if (pMem)
    {
        og->count = (int16_t *) (pMem + ogSize);
    }
    ogSize += sizeof(int16_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogSize = align(ogSize, 8);

    return ogSize;
}


int32_t PTK_Alg_StereoOG_getObjMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  PTK_Alg_StereoOG_Object *obj, uint8_t *memPtr)
{
    int32_t objSize = 0;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoOG_CreateOGParams  * ogParams = &cntxt->params.ogParams;

    if (pMem)
    {
        obj->classId = (uint8_t *) (pMem + objSize);
    }
    objSize += sizeof(uint8_t) * ogParams->maxNumObject;
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->count = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * ogParams->maxNumObject;
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->maxHeight = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * ogParams->maxNumObject;
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->minHeight = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * ogParams->maxNumObject;
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->maxObjXGrid = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->minObjXGrid = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->maxObjYGrid = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->minObjYGrid = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->merged = (uint8_t *) (pMem + objSize);
    }
    objSize += sizeof(uint8_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

#if QUAD_LATERAL_BB
    if (pMem)
    {
        obj->maxObjXGridYPos = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->minObjXGridYPos = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->maxObjYGridXPos = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->minObjYGridXPos = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);
#endif

    if (pMem)
    {
        obj->width = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->height = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->centerX = (float *) (pMem + objSize);
    }
    objSize += sizeof(float) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->centerY = (float *) (pMem + objSize);
    }
    objSize += sizeof(float) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->age = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->visible = (uint8_t *) (pMem + objSize);
    }
    objSize += sizeof(uint8_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->invisibleSpan = (uint8_t *) (pMem + objSize);
    }
    objSize += sizeof(uint8_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->correspondence = (int16_t *) (pMem + objSize);
    }
    objSize += sizeof(int16_t) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->dx = (float *) (pMem + objSize);
    }
    objSize += sizeof(float) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    if (pMem)
    {
        obj->dy = (float *) (pMem + objSize);
    }
    objSize += sizeof(float) * (ogParams->maxNumObject);
    objSize = align(objSize, 8);

    return objSize;
}


int32_t PTK_Alg_StereoOG_getOGConnMem(PTK_Alg_StereoOG_CreateOGObj * cntxt,  uint8_t *memPtr)
{
    int32_t ogConnSize = 0;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoOG_OGConn          * ogConn   = &cntxt->ogConn; 
    PTK_Alg_StereoOG_CreateOGParams  * ogParams = &cntxt->params.ogParams;

    if (pMem)
    {
        ogConn->visited = (int8_t *) (pMem + ogConnSize);
    }
    ogConnSize += sizeof(int8_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogConnSize = align(ogConnSize, 8);

    if (pMem)
    {
        ogConn->neighborQueue = (int32_t *) (pMem + ogConnSize);
    }
    ogConnSize += sizeof(int32_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogConnSize = align(ogConnSize, 8);


    if (pMem)
    {
        ogConn->neighborIndex = (int32_t *) (pMem + ogConnSize);
    }
    ogConnSize += sizeof(int32_t) * (ogParams->xGridNum * ogParams->yGridNum);
    ogConnSize = align(ogConnSize, 8);

    return ogConnSize;
}

/**
 * Get the required number of bytes to pass to a separate allocator (e.g. Utils_memAlloc) to create
 * memory for a point cloud able to contain the given number of points. The size calculated is
 * suitable for packing into an array of point clouds
 */
uint32_t PTK_Alg_StereoOG_getObsBBSize(const PTK_Alg_StereoOG_CreateOGParams *ogParams)
{
    int32_t  obsBBSize = 0;

    // Need space for the whole struct
    obsBBSize += sizeof(PTK_Alg_StereoOG_obs3DBox);
    obsBBSize = align(obsBBSize, 8);

    // Add storage for all points
    obsBBSize += sizeof(PTK_Alg_StereoOG_BoxProp) * ogParams->maxNumObject;
    obsBBSize = align(obsBBSize, 8);

    return obsBBSize;
}

/**
 * Initialize a point cloud structure using the given chunk of memory
 */
PTK_Alg_StereoOG_obs3DBox * PTK_Alg_StereoOG_initObsBB(uint8_t * memPtr)
{
    PTK_Alg_StereoOG_obs3DBox * obsBB = (PTK_Alg_StereoOG_obs3DBox *) memPtr;

    uint32_t offset = 0;
    offset = sizeof(PTK_Alg_StereoOG_obs3DBox);
    offset = align(offset, 8);

    obsBB->bbOffset = offset;
    obsBB->numObject = 0;

    return obsBB;
}

/**
 *  Create occupancy grid map
 */
void PTK_Alg_StereoOG_addPoints(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                                PTK_Point                    * points,
                                int32_t                        numPoints)
{
    int32_t    i, c;
    int32_t    xp, yp;
    uint8_t    classId;
    int8_t     cIdx;
    int16_t    maxCount, minHeight, maxHeight, count;

    float      X, Y, Z;

    PTK_Alg_StereoOG_OccupancyGrid   * og       = &cntxt->og;
    PTK_Alg_StereoOG_CreateOGParams  * ogParams = &cntxt->params.ogParams;

    int8_t   * gridNumClass;
    int16_t  * gridMaxHeight;
    int16_t  * gridMinHeight;
    uint8_t  * gridClass;
    int16_t  * gridCount;

    int32_t    xGridNum = ogParams->xGridNum;
    int32_t    yGridNum = ogParams->yGridNum;

    // 1. Add points into OG map
    for (i = 0; i < numPoints; i++)
    {
        X = points[i].x;
        Y = points[i].y;
        Z = points[i].z;
        classId = (uint8_t)points[i].meta.f;

        if (Z > GRID_MAX_HEIGHT || Z < GRID_MIN_HEIGHT)
        {
            continue;
        }

        if (X < ogParams->xMinRange || X > ogParams->xMaxRange)
        {
            continue;
        }

        if (Y < ogParams->yMinRange || Y > ogParams->yMaxRange)
        {
            continue;
        }

        if (Z < 0)
        {
            continue;
        }

        xp = floor((X - ogParams->xMinRange) / ogParams->xGridSize);
        yp = floor((Y - ogParams->yMinRange) / ogParams->yGridSize);

        if (PTK_Alg_StereoOG_classToPC(classId))
        {
            gridNumClass  = &(cntxt->gridProp.numClass[yp*xGridNum + xp]);
            gridMaxHeight = &(cntxt->gridProp.maxHeight[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);
            gridMinHeight = &(cntxt->gridProp.minHeight[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);
            gridClass     = &(cntxt->gridProp.classId[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);
            gridCount     = &(cntxt->gridProp.count[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);

            if (*gridNumClass == 0)
            {
                *gridNumClass = 1;
                gridMaxHeight[*gridNumClass - 1] = Z;
                gridMinHeight[*gridNumClass - 1] = Z;
                gridClass[*gridNumClass - 1]     = classId;
                gridCount[*gridNumClass - 1]     = 1;
            } else
            {
                // check if there was the same classId already
                cIdx = -1;
                for (c = 0; c < *gridNumClass; c++)
                {
                    if (gridClass[c] == classId)
                    {
                        cIdx = c;
                        break;
                    }
                }

                if (cIdx == -1)
                {
                    *gridNumClass += 1;
                    gridMaxHeight[*gridNumClass - 1] = Z;
                    gridMinHeight[*gridNumClass - 1] = Z;
                    gridClass[*gridNumClass - 1]     = classId;
                    gridCount[*gridNumClass - 1]     = 1;
                } else
                {
                    gridCount[cIdx] += 1;
                    if (Z > gridMaxHeight[cIdx])
                    {
                        gridMaxHeight[cIdx] = Z;
                    }
                    if (Z < gridMinHeight[cIdx])
                    {
                        gridMinHeight[cIdx] = Z;
                    }
                }
            }
        }
    }

    // 2. Determine whether each grid is occupied or not
    for (yp = 0; yp < yGridNum; yp++)
    {
        for (xp = 0; xp < xGridNum; xp++)
        {
            og->occupied[yp * xGridNum + xp] = 0;

            gridNumClass  = &(cntxt->gridProp.numClass[yp*xGridNum + xp]);
            gridMaxHeight = &(cntxt->gridProp.maxHeight[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);
            gridMinHeight = &(cntxt->gridProp.minHeight[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);
            gridClass     = &(cntxt->gridProp.classId[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);
            gridCount     = &(cntxt->gridProp.count[yp*MAX_CLASS_NUM_IN_GRID*xGridNum + xp*MAX_CLASS_NUM_IN_GRID]);

            if (*gridNumClass != 0)
            {
                maxCount = 0;
                cIdx = -1;

                // find the classId of maximum occurrence
                for (c = 0; c < *gridNumClass; c++)
                {
                    if (gridCount[c] > maxCount)
                    {
                        maxCount = gridCount[c];
                        cIdx = c;
                    }
                }

                classId   = gridClass[cIdx];
                maxHeight = gridMaxHeight[cIdx];
                minHeight = gridMinHeight[cIdx];
                count     = gridCount[cIdx];

                // Mark a grid occupied
                if (PTK_Alg_StereoOG_classToDetect(classId) && 
                    (maxCount > ogParams->thCnt) &&
                    (maxHeight - minHeight > 300))
                {
                    og->occupied[yp * xGridNum + xp]  = 1;
                    og->maxHeight[yp * xGridNum + xp] = maxHeight;
                    og->minHeight[yp * xGridNum + xp] = minHeight;
                    og->classId[yp * xGridNum + xp]   = classId;
                    og->count[yp * xGridNum + xp]     = count;
                }
            }
        }
    }
}

/**
 *  Perform connected component analysis to detect objects from OG map
 */
void PTK_Alg_StereoOG_CCA(PTK_Alg_StereoOG_CreateOGObj * cntxt)
{
    int32_t i, j, k;
    int32_t gridIdx;
    int32_t idx, xIdx, yIdx;
    int16_t numNeigh;
    int16_t objLabel = 0;
    uint8_t objClass;

    PTK_Alg_StereoOG_OccupancyGrid   * og       = &cntxt->og;
    PTK_Alg_StereoOG_Object          * object   = cntxt->curObject;
    PTK_Alg_StereoOG_OGConn          * ogConn   = &cntxt->ogConn;
    PTK_Alg_StereoOG_CreateOGParams  * ogParams = &cntxt->params.ogParams;

    int32_t xGridNum = ogParams->xGridNum;
    int32_t yGridNum = ogParams->yGridNum;

    int16_t objCntTh;
    int16_t gridCntTh;

    for (j = 0; j < yGridNum; j++)
    {
        for (i = 0; i < xGridNum; i++)
        {
            gridIdx = j*xGridNum + i;

            if (og->occupied[gridIdx]    == 0 ||
                og->classId[gridIdx]     == 0 ||
                ogConn->visited[gridIdx] == 1)
            {
                continue;
            }

            // push connected grid index into a queue
            ogConn->numGridInQueue++;
            ogConn->neighborQueue[ogConn->numGridInQueue-1] = gridIdx;
            // mark it VISITED
            ogConn->visited[gridIdx] = 1;

            // get object classId
            objClass = og->classId[gridIdx];

            while (ogConn->numGridInQueue > 0)
            {
                idx = ogConn->neighborQueue[ogConn->numGridInQueue-1];
                yIdx = idx / xGridNum;
                xIdx = idx % xGridNum;

                // poped one neighboring grid
                ogConn->numGridInQueue--;

                // increase the number of points for an objet
                object->count[objLabel] += og->count[gridIdx];

                if (xIdx > object->maxObjXGrid[objLabel])
                {
                    object->maxObjXGrid[objLabel]     = xIdx;
#if QUAD_LATERAL_BB
                    object->maxObjXGridYPos[objLabel] = yIdx;
#endif
                }
                if (xIdx < object->minObjXGrid[objLabel])
                {
                    object->minObjXGrid[objLabel]     = xIdx;
#if QUAD_LATERAL_BB
                    object->minObjXGridYPos[objLabel] = yIdx;
#endif
                }
                if (yIdx > object->maxObjYGrid[objLabel])
                {
                    object->maxObjYGrid[objLabel]     = yIdx;
#if QUAD_LATERAL_BB
                    object->maxObjYGridXPos[objLabel] = xIdx;
#endif
                }
                if (yIdx < object->minObjYGrid[objLabel])
                {
                    object->minObjYGrid[objLabel]     = yIdx;
#if QUAD_LATERAL_BB
                    object->minObjYGridXPos[objLabel] = xIdx;
#endif
                }

                if(og->maxHeight[idx] > object->maxHeight[objLabel])
                {
                    object->maxHeight[objLabel] = og->maxHeight[idx];
                }
                if(og->minHeight[idx] < object->minHeight[objLabel])
                {
                    object->minHeight[objLabel] = og->minHeight[idx];
                }

                // get connected grid to idx
                if ((ogParams->cNeighNum == 24)         &&
                    (yIdx > 1) && (yIdx < yGridNum - 2) &&
                    (xIdx > 1) && (xIdx < xGridNum - 2))
                {
                    numNeigh = getNeighbor24Idx(ogParams, ogConn, idx, xIdx, yIdx);
                } else
                {
                    numNeigh = getNeighbor8Idx(ogParams, ogConn, idx, xIdx, yIdx);
                }

                // add neiboring grids in a queue
                for (k = 0; k < numNeigh; k++)
                {
                    idx = ogConn->neighborIndex[k];
                    yIdx = idx / xGridNum;
                    xIdx = idx % xGridNum;

                    if (og->occupied[idx]    == 1        &&
                        og->classId[idx]     == objClass &&
                        ogConn->visited[idx] == 0)
                    {
                        ogConn->numGridInQueue++;
                        ogConn->neighborQueue[ogConn->numGridInQueue-1] = idx;
                        // mark it VISITED
                        ogConn->visited[idx] = 1;
                    }
                }
            } // while


            objCntTh = ogParams->thObjCnt;
            gridCntTh = 0;

            if (objClass == 13)
            {
                objCntTh *= 2;
                gridCntTh = 4;
            }

            if(object->count[objLabel] > objCntTh && 
               (object->maxObjXGrid[objLabel] - object->minObjXGrid[objLabel] + 1) * (object->maxObjYGrid[objLabel] - object->minObjYGrid[objLabel] + 1) > gridCntTh)
            {
                object->classId[objLabel] = objClass;

                // Needed for spatial merge
                object->width[objLabel]   =  object->maxObjXGrid[objLabel] - object->minObjXGrid[objLabel] + 1;
                object->height[objLabel]  =  object->maxObjYGrid[objLabel] - object->minObjYGrid[objLabel] + 1;
                object->centerX[objLabel] = (object->maxObjXGrid[objLabel] + object->minObjXGrid[objLabel])*1.0/2;
                object->centerY[objLabel] = (object->maxObjYGrid[objLabel] + object->minObjYGrid[objLabel])*1.0/2;

                // Needed for temporal merge
                object->age[objLabel]           = 1;
                object->visible[objLabel]       = 1;
                object->invisibleSpan[objLabel] = 0;
                object->dx[objLabel]            = 0.0;
                object->dy[objLabel]            = 0.0;

                object->numObject++;
                objLabel++;

                // The number of objects are limited
                if (object->numObject >= ogParams->maxNumObject)
                {
                    break;
                }
            } else
            {
                // re-init
                object->count[objLabel]       =  0;
                //object->classId[objLabel]     =  255;
                object->maxHeight[objLabel]   = -5000;
                object->minHeight[objLabel]   =  5000;
                object->maxObjXGrid[objLabel] =  0;
                object->minObjXGrid[objLabel] =  ogParams->xGridNum;
                object->maxObjYGrid[objLabel] =  0;
                object->minObjYGrid[objLabel] =  ogParams->yGridNum;
            }
        }
    }
}

/**
 *  Get 8-conn neighboring pixels
 */
int16_t getNeighbor8Idx(PTK_Alg_StereoOG_CreateOGParams * ogParams, 
                        PTK_Alg_StereoOG_OGConn         * ogConn,
                        int32_t                           idx,
                        int32_t                           xIdx,
                        int32_t                           yIdx)
{
    int16_t numNeigh = 0;
    int32_t xGridNum = ogParams->xGridNum;
    int32_t yGridNum = ogParams->yGridNum;

    // 1st row of 3x3 window
    if (yIdx > 0)
    {
        if (xIdx > 0)
        {
            ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx-1;
            numNeigh++;
        }
        
        ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx;
        numNeigh++;

        if (xIdx < xGridNum - 1)
        {
            ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx+1;
            numNeigh++;
        }
    } 

    // 2nd row of 3x3 window
    if (xIdx > 0)
    {
        ogConn->neighborIndex[numNeigh] = (yIdx)*xGridNum + xIdx-1;
        numNeigh++;
    }
    
    if (xIdx < xGridNum - 1)
    {
        ogConn->neighborIndex[numNeigh] = (yIdx)*xGridNum + xIdx+1;
        numNeigh++;
    }

    // 3rd row of 3x3 window
    if (yIdx < yGridNum - 1)
    {
        if (xIdx > 0)
        {
            ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx-1;
            numNeigh++;
        }

        ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx;
        numNeigh++;

        if (xIdx < xGridNum - 1)
        {
            ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx+1;
            numNeigh++;
        }
    }

    return numNeigh;
}

/**
 *  Get 24-conn neighboring pixels
 */
int16_t getNeighbor24Idx(PTK_Alg_StereoOG_CreateOGParams * ogParams, 
                         PTK_Alg_StereoOG_OGConn         * ogConn,
                         int32_t                           idx,
                         int32_t                           xIdx,
                         int32_t                           yIdx)
{
    int16_t numNeigh = 0;
    int32_t xGridNum = ogParams->xGridNum;

    // 1st row of 5x5 window
    ogConn->neighborIndex[numNeigh] = (yIdx-2)*xGridNum + xIdx-2;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-2)*xGridNum + xIdx-1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-2)*xGridNum + xIdx;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-2)*xGridNum + xIdx+1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-2)*xGridNum + xIdx+2;
    numNeigh++;

    // 2nd row of 5x5 window
    ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx-2;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx-1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx+1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx-1)*xGridNum + xIdx+2;
    numNeigh++;

    // 3rd row of 5x5 window
    ogConn->neighborIndex[numNeigh] = (yIdx)*xGridNum + xIdx-2;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx)*xGridNum + xIdx-1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx)*xGridNum + xIdx+1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx)*xGridNum + xIdx+2;
    numNeigh++;

    // 4th row of 5x5 window
    ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx-2;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx-1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx+1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+1)*xGridNum + xIdx+2;
    numNeigh++;

    // 5th row of 5x5 window
    ogConn->neighborIndex[numNeigh] = (yIdx+2)*xGridNum + xIdx-2;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+2)*xGridNum + xIdx-1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+2)*xGridNum + xIdx;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+2)*xGridNum + xIdx+1;
    numNeigh++;

    ogConn->neighborIndex[numNeigh] = (yIdx+2)*xGridNum + xIdx+2;
    numNeigh++;

    return numNeigh;
}


/**
 *  Perform objects merge process
 */
void PTK_Alg_StereoOG_mergeObjects(PTK_Alg_StereoOG_CreateOGObj * cntxt)
{
    int16_t i, n;
    int16_t numObjects;
    bool    merged;

    PTK_Alg_StereoOG_Object             * initObj;
    PTK_Alg_StereoOG_Object             * curObj;
    PTK_Alg_StereoOG_Object             * newObj;
    PTK_Alg_StereoOG_Object             * tmpObj;

    PTK_Alg_StereoOG_CreateOGParams  * ogParams  = &cntxt->params.ogParams;

    initObj = curObj = cntxt->curObject;
    newObj  = cntxt->auxObject;

    // 20 is max iteration
    for (n = 0; n < 20; n++)
    {
        numObjects        = curObj->numObject;
        newObj->numObject = 0;

        // All objects are initial detection before merging process
        for (i = 0; i < numObjects; i++)
        {
            curObj->merged[i] = 0;
        }

        merged = false;
        for (i = 0; i < numObjects; i++)
        {
            if (curObj->merged[i] == 0)
            {
                merged = PTK_Alg_StereoOG_mergeTwoObjects(
                             curObj, 
                             newObj, 
                             ogParams->xGridSize, 
                             i);

                if (merged == true)
                {
                    break;
                }
            }
        }

        // copy un-merged objects
        for (i = 0; i < numObjects; i++)
        {
            if (curObj->merged[i] == 0)
            {
                PTK_Alg_StereoOG_copyObjectInfo(
                    curObj, 
                    newObj, 
                    i, 
                    newObj->numObject);

                // Needed for temporal merge
                newObj->age[newObj->numObject]             = 1;
                newObj->visible[newObj->numObject]         = 1;
                newObj->invisibleSpan[newObj->numObject]   = 0;
                newObj->dx[newObj->numObject]              = 0.0;
                newObj->dy[newObj->numObject]              = 0.0;

                newObj->numObject++;
            }
        }

        // swap pointers
        tmpObj = curObj;
        curObj = newObj;
        newObj = tmpObj;

        if (merged == false)
        {
            break;
        }
    }


    // If initObj is not the same to curObj, we swap point again
    // so that curObj is should be cntxt->curObject
    // This is needed to swap cntxt->curObject and cntxt->prevObject always
    // for temporal filtering
    if (initObj != curObj)
    {
        tmpObj           = cntxt->curObject;
        cntxt->curObject = cntxt->auxObject;
        cntxt->auxObject = tmpObj;
    }
}

/** 
 * Merge two objects 
 */
bool PTK_Alg_StereoOG_mergeTwoObjects(PTK_Alg_StereoOG_Object      * curObj,
                                      PTK_Alg_StereoOG_Object      * newObj,
                                      int32_t                        gridSize,
                                      int16_t                        index)
{
    bool     merge = false;
    bool     xOverlap;
    bool     yOverlap;

    int16_t  i;
    int16_t  closeIdx = -1;
    int16_t  mergeIdx;
    int16_t  numObjects = curObj->numObject;

    // current object's width and height on OG map
    int16_t  objWidth, objHeight;
    // closest object's width and height on OG map
    int16_t  nWidth, nHeight;
    // combined object's width and height on OG map
    int16_t  cWidth, cHeight;

    int32_t  distTh;

    // Check if two objects should be merged
    objWidth  = curObj->maxObjXGrid[index] - curObj->minObjXGrid[index] + 1;
    objHeight = curObj->maxObjYGrid[index] - curObj->minObjYGrid[index] + 1;

    // Find the objet that should be merged
    for (i = index+1; i < numObjects; i++)
    {
        if (curObj->merged[i] == 0)
        {
            nWidth    = curObj->maxObjXGrid[i] - curObj->minObjXGrid[i] + 1;
            nHeight   = curObj->maxObjYGrid[i] - curObj->minObjYGrid[i] + 1;

            cWidth    = MAX(curObj->maxObjXGrid[index], curObj->maxObjXGrid[i]) - 
                        MIN(curObj->minObjXGrid[index], curObj->minObjXGrid[i]) + 1;
            cHeight   = MAX(curObj->maxObjYGrid[index], curObj->maxObjYGrid[i]) - 
                        MIN(curObj->minObjYGrid[index], curObj->minObjYGrid[i]) + 1;

            xOverlap = false;
            if (cWidth < objWidth + nWidth) 
            {
                xOverlap = true;
            }

            yOverlap = false;
            if (cHeight < objHeight + nHeight)
            {
                yOverlap = true;
            }

            if (xOverlap == true && yOverlap == true)
            {
                // If both X and Y of two bounding boxes overlap, we merge them.
                merge = true;
            }
            else if (xOverlap == true)
            {
                // If only X of two bounding boxes overlapes, we check distance of other side (Y)
                distTh = -1;
                if (curObj->classId[index] == 13 && curObj->classId[i] == 13)
                {
                    distTh = 600;
                } else 
                if (curObj->classId[index] == 11 && curObj->classId[i] == 11)
                {
                    distTh = 100;
                }

                // If distance is smaller than a threshold, we merge
                // The threshold depends on the classes of two objects
                if ((cHeight - nHeight - objHeight)*gridSize <= distTh)
                {
                    merge = true;
                }
            } else if (yOverlap == true)
            {
                // If only Y of two bounding boxes overlapes, we check distance of other side (X)
                distTh = -1;
                if (curObj->classId[index] == 13 && curObj->classId[i] == 13)
                {
                    distTh = 600;
                } else 
                if (curObj->classId[index] == 11 && curObj->classId[i] == 11)
                {
                    distTh = 100;
                }

                // If distance is smaller than a threshold, we merge
                // The threshold depends on the classes of two objects
                if ((cWidth - nWidth - objWidth)*gridSize <= distTh)
                {
                    merge = true;
                }
            }

            if (merge == true)
            {
                closeIdx = i;
                break;
            }
        }
    }

    // Do merge two objects 
    if (merge)
    {
        // Mark index and closeIdx as a merged object
        curObj->merged[index] = 1;
        curObj->merged[closeIdx] = 1;

        // Create a new object
        // if one of objects' class is 13 (car), the merged object's class is 13
        // - May need another criteria
        newObj->classId[newObj->numObject]     = curObj->classId[index];
        if (curObj->classId[closeIdx] == 13)
        {
            newObj->classId[newObj->numObject] = curObj->classId[closeIdx];
        }
        newObj->count[newObj->numObject]       = curObj->count[index] + curObj->count[closeIdx];
        newObj->maxHeight[newObj->numObject]   = MAX(curObj->maxHeight[index],   curObj->maxHeight[closeIdx]);
        newObj->minHeight[newObj->numObject]   = MIN(curObj->minHeight[index],   curObj->minHeight[closeIdx]);

        if (curObj->maxObjXGrid[index] > curObj->maxObjXGrid[closeIdx])
        {
            mergeIdx = index;
        } else
        {
            mergeIdx = closeIdx;
        }
        newObj->maxObjXGrid[newObj->numObject]     = curObj->maxObjXGrid[mergeIdx];
#if QUAD_LATERAL_BB
        newObj->maxObjXGridYPos[newObj->numObject] = curObj->maxObjXGridYPos[mergeIdx];
#endif

        if (curObj->minObjXGrid[index] < curObj->minObjXGrid[closeIdx])
        {
            mergeIdx = index;
        } else
        {
            mergeIdx = closeIdx;
        }
        newObj->minObjXGrid[newObj->numObject]     = curObj->minObjXGrid[mergeIdx];
#if QUAD_LATERAL_BB
        newObj->minObjXGridYPos[newObj->numObject] = curObj->minObjXGridYPos[mergeIdx];
#endif

        if (curObj->maxObjYGrid[index] > curObj->maxObjYGrid[closeIdx])
        {
            mergeIdx = index;
        } else
        {
            mergeIdx = closeIdx;
        }
        newObj->maxObjYGrid[newObj->numObject]     = curObj->maxObjYGrid[mergeIdx];
#if QUAD_LATERAL_BB
        newObj->maxObjYGridXPos[newObj->numObject] = curObj->maxObjYGridXPos[mergeIdx];
#endif

        if (curObj->minObjYGrid[index] < curObj->minObjYGrid[closeIdx])
        {
            mergeIdx = index;
        } else
        {
            mergeIdx = closeIdx;
        }
        newObj->minObjYGrid[newObj->numObject]     = curObj->minObjYGrid[mergeIdx];
#if QUAD_LATERAL_BB
        newObj->minObjYGridXPos[newObj->numObject] = curObj->minObjYGridXPos[mergeIdx];
#endif

        // Needed for spatial merge
        newObj->width[newObj->numObject]   =  newObj->maxObjXGrid[newObj->numObject] - 
                                              newObj->minObjXGrid[newObj->numObject] + 1;
        newObj->height[newObj->numObject]  =  newObj->maxObjYGrid[newObj->numObject] - 
                                              newObj->minObjYGrid[newObj->numObject] + 1;
        newObj->centerX[newObj->numObject] = (newObj->maxObjXGrid[newObj->numObject] + 
                                              newObj->minObjXGrid[newObj->numObject])*1.0/2;
        newObj->centerY[newObj->numObject] = (newObj->maxObjYGrid[newObj->numObject] + 
                                              newObj->minObjYGrid[newObj->numObject])*1.0/2;

        // Needed for temporal merge
        newObj->age[newObj->numObject]           = 1;
        newObj->visible[newObj->numObject]       = 1;
        newObj->invisibleSpan[newObj->numObject] = 0;
        newObj->dx[newObj->numObject]            = 0.0;
        newObj->dy[newObj->numObject]            = 0.0;

        newObj->numObject++;
    } else
    {
        // Do not merge
        curObj->merged[index] = 1;

        PTK_Alg_StereoOG_copyObjectInfo(
            curObj, 
            newObj, 
            index, 
            newObj->numObject);

        // Needed for temporal merge
        newObj->age[newObj->numObject]           = 1;
        newObj->visible[newObj->numObject]       = 1;
        newObj->invisibleSpan[newObj->numObject] = 0;
        newObj->dx[newObj->numObject]            = 0.0;
        newObj->dy[newObj->numObject]            = 0.0;

        newObj->numObject++;
    }

    return merge;
}


void PTK_Alg_StereoOG_applyTemporalFilter(PTK_Alg_StereoOG_CreateOGObj * cntxt)
{
    // prevObject.numObject = -1 means that this is the first frame
    if (cntxt->prevObject->numObject == -1)
    {
        return;
    }

    // For the objects in the previous frame, find the corresponding objects
    // in the current frame.
    PTK_Alg_StereoOG_findCorrespondence(cntxt, cntxt->prevObject, cntxt->curObject, DIR_FORWARD);

    // For the objects in the current frame, find the corresponding objects
    // in the previous frame.
    PTK_Alg_StereoOG_findCorrespondence(cntxt, cntxt->curObject, cntxt->prevObject, DIR_BACKWARD);

    // Assoicate previous and current frames' objects
    // - Forward serach and backward search should match
    PTK_Alg_StereoOG_associateCorrespondence(cntxt, cntxt->curObject, cntxt->prevObject);
}

void PTK_Alg_StereoOG_findCorrespondence(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                                         PTK_Alg_StereoOG_Object      * srcObj,
                                         PTK_Alg_StereoOG_Object      * refObj,
                                         uint8_t                        direction)
{
    int16_t        i, j;
    int16_t        numSrcObject;
    int16_t        numRefObject;
    int16_t        gridSize;

    float          distance;
    float          minDistance;
    float          distTh;

    numSrcObject = srcObj->numObject;
    numRefObject = refObj->numObject;

    gridSize     = cntxt->params.ogParams.xGridSize;
    if (cntxt->params.cfgParams.objectDistanceMode == 0)
    {
        distTh   = 3000;
    } else
    {
        distTh   = 8000;
    }

    // from previous to current
    for (i = 0; i < numSrcObject; i++)
    {
        //minDistance = FLT_MAX;
        minDistance = distTh;

        // init correspondence to CORRESPONDENCE_INIT
        srcObj->correspondence[i] = CORRESPONDENCE_INIT;

        for (j = 0; j < numRefObject; j++)
        {
            // Corresponding object should be the same class
            if (srcObj->classId[i] != refObj->classId[j])
            {
                continue;
            }

            // compute the distance between objects
            distance = PTK_Alg_StereoOG_computeDistanceObjects(
                           cntxt->params.cfgParams.objectDistanceMode,
                           srcObj,
                           refObj, 
                           i, 
                           j);

            if (distance*gridSize< minDistance)
            {
                minDistance = distance*gridSize;
                srcObj->correspondence[i] = j;
            }
        }
    }
}


void PTK_Alg_StereoOG_associateCorrespondence(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                                              PTK_Alg_StereoOG_Object *curObj,
                                              PTK_Alg_StereoOG_Object *prevObj)
{
    int16_t i, j;
    int16_t numCurObject;
    int16_t numPrevObject;
    int16_t corr;

    // Backward temporal merge
    // Check if there are objects in the current frame
    // that do not have correspondences in the previous frame
    PTK_Alg_StereoOG_mergeObjectsTemporal(cntxt, DIR_BACKWARD);

    // Forward temporal merge
    // Check if there are objects in the previous frame 
    // that do not have correspondences in the previous frame
    PTK_Alg_StereoOG_mergeObjectsTemporal(cntxt, DIR_FORWARD);

    // should be reset after temporal merge
    curObj        = cntxt->curObject;
    prevObj       = cntxt->prevObject;

    numCurObject  = curObj->numObject;
    numPrevObject = prevObj->numObject;

    // Assoicate the object
    // For association, forward and backward should match
    for (i = 0; i < numCurObject; i++)
    {
        corr = curObj->correspondence[i];
        if (corr != CORRESPONDENCE_INIT &&
            i == prevObj->correspondence[corr])
        {
            // this object is visible
            curObj->age[i]           = prevObj->age[corr] + 1;
            curObj->visible[i]       = 1;
            curObj->invisibleSpan[i] = 0;

            curObj->dx[i]            = curObj->centerX[i] - prevObj->centerX[corr];
            curObj->dy[i]            = curObj->centerY[i] - prevObj->centerY[corr];

            if (cntxt->params.cfgParams.enableTemporalObjSmoothing)
            {
                // dx and dy are estimates, so adding dx and dy can cause more flickerings.
                curObj->maxHeight[i]     = (curObj->maxHeight[i] + prevObj->maxHeight[corr] + 1)/2;
                curObj->maxObjXGrid[i]   = (curObj->maxObjXGrid[i] * 0.6 + prevObj->maxObjXGrid[corr] * 0.4 /* + prevObj->dx[corr] */ + 0.5);
                curObj->minObjXGrid[i]   = (curObj->minObjXGrid[i] * 0.6 + prevObj->minObjXGrid[corr] * 0.4 /* + prevObj->dx[corr] */ + 0.5);
                curObj->maxObjYGrid[i]   = (curObj->maxObjYGrid[i] * 0.6 + prevObj->maxObjYGrid[corr] * 0.4 /* + prevObj->dy[corr] */ + 0.5);
                curObj->minObjYGrid[i]   = (curObj->minObjYGrid[i] * 0.6 + prevObj->minObjYGrid[corr] * 0.4 /* + prevObj->dy[corr] */ + 0.5);;

                curObj->width[i]         =  curObj->maxObjXGrid[i] - curObj->minObjXGrid[i] + 1;
                curObj->height[i]        =  curObj->maxObjYGrid[i] - curObj->minObjYGrid[i] + 1;
                curObj->centerX[i]       = (curObj->maxObjXGrid[i] + curObj->minObjXGrid[i])*1.0 / 2;
                curObj->centerY[i]       = (curObj->maxObjYGrid[i] + curObj->minObjYGrid[i])*1.0 / 2;
            }

            // Set to CORRESPONDENCE_FOUND to indicate that this object has 
            // the correspondence in the current frame
            prevObj->correspondence[corr] = CORRESPONDENCE_FOUND;
        } else
        {
            curObj->age[i]           = 1;
            curObj->visible[i]       = 0;
            curObj->invisibleSpan[i] = 1;
            curObj->dx[i]            = 0.0;
            curObj->dy[i]            = 0.0;
        }
    }

    // Carry the unmatched object in the previous frame to the current frame
    for (j = 0; j < numPrevObject; j++)
    {
        if (prevObj->correspondence[j] != CORRESPONDENCE_FOUND && 
            prevObj->invisibleSpan[j] < 3)
        {
            // copy object info
            PTK_Alg_StereoOG_copyObjectInfo(prevObj, curObj, j, curObj->numObject);

            // reset some variables
            curObj->centerX[curObj->numObject]         = prevObj->centerX[j] + prevObj->dx[j];
            curObj->centerY[curObj->numObject]         = prevObj->centerY[j] + prevObj->dy[j];
            curObj->age[curObj->numObject]             = prevObj->age[j] + 1;
            curObj->visible[curObj->numObject]         = 0;
            curObj->invisibleSpan[curObj->numObject]   = prevObj->invisibleSpan[j] + 1;

            curObj->numObject++;
        }
    }
}


void PTK_Alg_StereoOG_mergeObjectsTemporal(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                                          int8_t                          direction)
{
    int16_t                   i, j;
    int16_t                   corr;
    int16_t                   refCorr;
    int16_t                   numCurObject;
    int16_t                   numRefObject;
    int16_t                   cIdx;
    int32_t                   gridSize;

    float                     distance;
    float                     minDistance;
    float                     distTh;

    // current object's width and height on OG map
    int16_t                   oWidth, oHeight;
    // reference object's width and height on OG map
    int16_t                   rWidth, rHeight;
    // combined object's width and height on OG map
    int16_t                   cWidth, cHeight;

    PTK_Alg_StereoOG_Object * tmpObj;
    PTK_Alg_StereoOG_Object * newObj;
    PTK_Alg_StereoOG_Object * curObj;
    PTK_Alg_StereoOG_Object * refObj;

    uint16_t                  numValidObject = 0;
    uint8_t                 * validFlag;
    uint8_t                 * tempCorr;

    gridSize          = cntxt->params.ogParams.xGridSize;
    if (cntxt->params.cfgParams.objectDistanceMode == 0)
    {
        distTh        = 3000;
    } else
    {
        distTh        = 8000;
    }

    if (direction == DIR_BACKWARD)
    {
        curObj        = cntxt->curObject;
        refObj        = cntxt->prevObject;
    } else
    {
        curObj        = cntxt->prevObject;
        refObj        = cntxt->curObject;
    }
    newObj            = cntxt->auxObject;
    numCurObject      = curObj->numObject;
    numRefObject      = refObj->numObject;

    // Reuse "merged" field
    validFlag         = newObj->merged;
    tempCorr          = curObj->merged;

    // 1. add the objects having correspondence to newObj
    newObj->numObject = 0;

    for (i = 0; i < numCurObject; i++)
    {
        corr = curObj->correspondence[i];

        refCorr = -1;
        if (corr != CORRESPONDENCE_INIT)
        {
            refCorr = refObj->correspondence[corr];
        }

        if ((corr != CORRESPONDENCE_INIT) && (i == refCorr))
        {
            validFlag[i] = 1;
            
            PTK_Alg_StereoOG_copyObjectInfo(curObj, newObj, i, newObj->numObject);

            // change correspondence index of the previous frames' object
            // since the index of the current frames' object chanages
            tempCorr[i] =  newObj->numObject;

            newObj->correspondence[newObj->numObject] = curObj->correspondence[i];
            newObj->numObject++;
        } else
        {
            validFlag[i] = 0;
        }
    }

    // number of objects that have correspondences in the previous frame
    numValidObject = newObj->numObject;

    // 2. Merge the objects not having correspondence to the object having correspondence
    for (i = 0; i < numCurObject; i++)
    {
        corr = curObj->correspondence[i];
        refCorr = -1;
        if (corr != CORRESPONDENCE_INIT)
        {
            refCorr = refObj->correspondence[corr];
        }

        if (validFlag[i] == 0)
        {
            minDistance = distTh;
            cIdx        = -1;

            // search among the valid objects
            for (j = 0; j < numValidObject; j++)
            {
                distance = PTK_Alg_StereoOG_computeDistanceObjects(
                               cntxt->params.cfgParams.objectDistanceMode,
                               curObj, 
                               newObj, 
                               i, 
                               j);

                if (distance*gridSize < minDistance)
                {
                    minDistance = distance*gridSize;
                    cIdx = j;
                }
            }

            if (cIdx == -1) 
            {
                // No object to merge with
                PTK_Alg_StereoOG_copyObjectInfo(curObj, newObj, i, newObj->numObject);

                // change correspondence index of the previous frames' object
                // since the index of the current frames' object chanages
                tempCorr[i] = newObj->numObject;

                newObj->correspondence[newObj->numObject] = curObj->correspondence[i];
                newObj->numObject++;
            } 
            else
            {
                corr = curObj->correspondence[cIdx];

                // check if the current object overlap with the corresponding object 
                // in the previous frame
                oWidth    = curObj->maxObjXGrid[i] - curObj->minObjXGrid[i] + 1;
                oHeight   = curObj->maxObjYGrid[i] - curObj->minObjYGrid[i] + 1;

                rWidth    = refObj->maxObjXGrid[corr] - refObj->minObjXGrid[corr] + 1;
                rHeight   = refObj->maxObjYGrid[corr] - refObj->minObjYGrid[corr] + 1;

                cWidth    = MAX(curObj->maxObjXGrid[i], refObj->maxObjXGrid[corr] + refObj->dx[corr]) - 
                            MIN(curObj->minObjXGrid[i], refObj->minObjXGrid[corr] + refObj->dx[corr]) + 1;
                cHeight   = MAX(curObj->maxObjYGrid[i], refObj->maxObjYGrid[corr] + refObj->dy[corr]) - 
                            MIN(curObj->minObjYGrid[i], refObj->minObjYGrid[corr] + refObj->dy[corr]) + 1;

                // if overlap, merge two objects in the current frame
                if ((cWidth < oWidth + rWidth) && (cHeight < oHeight + rHeight))
                {
                    // Class id does not change
                    newObj->count[cIdx]       = newObj->count[cIdx] + curObj->count[i];
                    newObj->maxHeight[cIdx]   = MAX(newObj->maxHeight[cIdx], curObj->maxHeight[i]);
                    newObj->minHeight[cIdx]   = MIN(newObj->minHeight[cIdx], curObj->minHeight[i]);

                    if (newObj->maxObjXGrid[cIdx] < curObj->maxObjXGrid[i])
                    {
                        newObj->maxObjXGrid[cIdx]     = curObj->maxObjXGrid[i];
#if QUAD_LATERAL_BB
                        newObj->maxObjXGridYPos[cIdx] = curObj->maxObjXGridYPos[i];
#endif
                    }

                    if (newObj->minObjXGrid[cIdx] > curObj->minObjXGrid[i])
                    {
                        newObj->minObjXGrid[cIdx]     = curObj->minObjXGrid[i];
#if QUAD_LATERAL_BB
                        newObj->minObjXGridYPos[cIdx] = curObj->minObjXGridYPos[i];
#endif
                    }

                    if (newObj->maxObjYGrid[cIdx] < curObj->maxObjYGrid[i])
                    {
                        newObj->maxObjYGrid[cIdx]     = curObj->maxObjYGrid[i];
#if QUAD_LATERAL_BB
                        newObj->maxObjYGridXPos[cIdx] = curObj->maxObjYGridXPos[i];
#endif
                    }

                    if (newObj->minObjYGrid[cIdx] > curObj->minObjYGrid[i])
                    {
                        newObj->minObjYGrid[cIdx]     = curObj->minObjYGrid[i];
#if QUAD_LATERAL_BB
                        newObj->minObjYGridXPos[cIdx] = curObj->minObjYGridXPos[i];
#endif
                    }

                    newObj->width[cIdx]   =  newObj->maxObjXGrid[cIdx] - 
                                                          newObj->minObjXGrid[cIdx] + 1;
                    newObj->height[cIdx]  =  newObj->maxObjYGrid[cIdx] - 
                                                          newObj->minObjYGrid[cIdx] + 1;
                    newObj->centerX[cIdx] = (newObj->maxObjXGrid[cIdx] + 
                                                          newObj->minObjXGrid[cIdx])*1.0/2;
                    newObj->centerY[cIdx] = (newObj->maxObjYGrid[cIdx] + 
                                                          newObj->minObjYGrid[cIdx])*1.0/2;

                    if (direction == DIR_BACKWARD)
                    { 
                        newObj->age[cIdx]           = 1;
                        newObj->visible[cIdx]       = 1;
                        newObj->invisibleSpan[cIdx] = 0;

                        newObj->dx[cIdx]            = 0.0;
                        newObj->dy[cIdx]            = 0.0;
                    } else
                    {
                        if (curObj->age[i] > newObj->age[cIdx])
                        {
                            newObj->age[cIdx] = curObj->age[i];
                        }

                        if (curObj->invisibleSpan[i] < newObj->invisibleSpan[cIdx])
                        {
                            newObj->invisibleSpan[cIdx] = curObj->invisibleSpan[i];
                        }
                    }

                    // change correspondence index of the previous frames' object
                    // since the index of the current frames' object chanages
                    tempCorr[i] = cIdx;
                } else
                {
                    PTK_Alg_StereoOG_copyObjectInfo(curObj, newObj, i, newObj->numObject);
    
                    // change correspondence index of the previous frames' object
                    // since the index of the current frames' object chanages
                    tempCorr[i] = newObj->numObject;

                    newObj->correspondence[newObj->numObject] = curObj->correspondence[i];
                    newObj->numObject++;
                }
            }
        }
    }

    // change correspondence index of the previous frames' object
    // since the index of the current frames' object chanages
    for (j = 0; j < numRefObject; j++)
    {
        if (refObj->correspondence[j] >= 0)
        {
            refObj->correspondence[j] = tempCorr[refObj->correspondence[j]] ;
        }
    }

    // 3. Swap pointer so that cntxt->curObject points to the merged objects
    if (direction == DIR_BACKWARD)
    {
        tmpObj            = cntxt->curObject;
        cntxt->curObject  = cntxt->auxObject;
        cntxt->auxObject  = tmpObj;
    } else
    {
        tmpObj            = cntxt->prevObject;
        cntxt->prevObject = cntxt->auxObject;
        cntxt->auxObject  = tmpObj;
    }
}

float PTK_Alg_StereoOG_computeDistanceObjects(uint8_t                   mode,
                                              PTK_Alg_StereoOG_Object *obj1,
                                              PTK_Alg_StereoOG_Object *obj2,
                                              int16_t                  idx1,
                                              int16_t                  idx2)
{
    float distance = 0.0;

    if (mode == 0)
    {
        float xdist = obj1->centerX[idx1] + obj1->dx[idx1] - obj2->centerX[idx2] - obj2->dx[idx2];
        float ydist = obj1->centerY[idx1] + obj1->dy[idx1] - obj2->centerY[idx2] - obj2->dy[idx2];

        // distance based on a center
        distance = sqrt((xdist * xdist) + (ydist * ydist));
    } else
    {    // distance based on four corners
        distance += abs(obj1->maxObjXGrid[idx1] + obj1->dx[idx1] - obj2->maxObjXGrid[idx2] - obj2->dx[idx2]);
        distance += abs(obj1->minObjXGrid[idx1] + obj1->dx[idx1] - obj2->minObjXGrid[idx2] - obj2->dx[idx2]);
        distance += abs(obj1->maxObjYGrid[idx1] + obj1->dy[idx1] - obj2->maxObjYGrid[idx2] - obj2->dy[idx2]);
        distance += abs(obj1->minObjYGrid[idx1] + obj1->dy[idx1] - obj2->minObjYGrid[idx2] - obj2->dy[idx2]);
    }

    return distance;
}

/**
 *  Create objects' bonding boxes from OG map
 */
void PTK_Alg_StereoOG_createBB(PTK_Alg_StereoOG_CreateOGObj * cntxt,
                               PTK_Alg_StereoOG_Object      * object,
                               PTK_Alg_StereoOG_obs3DBox    * obsBB)
{
    int32_t i;

    uint8_t classId;
    int16_t height;

    int16_t minx, maxx;
    int16_t miny, maxy;

    int16_t n;

#if QUAD_LATERAL_BB
    int16_t minx_y, maxx_y;
    int16_t miny_x, maxy_x;
#endif

    PTK_Alg_StereoOG_CreateOGParams    * ogParams  = &cntxt->params.ogParams;
    PTK_Alg_StereoOG_CreateOGCamParams * camParams = &cntxt->params.camParams;
    PTK_Alg_StereoOG_BoxProp           * boxProp   =
        (PTK_Alg_StereoOG_BoxProp *) PTK_Alg_StereOG_get3DBB(obsBB);

    int16_t imWidth   = cntxt->params.cfgParams.width;
    int16_t imHeight  = cntxt->params.cfgParams.height;
    float   f         = camParams->f;
    float   dcx       = camParams->dcx;
    float   dcy       = camParams->dcy;
    float   cosPitch  = camParams->cosPitch;
    float   sinPitch  = camParams->sinPitch;

    // 3D point
    // P4 P3
    // P1 P2
    PTK_Point P1, P2, P3, P4;

    obsBB->numObject = 0;
    for (i = 0; i < object->numObject; i++)
    {
        if ((cntxt->params.cfgParams.enableTemporalObjMerge) &&
            (object->age[i] < 3 || object->visible[i] == 0))
        {
            continue;
        }

        classId = object->classId[i];
        height  = object->maxHeight[i];
        minx    = object->minObjXGrid[i];
        maxx    = object->maxObjXGrid[i];
        miny    = object->minObjYGrid[i];
        maxy    = object->maxObjYGrid[i];

#if QUAD_LATERAL_BB
        minx_y  = object->minObjXGridYPos[i];
        maxx_y  = object->maxObjXGridYPos[i];
        miny_x  = object->minObjYGridXPos[i];
        maxy_x  = object->maxObjYGridXPos[i];
#endif

        /* Front Boxes */
        // Front box - 3D points
#if QUAD_LATERAL_BB
        P1.x = ogParams->xMinRange + minx * ogParams->xGridSize;
        P1.y = camParams->camHeight;
        P1.z = ogParams->yMinRange + minx_y * ogParams->yGridSize;

        P2.x = ogParams->xMinRange + (miny_x+1) * ogParams->xGridSize;
        P2.y = P1.y;
        P2.z = ogParams->yMinRange + miny * ogParams->yGridSize;

        P3.x = P2.x;
        P3.y = -height + camParams->camHeight;
        P3.z = P2.z;

        P4.x = P1.x;
        P4.y = P3.y;
        P4.z = P1.z;
#else
        P1.x = ogParams->xMinRange + minx * ogParams->xGridSize;
        P1.y = camParams->camHeight;
        P1.z = ogParams->yMinRange + miny * ogParams->yGridSize;

        P2.x = ogParams->xMinRange + (maxx+1) * ogParams->xGridSize;
        P2.y = P1.y;
        P2.z = P1.z;

        P3.x = P2.x;
        P3.y = -height + camParams->camHeight;
        P3.z = P2.z;

        P4.x = P1.x;
        P4.y = P3.y;
        P4.z = P3.z;
#endif

        n = obsBB->numObject;

        // Front box - 2D points on image
        boxProp[n].classId    = classId;

        boxProp[n].pf1x       = (f * P1.x) / P1.z + dcx;
        boxProp[n].pf1y       = (f * (P1.y*cosPitch - P1.z*sinPitch) / (P1.y*sinPitch + P1.z*cosPitch)) + dcy;
        boxProp[n].pf1x       = MAX(MIN(boxProp[n].pf1x, imWidth-1), 0);
        boxProp[n].pf1y       = MAX(MIN(boxProp[n].pf1y, imHeight-1), 0);
        boxProp[n].frontDepth = P1.z;

        boxProp[n].pf2x       = (f * P2.x) / P2.z + dcx;
        boxProp[n].pf2y       = (f * (P2.y*cosPitch - P2.z*sinPitch) / (P2.y*sinPitch + P2.z*cosPitch)) + dcy;
        boxProp[n].pf2x       = MAX(MIN(boxProp[n].pf2x, imWidth-1), 0);
        boxProp[n].pf2y       = MAX(MIN(boxProp[n].pf2y, imHeight-1), 0);

        boxProp[n].pf3x       = (f * P3.x) / P3.z + dcx;
        boxProp[n].pf3y       = (f * (P3.y*cosPitch - P3.z*sinPitch) / (P3.y*sinPitch + P3.z*cosPitch)) + dcy;
        boxProp[n].pf3x       = MAX(MIN(boxProp[n].pf3x, imWidth-1), 0);
        boxProp[n].pf3y       = MAX(MIN(boxProp[n].pf3y, imHeight-1), 0);

        boxProp[n].pf4x       = (f * P4.x) / P4.z + dcx;
        boxProp[n].pf4y       = (f * (P4.y*cosPitch - P4.z*sinPitch) / (P4.y*sinPitch + P4.z*cosPitch)) + dcy;
        boxProp[n].pf4x       = MAX(MIN(boxProp[n].pf4x, imWidth-1), 0);
        boxProp[n].pf4y       = MAX(MIN(boxProp[n].pf4y, imHeight-1), 0);

        /* Rear Boxes */
        // Rear Box - 3D points
#if QUAD_LATERAL_BB
        P1.x = ogParams->xMinRange + maxy_x * ogParams->xGridSize;
        P1.y = camParams->camHeight;
        P1.z = ogParams->yMinRange + (maxy+1) * ogParams->yGridSize;

        P2.x = ogParams->xMinRange + (maxx+1) * ogParams->xGridSize;
        P2.y = P1.y;
        P2.z = ogParams->yMinRange + (maxx_y+1) * ogParams->yGridSize;;

        P3.x = P2.x;
        P3.y = -height + camParams->camHeight;
        P3.z = P2.z;

        P4.x = P1.x;
        P4.y = P3.y;
        P4.z = P1.z;
#else
        P1.x = ogParams->xMinRange + minx * ogParams->xGridSize;
        P1.y = camParams->camHeight;
        P1.z = ogParams->yMinRange + (maxy+1) * ogParams->yGridSize;

        P2.x = ogParams->xMinRange + (maxx+1) * ogParams->xGridSize;
        P2.y = P1.y;
        P2.z = P1.z;

        P3.x = P2.x;
        P3.y = -height + camParams->camHeight;
        P3.z = P2.z;

        P4.x = P1.x;
        P4.y = P3.y;
        P4.z = P3.z;
#endif

        // Rear box - 2D points on image
        boxProp[n].pr1x      = (f * P1.x) / P1.z + dcx;
        boxProp[n].pr1y      = (f * (P1.y*cosPitch - P1.z*sinPitch) / (P1.y*sinPitch + P1.z*cosPitch)) + dcy;
        boxProp[n].pr1x      = MAX(MIN(boxProp[n].pr1x, imWidth-1), 0);
        boxProp[n].pr1y      = MAX(MIN(boxProp[n].pr1y, imHeight-1), 0);
        boxProp[n].rearDepth = P1.z;

        boxProp[n].pr2x      = (f * P2.x) / P2.z + dcx;
        boxProp[n].pr2y      = (f * (P2.y*cosPitch - P2.z*sinPitch) / (P2.y*sinPitch + P2.z*cosPitch)) + dcy;
        boxProp[n].pr2x      = MAX(MIN(boxProp[n].pr2x, imWidth-1), 0);
        boxProp[n].pr2y      = MAX(MIN(boxProp[n].pr2y, imHeight-1), 0);

        boxProp[n].pr3x      = (f * P3.x) / P3.z + dcx;
        boxProp[n].pr3y      = (f * (P3.y*cosPitch - P3.z*sinPitch) / (P3.y*sinPitch + P3.z*cosPitch)) + dcy;
        boxProp[n].pr3x      = MAX(MIN(boxProp[n].pr3x, imWidth-1), 0);
        boxProp[n].pr3y      = MAX(MIN(boxProp[n].pr3y, imHeight-1), 0);

        boxProp[n].pr4x      = (f * P4.x) / P4.z + dcx;
        boxProp[n].pr4y      = (f * (P4.y*cosPitch - P4.z*sinPitch) / (P4.y*sinPitch + P4.z*cosPitch)) + dcy;
        boxProp[n].pr4x      = MAX(MIN(boxProp[n].pr4x, imWidth-1), 0);
        boxProp[n].pr4y      = MAX(MIN(boxProp[n].pr4y, imHeight-1), 0);

        // Occupied grid position in mm
        boxProp[n].topLeftGridX     = ogParams->xMinRange + minx * ogParams->xGridSize;
        boxProp[n].topLeftGridY     = ogParams->yMinRange + (maxy+1) * ogParams->yGridSize;

        boxProp[n].bottomRightGridX = ogParams->xMinRange + (maxx+1) * ogParams->xGridSize;
        boxProp[n].bottomRightGridY = ogParams->yMinRange + miny * ogParams->yGridSize;

        // increase the number of objects
        obsBB->numObject++;
    }
}

/**
 * Copy object from one struct to another 
 */
void PTK_Alg_StereoOG_copyObjectInfo(PTK_Alg_StereoOG_Object *srcObj,
                                     PTK_Alg_StereoOG_Object *dstObj,
                                     int16_t                  srcIdx,
                                     int16_t                  dstIdx)
{
    dstObj->classId[dstIdx]         = srcObj->classId[srcIdx];
    dstObj->count[dstIdx]           = srcObj->maxHeight[srcIdx];
    dstObj->maxHeight[dstIdx]       = srcObj->maxHeight[srcIdx];
    dstObj->minHeight[dstIdx]       = srcObj->minHeight[srcIdx];
    dstObj->maxObjXGrid[dstIdx]     = srcObj->maxObjXGrid[srcIdx];
    dstObj->minObjXGrid[dstIdx]     = srcObj->minObjXGrid[srcIdx];
    dstObj->maxObjYGrid[dstIdx]     = srcObj->maxObjYGrid[srcIdx];
    dstObj->minObjYGrid[dstIdx]     = srcObj->minObjYGrid[srcIdx];

#if QUAD_LATERAL_BB
    dstObj->maxObjXGridYPos[dstIdx] = srcObj->maxObjXGridYPos[srcIdx];
    dstObj->minObjXGridYPos[dstIdx] = srcObj->minObjXGridYPos[srcIdx];
    dstObj->maxObjYGridXPos[dstIdx] = srcObj->maxObjYGridXPos[srcIdx];
    dstObj->minObjYGridXPos[dstIdx] = srcObj->minObjYGridXPos[srcIdx];
#endif

    // Needed for spatial merge
    dstObj->width[dstIdx]           = srcObj->width[srcIdx];
    dstObj->height[dstIdx]          = srcObj->height[srcIdx];
    dstObj->centerX[dstIdx]         = srcObj->centerX[srcIdx];
    dstObj->centerY[dstIdx]         = srcObj->centerY[srcIdx];

    // Needed for temporal merge
    dstObj->age[dstIdx]             = srcObj->age[srcIdx];
    dstObj->visible[dstIdx]         = srcObj->visible[srcIdx];
    dstObj->invisibleSpan[dstIdx]   = srcObj->invisibleSpan[srcIdx];
    dstObj->dx[dstIdx]              = srcObj->dx[srcIdx];
    dstObj->dy[dstIdx]              = srcObj->dy[srcIdx];
}


/**
 *  Reset OG map
 */
void PTK_Alg_StereoOG_resetOG(PTK_Alg_StereoOG_CreateOGObj * cntxt)
{
    PTK_Alg_StereoOG_GridProp        * gridProp = &cntxt->gridProp;
    PTK_Alg_StereoOG_OccupancyGrid   * og       = &cntxt->og;
    PTK_Alg_StereoOG_Object          * obj      = cntxt->curObject;
    PTK_Alg_StereoOG_OGConn          * ogConn   = &cntxt->ogConn;
    PTK_Alg_StereoOG_CreateOGParams  * ogParams = &cntxt->params.ogParams;

    int32_t i;
    int32_t totalNumGrid = ogParams->xGridNum *  ogParams->yGridNum;

    memset(gridProp->numClass,  0, totalNumGrid);

    memset(og->occupied,  0, totalNumGrid);
    for (i = 0; i <  totalNumGrid; i++)
    {
        og->maxHeight[i] = 0;
        og->minHeight[i] = 0;
        og->classId[i]   = 255;
        og->count[i]     = 0;
    }


    obj->numObject = 0;
    for (i = 0; i < ogParams->maxNumObject; i++)
    {
        //obj->classId[i]     = 255;
        obj->count[i]       = 0;
        obj->maxHeight[i]   = -5000;
        obj->minHeight[i]   =  5000;
        obj->maxObjXGrid[i] = 0;
        obj->minObjXGrid[i] = ogParams->xGridNum;
        obj->maxObjYGrid[i] = 0;
        obj->minObjYGrid[i] = ogParams->yGridNum;
    }

    ogConn->numGridInQueue = 0;
    for (i = 0; i < totalNumGrid; i++)
    {
        ogConn->visited[i] = 0;
        ogConn->neighborQueue[i] = -1;
        ogConn->neighborIndex[i] = -1;
    }
}


void PTK_Alg_StereoOG_resetTemporalReference(PTK_Alg_StereoOG_CreateOGObj * cntxt)
{
    cntxt->prevObject->numObject = -1;
}
