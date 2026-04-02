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
#include <stdint.h>
#include <stdlib.h>

#ifndef TIADALG_COMMONUTILS_H
#define TIADALG_COMMONUTILS_H
#define TIADALG_FILE_NAME_SIZE    (256)

int32_t tiadalg_read_f32_data_from_file(float* dest, int32_t num_elements, const char * filename);
int32_t tiadalg_read_s16_data_from_file(int16_t* dest, int32_t num_elements, const char * filename);
int32_t tiadalg_read_s8_data_from_file(int8_t* dest, int32_t num_elements, const char * filename);
int32_t tiadalg_comp_f32_data(float* src, float* dst, int32_t num_elements, float tolerance);
int32_t tiadalg_comp_s32_data(int32_t* src, int32_t* dst, int32_t num_elements, int32_t tolerance);
int32_t tiadalg_comp_s16_data(int16_t* src, int16_t* dst, int32_t num_elements, int16_t tolerance);
int32_t tiadalg_comp_u16_data(uint16_t* src, uint16_t* dst, int32_t num_elements, uint16_t tolerance);
int32_t tiadalg_read_bin_file(void* dest, int32_t num_bytes, char * file_name);
int32_t tiadalg_write_bin_file(void* dest, int32_t num_bytes, char * file_name);
int32_t tiadalg_fill_buffer_random(uint8_t* buf, int32_t size);
int32_t tiadalg_fill_buffer_range_random_16u(uint16_t* buf, int32_t size, uint16_t low, uint16_t high);
int32_t tiadalg_fill_buffer_range_random_8u(uint8_t* buf, int32_t size, uint8_t low, uint8_t high);

void tiadalg_init_cache();

#if !defined(HOST_EMULATION) && !defined(TIADALG_FREERTOS_BUILD) && defined(__C6600__)
#define L2CFG    (volatile unsigned int *)0x01840000
#define L1PCFG   (volatile unsigned int *)0x01840020
#define L1DCFG   (volatile unsigned int *)0x01840040
#endif

static int32_t inline gen_rand(int32_t min, int32_t max)
{
  int32_t range = max - min + 1;

  int32_t cur_rand = (rand() % range) - ((max + min) >> 1);

  if(cur_rand < min) cur_rand = min;

  if(cur_rand > max) cur_rand = max;

  return(cur_rand);
}

#if !defined(HOST_EMULATION) // in host emulation build of test application platform header file should not be required

#if (defined(__C7100__) || defined(__C7120__)) 
#include <c7x.h>
void displaySEParam(void *baseAddr, __SE_TEMPLATE_v1 seTemplate);
#endif
#endif

#endif
