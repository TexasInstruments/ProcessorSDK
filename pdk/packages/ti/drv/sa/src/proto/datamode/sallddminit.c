/******************************************************************************
 * FILE PURPOSE: Data Mode Init Functions
 ******************************************************************************
 * FILE NAME: sallddminit.c
 *
 * DESCRIPTION: The intialization code for Data Mode operation
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
#include "sallddm.h"
#include "sallddmloc.h"

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain memory requirements for an instance of 
 *                    Data Mode channel
 ******************************************************************************
 * DESCRIPTION: Obtains memory requirement for an Data Mode instance 
 *
 * uint16_t salld_data_mode_get_size (void)
 *
 *****************************************************************************/
uint16_t salld_data_mode_get_size (void)
{
  return(sizeof(salldDataModeInst_t)); 
}

/******************************************************************************
 * FUNCTION PURPOSE: Create an Data Mode channel instance 
 ******************************************************************************
 * DESCRIPTION: Initialize the Data Mode instance with proper configuration 
 *
 * int16_t  Sa_chanInit (
 *   void*        salldInst   - channel Instance
 *   void*        *cfg)       - a pointer to configuration structure
 *
 *****************************************************************************/
int16_t salld_data_mode_init (void *salldInst, void *cfg)
{
  salldDataModeInst_t *inst = (salldDataModeInst_t *)salldInst;
  salldDataModeTxInst_t *txInst = &inst->txInst;

  /* Clear the structure */
  memset(txInst, 0, sizeof(salldDataModeTxInst_t));
  memset(&inst->salldInst.stats, 0, sizeof(salldComStats_t));

  return (sa_ERR_OK);  
}

/******************************************************************************
 * FUNCTION PURPOSE: Close an Data Mode instance 
 ******************************************************************************
 * DESCRIPTION: Closes an SALLD Data Mode instance
 *
 *    int salld_data_mode_close(
 *            void          *salldInst)       - SALLD instance 
 *
 *****************************************************************************/
int16_t salld_data_mode_close (void *salldInst)
{
  salldDataModeInst_t *inst   = (salldDataModeInst_t *)salldInst; 
  salldDataModeTxInst_t *txInst = &inst->txInst;
  Sa_ScReqInfo_t* pScInfo;
  int16_t ret = sa_ERR_OK;
  
  /* Release all active SALLD channel and security context */
  if (SALLD_TEST_STATE_TX_ON(&inst->salldInst))
  {
    /* Clear DataModetive channel */
    salld_send_null_pkt ((Sa_ChanHandle) inst, &txInst->destInfo, 
                          &txInst->swInfo, SA_SC_FLAGS_TEAR|SA_SC_FLAGS_EVICT);
    
    /* free the security context */
    pScInfo = &txInst->scInfo;
    salldLObj.callOutFuncs.ScFree((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,inst), pScInfo->scID);
    memset(pScInfo, 0, sizeof(Sa_ScReqInfo_t));
    
    SALLD_SET_STATE_TX_ON(&inst->salldInst, 0);
    
  }
  
  return (ret);
}

/* Nothing past this point */
