/*
 * Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
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

/*!
 * \file  lwip2enet_queue.h
 *
 * \brief Queue of pbufs.
 *
 * lwIP does not natively support buffer queues. This file implements a custom
 * queue. Each member (called a buffer pointer) of the queue is a pointer
 * pointing to a next member as well as to a packet buffer. Every time a packet
 * buffer is passed to the enQ, a buffer pointer is allocated and associated to
 * the packet passed. The buffer pointer is then enqueued. When dequeuing, the
 * buffer pointer is freed. In this way the lwipstack is not modified while still
 * implementing a queue.
 * The enqueue routine for a buffer queued for transmit is slightly different. This
 * is because the stack may free the buffer any time after the buffer is passed
 * the queue is passed to the abstraction layer. To prevent the transmission of
 * empty buffers, additional steps must be made before queuing the packet.
 */

#ifndef LWIP2ENET_QUEUE_H_
#define LWIP2ENET_QUEUE_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/pbuf.h"
#include "lwipopts.h"

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*!
 * \brief Total number of available pbuf container nodes for all queues.
 *
 * Note: Make sure this number is sufficiently high to accommodate all the
 * buffers that could potentially be made by the stack in worst scenario.
 */
#define LWIP2ENETQ_POOL_SIZE                     PBUF_POOL_SIZE

/*!
 * \brief Get number of elements in the queue.
 */
#define Lwip2EnetQ_count(queue)                  ((queue)->count)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/*!
 * \brief Node in a Lwip2Enet queue.
 *
 * This node essentially acts as a container of the pbuf.
 */
typedef struct Lwip2EnetQ_Node_s
{
    /*! pbuf */
    struct pbuf *pbuf;

    /*! Pointer to next element in the queue */
    struct Lwip2EnetQ_Node_s *next;
} Lwip2EnetQ_Node;

/*!
 * \brief Lwip2Enet queue of pbufs.
 *
 * Note that this queue is different that the lwIP's native/internal pbuf queue.
 */
typedef struct Lwip2EnetQ_Queue_s
{
    /*! First node in the queue */
    Lwip2EnetQ_Node *head;

    /*! Last node in the queue */
    Lwip2EnetQ_Node *tail;

    /*! Number of elements in the queue */
    uint32_t count;
} Lwip2EnetQ_Queue;

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Initialize Lwip2EnetQ object.
 *
 * Initializes the backing memory used for the nodes in the pbuf queue.
 */
void Lwip2EnetQ_init(void);

/*!
 * \brief Initialize Lwip2EnetQ queue.
 *
 * Initialize pbuf queue.
 *
 * \param queue      Pointer to the pbuf queue.
 */
void Lwip2EnetQ_initQ(Lwip2EnetQ_Queue *queue);

/*!
 * \brief Enqueue a pbuf to the Lwip2EnetQ queue.
 *
 * Enqueues a pbuf to the pbuf queue.
 *
 * \param queue      Pointer to the pbuf queue.
 * \param pbuf       Pointer to the pbuf to be enqueued.
 */
void Lwip2EnetQ_enq(Lwip2EnetQ_Queue *queue,
                    struct pbuf *pbuf);

/*!
 * \brief Enqueue a pbuf to the head of the Lwip2EnetQ queue.
 *
 * Enqueues a pbuf to the head of pbuf queue.  This operation is handy when a pbuf
 * previously dequeued needs to be added back in the queue, and adding it to the
 * end of the queue would cause losing order.
 *
 * \param queue      Pointer to the pbuf queue.
 * \param pbuf       Pointer to the pbuf to be enqueued.
 */
void Lwip2EnetQ_enqHead(Lwip2EnetQ_Queue *queue,
                        struct pbuf *pbuf);

/*!
 * \brief Dequeue a pbuf from the Lwip2EnetQ queue.
 *
 * Dequeues a pbuf to the pbuf queue.
 *
 * \param queue      Pointer to the pbuf queue.
 *
 * \return Pointer to the dequeued pbuf or NULL if queue was already empty.
 */
struct pbuf *Lwip2EnetQ_deq(Lwip2EnetQ_Queue *queue);

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*                        Deprecated Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#endif /* LWIP2ENET_QUEUE_H_ */
