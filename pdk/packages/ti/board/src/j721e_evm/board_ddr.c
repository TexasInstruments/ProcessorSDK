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
#include <string.h>
#include "board_ddr.h"
#include "board_ddrRegInit.h"

/* Global variables */
static LPDDR4_Config gBoardDdrCfg;
static LPDDR4_PrivateData gBoardDdrPd;

/* Local function prototypes */
static int32_t emif_ConfigureECC(void);

#ifdef BOARD_DDR_ENABLE_PLL_BYPASS
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

    fieldVal = 1U;
    regAddr = (baseAddr + addrOffset + (DDR_PLL_INDEX * 0x1000) + CONTROL);
    regVal = HW_RD_REG32(regAddr);
    regVal |= (fieldVal << 31);
    HW_WR_REG32(regAddr, regVal);
}
#endif

/**
 * \brief   Set DDR PLL clock value
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
static Board_STATUS Board_DDRSetPLLClock(uint64_t frequency)
{
    Board_STATUS status = BOARD_SOK;

    status = Board_PLLInit(TISCI_DEV_DDR0,
                           TISCI_DEV_DDR0_DDRSS_DDR_PLL_CLK,
                           frequency);
    if(BOARD_SOK != status)
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
    uint32_t reqType;
    uint32_t regVal;
    volatile uint32_t counter;
    volatile uint32_t temp = 0U;

    temp = temp;  /* To suppress compiler warning */
    BOARD_DEBUG_LOG("--->>> LPDDR4 Initialization is in progress ... <<<---\n");

    for(counter = 0U; counter < DDRSS_PLL_FHS_CNT; counter++)
    {
        /* wait for freq change request */
        regVal = HW_RD_REG32(BOARD_DDR_FSP_CLKCHNG_REQ_ADDR) & 0x80U;
        BOARD_DEBUG_LOG("Reg Value: %d \n", regVal);

        while(0x0U == regVal)
        {
            regVal = HW_RD_REG32(BOARD_DDR_FSP_CLKCHNG_REQ_ADDR) & 0x80U;
            BOARD_DEBUG_LOG("Reg Value: %d \n", regVal);
        }

        reqType = HW_RD_REG32(BOARD_DDR_FSP_CLKCHNG_REQ_ADDR) & 0x03U;
        BOARD_DEBUG_LOG("Frequency Change type %d request from Controller \n", reqType);

        if(1U == reqType)
        {
            Board_DDRSetPLLClock(DDRSS_PLL_FREQUENCY_1);
        }
        else if(2U == reqType)
        {
            Board_DDRSetPLLClock(DDRSS_PLL_FREQUENCY_2);
        }
        else if(0U == reqType)
        {
#ifndef BOARD_DDR_ENABLE_PLL_BYPASS
            Board_DDRSetPLLClock(DDRSS_PLL_FREQUENCY_0);
#else
            Board_DDRSetPLLExtBypass();
#endif
        }
        else
        {
            BOARD_DEBUG_LOG("Invalid Request Type\n");
        }

        /* Acknowledge frequency change request */
        HW_WR_REG32(BOARD_DDR_FSP_CLKCHNG_ACK_ADDR, 0x1);

        while(0x80U == (HW_RD_REG32(BOARD_DDR_FSP_CLKCHNG_REQ_ADDR) & 0x80U));

        /* Clear frequency change request acknowledge */
        HW_WR_REG32(BOARD_DDR_FSP_CLKCHNG_ACK_ADDR, 0x0);
    }

    BOARD_DEBUG_LOG("--->>> Frequency Change request handshake is completed... <<<---\n");
}

/**
 * \brief   Function to handle the configuration requests from DDR lib
 *
 * \return  None
 */
static void Board_DDRInfoHandler(const LPDDR4_PrivateData *pd, LPDDR4_InfoType infotype)
{
    if (LPDDR4_DRV_SOC_PLL_UPDATE == infotype)
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

    if ((CDN_EOK != status) || (configsize != sizeof(LPDDR4_PrivateData)) ||
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
 * \brief   DDR registers initialization function
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */
static Board_STATUS Board_DDRHWRegInit(void)
{
    uint32_t status = 0U;

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
        BOARD_DEBUG_LOG(" ERROR: Board_DDRHWRegInit failed!!\n");
        return BOARD_FAIL;
    }

    return BOARD_SOK;
}

/**
 * \brief   DDR priority init function
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 */

static void Board_DDRPriorityInit(void)
{
    /* Flatten out AXI priorities so that VBUSM.C priorities take effect */
    uint32_t regVal;
    uint32_t regAddr;

    regVal = 0U;
	regVal = (0x7U << 28) |
             (0x7U << 24) |
             (0x7U << 20) |
             (0x7U << 16) |
             (0x7U << 12) |
             (0x7U <<  8) |
             (0x7U <<  4) |
             (0x7U <<  0);

    regAddr = 0x02980030U; /* DDRSS_V2A_LPT_DEF_PRI_MAP_REG */
    HW_WR_REG32(regAddr, regVal);

    regAddr = 0x0298004CU; /* DDRSS_V2A_HPT_DEF_PRI_MAP_REG */
    HW_WR_REG32(regAddr, regVal);

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
    if ((CDN_EOK < status) || (0U != (regval & 0x1U)))
    {
        BOARD_DEBUG_LOG("Board_DDRStart: FAIL\n");
        return BOARD_FAIL;
    }

    status = LPDDR4_Start(&gBoardDdrPd);
    if (CDN_EOK < status)
    {
        BOARD_DEBUG_LOG("Board_DDRStart: FAIL\n");
        return BOARD_FAIL;
    }

    status = LPDDR4_ReadReg(&gBoardDdrPd, LPDDR4_CTL_REGS, offset, &regval);
    if ((CDN_EOK < status) || (1U != (regval & 0x1U)))
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

    BOARD_DEBUG_LOG("\r\n Configuring ECC");

    memset(&emifCfg, 0, sizeof(emifCfg));

    emifCfg.bEnableMemoryECC = BTRUE;
    emifCfg.bReadModifyWriteEnable = BTRUE;
    emifCfg.bECCCheck = BTRUE;
    emifCfg.bWriteAlloc = BTRUE;
    emifCfg.ECCThreshold = 1U;
    emifCfg.pMemEccCfg.startAddr[0] = BOARD_DDR_START_ADDR-BOARD_DDR_START_ADDR;
    emifCfg.pMemEccCfg.endAddr[0] = BOARD_DDR_ECC_END_ADDR-BOARD_DDR_START_ADDR;
    cslResult = CSL_emifConfig((CSL_emif_sscfgRegs *)CSL_COMPUTE_CLUSTER0_SS_CFG_BASE,
                               &emifCfg);

    if (CSL_PASS != cslResult)
    {
        BOARD_DEBUG_LOG("\r\n CSL_emifConfig Failed");
        status = BOARD_FAIL;
    }

    /* Prime the memory */
#ifdef BOARD_DDR_ENABLE_DDR_MEM_PRIME
    if ( BOARD_SOK == status )
    {
        status = BOARD_udmaPrimeDDR((void *)BOARD_DDR_START_ADDR, BOARD_DDR_ECC_END_ADDR-BOARD_DDR_START_ADDR+1U);
    }
#else
    BOARD_DEBUG_LOG("\r\n DDR Memory is not primed (BOARD_DDR_ENABLE_DDR_MEM_PRIME is disabled)");
#endif
    if ( BOARD_SOK == status )
    {
        /* Clears ECC errors */
        CSL_emifClearAllECCErrors((CSL_emif_sscfgRegs *)CSL_COMPUTE_CLUSTER0_SS_CFG_BASE);
    }

    return status;
}

/**
 * \brief DDR4 Initialization function
 *
 * Invokes DDR CSL APIs to configure the DDR timing parameters and ECC configuration
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_DDRInit(Bool eccEnable)
{
    Board_STATUS status = BOARD_SOK;

    /* Unlock the PLL register access for DDR clock bypass */
    HW_WR_REG32(BOARD_PLL12_LOCK0, KICK0_UNLOCK);
    HW_WR_REG32(BOARD_PLL12_LOCK1, KICK1_UNLOCK);

#ifdef BOARD_DDR_ENABLE_PLL_BYPASS
    /* Bypass PLL while configuring the DDR */
    Board_DDRSetPLLExtBypass();
#else
    /* Set to Boot Frequency(F0) while configuring the DDR */
    Board_DDRSetPLLClock(DDRSS_PLL_FREQUENCY_0);
#endif

    /* Partition5 lockkey0 */
    HW_WR_REG32(BOARD_CTRL_MMR_PART5_LOCK0, KICK0_UNLOCK);
    /* Partition5 lockkey1 */
    HW_WR_REG32(BOARD_CTRL_MMR_PART5_LOCK1, KICK1_UNLOCK);

    status = Board_DDRProbe();
    if(BOARD_SOK != status)
    {
        return status;
    }

    status = Board_DDRInitDrv();
    if(BOARD_SOK != status)
    {
        return status;
    }

    status = Board_DDRHWRegInit();
    if(BOARD_SOK != status)
    {
        return status;
    }

    Board_DDRPriorityInit();
    
    status = Board_DDRStart();
    if(BOARD_SOK != status)
    {
        return status;
    }

    if (UTRUE == eccEnable)
    {
         status = emif_ConfigureECC();
    }

    /* Lock the PLL registers access */
    HW_WR_REG32(BOARD_PLL12_LOCK0, KICK_LOCK);
    HW_WR_REG32(BOARD_PLL12_LOCK1, KICK_LOCK);

    return status;
}
