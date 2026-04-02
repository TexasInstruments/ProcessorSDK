/**
* gtc.c
*
* GTC IP Driver Implementation
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

#include <stdint.h>
#include <stdio.h>
#include "gtc.h"
#include "gtc_access.h"

/* The GTC IP driver provides the interface to configure the global timebase counter. */

typedef struct {
    uint32_t offset;
    const char* name;
} gtc_mmr_t;

td_error_t GTC_open(
    const gtc_init_t *p_init,
    gtc_handle_t *p_handle)
{
    td_error_t err = e_ERR_NONE;
    gtc_data_t *p_data = 0;

    /* check that parameters are valid */
    if ((0 == p_init) || (0 == p_handle) || (0 == p_init->ta)) {
        return e_ERR_BAD_PARAM;
    }

    /* Create and initialize the client handle */
    p_data = (gtc_data_t *)malloc(sizeof(gtc_data_t));
    if (0 != p_data) {
        p_data->ta = *p_init->ta;
    }
    else {
        err =  e_ERR_ALLOC;
        return err;
    }

    if (e_ERR_NONE == err) {
        *p_handle = p_data;
    }
    else {
        free(p_data);
    }

    return err;
}


td_error_t GTC_configure(gtc_handle_t handle,
    bool b_halt_on_debug)
{
    handle->b_halt_on_debug = b_halt_on_debug;

    return e_ERR_NONE;
}


td_error_t GTC_enable(gtc_handle_t handle)
{

    uint32_t val = 0;
    td_error_t err = e_ERR_NONE;

    err = GTC_READ_CNTCR(&handle->ta, &val);
    if (err != e_ERR_NONE) {
        return e_ERR_READ;
    }

    // Enable halt on debug if requested
    if (handle->b_halt_on_debug) {
        val |= GTC_HALT_ON_DEBUG;
        err = GTC_WRITE_CNTCR(&handle->ta, val);
        if (err != e_ERR_NONE) {
            return err;
        }
    }

    // Enable if not already enabled
    if (!(val & GTC_SYSTEM_COUNTER_ENABLE)) {
        val |= GTC_SYSTEM_COUNTER_ENABLE;
        err = GTC_WRITE_CNTCR(&handle->ta, val);
        if (err != e_ERR_NONE) {
            return err;
        }
    }

    return e_ERR_NONE;
}


td_error_t GTC_disable(gtc_handle_t handle)
{
    uint32_t val = 0;
    td_error_t err = e_ERR_NONE;

    err = GTC_READ_CNTCR(&handle->ta, &val);
    if (err != e_ERR_NONE) {
        return e_ERR_READ;
    }

    // Disable if not already disabled
    if (val & GTC_SYSTEM_COUNTER_ENABLE) {
        val &= ~GTC_SYSTEM_COUNTER_ENABLE;
        err = GTC_WRITE_CNTCR(&handle->ta, val);
        if (err != e_ERR_NONE) {
            return err;
        }
    }

    return e_ERR_NONE;
}


td_error_t GTC_close(gtc_handle_t handle) {

    free(handle);

    return e_ERR_NONE;
}

