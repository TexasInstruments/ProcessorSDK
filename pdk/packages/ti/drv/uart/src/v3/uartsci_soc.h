/**
 *   @file  uartsci_soc.h
 *
 *   @brief
 *      This is the SoC Header for the UART SCI Driver, which defines
 *      SoC specific parameters.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/** @defgroup UART_SCI_SOC      UART Driver SoC Parameters
 */
#ifndef UART_SCI_SOC_H
#define UART_SCI_SOC_H

#include <stdint.h>
#include <stddef.h>

/**
@defgroup UART_SCI_INTERNAL_DATA_STRUCTURE      UART Driver Internal Data Structures
@ingroup UART_SCI_SOC
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup UART_SCI_SOC_INTERNAL_DATA_STRUCTURE
 @{ */

/*!
 * @brief   Command code used by UART_control() to place the UART in loopback mode
 *
 * This command is available to place the UART Driver in loopback mode. This is
 * a debug only feature. With this command code, @b arg is a pointer to an
 * integer. @b *arg contains the status of the loopback. 1 will enable the loopback
 * and 0 will disable it
 */
#define UART_CMD_LOOPBACK           (0U)

/*!
 * @brief   Command code used by UART_control() to get the UART Instance statistics
 *
 * This command is available to get the statistics associated with the UART instance.
 * This is a debug only feature. With this command code, @b arg is a pointer to the
 * UART_Stats structure.
 */
#define UART_CMD_GET_STATS          (1U)

/*!
 *  @brief  UART Statistics
 *
 *  This structure holds the statistics which are maintained by the UART
 *  driver instance and can be used for debugging purpose
  *
 *  @sa     UART_control()
 */
typedef struct UART_Stats_t
{
    /**
     * @brief   Number of receive interrupts
     */
    uint32_t    numRxInterrupts;

    /**
     * @brief   Number of transmit interrupts
     */
    uint32_t    numTxInterrupts;

    /**
     * @brief   Number of receive overrun interrupts
     */
    uint32_t    numRxOverrunInterrupts;

    /**
     * @brief   Number of dummy UART reads: Data was received on the console
     * but the application did not provide a buffer.
     */
    uint32_t    numDummyRead;

    /**
     * @brief   Total Number of interrupts
     */
    uint32_t    totalInterrupts;

    /**
     * @brief   Number of Receive DMA Interrupts: This is only if the UART instance
     * is configured to operate in DMA mode.
     */
    uint32_t    numRxDMAInterrupts;

    /**
     * @brief   Number of Transmit DMA Interrupts: This is only if the UART instance
     * is configured to operate in DMA mode.
     */
    uint32_t    numTxDMAInterrupts;

    /**
     * @brief   Number of Framing Errors:
     */
    uint32_t    numFramingErrors;

    /**
     * @brief   Number of Parity Errors:
     */
    uint32_t    numParityError;
}UART_Stats;

typedef struct UartSci_SwCfg_t  UartSci_SwCfg;
typedef struct UartSci_SwCfg_t  UART_HwAttrs;

/**
 * @brief
 *  SCI Driver SW configuration
 *
 * @details
 *  The structure is used to store the software specific configuration which is
 *  passed to each driver instance
 */
struct UartSci_SwCfg_t
{
    /**
     * @brief   Input Clock Frequency
     */
    uint32_t                    clockFrequency;

    /**
     * @brief   Handle to the EDMA Instance
     */
    void*                       edmaHandle;

    /**
     * @brief   Id of EDMA PaRAM set used to setup the transfers
     */
    uint32_t                    paramSetId;

    /**
     * @brief   flag to indicate in DMA mode
     */
    uint32_t                    dmaMode;

    /**
     * @brief   flag to indicate in loopback mode
     */
    uint32_t                    loopback;

    /**
     * @brief   flag to enable interrupt
     */
    uint32_t                    enableInterrupt;
};

void UART_socInitHwCfg(UART_Handle handle);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* UART_SCI_SOC_H */
