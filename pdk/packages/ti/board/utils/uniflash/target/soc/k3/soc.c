/*
 * Copyright (C) 2019-2021 Texas Instruments Incorporated - http://www.ti.com/
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "soc_k3.h"
#include "flash_programmer.h"

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(am64x_evm)
uint32_t uart_baseAddr = CSL_UART0_BASE;
#else
uint32_t uart_baseAddr = CSL_MCU_UART0_BASE;
#endif
uint8_t uart_inst = BOARD_UART_INSTANCE;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void _resetvectors (void);
int8_t UFP_openUartHandle(void);
void utilsCopyVecs2ATcm(void);
void ufpTcmEn(void);

#if defined(am64x_evm)
static int32_t UFP_isNoBootEnabled(void)
{
    uint32_t mainDevStat;

    mainDevStat = HW_RD_REG32(UFP_MAIN_DEVSTAT_ADDR);

    if((mainDevStat & UFP_MAIN_DEVSTAT_NOBOOT_MASK) == UFP_MAIN_DEVSTAT_NOBOOT_CFG)
    {
        return (TRUE);
    }

    return (FALSE);
}
#else
static int32_t UFP_isNoBootEnabled(void)
{
    uint32_t mainDevStat;
    uint32_t wkupDevStat;

    mainDevStat = HW_RD_REG32(UFP_MAIN_DEVSTAT_ADDR);
    wkupDevStat = HW_RD_REG32(UFP_WKUP_DEVSTAT_ADDR);

    if(((mainDevStat & UFP_MAIN_DEVSTAT_NOBOOT_MASK) == UFP_MAIN_DEVSTAT_NOBOOT_CFG) &&
       ((wkupDevStat & UFP_WKUP_DEVSTAT_NOBOOT_MASK) == UFP_WKUP_DEVSTAT_NOBOOT_CFG))
    {
        return (1);
    }

    return (0);
}
#endif

static void UFP_initUARTPwrClk(void)
{
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    HW_WR_REG32(UFP_UART_PLL_BASE + UFP_UART_PLL_KICK0_OFFSET, UFP_UART_PLL_KICK0_UNLOCK_VAL);
    HW_WR_REG32(UFP_UART_PLL_BASE + UFP_UART_PLL_KICK1_OFFSET, UFP_UART_PLL_KICK1_UNLOCK_VAL);

    HW_WR_REG32(UFP_UART_PLL_BASE + UFP_UART_PLL_DIV_OFFSET, UFP_UART_PLL_DIV_VAL);

    HW_WR_REG32(UFP_UART_PLL_BASE + UFP_UART_PLL_KICK0_OFFSET, UFP_UART_PLL_KICK_LOCK_VAL);
    HW_WR_REG32(UFP_UART_PLL_BASE + UFP_UART_PLL_KICK1_OFFSET, UFP_UART_PLL_KICK_LOCK_VAL);
#endif
}

static uint32_t UFP_GetCertLen(uint8_t *x509_cert_ptr)
{
    uint32_t cert_len = 0;
    uint8_t *cert_len_ptr = (uint8_t *)&cert_len;

    if ( *x509_cert_ptr != 0x30)
    {
        return 0;
    }

    cert_len = *(x509_cert_ptr + 1);

    /* If you need more than 2 bytes to store the cert length  */
    /* it means that the cert length is greater than 64 Kbytes */
    /* and we do not support it                                */
    if ((cert_len > 0x80) &&
        (cert_len != 0x82))
    {
        return 0;
    }

    if ( cert_len == 0x82)
    {
        *cert_len_ptr = *(x509_cert_ptr + 3);
        *(cert_len_ptr + 1) = *(x509_cert_ptr + 2);

        /* add current offset from start of x509 cert */
        cert_len += 3;
    }
    else
    {
        /* add current offset from start of x509 cert  */
        /* if cert len was obtained from 2nd byte i.e. */
        /* cert size is 127 bytes or less              */
        cert_len += 1;
    }

    return cert_len + 1;
}

static uint8_t *UFP_findSeq(uint8_t *x509_cert_ptr, uint32_t x509_cert_size, uint8_t *seq_oid, uint8_t seq_len)
{
    uint8_t *x509_cert_end = x509_cert_ptr + x509_cert_size - seq_len;

    /* searching for the following byte seq in the cert */
    /* seq_id(0x30) seq_len(< 0x80) 0x06 0x09 0x2B...   */
    while (x509_cert_ptr < x509_cert_end)
    {
        if ((*x509_cert_ptr == seq_oid[0]) &&
            (*(x509_cert_ptr + 2) == seq_oid[2]) &&
            (*(x509_cert_ptr - 2) == 0x30))
        {
            if ((bcmp((const void *)x509_cert_ptr, (const void *)seq_oid, seq_len)) == 0)
            {
                /* return start boot_seq */
                return (x509_cert_ptr - 2);
            }
        }
        x509_cert_ptr++;
    }

    return NULL;
}

static uint32_t UFP_isSysfwEnc(uint8_t *x509_cert_ptr)
{
    static uint32_t retVal = UFP_SYSFW_NOT_PROCESSED;
    uint8_t *encSeqPtr = NULL;
    uint32_t outer_cert_len = 0;
    uint8_t *innerCertStart = NULL;
    uint32_t inner_cert_len = 0;
    /* oid encoding of encryption seq extension for RBL - 1.3.6.1.4.1.294.1.4 */
    uint8_t enc_seq_oid[] = {0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x26, 0x01, 0x04};

    if (x509_cert_ptr)
    {
        uint32_t UFP_GetCertLen(uint8_t *x509_cert_ptr);

        outer_cert_len = UFP_GetCertLen(x509_cert_ptr);
        innerCertStart = x509_cert_ptr + outer_cert_len;
        inner_cert_len = UFP_GetCertLen(innerCertStart);
    }

    if (inner_cert_len)
    {
        uint8_t *UFP_findSeq(uint8_t *x509_cert_ptr, uint32_t x509_cert_size, uint8_t *seq_oid, uint8_t seq_len);

        encSeqPtr = UFP_findSeq(innerCertStart, inner_cert_len, enc_seq_oid, sizeof(enc_seq_oid));
        if (encSeqPtr)
        {
            /* SYSFW is double signed and encrypted */
            retVal = UFP_SYSFW_ENCRYPTED;
        }
    }
    else if ((x509_cert_ptr) && (retVal == UFP_SYSFW_NOT_PROCESSED))
    {
        /* SYSFW is single signed */
        retVal = UFP_SYSFW_CLEAR_TEXT;
    }

    return retVal;
}

/**
 * @brief Function to initialize the sciclient module
 *
 * \param   sysfw    [IN]	System firmware pointer
 *
 * @return - int8_t
 *      0 	- sciclient module initialization successful
 *     -1 	- sciclient module initialization failed
 *
 */
int8_t UFP_sciclientInit(void *sysfw)
{
    int32_t status = CSL_EFAIL;

    Sciclient_ConfigPrms_t        config =
    {
        SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
    };
    /* SYSFW board configurations */
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;
    Sciclient_BoardCfgPrms_t ufsBoardCfgPrms;
    Sciclient_BoardCfgPrms_t ufsBoardCfgPmPrms;
    Sciclient_BoardCfgPrms_t ufsBoardCfgRmPrms;
    Sciclient_BoardCfgPrms_t ufsBoardCfgSecPrms;

    UFP_isSysfwEnc((uint8_t *) sysfw);

    status = Sciclient_loadFirmware((const uint32_t *)sysfw);
    if (status != CSL_PASS)
    {
        return (-1);
    }

    Sciclient_configPrmsInit(&config);
    config.opModeFlag               =   SCICLIENT_SERVICE_OPERATION_MODE_POLLED;
    config.pBoardCfgPrms            =   NULL;
    config.isSecureMode             =   0; /* default board cfg is for non-secure mode */
    config.c66xRatRegion            =   0;
    config.skipLocalBoardCfgProcess =   1;

    status = Sciclient_init(&config);
    if (status != CSL_PASS)
    {
        return (-1);
    }

    status = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);
    if (status != CSL_PASS)
    {
        return (-1);
    }

    ufsBoardCfgPrms.boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLow;
    ufsBoardCfgPrms.boardConfigHigh = 0;
    ufsBoardCfgPrms.boardConfigSize = boardCfgInfo.boardCfgLowSize;
    ufsBoardCfgPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfg(&ufsBoardCfgPrms);
    if (status != CSL_PASS)
    {
        return (-1);
    }

    /** On J784S4 HS firewall is not opened for BOLT_PSC device by TIFS. As a workaround
     *  Uniflash needs to open this firewall before doing PM Init. This condition should be
     *  removed after having a fix in TIFS
     */
#if !(defined(SOC_J784S4) && defined(BUILD_HS))
    ufsBoardCfgPmPrms.boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLowPm;
    ufsBoardCfgPmPrms.boardConfigHigh = 0;
    ufsBoardCfgPmPrms.boardConfigSize = boardCfgInfo.boardCfgLowPmSize;
    ufsBoardCfgPmPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgPm(&ufsBoardCfgPmPrms);
    if (status != CSL_PASS)
    {
        return (-1);
    }
#endif

    ufsBoardCfgSecPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLowSec;
    ufsBoardCfgSecPrms.boardConfigHigh = 0;
    ufsBoardCfgSecPrms.boardConfigSize = boardCfgInfo.boardCfgLowSecSize;
    ufsBoardCfgSecPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgSec(&ufsBoardCfgSecPrms);
    if (status != CSL_PASS)
    {
        return (-1);
    }

    /* As a workaround do PM Init for J784S4 HS after opening up BOLT_PSC firewall */
#if defined(SOC_J784S4) && defined(BUILD_HS)
    ufsBoardCfgPmPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLowPm;
    ufsBoardCfgPmPrms.boardConfigHigh = 0;
    ufsBoardCfgPmPrms.boardConfigSize = boardCfgInfo.boardCfgLowPmSize;
    ufsBoardCfgPmPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgPm(&ufsBoardCfgPmPrms);
    if (status != CSL_PASS)
    {
        return (-1);
    }
#endif

    ufsBoardCfgRmPrms.boardConfigLow  = (uint32_t)boardCfgInfo.boardCfgLowRm;
    ufsBoardCfgRmPrms.boardConfigHigh = 0;
    ufsBoardCfgRmPrms.boardConfigSize = boardCfgInfo.boardCfgLowRmSize;
    ufsBoardCfgRmPrms.devGrp = DEVGRP_ALL;
    status = Sciclient_boardCfgRm(&ufsBoardCfgRmPrms);
    if (status != CSL_PASS)
    {
        return (-1);
    }

#if defined(SOC_AM64X)
    Sciclient_pmSetModuleState(TISCI_DEV_RTI8,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_RTI9,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_RTI10,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_RTI11,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_MCU_RTI0,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
#else
    /* RTI seems to be turned on by ROM. Turning it off so that Power domain can transition */
    Sciclient_pmSetModuleState(TISCI_DEV_MCU_RTI0,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
    Sciclient_pmSetModuleState(TISCI_DEV_MCU_RTI1,
                               TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                               TISCI_MSG_FLAG_AOP,
                               SCICLIENT_SERVICE_WAIT_FOREVER);
#endif

    return 0;
}

/**
 * @brief	This function initializes settings based on soc.
 *
 * \param   cfg		[IN]	Board initializing value
 *
 * @return	int8_t
 *      0 	- Init completed successfully
 *     -1 	- Error occurred
 *
 */
int8_t UFP_socInit(Board_initCfg *cfg)
{
    Board_initCfg boardCfg;
    UART_HwAttrs uart_cfg;
    int32_t noBootCfg;

#ifdef UFP_DISABLE_JTAG_LOAD
    noBootCfg = 0;
#else
    noBootCfg = UFP_isNoBootEnabled();
#endif

    if(noBootCfg == 1)
    {
        boardCfg = (BOARD_INIT_PINMUX_CONFIG |
                    BOARD_INIT_PLL |
                    BOARD_INIT_MODULE_CLOCK);

        /* Board library init shall be called only in JTAG mode as 
         * system firmware should be loaded before calling board init.
         * In case of UART load, system configurations done by RBL
         * shall be used till the system firmware is loaded and board init
         * is called for default intializations.
         */
        if (Board_init(boardCfg))
        {
            return -1;
        }
    }

    if(noBootCfg == 1)
    {
        /* No need of changing the UART input clock when
           PLL init is done by board
         */
        UFP_uartConfig(UFP_BAUDRATE_115200);
    }
    else
    {
        UFP_initUARTPwrClk();
        UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        /* Use UART fclk freq setup by ROM */
        uart_cfg.frequency = UFP_ROM_UART_MODULE_INPUT_CLK;
        /* Disable the UART interrupt */
        uart_cfg.enableInterrupt = UFALSE;
        UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
        UFP_uartConfig(UFP_BAUDRATE_115200);
        ufpTcmEn();
        utilsCopyVecs2ATcm();
    }

    return 0;
}

/**
 * @brief Function to load the system firmware
 *
 * \param   sysFW    [IN]	System firmware pointer
 *
 * @return - int8_t
 *      0 	- Firmware load successful
 *     -1 	- Firmware load failed
 *
 */
int8_t UFP_loadSysFW(void *sysFW)
{
    Board_initCfg boardCfg;
    UART_HwAttrs uart_cfg;

    UFP_sciclientInit(sysFW);

    /* Board Library Init. */
    boardCfg = (BOARD_INIT_PINMUX_CONFIG |
                BOARD_INIT_PLL |
                BOARD_INIT_MODULE_CLOCK);
    if (Board_init(boardCfg))
    {
        return -1;
    }

    /* Reconfigure the UART */
    UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
    /* Use UART fclk freq setup by SYSFW */
    uart_cfg.frequency = UFP_SYSFW_UART_MODULE_INPUT_CLK;
    /* Disable the UART interrupt */
    uart_cfg.enableInterrupt = UFALSE;
    UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);

    UFP_uartClose();
    UFP_uartConfig(UFP_BAUDRATE_115200);

    return 0;
}
