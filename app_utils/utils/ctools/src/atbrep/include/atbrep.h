/**
* @file atbrep.h
* @brief ATBREP API definitions.
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

#ifndef __ATBREP_H
#define __ATBREP_H

#include <stdint.h>
#include "td_error.h"
#include "target_access.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ATBREP_MAX_MASTERS 2

/**
* @brief ATB Replicator Internal Data
*
*/
typedef struct _atbrep_data_t
{
    uint32_t devtype; /**< Device Type */
    uint32_t master_count; /**< Number of Masters */
    target_access_t ta; /**< Target access data structure definition */
} atbrep_data_t;

/**
* @brief ATBREP instance handle.
*
* Client handle is a pointer to a structure internal to the module
* that contains the private instance data.  A client handle is returned by
* the ATBREP_open() call and is used by the client in subsequent calls
* into the module.
*
*/
typedef struct _atbrep_data_t *atbrep_handle_t;

/**
* @brief atbrep_init_t
*
* Initialization values for ATBREP_open()
*/
typedef struct atbrep_init_t
{
    target_access_handle_t ta; /**< target access handle */
} atbrep_init_t;

/**
* @brief atbrep_property_t
*
* Properties of replicator returned from ATBREP_property().
*/
typedef struct atbrep_property_t
{
    uint32_t master_count;  /**< Number of masters */
} atbrep_property_t;

/**
* @brief ATBREP filter
*
* Defines the constants used for filtering ATB IDs.
*/
typedef uint8_t atbrep_filters_t;
typedef enum atbrep_filter_t
{
    e_ATBREP_FILTER_00_0F = (1<<0), /**< Filter ATB IDS 00-0F */
    e_ATBREP_FILTER_10_1F = (1<<1), /**< Filter ATB IDS 10-1F */
    e_ATBREP_FILTER_20_2F = (1<<2), /**< Filter ATB IDS 20-2F */
    e_ATBREP_FILTER_30_3F = (1<<3), /**< Filter ATB IDS 30-3F */
    e_ATBREP_FILTER_40_4F = (1<<4), /**< Filter ATB IDS 40-4F */
    e_ATBREP_FILTER_50_5F = (1<<5), /**< Filter ATB IDS 50-5F */
    e_ATBREP_FILTER_60_6F = (1<<6), /**< Filter ATB IDS 60-6F */
    e_ATBREP_FILTER_70_7F = (1<<7)  /**< Filter ATB IDS 70-7F */
} atbrep_filter_t;


/**
* @brief Open an instance of the ATBREP
*
* Verify the device id. 
* Check that the hardware can be claimed and claim it.  
* Create an instance and return the instance pointer. 
*
* @param[in]  p_init    Initialization values
* @param[out] p_handle  On success, the handle to be used in subsequent ATBREP
*                       calls. The handle is valid until ATBREP_close() is called.
* @return returns td_error_t
*/
td_error_t  
ATBREP_open(
    const atbrep_init_t *p_init,
    atbrep_handle_t     *p_handle 
    );

/**
* @brief Fetch the properties of the ATBREP
*
* @param[in]  handle     Handle returned by successful ATBREP_open() call.
* @param[out] p_property Properties of the replicator
* @return returns td_error_t
*/
td_error_t
ATBREP_get_property(
    const atbrep_handle_t handle,
    atbrep_property_t *p_property
    );

/**
* @brief Set the ATBID filter of a master
*
* @param [in] handle    The handle returned from a successful ATBREP_open() call
* @param [in] master_id The master ID
* @param [in] filter    The filter setting
* @return returns td_error_t
*/
td_error_t
ATBREP_set_filter(
    const atbrep_handle_t  handle,
    const unsigned         master_id,
    const atbrep_filters_t filter
    );

td_error_t
ATBREP_get_filter(
const atbrep_handle_t  handle,
const unsigned         master_id,
atbrep_filters_t *p_filter
);


/**
* @brief Close/Release the ATBREP
* 
* Unclaim and unlock the ATBREP. 
* Free the memory pointed to by the handle. 
* On return the handle is no longer valid and cannot be used by the client.
*
* @param [in] handle    The handle returned from a successful ATBREP_open() call
*
* @return returns void
*/
void  
ATBREP_close(
        atbrep_handle_t handle 
        );


#ifdef __cplusplus
}
#endif

#endif //__ATBREP_H
