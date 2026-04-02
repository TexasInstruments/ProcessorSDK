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
 *  \file enet_serial.h
 *
 *  \brief Defines the serial transmission protocols supported
 */

#ifndef ENET_SERIAL_H_
#define ENET_SERIAL_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <inttypes.h>
#include <stdbool.h>
#include "enet_serialxmodem.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

// ! \brief The module magic number used to check input structures
// !
#define ENET_SERIAL_MODULE_MAGIC     0x49

/*! @anchor SerialProtocols
 *  @name   SerialProtocols
 *      Serial transmission protocol supported
 *  @{
 */
#define ENET_SERIAL_PROTOCOL_XMODEM  63

/*!@} */

// ! \brief Protocol configuration
// !

typedef struct EnetSerial_ConfigObj_s
{
    uint8_t magic;                      // !< The module magic number
    uint8_t protocol;                   // !< The transmission protocol @ref SerialProtocols

    union
    {
        EnetSerial_XmodemCfgObj xmodem;        // !< Xmodem configuration
    } protocolConfig;
} EnetSerial_ConfigObj;

// ! \brief Return values for functions returning an int
// !

typedef enum
{
    EnetSerial_success          = 0,      // !< Function returned successfully
    EnetSerial_bad_magic        = -1,     // !< Bad magic value in the context
    EnetSerial_bad_state        = -2,     // !< Bad state value in the context
    EnetSerial_bad_protocol     = -3,     // !< Invalid protocol value in the context
    EnetSerial_bad_cfg_magic    = -4,     // !< Bad magic value in the config (under context)
    EnetSerial_hw_fail          = -5,     // !< Call to hardware layer returned error
    EnetSerial_invalid_read_len = -6,     // !< Read called with an invalid length
    EnetSerial_read_past_end    = -7      // !< Read status called after protocol complete
} EnetSerial_retval;

// ! \brief The Configurations for Xmodem protocol
typedef struct EnetSerial_XmodemProtocol_s
{
    EnetSerialXmodem_states state;             // !< xmodem module status
    EnetSerialXmodem_StateObj sm;              // !< xmodem state machine information
    EnetSerial_XmodemStatsObj stats;           // !< Run time statistics
} EnetSerial_XmodemProtocol;

// ! \brief The Configurations of supported serial protocols
typedef struct EnetSerial_Protocol_s
{
    EnetSerial_XmodemProtocol xmodem;
}EnetSerial_Protocol;

// ! \brief The context structure which holds information on the single
// !        transfer that can occur at one time
typedef struct EnetSerial_ContextObj_s
{
    uint8_t serialMagic;                  // !< Set to ENET_SERIAL_MODULE_MAGIC value when initialized

    EnetSerial_ConfigObj cfg;             // !< Module configuration information

    EnetSerial_Protocol protocol;
} EnetSerial_ContextObj;

/* ========================================================================== */
/*                         Global Variables                                   */
/* ========================================================================== */

extern EnetSerial_ContextObj Serial_Context;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

// ! \brief          Initial protocol/driver setup
// ! \param[in] cfg  Protocol and driver configuration @ref EnetSerial_ConfigObj
// ! \return         The context information returned with each call
extern void *EnetSerial_init(EnetSerial_ConfigObj *cfg);

// ! \brief       Open the driver for reads
// ! \param[in]   cxt        The driver context information
// ! \param[in]   timeout    The call timeout period, in miliseconds
// ! \param[in]   refClkkHz  The module functional clock value, in kHz
// ! \return 0    on success, any other value on failure
extern int32_t EnetSerial_open(EnetSerial_ContextObj *cxt);

// ! \brief   The data receive protocol is initiated
// ! \param[in]   cxt  The driver context information
// ! \return      0 on success, any other value on failure
extern int32_t EnetSerial_start(EnetSerial_ContextObj *cxt);

// ! \brief Send debug info prior to protocol start
// ! \param[in]   cxt        The driver context information
// ! \param[in]   c          The debug character to send
// ! \return      Serial_success if the character was accepted
extern int32_t EnetSerial_sendDebugInfo(EnetSerial_ContextObj *cxt,
                                        uint8_t c);

// ! \brief       Initiate a frame read
// ! \param[in]   cxt The driver context information
// ! \param[in]   addr The location of the read buffer
// ! \param[in]   len The size of the buffer
// ! \return      the number of bytes read
extern int32_t EnetSerial_read(EnetSerial_ContextObj *cxt,
                               uint8_t *addr,
                               int32_t len);

// ! \brief               Check the status of a frame read
// ! \param[in] cxt       The driver context information
// ! \param[out] dataAddr The address of the received data when the return value is positive
// ! \return              negative value on error, 0 if read in progress, positive when read is done with the number of bytes read
// !
extern int32_t EnetSerial_readStatus(EnetSerial_ContextObj *cxt,
                                     uint8_t **dataAddr);

// ! \brief                  Close the driver
// ! \param[in] cxt          The driver context information
// ! \param[in] timeoutMsec  The timeout value for the command
// ! \return                 0 on success, any other value on failure
extern int32_t EnetSerial_close(EnetSerial_ContextObj *cxt,
                                uint32_t timeoutMsec);

// !@}  // ingroup

#endif // ENET_SERIAL_H_
