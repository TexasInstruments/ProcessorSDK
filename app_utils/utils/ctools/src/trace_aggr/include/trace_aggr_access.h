/**
* @file trace_aggr_access.h
* @brief CPTracer2 aggregator register access.
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

#ifndef __TRACE_AGGR_ACCESS_H__
#define __TRACE_AGGR_ACCESS_H__

#include "td_error.h"
#include "target_access.h"

#define TRACE_AGGR_ID_FUNCTION_BITS         (0xFFF << 16)
#define TRACE_AGGR_ID_FUNCTION              (0x281 << 16)
#define TRACE_AGGR_CTRL_TRC_ENABLE          (0x001 << 0)
#define TRACE_AGGR_OWN_CLAIM_BIT            (0x001 << 0)
#define TRACE_AGGR_OWN_BITS                 (0x003 << 1)
#define TRACE_AGGR_OWN_FREE                 (0x000 << 0)
#define TRACE_AGGR_OWN_DBG                  (0x001 << 1)
#define TRACE_AGGR_OWN_APP                  (0x001 << 2)
#define TRACE_AGGR_STP_TRACE_CTRL_TSEN      (0x001 << 1)
#define TRACE_AGGR_STP_TRACE_ID_MAX         (0x07F << 0)
#define TRACE_AGGR_STP_SYNC_COUNT_BITS      (0xFFF << 0)
#define TRACE_AGGR_STP_SYNC_COUNT_MODE      (0x001 << 12)
#define TRACE_AGGR_STP_SYNC_COUNT_RESET     (0x780)
#define TRACE_AGGR_STP_FLUSH_CONTROL        (0x001 << 0)


#ifdef __cplusplus
extern "C" {
#endif

    td_error_t TRACE_AGGR_READ_AGGREGATOR_ID(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_AGGREGATOR_CNTL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_AGGREGATOR_OWN(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_PORT0(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_ADDR0(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_DATA0(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_PORT1(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_ADDR1(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_DATA1(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_PORT2(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_ADDR2(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_DATA2(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_PORT3(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_ADDR3(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_CONT_READ_DATA3(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_STP_TRACE_CONTROL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_STP_TRACE_ID(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_STP_SYNC_CONTROL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_STP_FLUSH_CONTROL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TRACE_AGGR_READ_STP_FEATURES(const target_access_handle_t ta_handle, uint32_t *p_val);

    td_error_t TRACE_AGGR_WRITE_AGGREGATOR_CNTL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_AGGREGATOR_OWN(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT0(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR0(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA0(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT1(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR1(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA1(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT2(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR2(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA2(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT3(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR3(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA3(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_STP_TRACE_CONTROL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_STP_TRACE_ID(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_STP_SYNC_CONTROL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TRACE_AGGR_WRITE_STP_FLUSH_CONTROL(const target_access_handle_t ta_handle, const uint32_t val);

#ifdef __cplusplus
}
#endif

#endif // __TRACE_AGGR_ACCESS_H

