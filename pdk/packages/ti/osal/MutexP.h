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
/**
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_MutexP MutexP
 *            MutexP interface
 *
 *  @{
 */
/** ============================================================================
 *  @file       MutexP.h
 *
 *  @brief      Mailbox module for the RTOS Porting Interface.
  *
 *  ============================================================================
 */

#ifndef ti_osal_MutexP__include
#define ti_osal_MutexP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * \anchor MutexP_Status
 * \name MutexP Status codes
 *
 * @{
 */
/*!
 *  @brief  This enumerator defines the Status codes for MutexP APIs
 */
typedef int32_t MutexP_Status;
/*! API completed successfully */
#define MutexP_OK             0
/*! API failed */
#define MutexP_FAILURE        (-(int32_t)1)
/*! API failed  because of a timeout */
#define MutexP_TIMEOUT        (-(int32_t)2)
/* @} */

/*!
 *  @brief    Wait forever define
 */
#define MutexP_WAIT_FOREVER (~((uint32_t)0U))

/*!
 *  @brief    No wait define
 */
#define MutexP_NO_WAIT       ((uint32_t)0U)


/*!
 *  @brief    Opaque client reference to an instance of a MutexP
 *
 *  A MutexP_Handle returned from the ::MutexP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *MutexP_Handle;

/*!
 *  @brief    MutexP Mutex Object
 *
 *  Structure that contains the MutexP Mutex Object to be used with the
 *  MutexP_constructMutex function
 */
typedef struct MutexP_Object_s {
    void * object;      /*!< Pointer to OS specifc mutex object */
    uintptr_t key;      /*!< The key returned during mutex lock */
} MutexP_Object;


/*!
 *  @brief  Function to create a mutex.
 *
 *  @param  mutexObj  Pointer to the mutex object
 *
 *  @return  A MutexP_Handle on success or a NULL on an error
 */
extern MutexP_Handle MutexP_create(MutexP_Object *mutexObj);

/*!
 *  @brief  Function to delete a mutex.
 *
 *  @param  handle  A MutexP_Handle returned from MutexP_create
 *
 *  @return Status of the functions
 *    - MutexP_OK: Deleted the mutex instance
 *    - MutexP_FAILURE: Failed to delete the mutex instance
 */
extern MutexP_Status MutexP_delete(MutexP_Handle handle);

/*!
 *  @brief  Function to unlock the mutex.
 *
 *  @param  handle  A MutexP_Handle returned from MutexP_create
 *
 *  @return Status of the functions
 *    - MutexP_OK: Unlocked the mutex
 *    - MutexP_FAILURE: Failed to unlock the mutex
 */
extern MutexP_Status MutexP_unlock(MutexP_Handle handle);

/*!
 *  @brief  Function to lock the mutex.
 *
 *  @param  handle  A MutexP_Handle returned from MutexP_create
 *
 *  @param  timeout Timeout (in milliseconds) to wait for lock
 *                  the mutex.
 *
 *  @return Status of the functions
 *    - MutexP_OK: Locked the mutex
 *    - MutexP_TIMEOUT: Timed out waiting for a mutex unlock
 *    - MutexP_FAILURE: Failed to lock the mutex
 */
extern MutexP_Status MutexP_lock(MutexP_Handle handle,
                                 uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_MutexP__include */
/* @} */
