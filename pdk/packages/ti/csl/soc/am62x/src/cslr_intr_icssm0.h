/********************************************************************
*
* ICSSM0 INTERRUPT MAP. header file
*
* Copyright (C) 2015-2020 Texas Instruments Incorporated.
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
*/
#ifndef CSLR_ICSSM0_INTERRUPT_MAP_H_
#define CSLR_ICSSM0_INTERRUPT_MAP_H_

#include <ti/csl/cslr.h>
#include <ti/csl/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* List of intr sources for receiver: ICSSM0
*/

#define CSLR_ICSSM0_ISO_RESET_PROTCOL_REQ_GLUELOGIC_MAINRESET_REQUEST_GLUE_MAIN_RESETZ_SYNC_STRETCH_0 (0U)
#define CSLR_ICSSM0_PR1_EDC0_LATCH0_IN_TIMESYNC_EVENT_ROUTER0_OUTL_8                               (0U)
#define CSLR_ICSSM0_PR1_EDC0_LATCH1_IN_TIMESYNC_EVENT_ROUTER0_OUTL_9                               (0U)
#define CSLR_ICSSM0_PR1_IEP0_CAP_INTR_REQ_MAIN_GPIOMUX_INTROUTER0_OUTP_16                          (0U)
#define CSLR_ICSSM0_PR1_SLV_INTR_UART1_USART_IRQ_0                                                 (0U)
#define CSLR_ICSSM0_PR1_IEP0_CAP_INTR_REQ_MAIN_GPIOMUX_INTROUTER0_OUTP_17                          (1U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCASP1_XMIT_INTR_PEND_0                                           (1U)
#define CSLR_ICSSM0_PR1_IEP0_CAP_INTR_REQ_MAIN_GPIOMUX_INTROUTER0_OUTP_18                          (2U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCASP1_REC_INTR_PEND_0                                            (2U)
#define CSLR_ICSSM0_PR1_IEP0_CAP_INTR_REQ_MAIN_GPIOMUX_INTROUTER0_OUTP_19                          (3U)
#define CSLR_ICSSM0_PR1_SLV_INTR_ECAP1_ECAP_INT_0                                                  (3U)
#define CSLR_ICSSM0_PR1_IEP0_CAP_INTR_REQ_MAIN_GPIOMUX_INTROUTER0_OUTP_20                          (4U)
#define CSLR_ICSSM0_PR1_SLV_INTR_ECAP2_ECAP_INT_0                                                  (4U)
#define CSLR_ICSSM0_PR1_IEP0_CAP_INTR_REQ_MAIN_GPIOMUX_INTROUTER0_OUTP_21                          (5U)
#define CSLR_ICSSM0_PR1_SLV_INTR_EPWM2_EPWM_ETINT_0                                                (5U)
#define CSLR_ICSSM0_PR1_SLV_INTR_WKUP_MCU_GPIOMUX_INTROUTER0_OUTP_12                               (6U)
#define CSLR_ICSSM0_PR1_SLV_INTR_TIMESYNC_EVENT_ROUTER0_OUTL_24                                    (7U)
#define CSLR_ICSSM0_PR1_SLV_INTR_TIMESYNC_EVENT_ROUTER0_OUTL_25                                    (8U)
#define CSLR_ICSSM0_PR1_SLV_INTR_I2C0_POINTRPEND_0                                                 (9U)
#define CSLR_ICSSM0_PR1_SLV_INTR_ECAP0_ECAP_INT_0                                                  (10U)
#define CSLR_ICSSM0_PR1_SLV_INTR_EPWM0_EPWM_ETINT_0                                                (11U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCSPI0_INTR_SPI_0                                                 (12U)
#define CSLR_ICSSM0_PR1_SLV_INTR_EQEP0_EQEP_INT_0                                                  (13U)
#define CSLR_ICSSM0_PR1_SLV_INTR_EPWM1_EPWM_ETINT_0                                                (14U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCSPI1_INTR_SPI_0                                                 (15U)
#define CSLR_ICSSM0_PR1_SLV_INTR_EQEP1_EQEP_INT_0                                                  (16U)
#define CSLR_ICSSM0_PR1_SLV_INTR_EQEP2_EQEP_INT_0                                                  (17U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MAIN_GPIOMUX_INTROUTER0_OUTP_32                                   (18U)
#define CSLR_ICSSM0_PR1_SLV_INTR_UART0_USART_IRQ_0                                                 (19U)
#define CSLR_ICSSM0_PR1_SLV_INTR_UART2_USART_IRQ_0                                                 (20U)
#define CSLR_ICSSM0_PR1_SLV_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_80                            (21U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCASP0_REC_INTR_PEND_0                                            (22U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCASP0_XMIT_INTR_PEND_0                                           (23U)
#define CSLR_ICSSM0_PR1_SLV_INTR_GLUELOGIC_PWM_TRIP_OR_GLUE_OUT_0                                  (24U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MAIN_GPIOMUX_INTROUTER0_OUTP_33                                   (25U)
#define CSLR_ICSSM0_PR1_SLV_INTR_FSS0_OSPI_0_OSPI_LVL_INTR_0                                       (26U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCASP2_XMIT_INTR_PEND_0                                           (27U)
#define CSLR_ICSSM0_PR1_SLV_INTR_MCASP2_REC_INTR_PEND_0                                            (28U)
#define CSLR_ICSSM0_PR1_SLV_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_81                            (29U)
#define CSLR_ICSSM0_PR1_SLV_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_82                            (30U)
#define CSLR_ICSSM0_PR1_SLV_INTR_DMASS0_INTAGGR_0_INTAGGR_VINTR_PEND_83                            (31U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_ICSSM0_INTERRUPT_MAP_H_ */

