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
@file      tiadalg_alg_int_interface.h
@brief     This file defines internal common data structures for all the modules
@version 0.1 Sep 2018 : Initial Code
*/

/** @ingroup    TIADALG_ALG_INT_INTERFACE_H */
/*@{*/
#ifndef TIADALG_ALG_INT_INTERFACE_H
#define TIADALG_ALG_INT_INTERFACE_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define TIADALG_PI (3.14159265358f)

// these includes are needed for library building, and needed in host emulation also to emulate instructions
#if defined(__C7100__) || defined(__C7120__) || defined(BUILD_C7X_1)
#include <c7x.h>
#include <c6x_migration.h>
#else // __C6600__
#ifndef HOST_EMULATION
#include <c6x.h>
#else
#include "./../common/c_intrinsics_host_port/c6xsim/C6xSimulator.h"
#endif
#endif

#ifdef HOST_EMULATION
#ifndef restrict
#define restrict
#endif
#define _nassert assert
#endif

#ifndef mmax
#define mmax(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef mmin
#define mmin(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef msign
#define msign(x)  (x >= 0)?1:0
#endif

#ifndef SWAP_ME
#define SWAP_ME(a,b) a=a+b;b=a-b;a=a-b;
#endif

#define TIADALG_oneByXF32(x) 1/x

#define VXLIB_STATUS int32_t
#define VXLIB_SUCCESS (1)
#define VXLIB_ERR_FAILURE (0)
#define VXLIB_F32 float
#define VXLIB_D64 double

static size_t inline addr_align(void* addr, int32_t num_byte_align)
{
  size_t new_addr = (size_t)(addr);
  size_t mask = ~(num_byte_align-1);
  new_addr = new_addr + num_byte_align - 1;
  new_addr = new_addr & mask;
  return(new_addr);
}

#endif
