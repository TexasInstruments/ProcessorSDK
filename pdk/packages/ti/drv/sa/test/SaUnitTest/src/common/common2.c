/*
 *
 * Copyright (C) 2010-2020 Texas Instruments Incorporated - http://www.ti.com/
 * 
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



/* Common test utilities */
#include "unittest.h"

void testCommonSendNullPkt(tFramework_t *tf, uint32_t *swInfo)
{
    CSL_UdmapCppi5HMPD  *pDesc;
    uint32_t             pTsInfo, pSwInfo0, pSwInfo1, pSwInfo2;
    uintptr_t            cmdLblAddr;
    uint32_t             psWord;
    uint32_t             cmdLbSize = 4;
    extern void sauBookKeepTx(void);

    pDesc = testCommonGetBuffer(tf, 0);

    CSL_udmapCppi5SetDescType(pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST);
#if defined (DMA_TYPE_LCDMA)
#else
    CSL_udmapCppi5SetReturnPolicy( pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                       CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                       tf->txComplRingNum);
#endif
    /* Update EPIB information */
    CSL_udmapCppi5SetEpiDataPresent((void*) pDesc, EPI_PRESENT);
    CSL_udmapCppi5RdEpiData((void *) pDesc, &pTsInfo, &pSwInfo0, &pSwInfo1, &pSwInfo2);
    /* UPdate swInfo */
    pSwInfo0 = swInfo[0];
    pSwInfo1 = swInfo[1];
    pSwInfo2 = swInfo[2];
    CSL_udmapCppi5WrEpiData((void *) pDesc, pTsInfo, pSwInfo0, pSwInfo1, pSwInfo2);

    CSL_udmapCppi5SetPktLen((void *) pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST, 0);
    /* Attach the command label in PS data */
    CSL_udmapCppi5SetPsDataLoc((void *) pDesc, CSL_UDMAP_CPPI5_PD_DESCINFO_PSINFO_VAL_IN_DESC);
    cmdLblAddr = CSL_udmapCppi5GetPsDataAddr((void*) pDesc, 0, EPI_PRESENT);
    
    /* Update the first word of PS info words */
    psWord = tf->rxComplRingNum << 16;
    
    /* Copy ps info */
    memcpy ((void *) cmdLblAddr, &psWord, cmdLbSize);
    /* First word of PS INFO */
    CSL_udmapCppi5SetPsDataLen((void *) pDesc, cmdLbSize);
    
    RingPush(tf->gTxRingHandle, 0, (physptr_t) pDesc);
    sauBookKeepTx();

}

/* Request a buffer descriptor with the required buffer size */
CSL_UdmapCppi5HMPD*  testCommonGetBuffer(tFramework_t *tf, Int size)
{
   uint32_t sz = (uint32_t) size;
   FW_CPPI_DESC_T  *pCppiDesc;
   CSL_UdmapCppi5HMPD *hd = (CSL_UdmapCppi5HMPD *) NULL;

  /* Go through the TX Return queue which is where the descriptor is re-cycled after it is transmitted on the wire
     get the buffer and return to the application/stack */
  /* Get a free descriptor from the port free tx queue we setup during initialization. */
     if (tFramework.txReadyDescs != NULL)  {
       pCppiDesc = tFramework.txReadyDescs;
     }
     else
     {
       pCppiDesc = (FW_CPPI_DESC_T *) NULL;
     }

     if (pCppiDesc != NULL) {
#if defined(DMA_TYPE_LCDMA)
        if (sz <= pCppiDesc->hostDesc.bufInfo1) {
#else
        if (sz <= pCppiDesc->hostDesc.orgBufLen) {
#endif
          hd = &pCppiDesc->hostDesc;
          tFramework.txReadyDescs = (FW_CPPI_DESC_T *) (uintptr_t) pCppiDesc->nextPtr;
        }
        else {
#ifdef __LINUX_USER_SPACE
#else
         SALog("Buffer Size ( %d) exceeds the desc orig buffer size (%d) \n", sz, pCppiDesc->hostDesc.orgBufLen);
#endif
         System_flush();
          /* Buffer Size exceeds */
         salld_sim_halt();
        }
    }
    else {
#ifdef __LINUX_USER_SPACE
#else
        SALog("No Ready Desc available \n");
#endif
        System_flush();
        /* No Ready Desc available */
        salld_sim_halt();
    }
    return (hd);
}


/* Reset a buffer descriptor with linked buffer and place on the correct free buffer with 
 * descriptor queue */
Int testCommonRecycleLBDesc (tFramework_t *tf, CSL_UdmapCppi5HMPD *hd)
{
    uint32_t    pktLen;
      /* push to receive fdq */
    hd  = (CSL_UdmapCppi5HMPD * )Osal_PhysToVirt (hd);

    /* Push descriptor to Rx free descriptor queue */
    if (hd != NULL) {
      pktLen = CSL_udmapCppi5GetPktLen((void *) hd);
      RingPush (tFramework.gRxFreeRingHandle, pktLen,(physptr_t )hd);
    }

    return (0);
}

/* Reset a buffer descriptor with Host linked buffer and place on the correct free buffer with 
 * descriptor queue */
Int testCommonRecycleHostLBDesc (tFramework_t *tf, CSL_UdmapCppi5HMPD *hd)
{
    return (0);
}

/* Search the receive data packet queue for received data packets. Remain in 
 * this function until all buffers are restored to their respective queues */
void testDispPkts (CSL_UdmapCppi5HMPD *hd)
{
    uint32_t  swInfo[sa_MAX_SW_INFO_SIZE];
    uint32_t  pTsInfo;
    uint8_t   *psInfo;
    uint32_t  infoLen;
    uint8_t   *bufPtr = (uint8_t *)(uintptr_t)NULL;
    uint32_t  bufLen = 0;
    uint8_t   errFlags;
    int32_t   index = 0, ret;
    uint64_t  psDataAddr;
    char      msg_swInfo[80]  = "Software Info: 0x%08x, 0x%08x, 0x%08x \n";
    char      msg_tsInfo[80]  = "TimeStamp Info: 0x%08x \n";
    char      msg1_psInfo[80] = "PS Info[%d] = 0x%08x\n";
    char      msg2_psInfo[80] = "PS Info[%d] (remaing %d bytes)= 0x%08x\n";
    char      msg_pkt_err[80] = "Packet Error with error code %d\n";
    char      msg_pktSize[80] = "Packet of size %d received at 0x%04x\n";
   /* Dispaly Software Info */
   //Cppi_getSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t **)&swInfo);
   /* CSL_cppi5GetSoftwareInfo(CSL_CPPI5_DESCRIPTOR_TYPE_HOST, (uintptr_t *) hd, (uint8_t **)&swInfo); */
   ret = CSL_udmapCppi5RdEpiData((void *) hd, &pTsInfo, &swInfo[0],&swInfo[1],&swInfo[2]);

   if (ret == 0) {
     salld_sim_print(msg_swInfo, swInfo[0], swInfo[1], swInfo[2]);
     salld_sim_print(msg_tsInfo, pTsInfo);
   }
   
   /* Display PS Info if exists */
   /* CSL_cppi5GetPSData (CSL_CPPI5_DESCRIPTOR_TYPE_HOST, (uintptr_t *) hd, (uint8_t **)&psInfo, &infoLen); */
   psDataAddr = CSL_udmapCppi5GetPsDataAddr((void*) hd, 0, 1);
   infoLen    = CSL_udmapCppi5GetPsDataLen((void *) hd);
#if defined (BUILD_MPU1_0)
   psInfo     = (uint8_t *) psDataAddr;
#else
   psInfo     = (uint8_t *) (uint32_t) psDataAddr;
#endif

   #if !SA_GEN_TEST_VECTOR
   
   while (infoLen >= 4)
   {
    salld_sim_print(msg1_psInfo, index, psInfo[index]);
    index++;
    infoLen -= 4;
   }
   
   if (infoLen)
   {
    salld_sim_print(msg2_psInfo, index, infoLen, psInfo[index]);
   }
   
   #endif
   
            
   if ((errFlags = CSL_udmapCppi5GetErrorFlags((void *)hd)))
   {
    salld_sim_print(msg_pkt_err, errFlags);
   }  

   salld_sim_print(msg_pktSize, bufLen, bufPtr);
   
   #if SA_GEN_TEST_VECTOR
   utilOutputPkt(&tFramework, swInfo, infoLen, psInfo,
                  bufLen, bufPtr, FALSE);
   #endif

}

/*
 * Set the test status before executing the test framework. This allows to
 * choose whether to skip the test (in case device type does not support) or
 * preemptively fail and report the failure rather than crashing the test.
 */
int testCommonSetTestStatus(void (*fxn)(void*, void*), saTestStatus_t status)
{
    int ret = FALSE;
    int i;

    for (i = 0; saTestList[i].testFunction != NULL; i++ )  {
        if (saTestList[i].testFunction == fxn)
        {
            saTestList[i].testStatus = status;
            ret = TRUE;
        }
    }

    return ret;
}

/*
 * Get the test status. This gives the framework to decide whether to take
 * action if a test has been bypassed.
 */
saTestStatus_t testCommonGetTestStatus(void (*fxn)(void*, void*))
{
    saTestStatus_t status;
    int i;

    for (i = 0; saTestList[i].testFunction != NULL; i++ )  {
        if (saTestList[i].testFunction == fxn)
        {
            status = saTestList[i].testStatus;
        }
    }

    return status;
}

/* nothing past this point */

