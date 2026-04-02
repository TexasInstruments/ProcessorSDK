/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
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
#if !defined(_API_MEMORY_IF_H_)
#define _API_MEMORY_IF_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Maximum number of memory blocks per request.
 *
 * \ingroup group_ptk_utils
 */
#define PTK_API_MAX_MEM_BLK_REQ     (8U)

/**
 * \brief Grid data type definition
 *
 * \ingroup group_ptk_utils
 */
typedef enum
{
    /** Closest memory type to the core. Memory type with lowest latency. */
    PTK_Api_MemoryType_LVL2 = 0x1,

    /** Level next to LVL2. Memory type with access latency higher than LVL2. */
    PTK_Api_MemoryType_LVL3 = 0x2,

    /** Level next to LVL3. Memory type with access latency higher than LVL3. */
    PTK_Api_MemoryType_LVL4 = 0x4,

    PTK_Api_MemoryType_MAX

} PTK_Api_MemoryType;

/**
 * \brief Single memory block allocation request details.
 *
 * \ingroup group_ptk_utils
 */
typedef struct
{
    /** Size of memory block in bytes. */
    uint32_t            size;

    /** Memory alignment requested. The alignment will be on (2^align)
     *  boundary.
     */
    uint32_t            align;

    /** Identifies the type of memory the block is requested from. This is a
     *  bitmask built from the PTK_Api_MemoryType enumeration fields.
     */
    uint32_t            memType;

} PTK_Api_MemoryBlkReq;

/**
 * \brief API memory allocation request.
 *
 * \ingroup group_ptk_utils
 */
typedef struct
{
    /** Number of memory blocks requested. */
    uint32_t                numBlks;

    /** Memory blocks requests. */
    PTK_Api_MemoryBlkReq    blks[PTK_API_MAX_MEM_BLK_REQ];

} PTK_Api_MemoryReq;

/**
 * \brief Single memory block allocation response details.
 *
 * \ingroup group_ptk_utils
 */
typedef struct
{
    /** Size of memory block in bytes. */
    uint32_t            size;

    /** Identifies the type of memory the block is allocated from. */
    PTK_Api_MemoryType  memType;

    /** Pointer to the memory block allocated. */
    void              * mem;

} PTK_Api_MemoryBlkRsp;

/**
 * \brief API memory allocation response.
 *
 * \ingroup group_ptk_utils
 */
typedef struct
{
    /** Number of memory blocks requested. */
    uint32_t                numBlks;

    /** Memory blocks requests. */
    PTK_Api_MemoryBlkRsp    blks[PTK_API_MAX_MEM_BLK_REQ];

} PTK_Api_MemoryRsp;

#ifdef __cplusplus
}
#endif

#endif /* !defined(_API_MEMORY_IF_H_) */

