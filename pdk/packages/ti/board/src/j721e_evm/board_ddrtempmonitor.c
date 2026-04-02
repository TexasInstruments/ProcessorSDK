/******************************************************************************
 * Copyright (c) 2019-2022 Texas Instruments Incorporated - http://www.ti.com
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
#include <ti/csl/csl_lpddr.h>
#include <ti/csl/arch/csl_arch.h>
#include "board_ddr.h"

#ifdef BUILD_MCU
typedef struct Board_DDRThermalMgmtInstance_s
{
    LPDDR4_Config      boardDDRCfg;
    LPDDR4_PrivateData boardRuntimeDDRPd;
    uint32_t           boardDDRInitRefreshRate[LPDDR4_FSP_2+1];
    uint32_t           boardDDRTrasMax[LPDDR4_FSP_2+1];
    HwiP_Handle        boardTempInterruptHandle;
    Board_thermalMgmtCallbackFunction_t appCallBackFunction;
    uint16_t           devIdCore;
    uint16_t           devIdIr;
} Board_DDRThermalMgmtInstance_t;

static Board_DDRThermalMgmtInstance_t __attribute__((section(".data:BOARD_DDR_thermalManagement"))) gBoard_DDRThermalMgmtInstance;

/* Local defines */
#define BOARD_SCICLIENT_RESP_TIMEOUT 1000000

/* Multiplication factors assumes scaling by 8 */
static const uint32_t __attribute__((section(".const:BOARD_DDR_thermalManagement"))) gRefreshRateMultFactor[BOARD_MAX_TEMP_CHECK_REFRESH_RATE_VALUE+1U] =
{
    32U,  /* 4 x */
    32U,  /* 4 x */
    16U,  /* 2 x */
    8U,   /* 1 x */
    4U,   /* 0.5 x */
    2U,   /* 0.25 x */
    2U,   /* 0.25 x with derating */
    2U,   /* 0.25 x with derating */
};

static const LPDDR4_CtlFspNum __attribute__((section(".const:BOARD_DDR_thermalManagement"))) gBoardDDRFSPNum[LPDDR4_FSP_2+1] =
{
    LPDDR4_FSP_0,
    LPDDR4_FSP_1,
    LPDDR4_FSP_2,
};

void __attribute__((section(".text:BOARD_DDR_thermalManagement"))) Board_updateRefreshRate(const LPDDR4_CtlFspNum fsNum, uint32_t refreshMultFactor)
{
    uint32_t refreshRate;
    uint32_t trasMax;
    uint32_t status;

    /* Calculate refresh rate */
    refreshRate = (gBoard_DDRThermalMgmtInstance.boardDDRInitRefreshRate[fsNum] * refreshMultFactor) >> 3U;
    trasMax = (gBoard_DDRThermalMgmtInstance.boardDDRTrasMax[fsNum] * refreshMultFactor) >> 3U;

    /* Take action to update Refresh rate */
    status = LPDDR4_SetRefreshRate(&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), &fsNum, &refreshRate, &trasMax);

    if (status > 0U)
    {
        BOARD_DEBUG_LOG("LPDDR4_GetRefreshRate: FAIL\n");
        /* Add Assert if needed*/
    }
}

static void __attribute__((section(".text:BOARD_DDR_thermalManagement"))) Board_DDRSetDevId()
{ 
    CSL_ArmR5CPUInfo info = {0};

    CSL_armR5GetCpuID(&info);
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 == info.grpId)
    {
        /* MCU SS Pulsar R5 SS */
        /* For R5 cores in the MCU domain MAIN2MCU_LVL_INTRTR0 is the base interrupt to the VIM. */
        gBoard_DDRThermalMgmtInstance.devIdIr   = TISCI_DEV_MAIN2MCU_LVL_INTRTR0;
        gBoard_DDRThermalMgmtInstance.devIdCore = (CSL_ARM_R5_CPU_ID_0 == info.cpuID)?
                                                        TISCI_DEV_MCU_R5FSS0_CORE0:
                                                        TISCI_DEV_MCU_R5FSS0_CORE1;
    }
    else if (CSL_ARM_R5_CLUSTER_GROUP_ID_1 == info.grpId)
    {
        /* MAIN SS Pulsar R5 SS0 */
        gBoard_DDRThermalMgmtInstance.devIdIr   = TISCI_DEV_R5FSS0_INTROUTER0;
        gBoard_DDRThermalMgmtInstance.devIdCore = (CSL_ARM_R5_CPU_ID_0 == info.cpuID)?
                                                        TISCI_DEV_R5FSS0_CORE0:
                                                        TISCI_DEV_R5FSS0_CORE1;
    }
    else if (CSL_ARM_R5_CLUSTER_GROUP_ID_2 == info.grpId)
    {
        /* MAIN SS Pulsar R5 SS1 */
        gBoard_DDRThermalMgmtInstance.devIdIr   = TISCI_DEV_R5FSS1_INTROUTER0;
        gBoard_DDRThermalMgmtInstance.devIdCore = (CSL_ARM_R5_CPU_ID_0 == info.cpuID)?
                                                        TISCI_DEV_R5FSS1_CORE0:
                                                        TISCI_DEV_R5FSS1_CORE1;
    }
    
    return;
}

static Board_STATUS __attribute__((section(".text:BOARD_DDR_thermalManagement"))) Board_DDRGetIntNum(uint16_t *coreInterruptIdx)
{
    Board_STATUS    status = BOARD_SOK;
    uint16_t        irIntrIdx;
    struct tisci_msg_rm_get_resource_range_resp res;
    struct tisci_msg_rm_get_resource_range_req  req;

    memset(&res, 0, sizeof(res));
    memset(&req, 0, sizeof(req));

    req.type           = gBoard_DDRThermalMgmtInstance.devIdIr;
    req.subtype        = (uint8_t)TISCI_RESASG_SUBTYPE_IR_OUTPUT;
    req.secondary_host = (uint8_t)TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

    res.range_num = 0;
    res.range_start = 0;

    /* Get interrupt number range */
    status =  Sciclient_rmGetResourceRange(
                &req,
                &res,
                SCICLIENT_SERVICE_WAIT_FOREVER);
    if ((CSL_PASS != status) || (0U == res.range_num)) {
        /* Try with HOST_ID_ALL */
        req.type           = gBoard_DDRThermalMgmtInstance.devIdIr;
        req.subtype        = (uint8_t)TISCI_RESASG_SUBTYPE_IR_OUTPUT;
        req.secondary_host = TISCI_HOST_ID_ALL;

        status = Sciclient_rmGetResourceRange(
                &req,
                &res,
                SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if ((CSL_PASS == status) && (0U != res.range_num))
    {
        /* Translate IR Idx to Core Interrupt Idx */
        irIntrIdx = res.range_start;
        status = Sciclient_rmIrqTranslateIrOutput(gBoard_DDRThermalMgmtInstance.devIdIr,
                                                  irIntrIdx,
                                                  gBoard_DDRThermalMgmtInstance.devIdCore,
                                                  coreInterruptIdx);
    }
    else
    {
        status = BOARD_FAIL;
    }

    return status;
}

void __attribute__((section(".text:BOARD_DDR_thermalManagement"))) Board_updateAllRefreshRate(uint32_t refreshMultFactor)
{

    Board_updateRefreshRate(LPDDR4_FSP_0, refreshMultFactor);
    Board_updateRefreshRate(LPDDR4_FSP_1, refreshMultFactor);
    Board_updateRefreshRate(LPDDR4_FSP_2, refreshMultFactor);
}

/**
 * \brief Interrupt handler for DDR events
 *
 * Handles DDR events including temperature change events.
 *
 * \return  BOARD_SOK in case of success or appropriate error code
 *
 */
void __attribute__((section(".text:BOARD_DDR_thermalManagement"))) Board_DDRInterruptHandler(uintptr_t arg)
{
    bool     irqStatus;
    uint32_t regValue;
    uint32_t status;
    uint32_t tempCheckRefreshRateIndex;
    
    /* Get DDR interrupt status to check on thermal events */
    LPDDR4_CheckCtlInterrupt (&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), LPDDR4_TEMP_CHANGE, &irqStatus);
    if (irqStatus)
    {
        /* Check Temp check register to check on the temperature change:
         * Decide on refresh rate
         */
        /* Read Temp check register */
        status = LPDDR4_ReadReg(&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), LPDDR4_CTL_REGS,
                                LPDDR4__AUTO_TEMPCHK_VAL_0__REG_OFFSET,
                                &regValue);
        if (CDN_EOK == status)
        {
            /* Calculate refresh rate index */
            tempCheckRefreshRateIndex = ((regValue & LPDDR4__AUTO_TEMPCHK_VAL_0_MASK)
                                         >>  LPDDR4__AUTO_TEMPCHK_VAL_0_SHIFT )
                                        & LPDDR4__AUTO_TEMPCHK_OP0_MASK;

            /* Adjust refresh rate */
            Board_updateAllRefreshRate(gRefreshRateMultFactor[tempCheckRefreshRateIndex]);
        
            /* Call application callback function */
            if (NULL != gBoard_DDRThermalMgmtInstance.appCallBackFunction)
            {
                gBoard_DDRThermalMgmtInstance.appCallBackFunction((Board_DDRTempEventType)
                                                                  (BOARD_DDR_TEMP_EVENT_LOW_TEMP_ALARM
                                                                   + tempCheckRefreshRateIndex));
            }
        }

        /* Ack interrupt */
        LPDDR4_AckCtlInterrupt (&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), LPDDR4_TEMP_CHANGE);
    }
    /* Get DDR interrupt status to check on alarm events */
    LPDDR4_CheckCtlInterrupt (&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), LPDDR4_TEMP_ALERT, &irqStatus);
    if (irqStatus)
    {
        /* High or Low temperature alarm : call application callback */
        if (NULL != gBoard_DDRThermalMgmtInstance.appCallBackFunction)
        {
            gBoard_DDRThermalMgmtInstance.appCallBackFunction(BOARD_DDR_TEMP_EVENT_TEMP_ALERT);
        }
        /* Ack interrupt */
        LPDDR4_AckCtlInterrupt (&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), LPDDR4_TEMP_ALERT);
    }

    /* NOTE: Code to handle other DDR events can be added here */

}

/**
 * \brief DDR Temperature monitoring initialization function
 *
 * Configures and initializes DDR temperature handling.
 * NOTE: This assumes that the DDR parameters are already have the thermal events
 *       enabled in the DDR controller. This only handles software handling of the
 *       thermal events
 *
 * \return  BOARD_SOK in case of success or appropriate error code 
 *
 */
Board_STATUS Board_DDRTempMonitoringInit(Board_thermalMgmtCallbackFunction_t callbackFunction)
{
    Board_STATUS                status = BOARD_SOK;
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRet;
    uint64_t                    interruptMask;
    uint32_t                    lpddrStatus;
    uint32_t                    fspIndex;
    uint16_t                    coreInterruptIdx;
    
    struct tisci_msg_rm_irq_release_req irq_release_req =
    {
        .ia_id          = 0,
        .vint           = 0,
        .global_event   = 0,
        .vint_status_bit_index = 0,
    };
    struct tisci_msg_rm_irq_set_req irq_set_req =
    {
        .ia_id          = 0,
        .vint           = 0,
        .global_event   = 0,
        .vint_status_bit_index = 0,
    };
    struct tisci_msg_rm_irq_set_resp resp;

    /* Initialize LPDDR4 driver */
    gBoard_DDRThermalMgmtInstance.boardDDRCfg.ctlBase = (struct LPDDR4_CtlRegs_s *)BOARD_DDR_CTL_CFG_BASE;

    status = LPDDR4_Init(&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), &(gBoard_DDRThermalMgmtInstance.boardDDRCfg));

    if ((CDN_EOK < status) ||
        (gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd.ctlBase != (struct LPDDR4_CtlRegs_s *)gBoard_DDRThermalMgmtInstance.boardDDRCfg.ctlBase))
    {
        BOARD_DEBUG_LOG("LPDDR4_Init: FAIL\n");
        status = BOARD_FAIL;
    }

    if (BOARD_SOK == status)
    {
        /* Read and preserve the initial Refresh Rates as baseline */
        for (fspIndex = 0U; fspIndex <= LPDDR4_FSP_2; fspIndex++)
        {
            lpddrStatus = LPDDR4_GetRefreshRate(&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), &gBoardDDRFSPNum[fspIndex],
                                                &(gBoard_DDRThermalMgmtInstance.boardDDRInitRefreshRate[gBoardDDRFSPNum[fspIndex]]),
                                                &(gBoard_DDRThermalMgmtInstance.boardDDRTrasMax[gBoardDDRFSPNum[fspIndex]]));
            if (CDN_EOK < lpddrStatus)
            {
                BOARD_DEBUG_LOG("LPDDR4_GetRefreshRate: FAIL\n");
                status = BOARD_FAIL;
                break;
            }
        }
    }
    
    if (BOARD_SOK == status)
    {
        Board_DDRSetDevId();
        /* Get the Core IRQ Idx */
        status = Board_DDRGetIntNum(&coreInterruptIdx);
    }

    if (BOARD_SOK == status)
    {
        /* Configure interrupt router to route DDR Ctrl interrupt to Monitoring
         * CPU
         */
        /* Request irq release for specified interrupt source */
        irq_release_req.valid_params = TISCI_MSG_VALUE_RM_DST_ID_VALID
                                       | TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
        irq_release_req.src_id = TISCI_DEV_DDR0;
        irq_release_req.src_index = 0; /* First interrupt in DDR group is the DDR controller interrupt */
        irq_release_req.dst_id = gBoard_DDRThermalMgmtInstance.devIdCore;
        irq_release_req.dst_host_irq = coreInterruptIdx;

        /* Call irq Release */
        if (CSL_PASS != Sciclient_rmIrqRelease(&irq_release_req, BOARD_SCICLIENT_RESP_TIMEOUT))
        {
            /* Ignore if the release fails, as this is expected the first time the image is booted */
        }

        /* Request irq set for specified interrupt source */
        irq_set_req.valid_params = TISCI_MSG_VALUE_RM_DST_ID_VALID
                                   | TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
        irq_set_req.src_id = TISCI_DEV_DDR0;
        irq_set_req.src_index = 0; /* First interrupt in DDR group is the DDR controller interrupt */
        irq_set_req.dst_id = gBoard_DDRThermalMgmtInstance.devIdCore;
        irq_set_req.dst_host_irq = coreInterruptIdx;

        /* Call irq Set */
        if (CSL_PASS != Sciclient_rmIrqSet(&irq_set_req, &resp, BOARD_SCICLIENT_RESP_TIMEOUT))
        {
            status = BOARD_FAIL;
        }
    }

    if (BOARD_SOK == status)
    {

        /* Register DDR Control event handler */
        Osal_RegisterInterrupt_initParams(&intrPrms); 
        
        intrPrms.corepacConfig.corepacEventNum  = 0;
        intrPrms.corepacConfig.intVecNum        = coreInterruptIdx;
        intrPrms.corepacConfig.arg              = (uintptr_t)(&(gBoard_DDRThermalMgmtInstance.boardTempInterruptHandle));
        intrPrms.corepacConfig.isrRoutine       = &Board_DDRInterruptHandler;

        /* Clear Interrupt */
        Osal_ClearInterrupt(intrPrms.corepacConfig.corepacEventNum, intrPrms.corepacConfig.intVecNum);

        /* Register interrupts */
        osalRet = Osal_RegisterInterrupt(&intrPrms, &(gBoard_DDRThermalMgmtInstance.boardTempInterruptHandle));
        if (OSAL_INT_SUCCESS != osalRet)
        {
            status = BOARD_FAIL;
        }
    }

    if (BOARD_SOK == status)
    {
        /* Unmask only DDR thermal interrupt events */
        interruptMask = (uint64_t)(0xFFFFFFFFFFFFFFFFU)
                        & (~(((((uint64_t)1U) << LPDDR4_TEMP_CHANGE)
                             | (((uint64_t)1U) << LPDDR4_TEMP_ALERT))));
        LPDDR4_SetCtlInterruptMask(&(gBoard_DDRThermalMgmtInstance.boardRuntimeDDRPd), (const uint64_t *)(&interruptMask));

        /* Enable DDR controller interrupt */
        Osal_EnableInterrupt(intrPrms.corepacConfig.corepacEventNum, intrPrms.corepacConfig.intVecNum);
    }

    if (BOARD_SOK == status)
    {
        gBoard_DDRThermalMgmtInstance.appCallBackFunction = NULL;
        if (NULL != callbackFunction)
        {
            /* Register callback function */
            gBoard_DDRThermalMgmtInstance.appCallBackFunction = callbackFunction;
        }
    }
    return status;
}

#endif /* BUILD_MCU*/
