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
 *  \defgroup DRV_OSAL_MailboxP MailboxP
 *            MailboxP interface
 *
 *  @{
 */
/** ============================================================================
 *  @file       MailboxP.h
 *
 *  @brief      Mailbox module for the RTOS Porting Interface.
  *
 *  ============================================================================
 */

#ifndef ti_osal_MailboxP__include
#define ti_osal_MailboxP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 *  @brief    Status codes for MailboxP APIs
 */
typedef enum MailboxP_Status_e
{
    /*! API completed successfully */
    MailboxP_OK         = 0,
    /*! API failed */
    MailboxP_FAILURE    = (-(int32_t)1),
    /*! API failed  because of a timeout */
    MailboxP_TIMEOUT    = (-(int32_t)2)
} MailboxP_Status;

/*!
 *  @brief    Wait forever define
 */
#define MailboxP_WAIT_FOREVER (~((uint32_t)0U))

/*!
 *  @brief    No wait define
 */
#define MailboxP_NO_WAIT       ((uint32_t)0U)


/*!
 *  @brief    Opaque client reference to an instance of a MailboxP
 *
 *  A MailboxP_Handle returned from the ::MailboxP_create represents that
 *  instance and  is used in the other instance based functions
 */
typedef  void *MailboxP_Handle;

/*!
 *  @brief    Basic MailboxP Parameters
 *
 *  Structure that contains the parameters are passed into ::MailboxP_create
 *  when creating a MailboxP instance.
 *  NOTE : bufsize in SafeRTOS should have  additional portQUEUE_OVERHEAD_BYTES.
 */
typedef struct MailboxP_Params_s
{
    void *pErrBlk;     /*!< Pointer to the error block for mailbox create */
    void *name;        /*!< Name of the mailbox */
    uint32_t size;     /*!< The size of message, in bytes*/
    uint32_t count;    /*!< Length of mailbox. The maximum number of items the mailbox can hold at any one time. */
    uint32_t bufsize;  /*!< Buffer size. This should be large enough to hold the maximum number of items */
    void *buf;         /*!< pointer to buffer memory */
} MailboxP_Params;


/*!
 *  @brief  Initialize params structure to default values.
 *
 *  @param params  Pointer to the instance configuration parameters.
 */
extern void MailboxP_Params_init(MailboxP_Params *params);

/*!
 *  @brief  Function to create a mailbox.
 *
 *  @param  params  Pointer to the instance configuration parameters.
 *
 *  @return A MailboxP_Handle on success or a NULL on an error
 */
extern MailboxP_Handle MailboxP_create(const MailboxP_Params *params);

/*!
 *  @brief  Function to delete a mailbox.
 *
 *  @param  handle  A MailboxP_Handle returned from MailboxP_create
 *
 *  @return Status of the functions
 *    - MailboxP_OK: Deleted the mailbox instance
 *    - MailboxP_FAILURE: Failed to delete the mailbox instance
 */
extern MailboxP_Status MailboxP_delete(MailboxP_Handle handle);

/*!
 *  @brief  Function to post an message to the mailbox.
 *
 *  @param  handle  A MailboxP_Handle returned from MailboxP_create
 *
 *  @param  msg  The message to post
 *
 *  @param  timeout Timeout (in milliseconds) to wait for post a
 *                  message to the mailbox.
 *
 *  @return Status of the functions
 *    - MailboxP_OK: Deleted the mailbox instance
 *    - MailboxP_TIMEOUT: Timed out posting of message
 *    - MailboxP_FAILURE: Failed to start the mailbox instance
 */
extern MailboxP_Status MailboxP_post(MailboxP_Handle handle,
                                     void * msg,
                                     uint32_t timeout);

/*!
 *  @brief  Function to pend on a message for the mailbox.
 *
 *  @param  handle  A MailboxP_Handle returned from MailboxP_create
 *
 *  @param  msg  The message to pend
 *
 *  @param  timeout Timeout (in milliseconds) to wait for pend on a 
 *                  message to the mailbox.
 *
 *  @return Status of the functions
 *    - MailboxP_OK: Deleted the mailbox instance
 *    - MailboxP_TIMEOUT: Timed out pending on message
 *    - MailboxP_FAILURE: Failed to stop the mailbox instance
 */
extern MailboxP_Status MailboxP_pend(MailboxP_Handle handle,
                                     void * msg,
                                     uint32_t timeout);

/*!
 *  @brief  Function to return the count of the pending messages
 *
 *  @param  handle  A MailboxP_Handle returned from MailboxP_create
 *
 *  @return The count of the pending messages
 */
extern int32_t MailboxP_getNumPendingMsgs(MailboxP_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_osal_MailboxP__include */
/* @} */
