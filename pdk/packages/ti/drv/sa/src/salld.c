/******************************************************************************
 * FILE PURPOSE:  Main Routines for SA LLD
 ******************************************************************************
 * FILE NAME:   salld.c  
 *
 * DESCRIPTION: Main SA LLD functions 
 *
 * FUNCTION               DESCRIPTION
 * ========               ===========
 * salldGetSizes()        Obtains memory requirements for an instance of SALLD
 * Sa_chanInit()          Creates an instance of SALLD
 *
 *
 *
 * REVISION HISTORY:
 *
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
/* System level header files */

#include <stdlib.h>
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include <ti/drv/sa/src/salldloc.h>
#include <ti/drv/sa/src/salldproto.h>

/* SALLD Protocol Specific API files */
#include <ti/drv/sa/src/proto/srtp/salldsrtp.h>
#include <ti/drv/sa/src/proto/srtp/salldsrtcp.h>
#include <ti/drv/sa/src/proto/ipsec/salldipsec.h>
#include <ti/drv/sa/src/proto/airciph/salldac.h>
#include <ti/drv/sa/src/proto/datamode/sallddm.h>

/* SALLD protocol call table */
const Sa_ProtocolCallTbl_t* salld_callTblPtr[] = 
{
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    &Sa_callTblStrp,
    &Sa_callTblSrtcp,
    &Sa_callTblIpsecAh,
    &Sa_callTblIpsecEsp,
    &Sa_callTblAc,
#endif    
    &Sa_callTblDataMode,
    NULL
};

/* SA Local object */
salldLocObj_t    salldLObj;

/******************************************************************************
 * FUNCTION PURPOSE:  Return the Protocol-specific Channel instance size
 ******************************************************************************
 * DESCRIPTION: This function returns the Protocol-specific Channel instance size
 *****************************************************************************/
 static void salld_get_chan_inst_size(Sa_SecProto_e protocolType, int* pSize)
 {
 
    /* Get sizes based on the security protocol */
    switch (protocolType)
    {
        case sa_PT_NULL: 
            *pSize = salld_data_mode_get_size();
            break;
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
        case sa_PT_SRTP:
            *pSize = salld_srtp_get_size();
            break;
        case sa_PT_SRTCP:
            *pSize = salld_srtcp_get_size();
            break;
        case sa_PT_IPSEC_AH:
        case sa_PT_IPSEC_ESP:
            *pSize = salld_ipsec_get_size();
            break;
        case sa_PT_3GPP_AC:
            *pSize = salld_ac_get_size();
            break;
#endif        
        default:
            *pSize = 0;    
    }
 }
 
 
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/******************************************************************************
 * FUNCTION PURPOSE: Select IPSEC engine 
 ******************************************************************************
 * DESCRIPTION: This function returns the index of selected IPSEC engine 
 *				based on system and channel configuration. 
 *
 *****************************************************************************/
static uint16_t salld_select_ipsec_engine(salldObj_t *inst, uint16_t sel)
{
    int engSelMode = SALLD_GET_STATE_ENG_SEL_MODE(inst);
    
    switch (engSelMode)
    {
        case sa_EngSelMode_LOADBALANCED:
        {
            if (inst->engCounter[0] > inst->engCounter[1])
            {
                inst->engCounter[1]++;
                return(1);
            }
            else
            {
                inst->engCounter[0]++;
                return(0);
            }
        }
        
        case sa_EngSelMode_ROUNDROBIN:
        {
            if (inst->engCounter[0] == 0)
            {
                inst->engCounter[0] = 1;
                return(0);
            }
            else
            {
                inst->engCounter[0] = 0;
                return(1);
            }
        }
        
        case sa_EngSelMode_USERSPECIFIED:
        {
            /* sa_EngSelMode_USERSPECIFIED */
            return((sel == 0)?0:1);
        }
        
        default:
            return(0);
    }        
            
}

#endif

/******************************************************************************
 * FUNCTION PURPOSE:  Obtains memory buffer requirements for an SALLD
 *                    channel instance
 ******************************************************************************
 * DESCRIPTION: This function obtains memory buffer requirements for an SALLD
 *         channel instance. The memory buffer requirements are in terms of
 *         the size and alignment array which must not be changed by external 
 *         software.  
 *         Worst case memory size requirements are computed using parameters 
 *         provided by Sa_ChanSizeCfg_t. 
 *
 * int16_t Sa_chanGetBufferReq (
 *          Sa_ChanSizeCfg_t *sizeCfg,  - Configuration information to be used in
 *                                        estimating the worst case buffer sizes
 *          int sizes[],                - Array of size requirements  
 *          int aligns[])               - Array of alignment requirements                              
 *
 *   1. Get sizes for all the security profiles in the build.
 *   2. Find the maximum of all the sizes.
 *   3. Assumptions:
 *      --All the memory is from heap.
 *      --There is no special requirement for memory.
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 *
 *****************************************************************************/
int16_t Sa_chanGetBufferReq (Sa_ChanSizeCfg_t *cfg, int sizes[], int aligns[])
{
  
  int size;
  int alignment;
  
  if(cfg == NULL)
    return(sa_ERR_PARAMS);
    
  alignment = max(8, cfg->cacheLineSize);  

  /* Get sizes based on the security protocol */
  salld_get_chan_inst_size(cfg->protocolType, &size);
  
  if(size == 0)
    return(sa_ERR_PARAMS);    
  
  sizes[0] = SALLD_ROUND_UP(size, alignment);
  
  if (cfg->ctrlBitMap & sa_SIZE_CONFIG_CREATE_SHADOW_INST)
  {
    sizes[0] *= 2;  /* Double the instance size to include the shadow instance */
  }
  
  aligns[0] = alignment;
  
  return(sa_ERR_OK); 
} /* Sa_chanGetBufferReq */


/******************************************************************************
 * FUNCTION PURPOSE: Create and initialize an SALLD channel instance 
 ******************************************************************************
 * DESCRIPTION: This function initialize an instance of SALLD channel and its
 *         corresponding instance structure based on channel configuration data 
 *         such as the security protocol and etc.
 *
 * int16_t Sa_chanCreate (
 *          Sa_Handle       handle,   - SALLD instance identifier. 
 *          Sa_ChanConfig_t *cfg      - Pointer to configuration structure
 *          void*           bases[]   - Array of the memory buffer base addresses
 *          Sa_ChanHandle *pChanHdl)  - Store the channel instance pointer
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *                 sa_ERR_NOMEM 
 *                 sa_ERR_INV_BUF
 *                 sa_ERR_INV_PROTO_TYPE 
 *
 *****************************************************************************/
int16_t Sa_chanCreate (Sa_Handle handle, Sa_ChanConfig_t *cfg, void* bases[], 
                       Sa_ChanHandle *pChanHdl)
{
  Sa_ProtocolCallTbl_t  **CallTable;
  salldObj_t *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  salldInst_t    *inst = (salldInst_t *)bases[SALLD_CHAN_BUFN];
  int   i, instSize, alignment;
  salldInst_t *shadowInst = NULL;
  int16_t retCode;

  /* Test instance pointer */
  if (sysInst == NULL || cfg == NULL || pChanHdl == NULL)
    return(sa_ERR_PARAMS);
    
  if (sysInst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(sysInst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }  
  /* Get sizes based on the security protocol */
  salld_get_chan_inst_size(cfg->sizeConfig.protocolType, &instSize);
  alignment = max(4, cfg->sizeConfig.cacheLineSize);  
  instSize = SALLD_ROUND_UP(instSize, alignment);
  if (cfg->sizeConfig.cacheLineSize)
  {
    /* Invalidate the Cache Contents */
    Sa_osalBeginMemAccess(inst, instSize);  
  }  
  
  memset(inst, 0, instSize);
  
  /* Allocate all dynamic buffers (base address != NULL ?)   */
  for (i = 0; i < SALLD_CHAN_NBUF; i++) {
    if (bases[i] == NULL) {
      return (sa_ERR_NOMEM);
    }
    inst->memBaseOffsets[i] = (uint32_t) sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr, bases[i]);
  }
  
  if (cfg->sizeConfig.ctrlBitMap & sa_SIZE_CONFIG_CREATE_SHADOW_INST)
  {
    /* Shadow instance is created just after system instance in the memory
       hence the shadowoffset = system offset + size of system structure */
    inst->shadowInstOffset = (uint32_t) inst->memBaseOffsets[SALLD_CHAN_BUFN] + instSize;       
    shadowInst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->shadowInstOffset));
    
    if (cfg->sizeConfig.cacheLineSize)
    {
        /* Invalidate the Cache Contents */
        Sa_osalBeginMemAccess(shadowInst, instSize);  
    }  
  } 

  /* modify the Security Profile to one of the ones in the call table */
  for (i =0, CallTable = (Sa_ProtocolCallTbl_t  **)&salld_callTblPtr[0]; *CallTable != NULL; CallTable++, i++)
  {
    if ((*CallTable)->secProtocolType == cfg->sizeConfig.protocolType)
    {
	    inst->protoTblIndex = i;
        break;
    }    
  }

  /* return an error if the profile type is not supported */
  if (*CallTable == NULL)
    return (sa_ERR_INV_PROTO_TYPE);
    
  /* Default Instance Initialization */
  inst->magic              = SALLD_INST_MAGIC_WORD;
  inst->ID                 = cfg->ID;
  inst->stateBitfield      = 0;    /* set SALLD state to CLOSED  */
  inst->instSize           = instSize;
  inst->ownerInstOffset    = sysInst->memBaseOffsets[SALLD_BUFN];
  SALLD_SET_STATE_SHARED(inst, (cfg->sizeConfig.cacheLineSize > 0));
  
  *pChanHdl = (Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr, inst);

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  
  /* Read the engine select information from salldHandle */
  if ((cfg->sizeConfig.protocolType == sa_PT_IPSEC_AH) || (cfg->sizeConfig.protocolType == sa_PT_IPSEC_ESP))
  {
    uint32_t mtCsKey;
    /* CRITICAL Section Start: The global instance is a shared resource which needs to be
     * protected from the following:
     *  a) Multiple Cores acccess
     */
    Sa_osalMtCsEnter(&mtCsKey);
      
    /* Invalidate the Cache Contents */
    Sa_osalBeginMemAccess(sysInst, sizeof(salldObj_t));
    
    inst->engSelect = salld_select_ipsec_engine(sysInst, cfg->engSelect);
    
    /* Writeback the instance updates */
    Sa_osalEndMemAccess(sysInst, sizeof(salldObj_t));  
  
    /* Critical Section End */
    Sa_osalMtCsExit(mtCsKey);
  
  }
  else
  {
    inst->engSelect = SALLD_ENG_SELECT_NA;
  }
  
#endif  
  
  /* Populate the shadow Instance which will be used at another processer with different Endian mode if required */
  if (shadowInst)
  {
	salldObj_t *sysShadowInst = (salldObj_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, sysInst->shadowInstOffset));

    if (!sysShadowInst)
    {
        return(sa_ERR_INV_HANDLE);
    }
  
    memset(shadowInst, 0, instSize);
    shadowInst->magic = salld_swiz32(SALLD_INST_MAGIC_WORD);
    shadowInst->ID = SALLD_SWIZ(inst->ID);
    shadowInst->stateBitfield = SALLD_SWIZ(inst->stateBitfield);
    shadowInst->instSize = SALLD_SWIZ(inst->instSize);
    shadowInst->shadowInstOffset = SALLD_SWIZ(inst->shadowInstOffset);
    shadowInst->protoTblIndex = SALLD_SWIZ(inst->protoTblIndex);
    shadowInst->updateCnt =  SALLD_SWIZ(inst->updateCnt);
    shadowInst->engSelect =  SALLD_SWIZ(inst->engSelect);
    shadowInst->ownerInstOffset = SALLD_SWIZ(sysInst->shadowInstOffset);
    
    /* Note: the memory buffers can not be used by the shadow processors since the memory free can only occur at the master core */
  }
  
  /* call the initialization function for the actual security profile type */
  retCode = (*salld_callTblPtr[inst->protoTblIndex]->chanInit) (inst, (void *)cfg);
  
  /* write back the channel specific instance */
  if (SALLD_TEST_STATE_SHARED(inst))
  {
    Sa_osalEndMemAccess(inst, inst->instSize);  
    
    if (shadowInst)
    {
        Sa_osalEndMemAccess(shadowInst, inst->instSize);
    }
  }  
  
  return(retCode);
  
} /*  Sa_chanCreate  */


/******************************************************************************
 * FUNCTION PURPOSE: Start an SALLD channel instance 
 ******************************************************************************
 * DESCRIPTION: This function starts an instance of SALLD channel at local
 *         core. It is assumed that this channel has been created and configured
 *         at another core.   
 *
 * int16_t Sa_chanStart (
 *   Sa_ChanHandle      handle)     - SALLD channel identifier
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS
 *
 * Note: This function should be called prior to other channel APIs
 *       such as Sa_chanSendData() and Sa_chanReceiveData() are invoked
 *****************************************************************************/
int16_t Sa_chanStart (Sa_Handle handle)
{
  salldInst_t *inst = (salldInst_t *) (sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));

  /* Invalidate the Cache Contents */
  Sa_osalBeginMemAccess(inst, sizeof(salldInst_t));  
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }  
  
  /* Invalidate the protocol-specific channel instance */
  if(SALLD_TEST_STATE_SHARED(inst))
    Sa_osalBeginMemAccess(inst, inst->instSize);  
  
  return (sa_ERR_OK);

} 


/******************************************************************************
 * FUNCTION PURPOSE:  Obtain the SA channelID 
 ******************************************************************************
 * DESCRIPTION: This function returns the SA channel ID associated with the 
 *      specified handle. 
 *
 *****************************************************************************/
uint32_t  Sa_chanGetID (Sa_ChanHandle handle)
{
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
    
  if ((inst == NULL) || (inst->magic != SALLD_INST_MAGIC_WORD))
  {
    return(0);
  }  
    
  return (inst->ID);
}

/******************************************************************************
 * FUNCTION PURPOSE: SALLD Channel Control function
 ******************************************************************************
 * DESCRIPTION: This function controls the operations of a channel instance of 
 *         SALLD. It is used to configure and/or re-configure the SALLD channel 
 *         with various control information. This function should be called 
 *         multiple times to configure and activate the SALLD channel during 
 *         call setup period. Then it is typically called to perform re-key opeartion 
 *         subsequently 
 *
 * int16_t  Sa_chanControl (
 *   Sa_ChanHandle      handle      - SALLD channel identifier
 *   Sa_ChanCtrlInfo_t  *ctrl)      - a pointer to control structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *                 sa_ERR_INV_PROTO_TYPE 
 *
 *****************************************************************************/
int16_t Sa_chanControl (Sa_ChanHandle handle, Sa_ChanCtrlInfo_t *ctrl)
{
  int16_t ret = sa_ERR_GEN;
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
  salldInst_t* shadowInst = NULL;
  
  /* Invalidate the protocol-specific channel instance */
  if (SALLD_TEST_STATE_SHARED(inst))
  {
    Sa_osalBeginMemAccess(inst, inst->instSize);
    
	/* non zero shadow instance offset indicates the presense of shadow instance */
    if(inst->shadowInstOffset)
	{
        shadowInst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->shadowInstOffset));
        Sa_osalBeginMemAccess(shadowInst, inst->instSize);
    }
  }  
    
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return (sa_ERR_INV_ENDIAN_MODE);
    else
        return (sa_ERR_INV_HANDLE);    
  }  

  if((ctrl->ctrlType == sa_CHAN_CTRL_GEN_CONFIG) 
              && (ctrl->ctrlInfo.gen.validBitfield & sa_CONTROLINFO_VALID_CTRL_BITMAP))
  {
    inst->stateBitfield &= ~sa_CONTROLINFO_CTRL_OP_MASK;
    inst->stateBitfield |= (ctrl->ctrlInfo.gen.ctrlBitfield & sa_CONTROLINFO_CTRL_OP_MASK);
  }
  
  /* Calls the appropriate salldXyzControl function */
  ret = (*salld_callTblPtr[inst->protoTblIndex]->chanControl)(inst, (void *)ctrl);
  
  /* write back the channel specific instance */
  if (SALLD_TEST_STATE_SHARED(inst))
  {
    Sa_osalEndMemAccess(inst, inst->instSize);  
    
    if(inst->shadowInstOffset)
        Sa_osalEndMemAccess(shadowInst, inst->instSize);
  }  
  
  return(ret);
} /* Sa_chanControl */

/******************************************************************************
 * FUNCTION PURPOSE:  SALLD Get Channel Stats 
 ******************************************************************************
 * DESCRIPTION: This function obtains SALLD channel protocol-specific statistics
 *
 *  int16_t  Sa_chanGetStats (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   uint16_t             flags       - various control flags
 *   void                 *stats)     - a pointer to protocol_specific statistics
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *                 sa_ERR_INV_PROTO_TYPE 
 *                 sa_ERR_STATS_UNAVAIL
 *
 ******************************************************************************/
int16_t  Sa_chanGetStats (Sa_ChanHandle handle, uint16_t flags, Sa_Stats_t *stats)
{
  int16_t ret = sa_ERR_INV_PROTO_TYPE;
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
  salldInst_t* shadowInst = NULL;
  
  /* Invalidate the protocol-specific channel instance */ 
  if (SALLD_TEST_STATE_SHARED(inst))
  {
    Sa_osalBeginMemAccess(inst, inst->instSize);
    
	/* non zero shadow instance offset indicates the presense of shadow instance */
    if(inst->shadowInstOffset)
	{
        shadowInst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->shadowInstOffset));
        Sa_osalBeginMemAccess(shadowInst, inst->instSize);
    }	
  }  
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return (sa_ERR_INV_ENDIAN_MODE);
    else
        return (sa_ERR_INV_HANDLE);    
  }  
  
  /* Calls the appropriate msuXyzGetStats function */
  ret = (*salld_callTblPtr[inst->protoTblIndex]->chanGetStats)(inst, flags, (void *)stats);
  
  /* write back the channel specific instance */
  if (SALLD_TEST_STATE_SHARED(inst))
  {
    Sa_osalEndMemAccess(inst, inst->instSize);  
    
    if(inst->shadowInstOffset)
        Sa_osalEndMemAccess(shadowInst, inst->instSize);
  }  
  
  return(ret);    
}

/******************************************************************************
 * FUNCTION PURPOSE:  Delete an SALLD channel instance 
 ******************************************************************************
 * DESCRIPTION: This function clears the SALLD chanel instance and returns the base  
 *         address of all buffers previously allocated for the SA LLD Channel instance.  
 *
 *  int16_t Sa_chanClose  (
 *            Sa_ChanHandle handle,   - SALLD channel identifier
 *            void*         bases[])  - Output array of the memory buffer base addresses
 *
 * Return values:  sa_ERR_OK
 *
 *
 ******************************************************************************/
int16_t Sa_chanClose (Sa_ChanHandle handle, void* bases[])
{
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  salldObj_t  *sysInst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->ownerInstOffset);
#endif  
  int16_t ret_code;
  int i;
  salldInst_t* shadowInst = NULL;

  /* Invalidate the protocol-specific channel instance */ 
  if (SALLD_TEST_STATE_SHARED(inst))
  {
    Sa_osalBeginMemAccess(inst, inst->instSize);
	
	/* non zero shadow instance offset indicates the presense of shadow instance */
    if(inst->shadowInstOffset)
	{
        shadowInst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->shadowInstOffset));
        Sa_osalBeginMemAccess(shadowInst, inst->instSize);
    }
  }  
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }  
  
  for (i = 0; i < SALLD_CHAN_NBUF; i++) {
    bases[i] = (void *) sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->memBaseOffsets[i]);  
  }
  
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  if (inst->engSelect <= 1)
  {
    uint32_t mtCsKey;
    /* CRITICAL Section Start: The global instance is a shared resource which needs to be
     * protected from the following:
     *  a) Multiple Cores acccess
     */
    Sa_osalMtCsEnter(&mtCsKey);
    
    /* Invalidate the Cache Contents */
    Sa_osalBeginMemAccess(sysInst, sizeof(salldObj_t));
    
    if (SALLD_GET_STATE_ENG_SEL_MODE(sysInst) == sa_EngSelMode_LOADBALANCED)
    {
        if(sysInst->engCounter[inst->engSelect] > 0)
            sysInst->engCounter[inst->engSelect]--;
    }
  
    /* Writeback the instance updates */
    Sa_osalEndMemAccess(sysInst, sizeof(salldObj_t));  
  
    /* Critical Section End */
    Sa_osalMtCsExit(mtCsKey);
  }
#endif  
  
  ret_code = (*salld_callTblPtr[inst->protoTblIndex]->chanClose)(inst);
  
  inst->stateBitfield = 0;  /* indicate SALLD instance is closed */
  inst->magic = 0;
  
  if (shadowInst)
  {
    shadowInst->magic = 0;
  }
  
  /* write back the channel specific instance */
  if (SALLD_TEST_STATE_SHARED(inst))
  {
    Sa_osalEndMemAccess(inst, inst->instSize);  
    
    if(inst->shadowInstOffset)
        Sa_osalEndMemAccess(shadowInst, inst->instSize);
  }  
  
  return (ret_code);

} /*  Sa_chanClose */

/******************************************************************************
 * FUNCTION PURPOSE:  SALLD Get SW Info 
 ******************************************************************************
 * DESCRIPTION: This function obtains SA-specific software information
 *
 *  int16_t  Sa_chanGetSwInfo  (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   uint16_t             dir         - packet directions
 *   Sa_SWInfo_t         *pSwInfo)    - a pointer to swInfo
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS
 *                 sa_ERR_UNSUPPORTED
 *                 sa_ERR_SWINFO_UNAVAIL
 *
 ******************************************************************************/
int16_t Sa_chanGetSwInfo (Sa_ChanHandle handle, uint16_t dir, Sa_SWInfo_t* pChanSwInfo)
{
  int16_t ret;
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }  
  
  /* Error Check */
  if (((!SALLD_TEST_STATE_TX_ON(inst)) && (dir == sa_PKT_DIR_TO_NETWORK)) ||
      ((!SALLD_TEST_STATE_RX_ON(inst)) && (dir == sa_PKT_DIR_FROM_NETWORK)))
  {
    return (sa_ERR_SWINFO_UNAVAIL);
  }
  
  /* Calls the appropriate msuXyzGetStats function */
  ret = (*salld_callTblPtr[inst->protoTblIndex]->chanGetSwInfo)(inst, dir, pChanSwInfo);
  
  return(ret);    
}

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain the SA shadow channel handle 
 ******************************************************************************
 * DESCRIPTION: This function returns the shadow channel handle. The shadow  
 *           channel handle is set to NULL if it does not exist.
 *
 *  int16_t  Sa_chanGetShadowHandle  (
 *   Sa_ChanHandle        handle      - SALLD channel identifier
 *   Sa_ChanHandle       *shandle)    - Pointer to shadow channel handle
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_INV_ENDIAN_MODE
 *                 sa_ERR_INV_HANDLE
 *
 *****************************************************************************/
int16_t  Sa_chanGetShadowHandle  (Sa_ChanHandle handle, Sa_ChanHandle *shandle)
{
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }  
  
  *shandle = (Sa_ChanHandle)(uintptr_t) inst->shadowInstOffset;  
  
  return(sa_ERR_OK);

}

#if !defined(NSS_LITE) && !defined(NSS_LITE2)

/* Replay related utility function */
/*
 *
 * Note that the window is defined by a base and a size. The base is the
 * lowest index value that can be considered for updating or checking,
 * while the highest acceptable value for checking is win_base+win_size-1.
 *
 * On updating, if the index value is outside the window and "newer" than
 * the window, then the value of win_base will be adjust so that the
 * index will "just fit" within the window.
 *
 *
 *                |                           |
 *                |<------- win_size -------->|
 *                |                           |
 *   -------------+---------------------------+-------------------------
 *                ^                          ^
 *   << Older     |                          |                  Newer >>
 *
 *            win_base                  Highest seq idx
 *                                      we will accept
 *
 *
 * The window "packet present" bits are arranged as a 32-bit uint array.
 * Each array element tracks 32 index values. The array is circular in
 * nature so that the top wraps back to the bottom. It is one uint larger
 * than the maximum window size, allowing for up to 32-bits of "spill-over"
 * as new indices are added.
 *
 * The value of "win_base" is the lowest packet index value that is still
 * in the window. Any packet coming before this base is not considered.
 * The position of the bit corresponding to the window base in the physical
 * bit mask array is determined by two variables; "win_mask_index" is the
 * index of the 32-bit word in the mask array that hold the bit value,
 * and "win_mask_bitoff" is the bit offset within that word.
 *
 * As the window slides, the flag bits are not moved. Instead, the view
 * base into the uint array is altered. This is done by incrementing the
 * value of "win_mask_bitoff" (this is always "win_base & 0x1F"), but it
 * is tracked separately since it also controls when "win_mask_index" is
 * incremented. Once the value of "win_mask_bitoff" is greater than 31,
 * the old array word pointed to by "win_mask_index" is cleared and the
 * value of "win_mask_index" is incremented. This puts the cleared value
 * onto the head of the bit mask, and it is then used for new flags at the
 * top of the window.
 *
 *
 *  |    32 bits     |    32 bits     |    32 bits     |    32 bits     |
 *  |     newest     |     oldest     |     older      |     newer      |
 *  +----------------+----------------+----------------+----------------+
 *                   ^         ^
 *                   |         |
 *             win_mask_index  |
 *                        Window Base
 *
 *   win_mask_bitoff |<------->|
 *
 *  --------->|                |<------------------- Valid Window -------
 *
 *
 *  
 *  The replay related alogritms is based on the concept of RFC4302 Appendix B
 *  It is highly optimized to reduce operation cycles and be PDSP friendly by
 *  Mike Denio.
 *
 *
 */


/*******************************************************************************
 *  Function:   salld_replay_init
 *******************************************************************************
 *  DESCRIPTION:
 *      This function is called to initialize the replay context, or to 
 *      re-initialize it if an index it entirely out of range.
 *  
 ******************************************************************************/
void salld_replay_init( salldWindow_t* pCtl, uint32_t winBase)
{
    /* Initialize the mask */
    memset( pCtl->winMask, 0, sizeof(pCtl->winMask));

    /* Setup the window to be based at the supplied sequence number */
    pCtl->winMaskIndex = 0;
    pCtl->winBase = winBase;

    /*
     * For simplicity, we align the mask set on the same 32-bit
     * alignment as the index value. This isn't really necessary,
     * but it makes the array values a little easier to read, and
     * doesn't have any affect on code size or performance.
     */
    pCtl->winMaskBitoff = winBase & 0x1f;
}

/*******************************************************************************
 *  Function:   salld_replay_check
 *******************************************************************************
 *  DESCRIPTION:
 *      This function is called to check to see if the packet is within the
 *      window and has not been previously seen.
 *
 ******************************************************************************/
SALLD_REPLAY_RC_T  
salld_replay_check(salldWindow_t* pCtl, uint32_t seqNum)
{
    uint32_t seqNumDiff,bitNumber,indexOffset,bitOffset;

    /* Do the subract first so we are immune to 32-bit wrap */
    seqNumDiff = seqNum - pCtl->winBase;

    /* If the delta is "negative", then this packet comes before our base. 
     * It is equivalent to (seqNumDiff > 2^31)
     * the receive packet is well outside the replay windom, drop it
     */
    if( seqNumDiff & (1UL<<31) )
        return SALLD_REPLAY_RC_OLD;

    /* See if the packet falls beyond our window */
    if( seqNumDiff >= pCtl->winSize )
        return SALLD_REPLAY_RC_NEW;

    bitNumber = seqNumDiff + pCtl->winMaskBitoff;

    bitOffset = bitNumber & 0x1F;
    indexOffset = pCtl->winMaskIndex + (bitNumber>>5);

    /* The bit mask can wrap */
    if( indexOffset >= SA_WIN_MASK_SIZE )
        indexOffset -= SA_WIN_MASK_SIZE;

    if( pCtl->winMask[indexOffset] & (1<<bitOffset) )
        return SALLD_REPLAY_RC_DUP;
    else
        return SALLD_REPLAY_RC_OK;
}

/*******************************************************************************
 *  Function:   salld_replay_update
 *******************************************************************************
 *  DESCRIPTION:
 *      This function is called to update the replay state.
 *  
 *  Note: We may need to handle the sequence number wraparound
 *
 ******************************************************************************/
void  
salld_replay_update(salldWindow_t* pCtl, uint32_t seqNum)
{
    uint32_t seqNumDiff,bitNumber,indexOffset,bitOffset;

    /* Do the subract first so we are immune to 32-bit wrap */
    seqNumDiff = seqNum - pCtl->winBase;

    /* If the delta is "negative", then this packet comes before our base. 
     * It is equivalent to (seqNumDiff > 2^31)
     * the receive packet is well outside the replay windom, drop it
     */
    if( seqNumDiff & (1UL<<31) )
        return;

    // See if the packet falls beyond our window
    if( seqNumDiff >= pCtl->winSize )
    {
        uint32_t slide_delta;

        slide_delta = seqNumDiff-pCtl->winSize+1;
        
        /*
         * Check the win base wrap around condition 
         */
        if((pCtl->winBase + slide_delta) < pCtl->winBase)
            pCtl->winBaseHi++;     

        /*
         * If we have to slide more than the window size, then its
         * faster to just re-init.
         */
        if( slide_delta >= pCtl->winSize )
            salld_replay_init( pCtl, seqNum-pCtl->winSize+1 );
        else
        {
            pCtl->winBase += slide_delta;
            pCtl->winMaskBitoff += slide_delta;

            /* 
             * If we're still in the same 32-bit word, then we're done, but if
             * we moved out of the 32-bit word, we need to recover any empty
             * word for re-use. This means moving the mask index and zeroing
             * out the value.
             */
            while( pCtl->winMaskBitoff>=32 )
            {
                pCtl->winMaskBitoff -= 32;
                pCtl->winMask[pCtl->winMaskIndex] = 0;
                if( ++pCtl->winMaskIndex >= SA_WIN_MASK_SIZE )
                    pCtl->winMaskIndex = 0;
            }
        }

        /* We know we're alway setting the bit at offset "win_size-1" */
        seqNumDiff = pCtl->winSize - 1;
    }

    /* We now know this packet is inside the window range */
    bitNumber = seqNumDiff + pCtl->winMaskBitoff;

    bitOffset   = bitNumber & 0x1F;
    indexOffset = pCtl->winMaskIndex + (bitNumber>>5);

    /* The bit mask can wrap */
    if( indexOffset >= SA_WIN_MASK_SIZE )
        indexOffset -= SA_WIN_MASK_SIZE;

    pCtl->winMask[indexOffset] |= (1<<bitOffset);
}
/*******************************************************************************
 *  Function:   salld_replay_check_and_update
 *******************************************************************************
 *  DESCRIPTION:
 *      This function is a combination of checking the index value and then 
 *      updating the bit mask array.
 *
 *      If the packet status is RC_OK or RC_NEW, then the mask array is
 *      updated. When the status is RC_NEW, the window is also shifted
 *      so that the new value is just within the window.
 *
 *  Return Value:
 *     RC_OLD - Packet is older than the window
 *     RC_DUP - Packet is inside the window, but has been seen before
 *     RC_OK  - Packet in inside the window and has not been seen
 *     RC_NEW - Packet is newer than the window
 *
 ******************************************************************************/
SALLD_REPLAY_RC_T  
salld_replay_check_and_update(salldWindow_t* pCtl, uint32_t seqNum)
{
    uint32_t seqNumDiff,bitNumber,indexOffset,bitOffset;
    SALLD_REPLAY_RC_T  rc = SALLD_REPLAY_RC_OK;

    /* Do the subract first so we are immune to 32-bit wrap */
    seqNumDiff = seqNum - pCtl->winBase;

    /* If the delta is "negative", then this packet comes before our base. 
     * It is equivalent to (seqNumDiff > 2^31)
     * the receive packet is well outside the replay windom, drop it
     */
    if( seqNumDiff & (1UL<<31) )
        return SALLD_REPLAY_RC_OLD;

    /* See if the packet falls beyond our window */
    if( seqNumDiff >= pCtl->winSize )
    {
        uint32_t slideDelta;

        slideDelta = seqNumDiff-pCtl->winSize+1;

        /*
         * Check the win base wrap around condition 
         */
        if((pCtl->winBase + slideDelta) < pCtl->winBase)
            pCtl->winBaseHi++;     

        /*   
         * If we have to slide more than the window size, then its
         * faster to just re-init.
         */
        if( slideDelta >= pCtl->winSize )
            salld_replay_init(pCtl, seqNum-pCtl->winSize+1 );
        else
        {
            pCtl->winBase += slideDelta;
            pCtl->winMaskBitoff += slideDelta;

            /*
             * If we're still in the same 32-bit word, then we're done, but if
             * we moved out of the 32-bit word, we need to recover any empty
             * word for re-use. This means moving the mask index and zeroing
             * out the value.
             */
            while( pCtl->winMaskBitoff>=32 )
            {
                pCtl->winMaskBitoff -= 32;
                pCtl->winMask[pCtl->winMaskIndex] = 0;
                if( ++pCtl->winMaskIndex == SA_WIN_MASK_SIZE )
                    pCtl->winMaskIndex = 0;
            }
        }

        /* We know we're alway setting the bit at offset "winSize-1" */
        seqNumDiff = pCtl->winSize - 1;

        /* The state here is SALLD_REPLAY_RC_NEW */
        rc = SALLD_REPLAY_RC_NEW;
    }

    /* We now know this packet is inside the window range */
    bitNumber = seqNumDiff + pCtl->winMaskBitoff;

    bitOffset   = bitNumber & 0x1F;
    indexOffset = pCtl->winMaskIndex + (bitNumber>>5);

    /* The bit mask can wrap */
    if( indexOffset >= SA_WIN_MASK_SIZE )
        indexOffset -= SA_WIN_MASK_SIZE;

    if( pCtl->winMask[indexOffset] & (1<<bitOffset) )
        return SALLD_REPLAY_RC_DUP;

    pCtl->winMask[indexOffset] |= (1<<bitOffset);
    return rc;
}

/*******************************************************************************
 *  Function:   salld_replay_is_all_received
 *******************************************************************************
 *  DESCRIPTION:
 *      This function is called to check whether all packets up to the specified
 *      sequence number has been received
 *
 *   Return: TRUE  - all packets have been received
 *           FALSE - Otherwise
 *
 ******************************************************************************/
uint16_t salld_replay_is_all_received(salldWindow_t* pCtl, uint32_t seqNum)
{
    uint32_t seqNumDiff,bitNumber,indexOffset,bitOffset, mask, mask1, mask2;
    uint16_t index;
    
    if (pCtl->winBase >=  seqNum)
    {
        /*
         * The winBase has moved across the sequence number.
         * No packets with lower sequence number is allowed
         */
        return TRUE; 
    }

    seqNumDiff = seqNum - pCtl->winBase;

    bitNumber = seqNumDiff + pCtl->winMaskBitoff;
    bitOffset = bitNumber & 0x1F;
    indexOffset = pCtl->winMaskIndex + (bitNumber>>5);
    
    /* The bit mask can wrap */
    if( indexOffset >= SA_WIN_MASK_SIZE )
        indexOffset -= SA_WIN_MASK_SIZE;
    
    mask1 = (1<<(bitOffset + 1)) - 1;
    mask2 = (1<<(pCtl->winMaskBitoff + 1)) - 1;
    
    if (indexOffset == pCtl->winMaskIndex)
    {
        mask = mask1 ^ mask2;
        
        if((pCtl->winMask[indexOffset] & mask) == mask)
            return TRUE;
    }
    else
    {
        mask = 0xFFFFFFFF ^ mask2;
        if((pCtl->winMask[pCtl->winMaskIndex] & mask) != mask)    
            return FALSE;
        
        index = pCtl->winMaskIndex + 1;
        
        if( index >= SA_WIN_MASK_SIZE )
            index -= SA_WIN_MASK_SIZE;
            
        if (index > indexOffset)
        {
            while (index < (SA_WIN_MASK_SIZE - 1))
            {
                if(pCtl->winMask[index % SA_WIN_MASK_SIZE] != 0xFFFFFFFF)
                    return FALSE;
                index++;    
            } 
            index = 0;   
        }    
        
        while (index < indexOffset)
        {
            if(pCtl->winMask[index % SA_WIN_MASK_SIZE] != 0xFFFFFFFF)
                return FALSE;
            index++;    
        }    
            
        if((pCtl->winMask[index % SA_WIN_MASK_SIZE] & mask1) == mask1)
            return TRUE;
    }
    
    return FALSE;

}

/******************************************************************************
 * Function:    ones_complement_chksum
 ******************************************************************************
 * Description: Calculate an Internet style one's complement checksum
 *
 ******************************************************************************/
static inline uint16_t salld_ones_complement_chksum ( uint16_t *p_data, uint16_t len )
{
  uint32_t  chksum = 0;

  while (len > 0)
  {
    chksum += (uint32_t)pktRead16bits_m ((tword *)p_data,0);
    p_data++;
    len--;
  }
  chksum = (chksum >> 16) + (chksum & 0xFFFF); /* add in carry   */
  chksum += (chksum >> 16);                    /* maybe one more */
  return (uint16_t)chksum;

} /* end of salld_ones_complement_chksum() */

/**************************************************************************************
 * FUNCTION PURPOSE: Compute and insert the ipv4 checksum
 **************************************************************************************
 * DESCRIPTION: Compute and insert the ipv4 checksum
 **************************************************************************************/
void salld_set_ipv4_chksum (tword *data)
{
  uint16_t hdr_len; /* Hdr Length in 16-bit word */
  uint16_t ip_hdr_chksum;

  /* calculate IP header length */
  hdr_len =  (pktRead8bits_m(data, IPV4_BYTE_OFFSET_VER_HLEN) & IPV4_HLEN_MASK) << 1;
  
  pktWrite16bits_m(data, IPV4_BYTE_OFFSET_HDR_CHKSUM, 0);
  

  /* Length for IP Checksum calculation  should be in terms of 16-bit twords only */
  ip_hdr_chksum = salld_ones_complement_chksum ((uint16_t *)data, hdr_len);
  
  pktWrite16bits_m(data, IPV4_BYTE_OFFSET_HDR_CHKSUM, ~ip_hdr_chksum);

} /* salld_set_ipv4_chksum */

#endif

/****************************************************************************
 * FUNCTION PURPOSE: SALLD update security context information at the shadow 
 *                   instance  
 ****************************************************************************
 * DESCRIPTION: Pouplate and update the security context information at the
 *              shadow instance which is used by another processor running in 
 *              opposite Endian mode.
 *
 *  void salld_update_shadow_scInfo(
 *            Sa_ScReqInfo_t*  srcInfo,         -> Pointer to the source
 *            Sa_ScReqInfo_t*  destInfo         -> Pointer to the destination
 *                                 )
 * Return values:  None
 *
 ***************************************************************************/
void salld_update_shadow_scInfo(Sa_ScReqInfo_t *srcInfo, Sa_ScReqInfo_t *destInfo)
{
    destInfo->scSize = SALLD_SWIZ(srcInfo->scSize);
    destInfo->scID   = SALLD_SWIZ(srcInfo->scID);
    destInfo->scBuf  = (uintptr_t )SALLD_SWIZ((uintptr_t)srcInfo->scBuf);
}

/****************************************************************************
 * FUNCTION PURPOSE: SALLD update software information at the shadow 
 *                   instance  
 ****************************************************************************
 * DESCRIPTION: Pouplate and update the software information at the
 *              shadow instance which is used by another processor running in 
 *              opposite Endian mode.
 *
 *  void salld_update_shadow_swInfo(
 *            Sa_SWInfo_t*  srcInfo,         -> Pointer to the source
 *            Sa_SWInfo_t*  destInfo         -> Pointer to the destination
 *                                 )
 * Return values:  None
 *
 ***************************************************************************/
void salld_update_shadow_swInfo(Sa_SWInfo_t *srcInfo, Sa_SWInfo_t *destInfo)
{
    int i;
    
    destInfo->size = SALLD_SWIZ(srcInfo->size);
    
    for (i = 0; i < srcInfo->size; i++)
    {
        destInfo->swInfo[i]   = SALLD_SWIZ(srcInfo->swInfo[i]);
    }
} 

/****************************************************************************
 * FUNCTION PURPOSE: SALLD update destination information at the shadow 
 *                   instance  
 ****************************************************************************
 * DESCRIPTION: Pouplate and update the destination information at the
 *              shadow instance which is used by another processor running in 
 *              opposite Endian mode.
 *
 *  void salld_update_shadow_destInfo(
 *            Sa_DestInfo_t*  srcInfo,         -> Pointer to the source
 *            Sa_DestInfo_t*  destInfo         -> Pointer to the destination
 *                                 )
 * Return values:  None
 *
 ***************************************************************************/
void salld_update_shadow_destInfo(Sa_DestInfo_t *srcInfo, Sa_DestInfo_t *destInfo)
{
    destInfo->flowID  = SALLD_SWIZ(srcInfo->flowID);
    destInfo->queueID = SALLD_SWIZ(srcInfo->queueID);
    destInfo->swInfo0 = SALLD_SWIZ(srcInfo->swInfo0);
    destInfo->swInfo1 = SALLD_SWIZ(srcInfo->swInfo1);
}     

/* nothing past this point */
