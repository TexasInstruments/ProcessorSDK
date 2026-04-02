/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 *  \file     bist.c
 *
 *  \brief    This file implements BIST task function
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <soc.h>
#include "lbist_utils.h"
#include "pbist_utils.h"
#include "bist_core_defs.h"

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* This flags enables gathering timing information for BIST stages */
#define GATHER_BIST_STAGE_DETAILS

/* For PBIST and LBIST stage tests, status is saved in bitfields.
 * For these bitfields, status is represented by below defines */
#define TEST_PASS_BIT         1
#define TEST_FAIL_NOT_RUN_BIT 0

/* Define the unlock and lock values */
#define BOOT_APP_KICK0_UNLOCK_VAL               0x68EF3490
#define BOOT_APP_KICK1_UNLOCK_VAL               0xD172BC5A

/* ========================================================================== */
/*                    Internal Function Declarations                          */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static const char *testStatusPrint(int32_t status)
{
    char *name;

    switch(status)
    {
        case (TEST_PASS_BIT):
            name="PASS";
            break;
        case (TEST_FAIL_NOT_RUN_BIT):
            name="FAIL or NOT RUN";
            break;
        default:
            name="INVALID STATUS";
            break;
    }

    return name;
}

void BootApp_bistFxn(void)
{
    int32_t testResult = 0;
    int32_t     i, j;
    int32_t    *pbist_array;
    int32_t    *lbist_array;
    /* Bitmap representing status of each PBIST test in each boot stage */
    int32_t     pbist_stage_status[NUM_BOOT_STAGES];
    /* Bitmap representing status of each LBIST test in each boot stage */
    int32_t     lbist_stage_status[NUM_BOOT_STAGES];

    /* Initialize boot stage status bitmaps to "not run/fail" */
    for (i = 0; i < NUM_BOOT_STAGES; i++)
    {
        pbist_stage_status[i] = 0x0;
        lbist_stage_status[i] = 0x0;
    }

    testResult = BootApp_pbistCommonInit();

    if (testResult != 0)
    {
        UART_printf("BootApp_pbistCommonInit ...FAILED \n");
    }
    else
    {
        /* This example presents a scenario in which the BISTs are performed in stages
         * with booting of specific cores performed after certain stages. This example
	 * is adapted from the MCUSW Boot App for J721E. No cores are actually loaded
	 * in this example. Only the flow of performing BIST in stages is shown. */
        /* Run pre-boot-stage PBIST's.  The definitions of the pre-boot-stage PBIST's
         * are found in soc/<SOC Device>/bist_core_defs.c.*/

        for (i = 0; i < num_pbists_pre_boot; i++)
        {
            #if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
            /* Run test on selected instance */
            testResult = BootApp_pbistRunTest(pbist_pre_boot_stage[i],(uint8_t)PBIST_TEST_POSITIVE);
            /* Convert signed return value (with -1 = failure and 0 = pass) to become
             * 0 = failure and 1 = pass */
            pbist_pre_boot_stage_status[i] = testResult + 1;
            if ( testResult != 0)
            {
                UART_printf("PBIST functional test failed for %d\n",
                                pbist_pre_boot_stage[i]);
                break;
            }
            #else
            /* Run test on selected instance */
            testResult = BootApp_pbistRunTest(pbist_pre_boot_stage[i], (uint8_t)0);
            /* Convert signed return value (with -1 = failure and 0 = pass) to become
             * 0 = failure and 1 = pass */
            pbist_pre_boot_stage_status[i] = testResult + 1;
            if ( testResult != 0)
            {
                UART_printf("PBIST functional test failed for %d\n",
                                pbist_pre_boot_stage[i]);
                break;
            }
            #endif
        }

        /* Run pre-boot-stage LBIST's. The definitions of the pre-boot-stage LBIST's
         * are found in soc/<SOC Device>/bist_core_defs.c. */
        for (i = 0; i < num_lbists_pre_boot; i++)
        {
            /* Run test on selected instance */
            testResult = BootApp_lbistRunTest(lbist_pre_boot_stage[i]);
            lbist_pre_boot_stage_status[i] = testResult;
            if ((testResult == -1) ||
                (testResult == LBIST_POST_COMPLETED_FAILURE) ||
                (testResult == LBIST_POST_ATTEMPTED_TIMEOUT))
            {
                UART_printf("LBIST functional test failed for %d\n",
                                lbist_pre_boot_stage[i]);
                break;
            }
        }

        /* After running PBIST potentially on MSMC, re-initialize the CLEC */
        testResult = BootApp_pbistCommonInit();

        if (testResult != 0)
        {
            UART_printf("BootApp_pbistCommonInit after pre-boot stage ...FAILED \n");
        }

        /* Run LBIST and PBIST before each boot stage.  The definitions of the LBIST
         * and PBIST sections for each stage are defined in
         * soc/<SOC Device>/bist_core_defs.c. */
        for (j = 0; j < NUM_BOOT_STAGES; j++)
        {
            pbist_array = pbist_array_stage[j];
            lbist_array = lbist_array_stage[j];

            for (i = 0; i < num_pbists_per_boot_stage[j]; i++)
            {
                BootApp_pbistClecConfig(pbist_array[i]);

                #if defined (SOC_J784S4) || defined (SOC_J742S2)
                /* Main Infra0/1, NAVSS and MSMC should be run in SBL, if Boot App is running in DDR */
                if((i==2)||(i==6)||(i==7)||(i==25))
                {
                    continue;
                }
                /* HC has MMCSD in Auxiallary list */
                if(i==9)
                {
                    continue;
                }
                /* BISTs executed in SBL */
                if((i==13)||(i==14)||(i==15)||(i==16)||(i==1)||(i==12)||(i==4))
                {
                    continue;
                }
     
                /* Run test on selected instance */	
                testResult = BootApp_pbistRunTest(pbist_array[i], (uint8_t)PBIST_TEST_POSITIVE);

                /* Convert signed return value (with -1 = failure and 0 = pass) to become
                 * a single bit as part of bitfield with 0 = failure and 1 = pass */
                pbist_stage_status[j] |= ((uint32_t)(testResult + 1) << i);
                if ( testResult != 0)
                {
                    UART_printf("PBIST functional test failed for %d\n",
                                    pbist_array[i]);
                    break;
                }
                #elif defined (SOC_J721S2)
                /* Main Infra0/1, NAVSS and MSMC should be run in SBL, if Boot App is running in DDR */
                if((i==1)||(i==5)||(i==7)||(i==13))
                {
                    continue;
                }

                /* HC has MMCSD in Auxiallary list */
                if(i==6)
                {

                    continue;
                }

                /* BISTs executed in SBL */
                if((i==12)||(i==17)||(i==8))
                {
                    continue;
                }

                testResult = BootApp_pbistRunTest(pbist_array[i], (uint8_t)0);
                /* Convert signed return value (with -1 = failure and 0 = pass) to become
                 * a single bit as part of bitfield with 0 = failure and 1 = pass */
                pbist_stage_status[j] |= ((uint32_t)(testResult + 1) << i);
                if ( testResult != 0)
                {
                    UART_printf("PBIST functional test failed for %d\n",
                                    pbist_array[i]);
                    break;
                }
                #else 
                testResult = BootApp_pbistRunTest(pbist_array[i], (uint8_t)0);
                /* Convert signed return value (with -1 = failure and 0 = pass) to become
                 * a single bit as part of bitfield with 0 = failure and 1 = pass */
                pbist_stage_status[j] |= ((uint32_t)(testResult + 1) << i);
                if ( testResult != 0)
                {
                    UART_printf("PBIST functional test failed for %d\n",
                                    pbist_array[i]);
                    break;
                }
                #endif
            }
#if defined(DEBUG)
            UART_printf( "Ran PBIST for Stage %d\n", j);
#endif
            for (i = 0; i < num_lbists_per_boot_stage[j]; i++)
            {
                testResult = BootApp_lbistRunTest(lbist_array[i]);

                /* Convert signed return value (with -1 = failure and 0 = pass) to become
                 * a single bit as part of bitfield with 0 = failure and 1 = pass */
                lbist_stage_status[j] |= ((uint32_t)(testResult + 1) << i);
                if (testResult != 0)
                {
                    UART_printf("LBIST functional test failed for %d\n",
                                    lbist_array[i]);
                    break;
                }
            }
#if defined(DEBUG)
            UART_printf( "Ran LBIST for Stage %d\n", j);
#endif
            /* Signal Boot Task that BIST for the stage is completed */
            UART_printf("\n *** Boot stage %d is complete, cores for this stage may now be loaded ***\n\n", j);
        }
    }

    if (testResult == 0)
    {
        UART_printf("==========================\n");
        UART_printf("BIST: Example App Summary:\n");
        UART_printf("==========================\n");
        for (i = 0; i < num_pbists_pre_boot; i++)
        {
            UART_printf("BIST: Pre-boot Stage - Ran PBIST ID - %s, Result = %s\n",
                            BootApp_pbistName(pbist_pre_boot_stage[i]),
                            testStatusPrint(pbist_pre_boot_stage_status[i]));
        }
        UART_printf("Pre-boot stage - Ran %d PBIST total sections\n",
                        num_pbists_pre_boot);
        for (i = 0; i < num_lbists_pre_boot; i++)
        {
            UART_printf("BIST: Pre-boot Stage - Ran LBIST ID - %s, Result = %s\n",
                            BootApp_lbistName(lbist_pre_boot_stage[i]),
                            BootApp_lbistHwPostStatusPrint(lbist_pre_boot_stage_status[i]));
        }
        UART_printf("Pre-boot stage - Ran %d LBIST total sections\n",
                        num_lbists_pre_boot);

        for (j = 0; j < NUM_BOOT_STAGES; j++)
        {
            pbist_array = pbist_array_stage[j];
            lbist_array = lbist_array_stage[j];
#if defined(GATHER_BIST_STAGE_DETAILS)
            for (i = 0; i < num_pbists_per_boot_stage[j]; i++)
            {
            #if defined(SOC_J784S4) || defined(SOC_J742S2)
                /* Main Infra0/1, NAVSS and MSMC should be run in SBL, if Boot App is running in DDR */
                if((i==2)||(i==6)||(i==7)||(i==25))
                {
                    continue;
                }
                /* HC has MMCSD in Auxiallary list */
                if(i==9)
                {
                    continue;
                }
                /* BISTs executed in SBL */
                if((i==13)||(i==14)||(i==15)||(i==16)||(i==1)||(i==12)||(i==4))
                {
                    continue;
                }
            #elif defined(SOC_J721S2)
                if((i==1)||(i==5)||(i==7)||(i==13))
                {
                    continue;
                }
                /* HC has MMCSD in Auxiallary list */
                if(i==6)
                {
                    continue;
                }
                /* BISTs executed in SBL */
                if((i==12)||(i==17)||(i==8))
                {
                    continue;
                }
            #endif
                UART_printf("BIST: Stage %d - Ran PBIST ID - %s, Result = %s\n",
                                j, BootApp_pbistName(pbist_array[i]),
                                testStatusPrint((pbist_stage_status[j] >> i) & 0x1));
            }
            UART_printf("BIST: Stage %d - Ran %d PBIST total sections\n",
                            j, (uint32_t)num_pbists_per_boot_stage[j]);

            for (i = 0; i < num_lbists_per_boot_stage[j]; i++)
            {
                UART_printf("BIST: Stage %d - Ran LBIST ID - %s, Result = %s\n",
                                j, BootApp_lbistName(lbist_array[i]),
                                testStatusPrint((lbist_stage_status[j] >> i) & 0x1));
            }
            UART_printf("BIST: Stage %d - Ran %d LBIST sections\n",
                            j, (uint32_t)num_lbists_per_boot_stage[j]);
#endif
        }
        UART_printStatus("\r\nAll tests have passed. \r\n");
    }
}

