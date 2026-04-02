/*
 *  Copyright (c) Texas Instruments Incorporated 2022-2025
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

/**
 *  \file udma_rmcfg.c
 *
 *  \brief File containing the UDMA driver default RM configuration used to
 *  initialize the RM init parameters passed during driver init.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/src/udma_priv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU1_0 50U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU1_0 4U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU1_0 2U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU1_1 16U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU1_1 1U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU1_1 2U

#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU2_0 50U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU2_0 8U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU2_0 6U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU2_1 50U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU2_1 2U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU2_1 4U

#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU3_0 50U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU3_0 4U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU3_0 6U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU3_1 50U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU3_1 2U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU3_1 4U

#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU4_0 50U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU4_0 2U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU4_0 6U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU4_1 50U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU4_1 2U
#define UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU4_1 4U

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

/** \brief BCDMA defaultBoardCfg Params when requested by Main domain core. */
const Udma_RmDefBoardCfgPrms gUdmaRmDefBoardCfg_MainCore_Bcdma[UDMA_RM_NUM_BCDMA_RES] =
{
    /* resId,                               reqType,                           reqSubtype,                                  secHost */
    {UDMA_RM_RES_ID_BC_UHC,                 UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC_HC,                  UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC,                     UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_UHC,                 UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_HC,                  UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX,                     TISCI_DEV_NAVSS0_BCDMA_0,          TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_TX_CHAN, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_UHC,                 UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_HC,                  UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX,                     TISCI_DEV_NAVSS0_BCDMA_0,          TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_RX_CHAN, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_GLOBAL_EVENT,           TISCI_DEV_NAVSS0_UDMASS_INTA_0,     TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT,    TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_VINTR,                  TISCI_DEV_NAVSS0_UDMASS_INTA_0,     TISCI_RESASG_SUBTYPE_IA_VINT,              TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_IR_INTR,                TISCI_DEV_NAVSS0_INTR_0,            TISCI_RESASG_SUBTYPE_IR_OUTPUT,            TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
};

#if defined (BUILD_MCU)
/** \brief BCDMA defaultBoardCfg Params when requested by MCU domain core. */
const Udma_RmDefBoardCfgPrms gUdmaRmDefBoardCfg_McuCore_Bcdma[UDMA_RM_NUM_BCDMA_RES] =
{
    /* resId,                               reqType,                           reqSubtype,                                  secHost */
    {UDMA_RM_RES_ID_BC_UHC,                 UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC_HC,                  UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC,                     UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_UHC,                 UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_HC,                  UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX,                     TISCI_DEV_NAVSS0_BCDMA_0,          TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_TX_CHAN, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_UHC,                 UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_HC,                  UDMA_RM_SCI_REQ_TYPE_INVALID,      UDMA_RM_SCI_REQ_SUBTYPE_INVALID,             TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX,                     TISCI_DEV_NAVSS0_BCDMA_0,          TISCI_RESASG_SUBTYPE_BCDMA_SPLIT_TR_RX_CHAN, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_GLOBAL_EVENT,           TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT,    TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_VINTR,                  TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_VINT,              TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_IR_INTR,                TISCI_DEV_MCU_NAVSS0_INTR_ROUTER_0, TISCI_RESASG_SUBTYPE_IR_OUTPUT,            TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
};
#endif

/** \brief Main Navss defaultBoardCfg Params when requested by Main domain core. */
const Udma_RmDefBoardCfgPrms gUdmaRmDefBoardCfg_MainCore_MainNavss[UDMA_RM_DEFAULT_BOARDCFG_NUM_RES] =
{
    /* resId,                     reqType,                            reqSubtype,                               secHost */
    {UDMA_RM_RES_ID_BC_UHC,       UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC_HC,        UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC,           UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_UHC,       TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_HC,        TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX,           TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_UHC,       TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_HC,        TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX,           TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_GLOBAL_EVENT, TISCI_DEV_NAVSS0_UDMASS_INTA_0,     TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT,    TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_VINTR,        TISCI_DEV_NAVSS0_UDMASS_INTA_0,     TISCI_RESASG_SUBTYPE_IA_VINT,              TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_IR_INTR,      TISCI_DEV_NAVSS0_INTR_0,            TISCI_RESASG_SUBTYPE_IR_OUTPUT,            TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_UTC,          TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_FLOW,      TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING,         TISCI_DEV_NAVSS0_RINGACC_0,          TISCI_RESASG_SUBTYPE_RA_GP,                TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_PROXY,        TISCI_DEV_NAVSS0_PROXY_0,            TISCI_RESASG_SUBTYPE_PROXY_PROXIES,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING_MON,     TISCI_DEV_NAVSS0_RINGACC_0,          TISCI_RESASG_SUBTYPE_RA_MONITORS,          TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST}
};

#if defined (BUILD_MCU)
/** \brief Main Navss defaultBoardCfg Params when requested by MCU domain core. */
const Udma_RmDefBoardCfgPrms gUdmaRmDefBoardCfg_McuCore_MainNavss[UDMA_RM_DEFAULT_BOARDCFG_NUM_RES] =
{
    /* resId,                     reqType,                            reqSubtype,                               secHost */
    {UDMA_RM_RES_ID_BC_UHC,       UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC_HC,        UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC,           UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_UHC,       TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_HC,        TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX,           TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_UHC,       TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_HC,        TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX,           TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_GLOBAL_EVENT, TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT,    TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_VINTR,        TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_VINT,              TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_IR_INTR,      TISCI_DEV_MCU_NAVSS0_INTR_ROUTER_0, TISCI_RESASG_SUBTYPE_IR_OUTPUT,            TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_UTC,          TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_FLOW,      TISCI_DEV_NAVSS0_UDMAP_0,            TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING,         TISCI_DEV_NAVSS0_RINGACC_0,          TISCI_RESASG_SUBTYPE_RA_GP,                TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_PROXY,        TISCI_DEV_NAVSS0_PROXY_0,            TISCI_RESASG_SUBTYPE_PROXY_PROXIES,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING_MON,     TISCI_DEV_NAVSS0_RINGACC_0,          TISCI_RESASG_SUBTYPE_RA_MONITORS,          TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST}
};
#endif

/** \brief MCU Navss defaultBoardCfg Params when requested by Main domain core. */
const Udma_RmDefBoardCfgPrms gUdmaRmDefBoardCfg_MainCore_McuNavss[UDMA_RM_DEFAULT_BOARDCFG_NUM_RES] =
{
    /* resId,                     reqType,                            reqSubtype,                               secHost */
    {UDMA_RM_RES_ID_BC_UHC,       UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC_HC,        UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC,           UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_UHC,       TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_HC,        TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX,           TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_UHC,       TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_HC,        TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX,           TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_GLOBAL_EVENT, TISCI_DEV_NAVSS0_UDMASS_INTA_0,     TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT,    TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_VINTR,        TISCI_DEV_NAVSS0_UDMASS_INTA_0,     TISCI_RESASG_SUBTYPE_IA_VINT,              TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_IR_INTR,      TISCI_DEV_NAVSS0_INTR_0,            TISCI_RESASG_SUBTYPE_IR_OUTPUT,            TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_UTC,          TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_FLOW,      TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING,         TISCI_DEV_MCU_NAVSS0_RINGACC0,      TISCI_RESASG_SUBTYPE_RA_GP,                TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_PROXY,        TISCI_DEV_MCU_NAVSS0_PROXY0,        TISCI_RESASG_SUBTYPE_PROXY_PROXIES,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING_MON,     TISCI_DEV_MCU_NAVSS0_RINGACC0,      TISCI_RESASG_SUBTYPE_RA_MONITORS,          TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST}
};

#if defined (BUILD_MCU)
/** \brief MCU Navss defaultBoardCfg Params when requested by MCU domain core. */
const Udma_RmDefBoardCfgPrms gUdmaRmDefBoardCfg_McuCore_McuNavss[UDMA_RM_DEFAULT_BOARDCFG_NUM_RES] =
{
    /* resId,                     reqType,                            reqSubtype,                               secHost */
    {UDMA_RM_RES_ID_BC_UHC,       UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC_HC,        UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_BC,           UDMA_RM_SCI_REQ_TYPE_INVALID,        UDMA_RM_SCI_REQ_SUBTYPE_INVALID,           TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_UHC,       TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX_HC,        TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_TX,           TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_UHC,       TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_UHCHAN,      TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_HC,        TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_HCHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX,           TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_CHAN,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_GLOBAL_EVENT, TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_GLOBAL_EVENT_SEVT,    TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_VINTR,        TISCI_DEV_MCU_NAVSS0_UDMASS_INTA_0, TISCI_RESASG_SUBTYPE_IA_VINT,              TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_IR_INTR,      TISCI_DEV_MCU_NAVSS0_INTR_ROUTER_0, TISCI_RESASG_SUBTYPE_IR_OUTPUT,            TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_UTC,          TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_TX_ECHAN,       TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RX_FLOW,      TISCI_DEV_MCU_NAVSS0_UDMAP_0,        TISCI_RESASG_SUBTYPE_UDMAP_RX_FLOW_COMMON, TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING,         TISCI_DEV_MCU_NAVSS0_RINGACC0,      TISCI_RESASG_SUBTYPE_RA_GP,                TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_PROXY,        TISCI_DEV_MCU_NAVSS0_PROXY0,        TISCI_RESASG_SUBTYPE_PROXY_PROXIES,        TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST},
    {UDMA_RM_RES_ID_RING_MON,     TISCI_DEV_MCU_NAVSS0_RINGACC0,      TISCI_RESASG_SUBTYPE_RA_MONITORS,          TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST}
};
#endif

/**
  * Static sharing policy to share flows between different hosts.
  * RM Board Config does not take ownership of sharing of flows among cores.
  * UDMA driver need to define the sharing policy of these flows among cores.
*/
uint32_t gFlowInstShare[UDMA_NUM_CORE] =
{
    0U,                            /* MPU1_0  - Reserved in BoardCfg*/
    UDMA_RM_SHARED_RES_CNT_REST,   /* MCU2_0 */
    10U,                           /* MCU2_1 */
    8U,                            /* MCU3_0 */
    0U,                            /* MCU3_1 */
    0U,                            /* MCU4_0 */
    0U,                            /* MCU4_1 */
    0U,                            /* C7X_1 */
    0U,                            /* C7X_2 */
    0U,                            /* C7X_3 */
    0U,                            /* C7X_4 */
    0U,                            /* MCU1_0 */
    0U                             /* MCU1_1 */
};

#if defined (BUILD_MPU1_0)
    /*          instShare[              MAIN_NAVSS,                     MCU_NAVSS,              BCDMA] */
    uint32_t gEvtInstShare[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_MIN};
    uint32_t gVintInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_MIN};
    uint32_t gIntrInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_MIN};
#endif
#if defined (BUILD_MCU)
    /*          instShare[              MAIN_NAVSS,                     MCU_NAVSS,              BCDMA] */
    uint32_t gEvtInstShare_Main[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gVintInstShare_Main[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gIntrInstShare_Main[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};

    uint32_t gEvtInstShare_Mcu[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_REST, 0};
    uint32_t gVintInstShare_Mcu[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_REST, 0};
    uint32_t gIntrInstShare_Mcu[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_REST, 0};

    uint32_t gEvtInstShare2_0[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_REST};
    uint32_t gVintInstShare2_0[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_REST};
    uint32_t gIntrInstShare2_0[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_REST};  
#endif
#if defined (BUILD_C7X_1)
    /*          instShare[              MAIN_NAVSS,                     MCU_NAVSS,              BCDMA] */
    uint32_t gEvtInstShare[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gVintInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gIntrInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
#endif
#if defined (BUILD_C7X_2)
    /*          instShare[              MAIN_NAVSS,                     MCU_NAVSS,              BCDMA] */
    uint32_t gEvtInstShare[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gVintInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gIntrInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
#endif
#if defined (BUILD_C7X_3)
    /*          instShare[              MAIN_NAVSS,                     MCU_NAVSS,              BCDMA] */
    uint32_t gEvtInstShare[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gVintInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gIntrInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
#endif
#if defined (BUILD_C7X_4)
    /*          instShare[              MAIN_NAVSS,                     MCU_NAVSS,              BCDMA] */
    uint32_t gEvtInstShare[UDMA_NUM_INST_ID]  = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gVintInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
    uint32_t gIntrInstShare[UDMA_NUM_INST_ID] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN, 0};
#endif

/* These DRUs are local to C7X cores, user need to take care of resource overlapping when they try to override default allocation */
#if defined (BUILD_C7X)
    /*                         instShare[   C7X_1,                        C7X_2,                      C7X_3,                      C7X_4         ] */
    uint32_t gDru4InstShare[UDMA_NUM_C7X_CORE] = {UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_MIN};
    uint32_t gDru5InstShare[UDMA_NUM_C7X_CORE] = {UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_MIN};
    uint32_t gDru6InstShare[UDMA_NUM_C7X_CORE] = {UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_REST, UDMA_RM_SHARED_RES_CNT_MIN};
    uint32_t gDru7InstShare[UDMA_NUM_C7X_CORE] = {UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_MIN,  UDMA_RM_SHARED_RES_CNT_MIN, UDMA_RM_SHARED_RES_CNT_REST};
#endif

/** \brief Shared resource Params */
Udma_RmSharedResPrms gUdmaRmSharedResPrms[UDMA_RM_NUM_SHARED_RES] =
{
    /* MAIN NAVSS RX Free Flows are assigned to HOST_ID_ALL and some cores dosen't have core specific reservation */
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare */
    {UDMA_RM_RES_ID_RX_FLOW,      0U,            0U,          UDMA_NUM_CORE,     0U,     gFlowInstShare},
    /* Global Events/VINTR/IN INTR must be used based on core and split across MCU and MAIN NAVSS instances */
#if defined (BUILD_MPU1_0)
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare*/
    {UDMA_RM_RES_ID_GLOBAL_EVENT, 0U,            0U,          UDMA_NUM_INST_ID,  50U,    gEvtInstShare },
    {UDMA_RM_RES_ID_VINTR,        0U,            0U,          UDMA_NUM_INST_ID,  4U,     gVintInstShare},
    {UDMA_RM_RES_ID_IR_INTR,      0U,            6U,          UDMA_NUM_INST_ID,  4U,     gIntrInstShare},
#endif
#if defined (BUILD_MCU)
    /* Placeholder values for minReq. These will be modified to each core's
       corresponding value in Udma_rmGetSharedResPrms(). 
       instShare also will be changed to the corresponding MCU domain values
       in Udma_rmGetSharedResPrms() if required. */
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare */
    {UDMA_RM_RES_ID_GLOBAL_EVENT, 0U,            0U,          UDMA_NUM_INST_ID,  50U,    gEvtInstShare_Main },
    {UDMA_RM_RES_ID_VINTR,        0U,            0U,          UDMA_NUM_INST_ID,  8U,     gVintInstShare_Main},
    {UDMA_RM_RES_ID_IR_INTR,      0U,            6U,          UDMA_NUM_INST_ID,  6U,     gIntrInstShare_Main},
#endif
#if defined (BUILD_C7X_1)
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare*/
    {UDMA_RM_RES_ID_GLOBAL_EVENT, 0U,            0U,          UDMA_NUM_INST_ID,  50U,    gEvtInstShare },
    {UDMA_RM_RES_ID_VINTR,        0U,            0U,          UDMA_NUM_INST_ID,  3U,     gVintInstShare},
    {UDMA_RM_RES_ID_IR_INTR,      0U,            6U,          UDMA_NUM_INST_ID,  4U,     gIntrInstShare},
#endif
#if defined (BUILD_C7X_2)
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare*/
    {UDMA_RM_RES_ID_GLOBAL_EVENT, 0U,            0U,          UDMA_NUM_INST_ID,  50U,    gEvtInstShare },
    {UDMA_RM_RES_ID_VINTR,        0U,            0U,          UDMA_NUM_INST_ID,  3U,     gVintInstShare},
    {UDMA_RM_RES_ID_IR_INTR,      0U,            6U,          UDMA_NUM_INST_ID,  4U,     gIntrInstShare},
#endif
#if defined (BUILD_C7X_3)
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare */
    {UDMA_RM_RES_ID_GLOBAL_EVENT, 0U,            0U,          UDMA_NUM_INST_ID,  50U,    gEvtInstShare },
    {UDMA_RM_RES_ID_VINTR,        0U,            0U,          UDMA_NUM_INST_ID,  3U,     gVintInstShare},
    {UDMA_RM_RES_ID_IR_INTR,      0U,            6U,          UDMA_NUM_INST_ID,  2U,     gIntrInstShare},
#endif
#if defined (BUILD_C7X_4)
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare*/
    {UDMA_RM_RES_ID_GLOBAL_EVENT, 0U,            0U,          UDMA_NUM_INST_ID,  50U,    gEvtInstShare },
    {UDMA_RM_RES_ID_VINTR,        0U,            0U,          UDMA_NUM_INST_ID,  3U,     gVintInstShare},
    {UDMA_RM_RES_ID_IR_INTR,      0U,            6U,          UDMA_NUM_INST_ID,  2U,     gIntrInstShare},
#endif
/* These DRUs are local to C7X cores, user need to take care of resource overlapping when they try to override default allocation */
#if defined (BUILD_C7X)
    /* resId,                     startResrvCnt, endResrvCnt, numInst,           minReq, instShare */
    {UDMA_RM_C7X_MSMC_DRU4,       0U,            0U,          UDMA_NUM_C7X_CORE,     4U, gDru4InstShare},
    {UDMA_RM_C7X_MSMC_DRU5,       0U,            0U,          UDMA_NUM_C7X_CORE,     4U, gDru5InstShare},
    {UDMA_RM_C7X_MSMC_DRU6,       0U,            0U,          UDMA_NUM_C7X_CORE,     4U, gDru6InstShare},
    {UDMA_RM_C7X_MSMC_DRU7,       0U,            0U,          UDMA_NUM_C7X_CORE,     4U, gDru7InstShare},
#endif
};

/** \brief local BoardCfg resource ranges for DRUs local to C7X cores */
#if defined (BUILD_C7X)
const Udma_RmDefBoardCfgResp gUdmaLocalResRange[UDMA_RM_NUM_C7X_DRU] =
{
    /* resId,                                   rangeStart,                  rangeNum,        rangeStartSec,        rangeNumSec */
    {UDMA_RM_C7X_MSMC_DRU4,         UDMA_UTC_START_CH_DRU4,      UDMA_UTC_NUM_CH_DRU4,                   0U,               0U},
    {UDMA_RM_C7X_MSMC_DRU5,         UDMA_UTC_START_CH_DRU5,      UDMA_UTC_NUM_CH_DRU5,                   0U,               0U},
    {UDMA_RM_C7X_MSMC_DRU6,         UDMA_UTC_START_CH_DRU6,      UDMA_UTC_NUM_CH_DRU6,                   0U,               0U},
    {UDMA_RM_C7X_MSMC_DRU7,         UDMA_UTC_START_CH_DRU7,      UDMA_UTC_NUM_CH_DRU7,                   0U,               0U}
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

const Udma_RmDefBoardCfgPrms *Udma_rmGetDefBoardCfgPrms(uint32_t instId)
{
    const Udma_RmDefBoardCfgPrms  *rmDefBoardCfgPrms;

    if(UDMA_INST_ID_MCU_0 == instId)
    {

        rmDefBoardCfgPrms = &gUdmaRmDefBoardCfg_MainCore_McuNavss[0U];

#if defined (BUILD_MCU)
        CSL_ArmR5CPUInfo cpuInfo;
        CSL_armR5GetCpuID(&cpuInfo);

        /* MCU UDMA Instance requested by a Main core. */
        if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
        {
            rmDefBoardCfgPrms = &gUdmaRmDefBoardCfg_McuCore_McuNavss[0U];
        }
#endif

    }
    else if(UDMA_INST_ID_MAIN_0 == instId)
    {
        rmDefBoardCfgPrms = &gUdmaRmDefBoardCfg_MainCore_MainNavss[0U];

#if defined (BUILD_MCU)
        CSL_ArmR5CPUInfo cpuInfo;
        CSL_armR5GetCpuID(&cpuInfo);
        
        /* Main UDMA Instance requested by an MCU core. */
        if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
        {
            rmDefBoardCfgPrms = &gUdmaRmDefBoardCfg_McuCore_MainNavss[0U];
        }
#endif    
    }
    else
    {
        /* BCDMA Instance requested by a Main core. */
        rmDefBoardCfgPrms = &gUdmaRmDefBoardCfg_MainCore_Bcdma[0U];

#if defined (BUILD_MCU)
        CSL_ArmR5CPUInfo cpuInfo;
        CSL_armR5GetCpuID(&cpuInfo);

        /* BCDMA Instance requested by a MCU core. */
        if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
        {
            rmDefBoardCfgPrms = &gUdmaRmDefBoardCfg_McuCore_Bcdma[0U];
        }
#endif    
    }

    return (rmDefBoardCfgPrms);
}

Udma_RmSharedResPrms *Udma_rmGetSharedResPrms(uint32_t resId)
{
    Udma_RmSharedResPrms  *rmSharedResPrms = NULL;
    uint32_t    i;

#if defined (BUILD_MCU)
    /* Change the minReq and instShare to the correct value 
       for each core at runtime, before returning the 
       rmSharedResPrms.
    */
    CSL_ArmR5CPUInfo cpuInfo;
    CSL_armR5GetCpuID(&cpuInfo);

    if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU1_0;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU1_0;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU1_0;

            gUdmaRmSharedResPrms[1].instShare = gEvtInstShare_Mcu;
            gUdmaRmSharedResPrms[2].instShare = gVintInstShare_Mcu;
            gUdmaRmSharedResPrms[3].instShare = gIntrInstShare_Mcu;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU1_1;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU1_1;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU1_1;

            gUdmaRmSharedResPrms[1].instShare = gEvtInstShare_Mcu;
            gUdmaRmSharedResPrms[2].instShare = gVintInstShare_Mcu;
            gUdmaRmSharedResPrms[3].instShare = gIntrInstShare_Mcu;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_1)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU2_0;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU2_0;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU2_0;

            gUdmaRmSharedResPrms[1].instShare = gEvtInstShare2_0;
            gUdmaRmSharedResPrms[2].instShare = gVintInstShare2_0;
            gUdmaRmSharedResPrms[3].instShare = gIntrInstShare2_0;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU2_1;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU2_1;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU2_1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_2)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU3_0;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU3_0;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU3_0;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU3_1;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU3_1;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU3_1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else if (cpuInfo.grpId == CSL_ARM_R5_CLUSTER_GROUP_ID_3)
    {
        if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_0)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU4_0;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU4_0;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU4_0;
        }
        else if (cpuInfo.cpuID == CSL_ARM_R5_CPU_ID_1)
        {
            gUdmaRmSharedResPrms[1].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_GLOBAL_EVENT_MCU4_1;
            gUdmaRmSharedResPrms[2].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_VINTR_MCU4_1;
            gUdmaRmSharedResPrms[3].minReq = UDMA_RM_SHARED_RES_PRMS_MINREQ_IR_INTR_MCU4_1;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        /* Do Nothing */
    }
#endif

    for (i = 0U; i < UDMA_RM_NUM_SHARED_RES; i++)
    {
        if(resId == gUdmaRmSharedResPrms[i].resId)
        {
            rmSharedResPrms = &gUdmaRmSharedResPrms[i];
            break;
        }
    }

    return (rmSharedResPrms);
}

#if defined (BUILD_C7X)
const Udma_RmDefBoardCfgResp *Udma_rmGetLocalBoardCfgResp(uint32_t resId)
{
    const Udma_RmDefBoardCfgResp *ret = NULL;
    uint32_t    i;

    for (i = 0U; i < UDMA_RM_NUM_C7X_DRU; i++)
    {
        if(resId == gUdmaLocalResRange[i].resId)
        {
            ret = &gUdmaLocalResRange[i];
            break;
        }
    }

    return ret;
}
#endif

