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
 * \file     cpsw_test_board_j721e_gesi.c
 *
 * \brief    This file contains the board specific portion
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/osal/osal.h>

#include <ti/board/board.h>

#include "enet_test_entry.h"
#include "enet_test_board.h"
#include <ti/drv/enet/examples/utils/include/enet_board.h>

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

static EnetBoard_EthPort gEnetTestBoard_ports[] =
{
    {
        .macPort  = ENET_MAC_PORT_1,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId  = ENETBOARD_GESI_ID,
    },
    {
        .macPort  = ENET_MAC_PORT_8,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId  = ENETBOARD_GESI_ID,
    },
    {
        .macPort  = ENET_MAC_PORT_3,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId  = ENETBOARD_GESI_ID,
    },
    {
        .macPort  = ENET_MAC_PORT_4,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId  = ENETBOARD_GESI_ID,
    },
    {
        .macPort  = ENET_MAC_PORT_2,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN },
        .boardId  = ENETBOARD_GESI_ID,
    },
    {
        .macPort  = ENET_MAC_PORT_5,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .boardId  = ENETBOARD_GESI_ID,
    },
    {
        .macPort  = ENET_MAC_PORT_6,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .boardId  = ENETBOARD_GESI_ID,
    },
    {
        .macPort  = ENET_MAC_PORT_7,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB },
        .boardId  = ENETBOARD_GESI_ID,
    },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

uint32_t EnetTestBoard_getPhyAddr(Enet_Type enetType,
                                  Enet_MacPort portNum)
{
    const EnetBoard_PortCfg *portCfg = NULL;
    uint32_t instId = 0U;
    uint32_t i;

    for (i = 0U; i < ENET_ARRAYSIZE(gEnetTestBoard_ports); i++)
    {
        if (gEnetTestBoard_ports[i].macPort == portNum)
        {
            portCfg = EnetBoard_getPortCfg(enetType, instId, &gEnetTestBoard_ports[i]);
            break;
        }
    }

    return portCfg->phyCfg.phyAddr;
}

void EnetTestBoard_setPhyConfig(EnetTestTaskObj *taskObj,
                                Enet_MacPort portNum,
                                CpswMacPort_Cfg *macCfg,
                                EnetMacPort_Interface *interface,
                                EnetPhy_Cfg *phyCfg)
{
    const EnetBoard_PortCfg *portCfg;
    uint32_t i;

    for (i = 0U; i < ENET_ARRAYSIZE(gEnetTestBoard_ports); i++)
    {
        if (gEnetTestBoard_ports[i].macPort == portNum)
        {
            portCfg = EnetBoard_getPortCfg(taskObj->taskCfg->enetType,
                                           taskObj->taskCfg->instId,
                                           &gEnetTestBoard_ports[i]);

            EnetPhy_initCfg(phyCfg);
            phyCfg->phyAddr     = portCfg->phyCfg.phyAddr;
            phyCfg->isStrapped  = portCfg->phyCfg.isStrapped;
            phyCfg->loopbackEn  = BFALSE;
            phyCfg->skipExtendedCfg = portCfg->phyCfg.skipExtendedCfg;
            phyCfg->extendedCfgSize = portCfg->phyCfg.extendedCfgSize;
            memcpy(phyCfg->extendedCfg, portCfg->phyCfg.extendedCfg, phyCfg->extendedCfgSize);

            macCfg->sgmiiMode = portCfg->sgmiiMode;
            break;
        }
    }
}

void EnetTestBoard_init(void)
{
    Enet_Type enetType = ENET_CPSW_9G;
    uint32_t instId = 0U;
    int32_t status;

    EnetBoard_init();

    status = EnetBoard_setupPorts(enetType, instId,
                                  gEnetTestBoard_ports,
                                  ENET_ARRAYSIZE(gEnetTestBoard_ports));
    EnetAppUtils_assert(status == ENET_SOK);

#if defined(BUILD_MCU2_0)
#if defined(SOC_J721E)
    /*  RGMII ports: 1, 3, 4, 8
     * QSGMII ports: 2, 5, 6, 7 */
    Board_pinmuxUpdate(gJ721E_MainPinmuxDataGesiCpsw9gQsgmii,
                       BOARD_SOC_DOMAIN_MAIN);
#elif defined(SOC_J7200)
    /* Below is CPSW5G QSGMII pinmux configuration,
       RGMII Ports - 1. QSGMII ports - 2,3,4 */
    Board_pinmuxUpdate(gJ7200_MainPinmuxDataCpsw,
                       BOARD_SOC_DOMAIN_MAIN);
    Board_pinmuxUpdate(gJ7200_WkupPinmuxDataCpsw ,
                       BOARD_SOC_DOMAIN_MAIN);
#else
#error "Invalid Config"
#endif
#endif
}

void EnetTestBoard_deInit(void)
{
    EnetBoard_deinit();
}

/* end of file */
