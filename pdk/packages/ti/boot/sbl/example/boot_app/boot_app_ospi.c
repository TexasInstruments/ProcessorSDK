/*
 *  Copyright (c) Texas Instruments Incorporated 2024
 *  All rights reserved.
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
 */

/**
 *  \file boot_app_ospi.c
 *
 *  \brief supporting file for main file for ospi
 */

#include "boot_app_priv.h"
#include "boot_app_ospi.h"

int32_t BootApp_ospiLeaveConfigSPI(bool isNandBootEnabled)
{
    int32_t retVal = E_PASS;
    Board_flashHandle flashHandle;
    Board_FlashInfo *flashInfo;

#if defined(UART_PRINT_DEBUG)
    UART_printf("Entered BootApp_ospiLeaveConfigSPI function...\n");
#endif

    /* Get default OSPI cfg */
    OSPI_socGetInitCfg(BOARD_OSPI_DOMAIN, BOARD_OSPI_NOR_INSTANCE, &gOspiCfg);

    gOspiCfg.funcClk = OSPI_MODULE_CLK_133M;
    /* Configure the flash for SPI mode */
    gOspiCfg.xferLines = OSPI_XFER_LINES_SINGLE;
    /* Put controller in DAC mode so flash ID can be read directly */
    gOspiCfg.dacEnable = BTRUE;
    /* Disable PHY in legacy SPI mode (1-1-1) */
    gOspiCfg.phyEnable = BFALSE;
    gOspiCfg.dtrEnable = BFALSE;
    gOspiCfg.xipEnable = BFALSE;

    /* Set the default SPI init configurations */
    OSPI_socSetInitCfg(BOARD_OSPI_DOMAIN, BOARD_OSPI_NOR_INSTANCE, &gOspiCfg);

#if defined(SOC_J721S2) || defined(SOC_J784S4)
    if (isNandBootEnabled == BTRUE)
    {
        flashHandle = Board_flashOpen(BOARD_FLASH_ID_W35N01JWTBAG,
                            BOARD_OSPI_NOR_INSTANCE, NULL);
    }
    else
    {
        flashHandle = Board_flashOpen(BOARD_FLASH_ID_S28HS512T,
                            BOARD_OSPI_NOR_INSTANCE, NULL);
    }
#elif defined(SOC_J7200)
    flashHandle = Board_flashOpen(BOARD_FLASH_ID_S28HS512T,
                        BOARD_OSPI_NOR_INSTANCE, NULL);
#else
    flashHandle = Board_flashOpen(BOARD_FLASH_ID_MT35XU512ABA1G12,
                        BOARD_OSPI_NOR_INSTANCE, NULL);
#endif

    if (flashHandle)
    {
        UART_printf("OSPI flash left configured in Legacy SPI mode.\n");
        flashInfo = (Board_FlashInfo *)flashHandle;
        UART_printf("\n OSPI NOR device ID: 0x%x, manufacturer ID: 0x%x \n",
                flashInfo->device_id, flashInfo->manufacturer_id);
        Board_flashClose(flashHandle);
    }
    else
    {
        UART_printf("Board_flashOpen failed in SPI mode!!\n");
        retVal = E_FAIL;
    }

    return(retVal);
}

/* Boot all the OSPI images defined in the array */

int32_t BootApp_ospiBootStageImage(sblEntryPoint_t *pEntry, uint32_t address)
{
    int32_t status = E_FAIL;

    if ((0U != address) && (NULL != pEntry))
    {
        if (address != MAIN_DOMAIN_HLOS)
        {
            status = BootApp_OSPIBootImageLate(&gK3xx_evmEntry, address);
        }
        #if defined(MPU1_HLOS_BOOT_ENABLED)
        else
        {
            /* Load the HLOS appimages */
            status = BootApp_OSPIBootImageLate(&gK3xx_evmEntry, OSPI_OFFSET_A72IMG1);
            if (status != E_PASS)
            {
                UART_printf("Error parsing A72 appimage #1 for HLOS boot\n");
            }
            else
            {
                status = BootApp_OSPIBootImageLate(&gK3xx_evmEntry, OSPI_OFFSET_A72IMG2);
                if (status != E_PASS)
                {
                    UART_printf("Error parsing A72 appimage #2 for HLOS boot\n");
                }
                #if defined(HLOS_BOOT_LINUX_OS)
                else
                {
                    status = BootApp_OSPIBootImageLate(&gK3xx_evmEntry, OSPI_OFFSET_A72IMG3);
                    if (status != E_PASS)
                        UART_printf("Error parsing A72 appimage #3 for HLOS boot\n");
                }
                #endif
            }

            if (status == E_PASS)
            {
                /* Set the A72 entry point at the ATF address */
                pEntry->CpuEntryPoint[MPU1_CPU0_ID] = ATF_START_RAM_ADDR;
                BootApp_McuDCacheClean((void *)0x70000000, 0x20000);
            }
        } /* if (address == MAIN_DOMAIN_HLOS) */
        #endif /* #if defined(MPU1_HLOS_BOOT_ENABLED)*/
    } /* if ((NULL != address) && (NULL != pEntry)) */

    return status;
}

int32_t BootApp_OSPIBootImageLate(sblEntryPoint_t *pEntry, uint32_t imageOffset)
{
    int32_t retVal = E_FAIL;

    uint32_t offset = 0;

    /* Load the MAIN domain remotecore images included in the appimage */
    offset = imageOffset;

    fp_readData = &SBL_OSPI_ReadSectors;
    fp_seek     = &SBL_OSPI_seek;

    retVal = SBL_MulticoreImageParse((void *) &offset,
                                      imageOffset,
                                      pEntry,
                                      SBL_SKIP_BOOT_AFTER_COPY);

    if (retVal != E_PASS)
        UART_printf("Error parsing Main Domain appimage\n");

    return retVal;
}
