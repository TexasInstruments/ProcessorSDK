/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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
 * This example performs QSPI transfer test using polling mode.
 *
 * The JEDEC ID of flash is read and printed out. A block
 * of flash memory is erased by sending erase command and
 * data is  written into the flash from a prefilled buffer.
 * Then, data is read back from it and validated.
 * If the equality test is successful, the test was successful.
 */

#include "qspi_nor_flash_1s.h"
#include <kernel/dpl/DebugP.h>
#include <board/flash/sfdp/nor_spi_sfdp.h>
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <string.h>

#define APP_QSPI_FLASH_OFFSET  (0x40000U)

#define APP_QSPI_DATA_SIZE (256)

/* The source buffer used for transfer */
uint8_t gQspiTxBuf[APP_QSPI_DATA_SIZE];
/* Read buffer MUST be cache line aligned when using DMA */
uint8_t gQspiRxBuf[APP_QSPI_DATA_SIZE] __attribute__((aligned(CacheP_CACHELINE_ALIGNMENT)));

NorSpi_SfdpHeader gSfdpHeader;
NorSpi_SfdpParamHeader gParamHeaders[NOR_SPI_SFDP_NPH_MAX];
NorSpi_SfdpParamHeader *gBfptHeader;

NorSpi_SfdpBasicFlashParamTable gBfpt;
NorSpi_SfdpSectorMapParamTable gSmpt;
NorSpi_SfdpSCCRParamTable gSccr;
NorSpi_SfdpProfile1ParamTable gXpt1;
NorSpi_Sfdp4ByteAddressingParamTable g4bait;

NorSpi_GenericDevDefines gNorSpiDevDefines;

void qspi_flash_diag_test_fill_buffers();
int32_t qspi_flash_diag_test_compare_buffers();
int32_t qspi_flash_diag_print_sfdp(QSPI_Handle handle);
void qspi_flash_diag_print_defines(NorSpi_GenericDevDefines *norSpiDefines);

void qspi_flash_diag(void *args)
{

    int32_t status = SystemP_SUCCESS;
    uint32_t offset;
    uint32_t manfId=0, deviceId=0;

    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();

    DebugP_log("[QSPI Flash Diagnostic Test] Starting ...\r\n");

    QSPI_Handle qspiHandle = QSPI_getHandle(CONFIG_QSPI0);
    qspi_flash_diag_test_fill_buffers();

    /* Zero init the dev defines struct */
    memset(&gNorSpiDevDefines, 0, sizeof(gNorSpiDevDefines));

    QSPI_norFlashInit(qspiHandle);

    /* Read ID */
    status = QSPI_norFlashReadId(qspiHandle, &manfId, &deviceId);

    if(SystemP_SUCCESS == status)
    {
        DebugP_log("[QSPI Flash Diagnostic Test] Flash Manufacturer ID : 0x%X\r\n", manfId);
        DebugP_log("[QSPI Flash Diagnostic Test] Flash Device ID       : 0x%X\r\n", deviceId);

        gNorSpiDevDefines.NOR_SPI_MANF_ID = manfId;
        gNorSpiDevDefines.NOR_SPI_DEVICE_ID = deviceId;
    }

    /* Fill buffers with known data,
     * find block number from offset,
     * erase block, write the data, read back from a specific offset
     * and finally compare the results.
     */
    offset = APP_QSPI_FLASH_OFFSET;

    if( SystemP_SUCCESS == status)
    {
        qspi_flash_diag_test_fill_buffers();
        DebugP_log("[QSPI Flash Diagnostic Test] Executing Flash Erase on first block...\r\n");
        QSPI_norFlashErase(qspiHandle, offset);
        DebugP_log("[QSPI Flash Diagnostic Test] Done !!!\r\n");
        DebugP_log("[QSPI Flash Diagnostic Test] Performing Write-Read Test...\r\n");
        QSPI_norFlashWrite(qspiHandle, offset, gQspiTxBuf, APP_QSPI_DATA_SIZE);
        QSPI_norFlashRead(qspiHandle, offset, gQspiRxBuf, APP_QSPI_DATA_SIZE);

        status |= qspi_flash_diag_test_compare_buffers();

        if(SystemP_SUCCESS == status)
        {
            DebugP_log("[QSPI Flash Diagnostic Test] Write-Read Test Passed!\r\n");
        }
    }

    if(SystemP_SUCCESS == status)
    {
        /* Read the SFDP table and print flash details */
        status = qspi_flash_diag_print_sfdp(qspiHandle);
    }

    if(SystemP_SUCCESS == status)
    {
        DebugP_log("All tests have passed!!\r\n");
    }
    else
    {
        DebugP_log("Some tests have failed!!\r\n");
    }

    Board_driversClose();
    Drivers_close();
}

void qspi_flash_diag_test_fill_buffers()
{
    uint32_t i;

    for(i = 0U; i < APP_QSPI_DATA_SIZE; i++)
    {
        gQspiTxBuf[i] = i;
        gQspiRxBuf[i] = 0U;
    }
}

int32_t qspi_flash_diag_test_compare_buffers()
{
    int32_t status = SystemP_SUCCESS;
    uint32_t i;

    for(i = 0U; i < APP_QSPI_DATA_SIZE; i++)
    {
        if(gQspiTxBuf[i] != gQspiRxBuf[i])
        {
            status = SystemP_FAILURE;
            DebugP_logError("QSPI read data mismatch !!!\r\n");
            break;
        }
    }
    return status;
}

int32_t qspi_flash_diag_print_sfdp(QSPI_Handle handle)
{
    int32_t status = SystemP_SUCCESS;
    uint32_t ptp = 0xFFFFFFFFU;

    /* Read the SFDP header */
    status = QSPI_norFlashReadSfdp(handle, NOR_SPI_SFDP_HEADER_START_OFFSET, (void *)&gSfdpHeader, sizeof(NorSpi_SfdpHeader));

    gBfptHeader = &gSfdpHeader.firstParamHeader;

    /* Check if the signature is read correctly */
    if(gSfdpHeader.sfdpHeader.signature != NOR_SPI_SFDP_SIGNATURE)
    {
        DebugP_log("[QSPI Flash Diagnostic Test] Error in reading SFDP Table or SFDP not supported by Flash !!!\r\n");
    }
    else
    {
        /* Print SFDP basic information */
        DebugP_log("[QSPI Flash Diagnostic Test] SFDP Information : \r\n");
        DebugP_log("================================================\r\n");
        DebugP_log("                      SFDP                      \r\n");
        DebugP_log("================================================\r\n");

        DebugP_log("SFDP Major Revision                       : 0x%X\r\n", gSfdpHeader.sfdpHeader.majorRev);
        DebugP_log("SFDP Minor Revision                       : 0x%X\r\n", gSfdpHeader.sfdpHeader.minorRev);
        DebugP_log("Number of Parameter Headers in this Table : %u\r\n\r\n", gSfdpHeader.sfdpHeader.numParamHeaders + 1);

        /* First parameter header is already read, read the rest of parameter headers if they exist */
        uint32_t nph = gSfdpHeader.sfdpHeader.numParamHeaders;
        uint32_t i;

        if(nph > 0)
        {
            status = QSPI_norFlashReadSfdp(handle, NOR_SPI_SFDP_SECOND_PARAM_HEADER_OFFSET, (void *)&gParamHeaders, nph * sizeof(NorSpi_SfdpParamHeader));
        }

        if(status == SystemP_SUCCESS)
        {
            /* Print the types of parameter tables present */
            DebugP_log("Types of Additional Parameter Tables in this flash\r\n");
            DebugP_log("---------------------------------------------------\r\n");

            for(i = 0; i < nph; i++)
            {
                NorSpi_SfdpParamHeader *paramHeader = &gParamHeaders[i];

                uint32_t paramID = (uint32_t)((uint32_t)(paramHeader->paramIdMsb << 8U) | (uint32_t)(paramHeader->paramIdLsb));

                if(paramID != NOR_SPI_SFDP_BASIC_PARAM_TABLE_ID)
                {
                    char *paramName = NorSpi_Sfdp_getParameterTableName(paramID);

                    if(paramName == NULL)
                    {
                        DebugP_log("Unsupported Parameter Table type!!! - 0x%X\r\n", paramID);
                    }
                    else
                    {
                        DebugP_log("%s\r\n", paramName);
                    }
                }
                else
                {
                    /* Update the gBfpt pointer to the latest version */
                    if((paramHeader->paramTableMajorRev == NOR_SPI_SFDP_JESD216_MAJOR) &&
                        ((paramHeader->paramTableMinorRev > gBfptHeader->paramTableMinorRev) ||
                            ((paramHeader->paramTableMinorRev > gBfptHeader->paramTableMinorRev) &&
                                (paramHeader->paramTableLength > gBfptHeader->paramTableLength))))
                    {
                        gBfptHeader = paramHeader;
                    }
                }
            }
        }

        /* Read the Basic Flash Parameter Table (BFPT) */
        ptp = NorSpi_Sfdp_getPtp(gBfptHeader);

        status = QSPI_norFlashReadSfdp(handle, ptp, (void *)&gBfpt, gBfptHeader->paramTableLength * sizeof(uint32_t));

        /* Parse BFPT */
        if(status == SystemP_SUCCESS)
        {
            status = NorSpi_Sfdp_parseBfpt(&gBfpt, &gNorSpiDevDefines, gBfptHeader->paramTableLength);
        }

        /* Parse other parameter tables */
        for(i = 0; i < nph; i++)
        {
            NorSpi_SfdpParamHeader *paramHeader = &gParamHeaders[i];

            uint32_t paramID = (uint32_t)((uint32_t)(paramHeader->paramIdMsb << 8U) | (uint32_t)(paramHeader->paramIdLsb));

            ptp = NorSpi_Sfdp_getPtp(paramHeader);

            switch(paramID)
            {
                case NOR_SPI_SFDP_PROFILE_TABLE_ID:
                    status = QSPI_norFlashReadSfdp(handle, ptp, (void *)&gXpt1, paramHeader->paramTableLength * sizeof(uint32_t));
                    status = NorSpi_Sfdp_parseXpt1(&gXpt1, &gNorSpiDevDefines, paramHeader->paramTableLength);
                    break;

                case NOR_SPI_SFDP_4BYTE_ADDR_INSTR_TABLE_ID:
                    status = QSPI_norFlashReadSfdp(handle, ptp, (void *)&g4bait, paramHeader->paramTableLength * sizeof(uint32_t));
                    status = NorSpi_Sfdp_parse4bait(&g4bait, &gNorSpiDevDefines, paramHeader->paramTableLength);
                    break;

                case NOR_SPI_SFDP_SECTOR_MAP_TABLE_ID:
                    status = QSPI_norFlashReadSfdp(handle, ptp, (void *)&gSmpt, paramHeader->paramTableLength * sizeof(uint32_t));
                    status = NorSpi_Sfdp_parseSmpt(&gSmpt, &gNorSpiDevDefines, paramHeader->paramTableLength);
                    break;

                case NOR_SPI_SFDP_SCCR_TABLE_ID:
                    status = QSPI_norFlashReadSfdp(handle, ptp, (void *)&gSccr, paramHeader->paramTableLength * sizeof(uint32_t));
                    status = NorSpi_Sfdp_parseSccr(&gSccr, &gNorSpiDevDefines, paramHeader->paramTableLength);
                    break;

                default:
                    /* Parsing not yet supported */
                    DebugP_log("\r\n");
                    char *paramName = NorSpi_Sfdp_getParameterTableName(paramID);
                    if(paramName != NULL)
                    {
                        DebugP_log("Parsing of %s table not yet supported. \r\n", paramName);
                    }
                    break;
            }
        }

        /* Print the final config */
        if(status == SystemP_SUCCESS)
        {
            qspi_flash_diag_print_defines(&gNorSpiDevDefines);
        }
    }

    return status;
}

void qspi_flash_diag_print_defines(NorSpi_GenericDevDefines *norSpiDefines)
{
    if(norSpiDefines != NULL)
    {
        DebugP_log("\r\n");

        DebugP_log("Flash_NorSpiDevDefines gFlashNorSpiDeviceDefines_<part-number> = {\r\n");
        DebugP_log("\r\n");
        DebugP_log("    .NOR_SPI_CMD_RSTEN = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_RSTEN);
        DebugP_log("    .NOR_SPI_CMD_RSTMEM = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_RSTMEM);
        DebugP_log("    .NOR_SPI_CMD_WREN = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_WREN);
        DebugP_log("    .NOR_SPI_CMD_WRREG = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_WRREG);
        DebugP_log("    .NOR_SPI_CMD_BULK_ERASE = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_BULK_ERASE);
        DebugP_log("    .NOR_SPI_CMD_SECTOR_ERASE_3B = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_SECTOR_ERASE_3B);
        DebugP_log("    .NOR_SPI_CMD_SECTOR_ERASE_4B = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_SECTOR_ERASE_4B);
        DebugP_log("    .NOR_SPI_CMD_BLOCK_ERASE_3B = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_BLOCK_ERASE_3B);
        DebugP_log("    .NOR_SPI_CMD_BLOCK_ERASE_4B = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_BLOCK_ERASE_4B);
        DebugP_log("    .NOR_SPI_CMD_PAGE_PROG_3B = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_PAGE_PROG_3B);
        DebugP_log("    .NOR_SPI_CMD_PAGE_PROG_4B = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_PAGE_PROG_4B);
        DebugP_log("    .NOR_SPI_CMD_RDSR = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_RDSR);
        DebugP_log("    .NOR_SPI_CMD_RDREG = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_RDREG);
        DebugP_log("    .NOR_SPI_CMD_RDID = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_RDID);
        DebugP_log("    .NOR_SPI_CMD_READ = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_READ);
        DebugP_log("    .NOR_SPI_CMD_888_SDR_READ = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_888_SDR_READ);
        DebugP_log("    .NOR_SPI_CMD_888_DDR_READ = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_888_DDR_READ);
        DebugP_log("    .NOR_SPI_CMD_444_SDR_READ = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_444_SDR_READ);
        DebugP_log("    .NOR_SPI_CMD_444_DDR_READ = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_444_DDR_READ);
        DebugP_log("    .NOR_SPI_CMD_114_READ = 0x%02X,\r\n", norSpiDefines->NOR_SPI_CMD_114_READ);
        DebugP_log("    .NOR_SPI_SR_WIP = %u,\r\n", norSpiDefines->NOR_SPI_SR_WIP);
        DebugP_log("    .NOR_SPI_SR_WEL = %u,\r\n", norSpiDefines->NOR_SPI_SR_WEL);
        DebugP_log("    .NOR_SPI_RDID_NUM_BYTES = %u,\r\n", norSpiDefines->NOR_SPI_RDID_NUM_BYTES);
        DebugP_log("    .NOR_SPI_MANF_ID = 0x%02X,\r\n", norSpiDefines->NOR_SPI_MANF_ID);
        DebugP_log("    .NOR_SPI_DEVICE_ID = 0x%03X,\r\n", norSpiDefines->NOR_SPI_DEVICE_ID);
        DebugP_log("    .NOR_SPI_114_READ_MODE_CLKS = %u,\r\n", norSpiDefines->NOR_SPI_114_READ_MODE_CLKS);
        DebugP_log("    .NOR_SPI_114_READ_DUMMY_CYCLES = %u,\r\n", norSpiDefines->NOR_SPI_114_READ_DUMMY_CYCLES);
        DebugP_log("    .NOR_SPI_444_READ_MODE_CLKS = %u,\r\n", norSpiDefines->NOR_SPI_444_READ_MODE_CLKS);
        DebugP_log("    .NOR_SPI_444_READ_DUMMY_CYCLES = %u,\r\n", norSpiDefines->NOR_SPI_444_READ_DUMMY_CYCLES);
        DebugP_log("    .NOR_SPI_444_READ_DUMMY_CYCLES_LC = 0x%02X,\r\n", norSpiDefines->NOR_SPI_444_READ_DUMMY_CYCLES_LC);
        DebugP_log("    .NOR_SPI_QUAD_CMD_READ_DUMMY_CYCLES = 0x%02X,\r\n", norSpiDefines->NOR_SPI_QUAD_CMD_READ_DUMMY_CYCLES);
        DebugP_log("    .NOR_SPI_OCTAL_READ_DUMMY_CYCLE = %u,\r\n", norSpiDefines->NOR_SPI_OCTAL_READ_DUMMY_CYCLE);
        DebugP_log("    .NOR_SPI_OCTAL_READ_DUMMY_CYCLE_LC = 0x%02X,\r\n", norSpiDefines->NOR_SPI_OCTAL_READ_DUMMY_CYCLE_LC);
        DebugP_log("    .NOR_SPI_OCTAL_DDR_RDSR_DUMMY_CYCLE = %u,\r\n", norSpiDefines->NOR_SPI_OCTAL_DDR_RDSR_DUMMY_CYCLE);
        DebugP_log("    .NOR_SPI_OCTAL_DDR_RDREG_ADDR_BYTES = %u, \r\n", norSpiDefines->NOR_SPI_OCTAL_DDR_RDREG_ADDR_BYTES);
        DebugP_log("    .NOR_SPI_OCTAL_DDR_WRREG_ADDR_BYTES = %u, \r\n", norSpiDefines->NOR_SPI_OCTAL_DDR_WRREG_ADDR_BYTES);
        DebugP_log("    .NOR_SPI_OCTAL_DDR_RDVREG_DUMMY_CYCLE = %u, \r\n", norSpiDefines->NOR_SPI_OCTAL_DDR_RDVREG_DUMMY_CYCLE);
        DebugP_log("    .NOR_SPI_OCTAL_DDR_RDNVREG_DUMMY_CYCLE = %u, \r\n", norSpiDefines->NOR_SPI_OCTAL_DDR_RDNVREG_DUMMY_CYCLE);
        DebugP_log("    .NOR_SPI_OCTAL_RDSFDP_DUMMY_CYCLE = %u, \r\n", norSpiDefines->NOR_SPI_OCTAL_RDSFDP_DUMMY_CYCLE);
        DebugP_log("    .NOR_SPI_OCTAL_RDSFDP_ADDR_TYPE = %u, \r\n", norSpiDefines->NOR_SPI_OCTAL_RDSFDP_ADDR_TYPE);
        DebugP_log("    .NOR_SPI_WRR_WRITE_TIMEOUT = %u,\r\n", norSpiDefines->NOR_SPI_WRR_WRITE_TIMEOUT);
        DebugP_log("    .NOR_SPI_BULK_ERASE_TIMEOUT = %u,\r\n", norSpiDefines->NOR_SPI_BULK_ERASE_TIMEOUT);
        DebugP_log("    .NOR_SPI_PAGE_PROG_TIMEOUT = %u,\r\n", norSpiDefines->NOR_SPI_PAGE_PROG_TIMEOUT);
        DebugP_log("    .NOR_SPI_VREG_OFFSET = 0x%X,\r\n", norSpiDefines->NOR_SPI_VREG_OFFSET);
        DebugP_log("    .NOR_SPI_NVREG_OFFSET = 0x%X,\r\n", norSpiDefines->NOR_SPI_NVREG_OFFSET);
        DebugP_log("    .NOR_SPI_QUAD_MODE_CFG_ADDR = 0x%X,\r\n", norSpiDefines->NOR_SPI_QUAD_MODE_CFG_ADDR);
        DebugP_log("    .NOR_SPI_QUAD_MODE_CFG_BIT_LOCATION = 0x%X,\r\n", norSpiDefines->NOR_SPI_QUAD_MODE_CFG_BIT_LOCATION);
        DebugP_log("    .NOR_SPI_DDR_OCTAL_MODE_CFG_ADDR = 0x%X,\r\n", norSpiDefines->NOR_SPI_DDR_OCTAL_MODE_CFG_ADDR);
        DebugP_log("    .NOR_SPI_DDR_OCTAL_MODE_CFG_BIT_LOCATION = 0x%X,\r\n", norSpiDefines->NOR_SPI_DDR_OCTAL_MODE_CFG_BIT_LOCATION);
        DebugP_log("    .NOR_SPI_DUMMY_CYCLE_CFG_ADDR = 0x%X,\r\n", norSpiDefines->NOR_SPI_DUMMY_CYCLE_CFG_ADDR);
        DebugP_log("    .NOR_SPI_FLASH_SIZE = %u,\r\n", norSpiDefines->NOR_SPI_FLASH_SIZE);
        DebugP_log("    .NOR_SPI_PAGE_SIZE = %u,\r\n", norSpiDefines->NOR_SPI_PAGE_SIZE);
        DebugP_log("    .NOR_SPI_BLOCK_SIZE = %u,\r\n", norSpiDefines->NOR_SPI_BLOCK_SIZE);
        DebugP_log("    .NOR_SPI_SECTOR_SIZE = %u,\r\n", norSpiDefines->NOR_SPI_SECTOR_SIZE);
        DebugP_log("    .addrByteSupport = %u,\r\n", norSpiDefines->addrByteSupport);
        DebugP_log("    .dtrSupport = %u,\r\n", norSpiDefines->addrByteSupport);
        DebugP_log("    .qeType = %u,\r\n", norSpiDefines->qeType);
        DebugP_log("    .seq444Enable = { %u, %u, %u, %u, %u },\r\n", norSpiDefines->seq444Enable[0], norSpiDefines->seq444Enable[1], norSpiDefines->seq444Enable[2], norSpiDefines->seq444Enable[3], norSpiDefines->seq444Enable[4] );
        DebugP_log("    .seq444Disable = { %u, %u, %u, %u },\r\n", norSpiDefines->seq444Disable[0], norSpiDefines->seq444Disable[1], norSpiDefines->seq444Disable[2], norSpiDefines->seq444Disable[3]);
        DebugP_log("    .oeType = %u,\r\n", norSpiDefines->oeType);
        DebugP_log("    .cmdExtType = %u,\r\n", norSpiDefines->cmdExtType);
        DebugP_log("    .byteOrder = %u,\r\n", norSpiDefines->byteOrder);
        DebugP_log("};\r\n\r\n");
    }
}