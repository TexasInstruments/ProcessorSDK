/*
*
* Copyright (c) {2015 - 2017} Texas Instruments Incorporated
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
@file      tiadalg_common_data_struct.h
@brief     This file defines common data structures for all the modules
@version 0.1 Sep 2018 : Initial Code
*/

/** @ingroup    TIADALG_COMMON_DATA_STRUCTURE_H */
/*@{*/
#ifndef TIADALG_COMMON_DATA_STRUCT_H
#define TIADALG_COMMON_DATA_STRUCT_H

#include <stdint.h>
#include <stdlib.h>

#ifdef HOST_EMULATION
#define restrict
#endif
/** @enum e_tiadalg_data_type
 *  @brief Enumeration to indicate the data typ processing. In consistant with TIDL elementType
 *
 */
typedef enum
{
  TIADALG_DATA_TYPE_U08 = 0,
  TIADALG_DATA_TYPE_S08 = 1,
  TIADALG_DATA_TYPE_U16 = 2,
  TIADALG_DATA_TYPE_S16 = 3,
  TIADALG_DATA_TYPE_U32 = 4,
  TIADALG_DATA_TYPE_S32 = 5,
  TIADALG_DATA_TYPE_F32 = 6
}e_tiadalg_data_type;

/** @enum e_tiadalg_color_conv_type
 *  @brief Enumeration to indicate various color conversion from input to output in preprocessing API
 *
 */
typedef enum
{
  TIADALG_COLOR_CONV_NONE                 = 0,
  TIADALG_COLOR_CONV_YUV420_RGB           = 1,
  TIADALG_COLOR_CONV_YUV420_BGR           = 2,
  TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR    = 3,
  TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB    = 4,
  TIADALG_COLOR_CONV_RGB_YUV420           = 5,
  TIADALG_COLOR_CONV_BGR_YUV420           = 6
}e_tiadalg_color_conv_type;

/** @enum e_tiadalg_return_type
 *  @brief Enumeration to indicate various return type from process
 *
 */
typedef enum
{
  TIADALG_PROCESS_SUCCESS = 0,
  TIADALG_IN_PRM_ERR = 1,
  TIADALG_IN_BUF_ERR = 2,
  TIADALG_OUT_BUF_ERR = 3,
  TIADALG_PROCESS_FAILURE = 4
}e_tiadalg_return_type;


typedef struct{
  int32_t width;
  int32_t pitch;
  int32_t height;
  int32_t num_planes;
  int32_t bytes_per_element;
  int32_t element_pitch; /*Pitch for one pixel to another pixel. Assumed that all the pixels of same plane are at same pitch*/
  int32_t plane_pitch; /*Offset of one plane of data to another plane. Assumed that all the planes are seperated by same offset*/
  void* data;
  int32_t is_user_provided_data_ptr;
}tiadalg_buffer_context;

/**
  @struct   tiadalg_p3p_params
  @brief    P3P solution parmaters
 */
typedef struct tiadalg_p3p_params
{
  /** Camera focal length in x direction*/
	float fx;
  /** Camera focal length in y direction*/
	float fy;
  /** Image focal center in x direction*/
	float cx;
  /** Image focal center in y direction*/
	float cy;
  /** Maximum iteration count for ransac solution*/
	int iterationsCount;
  /** reprojection error threshold*/
	float reprojErrorThreshold;
  /** Inlier ratio*/
	float inliersRatio;
	uint64_t seed;
}tiadalg_p3p_params;

#endif
