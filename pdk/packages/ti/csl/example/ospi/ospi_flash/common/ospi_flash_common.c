/**
 *  \file   ospi_flash_common.c
 *
 *  \brief   OSPI Flash common sorce file
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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

#include "ospi_flash_common.h"

uint32_t OspiFlashDevDelays[4] = 
{
    0,                             /* default Chip Select Start of Transfer Delay */
    0,                             /* default Chip Select End of Transfer Delay */
    0,                             /* default Chip Select De-Assert Different Slaves Delay */
    OSPI_FLASH_DEV_DELAY_CSDA      /* default Chip Select De-Assert Delay */
};

static int32_t OspiFlash_waitIdle(uint32_t timeOut, uint32_t idleDelay);
static int32_t OspiFlash_execCmd(const CSL_ospi_flash_cfgRegs *pRegs);
static void OspiFlash_delay(uint32_t delay);
static int32_t OspiFlash_readId(bool intrPollMode);
static uint8_t OspiFlash_getDeviceStatus(uint32_t rdStatusCmd);
static int32_t OspiFlash_cmdRead(uint8_t    *cmd,
                                 uint32_t    cmdLen,
                                 uint8_t    *rxBuf,
                                 uint32_t    rxLen,
                                 uint32_t    dummyCycles);

static int32_t OSPI_waitIndWriteComplete(const CSL_ospi_flash_cfgRegs *pRegs);
static int32_t OSPI_waitWriteSramLvl(const CSL_ospi_flash_cfgRegs *pReg, 
                                     uint32_t *sramLvl);

/*
 *  ======== ConfigOspiClk ========
 */
#if !defined(SOC_AM65XX) /* Use Sciclient API's to configure RCLK*/
int32_t OspiFlash_ospiConfigClk(uint32_t clk)
{
    int32_t status = OSPI_FLASH_APP_STATUS_ERROR;
    uint64_t ospi_rclk_freq;
    uint32_t parClk;

    ospi_rclk_freq = (uint64_t)clk;

    status = Sciclient_pmModuleClkRequest(OSPI_FLASH_DEVICE_ID,
                                          OSPI_FLASH_RCLK_CLK,
                                          TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                          TISCI_MSG_FLAG_AOP,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if(status != CSL_PASS)
    {
	    goto ospi_flash_clk_cfg_exit;
    }   

    /* Max clocks */
    if(OSPI_MODULE_CLK_166M == clk)
    {
        parClk = OSPI_FLASH_PARENT_CLK_166M;
    }
    else
    {
        parClk = OSPI_FLASH_PARENT_CLK_133M;
    }

    status = Sciclient_pmSetModuleClkParent(OSPI_FLASH_DEVICE_ID,
                                            OSPI_FLASH_RCLK_CLK,
                                            parClk,
                                            SCICLIENT_SERVICE_WAIT_FOREVER); 
    if(status != CSL_PASS)
    {
	    goto ospi_flash_clk_cfg_exit;
    }

    status = Sciclient_pmSetModuleClkFreq(OSPI_FLASH_DEVICE_ID,
                                          OSPI_FLASH_RCLK_CLK,
                                          ospi_rclk_freq,
                                          TISCI_MSG_FLAG_AOP,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if(status != CSL_PASS)
    {
	    goto ospi_flash_clk_cfg_exit;
    }

    ospi_rclk_freq = 0;
    status = Sciclient_pmGetModuleClkFreq(OSPI_FLASH_DEVICE_ID,
                                          OSPI_FLASH_RCLK_CLK,
                                          &ospi_rclk_freq,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if(status != CSL_PASS)
    {
	    goto ospi_flash_clk_cfg_exit;
    }

ospi_flash_clk_cfg_exit:
    return (status);
}
#else /* #if !defined(SOC_AM65XX) */
uint32_t MMR_unlock_one(uint32_t * kick0, uint32_t * kick1)
{
    /* initialize the status variable */
    uint32_t status = 1;

    /* if either of the kick lock registers are locked */
    if (!(*kick0 & 0x1) | !(*kick1 & 0x1))
    {
        /* unlock the partition by writing the unlock values to the kick lock registers */
        *kick0 = KICK0_UNLOCK_VAL;
        *kick1 = KICK1_UNLOCK_VAL;
    }

    /* check to see if either of the kick registers are unlocked. */
    if (!(*kick0 & 0x1))
    {
        status = 0;
    }

    /* return the status to the calling program */
    return status;

}

uint32_t MMR_lock_one(uint32_t * kick0, uint32_t * kick1)
{
    /* create status return variable */
    uint32_t status = 1;

    /* check to see if either of the kick registers are unlocked. */
    if ((*kick0 & 0x1))
    {
        /* write the kick lock value to the kick lock registers to lock the partition */
        *kick0 = KICK_LOCK_VAL;
        *kick1 = KICK_LOCK_VAL;
    }

    /* check to see if either of the kick registers are still unlocked. */
    if ((*kick0 & 0x1))
    {
        status = 0;
    }
    /* return success or failure */
    return status;
}

uint32_t MCU_CTRL_MMR_unlock_all()
{

    /* initialize the status variable */
    uint32_t status = 1;

    /* Unlock the 0th partition */
    status &= MMR_unlock_one(
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0),
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK1));

    /* Unlock the 1st partition */
    status &= MMR_unlock_one(
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK0),
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK1));

    /* Unlock the 2nd partition */
    status &= MMR_unlock_one(
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0),
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK1));

    /* Unlock the 3rd partition */
    status &= MMR_unlock_one(
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK0),
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK1));

    /* Unlock the 4th partition */
    status &= MMR_unlock_one(
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK0),
            CSL2PTR ( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK1));
    return status;
}

uint32_t MCU_CTRL_MMR_lock_all()
{

    /* initialize the status variable */
    uint32_t status = 1;

    /* lock the 0th partition */
    status &= MMR_lock_one(
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0),
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK1));

    /* lock the 1st partition */
    status &= MMR_lock_one(
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK0),
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK1));
    /* lock the 2nd partition */
    status &= MMR_lock_one(
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0),
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK1));
    /* lock the 3rd partition */
    status &= MMR_lock_one(
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK0),
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK1));
    /* lock the 4th partition */
    status &= MMR_lock_one(
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK0),
            CSL2PTR (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK1));
    return status;
}

int32_t OspiFlash_ospiConfigClk(uint32_t clk)
{
	uint32_t divider = 0x12;

    MCU_CTRL_MMR_unlock_all();


    if(clk == OSPI_MODULE_CLK_166M)
    {
        /* Select CPSWHSDIV4 */
        *(uint32_t *)(CSL_MCU_CTRL_MMR0_CFG0_BASE + CSL_MCU_CTRL_MMR_CFG0_MCU_OSPI0_CLKSEL) = 1;

        divider = 0xC;   /* 2000/12 = 166 */

        *(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL1_HSDIV_CLKDIV) = \
                (*(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL1_HSDIV_CLKDIV) & 0x00FFFFFF) | ((divider-1) << 24);

        /* Load new values on 0->1 transition of bit 31 of tenabldiv */
        *(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL1_HSDIV_CTRL) &= 0x7FFFFFFF;
        OspiFlash_delay(1);
        *(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL1_HSDIV_CTRL) |= 0x80000000;
    }
    else
    {
        /* Select MCUPLL0HSDIV4 */
        *(uint32_t *)(CSL_MCU_CTRL_MMR0_CFG0_BASE + CSL_MCU_CTRL_MMR_CFG0_MCU_OSPI0_CLKSEL) = 0;

        divider = 18;    /* 2400/18 = 133 */

        *(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL0_HSDIV_CLKDIV) = \
                (*(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL0_HSDIV_CLKDIV) & 0x00FFFFFF) | ((divider-1) << 24);

        /* Load new values on 0->1 transition of bit 31 of tenabldiv */
        *(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL0_HSDIV_CTRL) &= 0x7FFFFFFF;
        OspiFlash_delay(1);
        *(uint32_t*)(CSL_MCU_PLL0_CFG_BASE+CSL_MCU_PLL_MMR_CFG_PLL0_HSDIV_CTRL) |= 0x80000000;
    }
    MCU_CTRL_MMR_lock_all();

    return 0;

}
#endif /* #if !defined(SOC_AM65XX) */

static void OSPI_transferCallback(uintptr_t arg)
{    
    App_OspiObj *ospiObj = (App_OspiObj*)arg;
    /* Indicate transfer complete */
    (void)SemaphoreP_post(ospiObj->ospiFlashTransferDoneSem);
}

static void OSPI_hwiFxn(uintptr_t arg)
{
    uint32_t intrStatus, sramLevel, rdBytes, wrBytes;
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    const uintptr_t dataAddr = (uintptr_t)(OSPI_FLASH_DATA_BASE_ADDR);
    App_OspiObj *ospiObj = (App_OspiObj*)arg;

    /* Read the interrupt status register */
    intrStatus = CSL_ospiIntrStatus(baseAddr);

    if (ospiObj->readBufIdx != NULL_PTR)
    {
		/* Indirect read operation */
        if ((intrStatus & CSL_OSPI_INTR_MASK_IND_XFER) != 0U)
        {
            if(ospiObj->readCountIdx != 0U)
            {
                while ((bool)true)
                {
                    sramLevel = CSL_ospiGetSramLvl(baseAddr, 1U);
                    if (sramLevel == 0U)
                    {
                        break;
                    }
                    rdBytes = sramLevel * CSL_OSPI_FIFO_WIDTH;
                    rdBytes = (rdBytes > ospiObj->readCountIdx) ? ospiObj->readCountIdx : rdBytes;

                    /* Read data from FIFO */
                    CSL_ospiReadFifoData(dataAddr, ospiObj->readBufIdx, rdBytes);

                    ospiObj->readBufIdx += rdBytes;
                    ospiObj->readCountIdx -= rdBytes;
                }

                if ((ospiObj->readCountIdx > 0U) &&
                    (ospiObj->readCountIdx < (CSL_OSPI_SRAM_WARERMARK_RD_LVL * CSL_OSPI_FIFO_WIDTH)))
                {
                    while((bool)true)
                    {
                        sramLevel = CSL_ospiGetSramLvl(baseAddr, 1U);
                        rdBytes = sramLevel * CSL_OSPI_FIFO_WIDTH;
                        if (rdBytes >= ospiObj->readCountIdx)
                        {
                            break;
                        }
                    }
                    rdBytes = ospiObj->readCountIdx;
                    CSL_ospiReadFifoData(dataAddr, ospiObj->readBufIdx, rdBytes);
                    ospiObj->readBufIdx += rdBytes;
                    ospiObj->readCountIdx -= rdBytes;
                }
            }

            if((ospiObj->readCountIdx == 0U) || ((intrStatus & CSL_OSPI_INTR_MASK_IND_OP_DONE) != 0U))
            {
                /* Clear indirect read operation complete status */
                CSL_ospiClrIndReadComplete(baseAddr);

                /* disable and clear the interrupts */
                CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL, FALSE);
                CSL_ospiIntrClear(baseAddr, CSL_OSPI_INTR_MASK_ALL);

                /* Call the call back function */
                OSPI_transferCallback(arg);
            }
            else
            {
                /* Clear interrupt status */
                CSL_ospiIntrClear(baseAddr, intrStatus);
            }
        }
        else
        {
            /* Clear interrupt status */
            CSL_ospiIntrClear(baseAddr, intrStatus);
        }
    }
    else if(ospiObj->writeBufIdx != NULL_PTR)
    {
		/* Indirect write operation */
        if ((intrStatus & CSL_OSPI_INTR_MASK_IND_XFER) != 0U)
        {
            if (ospiObj->writeCountIdx != 0U)
            {
                sramLevel = CSL_OSPI_SRAM_PARTITION_WR - CSL_ospiGetSramLvl(baseAddr, 0U);

                wrBytes = sramLevel * CSL_OSPI_FIFO_WIDTH;
                wrBytes = (wrBytes > ospiObj->writeCountIdx) ? ospiObj->writeCountIdx : wrBytes;

                /* Write data to FIFO */
                CSL_ospiWriteFifoData(dataAddr, ospiObj->writeBufIdx, wrBytes);

                ospiObj->writeBufIdx += wrBytes;
                ospiObj->writeCountIdx -= wrBytes;

                sramLevel = CSL_OSPI_SRAM_PARTITION_WR - CSL_ospiGetSramLvl(baseAddr, 0U);

                if ((ospiObj->writeCountIdx > 0U) &&
                    (ospiObj->writeCountIdx <= (sramLevel * CSL_OSPI_FIFO_WIDTH)))
                {
                    wrBytes = ospiObj->writeCountIdx;
                    CSL_ospiWriteFifoData(dataAddr, ospiObj->writeBufIdx, wrBytes);
                    ospiObj->writeBufIdx += wrBytes;
                    ospiObj->writeCountIdx -= wrBytes;
                }
            }

            if ((intrStatus & CSL_OSPI_INTR_MASK_IND_OP_DONE) != 0U)
            {
                /* Clear indirect write operation complete status */
                CSL_ospiClrIndWriteComplete(baseAddr);

                /* disable and clear the interrupts */
                CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL, FALSE);
                CSL_ospiIntrClear(baseAddr, CSL_OSPI_INTR_MASK_ALL);

                /* Call the call back function */
                OSPI_transferCallback(arg);
            }
            else
            {
                /* Clear interrupt status */
                CSL_ospiIntrClear(baseAddr, intrStatus);
            }
        }
        else
        {
            /* Clear interrupt status */
            CSL_ospiIntrClear(baseAddr, intrStatus);
        }
    }
    else
    {
        /* This was neither a write or read ISR, just clearing the interrupt */
        CSL_ospiIntrClear(baseAddr, intrStatus);
    }
}

#if defined(FLASH_TYPE_XSPI)
static int32_t OspiFlash_xspiRegRead(bool intrPollMode, uint32_t regAddr, uint8_t *data)
{
    int32_t     retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint8_t     cmd[6];

    cmd[0] = OSPI_FLASH_CMD_RDREG;
    cmd[1] = (regAddr >> 24) & 0xFF;
    cmd[2] = (regAddr >> 16) & 0xFF;
    cmd[3] = (regAddr >> 8) & 0xFF;
    cmd[4] = (regAddr) & 0xFF;

    retVal = OspiFlash_cmdRead(cmd, 5, data, 1, OSPI_FLASH_NOR_OCTAL_DDR_CMD_READ_DUMMY_CYCLE);

    return retVal;
}

static int32_t OspiFlash_xspiRegWrite(bool intrPollMode, uint32_t regAddr, uint8_t data)
{
    int32_t     retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint8_t     cmd[6];

    /* Enable flash write */
    cmd[0] = OSPI_FLASH_CMD_WREN;
    retVal += OspiFlash_cmdWrite(cmd, 1, NULL, 0, intrPollMode);

    retVal += OspiFlash_waitReady(OSPI_FLASH_WRR_WRITE_TIMEOUT, intrPollMode);

    cmd[0] = OSPI_FLASH_CMD_WRREG;
    cmd[1] = (regAddr >> 24) & 0xFF;
    cmd[2] = (regAddr >> 16) & 0xFF;
    cmd[3] = (regAddr >> 8) & 0xFF;
    cmd[4] = (regAddr) & 0xFF;
    cmd[5] = data;

    retVal += OspiFlash_cmdWrite(cmd, 5, &cmd[5], 1, intrPollMode);

    retVal += OspiFlash_waitReady(OSPI_FLASH_WRR_WRITE_TIMEOUT, intrPollMode);

    return retVal;
}

static int32_t OspiFlash_xspiHybridSectCfg(bool intrPollMode, uint8_t enable, uint32_t cfgFlag)
{
    int32_t     retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint8_t     regData;

    /* Read configuration register3 */
    retVal += OspiFlash_xspiRegRead(intrPollMode, OSPI_FLASH_NOR_CFG3_VREG_ADDR, &regData);

    if(enable == 1)
    {
        /* Enable hybrid sector configuration */
        regData &= ~0x8;
    }
    else
    {
        /* Disable hybrid sector configuration */
        regData |= 0x8;  
    }

    /* Write configuration register3 */
    retVal += OspiFlash_xspiRegWrite(intrPollMode, OSPI_FLASH_NOR_CFG3_NVREG_ADDR, regData);

    return retVal;
}

static int32_t OspiFlash_xspiSetDummyCycle(uint32_t dummyCycle)
{
    int32_t     retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint8_t     regData;

    /* Read configuration register2 */
    retVal += OspiFlash_xspiRegRead(FALSE, OSPI_FLASH_NOR_CFG2_VREG_ADDR, &regData);

    /* Set the dummy cycles */
    regData = (regData & ~0xF) | dummyCycle;

    /* Write configuration register2 */
    retVal += OspiFlash_xspiRegWrite(FALSE, OSPI_FLASH_NOR_CFG2_VREG_ADDR, regData);

    return retVal;
}
#endif

static int32_t OSPI_waitWriteSramLvl(const CSL_ospi_flash_cfgRegs *pReg, uint32_t *sramLvl)
{
    uint32_t retry = CSL_OSPI_REG_RETRY;
    uint32_t sramLevel;
    int32_t  retVal = OSPI_FLASH_APP_STATUS_SUCCESS;

    while(retry != 0U)
    {
        sramLevel = CSL_ospiGetSramLvl(pReg, 0);
        if (sramLevel <= CSL_OSPI_SRAM_WATERMARK_WR_LVL)
        {
			*sramLvl = sramLevel;
            break;
        }
        (void)Osal_delay(CSL_OSPI_POLL_IDLE_DELAY);
        retry--;
    }

    if (retry != 0U)
    {
        retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    }
    else
    {
        retVal = OSPI_FLASH_APP_STATUS_ERROR;
    }
    return(retVal);
}

static int32_t OSPI_waitIndWriteComplete(const CSL_ospi_flash_cfgRegs *pRegs)
{
    uint32_t retry  = OSPI_FLASH_WRITE_TIMEOUT;
    int32_t  retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    volatile uint32_t   delay;

    /* Check flash indirect write controller status */
    while (retry != 0U)
    {
        if (CSL_ospiIsIndWriteComplete(pRegs) == TRUE)
        {
            break;
        }
        delay = OSPI_FLASH_CHECK_IDLE_DELAY;
        while (delay > 0U)
            delay = delay - 1U;
        retry--;
    }

    if (retry != 0U)
    {
        /* Clear indirect completion status */
        CSL_ospiClrIndWriteComplete(pRegs);
        retVal = OSPI_FLASH_APP_STATUS_SUCCESS;
    }
    else
    {
        retVal = OSPI_FLASH_APP_STATUS_ERROR;
    }
    return(retVal);
}

int32_t OspiFlash_xspiIndacWrite(App_OspiObj *ospiObj, uint8_t *buf, uint32_t numBytes, bool intrPollMode, uint32_t offset) /* based on OSPI_ind_xfer_mode_write_v0 in OSPI_v0.c */
{
    int32_t             status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint8_t             *pSrc;          /* Source address */
    uint32_t            size;
    uint32_t            remainSize;
    uint32_t            totalSize;
    uint32_t            wrByte;
    uint32_t            sramLevel;
    uint32_t            blkNumOffset;
    uint8_t             cmdWren  = OSPI_FLASH_CMD_WREN;

    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    pSrc = (uint8_t *)buf;
    remainSize = 0;
    totalSize = (uint32_t)numBytes;
    blkNumOffset = (uint32_t)(offset/OSPI_FLASH_DEVICE_BLOCK_SIZE);

    /* Erase the region before writing */
    OspiFlash_ospiEraseBlk(blkNumOffset, totalSize, FALSE);

    while(totalSize>0)
    {
        size = totalSize < OSPI_FLASH_DEVICE_PAGE_SIZE ? totalSize : OSPI_FLASH_DEVICE_PAGE_SIZE;

        OspiFlash_ospiXferIntrInit(intrPollMode);

        OspiFlash_cmdWrite((const uint8_t *)&cmdWren, 1, NULL, 0, intrPollMode);
        OspiFlash_waitReady(OSPI_FLASH_WRR_WRITE_TIMEOUT, intrPollMode);

        /* Disable DAC mode */
        CSL_ospiDacEnable(baseAddr, FALSE);
        /* Set write address in indirect mode */
        CSL_ospiIndSetStartAddr(baseAddr, (uint32_t)offset, FALSE);

        if (intrPollMode)                /* if interrupt mode is used then OSPI_hwiFxn will handle, if poll mode is used the below code will handle */   
        {
            /* start the indirect write */
            CSL_ospiIndWriteExecute(baseAddr, size);

            remainSize = size;

            while(remainSize > 0U)
            {
                /* Wait indirect write SRAM fifo level below watermark */
                status = OSPI_waitWriteSramLvl(baseAddr,&sramLevel);

                wrByte = (CSL_OSPI_SRAM_PARTITION_WR - sramLevel) * CSL_OSPI_FIFO_WIDTH;        /* check if Macro is SoC specific */
                wrByte = (wrByte > remainSize) ? remainSize : wrByte;

                /* Write data to FIFO */
                CSL_ospiWriteFifoData((uintptr_t)(OSPI_FLASH_DATA_BASE_ADDR), pSrc, wrByte);   /* OSPI_FLASH_DATA_BASE_ADDR = MCU_FSS0_DAT_REG1 = 0x0050000000 */

                pSrc += wrByte;
                remainSize -= wrByte;
            }
            if(status == OSPI_FLASH_APP_STATUS_SUCCESS)
            {
                status = OSPI_waitIndWriteComplete(baseAddr);
            }
            if(status == OSPI_FLASH_APP_STATUS_ERROR)
            {
                CSL_ospiIndWriteCancel(baseAddr);           
            }
        }
        else
        {
            /* Book keeping of transmit and receive buffers */
            ospiObj->writeBufIdx = (uint8_t *)pSrc;
            ospiObj->writeCountIdx = (uint32_t)size;
            ospiObj->readBufIdx =  NULL_PTR;
            ospiObj->readCountIdx = 0; 

            /* start the indirect write */
            CSL_ospiIndWriteExecute(baseAddr, size);

            /* To wait till write completes in interrupt mode? */
            SemaphoreP_pend(ospiObj->ospiFlashTransferDoneSem, SemaphoreP_WAIT_FOREVER);
        }

        OspiFlash_waitReady(OSPI_FLASH_PAGE_PROG_TIMEOUT, FALSE);

        pSrc += size;
        offset += size;
        totalSize -= size;
    }

    return status;
}

int32_t OspiFlash_ospiOpen(App_OspiObj *ospiObj, uint32_t timeOut, uint32_t idleDelay, bool dacMode, bool intrPollMode)
{
    int32_t status = OSPI_FLASH_APP_STATUS_SUCCESS;
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    SemaphoreP_Params        semParams;
	OsalRegisterIntrParams_t interruptRegParams;
    int32_t eventId = 0;                    /* this was 0 in OSPI_v0_HwAttrs ospiInitCfg */
    int32_t intrNum = OSPI_FLASH_INTR_NUM;  /* =CSLR_R5FSS0_CORE0_INTR_FSS0_OSPI_0_OSPI_LVL_INTR_0 */
    uintptr_t arg = (uintptr_t)ospiObj;
    uintptr_t key;

    key = HwiP_disable();
    HwiP_restore(key);

    ospiObj->writeBufIdx = NULL_PTR;
    ospiObj->writeCountIdx = 0;
    ospiObj->readBufIdx =  NULL_PTR;
    ospiObj->readCountIdx = 0;

    /* If supporting interrupt mode, register and enable interrupts */
    if(FALSE == intrPollMode)
    {
        if(FALSE == dacMode)
        {
            Osal_RegisterInterrupt_initParams(&interruptRegParams);

            interruptRegParams.corepacConfig.name=NULL;
    #if ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'R'))
            interruptRegParams.corepacConfig.priority=0x20U;
    #else
            interruptRegParams.corepacConfig.priority=0x8U;
    #endif
            interruptRegParams.corepacConfig.corepacEventNum = (int32_t)eventId;
            interruptRegParams.corepacConfig.intVecNum = (int32_t)intrNum; /* Host Interrupt vector */
            interruptRegParams.corepacConfig.isrRoutine = (void (*)(uintptr_t))(&OSPI_hwiFxn);
            interruptRegParams.corepacConfig.arg = (uintptr_t)arg;

            (void)Osal_RegisterInterrupt(&interruptRegParams,&(ospiObj->ospiFlashInterruptHandle));
            
            if(ospiObj->ospiFlashInterruptHandle == NULL)
            {
                status = OSPI_FLASH_APP_STATUS_ERROR;
                /* disable the interrupts */
                CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL,FALSE);
            }
            else
            {
                SemaphoreP_Params_init(&semParams);
                semParams.mode = SemaphoreP_Mode_BINARY;
                ospiObj->ospiFlashTransferDoneSem = SemaphoreP_create(0, &semParams);
            }
        }
        else
        {
            SemaphoreP_Params_init(&semParams);
            semParams.mode = SemaphoreP_Mode_BINARY;
            ospiObj->ospiFlashTransferDoneSem = SemaphoreP_create(0, &semParams);
        }
    }

    if(OSPI_FLASH_APP_STATUS_SUCCESS == status)
    {
        /* Wait until Serial Interface and OSPI pipeline is IDLE. */
        status = OspiFlash_waitIdle(timeOut, idleDelay);

        /* Disable DAC */
        CSL_ospiDacEnable(baseAddr, FALSE);

        /* Disable DTR protocol */
        CSL_ospiDtrEnable(baseAddr, FALSE);

        /* Disable XIP */
        CSL_ospiXipEnable(baseAddr, FALSE);
        
        /* Disable OSPI controller */
        CSL_ospiEnable(baseAddr, FALSE);

        if(OSPI_FLASH_APP_STATUS_SUCCESS != status)
        {
            /* disable the interrupts */
            CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL, FALSE);
        }
        else
        {
            /* Chip select setup */
            CSL_ospiSetChipSelect(baseAddr, CSL_OSPI_CS0, CSL_OSPI_DECODER_SELECT4);

            /* Set clock mode */
            CSL_ospiSetClkMode(baseAddr, CSL_OSPI_CLK_MODE_0);

            /* Disable the adapted loopback clock circuit */
            CSL_ospiLoopbackClkEnable(baseAddr,FALSE);

            if (ospiObj->clk == OSPI_MODULE_CLK_133M)
            {
                OspiFlashDevDelays[3] = OSPI_FLASH_DEV_DELAY_CSDA_2;
            }
            else
            {
                OspiFlashDevDelays[3] = OSPI_FLASH_DEV_DELAY_CSDA_3;
            }

            /* Delay Setup */
            CSL_ospiSetDevDelay(baseAddr, OspiFlashDevDelays);

            /* Set default baud rate divider value */
            CSL_ospiSetPreScaler(baseAddr, CSL_OSPI_BAUD_RATE_DIVISOR(4U));

            if(FALSE == dacMode)       
            {
                /* Disable PHY mode */
                CSL_ospiPhyEnable(baseAddr, FALSE);
                /* Disable PHY pipeline mode */
                CSL_ospiPipelinePhyEnable(baseAddr, FALSE);
            }

            /* Set device size cofigurations */
            /* TO DO : If supporting Legacy SPI mode, the following should be based on dtrEnable.
            * ie, if Legacy SPI mode -> dtrEnable will be false -> use CSL_OSPI_MEM_MAP_NUM_ADDR_BYTES_3 */
            CSL_ospiSetDevSize(baseAddr,
                            CSL_OSPI_MEM_MAP_NUM_ADDR_BYTES_4,
                            OSPI_FLASH_DEVICE_PAGE_SIZE,
                            log2(OSPI_FLASH_DEVICE_BLOCK_SIZE)); 
            
            /* Set indirect trigger address register */
            if(dacMode)                       
            {
                CSL_ospiSetIndTrigAddr(baseAddr, 0x4000000);
            }
            else                                            
            {
                CSL_ospiSetIndTrigAddr(baseAddr, 0x0000000);
            }

            /* Disable write completion auto polling */
            CSL_ospiSetWrCompAutoPolling(baseAddr, CSL_OSPI_WRITE_COMP_AUTO_POLLING_DISABLE);

            /* Set SRAM partition configuration */
            CSL_ospiSetSramPartition(baseAddr, CSL_OSPI_SRAM_PARTITION_DEFAULT);

            /* disable and clear the interrupts */
            CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL, FALSE);
            CSL_ospiIntrClear(baseAddr, CSL_OSPI_INTR_MASK_ALL);

            /* disable or enable DAC based on boll dacMode field */
            CSL_ospiDacEnable(baseAddr, dacMode);
            
            CSL_ospiSetDataReadCapDelay(baseAddr, OSPI_FLASH_RD_DATA_CAP_DELAY);

            CSL_ospiSetCsSotDelay(baseAddr, OSPI_FLASH_CS_SOT_DELAY);

            /* Enable OSPI controller */
            CSL_ospiEnable(baseAddr, TRUE);
                    
            #if defined(FLASH_TYPE_XSPI)
            /* Disable hybrid sector configuration */
            status = OspiFlash_xspiHybridSectCfg(intrPollMode, 0, 0);
            #endif
        }
    }
    return (status);
}

int32_t OspiFlash_ospiEnableDDR(bool dacMode, bool intrPollMode)
{
    int32_t status = OSPI_FLASH_APP_STATUS_ERROR;
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    uint8_t cmdWren = OSPI_FLASH_CMD_WREN;
    uint32_t cmd, addr, data;

    /* --------------------------
     * Send Write Enable command 
     * --------------------------*/
    status = OspiFlash_cmdWrite((const uint8_t *)&cmdWren, 1, NULL, 0, intrPollMode);

    /* ---------------------------------
     * Enable double transfer rate mode
     * ---------------------------------*/
    if(OSPI_FLASH_APP_STATUS_SUCCESS == status)
    {
        /* send write VCR command to reg addr 0x0 to set to DDR mode */
        cmd = (OSPI_FLASH_NOR_CMD_WRITE_VCR << 24)   |  /* write volatile config reg cmd */
              (0 << 23)                              |  /* read data disable */
              (7 << 20)                              |  /* read 8 data bytes */
              (1 << 19)                              |  /* enable cmd adddr */
              (2 << 16)                              |  /* 3 address bytes */
              (1 << 15);                                /* write data enable */
        addr = OSPI_FLASH_NOR_CFG5_VREG_ADDR;           /* Non-volatile config register address */
        data = OSPI_FLASH_OCTAL_DDR;                    /* set to Octal DDR in Nonvolatile Config Reg */

        CSL_ospiFlashStig(baseAddr, cmd, addr, data);

        status = OspiFlash_execCmd(baseAddr);
        
        if(OSPI_FLASH_APP_STATUS_SUCCESS == status)
        {
            /* disable or enable DAC based on boll dacMode field */
            CSL_ospiDacEnable(baseAddr, dacMode);
            /* Enable DTR protocol */
            CSL_ospiDtrEnable(baseAddr, TRUE);
        }
    }
    
    return (status);
}

int32_t OspiFlash_ospiSetOpcode(bool dacMode)
{
    int32_t     status = OSPI_FLASH_APP_STATUS_SUCCESS;
    const       CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    uint32_t    rdDummyCycles;

    if(dacMode)
    {
        rdDummyCycles = OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_DAC;
    }
    else
    {
        rdDummyCycles = OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_INDAC + 1;  /* add 1 if not using PHY */
    }
    /* -------------------------------------------
     * Set read/write opcode and read dummy cycles
     * -------------------------------------------*/

    /* Set device size cofigurations */
    /* TO DO : If supporting Legacy SPI mode, the following should be based on dtrEnable.
     * ie, if Legacy SPI mode -> dtrEnable will be false -> use CSL_OSPI_MEM_MAP_NUM_ADDR_BYTES_3 */
    CSL_ospiSetDevSize(baseAddr,
                       CSL_OSPI_MEM_MAP_NUM_ADDR_BYTES_4,
                       OSPI_FLASH_DEVICE_PAGE_SIZE,
                       log2(OSPI_FLASH_DEVICE_BLOCK_SIZE)); 
    
    /* TO DO : If supporting Legacy SPI mode, the following should be based on dtrEnable.
     * ie, if Legacy SPI mode -> dtrEnable will be false -> use NOR_CMD_OCTAL_IO_FAST_RD */
    CSL_ospiConfigRead(baseAddr,
                       OSPI_FLASH_CMD_OCTAL_DDR_O_FAST_RD,
                       OSPI_FLASH_XFER_LINES_OCTAL,
                       rdDummyCycles); 

     /* TO DO : If supporting Legacy SPI mode, the following should be based on dtrEnable.
     * ie, if Legacy SPI mode -> dtrEnable will be false -> use NOR_CMD_EXT_OCTAL_FAST_PROG */
    CSL_ospiWriteSetup(baseAddr,
                       OSPI_FLASH_CMD_OCTAL_FAST_PROG,
                       OSPI_FLASH_XFER_LINES_OCTAL);

    #if defined(FLASH_TYPE_XSPI)                          /* dual opcode mode required for nor xspi chip - try using macro for xspi */
    uint32_t    data[6];
    uint32_t    *ctrlData = data;
    uint32_t    extOpcodeLo;
    uint32_t    extOpcodeUp;
    uint32_t    latencyCode;
    
    if(dacMode)
    {
        latencyCode   = OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_LC_DAC;
    }
    else
    {
        latencyCode   = OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_LC_INDAC;
    }

    /* Set the opcodes for dual opcode mode */
    data[0] = 0;
    data[1] = 0xFA;
    data[2] = OSPI_FLASH_CMD_OCTAL_DDR_O_FAST_RD;   /* readCmd */
    data[3] = OSPI_FLASH_CMD_OCTAL_FAST_PROG;       /* progCmd */
    data[4] = 0xF9;
    data[5] = 0x6;

    extOpcodeLo  = *ctrlData++;         /* EXT_STIG_OPCODE_FLD */
    extOpcodeLo |= (*ctrlData++ << 8);  /* EXT_POLL_OPCODE_FLD */
    extOpcodeLo |= (*ctrlData++ << 16); /* EXT_WRITE_OPCODE_FLD */
    extOpcodeLo |= (*ctrlData++ << 24); /* EXT_READ_OPCODE_FLD */

    extOpcodeUp  = (*ctrlData++ << 16); /* EXT_WEL_OPCODE_FLD */
    extOpcodeUp |= (*ctrlData << 24);   /* WEL_OPCODE_FLD */

    CSL_ospiExtOpcodeSet(baseAddr, extOpcodeLo, extOpcodeUp);
    CSL_ospiSetDualByteOpcodeMode(baseAddr, TRUE);

    OspiFlash_xspiSetDummyCycle(latencyCode);
    #endif

    return (status);
}

int32_t OspiFlash_ospiConfigPHY(uint32_t clk, bool intrPollMode)
{
    int32_t         status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint32_t        numDelayElems;
    uint32_t        delay;
    uint32_t        i = 0;
    uint32_t        readCnt = 0;
    uint32_t        readStart = 0;
    uint32_t        readCntPrv = 0;
    uint32_t        readStartPrv = 0;

    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    numDelayElems = (1000000U / (clk / 1000000U)) / OSPI_FLASH_PHY_DLL_ELEM_DELAY_PERIOD;

    /* -------------------------
     * set initial PHY DLL delay
     * -------------------------*/
    delay = 0U;
    CSL_ospiConfigPhy(baseAddr,
                      CSL_OSPI_CFG_PHY_DLL_MODE_DEFAULT,
                      numDelayElems,
                      (const uint32_t *)&delay);

    OspiFlash_delay(OSPI_FLASH_CALIBRATE_DELAY);

    /* -------------
     * calibrate PHY
     * -------------*/
    for (i = 0; i < 128U; i++)
    {
        if(OspiFlash_readId(intrPollMode) == OSPI_FLASH_APP_STATUS_SUCCESS)
        {
            /* Iterate flash reads, find the start index and successful read ID count */
            if(readCnt == 0)
                readStart = i;
            readCnt++;
        }
        else
        {
            if((readCnt != 0) && (readCnt > readCntPrv))
            {
                /* save the start index and most successful read ID count */
                readCntPrv = readCnt;
                readStartPrv = readStart;
                readCnt = 0;
                readStart = 0;
            }
        }

        /* Increment DLL delay */
        CSL_ospiConfigPhy(baseAddr,
                          CSL_OSPI_CFG_PHY_DLL_MODE_DEFAULT,
                          numDelayElems,
                          NULL);
                      
        OspiFlash_delay(OSPI_FLASH_CALIBRATE_DELAY);
    }

    if(readCnt > readCntPrv)
    {
        readCntPrv = readCnt;
        readStartPrv = readStart;
    }

    if(readCntPrv != 0U)
    {
        /* Find the delay in the middle working position */
        delay = readStartPrv + (readCntPrv / 2);
        CSL_ospiConfigPhy(baseAddr,
                          CSL_OSPI_CFG_PHY_DLL_MODE_DEFAULT,
                          numDelayElems,
                          (const uint32_t *)&delay);
                      
        OspiFlash_delay(OSPI_FLASH_CALIBRATE_DELAY);
    }
    else
    {
        status = OSPI_FLASH_APP_STATUS_ERROR;
    }

    return (status);
}

int32_t OspiFlash_ospiEraseBlk(uint32_t blkNumOffset, uint32_t testLen, bool intrPollMode)
{
    int32_t         status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint32_t        blockNum;     /* flash block number */
    uint32_t        i;
    uint32_t        address = 0;
    uint8_t         cmd[5];
    uint32_t        cmdLen;
    uint8_t         cmdWren  = OSPI_FLASH_CMD_WREN;
    
    for (i = 0; i < testLen; i += OSPI_FLASH_DEVICE_BLOCK_SIZE)
    {
        blockNum = i/OSPI_FLASH_DEVICE_BLOCK_SIZE + blkNumOffset;

        address   = blockNum * OSPI_FLASH_DEVICE_BLOCK_SIZE;
        cmd[0] = OSPI_FLASH_CMD_BLOCK_ERASE;
        /* TO DO : If supporting Legacy SPI mode, the following should be based on dtrEnable.
         * ie, if Legacy SPI mode -> dtrEnable will be false -> use 3 address bytes */
        cmd[1] = (address >> 24) & 0xff; /* 4 address bytes */
        cmd[2] = (address >> 16) & 0xff;
        cmd[3] = (address >>  8) & 0xff;
        cmd[4] = (address >>  0) & 0xff;
        cmdLen = 5;
        
        #if defined(FLASH_TYPE_XSPI)
        status += OspiFlash_waitReady(OSPI_FLASH_WRR_WRITE_TIMEOUT, intrPollMode);
        #endif
        status += OspiFlash_cmdWrite((const uint8_t *)&cmdWren, 1, NULL, 0, intrPollMode);

        status += OspiFlash_waitReady(OSPI_FLASH_WRR_WRITE_TIMEOUT, intrPollMode);

        status += OspiFlash_cmdWrite((const uint8_t *)cmd, cmdLen, NULL, 0, intrPollMode);

        status += OspiFlash_waitReady(OSPI_FLASH_BULK_ERASE_TIMEOUT, intrPollMode);
    }

    return (status);
}

void OspiFlash_ospiClose(App_OspiObj *ospiObj, bool intrPollMode)
{
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    /* disable the interrupts */
    CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL,FALSE);

    CSL_REG32_FINS(&baseAddr->RD_DATA_CAPTURE_REG,
                    OSPI_FLASH_CFG_RD_DATA_CAPTURE_REG_SAMPLE_EDGE_SEL_FLD,
                    0);

    CSL_REG32_FINS(&baseAddr->RD_DATA_CAPTURE_REG,
                    OSPI_FLASH_CFG_RD_DATA_CAPTURE_REG_DQS_ENABLE_FLD,
                    0);

    if(FALSE == intrPollMode)
    {
        if(ospiObj->ospiFlashInterruptHandle != NULL)
        {
            Osal_DeleteInterrupt(ospiObj->ospiFlashInterruptHandle, 0);
            ospiObj->ospiFlashInterruptHandle = NULL;
        }
        (void)SemaphoreP_delete(ospiObj->ospiFlashTransferDoneSem);
    }

    return;
}

static int32_t OspiFlash_readId(bool intrPollMode)
{
    int32_t     status = OSPI_FLASH_APP_STATUS_ERROR;
    uint8_t     cmd = OSPI_FLASH_CMD_RDID;
    uint8_t     idCode[OSPI_FLASH_RDID_NUM_BYTES];
    uint32_t    manfID, devID;

    OspiFlash_ospiXferIntrInit(intrPollMode);
    status = OspiFlash_cmdRead((uint8_t*)&cmd,
                               (uint32_t)1,
                               (uint8_t*)&idCode,
                               (uint32_t)OSPI_FLASH_RDID_NUM_BYTES,
                               (uint32_t)0);
    
    if(status == OSPI_FLASH_APP_STATUS_SUCCESS)
    {
        manfID = (uint32_t)idCode[0];
        devID = ((uint32_t)idCode[1] << 8) | ((uint32_t)idCode[2]);
        if(!((manfID == OSPI_FLASH_NOR_MANF_ID) && (devID == OSPI_FLASH_NOR_DEVICE_ID)))
        {
            status = OSPI_FLASH_APP_STATUS_ERROR;
        }
    }

    return (status);
}


int32_t OspiFlash_cmdWrite( const uint8_t  *cmdBuf,
                            uint32_t        cmdLen,
                            const uint8_t  *txBuf,
                            uint32_t        txLen,
                            bool            intrPollMode)
{
    int32_t status = OSPI_FLASH_APP_STATUS_ERROR;
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    OspiFlash_ospiXferIntrInit(intrPollMode);

    (void)CSL_ospiCmdWrite(baseAddr, cmdBuf, cmdLen, txBuf, txLen);

    status = OspiFlash_execCmd(baseAddr);

    return (status);
}

static int32_t OspiFlash_cmdRead(uint8_t                      *cmd,
                                 uint32_t                     cmdLen,
                                 uint8_t                      *rxBuf,
                                 uint32_t                     rxLen,
                                 uint32_t                     dummyCycles)
{
    uint32_t regVal;
    uint32_t rdLen;
    int32_t  status = OSPI_FLASH_APP_STATUS_ERROR;
    uint8_t *pBuf = rxBuf;
    
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    if(cmdLen==1)
    {
        (void)CSL_ospiCmdRead(baseAddr, *cmd, rxLen);
    }
    else
    {
        (void)CSL_ospiCmdExtRead(baseAddr, cmd, cmdLen, rxLen, dummyCycles);
    }
    status = OspiFlash_execCmd(baseAddr);
    if(status == OSPI_FLASH_APP_STATUS_SUCCESS)
    {
        regVal = CSL_REG32_RD(&baseAddr->FLASH_RD_DATA_LOWER_REG);

        /* Put the read value into rxBuf */
        rdLen = (rxLen > 4U) ? 4U : rxLen;
        (void)memcpy((void *)pBuf, (void *)(&regVal), rdLen);
        pBuf += rdLen;

        if(rxLen > 4U) {
            regVal = CSL_REG32_RD(&baseAddr->FLASH_RD_DATA_UPPER_REG);
            rdLen = rxLen - rdLen;
            (void)memcpy((void *)pBuf, (void *)(&regVal), rdLen);
        }
    }
    return (status);
}

int32_t OspiFlash_waitReady(uint32_t timeOut, bool intrPollMode)
{
    uint8_t         retVal=1;
    uint32_t        cmdLen;
    uint32_t        delay;
    
    #if defined(FLASH_TYPE_XSPI)
    uint8_t cmd[] 	= {OSPI_FLASH_CMD_RDREG, 0, OSPI_FLASH_NOR_VREG_OFFSET, 0, 0};
    cmdLen 	        = 5;
    delay 	        = OSPI_FLASH_NOR_OCTAL_DDR_CMD_READ_DUMMY_CYCLE;
    #else
    uint8_t cmd[] 	= {OSPI_FLASH_CMD_RDSR, 0, 0, 0, 0};
    cmdLen 	        = 1;
    delay 	        = 0;
    #endif

    do
    {   
        OspiFlash_ospiXferIntrInit(intrPollMode);

        if(OspiFlash_cmdRead(cmd, cmdLen, &retVal, 1, delay))
        {
            return OSPI_FLASH_APP_STATUS_ERROR;
        }

        if((retVal & OSPI_FLASH_SR_WIP) == 0)
        {
            break;
        }

        timeOut--;
        if(!timeOut) {
            break;
        }

    } while (1);

    if((retVal & OSPI_FLASH_SR_WIP) == 0)
    {
        return OSPI_FLASH_APP_STATUS_SUCCESS;
    }

    /* Timed out */
    return OSPI_FLASH_APP_STATUS_ERROR;
}

int32_t OspiFlash_waitDeviceReady(uint32_t timeOut)
{
    bool         status = OSPI_FLASH_APP_STATUS_ERROR;
    uint8_t      retVal;
    uint32_t     timeOutVal = timeOut;

    while (timeOutVal != 0U)
    {
        retVal = OspiFlash_getDeviceStatus(OSPI_FLASH_CMD_RDSR);
        if ((retVal & 1U) == 0U)
        {
            status = OSPI_FLASH_APP_STATUS_SUCCESS;
            break;
        }
        timeOutVal--;
        OspiFlash_delay(OSPI_FLASH_CHECK_IDLE_DELAY);
    }

    return (status);
}

static uint8_t OspiFlash_getDeviceStatus(uint32_t rdStatusCmd)
{
    uint8_t                status = 0xFF;

    (void)OspiFlash_cmdRead((uint8_t*)&rdStatusCmd,
                            (uint32_t)1,
                            (uint8_t*)&status,
                            (uint32_t)1,
                            (uint32_t)0);

    return (status);
}

inline void OspiFlash_ospiXferIntrInit(bool intrPollMode)
{
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    
    if(FALSE == intrPollMode)
    {
        Osal_EnableInterrupt((int32_t)0U, (int32_t)OSPI_FLASH_INTR_NUM);
    }

    /* Disable and clear the interrupts */
    CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL, FALSE);
    CSL_ospiIntrClear(baseAddr, CSL_OSPI_INTR_MASK_ALL);

    if(FALSE == intrPollMode)
    {
        CSL_ospiIntrEnable(baseAddr, CSL_OSPI_INTR_MASK_ALL, TRUE);
    }

    return;
}

static int32_t OspiFlash_execCmd(const CSL_ospi_flash_cfgRegs *pRegs)
{
    uint32_t retry = OSPI_FLASH_WRITE_TIMEOUT;
    int32_t  status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint32_t idleFlag = FALSE;

    while (idleFlag == FALSE)
    {
        idleFlag = CSL_ospiIsIdle(pRegs);
    }

    /* Start to execute flash read/write command */
    CSL_ospiFlashExecCmd(pRegs);

    while (retry != 0U)
    {
        /* Check the command execution status */
        if(CSL_ospiFlashExecCmdComplete(pRegs) == TRUE)
        {
            break;
        }
        OspiFlash_delay(OSPI_FLASH_CHECK_IDLE_DELAY);
        retry--;
    }

    if(retry == 0U)
    {
        status = OSPI_FLASH_APP_STATUS_ERROR;
    }

    idleFlag = FALSE;
    while (idleFlag == FALSE)
    {
        idleFlag = CSL_ospiIsIdle(pRegs);
    }

    return (status);
}

static int32_t OspiFlash_waitIdle(uint32_t timeOut, uint32_t idleDelay)
{
    uint32_t               retry = 0U;
    int32_t                status = OSPI_FLASH_APP_STATUS_ERROR;
    uint32_t               timeOutVal = timeOut;

    while (timeOutVal != 0U)
    {
        if(CSL_ospiIsIdle((const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR)) != 0U)
        {
            retry++;
            if(retry == 3U)
            {
                status = OSPI_FLASH_APP_STATUS_SUCCESS;
                break;
            }
        }
        else
        {
            retry = 0U;
        }
        OspiFlash_delay(idleDelay);
        timeOutVal--;
    }

    return (status);
}

static void OspiFlash_delay(uint32_t delay)
{
    volatile uint32_t i = delay;

    while (i > 0U)
    {
        i = i - 1U;
    }
}

int32_t OspiFlash_ClkRateSet(uint32_t modId,
                             uint32_t clkId,
                             uint64_t clkRate)
{
    uint32_t i                          = 0U;
    int32_t status                      = OSPI_FLASH_APP_STATUS_SUCCESS;
    int32_t finalStatus                 = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint64_t respClkRate                = 0;
    uint32_t numParents                 = 0U;
    uint32_t moduleClockParentChanged   = 0U;
    uint32_t clockStatus                = 0U;
    uint32_t origParent                 = 0U;
    uint32_t foundParent                = 0U;

    /* Check if the clock is enabled or not */
    status = Sciclient_pmModuleGetClkStatus(modId,
                                            clkId,
                                            &clockStatus,
                                            SCICLIENT_SERVICE_WAIT_FOREVER);
    if (status == CSL_PASS)
    {
        /* Get the number of parents for the clock */
        status = Sciclient_pmGetModuleClkNumParent(modId,
                                                   clkId,
                                                   &numParents,
                                                   SCICLIENT_SERVICE_WAIT_FOREVER);
        if ((status == CSL_PASS) && (numParents > 1U))
        {
            status = Sciclient_pmGetModuleClkParent(modId, 
                                                    clkId, 
                                                    &origParent,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
        }
    }
    if (status == CSL_PASS)
    {
        /* Disabling the clock */
        status = Sciclient_pmModuleClkRequest(modId,
                                              clkId,
                                              TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ,
                                              0U,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if (status == CSL_PASS)
    {
        foundParent = 0U;
        /* Try to loop and change parents of the clock */
        for(i=0U;i<numParents;i++)
        {
            if (numParents > 1U)
            {
                /* Setting the new parent */
                status = Sciclient_pmSetModuleClkParent(modId,
                                                        clkId,
                                                        clkId+i+1U,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
                /* Check if the clock can be set to desirable freq. */
                if (status == CSL_PASS)
                {
                    moduleClockParentChanged = 1U;
                }
            }
            if (status == CSL_PASS)
            {
                status = Sciclient_pmQueryModuleClkFreq(modId,
                                                        clkId,
                                                        clkRate,
                                                        &respClkRate,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            if ((status == CSL_PASS) && (respClkRate == clkRate))
            {
                foundParent = 1U;
                break;
            }
        }
    }
    if ((status == CSL_PASS) && (numParents == 0U))
    {
        status = Sciclient_pmQueryModuleClkFreq(modId,
                                                clkId,
                                                clkRate,
                                                &respClkRate,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
        if ((status == CSL_PASS) && (respClkRate == clkRate))
        {
            foundParent = 1U;
        }
    }
    if (foundParent == 1U)
    {
        /* Set the clock at the desirable frequency*/
        status = Sciclient_pmSetModuleClkFreq(modId,
                                              clkId,
                                              clkRate,
                                              TISCI_MSG_FLAG_CLOCK_ALLOW_FREQ_CHANGE,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    else
    {
        status = CSL_EFAIL;
    }
    if ((status == CSL_PASS) &&
        (clockStatus == (uint32_t) TISCI_MSG_VALUE_CLOCK_SW_STATE_UNREQ))
    {
        /* Restore the clock again to original state */
        status = Sciclient_pmModuleClkRequest(modId,
                                              clkId,
                                              clockStatus,
                                              0U,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    finalStatus = status;
    if ((status != CSL_PASS) && (moduleClockParentChanged == 1U))
    {
        /* Setting the original parent if failure */
        (void) Sciclient_pmSetModuleClkParent(modId,
                                              clkId,
                                              origParent,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    return (finalStatus);
}

