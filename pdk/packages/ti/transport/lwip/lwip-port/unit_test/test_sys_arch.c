/*
 *  Copyright ( C ) 2022 Texas Instruments Incorporated
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
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * \file  test_sys_arch.c
 *
 * \brief file for testing lwip porting layer
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>

/* lwip includes */
#include <lwip/sys.h>

/* Unity header files */
#include <unity.h>
#include <unity_config.h>

/* OSAL header files */
#include <ti/osal/DebugP.h>
#include <ti/osal/HwiP.h>
#include <OSAL_log.h>

#include <ti/csl/soc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#ifdef SOC_J721E
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           ( CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1 )
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           ( CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_R5FSS0_CORE0_INTR_R5FSS0_INTROUTER0_OUTL_1 )
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           ( CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_0 )
        #define PONG_INT_NUM           ( CSLR_R5FSS0_CORE1_INTR_R5FSS0_INTROUTER0_OUTL_1 )
    #endif
#endif

#ifdef SOC_J7200
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_MSMC_EN_SOC_EVENTS_OUT_LEVEL_1)
    #endif
#endif

#ifdef SOC_J721S2
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
#endif

#ifdef SOC_J784S4
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
#endif

#ifdef SOC_J742S2
    #ifdef BUILD_MCU1_0
        #define PING_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_0)
        #define PONG_INT_NUM           (CSLR_MCU_R5FSS0_CORE0_INTR_MAIN2MCU_LVL_INTRTR0_OUTL_1)
    #endif
    #ifdef BUILD_MCU2_0
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
    #ifdef BUILD_MCU2_1
        #define PING_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_0)
        #define PONG_INT_NUM           (CSLR_R5FSS0_CORE1_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_1)
    #endif
#endif


#define PING_TASK_SIZE              (4U * 1024U)
#define PONG_TASK_SIZE              (4U * 1024U)
#define MAX_MBOX_QUEUE_1_LENGTH     (136)           /* should be <= LWIP_SAFERTOS_MBOX_MAX_MSGCOUNT for safertos*/

#if defined(SAFERTOS)
#define TICK_RATE                   (configTICK_RATE_MS)
#else
#define TICK_RATE                   (portTICK_PERIOD_MS)
#endif

#define PING_TASK_PRI  ( 2u )
#define PONG_TASK_PRI  ( 3u )

#define MAX_DELAY                    0                        // 0 means that we are waiting infinite
#define NUM_TASK_SWITCHES           ( 1000000u )
#define NUM_MBOX_MESSAGE            ( 10 )
#define DELAY_MS                    ( 1000 )                      /* in msecs */
#define MAX_MBOX_QUEUE_2_LENGTH     ( 100 )


/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

sys_sem_t gPingSem, gPongSem, testStartPing, testStopPing, testStartPong, testStopPong;
sys_sem_t *gNetconnPingSem, *gNetconnPongSem;
sys_thread_t gPongtask, gPingtask;
sys_mbox_t mboxQueue, mboxQueue2;
sys_mutex_t gMutex;
volatile int gMutexGlobalVar = 0, gSysarchGlobalVar=0, gTcpIpCoreGlobalVar;
volatile int gQueueCount=0, gRetreiveCount=0, gQueueCount2=0, gRetreiveCount2=0;

uint32_t ping_isr_count = 0;
const char* mboxMessage = "Mbox message to send";
volatile uint32_t gExitTask = 1U;


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

enum sysArchTaskID{
    SYSARCH_UT_TASK_ID_PING = 0,
    SYSARCH_UT_TASK_ID_PONG = 1
};


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void ping_isr(uintptr_t arg)
{
    err_t postStatus;
    const char* mboxTryFetchMessage = "Mbox try post from isr message to send";
    ping_isr_count++;
    postStatus = sys_mbox_trypost_fromisr(&mboxQueue, (void *)mboxTryFetchMessage);
    DebugP_assert(postStatus==ERR_OK);
}

/* switch between ping and pong tasks using semaphores */
void test_taskSwitchWithSemaphore(void)
{
    u32_t count; 				/* loop `count` times */
	volatile uint64_t curTime; 		/* time in units of usecs */
    u32_t semaphoreResult=ERR_OK;

    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("\r\n");
    OSAL_log("test_taskSwitchWithSemaphore\r\n");

    count = NUM_TASK_SWITCHES;
    curTime = sys_now();

    while(count-- && semaphoreResult!=SYS_ARCH_TIMEOUT){
        if(semaphoreResult!=SYS_ARCH_TIMEOUT){
            sys_sem_signal(&gPongSem);                                      /* wake up pong task */
        }
        semaphoreResult = sys_arch_sem_wait(&gPingSem, MAX_DELAY);          /* wait for pong to signal */
        TEST_ASSERT(semaphoreResult!=SYS_ARCH_TIMEOUT);
    }

    curTime = sys_now() - curTime;

    OSAL_log("execution time for task switches = %d ms\r\n", (uint32_t)curTime);
    OSAL_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
    OSAL_log("time per task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime*1000000/(NUM_TASK_SWITCHES*2)));

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}

/* sends the same message count number of times via mailbox */
void test_taskMailbox(void){
    u32_t count;

    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("\r\n");
    OSAL_log("test_taskMailbox\r\n");

    count = NUM_MBOX_MESSAGE;
    while(count--){
        sys_mbox_post(&mboxQueue, (void*)mboxMessage);
    }

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}

void test_taskMailboxTryPost(){
    u32_t count;
    err_t postStatus;
    const char* mboxTryPostFetchMessage = "Mbox try post/fetch message to send";
    count = MAX_MBOX_QUEUE_1_LENGTH+1;                // number of msgs queued in this testcase > max capacity of mailbox
    gQueueCount=0;

    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("\r\n");
    OSAL_log("test for mailbox try post started\r\n");

    while(count --){
        postStatus = sys_mbox_trypost(&mboxQueue, (void *)mboxTryPostFetchMessage);
        if(postStatus==ERR_OK){
            gQueueCount++;
            TEST_ASSERT((gQueueCount-gRetreiveCount)<=MAX_MBOX_QUEUE_1_LENGTH);
        }
        else{
            TEST_ASSERT((gQueueCount-gRetreiveCount)>=MAX_MBOX_QUEUE_1_LENGTH);
            while((gQueueCount-gRetreiveCount)>=MAX_MBOX_QUEUE_1_LENGTH){
                sys_arch_msleep(DELAY_MS);
            }
        }
    }

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}

void test_taskMailboxTryPost2(){
    u32_t count;
    err_t postStatus;
    const char* mboxTryPostFetchMessage = "Mbox try post/fetch message to send";
    count = MAX_MBOX_QUEUE_2_LENGTH+1;                // number of msgs queued in this testcase > max capacity of mailbox
    gQueueCount2=0;

    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("\r\n");
    OSAL_log("test for mailbox try post 2 started\r\n");

    while(count --){
        postStatus = sys_mbox_trypost(&mboxQueue2, (void *)mboxTryPostFetchMessage);
        if(postStatus==ERR_OK){
            gQueueCount2++;
            TEST_ASSERT((gQueueCount2-gRetreiveCount2)<=MAX_MBOX_QUEUE_2_LENGTH);
        }
        else{
            TEST_ASSERT((gQueueCount2-gRetreiveCount2)>=MAX_MBOX_QUEUE_2_LENGTH);
            while((gQueueCount2-gRetreiveCount2)>=MAX_MBOX_QUEUE_2_LENGTH){
                sys_arch_msleep(DELAY_MS);
            }
        }
    }

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}

void test_taskMailboxTryFetch(){
    void *msg;
    u32_t count, result;
    count = MAX_MBOX_QUEUE_1_LENGTH+1;
    gRetreiveCount=0;

    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    while(count --){
        result = sys_arch_mbox_tryfetch(&mboxQueue, &msg);
        if(result!=SYS_MBOX_EMPTY){
            gRetreiveCount++;
            TEST_ASSERT((gQueueCount-gRetreiveCount)>=0);
        }
        else{
            TEST_ASSERT((gQueueCount-gRetreiveCount)<=0);
            while((gQueueCount-gRetreiveCount)<=0){
                sys_arch_msleep(DELAY_MS);
            }
        }
    }
    OSAL_log("test for mailbox try fetch ended\r\n");

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}

void test_taskMailboxTryFetch2(){
    void *msg;
    u32_t count, result;
    count = MAX_MBOX_QUEUE_2_LENGTH+1;
    gRetreiveCount2=0;

    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    while(count --){
        result = sys_arch_mbox_tryfetch(&mboxQueue2, &msg);
        if(result!=SYS_MBOX_EMPTY){
            gRetreiveCount2++;
            TEST_ASSERT((gQueueCount2-gRetreiveCount2)>=0);
        }
        else{
            TEST_ASSERT((gQueueCount2-gRetreiveCount2)<=0);
            while((gQueueCount2-gRetreiveCount2)<=0){
                sys_arch_msleep(DELAY_MS);
            }
        }
    }
    OSAL_log("test for mailbox try fetch 2 ended\r\n");

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}

/* switch from ping to ISR and back to the same task using mailbox, here there is no task switch */
void test_taskToIsrUsingMailboxAndNoTaskSwitch(void)
{
    uint32_t count;             /* loop count times */
    volatile uint64_t curTime;  /* time in units of usecs */
    HwiP_Params hwiParams;
    HwiP_Handle hHwi;
    HwiP_Status hwiStatus;
    u32_t result=ERR_OK;
    void *msg;
    char* mboxTryFetchMessage = "Mbox try post from isr message to send";

    OSAL_log("\r\n");
    OSAL_log("test_taskToIsrUsingMailboxAndNoTaskSwitch\r\n");

    HwiP_Params_init(&hwiParams);
    hHwi = HwiP_create(PING_INT_NUM, ping_isr, &hwiParams);
    TEST_ASSERT(hHwi != NULL);

    count = NUM_TASK_SWITCHES;
    ping_isr_count = 0;
    curTime = sys_now();
     while( count-- && result!=SYS_ARCH_TIMEOUT)
    {
        HwiP_post(PING_INT_NUM);
        result = sys_arch_mbox_fetch(&mboxQueue, &msg, MAX_DELAY); /* wait for ISR to signal */
        TEST_ASSERT(result!=SYS_ARCH_TIMEOUT);
        TEST_ASSERT(strncmp((char*)msg, mboxTryFetchMessage, strlen(mboxTryFetchMessage))==0);
    }

    curTime = sys_now() - curTime;

    hwiStatus = HwiP_delete(hHwi);
    TEST_ASSERT(hwiStatus == HwiP_OK);

    OSAL_log("execution time for task - ISR - task switches = %d ms\r\n", (uint32_t)curTime);
    OSAL_log("number of task switches = %d, num ISRs = %d \r\n", (uint32_t)NUM_TASK_SWITCHES, ping_isr_count);
    OSAL_log("time per task - ISR - task switch = %d ns\r\n", (uint32_t)(curTime * 1000000 / (NUM_TASK_SWITCHES)));
}

void taskMutexNestLevel2(enum sysArchTaskID taskIdentifier){
    int localVar = gMutexGlobalVar;
    if(taskIdentifier==SYSARCH_UT_TASK_ID_PONG){
        TEST_ASSERT(gMutexGlobalVar==0);
        gMutexGlobalVar++;
    }
    else{
        TEST_ASSERT(gMutexGlobalVar==1);
        gMutexGlobalVar--;
    }
    sys_arch_msleep(DELAY_MS);
    if(taskIdentifier==SYSARCH_UT_TASK_ID_PONG){
        TEST_ASSERT(gMutexGlobalVar-localVar==1);
    }
    else{
        TEST_ASSERT(localVar-gMutexGlobalVar==1);
    }
}

void taskMutexNestLevel1(enum sysArchTaskID taskIdentifier){
    sys_mutex_lock(&gMutex);
    taskMutexNestLevel2(taskIdentifier);
    sys_arch_msleep(DELAY_MS);
    sys_mutex_unlock(&gMutex);
}

void test_taskMutexPong(void){
    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    OSAL_log("\r\n");
    OSAL_log("test_taskMutexPong\r\n");
    sys_mutex_lock(&gMutex);
    taskMutexNestLevel1(SYSARCH_UT_TASK_ID_PONG);
    sys_arch_msleep(DELAY_MS);
    sys_mutex_unlock(&gMutex);
    OSAL_log("test_taskMutexPong ended\r\n");

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}

void test_taskMutexPing(void){
    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("test_taskMutexPing\r\n");
    sys_mutex_lock(&gMutex);
    taskMutexNestLevel1(SYSARCH_UT_TASK_ID_PING);
    sys_arch_msleep(DELAY_MS);
    sys_mutex_unlock(&gMutex);
    OSAL_log("test_taskMutexPing ended\r\n");

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}

#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
# if SYS_LIGHTWEIGHT_PROT

void task_sysarch_NestLevel2(enum sysArchTaskID taskIdentifier){
    int localVar = gSysarchGlobalVar;
    if(taskIdentifier==SYSARCH_UT_TASK_ID_PONG){
        TEST_ASSERT(gSysarchGlobalVar==0);
        gSysarchGlobalVar++;
    }
    else{
        TEST_ASSERT(gSysarchGlobalVar==1);
        gSysarchGlobalVar--;
    }
    sys_arch_msleep(DELAY_MS);
    if(taskIdentifier==SYSARCH_UT_TASK_ID_PONG){
        TEST_ASSERT(gSysarchGlobalVar-localVar==1);
    }
    else{
        TEST_ASSERT(localVar-gSysarchGlobalVar==1);
    }
}

void task_sysarch_NestLevel1(enum sysArchTaskID taskIdentifier){
    OSAL_log("task_sysarch_NestLevel1\r\n");
    sys_prot_t pval = sys_arch_protect();
    task_sysarch_NestLevel2(taskIdentifier);
    sys_arch_msleep(DELAY_MS);
    sys_arch_unprotect(pval);
}

void test_sysarch_protect_unprotectPong(){
    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    OSAL_log("\r\n");
    OSAL_log("test_sysarch_protect_unprotectPong started\r\n");
    sys_prot_t pval = sys_arch_protect();
    task_sysarch_NestLevel1(SYSARCH_UT_TASK_ID_PONG);
    sys_arch_msleep(DELAY_MS);
    sys_arch_unprotect(pval);
    OSAL_log("\r\n");
    OSAL_log("test_sysarch_protect_unprotectPong ended\r\n");

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}

void test_sysarch_protect_unprotectPing(){
    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("\r\n");
    OSAL_log("test_sysarch_protect_unprotectPing started\r\n");
    sys_prot_t pval = sys_arch_protect();
    task_sysarch_NestLevel1(SYSARCH_UT_TASK_ID_PING);
    sys_arch_msleep(DELAY_MS);
    sys_arch_unprotect(pval);
    OSAL_log("\r\n");
    OSAL_log("test_sysarch_protect_unprotectPing ended\r\n");

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}
#endif
#endif

#if LWIP_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING

void task_lock_tcpip_core_NestLevel2(enum sysArchTaskID taskIdentifier){
    int localVar = gTcpIpCoreGlobalVar;
    if(taskIdentifier==SYSARCH_UT_TASK_ID_PONG){
        TEST_ASSERT(gTcpIpCoreGlobalVar==0);
        gTcpIpCoreGlobalVar++;
    }
    else{
        TEST_ASSERT(gTcpIpCoreGlobalVar==1);
        gTcpIpCoreGlobalVar--;
    }
    sys_arch_msleep(DELAY_MS);
    if(taskIdentifier==SYSARCH_UT_TASK_ID_PONG){
        TEST_ASSERT(gTcpIpCoreGlobalVar-localVar==1);
    }
    else{
        TEST_ASSERT(localVar-gTcpIpCoreGlobalVar==1);
    }
}

void task_lock_tcpip_core_NestLevel1(enum sysArchTaskID taskIdentifier){
    OSAL_log("task_lock_tcpip_core_NestLevel1\r\n");
    sys_lock_tcpip_core();
    task_lock_tcpip_core_NestLevel2(taskIdentifier);
    sys_arch_msleep(DELAY_MS);
    sys_unlock_tcpip_core();
}

void test_sys_lock_unlock_tcpip_corePong(){
    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    OSAL_log("\r\n");
    OSAL_log("test_sys_lock_unlock_tcpip_corePong started\r\n");
    sys_lock_tcpip_core();
    sys_mark_tcpip_thread();
    sys_check_core_locking();
    task_lock_tcpip_core_NestLevel1(SYSARCH_UT_TASK_ID_PONG);
    sys_arch_msleep(DELAY_MS);
    sys_unlock_tcpip_core();
    OSAL_log("\r\n");
    OSAL_log("test_sys_lock_unlock_tcpip_corePong ended\r\n");

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}

void test_sys_lock_unlock_tcpip_corePing(){
    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("\r\n");
    OSAL_log("test_sys_lock_unlock_tcpip_corePing started\r\n");
    sys_lock_tcpip_core();
    task_lock_tcpip_core_NestLevel1(SYSARCH_UT_TASK_ID_PING);
    sys_arch_msleep(DELAY_MS);
    sys_unlock_tcpip_core();
    OSAL_log("\r\n");
    OSAL_log("test_sys_lock_unlock_tcpip_corePing ended\r\n");

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}
#endif
#endif

#if LWIP_NETCONN_SEM_PER_THREAD
/* switch between ping and pong tasks using netconn semaphores */
void test_taskSwitchWithNetconnSemaphore(void)
{
    u32_t count; 				    /* loop count times */
	volatile uint64_t curTime; 		/* time in units of usecs */

    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);

    OSAL_log("\r\n");
    OSAL_log("test_taskSwitchWithNetconnSemaphore\r\n");

    count = NUM_TASK_SWITCHES;
    curTime = sys_now();
    u32_t semaphoreResult=ERR_OK;

    while(count-- && semaphoreResult!=SYS_ARCH_TIMEOUT){
        if(semaphoreResult!=SYS_ARCH_TIMEOUT){
            sys_sem_signal(gNetconnPongSem);                                      /* wake up pong task */
        }
        semaphoreResult = sys_arch_sem_wait(gNetconnPingSem, MAX_DELAY);          /* wait for pong to signal */
        TEST_ASSERT(semaphoreResult!=SYS_ARCH_TIMEOUT);
    }

    curTime = sys_now() - curTime;

    OSAL_log("execution time for task switches = %d ms\r\n", (uint32_t)curTime);
    OSAL_log("number of task switches = %d \r\n", (uint32_t)NUM_TASK_SWITCHES*2);
    OSAL_log("time per task switch (semaphore give/take) = %d ns\r\n", (uint32_t)(curTime*1000000/(NUM_TASK_SWITCHES*2)));

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}
#endif

/* wait some msecs, this is just to show how DELAY_MS API can be used, 
* there is no need to DELAY_MS before deleting the task 
*/
void test_taskDelay(void)
{
    volatile uint64_t curTime; /* time in units of usecs */
    uint32_t delay1 = 2 * DELAY_MS, delay2 = 3 * DELAY_MS; /* in msecs */

    OSAL_log( "\r\n" );
	OSAL_log( "test_taskDelay started !!!\r\n" );

    curTime = sys_now();
    sys_arch_msleep(delay1);
    sys_arch_msleep(delay2);
    curTime = sys_now() - curTime;
    TEST_ASSERT_UINT32_WITHIN(TICK_RATE, (delay1 + delay2), (uint32_t)curTime);

    curTime = sys_jiffies();
    sys_arch_msleep(delay1);
    sys_arch_msleep(delay2);
    curTime = sys_jiffies() - curTime;
    TEST_ASSERT(curTime==delay1+delay2);

    OSAL_log( "\r\n" );
	OSAL_log( "test_taskDelay ended !!!\r\n" );
}

#if LWIP_NETCONN_SEM_PER_THREAD
/*Creating netconn semaphore (TLS object) of current executing task */
void createNetConnPingSemaphore(){
    sys_arch_sem_wait(&testStartPing, MAX_DELAY);
    sys_sem_signal(&testStartPong);
    
    sys_arch_netconn_sem_alloc();
    gNetconnPingSem = sys_arch_netconn_sem_get();
    DebugP_assert(gNetconnPingSem!=NULL);

    sys_arch_sem_wait(&testStopPing, MAX_DELAY);
    sys_sem_signal(&testStopPong);
}

/*Creating netconn semaphore (TLS object) of current executing task */
void createNetConnPongSemaphore(){
    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);
    
    sys_arch_netconn_sem_alloc();
    gNetconnPongSem = sys_arch_netconn_sem_get();
    DebugP_assert(gNetconnPongSem!=NULL);

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}
#endif

void setUp(void)
{
}

void tearDown(void)
{
}

#if LWIP_NETCONN_SEM_PER_THREAD
void taskSwitchWithNetconnSemaphorePong(){
    u32_t count;
    u32_t result=ERR_OK;
    
    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    count = NUM_TASK_SWITCHES;
    while(count-- && result!=SYS_ARCH_TIMEOUT)
    {
        result = sys_arch_sem_wait(gNetconnPongSem, MAX_DELAY);                /* wait for ping to signal */
        TEST_ASSERT(result!=SYS_ARCH_TIMEOUT);
        sys_sem_signal(gNetconnPingSem);                                       /* wakeup ping task */
    }

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}
#endif

void taskSwitchWithSemaphorePong(){
    u32_t count;
    u32_t result=ERR_OK;

    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    count = NUM_TASK_SWITCHES;
    while(count-- && result!=SYS_ARCH_TIMEOUT)
    {
        result = sys_arch_sem_wait(&gPongSem, MAX_DELAY);                /* wait for ping to signal */
        TEST_ASSERT(result!=SYS_ARCH_TIMEOUT);
        sys_sem_signal(&gPingSem);                                       /* wakeup ping task */
    }

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}

void taskMailboxPong(){
    u32_t count;
    u32_t result=ERR_OK;
    void *msg;

    sys_sem_signal(&testStartPing);
    sys_arch_sem_wait(&testStartPong, MAX_DELAY);

    count = NUM_MBOX_MESSAGE;
    while(count-- && result!=SYS_ARCH_TIMEOUT){
        result = sys_arch_mbox_fetch(&mboxQueue, &msg, MAX_DELAY);
        TEST_ASSERT(result!=SYS_ARCH_TIMEOUT);
        TEST_ASSERT(strncmp((char*)msg, mboxMessage, strlen(mboxMessage))==0);
    }

    OSAL_log( "\r\n" );
	OSAL_log( "test_taskMailbox ended !!!\r\n" );

    sys_sem_signal(&testStopPing);
    sys_arch_sem_wait(&testStopPong, MAX_DELAY);
}

void pong_main(void *args){
#if LWIP_NETCONN_SEM_PER_THREAD
    /*Creating netconn semaphore (TLS object) of current executing task */
    createNetConnPongSemaphore();
#endif

#if LWIP_NETCONN_SEM_PER_THREAD
    taskSwitchWithNetconnSemaphorePong();
#endif

    taskSwitchWithSemaphorePong();

    taskMailboxPong();

    test_taskMailboxTryFetch();

    test_taskMailboxTryFetch2();

#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
# if SYS_LIGHTWEIGHT_PROT
    test_sysarch_protect_unprotectPong();
#endif
#endif

#if LWIP_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING
    test_sys_lock_unlock_tcpip_corePong();
#endif
#endif

    test_taskMutexPong();

    sys_sem_free(&gPongSem);

/* Deleting netconn semaphore (TLS object) of current executing task */
#if LWIP_NETCONN_SEM_PER_THREAD
    sys_arch_netconn_sem_free();
#endif

/* If null is passed in here then we are deleting ourselves. */
#if defined(SAFERTOS)
    (void)xTaskDelete(NULL);
#else
    (void)vTaskDelete(NULL);
#endif    
}

void ping_main(void *args){

    UNITY_BEGIN();
#if LWIP_NETCONN_SEM_PER_THREAD
/*Creating netconn semaphore (TLS object) of current executing task */
    RUN_TEST(createNetConnPingSemaphore, 1, NULL);
#endif
#if LWIP_NETCONN_SEM_PER_THREAD
    RUN_TEST(test_taskSwitchWithNetconnSemaphore, 2, NULL);
#endif
    RUN_TEST(test_taskSwitchWithSemaphore, 3, NULL);
    RUN_TEST(test_taskMailbox, 4, NULL);
    RUN_TEST(test_taskMailboxTryPost, 5, NULL);
    RUN_TEST(test_taskToIsrUsingMailboxAndNoTaskSwitch, 6, NULL);
    RUN_TEST(test_taskMailboxTryPost2, 7, NULL);
#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
# if SYS_LIGHTWEIGHT_PROT
    RUN_TEST(test_sysarch_protect_unprotectPing, 8, NULL);
#endif
#endif
#if LWIP_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING
    RUN_TEST(test_sys_lock_unlock_tcpip_corePing, 9, NULL);
#endif
#endif
    RUN_TEST(test_taskMutexPing, 10, NULL);
    RUN_TEST(test_taskDelay, 11, NULL);

    sys_mbox_free(&mboxQueue);
    sys_mbox_free(&mboxQueue2);
    sys_mutex_free(&gMutex);
    sys_sem_free(&gPingSem);

/* Deleting netconn semaphore (TLS object) of current executing task */
#if LWIP_NETCONN_SEM_PER_THREAD
    sys_arch_netconn_sem_free();
#endif

    UNITY_END();

    if (Unity.TestFailures == 0)
    {
        OSAL_log("\r\n");
        OSAL_log("All tests have passed!!\r\n");
    }

/* If null is passed in here then we are deleting ourselves. */
#if defined(SAFERTOS)
    (void)xTaskDelete(NULL);
#else
    (void)vTaskDelete(NULL);
#endif  
}

void sys_arch_test_main( void *args){
    err_t pingStatus, pongStatus, mboxStatus, mutexStatus, startPingStatus, stopPingStatus, startPongStatus, stopPongStatus;

    /* Create the semaphore used by the first two tasks. */
    pingStatus = sys_sem_new(&gPingSem,0);
    TEST_ASSERT(pingStatus==ERR_OK);
    pongStatus = sys_sem_new(&gPongSem,0);
    TEST_ASSERT(pongStatus==ERR_OK);
    startPingStatus = sys_sem_new(&testStartPing,0);
    TEST_ASSERT(startPingStatus==ERR_OK);
    stopPingStatus = sys_sem_new(&testStopPing,0);
    TEST_ASSERT(stopPingStatus==ERR_OK);
    startPongStatus = sys_sem_new(&testStartPong,0);
    TEST_ASSERT(startPongStatus==ERR_OK);
    stopPongStatus = sys_sem_new(&testStopPong,0);
    TEST_ASSERT(stopPongStatus==ERR_OK);

    /* Create queue, length is MAX_MBOX_QUEUE_1_LENGTH*/
    mboxStatus = sys_mbox_new(&mboxQueue, MAX_MBOX_QUEUE_1_LENGTH);
    TEST_ASSERT(mboxStatus==ERR_OK);
    /* Create queue, length is MAX_MBOX_QUEUE_2_LENGTH*/
    mboxStatus = sys_mbox_new(&mboxQueue2, MAX_MBOX_QUEUE_2_LENGTH);
    TEST_ASSERT(mboxStatus==ERR_OK);

    /* Create mutex */
    mutexStatus = sys_mutex_new(&gMutex);
    TEST_ASSERT(mutexStatus==ERR_OK);

    /* then create the tasks, order of task creation does matter for this example */
    /* Parameters
        - Text name for the task.  This is to facilitate debugging only
        - Pointer to the function that implements the task
        - Task parameters, we are not using the task parameters
        - Stack depth in units of StackType_t typically uint32_t on 32b CPUs
        - task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest
    */
    gPongtask = sys_thread_new((const char *)"pong", &pong_main, NULL, PONG_TASK_SIZE, PONG_TASK_PRI);
    TEST_ASSERT(gPongtask.thread_handle!=NULL);
    gPingtask = sys_thread_new((const char *)"ping", &ping_main, NULL, PING_TASK_SIZE, PING_TASK_PRI);
    TEST_ASSERT(gPongtask.thread_handle!=NULL);

    while(gExitTask){
        sys_arch_msleep(DELAY_MS);
    }
}