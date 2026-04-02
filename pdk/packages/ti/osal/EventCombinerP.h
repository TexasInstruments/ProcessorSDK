/*
 * Copyright (c) 2017-present, Texas Instruments Incorporated
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
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_EventCombinerP EventCombinerP
 *            EventCombinerP interface
 *
 *  @{
 */ 
  
 
/** ============================================================================
 *  @file       EventCombinerP.h
 *
 *  @brief      Event Combiner routines for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_EventCombinerP__include
#define ti_osal_EventCombinerP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/osal/HwiP.h>
typedef void (*EventCombinerP_FuncPtr)(uint32_t arg);

/*!
 *  @brief  Function to disable an event inside event combiner
 *
 *  @param  eventId  event ID to be disabled in the event combiner
 *
 */
int32_t EventCombinerP_disableEvent(uint32_t eventId);

/*!
 *  @brief  Function to enable an event inside event combiner
 *
 *  @param  eventId  event ID to be disabled in the event combiner
 *
 */
int32_t EventCombinerP_enableEvent(uint32_t eventId);

/*!
 *  @brief  Function to clear an event inside event combiner
 *
 *  @param  eventId  event ID to be cleared in the event combiner
 *
 */
int32_t EventCombinerP_clearEvent(uint32_t eventId);

/*!
 *  @brief  Function to plug an ISR handler to an event inside event combiner
 *
 *  @param  eventId  The event id (4-127)
 *
 *  @param  eventIsrRoutine  The ISR routine for servicing the event
 *
 *  @param  arg  The argument to the ISR routine
 *
 *  @param  unmask Unmask the event
 */
int32_t EventCombinerP_dispatchPlug(uint32_t eventId, EventCombinerP_FuncPtr eventIsrRoutine,uintptr_t arg,bool unmask );

/*!
 *  @brief  Function to unplug an ISR handler to an event inside event combiner
 *
 *  @param  eventId  The event id (4-127)
 *
 */
int32_t EventCombinerP_dispatchUnplug(uint32_t eventId);

/*!
 *  @brief  Function to return the Hwi Handle corresponding to an event combiner group number
 *
 *  @param  groupNum  The event combiner group id (0-3)
 *
 */
HwiP_Handle EventCombinerP_getHwi(uint32_t groupNum);


/*!
 *  @brief  Function to return the interrupt vector number corresponding to
 *  a group number (0-3)
 *
 *  @param  groupNum  The event combiner group id (0-3)
 *
 */
uint32_t EventCombinerP_getIntNum(uint32_t groupNum);

/*!
 *  @brief  Function to register single event combiner group to a vector number
 *
 *  @param  groupNum  The event combiner group id (0-3)
 *
 *  @param  intNum    The interrupt vector id (4-15)
 *
 */
int32_t EventCombinerP_SingleRegisterInt(uint32_t groupNum, uint32_t intNum);

/*!
 *  @brief  Function to register single event combiner group to a vector number
 *
 *  @param  intNum []   List of interrupt vector ids (4-15) per ECM group(0-3)
 *
 */
int32_t EventCombinerP_GroupRegisterInt(uint32_t intNum[]);

/**
 *  \name Osal Event Combiner return Codes
 *  @brief  Return codes for Osal  Event Combiner functions
 *  @{
 */
/*! Successful call */
#define OSAL_EVTCOMBINE_GROUPREG_SUCCESS (0)
/*! Registration Error */
#define OSAL_EVTCOMBINE_ERR_GROUPREG_REGISTER_ERR (-1)
/*! Invalid interrupt number */
#define OSAL_EVTCOMBINE_ERR_GROUPREG_INVALID_INTNUM (-2)
/* @} */

/**
 *  \name Osal Event Combiner Defualt Group Interrupts
 *  @brief  Deafult group interrupt numbers for the Event Combiner
 *
 * These are the system default interrupt numbers if the Event Combiner has already not been registered
 * If applications register Event Combiners during startup, these values may not be used (in SYSBIOS case)
 * If not, these are the interrupts reserved for Event Combiner group events (0-3).
 * These defaults are stored in Osal_HwAttrs.ECM_intNum[] and can be obtained by the 
 * application at run time through the OSAL API Osal_getHwAttrs(). These values also can be modified 
 * using Osal_setHwAttrs(Osal_HwAttrs,OSAL_HWATTR_SET_ECM_INT)
 *  @{
 */
/*! ECM group 0 default interrupt: 4 */
#define OSAL_ECM_GROUP0_INT 4U
/*! ECM group 1 default interrupt: 5 */
#define OSAL_ECM_GROUP1_INT 5U
/*! ECM group 2 default interrupt: 6 */
#define OSAL_ECM_GROUP2_INT 6U
/*! ECM group 3 default interrupt: 7 */
#define OSAL_ECM_GROUP3_INT 7U
/* @} */

#ifdef __cplusplus
}
#endif

#endif /* ti_osalEventCombinerP__include */
/* @} */
