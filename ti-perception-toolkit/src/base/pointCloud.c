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

/**
 * @file point_cloud.c
 * @brief Implementation of functions used to manipulate point clouds
 */

#include <float.h>
#include <string.h>

#include <perception/base/core.h>
#include <perception/base/pointCloud.h>

#define align(x,b) ((x+(b-1))&~(b-1))

static void scale_same(PTK_PointCloud  *cloud,
                       float            scale)
{
    PTK_Point  *points;
    uint32_t    i;

    points = PTK_PointCloud_getPoints(cloud);

    for (i = 0; i < cloud->numPoints; ++i)
    {
        PTK_Vector_scale(&points[i], &points[i], scale);
    }
}

static void scale_different(PTK_PointCloud *__restrict          dst,
                            const PTK_PointCloud *__restrict    src,
                            float                               scale)
{
    PTK_Point  *srcPts;
    uint32_t    i;

    if (!PTK_PointCloud_hasSpace(dst, src->numPoints))
    {
        return;
    }

    srcPts = PTK_PointCloud_getPoints(src);

    for (i = 0; i < src->numPoints; ++i)
    {
        PTK_Point   temp;

        PTK_Vector_scale(&temp, &srcPts[i], scale);
        PTK_PointCloud_add(dst, &temp);
    }
}

static void transform_same(PTK_PointCloud              *cloud,
                           const PTK_RigidTransform    *M)
{
    PTK_Point  *points;
    uint32_t    i;

    points = PTK_PointCloud_getPoints(cloud);

    for (i = 0; i < cloud->numPoints; ++i)
    {
        PTK_Point_transform(&points[i], &points[i], M);
    }
}

static void transform_different(PTK_PointCloud *__restrict          dst,
                                const PTK_PointCloud *__restrict    src,
                                const PTK_RigidTransform           *M)
{
    PTK_Point  *srcPts;
    uint32_t    i;

    if (!PTK_PointCloud_hasSpace(dst, src->numPoints))
    {
        return;
    }

    srcPts = PTK_PointCloud_getPoints(src);

    for (i = 0; i < src->numPoints; ++i)
    {
        PTK_Point temp;
        PTK_Point_transform(&temp, &srcPts[i], M);
        PTK_PointCloud_add(dst, &temp);
    }
}

/**
 * Get the required number of bytes to pass to a separate allocator (e.g. Utils_memAlloc) to create
 * memory for a point cloud able to contain the given number of points. The size calculated is
 * suitable for packing into an array of point clouds
 */
uint32_t PTK_PointCloud_getSize(const PTK_PointCloudConfig *cfg)
{
    uint32_t size = 0;

    // Need space for the whole struct
    size += sizeof(PTK_PointCloud);
    size = align(size, 8);

    // Add storage for all points
    size += cfg->maxPoints * sizeof(PTK_Point);
    size = align(size, 8);

    return size;
}

/**
 * Initialize a point cloud structure using the given chunk of memory
 */
PTK_PointCloud *PTK_PointCloud_init(uint8_t                    *memPtr,
                                    const PTK_PointCloudConfig *cfg)
{
    PTK_PointCloud *pc = (PTK_PointCloud *) memPtr;
    uint32_t        offset = 0;

    // @todo assertion here: assert((memPtr & 3) == 0) to test for 4 byte alignment

    offset = sizeof(PTK_PointCloud);
    offset = align(offset, 8);
    pc->pointOffset = offset;
    pc->config = *cfg;

    PTK_PointCloud_clear(pc);

    return pc;
}

/**
 * Clears all of the points in the point cloud and resets used sizes to zero. The memory, however,
 * remains allocated exactly as it was before
 */
void PTK_PointCloud_clear(PTK_PointCloud *pc)
{
    pc->numPoints = 0;
}

/**
 * Test if there is room for N points in the point cloud
 */
uint32_t PTK_PointCloud_hasSpace(const PTK_PointCloud  *pc,
                                 uint32_t               points)
{
    if ((pc->config.maxPoints - pc->numPoints) >= points)
    {
        return 1;
    }

    return 0;
}

/**
 * Copy the given point into the point cloud at the end and mark it valid
 */
uint32_t PTK_PointCloud_add(PTK_PointCloud     *pc,
                            const PTK_Point    *xyzw)
{
    PTK_Point  *points;
    uint32_t    id;

    points = PTK_PointCloud_getPoints(pc);

    if (pc->numPoints == pc->config.maxPoints)
    {
        return PTK_POINTCLOUD_INVALID_POINT;
    }

    id = pc->numPoints;
    memcpy(&points[id], xyzw, sizeof(PTK_Point));
    pc->numPoints += 1;

    return id;
}

/**
 * Copy points from the given array to the end of the cloud and mark all valid
 */
uint32_t PTK_PointCloud_addv(PTK_PointCloud    *pc,
                             const PTK_Point   *xyzw,
                             uint32_t           numPoints)
{
    PTK_Point  *dst;
    uint32_t    start;

    if (!PTK_PointCloud_hasSpace(pc, numPoints))
    {
        return PTK_POINTCLOUD_INVALID_POINT;
    }

    dst = PTK_PointCloud_getPoints(pc);

    start = pc->numPoints;
    memcpy(&dst[start], xyzw, numPoints*sizeof(PTK_Point));
    pc->numPoints += numPoints;

    return start;
}

/**
 * Accessor for the numPoints field to provide an abstraction
 */
uint32_t PTK_PointCloud_getPointCount(const PTK_PointCloud *pc)
{
    return pc->numPoints;
}

uint32_t PTK_PointCloud_getPoint(const PTK_PointCloud  *pc,
                                 uint32_t               id,
                                 PTK_Point             *dst)
{
    if (id < pc->numPoints)
    {
        PTK_Point  *src;

        src = PTK_PointCloud_getPoints(pc);

        memcpy(dst, &src[id], sizeof(PTK_Point));
        return 1;
    }

    return 0;
}

/**
 * Get an immutable reference to a point at the given offset within the given cloud.
 */
const PTK_Point *PTK_PointCloud_crefPoint(const PTK_PointCloud *pc,
                                          uint32_t              id)
{
    if (id < pc->numPoints)
    {
        PTK_Point  *points;

        points = (PTK_Point*)((uint8_t*)pc + pc->pointOffset);
        points = PTK_PointCloud_getPoints(pc);

        return &points[id];
    }

    return NULL;
}

/**
 * Add one or more tag bits for the specified point index
 */
void PTK_PointCloud_tag(PTK_PointCloud *pc,
                        uint32_t        id,
                        uint32_t        tag)
{
    PTK_Point *pt = PTK_PointCloud_refPoint(pc, id);

    if (pt)
    {
        PTK_Point_tag(pt, tag);
    }
}

/**
 * Remove one or more tag bits for the specified point index
 */
void PTK_PointCloud_untag(PTK_PointCloud   *pc,
                          uint32_t          id,
                          uint32_t          tag)
{
    PTK_Point *pt = PTK_PointCloud_refPoint(pc, id);

    if (pt)
    {
        PTK_Point_untag(pt, tag);
    }
}

/**
 * Check if all of the given tag bits are set, including a bounds check
 */
uint32_t PTK_PointCloud_isTagged(const PTK_PointCloud  *pc,
                                 uint32_t               id,
                                 uint32_t               tag)
{
    if (id < pc->numPoints)
    {
        PTK_Point  *points;

        points = PTK_PointCloud_getPoints(pc);

        return PTK_Point_isTagged(&points[id], tag);
    }

    return 0;
}

/**
 * Raw read of the tag bits for purposes not covered by the rest of our API
 */
uint32_t PTK_PointCloud_readTag(const PTK_PointCloud   *pc,
                                uint32_t                id)
{
    if (id < pc->numPoints)
    {
        PTK_Point  *points;

        points = PTK_PointCloud_getPoints(pc);

        return points[id].meta.f;
    }

    return 0;
}

/**
 * Copies points from the source cloud to the destination cloud,
 * but only if they all fit
 */
uint32_t PTK_PointCloud_copy(PTK_PointCloud        *dst,
                             const PTK_PointCloud  *src)
{
    PTK_Point  *points;

    /* Copy metadata. */
    PTK_PointCloud_copyMetaData(dst, src);

    points = PTK_PointCloud_getPoints(src);

    return PTK_PointCloud_addv(dst, points, src->numPoints);
}

/**
 * Transform points in the source cloud to the destination cloud by using the given
 * rigid transformation. The source and destination cloud may be the same
 */
void PTK_PointCloud_transform(PTK_PointCloud           *dst,
                              PTK_PointCloud           *src,
                              const PTK_RigidTransform *M)
{
    if (dst != src)
    {
        transform_different(dst, src, M);
    }
    else
    {
        transform_same(src, M);
    }
}

void PTK_PointCloud_scale(PTK_PointCloud   *dst,
                          PTK_PointCloud   *src,
                          float             scale)
{
    if (dst == src)
    {
        scale_same(dst, scale);
    }
    else
    {
        scale_different(dst, src, scale);
    }
}

/**
 * Find minimum and maximum of quantity Q among all valid points in the cloud where
 * Q is the dot product between a point (x,y,z,w) and 4-vector 'basis'. For example, if
 * basis = [1, 0, 0, 0], the function finds the minimum and maximum x-value.
 */
void PTK_PointCloud_getMinMax(const PTK_PointCloud *pc,
                              const PTK_Point      *basis,
                              float                *minVal,
                              float                *maxVal,
                              uint32_t             *minId,
                              uint32_t             *maxId)
{
    PTK_Point  *points;
    float       curMin;
    float       curMax;
    uint32_t    curMinId;
    uint32_t    curMaxId;
    uint32_t    pp;

    points   = PTK_PointCloud_getPoints(pc);
    curMin   = FLT_MAX;
    curMax   = FLT_MIN;
    curMinId = 0;
    curMaxId = 0;

    for (pp = 0; pp < pc->numPoints; pp++)
    {
        float dotProd = PTK_Vector_dot4(&points[pp], basis);

        if (dotProd < curMin)
        {
            curMin = dotProd;
            curMinId = pp;
        }

        if (dotProd > curMax)
        {
            curMax = dotProd;
            curMaxId = pp;
        }
    }

    *minVal = curMin;
    *maxVal = curMax;
    *minId  = curMinId;
    *maxId  = curMaxId;
}
