/*
*
* Copyright (c) {2015 - 2019} Texas Instruments Incorporated
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
@file      tiadalg_image_color_blending.h
@brief     This file defines the interface for image color blending to highlight pixel level detection derived by other algorithm such as semantic segmentation
@version 0.1 Sep 2018 : Initial Code
*/

/** @ingroup    TI_IMG_COLOR_BLENDING_H */
/*@{*/
#ifndef TI_IMG_COLOR_BLENDING_H
#define TI_IMG_COLOR_BLENDING_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @func tiadalg_image_color_blending_cn
 *
 * @brief It does basic color blending to input image based on pixel level detection information provided by other module
 *
 * @param in_img             [in]  : list of input planes pointers
 * @param in_img_width       [in]  : Input image width
 * @param in_img_pitch       [in]  : Input image one line pitch
 * @param in_img_height      [in]  : Input image height
 * @param num_planes         [in]  : Num of planes in input image
 * @param in_color_format    [in]  : Input color format. 0 means YUV 420 nv12 format, 1 means RGB de-ineterleaved
 * @param out_color_format   [in]  : Output color format. 0 means YUV 420 nv12 format, 1 means RGB de-ineterleaved
 * @param valid_region       [in]  : valid region to be blended. It is expectecin [TL_x, TL_y, BR_x, BR_y]. For non valid region input is copied to output
 * @param det_plane          [in]  : Detection plane. Each pixel has values 0 to num_class-1
 * @param det_plane_pitch    [in]  : Detection plane one line pitch
 * @param det_scale_fact     [in]  : Detection plane values needs to be scaled by this factor
 * @param det_color_map_uv   [in]  : Interleaved U and V for each detection class
 * @param out_img            [out] : Output image buffer
 * @param out_img_pitch      [in]  : Output image one line pitch

 *
 * @note
 * - Number of valid pointers in the list \c in_img should be equal to \c num_planes
 * - Dimension of \c det_plane should be same as first plane of input image
 * - Data range for /c det_plane is assumed to be [0 - num_class-1]
 * - valid element in /c det_color_map_uv, should be equal to num_class*2
 *
 */

int32_t tiadalg_image_color_blending_cn(uint8_t* in_img[], int32_t in_img_width, int32_t in_img_pitch, int32_t in_img_height,
                                        int32_t num_planes, int32_t in_color_format, int32_t out_color_format, int32_t valid_region[4],
                                       uint8_t* det_plane, int32_t det_plane_pitch, float det_scale_fact, uint8_t* det_color_map_uv,
                                       uint8_t* out_img[], int32_t out_img_pitch);

/**
 * @func tiadalg_image_color_blending_c66
 *
 * @brief It does basic color blending to input image based on pixel level detection information provided by other module
 *
 * @param in_img             [in]  : list of input planes pointers
 * @param in_img_width       [in]  : Input image width
 * @param in_img_pitch       [in]  : Input image one line pitch
 * @param in_img_height      [in]  : Input image height
 * @param num_planes         [in]  : Num of planes in input image
 * @param in_color_format    [in]  : Input color format. 0 means YUV 420 nv12 format, 1 means RGB de-ineterleaved
 * @param out_color_format   [in]  : Output color format. 0 means YUV 420 nv12 format, 1 means RGB de-ineterleaved
 * @param valid_region       [in]  : valid region to be blended. It is expectecin [TL_x, TL_y, BR_x, BR_y]. For non valid region input is copied to output
 * @param det_plane          [in]  : Detection plane. Each pixel has values 0 to num_class-1
 * @param det_plane_pitch    [in]  : Detection plane one line pitch
 * @param det_scale_fact     [in]  : Detection plane values needs to be scaled by this factor
 * @param det_color_map_uv   [in]  : Interleaved U and V for each detection class
 * @param out_img            [out] : Output image buffer
 * @param out_img_pitch      [in]  : Output image one line pitch

 *
 * @note
 * - Currently only YUV420 nv12 format is supported for input and output for optimized kernel
 * - Number of valid pointers in the list \c in_img should be equal to \c num_planes
 * - Dimension of \c det_plane should be same as first plane of input image
 * - Data range for /c det_plane is assumed to be [0 - num_class-1]
 * - valid element in /c det_color_map_uv, should be equal to num_class*2
 *
 */

int32_t tiadalg_image_color_blending_c66(uint8_t* in_img[], int32_t in_img_width, int32_t in_img_pitch, int32_t in_img_height, int32_t num_planes,
                                          int32_t in_color_format, int32_t out_color_format, int32_t valid_region[4],
                                          uint8_t* det_plane, int32_t det_plane_pitch, float det_scale_fact, uint8_t* det_color_map_uv,
                                          uint8_t* out_img[], int32_t out_img_pitch);

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TI_IMG_COLOR_BLENDING_H */
