/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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
 *  ======== TimerP_default.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/csl/soc.h>
#include <ti/osal/src/nonos/Nonos_config.h>

/*!
 * @brief gDmTimerPInfoTbl
 *
 * This table sets the default configurations for the DM Timers
 * MCU Domain R5: configures MCU domain's DM Timer instance 0 - 9
 * Main Domain R5: configures Main domain's DM Timer instance 0 - 19
 * Main Domain A72: configures Main domain's DM Timer instance 0 - 19
 *
 */
TimerP_dmTimerDefault gDmTimerPInfoTbl[OSAL_NONOS_MAX_TIMERP_PER_SOC] = {
    {
        "DMTimer0",
        (uintptr_t)CSL_MCU_TIMER0_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER0_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer1",
        (uintptr_t)CSL_MCU_TIMER1_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER1_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer2",
        (uintptr_t)CSL_MCU_TIMER2_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER2_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer3",
        (uintptr_t)CSL_MCU_TIMER3_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER3_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer4",
        (uintptr_t)CSL_MCU_TIMER4_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER4_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer5",
        (uintptr_t)CSL_MCU_TIMER5_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER5_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer6",
        (uintptr_t)CSL_MCU_TIMER6_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER6_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer7",
        (uintptr_t)CSL_MCU_TIMER7_CFG_BASE,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER7_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer8",
        (uintptr_t)0x0,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER8_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer9",
        (uintptr_t)0x0,
        (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER9_INTR_PEND_0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer10",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer11",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer12",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer13",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer14",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer15",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer16",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer17",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer18",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    },
    {
        "DMTimer19",
        (uintptr_t)0x0,
        0,
        TIMERP_EVENT_NOT_AVAILABLE
    }
};

/* Returns the default frequency lower 32 bits */
uint32_t TimerP_getDefaultFreqLo(uint32_t timerId)
{
    (void)timerId;
    /* return the same default frequency for all timer Ids */
    return(TIMERP_TIMER_FREQ_LO);
}

/* Returns the default frequency higher 32 bits */
uint32_t TimerP_getDefaultFreqHi(uint32_t timerId)
{
    (void)timerId;
    /* return the same default frequency for all timer Ids */
    return(TIMERP_TIMER_FREQ_HI);
}

uint32_t TimerP_mapId(uint32_t id, uint32_t coreid)
{
    /* No remaps required */
    return (id);
}
uint32_t TimerP_reverseMapId(uint32_t id, uint32_t coreid)
{
    return (id);
}

/*!
 * @brief TimerP_updateDefaultInfoTbl
 *
 *  The OSAL is built once for a given class of processors (i.e. one lib for R5F
 *  which would be used for R5F in mcu & main domains. If we were to support
 *  one lib per core the size would increase drastically)
 *
 *  TimerP_updateDefaultInfoTbl() is called when TimerP_getTimerBaseAddr()
 *  is called first time to overwrite the default configurations if any.
 *
 *  By default timer details for timer in MCU domain is stored, we will override
 *  with timer details in main domain for R5F in main domain
 */

void TimerP_updateDefaultInfoTbl(void)
{
    uint32_t         i, intBase;
    CSL_ArmR5CPUInfo info = {0U, 0U, 0U};

    CSL_armR5GetCpuID(&info);

    /* Main domain R5F only */
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_1 == info.grpId)
    {
        intBase = CSLR_R5FSS0_CORE0_INTR_TIMER0_INTR_PEND_0;
        for (i = 0U; i < TimerP_numTimerDevices; i++)
        {
            gDmTimerPInfoTbl[i].baseAddr = (uintptr_t)CSL_TIMER0_CFG_BASE + (i * 0x10000U);
            gDmTimerPInfoTbl[i].eventId  = TIMERP_EVENT_NOT_AVAILABLE;

            if ((uintptr_t)CSL_TIMER12_CFG_BASE == gDmTimerPInfoTbl[i].baseAddr)
            {
                /* Interrupt number from Timer12 is a separate group */
                intBase = CSLR_R5FSS0_CORE0_INTR_TIMER12_INTR_PEND_0;
            }
            gDmTimerPInfoTbl[i].intNum   = (int32_t)intBase;
            intBase++;
        }
    }
    return;
}

int32_t TimerP_getPreferredDefInst(void)
{
    int32_t instVal;
    CSL_ArmR5CPUInfo info = {0U, 0U, 0U};
    CSL_armR5GetCpuID(&info);

    if ((CSL_ARM_R5_CLUSTER_GROUP_ID_0 == info.grpId) &&
        (CSL_ARM_R5_CPU_ID_0           == info.cpuID))
    {
        /* Required to be MCU_TIMER0 for SBL build on HS device (okay for GP also) */
        instVal = 0;
    }
    else
    {
        /* Main domain R5F only */
        instVal = 4;
        if (CSL_ARM_R5_CLUSTER_GROUP_ID_1 == info.grpId)
        {
            instVal = 6;
        }
        if (CSL_ARM_R5_CPU_ID_1 == info.cpuID)
        {
            instVal++;
        }
    }
    return (instVal);
}

/* Nothing past this point */
