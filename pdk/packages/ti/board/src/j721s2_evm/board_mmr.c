/******************************************************************************
 * Copyright (c) 2021-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/
/**
 *  \file   board_mmr.c
 *
 *  \brief  MMR configuration file
 *
 *  This file contains the function to unlock the MMR registers
 *
 */

#include "board_internal.h"
#include <ti/csl/soc/j721s2/src/cslr_wkup_ctrl_mmr.h>

/* Array to store the state of the kick registers before doing unlock */
static uint32_t gBoardKickState[BOARD_SOC_DOMAIN_MAX][BOARD_MMR_PARTITION_MAX] = {{0,0,0,0,0,0,0,0},
                                                                                  {0,0,0,0,0,0,0,0},
                                                                                  {0,0,0,0,0,0,0,0}};

/**
 *  \brief  Gets base address of MMR control registers
 *
 *  \param   domain [IN]  SoC domain for MMR register space
 *  \n                     BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                     BOARD_SOC_DOMAIN_MCU  - MCU domain
 *  \n                     BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *
 *  \return   Valid address in case success or 0 in case of failure
 */
static uint32_t Board_mmrGetBaseAddr(uint8_t domain)
{
    uint32_t baseAddr;

    switch(domain)
    {
        case BOARD_SOC_DOMAIN_MAIN:
            baseAddr = (CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK0);
        break;
        case BOARD_SOC_DOMAIN_MCU:
            baseAddr = (CSL_MCU_CTRL_MMR0_CFG0_BASE + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0);
        break;
        case BOARD_SOC_DOMAIN_WKUP:
            baseAddr = (CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_LOCK0_KICK0);
        break;
        default:
            baseAddr = 0;
        break;
    }

    return (baseAddr);
}

/**
 * \brief  Locks MMR register partition
 *
 *  \param   domain  [IN]  SoC domain for MMR register space
 *  \n                      BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                      BOARD_SOC_DOMAIN_MCU  - MCU domain
 *  \n                      BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *
 *  \param   partNum [IN]  Partition number
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_lockMMRPartition(uint32_t domain, uint32_t partNum)
{
    Board_STATUS status = BOARD_SOK;
    uint32_t baseAddr;
    uint32_t kick0;

    /**
     * Lock the partition only if the kick register is in lock state before board unlock.
     * This will ensure that the board module is not changing state of the kick registers
     * which are unlocked by some other module/core.
     */
    if(!gBoardKickState[domain][partNum])
    {
        baseAddr  = Board_mmrGetBaseAddr(domain);
        baseAddr += (BOARD_MMR_PARTITION_SIZE * partNum);
        HW_WR_REG32(baseAddr, BOARD_KICK0_LOCK_VAL);
        HW_WR_REG32((baseAddr + 4), BOARD_KICK1_LOCK_VAL);

        /* Confirm the kick registers are locked */
        kick0 = HW_RD_REG32(baseAddr);
        if(kick0 & 0x01)
        {
            status = BOARD_FAIL;
        }
    }

    return (status);
}

/**
 * \brief  Unlocks MMR register partition
 *
 *  \param   domain  [IN]  SoC domain for MMR register space
 *  \n                      BOARD_SOC_DOMAIN_MAIN - Main domain
 *  \n                      BOARD_SOC_DOMAIN_MCU  - MCU domain
 *  \n                      BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *
 *  \param   partNum [IN]  Partition number
 *
 * \return  Board_STATUS
 */

Board_STATUS Board_unlockMMRPartition(uint32_t domain, uint32_t partNum)
{
    Board_STATUS status = BOARD_SOK;
    uint32_t baseAddr;
    uint32_t kick0;

    baseAddr  = Board_mmrGetBaseAddr(domain);
    baseAddr += (BOARD_MMR_PARTITION_SIZE * partNum);

    /* Read the current state of the kick lock. Will be used by board lock function */
    gBoardKickState[domain][partNum] = HW_RD_REG32(baseAddr) & 0x1;

    HW_WR_REG32(baseAddr, BOARD_KICK0_UNLOCK_VAL);
    HW_WR_REG32((baseAddr + 4), BOARD_KICK1_UNLOCK_VAL);

    /* Confirm the kick registers are locked */
    kick0 = HW_RD_REG32(baseAddr);
    if(!(kick0 & 0x01))
    {
        status = BOARD_FAIL;
    }

    return (status);
}

/**
 * \brief  Locks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_lockMMR(void)
{
    Board_STATUS status;

    Board_setRATCfg();

    /* Lock MAIN MMR registers */
    status  = Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION0);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION1);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION2);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION3);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION5);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION7);

    /* Lock WAKEUP MMR registers */
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION0);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION1);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION2);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION3);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION5);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION6);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION7);

    /* Lock MCU MMR registers */
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION0);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION1);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION2);
    status |= Board_lockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION3);

    Board_restoreRATCfg();

    return status;
}

/**
 * \brief  Unlocks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_unlockMMR(void)
{
    Board_STATUS status;

    Board_setRATCfg();

    /* Unlock MAIN MMR registers */
    status  = Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION0);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION1);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION2);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION3);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION5);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MAIN, BOARD_MMR_PARTITION7);

    /* Unlock WAKEUP MMR registers */
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION0);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION1);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION2);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION3);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION5);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION6);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_WKUP, BOARD_MMR_PARTITION7);

    /* Unlock MCU MMR registers */
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION0);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION1);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION2);
    status |= Board_unlockMMRPartition(BOARD_SOC_DOMAIN_MCU, BOARD_MMR_PARTITION3);

    Board_restoreRATCfg();

    return status;
}
