/******************************************************************************
 * FILE PURPOSE: Main routine for exercising SA Basic Example.
 *
 ******************************************************************************
 * FILE NAME:    testMain.c
 *
 * DESCRIPTION:  The SA Basic examples are designed to demonstrate the basic
 *               SASS functionalities and SA LLD call flows for all supported
 *               security protocols such as IPSEC, SRTP and etc. It contains
 *               the following files:
 *
 *  testMain.c:  Primary test routines and the test table
 *  testconn.c:  System routines to establish connections and packet generation
 *               and other common test functionalities
 *  testmem.c:   Static memory allocation for test framework
 *  testutil.c:  Utility functions
 *  framework.c: Functions to setup system, CPPI, QMSS, PA and SA to establish
 *               test frameworks
 *  common.c:    Common system functions used by PA and SA
 *  salldsim:    Directory contains SA LLD API calls and configuration files
 *  test1.c:     Use case: IPSEC ESP
 *  test2.c:     Use case: IPSEC AH
 *  test3.c:     Use case: SRTP
 *  test4.c:     Use case: SRTP over IPSEC ESP
 *  test5.c:     Use case: SRTP over IPSEC AH
 *  test6.c:     Use case: 3GPP Air Ciphering LTE
 *  test7.c:     Use case: Data Mode
 *
 *  Configurations: Enable/Disable Null Encryption based on the test environment
 *      Global variable saNullEnc should be set based on the test environments
 *      as desribed below.
 *
 * (C) Copyright 2009-2018, Texas Instruments, Inc.
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

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#include <ti/csl/cslr_cp_ace.h>
#include <ti/csl/csl_bootcfgAux.h>
#endif

/*
 * Enable/Disable Null Encryption:
 *
 * TRUE: Null encryption and authentication with fixed hash data 0x10, 0x11, 0x12 ...
 * FALSE: Real encryption and authentication
 *
 * Note: The Keystone simulator only supports null encryption without the tisim_crypto.dll
 */
#ifdef SIMULATOR_SUPPORT
Bool saNullEnc = TRUE;
#else
Bool saNullEnc = FALSE;
#endif

#if !defined(NSS_LITE2)
/* Test wrappers */
void saESPTest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saESPTest((void *)&tArgs);
}

void saAHTest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saAHTest((void *)&tArgs);
}

void saSRTPTest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saSRTPTest((void *)&tArgs);
}

void saSrtpEspTest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saSrtpEspTest((void *)&tArgs);
}

void saSrtpAhTest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saSrtpAhTest((void *)&tArgs);
}

void saLTETest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saLTETest((void *)&tArgs);
}
#endif

void saDataModeTest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saDataModeTest((void *)&tArgs);
}

/* SA REG base address */
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
uint32_t saRegBaseAddr =(uint32_t)CSL_NETCP_CFG_SA_CFG_REGS;
#elif defined (NSS_LITE2)
uintptr_t saRegBaseAddr = (uintptr_t) CSL_SA2_UL0_BASE;
uint32_t no_bootMode = TRUE; /* assuming SA Examples would always work under NO Boot Mode for AM65X or J721E examples */
#else
uintptr_t saRegBaseAddr = (uintptr_t) (CSL_NSS_0_CFG_REGS + 0x00400000);
#endif

/* NULL terminated The list of tests */
saTest_t  saTestList[] = {
#if !defined(NSS_LITE2) && !defined(NSS_LITE)
    { saESPTest_bios,          "SA IPSEC ESP test",                   SA_TEST_NOT_RUN },
    { saAHTest_bios,           "SA IPSEC AH test",                    SA_TEST_NOT_RUN },
    { saSRTPTest_bios,         "SA SRTP test",                        SA_TEST_NOT_RUN },
    { saSrtpEspTest_bios,      "SA SRTP over ESP test",               SA_TEST_NOT_RUN },
    { saSrtpAhTest_bios,       "SA SRTP over AH test",                SA_TEST_NOT_RUN },
    #ifdef SA_3GPP_SUPPORT
	{ saLTETest_bios,          "SA 3GPP LTE test",                    SA_TEST_NOT_RUN },
    #endif
#endif
	{ saDataModeTest_bios,     "SA Data Mode test",                   SA_TEST_NOT_RUN },
	{ NULL,                    NULL,                                  SA_TEST_NOT_RUN }
};

void topLevelTest (UArg a0, UArg a1);

/* The exit code is a global. This is used so
 * the clock function can terminate the program with
 * the proper exit code */
Int exitCode;

#if !defined (NSS_LITE2)
paSysStats_t saTestExpectedStats;  /* Expected stats results */

t2Handles_t     l2Handles[10];
t2Handles_t     l3Handles[10];
t4Handles_t     l4Handles[10];
#endif

static void saTestInit(void)
{
#if !defined (NSS_LITE2)
    /* Zero out the handle arrays and packet counts */
    memset (l2Handles, 0, sizeof(l2Handles));
    memset (l3Handles, 0, sizeof(l3Handles));
    memset (l4Handles, 0, sizeof(l4Handles));
#endif
}


/* Creates a single task - the top level task. This is a low priority task that
 * spawns the individual tests */
#ifdef _TMS320C6X
extern cregister volatile unsigned int TSCL;
#endif

extern uint8_t ScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE];
extern uint8_t PktRam[TEST_PKT_RAM_SIZE];

#ifdef  SIMULATOR_SUPPORT
uint32_t autodetectLogic = FALSE;
#else
uint32_t autodetectLogic = TRUE;
#endif

#if defined(NSS_LITE2)
void saTestRecoverAndExit (tFramework_t *tf, saTest_t *pat, saTestStatus_t testResult)
{
    /* Set the result code */
    pat->testStatus = testResult;
    return;
}
#endif

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
void main ()
#else
int main(void)
#endif
{
	Task_Params tparams;
#if !defined(NSS_LITE2) && !defined(NSS_LITE)
	uint32_t    bootMode;
#endif

#ifdef __ARM_ARCH_7A__
    /* Add MMU entries for MMR's required for PCIE example */
    uint32_t privid, index;
    CSL_MsmcRegs *msmc = (CSL_MsmcRegs *)CSL_MSMC_CFG_REGS;
    Mmu_DescriptorAttrs attrs;
    extern char ti_sysbios_family_arm_a15_Mmu_Module_State_0_secondLevelTableBuf_1__A;
    uint32_t addr = (uint32_t)&ti_sysbios_family_arm_a15_Mmu_Module_State_0_secondLevelTableBuf_1__A;

    Mmu_initDescAttrs(&attrs);

    attrs.type = Mmu_DescriptorType_TABLE;
    attrs.shareable = 0;            // non-shareable
    attrs.accPerm = 1;              // read/write at any privelege level
    attrs.attrIndx = 0;             // Use MAIR0 Register Byte 3 for
                                    // determining the memory attributes
                                    // for each MMU entry


    // Update the first level table's MMU entry for 0x80000000 with the
    // new attributes.
    Mmu_setFirstLevelDesc((Ptr)0x40000000, (UInt64)addr, &attrs);

    // Set up SES & SMS to make all masters coherent
    for (privid = 0; privid < 16; privid++)
    {
      for (index = 0; index < 8; index++)
      {
        uint32_t ses_mpaxh = msmc->SES_MPAX_PER_PRIVID[privid].SES[index].MPAXH;
        uint32_t sms_mpaxh = msmc->SMS_MPAX_PER_PRIVID[privid].SMS[index].MPAXH;
        if (CSL_FEXT (ses_mpaxh, MSMC_SES_MPAXH_0_SEGSZ) != 0)
        {
          // Clear the "US" bit to make coherent.  This is at 0x80.
          ses_mpaxh &= ~0x80;
          msmc->SES_MPAX_PER_PRIVID[privid].SES[index].MPAXH = ses_mpaxh;
        }
        if (CSL_FEXT (sms_mpaxh, MSMC_SMS_MPAXH_0_SEGSZ) != 0)
        {
          // Clear the "US" bit to make coherent.  This is at 0x80.
          sms_mpaxh &= ~0x80;
          msmc->SMS_MPAX_PER_PRIVID[privid].SMS[index].MPAXH = sms_mpaxh;
        }
      }
    }
#endif

#ifdef _TMS320C6X
	TSCL = 1;
#endif

#if defined(NSS_LITE2) && !defined(NSS_LITE)
    /* No user mode support for NSS_LITE and hence no boot mode is true always */
    no_bootMode = TRUE;
#else
    if (autodetectLogic == TRUE)
    {
       bootMode = CSL_BootCfgGetBootMode() & 0x7;

       if (bootMode == 0)
    	  no_bootMode = TRUE;
       else
    	  no_bootMode = FALSE;
    }
    else {
    	no_bootMode = TRUE;
    }
    /* Zero out the expected stats. The stats will be updated as packets are sent into PA */
    memset (&saTestExpectedStats, 0, sizeof(saTestExpectedStats));
#endif

    salldsimScBuf = ScBuf;
    testPktRam = PktRam;

	/* The only initial task is the top level test */
	Task_Params_init (&tparams);
	tparams.instance->name = "Top Level Test";
	tparams.priority      = 1;

	Task_create (topLevelTest, &tparams, NULL);

#ifdef _TMS320C6X
	/* Start the cycle counter */
	TSCL = 1;
#endif

	BIOS_start ();

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#else
	return (0);
#endif
}

void clk1Fxn (UArg a0)
{
	BIOS_exit (exitCode);
}

/* Initialize the test framework and launch the individual tests */
void topLevelTest (UArg a0, UArg a1)
{
	Task_Params tparams;
	Task_Handle thandle;
	Task_Stat   tstat;

	Clock_Handle clkh;
    Clock_Params clkParams;

	Int i;

	Int passCount;
	Int failCount;
	Int notRunCount;

	/* For some reason some printfs are lost unless there is a delay between System_flush
	 * and System_exit, so delay is forced */
	Clock_Params_init(&clkParams);
    clkParams.period    = 0;
    clkParams.startFlag = FALSE;
    clkh = Clock_create(clk1Fxn, 1, &clkParams, NULL);

#ifdef _TMS320C6X
    /* Disable L1 and L2 Cache */
    CACHE_wbAllL1d (CACHE_WAIT);
    CACHE_setL1DSize(CACHE_L1_0KCACHE);
    CACHE_setL1PSize(CACHE_L1_0KCACHE);
    CACHE_setL2Size(CACHE_0KCACHE);
#endif

    System_printf ("\n\n ------- SA Unit Test Starting ---------\n");
    System_flush ();

    /* Initialize the PA, PA cpdma, QM and CPPI. Each test will use
     * the same framework */
    if (setupTestFramework ())  {
    	System_printf ("topLevelTest (%s:%d): setupTestFramework returned error, exiting\n", __FILE__, __LINE__);
    	System_flush ();
    	exitCode = -1;
    	Clock_start(clkh);
    	Task_exit ();
    }

    /* Make sure the setup matches what is expected */
    if (verifyTestFramework())  {
    	System_printf ("topLevelTest (%s:%d): verifyTestFramework returned error after initial framework setup, exiting\n", __FILE__, __LINE__);
    	System_flush();
    	exitCode = -1;
    	Clock_start(clkh);
    	Task_exit ();
    }

    /* Configure task parameters common to all test tasks */
    Task_Params_init (&tparams);
    tparams.priority = 2;
    tparams.arg0     = (UArg) &tFramework;
    tparams.stackSize = 4096;
    tFramework.fNullEnc = saNullEnc;

	/* Run the tests */
	for (i = 0; saTestList[i].testFunction != NULL; i++ )  {

        saTestInit();

		tparams.arg1 = (UArg)&saTestList[i];
		tparams.instance->name = saTestList[i].name;

		thandle = Task_create (saTestList[i].testFunction, &tparams, NULL);

		/* The test task will terminate upon completion. Verify that the
		 * task has completed in case the task itself uses multiple tasks
		 * that all wind up idle for a while. */
		do  {
			Task_stat (thandle, &tstat);
		} while (tstat.mode != Task_Mode_TERMINATED);

        Task_delete (&thandle);

		if (saTestList[i].testStatus == SA_TEST_PASSED)
		  System_printf ("%s:  PASSED\n", saTestList[i].name);
		else
		  System_printf ("%s:  FAILED\n", saTestList[i].name);

    	System_flush();


		/* Do a quick check of the test framework */
		if (verifyTestFramework ())  {
			System_printf ("topLevelTest (%s:%d): verifyTestFramework returned error after test %s. Exiting.\n", __FILE__, __LINE__, saTestList[i].name);
			exitCode = -1;
			System_flush ();
			Clock_start(clkh);
		}
	}

	/* Summarize the test results */
	for (i = passCount = failCount = notRunCount = 0; saTestList[i].testFunction != NULL; i++)  {
		if (saTestList[i].testStatus == SA_TEST_PASSED)
			passCount += 1;
		else if (saTestList[i].testStatus == SA_TEST_FAILED)
			failCount += 1;
		else
			notRunCount += 1;
	}

	System_printf ("\n\nTest summary:\n\tTests Passed: %d\n\tTests Failed: %d\n\tTests not run: %d\n\n",
	  passCount, failCount, notRunCount);


    /* Exit the Test framework */
    if (exitTestFramework ())  {
    	System_printf ("topLevelTest (%s:%d): exitTestFramework returned error, exiting\n", __FILE__, __LINE__);
    	System_flush ();
    	exitCode = -1;
    	Clock_start(clkh);
    	Task_exit ();
    }
    else
    {
    	System_printf("topLevelTest: exitTestFramework Successful \n");
    }

	System_printf ("\n\n ------- SA Unit Test Complete ---------\n");
	System_flush();
	Clock_start(clkh);

	Task_exit();
}




