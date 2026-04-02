/**
* @file target_access_direct.h
* @brief Target memory and register access definitions for local
* and hosted environment
*
* Copyright (C) 2025 Texas Instruments Incorporated - https://www.ti.com/
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*  Redistributions of source code must retain the above copyright 
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the 
*  documentation and/or other materials provided with the   
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __TARGET_ACCESS_DIRECT_H
#define __TARGET_ACCESS_DIRECT_H
#include <stdint.h>
#include "td_error.h"

/**
* @brief Memory access APIs for 64-bit address.
*/
#ifdef _64_BIT_TARGET

td_error_t static inline local_mem_read_32(const char* name, const uint64_t address, uint32_t *value)
{
    *value = *(volatile uint32_t *)(uintptr_t)(address);
    return e_ERR_NONE;
}

td_error_t static inline local_mem_write_32(const char* name, const uint64_t address, const uint32_t *value)
{
    *(volatile uint32_t *)(uintptr_t)(address) = *value;
    return e_ERR_NONE;
}

td_error_t static inline local_mem_read_16(const char* name, const uint64_t address, uint16_t *value)
{
    *value = *(volatile uint16_t *)(uintptr_t)(address);
    return e_ERR_NONE;
}

td_error_t static inline local_mem_write_16(const char* name, const uint64_t address, const uint16_t *value)
{
    *(volatile uint16_t *)(uintptr_t)(address) = *value;
    return e_ERR_NONE;
}

td_error_t static inline local_mem_read_8(const char* name, const uint64_t address, uint8_t *value)
{
    *value = *(volatile uint8_t *)(uintptr_t)(address);
    return e_ERR_NONE;
}

td_error_t static inline local_mem_write_8(const char* name, const uint64_t address, const uint8_t *value)
{
    *(volatile uint8_t *)(uintptr_t)(address) = *value;
    return e_ERR_NONE;
}

#else

/**
* @brief Memory access APIs for 32-bit address.
*/

td_error_t static inline local_mem_read_32(const char* name, const uint32_t address, uint32_t *value)
{
    *value = *(volatile uint32_t *)(uintptr_t)(address);
    return e_ERR_NONE;
}

td_error_t static inline local_mem_write_32(const char* name, const uint32_t address, const uint32_t *value)
{
    *(volatile uint32_t *)(uintptr_t)(address) = *value;
    return e_ERR_NONE;
}

td_error_t static inline local_mem_read_16(const char* name, const uint32_t address, uint16_t *value)
{
    *value = *(volatile uint16_t *)(uintptr_t)(address);
    return e_ERR_NONE;
}

td_error_t static inline local_mem_write_16(const char* name, const uint32_t address, const uint16_t *value)
{
    *(volatile uint16_t *)(uintptr_t)(address) = *value;
    return e_ERR_NONE;
}

td_error_t static inline local_mem_read_8(const char* name, const uint32_t address, uint8_t *value)
{
    *value = *(volatile uint8_t *)(uintptr_t)(address);
    return e_ERR_NONE;
}

td_error_t static inline local_mem_write_8(const char* name, const uint32_t address, const uint8_t *value)
{
    *(volatile uint8_t *)(uintptr_t)(address) = *value;
    return e_ERR_NONE;
}

#endif

/**
* @brief Register read/write APIs
*
* Target handle is unused in the local and self-hosted environment.
*/


#define READ_REG_32(ta_handle, name, reg, value)                (*value = reg) ? e_ERR_NONE:e_ERR_NONE
#define READ_REG_16(ta_handle, name, reg, value)                (*value = reg) ? e_ERR_NONE:e_ERR_NONE
#define READ_REG_8(ta_handle, name, reg, value)                 (*value = reg) ? e_ERR_NONE:e_ERR_NONE

#define WRITE_REG_32(ta_handle, name, reg, value)               (reg = *value) ? e_ERR_NONE:e_ERR_NONE
#define WRITE_REG_16(ta_handle, name, reg, value)               (reg = *value) ? e_ERR_NONE:e_ERR_NONE
#define WRITE_REG_8(ta_handle, name, reg, value)                (reg = *value) ? e_ERR_NONE:e_ERR_NONE

/**
* @brief Memory read/write APIs
* Construct the full address using the base address passed in the target access handle.
*/
#define READ_MEM_32(ta_handle, name, offset, value)            local_mem_read_32(name, (ta_handle)->base_address + offset, value)
#define READ_MEM_16(ta_handle, name, offset, value)            local_mem_read_16(name, (ta_handle)->base_address + offset, value)
#define READ_MEM_8(ta_handle, name, offset, value)             local_mem_read_8(name, (ta_handle)->base_address + offset, value)

#define WRITE_MEM_32(ta_handle, name, offset, value)           local_mem_write_32(name, (ta_handle)->base_address + offset, value)
#define WRITE_MEM_16(ta_handle, name, offset, value)           local_mem_write_16(name, (ta_handle)->base_address + offset, value)
#define WRITE_MEM_8(ta_handle, name, offset, value)            local_mem_write_8(name, (ta_handle)->base_address + offset, value)


#endif        //__TARGET_ACCESS_DIRECT_H
