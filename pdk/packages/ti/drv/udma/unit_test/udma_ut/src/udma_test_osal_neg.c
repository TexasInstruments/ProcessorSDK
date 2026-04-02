/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 *  \file udma_test_osal_neg.c
 *
 *  \brief File containing negative test cases for UDMA Osal APIs.
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
 * Test Case Description: Verifies the function Udma_osalSetCachePrms
 * 1)Test scenario 1: NULL Check for cachePrms.
 * 2)Test scenario 2: NULL Check when cacheWb and cacheInv are NULL.
 * 3)Test scenario 3: NULL Check for cacheInv.
 * 4)Test scenario 4: NULL Check for cacheWb.
 */
int32_t UdmaTestOsalSetCachePrmsNeg(UdmaTestTaskObj *taskObj)
{
    int32_t            retVal = UDMA_SOK;
    Udma_OsalCachePrms cachePrms;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA OsalSetCachePrms negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: NULL Check for cachePrms */
    retVal = Udma_osalSetCachePrms(NULL);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " OsalSetCachePrms:: Neg:: NULL Check for cachePrms!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: NULL Check for cacheWb and cacheInv */
        UdmaOsalCachePrms_init(&cachePrms);
        cachePrms.cacheWb  = NULL_PTR;
        cachePrms.cacheInv = NULL_PTR;
        retVal             = Udma_osalSetCachePrms(&cachePrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " OsalSetCachePrms:: Neg:: NULL Check when cacheWb and cacheInv!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: NULL Check for cacheInv */
        cachePrms.cacheWb  = &CacheP_wb;
        cachePrms.cacheInv = NULL_PTR;
        retVal             = Udma_osalSetCachePrms(&cachePrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " OsalSetCachePrms:: Neg:: NULL Check for cacheInv!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: NULL Check for cacheWb */
        cachePrms.cacheInv = &CacheP_Inv;
        cachePrms.cacheWb  = NULL_PTR;
        retVal             = Udma_osalSetCachePrms(&cachePrms);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                      " OsalSetCachePrms:: Neg:: NULL Check for cacheWb!!\n");
            retVal = UDMA_EFAIL;
        }
    }

   return retVal;
}

/*
 * Test Case Description: Verifies the functions when
 * 1)Test scenario 1: Check to get hwiHandle NULL by passing invalid parameters to the
 *                    function Udma_osalRegisterIntr
 * 2)Test scenario 2: Check lockMutex and unlockMutex when mutexHandle is NULL 
 */
int32_t UdmaTestOsalNeg(UdmaTestTaskObj *taskObj)
{
    int32_t        retVal = UDMA_SOK;
    Udma_OsalPrms  osalPrms;
    Udma_DrvHandle drvHandle;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA Udma_osalRegisterIntr negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: Check to get hwiHandle NULL by passing invalid parameters to the
     *                    function Udma_osalRegisterIntr
     */
    UdmaOsalPrms_init(&osalPrms);
    if(osalPrms.registerIntr(NULL_PTR, UDMA_INTR_INVALID, 1, NULL_PTR) == NULL_PTR)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA::"
                  " Udma_osalRegisterIntr:: Neg:: Check to get hwiHandle NULL by passing invalid"
                  " parameters to the function Udma_osalRegisterIntr!!\n");
        retVal = UDMA_EFAIL;
    }

    /* Test scenario 2: Check lockMutex and unlockMutex when mutexHandle is NULL */
    drvHandle = &taskObj->testObj->drvObj[UDMA_TEST_DEFAULT_UDMA_INST];
    if(UDMA_SOK == retVal)
    {
        drvHandle->initPrms.osalPrms.lockMutex(NULL_PTR);

        drvHandle->initPrms.osalPrms.unlockMutex(NULL_PTR);
    }

    return retVal;
}

