/******************************************************************************
 * Copyright (c) 2022 Texas Instruments Incorporated - http://www.ti.com
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
 *
 *****************************************************************************/

/**
 * \file   board_serdes_cfg.h
 *
 * \brief  Board serdes configurations header file.
 *
 * This file includes the structures, enums and register offsets
 * for configuring the serdes module.
 *
 */

#ifndef _BOARD_SERDES_CFG_H_
#define _BOARD_SERDES_CFG_H_

#include <stdio.h>

#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/cslr.h>
#include <ti/csl/csl_serdes.h>
#include <ti/csl/csl_serdes_ethernet.h>

#include <ti/board/board.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_SERDES_SGMII_INSTANCE       (CSL_TORRENT_SERDES2)

#define BOARD_SERDES_SGMII_ENET1_LANE_NUM       (2U)
#define BOARD_SERDES_SGMII_ENET1_LANE_COUNT     (1U)
#define BOARD_SERDES_SGMII_ENET1_LANE_MASK      (1 << BOARD_SERDES_SGMII_ENET1_LANE_NUM)

#define BOARD_SERDES_SGMII_ENET2_LANE_NUM       (3U)
#define BOARD_SERDES_SGMII_ENET2_LANE_COUNT     (1U)
#define BOARD_SERDES_SGMII_ENET2_LANE_MASK      (1 << BOARD_SERDES_SGMII_ENET2_LANE_NUM)

#define BOARD_SERDES_LANE_SELECT_CPSW           (1U)

extern CSL_SerdesStatus CSL_serdesConfigStatus (uint32_t baseAddr);

/**
 *  \brief serdes configurations for Sierra 1 in SGMII mode
 *
 *  The function configures the serdes module for SGMII instance
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgSgmii(void);

/**
 *  \brief serdes configurations for Sierra 1 in QSGMII mode
 *
 *  The function configures the serdes module for QSGMII instances
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgQsgmii(void);

/**
 *  \brief serdes configurations for USXGMII mode
 *
 *  The function configures the serdes module for USXGMII interface
 *
 * \return   BOARD_SOK in case of success or appropriate error code
 *
 */
Board_STATUS Board_serdesCfgUsxgmii(void);

/**
 *  \brief Query SerDes configuration status
 *
 *  The function gets the configuration status of Torrent SerDes module.
 *
 *  \retval TRUE    SerDes1 is configured
 *  \retval FALSE   SerDes1 is not configured
 */
bool Board_serdesCfgStatus(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BOARD_SERDES_CFG_H_ */
