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

#include <ti/csl/csl_types.h>
#include <ti/csl/csl_mailbox.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/src/ip/mailbox/V0/mailbox.h>
//#include <ti/drv/mailbox/mailbox.h>
//#include <ti/drv/mailbox/src/mailbox_internal.h>

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

/*! \brief MPU1_0 Mailbox INST */
#define MAILBOX_INST_MPU1_0        (0U)
/*! \brief MCU1_0 Mailbox INST */
#define MAILBOX_INST_MCU1_0        (1U)
/*! \brief MCU1_1 Mailbox INST */
#define MAILBOX_INST_MCU1_1        (2U)
/*! \brief MCU2_0 Mailbox INST */
#define MAILBOX_INST_MCU2_0        (3U)
/*! \brief MCU2_1 Mailbox INST */
#define MAILBOX_INST_MCU2_1        (4U)
/*! \brief M4F_0 Mailbox INST */
#define MAILBOX_INST_M4F_0         (5U)
/*! \brief Last Mailbox INST */
#define MAILBOX_INST_LAST          (MAILBOX_INST_M4F_0)
/*! \brief Invalid Mailbox INST */
#define MAILBOX_INST_INVALID       (0xFFU)

/** @}*/ /* end defgroup MAILBOX_INSTANCE */

/*! \brief */
#define MAILBOX_MAX_INST           (MAILBOX_INST_LAST + 1U)

#define  MAILBOX_CLUSTER_INVALID   (0xFFU)
#define  MAILBOX_USER_INVALID      (0xFFU)

#define TEST_VIM_BASE_ADDR (CSL_MAIN_DOMAIN_VIM_BASE_ADDR)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 * \brief Mailbox interrupt router configuration
 */
typedef struct Mailbox_MbConfig_s
{
    uint32_t   priority;
    uint32_t   eventId;
    uint32_t   inputIntrNum;
    uint32_t   outputIntrNum;
}Mailbox_MbConfig;

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
