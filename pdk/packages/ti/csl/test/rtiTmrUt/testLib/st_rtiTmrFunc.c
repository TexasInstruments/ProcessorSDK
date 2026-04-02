/*
 *   Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file     st_rtiTmrFunc.c
 *
 *  \brief    This file contains RTI Timer test code.
 *
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <st_rtiTmr.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define RTI_CLOCK_SOURCE_32KHZ_FREQ_KHZ     (32U)
#define RTI_CLOCK_SOURCE_200MHZ_FREQ_KHZ    (200000U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
volatile uint32_t isrCnts = 0;

/**< Flag used to indicate interrupt is generated */

static HwiP_Handle gHwiHandleIntr = NULL;
static st_RTITmrTestcaseParams_t *gCurrentTestParams;
static uint32_t	gIntFlag;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   This API to calculate preload in ticks from given configuration value.
 *
 * \param   rtiTmrClkSource RTI module clock source
 *                          Values given by enum #rtiTmrClockSource_t
 *
 * \param   prescaler       RTI Timer prescaler by the counter block.
 * \param   period          RTI Timer period value in milliseconds.
 *
 * \return  Preload value   Number of ticks in compare register.
 */
static uint32_t RTITmrGetPreloadValue(uint32_t rtiTmrClkSource,
                                      uint32_t prescaler,
                                      uint32_t period);
/**
 * \brief   This API to initialize and configure the timer.
 *
 * \param   rtiTmrClkSource RTI module clock source
 *                          Values given by enum #rtiTmrClockSource_t
 *
 * \param   config          pointer to the timer configuration.
 *
 * \return  CSL_EFAIL       configuration error
 *          CSL_PASS        operation succeeds.
 */
static int32_t RTITmrInitAndConfig(uint32_t rtiTmrClkSource, rtiTmrConfig_t *pConfig);
/**
 * \brief   This API to terminate the timer.
 *
 * \param   rtiTmrClkSource RTI module clock source
 *                          Values given by enum #rtiTmrClockSource_t
 *
 * \param   config          pointer to the timer configuration.
 *
 * \return  CSL_EFAIL       configuration error
 *          CSL_PASS        operation succeeds.
 */
static int32_t RTITmrTerminate(uint32_t rtiTmrClkSource, rtiTmrConfig_t *pConfig);
/**
 * \brief   This API to register interrupt for a given instance.
 *
 * \param   rtiTmrModuleSelect RTI module instance
 * \param   cmpBlkIndex	       Comapre Block index
 *
 * \return  None.
 */
static void RTITmrInterruptRegister(uint32_t rtiTmrModuleSelect, uint32_t cmpBlkIndex);
/**
 * \brief   This API to unregister the interrupt
 *
 * \return  None.
 */
static void RTITmrInterruptUnregister(void);
/**
 * \brief   ISR after interrupt generation, sets global flag
 *
 * \param   handle Object handle
 *
 * \retval  None
 */
static void RTITmrISR(uintptr_t handle);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
int32_t st_rtiTmrFunc_main(st_RTITmrTestcaseParams_t *testParams)
{
    uint32_t rtiTmrModule;
    rtiTmrConfig_t tmrCfg;
    int32_t  retVal = STW_SOK;

    gCurrentTestParams = testParams;
    isrCnts = 0;
    /* Configure RTI parameters */
    rtiTmrModule        = testParams->rtiTmrConfigParams.rtiModule;
    tmrCfg.cntBlkIndex  = testParams->rtiTmrConfigParams.rtiCntBlk;
    tmrCfg.cmpBlkIndex  = testParams->rtiTmrConfigParams.rtiCmpBlk;
    tmrCfg.prescaler    = testParams->rtiTmrConfigParams.rtiPrescaler;
    tmrCfg.clkSrc       = RTI_TMR_CLK_SRC_COUNTER;
    tmrCfg.ntu          = RTI_TMR_NTU_0;
    tmrCfg.period       = RTITmrGetPreloadValue(testParams->rtiTmrConfigParams.rtiClkSrc,
                                                testParams->rtiTmrConfigParams.rtiPrescaler,
                                                testParams->rtiTmrConfigParams.rtiPeriod);
    /* Register Interrupt */
    RTITmrInterruptRegister(testParams->rtiTmrConfigParams.rtiModule,
                            testParams->rtiTmrConfigParams.rtiCmpBlk);

    /* Select RTI module clock source */
    retVal = RTITmrInitAndConfig(rtiTmrModule, &tmrCfg);
    if (retVal == CSL_EFAIL)
    {
        UART_printf("Error during Timer configuration.\n");
    }

    if (retVal == CSL_PASS)
    {
        /* Configure RTI and do not service. Generates End time violation. */
        UART_printf("Timer  is configured for %u ms period for %u timer ticks\n",
                     testParams->rtiTmrConfigParams.rtiPeriod,
                     testParams->rtiTmrConfigParams.numTicks);
        (void)RTITmrEnable(rtiTmrModule, tmrCfg.cntBlkIndex);

        /* Wait for the desired timer ticks to be reached */
        while (isrCnts < testParams->rtiTmrConfigParams.numTicks)
        {
            Osal_delay(1U);
        }

        /* Terminate the timer test */
        retVal = RTITmrTerminate(rtiTmrModule, &tmrCfg);

         if (retVal == CSL_PASS)
         {
             UART_printf("RTI Timer functional test successful. \n\n");
         }
         else
         {
             UART_printf("RTI Timer functional test failed. \n");
         }
    }

    RTITmrInterruptUnregister();

    return retVal;
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */
static int32_t RTITmrGetIntFlagFromCmpIndex(uint32_t index, uint32_t *pIntFlag)
{
    switch (index)
    {
        case RTI_TMR_CMP_BLK_INDEX_0:
            *pIntFlag = RTI_TMR_INT_INT0_FLAG;
            break;

        case RTI_TMR_CMP_BLK_INDEX_1:
            *pIntFlag = RTI_TMR_INT_INT1_FLAG;
            break;

        case RTI_TMR_CMP_BLK_INDEX_2:
            *pIntFlag = RTI_TMR_INT_INT2_FLAG;
            break;

        case RTI_TMR_CMP_BLK_INDEX_3:
            *pIntFlag = RTI_TMR_INT_INT3_FLAG;
            break;

        default:
            UART_printf("RTITimerGetInt: failure on line no. %d with invalid compare block %u\n",
                        __LINE__, index);
            return (CSL_EFAIL);
    }

    return (CSL_PASS);
}

static int32_t RTITmrInitAndConfig(uint32_t rtiTmrClkSource, rtiTmrConfig_t *pConfig)
{
    uint32_t intFlags;

    /* Disable the specified counter block prior to configuration */
    if(RTITmrDisable(rtiTmrClkSource, pConfig->cntBlkIndex) != CSL_PASS)
    {
        UART_printf("RTITmrDisable: failure on line no. %d with counter block %u\n",
                    __LINE__, pConfig->cntBlkIndex);
        return (CSL_EFAIL);
    }

    /* Clear the specified counter and comapre block */
    if(RTITmrClear(rtiTmrClkSource, pConfig->cntBlkIndex, pConfig->cmpBlkIndex) != CSL_PASS)
    {
        UART_printf("RTITmrClear: failure on line no. %d with cnt/cmp block (%u, %u)\n",
                     __LINE__, pConfig->cntBlkIndex, pConfig->cmpBlkIndex);
        return (CSL_EFAIL);
    }

    /* Timer configuration */
    if(RTITmrConfigure(rtiTmrClkSource, pConfig) != CSL_PASS)
    {
        UART_printf("RTITmrConfigure: failure on line no. %d\n", __LINE__);
        return (CSL_EFAIL);
    }

    /* Enable interrupt */
    if(RTITmrGetIntFlagFromCmpIndex(pConfig->cmpBlkIndex, &intFlags) != CSL_PASS)
    {
        UART_printf("RTITmrGetIntFlagFromCmpIndex: failure on line no. %d compare block %u\n",
                    __LINE__, pConfig->cmpBlkIndex);
        return (CSL_EFAIL);
    }

    /* Record the interrupt flag */
    gIntFlag = intFlags;
    if(RTITmrIntEnable(rtiTmrClkSource, intFlags) != CSL_PASS)
    {
        UART_printf("RTITmrIntEnable: failure on line no. %d with flags 0x%0x\n",
                    __LINE__, intFlags);
        return (CSL_EFAIL);
    }
    return (CSL_PASS);
}


static int32_t RTITmrTerminate(uint32_t rtiTmrClkSource, rtiTmrConfig_t *pConfig)
{
    /* Disable the specified counter block prior to configuration */
    if(RTITmrDisable(rtiTmrClkSource, pConfig->cntBlkIndex) != CSL_PASS)
    {
        UART_printf("RTITmrDisable: failure on line no. %d with counter block %u\n",
                    __LINE__, pConfig->cntBlkIndex);
        return (CSL_EFAIL);
    }

    if(RTITmrIntDisable(rtiTmrClkSource, gIntFlag) != CSL_PASS)
    {
        UART_printf("RTITmrIntDisable: failure on line no. %d with flags 0x%0x\n",
                    __LINE__, gIntFlag);
        return (CSL_EFAIL);
    }

    /* Clear pending intrrupt */
    if(RTITmrIntStatusClear(rtiTmrClkSource, gIntFlag) != CSL_PASS)
    {
        UART_printf("RTITmrIntStatusClear: failure on line no. %d with flags 0x%0x\n",
                    __LINE__, gIntFlag);
        return (CSL_EFAIL);
    }
    return (CSL_PASS);
}

#if defined(SOC_TPR12) || defined (SOC_AWR294X)
#define MCU_RTITMR0_INT0                       CSL_MSS_INTR_MSS_RTIA_INT0
#define MCU_RTITMR0_INT1                       CSL_MSS_INTR_MSS_RTIA_INT1
#define MCU_RTITMR0_INT2                       CSL_MSS_INTR_MSS_RTIA_INT2
#define MCU_RTITMR0_INT3                       CSL_MSS_INTR_MSS_RTIA_INT3
#define MCU_RTITMR1_INT0                       CSL_MSS_INTR_MSS_RTIB_INT0
#define MCU_RTITMR1_INT1                       CSL_MSS_INTR_MSS_RTIB_INT1
#define MCU_RTITMR1_INT2                       CSL_MSS_INTR_MSS_RTIB_INT2
#define MCU_RTITMR1_INT3                       CSL_MSS_INTR_MSS_RTIB_INT3
#define MCU_RTITMR2_INT0                       CSL_MSS_INTR_MSS_RTIC_INT0
#define MCU_RTITMR2_INT1                       CSL_MSS_INTR_MSS_RTIC_INT1
#define MCU_RTITMR2_INT2                       CSL_MSS_INTR_MSS_RTIC_INT2
#define MCU_RTITMR2_INT3                       CSL_MSS_INTR_MSS_RTIC_INT3
#endif

static const uint32_t rtiTmrIntNum[3][4] = {{MCU_RTITMR0_INT0, MCU_RTITMR0_INT1, MCU_RTITMR0_INT2, MCU_RTITMR0_INT3},
                                            {MCU_RTITMR1_INT0, MCU_RTITMR1_INT1, MCU_RTITMR1_INT2, MCU_RTITMR1_INT3},
                                            {MCU_RTITMR2_INT0, MCU_RTITMR2_INT1, MCU_RTITMR2_INT2, MCU_RTITMR2_INT3}};

static void RTITmrInterruptRegister(uint32_t rtiTmrModuleSelect, uint32_t cmpBlkIndex)
{
    OsalRegisterIntrParams_t intrPrmsIntr;
    OsalInterruptRetCode_e osalRetVal;
    HwiP_Handle gHwiHandleIntr;
    uint32_t rtiTmrIndex = 0U;

    if (cmpBlkIndex > RTI_TMR_CMP_BLK_INDEX_MAX)
    {
        UART_printf("Error Could not register ISR: cmpBlkIndex %u out of range\n",
                    cmpBlkIndex);
        return;
    }

    Osal_RegisterInterrupt_initParams(&intrPrmsIntr);
    intrPrmsIntr.corepacConfig.arg          = (uintptr_t)0;
    intrPrmsIntr.corepacConfig.priority     = 1U;
    intrPrmsIntr.corepacConfig.corepacEventNum = 0U; /* NOT USED */
    intrPrmsIntr.corepacConfig.isrRoutine   = &RTITmrISR;
    switch (rtiTmrModuleSelect)
    {
        case CSL_MCU_RTITMR0_CFG_BASE:
            rtiTmrIndex = 0;
            break;

        case CSL_MCU_RTITMR1_CFG_BASE:
            rtiTmrIndex = 1;
            break;

        case CSL_MCU_RTITMR2_CFG_BASE:
            rtiTmrIndex = 2;
            break;

        default :
            UART_printf("Error : RTI Timer Instance not supported "
                "!!!\r\n");
        break;
    }

    intrPrmsIntr.corepacConfig.intVecNum    = rtiTmrIntNum[rtiTmrIndex][cmpBlkIndex];

    osalRetVal = Osal_RegisterInterrupt(&intrPrmsIntr, &gHwiHandleIntr);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        UART_printf("Error Could not register ISR !!!\n");
    }

    UART_printf("register ISR (%u) successfully\n", intrPrmsIntr.corepacConfig.intVecNum);

    return;
}

static void RTITmrInterruptUnregister(void)
{
    if (gHwiHandleIntr != NULL)
    {
        Osal_DeleteInterrupt(gHwiHandleIntr, 0U);
        gHwiHandleIntr = NULL;
    }
    return;
}

static uint32_t RTITmrGetPreloadValue(uint32_t rtiTmrClkSource,
                                      uint32_t prescaler,
                                      uint32_t period)
{
    uint32_t clkFreqKHz       = (uint32_t) RTI_CLOCK_SOURCE_32KHZ_FREQ_KHZ,
             timeoutNumCycles = 0;

    switch (rtiTmrClkSource)
    {
        case RTI_TMR_CLOCK_SOURCE_32KHZ:
            clkFreqKHz = (uint32_t) RTI_CLOCK_SOURCE_32KHZ_FREQ_KHZ;
            break;
        case RTI_TMR_CLOCK_SOURCE_200MHZ:
            clkFreqKHz = (uint32_t) RTI_CLOCK_SOURCE_200MHZ_FREQ_KHZ;
            break;
        default:
            break;
    }
    /* Get the clock ticks for given time-out value */
    timeoutNumCycles = period * clkFreqKHz / (prescaler + 1);
    return timeoutNumCycles;
}

static void RTITmrISR(uintptr_t handle)
{
    uint32_t rtiTmrModule = gCurrentTestParams->rtiTmrConfigParams.rtiModule;

    (void)RTITmrIntStatusClear(rtiTmrModule, gIntFlag);
    isrCnts++;
}
/********************************* End of file ******************************/
