/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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
 */

/**
 *  \file   dm_ut_clocks_list.c
 *
 *  \brief  This file contains lists of different types of clocks for C7X and A72 core present on J784S4.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/sciclient/soc/sysfw/include/j784s4/tisci_devices.h>
#include<ti/drv/sciclient/soc/sysfw/include/j784s4/tisci_clocks.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Max frequency supported by C7X core (1GHz) */
#define DM_APP_C7X_MAX_FREQ          ((uint64_t) 1000000000U)

/* Max frequency supported by A72 core (2GHz) */
#define DM_APP_A72_MAX_FREQ          ((uint64_t) 2000000000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Global Variables                                 */
/* ========================================================================== */

const uint32_t gDMAppProcessorClocks[][3] = {
    /* These are C7X clocks */
    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS0,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS1,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS2,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS3,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0,
        TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0_C7X_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_COMPUTE_CLUSTER0_DEBUG_WRAP_0,
        TISCI_DEV_COMPUTE_CLUSTER0_DEBUG_WRAP_0_CLK1_CLK_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS0,
        TISCI_DEV_A72SS0_ARM0_MSMC_CLK_CLK,
        DM_APP_C7X_MAX_FREQ
    },

    /* These are A72 Clocks comming from main_PLL8 */
    {
        TISCI_DEV_DCC3,
        TISCI_DEV_DCC3_DCC_CLKSRC7_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS0,
        TISCI_DEV_A72SS0_ARM0_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS0_CORE0,
        TISCI_DEV_A72SS0_CORE0_ARM0_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS0_CORE1,
        TISCI_DEV_A72SS0_CORE1_ARM0_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS0_CORE2,
        TISCI_DEV_A72SS0_CORE2_ARM0_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS0_CORE3,
        TISCI_DEV_A72SS0_CORE3_ARM0_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_MAIN_PLL8_SEL_EXTWAVE_VD,
        TISCI_DEV_MAIN_PLL8_SEL_EXTWAVE_VD_CLK_PARENT_HSDIV0_16FFT_MAIN_8_HSDIVOUT0_CLK,
        DM_APP_A72_MAX_FREQ
    },

    /* These are A72 Clocks comming from main_PLL9 */
    {
        TISCI_DEV_DCC3,
        TISCI_DEV_DCC3_DCC_CLKSRC3_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS1,
        TISCI_DEV_A72SS1_ARM1_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS1_CORE0,
        TISCI_DEV_A72SS1_CORE0_ARM1_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS1_CORE1,
        TISCI_DEV_A72SS1_CORE1_ARM1_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS1_CORE2,
        TISCI_DEV_A72SS1_CORE2_ARM1_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_A72SS1_CORE3,
        TISCI_DEV_A72SS1_CORE3_ARM1_CLK_CLK,
        DM_APP_A72_MAX_FREQ
    },

    {
        TISCI_DEV_MAIN_PLL9_SEL_EXTWAVE_VD,
        TISCI_DEV_MAIN_PLL9_SEL_EXTWAVE_VD_CLK_PARENT_HSDIV0_16FFT_MAIN_9_HSDIVOUT0_CLK,
        DM_APP_A72_MAX_FREQ
    }
};

uint32_t gDMAppProcessorClocksSize = sizeof(gDMAppProcessorClocks)/sizeof(gDMAppProcessorClocks[0]);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */
