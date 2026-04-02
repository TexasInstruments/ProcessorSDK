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
 *  \file enet_serialxmodem.h
 *
 *  \brief Defines the structures and definitions used within the serial
 *         module for the x-modem protocol
 */

#ifndef ENET_SERIAL_XMODEM_H_
#define ENET_SERIAL_XMODEM_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <inttypes.h>
#include <stdbool.h>
#include <ti/csl/tistdtypes.h>

// !
// ! \defgroup Xmodem Serial

// !
// ! \ingroup Xmodem Serial
// !@{

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

// ! \brief Magic identifier
// !
#define ENET_SERIAL_XMODEM_MAGIC0    0x65010000

/*! @name XmodemStates
 *     \brief Valid operation X-modem states
 *  @{
 */
#define ENET_SERIAL_XMODEM_STATE_PINGS           1   // !< Sending initial pings to sender
#define ENET_SERIAL_XMODEM_STATE_WAIT_CMD        2   // !< Waiting for a command
#define ENET_SERIAL_XMODEM_STATE_STX_SOH_RCVD    3   // !< A command word has been received
#define ENET_SERIAL_XMODEM_STATE_BLK_NUM_RCVD    4   // !< Block number received
#define ENET_SERIAL_XMODEM_STATE_DATA            5   // !< One or more bytes of data received
#define ENET_SERIAL_XMODEM_STATE_CRC1_RCVD       6   // !< MSB of CRC received
#define ENET_SERIAL_XMODEM_STATE_BLOCK_DONE      7   // !< A complete frame (block) received
#define ENET_SERIAL_XMODEM_STATE_DONE            8   // !< Transmission complete

/*@} */

/*! @name XmodemChars
 *      \brief Control characters in the X-modem protocol
 *  @{
 */
#define ENET_SERIAL_XMODEM_CHAR_PING        0x43      // !< Ping
#define ENET_SERIAL_XMODEM_CHAR_NACK        0x15      // !< Nack
#define ENET_SERIAL_XMODEM_CHAR_ACK         0x06      // !< Ack
#define ENET_SERIAL_XMODEM_CHAR_SOH         0x01      // !< Start of frame
#define ENET_SERIAL_XMODEM_CHAR_STX         0x02      // !< Start of long frame
#define ENET_SERIAL_XMODEM_CHAR_EOT         0x04      // !< End of transmission
#define ENET_SERIAL_XMODEM_CHAR_CAN         0x18      // !< Cancel transmission

/*@} */

/*! @name XmodemRetuns
 *      \brief Local return values from the state machine
 *  @{
 */
#define ENET_SERIAL_XMODEM_RET_NONE              0x3030
#define ENET_SERIAL_XMODEM_RET_TOTAL_TIMEOUT     0x3031
#define ENET_SERIAL_XMODEM_RET_GOOD_BLOCK        0x3032
#define ENET_SERIAL_XMODEM_RET_INVALID_STATE     0x3033

#define ENET_SERIAL_XMODEM_DATA_MAX_LEN          (1024U)
// ! \brief The operational state
// !
typedef enum
{
    EnetSerialXmodem_state_init,   // !< Module initialized
    EnetSerialXmodem_state_open,   // !< Module opened
    EnetSerialXmodem_state_closed  // !< Module closed
} EnetSerialXmodem_states;

// ! \brief The X-modem state machine
// !

typedef struct EnetSerialXmodem_StateObj_s
{
    uint64_t timeoutStart;           // !< Start time for current timeout period, mili-seconds Q.3 format

    uint32_t xmodemState;            // !< Current state machine state
    uint32_t xferLenBytes;           // !< Current frame length, 128 or 1024 bytes
    uint32_t currentXferBytes;       // !< Number of bytes received in the current frame
    uint32_t blockNum;               // !< Current block number
    uint32_t returnCode;             // !< Frame status return value
    uint32_t timeout;                // !< Timeout (delta) value, mili-seconds Q.3 format

    uint8_t *buffer;                 // !< Base address of the buffer used to hold received data
    uint16_t crc;                    // !< CRC value received in frame
    bool duplicateFrame;             // !< True if a frame is resent
    bool requestAbort;               // !< When true we generate an abort request
} EnetSerialXmodem_StateObj;

// ! \brief Xmodem protocol fields
// !

typedef struct EnetSerial_XmodemCfgObj_s
{
    uint8_t maxErrorCount;              // !< Error count threshold resulting in abort
    uint8_t ackTimeoutSec;              // !< Time, in seconds, to timeout on an ack, retry time for pings
    uint8_t charTimeoutMs;              // !< Inter-character timeout, in mili-seconds
} EnetSerial_XmodemCfgObj;

// ! \brief Xmodem stats recorded during operation
// !

typedef struct EnetSerial_XmodemStatsObj_s
{
    uint32_t bytesReceived;             // !< Total number of data bytes received
    uint32_t frames128Received;         // !< Number of 128 byte frames received
    uint32_t frames1024Received;        // !< Number of 1024 byte frames receieved
    uint32_t numPingsSent;              // !< Number of ping characters sent
    uint32_t numAckTimeouts;            // !< Number of timeouts after sending acks
    uint32_t numCharTimeouts;           // !< Number of timeouts while receiving characters
    uint32_t errorCount;                // !< Number of errors encountered in state machine
    uint32_t numDuplicateFrames;        // !< Number of frame received twice
    uint32_t numCrcErrs;                // !< Number of frames found with CRC errors
    uint32_t maxFifoDepth;              // !< Max Fifo depth value seen
} EnetSerial_XmodemStatsObj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

// ! \brief          Initial protocol/driver setup
// ! \param[in] cfg  Protocol and driver configuration @ref EnetSerial_ConfigObj
// ! \return         The context information returned with each call
extern void *EnetSerialXmodem_init(void *cfg);

// ! \brief       Open the driver for reads
// ! \param[in]   cxt The driver context information
// ! \param[in]   timeout The call timeout period, in miliseconds
// ! \param[in]   refClkkHz The module reference clock in kHz
// ! \return 0    on success, any other value on failure
extern int32_t EnetSerialXmodem_open(void *scxt);

// ! \brief  The xmodem receive protocol is initiated by sending a ping character
// ! \param[in]   cxt The driver context information
// ! \return      0 on success, any other value on failure
extern int32_t EnetSerialXmodem_start(void *scxt);

// ! \brief  Send a debug character through the serial interface
// ! \param[in]   cxt The driver context information
// ! \param[in]   c   the character to send
// ! \return      Serial_success on success, any other value if the character was not accepted
extern int32_t EnetSerialXmodem_sendDebugInfo(void *scxt,
                                              uint8_t c);

// ! \brief       Initiate a frame read
// ! \param[in]   cxt The driver context information
// ! \param[in]   addr The location of the read buffer
// ! \param[in]   len The size of the buffer
// ! \return      the number of bytes read
extern int32_t EnetSerialXmodem_read(void *scxt,
                                     uint8_t *addr,
                                     int32_t len);

// ! \brief               Check the status of a frame read
// ! \param[in] cxt       The driver context information
// ! \param[out] dataAddr The address of the received data when the return value is positive
// ! \return              negative value on error, 0 if read in progress, positive when read is done with the number of bytes read
// !
extern int32_t EnetSerialXmodem_readStatus(void *scxt,
                                           uint8_t **dataAddr);

// ! \brief                  Close the driver
// ! \param[in] cxt          The driver context information
// ! \param[in] timeoutMsec  The timeout value for the command
// ! \return                 0 on success, any other value on failure
extern int32_t EnetSerialXmodem_close(void *scxt,
                                      uint32_t timeoutMsec);

// !@}  // ingroup

#endif // ENET_SERIAL_XMODEM_H_
