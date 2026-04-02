/******************************************************************************
 * FILE PURPOSE: AC Init Functions
 ******************************************************************************
 * FILE NAME: sslldacinit.c
 *
 * DESCRIPTION: The intialization code for AC Code
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
/* RTSC header files */ 

/* Standard include files */
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldloc.h"
#include "src/salldport.h"
#include "salldac.h"
#include "salldacloc.h"

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain memory requirements for an instance of 
 *                    AC channel
 ******************************************************************************
 * DESCRIPTION: Obtains memory requirement for an AC instance 
 *
 * uint16_t salld_ac_get_size (void)
 *
 *****************************************************************************/
uint16_t salld_ac_get_size (void)
{
  return(sizeof(salldAcInst_t)); 
}

/******************************************************************************
 * FUNCTION PURPOSE: Create an AC channel instance 
 ******************************************************************************
 * DESCRIPTION: Initialize the AC instance with proper configuration 
 *
 * int16_t  Sa_chanInit (
 *   void*        salldInst   - channel Instance
 *   void*        *cfg)       - a pointer to configuration structure
 *
 *****************************************************************************/
int16_t salld_ac_init (void *salldInst, void *cfg)
{
  salldAcInst_t *inst = (salldAcInst_t *)salldInst;
  salldAcTxInst_t *txInst = &inst->txInst;
  salldAcRxInst_t *rxInst = &inst->rxInst;

  /* Clear the structure */
  memset(txInst, 0, sizeof(salldAcTxInst_t));
  memset(rxInst, 0, sizeof(salldAcTxInst_t));
  memset(&inst->salldInst.stats, 0, sizeof(salldComStats_t));

  return (sa_ERR_OK);  
}

/******************************************************************************
 * FUNCTION PURPOSE: Close an AC instance 
 ******************************************************************************
 * DESCRIPTION: Closes an SALLD AC instance
 *
 *   int16_t salld_ac_close(
 *            void          *salldInst)       - SALLD instance 
 *
 *****************************************************************************/
int16_t salld_ac_close (void *salldInst)
{
  salldAcInst_t *inst   = (salldAcInst_t *)salldInst; 
  salldAcTxInst_t *txInst = &inst->txInst;
  salldAcRxInst_t *rxInst = &inst->rxInst;
  salldAcComInfo_t *pTxComInfo = &txInst->comInfo;
  Sa_AcConfigParams_t  *pTxConfig = &pTxComInfo->config;
  salldAcComInfo_t *pRxComInfo = &rxInst->comInfo;
  Sa_AcConfigParams_t  *pRxConfig = &pRxComInfo->config;
  Sa_ScReqInfo_t* pScInfo;
  int16_t ret = sa_ERR_OK;
  // int coreNum = (int)Sa_osalGetProcId();
   
  /* Release all active SALLD channel and security context */
  if (SALLD_TEST_STATE_TX_ON(&inst->salldInst))
  {
    /* Check if need to free keys from scratch memory */
	if(pTxConfig->ctrlBitMap & sa_AC_CONFIG_KEY_IN_SCRATCH)
	{
		/* Initialize pointer to scratch memory key allocation bitmap */
		uint32_t * scratchAllocBitMap;
		salldObj_t * owner = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr,inst->salldInst.ownerInstOffset);  

		/* Get the scratch allocation bit map */
		scratchAllocBitMap = owner->scratchAllocBitmap;		
		
		/* Free both enc key and mac key from channel instance */
		salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, pTxComInfo->sessionEncKeyScratchOffset);
		salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, pTxComInfo->sessionMacKeyScratchOffset);
	}
	/* Clear Active channel */
    salld_send_null_pkt ((Sa_ChanHandle) inst, &txInst->destInfo, 
                          &txInst->swInfo, SA_SC_FLAGS_TEAR|SA_SC_FLAGS_EVICT);
    
    /* free the security context */
    pScInfo = &txInst->scInfo;
    salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
    memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    
    SALLD_SET_STATE_TX_ON(&inst->salldInst, 0);
    
  }
  
  if (SALLD_TEST_STATE_RX_ON(&inst->salldInst))
  {
    /* Check if need to free keys from scratch memory */
	if(pRxConfig->ctrlBitMap & sa_AC_CONFIG_KEY_IN_SCRATCH)
	{
		/* Initialize pointer to scratch memory key allocation bitmap */
		uint32_t * scratchAllocBitMap;
		salldObj_t * owner = (salldObj_t *) sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr,inst->salldInst.ownerInstOffset);  

		/* Get the scratch allocation bit map */
		scratchAllocBitMap = owner->scratchAllocBitmap;		
		
		/* Free both enc key and mac key from channel instance */
		salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, pRxComInfo->sessionEncKeyScratchOffset);
		salld_ac_key_allocation_bitmap_free(scratchAllocBitMap, pRxComInfo->sessionMacKeyScratchOffset);
	}
	/* Unregister the current Context Info */
    salldLObj.callOutFuncs.ChanUnRegister((Sa_ChanHandle) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), &rxInst->swInfo);

    /* Clear Active channel */
    salld_send_null_pkt ((Sa_ChanHandle) inst, &rxInst->destInfo, 
                          &rxInst->swInfo, SA_SC_FLAGS_TEAR|SA_SC_FLAGS_EVICT);

    /* free the security context */
    pScInfo = &rxInst->scInfo;
    salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
    memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    
    SALLD_SET_STATE_RX_ON(&inst->salldInst, 0);
    
  }
  
  
  
  return (ret);
}

/* Nothing past this point */
