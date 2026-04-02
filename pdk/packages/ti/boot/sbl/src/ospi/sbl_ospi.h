/**
 *  \file    sbl_ospi.h
 *
 *  \brief   This file contains functions prototypes for OSPI Boot functionality
 *           of SBL.
 *
 */

/*
 * Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef SBL_OSPI_H
#define SBL_OSPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/board/board.h>
#include "sbl_log.h"
#include "sbl_typecast.h"
#include "sbl_profile.h"

/* Macro representing the offset where the App Image has to be written/Read from
   the OSPI Flash.
*/
#define SBL_OSPI_OFFSET_SI              (0x100000U)
#define SBL_OSPI_OFFSET_SYSFW           (0x80000U)
#define SBL_OSPI_OFFSET_HSM             (0xC0000U)
#define SBL_HSM_HEADER                  (0x30)
/*
 *  \brief    SBL_OSPIBootImage function initializes the OSPI driver and copies
 *            the application image from the OSPI device to the DDR memory and
 *            gives control to the processor core.
 *
 *  \param    pointer to the structure holding the entry pointers for different
 *            cores.
 *
 *  \return   error status.If error has occured it returns a non zero value.
 *            If no error has occured then return status will be zero.
 *
 */
int32_t SBL_OSPIBootImage(sblEntryPoint_t *pEntry);

/**
 * @brief - SBL_ospiInit() - function to do initialize QSPI
 *
 *
 * @param
 *     handle = pointer to return QSPI handle
 *     isNandBootEnabled = flag to check if Nand Boot is enabled
 *
 * @return - int32t
 *      0 = Init completed successfully
 *     -1 = Error occurred
 *
 *
 */
int32_t SBL_ospiInit(void *handle, bool isNandBootEnabled);

/**
 * @brief - SBL_ospiFlashRead() - function to do flash QSPI
 *
 * @param
 *     handle = pointer to QSPI handle
 *     dst = byte pointer to destination
 *     length = size of source to copy
 *     offset = QSPI offset to flash into
 *
 * @return - int32t
 *      0 = Init completed successfully
 *     <0 = Negative value indicate error occurred
 *
 */
int32_t SBL_ospiFlashRead(const void *handle, uint8_t *dst, uint32_t length,
    uint32_t offset);

/**
 *
 * @brief - SBL_ospiClose() - function to do close QSPI handle
 *
 *
 * @param
 *
 *     handle = pointer to QSPI handle
 *
 *
 * @return - int32t
 *      0 = Init completed successfully
 *
 *     -1 = Error occurred
 *
 *
 */
int32_t SBL_ospiClose(const void *handle);

/* OSPI Flash Read Sector API */
int32_t SBL_OSPI_ReadSectors(void *dstAddr,
                             void *srcOffsetAddr,
                             uint32_t length);

/* Sets the src address to the given offset address */
void SBL_OSPI_seek(void *srcAddr, uint32_t location);

/* Proxy function for apps to be able to use same OSPI_init func, but called externally */
void SBL_SPI_init();

/* Update global variable isXIPEnable to true if BUILD_XIP is defined and update the OSPI frequency */
void SBL_enableXIPMode(uint32_t freq);

/* Update global variable gIsNandBootEnable to true if OSPI_NAND_BOOT is defined */
void SBL_enableNandBoot();

/**
 * @brief                 This function copies HSM binary to the specified OCMC address from the OSPI address 
 *
 * @param dstAddr         OCMC address where HSM binary is copied
 * @param srcOffsetAddr   OSPI offset address where hsm.bin is flashed
 * @param size            Max size of hsm.bin 
 *
 * @return -              CSL_PASS if copy successful else CSL_EFAIL or returns SBL_HSM_IMG_NOT_FOUND if file not found
 */
int32_t SBL_ospiCopyHsmImage(uint8_t** dstAddr, uint32_t srcOffsetAddr, uint32_t size);

#endif

#ifdef __cplusplus
}
#endif
