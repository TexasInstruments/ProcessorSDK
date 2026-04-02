/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 *  \ingroup CSL_IP_MODULE
 *  \defgroup CSL_MAILBOX Mailbox
 *
 *  
 */
 
/**
 *  \file  mailbox.h
 *
 *  \brief This file contains the function prototypes for Mailbox access.
 */

#ifndef MAILBOX_H_
#define MAILBOX_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \anchor MAILBOX_MessageQueue
 *  \name   User Id's Corresponds to the Core using the mailbox
 *          Mailbox Queue's Corresponds to the Mailbox Queue Number
 *  @{
 */
/** \brief Mailbox message queue 0 */
#define MAILBOX_QUEUE_0         0U
/** \brief Mailbox message queue 1 */
#define MAILBOX_QUEUE_1         1U
/** \brief Mailbox message queue 2 */
#define MAILBOX_QUEUE_2         2U
/** \brief Mailbox message queue 3 */
#define MAILBOX_QUEUE_3         3U
/** \brief Mailbox message queue 4 */
#define MAILBOX_QUEUE_4         4U
/** \brief Mailbox message queue 5 */
#define MAILBOX_QUEUE_5         5U
/** \brief Mailbox message queue 6 */
#define MAILBOX_QUEUE_6         6U
/** \brief Mailbox message queue 7 */
#define MAILBOX_QUEUE_7         7U
/** \brief Mailbox message queue 8 */
#define MAILBOX_QUEUE_8         8U
/** \brief Mailbox message queue 9 */
#define MAILBOX_QUEUE_9         9U
/** \brief Mailbox message queue 10 */
#define MAILBOX_QUEUE_10        10U
/** \brief Mailbox message queue 11 */
#define MAILBOX_QUEUE_11        11U
/** \brief Mailbox message queue 12 */
#define MAILBOX_QUEUE_12        12U
/** \brief Mailbox message queue 13 */
#define MAILBOX_QUEUE_13        13U
/** \brief Mailbox message queue 14 */
#define MAILBOX_QUEUE_14        14U
/** \brief Mailbox message queue 15 */
#define MAILBOX_QUEUE_15        15U
/* @} */

/**
 * \brief Mailbox response to notify that the message received/sent is valid
 */
#define MAILBOX_MESSAGE_VALID       0U
/**
 * \brief Mailbox response to notify that the message received/sent is invalid
 */
#define MAILBOX_MESSAGE_INVALID     1U

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                   */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief  This function gets the first message in the queue
 *
 *  \param   baseAddr    It is the Memory address of the Mailbox instance.
 *  \param   queueId     Queue to be read
 *  \param   *msgPtr     Message pointer in which the message will be returned
 *
 *  \return  Validity    The return value indicates whether the message is valid
 */
uint32_t Mailbox_getMessage(uintptr_t baseAddr, 
                            uint32_t queueId, 
                            uint32_t *msgPtr);

/**
 *  \brief  This function writes message in the queue
 *
 *  \param   baseAddr    It is the Memory address of the Mailbox instance.
 *  \param   queueId     Queue to be written
 *  \param   msg         Message to be sent
 *
 *  \return  status      The return value indicates whether the message is
 *                       written to the queue. Possible values are,
 *                       0   -   Written successfully
 *                       0   -   Queue full
 */
uint32_t Mailbox_sendMessage(uintptr_t baseAddr, 
                             uint32_t queueId, 
                             uint32_t msg);

/*  Mailbox user(HW using mailbox) access API's */
/**
 *  \brief   This function enables the new message interrupt for a user for given
 *           queue
 *
 *  \param   baseAddr    It is the Memory address of the Mailbox instance.
 *  \param   userId      User for whom the new message should be intimated
 *  \param   queueId     Queue to be monitored for new message
 *
 *  \return  None
 */
void Mailbox_enableNewMsgInt(uintptr_t baseAddr, 
                             uint32_t userId, 
                             uint32_t queueId);

/**
 *  \brief   This function disables the new message interrupt for a user for given
 *           queue
 *
 *  \param   baseAddr    It is the Memory address of the Mailbox instance.
 *  \param   userId      User for whom the new message event should be disabled
 *  \param   queueId     Queue to be monitored for new message
 *
 *  \return  None
 */
void Mailbox_disableNewMsgInt(uintptr_t baseAddr, 
                              uint32_t userId, 
                              uint32_t queueId);

/**
 * \brief   This function clears the queue not-full status
 *
 * \param   baseAddr    It is the Memory address of the Mailbox instance.
 * \param   userId      User for whom the event should be cleared
 * \param   queueId     Queue for  which the event should be cleared
 *
 * \return  None
 */
void Mailbox_clrNewMsgStatus(uintptr_t baseAddr, 
                             uint32_t userId, 
                             uint32_t queueId);

/**
 * \brief   This function gets the raw new message status
 *
 * \param   baseAddr    It is the Memory address of the Mailbox instance.
 * \param   userId      User for whom the event should be checked
 * \param   queueId     Queue for  which the event should be checked
 *
 * \return  status      status of new message
 */
uint32_t Mailbox_getRawNewMsgStatus(uintptr_t baseAddr, 
                                    uint32_t userId, 
                                    uint32_t queueId);

/**
 * \brief   This function gets message count in the mailbox
 *
 * \param   baseAddr    It is the Memory address of the Mailbox instance.
 * \param   queueId     Mailbox FIFO id
 *
 * \return  status      Queue not full status
 */
uint32_t Mailbox_getMessageCount(uintptr_t baseAddr, 
                                 uint32_t queueId);

/**
 * \brief   This function writes mailbox register
 *
 * \param   baseAddr    It is the Memory address of the Mailbox instance.
 * \param   queueId     Mailbox FIFO id
 * \param   msg         value to be written to mailbox
 *
 * \return  None
 */
void Mailbox_writeMessage(uintptr_t baseAddr, 
                          uint32_t queueId, 
                          uint32_t msg);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MAILBOX_H_ */

