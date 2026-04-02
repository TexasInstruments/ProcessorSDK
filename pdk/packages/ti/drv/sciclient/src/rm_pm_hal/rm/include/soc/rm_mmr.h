/*
 * DMSC RM Driver MMR Definition
 *
 * Resource Manager internal driver's MMR structure definition
 *
 * Copyright (C) 2017-2020, Texas Instruments Incorporated
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

#ifndef RM_MMR_H
#define RM_MMR_H

#include <types/short_types.h>
#include <types/address_types.h>

/**
 * Used to define an MMR as not having a channelized firewall.
 */
#define RM_MMR_FWL_INVALID 0xFFFFu

/**
 * \struct rm_mmr
 *
 * \brief MMR info for SoC device
 *
 * MMR info for a device within the SoC.
 *
 * \param base
 * Base address of the MMR region
 *
 * \param fwl_id
 * ID of channelized firewall which controls access to the MMR region.  Set to
 * @ref RM_MMR_FWL_INVALID if a channelized firewall does not protect the MMR
 * block.
 *
 * \param fwl_ch_start
 * Starting offset of channelized firewall channels within the firewall group
 *
 * \param fwl_ch_end
 * Ending offset of channelized firewall channels within the firewall group
 */
struct rm_mmr {
	soc_phys_addr_t base;
	u16		fwl_id;
	u16		fwl_ch_start;
	u16		fwl_ch_end;
};

#endif /* RM_MMR_H */
