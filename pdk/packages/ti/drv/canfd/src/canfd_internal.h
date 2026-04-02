/*!
 *   @file  canfd_internal.h
 *
 *   @brief
 *      This is an internal header file used by the CANFD module and should
 *      not be included directly by the applications.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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
@defgroup CANFD_DRIVER_INTERNAL_FUNCTION            CANFD Driver Internal Functions
@ingroup CANFD_DRIVER
@brief
*   The section has a list of all the internal functions used by the driver
*/
/**
@defgroup CANFD_DRIVER_INTERNAL_DATA_STRUCTURE      CANFD Driver Internal Data Structures
@ingroup CANFD_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifndef CANFD_INTERNAL_H
#define CANFD_INTERNAL_H

#include <ti/csl/csl_mcan.h>
#include <ti/osal/HwiP.h>
#include <ti/drv/canfd/canfd.h>
#include <ti/drv/canfd/soc/canfd_soc.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief      Maximum number of Tx message objects that can be supported by CANFD
 */
#define MCAN_MAX_TX_MSG_OBJECTS             (32U)

/*!
 *  @brief      Maximum number of Rx message objects that can be supported by CANFD
 */
#define MCAN_MAX_RX_MSG_OBJECTS             (32U)

/*!
 *  @brief      Maximum number of message objects that can be supported by CANFD
 */
#define MCAN_MAX_MSG_OBJECTS                (MCAN_MAX_TX_MSG_OBJECTS + MCAN_MAX_RX_MSG_OBJECTS)

/*! @brief  Standard ID Filter Element Size */
#define MCAN_MSG_RAM_STD_ELEM_SIZE          (1U)

/*! @brief  Extended ID Filter Element Size */
#define MCAN_MSG_RAM_EXT_ELEM_SIZE          (2U)

/*! @brief  Tx/Rx Element Size.
 * 18 words = 18 * 4 = 72 bytes: 8 bytes of header and 64 bytes of data */
#define MCAN_MSG_RAM_TX_RX_ELEM_SIZE        (18U)

/*! @brief  Message Identifier Masks */
#define XTD_MSGID_MASK                      (0x1fffffffU)
#define STD_MSGID_MASK                      (0x7ffU)
#define STD_MSGID_SHIFT                     (18U)

/*! @brief  Maximum payload supported by CAN-FD protocol in bytes. */
#define MCAN_MAX_PAYLOAD_BYTES              (64U)

/*! @brief  Maximum number of Rx buffers. */
#define MCAN_MAX_RX_BUFFERS                 (64U)

/*! @brief  Maximum number of Tx buffers. */
#define MCAN_MAX_TX_BUFFERS                 (32U)

/*! \brief Macro to get the size of an array */
#define CANFD_UTILS_ARRAYSIZE(x)  (sizeof(x) / sizeof(x[0]))

/*! \brief Get the index of the given element within an array */
#define CANFD_UTILS_GETARRAYINDEX(member, array)   (member - &array[0])

/*! \brief Macro to determine if a member is part of an array */
#define CANFD_UTILS_ARRAYISMEMBER(member, array)                              \
    (((((uint32)member - (uint32) & array[0]) % sizeof(array[0])) == 0)       \
     && (member >= &array[0])                                                 \
     && (CANFD_UTILS_GETARRAYINDEX(member, array) < CANFD_UTILS_ARRAYSIZE(array)))

/** @addtogroup CANFD_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 *  @brief    This enumeration defines the MCAN FIFO/Buffer element Size
 */
typedef enum CANFD_MCANElemSize_t
{
    /*! 8 byte data field */
    CANFD_MCANElemSize_8BYTES = 0U,

    /*! 12 byte data field */
    CANFD_MCANElemSize_12BYTES = 1U,

    /*! 16 byte data field */
    CANFD_MCANElemSize_16BYTES = 2U,

    /*! 20 byte data field */
    CANFD_MCANElemSize_20BYTES = 3U,

    /*! 24 byte data field */
    CANFD_MCANElemSize_24BYTES = 4U,

    /*! 32 byte data field */
    CANFD_MCANElemSize_32BYTES = 5U,

    /*! 48 byte data field */
    CANFD_MCANElemSize_48BYTES = 6U,

    /*! 64 byte data field */
    CANFD_MCANElemSize_64BYTES = 7U
}CANFD_MCANElemSize;

/*!
 *  @brief    This enumeration defines the values used to represent the CANFD driver state
 */
typedef enum CANFD_DriverState_t
{
    /*! CANFD controller not initialized */
    CANFD_DriverState_UNINIT,

    /*! CANFD controller started */
    CANFD_DriverState_STARTED,

    /*! CANFD controller stopped */
    CANFD_DriverState_STOPPED,

    /*! CANFD controller in sleep mode */
    CANFD_DriverState_SLEEP
}CANFD_DriverState;

/**
 * @brief  Defines all the interrupt are enabled.
 */
#define MCAN_INTR_MASK       ((uint32_t)MCAN_INTR_SRC_RX_FIFO0_NEW_MSG | \
                              (uint32_t)MCAN_INTR_SRC_RX_FIFO0_MSG_LOST |  \
                              (uint32_t)MCAN_INTR_SRC_RX_FIFO1_NEW_MSG | \
                              (uint32_t)MCAN_INTR_SRC_TRANS_COMPLETE |  \
                              (uint32_t)MCAN_INTR_SRC_TRANS_CANCEL_FINISH |  \
                              (uint32_t)MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG |  \
                              (uint32_t)MCAN_INTR_SRC_PROTOCOL_ERR_ARB |  \
                              (uint32_t)MCAN_INTR_SRC_PROTOCOL_ERR_DATA |  \
                              (uint32_t)MCAN_INTR_SRC_BUS_OFF_STATUS)

/**
 * @brief
 *  CANFD Master Control Block
 *
 * @details
 *  The structure describes the CANFD Driver and is used to hold the relevant
 *  information with respect to the CANFD module.
 */
typedef struct CANFD_DriverMCB_t
{
    /**
     * @brief   CANFD driver internal state
     */
    CANFD_DriverState               state;

    /**
     * @brief   Hardware Configuration of the CAN instance
     */
    CANFD_HwCfg                     hwCfg;

    /**
     * @brief   Data Length to DLC mapping
     */
    uint8_t                         mcanDataSize[16];

    /**
     * @brief   Message object handle book keeping
     */
    struct CANFD_MessageObject_t*  msgObjectHandle[MCAN_MAX_MSG_OBJECTS];

    /**
     * @brief   Tx Mapping to message handles for transmit post processing
     */
    struct CANFD_MessageObject_t*   txMapping[MCAN_MAX_TX_MSG_OBJECTS];

    /**
     * @brief   Rx Mapping to message handles for Rx processing
     */
    struct CANFD_MessageObject_t*   rxMapping[MCAN_MAX_RX_MSG_OBJECTS];

    /**
     * @brief   Registered Interrupt Handler. Indicates if interrupt 0 is configured. This needs to be done only
     * during the first message object create. Subsequent interrupts are enabled using interrupt muxing
     */
    HwiP_Handle                     hwiHandle0;

    /**
     * @brief   Registered Interrupt Handler. Indicates if interrupt 0 is configured. This needs to be done only
     * during the first message object create. Subsequent interrupts are enabled using interrupt muxing
     */
    HwiP_Handle                     hwiHandle1;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CANFD driver on error or status interrrupts.
     */
    CANFD_ErrStatusAppCallBack      appErrCallBack;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CANFD driver once Tx complete or data receive interrupt has been received.
     */
    CANFD_DataAppCallBack           appDataCallBack;

    /**
     * @brief   Number of error and status interrupts received
     */
    uint32_t                        errStatusInterrupts;

    /**
     * @brief   Number of interrupts received for message Tx or Rx
     */
    uint32_t                        interrupts;

    /**
     * @brief   Number of ECC interrupts received
     */
    uint32_t                        eccInterrupts;

    /**
     * @brief   Number of Bus-Off interrupts received
     */
    uint32_t                        busOffInterrupts;

    /**
     * @brief   Number of Protocol error in data phase interrupts received
     */
    uint32_t                        protoDataErrInterrupts;

    /**
     * @brief   Number of Protocol error in arbitration phase interrupts received
     */
    uint32_t                        protoArbErrInterrupts;

    /**
     * @brief   Tx Status of the message object
     */
    uint8_t                         txStatus[MCAN_MAX_TX_MSG_OBJECTS];

    /**
     * @brief   Flag to toggle the usage of FIFO 0 and FIFO 1. Valid values are 0 and 1.
     */
    uint32_t                        useFifoNum;

    /**
     * @brief   Buffer used to read message RAM.
     */
    MCAN_RxBufElement               rxBuffElem;
}CANFD_DriverMCB;

/**
 * @brief
 *  CAN message object block
 *
 * @details
 *  The structure defines the message object
 */
typedef struct CANFD_MessageObject_t
{
    /**
     * @brief   Starting range of the Message Id to which the configuration belongs.
     * For Tx and single Message Id objects the startMsgId = endMsgId.
     */
    uint32_t                startMsgId;

    /**
     * @brief   Ending range of the Message Id to which the configuration belongs
     * For Tx and single Message Id objects the startMsgId = endMsgId.
     */
    uint32_t                endMsgId;

    /**
     * @brief   Pointer to the CANFD driver MCB
     */
    CANFD_DriverMCB*        ptrDriverMCB;

    /**
     * @brief   Message object direction.
     */
    CANFD_Direction         direction;

    /**
     * @brief   Message object type.
     */
    CANFD_MCANXidType       msgIdType;

    /**
     * @brief   Allocated message object number
     */
    uint32_t                messageObjNum;

    /**
     * @brief   Tx buffer number used to send data
     */
    uint32_t                txElement;

    /**
     * @brief   Rx buffer number used to receive data
     */
    uint32_t                rxElement;

    /**
     * @brief   Part of message ram to accessed by this message object. Refer enum #MCAN_MemType.
     */
    MCAN_MemType            memType;

    /**
     * @brief   Number of interrupts received
     */
    uint32_t                interruptsRxed;

    /**
     * @brief   Number of messages processed
     */
    uint32_t                messageProcessed;
}CANFD_MessageObject;

/** @}*/



/** @addtogroup CANFD_DRIVER_INTERNAL_FUNCTION
 @{ */

CANFD_DriverMCB *CANFD_socGetInstHandle(uint8_t instanceId);

const CANFD_HwCfg * CANFD_socGetHwCfg(uint8_t instanceId);


/** @}*/
#ifdef __cplusplus
}
#endif

#endif /* CANFD_INTERNAL_H */
