/******************************************************************************
* Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the
* distribution.
*
* Neither the name of Texas Instruments Incorporated nor the names of
* its contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

/**
 *
 * \file   csirx_test_tpr12.h
 *
 * \brief  This is the header file for CSI-Rx diagnostic test.
 *
 */

#ifndef _CSIRX_TEST_TPR12_H_
#define _CSIRX_TEST_TPR12_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_BIOS
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <stdio.h>
#include <ti/sysbios/knl/Task.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Error.h>
#else
#ifdef BUILD_C66X_1
/* !! HACK to workaround c66x OSAL baremetal issue with not enabling Hwi interrupt associated with EventCombiner */
/* Osal baremetal lib for c66x does not enable the Hwi interrupt number when Osal_enableInterrupt is called.
 * This is correct behaviour but the issue is when registering interrupt the Hwi interrupt number is not
 * enabled if Osal_registerInterrupt enableIntr param is set to FALSE> This iswrong and the Hwi interrupt
 * number should be enabled irrespective of a particular eventCombiner event is enabled or not.
 * Until this is fixed in c6xx osal baremetal library, adding hack in application to enable the interrupt
 */
#include <ti/osal/src/nonos/Nonos_config.h>
#define CSIRX_C66X_COMMON_ISR_HWI_INT_NUM                                                         (6U)
#endif
#endif /* #ifdef USE_BIOS */

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <ti/drv/csirx/soc/csirx_soc_priv.h>
#include <ti/drv/csirx/csirx.h>
#include <ti/csl/cslr_csirx.h>
#include <ti/osal/CacheP.h>

#include <assert.h>
#include <ti/drv/csirx/csirx.h>

#include <ti/csl/soc.h>
#include <ti/osal/osal.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/CycleprofilerP.h>
#include <ti/board/board.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>


/* ======================================================================== */
/*                           Macros & Typedefs                              */
/* ======================================================================== */

/* should match with CSI TX, assumes complex sample */
/* Number of ADC Samples per chirp */
#define BOARD_DIAG_CSIRX_TEST_NUM_ADC_SAMPLES_PER_CHIRP     (256U)
/* Number of bytes per ADC sample */
#define BOARD_DIAG_CSIRX_TEST_BYTES_PER_ADC_SAMPLE          (4U)
/* Number of Chirps */
#define BOARD_DIAG_CSIRX_TEST_NUM_CHIRPS                    (1U)
/* Number of frames to be received */
#define BOARD_DIAG_CSIRX_TEST_NUM_FRAMES                    (10U)
/* Number of RX channels */
#define BOARD_DIAG_CSIRX_TEST_NUM_RX                        (4U)

/* CSIRX fixed frame pattern */
#define BOARD_DIAG_CSIRX_TEST_PATTERN                       (0xFFU)
/* CSIRX context to be used in the test */
#define BOARD_DIAG_CSIRX_TEST_CONTEXT                       (2U)
/* CSIRX test frame format */
#define BOARD_DIAG_CSIRX_TEST_FORMAT                        (CSIRX_FORMAT_RAW8)
/* CSIRX virtual channel used in the test */
#define BOARD_DIAG_CSIRX_TEST_VC                            (0U)
/* CSIRX User defined maaping */
#define BOARD_DIAG_CSIRX_TEST_USER_DEFINED_MAPPING          \
        (CSIRX_USER_DEFINED_FORMAT_RAW8)
/* CSIRX Available instances */
#define BOARD_DIAG_CSIRX_TEST_NUM_INSTANCES                 (2U)
/* Number of bytes per frame */
#define BOARD_DIAG_CSIRX_TEST_NUM_BYTES_PER_FRAME           \
    ((BOARD_DIAG_CSIRX_TEST_NUM_ADC_SAMPLES_PER_CHIRP *     \
      BOARD_DIAG_CSIRX_TEST_BYTES_PER_ADC_SAMPLE *          \
      BOARD_DIAG_CSIRX_TEST_NUM_CHIRPS * BOARD_DIAG_CSIRX_TEST_NUM_RX))
/* Argument to be passed for common callback function */
#define BOARD_DIAG_CSIRX_TEST_COMMON_CB_ARG                 (0x11112222U)
/* Argument to be passed for combined end of frame callback function */
#define BOARD_DIAG_CSIRX_TEST_COMBINED_EOF_CB_ARG           (0x33334444U)
/* Allignment for ping-pong buffers */
#define BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT           \
    CSL_MAX(CSIRX_PING_PONG_ADDRESS_LINEOFFSET_ALIGNMENT_IN_BYTES,  \
            CSL_CACHE_L1D_LINESIZE)
/* Ping-Pong total buffer size */
#define BOARD_DIAG_CSIRX_TEST_PING_OR_PONG_BUF_SIZE_ALIGNED \
    CSL_NEXT_MULTIPLE_OF(BOARD_DIAG_CSIRX_TEST_NUM_BYTES_PER_FRAME, \
                         BOARD_DIAG_CSIRX_TEST_PING_PONG_ALIGNMENT)

/* Ping-Pong Buffer initialization pattern */
#define BOARD_DIAG_CSIRX_TEST_BUF_INIT_PATTERN              (0xBE)
/* Ping-Pong buffer total initialization pattern size */
#define BOARD_DIAG_CSIRX_TEST_INIT_PATTERN_SIZE             \
        (BOARD_DIAG_CSIRX_TEST_PING_OR_PONG_BUF_SIZE_ALIGNED + 32U)

/**
 * @brief Defines RAM types for choosing ping-pong buffers
 *
 */
typedef enum BoardDiag_RamType_e
{
    /** L3 Ram type */
    BOARD_DIAG_CSIRX_TEST_L3RAM,
    /** HWA Ram type */
    BOARD_DIAG_CSIRX_TEST_HWARAM,
    /** L2 Ram type */
    BOARD_DIAG_CSIRX_TEST_L2RAM
} BoardDiag_RamType;

/**
 * @brief This structure holds the different structure configurations
 *        to be initialized in the csirx diag test
 *
 */
typedef struct Board_DiagConfig_s
{
    /** Dphy configuration structure */
    CSIRX_DphyConfig_t DPHYcfg;
    /** complex IO configuration structure */
    CSIRX_ComplexIoConfig_t complexIOcfg;
    /** Common configuration structure */
    CSIRX_CommonConfig_t commonCfg;
    /** context configuration structure */
    CSIRX_ContextConfig_t contextCfg;
} BoardDiag_Config_t;

/**
 * @brief This structure holds the frame end code and line end code detect
 *        IRQ counts
 *
 */
typedef struct BoardDiag_ContextIRQcount_s
{
    /** Frame end code detect IRQ */
    volatile uint32_t frameEndCodeDetect;
    /** Line end code detec IRQ */
    uint32_t lineEndCodeDetect;
} BoardDiag_ContextIRQcount_t;

/**
 * @brief This structure holds common IRQ error counts
 *
 */
typedef struct BoardDiag_CommonIRQcount_s
{
    /** OCP error detect variable */
    uint32_t isOCPerror;
    /** Generic packet detect variable */
    uint32_t isGenericShortPacketReceive;
    /** ECC One bit short packet error detect variable */
    uint32_t isECConeBitShortPacketErrorCorrect;
    /** ECC more than one bit cannot correct detect variable */
    uint32_t isECCmoreThanOneBitCannotCorrect;
    /** Complex IO error detect */
    uint32_t isComplexIOerror;
    /** FIFO Overflow error detect */
    uint32_t isFIFOoverflow;
} BoardDiag_CommonIRQcount_t;

/**
 * @brief This structure holds common IRQs, Context IRQs and Lane IRQs
 *
 */
typedef struct BoardDiag_IRQs_s
{
    /** context IRQ structure */
    CSIRX_ContextIRQ_t context[CSIRX_NUM_CONTEXTS];
    /** common IRQ structure */
    CSIRX_CommonIRQ_t common;
    /** Lane IRQ structure */
    CSIRX_LanesIRQ_t  complexIOlanes;
} BoardDiag_IRQs_t;

/**
 * @brief This structure holds count of common callback functions
 *
 */
typedef struct BoardDiag_CallBackCounts_s
{
    /** common CB count */
    uint32_t common;
    /** combined end of CB line count */
    uint32_t combinedEOL;
    /** combined end of CB frame count */
    uint32_t combinedEOF;
    /** End of frame0 CB count */
    uint32_t EOF0;
    /** End of frame1 CB count */
    uint32_t EOF1;
    /** Start of frame0 CB count */
    uint32_t SOF0;
    /** Start of frame1 CB count */
    uint32_t SOF1;
    /** context end of line CB count */
    uint32_t contextEOL[CSIRX_NUM_CONTEXTS];
} BoardDaig_CallBackCounts_t;

/**
 * @brief This structure holds all states of frame reception
 *
 */
typedef struct BoardDaig_State_s
{
    /** Common IRQ count */
	BoardDiag_CommonIRQcount_t commonIRQcount;
    /** IRQ count */
    BoardDiag_IRQs_t IRQ;
    /** callback count */
    BoardDaig_CallBackCounts_t callbackCount;
    /** payload received correction detect */
    bool isReceivedPayloadCorrect;
    /** context IRQ count */
	BoardDiag_ContextIRQcount_t contextIRQcounts[CSIRX_NUM_CONTEXTS];
    /** Frame ID to be received */
    uint32_t frameId;
    /** Line ID to be received */
    uint32_t lineId;
    /** Buffer used to receive frame */
	uint32_t receivedBuffer;
} BoardDiag_State_t;

/**
 *  \brief    The function performs the CSIRX Diagnostic
 *            test.
 *
 *  \param    NULL      For Bare Metal test.
 *
 *  \param    arg0      [IN]    argument-0 for BIOS test
 *  \param    arg1      [IN]    argument-1 for BIOS test
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
#ifdef USE_BIOS
int8_t BoardDiag_csirxTest(UArg arg0, UArg arg1);
#else
int8_t BoardDiag_csirxTest(void);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CSIRX_TEST_TPR12_H_ */
