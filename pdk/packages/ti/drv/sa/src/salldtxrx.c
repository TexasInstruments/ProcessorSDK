/******************************************************************************
 * FILE PURPOSE:  Packet Processing routines for SA LLD
 ******************************************************************************
 * FILE NAME:   salldtxrx.c  
 *
 * DESCRIPTION: The file contains impemenation of the SALLD Packet Processing
 *              routines
 *
 * FUNCTION               DESCRIPTION
 * ========               ===========
 * salldGetSizes()        Obtains memory requirements for an instance of SALLD
 * Sa_chanInit()        Creates an instance of SALLD
 *
 *
 * REVISION HISTORY:
 *
 *
 * (C) Copyright 2009-2018, Texas Instruments Inc.
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
#include "salldloc.h"

/******************************************************************************
 * FUNCTION PURPOSE: SALLD TX Processing Function
 ******************************************************************************
 * DESCRIPTION: This function processes the data packet to the networks. It 
 *          performs protocol-specific operations to prepare the data packets 
 *          to be encrypted and/or authenticated by the SA.
 *          It also performs the actual encryption and/or authentication in the 
 *          SW-only mode
 *
 *   int16_t  Sa_chanSendData ( 
 *            Sa_ChanHandle handle,       - SALLD channel identifier (instance) 
 *            Sa_PktInfo_t  *pktInfo      - packet pointer  
 *            uint16_t            claer)
 *    Returns sa_ERR_OK or proper error ID.
 *
 *****************************************************************************/
int16_t  Sa_chanSendData (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo, uint16_t clear)
{
  int16_t ret = sa_ERR_GEN;		
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return (sa_ERR_INV_HANDLE);    
  }  
  
  if(salld_callTblPtr[inst->protoTblIndex]->chanSendData)
  {
    if(!(inst->stateBitfield & sa_CONTROLINFO_CTRL_TX_ON))
      return (sa_ERR_OK);

    /* Calls the appropriate msuXyzEncrypt function */
	ret = (*salld_callTblPtr[inst->protoTblIndex]->chanSendData)((void *)inst, pktInfo, clear);
  }
  
  return(ret);

} /* Sa_chanSendData */

/******************************************************************************
 * FUNCTION PURPOSE: SALLD RX Processing Function
 ******************************************************************************
 * DESCRIPTION: This function processes packets received from the network. It 
 *         performs protocol-specific post-SA operations on the decrypted and/or 
 *         integrity-verified data packet. It also performs the actual decryption
 *         /authentication operation in the SW-only mode. 
 *
 *    int16_t  Sa_chanReceiveData(
 *            Sa_ChanHandle handle,       - SALLD channel identifier (instance) 
 *            Sa_PktInfo_t  *pktInfo)     - packet pointer  
 *
 *    Returns sa_ERR_OK or proper error ID.
 *
 *****************************************************************************/
int16_t  Sa_chanReceiveData (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo)
{
  int16_t ret = sa_ERR_GEN;		
  salldInst_t *inst = (salldInst_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return (sa_ERR_INV_ENDIAN_MODE);
    else
        return ( sa_ERR_INV_HANDLE);    
  }  
                        
  if(salld_callTblPtr[inst->protoTblIndex]->chanReceiveData)
  {
    /* If Channel is closed, no decryption */
    if(!(inst->stateBitfield & sa_CONTROLINFO_CTRL_RX_ON) && (salld_callTblPtr[inst->protoTblIndex]->secProtocolType != sa_PT_NULL))
      return (sa_ERR_OK);

    /* Calls the appropriate msuXyzDecrypt function */
	ret = (*salld_callTblPtr[inst->protoTblIndex]->chanReceiveData)((void *)inst,pktInfo);
  }
  
  return(ret);

} /* chanReceiveData */

/******************************************************************************
 * FUNCTION PURPOSE: Send Null Packet 
 ******************************************************************************
 * DESCRIPTION: This function format and send a null packet to inform us to
 *         release the specific security context 
 *
 *    void  salld_send_null_pkt(
 *            Sa_ChanHandle handle,       - SALLD channel identifier (instance) 
 *            Sa_DestInfo_t*  pDestInfo   - Pointer to the Destination Info
 *            Sa_SWInfo_t* pSwInfo        - pointer to the SW info which contains
 *                                            the Security context information  
 *            uint8_t        flags)         - various control flags
 *
 *****************************************************************************/
void salld_send_null_pkt (Sa_ChanHandle handle, Sa_DestInfo_t* pDestInfo, 
                          Sa_SWInfo_t* pSwInfo, uint8_t flags)
{

    Sa_PktInfo_t pktInfo;

    /* Prepare and send Null packet */
    memset(&pktInfo, 0, sizeof(Sa_PktInfo_t));
    pktInfo.validBitMap = sa_PKT_INFO_VALID_SW_INFO;
    pktInfo.swInfo = *pSwInfo;
#if !defined(NSS_LITE)
    SA_SW_SET_ENG_ID(pktInfo.swInfo.swInfo[0], SALLD_CMDL_ENGINE_ID_OUTPORT2);
#else
    SA_SW_SET_ENG_ID(pktInfo.swInfo.swInfo[0], SALLD_CMDL_ENGINE_ID_OUTPORT1);
#endif
    
#if defined(NSS_LITE2)
    /* Evict, Tear and No-Payload flags are used to override
     * the default behavior of context cache module. Nopayload
     * flag is a legacy control bit that is no longer supported 
     * and must be set to 0. */
    SA_SW_SET_FLAG_NOPAYLOAD(pktInfo.swInfo.swInfo[0], 0);
    SA_SW_SET_CMDLB_PRESENT(pktInfo.swInfo.swInfo[0], 0);
    if(flags & SA_SC_FLAGS_TEAR)  SA_SW_SET_FLAG_TEAR(pktInfo.swInfo.swInfo[0], 1);
    if(flags & SA_SC_FLAGS_EVICT) SA_SW_SET_FLAG_EVICT(pktInfo.swInfo.swInfo[0], 1);
    pktInfo.swInfo.size = 3;
    SA_SW_SET_DEST_INFO_PRESENT(pktInfo.swInfo.swInfo[0], 1);
#else   
    SA_SW_SET_FLAG_NOPAYLOAD(pktInfo.swInfo.swInfo[0], 1); 

    SA_SW_SET_CMDLB_PRESENT(pktInfo.swInfo.swInfo[0], 0); 
    if(flags & SA_SC_FLAGS_TEAR)  SA_SW_SET_FLAG_TEAR(pktInfo.swInfo.swInfo[0], 1);
    if(flags & SA_SC_FLAGS_EVICT) SA_SW_SET_FLAG_EVICT(pktInfo.swInfo.swInfo[0], 1);
    
#if defined(NSS_LITE)
    pktInfo.swInfo.swInfo[0] |= 0x80000000UL;
#endif
  
    if (pDestInfo)
    {
        pktInfo.swInfo.size = 3;
        SA_SW_SET_DEST_INFO_PRESENT(pktInfo.swInfo.swInfo[0], 1);
        pktInfo.swInfo.swInfo[2] = SA_FORM_SW2(pDestInfo->queueID,
                                               pDestInfo->flowID,
                                               0);
    }
    else
    {
        pktInfo.swInfo.size = 2;
        SA_SW_SET_DEST_INFO_PRESENT(pktInfo.swInfo.swInfo[0], 0);
    
    }
#endif  
  salldLObj.callOutFuncs.ChanSendNullPkt((Sa_ChanHandle)sa_CONV_ADDR_TO_OFFSET(salldLObj.instPoolBaseAddr,handle), &pktInfo);

}                                   

/* nothing past this point */
