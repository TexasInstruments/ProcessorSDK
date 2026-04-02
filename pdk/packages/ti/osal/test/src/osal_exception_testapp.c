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
 *  \file   osal_exception_testapp.c
 *
 *  \brief  OSAL driver test application main file.
 *
 */
  
/*===========================================================================*/
/*                             Includes                                      */
/*===========================================================================*/

#include <ti/board/board.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/osal.h>
#include <stdint.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/uart/UART_stdio.h>
#include "OSAL_log.h"

#if defined(BUILD_C7X)
#include <ti/csl/arch/c7x/Exception.h>
#endif

/*===========================================================================*/
/*                             Macros & Typedefs                             */
/*===========================================================================*/

#define APP_TSK_STACK_MAIN              (32U * 1024U)

/* ==========================================================================*/
/*                            Global Variables                               */
/* ==========================================================================*/

#if defined (BUILD_MCU)
extern const CSL_ArmR5MpuRegionCfg __attribute__((section(".startupData"))) gCslR5MpuCfg[CSL_ARM_R5F_MPU_REGIONS_MAX] =
{
    {
        /* Region 0 configuration: complete 32 bit address space = 4Gbits */
        .regionId         = 0U,
        .enable           = UTRUE,
        .baseAddr         = 0x0U,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_4GB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 1U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UFALSE,
        .cachePolicy      = 0U,
        .memAttr          = 0U,
    },
    {
        /* Region 1 configuration: 32 KB ATCM */
        .regionId         = 6U,
        .enable           = UTRUE,
        .baseAddr         = 0x0,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_32KB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_NON_CACHEABLE,
        .memAttr          = 0U,
    },
    {
        /* Region 2 configuration: 1MB KB MCU MSRAM */
        .regionId         = 2U,
        .enable           = UTRUE,
        .baseAddr         = 0x41C00000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_1MB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
    {
        /* Region 3 configuration: 2 GB DDR RAM */
        .regionId         = 4U,
        .enable           = UTRUE,
        .baseAddr         = 0x80000000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_2GB,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UTRUE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U,
    },
	{
        /* Region 4 configuration: 2 GB DDR RAM */
        .regionId         = 5U,
        .enable           = UTRUE,
        .baseAddr         = 0xFFFFF000,
        .size             = CSL_ARM_R5_MPU_REGION_SIZE_128B,
        .subRegionEnable  = CSL_ARM_R5_MPU_SUB_REGION_ENABLE_ALL,
        .exeNeverControl  = 0U,
        .accessPermission = CSL_ARM_R5_ACC_PERM_PRIV_USR_RD_WR,
        .shareable        = UFALSE,
        .cacheable        = UFALSE,
        .cachePolicy      = CSL_ARM_R5_CACHE_POLICY_WB_WA,
        .memAttr          = 0U
    },
    
};
#endif
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__(( aligned( APP_TSK_STACK_MAIN ))) = { 0 };

uint32_t gExceptionHookCount = 0U;

/* ==========================================================================*/
/*                         Structure Declarations                            */
/* ==========================================================================*/

/* None */

/* ==========================================================================*/
/*                          Internal Function Declarations                   */
/* ==========================================================================*/

/*
 * Description  : Test the below debug Exception APIs :
 *                 1. DebugP_registerExcptnLogFxn()
 *                 2. DebugP_deRegisterExcptnLogFxn()
 *                 3. DebugP_exceptionLog()
 */
static void OsalApp_debugNegativeTest(void);

/* Description  : BoardInit for OSAL */
static Board_STATUS OsalApp_boardInit(void);

/* Description  : Genearting the OSAL Exception*/
static void OsalApp_generateException(void); 

/* ==========================================================================*/
/*                      Internal Function Definitions                        */
/* ==========================================================================*/

static Board_STATUS OsalApp_boardInit(void)
{
    return Board_init(BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO);
}

void OsalApp_defaultExceptionHook(void)
{
    gExceptionHookCount++;
}

void OsalApp_returnHook(void)
{
    gExceptionHookCount++;
    DebugP_exceptionLog(" \n Exception Hook count is %x  \n", gExceptionHookCount, 0U);
    while (1U) {
    }
}

static void OsalApp_generateException(void)
{
    volatile uint64_t *myBadAddr;
#if defined (BUILD_MCU)
    myBadAddr = (uint64_t *)(0xFFFFFF003);
#elif defined (BUILD_C7X)
    Exception_Hooks exceptionHooks;
    exceptionHooks.exceptionHook = &OsalApp_defaultExceptionHook;
    exceptionHooks.internalHook = &OsalApp_defaultExceptionHook;
    exceptionHooks.returnHook = &OsalApp_returnHook;
    Exception_registerHooks(&exceptionHooks);
    myBadAddr = (uint64_t*)(0xC000B000);
#endif
    *myBadAddr = (0xDEADFADE);
}

static void OsalApp_debugNegativeTest(void)
{
    int32_t retval = DEBUGP_LOGFXN_ALREADY_REGISTERD;

    /* Checking the exception log before registering the exception */
    DebugP_exceptionLog(" \n ExceptionLog before Registering  \n", 0U, 0U);

    /* Registration of OSAL_log function */
    retval = DebugP_registerExcptnLogFxn(OSAL_log);
    if(DEBUGP_LOGFXN_ALREADY_REGISTERD == retval)
    {
        OSAL_log("\n Registration of Exception log function failed!!\n");
    }
    else
    {
        /* Checking the exception log after registering the exception */
        DebugP_exceptionLog("\n ExceptionLog after Registering !\n", 0U, 0U);
    }

    /* Registering twice without de-registarting the exception to check the DEBUGP_LOGFXN_ALREADY_REGISTERD condition */
    retval = DebugP_registerExcptnLogFxn(OSAL_log);
    if(DEBUGP_LOGFXN_ALREADY_REGISTERD == retval)
    {
        OSAL_log("\n DebugP Negative test for exception passed!\n");
    }
    else
    {
        OSAL_log("\n DebugP Negative test for exception failed!\n");
    }

    /* Debug log function de-registration */
    DebugP_deRegisterExcptnLogFxn();
}

void osal_test(void *arg0, void *arg1)
{
    int32_t status = CSL_PASS;

    if(BOARD_SOK != OsalApp_boardInit())
    {
        status = CSL_EFAIL;
    }

    OsalApp_debugNegativeTest();

    if(CSL_PASS == status)
    {
        DebugP_registerExcptnLogFxn(OSAL_log);
        
#if defined LDRA_DYN_COVERAGE_EXIT
        OSAL_log("\n LDRA ENTRY... \n");
        upload_execution_history();
        OSAL_log("\n LDRA EXIT... \n");
#endif
        OSAL_log("\n All tests have passed. \n");
        OsalApp_generateException();
    }
}


/*
 *  ======== main ========
 */
int main(void)
{
    TaskP_Params taskParams;

    OS_init();
    TaskP_Params_init(&taskParams);
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&osal_test, &taskParams);
    OS_start();

    return (0);
}

#if defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

