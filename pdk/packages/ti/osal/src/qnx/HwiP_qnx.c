/*
 * Copyright (c) 2016-2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== HwiP_qnx.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <ti/osal/HwiP.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include <ti/osal/osal.h>

typedef struct
{
    uint32_t evtId;
    int chid;
    struct sigevent isr_event;
    uint32_t coreIntrNum;
    uint32_t intrPriority;
    HwiP_Fxn isrFxn;
    uint32_t irq_autoEnable;
    uintptr_t arg;
} qnx_osal_hwi_info;

#define QNX_OSAL_MAX_INTR_COUNT           (16U)
#define ISR_PULSE                          _PULSE_CODE_MINAVAIL

static qnx_osal_hwi_info      g_hwi[QNX_OSAL_MAX_INTR_COUNT];
static uint8_t                g_currIntrCount = 0;

/*
 *  ======== isr_thread ========
 */
/* Thread level ISR handler */
void *isr_thread (void *arg)
{
    qnx_osal_hwi_info *hwi = (qnx_osal_hwi_info *)arg;
    int             rcvid;
    struct _pulse   pulse;
    uint32_t        interruptNum;

    while (1) {
        rcvid = MsgReceivePulse(hwi->chid, &pulse, sizeof(struct _pulse), NULL);
        if (-1 != rcvid) {
            switch (pulse.code) {
                case ISR_PULSE:
                    interruptNum = pulse.value.sival_int;

                    /* Call the callback function */
                    hwi->isrFxn(hwi->arg);

                    /* Check if application wants interrupt re-enabled */
                    if(1U == hwi->irq_autoEnable) {
                        HwiP_enableInterrupt(interruptNum);
                    }
                    break;
                default:
                    DebugP_log1("Rx Unknown pulse %d received",pulse.code);
                    break;
            }
        }
        else {
            DebugP_log0("MsgReceivePulse failed");
            delay(20);
        }
    }
    return NULL;
}


/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    return;
}


/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(uint32_t coreIntrNum, HwiP_Fxn hwiFxn,
                        const HwiP_Params *params)
{
    pthread_t      tid;
    pthread_attr_t thread_attr;
    struct sched_param  param;
    qnx_osal_hwi_info  *hwi = NULL;
    char threadName[128];
    uint32_t intrPriority;

    // NOTE: Override the interrupt priority pass as params->priority
    intrPriority = 21;

    if (QNX_OSAL_MAX_INTR_COUNT <= g_currIntrCount) {
        DebugP_log0("MAXed out on the hwi structure");
        OSAL_Assert(1);
    }
    hwi = &g_hwi[g_currIntrCount];
    g_currIntrCount++;


    hwi->chid = ChannelCreate( _NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK);
    if(-1 == hwi->chid)
    {
        DebugP_log1("Failed to create chid:%d", hwi->chid);
        OSAL_Assert(1);
    }

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    param.sched_priority = intrPriority;
    pthread_attr_setschedparam(&thread_attr, &param);

    if (EOK != pthread_create(&tid, &thread_attr, (void *)isr_thread, (void *)hwi)) {
        DebugP_log0("Unable to create isr thread");
        OSAL_Assert(1);
    }
    sprintf(threadName, "IntrThread_%d",coreIntrNum);
    pthread_setname_np(tid, threadName);

    /* Store pointer to the hwi structure */
    hwi->isrFxn = hwiFxn;
    hwi->coreIntrNum = coreIntrNum;
    hwi->intrPriority = intrPriority;
    hwi->arg = params->arg;
    hwi->irq_autoEnable = params->autoEnable;

    /* Init the pulse for interrupt event */
    hwi->isr_event.sigev_notify = SIGEV_PULSE;
    hwi->isr_event.sigev_code = _PULSE_CODE_MINAVAIL;
    hwi->isr_event.sigev_coid = ConnectAttach(0, 0, hwi->chid, _NTO_SIDE_CHANNEL, 0);
    hwi->isr_event.sigev_priority = intrPriority;     /* service interrupts at a higher priority then client requests */
    hwi->isr_event.sigev_value.sival_int = coreIntrNum;

    /*
     * Attach interrupt handler (thread level)
     *
     * _NTO_INTR_FLAGS_TRK_MSK - Track mask/unmask
     * _NTO_INTR_FLAGS_NO_UNMASK - Start with interrupt masked
     */
    hwi->evtId = InterruptAttachEvent (coreIntrNum, &hwi->isr_event,  0 /*_NTO_INTR_FLAGS_NO_UNMASK*/);
    if(-1 == hwi->evtId)
    {
       DebugP_log0("InterruptAttachEvent failed");
       OSAL_Assert(1);
    }
    else
    {
        //printf("%s: InterruptAttachEvent succeed irq/%d\n",__FUNCTION__, coreIntrNum);
    }

    return ((HwiP_Handle)hwi);
}

/*
 *  ======== HwiP_delete ========
 */
HwiP_Status HwiP_delete(HwiP_Handle handle)
{

    /* Need params->evtId returned by InterruptAttach */
    //InterruptDetach();

    return (HwiP_OK);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    intrspin_t *spinlock_ptr = NULL;

    /* Create a spinLock and lock interrupts */
    spinlock_ptr = malloc(sizeof(intrspin_t));
    OSAL_Assert(NULL == spinlock_ptr);
    memset( (void *) spinlock_ptr, 0, sizeof(intrspin_t) );

    InterruptLock(spinlock_ptr);

    return ((uintptr_t) spinlock_ptr);
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t key)
{
    /* Enable interrupts */

    OSAL_Assert(NULL == (intrspin_t *) key);
    InterruptUnlock((intrspin_t *) key);
    free((void *) key);
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(uint32_t interruptNum)
{
    InterruptMask(interruptNum, -1); // -1, don't track mask/unmask count
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(uint32_t interruptNum)
{
    /* Unmask interrupt */
    InterruptUnmask (interruptNum, -1);
}

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(uint32_t interruptNum)
{
  /* Empty */
}

/* Nothing past this point */
