/*
 * Copyright (c) 2018 - 2022, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

//#include "board_utils.h"
#include <ti/board/src/flash/nor/ospi/nor_qspi.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/csl/soc.h>

static NOR_HANDLE Nor_qspiOpen(uint32_t norIntf, uint32_t portNum, void *params);
static void Nor_qspiClose(NOR_HANDLE handle);
static NOR_STATUS Nor_qspiRead(NOR_HANDLE handle, uint32_t addr,
                               uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_qspiWrite(NOR_HANDLE handle, uint32_t addr,
                                uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_qspiErase(NOR_HANDLE handle, int32_t eraseCnt, bool blkErase);

static NOR_STATUS Nor_qspiCmdWrite(OSPI_Handle handle, uint8_t *cmdBuf,
                                   uint32_t cmdLen, uint32_t dataLen);
static NOR_STATUS Nor_qspiWaitReady(OSPI_Handle handle, uint32_t timeOut);

/* NOR function table for NOR OSPI interface implementation */
const NOR_FxnTable Nor_qspiFxnTable =
{
    &Nor_qspiOpen,
    &Nor_qspiClose,
    &Nor_qspiRead,
    &Nor_qspiWrite,
    &Nor_qspiErase,
};

NOR_Info Nor_qspiInfo =
{
    0,                          /* hwHandle */
    0,                          /* manufacturerId */
    0,                          /* deviceId */
    0,                          /* busWidth */
    NOR_NUM_BLOCKS,            /* blockCnt */
    NOR_NUM_PAGES_PER_BLOCK,   /* pageCnt */
    NOR_PAGE_SIZE,             /* pageSize */
    0,                         /* baseAddr */
    NOR_SECTOR_SIZE            /* sectorSize */
};

static NOR_STATUS NOR_qspiCmdRead(OSPI_Handle handle, uint8_t *cmdBuf,
                            uint32_t cmdLen, uint8_t *rxBuf, uint32_t rxLen)
{
    OSPI_Transaction  transaction;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_READ;
    bool             ret;

    /* Update the mode and transfer type with the required values */
    OSPI_control(handle, OSPI_V0_CMD_SET_CFG_MODE, NULL);
    OSPI_control(handle, OSPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

    transaction.txBuf = (void *)cmdBuf;
    transaction.rxBuf = (void *)rxBuf;
    transaction.count = cmdLen + rxLen;

    ret = OSPI_transfer(handle, &transaction);
    if (BTRUE == ret)
    {
        return NOR_PASS;
    }
	else
    {
        return NOR_FAIL;
    }
}

static NOR_STATUS Nor_qspiReadId(OSPI_Handle handle)
{
    NOR_STATUS  retVal;
    uint8_t     idCode[NOR_RDID_NUM_BYTES];
    uint8_t     cmd = NOR_CMD_RDID;
    uint32_t    manfID, devID;

    retVal = NOR_qspiCmdRead(handle, &cmd, 1, idCode, NOR_RDID_NUM_BYTES);
    if (NOR_PASS == retVal)
    {
        manfID = (uint32_t)idCode[0];
        devID = ((uint32_t)idCode[1] << 8) | ((uint32_t)idCode[2]);
        if ((NOR_MANF_ID == manfID) && (NOR_DEVICE_ID == devID))
        {
            Nor_qspiInfo.manufacturerId = manfID;
            Nor_qspiInfo.deviceId = devID;
        }
        else
        {
            retVal = NOR_FAIL;
        }
    }

    return (retVal);
}

static NOR_STATUS Nor_qspiEnableDDR(OSPI_Handle handle)
{
    OSPI_v0_HwAttrs const *hwAttrs;
    NOR_STATUS       retVal = NOR_PASS;
    uint8_t          cmdWren = NOR_CMD_WREN;
    uint8_t          data[3];
    uint32_t         opCode[3];
    uint32_t         dummyCycles;
    uint32_t         rx_lines;

    hwAttrs = (OSPI_v0_HwAttrs const *)handle->hwAttrs;

#if defined (j721e_evm) || defined (j7200_evm) || defined (am64x_svb) || defined (am640x_svb) || defined (j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm)
    /* Send Write Enable command */
    if (Nor_qspiCmdWrite(handle, &cmdWren, 1, 0))
    {
    	return NOR_FAIL;
    }

    if (Nor_qspiWaitReady(handle, NOR_WRR_WRITE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    /* Write Enhanced VCR register to enable DDR mode */
    data[0] = NOR_CMD_WRITE_ENVCR;
    data[1] = 0x5F;  /* Enable quad mode and DTR mode */
    retVal = Nor_qspiCmdWrite(handle, data, 1, 1);
    if (NOR_PASS == retVal)
    {
        /* Set opcodes */
        dummyCycles = NOR_QUAD_READ_DUMMY_CYCLE - 2U;
        rx_lines    = OSPI_XFER_LINES_QUAD;
        opCode[0]   = NOR_CMD_QUAD_DDR_O_FAST_RD;
        opCode[1]   = NOR_CMD_QUAD_FAST_PROG;
        opCode[2]   = NOR_CMD_RDSR;

        /* Update the read opCode, rx lines and read dummy cycles */
        OSPI_control(handle, OSPI_V0_CMD_RD_DUMMY_CLKS, (void *)&dummyCycles);
        OSPI_control(handle, OSPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
        OSPI_control(handle, OSPI_V0_CMD_XFER_OPCODE, (void *)opCode);
    }
#else
    /* Send Write Enable command */
    if (Nor_qspiCmdWrite(handle, &cmdWren, 1, 0))
    {
        return NOR_FAIL;
    }

    /* Write CR1 register to enable QSPI mode */
    data[0] = NOR_CMD_WRR;
    data[1] = 0x02;
    retVal = Nor_qspiCmdWrite(handle, data, 1, 1);
    if (NOR_PASS == retVal)
    {
        if (Nor_qspiWaitReady(handle, NOR_WRR_WRITE_TIMEOUT))
        {
            return NOR_FAIL;
        }

        /* Set opcodes */
        dummyCycles = NOR_QUAD_READ_DUMMY_CYCLE;
        rx_lines    = OSPI_XFER_LINES_QUAD;
        opCode[0]   = NOR_CMD_QUAD_DDR_IO_READ;
        opCode[1]   = NOR_CMD_QUAD_PAGE_PROG;
        opCode[2]   = NOR_CMD_RDSR;

        /* Update the read opCode, rx lines and read dummy cycles */
        OSPI_control(handle, OSPI_V0_CMD_RD_DUMMY_CLKS, (void *)&dummyCycles);
        OSPI_control(handle, OSPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
        OSPI_control(handle, OSPI_V0_CMD_XFER_OPCODE, (void *)opCode);
    }
#endif
    CSL_ospiDtrEnable((const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr), UTRUE);

    return retVal;
}

static NOR_STATUS Nor_qspiEnableSDR(OSPI_Handle handle)
{
    OSPI_v0_HwAttrs const *hwAttrs;
    CSL_ospi_flash_cfgRegs *regAddr;
    NOR_STATUS       retVal = NOR_PASS;
    uint8_t          cmdWren = NOR_CMD_WREN;
    uint8_t          data[3];
    uint32_t         opCode[3];
    uint32_t         dummyCycles;
    uint32_t         rx_lines;
    uint32_t         regVal;

    hwAttrs = (OSPI_v0_HwAttrs const *)handle->hwAttrs;

#if defined (j721e_evm) || defined (j7200_evm) || defined (am64x_svb) || defined (am640x_svb) || defined (j721s2_evm) || defined(j784s4_evm) || defined(j742s2_evm)
    /* Send Write Enable command */
    if (Nor_qspiCmdWrite(handle, &cmdWren, 1, 0))
    {
    	return NOR_FAIL;
    }

    /* Write Enhanced VCR register to enable quad mode */
    data[0] = NOR_CMD_WRITE_ENVCR;
    data[1] = 0x7F;  /* Enable quad mode */
    retVal = Nor_qspiCmdWrite(handle, data, 1, 1);
    if (NOR_PASS == retVal)
    {
        dummyCycles = NOR_QUAD_READ_DUMMY_CYCLE;
        rx_lines    = OSPI_XFER_LINES_QUAD;
        opCode[0]     = NOR_CMD_QUAD_IO_FAST_RD;
        opCode[1]     = NOR_CMD_QUAD_FAST_PROG;
        opCode[2]     = NOR_CMD_RDSR;

        OSPI_control(handle, OSPI_V0_CMD_RD_DUMMY_CLKS, (void *)&dummyCycles);
        OSPI_control(handle, OSPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
        OSPI_control(handle, OSPI_V0_CMD_XFER_OPCODE, (void *)opCode);
    }
#else
    /* Send Write Enable command */
    if (Nor_qspiCmdWrite(handle, &cmdWren, 1, 0))
    {
        return NOR_FAIL;
    }

    /* Write CR1 register to enable QSPI mode */
    data[0] = NOR_CMD_WRR;
    data[1] = 0x02;
    retVal = Nor_qspiCmdWrite(handle, data, 1, 1);
    if (NOR_PASS == retVal)
    {
        rx_lines      = OSPI_XFER_LINES_QUAD;
        opCode[0]     = NOR_CMD_QUAD_READ;
        opCode[1]     = NOR_CMD_PAGE_PROG;
        opCode[2]     = NOR_CMD_RDSR;

        OSPI_control(handle, OSPI_V0_CMD_RD_DUMMY_CLKS, (void *)&dummyCycles);
        OSPI_control(handle, OSPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
        OSPI_control(handle, OSPI_V0_CMD_XFER_OPCODE, (void *)opCode);
    }
#endif
    /* Flash device requires 4-bit access for command as well in quad mode */
    regAddr = (CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr);
    regVal = CSL_REG32_RD(&regAddr->DEV_INSTR_RD_CONFIG_REG);
    regVal |= 0x200;
    CSL_REG32_WR(&regAddr->DEV_INSTR_RD_CONFIG_REG, regVal);

    return retVal;
}

static NOR_STATUS Nor_qspiXipEnable(OSPI_Handle handle)
{
    return NOR_PASS;
}


NOR_HANDLE Nor_qspiOpen(uint32_t norIntf, uint32_t portNum, void *params)
{
    OSPI_Params      spiParams;  /* SPI params structure */
    OSPI_Handle      hwHandle;  /* SPI handle */
    NOR_HANDLE      norHandle = 0;
    uint32_t        i = 0;
    uint32_t        delay;
    uint32_t        readCnt = 0;
    uint32_t        readStart = 0;
    uint32_t        readCntPrv = 0;
    uint32_t        readStartPrv = 0;
    OSPI_v0_HwAttrs ospi_cfg;
    NOR_STATUS      retVal;

    if (params)
    {
		memcpy(&spiParams, params, sizeof(OSPI_Params));
    }
    else
    {
        /* Use default SPI config params if no params provided */
		OSPI_Params_init(&spiParams);
    }
    hwHandle = (OSPI_Handle)OSPI_open(SPI_OSPI_DOMAIN_MCU, portNum, &spiParams);

    if (hwHandle)
    {
        retVal = NOR_PASS;
        if (NOR_PASS == retVal)
        {
            OSPI_socGetInitCfg(SPI_OSPI_DOMAIN_MCU, portNum, &ospi_cfg);
            if (BTRUE == ospi_cfg.dtrEnable)
            {
                Nor_qspiEnableDDR(hwHandle);
            }
            else
            {
                Nor_qspiEnableSDR(hwHandle);
            }

            if (BTRUE == ospi_cfg.phyEnable)
            {
                /* set initial PHY DLL delay */
                delay = 0U;
                OSPI_control(hwHandle, OSPI_V0_CMD_CFG_PHY, (void *)(&delay));

                /* calibrate PHY */
                for (i = 0; i < 128U; i++)
                {
                    if (NOR_PASS == Nor_qspiReadId(hwHandle))
                    {
                        /* Iterate flash reads, find the start index and successful read ID count */
                        if (0U == readCnt)
                            readStart = i;
                        readCnt++;
                    }
                    else
                    {
                        if ((0U != readCnt) && (readCnt > readCntPrv))
                        {
                            /* save the start index and most successful read ID count */
                            readCntPrv = readCnt;
                            readStartPrv = readStart;
                            readCnt = 0;
                            readStart = 0;
                        }
                    }

                    /* Increment DLL delay */
                    OSPI_control(hwHandle, OSPI_V0_CMD_CFG_PHY, NULL);
                }

                if (readCnt > readCntPrv)
                {
                    readCntPrv = readCnt;
                    readStartPrv = readStart;
                }

                if (0U != readCntPrv)
                {
                    /* Find the delay in the middle working position */
                    delay = readStartPrv + (readCntPrv / 2);
                    OSPI_control(hwHandle, OSPI_V0_CMD_CFG_PHY, (void *)(&delay));
                    Nor_qspiInfo.hwHandle = (uintptr_t)hwHandle;
                    norHandle = (NOR_HANDLE)(&Nor_qspiInfo);
                }
                else
                {
                    OSPI_close(hwHandle);
                }
            }
            else /* ospi_cfg->phyEnable == BFALSE */
            {
                if (NOR_PASS == Nor_qspiReadId(hwHandle))
                {
                    Nor_qspiInfo.hwHandle = (uintptr_t)hwHandle;
                    norHandle = (NOR_HANDLE)(&Nor_qspiInfo);
                }
                else
                {
                    OSPI_close(hwHandle);
                }
            }

            if (BTRUE == ospi_cfg.xipEnable)
            {
                Nor_qspiXipEnable(hwHandle);
            }
        }
        else /* Nor_ospiEnableDDR(hwHandle) != NOR_PASS */
        {
            OSPI_close(hwHandle);
        }
    }

    return (norHandle);
}

void Nor_qspiClose(NOR_HANDLE handle)
{
    NOR_Info    *norOspiInfo;
    OSPI_Handle   spiHandle;

    if (handle)
    {
        norOspiInfo = (NOR_Info *)handle;
        spiHandle = (OSPI_Handle)norOspiInfo->hwHandle;

        if (spiHandle)
        {
            OSPI_close(spiHandle);
        }
    }
}

static NOR_STATUS Nor_qspiCmdWrite(OSPI_Handle handle, uint8_t *cmdBuf,
                                        uint32_t cmdLen, uint32_t dataLen)
{
    OSPI_Transaction  transaction;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_WRITE;
    bool             ret;

    /* Update the mode and transfer type with the required values */
    OSPI_control(handle, OSPI_V0_CMD_SET_CFG_MODE, NULL);
    OSPI_control(handle, OSPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

    transaction.txBuf = (void *)cmdBuf; /* Buffer includes command and write data */
    transaction.count = cmdLen + dataLen;
    transaction.rxBuf = NULL;
    transaction.arg = (void *)(uintptr_t)dataLen;

    ret = OSPI_transfer(handle, &transaction);
    if (BTRUE == ret)
    {
        return NOR_PASS;
    }
	else
    {
        return NOR_FAIL;
    }
}

static NOR_STATUS Nor_qspiWaitReady(OSPI_Handle handle, uint32_t timeOut)
{
    uint8_t         status;
    uint8_t         cmd = NOR_CMD_RDSR;

    do
    {
        if (NOR_qspiCmdRead(handle, &cmd, 1, &status, 1))
        {
            return NOR_FAIL;
        }
        if (0U == (status & NOR_SR_WIP))
        {
            break;
        }

        timeOut--;
        if (!timeOut) {
            break;
        }

    } while (1);

    if (0U == (status & NOR_SR_WIP))
    {
        return NOR_PASS;
    }

    /* Timed out */
    return NOR_FAIL;
}

static OSPI_Transaction transaction;
NOR_STATUS Nor_qspiRead(NOR_HANDLE handle, uint32_t addr,
                        uint32_t len, uint8_t *buf, uint32_t mode)
{
    NOR_Info        *norOspiInfo;
    OSPI_Handle       spiHandle;
    bool             ret;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_READ;

    if (!handle)
    {
        return NOR_FAIL;
    }

    norOspiInfo = (NOR_Info *)handle;
    if (!norOspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }
    spiHandle = (OSPI_Handle)norOspiInfo->hwHandle;

    /* Validate address input */
    if (NOR_SIZE < (addr + len))
    {
        return NOR_FAIL;
    }
    /* Set transfer mode and read type */
    OSPI_control(spiHandle, OSPI_V0_CMD_SET_XFER_MODE, NULL);
    OSPI_control(spiHandle, OSPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

    transaction.arg   = (void *)(uintptr_t)addr;
    transaction.txBuf = NULL;
    transaction.rxBuf = (void *)buf;
    transaction.count = len;

    ret = OSPI_transfer(spiHandle, &transaction);
    if (BTRUE == ret)
    {
        return NOR_PASS;
    }
	else
    {
        return NOR_FAIL;
    }
}

NOR_STATUS Nor_qspiWrite(NOR_HANDLE handle, uint32_t addr, uint32_t len,
                         uint8_t *buf, uint32_t mode)
{
    NOR_Info        *norOspiInfo;
    OSPI_Handle       spiHandle;
    bool             ret;
    uint32_t         byteAddr;
    uint32_t         wrSize = len;
    uint32_t         chunkLen;
    uint32_t         actual;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_WRITE;
    OSPI_v0_HwAttrs *hwAttrs;

    if (!handle)
    {
        return NOR_FAIL;
    }

    norOspiInfo = (NOR_Info *)handle;
    if (!norOspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }

    /* Validate address input */
    if (NOR_SIZE < (addr + len))
    {
        return NOR_FAIL;
    }

    spiHandle = (OSPI_Handle)norOspiInfo->hwHandle;
    hwAttrs = (OSPI_v0_HwAttrs *)spiHandle->hwAttrs;

    /* Set the transfer mode, write op code and tx lines */
    OSPI_control(spiHandle, OSPI_V0_CMD_SET_XFER_MODE, NULL);
    OSPI_control(spiHandle, OSPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

    if (hwAttrs->dacEnable )
    {
        /* direct access transfer mode */
        if ((hwAttrs->dmaEnable) && (hwAttrs->phyEnable))
        {
            wrSize = 16U;
        }
    }
    else
    {
        /* indirect access transfer mode */
        if (hwAttrs->intrEnable)
        {
            wrSize = 256U;
        }
    }
    byteAddr = addr & (wrSize - 1U);

    for (actual = 0U; actual < len; actual += chunkLen)
    {
        /* Send Page Program command */
        chunkLen = (((len - actual) < (wrSize - byteAddr)) ?
                    (len - actual) : (wrSize - byteAddr));

        transaction.arg   = (void *)(uintptr_t)addr;
        transaction.txBuf = (void *)(buf + actual);
        transaction.rxBuf = NULL;
        transaction.count = chunkLen;

        ret = OSPI_transfer(spiHandle, &transaction);
        if (BFALSE == ret)
        {
            return NOR_FAIL;
        }

        addr += chunkLen;
        byteAddr = 0;
    }

    return NOR_PASS;
}

NOR_STATUS Nor_qspiErase(NOR_HANDLE handle, int32_t erLoc, bool blkErase)
{
    uint8_t         cmd[5];
    uint32_t        cmdLen;
    uint32_t        address = 0;
    uint8_t         cmdWren  = NOR_CMD_WREN;
    NOR_Info       *norOspiInfo;
    OSPI_Handle      spiHandle;
    OSPI_v0_HwAttrs const *hwAttrs;

    if (!handle)
    {
        return NOR_FAIL;
    }

    norOspiInfo = (NOR_Info *)handle;
    if (!norOspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }
    spiHandle = (OSPI_Handle)norOspiInfo->hwHandle;
    hwAttrs = (OSPI_v0_HwAttrs const *)spiHandle->hwAttrs;

    if (NOR_BE_SECTOR_NUM == erLoc)
    {
        cmd[0]  = NOR_CMD_BULK_ERASE;
        cmdLen = 1;
    }
    else
    {
        if (BTRUE == blkErase)
		{
            if (NOR_NUM_BLOCKS <= erLoc)
            {
                return NOR_FAIL;
            }
			address   = erLoc * NOR_BLOCK_SIZE;
            cmd[0] = NOR_CMD_BLOCK_ERASE;
        }
        else
        {
            if (NOR_NUM_SECTORS <= erLoc)
            {
                return NOR_FAIL;
            }
            address   = erLoc * NOR_SECTOR_SIZE;
            cmd[0] = NOR_CMD_SECTOR_ERASE;
        }

        if (BTRUE == hwAttrs->dtrEnable)
        {
            cmd[1] = (address >> 24) & 0xFF; /* 4 address bytes */
            cmd[2] = (address >> 16) & 0xFF;
            cmd[3] = (address >>  8) & 0xFF;
            cmd[4] = (address >>  0) & 0xFF;
            cmdLen = 5U;
        }
        else
        {
            cmd[1] = (address >> 16) & 0xFF;
            cmd[2] = (address >>  8) & 0xFF;
            cmd[3] = (address >>  0) & 0xFF;
            cmdLen = 4U;
        }
    }

    if (Nor_qspiCmdWrite(spiHandle, &cmdWren, 1, 0))
    {
    	return NOR_FAIL;
    }

    if (Nor_qspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    if (Nor_qspiCmdWrite(spiHandle, cmd, cmdLen, 0))
    {
    	return NOR_FAIL;
    }

    if (Nor_qspiWaitReady(spiHandle, NOR_BULK_ERASE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    return NOR_PASS;
}
