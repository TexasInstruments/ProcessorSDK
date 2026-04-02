/*
 *  Copyright (c) Texas Instruments Incorporated 2026
 *  All rights reserved.
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
 */

/**
 *  \file main_baremetal.c
 *
 *  \brief Periodic Software Readback Example reads static configuration
 *         registers of Multicore Shared Memory Controller (MSMC) and
 *         reports match or mismatch between expected register values
 *         and actual (read) ones.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "ip_fma_clec.h"
#include "ip_fma_cfg_wrap_cc.h"
#include <ti/csl/csl_cbass.h>
#include <sciclient.h>
#include <ti/board/board.h>
#include <ti/csl/csl_types.h>
#include <ti/drv/uart/UART_stdio.h>
#if defined(SOC_J721S2)
#include <ti/csl/soc/j721s2/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/j721s2/src/cslr_mcu_r5fss0_baseaddress.h>
#elif defined(SOC_J784S4)
#include <ti/csl/soc/j784s4/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/j784s4/src/cslr_mcu_r5fss0_baseaddress.h>
#endif
#include <ti/csl/src/ip/rat/V0/csl_rat.h>
#include <ti/csl/src/ip/rat/V0/cslr_rat.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**< Delay time in ms */
#define DELAY_MS                                    ((uint8_t)2000U)
/**< Number of periodic readbacks */
#define PERIODIC_CHECK_NUM                          ((uint8_t)1U)
/**< Translated 32-bit base address */
#define COMPUTE_CLUSTER0_CC_LOCAL_BASE              (0x50000000)
/**< Firewall definitions */
#define FW_REGION_ENABLE                            (0xAU)
/**< Privid of R5F core */
#define FW_MCU_R5F0_PRIVID                          (96U)

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief Initializes the application by setting the configuration.
 *
 *  This is a initialization function that sets the board configuration.
 *  It configures the board by enabling UART module needed to print log
 *  messages to the user via UART console.
 *
 *  \return  BOARD_SOK in case of success or appropriate error code.
 */
static int32_t MsmcApp_Init();

/**
 * \brief   Local Delay function
 *
 * \param   wait_in_ms Delay time in miliseconds
 * 
 * \retval  None.
 */
static void MsmcApp_Delay(uint32_t wait_in_ms);

/**
 *  \brief Sets firewall settings to be able to access CLEC registers.
 *
 *  \return  CSL_PASS in case of success or appropriate error code.
 */
static int32_t MsmcApp_SetFirewall();

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * Application main
 */
int32_t main(void)
{
    int32_t retVal = CSL_PASS;
    int32_t status = CSL_PASS;

    /* Set board configuration */
    status = MsmcApp_Init();

    if (BOARD_SOK == status)
    {   
        UART_printf("\n[MSMC APP]: Board Init successful!\n");

        /* Set firewall to be able to access CLEC registers */
        status = MsmcApp_SetFirewall();

        if (CSL_PASS == status)
        {
            UART_printf("\n[MSMC APP]: Set firewall settings successful!\n");

            bool statusTranslate = (bool)false;
            /** 
             * The RAT module performs a region based address translation.
             * It translates a 32-bit input address into a 48-bit output address. 
            */
            uint32_t ratRegion0   = 0;
            CSL_ratRegs *pRatRegs = (CSL_ratRegs *)CSL_MCU_R5FSS0_RAT_CFG_BASE;
            CSL_RatTranslationCfgInfo TranslationCfgCC;

            TranslationCfgCC.sizeInBytes       = CSL_COMPUTE_CLUSTER0_CC_SIZE;    // whole COMPUTE_CLUSTERHP0_CC area (64KB) is translated
            TranslationCfgCC.baseAddress       = COMPUTE_CLUSTER0_CC_LOCAL_BASE;  // 32-bit local base address
            TranslationCfgCC.translatedAddress = CSL_COMPUTE_CLUSTER0_CC_BASE;    // 40-bit base address 0x004d 2100 0000

            if (ratRegion0 < CSL_ratGetMaxRegions(pRatRegs))
            {
                if (CSL_ratIsRegionTranslationEnabled(pRatRegs, ratRegion0) == (bool)false)
                {
                    CSL_ratEnableRegionTranslation(pRatRegs, ratRegion0);
                }
                
                /* Set up RAT translation */
                statusTranslate = CSL_ratConfigRegionTranslation(pRatRegs, ratRegion0, &TranslationCfgCC);

                if ((bool)true == statusTranslate)
                {
                    UART_printf("\n[MSMC APP]: Successfull configure the RAT at address 0x%08x%08x\n", \
                        (uint32_t)(TranslationCfgCC.translatedAddress >> 32), (uint32_t)TranslationCfgCC.translatedAddress);
                }
            }
        
            // Loading expected CC register values
            IpFma_CfgWrapCCRegs msmcCCRegsExpValues;
            status = IpFma_CfgWrapCC_GetRegs(COMPUTE_CLUSTER0_CC_LOCAL_BASE, &msmcCCRegsExpValues);

            if (IPFMA_OK == status)
            {
                int8_t i = PERIODIC_CHECK_NUM;
                
                while (i > 0)
                {
                    // Read the actual values from registers
                    IpFma_CfgWrapCCRegs msmcCCRegsActualValues;
                    status = IpFma_CfgWrapCC_GetRegs(COMPUTE_CLUSTER0_CC_LOCAL_BASE, &msmcCCRegsActualValues);

                    if (IPFMA_OK == status)
                    {
                        // Compare expected and actual values periodically
                        UART_printf("\n[MSMC APP]: Comparing expected-actual CC register values...");
                        status = IpFma_CfgWrapCC_CompareRegs(&msmcCCRegsExpValues, &msmcCCRegsActualValues);

                        if (IPFMA_OK == status)
                        {
                            UART_printf(" values MATCH!\n");
                        }
                        else
                        {
                            UART_printf("values MISMATCH!\n");
                            retVal = CSL_EFAIL;
                        }
                        i--;
                    }
                    else{
                        UART_printf("\nCC Register read failed \n"); 
                        retVal = CSL_EFAIL;
                    }
                    
                    MsmcApp_Delay(DELAY_MS);
                }
            }
            else{
                UART_printf("\nCC Register read failed \n");
                retVal = CSL_EFAIL;
            }


            // Loading expected CLEC register values
            IpFma_ClecRegs msmcClecRegsExpValues;
            status = IpFma_Clec_GetRegs(CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE, &msmcClecRegsExpValues);

            if (IPFMA_OK == status)
            {
                int8_t i = PERIODIC_CHECK_NUM;
                
                while (i > 0)
                {
                    // Read the actual values from registers
                    IpFma_ClecRegs msmcClecRegsActualValues;
                    status = IpFma_Clec_GetRegs(CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE, &msmcClecRegsActualValues);

                    if (IPFMA_OK == status)
                    {
                        // Compare expected and actual values periodically
                        UART_printf("\n[MSMC APP]: Comparing expected-actual CLEC register values...");
                        status = IpFma_Clec_CompareRegs(&msmcClecRegsExpValues, &msmcClecRegsActualValues);

                        if (IPFMA_OK == status)
                        {
                            UART_printf(" values MATCH!\n");
                        }
                        else
                        {
                            UART_printf(" values MISMATCH!\n");
                            retVal = CSL_EFAIL;
                        }
                        i--;
                    }
                    else{
                        UART_printf("\nCLEC Register read failed \n"); 
                        retVal = CSL_EFAIL;
                    }
                    
                    MsmcApp_Delay(DELAY_MS);
                }
            }
            else{
                UART_printf("\nCLEC Register read failed \n");
                retVal = CSL_EFAIL;
            }

            if (IPFMA_OK == status)
            {
                UART_printf("\nRegister check done ...\r\n");
                UART_printf("All tests have passed!!\n");
            }
            else
            {
                UART_printf("\nRegister check fail ...\r\n");
                UART_printf("Some tests have failed!!\n");
            }

        }
    
    }
    else{
        UART_printf("\n[MSMC APP]: Board Init failed \n");
        retVal = CSL_EFAIL;
    }

    return retVal;
}

static int32_t MsmcApp_Init()
{
    Board_STATUS  status = BOARD_FAIL;
    Board_initCfg boardCfg;

    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_MODULE_CLOCK | BOARD_INIT_UNLOCK_MMR;
    status   = Board_init(boardCfg);

    return status;
}

static void MsmcApp_Delay(uint32_t wait_in_ms)
{
    while (wait_in_ms--) {
        asm("   NOP");
    }
}

static int32_t MsmcApp_SetFirewall()
{
    int32_t retVal   = CSL_PASS;
    uint32_t reqFlag = TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_EXCLUSIVE;
    uint32_t timeout = SCICLIENT_SERVICE_WAIT_FOREVER;
    struct  tisci_msg_fwl_set_firewall_region_req request;
    Sciclient_ReqPrm_t reqParam   = {0};
    Sciclient_RespPrm_t respParam = {0};

	request.fwl_id  = (uint32_t)CSL_STD_FW_NAVSS0_VIRTSS_NB_SLV0_MEM0_ID;
	request.fwl_id  = (uint32_t)CSL_STD_FW_NAVSS0_VIRTSS_NB_SLV0_MEM0_ID;
	request.region  = (uint32_t) 1U; /* Pick up any unused region : 1 */
    request.n_permission_regs = CSL_FW_NUM_CBASS_FW_EP_REGION_PERMISSION;
    request.control = (FW_REGION_ENABLE & CSL_CBASS_ISC_EP_REGION_CONTROL_ENABLE_MASK);
    request.permissions[0] = (FW_MCU_R5F0_PRIVID << CSL_CBASS_FW_EP_REGION_PERMISSION_PRIV_ID_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_DEBUG_SHIFT);
    request.permissions[1] = (FW_MCU_R5F0_PRIVID << CSL_CBASS_FW_EP_REGION_PERMISSION_PRIV_ID_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_DEBUG_SHIFT);
    request.permissions[2] = (FW_MCU_R5F0_PRIVID << CSL_CBASS_FW_EP_REGION_PERMISSION_PRIV_ID_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_SEC_USER_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_SUPV_DEBUG_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_WRITE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_READ_SHIFT)
                             | (0U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_CACHEABLE_SHIFT)
                             | (1U << CSL_CBASS_FW_EP_REGION_PERMISSION_NONSEC_USER_DEBUG_SHIFT);

    request.start_address     = CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;
    request.end_address       = CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE + CSL_COMPUTE_CLUSTER0_CLEC_REGS_SIZE;

    reqParam.messageType      = (uint16_t) TISCI_MSG_SET_FWL_REGION;
    reqParam.flags            = (uint32_t) reqFlag;
    reqParam.pReqPayload      = (const uint8_t *) &request;
    reqParam.reqPayloadSize   = (uint32_t) sizeof (request);
    reqParam.timeout          = (uint32_t) timeout;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) 0;
    respParam.respPayloadSize = (uint32_t) 0;


    if (((reqFlag & TISCI_MSG_FLAG_AOP) != TISCI_MSG_FLAG_AOP) && (reqFlag != 0U))
    {
        retVal = CSL_EFAIL;
    }
    if (retVal == CSL_PASS)
    {
        retVal = Sciclient_service(&reqParam, &respParam);
    }
    if ((retVal != CSL_PASS) || ((reqFlag != 0U) &&
        ((respParam.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)))
    {
        retVal = CSL_EFAIL;
    }

    return retVal;
}