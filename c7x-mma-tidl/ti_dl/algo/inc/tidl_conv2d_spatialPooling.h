/*
*
* Copyright (c) {2015 - 2020} Texas Instruments Incorporated
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
 ----------------------------------------------------------------------------
 @file    tidl_conv2d_spatialPooling.h
 @brief   This file defines process function prototype of spatial pooling layer
 @version 0.1 (Dec 2016) : Initial version [TVS]
 ----------------------------------------------------------------------------
*/

#ifndef ITIDL_CONV2D_SPATIALPOOLING_H
#define ITIDL_CONV2D_SPATIALPOOLING_H

#include "tidl_alg_int.h"
#define SAT_LO_UCHAR           (0)
#define SAT_HI_UCHAR           (255)
static uint8_t TIDL_clip_i16u_o8u(uint16_t x);
static uint8_t TIDL_clip_i16u_o8u(uint16_t x)
{
  uint8_t y;

  y = (x > (uint16_t)SAT_HI_UCHAR) ? (uint16_t)SAT_HI_UCHAR : x;

  return (y);
}

void VXLIB_cnnAvgPooling_2x2_skip2_i8u
(
  uint8_t     pInChannel[],  
  uint16_t    width,         
  uint16_t    height,        
  uint16_t    inPitch,       
  uint16_t    outPitch,      
  uint16_t    numOutChannels,
  uint8_t     pOutChannel[], 
  uint16_t    roundBits      
);


typedef struct {
  uint16_t    width;
  uint16_t    height;
  uint16_t    inPitch;
  uint16_t    outPitch;
  uint32_t    inChPitch;
  uint32_t    outChPitch;
  uint16_t    numOutChannels;
  uint16_t    roundBits;
}tidlConv2dPoolingParams_t;

template <class T>
void TIDL_refAvgPooling_2x2_skip2_cn
(
  T*     pInChannel,
  T*     pOutChannel,
  tidlConv2dPoolingParams_t * params,
  T      min,
  T      max
  );
template <class T>
void TIDL_refMaxPooling_2x2_skip2_cn
(
  T*     pInChannel,
  T*     pOutChannel,
  tidlConv2dPoolingParams_t * params
  );

template <class T>
void TIDL_refMaxPooling_2x2_skip2_cn
(
  T*     pInChannel,
  T*     pOutChannel,
  tidlConv2dPoolingParams_t * params
  )
{
  uint16_t    width = params->width;
  uint16_t    height = params->height;
  uint16_t    inPitch = params->inPitch;
  uint16_t    outPitch = params->outPitch;
  uint16_t    numOutChannels = params->numOutChannels;
  uint32_t    inChPitch = params->inChPitch;
  uint32_t    outChPitch = params->outChPitch;

  T inRow0Col0, inRow0Col1;
  T inRow1Col0, inRow1Col1;
  T maxvalue0, maxvalue1, maxvalueBlock;
  uint16_t i1, i2, i3;

  for (i1 = 0; i1 < numOutChannels; i1++)
  {
    for (i2 = 0; i2 < (height >> 1); i2++)
    {
      for (i3 = 0; i3 < (width >> 1); i3++)
      {
        inRow0Col0 = pInChannel[(i1* inChPitch) + ((i2 << 1) * inPitch) +
          (i3 << 1)];
        inRow1Col0 = pInChannel[(i1* inChPitch) + (((i2 << 1) + 1U) * inPitch) +
          (i3 << 1)];

        inRow0Col1 = pInChannel[(i1* inChPitch) + ((i2 << 1) * inPitch) +
          (i3 << 1) + 1U];
        inRow1Col1 = pInChannel[(i1* inChPitch) + (((i2 << 1) + 1U) * inPitch) +
          (i3 << 1) + 1U];

        /* Maxpooling operations */
        maxvalue0 = (inRow0Col0 > inRow1Col0) ? inRow0Col0 : inRow1Col0;
        maxvalue1 = (inRow0Col1 > inRow1Col1) ? inRow0Col1 : inRow1Col1;
        maxvalueBlock = (maxvalue0 > maxvalue1) ? maxvalue0 : maxvalue1;

        pOutChannel[(i1* outChPitch) + (i2 * outPitch) + i3] = maxvalueBlock;
      }
    }
  }
}

template <class T>
void TIDL_refAvgPooling_2x2_skip2_cn
(
  T*     pInChannel,
  T*     pOutChannel,
  tidlConv2dPoolingParams_t * params,
  T      min,
  T      max
  )
{
  uint16_t    width = params->width;
  uint16_t    height = params->height;
  uint16_t    inPitch = params->inPitch;
  uint16_t    outPitch = params->outPitch;
  uint16_t    numOutChannels = params->numOutChannels;
  uint16_t    roundBits = params->roundBits;
  uint32_t    inChPitch = params->inChPitch;
  uint32_t    outChPitch = params->outChPitch;

  T inRow0Col0, inRow0Col1;
  T inRow1Col0, inRow1Col1;
  uint32_t sumValueBlock;
  int32_t result;
  uint32_t i1, i2, i3;

  for (i1 = 0; i1< numOutChannels; i1++)
  {
    for (i2 = 0; i2 <(height >> 1); i2++)
    {
      for (i3 = 0; i3 < (width >> 1); i3++)
      {
        inRow0Col0 = pInChannel[(i1* inChPitch) + ((i2 << 1) * inPitch) +
          (i3 << 1)];
        inRow1Col0 = pInChannel[(i1* inChPitch) + (((i2 << 1) + 1U) * inPitch) +
          (i3 << 1)];

        inRow0Col1 = pInChannel[(i1* inChPitch) + ((i2 << 1) * inPitch) +
          (i3 << 1) + 1U];
        inRow1Col1 = pInChannel[(i1* inChPitch) + (((i2 << 1) + 1U) * inPitch) +
          (i3 << 1) + 1U];

        /* Maxpooling operations */
        sumValueBlock = inRow0Col0 + inRow1Col0 + inRow0Col1 + inRow1Col1;

        /* Rounding and saturation */
        if (roundBits != 0U)
        {
          sumValueBlock += (1U << (roundBits - 1U));
          sumValueBlock >>= roundBits;
        }
        result = (sumValueBlock > (uint32_t)max) ? ((uint32_t)max) : (sumValueBlock);
        result = (sumValueBlock < (uint32_t)min) ? ((uint32_t)min) : (sumValueBlock);

        pOutChannel[(i1* outChPitch) + (i2 * outPitch) + i3] = result;
      }
    }
  }
}


#endif /* ITIDL_CONV2D_SPATIALPOOLING_H*/

