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

/* SRTP test.
 * This test performs a basic SRTP Tx/Rx test:
 * - Open and Configure a SALLD channel in SRTP mode
 * - Configure PA for the receive path (MAC/IP/UDP)
 * - Execute the test loop until all packets are processed
 * - Read and send the packet to PA PDSP0 as received from the network
 * - Wait and pop the received packet from the received queue
 * - Invoke the SALLD APIs to peform protocol-specific post-processing
 *   (End of receiving test)
 * - Invoke the SALLD APIs to perform protocol-specifc pre-processing
 * - Prepare and send a packet to the SA queue.
 * - Wait and pop the transmitted packet from the received queue
 * - End of the test loop
 * - Query and verify PA statistics
 * - Query the SALLD channel statistics
 * - Close the SALLD channel
 * - Remove all entries (handles) from the PA LUT1 and LUT2
 *
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 *
 */

#if 0 /* For reference only */

#ifdef _TMS320C6X
#pragma DATA_ALIGN(saSrtpPkt, 16)
const unsigned char saSrtpPkt[] = {
#else
const unsigned char saSrtpPkt[] __attribute__ ((aligned (16))) = {
#endif
                            0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC,                      /* Dest MAC */
                            0x00, 0xe0, 0xa6, 0x66, 0x57, 0x04,                      /* Src MAC  */
                            0x08, 0x00,                                              /* Ethertype = IPv4 */
                            0x45, 0x00, 0x00, 0x82,                                  /* IP version, services, total length */
                            0x00, 0x00, 0x00, 0x00,                                  /* IP ID, flags, fragment offset */
                            0x05, 0x11, 0x93, 0x85,                                  /* IP ttl, protocol (UDP), header checksum */
                            0x9e, 0xda, 0x6d, 0x0A,                                  /* Source IP address */
                            0x0a, 0x0b, 0x0c, 0x0d,                                  /* Destination IP address */
                            0x12, 0x34, 0x05, 0x55,                                  /* UDP source port, dest port */
                            0x00, 0x6e, 0xe1, 0x98,                                  /* UDP len, UDP checksum */
                            0x80, 0x64, 0x00, 0x64,                                  /* RTP: Version Payload Type Seq Number */
                            0x00, 0x00, 0x00, 0x00,                                  /* RTP: timestamp */
                            0xde, 0xad, 0xbe, 0xef,                                  /* RTP: SSRC */
                            0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,          /* 80 bytes of payload data */
                            0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
                            0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
                            0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51,
                            0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
                            0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61,
                            0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
                            0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
                            0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
                            0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81,
                            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,          /* SRTP Hash */
                            0x18, 0x19};
#endif

/* SALLD channel configurations */
/* SRTP */
salldTestConfig_t saTestCfg3 =
        {
            0,                      /* channel index */
            sa_PT_SRTP,             /* protocolType */
            sa_CipherMode_AES_CTR,  /* cipherMode */
            sa_AuthMode_HMAC_SHA1,  /* authMode */
            64,                     /* relayWinSize */
            {                       /* openCfg */
                sa_CONTROLINFO_CTRL_TX_ON |      /* ctrlBitMap */
                sa_CONTROLINFO_CTRL_RX_ON
            },
            {                       /* destInfo */
                {
                    0,              /* ctrlBitfield */
                    0,              /* Flow ID */
                    TF_PA_QUEUE_TXCMD,            /* Queue ID: PA PDSP4 */
                    0x33300000,     /* swInf0 */
                    0x00003330      /* swInf1 */
                },
                {
                    0,              /* ctrlBitfield */
                    0,              /* Flow ID */
                    DEST_QUEUE_PKT_RECV,     /* Queue ID */
                    0x33330000,     /* swInf0 */
                    0x00003333      /* swInf1 */
                },
            },
        };

static void paSetupSrtpRouting (tFramework_t *tf, saTest_t  *pat)
{
    Sa_SWInfo_t*   pSwInfo;

	paEthInfo_t ethInfo = { {  0x00, 0xe0, 0xa6, 0x66, 0x57, 0x04 },
                            {  0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC },	/*  PA entry 0 */
		                    0, 0x800, 0
                          };

    paIpInfo_t ipInfo =   {
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   /* IP source = dont care */   \
                             { 0x0a, 0x0b, 0x0c, 0x0d, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   /* IP dest */                 \
                               0,         /* SPI = dont care */                                                    \
                               0,         /* flow = dont care */                                                   \
                               pa_IPV4,   /* IP type */                                                            \
                               0,         /* GRE protocol */                                                       \
                               0,         /* Ip protocol = dont care (TCP or UDP or anything else) */              \
                               0,         /* TOS */                                                                \
                               FALSE,     /* TOS = dont care (seperate field since TOS=0 is valid */               \
                               0};        /* SCTP port = dont care */


 	paRouteInfo_t   matchRoute = {  pa_DEST_CONTINUE_PARSE_LUT1,/* Dest */
 								    0,					        /* Flow ID */
 								    0,					        /* queue */
 								    -1,					        /* Multi route */
 								    0,					        /* sw Info 0 */
                                    0,                          /* sw Info 1 */
                                    0,                          /* customType : not used */
                                    0,                          /* customIndex: not used */
                                    0,                          /* pkyType: for SRIO only */
                                    NULL};                      /* No commands */

 	paRouteInfo_t   nfailRoute = {  pa_DEST_DISCARD,            /* Dest */
 									0,					        /* Flow ID */
 									0,					        /* queue */
 									-1,					        /* Multi route */
 									0,					        /* sw Info 0 */
                                    0,                          /* sw Info 1 */
                                    0,                          /* customType : not used */
                                    0,                          /* customIndex: not used */
                                    0,                          /* pkyType: for SRIO only */
                                    NULL};                      /* No commands */

 	paCmdReply_t cmdReply = {  pa_DEST_HOST,			        /* Dest */
 							   0,						        /* Reply ID (returned in swinfo0) */
 							   0,						        /* Queue */
 							   0};						        /* Flow ID */

 	paRouteInfo_t   ipmatchRoute = {pa_DEST_CONTINUE_PARSE_LUT2,/* Dest */
 								    0,					        /* Flow ID */
 								    0,				            /* queue */ /* TF_SA_QUEUE_0 */
 								    -1,					        /* Multi route */
 								    0x12345678,					/* sw Info 0 */
 								    0x89abcdef, 				/* sw Info 1 */
                                    0,                          /* customType : not used */
                                    0,                          /* customIndex: not used */
                                    0,                          /* pkyType: for SRIO only */
                                    NULL};                      /* No commands */

 	paRouteInfo_t   ipnfailRoute = {  pa_DEST_DISCARD,		        /* Dest */
 									0,					        /* Flow ID */
 									0,					        /* queue */
 									-1,					        /* Multi route */
 									0,					        /* sw Info 0 */
                                    0,                          /* sw Info 1 */
                                    0,                          /* customType : not used */
                                    0,                          /* customIndex: not used */
                                    0,                          /* pkyType: for SRIO only */
                                    NULL};                      /* No commands */

 	paRouteInfo_t   udpRoute =   {  pa_DEST_SASS,     	     	/* Dest */
 								    0,					        /* Flow ID */
 								    TF_SA_QUEUE_0,                        /* queue */ /* TF_SA_QUEUE_0 */
 								    -1,					        /* Multi route */
 								    0x0,     					/* sw Info 0 */
                                    0,                          /* sw Info 1 */
                                    0,                          /* customType : not used */
                                    0,                          /* customIndex: not used */
                                    0,                          /* pkyType: for SRIO only */
                                    NULL};                      /* No commands */


    /* Runtime initial values */
    matchRoute.queue = (uint16_t) tf->QGen[Q_MATCH];
    nfailRoute.queue = (uint16_t) tf->QGen[Q_NFAIL];
    cmdReply.queue   = (uint16_t) tf->QGen[Q_CMD_REPLY];
	udpRoute.flowId  = ipnfailRoute.flowId = ipmatchRoute.flowId = nfailRoute.flowId = tf->tfFlowNum;
	cmdReply.flowId  = tf->tfFlowNum;

 	/* Initialize the first entry in the table */
 	cmdReply.replyId = TF_CMD_SWINFO0_ADD_ID + 0;  /* TF_CMD_SWINFO0_ADD_ID identifies command, 16 LS bits identify the local handle number */
 	cmdReply.queue = tf->QGen[Q_CMD_REPLY];
 	matchRoute.swInfo0 = nfailRoute.swInfo0 = TF_CMD_SWINFO0_PKT_ID;
 	if (!testCommonAddMac (tf, pat, (paEthInfo_t *)&ethInfo, &matchRoute, &nfailRoute,
 	                        &l2Handles[0].paHandle, tf->QLinkedBuf1, &cmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }

 	l2Handles[0].state = TF_L2_HANDLE_ACTIVE;
 	saTestExpectedStats.classify1.nPackets += 1;

 	/* Initialize the first IP entry in the table */
 	cmdReply.replyId = TF_CMD_SWINFO0_ADD_ID + 0;  /* TF_CMD_SWINFO0_ADD_ID identifies command, 16 LS bits identify the local handle number */
 	cmdReply.queue = tf->QGen[Q_CMD_REPLY];
 	if (!testCommonAddIp  (tf, pat, (paIpInfo_t *)&ipInfo, &ipmatchRoute, &ipnfailRoute,
 	                      l2Handles[0].paHandle, &l3Handles[0].paHandle, tf->QLinkedBuf1,
 	                      &cmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }

 	l3Handles[0].state = TF_L2_HANDLE_ACTIVE;
 	saTestExpectedStats.classify1.nPackets += 1;

 	/* Initialize the UDP entry in the table */
 	cmdReply.replyId = TF_CMD_SWINFO0_ADD_ID + 0;  /* TF_CMD_SWINFO0_ADD_ID identifies command, 16 LS bits identify the local handle number */
 	cmdReply.queue = tf->QGen[Q_CMD_REPLY];

    pSwInfo = &tf->salldSimChn[0].regSwInfo;
    udpRoute.swInfo0 = pSwInfo->swInfo[0];
    udpRoute.swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);

 	if (!testCommonAddUdp  (tf, pat, 0x0555, &udpRoute,
 	                       l3Handles[0].paHandle, l4Handles[0].paHandle, tf->QLinkedBuf1,
 	                       &cmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }

 	l4Handles[0].state = TF_L2_HANDLE_ACTIVE;
}

void saSRTPTest (void *arg)
{
 	tFramework_t  *tf  = ((saTestArgs_t *)arg)->tf;
 	saTest_t      *pat = ((saTestArgs_t *)arg)->pat;
 	Cppi_HostDesc *hd;
 	int  i;
 	uint32_t len;
    testPktDesc_t  pktDesc;
    uint16_t       pktNum;
    Sa_PktInfo_t   pktInfo;
    Sa_PktDesc_t*  pPktDesc = &pktInfo.pktDesc;
    salldSimChannel_t  *pSrtpChan = &tf->salldSimChn[saTestCfg3.chnum];
    uint32_t       psInfo[10];
    int            psInfoSize;
	paReturn_t     paret;

    paRouteInfo_t  route =  {  pa_DEST_HOST,  /* Route - host         */
                               0,             /* flow Id              */
                               DEST_QUEUE_PKT_RECV,  			  /* Queue                */
                               -1,            /* Multi route disabled */
                               0x7777000,     /* SWInfo 0             */
                               0,              /* sw Info 1 */
                               0,              /* customType : not used */
                               0,              /* customIndex: not used */
                               0,              /* pkyType: for SRIO only */
                               NULL};          /* No commands */

    paTxChksum_t pktUdpChksum = {
        34,     /* Start offset of UDP header */
        88,     /* Checksum length (UDP payload + UDP checksum */
        6,      /* Offset to checksum location RELATIVE TO THE START OF THE UDP HEADER */
        0x1054, /* Initial value is IPv4 pseudo header checksum value */
        1       /* computed value of 0 written as -0 */
    };

	/* Update run time values */
	saTestCfg3.destInfo[0].flowID = saTestCfg3.destInfo[1].flowID = route.flowId = tf->tfFlowNum;

    /* Input the test vectors */
    if (tf->fNullEnc)
    {
        utilInputTestPkts("pkt_test3.bin");
    }
    else
    {
        utilInputTestPkts("pkt_test3_enc.bin");
    }

    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);

    /* Setup the SA Channel */
    salldcfg_chan_init(&saTestCfg3);
    salldSim_init_chn(saTestCfg3.chnum);
    salldSim_open_chn(saTestCfg3.chnum, &(saTestCfg3.openCfg));

    /* Configure PA */
    paSetupSrtpRouting (tf, pat);

    pktNum = 1;
    while (utilGetPkt(&pktDesc))
    {
	    /* Send one data packet to the SA. The packet should be delivered back to Q 906 */
        hd = testCommonGetBuffer(tf, (int)pktDesc.size);
	    //hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tf->QLinkedBuf2)) & ~15);
	    if (hd == NULL)  {
		    System_printf ("%s (%s:%d): Queue pop failed for Link Buffer queue (%d)\n", pat->name, __FILE__, __LINE__, tf->QLinkedBuf2);
 		    saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	    }

        memcpy((void *)hd->buffPtr, pktDesc.pkt, pktDesc.size);
        hd->buffLen = pktDesc.size;
  	    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pktDesc.size);

        salld_sim_print("Routing Pkt %d to PA: BufPtr = 0x%08x, pkt at 0x%08x, size = %d\n",
                         pktNum, hd->buffPtr, pktDesc.pkt, pktDesc.size);

        //if(pktNum == 5) mdebugHaltPdsp (0);
        //mdebugHaltSaPdsp (0);
  	    Qmss_queuePush (tf->QPaTx[TF_PA_Q_INPUT], (Ptr)utilgAddr((uint32_t)hd), len, TF_SIZE_DESC, Qmss_Location_TAIL);

        /* Update expected statistics */
        saTestExpectedStats.classify1.nPackets += 2;    /* MAC/IP1 */
        saTestExpectedStats.classify1.nTableMatch += 2; /* MAC/IP */
        saTestExpectedStats.classify1.nIpv4Packets += 1;/* IP */
        saTestExpectedStats.classify2.nUdp++;

        //if(pktNum == 5) salld_sim_halt();

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
            pasahoLongInfo_t	*pLongInfo;
            uint32_t  infoLen;

	        hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));

            Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&pLongInfo, &infoLen);

            /* SRTP Rx processing */
            pPktDesc->size = hd->buffLen;
            pPktDesc->segments[0] = (void *)hd->buffPtr;
            pPktDesc->segUsedSizes[0] = pPktDesc->size;
            pPktDesc->segAllocSizes[0] = hd->origBufferLen;
            pPktDesc->payloadOffset = PASAHO_LINFO_READ_START_OFFSET(pLongInfo);  /* Pointer to RTP header */
            pPktDesc->payloadLen = (pPktDesc->size - pPktDesc->payloadOffset);
            pktInfo.validBitMap = sa_PKT_INFO_VALID_PKT_ERR_CODE;
            pktInfo.pktErrCode = Cppi_getDescError(Cppi_DescType_HOST, (Cppi_Desc *)hd);
            /* Call SA LLD API to perform Protocol Specific Operation */
            Sa_chanReceiveData(pSrtpChan->salldInst, &pktInfo);

            salld_sim_print("Receive Pkt %d from PA/SA\n", pktNum);
            testDispPkts(hd);

            /* Transmit SRTP over ESP packet */
            /* Reuse the link buffer */
            {
                uint16_t cmdSize;
                uint16_t rtpPayloadSize, udpPsudoChkSum;
                uint8_t  rtpPayloadOffset;

                memset (psInfo, 0, sizeof(psInfo));

                /* Preapare and send a packet through PA/SA */
	            /* Send one data packet to the SA. The packet should be delivered back to Q 950 */

                /* Call SA LLD API to perform Protocol Specific Operation */
                pktInfo.validBitMap = 0;
                Sa_chanSendData(pSrtpChan->salldInst, &pktInfo, FALSE);

                /* record RTP payload Info */
                rtpPayloadSize = pPktDesc->payloadLen;
                rtpPayloadOffset = pPktDesc->payloadOffset;

                /* Update UDP Length & clear the UDP checksum */
                pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]), (rtpPayloadOffset - 4), (rtpPayloadSize + 8));
                pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]), (rtpPayloadOffset - 2), 0);

                /* Update IP Length, checksum and psudo checksum */
                pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]),
                                (rtpPayloadOffset - 28 + 2),
                                (rtpPayloadSize + 28));
                utilSetIpv4ChkSum(((uint8_t *)pPktDesc->segments[0]) + 14);
                udpPsudoChkSum = utilGetIpv4PsudoChkSum(((uint8_t *)pPktDesc->segments[0]) + 14,
                                                        rtpPayloadSize + 8);

  	            Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);


                /* Prepare PS Information */
                /*
                * The following commands should be passed in the PS Info section
                * - Payload Info (short format) for SRTP
                * - UDP checksum
                * - Routing Info to Host
                */
                psInfo[0] = PASAHO_SINFO_FORMAT_CMD(rtpPayloadOffset, rtpPayloadSize);
                psInfoSize = 8;

                cmdSize = sizeof(psInfo) - psInfoSize;

                pktUdpChksum.startOffset = rtpPayloadOffset - 8;
                pktUdpChksum.lengthBytes = rtpPayloadSize + 8;
                pktUdpChksum.initialSum  = udpPsudoChkSum;

                paret = Pa_formatTxRoute (  &pktUdpChksum,       /* UDP checksum */
                                            NULL,                /* No second checksum   */
                                            &route,              /* Internal routing     */
                                            (Ptr)&psInfo[psInfoSize/4],     /* Command buffer       */
                                            &cmdSize);           /* Command size         */

                if (paret != pa_OK)  {
                    System_printf ("%s (%s:%d): Pa_formatTxRoute returned error code %d\n", pat->name, __FILE__, __LINE__, paret);
 	                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                }
                psInfoSize += cmdSize;

                /* Attach the command in PS data */
                Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, psInfoSize);

                //mdebugHaltPdsp (4);
                //mdebugHaltSaPdsp (0);
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
                    salld_sim_print("Receive TxPkt %d from PA/SA\n", pktNum);
                    testDispPkts(hd);
	                if (testCommonRecycleLBDesc (tf, hd))  {
		                System_printf ("%s (%s:%d): Failure in testCommonRecycleLBDesc\n", pat->name, __FILE__, __LINE__);
 		                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	                }
                }
            }
        } /* receive and transmit packet operation */

        pktNum++;

    } /* Send Packet Loop */
#ifndef __LINUX_USER_SPACE
	/* Request stats from the PA */
    if (!testCommonReqAndChkPaStats (tf, pat, (paSysStats_t *)&saTestExpectedStats))
    {
		System_printf ("%s (%s:%d): testCommonReqAndChkPaStats failed\n", pat->name, __FILE__, __LINE__);
  	    System_flush ();
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
#endif
  	System_flush ();

    /* Get Channel Statistics */
    salldSim_get_stats(saTestCfg3.chnum);

    /* Close SA channel */
	salldSim_close_chn(saTestCfg3.chnum);

 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */

}








