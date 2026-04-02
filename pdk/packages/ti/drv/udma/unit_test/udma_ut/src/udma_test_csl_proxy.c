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
 *  \file udma_test_csl_proxy.c
 *
 *  \brief File containing UDMA negative test cases for UDMA CSL PROXY API's
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

static void fpMemAccess(uintptr_t addr, uint8_t *pAppData, uint32_t elemSizeBytes, uint32_t elemCnt)
{
    return;
}

int32_t UdmaTestRingCSLProxy(UdmaTestTaskObj *taskObj)
{
    int32_t              retVal  = UDMA_SOK;
    uint32_t             elemCnt = 2U;
    uint32_t             heapId  = UTILS_MEM_HEAP_ID_MSMC;
    void                *ringMem = NULL;
    uint32_t             ringMode;
    uint32_t             ringMemSize;
    Udma_DrvHandle       drvHandle;
    Udma_RingPrms        ringPrms;
    struct Udma_RingObj  ringObj;
    struct Udma_ProxyObj proxyObj;
    Udma_RingHandle      ringHandle;
    Udma_ProxyHandle     proxyHandle;
    Udma_ProxyCfg        proxyCfg;
    CSL_ProxyCfg         cslProxyCfg;
    CSL_ProxyThreadCfg   threadCfg;
    uint32_t             backupbufferSizeBytes;

    ringMemSize = elemCnt * sizeof (uint64_t);
    ringMem     = Utils_memAlloc(heapId, ringMemSize, UDMA_CACHELINE_ALIGNMENT);
    if(NULL == ringMem)
    {
        retVal = UDMA_EALLOC;
        GT_0trace(taskObj->traceMask, GT_ERR, " Ring memory allocation failure\r\n");
    }

    if(UDMA_SOK == retVal)
    {
        ringHandle = &ringObj;
        ringMode   = TISCI_MSG_VALUE_RM_RING_MODE_RING;
        drvHandle  = &taskObj->testObj->drvObj[UDMA_TEST_INST_ID_MAIN_0];

        UdmaRingPrms_init(&ringPrms);
        ringPrms.ringMem     = ringMem;
        ringPrms.ringMemSize = ringMemSize;
        ringPrms.mode        = ringMode;
        ringPrms.elemCnt     = elemCnt;

        /* Allocate a free ring */
        retVal = Udma_ringAlloc(drvHandle, ringHandle, UDMA_RING_ANY, &ringPrms);
        if(UDMA_SOK != retVal)
        {
            GT_0trace(taskObj->traceMask, GT_ERR, " Ring alloc failed!!\n");
        }
        else
        {
            /* Allocate a proxy for queue operation */
            proxyHandle = &proxyObj;
            retVal      = Udma_proxyAlloc(drvHandle, proxyHandle, UDMA_PROXY_ANY);
            if(UDMA_SOK != retVal)
            {
                GT_0trace(taskObj->traceMask, GT_ERR, " Proxy alloc failed!!\n");
            }
            else
            {
                /* Config proxy for queue operation */
                proxyCfg.proxyMode = CSL_PROXY_QUEUE_ACCESS_MODE_TAIL;
                proxyCfg.elemSize  = UDMA_RING_ES_8BYTES;
                proxyCfg.ringNum   = Udma_ringGetNum(ringHandle);
                retVal             = Udma_proxyConfig(proxyHandle, &proxyCfg);
                if(UDMA_SOK != retVal)
                {
                    GT_0trace(taskObj->traceMask, GT_ERR, " Proxy config failed!!\n");
                }
                else
                {
                    if(UDMA_SOK == retVal)
                    {
                        /* Test scenario 1: CSL_proxyGetRevision API check */
                        GT_1trace(taskObj->traceMask, GT_INFO1,
                                  " |TEST INFO|:: Task:%d: Test CSL_proxyGetRevision ::\r\n",
                                  taskObj->taskId);
                        retVal = CSL_proxyGetRevision(&drvHandle->proxyCfg);
                        if(0 == retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyGetRevision:: pos:: "
                                      " CSL_proxyGetRevision API check!!\n");
                            retVal = UDMA_EFAIL;
                        }
                        else
                        {
                            retVal = UDMA_SOK;
                        }
                    }

                    if(UDMA_SOK == retVal)
                    {
                        /* Test scenario 2: CSL_proxyGetMaxMsgSize API check when targetNum is UDMA_PROXY_ANY */
                        GT_1trace(taskObj->traceMask, GT_INFO1,
                                  " |TEST INFO|:: Task:%d: Test CSL_proxyGetMaxMsgSize ::\r\n",
                                  taskObj->taskId);
                        retVal = CSL_proxyGetMaxMsgSize(&drvHandle->proxyCfg, UDMA_PROXY_ANY);
                        if(0 != retVal)
                        {
                            GT_0trace(taskObj->traceMask, GT_ERR,
                                      " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyGetMaxMsgSize:: neg:: "
                                      " CSL_proxyGetMaxMsgSize API check when targetNum is UDMA_PROXY_ANY!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 3: Validate CSL_proxyGetMaxMsgSize API check  */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyGetMaxMsgSize ::\r\n",
                              taskObj->taskId);
                    retVal = CSL_proxyGetMaxMsgSize(&drvHandle->proxyCfg, drvHandle->proxyTargetNumRing);
                    if(0 == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyGetMaxMsgSize:: pos:: "
                                  " CSL_proxyGetMaxMsgSize API check!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 4: Validate CSL_proxyGetMaxMsgSize API check when drvHandle->proxyCfg.bufferSizeBytes is UDMA_PROXY_ANY */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyGetMaxMsgSize ::\r\n",
                              taskObj->taskId);
                    backupbufferSizeBytes = drvHandle->proxyCfg.bufferSizeBytes;
                    drvHandle->proxyCfg.bufferSizeBytes = UDMA_PROXY_ANY;
                    retVal                              = CSL_proxyGetMaxMsgSize(&drvHandle->proxyCfg,
                                                                                 drvHandle->proxyTargetNumRing);
                    if(0 == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyGetMaxMsgSize:: n:: "
                                  " Validate CSL_proxyGetMaxMsgSize API check when drvHandle->proxyCfg.bufferSizeBytes"
                                  " is UDMA_PROXY_ANY!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                    drvHandle->proxyCfg.bufferSizeBytes = backupbufferSizeBytes;
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 5: Validate CSL_proxyAccessTarget API check */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyAccessTarget ::\r\n",
                              taskObj->taskId);
                    proxyHandle->proxyAddr = 0U;
                    fpMemAccess(proxyHandle->proxyAddr, NULL_PTR, ringMemSize, elemCnt);
                    CSL_proxyAccessTarget(&drvHandle->proxyCfg, drvHandle->proxyTargetNumRing,
                                          drvHandle->initPrms.rmInitPrms.proxyThreadNum, NULL_PTR,
                                          elemCnt, &fpMemAccess);
                    retVal = UDMA_SOK;
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 6: Validate CSL_proxyCfgThread API when elSz is UDMA_PROXY_ANY */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyCfgThread ::\r\n",
                              taskObj->taskId);
                    threadCfg.elSz = UDMA_PROXY_ANY;
                    retVal         = CSL_proxyCfgThread(&drvHandle->proxyCfg, drvHandle->proxyTargetNumRing,
                                                        drvHandle->initPrms.rmInitPrms.proxyThreadNum, &threadCfg);
                    if(0 == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyGetMaxMsgSize:: n:: "
                                  " Validate CSL_proxyCfgThread API when elSz is invalid!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 7: Validate CSL_proxyGetDataAddr when pProxyTargetParams is NULL */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyGetDataAddr ::\r\n",
                              taskObj->taskId);
                    
                    memset(&cslProxyCfg,0, sizeof(cslProxyCfg));

                    retVal = CSL_proxyGetDataAddr(&cslProxyCfg,0,drvHandle->initPrms.rmInitPrms.proxyThreadNum, elemCnt);
                    if(0 == retVal)
                    {
                        retVal = UDMA_SOK;
                    }
                    else
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyGetDataAddr:: neg:: "
                                  " Validate CSL_proxyGetDataAddr when pProxyTargetParams is NULL!!\n");
                        retVal = UDMA_EFAIL;
                    }
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 8: Validate CSL_proxyCfgThread API when elSz is less than
                     *                  pProxyCfg->numTargets
                     */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyCfgThread ::\r\n",
                              taskObj->taskId);
                    threadCfg.elSz = elemCnt;
                    retVal         = CSL_proxyCfgThread(&drvHandle->proxyCfg, drvHandle->proxyTargetNumRing,
                                                drvHandle->initPrms.rmInitPrms.proxyThreadNum, &threadCfg);
                    if(0 == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyCfgThread:: neg:: "
                                  " Validate CSL_proxyCfgThread API when elSz is less than "
                                  " pProxyCfg->numTargets!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 9: Validate CSL_proxyCfgThread API when elSz is invalid */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyCfgThread ::\r\n",
                              taskObj->taskId);
                    threadCfg.elSz = CSL_RINGACC_RING_MODE_INVALID + 1U;
                    retVal         = CSL_proxyCfgThread(&drvHandle->proxyCfg, drvHandle->proxyTargetNumRing,
                                                        drvHandle->initPrms.rmInitPrms.proxyThreadNum, &threadCfg);
                    if(0 == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyCfgThread:: neg:: "
                                  " Validate CSL_proxyCfgThread API when elSz is invalid!!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }

                if(UDMA_SOK == retVal)
                {
                    /* Test scenario 10: Validate CSL_proxyCfgThread API when queueNum is UDMA_PROXY_ANY */
                    GT_1trace(taskObj->traceMask, GT_INFO1,
                              " |TEST INFO|:: Task:%d: Test CSL_proxyCfgThread ::\r\n",
                              taskObj->taskId);
                    threadCfg.elSz     = CSL_RINGACC_RING_MODE_INVALID;
                    threadCfg.queueNum = UDMA_PROXY_ANY;
                    retVal = CSL_proxyCfgThread(&drvHandle->proxyCfg, drvHandle->proxyTargetNumRing,
                                                drvHandle->initPrms.rmInitPrms.proxyThreadNum, &threadCfg);
                    if(0 == retVal)
                    {
                        GT_0trace(taskObj->traceMask, GT_ERR,
                                  " |TEST INFO|:: FAIL:: UDMA:: CSL_proxyCfgThread:: neg:: "
                                  " Validate CSL_proxyCfgThread API when queueNum is UDMA_PROXY_ANY !!\n");
                        retVal = UDMA_EFAIL;
                    }
                    else
                    {
                        retVal = UDMA_SOK;
                    }
                }

             }
          }
            Udma_proxyFree(proxyHandle);
        }
        Udma_ringFree(ringHandle);
    }

    return retVal;
}
