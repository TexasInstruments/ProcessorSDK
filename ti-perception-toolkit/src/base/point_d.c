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
 * @file point_d.c
 * @brief Implementation of Point and Vector functions for double precision
 */

#include <perception/base/point.h>
#include <perception/base/rigidTransform.h>

void PTK_Point_d_set(PTK_Point_d *dst, double x, double y, double z)
{
    dst->x = x;
    dst->y = y;
    dst->z = z;
}

void PTK_Vector_d_add(PTK_Vector_d *dst, PTK_Vector_d *src1, PTK_Vector_d *src2)
{
    dst->x = src1->x + src2->x;
    dst->y = src1->y + src2->y;
    dst->z = src1->z + src2->z;
}

void PTK_Vector_d_subtract(PTK_Vector_d *dst, PTK_Vector_d *src1, PTK_Vector_d *src2)
{
    dst->x = src1->x - src2->x;
    dst->y = src1->y - src2->y;
    dst->z = src1->z - src2->z;
}

void PTK_Vector_d_scale(PTK_Vector_d *dst, PTK_Vector_d *src, double scale)
{
    dst->x = src->x * scale;
    dst->y = src->y * scale;
    dst->z = src->z * scale;
}

void PTK_Point_d_transform(PTK_Point_d *dst, PTK_Point_d *src, const PTK_RigidTransform_d *M)
{
    double x, y, z;
    x = M->M[0] * src->x + M->M[1] * src->y + M->M[2] * src->z + M->M[3];
    y = M->M[4] * src->x + M->M[5] * src->y + M->M[6] * src->z + M->M[7];
    z = M->M[8] * src->x + M->M[9] * src->y + M->M[10] * src->z + M->M[11];
    dst->x = x;
    dst->y = y;
    dst->z = z;
	dst->meta = src->meta;
}

double PTK_Vector_d_dot2(const PTK_Vector_d *p1, const PTK_Vector_d *p2)
{
    return (p1->x * p2->x + p1->y * p2->y);
}

double PTK_Vector_d_dot3(const PTK_Vector_d *p1, const PTK_Vector_d *p2)
{
    return (p1->x * p2->x + p1->y * p2->y + p1->z * p2->z);
}

double PTK_Vector_d_dot4(const PTK_Vector_d *p1, const PTK_Vector_d *p2)
{
    return (p1->x * p2->x + p1->y * p2->y + p1->z * p2->z + p1->meta.w * p2->meta.w);
}

void PTK_Point_d_tag(PTK_Point_d *pt, uint32_t tag)
{
	pt->meta.f |= tag;
}

void PTK_Point_d_untag(PTK_Point_d *pt, uint32_t tag)
{
	pt->meta.f &= ~tag;
}

uint32_t PTK_Point_d_isTagged(PTK_Point_d *pt, uint32_t tag)
{
	return (pt->meta.f & tag) == tag;
}

