/**
* trace_aggr.c
*
* CPTracer2 aggregator driver implementation
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
#include "trace_aggr.h"
#include "trace_aggr_access.h"

static td_error_t verify_module_id(const trace_aggr_handle_t handle);
static td_error_t claim_module(const trace_aggr_handle_t handle);

typedef struct {
    uint32_t offset;
    const char* name;
} trace_aggr_mmr_t;

td_error_t TRACE_AGGR_open(
    const trace_aggr_init_t *p_init,
    trace_aggr_handle_t *p_handle)
{
    td_error_t err = e_ERR_NONE;
    trace_aggr_data_t *p_data = 0;

    /* check that parameters are valid */
    if ((0 == p_init) || (0 == p_handle) || (0 == p_init->ta)) {
        return e_ERR_BAD_PARAM;
    }

    /* Create and initialize the client handle */
    p_data = (trace_aggr_data_t *)malloc(sizeof(trace_aggr_data_t));
    if (0 != p_data) {
        p_data->ta = *p_init->ta;
    }
    else {
        err = e_ERR_ALLOC;
        return err;
    }

    if (e_ERR_NONE == err) {
        err = claim_module(p_data);
    }

    /* Verify that module is actually CoreSight STM. */
    if (e_ERR_NONE == err) {
        err = verify_module_id(p_data);
    }

    if (e_ERR_NONE == err) {
        *p_handle = p_data;
    }
    else {
        free(p_data);
    }

    return err;
}

td_error_t TRACE_AGGR_configure(
    trace_aggr_handle_t handle,
    trace_aggr_options_t* p_options)
{
    td_error_t err = e_ERR_NONE;
    uint32_t val = 0;

    if (p_options->atb_id > TRACE_AGGR_STP_TRACE_ID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    val = p_options->atb_id;
    err = TRACE_AGGR_WRITE_STP_TRACE_ID(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    val = TRACE_AGGR_STP_SYNC_COUNT_MODE | TRACE_AGGR_STP_SYNC_COUNT_MODE;
    err = TRACE_AGGR_WRITE_STP_SYNC_CONTROL(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    // Enable auto-flush
    val = TRACE_AGGR_STP_FLUSH_CONTROL;
    err = TRACE_AGGR_WRITE_STP_FLUSH_CONTROL(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    // Enable timestamping
    val = TRACE_AGGR_STP_TRACE_CTRL_TSEN;
    err = TRACE_AGGR_WRITE_STP_TRACE_CONTROL(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    return e_ERR_NONE;
}

td_error_t TRACE_AGGR_enable(
    trace_aggr_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t val = 0;
    
    val |= TRACE_AGGR_CTRL_TRC_ENABLE;
    err = TRACE_AGGR_WRITE_AGGREGATOR_CNTL(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    err = TRACE_AGGR_READ_AGGREGATOR_CNTL(&handle->ta, &val);
    if (e_ERR_NONE != err) {
        return e_ERR_READ;
    }

    if ((val & TRACE_AGGR_CTRL_TRC_ENABLE) != TRACE_AGGR_CTRL_TRC_ENABLE) {
        err = e_ERR_PROGRAM;
    }

    return err;
}


td_error_t TRACE_AGGR_disable(
    trace_aggr_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t val = 0;

    err = TRACE_AGGR_READ_AGGREGATOR_CNTL(&handle->ta, &val);
    if (e_ERR_NONE != err) {
        return e_ERR_READ;
    }

    val &= ~TRACE_AGGR_CTRL_TRC_ENABLE;
    err = TRACE_AGGR_WRITE_AGGREGATOR_CNTL(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    err = TRACE_AGGR_READ_AGGREGATOR_CNTL(&handle->ta, &val);
    if (e_ERR_NONE != err) {
        return e_ERR_READ;
    }

    if ((val & TRACE_AGGR_CTRL_TRC_ENABLE) == TRACE_AGGR_CTRL_TRC_ENABLE) {
        err = e_ERR_PROGRAM;
    }

    return err;
}


td_error_t TRACE_AGGR_close(
    trace_aggr_handle_t handle) {

    free(handle);

    return e_ERR_NONE;
}

static td_error_t verify_module_id(const trace_aggr_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t val = 0x0;

    err = TRACE_AGGR_READ_AGGREGATOR_ID(&handle->ta, &val);
    if (err != e_ERR_NONE) {
        return e_ERR_READ;
    }

    if ((val & TRACE_AGGR_ID_FUNCTION_BITS) != TRACE_AGGR_ID_FUNCTION) {
        return e_ERR_BAD_HWTYPE;
    }

    return e_ERR_NONE;
}


static td_error_t claim_module(const trace_aggr_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t ownership = 0;

    // Check if the aggregator is owned by debugger or application.
    err = TRACE_AGGR_READ_AGGREGATOR_OWN(&handle->ta, &ownership);
    if (err != e_ERR_NONE) {
        return e_ERR_READ;
    }

    if ((ownership & TRACE_AGGR_OWN_BITS) == TRACE_AGGR_OWN_FREE) {

        // Aggregator is not owned. Claim it.
        err = TRACE_AGGR_WRITE_AGGREGATOR_OWN(&handle->ta, TRACE_AGGR_OWN_CLAIM_BIT);
        if (err != e_ERR_NONE) {
            return e_ERR_WRITE;
        }

        err = TRACE_AGGR_READ_AGGREGATOR_OWN(&handle->ta, &ownership);
        if (err != e_ERR_NONE) {
            return e_ERR_READ;
        }

#ifdef _SELF_HOSTED
        if ((ownership & TRACE_AGGR_OWN_BITS) != TRACE_AGGR_OWN_APP) {
            err = e_ERR_OWNERSHIP;
        }
#else
        if ((ownership & TRACE_AGGR_OWN_BITS) != TRACE_AGGR_OWN_DBG) {
            err = e_ERR_OWNERSHIP;
        }
#endif

    }
    else {
        err = e_ERR_OWNERSHIP;
    }

    if (e_ERR_OWNERSHIP == err) {

#ifdef _SELF_HOSTED
        // If aggegator is not already owned by application, nothing can be done from application side.
        if ((ownership & TRACE_AGGR_OWN_BITS) != TRACE_AGGR_OWN_APP) {
            err = e_ERR_OWNERSHIP;
        }
        else {
            err =  e_ERR_NONE;
        }
#else
        // If aggregator is already owned by debugger, nothing else needs to be done.
        if ((ownership & TRACE_AGGR_OWN_BITS) == TRACE_AGGR_OWN_DBG) {
            err = e_ERR_NONE;
        }
        else {
            // If aggregator is owned by application, override ownership as debugger.

            ownership &= ~TRACE_AGGR_OWN_CLAIM_BIT;
            err = TRACE_AGGR_WRITE_AGGREGATOR_OWN(&handle->ta, ownership);
            if (err != e_ERR_NONE) {
                return e_ERR_WRITE;
            }

            err = TRACE_AGGR_WRITE_AGGREGATOR_OWN(&handle->ta, TRACE_AGGR_OWN_CLAIM_BIT);
            if (err != e_ERR_NONE) {
                return e_ERR_WRITE;
            }

            err = TRACE_AGGR_READ_AGGREGATOR_OWN(&handle->ta, &ownership);
            if (err != e_ERR_NONE) {
                return e_ERR_READ;
            }

            if ((ownership & TRACE_AGGR_OWN_BITS) != TRACE_AGGR_OWN_DBG) {
                err = e_ERR_OWNERSHIP;
            }
        }
#endif
    }

    return err;
}
