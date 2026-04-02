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
#pragma once
#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

/**
 * @file pointCloud.h
 * @brief Declaration of structures and functions used to manipulate point clouds
 */

#include <stdint.h>
#include <string.h>
#include <perception/base/point.h>
#include <perception/base/rigidTransform.h>

/**
 * \defgroup group_ptk_point_cloud PTK Point Cloud
 * \ingroup group_ptk_base
 *
 * A PTK_PointCloud is used store a collection of 3D points. It is independent
 * of source of the points and does not store sensor-specific metadata. It does,
 * however, provide access to the tag metadata field on each point which allows
 * for up to 32 application- or algorithm-specific tags to be created. Points
 * within a point cloud are identified uniquely by integers, which are used for
 * reading points and their tags. The identifying integers are assigned
 * sequentially as points are added to a point cloud.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Macro for getting a pointer to the point representation.
 *
 * \ingroup group_ptk_point
 */
#define PTK_PointCloud_getPoints(pc) (PTK_Point*)((uint8_t*)pc + pc->pointOffset)

/**
 * \brief Constant for marking invalid result.
 *
 * \ingroup group_ptk_point
 */
#define PTK_POINTCLOUD_INVALID_POINT (~0U)

/**
 * \brief Definition of vector meta data.
 *
 * \ingroup group_ptk_point_cloud
 *
 * A PTK_PointCloud is used store a collection of 3D points. It is independent
 * of source of the points and does not store sensor-specific metadata. It does,
 * however, provide access to the tag metadata field on each point which allows
 * for up to 32 application- or algorithm-specific tags to be created. Points
 * within a point cloud are identified uniquely by integers, which are used for
 * reading points and their tags. The identifying integers are assigned
 * sequentially as points are added to a point cloud.
 */
typedef struct
{
    /** Maximum number of points for which storage space will be available in
     *  the resulting PTK_PointCloud.
     */
    uint32_t maxPoints;

} PTK_PointCloudConfig;

typedef struct
{
    /** */
    PTK_PointCloudConfig    config;

    /** */
    uint32_t                numPoints;

    /** */
    uint32_t                pointOffset;

    /** */
    uint32_t                dummy;

} PTK_PointCloud;

/**
 * \brief Compute how much memory is required to store a complete PTK_PointCloud
 *        struct matching the given configuration.
 *
 * \param [in] cfg Configuration structure defining the point cloud.
 *
 * \return Amount of memory required, in bytes.
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_getSize(const PTK_PointCloudConfig *cfg);

/**
 * \brief Initialize an externally allocated piece of memory to be a valid
 *        PTK_PointCloud.
 *
 * \param [out] memPtr Externally allocated memory of at least
 *                     PTK_PointCloud_getSize() bytes.
 *
 * \param [in] cfg Configuration structure to initialize from.
 *
 * \return Pointer to newly initialized PTK_PointCloud instance.
 *
 * \ingroup group_ptk_point_cloud
 */
PTK_PointCloud *PTK_PointCloud_init(uint8_t                    *memPtr,
                                    const PTK_PointCloudConfig *cfg);

/**
 * \brief Clear a PTK_PointCloud by setting used point count to zero..
 *
 * \param [out] pc Point cloud to modify.
 *
 * \ingroup group_ptk_point_cloud
 */
void PTK_PointCloud_clear(PTK_PointCloud *pc);

/**
 * \brief Check if the given point cloud has enough space to accomodate the
 *        addition of points additional PTK_Points.
 *
 * \param [out] pc The point cloud to check.
 *
 * \param [in] points The number of points to check for.
 *
 * \return
 *         - 1 if there is enough room
 *         - 0, otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_hasSpace(const PTK_PointCloud  *pc,
                                 uint32_t               points);

/**
 * \brief Add a copy of the given point to the point cloud, if there is room.
 *
 * \param [in] pc Add to this point cloud.
 *
 * \param [in] xyzw Point to add.
 *
 * \return
 *         - 1 if there is enough room
 *         - PTK_POINTCLOUD_INVALID_POINT, otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_add(PTK_PointCloud     *pc,
                            const PTK_Point    *xyzw);

/**
 * \brief Add a copy of each of the points in the array pv to the given point
 *        cloud, if there is enough space. If not, no points are copied. The
 *        IDs of the copied points are sequential after the first one, so only
 *        the first ID is returned.
 *
 * \param [out] pc Add points to this point cloud.
 *
 * \param [in] xyzw Pointer to several points in a contiguous block of memory.
 *
 * \param [in] numPoints The number of points to add.
 *
 * \return
 *        - The ID of the first point added (all IDs are sequential), if successful
 *         - PTK_POINTCLOUD_INVALID_POINT, otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_addv(PTK_PointCloud    *pc,
                             const PTK_Point   *xyzw,
                             uint32_t           numPoints);

/**
 * \brief This returns the total number of points that have been added since
 *        PTK_PointCloud_clear() was last called.
 *
 * \param [in] pc Read from this point cloud.
 *
 * \return Total number of points that have been added.
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_getPointCount(const PTK_PointCloud *pc);

/**
 * \brief This copies a PTK_Point out of the point cloud into another
 *        (user-supplied) memory location.
 *
 * \param [out] pc Read from this point cloud.
 *
 * \param [in] id Read point with the given identifier.
 *
 * \param [out] dst Copy the point here.
 *
 * \return
 *        - 1 if a point was read
 *        - 0 otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_getPoint(const PTK_PointCloud  *pc,
                                 uint32_t               id,
                                 PTK_Point             *dst);

/**
 * \brief This gets a direct pointer to a specific PTK_Point within the point cloud.
 *
 * \param [in] pc Get point from this cloud.
 *
 * \param [in] id Read point with the given identifier.
 *
 * \return
 *        - Direct pointer to the PTK_Point requested, if id is valid
 *        - NULL, otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
static inline PTK_Point *PTK_PointCloud_refPoint(PTK_PointCloud   *pc,
                                                 uint32_t         id)
{
    if (id < pc->numPoints)
    {
        PTK_Point  *points;

        points = (PTK_Point*)((uint8_t*)pc + pc->pointOffset);

        return &points[id];
    }

    return NULL;
}

/**
 * \brief This gets an immutable pointer to a specific PTK_Point within the point cloud.
 *
 * \param [in] pc Get point from this cloud.
 *
 * \param [in] id Read point with the given identifier.
 *
 * \return
 *        - Direct pointer to the PTK_Point requested, if id is valid
 *        - NULL, otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
const PTK_Point *PTK_PointCloud_crefPoint(const PTK_PointCloud *pc,
                                          uint32_t              id);

/**
 * \brief This sets tag bits for the point with the given identifier.
 *
 * \param [inout] pc Modify a point in this cloud.
 *
 * \param [in] id Tag the point with this identifier.
 *
 * \param [in] tag Apply these tag bits.
 *
 * \ingroup group_ptk_point_cloud
 */
void PTK_PointCloud_tag(PTK_PointCloud *pc,
                        uint32_t        id,
                        uint32_t        tag);

/**
 * \brief This removes tag bits for the point at the given identifier.
 *
 * \param [in] pc Modify a point in this cloud.
 *
 * \param [in] id Untag the point with this identifier.
 *
 * \param [in] tag Remove these tag bits
 *
 * \ingroup group_ptk_point_cloud
 */
void PTK_PointCloud_untag(PTK_PointCloud   *pc,
                          uint32_t          id,
                          uint32_t          tag);

/**
 * \brief This checks if all of the bits set in tag are also set in the tag for
 *        the specified PTK_Point. Note that the point in question may have
 *        additional tag bits set, but it may not be missing any.
 *
 * \param [out] pc Check tag bits for a point in this cloud.
 *
 * \param [in] id Check the point with this identifier.
 *
 * \param [in] tag Check for these tag bits.
 *
 * \return
 *        - 1 if of the given tag bits are set
 *        - 0 otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_isTagged(const PTK_PointCloud  *pc,
                                 uint32_t               id,
                                 uint32_t               tag);

/**
 * \brief This reads the tag for a given point identifier in the given point cloud.
 *
 * \param [in] pc Read tag bits for a point in this cloud.
 *
 * \param [in] id Read tag bits for a point with this identifier.
 *
 * \return
 *        - The complete 32-bit tag for the specified point identifier, if
 *          id is in valid range.
 *        - 0, otherwise
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_readTag(const PTK_PointCloud   *pc,
                                uint32_t                id);

/**
 * \brief Copy metadata from src to dst.
 *
 * \param [out] dst Copy metadata to this cloud.
 *
 * \param [in] src Copy metadata from this cloud.
 *
 * \ingroup group_ptk_point_cloud
 */
static inline void PTK_PointCloud_copyMetaData(PTK_PointCloud          *dst,
                                               const PTK_PointCloud    *src)
{
    /* Copy meta data. */
    dst->config      = src->config;
    dst->pointOffset = src->pointOffset;

    return;
}

/**
 * \brief Copy points from src to dst, appending points so long as space remains
 *        in the dst cloud.
 *
 * \param [out] dst Copy points to this cloud.
 *
 * \param [in] src Copy points from this cloud.
 *
 * \return Total number of points copied from src to dst.
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_copy(PTK_PointCloud        *dst,
                             const PTK_PointCloud  *src);

/**
 * \brief Each point in src is transformed by M and then written to the same
 *        location in dst. The tag bits are also propagated to dst, and the
 *        total number of points present in dst is updated accordingly. src
 *        and dst may point to the same PTK_PointCloud. In practice, it is most
 *        common for dst to be empty first or to have src= dst. The return
 *        value indicates the number of successfully transformed and copied
 *        points, which may be important if transforming and appending onto a
 *        non-empty cloud for detecting space limitations.
 *
 * \param [out] dst Transformed points to be written here.
 *
 * \param [out] src Source cloud to be transformed.
 *
 * \param [in] M Rigid transformation to apply.
 *
 * \ingroup group_ptk_point_cloud
 */
void PTK_PointCloud_transform(PTK_PointCloud           *dst,
                              PTK_PointCloud           *src,
                              const PTK_RigidTransform *M);

/**
 * \brief Each point in src is scaled by scale and then written with the same id
 *        in dst. The tag bits are also propagated to dst. src and dst may point
 *        to the same PTK_PointCloud. In practice, it is most common for dst to
 *        be empty first, or to have src = dst. The return value indicates how
 *        many points were scaled and appended to dst, which may be important if
 *        appending to a non-empty point cloud for detectin space limitations.
 *
 * \param [out] dst Scaled points written here.
 *
 * \param [out] src Cloud to be scaled.
 *
 * \param [in] scale Scale factor.
 *
 * \ingroup group_ptk_point_cloud
 */
void PTK_PointCloud_scale(PTK_PointCloud   *dst,
                          PTK_PointCloud   *src,
                          float             scale);

/**
 * \brief Find PTK_Points with the minimum and maximum PTK Vector dot4 value
 *        computed with the given basis. If you are using tag bits rather than
 *        a fourth coordinate, be sure to set basis.meta.w to 0.
 *
 * \param [in] pc Cloud to operate on.
 *
 * \param [in] basis Basis vector for inner product metric.
 *
 * \param [out] minVal Minimum value written here.
 *
 * \param [out] maxVal Maximum value written here.
 *
 * \param [out] minId Minimum id written here.
 *
 * \param [out] maxId Maximum id written here.
 *
 * \ingroup group_ptk_point_cloud
 */
void PTK_PointCloud_getMinMax(const PTK_PointCloud *pc,
                              const PTK_Point      *basis,
                              float                *minVal,
                              float                *maxVal,
                              uint32_t             *minId,
                              uint32_t             *maxId);

/**
 * \brief
 *
 * \param [in] pc
 *
 * \param [in] ids
 *
 * \param [in] N
 *
 * \param [in] tol
 *
 * \param [in] iters
 *
 * \param [out] nd
 *
 * \param [out] foundInliers
 *
 * \return
 *
 * \ingroup group_ptk_point_cloud
 */
uint32_t PTK_PointCloud_ransacPlane(const PTK_PointCloud *pc, const uint32_t *__restrict ids, uint32_t N, float tol, uint32_t iters, float *nd, uint32_t *__restrict foundInliers);

#ifdef __cplusplus
}
#endif

#endif
