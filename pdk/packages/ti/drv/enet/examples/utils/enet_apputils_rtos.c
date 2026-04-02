/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/*!
 * \file     enet_apputils_rtos.c
 *
 * \brief    CPSW application utility to get Tasks Stack Status.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#include <ti/drv/enet/enet.h>
#include "include/enet_apputils.h"
#include "include/enet_apputils_rtos.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if 0 //TODO - NEED TO BR PORTED
static void EnetAppUtils_printTaskStackStats(TaskP_Handle hTask)
{
    Task_Stat statBuff;
    uint32_t stackPercent;
    char *taskName = NULL;
    char nameLessTask[20] = "empty-task-name";

    EnetAppUtils_assert(NULL != hTask);

    /* Function get Task status */
    Task_stat(hTask, &statBuff);
    taskName = Task_Handle_name(hTask);
    if (NULL == taskName)
    {
        taskName = nameLessTask;
    }

    if (statBuff.stackSize != 0U)
    {
        stackPercent = ENET_DIV_ROUNDUP(100U * statBuff.used, statBuff.stackSize);
    }
    else
    {
        stackPercent = 0U;
    }

    EnetAppUtils_print("%-30s\t %6d\t\t%6d\t\t%6d\t\t%6d%%\r\n",
                       taskName,
                       statBuff.priority,
                       statBuff.stackSize,
                       statBuff.used,
                       stackPercent);

    if (stackPercent > 90U)
    {
        EnetAppUtils_print("Task stack (%u%%) is about to overflow\n", stackPercent);
    }
}
#endif

void EnetAppUtils_printTaskStackUsage(void)
{
#if 0 //TODO - NEED TO BR PORTED
    Task_Object *task;
    uint32_t i;

    EnetAppUtils_print("==========================================================================="
                       "====================\n");
    EnetAppUtils_print("Task Name\t\t\tPriority\tStack Size\tStack Peak\tPercentage Used \n");
    EnetAppUtils_print("==========================================================================="
                       "====================\n");

    /* Below for loop is to traverse through the list of statically created tasks
     * Task_Object_count() returns the size of the array */
    for (i = 0; i < Task_Object_count(); i++)
    {
        task = Task_Object_get(NULL, i);
        EnetAppUtils_printTaskStackStats(task);
    }

    /* Below while loop is used to traverse the linked list maintaining
     * dynamically created tasks */
    task = Task_Object_first();
    while (NULL != task)
    {
        EnetAppUtils_printTaskStackStats(task);
        task = Task_Object_next(task);
    }
#endif
}

void EnetAppUtils_wait(uint32_t waitTime)
{
    TaskP_sleep(waitTime);
}

