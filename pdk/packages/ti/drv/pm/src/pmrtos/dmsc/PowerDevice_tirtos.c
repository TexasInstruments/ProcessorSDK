/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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
 *  ======== PowerDevice_tirtos.c ========
 */

#include <stdbool.h>
#include <stdint.h>

#include <Power.h>
#include <PowerExtended.h>
#include <PowerDevice.h>

#include <ti/csl/csl_types.h>

#include <ti/osal/HwiP.h>
#include <ti/osal/osal.h>

/*
 *  ======== PowerDevice_policyFxn ========
 */
void PowerDevice_policyFxn()
{
    uint32_t constraints;
    uint32_t key;
    bool     slept = false;

    key = HwiP_disable();
    HwiP_restore(key);

    /* query the declared constraints */
    constraints = Power_getConstraintMask();

    /*
     *  Check if can go to a sleep state, starting with the deepest level.
     *  Do not go to a sleep state if a lesser sleep state is disallowed.
     */

    /* check if can go to DEEPSLEEP */
    if ((constraints & (1 << Power_DISALLOW_DEEPSLEEP)) == 0)
    {
        /* go to DEEPSLEEP */
        Power_sleep(Power_DEEPSLEEP);

        /* set 'slept' to true*/
        slept = TRUE;
    }

    /* if didn't sleep yet, now check if can go to SLEEP */
    if ((TRUE != slept) &&
        ((constraints & (1 << Power_DISALLOW_SLEEP)) == 0))
    {
        /* go to SLEEP */
        Power_sleep(Power_SLEEP);
    }
}

/*
 *  ======== PowerDevice_policyInitFxn ========
 */
void PowerDevice_policyInitFxn()
{
}

/*
 *  ======== PowerDevice_policyDeinitFxn ========
 */
void PowerDevice_policyDeinitFxn()
{
}

