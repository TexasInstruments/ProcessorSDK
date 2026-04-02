/*
 *  Copyright (C) 2013-2023 Texas Instruments Incorporated - http://www.ti.com/
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
 *
 */
 
/**
 *   \file       main.c
 *   \brief      This demonstrates Spinlock functionality by acquiring and releasing different locks.
**/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "stdio.h"
#include "stdint.h"
#include <ti/csl/soc.h>
#include <ti/csl/csl_spinlock.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

#define MAX_NUM_SPINLOCKS       (256U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t spinlockBaseAddr = CSL_NAVSS0_SPINLOCK_BASE;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void SpinlockApp_print(const char * str);
static void SpinlockApp_printNum(uint32_t num);
static uint32_t SpinlockApp_isPrintfSupported(void);
void SpinlockApp_cslTestRunner(void);
int32_t SpinlockApp_lockAcquireTest(void);
int32_t SpinlockApp_lockFreeTest(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t SpinlockApp_lockFreeTest(void)
{
    int32_t  testStatus = CSL_PASS;
    uint32_t prevSpinlockVal = CSL_SPINLOCK_VAL_FREE, status = CSL_PASS, lock_num = 0;

    /* Resetting SpinLock module to ensure all locks are free */
    SPINLOCKModuleReset(spinlockBaseAddr);

    SpinlockApp_print("\nSpinlock test for freeing the lock\n");
    SpinlockApp_print("Case 1: Releasing a taken lock \n");

    /* Acquiring lock before releasing it */
    prevSpinlockVal = SPINLOCKLockStatusSet(spinlockBaseAddr, lock_num);

    /* Releasing taken lock */
    SPINLOCKLockStatusFree(spinlockBaseAddr, lock_num);

    /* To check the current status of the Spinlock, SPINLOCKLockStatusSet is used which does so by trying to acquire the lock */
    /* Checking if lock is free */
    prevSpinlockVal = SPINLOCKLockStatusSet(spinlockBaseAddr, lock_num);

    if (CSL_SPINLOCK_VAL_FREE == prevSpinlockVal)
    {
        SpinlockApp_print("Lock released successfully\n");
    }
    else if (CSL_SPINLOCK_VAL_TAKEN == prevSpinlockVal)
    {
        SpinlockApp_print("Lock not released successfully\n");
        status = CSL_EFAIL;
    }

    if(CSL_PASS == status)
    {
        lock_num = 1;
        SpinlockApp_print("Case 2: Releasing a free lock\n");

        /* Releasing a free lock */
        SPINLOCKLockStatusFree(spinlockBaseAddr, lock_num);

        /* Checking if lock is free */
        prevSpinlockVal = SPINLOCKLockStatusSet(spinlockBaseAddr, lock_num);

        if (CSL_SPINLOCK_VAL_FREE == prevSpinlockVal)
        {
            SpinlockApp_print("Lock released successfully\n");
            SpinlockApp_print("Released locks successfully. Mark test as Pass.\n");
        }
        else if (CSL_SPINLOCK_VAL_TAKEN == prevSpinlockVal)
        {
            SpinlockApp_printNum(lock_num);
            SpinlockApp_print("Lock not released successfully\n");
            SpinlockApp_print("Could not release free lock. Mark test as FAIL.\n");
            testStatus = CSL_EFAIL;
        }
    }
    else
    {
        SpinlockApp_print(" Could not release taken lock. Mark test as FAIL.\n");
        testStatus = CSL_EFAIL;
    }

    return testStatus;
}

int32_t SpinlockApp_lockAcquireTest(void)
{
    int32_t  testStatus = CSL_PASS;
    uint32_t status = 0, lock_num = 0;
    uint32_t numLocAcquired = 0;

    /* Board init */
    Board_initCfg boardCfg;
    boardCfg =  BOARD_INIT_UART_STDIO |
                BOARD_INIT_PINMUX_CONFIG;
    Board_init(boardCfg);

    #if defined (BUILD_MPU1_0)
    char     coreName[10] = {"MPU1_0"};
    #elif defined (BUILD_MCU1_0)
    char     coreName[10] = {"MCU1_0"};
    #elif defined (BUILD_MCU2_1)
    char     coreName[10] = {"MCU2_1"};
    #endif

    SpinlockApp_print("\nOn ");
    SpinlockApp_print(coreName);
    SpinlockApp_print("\nSpinlock App\n");

    for (lock_num = 0; lock_num < MAX_NUM_SPINLOCKS; lock_num++)
    {
        status = SPINLOCKLockStatusSet(spinlockBaseAddr, lock_num);

        if (CSL_SPINLOCK_VAL_FREE == status)
        {
            SpinlockApp_print("\n");
            SpinlockApp_print(coreName);
            SpinlockApp_print(" acquired lock number:");
            SpinlockApp_printNum(lock_num);
            numLocAcquired++;
        }
        else if (CSL_SPINLOCK_VAL_TAKEN == status)
        {
            SpinlockApp_print("\n");
            SpinlockApp_print(coreName);
            SpinlockApp_print(" could not acquire lock number:");
            SpinlockApp_printNum(lock_num);
            SpinlockApp_print(", lock is not free");
        }
    }
    SpinlockApp_print("\n");

    if (numLocAcquired == MAX_NUM_SPINLOCKS)
    {
        SpinlockApp_print("Acquired all available locks. Mark test as Pass.\n");
        SpinlockApp_print("This testcase requires board power cycle.\n");
    }
    else
    {
        SpinlockApp_print("Could not acquire all available locks. Mark test as FAIL.\n");
        SpinlockApp_print("This testcase requires board power cycle.\n");
        testStatus = CSL_EFAIL;
    }

    return testStatus;
}

static void SpinlockApp_print(const char * str)
{
    if (TRUE == SpinlockApp_isPrintfSupported())
    {
        printf("%s", str);
    }
    UART_printf(str);
}

static void SpinlockApp_printNum(uint32_t num)
{
    if (TRUE == SpinlockApp_isPrintfSupported())
    {
        printf("%d", num);
    }
    UART_printf("%d", num);
}

static uint32_t SpinlockApp_isPrintfSupported(void)
{
    uint32_t retVal = TRUE;
    /* Printf doesn't work for MPU when run from SBL with no CCS connection
     * There is no flag to detect SBL or CCS mode. Hence disable the print
     * for MPU unconditionally */
#if defined (BUILD_MPU)
    retVal = FALSE;
#endif
    return retVal;
}

void SpinlockApp_cslTestRunner(void)
{
    /* @description:Test runner for spinlock tests

       @requirements: PDK-2444

       @cores: mcu1_0 */

    int32_t  testStatus = CSL_PASS;

    testStatus += SpinlockApp_lockAcquireTest();
    testStatus += SpinlockApp_lockFreeTest();

    if(CSL_PASS == testStatus)
    {
        SpinlockApp_print("\nAll tests have passed..\n");
    }
    else{
        SpinlockApp_print("\nTest Failed..\n");
    }

    return;
}

int main(void)
{
    (void) SpinlockApp_cslTestRunner();

    while (1)
    {
        /* Do nothing */
    }
}
/********************************* End of file ******************************/

