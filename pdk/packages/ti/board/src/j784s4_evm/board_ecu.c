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
 *  \file  board_ecu.c
 *
 *  \brief Auto generated file from DM Power Configuration Tool, used to configure
 *         PLL and PSC based on use case.
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/sciclient/sciclient.h>
#include "board_ecu.h"
#include "board_pll.h"
#include "board_clock.h"
#include "board_utils.h"

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint32_t gBoardEcuClkModuleMcuIDInit_powerConfig0[] ={
    // Fixed config - None
    // GUI configured

};

static uint32_t gBoardEcuClkModuleMcuIDInit_powerConfig1[] ={
    // Fixed config - None
    // GUI configured

};

static uint32_t gBoardEcuClkModuleMcuIDInit_powerConfig2[] ={
    // Fixed config - None
    // GUI configured

};

static uint32_t gBoardEcuClkModuleMainIDInit_powerConfig0[] = {
    // Fixed config - None
    // GUI configured
    TISCI_DEV_VPAC0,
    TISCI_DEV_VPAC1,
    TISCI_DEV_DMPAC0,
    TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
    TISCI_DEV_CODEC0,
    TISCI_DEV_CODEC1,
    TISCI_DEV_EMIF_DATA_0_VD,
    TISCI_DEV_DDR0,
    TISCI_DEV_EMIF_DATA_1_VD,
    TISCI_DEV_DDR1,
    TISCI_DEV_EMIF_DATA_2_VD,
    TISCI_DEV_DDR2,
    TISCI_DEV_EMIF_DATA_3_VD,
    TISCI_DEV_DDR3,
};

static uint32_t gBoardEcuClkModuleMainIDInit_powerConfig1[] = {
    // Fixed config - None
    // GUI configured
    TISCI_DEV_VPAC0,
    TISCI_DEV_VPAC1,
    TISCI_DEV_DMPAC0,
    TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
    TISCI_DEV_CODEC0,
    TISCI_DEV_CODEC1,
    TISCI_DEV_EMIF_DATA_0_VD,
    TISCI_DEV_DDR0,
    TISCI_DEV_EMIF_DATA_1_VD,
    TISCI_DEV_DDR1,
    TISCI_DEV_EMIF_DATA_2_VD,
    TISCI_DEV_DDR2,
    TISCI_DEV_EMIF_DATA_3_VD,
    TISCI_DEV_DDR3,
};

static uint32_t gBoardEcuClkModuleMainIDInit_powerConfig2[] = {
    // Fixed config - None
    // GUI configured
    TISCI_DEV_VPAC0,
    TISCI_DEV_VPAC1,
    TISCI_DEV_DMPAC0,
    TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
    TISCI_DEV_CODEC0,
    TISCI_DEV_CODEC1,
    TISCI_DEV_EMIF_DATA_0_VD,
    TISCI_DEV_DDR0,
    TISCI_DEV_EMIF_DATA_1_VD,
    TISCI_DEV_DDR1,
    TISCI_DEV_EMIF_DATA_2_VD,
    TISCI_DEV_DDR2,
    TISCI_DEV_EMIF_DATA_3_VD,
    TISCI_DEV_DDR3,
};

static Board_PllClkCfg_t gBoardEcuPllClkCfgMcu_powerConfig0[] =
{  // Fixed config
    {
      TISCI_DEV_MCU_CPSW0,
      TISCI_DEV_MCU_CPSW0_CPTS_RFT_CLK_PARENT_HSDIV4_16FFT_MCU_2_HSDIVOUT1_CLK,
      200000000
    },
   // GUI configured
};

static Board_PllClkCfg_t gBoardEcuPllClkCfgMcu_powerConfig1[] =
{  // Fixed config
    {
      TISCI_DEV_MCU_CPSW0,
      TISCI_DEV_MCU_CPSW0_CPTS_RFT_CLK_PARENT_HSDIV4_16FFT_MCU_2_HSDIVOUT1_CLK,
      200000000
    },
   // GUI configured
};

static Board_PllClkCfg_t gBoardEcuPllClkCfgMcu_powerConfig2[] =
{  // Fixed config
    {
      TISCI_DEV_MCU_CPSW0,
      TISCI_DEV_MCU_CPSW0_CPTS_RFT_CLK_PARENT_HSDIV4_16FFT_MCU_2_HSDIVOUT1_CLK,
      200000000
    },
   // GUI configured
};


static Board_PllClkCfg_t gBoardEcuPllClkCfgMain_powerConfig0[] =
{  // Fixed config
    {
      TISCI_DEV_GTC0,
      TISCI_DEV_GTC0_GTC_CLK_PARENT_POSTDIV3_16FFT_MAIN_0_HSDIVOUT6_CLK,
      200000000
    },
    // GUI configured
    {
      TISCI_DEV_A72SS0,
      TISCI_DEV_A72SS0_ARM0_CLK_CLK,
      2000000000
    },
    {
      TISCI_DEV_A72SS1,
      TISCI_DEV_A72SS1_ARM1_CLK_CLK,
      2000000000
    },
    {
      TISCI_DEV_R5FSS0_CORE0,
      TISCI_DEV_R5FSS0_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_R5FSS1_CORE0,
      TISCI_DEV_R5FSS1_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_R5FSS2_CORE0,
      TISCI_DEV_R5FSS2_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_VPAC0,
      TISCI_DEV_VPAC0_MAIN_CLK,
      720000000
    },
    {
      TISCI_DEV_VPAC1,
      TISCI_DEV_VPAC1_MAIN_CLK,
      720000000
    },
    {
      TISCI_DEV_DMPAC0,
      TISCI_DEV_DMPAC0_CLK,
      480000000
    },
    {
      TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
      TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0_GPU_PLL_CLK,
      800000000
    },
    {
      TISCI_DEV_CODEC0,
      TISCI_DEV_CODEC0_VPU_ACLK_CLK,
      600000000
    },
    {
      TISCI_DEV_CODEC1,
      TISCI_DEV_CODEC1_VPU_ACLK_CLK,
      600000000
    },
};

static Board_PllClkCfg_t gBoardEcuPllClkCfgMain_powerConfig1[] =
{  // Fixed config
    {
      TISCI_DEV_GTC0,
      TISCI_DEV_GTC0_GTC_CLK_PARENT_POSTDIV3_16FFT_MAIN_0_HSDIVOUT6_CLK,
      200000000
    },
    // GUI configured
    {
      TISCI_DEV_A72SS0,
      TISCI_DEV_A72SS0_ARM0_CLK_CLK,
      2000000000
    },
    {
      TISCI_DEV_A72SS1,
      TISCI_DEV_A72SS1_ARM1_CLK_CLK,
      2000000000
    },
    {
      TISCI_DEV_R5FSS0_CORE0,
      TISCI_DEV_R5FSS0_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_R5FSS1_CORE0,
      TISCI_DEV_R5FSS1_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_R5FSS2_CORE0,
      TISCI_DEV_R5FSS2_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_VPAC0,
      TISCI_DEV_VPAC0_MAIN_CLK,
      720000000
    },
    {
      TISCI_DEV_VPAC1,
      TISCI_DEV_VPAC1_MAIN_CLK,
      720000000
    },
    {
      TISCI_DEV_DMPAC0,
      TISCI_DEV_DMPAC0_CLK,
      480000000
    },
    {
      TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
      TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0_GPU_PLL_CLK,
      800000000
    },
    {
      TISCI_DEV_CODEC0,
      TISCI_DEV_CODEC0_VPU_ACLK_CLK,
      600000000
    },
    {
      TISCI_DEV_CODEC1,
      TISCI_DEV_CODEC1_VPU_ACLK_CLK,
      600000000
    },
};

static Board_PllClkCfg_t gBoardEcuPllClkCfgMain_powerConfig2[] =
{  // Fixed config
    {
      TISCI_DEV_GTC0,
      TISCI_DEV_GTC0_GTC_CLK_PARENT_POSTDIV3_16FFT_MAIN_0_HSDIVOUT6_CLK,
      200000000
    },
    // GUI configured
    {
      TISCI_DEV_A72SS0,
      TISCI_DEV_A72SS0_ARM0_CLK_CLK,
      2000000000
    },
    {
      TISCI_DEV_A72SS1,
      TISCI_DEV_A72SS1_ARM1_CLK_CLK,
      2000000000
    },
    {
      TISCI_DEV_R5FSS0_CORE0,
      TISCI_DEV_R5FSS0_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_R5FSS1_CORE0,
      TISCI_DEV_R5FSS1_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_R5FSS2_CORE0,
      TISCI_DEV_R5FSS2_CORE0_CPU_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS0_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS1_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS2_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0,
      TISCI_DEV_COMPUTE_CLUSTER0_C71SS3_CORE0_C7X_CLK,
      1000000000
    },
    {
      TISCI_DEV_VPAC0,
      TISCI_DEV_VPAC0_MAIN_CLK,
      720000000
    },
    {
      TISCI_DEV_VPAC1,
      TISCI_DEV_VPAC1_MAIN_CLK,
      720000000
    },
    {
      TISCI_DEV_DMPAC0,
      TISCI_DEV_DMPAC0_CLK,
      480000000
    },
    {
      TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0,
      TISCI_DEV_J7AEP_GPU_BXS464_WRAP0_GPU_SS_0_GPU_PLL_CLK,
      800000000
    },
    {
      TISCI_DEV_CODEC0,
      TISCI_DEV_CODEC0_VPU_ACLK_CLK,
      600000000
    },
    {
      TISCI_DEV_CODEC1,
      TISCI_DEV_CODEC1_VPU_ACLK_CLK,
      600000000
    },
};

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                        Function Declarations                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Internal Function Declarations                       */
/* ========================================================================== */

static Board_STATUS Board_ecuInitPll(uint32_t cfg);
static Board_STATUS Board_ecuInitClock(uint32_t cfg);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

Board_STATUS Board_ecuInit(uint32_t cfg)
{
    Board_STATUS status = BOARD_SOK;

    status = Board_ecuInitPll(cfg);
    if(status == BOARD_SOK)
    {
        status = Board_ecuInitClock(cfg);
    }

    return status;
}

/* ========================================================================== */
/*                       Internal Function Definitions                        */
/* ========================================================================== */

static Board_STATUS Board_ecuInitPll(uint32_t cfg)
{
    Board_PllClkCfg_t *pMcuArray = NULL;
    Board_PllClkCfg_t *pMainArray = NULL;
    uint32_t mcuArraySize = 0;
    uint32_t mainArraySize = 0;
    Board_STATUS status = BOARD_SOK;
    uint32_t index = 0;

   /* Select array to use based on config */
    switch(cfg)
    {
        case BOARD_ECU_NONE:
        {
          /* For ECU_NONE use default PLL init */
            status= Board_PLLInitMcu();

            if(status == BOARD_SOK)
            {
                status = Board_PLLInitMain();
            }
        }
        break;

        case BOARD_ECU_FC:
        {
            pMcuArray = gBoardEcuPllClkCfgMcu_powerConfig0;
            mcuArraySize = sizeof(gBoardEcuPllClkCfgMcu_powerConfig0) / sizeof(Board_PllClkCfg_t);

            pMainArray = gBoardEcuPllClkCfgMain_powerConfig0;
            mainArraySize = sizeof(gBoardEcuPllClkCfgMain_powerConfig0) / sizeof(Board_PllClkCfg_t);
        }
        break;

        case BOARD_ECU_SRV:
        {
            pMcuArray = gBoardEcuPllClkCfgMcu_powerConfig1;
            mcuArraySize = sizeof(gBoardEcuPllClkCfgMcu_powerConfig1) / sizeof(Board_PllClkCfg_t);

            pMainArray = gBoardEcuPllClkCfgMain_powerConfig1;
            mainArraySize = sizeof(gBoardEcuPllClkCfgMain_powerConfig1) / sizeof(Board_PllClkCfg_t);
        }
        break;

        case BOARD_ECU_AVP4:
        {
            pMcuArray = gBoardEcuPllClkCfgMcu_powerConfig2;
            mcuArraySize = sizeof(gBoardEcuPllClkCfgMcu_powerConfig2) / sizeof(Board_PllClkCfg_t);

            pMainArray = gBoardEcuPllClkCfgMain_powerConfig2;
            mainArraySize = sizeof(gBoardEcuPllClkCfgMain_powerConfig2) / sizeof(Board_PllClkCfg_t);
        }
        break;

        default:
        {
            status = BOARD_INVALID_PARAM;
        }
        break;
    }

    /* Set MCU PLLs */
    if (pMcuArray != NULL)
    {
        for (index = 0; index < mcuArraySize && status == BOARD_SOK; index++)
        {
            status = Board_PLLInit(pMcuArray[index].tisciDevID,
                                   pMcuArray[index].tisciClkID,
                                   pMcuArray[index].clkRate);
        }
    }

    /* Set Main PLLs */
    if (pMainArray != NULL)
    {
        for (index = 0; index < mainArraySize && status == BOARD_SOK; index++)
        {
            status = Board_PLLInit(pMainArray[index].tisciDevID,
                                   pMainArray[index].tisciClkID,
                                   pMainArray[index].clkRate);
        }
    }

    return status;
}

static Board_STATUS Board_ecuInitClock(uint32_t cfg)
{
    uint32_t *pMcuArray = NULL;
    uint32_t *pMainArray = NULL;
    uint32_t mcuArraySize = 0;
    uint32_t mainArraySize = 0;
    Board_STATUS status = BOARD_SOK;
    uint32_t index = 0;

    /* Select array to use based on config */
    switch (cfg)
    {
        case BOARD_ECU_NONE:
        {
          /* For ECU_NONE, set main and mcu clock groups to all groups */
            Board_initParams_t initParams;
            Board_getInitParams(&initParams);
            initParams.mainClkGrp = BOARD_MAIN_CLOCK_GROUP_ALL;
            initParams.mcuClkGrp  = BOARD_MCU_CLOCK_GROUP_ALL;
            Board_setInitParams(&initParams);

            Board_init(BOARD_INIT_MODULE_CLOCK);
        }
        break;

        case BOARD_ECU_FC:
        {
            pMcuArray = gBoardEcuClkModuleMcuIDInit_powerConfig0;
            mcuArraySize = sizeof(gBoardEcuClkModuleMcuIDInit_powerConfig0) / sizeof(uint32_t);

            pMainArray = gBoardEcuClkModuleMainIDInit_powerConfig0;
            mainArraySize = sizeof(gBoardEcuClkModuleMainIDInit_powerConfig0) / sizeof(uint32_t);
        }
        break;

        case BOARD_ECU_SRV:
        {
            pMcuArray = gBoardEcuClkModuleMcuIDInit_powerConfig1;
            mcuArraySize = sizeof(gBoardEcuClkModuleMcuIDInit_powerConfig1) / sizeof(uint32_t);

            pMainArray = gBoardEcuClkModuleMainIDInit_powerConfig1;
            mainArraySize = sizeof(gBoardEcuClkModuleMainIDInit_powerConfig1) / sizeof(uint32_t);
        }
        break;

        case BOARD_ECU_AVP4:
        {
            pMcuArray = gBoardEcuClkModuleMcuIDInit_powerConfig2;
            mcuArraySize = sizeof(gBoardEcuClkModuleMcuIDInit_powerConfig2) / sizeof(uint32_t);

            pMainArray = gBoardEcuClkModuleMainIDInit_powerConfig2;
            mainArraySize = sizeof(gBoardEcuClkModuleMainIDInit_powerConfig2) / sizeof(uint32_t);
        }
        break;

        default:
        {
            status = BOARD_INVALID_PARAM;
        }
        break;
    }

    /* Set MCU clocks */
    if (pMcuArray != NULL)
    {
        for (index = 0; index < mcuArraySize && status == BOARD_SOK; index++)
        {
            status = Board_moduleClockEnable(pMcuArray[index]);
        }
    }

    /* Set Main clocks */
    if (pMainArray != NULL)
    {
        for (index = 0; index < mainArraySize && status == BOARD_SOK; index++)
        {
            status = Board_moduleClockEnable(pMainArray[index]);
        }
    }

    return status;
}
