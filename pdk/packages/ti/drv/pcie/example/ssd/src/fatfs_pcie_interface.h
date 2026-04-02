/**
 *  \file  fatfs_pcie_interface.h
 *
 *  \brief This file provides structure, api, variable and macro required to
 *         port pcie nvme to FATFS.
 *
 *  \copyright Copyright (C) 2013-2016 Texas Instruments Incorporated -
 *             http://www.ti.com/
 */

/*
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



/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */


/* ========================================================================== */
/*                            Global Variables Declarations                   */
/* ========================================================================== */


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   Read data from drive of a device.
 *
 * \param   pDev        FATFS device.
 * \param   pBuf        Buffer to read data.
 * \param   sector      Start sector.
 * \param   cnt         Number of sectors to be read.
 *
 * \retval  NVME_OK         Sector read successfully.
 * \retval  NVME_READ_ERR   Read Error.
 */
int32_t FATFS_PCIE_Read(void* fatfsdev, uint8_t *pBuf, uint32_t sector, uint32_t cnt);

/**
 * \brief   Write data to drive of a device.
 *
 * \param   pDev        FATFS device.
 * \param   pBuf        Buffer to write data.
 * \param   sector      Start sector.
 * \param   cnt         Number of sectors to be written.
 *
 * \retval  NVME_OK          Data written to sector successfully.
 * \retval  NVME_WRITE_ERR   Write Error.
 */
int32_t FATFS_PCIE_Write(void* fatfsdev, uint8_t *pBuf, uint32_t sector, uint32_t cnt);




int32_t FATFS_PCIE_Disk_Close(void* handle);

int32_t FATFS_PCIE_Disk_Open(uint32_t index, void* params, void** handle);

int32_t FATFS_PCIE_Disk_Initialize(void);

