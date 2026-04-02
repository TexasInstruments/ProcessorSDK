/**
 *  \file   emac_drv_v5.c
 *
 *  \brief  emac driver API implementation for AM6 for ICSS-G and CPSW
 *
 *  This file contains the driver APIs for EMAC peripheral for AM6
 */

/*
 * Copyright (C) 2018-2020 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <ti/drv/emac/emac_drv.h>
#include "ti/drv/emac/src/v5/emac_drv_v5.h"

/**********************************************************************
 *************************** EMAC Global Variables*********************
 **********************************************************************/
EMAC_MCB_V5_T      emac_mcb;

/* EMAC_v5 API functions */
static EMAC_DRV_ERR_E EMAC_open_v5(uint32_t port_num,  EMAC_OPEN_CONFIG_INFO_T *p_config);

static EMAC_DRV_ERR_E  EMAC_config_v5(uint32_t port_num,  EMAC_CONFIG_INFO_T *p_config);

static EMAC_DRV_ERR_E EMAC_close_v5(uint32_t port_num);

static EMAC_DRV_ERR_E EMAC_poll_pkt_v5(uint32_t port_num);


static EMAC_DRV_ERR_E EMAC_send_v5(uint32_t port_num, EMAC_PKT_DESC_T* p_desc);

static  EMAC_DRV_ERR_E EMAC_poll_v5(uint32_t port_num, EMAC_LINK_INFO_T* p_info);

static EMAC_DRV_ERR_E EMAC_get_stats_v5(uint32_t port_num, EMAC_STATISTICS_T*      p_stats);

static EMAC_DRV_ERR_E EMAC_get_stats_icssg_v5(uint32_t port_num, EMAC_STATISTICS_ICSSG_T *p_stats, bool clear);

static EMAC_DRV_ERR_E EMAC_ioctl_v5(uint32_t port_num, EMAC_IOCTL_CMD emacIoctlCmd, EMAC_IOCTL_PARAMS *emacIoctlParams);

static EMAC_DRV_ERR_E EMAC_poll_ctrl_v5(uint32_t port_num, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings);

/* EMAC function table for EMAC_v5 implementation */
const EMAC_FxnTable EMAC_FxnTable_v5 =
{
    EMAC_open_v5,
    EMAC_config_v5,
    EMAC_close_v5,
    EMAC_send_v5,
    EMAC_poll_v5,
    EMAC_get_stats_v5,
    EMAC_poll_pkt_v5,
    EMAC_get_stats_icssg_v5,
    EMAC_ioctl_v5, 
    EMAC_poll_ctrl_v5
};

extern EMAC_FxnTable EMAC_FxnTable_ICSSG_DM_v5;
extern EMAC_FxnTable EMAC_FxnTable_ICSSG_SWITCH_v5;
extern EMAC_FxnTable EMAC_FxnTable_CPSW_v5;

EMAC_PORT_INFO_T emac_port_info[] = {
     {EMAC_ICSSG0_INSTANCE, -1, 0, -1, 0, 0, 0, &EMAC_FxnTable_ICSSG_DM_v5},
     {EMAC_ICSSG0_INSTANCE, -1, 1, -1, 1, 1, 1, &EMAC_FxnTable_ICSSG_DM_v5},
     {EMAC_ICSSG1_INSTANCE, -1, 2, -1, 2, 2, 2, &EMAC_FxnTable_ICSSG_DM_v5},
     {EMAC_ICSSG1_INSTANCE, -1, 3, -1, 3, 3, 3, &EMAC_FxnTable_ICSSG_DM_v5},
     {EMAC_ICSSG2_INSTANCE, -1, 4, -1, 4, 4, 4, &EMAC_FxnTable_ICSSG_DM_v5},
     {EMAC_ICSSG2_INSTANCE, -1, 5, -1, 5, 5, 5, &EMAC_FxnTable_ICSSG_DM_v5},
     {-1,                   -1, 6, -1, 6, -1,6, &EMAC_FxnTable_CPSW_v5},

     {EMAC_ICSSG0_INSTANCE, EMAC_ICSSG0_SWITCH_PORT,  0, 1, -1, EMAC_ICSSG0_SWITCH_PORT,    EMAC_ICSSG0_SWITCH_PORT,  &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG0_INSTANCE, EMAC_ICSSG0_SWITCH_PORT0, 0, 1, -1, EMAC_ICSSG0_SWITCH_PORT0,  -1,                        &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG0_INSTANCE, EMAC_ICSSG0_SWITCH_PORT1, 0, 1,  0, 0,                          EMAC_ICSSG0_SWITCH_PORT1, &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG0_INSTANCE, EMAC_ICSSG0_SWITCH_PORT2, 0, 1,  1, 1,                          EMAC_ICSSG0_SWITCH_PORT2, &EMAC_FxnTable_ICSSG_SWITCH_v5},

     {EMAC_ICSSG1_INSTANCE, EMAC_ICSSG1_SWITCH_PORT,  2, 3, -1, EMAC_ICSSG1_SWITCH_PORT,    EMAC_ICSSG1_SWITCH_PORT,  &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG1_INSTANCE, EMAC_ICSSG1_SWITCH_PORT0, 2, 3, -1, EMAC_ICSSG1_SWITCH_PORT0,  -1,                        &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG1_INSTANCE, EMAC_ICSSG1_SWITCH_PORT1, 2, 3,  2, 2,                          EMAC_ICSSG1_SWITCH_PORT1, &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG1_INSTANCE, EMAC_ICSSG1_SWITCH_PORT2, 2, 3,  3, 3,                          EMAC_ICSSG1_SWITCH_PORT2, &EMAC_FxnTable_ICSSG_SWITCH_v5},

     {EMAC_ICSSG2_INSTANCE, EMAC_ICSSG2_SWITCH_PORT,  4, 5, -1, EMAC_ICSSG2_SWITCH_PORT,    EMAC_ICSSG2_SWITCH_PORT,  &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG2_INSTANCE, EMAC_ICSSG2_SWITCH_PORT0, 4, 5, -1, EMAC_ICSSG2_SWITCH_PORT0,  -1,                        &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG2_INSTANCE, EMAC_ICSSG2_SWITCH_PORT1, 4, 5,  4, 4,                          EMAC_ICSSG2_SWITCH_PORT1, &EMAC_FxnTable_ICSSG_SWITCH_v5},
     {EMAC_ICSSG2_INSTANCE, EMAC_ICSSG2_SWITCH_PORT2, 4, 5,  5, 5,                          EMAC_ICSSG2_SWITCH_PORT2, &EMAC_FxnTable_ICSSG_SWITCH_v5},
};

static inline EMAC_DRV_ERR_E emac_is_port_open(uint32_t port_num)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_ERR_PORT_CLOSED;
    uint32_t portLoc = emac_port_info[port_num].portNum1;
    if (emac_mcb.port_cb[portLoc].emacState == EMAC_PORT_STATE_OPEN)
    {
        retVal = EMAC_DRV_RESULT_OK;
    }
    return retVal;
}

/*
 *  ======== EMAC_open_v5 ========
 */
static EMAC_DRV_ERR_E  EMAC_open_v5(uint32_t port_num,  EMAC_OPEN_CONFIG_INFO_T *p_config)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;

    if (port_num < EMAC_PORT_MAX)
    {
        retVal = emac_port_info[port_num].fxnTable->openFxn(port_num, p_config);
    }

    return retVal;
}

/*
 *  ======== EMAC_config_v5 ========
 */
static EMAC_DRV_ERR_E  EMAC_config_v5(uint32_t port_num,  EMAC_CONFIG_INFO_T *p_config)
{
    return EMAC_DRV_RESULT_OK;
}

/*
 *  ======== EMAC_close_v5 ========
 */
static EMAC_DRV_ERR_E  EMAC_close_v5(uint32_t port_num)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;

    if (port_num < EMAC_PORT_MAX)
    {
        retVal = emac_port_info[port_num].fxnTable->closeFxn(port_num);
    }

    return retVal;
}

/*
 *  ======== EMAC_get_stats_v5 ========
 */
static EMAC_DRV_ERR_E EMAC_get_stats_v5(uint32_t port_num, EMAC_STATISTICS_T*      p_stats)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;

    if (port_num == EMAC_CPSW_PORT1)
    {
        if ((retVal = emac_is_port_open(port_num)) == EMAC_DRV_RESULT_OK)
        {
            retVal = emac_port_info[port_num].fxnTable->getStatsFxn(port_num, p_stats);
        }
    }
    return retVal;
}

/*
 *  ======== EMAC_get_stats_icssg_v5 ========
 */
static EMAC_DRV_ERR_E EMAC_get_stats_icssg_v5(uint32_t port_num, EMAC_STATISTICS_ICSSG_T *p_stats, bool clear)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;

    if (port_num < EMAC_PORT_MAX)
    {
        if ((retVal = emac_is_port_open(port_num)) == EMAC_DRV_RESULT_OK)
        {
            retVal = emac_port_info[port_num].fxnTable->getStatsIcssgFxn(port_num, p_stats,clear);
        }
    }
    return retVal;
}

/*

 */
static EMAC_DRV_ERR_E EMAC_send_v5(uint32_t port_num, EMAC_PKT_DESC_T* p_desc)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;

    if (port_num < EMAC_PORT_MAX)
    {
        if ((retVal = emac_is_port_open(port_num)) == EMAC_DRV_RESULT_OK)
        {
            retVal = emac_port_info[port_num].fxnTable->sendFxn(emac_port_info[port_num].sendPortNum, p_desc);
        }
    }
    return retVal;
}

/*
 *  ======== EMAC_poll_v5 ========
 */
static  EMAC_DRV_ERR_E EMAC_poll_v5(uint32_t port_num, EMAC_LINK_INFO_T* p_info)
{

    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;

    if (port_num < EMAC_PORT_MAX)
    {
        if ((retVal = emac_is_port_open(port_num)) == EMAC_DRV_RESULT_OK)
        {
            retVal = emac_port_info[port_num].fxnTable->pollFxn(emac_port_info[port_num].pollPortNum, p_info);
        }
    }
    return retVal;
}

/*
 *  ======== EMAC_poll_pkt_v5 ========
 */
static EMAC_DRV_ERR_E EMAC_poll_pkt_v5(uint32_t port_num)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;

    if (port_num < EMAC_PORT_MAX)
    {
        if ((retVal = emac_is_port_open(port_num)) == EMAC_DRV_RESULT_OK)
        {
            retVal = emac_port_info[port_num].fxnTable->pollPktFxn(emac_port_info[port_num].pollPortNum);
        }
    }
    return retVal;
}

/*
 *  ======== EMAC_ioctl_v5 ========
 */
static EMAC_DRV_ERR_E EMAC_ioctl_v5(uint32_t port_num, EMAC_IOCTL_CMD emacIoctlCmd, EMAC_IOCTL_PARAMS *pEmacIoctlParams)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;
    if (port_num < EMAC_PORT_MAX)
    {
        if ((retVal = emac_is_port_open(port_num)) == EMAC_DRV_RESULT_OK)
        {
            retVal = emac_port_info[port_num].fxnTable->ioctlFxn(emac_port_info[port_num].ioctlPortNum, emacIoctlCmd, pEmacIoctlParams);
        }
    }
    return retVal;
}

/*
 *  ======== EMAC_poll_ctrl_v5 ========
 */
static EMAC_DRV_ERR_E EMAC_poll_ctrl_v5(uint32_t port_num, uint32_t rxPktRings, uint32_t rxMgmtRings, uint32_t txRings)
{
    EMAC_DRV_ERR_E retVal = EMAC_DRV_RESULT_INVALID_PORT;
    if (port_num < EMAC_PORT_MAX)
    {
        if ((retVal = emac_is_port_open(port_num)) == EMAC_DRV_RESULT_OK)
        {
            retVal = emac_port_info[port_num].fxnTable->pollCtrl(emac_port_info[port_num].pollPortNum, rxPktRings, rxMgmtRings,txRings);
        }
    }
    return retVal;
}

/* Nothing past this point */

