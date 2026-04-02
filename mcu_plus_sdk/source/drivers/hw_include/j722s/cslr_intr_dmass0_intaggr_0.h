/********************************************************************
*
* DMASS0_INTAGGR_0 INTERRUPT MAP. header file
*
* Copyright (C) 2015-2023 Texas Instruments Incorporated.
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
#ifndef CSLR_DMASS0_INTAGGR_0_INTERRUPT_MAP_H_
#define CSLR_DMASS0_INTAGGR_0_INTERRUPT_MAP_H_

#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/tistdtypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
* List of intr sources for receiver: DMASS0_INTAGGR_0
*/

#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_CPSW0_CPTS_COMP_0                                  (0U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_PCIE0_PCIE_CPTS_COMP_0                             (1U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_DSS0_DISPC_INTR_REQ_0_0                            (2U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_DSS0_DISPC_INTR_REQ_1_0                            (3U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_DSS1_DISPC_INTR_REQ_0_0                            (4U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_DSS1_DISPC_INTR_REQ_1_0                            (5U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MCRC64_0_INT_MCRC_0                                (7U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_0                   (8U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_1                   (9U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_2                   (10U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_3                   (11U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_4                   (12U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_5                   (13U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_6                   (14U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_TIMESYNC_EVENT_INTROUTER0_OUTL_7                   (15U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_24                    (16U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_25                    (17U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_26                    (18U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_27                    (19U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_28                    (20U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_29                    (21U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_30                    (22U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_31                    (23U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_22                    (24U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MAIN_GPIOMUX_INTROUTER0_OUTP_23                    (25U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_GPMC0_GPMC_SDMAREQ_0                               (26U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_DEBUGSS0_DAVDMA_LEVEL_0                            (27U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MCRC64_0_DMA_EVENT_0                               (28U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MCRC64_0_DMA_EVENT_1                               (29U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MCRC64_0_DMA_EVENT_2                               (30U)
#define CSLR_DMASS0_INTAGGR_0_INTAGGR_LEVI_PEND_MCRC64_0_DMA_EVENT_3                               (31U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_DMASS0_INTAGGR_0_INTERRUPT_MAP_H_ */

