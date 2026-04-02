/* Copyright (c) Texas Instruments Incorporated 2019-2020
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
 *  \file     ecc_aggr_apiTest.c
 *
 *  \brief    This file contains ECC Aggregator API test code.
 *
 *  \details  ECC Aggregator API tests
 **/
/*===========================================================================*/
/*                         Include files                                     */
/*===========================================================================*/
#include <ti/csl/csl_ecc_aggr.h>
#include "ecc_aggr_test.h"

/*===========================================================================*/
/*                         Declarations                                      */
/*===========================================================================*/
/* None */

/*===========================================================================*/
/*                         Macros                                            */
/*===========================================================================*/
/* None */

/*===========================================================================*/
/*                         Internal function declarations                    */
/*===========================================================================*/

/*===========================================================================*/
/*                         Function definitions                              */
/*===========================================================================*/

int32_t cslEccAggr_apiTest(void)
{
    CSL_ecc_aggrRegs      *pEccAggrRegs = ((CSL_ecc_aggrRegs *)((uintptr_t)CSL_TEST_ECC_AGGR_BASE));
    CSL_Ecc_AggrEccRamErrorStatusInfo  eccRamErrorStatus;
    CSL_Ecc_AggrErrorInfo  eccErrorInfo;
    CSL_Ecc_AggrEDCInterconnectErrorStatusInfo  edcInterconnectErrorStatus;
    CSL_Ecc_AggrEDCInterconnectErrorInfo  eccEDCInterconnectErrorInfo;
    CSL_ecc_aggrStaticRegs eccStaticRegs;
    int32_t                testStatus = CSL_APP_TEST_PASS;
    uint32_t               val;
    bool                   isPend;

    /* CSL_ecc_aggrGetRevision API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetRevision(pEccAggrRegs, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrGetNumRams API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetNumRams(pEccAggrRegs, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrReadEccRamReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, 0U, CSL_ECC_RAM_WRAP_REV, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrReadEDCInterconnectReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEDCInterconnectReg(pEccAggrRegs, 0U, CSL_ECC_RAM_WRAP_REV, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrReadEccRamWrapRevReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamWrapRevReg(pEccAggrRegs, 0U, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrReadEccRamCtrlReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamCtrlReg(pEccAggrRegs, 0U, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrReadEccRamErrCtrlReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrCtrlReg(pEccAggrRegs, 0U, 0U, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrReadEccRamErrStatReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrStatReg(pEccAggrRegs, 0U, 0U, &val) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrWriteEccRamReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamReg(pEccAggrRegs, 0U, CSL_ECC_RAM_WRAP_REV, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrWriteEDCInterconnectReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEDCInterconnectReg(pEccAggrRegs, 0U, CSL_ECC_RAM_WRAP_REV, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrWriteEccRamCtrlReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamCtrlReg(pEccAggrRegs, 0U, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrWriteEccRamErrCtrlReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrCtrlReg(pEccAggrRegs, 0U, 0U, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrWriteEccRamErrStatReg API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrStatReg(pEccAggrRegs, 0U, 0U, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrConfigEccRam API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrConfigEccRam(pEccAggrRegs, 0U, 0U, 0U, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrVerifyConfigEccRam API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrVerifyConfigEccRam(pEccAggrRegs, 0U, 0U, 0U, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrConfigEDCInterconnect API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrConfigEDCInterconnect(pEccAggrRegs, 0U, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrVerifyConfigEDCInterconnect API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrVerifyConfigEDCInterconnect(pEccAggrRegs, 0U, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrGetEccRamGetErrorStatus API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEccRamErrorStatus(pEccAggrRegs, 0U, &eccRamErrorStatus) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrForceEccRamError API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrForceEccRamError(pEccAggrRegs, 0U, &eccErrorInfo) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrGetEDCInterconnectErrorStatus API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEDCInterconnectErrorStatus(pEccAggrRegs, 0U, &edcInterconnectErrorStatus) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrForceEDCInterconnectError API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        eccEDCInterconnectErrorInfo.intrSrc    = 0;
        eccEDCInterconnectErrorInfo.eccGroup   = 0;
        eccEDCInterconnectErrorInfo.eccBit1    = 0;
        eccEDCInterconnectErrorInfo.eccBit2    = 0;
        eccEDCInterconnectErrorInfo.bNextBit   = UFALSE;
        eccEDCInterconnectErrorInfo.eccPattern = 0;
        if (CSL_ecc_aggrForceEDCInterconnectError(pEccAggrRegs, 0U, &eccEDCInterconnectErrorInfo) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrAckIntr API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrAckIntr(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrIsEccRamIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEccRamIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrSetEccRamIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrClrEccRamIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }


    /* CSL_ecc_aggrIsEDCInterconnectIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEDCInterconnectIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrSetEDCInterconnectNIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                                                       CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrSetEDCInterconnectNIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                                                       CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrClrEDCInterconnectNIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                                                       CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrClrEDCInterconnectNIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                                                       CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_PASS)

        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }


    /* CSL_ecc_aggrIsIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrIsAnyIntrPending API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsAnyIntrPending(pEccAggrRegs, 0U, &isPend) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrEnableIntr API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableIntr(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrDisableIntr API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableIntr(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrEnableAllIntr API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableAllIntr(pEccAggrRegs, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrDisableAllIntr API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableAllIntr(pEccAggrRegs, 0U) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrEnableIntrs API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrDisableIntrs API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrEnableAllIntrs API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableAllIntrs(pEccAggrRegs) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrDisableAllIntrs API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableAllIntrs(pEccAggrRegs) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrReadStaticRegs API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadStaticRegs(pEccAggrRegs, &eccStaticRegs) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrIntrEnableCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrEnableCtrl enableCtrl;

        enableCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        enableCtrl.intrEnableTimeoutErr = TRUE;
        enableCtrl.intrEnableParityErr  = TRUE;        
        if (CSL_ecc_aggrIntrEnableCtrl(pEccAggrRegs, &enableCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrIntrEnableCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrEnableCtrl enableCtrl;

        enableCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        enableCtrl.intrEnableTimeoutErr = FALSE;
        enableCtrl.intrEnableParityErr  = TRUE;        
        if (CSL_ecc_aggrIntrEnableCtrl(pEccAggrRegs, &enableCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }    
    /* CSL_ecc_aggrIntrEnableCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrEnableCtrl enableCtrl;

        enableCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        enableCtrl.intrEnableTimeoutErr = TRUE;
        enableCtrl.intrEnableParityErr  = FALSE;        
        if (CSL_ecc_aggrIntrEnableCtrl(pEccAggrRegs, &enableCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }
    /* CSL_ecc_aggrIntrEnableCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrEnableCtrl enableCtrl;

        enableCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        enableCtrl.intrEnableTimeoutErr = FALSE;
        enableCtrl.intrEnableParityErr  = FALSE;        
        if (CSL_ecc_aggrIntrEnableCtrl(pEccAggrRegs, &enableCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    /* CSL_ecc_aggrIntrStatusCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrStatusCtrl statusCtrl;

        statusCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        statusCtrl.intrStatusSetTimeoutErr  = TRUE;
        statusCtrl.intrStatusSetParityErr   = TRUE;
        statusCtrl.timeOutCnt               = (uint8_t)0U;
        statusCtrl.parityCnt                = (uint8_t)1U;
        if (CSL_ecc_aggrIntrStatusCtrl(pEccAggrRegs, &statusCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }
    /* CSL_ecc_aggrIntrStatusCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrStatusCtrl statusCtrl;

        statusCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        statusCtrl.intrStatusSetTimeoutErr  = TRUE;
        statusCtrl.intrStatusSetParityErr   = FALSE;
        statusCtrl.timeOutCnt               = (uint8_t)0U;
        statusCtrl.parityCnt                = (uint8_t)0U;
        if (CSL_ecc_aggrIntrStatusCtrl(pEccAggrRegs, &statusCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }
    /* CSL_ecc_aggrIntrStatusCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrStatusCtrl statusCtrl;

        statusCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        statusCtrl.intrStatusSetTimeoutErr  = FALSE;
        statusCtrl.intrStatusSetParityErr   = TRUE;
        statusCtrl.timeOutCnt               = (uint8_t)0U;
        statusCtrl.parityCnt                = (uint8_t)1U;
        if (CSL_ecc_aggrIntrStatusCtrl(pEccAggrRegs, &statusCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }
    /* CSL_ecc_aggrIntrStatusCtrl API test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        CSL_ecc_aggrStatusCtrl statusCtrl;

        statusCtrl.validCfg = (CSL_ECC_AGGR_VALID_TIMEOUT_ERR | \
                               CSL_ECC_AGGR_VALID_PARITY_ERR) ;
        statusCtrl.intrStatusSetTimeoutErr  = FALSE;
        statusCtrl.intrStatusSetParityErr   = FALSE;        
        statusCtrl.timeOutCnt               = (uint8_t)0U;
        statusCtrl.parityCnt                = (uint8_t)1U;
        if (CSL_ecc_aggrIntrStatusCtrl(pEccAggrRegs, &statusCtrl) != CSL_PASS)
        {
            testStatus = CSL_APP_TEST_FAILED;
            UART_printf("cslEccAggr_apiTest: failure on line no. %d \n", __LINE__);
        }
    }

    return (testStatus);
}

/* Nothing past this point */
