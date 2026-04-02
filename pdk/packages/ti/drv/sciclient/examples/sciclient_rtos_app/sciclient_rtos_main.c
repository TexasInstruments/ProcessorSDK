/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *  \file  sciclient_rtos_main.c
 *
 *  \brief Implementation of tests for RTOS
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/drv/sciclient/sciclient.h>
#include <ti/drv/sciclient/examples/common/sci_app_common.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Test application stack size */
#if defined (BUILD_C7X)
/* Temp workaround to avoid assertion failure: A_stackSizeTooSmall : Task stack size must be >= 32KB.
 * until the Bug PDK-7605 is resolved */
#define SCICLIENT_APP_TSK_STACK_MAIN (32U * 1024U)
#else
#define SCICLIENT_APP_TSK_STACK_MAIN (8U * 1024U)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(SAFERTOS)
/* Test application stack */
static uint8_t gSciclientAppTskStackMain[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(SCICLIENT_APP_TSK_STACK_MAIN)));

/* Test application stack */
static uint8_t gSciclientAppTskStackRevTest1[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(SCICLIENT_APP_TSK_STACK_MAIN)));
static uint8_t gSciclientAppTskStackRevTest2[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(SCICLIENT_APP_TSK_STACK_MAIN)));
#else
/* Test application stack */
static uint8_t gSciclientAppTskStackMain[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(32)));

/* Test application stack */
static uint8_t gSciclientAppTskStackRevTest1[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
static uint8_t gSciclientAppTskStackRevTest2[SCICLIENT_APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
#endif

int32_t gTsk1Pass = CSL_EFAIL;
int32_t gTsk2Pass = CSL_EFAIL;

SemaphoreP_Handle gSemTest1;
SemaphoreP_Handle gSemTest2;

/* ========================================================================== */
/*                         Structures Declarations                            */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Function Declarations                              */
/* ========================================================================== */

void appReset(void);

/* ========================================================================== */
/*                         Internal Function Declarations                     */
/* ========================================================================== */

static void mainTsk(void* arg0, void* arg1);
static void SciclientApp_getRevisionTest1(void* arg0, void *arg1);
static void SciclientApp_getRevisionTest2(void* arg0, void *arg1);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle taskHdl;
    TaskP_Params taskParams;
    int32_t retVal = CSL_PASS;

    OS_init();

    TaskP_Params_init(&taskParams);
    taskParams.priority  = 14;
    taskParams.stack     = gSciclientAppTskStackMain;
    taskParams.stacksize = sizeof(gSciclientAppTskStackMain);

    taskHdl = TaskP_create(&mainTsk, &taskParams);
    if (taskHdl == NULL)
    {
        SciApp_printf("Task_create() mainTsk failed!\n");
        OS_stop();
    }

    /* Start BIOS */
    OS_start();

    return retVal;
}

void appReset(void)
{
  
}

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

static void mainTsk(void* arg0, void* arg1)
{
    TaskP_Handle task1;
    TaskP_Handle task2;
    TaskP_Params taskParams1;
    TaskP_Params taskParams2;
    SemaphoreP_Params params;

    SemaphoreP_Params_init(&params);
    params.mode = SemaphoreP_Mode_BINARY;
    gSemTest1   = SemaphoreP_create(0U, &params);
    if (NULL == gSemTest1)
    {
        SciApp_printf("SemaphoreP_create() gSemTest1 Failed \n");
    }

    SemaphoreP_Params_init(&params);
    params.mode = SemaphoreP_Mode_BINARY;
    gSemTest2   = SemaphoreP_create(0U, &params);
    if (NULL == gSemTest2)
    {
        SciApp_printf("SemaphoreP_create() gSemTest2 Failed \n");
    }

    TaskP_Params_init(&taskParams1);
    taskParams1.priority  = 14;
    taskParams1.stack     = gSciclientAppTskStackRevTest1;
    taskParams1.stacksize = sizeof(gSciclientAppTskStackRevTest1);

    TaskP_Params_init(&taskParams2);
    taskParams2.stack     = gSciclientAppTskStackRevTest2;
    taskParams2.stacksize = sizeof(gSciclientAppTskStackRevTest2);
    taskParams2.priority  = 15;

    SciApp_consoleInit();

    task1 = TaskP_create(&SciclientApp_getRevisionTest1, &taskParams1);
    if (task1 == NULL)
    {
        SciApp_printf("Task_create() SciclientApp_getRevisionTest1 failed!\n");
    }

    task2 = TaskP_create(&SciclientApp_getRevisionTest2, &taskParams2);
    if (task2 == NULL)
    {
        SciApp_printf("Task_create() SciclientApp_getRevisionTest2 failed!\n");
    }

    SemaphoreP_pend(gSemTest1, SemaphoreP_WAIT_FOREVER);
    SemaphoreP_pend(gSemTest2, SemaphoreP_WAIT_FOREVER);
    
    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA Entry... \n");
    upload_execution_history();
    UART_printf("\n LDRA Exit... \n");
    #endif
     
    if ((gTsk1Pass == CSL_PASS) && (gTsk2Pass == CSL_PASS))
    {
        SciApp_printf("All tests have passed \n");
    }
    else
    {
        SciApp_printf("Some Tests have failed \n");
    }
  
}

static void SciclientApp_getRevisionTest1(void* arg0, void* arg1)
{
    int32_t status                  = CSL_PASS;
    Sciclient_ConfigPrms_t   config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL
    };
    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t     respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof(response)
    };

    SciApp_printf("SCIClient RTOS Test App1\n\n");

    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if (respPrm.flags == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                SciApp_printf(" DMSC Firmware Version 1 %s\n",
                                  (char *) response.str);
                SciApp_printf(" Firmware revision 0x%x\n", response.version);
                SciApp_printf(" ABI revision %d.%d\n", response.abi_major,
                                  response.abi_minor);
            }
            else
            {
                SciApp_printf(" DMSC Firmware Get Version failed \n");
            }
        }
        else
        {
            SciApp_printf(" DMSC Firmware Get Version failed \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    if (status == CSL_PASS)
    {
        gTsk1Pass = CSL_PASS;
    }
    else
    {
        gTsk1Pass = CSL_EFAIL;
    }

    SemaphoreP_post(gSemTest1);
}

static void SciclientApp_getRevisionTest2(void* arg0, void*  arg1)
{
    int32_t status                  = CSL_PASS;
    Sciclient_ConfigPrms_t   config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL
    };
    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t     respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };

    SciApp_printf("SCIClient RTOS Test App2\n\n");

    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if (respPrm.flags == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                SciApp_printf(" DMSC Firmware Version 2 %s\n",
                                  (char *) response.str);
                SciApp_printf(" Firmware revision 0x%x\n", response.version);
                SciApp_printf(" ABI revision %d.%d\n", response.abi_major,
                                  response.abi_minor);
                TaskP_sleep(100);
            }
            else
            {
                SciApp_printf(" DMSC Firmware Get Version failed \n");
            }
        }
        else
        {
            SciApp_printf(" DMSC Firmware Get Version failed \n");
        }
    }

    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    if (status == CSL_PASS)
    {
        gTsk2Pass = CSL_PASS;
    }
    else
    {
        gTsk2Pass = CSL_EFAIL;
    }

    SemaphoreP_post(gSemTest2);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

