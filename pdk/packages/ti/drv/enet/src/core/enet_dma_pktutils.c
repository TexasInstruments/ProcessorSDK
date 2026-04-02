/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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

/*!
 * \file  enet_dma_pktutils.c
 *
 * \brief This file contains the implementation of Enet DMA packet utils.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x006

#include <stdint.h>
#include <stdarg.h>
#include <ti/drv/enet/include/core/enet_dma_pktutils.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void EnetDma_checkDescState(uint32_t *pStateVar,
                            EnetDma_DescStateMemMgr expectedState,
                            EnetDma_DescStateMemMgr newState)
{
    Enet_assert(ENET_UTILS_GET_DESC_MEMMGR_STATE(pStateVar) == expectedState);
    ENET_UTILS_SET_DESC_MEMMGR_STATE(pStateVar, newState);
}

void EnetDma_checkPktState(uint32_t *pStateVar,
                           EnetDma_PktStateModuleType module,
                           uint32_t expectedState,
                           uint32_t newState)
{
    switch (module)
    {
        case ENET_PKTSTATE_MODULE_APP:
            Enet_assert(ENET_UTILS_GET_PKT_APP_STATE(pStateVar) == expectedState);
            ENET_UTILS_SET_PKT_APP_STATE(pStateVar, newState);
            break;

        case ENET_PKTSTATE_MODULE_DRIVER:
            Enet_assert(ENET_UTILS_GET_PKT_DRIVER_STATE(pStateVar) == expectedState);
            ENET_UTILS_SET_PKT_DRIVER_STATE(pStateVar, newState);
            break;

        case ENET_PKTSTATE_MODULE_MEMMGR:
            Enet_assert(ENET_UTILS_GET_PKT_MEMMGR_STATE(pStateVar) == expectedState);
            ENET_UTILS_SET_PKT_MEMMGR_STATE(pStateVar, newState);
            break;

        default:
            break;
    }
}
