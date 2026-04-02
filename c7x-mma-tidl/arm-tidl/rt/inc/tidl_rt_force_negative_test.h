/*
*
* Copyright (c) {2015 - 2025} Texas Instruments Incorporated
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
@file      tidl_rt_force_negative_test.h
brief      This file defines the various force negative tests for arm-tidl\n

@version 0.1 July 2025 : Initial Code
*/

/** @ingroup  TIDL_FORCE_NEGATIVE_TEST */

/*@{*/
#ifndef TIDL_FORCE_NEGATIVE_TEST_H_
#define TIDL_FORCE_NEGATIVE_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER                             (-1000)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_DEINIT                           (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 0)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_PRMS_DEFAULT                     (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 1)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_LOAD_UNLOAD_KERNELS              (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 2)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_HANDLE_FAILURE                   (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 3)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_TRACE_SUBGRAPH_NAME              (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 4)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_TRACE_BASE_NAME_NULL             (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 5)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_ALLOC_DELLOC_SHARED_TENSORS      (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 6)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_ALLOC_SHARED_TENSORS             (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 7)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INIT_OPTEXTMEM                   (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 8)
#define TIDL_SAFETY_FLAG_TIDL_RT_UTILS_FORCE_PADEDBUF_ZERO              (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 9)
#define TIDL_SAFETY_FLAG_TIDL_RT_ALLOC_FORCE_VPRINTF_NULL               (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 10)
#define TIDL_SAFETY_FLAG_TIDL_RT_ALLOC_FORCE_DMACREATEOBJ_NULL          (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 11)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_PREEMPTION_DISABLED              (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 12)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INIT1_FNT                        (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 13)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INIT2_FNT                        (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 14)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INIT3_FNT                        (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 15)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_PROC1_FNT                        (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 16)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_PROC2_FNT                        (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 17)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_CTRL_NFNT                        (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 18)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_TIOVX_ALLOC_FAILURE              (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 19)

#define TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES                   (TIDL_SAFETY_FLAG_TIDL_RT_FNT_NUMBER - 100)

// Create time failures
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_TIOVX_INIT_FAILURE               (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 0)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_CONTEXT_FAILURE                  (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 1)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_KERNEL_FAILURE                   (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 2)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_CREATE_GRAPH_FAILURE             (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 3)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_CREATE_NODE_AND_GRAPH_FAILURE    (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 4)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_CONFIG_FAILURE                   (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 5)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_NETWORK_FAILURE                  (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 6)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_CREATE_PARAMS_FAILURE            (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 7)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INARGS_FAILURE                   (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 8)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_OUTARGS_FAILURE                  (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 9)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_TRACE_DATA_FAILURE               (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 10)

#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INCORRECT_CONFIG_CHECKSUM        (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 11)

#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_EXPORT_NETWORK_FAILURE           (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 12)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_FREE_NET_FAILURE                 (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 13)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_NET_BUF_FAILURE                  (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 14)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_RELEASE_VX_NET_FAILURE           (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 15)

#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_EXPORT_TENSOR_BUFFER_FAILURE     (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 16)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_FREE_TENSOR_BUF_FAILURE          (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 17)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_TENSOR_BUF_FAILURE               (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 18)

#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_EXCEED_MAX_KERNEL_PARAMS_FAILURE (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 19)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_SHUFFLE_KERNEL_PARAMETER_FAILURE (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 20)

#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_VERIFY_FAILURE                   (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 21)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INTERMEDIATE_ALLOC_FAILURE       (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 22)

#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_CAPACITY_ZERO                    (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 23)


#define TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES                  (TIDL_SAFETY_FLAG_TIDL_RT_CREATE_TIME_FAILURES - 50)

// Process time failures (Indirect ones from create are added here) 
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INCORRECT_NETWORK_CHECKSUM       (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 0)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_TRACE_DATA_SIZE_1                (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 1)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_RELEASE_INPUT_TENSORS            (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 2)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_RELEASE_OUTPUT_TENSORS           (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 3)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_OUTARGS_NULL                     (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 4)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_PROCESS_FAILURE                  (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 5)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_MULTI_INVOKE_AND_NULL            (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 6)

#define TIDL_SAFETY_FLAG_TIDL_RT_DELETE_TIME_FAILURES                   (TIDL_SAFETY_FLAG_TIDL_RT_PROCESS_TIME_FAILURES - 50)

// Delete time failures
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_RELEASE_TENSORS_AFTER_PROCESS    (TIDL_SAFETY_FLAG_TIDL_RT_DELETE_TIME_FAILURES - 0)
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_RELEASE_TENSOR_BEFORE_DEINIT		(TIDL_SAFETY_FLAG_TIDL_RT_DELETE_TIME_FAILURES - 1)

// Force negative tests for TIDLNodeV2 implementation (added for new vx_object_array interface)
#define TIDL_SAFETY_FLAG_TIDL_RT_TIDLNODEV2_BASE                        (TIDL_SAFETY_FLAG_TIDL_RT_DELETE_TIME_FAILURES - 100)

// Tests for dsp/vx_tidl_target.c - tivxKernelTIDLProcess function
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_INVALID_INPUT_TYPE               (TIDL_SAFETY_FLAG_TIDL_RT_TIDLNODEV2_BASE - 0)

// Tests for mpu/vx_tidl_target.c - tivxKernelTIDLCreate function
#define TIDL_SAFETY_FLAG_TIDL_RT_FORCE_OBJARRAY_CREATION_FAIL           (TIDL_SAFETY_FLAG_TIDL_RT_TIDLNODEV2_BASE - 1)

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* TIDL_FORCE_NEGATIVE_TEST_H_ */
