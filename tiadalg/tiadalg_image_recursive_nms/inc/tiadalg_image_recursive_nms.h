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
@file      tiadalg_image_processing.h
@brief     This file defines the interface for TI image processing module
@version 0.1 Sep 2018 : Initial Code
*/

/** @ingroup    TI_IMG_RECURSIVE_NMS_H */
/*@{*/
#ifndef TI_IMG_RECURSIVE_NMS_H
#define TI_IMG_RECURSIVE_NMS_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @func tiadalg_image_recursive_nms_cn
 *
 * @brief It does scores buffer thresholding, and then NMS is performed. This is helpful
 *        in the scenario where score are sturated in clusters.
 *
 * @param score_buf          [in]  : Input 8b score buffer
 * @param img_width          [in]  : Input image width corresponding to \c score_buf
 * @param img_pitch          [in]  : Input image width pitch corresponding to \c score_buf
 * @param img_height         [in]  : Input image height corresponding to \c score_buf
 * @param cur_score_th       [in]  : score threshold value
 * @param max_frame_feat     [in]  : maximum feature points
 * @param guard_pixels       [in]  : guard pixels to be avoided in each direction of x and y.
 * @param scratch            [in]  : scratch buffer
 * @param data_type          [in]  : score_buf data type
 * @param out_buf            [out] : interleaved x and y of each feature points
 *
 * @note
 * - Step 1 if score_buf[i] < cur_score_th, then score_buf[i] = 0
 * - Step 2 Then 3x3 NMS is performed on filtered out scores
 * - Size of the buffer \c scratch is \c img_width * 2
 * - \c out_buf will have feature points x and y co-ordinated interleaved. x0,y0,x1,y1,x2,y2,x3,y3,... so on
 * - return value is total number of generated feature points
 */

int32_t tiadalg_image_recursive_nms_cn(int8_t* score_buf, int32_t img_width, int32_t img_pitch, int32_t img_height,
                                       int32_t cur_score_th, int32_t max_frame_feat,
                                       int32_t guard_pixels, int8_t* scratch, int32_t data_type,
                                       int32_t* out_buf);

/**
 * @func tiadalg_image_recursive_nms_c66
 *
 * @brief It does scores buffer thresholding, and then NMS is performed. This is helpful
 *        in the scenario where score are sturated in clusters.
 *
 * @param score_buf          [in]  : Input 8b score buffer
 * @param img_width          [in]  : Input image width corresponding to \c score_buf
 * @param img_pitch          [in]  : Input image width pitch corresponding to \c score_buf
 * @param img_height         [in]  : Input image height corresponding to \c score_buf
 * @param cur_score_th       [in]  : score threshold value
 * @param max_frame_feat     [in]  : maximum feature points
 * @param guard_pixels       [in]  : guard pixels to be avoided in each direction of x and y.
 * @param scratch            [in]  : scratch buffer
 * @param data_type          [in]  : score_buf data type
 * @param out_buf            [out] : interleaved x and y of each feature points
 *
 * @note
 * - Step 1 if score_buf[i] < cur_score_th, then score_buf[i] = 0
 * - Step 2 Then 3x3 NMS is performed on filtered out scores
 * - Size of the buffer \c scratch is \c img_width * 2
 * - \c out_buf will have feature points x and y co-ordinated interleaved. x0,y0,x1,y1,x2,y2,x3,y3,... so on
 * - return value is total number of generated feature points
 */
int32_t tiadalg_image_recursive_nms_c66(int8_t* score_buf, int32_t img_width, int32_t img_pitch, int32_t img_height,
                                        int32_t cur_score_th, int32_t max_frame_feat,
                                        int32_t guard_pixels, int8_t* scratch, int32_t data_type,
                                        int32_t* out_buf);

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TI_IMG_RECURSIVE_NMS_H */
