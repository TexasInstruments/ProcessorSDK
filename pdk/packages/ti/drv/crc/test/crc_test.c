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
 *   \file  crc_test.c
 *
 *   \brief
 *      This file contains common CRC Test Functions.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "ti/drv/crc/crc.h"
#include "crc_log.h"
#include "crc_test.h"
#include "crcmodel.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define CRC_API_TEST_REAL_DATA_SIZE             (16U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t  gDataAligned16 [CRC_API_TEST_REAL_DATA_SIZE] __attribute__((aligned(16)));
static uint8_t  gDataAligned32 [CRC_API_TEST_REAL_DATA_SIZE] __attribute__((aligned(32)));
static uint8_t  gDataAligned128[CRC_API_TEST_REAL_DATA_SIZE] __attribute__((aligned(128)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t Test_crcInitAPI(void)
{
    CRC_Config      crcCfg;
    CRC_Handle      crcHandle1;
    CRC_Handle      crcHandle2;
    int32_t         errCode;

    /**************************************************************************
     * Test: Open & close CRC driver(s)
     **************************************************************************/
    /* Setup the default configuration: */
    CRC_initConfigParams(&crcCfg);
    crcCfg.channel = CRC_Channel_CH1;

    crcHandle1 = CRC_open (&crcCfg, &errCode);
    if (crcHandle1 == NULL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) open failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the default configuration: */
    CRC_initConfigParams(&crcCfg);
    crcCfg.channel = CRC_Channel_CH2;

    /* Open the CRC Driver */
    crcHandle2 = CRC_open (&crcCfg, &errCode);
    if (crcHandle2 == NULL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 2) open failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Close the CRC Driver */
    if (CRC_close (crcHandle1, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) close failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Close the CRC Driver */
    if (CRC_close (crcHandle2, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 2) close failed [Error code %d]\n", errCode);
        return -1;
    }
    Test_crcAppPrint0 ("Debug: CRC Open/Close Test Passed\n");
    return 0;
}

int32_t Test_crcComputeSignatureAPI(void)
{
    CRC_Config      crcCfg;
    CRC_Handle      crcHandle;
    int32_t         errCode;
    CRC_SigGenCfg   signGenCfg;
    uint64_t        signature;
    uint32_t        transactionId;
    uint16_t        data[] = {0x0123, 0x4567, 0x8910, 0x1112};

    /**************************************************************************
     * Initialization
     **************************************************************************/
    CRC_initConfigParams(&crcCfg);

    /* Open the CRC Driver: */
    crcHandle = CRC_open (&crcCfg, &errCode);
    if (crcHandle == NULL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) open failed [Error code %d]\n", errCode);
        return -1;
    }

    /**************************************************************************
     * Test: Compute Signature with invalid transaction id
     **************************************************************************/

    /* Initialize the signature generation configuration */
    memset ((void *)&signGenCfg, 0, sizeof(CRC_SigGenCfg));

    /* Populate the configuration: */
    signGenCfg.transactionId = 0x2;
    signGenCfg.ptrData       = (uint8_t*)&data[0];
    signGenCfg.dataLen       = sizeof(data);

    /* Compute the signature for the specific data on Channel-1 */
    if (CRC_computeSignature(crcHandle, &signGenCfg, &errCode) == 0)
    {
        Test_crcAppPrint0("Error: CRC Driver (Channel 1) compute signature successful with invalid Transaction Id\n");
        return -1;
    }
    Test_crcAppPrint0 ("Debug: Compute Signature API with invalid transaction id Passed.\n");

    /**************************************************************************
     * Test: Compute Signature with valid transaction id
     **************************************************************************/

    /* Initialize the signature generation configuration */
    memset ((void *)&signGenCfg, 0, sizeof(CRC_SigGenCfg));

    /* Allocate a unique transaction id: */
    if (CRC_getTransactionId(crcHandle, &transactionId, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) get transaction id failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Populate the signature generation configuration: */
    signGenCfg.transactionId = transactionId;
    signGenCfg.ptrData       = (uint8_t*)&data[0];
    signGenCfg.dataLen       = sizeof(data);

    /* Compute the signature for the specific data on Channel-1 */
    if (CRC_computeSignature (crcHandle, &signGenCfg, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) compute signature failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Use the correct transaction id now */
    signGenCfg.transactionId = transactionId;
    signGenCfg.ptrData       = (uint8_t*)&data[0];
    signGenCfg.dataLen       = sizeof(data);

    /* Compute the signature for the specific data on Channel-1 */
    if (CRC_computeSignature(crcHandle, &signGenCfg, &errCode) < 0)
    {
        Test_crcAppPrint1("Error: CRC Driver (Channel 1) compute signature failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Get the signature: This will give us the valid CRC signature and should also release
     * the transaction identifier. */
    if (CRC_getSignature (crcHandle, signGenCfg.transactionId, &signature, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Get signature failed [Error %d]\n", errCode);
        return -1;
    }
    Test_crcAppPrint0 ("Debug: Compute Signature API with valid transaction id Passed\n");

    /**************************************************************************
     * Cleanup
     **************************************************************************/
    if (CRC_close (crcHandle, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver close failed [Error code %d]\n", errCode);
        return -1;
    }
    return 0;
}

int32_t Test_crcGetSignatureAPI(void)
{
    CRC_Config      crcCfg;
    CRC_Handle      crcHandle;
    int32_t         errCode;
    CRC_SigGenCfg   signGenCfg;
    uint64_t        signature;
    uint16_t        data[] = {0x0123, 0x4567, 0x8910, 0x1112};

    /**************************************************************************
     * Initialization
     **************************************************************************/
    CRC_initConfigParams(&crcCfg);

    /* Open the CRC Driver: */
    crcHandle = CRC_open (&crcCfg, &errCode);
    if (crcHandle == NULL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) open failed [Error code %d]\n", errCode);
        return -1;
    }

    /**************************************************************************
     * Test: Get Signature API with Invalid Transaction Id
     **************************************************************************/
    if (CRC_getSignature (crcHandle, 0x0U, &signature, &errCode) == 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Get signature was successful [Signature %lld]\n", signature);
        return -1;
    }
    if (errCode != CRC_EINVAL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Get signature failed with invalid error code [Error code %d]\n", errCode);
        return -1;
    }
    Test_crcAppPrint0 ("Debug: Get Signature with Invalid Transaction Id Passed\n");

    /**************************************************************************
     * Test: Get Signature API with Valid Transaction Id
     **************************************************************************/

    /* Initialize the signature generation configuration */
    memset ((void *)&signGenCfg, 0, sizeof(CRC_SigGenCfg));

    /* Allocate a unique transaction id: */
    if (CRC_getTransactionId (crcHandle, &signGenCfg.transactionId, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) get transaction id failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Populate the signature generation configuration: */
    signGenCfg.ptrData = (uint8_t*)&data[0];
    signGenCfg.dataLen = sizeof(data);

    /* Compute the signature for the specific data on Channel-1 */
    if (CRC_computeSignature (crcHandle, &signGenCfg, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) compute signature failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Get the signature generation: */
    if (CRC_getSignature (crcHandle, signGenCfg.transactionId, &signature, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Get signature failed [Error code %d]\n", errCode);
        return -1;
    }
    Test_crcAppPrint0 ("Debug: Get Signature API with Valid Transaction Id Passed\n");

    /**************************************************************************
     * Cleanup
     **************************************************************************/
    if (CRC_close (crcHandle, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver close failed [Error code %d]\n", errCode);
        return -1;
    }
    return 0;
}

int32_t Test_crcCancelSignatureAPI(void)
{
    CRC_Config      crcCfg;
    CRC_Handle      crcHandle;
    int32_t         errCode;
    CRC_SigGenCfg   signGenCfg;
    uint64_t        signature;
    uint32_t        transactionId;
    uint16_t        data[] = {0x0123, 0x4567, 0x8910, 0x1112};

    /**************************************************************************
     * Initialization
     **************************************************************************/
    CRC_initConfigParams(&crcCfg);

    /* Open the CRC Driver: */
    crcHandle = CRC_open (&crcCfg, &errCode);
    if (crcHandle == NULL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) open failed [Error code %d]\n", errCode);
        return -1;
    }

    /**************************************************************************
     * Test: Cancel Signature API with Invalid Transaction Id
     **************************************************************************/
    if (CRC_cancelSignature (crcHandle, 0, &errCode) == 0)
    {
        Test_crcAppPrint0 ("Error: CRC Driver (Channel 1) Cancel signature was successful with Invalid Transaction Id\n");
        return -1;
    }
    if (errCode != CRC_EINVAL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Cancel signature failed with invalid error code [Error %d]\n", errCode);
        return -1;
    }
    Test_crcAppPrint0 ("Debug: CRC Driver cancel signature with invalid transaction id Passed\n");

    /**************************************************************************
     * Test: Cancel Signature API with Valid Transaction Id
     **************************************************************************/

    /* Initialize the signature generation configuration */
    memset ((void *)&signGenCfg, 0, sizeof(CRC_SigGenCfg));

    /* Allocate a unique transaction id: */
    if (CRC_getTransactionId(crcHandle, &transactionId, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) get transaction id failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Populate the signature generation configuration: */
    signGenCfg.transactionId = transactionId;
    signGenCfg.ptrData       = (uint8_t*)&data[0];
    signGenCfg.dataLen       = sizeof(data);

    /* Compute the signature for the specific data on Channel-1 */
    if (CRC_computeSignature (crcHandle, &signGenCfg, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) compute signature failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Cancel the signature: This should be successful */
    if (CRC_cancelSignature (crcHandle, signGenCfg.transactionId, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Cancel signature failed [Error Code %d]\n", errCode);
        return -1;
    }

    /* Validation: Try and get the signature but since the transaction has been cancelled this will fail
     * because the driver is NOT in use anymore and the transaction id is INVALID. */
    if (CRC_getSignature (crcHandle, signGenCfg.transactionId, &signature, &errCode) == 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Get signature was successful [Signature %lld]\n", signature);
        return -1;
    }
    if (errCode != CRC_EINVAL)
    {
        Test_crcAppPrint1 ("Error: CRC Driver (Channel 1) Get signature failed with invalid error code [Error code %d]\n", errCode);
        return -1;
    }
    Test_crcAppPrint0 ("Debug: CRC Driver cancel signature with valid transaction id Passed\n");

    /**************************************************************************
     * Cleanup
     **************************************************************************/
    if (CRC_close (crcHandle, &errCode) < 0)
    {
        Test_crcAppPrint1 ("Error: CRC Driver close failed [Error code %d]\n", errCode);
        return -1;
    }
    return 0;
}

int32_t Test_crcAPI(CRC_Type type, CRC_DataLen dataLen)
{
    CRC_Config      crcCfg;
    CRC_Handle      crcHandle[3];
    int32_t         errCode;
    char*           ptrDataLengthTestString;
    char*           ptrCRCTypeString;
    uint64_t        signature;
    CRC_SigGenCfg   signGenCfg;
    CRC_Channel     crcChannelNum;
    uint8_t         realData[CRC_API_TEST_REAL_DATA_SIZE] = { 0x70, 0x61, 0x6e, 0x6b, 0x61, 0x6a, 0x20, 0x6b, 0x61, 0x70, 0x6f, 0x6f, 0x72, 0x21, 0x21, 0x21 };
    int32_t         CRC16Bit;
    uint32_t        CRC32Bit;
    uint64_t        CRC64Bit;
    cm_t            crcModelCfg;
    uint8_t*        ptrTestData = NULL;

    /* Initialize the default CRC Configuration: */
    CRC_initConfigParams(&crcCfg);

    /* Get the display message: */
    switch (type)
    {
        case CRC_Type_16BIT:
        {
            /* Allocate memory for the test data: This needs to be done to ensure that the alignment is 16 bits */
            ptrTestData = &gDataAligned16[0];
            /* Populate the CRC Model Configuration: */
            crcModelCfg.cm_width = 16;
            crcModelCfg.cm_poly  = 0x1021;
            crcModelCfg.cm_init  = 0xFFFF;
            crcModelCfg.cm_refin = FALSE;
            crcModelCfg.cm_refot = FALSE;
            crcModelCfg.cm_xorot = 0x0;

            /* Initialize and perform the 16 bit CRC computation:
             * Refer to the following link:
             *  http://www.sunshine2k.de/coding/javascript/crc/crc_js.html */
            cm_ini(&crcModelCfg);
            cm_blk(&crcModelCfg, &realData[0], sizeof(realData));
            CRC16Bit = cm_crc(&crcModelCfg);

            /* Debug message: */
            ptrCRCTypeString = "CCIT(16 Bit Polynomial)";
            Test_crcAppPrint1 ("Debug: 16 Bit Software CRC is 0x%x\n", CRC16Bit);
            break;
        }
        case CRC_Type_32BIT:
        {
            /* Allocate memory for the test data: This needs to be done to ensure that the alignment is 32 bits */
            ptrTestData = &gDataAligned32[0];
            /* Populate the CRC Model Configuration: */
            crcModelCfg.cm_width = 32;
            crcModelCfg.cm_poly  = 0x4C11DB7;
            crcModelCfg.cm_init  = 0xFFFFFFFF;
            crcModelCfg.cm_refin = TRUE;
            crcModelCfg.cm_refot = TRUE;
            crcModelCfg.cm_xorot = 0xFFFFFFFF;

            /* Initialize and perform the 32 bit CRC computation:
             * Refer to the following link:
             *  http://www.sunshine2k.de/coding/javascript/crc/crc_js.html */
            cm_ini(&crcModelCfg);
            cm_blk(&crcModelCfg, &realData[0], sizeof(realData));
            CRC32Bit = cm_crc(&crcModelCfg);

            /* Debug message: */
            Test_crcAppPrint1 ("Debug: 32 Bit Software CRC is 0x%x\n", CRC32Bit);
            ptrCRCTypeString = "Ethernet(32 Bit Polynomial)";

            /* Setup the CRC Configuration: */
            crcCfg.bitSwap  = CRC_BitSwap_LSB;
            crcCfg.byteSwap = CRC_ByteSwap_DISABLED;
            break;
        }
        case CRC_Type_64BIT:
        {
            /* Allocate memory for the test data: This needs to be done to ensure that the alignment is 64 bits */
            ptrTestData = &gDataAligned128[0];
            /* This is the computed CRC using the software algorithm:
             * Refer to the following link:
             *  http://www.zorc.breitbandkatze.de/crc.html
             * NOTE: If the data is modified then there will be a failure
             * and a new CRC64 has to be generated from the above link */
            CRC64Bit = 0x77c41a42f112ad04;
            CRC_log ("Debug: 64 Bit Software CRC is 0x%016llx\n", CRC64Bit);

            ptrCRCTypeString = "ISO 3309(64 Bit Polynomial)";

            /* Debug Message: */
            Test_crcAppPrint2 ("Debug: Bit Swap: %s Byte Swap: %s\n",
                    (crcCfg.bitSwap == CRC_BitSwap_MSB) ? (uintptr_t) "MSB" : (uintptr_t) "LSB",
                    (crcCfg.byteSwap == CRC_ByteSwap_DISABLED) ? (uintptr_t) "Disabled" : (uintptr_t) "Enabled");
            break;
        }
        default:
        {
            Test_crcAppPrint1 ("Error: Invalid CRC Polynomial value [Type %d]\n", type);
            return -1;
        }
    }

    /**************************************************************************
     * Test: Open the CRC Drivers and test the CRC Functionality for all
     * supported data lengths
     **************************************************************************/
    switch (dataLen)
    {
        case CRC_DataLen_16_BIT:
        {
            ptrDataLengthTestString = "16 bits";
            break;
        }
        case CRC_DataLen_32_BIT:
        {
            ptrDataLengthTestString = "32 bits";
            break;
        }
        case CRC_DataLen_64_BIT:
        {
            ptrDataLengthTestString = "64 bits";
            break;
        }
        default:
        {
            Test_crcAppPrint1 ("Error: Invalid Data Length %d detected\n", dataLen);
            return -1;
        }
    }
    Test_crcAppPrint3 ("Debug: Testing CRC Functionality Polynomial [%s] for Pointer Data %x Data Length %s\n",
                    (uintptr_t) ptrCRCTypeString, (uintptr_t) ptrTestData, (uintptr_t) ptrDataLengthTestString);

    /* Copy over the Real Data: */
    memcpy ((void *)ptrTestData, (void *)&realData[0], sizeof(realData));

    /******************************************************************************************
     * Test Initialization:
     ******************************************************************************************/
    for (crcChannelNum = CRC_Channel_CH1; crcChannelNum <= CRC_Channel_CH2; crcChannelNum++)
    {
        /* Setup the configuration: */
        crcCfg.channel = crcChannelNum;
        crcCfg.dataLen = dataLen;
        crcCfg.type    = type;

        /* Open the CRC Driver */
        crcHandle[crcChannelNum] = CRC_open (&crcCfg, &errCode);
        if (crcHandle[crcChannelNum] == NULL)
        {
            Test_crcAppPrint2 ("Error: CRC Driver (Channel %d) open failed [Error code %d]\n", crcChannelNum, errCode);
            return -1;
        }
    }

	/******************************************************************************************
	 * Test the signatures
	 ******************************************************************************************/
	for (crcChannelNum = CRC_Channel_CH1; crcChannelNum <= CRC_Channel_CH2; crcChannelNum++)
	{
		/* Initialize the signature generation configuration */
		memset ((void *)&signGenCfg, 0, sizeof(CRC_SigGenCfg));

		/* Allocate a unique transaction id: */
		if (CRC_getTransactionId (crcHandle[crcChannelNum], &signGenCfg.transactionId, &errCode) < 0)
		{
			Test_crcAppPrint2 ("Error: CRC Driver (Channel %d) get transaction id failed [Error code %d]\n", crcChannelNum, errCode);
			return -1;
		}

		/* Populate the signature generation configuration: */
		signGenCfg.ptrData = (uint8_t*)ptrTestData;
		signGenCfg.dataLen = sizeof(realData);

		/* Compute the signature for the specific data on Channel-1 */
		if (CRC_computeSignature (crcHandle[crcChannelNum], &signGenCfg, &errCode) < 0)
		{
			Test_crcAppPrint2 ("Error: CRC Driver (Channel %d) compute signature failed [Error code %d]\n", crcChannelNum, errCode);
			return -1;
		}

		/* Get the Signature for Channel */
		if (CRC_getSignature (crcHandle[crcChannelNum], signGenCfg.transactionId, &signature, &errCode) < 0)
		{
			Test_crcAppPrint2 ("Error: CRC Driver (Channel %d) get signature failed [Error code %d]\n", crcChannelNum, errCode);
			return -1;
		}

		/* Debug Message: */
        if (type == CRC_Type_64BIT)
        {
            CRC_log("Debug: CRC Driver signature is 0x%016llx\n", signature);
        }
        else
        {
            Test_crcAppPrint1 ("Debug: CRC Driver signature is 0x%x\n", signature);
        }

		/******************************************************************************************
		 * Validate the signature(s)
		 ******************************************************************************************/
		switch (type)
		{
			case CRC_Type_16BIT:
			{
				if (CRC16Bit != signature)
				{
					Test_crcAppPrint2 ("Error: Mismatch in the CRC Detected Expected: 0x%x Got 0x%x\n", CRC16Bit, signature);
					return -1;
				}
				break;
			}
			case CRC_Type_32BIT:
			{
				if (CRC32Bit != signature)
				{
					Test_crcAppPrint2 ("Error: Mismatch in the CRC Detected Expected: 0x%x Got 0x%x\n", CRC32Bit, signature);
					return -1;
				}
				break;
			}
			case CRC_Type_64BIT:
			{
				if (CRC64Bit != signature)
				{
					CRC_log ("Error: Mismatch in the CRC Detected Expected: %016llx Got %016llx\n", CRC64Bit, signature);
					return -1;
				}
				break;
			}
			default:
			{
				Test_crcAppPrint1 ("Error: Invalid CRC Polynomial value [Type %d]\n", type);
				return -1;
			}
		}
	}

    /******************************************************************************************
     * Test Cleanup:
     ******************************************************************************************/
    for (crcChannelNum = CRC_Channel_CH1; crcChannelNum <= CRC_Channel_CH2; crcChannelNum++)
    {
        /* Close the CRC Channel: */
        if (CRC_close (crcHandle[crcChannelNum], &errCode) < 0)
        {
            Test_crcAppPrint2 ("Error: CRC Driver (Channel %d) close failed [Error code %d]\n", crcChannelNum, errCode);
            return -1;
        }
    }

    /* Debug Message: */
    Test_crcAppPrint2 ("Debug: Testing CRC Functionality Polynomial [%s] for Data Length %s Passed\n",
            (uintptr_t) ptrCRCTypeString, (uintptr_t) ptrDataLengthTestString);

    return 0;
}

void Test_crcAppPrint0(const char   *infoString)
{
    CRC_log(infoString);
}

void Test_crcAppPrint1(const char   *infoString,
                       uintptr_t     param0)
{
    CRC_log(infoString, param0);
}

void Test_crcAppPrint2(const char   *infoString,
                       uintptr_t     param0,
                       uintptr_t     param1)
{
    CRC_log(infoString, param0, param1);
}

void Test_crcAppPrint3(const char   *infoString,
                       uintptr_t     param0,
                       uintptr_t     param1,
                       uintptr_t     param2)
{
    CRC_log(infoString, param0, param1, param2);
}

