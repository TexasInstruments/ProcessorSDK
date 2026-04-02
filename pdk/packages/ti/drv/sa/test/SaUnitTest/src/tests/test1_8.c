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

/* Air Ciphering Key stream generation test.
 * This test is designed to verify the key stream generation for the following
 * three 3GPP encrytion modes:
 *  - GSM A5/3: 228-bit (114-bit for uplink, 114-bit for downlink)
 *  - ECSD A5/3: 692-bit (348-bit for uplink, 348-bit for downlink)
 *  - GEA3: 8M bit (M = key size in bytes)
 *  of Air Ciphering
 *
 * Note: The Key stream generation is a special case of the normal 3GPP Air Ciphering
 *       operation with fixed or variable size all-zero payload. 
 *
 * Test Procedures:
 * 
 * - Create multiple Air Ciphering connections including SA LLD Air Ciphering
 *   channels with configurations specified below
 * - Perform multi-packet, multi-connection key generation test
 *   - For each pkt loop
 *     - Generate fixed or variable size all-zero payload based on the ciphering mode
 *     - For each connection
 *       - Format the IV based on the ciphering mode
 *       - Perform the following protocol-specific operations 
 *          - Invoke salld_receiveData() API for Air Ciphering channel
 *       - Forward the tx packet to SA for From-Air packet processing
 *       - Preceive the rx packet from the SA sub-system
 *       - Perform key straem display              
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
static sauAcConfig_t  testAcCfg[] =
{
    /* Cipher Mode, Auth Mode, pudType, upLink */
    {sa_CipherMode_GSM_A53,   sa_AuthMode_NULL,       sa_AcPduType_GSM,       FALSE},
    {sa_CipherMode_ECSD_A53,  sa_AuthMode_NULL,       sa_AcPduType_GSM,       FALSE},
    {sa_CipherMode_GEA3,      sa_AuthMode_NULL,       sa_AcPduType_GSM,       FALSE},
    {sa_CipherMode_GEA3,      sa_AuthMode_NULL,       sa_AcPduType_GSM,       TRUE}
};

/* SA Air Ciphering Key Generation test */
void saAirCipheringKeyGenTest (void* a0, void* a1)
{

 	tFramework_t  *tf  = (tFramework_t *)a0;
 	saTest_t      *pat = (saTest_t *)a1;
    sauHdrHandle_t *pAcHdl;
 	int  i;
    
    /* Initialize protocol addresses */
    
    /* Create test channels */
    for ( i = 0; i < 4; i++)
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
    sauConnAcKeyGenTest(tf, pat, 
                        numTestPkt1,     /* Number of packets per connection */
                        80,              /* initial payload length: NA for certain air ciphering modes */
                        4                /* payload llength increment step:  NA for certain air ciphering modes*/
                        );
                        
    sauConnAcKeyGenTest(tf, pat, 
                        numTestPkt2,     /* Number of packets per connection */
                        300,             /* initial payload length: NA for certain air ciphering modes */
                        10               /* payload llength increment step:  NA for certain air ciphering modes*/
                        );
                        
    
    /* Get Channel Statistics */
    salldSim_get_all_chan_stats();
    
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
