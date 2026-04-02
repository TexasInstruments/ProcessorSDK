/**
* @file cpt2_access.c
* @brief CPT2 probe target access implementation.
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
#include "cpt2_access.h"

typedef struct reg_info_t {
    uint32_t offset;
    const char *name;
} reg_info_t;

static reg_info_t PROBE_ID = { 0x000, "PROBE_ID" };
static reg_info_t PROBE_OWN = { 0x004, "PROBE_OWN" };
static reg_info_t PROBE_CNTL = { 0x008, "PROBE_CNTL" };
static reg_info_t PROBE_SAMP_PERIOD = { 0x00C, "PROBE_SAMP_PERIOD" };
static reg_info_t PROBE_STATUS = { 0x010, "PROBE_STATUS" };
static reg_info_t FILT_ADDR_LOW_LSB = { 0x020 , "FILT_ADDR_LOW_LSB" };
static reg_info_t FILT_ADDR_LOW_MSB = { 0x024 , "FILT_ADDR_LOW_MSB" };
static reg_info_t FILT_ADDR_HIGH_LSB = { 0x028 , "FILT_ADDR_HIGH_LSB" };
static reg_info_t FILT_ADDR_HIGH_MSB = { 0x02C, "FILT_ADDR_HIGH_MSB" };
static reg_info_t FILT_QUAL0 = { 0x030, "FILT_QUAL0" };
static reg_info_t FILT_QUAL1 = { 0x034, "FILT_QUAL1" };
static reg_info_t FILT_QUAL2 = { 0x038, "FILT_QUAL2" };
static reg_info_t FILT_QUAL3 = { 0x03C, "FILT_QUAL3" };
static reg_info_t FILT_MASK0 = { 0x040, "FILT_MASK0" };
static reg_info_t FILT_MASK1 = { 0x044, "FILT_MASK1" };
static reg_info_t FILT_MASK2 = { 0x048, "FILT_MASK2" };
static reg_info_t FILT_MASK3 = { 0x04C, "FILT_MASK3" };
static reg_info_t THRU_BYTES = { 0x050, "THRU_BYTES" };
static reg_info_t THRU_XCOUNT = { 0x054, "THRU_XCOUNT" };
static reg_info_t LATEN_XCOUNT = { 0x058, "LATEN_XCOUNT" };
static reg_info_t LATEN_TCOUNT = { 0x05C, "LATEN_TCOUNT" };
static reg_info_t LATEN_MAXWAIT = { 0x060, "LATEN_MAXWAIT" };
static reg_info_t LATEN_TOTWAIT = { 0x064, "LATEN_TOTWAIT" };
static reg_info_t LATEN_CREDWAIT = { 0x068, "LATEN_CREDWAIT" };
static reg_info_t STATS_TIME = { 0x06C, "STATS_TIME" };

td_error_t CPT2_READ_PROBE_ID(
    const target_access_handle_t ta_handle,
    uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, PROBE_ID.name, PROBE_ID.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_PROBE_OWN(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, PROBE_OWN.name, PROBE_OWN.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_PROBE_CNTL(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, PROBE_CNTL.name, PROBE_CNTL.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_PROBE_SAMP_PERIOD(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, PROBE_SAMP_PERIOD.name, PROBE_SAMP_PERIOD.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_PROBE_STATUS(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, PROBE_STATUS.name, PROBE_STATUS.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_ADDR_LOW_LSB(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_ADDR_LOW_LSB.name, FILT_ADDR_LOW_LSB.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_ADDR_LOW_MSB(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_ADDR_LOW_MSB.name, FILT_ADDR_LOW_MSB.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_ADDR_HIGH_LSB(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_ADDR_HIGH_LSB.name, FILT_ADDR_HIGH_LSB.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_ADDR_HIGH_MSB(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_ADDR_HIGH_MSB.name, FILT_ADDR_HIGH_MSB.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_QUAL0(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_QUAL0.name, FILT_QUAL0.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_QUAL1(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_QUAL1.name, FILT_QUAL1.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_QUAL2(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_QUAL2.name, FILT_QUAL2.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_QUAL3(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_QUAL3.name, FILT_QUAL3.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_MASK0(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_MASK0.name, FILT_MASK0.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_MASK1(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_MASK1.name, FILT_MASK1.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_MASK2(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_MASK2.name, FILT_MASK2.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_FILT_MASK3(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, FILT_MASK3.name, FILT_MASK3.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_THRU_BYTES(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, THRU_BYTES.name, THRU_BYTES.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_THRU_XCOUNT(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, THRU_XCOUNT.name, THRU_XCOUNT.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_LATEN_XCOUNT(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, LATEN_XCOUNT.name, LATEN_XCOUNT.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_LATEN_TCOUNT(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, LATEN_TCOUNT.name, LATEN_TCOUNT.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_LATEN_MAXWAIT(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, LATEN_MAXWAIT.name, LATEN_MAXWAIT.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_LATEN_TOTWAIT(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, LATEN_TOTWAIT.name, LATEN_TOTWAIT.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_LATEN_CREDWAIT(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, LATEN_CREDWAIT.name, LATEN_CREDWAIT.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_READ_STATS_TIME(
    const target_access_handle_t ta_handle,
    uint32_t *p_val)
{
    td_error_t err = READ_32(ta_handle, STATS_TIME.name, STATS_TIME.offset, p_val, MEM);
    return err;
}

td_error_t CPT2_WRITE_PROBE_OWN(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, PROBE_OWN.name, PROBE_OWN.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_PROBE_CNTL(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, PROBE_CNTL.name, PROBE_CNTL.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_PROBE_SAMP_PERIOD(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, PROBE_SAMP_PERIOD.name, PROBE_SAMP_PERIOD.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_PROBE_STATUS(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, PROBE_STATUS.name, PROBE_STATUS.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_ADDR_LOW_LSB(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_ADDR_LOW_LSB.name, FILT_ADDR_LOW_LSB.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_ADDR_LOW_MSB(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_ADDR_LOW_MSB.name, FILT_ADDR_LOW_MSB.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_ADDR_HIGH_LSB(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_ADDR_HIGH_LSB.name, FILT_ADDR_HIGH_LSB.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_ADDR_HIGH_MSB(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_ADDR_HIGH_MSB.name, FILT_ADDR_HIGH_MSB.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_QUAL0(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_QUAL0.name, FILT_QUAL0.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_QUAL1(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_QUAL1.name, FILT_QUAL1.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_QUAL2(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_QUAL2.name, FILT_QUAL2.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_QUAL3(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_QUAL3.name, FILT_QUAL3.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_MASK0(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_MASK0.name, FILT_MASK0.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_MASK1(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_MASK1.name, FILT_MASK1.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_MASK2(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_MASK2.name, FILT_MASK2.offset, &regval, MEM);
    return err;
}

td_error_t CPT2_WRITE_FILT_MASK3(
    const target_access_handle_t  ta_handle,
    const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, FILT_MASK3.name, FILT_MASK3.offset, &regval, MEM);
    return err;
}

void test(void)
{
}
