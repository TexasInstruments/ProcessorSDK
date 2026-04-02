/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \file mailbox_soc.c
 *
 *       The file implements the Platform specific Mailbox Driver Interface.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/drv/cbuff/cbuff.h>
#include <ti/drv/cbuff/src/cbuff_internal.h>
#include <ti/osal/MemoryP.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None. */

/* ========================================================================== */
/*                          Global Variables                                  */
/* ========================================================================== */

CBUFF_DriverMCB    g_cbuffDriver = {0};
CBUFF_Session      g_cbuffSession[CBUFF_MAX_NUM_SESSION] = {0u};

/**
 * @brief   This is the AWR294x Hardware attributes which are used by the
 * CBUFF driver
 */
CBUFF_Hw_Attrib gCBUFFHwAttribute =
{
    CSL_DSS_CBUFF_U_BASE,
    CSL_DSS_CBUFF_FIFO_U_BASE,
    {
        EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0,
		EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ1,
		EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ2,
		EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ3,
		EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ4,
		EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ5,
		EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ6
    },
    CSL_RSS_ADCBUF_READ_U_BASE,
    /***************************************************************
     * Chirp Parameters:
     * - Single Chirp Mode
     * - In Interleaved mode the Chirp Parameters are stored from
     *   CPREG4 to CPREG7
     * - Each Chirp Parameter is 4 bytes or 2 CBUFF Units
     ***************************************************************/
    {
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG4) + 0*4U),
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG4) + 1*4U),
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG4) + 2*4U),
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG4) + 3*4U)
    },
    /***************************************************************
     * Chirp Parameters:
     * - Single Chirp Mode
     * - In Non-Interleaved mode the Chirp Parameters are stored from
     *   CPREG0 to CPREG3
     * - Each Chirp Parameter is 4 bytes or 2 CBUFF Units
     ***************************************************************/
    {
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0) + 0*4U),
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0) + 1*4U),
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0) + 2*4U),
        ((CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0) + 3*4U)
    },
    /***************************************************************
     * Chirp Parameters:
     * - Multiple Chirp Mode
     * - This is the base address of each Chirp Profile. There
     *   can be upto 8 chirps in Multi-Chirp Mode.
     ***************************************************************/
    {
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH0CPREG0),
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH1CPREG0),
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH2CPREG0),
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH3CPREG0),
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH4CPREG0),
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH5CPREG0),
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH6CPREG0),
        (CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH7CPREG0)
    },
    4U,     /* Maximum of 4 LVDS Lanes supported on the AWR2944  */
#if defined (__TI_ARM_V7R4__)
	CSL_MSS_INTR_DSS_CBUFF_INT_ERR,
	CSL_MSS_INTR_DSS_CBUFF_INT,
#endif

#if defined (_TMS320C6X)
	CSL_DSS_INTR_DSS_CBUFF_INT_ERR,
	CSL_DSS_INTR_DSS_CBUFF_INT,
#endif
    /***************************************************************
     * Chirp Mode:
     *  AWR294X supports single & multiple chirp mode. The range is
     *  thus defined from 1 to 8
     ***************************************************************/
    1U,
    8U,
    (uint32_t)&g_cbuffDriver,
    (uint32_t)&g_cbuffSession[0]
};


/**
 * @brief   This is the CBUFF supported high speed interfaces for AWR294X
 */
CBUFF_InterfaceFxn gCBUFFInterfaceFxn[] =
{
    {
        CBUFF_Interface_LVDS,               /* LVDS Interface                       */
        &CBUFF_initLVDS,                    /* Initialize the LVDS Interface        */
        &CBUFF_deinitLVDS,                  /* Deinitialize the LVDS Interface      */
        &CBUFF_openLVDS,                    /* Open the LVDS Interface              */
        &CBUFF_closeLVDS,                   /* Close the LVDS Interface             */
        &CBUFF_initLVDSLinkListParams,      /* LVDS Init Linked List Parameters     */
        &CBUFF_finalizeLVDSLinkListParams   /* LVDS Finalize Linked List Parameters */
    },
    {
        CBUFF_Interface_MDO,                /* MDO/Aurora Interface                 */
        NULL,                               /* NULL                                 */
        NULL,                               /* NULL                                 */
        NULL,                               /* NULL                                 */
		NULL                                /* NULL                                 */
    }
};

/**
 * @brief   On the AWR2944 Interleaved mode is not supported
 */
CBUFF_setupTransferFxn gInterleavedTransferFxn[] =
{
    CBUFF_setupInterleaved_ADC,                        /* Data Format: CBUFF_DataFmt_ADC_DATA          */
    CBUFF_setupInterleaved_CP_ADC,                     /* Data Format: CBUFF_DataFmt_CP_ADC            */
    CBUFF_setupInterleaved_ADC_CP,                     /* Data Format: CBUFF_DataFmt_ADC_CP            */
    CBUFF_setupInterleaved_CP_ADC_CQ,                  /* Data Format: CBUFF_DataFmt_CP_ADC_CQ         */
    CBUFF_setupInterleaved_ADC_USER,                   /* Data Format: CBUFF_DataFmt_ADC_USER          */
    CBUFF_setupInterleaved_CP_ADC_CQ_USER              /* Data Format: CBUFF_DataFmt_CP_ADC_CQ_USER    */
};

bool CBUFF_isInterleavedDataFormatSupported(CBUFF_DataFmt dataFmt)
{
    return(true);
}

/**
 * @brief   This is the function table for non-interleaved transfer functions.
 * The default driver library supports some data formats. The full
 * set is provided as reference under the \#ifdef ENABLE_ALL..
 * Modify this table to enable the formats of choice and rebuild the driver.
 */
CBUFF_setupTransferFxn gNonInterleavedTransferFxn[] =
{
    CBUFF_setupNonInterleaved_ADC,                      /* Data Format: CBUFF_DataFmt_ADC_DATA          */
    CBUFF_setupNonInterleaved_CP_ADC,                   /* Data Format: CBUFF_DataFmt_CP_ADC            */
    CBUFF_setupNonInterleaved_ADC_CP,                   /* Data Format: CBUFF_DataFmt_ADC_CP            */
    CBUFF_setupNonInterleaved_CP_ADC_CQ,                /* Data Format: CBUFF_DataFmt_CP_ADC_CQ         */
    CBUFF_setupNonInterleaved_ADC_USER,                 /* Data Format: CBUFF_DataFmt_ADC_USER          */
    CBUFF_setupNonInterleaved_CP_ADC_CQ_USER            /* Data Format: CBUFF_DataFmt_CP_ADC_CQ_USER    */
};

bool CBUFF_isNonInterleavedDataFormatSupported(CBUFF_DataFmt dataFmt)
{
    return((dataFmt == CBUFF_DataFmt_ADC_DATA) || (dataFmt == CBUFF_DataFmt_CP_ADC_CQ));
}

/**
 * @brief   This is the function table for multi-chirp non-interleaved transfer functions.
 * The default driver library does not support any data format. The full
 * set is provided as reference under the \#ifdef ENABLE_ALL..
 * Modify this table to enable the formats of choice and rebuild the driver.
 * *NOTE*: There are some transfer functions which are the same between single and multiple
 * chirp.
 */
CBUFF_setupTransferFxn gMultipleChirpNonInterleavedTransferFxn[] =
{
#ifdef ENABLE_ALL_MULTI_CHIRP_NON_INTERLEAVED
    CBUFF_setupNonInterleaved_ADC,                      /* Data Format: CBUFF_DataFmt_ADC_DATA          */
    CBUFF_setupNonInterleaved_MultiChirp_CP_ADC,        /* Data Format: CBUFF_DataFmt_CP_ADC            */
    CBUFF_setupNonInterleaved_MultiChirp_ADC_CP,        /* Data Format: CBUFF_DataFmt_ADC_CP            */
    CBUFF_setupNonInterleaved_MultiChirp_CP_ADC_CQ,     /* Data Format: CBUFF_DataFmt_CP_ADC_CQ         */
    CBUFF_setupNonInterleaved_ADC_USER,                 /* Data Format: CBUFF_DataFmt_ADC_USER          */
    CBUFF_setupNonInterleaved_MultiChirp_CP_ADC_CQ_USER /* Data Format: CBUFF_DataFmt_CP_ADC_CQ_USER    */
#else
    NULL,                                               /* Data Format: CBUFF_DataFmt_ADC_DATA          */
    NULL,                                               /* Data Format: CBUFF_DataFmt_CP_ADC            */
    NULL,                                               /* Data Format: CBUFF_DataFmt_ADC_CP            */
    NULL,                                               /* Data Format: CBUFF_DataFmt_CP_ADC_CQ         */
    NULL,                                               /* Data Format: CBUFF_DataFmt_ADC_USER          */
    NULL                                                /* Data Format: CBUFF_DataFmt_CP_ADC_CQ_USER    */
#endif
};

bool CBUFF_isMultipleChirpNonInterleavedDataFormatSupported(CBUFF_DataFmt dataFmt)
{
    return(false);
}
