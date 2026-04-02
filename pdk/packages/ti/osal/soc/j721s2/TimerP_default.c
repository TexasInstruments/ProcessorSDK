/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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

/*
 * This table sets the default configurations for the DM Timers
 * R5: configures MCU domain's DM Timer instance 0 - 9
 * A72: configures Main domain's DM Timer instance 0 - 19
 *
 * TimerP_updateDefaultInfoTbl() is called when TimerP_getTimerBaseAddr()
 * is called first time to overwrite the defaut configurations with
 * the configurations of Main domain's DM Timer instances if
 * R5 is on the Main domain
 */
TimerP_dmTimerDefault gDmTimerPInfoTbl[TimerP_numTimerDevices] = {
  /* Timer ID 0 */
  {
    "DMTimer0",                                                       /* Timer Name */
#if defined (BUILD_MCU)
     /* Default configurations for R5 core */
     (uintptr_t)CSL_MCU_TIMER0_CFG_BASE,                              /* MCU domain's DM Timer base address */
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER0_INTR_PEND_0,      /* MCU domain's DM Timer interrupt number */
     TIMERP_EVENT_NOT_AVAILABLE                                       /* Event Id */
#endif
#if defined (BUILD_MPU)
     /* Default configurations for A72 core */
     (uintptr_t)CSL_TIMER0_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER0_INTR_PEND_0,  /* Main domain's DM Timer interrupt number */
     TIMERP_EVENT_NOT_AVAILABLE                                       /* Event Id */
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER0_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x0E,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER0_INTR_PEND_0,  /* eventId 0x4e0 */
#endif

  },
  /* Timer ID 1 */
  {
    "DMTimer1",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER1_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER1_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER1_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER1_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER1_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x0F,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER1_INTR_PEND_0,    /* eventId 0x4e1 */
#endif
  },
  /* Timer ID 2 */
  {
    "DMTimer2",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER2_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER2_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER2_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER2_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER2_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x10,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER2_INTR_PEND_0,  /* eventId 0x4e2 */
#endif
  },
  /* Timer ID 3 */
  {
    "DMTimer3",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER3_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER3_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER3_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER3_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER3_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x11,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER3_INTR_PEND_0,  /* eventId 0x4e3 */
#endif
  },
  /* Timer ID 4 */
  {
    "DMTimer4",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER4_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER4_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER4_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER4_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER4_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x12,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER4_INTR_PEND_0,  /* eventId 0x4e4 */
#endif
  },
  /* Timer ID 5 */
  {
    "DMTimer5",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER5_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER5_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER5_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER5_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER5_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x13,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER5_INTR_PEND_0,  /* eventId 0x4e5 */
#endif
  },
  /* Timer ID 6 */
  {
    "DMTimer6",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER6_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER6_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER6_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER6_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER6_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x14,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER6_INTR_PEND_0,  /* eventId 0x4e6 */
#endif
  },
   /* Timer ID 7 */
  {
    "DMTimer7",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER7_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER7_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER7_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER7_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_C7X)
     (uintptr_t)CSL_TIMER7_CFG_BASE,                                  /* Main domain's DM Timer base address */
     (int32_t)0x15,                                                   /* intNum */
     992 + (int32_t)CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_TIMER7_INTR_PEND_0,   /* eventId 0x4e7 */
#endif
  },
  /* Timer ID 8 */
  {
    "DMTimer8",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER8_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER8_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER8_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER8_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 9 */
  {
    "DMTimer9",
#if defined (BUILD_MCU)
     (uintptr_t)CSL_MCU_TIMER9_CFG_BASE,
     (int32_t)CSLR_MCU_R5FSS0_CORE0_INTR_MCU_TIMER9_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER9_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER9_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 10 */
  {
    "DMTimer10",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER10_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER10_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 11 */
  {
    "DMTimer11",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER11_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER11_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 12 */
  {
    "DMTimer12",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER12_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER12_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 13 */
  {
    "DMTimer13",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER13_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER13_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 14 */
  {
    "DMTimer14",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER14_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER14_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 15 */
  {
    "DMTimer15",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER15_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER15_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 16 */
  {
    "DMTimer16",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER16_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER16_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 17 */
  {
    "DMTimer17",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER17_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER17_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 18 */
  {
    "DMTimer18",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER18_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER18_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  },
  /* Timer ID 19 */
  {
    "DMTimer19",
#if defined (BUILD_MCU)
     (uintptr_t)0U,
     0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
#if defined (BUILD_MPU)
     (uintptr_t)CSL_TIMER19_CFG_BASE,
     (int32_t)CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_TIMER19_INTR_PEND_0,
     TIMERP_EVENT_NOT_AVAILABLE
#endif
  }
};

/* Returns the default frquency lower 32 bits */
uint32_t TimerP_getDefaultFreqLo(uint32_t timerId)
{
    (void)timerId;
    /* return the same default frequency for all timer Ids */
    return(TIMERP_TIMER_FREQ_LO);
}

/* Returns the default frquency higher 32 bits */
uint32_t TimerP_getDefaultFreqHi(uint32_t timerId)
{
    (void)timerId;
    /* return the same default frequency for all timer Ids */
    return(TIMERP_TIMER_FREQ_HI);
}


uint32_t TimerP_mapId(uint32_t id, uint32_t coreId)
{
    /* No remaps required since all timers interrupts are directly connected to Pulsar VIMs */
    return (id);
}
uint32_t TimerP_reverseMapId(uint32_t id, uint32_t coreId)
{
    return (id);
}

#if defined (BUILD_MCU)
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
    uint32_t         i, intIdx;
    CSL_ArmR5CPUInfo info = {0U, 0U, 0U};

    CSL_armR5GetCpuID(&info);

    /* Main domain R5F only */
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 != info.grpId)
    {
        intIdx = CSLR_R5FSS0_CORE0_INTR_TIMER0_INTR_PEND_0;
        for (i = 0U; i < TimerP_numTimerDevices; i++)
        {
            gDmTimerPInfoTbl[i].baseAddr = (uintptr_t)CSL_TIMER0_CFG_BASE + (i * 0x10000U);
            if(12U == i)
            {
                /* Interrupt number from Timer12 is a separate group */ 
                intIdx = CSLR_R5FSS0_CORE0_INTR_TIMER12_INTR_PEND_0;
            }
            gDmTimerPInfoTbl[i].intNum = (int32_t)intIdx;
            intIdx++;
        }
    }
    return;
}
#endif

/* Nothing past this point */
