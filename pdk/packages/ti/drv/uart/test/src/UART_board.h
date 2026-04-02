/**
 *  \file   UART_board.h
 *
 *  \brief  UART Board header file
 *
 */

/*
 * Copyright (C) 2015 - 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 */


#ifndef _UART_BOARD_H
#define _UART_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Example/Board Header files */
#include <ti/board/board.h>

#ifdef UART_DMA_ENABLE
#if defined (SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/drv/udma/udma.h>
#elif defined (SOC_TPR12) || defined (SOC_AWR294X)
#include <ti/drv/edma/edma.h>
#include <ti/drv/edma/soc/edma_soc.h>
static EDMA_Handle UartApp_edmaInit(void);
#else
/* EDMA3 Header files */
#include <ti/sdo/edma3/drv/edma3_drv.h>
#include <ti/sdo/edma3/rm/edma3_rm.h>
#include <ti/sdo/edma3/rm/sample/bios6_edma3_rm_sample.h>

static EDMA3_RM_Handle UartApp_edmaInit(void);

extern EDMA3_RM_InstanceInitConfig sampleInstInitConfig[][EDMA3_MAX_REGIONS];
extern EDMA3_RM_GblConfigParams sampleEdma3GblCfgParams[];
#endif
#endif


#if defined(idkAM574x) || defined (idkAM572x) || defined (idkAM571x)
#define UART_INSTANCE   (2U)
#elif defined(SOC_TPR12) || defined (SOC_AWR294X)
/*
 * MCU: 0 (MSS UART 0), 1 (MSS UART 1, tx only), 2 (DSS UART 0)
 * DSP: 0 (DSS UART 0)
 */
#define UART_INSTANCE   (0U)
#else
#define UART_INSTANCE   (BOARD_UART_INSTANCE)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _UART_BOARD_H */
