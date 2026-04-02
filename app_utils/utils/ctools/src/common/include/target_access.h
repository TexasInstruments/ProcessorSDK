/**
* @file target_access.h
* @brief Target memory and register access API definitions.
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
#ifndef __TARGET_ACCESS_H
#define __TARGET_ACCESS_H

#include <stdint.h>
#include "target_access_direct.h"

#if defined _UNUSED_PARAM
#undef _UNUSED_PARAM
#endif
#define _UNUSED_PARAM(x) (void)x

/**
* @brief Target access data structure definition
*
* The target access data structure encapsulates the target access helper (needed for
* remote environment) as well as the configuration base address of the IP driver for
* memory accesses. This allows the IP driver to only pass in the offset address to the
* memory API and be base address agnostic. It will be the responsibility of the higher
* layer to create the handle and pass it to the IP driver.
*
* @param helper Pointer to helper function if needed
* @param base_address Target base address
*
*/

#if defined(_64_BIT_TARGET)
    typedef uint64_t ADDR_TYPE;
#else
    typedef uint32_t ADDR_TYPE;
#endif


typedef struct {
    void * helper;
    ADDR_TYPE base_address;
}target_access_t;

/**
* @brief Target access handle
*
* Each IP driver will be given an instance of the handle in the open call.
* The target handle must exist for the lifetime of the library as the library
* references the handle passed in and does not create a copy.
*/
typedef target_access_t* target_access_handle_t;

/**
* @brief Target register and memory access API defintions.
*
* A common API is defined for memory and register accesses whether the IP driver is
* running on the remote host or in a self-hosted/local environment. Only include this
* file in the IP driver and the relevant dependent header file will be pulled in 
* depending on the environment.
*
* Requirements
* 1) Direct access for self hosted and local environment
* 2) Callback handle for remote environment
* 3) Direct register access using mnemonics for self hosted environment
*
* @params
* 1. access_handle: This is the target access handle passed to the IP driver as a
*                   target_access_handle_t type. The handle encapsulates the helper
*                   and the base address. The helper is unused for self and local
*                   environment and relevant only in the remote environment. It is
*                   expected that a higher level will create the target access handle
*                   and pass it to the IP driver during the open call.
* 2. element_name : This is the string name of the element and is used for logging the
                    call. If NULL is supplied then the call will not be logged.
* 3. element_id   : This is the config "offset" address for memory accesses and register
*                   name for register access.
* 4. storage      : Pointer to the variable that either contains the data for a write
*                   operation or the data which will hold the read value.
* 5. type         : For memory access the keyword is MEM for register it is REG.
*
* @return The macro APIs return an td_error_t type value.
*
* Examples
*
* Assume: IP driver was passed in "target_access" of type target_access_handle_t
* during the open call.
*
* 1. Read a 32 bit memory mapped register
*       uint32_t reg_val;
*       uint32_t offset_address = ADTF_LOCK_STATUS;
*       READ_32(target_access, "ADTF_LOCK_STATUS", offset_address, &reg_val, MEM);
* 2. Write to a 16-bit register
*       uint32_t reg_val = 0xABCD;
*       WRITE_16(target_access, "TCU_CTRL", TCU_CTRL, &reg_val, REG);
*    Here TCU_CTRL is either a directly mapped mnemonic in the hosted environment which
*    is recognized by the compiler or maps to a string name in the remote environment.
*/

typedef enum {
    MEM,
    REG
}ta_type_t;

/**
* @brief Read APIs
*/


#define READ_64(access_handle, element_name, element_id, storage, type)            \
                        READ_##type##_64(access_handle, element_name, element_id, storage)
#define READ_32(access_handle, element_name, element_id, storage, type)            \
                        READ_##type##_32(access_handle, element_name, element_id, storage)
#define READ_16(access_handle, element_name, element_id, storage, type)            \
                        READ_##type##_16(access_handle, element_name, element_id, storage)
#define READ_8(access_handle, element_name, element_id, storage, type)             \
                        READ_##type##_8(access_handle, element_name, element_id, storage)

/**
* @brief Write APIs
*/
#define WRITE_64(access_handle, element_name, element_id, storage, type)          \
                        WRITE_##type##_64(access_handle, element_name, element_id, storage)
#define WRITE_32(access_handle, element_name, element_id, storage, type)          \
                        WRITE_##type##_32(access_handle, element_name, element_id, storage)
#define WRITE_16(access_handle, element_name, element_id, storage, type)          \
                        WRITE_##type##_16(access_handle, element_name, element_id, storage)
#define WRITE_8(access_handle, element_name, element_id, storage, type)           \
                        WRITE_##type##_8(access_handle, element_name, element_id, storage)




#endif //__TARGET_ACCESS_H
