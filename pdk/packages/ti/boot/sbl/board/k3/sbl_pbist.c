/*
 *  Copyright (c) Texas Instruments Incorporated 2025
 *  All rights reserved.
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
 *  \file sbl_pbist_test.c
 *
 *  \brief File containing the implementation of PBIST tests
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <sbl_err_trap.h>
#include "sbl_sci_client.h"
#if defined(SBL_ENABLE_BIST)
#include <device.h>
#include <device_pm.h>
#include <types/address_types.h>
#include <priv/osal/osal_clock_user.h>
#include <clk.h>
#include <lib/mmr_lock.h>
#include <lib/io.h>
#include <pbist_utils.h>
#include "sbl_pbist.h"
#include <bist.h>
#include <bist_core_defs.h>
#if defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_utils.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_utils.h>
#elif defined(SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_utils.h>
#endif
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void SBL_runPBIST(uint32_t instanceId)
{
    int32_t testResult = 0;
    testResult = BootApp_pbistCommonInit();
    if (testResult != 0)
    {
        SBL_log(SBL_LOG_ERR,"BootApp_pbistCommonInit ...FAILED \n");
    }
    else
    {
        BootApp_pbistClecConfig(instanceId);
    }
    /* Run test on provided instance */
    testResult = BootApp_pbistRunTest(instanceId, false);
    /* PBIST_runtTest return value (-1 = failure and 0 = pass) */
    if ( testResult != 0)
    {
        SBL_log(SBL_LOG_ERR,"PBIST functional test failed for %d\n", instanceId);
    }
}

/* Refer TISCI_MSG_SYS_RESET in TISCI user guide for more details
   http://software-dl.ti.com/tisci/esd/latest/2_tisci_msgs/pm/sysreset.html */
int32_t SBL_swResetMainDomain(void)
{
    int32_t retVal = E_FAIL;

    struct tisci_msg_sys_reset_req  request;
    struct tisci_msg_sys_reset_resp response = {0};

    Sciclient_ReqPrm_t  reqParam  = {0};
    Sciclient_RespPrm_t respParam = {0};

    memset(&request, 0, sizeof(request));
    request.domain = 0x2; /* 0x2 corresponds to the MAIN domain */

    reqParam.messageType      = (uint16_t) TISCI_MSG_SYS_RESET;
    reqParam.flags            = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload      = (const uint8_t *) &request;
    reqParam.reqPayloadSize   = (uint32_t) sizeof (request);
    reqParam.timeout          = (uint32_t) SCICLIENT_SERVICE_WAIT_FOREVER;
    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    retVal = Sciclient_service(&reqParam, &respParam);
    if (((respParam.flags & TISCI_MSG_FLAG_ACK) == 0) || (retVal != CSL_PASS))
    {
        SBL_log(SBL_LOG_ERR,"SBL_swResetMainDomain failed, retVal = %d\n resp flag = 0x%08x\n",
                     retVal, respParam.flags);
    }

    return retVal;
}

void SBL_mainDomainBootSetup(void)
{
    int32_t retVal = CSL_EFAIL;
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
    retVal = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);

    if(CSL_PASS != retVal)
    {
        SBL_log(SBL_LOG_ERR, "Sciclient get default board config...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }

    /* Unlock PLL MMRs putting back to same state prior to reset */
    SBL_log(SBL_LOG_MAX, "Unlocking pll mmrs ...");
    SBL_unlockPllMmrs();
    SBL_log(SBL_LOG_MAX, "done.\n");

    Sciclient_BoardCfgPrms_t bootAppBoardCfgPrms    = {
                                                       .boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLow,
                                                       .boardConfigHigh = 0,
                                                       .boardConfigSize = boardCfgInfo.boardCfgLowSize,
                                                       .devGrp = DEVGRP_01
                                                      };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgPmPrms  = {
                                                       .boardConfigLow = (uint32_t)NULL,
                                                       .boardConfigHigh = 0,
                                                       .boardConfigSize = 0,
                                                       .devGrp = DEVGRP_01
                                                      };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgRmPrms  = {
                                                       .boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLowRm,
                                                       .boardConfigHigh = 0,
                                                       .boardConfigSize = boardCfgInfo.boardCfgLowRmSize,
                                                       .devGrp = DEVGRP_01
                                                      };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgSecPrms = {
                                                       .boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLowSec,
                                                       .boardConfigHigh = 0,
                                                       .boardConfigSize = boardCfgInfo.boardCfgLowSecSize,
                                                       .devGrp = DEVGRP_01
                                                      };

    retVal = Sciclient_boardCfg(&bootAppBoardCfgPrms);
    if (retVal != CSL_PASS)
    {
         SBL_log(SBL_LOG_MAX,"Sciclient_boardCfg() failed.\n");
    }
    retVal = Sciclient_boardCfgPm(&bootAppBoardCfgPmPrms);
    if (retVal != CSL_PASS)
    {
         SBL_log(SBL_LOG_MAX,"Sciclient_boardCfgPm() failed.\n");
    }
    retVal = Sciclient_boardCfgRm(&bootAppBoardCfgRmPrms);
    if (retVal != CSL_PASS)
    {
         SBL_log(SBL_LOG_MAX,"Sciclient_boardCfgRm() failed.\n");
    }
    retVal = Sciclient_boardCfgSec(&bootAppBoardCfgSecPrms);
    if (retVal != CSL_PASS)
    {
         SBL_log(SBL_LOG_MAX,"Sciclient_boardCfgSec() failed.\n");
    }

    /* Init Pinmux */
    if(Board_init(BOARD_INIT_PINMUX_CONFIG) != BOARD_SOK)
	{
       SBL_log(SBL_LOG_ERR,"Board_init failed for BOARD_INIT_PINMUX_CONFIG\n");
	}

    /* Init PLLS */
    Board_init(BOARD_INIT_PLL_MAIN);

	/* Init Clocks */
    Board_initParams_t initParams;
    Board_getInitParams(&initParams);
    initParams.mainClkGrp = BOARD_MAIN_CLOCK_GROUP1;
    initParams.mcuClkGrp  = BOARD_MCU_CLOCK_GROUP1;
    Board_setInitParams(&initParams);
    if(Board_init(BOARD_INIT_MODULE_CLOCK_MAIN) != BOARD_SOK)
	{
       SBL_log(SBL_LOG_ERR,"Board_init failed for BOARD_INIT_MODULE_CLOCK\n");
	}

	/* Unlock CTRL MMR */
	SBL_log(SBL_LOG_MAX, "Unlocking CTRL MMRs ...");
    if(Board_init(BOARD_INIT_UNLOCK_MMR) != BOARD_SOK)
	{
       SBL_log(SBL_LOG_ERR,"Board_init failed for BOARD_INIT_UNLOCK_MMR\n");
	}
    SBL_log(SBL_LOG_MAX, "done.\n");

#ifdef BOOT_MMCSD
    /* Set eMMC1CLKSEL */
    #define EMMC1_CLKSEL_ADDR (0x001080B4)
    *(unsigned int *)(EMMC1_CLKSEL_ADDR) = 0x0;

    /* Set VPACCLKSEL */
    #define VPAC_CLKSEL_ADDR (0x001080F0)
    *(unsigned int *)(VPAC_CLKSEL_ADDR) = 0x1;
#endif
}

void SBL_unlockPllMmrs(void)
{
    HW_WR_REG32(PLL0_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL0_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL1_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL1_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL2_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL2_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL3_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL3_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL4_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL4_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL5_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL5_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL6_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL6_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL7_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL7_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL8_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL8_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL9_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL9_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL12_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL12_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL14_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL14_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL16_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL16_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL17_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL17_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL19_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL19_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL25_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL25_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL26_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL26_LOCKKEY1, KICK1_UNLOCK);
#if defined (SOC_J784S4) || defined (SOC_J742S2)
    HW_WR_REG32(PLL27_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL27_LOCKKEY1, KICK1_UNLOCK);
    HW_WR_REG32(PLL28_LOCKKEY0, KICK0_UNLOCK);
    HW_WR_REG32(PLL28_LOCKKEY1, KICK1_UNLOCK);
#endif
}

void SBL_bistMainDomainReset(void)
{
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
    int32_t status = CSL_EFAIL;

    #if defined(SOC_J784S4) || defined(SOC_J742S2)
    uint32_t PBIST_INSTANCES[NUM_BIST_TESTS]= {
        PBIST_INSTANCE_CODEC,
        PBIST_INSTANCE_MAININFRA_1,
        PBIST_INSTANCE_DSS,
        PBIST_INSTANCE_NAVSS,
        PBIST_INSTANCE_MAININFRA_0,
        PBIST_INSTANCE_HC,
        PBIST_INSTANCE_CODEC_1,
        PBIST_INSTANCE_A72_0_0,
        PBIST_INSTANCE_A72_0_1,
        PBIST_INSTANCE_A72_1_0,
        PBIST_INSTANCE_A72_1_1,
        PBIST_INSTANCE_MSMC
        };
    #elif defined(SOC_J721S2)
    uint32_t PBIST_INSTANCES[NUM_BIST_TESTS]= {
        PBIST_INSTANCE_MAININFRA_1,
        PBIST_INSTANCE_MAININFRA_0,
        PBIST_INSTANCE_HC,
        PBIST_INSTANCE_NAVSS,
        PBIST_INSTANCE_CODEC_1,
        PBIST_INSTANCE_A72_0,
        PBIST_INSTANCE_MSMC,
        PBIST_INSTANCE_DSS
        };
    #endif

    status = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);
    if (CSL_PASS != status)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient get default board config...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }

    /* Needed for Sciclient_pmSetModuleState to power on/off the devices */
    SBL_SciclientBoardCfgPm(DEVGRP_ALL, &boardCfgInfo);

    /* Initialize SDL Osal Layer */
    status = BootApp_osalWrapper();
    if (SDL_PASS != status)
    {
        SBL_log(SBL_LOG_ERR,"SDL OSAL Init Failed\n");
    }

    /* Run PBIST tests */
    for(uint32_t i = 0; i<NUM_BIST_TESTS; i++)
    {
        SBL_runPBIST(PBIST_INSTANCES[i]);
	 }

    /* Reset Main Domain */
    SBL_log(SBL_LOG_MIN, "Resetting Main Domain ...");
    status = SBL_swResetMainDomain();
    if(CSL_PASS != status)
    {
        SBL_log(SBL_LOG_ERR, "SBL_swResetMainDomain FAILED!!\n");
    }
    else
    {
        SBL_log(SBL_LOG_MIN, "done.\n");
    }

    /* Needed for initializing devices and clocks after Reset */
    SBL_SciclientBoardCfgPm(DEVGRP_ALL, &boardCfgInfo);

    /* Initializing Board PLL in Main Domain after Reset */
    status = Board_PLLInitMain();
    if(CSL_PASS != status)
    {
        SBL_log(SBL_LOG_ERR, "Board_PLLInitMain FAILED!!\n");
    }

    /* Initializing Clocks in Main Domain after Reset */
    Board_initParams_t initParams;
    Board_getInitParams(&initParams);
    initParams.mainClkGrp = BOARD_MAIN_CLOCK_GROUP1;
    Board_setInitParams(&initParams);
    status = Board_moduleClockInitMain();
    if(CSL_PASS != status)
    {
        SBL_log(SBL_LOG_ERR, "Board_moduleClockInitMain FAILED!!\n");
    }

    /* Re-configure Firewalls in Main Domain */
    status = SBL_SciclientReconfigureFirewalls();
    if(CSL_PASS != status)
    {
        SBL_log(SBL_LOG_ERR, "SBL_SciclientReconfigureFirewalls FAILED!!\n");
    }
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */