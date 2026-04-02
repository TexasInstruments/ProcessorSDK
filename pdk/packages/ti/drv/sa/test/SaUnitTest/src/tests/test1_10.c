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

/* Multi-segment IPSEC AH Functional test.
 * This test is designed to verify the basic opertions of IPSEC AH
 * by creating multiple IPSEC AH channels with different configurations.
 * The goal is to cover all the normal program flows in both SA LLD and the
 * PDSP firmware inside the SA sub-system.
 * Note: Special operations such as IPSEC replay protection, error handling 
 * and etc will be covered in other tests.
 *
 * Test Procedures:
 * 
 * - Create multiple IPSEC AH tunnels including SALLD IPSEC AH
 *   channels with configurations specified below
 * - Establish a connection for each IPSEC Tunnel including
 *   - Configure PA for the receive path (MAC/IP/AH/IP/UDP)
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

/*
 * IPSEC AH channel configuration array
 */
static sauIpsecConfig_t  testIpsecCfg[] =
{
    /* Cipher Mode, Auth Mode, encKeySize, macKeySize, macSize, esnFlag, replayWinSize */
    {sa_CipherMode_NULL, sa_AuthMode_HMAC_SHA1,      0, 20, 12, TRUE,  64},
    {sa_CipherMode_NULL, sa_AuthMode_HMAC_MD5,       0, 16, 12, FALSE, 64},
    {sa_CipherMode_NULL, sa_AuthMode_CMAC,           0, 16, 8, TRUE,  64},
};

/* SA IPSEC AH Multisegment Basic Functional test */
void saAHMultiSgmtTest (void* a0, void* a1)
{

    uint8_t macSrc[6], macDest[6], ipSrc[16], ipDest[16], ipinSrc[16], ipinDest[16];

 	tFramework_t  *tf  = (tFramework_t *)a0;
 	saTest_t      *pat = (saTest_t *)a1;
    sauHdrHandle_t *pMacHdl, *pIpHdl, *pIpinHdl, *pUdpHdl;
 	int  i;

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
    for ( i = 0; i < 1; i++)
    {
        ipDest[0] = (uint8_t)i;
        if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_IP_IN_IP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

        /* Attach IPSEC AH channel to Outer IP */
        if (!sauCreateIpsec(pIpHdl, SA_TEST_GET_SPI(i), SAU_IPSEC_TYPE_AH,  &testIpsecCfg[i]))
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
    for ( i = 1; i < 2; i++)
    {
        ipDest[0] = (uint8_t)i;
        if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_IPV6_IN_IP, FALSE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

        /* Attach IPSEC AH channel to Outer IP */
        if (!sauCreateIpsec(pIpHdl, SA_TEST_GET_SPI(i), SAU_IPSEC_TYPE_AH,  &testIpsecCfg[i]))
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

    /* Third IP group (IP) */
    macDest[0] = 0x32;
    if ((pMacHdl = sauCreateMac(macSrc, macDest, ETH_TYPE_IP)) == NULL)
    {
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }

    for ( i = 2; i < 3; i++)
    {
        /* IPSEC AH Transport Mode */
        ipDest[0] = (uint8_t)i;
        if ((pIpHdl = sauCreateIp(pMacHdl, ipSrc, ipDest, IP_PROTO_UDP, TRUE)) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

        /* Attach IPSEC AH channel to IP */
        if (!sauCreateIpsec(pIpHdl, SA_TEST_GET_SPI(i), SAU_IPSEC_TYPE_AH,  &testIpsecCfg[i]))
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

        /* Create UDP channel */
        if ((pUdpHdl = sauCreateUdp(pIpHdl, SA_TEST_GET_UDP_PORT(i), SA_TEST_GET_UDP_PORT(i))) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }

        /* Create connection */
        if (sauCreateConnection(tf, pat, pUdpHdl) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
    }
    /* Packet Tests */
    sauConnMultiSegmentPktTest(tf, pat,
                       numTestPkt1,     /* Number of packets per connection */
                       80,              /* initial payload length (152) */
                       3,               /* payload llength increment step */
                       SAU_PAYLOAD_INC8,/* payload pattern */
                       0,               /* Error Rate */
                       TWO_SEGMENT);

    sauConnMultiSegmentPktTest(tf, pat,
                       numTestPkt2,     /* Number of packets per connection */
                       450,             /* initial payload length (152) */
                       6,               /* payload llength increment step */
                       SAU_PAYLOAD_INC8,/* payload pattern */
                       0,               /* Error Rate */
                       TWO_SEGMENT);

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
