/**
 * @file   UART_soc.c
 *
 * @brief  This file defines the UART interface structure specific to TPR12
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

#include <ti/csl/soc.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/soc/UART_soc.h>
#include <ti/drv/uart/src/v3/uartsci.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/

/**
 * @brief   This is the UART Driver registered function table
 */
extern UART_FxnTable gUartSciFxnTable;

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**************************************************************************
 * On the XWR18xx: The UART is available on both the DSS and MSS. The UART
 * platform configuration is MSS and DSS specific
 **************************************************************************/

#define UART_MODULE_FREQ_200M    (200000000U)


#if defined (__TI_ARM_V7R4__)
#define CSL_UART_PER_CNT    (CSL_MSS_UART_PER_CNT + CSL_DSS_UART_PER_CNT)

/**
 * @brief This is the TPR12 MSS accessible UART configuration. There are
 * 2 UART instances (SCI-A and SCI-B) available on the MSS and 1 UART
 * instance (SCI-A) available on the DSS.
 */
UartSci_HwCfg uartInitCfg[CSL_UART_PER_CNT] =
{
    /* UART1 Hardware configuration:
     * - Capable of sending and receiving data
     */
    {
        (CSL_sciRegs *)CSL_MSS_SCIA_U_BASE,
        UartSci_Duplexity_FULL,
        CSL_MSS_INTR_MSS_SCIA_INT0,
        EDMA_MSS_TPCC_A_EVT_SCIA_TX_DMA_REQ,
        EDMA_MSS_TPCC_A_EVT_SCIA_RX_DMA_REQ,
#ifdef UART_DMA_ENABLE
        &UartSci_openEDMA,
        &UartSci_closeEDMA,
        &UartSci_isEDMAEnabled,
        &UartSci_initiateRxEDMA,
        &UartSci_initiateTxEDMA,
#else
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#endif
        {
            UART_MODULE_FREQ_200M,
            NULL,
            0,
            TRUE,  /* default DMA mode */
            FALSE, /* Loopback disabled by default */
		    TRUE   /* Interrupt enabled by default */
        }
    },

    /* UART2 Hardware configuration:
     * - Capable of only sending data
     */
    {
        (CSL_sciRegs *)CSL_MSS_SCIB_U_BASE,
        UartSci_Duplexity_TX_ONLY,
        CSL_MSS_INTR_MSS_SCIB_INT0,
        EDMA_MSS_TPCC_A_EVT_SCIB_TX_DMA_REQ,
        EDMA_MSS_TPCC_A_EVT_SCIB_RX_DMA_REQ,
#ifdef UART_DMA_ENABLE
        &UartSci_openEDMA,
        &UartSci_closeEDMA,
        &UartSci_isEDMAEnabled,
        &UartSci_initiateRxEDMA,
        &UartSci_initiateTxEDMA,
#else
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#endif
        {
            UART_MODULE_FREQ_200M,
            NULL,
            0,
            TRUE,  /* default DMA mode */
            FALSE, /* Loopback disabled by default */
		    TRUE   /* Interrupt enabled by default */
        }
    },

    /* DSS UART Hardware configuration:
     * - Capable of sending and receiving data
     * Note: Need to use EDMA_DRV_INST_DSS_B for EDMA_DSS_TPCC_B
     */
    {
        (CSL_sciRegs *)CSL_DSS_SCIA_U_BASE,
        UartSci_Duplexity_FULL,
        CSL_MSS_INTR_DSS_SCIA_INT0,
        EDMA_DSS_TPCC_B_EVT_SCIA_TX_DMA_REQ,
        EDMA_DSS_TPCC_B_EVT_SCIA_RX_DMA_REQ,
#ifdef UART_DMA_ENABLE
        &UartSci_openEDMA,
        &UartSci_closeEDMA,
        &UartSci_isEDMAEnabled,
        &UartSci_initiateRxEDMA,
        &UartSci_initiateTxEDMA,
#else
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#endif
        {
            UART_MODULE_FREQ_200M,
            NULL,
            0,
            TRUE,  /* default DMA mode */
            FALSE, /* Loopback disabled by default */
		    TRUE   /* Interrupt enabled by default */
        }
    }
};

/* UART objects */
UartSci_Driver UartObjects[CSL_UART_PER_CNT];

/**
 * @brief   The MSS on the TPR12 Platform has 3 UART Modules which can be used.
 */
UART_Config UART_config[] =
{
    {
        &gUartSciFxnTable,            /* UART SCI Driver Function Table:        */
        (void *)&UartObjects[0],      /* UART Driver Object:                    */
        (void *)&uartInitCfg[0]       /* UART Hw configuration:                 */
    },
    {
        &gUartSciFxnTable,            /* UART SCI Driver Function Table:        */
        (void *)&UartObjects[1],      /* UART Driver Object:                    */
        (void *)&uartInitCfg[1]       /* UART Hw configuration:                 */
    },
    {
        &gUartSciFxnTable,            /* UART SCI Driver Function Table:        */
        (void *)&UartObjects[2],      /* UART Driver Object:                    */
        (void *)&uartInitCfg[2]       /* UART Hw configuration:                 */
    },
    {
        NULL,                         /* UART SCI Driver Function Table:        */
        NULL,                         /* UART Driver Object:                    */
        NULL                          /* UART Hw configuration:                 */
    }
};

#elif defined (_TMS320C6X)

/**
 * @brief   This is the TPR12 DSS specific UART configuration. There is only 1 available
 * UART instance. This should *NOT* be modified by the customer.
 */
#define CSL_UART_PER_CNT    CSL_DSS_UART_PER_CNT

UartSci_HwCfg uartInitCfg[CSL_UART_PER_CNT] =
{
    /* DSS UART Hardware configuration:
     * - Capable of sending and receiving data
     */
    {
        (CSL_sciRegs *)CSL_DSS_SCIA_U_BASE,
        UartSci_Duplexity_FULL,
        CSL_DSS_INTR_DSS_SCIA_INT0,
        EDMA_DSS_TPCC_B_EVT_SCIA_TX_DMA_REQ,
        EDMA_DSS_TPCC_B_EVT_SCIA_RX_DMA_REQ,
#ifdef UART_DMA_ENABLE
        &UartSci_openEDMA,
        &UartSci_closeEDMA,
        &UartSci_isEDMAEnabled,
        &UartSci_initiateRxEDMA,
        &UartSci_initiateTxEDMA,
#else
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
#endif
        {
            UART_MODULE_FREQ_200M,
            NULL,
            0,
            TRUE,  /* default DMA mode */
            FALSE, /* Loopback disabled by default */
		    TRUE   /* Interrupt enabled by default */
        }
    }
};

/* UART objects */
UartSci_Driver UartObjects[CSL_UART_PER_CNT];

/**
 * @brief   The DSS on the TPR12 Platform has 1 UART Modules which can be used.
 */
UART_Config UART_config[] =
{
    {
        &gUartSciFxnTable,            /* UART SCI Driver Function Table:        */
        (void *)&UartObjects[0],      /* UART Driver Object:                    */
        (void *)&uartInitCfg[0]       /* UART Hw configuration:                 */
    },
    {
        NULL,                         /* UART SCI Driver Function Table:        */
        NULL,                         /* UART Driver Object:                    */
        NULL                          /* UART Hw configuration:                 */
    }
};

#else
#error "Error: Please check the compiler flags since BUILD_XXX is not defined for the TPR12 device"
#endif

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
        *cfg = uartInitCfg[index].swCfg;
    }
    else
    {
        ret = (-((int32_t)1));
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
        uartInitCfg[index].swCfg = *cfg;
    }
    else
    {
        ret = (-((int32_t)1));
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

void UART_socInitHwCfg(UART_Handle handle)
{
    UART_Config*             ptrUARTConfig;
    UartSci_HwCfg*           ptrHwCfg;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;
    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Update the input frequency based on efuse. */
    ptrHwCfg->swCfg.clockFrequency = CSL_SocGetSysclkFreq();

    return;
}
