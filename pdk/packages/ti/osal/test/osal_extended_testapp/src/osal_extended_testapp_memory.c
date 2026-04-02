/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

 /**
  *  \file    osal_extended_testapp_memory.c
  *
  *  \brief  OSAL Memory Sub Module testcase file.
  *
  */

 /* ================================================================================== */
 /*                                      Include Files                                 */
 /* ================================================================================== */

 #include "osal_extended_test.h"

 /* ================================================================================== */
 /*                                    Macros & Typedefs                               */
 /* ================================================================================== */

 #define OSAL_APP_MEMORY_ALLOCATION_SIZE      (100U)
 #define OSAL_APP_MEMORY_ALIGNMENT_SIZE       (6U)

 /* ================================================================================== */
 /*                                     Global Variables                               */
 /* ================================================================================== */

 /* None */

 /* ================================================================================== */
 /*                                    Function Declarations                           */
 /* ================================================================================== */
 
/*
* Description  : Test the following:
*                  1. allocate memory for data operations.
*                  2. free previously allocated data memory.
*/
int32_t OsalApp_memoryTests(void);

/* ================================================================================== */
/*                                  Function Definitions                              */
/* ================================================================================== */

int32_t OsalApp_memoryTests(void)
{
    void *memPtr;
    MemoryP_Stats stats;
    int32_t result = osal_OK;

    /* return Pointer to the allocated memory on success or a NULL on an error */ 
    memPtr = MemoryP_dataAlloc(OSAL_APP_MEMORY_ALLOCATION_SIZE , OSAL_APP_MEMORY_ALIGNMENT_SIZE);

    if(NULL == memPtr)
    {
        result = osal_FAILURE;
    }

    if(osal_OK == result)
    {
        MemoryP_getStats(&stats);
        MemoryP_dataFree(memPtr, OSAL_APP_MEMORY_ALLOCATION_SIZE );
        OSAL_log("\n OsalApp_memoryTests have passed! \n");
    }
    else
    {
        OSAL_log("\n OsalApp_memoryTests have failed! \n");
    }

    return result;
}

