/**
* @file tbr_access.c
* @brief CT-TBR target access implementation.
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
#include <stdbool.h>
#include "td_error.h"
#include "target_access.h"
#include "target_access_direct.h"
#include "tbr_access.h"

typedef struct reg_info_t {
    uint32_t offset;
    const char *name;
} reg_info_t;


static reg_info_t RAMSZ_REG         = { 0x0004, "RAMSZ" };
static reg_info_t FIFOSZ_REG        = { 0x0008, "FIFOSZ" };
static reg_info_t STAT_REG          = { 0x000C, "STAT" };
static reg_info_t RAMRDAT_REG       = { 0x0010, "RAMRDAT" };
static reg_info_t RAMRPTR_REG       = { 0x0014, "RAMRPTR" };
static reg_info_t RAMWPTR_REG       = { 0x0018, "RAMWPTR" };
static reg_info_t TRGCNT_REG        = { 0x001C, "TRGCNT" };
static reg_info_t CTRL_REG          = { 0x0020, "CTRL" };
static reg_info_t RAMWDAT_REG       = { 0x0024, "RAMWDAT" };
static reg_info_t OUTLVL_REG        = { 0x0100, "OUTLVL" };
static reg_info_t SICTRL_REG        = { 0x0104, "SICTRL" };
static reg_info_t IDPERIOD_REG      = { 0x0108, "IDPERIOD" };
static reg_info_t SEQCNTL_REG       = { 0x010C, "SEQCNTL" };
static reg_info_t EOI_REG           = { 0x0120, "EOI" };
static reg_info_t IRQSTATUS_RAW_REG = { 0x0124, "IRQSTATUS_RAW" };
static reg_info_t IRQSTATUS_REG     = { 0x0128, "IRQSTATUS" };
static reg_info_t IRQENABLE_SET_REG = { 0x012C, "IRQENABLE_SET" };
static reg_info_t IRQENABLE_CLR_REG = { 0x0130, "IRQENABLE_CLR" };
static reg_info_t OPSTAT_REG        = { 0x0300, "OPSTAT" };
static reg_info_t OPCTRL_REG        = { 0x0304, "OPCTRL" };
static reg_info_t CLAIMSET_REG      = { 0x0FA0, "CLAIMSET" };
static reg_info_t CLAIMCLR_REG      = { 0x0FA4, "CLAIMCLR" };
static reg_info_t LOCKACC_REG       = { 0x0FB0, "LOCKACC" };
static reg_info_t LOCKSTAT_REG      = { 0x0FB4, "LOCKSTAT" };
static reg_info_t AUTHSTAT_REG      = { 0x0FB8, "AUTHSTAT" };
static reg_info_t DEVID_REG         = { 0x0FC8, "DEVID" };
static reg_info_t DEVTYPE_REG       = { 0x0FCC, "DEVTYPE" };
static reg_info_t PERIPHID0_REG     = { 0x0FE0, "PERIPHID0" };
static reg_info_t PERIPHID1_REG     = { 0x0FE4, "PERIPHID1" };
static reg_info_t PERIPHID2_REG     = { 0x0FE8, "PERIPHID2" };
static reg_info_t PERIPHID3_REG     = { 0x0FEC, "PERIPHID3" };



td_error_t
TBR_READ_RAMSZ(
    const target_access_handle_t   ta_handle,
    uint32_t                      *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, RAMSZ_REG.name, RAMSZ_REG.offset, p_val, MEM);

    return err;
}

td_error_t
TBR_READ_FIFOSZ(
    const target_access_handle_t   ta_handle,
    uint32_t                      *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, FIFOSZ_REG.name, FIFOSZ_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_READ_STAT(
    const target_access_handle_t   ta_handle,
    uint32_t                      *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, STAT_REG.name, STAT_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_RAMRDAT(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, RAMRDAT_REG.name, RAMRDAT_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_RAMRDAT(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, RAMRDAT_REG.name, RAMRDAT_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_RAMRPTR(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;
    
    err = WRITE_32(ta_handle, RAMRPTR_REG.name, RAMRPTR_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_RAMRPTR(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, RAMRPTR_REG.name, RAMRPTR_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_RAMWPTR(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, RAMWPTR_REG.name, RAMWPTR_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_RAMWPTR(
    const target_access_handle_t  ta_handle,
    uint32_t                     *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, RAMWPTR_REG.name, RAMWPTR_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_TRGCNT(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, TRGCNT_REG.name, TRGCNT_REG.offset, &regval, MEM);
    
    return err;
}


td_error_t
TBR_READ_TRGCNT(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
)
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, TRGCNT_REG.name, TRGCNT_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_CTRL(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, CTRL_REG.name, CTRL_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_CTRL(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, CTRL_REG.name, CTRL_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_RAMWDAT(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, RAMWDAT_REG.name, RAMWDAT_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_RAMWDAT(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, RAMWDAT_REG.name, RAMWDAT_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_OUTLVL(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, OUTLVL_REG.name, OUTLVL_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_OUTLVL(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, OUTLVL_REG.name, OUTLVL_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_SICTRL(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, SICTRL_REG.name, SICTRL_REG.offset, &regval, MEM);
    
    return err;
}


td_error_t
TBR_READ_SICTRL(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, SICTRL_REG.name, SICTRL_REG.offset, p_val, MEM);
    
    return err;
}


td_error_t
TBR_WRITE_IDPERIOD(
    const target_access_handle_t   ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, IDPERIOD_REG.name, IDPERIOD_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_IDPERIOD(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, IDPERIOD_REG.name, IDPERIOD_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_SEQCNTL(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, SEQCNTL_REG.name, SEQCNTL_REG.offset, &regval, MEM);
    
    return err;
}


td_error_t
TBR_READ_SEQCNTL(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, SEQCNTL_REG.name, SEQCNTL_REG.offset, p_val, MEM);
    
    return err;
}


td_error_t
TBR_WRITE_EOI(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, EOI_REG.name, EOI_REG.offset, &regval, MEM);
    
    return err;
}


td_error_t
TBR_READ_EOI(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, EOI_REG.name, EOI_REG.offset, p_val, MEM);
    
    return err;
}


td_error_t
TBR_WRITE_IRQSTATUS_RAW(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    // IRQSTATUS register not supported by dedicated drivers
    err = WRITE_32(ta_handle, IRQSTATUS_RAW_REG.name, IRQSTATUS_RAW_REG.offset, &regval, MEM);
    
    return err;
}


td_error_t
TBR_READ_IRQSTATUS_RAW(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    // IRQSTATUS register not supported by dedicated drivers
    err = READ_32(ta_handle, IRQSTATUS_RAW_REG.name, IRQSTATUS_RAW_REG.offset, p_val, MEM);
    
    return err;
}



td_error_t
TBR_WRITE_IRQSTATUS(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    // IRQSTATUS register not supported by dedicated drivers
    err = WRITE_32(ta_handle, IRQSTATUS_REG.name, IRQSTATUS_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_IRQSTATUS(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    // IRQSTATUS register not supported by dedicated drivers
    err = READ_32(ta_handle, IRQSTATUS_REG.name, IRQSTATUS_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_IRQENABLE_SET(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    //. IRQENABLE register not supported by dedicated driver
    err = WRITE_32(ta_handle, IRQENABLE_SET_REG.name, IRQENABLE_SET_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_IRQENABLE_SET(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, IRQENABLE_SET_REG.name, IRQENABLE_SET_REG.offset, p_val, MEM);

    return err;
}



td_error_t
TBR_WRITE_IRQENABLE_CLR(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    // Register is not supported for dedicated tbr adapter
    err = WRITE_32(ta_handle, IRQENABLE_CLR_REG.name, IRQENABLE_CLR_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_IRQENABLE_CLR(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    // Register is not supported for dedicated tbr adapter
    err = READ_32(ta_handle, IRQENABLE_CLR_REG.name, IRQENABLE_CLR_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_READ_OPSTAT(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, OPSTAT_REG.name, OPSTAT_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_WRITE_OPCTRL(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, OPCTRL_REG.name, OPCTRL_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_OPCTRL(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, OPCTRL_REG.name, OPCTRL_REG.offset, p_val, MEM);

    return err;
}



td_error_t
TBR_WRITE_CLAIMSET(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, CLAIMSET_REG.name, CLAIMSET_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_CLAIMSET(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, CLAIMSET_REG.name, CLAIMSET_REG.offset, p_val, MEM);

    return err;
}



td_error_t
TBR_WRITE_CLAIMCLR(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
    )
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, CLAIMCLR_REG.name, CLAIMCLR_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_CLAIMCLR(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, CLAIMCLR_REG.name, CLAIMCLR_REG.offset, p_val, MEM);

    return err;
}

td_error_t
TBR_WRITE_LOCKACC(
    const target_access_handle_t  ta_handle,
    const uint32_t                val
)
{
    uint32_t regval = val;
    td_error_t err = e_ERR_NONE;

    err = WRITE_32(ta_handle, LOCKACC_REG.name, LOCKACC_REG.offset, &regval, MEM);

    return err;
}


td_error_t
TBR_READ_LOCKSTAT(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, LOCKSTAT_REG.name, LOCKSTAT_REG.offset, p_val, MEM);

    return err;
}

td_error_t
TBR_READ_AUTHSTAT(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, AUTHSTAT_REG.name, AUTHSTAT_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_READ_DEVID(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
)
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, DEVID_REG.name, DEVID_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_READ_DEVTYPE(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, DEVTYPE_REG.name, DEVTYPE_REG.offset, p_val, MEM);

    return err;
}


td_error_t
TBR_READ_PERIPHID0(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, PERIPHID0_REG.name, PERIPHID0_REG.offset, p_val, MEM);

    return err;
}

td_error_t
TBR_READ_PERIPHID1(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, PERIPHID1_REG.name, PERIPHID1_REG.offset, p_val, MEM);

    return err;
}

td_error_t
TBR_READ_PERIPHID2(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, PERIPHID2_REG.name, PERIPHID2_REG.offset, p_val, MEM);

    return err;
}

td_error_t
TBR_READ_PERIPHID3(
    const target_access_handle_t   ta_handle,
    uint32_t                *p_val
    )
{
    td_error_t err = e_ERR_NONE;

    err = READ_32(ta_handle, PERIPHID3_REG.name, PERIPHID3_REG.offset, p_val, MEM);

    return err;
}

td_error_t 
TBR_READ_BUFFER(
    const target_access_handle_t ta_handle,
    const uint32_t requested,
    uint32_t *p_buffer,
    unsigned *p_actual
    )
{
    td_error_t err = e_ERR_NONE;
    uint32_t stat = 0;
    uint32_t ramsz = 0;
    uint32_t ramsz_in_words = 0;  // # of 32b entries possible
    uint32_t available = 0;       // # of 32b entries available
    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t actual = 0;
    uint32_t count = requested;


    err = TBR_READ_RAMSZ(ta_handle, &ramsz);
    if (e_ERR_NONE == err) {
        if (ramsz != 0) {
            ramsz_in_words = ((2 << ramsz) * 1024)/4;
        }
    }

    if (e_ERR_NONE == err) {
        // end is the value of the write pointer (RAMWPTR) in the TBR buffer. It indicates the position up to which data has been written.
        err = TBR_READ_RAMWPTR(ta_handle, &end);
    }

    if (e_ERR_NONE == err) {
        // start is the value of the read pointer (RAMRPTR) in the TBR buffer. It indicates the position from which data will be read next.
        err = TBR_READ_RAMRPTR(ta_handle, &start);
    }

    if (e_ERR_NONE == err) {
        err = TBR_READ_STAT(ta_handle, &stat);
    }

    if (e_ERR_NONE == err) {
        bool wrap = (STAT_WRAP == (stat & STAT_WRAP));

        if (wrap) {
            start = end;
            available = ramsz_in_words; // available (# available 32b-words)
        }
        else {
            if (end >= start) {
                available = end - start;
            }
            else {
                // should never hit this but just in case:
                available = ramsz_in_words - (start - end);
            }
		}
    }

    actual = 0;
    if ((e_ERR_NONE == err) && (available > 0)) {

        if (count > available) {
            count = available;
        }

    err = TBR_WRITE_RAMRPTR(ta_handle, start);
    while ((e_ERR_NONE == err) && (actual < count)) {
        err = TBR_READ_RAMRDAT(ta_handle, &p_buffer[actual]);
        actual++;
    }
    }

    if (e_ERR_NONE == err) {
        *p_actual = actual;
    }

    return err;
}
