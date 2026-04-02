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
#ifndef PTK_RIGID_TRANSFORM_H
#define PTK_RIGID_TRANSFORM_H

/**
 * @file rigidTransform.h
 * @brief Declaration of structures and functions for rigid transforms
 */

/**
 * \defgroup group_ptk_rigid_transform PTK Rigid Transform APIs
 * \ingroup group_ptk_base
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Rigid Transform Matrix (single precision).
 *
 * \ingroup group_ptk_rigid_transform
 */
typedef struct
{
    /**
     * Row-major representation of the matrix with the fourth row set to
     *  [0 0 0 1].
     *
     * <b>
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *  +--                     --+       +--                     --+
     *  | M[0]  M[1]  M[2]  M[3]  |       | R[0]  R[1]  R[2]  T[0]  |
     *  | M[4]  M[5]  M[6]  M[7]  |  ==   | R[3]  R[4]  R[5]  T[1]  |
     *  | M[8]  M[9]  M[10] M[11] |       | R[6]  R[7]  R[8]  T[2]  |
     *  | M[12] M[13] M[14] M[15] |       |  0     0     0     1    |
     *  +--                     --+       +--                     --+
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *  where R[] is the rotation matrix and T[] is the translation vector.
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *  </b>
     */
    float M[16];

} PTK_RigidTransform;

/**
 * \brief Rigid Transform Matrix (double precision).
 *
 * The PTK RigidTransform d is used to represent 3D transformations with double
 * precision, compared with the single precision used by PTK_RigidTransform.
 *
 * \ingroup group_ptk_rigid_transform
 */
typedef struct
{
    /**
     * Row-major representation of the matrix with the fourth row set to
     *  [0 0 0 1].
     *
     * <b>
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *  +--                     --+       +--                     --+
     *  | M[0]  M[1]  M[2]  M[3]  |       | R[0]  R[1]  R[2]  T[0]  |
     *  | M[4]  M[5]  M[6]  M[7]  |  ==   | R[3]  R[4]  R[5]  T[1]  |
     *  | M[8]  M[9]  M[10] M[11] |       | R[6]  R[7]  R[8]  T[2]  |
     *  | M[12] M[13] M[14] M[15] |       |  0     0     0     1    |
     *  +--                     --+       +--                     --+
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *  where R[] is the rotation matrix and T[] is the translation vector.
     *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *  </b>
     */
    double M[16];

} PTK_RigidTransform_d;


/* Single precision functions. */
/**
 * \brief This replaces the contents of the given rigid transformation with an
 *        identity transformation. This handles the duties of both
 *        initialization and resetting or clearing the data structure.
 *
 * \param [inout] M Transform to modify.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_makeIdentity(PTK_RigidTransform *M);

/**
 * \brief Convenience function to update all three components of the transform's
 *        translation component safely.
 *
 * \param [out] M Transformation to update.
 *
 * \param [in] x New x.
 *
 * \param [in] y New y.
 *
 * \param [in] z New z.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_setTranslation(PTK_RigidTransform *M, float x, float y, float z);

/**
 * \brief Convenience function to get all three components of the transform's
 *        translation component safely.
 *
 * \param [in] M Transformation to read from.
 *
 * \param [out] x X-component of the translation vector.
 *
 * \param [out] y Y-component of the translation vector.
 *
 * \param [out] z Z-component of the translation vector.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_getTranslation(const PTK_RigidTransform *M, float *x, float *y, float *z);

/**
 * \brief Convenience function to update the rotation matrix embedded within
 *        this rigid transform. The rotation argument must be an orthonormal
 *        matrix, otherwise the subsequent behavior of the PTK_RigidTransform
 *        is undefined.
 *
 * \param [inout] M The transformation to update.
 *
 * \param [in] R 9 element array containing a 3x3 rotation matrix in
 *               row-major order
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_setRotation(PTK_RigidTransform *__restrict M, const float *__restrict R);

/**
 * \brief Convenience function to read the rotation matrix embedded within
 *        this rigid transform.
 *
 * \param [in] M The transformation to update.
 *
 * \param [out] R 9 element array containing a 3x3 rotation matrix in
 *              row-major order
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_getRotation(const PTK_RigidTransform *__restrict M, float *__restrict R);

/**
 * \brief This creates a single transformation that acts as the composition of
 *        the two given transformations such that M31 = M32*M21. M31 must
 *        be distinct from both M21 and M32.
 *
 * \param [out] M31_out M31 transformation.
 *
 * \param [in] M32_in M32 transformation.
 *
 * \param [in] M21_in M21 transformation.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_compose(PTK_RigidTransform *__restrict M31_out, const PTK_RigidTransform *__restrict M32_in, const PTK_RigidTransform *__restrict M21_in);

/**
 * \brief This computes the inverse of the given rigid transformation. The
 *        output must be different from the input. Note that this assumes
 *        that a valid rigid transformation has been supplied: the rotation
 *        component must be an orthonormal matrix. A general matrix inversion
 *        is not used; instead an equation specific to inverting matrices that
 *        match the structure described above is used for efficiency.
 *
 * \param [out] M21_out Write the inverse here.
 *
 * \param [in] M12_in The transform to compute the inverse of.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_invert(PTK_RigidTransform *__restrict M21_out, const PTK_RigidTransform *__restrict M12_in);

/**
 * \brief Prints the given transformation via platform-specific output
 *        mechanism.
 *
 * \param [in] M Transform to be printed.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_print(const PTK_RigidTransform *M);

/* Conversions. */
/**
 * \brief Convert the given PTK RigidTransform from single to double precision.
 *
 * \param [out] Mout Double precision version written here.
 *
 * \param [in] Min Floating point transform to convert.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_convertToDouble(PTK_RigidTransform_d *__restrict Mout, const PTK_RigidTransform *__restrict Min);

/* Double precision functions. */
/**
 * \brief This replaces the contents of the given rigid transformation with an
 *        identity transformation. This handles the duties of both
 *        initialization and resetting or clearing the data structure.
 *
 * \param [inout] M Transform to modify.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_makeIdentity(PTK_RigidTransform_d *M);

/**
 * \brief Convenience function to update all three components of the transform's
 *        translation component safely.
 *
 * \param [out] M Transformation to update.
 *
 * \param [in] x New x.
 *
 * \param [in] y New y.
 *
 * \param [in] z New z.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_setTranslation(PTK_RigidTransform_d *M, double x, double y, double z);

/**
 * \brief Convenience function to get all three components of the transform's
 *        translation component safely.
 *
 * \param [in] M Transformation to read from.
 *
 * \param [out] x X-component of the translation vector.
 *
 * \param [out] y Y-component of the translation vector.
 *
 * \param [out] z Z-component of the translation vector.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_getTranslation(const PTK_RigidTransform_d *M, double *x, double *y, double *z);

/**
 * \brief Convenience function to update the rotation matrix embedded within
 *        this rigid transform. The rotation argument must be an orthonormal
 *        matrix, otherwise the subsequent behavior of the PTK_RigidTransform
 *        is undefined.
 *
 * \param [inout] M The transformation to update.
 *
 * \param [in] R 9 element array containing a 3x3 rotation matrix in
 *               row-major order
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_setRotation(PTK_RigidTransform_d *__restrict M, const double *__restrict R);

/**
 * \brief Convenience function to read the rotation matrix embedded within
 *        this rigid transform.
 *
 * \param [in] M The transformation to update.
 *
 * \param [out] R 9 element array containing a 3x3 rotation matrix in
 *              row-major order
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_getRotation(const PTK_RigidTransform_d *__restrict M, double *__restrict R);

/**
 * \brief This creates a single transformation that acts as the composition of
 *        the two given transformations such that M31 = M32*M21. M31 must
 *        be distinct from both M21 and M32.
 *
 * \param [out] M31_out M31 transformation.
 *
 * \param [in] M32_in M32 transformation.
 *
 * \param [in] M21_in M21 transformation.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_compose(PTK_RigidTransform_d *__restrict M31_out, const PTK_RigidTransform_d *__restrict M32_in, const PTK_RigidTransform_d *__restrict M21_in);

/**
 * \brief This computes the inverse of the given rigid transformation. The
 *        output must be different from the input. Note that this assumes
 *        that a valid rigid transformation has been supplied: the rotation
 *        component must be an orthonormal matrix. A general matrix inversion
 *        is not used; instead an equation specific to inverting matrices that
 *        match the structure described above is used for efficiency.
 *
 * \param [out] M21_out Write the inverse here.
 *
 * \param [in] M12_in The transform to compute the inverse of.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_invert(PTK_RigidTransform_d *__restrict M21_out, const PTK_RigidTransform_d *__restrict M12_in);

/**
 * \brief Prints the given transformation via platform-specific output
 *        mechanism.
 *
 * \param [in] M Transform to be printed.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_print(const PTK_RigidTransform_d *M);

/**
 * \brief Convert the given RigidTransform d from double to single precision.
 *
 * \param [out] Mout Single precision version here.
 *
 * \param [in] Min Double precision transform to convert.
 *
 * \ingroup group_ptk_rigid_transform
 */
void PTK_RigidTransform_d_convertToSingle(PTK_RigidTransform *__restrict Mout, const PTK_RigidTransform_d *__restrict Min);

#ifdef __cplusplus
}
#endif

#endif
