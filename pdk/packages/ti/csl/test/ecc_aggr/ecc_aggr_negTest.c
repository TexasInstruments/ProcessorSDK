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
 *  \file     ecc_aggr_negTest.c
 *
 *  \brief    This file contains ECC Aggregator Negtive test code.
 *
 *  \details  ECC Aggregator Negative tests
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

int32_t cslEccAggr_negTest(void)
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

    /* CSL_ecc_aggrGetRevision negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetRevision(NULL, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetRevision(pEccAggrRegs, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrGetNumRams negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetNumRams(NULL, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetNumRams(pEccAggrRegs, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrReadEccRamReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamReg(NULL, 0U, CSL_ECC_RAM_WRAP_REV, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, 0xFFFFU, CSL_ECC_RAM_WRAP_REV, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, 0U, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, 0U, CSL_ECC_RAM_WRAP_REV, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrReadEDCInterconnectReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEDCInterconnectReg(NULL, 0U, CSL_EDC_CTL_REVISION, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEDCInterconnectReg(pEccAggrRegs, 0xFFFFU, CSL_EDC_CTL_REVISION, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEDCInterconnectReg(pEccAggrRegs, 0U, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEDCInterconnectReg(pEccAggrRegs, 0U, CSL_EDC_CTL_REVISION, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }
    /* CSL_ecc_aggrReadEccRamWrapRevReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamWrapRevReg(NULL, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamWrapRevReg(pEccAggrRegs, 0xFFFFU, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamWrapRevReg(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrReadEccRamCtrlReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamCtrlReg(NULL, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamCtrlReg(pEccAggrRegs, 0xFFFFU, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamCtrlReg(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrReadEccRamErrCtrlReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrCtrlReg(NULL, 0U, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrCtrlReg(pEccAggrRegs, 0xFFFFU, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrCtrlReg(pEccAggrRegs, 0U, 0xFFFFU, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrCtrlReg(pEccAggrRegs, 0U, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrReadEccRamErrStatReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrStatReg(NULL, 0U, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrStatReg(pEccAggrRegs, 0xFFFFU, 0U, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrStatReg(pEccAggrRegs, 0U, 0xFFFFU, &val) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadEccRamErrStatReg(pEccAggrRegs, 0U, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrWriteEccRamReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamReg(NULL, 0U, CSL_ECC_RAM_WRAP_REV, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamReg(pEccAggrRegs, 0xFFFFU, CSL_ECC_RAM_WRAP_REV, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamReg(pEccAggrRegs, 0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }


    /* CSL_ecc_aggrWriteEDCInterconnectReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEDCInterconnectReg(NULL, 0U, CSL_EDC_CTL_REVISION, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEDCInterconnectReg(pEccAggrRegs, 0xFFFFU, CSL_EDC_CTL_REVISION, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEDCInterconnectReg(pEccAggrRegs, 0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrWriteEccRamCtrlReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamCtrlReg(NULL, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamCtrlReg(pEccAggrRegs, 0xFFFFU, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrWriteEccRamErrCtrlReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrCtrlReg(NULL, 0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrCtrlReg(pEccAggrRegs, 0xFFFFU, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrCtrlReg(pEccAggrRegs, 0U, 0xFFFFU, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrWriteEccRamErrStatReg negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrStatReg(NULL, 0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrStatReg(pEccAggrRegs, 0xFFFFU, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrWriteEccRamErrStatReg(pEccAggrRegs, 0U, 0xFFFFU, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrConfigEccRam negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrConfigEccRam(NULL, 0U,  0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrConfigEccRam(pEccAggrRegs, 0xFFFFU, 0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrVerifyConfigEccRam negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrVerifyConfigEccRam(NULL, 0U, 0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrVerifyConfigEccRam(pEccAggrRegs, 0xFFFFU, 0U, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrConfigEDCInterconnect negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrConfigEDCInterconnect(NULL, 0U,  0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrConfigEDCInterconnect(pEccAggrRegs, 0xFFFFU, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrVerifyConfigEDCInterconnect negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrVerifyConfigEDCInterconnect(NULL, 0U, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrVerifyConfigEDCInterconnect(pEccAggrRegs, 0xFFFFU, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrGetEccRamErrorStatus negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEccRamErrorStatus(NULL, 0U, &eccRamErrorStatus) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEccRamErrorStatus(pEccAggrRegs, 0xFFFFU, &eccRamErrorStatus) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEccRamErrorStatus(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrGetEccRamErrorStatus negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEccRamErrorStatus(NULL, 0U, &eccRamErrorStatus) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEccRamErrorStatus(pEccAggrRegs, 0xFFFFU, &eccRamErrorStatus) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEccRamErrorStatus(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrGetEDCInterconnectErrorStatus negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEDCInterconnectErrorStatus(NULL, 0U, &edcInterconnectErrorStatus) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEDCInterconnectErrorStatus(pEccAggrRegs, 0xFFFFU, &edcInterconnectErrorStatus) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrGetEDCInterconnectErrorStatus(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrForceEccRamError negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrForceEccRamError(NULL, 0U, &eccErrorInfo) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrForceEccRamError(pEccAggrRegs, 0xFFFFU, &eccErrorInfo) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrForceEccRamError(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrForceEDCInterconnectError negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrForceEDCInterconnectError(NULL, 0U, &eccEDCInterconnectErrorInfo) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrForceEDCInterconnectError(pEccAggrRegs, 0xFFFFU, &eccEDCInterconnectErrorInfo) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrForceEDCInterconnectError(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrAckIntr negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrAckIntr(NULL, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrAckIntr(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_INVALID) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrIsEccRamIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEccRamIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEccRamIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEccRamIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrSetEccRamIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrClrEccRamIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrSetEccRamNIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamNIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamNIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEccRamNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 4U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrClrEccRamNIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamNIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamNIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEccRamNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, 4U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }

    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }


    /* CSL_ecc_aggrIsEDCInterconnectIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEDCInterconnectIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEDCInterconnectIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEDCInterconnectIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsEDCInterconnectIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrSetEDCInterconnectNIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEDCInterconnectNIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEDCInterconnectNIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrSetEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 4U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }

    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrClrEDCInterconnectIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEDCInterconnectNIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEDCInterconnectNIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 1U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrClrEDCInterconnectNIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT,
                CSL_ECC_AGGR_ERROR_SUBTYPE_NORMAL, 4U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }

    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrIsIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsIntrPending(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsIntrPending(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsIntrPending(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrIsAnyIntrPending negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsAnyIntrPending(NULL, 0U, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsAnyIntrPending(pEccAggrRegs, 0xFFFFU, &isPend) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrIsAnyIntrPending(pEccAggrRegs, 0U, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrEnableIntr negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableIntr(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableIntr(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableIntr(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrDisableIntr negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableIntr(NULL, 0U, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableIntr(pEccAggrRegs, 0xFFFFU, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableIntr(pEccAggrRegs, 0U, CSL_ECC_AGGR_INTR_SRC_INVALID) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrEnableAllIntr negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableAllIntr(NULL, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableAllIntr(pEccAggrRegs, 0xFFFFU) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrDisableAllIntr negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableAllIntr(NULL, 0U) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableAllIntr(pEccAggrRegs, 0xFFFFU) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrEnableIntrs negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableIntrs(NULL, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_INVALID) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrDisableIntrs negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableIntrs(NULL, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_INVALID) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrEnableAllIntrs negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrEnableAllIntrs(NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrDisableAllIntrs negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrDisableAllIntrs(NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    /* CSL_ecc_aggrReadStaticRegs negative test */
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadStaticRegs(NULL, &eccStaticRegs) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus == CSL_APP_TEST_PASS)
    {
        if (CSL_ecc_aggrReadStaticRegs(pEccAggrRegs, NULL) != CSL_EBADARGS)
        {
            testStatus = CSL_APP_TEST_FAILED;
        }
    }
    if (testStatus != CSL_APP_TEST_PASS)
    {
        UART_printf("cslEccAggr_negTest: failure on line no. %d \n", __LINE__);
    }

    return (testStatus);
}

/* Nothing past this point */
