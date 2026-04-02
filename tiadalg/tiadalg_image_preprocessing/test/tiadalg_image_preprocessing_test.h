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

#include <../common/tiadalg_test_interface.h>

uint8_t test_img_1[6*3*3] =
{
40, 80, 120,   160, 200, 240,   255, 0, 254,
40, 80, 120,   160, 200, 240,   255, 0, 254,
40, 80, 120,   160, 200, 240,   255, 0, 254,
40, 80, 120,   160, 200, 240,   255, 0, 254,
40, 80, 120,   160, 200, 240,   255, 0, 254,
40, 80, 120,   160, 200, 240,   255, 0, 254
};

uint8_t test_img_2[6*3*3] =
{
  40, 160,255,
  40, 160,255,
  40, 160,255,
  40, 160,255,
  40, 160,255,
  40, 160,255,
  80, 200,0,
  80, 200,0,
  80, 200,0,
  80, 200,0,
  80, 200,0,
  80, 200,0,
  120,240,254,
  120,240,254,
  120,240,254,
  120,240,254,
  120,240,254,
  120,240,254
};

uint8_t test_img_3[6*3*3] =
{
  120,240,254,120,240,254,120,240,254,
  120,240,254,120,240,254,120,240,254,
  80, 200,0,80, 200,0,80, 200,0,
  80, 200,0,80, 200,0,80, 200,0,

  40, 160,255,40, 160,255,40, 160,255,
  40, 160,255,40, 160,255,40, 160,255
};
uint8_t test_img_4[10*4 + 10*2] =
{
40, 80, 120,   160, 200, 240,   255, 0, 254, 128,
40, 80, 120,   160, 200, 240,   255, 0, 254, 128,
40, 80, 120,   160, 200, 240,   255, 0, 254, 128,
40, 80, 120,   160, 200, 240,   255, 0, 254, 128,

40, 80, 120,   160, 200, 240,   255, 0, 254, 128,
40, 80, 120,   160, 200, 240,   255, 0, 254, 128
};

uint8_t test_img_5[10*4*3] =
{
  0,0,172,218,255,255,73,0,255,130,
  0,0,172,218,255,255,73,0,255,130,
  0,0,172,218,255,255,73,0,255,130,
  0,0,172,218,255,255,73,0,255,130,

  101,147,98,144,95,141,255,35,227,81,
  101,147,98,144,95,141,255,35,227,81,
  101,147,98,144,95,141,255,35,227,81,
  101,147,98,144,95,141,255,35,227,81,

  0,0,104,151,255,255,255,237,255,255,
  0,0,104,151,255,255,255,237,255,255,
  0,0,104,151,255,255,255,237,255,255,
  0,0,104,151,255,255,255,237,255,255,

};

uint8_t test_img_6[10*4*3] =
{

  0,0,104,151,255,255,255,237,255,255,
  0,0,104,151,255,255,255,237,255,255,
  0,0,104,151,255,255,255,237,255,255,
  0,0,104,151,255,255,255,237,255,255,

  101,147,98,144,95,141,255,35,227,81,
  101,147,98,144,95,141,255,35,227,81,
  101,147,98,144,95,141,255,35,227,81,
  101,147,98,144,95,141,255,35,227,81,

  0,0,172,218,255,255,73,0,255,130,
  0,0,172,218,255,255,73,0,255,130,
  0,0,172,218,255,255,73,0,255,130,
  0,0,172,218,255,255,73,0,255,130
};

static int32_t num_planes = 3;
static float mean_pixel1[3] = {103.94f,116.78f,123.68f};
static float scale_val1[3] = {1.2f, 1.5f, 2.6f};

static float mean_pixel2[3] = {0.0f, 0.0f, 0.0f};
static float scale_val2[3] = {1.0f, 1.0f, 1.0f};

typedef struct {
    int32_t in_data_type; /*0 -> static, 1 -> random, 2 -> file*/
    int32_t out_data_type;
    uint8_t *in_img;
    uint8_t *ref_out_img;
    int32_t img_width;
    int32_t img_height;
    int32_t num_planes;
    float * mean_pixel;
    float * scale_val;
    int32_t color_conv_type;
} tiadalg_image_preprocessing_testParams_t;

static tiadalg_image_preprocessing_testParams_t test_cases[] =
{
  {
    1,
    TIADALG_DATA_TYPE_U08,
    0x0,
    0x0,
    768,
    16,
    num_planes,
    mean_pixel2,
    scale_val2,
    TIADALG_COLOR_CONV_YUV420_RGB,
   },
  {
   1,
   TIADALG_DATA_TYPE_U16,
   0x0,
   0x0,
   768,
   16,
   num_planes,
   mean_pixel2,
   scale_val2,
   TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB,
  },
  {
   1,
   TIADALG_DATA_TYPE_U16,
   0x0,
   0x0,
   768,
   16,
   num_planes,
   mean_pixel2,
   scale_val2,
   TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR,
  },
  {
    0,
    TIADALG_DATA_TYPE_U08,
    test_img_4,
    0x0,
    10,
    4,
    num_planes,
    mean_pixel2,
    scale_val2,
    TIADALG_COLOR_CONV_YUV420_BGR,
   },
  {
    0,
    TIADALG_DATA_TYPE_U08,
    test_img_4,
    0x0,
    10,
    4,
    num_planes,
    mean_pixel2,
    scale_val2,
    TIADALG_COLOR_CONV_YUV420_RGB,
   },
  {
   0,
   TIADALG_DATA_TYPE_U08,
   test_img_1,
   0x0,
   3,
   6,
   num_planes,
   mean_pixel2,
   scale_val2,
   TIADALG_COLOR_CONV_RGBINTERLEAVE_RGB,
  },
  {
   0,
   TIADALG_DATA_TYPE_U08,
   test_img_1,
   0x0,
   3,
   6,
   num_planes,
   mean_pixel2,
   scale_val2,
   TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR,
  },
  {
   0,
   TIADALG_DATA_TYPE_U08,
   test_img_1,
   0x0,
   18,
   1,
   num_planes,
   mean_pixel2,
   scale_val2,
   TIADALG_COLOR_CONV_RGBINTERLEAVE_BGR,
  }
};

