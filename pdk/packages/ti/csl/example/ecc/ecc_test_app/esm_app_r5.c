/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2019
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
#include <stdint.h>

#include <ti/osal/osal.h>
#include "esm_app_r5.h"

/* static global variables */
HwiP_Handle gEsmHiHwiPHandle;
HwiP_Handle gEsmLoHwiPHandle;
HwiP_Handle gEsmCfgHwiPHandle;

/* ----------------- local Function prototypes ------------------ */
static int32_t cslAppClearESMIntrStatus(uint32_t baseAddr, int32_t intNum);
static int32_t cslAppEnableIntr(uint32_t baseAddr, int32_t intNum);
static int32_t cslAppSetPri(uint32_t baseAddr, uint32_t pri, int32_t intNum);
static int32_t cslAppEnableEsmGlobalIntr(uint32_t baseAddr);
static int32_t cslAppEsmSetupHighPriHandler(uint32_t esm_hi_pri_evt);
static int32_t cslAppEsmSetupLowPriHandler(uint32_t esm_lo_pri_evt);
static void    cslAppEsmHighInterruptHandler(uintptr_t arg);
static void    cslAppEsmLowInterruptHandler(uintptr_t arg);
static void    cslAppEsmInterruptHandler (esmIntrPriorityLvl_t esmIntrPriorityLvlType,
                                          uintptr_t arg );
static void    cslAppEsmProcessInterruptSource(uintptr_t arg, uint32_t intSrc);


volatile uint32_t gSecTestPass;
volatile uint32_t gDedTestPass;
volatile char     gOption;


/* local function implementation */
static void cslAppEsmProcessInterruptSource(uintptr_t arg, uint32_t intSrc)
{

    if (intSrc != NO_EVENT_VALUE)
    {
        /* Clear this error */
        (void)ESMClearIntrStatus(((uint32_t)ESM_CFG_BASE), intSrc);
    }
    return;
}

static void cslAppEsmInterruptHandler (esmIntrPriorityLvl_t esmIntrPriorityLvlType,
                                           uintptr_t arg )
{
    uint32_t             intSrc1, intSrc2;
    esmGroupIntrStatus_t localEsmGroupIntrStatus;

    /* Check if the Interrupt caused by the RAM ID expected
       CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID (56) is under Register offset 0x44
     */
    cslAppChkIsExpectedEvent(arg);

    /* Check on the highest priority event and handle it */
    do {
        (void)ESMGetGroupIntrStatus(((uint32_t)ESM_CFG_BASE), (uint32_t)esmIntrPriorityLvlType,
                              &localEsmGroupIntrStatus);
        intSrc1 = localEsmGroupIntrStatus.highestPendPlsIntNum;
        cslAppEsmProcessInterruptSource(arg, intSrc1);
        if (intSrc1 == (uint32_t) arg)
        {
            break;
        }
        intSrc2 = localEsmGroupIntrStatus.highestPendLvlIntNum;
        cslAppEsmProcessInterruptSource(arg, intSrc2);
        if (intSrc2 == (uint32_t) arg)
        {
            break;
        }
    } while ((intSrc1 != (uint32_t)(NO_EVENT_VALUE)) || (intSrc2 != (uint32_t)(NO_EVENT_VALUE)));

    return;
}

void cslAppEsmHighInterruptHandler (uintptr_t arg)
{
    uint32_t ecc_err_evt = (uint32_t) arg;

    /* Call common Interrupt handler */
     cslAppEsmInterruptHandler(ESM_INTR_PRIORITY_LEVEL_HIGH, ecc_err_evt);

    /* Write end of interrupt */
    (void)ESMClearIntrStatus(((uint32_t)ESM_CFG_BASE), ecc_err_evt);
    return ;
}

void cslAppEsmLowInterruptHandler (uintptr_t arg)
{
    uint32_t ecc_err_evt = (uint32_t) arg;

    /* Call common Interrupt handler */
     cslAppEsmInterruptHandler(ESM_INTR_PRIORITY_LEVEL_LOW, ecc_err_evt);

    /* Write end of interrupt */
    (void)ESMClearIntrStatus(((uint32_t)ESM_CFG_BASE), ecc_err_evt);

    return ;
}


static int32_t cslAppEsmSetupHighPriHandler(uint32_t esm_hi_pri_evt)
{
    HwiP_Params       hwiParams;
    uint32_t          intNumHi = ESM_HI_INT;

    HwiP_disableInterrupt(intNumHi);
    HwiP_Params_init(&hwiParams);
    hwiParams.arg = esm_hi_pri_evt;
    hwiParams.enableIntr = FALSE;
    /* register the call back function for ESM Hi interrupt */
    gEsmHiHwiPHandle = HwiP_create(intNumHi,
                                   (HwiP_Fxn) cslAppEsmHighInterruptHandler,
                                   (void *)&hwiParams);
    HwiP_enableInterrupt(intNumHi);

    return (CSL_PASS);
}

static int32_t cslAppEsmSetupLowPriHandler(uint32_t esm_lo_pri_evt)
{
    HwiP_Params       hwiParams;
    uint32_t          intNumLo = ESM_LO_INT;

    HwiP_disableInterrupt(intNumLo);
    HwiP_Params_init(&hwiParams);
    hwiParams.arg = esm_lo_pri_evt;
    hwiParams.enableIntr = FALSE;
    /* register the call back function for ESM Lo interrupt */
    gEsmHiHwiPHandle = HwiP_create(intNumLo,
                                   (HwiP_Fxn) cslAppEsmLowInterruptHandler,
                                   (void *)&hwiParams);
    HwiP_enableInterrupt(intNumLo);
    return (CSL_PASS);
}

/* This function clears the ESM interrrupt status */
static int32_t cslAppClearESMIntrStatus(uint32_t baseAddr, int32_t intNum)
{
    int32_t   cslRet;
    uint32_t  intStatus;

    /* Clear interrupt status, so that we start with clean state */
    cslRet = ESMClearIntrStatus(baseAddr, intNum);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMGetIntrStatus(baseAddr, intNum, &intStatus);
    }
    if (cslRet == CSL_PASS)
    {
        if (intStatus != ((uint32_t)0U))
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}

/* Enable the ESM event */
static int32_t cslAppEnableIntr(uint32_t baseAddr, int32_t intNum)
{
    int32_t   cslRet;
    uint32_t  intStatus;

    /* Enable interrupt and verify if interrupt status is enabled */
    cslRet = ESMEnableIntr(baseAddr, intNum);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMIsEnableIntr(baseAddr, intNum, &intStatus);
    }
    if (cslRet == CSL_PASS)
    {
        if (intStatus != ((uint32_t)1U))
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}


/* Set the ESM Pri for that event */
static int32_t cslAppSetPri(uint32_t baseAddr, uint32_t pri, int32_t intNum)
{
    int32_t    cslRet;
    esmIntrPriorityLvl_t intrPriorityLvlWr, intrPriorityLvlRd;

    intrPriorityLvlWr = pri;

    cslRet = ESMSetIntrPriorityLvl(baseAddr, intNum, intrPriorityLvlWr);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMGetIntrPriorityLvl(baseAddr,
                                       intNum,
                                       &intrPriorityLvlRd);
    }
    if (cslRet == CSL_PASS)
    {
        if (intrPriorityLvlWr != intrPriorityLvlRd)
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}

/* Enable the global interrupt */
static int32_t cslAppEnableEsmGlobalIntr(uint32_t baseAddr)
{
    int32_t     cslRet;
    uint32_t    intStatus;

    /* Enable Global interrupt and verify if global interrupt is enabled for ESM */
    cslRet = ESMEnableGlobalIntr(baseAddr);

    if (cslRet == CSL_PASS)
    {
        cslRet = ESMGetGlobalIntrEnabledStatus(baseAddr, &intStatus);
    }
    if (cslRet == CSL_PASS)
    {
        if (intStatus != CSL_TEST_ESM_EN_KEY_ENBALE_VAL)
        {
            cslRet = CSL_EFAIL;
        }
    }
    return (cslRet);
}

/* ---------------------------------------------------- */
void cslAppChkIsExpectedEvent(uint32_t eventId)
{
    int32_t                             cslRet = CSL_PASS;
    CSL_ECCErrorInfo                    ECCErrorInfo;
    volatile uint32_t                   *translatedMemPtr;
#if defined (MSMC_PARITY_TEST_SUPPORT)
    bool                                isPend;
    uint32_t                            intrSrc;
#endif
    switch( eventId )
    {
#if defined (MSMC_PARITY_TEST_SUPPORT)
        case MSMC_ECC_AGGR0_DED_ERR_EVENT:
        case MSMC_ECC_AGGR0_SEC_ERR_EVENT:
            gMsmcMemParityInterrupt = TRUE;

            if (eventId == MSMC_ECC_AGGR0_DED_ERR_EVENT)
            {
                intrSrc =  CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT;
            }
            else
            {
                intrSrc = CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT;
            }

            /* Check if the Interrupt caused by the RAM ID expected  */
            if (cslRet == CSL_PASS)
            {
                cslRet = CSL_ecc_aggrIsEDCInterconnectIntrPending((CSL_ecc_aggrRegs*)MSMC_PARITY_ECC_AGGR_REGION_LOCAL_BASE, \
                                                                  CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID, \
                                                                  intrSrc, &isPend);
            }

            if (cslRet == CSL_PASS)
            {
                if (isPend != true)
                {
                    break;
                }
                else
                {
                    cslRet = CSL_ecc_aggrClrEDCInterconnectNIntrPending((CSL_ecc_aggrRegs*)MSMC_PARITY_ECC_AGGR_REGION_LOCAL_BASE, \
                                                                       CC_MSMC_WRAP_ECC_AGGR0_MSMC_DATA_RAM_ID, \
                                                                       intrSrc, \
                                                                       CSL_ECC_AGGR_ERROR_SUBTYPE_INJECT, \
                                                                       1U);
                }

                if (cslRet == CSL_PASS)
                {
                    CSL_ecc_aggrAckIntr((CSL_ecc_aggrRegs*)MSMC_PARITY_ECC_AGGR_REGION_LOCAL_BASE,
                                        intrSrc);
                }
            }
            break;
#endif
        case DDR_ECC_AGGR0_DED_ERR_EVENT:
            /* Read ECC registers and double check address */
            cslRet = CSL_emifGetECCErrorInfo((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE,
                                              &ECCErrorInfo);
            if (cslRet != CSL_PASS)
            {
                UART_printf( "\r\n CSL_emifGetECCErrorInfo Failed ");
                /* TODO: Need to add assert */
                break;
            }
            if ((ECCErrorInfo.doublebitErrorAddress & (~0x7u))
                == ((DDR_ECC_TEST_ADDR- EMIF_DDR_START_ADDR) & (~0x7u)))
            {
                gDedTestPass = TRUE;
                extern volatile uint32_t   testVal;

                /* NOTE: The following section is test code only cannot be used in real application */
                /* ================================================================================ */
                /* Temporarily disable ECC */
                CSL_emifDisableECC((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE);

                /* Calculate translated address */
                translatedMemPtr = (volatile uint32_t *)(DDRGetTranslatedAddress((uintptr_t)gTest_Addr));

                /* Now replace location with original value */
                *(translatedMemPtr) = testVal;

                /* Write back any pending writes */
                CacheP_wbInv((const void *)translatedMemPtr, 4);

                /* Enable back ECC */
                CSL_emifEnableECC((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE);

                /* ================================================================================ */
            }

            /* Clear specific error */
            cslRet = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE,
                                           CSL_EMIF_ECC_ERROR_TYPE_DOUBLE_BIT);
            if (cslRet != CSL_PASS)
            {
                UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
                /* TODO: Need to add assert */
                break;
            }

            /* Clear ECC interrupt bits */
            cslRet = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE,
                                                     CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC2BERR_EN_MASK);
            if (cslRet != CSL_PASS)
            {
                UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
                /* TODO: Need to add assert */
                break;
            }

            break;

        case DDR_ECC_AGGR0_SEC_ERR_EVENT:
            /* Read ECC registers and double check address */
            cslRet = CSL_emifGetECCErrorInfo((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE,
                                              &ECCErrorInfo);
            if (cslRet != CSL_PASS)
            {
                UART_printf( "\r\n CSL_emifConfig ");
                /* TODO: Need to add assert */
                break;
            }

            if ((ECCErrorInfo.singlebitErrorAddress & (~0x7u))
                == ((DDR_ECC_TEST_ADDR- EMIF_DDR_START_ADDR) & (~0x7u)))
            {
                gSecTestPass = TRUE;
            }

            /* Clear Specific ECC error */
            cslRet = CSL_emifClearECCError((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE,
                                           CSL_EMIF_ECC_ERROR_TYPE_SINGLE_BIT);
            if (cslRet != CSL_PASS)
            {
                UART_printf( "\r\n CSL_emifClearECCErrors  failed ");
                /* TODO: Need to add assert */
                break;
            }

            /* Clear ECC interupt bits */
            cslRet = CSL_emifClearECCInterruptStatus((CSL_emif_sscfgRegs *)DDRSS_CFG_BASE,
                                                     CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECC1BERR_EN_MASK
                                                     | CSL_EMIF_SSCFG_V2A_INT_SET_REG_ECCM1BERR_EN_MASK);
            if (cslRet != CSL_PASS)
            {
                UART_printf( "\r\n CSL_emifClearECCInterruptStatus Failed ");
                /* TODO: Need to add assert */
                break;
            }

            break;

        default:
            break;
    }

    return;
}

/* function initializes ESM */
int32_t cslAppEsmSetup(uint32_t  baseAddr, CSL_esm_app_R5_cfg* cfg)
{
    int32_t     cslRet;

    /* ESM reset and configure */
    cslRet = ESMReset((uint32_t)ESM_CFG_BASE);
    if ( cslRet != CSL_PASS)
    {
        UART_printf( "\r\nESM reset failed...");
    }
    if (cslRet == CSL_PASS)
    {
        cslRet = ESMReset((uint32_t)ESM_CFG_BASE);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\nMain ESM reset failed...");
        }
    }

    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppClearESMIntrStatus((uint32_t) ESM_CFG_BASE, cfg->hi_pri_evt);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\n cslAppClearESMIntrStatus hi pri event failed...");
        }
    }

    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppClearESMIntrStatus((uint32_t) ESM_CFG_BASE, cfg->lo_pri_evt);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\n cslAppClearESMIntrStatus lo pri event failed...");
        }
    }

    /* Enable the esm_lvl_event_48 and esm_lvl_event_49
     * for monitoring msmc_eccaggr0_uncorrected and msmc_eccaggr0_corrected
     * events */
     /* Enable interrupt and verify if interrupt status is enabled */
     if (cslRet == CSL_PASS)
    {
        cslRet = cslAppEnableIntr(((uint32_t)ESM_CFG_BASE), cfg->hi_pri_evt);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\nError in ESM Intr Enable for hi pri Event...");
        }
    }

    if (cslRet == CSL_PASS)
    {
         cslRet = cslAppEnableIntr(((uint32_t)ESM_CFG_BASE), cfg->lo_pri_evt);
         if ( cslRet != CSL_PASS)
         {
            UART_printf( "\r\nError in ESM Intr Enable for lo pri Event...");
         }
    }

    /* Assign the priority for the events
     * as single bit errors are corrected, can be assigned to lo pri
     * and double bit errors are not corrected, can be assigned to hi pri
     */

    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppSetPri(((uint32_t)ESM_CFG_BASE), ESM_INTR_PRIORITY_LEVEL_HIGH, cfg->hi_pri_evt);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\nError in setting Pri for hi pri Event...");
        }
    }

     if (cslRet == CSL_PASS)
     {
         cslRet = cslAppSetPri(((uint32_t)ESM_CFG_BASE), ESM_INTR_PRIORITY_LEVEL_LOW, cfg->lo_pri_evt);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\nError in setting Pri for lo pri Event...");
        }
     }

     if (cslRet == CSL_PASS)
     {
        cslRet = cslAppEnableEsmGlobalIntr((uint32_t) ESM_CFG_BASE);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\nESM Enable Global Interrupt Failed...");
        }
     }

    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppEsmSetupHighPriHandler(cfg->hi_pri_evt);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\nESM High Priority handler setup Failed...");
        }
    }

    if (cslRet == CSL_PASS)
    {
        cslRet = cslAppEsmSetupLowPriHandler(cfg->lo_pri_evt);
        if ( cslRet != CSL_PASS)
        {
            UART_printf( "\r\nESM Lo Priority handler setup Failed...");
        }
    }

    if (cslRet == CSL_PASS)
    {
        UART_printf( "\r\n cslAppEsmInit...Done");
    }
    return (cslRet);
}
