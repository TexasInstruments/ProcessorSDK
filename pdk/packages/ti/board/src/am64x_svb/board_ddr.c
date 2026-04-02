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

/** \file board_ddr.c
 *
 *  \brief This file used to configure the DDR timing parameters.
 *
 */

#include <string.h>
#include <soc.h>
#include "board_ddr.h"
#include "board_ddrRegInit.h"
#include "board_lpddr4RegInit.h"

/* Global variables */
static LPDDR4_Config gBoardDdrCfg;
static LPDDR4_PrivateData gBoardDdrPd;

/* Local function prototypes */
static int32_t emif_ConfigureECC(void);

#ifndef SIM_BUILD
/**
 * \brief   Set DDR PLL to bypass, efectively 20MHz or 19.2MHz (on silicon).
 *
 * \return  none
 */
static void Board_DDRSetPLLExtBypass(void)
{

    uint32_t addrOffset = 0x00000000;
    uint32_t baseAddr = CSL_PLL0_CFG_BASE;
    uint32_t regVal;
    uint32_t fieldVal;
    uint32_t regAddr;

    fieldVal = 1;
    regAddr = (baseAddr + addrOffset + (DDR_PLL_INDEX * 0x1000) + CSL_MAIN_PLL_MMR_CFG_PLL0_CTRL);
    regVal = HW_RD_REG32(regAddr);
    regVal |= (fieldVal << 31);
    HW_WR_REG32(regAddr, regVal);

}

#endif /* SIM_BUILD */

/**
 * \brief   Set DDR PLL clock value
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
static Board_STATUS Board_DDRSetPLLClock(void)
{
    Board_STATUS status = BOARD_SOK;

    status = Board_PLLInit(TISCI_DEV_DDR16SS0,
                           TISCI_DEV_DDR16SS0_DDRSS_DDR_PLL_CLK,
                           DDRSS_PLL_FREQUENCY_1);
    if(status != BOARD_SOK)
    {
        BOARD_DEBUG_LOG("Failed to Set the DDR PLL Clock Frequency\n");
    }

    return status;
}

/**
 * \brief   Controls the DDR PLL clock change sequence during inits
 *
 * \return  None
 */
static void Board_DDRChangeFreqAck(void)
{

    /* Configure PLL Clock */
    Board_DDRSetPLLClock();

    BOARD_DEBUG_LOG("--->>> DDR PLL clock configured ... <<<---\n");
}

/**
 * \brief   Function to handle the configuration requests from DDR lib
 *
 * \return  None
 */
static void Board_DDRInfoHandler(const LPDDR4_PrivateData *pd, LPDDR4_InfoType infotype)
{
    if (infotype == LPDDR4_DRV_SOC_PLL_UPDATE)
    {
        Board_DDRChangeFreqAck();
    }
}

/**
 * \brief   DDR probe function
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
static Board_STATUS Board_DDRProbe(void)
{
    uint32_t status = 0U;
    uint16_t configsize = 0U;

    status = LPDDR4_Probe(&gBoardDdrCfg, &configsize);

    if ((status != 0) || (configsize != sizeof(LPDDR4_PrivateData)) ||
        (configsize > BOARD_DDR_SRAM_MAX))
    {
        BOARD_DEBUG_LOG("Board_DDRProbe: FAIL\n");
        return BOARD_FAIL;
    }
    else
    {
        BOARD_DEBUG_LOG("Board_DDRProbe: PASS\n");
    }

    return BOARD_SOK;
}

/**
 * \brief   DDR driver initialization function
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
static Board_STATUS Board_DDRInitDrv(void)
{
    uint32_t status = 0U;

    if ((sizeof(gBoardDdrPd) != sizeof(LPDDR4_PrivateData)) ||
        (sizeof(gBoardDdrPd) > BOARD_DDR_SRAM_MAX))
    {
        BOARD_DEBUG_LOG("Board_DDRInitDrv: FAIL\n");
        return BOARD_FAIL;
    }

    gBoardDdrCfg.ctlBase = (struct LPDDR4_CtlRegs_s *)BOARD_DDR_CTL_CFG_BASE;
    gBoardDdrCfg.infoHandler = (LPDDR4_InfoCallback) Board_DDRInfoHandler;

    status = LPDDR4_Init(&gBoardDdrPd, &gBoardDdrCfg);

    if ((status > 0U) ||
        (gBoardDdrPd.ctlBase != (struct LPDDR4_CtlRegs_s *)gBoardDdrCfg.ctlBase) ||
        (gBoardDdrPd.ctlInterruptHandler != gBoardDdrCfg.ctlInterruptHandler) ||
        (gBoardDdrPd.phyIndepInterruptHandler != gBoardDdrCfg.phyIndepInterruptHandler))
    {
        BOARD_DEBUG_LOG("Board_DDRInitDrv: FAIL\n");
        return BOARD_FAIL;
    }
    else
    {
        BOARD_DEBUG_LOG("Board_DDRInitDrv: PASS\n");
    }

    return BOARD_SOK;
}

/**
 * \brief   DDR registers initialization function for DDR4.
 *
 * \return  BOARD_SOK in case of success or appropriate error code.
 */
static Board_STATUS Board_DDRHWRegInitDdr4(void)
{
    uint32_t status = 0U;

    /* VBUSM2AXI Control Register sdram_idx, region_idx 0x11 --> 0x0F = log2(connected SDRAM size) - 16 */
    HW_WR_REG32((CSL_DDR16SS0_SS_CFG_BASE + CSL_EMIF_SSCFG_V2A_CTL_REG),
                 (0xFU << CSL_EMIF_SSCFG_V2A_CTL_REG_SDRAM_IDX_SHIFT)
                 | (0xFU << CSL_EMIF_SSCFG_V2A_CTL_REG_REGION_IDX_SHIFT));


    status = LPDDR4_WriteCtlConfig(&gBoardDdrPd,
                                            DDRSS_ctlReg,
                                            DDRSS_ctlRegNum,
                                            (uint16_t)DDRSS_CTL_REG_INIT_COUNT);
    if (!status)
    {
        status = LPDDR4_WritePhyIndepConfig(&gBoardDdrPd,
                                                     DDRSS_phyIndepReg,
                                                     DDRSS_phyIndepRegNum,
                                                     (uint16_t)DDRSS_PHY_INDEP_REG_INIT_COUNT);
    }
    if (!status)
    {
        status = LPDDR4_WritePhyConfig(&gBoardDdrPd,
                                                DDRSS_phyReg,
                                                DDRSS_phyRegNum,
                                                (uint16_t)DDRSS_PHY_REG_INIT_COUNT);
    }

    if (status)
    {
        BOARD_DEBUG_LOG(" ERROR: Board_DDRHWRegInitDdr4 failed!!\n");
        return BOARD_FAIL;
    }
    return BOARD_SOK;
}

/**
 * \brief   DDR registers initialization function for LPDDR4.
 *
 * \return  BOARD_SOK in case of success or appropriate error code.
 */
static Board_STATUS Board_DDRHWRegInitLpddr4(void)
{
    uint32_t status = 0U;

    /* VBUSM2AXI Control Register sdram_idx, region_idx 0x11 --> 0x0F = log2(connected SDRAM size) - 16 */
    HW_WR_REG32((CSL_DDR16SS0_SS_CFG_BASE + CSL_EMIF_SSCFG_V2A_CTL_REG),
                 (0xFU << CSL_EMIF_SSCFG_V2A_CTL_REG_SDRAM_IDX_SHIFT)
                 | (0xFU << CSL_EMIF_SSCFG_V2A_CTL_REG_REGION_IDX_SHIFT));


    status = LPDDR4_WriteCtlConfig(&gBoardDdrPd,
                                            DDRSS_ctlReg_lpddr4,
                                            DDRSS_ctlRegNum_lpddr4,
                                            (uint16_t)DDRSS_CTL_REG_INIT_COUNT);
    if (!status)
    {
        status = LPDDR4_WritePhyIndepConfig(&gBoardDdrPd,
                                                     DDRSS_phyIndepReg_lpddr4,
                                                     DDRSS_phyIndepRegNum_lpddr4,
                                                     (uint16_t)DDRSS_PHY_INDEP_REG_INIT_COUNT);
    }
    if (!status)
    {
        status = LPDDR4_WritePhyConfig(&gBoardDdrPd,
                                                DDRSS_phyReg_lpddr4,
                                                DDRSS_phyRegNum_lpddr4,
                                                (uint16_t)DDRSS_PHY_REG_INIT_COUNT);
    }

    if (status)
    {
        BOARD_DEBUG_LOG(" ERROR: Board_DDRHWRegInitLpddr4 failed!!\n");
        return BOARD_FAIL;
    }
    return BOARD_SOK;
}

/**
 * \brief   DDR start function
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
static Board_STATUS Board_DDRStart(void)
{
    uint32_t status = 0U;
    uint32_t regval = 0U;
    uint32_t offset = 0U;

    offset = BOARD_DDR_CTL_REG_OFFSET;

    status = LPDDR4_ReadReg(&gBoardDdrPd, LPDDR4_CTL_REGS, offset, &regval);
    if ((status > 0U) || ((regval & 0x1U) != 0U))
    {
        BOARD_DEBUG_LOG("Board_DDRStart: FAIL\n");
        return BOARD_FAIL;
    }

    status = LPDDR4_Start(&gBoardDdrPd);
    if (status > 0U)
    {
        BOARD_DEBUG_LOG("Board_DDRStart: FAIL\n");
        return BOARD_FAIL;
    }

    status = LPDDR4_ReadReg(&gBoardDdrPd, LPDDR4_CTL_REGS, offset, &regval);
    if ((status > 0U) || ((regval & 0x1U) != 1U))
    {
        BOARD_DEBUG_LOG("Board_DDRStart: FAIL\n");
        return BOARD_FAIL;
    }
    else
    {
        BOARD_DEBUG_LOG("LPDDR4_Start: PASS\n");
    }

    return BOARD_SOK;
}

/**
 * \brief Configures DDR ECC
 *
 * Invokes EMIF CSL APIs to configure ECC and Primes the memory
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
/* Refer EMIF ECC Configuration Section in TRM */
static Board_STATUS emif_ConfigureECC(void)
{
    Board_STATUS   status    = BOARD_SOK;
    int32_t        cslResult = CSL_PASS;
    CSL_EmifConfig emifCfg;
    uintptr_t      memPtr;

    BOARD_DEBUG_LOG("\r\n Configuring ECC");

    memset(&emifCfg, 0, sizeof(emifCfg));

    emifCfg.bEnableMemoryECC = true;
    emifCfg.bReadModifyWriteEnable = true;
    emifCfg.bECCCheck = true;
    emifCfg.bWriteAlloc = true;
    emifCfg.ECCThreshold = 1U;
    emifCfg.pMemEccCfg.startAddr[0] = BOARD_DDR_START_ADDR-BOARD_SOC_DDR_START_ADDR;
    emifCfg.pMemEccCfg.endAddr[0] = BOARD_DDR_ECC_END_ADDR-BOARD_SOC_DDR_START_ADDR;
    cslResult = CSL_emifConfig((CSL_emif_sscfgRegs *)CSL_DDR16SS0_SS_CFG_BASE,
                               &emifCfg);

    if (cslResult != CSL_PASS)
    {
        BOARD_DEBUG_LOG("\r\n CSL_emifConfig Failed");
        status = BOARD_FAIL;
    }

    /* Prime the memory */
    if ( status == BOARD_SOK )
    {
        /* Prime memory with known pattern */
        for (memPtr = BOARD_DDR_START_ADDR; memPtr < BOARD_DDR_ECC_END_ADDR; memPtr += 4)
        {
            *((volatile uint32_t *) memPtr) = memPtr;
        }

        /* Make sure the write is complete by writeback */
        CacheP_wbInv((const void *)BOARD_DDR_START_ADDR, BOARD_DDR_ECC_END_ADDR-BOARD_DDR_START_ADDR);

        /* Clears ECC errors */
        CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)CSL_DDR16SS0_SS_CFG_BASE);
    }

    return status;
}

/**
 * \brief DDR4 Initialization function
 *
 * Initializes the DDR timing parameters. Sets the DDR timing parameters
 * based in the DDR PLL controller configuration done by the board library.
 * Any changes to DDR PLL requires change to DDR timing.
 *
 * \param   void
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_DDRInit(Bool eccEnable)
{
    Board_STATUS status = BOARD_SOK;
    Board_initParams_t initParams;
    uint32_t ddrType=0;

#ifndef SIM_BUILD
    /* PLL should be bypassed while configuring the DDR */
    Board_DDRSetPLLExtBypass();
#endif /* SIM_BUILD */

    Board_getInitParams(&initParams);
    if(initParams.ddrType == BOARD_DDR_AUTO)
    {
        /* Detecting the type of DDR connected */
        if(Board_detectBoard(BOARD_ID_LPDDR4))
        {
            ddrType = BOARD_ID_LPDDR4;
        }
        else if(Board_detectBoard(BOARD_ID_DDR4))
        {
            ddrType = BOARD_ID_DDR4;
        }
        else
        {
            status = BOARD_FAIL;
            return status;
        }
    }
    else if(initParams.ddrType == BOARD_DDR_DDR4)
    {
        ddrType = BOARD_ID_DDR4;
    }
    else if(initParams.ddrType == BOARD_DDR_LPDDR4)
    {
        ddrType = BOARD_ID_LPDDR4;
    }

    /* Partition5 lockkey0 */
    HW_WR_REG32((CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0),
                KICK0_UNLOCK);
    /* Partition5 lockkey1 */
    HW_WR_REG32((CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK1),
                KICK1_UNLOCK);

    status = Board_DDRProbe();
    if(status != BOARD_SOK)
    {
        return status;
    }

    status = Board_DDRInitDrv();
    if(status != BOARD_SOK)
    {
        return status;
    }

    if(ddrType == BOARD_ID_DDR4)
    {
        status = Board_DDRHWRegInitDdr4();
        if(status != BOARD_SOK)
        {
            return status;
        }
    }
    else if(ddrType == BOARD_ID_LPDDR4)
    {
        status = Board_DDRHWRegInitLpddr4();
        if(status != BOARD_SOK)
        {
            return status;
        }
    }

    status = Board_DDRStart();
    if(status != BOARD_SOK)
    {
        return status;
    }

    if (eccEnable == TRUE)
    {
         status = emif_ConfigureECC();
    }

    return status;
}
