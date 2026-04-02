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

/*!
 * \file     cpsw_test_sgmii_lpbk.h.c
 *
 * \brief    This file contains the cpsw_test_sgmii_lpbk test implementation.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/drv/uart/UART_stdio.h>
#include <ti/csl/csl_cpswitch.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>
#if defined (SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_ethernet_config.h>
#elif defined(SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_ethernet_config.h>
#endif
#include "enet_test_entry.h"
#include "enet_test_base.h"
#include "cpsw_test_sgmii_lpbk.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
void EnetTestSgmiiLpbk_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                         Enet_MacPort portNum)
{
    CpswMacPort_Cfg *macCfg = (CpswMacPort_Cfg*)pLinkArgs->macCfg;
    Enet_Type enetType;
    uint32_t instId;
    EnetMacPort_Interface mii = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_SERIAL };

#if defined (SOC_J721E)
    enetType = ENET_CPSW_9G;
    instId   = 0U;
#elif defined (SOC_J7200)
    enetType = ENET_CPSW_5G;
    instId   = 0U;
#endif

    /* Override the ENET control set by board lib */
    EnetBoard_setEnetControl(enetType, instId, portNum, &mii);

    /* We use RMII port for this testing */
   EnetAppUtils_setNoPhyCfgSgmii(&pLinkArgs->mii,
                                  macCfg,
                                  &pLinkArgs->phyCfg);

    macCfg->loopbackEn = BTRUE;
    pLinkArgs->linkCfg.speed = ENET_SPEED_1GBIT;
    pLinkArgs->linkCfg.duplexity= ENET_DUPLEX_FULL;
}
