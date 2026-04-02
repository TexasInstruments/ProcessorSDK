/*
 * FreeRTOS Kernel V10.4.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */
/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
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
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ti/osal/HwiP.h>
#include <ti/osal/TimerP.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/CycleprofilerP.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/r5/csl_arm_r5_pmu.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/osal/src/nonos/Nonos_config.h>
#if defined (SOC_J721S2)
#include <ti/kernel/freertos/config/j721s2/r5f/FreeRTOSConfig.h>
#elif defined (SOC_J721E)
#include <ti/kernel/freertos/config/j721e/r5f/FreeRTOSConfig.h>
#elif defined (SOC_J7200)
#include <ti/kernel/freertos/config/j7200/r5f/FreeRTOSConfig.h>
#elif defined (SOC_J784S4)
#include <ti/kernel/freertos/config/j784s4/r5f/FreeRTOSConfig.h>
#endif

/* Let the user override the pre-loading of the initial LR with the address of
 * prvTaskExitError() in case is messes up unwinding of the stack in the
 * debugger. */
#ifdef configTASK_RETURN_ADDRESS
    #define portTASK_RETURN_ADDRESS    configTASK_RETURN_ADDRESS
#else
    #define portTASK_RETURN_ADDRESS    prvTaskExitError
#endif

/* Constants required to setup the initial task context. */
#define portINITIAL_SPSR                 ( ( StackType_t ) 0x1F ) /* System mode, ARM mode, IRQ enabled FIQ enabled. */
#define portTHUMB_MODE_BIT               ( ( StackType_t ) 0x20 )
#define portINTERRUPT_ENABLE_BIT         ( 0x80UL )
#define portTHUMB_MODE_ADDRESS           ( 0x01UL )
#define portINITIAL_FPSR                 ( ( StackType_t ) 0x0 )
#define portNUM_FPU_REGS                 ( 32U )

/* A critical section is exited when the critical section nesting count reaches
 * this value. */
#define portNO_CRITICAL_NESTING          ( ( uint32_t ) 0 )

/* Tasks are not created with a floating point context, but can be given a
 * floating point context after they have been created.  A variable is stored as
 * part of the tasks context that holds portNO_FLOATING_POINT_CONTEXT if the task
 * does not have an FPU context, or any other value if the task does have an FPU
 * context. */
#define portNO_FLOATING_POINT_CONTEXT    ( ( StackType_t ) 0 )

/* Tasks are created with a floating point context. A variable is stored as
 * part of the tasks context that holds portFLOATING_POINT_CONTEXT if the task
 * have an FPU context. */
#define portFLOATING_POINT_CONTEXT       ( ( StackType_t ) 1 )

/* A variable is used to keep track of the critical section nesting.  This
 * variable has to be stored as part of the task context and must be initialised to
 * a non zero value to ensure interrupts don't inadvertently become unmasked before
 * the scheduler starts.  As it is stored as part of the task context it will
 * automatically be set to 0 when the first task is started. */
volatile uint32_t ulCriticalNesting = 9999U;

/* Saved as part of the task context.  If ulPortTaskHasFPUContext is non-zero then
 * a floating point context must be saved and restored for the task. */
BaseType_t ulPortTaskHasFPUContext = pdFALSE;

/* Set to 1 to pend a context switch from an ISR. */
BaseType_t ulPortYieldRequired = pdFALSE;

/* Counts the interrupt nesting depth.  A context switch is only performed if
 * if the nesting depth is 0. */
BaseType_t ulPortInterruptNesting = pdFALSE;

/* set to true when schedular gets enabled in xPortStartScheduler */
BaseType_t ulPortSchedularRunning = pdFALSE;

/* Store the PMU counter timestamp during an OS tick.
 * This is required to calculate the time in microseconds for 
 * uiPortGetRunTimeCounterValue. */
static volatile uint64_t ullPortLastTickPmuTs;

/*  PMU counter timestamp overflow count. */
static uint32_t ulPmuTsOverFlowCount = 0U;

/* Faulty Stack Pointer at Data Abort. */
uint32_t FaultySP;

/* Faulty Link Register at Data Abort. */
uint32_t FaultyLR;

/* Faulty General Purpose Registers at Data Abort. */
uint32_t FaultyGPR[13];

/*
 * Run Time Timer Control Structure. 
 */
typedef struct xRUN_TIME_TIMER_CONTROL 
{
    uint64_t        uxTicks;
    TimerP_Handle   pxTimerHandle;
    uint32_t        ulUSecPerTick;
} RunTimeTimerControl_t;

RunTimeTimerControl_t   gTimerCntrl;

/*
 * Starts the first task executing.  This function is necessarily written in
 * assembly code so is implemented in portASM.s.
 */
extern void vPortRestoreTaskContext( void );

extern uint32_t ulGetDataFaultStatusRegister( void );
extern uint32_t ulGetDataFaultAddressRegister( void );
extern uint32_t ulGetInstructionFaultStatusRegister( void );
extern uint32_t ulGetInstructionFaultAddressRegister( void );
extern uint32_t ulGetCPSR( void );
extern uint32_t ulDoWFI( void );
extern uint32_t ulDoCPSID( void );
extern uint32_t ulDoCPSIE( void );

void vPortDumpExceptionState( void );
uint64_t getRunTimeCounterValue(void);
uint64_t getRunTimeCounterValue64(void);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);
int32_t _system_pre_init(void);
void _system_post_cinit(void);
void vApplicationIdleHook(void);

BaseType_t xPortInIsrContext(void);

uint64_t uxPortReadPmuCounter(void);

uint64_t uiPortGetRunTimeCounterValue64(void);

static void prvTaskExitError( void )
{
    /* A function that implements a task must not exit or attempt to return to
     * its caller as there is nothing to return to.  If a task wants to exit it
     * should instead call vTaskDelete( NULL ).
     *
     * Force an assert() to be triggered if configASSERT() is
     * defined, then stop here so application writers can catch the error. */
    DebugP_assert(BFALSE);
}

StackType_t * pxPortInitialiseStack( StackType_t * pxTopOfStack,
                                     TaskFunction_t pxCode,
                                     void * pvParameters )
{
    StackType_t *pxTOS = pxTopOfStack;

    /* Setup the initial stack of the task.  The stack is set exactly as
     * expected by the portRESTORE_CONTEXT() macro.
     *
     * The fist real value on the stack is the status register, which is set for
     * system mode, with interrupts enabled.  A few NULLs are added first to ensure
     * GDB does not try decoding a non-existent return address. */
    *pxTOS = ( StackType_t ) NULL;
    pxTOS--;
    *pxTOS = ( StackType_t ) NULL;
    pxTOS--;
    *pxTOS = ( StackType_t ) NULL;
    pxTOS--;
    *pxTOS = ( StackType_t ) portINITIAL_SPSR;

    if( 0x00UL != ( ( uint32_t ) pxCode & portTHUMB_MODE_ADDRESS ) )
    {
        /* The task will start in THUMB mode. */
        *pxTOS |= portTHUMB_MODE_BIT;
    }

    pxTOS--;

    /* Next the return address, which in this case is the start of the task. */
    *pxTOS = ( StackType_t ) pxCode;
    pxTOS--;

    /* Next all the registers other than the stack pointer. */
    *pxTOS = ( StackType_t ) portTASK_RETURN_ADDRESS; /* R14 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x12121212;              /* R12 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x11111111;              /* R11 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x10101010;              /* R10 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x09090909;              /* R9 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x08080808;              /* R8 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x07070707;              /* R7 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x06060606;              /* R6 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x05050505;              /* R5 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x04040404;              /* R4 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x03030303;              /* R3 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x02020202;              /* R2 */
    pxTOS--;
    *pxTOS = ( StackType_t ) 0x01010101;              /* R1 */
    pxTOS--;
    *pxTOS = ( StackType_t ) pvParameters;            /* R0 */
    pxTOS--;

    /* The task will start with a critical nesting count of 0 as interrupts are
     * enabled. */
    *pxTOS = portNO_CRITICAL_NESTING;
    pxTOS--;

#if (configFLOATING_POINT_CONTEXT==0)
    /* The task will start without a floating point context.  A task that uses
     * the floating point hardware must call vPortTaskUsesFPU() before executing
     * any floating point instructions. */
    *pxTOS = portNO_FLOATING_POINT_CONTEXT;
#else
    /* The task will start with a floating point context. */

    *pxTOS = portINITIAL_FPSR;
    pxTOS--;

    /* Next all the FPU bank registers S0 to S31 */
    uint32_t ulNumFpuReg = portNUM_FPU_REGS;
    while (ulNumFpuReg > 0U)
    {
        ulNumFpuReg--;
        *pxTOS = ( StackType_t ) 0x00000000;     /* S0 to S31 */
        pxTOS--;
    }

    *pxTOS = portFLOATING_POINT_CONTEXT;
#endif

    return pxTOS;
}

static void prvPorttimerTickIsr(uintptr_t args)
{
    void vPortTimerTickHandler(void);

    /* increment the systick counter */
    gTimerCntrl.uxTicks++;
    /* Store the PMU counter timestamp  */
    ullPortLastTickPmuTs = uxPortReadPmuCounter();

    vPortTimerTickHandler();
}

static void prvPortInitTickTimer(void)
{

    TimerP_Params timerParams;
    TimerP_Handle pTickTimerHandle;

    TimerP_Params_init(&timerParams);
    timerParams.runMode    = TimerP_RunMode_CONTINUOUS;
    timerParams.startMode  = TimerP_StartMode_USER;
    timerParams.periodType = TimerP_PeriodType_MICROSECS;
    timerParams.period     = (portTICK_PERIOD_MS * 1000U);

    pTickTimerHandle = TimerP_create(configTIMER_ID, &prvPorttimerTickIsr, &timerParams);

    /* don't expect the handle to be null */
    DebugP_assert (NULL != pTickTimerHandle);

    /* init internal data structure */
    gTimerCntrl.uxTicks             = 0U;
    gTimerCntrl.ulUSecPerTick       = (portTICK_PERIOD_MS * 1000U);
    gTimerCntrl.pxTimerHandle       = pTickTimerHandle;
}

static void prvPortStartTickTimer(void)
{
    TimerP_Status status;
    status = TimerP_start(gTimerCntrl.pxTimerHandle);

    /* don't expect the handle to be null */
    DebugP_assert (status == TimerP_OK);

}

BaseType_t xPortStartScheduler(void)
{
    /* Interrupts are turned off in the CPU itself to ensure tick does
     * not execute	while the scheduler is being started.  Interrupts are
     * automatically turned back on in the CPU when the first task starts
     * executing.
     */
    portDISABLE_INTERRUPTS();

    #if (configCOPY_RESET_VECTORS==1)
    /* Relocate FreeRTOS Reset Vectors to ATCM*/
    void _freertosresetvectors (void);  
    memcpy((void *)configMCU_ATCM_BASE, (void *)_freertosresetvectors, 0x40);
    #endif

    /* Start the ISR handling of the timer that generates the tick ISR. */
    ulPortSchedularRunning = pdTRUE;

    prvPortInitTickTimer();
    prvPortStartTickTimer();
    /* Start the first task executing. */
    vPortRestoreTaskContext();

    /* Will only get here if vTaskStartScheduler() was called with the CPU in
     * a non-privileged mode or the binary point register was not set to its lowest
     * possible value.  prvTaskExitError() is referenced to prevent a compiler
     * warning about it being defined but not referenced in the case that the user
     * defines their own exit address. */
    ( void ) prvTaskExitError;

    return pdTRUE;
}

void vPortYeildFromISR( uint32_t xSwitchRequired )
{
    if( UFALSE != xSwitchRequired )
    {
        ulPortYieldRequired = pdTRUE;
    }
}

void vPortTimerTickHandler(void)
{
    if( pdTRUE == ulPortSchedularRunning )
    {
        /* Increment the RTOS tick. */
        if( pdFALSE != xTaskIncrementTick() )
        {
            ulPortYieldRequired = pdTRUE;
        }
    }
}

void vPortTaskUsesFPU( void )
{
#if (configFLOATING_POINT_CONTEXT==0)
    uint32_t ulInitialFPSCR = 0U;

    /* A task is registering the fact that it needs an FPU context.  Set the
     * FPU flag (which is saved as part of the task context). */
    ulPortTaskHasFPUContext = pdTRUE;

    /* Initialise the floating point status register. */
    __asm__ volatile ( "FMXR 	FPSCR, %0" ::"r" ( ulInitialFPSCR ) : "memory" );
#endif
}

void vPortEnterCritical( void )
{
    /* Mask interrupts up to the max syscall interrupt priority. */
    ulDoCPSID();

    /* Now interrupts are disabled ulCriticalNesting can be accessed
     * directly.  Increment ulCriticalNesting to keep a count of how many times
     * portENTER_CRITICAL() has been called. */
    ulCriticalNesting = ulCriticalNesting + 1U;

    #if (configOPTIMIZE_FOR_LATENCY==0)
    /* This API should NOT be called from within ISR context. Below logic checks for this.
     * Commenting this reduces task switch latency a bit, however if this API is by mistale
     * called in a ISR by user, it could have unexpected side effects.
     */
    /* This is not the interrupt safe version of the enter critical function so
     * assert() if it is being called from an interrupt context.  Only API
     * functions that end in "FromISR" can be used in an interrupt.  Only assert if
     * the critical nesting count is 1 to protect against recursive calls if the
     * assert function also uses a critical section. */
    if( 1U == ulCriticalNesting )
    {
        DebugP_assert( pdFALSE == ulPortInterruptNesting );
    }
    #endif
}

void vPortExitCritical( void )
{
    /* Make sure critical nesting count is non zero */
    DebugP_assert( ulCriticalNesting > portNO_CRITICAL_NESTING );

    {
        /* Decrement the nesting count as the critical section is being
         * exited. */
        ulCriticalNesting = ulCriticalNesting - 1U;

        /* If the nesting level has reached zero then all interrupt
         * priorities must be re-enabled. */
        if( ulCriticalNesting == portNO_CRITICAL_NESTING )
        {
            /* Critical nesting has reached zero so all interrupt priorities
             * should be unmasked. */
            ulDoCPSIE();
        }
    }
}

void vPortDumpExceptionState( void )
{
    volatile uint32_t DFSR, DFAR, IFSR, IFAR, CPSR;
    DFSR = ulGetDataFaultStatusRegister();
    DFAR = ulGetDataFaultAddressRegister();
    IFSR = ulGetInstructionFaultStatusRegister();
    IFAR = ulGetInstructionFaultAddressRegister();
    CPSR = ulGetCPSR();
    DebugP_exceptionLog("[FATAL]: Core has Aborted!!!\nDFAR =0x%x DFSR =0x%x\n", (uintptr_t)DFAR, (uintptr_t)DFSR);
    DebugP_exceptionLog("IFAR =0x%x IFSR =0x%x\n", (uintptr_t)IFAR, (uintptr_t)IFSR);
    DebugP_exceptionLog("CPSR =0x%x SP =0x%x\n", (uintptr_t)CPSR, (uintptr_t)FaultySP);
    DebugP_exceptionLog("LR =0x%x R0 =0x%x\n", (uintptr_t)FaultyLR, (uintptr_t)FaultyGPR[0]);
    DebugP_exceptionLog("R1 =0x%x R2 =0x%x\n", (uintptr_t)FaultyGPR[1], (uintptr_t)FaultyGPR[2]);
    DebugP_exceptionLog("R3 =0x%x R4 =0x%x\n", (uintptr_t)FaultyGPR[3], (uintptr_t)FaultyGPR[4]);
    DebugP_exceptionLog("R5 =0x%x R6 =0x%x\n", (uintptr_t)FaultyGPR[5], (uintptr_t)FaultyGPR[6]);
    DebugP_exceptionLog("R7 =0x%x R8 =0x%x\n", (uintptr_t)FaultyGPR[7], (uintptr_t)FaultyGPR[8]);
    DebugP_exceptionLog("R9 =0x%x R10 =0x%x\n", (uintptr_t)FaultyGPR[9], (uintptr_t)FaultyGPR[10]);
    DebugP_exceptionLog("R11 =0x%x R12 =0x%x\n", (uintptr_t)FaultyGPR[11], (uintptr_t)FaultyGPR[12]);
}

/* initialize high resolution timer for CPU and task load calculation */
void vPortConfigTimerForRunTimeStats(void)
{
    /* Timer is initialized by prvPortInitTickTimer before the schedular is started */

    /* Configure and initialize PMU Counter for calculating micro seconds */
    CSL_armR5PmuEnableAllCntrs(1);    /* Set/clear PMCR E-bit */
    CSL_armR5PmuResetCntrs();         /* Set PMCR P-bit */
    CSL_armR5PmuResetCycleCnt();      /* Set PMCR C-bit */
    CSL_armR5PmuEnableCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, 1);     /* Set PMCNTENSET for event */
    CSL_armR5PmuClearCntrOverflowStatus(0x80000007U);
    ulPmuTsOverFlowCount = 0U;
}

uint64_t uxPortReadPmuCounter(void)
{
    uint32_t tsLo;
    uint32_t ovsrStatus;
    uint64_t ts;

    tsLo = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);

    if( pdTRUE == xPortInIsrContext() )
    {
        ovsrStatus = (CSL_armR5PmuReadCntrOverflowStatus() & (0x1U << CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM));

        if (0U != ovsrStatus)
        {
            tsLo = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
            ulPmuTsOverFlowCount++;
            CSL_armR5PmuClearCntrOverflowStatus((0x1U << CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM));
        }
    }
    ts = ((uint64_t)tsLo | ((uint64_t) ulPmuTsOverFlowCount << 32U));

    return ts;
}
/* Return current counter value of high speed counter in usecs, or return 0 in case of an unexpected error. */
uint64_t getRunTimeCounterValue(void)
{
    uint64_t uxDeltaTs, uxTimeInUsecs;
    volatile uint64_t uxTimeInMilliSecs, t1, t2, pmuCounterRead, pmuCounterReadHi, pmuCounterReadLow;
    uint32_t noBusyWaiting = 2U;
    bool cond1, cond2;
    /* If there is a tick in between reading the micro seconds from last tick, then the differenfece will get corrupted.
     * We should keep looping to ensure that no tick happened during the miscrosecond offset measurement.
     * Worst case expectation is that the loop will execute 2 times, as the tick interrupt occurs in 
     * magnitude of 1ms and the below instructions are executed on a 1GHz processor.
     */
    do
    {
        noBusyWaiting = noBusyWaiting - 1U;
        t1 = gTimerCntrl.uxTicks;
        /* PMU counter increments after last OS tick  */
        pmuCounterRead = uxPortReadPmuCounter();

        /* PMU Overflow is only handled in the tick timer. The below check
         * is to handle the cases when the PMU overflows and the tick has not happened yet.
         */
        if (pmuCounterRead < ullPortLastTickPmuTs)
        {
            pmuCounterReadHi = pmuCounterRead >> 32U;
            pmuCounterReadLow = pmuCounterRead & 0xFFFFFFFFU;
            /* Increase the higher 32 bits by 1, as overflow has happened. Do not handle the overflow
             * as the tick timer will handle it. Just use the correct value here.
             */
            pmuCounterReadHi = pmuCounterReadHi + 1ULL;
            pmuCounterRead =  (pmuCounterReadHi << 32U) | pmuCounterReadLow;
        }
        uxDeltaTs = pmuCounterRead - ullPortLastTickPmuTs;
        t2 = gTimerCntrl.uxTicks;
        cond1 = (t1 != t2);
        cond2 = (0U != noBusyWaiting);
    } while (cond1 && cond2);

    /* If t1 and t2 are not equal after 2 iterations of the while loop, then this is 
     * as unexpected situation and we should return an error, i.e., 0
     */
    if ( ( 0U == noBusyWaiting ) && cond1 )
    {
        uxTimeInUsecs = 0ULL;
    }
    else
    {
        /* time in milliseconds based on no. of OS ticks */
        uxTimeInMilliSecs = t2 * (uint64_t)portTICK_PERIOD_MS;

        uxTimeInUsecs = ((uxTimeInMilliSecs * 1000ULL) + 
                            ((uxDeltaTs * 1000000ULL) / (uint64_t)configCPU_CLOCK_HZ)); /* convert PMU timestamp to microseconds */

        /* note, there is no overflow protection for this 32b value in FreeRTOS
        *
        * This value will overflow in
        * ((0xFFFFFFFF)/(1000000*60)) minutes ~ 71 minutes
        *
        * We call LoadP_update() in idle loop (from vApplicationIdleHook) to accumulate the task load into a 64b value.
        * The implementation of LoadP_update() is in osal/src/freertos/LoadP_freertos.c
        * 
        */
    }
    return (uxTimeInUsecs);
}

/* Return current counter value of high speed counter in usecs as uint32_t, or return 0 in case of an unexpected error. */
configRUN_TIME_COUNTER_TYPE uiPortGetRunTimeCounterValue(void)
{
    return (configRUN_TIME_COUNTER_TYPE)getRunTimeCounterValue();
}

/* Return current counter value of high speed counter in usecs as uint64_t, or return 0 in case of an unexpected error. */
uint64_t uiPortGetRunTimeCounterValue64(void)
{
    return getRunTimeCounterValue();
}

/* This is used to make sure we are using the FreeRTOS API from within a valid interrupt priority level
 * In our R5F port this means IRQ.
 * i.e FreeRTOS API should not be called from FIQ, however right now we dont enforce it by checking
 * if we are in FIQ when this API is called.
 */
void vPortValidateInterruptPriority(void)
{
}

/* This is called as part of vTaskEndScheduler(), in our port, there is nothing to do here.
 * interrupt are disabled by FreeRTOS before calling this.
 */
void vPortEndScheduler(void)
{
    /* nothing to do */
}

/* configCHECK_FOR_STACK_OVERFLOW is set to 1, so the application must provide an
 * implementation of vApplicationStackOverflowHook()
 */
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char * pcTaskName )
{
    DebugP_log1("[FreeRTOS] Stack overflow detected for task [%s]", (uintptr_t)pcTaskName);
    DebugP_assert(BFALSE);
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task.
 */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE * puxIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static – otherwise they will be allocated on
 * the stack and so not exists after this function exits.
 */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
     * state will be stored.
     */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task’s stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes.
     */
    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task.
 */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static – otherwise they will be allocated on
 * the stack and so not exists after this function exits.
 */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task’s state will be stored.
     */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task’s stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes.
     */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/*
 * Returns true if the current core is in ISR context; low prio ISR, med prio ISR or timer tick ISR. High prio ISRs
 * aren't detected here, but they normally cannot call C code, so that should not be an issue anyway.
 */
BaseType_t xPortInIsrContext(void)
{
    BaseType_t inISR = pdFALSE;
    if (pdFALSE != ulPortInterruptNesting)
    {
        inISR =  pdTRUE;
    }
    return inISR;
}

/*****************************************************************************/
/* _SYSTEM_PRE_INIT() - _system_pre_init() is called in the C/C++ startup    */
/* routine (_c_int00()) and provides a mechanism for the user to             */
/* insert application specific low level initialization instructions prior   */
/* to calling main().  The return value of _system_pre_init() is used to     */
/* determine whether or not C/C++ global data initialization will be         */
/* performed (return value of 0 to bypass C/C++ auto-initialization).        */
/*                                                                           */
/* PLEASE NOTE THAT BYPASSING THE C/C++ AUTO-INITIALIZATION ROUTINE MAY      */
/* RESULT IN PROGRAM FAILURE.                                                */
/*                                                                           */
/* The version of _system_pre_init() below is skeletal and is provided to    */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/

int32_t _system_pre_init(void)
{
    extended_system_pre_init();
    return 1;
}

/*****************************************************************************/
/* _SYSTEM_POST_CINIT() - _system_post_cinit() is a hook function called in  */
/* the C/C++ auto-initialization function after cinit() and before pinit().  */
/*                                                                           */
/* The version of _system_post_cinit() below is skeletal and is provided to  */
/* illustrate the interface and provide default behavior.  To replace this   */
/* version rewrite the routine and include it as part of the current project.*/
/* The linker will include the updated version if it is linked in prior to   */
/* linking with the C/C++ runtime library.                                   */
/*****************************************************************************/

/*---------------------------------------------------------------------------*/
/* __TI_default_system_post_cinit indicates that the default                 */

__attribute__((weak)) void _system_post_cinit(void)
{
    osalArch_Config_t cfg;

    cfg.disableIrqOnInit = BTRUE;
    osalArch_Init(&cfg);
    extended_system_post_cinit();
}

/* This function is called when configUSE_IDLE_HOOK is 1 in FreeRTOSConfig.h */
__attribute__((weak)) void vApplicationIdleHook( void )
{
#if (configLOAD_UPDATE_IN_IDLE==1)
    void vApplicationLoadHook(void);

    vApplicationLoadHook();
#endif

    ulDoWFI();
}

