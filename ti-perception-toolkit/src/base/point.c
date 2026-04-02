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
 * @file point.c
 * @brief Implementation of Point and Vector functions
 */

#include <math.h>
#include <perception/base/point.h>
#include <perception/base/rigidTransform.h>

#if !defined(INFINITY)
#define INFINITY 1e20
#endif

void PTK_Point_set(PTK_Point *dst, float x, float y, float z)
{
    dst->x = x;
    dst->y = y;
    dst->z = z;
}

void PTK_Vector_add(PTK_Vector *dst, const PTK_Vector *src1, const PTK_Vector *src2)
{
    dst->x = src1->x + src2->x;
    dst->y = src1->y + src2->y;
    dst->z = src1->z + src2->z;
}

void PTK_Vector_subtract(PTK_Vector *dst, const PTK_Vector *src1, const PTK_Vector *src2)
{
    dst->x = src1->x - src2->x;
    dst->y = src1->y - src2->y;
    dst->z = src1->z - src2->z;
}

void PTK_Vector_scale(PTK_Vector *dst, const PTK_Vector *src, float scale)
{
    dst->x = src->x * scale;
    dst->y = src->y * scale;
    dst->z = src->z * scale;
}

/**
 * Apply the given rigid transformation to a single point. dst = M_dst_src.R * src + M_dst_src.t.
 * w coordinate is unchanged.
 */
void PTK_Point_transform(PTK_Point *dst, const PTK_Point *src, const PTK_RigidTransform *M)
{
    float x, y, z;
    x = M->M[0] * src->x + M->M[1] * src->y + M->M[2] * src->z + M->M[3];
    y = M->M[4] * src->x + M->M[5] * src->y + M->M[6] * src->z + M->M[7];
    z = M->M[8] * src->x + M->M[9] * src->y + M->M[10] * src->z + M->M[11];
    dst->x = x;
    dst->y = y;
    dst->z = z;
	dst->meta = src->meta;
}

float PTK_Vector_dot2(const PTK_Vector *p1, const PTK_Vector *p2)
{
    return (p1->x * p2->x + p1->y * p2->y);
}

float PTK_Vector_dot3(const PTK_Vector *p1, const PTK_Vector *p2)
{
    return (p1->x * p2->x + p1->y * p2->y + p1->z * p2->z);
}

float PTK_Vector_dot4(const PTK_Vector *p1, const PTK_Vector *p2)
{
    return (p1->x * p2->x + p1->y * p2->y + p1->z * p2->z + p1->meta.w * p2->meta.w);
}

void PTK_Vector_computeCrossProduct(PTK_Vector *__restrict dst, const PTK_Vector *__restrict a, const PTK_Vector *__restrict b)
{
    float x = a->y*b->z - a->z * b->y;
    float y = a->z*b->x - a->x * b->z;
    float z = a->x*b->y - a->y * b->x;
    dst->x = x;
    dst->y = y;
    dst->z = z;
}

float PTK_Vector_computeNorm(const PTK_Vector *v)
{
    return sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
}

float PTK_Vector_normalize(PTK_Vector *__restrict dst, const PTK_Vector *__restrict in)
{
    float normSq = in->x*in->x + in->y*in->y + in->z*in->z;
    float OneOverNorm;
    if (normSq > 0.f)
    {
        OneOverNorm = 1/sqrtf(normSq);
        dst->x = in->x * OneOverNorm;
        dst->y = in->y * OneOverNorm;
        dst->z = in->z * OneOverNorm;
        dst->meta = in->meta;
    }
    else
    {
        OneOverNorm = INFINITY;
        *dst = *in;
    }

    return OneOverNorm;
}

void PTK_Point_tag(PTK_Point *pt, uint32_t tag)
{
	pt->meta.f |= tag;
}

void PTK_Point_untag(PTK_Point *pt, uint32_t tag)
{
	pt->meta.f &= ~tag;
}

uint32_t PTK_Point_isTagged(PTK_Point *pt, uint32_t tag)
{
	return (pt->meta.f & tag) == tag;
}
