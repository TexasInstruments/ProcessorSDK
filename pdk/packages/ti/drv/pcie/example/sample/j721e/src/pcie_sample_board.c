/*  ============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2018-2019
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
 * @file pcie_example_board.h
 *
 */

#include "pcie_sample.h"
#include "pcie_sample_board.h"
#include <ti/csl/soc.h>
#include <ti/csl/csl_serdes.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/csl_serdes_pcie.h>
#include <ti/csl/cslr_pcie.h>
#include <ti/csl/soc/j721e/src/cslr_soc_ctrl_mmr.h>

#include <ti/osal/osal.h>
#include <ti/drv/pcie/pcie.h>
#include <ti/drv/pcie/src/pcieloc.h>
#include <ti/drv/pcie/src/v3/pcieloc.h>
#include <ti/drv/sciclient/sciclient.h>

#include <string.h>

#include <stdint.h>
#include <stdbool.h>

/* UART Header files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#if defined (BUILD_MPU)
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/v8a/Mmu.h>

#endif

#ifdef BUILD_MPU
static uint32_t msi_ints = 0, intx_ints = 0, unknown_ints = 0;
SemaphoreP_Handle semaphoreHandle;
static void PlatformMsiIsr (uintptr_t vhandle)
{
    /* Interrupt is already demuxed as it is specific SPI */
    msi_ints++;

    /* Tell user task ISR happend */
    SemaphoreP_postFromISR (semaphoreHandle);
}

static void PlatformIntxIsr (uintptr_t vhandle)
{
    /* Interrupt is already demuxed as it is specific SPI */
    intx_ints++;

    /* Tell user task ISR happend */
    SemaphoreP_postFromISR (semaphoreHandle);
}

void *pcieMsiHwi;
void *pcieIntxHwi;

SemaphoreP_Handle PlatformSetupMSIAndINTX (Pcie_Handle handle)
{
    OsalRegisterIntrParams_t           interruptRegParams;
    pcieRet_e                          retVal;
    pcieRegisters_t                    regs;
    pcieRegisters_t                    epRegs;
    pcieMsiCapReg_t                    epMsiCap;
    pcieMsiLo32Reg_t                   epMsiLowAddress;
    pcieMsiUp32Reg_t                   epMsiUpAddress;
    pcieMsiDataReg_t                   epMsiDataVal;
    pcieLegacyIrqEnableSetReg_t        rcLegacyEnable;
    uint32_t                           i;

    /* Create a semaphore for user task to wait for interrupt */
    semaphoreHandle = SemaphoreP_create (0, NULL);
    if (!semaphoreHandle)
    {
        PCIE_logPrintf("Failed to create semaphore\n");
        exit(1);
    }
    memset (&regs, 0, sizeof(regs));
    memset (&epRegs, 0, sizeof(epRegs));
    memset (&epMsiCap, 0, sizeof(epMsiCap));
    memset (&epMsiLowAddress, 0, sizeof(epMsiLowAddress));
    memset (&epMsiUpAddress, 0, sizeof(epMsiUpAddress));
    memset (&epMsiDataVal, 0, sizeof(epMsiDataVal));

    /* Read existing EP registers */
    epRegs.msiCap = &epMsiCap;
    epRegs.msiLo32 = &epMsiLowAddress;
    epRegs.msiUp32 = &epMsiUpAddress;
    epRegs.msiData = &epMsiDataVal;
    retVal = Pcie_readRegs (handle, pcie_LOCATION_REMOTE, &epRegs);
    if (retVal != pcie_RET_OK)
    {
        PCIE_logPrintf("read of EP interrupt regs failed (%d)\n", retVal);
        exit(1);
    }

    /* Enable MSI on EP */
    epMsiCap.msiEn = 1;
    epMsiDataVal.data = PCIE_WINDOW_MSI_DATA;
    epMsiUpAddress.addr = 0;
    epMsiLowAddress.addr = (PCIE_PCIE_MSI_BASE+PCIE_PCIE_MSI_OFF)>>2; /* because lld wants upper 30 bits only */
    retVal = Pcie_writeRegs (handle, pcie_LOCATION_REMOTE, &epRegs);
    if (retVal != pcie_RET_OK)
    {
        PCIE_logPrintf("write of EP interrupt regs failed (%d)\n", retVal);
        exit(1);
    }

    /* Enable legacy on RC */
    for (i=0; i<4; i++)
    {
       memset (&rcLegacyEnable, 0, sizeof(rcLegacyEnable));
       regs.legacyIrqEnableSet[i] = &rcLegacyEnable;

       /* read current */
       retVal = Pcie_readRegs (handle, pcie_LOCATION_LOCAL, &regs);
       if (retVal != pcie_RET_OK)
       {
           PCIE_logPrintf("Can't read legacy enable\n");
           exit(1);
       }
       rcLegacyEnable.legacyIrqEnSet = 1;

       /* write back */
       retVal = Pcie_writeRegs (handle, pcie_LOCATION_LOCAL, &regs);
       if (retVal != pcie_RET_OK)
       {
           PCIE_logPrintf("Can't write legacy enable\n");
           exit(1);
       }
    }

    /*   Construct Hwi object for this peripheral. */
    /*   Initialize with defaults. */
    Osal_RegisterInterrupt_initParams(&interruptRegParams);

    /* Populate the interrupt parameters */
    interruptRegParams.corepacConfig.arg = (uintptr_t)handle;
    interruptRegParams.corepacConfig.name = "PCIE_MSI";
    interruptRegParams.corepacConfig.isrRoutine = PlatformMsiIsr;
    interruptRegParams.corepacConfig.priority = 0x01U;
    interruptRegParams.corepacConfig.corepacEventNum = 0;               /* Only needed for c66 */
    interruptRegParams.corepacConfig.intVecNum = PCIE_SPI_BASE;
    interruptRegParams.corepacConfig.triggerSensitivity = OSAL_ARM_GIC_TRIG_TYPE_EDGE;

    Osal_RegisterInterrupt(&interruptRegParams, &pcieMsiHwi);

    if (! pcieMsiHwi)
    {
        PCIE_logPrintf("Hwi create (MSI) failed\n");
        exit(1);
    }

    /* Populate the interrupt parameters */
    interruptRegParams.corepacConfig.arg = (uintptr_t)handle;
    interruptRegParams.corepacConfig.name = "PCIE_INTX";
    interruptRegParams.corepacConfig.isrRoutine = PlatformIntxIsr;
    interruptRegParams.corepacConfig.priority = 0x01U;
    interruptRegParams.corepacConfig.corepacEventNum = 0;               /* Only needed for c66 */
    interruptRegParams.corepacConfig.intVecNum = CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_PCIE1_PCIE_LEGACY_PULSE_0;
    interruptRegParams.corepacConfig.triggerSensitivity = OSAL_ARM_GIC_TRIG_TYPE_EDGE;

    Osal_RegisterInterrupt(&interruptRegParams, &pcieIntxHwi);

    if (! pcieIntxHwi)
    {
        PCIE_logPrintf("Hwi create (INTX) failed\n");
        exit(1);
    }

    return semaphoreHandle;
}

void PlatformGetInts (uint32_t *msis, uint32_t *intx, uint32_t *unknowns)
{
    if (msis)
    {
        *msis = msi_ints;
    }
    if (intx)
    {
        *intx = intx_ints;
    }
    if (unknowns)
    {
        *unknowns = unknown_ints;
    }
}
#endif /* #ifdef BUILD_MPU */

typedef enum /* updated for J7 */
{
    SERDES_DIAG_PCIE_GEN1 = 1,

    SERDES_DIAG_PCIE_GEN2 = 2,

    SERDES_DIAG_PCIE_GEN3 = 3,

    SERDES_DIAG_PCIE_GEN4 = 4
} SERDES_DIAG_PCIE_TYPE;

#if defined(GEN3)
#define SERDES_DIAG_TEST_PCIE_GEN_TYPE          SERDES_DIAG_PCIE_GEN3 /* Generation to be tested */
#else
#define SERDES_DIAG_TEST_PCIE_GEN_TYPE          SERDES_DIAG_PCIE_GEN2 /* Generation to be tested */
#endif
#define SERDES_DIAG_TEST_NUM_LANES              (0x2) /* Number of lanes to be tested */
#define DMA_TRANSFER                            (0) /* Use DMA or CPU */
#define NAV_INSTANCE                            (0) /* NAV_INSTANCE = 0, navss512m | NAV_INSTANCE = 1, mcuNavss */
#define PCIE_INSTANCE_EP                        (0) /* Instance to be End-Point */
#define PCIE_INSTANCE_RP                        (1) /* Instance to be Root-Point */
#define PCIE_BYTE_CNT                           (1024*1024*8) /* Bytes to be transferred */

/*            vvv  Leave these alone unless you know what you're doing  vvv           */

#if PCIE_INSTANCE_EP == 0
    #define SERDES_DIAG_TEST_BASE_ADDR_EP       CSL_SERDES_16G0_BASE /* Serdes base address */
#elif PCIE_INSTANCE_EP == 1
    #define SERDES_DIAG_TEST_BASE_ADDR_EP       CSL_SERDES_16G1_BASE /* Serdes base address */
#elif PCIE_INSTANCE_EP == 2
    #define SERDES_DIAG_TEST_BASE_ADDR_EP       CSL_SERDES_16G2_BASE /* Serdes base address */
#elif PCIE_INSTANCE_EP == 3
    #define SERDES_DIAG_TEST_BASE_ADDR_EP       CSL_SERDES_16G3_BASE /* Serdes base address */
#endif
#if PCIE_INSTANCE_RP == 0
    #define SERDES_DIAG_TEST_BASE_ADDR_RP       CSL_SERDES_16G0_BASE /* Serdes base address */
#elif PCIE_INSTANCE_RP == 1
    #define SERDES_DIAG_TEST_BASE_ADDR_RP       CSL_SERDES_16G1_BASE /* Serdes base address */
#elif PCIE_INSTANCE_RP == 2
    #define SERDES_DIAG_TEST_BASE_ADDR_RP       CSL_SERDES_16G2_BASE /* Serdes base address */
#elif PCIE_INSTANCE_RP == 3
    #define SERDES_DIAG_TEST_BASE_ADDR_RP       CSL_SERDES_16G3_BASE /* Serdes base address */
#endif
#define serdes_diag_test_phy_type               TEST_SERDES_PCIe
#define SERDES_DIAG_TEST_REF_CLOCK              CSL_SERDES_REF_CLOCK_100M /* Ref clock of serdes */
#define SERDES_DIAG_TEST_SSC_MODE               CSL_SERDES_NO_SSC
#define SERDES_DIAG_TEST_LANE_MASK              0x3 /* All lanes are set */
#define SERDES_DIAG_TEST_PHY_TYPE               CSL_SERDES_PHY_TYPE_PCIe /* For running PCIe tests */
#define SERDES_DIAG_TEST_PRBS_PATTERN           SERDES_PRBS_7 /* prbs7 pattern */
#define SERDES_DIAG_TEST_LANE_RATE              CSL_SERDES_LANE_FULL_RATE /* Set to run at full rate of the link rate */
#define SERDES_DIAG_TEST_LOOPBACK_MODE          CSL_SERDES_LOOPBACK_DISABLED /* For internal near end serial loopback tests */
#define SERDES_DIAG_TEST_OPERATING_MODE         CSL_SERDES_FUNCTIONAL_MODE /* Should always be set to Diagnostic Mode for BER, EYE and PRBS tests */
#define SERDES_DIAG_TEST_FORCEATTBOOST          CSL_SERDES_FORCE_ATT_BOOST_DISABLED
#define SERDES_DIAG_TEST_REF_CLOCK_SRC          CSL_SERDES_REF_CLOCK_INT /* Internally sourced ref clk */
#define TRCNT                                   (16) /* Number of ring entries */
#define TRSIZE                                  (8) /* Size (in bytes) of each ring entry (for 48-bit packet descriptor ptr) */

#if SERDES_DIAG_TEST_PCIE_GEN_TYPE == SERDES_DIAG_PCIE_GEN1
    #define SERDES_DIAG_TEST_LINK_RATE            CSL_SERDES_LINK_RATE_3p125G /* Link rate of serdes */
#elif SERDES_DIAG_TEST_PCIE_GEN_TYPE == SERDES_DIAG_PCIE_GEN2
    #define SERDES_DIAG_TEST_LINK_RATE            CSL_SERDES_LINK_RATE_5G /* Link rate of serdes */
#elif SERDES_DIAG_TEST_PCIE_GEN_TYPE == SERDES_DIAG_PCIE_GEN3
    #define SERDES_DIAG_TEST_LINK_RATE            CSL_SERDES_LINK_RATE_8G /* Link rate of serdes */
#elif SERDES_DIAG_TEST_PCIE_GEN_TYPE == SERDES_DIAG_PCIE_GEN4
    #define SERDES_DIAG_TEST_LINK_RATE            CSL_SERDES_LINK_RATE_16G /* Link rate of serdes */
#endif

/* define the unlock and lock values */
#define KICK0_UNLOCK_VAL 0x68EF3490
#define KICK1_UNLOCK_VAL 0xD172BC5A
#define KICK_LOCK_VAL    0x00000000

#define MAIN_MMR_BASE_ADDRESS   CSL_CTRL_MMR0_CFG0_BASE
#define MCU_MMR_BASE_ADDRESS    CSL_MCU_CTRL_MMR0_CFG0_BASE
#define WKUP_MMR_BASE_ADDRESS   CSL_WKUP_CTRL_MMR0_CFG0_BASE
#define MAIN_PLL_BASE_ADDRESS    CSL_PLL0_CFG_BASE
#define MCU_PLL_BASE_ADDRESS    CSL_MCU_PLL0_CFG_BASE

#define MAIN_CTRL_ACSPCIE0_CTRL                 (0x18090)
#define MAIN_CTRL_ACSPCIE1_CTRL                 (0x18094)

void pcie_refclk_to_io(uint32_t serdesInstance, uint32_t ref_clk){
  switch(serdesInstance){
      case 0:
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK0_CLKSEL) = ref_clk;
          /* Enable output clock */
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK0_CLKSEL) |= 0x100;
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + MAIN_CTRL_ACSPCIE0_CTRL) &= 0xFFFFFFFC;
          break;
      case 1:
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK1_CLKSEL) = ref_clk;
          /* Enable output clock */
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK1_CLKSEL) |= 0x100;
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + MAIN_CTRL_ACSPCIE0_CTRL) &= 0xFFFFFFFC;
          break;
      case 2:
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK2_CLKSEL) = ref_clk;
          /* Enable output clock */
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK2_CLKSEL) |= 0x100;
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + MAIN_CTRL_ACSPCIE1_CTRL) &= 0xFFFFFFFC;
          break;
      case 3:
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK3_CLKSEL) = ref_clk;
          /* Enable output clock */
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + CSL_MAIN_CTRL_MMR_CFG0_PCIE_REFCLK3_CLKSEL) |= 0x100;
          *(uint32_t *)(CSL_CTRL_MMR0_CFG0_BASE + MAIN_CTRL_ACSPCIE1_CTRL) &= 0xFFFFFFFC;
          break;
      default:
          break;
      }
}

/* *********************** SRIS ENABLE DISABLE ****************************** */
void sris_control(Pcie_Handle handle, uint32_t enable){
  Pcie_DeviceCfgBaseAddr *cfg = pcie_handle_to_cfg (handle);
  Pciev3_DevParams *params = (Pciev3_DevParams*)cfg->devParams;

  CSL_user_cfgRegs *userCfg = (CSL_user_cfgRegs *)params->userCfgBase;
  uint32_t val = userCfg->INITCFG;

  pcie_setbits(val, CSL_USER_CFG_INITCFG_SRIS_ENABLE,     enable);

  userCfg->INITCFG = val;
}

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

uint32_t MAIN_CTRL_MMR_unlock_all()
{
    /* initialize the status variable */
    uint32_t status = 1;
    /* Unlock the 0th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK1));
    /* Unlock the 1st partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK1));
    /* Unlock the 2nd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK1));
    /* Unlock the 3rd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK1));
    /* Unlock the 4th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK1));
/* These two not in the CSL yet
   status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK1));
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK1));
*/
    /* Unlock the 7th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK1));
    /* Return status to calling program */
    return status;
}

uint32_t MAIN_CTRL_MMR_lock_all()
{
    /* create status return variable */
    uint32_t status = 1;
    /* lock the 0th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK0_KICK1));
    /* lock the 1st partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK1_KICK1));
    /* lock the 2nd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK2_KICK1));
    /* lock the 3rd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK3_KICK1));
    /* lock the 4th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK4_KICK1));
/* These two not in the CSL yet
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK5_KICK1));
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK6_KICK1));
*/
    /* lock the 7th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK0),
            (uint32_t *)(MAIN_MMR_BASE_ADDRESS
                    + CSL_MAIN_CTRL_MMR_CFG0_LOCK7_KICK1));
    return status;

}

uint32_t MAIN_PLL_MMR_unlock_all()
{
    /* initialize the status variable */
    uint32_t status = 1;
    /* Unlock the 0th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL0_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL0_LOCKKEY1));
    /* Unlock the 1st partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL1_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL1_LOCKKEY1));
    /* Unlock the 2nd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL2_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL2_LOCKKEY1));
    /* Unlock the 3rd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL3_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL3_LOCKKEY1));
    /* Unlock the 4th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL4_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL4_LOCKKEY1));
    /* Unlock the 5th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL5_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL5_LOCKKEY1));
    /* Unlock the 6th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL6_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL6_LOCKKEY1));
    /* Unlock the 7th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL7_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL7_LOCKKEY1));
    /* Unlock the 8th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL8_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL8_LOCKKEY1));
    /* Unlock the 12th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY1));
    /* Unlock the 13th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL13_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL13_LOCKKEY1));
    /* Unlock the 14th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL14_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL14_LOCKKEY1));
    /* Unlock the 15th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL15_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL15_LOCKKEY1));
    /* Unlock the 16th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL16_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL16_LOCKKEY1));
    /* Unlock the 17th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL17_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL17_LOCKKEY1));
    /* Unlock the 18th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL18_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL18_LOCKKEY1));
    /* Unlock the 19th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL19_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL19_LOCKKEY1));
    /* Unlock the 23rd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL23_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL23_LOCKKEY1));
    /* Unlock the 24th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL24_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL24_LOCKKEY1));
    /* Unlock the 25th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL25_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL25_LOCKKEY1));
    /* Return status to calling program */
    return status;
}

uint32_t MAIN_PLL_MMR_lock_all()
{
    /* initialize the status variable */
    uint32_t status = 1;
    /* Lock the 0th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL0_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL0_LOCKKEY1));
    /* Lock the 1st partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL1_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL1_LOCKKEY1));
    /* Lock the 2nd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL2_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL2_LOCKKEY1));
    /* Lock the 3rd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL3_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL3_LOCKKEY1));
    /* Lock the 4th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL4_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL4_LOCKKEY1));
    /* Lock the 5th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL5_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL5_LOCKKEY1));
    /* Lock the 6th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL6_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL6_LOCKKEY1));
    /* Lock the 7th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL7_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL7_LOCKKEY1));
    /* Lock the 8th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL8_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL8_LOCKKEY1));
    /* Lock the 12th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL12_LOCKKEY1));
    /* Lock the 13th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL13_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL13_LOCKKEY1));
    /* Lock the 14th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL14_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL14_LOCKKEY1));
    /* Lock the 15th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL15_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL15_LOCKKEY1));
    /* Lock the 16th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL16_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL16_LOCKKEY1));
    /* Lock the 17th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL17_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL17_LOCKKEY1));
    /* Lock the 18th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL18_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL18_LOCKKEY1));
    /* Lock the 19th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL19_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL19_LOCKKEY1));
    /* Lock the 23rd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL23_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL23_LOCKKEY1));
    /* Lock the 24th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL24_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL24_LOCKKEY1));
    /* Lock the 25th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL25_LOCKKEY0),
            (uint32_t *)(MAIN_PLL_BASE_ADDRESS
                    + CSL_MAIN_PLL_MMR_CFG_PLL25_LOCKKEY1));
    /* Return status to calling program */
    return status;
}

uint32_t MCU_CTRL_MMR_unlock_all()
{

    /* initialize the status variable */
    uint32_t status = 1;

    /* Unlock the 0th partition */
    status &= MMR_unlock_one(
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0),
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK1));

    /* Unlock the 1st partition */
    status &= MMR_unlock_one(
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK0),
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK1));

    /* Unlock the 2nd partition */
    status &= MMR_unlock_one(
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0),
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK1));

    /* Unlock the 3rd partition */
    status &= MMR_unlock_one(
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK0),
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK1));

    /* Unlock the 4th partition */
    status &= MMR_unlock_one(
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK0),
            (uint32_t *)( MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK1));
    return status;
}

uint32_t MCU_CTRL_MMR_lock_all()
{

    /* initialize the status variable */
    uint32_t status = 1;

    /* lock the 0th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK0),
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK0_KICK1));

    /* lock the 1st partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK0),
            (uint32_t *) (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK1_KICK1));
    /* lock the 2nd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK0),
            (uint32_t *) (MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK2_KICK1));
    /* lock the 3rd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK0),
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK3_KICK1));
    /* lock the 4th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK0),
            (uint32_t *)(MCU_MMR_BASE_ADDRESS
                    + CSL_MCU_CTRL_MMR_CFG0_LOCK4_KICK1));
    return status;
}

uint32_t MCU_PLL_MMR_unlock_all()
{
    /* initialize the status variable */
    uint32_t status = 1;
    /* Unlock the 0th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL0_LOCKKEY0),
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL0_LOCKKEY1));
    /* Unlock the 1st partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY0),
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY1));
    /* Unlock the 2nd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL2_LOCKKEY0),
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL2_LOCKKEY1));
    /* Return status to calling program */
    return status;
}

uint32_t MCU_PLL_MMR_lock_all()
{
    /* initialize the status variable */
    uint32_t status = 1;
    /* lock the 0th partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL0_LOCKKEY0),
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL0_LOCKKEY1));
    /* lock the 1st partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY0),
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL1_LOCKKEY1));
    /* lock the 2nd partition */
    status &= MMR_lock_one(
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL2_LOCKKEY0),
            (uint32_t *)(MCU_PLL_BASE_ADDRESS
                    + CSL_MCU_PLL_MMR_CFG_PLL2_LOCKKEY1));
    /* Return status to calling program */
    return status;
}

uint32_t WKUP_CTRL_MMR_unlock_all()
{

    /* initialize the status variable */
    uint32_t status = 1;

    /* Unlock the 0th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK0_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK0_KICK1));

    /* Unlock the 1st partition */
    status &= MMR_unlock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK1_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK1_KICK1));

    /* Unlock the 2nd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK2_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK2_KICK1));

    /* Unlock the 3rd partition */
    status &= MMR_unlock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK3_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK3_KICK1));

    /* Unlock the 4th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK4_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK4_KICK1));

    /* Unlock the 6th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK1));

    /* Unlock the 7th partition */
    status &= MMR_unlock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK7_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK7_KICK1));

    return status;
}

uint32_t WKUP_CTRL_MMR_lock_all()
{

    /* initialize the status variable */
    uint32_t status = 1;

    /* lock the 0th partition */
    status &= MMR_lock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK0_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK0_KICK1));
    /* lock the 1st partition */
    status &= MMR_lock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK1_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK1_KICK1));
    /* lock the 2nd partition */
    status &= MMR_lock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK2_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK2_KICK1));
    /* lock the 3rd partition */
    status &= MMR_lock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK3_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK3_KICK1));
    /* lock the 4th partition */
    status &= MMR_lock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK4_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK4_KICK1));
    /* lock the 6th partition */
    status &= MMR_lock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK6_KICK1));
    /* lock the 7th partition */
    status &= MMR_lock_one(
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK7_KICK0),
            (uint32_t *)(WKUP_MMR_BASE_ADDRESS
                    + CSL_WKUP_CTRL_MMR_CFG0_LOCK7_KICK1));

    return status;
}

uint32_t serdes_init(uint8_t serdesInstance, CSL_SerdesSSCMode SSC_Mode){
    CSL_SerdesResult status;
    uint32_t i, laneNum;
    CSL_SerdesLaneEnableParams serdesLaneEnableParams;
    CSL_SerdesLaneEnableStatus laneRetVal = CSL_SERDES_LANE_ENABLE_NO_ERR;
    /* SERDES_DIAG_PCIE_TYPE pcie_gen_type;  We can use use link rate to determine the gen type in code */

    if(serdesInstance > 3){
        PCIE_logPrintf("Invalid Serdes!\n");
        return 1;
    }

    memset(&serdesLaneEnableParams, 0, sizeof(serdesLaneEnableParams));
    serdesLaneEnableParams.serdesInstance = (CSL_SerdesInstance)serdesInstance;

    switch(serdesInstance){
        case 0:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G0_BASE;
            break;
        case 1:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G1_BASE;
            break;
        case 2:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G2_BASE;
            break;
        case 3:
            serdesLaneEnableParams.baseAddr = CSL_SERDES_16G3_BASE;
            break;
    }

    serdesLaneEnableParams.refClock         = SERDES_DIAG_TEST_REF_CLOCK;
    serdesLaneEnableParams.refClkSrc        = SERDES_DIAG_TEST_REF_CLOCK_SRC;
    serdesLaneEnableParams.linkRate         = SERDES_DIAG_TEST_LINK_RATE;
    serdesLaneEnableParams.numLanes         = SERDES_DIAG_TEST_NUM_LANES;
    serdesLaneEnableParams.laneMask         = SERDES_DIAG_TEST_LANE_MASK;
    serdesLaneEnableParams.SSC_mode         = SSC_Mode;
    serdesLaneEnableParams.phyType          = SERDES_DIAG_TEST_PHY_TYPE;
    serdesLaneEnableParams.pcieGenType      = SERDES_DIAG_TEST_PCIE_GEN_TYPE;
    serdesLaneEnableParams.operatingMode    = SERDES_DIAG_TEST_OPERATING_MODE;
    serdesLaneEnableParams.phyInstanceNum   = serdesInstance;
    for(i=0; i< serdesLaneEnableParams.numLanes; i++){
      serdesLaneEnableParams.laneCtrlRate[i] = SERDES_DIAG_TEST_LANE_RATE;
      serdesLaneEnableParams.loopbackMode[i] = SERDES_DIAG_TEST_LOOPBACK_MODE; /* still have to change to correct loopback mode */
    }
    /* pcie_gen_type = SERDES_DIAG_TEST_PCIE_GEN_TYPE; */

    CSL_serdesPorReset(serdesLaneEnableParams.baseAddr);

    /* Select the IP type, IP instance num, Serdes Lane Number */
    for (laneNum=0; laneNum < serdesLaneEnableParams.numLanes; laneNum++){
        CSL_serdesIPSelect(CSL_CTRL_MMR0_CFG0_BASE,
                           serdesLaneEnableParams.phyType,
                           serdesLaneEnableParams.phyInstanceNum,
                           serdesLaneEnableParams.serdesInstance,
                           laneNum);
    }

    /* selects the appropriate clocks for all serdes based on the protocol chosen */
     status = CSL_serdesRefclkSel(CSL_CTRL_MMR0_CFG0_BASE,
                                  serdesLaneEnableParams.baseAddr,
                                  serdesLaneEnableParams.refClock,
                                  serdesLaneEnableParams.refClkSrc,
                                  serdesLaneEnableParams.serdesInstance,
                                  serdesLaneEnableParams.phyType);

     /* Assert PHY reset and disable all lanes */
     CSL_serdesDisablePllAndLanes(serdesLaneEnableParams.baseAddr, serdesLaneEnableParams.numLanes, serdesLaneEnableParams.laneMask);

    /*Load the Serdes Config File */
    status = CSL_serdesPCIeInit(&serdesLaneEnableParams); /* Use this for PCIe serdes config load */

      /* Return error if input params are invalid */
    if (status != CSL_SERDES_NO_ERR){
        PCIE_logPrintf("Invalid SERDES Init Params \n");
    }

    /* Set this to standard mode defined by Cadence */
    for (laneNum=0; laneNum < serdesLaneEnableParams.numLanes; laneNum++){
        CSL_serdesPCIeModeSelect(serdesLaneEnableParams.baseAddr, serdesLaneEnableParams.pcieGenType, laneNum);
    }

    /* Common Lane Enable API for lane enable, pll enable etc */
    laneRetVal = CSL_serdesLaneEnable(&serdesLaneEnableParams);

    if (laneRetVal != 0){
        PCIE_logPrintf("Invalid Serdes Lane Enable\n");
        return 2;
    }

    PCIE_logPrintf("Serdes %d Init Complete\n", serdesInstance);
    return 0;
}

void PlatformPCIESSSerdesConfig(int32_t serdes, int32_t iface)
{
  int32_t sciStatus = CSL_EFAIL;
  Sciclient_ConfigPrms_t        sciConfig =
  {
    SCICLIENT_SERVICE_OPERATION_MODE_POLLED,
  };
  const Sciclient_ReqPrm_t      sciReqPrm =
  {
    TISCI_MSG_VERSION,
    TISCI_MSG_FLAG_AOP,
    (uint8_t *)NULL,
    0,
    SCICLIENT_SERVICE_WAIT_FOREVER
  };

  struct tisci_msg_version_resp sciResponse;
  Sciclient_RespPrm_t           sciRespPrm =
  {
    0,
    (uint8_t *) &sciResponse,
    (uint32_t)sizeof (sciResponse)
  };

  sciStatus = Sciclient_init(&sciConfig);
  if (sciStatus != CSL_PASS)
  {
    PCIE_logPrintf("SYSFW init ...FAILED\n");
    exit(1);
  }
  /* Check that sciclient is working (tap the mic) */
  sciStatus = Sciclient_service(&sciReqPrm, &sciRespPrm);
  if (CSL_PASS == sciStatus)
  {
    if (sciRespPrm.flags == (uint32_t)TISCI_MSG_FLAG_ACK)
    {
      PCIE_logPrintf("SYSFW  ver %s running\n", (char *) sciResponse.str);
    }
    else
    {
      PCIE_logPrintf("SYSFW Get Version failed \n");
      exit(1);
    }
  }

  MAIN_CTRL_MMR_unlock_all();

  serdes_init(serdes, CSL_SERDES_NO_SSC);

  PCIE_logPrintf("Serdes Init Complete\n");

  /* Check CMU_OK */
  /* CSL_serdesWaitForCMUOK(serdesLaneEnableParams.baseAddr); */
}

/* Nothing past this point */

#if defined(BUILD_MPU) || defined (__C7100__)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
