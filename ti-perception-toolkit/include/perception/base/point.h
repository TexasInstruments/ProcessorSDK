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
#ifndef PTK_POINT_H
#define PTK_POINT_H

/**
 * @file point.h
 * @brief Declarations for PTK_Point and PTK_Vector struct+functions
 */

#include <stdint.h>

#include <perception/base/rigidTransform.h>

/**
 * \defgroup group_ptk_point PTK Point
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
 * \brief Definition of vector meta data.
 *
 * \ingroup group_ptk_point
 */
typedef union
{
    /** A fourth coordinate, if necessary. */
    float       w;

    /** A bitwise flag field used to track point status (coordinate system,
     *  point cloud validity, and other application-specific uses).
     */
    uint32_t    f;

} PTK_Vector_Meta;

/**
 * \brief Definition of a point.
 *
 * \ingroup group_ptk_point
 */
typedef struct
{
    /** First component. */
    float           x;

    /** Second component. */
    float           y;

    /** Third component. */
    float           z;

    /** Metadata component, the use is application specific. */
    PTK_Vector_Meta meta;

} PTK_Vector;

/**
 * \brief Definition of a point.
 *
 * \ingroup group_ptk_point
 */
typedef PTK_Vector PTK_Point;

/**
 * \brief Definition of doube precision vector meta data.
 *
 * \ingroup group_ptk_point
 */
typedef union 
{
    /** A fourth coordinate, if necessary. */
    double      w;

    /** A bitwise flag field used to track point status (coordinate system,
     *  point cloud validity, and other application-specific uses).
     */
    uint32_t    f;

} PTK_Vector_d_Meta;

/**
 * \brief Definition of a double precision Vector.
 *
 * The PTK_Point_d is used to represent a point or vector with double precision,
 * as may be required for accurate representation of coordinates in ECEF. The
 * PTK_Point_d is defined analogously to the PTK_Point_d except that d is
 * appended everywhere where Point appears, and double replaces float in all
 * instances.
 *
 * \ingroup group_ptk_point
 */
typedef struct
{
    /** First component. */
    double              x;

    /** Second component. */
    double              y;

    /** Third component. */
    double              z;

    /** Metadata component, the use is application specific. */
    PTK_Vector_d_Meta   meta;

} PTK_Vector_d;

/**
 * \brief Definition of a double precision Point.
 *
 * \ingroup group_ptk_point
 */
typedef PTK_Vector_d PTK_Point_d;


/**
 * \brief Definition of a 2D point.
 *
 * \ingroup group_ptk_point
 */
typedef struct
{
    /** First component. */
    float           x;

    /** Second component. */
    float           y;

} PTK_2D_Vector;

typedef PTK_2D_Vector PTK_2D_Point;


/**
 * \brief Updates all three components stored within the given PTK_Point to
 *        match those given as arguments, to be used primarily for convenience
 *        when initializing a point.
 *
 * \param [out] dst PTK_Point struct to be modified.
 *
 * \param [in] x New x coordinate.
 *
 * \param [in] y New y coordinate.
 *
 * \param [in] z New z coordinate.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_set(PTK_Point *dst, float x, float y, float z);

/**
 * \brief Perform component-wise addition of the vectors stored in src1 and
 *        src2, writing the result to dst.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src1 First source vector to be added.
 *
 * \param [in] src2 Second source vector to be added.
 *
 * \ingroup group_ptk_point
 */
void PTK_Vector_add(PTK_Vector *dst, const PTK_Vector *src1, const PTK_Vector *src2);

/**
 * \brief Perform component-wise subtraction, subtracting src2 from src1 and
 *        writing the result to dst.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src1 The term subtracted from.
 *
 * \param [in] src2 The term being subtracted.
 *
 * \ingroup group_ptk_point
 */
void PTK_Vector_subtract(PTK_Vector *dst, const PTK_Vector *src1, const PTK_Vector *src2);

/**
 * \brief Perform component-wise scaling of the given PTK Vector and store the
 *        result at dst.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src The vector to be scaled.
 *
 * \param [in] scale The magnitude of the scale factor.
 *
 * \ingroup group_ptk_point
 */
void PTK_Vector_scale(PTK_Vector *dst, const PTK_Vector *src, float scale);

/**
 * \brief Perform a rigid transformation on the given point, d = (Mr*s) + Mt.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src The point to be transformed.
 *
 * \param [in] M The transformation to apply to src.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_transform(PTK_Point *dst, const PTK_Point *src, const PTK_RigidTransform *M);

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
float PTK_Vector_dot2(const PTK_Vector *p1, const PTK_Vector *p2);

/**
 * \brief Compute the dot product of the two given vectors in 3D, given by
 *        r = ax*bx + ay*by + az*bz.
 *
 * \param [in] p1 First vector for dot product.
 *
 * \param [in] p2 Second vector for dot product.
 *
 * \return Dot product value.
 *
 * \ingroup group_ptk_point
 */
float PTK_Vector_dot3(const PTK_Vector *p1, const PTK_Vector *p2);

/**
 * \brief Compute the dot product of the two given vectors in 3D, given by
 *        r = ax*bx + ay*by + az*bz + aw*bw.
 *
 *        Note that this reads the w field from the metadata, so it cannot be
 *        used to store tag bits simultaneously.
 *
 * \param [in] p1 First vector for dot product.
 *
 * \param [in] p2 Second vector for dot product.
 *
 * \return Dot product value.
 *
 * \ingroup group_ptk_point
 */
float PTK_Vector_dot4(const PTK_Vector *p1, const PTK_Vector *p2);

/**
 * \brief Computes the cross product of the two given vectors in 3D, given by
 *        rx = ay*bz - az*by
 *        ry = az*bx - ax*bz
 *        rz = ax*by - ay*bx
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] a First vector for cross product.
 *
 * \param [in] b Second vector for cross product.
 *
 * \ingroup group_ptk_point
 */
void PTK_Vector_computeCrossProduct(PTK_Vector *__restrict dst, const PTK_Vector *__restrict a, const PTK_Vector *__restrict b);

/**
 * \brief Computes the L2 norm of the given vector, given by
 *        v = sqrt(vx*vx + vy*vy + vz*vz).
 *
 * \param [in] v Vector whose norm is to be computed.
 *
 * \return Computed norm.
 *
 * \ingroup group_ptk_point
 */
float PTK_Vector_computeNorm(const PTK_Vector *v);

/**
 * \brief Normalizes the given input vector and writes to an output vector.
 *
 * \param [out] dst Resulting normalized vector.
 *
 * \param [in] in Vector to be normalized.
 *
 * \return Inverse of the vector norm i.e. 1.0/norm(v).
 *
 * \ingroup group_ptk_point
 */
float PTK_Vector_normalize(PTK_Vector *__restrict dst, const PTK_Vector *__restrict in);

/**
 * \brief This sets tag bits for the given point.
 *
 * \param [out] pt The point to tag.
 *
 * \param [in] tag Tag bits to be applied.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_tag(PTK_Point *pt, uint32_t tag);

/**
 * \brief This removes tag bits for the given point.
 *
 * \param [out] pt The point to untag.
 *
 * \param [in] tag Tag bits to be removed.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_untag(PTK_Point *pt, uint32_t tag);

/**
 * \brief This checks if all of the bits set in tag are also set in the tag for
 *        the given PTK_Point. Additional bits may be set, but none may be
 *        missing from the given mask.
 *
 * \param [out] pt Point whose tag bits are to be queried.
 *
 * \param [in] tag The tag bits to check for.
 *
 * \return
 *         - 1 if all of the given tag bits are set
 *         - 0, otherwise
 *
 * \ingroup group_ptk_point
 */
uint32_t PTK_Point_isTagged(PTK_Point *pt, uint32_t tag);

/**
 * \brief Updates all three components stored within the given PTK_Point to
 *        match those given as arguments, to be used primarily for convenience
 *        when initializing a point.
 *
 * \param [out] dst PTK_Point struct to be modified.
 *
 * \param [in] x New x coordinate.
 *
 * \param [in] y New y coordinate.
 *
 * \param [in] z New z coordinate.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_d_set(PTK_Point_d *dst, double x, double y, double z);

/**
 * \brief Perform component-wise addition of the vectors stored in src1 and
 *        src2, writing the result to dst.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src1 First source vector to be added.
 *
 * \param [in] src2 Second source vector to be added.
 *
 * \ingroup group_ptk_point
 */
void PTK_Vector_d_add(PTK_Vector_d *dst, PTK_Vector_d *src1, PTK_Vector_d *src2);

/**
 * \brief Perform component-wise subtraction, subtracting src2 from src1 and
 *        writing the result to dst.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src1 The term subtracted from.
 *
 * \param [in] src2 The term being subtracted.
 *
 * \ingroup group_ptk_point
 */
void PTK_Vector_d_subtract(PTK_Vector_d *dst, PTK_Vector_d *src1, PTK_Vector_d *src2);

/**
 * \brief Perform component-wise scaling of the given PTK Vector and store the
 *        result at dst.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src The vector to be scaled.
 *
 * \param [in] scale The magnitude of the scale factor.
 *
 * \ingroup group_ptk_point
 */
void PTK_Vector_d_scale(PTK_Vector_d *dst, PTK_Vector_d *src, double scale);

/**
 * \brief Perform a rigid transformation on the given point, d = (Mr*s) + Mt.
 *
 * \param [out] dst Resulting vector.
 *
 * \param [in] src The point to be transformed.
 *
 * \param [in] M The transformation to apply to src.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_d_transform(PTK_Point_d *dst, PTK_Point_d *src, const PTK_RigidTransform_d *M);

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

/**
 * \brief Compute the dot product of the two given vectors in 3D, given by
 *        r = ax*bx + ay*by + az*bz.
 *
 * \param [in] p1 First vector for dot product.
 *
 * \param [in] p2 Second vector for dot product.
 *
 * \return Dot product value.
 *
 * \ingroup group_ptk_point
 */
double PTK_Vector_d_dot3(const PTK_Vector_d *p1, const PTK_Vector_d *p2);

/**
 * \brief Compute the dot product of the two given vectors in 3D, given by
 *        r = ax*bx + ay*by + az*bz + aw*bw.
 *
 *        Note that this reads the w field from the metadata, so it cannot be
 *        used to store tag bits simultaneously.
 *
 * \param [in] p1 First vector for dot product.
 *
 * \param [in] p2 Second vector for dot product.
 *
 * \return Dot product value.
 *
 * \ingroup group_ptk_point
 */
double PTK_Vector_d_dot4(const PTK_Vector_d *p1, const PTK_Vector_d *p2);

/**
 * \brief This sets tag bits for the given point.
 *
 * \param [out] pt The point to tag.
 *
 * \param [in] tag Tag bits to be applied.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_d_tag(PTK_Point_d *pt, uint32_t tag);

/**
 * \brief This removes tag bits for the given point.
 *
 * \param [out] pt The point to untag.
 *
 * \param [in] tag Tag bits to be removed.
 *
 * \ingroup group_ptk_point
 */
void PTK_Point_d_untag(PTK_Point_d *pt, uint32_t tag);

/**
 * \brief This checks if all of the bits set in tag are also set in the tag for
 *        the given PTK_Point. Additional bits may be set, but none may be
 *        missing from the given mask.
 *
 * \param [out] pt Point whose tag bits are to be queried.
 *
 * \param [in] tag The tag bits to check for.
 *
 * \return
 *         - 1 if all of the given tag bits are set
 *         - 0, otherwise
 *
 * \ingroup group_ptk_point
 */
uint32_t PTK_Point_d_isTagged(PTK_Point_d *pt, uint32_t tag);

#ifdef __cplusplus
}
#endif

#endif
