/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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
 *  \file udma_test_utils_neg.c
 *
 *  \brief UDMA Utils negative test cases.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <udma_test.h>

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

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Test Case Description: Verifies the function Udma_virtToPhyFxn
 * when an input parameter virtToPhyFxn is NULL_PTR.
 *
 * */
int32_t udmaTestUdmaVirtToPhyNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t retVal           = UDMA_SOK;
    uint32_t instId          = 0U;
    void *pVirtAddr          = NULL_PTR;
    uint64_t retPhyAddr      = 0U;
    UdmaTestObj *testObj     = taskObj->testObj;
    Udma_DrvHandle drvHandle = (Udma_DrvHandle) NULL_PTR;
    struct Udma_ChObj chObj;
    Udma_ChHandle chHandle   = &chObj;

    GT_1trace(taskObj->traceMask, GT_INFO1,
    	      " |TEST INFO|:: Task:%d:: Udma_virtToPhyFxn verification testcase when virtToPhyFxn is NULL_PTR\r\n", taskObj->taskId);
 
    /* Deinit the drivers */
    retVal = udmaTestDeinitDriver(testObj);
    if(UDMA_SOK == retVal)
    {
        for(instId = UDMA_INST_ID_START; instId < UDMA_TEST_INST_ID_MAX; instId++)
		{
		    drvHandle = &testObj->drvObj[instId];
        	drvHandle->initPrms.virtToPhyFxn = (Udma_VirtToPhyFxn) NULL_PTR;
    
        	/*Test Case: Verifies Udma_defaultVirtToPhyFxn in else branch*/
        	retPhyAddr = Udma_virtToPhyFxn(&pVirtAddr, drvHandle, chHandle);
        	if((uint64_t)&pVirtAddr == retPhyAddr)
        	{
          		GT_1trace(testObj->traceMask, GT_INFO1,
						  " |TEST INFO|:: Test Instance: %d:: Pass\r\n", instId);
      		}
      		else
      		{
        		GT_1trace(testObj->traceMask, GT_ERR,
						  " |TEST INFO|:: Test Instance: %d:: Failed\r\n", instId);
      		}
    	}
  	}

    /*Reinit the driver*/
	retVal = udmaTestInitDriver(testObj);
	return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_phyToVirtFxn
 * when an input parameter virtToPhyFxn is NULL_PTR.
 * 
 * */
int32_t udmaTestUdmaPhyToVirtNegTc(UdmaTestTaskObj *taskObj)
{
    int32_t retVal           = UDMA_SOK;
	uint32_t instId          = 0U;
	uint64_t phyAddr         = 0U;
	void *pRetVirtAddr       = NULL_PTR;
	UdmaTestObj *testObj     = taskObj->testObj;
	Udma_DrvHandle drvHandle = (Udma_DrvHandle) NULL_PTR;
	struct Udma_ChObj chObj;
	Udma_ChHandle chHandle   = &chObj;

	GT_1trace(taskObj->traceMask, GT_INFO1,
			  " |TEST INFO|:: Task:%d:: Udma_phyToVirtFxn verification testcase when virtToPhyFxn is NULL_PTR\r\n", taskObj->taskId);

	/* Deinit the drivers */
	retVal = udmaTestDeinitDriver(testObj);
	if(UDMA_SOK == retVal)
	{
		for(instId = UDMA_INST_ID_START; instId <= UDMA_TEST_INST_ID_MAX; instId++)
		{
			drvHandle = &testObj->drvObj[instId];
			drvHandle->initPrms.virtToPhyFxn = (Udma_VirtToPhyFxn) NULL_PTR;

			/*Test Case: Execute Udma_defaultPhyToVirtFxn in else branch*/
			pRetVirtAddr = Udma_phyToVirtFxn((uint64_t)&phyAddr, drvHandle, chHandle);
			if(&phyAddr == pRetVirtAddr)
			{
				GT_1trace(testObj->traceMask, GT_INFO1,
						  " |TEST INFO|:: Test Instance: %d:: Pass\r\n", instId);
			}
			else
			{
				GT_1trace(testObj->traceMask, GT_ERR,
						  " |TEST INFO|:: Test Instance: %d:: Failed\r\n", instId);
			}
		}
	}

	/*Reinit the driver*/
	retVal = udmaTestInitDriver(testObj);
	return (retVal);
}

/*
 * Test Case Description: Verifies the function UdmaUtils_getTrSizeBytes
 * and UdmaUtils_getTrSizeEncoded for an invalid TR Type
 * 
 * */
int32_t udmaTestUdmaGetTrSizeNegTc(UdmaTestTaskObj *taskObj)
{
	int32_t retVal         = UDMA_SOK;
	uint32_t trSize        = 0U;
	uint32_t trSizeEncoded = 0U;
	uint32_t trType        = UDMA_TR_TYPE_15 + 1U; /* Use Invalid TR type*/
	UdmaTestObj *testObj   = taskObj->testObj;

	GT_1trace(taskObj->traceMask, GT_INFO1,
		      " |TEST INFO|:: Task:%d:: Invalid TR type verifcation test case \r\n", taskObj->taskId);

	/* Deinit the drivers */
	retVal = udmaTestDeinitDriver(testObj);
	if(UDMA_SOK == retVal)
	{
		trSize        = UdmaUtils_getTrSizeBytes(trType);
	    trSizeEncoded = UdmaUtils_getTrSizeEncoded(trType);
	    GT_2trace(taskObj->traceMask, GT_INFO1,
				  " |TEST INFO|:: TR15 Size: %d Bytes (%db Encoded)\r\n", trSize, trSizeEncoded);
	}

	/*Reinit the driver*/
	retVal = udmaTestInitDriver(testObj);
	return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_printf
 * for an invalid drvHandle as input.
 * 
 * */
int32_t udmaTestUdmaPrinfNegTc(UdmaTestTaskObj *taskObj)
{
	int32_t retVal           = UDMA_SOK;
	uint32_t instId          = 0U;
	UdmaTestObj *testObj     = taskObj->testObj;
	const char *format       = "Msg";
	Udma_DrvHandle drvHandle = (Udma_DrvHandle) NULL_PTR;

	GT_1trace(taskObj->traceMask, GT_INFO1,
		      " |TEST INFO|:: Task:%d:: Udma_printf API testcase for an invalid drvHandle\r\n", taskObj->taskId);

	/* Deinit the drivers */
	retVal = udmaTestDeinitDriver(testObj);
	if(UDMA_SOK == retVal)
	{
		for(instId = UDMA_INST_ID_START; instId <= UDMA_TEST_INST_ID_MAX; instId++)
		{
			/*Test case: drvHandle Null check*/
			drvHandle = (Udma_DrvHandle) NULL_PTR;
			Udma_printf(drvHandle, format);

			/*Test case: drvHandle is not NULL_PTR */
			drvHandle = &testObj->drvObj[instId];
			drvHandle->initPrms.printFxn = (Udma_PrintFxn ) NULL_PTR;
			Udma_printf(drvHandle, format);
		}
	}

	/*Reinit the driver*/
	retVal = udmaTestInitDriver(testObj);
	return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_printf
 * when lockMutex and unlockMutex are NULL.
 * 
 * */
int32_t udmaTestUdmaPrinftMutexNegTc(UdmaTestTaskObj *taskObj)
{
	int32_t retVal           =  UDMA_SOK;
	uint32_t instId          =  0U;
	const char *format       = "Msg";
	Udma_DrvHandle drvHandle = (Udma_DrvHandle) NULL_PTR;;
	UdmaTestObj *testObj     =  taskObj->testObj;
	Udma_InitPrms initPrms;

	GT_1trace(taskObj->traceMask, GT_INFO1,
	          " |TEST INFO|:: Task:%d: Udma_printf API testcase when lockMutex and unlockMutex are NULL_PTR\r\n", taskObj->taskId);

	/* Deinit the drivers */
	retVal = udmaTestDeinitDriver(testObj);
	if(UDMA_SOK == retVal)
	{
		for(instId = UDMA_INST_ID_START; instId <= UDMA_TEST_INST_ID_MAX; instId++)
		{
			drvHandle = &testObj->drvObj[instId];
			UdmaInitPrms_init(instId, &initPrms);
			drvHandle->initPrms.printFxn             = &udmaDrvPrint;
			drvHandle->initPrms.osalPrms.lockMutex   = (Udma_OsalMutexLockFxn) NULL_PTR; 
			drvHandle->initPrms.osalPrms.unlockMutex = (Udma_OsalMutexUnlockFxn) NULL_PTR;
			Udma_printf(drvHandle, format);
		}
	}

	/* Re-init driver with default params */
	retVal = udmaTestInitDriver(testObj);
	return (retVal);
}

