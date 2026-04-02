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
#ifndef PTK_PLANE_H
#define PTK_PLANE_H

/**
 * @file plane.h
 * @brief Declarations for PTK_Plane struct+functions
 */

#include <stdint.h>

#include <perception/base/point.h>
#include <perception/base/rigidTransform.h>

/**
 * \defgroup group_ptk_plane PTK Plane
 * \ingroup group_ptk_base
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Definition of a plane.
 *
 * \ingroup group_ptk_plane
 */
typedef struct
{
    /** Normal vector, x-component. */
    float   nx;

    /** Normal vector, y-component. */
    float   ny;

    /** Normal vector, z-component. */
    float   nz;

    /** nx*x+ny*y+nz*z=d for every point (x,y,z) on the plane. */
    float   d;

} PTK_Plane;

/**
 * \brief Set all four values for a point as a convenience function.
 *        At least one of nx, ny, nz must be non-zero.
 *
 * \param[inout] dst Pointer to Plane to be modified
 *
 * \param[in] nx New nx value
 *
 * \param[in] ny New ny value
 *
 * \param[in] nz New nz value
 *
 * \param[in] d  New d value
 *
 * return
 *       - 0 on success
 *       - -1 on fail
 *
 * \ingroup group_ptk_plane
 */
int32_t PTK_Plane_set(PTK_Plane *dst, float nx, float ny, float nz, float d);

/**
 * \brief Normalize plane parameters so that nx^2+ny^2+nz^2=1
 *
 * \param[inout] src Pointer to input plane
 *
 * \param[inout] dst Pointer to normalized plane. Can be same as src.
 *
 * \ingroup group_ptk_plane
 */
void PTK_Plane_normalize(PTK_Plane *dst, PTK_Plane *src);

/**
 * \brief Apply the given rigid transformation to represent the plane
 *        (given in src reference frame) in dst reference frame. Output
 *        plane will have the same normalization factor as the input plane.
 *
 * \param[in]    src Pointer to source plane
 *
 * \param[inout] dst Pointer to destination plane, which may be the same as src
 *
 * \param[in]    M_dst_src The rigid transformation to be applied
 *
 * \ingroup group_ptk_plane
 */
void PTK_Plane_transform(PTK_Plane *dst, PTK_Plane *src, const PTK_RigidTransform  *M_dst_src);

/**
 * \brief Dot product between a plane and a 3d point (x,y,z), which is
 *        out = x*nx + y*nx + z*nz - d. If the plane is the normalized, then
 *        out is the signed distance of the point from the plane, where
 *        positive is in the direction of the normal vector
 *
 * \param[in]  plane Pointer to plane
 *
 * \param[in]  point Pointer to point
 *
 * \return Result of the dot product.
 *         dot product out = point.x*plane.nx + point.y*plane.ny +
 *                           point.z*plane.nz - plane.d
 *
 * \ingroup group_ptk_plane
 */
float PTK_Plane_dot(const PTK_Plane *plane, const PTK_Point *point);

/**
 * \brief Construct normalized plane from 3 non-colinear points in 3D space by
 *        taking cross product
 *
 * \param[out] plane  Pointer to normalized plane.
 *
 * \param[in]  a Pointer to first point.
 *
 * \param[in]  b Pointer to second point.
 *
 * \param[in]  c Pointer to third point.
 *
 * return
 *        - 0 on success
 *        - -1 on fail (when points are colinear)
 *
 * \ingroup group_ptk_plane
 */
int32_t PTK_Plane_construct(PTK_Plane *plane, PTK_Point *a, PTK_Point *b, PTK_Point *c);

/**
 * \brief Project 3D point onto the plane
 *
 * \param[out]  plane Pointer to normalized plane
 *
 * \param[in]   point Pointer to point to be projected
 *
 * \param[out]  proj  Pointer to projected point on the plane.
 *                    Can be same as point.
 *
 * \ingroup group_ptk_plane
 */
void PTK_Plane_projectPoint(PTK_Plane *plane, PTK_Point *proj, PTK_Point *point);

#ifdef __cplusplus
}
#endif

#endif
