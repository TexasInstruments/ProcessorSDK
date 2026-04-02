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
 *  \file mailbox_soc.h
 *
 *  \brief MAILBOX Driver SOC specific file.
 */

#ifndef MAILBOX_SOC_H_
#define MAILBOX_SOC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/*! @brief Mailbox Instance Number */
typedef uint32_t Mailbox_Instance;

/** @defgroup MAILBOX_INSTANCE Instance IDs
\ingroup DRV_MAILBOX_MODULE
 *
 @{ */

/*! \brief MCU1_0 Mailbox INST */
#define MAILBOX_INST_MSS_CR5A        (0U)
/*! \brief MCU1_1 Mailbox INST */
#define MAILBOX_INST_MSS_CR5B        (1U)
/*! \brief HSM M4 Mailbox INST */
#define MAILBOX_INST_HSM_CM4         (2U)
/*! \brief C66x Mailbox INST */
#define MAILBOX_INST_DSP             (4U)
/*! \brief DSS M4 Mailbox INST */
#define MAILBOX_INST_DSP_CM4         (5U)
/*! \brief Last Mailbox INST */
#define MAILBOX_INST_LAST            (MAILBOX_INST_DSP_CM4)
/*! \brief Invalid Mailbox INST */
#define MAILBOX_INST_INVALID         (0xFFU)

/** @}*/ /* end defgroup MAILBOX_INSTANCE */

/*! \brief */
#define MAILBOX_MAX_INST           (MAILBOX_INST_LAST + 1U)

#define MAILBOX_DRIVER_POOL_NUM_ELEMENTS    (MAILBOX_MAX_INST * MAILBOX_CH_ID_MAX)

/* Devide DSS Mailbox mem half between CR5A and CR5B. */
#define DSS_MBX_CR5A_OFFSET         (0)
#define DSS_MBX_CR5B_OFFSET         (CSL_DSS_MAILBOX_U_SIZE/2)

/* R5 Mailbox is common betweeen CR5A and CR5B.
 * Divide equally between the supported remote croes. */
#define CR5A_MBX_DSS_OFFSET         (0)
#define CR5A_MBX_CR5B_OFFSET        (CSL_MSS_MAILBOX_U_SIZE/4)

#define CR5B_MBX_DSS_OFFSET         (CSL_MSS_MAILBOX_U_SIZE/2)
#define CR5B_MBX_CR5A_OFFSET        ((CSL_MSS_MAILBOX_U_SIZE/4)*3)

/* Mailbox addresses for writing from Core1 to Core2. */
#define CR5A_TO_DSS_MBX_MEM         (CSL_DSS_MAILBOX_U_BASE + DSS_MBX_CR5A_OFFSET)
#define CR5A_TO_CR5B_MBX_MEM        (CSL_MSS_MBOX_U_BASE + CR5B_MBX_CR5A_OFFSET)

#define CR5B_TO_DSS_MBX_MEM         (CSL_DSS_MAILBOX_U_BASE + DSS_MBX_CR5B_OFFSET)
#define CR5B_TO_CR5A_MBX_MEM        (CSL_MSS_MBOX_U_BASE + CR5A_MBX_CR5B_OFFSET)

#define DSS_TO_CR5A_MBX_MEM         (CSL_MSS_MBOX_U_BASE + CR5A_MBX_DSS_OFFSET)
#define DSS_TO_CR5B_MBX_MEM         (CSL_MSS_MBOX_U_BASE + CR5B_MBX_DSS_OFFSET)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MAILBOX_SOC_H_ */
