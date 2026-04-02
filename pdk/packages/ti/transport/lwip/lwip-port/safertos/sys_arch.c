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
 * \file  sys_arch.c
 *
 * \brief Main file for lwIP porting.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>

/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "lwip/tcpip.h"

/* OSAL header files */
#include <ti/osal/QueueP.h>
#include <ti/osal/src/safertos/SafeRTOS_priv.h>

#include "SafeRTOS_API.h"
#include <sys/cdefs.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** Set this to 1 if you want the stack size passed to sys_thread_new() to be
 *  interpreted as number of stack words (SafeRTOS-like).
 *  Default is that they are interpreted as byte count (lwIP-like). */
#ifndef LWIP_SAFERTOS_THREAD_STACKSIZE_IS_STACKWORDS
#define LWIP_SAFERTOS_THREAD_STACKSIZE_IS_STACKWORDS  0
#endif

/** Set this to 1 to use a mutex for SYS_ARCH_PROTECT() critical regions.
 * Default is 0 and locks interrupts/scheduler for SYS_ARCH_PROTECT(). */
#ifndef LWIP_SYS_ARCH_PROTECT_USES_MUTEX
#define LWIP_SYS_ARCH_PROTECT_USES_MUTEX              0
#endif

/** Set this to 1 to include a sanity check that SYS_ARCH_PROTECT() and
 *  SYS_ARCH_UNPROTECT() are called matching. */
#ifndef LWIP_SAFERTOS_SYS_ARCH_PROTECT_SANITY_CHECK
#define LWIP_SAFERTOS_SYS_ARCH_PROTECT_SANITY_CHECK   0
#endif

/** Set this to 1 to let sys_mbox_free check that queues are empty when freed */
#ifndef LWIP_SAFERTOS_CHECK_QUEUE_EMPTY_ON_FREE
#define LWIP_SAFERTOS_CHECK_QUEUE_EMPTY_ON_FREE       0
#endif

/** Set this to 1 to enable core locking check functions in this port.
 *  For this to work, you'll have to define LWIP_ASSERT_CORE_LOCKED()
 *  and LWIP_MARK_TCPIP_THREAD() correctly in your lwipopts.h! */
#ifndef LWIP_CHECK_CORE_LOCKING
#define LWIP_CHECK_CORE_LOCKING                       0
#endif

/** Set this to 0 to implement sys_now() yourself, e.g. using a hw timer.
 *  Default is 1, where SafeRTOS ticks are used to calculate back to ms. */
#ifndef LWIP_SAFERTOS_SYS_NOW_FROM_SAFERTOS
#define LWIP_SAFERTOS_SYS_NOW_FROM_SAFERTOS           1
#endif

#define LWIP_TASK_PRIORITY_HIGHEST                    ( configMAX_PRIORITIES - 1 )
#define LWIP_TASK_PRIORITY_LOWEST                     ( 0u )

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if SYS_LIGHTWEIGHT_PROT && LWIP_SYS_ARCH_PROTECT_USES_MUTEX
static uint64_t sys_arch_protect_mutex_buffer[((safertosapiQUEUE_OVERHEAD_BYTES/sizeof(uint64_t)) + 1)] = {0};
static xMutexHandleType sys_arch_protect_mutex = NULL;
#endif
#if SYS_LIGHTWEIGHT_PROT && LWIP_SAFERTOS_SYS_ARCH_PROTECT_SANITY_CHECK
static sys_prot_t sys_arch_protect_nesting = 0;
#endif

static struct sys_thread_state sys_thread_state_obj[LWIP_SAFERTOS_MAX_TASK_COUNT];
static QueueP_Handle sys_thread_state_obj_queue = NULL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* Initialize this module (see description in sys.h) */
void sys_init(void)
{
  QueueP_Params queueParams;
  int i;
#if SYS_LIGHTWEIGHT_PROT && LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  /* initialize sys_arch_protect global mutex */
  /* Create a mutex. */
  portBaseType mutexResult = xMutexCreate((portInt8Type *)&sys_arch_protect_mutex_buffer[0], &sys_arch_protect_mutex);
  LWIP_ASSERT("failed to create sys_arch_protect mutex", mutexResult == pdPASS);
#endif /* SYS_LIGHTWEIGHT_PROT && LWIP_SYS_ARCH_PROTECT_USES_MUTEX */

  QueueP_Params_init(&queueParams);
  sys_thread_state_obj_queue = QueueP_create(&queueParams);
  LWIP_ASSERT("failed to create task queue", sys_thread_state_obj_queue != NULL);
  for (i = 0; i < LWIP_SAFERTOS_MAX_TASK_COUNT; i++)
  {
    QueueP_Status status = QueueP_put(sys_thread_state_obj_queue,&sys_thread_state_obj[i].queueElement);
    LWIP_ASSERT("task queue insert failed", status == QueueP_OK);
  }
}

#if configUSE_16_BIT_TICKS == 1
#error This port requires 32 bit ticks or timer overflow will fail
#endif

#if LWIP_SAFERTOS_SYS_NOW_FROM_SAFERTOS
u32_t sys_now(void)
{
  LWIP_ASSERT("error, sys_ now called in ISR", Osal_isInISRContext() == false);
  return xTaskGetTickCount() * configTICK_RATE_MS;
}
#endif

u32_t sys_jiffies(void)
{
  LWIP_ASSERT("error, sys_jiffies called in ISR", Osal_isInISRContext() == false);
  return xTaskGetTickCount();
}

#if SYS_LIGHTWEIGHT_PROT
sys_prot_t sys_arch_protect(void)
{
#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  portBaseType ret;

  LWIP_ASSERT("sys_arch_protect_mutex != NULL", sys_arch_protect_mutex != NULL);
  LWIP_ASSERT("error, mutex is in ISR", Osal_isInISRContext() == false);

  ret = xMutexTake(sys_arch_protect_mutex, safertosapiMAX_DELAY);
  LWIP_ASSERT("sys_arch_protect failed to take the mutex", ret == pdPASS);
#else /* LWIP_SYS_ARCH_PROTECT_USES_MUTEX */
  safertosapiENTER_CRITICAL();
#endif /* LWIP_SYS_ARCH_PROTECT_USES_MUTEX */

#if LWIP_SAFERTOS_SYS_ARCH_PROTECT_SANITY_CHECK
  {
    /* every nested call to sys_arch_protect() returns an increased number */
    sys_prot_t ret = sys_arch_protect_nesting;
    sys_arch_protect_nesting++;
    LWIP_ASSERT("sys_arch_protect overflow", sys_arch_protect_nesting > ret);
    return ret;
  }
#else
  return 1;
#endif
}

void sys_arch_unprotect(sys_prot_t pval)
{
#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  portBaseType ret;
#endif

#if LWIP_SAFERTOS_SYS_ARCH_PROTECT_SANITY_CHECK
  LWIP_ASSERT("unexpected sys_arch_protect_nesting", sys_arch_protect_nesting > 0);
  sys_arch_protect_nesting--;
  LWIP_ASSERT("unexpected sys_arch_protect_nesting", sys_arch_protect_nesting == pval);
#endif

#if LWIP_SYS_ARCH_PROTECT_USES_MUTEX
  LWIP_ASSERT("sys_arch_protect_mutex != NULL", sys_arch_protect_mutex != NULL);
  LWIP_ASSERT("error, mutex is in ISR", Osal_isInISRContext() == false);
  ret = xMutexGive(sys_arch_protect_mutex);
  LWIP_ASSERT("sys_arch_unprotect failed to give the mutex", ret == pdPASS);
#else /* LWIP_SYS_ARCH_PROTECT_USES_MUTEX */
  safertosapiEXIT_CRITICAL();
#endif /* LWIP_SYS_ARCH_PROTECT_USES_MUTEX */

  LWIP_UNUSED_ARG(pval);
}
#endif /* SYS_LIGHTWEIGHT_PROT */

void sys_arch_msleep(u32_t delay_ms)
{
  portTickType delay_ticks = delay_ms / configTICK_RATE_MS;
  xTaskDelay(delay_ticks);
}

#if !LWIP_COMPAT_MUTEX

/* Create a new mutex*/
err_t sys_mutex_new(sys_mutex_t *mutex)
{
  err_t errStatus = ERR_OK;
  portBaseType mutexResult;

  LWIP_ASSERT("mutex is NULL", mutex != NULL);

  mutexResult = xMutexCreate((portInt8Type *)&(mutex->mutObj[0]), &mutex->mutHndl);
  if (mutexResult != pdPASS)
  {
    /* falied to create mutex */
    SYS_STATS_INC(mutex.err);
    errStatus = ERR_MEM;
  }
  else
  {
    SYS_STATS_INC_USED(mutex);
  }

  return errStatus;
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
  portBaseType ret;

  LWIP_ASSERT("mutex is NULL", mutex != NULL);
  /* Should not be called from ISR */
  LWIP_ASSERT("error, mutex is in ISR", Osal_isInISRContext() == false);

  ret = xMutexTake(mutex->mutHndl, safertosapiMAX_DELAY);
  LWIP_ASSERT("failed to take the mutex", ret == pdPASS);
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
  portBaseType ret;

  LWIP_ASSERT("mutex is NULL", mutex != NULL);
  /* Should not be called from ISR */
  LWIP_ASSERT("error, mutex is in ISR", Osal_isInISRContext() == false);

  ret = xMutexGive(mutex->mutHndl);
  LWIP_ASSERT("failed to give the mutex", ret == pdPASS);
}

void sys_mutex_free(sys_mutex_t *mutex)
{
  LWIP_ASSERT("mutex is NULL", mutex != NULL);
  LWIP_ASSERT("mutex->mutHndl is NULL", mutex->mutHndl != NULL);

  /* NOTE: Mutex delete is not supported in safertos, we just memset the memory to zero */
  memset(&mutex->mutObj, 0, sizeof(mutex->mutObj));
  SYS_STATS_DEC(mutex.used);
  mutex->mutHndl = NULL;
}

#endif /* !LWIP_COMPAT_MUTEX */

err_t sys_sem_new(sys_sem_t *sem, u8_t initial_count)
{
  err_t errStatus = ERR_OK;
  portBaseType semResult;

  LWIP_ASSERT("sem is NULL", sem != NULL);

  LWIP_ASSERT("initial_count invalid (not 0 or 1)", (initial_count == 0) || (initial_count == 1));
  LWIP_ASSERT("error, mutex is in ISR", Osal_isInISRContext() == false);

  semResult = xSemaphoreCreateBinary((portInt8Type *)&(sem->semObj[0]), &sem->semHndl);
  if (semResult != pdPASS)
  {
    /* falied to create mutex */
    SYS_STATS_INC(sem.err);
    errStatus = ERR_MEM;
  }
  else
  {
    SYS_STATS_INC_USED(sem);
  }

  if ((errStatus == ERR_OK) && (initial_count == 0))
  {
      uint32_t isSemTaken;

      LWIP_ASSERT("Could not create a semaphore with initial_count value 0 while in ISR", Osal_isInISRContext() == false);

      /* Binary semaphore creation in SafeRTOS initializes semaphore with count of 1.
       * If requested initial count is 0, then we need to take semaphore to honor
       * the requested initial count. */
      isSemTaken = xSemaphoreTake(sem->semHndl, safertosapiMAX_DELAY);
      LWIP_ASSERT("sys_sem_new: initial take failed", isSemTaken == true);
  }

  return errStatus;
}

void sys_sem_signal(sys_sem_t *sem)
{
  portBaseType ret;

  LWIP_ASSERT("sem is NULL", sem != NULL);
  LWIP_ASSERT("error, semaphore is in ISR", Osal_isInISRContext() == false);

  ret = xSemaphoreGive(sem->semHndl);
  /* semaphore already given is OK, this is a signal only... */
  LWIP_ASSERT("sys_sem_signal: sane return value", (ret == pdPASS) || (ret == errSEMAPHORE_ALREADY_GIVEN));
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout_ms)
{
  portBaseType ret;
  u32_t retVal = ERR_OK;

  LWIP_ASSERT("sem != NULL", sem != NULL);
  LWIP_ASSERT("error, semaphore is in ISR", Osal_isInISRContext() == false);

  if (!timeout_ms)
  {
    /* wait infinite */
    ret = xSemaphoreTake(sem->semHndl, safertosapiMAX_DELAY);
    LWIP_ASSERT("taking semaphore failed", ret == pdPASS);
  } 
  else
  {
    portTickType timeout_ticks = timeout_ms / configTICK_RATE_MS;

    ret = xSemaphoreTake(sem->semHndl, timeout_ticks);
    if (ret != pdPASS)
    {
      /* timed out */
      retVal = SYS_ARCH_TIMEOUT;
    }
  }

  return retVal;
}

void sys_sem_free(sys_sem_t *sem)
{
  LWIP_ASSERT("sem is NULL", sem != NULL);
  LWIP_ASSERT("sem->semHndl is NULL", sem->semHndl != NULL);

  /* NOTE: there is no delete Semaphore API in safertos, we just memset the memory to zero */
  memset(&sem->semObj, 0, sizeof(sem->semObj));
  SYS_STATS_DEC(sem.used);
  sem->semHndl = NULL;
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
  portBaseType xQueueStatus;
  portUnsignedBaseType uxBufferLength;
  err_t errStatus = ERR_OK;

  LWIP_ASSERT("error, empty mail box pointer passed", (mbox != NULL));
  LWIP_ASSERT("error, queue size is very large", (size <= LWIP_SAFERTOS_MBOX_MAX_MSGCOUNT));

  /* Note: Here buffer size should have addition portQUEUE_OVERHEAD_BYTES */
  uxBufferLength = size * sizeof(void *) + safertosapiQUEUE_OVERHEAD_BYTES;
  LWIP_ASSERT("error, queue size is very large",(uxBufferLength <= sizeof(mbox->queueBuf)));

  xQueueStatus = xQueueCreate((portInt8Type *)&(mbox->queueBuf[0]), uxBufferLength, size, sizeof(void *), &mbox->mailboxHndl);
  if (xQueueStatus != pdPASS)
  {
    /* mailbox creating failed */
    SYS_STATS_INC(mbox.err);
    errStatus = ERR_MEM;
  }
  else
  {
    SYS_STATS_INC_USED(mbox);
  }

  return errStatus;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
  portBaseType xQueueStatus;

  LWIP_ASSERT("mbox is NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mailboxHndl is NULL", mbox->mailboxHndl != NULL);
  LWIP_ASSERT("error, mailbox is in ISR", Osal_isInISRContext() == false);

  xQueueStatus = xQueueSend(mbox->mailboxHndl, &msg, safertosapiMAX_DELAY);
  LWIP_ASSERT("mailbox post failed",xQueueStatus == pdPASS);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
  portBaseType xQueueStatus;
  err_t retVal;

  LWIP_ASSERT("mbox is NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mailboxHndl is NULL", mbox->mailboxHndl != NULL);
  LWIP_ASSERT("error, mailbox is in ISR", Osal_isInISRContext() == false);

  /*Call returns immediately if xTicksToWait is 0 and the queue is full.*/
  xQueueStatus = xQueueSend(mbox->mailboxHndl, &msg, 0);
  if (xQueueStatus == pdPASS)
  {
    retVal = ERR_OK;
  }  
  else
  {
    SYS_STATS_INC(mbox.err);
    retVal= ERR_MEM;
  }

  return retVal;
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
  portBaseType xQueueStatus;
  err_t retVal;

  LWIP_ASSERT("mbox is NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mailboxHndl is NULL", mbox->mailboxHndl != NULL);
  LWIP_ASSERT("error, mailbox is in not ISR", Osal_isInISRContext());

  xQueueStatus = xQueueSendFromISR(mbox->mailboxHndl, &msg);
  if (xQueueStatus == pdPASS)
  {
    /* will cause a context switch only if xQueueSendFromISR() internally requested one */
    safertosapiYIELD_FROM_ISR();
    retVal = ERR_OK;
  }
  else
  {
    SYS_STATS_INC(mbox.err);
    retVal = ERR_MEM;
  }

  return retVal;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout_ms)
{
  portBaseType ret;
  u32_t retVal = ERR_OK;
  void *msg_dummy;

  LWIP_ASSERT("mbox is NULL", mbox != NULL);
  LWIP_ASSERT("error, mailbox is in ISR", Osal_isInISRContext() == false);

  if (!msg)
  {
    msg = &msg_dummy;
  }

  if (!timeout_ms) 
  {
    /* wait infinite */
    ret = xQueueReceive(mbox->mailboxHndl, msg, safertosapiMAX_DELAY);
    LWIP_ASSERT("mbox fetch failed", ret == pdPASS);
  } 
  else
  {
    portTickType timeout_ticks = timeout_ms / configTICK_RATE_MS;

    ret = xQueueReceive(mbox->mailboxHndl, msg, timeout_ticks);
    if (ret != pdPASS)
    {
      /* timed out */
      *msg = NULL;
      retVal = SYS_ARCH_TIMEOUT;
    }
  }

  return retVal;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  portBaseType ret;
  u32_t retVal = ERR_OK;
  void *msg_dummy;

  LWIP_ASSERT("mbox is NULL", mbox != NULL);
  LWIP_ASSERT("error, mailbox is in ISR", Osal_isInISRContext() == false);

  if (!msg)
  {
    msg = &msg_dummy;
  }

  /* Call returns immediately if xTicksToWait is 0 and the queue is empty */
  ret = xQueueReceive(mbox->mailboxHndl, msg, 0);
  if (ret != pdPASS)
  {
    *msg = NULL;
    retVal = SYS_MBOX_EMPTY;
  }

  return retVal;
}

void sys_mbox_free(sys_mbox_t *mbox)
{
  portUnsignedBaseType msgs_waiting;
  portBaseType queueResult;

  LWIP_ASSERT("mbox is NULL", mbox != NULL);
  LWIP_ASSERT("mbox->mailboxHndl is NULL", mbox->mailboxHndl != NULL);
  LWIP_ASSERT("error, mailbox is in ISR", Osal_isInISRContext() == false);

#if LWIP_SAFERTOS_CHECK_QUEUE_EMPTY_ON_FREE
  queueResult = xQueueMessagesWaiting(mbox->mailboxHndl, &msgs_waiting);
  LWIP_ASSERT("failed to fetch messages", queueResult == pdPASS);
  LWIP_ASSERT("mbox quence not empty", msgs_waiting == 0);
#endif

  /* NOTE: there is no delete queue API in safertos */
  SYS_STATS_DEC(mbox.used);
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
  portBaseType xCreateResult;
  portTaskHandleType rtos_task;
  sys_thread_t lwip_thread;
  QueueP_Elem *element;
  struct sys_thread_state *pThreadState;

  LWIP_ASSERT("task queue handle null error", sys_thread_state_obj_queue != NULL);

  element = (QueueP_Elem*)QueueP_get(sys_thread_state_obj_queue);
  LWIP_ASSERT("task queue element not found", element != NULL);

  pThreadState = __containerof(element, struct sys_thread_state, queueElement);
  LWIP_ASSERT("invalid stacksize", stacksize > 0 && sizeof(pThreadState->tStack) >= stacksize);
  if (prio > LWIP_TASK_PRIORITY_HIGHEST)
  {
      prio = LWIP_TASK_PRIORITY_HIGHEST;
  }
  if (prio <= LWIP_TASK_PRIORITY_LOWEST)
  {
      prio = LWIP_TASK_PRIORITY_LOWEST;
  }

  /* The structure passed to xTaskCreate() to create the check task. */
  xTaskParameters taskParams =
  {
      thread,                                 /* The function that implements the task being created. */
      name,                                   /* The name of the task being created. The kernel does not use this itself, its just to assist debugging. */
      &pThreadState->tcb,                     /* TCB for the task. */
      pThreadState->tStack,                   /* The buffer allocated for use as the task stack. */
      sizeof(pThreadState->tStack),           /* The size of the buffer allocated for use as the task stack - note this is in BYTES! */
      arg,                                    /* The task parameter. */
      prio,                                   /* The priority to assigned to the task being created. */
      &pThreadState->tlsSemObj,               /* TLS object, A pointer to user defined data to be associated with this task */
      pdFALSE,                                /* Check that task does not use the FPU. */
      {                                       /* MPU task parameters. */
          mpuPRIVILEGED_TASK,                 /* Check task is privileged. */
          {
              { NULL, 0U, 0U, 0U },           /* No additional region definitions are required. */
              { NULL, 0U, 0U, 0U },
          }
      }
  };

  /* Create the check task. */
  xCreateResult = xTaskCreate(&taskParams,    /* The structure containing the task parameters created at the start of this function. */
                              &rtos_task);    /* This parameter is used to receive a handle to the created task*/
  LWIP_ASSERT("error task creation failed", (xCreateResult != pdFAIL) && (rtos_task != NULL));
  lwip_thread.thread_handle = rtos_task;

  return lwip_thread;
}

#if LWIP_NETCONN_SEM_PER_THREAD

sys_sem_t *sys_arch_netconn_sem_get(void)
{
  sys_sem_t *ret;

  LWIP_ASSERT("error, semaphore is in ISR", Osal_isInISRContext() == false);

  /* Returns a pointer to the local storage object of the currently executing task. */
  ret = (sys_sem_t*)pvTaskTLSObjectGet();

  return ret;
}

void sys_arch_netconn_sem_alloc(void)
{
  sys_sem_t *sem;
  err_t err;

  LWIP_ASSERT("error, semaphore is in ISR", Osal_isInISRContext() == false);

  sem = (sys_sem_t*)pvTaskTLSObjectGet();
  LWIP_ASSERT("TLS object invalid", sem != NULL);

  err = sys_sem_new(sem, 0);
  LWIP_ASSERT("error semaphore could not be allocated", err == ERR_OK);
  LWIP_ASSERT("sem invalid", sys_sem_valid(sem));
}

void sys_arch_netconn_sem_free(void)
{
  void *sem;

  LWIP_ASSERT("error, semaphore is in ISR", Osal_isInISRContext() == false);

  sem = pvTaskTLSObjectGet();
  if(sem != NULL)
  {
    sys_sem_free((sys_sem_t *)sem);
  }
}

#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#if LWIP_CHECK_CORE_LOCKING
#if LWIP_TCPIP_CORE_LOCKING

/** Flag the core lock held. A counter for recursive locks. */
static u8_t lwip_core_lock_count = 0U;
static portTaskHandleType lwip_core_lock_holder_thread;

void sys_lock_tcpip_core(void)
{
  sys_mutex_lock(&lock_tcpip_core);
  if (lwip_core_lock_count == 0)
  {
    LWIP_ASSERT("error, cannot lock core, in ISR", Osal_isInISRContext() == false);
    lwip_core_lock_holder_thread = xTaskGetCurrentTaskHandle();
  }
  lwip_core_lock_count++;
}

void sys_unlock_tcpip_core(void)
{
   lwip_core_lock_count--;
   if (lwip_core_lock_count == 0)
   {
       lwip_core_lock_holder_thread = 0;
   }
   sys_mutex_unlock(&lock_tcpip_core);
}

#endif /* LWIP_TCPIP_CORE_LOCKING */

#if !NO_SYS
static portTaskHandleType lwip_tcpip_thread;
#endif

void sys_mark_tcpip_thread(void)
{
#if !NO_SYS
  LWIP_ASSERT("error, in ISR", Osal_isInISRContext() == false);
  lwip_tcpip_thread = xTaskGetCurrentTaskHandle();
#endif
}

void sys_check_core_locking(void)
{
#if !NO_SYS
  if (lwip_tcpip_thread != 0)
  {
    portTaskHandleType current_thread;

    LWIP_ASSERT("error, check core locking failed, in ISR", Osal_isInISRContext() == false);

    current_thread = xTaskGetCurrentTaskHandle();
#if LWIP_TCPIP_CORE_LOCKING
    LWIP_ASSERT("Function called without core lock", (current_thread == lwip_core_lock_holder_thread) && (lwip_core_lock_count > 0));
#else /* LWIP_TCPIP_CORE_LOCKING */
    LWIP_ASSERT("Function called from wrong thread", current_thread == lwip_tcpip_thread);
#endif /* LWIP_TCPIP_CORE_LOCKING */
  }
#endif /* !NO_SYS */
}

#endif /* LWIP_CHECK_CORE_LOCKING*/
