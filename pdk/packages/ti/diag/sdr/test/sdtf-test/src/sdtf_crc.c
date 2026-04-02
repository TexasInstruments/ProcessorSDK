/*
 * SDTF CRC
 *
 * Software Diagnostics Reference Test for CRC module
 *
 *  Copyright (c) Texas Instruments Incorporated 2019-2020
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

#include <stdint.h>

#include <sdr_crc.h>

#include "sdtf_common.h"
#include "sdtf_profile.h"
#include "sdtf_test.h"
#include "sdtf_crc.h"

/* SDTF CRC local defines */
#define SDTF_CRC_REF_SIGN_HIGH    (0xF2C2E9EEU)  /* Reference high 32-bit CRC signautre value */
#define SDTF_CRC_REF_SIGN_LOW     (0xEBEB19C1U)  /* Reference low 32-bit CRC signautre value */
#define SDTF_CRC_DATA_SIZE        (1843200U)

/* CRC Self Test data */
uint8_t SDTF_crcTestData[SDTF_CRC_DATA_SIZE] __attribute__ ((aligned(128))) __attribute__((section(".bss:extMemCache:ramdisk")));

/*********************************************************************
 * @fn      SDTF_runCRCSelfTest
 *
 * @brief   Execute CRC Self Test
 *
 * @param   None
 *
 * @return  0 : Success; < 0 for failures
 */
int32_t SDTF_runCRCSelfTest(void)
{
    SDR_Result           result;
    int32_t              retVal=0;
    SDR_CRC_dataConfig_t crcData;
    uint32_t             i;
    uint32_t  *pCRCData;

    SDTF_printf("\n CRC self test: starting");

    crcData.pCRCData       = (uint32_t *)SDTF_crcTestData;
    crcData.size           = SDTF_CRC_DATA_SIZE;
    crcData.dataBitSize    = SDR_CRC_DATA_32_BIT;
    crcData.crcOpMode      = CRC_OPERATION_MODE_FULLCPU;
    crcData.refCRCValueMSW = SDTF_CRC_REF_SIGN_HIGH;
    crcData.refCRCValueLSW = SDTF_CRC_REF_SIGN_LOW;

    pCRCData = (uint32_t *)crcData.pCRCData;
    for (i = 0; i < (crcData.size / 4U); i++)
    {
        pCRCData[i] = i;
    }

    SDTF_profileBegin(SDTF_PROFILE_ONESHOT);

    result = SDR_CRC_selftest(CRC_CHANNEL_1, &crcData);

    SDTF_profileEnd(SDTF_PROFILE_ONESHOT);

    if (result != SDR_PASS)
    {
        SDTF_printf("\n CRC self test failed");
        retVal = SDR_FAIL;
    }
    else
    {
        SDTF_printf("\n CRC self test complete");
        SDTF_printf("\n Cycles taken %d", SDTF_profileDelta(SDTF_PROFILE_ONESHOT));
    }

    return retVal;
}

int32_t SDTF_runCRCNegativeTest(void)
{
    int32_t              retVal = 0;
    SDR_CRC_dataConfig_t crcData;

    SDTF_printf("\n CRC negative tests: starting");

    /* SDR_CRC_selftest Negative tests with invalid params */
    if (SDR_CRC_selftest(CRC_CHANNEL_1 - 1U, &crcData) != SDR_BADARGS)
    {
        retVal = -1;
    }

    if (retVal == 0)
    {
        if (SDR_CRC_selftest(CRC_CHANNEL_4 + 1U, &crcData) != SDR_BADARGS)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        if (SDR_CRC_selftest(CRC_CHANNEL_1, NULL_PTR) != SDR_BADARGS)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        crcData.pCRCData = NULL_PTR;
        if (SDR_CRC_selftest(CRC_CHANNEL_1, NULL_PTR) != SDR_BADARGS)
        {
            retVal = -1;
        }
    }

    if (retVal == 0)
    {
        SDTF_printf("\n CRC negative tests: success");
    }
    else
    {
        SDTF_printf("\n CRC negative tests: failed");
    }

    return retVal;
}
