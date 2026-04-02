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
 * \file  lwip2enet_queue.c
 *
 * \brief Queue of pbufs.
 */

#include <assert.h>
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip2enet_queue.h"
#include <ti/osal/osal.h>
#include <ti/drv/enet/include/core/enet_utils.h>

/*! Queue of free Lwip2EnetQ nodes */
static Lwip2EnetQ_Queue gLwip2EnetQ_freeQ;

/*! Node memory */
static Lwip2EnetQ_Node gLwip2EnetQ_nodeMem[LWIP2ENETQ_POOL_SIZE];

static bool gLwip2EnetQ_initialized = BFALSE;

static inline void Lwip2EnetQ_assert(uint8_t cond)
{
    assert(cond);
}

/*
 * Initializes the freeQ with node memory.
 */
//TODO: Refractor using enQ/deQ
void Lwip2EnetQ_init(void)
{
    uint32_t i;

    if (!gLwip2EnetQ_initialized)
    {
        for (i = 0U; i < (LWIP2ENETQ_POOL_SIZE - 1); i++)
        {
            gLwip2EnetQ_nodeMem[i].next = &gLwip2EnetQ_nodeMem[i + 1];
            gLwip2EnetQ_nodeMem[i].pbuf = NULL;
        }

        gLwip2EnetQ_nodeMem[i].next = NULL;
        gLwip2EnetQ_nodeMem[i].pbuf = NULL;

        gLwip2EnetQ_freeQ.head = &gLwip2EnetQ_nodeMem[0];
        gLwip2EnetQ_freeQ.tail = &gLwip2EnetQ_nodeMem[LWIP2ENETQ_POOL_SIZE - 1];
        gLwip2EnetQ_freeQ.count = LWIP2ENETQ_POOL_SIZE;

        gLwip2EnetQ_initialized = BTRUE;
    }
}

/*
 * Allocates memory from the freeQ to be used by other queues
 */
static Lwip2EnetQ_Node *Lwip2EnetQ_allocNode(void)
{
    Lwip2EnetQ_Node *node = gLwip2EnetQ_freeQ.head;

    gLwip2EnetQ_freeQ.head = gLwip2EnetQ_freeQ.head->next;
    gLwip2EnetQ_freeQ.count--;

    return node;
}

/*
 * Returns buffer pointers used by other queues back to freeQ
 */
static void Lwip2EnetQ_freeNode(Lwip2EnetQ_Node *node)
{
    node->next = NULL;
    node->pbuf = NULL;
    gLwip2EnetQ_freeQ.tail->next = node;
    gLwip2EnetQ_freeQ.tail = node;
    gLwip2EnetQ_freeQ.count++;
}

/*
 * Initializes a queue. Must be called after declaring a queue
 */
void Lwip2EnetQ_initQ(Lwip2EnetQ_Queue *queue)
{
    uint32_t key;

    key = HwiP_disable();

    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;

    HwiP_restore(key);
}

/*
 * Enqueues a buffer to the tail of the queue
 */
void Lwip2EnetQ_enq(Lwip2EnetQ_Queue *queue,
                    struct pbuf *pbuf)
{
    Lwip2EnetQ_Node *node = NULL;
    uint32_t key;

    key = HwiP_disable();

    node = Lwip2EnetQ_allocNode();
    Lwip2EnetQ_assert(NULL != node);

    node->pbuf = pbuf;
    node->next = NULL;

    if (queue->count == 0)
    {
        queue->head = node;
        queue->tail = node;
    }
    else if (queue->count == 1)
    {
        queue->head->next = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = queue->tail->next;
    }

    queue->count++;

    HwiP_restore(key);
}

/*
 * Enqueues a packet to the head of the queue.
 */
void Lwip2EnetQ_enqHead(Lwip2EnetQ_Queue *queue,
                        struct pbuf *pbuf)
{
    Lwip2EnetQ_Node *node = NULL;
    uint32_t key;

    Lwip2EnetQ_assert(pbuf != NULL);

    node = Lwip2EnetQ_allocNode();
    Lwip2EnetQ_assert(NULL != node);

    key = HwiP_disable();

    node->pbuf = pbuf;
    node->next = NULL;
    if (queue->count == 0)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        node->next = queue->head;
        queue->head = node;
    }

    HwiP_restore(key);
}

/*
 * Dequeues from the queue
 */
struct pbuf *Lwip2EnetQ_deq(Lwip2EnetQ_Queue *queue)
{
    Lwip2EnetQ_Node *node = NULL;
    struct pbuf *pbuf = NULL;
    uint32_t key;

    key = HwiP_disable();

    if (queue->count != 0)
    {
        pbuf = queue->head->pbuf;
        node = queue->head;
        queue->head = queue->head->next;
        if (queue->count == 1)
        {
            queue->tail = NULL;
        }
        queue->count--;

        Lwip2EnetQ_assert(pbuf != NULL);
        Lwip2EnetQ_assert(pbuf->payload != NULL);
        Lwip2EnetQ_freeNode(node);
    }

    HwiP_restore(key);

    return pbuf;
}
