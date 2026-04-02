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
 *  \defgroup DRV_OSAL_Queue Queue
 *            Queue interface
 *
 *  @{
 */ 
 
/** ============================================================================
 *  @file       QueueP.h
 *
 *  @brief      Queue Handling routines for RTOS 
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_QueueP__include
#define ti_osal_QueueP__include

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \anchor QueueP_Status
 * \name Status codes for QueueP APIs
 *
 * @{
 */
/*!
 *  @brief    Status codes for QueueP APIs
 */
typedef int32_t QueueP_Status;
/*! API completed successfully */
#define QueueP_OK           (int32_t)(0)
/*! API failed */
#define QueueP_FAILURE      (int32_t)(-1)
/* @} */

/**
 * \anchor QueueP_State
 * \name State codes for current queue state
 *
 * @{
 */
/*!
 *  @brief    State codes for current queue state
 */
typedef bool QueueP_State;
/*! Elements are present in the the queue */
#define QueueP_NOTEMPTY            (BFALSE)
/*! Queue is empty. No elements are present. */
#define QueueP_EMPTY               (BTRUE)
/* @} */

/*!
 *  @brief    Opaque client reference to an instance of a QueueP
 *
 *  A QueueP_Handle returned from the ::QueueP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *QueueP_Handle;

/*!
 *  @brief  Opaque QueueP element
 *
 *  Structure that defines a single queue element and/or a list of queue elements.
 *  A field of this type is placed at the head of client structs.
 *
 */
typedef struct QueueP_Elem_s{
    /*! Pointer to the next queue element */
    struct QueueP_Elem_s *next;

    /*! Pointer to the previous queue element */
    struct QueueP_Elem_s *prev;
} QueueP_Elem;

/*!
 *  @brief    Basic QueueP Parameters
 *
 *  Structure that contains the parameters are passed into ::QueueP_create
 *  when creating a QueueP instance.
 */
typedef struct QueueP_Params_s
{
    void *pErrBlk;     /*!< Pointer to the error block for queue create */
} QueueP_Params;

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  [out] Pointer to the instance configuration parameters.
 */
extern void QueueP_Params_init(QueueP_Params *params);

/*!
 *  @brief  Function to create a queue.
 *
 *  @param  params  [in] Pointer to the instance configuration parameters.
 *
 *  @return A QueueP_Handle on success or a NULL on an error
 */
extern QueueP_Handle QueueP_create(const QueueP_Params *params);

/*!
 *  @brief  Function to delete a queue.
 *
 *  @param  handle  [in] A QueueP_Handle returned from QueueP_create
 *
 *  @return Status of the functions
 *    - QueueP_OK: Deleted the queue instance
 *    - QueueP_FAILURE: Failed to delete the queue instance
 */
extern QueueP_Status QueueP_delete(QueueP_Handle handle);

/*!
 *  @brief  Function to Get the element at the front of the queue.
            This function removes an element from the front of a queue and returns it.
 *
 *  @param  handle  [in] A QueueP_Handle returned from QueueP_create
 *
 *  @return pointer to the element or 
 *          pointer to queue itself incase of empty queue
 */
extern void * QueueP_get(QueueP_Handle handle);

/*!
 *  @brief  Function to Put an element at end of queue.
 *
 *  @param  handle  [in] A QueueP_Handle returned from QueueP_create
 *
 *  @param  elem [in] Pointer to new queue element
 *
 *  @return Status of the functions
 *    - QueueP_OK: Put the element at end of queue
 *    - QueueP_FAILURE: Failed to Put the element at end of queue
 */
extern QueueP_Status QueueP_put(QueueP_Handle handle,
                                     void *elem);

/*!
 *  @brief  Function to perform queue empty check
 *
 *  @param  handle  [in] A QueueP_Handle returned from QueueP_create
 *
 *  @return Current state of the Queue
 *    - QueueP_NOTEMPTY: queue is not empty
 *    - QueueP_EMPTY: queue is empty
 */
extern QueueP_State QueueP_isEmpty(QueueP_Handle handle);

/*!
 *  @brief  Function to get pointer to the queue
 *
 *  @param  handle  [in] A QueueP_Handle returned from QueueP_create
 *
 *  @return pointer to the queue
 */
extern void * QueueP_getQPtr(QueueP_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_QueueP__include */
/* @} */
