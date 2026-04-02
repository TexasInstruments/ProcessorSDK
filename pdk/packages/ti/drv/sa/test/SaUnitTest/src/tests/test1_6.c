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

/* Air Ciphering Basic Functional test.
 * This test is designed to verify the basic opertions of Air Ciphering
 * by creating multiple Air Ciphering channels with different configurations
 * and performing packet verification tests.
 * The goal is to cover all the normal program flows in both SA LLD and the
 * PDSP firmware inside the SA sub-system.
 * Note: Special operations such as error handling and etc will be covered in 
 * other tests.
 *
 * Test Procedures:
 * 
 * - Create multiple Air Ciphering connections including SA LLD Air Ciphering
 *   channels with configurations specified below
 * - Perform multi-packet, multi-connection data verification test
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - Invoke salld_receiveData() API for Air Ciphering channel
 *       - Prepare and send packet to SA for To-Air operation
 *       - Receive encrypted (To-Air) packet from SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_sendData() API for Air Ciphering channel
 *       - Forward the tx packet to SA for From-Air packet processing
 *       - Preceive the rx packet from the SA sub-system
 *       - Perform payload verification              
 *   - End of the test loop
 * - Query all the SALLD channel statistics
 * - Close all the SALLD channels
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */

/*
 * Air Ciphering channel configuration array
 */
#if SAUTEST_KEY_IN_INTMEM_NOT_SUPPORTED
static sauAcConfig_t  testAcCfg[] =
{
    /* Cipher Mode, Auth Mode, pudType, upLink, internal Key, countCPresent */
    {sa_CipherMode_NULL,      sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE,  FALSE},
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_NULL,       sa_AcPduType_GSM,       TRUE,  FALSE,  FALSE},
    {sa_CipherMode_AES_CTR,   sa_AuthMode_NULL,       sa_AcPduType_WCDMA_TMD, TRUE,  FALSE,  FALSE},
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_NULL,       sa_AcPduType_WCDMA_UMD, FALSE, FALSE,  FALSE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_WCDMA_AMD, TRUE,  FALSE,  FALSE},
#ifdef NSS_GEN2    
    {sa_CipherMode_ZUC_F8,    sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE,  FALSE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_SNOW3G_F9,  sa_AcPduType_LTE_CP,    TRUE,  FALSE,  FALSE},
    {sa_CipherMode_ZUC_F8,    sa_AuthMode_ZUC_F9,     sa_AcPduType_LTE_CP,    FALSE, FALSE,  FALSE},
    {sa_CipherMode_AES_CTR,   sa_AuthMode_CMAC,       sa_AcPduType_LTE_CP,    TRUE,  FALSE,  FALSE},
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_KASUMI_F9,  sa_AcPduType_LTE_CP,    FALSE, FALSE,  FALSE},
#else
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE,  TRUE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE,  FALSE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE_CP,    FALSE, FALSE,  FALSE},
#endif    
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE_CP,    FALSE, FALSE,  FALSE},
    {sa_CipherMode_NULL,      sa_AuthMode_KASUMI_F9,  sa_AcPduType_LTE_CP,    FALSE, FALSE,  FALSE},
    {sa_CipherMode_NULL,      sa_AuthMode_CMAC,       sa_AcPduType_LTE_CP,    FALSE, FALSE,  FALSE},
    {sa_CipherMode_AES_CTR,   sa_AuthMode_NULL,       sa_AcPduType_LTE_CP,    TRUE,  FALSE,  FALSE}
};
#else
static sauAcConfig_t  testAcCfg[] =
{
    /* Cipher Mode, Auth Mode, pudType, upLink, internal Key, countCPresent */
    {sa_CipherMode_NULL,      sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE, FALSE},
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_NULL,       sa_AcPduType_GSM,       TRUE,  TRUE,  FALSE},
    {sa_CipherMode_AES_CTR,   sa_AuthMode_NULL,       sa_AcPduType_WCDMA_TMD, TRUE,  TRUE,  FALSE},
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_NULL,       sa_AcPduType_WCDMA_UMD, FALSE, FALSE, FALSE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_WCDMA_AMD, TRUE,  TRUE,  FALSE},
#ifdef NSS_GEN2    
    {sa_CipherMode_ZUC_F8,    sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE, FALSE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_SNOW3G_F9,  sa_AcPduType_LTE_CP,    TRUE,  TRUE,  FALSE},
    {sa_CipherMode_ZUC_F8,    sa_AuthMode_ZUC_F9,     sa_AcPduType_LTE_CP,    FALSE, FALSE, FALSE},
    {sa_CipherMode_AES_CTR,   sa_AuthMode_CMAC,       sa_AcPduType_LTE_CP,    TRUE,  TRUE,  FALSE},
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_KASUMI_F9,  sa_AcPduType_LTE_CP,    FALSE, FALSE, FALSE},
#else
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE, TRUE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE,       FALSE, FALSE, FALSE},
    {sa_CipherMode_SNOW3G_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE_CP,    FALSE, TRUE,  FALSE},
#endif    
    {sa_CipherMode_KASUMI_F8, sa_AuthMode_NULL,       sa_AcPduType_LTE_CP,    FALSE, TRUE,  FALSE},
    {sa_CipherMode_NULL,      sa_AuthMode_KASUMI_F9,  sa_AcPduType_LTE_CP,    FALSE, FALSE, FALSE},
    {sa_CipherMode_NULL,      sa_AuthMode_CMAC,       sa_AcPduType_LTE_CP,    FALSE, TRUE,  FALSE},
    {sa_CipherMode_AES_CTR,   sa_AuthMode_NULL,       sa_AcPduType_LTE_CP,    TRUE,  FALSE, FALSE}
};
#endif

/* SA Air Ciphering Basic Functional test */
void saAirCipheringTest (void* a0, void* a1)
{

 	tFramework_t  *tf  = (tFramework_t *)a0;
 	saTest_t      *pat = (saTest_t *)a1;
    sauHdrHandle_t *pAcHdl;
 	int  i;
    #ifdef NSS_GEN2
    int  numChan = (saSimTest)?11:14;
    #else
    int  numChan = (saSimTest)?8:12;
    #endif
    
    /* Create test channels */
    for ( i = 0; i < numChan; i++)
    {
        /* Create AC channel */
        if ((pAcHdl = sauCreateAc(NULL, 0, &testAcCfg[i])) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create connection */
        if (sauCreateConnection(tf, pat, pAcHdl) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
    }
    
    /* Packet Tests */
    sauConnAcPktTest(tf, pat, 
                     numTestPkt1,     /* Number of packets per connection */
                     80,              /* initial payload length */
                     3,               /* payload llength increment step */
                     SAU_PAYLOAD_INC8 /* payload pattern */
                     );
    
    sauConnAcPktTest(tf, pat, 
                     numTestPkt2,     /* Number of packets per connection */
                     300,             /* initial payload length */
                     6,               /* payload llength increment step */
                     SAU_PAYLOAD_INC8 /* payload pattern */
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
    /* Get System Statistics */
    salldSim_get_sys_stats();
    salld_sim_disp_control(FALSE);    
    
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
