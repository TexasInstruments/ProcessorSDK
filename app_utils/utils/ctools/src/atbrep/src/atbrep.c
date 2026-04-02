/**
* atb_rep.c
*
* ATB Replicator IP Driver Implementation
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
#include "atbrep.h"
#include "atbrep_access.h"
#include "td_error.h"
#include "target_access.h"

#define LOCKACC_UNLOCK_VALUE      0xC5ACCE55
#define LOCKSTAT_IMPLEMENTED_BIT	 (1<<0)
#define LOCKSTAT_LOCKED_BIT	     (1<<1)
#define DEVTYPE_VAL                  0x22


static td_error_t _unlock(const target_access_handle_t handle);
static td_error_t _check_device(const target_access_handle_t handle);

td_error_t  
ATBREP_open(
    const atbrep_init_t *p_init,
    atbrep_handle_t     *p_handle
    )
{
    atbrep_data_t  *p_data = 0;
    uint32_t        devtype = 0;
    uint32_t        devid = 0;
    td_error_t         err = e_ERR_NONE;

    if ((0 == p_init) || (0 == p_handle) || (0 == p_init->ta) ) {
        return e_ERR_BAD_PARAM;
    }

    /* initalize client handle to NULL*/
    *p_handle = 0;

    /* Unlock the hardware */
    err = _unlock(p_init->ta);

    if (e_ERR_NONE == err) {
        err = _check_device(p_init->ta);
    }

    if (e_ERR_NONE == err) {
        /* read the master id count */
        err = ATBREP_READ_DEVID(p_init->ta, &devid);
    }

    if (e_ERR_NONE == err) {
        /* read the devtype */
        err = ATBREP_READ_DEVTYPE(p_init->ta, &devtype);
    }

    if (e_ERR_NONE == err) {
        /* Set up the client handle */
        p_data = (atbrep_data_t *)malloc(sizeof(atbrep_data_t));
        if (0 == p_data) {
            err = e_ERR_ALLOC;
        }
    }

    if (e_ERR_NONE == err) {

        p_data->ta = *(p_init->ta);
        p_data->devtype = devtype;
        p_data->master_count = devid & 0x7;

        *p_handle = p_data;
    }
    else if (0 != p_data) {
        free(p_data);
    }

    return err;

}

td_error_t
ATBREP_get_property(
    const atbrep_handle_t handle,
    atbrep_property_t *p_property
    )
{
    /* check that parameters are valid */
    if ((0 == handle) || (handle->devtype != DEVTYPE_VAL)) {
        return e_ERR_BAD_PARAM;
    }
    p_property->master_count = handle->master_count;

    return e_ERR_NONE;

}

td_error_t
ATBREP_set_filter(
    const atbrep_handle_t  handle,
    const unsigned         master_id,
    const atbrep_filters_t filter
)

{
    td_error_t  err = e_ERR_NONE;

    if ((0 == handle) || (handle->devtype != DEVTYPE_VAL) || (master_id >= handle->master_count)) {
        return e_ERR_BAD_PARAM;
    }

    if (master_id == 0) {
        err = ATBREP_WRITE_IDFILTER0(&(handle->ta), filter);
    }
    else {
        err = ATBREP_WRITE_IDFILTER1(&(handle->ta), filter);
    }

    return err;

}

td_error_t
ATBREP_get_filter(
const atbrep_handle_t  handle,
const unsigned         master_id, 
atbrep_filters_t *p_filters
)

{
    td_error_t  err = e_ERR_NONE;
    (void)p_filters;

    if ((0 == handle) || (handle->devtype != DEVTYPE_VAL) || (master_id >= handle->master_count)) {
        return e_ERR_BAD_PARAM;
    }

    uint32_t val;
    if (master_id == 0) {
        err = ATBREP_READ_IDFILTER0(&(handle->ta), &val);
    }
    else {
        err = ATBREP_READ_IDFILTER1(&(handle->ta), &val);
    }

    return err;

}

void
ATBREP_close(
    const atbrep_handle_t handle
    )
{
    if ((0 != handle) || (handle->devtype == DEVTYPE_VAL)) {
        handle->devtype = 0;
        free(handle);
    }

}

static td_error_t _unlock(const target_access_handle_t handle)
{
    td_error_t err;
    uint32_t regval;

    /* Read the lock status register         */
    err = ATBREP_READ_LOCKSTAT(handle, &regval);

    if (e_ERR_NONE == err) {

        /* If locking is implemented and the hardware is locked then unlock it */
        if ((LOCKSTAT_IMPLEMENTED_BIT == (regval & LOCKSTAT_IMPLEMENTED_BIT)) &&
            (LOCKSTAT_LOCKED_BIT == (regval & LOCKSTAT_LOCKED_BIT))) {

            err = ATBREP_WRITE_LOCKACC(handle, LOCKACC_UNLOCK_VALUE);
            if (e_ERR_NONE == err) {

                /* Read back the LSR to make sure the unlock was successful */
                regval = 0;
                err = ATBREP_READ_LOCKACC(handle, &regval);
                if (e_ERR_NONE == err) {
                    if ((regval & LOCKSTAT_LOCKED_BIT) == LOCKSTAT_LOCKED_BIT) {
                        err = e_ERR_LOCKED;
                    }
                }
            }
        }
    }

    return err;

}

static td_error_t _check_device(const target_access_handle_t handle)
{
    td_error_t err = e_ERR_NONE;
    uint32_t devtype;

    /* Check that device identifier is correct */
    err = ATBREP_READ_DEVTYPE(handle, &devtype);
    if (e_ERR_NONE == err) {
        if (devtype != DEVTYPE_VAL) {
            err = e_ERR_BAD_HWTYPE;
        }
    }

    return err;

}
