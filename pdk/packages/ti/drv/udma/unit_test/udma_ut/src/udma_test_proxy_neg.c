/*
 *  Copyright (c) Texas Instruments Incorporated 2023-2024
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
 *  \file udma_test_proxy_neg.c
 *
 *  \brief File containing UDMA negative test cases for UDMA proxy APIs..
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
 * Test Case Description: Verifies the function Udma_proxyAlloc for
 * 1)Test scenario 1: NULL Check for proxyHandle and driver handle.
 * 2)Test scenario 2: NULL Check for driver handle.
 * 3)Test scenario 3: NULL check for proxyHandle.
 * 4)Test scenario 4: Check for drvInitDone when it is not equal to UDMA_INIT_DONE.
 * 5)Test scenario 5: Check to print [Error] Out of range proxy index when proxyNum
 *                    is UDMA_PROXY_INVALID.
 * 6)Test scenario 6: Check when maxProxy is greater than proxyNum.
 * 7)Test scenario 7: Check when proxyNum is UDMA_PROXY_ANY and to get error from proxy RM API's.
 * 8)Test scenario 8: Check to get [Error] SciClient Set proxy config failed when proxyNum
 *                    is UDMA_PROXY_ANY.
 */
int32_t UdmaTest_proxyAllocNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal   = UDMA_SOK;
    uint16_t             proxyNum = UDMA_PROXY_ANY;
    struct Udma_ProxyObj proxyObj;
    struct Udma_DrvObj   backUpDrvObj;
    Udma_ProxyHandle     proxyHandle;
    Udma_DrvHandle       drvHandle;
    uint32_t             backUpDrvInitDone;
    uint32_t             backUpMaxproxy;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA proxy alloc negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: NULL Check for proxyHandle and driver handle */
    drvHandle   = (Udma_DrvHandle) NULL_PTR;
    proxyHandle = (Udma_ProxyHandle) NULL_PTR;
    retVal      = Udma_proxyAlloc(drvHandle, proxyHandle, proxyNum);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                  " NULL Check for proxyHandle and driver handle!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: NULL Check for driver handle */
        drvHandle   = (Udma_DrvHandle) NULL_PTR;
        proxyHandle = &proxyObj;
        retVal      = Udma_proxyAlloc(drvHandle, proxyHandle, proxyNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                      " NULL Check for driver handle!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: NULL check for proxyHandle */
        drvHandle   = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        proxyHandle = (Udma_ProxyHandle) NULL_PTR;
        retVal      = Udma_proxyAlloc(drvHandle, proxyHandle, proxyNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                      " NULL check for proxyHandle!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check for drvInitDone when it is not equal to UDMA_INIT_DONE */
        proxyHandle            = &proxyObj;
        backUpDrvInitDone      = drvHandle->drvInitDone;
        drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                 = Udma_proxyAlloc(drvHandle, proxyHandle, proxyNum);
        if(UDMA_SOK != retVal)
        {
             retVal = UDMA_SOK;
        }
        else
        {
             GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                       " Check for drvInitDone when it is not equal to UDMA_INIT_DONE!!\n");
             retVal = UDMA_EFAIL;
        }
        drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check to print [Error] Out of range proxy index when proxyNum is
         *                  UDMA_PROXY_INVALID
         */
        proxyNum = UDMA_PROXY_INVALID;
        retVal   = Udma_proxyAlloc(drvHandle, proxyHandle, proxyNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                      " Check to print [Error] Out of range proxy index when proxyNum is"
                      " UDMA_PROXY_INVALID!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check when maxProxy is greater than proxyNum */
        backUpMaxproxy      = drvHandle->maxProxy;
        drvHandle->maxProxy = UDMA_PROXY_INVALID + 1U;
        retVal              = Udma_proxyAlloc(drvHandle, proxyHandle, proxyNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                      " Check when maxProxy is greater than proxyNum!!\n");
            retVal = UDMA_EFAIL;
        }
        drvHandle->maxProxy = backUpMaxproxy;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 7: Check when proxyNum is UDMA_PROXY_ANY and to get error from proxy RM API's */
        backUpDrvObj                            = taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        drvHandle                               = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        proxyNum                                = UDMA_PROXY_ANY;
        drvHandle->initPrms.rmInitPrms.numProxy = 0U;
        proxyHandle->proxyNum                   = UDMA_PROXY_INVALID;
        retVal                                  = Udma_proxyAlloc(drvHandle, proxyHandle, proxyNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                      " Check when proxyNum is UDMA_PROXY_ANY and to get error from proxy RM API's!!\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0] = backUpDrvObj;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 8: Check to get [Error] SciClient Set proxy config failed when proxyNum
         *                  is UDMA_PROXY_ANY
         */
        backUpDrvObj                            = taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        drvHandle                               = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        proxyNum                                = UDMA_PROXY_ANY;
        drvHandle->devIdProxy                   = UDMA_PROXY_INVALID;
        drvHandle->initPrms.rmInitPrms.numProxy = 1U;
        retVal                                  = Udma_proxyAlloc(drvHandle, proxyHandle,
                                                                  proxyNum);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Alloc:: Neg::"
                      " Check to get [Error] SciClient Set proxy config failed when proxyNum"
                      " is UDMA_PROXY_ANY\n");
            retVal = UDMA_EFAIL;
        }
        taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0] = backUpDrvObj;
    }

    return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_proxyFree for
 * 1)Test scenario 1: NULL check for proxyHandle.
 * 2)Test scenario 2: Check for proxyInitDone when it is not equal to UDMA_INIT_DONE.
 * 3)Test scenario 3: NULL check for drvHandle.
 * 4)Test scenario 4: Check for drvInitDone when it is not equal to UDMA_INIT_DONE.
 * 5)Test scenario 5: Check for instType when it is not equal to UDMA_INST_TYPE_NORMAL.
 */
int32_t UdmaTest_proxyFreeNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    struct Udma_ProxyObj proxyObj;
    Udma_ProxyHandle     proxyHandle;
    uint32_t             backUpDrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA proxy free negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: NULL check for proxyHandle */
    proxyHandle = (Udma_ProxyHandle) NULL_PTR;
    retVal      = Udma_proxyFree(proxyHandle);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Free:: Neg::"
                  " NULL check for ProxyHandle!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check for proxyInitDone when it is not equal UDMA_INIT_DONE */
        proxyHandle                = &proxyObj;
        proxyHandle->proxyInitDone = UDMA_DEINIT_DONE;
        retVal                     = Udma_proxyFree(proxyHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Free:: Neg::"
                      " Check for proxyInitDone when it is not equal UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: NULL check for driver Handle */
        proxyHandle->proxyInitDone = UDMA_INIT_DONE;
        proxyHandle->drvHandle     = NULL_PTR;
        retVal                     = Udma_proxyFree(proxyHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Free:: Neg::"
                      " NULL check for driver handle!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check for drvInitDone when it is not equal to UDMA_INIT_DONE */
        proxyHandle->drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        backUpDrvInitDone                   = proxyHandle->drvHandle->drvInitDone;
        proxyHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                              = Udma_proxyFree(proxyHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Free:: Neg::"
                      " Check for drvInitDone when it is not equal to UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        proxyHandle->drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check for instType when it is not equal to UDMA_INST_TYPE_NORMAL */
        backUpDrvInitDone                   = proxyHandle->drvHandle->drvInitDone;
        proxyHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
        proxyHandle->drvHandle->instType    = UDMA_INST_TYPE_LCDMA_BCDMA;
        retVal                              = Udma_proxyFree(proxyHandle);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Free:: Neg::"
                      " Check for instType when it is not equal to UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
        proxyHandle->drvHandle->drvInitDone = backUpDrvInitDone;
        proxyHandle->drvHandle->instType    = UDMA_INST_TYPE_NORMAL;
    }

    return (retVal);
}

/*
 * Test Case Description: Verifies the function Udma_proxyConfig for
 * 1)Test scenario 1: NULL check for proxyHandle.
 * 2)Test scenario 2: Check when proxyInitDone when it is not equal to UDMA_DEINIT_DONE.
 * 3)Test scenario 3: NULL check for driver handle.
 * 4)Test scenario 4: Check for drvInitDone when it is not equal to UDMA_DEINIT_DONE.
 * 5)Test scenario 5: Check when instType when it is not equal to UDMA_INST_TYPE_NORMAL.
 * 6)Test scenario 6: Check to  print "[Error] Proxy config failed.
 */
int32_t UdmaTest_proxyConfigNeg(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal = UDMA_SOK;
    struct Udma_ProxyObj proxyObj;
    Udma_ProxyHandle     proxyHandle;
    Udma_ProxyCfg        proxyCfg;
    uint32_t             backUpDrvInitDone;

    GT_1trace(taskObj->traceMask, GT_INFO1,
              " |TEST INFO|:: Task:%d: UDMA proxy Config negative Testcase ::\r\n",
              taskObj->taskId);

    /* Test scenario 1: NULL check for proxyHandle */
    proxyHandle = (Udma_ProxyHandle) NULL_PTR;
    retVal      = Udma_proxyConfig(proxyHandle, &proxyCfg);
    if(UDMA_SOK != retVal)
    {
        retVal = UDMA_SOK;
    }
    else
    {
        GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Config:: Neg::"
                  " NULL check for ProxyHandle!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 2: Check for proxyInitDone when it is not equal to UDMA_INIT_DONE */
        proxyHandle                = &proxyObj;
        proxyHandle->proxyInitDone = UDMA_DEINIT_DONE;
        retVal                     = Udma_proxyConfig(proxyHandle, &proxyCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Config:: Neg::"
                      " Check for proxyInitDone when it is not equal to UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 3: NULL Check for driver handle */
        proxyHandle->proxyInitDone = UDMA_INIT_DONE;
        proxyHandle->drvHandle     = NULL_PTR;
        retVal                     = Udma_proxyConfig(proxyHandle, &proxyCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Config:: Neg::"
                      " NULL Check for driver handle!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 4: Check for drvInitDone when it is not equal to UDMA_INIT_DONE */
        proxyHandle->drvHandle              = &taskObj->testObj->drvObj[UDMA_INST_ID_MAIN_0];
        backUpDrvInitDone                   = proxyHandle->drvHandle->drvInitDone;
        proxyHandle->drvHandle->drvInitDone = UDMA_DEINIT_DONE;
        retVal                              = Udma_proxyConfig(proxyHandle, &proxyCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Config:: Neg::"
                      " Check for drvInitDone when it is not equal to UDMA_INIT_DONE!!\n");
            retVal = UDMA_EFAIL;
        }
        proxyHandle->drvHandle->drvInitDone = backUpDrvInitDone;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 5: Check for instType when it is not equal to UDMA_INST_TYPE_NORMAL */
        backUpDrvInitDone                   = proxyHandle->drvHandle->drvInitDone;
        proxyHandle->drvHandle->drvInitDone = UDMA_INIT_DONE;
        proxyHandle->drvHandle->instType    = UDMA_INST_TYPE_LCDMA_BCDMA;
        retVal                              = Udma_proxyConfig(proxyHandle, &proxyCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Config:: Neg::"
                      " Check for instType when it is not equal to UDMA_INST_TYPE_NORMAL!!\n");
            retVal = UDMA_EFAIL;
        }
        proxyHandle->drvHandle->drvInitDone = backUpDrvInitDone;
        proxyHandle->drvHandle->instType    = UDMA_INST_TYPE_NORMAL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Test scenario 6: Check to  print [Error] Proxy config failed!! */
        proxyHandle->proxyNum = UDMA_PROXY_INVALID;
        retVal                = Udma_proxyConfig(proxyHandle, &proxyCfg);
        if(UDMA_SOK != retVal)
        {
            retVal = UDMA_SOK;
        }
        else
        {
            GT_0trace(taskObj->traceMask, GT_ERR," |TEST INFO|:: FAIL:: UDMA:: Proxy Config:: Neg::"
                      " Check to  print [Error] Proxy config failed!!\n");
            retVal = UDMA_EFAIL;
        }
    }

    return (retVal);
}

