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
 * \file  enet_board.h
 *
 * \brief This file contains the type definitions and helper macros for the
 *        Enet example board utils interface.
 */

#ifndef ENET_BOARD_H_
#define ENET_BOARD_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/phy/enetphy.h>
#if defined(SOC_J721E)
#include <ti/drv/enet/examples/utils/V1/enet_board_j721e_evm.h>
#elif defined(SOC_J7200)
#include <ti/drv/enet/examples/utils/V2/enet_board_j7200_evm.h>
#elif defined(SOC_J721S2)
#include <ti/drv/enet/examples/utils/V3/enet_board_j721s2_evm.h>
#elif defined(SOC_J784S4)
#include <ti/drv/enet/examples/utils/V4/enet_board_j784s4_evm.h>
#elif defined(SOC_J742S2)
#include <ti/drv/enet/examples/utils/V5/enet_board_j742s2_evm.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*!
 * \brief Any board available.
 *
 * It can be used as a wildcard when looking for a port in any board present in
 * the system.
 */
#define ENETBOARD_ANY_ID                      (0xFFFFFFFFU)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/*!
 * \brief Board related configuration parameters of an Ethernet PHY.
 */
typedef struct EnetBoard_PhyCfg_s
{
    /*! PHY device address */
    uint32_t phyAddr;

    /*! Interface type */
    EnetPhy_Mii mii;

    /*! Whether PHY is strapped or not */
    bool isStrapped;

    /*! Whether to skip PHY-specific extended configuration */
    bool skipExtendedCfg;

    /*! Extended PHY-specific configuration */
    const void *extendedCfg;

    /*! Size of the extended configuration */
    uint32_t extendedCfgSize;
} EnetBoard_PhyCfg;

/*!
 * \brief Ethernet port configuration parameters.
 */
typedef struct EnetBoard_PortCfg_s
{
    /*! Peripheral type connected to */
    Enet_Type enetType;

    /*! Instance Id of the peripheral connected to */
    uint32_t instId;

    /*! MAC port connected to */
    Enet_MacPort macPort;

    /*! MAC port interface */
    EnetMacPort_Interface mii;

    /*! PHY configuration parameters */
    EnetBoard_PhyCfg phyCfg;

    /*! SGMII mode. Applicable only when port is used in Q/SGMII mode */
    EnetMac_SgmiiMode sgmiiMode;

    /*! Link configuration (speed and duplexity) */
    EnetMacPort_LinkCfg linkCfg;

    /*! Configuration flags indicating the board features/components that need
     *  to be setup */
    uint32_t flags;
} EnetBoard_PortCfg;

/*!
 * \brief Ethernet port.
 */
typedef struct EnetBoard_EthPort_s
{
    /*! MAC port connected to */
    Enet_MacPort macPort;

    /*! MAC port interface */
    EnetMacPort_Interface mii;

    /*! Board Id where port is physically present */
    uint32_t boardId;

    /*! EVM slot where the expansion board is connected to */
    uint32_t expPort;
} EnetBoard_EthPort;

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Get configuration info for the requested port.
 *
 * Gets the configuration information for a given port.  The PHY is found in
 * a list of PHYs available in the supported board(s).
 *
 * The search criteria includes peripheral (type and instance), MAC port that
 * the PHY is connected to and the MII interface type.  The search can be done
 * in one or multiple boards (bitmask) via \ref EnetBoard_EthPort::boardId.
 *
 * \param enetType  Peripheral type
 * \param instId    Instance number
 * \param ethPort   Ethernet port to get configuration for
 *
 * \return Pointer to the port configuration info.
 */
const EnetBoard_PortCfg *EnetBoard_getPortCfg(Enet_Type enetType,
                                              uint32_t instId,
                                              const EnetBoard_EthPort *ethPort);

/*!
 * \brief Initialize board configuration.
 *
 * \return \ref Enet_ErrorCodes
 */
int32_t EnetBoard_init(void);

/*!
 * \brief Deinitialize board configuration.
 */
void EnetBoard_deinit(void);

/*!
 * \brief Setup board configuration for a given set of ports.
 *
 * \param enetType        Peripheral type
 * \param instId          Instance number
 * \param ethPorts        Array of ports to be setup
 * \param numEthPorts     Size of the ports array
 *
 * \return \ref Enet_ErrorCodes
 */
int32_t EnetBoard_setupPorts(Enet_Type enetType,
                             uint32_t instId,
                             EnetBoard_EthPort *ethPorts,
                             uint32_t numEthPorts);

/*!
 * \brief Set ENET_CTRL register to the requested MII mode.
 *
 * \param enetType        Peripheral type
 * \param instId          Instance number
 * \param macPort         MAC port
 * \param mii             MII mode to be configured in ENET_CTLR register
 */
void EnetBoard_setEnetControl(Enet_Type enetType,
                              uint32_t instId,
                              Enet_MacPort macPort,
                              EnetMacPort_Interface *mii);

/*!
 * \brief Set ENET_CTRL register to the requested MII mode.
 *
 * \param enetType        Peripheral type
 * \param instId          Instance number
 * \param macAddr         MAC address array/pool
 * \param maxMacEntries   Max number of entries to populated in the pool
 *
 * \return Number of MAC addresses added
 */
uint32_t EnetBoard_getMacAddrList(Enet_Type enetType,
                                  uint32_t instId,
                                  uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                                  uint32_t maxMacEntries);

/* ========================================================================== */
/*                        Deprecated Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* ENET_BOARD_H_ */
