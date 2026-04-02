/*
 *  Copyright (c) Texas Instruments Incorporated 2026
 *  All rights reserved.
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
 *  \file     ecc_ddr.h
 *
 *  \brief    This file contains DDR ECC example code defines.
 */

/*===========================================================================*/
/*                            Include files                                  */
/*===========================================================================*/

#ifndef ECC_DDR_H_
#define ECC_DDR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/csl/csl_emif.h>
#include <ti/csl/soc.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/uart/UART_stdio.h>

#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

extern volatile uint32_t *gTest_Addr;

#if defined (SOC_J721S2)
#define DDRSS0_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_DDR0_0_SS_CFG_BASE)
#define DDRSS1_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_DDR1_1_SS_CFG_BASE)
#elif defined (SOC_J784S4)
#define DDRSS0_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_SSCFG_BASE)
#define DDRSS1_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_VBUSP_DDRSS1_SSCFG_BASE)
#define DDRSS2_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_VBUSP_DDRSS2_SSCFG_BASE)
#define DDRSS3_CFG_BASE                  (CSL_COMPUTE_CLUSTER0_VBUSP_DDRSS3_SSCFG_BASE)
#endif

#if defined (SOC_J721S2)
#define NUM_DDR_SUBSYSTEMS               (2)
#elif defined (SOC_J784S4)
#define NUM_DDR_SUBSYSTEMS               (3)
#endif

#define DDR_ECC_START_ADDR               (0x80000000)
#define DDR_ECC_END_ADDR                 (0xA0000000)
#define DDR_ECC_START_ADDR1              (0xA0000000)
#define DDR_ECC_END_ADDR1                (0xFFFFFFFF)
#define DDR_ECC_START_ADDR2              (0x880000000)
#define DDR_ECC_END_ADDR2                (0x900000000)

#define EMIF_DDR_START_ADDR              (0x80000000u)
#define EMIF_DDR_LOW_MEM_SIZE            (0x80000000u)
#define EMIF_DDR_HIMEM_START_ADDR        (0x880000000U)
#define EMIF_DDR_HIMEM_SIZE              (0x10000000u)

#define DDR_HIMEM_R5F_ADDRESS            (0x60000000)
#define DDR_HIMEM_RAT_REGION             (10)

/* ECC Aggregation register RAT configuration */
#if defined (SOC_J784S4) 
#define ECC_AGGR_PHYS_BASE        (CSL_COMPUTE_CLUSTER0_VBUSP_DDRSS0_ECC_AGGR_CTL_BASE)
#else
#define ECC_AGGR_PHYS_BASE        (CSL_COMPUTE_CLUSTER0_DDR0_0_ECC_AGGR_CTL_BASE)
#endif

/* Size of the memory being translated */
#define ECC_AGGR_SPAN_BYTES       (0x4000ULL)
/* Base address forr ECC Aggregation registers after translation */
#define ECC_AGGR_LOCAL_BASE       (0x50000000u)
/* Set a different region than DDR high */
#define ECC_AGGR_RAT_REGION       (11u) 

/* Base addresses for different DDRSS instances */
#define DDRSS0_ECC_AGGR_LOCAL   (ECC_AGGR_LOCAL_BASE + 0x0000u)
#define DDRSS1_ECC_AGGR_LOCAL   (ECC_AGGR_LOCAL_BASE + 0x0C00u)
#if defined (SOC_J784S4)
#define DDRSS2_ECC_AGGR_LOCAL   (ECC_AGGR_LOCAL_BASE + 0x1800u)
#define DDRSS3_ECC_AGGR_LOCAL   (ECC_AGGR_LOCAL_BASE + 0x2400u)
#endif


/* Test address set based on interleaving factor of 128 bytes */
#define DDRSS0_TEST_ADDRESS              (0x80000015)
#define DDRSS1_TEST_ADDRESS              (0x80000095)
#define DDRSS2_TEST_ADDRESS              (0x80000115)
#define DDRSS3_TEST_ADDRESS              (0x80000195)
#define DDRSS0_HIMEM_TEST_ADDRESS        (DDR_HIMEM_R5F_ADDRESS + 0x10)
#define DDRSS1_HIMEM_TEST_ADDRESS        (DDR_HIMEM_R5F_ADDRESS + 0x90)
#define DDRSS2_HIMEM_TEST_ADDRESS        (DDR_HIMEM_R5F_ADDRESS + 0x110)
#define DDRSS3_HIMEM_TEST_ADDRESS        (DDR_HIMEM_R5F_ADDRESS + 0x190)


/* ========================================================================== */
/*                         Function Declarations                              */
/* ========================================================================== */

int32_t DdrApp_DdrEccTest(void);
int32_t DdrApp_DdrEnableECC(CSL_emif_sscfgRegs *ddrCfgAddr);
int32_t DdrApp_DdrDisableECC(CSL_emif_sscfgRegs *ddrCfgAddr);

int32_t DdrApp_EccDdrUdmaPrimeDDR(uint64_t startAddr, uint32_t size);
int32_t DdrApp_EccDdrUdmaInit(Udma_DrvHandle drvHandle);
int32_t DdrApp_EccDdrUdmaDeInit(Udma_DrvHandle drvHandle);

#ifdef __cplusplus
}
#endif

#endif /* ECC_DDR_H_ */