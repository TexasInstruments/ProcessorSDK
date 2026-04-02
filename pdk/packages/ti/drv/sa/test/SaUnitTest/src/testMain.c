/******************************************************************************
 * FILE PURPOSE: Main routine for exercising SA Unit Tests.
 *
 ******************************************************************************
 * FILE NAME:    testMain.c
 *
 * DESCRIPTION:  The SA unit tests are designed to veriy the SASS and SA LLD
 *               functionalities for all supported security protocols such as 
 *               IPSEC, SRTP and etc. Please refer to the SA LLD test plan and 
 *               the description in each test file for details. 
 *
 *               It contains the following files and directories:
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
 *  test:        Directory contains the test files for all test cases          
 *
 *  Configurations: Enable/Disable Test log
 *      Global variable saTestLogEnable should be set or clear as desribed below.
 *  
 * (C) Copyright 2009-2020, Texas Instruments, Inc. 
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
#include "testconn.h"
#include <ti/csl/arch/a53/csl_a53.h>

#if (defined(USE_BIOS) && defined(_TMS320C6X))
#include <ti/sysbios/family/c66/Cache.h>
#endif

#ifdef DEVICE_K2G
void saTestInitPerfCounters()
{
#if defined (_TMS320C6X)
#else
     // program the performance-counter control-register:
    __asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(0x17));

    // enable all counters:
    __asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));

    // clear overflows:
    __asm__ __volatile__ ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));
#endif
}

#endif
/* Enable/Disable SA Unit Test log display */
Bool saTestLogEnable = FALSE;

/* 
 * Enable/Disable Simulator:
 *  
 * TRUE: simulator is used 
 * FALSE: silicon is used
 *
 * Note: This varaible is used to determine some test parameters such as number of test packets,
 *       number of test channels to avoid the long test time at simulator.
 */
#ifdef SIMULATOR_SUPPORT
Bool saSimTest = TRUE;
#else
Bool saSimTest = FALSE;
#endif


/* NULL terminated The list of tests */
saTest_t  saTestList[] = {
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
	{ saESPTest,          "SA IPSEC ESP test",                   SA_TEST_NOT_RUN },
    { saAHTest,           "SA IPSEC AH test",                    SA_TEST_NOT_RUN },
	{ saESPNATTTest,      "SA IPSEC ESP NAT-T test",             SA_TEST_NOT_RUN },
    { saSRTPTest,         "SA SRTP test",                        SA_TEST_NOT_RUN },
    { saSRTCPTest,        "SA SRTCP test",                       SA_TEST_NOT_RUN },
    { saSrtpEspTest,      "SA SRTP over ESP test",               SA_TEST_NOT_RUN },
    { saSrtpAhTest,       "SA SRTP over AH test",                SA_TEST_NOT_RUN },
	{ saESPMultiSgmtTest, "SA IPSEC ESP Multi-segment test",     SA_TEST_NOT_RUN },
	{ saAHMultiSgmtTest,  "SA IPSEC AH Multi-segment test",      SA_TEST_NOT_RUN },
	{ saSRTPMultiSgmtTest,"SA SRTP Multi-segment test", 	     SA_TEST_NOT_RUN },
	{ saSRTCPMultiSgmtTest,"SA SRTCP Multi-segment test", 	     SA_TEST_NOT_RUN },
    #ifdef SA_3GPP_SUPPORT
	{ saAirCipheringTest, "SA 3GPP Air Ciphering test",          SA_TEST_NOT_RUN },
	{ saAirCipheringKeyGenTest, "SA 3GPP Air Ciphering Key stream generation test",          SA_TEST_NOT_RUN },
    #endif
	{ saSRTPRekeyTest1,   "SA SRTP Rekey test 1",                SA_TEST_NOT_RUN },
	{ saSRTPRekeyTest2,   "SA SRTP Rekey test 2",                SA_TEST_NOT_RUN },
	{ saSRTPRekeyTest3,   "SA SRTP Rekey test 3",                SA_TEST_NOT_RUN },
	{ saSRTPReKeyTest4,   "SA SRTP Rekey test 4",           	 SA_TEST_NOT_RUN },
	{ saSRTCPRekeyTest1,  "SA SRTCP Rekey test 1",               SA_TEST_NOT_RUN },
	{ saSRTCPRekeyTest2,  "SA SRTCP Rekey test 2",               SA_TEST_NOT_RUN },
	{ saESPReplayTest,    "SA IPSEC ESP Replay test",            SA_TEST_NOT_RUN },
    /* There are no PA stats check for the following tests due to errors expected */
	{ saAHReplayTest,     "SA IPSEC AH Replay test",             SA_TEST_NOT_RUN },
	{ saSRTPReplayTest,   "SA SRTP Replay test",                 SA_TEST_NOT_RUN },
	{ saSRTCPReplayTest,  "SA SRTCP Replay test",                SA_TEST_NOT_RUN },
	{ saESPErrTest,       "SA IPSEC ESP Error Handling test",    SA_TEST_NOT_RUN },
    { saAHErrTest,        "SA IPSEC AH Error Handling test",     SA_TEST_NOT_RUN },
    { saSRTPErrTest,      "SA SRTP Error Handling test",         SA_TEST_NOT_RUN },
    { saSRTCPErrTest,     "SA SRTCP Error Handling test",        SA_TEST_NOT_RUN },
	{ saDataModeTest,     "SA Data Mode test",                   SA_TEST_NOT_RUN },
	{ saRngTest,          "SA TRNG test",                        SA_TEST_NOT_RUN },
    #ifndef SIMULATOR_SUPPORT    
	{ saPkaTest,          "SA PKA test",                         SA_TEST_NOT_RUN },
    #endif 
#else
	{ saDataModeTest,     "SA Data Mode test",                   SA_TEST_NOT_RUN },
#if !defined(SOC_AM64X)
/* Will run post si */
	{ saRngTest,          "SA TRNG test",                        SA_TEST_NOT_RUN },
#endif
	{ saPkaTest,          "SA PKA Basic Operation test",         SA_TEST_NOT_RUN },
	{ saPkaTest2,         "SA PKA Complex Operation test",       SA_TEST_NOT_RUN },
#endif
	{ NULL,               "",                                    SA_TEST_NOT_RUN }
};

#if defined(USE_RTOS)
void topLevelTest (void * a0, void * a1);
#else
void topLevelTest(int32_t a0, int32_t a1);
#endif

/*
 * Stack for the task.
 */
#ifdef USE_RTOS
#define APP_TSK_STACK_MAIN (16U * 1024U)
static uint8_t gAppTskStacksaTestList[sizeof(saTestList)/sizeof(saTestList[0])][APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
static uint8_t gAppTskStacktopLevelTest[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
#endif

/* The exit code is a global. This is used so
 * the clock function can terminate the program with
 * the proper exit code */
Int exitCode = 0;
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
paSysStats_t paTestExpectedStats;  /* Expected stats results */

t2Handles_t     l2Handles[SA_TEST_MAX_L2_HANDLES];
t2Handles_t     l3Handles[SA_TEST_MAX_L3_HANDLES];
t4Handles_t     l4Handles[SA_TEST_MAX_L4_HANDLES];
int numL2Handles, numL3Handles, numL4Handles;
#endif

#ifdef USE_BIOS
#if defined(NSS_LITE2)
#include <ti/sysbios/family/arm/v8a/Mmu.h>

volatile int emuwait_mmu=1;

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

#endif /* NSS_LITE2 */

#endif /* NOT BAREMETAL */

void saTestRecoverAndExit (tFramework_t *tf, saTest_t *pat, saTestStatus_t testResult)
{
#ifndef NSS_LITE2
  Cppi_HostDesc *hd;
  Int numCon;
#endif
	/* Set the result code */
	pat->testStatus = testResult;
	
	/* Restore the packet recycle queues */
  #ifndef NSS_LITE2
	while (Qmss_getQueueEntryCount ((tf->QGen)[Q_DATA_RECYCLE]))  {
		
		hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tf->QGen[Q_DATA_RECYCLE])) & ~15);
		
		hd->buffPtr = hd->origBuffPtr;
		hd->buffLen = hd->origBufferLen;
		
		Qmss_queuePushDesc (tf->QfreeDesc, (Ptr)hd);
	}
  #endif
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    /* Zero out the expected stats. The stats will be updated as packets are sent into PA */
    memset (&paTestExpectedStats, 0, sizeof(paTestExpectedStats));
    
    /* Delete L2/L3/l4 Handles */
    if (testCommonDelL4Connection(tf, pat, numL4Handles, l4Handles, &numCon))
    {
        paTestExpectedStats.classify2.nPackets += numCon;    
    }
    else
    {
        pat->testStatus = SA_TEST_FAILED;
    }
    
    if (testCommonDelL2L3Connection(tf, pat, numL3Handles, l3Handles, &numCon))
    {
        paTestExpectedStats.classify1.nPackets += numCon;    
    }
    else
    {
        pat->testStatus = SA_TEST_FAILED;
    }
    
    if (testCommonDelL2L3Connection(tf, pat, numL2Handles, l2Handles, &numCon))
    {
        paTestExpectedStats.classify1.nPackets += numCon;    
    }
    else
    {
        pat->testStatus = SA_TEST_FAILED;
    }

#endif

    /* Close all SA LLD channels */
    salldSim_close_all_chns();
    
    /* Return */
    if (pat->testStatus == SA_TEST_FAILED)
    {
        #ifdef USE_BIOS
        Task_exit();
        #endif
    }
}

static void saTestInit(void)
{
    /* Zero out the handle arrays and packet counts */
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    memset (l2Handles, 0, sizeof(l2Handles));
    memset (l3Handles, 0, sizeof(l3Handles));
    memset (l4Handles, 0, sizeof(l4Handles));
    numL2Handles = 0;
    numL3Handles = 0;
    numL4Handles = 0;
#endif

    /* Zero out all connection realted handles */
    numHdrHandles = 0;
    numConnHandles = 0;
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    numSrtpHandles = 0;
    numIpsecHandles = 0;
#endif
    /* Zero out SA LLD channel instances */
    numSalldSimChans = 0;

}    

/* Creates a single task - the top level task. This is a low priority task that
 * spawns the individual tests */
#ifndef ARM11 
#ifdef _TMS320C6X
extern cregister volatile unsigned int TSCL;
#endif
#endif

#ifdef USE_RTOS
int main_rtos(void)
{
    TaskP_Params tparams;

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
#endif /* __ARM_ARCH_7A__ */
    /* Before calling any OSAL API's OS_init is needed */
    OS_init();

	/* The only initial task is the top level test */
	TaskP_Params_init (&tparams);
	tparams.name = (const char *)"Top Level Test";
	tparams.priority      = 1;
	tparams.stack = gAppTskStacktopLevelTest;
	tparams.stacksize = sizeof (gAppTskStacktopLevelTest);

	TaskP_create(&topLevelTest, &tparams);

#ifdef _TMS320C6X
	/* Start the cycle counter */
	TSCL = 1;
#endif

	OS_start();

    return (0);
}
#else
int main_baremetal(void)
{
	/* Start the cycle counter */
#ifdef _TMS320C6X
	TSCL = 1;
#endif

#if defined (__aarch64__)
    initialise_monitor_handles();
#endif
    topLevelTest((uintptr_t)NULL, (uintptr_t)NULL);
    return(0);
}
#endif

/*
 *  ======== Board_initSA ========
 */
#include <ti/board/board.h>
void Board_initSA(void);
void Board_initSA(void)
{
#if defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_AM64X)
    Board_initCfg boardCfg;
    Board_STATUS  status;

    boardCfg = BOARD_INIT_MODULE_CLOCK;
    status = Board_init(boardCfg);
    if (status != BOARD_SOK)
    {
        SALog(" Board_init() is not successful...unexpected results may happen \n");
    }
    else
    {
/* Need to uncomment this during bring up 
   For some reason, this hangs on QT
 */
#if !defined(SOC_AM64X)
        Osal_delay(1);
        boardCfg = BOARD_INIT_PINMUX_CONFIG;
        status = Board_init(boardCfg);
        if (status != BOARD_SOK)
        {
            SALog(" Board_init() is not successful...unexpected results may happen \n");
        }
#endif
    }
    if (status == BOARD_SOK)
    {
        Osal_delay(1);
        boardCfg =  BOARD_INIT_UART_STDIO;

        status = Board_init(boardCfg);
        if (status != BOARD_SOK)
        {
            SALog(" Board_init() is not successful...unexpected results may happen \n");
        }
    }
#endif

    return;
}

int main ()
{
    Board_initSA();
#ifdef USE_RTOS
    main_rtos();
#else
    main_baremetal();
#endif
  return(0);
}

void clk1Fxn (void* a0)
{
	OS_stop();
}

uint16_t numTestPkt1 = 200;
uint16_t numTestPkt2 = 40;
uint16_t numTestPkt3 = 40;

#ifndef __LINUX_USER_SPACE
#define  SAUTEST_CORE_DUMP_SIZE     sa_CORE_DUMP_BUF_SIZE
uint32_t coreDumpBuf[SAUTEST_CORE_DUMP_SIZE];
int      coreDump2File = FALSE;
#endif

/* Initialize the test framework and launch the individual tests */
#ifdef USE_RTOS
void topLevelTest (void * a0, void * a1)
#else
void topLevelTest(int32_t a0, int32_t a1)
#endif
{
#ifdef USE_RTOS
#if    SAUTEST_DISABLE_TASK_GATE_SEM
#else
	TaskP_Params tparams;
	TaskP_Handle thandle;

#endif
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  Clock_Params clkParams;
  Clock_Handle clkh;
  FILE         *fp;
#endif
#endif    
	Int i;
	
	Int passCount;
	Int failCount;
	Int notRunCount;
	
#if defined(USE_RTOS) && !defined(NSS_LITE) && !defined(NSS_LITE2)
	/* For some reason some printfs are lost unless there is a delay between System_flush
	 * and System_exit, so delay is forced */	
	ClockP_Params_init(&clkParams);
	clkParams.startMode = ClockP_StartMode_USER;
    clkParams.period    = 1;
    clkParams.runMode   = ClockP_RunMode_ONESHOT;
    clkh = ClockP_create(clk1Fxn, &clkParams);
#endif

#ifdef _TMS320C6X
#ifdef USE_BIOS
    Cache_Size setSize;

    setSize.l1pSize = Cache_L1Size_32K;
    setSize.l1dSize = Cache_L1Size_32K;
    setSize.l2Size = Cache_L2Size_0K;

    /* set the cache sizes */
    Cache_setSize(&setSize);
#else
    /* Disable L1 and L2 Cache */
    //CACHE_wbAllL1d (CACHE_WAIT);
    //CACHE_setL1DSize(CACHE_L1_0KCACHE);
    //CACHE_setL1PSize(CACHE_L1_0KCACHE);
    CACHE_setL2Size(CACHE_0KCACHE);
#endif
#endif

#ifdef DEVICE_K2G
    saTestInitPerfCounters();
#endif

    SALog ("\n\n ------- SA Unit Test Starting ---------\n");
    System_flush ();

    if (saSimTest)
    {
        /* reduce the number of test packets to shorten test time */
        numTestPkt1 = 200;  /* 10 */
        numTestPkt2 = 300;
        numTestPkt3 = 100;
    }

    /* Initialize the PA, PA cpdma, QM and CPPI. Each test will use
     * the same framework */
    if (setupTestFramework ())  {
    	SALog ("topLevelTest (%s:%d): setupTestFramework returned error, exiting\n", __FILE__, __LINE__);
    	System_flush ();
    	exitCode = -1;
        #ifdef USE_RTOS
#if defined (NSS_LITE) || defined(NSS_LITE2)
#if     SAUTEST_DISABLE_TASK_GATE_SEM
#else
    	OS_stop();
#endif
#else
    	ClockP_start(clkh);
#endif
        #endif
        #ifdef USE_BIOS
        Task_exit();
        #endif
    }
    
    /* Make sure the setup matches what is expected */
    if (verifyTestFramework())  {
    	SALog ("topLevelTest (%s:%d): verifyTestFramework returned error after initial framework setup, exiting\n", __FILE__, __LINE__);
    	System_flush();
    	exitCode = -1;
        #ifdef USE_RTOS
#if defined(NSS_LITE) || defined(NSS_LITE2)
#if     SAUTEST_DISABLE_TASK_GATE_SEM
#else
    	OS_stop();
#endif
#else
    	ClockP_start(clkh);
#endif
        #endif
        #ifdef USE_BIOS
        Task_exit();
        #endif
    }

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    /* Zero out the expected stats. The stats will be updated as packets are sent into PA */
    memset (&paTestExpectedStats, 0, sizeof(paTestExpectedStats));
#endif

#ifdef USE_RTOS
#if    SAUTEST_DISABLE_TASK_GATE_SEM
#else
    /* Configure task parameters common to all test tasks */
    TaskP_Params_init (&tparams);
    tparams.priority = 2;
    tparams.stacksize  = 8192;
    tparams.arg0     = (void*) &tFramework;
#endif
#endif
    
	/* Run the tests */
	for (i = 0; saTestList[i].testFunction != NULL; i++ )  {

        /*
         * If test has not run, we proceed. Otherwise skip (e.g. if we
         * pre-determined to skip or fail the test).
         */
        if (saTestList[i].testStatus != SA_TEST_NOT_RUN)
            continue;

        saTestInit();
        utilPrepFout(&tFramework);
        
        #ifdef USE_RTOS
#if    SAUTEST_DISABLE_TASK_GATE_SEM
       /* Just call the test function */
       saTestList[i].testFunction((void *)&tFramework, (void *)&saTestList[i]);
#else        
		tparams.arg1 = (void *)&saTestList[i];
        tFramework.pTest = &saTestList[i];
		tparams.name = (const char *)saTestList[i].name;
	    tparams.stack = gAppTskStacksaTestList[i];
	    tparams.stacksize = sizeof (gAppTskStacksaTestList[i]);
		
		thandle = TaskP_create(saTestList[i].testFunction, &tparams);
        
		/* The test task will terminate upon completion. Verify that the
		 * task has completed in case the task itself uses multiple tasks
		 * that all wind up idle for a while. */
		while (TaskP_isTerminated (thandle) != 1)
        {
            /* Do Nothing */
        }
        
        TaskP_delete (&thandle);
#endif        
        #else
        #if defined (BUILD_C7X)
        saTestList[i].testFunction((void *)&tFramework,(void*)&saTestList[i]);
        #else
        /* Just call the test function */
        saTestList[i].testFunction((void *)&tFramework,(void *)&saTestList[i]);
        #endif
        #endif
		
		if (saTestList[i].testStatus == SA_TEST_PASSED)
		  SALog ("%s:  PASSED\n", saTestList[i].name);
		else
		  SALog ("%s:  FAILED\n", saTestList[i].name);
          
        System_flush ();
		
		/* Do a quick check of the test framework */
		if (verifyTestFramework ())  {
			SALog ("topLevelTest (%s:%d): verifyTestFramework returned error after test %s. Exiting.\n", __FILE__, __LINE__, saTestList[i].name);
			exitCode = -1;
			System_flush ();
            #ifdef USE_RTOS
#if defined(NSS_LITE) || defined(NSS_LITE2)
#if         SAUTEST_DISABLE_TASK_GATE_SEM
#else
			OS_stop();
#endif
#else
			ClockP_start(clkh);
#endif
            #endif
		}
        
        utilCloseFout(&tFramework);
	} 

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#ifndef __LINUX_USER_SPACE
  /* Test the core dump after all the tests are complete, since core dump puts the SASS in
     undefined state and needs re-download of firmware etc. When we do the test later for
     DSP only cases, the redownload happens automatically for the next run, not breaking
     any of the unit tests */
     Sa_coreDump(tFramework.salld_handle, coreDumpBuf, SAUTEST_CORE_DUMP_SIZE);

     if (coreDump2File == TRUE) {
       fp = fopen ("SaCoreDump.txt", "w+");

       if (fp == NULL)
       {
         SALog (" Error in writing to SaCoreDump file \n");
       }

       for (i = 0; i < SAUTEST_CORE_DUMP_SIZE; i++)
       {
         fprintf (fp, "0x%08x\n", (unsigned int)coreDumpBuf[i]);
       }
       fclose (fp);
     }
#endif
#endif
	/* Summarize the test results */
	for (i = passCount = failCount = notRunCount = 0; saTestList[i].testFunction != NULL; i++)  {
		if (saTestList[i].testStatus == SA_TEST_PASSED)
			passCount += 1;
		else if (saTestList[i].testStatus == SA_TEST_FAILED)
			failCount += 1;
		else
			notRunCount += 1;
	}
	
	SALog ("\n\nTest summary:\n\tTests Passed: %d\n\tTests Failed: %d\n\tTests not run: %d\n\n",
	  passCount, failCount, notRunCount);
      
    /* Exit the Test framework */
    if (exitTestFramework ())  {
    	SALog ("topLevelTest (%s:%d): exitTestFramework returned error, exiting\n", __FILE__, __LINE__);
    	exitCode = -1;
    }
    else
    {
    	SALog("topLevelTest: exitTestFramework Successful \n");
    }

	SALog ("\n\n ------- SA Unit Test Complete ---------\n");

    if (failCount == 0)
    {
        if (notRunCount != 0)
        {
            SALog ("\nSome tests skipped.\n");
        }
        SALog ("\nAll tests have PASSED.\n");
    }
    else
    {
        SALog ("\nSome tests have FAILED, failCount = %d.\n", failCount);
    }

	System_flush();
    #ifdef USE_RTOS
#if defined(NSS_LITE) || defined(NSS_LITE2)
#if SAUTEST_DISABLE_TASK_GATE_SEM
#else
	OS_stop();
#endif
#else
	ClockP_start(clkh);
#endif
    #endif
	
    #ifdef USE_BIOS
	Task_exit();
    #endif
}




