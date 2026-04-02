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

/* 3GPP LTE Tx/Rx test.
 * This test performs a basic 3GPP LTE Tx/Rx test:
 * - Open and Configure a SALLD channel in 3GPP LTE mode
 * - Read and send the canned LTE to-air packet to SA
 * - Wait and pop the received packet from the received queue
 * - Invoke the SALLD APIs to peform protocol-specific post-processing
 *   (End of receiving test)
 * - Use the processed To-Air packet as the LTE From-Air packet
 * - Invoke the SALLD APIs to perform protocol-specifc pre-processing 
 * - Prepare and send a packet to the SA queue.
 * - Wait and pop the processed From-Air packet from the received queue
 * - Query the SALLD channel statistics
 * - Close the SALLD channel
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */
 

/* From Network Traffic */
const unsigned char ACPacketLTEToAir[] = {
    0x55,                                             /* 84 byte PDU Data */
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,   
    0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
    0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
    0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61,
    0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
    0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81,
    0x82, 0x83, 0x84};

#if 0 /* for reference only */

/* To Network Traffic */
const unsigned char ACPacketLTEFromAir[] = {
    0x00, 0x00, 0x20, 0x00,                           /* Count-C */
    0x55,                                             /* 84 byte PDU Data */
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,   
    0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
    0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
    0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61,
    0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
    0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81,
    0x82, 0x83, 0x84 }; 
#endif

/* SALLD channel configurations */                            
/* 3GPP LTE */
static salldTestConfig_t saTestCfgLTE =        
        {            
            0,                      /* channel index */
            sa_PT_3GPP_AC,          /* protocolType */
            sa_CipherMode_SNOW3G_F8,/* cipherMode */
            sa_AuthMode_NULL,       /* authMode */
            0,                      /* relayWinSize */
            {                       /* openCfg */
                sa_CONTROLINFO_CTRL_TX_ON |      /* ctrlBitMap */
                sa_CONTROLINFO_CTRL_RX_ON    
            },
            {                       /* destInfo */
                {
                    0,            /* ctrlBitfield */
                    0,            /* Flow ID */
                    DEST_QUEUE_PKT_RECV,         /* Queue ID (PA PDSP4) or DEST_QUEUE_PKT_RECV*/
                    0x44400000,   /* swInf0 */
                    0x00004440    /* swInf0 */
                },
                {
                    0,                          /* ctrlBitfield */
                    0,                          /* Flow ID */
                    DEST_QUEUE_PKT_RECV,        /* Queue ID */
                    0x44440000,                 /* swInf0 */
                    0x00004444                  /* swInf1 */
                },
            },
        };
  	
void saLTETest (void *arg)
{
 	tFramework_t  *tf  = ((saTestArgs_t *)arg)->tf;
 	saTest_t      *pat = ((saTestArgs_t *)arg)->pat;
 	Cppi_HostDesc *hd;
    int i;
 	uint32_t len;
    Sa_PktInfo_t   pktInfo;
    Sa_PktDesc_t*  pPktDesc = &pktInfo.pktDesc;
    salldSimChannel_t  *pLteChan = &tf->salldSimChn[saTestCfgLTE.chnum];
    Sa_SWInfo_t*       pSwInfo = &pLteChan->regSwInfo;
    
    uint32_t         psInfo[2];

	/* Update the run time values */
	saTestCfgLTE.destInfo[0].flowID = tf->tfFlowNum;
	saTestCfgLTE.destInfo[1].flowID = tf->tfFlowNum;
	
    /* Input the test vectors */
    //utilInputTestPkts("ti/drv/sa/example/SaBasicExample/vectors/pkt_test1.bin");
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    
    /* Setup the SA Channel */
    salldcfg_chan_init(&saTestCfgLTE);
    salldSim_init_chn(saTestCfgLTE.chnum);
    salldSim_open_chn(saTestCfgLTE.chnum, &(saTestCfgLTE.openCfg));
    
    //while (utilGetPkt(&pktDesc))
    {
	    /* Prepare and send one data packet to the SA. The packet should be delivered back to Q 950 */
        len = sizeof(ACPacketLTEToAir);
        hd = testCommonGetBuffer(tf, (int)len);
	    if (hd == NULL)  {
		    System_printf ("%s (%s:%d): Queue pop failed for Link Buffer queue (%d)\n", pat->name, __FILE__, __LINE__, tf->QLinkedBuf2);
 		    saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	    }
        memcpy((void *)hd->buffPtr, ACPacketLTEToAir, len);
        hd->buffLen = len;   
  	    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, len);
  	    Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pSwInfo->swInfo);
        psInfo[0] = PASAHO_SINFO_FORMAT_CMD(0, len);
        psInfo[1] = 0;
        Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, 8);
        
        salld_sim_print("Routing Pkt to SA: BufPtr = 0x%08x, pkt at 0x%08x, size = %d\n",
                         hd->buffPtr, ACPacketLTEToAir, len); 
    
        //mdebugHaltSaPdsp (1);
  	    Qmss_queuePush (tf->QPaTx[TF_PA_TX_QUEUE_INDEX_SA0],
                        (Ptr)utilgAddr((uint32_t)hd), len, TF_SIZE_DESC, Qmss_Location_TAIL);
        // salld_sim_halt();
  	
  	    /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	    for (i = 0; i < 100; i++)  {
		    utilCycleDelay (1000);
		    if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) == 1)
			    break;
	    }
	
	    if (i == 100)  {
		    salld_sim_halt();
		    System_printf ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		    saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	    }
	    /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
        else
        {
	        hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
            
            /* 3GPP LTE Rx processing */
            pPktDesc->size = hd->buffLen;
            pPktDesc->segments[0] = (void *)hd->buffPtr; 
            pPktDesc->segUsedSizes[0] = pPktDesc->size;
            pPktDesc->segAllocSizes[0] = hd->origBufferLen;
            pPktDesc->payloadOffset = 0;
            pPktDesc->payloadLen = pPktDesc->size;
            pktInfo.validBitMap = sa_PKT_INFO_VALID_PKT_ERR_CODE;
            pktInfo.pktErrCode = sa_PKT_ERR_OK;
            /* Call SA LLD API to perform Protocol Specific Operation */
            Sa_chanReceiveData(pLteChan->salldInst, &pktInfo);
            
            salld_sim_print("Receive Pkt from PA/SA\n"); 
            testDispPkts(hd);
            
            /* Transmit 3GPP Air Ciphering packet */
            /* Reuse the link buffer */
            {
                
                memset (psInfo, 0, sizeof(psInfo));
    
                /* Preapare and send a packet through SA */
	            /* Prepare Send one data packet to the SA. The packet should be delivered back to Q 950 */
    
                /* 3GPP Air Ciphering Tx related operation */
                pktInfo.validBitMap = 0;
        
                /* Call SA LLD API to perform Protocol Specific Operation */
                Sa_chanSendData(pLteChan->salldInst, &pktInfo, FALSE);
    
                /* Prepare to send pkt to PA/SA */
  	            Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	            Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
  	            Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
    
                /* Prepare PS Information 
                 * The following commands should be passed in the PS Info section
                 * - Payload Info (short format) for 3GPP LTE PDU
                 */
                psInfo[0] = PASAHO_SINFO_FORMAT_CMD(4, pPktDesc->size - 4);
                psInfo[1] = pktRead32bits_m ((uint8_t *)hd->buffPtr,0);
                Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, 8);
                
                //mdebugHaltSaPdsp (1);
  	            Qmss_queuePush (tf->QPaTx[TF_PA_TX_QUEUE_INDEX_SA0], (Ptr)utilgAddr((uint32_t)hd), len, TF_SIZE_DESC, Qmss_Location_TAIL);
                //salld_sim_halt();
  	
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
                    salld_sim_print("Receive TxPkt from PA/SA\n"); 
                    testDispPkts(hd);
	                if (testCommonRecycleLBDesc (tf, hd))  {
		                System_printf ("%s (%s:%d): Failure in testCommonRecycleLBDesc\n", pat->name, __FILE__, __LINE__);
 		                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	                }
                }
            }
        } /* receive and transmit packet operation */  
        
    } /* Send Packet Loop */
    
    /* Get Channel Statistics */
    salldSim_get_stats(saTestCfgLTE.chnum);
    
    /* Close SA channel */
	salldSim_close_chn(saTestCfgLTE.chnum);
    
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
