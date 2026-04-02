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

/* SRTP Re-key test.
 * This test is designed to verify the Re-key opertions of SRTP
 * by creating multiple SRTP channels with different configurations.
 * The goal is to cover all the re-key program flows in both SA LLD and the
 * PDSP firmware inside the SA sub-system.
 *
 * Test Procedures:
 * 
 * - Create multiple MAC/IP/UDP/SRTP connections including SA LLD SRTP
 *   channels with configurations specified below
 * - For each SRTP channel, set the initial sequence number to trigger
 *   Re-key conndition.
 * - Configure PA for the receive path (MAC/IP/UDP/SRTP)
 * - Perform multi-packet, multi-connection data verification test
 *   - For each pkt loop
 *     - Generate fixed-length payload with specific data pattern
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - Invoke salld_sendData() API for SRTP channel
 *         - Prepare UDP checksum and routing commands 
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
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
Bool sauDebug = FALSE;

/*
 * SRTP channel configuration array
 */
static sauSrtpConfig_t  testSrtpCfg[] =
{
    /* Cipher Mode, Auth Mode, macSize, replayWinSize, derivRate, mkiSize, fromToFlag, KeyLifeTime, FromEsn, ToEsn */
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,  10, 64, 17, 0, TRUE,  0xffffffff, 0, 0xffffffff},
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,  10, 64, 24, 0, FALSE,  1000, 0, 0xffffffff},
};

/* SA SRTP Basic Functional test */
void saSRTPRekeyTest2 (void* a0, void* a1)
{

    uint8_t macSrc[6], macDest[6], ipSrc[16], ipDest[16];
 	tFramework_t  *tf  = (tFramework_t *)a0;
 	saTest_t      *pat = (saTest_t *)a1;
    sauHdrHandle_t *pMacHdl, *pIpHdl, *pUdpHdl[4];
 	int  i;
    
    /* Initialize protocol addresses */
    memcpy(macSrc, testMacSrcAddr, 6);
    memcpy(macDest, testMacDestAddr, 6);
    memcpy(ipSrc, testIpSrcAddr, 16);
    memcpy(ipDest, testIpDestAddr, 16);
    
    /* Create test channels */
    
    /* Create MAC/IP channel */
    if ((pMacHdl = sauCreateMac(macSrc, macDest, ETH_TYPE_IP)) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_UDP, TRUE)) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    /* First UDP group */
    for ( i = 0; i < 1; i++)
    {
        /* Create UDP channel */
        if ((pUdpHdl[i] = sauCreateUdp(pIpHdl, SA_TEST_GET_UDP_PORT(i), SA_TEST_GET_UDP_PORT(i))) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Attach SRTP channel to UDP */
        if (!sauCreateSrtp(pUdpHdl[i], 0, 0xbabeface,  &testSrtpCfg[i]))
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create connection */
        if (sauCreateConnection(tf, pat, pUdpHdl[i]) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
    }
    
    /* Packet Tests */
    /* Sequence 0x7ff8 ==> 0x8007 */
    sauUpdateSrtp(pUdpHdl[0], 0x7ff8, 0xbabefac1);
    sauConnPktTest(tf, pat, 
                   16,              /* Number of packets per connection */
                   80,              /* initial payload length */
                   0,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8,/* payload pattern */
                   0                /* Error Rate */
                   );
                   
    /* Sequence 0xfff8 ==> 0x0007 (roc = 1) */
    sauUpdateSrtp(pUdpHdl[0], 0xfff8, 0xbabefac2);
    sauConnPktTest(tf, pat, 
                   16,              /* Number of packets per connection */
                   80,              /* initial payload length */
                   0,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8,/* payload pattern */
                   0                /* Error Rate */
                   );
                   
    /* Sequence 0x7ff8 ==> 0x8007 (roc = 1) */
    sauUpdateSrtp(pUdpHdl[0], 0x7ff8, 0xbabefac3);
    sauConnPktTest(tf, pat, 
                   16,              /* Number of packets per connection */
                   80,              /* initial payload length */
                   0,               /* payload llength increment step */
                   SAU_PAYLOAD_INC8,/* payload pattern */
                   0                /* Error Rate */
                   );
                   
    /* Sequence 0xfff0 ==> 0x007f (roc = 2) */
    sauUpdateSrtp(pUdpHdl[0], 0xfff0, 0xbabefac4);
    sauConnPktTest(tf, pat, 
                   154,             /* Number of packets per connection */
                   80,              /* initial payload length */
                   0,               /* payload llength increment step */
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
    salld_sim_disp_control(TRUE);    
    salldSim_get_all_chan_stats();
    salld_sim_disp_control(FALSE);
    
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
