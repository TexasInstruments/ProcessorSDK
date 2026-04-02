/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#include "board_utils.h"
#include <ti/board/src/flash/nor/qspi/nor_qspi.h>
#include <ti/csl/soc.h>

static NOR_HANDLE Nor_qspiOpen(uint32_t norIntf, uint32_t portNum, void *params);
static void Nor_qspiClose(NOR_HANDLE handle);
static NOR_STATUS Nor_qspiRead(NOR_HANDLE handle, uint32_t addr,
                               uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_qspiWrite(NOR_HANDLE handle, uint32_t addr,
                                uint32_t len, uint8_t *buf, uint32_t mode);
static NOR_STATUS Nor_qspiErase(NOR_HANDLE handle, int32_t erLoc, bool blkErase);
NOR_STATUS Nor_qspiQuadModeCtrl(SPI_Handle handle,
                                uint8_t enable);

/* NOR function table for NOR QSPI interface implementation */
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

static uint32_t gBoardQspiFlashSize = NOR_SIZE;

static NOR_STATUS NOR_qspiCmdRead(SPI_Handle handle, uint8_t *cmdBuf,
                            uint32_t cmdLen, uint8_t *rxBuf, uint32_t rxLen)
{
    QSPI_v1_Object  *object;
    SPI_Transaction  transaction;
    uint32_t         transferType;
    bool             ret;
    unsigned int rxLines;
    unsigned int frmLength;
    unsigned int rxLinesArg;
    unsigned int operMode;

    object = (QSPI_v1_Object *)handle->object;

    operMode = object->qspiMode;
    rxLines  = object->rxLines;

    /* Update the mode and transfer type with the required values */
    SPI_control(handle, SPI_CMD_SETCONFIGMODE, NULL);

    rxLinesArg = QSPI_RX_LINES_SINGLE;
    SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *)&rxLinesArg);

    transaction.txBuf = (void *)cmdBuf;
    transaction.rxBuf = NULL;
    transaction.count = cmdLen;

    frmLength = cmdLen + rxLen;
    SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, (void *)&frmLength);

    transferType = SPI_TRANSACTION_TYPE_WRITE;
    SPI_control(handle, SPI_CMD_TRANSFERMODE_RW, (void *)&transferType);

    ret = SPI_transfer(handle, &transaction);
    if (BTRUE != ret)
    {
        return NOR_FAIL;
    }

    transaction.txBuf = NULL;
    transaction.rxBuf = (void *)rxBuf;
    transaction.count = rxLen;

    transferType = SPI_TRANSACTION_TYPE_READ;
    SPI_control(handle, SPI_CMD_TRANSFERMODE_RW, (void *)&transferType);

    ret = SPI_transfer(handle, &transaction);
    if (BTRUE != ret)
    {
        return NOR_FAIL;
    }

    object->qspiMode = operMode;
    SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *)&rxLines);

    return NOR_PASS;
}

static NOR_STATUS Nor_qspiReadId(SPI_Handle handle)
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

        if (((NOR_MANF_ID == manfID) && (NOR_DEVICE_ID == devID))             ||
            ((NOR_MANF_ID == manfID) && (NOR_DEVICE_ID_GD25B16CSAG == devID)) ||
            ((NOR_MANF_ID_MX25V1635F == manfID) && (NOR_DEVICE_ID_MX25V1635F == devID)))
        {
            Nor_qspiInfo.manufacturerId = manfID;
            Nor_qspiInfo.deviceId = devID;

            if(NOR_DEVICE_ID != devID)
            {
                Nor_qspiInfo.blockCnt = NOR_NUM_BLOCKS_2MBIT_FLASH;
                gBoardQspiFlashSize   = NOR_SIZE_2MBIT_FLASH;
            }
        }
        else
        {
            retVal = NOR_FAIL;
        }
    }

    return (retVal);
}

NOR_HANDLE Nor_qspiOpen(uint32_t norIntf, uint32_t portNum, void *params)
{
    SPI_Params      spiParams;  /* SPI params structure */
    SPI_Handle      hwHandle;  /* SPI handle */
    NOR_HANDLE      norHandle = 0;
    uint8_t         status;
    uint8_t         cmd = NOR_CMD_RDSR_S8_S15;
    unsigned int rxLinesArg;

    /* Init SPI driver */
    SPI_init();

    if (params)
    {
        memcpy(&spiParams, params, sizeof(SPI_Params));
    }
    else
    {
        /* Use default SPI config params if no params provided */
        SPI_Params_init(&spiParams);
    }
    hwHandle = (SPI_Handle)SPI_open(portNum + SPI_CONFIG_OFFSET, &spiParams);

    rxLinesArg = QSPI_RX_LINES_SINGLE;
    SPI_control(hwHandle, SPI_V1_CMD_SETRXLINES, (void *)&rxLinesArg);

    if (hwHandle)
    {
        if (NOR_PASS == Nor_qspiReadId(hwHandle))
        {
            /* Quad enable bit is set by default as needed for RoM boot */
            if (NOR_qspiCmdRead(hwHandle, &cmd, 1, &status, 1))
            {
                return NOR_FAIL;
            }

            if (0U == (status & NOR_SR_QE))
            {
                if (Nor_qspiQuadModeCtrl(hwHandle, 1))
                {
                    return NOR_FAIL;
                }

                rxLinesArg = QSPI_RX_LINES_QUAD;
                SPI_control(hwHandle, SPI_V1_CMD_SETRXLINES, (void *)&rxLinesArg);
            }
            else
            {
                rxLinesArg = QSPI_RX_LINES_QUAD;
                SPI_control(hwHandle, SPI_V1_CMD_SETRXLINES, (void *)&rxLinesArg);                
            }

            Nor_qspiInfo.hwHandle = (uint32_t)hwHandle;
            norHandle = (NOR_HANDLE)(&Nor_qspiInfo);
        }
    }

    return (norHandle);
}

void Nor_qspiClose(NOR_HANDLE handle)
{
    NOR_Info    *norQspiInfo;
    SPI_Handle   spiHandle;

    if (handle)
    {
        norQspiInfo = (NOR_Info *)handle;
        spiHandle = (SPI_Handle)norQspiInfo->hwHandle;

        if (spiHandle)
        {
            SPI_close(spiHandle);
        }
    }
}

static NOR_STATUS Nor_qspiCmdWrite(SPI_Handle handle, uint8_t *cmdBuf,
                                        uint32_t cmdLen, uint32_t dataLen)
{
    QSPI_v1_Object  *object;
    SPI_Transaction  transaction;
    uint32_t         transferType = SPI_TRANSACTION_TYPE_WRITE;
    bool             ret;
    unsigned int operMode;
    unsigned int rxLines;
    unsigned int rxLinesArg;
    unsigned int frmLength;

    object = (QSPI_v1_Object *)handle->object;

    operMode = object->qspiMode;
    rxLines  = object->rxLines;

    /* Update the mode and transfer type with the required values */
    SPI_control(handle, SPI_CMD_SETCONFIGMODE, NULL);
    SPI_control(handle, SPI_CMD_TRANSFERMODE_RW, (void *)&transferType);

    rxLinesArg = QSPI_RX_LINES_SINGLE;
    SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *)&rxLinesArg);

    transaction.txBuf = (void *)cmdBuf; /* Buffer includes command and write data */
    transaction.count = cmdLen + dataLen;
    transaction.rxBuf = NULL;
    transaction.arg = (void *)dataLen;

    /* Total transaction frame length */
    frmLength = transaction.count;
    SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, ((void *)&frmLength));

    ret = SPI_transfer(handle, &transaction);
    if (BTRUE != ret)
    {
        return NOR_FAIL;
    }

    object->qspiMode = operMode;
    SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *)&rxLines);

    return NOR_PASS;
}

static NOR_STATUS Nor_qspiWaitReady(SPI_Handle handle, uint32_t timeOut)
{
    uint8_t         status;
    uint8_t         cmd = NOR_CMD_RDSR_S0_S7;

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

NOR_STATUS Nor_qspiQuadModeCtrl(SPI_Handle handle,
                                uint8_t enable)
{
    uint8_t status;
    uint8_t cmd[3];

    /* Write enable command */
    cmd[0] = NOR_CMD_WREN;
    if (Nor_qspiCmdWrite(handle, cmd, 1, 0))
    {
        goto err;
    }

    /* Read status register */
    cmd[0] = NOR_CMD_RDSR_S8_S15;
    status = 0;
    if (NOR_qspiCmdRead(handle, cmd, 1, &status, 1))
    {
        goto err;
    }

    cmd[0] = NOR_CMD_WRSR_S8_S15;
    if (enable)
    {
        /* quad enabled */
        cmd[1] = status | NOR_SR_QE;
    }
    else
    {
        /* quad disabled */
        cmd[1] = status & (~(NOR_SR_QE));
    }

    if (Nor_qspiCmdWrite(handle, cmd, 1, 1))
    {
        goto err;
    }

    if (Nor_qspiWaitReady(handle, NOR_WRR_WRITE_TIMEOUT))
    {
        goto err;
    }

    cmd[0] = NOR_CMD_RDSR_S8_S15;
    status = 0;
    if (NOR_qspiCmdRead(handle, cmd, 1, &status, 1))
    {
        goto err;
    }

    if (status != cmd[1])
    {
        goto err;
    }

    return NOR_PASS;

err :
    return NOR_FAIL;
}

static SPI_Transaction transaction;
NOR_STATUS Nor_qspiRead(NOR_HANDLE handle, uint32_t addr,
                        uint32_t len, uint8_t *buf, uint32_t mode)
{
    NOR_Info        *norQspiInfo;
    uint32_t         command;
    uint32_t         dummyCycles;
    SPI_Handle       spiHandle;
    bool             ret;
    unsigned int transferType;
    unsigned char dummyWrite[4];    /* dummy data to be written */
    uint32_t         rx_lines;
    uint32_t         rxLinesArg;
    unsigned int frmLength;
    unsigned char writeVal[4];
    QSPI_v1_Object  *object;

    if (!handle)
    {
        return NOR_FAIL;
    }

    norQspiInfo = (NOR_Info *)handle;
    if (!norQspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }
    spiHandle = (SPI_Handle)norQspiInfo->hwHandle;
    object = (QSPI_v1_Object *)spiHandle->object;

    rxLinesArg = object->rxLines;

    /* Validate address input */
    if ((addr + len) > gBoardQspiFlashSize)
    {
        return NOR_FAIL;
    }

    switch(mode)
    {
        case QSPI_FLASH_SINGLE_READ :
            command     = NOR_CMD_READ;
            dummyCycles = NOR_SINGLE_READ_DUMMY_CYCLE;
            rx_lines    = QSPI_IO_LINES_SINGLE;
            break;
        case QSPI_FLASH_DUAL_READ :
            command     = NOR_CMD_DUAL_READ;
            dummyCycles = NOR_DUAL_READ_DUMMY_CYCLE;
            rx_lines    = QSPI_IO_LINES_DUAL;
            break;
        case QSPI_FLASH_QUAD_READ :
            command     = NOR_CMD_QUAD_READ;
            dummyCycles = NOR_QUAD_READ_DUMMY_CYCLE;
            rx_lines    = QSPI_IO_LINES_QUAD;
            break;
        default :
            command     = NOR_CMD_READ;
            dummyCycles = NOR_SINGLE_READ_DUMMY_CYCLE;
            rx_lines    = QSPI_IO_LINES_SINGLE;
            break;
    }

    if(QSPI_OPER_MODE_MMAP == object->qspiMode)
    {
        /* Update the indirect read command, rx lines and read dummy cycles */
        SPI_control(spiHandle, SPI_CMD_SETXFERLINES, (void *)&rx_lines);

        transaction.txBuf = (unsigned char *)addr;
        transaction.rxBuf = buf;
        transaction.count = len;

        transferType = SPI_TRANSACTION_TYPE_READ;
        SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);
        SPI_control(spiHandle, SPI_V1_CMD_MMAP_TRANSFER_CMD, (void *)&command);

        ret = SPI_transfer(spiHandle, &transaction);
        if (BFALSE == ret)
        {
            return NOR_FAIL;
        }
    }
    else
    {
        /* total transaction frame length */
        frmLength = len+1+3;
        SPI_control(spiHandle, SPI_V1_CMD_SETFRAMELENGTH, (void *)&frmLength);

        /* Write read command */
        writeVal[0] = command;
        transaction.txBuf = (unsigned char *)&writeVal[0];
        transaction.rxBuf = NULL;
        transaction.count = 1;

        transferType = SPI_TRANSACTION_TYPE_WRITE;
        SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);
        ret = SPI_transfer(spiHandle, &transaction);

        /* Write Address Bytes */
        writeVal[0] = (addr >> 16) & 0xFF;
        writeVal[1] = (addr >> 8) & 0xFF;
        writeVal[2] = (addr >> 0) & 0xFF;
        transaction.txBuf = (unsigned char *)&writeVal[0];
        transaction.rxBuf = NULL;
        transaction.count = 3;

        transferType = SPI_TRANSACTION_TYPE_WRITE;
        SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);
        ret = SPI_transfer(spiHandle, &transaction);

        if(dummyCycles != 0)
        {
            /* Dummy Byte Write */
            dummyWrite[0] = 0U;
            transaction.txBuf = (unsigned char *)&dummyWrite[0];
            transaction.rxBuf = NULL;
            transaction.count = (dummyCycles >> 3);      /* In bytes */

            transferType = SPI_TRANSACTION_TYPE_WRITE;
            SPI_control(spiHandle, SPI_CMD_TRANSFERMODE_RW, (void *)&transferType);
            ret = SPI_transfer(spiHandle, &transaction);
        }

        /* Update the indirect read command, rx lines and read dummy cycles */
        SPI_control(spiHandle, SPI_CMD_SETXFERLINES, (void *)&rx_lines);

        transferType = SPI_TRANSACTION_TYPE_READ;
        SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);

        transaction.arg   = (void *)addr;
        transaction.txBuf = NULL;
        transaction.rxBuf = (void *)buf;
        transaction.count = len;

        ret = SPI_transfer(spiHandle, &transaction);
        if (BFALSE == ret)
        {
            return NOR_FAIL;
        }
    }

    SPI_control(spiHandle, SPI_V1_CMD_SETRXLINES, (void *)&rxLinesArg);

    return NOR_PASS;
}

NOR_STATUS Nor_qspiWrite(NOR_HANDLE handle, uint32_t addr, uint32_t len,
                         uint8_t *buf, uint32_t mode)
{
    NOR_Info        *norQspiInfo;
    SPI_Handle       spiHandle;
    bool             ret;
    uint8_t          cmdWren = NOR_CMD_WREN;
    uint32_t         command;
    uint32_t         byteAddr;
    uint32_t         pageSize;
    uint32_t         chunkSize;
    uint32_t         chunkLen;
    uint32_t         actual;
    unsigned int frmLength;
    unsigned char writeVal[4];
    unsigned int transferType;
    QSPI_v1_Object  *object;
    QSPI_HwAttrs   *hwAttrs;

    if (!handle)
    {
        return NOR_FAIL;
    }

    norQspiInfo = (NOR_Info *)handle;
    if (!norQspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }
    spiHandle = (SPI_Handle)norQspiInfo->hwHandle;
    object    = (QSPI_v1_Object *)spiHandle->object;
    hwAttrs   = (QSPI_HwAttrs *)(spiHandle->hwAttrs);

    /* Validate address input */
    if ((addr + len) > gBoardQspiFlashSize)
    {
        return NOR_FAIL;
    }

    command = NOR_CMD_PAGE_PROG;

    if(object->qspiMode == QSPI_OPER_MODE_MMAP)
    {
        if (hwAttrs->dmaEnable)
        {
            chunkSize = 32;
        }
        else
        {
            chunkSize = 1;
        }

        for (actual = 0; actual < len; actual += chunkLen)
        {
            /* Send Write Enable command */
            if (Nor_qspiCmdWrite(spiHandle, &cmdWren, 1, 0))
            {
                return NOR_FAIL;
            }

            /* Send Page Program command */
            chunkLen = ((len - actual) < chunkSize ?
                    (len - actual) : chunkSize);

            transaction.txBuf = (unsigned char *)addr;
            transaction.rxBuf = (buf + actual);
            transaction.count = chunkLen;

            transferType = SPI_TRANSACTION_TYPE_WRITE;
            SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);
            SPI_control(spiHandle, SPI_V1_CMD_MMAP_TRANSFER_CMD, (void *)&command);

            ret = SPI_transfer(spiHandle, &transaction);
            if (BFALSE == ret)
            {
                return NOR_FAIL;
            }

            if (Nor_qspiWaitReady(spiHandle, NOR_PAGE_PROG_TIMEOUT)) {
                return NOR_FAIL;
            }

            addr += chunkLen;
            byteAddr = 0;
        }
    }
    else
    {
        /* The QSPI Flash Controller will automatically issue
           the WREN command before triggering a write command via the direct or
           indirect access controllers (DAC/INDAC) – i.e the user does not need
           to perform this operation.
        */
        pageSize    = NOR_PAGE_SIZE;
        byteAddr    = addr & (NOR_PAGE_SIZE - 1); /* % page_size; */

        for (actual = 0; actual < len; actual += chunkLen)
        {
            /* Send Write Enable command */
            if (Nor_qspiCmdWrite(spiHandle, &cmdWren, 1, 0))
            {
                return NOR_FAIL;
            }

            /* Send Page Program command */
            chunkLen = ((len - actual) < (pageSize - byteAddr) ?
                    (len - actual) : (pageSize - byteAddr));

            frmLength = chunkLen + 4;
            SPI_control(spiHandle, SPI_V1_CMD_SETFRAMELENGTH, ((void *)&frmLength));

            /* Send Flash write command */
            writeVal[0] = command;   /* Flash write command */
            transaction.txBuf = (unsigned char *)&writeVal[0];
            transaction.rxBuf = NULL;
            transaction.count = 1U;

            transferType = SPI_TRANSACTION_TYPE_WRITE;
            SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);
            ret = SPI_transfer(spiHandle, &transaction);

            /* Send flash address offset to which data has to be written */
            writeVal[0] = (addr >> 16) & 0xFF;
            writeVal[1] = (addr >> 8) & 0xFF;
            writeVal[2] = (addr >> 0) & 0xFF;
            transaction.txBuf = (unsigned char *)&writeVal[0];
            transaction.rxBuf = NULL;
            transaction.count = 3;

            transferType = SPI_TRANSACTION_TYPE_WRITE;
            SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);
            ret = SPI_transfer(spiHandle, &transaction);

            transaction.arg   = (void *)addr;
            transaction.txBuf = (void *)(buf + actual);
            transaction.rxBuf = NULL;
            transaction.count = chunkLen;

            transferType = SPI_TRANSACTION_TYPE_WRITE;
            SPI_control(spiHandle, SPI_V1_CMD_TRANSFERMODE_RW, (void *)&transferType);

            ret = SPI_transfer(spiHandle, &transaction);
            if (BFALSE == ret)
            {
                return NOR_FAIL;
            }

            if (Nor_qspiWaitReady(spiHandle, NOR_PAGE_PROG_TIMEOUT)) {
                return NOR_FAIL;
            }

            addr += chunkLen;
            byteAddr = 0;
        }
    }

    return NOR_PASS;
}

NOR_STATUS Nor_qspiErase(NOR_HANDLE handle, int32_t erLoc, bool blkErase)
{
    uint8_t         cmd[5];
    uint32_t        cmdLen;
    uint32_t        address = 0;
    uint8_t         cmdWren  = NOR_CMD_WREN;
    NOR_Info       *norQspiInfo;
    SPI_Handle      spiHandle;

    if (!handle)
    {
        return NOR_FAIL;
    }

    norQspiInfo = (NOR_Info *)handle;
    if (!norQspiInfo->hwHandle)
    {
        return NOR_FAIL;
    }

    spiHandle = (SPI_Handle)norQspiInfo->hwHandle;

    if (NOR_BE_SECTOR_NUM == erLoc)
    {
        cmd[0]  = NOR_CMD_BULK_ERASE;
        cmdLen = 1;
    }
    else
    {
        if (BTRUE == blkErase)
        {
            if (erLoc >= Nor_qspiInfo.blockCnt)
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
        cmd[1] = (address >> 16) & 0xFF; /* 64MB flash device */
        cmd[2] = (address >>  8) & 0xFF;
        cmd[3] = (address >>  0) & 0xFF;

        cmdLen = 4U;
    }

    if (Nor_qspiCmdWrite(spiHandle, &cmdWren, 1, 0))
    {
        return NOR_FAIL;
    }

    if (Nor_qspiWaitReady(spiHandle, NOR_WRR_WRITE_TIMEOUT))
    {
        return NOR_FAIL;
    }

    if (Nor_qspiCmdWrite(spiHandle, &cmd[0], cmdLen, 0))
    {
        return NOR_FAIL;
    }

    if (Nor_qspiWaitReady(spiHandle, NOR_BULK_ERASE_TIMEOUT))
    {
        return NOR_FAIL;
    }

    return NOR_PASS;
}
