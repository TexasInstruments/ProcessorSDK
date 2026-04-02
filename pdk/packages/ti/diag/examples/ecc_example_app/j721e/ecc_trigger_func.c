/*
 * ECC Example
 *
 * Error Correcting Code (ECC) Example Application
 *
 *  Copyright (c) 2020 Texas Instruments Incorporated
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
 *  \file ecc_trigger_func.c
 *
 *  \brief This file contains SoC-specific functions that need to be run as
 *         part of the ECC error event input triggers by the Error Correcting
 *         Code Module (ECC) application.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>

#include <ti/csl/soc.h>
#include <ti/csl/soc/cslr_soc_ecc_aggr.h>
#include <ti/csl/csl_cbass.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/sciclient/sciclient.h>

#include <ti/osal/HwiP.h>

#include <diag_utils.h>

#include <sdr_ecc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define FW_MCU_R5F0_PRIVID (96U)

#define CLEC_EVENT         CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void    eventHandler(void);
static int32_t CLEC_setFirewall(void);
static int32_t configCLEC(void);
static int32_t sendClecEvent(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* The following array defines trigger functions needed for each Use Case
 * (if needed). If no trigger functions are needed, then the array slot value
 * is set to NULL. */

int32_t (*useCaseTriggerFuncArray[6])() =
{
    NULL,
    NULL,
    NULL,
    sendClecEvent,
    NULL,
    sendClecEvent
};

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* This function performs any high-level event trigger setup that should be done
 * before any of the use cases in the ECC example application start. */
int32_t eventTriggerSetup(void)
{
    int32_t retValue = 0;
    CSL_ErrType_t status;

    /* Add firewall entry to gain access to CLEC registers */
    status = CLEC_setFirewall();
    if (status != CSL_PASS)
    {
        DIAG_printf(" CLEC_setFirewall failed \n");
        retValue = -1;
    }
    
    retValue = configCLEC();

    return retValue;
}

static int32_t configCLEC(void)
{
    int32_t retValue = 0;
    CSL_ErrType_t status;
    CSL_ClecEventConfig evtCfg;
    HwiP_Params hwiParams;
    HwiP_Handle hwiPHandle;

    evtCfg.secureClaimEnable = 1U;
    evtCfg.evtSendEnable     = 1U;
    evtCfg.extEvtNum         = CLEC_EVENT;
    evtCfg.rtMap             = 2U;
    evtCfg.c7xEvtNum         = 0U;

    /* Configure interrupt router to ensure that interrupt event will trigger ECC
     * interrept when ECC error is injected */
    status =  CSL_clecConfigEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                                  CLEC_EVENT,
                                  &evtCfg);
    if (status != CSL_PASS)
    {
        DIAG_printf(" CSL_clecConfigEvent failed \n");
        retValue = -1;
    }


    /* Disable interrupt */
    HwiP_disableInterrupt(CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_12);

    /* Default parameter initialization */
    HwiP_Params_init(&hwiParams);

#ifdef DEBUG_PRINT
    DIAG_printf("\n HwiP_Params_init complete \n");
#endif
    /* Register call back function for Interrupt */
    hwiPHandle = HwiP_create(CSLR_MCU_R5FSS0_CORE0_INTR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_OUT_LEVEL_12,
                             (HwiP_Fxn)eventHandler,
                             (void *)&hwiParams);
    if (hwiPHandle == NULL)
    {
        DIAG_printf(" Interrupt registration failed \n");
        retValue = -1;
    }

    return retValue;
}


void eventHandler(void)
{
    CSL_ErrType_t status;

#ifdef DEBUG_PRINT
    DIAG_printf("Got CLEC event\n");
#endif

    /* Clear C7x PBIST interrupt event in CLEC */
    status = CSL_clecClearEvent((CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE,
                   CSLR_COMPUTE_CLUSTER0_CLEC_MSMC_EVENT_IN_COMPUTE_CLUSTER0_CORE_CORE_MSMC_INTR_12);
    if (status != CSL_PASS) {
        DIAG_printf("eventHandler: Clearing CLEC event failed \n");
    }

    return;
}

static int32_t sendClecEvent(void)
{
    CSL_CLEC_EVTRegs *pRegs = (CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;
    int32_t           status;

    uint32_t evtNum = CLEC_EVENT;
    status = CSL_clecSendEvent(pRegs, evtNum);

    return status;
}

/* CLEC_setFirewall: Sets firewall settings to be able to access CLEC registers */
static int32_t CLEC_setFirewall(void)
{
    int32_t retVal = CSL_PASS;
    uint32_t reqFlag = TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_EXCLUSIVE;
    uint32_t timeout =  SCICLIENT_SERVICE_WAIT_FOREVER;
    struct  tisci_msg_fwl_set_firewall_region_req request;
    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;

    request.fwl_id       = (uint32_t)CSL_STD_FW_NAVSS0_NAV_SRAM0_ID;
    request.region = (uint32_t) 0U; /* Pick up any unused region : 0 */
    request.n_permission_regs = CSL_FW_NUM_CBASS_FW_EP_REGION_PERMISSION;
    request.control = (CSL_FW_REGION_ENABLE & CSL_CBASS_ISC_EP_REGION_CONTROL_ENABLE_MASK);
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
