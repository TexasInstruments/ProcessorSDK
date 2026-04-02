/**
 *   @file  k2e/src/nss_device.c
 *
 *   @brief   
 *      This file contains the device specific configuration and initialization routines
 *      for NSS (Network Sub-System). These configurations are not used by PA LLD, SA LLD
 *      or CPSW CSL FL. Instaed, they may be included and used by the application modules
 *      which invoke NSS and those LLDs.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2014, Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 *  \par
*/

/* System includes */
#include <stdint.h>
#include <stdlib.h>

/* NSS includes */
#ifdef NSS_LITE
#include <ti/drv/sa/test/SaUnitTest/k2g/nss/nss_if.h>
#include <ti/drv/sa/test/SaUnitTest/k2g/nss/nss_cfg.h>
#else
#include <ti/drv/pa/nss_if.h>
#include <ti/drv/pa/nss_cfg.h>
#endif

/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/csl_qm_queue.h>

/* PDSP images */


/** @brief NSS initialization parameters */
nssGlobalConfigParams_t nssGblCfgParams =
{
    /** NSS Layout parameters */
    { 
        /** 1: NSS Gen2 or Lite device */
        TRUE,
        
        /** Number of PacketDMA Rx channels */
        NSS_NUM_RX_PKTDMA_CHANNELS_LITE,
        
        /** Number of PacketDMA Tx channels */
        NSS_NUM_TX_PKTDMA_CHANNELS_LITE,
        
        /** Number of Transmit Queues */
        NSS_NUM_TX_QUEUES_LITE,
        
        /** Transmit Queue base */
        QMSS_PASS_QUEUE_BASE,
        
        /** Local Transmit Queue base (NSS Gen2 only) */
        NULL,
            
        /** Index to the PASS packet input queue */
        NSS_LAYOUT_PARAM_NA,
      
        /** Index to the PASS Mac queue for MAC/SRIO configuration packets */
        NSS_LAYOUT_PARAM_NA,
        
        /** Index to the PASS IP queue for (outer) IP configuration packets */
        NSS_LAYOUT_PARAM_NA,
        
        /** Index to the PASS Inner IP queue for inner IP configuration packets and 
             IPSEC Tunnel re-entry packets from SASS */
        NSS_LAYOUT_PARAM_NA,
             
        /** Index to the PASS LUT2 queue for LUT2 configuration packets and 
             IPSEC Transport mode re-entry packets from SASS */
        NSS_LAYOUT_PARAM_NA,
             
        /** Index to the PASS IPSEC queue for (outer) IP/IPSEC configuration packets */
        NSS_LAYOUT_PARAM_NA,
        
        /** Index to the PASS IPSEC2 queue for inner IP/IPSEC configuration packets */
        NSS_LAYOUT_PARAM_NA,
        
        /** Index to the PASS Post-Classification queue for Post-Classification
             related configuration packets */
        NSS_LAYOUT_PARAM_NA,
             
        /** Index to the PASS Tx command queue for Egress Packets */
        NSS_LAYOUT_PARAM_NA,
        
        /** Index to the PASS (outer) IP firewall queue for (outer) IP firewall configuration
             packets (NSS Gen2 only) */
        NSS_LAYOUT_PARAM_NA,
              
        /** Index to the PASS Inner IP firewall queue for inner IP firewall configuration 
             packets (NSS Gen2 only)*/
        NSS_LAYOUT_PARAM_NA,
             
        /** Index to the PASS Egress stage 0 queue (NSS Gen2 only)*/
        NSS_LAYOUT_PARAM_NA,
        
        /** Index to the PASS Egress stage 1 queue (NSS Gen2 only)*/
        NSS_LAYOUT_PARAM_NA,
        
        /** Index to the PASS Egress stage 2 queue (NSS Gen2 only)*/
        NSS_LAYOUT_PARAM_NA,
      
        /** Index to the SASS input 1 queue for IPSEC and SRTP packets*/
        NSS_SA_QUEUE_SASS_INDEX_LITE,
        
        /** Index to the SASS input 2 queue for Air-Ciphering and data mode packets */
        NSS_SA_QUEUE_SASS2_INDEX_LITE,
      
        /** Index to the CPSW CPPI port transmit queue */
        NSS_CPSW_QUEUE_ETH_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 0 packets */
        NSS_CPSW_QUEUE_ETH_PRI0_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 1 packets */
        NSS_CPSW_QUEUE_ETH_PRI1_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 2 packets */
        NSS_CPSW_QUEUE_ETH_PRI2_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 3 packets */
        NSS_CPSW_QUEUE_ETH_PRI3_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 4 packets */
        NSS_CPSW_QUEUE_ETH_PRI4_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 5 packets */
        NSS_CPSW_QUEUE_ETH_PRI5_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 6 packets */
        NSS_CPSW_QUEUE_ETH_PRI6_INDEX_LITE,
        
        /** Index to the CPSW CPPI port transmit queue of priority 7 packets */
        NSS_CPSW_QUEUE_ETH_PRI7_INDEX_LITE,
        
        /** Number of PASS PDSPs */
        0,
        
        /** Number of SASS PDSPs */
        0,
        
        /** PA PDSP images */
        {
            /* PDSP0 image */
            NULL,
            
        },
        
        
        /** PA PDSP image sizes  */
        {
            /* PDSP0 image */
            0,
        },
        
        /** SA PDSP images */
        {
            /* Packet Header Processing 1 image */
            NULL,
        },
        
        
        /** SA PDSP image sizes */
        {
            /* Packet Header Processing 1 image */
            0,
        }
    }
};

/**
@}
*/

