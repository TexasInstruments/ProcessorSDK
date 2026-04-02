/*
 * Copyright (c) 2021-2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_HeapP HeapP
 *            HeapP interface
 *
 *  @{
 */
/*! ============================================================================
 *  @file       HeapP.h
 *
 *  @brief      Heap management module for the RTOS Porting Interface.
  *
 *  ============================================================================
 */

#ifndef ti_osal_HeapP__include
#define ti_osal_HeapP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 *  @brief    Status codes for HeapP APIs
 */
typedef enum HeapP_Status_e
{
    /*! API completed successfully */
    HeapP_OK         = 0,
    /*! API failed */
    HeapP_FAILURE    = (-(int32_t)1)
} HeapP_Status;

/*!
 *  @brief    Opaque client reference to an instance of a HeapP
 *
 *  A HeapP_Handle returned from the ::HeapP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *HeapP_Handle;

/*!
 *  @brief    Basic HeapP Parameters
 *
 *  Structure that contains the parameters are passed into ::HeapP_create
 *  when creating a HeapP instance.
 */
typedef struct HeapP_Params_s
{
    void     *buf;          /*!< Base address of memory to be used as heap */
    uint32_t size;          /*!< Size of memory block that is to be used as heap */
    uint32_t align;         /*!< Minimum alignment for each allocation in the heap, in units of bytes.  Should be power of 2. */
} HeapP_Params;

/*!
 * @brief Structure used to pass information about the heap out of HeapP_getHeapStats().
 */
typedef struct HeapP_MemStats_s
{
    uint32_t totalSize;         /*!< Total size of heap, in bytes. */
    uint32_t totalFreeSize;     /*!< Current size of free memory in the heap, in bytes. */
    uint32_t largestFreeSize;   /*!< Current largest contiguous free block (in bytes) within the heap at the time HeapP_getHeapStats is called. */
} HeapP_MemStats;

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void HeapP_Params_init(HeapP_Params *params);

/*!
 * @brief Create a user defined heap
 *
 * The actual heap start address and size will be adjusted to satisfy
 * the align parameter.
 *
 *  @param  params  Pointer to the instance configuration parameters.
 * 
 *  @return A HeapP_Handle on success or a NULL on an error
 */
extern HeapP_Handle HeapP_create(const HeapP_Params *params);

/*!
 * @brief Delete the user defined heap
 *
 * @param handle    [in] Heap handle
 */
extern HeapP_Status HeapP_delete(HeapP_Handle handle);

/*!
 * @brief Alloc memory from user defined heap
 *
 * @param handle    [in] Heap handle
 * @param allocSize [in] Size of memory to allocate
 *
 * NOTE: Application should make sure that there is no buffer overflow. 
 * i.e., writing to memory beyond allocated size for this heap chunk would 
 * potentially cause unexpected failures.
 * Specifically the heap implementation stores relevant software book-keeping 
 * parameters in the unallocated/free heap regions. Overflow of a previous 
 * allocated buffer could corrupt this and could cause issues with successive 
 * heap allocations. Similarly this could also corrupt the contents of 
 * successive allocated buffers as well.
 * 
 * \return pointer to allcoated memory
 * \return NULL - memory could not be allocated since a free block of required size could not be found
 */
extern void *HeapP_alloc(HeapP_Handle handle, size_t allocSize);

/*!
 * @brief Free memory from user defined heap
 *
 * @param handle    [in] Heap handle
 * @param ptr       [in] Pointer to memory allocated using \ref HeapP_alloc
 * @param size      [in] Size of the block of memory to free
 *
 */
extern HeapP_Status HeapP_free(HeapP_Handle handle, void *ptr, uint32_t size);

/*!
 * @brief Get detailed heap statistics
 *
 * @param handle    [in] Heap handle
 * @param stats     [out] Returned heap statistics
 */
extern HeapP_Status HeapP_getHeapStats(HeapP_Handle handle, HeapP_MemStats *stats);

/*! @} */

#ifdef __cplusplus
}
#endif

#endif /* HEAPP_H */
