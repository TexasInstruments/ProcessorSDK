/*
 * Copyright (c) 2017-2024, Texas Instruments Incorporated
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
/**
 *  \file  sciclient_osal.c
 *
 *  \brief File containing the implementation of critical section for Sciclient_serviceSecureProxy().
 *         Different OSes have different implementations for acquiring and locking a resource and 
 *         hence the functions in this file can be modified according to the OS in which it is executed.
 */

#include <ti/csl/soc.h>
#include <ti/csl/csl_rat.h>
#include <ti/osal/osal.h>
#include <ti/drv/sciclient/sciclient.h>

extern uint32_t gSciclient_writeInProgress;

int32_t Sciclient_osalAcquireSecureProxyAcess(uintptr_t* key, uint32_t timeout)
{
    uint32_t timeToWait = 0;
    int32_t status = CSL_PASS;

    (*key) = HwiP_disable();
    timeToWait = timeout;
    while (gSciclient_writeInProgress == 1U)
    {
        HwiP_restore((*key));
        if (timeToWait > 0U)
        {
            timeToWait--;
        }
        else
        {
            status = CSL_ETIMEOUT;
            (*key) = HwiP_disable();
            break;
        }
        Osal_delay(10);
        (*key) = HwiP_disable();
    }
    gSciclient_writeInProgress = 1U;
    HwiP_restore((*key));

    return status;
}

void Sciclient_osalReleaseSecureProxyAcess(uintptr_t* key)
{
    (*key) = HwiP_disable();
    gSciclient_writeInProgress = 0U;
    HwiP_restore((*key));
}
