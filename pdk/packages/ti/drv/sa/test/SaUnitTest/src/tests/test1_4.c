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



#include "../unittest.h"
#include "../testconn.h"
#include "../salldsim/salldcfg.h"

/* SERP over IPSEC ESP Basic Functional test.
 * This test is designed to verify the basic opertions of SRTP over IPSEC ESP
 * by creating single or multiple SRTP over IPSEC ESP channels with different 
 * configurations.
 *
 * The goal is simply to serve as an exampe of SRTP over IPSEC ESP operation 
 * because both of them have been already verified in the previous test cases
 * thoughtfully.
 *
 * Test Procedures:
 * 
 * - Create single or multiple IPSEC ESP tunnels including 
 *   SALLD IPSEC ESP channels with configurations specified below
 * - Create single or multiple IP/UDP/SRTP connection including
 *   SA LLD SRTP channels per IPSEC tunnel
 * - Establish a connection for each SRTP channel
 *   - Configure PA for the receive path (MAC/IP/ESP/IP/UDP/SRTP)
 * - Perform multi-packet, multi-connection data verification test
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - Invoke salld_sendData() API for SRTP channel
 *         - Prepare UDP checksum and routing commands 
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *          - Invoke salld_receiveData() API for SRTP channel
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
 
#define TEST_NUM_IPSEC_TUNNELS                1
#define TEST_NUM_SRTP_CHANS_PER_TUNNEL        1

/*
 * IPSEC ESP channel configuration array
 */
static sauIpsecConfig_t  testIpsecCfg[] =
{
    /* Cipher Mode, Auth Mode, encKeySize, macKeySize, macSize, esnFlag, replayWinSize */
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,     16, 20, 12, TRUE,  64},
};

static sauSrtpConfig_t  testSrtpCfg[] =
{
    /* Cipher Mode, Auth Mode, macSize, replayWinSize, derivRate, mkiSize, fromToFlag, KeyLifeTime, FromEsn, ToEsn */
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1, 10, 64, 24, 0, FALSE, 0xffffffff, 0, 0xffffffff},
};


/* SA SRTP over IPSEC ESP Functional test */
void saSrtpEspTest (void* a0, void* a1)
{
    uint8_t macSrc[6], macDest[6], ipSrc[16], ipDest[16], ipinSrc[16], ipinDest[16];
 	tFramework_t  *tf  = (tFramework_t *)a0;
 	saTest_t      *pat = (saTest_t *)a1;
    sauHdrHandle_t *pMacHdl, *pIpHdl, *pIpinHdl, *pUdpHdl;
 	int  i, j;
    
    /* Initialize protocol addresses */
    memcpy(macSrc, testMacSrcAddr, 6);
    memcpy(macDest, testMacDestAddr, 6);
    memcpy(ipSrc, testIpSrcAddr, 16);
    memcpy(ipDest, testIpDestAddr, 16);
    memcpy(ipinSrc, testIpinSrcAddr, 16);
    memcpy(ipinDest, testIpinDestAddr, 16);
    
    /* Create test channels */
    
    /* First MAC group */
    if ((pMacHdl = sauCreateMac(macSrc, macDest, ETH_TYPE_IP)) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    /* First IP group */
    for ( i = 0; i < TEST_NUM_IPSEC_TUNNELS; i++)
    {
        ipDest[0] = (uint8_t)i;
        if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_IP_IN_IP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Attach IPSEC ESP channel to Outer IP */
        if (!sauCreateIpsec(pIpHdl, SA_TEST_GET_SPI(i), SAU_IPSEC_TYPE_ESP,  &testIpsecCfg[i]))
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        for (j = 0; j < TEST_NUM_SRTP_CHANS_PER_TUNNEL; j++)
        {
            /* Create inner IP */
            ipinDest[0] = (uint8_t)(j + (i * TEST_NUM_SRTP_CHANS_PER_TUNNEL));
            if ((pIpinHdl = sauCreateIp(pIpHdl, ipinSrc, ipinDest, IP_PROTO_UDP, TRUE)) == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            /* Create UDP channel */
            if ((pUdpHdl = sauCreateUdp(pIpinHdl, SA_TEST_GET_UDP_PORT((i*TEST_NUM_SRTP_CHANS_PER_TUNNEL+j)), 
                                        SA_TEST_GET_UDP_PORT((i*TEST_NUM_SRTP_CHANS_PER_TUNNEL+j)))) == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
            
            /* Attach SRTP channel to UDP */
            if (!sauCreateSrtp(pUdpHdl, 0, 0xbabeface,  &testSrtpCfg[j]))
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
            
            /* Create connection */
            if (sauCreateConnection(tf, pat, pUdpHdl) == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        }    
    }
    
    /* Second IP group */
    for ( i = 0; i < TEST_NUM_IPSEC_TUNNELS; i++)
    {
        ipDest[0] = (uint8_t)(i + TEST_NUM_IPSEC_TUNNELS);
        if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_IPV6_IN_IP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Attach IPSEC ESP channel to Outer IP */
        if (!sauCreateIpsec(pIpHdl, SA_TEST_GET_SPI((TEST_NUM_IPSEC_TUNNELS + i)), SAU_IPSEC_TYPE_ESP,  &testIpsecCfg[i]))
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        for (j = 0; j < TEST_NUM_SRTP_CHANS_PER_TUNNEL; j++)
        {
            /* Create inner IP */
            ipinDest[0] = (uint8_t)(j + (TEST_NUM_IPSEC_TUNNELS + i) * TEST_NUM_SRTP_CHANS_PER_TUNNEL);
            if ((pIpinHdl = sauCreateIp(pIpHdl, ipinSrc, ipinDest, IP_PROTO_UDP, FALSE)) == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            /* Create UDP channel */
            if ((pUdpHdl = sauCreateUdp(pIpinHdl, SA_TEST_GET_UDP_PORT(((TEST_NUM_IPSEC_TUNNELS + i)*TEST_NUM_SRTP_CHANS_PER_TUNNEL+j)), 
                                        SA_TEST_GET_UDP_PORT(((TEST_NUM_IPSEC_TUNNELS + i)*TEST_NUM_SRTP_CHANS_PER_TUNNEL+j)))) == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
            
            /* Attach SRTP channel to UDP */
            if (!sauCreateSrtp(pUdpHdl, 0, 0xbabebeef,  &testSrtpCfg[j]))
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
            
            /* Create connection */
            if (sauCreateConnection(tf, pat, pUdpHdl) == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        }    
    }
    
    
    /* Packet Tests */
    sauConnPktTest(tf, pat, 
                   numTestPkt3,     /* Number of packets per connection */
                   80,              /* initial payload length */
                   3,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8,/* payload pattern */
                   0                /* Error Rate */
                   );
                   
	/* Request stats from the PA */
    if (!testCommonReqAndChkPaStats (tf, pat, (paSysStats_t *)&paTestExpectedStats))
    {
		SALog ("%s (%s:%d): testCommonReqAndChkPaStats failed\n", pat->name, __FILE__, __LINE__);
  	    System_flush ();
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
  	System_flush ();
    
    /* Get Channel Statistics */
    salldSim_get_all_chan_stats();
    
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
