/**
 *  \file   fsi_app_common.c
 *
 *  \brief  FSI appliction common driver
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "fsi_app_common.h"

/*
 * Initializes FSI Tx/Rx and sends FLUSH sequence
 */
static int32_t FSI_APP_initFSI(FSI_APP_Object *pObj)
{
    int32_t  status = CSL_PASS;
    uint32_t i;
    /*
     * Initialize Tx/Rx, reset sequence, clear events
     */
    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_performTxInitialization(pObj->txBaseAddr[i],
                                                     (uint16_t)(pObj->txPreScaler));
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_performRxInitialization(pObj->rxBaseAddr[i]);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    /*
     * Set internalLoopback mode
     */
    if ((status == CSL_PASS) && (pObj->lbEnable == TRUE))
    {
        /*
         * TX0 can be looped back to RX0/RX1/RX2,
         * TX1 can be looped back to RX3/RX4/RX5
         */
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_enableRxInternalLoopback(pObj->rxBaseAddr[i]);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    /*
     * Flush Sequence before and after releasing Rx core reset, ensures flushing
     * of Rx data/clock lines and prepares it for reception
     */
    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_resetRxModule(pObj->rxBaseAddr[i], FSI_RX_MASTER_CORE_RESET);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_executeTxFlushSequence(pObj->txBaseAddr[i],
                                                    (uint16_t)(pObj->txPreScaler));
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSIRX_MAIN_CNT; i++)
        {
            if (pObj->rxBaseAddr[i] != 0U)
            {
                status = FSI_clearRxModuleReset(pObj->rxBaseAddr[i], FSI_RX_MASTER_CORE_RESET);
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    if (status == CSL_PASS)
    {
        for (i = 0; i < CSL_FSITX_MAIN_CNT; i++)
        {
            if (pObj->txBaseAddr[i] != 0U)
            {
                status = FSI_executeTxFlushSequence(pObj->txBaseAddr[i],
                                                    (uint16_t)(pObj->txPreScaler));
                if (status != CSL_PASS)
                {
                    break;
                }
            }
        }
    }

    return status;
}

/*
 * FSI Application Initialization API
 */
int32_t FSI_App_init(FSI_APP_Object *pObj)
{
    int32_t status = FSI_APP_STATUS_ERROR;

    if (pObj != NULL)
    {
        memset(pObj, 0, sizeof(FSI_APP_Object));
        status = FSI_APP_STATUS_SUCCESS;
    }

    return status;
}

/*
 * FSI Application Open API
 */
int32_t FSI_App_open(FSI_APP_Object *pObj)
{
    int32_t                  status = FSI_APP_STATUS_ERROR;
    uint32_t                 i, j;
    OsalRegisterIntrParams_t intRegParams;
    OsalInterruptRetCode_e   osalRet;

    if ((pObj != NULL)                   &&
        ((pObj->txBaseAddr[0] != 0U) ||
         (pObj->txBaseAddr[1] != 0U))    &&
        ((pObj->rxBaseAddr[0] != 0U) ||
         (pObj->rxBaseAddr[1] != 0U) ||
         (pObj->rxBaseAddr[2] != 0U) ||
         (pObj->rxBaseAddr[3] != 0U) ||
         (pObj->rxBaseAddr[4] != 0U) ||
         (pObj->rxBaseAddr[5] != 0U)))
    {
        status = FSI_APP_STATUS_SUCCESS;

        if (pObj->intEnable == TRUE)
        {
            /* Register FSI TX interrupts */
            for (i = 0U; i < FSI_APP_MAX_NUM_INTS; i++)
            {
                for (j = 0U; j < CSL_FSITX_MAIN_CNT; j++)
                {
                    if ((pObj->txIsr[j][i] != NULL) &&
                        (status == FSI_APP_STATUS_SUCCESS))
                    {
                        /* Initialize with defaults */
                        Osal_RegisterInterrupt_initParams(&intRegParams);
                        intRegParams.corepacConfig.name = NULL;
                        intRegParams.corepacConfig.corepacEventNum = (int32_t)0U;

                        /* Populate the interrupt parameters */
                        intRegParams.corepacConfig.arg = (uintptr_t)pObj;
                        intRegParams.corepacConfig.isrRoutine = pObj->txIsr[j][i];
                        intRegParams.corepacConfig.intVecNum = (int32_t)(pObj->txIntNum[j][i]);
                        osalRet = Osal_RegisterInterrupt(&intRegParams, &(pObj->txHwi[j][i]));
                        if (osalRet != OSAL_INT_SUCCESS)
                        {
                            FSI_log("\n Error %d registering TX (0x%8x) interrupt %d \n",
                                    osalRet,
                                    pObj->txBaseAddr[j],
                                    i + 1U);
                            status = FSI_APP_STATUS_ERROR;
                            break;
                        }
                    }
                }
            }

            /* Register FSI RX interrupts */
            for (i = 0U; i < FSI_APP_MAX_NUM_INTS; i++)
            {
                for (j = 0U; j < CSL_FSIRX_MAIN_CNT; j++)
                {
                    if ((pObj->rxIsr[j][i] != NULL) &&
                        (status == FSI_APP_STATUS_SUCCESS))
                    {
                        /* Initialize with defaults */
                        Osal_RegisterInterrupt_initParams(&intRegParams);
                        intRegParams.corepacConfig.name = NULL;
                        intRegParams.corepacConfig.corepacEventNum = (int32_t)0U;

                        /* Populate the interrupt parameters */
                        intRegParams.corepacConfig.arg = (uintptr_t)pObj;
                        intRegParams.corepacConfig.isrRoutine = pObj->rxIsr[j][i];
                        intRegParams.corepacConfig.intVecNum = (int32_t)(pObj->rxIntNum[j][i]);
                        osalRet = Osal_RegisterInterrupt(&intRegParams, &(pObj->rxHwi[j][i]));
                        if (osalRet != OSAL_INT_SUCCESS)
                        {
                            FSI_log("\n Error %d registering RX (0x%8x) interrupt %d \n",
                                    osalRet,
                                    pObj->rxBaseAddr[j],
                                    i + 1U);
                            status = FSI_APP_STATUS_ERROR;
                            break;
                        }
                    }
                }
                if (status == FSI_APP_STATUS_ERROR)
                {
                    break;
                }
            }
        }

        if (status == FSI_APP_STATUS_SUCCESS)
        {
            status = FSI_APP_initFSI(pObj);
        }
    }

    return status;
}

/*
 * FSI Application Close API
 */
int32_t FSI_App_close(FSI_APP_Object *pObj)
{
    int32_t                  status = FSI_APP_STATUS_ERROR;
    uint32_t                 i, j;
    OsalInterruptRetCode_e   osalRet = OSAL_INT_SUCCESS;

    if ((pObj != NULL)                   &&
        ((pObj->txBaseAddr[0] != 0U) ||
         (pObj->txBaseAddr[1] != 0U))    &&
        ((pObj->rxBaseAddr[0] != 0U) ||
         (pObj->rxBaseAddr[1] != 0U) ||
         (pObj->rxBaseAddr[2] != 0U) ||
         (pObj->rxBaseAddr[3] != 0U) ||
         (pObj->rxBaseAddr[4] != 0U) ||
         (pObj->rxBaseAddr[5] != 0U)))
    {
        status = FSI_APP_STATUS_SUCCESS;

        if (pObj->intEnable == TRUE)
        {
            /* Unregister FSI TX interrupts */
            for (i = 0U; i < FSI_APP_MAX_NUM_INTS; i++)
            {
                for (j = 0U; j < CSL_FSITX_MAIN_CNT; j++)
                {
                    if (pObj->txHwi[j][i] != NULL)
                    {

                        osalRet = Osal_DeleteInterrupt(pObj->txHwi[j][i], (int32_t)0U);
                        if (osalRet != OSAL_INT_SUCCESS)
                        {
                            FSI_log("\n Error %d deleting TX (0x%8x) hwi %d \n",
                                    osalRet,
                                    pObj->txBaseAddr[j],
                                    i + 1U);
                            status = FSI_APP_STATUS_ERROR;
                        }
                    }
                }
            }

            /* Unregister FSI RX interrupts */
            for (i = 0U; i < FSI_APP_MAX_NUM_INTS; i++)
            {
                for (j = 0U; j < CSL_FSIRX_MAIN_CNT; j++)
                {
                    if (pObj->rxHwi[j][i] != NULL)
                    {
                        osalRet = Osal_DeleteInterrupt(pObj->rxHwi[j][i], (int32_t)0U);
                        if (osalRet != OSAL_INT_SUCCESS)
                        {
                            FSI_log("\n Error %d deleting RX (0x%8x) hwi %d \n",
                                    osalRet,
                                    pObj->rxBaseAddr[j],
                                    i + 1U);
                            status = FSI_APP_STATUS_ERROR;
                        }
                    }
                }
            }
        }
    }

    return (status);
}
