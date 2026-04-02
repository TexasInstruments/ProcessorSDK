/**
 *  \file    sbl_uart.c
 *
 *  \brief   This file contains functions for UART read/write operations for SBL
 *
 */

/*
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
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
#include <stdint.h>
#include "string.h"

/* SBL Header files. */
#include "sbl_soc.h"
#include "sbl_soc_cfg.h"
#include "sbl_rprc_parse.h"

#include "sbl_err_trap.h"

#if (defined(SOC_TPR12) || defined(SOC_AWR294X))
#include "sbl_rcm.h"
#else /* TPR12 does not have SCI CLIENT, SYSFW, UDMA modules */
#include "sbl_sci_client.h"
#include <ti/drv/udma/udma.h>
#include <ti/drv/sciclient/sciclient.h>
#endif

/* TI-RTOS Header files */
#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/board/board_cfg.h>
#include "sbl_uart.h"

/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/
#define XMODEM_TEMP_BUFF_SIZE    ((SBL_SCRATCH_MEM_SIZE) / 2)
#define SBL_XMODEM_BUFF_ADDR     ((SBL_SCRATCH_MEM_START) + (XMODEM_TEMP_BUFF_SIZE))


static int32_t SBL_UART_ReadXmodemBuffer(void *dstAddr,
                             void *srcOffsetAddr,
                             uint32_t length)
{
    int32_t ret = 0;
    uint32_t *xModemPktBuffIndx = (uint32_t *)srcOffsetAddr;
    void *srcAddr = (void *)(SBL_XMODEM_BUFF_ADDR + *xModemPktBuffIndx);

    SBL_log(SBL_LOG_MAX, "\nRead: dstAddr=0x%x, srcOffsetAddr=0x%x, srcAddr=0x%x, length=%d\n", dstAddr, srcOffsetAddr, srcAddr, length);

    memcpy(dstAddr, srcAddr, length);
    *xModemPktBuffIndx += length;

    return ret;
}

static void SBL_UART_seek(void *srcAddr, uint32_t location)
{
    uint32_t *xModemPktBuffIndx = (uint32_t *)srcAddr;

    SBL_log(SBL_LOG_MAX, "\nSeek: srcAddr=0x%x, location=%d\n", srcAddr, location);
    *xModemPktBuffIndx = location;
}

#if (!defined(SOC_TPR12)  && !defined(SOC_AWR294X))/* SysFw not present for TPR12 */
int32_t SBL_ReadSysfwImage(void **pBuffer, uint32_t num_bytes)
{

    SBL_uartInit(SBL_ROM_UART_MODULE_INPUT_CLK);
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    SBL_log(SBL_LOG_MIN, "Waiting for tifs.bin ...\n");
#else
    SBL_log(SBL_LOG_MIN, "Waiting for sysfw.bin ...\n");
#endif

    SBL_uartXmodemRead((uint8_t *)(*pBuffer), SBL_SYSFW_MAX_SIZE);

    return CSL_PASS;
}
#endif

int32_t SBL_uartCopyHsmImage(uint8_t* pHsm, uint32_t numBytes)
{
    int32_t retVal = 0;
    SBL_log(SBL_LOG_MIN, "Waiting for hsm.bin ...\n");

    retVal = SBL_uartXmodemRead(pHsm, numBytes);
    /* SBL_uartXmodemRead returns 0 incase of failure and returns size in case of success */
    if(retVal > 0)
    {
        return CSL_PASS;
    }
    else
    {
        return CSL_EFAIL;
    }
}

int32_t SBL_uartInit(uint32_t inClkFreqHz)
{
    /* Init UART for xmodem boot */
    UART_HwAttrs uart_cfg;

    UART_socGetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);
#if (defined(SOC_TPR12) || defined (SOC_AWR294X))
    uart_cfg.clockFrequency = inClkFreqHz;
    uart_cfg.dmaMode = 0;
#else
    uart_cfg.frequency = inClkFreqHz;
#endif
    /* Disable the UART interrupt */
    uart_cfg.enableInterrupt = UFALSE;
    UART_socSetInitCfg(BOARD_UART_INSTANCE, &uart_cfg);

#if(defined(SOC_TPR12) || defined (SOC_AWR294X))
    {
        UART_Params uartPrms;

        UART_Params_init(&uartPrms);
        uartPrms.readDataMode = UART_DATA_BINARY;
        UART_stdioInit2(BOARD_UART_INSTANCE, &uartPrms);
    }
#else
    UART_stdioInit(BOARD_UART_INSTANCE);
#endif

    return 0;
}

int32_t SBL_uartClose(void)
{

    UART_stdioDeInit();

    return 0;
}

int32_t SBL_UARTBootImage(sblEntryPoint_t *pEntry)
{
    int32_t retVal = 0;
    uint32_t imgOffset = 0;

    /* Re-initialize the uart to a different freq */
    /* depending on whether SysFwConfigPm has run */
    /* or not                                     */
#if (defined(SOC_TPR12) || defined(SOC_AWR294X))
    {
        uint32_t uartFreq;
        Rcm_Return rcmRetVal;

        rcmRetVal = SBL_RcmGetPeripheralFreq(Rcm_PeripheralId_MSS_SCIA, &uartFreq);
        DebugP_assert(rcmRetVal == Rcm_Return_SUCCESS);
        SBL_uartInit(uartFreq);
    }
#else
 #if defined(DSBL_SKIP_BRD_CFG_PM) || defined(SBL_SKIP_SYSFW_INIT)
      SBL_uartInit(SBL_ROM_UART_MODULE_INPUT_CLK);
 #else
      SBL_uartInit(SBL_SYSFW_UART_MODULE_INPUT_CLK);
 #endif
#endif
    /* Initialize the function pointers to parse through the RPRC format. */
    fp_readData = &SBL_UART_ReadXmodemBuffer;
    fp_seek     = &SBL_UART_seek;

    SBL_log(SBL_LOG_MIN, "Waiting for multicore app ...\n");

    SBL_uartXmodemRead((uint8_t *)SBL_XMODEM_BUFF_ADDR, XMODEM_TEMP_BUFF_SIZE);

    retVal =  SBL_MulticoreImageParse((void *)&imgOffset, imgOffset, pEntry, SBL_BOOT_AFTER_COPY);

    return retVal;
}
