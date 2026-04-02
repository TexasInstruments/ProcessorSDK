/*
 * Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * \file  lwipif2enet_appif.h
 *
 * \brief Header file for application interfaces to the LwIP Enet interface.
 */

/*!
 * \ingroup  DRV_ENET_MODULE
 * \defgroup ENET_LWIP_API Enet lwIP native netif
 * @{
 *
 * \defgroup ENET_LWIP_APPIF_API Enet lwIP application interface APIs
 *
 * @{
 */

#ifndef LWIPIF2ENET_APPIF_H_
#define LWIPIF2ENET_APPIF_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <assert.h>
#include <ti/drv/enet/enet.h>
#include <lwip/netif.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

/*! \brief Number of RX channels. */
#define LWIP2ENET_RX_NUM                    (2U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/*!
 * \brief Callback function used to free packets.
 *
 * Prototype of the callback function used by Enet's netif to free packets during
 * deinitialization stage.
 *
 * \param cbArg         Callback function argument.
 * \param fqPktInfoQ    Pointer to free queue (FQ).
 * \param cqPktInfoQ    Pointer to completion queue (CQ).
 */
typedef void (*LwipifEnetAppIf_FreePktCbFxn)(void *cbArg,
                                             EnetDma_PktQ *fqPktInfoQ,
                                             EnetDma_PktQ *cqPktInfoQ);

/*!
 * \brief Callback function used to check PHY link status.
 *
 * Prototype of the callback function used by Enet's netif to check the PHY link
 * status.
 *
 * \param netif   Enet's netif.
 * \param hEnet   Enet driver handle.
 *
 * \return BTRUE if PHY link is up, BFALSE otherwise.
 */
typedef bool (*LwipifEnetAppIf_IsPhyLinkedCbFxn)(struct netif *netif,
                                                 void *arg);

/*!
 * \brief Callback function used to pass packets to application for processing.
 *
 * When application provides a valid callback function, netif will call this function
 * for "processing":
 *  - Application consumes the packet and returns BTRUE.  The packet is not passed
 *    to the lwIP stack.
 *  - Application reads the packet but doesn't consume it, and returns BFALSE.  The
 *    packet is passed to lwIP stack as usual.
 *
 * This mechanism requires `LWIPIF_APP_RX_PKT_HANDLING` build flag to be enabled.
 *
 * \param netif         Enet's netif.
 * \param pbuf          Pointer to pbuf to be handled by app.
 *
 * \retval BTRUE  Packet will not be passed to the stack.
 * \retval BFALSE Packet will be passed to the stack as usual.
 */
typedef bool (*LwipifEnetAppIf_HandleRxPktFxn)(struct netif *netif,
                                               struct pbuf *pbuf);

/*!
 * \brief Callback function used to pass error packets back to the application
 *        for processing (logging or diagnostics).
 *
 * Upon reception of error frames, typically the underlying Ethernet device could
 * drop them or pass them to host application for logging or diagnostics.
 * If the latter, the packet will be passed to the application through this
 * callback.
 *
 * \param netif         Enet's netif.
 * \param pbuf          Pointer to error frame pbuf.
 * \param errCode       Device specific error code. For CPSW, refer to
 *                      \ref EnetUdma_CpswPktErr.
 */
typedef void (*LwipifEnetAppIf_HandleRxErrPktFxn)(struct netif *netif,
                                                  struct pbuf *pbuf,
                                                  uint32_t errCode);

/*!
 * RX configuration parameters.
 */
typedef struct LwipifEnetAppIf_RxConfig_s
{
    /*! Packet notify callback function.  Enet's netif passes the function that should
     *  be called when RX packets are ready.  Application should register this function
     *  as the DMA RX event function or equivalent. */
    EnetDma_PktNotifyCb notifyCb;

    /*! Packet notify callback argument. */
    void *cbArg;

    /*! Number of RX packets that the netif intends to use.  For instance, this determines
     *  the ring element count in UDMA based peripherals. */
    uint32_t numPackets;
} LwipifEnetAppIf_RxConfig;

/*!
 * TX configuration parameters.
 */
typedef struct LwipifEnetAppIf_TxConfig_s
{
    /*! Packet notify callback function.  Enet's netif passes the function that should
     *  be called upon TX packet completion.  Application should register this function
     *  as the DMA TX event function or equivalent. */
    EnetDma_PktNotifyCb notifyCb;

    /*! Packet notify callback argument. */
    void *cbArg;

    /*! Number of TX packets that the netif intends to use.  For instance, this determines
     *  the ring element count in UDMA based peripherals. */
    uint32_t numPackets;
} LwipifEnetAppIf_TxConfig;

/*!
 * \brief Input arguments passed to the application via LwipifEnetAppCb_getHandle()
 *        when Enet's netif is initialized.
 */
typedef struct LwipifEnetAppIf_GetHandleInArgs_s
{
    /*! Enet's netif */
    struct netif *netif;

    /*! TX configuration parameters */
    LwipifEnetAppIf_TxConfig txCfg;

    /*! RX configuration parameters */
    LwipifEnetAppIf_RxConfig rxCfg[LWIP2ENET_RX_NUM];
} LwipifEnetAppIf_GetHandleInArgs;

/*!
 * \brief Container structure of packet free callback info.
 */
typedef struct Lwip2EnetAppIf_FreePktInfo_s
{
    /*! Callback function used to free TX or RX packets */
    LwipifEnetAppIf_FreePktCbFxn cb;

    /*! Callback function argument */
    void *cbArg;
} Lwip2EnetAppIf_FreePktInfo;

/*!
 * \brief RX-related parameters required by Enet's netif. They are populated
 *        by application.
 */
typedef struct LwipifEnetAppIf_RxHandleInfo_s
{
    /*! Enet DMA receive channel. */
    EnetDma_RxChHandle hRxFlow;

    /*! UDMA flow index for flow used. */
    uint32_t rxFlowStartIdx;

    /*! UDMA Flow index for flow used. */
    uint32_t rxFlowIdx;

    /*! MAC address allocated for the flow. */
    uint8_t macAddr[ENET_MAC_ADDR_LEN];

    /*! Whether to use RX event or not. When disabled, it uses pacing timer to
     * retrieve packets periodically from driver. */
    bool disableEvent;

    /*! Pointer for function that lets application handle packet locally.
     *  Pass NULL if packets from this RX flow should be passed directly to the stack. */
    LwipifEnetAppIf_HandleRxPktFxn handlePktFxn;

    /*! Pointer to error packet handling function. Pass NULL if error packets are
     *  to be dropped by Enet's netif */
    LwipifEnetAppIf_HandleRxErrPktFxn handleErrPktFxn;
} LwipifEnetAppIf_RxHandleInfo;

/*!
 * \brief TX-related parameters required by Enet's netif. They are populated
 *        by application.
 */
typedef struct LwipifEnetAppIf_TxHandleInfo_s
{
    /*! Enet DMA transmit channel. */
    EnetDma_TxChHandle hTxChannel;

    /*! TX channel peer id. */
    uint32_t txChNum;

    /*! Whether to use TX event or not. When disabled, it uses "lazy" recycle mechanism
     *  to defer packet desc retrieval. */
    bool disableEvent;

    /*! Directed port number. Set to \ref ENET_MAC_PORT_INV for non-directed packets. */
    Enet_MacPort txPortNum;
} LwipifEnetAppIf_TxHandleInfo;

/*!
 * \brief Output arguments to be populated by application via
 *        LwipifEnetAppCb_getHandle() when Enet's netif is initialized.
 */
typedef struct LwipifEnetAppIf_GetHandleOutArgs_s
{
    /*! Underlying Ethernet device handler.  For native interfaces, app should pass an
     *  \ref Enet_Handle, while for virtual interfaces app should pass a handle to
     *  the remote device connection */
    void *handleArg;

#if defined (ENET_SOC_HOSTPORT_DMA_TYPE_UDMA)
    /*! UDMA driver handler. */
    Udma_DrvHandle hUdmaDrv;
#endif

    /*! Self core id. */
    uint32_t coreId;

    /*! Core key returned by Enet LLD. */
    uint32_t coreKey;

    /*! Print function to be used by Enet's netif. */
    Enet_Print print;

    /*! Max TX packet size per priority. */
    uint32_t txMtu[ENET_PRI_NUM];

    /*! Max RX packet size. */
    uint32_t hostPortRxMtu;

    /*! Callback used by Enet's netif to query PHY link status. */
    LwipifEnetAppIf_IsPhyLinkedCbFxn isPortLinkedFxn;

    /*! Packet transmission parameters populated by application,
     *  i.e. TX channel handle. */
    LwipifEnetAppIf_TxHandleInfo txInfo;

    /*! Packet reception parameters populated by application, i.e.
     *  RX channel (flow) handle, flow index and start index, etc. */
    LwipifEnetAppIf_RxHandleInfo rxInfo[LWIP2ENET_RX_NUM];

    /*! Timer interval for timer based RX pacing. */
    uint32_t timerPeriodUs;

    /*! Whether TX checksum offload is supported for TCP and UDP */
    bool txCsumOffloadEn;

    /*! Whether RX checksum offload is supported for TCP and UDP */
    bool rxCsumOffloadEn;
} LwipifEnetAppIf_GetHandleOutArgs;

/*!
 * \brief Arguments passed to application by Enet's netif during deinitialization.
 */
typedef struct LwipifEnetAppIf_ReleaseHandleInfo_s
{
    /*! Enet's netif. */
    struct netif *netif;

    /*! Underlying Ethernet device handler pased app init time in
     *  \ref LwipifEnetAppIf_GetHandleOutArgs::handleArg */
    void *handleArg;

#if defined (ENET_SOC_HOSTPORT_DMA_TYPE_UDMA)
    /*! UDMA driver handler. */
    Udma_DrvHandle hUdmaDrv;
#endif

    /*! Self core id. */
    uint32_t coreId;

    /*! Core key returned by Enet LLD. */
    uint32_t coreKey;

    /*! Packet transmission parameters that application needs to close TX channel. */
    LwipifEnetAppIf_TxHandleInfo txInfo;

    /*! Packet reception parameters that application needs to close RX channel. */
    LwipifEnetAppIf_RxHandleInfo rxInfo[LWIP2ENET_RX_NUM];

    /*! Callback used to free TX packets during deinitialization. */
    Lwip2EnetAppIf_FreePktInfo txFreePkt;

    /*! Callback used to free RX packets during deinitialization. */
    Lwip2EnetAppIf_FreePktInfo rxFreePkt[LWIP2ENET_RX_NUM];
} LwipifEnetAppIf_ReleaseHandleInfo;

/* ========================================================================== */
/*                         Global Variables Declarations                      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Callback function used by Enet's netif to delegate TX/RX channel open
 *        to the application.
 *
 * This callback function is called by Enet's netif during initialization stage
 * to open TX and RX channels.  This callback must be implemented by the
 * application.
 *
 * \param inArgs   Pointer to parameters needed by application to open TX and
 *                 RX channels.
 * \param outArgs  Parameters obtained during TX/RX channel opening which need
 *                 to be returned to Enet's netif, i.e. channel handles.
 */
extern void LwipifEnetAppCb_getHandle(LwipifEnetAppIf_GetHandleInArgs *inArgs,
                                      LwipifEnetAppIf_GetHandleOutArgs *outArgs);

/*!
 * \brief Callback function used by Enet's netif to delegate TX/RX channel close
 *        to the application.
 *
 * This callback function is called by Enet's netif during deinitialization stage
 * to close TX and RX channels.  This callback must be implemented by the
 * application.
 *
 * \param releaseInfo  Pointer to parameters/handles needed by application to close
 *                     TX and RX channels.
 */
extern void LwipifEnetAppCb_releaseHandle(LwipifEnetAppIf_ReleaseHandleInfo *releaseInfo);

/*!
 * \brief Callback function used by Enet's netif to open the TX/RX channel 
 *        to the application during reset recovery.
 *
 * This callback function is called during reset-recovery handling to open
 * the DMA channels.This will be called by lwipif layer trigger from Application.
 * This callback must be implemented by the application.
 *
 * \param inArgs   Pointer to parameters needed by application to open TX and
 *                 RX channels.
 * \param outArgs  Parameters obtained during TX/RX channel opening which need
 *                 to be returned to Enet's netif, i.e. channel handles.
 */
extern void LwipifEnetAppCb_openDma(LwipifEnetAppIf_GetHandleInArgs *inArgs,
                                    LwipifEnetAppIf_GetHandleOutArgs *outArgs);

/*!
 * \brief Callback function used by Enet's netif to close TX/RX channel close
 *        to the application.
 *
 * This callback function is called during reset-recovery handling to close
 * the DMA channels.This will be called by lwipif layer trigger from Application.
 * This callback must be implemented by the application.
 *
 * \param releaseInfo  Pointer to parameters/handles needed by application to close
 *                     TX and RX channels.
 */
extern void LwipifEnetAppCb_closeDma(LwipifEnetAppIf_ReleaseHandleInfo *releaseInfo);

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

#endif /* LWIPIF2ENET_APPIF_H_ */

/*!
 * @}
 * @}
 */
