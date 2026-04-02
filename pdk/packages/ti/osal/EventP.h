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

/**
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_EventP EventP
 *            EventP interface
 *
 *  @{
 */ 
 
/** ============================================================================
 *  @file       EventP.h
 *
 *  @brief      Event module for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_EventP__include
#define ti_osal_EventP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * \name Event ID definitions
 * Pre-defined Event Ids. EventP_Ids are provided to simplify the specification
 * of mask argument to EventP_wait().
 * Since each EventP_Id is a bitmask composed of only a single bit,
 * a group of EventP_Ids within a Mask can be indicated by simply adding them together.
 *  @{
 */

/*!
*  @brief    Event ID NONE
*/
#define EventP_ID_NONE     0x0U
/*!
 *  @brief    Event ID 0
 */
#define EventP_ID_00       0x1U
/*!
 *  @brief    Event ID 1
 */
#define EventP_ID_01       0x2U
/*!
 *  @brief    Event ID 2
 */
#define EventP_ID_02       0x4U
/*!
 *  @brief    Event ID 3
 */
#define EventP_ID_03       0x8U
/*!
 *  @brief    Event ID 4
 */
#define EventP_ID_04       0x10U
/*!
 *  @brief    Event ID 5
 */
#define EventP_ID_05       0x20U
/*!
 *  @brief    Event ID 6
 */
#define EventP_ID_06       0x40U
/*!
 *  @brief    Event ID 7
 */
#define EventP_ID_07       0x80U
/*!
 *  @brief    Event ID 8
 */
#define EventP_ID_08       0x100U
/*!
 *  @brief    Event ID 9
 */
#define EventP_ID_09       0x200U
/*!
 *  @brief    Event ID 10
 */
#define EventP_ID_10       0x400U
/*!
 *  @brief    Event ID 11
 */
#define EventP_ID_11       0x800U
/*!
 *  @brief    Event ID 12
 */
#define EventP_ID_12       0x1000U
/*!
 *  @brief    Event ID 13
 */
#define EventP_ID_13       0x2000U
/*!
 *  @brief    Event ID 14
 */
#define EventP_ID_14       0x4000U
/*!
 *  @brief    Event ID 15
 */
#define EventP_ID_15       0x8000U
/*!
 *  @brief    Event ID 16
 */
#define EventP_ID_16       0x10000U
/*!
 *  @brief    Event ID 17
 */
#define EventP_ID_17       0x20000U
/*!
 *  @brief    Event ID 18
 */
#define EventP_ID_18       0x40000U
/*!
 *  @brief    Event ID 19
 */
#define EventP_ID_19       0x80000U
/*!
 *  @brief    Event ID 20
 */
#define EventP_ID_20       0x100000U
/*!
 *  @brief    Event ID 21
 */
#define EventP_ID_21       0x200000U
/*!
 *  @brief    Event ID 22
 */
#define EventP_ID_22       0x400000U
/*!
 *  @brief    Event ID 23
 */
#define EventP_ID_23       0x800000U
/* @} */

/**
 * \anchor EventP_Status
 * \name Status codes for EventP APIs
 *
 * @{
 */
/*!
 *  @brief    Status codes for EventP APIs
 */
typedef int32_t EventP_Status;
/*! API completed successfully */
#define EventP_OK                               ((int32_t) 0)
/*! API failed */
#define EventP_FAILURE                          ((int32_t) (-1))
/* @} */

/**
 * \anchor EventP_WaitMode
 * \name Event wait modes for EventP_wait API
 *
 * @{
 */
/*!
 *  @brief  This enumerator defines the Event wait modes
 */
typedef uint8_t EventP_WaitMode;
/* EventP_wait will return when ANY of the bits set in mask are set in the Event bits */
#define EventP_WaitMode_ANY                     ( (uint8_t) 0U)
/* EventP_wait will return when ALL the bits set in mask are set in the Event bits */
#define EventP_WaitMode_ALL                     ( (uint8_t) 1U)
/* @} */

/*!
 *  @brief    Wait forever define
 */
#define EventP_WAIT_FOREVER                     (~((uint32_t)0U))

/*!
 *  @brief    No wait define
 */
#define EventP_NO_WAIT                          ((uint32_t)0U)


/*!
 *  @brief    Opaque client reference to an instance of a EventP
 *
 *  A EventP_Handle returned from the ::EventP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *EventP_Handle;

/*!
 *  @brief    Basic EventP Parameters
 *
 *  Structure that contains the parameters are passed into ::EventP_create
 *  when creating a EventP instance.
 */
typedef struct EventP_Params_s
{
    void *instance;          /*!< Name of the task instance.*/
} EventP_Params;

/*!
 *  @brief  Function to create an event.
 *
 *  @param  params  Pointer to the instance configuration parameters.
 *
 *  @return A EventP_Handle on success or a NULL on an error
 */
extern EventP_Handle EventP_create(EventP_Params *params);

/*!
 *  @brief  Function to delete an event.
 *
 *  @param  handle  A EventP_Handle returned from EventP_create
 *  @return Status of the functions
 *    - EventP_OK: Deleted the event instance
 *    - EventP_FAILURE: Failed to delete the event instance
 */
extern EventP_Status EventP_delete(EventP_Handle *handle);

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void EventP_Params_init(EventP_Params *params);

/*!
 *  @brief  Function for Event Wait
 *
 *  @param handle    EventP_Handle created via EventP_create API
 *  @param eventMask mask of eventIds to pend on (must be non-zero)
 *                   Only supports upto 24 bits.
 *  @param waitMode  Event wait mode. \ref EventP_WaitMode
 *  @param timeout   return from wait() after this many system time units
 *  @return All consumed events or zero if timeout
 */
extern uint32_t EventP_wait(EventP_Handle handle, uint32_t eventMask,
                            uint8_t waitMode, uint32_t timeout);

/*!
 *  @brief  Function for Event Post.
 *
 *  @param handle  A EventP_Handle returned from EventP_create.
 *  @param eventMask  mask of eventIds to post (must be non-zero)
 *  @return Status of the functions
 *    - EventP_OK: Posted the events
 *    - EventP_FAILURE: Failed to post the events
 */
extern EventP_Status EventP_post(EventP_Handle handle, uint32_t eventMask);

/*!
 *  @brief  Function to return Event Posted.
 *
 *  @param handle  A EventP_Handle returned from EventP_create.
 *  @return All un-consumed events posted
 */
extern uint32_t EventP_getPostedEvents(EventP_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_EventP__include */
/* @} */
