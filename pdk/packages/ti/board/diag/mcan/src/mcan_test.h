/******************************************************************************
 * Copyright (c) 2018-2022 Texas Instruments Incorporated - http://www.ti.com
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
 
/**
 *
 * \file   mcan_test.h
 *
 * \brief  This is the header file for mcan diagnostic test.
 *
 */

#ifndef _MCAN_TEST_H_
#define _MCAN_TEST_H_ 

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr_mcan.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_mcan.h>
#if !defined(SOC_AM64X)
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#endif

#include "board.h"

#if defined(SOC_AM64X)
#include "board_cfg.h"
#endif
#include "diag_common_cfg.h"

#if defined(SOC_J721E) || defined(SOC_J7200)|| defined(SOC_J721S2) || defined(SOC_J784S4)
#include "board_pinmux.h"
#include "board_control.h"
#endif
#if !defined(am65xx_idk) && !defined(SOC_TPR12) && !defined(SOC_AWR294X)
#include "board_i2c_io_exp.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#if defined(DIAG_STRESS_TEST) && ((defined(am65xx_idk)) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X)  || defined(SOC_J721S2) || defined(SOC_J784S4))
#define PKT_SEND_COUNT                  (10240U)
#else
#define PKT_SEND_COUNT                  (5U)
#endif

#define APP_MCAN_STD_ID_FILT_START_ADDR          (0U)
#define APP_MCAN_STD_ID_FILTER_NUM               (1U)
#define APP_MCAN_EXT_ID_FILT_START_ADDR          (48U)
#define APP_MCAN_EXT_ID_FILTER_NUM               (1U)
#define APP_MCAN_TX_EVENT_START_ADDR             (100U)
#define APP_MCAN_TX_EVENT_SIZE                   (5U)
#define APP_MCAN_TX_BUFF_START_ADDR              (148U)
#define APP_MCAN_TX_BUFF_SIZE                    (5U)
#define APP_MCAN_TX_FIFO_SIZE                    (5U)
#define APP_MCAN_FIFO_0_START_ADDR               (548U)
#define APP_MCAN_FIFO_0_NUM                      (5U)
#define APP_MCAN_FIFO_1_START_ADDR               (748U)
#define APP_MCAN_FIFO_1_NUM                      (5U)
#define APP_MCAN_RX_BUFF_START_ADDR              (948U)

#define APP_MCAN_TX_BUF_NUM                      (1U)

#define APP_MCAN_EXT_ID_AND_MASK                 (0x1FFFFFFFU)
#define MCAN_MAX_PAYLOAD_BYTES                   (64U)

#if defined(am65xx_idk)
#if defined (__aarch64__)
#define MCAN_DIAG_INTR_ENABLE                    (1)
#endif
#endif

#if defined(j721e_evm) || defined(j7200_evm)|| defined(j721s2_evm) || defined(j784s4_evm)

#if defined(j721s2_evm)
#define MCAN_MAX_PORTS_EXP    (0U)
#define MCAN_MAX_PORTS_CP     (4U)
#elif defined(j784s4_evm)
#define MCAN_MAX_PORTS_EXP    (0U)
#define MCAN_MAX_PORTS_CP     (6U)
#else
#define MCAN_MAX_PORTS_EXP    (10U)
#define MCAN_MAX_PORTS_CP     (4U)
#endif

#define BOARD_NAME_LENGTH     (12U)

/* Interrupt configurations */
#define MCU_MCAN0_TX_INT_NUM    (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN0_MCANSS_MCAN_LVL_INT_0)
#define MCU_MCAN0_RX_INT_NUM    (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN0_MCANSS_MCAN_LVL_INT_1)
#define MCU_MCAN0_TS_INT_NUM    (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#define MCU_MCAN1_TX_INT_NUM    (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN1_MCANSS_MCAN_LVL_INT_0)
#define MCU_MCAN1_RX_INT_NUM    (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN1_MCANSS_MCAN_LVL_INT_1)
#define MCU_MCAN1_TS_INT_NUM    (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN1_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#define MAIN_MCAN0_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN0_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN0_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN0_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN0_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#if defined(j721e_evm)
#define MAIN_MCAN2_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN2_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN2_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN2_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN2_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN2_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

#if defined(j7200_evm) || defined (j721s2_evm) || defined (j784s4_evm)
#define MAIN_MCAN3_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN3_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN3_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN3_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN3_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN3_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

#define MAIN_MCAN4_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN4_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN4_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN4_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN4_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN4_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#define MAIN_MCAN5_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN5_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN5_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN5_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN5_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN5_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#define MAIN_MCAN6_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN6_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN6_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN6_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN6_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN6_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#define MAIN_MCAN7_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN7_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN7_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN7_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN7_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN7_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#if defined(j7200_evm)
#define MAIN_MCAN8_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN8_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN8_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN8_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN8_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN8_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

#if defined(j721e_evm)
#define MAIN_MCAN9_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN9_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN9_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN9_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN9_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN9_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

#if defined(j7200_evm)
#define MAIN_MCAN10_TX_INT_NUM  (CSLR_R5FSS0_CORE0_INTR_MCAN10_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN10_RX_INT_NUM  (CSLR_R5FSS0_CORE0_INTR_MCAN10_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN10_TS_INT_NUM  (CSLR_R5FSS0_CORE0_INTR_MCAN10_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

#if defined(j721e_evm)
#define MAIN_MCAN11_TX_INT_NUM  (CSLR_R5FSS0_CORE0_INTR_MCAN11_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN11_RX_INT_NUM  (CSLR_R5FSS0_CORE0_INTR_MCAN11_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN11_TS_INT_NUM  (CSLR_R5FSS0_CORE0_INTR_MCAN11_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

#if defined(j784s4_evm)
#define MAIN_MCAN16_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN16_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN16_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN16_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN16_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN16_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif
#elif defined(SOC_AM64X)
#define MCAN_MAX_PORTS    (2U)

/* Interrupt configurations */

#define MAIN_MCAN0_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN0_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN0_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN0_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN0_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#define MAIN_MCAN1_TX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN1_MCANSS_MCAN_LVL_INT_0)
#define MAIN_MCAN1_RX_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN1_MCANSS_MCAN_LVL_INT_1)
#define MAIN_MCAN1_TS_INT_NUM   (CSLR_R5FSS0_CORE0_INTR_MCAN1_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)

#elif (defined(SOC_TPR12) || defined(SOC_AWR294X))
#define MCAN_MAX_PORTS    (2U)

/* Interrupt configurations */

#define MAIN_MCAN0_TX_INT_NUM   (CSL_MSS_INTR_MSS_MCANA_INT0)
#define MAIN_MCAN0_RX_INT_NUM   (CSL_MSS_INTR_MSS_MCANA_INT1)
#define MAIN_MCAN0_TS_INT_NUM   (ESMG1_MCANA_TS_ERR)

#define MAIN_MCAN1_TX_INT_NUM   (CSL_MSS_INTR_MSS_MCANB_INT0)
#define MAIN_MCAN1_RX_INT_NUM   (CSL_MSS_INTR_MSS_MCANB_INT1)
#define MAIN_MCAN1_TS_INT_NUM   (ESMG1_MCANB_TS_ERR)

#else
#define MCAN_MAX_PORTS    (2U)
#endif /* #if defined(j721e_evm) || defined(j7200_evm) */

/**
 * \brief Structure to hold the MCAN port mapping information
 */
typedef struct BoardDiag_McanPortInfo_s
{
    /** Base address of MCAN port */
    uint32_t mcanBaseAddr;
    /** Instance number of MCAN port */
	uint8_t  mcanInstance;
    /** MCAN interrupt number for Tx channel */
    uint32_t mcanTxIntNum;
    /** MCAN interrupt number for Rx channel */
    uint32_t mcanRxIntNum;
    /** MCAN interrupt number for Timestamp */
    uint32_t mcanTsIntNum;
} BoardDiag_McanPortInfo_t;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   This function executes MCAN diagnostic test
 *
 * \return  int32_t
 *             0   - in case of success
 *            -1   - in case of failure
 *
 */
int32_t BoardDiag_mcanTest(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MCAN_TEST_H_ */

