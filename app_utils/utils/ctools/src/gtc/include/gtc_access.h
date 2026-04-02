/**
* @file gtc_access.h
* @brief GTC register access.
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

#ifndef __GTC_ACCESS_H__
#define __GTC_ACCESS_H__

#include "td_error.h"
#include "target_access.h"

#define GTC_SYSTEM_COUNTER_ENABLE (1 << 0)
#define GTC_HALT_ON_DEBUG         (1 << 1)

#ifdef __cplusplus
extern "C" {
#endif

    td_error_t GTC_READ_CNTCR(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t GTC_READ_CNTSR(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t GTC_READ_CNTCV_LO(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t GTC_READ_CNTCV_HI(const target_access_handle_t ta_handle, uint32_t *p_val);

    td_error_t GTC_WRITE_CNTCR(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t GTC_WRITE_CNTCV_LO(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t GTC_WRITE_CNTCV_HI(const target_access_handle_t ta_handle, const uint32_t val);

#ifdef __cplusplus
}
#endif

#endif // __GTC_ACCESS_H
