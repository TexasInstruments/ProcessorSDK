/*
 * UDMAP SoC configuration
 *
 * Defines the SoC-specific configuration of the UDMAP architecture
 *
 * Copyright (C) 2018-2023, Texas Instruments Incorporated
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

#ifndef UDMAP_CFG_H
#define UDMAP_CFG_H

/**
 * AM6 Main Navigator Subsystem total number of UDMAP transmit channels
 */
#define AM6_MAIN_NAVSS_UDMAP_TX_CHAN_MAX_INDEX    (152u)

/**
 * AM6 Main Navigator Subsystem total number of UDMAP receive channels
 */
#define AM6_MAIN_NAVSS_UDMAP_RX_CHAN_MAX_INDEX    (150u)

/**
 * AM6 Main Navigator Subsystem total number of UDMAP receive flows
 */
#define AM6_MAIN_NAVSS_UDMAP_RX_FLOW_MAX_INDEX    (300u)

/**
 * AM6 MCU Navigator Subsystem total number of UDMAP transmit channels
 */
#define AM6_MCU_NAVSS_UDMAP_TX_CHAN_MAX_INDEX     (48u)

/**
 * AM6 MCU Navigator Subsystem total number of UDMAP receive channels
 */
#define AM6_MCU_NAVSS_UDMAP_RX_CHAN_MAX_INDEX     (48u)

/**
 * AM6 MCU Navigator Subsystem total number of UDMAP receive flows
 */
#define AM6_MCU_NAVSS_UDMAP_RX_FLOW_MAX_INDEX     (96u)

/**
 * UDMAP transmit channel OES index offset
 */
#define UDMAP_TCHAN_OES_EVT_INDEX_OFFSET (0x0U)
/**
 * UDMAP transmit channel error OES index offset
 */
#define UDMAP_TCHAN_EOES_EVT_INDEX_OFFSET (0x100U)
/**
 * UDMAP receive channel OES index offset
 */
#define UDMAP_RCHAN_OES_EVT_INDEX_OFFSET (0x200U)
/**
 * UDMAP receive channel error OES index offset
 */
#define UDMAP_RCHAN_EOES_EVT_INDEX_OFFSET (0x300U)
/**
 * UDMAP invalid flow event index.  Must be greater than the max of all SoC
 * UDMAPs channel OES indexes to avoid event indexing conflicts.
 */
#define UDMAP_GCFG_INVALID_FLOW_EVT_INDEX (0x400U)
/**
 * Total number of UDMA instances
 */
#define UDMAP_INST_RANGE (2U)

#endif /* UDMAP_CFG_H */
