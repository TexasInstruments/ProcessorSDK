/*
 *  Copyright (c) Texas Instruments Incorporated 2022
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
 * \file  test_config_j721s2.c
 *
 * \brief This file contains J721S2-specific test configuration parameters.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <ti/board/board.h>
#include <ti/board/src/j721s2_evm/include/board_ethernet_config.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_board.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Enable selection of test parameters:
 *   0 - dynamic (runtime via menu)
 *   1 - static (compile time)
 */
#define APP_ENABLE_STATIC_CFG                      (0U)

/* Helper macro used to create loopback port menu options */
#define ENETLPBK_PORT_OPT(macPort, macMode, boardId, expPort) { #macPort " - "  #macMode, \
                                                                (macPort), \
                                                                (macMode), \
                                                                (boardId), \
                                                                (expPort) }

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct EnetLpbk_EnetTypeMenu_s
{
    const char *text;
    Enet_Type enetType;
    uint32_t instId;
} EnetLpbk_EnetTypeMenu;

typedef struct EnetLpbk_PortMenu_s
{
    const char *text;
    Enet_MacPort macPort;
    emac_mode macMode;
    uint32_t boardId;
    uint32_t expPort;
} EnetLpbk_PortMenu;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

#if (0U == APP_ENABLE_STATIC_CFG)
static void EnetLpbk_showMenu(Enet_Type *enetType,
                              uint32_t *instId,
                              bool *testPhyLoopback,
                              Enet_MacPort *macPort,
                              emac_mode *macMode,
                              uint32_t *boardId,
                              uint32_t *expPort);

static void EnetLpbk_showEnetTypeMenu(Enet_Type *enetType,
                                      uint32_t *instId);

static void EnetLpbk_showLpbkMenu(bool *testPhyLoopback);

static int32_t EnetLpbk_showPortMenu(Enet_Type enetType,
                                     uint32_t instId,
                                     bool testPhyLoopback,
                                     Enet_MacPort *macPort,
                                     emac_mode *macMode,
                                     uint32_t *boardId,
                                     uint32_t *expPort);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if (0U == APP_ENABLE_STATIC_CFG)
static EnetLpbk_EnetTypeMenu gEnetLpbk_EnetTypeMenu[] =
{
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1) || defined(BUILD_MPU1_0)
    { "CPSW 2G", ENET_CPSW_2G, 0U },
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    { "CPSW 2G MAIN", ENET_CPSW_2G, 1U },
#endif
};

static EnetLpbk_PortMenu gEnetLpbk_MacLpbkMenu[] =
{
    ENETLPBK_PORT_OPT(ENET_MAC_PORT_1, RGMII, ENETBOARD_LOOPBACK_ID, ENETBOARD_EXP_PORT_NONE),
};

static EnetLpbk_PortMenu gEnetLpbk_cpsw2gPhyLpbkMenu[] =
{
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1) || defined(BUILD_MPU1_0)
    ENETLPBK_PORT_OPT(ENET_MAC_PORT_1, RGMII, ENETBOARD_CPB_ID, ENETBOARD_EXP_PORT_NONE),
#endif
};

static EnetLpbk_PortMenu gEnetLpbk_cpsw2gMainPhyLpbkMenu[] =
{
#if defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    ENETLPBK_PORT_OPT(ENET_MAC_PORT_1, RGMII, ENETBOARD_GESI_ID, ENETBOARD_EXP_PORT_GESI),
#endif
};
#endif /* (0U == APP_ENABLE_STATIC_CFG) */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void EnetLpbk_getTestConfig(Enet_Type *enetType,
                            uint32_t *instId,
                            bool *testPhyLoopback,
                            Enet_MacPort *macPort,
                            emac_mode *macMode,
                            uint32_t *boardId,
                            uint32_t *expPort)
{
#if (1U == APP_ENABLE_STATIC_CFG)
    *testPhyLoopback = BTRUE;
#if defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    *enetType = ENET_CPSW_2G;
    *instId   = 1U;
    *boardId  = ENETBOARD_GESI_ID;
    *expPort  = ENETBOARD_EXP_PORT_GESI;
    *macPort  = ENET_MAC_PORT_1;
    *macMode  = RGMII;
#else
    *enetType = ENET_CPSW_2G;
    *instId   = 0U;
    *boardId  = ENETBOARD_CPB_ID;
    *expPort  = ENETBOARD_EXP_PORT_NONE;
    *macPort  = ENET_MAC_PORT_1;
    *macMode  = RGMII;
#endif
#else
    EnetLpbk_showMenu(enetType, instId, testPhyLoopback, macPort, macMode, boardId, expPort);
#endif
}

#if (0U == APP_ENABLE_STATIC_CFG)
static void EnetLpbk_showMenu(Enet_Type *enetType,
                              uint32_t *instId,
                              bool *testPhyLoopback,
                              Enet_MacPort *macPort,
                              emac_mode *macMode,
                              uint32_t *boardId,
                              uint32_t *expPort)
{
    int32_t status;

    do
    {
        EnetLpbk_showEnetTypeMenu(enetType, instId);

        EnetLpbk_showLpbkMenu(testPhyLoopback);

        status = EnetLpbk_showPortMenu(*enetType, *instId, *testPhyLoopback, macPort, macMode, boardId, expPort);

        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Try again...\n\n");
        }
    }
    while (status != ENET_SOK);
}

static void EnetLpbk_showEnetTypeMenu(Enet_Type *enetType,
                                      uint32_t *instId)
{
    bool retry;
    int32_t choice;
    uint32_t i;

    do
    {
        EnetAppUtils_print("Select Enet peripheral type:\n");
        for (i = 0U; i < ENET_ARRAYSIZE(gEnetLpbk_EnetTypeMenu); i++)
        {
            EnetAppUtils_print("%u: %s\n", i, gEnetLpbk_EnetTypeMenu[i].text);
        }

        choice = EnetAppUtils_getNum();

        if ((choice >= 0) &&
            (choice < ENET_ARRAYSIZE(gEnetLpbk_EnetTypeMenu)))
        {
            *enetType = gEnetLpbk_EnetTypeMenu[choice].enetType;
            *instId = gEnetLpbk_EnetTypeMenu[choice].instId;
            retry = BFALSE;
        }
        else
        {
            EnetAppUtils_print("Wrong option, try again...\n\n");
            retry = BTRUE;
        }
    }
    while (retry);

}

static void EnetLpbk_showLpbkMenu(bool *testPhyLoopback)
{
    bool retry;
    int32_t choice;

    do
    {
        EnetAppUtils_print("Select loopback type:\n");
        EnetAppUtils_print("0: Internal (MAC loopback)\n");
        EnetAppUtils_print("1: External (PHY loopback)\n");

        choice = EnetAppUtils_getNum();

        switch (choice)
        {
            case 0:
                *testPhyLoopback = BFALSE;
                retry = BFALSE;
                break;
            case 1:
                *testPhyLoopback = BTRUE;
                retry = BFALSE;
                break;
            default:
                EnetAppUtils_print("Wrong option, try again...\n\n");
                retry = BTRUE;
                break;
        }

    }
    while (retry);
}

static int32_t EnetLpbk_showPortMenu(Enet_Type enetType,
                                     uint32_t instId,
                                     bool testPhyLoopback,
                                     Enet_MacPort *macPort,
                                     emac_mode *macMode,
                                     uint32_t *boardId,
                                     uint32_t *expPort)
{
    EnetLpbk_PortMenu *portMenu = NULL;
    uint32_t portMenuLen = 0U;
    uint32_t i;
    int32_t choice;
    bool retry;
    int32_t status = ENET_SOK;

    if (testPhyLoopback)
    {
        if (enetType == ENET_CPSW_2G)
        {
            if (instId == 0U)
            {
                portMenu = gEnetLpbk_cpsw2gPhyLpbkMenu;
                portMenuLen = ENET_ARRAYSIZE(gEnetLpbk_cpsw2gPhyLpbkMenu);
            }
            else if (instId == 1U)
            {
                portMenu = gEnetLpbk_cpsw2gMainPhyLpbkMenu;
                portMenuLen = ENET_ARRAYSIZE(gEnetLpbk_cpsw2gMainPhyLpbkMenu);
            }
        }
    }
    else
    {
        portMenu = gEnetLpbk_MacLpbkMenu;
        portMenuLen = ENET_ARRAYSIZE(gEnetLpbk_MacLpbkMenu);
    }

    if ((portMenuLen == 0U) && (portMenu == NULL))
    {
        EnetAppUtils_print("Ethernet periperhal not supported\n");
        status = ENET_EINVALIDPARAMS;
    }
    else if ((portMenuLen == 0U) && (portMenu != NULL))
    {
        EnetAppUtils_print("No ports supported on current core\n");
        status = ENET_EINVALIDPARAMS;
    }
    else
    {
        do
        {
            EnetAppUtils_print("Select MAC port:\n");
            for (i = 0U; i < portMenuLen; i++)
            {
                EnetAppUtils_print("%u: %s\n", i, portMenu[i].text);
            }

            choice = EnetAppUtils_getNum();

            if ((choice >= 0) &&
                (choice < portMenuLen))
            {
                *macPort = portMenu[choice].macPort;
                *macMode = portMenu[choice].macMode;
                *boardId = portMenu[choice].boardId;
                *expPort = portMenu[choice].expPort;
                retry = BFALSE;
            }
            else
            {
                EnetAppUtils_print("Wrong option, try again...\n\n");
                retry = BTRUE;
            }
        }
        while (retry);
    }

    return status;
}
#endif

int32_t EnetLpbk_macMode2PhyMii(emac_mode macMode,
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

void EnetLpbk_macMode2MacMii(emac_mode macMode,
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
