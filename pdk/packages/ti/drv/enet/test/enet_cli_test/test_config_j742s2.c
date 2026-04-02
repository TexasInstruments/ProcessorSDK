/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 * \file  test_config_j742s2.c
 *
 * \brief This file contains J742S2-specific test configuration parameters.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <ti/board/board.h>
#include <ti/board/src/j742s2_evm/include/board_ethernet_config.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* NONE */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* NONE */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void EnetApp_getTestConfig(Enet_Type *enetType,
                           uint32_t *instId,
                           Enet_MacPort *macPort,
                           emac_mode *macMode,
                           uint32_t *boardId,
                           uint32_t *expPort,
                           uint8_t *numMacPorts,
                           uint8_t numPortsToEnable)
{
    uint8_t portNum;

    #if defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    {
        *enetType = ENET_CPSW_9G;
        *instId   = 0U,
        *numMacPorts = 8;
    }
    #elif defined(BUILD_MCU2_1)
    {
        *enetType = ENET_CPSW_2G;
        *instId   = 1U;
        *numMacPorts = 1;
    }
    #else
    {
        *enetType = ENET_CPSW_2G;
        *instId   = 0U;
        *numMacPorts = 1;
    }
    #endif
    
    for ( portNum = 0; portNum < *numMacPorts; portNum++)
    {
        switch(portNum)
        {
            case 0:
            {
                macPort[portNum] = ENET_MAC_PORT_1;
                macMode[portNum] = QSGMII;
                boardId[portNum] = ENETBOARD_QPENET_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET1;
                break;
            }
            case 1:
            {
                macPort[portNum] = ENET_MAC_PORT_2;
                macMode[portNum] = QSGMII;
                boardId[portNum] = ENETBOARD_BRIDGE_SGMII_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET1;
                break;
            }
            case 2:
            {
                macPort[portNum] = ENET_MAC_PORT_3;
                macMode[portNum] = QSGMII_SUB;
                boardId[portNum] = ENETBOARD_QPENET_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET2;
                break;
            }
            case 3:
            {
                macPort[portNum] = ENET_MAC_PORT_4;
                macMode[portNum] = QSGMII_SUB;
                boardId[portNum] = ENETBOARD_QPENET_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET1;
                break;
            }
            case 4:
            {
                macPort[portNum] = ENET_MAC_PORT_5;
                macMode[portNum] = QSGMII_SUB;
                boardId[portNum] = ENETBOARD_QPENET_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET1;
                break;
            }
            case 5:
            {
                macPort[portNum] = ENET_MAC_PORT_6;
                macMode[portNum] = QSGMII_SUB;
                boardId[portNum] = ENETBOARD_QPENET_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET1;
                break;
            }
            case 6:
            {
                macPort[portNum] = ENET_MAC_PORT_7;
                macMode[portNum] = QSGMII_SUB;
                boardId[portNum] = ENETBOARD_QPENET_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET1;
                break;
            }
            case 7:
            {
                macPort[portNum] = ENET_MAC_PORT_8;
                macMode[portNum] = QSGMII_SUB;
                boardId[portNum] = ENETBOARD_QPENET_ID;
                expPort[portNum] = ENETBOARD_EXP_PORT_ENET1;
                break;
            }
        }
    }
}

int32_t EnetApp_macMode2PhyMii(emac_mode macMode,
                               EnetPhy_Mii *mii)
{
    int32_t status = ENET_SOK;

    switch (macMode)
    {
        case RMII:
            *mii = ENETPHY_MAC_MII_RMII;
            break;

        case RGMII:
            *mii = ENETPHY_MAC_MII_RGMII;
            break;

        case GMII:
            *mii = ENETPHY_MAC_MII_GMII;
            break;

        case SGMII:
            *mii = ENETPHY_MAC_MII_SGMII;
            break;

        case QSGMII:
        case QSGMII_SUB:
            *mii = ENETPHY_MAC_MII_QSGMII;
            break;

        case XFI:
            status = ENET_EFAIL;
            EnetAppUtils_print("XFI is not supported by EnetPhy driver\n");
            EnetAppUtils_assert(BFALSE);
            break;

        default:
            status = ENET_EFAIL;
            EnetAppUtils_print("Invalid MAC mode: %u\n", macMode);
            EnetAppUtils_assert(BFALSE);
            break;
    }

    return status;
}

void EnetApp_macMode2MacMii(emac_mode macMode,
                            EnetMacPort_Interface *mii)
{
    switch (macMode)
    {
        case RMII:
            mii->layerType    = ENET_MAC_LAYER_MII;
            mii->sublayerType = ENET_MAC_SUBLAYER_REDUCED;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case RGMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_REDUCED;
            mii->variantType  = ENET_MAC_VARIANT_FORCED;
            break;

        case GMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_STANDARD;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case SGMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_SERIAL;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case QSGMII:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_QUAD_SERIAL_MAIN;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case QSGMII_SUB:
            mii->layerType    = ENET_MAC_LAYER_GMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_QUAD_SERIAL_SUB;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        case XFI:
            mii->layerType    = ENET_MAC_LAYER_XGMII;
            mii->sublayerType = ENET_MAC_SUBLAYER_SERIAL;
            mii->variantType  = ENET_MAC_VARIANT_NONE;
            break;

        default:
            EnetAppUtils_print("Invalid MAC mode: %u\n", macMode);
            EnetAppUtils_assert(BFALSE);
            break;
    }
}
