/******************************************************************************
 * FILE PURPOSE: IPSEC Init Functions
 ******************************************************************************
 * FILE NAME: sslldipsecinit.c
 *
 * DESCRIPTION: The intialization code for IPSEC Code
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
#include "salldipsec.h"
#include "salldipsecloc.h"

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain memory requirements for an instance of 
 *                    IPSEC channel
 ******************************************************************************
 * DESCRIPTION: Obtains memory requirement for an IPSEC instance 
 *
 * uint16_t salld_ipsec_get_size (void)
 *
 *****************************************************************************/
uint16_t salld_ipsec_get_size (void)
{
  return(sizeof(salldIpsecInst_t)); 
}

/******************************************************************************
 * FUNCTION PURPOSE: Create an IPSEC channel instance 
 ******************************************************************************
 * DESCRIPTION: Initialize the IPSEC instance with proper configuration 
 *
 * int16_t  Sa_chanInit (
 *   void*        salldInst   - channel Instance
 *   void*        *cfg)       - a pointer to configuration structure
 *
 *****************************************************************************/
int16_t salld_ipsec_init (void *salldInst, void *cfg)
{
  salldIpsecInst_t *inst = (salldIpsecInst_t *)salldInst;
  salldIpsecTxInst_t *txInst = &inst->txInst;
  salldIpsecRxInst_t *rxInst = &inst->rxInst;

  /* Clear the structure */
  memset(txInst, 0, sizeof(salldIpsecTxInst_t));
  memset(rxInst, 0, sizeof(salldIpsecTxInst_t));
  memset(&inst->salldInst.stats, 0, sizeof(salldComStats_t));

  return (sa_ERR_OK);  
}

/******************************************************************************
 * FUNCTION PURPOSE: Close an IPSEC instance 
 ******************************************************************************
 * DESCRIPTION: Closes an SALLD IPSEC instance
 *
 *    int salld_ipsec_close(
 *            void          *salldInst)       - SALLD instance 
 *
 *****************************************************************************/
int16_t salld_ipsec_close (void *salldInst)
{
  salldIpsecInst_t *inst   = (salldIpsecInst_t *)salldInst; 
  salldIpsecTxInst_t *txInst = &inst->txInst;
  salldIpsecRxInst_t *rxInst = &inst->rxInst;
  Sa_ScReqInfo_t* pScInfo;
  int16_t ret = sa_ERR_OK;
  
  /* Release all active SALLD channel and security context */
  if (SALLD_TEST_STATE_TX_ON(&inst->salldInst))
  {
  
    /* Clear Active channel */
    salld_send_null_pkt ((Sa_ChanHandle) inst, &txInst->destInfo, 
                          &txInst->swInfo, SA_SC_FLAGS_TEAR|SA_SC_FLAGS_EVICT);
    
    /* free the security context */
    pScInfo = &txInst->scInfo;
    salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID & sa_SC_ID_MASK);
    memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    
    SALLD_SET_STATE_TX_ON(&inst->salldInst, 0);
    
  }
  
  if (SALLD_TEST_STATE_RX_ON(&inst->salldInst))
  {
    /* Unregister the current Context Info */
    salldLObj.callOutFuncs.ChanUnRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), &rxInst->swInfo);
  
    /* Clear Active channel */
    salld_send_null_pkt ((Sa_ChanHandle) inst, &rxInst->destInfo, 
                          &rxInst->swInfo, SA_SC_FLAGS_TEAR|SA_SC_FLAGS_EVICT);
    
    /* free the security context */
    pScInfo = &rxInst->scInfo;
    salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID & sa_SC_ID_MASK);
    memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    
    SALLD_SET_STATE_RX_ON(&inst->salldInst, 0);
    
  }
  return (ret);
}
   
/* Nothing past this point */
