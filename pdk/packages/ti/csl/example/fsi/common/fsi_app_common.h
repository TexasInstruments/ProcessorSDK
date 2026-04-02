/**
 *  \file   fsi_app_common.h
 *
 *  \brief  FSI appliction common driver header
 *
 */

/*
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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


#ifndef FSI_APP_COMMON_H
#define FSI_APP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include <ti/csl/soc.h>
#include <ti/csl/csl_fsi_rx.h>
#include <ti/csl/csl_fsi_tx.h>
#include <ti/osal/osal.h>

/* FSI application function return status */
#define FSI_APP_STATUS_SUCCESS          ((int32_t)(0))
#define FSI_APP_STATUS_ERROR            (-((int32_t)1))

/* FSI application max # of interrupts per TX/RX instance */
#define FSI_APP_MAX_NUM_INTS            (2U)


typedef struct FSI_APP_Object_s
{
    uint32_t        txBaseAddr[CSL_FSITX_MAIN_CNT]; /* base address for FSI_TX */
    uint32_t        rxBaseAddr[CSL_FSIRX_MAIN_CNT]; /* base address for FSI_RX */
    uint32_t        lbEnable;                        /* Loopback enable flag */
    uint32_t        intEnable;                       /* Interrupt enable flag */
    uint32_t        txIntNum[CSL_FSITX_MAIN_CNT][FSI_APP_MAX_NUM_INTS];  /* Int # for txInt1, txInt2 */
    uint32_t        rxIntNum[CSL_FSIRX_MAIN_CNT][FSI_APP_MAX_NUM_INTS];  /* Int # for rxInt1, rxInt2 */
    Osal_IsrRoutine txIsr[CSL_FSITX_MAIN_CNT][FSI_APP_MAX_NUM_INTS];     /* ISR func pointer for txInt1, txInt2, */
    Osal_IsrRoutine rxIsr[CSL_FSIRX_MAIN_CNT][FSI_APP_MAX_NUM_INTS];     /* ISR func pointer for rxInt1, rxInt2 */
    void*           txHwi[CSL_FSITX_MAIN_CNT][FSI_APP_MAX_NUM_INTS];     /* hwi pointer for txInt1, txInt2 */
    void*           rxHwi[CSL_FSIRX_MAIN_CNT][FSI_APP_MAX_NUM_INTS];     /* hwi pointer for rxInt1, rxInt2 */
    uint32_t        txPreScaler;                     /* FSI TX pre-scaler value */

    uint16_t        txEventSts[CSL_FSITX_MAIN_CNT];  /* FSI TX interrupt event status */
    uint16_t        rxEventSts[CSL_FSIRX_MAIN_CNT];  /* FSI RX interrupt event status */

#if defined (SOC_AM64X)
    uint64_t        txBufData[(FSI_MAX_VALUE_BUF_PTR_OFF + 1U)/4U];
#else
    uint16_t        txBufData[FSI_MAX_VALUE_BUF_PTR_OFF + 1U];
#endif

    volatile uint32_t txInt1Received[CSL_FSITX_MAIN_CNT];
    volatile uint32_t txInt2Received[CSL_FSITX_MAIN_CNT];
    volatile uint32_t rxInt1Received[CSL_FSIRX_MAIN_CNT];
    volatile uint32_t rxInt2Received[CSL_FSIRX_MAIN_CNT];

    uint32_t        txTimeOutCntr;
    uint32_t        rxTimeOutCntr;
    uint32_t        dataFrameCntr[CSL_FSIRX_MAIN_CNT];
    uint32_t        errorCntr;
} FSI_APP_Object;

extern void UART_printf(const char *pcString, ...);
extern int32_t FSI_App_init(FSI_APP_Object *pObj);
extern int32_t FSI_App_open(FSI_APP_Object *pObj);
extern int32_t FSI_App_close(FSI_APP_Object *pObj);

#ifndef IO_CONSOLE
#define FSI_log                UART_printf
#else
#define FSI_log                printf
#endif



#ifdef __cplusplus
}
#endif

#endif /* FSI_APP_COMMON_H */
