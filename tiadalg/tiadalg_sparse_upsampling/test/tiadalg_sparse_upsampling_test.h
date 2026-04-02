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

typedef struct {

    char * in_score_file;
    char * in_desc_file;
    char * in_filter_coeff_file;
    char * in_filter_bis_file;
    char * ref_desc_file;
    uint8_t score_th;
    int32_t max_key_points;
    int32_t score_width;
    int32_t score_height;
    int32_t desc_plane_width;
    int32_t desc_plane_height;
    int32_t num_desc_planes;
    int32_t in_desc_scale_pw2;
    int32_t in_filter_scale_pw2;
    int32_t in_bias_scale_pw2;
    int32_t out_desc_scale;

} tiadalg_sparse_upsampling_testParams_t;

static tiadalg_sparse_upsampling_testParams_t test_cases[] =
{
#if 0
  {
   "./../data/key_point_score.bin",
   0x0,
   "./../data/module.decoders.0.upsample2.1_shape_65_1_7_7.bin",
   "./../data/module.decoders.0.upsample2.1_shape_65.bin",
   0x0,
   128,
   20000,
   768,
   384,
   192,
   96,
   64,
   -1,
   9,
   16,
   -1
  },
#endif
  {
   (char*)"./../data/key_point_score.bin",
   (char*)"./../data/module.decoders.0.upsample2.0_shape_1_65_96_192.bin",
   (char*)"./../data/module.decoders.0.upsample2.1_shape_65_1_7_7.bin",
   (char*)"./../data/module.decoders.0.upsample2.1_shape_65.bin",
   0x0,
   128,
   20000,
   768,
   384,
   192,
   96,
   64,
   -1,
   9,
   16,
   -1
  }
};

