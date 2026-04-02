/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *  \file edma_soc.h
 *
 *  \brief EDMA Low Level Driver SOC specific file.
 */

#ifndef EDMA_SOC_H_
#define EDMA_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** @defgroup EDMA_INSTANCE Instance IDs
\ingroup DRV_EDMA_MODULE
 *
 @{ */

/*! @brief DSS_A instance of EDMA */
#define EDMA_DRV_INST_DSS_A                         (0U)
/*! @brief DSS_B instance of EDMA */
#define EDMA_DRV_INST_DSS_B                         (1U)
/*! @brief DSS_C instance of EDMA */
#define EDMA_DRV_INST_DSS_C                         (2U)
/*! @brief MSS_A instance of EDMA */
#define EDMA_DRV_INST_MSS_A                         (3U)
/*! @brief MSS_B instance of EDMA */
#define EDMA_DRV_INST_MSS_B                         (4U)
/*! @brief RCSS_A instance of EDMA */
#define EDMA_DRV_INST_RSS_A                         (5U)

/*! @brief First instance of EDMA */
#define EDMA_DRV_INST_MIN                           (EDMA_DRV_INST_DSS_A)
/*! @brief Last instance of EDMA */
#define EDMA_DRV_INST_MAX                           (EDMA_DRV_INST_RSS_A)

/** @}*/ /* end defgroup EDMA_INSTANCE */

/*! \brief Total number of DMA channels */
#define EDMA_NUM_DMA_CHANNELS (64U)

/*! \brief Total number of transfer completion codes */
#define EDMA_NUM_TCC ((uint8_t)64)

/*! \brief Total number of QDMA channels */
#define EDMA_NUM_QDMA_CHANNELS ((uint8_t)8)

/*! \brief Maximum number of event queues in any EDMA IP */
#define EDMA_MAX_NUM_EVENT_QUEUES ((uint8_t)8)

/*! \brief Maximum number of transfer controllers in any EDMA IP */
#define EDMA_MAX_NUM_TRANSFER_CONTROLLERS (EDMA_MAX_NUM_EVENT_QUEUES)

/*! \brief Number of entries in each queue in any EDMA IP */
#define EDMA_NUM_QUEUE_ENTRIES ((uint8_t)16)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void EDMA_getInstanceName (uint32_t instanceId, char *str, uint32_t size);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef EDMA_SOC_TOP_H_ */
