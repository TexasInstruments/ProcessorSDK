/*
 * Copyright (c) 2020-2021, Texas Instruments Incorporated
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
 *  ======== QueueP_qnx.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ti/osal/QueueP.h>
#include <ti/osal/HwiP.h>

/*
 *  ======== QueueP_Params_init ========
 */
void QueueP_Params_init(QueueP_Params *params)
{
    if (NULL != params)
    {
        params->pErrBlk =  NULL;
    }
    return;
}

/*
 *  ======== QueueP_create ========
 */
QueueP_Handle QueueP_create(const QueueP_Params *params)
{
    QueueP_Elem *obj = NULL;

    obj = calloc(1, sizeof(QueueP_Elem));
    if (NULL != obj)
    {
        obj->next = (QueueP_Elem *)obj;
        obj->prev = (QueueP_Elem *)obj;
    }
    return (QueueP_Handle)obj;
}

/*
 *  ======== QueueP_delete ========
 */
QueueP_Status QueueP_delete(QueueP_Handle queueHandle)
{
    if (NULL != queueHandle)
    {
        free(queueHandle);
    }

    return QueueP_OK;
}

/*
 *  ======== QueueP_get ========
 */
void * QueueP_get(QueueP_Handle queueHandle)
{
    QueueP_Elem *elem = NULL;
    QueueP_Elem *obj = (QueueP_Elem *)queueHandle;
    uintptr_t key;

    if (NULL != obj)
    {
        key = HwiP_disable();

        elem = (QueueP_Elem*)obj->next;
        obj->next = elem->next;
        ((QueueP_Elem*)(elem->next))->prev = (QueueP_Elem*)obj;

        HwiP_restore(key);
    }

    return (elem);
}

/*
 *  ======== QueueP_put ========
 */
QueueP_Status QueueP_put(QueueP_Handle queueHandle, void *new)
{
    uintptr_t key;
    QueueP_Elem *obj = (QueueP_Elem *)queueHandle;
    QueueP_Elem *elem = (QueueP_Elem *)new;

    if ((NULL != obj) && (NULL != elem))
    {
        key = HwiP_disable();

        elem->next = (QueueP_Elem*)obj;
        elem->prev = obj->prev;
        ((QueueP_Elem*)(obj->prev))->next = (QueueP_Elem*)elem;
        obj->prev = (QueueP_Elem*)elem;

        HwiP_restore(key);
    }

    return QueueP_OK;
}

/*
 *  ======== QueueP_isEmpty ========
 */
QueueP_State QueueP_isEmpty(QueueP_Handle queueHandle)
{
    QueueP_State state = QueueP_EMPTY;
    QueueP_Elem *obj = (QueueP_Elem *) queueHandle;

    if (NULL != obj)
    {
        if(obj->next != (QueueP_Elem*)obj)
        {
            state = QueueP_NOTEMPTY;
        }
    }

    return state;
}

/*
 *  ======== QueueP_getQPtr ========
 */
void * QueueP_getQPtr(QueueP_Handle handle)
{
    /* For QNX, queue pointer is the handle itself */
    return handle;
}

/* Nothing past this point */

