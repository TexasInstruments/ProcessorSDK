/**
 * @file   UART_soc.c
 *
 * @brief  This file defines the UART interface structure specific to AM64X
 */
/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#include <ti/csl/csl_utils.h>
#include <ti/csl/soc/am64x/src/cslr_soc.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/soc/UART_soc.h>

#define CSL_UART_PER_CNT    (9U)  /* 7 Main UART + 2 MCU UART instances */
#if defined (BUILD_M4F)
#define UART_RAT_MAP_ADDR   (0x60000000U) /* RAT map address for UART */
#endif

/* UART configuration structure */
UART_HwAttrs uartInitCfg[CSL_UART_PER_CNT] =
{
    {
        /* UART0 on the Main domain */
#if defined (BUILD_M4F)
        CSL_UART0_BASE + UART_RAT_MAP_ADDR, /* baseAddr */
#else
        CSL_UART0_BASE,                     /* baseAddr */
#endif
#if defined (BUILD_MPU)
        /* A53 cores on the Main domain */
        CSLR_GICSS0_SPI_UART0_USART_IRQ_0,  /* intNum */
#elif defined (BUILD_MCU)
        /* R5 cores on the Main domain */
        CSLR_R5FSS0_CORE0_INTR_UART0_USART_IRQ_0,
#else
        /* M4 core on the MCU Channel */
        0U,  /* TBD, no UART0 interrupt routed to M4 core */
#endif
        0U,                                 /* eventId, not used for am64x */
        UART_MODULE_CLOCK,                  /* frequency, default 48MHz */
        CSL_PDMA_CH_MAIN0_UART0_CH0_RX,     /* rxDmaEventNumber, used as UART PDMA RX
                                               thread # for the UART instance */
        CSL_PDMA_CH_MAIN0_UART0_CH0_TX,     /* txDmaEventNumber, used as UART PDMA TX
                                               thread # for the UART instance */
        0,                                  /* edmaTxTCC, not used for am64x */
        0,                                  /* edmaRxTCC, not used for am64x */
        0,                                  /* edmaTxTC, not used for am64x */
        0,                                  /* edmaRxTC, not used for am64x */
        0,                                  /* version */
        NULL,                               /* edmaHandle */
        UART_RXTRIGLVL_8,                   /* rxTrigLvl */
        UART_TXTRIGLVL_56,                  /* txTrigLvl */
        FALSE,                              /* dmaMode */
        FALSE,                              /* loopback */
#if defined (BUILD_MPU) || defined (BUILD_MCU)
        TRUE,                               /* enableInterrupt, enable interrupt for A53F cores */
#else
        FALSE,                              /* enableInterrupt, disable interrupt for M4 core */
#endif
        UART16x_OPER_MODE,                  /* operMode, 16x over sampling mode by default */
        NULL,                               /* dmaInfo */
        NULL,                               /* configSocIntrPath */
        FALSE,                              /* dirEnable, RS-485 External Transceiver Direction */
        UART_MDR3_DIR_POL_0,                /* dirPol, RS-485 External Transceiver Direction Polarity */
    },
    {
        /* UART1 on the Main domain */
#if defined (BUILD_M4F)
        CSL_UART1_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_UART1_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_UART1_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_UART1_USART_IRQ_0,
#else
        0U,
#endif
        0U,
        UART_MODULE_CLOCK,
        CSL_PDMA_CH_MAIN0_UART1_CH0_RX,
        CSL_PDMA_CH_MAIN0_UART1_CH0_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
#if defined (BUILD_MPU) || defined (BUILD_MCU)
        TRUE,
#else
        FALSE,
#endif
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
        /* UART2 on the Main domain */
#if defined (BUILD_M4F)
        CSL_UART2_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_UART2_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_UART2_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_UART2_USART_IRQ_0,
#else
        0U,
#endif
        0U,
        UART_MODULE_CLOCK,
        CSL_PDMA_CH_MAIN1_UART2_CH0_RX,
        CSL_PDMA_CH_MAIN1_UART2_CH0_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
#if defined (BUILD_MPU) || defined (BUILD_MCU)
        TRUE,
#else
        FALSE,
#endif
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
        /* UART3 on the Main domain */
#if defined (BUILD_M4F)
        CSL_UART3_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_UART3_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_UART3_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_UART3_USART_IRQ_0,
#else
        0U,
#endif
        0U,
        UART_MODULE_CLOCK,
        CSL_PDMA_CH_MAIN1_UART3_CH0_RX,
        CSL_PDMA_CH_MAIN1_UART3_CH0_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
#if defined (BUILD_MPU) || defined (BUILD_MCU)
        TRUE,
#else
        FALSE,
#endif
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
        /* UART4 on the Main domain */
#if defined (BUILD_M4F)
        CSL_UART4_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_UART4_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_UART4_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_UART4_USART_IRQ_0,
#else
        0U,
#endif
        0U,
        UART_MODULE_CLOCK,
        CSL_PDMA_CH_MAIN1_UART4_CH0_RX,
        CSL_PDMA_CH_MAIN1_UART4_CH0_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
#if defined (BUILD_MPU) || defined (BUILD_MCU)
        TRUE,
#else
        FALSE,
#endif
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
        /* UART5 on the Main domain */
#if defined (BUILD_M4F)
        CSL_UART5_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_UART5_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_UART5_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_UART5_USART_IRQ_0,
#else
        0U,
#endif
        0U,
        UART_MODULE_CLOCK,
        CSL_PDMA_CH_MAIN1_UART5_CH0_RX,
        CSL_PDMA_CH_MAIN1_UART5_CH0_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
#if defined (BUILD_MPU) || defined (BUILD_MCU)
        TRUE,
#else
        FALSE,
#endif
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
        /* UART6 on the Main domain */
#if defined (BUILD_M4F)
        CSL_UART6_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_UART6_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_UART6_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_UART6_USART_IRQ_0,
#else
        0U,
#endif
        0U,
        UART_MODULE_CLOCK,
        CSL_PDMA_CH_MAIN1_UART6_CH0_RX,
        CSL_PDMA_CH_MAIN1_UART6_CH0_TX,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
#if defined (BUILD_MPU) || defined (BUILD_MCU)
        TRUE,
#else
        FALSE,
#endif
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
        /* UART0 on the MCU channel */
#if defined (BUILD_M4F)
        CSL_MCU_UART0_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_MCU_UART0_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCU_UART0_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCU_UART0_USART_IRQ_0,
#else
        CSLR_MCU_M4FSS0_CORE0_NVIC_MCU_UART0_USART_IRQ_0,
#endif
        0U,
        UART_MODULE_CLK_96M,
        0U,  /* TBD, no PDMA support for MCU_UART0 */
        0U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
        TRUE,
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
    {
        /* UART1 on the MCU channel */
#if defined (BUILD_M4F)
        CSL_MCU_UART1_BASE + UART_RAT_MAP_ADDR,
#else
        CSL_MCU_UART1_BASE,
#endif
#if defined (BUILD_MPU)
        CSLR_GICSS0_SPI_MCU_UART1_USART_IRQ_0,
#elif defined (BUILD_MCU)
        CSLR_R5FSS0_CORE0_INTR_MCU_UART1_USART_IRQ_0,
#else
        CSLR_MCU_M4FSS0_CORE0_NVIC_MCU_UART1_USART_IRQ_0,
#endif
        0U,
        UART_MODULE_CLK_96M,
        0U,  /* TBD, no PDMA support for MCU_UART1 */
        0U,
        0,
        0,
        0,
        0,
        0,
        NULL,
        UART_RXTRIGLVL_8,
        UART_TXTRIGLVL_56,
        FALSE,
        FALSE,
        TRUE,
        UART16x_OPER_MODE,
        NULL,
        NULL,
        FALSE,
        UART_MDR3_DIR_POL_0,
    },
};

/* UART objects */
UART_V1_Object UartObjects[CSL_UART_PER_CNT];

/* UART configuration structure */
UART_Config UART_config[CSL_UART_PER_CNT + 1U] = {
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

    {NULL, NULL, NULL}
};

/**
 * \brief  This API gets the SoC level of UART intial configuration
 *
 * \param  index     UART instance index.
 * \param  cfg       Pointer to UART SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t UART_socGetInitCfg(uint32_t index, UART_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_UART_PER_CNT)
    {
        *cfg = uartInitCfg[index];
    }
    else
    {
        ret = (int32_t)(-1);
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of UART intial configuration
 *
 * \param  index     UART instance index.
 * \param  cfg       Pointer to UART SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t UART_socSetInitCfg(uint32_t index, const UART_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (index < CSL_UART_PER_CNT)
    {
        uartInitCfg[index] = *cfg;
    }
    else
    {
        ret = (int32_t)(-1);
    }

    return ret;
}

#if defined (BUILD_MCU)
/**
 * \brief  This API initializes the SoC level of UART configuration
 *         based on the core and domain
 *
 * \param  none
 *
 * \return none
 *
 */
void UART_socInit(void)
{
}
#endif
