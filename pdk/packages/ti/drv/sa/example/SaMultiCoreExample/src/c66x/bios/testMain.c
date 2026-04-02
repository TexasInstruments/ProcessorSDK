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
 *
 *  Configurations: Enable/Disable Null Encryption based on the test environment
 *      Global variable saNullEnc should be set based on the test environments
 *      as desribed below.
 *  
 * (C) Copyright 2009-2013, Texas Instruments, Inc. 
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
#include <ti/csl/cslr_cp_ace.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_bootcfgAux.h>
#include "testconn.h"

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

/* Test wrappers */
void saESPTest_bios (UArg a0, UArg a1) {
    saTestArgs_t tArgs;
    tArgs.tf  = (tFramework_t *)a0;
    tArgs.pat = (saTest_t *)a1;
    saESPTest((void *)&tArgs);
}

/* NULL terminated The list of tests */
saTest_t  saTestList[] = {
	{ saESPTest_bios,          "SA IPSEC ESP test",                   SA_TEST_NOT_RUN },
	{ NULL,                     NULL,                                  SA_TEST_NOT_RUN }
};


void topLevelTest (UArg a0, UArg a1);

/* SA REG base address */
uint32_t saRegBaseAddr =(uint32_t)CSL_NETCP_CFG_SA_CFG_REGS;

/* The exit code is a global. This is used so
 * the clock function can terminate the program with
 * the proper exit code */
Int exitCode;

t4Handles_t     l4Handles[SA_TEST_MAX_L4_HANDLES];
int numL4Handles;

static void saTestInit(void)
{
    t2Handles_t*  l2Handles;
    t2Handles_t*  l3Handles;
    sa_MCExample_entry_t val;

    if (coreNum == SYSINIT)
    {
    	val = fw_shmGetEntry(getPaL2HndCmd);
    	l2Handles =  val.l2Handle;
    	val = fw_shmGetEntry(getPaL3HndCmd);
        l3Handles =  val.l3Handle;
    	/* Zero out the handle arrays and packet counts */
        memset (l2Handles, 0, sizeof(l2Handles));
        memset (l3Handles, 0, sizeof(l3Handles));

    }
	/* Zero out the handle arrays and packet counts */
    memset (l4Handles, 0, sizeof(l4Handles));
}    


/* Creates a single task - the top level task. This is a low priority task that
 * spawns the individual tests */
extern cregister volatile unsigned int TSCL;

extern uint8_t ScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE];

#ifdef  SIMULATOR_SUPPORT
uint32_t autodetectLogic = FALSE;
#else
uint32_t autodetectLogic = TRUE;
#endif
void main ()
{
	Task_Params tparams;
	uint32_t    bootMode;
    
    salldsimScBuf = ScBuf;

	TSCL = 1;

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

	/* The only initial task is the top level test */
	Task_Params_init (&tparams);
	tparams.instance->name = "Top Level Test";
	tparams.priority      = 1;
	
	Task_create (topLevelTest, &tparams, NULL);
	
	/* Start the cycle counter */
	TSCL = 1;
	
	BIOS_start ();
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

    /* Get the core number. */
    coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);

    /* For some reason some printfs are lost unless there is a delay between System_flush
	 * and System_exit, so delay is forced */	
	Clock_Params_init(&clkParams);
    clkParams.period    = 0;
    clkParams.startFlag = FALSE;
    clkh = Clock_create(clk1Fxn, 1, &clkParams, NULL);
    
    /* Disable L1 and L2 Cache */
    CACHE_wbAllL1d (CACHE_WAIT);
    CACHE_setL1DSize(CACHE_L1_0KCACHE);
    CACHE_setL1PSize(CACHE_L1_0KCACHE);
    CACHE_setL2Size(CACHE_0KCACHE);
    
    System_printf ("\n\n ------- SA Multicore Test Starting (CORE ID: %d)---------\n", coreNum);
    System_flush ();

    /* Configure task parameters common to all test tasks */
    Task_Params_init (&tparams);
    tparams.priority = 2;
    tparams.stackSize = 4096;
    tparams.arg0     = (UArg) &tFramework;
    tFramework.fNullEnc = saNullEnc;

    if (coreNum == SYSINIT)
    {
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
    }
    else
    {
    	fw_shmOpen();
        System_printf ("Waiting for global config (core %d) ...\n", coreNum);
        APP_waitGlobalCfgDone();

    	/* Initialize the PA, PA cpdma, QM and CPPI. Each test will use
         * the same framework */
        if (setupTestFrameworkLocal ())  {
        	System_printf ("topLevelTest (%s:%d): setupTestFramework local returned error, exiting\n", __FILE__, __LINE__);
        	System_flush ();
        	exitCode = -1;
        	Clock_start(clkh);
        	Task_exit ();
        }
    }

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
		  System_printf ("%s:  PASSED (CORE ID: %d)\n", saTestList[i].name, coreNum);
		else
		  System_printf ("%s:  FAILED (CORE ID: %d)\n", saTestList[i].name, coreNum);
          
    	System_flush();
          
        if (coreNum == SYSINIT) {
		   /* Do a quick check of the test framework */
		   if (verifyTestFramework ())  {
			   System_printf ("topLevelTest (%s:%d): verifyTestFramework returned error after test %s. Exiting.\n", __FILE__, __LINE__, saTestList[i].name);
			   exitCode = -1;
			   System_flush ();
			   Clock_start(clkh);
		   }
        }
	} 

	/* System Clean up */
	if (coreNum == SYSINIT)
	{
		/* wait until clean up is done from all slave cores for final clean up from master */
		APP_waitSlaveCleanupCompletion();
		/* Exit the Test framework */
	    if (exitTestFramework ())  {
	    	System_printf ("topLevelTest (%s:%d): exitTestFramework returned error (core (%d)), exiting\n", __FILE__, __LINE__, coreNum);
	    	System_flush ();
	    }
	    else
	    {
	    	System_printf("topLevelTest: exitTestFramework Successful \n");
	    }
	}
	else
	{
	    /* Exit the Test framework */
	    if (exitTestFrameworkLocal ())  {
	    	System_printf ("topLevelTest (%s:%d): exitTestFrameworkLocal returned error (core (%d)), exiting\n", __FILE__, __LINE__, coreNum);
	    	System_flush ();
	    }
	    else
	    {
	    	System_printf("topLevelTest: exitTestFramework Successful \n");
	    }
	    APP_publishSlaveCleanupDone();
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
	
	System_printf ("\n\nTest summary(core id %d):\n\tTests Passed: %d\n\tTests Failed: %d\n\tTests not run: %d\n\n",
	  coreNum, passCount, failCount, notRunCount);
	


	System_printf ("\n\n ------- SA Multicore Test Complete (CORE ID: %d)---------\n", coreNum);
	System_flush();
	Clock_start(clkh);
	
	Task_exit();
}




