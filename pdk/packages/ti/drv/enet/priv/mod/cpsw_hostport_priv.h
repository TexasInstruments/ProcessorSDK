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
 * \file  cpsw_hostport_priv.h
 *
 * \brief This file contains private type definitions and helper macros for the
 *        CPSW host port module which are meant for internal use in Enet Per
 *        drivers.
 */

#ifndef CPSW_HOSTPORT_PRIV_H_
#define CPSW_HOSTPORT_PRIV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/enet/include/core/enet_mod_hostport.h>
#include <ti/drv/enet/include/core/enet_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*! \brief Helper macro to create private IOCTL commands for CPSW MAC port module. */
#define CPSW_HOSTPORT_PRIVATE_IOCTL(x)         (ENET_IOCTL_TYPE_PRIVATE |  \
                                                ENET_IOCTL_HOSTPORT_BASE | \
                                                ENET_IOCTL_PER_CPSW |      \
                                                ENET_IOCTL_MIN(x))

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/*!
 * \brief Host port private IOCTL commands.
 */
typedef enum CpswHostPort_PrivIoctls_e
{
    /*!
     * \brief Set flow ID offset.
     *
     * Offset value which is added to the transmit (egress) flow Id.
     *
     * IOCTL parameters:
     *   inArgs: uint32_t
     *  outArgs: None
     */
    CPSW_HOSTPORT_SET_FLOW_ID_OFFSET = CPSW_HOSTPORT_PRIVATE_IOCTL(0U),
} CpswHostPort_PrivIoctls;

/*!
* \brief Structure to save context of HostPort
*/
typedef struct CpswHostPort_Ctxt_s
{
    /*! CPSW HostPort Control register value */
    uint32_t control;

    /*! CPSW HostPort Transmit FLOW ID Offset register value */
    uint32_t flowIdOffset;

    /*! CPSW HostPort Vlan register value */
    uint32_t portVlan;

    /*! CPSW HostPort Tx Header Priority to Switch Priority Map register value */
    uint32_t txPriMap;

    /*! CPSW HostPort Priority Control register value */
    uint32_t priCtl;

    /*! CPSW HostPort RX Paket Priority to Header Priority Map register value */
    uint32_t rxPriMap;

    /*! CPSW HostPort Receive Frame Max Length register value */
    uint32_t rxMaxlen;

    /*! CPSW HostPort Receive Packets Per Priority register value */
    uint32_t rxPktsPri;

    /*! CPSW HostPort Receive Gap register value */
    uint32_t rxGap;

    /*! CPSW HostPort Receive IPV4/IPV6 DSCP Map register value */
    uint32_t rxDscpMap[ENET_PRI_NUM];

    /*! CPSW HostPort Rx Priority Committed Information Rate register value */
    uint32_t priCir[ENET_PRI_NUM];

    /*! CPSW HostPort Rx Priority Excess Information Rate register value */
    uint32_t priEir[ENET_PRI_NUM];
} CpswHostPort_Ctxt;


/*!
 * \brief CPSW host port object.
 */
typedef struct CpswHostPort_Obj_s
{
    /*! EnetMod must be the first member */
    EnetMod_Obj enetMod;

    /*! Ethernet peripheral type. Required to query SoC parameters (clock freq) */
    Enet_Type enetType;

    /*! Peripheral instance number. Required to query SoC parameters (clock freq) */
    uint32_t instId;

    /*! HostPort context */
    CpswHostPort_Ctxt ctxt;
} CpswHostPort_Obj;

/*!
 * \brief Host port module handle.
 */
typedef CpswHostPort_Obj *CpswHostPort_Handle;

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Open and initialize CPSW host port.
 *
 * \param hMod      Enet Module handle
 * \param enetType  Enet Peripheral type
 * \param instId    Enet Peripheral instance id
 * \param cfg       Configuration parameters
 * \param cfgSize   Size of the configuration parameters
 *
 * \return \ref Enet_ErrorCodes
 */
int32_t CpswHostPort_open(EnetMod_Handle hMod,
                          Enet_Type enetType,
                          uint32_t instId,
                          const void *cfg,
                          uint32_t cfgSize);

/*!
 * \brief Rejoin a running CPSW host port.
 *
 * \param hMod      Enet Module handle
 * \param enetType  Enet Peripheral type
 * \param instId    Enet Peripheral instance id
 *
 * \return \ref Enet_ErrorCodes
 */
int32_t CpswHostPort_rejoin(EnetMod_Handle hMod,
                            Enet_Type enetType,
                            uint32_t instId);

/*!
 * \brief Run an IOCTL operation on CPSW host port.
 *
 * \param hMod         Enet Module handle
 * \param cmd          IOCTL command Id
 * \param prms         IOCTL parameters
 *
 * \return \ref Enet_ErrorCodes
 */
int32_t CpswHostPort_ioctl(EnetMod_Handle hMod,
                           uint32_t cmd,
                           Enet_IoctlPrms *prms);

/*!
 * \brief Close CPSW host port.
 *
 * \param hMod         Enet Module handle
 */
void CpswHostPort_close(EnetMod_Handle hMod);

/*!
 * \brief Save the context for CPSW HostPort.
 *
 * \param hMod         Enet Module handle
 */
void CpswHostPort_saveCtxt(EnetMod_Handle hMod);

/*!
 * \brief Restore the context for CPSW HostPort.
 *
 * \param hMod         Enet Module handle
 *
 * \return \ref Enet_ErrorCodes
 */
int32_t CpswHostPort_restoreCtxt(EnetMod_Handle hMod);

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

#endif /* CPSW_HOSTPORT_PRIV_H_ */
