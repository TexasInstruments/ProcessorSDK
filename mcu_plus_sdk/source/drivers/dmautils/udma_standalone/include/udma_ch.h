/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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
 *  \ingroup DRV_UDMA_MODULE
 *  \defgroup DRV_UDMA_CH_MODULE UDMA Driver Channel API
 *            This is UDMA driver channel related configuration parameters and
 *            API
 *
 *  @{
 */

/**
 *  \file udma_ch.h
 *
 *  \brief UDMA Channel related parameters and API.
 */

#ifndef UDMA_CH_H_
#define UDMA_CH_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 * \brief Macro used to specify that DMA Channel ID is invalid.
 * Used in the API #Udma_chOpen.
 */
#define UDMA_DMA_CH_INVALID             ((uint32_t) 0xFFFF0000U)
/**
 * \brief Macro used to specify any available DMA Channel while requesting
 * one. Used in the API #Udma_chOpen.
 */
#define UDMA_DMA_CH_ANY                 ((uint32_t) 0xFFFF0001U)
/**
 * \brief Macro used to specify that the DMA Channel is not applicable for a
 * particular mode.
 */
#define UDMA_DMA_CH_NA                  ((uint32_t) 0xFFFF0002U)
/** \brief Macro used to specify that the UTC ID is invalid. */
#define UDMA_UTC_ID_INVALID             ((uint32_t) 0xFFFF0003U)
/** \brief Macro used to specify that the Mapped Channel Group is invalid. */
#define UDMA_MAPPED_GROUP_INVALID       ((uint32_t) 0xFFFF0004U)


/**
 *  \anchor Udma_ChFlag
 *  \name UDMA Channel Flag
 *
 *  UDMA channel flags bit field used to form the channel type.
 *
 *  @{
 */
/** \brief UTC channel flag */
#define UDMA_CH_FLAG_UTC                ((uint32_t) 0x0020U)
/* @} */


/**
 *  \brief UTC channel. This could be
 *      - UTC with descriptor posted through UDMA external channel like VPAC/DMPAC
 *      - DRU channel with direct mode with descriptor posted through direct
 *        DRU register writes or with indirect mode through External channel
 */
#define UDMA_CH_TYPE_UTC                (UDMA_CH_FLAG_UTC)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief UDMA channel open parameters.
 */
typedef struct
{
    uint32_t                chNum;
    /**< [IN] UDMAP channel to allocate.
     *
     *   Set to #UDMA_DMA_CH_ANY  if the channel to allocate and open
     *   could be any from the free pool.
     *   Set to the actual DMA channel when specific DMA channel need to be
     *   allocated. This channel number is relative to the channel type
     *   (TX, RX or External). The driver will internally calculate the
     *   respective offset to get the actual UDMAP channel number.
     */
    uint32_t                utcId;
    /**< [IN] The UTC instance to use when channel type is #UDMA_CH_TYPE_UTC.
     *   Refer \ref Udma_UtcIdSoc macro for details.
     *
     *   For other channel type set to #UDMA_UTC_ID_INVALID
     */

} Udma_ChPrms;

/**
 *  \brief UDMA UTC channel parameters.
 */
typedef struct
{
    uint8_t                 pauseOnError;
    /**< [IN] Bool: When set (TRUE), pause channel on error */
    uint8_t                 addrType;
    /**< [IN] Address type for this channel.
     *   Refer \ref tisci_msg_rm_udmap_tx_ch_cfg_req::tx_atype */
    uint8_t                 chanType;
    /**< [IN] Channel type. Refer \ref tisci_msg_rm_udmap_tx_ch_cfg_req::tx_chan_type */
    uint16_t                fetchWordSize;
    /**< [IN] Descriptor/TR Size in 32-bit words */
    uint8_t                 busPriority;
    /**< [IN] 3-bit priority value (0=highest, 7=lowest) */
    uint8_t                 busQos;
    /**< [IN] 3-bit qos value (0=highest, 7=lowest) */
    uint8_t                 busOrderId;
    /**< [IN] 4-bit orderid value */
    uint8_t                 dmaPriority;
    /**< [IN] This field selects which scheduling bin the channel will be
     *   placed in for bandwidth allocation of the Tx DMA units.
     *   Refer \ref tisci_msg_rm_udmap_tx_ch_cfg_req::tx_sched_priority */
    uint8_t                 burstSize;
    /**< [IN] Specifies the nominal burst size and alignment for data transfers
     *   on this channel.
     *   Refer \ref tisci_msg_rm_udmap_tx_ch_cfg_req::tx_burst_size.
     *   Note1: This parameter should be set less than or equal to the FIFO
     *   depth parameter set i.e.
     *          fifoDepth >= burstSize
     *
     *   Below are the supported burst sizes for various channel types
     *   Normal Capacity Channel        - 64 or 128 bytes
     *   High Capacity Channel          - 64, 128 or 256 bytes
     *   Ultra High Capacity Channel    - 64, 128 or 256 bytes
     */
    uint8_t                 supressTdCqPkt;
    /**< [IN] Bool: Specifies whether or not the channel should suppress
     *   sending the single data phase teardown packet when teardown is
     *   complete.
     *      FALSE = TD packet is sent
     *      TRUE = Suppress sending TD packet
     *   TODO: Should we allocate tdCq based on this flag?
     */

    uint64_t                druOwner;
    /**< [IN] This field controls how the TR is received by the DRU.
     *   Refer \ref CSL_DruOwner. This only applicable for DRU UTC type
     *   Note: This mode once set cannot be changed after a channel is enabled.
     *   One can't switch the submission mode dynamically when the channel is
     *   enabled. It can changed only after a successful teardown/disable
     *   sequence.
     */
    uint32_t                druQueueId;
    /**< [IN] Channel scheduling queue to be used. Refer \ref CSL_DruQueueId.
     *        This only applicable for DRU UTC type
     *        All the DRUs only have 5 queues implemented in the current design.
     *        1 Priority queue and 4 round robin queues.
     *        The priority queue is queue 0 the round robin
     *        queues are queues 1 - 4.
     */
}Udma_ChUtcPrms;



/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  \brief UDMA open channel.
 *
 *  Opens the UDMA channel based on the channel parameters. This also does
 *  the PSILCFG pairing based on the peer thread ID provided.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2578)
 *
 *  \param drvHandle    [IN] UDMA driver handle pointer passed during
 *                           #Udma_init
 *  \param chHandle     [IN/OUT] UDMA channel handle. The caller need to
 *                           allocate memory for this object and pass this
 *                           pointer to all further APIs. The caller should
 *                           not change any parameters as this is owned and
 *                           maintained by the driver.
 *                           This parameter can't be NULL.
 *  \param chType       [IN] UDMA channel type. Refer \ref Udma_ChType.
 *  \param chPrms       [IN] UDMA channel parameters.
 *                           This parameter can't be NULL.
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t Udma_chOpen(Udma_DrvHandle drvHandle,
                    Udma_ChHandle chHandle,
                    uint32_t chType,
                    const Udma_ChPrms *chPrms);

/**
 *  \brief UDMA close channel.
 *
 *  Closes the UDMA channel and frees all associated resources.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2579)
 *
 *  \param chHandle     [IN] UDMA channel handle.
 *                           This parameter can't be NULL.
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t Udma_chClose(Udma_ChHandle chHandle);

/**
 *  \brief UDMA configure UTC channel.
 *
 *  Configures the UTC channel parameters. Note: This is applicable only
 *  when the channel type is UTC
 *
 *  Note: This API can't be called after channel enable.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2582)
 *
 *  \param chHandle     [IN] UDMA channel handle.
 *                           This parameter can't be NULL.
 *  \param utcPrms      [IN] UDMA UTC channel parameter.
 *                           Refer #Udma_ChUtcPrms.
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t Udma_chConfigUtc(Udma_ChHandle chHandle, const Udma_ChUtcPrms *utcPrms);

/**
 *  \brief UDMA channel enable API.
 *
 *  This function will enable the UDMA channel.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2584)
 *
 *  \param chHandle     [IN] UDMA channel handle.
 *                           This parameter can't be NULL.
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t Udma_chEnable(Udma_ChHandle chHandle);

/**
 *  \brief UDMA channel teardown and disable API.
 *
 *  This function will perform the channel teardown and eventually disables
 *  the UDMA channel.
 *  This initiates the force teardown sequence based on the channel type and
 *  wait for teardown to complete gracefully.
 *  If the teardown doesn't complete within the timeout provided, then this
 *  will initiate a force teardown sequence.
 *
 *  Caution: This API is blocking. Hence cannot be called from ISR context!!
 *
 *  Requirement: DOX_REQ_TAG(PDK-2585)
 *
 *  \param chHandle     [IN] UDMA channel handle.
 *                           This parameter can't be NULL.
 *  \param timeout      [IN] Timeout in ms.
 *                           Use #UDMA_WAIT_FOREVER to wait forever.
 *
 *  \return \ref Udma_ErrorCodes
 */
int32_t Udma_chDisable(Udma_ChHandle chHandle, uint32_t timeout);



/**
 *  \brief Returns the channel number offset with in a channel type - TX, RX
 *  and External (UTC) channel types.
 *
 *  In case of UTC type, this returns the relative offset from the start
 *  of UTC it belongs to (and not from the external channel start).
 *
 *  Requirement: TODO
 *
 *  \param chHandle     [IN] UDMA channel handle.
 *                           This parameter can't be NULL.
 *
 *  \return Channel number. Returns #UDMA_DMA_CH_INVALID for error.
 */
uint32_t Udma_chGetNum(Udma_ChHandle chHandle);


/*
 * Structure Init functions
 */
/**
 *  \brief Udma_ChPrms structure init function.
 *
 *  \param chPrms       [IN] Pointer to #Udma_ChPrms structure.
 *  \param chType       [IN] UDMA channel type. Refer \ref Udma_ChType.
 *
 */
void UdmaChPrms_init(Udma_ChPrms *chPrms, uint32_t chType);

/**
 *  \brief Udma_ChUtcPrms structure init function.
 *
 *  \param utcPrms      [IN] Pointer to #Udma_ChUtcPrms structure.
 *
 */
void UdmaChUtcPrms_init(Udma_ChUtcPrms *utcPrms);
volatile uint64_t *Udma_druGetTriggerRegAddr(Udma_ChHandle chHandle);


/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Structure Declarations                   */
/* ========================================================================== */

/**
 *  \brief UDMA UTC instance information.
 *
 *  Note: This is an internal/private driver structure and should not be
 *  used or modified by caller.
 */
typedef struct
{
    CSL_DRU_t              *druRegs;
    /**< DRU register overlay - applicable only for DRU type UTC. Else this
     *   will be set to NULL */
    uint32_t                numQueue;
    /**< Number of queues present in a DRU*/
        uint32_t                numCh;
    /**< Number of channels present in the UTC */
} Udma_UtcInstInfo;

/**
 *  \brief UDMA channel object.
 *
 *  Note: This is an internal/private driver structure and should not be
 *  used or modified by caller.
 */
struct Udma_ChObj
{
    /**< UDMA channel type. Refer \ref Udma_ChType. */
    Udma_ChPrms             chPrms;
    uint32_t                druChNum;
    uint32_t                chInitDone;

    Udma_DrvHandle          drvHandle;
    /**< Pointer to global driver handle. */
    const Udma_UtcInstInfo *utcInfo;
    /**< Pointer to global UTC instance info. */
    Udma_ChUtcPrms          utcPrms;
    /**< UTC channel parameter passed during channel config. */
    volatile CSL_DRU_CHRTRegs_CHRT      *pDruRtRegs;
    volatile CSL_DRU_CHNRTRegs_CHNRT    *pDruNrtRegs;
};


#ifdef __cplusplus
}
#endif

#endif /* #ifndef UDMA_CH_H_ */

/* @} */
