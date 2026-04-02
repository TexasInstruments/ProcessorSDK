/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *  \file   sciclient_ut_main.c
 *
 *  \brief  Implementation of Sciclient Unit Test
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
#include <ti/csl/cslr_pvu.h>
#endif
#include <ti/osal/osal.h>
#include <ti/osal/TimerP.h>
#include <ti/osal/TaskP.h>
#include <ti/board/board.h>
#if (defined (BUILD_MCU1_0))
#include <ti/drv/sciclient/examples/sciclient_unit_testapp/uart_utils.h>
#endif
#if defined(ENABLE_FW_NOTIFICATION)
#include <ti/drv/sciclient/examples/sciclient_unit_testapp/sciclient_fw_notify.h>
#endif
/* This is only needed as this test case is running back to back Sciclient
 * Init and de-inits.
 */
#include <ti/drv/sciclient/src/sciclient/sciclient_priv.h>
#include <ti/drv/sciclient/examples/common/sci_app_common.h>
#include <ti/drv/sciclient/examples/sciclient_unit_testapp/sciclient_ut_tests.h>
#include <ti/drv/sciclient/include/sciclient_pm.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Request types for CC Register configuration */
#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
#define CC_REG_READ_OP    0xAAU
#define CC_REG_WRITE_OP   0xBBU
#define PM_FFI_CONFIG0		   0x1034U
#define CSL_DMSC_BOOT_PM_FFI_CONFIG0 CSL_COMPUTE_CLUSTER0_DMSC_BOOT_BASE + PM_FFI_CONFIG0
#endif
/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

extern Sciclient_ServiceHandle_t gSciclientHandle;
extern volatile uint32_t gInterruptRecieved;
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS) && defined (BUILD_MCU)
static uint8_t  gSciclientAppTskStackMain[32*1024] __attribute__((aligned(32*1024))) = { 0 };
#else
static uint8_t  gSciclientAppTskStackMain[32*1024] __attribute__((aligned(8192)));
#endif
/* IMPORTANT NOTE: For C7x,
 * - stack size and stack ptr MUST be 8KB aligned
 * - AND min stack size MUST be 32KB
 * - AND stack assigned for task context is "size - 8KB"
 *       - 8KB chunk for the stack area is used for interrupt handling in this task context
 */
#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
static volatile int32_t gAppIsrExecNum = 0;
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Function Declarations                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Internal Function Declarations                       */
/* ========================================================================== */

static void mainTask(void* arg0, void* arg1);
static int32_t SciclientApp_getRevisionTestPol(void);
static int32_t SciclientApp_getRevisionTestIntr(void);
static int32_t SciclientApp_timeoutTest(void);
static int32_t SciclientApp_invalidReqPrmTest(void);
static int32_t SciclientApp_msmcQueryTest(void);
#if defined(ENABLE_MSG_FWD)
static int32_t SciclientApp_tifs2dmMsgForwardingTest(void);
#endif
#if defined(ENABLE_FW_NOTIFICATION)
static int32_t SciclientApp_fwExcpNotificationTest(void);
#endif
#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
static int32_t SciclientApp_pvu2R5IntrTest(void);
static void SciclientApp_pvu2R5IntrTestIsr(void);
static int32_t SciclientApp_pvu2GICIntrTest(void);
static int32_t SciclientApp_testccconfig(void);
#endif
#if ((defined (SOC_J721S2) || defined (SOC_J721E) || defined (SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_J7200)))
static int32_t SciclientApp_firewallInitDevgrpTest(void);
#endif
#if (defined (BUILD_MCU1_0))
static int32_t SciclientApp_mainUart2MCUR5IntrTest(void);
#endif
static int32_t SciclientApp_getDMVersion(void);
#if !defined(BUILD_MCU1_1)
static int32_t SciclientApp_mcuR5SetStateMsgForwarding2TifsTest(void);
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int main(void)
{
    TaskP_Handle task;
    TaskP_Params taskParams;

    uint32_t retVal = CSL_PASS;

    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    memset(gSciclientAppTskStackMain, 0xFF, sizeof(gSciclientAppTskStackMain));
    TaskP_Params_init(&taskParams);
    taskParams.priority     = 2;
    taskParams.stack        = gSciclientAppTskStackMain;
    taskParams.stacksize    = sizeof (gSciclientAppTskStackMain);
    task = TaskP_create(&mainTask, &taskParams);
    if(NULL == task)
    {
        OS_stop();
    }

    OS_start();

    return retVal;
}

uint32_t SciApp_getNumTests(void)
{
    return SCICLIENT_NUM_TESTCASES;
}

int32_t SciApp_testMain(SciApp_TestParams_t *testParams)
{
    switch (testParams->testcaseId)
    {
        case 1:
            testParams->testResult = SciclientApp_getRevisionTestPol();
            break;
        case 2:
            testParams->testResult = SciclientApp_getRevisionTestIntr();
            break;
        case 3:
            testParams->testResult = SciclientApp_invalidReqPrmTest();
            break;
        case 4:
            testParams->testResult = SciclientApp_timeoutTest();
            break;
        case 5:
            testParams->testResult = SciclientApp_msmcQueryTest();
            break;
#if defined(ENABLE_MSG_FWD)
        case 6:
            testParams->testResult = SciclientApp_tifs2dmMsgForwardingTest();
            break;
#endif
#if defined(ENABLE_FW_NOTIFICATION)
        case 7:
            testParams->testResult = SciclientApp_fwExcpNotificationTest();
            break;
#endif
#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
        case 8:
            testParams->testResult = SciclientApp_pvu2R5IntrTest();
            break;
        case 9:
            testParams->testResult = SciclientApp_pvu2GICIntrTest();
            break;
#endif
#if (defined (BUILD_MCU1_0))
        case 10:
            testParams->testResult = SciclientApp_mainUart2MCUR5IntrTest();
            break;
#endif
        case 11:
            testParams->testResult = SciclientApp_getDMVersion();
            break;
#if !defined(BUILD_MCU1_1)
        case 12:
            testParams->testResult = SciclientApp_mcuR5SetStateMsgForwarding2TifsTest();
            break;
#endif
#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
		case 13:
			testParams->testResult = SciclientApp_testccconfig();
			break;
#endif
#if ((defined (SOC_J721S2) || defined (SOC_J721E) || defined (SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_J7200)))
		case 14:
			testParams->testResult = SciclientApp_firewallInitDevgrpTest();
			break;
#endif
        default:
            break;
    }
    
    return 0;
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */


static void mainTask(void* arg0, void* arg1)
{
    /*To suppress unused variable warning*/
    (void)arg0;
    (void)arg1;
    volatile uint32_t loopForever = 1U;

    SciApp_parser();

    while(loopForever);
}

static int32_t SciclientApp_getRevisionTestPol(void)
{
    int32_t status                  = CSL_EFAIL;
    Sciclient_ConfigPrms_t  config  =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };
    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t     respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };
    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }

    status = Sciclient_init(&config);

    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);

        if (CSL_PASS == status)
        {
            if (respPrm.flags == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                SciApp_printf(" DMSC Firmware Version %s\n", 
                              (char *) response.str);
                SciApp_printf(" Firmware revision 0x%x\n", 
                              response.version);
                SciApp_printf(" ABI revision %d.%d\n", 
                              response.abi_major, response.abi_minor);
            }
            else
            {
                SciApp_printf(" DMSC Firmware Get Version failed \n");
            }
        }
        else
        {
            SciApp_printf(" DMSC Firmware Get Version failed \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    
    return status;
}

static int32_t SciclientApp_getRevisionTestIntr(void)
{
    int32_t status                  = CSL_EFAIL;
    Sciclient_ConfigPrms_t   config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_INTERRUPT,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };

    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };
    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t     respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };
    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }

    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if (respPrm.flags == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                SciApp_printf(" DMSC Firmware Version %s\n", 
                              (char *) response.str);
                SciApp_printf(" Firmware revision 0x%x\n", 
                              response.version);
                SciApp_printf(" ABI revision %d.%d\n", response.abi_major, 
                              response.abi_minor);
            }
            else
            {
                SciApp_printf(" DMSC Firmware Get Version failed \n");
            }
        }
        else
        {
            SciApp_printf(" DMSC Firmware Get Version failed \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    
    return status;
}

static int32_t SciclientApp_invalidReqPrmTest(void)
{
    int32_t status                            = CSL_EFAIL;
    Sciclient_ConfigPrms_t             config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };
    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t reqPrm_badTxSize =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t*) &request,
        100,
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    const Sciclient_ReqPrm_t      reqPrm_good =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t*)&request,
        0,
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t     respPrm_badRxsize =
    {
        0,
        (uint8_t *) &response,
        100
    };

    Sciclient_RespPrm_t          respPrm_good =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };
    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }
    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(NULL, &respPrm_good);
        if (CSL_EBADARGS == status)
        {
            status = CSL_PASS;
            SciApp_printf(" NULL Arg Test PASSED \n");
        }
        else
        {
            SciApp_printf(" NULL Arg Test FAILED \n");
        }
        status = Sciclient_service(&reqPrm_badTxSize, &respPrm_good);
        if (CSL_EBADARGS == status)
        {
            status = CSL_PASS;
            SciApp_printf(" Tx Payload Check PASSED \n");
        }
        else
        {
            SciApp_printf(" Tx Payload Check FAILED \n");
        }
        status = Sciclient_service(&reqPrm_good, &respPrm_badRxsize);
        if (CSL_EBADARGS == status)
        {
            status = CSL_PASS;
            SciApp_printf(" Rx Payload Check PASSED \n");
        }
        else
        {
            SciApp_printf(" Rx Payload Check FAILED \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    
    return status;
}

static int32_t SciclientApp_timeoutTest(void)
{
    int32_t status                  = CSL_EFAIL;
    Sciclient_ConfigPrms_t   config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };
    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *)&request,
        sizeof(request),
        (uint32_t)0x0A
    };
    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t     respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };
    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }
    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_ETIMEOUT == status)
        {
            status = CSL_PASS;
            SciApp_printf(" Timeout test PASSED \n");
        }
        else
        {
            SciApp_printf(" Timeout Test FAILED \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    
    return status;
}

static int32_t SciclientApp_msmcQueryTest(void)
{
    int32_t status                = CSL_EFAIL;
    Sciclient_ConfigPrms_t config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };

    const struct tisci_query_msmc_req req = {};
    struct tisci_query_msmc_resp resp;
    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }
    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_msmcQuery(&req, &resp, SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if (status == CSL_PASS)
    {
        SciApp_printf("MSMC Start Address = 0x%X%X\n", 
                      resp.msmc_start_high, resp.msmc_start_low);
        SciApp_printf("MSMC End Address = 0x%X%X\n", 
                      resp.msmc_end_high, resp.msmc_end_low);
    }
    else
    {
        SciApp_printf("MSMC Query FAILED !!");
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    
    return status;
}

#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
static int32_t SciclientApp_testccconfig(void)
{
	int32_t status = CSL_EFAIL;
	int32_t retVal = CSL_PASS;
	const uint32_t testRegAddr = CSL_DMSC_BOOT_PM_FFI_CONFIG0;
	const uint32_t testRegValue = 14U;

	Sciclient_ConfigPrms_t config =
	{
		SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
		NULL,
		0U /* isSecure = 0 unsecured for all cores */
	};
	struct tisci_msg_cc_configuration_req req = {0U};
	struct tisci_msg_cc_configuration_resp resp = {0U};

	/* This is only needed as this test case is running back to back Sciclient
	 * Init and de-inits.
	 */
	while (gSciclientHandle.initCount != 0)
	{
		status = Sciclient_deinit();
		if (status != CSL_PASS)
		{
			SciApp_printf("ERROR: Sciclient_deinit failed with status: %d\n", status);
			retVal = CSL_EFAIL;
			goto exit;
		}
	}

	status = Sciclient_init(&config);
	if (status != CSL_PASS)
	{
		SciApp_printf("ERROR: Sciclient_init failed with status: %d\n", status);
		retVal = CSL_EFAIL;
		goto exit;
	}

	/* Test 1: Write to CC register */
	req.reg_addr = testRegAddr;
	req.reg_val = testRegValue;
	req.req_type = CC_REG_WRITE_OP;

	status = Sciclient_configureCC(&req, &resp, SCICLIENT_SERVICE_WAIT_FOREVER);
	if (status != CSL_PASS)
	{
		SciApp_printf("CC register configuration write FAILED with status: %d\n", status);
		retVal = CSL_EFAIL;
		goto cleanup;
	}
	SciApp_printf("CC register configuration write PASSED !!\n");

	/* Test 2: Read from CC register */
	memset((void *)&req, 0, sizeof(req));
	memset((void *)&resp, 0, sizeof(resp));

	req.reg_addr = testRegAddr;
	req.req_type = CC_REG_READ_OP;

	status = Sciclient_configureCC(&req, &resp, SCICLIENT_SERVICE_WAIT_FOREVER);
	if (status != CSL_PASS)
	{
		SciApp_printf("CC register configuration read FAILED with status: %d\n", status);
		retVal = CSL_EFAIL;
		goto cleanup;
	}
	SciApp_printf("CC register configuration read PASSED !!\n");

	/* Test 3: Verify read value matches written value */
	if (resp.reg_val != testRegValue)
	{
		SciApp_printf("cc register Configuration test FAILED !! (Expected: 0x%x, Actual: 0x%x)\n",
		              testRegValue, resp.reg_val);
		retVal = CSL_EFAIL;
		goto cleanup;
	}
	SciApp_printf("cc register Configuration test PASSED !!\n");

cleanup:
	status = Sciclient_deinit();
	if (status != CSL_PASS)
	{
		SciApp_printf("ERROR: Sciclient_deinit failed with status: %d\n", status);
		retVal = CSL_EFAIL;
	}

exit:
	return retVal;
}
#endif

#if ((defined (SOC_J721S2) || defined (SOC_J721E) || defined (SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_J7200)))
static int32_t SciclientApp_firewallInitDevgrpTest(void)
{
    int32_t status = CSL_EFAIL;
    int32_t retVal = CSL_PASS;
    const uint32_t testRegion = 0U;
    const uint32_t testDevgrp = DEVGRP_01;

    /* Select SOC-specific firewall ID from DEVGRP_MAIN with owner=none
     * j721s2/j784s4/j742s2: FW ID 2306, Region 0, Owner: none, DEVGRP_MAIN
     * j721e/j7200:          FW ID 2312, Region 0, Owner: none, DEVGRP_MAIN
     */
#if (defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2))
    const uint16_t testFwlId = 2306U;
#elif (defined(SOC_J721E) || defined(SOC_J7200))
    const uint16_t testFwlId = 2312U;
#endif

    struct tisci_msg_fwl_get_firewall_region_req getReq = {0};
    struct tisci_msg_fwl_get_firewall_region_resp getInitialResp = {0};
    struct tisci_msg_fwl_set_firewall_region_req setReq = {0};
    struct tisci_msg_fwl_set_firewall_region_resp setResp = {0};
    struct tisci_msg_fwl_get_firewall_region_resp getAfterSetResp = {0};
    struct tisci_msg_init_fwl_devgrp_req initReq = {0};
    struct tisci_msg_init_fwl_devgrp_resp initResp = {0};
    struct tisci_msg_fwl_get_firewall_region_resp getFinalResp = {0};

    Sciclient_ConfigPrms_t config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0U /* isSecure = 0 unsecured for all cores */
    };

    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
        if (status != CSL_PASS)
        {
            SciApp_printf("ERROR: Sciclient_deinit failed with status: %d\n", status);
            retVal = CSL_EFAIL;
            goto exit;
        }
    }

    status = Sciclient_init(&config);
    if (status != CSL_PASS)
    {
        SciApp_printf("ERROR: Sciclient_init failed with status: %d\n", status);
        retVal = CSL_EFAIL;
        goto exit;
    }

    SciApp_printf("Starting Firewall devgrp initialization test for FW ID %d\n", testFwlId);

    /* Test Step 1: Get initial firewall configuration */
    getReq.fwl_id = testFwlId;
    getReq.region = testRegion;
    getReq.n_permission_regs = 3U;

    status = Sciclient_firewallGetRegion(&getReq, &getInitialResp,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status != CSL_PASS)
    {
        SciApp_printf("ERROR: Initial Sciclient_firewallGetRegion failed for FW ID %d with status: %d\n",
                      testFwlId, status);
        retVal = CSL_EFAIL;
        goto cleanup;
    }

    SciApp_printf("Initial FW config: control=0x%x, start=0x%x, end=0x%x\n",
                  getInitialResp.control, getInitialResp.start_address, getInitialResp.end_address);

    /* Test Step 2: Modify firewall configuration using Sciclient_firewallSetRegion */
    /* Set all fields to 0 */
    setReq.fwl_id = testFwlId;
    setReq.region = testRegion;
    setReq.n_permission_regs = 3U;
    setReq.control = 0x0U;
    setReq.permissions[0] = 0x0U;
    setReq.permissions[1] = 0x0U;
    setReq.permissions[2] = 0x0U;
    setReq.start_address = 0x0U;
    setReq.end_address = 0x0U;

    status = Sciclient_firewallSetRegion(&setReq, &setResp,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status != CSL_PASS)
    {
        SciApp_printf("ERROR: Sciclient_firewallSetRegion failed for FW ID %d with status: %d\n",
                      testFwlId, status);
        retVal = CSL_EFAIL;
        goto cleanup;
    }

    SciApp_printf("Firewall configuration modified successfully (all fields set to 0)\n");

    /* Test Step 3: Read back configuration after set and verify */
    status = Sciclient_firewallGetRegion(&getReq, &getAfterSetResp,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status != CSL_PASS)
    {
        SciApp_printf("ERROR: Sciclient_firewallGetRegion (after set) failed for FW ID %d with status: %d\n",
                      testFwlId, status);
        retVal = CSL_EFAIL;
        goto cleanup;
    }

    /* Verify the set operation */
    if (getAfterSetResp.control != setReq.control)
    {
        SciApp_printf("ERROR: FW control mismatch after set. Expected: 0x%x, Got: 0x%x\n",
                      setReq.control, getAfterSetResp.control);
        retVal = CSL_EFAIL;
        goto cleanup;
    }

    SciApp_printf("FW config after set: control=0x%x, start=0x%x, end=0x%x\n",
                  getAfterSetResp.control, getAfterSetResp.start_address, getAfterSetResp.end_address);

    /* Test Step 4: Call Sciclient_firewallInitDevgrp with devgrp 1 */
    initReq.dgrp = testDevgrp;

    status = Sciclient_firewallInitDevgrp(&initReq, &initResp,
                                           SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status != CSL_PASS)
    {
        SciApp_printf("ERROR: Sciclient_firewallInitDevgrp failed with status: %d\n", status);
        retVal = CSL_EFAIL;
        goto cleanup;
    }

    SciApp_printf("Firewall devgrp %d initialized successfully\n", testDevgrp);

    /* Test Step 5: Read firewall configuration again after devgrp initialization */
    status = Sciclient_firewallGetRegion(&getReq, &getFinalResp,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status != CSL_PASS)
    {
        SciApp_printf("ERROR: Sciclient_firewallGetRegion (after devgrp init) failed for FW ID %d with status: %d\n",
                      testFwlId, status);
        retVal = CSL_EFAIL;
        goto cleanup;
    }

    SciApp_printf("FW config after devgrp init: control=0x%x, start=0x%x, end=0x%x\n",
                  getFinalResp.control, getFinalResp.start_address, getFinalResp.end_address);

    /* Test Step 6: Compare final configuration with initial configuration */
    /* After devgrp init, firewall should be restored to initial default state */
    if ((getFinalResp.control != getInitialResp.control) ||
        (getFinalResp.start_address != getInitialResp.start_address) ||
        (getFinalResp.end_address != getInitialResp.end_address))
    {
        SciApp_printf("INFO: FW ID %d configuration changed after devgrp init (expected behavior)\n", testFwlId);
        SciApp_printf("  Initial: control=0x%x, start=0x%llx, end=0x%llx\n",
                      getInitialResp.control, getInitialResp.start_address, getInitialResp.end_address);
        SciApp_printf("  Final:   control=0x%x, start=0x%llx, end=0x%llx\n",
                      getFinalResp.control, getFinalResp.start_address, getFinalResp.end_address);
    }
    else
    {
        SciApp_printf("INFO: FW ID %d configuration remained same after devgrp init\n", testFwlId);
    }

    if (retVal == CSL_PASS)
    {
        SciApp_printf("Firewall devgrp initialization test PASSED !!\n");
    }

cleanup:
    status = Sciclient_deinit();
    if (status != CSL_PASS)
    {
        SciApp_printf("ERROR: Sciclient_deinit failed with status: %d\n", status);
        retVal = CSL_EFAIL;
    }

exit:
    return retVal;
}
#endif

#if defined(ENABLE_MSG_FWD)
static int32_t SciclientApp_tifs2dmMsgForwardingTest(void)
{
    int32_t status                  = CSL_EFAIL;
    Sciclient_ConfigPrms_t config   =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_INTERRUPT,
        NULL,
        1 /* isSecure = 1 to test passing message on secure channel */
    };

    SciApp_printf(" Starting TIFS2DM msg forwarding test (from secure queue)\n");

    struct tisci_msg_get_device_req request;
    request.id = 0x1; /* Use DEV ID of 1 as a device to check state (any DEV ID will do) */
    const Sciclient_ReqPrm_t reqPrm =
    {
        TISCI_MSG_GET_DEVICE,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_get_device_resp response;
    Sciclient_RespPrm_t     respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof(response)
    };
    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }

    status = Sciclient_init(&config);
    if (status == CSL_PASS)
    {
        status = Sciclient_service(&reqPrm, &respPrm);
        if (CSL_PASS == status)
        {
            if ((respPrm.flags & TISCI_MSG_FLAG_ACK) == TISCI_MSG_FLAG_ACK)
            {
                status = CSL_PASS;
                SciApp_printf(" Device ID 0 - Context Loss Count: %d\n",
                              response.context_loss_count);
                SciApp_printf("             - Resets: %d\n",
                              response.resets);
                SciApp_printf("             - Programmed state: %d\n",
                              response.programmed_state);
                SciApp_printf("             - Current state: %d\n",
                              response.current_state);
            }
            else
            {
                SciApp_printf(" Device ID 0 - Get device state failed \n");
            }
        }
        else
        {
            SciApp_printf(" Device ID 0 - Get device state failed \n");
        }
    }
    if (status == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    return status;
}
#endif

#if defined(ENABLE_FW_NOTIFICATION)
static int32_t SciclientApp_fwExcpNotificationTest(void)
{
    int32_t   status = CSL_PASS;
    volatile  uint32_t* excpRegCmbn;
    volatile  uint32_t* excpRegDmsc;
    OsalRegisterIntrParams_t intrPrmsDmscIntr;
    OsalRegisterIntrParams_t intrPrmsCmbnIntr;
    CSL_R5ExptnHandlers sciclientR5ExptnHandlers;
    HwiP_Handle hwiPHandleDmscIntr;
    HwiP_Handle hwiPHandleCmbnIntr;

    SciApp_printf(" Starting firewall notification handling (from secure queue)\n");

    excpRegDmsc = (volatile uint32_t*)DMSC_FW_EXCP_REG;
    excpRegCmbn = (volatile uint32_t*)CMBN_FW_EXCP_REG;

    /* Register an abort exception handler */
    Intc_InitExptnHandlers(&sciclientR5ExptnHandlers);
    sciclientR5ExptnHandlers.dabtExptnHandler = (void*)SciclientApp_fwAbortHandlerIsr;
    Intc_RegisterExptnHandlers(&sciclientR5ExptnHandlers);

    /* Interrupt registration for dmsc interrupt notification routing */
    Osal_RegisterInterrupt_initParams(&intrPrmsCmbnIntr);
    intrPrmsCmbnIntr.corepacConfig.isrRoutine       =
                                (void*)SciclientApp_fwNotiIsrCmbn;
    intrPrmsCmbnIntr.corepacConfig.corepacEventNum  = 0;
    intrPrmsCmbnIntr.corepacConfig.intVecNum        = CMBN_FW_EXCEPTION_INTR;
    status =Osal_RegisterInterrupt(&intrPrmsCmbnIntr, &hwiPHandleCmbnIntr);

    /* Interrupt registration for dmsc interrupt notification routing */
    Osal_RegisterInterrupt_initParams(&intrPrmsDmscIntr);
    intrPrmsDmscIntr.corepacConfig.isrRoutine       =
                                (void*)SciclientApp_fwNotiIsrDmsc;
    intrPrmsDmscIntr.corepacConfig.corepacEventNum  = 0;
    intrPrmsDmscIntr.corepacConfig.intVecNum        = DMSC_FW_EXCEPTION_INTR;
    status =Osal_RegisterInterrupt(&intrPrmsDmscIntr, &hwiPHandleDmscIntr);

    /* Invoking a firewall exception notification for the dmsc exception handler by writing to dmsc address */
    *excpRegDmsc = 0x01;

    Osal_delay(1U); /*Delay of 1ms*/

    /* Invoking a firewall exception notification for the cmbn exception handler by writing to dmsc address */
    *excpRegCmbn = 0x01;

    /* This ensures that all the LOAD and STORE operations that were issued before the barrier 
    occur before the LOAD and STORE operations issued after the barrier */
    __sync_synchronize();

    if (gInterruptRecieved != EXCEPTION_INTERRUPT_CNT)
    {
        status = CSL_EFAIL;
    }
    
    return status;
}
#endif

#if ((defined (SOC_J721S2) || defined (SOC_J784S4) || defined(SOC_J742S2)))
static void SciclientApp_pvu2R5IntrTestIsr(void)
{
    gAppIsrExecNum++;

    /* Clear the PVU interrupt by writing 1 to pend clear register */
    CSL_REG32_WR(CSL_NAVSS0_IO_PVU0_CFG_MMRS_BASE+CSL_PVU_EXCEPTION_PEND_CLEAR, 1);

    /* Disable the PVU interrupt by writing 1 to enable clear register */
    CSL_REG32_WR(CSL_NAVSS0_IO_PVU0_CFG_MMRS_BASE+CSL_PVU_EXCEPTION_ENABLE_CLEAR, 1);
}

static int32_t SciclientApp_pvu2R5IntrTest(void)
{
    int32_t  status                = CSL_PASS;
    int32_t  sciclientInitStatus   = CSL_PASS;
    int32_t  pvu2R5RouteStatus     = CSL_PASS;
    uint16_t intNum                = 0;
    Sciclient_ConfigPrms_t config  =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };
    struct tisci_msg_rm_irq_set_req rmIrqReq;
    struct tisci_msg_rm_irq_set_resp rmIrqResp;
    struct tisci_msg_rm_get_resource_range_req req;
    struct tisci_msg_rm_get_resource_range_resp res;
    struct tisci_msg_rm_irq_release_req rmIrqReqRel;
    memset(&req, 0, sizeof(req));
    memset(&res, 0, sizeof(res));
    memset(&rmIrqReqRel,0,sizeof(rmIrqReqRel));

    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }
    status = Sciclient_init(&config);
    sciclientInitStatus = status;

    if(sciclientInitStatus == CSL_PASS)
    {
        SciApp_printf("PVU to Main R5 Interrupt Test\n");

        /* Enable the PVU interrupt by writing 1 to enable set register */
        CSL_REG32_WR(CSL_NAVSS0_IO_PVU0_CFG_MMRS_BASE+CSL_PVU_EXCEPTION_ENABLE_SET, 1);

        req.type = TISCI_DEV_NAVSS0_INTR_0;
        req.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

        status  = Sciclient_rmGetResourceRange(&req,
                                               &res,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);

        if(status == CSL_PASS)
        {
            SciApp_printf("Sciclient_rmGetResourceRange() execution is successful\n");
            status = Sciclient_rmIrqTranslateIrOutput(req.type,
                                                      res.range_start,
                                                      TISCI_DEV_R5FSS0_CORE0,
                                                      &intNum);
            if(status == CSL_PASS)
            {
                SciApp_printf("Sciclient_rmIrqTranslateIrOutput() execution is successful and host interrupt number is %d\n", intNum);

                memset(&rmIrqReq, 0, sizeof(rmIrqReq));
                rmIrqReq.valid_params   = TISCI_MSG_VALUE_RM_DST_ID_VALID | TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;
                rmIrqReq.valid_params  |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
                rmIrqReq.src_id         = TISCI_DEV_NAVSS0_PVU_0;
                rmIrqReq.src_index      = 0;
                rmIrqReq.dst_id         = TISCI_DEV_R5FSS0_CORE0;
                rmIrqReq.dst_host_irq   = intNum;
                rmIrqReq.secondary_host = TISCI_HOST_ID_MAIN_0_R5_0;

                status                  = Sciclient_rmIrqSet(&rmIrqReq,
                                                            &rmIrqResp,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                pvu2R5RouteStatus       = status;
                if(status == CSL_PASS)
                {
                    SciApp_printf("Sciclient_rmIrqSet() execution is successful\n");
                }
                else
                {
                    SciApp_printf("Sciclient_rmIrqSet() has failed\n");
                }
            }
            else
            {
                SciApp_printf("Sciclient_rmIrqTranslateIrOutput() has failed\n");
            }
        }
        else
        {
            SciApp_printf("Sciclient_rmGetResourceRange() has failed\n");
        }

        /* Generates an interrupt from PVU and checks if the registered ISR is hit.
        * This helps in checking the correctness of PVU to R5 interrupt routing. */
        if(status == CSL_PASS)
        {
            OsalRegisterIntrParams_t intrPrmsCmbnIntr;
            HwiP_Handle hwiPHandleCmbnIntr;

            Osal_RegisterInterrupt_initParams(&intrPrmsCmbnIntr);
            intrPrmsCmbnIntr.corepacConfig.isrRoutine       = (void (*)(uintptr_t))SciclientApp_pvu2R5IntrTestIsr;
            intrPrmsCmbnIntr.corepacConfig.corepacEventNum  = 0;
            intrPrmsCmbnIntr.corepacConfig.intVecNum        = rmIrqReq.dst_host_irq;
            status = Osal_RegisterInterrupt(&intrPrmsCmbnIntr, &hwiPHandleCmbnIntr);
            if(status == CSL_PASS)
            {
                SciApp_printf("Interrupt registration is successful\n");
                /* Generate the PVU interrupt by writing 1 to pend set register */
                CSL_REG32_WR(CSL_NAVSS0_IO_PVU0_CFG_MMRS_BASE+CSL_PVU_EXCEPTION_PEND_SET, 1);
                if(gAppIsrExecNum == 1)
                {
                    SciApp_printf("ISR is hit, hence PVU to Main R5 interrupt route is properly set\n");
                    status = CSL_PASS;
                }
                else
                {
                    SciApp_printf("ISR is not hit, hence PVU to Main R5 interrupt route is not properly set\n");
                    /* Clear the PVU interrupt by writing 1 to pend clear register */
                    CSL_REG32_WR(CSL_NAVSS0_IO_PVU0_CFG_MMRS_BASE+CSL_PVU_EXCEPTION_PEND_CLEAR, 1);

                    /* Disable the PVU interrupt by writing 1 to enable clear register */
                    CSL_REG32_WR(CSL_NAVSS0_IO_PVU0_CFG_MMRS_BASE+CSL_PVU_EXCEPTION_ENABLE_CLEAR, 1);
                    status = CSL_EFAIL;
                }
            }
            else
            {
                SciApp_printf("Interrupt registration has failed\n");
            }
        }

        if(pvu2R5RouteStatus == CSL_PASS)
        {
            SciApp_printf("\nDeleting PVU to Main R5 Interrupt Route\n");

            rmIrqReqRel.valid_params   = TISCI_MSG_VALUE_RM_DST_ID_VALID | TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;
            rmIrqReqRel.valid_params  |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
            rmIrqReqRel.src_id         = TISCI_DEV_NAVSS0_PVU_0;
            rmIrqReqRel.src_index      = 0;
            rmIrqReqRel.dst_id         = TISCI_DEV_R5FSS0_CORE0;
            rmIrqReqRel.dst_host_irq   = intNum;
            rmIrqReqRel.secondary_host = TISCI_HOST_ID_MAIN_0_R5_0;
            gAppIsrExecNum = 0;

            status = Sciclient_rmIrqRelease(&rmIrqReqRel, SCICLIENT_SERVICE_WAIT_FOREVER);
            if(status == CSL_PASS)
            {
                SciApp_printf("Sciclient_rmIrqRelease() execution is successful\n");
            }
            else
            {
                SciApp_printf("Sciclient_rmIrqRelease() has failed\n");
            }
        }
    }
    else
    {
        SciApp_printf("Sciclient_init() has failed\n");
    }

    if (sciclientInitStatus == CSL_PASS)
    {
        status = Sciclient_deinit();
    }

    return status;
}

static int32_t SciclientApp_pvu2GICIntrTest(void)
{
    int32_t  status               = CSL_PASS;
    int32_t  sciclientInitStatus  = CSL_PASS;
    uint16_t intNum               = 0;
    Sciclient_ConfigPrms_t config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };
    struct tisci_msg_rm_irq_set_req rmIrqReq;
    struct tisci_msg_rm_irq_set_resp rmIrqResp;
    struct tisci_msg_rm_get_resource_range_req req;
    struct tisci_msg_rm_get_resource_range_resp res;
    memset(&req, 0, sizeof(req));
    memset(&res, 0, sizeof(res));

    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }
    status = Sciclient_init(&config);
    sciclientInitStatus = status;

    if(sciclientInitStatus == CSL_PASS)
    {
        SciApp_printf("PVU to GIC Interrupt Test\n");

        req.type = TISCI_DEV_NAVSS0_INTR_0;
        req.secondary_host = TISCI_HOST_ID_A72_2;

        status  = Sciclient_rmGetResourceRange(&req,
                                               &res,
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
        if(status == CSL_PASS)
        {
            SciApp_printf("Sciclient_rmGetResourceRange() execution is successful\n");
            status = Sciclient_rmIrqTranslateIrOutput(req.type,
                                                      res.range_start,
                                                      TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS,
                                                      &intNum);
            if(status == CSL_PASS)
            {
                SciApp_printf("Sciclient_rmIrqTranslateIrOutput() execution is successful and host interrupt number is %d\n", intNum);

                memset(&rmIrqReq, 0, sizeof(rmIrqReq));
                rmIrqReq.valid_params   = TISCI_MSG_VALUE_RM_DST_ID_VALID;
                rmIrqReq.valid_params  |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
                rmIrqReq.valid_params  |= TISCI_MSG_VALUE_RM_SECONDARY_HOST_VALID;
                rmIrqReq.src_id         = TISCI_DEV_NAVSS0_PVU_0;
                rmIrqReq.src_index      = 0;
                rmIrqReq.dst_id         = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS;
                rmIrqReq.dst_host_irq   = intNum;
                rmIrqReq.secondary_host = TISCI_HOST_ID_A72_2;

                status                  = Sciclient_rmIrqSet(&rmIrqReq,
                                                            &rmIrqResp,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if(status == CSL_PASS)
                {
                    SciApp_printf("Sciclient_rmIrqSet() execution is successful\n");
                }
                else
                {
                    SciApp_printf("Sciclient_rmIrqSet() has failed\n");
                }
            }
            else
            {
                SciApp_printf("Sciclient_rmIrqTranslateIrOutput() has failed\n");
            }
        }
        else
        {
            SciApp_printf("Sciclient_rmGetResourceRange() has failed\n");
        }
    }
    else
    {
        SciApp_printf("Sciclient_init() has failed\n");
    }

    if (sciclientInitStatus == CSL_PASS)
    {
        status = Sciclient_deinit();
    }

    return status;
}
#endif

#if (defined (BUILD_MCU1_0))
static int32_t SciclientApp_mainUart2MCUR5IntrTest(void)
{
    int32_t  status                    = CSL_PASS;
    int32_t  sciclientInitStatus       = CSL_PASS;
    int32_t  uartWriteStatus           = CSL_PASS;
    int32_t  mainUart2MCUR5RouteStatus = CSL_PASS;
    uint32_t mainUartTestInstance;
    const UART_Params userParams       = 
    {
        UART_MODE_BLOCKING,       /* readMode */
        UART_MODE_BLOCKING,       /* writeMode */
        SemaphoreP_WAIT_FOREVER,  /* readTimeout */
        SemaphoreP_WAIT_FOREVER,  /* writeTimeout */
        NULL,                     /* readCallback */
        NULL,                     /* writeCallback */
        UART_RETURN_NEWLINE,      /* readReturnMode */
        UART_DATA_TEXT,           /* readDataMode */
        UART_DATA_TEXT,           /* writeDataMode */
        UART_ECHO_ON,             /* readEcho */
        115200,                   /* baudRate */
        UART_LEN_8,               /* dataLength */
        UART_STOP_ONE,            /* stopBits */
        UART_PAR_NONE             /* parityType */
    };
    struct UART_HWAttrs uartHwAttrs;
    UART_Handle uartHandle = NULL;
    char echoPrompt[]      = "Testing Main UART to MCU R5F routing\n";

    struct tisci_msg_rm_irq_set_req rmIrqReq;
    struct tisci_msg_rm_irq_set_resp rmIrqResp;
    struct tisci_msg_rm_get_resource_range_req req;
    struct tisci_msg_rm_get_resource_range_resp res;
    struct tisci_msg_rm_irq_release_req rmIrqReqRel;

    uint16_t intNum = 0;
    Sciclient_ConfigPrms_t config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };
    /* This is only needed as this test case is running back to back Sciclient
     * Init and de-inits.
     */
    while (gSciclientHandle.initCount != 0)
    {
        status = Sciclient_deinit();
    }
    status = Sciclient_init(&config);
    sciclientInitStatus = status;

    memset(&req, 0, sizeof(req));
    memset(&res, 0, sizeof(res));
    memset(&rmIrqReqRel,0,sizeof(rmIrqReqRel));
    mainUartTestInstance = BOARD_UART_INSTANCE;
    if(BOARD_UART_INSTANCE == 0)
    {
        mainUartTestInstance = App_getMainUartTestInstance();
    }
    /* Sets the Main UART configuration */
    UART_socGetInitCfg(BOARD_UART_INSTANCE, &uartHwAttrs);
    App_setMainUartHwAttrs(BOARD_UART_INSTANCE, &uartHwAttrs);

    (uartHwAttrs).configSocIntrPath = NULL;
    /* Programs the interrupt route between Main domain UART and MCU R5F core */
    req.type = TISCI_DEV_MAIN2MCU_LVL_INTRTR0;
    req.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
    status  = Sciclient_rmGetResourceRange(&req,
                                        &res,
                                        SCICLIENT_SERVICE_WAIT_FOREVER);
    if(status == CSL_PASS)
    {
        status = Sciclient_rmIrqTranslateIrOutput(req.type,
                                                res.range_start,
                                                TISCI_DEV_MCU_R5FSS0_CORE0,
                                                &intNum);
        if(status == CSL_PASS)
        {
            memset(&rmIrqReq, 0, sizeof(rmIrqReq));
            rmIrqReq.valid_params       = TISCI_MSG_VALUE_RM_DST_ID_VALID;
            rmIrqReq.valid_params      |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
            rmIrqReq.src_id             = App_getUartSrcID(uartHwAttrs.baseAddr);
            rmIrqReq.src_index          = 0;
            rmIrqReq.dst_id             = TISCI_DEV_MCU_R5FSS0_CORE0;
            rmIrqReq.dst_host_irq       = intNum;
            rmIrqReq.secondary_host     = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
            status                      = Sciclient_rmIrqSet(&rmIrqReq, &rmIrqResp, SCICLIENT_SERVICE_WAIT_FOREVER);
            mainUart2MCUR5RouteStatus   = status;
        }
    }
    (uartHwAttrs).intNum = intNum;
    (uartHwAttrs).enableInterrupt = 1;

    UART_socSetInitCfg(mainUartTestInstance, &uartHwAttrs);

    uartHandle = UART_open(mainUartTestInstance, (UART_Params*) &userParams);

    uartWriteStatus = UART_write(uartHandle, echoPrompt, sizeof(echoPrompt));

    if(mainUart2MCUR5RouteStatus == CSL_PASS)
    {
        /* Release the Route between Main UART and MCU R5F */
        rmIrqReqRel.valid_params   = TISCI_MSG_VALUE_RM_DST_ID_VALID;
        rmIrqReqRel.valid_params  |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
        rmIrqReqRel.src_id         = App_getUartSrcID(uartHwAttrs.baseAddr);
        rmIrqReqRel.src_index      = 0;
        rmIrqReqRel.dst_id         = TISCI_DEV_MCU_R5FSS0_CORE0;
        rmIrqReqRel.dst_host_irq   = intNum;
        rmIrqReqRel.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

        status = Sciclient_rmIrqRelease(&rmIrqReqRel, SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    UART_close(uartHandle);

    if (sciclientInitStatus == CSL_PASS)
    {
        status = Sciclient_deinit();
    }
    if(uartWriteStatus >= 0)
    {
        uartWriteStatus = CSL_PASS;
    }
    else
    {
        uartWriteStatus =  CSL_EFAIL;
    }
    
    return uartWriteStatus;
}
#endif

static int32_t SciclientApp_getDMVersion(void)
{
    int32_t status                  = CSL_EFAIL;

    struct tisci_msg_dm_version_req request;
    const Sciclient_ReqPrm_t reqPrm =
    {
        TISCI_MSG_DM_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_dm_version_resp response;
    Sciclient_RespPrm_t     respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };

    status = Sciclient_service(&reqPrm, &respPrm);
    if (CSL_PASS == status)
    {
        SciApp_printf(" Sciserver Version %s\n", (char *) response.sciserver_version);
        SciApp_printf(" rm_pm_hal Version %s\n", (char *) response.rm_pm_hal_version);
        SciApp_printf(" ABI revision %d.%d\n", response.abi_major, response.abi_minor);
    }
    else
    {
        SciApp_printf(" DM Firmware Get Version failed \n");
    }

    return status;
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

#if !defined(BUILD_MCU1_1)
static int32_t SciclientApp_mcuR5SetStateMsgForwarding2TifsTest(void)
{
    uint32_t moduleId = TISCI_DEV_MCU_R5FSS0_CORE1;
    uint32_t moduleState = 0;
    uint32_t resetState = 0;
    uint32_t contextLossState;
    int32_t ret = CSL_EFAIL;

    Sciclient_ConfigPrms_t config   =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        0 /* isSecure = 0 un secured for all cores */
    };

    SciApp_printf(" Starting mcu1_1 set state msg forwarding to TIFS test \n");

    while (gSciclientHandle.initCount != 0)
    {
        ret = Sciclient_deinit();
    }

    ret = Sciclient_init(&config);

    if (ret == CSL_PASS)
    {
        ret = Sciclient_procBootRequestProcessor(SCICLIENT_PROC_ID_MCU_R5FSS0_CORE1, SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if (ret == CSL_PASS)
    {
        int32_t status = CSL_EFAIL;
        
        status = Sciclient_pmSetModuleState(moduleId, TISCI_MSG_VALUE_DEVICE_SW_STATE_ON, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);

        if (status == CSL_PASS)
        {
            status = Sciclient_pmGetModuleState(moduleId, &moduleState, &resetState, &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if ((status == CSL_PASS) && (moduleState == TISCI_MSG_VALUE_DEVICE_HW_STATE_ON))
        {
            SciApp_printf(" Set device state test: Pass\n");
        }
        else
        {
            SciApp_printf(" Set device state test: Fail\n");
            ret = status;
        }
        
        status = Sciclient_pmSetModuleRst(moduleId, 0, SCICLIENT_SERVICE_WAIT_FOREVER);

        if (status == CSL_PASS)
        {
            status = Sciclient_pmGetModuleState(moduleId, &moduleState, &resetState, &contextLossState, SCICLIENT_SERVICE_WAIT_FOREVER);
        }
        if ((status == CSL_PASS) && (resetState == 0))
        {
            SciApp_printf(" Set device reset test: Pass\n");
        }
        else
        {
            SciApp_printf(" Set device reset test: Fail\n");
            ret = status;
        }

        status = Sciclient_procBootReleaseProcessor(SCICLIENT_PROC_ID_MCU_R5FSS0_CORE1, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status != CSL_PASS)
        {
            ret = status;
        }
        
    }
    if (ret == CSL_PASS)
    {
        ret = Sciclient_deinit();
    }
    return ret;
}
#endif
