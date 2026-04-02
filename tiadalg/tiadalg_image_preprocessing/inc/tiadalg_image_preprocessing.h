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

/** @ingroup    TIADALG_IMG_PREPROCESSING_H */
/*@{*/
#ifndef TI_IMG_PROCESSING_H
#define TI_IMG_PROCESSING_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @func tiadalg_image_preprocessing_cn
 *
 * @brief It does basic image processing like plane deinterleaving,
 *  mean subtraction, scaling and padding. This API is intended to do
 *  image preprocessing for images beore feeding it to Deep Learning
 *  networks.
 *
 * @param in_img             [in]  : Input Image buffer
 * @param in_width           [in]  : Input image width
 * @param in_height          [in]  : Input image height
 * @param in_stride          [in]  : Input image stride
 * @param data_type          [in]  : data type of output buffer. Input data type is always assumed as U08
 * @param color_conv_type    [in]  : input to output color conversion flag
 * @param scale_val          [in]  : Scale value for each output plane pixels
 * @param mean_pixel         [in]  : Mean value to be subtracted from each output plane pixels
 * @param pad_pixel          [in]  : Number of pixel to be padded for output plane in left, top, right and bottom sides.
 * @param out_img            [out] : Output buffer, where data is generated continiously in memory
 *
 * @note
 * - Currently only 8 bit data type is supported
 * - if Pixel subtraction values (\c mean_pixel) shoould be given in output plane order.
 * - if Pixel scaling values (\c scale_val) shoould be given in output plane order.
 * - Processing happens in floating point, and finally data is kept in U08/U16/S8/S16 container
 * - For U16/S16 output buffer data is clipped according U8/S8 to match optmized flow, where processing happens alwaysin 8 bit domain.
 *
 */

  int32_t tiadalg_image_preprocessing_cn(void* in_img[], int32_t in_width, int32_t in_height, int32_t in_stride,
    int32_t data_type, int32_t color_conv_type, float scale_val[],
    float mean_pixel[], int32_t pad_pixel[4], void* out_img);

/**
 * @func tiadalg_image_preprocessing_c66
 *
 * @brief It does basic image processing like plane deinterleaving,
 *  mean subtraction, scaling and padding. This API is intended to do
 *  image preprocessing for images beore feeding it to Deep Learning
 *  networks.
 *
 * @param in_img             [in]  : Input Image buffer
 * @param in_width           [in]  : Input image width
 * @param in_height          [in]  : Input image height
 * @param in_stride          [in]  : Input image stride
 * @param data_type          [in]  : data type of output buffer. Input data type is always assumed as U08
 * @param color_conv_type    [in]  : input to output color conversion flag
 * @param scale_val          [in]  : Scale value for each output plane pixels
 * @param mean_pixel         [in]  : Mean value to be subtracted from each output plane pixels
 * @param pad_pixel          [in]  : Number of pixel to be padded for output plane in left, top, right and bottom sides.
 * @param out_img            [out] : Output buffer, where data is generated continiously in memory
 *
 * @note
 * - Currently only 8 bit data type is supported
 * - if Pixel subtraction values (\c mean_pixel) shoould be given in output plane order.
 * - if Pixel scaling values (\c scale_val) shoould be given in output plane order.
 * - For 16 bit output flow , all processing happens for 8b and at last output is just put in 16 container
 *
 */

  int32_t tiadalg_image_preprocessing_c66(void* in_img[], int32_t in_width, int32_t in_height, int32_t in_pitch,
    int32_t data_type, int32_t color_conv_type, float* scale_val,
    float* mean_pixel, int32_t pad_pixel[4], void* out_img);

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TI_IMG_PREPROCESSING_H */
