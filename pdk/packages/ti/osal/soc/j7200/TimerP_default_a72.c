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
TimerP_dmTimerDefault gDmTimerPInfoTbl[TimerP_numTimerDevices] = {
  /* Timer ID 0 */
  {
    "DMTimer0",
    (uintptr_t)CSL_TIMER0_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER0_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 1 */
  {
    "DMTimer1",
    (uintptr_t)CSL_TIMER1_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER1_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 2 */
  {
    "DMTimer2",
    (uintptr_t)CSL_TIMER2_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER2_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 3 */
  {
    "DMTimer3",
    (uintptr_t)CSL_TIMER3_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER3_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 4 */
  {
    "DMTimer4",
    (uintptr_t)CSL_TIMER4_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER4_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 5 */
  {
    "DMTimer5",
    (uintptr_t)CSL_TIMER5_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER5_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 6 */
  {
    "DMTimer6",
    (uintptr_t)CSL_TIMER6_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER6_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
   /* Timer ID 7 */
  {
    "DMTimer7",
    (uintptr_t)CSL_TIMER7_CFG_BASE,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER7_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 8 */
  {
    "DMTimer8",
    (uintptr_t)0x0,
    (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER8_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 9 */
  {
    "DMTimer9",
    (uintptr_t)0x0,
    (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER9_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 10 */
  {
    "DMTimer10",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER10_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 11 */
  {
    "DMTimer11",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER11_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 12 */
  {
    "DMTimer12",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER12_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 13 */
  {
    "DMTimer13",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER13_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 14 */
  {
    "DMTimer14",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER14_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 15 */
  {
    "DMTimer15",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER15_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 16 */
  {
    "DMTimer16",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER16_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 17 */
  {
    "DMTimer17",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER17_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 18 */
  {
    "DMTimer18",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER18_INTR_PEND_0,
    TIMERP_EVENT_NOT_AVAILABLE
  },
  /* Timer ID 19 */
  {
    "DMTimer19",
    (uintptr_t)0x0,
    (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER19_INTR_PEND_0,
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

uint32_t TimerP_mapId(uint32_t id, uint32_t coreId)
{
    /* No remaps required */
    return (id);
}

uint32_t TimerP_reverseMapId(uint32_t id, uint32_t coreId)
{
    return (id);
}

/* Nothing past this point */
