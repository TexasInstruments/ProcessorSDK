/**
* tbr.c
*
* CT-TBR Driver Implementation
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
#include "tbr.h"
#include "tbr_access.h"


#define MAX_TRIES 20

typedef struct _tbr_data_t {
    uint32_t               handle_id;
    target_access_t        ta;
    uint32_t               depth;        // Depth in 32b words
    bool                   b_siwidth_64;
    tbr_config_t           config;
} tbr_data_t;



#define HANDLE_ID ((DEVTYPE<<16)|(PARTNO))

static td_error_t  _flush(const tbr_handle_t handle, tbr_flush_type_t type);
static td_error_t  _unlock(const tbr_handle_t handle);
static td_error_t  _claim(const tbr_handle_t handle);
static td_error_t  _unclaim(const tbr_handle_t handle);
static td_error_t  _check_device(const tbr_handle_t handle);
static td_error_t  _configure(const tbr_handle_t handle);
static td_error_t  _disable(const tbr_handle_t handle);
static td_error_t  _enable(const tbr_handle_t handle);
static td_error_t  _reset(const tbr_handle_t handle);
static td_error_t  _status(const tbr_handle_t handle, tbr_status_t *status, uint32_t *available);


/**
* TBR_open - open TBR programming module interface
*/
td_error_t
TBR_open(
    const tbr_init_t      *p_init,
    tbr_handle_t          *p_handle
    )
{
    td_error_t err = e_ERR_NONE;
    tbr_data_t *p_data = 0;
    tbr_config_t default_config = cTBR_DEFAULT_CONFIG;

    /* check that parameters are valid */
    if ((0 == p_init) || (0 == p_handle) || (0 == p_init->ta)) {
        return e_ERR_BAD_PARAM;
    }

    /* Create and initialize the client handle */

    p_data = (tbr_data_t *)malloc(sizeof(tbr_data_t));
    if (0 != p_data) {

        p_data->depth = 0;
        p_data->handle_id = HANDLE_ID;
        p_data->ta = *p_init->ta;

        /* default configuration */
        p_data->config = default_config;

    }
    else {
        err = e_ERR_ALLOC;
        return err;
    }

    if (e_ERR_NONE == err) {
        err = _unlock(p_data);
    }
     
    if (e_ERR_NONE == err) {
        /* claim the TBR */
        err = _claim(p_data);
    }

    /* unlock the TBR */
    if (e_ERR_NONE == err) {

        /* make sure this is a TBR */
        if (e_ERR_NONE == err) {

            err = _check_device(p_data);

            /* read the size of the TBR buffer - 0 implies system bridge only */
            if (e_ERR_NONE == err) {
                uint32_t val;

                err = TBR_READ_RAMSZ(&p_data->ta, &val);

                if (e_ERR_NONE == err) {
                    if (0 != val) {
                        p_data->depth = ((2 << val) * 1024) / 4;
                    }
                }
            }

            if (e_ERR_NONE == err) {
                uint32_t val;
                err = TBR_READ_SICTRL(&p_data->ta, &val);
                if (e_ERR_NONE == err) {
                    p_data->b_siwidth_64 = ((val & SICTRL_DATA_WIDTH_64) == SICTRL_DATA_WIDTH_64);
                }
            }

            /* initialize the TBR  */
            if (e_ERR_NONE == err) {
                err = _configure(p_data);
            }

        }

        /* If an error occured - unclaim */
        if (e_ERR_NONE != err) {
            _unclaim(p_data);
        }
        
    }


    /* set the return parameters */
    if (e_ERR_NONE == err) {
        *p_handle = p_data;
    }
    else {
        free(p_data);
    }

    return err;

}

td_error_t
TBR_configure(
    const tbr_handle_t            handle,
    const tbr_config_t           *p_config
    )
{
    if ((0 == handle) || (handle->handle_id != HANDLE_ID) ) {
        return e_ERR_BAD_PARAM;
    }

    if (p_config) {
        handle->config = *p_config;
    }

    return _configure(handle);
 }


void
TBR_close(
    tbr_handle_t handle
    )
{
    /* check that the parameters are valid */
    if ((0 != handle) && (handle->handle_id == HANDLE_ID)) {

        /* clear the the claim */
        (void)_unclaim(handle);

        /* delete the handle storage */
        /* set the handle id to an invalid value before deleting */
        /* to avoid a stale handle being subsequently used       */
        handle->handle_id = 0;
        free(handle);
    }

}

td_error_t
TBR_get_property(
    const tbr_handle_t handle,
    tbr_property_t *p_property
    )
{
    /* check that parameters are valid */
    if ((0 == handle) || (handle->handle_id != HANDLE_ID) || (0==p_property)) {
        return e_ERR_BAD_PARAM;
    }
    p_property->depth = handle->depth;

    return e_ERR_NONE;

}


td_error_t 
TBR_disable(
    const tbr_handle_t handle
    )
{
    /* check that parameters are valid */
    if ((0 == handle) || (handle->handle_id != HANDLE_ID)) {
        return e_ERR_BAD_PARAM;
    }

    /* call internal function to disable */
    return _disable(handle);
}


td_error_t 
TBR_reset(
    const  tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;
    int retries = 1000;
    bool b_reset_active = true;

    /* check that parameters are valid */
    if ((0 == handle) || (handle->handle_id != HANDLE_ID)) {
        return e_ERR_BAD_PARAM;
    }
   
    /* call internal function to reset */
    err = _reset(handle);

    while (b_reset_active && (e_ERR_NONE == err) && (retries-->0)) {
        uint32_t stat;
        err = TBR_READ_STAT(&handle->ta, &stat);
        if (e_ERR_NONE == err) {
            b_reset_active = ((stat & STAT_RESET_ACT) == STAT_RESET_ACT);
        }
    }

    if (b_reset_active) {
        err = e_ERR_PROGRAM;
    }

    /* unlock the TBR */
    if (e_ERR_NONE == err) {
        err = _unlock(handle);
    }

    if (e_ERR_NONE == err) {
       err = _configure(handle);
    }

    return err;
}
    
td_error_t  
TBR_restart(
    const tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;

    /* check that parameters are valid */
    if ((0 == handle) || (handle->handle_id != HANDLE_ID)) {
        return e_ERR_BAD_PARAM;
    }

    /* Reset TBR RAM write pointer to 0 */
    err = TBR_WRITE_RAMWPTR(&handle->ta, 0);

    if (e_ERR_NONE == err) {
        /* Reset the RAM read pointer to 0 */
        err = TBR_WRITE_RAMRPTR(&handle->ta, 0);
    }

    return err;

}

td_error_t
TBR_enable(
    const tbr_handle_t handle
    )
{
    if ((0 == handle) || (handle->handle_id != HANDLE_ID)) {
        return e_ERR_BAD_PARAM;
    }
    return _enable(handle);
}


/**
* TBR_status- Return TBR status 

31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|                              Reserved                                    | 0| 0| 1| 1| 1| 0| 0|
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

RESET_ACT[6:6]     - Reset active
PARTIAL_OUT[5:5]   - Partial output block in memory (OUTFLUSH sequence may be required)
DRAIN_DONE[4:4]    - Data in Output FIFO (only system bridge mode)
FMT_DONE[3:3]      - Final data from formatter has been posted
CPT_DONE[2:2]      - Trace capture complete
TRIG[1:1]          - Trigger input detected and buffer locked
WRAP[0:0]          - Write pointer has wrapped

*/

td_error_t  
TBR_status(
    const tbr_handle_t handle, 
    tbr_status_t* p_status, 
    uint32_t *p_available
    )
{
    td_error_t err = e_ERR_NONE;

    /* check that parameters are valid */
    if ((0 == handle) || (handle->handle_id != HANDLE_ID) ) {
        return e_ERR_BAD_PARAM;
    }

    err = _status(handle, p_status, p_available);

    return err;
}



/**
* TBR_read- Read TBR data
*
*
*/
td_error_t  
TBR_read(
    const tbr_handle_t handle, 
    const uint32_t requested, 
    uint32_t *p_buffer, 
    uint32_t *p_actual
    )
{
    td_error_t err = e_ERR_NONE;
    uint32_t actual = 0;

    /* check that parameters are valid */
    if ((0 == handle) || (handle->handle_id != HANDLE_ID) || (NULL == p_buffer) ) {
        return e_ERR_BAD_PARAM;
    }

    err = _disable(handle);

    if (e_ERR_NONE == err){
        err = TBR_READ_BUFFER(&handle->ta, requested, p_buffer, &actual);

        if (e_ERR_NONE == err) {
            *p_actual = actual;
        }
    }

    return err;
}




/**
* TBR_flush - Flush the TBR
*/
td_error_t  
TBR_flush(
    const tbr_handle_t handle,
    const tbr_flush_type_t type
    )
{
    /* check that parameters are valid */
    if ((0 == handle) || (handle->handle_id != HANDLE_ID) ) {
        return e_ERR_BAD_PARAM;
    }


    return _flush(handle,type);

}




/**
* flush - internal function used to flush the TBR from both the TBR_Disable API
*         and the TBR_flush API.
*/
static td_error_t 
_flush( 
    const tbr_handle_t handle, 
    const tbr_flush_type_t type 
    )
{
    td_error_t err = e_ERR_NONE;
    uint32_t opctrl = 0;
    unsigned maxtries;

    err = TBR_READ_OPCTRL(&handle->ta, &opctrl);

    if (e_ERR_NONE == err) {

        /* Acquistion flush */
        if (e_TBR_FLUSH_ACQUISTION == type) {

            opctrl |= OPCTRL_FLUSH;
            err = TBR_WRITE_OPCTRL(&handle->ta, opctrl);

            maxtries = 20;

            /* Wait for acquisition flush to complete by monitoring OPCTRL::FLUSH bit */
            while ((e_ERR_NONE == err) && (maxtries-- > 0) && (OPCTRL_FLUSH == (opctrl & OPCTRL_FLUSH))) {
                err = TBR_READ_OPCTRL(&handle->ta, &opctrl);
            }
#if 0
            if (OPCTRL_FLUSH == (opctrl & OPCTRL_FLUSH)) {
                err = e_ERR_PROGRAM;
            }
#endif
        }

        /* Output flush - only available in system bridge mode */
        else if (eTBR_MODE_BRIDGE == handle->config.mode ) {
            opctrl |= OPCTRL_OUTFLUSH;
            err = TBR_WRITE_OPCTRL(&handle->ta, opctrl);

            maxtries = 1000;

            /* Wait for acquisition flush to complete by monitoring OPCTRL::OUTFLUSH bit */
            while ((e_ERR_NONE == err) && (maxtries-- > 0) && (OPCTRL_OUTFLUSH == (opctrl & OPCTRL_OUTFLUSH))) {
                err = TBR_READ_OPCTRL(&handle->ta, &opctrl);
            }
        }
    }

    return err;
}





static td_error_t 
_configure( 
    const tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;

    handle->config.b_formatter = true;

    err = _disable(handle);

    if (e_ERR_NONE == err) {
        uint32_t trgcnt = 0;

        /* Configure TRGCNT register */
        /*
         * TRGCNT
         *
         *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
         *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
         *  |                              Reserved                  |                  COUNT               |
         *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
         *
         * TRGCNT::COUNT[12:0] - the number of 128-bit words written local memory after a trigger is detected
         */

        switch (handle->config.trigger_mode) {
        case eTBR_TRACE_BEFORE:
            /* */
            trgcnt = 8;
            break;
        case eTBR_TRACE_ABOUT:
            /* half of the buffer */
			trgcnt = (handle->depth / 4) / 2;  // Convert to # of 16B entries (depth / 4) then compute half (/2)
            break;
        case eTBR_TRACE_AFTER:
            trgcnt = (handle->depth / 4) - 100; // Convert to # of 16B entries (depth / 4) then reduce by 100
            break;
        case eTBR_TRACE_NOTRG:
        default:
            break;
        }
        err = TBR_WRITE_TRGCNT(&handle->ta, trgcnt);
    }

    if (e_ERR_NONE == err) {
        uint32_t tbr_ctrl = 0;

        /* Configure the TBR_CTRL register */

        /* TBR_CTRL
         *
         *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
         *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
         *  |                              Reserved                                          |I |S |R |M |E |
         *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
         *
         * TBR_CTRL::ENBL[1:0]         - Enable
         * TBR_CTRL::MODE[2:1]         - Buffer or System bridge mode
         * TBR_CTRL::SRST[3:2]         - Perform Software Reset
         * TBR_CTRL::SYNCREQ_WRAP[4:3] - Issue a pulse on SYNCREQ when buffer wraps to 0
         * TBR_CTRL::IDLE_EMPTY[5:4]   - Controls whether TBR can enter idle with data present
         */

      

        if (eTBR_MODE_BRIDGE == handle->config.mode) {
            tbr_ctrl |= CTRL_MODE;
        }
        else {
            if (handle->config.b_syncreq_wrap) {
                tbr_ctrl |= CTRL_SYNCREQ_WRAP;
            }
        }
        err = TBR_WRITE_CTRL(&handle->ta, tbr_ctrl);
    }


    if (e_ERR_NONE == err) {

        /* Configure the OPCTRL register */
        uint32_t opctrl = 0;

        /* OPCTRL
        *
        *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
        *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        *  |                  Reserved               |SR|OF|SF|x |ST|SF|x |TF|TE|TT|x |FL|FT|FF|x |x |IT|FE|
        *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        *
        * OPCTRL::FMT_ENBL[0:0]     - Enable formatter
        * OPCTRL::INSRT_TRG[1:1]    - Insert TWP trigger sequence into stream
        * OPCTRL::FLSH_FLSHIN[4:4]  - Generate a flush on rising edge of FLUSHIN
        * OPCTRL::FLSH_TRG[5:5]     - Generate a flush when trigger condition is detected
        * OPCTRL::FLUSH[6:6]        - Generate a Manaual Flush
        * OPCTRL::TRG_TRGIN[8:8]    - Generate trigger on rising edge of TRGIN
        * OPCTRL::TRG_EVT[9:9]      - Delay trigger until trigger count matches TRGCNT
        * OPCTRL::TRG_FLSH[10:10]   - Stop formatter when flush completes
        * OPCTRL::STP_FLSH[12:12]   - Generate trigger when flush completes
        * OPCTRL::STP_TRG[13:13]    - Formatter stops when trigger is detected
        * OPCTRL::STP_FULL[15:15]   - Formatter stops when local memory is full
        * OPCTRL::OUTFLUSH[16:16]   - Trigger a flush of the existing data in the TBR memory
        * OPCTRL::SYNCREQ[17:17]    - Stimulate a synchronization request
        */

        if (eTBR_TRACE_NOTRG != handle->config.trigger_mode) {
            opctrl |= OPCTRL_TRG_EVT;
        }

        if (handle->config.b_formatter ) {
            opctrl |= OPCTRL_FMT_ENBL;
            if (handle->config.b_insert_trig) {
                opctrl |= OPCTRL_INSRT_TRG;
            }
        }

        if (handle->config.b_flshin_enable) {
            opctrl |= OPCTRL_FLSH_FLSHIN;
        }
        if (handle->config.b_trgin_enable) {
            opctrl |= OPCTRL_TRG_TRGIN;
        }
        switch (handle->config.on_trigger) {
            case eTBR_ACTION_FLUSH:
                opctrl |= OPCTRL_FLSH_TRG;
                break;
            case eTBR_ACTION_STOP:
                opctrl |= OPCTRL_STP_TRG;
                break;
            case eTBR_ACTION_TRIGGER:
            case eTBR_ACTION_NONE:
            default:
                break;
        }

        switch (handle->config.on_flush_complete) {
            case eTBR_ACTION_TRIGGER:
                opctrl |= OPCTRL_TRG_FLSH;
                break;
            case eTBR_ACTION_STOP:
                opctrl |= OPCTRL_STP_FLSH;
                break;
            case eTBR_ACTION_FLUSH:
            case eTBR_ACTION_NONE:
            default:
                break;
        }

        if (handle->config.buffer_mode == eTBR_MODE_ONE_SHOT) {
            opctrl |= OPCTRL_STP_FULL;
        }

        err = TBR_WRITE_OPCTRL(&handle->ta, opctrl);
    }

    if ((e_ERR_NONE == err) && (eTBR_MODE_BRIDGE == handle->config.mode)) {
        uint32_t outlvl = 0;

        /* Configure the OUTLVL register */

        /* OUTLVL
        *
        *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
        *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        *  |                              Reserved         |      NUMBLOCK         |         BLOCKSZ       |
        *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        *
        * OUTLVL::NUMBLOCK[15:8]  - Block count
        * OUTLVL::BLOCKSZ[7:0]    - Block size
        */
        outlvl = (((handle->config.block_count) & 0xFF) << 8) |
                 ((handle->config.block_size) & 0xFF);

        err = TBR_WRITE_OUTLVL(&handle->ta, outlvl);
    }

    if (handle->config.b_formatter) {

        if (e_ERR_NONE == err) {
            /* IDPERIOD
            *
            *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
            *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            *  |                              Reserved                           |            PERIOD           |
            *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            *
            * IDPERIOD::PERIOD[9:0]  - Maximum number of 128-bit frames that will be forwarded without a id change
            */
            uint32_t idperiod = handle->config.src_period;
            err = TBR_WRITE_IDPERIOD(&handle->ta, idperiod);
        }
        if (e_ERR_NONE == err) {
            /* SEQCNTL
            *
            *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
            *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            *  |       Reserved           |     SEQID          |    Reserved     |            PERIOD           |
            *  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            *
            * SEQCNTL::PERIOD[9:0]  - Number of 128-bit frames without inserting incrementing data byte using SEQID
            * SEQCNTL::SEQID[22:16]  - Maximum number of 128-bit frames that will be forwarded without a id change
            */
            uint32_t seqcntl = ((handle->config.seq_id & 0x7F) << 16) | (handle->config.seq_period & 0x3FF);
            err = TBR_WRITE_SEQCNTL(&handle->ta, seqcntl);
        }

    }


    /* Disable IRQ interrupts */
    if (e_ERR_NONE == err) {
        err = TBR_WRITE_IRQENABLE_CLR(&handle->ta, IRQ_AQCMP | IRQ_DAV);
    }


    return err;

}

static td_error_t 
_unlock(
    const tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;

    /* Unlock TBR in order to enable write access to TBR registers */
    // If this bit is set, it device access is locked, we need to unlock the device
    err = TBR_WRITE_LOCKACC(&handle->ta, LOCKACC_UNLOCK_VALUE);

    return err;

}



static td_error_t 
_claim(
    const tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;
    uint32_t claimset = 0, claimclr = 0;

    err = TBR_READ_CLAIMSET(&handle->ta, &claimset);
  
    if (e_ERR_NONE == err) {

        if (0 != (claimset & CLAIM_MASK)) {
            /* Claim set is implemented */
            err = TBR_READ_CLAIMCLR(&handle->ta, &claimclr);
            if (err == e_ERR_NONE) {
                if ((0 != (claimclr & CLAIM_MASK)) && (CLAIM_VAL != (claimclr & CLAIM_MASK))) {
                    err = e_ERR_OWNERSHIP;
                }
                else {
                    err = TBR_WRITE_CLAIMSET(&handle->ta, CLAIM_VAL);
                }
            }

        }
    }
    return err;
}

static td_error_t 
_unclaim(
    const tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;
    uint32_t claimset = 0, claimclr = 0;

    err = TBR_READ_CLAIMSET(&handle->ta, &claimset);

    if (e_ERR_NONE == err) {

        if (0 != (claimset & CLAIM_MASK)) {
            err = TBR_READ_CLAIMCLR(&handle->ta, &claimclr);
            if (err == e_ERR_NONE) {
                if (0 != (claimclr & CLAIM_MASK)) {
                    err = TBR_WRITE_CLAIMCLR(&handle->ta, CLAIM_VAL);
                }
            }

        }
    }
    return err;
}



static td_error_t 
_disable(
    const tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;
    uint32_t ctrl;

    err = TBR_READ_CTRL(&handle->ta, &ctrl);

    if ((e_ERR_NONE == err) && (ctrl & CTRL_ENBL)) {
        bool enabled = true;
        unsigned max_tries = 1000;

        ctrl &= ~CTRL_ENBL;
        err = TBR_WRITE_CTRL(&handle->ta, ctrl);

        while (enabled && (max_tries-- > 0) && (e_ERR_NONE == err)) {
            err = TBR_READ_CTRL(&handle->ta, &ctrl);
            enabled = (0 != (ctrl & CTRL_ENBL));
        }

        if (true == enabled) {
            err = e_ERR_PROGRAM;
        }

        /* If in system bridge mode then disable DMA interrupt */
        if ((eTBR_MODE_BRIDGE == handle->config.mode)) {
            err = TBR_WRITE_IRQENABLE_CLR(&handle->ta, IRQ_DAV);
        }


    }

    return err;
}

static td_error_t
_enable(
    const tbr_handle_t handle
    )
{
    td_error_t err = e_ERR_NONE;
    uint32_t ctrl;
    bool proceed = false;

    /* read the current enable status of the TBR */
    err = TBR_READ_CTRL(&handle->ta, &ctrl);
    if (e_ERR_NONE == err) {
        proceed = ((ctrl & CTRL_ENBL) != CTRL_ENBL);
    }

    /* Perform the steps to enable the TBR */
    if (proceed) {

        /* If in system bridge mode then enable DMA interrupt */
        if ((eTBR_MODE_BRIDGE == handle->config.mode)) {
            err = TBR_WRITE_IRQENABLE_SET(&handle->ta, IRQ_DAV);
        }

        /* Clear all interrupts before enabling trace, just to make sure nothing pending */
        if (e_ERR_NONE == err) {
            err = TBR_WRITE_IRQSTATUS(&handle->ta, IRQ_AQCMP | IRQ_DAV);
        }

        if (e_ERR_NONE == err) {
            unsigned max_tries = 1000;
            bool enabled = false;

            ctrl |= CTRL_ENBL;
            err = TBR_WRITE_CTRL(&handle->ta, ctrl);

            while (!enabled && (max_tries-- > 0) && (e_ERR_NONE == err)) {
                err = TBR_READ_CTRL(&handle->ta, &ctrl);
                enabled = (0 != (ctrl & CTRL_ENBL));
            }

            if (false == enabled) {
                err = e_ERR_PROGRAM;
            }
        }

    }

    return err;
}

static
td_error_t _reset(
    const tbr_handle_t handle
    )
{
    uint32_t ctrl;

    td_error_t err = e_ERR_NONE;
    err = TBR_READ_CTRL(&handle->ta, &ctrl);
    if (err == e_ERR_NONE) {
        ctrl |= CTRL_SRST;
        err = TBR_WRITE_CTRL(&handle->ta, ctrl);
    }

    return err;
}

td_error_t _check_device(const tbr_handle_t handle)
{
    td_error_t  err     = e_ERR_NONE;
    uint32_t devtype = 0;

    /* Check that device identifier is correct */
    err = TBR_READ_DEVTYPE(&handle->ta, &devtype);

    if (e_ERR_NONE == err) {
        if (DEVTYPE != (devtype & 0xFF)) {
            err = e_ERR_BAD_HWTYPE;
        }
        else {
            uint32_t periph0 = 0;
            uint32_t periph1 = 0;

            err = TBR_READ_PERIPHID0(&handle->ta, &periph0);
            if (e_ERR_NONE == err) {
                err = TBR_READ_PERIPHID1(&handle->ta, &periph1);
                if (e_ERR_NONE == err) {
                    uint32_t partno = ((periph1 & 0xF) << 8) | (periph0 & 0xFF);
                    if (PARTNO != partno) {
                        err = e_ERR_BAD_HWTYPE;
                    }
                }
            }
        }
    }

    return err;

}


td_error_t _status(const tbr_handle_t handle, tbr_status_t* p_status, uint32_t *p_available)
{
    td_error_t err = e_ERR_NONE;
    uint32_t cntl = 0;
    uint32_t stat = 0;
    uint32_t opstat = 0;


    err = TBR_READ_CTRL(&handle->ta, &cntl);

    if (e_ERR_NONE == err) {
        err = TBR_READ_STAT(&handle->ta, &stat);
    }

    if (e_ERR_NONE == err) {
        TBR_READ_OPSTAT(&handle->ta, &opstat);
    }

    if (e_ERR_NONE == err) {

        p_status->b_enabled = (0 != (cntl & CTRL_ENBL));
        p_status->b_wrapped = (0 != (stat & STAT_WRAP));
        p_status->b_triggered = (0 != (stat & STAT_TRIG));
        p_status->b_reset_active = (0 != (stat & STAT_RESET_ACT));
        p_status->b_partial_out = (0 != (stat & STAT_PARTIAL_OUT));
        p_status->b_format_done = (0 != (stat & STAT_FMT_DONE));
        p_status->b_drain_done = (0 != (stat & STAT_DRAIN_DONE));
        p_status->b_capture_done = (0 != (stat & STAT_CPT_DONE));

        p_status->b_flush_active = (0 != (opstat & OPSTAT_FLSH_ACT));
        p_status->b_formatter_halted = (0 != (opstat & OPSTAT_FMT_HALTED));


        if (p_available ) {
            uint32_t count = 0;  // # of 32b entries

            if (p_status->b_wrapped) {
                count = handle->depth;
            }
            else {
                uint32_t end = 0;
                uint32_t start = 0;
                err = TBR_READ_RAMWPTR(&handle->ta, &end);
                if (e_ERR_NONE == err) {
                    err = TBR_READ_RAMRPTR(&handle->ta, &start);
                }
                if (e_ERR_NONE == err) {
                    if (end >= start) {
                        count = end - start;
                    }
                    else {
                        count = handle->depth - (start - end);
                    }
                }
            }

            *p_available = count;
        }

    }
    
    return err;
}
