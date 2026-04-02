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
 * @file plane.c
 * @brief Implementation of Plane functions
 */

#include <math.h>
#include <perception/base/plane.h>

int32_t PTK_Plane_set(PTK_Plane *dst, float nx, float ny, float nz, float d)
{
    dst->nx = nx;
    dst->ny = ny;
    dst->nz = nz;
    dst->d = d;

    if (0.f == nx && 0.f == ny && 0.f == nz)
        return -1;
    else
        return 0;
}

void PTK_Plane_normalize(PTK_Plane *dst, PTK_Plane *src)
{
    float OneOverNormN = 1/sqrtf(src->nx*src->nx + src->ny*src->ny + src->nz*src->nz);
    dst->nx = src->nx * OneOverNormN;
    dst->ny = src->ny * OneOverNormN;
    dst->nz = src->nz * OneOverNormN;
    dst->d  = src->d  * OneOverNormN;
}

void PTK_Plane_transform(PTK_Plane *dst, PTK_Plane *src, const PTK_RigidTransform  *M_dst_src)
{
    //n
    PTK_RigidTransform R = *M_dst_src;
    PTK_RigidTransform_setTranslation(&R, 0.f, 0.f, 0.f);

    PTK_Point nsrc, ndst;
    PTK_Point_set(&nsrc, src->nx, src->ny, src->nz);

    PTK_Point_transform(&ndst, &nsrc, &R);

    //d
    PTK_RigidTransform M_src_dst;
    PTK_RigidTransform_invert(&M_src_dst, M_dst_src);

    PTK_Point t_src_dst;
    PTK_Point_set(&t_src_dst, M_src_dst.M[3], M_src_dst.M[7], M_src_dst.M[11]);

    float t_dot_n = PTK_Vector_dot3(&t_src_dst, &nsrc);

    dst->nx = ndst.x;
    dst->ny = ndst.y;
    dst->nz = ndst.z;
    dst->d  = src->d - t_dot_n;
}

float PTK_Plane_dot(const PTK_Plane *plane, const PTK_Point *point)
{
    return plane->nx*point->x + plane->ny * point->y + plane->nz * point->z - plane->d;
}

int32_t PTK_Plane_construct(PTK_Plane *plane, PTK_Point *a, PTK_Point *b, PTK_Point *c)
{
    PTK_Point t, s, n, ntmp;

    PTK_Vector_subtract(&t, b, a);
    PTK_Vector_subtract(&s, c, a);

    PTK_Vector_computeCrossProduct(&ntmp, &t, &s);

    float normInv = PTK_Vector_normalize(&n, &ntmp);
    if (normInv > 1e9f)
        return -1;

    float d = PTK_Vector_dot3(&n, a);
    if (d < 0)
    {
        PTK_Plane_set(plane, -n.x, -n.y, -n.z, -d);
    }
    else
    {
        PTK_Plane_set(plane, n.x, n.y, n.z, d);
    }

    return 0;
}

void PTK_Plane_projectPoint(PTK_Plane *plane, PTK_Point *proj, PTK_Point *point)
{
    PTK_Point scaledNormal;
    PTK_Plane planeNorm;
    PTK_Plane_normalize(&planeNorm, plane);
    float d = PTK_Plane_dot(&planeNorm, point);

    scaledNormal.x = -d*planeNorm.nx;
    scaledNormal.y = -d*planeNorm.ny;
    scaledNormal.z = -d*planeNorm.nz;

    PTK_Vector_add(proj, point, &scaledNormal);
}

