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

#if (0U == APP_ENABLE_STATIC_CFG)
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
/* MCU or Main CPSW2G RGMII port in baseboard */
static EnetBoard_EthPort gEnetApp_RgmiiBaseBoard[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_CPB_ID,
        .expPort = ENETBOARD_EXP_PORT_NONE,
    },
};
#endif

#if defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
/* RGMII port in GESI board */
static EnetBoard_EthPort gEnetApp_GesiBoard[] =
{
    {
        .macPort = ENET_MAC_PORT_1,
        .mii     = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .boardId = ENETBOARD_GESI_ID,
        .expPort = ENETBOARD_EXP_PORT_GESI,
    },
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
static void EnetApp_getTestMacPort(EnetBoard_EthPort *ethPorts,
                                   uint32_t *macPortNum)
{
    uint32_t choice = 0U;
    bool retry;

    do
    {
        EnetAppUtils_print("Select MAC port\n");
        EnetAppUtils_print("0: ENET_MAC_PORT_1 - RGMII\n");
        retry = BFALSE;
        choice = EnetAppUtils_getNum();
        switch (choice)
        {
            case 0:
                memcpy(ethPorts, gEnetApp_RgmiiBaseBoard, sizeof(gEnetApp_RgmiiBaseBoard));
                *macPortNum = ENET_ARRAYSIZE(gEnetApp_RgmiiBaseBoard);
                break;

            default:
                EnetAppUtils_print("Wrong MAC port, enter again\n");
                retry = BTRUE;
                break;
        }
    }
    while (retry);
}
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
static void EnetApp_getTestMacPort(EnetBoard_EthPort *ethPorts,
                                   uint32_t *macPortNum)
{
    uint32_t choice = 0U;
    bool retry;

    do
    {
        EnetAppUtils_print("Select MAC port\n");
        EnetAppUtils_print("0: ENET_MAC_PORT_1 - RGMII\n");
        retry = BFALSE;
        choice = EnetAppUtils_getNum();
        switch (choice)
        {
            case 0:
                memcpy(ethPorts, gEnetApp_GesiBoard, sizeof(gEnetApp_GesiBoard));
                *macPortNum = ENET_ARRAYSIZE(gEnetApp_GesiBoard);
                break;

            default:
                EnetAppUtils_print("Wrong MAC port, enter again\n");
                retry = BTRUE;
                break;
        }
    }
    while (retry);
}
#endif

void EnetApp_getEstTestConfig(Enet_Type *enetType,
                              uint32_t *instId,
                              EnetBoard_EthPort *ethPorts,
                              uint32_t *macPortNum)
{
    uint32_t choice = 0U;
    bool retry;
    static const char enetTypeSetting[] =
    {
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
        "0: CPSW_2G\n"
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
        "0: Main CPSW_2G\n"
#endif
    };

    do
    {
        EnetAppUtils_print("Select peripheral type\n");
        EnetAppUtils_print("%s\n", enetTypeSetting);
        retry = BFALSE;

        choice = EnetAppUtils_getNum();
        switch(choice)
        {
            case 0:
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
                *enetType = ENET_CPSW_2G;
                *instId   = 0U;
                EnetApp_getTestMacPort(ethPorts, macPortNum);
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
                *enetType = ENET_CPSW_2G;
                *instId   = 1U;
                EnetApp_getTestMacPort(ethPorts, macPortNum);
#endif
                break;

default:
    EnetAppUtils_print("Wrong peripheral type, enter again\n");
    retry = BTRUE;
    break;
        }
    }
    while (retry);
}
#else /* (1U == APP_ENABLE_STATIC_CFG) */
void EnetApp_getEstTestConfig(Enet_Type *enetType,
                           uint32_t *instId,
                           EnetBoard_EthPort *ethPorts,
                           uint32_t *macPortNum)
{
#if defined(BUILD_MCU1_0) || defined(BUILD_MCU2_1)
    *enetType = ENET_CPSW_2G;
    *instId   = 0U;
    memcpy(ethPorts, gEnetApp_RgmiiBaseBoard, sizeof(gEnetApp_RgmiiBaseBoard));
    *macPortNum = ENET_ARRAYSIZE(gEnetApp_RgmiiBaseBoard);
#elif defined(BUILD_MCU2_0) || defined(BUILD_MCU3_0)
    *enetType = ENET_CPSW_2G;
    *instId   = 1U;
    memcpy(ethPorts, gEnetApp_GesiBoard, sizeof(gEnetApp_GesiBoard));
    *macPortNum = ENET_ARRAYSIZE(gEnetApp_GesiBoard);
#else
#error "lwIP example is not supported on this core"
#endif
}
#endif /* (0U == APP_ENABLE_STATIC_CFG) */
