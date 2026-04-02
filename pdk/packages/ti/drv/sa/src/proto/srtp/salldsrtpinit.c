/******************************************************************************
 * FILE PURPOSE: Secure RTP Init Functions
 ******************************************************************************
 * FILE NAME: sslldsrtpinit.c
 *
 * DESCRIPTION: The intialization code for Secure RTP Code
 *
 * (C) Copyright 2009-2013, Texas Instruments Inc.
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
/* RTSC header files */ 

/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "salldsrtp.h"
#include "salldsrtploc.h"

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain memory requirements for an instance of 
 *                    SRTP channel
 ******************************************************************************
 * DESCRIPTION: Obtains memory requirement for an SRTP instance 
 *
 * uint16_t salld_srtp_get_size (void)
 *
 *****************************************************************************/
uint16_t salld_srtp_get_size (void)
{
  return(sizeof(salldSrtpInst_t)); 
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
int16_t salld_srtp_init (void *salldInst, void *cfg)
{
  salldSrtpInst_t *inst = (salldSrtpInst_t *)salldInst;
  salldSrtpTxInst_t *txInst = &inst->txInst;
  salldSrtpRxInst_t *rxInst = &inst->rxInst;

  /* Clear the structure */
  memset(txInst, 0, sizeof(salldSrtpTxInst_t));
  memset(rxInst, 0, sizeof(salldSrtpTxInst_t));
  memset(&inst->salldInst.stats, 0, sizeof(salldComStats_t));

  /* Default key_derivation_rate = no key derivation */
  txInst->keyInfo.kdBitfield = SALLD_SRTP_KD_RATE_MASK;
  rxInst->keyInfo.kdBitfield = SALLD_SRTP_KD_RATE_MASK;
                    
  /* Default SRTP master key lifetime */
  txInst->keyInfo.keyLifetimeMsb = 0xFFFF;
  txInst->keyInfo.keyLifetimeLsb = 0xFFFFFFFF;
  rxInst->keyInfo.keyLifetimeMsb = 0xFFFF;
  rxInst->keyInfo.keyLifetimeLsb = 0xFFFFFFFF;

  rxInst->windowCheck = 64; /* 64 bit sliding window for seq check */
  rxInst->replayWindow.winSize = 64;
  salld_replay_init(&rxInst->replayWindow, 0);
  
  return (sa_ERR_OK);  
}

/******************************************************************************
 * FUNCTION PURPOSE: Close an SRTP instance 
 ******************************************************************************
 * DESCRIPTION: Closes an SALLD SRTP instance
 *
 *    int salld_srtp_close(
 *            void          *salldInst)       - SALLD instance 
 *
 *****************************************************************************/
int16_t salld_srtp_close (void *salldInst)
{
  salldSrtpInst_t *inst   = (salldSrtpInst_t *)salldInst; 
  salldSrtpTxInst_t *txInst = &inst->txInst;
  salldSrtpRxInst_t *rxInst = &inst->rxInst;
  Sa_ScReqInfo_t* pScInfo;
  int16_t ret = sa_ERR_OK;
  
  /* Release all active SALLD channel and security context */
  if (SALLD_TEST_STATE_TX_ON(&inst->salldInst))
  {
    /* Clear Active channel */
    salld_send_null_pkt ((Sa_ChanHandle) inst, &txInst->destInfo, 
                          &txInst->swInfo, SA_SC_FLAGS_TEAR | SA_SC_FLAGS_EVICT);
    
    /* free the security context */
    pScInfo = &txInst->scInfo[SRTP_ACTIVE_SC_INDEX];
    salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
    memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    
    SALLD_SET_STATE_TX_ON(&inst->salldInst, 0);
    
    /* Free the pending SC if it is not freed yet */
    pScInfo = &txInst->scInfo[SRTP_PENDING_SC_INDEX];
    if (pScInfo->scSize)
    {
        salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
        memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    }
  }
  
  if (SALLD_TEST_STATE_RX_ON(&inst->salldInst))
  {
    /* Unregister the current Context Info */
    salldLObj.callOutFuncs.ChanUnRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), &rxInst->swInfo);

    /* Clear Active channel */
    salld_send_null_pkt ((Sa_ChanHandle) inst, &rxInst->destInfo, 
                          &rxInst->swInfo, SA_SC_FLAGS_TEAR | SA_SC_FLAGS_EVICT);
    
    /* free the security context */
    pScInfo = &rxInst->scInfo[SRTP_ACTIVE_SC_INDEX];
    salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
    memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    
    SALLD_SET_STATE_RX_ON(&inst->salldInst, 0);
    
    /* Free the pending SC if it is not freed yet */
    pScInfo = &rxInst->scInfo[SRTP_PENDING_SC_INDEX];
    if (pScInfo->scSize)
    {
        salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
        memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    }
  }

  return (ret);
}

/* Nothing past this point */
