/******************************************************************************
 * FILE PURPOSE: Secure RTCP Init Functions
 ******************************************************************************
 * FILE NAME: salldsrtcpinit.c
 *
 * DESCRIPTION: The intialization code for Secure RTCP Code
 *
 *
 * (C) Copyright 2009, Texas Instruments Inc.
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
*/
/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "salldsrtcp.h"
#include "salldsrtcploc.h"

/* system files */
#include <string.h>

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain memory requirements for an instance of 
 *                    SRTCP LLD
 ******************************************************************************
 * DESCRIPTION: Obtains memory requirement for an SRTCP instance 
 *
 * uint16_t salld_srtcp_get_size (void)
 *
 *****************************************************************************/
uint16_t salld_srtcp_get_size (void)
{
  return(sizeof(salldSrtcpInst_t)); 
}

/******************************************************************************
 * FUNCTION PURPOSE: Create an SRTP channel instance 
 ******************************************************************************
 * DESCRIPTION: Initialize the SRTP instance with proper configuration 
 *
 * int16_t  Sa_chanInit (
 *   void*        salldInst   - channel Instance
 *   void*        *cfg)       - a pointer to configuration structure
 *
 *****************************************************************************/
int16_t salld_srtcp_init (void *salldInst, void *cfg)
{
  salldSrtcpInst_t *inst = (salldSrtcpInst_t *)salldInst;
  salldSrtcpTxInst_t *txInst = &inst->txInst;
  salldSrtcpRxInst_t *rxInst = &inst->rxInst;

  /* Clear the structure */
  memset(txInst, 0, sizeof(salldSrtcpTxInst_t));
  memset(rxInst, 0, sizeof(salldSrtcpTxInst_t));
  memset(&inst->salldInst.stats, 0, sizeof(salldComStats_t));

  /* Default key_derivation_rate = no key derivation */
  txInst->keyInfo.kdBitfield = SALLD_SRTP_KD_RATE_MASK;
  rxInst->keyInfo.kdBitfield = SALLD_SRTP_KD_RATE_MASK;

  /* Default SRTCP master key lifetime */
  txInst->keyInfo.keyLifetime = 0x7FFFFFFF;
  rxInst->keyInfo.keyLifetime = 0x7FFFFFFF;

  rxInst->windowCheck = 64; /* 64 bit sliding window for seq check */
  rxInst->replayWindow.winSize = 64;
  salld_replay_init(&rxInst->replayWindow, 0); 

  return (sa_ERR_OK);  
}

/******************************************************************************
 * FUNCTION PURPOSE: Close an SRTCP instance 
 ******************************************************************************
 * DESCRIPTION: Closes an SALLD SRTCP instance
 *
 *    int salld_srtcp_close(
 *            void          *salldInst)       - SALLD instance 
 *
 *****************************************************************************/
int16_t salld_srtcp_close (void *salldInst)
{
  /* No SRTCP specific requirement at this moment */
  return (sa_ERR_OK);

}
   
/* Nothing past this point */
