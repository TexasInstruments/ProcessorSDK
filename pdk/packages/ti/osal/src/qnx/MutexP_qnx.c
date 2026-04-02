/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
 *  ======== MutexP_qnx.c ========
 */
#include <ti/osal/MutexP.h>
#include <ti/osal/osal.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>


/*
 *  ======== MutexP_create ========
 */
MutexP_Handle MutexP_create(MutexP_Object *mutexObj)
{
    pthread_mutex_t *mutexHandle = (pthread_mutex_t *)calloc(1, sizeof(pthread_mutex_t));

    if (NULL != mutexHandle)
    {
        if (EOK == pthread_mutex_init((pthread_mutex_t *) mutexHandle, NULL))
        {
            return ((MutexP_Handle) mutexHandle);
        }
        else
        {
            DebugP_log1("pthread_mutex_init for QNX Failed - errno-%d", errno);
            return (NULL);
        }
    }
    else
    {
        DebugP_log0("MutexP_create for QNX Failed due to calloc");
        return (NULL);
    }
}


/*
 *  ======== MutexP_delete ========
 */
MutexP_Status MutexP_delete(MutexP_Handle handle)
{
    OSAL_Assert(NULL == handle);

    int ret;

    ret = pthread_mutex_destroy((pthread_mutex_t *)handle);
    if (0 > ret) {
        DebugP_log1("pthread_mutex_destroy for QNX Failed - errno-%d", errno);
        return MutexP_FAILURE;
    }

    free(handle);
    handle = NULL;

    return (MutexP_OK);
}

/*
 *  ======== MutexP_lock ========
 */
MutexP_Status MutexP_lock(MutexP_Handle handle, uint32_t timeout)
{
    OSAL_Assert(NULL == handle);

    int ret;
    struct timespec ts;
    int timeout_ns = timeout*1000000;

    if (MutexP_WAIT_FOREVER == timeout) {
        ret = pthread_mutex_lock((pthread_mutex_t *)handle);
    } else {
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += (timeout_ns)/1000000000;
        ts.tv_nsec += timeout_ns%1000000000;

        ret = pthread_mutex_timedlock((pthread_mutex_t *)handle, &ts);
    }
    if (0 > ret) {
        if (ETIMEDOUT == errno) {
            return (MutexP_TIMEOUT);
        } else {
            return (MutexP_FAILURE);
        }
    }

    return (MutexP_OK);
}

/*
 *  ======== MutexP_unlock ========
 */
MutexP_Status MutexP_unlock(MutexP_Handle handle)
{
    OSAL_Assert(NULL == handle);

    int ret;

    ret = pthread_mutex_unlock((pthread_mutex_t *)handle);
    if (0 > ret) {
        return (MutexP_FAILURE);
    }

    return (MutexP_OK);
}

/* Nothing past this point */

