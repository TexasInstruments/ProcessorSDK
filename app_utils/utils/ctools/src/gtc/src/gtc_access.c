/**
* @file gtc_access.c
* @brief GTC target access definitions.
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
#include <stdio.h>
#include "td_error.h"
#include "target_access.h"
#include "target_access_direct.h"
#include "gtc_access.h"

typedef struct reg_info_t {
    uint32_t offset;
    const char *name;
} reg_info_t;

static reg_info_t CNTCR = { 0x000, "CNTCR" };
static reg_info_t CNTSR = { 0x004, "CNTSR" };
static reg_info_t CNTCV_LO = { 0x008, "CNTCV_LO" };
static reg_info_t CNTCV_HI = { 0x00C, "CNTCV_HI" };

td_error_t GTC_READ_CNTCR(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, CNTCR.name, CNTCR.offset, p_val, MEM);
    return err;
}

td_error_t GTC_READ_CNTSR(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, CNTSR.name, CNTSR.offset, p_val, MEM);
    return err;
}

td_error_t GTC_READ_CNTCV_LO(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, CNTCV_LO.name, CNTCV_LO.offset, p_val, MEM);
    return err;
}

td_error_t GTC_READ_CNTCV_HI(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, CNTCV_HI.name, CNTCV_HI.offset, p_val, MEM);
    return err;
}

td_error_t GTC_WRITE_CNTCR(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CNTCR.name, CNTCR.offset, &regval, MEM);
    return err;
}

td_error_t GTC_WRITE_CNTCV_LO(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CNTCV_LO.name, CNTCV_LO.offset, &regval, MEM);
    return err;
}

td_error_t GTC_WRITE_CNTCV_HI(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CNTCV_HI.name, CNTCV_HI.offset, &regval, MEM);
    return err;
}
