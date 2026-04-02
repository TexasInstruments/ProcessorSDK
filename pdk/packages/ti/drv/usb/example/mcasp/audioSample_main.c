/*
 * audioSample_main.c
 *
 * This file contains the test / demo code to demonstrate the Audio component
 * driver functionality on SYS/BIOS 6.
 *
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
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

/** \file   audioSample_main.c
 *
 *  \brief    sample application for demonstration of audio driver usage
 *
 *  This file contains the implementation of the sample appliation for the
 *  demonstration of audio playing through the audio interface layer.
 *
 *             (C) Copyright 2009, Texas Instruments, Inc
 */

/* ========================================================================== */
/*                            INCLUDE FILES                                   */
/* ========================================================================== */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"

#include <mcasp_drv.h>
#ifdef AIC_CODEC
#include <Aic31.h>
#endif
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/sdo/edma3/drv/edma3_drv.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_bootcfg.h>

#include "stdio.h"
#include "stdlib.h"

/* ========================================================================== */
/*                           MACRO DEFINTIONS                                 */
/* ========================================================================== */
/* Test application stack size */
#define APP_TSK_STACK_MAIN         (0x1c00U)
/* Test application stack */
static uint8_t gTskStackMain[APP_TSK_STACK_MAIN];



/* Handle to the EDMA driver instance                                         */
EDMA3_DRV_Handle hEdma;

extern void configureAudio(void);
extern void McaspDevice_init(void);
extern void configMcASP_SocHwInfo(void);
extern void Audio_echo_Task(void *arg0, void *arg1);
extern int usb_main(void);

/* ========================================================================== */
/*                           FUNCTION DEFINITIONS                             */
/* ========================================================================== */


/**
 *  \brief  void main(void)
 *
 *   Main function of the sample application. This function enables
 *   the mcasp instance in the power sleep controller and also
 *   enables the pinmux for the mcasp instance. This also powers up
 *   any codecs if attached to McASP like the AIC codec, before switching to
 *   the task to Audio_echo_task().
 *
 *  \param  None
 *  \return None
 */
int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    /* enable the pinmux & PSC-enable for the mcasp device    */
    configureAudio();

    /* Initializing McASP HwInfo parameters */
    McaspDevice_init();

    /* Perform SOC specific McASP HwInfo Configuration for non-default parameters
     * using the socGetConfig() and socSetConfig(). Please note that
      this is being called AFTER McaspDevice_init() which initializes with the
      default parameters */
    configMcASP_SocHwInfo();
#if defined(AIC_CODEC)
    Aic31_init();
#endif

    OS_init();

    TaskP_Params_init(&taskParams);
    taskParams.stack     = gTskStackMain;
    taskParams.stacksize = sizeof(gTskStackMain);
    taskParams.priority = 2;
    task = TaskP_create(&Audio_echo_Task, &taskParams, &eb);
    if (task == NULL) {
        printf("TaskP_create() failed!\n");
        OS_stop();
    }

    usb_main();

    Log_info0("\r\nAudio Sample Main\n");

    OS_start();    /* does not return */

    return 0;
}
/*
 * Mcasp init function called when creating the driver.
 */




/* ========================================================================== */
/*                                END OF FILE                                 */
/* ========================================================================== */
