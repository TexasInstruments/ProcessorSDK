/**
 *   @file  salldsim.c
 *
 *   @brief   
 *      This is the SA Low Level Driver unit test file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2020, Texas Instruments, Inc.
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

#include "../unittest.h"
#include "salldcfg.h"
#include <ti/csl/cslr_cp_ace.h>

#ifdef SA_3GPP_SUPPORT
#include <ti/drv/sa/sa3gppEnabler/sa3gpp.h>
#endif

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/* SA control register for debugging purpose only */
static CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)CSL_NETCP_CFG_SA_CFG_REGS;
#endif

extern Bool saTestLogEnable;


/* Global booleans */
volatile Bool salldSimHalt;

/* Global eror code */
salldSimErr_e salldSimErrorId;

/* Gobal system timer variable */
uint32_t systemTime = 0;
uint16_t prevTime = 0;

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
volatile uint32_t* pSAModelTrig = (volatile uint32_t*)(CSL_NETCP_CFG_SA_CFG_REGS + 0x4000);
#endif

/* Global char signifying entity calling salldsimDebug() function */
char salldsimDebugString = 0;

/* Global char signifying entity calling salldsimFatalError() function */
char salldsimFatalString = 0;

char err_string[1];

/* Global string for logfile printing */
char print_string[salld_SIM_MAX_STRING];

int numSalldSimChans;

/* Simulation configuration structure & pointer */
simConfig_t simConfig;
simConfig_t *simCfg = &simConfig;

/* Global SALLD call-out function table */
static Sa_CallOutFuncs_t salldsim_calloutFunc = {
    (void (*)(Sa_ChanHandle, uint16_t, uint16_t, uint16_t, uint16_t *)) salldsimDebugInfo,               /* Debug function pointer */
    (void (*)(Sa_ChanHandle, Sa_KeyRequest_t *))salldsimChanKeyRequest,          /* Key Request function Pointer */
    (void (*)(Sa_ChanHandle handle, Sa_ScReqInfo_t* scReqInfo))salldsimScAlloc,                 /* Security Context Allocation function pointer */
    (void (*)(Sa_ChanHandle handle, uint16_t scID))salldsimScFree,                  /* Security Context Free Function pointer */
    (void (*)(Sa_ChanHandle handle, Sa_SWInfo_t* chanSwInfo))salldsimChanRegister,            /* Channel Registration Function pointer */
    (void (*)(Sa_ChanHandle handle, Sa_SWInfo_t* chanSwInfo))salldsimChanUnRegister,          /* Channel UnRegister Function pointer */
    (void (*)(Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo))salldsimChanSendNullPkt          /* Channel Send Null Packet function pointer */  
};


#define SALLDSIM_GET_CHAN(id)     ((id) & 0x00FF)

extern void sauReregegisterUdp(void* pHdl);

/*Internal Memory Scratch Buffer */
#ifdef _TMS320C6X
#pragma DATA_SECTION(gIntBuf, ".intMem")
#endif
uint8_t gIntBuf[SALLDSIM_SC_BUF_SIZE];


/* Define SA related base address */
void mdebugHaltSaPdsp (Int pdspNum)
{
    #if !defined(NSS_LITE) && !defined(NSS_LITE2)
	pSaRegs->PDSP_CONTROL_STATUS[pdspNum].PDSP_CONTROL &= ~(CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK);
    #endif

}

/*-----------------------------------------------------------------
 * Function:  salld_sim_halt
 *							  
 * Description: Utility to provide simulation breakpoint.
 *-----------------------------------------------------------------*/
void salld_sim_halt (void)
{
  /* Wait for further actions - exit when user sets "halt" to FALSE */
  while (salldSimHalt) {
    asm ("  NOP");
    asm ("  NOP");
    asm ("  NOP");
    asm ("  NOP");
  }
  /* Reset "halt" to TRUE for next entrance */
  salldSimHalt = TRUE;
}

/******************************************************************************
 * FUNCTION PURPOSE: Generic function for handling SALLD exception calls.
 ******************************************************************************
 * DESCRIPTION: Handles exception and debug calls. Goes into and infinite loop,
 *              allowing for an eventual break and examintaaion of reason for
 *              calling a fata error. The global variable testFatalString
 *              contains the reason for entry, as enumerated in the global
 *              enumeration testFatalErrorChar_e of file sa_lld_test.h.
 *
 * CALLING FORMAT:
 *   testFatalError   (uint16_t ID,       -  ID and channel of calling entity
 *                                         (either FM or DPU)
 *                    char* str)        -  Global ptr to char that holds the
 *                                          value of reason for entering
 *                                          this function
 *
 * RETURN VALUE:
 *   None
 *    
 *****************************************************************************/
void salldsimFatalError (uint16_t id, char* str)
{
    salld_sim_print("SALLDSIM %d: %s \n", id, str);
    salld_sim_halt ();
}


/******************************************************************************
 * FUNCTION PURPOSE: Implements SALLD's debugInfo() API call.
 ******************************************************************************
 * DESCRIPTION: Handles SALLD API calls to debugInfo(). All INFORMATIONAL msgs
 *              are ignored; if the msgType is FATAL_ERROR or undefined,
 *              a call to salldsimFatalError() is made. 
 *
 * CALLING FORMAT:
 *   salldsimSalldDebugInfo (void * moduleID,	  - ID and channel of FIU
 *                       uint16_t msgType,		  - FATAL_ERROR (2) or
 *                                                  INFORMATIONAL (0)
 *                       uint16_t messageCode,	  - not used 
 *                       uint16_t msgLength,	  - not used 
 *                       uint16_t *supportingData)- not used 
 *
 * RETURN VALUES:
 *   None
 *    
 *****************************************************************************/
void salldsimDebugInfo (void* mID, uint16_t msgType, uint16_t messageCode, 
                        uint16_t msgLength,  uint16_t *supportingData)
{

  uint16_t id = Sa_chanGetID((Sa_ChanHandle) mID);
  int chnum = SALLDSIM_GET_CHAN(id);
  char *string = (char *)(uintptr_t)NULL;
  char msg_default[80]   = "TEST_EXCEPTION_DBG_DEFAULT";
  char msg_fatal_err[80] = "TEST_EXCEPTION_DBG_FATAL_ERROR";
  uint16_t time = 0;
  uint16_t deltaTime = 0;
  
  switch (msgType)
  {
     case sa_DBG_INFORMATIONAL:         
         if (msgLength == 2)
         {
           time = supportingData[1];

           if (time >= prevTime)
               deltaTime = time - prevTime;
            else
               deltaTime = (0xFFFF - prevTime + 1) + time;

           prevTime = time;
         }

         salld_sim_print ("%.4X(+%.4dms)GW%d:SALLD~ ", time, deltaTime,chnum);
         salld_sim_print (":msgId = 0x%X, msgData = 0x%X\n", messageCode, *supportingData);
         break;

     case sa_DBG_FATAL_ERROR:
	    string = msg_fatal_err;
	    salldsimFatalError (id, string);
		break;

     default:
	    string = msg_default;
	    salldsimFatalError (id, string);
		break;
  }
} /* salldsimSalldDebugInfo */


/**                      
 *  @brief  Callout to externally supplied system to request a new security key. This function may be triggered
 *  by either the sslldChanSendData() or sslldChanReceiveData() APIs. The application should call the 
 *  Sa_chanControl() API to pass the new key when it is available.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   keyReq       Pointer to SALLD key Request structure.
 *
 *  @sa Sa_KeyRequest_t
 *
 */
void salldsimChanKeyRequest (Sa_ChanHandle handle, Sa_KeyRequest_t* keyReq)
{
    uint16_t id = Sa_chanGetID(handle);
    int chnum = SALLDSIM_GET_CHAN(id);
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    salldSimChannel_t *pChan = &tFrameworkSecure.salldSimChn[chnum - 1];
#else
    salldSimChannel_t *pChan = &tFramework.salldSimChn[chnum - 1];
#endif
    if (keyReq->params.srtp.ctrlBitfield & sa_SRTP_KEY_REQUEST_TX_KEY)
    {
        pChan->txInfo.keyReq = *keyReq;
        pChan->stateBitMap |= salld_SIM_STATE_TX_REKEY;
    }
    
    if (keyReq->params.srtp.ctrlBitfield & sa_SRTP_KEY_REQUEST_RX_KEY)
    {
        pChan->rxInfo.keyReq = *keyReq;
        pChan->stateBitMap |= salld_SIM_STATE_RX_REKEY;
    }
    
    salld_sim_print("SALLD Channel %d: Key Request with ctrlBitfield = 0x%04x\n", chnum - 1, keyReq->params.srtp.ctrlBitfield);
}
   
/**
 *  @brief  Callout to externally supplied system to allocate the security context with the specified size.
 *  This function must be implemented as a simple non-blocking function.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   scReqInfo    Pointer to SALLD security context Control Information structure.
 *
 *  @sa Sa_ScReqInfo_t
 *
 */
void salldsimScAlloc (Sa_ChanHandle handle, Sa_ScReqInfo_t* scReqInfo)
{
    uint16_t id = Sa_chanGetID(handle);
    int chnum = SALLDSIM_GET_CHAN(id);
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    salldSimChannel_t *pChan = &tFrameworkSecure.salldSimChn[chnum - 1];
#else
    salldSimChannel_t *pChan = &tFramework.salldSimChn[chnum - 1];
#endif

    int index;
    
    if(salld_sim_alloc_sc(scReqInfo->scSize, &scReqInfo->scID, (uint8_t **) &scReqInfo->scBuf))
    {
        /* Record the scID and ScBuf in the instance */ 
        for (index = 0; index < 4; index++)
        {
            if(pChan->scInfo[index].scID == 0)
                break;
        }
        
        if (index >= 4)
        {
            salld_sim_print("SALLD Channel %d: salldsimScAlloc failed due to lack of internal buffer\n!", chnum - 1);
            salld_sim_free_sc(scReqInfo->scID);
            scReqInfo->scBuf = (uintptr_t ) NULL;
            return;        
        }
        pChan->scInfoIndex = index;
        pChan->scInfo[index] = *scReqInfo;
    }
    else
    {
         uint16_t      maxAllowed = SALLDSIM_SC_BUF_SIZE;
        SALog("SALLD Channel %d: salldsimScAlloc failed with size = %d, Max allowed is %d!\n", chnum, scReqInfo->scSize, maxAllowed);
        scReqInfo->scBuf = (uintptr_t) NULL;
    }
}
   
/**
 *  @brief  Callout to externally supplied system to release the security context with the specified ID.
 *  This function must be implemented as a simple non-blocking function.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   scID         Security Context ID
 *
 */
void salldsimScFree (Sa_ChanHandle handle, uint16_t scID)
{
    uint16_t id = Sa_chanGetID(handle);
    int chnum = SALLDSIM_GET_CHAN(id);
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    salldSimChannel_t *pChan = &tFrameworkSecure.salldSimChn[chnum - 1];
#else
    salldSimChannel_t *pChan = &tFramework.salldSimChn[chnum - 1];
#endif
    int index;
    
    /* Record the scID and ScBuf in the instance */ 
    for (index = 0; index < 4; index++)
    {
        if (pChan->scInfo[index].scID == scID)
        {
            pChan->scInfo[index].scID = 0;
            salld_sim_free_sc(scID);
            break;
        }    
    }
    
    if (index >= 4)
    {
        salld_sim_print("SALLD channel %d: salldsimScFree failed because scID can not be found at channel instance\n!", chnum - 1);
    }
}
   
/**
 *  @brief  Callout to externally supplied system to register the security channel with its software
 *  routing information.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   chanSwInfo   Pointer to SALLD software routing information structure.
 *
 *  @sa Sa_SWInfo_t
 *
 */
void salldsimChanRegister (Sa_ChanHandle handle, Sa_SWInfo_t* pSwInfo)
{
    uint16_t id = Sa_chanGetID(handle);
    int chnum = SALLDSIM_GET_CHAN(id);
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    salldSimChannel_t *pChan = &tFrameworkSecure.salldSimChn[chnum - 1];
#else
    salldSimChannel_t *pChan = &tFramework.salldSimChn[chnum - 1];
#endif

    pChan->regSwInfo = *pSwInfo;
    
    salld_sim_print("SALLD channel %d: Register SA Info\n", chnum - 1);
    #if !defined(NSS_LITE) && !defined(NSS_LITE2)
    //salld_sim_output_sainfo(chnum, FALSE, pSwInfo);
    
    if ((pChan->state == SALLD_STATE_ACTIVE) && (pChan->protocolType == sa_PT_SRTP))
    {
        sauReregegisterUdp(pChan->pConnHdl);    
    }
    #endif
}

/**
 *  @brief  Callout to externally supplied system to un-register the security channel with its software
 *  routing information.
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   chanSwInfo   Pointer to SALLD software routing information structure.
 *
 *  @sa Sa_SWInfo_t
 *
 */
void salldsimChanUnRegister (Sa_ChanHandle handle, Sa_SWInfo_t* pSwInfo)
{
    uint16_t id = Sa_chanGetID(handle);
    int chnum = SALLDSIM_GET_CHAN(id);
    
    salld_sim_print("SALLD channel %d: UnRegister Sw Info\n", chnum - 1);
}

/**
 *  @brief  Callout to externally supplied system to send an Null packet to the SA sub-system
 *  This is a function pointer and must point to a valid function location who meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   pktInfo      Pointer to the packet info structure.
 *
 *  @sa Sa_PktInfo_t
 *
 */
void salldsimChanSendNullPkt (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo)
{
#if defined(SAU_PRMOTE_DEMOTE_TEST)
#else
    uint16_t id = Sa_chanGetID(handle);
    int chnum = SALLDSIM_GET_CHAN(id);
    Sa_SWInfo_t* pSwInfo = &pktInfo->swInfo;

    salld_sim_print("SALLD channel %d: Send Null Packet\n", chnum - 1);

    testCommonSendNullPkt(&tFramework, pSwInfo->swInfo);
#endif

}

/******************************************************************************/
/******************************************************************************
 **   EXTERNAL API FUNCTIONS  (salldSim_...)
 ******************************************************************************/
/******************************************************************************/

Sa_ChanHandle sauShadowChanHandle;

/*-----------------------------------------------------------------
 * Function:  salldSim_init_chn
 *							  
 * Description: SALLD Channel instantiation.
 *-----------------------------------------------------------------*/
uint16_t salldSim_init_chn (salldSimChannel_t *pSimChan)
{

  uint16_t           index = SALLDSIM_GET_CHAN(pSimChan->ID) - 1;
  Sa_ChanSizeCfg_t   sizeCfg;
  Sa_ChanConfig_t    chanCfg;
  int                aligns[sa_CHAN_N_BUFS];
  uintptr_t          bases[sa_CHAN_N_BUFS];
  int i;
  int16_t ret_code;
  Bool               fShared = ((pSimChan->protocolType == sa_PT_IPSEC_AH) || (pSimChan->protocolType == sa_PT_IPSEC_ESP))?
                               TRUE:FALSE;
  char               msg_start[80] = "Instantiating SALLD Channel %d ...\n";

  /* Check that the channel is not already created */
  if (pSimChan->salldInst != (Sa_ChanHandle)0U) {
    return (salld_SIM_ERR);
  }
  
  /* Progress (verbose only) */
  salld_sim_iprint (msg_start, index);
  
  /* salldGetSizes() parameters */
  memset(&sizeCfg, 0, sizeof(Sa_ChanSizeCfg_t));
  memset(&chanCfg, 0, sizeof(Sa_ChanConfig_t));
  sizeCfg.protocolType = pSimChan->protocolType;
  sizeCfg.cacheLineSize = fShared?SYS_CACHE_LINE_SIZE:0;

#ifndef NSS_LITE2
  sizeCfg.ctrlBitMap = sa_SIZE_CONFIG_CREATE_SHADOW_INST;
#endif

  /* Get SALLD Channel buffer requirements and ensure #buffers within limits */
  ret_code = Sa_chanGetBufferReq (&sizeCfg, pSimChan->buf_sizes, aligns);
  salld_sim_check_rc (salld_SIM_ERR_SALLDGETBUFDEF, ret_code, sa_ERR_OK);

  /* Allocate buffers */
  /* Allocate SALLD memory buffers & increment salld heap size. Data from the (global)
     variable salldBufs is copied into the (local) variable salldLocalBufs in order to
     correctly set salldLocalBufs' size parameter */ 
  for (i = 0; i < sa_CHAN_N_BUFS; i++)
  {
     bases[i]       =  salld_sim_malloc (pSimChan->buf_sizes[i], aligns[i]);
     /*
      * Global address should be used if the channel may be shared among multiple cores 
      */
     if (fShared)
     {
        bases[i] = utilgAddr(bases[i]);
     }
	 pSimChan->salld_heap_size   += pSimChan->buf_sizes[i];
  }

  /* Create (NEW) SALLD channel */
  chanCfg.ID  = (salld_MOD_ID << 8) | (index + 1);
  chanCfg.sizeConfig = sizeCfg;
#if defined(SAU_PRMOTE_DEMOTE_TEST)
  ret_code = Sa_chanCreate (tFrameworkSecure.salld_handle, &chanCfg, (void **) &bases, &pSimChan->salldInst); 
#else
  ret_code = Sa_chanCreate (tFramework.salld_handle, &chanCfg, (void **) &bases, &pSimChan->salldInst); 
#endif
  salld_sim_check_rc (salld_SIM_ERR_SALLDINIT, ret_code, sa_ERR_OK);
  
  /* Progress (verbose only) */
  salld_SIM_PRINT_DONE ();
  
  /* Return success */
  return (salld_SIM_NOERR);
}

/*-----------------------------------------------------------------
 * Function:  salldSim_open_chn
 *							  
 * Description: SALLD Channel open.
 *-----------------------------------------------------------------*/
uint16_t salldSim_open_chn (salldSimChannel_t *pChan, salldOpenConfig_t *cfg)
{
  uint16_t          index = SALLDSIM_GET_CHAN(pChan->ID) - 1;
  Sa_ChanCtrlInfo_t chanCtrlInfo;
  Sa_GenCtrlInfo_t *pGenCtrlInfo = &chanCtrlInfo.ctrlInfo.gen;
  Sa_KeyCtrlInfo_t *pKeyCtrlInfo = &chanCtrlInfo.ctrlInfo.key;
  int16_t           ret_code;
  char              msg_open[80]   = "Opening SALLD Channel %d ...\n";
  char              msg_genCfg[80] = "SALLD Channel %d: General Configuration\n";
  char              msg_keyCfg[80] = "SALLD Channel %d: Key Configuration\n";
  char              msg_txChEnable[80] = "SALLD Channel %d: Enable Tx\n";
  char              msg_rxChEnable[80] = "SALLD Channel %d: Enable Rx\n";

  /* Ensure channel is available */
  if (pChan->salldInst  == (Sa_ChanHandle)0U) 
    return (salld_SIM_ERR);
    
  /* Progress (verbose only) */
  salld_sim_iprint (msg_open, index);

  /* Progress (verbose only) */
  salld_sim_iprint (msg_genCfg, index);

  memset(pGenCtrlInfo, 0, sizeof(Sa_GenCtrlInfo_t));
  if (cfg->ctrlBitMap & sa_CONTROLINFO_CTRL_RX_ON)
  {
    pGenCtrlInfo->validBitfield = sa_CONTROLINFO_VALID_RX_CTRL |
                                  sa_CONTROLINFO_VALID_REPLAY_WIN;
    pGenCtrlInfo->replayWindowSize = pChan->relayWinSize;
    pGenCtrlInfo->rxCtrl = pChan->rxInfo.genCfg;
  }
  
  if (cfg->ctrlBitMap & sa_CONTROLINFO_CTRL_TX_ON)
  {
    pGenCtrlInfo->validBitfield |= sa_CONTROLINFO_VALID_TX_CTRL;
    pGenCtrlInfo->txCtrl = pChan->txInfo.genCfg;
  }
  
  chanCtrlInfo.ctrlType =  (uint16_t)sa_CHAN_CTRL_GEN_CONFIG;                              
                                 
  ret_code = Sa_chanControl ((Sa_ChanHandle)pChan->salldInst, &chanCtrlInfo);
  salld_sim_check_rc (salld_SIM_ERR_SALLDCONTROL, ret_code, sa_ERR_OK);
  
  /* Progress (verbose only) */
  salld_sim_iprint (msg_keyCfg, index);
  
  chanCtrlInfo.ctrlType =  (uint16_t)sa_CHAN_CTRL_KEY_CONFIG;                              
  memset(pKeyCtrlInfo, 0, sizeof(Sa_KeyCtrlInfo_t));
  
  if (cfg->ctrlBitMap & sa_CONTROLINFO_CTRL_RX_ON)
  {
    pKeyCtrlInfo->ctrlBitfield = sa_KEY_CONTROL_RX_KEY_VALID;
    pKeyCtrlInfo->rxKey = pChan->rxInfo.keyCfg;
  }
  
  if (cfg->ctrlBitMap & sa_CONTROLINFO_CTRL_TX_ON)
  {
    pKeyCtrlInfo->ctrlBitfield |= sa_KEY_CONTROL_TX_KEY_VALID;
    pKeyCtrlInfo->txKey = pChan->txInfo.keyCfg;
  }
  
  ret_code = Sa_chanControl ((Sa_ChanHandle)pChan->salldInst, &chanCtrlInfo);
  salld_sim_check_rc (salld_SIM_ERR_SALLDCONTROL, ret_code, sa_ERR_OK);

  /* Enable Tx and Rx seperately so that we can display the SC accordingly */
  chanCtrlInfo.ctrlType =  (uint16_t)sa_CHAN_CTRL_GEN_CONFIG;                              
  memset(pGenCtrlInfo, 0, sizeof(Sa_GenCtrlInfo_t));
  pGenCtrlInfo->validBitfield = sa_CONTROLINFO_VALID_CTRL_BITMAP;
  
  if (cfg->ctrlBitMap & sa_CONTROLINFO_CTRL_TX_ON)
  {
    salld_sim_iprint (msg_txChEnable, index);
  
    pGenCtrlInfo->ctrlBitfield = cfg->ctrlBitMap & ~sa_CONTROLINFO_CTRL_RX_ON;

    ret_code = Sa_chanControl ((Sa_ChanHandle)pChan->salldInst, &chanCtrlInfo);
    salld_sim_check_rc (salld_SIM_ERR_SALLDCONTROL, ret_code, sa_ERR_OK);
    
    /* Output the Tx Security Context */
    //salld_sim_output_sc(index+1, TRUE, &pChan->scInfo[pChan->scInfoIndex]);
    
  }
  
  if (cfg->ctrlBitMap & sa_CONTROLINFO_CTRL_RX_ON)
  {
    #if SA_GEN_TEST_VECTOR_RX
    Sa_ScReqInfo_t* pReqInfo;
    #endif
  
    salld_sim_iprint (msg_rxChEnable, index);
  
    pGenCtrlInfo->ctrlBitfield = cfg->ctrlBitMap;
    ret_code = Sa_chanControl ((Sa_ChanHandle)pChan->salldInst, &chanCtrlInfo);
    salld_sim_check_rc (salld_SIM_ERR_SALLDCONTROL, ret_code, sa_ERR_OK);
    
    /* Output the Rx Security Context */
    #if SA_GEN_TEST_VECTOR_RX
    pReqInfo = &pChan->scInfo[pChan->scInfoIndex];
    utlOutputSc(&tFramework, pReqInfo->scSize, pReqInfo->scID, pReqInfo->scBuf);
    #endif
  }
  
  /* Record the new state */
  pChan->stateBitMap = cfg->ctrlBitMap;
  
  pChan->state = SALLD_STATE_ACTIVE;
  
  ret_code = Sa_chanGetShadowHandle((Sa_ChanHandle)pChan->salldInst, &sauShadowChanHandle);

  /* Progress (verbose only) */
  salld_SIM_PRINT_DONE ();

  /* Return success */
  return (salld_SIM_NOERR);
}

/*-----------------------------------------------------------------
 * Function:  salldSim_close_chn
 *							  
 * Description: SALLD Channel close.
 *-----------------------------------------------------------------*/
uint16_t salldSim_close_chn (salldSimChannel_t *pChan)
{
  int16_t index = SALLDSIM_GET_CHAN(pChan->ID) - 1;
  int16_t ret_code;
  void*   bases[sa_CHAN_N_BUFS];
  int     i;
  char    msg_chClose[80] = "Closing SALLD Channel %d ...\n";

  /* Ensure channel is open */
  if (pChan->salldInst  == (Sa_ChanHandle)0U) 
    return (salld_SIM_ERR);
  
  /* Progress (verbose only) */
  salld_sim_iprint (msg_chClose, index);

  /* Close the SALLD channel */
  ret_code = Sa_chanClose (pChan->salldInst, bases);
  salld_sim_check_rc (salld_SIM_ERR_SALLDCLOSE, ret_code, sa_ERR_OK);

  /* Set Channel state to "closed" */
  pChan->salldInst = (Sa_ChanHandle)0U;
  
  /* Free memory allocated for SALLD buffers & adjust salld_heap_size */
  for (i = 0; i < sa_CHAN_N_BUFS; i++)
  {
     #if !defined(NSS_LITE2)
     if (((uint32_t)bases[i] & 0xf0000000) == 0x10000000)
     {
        /* Convert back to local address */
        bases[i] = (void *)((uint32_t)bases[i] & 0xffffff);   
     }
     #endif
  
     salld_sim_free (bases[i], pChan->buf_sizes[i]);
	 pChan->salld_heap_size -= pChan->buf_sizes[i];
  }

  /* TBD: clear memory */
  memset(pChan, 0, sizeof(salldSimChannel_t));
  pChan->ID = (salld_SIM_ID << 8) | (index + 1);
  
  pChan->state = SALLD_STATE_IDLE;

  /* Progress (verbose only) */
  salld_SIM_PRINT_DONE ();

  /* Return success */
  return (salld_SIM_NOERR);
}

/*-----------------------------------------------------------------
 * Function:  salldSim_poll_chn
 *							  
 * Description: Poll SALLD Channel to simulate some special operations
 *              such as re-key
 *-----------------------------------------------------------------*/
uint16_t salldSim_poll_chn (salldSimChannel_t *pChan)
{
  Sa_ChanCtrlInfo_t chanCtrlInfo;
  Sa_KeyCtrlInfo_t *pKeyCtrlInfo = &chanCtrlInfo.ctrlInfo.key;
  Sa_SrtpKeyRequest_t*  pKeyReq;
  Sa_SrtpKeyParams_t* pKeyParams;
  int16_t ret_code;

  /* Ensure channel is available */
  if (pChan->salldInst  == (Sa_ChanHandle)0U) 
    return (salld_SIM_ERR);
    
  memset(&chanCtrlInfo, 0, sizeof(Sa_ChanCtrlInfo_t));
  chanCtrlInfo.ctrlType =  (uint16_t)sa_CHAN_CTRL_KEY_CONFIG;                              
    
  /* Tx Rekey Operation */  
  if (pChan->stateBitMap & salld_SIM_STATE_TX_REKEY) 
  {
     pChan->stateBitMap &= ~salld_SIM_STATE_TX_REKEY;
     pKeyCtrlInfo->ctrlBitfield = sa_KEY_CONTROL_TX_KEY_VALID;
     
     pKeyReq = &pChan->txInfo.keyReq.params.srtp;
     pKeyParams = &pChan->txInfo.keyCfg.srtp;
     
     // Clear the roc(index) flag
     pKeyParams->ctrlBitfield &= ~sa_SRTP_KEY_CTRL_ROC;
     
     if (pKeyReq->ctrlBitfield & sa_SRTP_KEY_REQUEST_KEY_TYPE_MKI)
     {
        /* MKI key */
        if (pKeyParams->mkiSize)
        {
            pKeyParams->mki++;
            pKeyParams->keyLifeTimeLsw = 0xffffffff;
            
            /* Set the expected RX re-key info */
            pChan->rxInfo.keyCfg.srtp.mki = pKeyParams->mki;
        }
     }
     else
     {
        /* From-To key */
        /* Update the from To range */
        pKeyParams->fromEsnMsw = pKeyParams->toEsnMsw;
        pKeyParams->fromEsnLsw = pKeyParams->toEsnLsw+1;
        
        pKeyParams->toEsnLsw += 1000;
        if(pKeyParams->toEsnLsw < pKeyParams->fromEsnLsw)
            pKeyParams->toEsnMsw++;    
     }
     
     pKeyParams->masterKey[15]++;
     pKeyParams->masterSalt[13]++;
     
     pKeyCtrlInfo->txKey = pChan->txInfo.keyCfg;
     
  }   
    
    
  if (pChan->stateBitMap & salld_SIM_STATE_RX_REKEY)
  {
     pChan->stateBitMap &= ~salld_SIM_STATE_RX_REKEY;
     pKeyCtrlInfo->ctrlBitfield |= sa_KEY_CONTROL_RX_KEY_VALID;
     
     pKeyReq = &pChan->rxInfo.keyReq.params.srtp;
     pKeyParams = &pChan->rxInfo.keyCfg.srtp;
     
     //clear ROC (index)  flag
     pKeyParams->ctrlBitfield &= ~sa_SRTP_KEY_CTRL_ROC;
     
     if (pKeyReq->ctrlBitfield & sa_SRTP_KEY_REQUEST_KEY_TYPE_MKI)
     {
        if (pKeyReq->ctrlBitfield & sa_SRTP_KEY_REQUEST_MKI_VALID)
        {
            if (pKeyParams->mki != pKeyReq->mki)
            {
                /* The requested key is not available */
                pKeyCtrlInfo->ctrlBitfield &= ~sa_KEY_CONTROL_RX_KEY_VALID;
            }
        }
        else
        {
            pKeyParams->mki++;
        }
        
        pKeyParams->keyLifeTimeLsw = 0xffffffff;
        
     }
     else
     {
        /* From-To key */
        /* Update the from To range */
        pKeyParams->fromEsnMsw = pKeyParams->toEsnMsw;
        pKeyParams->fromEsnLsw = pKeyParams->toEsnLsw+1;
        
        pKeyParams->toEsnLsw += 1000;
        if(pKeyParams->toEsnLsw < pKeyParams->fromEsnLsw)
            pKeyParams->toEsnMsw++;    
     }
     
     /* TBD: It has been updated by TX key already */
     #if 0
     pKeyParams->masterKey[15]++;
     pKeyParams->masterSalt[13]++;
     #endif
     
     pKeyCtrlInfo->rxKey = pChan->rxInfo.keyCfg;
  }   
  
  if (pKeyCtrlInfo->ctrlBitfield)
  {
    ret_code = Sa_chanControl ((Sa_ChanHandle)pChan->salldInst, &chanCtrlInfo);
    salld_sim_check_rc (salld_SIM_ERR_SALLDCONTROL, ret_code, sa_ERR_OK);
  }

  /* Return success */
  return (salld_SIM_NOERR);
}


/*-----------------------------------------------------------------
 * Function:  salldSim_get_stats
 *							  
 * Description: SALLD Channel Satistics.
 *-----------------------------------------------------------------*/
uint16_t salldSim_get_stats (salldSimChannel_t *pChan)
{
  int16_t            index = SALLDSIM_GET_CHAN(pChan->ID) - 1;
  Sa_Stats_t         stats;
  int16_t ret_code = sa_ERR_OK;
  int i;
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  volatile uint32_t fakeRead;
#endif
  /* Ensure channel is open */
  if (pChan->salldInst  == (Sa_ChanHandle) 0U) 
    return (salld_SIM_ERR);
  
  if (Sa_chanGetStats (pChan->salldInst, sa_STATS_QUERY_FLAG_TRIG, &stats) == sa_ERR_OK)
  {
      /* Statistics is already available */
      salld_sim_disp_chan_stats(index, pChan->protocolType, &stats);

      /* Return success */
      return (salld_SIM_NOERR);
  }
  
  /* Wait for the stats reply */
  for (i = 0; i < 200; i++)  {
        /* Simulator workaround: Kick off the SA simulator clock */
        /* No effect for the real chip */
        #if !defined(NSS_LITE) && !defined(NSS_LITE2)
        fakeRead = *pSAModelTrig; 
        #endif
	    utilCycleDelay (1000);
	    if (Sa_chanGetStats (pChan->salldInst, 0, &stats) == sa_ERR_OK)
		    break;
  }
  
  if (i >= 200)
  {
      salld_sim_print("salldSim_get_stats: Stats is not available in time\n");
      ret_code = Sa_chanGetStats (pChan->salldInst, sa_STATS_QUERY_FLAG_NOW, &stats);
  }
  
  salld_sim_check_rc (salld_SIM_ERR_SALLDSTATS, ret_code, sa_ERR_OK);
  
  salld_sim_disp_chan_stats(index, pChan->protocolType, &stats);

  /* Return success */
  return (salld_SIM_NOERR);
}

/*-----------------------------------------------------------------
 * Function:  salldSim_close_all_chns
 *							  
 * Description: SALLD Channel close.
 *-----------------------------------------------------------------*/
void salldSim_close_all_chns (void)
{
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    salldSimChannel_t *pChan = &tFrameworkSecure.salldSimChn[0];
#else
    salldSimChannel_t *pChan = &tFramework.salldSimChn[0];
#endif
    int i;
    
    for (i = 0; i < numSalldSimChans; i++, pChan++)
    {
        salldSim_close_chn(pChan);    
    }
    numSalldSimChans = 0;
}

/*-----------------------------------------------------------------
 * Function:  salldSim_get_all_chan_stats
 *							  
 * Description: Inquire all channel stats.
 *-----------------------------------------------------------------*/
void salldSim_get_all_chan_stats (void)
{
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    salldSimChannel_t *pChan = &tFrameworkSecure.salldSimChn[0];
#else
    salldSimChannel_t *pChan = &tFramework.salldSimChn[0];
#endif

    int i;
    
    for (i = 0; i < numSalldSimChans; i++, pChan++)
    {
        salldSim_get_stats(pChan);    
    }
}

/*-----------------------------------------------------------------
 * Function:  salldSim_get_sys_stats
 *							  
 * Description: SALLD System Satistics.
 *-----------------------------------------------------------------*/
void salldSim_get_sys_stats (void)
{
  Sa_SysStats_t         stats;

#if defined(SAU_PRMOTE_DEMOTE_TEST)
  Sa_getSysStats (tFrameworkSecure.salld_handle, &stats);

#else
  Sa_getSysStats (tFramework.salld_handle, &stats);
#endif

  salld_sim_disp_system_stats(&stats);
}

/******************************************************************************/
/******************************************************************************
 **   LOCAL API FUNCTIONS (salld_sim_...)
 ******************************************************************************/
/******************************************************************************/

/*-----------------------------------------------------------------
 * Function:  salld_sim_check_rc
 *							  
 * Description: Check return code for failure.
 *-----------------------------------------------------------------*/
void salld_sim_check_rc (salldSimErr_e id, uint16_t rc, uint16_t pass_code)
{
  char msg_badRc[80] = "\nERROR Bad Return Code: 0x%4x\n\n";
  /* Wait forever */
  if (rc != pass_code) {
    /* Set ID for quick identifcation */
    salldSimErrorId = id;
    salld_sim_iprint (msg_badRc, (int32_t) id);
    salld_sim_halt ();
  }
}


/*-----------------------------------------------------------------
 * Function:  salld_sim_print
 *							  
 * Description: Utility to provide static progress information.
 *-----------------------------------------------------------------*/
#define SATEST_PRINT_BUF_LEN   (2048U)
void salld_sim_print (const char* fmt, ...)
{
  if (simCfg->verbose_print)
  {
#if defined(SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_AM64X)
     char  buf[SATEST_PRINT_BUF_LEN];
#endif
     va_list args;
     va_start(args, fmt);
#ifdef __LINUX_USER_SPACE
     vprintf(fmt, args);
#elif defined(SOC_AM65XX) || defined (SOC_J721E) || defined(SOC_AM64X)
    SAPrintBuf(buf, SATEST_PRINT_BUF_LEN, (const char *) fmt, args);
#else
     System_vprintf(fmt, args);
#endif
     va_end(args);

#if defined(SOC_AM65XX) || defined(SOC_J721E) || defined (SOC_AM64X)
     SALog(buf);
#endif
  }
}


/*-----------------------------------------------------------------
 * Function:  salld_sim_iprint
 *							  
 * Description: Utility to provide integer progress information.
 *-----------------------------------------------------------------*/
void salld_sim_iprint (char *str, int32_t ivar)
{
  sprintf (print_string, str, ivar);
  salld_sim_print (print_string);
}


/*-----------------------------------------------------------------
 * Function:  salld_sim_sprint
 *							  
 * Description: Utility to provide string progress information.
 *-----------------------------------------------------------------*/
void salld_sim_sprint (char *str, char *svar)
{
  sprintf (print_string, str, svar);
  salld_sim_print (print_string);
}

/*-----------------------------------------------------------------
 * Function:  salld_sim_set_defaults
 *							  
 * Description: SALLD simulation default initialization.
 *-----------------------------------------------------------------*/
void salld_sim_set_defaults (void)
{
  /* Default globals initialization */
  salldSimHalt     = TRUE;
  salldSimErrorId  = salld_SIM_ERR_NONE;

  /* Default boolean initialization */
  simCfg->verbose_print = saTestLogEnable;
  
}

/*-----------------------------------------------------------------
 * Function:  salld_sim_disp_control
 *							  
 * Description: SALLD print control
 *          TRUE: force enable
 *          FALSE: reset to default   
 *-----------------------------------------------------------------*/
void salld_sim_disp_control (bool enable)
{

  /* Default boolean initialization */
  if(enable)
    simCfg->verbose_print = TRUE;
  else
    simCfg->verbose_print = saTestLogEnable;
  
}


/*-----------------------------------------------------------------
 * Function:  salld_sim_disp_chan_stats
 *							  
 * Description: SALLD simulation display channel statistics.
 *-----------------------------------------------------------------*/
void salld_sim_disp_chan_stats(int16_t chanNum, Sa_SecProto_e protocolType, Sa_Stats_t *pStats)
{
    switch (protocolType)
    {
        case sa_PT_NULL:
        {
            Sa_DataModeStats_t  *p_stats = &pStats->data;
            
            salld_sim_print("Chan %d (Data Mode) Statistics:\n", chanNum);
            salld_sim_print("----------------------------------------------------\n");
            salld_sim_print("numPktsHi =          0x%08x\n", p_stats->pktHi);
            salld_sim_print("numPktsLo =          0x%08x\n", p_stats->pktLo);
            break;
        }
    
        case sa_PT_SRTP:
        {
            Sa_SrtpStats_t  *p_stats = &pStats->srtp;
            
            salld_sim_print("Chan %d (SRTP) Statistics:\n", chanNum);
            salld_sim_print("----------------------------------------------------\n");
            salld_sim_print("numPktsEnc =         0x%04x%08x\n", p_stats->pktEncHi, p_stats->pktEncLo);
            salld_sim_print("numPktsDec =         0x%04x%08x\n", p_stats->pktDecHi, p_stats->pktDecLo);
            salld_sim_print("numTxRekey =         %d\n", p_stats->txRekey);
            salld_sim_print("numRxRekey =         %d\n", p_stats->rxRekey);
            salld_sim_print("numReplayOld =       %d\n", p_stats->replayOld);
            salld_sim_print("numReplayDup =       %d\n", p_stats->replayDup);
            salld_sim_print("numAuthFail  =       %d\n", p_stats->authFail);
            salld_sim_print("txROC  =             0x%08x\n", p_stats->txROC);
            salld_sim_print("rxROC  =             0x%08x\n", p_stats->rxROC);
            break;
        }
        
        case sa_PT_SRTCP:
        {
            Sa_SrtcpStats_t  *p_stats = &pStats->srtcp;
            
            salld_sim_print("Chan %d (SRTCP) Statistics:\n", chanNum);
            salld_sim_print("----------------------------------------------------\n");
            salld_sim_print("numPktsEnc =         0x%08x\n", p_stats->pktEnc);
            salld_sim_print("numPktsDec =         0x%08x\n", p_stats->pktDec);
            salld_sim_print("numTxRekey =         %d\n", p_stats->txRekey);
            salld_sim_print("numRxRekey =         %d\n", p_stats->rxRekey);
            salld_sim_print("numReplayOld =       %d\n", p_stats->replayOld);
            salld_sim_print("numReplayDup =       %d\n", p_stats->replayDup);
            salld_sim_print("numAuthFail  =       %d\n", p_stats->authFail);
            break;
        }
        
        
        case sa_PT_IPSEC_ESP:
        case sa_PT_IPSEC_AH:
        {
            Sa_IpsecStats_t  *p_stats = &pStats->ipsec;
            
            salld_sim_print("Chan %d (IPSEC) Statistics:\n", chanNum);
            salld_sim_print("----------------------------------------------------\n");
            salld_sim_print("numPktsEncHi =       0x%08x\n", p_stats->pktEncHi);
            salld_sim_print("numPktsEncLo =       0x%08x\n", p_stats->pktEncLo);
            salld_sim_print("numPktsDecHi =       0x%08x\n", p_stats->pktDecHi);
            salld_sim_print("numPktsDecLo =       0x%08x\n", p_stats->pktDecLo);
            salld_sim_print("numBytesEncHi =      0x%08x\n", p_stats->txByteCountHi);
            salld_sim_print("numBytesEncLo =      0x%08x\n", p_stats->txByteCountLo);
            salld_sim_print("numBytesDecHi =      0x%08x\n", p_stats->rxByteCountHi);
            salld_sim_print("numBytesDecLo =      0x%08x\n", p_stats->rxByteCountLo);
            salld_sim_print("numReplayOld =       %d\n", p_stats->replayOld);
            salld_sim_print("numReplayDup =       %d\n", p_stats->replayDup);
            salld_sim_print("numAuthFail  =       %d\n", p_stats->authFail);
            salld_sim_print("txESN  =             0x%08x\n", p_stats->txESN);
            salld_sim_print("txSN   =             0x%08x\n", p_stats->txSN);
            salld_sim_print("rxESN  =             0x%08x\n", p_stats->rxESN);
            break;
        }
    
        case sa_PT_3GPP_AC:
        {
        
            Sa_AcStats_t  *p_stats = &pStats->ac;
            
            salld_sim_print("Chan %d (Air Ciphering) Statistics:\n", chanNum);
            salld_sim_print("----------------------------------------------------\n");
            salld_sim_print("numPktsToAirHi =       0x%08x\n", p_stats->pktToAirHi);
            salld_sim_print("numPktsToAirLo =       0x%08x\n", p_stats->pktToAirLo);
            salld_sim_print("numPktsFromAirHi =     0x%08x\n", p_stats->pktFromAirHi);
            salld_sim_print("numPktsFromAirLo =     0x%08x\n", p_stats->pktFromAirLo);
            salld_sim_print("numAuthFail  =         %d\n", p_stats->authFail);
            salld_sim_print("toAirCount-C  =        0x%08x\n", p_stats->toAirCountC);
            salld_sim_print("fromAirCount-C  =      0x%08x\n", p_stats->fromAirCountC);
        
            break;
        }
        
        default:
            salld_sim_print("Chan %d Invalid Protocol %d\n", chanNum, protocolType);
            break;
    }
}    

/*-----------------------------------------------------------------
 * Function:  salld_sim_disp_system_stats
 *							  
 * Description: SALLD simulation display system statistics.
 *-----------------------------------------------------------------*/
void salld_sim_disp_system_stats(Sa_SysStats_t *p_stats)
{
    salld_sim_print("SA System Statistics: Error\n");
    salld_sim_print("--------------------------------------------\n");
    salld_sim_print("errNoMem =     0x%08x\n", p_stats->err.errNoMem);
    salld_sim_print("errCtx   =     0x%08x\n", p_stats->err.errCtx);
    salld_sim_print("errEngine=     0x%08x\n", p_stats->err.errEngine);
    salld_sim_print("errProto =     0x%08x\n", p_stats->err.errProto);
    
    salld_sim_print("\nSA System Statistics: IPSEC ESP\n");
    salld_sim_print("--------------------------------------------\n");
    salld_sim_print("replayOld =    0x%08x\n", p_stats->esp.replayOld);
    salld_sim_print("replayDup =    0x%08x\n", p_stats->esp.replayDup);
    salld_sim_print("authFail  =    0x%08x\n", p_stats->esp.authFail );
    salld_sim_print("pktEncHi  =    0x%08x\n", p_stats->esp.pktEncHi );
    salld_sim_print("pktEncLo  =    0x%08x\n", p_stats->esp.pktEncLo );
    salld_sim_print("pktDecHi  =    0x%08x\n", p_stats->esp.pktDecHi );
    salld_sim_print("pktDecLo  =    0x%08x\n", p_stats->esp.pktDecLo );
    
    salld_sim_print("\nSA System Statistics: IPSEC AH\n");
    salld_sim_print("--------------------------------------------\n");
    salld_sim_print("replayOld =    0x%08x\n", p_stats->ah.replayOld);
    salld_sim_print("replayDup =    0x%08x\n", p_stats->ah.replayDup);
    salld_sim_print("authFail  =    0x%08x\n", p_stats->ah.authFail );
    salld_sim_print("pktEncHi  =    0x%08x\n", p_stats->ah.pktEncHi );
    salld_sim_print("pktEncLo  =    0x%08x\n", p_stats->ah.pktEncLo );
    salld_sim_print("pktDecHi  =    0x%08x\n", p_stats->ah.pktDecHi );
    salld_sim_print("pktDecLo  =    0x%08x\n", p_stats->ah.pktDecLo );

    salld_sim_print("\nSA System Statistics: SRTP\n");
    salld_sim_print("--------------------------------------------\n");
    salld_sim_print("replayOld =    0x%08x\n", p_stats->srtp.replayOld);
    salld_sim_print("replayDup =    0x%08x\n", p_stats->srtp.replayDup);
    salld_sim_print("authFail  =    0x%08x\n", p_stats->srtp.authFail );
    salld_sim_print("pktEncHi  =    0x%08x\n", p_stats->srtp.pktEncHi );
    salld_sim_print("pktEncLo  =    0x%08x\n", p_stats->srtp.pktEncLo );
    salld_sim_print("pktDecHi  =    0x%08x\n", p_stats->srtp.pktDecHi );
    salld_sim_print("pktDecLo  =    0x%08x\n", p_stats->srtp.pktDecLo );
    
    salld_sim_print("\nSA System Statistics: Air Ciphering\n");
    salld_sim_print("--------------------------------------------\n");
    salld_sim_print("authFail    =  0x%08x\n", p_stats->ac.authFail );
    salld_sim_print("pktToAirHi  =  0x%08x\n", p_stats->ac.pktToAirHi );
    salld_sim_print("pktToAirLo  =  0x%08x\n", p_stats->ac.pktToAirLo );
    salld_sim_print("pktFromAirHi=  0x%08x\n", p_stats->ac.pktFromAirHi );
    salld_sim_print("pktFromAirLo=  0x%08x\n", p_stats->ac.pktFromAirLo );
}


/*-----------------------------------------------------------------
 * Function:  salld_download_pdsp_images
 *							  
 * Description: Download PDSP images.
 *-----------------------------------------------------------------*/
int16_t salld_download_pdsp_images (void)
{

#if !defined(NSS_LITE) && !defined(NSS_LITE2)

  uint32_t  version;
  int i;

  Sa_resetControl (tFramework.salld_handle, sa_STATE_RESET);
  
  for ( i = 0; i < nssGblCfgParams.layout.numSaPdsps; i++)
  {
  
    Sa_downloadImage (tFramework.salld_handle, i, 
                      (Ptr)nssGblCfgParams.layout.saPdspImage[i], 
                      nssGblCfgParams.layout.saPdspImageSize[i]);
  }
  
  if(Sa_resetControl (tFramework.salld_handle, sa_STATE_ENABLE) != sa_STATE_ENABLE)
    return (-1);
    
  utilCycleDelay (1000);
    
  for ( i = 0; i < nssGblCfgParams.layout.numSaPdsps; i++)
  {
    Sa_getPDSPVersion(tFramework.salld_handle, i, &version);
    SALog ("PDSP %d version = 0x%08x\n", i, version);
    System_flush();
  }

#endif    
  return (sa_ERR_OK);
}

Sa_Handle sauShadowHandle;

/*-----------------------------------------------------------------
 * Function:  salld_sim_init_sa
 *							  
 * Description: Initialize the SA instance.
 *-----------------------------------------------------------------*/
void salld_sim_init_sa (uint8_t limitAccess)
{
  Sa_SizeCfg_t  sizeCfg;
  Sa_Config_t   cfg;
  int           aligns[sa_N_BUFS];
  void*         bases[sa_N_BUFS];
  int i;
  int16_t       ret_code;

  /* Progress (verbose only) */
  salld_sim_print ("Instantiating SALLD ...");
  
  /* Get SALLD buffer requirements and ensure #buffers within limits */
  memset(&sizeCfg, 0, sizeof(Sa_SizeCfg_t));
  memset(&cfg, 0, sizeof(Sa_Config_t));
  sizeCfg.nMaxChan = salld_SIM_MAX_CHANNELS;
  sizeCfg.cacheLineSize = SYS_CACHE_LINE_SIZE;
#ifdef NSS_LITE2
  sizeCfg.ctrlBitMap  |= sa_SIZE_CONFIG_SASS_UL_GEN2;
#else
  sizeCfg.ctrlBitMap = sa_SIZE_CONFIG_CREATE_SHADOW_INST;
  if (nssGblCfgParams.layout.fNssGen2)
  {
    sizeCfg.ctrlBitMap |= sa_SIZE_CONFIG_SASS_GEN2;
  }
#endif

#if defined(SAU_PRMOTE_DEMOTE_TEST)
  ret_code = Sa_getBufferReq (&sizeCfg, tFrameworkSecure.salld_buf_sizes, aligns);
#else
  ret_code = Sa_getBufferReq (&sizeCfg, tFramework.salld_buf_sizes, aligns);
#endif
  salld_sim_check_rc (salld_SIM_ERR_SYSGETBUFDEF, ret_code, sa_ERR_OK);

  /* Allocate buffers */
  /* Allocate SALLD memory buffers & increment salld heap size. Data from the (global)
     variable salldBufs is copied into the (local) variable salldLocalBufs in order to
     correctly set salldLocalBufs' size parameter */ 
  for (i = 0; i < sa_N_BUFS; i++)
  {
     /*
      * The LLD system buffers may be shared among multiple cores.
      * Therefore global address should be provided.
      */
#if defined(SAU_PRMOTE_DEMOTE_TEST)
     bases[i]  =  (void *)utilgAddr((uint32_t)salld_sim_malloc (tFrameworkSecure.salld_buf_sizes[i], aligns[i]));
#else
     bases[i]  =  (void *)utilgAddr((uint32_t)salld_sim_malloc (tFramework.salld_buf_sizes[i], aligns[i]));
#endif
  }

  /* Create SALLD system */
  cfg.ID  = 0xbabebeef;
  cfg.callTable = &salldsim_calloutFunc;
  #if !defined(NSS_LITE) && !defined(NSS_LITE2)
  cfg.baseAddr = CSL_NETCP_CFG_SA_CFG_REGS;
  #else
  #ifdef NSS_LITE2
  cfg.baseAddr = SA_UTEST_SA2_UL0_BASE;
  #else
  cfg.baseAddr = CSL_NSS_0_CFG_REGS + 0x00400000;
  #endif
  #endif
  cfg.sizeConfig = &sizeCfg;
  cfg.ctrlBitMap = 0;
  
  /* Provide an internal scratch memory for SA */
  cfg.intBuf = (void *)gIntBuf;
#ifndef __LINUX_USER_SPACE
  cfg.ctrlBitMap = sa_CONFIG_CTRL_BITMAP_TRIGGER_SYS_ERR_HALT;
  /*
   * Reference only
   * This flag can be set only if there are only 3GPP channels running on PDSP2,
   * in other word, there are no SRTP channels.
   */
  // cfg.ctrlBitMap |= sa_CONFIG_CTRL_BITMAP_TRIGGER_PKT_INFO_LOG;
#endif

#if defined(NSS_LITE2) && defined(SAU_PRMOTE_DEMOTE_TEST)
  {
    uint32_t scPtrPromoteLowRangeL,  scPtrPromoteLowRangeH;
    uint32_t scPtrPromoteHighRangeL, scPtrPromoteHighRangeH;

    scPtrPromoteLowRangeL  = ((uint32_t)(((uint64_t)&salldsimScBufSecure[0][0]) >> 0U )) & ((uint32_t) 0xFFFFFFFF);
    scPtrPromoteLowRangeH  = ((uint32_t)(((uint64_t)&salldsimScBufSecure[0][0]) >> 32U)) & ((uint32_t) 0xFFFFFFFF);
    scPtrPromoteHighRangeL = ((uint32_t)(((uint64_t)&salldsimScBufSecure[0][0]) >> 0U)) & ((uint32_t) 0xFFFFFFFF);
    scPtrPromoteHighRangeH = ((uint32_t)(((uint64_t)&salldsimScBufSecure[0][0]) >> 32U)) & ((uint32_t) 0xFFFFFFFF);

    cfg.scPtrRange.scPtrPromoteLowRangeL  = scPtrPromoteLowRangeL & 0x1000;
    cfg.scPtrRange.scPtrPromoteLowRangeH  = (scPtrPromoteLowRangeH + 0x1000) & 0x1000;
    cfg.scPtrRange.scPtrPromoteHighRangeL = scPtrPromoteHighRangeL & 0x1000;
    cfg.scPtrRange.scPtrPromoteHighRangeH = (scPtrPromoteHighRangeH + 0x1000) & 0x1000;
    cfg.ctrlBitMap |= sa_CONFIG_CTRL_BITMAP_SET_SCPTR_RANGE;
  }
#endif

    /*
     * Has the framework determined if the access should be limited for this
     * instance?
     */
    if (limitAccess)
    {
        cfg.ctrlBitMap |= sa_CONFIG_CTRL_BITMAP_LIMIT_ACCESS;
    }

#if defined(SAU_PRMOTE_DEMOTE_TEST)
  ret_code = Sa_create (&cfg, bases, &tFrameworkSecure.salld_handle);
  salld_sim_check_rc (salld_SIM_ERR_SYSINIT, ret_code, sa_ERR_OK);
  
  ret_code =  Sa_getShadowHandle(tFrameworkSecure.salld_handle, &sauShadowHandle);
#else
  ret_code = Sa_create (&cfg, (void **)bases, &tFramework.salld_handle);
  salld_sim_check_rc (salld_SIM_ERR_SYSINIT, ret_code, sa_ERR_OK);
  
  ret_code =  Sa_getShadowHandle(tFramework.salld_handle, &sauShadowHandle);
#endif

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  /* Download SA PDSP Firmwares */
  ret_code = salld_download_pdsp_images();

  salld_sim_check_rc (salld_SIM_ERR_PDSPDNLD, ret_code, sa_ERR_OK);
  
  //salld_sim_halt();
#ifdef SA_3GPP_SUPPORT 
  // Enable 3GPP 
  Sa_3gppEnabler();
#endif
#endif

  /* Progress (verbose only) */
  salld_SIM_PRINT_DONE ();
  
}

/*-----------------------------------------------------------------
 * Function:  salld_sim_close_sa
 *							  
 * Description: SALLD close.
 *-----------------------------------------------------------------*/
void salld_sim_close_sa (void)
{
  int16_t ret_code; 
  int i;
  void* bases[sa_N_BUFS];

  /* Progress (verbose only) */
  salld_sim_print ("Closing SALLD ...");

  /* Close the SALLD */
#if defined(SAU_PRMOTE_DEMOTE_TEST)
  ret_code = Sa_close (tFrameworkSecure.salld_handle, bases);
#else
  ret_code = Sa_close (tFramework.salld_handle, bases);
#endif
  salld_sim_check_rc (salld_SIM_ERR_SYSCLOSE, ret_code, sa_ERR_OK);

  /* Free memory allocated for SALLD buffers & adjust salld_heap_size */
  for (i = 0; i < sa_N_BUFS; i++)
  {
     #if !defined(NSS_LITE2)
     if (((uint32_t)bases[i] & 0xf0000000) == 0x10000000)
     {
        /* Convert back to local address */
        bases[i] = (void *)((uint32_t)bases[i] & 0xffffff);   
     }
     #endif
#if defined(SAU_PRMOTE_DEMOTE_TEST)
     salld_sim_free (bases[i], tFrameworkSecure.salld_buf_sizes[i]);
#else
     salld_sim_free (bases[i], tFramework.salld_buf_sizes[i]);
#endif
  }

  /* Free memory for SALLD buffer descriptors & adjust aux_heap_size */
#if defined(SAU_PRMOTE_DEMOTE_TEST)
  tFrameworkSecure.salld_handle = NULL;
#else
  tFramework.salld_handle = (Sa_Handle)0U;
#endif
  /* Progress (verbose only) */
  salld_SIM_PRINT_DONE ();

}

/*-----------------------------------------------------------------
 * Function:  salld_sim_initialize
 *							  
 * Description: SALLD simulation default initialization.
 *-----------------------------------------------------------------*/
void salld_sim_initialize (uint8_t limitAccess)
{
  int16_t i;
  
  salld_sim_set_defaults();

  #if defined(SAU_PRMOTE_DEMOTE_TEST)
  /* Initialize SALLD engine and channel structures */
  for (i=0; i<salld_SIM_MAX_CHANNELS; i++) {
    memset(&tFrameworkSecure.salldSimChn[i], 0, sizeof(salldSimChannel_t));
    tFrameworkSecure.salldSimChn[i].ID = (salld_SIM_ID << 8) | (i + 1);
    tFrameworkSecure.salldSimChn[i].state = SALLD_STATE_IDLE;
    tFrameworkSecure.salldSimChn[i].rxInfo.pgnIndex = SALLD_PGN_INDEX_NONE;
    tFrameworkSecure.salldSimChn[i].txInfo.pgnIndex = SALLD_PGN_INDEX_NONE;
  }
  #else
  /* Initialize SALLD engine and channel structures */
  for (i=0; i<salld_SIM_MAX_CHANNELS; i++) {
    memset(&tFramework.salldSimChn[i], 0, sizeof(salldSimChannel_t));
    tFramework.salldSimChn[i].ID = (salld_SIM_ID << 8) | (i + 1);
    tFramework.salldSimChn[i].state = SALLD_STATE_IDLE;
    tFramework.salldSimChn[i].rxInfo.pgnIndex = SALLD_PGN_INDEX_NONE;
    tFramework.salldSimChn[i].txInfo.pgnIndex = SALLD_PGN_INDEX_NONE;
  }
  #endif

  /* Initialize the Security context management system */
  salld_sim_init_sc();

  /* Initialize the SA system */
  salld_sim_init_sa(limitAccess);

}

/* Nothing past this point */
