/*
 * DM Stub Firmware
 *
 * CDNS DDR driver
 *
 * Copyright (C) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CDNS_DDR_REG_CONFIG_H_
#define _CDNS_DDR_REG_CONFIG_H_

/*
 * -----------------------------------------------------------------------------
 * Common Include files
 * -----------------------------------------------------------------------------
 */
#include <stdint.h>
#include <ddr_functions.h>
#include <cdns_lpddr4_reg_config_3733_svb.h>

/*
 * -----------------------------------------------------------------------------
 * Sub-routine definitions for Cadence register configuration
 * -----------------------------------------------------------------------------
 */
void configure_CTL_registers(struct emif_handle_s *h);
void configure_PI_registers(struct emif_handle_s *h);
void configure_PHY_registers(struct emif_handle_s *h);
void configure_data_slice0_registers(struct emif_handle_s *h);
void configure_data_slice1_registers(struct emif_handle_s *h);
void configure_data_slice2_registers(struct emif_handle_s *h);
void configure_data_slice3_registers(struct emif_handle_s *h);
void configure_address_slice0_registers(struct emif_handle_s *h);
void configure_address_slice1_registers(struct emif_handle_s *h);
void configure_address_slice2_registers(struct emif_handle_s *h);
void configure_ddrphy_registers(struct emif_handle_s *h);
void configure_rd_sample_delays(struct emif_handle_s *h, uint32_t delay);
void configure_rd_dq_delays(struct emif_handle_s *h, uint32_t rd_delay);
void configure_rddqs_gate_delays(struct emif_handle_s *h, uint32_t delay);
void configure_registers_for_IXCOM(struct emif_handle_s *h);
void configure_registers_for_wr_delays(struct emif_handle_s *h, uint32_t wr_delay);
void configure_registers_for_rd_delays(struct emif_handle_s *h, uint32_t rd_delay);
void save_registers(struct emif_handle_s *h);
void restore_registers(struct emif_handle_s *h);
void save_registers_optimized(struct emif_handle_s *h);
void restore_registers_optimized(struct emif_handle_s *h);

#endif
