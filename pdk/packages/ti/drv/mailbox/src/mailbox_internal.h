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

/**
 *  \file mailbox_internal.h
 *
 *  \brief Mailbox Internal definitions
 *
 */

#ifndef MAILBOX_INTERNAL_H_
#define MAILBOX_INTERNAL_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/HwiP.h>
#include <ti/drv/mailbox/mailbox.h>
#if defined (SOC_TPR12)
#include <ti/drv/mailbox/soc/tpr12/csl_mbox.h>
#endif

#if defined (SOC_AWR294X)
#include <ti/drv/mailbox/soc/awr294x/csl_mbox.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** @addtogroup MAILBOX_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief   Mailbox header size in bytes. Header is used only between MSS and DSS communication when mailbox channel type is multi-channel.
 */
#define MAILBOX_MULTI_CH_HEADER_SIZE   (4U)

/**
 * @brief   Mailbox unused channel ID
 */
#define MAILBOX_UNUSED_CHANNEL_ID (0xFFU)

/**
 * @brief   Mailbox number of remote endpoints
 */
#define MAILBOX_MAX_NUM_REMOTES_ENDPOINTS   (10U)
/*!
 *  @brief      Mailbox TX box status enum
 *
 *  This enum defines the possible TX box status.
 *
 */
typedef enum
{
    /*!
      *  Status of TX box is empty. Acknowledge of the last TX message was received
      *  and a new message can be transmitted.
      */
    MAILBOX_TX_BOX_EMPTY,
    /*!
      *  Status of TX box is full. Acknowledge of the last TX message was not received
      *  and a new message can not be transmitted.
      */
    MAILBOX_TX_BOX_FULL
} Mailbox_TxBoxStatus;

/*!
 *  @brief  Mailbox Acknowledge flag
 *
 *  This enum defines the possible acknowledge flag values. Used for write polling mode only.
 *
 */
typedef enum
{
    /*!
      *  Ack not received
      */
    MAILBOX_TX_ACK_NOT_RECEIVED,
    /*!
      *  Ack received
      */
    MAILBOX_TX_ACK_RECEIVED
} Mailbox_TxAckFlag;

/*!
 *  @brief      Mailbox new message flag
 *
 *  This enum defines the possible acknowledge flag values. Used for read polling mode only.
 *
 */
typedef enum
{
    /*!
      *  new message not received
      */
    MAILBOX_NEW_MESSAGE_NOT_RECEIVED,
    /*!
      *  New message received
      */
    MAILBOX_NEW_MESSAGE_RECEIVED
} Mailbox_NewMsgFlag;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct Mailbox_RemoteCfg_t
{
    /**
     * @brief
     *  Stores the local endpoint which must be the same across all instances of the driver for
     *  a given master.
     */
    Mailbox_Instance   remoteEndpoint;
    /**
     * @brief
     *  Assures that only one instance of the driver is open (for this master) to the BSS
     */
    uint8_t        instCount;
    /**
     * @brief
     *  Stores status of read channel. Used to prevent multiple instances of MSS-DSS
     *  mailbox to use the channel at the same time.
     */
    uint8_t        readChIDInUse;
    /**
     * @brief
     *  Stores status of write channel. Used to prevent multiple instances of MSS-DSS
     *  mailbox to use the channel at the same time.
     */
    uint8_t        writeChIDInUse;
    /**
     * @brief
     *  Indicates if multichannel is enabled between MSS<->DSS.
     *  All mailbox instances between MSS and DSS must have the same channel type configuration.
     */
    Mailbox_ChType chType;
    /**
     * @brief
     *  Stores the handle of the driver instance for the last message that was sent.
     *  Used during ISR to determine which driver received the acknowledgement as ISR is received without message.
     *  Used in extended mode for MSS<->DSS communication only.
     */
    Mbox_Handle lastMsgSentHandle;
    /**
     * @brief
     *  Stores a pointer for the hardware configuration used by the MSS<->DSS communication.
     */
    void* hwCfgPtr;
    /**
     * @brief
     *  Array that stores handles for all mailbox instances. Used in extended mode for MSS<->DSS communication only.
     */
    Mbox_Handle handleArray[MAILBOX_CH_ID_MAX + 1];
}Mailbox_RemoteCfg;

/**
 * @brief
 *  Mailbox Driver
 *
 * @details
 *  The structure describes the Mailbox Driver and tracks all the information
 *  relevant to the driver instance
 *
 */
typedef struct Mailbox_Driver_t
{
    /**
     * @brief   Mailbox Configuration which was used to instantiate the driver
     */
    Mailbox_Config        cfg;
    /**
     * @brief   Local type of Mailbox
     */
    Mailbox_Instance      localEndpoint;
    /**
     * @brief  Remote type of Mailbox
     */
    Mailbox_Instance      remoteEndpoint;
    /**
     * @brief  Handle to the Read semaphore.
     */
    void                  *readSem;
    /**
     * @brief  Handle to the Write semaphore.
     */
    void                  *writeSem;
    /**
     * @brief   Status of TX box (full or empty)
     */
    Mailbox_TxBoxStatus   txBoxStatus;
    /**
     * @brief   Number of TX messages successfully sent (acknowledgement has been received)
     */
    uint16_t              txCount;
    /**
     * @brief   Number of RX messages received
     */
    uint16_t              rxCount;
    /**
     * @brief   Flag used for read polling mode. Indicate if a new message has arrived.
     */
    Mailbox_NewMsgFlag    newMessageFlag;
    /**
     * @brief   Number of bytes already read by past calls to the read API in current message
     */
    /*Code review: This variable can be 16bit, however it is used in expressions with 32bit variables. For simplicity it is 32bit.*/
    uint32_t              numBytesRead;
    /**
     * @brief  Pointer to hardware configuration.
     */
    void                  *hwCfg;
    /**
     * @brief   Number of full box isr received
     */
    uint16_t              boxFullIsrCount;
    /**
     * @brief   Number of empty box isr received
     */
    uint16_t              boxEmptyIsrCount;
    /**
     * @brief   Number of times the readFlush API was called.
     */
    uint16_t              readFlushCount;
    /**
     * @brief   Pointer to the common remote config structure maintained by driver.
     */
    Mailbox_RemoteCfg     *remoteCfgPtr;
    /**
     * @brief   Base Address of the Tx Mailbox for this instance (used for am65xx/am64x/j721e/j7200)
     */
    uint32_t              baseAddrTx;
    /**
     * @brief   Base Address of the Rx Mailbox for this instance (used for am65xx/am64x/j721e/j7200)
     */
    uint32_t              baseAddrRx;
}Mailbox_Driver;

/*!
 *  @brief      Mailbox ISR error counters
 *
 *  This Structure contains the error counters for all ISRs
 */
typedef struct
{
    /**
     * @brief  BSS mailbox full ISR
     */
    uint16_t mailboxFull;
    /**
     * @brief  BSS mailbox empty ISR
     */
    uint16_t mailboxEmpty;
} Mailbox_ISRErrorCnt_t;

/*!
 *  @brief      Mailbox Interrupt Handles
 *
 *  This Structure contains the mailbox interrupt handles for all ISRs
 */
typedef struct
{
    /**
     * @brief  Registered Mailbox Full Interrupt Handler to communicate with BSS.
     */
    void *mailboxFull;
    /**
     * @brief  Registered Mailbox empty Interrupt Handler to communicate with BSS.
     */
    void *mailboxEmpty;
} Mailbox_HwiHandles_t;

/*!
 *  @brief      Mailbox MCB
 *
 *  Structure for tracking information required by the mailbox driver
 */
typedef struct Mailbox_MCB_t
{
    /**
     * @brief
     *  Flag that checks if the Driver has been initialized. It can be initialized only once per master (DSS or MSS).
     */
    uint8_t        initFlag;
    /**
     * @brief
     *  Stores the local endpoint which must be the same across all instances of the driver for
     *  a given master.
     */
    Mailbox_Instance   localEndpoint;
    /**
    * @brief
    *   Total number of driver instances opened.
    */
    uint32_t       totalInstCnt;
    /**
    * @brief
    *   Configuration specific to the remote end point.
    */
    Mailbox_RemoteCfg remoteConfig[MAILBOX_MAX_NUM_REMOTES_ENDPOINTS];
    /**
     * @brief   Registered Mailbox Interrupt Handlers
     */
    Mailbox_HwiHandles_t hwiHandles;
    /**
     * @brief
     *  Store ISR error conditions.
     */
    Mailbox_ISRErrorCnt_t errCnt;
    /**
     * @brief
     * Store the init params
     */
    Mailbox_initParams initParam;

} Mailbox_MCB;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t Mailbox_validateLocalEndPoint(Mailbox_Instance localEndpoint);

int32_t Mailbox_validateRemoteEndPoint(Mailbox_Instance localEndpoint, Mailbox_Instance remoteEndpoint);

int32_t Mailbox_validateDataTransferMode(Mailbox_DataTransferMode dataTransferMode);

int32_t Mailbox_validateReadWriteMode(Mailbox_Mode readMode, Mailbox_Mode writeMode);

int32_t Mailbox_isMultiChannelSupported(Mailbox_Instance localEndpoint, Mailbox_Instance remoteEndpoint);

Mbox_Handle Mailbox_allocDriver(Mailbox_Instance remoteEndpoint);

int32_t Mailbox_freeDriver(Mbox_Handle handle);

void* Mailbox_getHwCfg(Mailbox_Instance remoteEndpoint);

int32_t Mailbox_getBoxFullRemoteInst(Mailbox_Instance *remoteEndpoint);

int32_t Mailbox_getBoxEmptyRemoteInst(Mailbox_Instance *remoteEndpoint);

int32_t Mailbox_initHw(Mbox_Handle handle);

static inline void Mailbox_RemoteCfg_init (Mailbox_RemoteCfg* remoteCfg);

int32_t Mailbox_registerInterrupts(Mbox_Handle handle);

int32_t Mailbox_unregisterInterrupts(Mbox_Handle handle);

/* ========================================================================== */
/*                       Static Function Definition                           */
/* ========================================================================== */

static inline void Mailbox_RemoteCfg_init (Mailbox_RemoteCfg* remoteCfg)
{
    uint32_t i;
    remoteCfg->remoteEndpoint      = MAILBOX_INST_INVALID;
    remoteCfg->instCount           = 0;
    remoteCfg->chType              = MAILBOX_CHTYPE_SINGLE;
    remoteCfg->lastMsgSentHandle   = NULL;
    remoteCfg->hwCfgPtr            = NULL;
    remoteCfg->readChIDInUse       = MAILBOX_UNUSED_CHANNEL_ID;
    remoteCfg->writeChIDInUse      = MAILBOX_UNUSED_CHANNEL_ID;
    for(i=0; i <= MAILBOX_CH_ID_MAX; i++)
    {
        remoteCfg->handleArray[i] = NULL;
    }
}


/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* MAILBOX_INTERNAL_H_ */
