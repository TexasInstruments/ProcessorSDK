/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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

#include <ti/csl/soc/am64x/src/cslr_main_ctrl_mmr.h>
#include <ti/csl/soc/am64x/src/cslr_mcu_ctrl_mmr.h>
#include <ti/csl/soc/am64x/src/cslr_main_pll_mmr.h>
#include <ti/csl/soc/am64x/src/cslr_mcu_pll_mmr.h>
#include "board_internal.h"

#define BOARD_MMR_PASS   (1U)
#define BOARD_MMR_FAIL   (0U)

typedef enum 
{
    MMR_UNLOCK= 1,
    MMR_LOCK=0
} Board_MmrLockActions_t;

/* define the unlock and lock values */
#define KICK_LOCK_VAL    0x00000000

#define MAIN_MMR_BASE_ADDRESS   CSL_CTRL_MMR0_CFG0_BASE
#define MCU_MMR_BASE_ADDRESS    CSL_MCU_CTRL_MMR0_CFG0_BASE

#define MAIN_PLL_MMR_BASE_ADDRESS   CSL_PLL0_CFG_BASE
#define MCU_PLL_MMR_BASE_ADDRESS    CSL_MCU_PLL0_CFG_BASE

#ifdef BUILD_M4F
#define MAIN_PADCONFIG_MMR_BASE_ADDRESS CSL_PADCFG_CTRL0_CFG0_BASE + 0x60000000
#define MCU_PADCONFIG_MMR_BASE_ADDRESS CSL_MCU_PADCFG_CTRL0_CFG0_BASE + 0x60000000
#else
#define MAIN_PADCONFIG_MMR_BASE_ADDRESS CSL_PADCFG_CTRL0_CFG0_BASE
#define MCU_PADCONFIG_MMR_BASE_ADDRESS CSL_MCU_PADCFG_CTRL0_CFG0_BASE
#endif

static const uint32_t Board_mmrMainPadCfgKickOffsets[]= {CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK0_KICK0,
                                                         CSL_MAIN_PADCFG_CTRL_MMR_CFG0_LOCK1_KICK0};
static const uint32_t Board_mmrMainPadCfgKickNum = sizeof(Board_mmrMainPadCfgKickOffsets)/sizeof(uint32_t);

static const uint32_t Board_mmrMcuPadCfgKickOffsets[]= {CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0,
                                                        CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK0};
static const uint32_t Board_mmrMcuPadCfgKickNum = sizeof(Board_mmrMcuPadCfgKickOffsets)/sizeof(uint32_t);

static uint32_t Board_mmrChangeLock(Board_MmrLockActions_t targetState,
                                    volatile uint32_t *kick0)
{
    volatile uint32_t *kick1 = kick0 + 1;
    uint32_t lock_state = (*kick0 & 0x1); /* status is 1 if unlocked, 0 if locked */

    /* If lock state is not what we want, change it */
    if (lock_state != (uint32_t) targetState )
    {
        switch(targetState)
        {
            case MMR_LOCK:
                /* lock the partition by writing the lock values to the kick lock registers */
                *kick0 = KICK_LOCK_VAL;
                *kick1 = KICK_LOCK_VAL;
                break;
            case MMR_UNLOCK:
                /* unlock the partition by writing the unlock values to the kick lock registers */
                *kick0 = KICK0_UNLOCK_VAL;
                *kick1 = KICK1_UNLOCK_VAL;
                break;
        }

        lock_state = (*kick0 & 0x1);
        /* Error out if the change did not take effect */
        if(lock_state!= (uint32_t) targetState )
        {
            return BOARD_MMR_FAIL;
        }
    }

    /* Return pass if lock is already what we want or if changing lock succeeds */
    return BOARD_MMR_PASS;
}

static uint32_t Board_mmrChangeAllLocks(Board_MmrLockActions_t targetState,
                                        uint32_t base_addr,
                                        const uint32_t * offset_array,
                                        uint32_t array_size)
{
    uint32_t errors = 0;
    uint32_t i = 0;
    volatile uint32_t * kick0_ptr;

    for(i = 0; i < array_size; i++)
    {
        kick0_ptr = (volatile uint32_t *) (base_addr + offset_array[i]);
        if(Board_mmrChangeLock(targetState, kick0_ptr) == BOARD_MMR_FAIL)
        {
            errors++;
        }
    }

    return errors;
}

static uint32_t Board_mmrMainPadCfgChangeAllLocks(Board_MmrLockActions_t targetState)
{
    uint32_t errors=Board_mmrChangeAllLocks(targetState, (uint32_t) MAIN_PADCONFIG_MMR_BASE_ADDRESS, Board_mmrMainPadCfgKickOffsets, Board_mmrMainPadCfgKickNum);

    if(errors == 0)
    {
        return BOARD_MMR_PASS;
    }
    else
    {
        return BOARD_MMR_FAIL;
    }
}

static uint32_t Board_mainPadCfgMmrUnlockAll(void)
{
    return Board_mmrMainPadCfgChangeAllLocks(MMR_UNLOCK);
}

static uint32_t Board_mmrMainPadCfgLockAll(void)
{
    return Board_mmrMainPadCfgChangeAllLocks(MMR_LOCK);
}

static uint32_t Board_mmrMcuPadCfgChangeAllLocks(Board_MmrLockActions_t targetState)
{
    uint32_t errors=Board_mmrChangeAllLocks(targetState, (uint32_t) MCU_PADCONFIG_MMR_BASE_ADDRESS, Board_mmrMcuPadCfgKickOffsets, Board_mmrMcuPadCfgKickNum);

    if(errors == 0)
    {
        return BOARD_MMR_PASS;
    }
    else
    {
        return BOARD_MMR_FAIL;
    }
}

static uint32_t Board_mmrMcuPadCfgUnlockAll(void)
{
    return Board_mmrMcuPadCfgChangeAllLocks(MMR_UNLOCK);
}

uint32_t Board_mmrMcuPadCfgLockAll(void)
{
    return Board_mmrMcuPadCfgChangeAllLocks(MMR_LOCK);
}

/**
 * \brief  Unlocks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_unlockMMR(void)
{
    Board_mainPadCfgMmrUnlockAll();
    Board_mmrMcuPadCfgUnlockAll();

    return BOARD_SOK;
}

/**
 * \brief  Locks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_lockMMR(void)
{
    Board_mmrMainPadCfgLockAll();
    Board_mmrMcuPadCfgLockAll();

    return BOARD_SOK;
}
