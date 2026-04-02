/**
* cpt2.c
*
* CPT2 Probe Driver Implementation
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
#include <string.h>
#include "cpt2.h"
#include "cpt2_access.h"
#include <utils/console_io/include/app_log.h>

#define MAX_CHANNELS   256
#define nullptr 0


static td_error_t verify_module_id(const cpt2_handle_t handle);
static td_error_t claim_module(const cpt2_handle_t handle);
static td_error_t check_filter_and_masks(const cpt2_options_t * p_options);
static uint32_t calculate_filt0(const cpt2_options_t * p_options);
static uint32_t calculate_filt1(const cpt2_options_t * p_options);
static uint32_t calculate_filt2(const cpt2_options_t * p_options);
static uint32_t calculate_filt3(const cpt2_options_t * p_options);
static uint32_t calculate_mask0(const cpt2_options_t * p_options);
static uint32_t calculate_mask1(const cpt2_options_t * p_options);
static uint32_t calculate_mask2(const cpt2_options_t * p_options);
static uint32_t calculate_mask3(const cpt2_options_t * p_options);

// Mostly derived from reset values.
const cpt2_options_t g_defaultCpt2Options = {
    0x00FFFFFF, //sampling_period = 0x00FFFFFF;
    eCPT2_MODE_UNDEFINED,//mode = eCPT2_MODE_UNDEFINED; // Reset value is actually eCPT2_MODE_LATENCY
    false,//filt_addr_low_range_exclude = false;
    0x0,//filt_addr_low_lsb = 0x0;
    0x0,//filt_addr_low_msb = 0x0;
    0xFFFFFFFF,//filt_addr_high_lsb = 0xFFFFFFFF;
    0xFFFF,//filt_addr_high_msb = 0xFFFF;
    0x0,//filt_channel_id = 0x0;
    0x0,//filt_channel_id_mask = 0x0;
    false,//filt_pable = false;
    false,//filt_pable_mask = false;
    0x0,//filt_cmsideband = 0x0;
    0x0,//filt_cmsideband_mask = 0x0;
    0x0,//filt_csideband = 0x0;
    0x0,//filt_csideband_mask = 0x0;
    0x0,//filt_order_id = 0x0;
    0x0,//filt_order_id_mask = 0x0;
    false,//filt_excl = false;
    false,//filt_excl_mask = false;
    false,//filt_dir = false;  // Write transactions are getting recorded if false. If it's true, it will recored read transactions for VBUSM and VBUSP probes
    false,//filt_dir_mask = false; // Direction mask must always be set to 1 for VBUSM and VBUSP probes in Throughput and Latency statistics.
    0x0,//filt_amode = 0x0;
    0x0,//filt_amode_mask = 0x0;
    0x0,//filt_route_id = 0x0;
    0x0,//filt_route_id_mask = 0x0;
    0x0,//filt_rsel = 0x0;
    0x0,//filt_rsel_mask = 0x0;
    0x0,//filt_dtype = 0x0;
    0x0,//filt_dtype_mask = 0x0;
    0x0,//filt_priv = 0x0;
    0x0,//filt_priv_mask = 0x0;
    0x0,//filt_clsize = 0x0;
    0x0,//filt_clsize_mask = 0x0;
    0x0,//filt_epriority = 0x0;
    0x0,//filt_epriority_mask = 0x0;
    0x0,//filt_priority = 0x0;
    0x0,//filt_priority_mask = 0x0;
    0x0,//filt_bytecount = 0x0;
    0x0,//filt_bytecount_mask = 0x0;
    0x0,//filt_opcode = 0x0;
    0x0,//filt_opcode_mask = 0x0;
    0x0,//filt_memtype = 0x0;
    0x0,//filt_memtype_mask = 0x0;
    false,//filt_secure = false;
    false,//filt_secure_mask = false;
    0x0,//filt_qos = 0x0;
    0x0,//filt_qos_mask = 0x0;
    0x0,//filt_sdomain = 0x0;
    0x0,//filt_sdomain_mask = 0x0;
    0x0,//filt_outer = 0x0;
    0x0,//filt_outer_mask = 0x0;
    0x0,//filt_inner = 0x0;
    0x0,//filt_inner_mask = 0x0;
    false,//filt_emudbg = false;
    false,//filt_emudbg_mask = false;
    false,//filt_interest = false;
    false,//filt_interest_mask = false;
    false,//filt_flush = false;
    false,//filt_flush_mask = false;
    0x0,//filt_fwpass = 0x0;
    0x0,//filt_fwpass_mask = 0x0;
    0x0,//filt_asel = 0x0;
    0x0,//filt_asel_mask = 0x0;
    0x0,//filt_atype = 0x0;
    0x0,//filt_atype_mask = 0x0;
    0x0,//filt_privid = 0x0;
    0x0,//filt_privid_mask = 0x0;
    0x0,//filt_virtid = 0x0;
    0x0,//filt_virtid_mask = 0x0;
    0x0, //mast_id_3_msbs = 0x0;
    false //fast_enable_disable = false;
};

td_error_t CPT2_open(
    const cpt2_init_t *p_init,
    cpt2_handle_t *p_handle)
{
    td_error_t err = e_ERR_NONE;
    cpt2_data_t *p_data = 0;

    /* check that parameters are valid */
    if ((0 == p_init) || (0 == p_handle) || (0 == p_init->ta)) {
        return e_ERR_BAD_PARAM;
    }

    /* Create and initialize the client handle */
    p_data = (cpt2_data_t *)malloc(sizeof(cpt2_data_t));
    if (0 != p_data) {
        p_data->ta = *p_init->ta;
    }
    else {
        err = e_ERR_ALLOC;
        return err;
    }

    /* Verify that module is actually a CPT2 probe. */
    if (e_ERR_NONE == err) {
        err = verify_module_id(p_data);
    }

    /* Claim ownership of the CPT2 probe. */
    if (e_ERR_NONE == err) {
        err = claim_module(p_data);
    }

    if (e_ERR_NONE == err) {
        *p_handle = p_data;
    }
    else {
        free(p_data);
    }

    return err;
}

td_error_t CPT2_configure(cpt2_handle_t handle,
    const cpt2_options_t * p_options)
{
    td_error_t err = e_ERR_NONE;
    uint32_t val = 0;
    uint32_t sampling_period = 0;
    uint32_t probe_stat = 0;
    uint32_t mast_id = 0;
    uint32_t filt_qual_0 = 0;
    uint32_t filt_qual_1 = 0;
    uint32_t filt_qual_2 = 0;
    uint32_t filt_qual_3 = 0;
    uint32_t filt_mask_0 = 0;
    uint32_t filt_mask_1 = 0;
    uint32_t filt_mask_2 = 0;
    uint32_t filt_mask_3 = 0;

    if (nullptr == p_options) {
        return e_ERR_BAD_PARAM;
    }

    sampling_period = p_options->sampling_period;

    if ((sampling_period > CPT2_PROBE_SAMP_PERIOD_MAX) ||
        (sampling_period < CPT2_PROBE_SAMP_PERIOD_MIN)) {

        // Invalid sampling window
        return e_ERR_BAD_PARAM;
    }

    err = CPT2_READ_PROBE_STATUS(&handle->ta, &probe_stat);
    if (e_ERR_NONE != err) {
        return e_ERR_READ;
    }

    // Probe should be disabled before configuration.
    if ((probe_stat & CPT2_PROBE_EN_STAT) == CPT2_PROBE_EN_STAT) {

        err = CPT2_READ_PROBE_CNTL(&handle->ta, &handle->probe_ctrl);
        if (e_ERR_NONE != err) {
            return e_ERR_WRITE;
        }

        handle->probe_ctrl &= ~CPT2_PROBE_CTRL_EN;
        err = CPT2_WRITE_PROBE_CNTL(&handle->ta, handle->probe_ctrl);
        if (e_ERR_NONE != err) {
            return e_ERR_WRITE;
        }

        err = CPT2_READ_PROBE_STATUS(&handle->ta, &probe_stat);
        if (e_ERR_NONE != err) {
            return e_ERR_READ;
        }

        // If probe is not disabled at this point, error out.
        if ((probe_stat & CPT2_PROBE_EN_STAT) == CPT2_PROBE_EN_STAT) {
            return e_ERR_PROGRAM;
        }
    }

    /* The sample window counter is 24-bits wide.
    The CPT2_PROBE_SAMP_PERIOD_BITS are the 14 MSBs of that value.
    The 10 LSBs are always set to 1.

    E.g. sampling window = 0x00FFFFFF -> CPT2_PROBE_SAMP_PERIOD_BITS = 0x3FFF
    E.g. sampling window = 0x00001000 -> CPT2_PROBE_SAMP_PERIOD_BITS = 0x0004
    */
    val = (sampling_period >> 10);

    err = CPT2_WRITE_PROBE_SAMP_PERIOD(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    handle->probe_ctrl = 0;
    switch (p_options->mode)
    {
    case eCPT2_MODE_LATENCY:
        handle->probe_ctrl |= CPT2_PROBE_CTRL_OP_LAT;
        break;
    case eCPT2_MODE_THROUGHPUT:
        handle->probe_ctrl |= CPT2_PROBE_CTRL_OP_THROUGH;
        break;
    case eCPT2_MODE_TRANSACTION:
        handle->probe_ctrl |= CPT2_PROBE_CTRL_OP_TRANS;
        break;
    default:
        return e_ERR_BAD_PARAM;
    }

    // Set 3 MSBs of Master ID
    // Say: cpt2_mst_id = 129
    // p_options->mast_id_3_msbs = cpt2_mst_id >> 5 = 4;
    // p_options->mast_id_3_msbs << CPT2_PROBE_CTRL_MST_ID_OFFSET = 4 << 12 = 0x4000
    // Also, CPT2_PROBE_CTRL_MST_ID_BITS = (0x007 << 12) = 0x7000
    // So, mast_id = 0x7000 & 0x4000 = 0x4000
    mast_id = (CPT2_PROBE_CTRL_MST_ID_BITS & (p_options->mast_id_3_msbs << CPT2_PROBE_CTRL_MST_ID_OFFSET));
    handle->probe_ctrl |= mast_id;

    err = CPT2_WRITE_PROBE_CNTL(&handle->ta, handle->probe_ctrl);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    // Program address filters
    val = p_options->filt_addr_low_lsb;
    err = CPT2_WRITE_FILT_ADDR_LOW_LSB(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    val = p_options->filt_addr_low_msb;
    if (p_options->filt_addr_low_range_exclude) {
        val |= (uint32_t)CPT2_PROBE_FILT_ADDR_LOW_RANGE_EXCLUDE;
    }
    err = CPT2_WRITE_FILT_ADDR_LOW_MSB(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    val = p_options->filt_addr_high_lsb;
    err = CPT2_WRITE_FILT_ADDR_HIGH_LSB(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    val = p_options->filt_addr_high_msb;
    err = CPT2_WRITE_FILT_ADDR_HIGH_MSB(&handle->ta, val);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    // Set up filters and masks
    err = check_filter_and_masks(p_options);
    if (e_ERR_NONE != err) {
        return err;
    }

    filt_qual_0 = calculate_filt0(p_options);
    err = CPT2_WRITE_FILT_QUAL0(&handle->ta, filt_qual_0);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    filt_qual_1 = calculate_filt1(p_options);
    err = CPT2_WRITE_FILT_QUAL1(&handle->ta, filt_qual_1);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    filt_qual_2 = calculate_filt2(p_options);
    err = CPT2_WRITE_FILT_QUAL2(&handle->ta, filt_qual_2);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    filt_qual_3 = calculate_filt3(p_options);
    err = CPT2_WRITE_FILT_QUAL3(&handle->ta, filt_qual_3);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    filt_mask_0 = calculate_mask0(p_options);
    err = CPT2_WRITE_FILT_MASK0(&handle->ta, filt_mask_0);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    filt_mask_1 = calculate_mask1(p_options);
    err = CPT2_WRITE_FILT_MASK1(&handle->ta, filt_mask_1);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    filt_mask_2 = calculate_mask2(p_options);
    err = CPT2_WRITE_FILT_MASK2(&handle->ta, filt_mask_2);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    filt_mask_3 = calculate_mask3(p_options);
    err = CPT2_WRITE_FILT_MASK3(&handle->ta, filt_mask_3);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    handle->fast_enable_disable = p_options->fast_enable_disable;

    return err;
}

td_error_t CPT2_enable(
    cpt2_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t probe_stat = 0;

    // Write the configure before enabling.
    handle->probe_ctrl |= CPT2_PROBE_CTRL_TRACE_EN;
    handle->probe_ctrl |= CPT2_PROBE_CTRL_EN;
    err = CPT2_WRITE_PROBE_CNTL(&handle->ta, handle->probe_ctrl);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    if (!handle->fast_enable_disable) {

        err = CPT2_READ_PROBE_CNTL(&handle->ta, &handle->probe_ctrl);
        if (e_ERR_NONE != err) {
            return e_ERR_READ;
        }

        if ((handle->probe_ctrl & CPT2_PROBE_CTRL_TRACE_EN) != CPT2_PROBE_CTRL_TRACE_EN) {
            // Probe trace is not enabled
            err = e_ERR_PROGRAM;
        }

        if ((handle->probe_ctrl & CPT2_PROBE_CTRL_EN) != CPT2_PROBE_CTRL_EN) {
            // Probe is not enabled
            err = e_ERR_PROGRAM;
        }

        err = CPT2_READ_PROBE_STATUS(&handle->ta, &probe_stat);
        if (e_ERR_NONE != err) {
            return e_ERR_READ;
        }

        if ((probe_stat & CPT2_PROBE_EN_STAT) != CPT2_PROBE_EN_STAT) {
            // Probe is not processing transactions
            err = e_ERR_PROGRAM;
        }

    }

    return err;
}

td_error_t CPT2_disable(
    cpt2_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t probe_stat = 0;

    handle->probe_ctrl &= ~CPT2_PROBE_CTRL_EN;
    handle->probe_ctrl &= ~CPT2_PROBE_CTRL_TRACE_EN;
    err = CPT2_WRITE_PROBE_CNTL(&handle->ta, handle->probe_ctrl);
    if (e_ERR_NONE != err) {
        return e_ERR_WRITE;
    }

    if (!handle->fast_enable_disable) {

        err = CPT2_READ_PROBE_CNTL(&handle->ta, &handle->probe_ctrl);
        if (e_ERR_NONE != err) {
            return e_ERR_READ;
        }

        if ((handle->probe_ctrl & CPT2_PROBE_CTRL_TRACE_EN) == CPT2_PROBE_CTRL_TRACE_EN) {
            // Probe trace is still enabled.
            err = e_ERR_PROGRAM;
        }

        if ((handle->probe_ctrl & CPT2_PROBE_CTRL_EN) == CPT2_PROBE_CTRL_EN) {
            // Probe is still enabled.
            err = e_ERR_PROGRAM;
        }

        err = CPT2_READ_PROBE_STATUS(&handle->ta, &probe_stat);
        if (e_ERR_NONE != err) {
            return e_ERR_READ;
        }

        if ((probe_stat & CPT2_PROBE_EN_STAT) == CPT2_PROBE_EN_STAT) {
            // Probe is still processing transactions.
            err = e_ERR_PROGRAM;
        }

    }

    return err;
}


td_error_t CPT2_close(cpt2_handle_t handle)
{
    free(handle);

    return e_ERR_NONE;
}

td_error_t CPT2_get_options_string(cpt2_handle_t handle, cpt2_options_t* p_options, char * buf)
{
    td_error_t err = e_ERR_NONE;
    uint32_t val = 0;

    //<master name>;<mode>;<sampling window>;<addr low msb>;<addr low lsb;<addr hi msb>;<addr hi lsb>;...
    //<qual0>;<qual1>;<qual2>;<qual3>;<mask0>;<mask1>;<mask2>;<mask3>;<bus type>
    //
    // Note: Inserting place holders for master name probe domain as they will be popluated by the decoder.
    sprintf(buf, "MASTERNAME;");

    val = p_options->mode;
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = p_options->sampling_period;
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = p_options->filt_addr_low_msb;
    if (p_options->filt_addr_low_range_exclude) {
        val |= (uint32_t)CPT2_PROBE_FILT_ADDR_LOW_RANGE_EXCLUDE;
    }
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = p_options->filt_addr_low_lsb;
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = p_options->filt_addr_high_msb;
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = p_options->filt_addr_high_lsb;
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_filt0(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_filt1(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_filt2(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_filt3(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_mask0(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_mask1(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_mask2(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    val = calculate_mask3(p_options);
    sprintf(&buf[strlen(buf)], "%u;", val);

    if (handle) {
        sprintf(&buf[strlen(buf)], "%u", handle->bus_type);
    }
    else {
        sprintf(&buf[strlen(buf)], "%u", eCPT2_BUS_UNKNOWN);
    }

    return err;
}


static td_error_t verify_module_id(const cpt2_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t val = 0x0;

    err = CPT2_READ_PROBE_ID(&handle->ta, &val);
    if (err != e_ERR_NONE) {
        return e_ERR_READ;
    }

    if ((val & CPT2_PROBE_ID_FUNCTION_BITS) != CPT2_PROBE_ID_FUNCTION) {
        return e_ERR_BAD_HWTYPE;
    }

    err = CPT2_READ_PROBE_CNTL(&handle->ta, &val);
    if (err != e_ERR_NONE) {
        return e_ERR_READ;
    }

    handle->bus_type = (cpt2_bus_type_t)((val & CPT2_PROBE_CTRL_BUS_BITS) >> CPT2_PROBE_CTRL_BUS_OFFSET);

    return e_ERR_NONE;
}


static td_error_t claim_module(const cpt2_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t ownership = 0;

    // Check if the probe is owned by debugger or application.
    err = CPT2_READ_PROBE_OWN(&handle->ta, &ownership);
    if (err != e_ERR_NONE) {
        return e_ERR_READ;
    }

    if ((ownership & CPT2_PROBE_OWN_BITS) == CPT2_PROBE_OWN_FREE) {
        // Aggregator is not owned. Claim it.
        err = CPT2_WRITE_PROBE_OWN(&handle->ta, CPT2_PROBE_OWN_CLAIM_BIT);
        if (err != e_ERR_NONE) {
            return e_ERR_WRITE;
        }

        err = CPT2_READ_PROBE_OWN(&handle->ta, &ownership);
        if (err != e_ERR_NONE) {
            return e_ERR_READ;
        }

#ifdef _SELF_HOSTED
        if ((ownership & CPT2_PROBE_OWN_BITS) != CPT2_PROBE_OWN_APP) {
            err = e_ERR_OWNERSHIP;
        }
#else
        if ((ownership & CPT2_PROBE_OWN_BITS) != CPT2_PROBE_OWN_DBG) {
            appLogPrintf("Probe is owned by another entity\n");
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
        if ((ownership & CPT2_PROBE_OWN_BITS) != CPT2_PROBE_OWN_APP) {
            err = e_ERR_OWNERSHIP;
        }
        else {
            err = e_ERR_NONE;
        }
#else
        appLogPrintf("Probe is owned by another entity\n");
        // If probe is already owned by debugger, nothing else needs to be done.
        if ((ownership & CPT2_PROBE_OWN_BITS) == CPT2_PROBE_OWN_DBG) {
            err = e_ERR_NONE;
        }
        else {
            // If probe is owned by application, override ownership as debugger.

            ownership &= ~CPT2_PROBE_OWN_CLAIM_BIT;
            err = CPT2_WRITE_PROBE_OWN(&handle->ta, ownership);
            if (err != e_ERR_NONE) {
                return e_ERR_WRITE;
            }

            err = CPT2_WRITE_PROBE_OWN(&handle->ta, CPT2_PROBE_OWN_CLAIM_BIT);
            if (err != e_ERR_NONE) {
                return e_ERR_WRITE;
            }

            err = CPT2_READ_PROBE_OWN(&handle->ta, &ownership);
            if (err != e_ERR_NONE) {
                return e_ERR_READ;
            }

            if ((ownership & CPT2_PROBE_OWN_BITS) != CPT2_PROBE_OWN_DBG) {
                err = e_ERR_OWNERSHIP;
            }
        }
#endif
    }

    return err;
}

static td_error_t check_filter_and_masks(const cpt2_options_t * p_options)
{
    td_error_t err = e_ERR_NONE;

    if (p_options->filt_channel_id > CPT2_PROBE_CHANNEL_ID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_channel_id_mask > CPT2_PROBE_CHANNEL_ID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_cmsideband > CPT2_PROBE_CMSIDEBAND_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_cmsideband_mask > CPT2_PROBE_CMSIDEBAND_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_csideband > CPT2_PROBE_CSIDEBAND_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_csideband_mask > CPT2_PROBE_CSIDEBAND_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_order_id > CPT2_PROBE_ORDERID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_order_id_mask > CPT2_PROBE_ORDERID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_amode > CPT2_PROBE_AMODE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_amode_mask > CPT2_PROBE_AMODE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_route_id > CPT2_PROBE_ROUTEID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_route_id_mask > CPT2_PROBE_ROUTEID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_rsel > CPT2_PROBE_RSEL_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_rsel_mask > CPT2_PROBE_RSEL_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_dtype > CPT2_PROBE_DTYPE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_dtype_mask > CPT2_PROBE_DTYPE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_priv > CPT2_PROBE_PRIV_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_priv_mask > CPT2_PROBE_PRIV_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_clsize > CPT2_PROBE_CLSIZE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_clsize_mask > CPT2_PROBE_CLSIZE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_epriority > CPT2_PROBE_EPRIORITY_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_epriority_mask > CPT2_PROBE_EPRIORITY_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_priority > CPT2_PROBE_PRIORITY_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_priority_mask > CPT2_PROBE_PRIORITY_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_bytecount > CPT2_PROBE_BYTECOUNT_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_bytecount_mask > CPT2_PROBE_BYTECOUNT_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_opcode > CPT2_PROBE_OPCODE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_opcode_mask > CPT2_PROBE_OPCODE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_memtype > CPT2_PROBE_MEMTYPE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_memtype_mask > CPT2_PROBE_MEMTYPE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_qos > CPT2_PROBE_QOS_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_qos_mask > CPT2_PROBE_QOS_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_sdomain > CPT2_PROBE_SDOMAIN_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_sdomain_mask > CPT2_PROBE_SDOMAIN_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_outer > CPT2_PROBE_OUTER_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_outer_mask > CPT2_PROBE_OUTER_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_inner > CPT2_PROBE_INNER_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_inner_mask > CPT2_PROBE_INNER_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_fwpass > CPT2_PROBE_FWPASS_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_fwpass_mask > CPT2_PROBE_FWPASS_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_asel > CPT2_PROBE_ASEL_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_asel_mask > CPT2_PROBE_ASEL_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_atype > CPT2_PROBE_ATYPE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_atype_mask > CPT2_PROBE_ATYPE_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_privid > CPT2_PROBE_PRIVID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_privid_mask > CPT2_PROBE_PRIVID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_virtid > CPT2_PROBE_VIRTID_MAX) {
        return e_ERR_BAD_PARAM;
    }

    if (p_options->filt_virtid_mask > CPT2_PROBE_VIRTID_MAX) {
        return e_ERR_BAD_PARAM;
    }
    
    return err;
}

uint32_t calculate_filt0(const cpt2_options_t * p_options)
{
    uint32_t filt_qual_0 = 0;

    filt_qual_0 |= (CPT2_PROBE_FILT0_CHANID_7_5_BITS & (p_options->filt_channel_id << CPT2_PROBE_FILT0_CHANID_7_5_SHIFT));
    filt_qual_0 |= (CPT2_PROBE_FILT0_PABLE_BIT & (p_options->filt_pable << CPT2_PROBE_FILT0_PABLE_OFFSET));
    filt_qual_0 |= (CPT2_PROBE_FILT0_CHANID_4_2_BITS & (p_options->filt_channel_id << CPT2_PROBE_FILT0_CHANID_4_2_SHIFT));
    filt_qual_0 |= (CPT2_PROBE_FILT0_CMSIDEBANDEXT_BIT & (p_options->filt_cmsideband << CPT2_PROBE_FILT0_CMSIDEBANDEXT_SHIFT));
    filt_qual_0 |= (CPT2_PROBE_FILT0_CHANID_1_0_BITS & (p_options->filt_channel_id << CPT2_PROBE_FILT0_CHANID_1_0_SHIFT));
    filt_qual_0 |= (CPT2_PROBE_FILT0_CSIDEBANDEXT_BITS & (p_options->filt_csideband << CPT2_PROBE_FILT0_CSIDEBANDEXT_SHIFT));
    filt_qual_0 |= (CPT2_PROBE_FILT0_ORDERID_BITS & (p_options->filt_order_id << CPT2_PROBE_FILT0_ORDERID_OFFSET));
    filt_qual_0 |= (CPT2_PROBE_FILT0_EXCL_BIT & (p_options->filt_excl << CPT2_PROBE_FILT0_EXCL_OFFSET));
    filt_qual_0 |= (CPT2_PROBE_FILT0_DIR_BIT & (p_options->filt_dir << CPT2_PROBE_FILT0_DIR_OFFSET));
    filt_qual_0 |= (CPT2_PROBE_FILT0_AMODE_BITS & (p_options->filt_amode << CPT2_PROBE_FILT0_AMODE_OFFSET));
    filt_qual_0 |= (CPT2_PROBE_FILT0_ROUTEID_BITS & (p_options->filt_route_id << CPT2_PROBE_FILT0_ROUTEID_OFFSET));

    return filt_qual_0;
}

uint32_t calculate_filt1(const cpt2_options_t * p_options)
{
    uint32_t filt_qual_1 = 0;

    filt_qual_1 |= (CPT2_PROBE_FILT1_RSEL_BITS & (p_options->filt_rsel << CPT2_PROBE_FILT1_RSEL_OFFSET));
    filt_qual_1 |= (CPT2_PROBE_FILT1_DTYPE_BITS & (p_options->filt_dtype << CPT2_PROBE_FILT1_DTYPE_OFFSET));
    filt_qual_1 |= (CPT2_PROBE_FILT1_CHANID_11_8_BITS & (p_options->filt_channel_id << CPT2_PROBE_FILT1_CHANID_11_8_SHIFT));
    filt_qual_1 |= (CPT2_PROBE_FILT1_PRIV_BITS & (p_options->filt_priv << CPT2_PROBE_FILT1_PRIV_OFFSET));
    filt_qual_1 |= (CPT2_PROBE_FILT1_CLSIZE_BITS & (p_options->filt_clsize << CPT2_PROBE_FILT1_CLSIZE_OFFSET));
    filt_qual_1 |= (CPT2_PROBE_FILT1_EPRIORITY_BITS & (p_options->filt_epriority << CPT2_PROBE_FILT1_EPRIORITY_OFFSET));
    filt_qual_1 |= (CPT2_PROBE_FILT1_PRIORITY_BITS & (p_options->filt_priority << CPT2_PROBE_FILT1_PRIORITY_OFFSET));
    filt_qual_1 |= (CPT2_PROBE_FILT1_BYTECOUNT_BITS & (p_options->filt_bytecount << CPT2_PROBE_FILT1_BYTECOUNT_OFFSET));

    return filt_qual_1;
}

uint32_t calculate_filt2(const cpt2_options_t * p_options)
{
    uint32_t filt_qual_2 = 0;

    filt_qual_2 |= (CPT2_PROBE_FILT2_CMSIDEBAND_BITS & (p_options->filt_cmsideband << CPT2_PROBE_FILT2_CMSIDEBAND_SHIFT));
    filt_qual_2 |= (CPT2_PROBE_FILT2_CSIDEBAND_BITS & (p_options->filt_csideband << CPT2_PROBE_FILT2_CSIDEBAND_SHIFT));
    filt_qual_2 |= (CPT2_PROBE_FILT2_OPCODE_BITS & (p_options->filt_opcode << CPT2_PROBE_FILT2_OPCODE_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_MEMTYPE_BITS & (p_options->filt_memtype << CPT2_PROBE_FILT2_MEMTYPE_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_SECURE_BIT & (p_options->filt_secure << CPT2_PROBE_FILT2_SECURE_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_QOS_BITS & (p_options->filt_qos << CPT2_PROBE_FILT2_QOS_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_SDOMAIN_BITS & (p_options->filt_sdomain << CPT2_PROBE_FILT2_SDOMAIN_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_OUTER_BITS & (p_options->filt_outer << CPT2_PROBE_FILT2_OUTER_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_INNER_BITS & (p_options->filt_inner << CPT2_PROBE_FILT2_INNER_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_EMUDBG_BIT & (p_options->filt_emudbg << CPT2_PROBE_FILT2_EMUDBG_OFFSET));
    filt_qual_2 |= (CPT2_PROBE_FILT2_INTEREST_BIT & (p_options->filt_interest << CPT2_PROBE_FILT2_INTEREST_OFFSET));

    return filt_qual_2;
}

uint32_t calculate_filt3(const cpt2_options_t * p_options)
{
    uint32_t filt_qual_3 = 0;

    filt_qual_3 |= (CPT2_PROBE_FILT3_FLUSH_BIT & (p_options->filt_flush << CPT2_PROBE_FILT3_FLUSH_OFFSET));
    filt_qual_3 |= (CPT2_PROBE_FILT3_FWPASS_BITS & (p_options->filt_fwpass << CPT2_PROBE_FILT3_FWPASS_OFFSET));
    filt_qual_3 |= (CPT2_PROBE_FILT3_ASEL_BITS & (p_options->filt_asel << CPT2_PROBE_FILT3_ASEL_OFFSET));
    filt_qual_3 |= (CPT2_PROBE_FILT3_ATYPE_BITS & (p_options->filt_atype << CPT2_PROBE_FILT3_ATYPE_OFFSET));
    filt_qual_3 |= (CPT2_PROBE_FILT3_PRIVID_BITS & (p_options->filt_privid << CPT2_PROBE_FILT3_PRIVID_OFFSET));
    filt_qual_3 |= (CPT2_PROBE_FILT3_VIRTID_BITS & (p_options->filt_virtid << CPT2_PROBE_FILT3_VIRTID_OFFSET));

    return filt_qual_3;
}

uint32_t calculate_mask0(const cpt2_options_t * p_options)
{
    uint32_t filt_mask_0 = 0;

    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_CHANID_7_5_BITS & (p_options->filt_channel_id_mask << CPT2_PROBE_FILTMASK0_CHANID_7_5_SHIFT));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_PABLE_BIT & (p_options->filt_pable_mask << CPT2_PROBE_FILTMASK0_PABLE_OFFSET));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_CHANID_4_2_BITS & (p_options->filt_channel_id_mask << CPT2_PROBE_FILTMASK0_CHANID_4_2_SHIFT));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_CMSIDEBANDEXT_BIT & (p_options->filt_cmsideband_mask << CPT2_PROBE_FILTMASK0_CMSIDEBANDEXT_SHIFT));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_CHANID_1_0_BITS & (p_options->filt_channel_id_mask << CPT2_PROBE_FILTMASK0_CHANID_1_0_SHIFT));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_CSIDEBANDEXT_BITS & (p_options->filt_csideband_mask << CPT2_PROBE_FILTMASK0_CSIDEBANDEXT_SHIFT));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_ORDERID_BITS & (p_options->filt_order_id_mask << CPT2_PROBE_FILTMASK0_ORDERID_OFFSET));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_EXCL_BIT & (p_options->filt_excl_mask << CPT2_PROBE_FILTMASK0_EXCL_OFFSET));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_DIR_BIT & (p_options->filt_dir_mask << CPT2_PROBE_FILTMASK0_DIR_OFFSET));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_AMODE_BITS & (p_options->filt_amode_mask << CPT2_PROBE_FILTMASK0_AMODE_OFFSET));
    filt_mask_0 |= (CPT2_PROBE_FILTMASK0_ROUTEID_BITS & (p_options->filt_route_id_mask << CPT2_PROBE_FILTMASK0_ROUTEID_OFFSET));

    return filt_mask_0;
}

uint32_t calculate_mask1(const cpt2_options_t * p_options)
{
    uint32_t filt_mask_1 = 0;

    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_RSEL_BITS & (p_options->filt_rsel_mask << CPT2_PROBE_FILTMASK1_RSEL_OFFSET));
    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_DTYPE_BITS & (p_options->filt_dtype_mask << CPT2_PROBE_FILTMASK1_DTYPE_OFFSET));
    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_CHANID_11_8_BITS & (p_options->filt_channel_id_mask << CPT2_PROBE_FILTMASK1_CHANID_11_8_SHIFT));
    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_PRIV_BITS & (p_options->filt_priv_mask << CPT2_PROBE_FILTMASK1_PRIV_OFFSET));
    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_CLSIZE_BITS & (p_options->filt_clsize_mask << CPT2_PROBE_FILTMASK1_CLSIZE_OFFSET));
    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_EPRIORITY_BITS & (p_options->filt_epriority_mask << CPT2_PROBE_FILTMASK1_EPRIORITY_OFFSET));
    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_PRIORITY_BITS & (p_options->filt_priority_mask << CPT2_PROBE_FILTMASK1_PRIORITY_OFFSET));
    filt_mask_1 |= (CPT2_PROBE_FILTMASK1_BYTECOUNT_BITS & (p_options->filt_bytecount_mask << CPT2_PROBE_FILTMASK1_BYTECOUNT_OFFSET));

    return filt_mask_1;
}

uint32_t calculate_mask2(const cpt2_options_t * p_options)
{
    uint32_t filt_mask_2 = 0;

    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_CMSIDEBAND_BITS & (p_options->filt_cmsideband_mask << CPT2_PROBE_FILTMASK2_CMSIDEBAND_SHIFT));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_CSIDEBAND_BITS & (p_options->filt_csideband_mask << CPT2_PROBE_FILTMASK2_CSIDEBAND_SHIFT));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_OPCODE_BITS & (p_options->filt_opcode_mask << CPT2_PROBE_FILTMASK2_OPCODE_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_MEMTYPE_BITS & (p_options->filt_memtype_mask << CPT2_PROBE_FILTMASK2_MEMTYPE_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_SECURE_BIT & (p_options->filt_secure_mask << CPT2_PROBE_FILTMASK2_SECURE_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_QOS_BITS & (p_options->filt_qos_mask << CPT2_PROBE_FILTMASK2_QOS_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_SDOMAIN_BITS & (p_options->filt_sdomain_mask << CPT2_PROBE_FILTMASK2_SDOMAIN_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_OUTER_BITS & (p_options->filt_outer_mask << CPT2_PROBE_FILTMASK2_OUTER_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_INNER_BITS & (p_options->filt_inner_mask << CPT2_PROBE_FILTMASK2_INNER_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_EMUDBG_BIT & (p_options->filt_emudbg_mask << CPT2_PROBE_FILTMASK2_EMUDBG_OFFSET));
    filt_mask_2 |= (CPT2_PROBE_FILTMASK2_INTEREST_BIT & (p_options->filt_interest_mask << CPT2_PROBE_FILTMASK2_INTEREST_OFFSET));

    return filt_mask_2;
}

uint32_t calculate_mask3(const cpt2_options_t * p_options)
{
    uint32_t filt_mask_3 = 0;

    filt_mask_3 |= (CPT2_PROBE_FILTMASK3_FLUSH_BIT & (p_options->filt_flush_mask << CPT2_PROBE_FILTMASK3_FLUSH_OFFSET));
    filt_mask_3 |= (CPT2_PROBE_FILTMASK3_FWPASS_BITS & (p_options->filt_fwpass_mask << CPT2_PROBE_FILTMASK3_FWPASS_OFFSET));
    filt_mask_3 |= (CPT2_PROBE_FILTMASK3_ASEL_BITS & (p_options->filt_asel_mask << CPT2_PROBE_FILTMASK3_ASEL_OFFSET));
    filt_mask_3 |= (CPT2_PROBE_FILTMASK3_ATYPE_BITS & (p_options->filt_atype_mask << CPT2_PROBE_FILTMASK3_ATYPE_OFFSET));
    filt_mask_3 |= (CPT2_PROBE_FILTMASK3_PRIVID_BITS & (p_options->filt_privid_mask << CPT2_PROBE_FILTMASK3_PRIVID_OFFSET));
    filt_mask_3 |= (CPT2_PROBE_FILTMASK3_VIRTID_BITS & (p_options->filt_virtid_mask << CPT2_PROBE_FILTMASK3_VIRTID_OFFSET));

    return filt_mask_3;
}

