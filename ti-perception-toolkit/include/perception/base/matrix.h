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
#ifndef MATRIX_H
#define MATRIX_H

/**
 * @file matrix.h
 * @brief Declarations for PTK_Matrix struct+functions
 */

#include <stdint.h>


/**
 * \defgroup group_ptk_matrix PTK Matrix
 * \ingroup group_ptk_base
 *
 * The geometry component provides a common framework for representing and
 * manipulating geometric quantities, such as points, vectors, and
 * transformations between coordinate spaces. These are fundamental elements
 * which are used to build most other components. For example, Points structs
 * are necessary to represent position information in the geodesy component,
 * and RigidTransforms are needed to represent the conversion between coordinate
 * systems.
 *
 * Mathematically, a "point" with 3 coordinates and a "vector" with 3 components
 * are similar and we may implicitly convert between a point and a vector from
 * the origin to a point. They are however, semantically distinct, leading to
 * the natural requirement for distinct types. In the interest of providing
 * ease of use and reusing implementation logic where possible, point and
 * vector types are provided as aliases of the same type, ptk abstract vector,
 * and the specific type name used in a function description is used to indicate
 * the semantic usage of that function. For instance, points are not added
 * together, but vectors are. Points may be further broken down by the
 * coordinate system they are associated with, and this is stored within the
 * metadata field.
 *
 * The abstract vector type has a fixed layout on all platforms, shown below,
 * chosen for effective memory alignment and compatibility with C7x, ARM, and
 * x86 architectures. Although it is largely a transparent structure, several
 * convenience functions are provided for interacting with its fields, and the
 * metadata union should only be accessed through the provided API.
 *
 * Note that PTK_Point and PTK Vector are aliases and thus may be supplied
 * interchangeably to all of the functions below, but the appropriate semantic
 * name is used in determining the function name and as the argument type.
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief Definition of a 3x3 matrix
 *
 * \ingroup group_ptk_matrix
 */
typedef struct {
	/**
     *  M[0]  M[1]  M[2]
     *  M[3]  M[4]  M[5]
     *  M[6]  M[7]  M[8]
     */
 	float M[9];

 } PTK_3D_Matrix;


#if 0


/**
 * \brief Compute the dot product of the two given vectors in 2D, given by
 *        r = ax*bx + ay*by.
 *
 * \param [in] p1 First vector for dot product.
 *
 * \param [in] p2 Second vector for dot product.
 *
 * \return Dot product value.
 *
 * \ingroup group_ptk_point
 */
double PTK_Vector_d_dot2(const PTK_Vector_d *p1, const PTK_Vector_d *p2);
#endif

#ifdef __cplusplus
}
#endif

#endif
