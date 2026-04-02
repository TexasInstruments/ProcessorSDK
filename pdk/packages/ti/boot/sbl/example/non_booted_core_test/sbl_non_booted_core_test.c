/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
/**
 *  \file sbl_non_booted_core_test.c
 *
 *  \brief File to check whether non-booted cores are properly switched off or not
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/boot/sbl/soc/k3/sbl_soc_cfg.h>
#include <ti/boot/sbl/soc/k3/sbl_slave_core_boot.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct App_CoreInfo_s
{
    uint32_t deviceId;
    char* coreName;
} App_CoreInfo;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

 
App_CoreInfo coreInfo[] =
{
    /* MPU1_CPU0 info */
    {
    SBL_DEV_ID_MPU1_CPU0,
    "MPU1_0"
    },
    /* MPU1_CPU1 info */
    {
    SBL_DEV_ID_MPU1_CPU1,
    "MPU1_1"
    },
    /* MPU1_CPU2 info */
    {
    SBL_DEV_ID_MPU1_CPU2,
    "MPU1_2"
    },
    /* MPU1_CPU3 info */
    {
    SBL_DEV_ID_MPU1_CPU3,
    "MPU1_3"
    },
    /* MPU2_CPU0 info */
    {
    SBL_DEV_ID_MPU2_CPU0,
    "MPU2_0"
    },
    /* MPU2_CPU1 info */
    {
    SBL_DEV_ID_MPU2_CPU1,
    "MPU2_1"
    },
    /* MPU2_CPU2 info */
    {
    SBL_DEV_ID_MPU2_CPU2,
    "MPU2_2"
    },
    /* MPU2_CPU3 info */
    {
    SBL_DEV_ID_MPU2_CPU3,
    "MPU2_3"
    },
    /* MCU1_CPU0 info */
    {
    SBL_DEV_ID_MCU1_CPU0,
    "MCU1_0"
    },
    /* MCU1_CPU1 info */
    {
    SBL_DEV_ID_MCU1_CPU1,
    "MCU1_1"
    },
    /* MCU2_CPU0 info */
    {
    SBL_DEV_ID_MCU2_CPU0,
    "MCU2_0"
    },
    /* MCU2_CPU1 info */
    {
    SBL_DEV_ID_MCU2_CPU1,
    "MCU2_1"
    },
    /* MCU3_CPU0 info */
    {
    SBL_DEV_ID_MCU3_CPU0,
    "MCU3_0"
    },
    /* MCU3_CPU1 info */
    {
    SBL_DEV_ID_MCU3_CPU1,
    "MCU3_1"
    },
    /* MCU4_CPU0 info */
    {
    SBL_DEV_ID_MCU4_CPU0,
    "MCU4_0"
    },
    /* MCU4_CPU1 info */
    {
    SBL_DEV_ID_MCU4_CPU1,
    "MCU4_1"
    },
    /* DSP1_C66X info */
    {
    SBL_DEV_ID_DSP1_C66X,
    "C66X_0"
    },
    /* DSP2_C66X info */
    {
    SBL_DEV_ID_DSP2_C66X,
    "C66X_1"
    },
    /* DSP1_C7X info */
    {
    SBL_DEV_ID_DSP1_C7X,
    "C7X_1"
    },
    /* DSP2_C7X info */
    {
    SBL_DEV_ID_DSP2_C7X,
    "C7X_2"
    },
    /* DSP3_C7X info */
    {
    SBL_DEV_ID_DSP3_C7X,
    "C7X_3"
    },
    /* DSP4_C7X info */
    {
    SBL_DEV_ID_DSP4_C7X,
    "C7X_4"
    }
};

int main()
{
    Board_initCfg           boardCfg;
    int32_t                 retVal;
    uint32_t                moduleState = 0U;
    uint32_t                resetState = 0U;
    uint32_t                contextLossState = 0U;
    uint32_t                moduleId = 0U;
    int32_t                 result = 0;

    boardCfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    for(int32_t coreId = MPU1_CPU0_ID; coreId <= DSP4_C7X_ID; coreId++)
    {
        if (MCU1_CPU0_ID == coreId || MCU1_CPU1_ID == coreId)
        {
            /* Since this application is running on mcu1_0, no need to check the
               power status of mcu1_0 and mcu1_1 */
            continue;
        }
        /* moduleId for core is SBL_INVALID_ID if corresponding core is not present in the SOC*/
        moduleId = (uint32_t) coreInfo[coreId].deviceId;
        if (SBL_INVALID_ID != moduleId)
        {
            retVal = Sciclient_pmGetModuleState(moduleId,
                                                &moduleState,
                                                &resetState,
                                                &contextLossState,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            if (CSL_PASS != retVal)
            {
                UART_printf("Sciclient_pmGetModuleState Failed !! \n");
            }
            else
            {
                if (0U != moduleState)
                {
                    UART_printf("%s is powered ON !! \n ", coreInfo[coreId].coreName);
                    result += CSL_EFAIL;
                }
                else
                {
                    UART_printf("%s is powered OFF !! \n ", coreInfo[coreId].coreName);
                }
            }
        }

    }

    if (0U != result)
    {
        UART_printf("Some tests have failed !! \n");
    }
    else
    {
        UART_printf("All tests have passed\n");
    }

    return 0;
}
