/**
 *  \file   main.c
 *
 *  \brief  This file contains main function, UART init functions & call to
 *  OTP Keywriter API
 *
 */

/*
 * Copyright (C) 2020-2024 Texas Instruments Incorporated - http://www.ti.com/
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

#include "main.h"

extern uint32_t keywr_end;

#define KEYWRITER_BIN_WORD_SIZE (TIFS_KEYWRITER_BIN_SIZE_IN_BYTES + 3) / 4

/* These two macros define size, and hex code of system firmware Key writer binary */
uint32_t __attribute((section(".data:kw_firmware")))
    gSciclient_firmware[KEYWRITER_BIN_WORD_SIZE] = TIFS_KEYWRITER_BIN;

Sciclient_BoardCfgPrms_t sblBoardCfgPrms = {0};
Sciclient_BoardCfgPrms_t sblBoardCfgPmPrms = {0};
Sciclient_BoardCfgPrms_t sblBoardCfgSecPrms = {0};

#if defined(SBL_COMBINED_BOOT)
static void OTP_sciClientCombinedBootInit(void)
{
    int32_t status = CSL_EFAIL;
    UART_HwAttrs uart_cfg;

    /* SYSFW board configurations */
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
    Sciclient_ConfigPrms_t config;
    Sciclient_configPrmsInit(&config);
    config.opModeFlag               =   SCICLIENT_SERVICE_OPERATION_MODE_POLLED;
    config.pBoardCfgPrms            =   NULL;
    config.isSecureMode             =   0;
    config.c66xRatRegion            =   0;
    config.skipLocalBoardCfgProcess =   1;
    
    status = Sciclient_bootNotification();
    if (status != CSL_PASS)
    {
        UART_printf("Sciclient_bootNotification ...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }
    
    /* Use snapshot of sciclient boardconfigs */
    boardCfgInfo.boardCfgLow        = &gKeywr_boardCfgLow[0U];
    boardCfgInfo.boardCfgLowRm      = &gKeywr_boardCfgLow_rm[0U];
    boardCfgInfo.boardCfgLowSec     = &gKeywr_boardCfgLow_sec[0U];
    boardCfgInfo.boardCfgLowPm      = &gKeywr_boardCfgLow_pm[0U];
    boardCfgInfo.boardCfgLowSize    = KEYWR_BOARDCFG_SIZE_IN_BYTES;
    boardCfgInfo.boardCfgLowRmSize  = KEYWR_BOARDCFG_RM_SIZE_IN_BYTES;
    boardCfgInfo.boardCfgLowSecSize = KEYWR_BOARDCFG_SECURITY_SIZE_IN_BYTES;
    boardCfgInfo.boardCfgLowPmSize  = KEYWR_BOARDCFG_PM_SIZE_IN_BYTES;

    status = Sciclient_init(&config);
    if (status != CSL_PASS)
    {
        UART_printf("Sciclient init ...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }
    
    sblBoardCfgPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLow;
    sblBoardCfgPrms.boardConfigHigh = 0U;
    sblBoardCfgPrms.boardConfigSize = boardCfgInfo.boardCfgLowSize;
    sblBoardCfgPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfg(&sblBoardCfgPrms);

    if (CSL_PASS != status)
    {
        UART_printf("Sciclient board config ...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }

    UART_stdioDeInit();

    sblBoardCfgPmPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLowPm;
    sblBoardCfgPmPrms.boardConfigHigh = 0U;
    sblBoardCfgPmPrms.boardConfigSize = boardCfgInfo.boardCfgLowPmSize;
    sblBoardCfgPmPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgPm(&sblBoardCfgPmPrms);

    if (CSL_PASS != status)
    {
        /* Sciclient board config pm..FAILS */
        KeywrErrLoop(__FILE__, __LINE__);
    }

    /* Re-init UART for logging */
    UART_socGetInitCfg(KEYWRITER_BOARD_UART_INSTANCE, &uart_cfg);
    uart_cfg.frequency = SBL_SYSFW_UART_MODULE_INPUT_CLK;
    UART_socSetInitCfg(KEYWRITER_BOARD_UART_INSTANCE, &uart_cfg);
    UART_stdioInit(KEYWRITER_BOARD_UART_INSTANCE);

    sblBoardCfgSecPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLowSec;
    sblBoardCfgSecPrms.boardConfigHigh = 0U;
    sblBoardCfgSecPrms.boardConfigSize = boardCfgInfo.boardCfgLowSecSize;
    sblBoardCfgSecPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgSec(&sblBoardCfgSecPrms);

    if (CSL_PASS != status)
    {
        UART_printf("Sciclient board config sec...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }

    /* Skipping board cfg RM, since it is not included in keywriter firmware */
    /* Print System Firmware Version Information */
    struct tisci_msg_version_req req     = {0};
    const Sciclient_ReqPrm_t      reqPrm = {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (const uint8_t *)&req,
        sizeof(req),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm = {
        0,
        (uint8_t *)&response,
        (uint32_t)sizeof(response)
    };

    status = Sciclient_service(&reqPrm, &respPrm);

    if (CSL_PASS == status)
    {
        if ((uint32_t)TISCI_MSG_FLAG_ACK == respPrm.flags)
        {
            UART_printf("\n OTP Keywriter ver: %s\n", (char *)response.str);
        }
        else
        {
            UART_printf(" OTP Keywriter Get Version failed \n");
            KeywrErrLoop(__FILE__, __LINE__);
        }
    }

}
#else
static void OTP_SciClientInit(void)
{
    int32_t status  = CSL_EFAIL;
    void *sysfw_ptr = gSciclient_firmware;
    UART_HwAttrs uart_cfg;

    /* SYSFW board configurations */
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
    Sciclient_ConfigPrms_t        config = {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
        NULL,
        UTRUE,
        0U,
        1U
    };

    /* Use snapshot of sciclient boardconfigs */
    boardCfgInfo.boardCfgLow        = &gKeywr_boardCfgLow[0U];
    boardCfgInfo.boardCfgLowRm      = &gKeywr_boardCfgLow_rm[0U];
    boardCfgInfo.boardCfgLowSec     = &gKeywr_boardCfgLow_sec[0U];
    boardCfgInfo.boardCfgLowPm      = &gKeywr_boardCfgLow_pm[0U];
    boardCfgInfo.boardCfgLowSize    = KEYWR_BOARDCFG_SIZE_IN_BYTES;
    boardCfgInfo.boardCfgLowRmSize  = KEYWR_BOARDCFG_RM_SIZE_IN_BYTES;
    boardCfgInfo.boardCfgLowSecSize = KEYWR_BOARDCFG_SECURITY_SIZE_IN_BYTES;
    boardCfgInfo.boardCfgLowPmSize  = KEYWR_BOARDCFG_PM_SIZE_IN_BYTES;

    status = Sciclient_loadFirmware((const uint32_t *)sysfw_ptr);

    if (CSL_PASS != status)
    {
        UART_printf("TIFS load...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }

    status = Sciclient_init(&config);

    if (CSL_PASS != status)
    {
        UART_printf("Sciclient init ...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }

    sblBoardCfgPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLow;
    sblBoardCfgPrms.boardConfigHigh = 0U;
    sblBoardCfgPrms.boardConfigSize = boardCfgInfo.boardCfgLowSize;
    sblBoardCfgPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfg(&sblBoardCfgPrms);

    if (CSL_PASS != status)
    {
        UART_printf("Sciclient board config ...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }

    UART_stdioDeInit();

    sblBoardCfgPmPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLowPm;
    sblBoardCfgPmPrms.boardConfigHigh = 0U;
    sblBoardCfgPmPrms.boardConfigSize = boardCfgInfo.boardCfgLowPmSize;
    sblBoardCfgPmPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgPm(&sblBoardCfgPmPrms);

    if (CSL_PASS != status)
    {
        /* Sciclient board config pm..FAILS */
        KeywrErrLoop(__FILE__, __LINE__);
    }

    /* Re-init UART for logging */
    UART_socGetInitCfg(KEYWRITER_BOARD_UART_INSTANCE, &uart_cfg);
    uart_cfg.frequency = SBL_SYSFW_UART_MODULE_INPUT_CLK;
    UART_socSetInitCfg(KEYWRITER_BOARD_UART_INSTANCE, &uart_cfg);
    UART_stdioInit(KEYWRITER_BOARD_UART_INSTANCE);

    sblBoardCfgSecPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLowSec;
    sblBoardCfgSecPrms.boardConfigHigh = 0U;
    sblBoardCfgSecPrms.boardConfigSize = boardCfgInfo.boardCfgLowSecSize;
    sblBoardCfgSecPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgSec(&sblBoardCfgSecPrms);

    if (CSL_PASS != status)
    {
        UART_printf("Sciclient board config sec...FAILED \n");
        KeywrErrLoop(__FILE__, __LINE__);
    }

    /* Skipping board cfg RM, since it is not included in keywriter firmware */

    /* Print System Firmware Version Information */
    struct tisci_msg_version_req req     = {0};
    const Sciclient_ReqPrm_t      reqPrm = {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (const uint8_t *)&req,
        sizeof(req),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };

    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm = {
        0,
        (uint8_t *)&response,
        (uint32_t)sizeof(response)
    };

    status = Sciclient_service(&reqPrm, &respPrm);

    if (CSL_PASS == status)
    {
        if ((uint32_t)TISCI_MSG_FLAG_ACK == respPrm.flags)
        {
            UART_printf("\n OTP Keywriter ver: %s\n", (char *)response.str);
        }
        else
        {
            UART_printf(" OTP Keywriter Get Version failed \n");
            KeywrErrLoop(__FILE__, __LINE__);
        }
    }
}
#endif

static void mmr_unlock(uint32_t base, uint32_t partition)
{
    /*  Translate the base address */
    uint32_t part_base = base + partition * CTRL_MMR0_PARTITION_SIZE;

    HW_WR_REG32(part_base + CTRLMMR_LOCK_KICK0, CTRLMMR_LOCK_KICK0_UNLOCK_VAL);
    HW_WR_REG32(part_base + CTRLMMR_LOCK_KICK1, CTRLMMR_LOCK_KICK1_UNLOCK_VAL);
}

int main()
{
    int32_t status = CSL_EFAIL;
    uint32_t debug_response  = 0U;
    uint32_t *keywriter_cert = &keywr_end + 1U;
    UART_HwAttrs uart_cfg;

    /* padconfig unlock */
    mmr_unlock(WKUP_CTRL_BASE, 7);
    /* pinmux for R5 logs */
    HW_WR_REG32(BOARD_MCU_UART_TX_PINMUX_ADDR, PIN_OUTPUT | PIN_MODE(0));
    /* pinmux for M3 logs */
    HW_WR_REG32(WKUP_UART_TXD_MUX_ADDR, PIN_OUTPUT | PIN_MODE(0));

    UART_socGetInitCfg(KEYWRITER_BOARD_UART_INSTANCE, &uart_cfg);
#if defined(SBL_COMBINED_BOOT)
    uart_cfg.frequency       = SBL_SYSFW_UART_MODULE_INPUT_CLK;
#else
    uart_cfg.frequency       = SBL_ROM_UART_MODULE_INPUT_CLK;
#endif
    uart_cfg.enableInterrupt = UFALSE;
    UART_socSetInitCfg(KEYWRITER_BOARD_UART_INSTANCE, &uart_cfg);
    UART_stdioInit(KEYWRITER_BOARD_UART_INSTANCE);

    UART_printf("%s (%s - %s)\n", OTP_VERSION_STR, __DATE__, __TIME__);

#if defined(SBL_COMBINED_BOOT)
    OTP_sciClientCombinedBootInit();
    UART_printf("Combined Boot Mode\n");
#else
    OTP_SciClientInit();
    UART_printf("Legacy Boot Mode\n");
#endif

    OTP_VppEn();

    UART_printf("Key programming sequence initialted\n");
    UART_printf("Taking OTP certificate from 0x%x\n",
                (uint32_t *)keywriter_cert);
    status = Sciclient_otpProcessKeyCfg((uint32_t *)keywriter_cert,
                                        SCICLIENT_SERVICE_WAIT_FOREVER,
                                        &debug_response);

    if (CSL_PASS != status)
    {
        UART_printf("Sciclient_otpProcessKeyCfg returns: %d\n", status);
    }

    UART_printf("Debug response: 0x%x\n", debug_response);

    UART_printf("Key programming sequence completed\n");

    asm volatile (" wfi");

    return 0;
}
