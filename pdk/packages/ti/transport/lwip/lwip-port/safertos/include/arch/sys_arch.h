/**
 *  Copyright (c) Texas Instruments Incorporated 2022
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
 */
#ifndef LWIP_ARCH_SYS_ARCH_H
#define LWIP_ARCH_SYS_ARCH_H

#include <stdbool.h>
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "SafeRTOS_API.h"
#include <ti/osal/QueueP.h>

#define LWIP_SYSARCH_SAFERTOS_MAX2(a,b)       ((a) > (b) ? (a) : (b))
#define LWIP_SYSARCH_SAFERTOS_MAX4(a,b,c,d)   (LWIP_SYSARCH_SAFERTOS_MAX2(LWIP_SYSARCH_SAFERTOS_MAX2(a,b),LWIP_SYSARCH_SAFERTOS_MAX2(c,d)))
#define LWIP_SYSARCH_SAFERTOS_MAX5(a,b,c,d,e) (LWIP_SYSARCH_SAFERTOS_MAX2(LWIP_SYSARCH_SAFERTOS_MAX4(a,b,c,d),e))

#define LWIP_SAFERTOS_MBOX_MAX_MSGCOUNT       (LWIP_SYSARCH_SAFERTOS_MAX5(DEFAULT_ACCEPTMBOX_SIZE, \
                                                  DEFAULT_UDP_RECVMBOX_SIZE, \
                                                  DEFAULT_RAW_RECVMBOX_SIZE, \
                                                  DEFAULT_TCP_RECVMBOX_SIZE, \
                                                  TCPIP_MBOX_SIZE))

void sys_arch_msleep(u32_t delay_ms);
#define sys_msleep(ms)     sys_arch_msleep(ms)

#if !LWIP_COMPAT_MUTEX
typedef struct _sys_mut {
  uint64_t         mutObj[((safertosapiQUEUE_OVERHEAD_BYTES/sizeof(uint64_t)) + 1)];
  xMutexHandleType mutHndl;
} sys_mutex_t;

#define sys_mutex_valid_val(mutex)   ((mutex).mutHndl != NULL)
#define sys_mutex_valid(mutex)       (((mutex) != NULL) && sys_mutex_valid_val(*(mutex)))
#define sys_mutex_set_invalid(mutex) ((mutex)->mutHndl = NULL)
#endif /* !LWIP_COMPAT_MUTEX */

typedef struct _sys_sem {
  uint64_t         semObj[((safertosapiQUEUE_OVERHEAD_BYTES/sizeof(uint64_t)) + 1)];
  xSemaphoreHandle semHndl;
} sys_sem_t;

#define sys_sem_valid_val(sema)      ((sema).semHndl != NULL)
#define sys_sem_valid(sema)          (((sema) != NULL) && sys_sem_valid_val(*(sema)))
#define sys_sem_set_invalid(sema)    ((sema)->semHndl = NULL)

typedef struct _sys_mbox {
    /* Handle through which queue can be referenced */
    xQueueHandle mailboxHndl;
    /* Buffer memory */
    uint64_t     queueBuf[((((sizeof(void *) * LWIP_SAFERTOS_MBOX_MAX_MSGCOUNT) + safertosapiQUEUE_OVERHEAD_BYTES)/sizeof(uint64_t)) + 1)];
} sys_mbox_t;

#define sys_mbox_valid_val(mbox)     ((mbox).mailboxHndl != NULL)
#define sys_mbox_valid(mbox)         (((mbox) != NULL) && sys_mbox_valid_val(*(mbox)))
#define sys_mbox_set_invalid(mbox)   ((mbox)->mailboxHndl = NULL)

/* Threads */
typedef struct _sys_thread {
  void *thread_handle;
} sys_thread_t;

struct sys_thread_state {
  QueueP_Elem  queueElement;
  xTCB         tcb;
  portInt8Type tStack[LWIP_SAFERTOS_MAX_TASK_STACK_SIZE] __attribute__((aligned(LWIP_SAFERTOS_MAX_TASK_STACK_SIZE)));
  sys_sem_t    tlsSemObj;
};

#if LWIP_NETCONN_SEM_PER_THREAD
sys_sem_t *sys_arch_netconn_sem_get(void);
void sys_arch_netconn_sem_alloc(void);
void sys_arch_netconn_sem_free(void);
#define LWIP_NETCONN_THREAD_SEM_GET()      sys_arch_netconn_sem_get()
#define LWIP_NETCONN_THREAD_SEM_ALLOC()    sys_arch_netconn_sem_alloc()
#define LWIP_NETCONN_THREAD_SEM_FREE()     sys_arch_netconn_sem_free()
#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#define configTICK_RATE_HZ              ( ( portTickType ) 1000UL )
/* This macro calculates the number of ticks per millisecond.
 * NOTE: This integer calculation is only correct for values of
 * configTICK_RATE_HZ less than or equal to 1000 that are also divisors
 * of 1000.
 */
#define configTICK_RATE_MS              ( ( portTickType ) 1000UL / configTICK_RATE_HZ )

#endif /* LWIP_ARCH_SYS_ARCH_H */
