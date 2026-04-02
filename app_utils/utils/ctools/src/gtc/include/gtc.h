/**
* @file gtc.h
* @brief GTC API definitions.
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

#ifndef __GTC_H
#define __GTC_H

#include <stdint.h>
#include <stdbool.h>
#include "td_error.h"
#include "target_access.h"

#ifdef __cplusplus
extern "C" {
#endif

    static const uint32_t  GTC_LIBRARY_MAJOR_VERSION = 0;
    static const uint32_t  GTC_LIBRARY_MINOR_VERSION = 1;

    /**
    * @brief GTC Internal Data.
    *
    */
    typedef struct _gtc_data_t {
        target_access_t ta; /**< Target access data structure definition */
        uint32_t b_halt_on_debug; /**< halt system counter when debug halt is asserted */
    } gtc_data_t;


    /**
    * @brief GTC instance handle.
    *
    * Forward reference serves as opaque pointer type private to the GTC module.
    * Returned by GTC_open() and passed into subsequent calls into the module
    */
    typedef struct _gtc_data_t* gtc_handle_t;


    /**
    * @brief GTC initialization structure.
    *
    * Structure of device specific information to program GTC unit.
    *
    */
    typedef struct gtc_init_t {
        target_access_handle_t ta; /**<Target access handle */
    } gtc_init_t;


    /**
    * @brief Open GTC programming interface.
    *
    * Populate handle with configuration structure.
    * Return valid GTC handle.
    *
    * @param[in] p_init initialization object (pass null for self-hosted
    *               implementation)
    * @param[out] p_handle pointer to GTC handle
    */
    td_error_t GTC_open(const gtc_init_t *p_init,
        gtc_handle_t* p_handle);


    /**
    * @brief Configure GTC programming interface.
    *
    * @param[in] handle GTC handle
    * @param[in] b_halt_on_debug halt counter on debug stop
    */
    td_error_t GTC_configure(gtc_handle_t handle,
        bool b_halt_on_debug);


    /**
    * @brief Enable GTC export.
    *
    * Enable system counter
    * @param[in] handle GTC handle
    */
    td_error_t GTC_enable(gtc_handle_t handle);

    
    /**
    * @brief Close GTC programming interface.
    *
    * @param[in] handle GTC handle
    */
    td_error_t GTC_close(gtc_handle_t handle);
    

#ifdef __cplusplus
}
#endif

#endif //__GTC_H

