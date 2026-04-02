/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
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

/**
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_CacheP CacheP
 *            CacheP interface
 *
 *  @{
 */ 
 
/** ============================================================================
 *  @file       CacheP.h
 *
 *  @brief      Cache Handling routines for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_CacheP__include
#define ti_osal_CacheP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 *  \anchor Osal_CacheP_isCoherent
 *  \name Cache coherent type definitions
 *  @{
 */
/*!
 *  @brief  This enumerator defines the cache coherent types
 */
typedef uint32_t Osal_CacheP_isCoherent;
    /** Cache is coherent on this CPU */
#define OSAL_CACHEP_COHERENT                            ((uint32_t) 0U)
    /**Cache is not coherent on this CPU */
#define OSAL_CACHEP_NOT_COHERENT                        ((uint32_t) 1U)
/* @} */

/**
 * \anchor CacheP_Mar
 * \name Set Cache MAR register
 *
 * @{
 */
/*!
 *  @brief  This enumerator defines the MAR register setting types
 */
typedef uint32_t CacheP_Mar;
    /** Cache MAR register disabled */
#define CacheP_Mar_DISABLE                             ((uint32_t) 0U)
    /** Cache MAR register is enabled */
#define CacheP_Mar_ENABLE                              ((uint32_t) 1U)
/* @} */

/*!
 *  @brief  Function to write back cache lines
 *
 *  @param  addr  Start address of the cache line/s
 *
 *  @param  size  size (in bytes) of the memory to be written back
 *
 */
extern void CacheP_wb(const void * addr, uint32_t size);


/*!
 *  @brief  Function to invalidate cache lines
 *
 *  @param  addr  Start address of the cache line/s
 *
 *  @param  size  size (in bytes) of the memory to invalidate
 *
 */
extern void CacheP_Inv(const void * addr, uint32_t size);


/*!
 *  @brief  Function to write back and invalidate cache lines
 *
 *  @param  addr  Start address of the cache line/s
 *
 *  @param  size  size (in bytes) of the memory to be written back and invalidate
 *
 */
extern void CacheP_wbInv(const void * addr, uint32_t size);

/*!
 *  @brief Set MAR attribute for a memory range
 *  
 *  [C66x Only] The API will set the MAR attribute for the range,
 *  start_addr = floor(baseAddr, 16MB) .. end_addr = ceil(baseAddr+size, 16MB)
 *
 *  @param baseAddr  Region start address. Recommended to be 16MB aligned
 * 
 *  @param size  Region size in bytes. Recommended to be multiple of 16MB aligned
 * 
 *  @param value   value for setting MAR register \ref CacheP_Mar
 */
void CacheP_setMar(void *baseAddr, uint32_t size, uint32_t value);

/*!
 *  @brief Get MAR attribute for a region of 16MB
 *
 * [C66x Only] The API will return the MAR attribute of the region with start addr as
 *    start_addr = floor(baseAddr, 16MB)
 *
 *  @param baseAddr  region start address, recommended to be 16MB aligned
 *
 *  @return MAR attribute for this 16MB region
 */
uint32_t CacheP_getMar(uintptr_t baseAddr);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_CacheP__include */
/* @} */
