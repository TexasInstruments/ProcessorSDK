/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 *  \file pbist_utils.c
 *
 *  \brief PBIST utility functions
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <sdl_types.h>
#if defined (SOC_J742S2)
#include <src/ip/pbist/V0/sdlr_pbist.h>
#include <src/ip/pbist/V0/sdl_ip_pbist.h>
#include <pbist/soc/j784s4/sdl_pbist.h>
#endif
#include <src/sdl/sdl_pbist.h>
#include <ti/csl/csl_rat.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_cbass.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/sciclient/sciclient.h>

#if !defined (SOC_J7200)
#include <ti/csl/csl_clec.h>
#endif

/* Osal API header files */
#include <ti/osal/TimerP.h>

#include "power_seq.h"

#include "pbist_utils.h"
#include "pbist_defs.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* This is to power up the cores before test and power down afterwards */
#define POWERUP_CORES_BEFORE_TEST

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* BootApp_pbistSetFirewall: Sets firewall settings to be able to access CLEC registers */

#if defined(SOC_J721E) || defined(SOC_J721S2)|| defined(SOC_J784S4) || defined(SOC_J742S2)


static int32_t BootApp_pbistSetFirewall(void)
{
    int32_t retVal = CSL_PASS;
    uint32_t reqFlag = TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_EXCLUSIVE;
    uint32_t timeout =  SCICLIENT_SERVICE_WAIT_FOREVER;
    struct  tisci_msg_fwl_set_firewall_region_req request;
    Sciclient_ReqPrm_t reqParam = {0};
    Sciclient_RespPrm_t respParam = {0};
    #if defined(SOC_J721E)
    request.fwl_id       = (uint32_t)CSL_STD_FW_NAVSS0_NAV_SRAM0_ID;
    #endif
	#if defined(SOC_J721S2)
	request.fwl_id       = (uint32_t)CSL_STD_FW_NAVSS0_VIRTSS_NB_SLV0_MEM0_ID;
	#endif
	#if defined(SOC_J784S4) || defined(SOC_J742S2)
	request.fwl_id       = (uint32_t)CSL_STD_FW_NAVSS0_VIRTSS_NB_SLV0_MEM0_ID;
	#endif	
	request.region = (uint32_t) 1U; /* Pick up any unused region : 1 */
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
    request.start_address = CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;
    request.end_address = CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE + CSL_COMPUTE_CLUSTER0_CLEC_REGS_SIZE;

    reqParam.messageType    = (uint16_t) TISCI_MSG_SET_FWL_REGION;
    reqParam.flags          = (uint32_t) reqFlag;
    reqParam.pReqPayload    = (const uint8_t *) &request;
    reqParam.reqPayloadSize = (uint32_t) sizeof (request);
    reqParam.timeout        = (uint32_t) timeout;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) 0;
    respParam.respPayloadSize = (uint32_t) 0;


    if (((reqFlag & TISCI_MSG_FLAG_AOP) != TISCI_MSG_FLAG_AOP)&&
        (reqFlag != 0U))
    {
        retVal = CSL_EFAIL;
    }
    if (retVal == CSL_PASS)
    {
        retVal = Sciclient_service(&reqParam, &respParam);
    }
    if ((retVal != CSL_PASS) ||
        ((reqFlag != 0U) &&
        ((respParam.flags & TISCI_MSG_FLAG_ACK) != TISCI_MSG_FLAG_ACK)))
    {
        retVal = CSL_EFAIL;
    }
    return retVal;
}
#endif

int32_t SDL_OSAL_getClecOutputEvent(uint32_t *clecOutputEvt)
{
    int32_t retVal = SDL_PASS;
#if defined(SOC_J721E) || defined(SOC_J721S2)
    *clecOutputEvt = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12;
#endif
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    *clecOutputEvt = 12;
#endif	
    return retVal;
}

int32_t SDL_OSAL_releaseClecOutputEvent(uint32_t clecOutputEvt)
{
    return SDL_PASS;
}

/* CLEC configuration for C7X and A72 instances */
int32_t BootApp_pbistClecConfig(uint32_t instance)
{
	#if defined(SOC_J721E)
    SDL_ErrType_t status = SDL_EFAIL;
    CSL_ClecEventConfig evtCfg;

    evtCfg.secureClaimEnable = 1U;
    evtCfg.evtSendEnable = 1U;
    evtCfg.rtMap = 2U;
    evtCfg.c7xEvtNum = 0U;

    if (PBIST_TestHandleArray[instance].pbistInst == SDL_PBIST_INST_A72)
    {
        evtCfg.extEvtNum = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12;
        /* Configure interrupt router to take care of routing A72 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_8,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent A72 failed \n");
        }
    }
    else if (PBIST_TestHandleArray[instance].pbistInst == SDL_PBIST_INST_C7X)
    {
        evtCfg.extEvtNum = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12;
        /* Configure interrupt router to take care of routing C7x PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7x failed \n");
        }
    }

    return status;
#endif

#if defined(SOC_J7200)
 return SDL_PASS;
#endif

#if defined(SOC_J721S2)
    SDL_ErrType_t status = SDL_EFAIL;
    CSL_ClecEventConfig evtCfg;

    evtCfg.secureClaimEnable = 1U;
    evtCfg.evtSendEnable = 1U;
    evtCfg.rtMap = 2U;
    evtCfg.c7xEvtNum = 0U;


    if (PBIST_TestHandleArray[instance].pbistInst == SDL_PBIST_INST_A72)
    {
        evtCfg.extEvtNum = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12;
        /* Configure interrupt router to take care of routing A72 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_8,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent A72 failed \n");
        }
    }
    else if (PBIST_TestHandleArray[instance].pbistInst == SDL_PBIST_INST_C7X_0)
    {
        evtCfg.extEvtNum = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12;
        /* Configure interrupt router to take care of routing C7x PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7x_0 failed \n");
        }
    }
    else if (PBIST_TestHandleArray[instance].pbistInst == SDL_PBIST_INST_C7X_1)
    {
        evtCfg.extEvtNum = CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12;
        /* Configure interrupt router to take care of routing C7x PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_13,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7x_1 failed \n");
        }
    }
    return status;
#endif
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    SDL_ErrType_t status = SDL_EFAIL;
    CSL_ClecEventConfig evtCfg;
    int32_t retValue = 0;
    if (retValue == 0)
    {
        evtCfg.secureClaimEnable = 1U;
        evtCfg.evtSendEnable = 1U;
        evtCfg.extEvtNum = 12;
        evtCfg.rtMap = 2U;
        evtCfg.c7xEvtNum = 0U;
        evtCfg.acDru = 0U;
   
        /* Configure interrupt router to take care of routing A72_0_0 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      8,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent A72_0_0 failed \n");
            retValue = -1;
        }
    }
   
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing A72_0_1 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      524,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent A72_0_1 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing A72_1_0 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      9,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent A72_1_0 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing A72_1_1 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      525,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent A72_1_1 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing C7X_0 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      12,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7X_0 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing C7x_1 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      13,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7x_1 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing C7X_2 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      14,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7X_2 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing C7x_3 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      22,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent C7x_3 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing ANA_0 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      520,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent ANA_0 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing ANA_1 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      521,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent ANA_1 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing ANA_2 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      522,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent ANA_2 failed \n");
            retValue = -1;
        }
    }
    if (retValue == 0)
    {
        evtCfg.extEvtNum = 12;
        /* Configure interrupt router to take care of routing ANA_3 PBIST interrupt event */
        status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                      523,
                                      &evtCfg);
        if (status != CSL_PASS)
        {
            UART_printf(" CSL_clecConfigEvent ANA_3 failed \n");
            retValue = -1;
        }
    }
   
    return status;
   // return SDL_PASS;
#endif
}

/* Captures common Initialization: currently initializes CLEC interrupt routing
   for C7x & A72 */
int32_t BootApp_pbistCommonInit(void)
{
 #if defined(SOC_J721E) || defined(SOC_J721S2)|| defined(SOC_J784S4) || defined(SOC_J742S2)
    CSL_ErrType_t status;
    /* Add firewall entry to gain access to CLEC registers */
    status = BootApp_pbistSetFirewall();

    if (status != CSL_PASS)
    {
        UART_printf( " BootApp_pbistSetFirewall failed \n");
    }

    return status;
#endif

#if defined(SOC_J7200)
	return SDL_PASS;
#endif
}


/* This function runs the full PBIST test for a particular section.
 * It includes the following steps (each step has many sub-steps specificied
 * in more details within the function).
 * For HW POST PBIST:
 * Step 1: Configure interrupt handler
 * Step 2: Check POST results (already run at startup)
 *
 * For SW-initiated PBIST:
 * Step 1: Configure interrupt handler
 * Step 2: Configure processor to correct state
 * Step 3: Run PBIST test (includes checking the result)
 * Step 4: Restore cores
 * */

int32_t BootApp_pbistRunTest(uint32_t instanceId, uint8_t test)
{
    int32_t testResult = 0;
    SDL_ErrType_t status;
    bool PBISTResult = true;
    SDL_PBIST_testType testType;
    uint32_t i = 0;

    uint32_t moduleState = TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF;
    uint32_t resetState = 0U;
    uint32_t contextLossState = 0U;

#if defined(PROFILE_PBIST_TIMING)
    uint64_t startTime , testStartTime,  testEndTime, endTime;
    uint64_t prepTime, diffTime, restoreTime;
#endif

    #if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
        UART_printf("\n Starting PBIST test on %s, index %d...\n",
                    PBIST_TestHandleArray[instanceId].testName,
                    instanceId);
        testType = SDL_PBIST_TEST;
    #else
        UART_printf("\n Starting PBIST test on %s, index %d...\n",
                    PBIST_TestHandleArray[instanceId].testName,
                    instanceId);
        testType = SDL_PBIST_TEST;
    #endif

#if defined(PROFILE_PBIST_TIMING)
    /* Get start time of test */
    startTime = TimerP_getTimeInUsecs();
#endif

    /* Step 1: (if HW Power-On Self Test, i.e. POST) Check POST results  */
    if ((testResult == 0) &&
        (PBIST_TestHandleArray[instanceId].numPostPbistToCheck > 0))
    {
        if (test == 0u)
        {
#ifdef DEBUG
            UART_printf("  HW POST: Running test on HW POST, %d Instances \n",
                        PBIST_TestHandleArray[instanceId].numPostPbistToCheck);
#endif
            SDL_PBIST_postResult result;

            status = SDL_PBIST_getPOSTStatus(&result);

            if (status != SDL_PASS)
            {
                testResult = -1;
                UART_printf("    SDL_PBIST_getPOSTStatus failed: Status %d \n", status);
            }
            else
            {
                PBIST_printPostStatus(&result);
            }
        }
    }
    else
    {
        /* Step 2: (if SW-initiated PBIST) Configure processor to correct state  */

        /**--- Step 2a: Request Primary core ---*/
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            if (PBIST_TestHandleArray[instanceId].tisciProcId != 0u)
            {
#ifdef DEBUG
                UART_printf("  Primary core: %s: Requesting processor \n",
                            PBIST_TestHandleArray[instanceId].coreName);
#endif
                /* Request Primary core */
                status = Sciclient_procBootRequestProcessor(PBIST_TestHandleArray[instanceId].tisciProcId,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciProcId);
                    testResult = -1;
                }
            }
        }

        /**--- Step 2b: Request Secondary core ---*/
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            if ((PBIST_TestHandleArray[instanceId].secondaryCoreNeeded)
                && (PBIST_TestHandleArray[instanceId].tisciSecProcId != 0u))
            {

#ifdef DEBUG
                UART_printf("  Secondary core: %s: Requesting processor \n",
                            PBIST_TestHandleArray[instanceId].secCoreName);
#endif
                /* Request secondary core */
                status = Sciclient_procBootRequestProcessor(PBIST_TestHandleArray[instanceId].tisciSecProcId,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciSecProcId);
                    testResult = -1;
                }
            }
        }

#if defined (SOC_J784S4) || defined (SOC_J742S2)
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
    {
        if ((PBIST_TestHandleArray[instanceId].thirdCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciThProcId != 0u))
        {

#ifdef DEBUG
            UART_printf("  Third core: %s: Requesting processor \n",
                    PBIST_TestHandleArray[instanceId].thCoreName);
#endif
            /* Request third core */
            status = Sciclient_procBootRequestProcessor(PBIST_TestHandleArray[instanceId].tisciThProcId,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Third core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciThProcId);
                testResult = -1;
            }
        }
    }

    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
    {
        if ((PBIST_TestHandleArray[instanceId].fourthCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciFoProcId != 0u))
        {

#ifdef DEBUG
            UART_printf("  Fourth core: %s: Requesting processor \n",
                    PBIST_TestHandleArray[instanceId].foCoreName);
#endif
            /* Request fourth core */
            status = Sciclient_procBootRequestProcessor(PBIST_TestHandleArray[instanceId].tisciFoProcId,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Fourth core: Sciclient_procBootRequestProcessor, ProcId 0x%x...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciFoProcId);
                testResult = -1;
            }
        }
    }
#endif
        /**--- Step 2c: Put Primary core in local reset ---*/
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            if (PBIST_TestHandleArray[instanceId].tisciDeviceId != 0u)
            {
                /* Set Local reset for Primary core */
#ifdef DEBUG
                UART_printf("  %s: Primary core: Set module reset \n",
                            PBIST_TestHandleArray[instanceId].coreName);
#endif
                status =  Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciDeviceId,
                                                   0x1, /* Local Reset asserted */
                                                   SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_pmSetModuleRst...FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 2d: Put Secondary core in local reset ---*/
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            if ((PBIST_TestHandleArray[instanceId].secondaryCoreNeeded)
                && (PBIST_TestHandleArray[instanceId].tisciSecDeviceId != 0u))
            {
                /* Set Local reset for Secondary core */
#ifdef DEBUG
                UART_printf("  %s: Secondary core: Set Module reset \n",
                            PBIST_TestHandleArray[instanceId].secCoreName);
#endif
                status =  Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciSecDeviceId,
                                                   0x1, /* Local Reset asserted */
                                                   SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleRst...FAILED \n");
                    testResult = -1;
                }
            }
        }
		
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
    {
        if ((PBIST_TestHandleArray[instanceId].thirdCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciThDeviceId != 0u))
        {
            /* Set Local reset for Third core */
#ifdef DEBUG
            UART_printf("  %s: Third core: Set Module reset \n",
                        PBIST_TestHandleArray[instanceId].thCoreName);
#endif
            status =  Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciThDeviceId,
                                               0x1, /* Local Reset asserted */
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Third core: Sciclient_pmSetModuleRst...FAILED \n");
                testResult = -1;
            }
        }
    }

    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
    {
        if ((PBIST_TestHandleArray[instanceId].fourthCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciFoDeviceId != 0u))
        {
            /* Set Local reset for Fourth core */
#ifdef DEBUG
            UART_printf("  %s: Fourth core: Set Module reset \n",
                        PBIST_TestHandleArray[instanceId].foCoreName);
#endif
            status =  Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciFoDeviceId,
                                               0x1, /* Local Reset asserted */
                                               SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Fourth core: Sciclient_pmSetModuleRst...FAILED \n");
                testResult = -1;
            }
        }
    }
#endif		
#ifdef POWERUP_CORES_BEFORE_TEST
        /**--- Step 2e: Perform any additional power sequencing, if needed ---*/
        /* Custom core power restore sequence - needed to allow core to be powered
         * up later by Secondary Bootloader (SBL) */
        if ((testResult == 0) &&
            (PBIST_TestHandleArray[instanceId].coreCustPwrSeqNeeded) &&
            (PBIST_TestHandleArray[instanceId].tisciProcId != 0u))
        {
            status = BootApp_customPrepareForPowerUpSequence(PBIST_TestHandleArray[instanceId].tisciProcId);
            if (status != CSL_PASS)
            {
                UART_printf("  Custom core power restore sequence, ProcId 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciProcId);
                testResult = -1;
            }
        }

        /**--- Step 2f: Power up of Auxilliary modules needed to run test */
        if (testResult == 0)
        {
            /* Power all modules required for test */
            for ( i = 0; i < PBIST_TestHandleArray[instanceId].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Powering on Device number %d Device Id %x\n",
                            i, PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i]);
#endif

                status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i],
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i]);
                    testResult = -1;
                    break;
                }
            }
        }

        /**--- Step 2g: Power up Primary core */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                              && (PBIST_TestHandleArray[instanceId].tisciDeviceId != 0U))
        {
            /* power on Primary core*/
#ifdef DEBUG
            UART_printf("  Primary core: Powering on %s \n",
                        PBIST_TestHandleArray[instanceId].coreName);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

            if (status != CSL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciDeviceId);
                testResult = -1;
            }
        }

        /**--- Step 2h: Power up Secondary core */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                              && (PBIST_TestHandleArray[instanceId].tisciSecDeviceId != 0U))
        {
            if (PBIST_TestHandleArray[instanceId].secondaryCoreNeeded)
            {
                /* power on Secondary core*/
#ifdef DEBUG
                UART_printf("  Secondary core: Powering on %s \n",
                            PBIST_TestHandleArray[instanceId].secCoreName);
#endif
                status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciSecDeviceId,
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciSecDeviceId);
                    testResult = -1;
                    return testResult;
                }
            }
        }

#if defined(SOC_J784S4) || defined(SOC_J742S2)
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciThDeviceId != 0U))
    {
        if (PBIST_TestHandleArray[instanceId].thirdCoreNeeded)
        {
            /* power on Third core*/
#ifdef DEBUG
            UART_printf("  Third core: Powering on %s \n",
                        PBIST_TestHandleArray[instanceId].thCoreName);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciThDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Third core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciThDeviceId);
                testResult = -1;
                return testResult;
            }
        }
    }

    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciFoDeviceId != 0U))
    {
        if (PBIST_TestHandleArray[instanceId].fourthCoreNeeded)
        {
            /* power on Fourth core*/
#ifdef DEBUG
            UART_printf("  Fourth core: Powering on %s \n",
                        PBIST_TestHandleArray[instanceId].foCoreName);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciFoDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Fourth core: Sciclient_pmSetModuleState 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciFoDeviceId);
                testResult = -1;
                return testResult;
            }
        }
    }
#endif
        /**--- Step 2i: Double check the Power up of Auxilliary modules needed to run test
         * and wait until they are powered up */
        if (testResult == 0)
        {
            /* Wait for all modules required for test to be powered up */
            for ( i = 0; i < PBIST_TestHandleArray[instanceId].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Double checking Powering on Device number %d Device Id %x\n",
                            i, PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i]);
#endif
                do
                {
                    status = Sciclient_pmGetModuleState(PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i],
                                                        &moduleState,
                                                        &resetState,
                                                        &contextLossState,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
                    if (status != CSL_PASS)
                    {
                        UART_printf("  Sciclient_pmGetModuleState 0x%x ...FAILED \n",
                                    PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i]);
                        testResult = -1;
                        break;
                    }
                } while (moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_ON);
            }
        }

        /**--- Step 2j: Double check the Power up of Primary core and wait until it is
         * powered up */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                              && (PBIST_TestHandleArray[instanceId].tisciDeviceId != 0U))
        {
            /* Double check power on Primary core*/
#ifdef DEBUG
            UART_printf("  Primary core: Double checking Powering on %s \n",
                        PBIST_TestHandleArray[instanceId].coreName);
#endif
            do
            {
                status = Sciclient_pmGetModuleState(PBIST_TestHandleArray[instanceId].tisciDeviceId,
                                                    &moduleState,
                                                    &resetState,
                                                    &contextLossState,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_pmGetModuleState 0x%x ...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciDeviceId);
                    testResult = -1;
                    break;
                }
            } while (moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_ON);
        }

        /**--- Step 2k: Double check the Power up of Primary core and wait until it is
         * powered up */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                              && (PBIST_TestHandleArray[instanceId].tisciSecDeviceId != 0U))
        {
            if (PBIST_TestHandleArray[instanceId].secondaryCoreNeeded)
            {
                /* Double check power on Secondary core*/
#ifdef DEBUG
                UART_printf("  Secondary core: Double checking Powering on %s \n",
                            PBIST_TestHandleArray[instanceId].coreName);
#endif
                do
                {
                    status = Sciclient_pmGetModuleState(PBIST_TestHandleArray[instanceId].tisciSecDeviceId,
                                                        &moduleState,
                                                        &resetState,
                                                        &contextLossState,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
                    if (status != CSL_PASS)
                    {
                        UART_printf("   Secondary core: Sciclient_pmGetModuleState 0x%x ...FAILED \n",
                                    PBIST_TestHandleArray[instanceId].tisciSecDeviceId);
                        testResult = -1;
                        break;
                    }
                } while (moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_ON);
            }
        }

#if defined(SOC_J784S4) || defined(SOC_J742S2)
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciThDeviceId != 0U))
    {
        if (PBIST_TestHandleArray[instanceId].thirdCoreNeeded)
        {
            /* Double check power on Third core*/
#ifdef DEBUG
            UART_printf(
                            "  Third core: Double checking Powering on %s \n",
                            PBIST_TestHandleArray[instanceId].thCoreName);
#endif
            do
            {
                status = Sciclient_pmGetModuleState(PBIST_TestHandleArray[instanceId].tisciThDeviceId,
                                                    &moduleState,
                                                    &resetState,
                                                    &contextLossState,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != SDL_PASS)
                {
                    UART_printf("   Third core: Sciclient_pmGetModuleState 0x%x ...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciThDeviceId);
                    testResult = -1;
                    break;
                }
            } while (moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_ON);
        }
    }

    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciFoDeviceId != 0U))
    {
        if (PBIST_TestHandleArray[instanceId].fourthCoreNeeded)
        {
            /* Double check power on Fourth core*/
#ifdef DEBUG
            UART_printf(
                            "  Fourth core: Double checking Powering on %s \n",
                            PBIST_TestHandleArray[instanceId].foCoreName);
#endif
            do
            {
                status = Sciclient_pmGetModuleState(PBIST_TestHandleArray[instanceId].tisciFoDeviceId,
                                                    &moduleState,
                                                    &resetState,
                                                    &contextLossState,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != SDL_PASS)
                {
                    UART_printf("   Fourth core: Sciclient_pmGetModuleState 0x%x ...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciFoDeviceId);
                    testResult = -1;
                    break;
                }
            } while (moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_ON);
        }
    }
#endif
#endif /* #ifdef POWERUP_CORES_BEFORE_TEST */
        /**--- Step 2l: Power up PBIST */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId != 0u))
        {
#ifdef DEBUG
            UART_printf("  Powering on PBIST %d \n",
                        PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

            if (status != CSL_PASS)
            {
                UART_printf("   PBIST Sciclient_pmSetModuleState 0x%x ...FAILED: retValue %d\n",
                            PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId, status);
                testResult = -1;
            }
        }

        /**--- Step 2m: Execute Auxialliary init function for any final core-internal register
         * setup needed for the PBIST test */
        if (testResult == 0)
        {
            if (PBIST_TestHandleArray[instanceId].auxInitRestoreFunction != 0)
            {
                status = PBIST_TestHandleArray[instanceId].auxInitRestoreFunction(TRUE);
                if (status != CSL_PASS)
                {
                    testResult = -1;
                }
            }
        }

#if defined(PROFILE_PBIST_TIMING)
        /* Get start time for PBIST test */
        testStartTime = TimerP_getTimeInUsecs();
#endif

        /**-- Step 3: Run PBIST test. --*/
        status = SDL_PBIST_selfTest((SDL_PBIST_inst)PBIST_TestHandleArray[instanceId].pbistInst, testType, NULL, &PBISTResult);
        if ((status != SDL_PASS) || (PBISTResult == false))
        {
            testResult = -1;
        }

#if defined(PROFILE_PBIST_TIMING)
        /* Record test end time */
        testEndTime = TimerP_getTimeInUsecs();
#endif

        /**-- Step 4: Restore cores --*/

        /**--- Step 4a: Execute Auxilliary restore function to restore core-internal
         * registers to original state */
        if (testResult == 0)
        {

            if (PBIST_TestHandleArray[instanceId].auxInitRestoreFunction != 0)
            {
                status = PBIST_TestHandleArray[instanceId].auxInitRestoreFunction(FALSE);
                if (status != CSL_PASS)
                {
                    testResult = -1;
                }
            }
        }

        /**--- Step 4b: Power off PBIST */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId != 0u))
        {
#ifdef DEBUG
            UART_printf("  Powering off PBIST %d \n",
                        PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

            if (status != CSL_PASS)
            {
                status = Sciclient_pmGetModuleState(PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId,
                                                    &moduleState,
                                                    &resetState,
                                                    &contextLossState,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if ((status != CSL_PASS) || (TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF != moduleState))
                {
                    UART_printf("   PBIST Sciclient_pmSetModuleState 0x%x ...FAILED \n",PBIST_TestHandleArray[instanceId].tisciPBISTDeviceId);
                    testResult = -1;
                }
            }
        }
#ifdef POWERUP_CORES_BEFORE_TEST
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciFoDeviceId != 0U))
    {
        if (PBIST_TestHandleArray[instanceId].fourthCoreNeeded)
        {
            /* power off Fourth core*/
#ifdef DEBUG
            UART_printf("  Fourth core: Powering off %s \n",
                        PBIST_TestHandleArray[instanceId].foCoreName);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciFoDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Fourth core: Sciclient_pmSetModuleState Power off 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciFoDeviceId);
                testResult = -1;
                return testResult;
            }
        }
    }

    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciThDeviceId != 0U))
    {
        if (PBIST_TestHandleArray[instanceId].thirdCoreNeeded)
        {
            /* power off Third core*/
#ifdef DEBUG
            UART_printf("  Third core: Powering off %s \n",
                        PBIST_TestHandleArray[instanceId].thCoreName);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciThDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Third core: Sciclient_pmSetModuleState Power off 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciThDeviceId);
                testResult = -1;
                return testResult;
            }
        }
    }
#endif

        /**--- Step 4c: Power off Secondary core */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                              && (PBIST_TestHandleArray[instanceId].tisciSecDeviceId != 0U))
        {
            if (PBIST_TestHandleArray[instanceId].secondaryCoreNeeded)
            {
                /* power off Secondary core*/
#ifdef DEBUG
                UART_printf("  Secondary core: Powering off %s \n",
                            PBIST_TestHandleArray[instanceId].secCoreName);
#endif
                status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciSecDeviceId,
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_pmSetModuleState Power off 0x%x ...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciSecDeviceId);
                    testResult = -1;
                    return testResult;
                }
            }
        }

        /**--- Step 4d: Perform any custom/core-specific power down sequence */
        if ((testResult == 0) &&
            (PBIST_TestHandleArray[instanceId].coreCustPwrSeqNeeded) &&
            (PBIST_TestHandleArray[instanceId].tisciProcId != 0u))
        {
            status = BootApp_customPowerDownSequence(PBIST_TestHandleArray[instanceId].tisciProcId);
            if (status != CSL_PASS)
            {
                UART_printf("  Custom core power down sequence, ProcId 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciProcId);
                testResult = -1;
            }
        }

        /**--- Step 4e: Power off Primary core */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].tisciProcId != 0u)
                        && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            /* power off Primary core*/
#ifdef DEBUG
            UART_printf("  Primary core: Powering off %s \n",
                        PBIST_TestHandleArray[instanceId].coreName);
#endif
            status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciDeviceId,
                                                TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                TISCI_MSG_FLAG_AOP,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);

            if (status != CSL_PASS)
            {
                UART_printf("   Primary core: Sciclient_pmSetModuleState Power off 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciDeviceId);
                testResult = -1;
            }
        }

#if !defined(SOC_J784S4) && !defined(SOC_J742S2)
        /**--- Step 4f: Power off of Auxilliary modules needed to run test */
        if (testResult == 0)
        {
            /* Power all modules required for test */
            for ( i = 0; i < PBIST_TestHandleArray[instanceId].numAuxDevices; i++)
            {
#ifdef DEBUG
                UART_printf("  Powering off Device number %d Device Id %x\n",
                            i, PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i]);
#endif
                status = Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i],
                                                    TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                    TISCI_MSG_FLAG_AOP,
                                                    SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    status = Sciclient_pmGetModuleState(PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i],
                                                        &moduleState,
                                                        &resetState,
                                                        &contextLossState,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
                    if ((status != CSL_PASS) || (moduleState != TISCI_MSG_VALUE_DEVICE_HW_STATE_OFF))
                    {
                        testResult = -1;
                        UART_printf("  Sciclient_pmSetModuleState 0x%x ...FAILED \n",PBIST_TestHandleArray[instanceId].auxDeviceIdsP[i]);
                        break;
                    }
                }
            }
        }
#endif

        /**--- Step 4g: Perform any custom/core-specific power restore sequence needed to
         * allow core to be powered up properly later */
        if ((testResult == 0) &&
            (PBIST_TestHandleArray[instanceId].coreCustPwrSeqNeeded) &&
            (PBIST_TestHandleArray[instanceId].tisciProcId != 0u))
        {
            status = BootApp_customPrepareForPowerUpSequence(PBIST_TestHandleArray[instanceId].tisciProcId);
            if (status != CSL_PASS)
            {
                UART_printf("  Custom core power restore sequence, ProcId 0x%x ...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciProcId);
                testResult = -1;
            }
        }

        /**--- Step 4h: Take Primary core out of local reset */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                              && (PBIST_TestHandleArray[instanceId].tisciDeviceId != 0U))
        {
#ifdef DEBUG
            UART_printf("  Primary core: Taking out of local reset the core %s \n",
                        PBIST_TestHandleArray[instanceId].coreName);
#endif
            status = Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciDeviceId,
                                              0x0, /* Local Reset de-asserted */
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                 UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                             PBIST_TestHandleArray[instanceId].tisciDeviceId);
                 testResult = -1;
            }
        }

        /**--- Step 4i: Take Secondary core out of local reset */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                              && (PBIST_TestHandleArray[instanceId].tisciSecDeviceId != 0U))
        {
#ifdef DEBUG
            UART_printf("  Secondary core: Taking out of local reset the core %s \n",
                        PBIST_TestHandleArray[instanceId].secCoreName);
#endif
            status = Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciSecDeviceId,
                                              0x0, /* Local Reset de-asserted */
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                 UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                             PBIST_TestHandleArray[instanceId].tisciSecDeviceId);
                 testResult = -1;
            }
        }
#if defined(SOC_J784S4) || defined(SOC_J742S2)
    /* Take Third core out of local reset */
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciThDeviceId != 0U))
    {
#ifdef DEBUG
        UART_printf("  Third core: Taking out of local reset the core %s \n",
                    PBIST_TestHandleArray[instanceId].thCoreName);
#endif
        status = Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciThDeviceId,
                                          0x0, /* Local Reset de-asserted */
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status != SDL_PASS)
        {
             UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                         PBIST_TestHandleArray[instanceId].tisciThDeviceId);
             testResult = -1;
        }
    }

    /* Take Fourth core out of local reset */
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded)
                          && (PBIST_TestHandleArray[instanceId].tisciFoDeviceId != 0U))
    {
#ifdef DEBUG
        UART_printf("  Third core: Taking out of local reset the core %s \n",
                    PBIST_TestHandleArray[instanceId].foCoreName);
#endif
        status = Sciclient_pmSetModuleRst(PBIST_TestHandleArray[instanceId].tisciFoDeviceId,
                                          0x0, /* Local Reset de-asserted */
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
        if (status != SDL_PASS)
        {
             UART_printf("  Sciclient_pmSetModuleRst 0x%x ...FAILED \n",
                         PBIST_TestHandleArray[instanceId].tisciFoDeviceId);
             testResult = -1;
        }
    }
#endif		
#endif /* #ifdef POWERUP_CORES_BEFORE_TEST */
        /* Ensure that cores have been turned off to prepare for booting of the cores */

        /**--- Step 4j: Power off Primary core */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            if (PBIST_TestHandleArray[instanceId].tisciDeviceId != 0u)
            {
                /* Set Software Reset Disable State for Primary core */
#ifdef DEBUG
                UART_printf("  %s: Primary core: Put in Software Reset Disable \n",
                            PBIST_TestHandleArray[instanceId].coreName);
#endif
                status =  Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciDeviceId,
                                                     TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                     TISCI_MSG_FLAG_AOP,
                                                     SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Primary core: Sciclient_pmSetModuleState...FAILED \n");
                    testResult = -1;
                }
            }
        }

        /**--- Step 4k: Power off Secondary core */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            if ((PBIST_TestHandleArray[instanceId].secondaryCoreNeeded)
                && (PBIST_TestHandleArray[instanceId].tisciSecDeviceId != 0u))
            {
                /* Set Software Reset Disable State for Secondary core */
#ifdef DEBUG
                UART_printf("  %s: Secondary Core Put in Software Reset Disable \n",
                            PBIST_TestHandleArray[instanceId].secCoreName);
#endif
                status =  Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciSecDeviceId,
                                                     TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                     TISCI_MSG_FLAG_AOP,
                                                     SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core Sciclient_pmSetModuleState...FAILED \n");
                    testResult = -1;
                }
            }
        }

#if defined(SOC_J784S4) || defined(SOC_J742S2)
    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
    {
        if ((PBIST_TestHandleArray[instanceId].thirdCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciThDeviceId != 0u))
        {
            /* Set Software Reset Disable State for Third core */
#ifdef DEBUG
            UART_printf("  %s: Third Core Put in Software Reset Disable \n",
                        PBIST_TestHandleArray[instanceId].thCoreName);
#endif
            status =  Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciThDeviceId,
                                                 TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                 TISCI_MSG_FLAG_AOP,
                                                 SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Third core Sciclient_pmSetModuleState...FAILED \n");
                testResult = -1;
            }
        }
    }

    if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].procRstNeeded))
    {
        if ((PBIST_TestHandleArray[instanceId].fourthCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciFoDeviceId != 0u))
        {
            /* Set Software Reset Disable State for Fourth core */
#ifdef DEBUG
            UART_printf("  %s: Fourth Core Put in Software Reset Disable \n",
                        PBIST_TestHandleArray[instanceId].foCoreName);
#endif
            status =  Sciclient_pmSetModuleState(PBIST_TestHandleArray[instanceId].tisciFoDeviceId,
                                                 TISCI_MSG_VALUE_DEVICE_SW_STATE_AUTO_OFF,
                                                 TISCI_MSG_FLAG_AOP,
                                                 SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Fourth core Sciclient_pmSetModuleState...FAILED \n");
                testResult = -1;
            }
        }
    }
#endif		

        /**--- Step 4m: Release Primary core */
        if ((testResult == 0) && (PBIST_TestHandleArray[instanceId].tisciProcId != 0u)
                && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            /* release processor Primary core */
#ifdef DEBUG
            UART_printf("  Primary core: Releasing %s \n",
                        PBIST_TestHandleArray[instanceId].coreName);
#endif

            status = Sciclient_procBootReleaseProcessor(PBIST_TestHandleArray[instanceId].tisciProcId,
                                                        TISCI_MSG_FLAG_AOP,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != CSL_PASS)
            {
                UART_printf("   Primary core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciProcId);
                testResult = -1;
            }
        }    

        /**--- Step 4n: Release Secondary core */
        if (testResult == 0)
        {
            if ((PBIST_TestHandleArray[instanceId].secondaryCoreNeeded)
                && (PBIST_TestHandleArray[instanceId].tisciSecProcId != 0u)
                && (PBIST_TestHandleArray[instanceId].procRstNeeded))
            {
                /* release processor Secondary core */
#ifdef DEBUG
                UART_printf("  Secondary core: Releasing %s \n",
                            PBIST_TestHandleArray[instanceId].secCoreName);
#endif
                status = Sciclient_procBootReleaseProcessor(PBIST_TestHandleArray[instanceId].tisciSecProcId,
                                                            TISCI_MSG_FLAG_AOP,
                                                            SCICLIENT_SERVICE_WAIT_FOREVER);
                if (status != CSL_PASS)
                {
                    UART_printf("   Secondary core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                                PBIST_TestHandleArray[instanceId].tisciSecProcId);
                    testResult = -1;
                }
            }
        }

#if defined(SOC_J784S4) || defined(SOC_J742S2)
    if (testResult == 0)
    {
        if ((PBIST_TestHandleArray[instanceId].thirdCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciThProcId != 0u)
            && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            /* release processor Third core */
#ifdef DEBUG
            UART_printf("  Third core: Releasing %s \n",
                        PBIST_TestHandleArray[instanceId].thCoreName);
#endif
            status = Sciclient_procBootReleaseProcessor(PBIST_TestHandleArray[instanceId].tisciThProcId,
                                                        TISCI_MSG_FLAG_AOP,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Third core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciThProcId);
                testResult = -1;
            }
        }
    }

    if (testResult == 0)
    {
        if ((PBIST_TestHandleArray[instanceId].fourthCoreNeeded)
            && (PBIST_TestHandleArray[instanceId].tisciFoProcId != 0u)
            && (PBIST_TestHandleArray[instanceId].procRstNeeded))
        {
            /* release processor Fourth core */
#ifdef DEBUG
            UART_printf("  Fourth core: Releasing %s \n",
                        PBIST_TestHandleArray[instanceId].foCoreName);
#endif
            status = Sciclient_procBootReleaseProcessor(PBIST_TestHandleArray[instanceId].tisciFoProcId,
                                                        TISCI_MSG_FLAG_AOP,
                                                        SCICLIENT_SERVICE_WAIT_FOREVER);
            if (status != SDL_PASS)
            {
                UART_printf("   Fourth core: Sciclient_procBootReleaseProcessor, ProcId 0x%x...FAILED \n",
                            PBIST_TestHandleArray[instanceId].tisciFoProcId);
                testResult = -1;
            }
        }
    }
#endif
#if defined(PROFILE_PBIST_TIMING)
        /* Record end time */
        endTime = TimerP_getTimeInUsecs();

        prepTime = testStartTime - startTime;
        diffTime = testEndTime - testStartTime;
        restoreTime = endTime - testEndTime;
        UART_printf("  Delta Cores prep time in micro secs %d \n",
                    (uint32_t)prepTime);
         UART_printf("  Delta PBIST execution time in micro secs %d \n",
                    (uint32_t)diffTime);
        UART_printf("  Delta Cores restore time in micro secs %d \n",
                    (uint32_t)restoreTime);
        UART_printf(" PBIST complete %s, test index %d\n",
                    PBIST_TestHandleArray[instanceId].testName,
                    instanceId);
#endif
    }

    return (testResult);
}
