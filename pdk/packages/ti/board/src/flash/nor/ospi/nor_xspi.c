/*
 * Copyright (c) 2020-2022, Texas Instruments Incorporated
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

#include <ti/board/src/flash/nor/ospi/nor_xspi.h>
#include <ti/drv/spi/soc/SPI_soc.h>
#include <ti/csl/soc.h>
#include <ti/csl/src/ip/fss/V0/csl_fss.h>

static NOR_HANDLE Nor_xspiOpen(uint32_t norIntf, uint32_t portNum, void *params);
static void Nor_xspiClose(NOR_HANDLE handle);
static NOR_STATUS Nor_xspiRead(NOR_HANDLE handle, uint32_t addr,
                               uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_xspiWrite(NOR_HANDLE handle, uint32_t addr,
                                uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_xspiErase(NOR_HANDLE handle, int32_t eraseCnt, bool blkErase);

static NOR_STATUS Nor_xspiCmdWrite(OSPI_Handle handle, uint8_t *cmdBuf,
                                   uint32_t cmdLen, uint32_t dataLen);
static NOR_STATUS Nor_xspiWaitReady(OSPI_Handle handle, uint32_t timeOut);

/* NOR function table for NOR OSPI interface implementation */
const NOR_FxnTable Nor_xspiFxnTable =
{
    &Nor_xspiOpen,
    &Nor_xspiClose,
    &Nor_xspiRead,
    &Nor_xspiWrite,
    &Nor_xspiErase,
};

NOR_Info Nor_xspiInfo =
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

static bool gPhyEnable;
static bool gDtrEnable;

static NOR_STATUS Nor_xspiCmdRead(OSPI_Handle handle, uint8_t *cmdBuf,
                                  uint32_t cmdLen, uint8_t *rxBuf,
                                  uint32_t rxLen)
{
    OSPI_Transaction transaction;
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

static NOR_STATUS Nor_xspiCmdWrite(OSPI_Handle handle, uint8_t *cmdBuf,
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

static NOR_STATUS Nor_xspiRegRead(OSPI_Handle handle,
                                  uint32_t   regAddr,
                                  uint8_t    *data)
{
    NOR_STATUS retVal;
    uint8_t    cmd[6];

    cmd[0] = NOR_CMD_RDREG;
    cmd[1] = (regAddr >> 24) & 0xFF;
    cmd[2] = (regAddr >> 16) & 0xFF;
    cmd[3] = (regAddr >> 8) & 0xFF;
    cmd[4] = (regAddr) & 0xFF;

    retVal = Nor_xspiCmdRead(handle, cmd, 5, data, 1);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    return retVal;
}

static NOR_STATUS Nor_xspiRegWrite(OSPI_Handle handle,
                                   uint32_t   regAddr,
                                   uint8_t    data)
{
    NOR_STATUS retVal;
    uint8_t    cmd[6];

    /* Enable flash write */
    cmd[0] = NOR_CMD_WREN;
    retVal = Nor_xspiCmdWrite(handle, cmd, 1, 0);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    retVal = Nor_xspiWaitReady(handle, NOR_WRR_WRITE_TIMEOUT);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    cmd[0] = NOR_CMD_WRREG;
    cmd[1] = (regAddr >> 24) & 0xFF;
    cmd[2] = (regAddr >> 16) & 0xFF;
    cmd[3] = (regAddr >> 8) & 0xFF;
    cmd[4] = (regAddr) & 0xFF;
    cmd[5] = data;

    retVal = Nor_xspiCmdWrite(handle, cmd, 5, 1);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    retVal = Nor_xspiWaitReady(handle, NOR_WRR_WRITE_TIMEOUT);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    return retVal;
}

static NOR_STATUS Nor_xspiHybridSectCfg(OSPI_Handle handle,
                                        uint8_t    enable,
                                        uint32_t   cfgFlag)
{
    NOR_STATUS  retVal;
    uint8_t     regData;

    /* Read configuration register3 */
    retVal = Nor_xspiRegRead(handle, NOR_CFG3_VREG_ADDR, &regData);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    if(1 == enable)
    {
        if(0 == (regData & 0x8))
        {
            /* Hybrid sector configuration is already in enabled state.
               No further action needed */
            return NOR_PASS;
        }
        else
        {
            /* Enable hybrid sector configuration */
            regData &= ~0x8;
        }
    }
    else
    {
        if(0 != (regData & 0x8))
        {
            /* Hybrid sector configuration is already in disabled state.
               No further action needed */
            return NOR_PASS;
        }
        else
        {
            /* Disable hybrid sector configuration */
            regData |= 0x8;
        }
    }

    /* Write configuration register3 */
    retVal = Nor_xspiRegWrite(handle, NOR_CFG3_NVREG_ADDR, regData);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    return retVal;
}

static NOR_STATUS Nor_xspiReadId(OSPI_Handle handle)
{
    NOR_STATUS  retVal;
    uint8_t     idCode[NOR_RDID_NUM_BYTES];
    uint8_t     cmd[6];
    uint32_t    manfID, devID;
    uint32_t    cmdLen;
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;

    cmd[0] = NOR_CMD_RDID;
    cmd[1] = 0;  /* Address Bytes */
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;

    if (hwAttrs->xferLines == OSPI_XFER_LINES_OCTAL)
    {
        cmdLen = NOR_RDID_CMD_LENGTH_OCTAL;
    }
    else
    {
        cmdLen = NOR_RDID_CMD_LENGTH_SINGLE;
    }

    retVal = Nor_xspiCmdRead(handle, cmd, cmdLen, idCode, NOR_RDID_NUM_BYTES);
    if (retVal == NOR_PASS)
    {
        manfID = (uint32_t)idCode[0];
        devID = ((uint32_t)idCode[1] << 8) | ((uint32_t)idCode[2]);
        if ((manfID == NOR_MANF_ID) && (devID == NOR_DEVICE_ID))
        {
            Nor_xspiInfo.manufacturerId = manfID;
            Nor_xspiInfo.deviceId = devID;
        }
        else
        {
            retVal = NOR_FAIL;
        }
    }

    return (retVal);
}

static NOR_STATUS Nor_xspiEnableDDR(OSPI_Handle handle)
{
    NOR_STATUS             retVal;
    uint8_t                cmdWren = NOR_CMD_WREN;
    uint32_t               data[6];

    /* Send Write Enable command */
    retVal = Nor_xspiCmdWrite(handle, &cmdWren, 1, 0);

    /* Enable double transfer rate mode */
    if (retVal == NOR_PASS)
    {
        /* send write VCR command to reg addr 0x0 to set to DDR mode */
        data[0] = (NOR_CMD_WRITE_VCR << 24)         | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0x00800006;     /* Non-volatile config register address */
        data[2] = 0x43; /* set to Octal DDR in Nonvolatile Config Reg 0x0 */
        OSPI_control(handle, OSPI_V0_CMD_ENABLE_DDR, (void *)data);
    }

    return retVal;
}

static NOR_STATUS Nor_xspiEnableSDR(OSPI_Handle handle)
{
    NOR_STATUS             retVal;
    uint8_t                cmdWren = NOR_CMD_WREN;
    uint32_t               data[6];
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;
    CSL_ospi_flash_cfgRegs *regAddr;
    uint32_t               regVal;

    /* Send Write Enable command */
    retVal = Nor_xspiCmdWrite(handle, &cmdWren, 1, 0);

    /* Enable double transfer rate mode */
    if (retVal == NOR_PASS)
    {
        /* send write VCR command to reg addr 0x0 to set to DDR mode */
        data[0] = (NOR_CMD_WRITE_VCR << 24)         | /* write volatile config reg cmd */
                  (0 << 23)                         | /* read data disable */
                  (7 << 20)                         | /* read 8 data bytes */
                  (1 << 19)                         | /* enable cmd adddr */
                  (2 << 16)                         | /* 3 address bytes */
                  (1 << 15);                          /* write data enable */
        data[1] = 0x00800006;     /* Non-volatile config register address */
        data[2] = 0x41U; /* set to Extended SPI mode in Nonvolatile Config Reg 0x0 */
        OSPI_control(handle, OSPI_V0_CMD_ENABLE_SDR, (void *)data);

        /* Flash device requires 4-bit access for command as well in quad mode */
        regAddr = (CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr);
        regVal = CSL_REG32_RD(&regAddr->DEV_INSTR_RD_CONFIG_REG);
        regVal |= 0x300;
        CSL_REG32_WR(&regAddr->DEV_INSTR_RD_CONFIG_REG, regVal);
    }

    return NOR_PASS;
}

static NOR_STATUS Nor_xspiSetDummyCycle(OSPI_Handle handle, uint32_t dummyCycle)
{
    NOR_STATUS  retVal;
    uint8_t     regData;

    /* Read configuration register2 */
    retVal = Nor_xspiRegRead(handle, NOR_CFG2_VREG_ADDR, &regData);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    /* Set the dummy cycles */
    regData = (regData & ~0xF) | dummyCycle;

    /* Write configuration register2 */
    retVal = Nor_xspiRegWrite(handle, NOR_CFG2_VREG_ADDR, regData);
    if(NOR_PASS != retVal)
    {
        return NOR_FAIL;
    }

    return retVal;
}

static NOR_STATUS Nor_xspiResetMemory(OSPI_Handle handle)
{
    uint8_t     cmd;

    cmd = NOR_CMD_SRSTE;
    if (Nor_xspiCmdWrite(handle, &cmd, 1, 0))
    {
        return NOR_FAIL;
    }

    cmd = NOR_CMD_SFRST;
    if (Nor_xspiCmdWrite(handle, &cmd, 1, 0))
    {
        return NOR_FAIL;
    }

    return NOR_PASS;
}

static void Nor_ospiSetOpcode(OSPI_Handle handle)
{
    uint32_t               data[6];
    uint32_t               rdDummyCycles = 0U;
    uint32_t               cmdDummyCycles = 0U;
    uint32_t               readCmd;
    uint32_t               progCmd;
    uint32_t               latencyCode = 0U;
    uint32_t               rx_lines;
    OSPI_v0_HwAttrs const *hwAttrs= (OSPI_v0_HwAttrs const *)handle->hwAttrs;

    rx_lines = hwAttrs->xferLines;
    if (OSPI_XFER_LINES_OCTAL == rx_lines)
    {
        if (hwAttrs->dacEnable)
        {
            rdDummyCycles = NOR_OCTAL_READ_DUMMY_CYCLE;
            latencyCode   = NOR_OCTAL_READ_DUMMY_CYCLE_LC;
        }
        else
        {
            rdDummyCycles = NOR_OCTAL_READ_DUMMY_CYCLE_INDAC;
            latencyCode   = NOR_OCTAL_READ_DUMMY_CYCLE_LC_INDAC;
        }

        if (BTRUE == gDtrEnable)
        {
            cmdDummyCycles = NOR_OCTAL_SDR_CMD_READ_DUMMY_CYCLE;
            readCmd = NOR_CMD_OCTAL_DDR_READ;
            if (BTRUE == gPhyEnable)
            {
                rdDummyCycles -= 1U;
            }
        }
        else
        {
            cmdDummyCycles = NOR_OCTAL_SDR_CMD_READ_DUMMY_CYCLE;
            readCmd = NOR_CMD_OCTAL_READ;
        }

        progCmd = NOR_CMD_OCTAL_PROG;
    }
    else
    {
        /* Set to legacy SPI mode 1-1-1 if not Octal mode */
        cmdDummyCycles = NOR_SINGLE_CMD_READ_DUMMY_CYCLE;
        readCmd = NOR_CMD_READ;
        progCmd = NOR_CMD_PAGE_PROG;
    }

    data[0] = readCmd;
    data[1] = progCmd;
    data[2] = NOR_CMD_RDSR;

    /* Update the read opCode, rx lines and read dummy cycles */
    OSPI_control(handle, OSPI_V0_CMD_RD_DUMMY_CLKS, (void *)&rdDummyCycles);
    OSPI_control(handle, OSPI_V0_CMD_SET_XFER_LINES, (void *)&rx_lines);
    OSPI_control(handle, OSPI_V0_CMD_XFER_OPCODE, (void *)data);
    OSPI_control(handle, OSPI_V0_CMD_EXT_RD_DUMMY_CLKS, (void *)&cmdDummyCycles);

    if (rx_lines == OSPI_XFER_LINES_OCTAL)
    {
        /* Set the opcodes for dual opcode mode */
        data[0] = 0;
        data[1] = 0xFA;
        data[2] = readCmd;
        data[3] = progCmd;
        data[4] = 0xF9;
        data[5] = 0x6;
        OSPI_control(handle, OSPI_V0_CMD_XFER_OPCODE_EXT, (void *)data);

        /* Set read dummy cycles to the flash device */
        Nor_xspiSetDummyCycle(handle, latencyCode);
    }

    return;
}

NOR_HANDLE Nor_xspiOpen(uint32_t norIntf, uint32_t portNum, void *params)
{
    OSPI_Params      spiParams;  /* SPI params structure */
    OSPI_Handle      hwHandle;  /* SPI handle */
    NOR_HANDLE      norHandle = 0;
    OSPI_v0_HwAttrs ospiCfg;
    NOR_STATUS      retVal;
    uint32_t        data;
    OSPI_v0_HwAttrs const *hwAttrs;

    /* Get the OSPI SoC configurations */
    OSPI_socGetInitCfg(SPI_OSPI_DOMAIN_MCU, portNum, &ospiCfg);

    /* Save the DTR enable flag */
    gDtrEnable = ospiCfg.dtrEnable;

    /* Reset the PHY tunning configuration data when enabled */
    data = *(uint32_t *)params;
    if (0U != data)
    {
        Nor_spiPhyTuneReset(gDtrEnable);
    }

    /* Save the PHY enable flag */
    gPhyEnable = ospiCfg.phyEnable;
    if (BTRUE == gPhyEnable)
    {
        /*
         * phyEnable is turned on only for DAC read,
         * it turned off for open/erase/write operation
         */
        ospiCfg.phyEnable = BFALSE;
    }

    /* For XSPI flash the read data capture delay needs to be half the value of the baudrate divider */
    if(ospiCfg.baudRateDiv > 0U)
    {
        ospiCfg.rdDataCapDelay = ospiCfg.baudRateDiv/2U;
    }
    else
    {
        ospiCfg.rdDataCapDelay = NOR_XSPI_READ_DELAY_MAX;
    }

    OSPI_socSetInitCfg(SPI_OSPI_DOMAIN_MCU, portNum, &ospiCfg);

    /* Use default SPI config params if no params provided */
    OSPI_Params_init(&spiParams);

    hwHandle = (OSPI_Handle)OSPI_open(SPI_OSPI_DOMAIN_MCU, portNum, &spiParams);

    if (hwHandle)
    {
        retVal = NOR_PASS;
        if (NOR_PASS == retVal)
        {
            OSPI_socGetInitCfg(SPI_OSPI_DOMAIN_MCU, portNum, &ospiCfg);

            if (OSPI_XFER_LINES_OCTAL == ospiCfg.xferLines)
            {
                if (BTRUE == gPhyEnable)
                {
                    Nor_xspiResetMemory(hwHandle);
                }

                if (BTRUE == ospiCfg.dtrEnable)
                {
                    Nor_xspiEnableDDR(hwHandle);
                }
                else
                {
                    Nor_xspiEnableSDR(hwHandle);
                }

                Nor_ospiSetOpcode(hwHandle);
            }
            else
            {
                /* Reset device memory for all the other lines */
                hwAttrs = (OSPI_v0_HwAttrs const *)hwHandle->hwAttrs;
                CSL_ospiSetDualByteOpcodeMode((const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr),
                                              UFALSE);
                Nor_xspiResetMemory(hwHandle);
                Nor_ospiSetOpcode(hwHandle);
                Nor_xspiWaitReady(hwHandle, NOR_WRR_WRITE_TIMEOUT);
                if(NOR_PASS != retVal)
                {
                    return (NOR_HANDLE)NOR_FAIL;
                }
            }

            if (NOR_PASS == Nor_xspiReadId(hwHandle))
            {
                Nor_xspiInfo.hwHandle = (uintptr_t)hwHandle;
                norHandle = (NOR_HANDLE)(&Nor_xspiInfo);
            }
        }

        if (0 == norHandle)
        {
            OSPI_close(hwHandle);
        }
    }

    if(((NOR_HANDLE)0 != norHandle) && (OSPI_XFER_LINES_OCTAL == ospiCfg.xferLines))
    {
        /* Disable hybrid sector configuration */
        Nor_xspiHybridSectCfg(hwHandle, 0, 0);
    }

    return (norHandle);
}

void Nor_xspiClose(NOR_HANDLE handle)
{
    NOR_Info    *norOspiInfo;
    OSPI_Handle   spiHandle;
    OSPI_v0_HwAttrs const        *hwAttrs;
    const CSL_ospi_flash_cfgRegs *pRegs;

    if (handle)
    {
        norOspiInfo = (NOR_Info *)handle;
        spiHandle = (OSPI_Handle)norOspiInfo->hwHandle;

        if (spiHandle)
        {
            /* 
            * Some fields in RD_DATA_CAPTURE_REG are modified by the Nor_spiPhyTune API.
            * These fields need to be reset here to avoid errors in subsequent tests.
            */
            hwAttrs = (OSPI_v0_HwAttrs const *)spiHandle->hwAttrs;
            pRegs = (const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr);
            CSL_REG32_FINS(&pRegs->RD_DATA_CAPTURE_REG,
                            OSPI_FLASH_CFG_RD_DATA_CAPTURE_REG_SAMPLE_EDGE_SEL_FLD,
                            CSL_OSPI_CFG_PHY_SAMPLE_EDGE_DEFAULT);

            CSL_REG32_FINS(&pRegs->RD_DATA_CAPTURE_REG,
                            OSPI_FLASH_CFG_RD_DATA_CAPTURE_REG_DQS_ENABLE_FLD,
                            0);

            OSPI_close(spiHandle);
        }
    }
}

static NOR_STATUS Nor_xspiWaitReady(OSPI_Handle handle, uint32_t timeOut)
{
    uint8_t         status;
    uint8_t         cmd[6];
    uint32_t        cmdLen;
    OSPI_v0_HwAttrs const *hwAttrs = (OSPI_v0_HwAttrs const *)handle->hwAttrs;

    if (OSPI_XFER_LINES_OCTAL == hwAttrs->xferLines)
    {
        cmd[0] = NOR_CMD_RDREG;
        cmd[1] = 0;
        cmd[2] = NOR_VREG_OFFSET;
        cmd[3] = 0;
        cmd[4] = 0;
        cmdLen = 5;
    }
    else
    {
        cmd[0] = NOR_CMD_RDSR;
        cmdLen = 1;
    }

    do
    {
        if (Nor_xspiCmdRead(handle, cmd, cmdLen, &status, 1))
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
NOR_STATUS Nor_xspiRead(NOR_HANDLE handle, uint32_t addr,
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

    OSPI_v0_HwAttrs const        *hwAttrs = (OSPI_v0_HwAttrs const *)spiHandle->hwAttrs;
    const CSL_ospi_flash_cfgRegs *pRegs = (const CSL_ospi_flash_cfgRegs *)(hwAttrs->baseAddr);

    /* Validate address input */
    if (NOR_SIZE < (addr + len))
    {
        return NOR_FAIL;
    }

    if (BTRUE == gPhyEnable)
    {
        CSL_REG32_FINS(&pRegs->DEV_INSTR_RD_CONFIG_REG,
                    OSPI_FLASH_CFG_DEV_INSTR_RD_CONFIG_REG_DUMMY_RD_CLK_CYCLES_FLD,
                    NOR_OCTAL_READ_DUMMY_CYCLE - 1U);
                    
        if (NOR_FAIL == Nor_spiPhyTune(spiHandle, NOR_TUNING_DATA_OFFSET))
        {
           return NOR_FAIL;
        }
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

NOR_STATUS Nor_xspiWrite(NOR_HANDLE handle, uint32_t addr, uint32_t len,
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
    CSL_FssCfg             fssCfg;
    uint32_t         xipPrefetchEnable;
    OSPI_v0_HwAttrs *hwAttrs;
    uint8_t         cmdWren[2];

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

    /* Disable XIP Prefetch before programming flash memory */
    xipPrefetchEnable = UFALSE;
    fssCfg.pFsasRegs = (CSL_fss_fsas_genregsRegs *)CSL_MCU_FSS0_FSAS_CFG_BASE;
    CSL_fssOspiSetXipPrefetchEnable(&fssCfg, CSL_FSS_FSAS_INTERFACE_PATH_SELECT_OSPI0, xipPrefetchEnable);

    cmdWren[0]  = NOR_CMD_WREN;

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
        wrSize = NOR_PAGE_SIZE;
    }
    byteAddr = addr & (wrSize - 1U);

    for (actual = 0U; actual < len; actual += chunkLen)
    {
        if (Nor_xspiCmdWrite(spiHandle, cmdWren, 1, 0))
        {
            return NOR_FAIL;
        }

        if (Nor_xspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
        {
            return NOR_FAIL;
        }

        /* Set the transfer mode, write op code and tx lines */
        OSPI_control(spiHandle, OSPI_V0_CMD_SET_XFER_MODE, NULL);
        OSPI_control(spiHandle, OSPI_V0_CMD_XFER_MODE_RW, (void *)&transferType);

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

        /* Wait till the write operation completes */
        if (Nor_xspiWaitReady(spiHandle, NOR_PAGE_PROG_TIMEOUT))
        {
            return NOR_FAIL;
        }
    }

    /* Enable back XIP prefetch */
    xipPrefetchEnable = UTRUE;
    fssCfg.pFsasRegs = (CSL_fss_fsas_genregsRegs *)CSL_MCU_FSS0_FSAS_CFG_BASE;
    CSL_fssOspiSetXipPrefetchEnable(&fssCfg, CSL_FSS_FSAS_INTERFACE_PATH_SELECT_OSPI0, xipPrefetchEnable);

    return NOR_PASS;
}

NOR_STATUS Nor_xspiErase(NOR_HANDLE handle, int32_t erLoc, bool blkErase)
{
    uint8_t         cmd[10];
    uint32_t        cmdLen;
    uint32_t        address = 0;
    uint8_t         cmdWren[2];
    CSL_FssCfg             fssCfg;
    uint32_t        xipPrefetchEnable;
    NOR_Info        *norOspiInfo;
    OSPI_Handle      spiHandle;

    if (!handle)
    {
        return NOR_FAIL;
    }

    cmdWren[0]  = NOR_CMD_WREN;

    norOspiInfo = (NOR_Info *)handle;
    if (!norOspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }
    spiHandle = (OSPI_Handle)norOspiInfo->hwHandle;

    if (erLoc == NOR_BE_SECTOR_NUM)
    {
        cmd[0] = NOR_CMD_BULK_ERASE;
        cmdLen = 1;
    }
    else
    {
        if (erLoc >= NOR_NUM_BLOCKS)
        {
            return NOR_FAIL;
        }

        address = erLoc * NOR_BLOCK_SIZE;
        cmd[0]  = NOR_CMD_BLOCK_ERASE;

        cmd[1] = (address >> 24) & 0xff; /* 4 address bytes */
        cmd[2] = (address >> 16) & 0xff;
        cmd[3] = (address >>  8) & 0xff;
        cmd[4] = (address >>  0) & 0xff;
        cmdLen = 5;
    }

    /* Disable XIP Prefetch before programming flash memory */
    xipPrefetchEnable = UFALSE;
    fssCfg.pFsasRegs = (CSL_fss_fsas_genregsRegs *)CSL_MCU_FSS0_FSAS_CFG_BASE;
    CSL_fssOspiSetXipPrefetchEnable(&fssCfg, CSL_FSS_FSAS_INTERFACE_PATH_SELECT_OSPI0, xipPrefetchEnable);

    if (Nor_xspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiCmdWrite(spiHandle, cmdWren, 1, 0))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiCmdWrite(spiHandle, cmd, cmdLen, 0))
    {
    	return NOR_FAIL;
    }

    if (Nor_xspiWaitReady(spiHandle, NOR_BULK_ERASE_TIMEOUT))
    {
    	return NOR_FAIL;
    }

    /* Enable back XIP prefetch */
    xipPrefetchEnable = UTRUE;
    fssCfg.pFsasRegs = (CSL_fss_fsas_genregsRegs *)CSL_MCU_FSS0_FSAS_CFG_BASE;
    CSL_fssOspiSetXipPrefetchEnable(&fssCfg, CSL_FSS_FSAS_INTERFACE_PATH_SELECT_OSPI0, xipPrefetchEnable);

    return NOR_PASS;
}
