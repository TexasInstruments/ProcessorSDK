/*
 *
 * Copyright (C) 2010-2020 Texas Instruments Incorporated - http://www.ti.com/ 
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
#include <inttypes.h>

/* Data Mode Basic Functional test.
 * This test is designed to verify the basic opertions of Data Mode
 * by creating multiple Data Mode channels with different configurations
 * and performing packet verification tests.
 * The goal is to cover all the normal program flows in both SA LLD and the
 * PDSP firmware inside the SA sub-system.
 *
 * Test Procedures:
 * 
 * - Create multiple Data Mode connections including pairs of SA LLD Data Mode
 *   channels with configurations specified below
 * - Perform multi-packet, multi-connection data verification test
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each pair of connections
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - Invoke salld_sendData() API for Data Mode channel
 *       - Prepare and send packet to SA for Encryption (and Authentication)
 *       - Receive encrypted packet from SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_sendData() API for the corresponding Data Mode channel
 *       - Forward the packet to SA for Decryption (and Authentication) 
 *       - Preceive the decrypted packet from the SA sub-system
 *       - Perform payload verification              
 *   - End of the test loop
 * - Query all the SALLD channel statistics
 * - Close all the SALLD channels
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */

#if defined (NSS_LITE2)
/* Enable the console print post silicon */
#define ENABLE_TPUT_CONSOLE_PRINT            1
/*
 * Throughput collection arrary
 */
volatile sauTest_tputData_t gSauTputData[SAU_TPUT_NUM_PKTSIZES][SAU_TPUT_MAX_PROFILE_CHANNELS];
#endif

/*
 * Data Mode channel configuration array
 */
static sauDataModeConfig_t  testDmCfg[] =
{
    /* Cipher Mode, Auth Mode, encKeySize, macKeySize, macSize, aadSize, ivSize, saltSize, enc flag, enc1st flag selACEng flag*/
    {sa_CipherMode_NULL, sa_AuthMode_NULL,      0, 0,  0,  0,  16, 0, TRUE, TRUE, FALSE },              /* 0 */
    {sa_CipherMode_NULL, sa_AuthMode_NULL,      0, 0,  0,  0,  16, 0, FALSE, FALSE, FALSE},             /* 1 */

#if !defined NSS_LITE2
    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,      16, 0,  0,  0,  16, 0, TRUE, TRUE, FALSE },          /*  */
    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,      16, 0,  0,  0,  16, 0, TRUE, TRUE, FALSE },          /*  */
#endif

    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,      16, 0,  0,  0,  16, 0, TRUE, TRUE, FALSE },          /* 2 */
    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,      16, 0,  0,  0,  16, 0, FALSE, FALSE, FALSE},         /* 3 */

    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,      32, 0,  0,  0,  16, 0, TRUE, TRUE, FALSE },          /* 4 */
    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,      32, 0,  0,  0,  16, 0, FALSE, FALSE, FALSE},         /* 5 */

    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,      16, 20,  8,  0,  16, 0, TRUE, TRUE, FALSE },    /* 6 */
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,      16, 20,  8,  0,  16, 0, FALSE, FALSE, FALSE},   /* 7 */

    {sa_CipherMode_GCM,     sa_AuthMode_NULL,           16,  0,  0, 12,  8, 4, TRUE, TRUE, FALSE},      /* 8 */
    {sa_CipherMode_GCM,     sa_AuthMode_NULL,           16,  0,  0, 12,  8, 4, FALSE, TRUE, FALSE},     /* 9 */

    {sa_CipherMode_NULL,    sa_AuthMode_GMAC,           0,  24, 12,  8,  8, 4,FALSE, FALSE, FALSE},     /* 10 */
    {sa_CipherMode_NULL,    sa_AuthMode_GMAC,           0,  24, 12,  8,  8, 4,FALSE, FALSE, FALSE},     /* 11 */

    {sa_CipherMode_NULL,    sa_AuthMode_GMAC_AH,        0,  16, 12,  0,  8, 4,FALSE, FALSE, FALSE},     /* 12 */
    {sa_CipherMode_NULL,    sa_AuthMode_GMAC_AH,        0,  16, 12,  0,  8, 4,FALSE, FALSE, FALSE},     /* 13 */

    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           16,  0, 0,  12,  8, 3,TRUE, TRUE, FALSE},       /* 14 */
    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           16,  0, 0,  12,  8, 3,FALSE, TRUE, FALSE},      /* 15 */

    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           24,  0, 0,  0,  13, 0,TRUE, TRUE, FALSE},       /* 16 */
    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           24,  0, 0,  0,  13, 0,FALSE, TRUE, FALSE},      /* 17 */

    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           32,  0, 0,  0,  13, 0,TRUE, TRUE, FALSE},       /* 18 */
    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           32,  0, 0,  0,  13, 0,FALSE, TRUE, FALSE},       /* 19 */

    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           32,  0, 0,  12,  8, 3,TRUE, TRUE, FALSE},       /* 20 */
    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           32,  0, 0,  12,  8, 3,FALSE, TRUE, FALSE}       /* 21 */,

    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           32,  0, 0,  0,  13, 0,TRUE, TRUE, FALSE},       /* 22 */
    {sa_CipherMode_CCM,     sa_AuthMode_NULL,           32,  0, 0,  0,  13, 0,FALSE, TRUE, FALSE}       /* 23 */,

    {sa_CipherMode_AES_CBC, sa_AuthMode_HMAC_MD5,       32, 20, 12,  0,  16, 0,TRUE, TRUE, FALSE}       /* 24*/,
    {sa_CipherMode_AES_CBC, sa_AuthMode_HMAC_MD5,       32, 20, 12,  0,  16, 0,FALSE, FALSE, FALSE},    /* 25 */

    {sa_CipherMode_3DES_CBC, sa_AuthMode_HMAC_SHA2_256,  24, 32, 16,  0, 8, 0,TRUE, TRUE, FALSE},       /* 26 */
    {sa_CipherMode_3DES_CBC, sa_AuthMode_HMAC_SHA2_256,  24, 32, 16,  0, 8, 0,FALSE, FALSE, FALSE},     /* 27 */

    {sa_CipherMode_3DES_CBC, sa_AuthMode_NULL,  24, 0, 0,  0, 8, 0,TRUE, TRUE, FALSE},                  /* 28 */
    {sa_CipherMode_3DES_CBC, sa_AuthMode_NULL,  24, 0, 0,  0, 8, 0,FALSE, FALSE, FALSE},                /* 29 */

    #ifdef SA_3GPP_SUPPORT
    {sa_CipherMode_SNOW3G_F8,  sa_AuthMode_NULL,        16, 0,  0,  0,  16, 0, FALSE, TRUE, FALSE},       /*  */
    {sa_CipherMode_SNOW3G_F8,  sa_AuthMode_NULL,        16, 0,  0,  0,  16, 0, FALSE, TRUE, FALSE},       /*  */

    {sa_CipherMode_KASUMI_F8,    sa_AuthMode_KASUMI_F9, 16, 16,  4,  0, 8, 0, FALSE, FALSE, FALSE},
    {sa_CipherMode_KASUMI_F8,    sa_AuthMode_KASUMI_F9, 16, 16,  4,  0, 8, 0, FALSE, TRUE, FALSE},       /*  */
    #endif

    {sa_CipherMode_NULL,    sa_AuthMode_CMAC,           0,  16, 12,  0,  16, 0,  TRUE, TRUE, FALSE},       /* 30 */
    {sa_CipherMode_NULL,    sa_AuthMode_CMAC,           0,  16, 12,  0,  16, 0, FALSE, FALSE, FALSE},      /* 31 */

    {sa_CipherMode_NULL,    sa_AuthMode_MD5,            0,  16, 12,  0,  16, 0, TRUE, TRUE, FALSE},        /* 32 */
    {sa_CipherMode_NULL,    sa_AuthMode_MD5,            0,  16, 12,  0,  16, 0, FALSE, FALSE, FALSE}       /* 33 */,

    #ifdef SA_3GPP_SUPPORT       /* 1 */
    {sa_CipherMode_NULL,    sa_AuthMode_KASUMI_F9,      0,  16,  4,  0,  8, 0,FALSE, FALSE, FALSE},       /* 1 */
    {sa_CipherMode_NULL,    sa_AuthMode_KASUMI_F9,      0,  16,  4,  0,  8, 0, FALSE, FALSE, FALSE}       /* 1 */,

    {sa_CipherMode_NULL,    sa_AuthMode_CMAC,           0,  24, 12,  8,  8, 4, FALSE, FALSE, TRUE},       /* */
    {sa_CipherMode_NULL,    sa_AuthMode_CMAC,           0,  24, 12,  8,  8, 4, FALSE, FALSE, TRUE},       /* 1 */

    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,          16, 0,  0,  0,  16, 0, TRUE, TRUE, TRUE },       /* 1 */
    {sa_CipherMode_AES_CTR, sa_AuthMode_NULL,          16, 0,  0,  0,  16, 0, TRUE, TRUE, TRUE },
    #endif

    {sa_CipherMode_NULL,    sa_AuthMode_NULL,           0,   0,  0,  0,  16, 0,TRUE,  TRUE, FALSE},        /* 34 */
    {sa_CipherMode_NULL,    sa_AuthMode_NULL,           0,   0,  0,  0,  16, 0,FALSE, FALSE, FALSE},       /* 35 */

    {sa_CipherMode_NULL,    sa_AuthMode_CMAC,           0,  24, 12,  8,  8, 4, FALSE, FALSE, FALSE},       /* 36 */
    {sa_CipherMode_NULL,    sa_AuthMode_CMAC,           0,  24, 12,  8,  8, 4, FALSE, FALSE, FALSE},       /* 37 */

    {sa_CipherMode_NULL,    sa_AuthMode_MD5,            0,  16, 12,  0,  16, 0, TRUE, TRUE, FALSE},        /* 38 */
    {sa_CipherMode_NULL,    sa_AuthMode_MD5,            0,  16, 12,  0,  16, 0, FALSE, FALSE, FALSE},      /* 39 */

    {sa_CipherMode_NULL,    sa_AuthMode_SHA1,           0,  16, 12,  0,  16, 0,  TRUE, TRUE, FALSE},       /* 40 */
    {sa_CipherMode_NULL,    sa_AuthMode_SHA1,           0,  16, 12,  0,  16, 0, FALSE, FALSE, FALSE},      /* 41 */

    {sa_CipherMode_NULL,    sa_AuthMode_SHA2_256,       0,  16, 12,  0,  16, 0,  TRUE, TRUE, FALSE},       /* 42 */
    {sa_CipherMode_NULL,    sa_AuthMode_SHA2_256,       0,  16, 12,  0,  16, 0, FALSE, FALSE, FALSE},      /* 43 */

    {sa_CipherMode_NULL, sa_AuthMode_HMAC_MD5,       0, 20, 12,  0,  16, 0,TRUE, TRUE, FALSE},             /* 44 */
    {sa_CipherMode_NULL, sa_AuthMode_HMAC_MD5,       0, 20, 12,  0,  16, 0,FALSE, FALSE, FALSE},           /* 45 */

    {sa_CipherMode_NULL, sa_AuthMode_HMAC_SHA2_256,  0, 32, 16,  0, 8, 0,TRUE, TRUE, FALSE},               /* 46 */
    {sa_CipherMode_NULL, sa_AuthMode_HMAC_SHA2_256,  0, 32, 16,  0, 8, 0,FALSE, FALSE, FALSE},             /* 47 */

#if defined (NSS_LITE2)
    {sa_CipherMode_NULL, sa_AuthMode_HMAC_SHA2_512,  0, 64, 16,  0, 8, 0,TRUE, TRUE, FALSE},               /* 48 */
    {sa_CipherMode_NULL, sa_AuthMode_HMAC_SHA2_512,  0, 64, 16,  0, 8, 0,FALSE, FALSE, FALSE},             /* 49 */
#endif
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,      16, 20,  8,  0,  16, 0, TRUE, TRUE, FALSE },       /* 50 */
    {sa_CipherMode_AES_CTR, sa_AuthMode_HMAC_SHA1,      16, 20,  8,  0,  16, 0, FALSE, FALSE, FALSE},      /* 51 */

#ifdef NSS_LITE2
    {sa_CipherMode_3DES_CBC, sa_AuthMode_HMAC_SHA2_512,  24, 32, 16,  8, 8, 0,TRUE, TRUE, FALSE},         /* 52 */
    {sa_CipherMode_3DES_CBC, sa_AuthMode_HMAC_SHA2_512,  24, 32, 16,  8, 8, 0,FALSE, FALSE, FALSE},       /* 53 */

    {sa_CipherMode_DES_CBC, sa_AuthMode_HMAC_SHA2_384,   8, 32, 16,  8,  8, 0,TRUE, TRUE, FALSE},         /* 54 */
    {sa_CipherMode_DES_CBC, sa_AuthMode_HMAC_SHA2_384,   8, 32, 16,  8,  8, 0,FALSE, FALSE, FALSE},       /* 55 */
#endif

    {sa_CipherMode_DES_CBC, sa_AuthMode_HMAC_SHA2_224,   8, 32, 16,  0,  8, 0,TRUE, TRUE, FALSE},       /* 56 */
    {sa_CipherMode_DES_CBC, sa_AuthMode_HMAC_SHA2_224,   8, 32, 16,  0,  8, 0,FALSE, FALSE, FALSE},     /* 57 */
};

#define SAU_TPUT_MAX_CHANNELS (sizeof(testDmCfg)/sizeof(sauDataModeConfig_t))

/* Internal data strucure */
typedef struct sauMetaData_s
{
    char        cipherMode[80];
    char        authMode[80];
    uint32_t    encKeySize;
    uint32_t    macKeySize;
}sauMetaDataInfo_t;

/* Internal functions */
static void sauDataModeGetMetaDataFromConnId(uint32_t connId, sauMetaDataInfo_t *metaData);
static void sauDataModeGetMetaDataFromConnId(uint32_t connId, sauMetaDataInfo_t *metaData)
{
    Sa_CipherMode_e     cipherMode;
    Sa_AuthMode_e       authMode;
    cipherMode = testDmCfg[connId].cipherMode;
    authMode   = testDmCfg[connId].authMode;
    metaData->encKeySize    = testDmCfg[connId].encKeySize;
    metaData->macKeySize    = testDmCfg[connId].macKeySize;

    switch (cipherMode)
    {
      case   sa_CipherMode_AES_CTR:
            strncpy(metaData->cipherMode, "CipherMode_AES_CTR", 40);
            break;
      case   sa_CipherMode_AES_F8:
            strncpy(metaData->cipherMode, "CipherMode_AES_F8", 40);
            break;
      case   sa_CipherMode_AES_CBC:
            strncpy(metaData->cipherMode, "CipherMode_AES_CBC", 40);
            break;
      case   sa_CipherMode_DES_CBC:
            strncpy(metaData->cipherMode, "CipherMode_DES_CBC", 40);
            break;
      case   sa_CipherMode_3DES_CBC:
            strncpy(metaData->cipherMode, "CipherMode_3DES_CBC", 40);
            break;
      case   sa_CipherMode_CCM:
            strncpy(metaData->cipherMode, "CipherMode_CCM", 40);
            break;
      case   sa_CipherMode_GCM:
            strncpy(metaData->cipherMode, "CipherMode_GCM", 40);
            break;
      case   sa_CipherMode_GSM_A53:
            strncpy(metaData->cipherMode, "CipherMode_GSM_A53", 40);
            break;
      case   sa_CipherMode_ECSD_A53:
            strncpy(metaData->cipherMode, "CipherMode_ECSD_A53", 40);
            break;
      case   sa_CipherMode_GEA3:
            strncpy(metaData->cipherMode, "CipherMode_GEA3", 40);
            break;
      case   sa_CipherMode_KASUMI_F8:
            strncpy(metaData->cipherMode, "CipherMode_KASUMI_F8", 40);
            break;
      case   sa_CipherMode_SNOW3G_F8:
            strncpy(metaData->cipherMode, "CipherMode_SNOW3G_F8", 40);
            break;
      case   sa_CipherMode_ZUC_F8:
            strncpy(metaData->cipherMode, "CipherMode_ZUC_F8", 40);
            break;
      case   sa_CipherMode_NULL:
      default:
            strncpy(metaData->cipherMode, "CipherMode_NULL", 40);
            break;
    }

    switch (authMode)
    {
      case   sa_AuthMode_MD5:
            strncpy(metaData->authMode, "AuthMode_MD5", 40);
            break;
      case   sa_AuthMode_SHA1:
          strncpy(metaData->authMode, "AuthMode_SHA1", 40);
          break;
      case   sa_AuthMode_SHA2_224:
          strncpy(metaData->authMode, "AuthMode_SHA2_224", 40);
          break;
      case   sa_AuthMode_SHA2_256:
          strncpy(metaData->authMode, "AuthMode_SHA2_256", 40);
          break;
      case   sa_AuthMode_SHA2_384:
          strncpy(metaData->authMode, "AuthMode_SHA2_384", 40);
          break;
      case   sa_AuthMode_SHA2_512:
          strncpy(metaData->authMode, "AuthMode_SHA2_512", 40);
          break;
      case   sa_AuthMode_HMAC_MD5:
          strncpy(metaData->authMode, "AuthMode_HMAC_MD5", 40);
          break;
      case   sa_AuthMode_HMAC_SHA1:
          strncpy(metaData->authMode, "AuthMode_HMAC_SHA1", 40);
          break;
      case   sa_AuthMode_HMAC_SHA2_224:
          strncpy(metaData->authMode, "AuthMode_HMAC_SHA2_224", 40);
          break;
      case   sa_AuthMode_HMAC_SHA2_256:
          strncpy(metaData->authMode, "AuthMode_HMAC_SHA2_256", 40);
          break;
      case   sa_AuthMode_HMAC_SHA2_384:
          strncpy(metaData->authMode, "AuthMode_HMAC_SHA2_384", 40);
          break;
      case   sa_AuthMode_HMAC_SHA2_512:
          strncpy(metaData->authMode, "AuthMode_HMAC_SHA2_512", 40);
          break;
      case   sa_AuthMode_GMAC:
          strncpy(metaData->authMode, "AuthMode_GMAC", 40);
          break;
      case   sa_AuthMode_GMAC_AH:
          strncpy(metaData->authMode, "AuthMode_GMAC_AH", 40);
          break;
      case   sa_AuthMode_CMAC:
          strncpy(metaData->authMode, "AuthMode_CMAC", 40);
          break;
      case   sa_AuthMode_CBC_MAC:
          strncpy(metaData->authMode, "AuthMode_CBC_MAC", 40);
          break;
      case   sa_AuthMode_AES_XCBC:
          strncpy(metaData->authMode, "AuthMode_AES_XCBC", 40);
          break;
      case   sa_AuthMode_KASUMI_F9:
          strncpy(metaData->authMode, "AuthMode_KASUMI_F9", 40);
          break;
      case   sa_AuthMode_SNOW3G_F9:
          strncpy(metaData->authMode, "AuthMode_SNOW3G_F9", 40);
          break;
      case   sa_AuthMode_ZUC_F9:
          strncpy(metaData->authMode, "AuthMode_ZUC_F9", 40);
          break;
      case   sa_AuthMode_NULL:
      default:
          strncpy(metaData->authMode, "AuthMode_NULL", 40);
          break;
    }

    return;
}

/* SA Data Mode Basic Functional test */
void saDataModeTest (void* a0, void* a1)
{

 	tFramework_t  *tf  = (tFramework_t *)(uintptr_t)a0;
 	saTest_t      *pat = (saTest_t *)(uintptr_t)a1;
    sauHdrHandle_t *pDmHdl;
 	int32_t  i;
#if ENABLE_TPUT_CONSOLE_PRINT
#if defined (NSS_LITE2)
    int32_t  j;
#endif
#endif

    uint16_t hdrSize = 12;  
    
    /* Initialize protocol addresses */
    /* Create test channels */
    for ( i = 0; i <  sizeof(testDmCfg)/sizeof(sauDataModeConfig_t) ; i += 2, hdrSize += 2)
    {
        /* Create a pair of Data Mode channels */
        if ((pDmHdl = sauCreateDm(NULL, hdrSize, &testDmCfg[i])) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create connection */
        if (sauCreateConnection(tf, pat, pDmHdl) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        if ((pDmHdl = sauCreateDm(NULL, hdrSize, &testDmCfg[i+1])) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Create connection */
        if (sauCreateConnection(tf, pat, pDmHdl) == NULL)
        {
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
    }


#if defined(NSS_LITE2)
    numTestPkt1 = 3;
    /* Packet Tests */
    sauConnDmPktTest(tf, pat, 
                     numTestPkt1,     /* Number of packets per connection */
                     180,             /* initial payload length */
                     3,               /* payload length increment step */
                     SAU_PAYLOAD_INC8 /* payload pattern */
                     );
    numTestPkt1 = 3;
    sauConnDmPktTest(tf, pat, 
                     numTestPkt1,     /* Number of packets per connection */
                     300,              /* initial payload length */
                     3,               /* payload length increment step */
                     SAU_PAYLOAD_INC8 /* payload pattern */
                     );
    /* There is a throughput test covering this case for k3
     * hence not running this for k3 to save QT time
     * enable this post silicon
     */
     /* Packet Tests */
     numTestPkt2 = SAU_TPUT_PKTBATCH_CNT;
     sauConnTputDmPktTest(tf, pat,
                      numTestPkt2      /* Number of packets per connection */
                      );

#if ENABLE_TPUT_CONSOLE_PRINT
    /* Summarize the throughput test results */
    SALog(" BenchMark Results: \n");
    SALog(" ================== \n ");
    System_flush();
    for (i = 0; i < SAU_TPUT_NUM_PKTSIZES; i++)
    {
        sauMetaDataInfo_t mData;
        for (j = 0; j < SAU_TPUT_MAX_CHANNELS; j++)
        {
            sauDataModeGetMetaDataFromConnId(gSauTputData[i][j].connId, &mData);
            SALog(" %s,\t\t%s,\t\t%d,\t\t%d,\t\t%d,\t\t%d,\t\t%d, \n", \
                            mData.cipherMode, \
                            mData.authMode,
                            gSauTputData[i][j].pktLen, \
                            gSauTputData[i][j].numPkts, \
                            gSauTputData[i][j].cycles,\
                            gSauTputData[i][j].ringPushCycles,\
                            gSauTputData[i][j].ringPopCycles\
                            );
            System_flush();
        }
    }
#endif /* ENABLE_TPUT_CONSOLE_PRINT */
#else

    /* Packet Tests */
    sauConnDmPktTest(tf, pat, 
                     numTestPkt1,     /* Number of packets per connection */
                     80,              /* initial payload length */
                     3,               /* payload length increment step (3)*/
                     SAU_PAYLOAD_INC8 /* payload pattern */
                     );

    sauConnDmPktTest(tf, pat, 
                     numTestPkt2,     /* Number of packets per connection */
                     300,             /* initial payload length */
                     6,               /* payload llength increment step */
                     SAU_PAYLOAD_INC8 /* payload pattern */
                     );
#endif

    /* Get Channel Statistics */
    salldSim_get_all_chan_stats();
    
    saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}

