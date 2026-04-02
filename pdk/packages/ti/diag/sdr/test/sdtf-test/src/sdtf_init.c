/*
 * SDR TEST
 *
 * Software Diagnostics Reference Test
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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
#include <string.h>
#include <sdr_common.h>
#include <sdr_esm.h>
#include <sdr_ccm.h>
#include <sdr_crc.h>

/* UART driver API header files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/soc/UART_soc.h>

/* Osal API header files */
#include <ti/osal/HwiP.h>

/* Local SDTF header files */
#include "sdtf_common.h"
#include <sdtf_platform.h>
#include "sdtf_exception.h"
#include "sdtf_profile.h"
#include "sdtf_wdt.h"
#include "sdtf_ecc.h"
#include "sdtf_utils.h"
#include <sdtf_soc.h>

#ifdef SDTF_BOARD
#include <ti/board/board.h>
#include <ti/osal/osal.h>
#endif

/* Defines */
#define MCU_R5F_MAX_MEM_SECTIONS   (8u)
#define MCU_CBASS_MAX_MEM_SECTIONS (3u)

/* Function prototypes */
void SDTF_copyResetVector(void);
void SDTF_uartStdioInit(void);
int32_t SDTF_EsmInitHandlerInit(SDR_ESM_InstanceType esmInstType);
int32_t SDTF_init (void);
void SDTF_VIMDEDInterruptHandler(void);


static SDR_ECC_MemSubType SDTF_R5FCoresubMemTypeList[MCU_R5F_MAX_MEM_SECTIONS] =
{
    SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID,
    SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID,
};
static SDR_ECC_InitConfig_t SDTF_R5FCoreECCInitConfig =
{
    .numRams = MCU_R5F_MAX_MEM_SECTIONS,
    /**< Number of Rams ECC is enabled  */
    .pMemSubTypeList = &(SDTF_R5FCoresubMemTypeList[0]),
    /**< Sub type list  */
};

#ifdef SOC_J721E
static SDR_ECC_MemSubType SDTF_MCUCBASSsubMemTypeList[MCU_CBASS_MAX_MEM_SECTIONS] =
{
    SDR_ECC_MCU_CBASS_MEM_SUBTYPE_WR_RAMECC_ID,
    SDR_ECC_MCU_CBASS_MEM_SUBTYPE_RD_RAMECC_ID,
    SDR_ECC_MCU_CBASS_MEM_SUBTYPE_EDC_CTRL_ID,
};


static SDR_ECC_InitConfig_t SDTF_MCUCBASSECCInitConfig =
{
    .numRams = MCU_CBASS_MAX_MEM_SECTIONS,
    /**< Number of Rams ECC is enabled  */
    .pMemSubTypeList = &(SDTF_MCUCBASSsubMemTypeList[0]),
    /**< Sub type list  */
};
#endif

HwiP_Handle SDTF_EsmHiHwiPHandle;
HwiP_Handle SDTF_EsmLoHwiPHandle;
HwiP_Handle SDTF_EsmCfgHwiPHandle;

extern void *_resetvectors;
#define VECTOR_SIZE 0x100
#define APP_MAX_PROFILE_SIZE 16

void SDTF_copyResetVector(void)
{
    /* Invalidate cache */
    SDTF_cpuCacheInv(0, VECTOR_SIZE);

    /* copy teh vectors into 0 location */
    /* Vectors are always assumed to be at 0 for R5 */
    memcpy(0, &_resetvectors, VECTOR_SIZE);

    /* Write back cache */
    SDTF_cpuCacheWbInv(0, VECTOR_SIZE);
}

/**
 * \brief   This function initializes the UART instance for use for
 *          console operations.
 *
 * \return  None
 *
 */
void SDTF_uartStdioInit(void)
{
    UART_HwAttrs uart_cfg;

    /* Disable the UART interrupt */
    (void)UART_socGetInitCfg(SDTF_UART_INSTANCE, &uart_cfg);
    uart_cfg.enableInterrupt = FALSE;
    (void)UART_socSetInitCfg(SDTF_UART_INSTANCE, &uart_cfg);

    UART_stdioInit(SDTF_UART_INSTANCE);
    return;
}

int32_t SDTF_EsmInitHandlerInit(SDR_ESM_InstanceType esmInstType)
{
    int32_t result = 0;
    uint32_t intNumHi, intNumLo, intNumCfg;
    HwiP_Params       hwiParams;
    void (*pHiInterruptHandler)(uintptr_t);
    void (*pLoInterruptHandler)(uintptr_t);
    void (*pConfigInterruptHandler)(uintptr_t);

    switch (esmInstType) {
        case (SDR_ESM_INSTANCE_WKUP):
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_WKUP;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_WKUP;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_WKUP;
            break;

        case (SDR_ESM_INSTANCE_MAIN):
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_MAIN;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_MAIN;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_MAIN;
            break;

        case (SDR_ESM_INSTANCE_MCU):
        default:
            pHiInterruptHandler = &SDR_ESM_hiInterruptHandler_MCU;
            pLoInterruptHandler = &SDR_ESM_loInterruptHandler_MCU;
            pConfigInterruptHandler = &SDR_ESM_configInterruptHandler_MCU;
            break;
    }

    /* Get the interrupt number corresponding to ESM HI interrupt */
    intNumHi = SDR_ESM_getIntNumber(esmInstType,
                                    SDR_ESM_INT_TYPE_HI);
    if ( intNumHi == SDR_ESM_INTNUMBER_INVALID ) {
        result = -1;
    } else {
        /* Get the interrupt number corresponding to ESM LO interrupt */
        intNumLo = SDR_ESM_getIntNumber(esmInstType,
                                        SDR_ESM_INT_TYPE_LO);
        if ( intNumLo == SDR_ESM_INTNUMBER_INVALID ) {
            result = -1;
        } else {
            /* Get the interrupt number corresponding to ESM Config interrupt */
            intNumCfg = SDR_ESM_getIntNumber(esmInstType,
                                             SDR_ESM_INT_TYPE_CFG);
            if ( intNumCfg == SDR_ESM_INTNUMBER_INVALID ) {
                result = -1;
            } else {
                SDTF_printf("\n SDTF_EsmInitHandlerInit: Obtain interrupt number complete \n");
                /* Clear all interrupts first */
                HwiP_disableInterrupt(intNumHi);
                HwiP_disableInterrupt(intNumLo);
                HwiP_disableInterrupt(intNumCfg);

                SDTF_printf("\n SDTF_EsmInitHandlerInit: Disable interrupts complete \n");

                /* Default parameter initialization */
                HwiP_Params_init(&hwiParams);

                hwiParams.arg = intNumHi;
                SDTF_printf("\n SDTF_EsmInitHandlerInit: HwiP_Params_init complete \n");

                /* Register call back function for ESM Hi Interrupt */
                SDTF_EsmHiHwiPHandle = HwiP_create(intNumHi,
                                                   (HwiP_Fxn)(*pHiInterruptHandler),
                                                   (void *)&hwiParams);
                SDTF_printf("\n SDTF_EsmInitHandlerInit: intNumHi registration complete \n");
                if (SDTF_EsmHiHwiPHandle == (HwiP_Handle) NULL) {
                    result = -1;
                } else {
                    hwiParams.arg = intNumLo;
                    /* Register call back function for ESM Lo Interrupt */
                    SDTF_EsmLoHwiPHandle = HwiP_create(intNumLo,
                                                       (HwiP_Fxn)(*pLoInterruptHandler),
                                                       (void *)&hwiParams);
                    SDTF_printf("\n SDTF_EsmInitHandlerInit: intNumLo registration complete \n");
                    if (SDTF_EsmLoHwiPHandle == (HwiP_Handle) NULL) {
                        result = -1;
                    } else {
                        hwiParams.arg = intNumCfg;
                        /* Register call back function for ESM Config Interrupt */
                        SDTF_EsmCfgHwiPHandle = HwiP_create(intNumCfg,
                                                   (HwiP_Fxn)(*pConfigInterruptHandler),
                                                   (void *)&hwiParams);
                        SDTF_printf("\n SDTF_EsmInitHandlerInit: intNumCfg registration complete \n");
                        if (SDTF_EsmCfgHwiPHandle == (HwiP_Handle) NULL) {
                            result = -1;
                        } else {
                            /* Enable all ESM Interrupts */
                            HwiP_enableInterrupt(intNumHi);
                            HwiP_enableInterrupt(intNumLo);
                            HwiP_enableInterrupt(intNumCfg);
                       }
                    }
                }
            }
        }
    }

    return result;
}
#define SDTF_NEGATIVE_TESTS
#ifdef SDTF_NEGATIVE_TESTS
/*********************************************************************
* @fn      SDTF_negativeInitTests
*
* @brief   Run negative tests on init functions
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t SDTF_negativeInitTests (void)
{
    int32_t retValue=0;
    SDR_Result result;

    /* Run negative test on init functions */
    result = SDR_ESM_init(SDR_ESM_INSTANCE_MCU, NULL);
    if (result == SDR_PASS) {
        /* Negative test failed */
        SDTF_printf("SDTF_init: Error ESM Negative tests: result = %d\n", result);
        retValue = -1;
    }

    if (retValue == 0) {
        if (SDR_ESM_init((SDR_ESM_InstanceType)((uint16_t)SDR_ESM_INSTANCE_MCU - 1U),
                          &SDTF_esmInitConfig_MCU) == SDR_PASS)
        {
            SDTF_printf("SDTF_init: Error ESM Negative tests: ESM instance type below " \
                        "valid SDR_ESM_InstanceType improperly passed\n");
            retValue = -1;
        }
    }

    if (retValue == 0)
    {
        if (SDR_ESM_init((SDR_ESM_InstanceType)((uint16_t)SDR_ESM_INSTANCE_MAIN + 1U),
                         &SDTF_esmInitConfig_MCU) == SDR_PASS)
        {
            SDTF_printf("SDTF_init: Error ESM Negative tests: ESM instance type above " \
                        "valid SDR_ESM_InstanceType improperly passed\n");
            retValue = -1;
        }
    }

    if (retValue == 0) {
        result = SDR_ECC_init(SDR_ECC_MEMTYPE_MCU_R5F0_CORE, NULL);
        if (result == SDR_PASS) {
            /* Negative test failed */
            SDTF_printf("SDTF_init: Error ECC Negative tests: result = %d\n", result);
            retValue = -1;
        }
    }

    if (retValue == 0) {
        result = SDR_ECC_init(SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0, NULL);
        if (result == SDR_PASS) {
            /* Negative test failed */
            SDTF_printf("SDTF_init: Error ECC Negative tests: result = %d\n", result);
            retValue = -1;
        }
    }
    return retValue;
}
#endif

/*********************************************************************
* @fn      SDTF_init
*
* @brief   Initializes Software Diagostics Test Framework
*
* @param   None
*
* @return    0 : Success; < 0 for failures
*/
int32_t SDTF_init (void)
{
    int32_t retValue=0;

    SDR_Result result;

    /* Enable ATCM and BTCM */
    SDTF_UTILS_TCMEnable();

    /* Initialize ATCM */
    SDTF_UTILS_initATCM();

#ifdef SDTF_BOARD
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);
#else
    /* Initialize UART instance */
    SDTF_uartStdioInit();
#endif

    SDTF_printf("\nSDTF_init: UART ready to print, proceeding with SDTF init \n");

    /* Initialise Profile module */
    SDTF_profileInit(APP_MAX_PROFILE_SIZE);

    SDTF_printf("\nSDTF_init: Profile init complete \n");

    /* Call Init for SDTF software structures */

    /* Copy reset vector to 0 location */
    SDTF_copyResetVector();

    SDTF_printf("\nSDTF_init: Copy Reset vector complete \n");

    /* Initialise exception handler */
    SDTF_exceptionInit();

    SDTF_printf("\nSDTF_init: Exception init complete \n");

    /* Register VIM DED interrupt handler */
    SDR_ECC_registerVIMDEDHandler(&SDTF_VIMDEDInterruptHandler);

    SDTF_printf("\nSDTF_init: register VIM DED Handler complete \n");

#ifdef SDTF_NEGATIVE_TESTS
    retValue = SDTF_negativeInitTests();
#endif

    if (retValue == 0) {
        /* Initialize MCU ESM module */
        result = SDR_ESM_init(SDR_ESM_INSTANCE_MCU, &SDTF_esmInitConfig_MCU);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing MCU ESM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: Init MCU ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize WKUP ESM module */
        result = SDR_ESM_init(SDR_ESM_INSTANCE_WKUP, &SDTF_esmInitConfig_WKUP);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing WKUP ESM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: Init WKUP ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize MAIN ESM module */
        result = SDR_ESM_init(SDR_ESM_INSTANCE_MAIN, &SDTF_esmInitConfig_MAIN);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing MAIN ESM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: Init MAIN ESM complete \n");
        }
    }

    if (retValue == 0) {
        /* Initialize ECC */
        result = SDR_ECC_init(SDR_ECC_MEMTYPE_MCU_R5F0_CORE, &SDTF_R5FCoreECCInitConfig);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing R5F core ECC: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: R5F Core ECC Init complete \n");
        }
    }

#ifdef SOC_J721E
    if (retValue == 0) {
        /* Initialize ECC */
        result = SDR_ECC_init(SDR_ECC_MEMTYPE_MCU_CBASS_ECC_AGGR0, &SDTF_MCUCBASSECCInitConfig);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing R5F core ECC: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: MCU CBASS ECC Init complete \n");
        }
    }
#endif
    if (retValue == 0) {
        /* Initialize ECC callbacks within the MCU ESM */
        result = SDR_ECC_initEsm(SDR_ESM_INSTANCE_MCU);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing ECC callback for MCU ESM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: ECC Callback Init complete for MCU ESM \n");
        }
    }

    if (retValue == 0) {
        /* Initialize ECC callbacks within the Main ESM */
        result = SDR_ECC_initEsm(SDR_ESM_INSTANCE_MAIN);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing ECC callback for Main ESM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: ECC Callback Init complete for Main ESM \n");
        }
    }

    if (retValue == 0) {
        /* Initialize ECC callbacks within the WKUP ESM */
        result = SDR_ECC_initEsm(SDR_ESM_INSTANCE_WKUP);
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing ECC callback for WKUP ESM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: ECC Callback Init complete for WKUP ESM \n");
        }
    }

    if (retValue == 0) {
        /* Initialize VIM ECC memory ; This is specifically for VIM in lockstep*/
        /* All other memories are auto initialized by hardware */
        result = SDR_ECC_initMemory(SDR_ECC_MEMTYPE_MCU_R5F0_CORE, SDR_ECC_R5F_MEM_SUBTYPE_KS_VIM_RAM_VECTOR_ID);
        if (result != SDR_PASS) {
             /* print error and quit */
              SDTF_printf("SDTF_init: Error initializing ECC memory: retValue = %d\n", retValue);
             retValue = -1;
         } else {
             SDTF_printf("\nSDTF_init: ECC Memory init complete \n");
         }
    }

    if (retValue == 0) {
        /* Wait for any self test running to settle */
        Osal_delay(2000);

        /* Initialize CCM module */
        result = SDR_CCM_init();
        if (result != SDR_PASS) {
            /* print error and quit */
             SDTF_printf("SDTF_init: Error initializing CCM: result = %d\n", result);

            retValue = -1;
        } else {
            SDTF_printf("\nSDTF_init: Init CCM complete \n");
        }
    }

    if (retValue == 0) {
        /* Register MCU Esm handlers */
        retValue = SDTF_EsmInitHandlerInit(SDR_ESM_INSTANCE_MCU);
        if (retValue != 0) {
            SDTF_printf("\nSDTF_init: Init MCU ESM handlers failed \n");
        } else {
            SDTF_printf("\nSDTF_init: Init MCU ESM handlers complete \n");
        }

        /* Register WKUP Esm handlers */
        retValue = SDTF_EsmInitHandlerInit(SDR_ESM_INSTANCE_WKUP);
        if (retValue != 0) {
            SDTF_printf("\nSDTF_init: Init WKUP ESM handlers failed \n");
        } else {
            SDTF_printf("\nSDTF_init: Init WKUP ESM handlers complete \n");
        }

        /* Register MAIN Esm handlers */
        retValue = SDTF_EsmInitHandlerInit(SDR_ESM_INSTANCE_MAIN);
        if (retValue != 0) {
            SDTF_printf("\nSDTF_init: Init MAIN ESM handlers failed \n");
        } else {
            SDTF_printf("\nSDTF_init: Init MAIN ESM handlers complete \n");
        }
    }

    if (retValue == 0)
    {
        /* Initialize CRC SDR */
        SDR_CRC_init();
        SDTF_printf("\nSDTF_init: Init CRC complete \n");
    }

    if (retValue == 0) {
        /* Initialize Watchdog timer */
        retValue = SDTF_WDT_init();
        if (retValue != 0) {
            SDTF_printf("\nSDTF_init: Init WDT failed \n");
        } else {
            SDTF_printf("\nSDTF_init: Init WDT complete \n");
        }
    }

    return retValue;
}

void SDR_assertExternalFunction(SDR_assertErrorNumber errorNumber)
{

    SDTF_printf("\n  Assert called with error Number %d \n",
                errorNumber);
    /* Asserts are fatal: Currently just wait for ever */
    while(1);
}


uint32_t SDR_getTime(void)
{
    return SDTF_profileTimerRead();
}

void SDR_ESM_applicationCallbackFunction(SDR_ESM_InstanceType esmInstType,
                                         SDR_ESM_IntType esmIntType,
                                         uint32_t grpChannel,
                                         uint32_t index,
                                         uint32_t intSrc){

    SDTF_printf("\n  ESM Call back function called : instType 0x%x, intType 0x%x, " \
                "grpChannel 0x%x, index 0x%x, intSrc 0x%x \n",
                esmInstType, esmIntType, grpChannel, index, intSrc);
    SDTF_printf("  Take action \n");

    /* Any additional customer specific actions can be added here */

}

void SDR_ECC_applicationCallbackFunction(SDR_ECC_MemType eccMemType,
                                         uint32_t errorSrc,
                                         uint32_t address,
                                         uint32_t ramId,
                                         uint64_t bitErrorOffset,
                                         uint32_t bitErrorGroup){

    SDTF_printf("\n  ECC Error Call back function called : eccMemType %d, errorSrc 0x%x, " \
                "address 0x%x, ramId %d, bitErrorOffset 0x%04x%04x, bitErrorGroup %d\n",
                eccMemType, errorSrc, address, ramId, (uint32_t)(bitErrorOffset >> 32),
                (uint32_t)(bitErrorOffset & 0x00000000FFFFFFFF), bitErrorGroup);
    SDTF_printf("  Take action \n");

    /* Any additional customer specific actions can be added here */

}

void SDR_CCM_applicationCallbackFunction(SDR_CCM_MonitorType monitorType,
                                         SDR_CCM_ErrorStatus_t *pErrorStatus)
{
    SDTF_printf("\n  CCM Call back function called : monitorType 0x%x, cmp error flag 0x%x,"
            "self test error flag  0x%x, self test error type 0x%x.\n",
            monitorType, pErrorStatus->compareErrorFlag, pErrorStatus->selfTestErrorFlag,
            pErrorStatus->selfTestErrorTypeFlag);
    SDTF_printf("  Take action \n");

   /* Any additional customer specific actions can be added here */

}

/* Note the following DED vector handler need to be declared with the attribute of interrupt
 * so that the appropriate return is implemented correctly
 */
__attribute__((interrupt)) __attribute__((target("arm"))) void SDTF_VIMDEDInterruptHandler(void);
/*********************************************************************
* @fn      SDTF_VIMDEDInterruptHandler
*
* @brief   Sample DED Interrupt handler
*
* @param   None
*
* @return   None
*/
void SDTF_VIMDEDInterruptHandler(void)
{
    /* Action to be taken by customer */
    SDTF_printf("\n  VIM DED error happened \n");
    SDTF_printf("  Take action \n");

    /* clear appropriate interrupt events */

    SDTF_ECC_indicateDEDTrigger();
}
