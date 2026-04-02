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

/* global xdc */
let Program = xdc.module('xdc.rov.Program');
let Monitor = xdc.module("xdc.rov.runtime.Monitor");

let moduleName = "FreeRTOS";

/* arranged alphabetically */
let viewMap = [
    {name: "Heap", fxn: "getHeap", structName: "Heap"},
    {name: "Semaphore, Mutex and Queue Instances", fxn: "getQueueInstances", structName: "QueueInstance"},
    {name: "Stack", fxn: "getStack", structName: "Stack"},
    {name: "Task Instances", fxn: "getTaskInstances", structName: "TaskInstance"},
    {name: "Task Module", fxn: "getTaskModule", structName: "TaskModule"},
    {name: "Timer Instances", fxn: "getTimerInstances", structName: "TimerInstance"},
];

function isFREERTOS()
{
    let status = false;

    if( Program.lookupSymbolValue("pxCurrentTCB") >= 0 )
    {
        status = true;
    }

    return status;
}

function isR5F()
{
    let status = false;

    if( Program.lookupSymbolValue("__IRQ_STACK_END") >= 0 )
    {
        status = true;
    }

    return status;
}

function isC66x()
{
    let status = false;

    if( Program.lookupSymbolValue("Hwi_intcVectorTable") >= 0 )
    {
        status = true;
    }

    return status;
}

function isC7x()
{
    let status = false;

    if( Program.lookupSymbolValue("ti_sysbios_family_c7x_Mmu_tableArray") >= 0 )
    {
        status = true;
    }

    return status;
}

function getString(charPtr)
{
    let name = "";
    for (let j = 0; j < 12; j++) {
        if (charPtr[j] == 0) break;
        name = name + String.fromCharCode(charPtr[j]);
    }
    return name;
}

function compareHandle(a, b) {
    let comparison = 0;
    if (a.Handle > b.Handle) {
      comparison = 1;
    } else if (a.Handle < b.Handle) {
      comparison = -1;
    }
    return comparison;
  }

  function compareType(a, b) {
      let comparison = 0;
      if (a.Type > b.Type) {
          comparison = 1;
      } else if (a.Type < b.Type) {
          comparison = -1;
      }
      return comparison;
}

function getStringFromAddress(charPtr, len)
{
    let name = "";
    for (let j = 0; j < len; j++) {
        let stringData = Program.fetchArray(
            {
                type: 'xdc.rov.support.ScalarStructs.S_UChar',
                isScalar: true
            }, charPtr, 1);

        if (stringData[0] == 0) break;
        name = name + String.fromCharCode(stringData[0]);
        charPtr++;
    }
    return name;
}

function Stack()
{
    this.Type = null;
    this.BaseAddress = null;
    this.Size = null;
    this.Description = null;
}

function getStack()
{
    let table = new Array();
    let freertos = isFREERTOS();
    let r5f = isR5F();
    let c66x = isC66x();
    let c7x = isC7x();

    if(r5f)
    {
        let stackInfo = new Stack();

        stackInfo.Type = "IRQ";
        stackInfo.BaseAddress = "0x" + Program.lookupSymbolValue("__IRQ_STACK_END").toString(16)
        stackInfo.Size = Program.lookupSymbolValue("__IRQ_STACK_SIZE");
        if(freertos)
            stackInfo.Description = "Stack used by IRQ for initial IRQ handling before switching to SVC stack";

        table.push(stackInfo);

        stackInfo = new Stack()

        stackInfo.Type = "FIQ";
        stackInfo.BaseAddress = "0x" + Program.lookupSymbolValue("__FIQ_STACK_END").toString(16)
        stackInfo.Size = Program.lookupSymbolValue("__FIQ_STACK_SIZE");

        table.push(stackInfo);

        stackInfo = new Stack();

        stackInfo.Type = "SVC";
        stackInfo.BaseAddress = "0x" + Program.lookupSymbolValue("__SVC_STACK_END").toString(16)
        stackInfo.Size = Program.lookupSymbolValue("__SVC_STACK_SIZE");
        if(freertos)
            stackInfo.Description = "Stack used by SVC handler and also by IRQ handler after initial IRQ handling. User ISR runs within this stack context";

        table.push(stackInfo);

        stackInfo = new Stack()

        stackInfo.Type = "ABORT";
        stackInfo.BaseAddress = "0x" + Program.lookupSymbolValue("__ABORT_STACK_END").toString(16)
        stackInfo.Size = Program.lookupSymbolValue("__ABORT_STACK_SIZE");

        table.push(stackInfo);

        stackInfo = new Stack()

        stackInfo.Type = "UNDEFINED";
        stackInfo.BaseAddress = "0x" + Program.lookupSymbolValue("__UND_STACK_END").toString(16)
        stackInfo.Size = Program.lookupSymbolValue("__UND_STACK_SIZE");

        table.push(stackInfo);

        stackInfo = new Stack()

        stackInfo.Type = "STACK";
        stackInfo.BaseAddress = "0x" + Program.lookupSymbolValue("__STACK_END").toString(16)
        stackInfo.Size = Program.lookupSymbolValue("__STACK_SIZE");
        if(freertos)
            stackInfo.Description = "Stack used by program until FreeRTOS schedular is started in main()";
        else
            stackInfo.Description = "Stack used by non ISR context";
    }
    else
    if(c66x || c7x)
    {
        let stackInfo = new Stack();

        stackInfo.Type = "STACK";
        stackInfo.BaseAddress = "0x" + Program.lookupSymbolValue("__TI_STACK_END").toString(16)
        stackInfo.Size = Program.lookupSymbolValue("__TI_STACK_SIZE");
        if(freertos)
            stackInfo.Description = "Stack used by program until FreeRTOS schedular is started in main() and later stack used by ISR context";
        else
            stackInfo.Description = "Stack used by non ISR and ISR context";
    }

    table.push(stackInfo);

    table.sort(compareType);

    return (table);
}

function Heap()
{
    this.Type        = null;
    this.BaseAddress = null;
    this.Size        = null;
    this.Description = null;
}

function getHeap()
{
    let table = new Array();
    let r5f = isR5F();
    let c66x = isC66x();
    let c7x = isC7x();

    let heapInfo = new Heap();

    heapInfo.Type = "System";
    if(r5f)
    {
        heapInfo.BaseAddress = "0x" + Program.lookupSymbolValue("_sys_memory").toString(16);
        heapInfo.Size = Program.lookupSymbolValue("__SYSMEM_SIZE");
    }
    else
    if(c66x || c7x)
    {
        heapInfo.BaseAddress = "0x" + Program.lookupSymbolValue("_sys_memory").toString(16);
        heapInfo.Size = Program.lookupSymbolValue(" __TI_SYSMEM_SIZE");
    }
    heapInfo.Description = "Heap used by malloc() / pvPortMalloc() / MemoryP ";

    table.push(heapInfo);

    table.sort(compareType);

    return (table);
}

function TaskModule()
{
    this.NumPriorities   = null;
    this.NumTasks        = null;
}

function getTaskModule()
{
    let table = new Array();

    if( ! isFREERTOS() )
    {
        return table;
    }

    let taskInfo = new TaskModule();

    let readyList = Program.fetchVariable("pxReadyTasksLists");

    taskInfo.NumPriorities = readyList.length;
    taskInfo.NumTasks      = Program.fetchVariable("uxCurrentNumberOfTasks");

    table.push(taskInfo);

    return (table);
}

function TaskInstance()
{
    this.Handle          = null;
    this.Name            = null;
    this.Priority        = null;
    this.BasePriority    = null;
    this.State           = null;
    this.StackBase       = null;
    this.EstimatedFreeStackSize  = null;
    this.CurrentStackTop = null;
}

function getTaskFreeStackSize(stackBase, currentTaskSP)
{
    let readSize = 4;
    let skipSize = 128;

    /*
        * We don't know the size of the task stack, so look every n bytes :(
        */
    let stackData = Program.fetchArray(
        {
            type: 'xdc.rov.support.ScalarStructs.S_UChar',
            isScalar: true
        }, stackBase, readSize);

    let index = stackBase;

    /*
        * Find the first non-0xa5.
        */
    while ((stackData[0] == 0xa5) &&
           (stackData[1] == 0xa5) &&
           (stackData[2] == 0xa5) &&
           (stackData[3] == 0xa5) &&
           (index < currentTaskSP)
        ) {
        index += skipSize;
        let stackData = Program.fetchArray(
            {
                type: 'xdc.rov.support.ScalarStructs.S_UChar',
                isScalar: true
            }, index, readSize);
    }

    if (stackBase >= index)
    {
        return "STACK OVERFLOW";
    }
    else {
        return index - stackBase;
    }
}

function fillInTaskInstance(table, list, state)
{
    let currentTask     = Program.fetchVariable("pxCurrentTCB");
    let ptrSize = 4;
    let c7x = isC7x();

    if(c7x)
    {
        ptrSize = 8;
    }

    let tcbBase         = list.xListEnd.pxNext - ptrSize;

    for (let i = 0; i < list.uxNumberOfItems; i++) {
        let task = Program.fetchFromAddr(tcbBase, "TCB_t");

        let taskInfo = new TaskInstance();

        taskInfo.Handle   = "0x" + tcbBase.toString(16);
        taskInfo.Name     = getString(task.pcTaskName);

        taskInfo.Priority     = task.uxPriority;
        taskInfo.BasePriority = task.uxBasePriority;
        if (tcbBase == currentTask) {
            taskInfo.State    = "Running";
        }
        else {
            taskInfo.State    = state;
        }
        taskInfo.StackBase   = task.pxStack;
        taskInfo.CurrentStackTop  = task.pxTopOfStack;

        taskInfo.EstimatedFreeStackSize = getTaskFreeStackSize(taskInfo.StackBase, taskInfo.CurrentStackTop);

        table.push(taskInfo);

        /* Traverse the list */
        tcbBase = task.xStateListItem.pxNext - ptrSize;
    }
}

function getTaskInstances()
{
    let table = new Array();

    if( ! isFREERTOS() )
    {
        return table;
    }

    /* Ready List */
    let readyList = Program.fetchVariable("pxReadyTasksLists");
    for (let i = 0; i < readyList.length; i++) {
        fillInTaskInstance(table, readyList[i], "Ready");
    }

    /* Suspended List */
    let suspendedList = Program.fetchVariable("xSuspendedTaskList");
    fillInTaskInstance(table, suspendedList, "Blocked");

    /* Delay1 List */
    let delay1List = Program.fetchVariable("xDelayedTaskList1");
    fillInTaskInstance(table, delay1List, "Delayed");

    /* Delay2 List */
    let delay2List = Program.fetchVariable("xDelayedTaskList2");
    fillInTaskInstance(table, delay2List, "Delayed");

    /* Terminated List */
    let terminatedList = Program.fetchVariable("xTasksWaitingTermination");
    fillInTaskInstance(table, terminatedList, "Terminated");

    /* Sort by Address so the tasks don't bounce around in ROV */
    table.sort(compareHandle);

    return (table);
}

function QueueInstance()
{
    this.Type = null;
    this.Handle = null;
    this.Name = null;
    this.CurCount = null;
    this.MaxCount = null;
    this.TasksWaitingToRecv = null;
    this.TopWaitingToRecvTaskHandle = null;
    this.TopWaitingToRecvTaskName = null;
    this.TasksWaitingToSend = null;
    this.TopWaitingToSendTaskHandle = null;
    this.TopWaitingToSendTaskName = null;
    this.MutexHolderTaskHandle = null;
    this.MutexHolderTaskName = null;
    this.RecursiveMutexCallCount = null;
    this.QueueElemSize = null;
}

function getQueueType(ucQueueType)
{
    switch(ucQueueType)
    {
        default:
        case 0:
            return "Queue";
        case 1:
            return "Mutex";
        case 2:
            return "Semaphore (Counting)";
        case 3:
            return "Semaphore (Binary)";
        case 4:
            return "Mutex (Recursive)";
    }
}

function getTaskTcbFromQueueList(list)
{
    let listItem = Program.fetchFromAddr(list.xListEnd.pxNext, "ListItem_t");
    let tcb = Program.fetchFromAddr(listItem.pvOwner, "TCB_t");

    return { tcb: tcb, tcbBase: listItem.pvOwner};
}

function getQueueInstances()
{
    let table = new Array();

    if( ! isFREERTOS() )
    {
        return table;
    }

    let xQueueRegistry     = Program.fetchVariable("xQueueRegistry");

    for(let i = 0; i<xQueueRegistry.length; i++)
    {
        if(xQueueRegistry[i].pcQueueName != 0 && xQueueRegistry[i].xHandle != 0 )
        {
            let xHandle = Program.fetchFromAddr(xQueueRegistry[i].xHandle, "xQUEUE_ROV");

            let queue = new QueueInstance();

            queue.Type = getQueueType(xHandle.ucQueueType);
            queue.Name = getStringFromAddress(xQueueRegistry[i].pcQueueName, 16);
            queue.Handle = "0x" + Number(xQueueRegistry[i].xHandle).toString(16);
            queue.CurCount = xHandle.uxMessagesWaiting;
            queue.MaxCount = xHandle.uxLength;
            queue.QueueElemSize = xHandle.uxItemSize;
            queue.TasksWaitingToRecv = xHandle.xTasksWaitingToReceive.uxNumberOfItems;
            queue.TasksWaitingToSend = xHandle.xTasksWaitingToSend.uxNumberOfItems;
            if(queue.TasksWaitingToRecv > 0)
            {
                let task = getTaskTcbFromQueueList(xHandle.xTasksWaitingToReceive);

                queue.TopWaitingToRecvTaskHandle = task.tcbBase;
                queue.TopWaitingToRecvTaskName = getString(task.tcb.pcTaskName);
            }
            if(queue.TasksWaitingToSend > 0)
            {
                let task = getTaskTcbFromQueueList(xHandle.xTasksWaitingToSend);

                queue.TopWaitingToSendTaskHandle = task.tcbBase;
                queue.TopWaitingToSendTaskName = getString(task.tcb.pcTaskName);
            }
            if( (queue.Type=="Mutex" || queue.Type=="Mutex (Recursive)") && xHandle.xSemaphore.xMutexHolder != 0)
            {
                let tcb = Program.fetchFromAddr(xHandle.xSemaphore.xMutexHolder, "TCB_t");

                queue.MutexHolderTaskHandle = "0x" + Number(xHandle.xSemaphore.xMutexHolder).toString(16);
                queue.MutexHolderTaskName = getString(tcb.pcTaskName);
                queue.RecursiveMutexCallCount = xHandle.xSemaphore.uxRecursiveCallCount;
            }

            table.push(queue);
        }
    }

    table.sort(compareType);

    return table;
}

function TimerInstance()
{
    this.Handle = null;
    this.Name = null;
    this.PeriodInTicks = null;
    this.AutoReload = null;
    this.Active = null;
    this.CallbackAddress = null;
    this.TimerID = null;
}

function fillInTimerInstance(table, list)
{
    if(list.uxNumberOfItems > 0)
    {
        let listItem = Program.fetchFromAddr(list.xListEnd.pxNext, "ListItem_t");
        for (let i = 0; i < list.uxNumberOfItems; i++)
        {
            let timer = Program.fetchFromAddr(listItem.pvOwner, "Timer_t");
            let timerInfo = new TimerInstance();

            timerInfo.Handle = listItem.pvOwner;
            timerInfo.Name = getStringFromAddress(timer.pcTimerName, 16);
            timerInfo.PeriodInTicks = timer.xTimerPeriodInTicks;
            timerInfo.AutoReload = "NO";
            timerInfo.Active = "NO";
            if( timer.ucStatus & 0x1 )
                timerInfo.Active = "YES";
            if( timer.ucStatus & 0x4 )
                timerInfo.AutoReload = "YES";
            timerInfo.CallbackAddress = timer.pxCallbackFunction;
            timerInfo.TimerID = timer.pvTimerID;

            table.push(timerInfo);

            if( i < (list.uxNumberOfItems-1))
            {
                /* Traverse the list */
                listItem = Program.fetchFromAddr(listItem.pxNext, "ListItem_t");
            }
        }
    }
}

function getTimerInstances()
{
    let table = new Array();

    if( ! isFREERTOS() )
    {
        return table;
    }

    let activeList1 = Program.fetchVariable("xActiveTimerList1");
    fillInTimerInstance(table, activeList1);

    let activeList2 = Program.fetchVariable("xActiveTimerList2");
    fillInTimerInstance(table, activeList2);

    table.sort(compareHandle);

    return table;
}

