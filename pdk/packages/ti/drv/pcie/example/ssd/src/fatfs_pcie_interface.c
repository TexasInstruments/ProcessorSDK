
/*-----------------------------------------------------------------------*/
/* PCIE - FATFS interface                                                 */
/*-----------------------------------------------------------------------*/

/**
 *  Copyright (c) Texas Instruments Incorporated 2015-2016
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


#include <ti/drv/pcie/example/ssd/src/nvme.h>
#include <ti/drv/pcie/example/ssd/src/fatfs_pcie_interface.h>

/*-----------------------------------------------------------------------*/
/* This function reads sector(s) from the disk drive                     */
/*-----------------------------------------------------------------------*/

int32_t FATFS_PCIE_Read (
    void* drv,
    uint8_t* buff,          /* Pointer to the data buffer to store read data */
    uint32_t sector,        /* Starting sector */
    uint32_t count)         /* Sector count (1..255) */
{
    NVME_ERR err;

    err = nvme_read(sector,buff,count);
    if(err != NVME_OK)
    {
        NVME_printf("Failed to read sector : 0x%x",sector);
        return -NVME_READ_ERR;
    }
    return NVME_OK;
}

/*-----------------------------------------------------------------------*/
/* DiskInitFxn                                                           */
/*-----------------------------------------------------------------------*/
int32_t
FATFS_PCIE_Disk_Initialize(void)
{
    return NVME_OK;
}



/*-----------------------------------------------------------------------*/
/* This function writes sector(s) to the disk drive                     */
/*-----------------------------------------------------------------------*/
int32_t FATFS_PCIE_Write (
    void* usbDrv,
    uint8_t* buff,       /* Pointer to the data to be written */
    uint32_t sector,           /* Start sector number (LBA) */
    uint32_t count)             /* Sector count (1..255) */
{
    NVME_ERR err;

    err = nvme_write(buff,sector, count);
    if(err != NVME_OK)
    {
        NVME_printf("Failed to write to sector : 0x%x",sector);
        return -NVME_WRITE_ERR;
    }
    return NVME_OK;
}


/*-----------------------------------------------------------------------*/
/* closeDrvFxn                                                           */
/*-----------------------------------------------------------------------*/
int32_t FATFS_PCIE_Disk_Close(void* handle)
{
    return NVME_OK;
}

/*-----------------------------------------------------------------------*/
/* openDrvFxn                                                            */
/*-----------------------------------------------------------------------*/
int32_t FATFS_PCIE_Disk_Open(uint32_t index, void* params, void** handle)
{
    return NVME_OK;
}
