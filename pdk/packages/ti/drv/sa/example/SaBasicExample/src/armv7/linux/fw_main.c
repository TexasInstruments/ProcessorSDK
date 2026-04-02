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
 * (C) Copyright 2012, Texas Instruments, Inc.
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "fw_mem_allocator.h"
#include "unittest.h"


void *saESPTest_linux (void *arg)
{
    saESPTest(arg);
    return (void *)0;
}

void *saAHTest_linux (void *arg)
{
    saAHTest(arg);
    return (void *)0;
}

void *saSRTPTest_linux (void *arg)
{
    saSRTPTest(arg);
    return (void *)0;
}

void *saSrtpEspTest_linux (void *arg)
{
    saSrtpEspTest(arg);
    return (void *)0;
}

void *saSrtpAhTest_linux (void *arg)
{
    saSrtpAhTest(arg);
    return (void *)0;
}

void *saLTETest_linux (void *arg)
{
    saLTETest(arg);
    return (void *)0;
}

void *saDataModeTest_linux (void *arg)
{
    saDataModeTest(arg);
    return (void *)0;
}
/* NULL terminated The list of tests */
saTest_t  saTestList[] = {
	{ saESPTest_linux,          "SA IPSEC ESP test",                   SA_TEST_NOT_RUN },
    { saAHTest_linux,           "SA IPSEC AH test",                    SA_TEST_NOT_RUN },
    { saSRTPTest_linux,         "SA SRTP test",                        SA_TEST_NOT_RUN },
    { saSrtpEspTest_linux,      "SA SRTP over ESP test",               SA_TEST_NOT_RUN },
    { saSrtpAhTest_linux,       "SA SRTP over AH test",                SA_TEST_NOT_RUN },
    #ifdef SA_3GPP_SUPPORT
	{ saLTETest_linux,          "SA 3GPP LTE test",                    SA_TEST_NOT_RUN },
    #endif
	{ saDataModeTest_linux,     "SA Data Mode test",                   SA_TEST_NOT_RUN },
	{ NULL,                     NULL,                                  SA_TEST_NOT_RUN }
};

paSysStats_t    saTestExpectedStats;  /* Expected stats results */
t2Handles_t     l2Handles[10];
t2Handles_t     l3Handles[10];
t4Handles_t     l4Handles[10];

/*
 * Enable/Disable Null Encryption:
 *
 * TRUE: Null encryption and authentication with fixed hash data 0x10, 0x11, 0x12 ...
 * FALSE: Real encryption and authentication
 *
 */
Bool saNullEnc = FALSE;

/* SA REG base address */
uint32_t saRegBaseAddr;

/* Global variables to hold virtual address of various subsystems */
void *fw_qmssCfgVaddr;
void *fw_qmssDataVaddr;
void *fw_srioCfgVaddr;
void *fw_passCfgVaddr;
void *fw_saCfgVaddr;

void  *topLevelTest (void *args);

typedef pthread_t task_handle;

#define DEFAULT_STACK_SIZE  0x8000
/** ============================================================================
 *   @n@b task_create
 *
 *   @b Description
 *   @n Create thread to run the test program
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
static int task_create ( void *(start_routine)(void*), void* args, void* handle)
{
    int max_priority, err;
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;

    max_priority = sched_get_priority_max(SCHED_FIFO);
    err = pthread_attr_init(&attr);
    if (err) {
        printf("pthread_attr_init failed: (%s)\n", strerror(err));
        return err;
    }
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    if (err) {
        printf("pthread_attr_setdetachstate failed: (%s)\n", strerror(err));
        return err;
    }
    err = pthread_attr_setstacksize(&attr, DEFAULT_STACK_SIZE);
    if (err) {
        printf("pthread_attr_setstacksize failed: (%s)\n", strerror(err));
        return err;
    }
    err = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (err) {
        printf("pthread_attr_setinheritsched failed: (%s)\n", strerror(err));
        return err;
    }
    err = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (err) {
        printf("pthread_attr_setschedpolicy failed: (%s)\n", strerror(err));
        return err;
    }
    memset(&param, 0, sizeof(param));
    param.sched_priority = max_priority;
    err = pthread_attr_setschedparam(&attr, &param);
    if (err) {
        printf("pthread_attr_setschedparam failed: (%s)\n", strerror(err));
        return err;
    }
    if (err) return err;
    err = pthread_create(&thread, &attr, start_routine, args);
    if (err) {
        printf("pthread_create failed: (%s)\n", strerror(err));
        return err;
    }
    if (err) return err;
    *(pthread_t*)handle = thread;
    return 0;
}

/** ============================================================================
 *   @n@b task_wait
 *
 *   @b Description
 *   @n Wait for Task completion
 *
 *   @return    void
 * =============================================================================
 */
static void task_wait (void *handle)
{
    pthread_join(*((pthread_t*)handle), NULL);
    return;
}
/** ============================================================================
 *   @n@b task_sleep
 *
 *   @b Description
 *   @n Sleep the thread for msec duration
 *
 *   @return    void
 * =============================================================================
 */

static void task_sleep(int time_in_msec)
{
    pthread_mutex_t fake_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t fake_cond = PTHREAD_COND_INITIALIZER;
    struct timespec ts;
    int rt;
    unsigned int sec, nsec;

    sec = time_in_msec/1000;
    nsec = (time_in_msec - (sec*1000)) * 1000000;

    /* Use the wall-clock time */
    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += sec;
    ts.tv_nsec += nsec;

    pthread_mutex_lock(&fake_mutex);
    rt = pthread_cond_timedwait(&fake_cond, &fake_mutex, &ts);
    pthread_mutex_unlock(&fake_mutex);
}


/** ============================================================================
 *   @n@b main
 *
 *   @b Description
 *   @n test application main
 *
 *   @return    int
 * =============================================================================
 */
int main() {
    task_handle test_th;
    int status;

    fw_osalInit();

    if (fw_memAllocInit((uint8_t*)MSMC_SRAM_BASE_ADDR,
                                  MSMC_TEST_PERM_MEM_SZ) == fw_FALSE) {
        printf("ERROR: \"Top Level Test\" fw_memAllocInit failed\n");
        return (-1);
    }


    /* Create virtual memory maps */
    /* QMSS CFG Regs */
    fw_qmssCfgVaddr = fw_memMap((void*)QMSS_CFG_BASE_ADDR,
                                            QMSS_CFG_BLK_SZ);
    if (!fw_qmssCfgVaddr)
    {
        printf("ERROR: Failed to map QMSS CFG registers\n");
        return (-1);
    }
#ifdef EXT_DEBUG
    printf("main:QMSS_CFG_BASE_ADDR:0x%x Memory mapped at address %p.\n",(void*)QMSS_CFG_BASE_ADDR, fw_qmssCfgVaddr);
#endif

    /* QMSS DATA Regs */
    fw_qmssDataVaddr = fw_memMap((void*)QMSS_DATA_BASE_ADDR,
                                            QMSS_DATA_BLK_SZ);
    if (!fw_qmssDataVaddr)
    {
        printf("ERROR: Failed to map QMSS DATA registers\n");
        return (-1);
    }
#ifdef EXT_DEBUG
    printf("main:QMSS_DATA_BASE_ADDR:0x%x Memory mapped at address %p.\n",(void*)QMSS_DATA_BASE_ADDR, fw_qmssDataVaddr);
#endif

#if defined(DEVICE_K2K) || defined(DEVICE_K2H)  || defined(SOC_K2K)  || defined(SOC_K2H)
    /* SRIO CFG Regs */
    fw_srioCfgVaddr = fw_memMap((void*)SRIO_CFG_BASE_ADDR,
                                            SRIO_CFG_BLK_SZ);
    if (!fw_srioCfgVaddr)
    {
        printf("ERROR: Failed to map SRIO CFG registers\n");
        return (-1);
    }
#ifdef EXT_DEBUG
    printf("main:SRIO_CFG_BASE_ADDR:0x%x Memory mapped at address %p.\n",(void*)SRIO_CFG_BASE_ADDR, fw_srioCfgVaddr);
#endif
#endif

    /* PASS CFG Regs */
    fw_passCfgVaddr = fw_memMap((void*)PASS_CFG_BASE_ADDR,
                                            PASS_CFG_BLK_SZ);
    if (!fw_passCfgVaddr)
    {
        printf("ERROR: Failed to map PASS CFG registers\n");
        return (-1);
    }
#ifdef EXT_DEBUG
    printf("main:PASS_CFG_BASE_ADDR:0x%x Memory mapped at address %p.\n",(void*)PASS_CFG_BASE_ADDR, fw_passCfgVaddr);
#endif

    /* SA CFG Regs */
    fw_saCfgVaddr = fw_memMap((void*)SA_CFG_BASE_ADDR, SA_CFG_BLK_SZ);

    if (!fw_saCfgVaddr)
    {
        printf("ERROR: Failed to map SA CFG registers\n");
        return (-1);
    }
#ifdef EXT_DEBUG
    printf("main:SA_CFG_BASE_ADDR:0x%x Memory mapped at address %p.\n",(void*)SA_CFG_BASE_ADDR, fw_saCfgVaddr);
#endif
    saRegBaseAddr = (uint32_t)fw_saCfgVaddr;

    if (status = task_create(topLevelTest, NULL, &test_th)) {
        printf("ERROR: \"Top Level Test\" task-create failed (%d)\n", status);
        return (-1);
    }

    task_wait(&test_th);
    fw_osalshutdown();
    return 0;
}

/** ============================================================================
 *   @n@b topLevelTest
 *
 *   @b Description
 *   @n Routine running as a separate thread
 *
 *   @return    int
 * =============================================================================
 */
void *topLevelTest (void *args)
{
    task_handle fw_test_th;
    int status;
    saTestArgs_t testArgs;
	int passCount;
	int failCount;
	int notRunCount;
    int i;

    tFramework.fNullEnc = saNullEnc;
    testArgs.tf = &tFramework;

    printf ("\n\n ------- SA Basic Example Starting ---------\n");

    /* Initialize the PA, PA cpdma, QM and CPPI. Each test will use
     * the same framework */
    if (setupTestFramework ())  {
    	printf ("topLevelTest (%s:%d): setupTestFramework returned error, exiting\n", __FILE__, __LINE__);
        return;
    }

    /* Make sure the setup matches what is expected */
    if (verifyTestFramework())  {
    	printf ("topLevelTest (%s:%d): verifyTestFramework returned error after initial framework setup, exiting\n", __FILE__, __LINE__);
        return;
    }

	/* Run the tests */
	for (i = 0; saTestList[i].testFunction != NULL; i++ )  {

        testArgs.pat = &saTestList[i];

        if (status = task_create(saTestList[i].testFunction, (void *)&testArgs, &fw_test_th)) {
            printf("ERROR: fw_main task-create failed (%d)\n", status);
            return;
        }

		/* The test task will terminate upon completion. */
        task_wait(&fw_test_th);

		if (saTestList[i].testStatus == SA_TEST_PASSED)
		  printf ("%s:  PASSED\n", saTestList[i].name);
		else
		  printf ("%s:  FAILED\n", saTestList[i].name);

		/* Do a quick check of the test framework */
		if (verifyTestFramework ())  {
			printf ("topLevelTest (%s:%d): verifyTestFramework returned error after test %s. Exiting.\n", __FILE__, __LINE__, saTestList[i].name);
            return;
        }
    }

    /* Exit the Test framework */
    if (exitTestFramework ())  {
    	System_printf ("topLevelTest (%s:%d): exitTestFramework returned error, exiting\n", __FILE__, __LINE__);
    	System_flush ();
    	Task_exit ();
    }
    else
    {
    	System_printf("topLevelTest: exitTestFramework Successful \n");
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

	printf ("\n\nTest summary:\n\tTests Passed: %d\n\tTests Failed: %d\n\tTests not run: %d\n\n",
	  passCount, failCount, notRunCount);

	printf ("\n\n ------- SA Basic Example Complete ---------\n");

    pthread_exit((void*) 0);
    return;
}


