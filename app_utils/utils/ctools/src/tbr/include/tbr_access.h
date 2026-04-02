/**
* @file tbr_access.h
* @brief CT-TBR register access.
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

#ifndef __TBR_ACCESS_H__
#define __TBR_ACCESS_H__

#define DEVTYPE             0x21
#define PARTNO              0xEDF
#define OPCTRL_FMT_ENBL     (1<<0)
#define OPCTRL_INSRT_TRG    (1<<1)
#define OPCTRL_FLSH_FLSHIN  (1<<4)
#define OPCTRL_FLSH_TRG     (1<<5)
#define OPCTRL_FLUSH        (1<<6)
#define OPCTRL_TRG_TRGIN    (1<<8)
#define OPCTRL_TRG_EVT      (1<<9)
#define OPCTRL_TRG_FLSH     (1<<10)
#define OPCTRL_STP_FLSH     (1<<12)
#define OPCTRL_STP_TRG      (1<<13)
#define OPCTRL_STP_FULL     (1<<15)
#define OPCTRL_OUTFLUSH     (1<<16)
#define OPCTRL_SYNCREQ      (1<<17)

#define STAT_WRAP           (1<<0)
#define STAT_TRIG           (1<<1)
#define STAT_CPT_DONE       (1<<2)
#define STAT_FMT_DONE       (1<<3)
#define STAT_DRAIN_DONE     (1<<4)
#define STAT_PARTIAL_OUT    (1<<5)
#define STAT_RESET_ACT      (1<<6)

#define OPSTAT_FLSH_ACT     (1<<0)
#define OPSTAT_FMT_HALTED   (1<<1)

#define IRQ_DAV             (1<<0)
#define IRQ_AQCMP           (1<<1)

#define CLAIM_VAL  0xF
#define CLAIM_MASK 0xF

#define LOCKSTAT_LOCKIMP    (1<<0)
#define LOCKSTAT_STAT       (1<<1)
#define LOCKACC_UNLOCK_VALUE 0xC5ACCE55


#define CTRL_ENBL           (1<<0)
#define CTRL_MODE           (1<<1)
#define CTRL_SRST           (1<<2)
#define CTRL_SYNCREQ_WRAP   (1<<3)
#define CTRL_IDLE_EMPTY     (1<<4)

#define SICTRL_IDLE_MODE_MASK  (0x3 << 4)
#define SICTRL_FORCE_IDLE      (0x0 << 4)
#define SICTRL_NO_IDLE         (0x1 << 4)
#define SICTRL_SMART_IDLE      (0x2 << 4)
#define SICTRL_SMART_IDLE_WKUP (0x3 << 4)

#define SICTRL_DATA_WIDTH_32   (0<<0)
#define SICTRL_DATA_WIDTH_64   (1<<0)

#include "td_error.h"
#include "target_access.h"

#ifdef __cplusplus
extern "C" {
#endif

    td_error_t TBR_READ_RAMSZ(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_FIFOSZ(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_STAT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_RAMRDAT(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_RAMRDAT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_RAMRPTR(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_RAMRPTR(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_RAMWPTR(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_RAMWPTR(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_TRGCNT(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_TRGCNT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_CTRL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_CTRL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_RAMWDAT(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_RAMWDAT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_OUTLVL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_OUTLVL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_SICTRL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_SICTRL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_IDPERIOD(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_IDPERIOD(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_SEQCNTL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_SEQCNTL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_EOI(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_EOI(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_IRQSTATUS_RAW(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_IRQSTATUS_RAW(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_IRQSTATUS(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_IRQSTATUS(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_IRQENABLE_SET(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_IRQENABLE_SET(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_IRQENABLE_CLR(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_IRQENABLE_CLR(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_OPSTAT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_OPCTRL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_OPCTRL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_CLAIMSET(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_CLAIMSET(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_CLAIMCLR(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_CLAIMCLR(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_WRITE_LOCKACC(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t TBR_READ_LOCKSTAT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_AUTHSTAT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_DEVID(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_DEVTYPE(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_PERIPHID0(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_PERIPHID1(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_PERIPHID2(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t TBR_READ_PERIPHID3(const target_access_handle_t ta_handle, uint32_t *p_val);

    td_error_t TBR_READ_BUFFER(const target_access_handle_t ta_handle, const uint32_t count, uint32_t *p_buffer, unsigned *p_actual);

#ifdef __cplusplus
}
#endif

#endif // __TBR_ACCESS_H__
