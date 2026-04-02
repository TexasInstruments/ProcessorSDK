/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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
 *
 *  \details    This application is expected to be hosted on SoC/cores
 *                  J721E   / MCU 1_0 and MCU 2_1
 *                  J7200   / MCU 1_0 and MCU 2_1
 *                  J721S2  / MCU 1_0 and MCU 2_1
 *
 *  Note on Available Instances on EVM, and Tested Instances with this test:
 *              For J721S2_EVM:
 *                  Available Instances    : MCU_MCAN0,MCU_MCAN1,MAIN_MCAN3,
 *                                           MAIN_MCAN4,MAIN_MCAN5,MAIN_MCAN16
 *                  Test support Instances : For mcu1_0
 *                                            - Default MCAN Module   : MCU_MCAN0
 *                                           For mcu2_1
 *                                            - Default MCAN Module   : MAIN_MCAN4
 *                                           NOTE: MAIN_MCAN4 is J6(MC9) on GESI Board for J721S2.
 *              For J7200_EVM:
 *                  Available Instances    : MCU_MCAN0, MCU_MCAN1, MAIN_MCAN0, MAIN_MCAN3
 *                                           MAIN_MCAN4, MAIN_MCAN5, MAIN_MCAN6, MAIN_MCAN7, 
 *                                           MAIN_MCAN8, MAIN_MCAN10
 *                  Test support Instances : For mcu1_0
 *                                            - Default MCAN Module   : MCU_MCAN0
 *                                            - Default MCAN RX Module: MCU_MCAN1
 *                                           For mcu2_1
 *                                            - Default MCAN Module   : MAIN_MCAN0
 *                                            - Default MCAN RX Module: MAIN_MCAN1
 *              For J721E_EVM:
 *                  Available Instances    : MCU_MCAN0,MCU_MCAN1,MAIN_MCAN0,MAIN_MCAN2
 *                                            MAIN_MCAN4,MAIN_MCAN5,MAIN_MCAN6,MAIN_MCAN7,
 *                                            MAIN_MCAN9,MAIN_MCAN11
 *                  Test support Instances : For mcu1_0
 *                                            - Default MCAN Module   : MCU_MCAN0
 *                                            - Default MCAN RX Module: MCU_MCAN1
 *                                           For mcu2_1
 *                                            - Default MCAN Module   : MAIN_MCAN0
 *                                            - Default MCAN RX Module: MAIN_MCAN1
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/tistdtypes.h>
#include <st_mcan.h>
#include <ti/osal/src/nonos/Nonos_config.h>

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

int32_t main_task(void);
void enbaleIsrPrint(uint32_t status);
void st_mcanCallTxFunc(st_mcanTestcaseParams_t *testParams);
extern int32_t st_mcanTxApp_main(st_mcanTestcaseParams_t *testParams);
uint64_t Utils_prfTsGet64(void);
uint32_t Utils_prfTsGetFreq(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern volatile uint32_t isrPrintEnable;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    main_task();
    return(0);
}

void enbaleIsrPrint(uint32_t status)
{
    if (status == TRUE)
    {
        isrPrintEnable = TRUE;
    }
    else
    {
        isrPrintEnable = FALSE;
    }
}

void st_mcanCallTxFunc(st_mcanTestcaseParams_t *testParams)
{
	st_mcanTxApp_main(testParams);
}

uint64_t Utils_prfTsGet64(void)
{
	uint64_t retVal;
	TimeStamp_Struct timeStamp;
	osalArch_TimestampGet64(&timeStamp);
	retVal = (uint64_t) timeStamp.hi << 32U;
	retVal |= timeStamp.lo;
	return retVal;
}

uint32_t Utils_prfTsGetFreq(void)
{
	return (osalArch_TimeStampGetFreqKHz());
}
