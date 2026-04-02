/**
 *  \file    sbl_emmc.c
 *
 *  \brief   This file contains functions for eMMC File read operations for SBL
 *
 */

/*
 * Copyright (C) 2015 - 2022 Texas Instruments Incorporated - http://www.ti.com/
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

/* TI-RTOS Header files */
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/mmcsd/MMCSD.h>
#include <ti/drv/mmcsd/soc/MMCSD_soc.h>
#include <ti/drv/mmcsd/src/MMCSD_osal.h>

/* SBL Header files. */
#include "sbl_rprc_parse.h"
#include "sbl_emmc.h"
#include "sbl_log.h"

/* K3 Header files */
#ifdef BUILD_MCU
#include <sbl_sci_client.h>
#endif

/* Global variable to check whether BOOT_EMMC_BOOT0 is defined or not */
bool gIsEmmcBoot0Enable = BFALSE;
/* This variable is used to initialize cidPtr pointer to pointer variable.
   If initialized to NULL, it is showing SA issues which is dereferencing NULL Ptr */
uint32_t gCidAddr;

/* eMMC Sector size */
#define SECTORSIZE                      (512U) //0x200
/* System firmware offset in eMMC boot0 partition */
#define EMMC_BOOT0_SYSFS_OFFSET         (0x80000) //0x400 sector
/* Application Image offset in eMMC boot0 partition */
#define EMMC_BOOT0_APP_OFFSET           (0x480000) //0x2400 sector (as u-boot.img needs to be at 0x1400)

/* Application Image Size */
#define APP_SIZE                        MAX_APP_SIZE_EMMC
extern volatile uint32_t * outData01;
/* Handle to operate eMMC read/write */
MMCSD_Handle gHandle=NULL;
/* SBL scratch memory defined at compile time */
static uint8_t *sbl_scratch_mem_for_appcopy = ((uint8_t *)(SBL_SCRATCH_MEM_APP_COPY_START));

/**
 * \brief    SBL_FileRead function reads N bytes from eMMC and
 *           advances the cursor.
 *
 * \param     buff - Pointer to data buffer
 * \param     fileptr - Read head pointer
 * \param     size - Number of bytes to read
 *
 * \return    Error code on file error
 */
int32_t SBL_FileRead(void  *buff,
                     void *fileptr,
                     uint32_t size);

/**
 *  \brief    SBL_FileSeek function to move the read head by n bytes
 *
 *  \param    srcAddr - Read head pointer
 *  \param    location - Move the read head pointer by n bytes
 *
 * \return  none
 */
void SBL_FileSeek(void *fileptr, uint32_t location);

/**
 * \brief    SBL_emmcRead function copies memory contents from DDR to the
 *           destination and advances the cursor.
 *
 * \param     buff - Pointer to data buffer
 * \param     fileptr - Read head pointer
 * \param     size - Number of bytes to read
 *
 * \return    Error code on file error
 */
int32_t SBL_emmcRead(void  *buff,
                     void *fileptr,
                     uint32_t size);

/**
 *  \brief    SBL_emmcSeek function to move the read head by the provided location
 *
 *  \param    srcAddr - Read head pointer
 *  \param    location - Move the read head pointer to the provided location
 *
 * \return  none
 */
void SBL_emmcSeek(void *fileptr, uint32_t location);

int32_t SBL_loadMMCSDBootFile(FIL * fp);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* FATFS function table for eMMC implementation */
FATFS_DrvFxnTable FATFS_drvFxnTable = {
    &MMCSD_close,
    &MMCSD_control,
    &MMCSD_init,
    &MMCSD_open,
    &MMCSD_write,
    &MMCSD_read
};

/* FATFS configuration structure */
FATFS_HwAttrs FATFS_initCfg[_VOLUMES] =
{
    {
#if defined(j721e_evm) || defined(j7200_evm) || defined(j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm)
        0U
#else
        1U
#endif
    },
    {
#if defined(j721e_evm) || defined(j7200_evm) || defined(j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm)
        0U
#else
        1U
#endif
    },
    {
#if defined(j721e_evm) || defined(j7200_evm) || defined(j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm)
        0U
#else
        1U
#endif
    },
    {
#if defined(j721e_evm) || defined(j7200_evm) || defined(j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm)
        0U
#else
        1U
#endif
    }
};

/* FATFS objects */
FATFS_Object FATFS_objects[_VOLUMES];

/* FATFS Handle */
FATFS_Handle sbl_fatfsHandle = NULL;

/* FATFS configuration structure */
const FATFS_Config FATFS_config[_VOLUMES + 1] = {
    {
        &FATFS_drvFxnTable,
        &FATFS_objects[0],
        &FATFS_initCfg[0]
    },

    {
         &FATFS_drvFxnTable,
         &FATFS_objects[1],
         &FATFS_initCfg[1]
    },

    {
         &FATFS_drvFxnTable,
         &FATFS_objects[2],
         &FATFS_initCfg[2]
    },

    {NULL, NULL, NULL},

    {NULL, NULL, NULL}
};

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#define SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MASK      (0x78U)
#define SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MMCSD     (0x1U)
#define SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK (0x20U)
#define SBL_MAIN_DEVSTAT_BACKUP_BOOT_MASK       (0xEU)
#define SBL_MAIN_DEVSTAT_BACKUP_BOOT_MMCSD      (0xAU)
#define SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK    (0x1U)
/*  eMMC Micron Flash ID  */
#define EMMC_MICRON_FLASH_MANUFACTURER_ID       (0X13U)


#endif

#ifdef BUILD_MCU
int32_t SBL_ReadSysfwImage(void **pBuffer, uint32_t num_bytes)
{
    int32_t retVal = CSL_PASS;
    /* Following variables are used for eMMC Boot0 Boot*/
    uint32_t partition = 1;
    /* Total number of blocks to be read*/
    uint32_t num_blocks_read  = (num_bytes/SECTORSIZE) + 1;
    /* Start block to be read */
    uint32_t mmcStartSector = (EMMC_BOOT0_SYSFS_OFFSET/SECTORSIZE);
    /* Following variables are used for eMMC UDA Boot*/
    FIL     fp;
    memset(&fp, 0, sizeof(fp));
    FRESULT  fresult;
    uint32_t bytes_read = 0;
    const TCHAR *fileName = "0:/tifs.bin";
    void *sysfw_ptr = *pBuffer;
    MMCSD_v2_HwAttrs hwAttrsConfig;
    uint32_t** cidPtr = (uint32_t**)&gCidAddr;
    uint8_t emmcManufacturerId = 0U;

     if(MMCSD_socGetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0) {
       UART_printf("\nUnable to get config.Exiting. TEST FAILED.\r\n");
       retVal = E_FAIL;
     }

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    uint32_t wkupCtrlDevstat = (*((volatile uint32_t *)(CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_WKUP_DEVSTAT)));
    uint32_t mainCtrlDevstat = (*((volatile uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_MAIN_DEVSTAT)));

    if(((wkupCtrlDevstat & SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MASK) == SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MMCSD) &&
       ((mainCtrlDevstat & SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK) == SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK))
    {
        /* eMMC as primary bootmode */
        /* Check MAIN CTRL MMR DEVSTAT register for Primary boot mode Bus Width */
        if((mainCtrlDevstat & SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK) == SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK)
        {
            hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_1BIT;
        }
        else
        {
            hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_8BIT;
        }
    }
    else if((mainCtrlDevstat & SBL_MAIN_DEVSTAT_BACKUP_BOOT_MASK) == SBL_MAIN_DEVSTAT_BACKUP_BOOT_MMCSD)
    {
        /* eMMC as backup bootmode only supports 1-bit bus width, as set by ROM code */
        hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_1BIT;
    }
    else
    {
        hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_8BIT;
    }
#endif

    hwAttrsConfig.enableInterrupt = ((uint32_t)(0U));
    hwAttrsConfig.configSocIntrPath=NULL;

    if (gIsEmmcBoot0Enable == BTRUE)
    {
        hwAttrsConfig.enableDma = 1;
        hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_8BIT;
        hwAttrsConfig.enableInterrupt = ((uint32_t)(0U));
        hwAttrsConfig.supportedBusVoltages = MMCSD_BUS_VOLTAGE_1_8V;
        hwAttrsConfig.supportedModes = MMCSD_SUPPORT_MMC_DS;
    }

    if(MMCSD_socSetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0) {
       UART_printf("\nUnable to set config.Exiting. TEST FAILED.\r\n");
       retVal = E_FAIL;
     }

    if (gIsEmmcBoot0Enable == BTRUE)
    {
        MMCSD_init();
        retVal = MMCSD_readcid(FATFS_initCfg[0].drvInst,cidPtr);
        if(retVal != CSL_PASS)
        {
            UART_printf("Failed to get CID Register\n");
        }

        if (retVal == CSL_PASS)
        {
            if (cidPtr != NULL)
            {
                emmcManufacturerId = *(*cidPtr+3) >> 24;
            }
          if (EMMC_MICRON_FLASH_MANUFACTURER_ID == emmcManufacturerId)
          {
            if(MMCSD_socGetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0)
            {
                UART_printf("Unable to get MMC init config, MMCSD_socGetInitCfg failed.\r\n");
                retVal = E_FAIL;
            }
            else
            {
                hwAttrsConfig.drvStrength=0;
                hwAttrsConfig.phydrvStrength=0;
            }
            if(MMCSD_socSetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0) 
            {
                UART_printf("Unable to set MMC init config, MMCSD_socSetInitCfg failed.\r\n");
                retVal = E_FAIL;
            }
            else
            {
                /* Do Nothing */
            }
          }
          else
          {
            /* User Can add different Drive strength based on flash manufacturer IDs */
          }
        }

	retVal = MMCSD_open(FATFS_initCfg[0].drvInst, NULL, &gHandle);

    if (retVal != CSL_PASS)
        {
            UART_printf("\n MMCSD open fails for eMMC Boot \n");
        }
        /* Enable boot partition configurations */
        if (retVal == CSL_PASS)
        {
            retVal = MMCSD_control(gHandle, MMCSD_CMD_ENABLEBOOTPARTITION, (void *)&partition);
            if (retVal != CSL_PASS)
            {
                UART_printf("\n MMCSD control fails for eMMC Boot \n");
            }
        }
        if(retVal == CSL_PASS)
        {
            retVal = MMCSD_read(gHandle, (uint8_t *) sysfw_ptr, mmcStartSector, num_blocks_read);
            if (retVal != CSL_PASS)
            {
                UART_printf("\n eMMC Boot from BOOT0 partition - TIFS File Read Failure \n");
            }
        }
    }
    else
    {
        /* Initialization of the driver. */
        FATFS_init();

        /* eMMC FATFS initialization */
        FATFS_open(0U, NULL, &sbl_fatfsHandle);

        fresult = f_open(&fp, fileName, ((BYTE)FA_READ));
        if (fresult != FR_OK)
        {
            UART_printf("\n eMMC Boot from UDA partition - TIFS File Open Failure \n");
            retVal = E_FAIL;
        }
        else
        {
            fresult  = f_read(&fp, sysfw_ptr, num_bytes, &bytes_read);
            if (fresult != FR_OK)
            {
                UART_printf("\n eMMC Boot from UDA partition - TIFS Read Failure \n");
                retVal = E_FAIL;
            }

            f_close(&fp);
        }

        FATFS_close(sbl_fatfsHandle);
        sbl_fatfsHandle = NULL;
    #endif
    }
    return retVal;
}

int32_t SBL_eMMCBootImage(sblEntryPoint_t *pEntry)
{
    SBL_log(SBL_LOG_MAX, "\n If you don't get the logs of the application while booting \
    from boot0 partition then you might need to pass the MAX_APP_SIZE_EMMC (=<size of you application>) \
    while building your appimage \n");

    int32_t retVal = E_PASS;
    /* Total number of blocks to be read*/
    uint32_t num_blocks_read  = (APP_SIZE/SECTORSIZE) + 1;
    /* Start block to be read */
    uint32_t mmcStartSector = (EMMC_BOOT0_APP_OFFSET/SECTORSIZE);
    const TCHAR *fileName = "0:/app";
    FIL     fp;
    memset(&fp, 0, sizeof(fp));
    FRESULT  fresult;

    /* Profile point after EEPROM copying and before eMMC init */
    SBL_ADD_PROFILE_POINT;

    if (gIsEmmcBoot0Enable == BTRUE)
    {
        /* Read the application image into DDR */
        retVal = MMCSD_read(gHandle, sbl_scratch_mem_for_appcopy, mmcStartSector, num_blocks_read);
        if(retVal != FR_OK)
        {
            UART_printf("MMCSD_read Failed to read contents into DDR\n");
        }

        if(gHandle)
        {
            MMCSD_close(gHandle);
        }
        fp_readData = &SBL_emmcRead;
        fp_seek     = &SBL_emmcSeek;

        /* Profile point after eMMC init and before phy tuning */
        SBL_ADD_PROFILE_POINT;

#if defined(SBL_ENABLE_HLOS_BOOT) && (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2))
        retVal = SBL_MulticoreImageParse((void *) &sbl_scratch_mem_for_appcopy, (SBL_SCRATCH_MEM_APP_COPY_START), pEntry, SBL_SKIP_BOOT_AFTER_COPY);
#else
        retVal = SBL_MulticoreImageParse((void *) &sbl_scratch_mem_for_appcopy, (SBL_SCRATCH_MEM_APP_COPY_START), pEntry, SBL_BOOT_AFTER_COPY);
#endif
    }
    else
    {
        /* Initialization of the driver. */
        FATFS_init();

        /* eMMC FATFS initialization */
        FATFS_open(0U, NULL, &sbl_fatfsHandle);

        fresult = f_open(&fp, fileName, ((BYTE)FA_READ));
        if (fresult != FR_OK)
        {
            UART_printf("\n SD Boot - File open fails \n");
            retVal = E_FAIL;
        }
        else
        {
            fp_readData = &SBL_FileRead;
            fp_seek     = &SBL_FileSeek;
        
        /* Profile point after eMMC init and before phy tuning */
        SBL_ADD_PROFILE_POINT;
#if defined(SBL_ENABLE_HLOS_BOOT) && (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2))
            retVal = SBL_MulticoreImageParse((void *) &fp, 0, pEntry, SBL_SKIP_BOOT_AFTER_COPY);
#else
            retVal = SBL_MulticoreImageParse((void *) &fp, 0, pEntry, SBL_BOOT_AFTER_COPY);
#endif

            f_close(&fp);
        }

        FATFS_close(sbl_fatfsHandle);
        sbl_fatfsHandle = NULL;
    }

    return retVal;
}

int32_t SBL_FileRead(void       *buff,
                      void *fileptr,
                      uint32_t    size)
{
    FIL     *fp         = (FIL *) (fileptr);
    uint8_t *tmp_buff_ptr = (uint8_t *)buff;
    uint32_t i          = 0;
    uint32_t bytes_read = 0;
    uint32_t Max_read   = 0x400U; /*setting a fatfs read size of 1k */
    FRESULT  fresult    = FR_OK;
    int32_t retVal = E_FAIL;

    for (i = ((uint32_t)0U); i < (size / Max_read); ++i)
    {
        fresult  = f_read(fp, (void *)tmp_buff_ptr, Max_read, &bytes_read);
        tmp_buff_ptr = tmp_buff_ptr + bytes_read;
        if (fresult != FR_OK)
        {
            break;
        }
    }
    if (fresult == FR_OK)
    {
        fresult = f_read(fp, (void *)tmp_buff_ptr, (UINT) (size % Max_read), &bytes_read);
    }

    if (fresult == FR_OK)
    {
        retVal = E_PASS;
    }

    return retVal;
}

void SBL_FileSeek(void *fileptr, uint32_t location)
{
    FIL *fp = (FIL *) (fileptr);
    f_lseek(fp, location);
}

int32_t SBL_emmcRead(void     *buff,
                    void     *fileptr,
                    uint32_t size)
{
    int32_t retVal = E_PASS;
    uint8_t *tmp_buff_ptr = (uint8_t *)buff;
    memcpy((void *)tmp_buff_ptr, (void *)(*((uint32_t *)fileptr)), size);
    *((uint32_t *)fileptr) += size;
    return retVal;
}

void SBL_emmcSeek(void *fileptr, uint32_t location)
{
    *((uint32_t *) fileptr) = location;
}

void SBL_enableEmmcBoot0()
{
    gIsEmmcBoot0Enable = BTRUE;
}

