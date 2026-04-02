/**
 *  \file    sbl_mmcsd.c
 *
 *  \brief   This file contains functions for MMCSD File read operations for SBL
 *
 */

/*
 * Copyright (C) 2015 - 2018 Texas Instruments Incorporated - http://www.ti.com/
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
#include <ti/boot/sbl/soc/k3/sbl_soc_cfg.h>
#include <ti/osal/CacheP.h>

/* SBL Header files. */
#include "sbl_rprc_parse.h"
#include "sbl_mmcsd.h"

/* K3 Header files */
#ifdef BUILD_MCU
#include <sbl_sci_client.h>
#endif

/**
 * \brief    SBL_FileRead function reads N bytes from SD card and
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
 * \brief    SBL_getWkupCtrlDevStat function to get the value of Wakeup
 *           Domain Device Status Register
 *
 * \return   Value of Wakeup Domain Device Status Register
 */
static uint32_t SBL_getWkupCtrlDevStat(void);

/**
 * \brief    SBL_getMainCtrlDevStat function to get the value of Main
 *           Domain Device Status Register
 *
 * \return   Value of Main Domain Device Status Register
 */
static uint32_t SBL_getMainCtrlDevStat(void);

int32_t SBL_loadMMCSDBootFile(FIL * fp);

/**
 * \brief    SBL_LazyBootGetAppname function to get the name of 'app'
 *           to be booted. In lazy boot, SBL may not boot 'app' and
 *           boot 'appX' instead.
 *
 * \return   none
 */
static void SBL_LazyBootGetAppname(TCHAR fileName[16]);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* MMCSD function table for MMCSD implementation */
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
        1U
#else
        0U
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
#define SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MMCSD     (0x0U)
#define SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK (0x20U)
#define SBL_MAIN_DEVSTAT_BACKUP_BOOT_MASK       (0xEU)
#define SBL_MAIN_DEVSTAT_BACKUP_BOOT_MMCSD      (0xAU)
#define SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK    (0x1U)
#endif
/* Below macros are used by SBL Lazy Boot */
#define SBL_MMCSD_BLOCK_SIZE       (512U)
#define SBL_META_BUFF_LEN          (SBL_MMCSD_BLOCK_SIZE)
#define SBL_META_BUFF_ALIGN        (256U)
#define ASCII_FOR_ZERO             (48U)
#define SBL_NUM_DIGITS_IN_METAFILE (4U)

__attribute((aligned(SBL_META_BUFF_ALIGN))) uint8_t metabuf[SBL_META_BUFF_LEN];

#ifdef BUILD_MCU
int32_t SBL_ReadSysfwImage(void **pBuffer, uint32_t num_bytes)
{
    int32_t retVal = CSL_PASS;
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    const TCHAR *fileName = "0:/tifs.bin";
#else
    const TCHAR *fileName = "0:/sysfw.bin";
#endif
    FIL     fp;
    memset(&fp, 0, sizeof(fp));
    FRESULT  fresult;
    uint32_t bytes_read = 0;
    void *sysfw_ptr = *pBuffer;
    MMCSD_v2_HwAttrs hwAttrsConfig;

     if(MMCSD_socGetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0) {
       UART_printf("\nUnable to get config.Exiting. TEST FAILED.\r\n");
       retVal = E_FAIL;
     }

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    uint32_t wkupCtrlDevstat = SBL_getWkupCtrlDevStat();
    uint32_t mainCtrlDevstat = SBL_getMainCtrlDevStat();

    if(((wkupCtrlDevstat & SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MASK) == SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MMCSD) &&
       ((mainCtrlDevstat & SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK) == SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK))
    {
        /* MMCSD as primary bootmode */
        /* Check MAIN CTRL MMR DEVSTAT register for Primary boot mode Bus Width */
        if((mainCtrlDevstat & SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK) == SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK)
        {
            hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_1BIT;
        }
        else
        {
            hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_4BIT;
        }
    }
    else if((mainCtrlDevstat & SBL_MAIN_DEVSTAT_BACKUP_BOOT_MASK) == SBL_MAIN_DEVSTAT_BACKUP_BOOT_MMCSD)
    {
        /* MMCSD as backup bootmode only supports 1-bit bus width, as set by ROM code */
        hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_1BIT;
    }
    else
    {
        hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_4BIT;
    }
#endif

    hwAttrsConfig.enableInterrupt = ((uint32_t)(0U));
    hwAttrsConfig.configSocIntrPath=NULL;

    /* Disabling DMA if enabled already while reading SYSFW since SBL loaded memory 
       is firewalled for DMA access. Enable the DMA after reading SYSFW */
    #if defined(BUILD_HS)
        hwAttrsConfig.enableDma = 0;
    #endif

    if(MMCSD_socSetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0) {
       UART_printf("\nUnable to set config.Exiting. TEST FAILED.\r\n");
       retVal = E_FAIL;
     }

    /* Initialization of the driver. */
    FATFS_init();

    /* MMCSD FATFS initialization */
    FATFS_open(0U, NULL, &sbl_fatfsHandle);

    fresult = f_open(&fp, fileName, ((BYTE)FA_READ));
    if (fresult != FR_OK)
    {
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
        UART_printf("\n SD Boot - tifs File open fails \n");
#else
        UART_printf("\n SD Boot - sysfw File open fails \n");
#endif
        retVal = E_FAIL;
    }
    else
    {
        fresult  = f_read(&fp, sysfw_ptr, num_bytes, &bytes_read);
        if (fresult != FR_OK)
        {
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
            UART_printf("\n SD Boot - tifs read fails \n");
#else
            UART_printf("\n SD Boot - sysfw read fails \n");
#endif
            retVal = E_FAIL;
        }

        f_close(&fp);
    }

    /* Re-Enabling DMA since TIFS is already copied to OCMC by this time */
    #if defined(BUILD_HS)
        hwAttrsConfig.enableDma = 1;
        if(MMCSD_socSetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0) 
        {
            UART_printf("\nUnable to set MMCSD config, exiting. Test Failed.\r\n");
            retVal = E_FAIL;
        }
    #endif

    FATFS_close(sbl_fatfsHandle);
    sbl_fatfsHandle = NULL;

    return retVal;
}
#endif

static void SBL_LazyBootGetAppname(TCHAR fileName[16])
{
    const TCHAR *metafilename = "0:/metafile";
    uint32_t appId = 0, nextAppId=1, maxAppId=0, appInd=0, numStarted=0, metaBuffInd;
    FIL     metaFilefp;
    FRESULT  metaFresult;
    uint32_t writtenSize=0, readBytes=0;

    /* Try to open the metafile. 
     * 'metafile' is used by MMCSD SBL to boot application in Lazy Boot fashion.
     * Please refer the SBL documentation to understand the contents of the 'metafile'.
     */
    metaFresult = f_open(&metaFilefp, metafilename, ((BYTE)FA_READ));
    /* If opening metafile fails, continue to normal boot. */
    if(FR_OK == metaFresult)
    {
        /* If metafile read fails, then continue to boot the normal 'app' */
        if (FR_OK != f_read(&metaFilefp, &metabuf[0], SBL_META_BUFF_LEN, &readBytes))
        {
            SBL_log(SBL_LOG_MAX, "\n Meta file read failed... Continuing the boot. \n");
        }
        else
        {
            /* Close the 'metafile' in the read mode., and open it in the
             * write mode to write the index of the next 'appN'.
             */
            f_close(&metaFilefp);
            metaFresult = f_open(&metaFilefp, metafilename, ((BYTE)FA_WRITE));

            CacheP_Inv(metabuf , 4U);
            /* Max supported appId is 9999.
             * 'metafile' is ASCII encoded, containing decimal digits. Number formed with the first 4 digits
             * denote the appId to boot and the number formed with the next 4 digits denote the maxAppId
             * present in the SD card.
             */
            appId = (metabuf[0] - ASCII_FOR_ZERO)*1000 + (metabuf[1] - ASCII_FOR_ZERO)*100 + \
                    (metabuf[2] - ASCII_FOR_ZERO)*10 + (metabuf[3] - ASCII_FOR_ZERO);
            maxAppId = (metabuf[4] - ASCII_FOR_ZERO)*1000 + (metabuf[5] - ASCII_FOR_ZERO)*100 + \
                    (metabuf[6] - ASCII_FOR_ZERO)*10 + (metabuf[7] - ASCII_FOR_ZERO);
            while(fileName[appInd] != '\0')
            {
                appInd++;
            }
            /* Do not prepend 0s to appname, if these are leading 0s. */
            for(metaBuffInd = 0; metaBuffInd < SBL_NUM_DIGITS_IN_METAFILE; metaBuffInd++)
            {
                if (numStarted || (metabuf[metaBuffInd] - ASCII_FOR_ZERO) != 0)
                {
                    fileName[appInd++] = metabuf[metaBuffInd];
                    numStarted = 1;
                }
            }
            fileName[appInd]   = '\0';

            nextAppId = appId+1;
            /* If the nextAppId overflows maxAppId, restore the nextAppId to 1. */
            if (nextAppId > maxAppId)
            {
                nextAppId = 1U;
            }
            /* Store the appId of the next application to boot back into the SD card. */
            metabuf[0] = (nextAppId/1000) + ASCII_FOR_ZERO;
            nextAppId %= 1000;
            metabuf[1] = (nextAppId/100) + ASCII_FOR_ZERO;
            nextAppId %= 100;
            metabuf[2] = (nextAppId/10) + ASCII_FOR_ZERO;
            nextAppId %= 10;
            metabuf[3] = (nextAppId) + ASCII_FOR_ZERO;

            CacheP_wb(metabuf, 4U);
            f_write(&metaFilefp, &metabuf[0], SBL_META_BUFF_LEN, &writtenSize);
        }
    }

}

int32_t SBL_MMCBootImage(sblEntryPoint_t *pEntry)
{
    int32_t retVal = E_PASS;
    TCHAR fileName[16] = "0:/app";
    FIL     fp;
    memset(&fp, 0, sizeof(fp));
    FRESULT  fresult;

    /* Profile point after EEPROM copying and before MMCSD init */
    SBL_ADD_PROFILE_POINT;
    /* Initialization of the driver. */
    FATFS_init();

    /* MMCSD FATFS initialization */
    FATFS_open(0U, NULL, &sbl_fatfsHandle);

    /* If this is a Lazy Boot, then SBL_LazyBootGetAppname will change the 'fileName'
     * to the name of the app that needs to be booted. 'fileName' remains unchanged otherwise.
     */
    SBL_LazyBootGetAppname(fileName);

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

    /* Profile point after MMCSD init and before phy tuning */
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

    return retVal;
}

int32_t SBL_mmcCopyHsmImage(uint8_t* pHsm, uint32_t numBytes)
{
    int32_t retVal = CSL_PASS;
    const TCHAR *fileName = "0:/hsm.bin";
    FIL     fp;
    memset(&fp, 0, sizeof(fp));
    FRESULT  fresult;
    uint32_t bytes_read = 0;
    MMCSD_v2_HwAttrs hwAttrsConfig;

    if(MMCSD_socGetInitCfg(FATFS_initCfg[0].drvInst, &hwAttrsConfig) != 0)
    {
        UART_printf("\nUnable to get MMCSD default config.\r\n");
        retVal = E_FAIL;
    }

    uint32_t wkupCtrlDevstat = SBL_getWkupCtrlDevStat();
    uint32_t mainCtrlDevstat = SBL_getMainCtrlDevStat();

    if(((wkupCtrlDevstat & SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MASK) == SBL_WKUP_DEVSTAT_PRIMARY_BOOT_MMCSD) &&
       ((mainCtrlDevstat & SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK) == SBL_MAIN_DEVSTAT_PRIMARY_BOOT_B_MASK))
    {
        /* MMCSD as primary bootmode */
        /* Check MAIN CTRL MMR DEVSTAT register for Primary boot mode Bus Width */
        if((mainCtrlDevstat & SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK) == SBL_MAIN_DEVSTAT_PRIMARY_BUS_WIDTH_MASK)
        {
            hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_1BIT;
        }
        else
        {
            hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_4BIT;
        }
    }
    else if((mainCtrlDevstat & SBL_MAIN_DEVSTAT_BACKUP_BOOT_MASK) == SBL_MAIN_DEVSTAT_BACKUP_BOOT_MMCSD)
    {
        /* MMCSD as backup bootmode only supports 1-bit bus width, as set by ROM code */
        hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_1BIT;
    }
    else
    {
        hwAttrsConfig.supportedBusWidth = MMCSD_BUS_WIDTH_4BIT;
    }

    hwAttrsConfig.enableInterrupt = ((uint32_t)(0U));
    hwAttrsConfig.configSocIntrPath = NULL;

    if(MMCSD_socSetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0) {
       UART_printf("\nUnable to set config.Exiting. TEST FAILED.\r\n");
       retVal = E_FAIL;
     }

    /* Initialization of the driver. */
    FATFS_init();

    /* MMCSD FATFS initialization */
    FATFS_open(0U, NULL, &sbl_fatfsHandle);

    fresult = f_open(&fp, fileName, ((BYTE)FA_READ));
    if (fresult != FR_OK)
    {
        SBL_log(SBL_LOG_MAX, "\n SD Boot - hsm File open fails \n");
        retVal = SBL_HSM_IMG_NOT_FOUND;
        return retVal;
    }
    else
    {
        fresult  = f_read(&fp, pHsm, numBytes, &bytes_read);
        if (fresult != FR_OK)
        {
            UART_printf("\n SD Boot - hsm read fails \n");
            retVal = E_FAIL;
        }

        f_close(&fp);
    }

    FATFS_close(sbl_fatfsHandle);
    sbl_fatfsHandle = NULL;

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
    MMCSD_v2_HwAttrs hwAttrsConfig;
    volatile uint32_t atcmSize = sblAtcmSize(); 

    /* Disabling the DMA while copying to ATCM since ATCM address is not translated to global address */
    if ((uint32_t) buff < atcmSize)
    {
        if(MMCSD_socGetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig)!=0)
        {
            UART_printf("\nUnable to get default MMCSD config, exiting. SBL Failed.\r\n");
            retVal = E_FAIL;
        }

        hwAttrsConfig.enableDma = 0;
        if(MMCSD_socSetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig) != 0) 
        {
            UART_printf("\nUnable to set MMCSD config, exiting. SBL Failed.\r\n");
            retVal = E_FAIL;
        }
    }

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

    /* Re-Enabling DMA if it is disabled while copying to ATCM */
    if ((uint32_t) buff < atcmSize)
    {
        hwAttrsConfig.enableDma = 1;
        if(MMCSD_socSetInitCfg(FATFS_initCfg[0].drvInst,&hwAttrsConfig) != 0) 
        {
            UART_printf("\nUnable to set MMCSD config, exiting. SBL Failed.\r\n");
            retVal = E_FAIL;
        }
    }

    return retVal;
}

void SBL_FileSeek(void *fileptr, uint32_t location)
{
    FIL *fp = (FIL *) (fileptr);
    f_lseek(fp, location);
}

static uint32_t SBL_getWkupCtrlDevStat()
{
    uint32_t devStat = CSL_REG32_RD(CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_WKUP_DEVSTAT);
    return devStat;
}

static uint32_t SBL_getMainCtrlDevStat()
{
    uint32_t devStat = CSL_REG32_RD(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_MAIN_DEVSTAT);
    return devStat;
}
