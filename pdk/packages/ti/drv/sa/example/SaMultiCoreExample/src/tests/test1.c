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
#include "testconn.h"
#include "salldsim/salldcfg.h"

/* IPSEC ESP test.
 * This test performs a basic IPSEC ESP Tx/Rx test:
 * - Open and Configure a SALLD channel in IPSEC ESP mode
 * - Configure PA for the receive path (MAC/IP/ESP/IP/UDP)
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
 
/* SALLD channel configurations */                            
/* IPSEC ESP */
static salldTestConfig_t saTestCfg1 =        
        {            
            0,                      /* channel index */
            sa_PT_IPSEC_ESP,        /* protocolType */
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
                    TF_PA_QUEUE_INPUT,             /* Queue ID (PA PDSP0)*/
                    0x11100000,                    /* swInf0 */
                    0x1110                         /* swInf1 */
                },
                {
                    0,              /* ctrlBitfield */ 
                    0,              /* Flow ID */
                    TF_PA_QUEUE_INNER_IP,         /* Queue ID */  
                    0x11110000,     /* swInf0 */
                    0x1111          /* swInf1 */
                },
            },
        };
        
        

void saESPTest (void *arg)
{
    uint8_t        macSrc[6], macDest[6], ipSrc[16], ipDest[16], ipinSrc[16], ipinDest[16];
    sauHdrHandle_t *pMacHdl, *pIpHdl, *pIpinHdl, *pUdpHdl;
	tFramework_t  *tf  = ((saTestArgs_t *)arg)->tf;
 	saTest_t      *pat = ((saTestArgs_t *)arg)->pat;
    sa_MCExample_entry_t val;
    
    /* Initialize protocol addresses */
    memcpy(macSrc, testMacSrcAddr, 6);
    memcpy(macDest, testMacDestAddr, 6);
    memcpy(ipSrc, testIpSrcAddr, 16);
    memcpy(ipDest, testIpDestAddr, 16);
    memcpy(ipinSrc, testIpinSrcAddr, 16);
    memcpy(ipinDest, testIpinDestAddr, 16);

	/* Update run time values */
	saTestCfg1.destInfo[0].flowID = tf->tfFlowNum;
	saTestCfg1.destInfo[1].flowID = tf->tfFlowNum;
	
	setsauHostRouteQ(); 
	/* Set the result code */
	pat->testStatus = SA_TEST_NOT_RUN;	

    /* Create a Single IPSec Channel from Master Core */
    if (coreNum == SYSINIT)
    {
        /* First MAC group */
        if ((pMacHdl = sauCreateMac(macSrc, macDest, ETH_TYPE_IP)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

    	if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_IP_IN_IP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

        /* Attach IPSEC ESP channel to Outer IP */
        if (!sauCreateIpsec(pIpHdl, SAU_IPSEC_TYPE_ESP, &saTestCfg1))
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

        /* Create inner IP */
        if ((pIpinHdl = sauCreateIp(pIpHdl, ipinSrc, ipinDest, IP_PROTO_UDP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        /* save the Inner Ip handle offset in shared memory */
        val.ipInHandle = sauConvBase2Offset(pIpinHdl);
        fw_shmSetEntry(setgIpInHndCmd, val);
		System_printf ("Published the Inner IP Handle offset (CORE ID:%d) %d \n", coreNum, val.ipInHandle);		
    }
    else
    {
    	val = fw_shmGetEntry(getgIpInHndCmd);
		System_printf ("Obtained the Inner IP Handle offset (CORE ID:%d) %d \n", coreNum, val.ipInHandle);
    	pIpinHdl = (sauHdrHandle_t*) sauConvOffset2Base(val.ipInHandle);

    }

    /* Create UDP channel from other cores */
    if ((pUdpHdl = sauCreateUdp(pIpinHdl, SA_TEST_GET_UDP_PORT(coreNum), SA_TEST_GET_UDP_PORT(coreNum))) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }

    /* Create connection */
    System_printf ("Creating Connection (Core:%d)...\n", coreNum);		
    if (sauCreateConnection(tf, pat, pUdpHdl) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    System_printf ("Done (Core:%d)...\n", coreNum);		

    /* publish from the master core that all global configurations are done */
    if (coreNum == SYSINIT) {
        System_printf ("Publishing global config done(Core:%d)...\n", coreNum);		
    	APP_publishGlobalCfgDone();
    }

    /* All cores update the counter informing that they finished their setup */
    /* The global variable is a shared resource which is being accessed from multiple cores.
     * So here we need to protect it and ensure that there is only 1 core which is accessing
     * it at a time. We use a Hardware Semaphore to protect this. */
    System_printf ("Publishing local config done(Core:%d)...\n", coreNum);
    APP_publishLocalCfgDone();

    /* All cores wait here to sync up and send packets to PA SA
       at the same time. */
    System_printf ("Waiting for all cores to reach the barrier before transmission starts (CORE ID: %d)... \n", coreNum);
    APP_waitAllLocalCfgDone();

    sauConnPktTest(tf, pat,
                   10,              /* Number of packets per connection */
                   153,              /* initial payload length (153) */
                   3,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8 /* payload pattern */
                   );

    salld_sim_disp_control(TRUE);

    System_flush ();
    
    /* Indicate that the test is complete from slave processes */
	if (coreNum != SYSINIT)
		APP_publishTestStatus();

    /* Close SA channel */
    if (coreNum == SYSINIT)
    {
		System_printf ("Waiting for all packets to be Received in all slave cores before freeing up the channel from Sysinit core.. \n");
		APP_waitSlaveTestStatus();	
    
        /* Get Channel Statistics */
        salldSim_get_stats(saTestCfg1.chnum);

    	salldSim_close_chn(saTestCfg1.chnum);
    	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */	
		/* Indicate test complete from Master */
		APP_publishTestStatus();
    }

    /* Wait for all test completion from all processes */
	APP_waitTestStatus();	

}
	
/* Nothing past this point */
