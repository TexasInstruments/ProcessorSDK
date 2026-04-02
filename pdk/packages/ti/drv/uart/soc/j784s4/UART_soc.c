/**
 * @file   UART_soc.c
 *
 * @brief  This file defines the UART interface structure specific to J784S4
 */
/*
 * Copyright (c) 2022, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================*/

#include <string.h>
#include <ti/csl/csl_utils.h>
#include <ti/csl/arch/r5/csl_arm_r5.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <ti/drv/sciclient/sciclient.h>
#if defined(SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_cfg.h>
#elif defined(SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_cfg.h>
#endif

#define MCU_UART_INSTANCE               (BOARD_UART_INSTANCE)

#if defined (BUILD_MCU)
#define UART_PDMA_TX_THREAD_BASE        (CSL_PDMA_CH_MCU_UART0_CH0_TX)
#define UART_PDMA_RX_THREAD_BASE        (CSL_PDMA_CH_MCU_UART0_CH0_RX)
#else
#define UART_PDMA_TX_THREAD_BASE        (CSL_PDMA_CH_MAIN_UART0_CH0_TX)
#define UART_PDMA_RX_THREAD_BASE        (CSL_PDMA_CH_MAIN_UART0_CH0_RX)
#endif

#define UART_INPUT_CLK_48M              (48000000U)
#define UART_INPUT_CLK_96M              (96000000U)

/* DMSC SYSFW invalid UART device ID */
#define UART_TISCI_INVALID_DEV_ID       (0xFFFFU)

/* CLEC input event # offset for GIC SPI */
#define UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET (1024U - 32U)

/* C7x INTC int # for UART0 */
#define UART_C7X_IRQ0                   (20U)

static int32_t UART_socConfigIntrPath(const void *pHwAttrs, bool setIntrPath);

/* UART configuration structure */
UART_HwAttrs uartInitCfg[CSL_UART_MAIN_CNT] =
{
    {
#if defined (BUILD_MPU)
        /* default configuration for UART instance and A72 core on Main domain*/
        (uint32_t)CSL_UART0_BASE,       /* baseAddr */
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART0_USART_IRQ_0,   /* intNum */
        0,                              /* eventId, used only for C6x */
        UART_INPUT_CLK_48M,             /* frequency */
#endif
#if defined (BUILD_MCU)
        /*
         * default configuration for UART instances and R5 cores on MCU domain
         * UART_socInit() is called during initialization to change the default
         * configurations to use Main Domain's UART instances if R5 is running
         * on Main domain
         */
        (uint32_t)CSL_MCU_UART0_BASE,   /* baseAddr */
        CSLR_MCU_R5FSS0_CORE0_INTR_MCU_UART0_USART_IRQ_0,   /* intNum */
        0,                              /* eventId, used only for C6x */
        UART_INPUT_CLK_96M,             /* frequency */
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART0_BASE,
        UART_C7X_IRQ0,                  /* intNum */
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART0_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET, /* eventId, input event # to CLEC */
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE,       /* rxDmaEventNumber, used as UART PDMA RX
                                           thread # for the UART instance */
        UART_PDMA_TX_THREAD_BASE,       /* txDmaEventNumber, used as UART PDMA TX
                                           thread # for the UART instance */
        0,                              /* edmaTxTCC */
        0,                              /* edmaRxTCC */
        0,                              /* edmaTxTC */
        0,                              /* edmaRxTC */
        0,                              /* version */
        NULL,                           /* edmaHandle */
        UART_RXTRIGLVL_8,               /* rxTrigLvl */
        UART_TXTRIGLVL_56,              /* txTrigLvl */
        UFALSE,                         /* dmaMode */
        UFALSE,                         /* loopback */
        1U,                             /* enableInterrupt */
        UART16x_OPER_MODE,              /* operMode */
        NULL,                           /* dmaInfo */
        UART_socConfigIntrPath,         /* configSocIntrPath */
        UFALSE,                         /* dirEnable, RS-485 External Transceiver Direction */
        UART_MDR3_DIR_POL_0,            /* dirPol, RS-485 External Transceiver Direction Polarity */
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART1_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART1_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART1_BASE,
        UART_C7X_IRQ0 + 1U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART1_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 1U,
        UART_PDMA_TX_THREAD_BASE + 1U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART2_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART2_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART2_BASE,
        UART_C7X_IRQ0 + 2U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART2_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 2U,
        UART_PDMA_TX_THREAD_BASE + 2U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART3_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART3_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART3_BASE,
        UART_C7X_IRQ0 + 3U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART3_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 3U,
        UART_PDMA_TX_THREAD_BASE + 3U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART4_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART4_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART4_BASE,
        UART_C7X_IRQ0 + 4U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART4_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 4U,
        UART_PDMA_TX_THREAD_BASE + 4U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART5_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART5_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART5_BASE,
        UART_C7X_IRQ0 + 5U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART5_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 5U,
        UART_PDMA_TX_THREAD_BASE + 5U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART6_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART6_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART6_BASE,
        UART_C7X_IRQ0 + 6U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART6_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 6U,
        UART_PDMA_TX_THREAD_BASE + 6U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART7_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART7_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART7_BASE,
        UART_C7X_IRQ0 + 7U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART7_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 7U,
        UART_PDMA_TX_THREAD_BASE + 7U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART8_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART8_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        (uint32_t)CSL_MCU_UART0_BASE,   /* baseAddr */
        CSLR_MCU_R5FSS0_CORE0_INTR_MCU_UART0_USART_IRQ_0,   /* intNum */
        0,                              /* eventId, used only for C6x */
        UART_INPUT_CLK_96M,             /* frequency */
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART8_BASE,
        UART_C7X_IRQ0 + 8U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART8_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 8U,
        UART_PDMA_TX_THREAD_BASE + 8U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
#if defined (BUILD_MPU)
        (uint32_t)CSL_UART9_BASE,
        CSLR_COMPUTE_CLUSTER0_GIC500SS_SPI_UART9_USART_IRQ_0,
        0,
        UART_INPUT_CLK_48M,
#endif
#if defined (BUILD_MCU)
        0,
        0,
        0,
        0,
#endif
#if defined (BUILD_C7X)
        (uint32_t)CSL_UART9_BASE,
        UART_C7X_IRQ0 + 9U,
        CSLR_COMPUTE_CLUSTER0_CLEC_SOC_EVENTS_IN_UART9_USART_IRQ_0 + UART_CLEC_SOC_EVENTS_IN_EVT_OFFSET,
        UART_INPUT_CLK_48M,
#endif
        UART_PDMA_RX_THREAD_BASE + 9U,
        UART_PDMA_TX_THREAD_BASE + 9U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        UFALSE,
        UFALSE,
        1U,
        UART16x_OPER_MODE,
        NULL,
        UART_socConfigIntrPath,
        UFALSE,
        UART_MDR3_DIR_POL_0,
    },
};

/* UART objects */
UART_V1_Object UartObjects[CSL_UART_MAIN_CNT];

/* UART configuration structure */
UART_Config UART_config[CSL_UART_MAIN_CNT + 1U] = {
    {
        &UART_FxnTable_v1,
        &UartObjects[0],
        &uartInitCfg[0]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[1],
        &uartInitCfg[1]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[2],
        &uartInitCfg[2]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[3],
        &uartInitCfg[3]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[4],
        &uartInitCfg[4]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[5],
        &uartInitCfg[5]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[6],
        &uartInitCfg[6]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[7],
        &uartInitCfg[7]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[8],
        &uartInitCfg[8]
    },

    {
        &UART_FxnTable_v1,
        &UartObjects[9],
        &uartInitCfg[9]
    },

    {NULL, NULL, NULL}
};

/**
 * \brief  This API gets the SoC level of UART initial configuration
 *
 * \param  idx       UART instance index.
 * \param  cfg       Pointer to UART SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t UART_socGetInitCfg(uint32_t idx, UART_HwAttrs *cfg)
{
    int32_t ret = UART_SUCCESS;

    if (idx < CSL_UART_MAIN_CNT)
    {
        *cfg = uartInitCfg[idx];
    }
    else
    {
        ret = UART_ERROR;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of UART initial configuration
 *
 * \param  idx       UART instance index.
 * \param  cfg       Pointer to UART SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t UART_socSetInitCfg(uint32_t idx, const UART_HwAttrs *cfg)
{
    int32_t ret = UART_SUCCESS;

    if (idx < CSL_UART_MAIN_CNT)
    {
        uartInitCfg[idx] = *cfg;
    }
    else
    {
        ret = UART_ERROR;
    }

    return ret;
}

#if defined (BUILD_MCU)
/**
 * \brief  This API update the default SoC level of configurations
 *         based on the core and domain
 *
 *         uartInitCfg table configures MCU domain's UART instances by
 *         default for R5, UART_socInit() is called UART_init() to
 *         overwrite the defaut configurations with the configurations
 *         of Main domain's UART instances if R5 is on the Main domain
 *
 * \param  none
 *
 * \return none
 *
 */
void UART_socInit(void);
void UART_socInit(void)
{
    uint32_t         i;
    CSL_ArmR5CPUInfo info = {0U, 0U, 0U};

    CSL_armR5GetCpuID(&info);
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 != info.grpId)
    {
        /* Pulsar R5 core is on the Main domain */
        for (i = 0U; i < CSL_UART_MAIN_CNT; i++)
        {
            /* Configure the Main SS UART instances for Main SS Pulsar R5 */
            uartInitCfg[i].baseAddr = (uint32_t)CSL_UART0_BASE + (0x10000U * i);

            if (i < 3U)
            {
                /* Main domain's UART0-2 are directly connected to the MAIN Pulsar VIMs */

                uartInitCfg[i].intNum = CSLR_R5FSS0_CORE0_INTR_UART0_USART_IRQ_0 + i;
            }
            else
            {
                /*
                 * Main domain's UART3-9 are directly connected to the MAIN Pulsar VIMs 
                 * starting from different offset.
                 */
                uartInitCfg[i].intNum = CSLR_R5FSS0_CORE0_INTR_UART3_USART_IRQ_0 + i - 3U;
            }
            uartInitCfg[i].frequency        = UART_INPUT_CLK_48M;
            uartInitCfg[i].rxDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_RX + i;
            uartInitCfg[i].txDmaEventNumber = CSL_PDMA_CH_MAIN_UART0_CH0_TX + i;
        }
    }
    else
    {
        uartInitCfg[MCU_UART_INSTANCE].rxDmaEventNumber = UART_PDMA_RX_THREAD_BASE;
        uartInitCfg[MCU_UART_INSTANCE].txDmaEventNumber = UART_PDMA_TX_THREAD_BASE;
    }
}
#endif

static int32_t UART_socConfigIntrPath(const void *pHwAttrs, bool setIntrPath)
{
   int32_t ret = UART_SUCCESS;

#if defined (BUILD_C7X)
    int32_t               retVal;
    UART_HwAttrs         *hwAttrs = (UART_HwAttrs *)(pHwAttrs);
    CSL_ClecEventConfig   cfgClec;
    CSL_CLEC_EVTRegs     *clecBaseAddr = (CSL_CLEC_EVTRegs *)CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;

    /* Configure CLEC for UART */
    cfgClec.secureClaimEnable = UFALSE;
    cfgClec.evtSendEnable     = UTRUE;
    cfgClec.rtMap             = CSL_CLEC_RTMAP_CPU_ALL;
    cfgClec.extEvtNum         = 0U;
    cfgClec.c7xEvtNum         = hwAttrs->intNum;
    retVal = CSL_clecConfigEvent(clecBaseAddr, hwAttrs->eventId, &cfgClec);
    if (CSL_PASS != retVal)
    {
        ret = UART_ERROR;
    }
    setIntrPath = setIntrPath;
#else
    (void)pHwAttrs;
#endif

    return(ret);
}
