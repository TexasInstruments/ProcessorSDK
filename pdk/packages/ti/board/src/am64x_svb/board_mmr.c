/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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

static const uint32_t main_ctrl_mmr_kick_offsets[]= {  CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK0,
                                                CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0,
                                                //CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK0,
                                                //CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK0,
                                             };
static const uint32_t main_ctrl_mmr_kick_num = sizeof(main_ctrl_mmr_kick_offsets)/sizeof(uint32_t);

static const uint32_t mcu_ctrl_mmr_kick_offsets[]= {   CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK0,
                                                //CSL_MCU_CTRL_MMR_CFG0_LOCK5_KICK0,
                                                CSL_MCU_CTRL_MMR_CFG0_LOCK6_KICK0,
                                                //CSL_MCU_CTRL_MMR_CFG0_LOCK7_KICK0,
                                             };
static const uint32_t mcu_ctrl_mmr_kick_num = sizeof(mcu_ctrl_mmr_kick_offsets)/sizeof(uint32_t);



static const uint32_t main_pll_mmr_kick_offsets[]= {  CSL_MAIN_PLL_MMR_CFG_PLL0_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL1_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL2_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL3_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL4_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL5_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL6_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL7_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL8_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL9_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL10_LOCKKEY0,
                                               //CSL_MAIN_PLL_MMR_CFG_PLL11_LOCKKEY0,
                                               CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY0,
                                             };
static const uint32_t main_pll_mmr_kick_num = sizeof(main_pll_mmr_kick_offsets)/sizeof(uint32_t);

static const uint32_t mcu_pll_mmr_kick_offsets[]= {  CSL_MCU_PLL_MMR_CFG_PLL0_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL2_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL3_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL4_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL5_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL6_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL7_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL8_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL9_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL10_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL11_LOCKKEY0,
                                              //CSL_MCU_PLL_MMR_CFG_PLL12_LOCKKEY0,
                                            };
static const uint32_t mcu_pll_mmr_kick_num = sizeof(mcu_pll_mmr_kick_offsets)/sizeof(uint32_t);


#define TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK0_KICK0_OFFSET (0x1008)
#define TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK1_KICK0_OFFSET (0x5008)

#define TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK0_KICK0_OFFSET (0x1008)
#define TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK1_KICK0_OFFSET (0x5008)

static const uint32_t main_padcfg_mmr_kick_offsets[]= {  TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK0_KICK0_OFFSET,
                                                  TEMP_REPLACE_WITH_CSL_MAIN_PADCONFIG_LOCK1_KICK0_OFFSET,
                                               };
static const uint32_t main_padcfg_mmr_kick_num = sizeof(main_padcfg_mmr_kick_offsets)/sizeof(uint32_t);

static const uint32_t mcu_padcfg_mmr_kick_offsets[]= {   TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK0_KICK0_OFFSET,
                                                  TEMP_REPLACE_WITH_CSL_MCU_PADCONFIG_LOCK1_KICK0_OFFSET,
                                              };
static const uint32_t mcu_padcfg_mmr_kick_num = sizeof(mcu_padcfg_mmr_kick_offsets)/sizeof(uint32_t);



typedef enum {
    MMR_UNLOCK= 1,
    MMR_LOCK=0
} mmr_lock_actions_t;

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

uint32_t MMR_change_lock(mmr_lock_actions_t target_state, volatile uint32_t * kick0);
uint32_t generic_mmr_change_all_locks(mmr_lock_actions_t target_state, uint32_t base_addr, const  uint32_t * offset_array, uint32_t array_size);

uint32_t MAIN_PADCONFIG_MMR_unlock_all();
uint32_t MAIN_PADCONFIG_MMR_lock_all();
uint32_t MAIN_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MCU_PADCONFIG_MMR_unlock_all();
uint32_t MCU_PADCONFIG_MMR_lock_all();
uint32_t MCU_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MAIN_CTRL_MMR_unlock_all();
uint32_t MAIN_CTRL_MMR_lock_all();
uint32_t MAIN_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MCU_CTRL_MMR_unlock_all();
uint32_t MCU_CTRL_MMR_lock_all();
uint32_t MCU_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MAIN_PLL_MMR_unlock_all();
uint32_t MAIN_PLL_MMR_lock_all();
uint32_t MAIN_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state);

uint32_t MCU_PLL_MMR_unlock_all();
uint32_t MCU_PLL_MMR_lock_all();
uint32_t MCU_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state);


    uint32_t MMR_change_lock(mmr_lock_actions_t target_state, volatile uint32_t * kick0) {
        volatile uint32_t * kick1 = kick0 + 1;
        uint32_t lock_state = (*kick0 & 0x1); //status is 1 if unlocked, 0 if locked

        //If lock state is not what we want, change it
        if (lock_state != (uint32_t) target_state ) {
            switch(target_state) {
                case MMR_LOCK:
                    // lock the partition by writing the lock values to the kick lock registers
                    *kick0 = KICK_LOCK_VAL;
                    *kick1 = KICK_LOCK_VAL;
                    break;
                case MMR_UNLOCK:
                    // unlock the partition by writing the unlock values to the kick lock registers
                    *kick0 = KICK0_UNLOCK_VAL;
                    *kick1 = KICK1_UNLOCK_VAL;
                    break;
            }

            lock_state = (*kick0 & 0x1);
            //Error out if the change did not take effect
            if(lock_state!= (uint32_t) target_state ){
                //Could insert debug statement here
                //printf("SAVV_DEBUG: Error in changing MMR lock state at address %llx", kick0 );
                return BOARD_MMR_FAIL;
            }
        }
        //Return pass if lock is already what we want or if changing lock succeeds
        return BOARD_MMR_PASS;
    }
    uint32_t generic_mmr_change_all_locks(mmr_lock_actions_t target_state, uint32_t base_addr, const uint32_t * offset_array, uint32_t array_size) {
        uint32_t errors=0;
        uint32_t i=0;
        volatile uint32_t * kick0_ptr;
        for(i=0;i<array_size;i++) {
            kick0_ptr = (volatile uint32_t *) (base_addr + offset_array[i]);
            if(MMR_change_lock(target_state, kick0_ptr) == BOARD_MMR_FAIL){
                errors++;
            }
        }
        return errors;
    }
    //MAIN_CTRL_MMR
    uint32_t MAIN_CTRL_MMR_unlock_all() {
        return MAIN_CTRL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MAIN_CTRL_MMR_lock_all() {
        return MAIN_CTRL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MAIN_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MAIN_MMR_BASE_ADDRESS, main_ctrl_mmr_kick_offsets, main_ctrl_mmr_kick_num);
        if(errors==0) { return BOARD_MMR_PASS; }
        else          { return BOARD_MMR_FAIL; }
    }
    //MCU_CTRL_MMR
    uint32_t MCU_CTRL_MMR_unlock_all() {
        return MCU_CTRL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MCU_CTRL_MMR_lock_all() {
        return MCU_CTRL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MCU_CTRL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MCU_MMR_BASE_ADDRESS, mcu_ctrl_mmr_kick_offsets, mcu_ctrl_mmr_kick_num);
        if(errors==0) { return BOARD_MMR_PASS; }
        else          { return BOARD_MMR_FAIL; }
    }
    //MAIN_PLL_MMR
    uint32_t MAIN_PLL_MMR_unlock_all() {
        return MAIN_PLL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MAIN_PLL_MMR_lock_all() {
        return MAIN_PLL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MAIN_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MAIN_PLL_MMR_BASE_ADDRESS, main_pll_mmr_kick_offsets, main_pll_mmr_kick_num);
        if(errors==0) { return BOARD_MMR_PASS; }
        else          { return BOARD_MMR_FAIL; }
    }
    //MCU_PLL_MMR
    uint32_t MCU_PLL_MMR_unlock_all() {
        return MCU_PLL_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MCU_PLL_MMR_lock_all() {
        return MCU_PLL_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MCU_PLL_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MCU_PLL_MMR_BASE_ADDRESS, mcu_pll_mmr_kick_offsets, mcu_pll_mmr_kick_num);
        if(errors==0) { return BOARD_MMR_PASS; }
        else          { return BOARD_MMR_FAIL; }
    }
    //MAIN_PADCONFIG_MMR
    uint32_t MAIN_PADCONFIG_MMR_unlock_all() {
        return MAIN_PADCONFIG_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MAIN_PADCONFIG_MMR_lock_all() {
        return MAIN_PADCONFIG_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MAIN_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MAIN_PADCONFIG_MMR_BASE_ADDRESS, main_padcfg_mmr_kick_offsets, main_padcfg_mmr_kick_num);
        if(errors==0) { return BOARD_MMR_PASS; }
        else          { return BOARD_MMR_FAIL; }
    }
    //MCU_PADCONFIG_MMR
    uint32_t MCU_PADCONFIG_MMR_unlock_all() {
        return MCU_PADCONFIG_MMR_change_all_locks(MMR_UNLOCK);
    }
    uint32_t MCU_PADCONFIG_MMR_lock_all() {
        return MCU_PADCONFIG_MMR_change_all_locks(MMR_LOCK);
    }
    uint32_t MCU_PADCONFIG_MMR_change_all_locks(mmr_lock_actions_t target_state) {
        uint32_t errors=generic_mmr_change_all_locks(target_state, (uint32_t) MCU_PADCONFIG_MMR_BASE_ADDRESS, mcu_padcfg_mmr_kick_offsets, mcu_padcfg_mmr_kick_num);
        if(errors==0) { return BOARD_MMR_PASS; }
        else          { return BOARD_MMR_FAIL; }
    }

/**
 * \brief  Unlocks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_unlockMMR(void)
{
    MAIN_PADCONFIG_MMR_unlock_all();
    MCU_PADCONFIG_MMR_unlock_all();
    return BOARD_SOK;
}

/**
 * \brief  Locks MMR registers
 *
 * \return  Board_STATUS
 */
Board_STATUS Board_lockMMR(void)
{
    MAIN_PADCONFIG_MMR_lock_all();
    MCU_PADCONFIG_MMR_lock_all();
    return BOARD_SOK;
}

