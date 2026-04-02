/**
* @file cpt2_access.h
* @brief CPTracer2 bus probe register access.
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

#ifndef __CPT2_ACCESS_H__
#define __CPT2_ACCESS_H__

#include "td_error.h"
#include "target_access.h"

#define CPT2_PROBE_ID_FUNCTION_BITS                 (0xFFF << 16)
#define CPT2_PROBE_ID_FUNCTION                      (0x282 << 16)
#define CPT2_PROBE_OWN_CLAIM_BIT                    (0x001 << 0)
#define CPT2_PROBE_OWN_BITS                         (0x003 << 1)
#define CPT2_PROBE_OWN_FREE                         (0x000 << 0)
#define CPT2_PROBE_OWN_DBG                          (0x001 << 1)
#define CPT2_PROBE_OWN_APP                          (0x001 << 2)
#define CPT2_PROBE_CTRL_EN                          (0x001 << 0)
#define CPT2_PROBE_CTRL_OP_BITS                     (0x003 << 1)
#define CPT2_PROBE_CTRL_OP_LAT                      (0x000 << 1)
#define CPT2_PROBE_CTRL_OP_THROUGH                  (0x001 << 1)
#define CPT2_PROBE_CTRL_OP_TRANS                    (0x002 << 1)
#define CPT2_PROBE_CTRL_TRACE_EN                    (0x001 << 3)
#define CPT2_PROBE_CTRL_BUS_OFFSET                  (8)
#define CPT2_PROBE_CTRL_BUS_BITS                    (0x007 << CPT2_PROBE_CTRL_BUS_OFFSET)
#define CPT2_PROBE_CTRL_BUS_VBUSP                   (0x000 << CPT2_PROBE_CTRL_BUS_OFFSET)
#define CPT2_PROBE_CTRL_BUS_VBUSM                   (0x001 << CPT2_PROBE_CTRL_BUS_OFFSET)
#define CPT2_PROBE_CTRL_BUS_VBUSMC                  (0x003 << CPT2_PROBE_CTRL_BUS_OFFSET)
#define CPT2_PROBE_CTRL_MST_ID_OFFSET               (12)
#define CPT2_PROBE_CTRL_MST_ID_BITS                 (0x007 << CPT2_PROBE_CTRL_MST_ID_OFFSET)
#define CPT2_PROBE_SAMP_PERIOD_MIN                  (0x00001000)
#define CPT2_PROBE_SAMP_PERIOD_MAX                  (0x00FFFFFF)
#define CPT2_PROBE_SAMP_PERIOD_BITS                 (0x00003FFF)
#define CPT2_PROBE_EN_STAT                          (0x001 << 0)
#define CPT2_PROBE_FILT_ADDR_LOW_RANGE_EXCLUDE      (0x001 << 31)

#define CPT2_PROBE_FILT0_CHANID_7_5_OFFSET          (29)
#define CPT2_PROBE_FILT0_PABLE_OFFSET               (28)
#define CPT2_PROBE_FILT0_CHANID_4_2_OFFSET          (25)
#define CPT2_PROBE_FILT0_CMSIDEBANDEXT_OFFSET       (24)
#define CPT2_PROBE_FILT0_CHANID_1_0_OFFSET          (22)
#define CPT2_PROBE_FILT0_CSIDEBANDEXT_OFFSET        (20)
#define CPT2_PROBE_FILT0_ORDERID_OFFSET             (16)
#define CPT2_PROBE_FILT0_EXCL_OFFSET                (15)
#define CPT2_PROBE_FILT0_DIR_OFFSET                 (14)
#define CPT2_PROBE_FILT0_AMODE_OFFSET               (12)
#define CPT2_PROBE_FILT0_ROUTEID_OFFSET             (0)

#define CPT2_PROBE_FILT0_CHANID_7_5_SHIFT           (24)
#define CPT2_PROBE_FILT0_CHANID_7_5_BITS            (0x007 << CPT2_PROBE_FILT0_CHANID_7_5_OFFSET)
#define CPT2_PROBE_FILT0_PABLE_BIT                  (0x001 << CPT2_PROBE_FILT0_PABLE_OFFSET)
#define CPT2_PROBE_FILT0_CHANID_4_2_SHIFT           (23)
#define CPT2_PROBE_FILT0_CHANID_4_2_BITS            (0x007 << CPT2_PROBE_FILT0_CHANID_4_2_OFFSET)
#define CPT2_PROBE_FILT0_CMSIDEBANDEXT_SHIFT        (19)
#define CPT2_PROBE_FILT0_CMSIDEBANDEXT_BIT          (0x001 << CPT2_PROBE_FILT0_CMSIDEBANDEXT_OFFSET)
#define CPT2_PROBE_FILT0_CHANID_1_0_SHIFT           (22)
#define CPT2_PROBE_FILT0_CHANID_1_0_BITS            (0x003 << CPT2_PROBE_FILT0_CHANID_1_0_OFFSET)
#define CPT2_PROBE_FILT0_CSIDEBANDEXT_SHIFT         (14)
#define CPT2_PROBE_FILT0_CSIDEBANDEXT_BITS          (0x003 << CPT2_PROBE_FILT0_CSIDEBANDEXT_OFFSET)
#define CPT2_PROBE_FILT0_ORDERID_BITS               (0x00F << CPT2_PROBE_FILT0_ORDERID_OFFSET)
#define CPT2_PROBE_FILT0_EXCL_BIT                   (0x001 << CPT2_PROBE_FILT0_EXCL_OFFSET)
#define CPT2_PROBE_FILT0_DIR_BIT                    (0x001 << CPT2_PROBE_FILT0_DIR_OFFSET)
#define CPT2_PROBE_FILT0_AMODE_BITS                 (0x003 << CPT2_PROBE_FILT0_AMODE_OFFSET)
#define CPT2_PROBE_FILT0_ROUTEID_BITS               (0xFFF << CPT2_PROBE_FILT0_ROUTEID_OFFSET)

#define CPT2_PROBE_FILT1_RSEL_OFFSET                (28)
#define CPT2_PROBE_FILT1_DTYPE_OFFSET               (26)
#define CPT2_PROBE_FILT1_CHANID_11_8_OFFSET         (21)
#define CPT2_PROBE_FILT1_PRIV_OFFSET                (19)
#define CPT2_PROBE_FILT1_CLSIZE_OFFSET              (16)
#define CPT2_PROBE_FILT1_EPRIORITY_OFFSET           (13)
#define CPT2_PROBE_FILT1_PRIORITY_OFFSET            (10)
#define CPT2_PROBE_FILT1_BYTECOUNT_OFFSET           (0)

#define CPT2_PROBE_FILT1_RSEL_BITS                  (0x00F << CPT2_PROBE_FILT1_RSEL_OFFSET)
#define CPT2_PROBE_FILT1_DTYPE_BITS                 (0x003 << CPT2_PROBE_FILT1_DTYPE_OFFSET)
#define CPT2_PROBE_FILT1_CHANID_11_8_SHIFT          (13)
#define CPT2_PROBE_FILT1_CHANID_11_8_BITS           (0x00F << CPT2_PROBE_FILT1_CHANID_11_8_OFFSET)
#define CPT2_PROBE_FILT1_PRIV_BITS                  (0x003 << CPT2_PROBE_FILT1_PRIV_OFFSET)
#define CPT2_PROBE_FILT1_CLSIZE_BITS                (0x007 << CPT2_PROBE_FILT1_CLSIZE_OFFSET)
#define CPT2_PROBE_FILT1_EPRIORITY_BITS             (0x007 << CPT2_PROBE_FILT1_EPRIORITY_OFFSET)
#define CPT2_PROBE_FILT1_PRIORITY_BITS              (0x007 << CPT2_PROBE_FILT1_PRIORITY_OFFSET)
#define CPT2_PROBE_FILT1_BYTECOUNT_BITS             (0x3FF << CPT2_PROBE_FILT1_BYTECOUNT_OFFSET)

#define CPT2_PROBE_FILT2_CMSIDEBAND_OFFSET          (26)
#define CPT2_PROBE_FILT2_CSIDEBAND_OFFSET           (20)
#define CPT2_PROBE_FILT2_OPCODE_OFFSET              (14)
#define CPT2_PROBE_FILT2_MEMTYPE_OFFSET             (12)
#define CPT2_PROBE_FILT2_SECURE_OFFSET              (11)
#define CPT2_PROBE_FILT2_QOS_OFFSET                 (8)
#define CPT2_PROBE_FILT2_SDOMAIN_OFFSET             (6)
#define CPT2_PROBE_FILT2_OUTER_OFFSET               (4)
#define CPT2_PROBE_FILT2_INNER_OFFSET               (2)
#define CPT2_PROBE_FILT2_EMUDBG_OFFSET              (1)
#define CPT2_PROBE_FILT2_INTEREST_OFFSET            (0)

#define CPT2_PROBE_FILT2_CMSIDEBAND_BITS            (0x03F << CPT2_PROBE_FILT2_CMSIDEBAND_OFFSET)
#define CPT2_PROBE_FILT2_CMSIDEBAND_SHIFT           (27)
#define CPT2_PROBE_FILT2_CSIDEBAND_BITS             (0x03F << CPT2_PROBE_FILT2_CSIDEBAND_OFFSET)
#define CPT2_PROBE_FILT2_CSIDEBAND_SHIFT            (20)
#define CPT2_PROBE_FILT2_OPCODE_BITS                (0x03F << CPT2_PROBE_FILT2_OPCODE_OFFSET)
#define CPT2_PROBE_FILT2_MEMTYPE_BITS               (0x003 << CPT2_PROBE_FILT2_MEMTYPE_OFFSET)
#define CPT2_PROBE_FILT2_SECURE_BIT                 (0x001 << CPT2_PROBE_FILT2_SECURE_OFFSET)
#define CPT2_PROBE_FILT2_QOS_BITS                   (0x007 << CPT2_PROBE_FILT2_QOS_OFFSET)
#define CPT2_PROBE_FILT2_SDOMAIN_BITS               (0x003 << CPT2_PROBE_FILT2_SDOMAIN_OFFSET)
#define CPT2_PROBE_FILT2_OUTER_BITS                 (0x003 << CPT2_PROBE_FILT2_OUTER_OFFSET)
#define CPT2_PROBE_FILT2_INNER_BITS                 (0x003 << CPT2_PROBE_FILT2_INNER_OFFSET)
#define CPT2_PROBE_FILT2_EMUDBG_BIT                 (0x001 << CPT2_PROBE_FILT2_EMUDBG_OFFSET)
#define CPT2_PROBE_FILT2_INTEREST_BIT               (0x001 << CPT2_PROBE_FILT2_INTEREST_OFFSET)

#define CPT2_PROBE_FILT3_FLUSH_OFFSET               (28)
#define CPT2_PROBE_FILT3_FWPASS_OFFSET              (26)
#define CPT2_PROBE_FILT3_ASEL_OFFSET                (22)
#define CPT2_PROBE_FILT3_ATYPE_OFFSET               (20)
#define CPT2_PROBE_FILT3_PRIVID_OFFSET              (12)
#define CPT2_PROBE_FILT3_VIRTID_OFFSET              (0)

#define CPT2_PROBE_FILT3_FLUSH_BIT                  (0x001 << CPT2_PROBE_FILT3_FLUSH_OFFSET)
#define CPT2_PROBE_FILT3_FWPASS_BITS                (0x003 << CPT2_PROBE_FILT3_FWPASS_OFFSET)
#define CPT2_PROBE_FILT3_ASEL_BITS                  (0x00F << CPT2_PROBE_FILT3_ASEL_OFFSET)
#define CPT2_PROBE_FILT3_ATYPE_BITS                 (0x003 << CPT2_PROBE_FILT3_ATYPE_OFFSET)
#define CPT2_PROBE_FILT3_PRIVID_BITS                (0x0FF << CPT2_PROBE_FILT3_PRIVID_OFFSET)
#define CPT2_PROBE_FILT3_VIRTID_BITS                (0xFFF << CPT2_PROBE_FILT3_VIRTID_OFFSET)

#define CPT2_PROBE_FILTMASK0_CHANID_7_5_OFFSET      (29)
#define CPT2_PROBE_FILTMASK0_PABLE_OFFSET           (28)
#define CPT2_PROBE_FILTMASK0_CHANID_4_2_OFFSET      (25)
#define CPT2_PROBE_FILTMASK0_CMSIDEBANDEXT_OFFSET   (24)
#define CPT2_PROBE_FILTMASK0_CHANID_1_0_OFFSET      (22)
#define CPT2_PROBE_FILTMASK0_CSIDEBANDEXT_OFFSET    (20)
#define CPT2_PROBE_FILTMASK0_ORDERID_OFFSET         (16)
#define CPT2_PROBE_FILTMASK0_EXCL_OFFSET            (15)
#define CPT2_PROBE_FILTMASK0_DIR_OFFSET             (14)
#define CPT2_PROBE_FILTMASK0_AMODE_OFFSET           (12)
#define CPT2_PROBE_FILTMASK0_ROUTEID_OFFSET         (0)

#define CPT2_PROBE_FILTMASK0_CHANID_7_5_SHIFT       (24)
#define CPT2_PROBE_FILTMASK0_CHANID_7_5_BITS        (0x007 << CPT2_PROBE_FILTMASK0_CHANID_7_5_OFFSET)
#define CPT2_PROBE_FILTMASK0_PABLE_BIT              (0x001 << CPT2_PROBE_FILTMASK0_PABLE_OFFSET)
#define CPT2_PROBE_FILTMASK0_CHANID_4_2_SHIFT       (23)
#define CPT2_PROBE_FILTMASK0_CHANID_4_2_BITS        (0x007 << CPT2_PROBE_FILTMASK0_CHANID_4_2_OFFSET)
#define CPT2_PROBE_FILTMASK0_CMSIDEBANDEXT_SHIFT    (19)
#define CPT2_PROBE_FILTMASK0_CMSIDEBANDEXT_BIT      (0x001 << CPT2_PROBE_FILTMASK0_CMSIDEBANDEXT_OFFSET)
#define CPT2_PROBE_FILTMASK0_CHANID_1_0_SHIFT       (22)
#define CPT2_PROBE_FILTMASK0_CHANID_1_0_BITS        (0x003 << CPT2_PROBE_FILTMASK0_CHANID_1_0_OFFSET)
#define CPT2_PROBE_FILTMASK0_CSIDEBANDEXT_SHIFT     (14)
#define CPT2_PROBE_FILTMASK0_CSIDEBANDEXT_BITS      (0x003 << CPT2_PROBE_FILTMASK0_CSIDEBANDEXT_OFFSET)
#define CPT2_PROBE_FILTMASK0_ORDERID_BITS           (0x00F << CPT2_PROBE_FILTMASK0_ORDERID_OFFSET)
#define CPT2_PROBE_FILTMASK0_EXCL_BIT               (0x001 << CPT2_PROBE_FILTMASK0_EXCL_OFFSET)
#define CPT2_PROBE_FILTMASK0_DIR_BIT                (0x001 << CPT2_PROBE_FILTMASK0_DIR_OFFSET)
#define CPT2_PROBE_FILTMASK0_AMODE_BITS             (0x003 << CPT2_PROBE_FILTMASK0_AMODE_OFFSET)
#define CPT2_PROBE_FILTMASK0_ROUTEID_BITS           (0xFFF << CPT2_PROBE_FILTMASK0_ROUTEID_OFFSET)

#define CPT2_PROBE_FILTMASK1_RSEL_OFFSET            (28)
#define CPT2_PROBE_FILTMASK1_DTYPE_OFFSET           (26)
#define CPT2_PROBE_FILTMASK1_CHANID_11_8_OFFSET     (21)
#define CPT2_PROBE_FILTMASK1_PRIV_OFFSET            (19)
#define CPT2_PROBE_FILTMASK1_CLSIZE_OFFSET          (16)
#define CPT2_PROBE_FILTMASK1_EPRIORITY_OFFSET       (13)
#define CPT2_PROBE_FILTMASK1_PRIORITY_OFFSET        (10)
#define CPT2_PROBE_FILTMASK1_BYTECOUNT_OFFSET       (0)

#define CPT2_PROBE_FILTMASK1_RSEL_BITS              (0x00F << CPT2_PROBE_FILTMASK1_RSEL_OFFSET)
#define CPT2_PROBE_FILTMASK1_DTYPE_BITS             (0x003 << CPT2_PROBE_FILTMASK1_DTYPE_OFFSET)
#define CPT2_PROBE_FILTMASK1_CHANID_11_8_SHIFT      (13)
#define CPT2_PROBE_FILTMASK1_CHANID_11_8_BITS       (0x00F << CPT2_PROBE_FILTMASK1_CHANID_11_8_OFFSET)
#define CPT2_PROBE_FILTMASK1_PRIV_BITS              (0x003 << CPT2_PROBE_FILTMASK1_PRIV_OFFSET)
#define CPT2_PROBE_FILTMASK1_CLSIZE_BITS            (0x007 << CPT2_PROBE_FILTMASK1_CLSIZE_OFFSET)
#define CPT2_PROBE_FILTMASK1_EPRIORITY_BITS         (0x007 << CPT2_PROBE_FILTMASK1_EPRIORITY_OFFSET)
#define CPT2_PROBE_FILTMASK1_PRIORITY_BITS          (0x007 << CPT2_PROBE_FILTMASK1_PRIORITY_OFFSET)
#define CPT2_PROBE_FILTMASK1_BYTECOUNT_BITS         (0x3FF << CPT2_PROBE_FILTMASK1_BYTECOUNT_OFFSET)

#define CPT2_PROBE_FILTMASK2_CMSIDEBAND_OFFSET      (26)
#define CPT2_PROBE_FILTMASK2_CSIDEBAND_OFFSET       (20)
#define CPT2_PROBE_FILTMASK2_OPCODE_OFFSET          (14)
#define CPT2_PROBE_FILTMASK2_MEMTYPE_OFFSET         (12)
#define CPT2_PROBE_FILTMASK2_SECURE_OFFSET          (11)
#define CPT2_PROBE_FILTMASK2_QOS_OFFSET             (8)
#define CPT2_PROBE_FILTMASK2_SDOMAIN_OFFSET         (6)
#define CPT2_PROBE_FILTMASK2_OUTER_OFFSET           (4)
#define CPT2_PROBE_FILTMASK2_INNER_OFFSET           (2)
#define CPT2_PROBE_FILTMASK2_EMUDBG_OFFSET          (1)
#define CPT2_PROBE_FILTMASK2_INTEREST_OFFSET        (0)

#define CPT2_PROBE_FILTMASK2_CMSIDEBAND_BITS        (0x03F << CPT2_PROBE_FILTMASK2_CMSIDEBAND_OFFSET)
#define CPT2_PROBE_FILTMASK2_CMSIDEBAND_SHIFT       (27)
#define CPT2_PROBE_FILTMASK2_CSIDEBAND_BITS         (0x03F << CPT2_PROBE_FILTMASK2_CSIDEBAND_OFFSET)
#define CPT2_PROBE_FILTMASK2_CSIDEBAND_SHIFT        (20)
#define CPT2_PROBE_FILTMASK2_OPCODE_BITS            (0x03F << CPT2_PROBE_FILTMASK2_OPCODE_OFFSET)
#define CPT2_PROBE_FILTMASK2_MEMTYPE_BITS           (0x003 << CPT2_PROBE_FILTMASK2_MEMTYPE_OFFSET)
#define CPT2_PROBE_FILTMASK2_SECURE_BIT             (0x001 << CPT2_PROBE_FILTMASK2_SECURE_OFFSET)
#define CPT2_PROBE_FILTMASK2_QOS_BITS               (0x007 << CPT2_PROBE_FILTMASK2_QOS_OFFSET)
#define CPT2_PROBE_FILTMASK2_SDOMAIN_BITS           (0x003 << CPT2_PROBE_FILTMASK2_SDOMAIN_OFFSET)
#define CPT2_PROBE_FILTMASK2_OUTER_BITS             (0x003 << CPT2_PROBE_FILTMASK2_OUTER_OFFSET)
#define CPT2_PROBE_FILTMASK2_INNER_BITS             (0x003 << CPT2_PROBE_FILTMASK2_INNER_OFFSET)
#define CPT2_PROBE_FILTMASK2_EMUDBG_BIT             (0x001 << CPT2_PROBE_FILTMASK2_EMUDBG_OFFSET)
#define CPT2_PROBE_FILTMASK2_INTEREST_BIT           (0x001 << CPT2_PROBE_FILTMASK2_INTEREST_OFFSET)

#define CPT2_PROBE_FILTMASK3_FLUSH_OFFSET           (28)
#define CPT2_PROBE_FILTMASK3_FWPASS_OFFSET          (26)
#define CPT2_PROBE_FILTMASK3_ASEL_OFFSET            (22)
#define CPT2_PROBE_FILTMASK3_ATYPE_OFFSET           (20)
#define CPT2_PROBE_FILTMASK3_PRIVID_OFFSET          (12)
#define CPT2_PROBE_FILTMASK3_VIRTID_OFFSET          (0)

#define CPT2_PROBE_FILTMASK3_FLUSH_BIT              (0x001 << CPT2_PROBE_FILTMASK3_FLUSH_OFFSET)
#define CPT2_PROBE_FILTMASK3_FWPASS_BITS            (0x003 << CPT2_PROBE_FILTMASK3_FWPASS_OFFSET)
#define CPT2_PROBE_FILTMASK3_ASEL_BITS              (0x00F << CPT2_PROBE_FILTMASK3_ASEL_OFFSET)
#define CPT2_PROBE_FILTMASK3_ATYPE_BITS             (0x003 << CPT2_PROBE_FILTMASK3_ATYPE_OFFSET)
#define CPT2_PROBE_FILTMASK3_PRIVID_BITS            (0x0FF << CPT2_PROBE_FILTMASK3_PRIVID_OFFSET)
#define CPT2_PROBE_FILTMASK3_VIRTID_BITS            (0xFFF << CPT2_PROBE_FILTMASK3_VIRTID_OFFSET)

// If these max values get updated, update the doxygen comments in cpt2.h
// Also update the range allowed in CCS specified by the AETT_property_*_system_trace_*.xml file.
#define CPT2_PROBE_CHANNEL_ID_MAX                   (0xFFF)
#define CPT2_PROBE_CMSIDEBAND_MAX                   (0x03F)
#define CPT2_PROBE_CSIDEBAND_MAX                    (0x0FF)
#define CPT2_PROBE_ORDERID_MAX                      (0x00F)
#define CPT2_PROBE_AMODE_MAX                        (0x003)
#define CPT2_PROBE_ROUTEID_MAX                      (0xFFF)
#define CPT2_PROBE_RSEL_MAX                         (0x00F)
#define CPT2_PROBE_DTYPE_MAX                        (0x003)
#define CPT2_PROBE_PRIV_MAX                         (0x003)
#define CPT2_PROBE_CLSIZE_MAX                       (0x007)
#define CPT2_PROBE_EPRIORITY_MAX                    (0x007)
#define CPT2_PROBE_PRIORITY_MAX                     (0x007)
#define CPT2_PROBE_BYTECOUNT_MAX                    (0x3FF)
#define CPT2_PROBE_OPCODE_MAX                       (0x03F)
#define CPT2_PROBE_MEMTYPE_MAX                      (0x003)
#define CPT2_PROBE_QOS_MAX                          (0x007)
#define CPT2_PROBE_SDOMAIN_MAX                      (0x003)
#define CPT2_PROBE_OUTER_MAX                        (0x003)
#define CPT2_PROBE_INNER_MAX                        (0x003)
#define CPT2_PROBE_FWPASS_MAX                       (0x003)
#define CPT2_PROBE_ASEL_MAX                         (0x00F)
#define CPT2_PROBE_ATYPE_MAX                        (0x003)
#define CPT2_PROBE_PRIVID_MAX                       (0x0FF)
#define CPT2_PROBE_VIRTID_MAX                       (0xFFF)

#ifdef __cplusplus
extern "C" {
#endif

    td_error_t CPT2_READ_PROBE_ID(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_PROBE_OWN(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_PROBE_CNTL(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_PROBE_SAMP_PERIOD(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_PROBE_STATUS(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_ADDR_LOW_LSB(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_ADDR_LOW_MSB(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_ADDR_HIGH_LSB(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_ADDR_HIGH_MSB(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_QUAL0(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_QUAL1(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_QUAL2(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_QUAL3(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_MASK0(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_MASK1(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_MASK2(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_FILT_MASK3(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_THRU_BYTES(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_THRU_XCOUNT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_LATEN_XCOUNT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_LATEN_TCOUNT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_LATEN_MAXWAIT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_LATEN_TOTWAIT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_LATEN_CREDWAIT(const target_access_handle_t ta_handle, uint32_t *p_val);
    td_error_t CPT2_READ_STATS_TIME(const target_access_handle_t ta_handle, uint32_t *p_val);

    td_error_t CPT2_WRITE_PROBE_OWN(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_PROBE_CNTL(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_PROBE_SAMP_PERIOD(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_PROBE_STATUS(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_ADDR_LOW_LSB(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_ADDR_LOW_MSB(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_ADDR_HIGH_LSB(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_ADDR_HIGH_MSB(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_QUAL0(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_QUAL1(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_QUAL2(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_QUAL3(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_MASK0(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_MASK1(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_MASK2(const target_access_handle_t ta_handle, const uint32_t val);
    td_error_t CPT2_WRITE_FILT_MASK3(const target_access_handle_t ta_handle, const uint32_t val);

#ifdef __cplusplus
}
#endif

#endif // __CPT2_ACCESS_H
