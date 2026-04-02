/*
 *
 * Copyright (C) 2010-2013 Texas Instruments Incorporated - http://www.ti.com/ 
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



#include "unittest.h"
#include "salldsim/salldcfg.h"

/* Data Mode test.
 * This test performs a basic Data Mode test:
 * - Open and Configure a SALLD channel in Data mode
 * - Execute the test loop until all packets are processed
 * - Read the input IPSEC ESP packet
 * - Allocate buffer based on the packet size
 * - Fill in the required paylaod and other information in the packet info data structure
 * - Invoke the SALLD APIs to perform data mode processing 
 * - Prepare the Host HD for this packet
 *   - copy software info words
 *   - copy command label into the PS info 
 * - Send a packet to the SA queue.
 * - Wait and pop the processed packet from the received queue
 * - End of the test loop
 * - Query the SALLD channel statistics
 * - Close the SALLD channel
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */
 

/* SALLD channel configurations */                            
/* Data Mode */
static salldTestConfig_t saTestCfgDataMode =        
        {            
            0,                      /* channel index */
            sa_PT_NULL,             /* protocolType */
            sa_CipherMode_AES_CTR,  /* cipherMode */
            sa_AuthMode_HMAC_SHA1,  /* authMode */
            64,                     /* relayWinSize */
            {                       /* openCfg */
                sa_CONTROLINFO_CTRL_TX_ON      /* ctrlBitMap */
            },
            {                      /* destInfo */
                {
                    0,             /* ctrlBitfield */
#if defined(NSS_LITE2)
                    0, //TF_RXCHAN_FLOWID,
                    0, //TF_RING_RXQ,
#else
                    0,                      /* Flow ID */
                    DEST_QUEUE_PKT_RECV,    /* Queue ID (PA PDSP4) or DEST_QUEUE_PKT_RECV*/ 
#endif
                    0x55500000,   /* swInf0 */
                    0x00005550    /* swInf1 */
                     
                },
                {
                    0,           /* ctrlBitfield */
                    0,           /* Flow ID */
                    0,           /* Queue ID */  
                    0,           /* swInf0 */
                    0            /* swInf1 */
                },
            },
        };

#if defined (NSS_LITE2)
void sauBookKeepTx(void);
void sauBookKeepTx(void)
{
  FW_CPPI_DESC_T *pCppiDesc;

  /* return queue processing begin */
  do
  {
      /* Need to free up the FW_PKT_DESC_T back to the application */
      pCppiDesc = (FW_CPPI_DESC_T *) NULL;

      /* Go through the TX Return queue which is where the descriptor is re-cycled after it is transmitted on the wire
          get the buffer which is this case is a FW_PKT_DESC_T and return it to the application/stack */
      RingPop(tFramework.gTxComplRingHandle, &pCppiDesc);

      if (pCppiDesc)  {
         pCppiDesc->nextPtr =(uint64_t) (uintptr_t) tFramework.txReadyDescs;
         tFramework.txReadyDescs = pCppiDesc;
      }
  } while (pCppiDesc != NULL);
  /* return queue processing end */
}
static void sauBookKeepRx(void);
static void sauBookKeepRx(void)
{
  FW_CPPI_DESC_T *pCppiDesc;
  CSL_UdmapCppi5HMPD *hd;

  /* return queue processing begin */
  do
  {
      /* Need to free up the FW_PKT_DESC_T back to the application */
      pCppiDesc = (FW_CPPI_DESC_T *) NULL;
      hd        = (CSL_UdmapCppi5HMPD *) NULL;

      /* Go through the TX Return queue which is where the descriptor is re-cycled after it is transmitted on the wire
         get the buffer which is this case is a FW_PKT_DESC_T and return it to the application/stack */
      RingPop(tFramework.gRxRingHandle, &pCppiDesc);

      /* push to receive fdq */
      if (pCppiDesc != (FW_CPPI_DESC_T *) NULL)
      {
          hd  = &pCppiDesc->hostDesc;
          hd  = (CSL_UdmapCppi5HMPD * )Osal_PhysToVirt (hd);
      }

      /* Push descriptor to Rx free descriptor queue */
      if (hd != NULL) {
        RingPush (tFramework.gRxFreeRingHandle, hd->bufInfo1,(physptr_t)hd);
      }

  } while (hd != NULL);
  /* return queue processing end */
}
#endif
  	
void saDataModeTest (void *arg0, void* arg1)
{
 	tFramework_t  *tf  = ((saTestArgs_t *)arg)->tf;
 	saTest_t      *pat = ((saTestArgs_t *)arg)->pat;
#if defined(NSS_LITE2)
 	FW_CPPI_DESC_T     *pAppDesc;
 	CSL_UdmapCppi5HMPD *hd;
    saTestStatus_t      testResult = SA_TEST_PASSED;
    uintptr_t           cmdLblAddr;
    uint32_t            flowId,  pktId, psFlags;
    uint32_t            pTsInfo, pSwInfo0, pSwInfo1, pSwInfo2;
#else
 	Cppi_HostDesc *hd;
#endif
    int  i;
    testPktDesc_t       pktDesc;
    uint16_t            pktNum;
    Sa_PktInfo_t        pktInfo;
    Sa_PktDesc_t*       pPktDesc = &pktInfo.pktDesc;
    Sa_PayloadInfo_t*   pPayloadInfo = &pktInfo.payloadInfo;
    Sa_CmdLabelInfo_t*  pCmdLbInfo = &pktInfo.cmdlb;
    salldSimChannel_t  *pDataModeChan = &tf->salldSimChn[saTestCfgDataMode.chnum];
    uint8_t   cmdLbBuf[sa_MAX_CMDLB_SIZE];
 	uint8_t   iv[16] = {0xde, 0xad, 0xbe, 0xef,     /* Salt */
                      0, 0, 0, 0, 0, 0, 0, 0,     /* 8-byte IV from the packet */
                      0, 0, 0, 1};                /* counter */
                      
    /* Update run time values */
    saTestCfgDataMode.destInfo[0].flowID = tf->tfFlowNum;
    saTestCfgDataMode.destInfo[1].flowID = tf->tfFlowNum;
#if defined(NSS_LITE2)
    saTestCfgDataMode.destInfo[0].queueID = tf->rxComplRingNum;
    saTestCfgDataMode.destInfo[1].queueID = tf->rxComplRingNum;
#endif
    /* Clear the pktInfo structure elements */
	memset((void*) &pktInfo, 0, sizeof (Sa_PktInfo_t));

    /* Input the test vectors */
    utilInputTestPkts("pkt_test7.bin");
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    
    /* Setup the SA Channel */
    salldcfg_chan_init(&saTestCfgDataMode);
    salldSim_init_chn(saTestCfgDataMode.chnum);
    salldSim_open_chn(saTestCfgDataMode.chnum, &(saTestCfgDataMode.openCfg));
    
    pktNum = 1;
    while (utilGetPkt(&pktDesc))
    {
        /* 
         * Need to remove the ICV from the packet due to the limitation of the packet generator
         */
        pktDesc.size -= 8;
        pktDesc.payloadLen -= 8; 
    
	    /* Allocate the buffer descriptor based on the packet size */
        hd = testCommonGetBuffer(tf, (int)pktDesc.size);
	    if (hd == NULL)  {
#if defined(NSS_LITE2)
            System_printf ("saDataModeTest: no link buffers are available \n");
            testResult = SA_TEST_FAILED;  /* no return */
            goto testcomplete;
#else
	        System_printf ("%s (%s:%d): Queue pop failed for Link Buffer queue (%d)\n", pat->name, __FILE__, __LINE__, tf->QLinkedBuf2);
#endif
	    }
#if defined(NSS_LITE2)
    #if defined (BUILD_MCU)
        pPktDesc->segments[0]  = (void *)(uint32_t) (hd->bufPtr);
    #else
        pPktDesc->segments[0]  = (void *) (hd->bufPtr);
    #endif
        pPktDesc->size             = pktDesc.size;
        pPktDesc->segUsedSizes[0]  = pPktDesc->size;
        pPktDesc->segAllocSizes[0] = hd->orgBufLen;
        /* Construct the original packet and update the first 32-bit payload with connection ID */
        memcpy(pPktDesc->segments[0], pktDesc.pkt,  pktDesc.size);
#else
        memcpy((void *)hd->buffPtr, pktDesc.pkt, pktDesc.size);
        hd->buffLen = pktDesc.size;   
        
        salld_sim_print("Routing Pkt %d to SA: BufPtr = 0x%08x, pkt at 0x%08x, size = %d\n",
                         pktNum, hd->buffPtr, pktDesc.pkt, pktDesc.size); 
            
         /* Data Mode processing */
         pPktDesc->size             = hd->buffLen;
         pPktDesc->segments[0]      = (void *)hd->buffPtr;
         pPktDesc->segAllocSizes[0] = hd->origBufferLen;
#endif
         pPktDesc->segUsedSizes[0] = pPktDesc->size;
         pPktDesc->payloadOffset   = pktDesc.payloadOffset;
         pPktDesc->payloadLen      = pktDesc.payloadLen;
         
         /* Payload Information */
         pPayloadInfo->encOffset  = pktDesc.payloadOffset + 16;  /* ESP header size (Basic, IV) = (8, 8) */
         pPayloadInfo->authOffset = pktDesc.payloadOffset;
         pPayloadInfo->authSize   = pktDesc.payloadLen;
         pPayloadInfo->encSize    = pktDesc.payloadLen - 16;
         pPayloadInfo->encIV      = &iv[0];
         pPayloadInfo->authIV     = NULL;
         pPayloadInfo->aad        = iv;
         memcpy(&iv[4], &pktDesc.pkt[pktDesc.payloadOffset + 8], 8);
          
         /* Command Label Info */
         pCmdLbInfo->cmdLbBuf = cmdLbBuf;
          
         pktInfo.validBitMap = sa_PKT_INFO_VALID_PAYLOAD_INFO |
                               sa_PKT_INFO_VALID_CMDLB_INFO;
         
         /* Call SA LLD API to perform Protocol Specific Operation */
         Sa_chanSendData(pDataModeChan->salldInst, &pktInfo, FALSE);

#if defined(NSS_LITE2)
         /* Update EPIB information */
         CSL_udmapCppi5SetEpiDataPresent((void*) hd, EPI_PRESENT);
         CSL_udmapCppi5RdEpiData((void *) hd, &pTsInfo, &pSwInfo0, &pSwInfo1, &pSwInfo2);
         /* UPdate swInfo */
         pSwInfo0 = pktInfo.swInfo.swInfo[0];
         pSwInfo1 = pktInfo.swInfo.swInfo[1];
         pSwInfo2 = pktInfo.swInfo.swInfo[2];
         CSL_udmapCppi5WrEpiData((void *) hd, pTsInfo, pSwInfo0, pSwInfo1, pSwInfo2);
         /* overwrite the flow id */
         CSL_udmapCppi5GetIds((void*) hd, &pktId, &flowId);
         /* Clear PS flags and any error */
         hd->pktInfo1 = 0;
         /* Clear any next descriptor pointer */
         hd->nextDescPtr = 0;
         flowId = tFramework.tfFlowNum; //TF_RXCHAN_FLOWID;
         CSL_udmapCppi5SetIds((void*) hd, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST, pktId, flowId);
         /* Prepare to send pkt to PA/SA */
         CSL_udmapCppi5SetBufferAddr((CSL_UdmapCppi5HMPD*) hd, (uint64_t)Osal_VirtToPhys(pPktDesc->segments[0]));
         CSL_udmapCppi5SetBufferLen ((CSL_UdmapCppi5HMPD*) hd, pPktDesc->size);
         CSL_udmapCppi5SetPktLen((void *) hd, CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST, pPktDesc->size);

         /* Attach the command label in PS data */
         CSL_udmapCppi5SetPsDataLoc((void *) hd, CSL_UDMAP_CPPI5_PD_DESCINFO_PSINFO_VAL_IN_DESC);
         cmdLblAddr = CSL_udmapCppi5GetPsDataAddr((void*) hd, 0, EPI_PRESENT);
         /* First word of PS INFO */
         memcpy ((void *) cmdLblAddr, pCmdLbInfo->cmdLbBuf, pCmdLbInfo->cmdLbSize);
         CSL_udmapCppi5SetPsDataLen((void *) hd, pCmdLbInfo->cmdLbSize);

         gRxPktCntInRing = 0;
         RingPush(tFramework.gTxRingHandle, hd->bufInfo1, (physptr_t) hd);

         /* The packet should loop back into receive ring queue */
         for (i = 0; i < 200; i++)  {
             utilCycleDelay (1000);
             if (gRxPktCntInRing != 0)
             {
                 break;
             }
         }

         if (i >= 200)  {
           System_printf ("%s (%s:%d): Did not find SA reply in ring \n", pat->name, __FILE__, __LINE__);
           System_flush();
           testResult = SA_TEST_FAILED;  /* no return */
           goto testcomplete;
         }

         /* Book Keep the Tx */
         sauBookKeepTx();

         /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
       //  if (CSL_ringaccGetRingOcc(&tFramework.ringAccCfg, TF_RING_RXQ ))
         if (gRxPktCntInRing != 0)
         {
             /* Set the interrupt flag to false to indicate it is processed now */
             gRxPktCntInRing = 0;
             pAppDesc = (FW_CPPI_DESC_T *) NULL;
             hd = (CSL_UdmapCppi5HMPD *)(NULL);
             RingPop(tFramework.gRxRingHandle, &pAppDesc);
             if (pAppDesc != (FW_CPPI_DESC_T *) NULL)
             {
                 hd = &pAppDesc->hostDesc;
             }
             if (hd == NULL)  {
                 System_printf ("saDataModeTest: Fail to receive \n");
                 System_flush();
                 testResult = SA_TEST_FAILED;  /* no return */
                 goto testcomplete;
             }
             /* Check if the descriptor has some errors, if yes - halt the test for inspection */
             psFlags = CSL_udmapCppi5GetPsFlags(hd);

             if (psFlags)
             {
                System_printf(" The SA2UL returned error code = %d \n", psFlags);
                System_flush();
                testResult = SA_TEST_FAILED;  /* no return */
                goto testcomplete;
             }

             sauBookKeepRx();
         }
         else
         {
             System_printf ("saDataModeTest: Fail to receive \n");
             System_flush();
             testResult = SA_TEST_FAILED;  /* no return */
             goto testcomplete;
         }
#else
         /* Prepare to send pkt to PA/SA */
  	     Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	     Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
  	     Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
    
         /* Attach the command label in PS data */
         Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, pCmdLbInfo->cmdLbBuf, pCmdLbInfo->cmdLbSize);
    
  	     Qmss_queuePush (tf->QPaTx[TF_PA_TX_QUEUE_INDEX_SA0], (Ptr)utilgAddr((uint32_t)hd), pPktDesc->size, TF_SIZE_DESC, Qmss_Location_TAIL);


  	     /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	     for (i = 0; i < 100; i++)  {
		     utilCycleDelay (1000);
		     if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
		         break;
	     }

	     if (i == 100)  {
		     salld_sim_halt();
		     System_printf ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		     saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	     }

	     /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
         while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
         {
	         hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
             salld_sim_print("Receive TxPkt %d from PA/SA\n", pktNum); 
             testDispPkts(hd);
	         if (testCommonRecycleLBDesc (tf, hd))  {
		         System_printf ("%s (%s:%d): Failure in testCommonRecycleLBDesc\n", pat->name, __FILE__, __LINE__);
 		         saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	         }
         }
#endif
        pktNum++;
        
    } /* Send Packet Loop */

testcomplete:
    /* Get Channel Statistics */
    salldSim_get_stats(saTestCfgDataMode.chnum);
    
    /* Close SA channel */
	salldSim_close_chn(saTestCfgDataMode.chnum);

#if defined (NSS_LITE2)
	saTestRecoverAndExit(tf, pat, testResult);
#else
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
#endif
}
	
	
	
		
	


  		
