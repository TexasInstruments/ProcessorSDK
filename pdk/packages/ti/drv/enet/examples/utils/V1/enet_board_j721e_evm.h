/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2021
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
 * \file  enet_board_j721e_evm.h
 *
 * \brief This file contains the definitions used for J721E EVM board.
 */

#ifndef ENET_BOARD_J721E_EVM_H_
#define ENET_BOARD_J721E_EVM_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*! \brief J7X Common Processor Board (CPB) id. */
#define ENETBOARD_CPB_ID                      (0U)

/*! \brief J7X Gateway/Ethernet Switch/Industrial Expansion (GESI) board id. */
#define ENETBOARD_GESI_ID                     (1U)

/*! \brief J7X Quad Port Eth Expansion (QpENet) board id. */
#define ENETBOARD_QPENET_ID                   (2U)

/*! \brief J7X SGMII board id. */
#define ENETBOARD_SGMII_ID                    (3U)

/*! \brief ETH expansion connector (SGMII bridge) - SGMII. */
#define ENETBOARD_BRIDGE_SGMII_ID             (4U)

/*! \brief ETH expansion connector (SGMII bridge) - XAUI. */
#define ENETBOARD_BRIDGE_XAUI_ID              (5U)

/*! \brief J7X dummy board, used for MAC loopback. */
#define ENETBOARD_LOOPBACK_ID                 (6U)

/*! \brief No expansion port (i.e. for baseboard). */
#define ENETBOARD_EXP_PORT_NONE               (0U)

/*! \brief ENET1 expansion port (QpENet, SGMII board, SGMII bridge). */
#define ENETBOARD_EXP_PORT_GESI               (1U)

/*! \brief ENET expansion port (QpENet, SGMII board, SGMII bridge). */
#define ENETBOARD_EXP_PORT_ENET               (2U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

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

#endif /* ENET_BOARD_J721E_EVM_H_ */
