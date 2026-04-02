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

/**
 *  \file main_baremetal.c
 *
 *  \brief Main file for baremetal build
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/tistdtypes.h>
#include <ti/osal/src/nonos/Nonos_config.h>
/* SCI Client */
#include <ti/drv/sciclient/sciclient.h>
#include <ti/board/board.h>

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

extern int32_t Ipc_echo_test(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern volatile uint32_t isrPrintEnable;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * This function initializes the SCI Client driver
 *
 */
void ipc_initSciclient()
{
    Sciclient_ConfigPrms_t        config;

    /* Now reinitialize it as default parameter */
    Sciclient_configPrmsInit(&config);

#if defined(BUILD_MPU1_0) || defined(BUILD_C7X)
    config.opModeFlag  = SCICLIENT_SERVICE_OPERATION_MODE_POLLED;
#endif

    Sciclient_init(&config);

}

#if !defined(A72_LINUX_OS)
void ipc_boardInit()
{
    Board_initCfg           boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

}
#endif

/*
 * Main function for the test code
 *
 */
int main(void)
{

    ipc_initSciclient();

#if !defined(A72_LINUX_OS)
    ipc_boardInit();
#endif

    Ipc_echo_test();
    
    #if defined LDRA_DYN_COVERAGE_EXIT
    UART_printf("\n LDRA ENTRY... \n");
    upload_execution_history();
    UART_printf("\n LDRA EXIT... \n");
    #endif
    
    return(0);
}
