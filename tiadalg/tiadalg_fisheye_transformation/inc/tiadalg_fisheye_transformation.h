/*
*
* Copyright (c) {2015 - 2018} Texas Instruments Incorporated
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
@file      tiadalg_fisheye_transformation.h
@brief     This file defines the interface for fish eye transformation of list of key points
@version 0.1 Sep 2018 : Initial Code
*/

/** @ingroup    TIADALG_FISHEYE_TRANSFORMATION_H */
/*@{*/
#ifndef TI_FISH_EYE_TARNSFORMATION_H
#define TI_FISH_EYE_TARNSFORMATION_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @func tiadalg_fisheye_transformation_cn
 *
 * @brief It does transformation of list of 2D points from rectalinear (source) image domain to
 *        fish eye (destimation) image domain, for given angle table.
 *
 * @param in_img_points [in] : Input image points in (x,y) format. Size of this buffer should be \c num_points * 2
 * @param num_points    [in] : Number of input points
 * @param in_center_x   [in] : image center in x direction for input image dimension
 * @param in_center_y   [in] : image center in y direction for input image dimension
 * @param focal_length  [in] : focal length of the camera for input and output image
 * @param out_center_cx [in] : image center in x direction for output image dimension
 * @param out_center_cy [in] : image center in y direction for output image dimension
 * @param angle_table   [in] : Angle tuple for the ray originating from focal center to image plane in input image and corresponding angle in output image
 * @param num_table_row [in] : Number of rows in \c in above table
 * @param out_img_points[out]: Output image points corresponding to each input point \c in_img_points
 *
 * @note
 * - size of the /c angle_table buffer should be /c num_table_row * 2
 * - each entry of table is the pair of angles, where first element is for source domain angle and second
 *   element is destination image domain angle.
 * - These angles are the angle subtended by light ray with optical axis, and it should be in ascending order
 */

  int32_t tiadalg_fisheye_transformation_cn(int16_t* in_img_points, int32_t num_points,
    uint32_t in_center_x, uint32_t in_center_y, float focal_length,
    uint32_t out_center_cx, uint32_t out_center_cy,
    float(*angle_table)[2], int32_t num_table_row, 
    uint16_t * scratch, int32_t is_scratch_filled,    
    int16_t* out_img_points);
    
/**
 * @func tiadalg_fisheye_transformation_c66
 *
 * @brief It does transformation of list of 2D points from rectalinear (source) image domain to
 *        fish eye (destimation) image domain, for given angle table.
 *
 * @param in_img_points [in] : Input image points in (x,y) format. Size of this buffer should be \c num_points * 2
 * @param num_points    [in] : Number of input points
 * @param in_center_x   [in] : image center in x direction for input image dimension
 * @param in_center_y   [in] : image center in y direction for input image dimension
 * @param focal_length  [in] : focal length of the camera for input and output image
 * @param out_center_cx [in] : image center in x direction for output image dimension
 * @param out_center_cy [in] : image center in y direction for output image dimension
 * @param angle_table   [in] : Angle tuple for the ray originating from focal center to image plane in input image and corresponding angle in output image
 * @param num_table_row [in] : Number of rows in \c in above table
 * @param out_img_points[out]: Output image points corresponding to each input point \c in_img_points
 *
 * @note
 * - size of the /c angle_table buffer should be /c num_table_row * 2
 * - each entry of table is the pair of angles, where first element is for source domain angle and second
 *   element is destination image domain angle.
 * - These angles are the angle subtended by light ray with optical axis, and it should be in ascending order
 */
 
  int32_t tiadalg_fisheye_transformation_c66(int16_t * in_img_points, int32_t num_points,
    uint32_t in_center_x, uint32_t in_center_y, float focal_length,
    uint32_t out_center_cx, uint32_t out_center_cy,
    float(*angle_table)[2], int32_t num_table_row, 
    uint16_t * scratch, int32_t is_scratch_filled,    
    int16_t* out_img_points);

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TI_FISH_EYE_TARNSFORMATION_H */
