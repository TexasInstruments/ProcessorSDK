/*
 *
 * Copyright (C) 2011-2013 Texas Instruments Incorporated - http://www.ti.com/ 
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
#include "../salldsim/salldsim.h"

/* TRNG (True Random Number Generator) test.
 * This test is designed to verify the opertions of the True Random Number
 * Generator (TRNG) within SASS.
 *
 * Test Procedures:
 * 
 * - Configure and enable TRNG with all default parameters
 *   - Clock Divider = 1
 *   - Startup Cycle = 512 (2**8)
 *   - Minimum Refill Cycle = 64 (2**6)
 *   - Maxmium Refill Cycle = 2**24
 *
 * - Extract (and display) 64-bit random number every 200 CPU cycles
 * - Report number of random number received per 100 tries
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */
 
#define  SA_RNG_NUM_RANDOM_NUMBER       100

#if defined (NSS_LITE2)
uint32_t gPStr[sa_MAX_PS_AI_WORD_COUNT] = {
        0xaa6bbcab,
        0xef45e339,
        0x136ca1e7,
        0xbce1c881,
        0x9fa37b09,
        0x63b53667,
        0xb36e0053,
        0xa202ed81,
        0x4650d90d,
        0x8eed6127,
        0x666f2402,
        0x0dfd3af9
};
#endif

static uint16_t saRngTestPerCfg(Sa_Handle salld_handle, uintptr_t cfg, uintptr_t trngData)
{
#if defined (NSS_LITE2)
    Sa_Rng2ConfigParams_t *rng2Cfg = (Sa_Rng2ConfigParams_t *)cfg;
    Sa_Rng2Data_t *rngData = (Sa_Rng2Data_t *)trngData;
#else
    Sa_RngConfigParams_t *rngCfg = (Sa_RngConfigParams_t *)cfg;
    Sa_RngData_t *rngData = (Sa_RngData_t *)trngData;
#endif
    int  i, numRn;
    int16_t retCode;
    
    /* Initialize  TRNG */
#if defined (NSS_LITE2)
    /* Configure the TRNG module to generate True random number  */
    retCode = Sa_rng2Init(salld_handle, rng2Cfg);
#else
    /* Configure the TRNG module to generate True random number  */
    retCode = Sa_rngInit(salld_handle, rngCfg);
#endif
    if (retCode != sa_ERR_OK)
    {
        goto exitFromHere;
    }
    
    /* Wait for the first random number */
    #ifndef SIMULATOR_SUPPORT
    utilCycleDelay(20000);
    #else
    utilCycleDelay(200000);
    #endif
    
    /* Random Number Test */
    for (i = 0, numRn = 0; i < 100;)
    {
#if defined (NSS_LITE2)
        retCode = Sa_getRandomNum2 (salld_handle, FALSE, &rngData[numRn]);
#else
        retCode = Sa_getRandomNum (salld_handle, FALSE, &rngData[numRn]);
#endif
        if ( retCode == sa_ERR_OK)
        {
            numRn++; i++;
        }
        else if (retCode == sa_ERR_MODULE_BUSY)
        {
            /* Wait for RND number to be gernated, poll again */
        }
        else
        {
            salld_sim_halt();
        }
        #ifndef SIMULATOR_SUPPORT
        utilCycleDelay (200);
        #else
        utilCycleDelay (10000);
        #endif
    }    
    
    /* Display the random numbers */
    for (i = 0;  i < numRn; i++)
    {
        salld_sim_print("saRngTest: Random Number %d = 0x%08x%08x!\n", i, rngData[i].hi, rngData[i].lo);
    }

    salld_sim_disp_control(TRUE);
    salld_sim_print("saRngTest: %d (out of %d) Random Number are generated!\n", numRn, SA_RNG_NUM_RANDOM_NUMBER);
    salld_sim_disp_control(FALSE);

    /* Close the RNG */
    retCode = Sa_rngClose(salld_handle);

exitFromHere:
    return (retCode);
}


/* SA TRNG test */
void saRngTest (void* a0, void* a1)
{
 	tFramework_t  *tf  = (tFramework_t *)(uintptr_t)a0;
 	saTest_t      *pat = (saTest_t *)(uintptr_t)a1;
#if defined (NSS_LITE2)
    Sa_Rng2ConfigParams_t rngCfg;
    Sa_Rng2Data_t rngData[SA_RNG_NUM_RANDOM_NUMBER];
    int i;
#else
    Sa_RngConfigParams_t rngCfg;
    Sa_RngData_t rngData[SA_RNG_NUM_RANDOM_NUMBER];
#endif
    int16_t retCode;
    
    /* Initialize  TRNG */
#if defined (NSS_LITE2)
    memset(&rngCfg, 0, sizeof(Sa_Rng2ConfigParams_t));
    rngCfg.ctrlBitfield = sa_RNG2_CTRL_REINIT;   /* Force re-initialize */
    rngCfg.clockDiv = 0; /* default (1) */
    rngCfg.sampleCycles = 1024; /* default (512) */
#else
    memset(&rngCfg, 0, sizeof(Sa_RngConfigParams_t));
    rngCfg.ctrlBitfield = sa_RNG_CTRL_REINIT;   /* Force re-initialize */
    rngCfg.clockDiv = 0; /* default (1) */
    rngCfg.startupCycles = 0; /* default (2**8) */
    rngCfg.minRefillCycles = 0; /* default (2**6) */
    rngCfg.maxRefillCycles = 0; /* default (2**24) */
#endif

#if defined (NSS_LITE2)
    salld_sim_disp_control(TRUE);    
    salld_sim_print("saRng2Test: Testing for RND generation without DRBG \n");
    salld_sim_disp_control(FALSE);
#endif

    /* Test the RND number generation */
    retCode = saRngTestPerCfg(tf->salld_handle, (uintptr_t)&rngCfg, (uintptr_t)&rngData);
    if (retCode != sa_ERR_OK)
    {
        salld_sim_disp_control(TRUE);    
        salld_sim_print("saRngTest: Sa_rngInit() returns error code = %d!\n", retCode);
        salld_sim_disp_control(FALSE);    
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }

#if defined (NSS_LITE2) && defined(DRBG_KNOWN_ANSWER_TEST)
    /* Now test with "known-answer-tests" */
    rngCfg.ctrlBitfield |= sa_RNG2_CTRL_DRBG_KNOWN_TESTS;
    retCode = saRngTestPerCfg(tf->salld_handle, (uintptr_t)&rngCfg, (uintptr_t) &rngData);
    if (retCode != sa_ERR_OK)
    {
        salld_sim_disp_control(TRUE);    
        salld_sim_print("saRngTest: Sa_rngInit() returns error code = %d!\n", retCode);
        salld_sim_disp_control(FALSE);    
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    /* Clear the known anwer test */
    rngCfg.ctrlBitfield &= ~sa_RNG2_CTRL_DRBG_USE;
#endif

#if defined (NSS_LITE2)
    salld_sim_disp_control(TRUE);    
    salld_sim_print("saRng2Test: Testing for RND generation with DRBG \n");
    salld_sim_disp_control(FALSE);

    /* Now test with DRBG cfg for SA2UL */
    rngCfg.ctrlBitfield |= sa_RNG2_CTRL_DRBG_USE;
    rngCfg.pStringLen         = 12;
    for ( i = 0; i < sa_MAX_PS_AI_WORD_COUNT; i++)
    {
        rngCfg.pStringData[i]    = gPStr[i];
    }
    /* Test the RND number generation */
    retCode = saRngTestPerCfg(tf->salld_handle, (uintptr_t)&rngCfg, (uintptr_t)&rngData);
    if (retCode != sa_ERR_OK)
    {
        salld_sim_disp_control(TRUE);    
        salld_sim_print("saRngTest: Sa_rngInit() returns error code = %d!\n", retCode);
        salld_sim_disp_control(FALSE);    
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
#endif
    saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}


