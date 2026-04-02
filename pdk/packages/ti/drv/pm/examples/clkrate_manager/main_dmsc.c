/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018
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
 *  \file   main_dmsc.c
 *
 *  \brief  Clock Rate Manager Library Examples and tests Application
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ti/drv/pm/pmlib.h>
#include <ti/drv/pm/examples/clkrate_manager/pmtest_testsList.h>
#include "app_utils.h"

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Test to set the allowed clkrates for all modules and all clocks.
 *
 *  \param  None
 *
 *  \return None
 */
int setFreqAll();

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    int32_t status = PM_SUCCESS;
    AppUtils_defaultInit();
    App_printf("\r\nPM ClockRate Test Application\n");
    /* Functions to Test clkrate manager */

    status = setFreqAll();
    if (status == PM_SUCCESS)
    {
        App_printf("All tests have passed.\n\n");
    }
    else
    {
        App_printf("Some tests have failed.\n\n");
    }
    return 0;
}

int setFreqAll()
{
    uint32_t    i = 0;
    int32_t     status = PM_SUCCESS;
    int32_t     retval = PM_SUCCESS;
    uint32_t modId;
    uint32_t clkId;
    uint64_t clkRate;
    uint32_t    numTableEntries = sizeof (gPmtest_setFreqTestList) /
                                 sizeof (Pmtest_inputClk_t);

    status = Sciclient_init(NULL);

    if (status == PM_SUCCESS)
    {
        uint32_t j;
        char strToPrint[256];
        App_printf("Sciclient initialization passed...\n");
        for (j = 0U; j < 80U; j++)
        {
            App_printf("-");
        }
        App_printf("\n");
        for (i = 0U; i < numTableEntries; i++)
        {
            modId = gPmtest_setFreqTestList[i].modId;
            clkId = gPmtest_setFreqTestList[i].clkId;
            clkRate = gPmtest_setFreqTestList[i].clkRate;

            App_printf("Module ID: %d\nClock ID: %d\n",
                            modId,
                            clkId);
            if(clkRate == 0)
            {
                App_printf("Frequency to be set: %11s\n", "NA");
                status = PMLIBClkRateGet(modId,clkId,&clkRate);
#if defined (BUILD_MCU)
                snprintf(strToPrint, 256, "%llu.%06llu", clkRate/1000000, clkRate %1000000);
#else
                snprintf(strToPrint, 256, "%lu.%06lu", clkRate/1000000, clkRate %1000000);
#endif
                App_printf("Current Frequency: %s MHz\n", strToPrint);
            }
            else
            {
                uint64_t prevClkRate = 0;
#if defined (BUILD_MCU)
                snprintf(strToPrint, 256, "%llu.%06llu", clkRate/1000000, clkRate %1000000);
#else
                snprintf(strToPrint, 256, "%lu.%06lu", clkRate/1000000, clkRate %1000000);
#endif
                App_printf("Frequency to be set:  %s MHz\n", strToPrint);
                status = PMLIBClkRateGet(modId,clkId,&prevClkRate);
                if (status == PM_SUCCESS)
                {
                    if (clkRate != prevClkRate)
                    {
                        status = PMLIBClkRateSet(modId,clkId,clkRate);
                        status += PMLIBClkRateGet(modId,clkId,&clkRate);
                    }
                    else
                    {
                        status = PM_SUCCESS;
                        App_printf("Frequency is same as before. No Change.\n");
                    }
                }
#if defined (BUILD_MCU)
                snprintf(strToPrint, 256, "%llu.%06llu", clkRate/1000000, clkRate %1000000);
#else
                snprintf(strToPrint, 256, "%lu.%06lu", clkRate/1000000, clkRate %1000000);
#endif
                App_printf("Frequency after setting:  %s MHz\n", strToPrint);
                if (clkRate != gPmtest_setFreqTestList[i].clkRate)
                {
                    status = PM_FAIL;
                }
            }
            if (status != PM_SUCCESS)
            {
                App_printf("Test Status: FAIL\n");
            }
            else
            {
                App_printf("Test Status: PASS\n");
            }
            for (j = 0U; j < 80U; j++)
            {
                App_printf("-");
            }
            App_printf("\n");
            retval = retval + status;
        }
    }
    else
    {
         App_printf("Sciclient initialization failed!!!\n");
    }
    retval = retval+status;

    App_printf("\n");
    for (i = 0U; i < 80U; i++)
    {
        App_printf("-");
    }
    App_printf("\n");
    App_printf("Clkrate setFrequencyAll test case done!!!\n");

    Sciclient_deinit();

    return retval;
}
