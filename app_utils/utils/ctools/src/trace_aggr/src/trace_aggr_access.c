/**
* @file trace_aggr_access.c
* @brief CPTracer2 aggregator target access definitions.
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
#include "trace_aggr_access.h"

typedef struct reg_info_t {
    uint32_t offset;
    const char *name;
} reg_info_t;

static reg_info_t AGGREGATOR_ID =       { 0x000, "AGGREGATOR_ID" };     //RO
static reg_info_t AGGREGATOR_CNTL =     { 0x004, "AGGREGATOR_CNTL" };   //RW
static reg_info_t AGGREGATOR_OWN  =     { 0x008, "AGGREGATOR_OWN" };   //RW
static reg_info_t CONT_READ_PORT0 =     { 0x020, "CONT_READ_PORT0" };   //RW
static reg_info_t CONT_READ_ADDR0 =     { 0x024, "CONT_READ_ADDR0" };   //RW
static reg_info_t CONT_READ_DATA0 =     { 0x028, "CONT_READ_DATA0" };   //RW
static reg_info_t CONT_READ_PORT1 =     { 0x030 , "CONT_READ_PORT1" };  //RW
static reg_info_t CONT_READ_ADDR1 =     { 0x034 , "CONT_READ_ADDR1" };  //RW
static reg_info_t CONT_READ_DATA1 =     { 0x038 , "CONT_READ_DATA1" };  //RW
static reg_info_t CONT_READ_PORT2 =     { 0x040 , "CONT_READ_PORT2" };  //RW
static reg_info_t CONT_READ_ADDR2 =     { 0x044 , "CONT_READ_ADDR2" };  //RW
static reg_info_t CONT_READ_DATA2 =     { 0x048 , "CONT_READ_DATA2" };  //RW
static reg_info_t CONT_READ_PORT3 =     { 0x050 , "CONT_READ_PORT3" };  //RW
static reg_info_t CONT_READ_ADDR3 =     { 0x054 , "CONT_READ_ADDR3" };  //RW
static reg_info_t CONT_READ_DATA3 =     { 0x058 , "CONT_READ_DATA3" };  //RW
static reg_info_t STP_TRACE_CONTROL =   { 0x100, "STP_TRACE_CONTROL" }; //RW
static reg_info_t STP_TRACE_ID =        { 0x104, "STP_TRACE_ID" };      //RW
static reg_info_t STP_SYNC_CONTROL =    { 0x110, "STP_SYNC_CONTROL" };  //RW
static reg_info_t STP_FLUSH_CONTROL =   { 0x114, "STP_FLUSH_CONTROL" }; //RW
static reg_info_t STP_FEATURES =        { 0x118, "STP_FEATURES" };      //RO


td_error_t TRACE_AGGR_READ_AGGREGATOR_ID(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, AGGREGATOR_ID.name, AGGREGATOR_ID.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_AGGREGATOR_CNTL(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, AGGREGATOR_CNTL.name, AGGREGATOR_CNTL.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_AGGREGATOR_OWN(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, AGGREGATOR_OWN.name, AGGREGATOR_OWN.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_PORT0(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_PORT0.name, CONT_READ_PORT0.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_ADDR0(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_ADDR0.name, CONT_READ_ADDR0.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_DATA0(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_DATA0.name, CONT_READ_DATA0.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_PORT1(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_PORT1.name, CONT_READ_PORT1.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_ADDR1(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_ADDR1.name, CONT_READ_ADDR1.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_DATA1(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_DATA1.name, CONT_READ_DATA1.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_PORT2(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_PORT2.name, CONT_READ_PORT2.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_ADDR2(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_ADDR2.name, CONT_READ_ADDR2.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_DATA2(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_DATA2.name, CONT_READ_DATA2.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_PORT3(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_PORT3.name, CONT_READ_PORT3.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_ADDR3(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_ADDR3.name, CONT_READ_ADDR3.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_CONT_READ_DATA3(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, CONT_READ_DATA3.name, CONT_READ_DATA3.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_STP_TRACE_CONTROL(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, STP_TRACE_CONTROL.name, STP_TRACE_CONTROL.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_STP_TRACE_ID(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, STP_TRACE_ID.name, STP_TRACE_ID.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_STP_SYNC_CONTROL(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, STP_SYNC_CONTROL.name, STP_SYNC_CONTROL.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_STP_FLUSH_CONTROL(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, STP_FLUSH_CONTROL.name, STP_FLUSH_CONTROL.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_READ_STP_FEATURES(const target_access_handle_t ta_handle, uint32_t * p_val)
{
    td_error_t err = READ_32(ta_handle, STP_FEATURES.name, STP_FEATURES.offset, p_val, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_AGGREGATOR_CNTL(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, AGGREGATOR_CNTL.name, AGGREGATOR_CNTL.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_AGGREGATOR_OWN(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, AGGREGATOR_OWN.name, AGGREGATOR_OWN.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT0(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_PORT0.name, CONT_READ_PORT0.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR0(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_ADDR0.name, CONT_READ_ADDR0.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA0(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_DATA0.name, CONT_READ_DATA0.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT1(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_PORT1.name, CONT_READ_PORT1.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR1(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_ADDR1.name, CONT_READ_ADDR1.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA1(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_DATA1.name, CONT_READ_DATA1.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT2(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_PORT2.name, CONT_READ_PORT2.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR2(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_ADDR2.name, CONT_READ_ADDR2.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA2(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_DATA2.name, CONT_READ_DATA2.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_PORT3(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_PORT3.name, CONT_READ_PORT3.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_ADDR3(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_ADDR3.name, CONT_READ_ADDR3.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_CONT_READ_DATA3(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, CONT_READ_DATA3.name, CONT_READ_DATA3.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_STP_TRACE_CONTROL(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, STP_TRACE_CONTROL.name, STP_TRACE_CONTROL.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_STP_TRACE_ID(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, STP_TRACE_ID.name, STP_TRACE_ID.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_STP_SYNC_CONTROL(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, STP_SYNC_CONTROL.name, STP_SYNC_CONTROL.offset, &regval, MEM);
    return err;
}

td_error_t TRACE_AGGR_WRITE_STP_FLUSH_CONTROL(const target_access_handle_t ta_handle, const uint32_t val)
{
    uint32_t regval = val;
    td_error_t err = WRITE_32(ta_handle, STP_FLUSH_CONTROL.name, STP_FLUSH_CONTROL.offset, &regval, MEM);
    return err;
}
