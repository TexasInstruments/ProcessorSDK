/*
 *
 * Copyright (C) 2012-2013 Texas Instruments Incorporated - http://www.ti.com/ 
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

#include "../unittest.h"
#include "../testconn.h"
#include "../salldsim/salldcfg.h"

/* IPSEC ESP NAT-T Basic Functional test.
 * This test is designed to verify the basic opertions of IPSEC ESP NAT-T
 * by creating multiple IPSEC ESP NAT-T channels with different configurations.
 * The goal is to cover all the normal program flows in both SA LLD and the
 * PDSP firmware inside the SA sub-system.
 * Note: Special operations such as IPSEC replay protection, error handling 
 * and etc will be covered in other tests.
 *
 * Test Procedures:
 * 
 * - Create multiple IPSEC ESP NAT-T tunnels including SALLD IPSEC ESP NAT-T
 *   channels with configurations specified below
 * - Establish a connection for each IPSEC Tunnel including
 *   - Configure PA for the receive path (MAC/IP/ESP/IP/UDP)
 * - Perform multi-packet, multi-connection data verification test
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * - Query and verify PA statistics
 * - Query all the SALLD channel statistics
 * - Close all the SALLD channels
 * - Remove all entries (handles) from the PA LUT1 and LUT2
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */
 
#undef SA_MULT_PKT_TEST 

/*
 * IPSEC ESP channel configuration array
 */
static sauIpsecConfig_t  testIpsecCfg[] =
{
    /* Cipher Mode, Auth Mode, encKeySize, macKeySize, macSize, esnFlag, replayWinSize */
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,     16, 20, 12, TRUE,  64},
    {sa_CipherMode_AES_CBC, sa_AuthMode_HMAC_MD5,      24, 16, 12, FALSE, 64},
    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,          16,  0,  0, TRUE,  0},
    {sa_CipherMode_3DES_CBC, sa_AuthMode_HMAC_SHA2_256, 24, 32, 16, TRUE,  128},
    {sa_CipherMode_GCM,     sa_AuthMode_NULL,          16,  0,  8, FALSE, 128},
    {sa_CipherMode_CCM,     sa_AuthMode_NULL,          24,  0, 16, TRUE,  128},
    {sa_CipherMode_NULL,    sa_AuthMode_GMAC,           0, 32, 16, FALSE, 128},     
    {sa_CipherMode_NULL,    sa_AuthMode_HMAC_MD5,      0, 16, 16, FALSE,  64},
};

/*
 * NAT-T exception route
 */
#define T14_NUM_EXCEPTION_ROUTES    1
 static int testErouteTypes[] = {
    pa_EROUTE_NAT_T_DATA,
 };
 
 static paRouteInfo_t testEroutes[] = {
    /* NAT-T Data  */
 	{  pa_DEST_HOST,		/* Dest */
 	   0,					/* Flow ID */
 	   0,                   /* queue (TF_PA_QUEUE_OUTER_IP) */
 	   -1,					/* Multi route */
 	   0,                   /* sw Info 0 */
       0,                   /* sw Info 1 */       
       0,                   /* customType : not used */         
       0,                   /* customIndex: not used */     
       0,                   /* pkyType: for SRIO only */    
       NULL                 /* No commands */
    }
 };
 
  static paRouteInfo_t testEroutes2[] = {
    /* NAT-T DATA  */
 	{  pa_DEST_DISCARD,		/* Dest */
 	   0,					/* Flow ID */
 	   0,                   /* queue */
 	   -1,					/* Multi route */
 	   0,                   /* sw Info 0 */
       0,                   /* sw Info 1 */       
       0,                   /* customType : not used */         
       0,                   /* customIndex: not used */     
       0,                   /* pkyType: for SRIO only */    
       NULL                 /* No commands */
    }
 };
 
static paCmdReply_t cmdReply = {  pa_DEST_HOST,				/* Dest */
 							      0,						/* Reply ID (returned in swinfo0) */
 							   	  0,						/* Queue */
 							      0 };						/* Flow ID */

static Bool testNatTConfiguration (tFramework_t *tf, saTest_t *pat, int enable)
{
    paCtrlInfo_t   ctrlInfo;
    Bool           ret;
    
    /* Prepare and issue global configuration command */
    memset(&ctrlInfo, 0, sizeof(paCtrlInfo_t));
    ctrlInfo.code = pa_CONTROL_IPSEC_NAT_T_CONFIG;
    ctrlInfo.params.ipsecNatTDetCfg.udpPort = 4500;
    ctrlInfo.params.ipsecNatTDetCfg.ctrlBitMap = enable?pa_IPSEC_NAT_T_CTRL_ENABLE:0;
    cmdReply.replyId  = TF_CMD_SWINFO0_CONFIG_ID;
	cmdReply.queue    = tf->QGen[Q_CMD_REPLY];
    
    ret = testCommonGlobalConfig(tf, pat, &ctrlInfo, tf->QLinkedBuf1, &cmdReply);
    
 	paTestExpectedStats.classify2.nPackets++;
    
    if(nssGblCfgParams.layout.fNssGen2)
 	    paTestExpectedStats.classify1.nPackets++;
    
    if(!ret)return(ret);
    
    ret = testCommonConfigExceptionRoute(tf, pat, T14_NUM_EXCEPTION_ROUTES, testErouteTypes, (enable)?testEroutes:testEroutes2, 
                                         tf->QLinkedBuf4, &cmdReply);
    return(ret);
}

/* SA IPSEC ESP NAT-T Basic Functional test */
void saESPNATTTest (void* a0, void* a1)
{

    uint8_t macSrc[6], macDest[6], ipSrc[16], ipDest[16], ipinSrc[16], ipinDest[16];
 	tFramework_t  *tf  = (tFramework_t *)a0;
 	saTest_t      *pat = (saTest_t *)a1;
    sauHdrHandle_t *pMacHdl, *pIpHdl, *pIpinHdl, *pUdpHdl;
 	int  i;
    int  numGrp1Chan, numGrp2Chan;
    
    /* Initialize protocol addresses */
    memcpy(macSrc, testMacSrcAddr, 6);
    memcpy(macDest, testMacDestAddr, 6);
    memcpy(ipSrc, testIpSrcAddr, 16);
    memcpy(ipDest, testIpDestAddr, 16);
    memcpy(ipinSrc, testIpinSrcAddr, 16);
    memcpy(ipinDest, testIpinDestAddr, 16);
    
    /* Initialize exception routes */
    /* NAT-T */
    testEroutes[0].queue = (uint16_t)(nssGblCfgParams.layout.txQueueBase +      
                                      nssGblCfgParams.layout.qPaIpIndex);
    
    if (!saSimTest)
    {
        numGrp1Chan = 4;
        numGrp2Chan = 4;
    }
    else
    {
        numGrp1Chan = 2;
        numGrp2Chan = 2;
    }
    
    /* Create test channels */
    
    /* First MAC group */
    if ((pMacHdl = sauCreateMac(macSrc, macDest, ETH_TYPE_IP)) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    /* First IP group */
    for ( i = 0; i < numGrp1Chan ; i++)
    {
        ipDest[0] = (uint8_t)i;
        if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_IP_IN_IP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Attach IPSEC ESP channel to Outer IP */
        if (!sauCreateIpsec(pIpHdl, SA_TEST_GET_SPI(i), SAU_IPSEC_TYPE_ESP_NAT_T,  &testIpsecCfg[i]))
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create inner IP */
        ipinDest[0] = (uint8_t)i;
        if ((pIpinHdl = sauCreateIp(pIpHdl, ipinSrc, ipinDest, IP_PROTO_UDP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create UDP channel */
        if ((pUdpHdl = sauCreateUdp(pIpinHdl, SA_TEST_GET_UDP_PORT(i), SA_TEST_GET_UDP_PORT(i))) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create connection */
        if (sauCreateConnection(tf, pat, pUdpHdl) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
    }
    
    /* Second IP group (IPV6) */
    macDest[0] = 0x22;
    if ((pMacHdl = sauCreateMac(macSrc, macDest, ETH_TYPE_IPV6)) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    for ( i = 4; i < (numGrp2Chan + 4); i++)
    {
        ipDest[0] = (uint8_t)i;
        if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_IPV6_IN_IP, FALSE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Attach IPSEC ESP channel to Outer IP */
        if (!sauCreateIpsec(pIpHdl, SA_TEST_GET_SPI(i), SAU_IPSEC_TYPE_ESP_NAT_T,  &testIpsecCfg[i]))
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create inner IP */
        ipinDest[0] = (uint8_t)i;
        if ((pIpinHdl = sauCreateIp(pIpHdl, ipinSrc, ipinDest, IP_PROTO_UDP, FALSE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create UDP channel */
        if ((pUdpHdl = sauCreateUdp(pIpinHdl, SA_TEST_GET_UDP_PORT(i), SA_TEST_GET_UDP_PORT(i))) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create connection */
        if (sauCreateConnection(tf, pat, pUdpHdl) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
    }
    
    if (testNatTConfiguration(tf, pat, TRUE) == FALSE)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    #ifndef SA_MULT_PKT_TEST
    /* Packet Tests */
    sauConnPktTest(tf, pat, 
                   numTestPkt1,     /* Number of packets per connection */
                   152,             /* initial payload length (152) */
                   3,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8,/* payload pattern */
                   0                /* Error Rate */
                   );
                   
    sauConnPktTest(tf, pat, 
                   numTestPkt2,     /* Number of packets per connection */
                   450,             /* initial payload length (250)*/
                   6,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8,/* payload pattern */
                   0                /* Error Rate */
                   );
    #else
    sauMultiPktTest(tf, pat, 
                   1,               /* Number of packets per connection */
                   250,             /* initial payload length */
                   4,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8,/* payload pattern */
                   0                /* Error Rate */
                   );
    #endif    
    
    if (testNatTConfiguration(tf, pat, FALSE) == FALSE)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
                   
	/* Request stats from the PA */
    if (!testCommonReqAndChkPaStats (tf, pat, (paSysStats_t *)&paTestExpectedStats))
    {
		SALog ("%s (%s:%d): testCommonReqAndChkPaStats failed\n", pat->name, __FILE__, __LINE__);
  	    System_flush ();
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
  	System_flush ();
    
    /* Get Channel Statistics */
    salld_sim_disp_control(TRUE);    
    salldSim_get_all_chan_stats();
    /* Get System Statistics */
    salldSim_get_sys_stats();
    
    salld_sim_disp_control(FALSE);    
    
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
