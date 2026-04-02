/**
 *  \file   ospi_flash_common.h
 *
 *  \brief  OSPI Flash common header
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


#ifndef OSPI_FLASH_COMMON_H
#define OSPI_FLASH_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ti/osal/osal.h>
#include <ti/csl/csl_ospi.h>
#include <ti/csl/soc.h>

#include "ospi_flash_soc.h"

#include <ti/drv/sciclient/sciclient.h>

/*!
 *  @brief  OSPI object based on OSPI_v0_Object in src/v0/OSPI_v0.h
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct
{
    uint8_t          *writeBufIdx;                  /* Internal inc. writeBuf index */
    uint32_t          writeCountIdx;                /* Internal dec. writeCounter */
    uint8_t          *readBufIdx;                   /* Internal inc. readBuf index */
    uint32_t          readCountIdx;                 /* Internal dec. readCounter */
    uint32_t          clk;                          /* OSPI Clock Frequency */
    SemaphoreP_Handle ospiFlashTransferDoneSem;    /* Semaphore to indicate transfer completion */
    HwiP_Handle       ospiFlashInterruptHandle;
} App_OspiObj;

/* OSPI Flash application function return status */
#define OSPI_FLASH_APP_STATUS_SUCCESS           ((int32_t)(0))
#define OSPI_FLASH_APP_STATUS_ERROR             (-((int32_t)1))

#define OSPI_MODULE_CLK_133M                    (133333333U)
#define OSPI_MODULE_CLK_166M                    (166666666U)

#define OSPI_FLASH_WRITE_TIMEOUT                (500000U)
#define OSPI_FLASH_CHECK_IDLE_DELAY             (10U)
#define OSPI_FLASH_CALIBRATE_DELAY              (20U)

#define OSPI_FLASH_DEV_DELAY_CSDA_2             (0x2U)                              /* Chip Select De-Assert device delay in # of ref_clk */
#define OSPI_FLASH_DEV_DELAY_CSDA_3             (0x3U)                              /* Chip Select De-Assert device delay in # of ref_clk */
#define OSPI_FLASH_DEV_DELAY_CSDA               (OSPI_FLASH_DEV_DELAY_CSDA_2)

#define OSPI_FLASH_RD_DATA_CAP_DELAY            (0U)                                /* Read data capture delay in # of ref_clk cycles */
#define OSPI_FLASH_CS_SOT_DELAY                 (10U)                               /* Chip Select Start Of Transfer delay */

#define OSPI_FLASH_SR_WIP                       (1U << 0U)              

#define OSPI_FLASH_XFER_LINES_OCTAL             (3U)

#define OSPI_FLASH_TUNING_DATA_OFFSET           (OSPI_FLASH_DEVICE_SIZE - OSPI_FLASH_DEVICE_BLOCK_SIZE)

/* SOC NOR Flash Device Id's */
#define OSPI_FLASH_NOR_DEVICE_ID_MT35XU512      (0x5B1AU)
#define OSPI_FLASH_NOR_DEVICE_ID_MT35XU256      (0x5B19U)
#define OSPI_FLASH_NOR_DEVICE_ID_S28HS512T      (0x5B1AU)

/* worst delay element periord in ps */
#define OSPI_FLASH_PHY_DLL_ELEM_DELAY_PERIOD    (80U) 

int32_t OspiFlash_ospiConfigClk(uint32_t clk);

int32_t OspiFlash_ospiOpen(App_OspiObj *ospiObj, 
                           uint32_t timeOut, 
                           uint32_t idleDelay, 
                           bool dacMode, 
                           bool intrPollMode);

int32_t OspiFlash_ospiEnableDDR(bool dacMode, 
                                bool intrPollMode);

int32_t OspiFlash_ospiSetOpcode(bool dacMode);
int32_t OspiFlash_ospiConfigPHY(uint32_t clk, 
                                bool intrPollMode);

int32_t OspiFlash_ospiEraseBlk(uint32_t blkNumOffset,
                               uint32_t testLen, 
                               bool intrPollMode);

void OspiFlash_ospiClose(App_OspiObj *ospiObj,
                         bool intrPollMode);

void OspiFlash_ospiXferIntrInit(bool intrPollMode);
int32_t OspiFlash_waitDeviceReady(uint32_t timeOut);
int32_t OspiFlash_ClkRateSet(uint32_t modId,
                             uint32_t clkId,
                             uint64_t clkRate);

int32_t OspiFlash_waitReady(uint32_t timeOut, 
                            bool intrPollMode);

int32_t OspiFlash_cmdWrite(const uint8_t  *cmdBuf,
                           uint32_t        cmdLen,
                           const uint8_t  *txBuf,
                           uint32_t        txLen,
                           bool            intrPollMode);

int32_t OspiFlash_xspiIndacWrite(App_OspiObj *ospiObj, 
                                 uint8_t *buf, 
                                 uint32_t numBytes, 
                                 bool intrPollMode,
                                 uint32_t offset);

#ifdef __cplusplus
}
#endif

#endif /* OSPI_FLASH_COMMON_H */
