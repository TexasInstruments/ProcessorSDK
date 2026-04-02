/*
 * Copyright (C) 2018-2022 Texas Instruments Incorporated - http://www.ti.com/
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
#include <string.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <ti/board/board.h>
#include <sbl_soc.h>
#include <sbl_soc_cfg.h>
#include <sbl_err_trap.h>
#include <sbl_sci_client.h>
#include <ti/drv/sciclient/include/sciclient_firewall.h>

#define CNTR_RELOAD_VALUE 0U

extern int32_t SBL_ReadSysfwImage(void **pBuffer, uint32_t num_bytes);
uint32_t __attribute__((section(".firmware"))) gSciclient_firmware[1];

/* Global variable to check whether combined ROM boot image format is used or not */
uint8_t combinedBootmode = 0;

#if SCICLIENT_FIRMWARE_SIZE_IN_BYTES > SBL_SYSFW_MAX_SIZE
#error "SYSFW too large...update SBL_SYSFW_MAX_SIZE"
#endif

#if (!defined(SBL_SKIP_BRD_CFG_PM)) || (!defined(SBL_SKIP_BRD_CFG_RM))
static int32_t Sciclient_setBoardConfigHeader ();
#endif

/* Firewall Data for OCMC region */
#if defined(SOC_J721E)
    #define OCMC_FWID       CSL_STD_FW_MCU_MSRAM_1MB0_SLV_ID
    #define OCMC_REGIONS    CSL_STD_FW_MCU_MSRAM_1MB0_SLV_NUM_REGIONS
#else
    #define OCMC_FWID       CSL_STD_FW_MCU_MSRAM_1MB0_RAM_ID
    #define OCMC_REGIONS    CSL_STD_FW_MCU_MSRAM_1MB0_RAM_NUM_REGIONS
#endif

/* Firewall Data for MCU_FSS_S0 region */
#if defined(SOC_J721E)
#define MCU_FSS0_S0_FWID           CSL_STD_FW_MCU_FSS0_FSS_S0_ID
#define MCU_FSS0_S0_FW_REGIONS     CSL_STD_FW_MCU_FSS0_FSS_S0_NUM_REGIONS
#define MCU_FSS0_S1_FWID           CSL_STD_FW_MCU_FSS0_FSS_S1_ID
#define MCU_FSS0_S1_FW_REGIONS     CSL_STD_FW_MCU_FSS0_FSS_S1_NUM_REGIONS
#elif defined(SOC_J7200)
#define MCU_FSS0_S0_FWID           CSL_STD_FW_MCU_FSS0_DAT_REG1_ID
#define MCU_FSS0_S0_FW_REGIONS     CSL_STD_FW_MCU_FSS0_DAT_REG1_NUM_REGIONS
#define MCU_FSS0_S1_FWID           CSL_STD_FW_MCU_FSS0_OSPI1_OSPI_DATA_VBP_R1_MAP_ID
#define MCU_FSS0_S1_FW_REGIONS     CSL_STD_FW_MCU_FSS0_OSPI1_OSPI_DATA_VBP_R1_MAP_NUM_REGIONS
#else
#define MCU_FSS0_S0_FWID           CSL_STD_FW_MCU_FSS0_FSAS_0_DAT_REG1_ID
#define MCU_FSS0_S0_FW_REGIONS     CSL_STD_FW_MCU_FSS0_FSAS_0_DAT_REG1_NUM_REGIONS
#define MCU_FSS0_S1_FWID           CSL_STD_FW_MCU_FSS0_OSPI_1_OSPI1_OSPI_DATA_VBP_R1_MAP_ID
#define MCU_FSS0_S1_FW_REGIONS     CSL_STD_FW_MCU_FSS0_OSPI_1_OSPI1_OSPI_DATA_VBP_R1_MAP_NUM_REGIONS
#endif

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
/** \brief Aligned address at which the Board Config header is placed. */
#define SCISERVER_BOARDCONFIG_HEADER_ADDR (0x41c80000U)

/** \brief Aligned address at which the Board Config is placed. */
#define SCISERVER_BOARDCONFIG_DATA_ADDR (0x41c80040U)

#endif

/* Global array which contains firewall data */
const sblFwlData gFwlDisableInfo [] =
{
#if defined(SOC_J721E)
    /* Firewall data for MCU_FSS0_S0 */
    {
        MCU_FSS0_S0_FWID,
        MCU_FSS0_S0_FW_REGIONS
    },
    {
        MCU_FSS0_S1_FWID,
        MCU_FSS0_S1_FW_REGIONS
    },
    /* Firewall data for OCMC Region */
    {
        OCMC_FWID,
        OCMC_REGIONS
    },
    /* Firewall data for PCIE0 Region */
    {
        CSL_STD_FW_PCIE0_PCIE_CFG_ID,
        CSL_STD_FW_PCIE0_PCIE_CFG_NUM_REGIONS
    },
    /* Firewall data for PCIE1 Region */
    {
        CSL_STD_FW_PCIE1_PCIE_CFG_ID,
        CSL_STD_FW_PCIE1_PCIE_CFG_NUM_REGIONS
    }
#else
    /* Firewall data for MCU_FSS0_S0 */
    {
        MCU_FSS0_S0_FWID,
        MCU_FSS0_S0_FW_REGIONS
    },
    {
        MCU_FSS0_S1_FWID,
        MCU_FSS0_S1_FW_REGIONS
    },
    /* Firewall data for OCMC Region */
    {
        OCMC_FWID,
        OCMC_REGIONS
    }
#endif
};

uint32_t SBL_IsAuthReq(void)
{
    uint32_t retVal = SBL_ALWAYS_AUTH_APP;
    uint32_t dev_type;
    uint32_t dev_subtype;

    dev_type = CSL_REG32_RD(SBL_SYS_STATUS_REG) & SBL_SYS_STATUS_DEV_TYPE_MASK;
    dev_subtype = CSL_REG32_RD(SBL_SYS_STATUS_REG) & SBL_SYS_STATUS_DEV_SUBTYPE_MASK;

    /* No auth possible, if valid SMPK/BMPK is not present */
    if ((dev_subtype == SBL_SYS_STATUS_DEV_SUBTYPE_FS) ||
        (dev_type == SBL_SYS_STATUS_DEV_TYPE_GP) ||
        (dev_type == SBL_SYS_STATUS_DEV_TYPE_TEST))
    {
        retVal = SBL_NEVER_AUTH_APP;
    }

    return retVal;
}
uint16_t gCertLengthPm = 0;
#ifndef SBL_SKIP_SYSFW_INIT
Sciclient_BoardCfgPrms_t sblBoardCfgPrms = {0};
Sciclient_BoardCfgPrms_t sblBoardCfgPmPrms = {0};
Sciclient_BoardCfgPrms_t sblBoardCfgRmPrms = {0};
#ifndef SBL_SKIP_BRD_CFG_SEC
Sciclient_BoardCfgPrms_t sblBoardCfgSecPrms = {0};
#endif
#endif

#ifndef SBL_SKIP_BRD_CFG_RM
uint16_t gCertLength = 0;
#endif
static uint16_t boardcfgRmFindCertSize(uint32_t *msg_recv)
{
    uint16_t cert_len = 0U;
    uint8_t *cert_len_ptr = (uint8_t *)&cert_len;
    uint8_t *x509_cert_ptr;

    x509_cert_ptr = (uint8_t *)msg_recv;


    if (*x509_cert_ptr != 0x30)
    {
        /* The data does not contain a certificate - return */
        return 0;
    }

    cert_len = *(x509_cert_ptr + 1);

    /* If you need more than 2 bytes to store the cert length  */
    /* it means that the cert length is greater than 64 Kbytes */
    /* and we do not support it                                */
    if ((cert_len > 0x80U) &&
        (cert_len != 0x82U))
    {
        return 0;
    }

    if (cert_len == 0x82U)
    {
        *cert_len_ptr = *(x509_cert_ptr + 3);
        *(cert_len_ptr + 1) = *(x509_cert_ptr + 2);

        /* add current offset from start of x509 cert */
        cert_len += 3U;
    }
    else
    {
        /* add current offset from start of x509 cert  */
        /* if cert len was obtained from 2nd byte i.e. */
        /* cert size is 127 bytes or less              */
        cert_len += 1U;
    }

    /* cert_len now contains the offset of the last byte */
    /* of the cert from the ccert_start. To get the size */
    /* of certificate, add 1                             */

    return cert_len + 1U;
}




void SBL_SciclientBoardCfg(uint32_t devGroup, Sciclient_DefaultBoardCfgInfo_t *boardCfgInfo)
{
    int32_t status = CSL_EFAIL;
    sblBoardCfgPrms.boardConfigLow = (uint32_t)boardCfgInfo->boardCfgLow;
    sblBoardCfgPrms.boardConfigHigh = 0U;
    sblBoardCfgPrms.boardConfigSize = boardCfgInfo->boardCfgLowSize;
    sblBoardCfgPrms.devGrp = devGroup;


    status = Sciclient_boardCfg(&sblBoardCfgPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient board config ...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
}

void SBL_SciclientBoardCfgPm(uint32_t devGroup, Sciclient_DefaultBoardCfgInfo_t *boardCfgInfo)
{
    int32_t status = CSL_EFAIL;
    if (SBL_LOG_LEVEL > SBL_LOG_NONE)
    {
        UART_stdioDeInit();
    }
    gCertLengthPm = boardcfgRmFindCertSize((uint32_t*)boardCfgInfo->boardCfgLowPm);
    CSL_armR5PmuSetCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, CNTR_RELOAD_VALUE);
    SBL_ADD_PROFILE_POINT;
    sblBoardCfgPmPrms.boardConfigLow = (uint32_t)boardCfgInfo->boardCfgLowPm;
    sblBoardCfgPmPrms.boardConfigHigh = 0U;
    sblBoardCfgPmPrms.boardConfigSize = boardCfgInfo->boardCfgLowPmSize;
    sblBoardCfgPmPrms.devGrp = devGroup;

    status = Sciclient_boardCfgPm(&sblBoardCfgPmPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient board config pm...FAILED \n")
        SblErrLoop(__FILE__, __LINE__);
    }

    if (SBL_LOG_LEVEL > SBL_LOG_NONE)
    {
        /* Re-init UART for logging */
        UART_HwAttrs uart_cfg;

        UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        uart_cfg.frequency = SBL_SYSFW_UART_MODULE_INPUT_CLK;
        UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        UART_stdioInit(BOARD_UART_INSTANCE);
    }
}

void SBL_SciclientCfgSec(uint32_t devGroup, Sciclient_DefaultBoardCfgInfo_t *boardCfgInfo)
{
    int32_t status = CSL_EFAIL;
    sblBoardCfgSecPrms.boardConfigLow = (uint32_t)boardCfgInfo->boardCfgLowSec;
    sblBoardCfgSecPrms.boardConfigHigh = 0U;
    sblBoardCfgSecPrms.boardConfigSize = boardCfgInfo->boardCfgLowSecSize;
    sblBoardCfgSecPrms.devGrp = devGroup;

    status = Sciclient_boardCfgSec(&sblBoardCfgSecPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient board config sec...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
}

void SBL_OpenFirewalls()
{
#ifndef SBL_SKIP_BRD_CFG_SEC
    int32_t status = CSL_EFAIL;
    int32_t fwlDataSize = sizeof(gFwlDisableInfo)/sizeof(gFwlDisableInfo[0]);
    struct tisci_msg_fwl_get_firewall_region_req reqGetFwlCtrl;
    for (int32_t loopCnt = 0; loopCnt < fwlDataSize; loopCnt++)
    {
        memset(&reqGetFwlCtrl, 0, sizeof(reqGetFwlCtrl));
        reqGetFwlCtrl = (struct tisci_msg_fwl_get_firewall_region_req) {
            .fwl_id = gFwlDisableInfo[loopCnt].fwlId
        };

        /* Remove Foreground firewall regions */
        status = SBL_disableFwlRegion(&reqGetFwlCtrl, gFwlDisableInfo[loopCnt].numRegions, SBL_FWL_REGION_FOREGROUND);
        if (status != CSL_PASS)
        {
            SBL_log(SBL_LOG_ERR, "Failed to disable foreground region firewall of ID %d !! \n", gFwlDisableInfo[loopCnt].fwlId);
        }
        /* Remove Background firewall regions */
        status = SBL_disableFwlRegion(&reqGetFwlCtrl, gFwlDisableInfo[loopCnt].numRegions, SBL_FWL_REGION_BACKGROUND);
        if (status != CSL_PASS)
        {
            SBL_log(SBL_LOG_ERR, "Failed to disable background region firewall of ID %d !! \n", gFwlDisableInfo[loopCnt].fwlId);
        }
    }
#endif
}

void SBL_SciclientBoardCfgRm(uint32_t devGroup, Sciclient_DefaultBoardCfgInfo_t *boardCfgInfo)
{
    int32_t status = CSL_EFAIL;
    sblBoardCfgRmPrms.boardConfigLow = (uint32_t)boardCfgInfo->boardCfgLowRm;
    sblBoardCfgRmPrms.boardConfigHigh = 0U;
    sblBoardCfgRmPrms.boardConfigSize = boardCfgInfo->boardCfgLowRmSize;
    sblBoardCfgRmPrms.devGrp = devGroup;

    gCertLength = boardcfgRmFindCertSize((uint32_t*)boardCfgInfo->boardCfgLowRm);
    status = Sciclient_boardCfgRm(&sblBoardCfgRmPrms);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient board config rm...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
}

void SBL_getSysfwVersion()
{
    int32_t status = CSL_EFAIL;
    struct tisci_msg_version_req req = {0};
    const Sciclient_ReqPrm_t      reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (const uint8_t *)&req,
        sizeof(req),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm =
    {
        0,
        (uint8_t *) &response,
        (uint32_t)sizeof (response)
    };

    status = Sciclient_service(&reqPrm, &respPrm);
    if (CSL_PASS == status)
    {
        if (respPrm.flags == (uint32_t)TISCI_MSG_FLAG_ACK)
        {
            /* These profile points are added to remove tifs revision print time */
            SBL_ADD_PROFILE_POINT;
            SBL_log(SBL_LOG_MIN,"TIFS  ver: %s\n", (char *) response.str);
            SBL_ADD_PROFILE_POINT;
        }
        else
        {
            SBL_log(SBL_LOG_ERR,"TIFS Get Version failed \n");
            SblErrLoop(__FILE__, __LINE__);
        }
    }

}

void SBL_SciClientInit(uint32_t devGroup)
{
    int32_t status = CSL_EFAIL;

#ifndef SBL_SKIP_SYSFW_INIT
    /* SYSFW board configurations */
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
    Sciclient_ConfigPrms_t config;
    Sciclient_configPrmsInit(&config);
    config.opModeFlag               =   SCICLIENT_SERVICE_OPERATION_MODE_POLLED;
    config.pBoardCfgPrms            =   NULL;
    config.isSecureMode             =   0; /* default board cfg is for non-secure mode */
    config.c66xRatRegion            =   0;
    config.skipLocalBoardCfgProcess =   1;
#endif

    void *sysfw_ptr = gSciclient_firmware;
    status = SBL_ReadSysfwImage(&sysfw_ptr, SBL_SYSFW_MAX_SIZE);
    if (status != CSL_PASS)
    {
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
        SBL_log(SBL_LOG_ERR,"TIFS read...FAILED \n");
#else
        SBL_log(SBL_LOG_ERR,"SYSFW read...FAILED \n");
#endif
        SblErrLoop(__FILE__, __LINE__);
    }
    /* Profile point after reading sysfw and before loading sysfw */
    SBL_ADD_PROFILE_POINT;

#ifndef SBL_SKIP_SYSFW_INIT

    status = Sciclient_loadFirmware((const uint32_t *) sysfw_ptr);
    if (status != CSL_PASS)
    {
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
        SBL_log(SBL_LOG_ERR,"TIFS load...FAILED \n");
#else
        SBL_log(SBL_LOG_ERR,"SYSFW load...FAILED \n");
#endif
        SblErrLoop(__FILE__, __LINE__);
    }
    /* Profile point after loading sysfw and before sciclient init */
    SBL_ADD_PROFILE_POINT;

    status = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);

    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient get default board config...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }

    status = Sciclient_init(&config);
    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient init ...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }
    /* Profile point after sciclient init and before Board Cfg */
    SBL_ADD_PROFILE_POINT;

#ifndef SBL_SKIP_BRD_CFG_BOARD
    SBL_SciclientBoardCfg(devGroup, &boardCfgInfo);
#endif
    /* Profile point after Board Cfg and before PM Board Cfg */
    SBL_ADD_PROFILE_POINT;

#ifndef SBL_SKIP_BRD_CFG_PM
    SBL_SciclientBoardCfgPm(devGroup, &boardCfgInfo);
#endif
    /* Profile point after PM Board Cfg and before Security Board Cfg */
    SBL_ADD_PROFILE_POINT;

#ifndef SBL_SKIP_BRD_CFG_SEC
    SBL_SciclientCfgSec(devGroup, &boardCfgInfo);
#endif

    SBL_OpenFirewalls();
    /* Profile point after Security Board Cfg and before RM Board Cfg */
    SBL_ADD_PROFILE_POINT;

#ifndef SBL_SKIP_BRD_CFG_RM
    SBL_SciclientBoardCfgRm(devGroup, &boardCfgInfo);
#endif
    /* Profile point after RM Board Cfg and before Board init pinmux */
    SBL_ADD_PROFILE_POINT;

/* Get SYSFW/TIFS version */
    SBL_getSysfwVersion();


#if (!defined(SBL_SKIP_BRD_CFG_PM)) || (!defined(SBL_SKIP_BRD_CFG_RM))
    status = Sciclient_setBoardConfigHeader();
    if (CSL_PASS == status)
    {
        SBL_log(SBL_LOG_MAX,"Sciclient_setBoardConfigHeader... PASSED\n");
    }
    else
    {
        SBL_log(SBL_LOG_ERR,"Sciclient_setBoardConfigHeader... FAILED\n");
        SblErrLoop(__FILE__, __LINE__);
    }
#endif

#if !defined(SBL_SKIP_MCU_RESET)
    /* RTI seems to be turned on by ROM. Turning it off so that Power domain can transition */
    Sciclient_pmSetModuleState(SBL_DEV_ID_RTI0, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(SBL_DEV_ID_RTI1, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
#endif
#endif
}

void SBL_SciClientCombinedBootInit(uint32_t devGroup)
{
    int32_t status = CSL_EFAIL;
    combinedBootmode = 1;

#ifndef SBL_SKIP_SYSFW_INIT
    /* SYSFW board configurations */
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
    Sciclient_ConfigPrms_t config;
    Sciclient_configPrmsInit(&config);
    config.opModeFlag               =   SCICLIENT_SERVICE_OPERATION_MODE_POLLED;
    config.pBoardCfgPrms            =   NULL;
    config.isSecureMode             =   0; /* default board cfg is for non-secure mode */
    config.c66xRatRegion            =   0;
    config.skipLocalBoardCfgProcess =   0;
#endif

#ifndef SBL_SKIP_SYSFW_INIT
    status = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);

    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient get default board config...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }

    CSL_armR5PmuSetCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, CNTR_RELOAD_VALUE);
    SBL_ADD_PROFILE_POINT;

    status = Sciclient_init(&config);
    /* Profile point after sciclient init and before board init pinmux */
    SBL_ADD_PROFILE_POINT;

    if (SBL_LOG_LEVEL > SBL_LOG_NONE)
    {
        /* De-init and Re-init UART for logging */
        UART_stdioDeInit();
        UART_HwAttrs uart_cfg;

        UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        uart_cfg.frequency = SBL_SYSFW_UART_MODULE_INPUT_CLK;
        UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        UART_stdioInit(BOARD_UART_INSTANCE);
    }

    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"Sciclient init ...FAILED \n");
        SblErrLoop(__FILE__, __LINE__);
    }

    SBL_OpenFirewalls();

/* Get SYSFW/TIFS version */
    SBL_getSysfwVersion();

#if (!defined(SBL_SKIP_BRD_CFG_PM)) || (!defined(SBL_SKIP_BRD_CFG_RM))
    status = Sciclient_setBoardConfigHeader();
    if (CSL_PASS == status)
    {
        SBL_log(SBL_LOG_MAX,"Sciclient_setBoardConfigHeader... PASSED\n");
    }
    else
    {
        SBL_log(SBL_LOG_ERR,"Sciclient_setBoardConfigHeader... FAILED\n");
        SblErrLoop(__FILE__, __LINE__);
    }
#endif

#if !defined(SBL_SKIP_MCU_RESET)
    /* RTI seems to be turned on by ROM. Turning it off so that Power domain can transition */
    Sciclient_pmSetModuleState(SBL_DEV_ID_RTI0, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(SBL_DEV_ID_RTI1, TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF, TISCI_MSG_FLAG_AOP, SCICLIENT_SERVICE_WAIT_FOREVER);
#endif
#endif
}

#if (!defined(SBL_SKIP_BRD_CFG_PM)) || (!defined(SBL_SKIP_BRD_CFG_RM))
static int32_t Sciclient_setBoardConfigHeader ()
{
    int32_t status = CSL_PASS;
#if defined (SOC_J7200) || defined (SOC_J721E) || defined (SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    //uint32_t alignedOffset = ((SCICLIENT_BOARDCFG_PM_SIZE_IN_BYTES + 128U)/128U)*128U;
    uint32_t alignedOffset = SCICLIENT_BOARDCFG_PM_SIZE_IN_BYTES;
    Sciclient_BoardCfgPrms_t boardCfgPrms_pm =
    {
        .boardConfigLow = (uint32_t)SCISERVER_BOARDCONFIG_DATA_ADDR,
        .boardConfigHigh = 0U,
        .boardConfigSize = (uint16_t)SCICLIENT_BOARDCFG_PM_SIZE_IN_BYTES - gCertLengthPm,
        .devGrp = DEVGRP_ALL
    };
    Sciclient_BoardCfgPrms_t boardCfgPrms_rm =
    {
        .boardConfigLow =
            (uint32_t) SCISERVER_BOARDCONFIG_DATA_ADDR + alignedOffset,
        .boardConfigHigh = 0U,
        .boardConfigSize = (uint16_t)SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES - gCertLength,
        .devGrp = DEVGRP_ALL
    };

    if (1U != combinedBootmode)
    {
        status = Sciclient_boardCfgPrepHeader (
        (uint8_t *) SCISERVER_COMMON_X509_HEADER_ADDR,
        (uint8_t *) SCISERVER_BOARDCONFIG_HEADER_ADDR,
        &boardCfgPrms_pm, &boardCfgPrms_rm);
        if (CSL_PASS == status)
        {
            SBL_log(SBL_LOG_MAX,"SCISERVER Board Configuration header population... ");
            SBL_log(SBL_LOG_MAX,"PASSED\n");
        }
        else
        {
            SBL_log(SBL_LOG_MIN,"SCISERVER Board Configuration header population... ");
            SBL_log(SBL_LOG_MIN,"FAILED\n");
        }
        memcpy((void *)boardCfgPrms_pm.boardConfigLow, (void *) sblBoardCfgPmPrms.boardConfigLow, SCICLIENT_BOARDCFG_PM_SIZE_IN_BYTES - gCertLengthPm);
        memcpy((void *)boardCfgPrms_rm.boardConfigLow, (void *) sblBoardCfgRmPrms.boardConfigLow, SCICLIENT_BOARDCFG_RM_SIZE_IN_BYTES - gCertLength);
    }
    else
    {
        status = Sciclient_boardCfgParseHeader(
            (uint8_t *) SCISERVER_COMMON_X509_HEADER_ADDR,
            &boardCfgPrms_pm, &boardCfgPrms_rm);
        if (CSL_PASS == status)
        {
            SBL_log(SBL_LOG_MAX,"SCISERVER Board Cfg Parsing.... Passed \n");
        }
        else
        {
            SBL_log(SBL_LOG_MIN,"SCISERVER Board Cfg Parsing.... Failed \n");
        }
    }
#endif
    return status;
}
#endif

int32_t SBL_disableFwlRegion(struct tisci_msg_fwl_get_firewall_region_req* reqGetFwlCtrl, uint32_t numRegions, uint32_t disableRegion)
{
    int32_t retVal = CSL_PASS;
    uint32_t currentRegionType;
    struct tisci_msg_fwl_get_firewall_region_resp respGetFwlCtrl;
    struct tisci_msg_fwl_set_firewall_region_resp respSetFwlCtrl;
    struct tisci_msg_fwl_set_firewall_region_req reqSetFwlCtrl;
    memset(&respGetFwlCtrl, 0, sizeof(respGetFwlCtrl));
    memset(&respSetFwlCtrl, 0, sizeof(respSetFwlCtrl));
    memset(&reqSetFwlCtrl, 0, sizeof(reqSetFwlCtrl));

    /* Looping over all the regions for that particular Firewall ID */
    for(uint32_t regionNum = 0; regionNum < numRegions; regionNum++)
    {
        reqGetFwlCtrl->region = regionNum;
        retVal = Sciclient_firewallGetRegion(reqGetFwlCtrl, &respGetFwlCtrl, SCICLIENT_SERVICE_WAIT_FOREVER);
        currentRegionType = ((respGetFwlCtrl.control & SBL_FWL_REGION_MASK) >> SBL_FWL_REGION_SHIFT);
        if (retVal != CSL_PASS){
            SBL_log(SBL_LOG_ERR,"Sciclient_firewallGetRegion failed !!\n");
        }
        else
        {
            /* Check whether firewall is enabled */
            if (respGetFwlCtrl.control != 0)
            {
                reqSetFwlCtrl = (struct tisci_msg_fwl_set_firewall_region_req) {
                    .fwl_id = respGetFwlCtrl.fwl_id,
                    .region = regionNum,
                    /* Set control to zero to disable the firewall region */
                    .control = (uint32_t) 0U,
                    .start_address = respGetFwlCtrl.start_address,
                    .end_address = respGetFwlCtrl.end_address
                };

                if (disableRegion == currentRegionType)
                {
                    retVal = Sciclient_firewallSetRegion(&reqSetFwlCtrl, &respSetFwlCtrl, SCICLIENT_SERVICE_WAIT_FOREVER);
                    if (retVal != CSL_PASS)
                    {
                        SBL_log(SBL_LOG_ERR,"Firewall region (background) # %d of ID %d disable...FAILED \n", regionNum, reqGetFwlCtrl->fwl_id);
                    }
                }
            }
        }
    }
    return retVal;
}

int32_t SBL_SciclientReconfigureFirewalls()
{
    int32_t status = CSL_EFAIL;
    const uint32_t testDevgrp = DEVGRP_01;
    struct tisci_msg_init_fwl_devgrp_req initReq = {0};
    struct tisci_msg_init_fwl_devgrp_resp initResp = {0};

    /* Call Sciclient_firewallInitDevgrp with devgrp 1 */
    initReq.dgrp = testDevgrp;

    status = Sciclient_firewallInitDevgrp(&initReq, &initResp, SCICLIENT_SERVICE_WAIT_FOREVER);

    if (status != CSL_PASS)
    {
        SBL_log(SBL_LOG_ERR,"ERROR: Sciclient_firewallInitDevgrp failed with status: %d\n", status);
    }
    else
    {
        SBL_log(SBL_LOG_MAX,"Firewall devgrp %d initialized successfully\n", testDevgrp);
    }

    return status;
}

