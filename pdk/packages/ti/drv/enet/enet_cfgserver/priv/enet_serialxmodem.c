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
 *  \file enet_serialxmodem.c
 *
 *  \brief The Xmodem protocol layer driver
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <ti/osal/TimerP.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

#include "inc/enet_serial.h"
#include "enet_serialxmodem.h"

/* ========================================================================== */
/*                         Global Variables                                   */
/* ========================================================================== */

/*! \brief The xmodem CRC table. Generated with the polynomial 0x1021 */
const uint16_t EnetSerialXmodemCrc[] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/*    Initialize the xmodem protocol driver */

void *EnetSerialXmodem_init(void *cfg)
{
    EnetSerial_ConfigObj *scfg = (EnetSerial_ConfigObj *)cfg;

    /*  Sanity check the input structure by magic number */
    if (scfg->magic != ENET_SERIAL_MODULE_MAGIC)
    {
        return(NULL);
    }

    /*  Setup the local context */
    Serial_Context.serialMagic = ENET_SERIAL_MODULE_MAGIC;

    /*  Protocol based configuration */
    if (scfg->protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        Serial_Context.serialMagic           = ENET_SERIAL_MODULE_MAGIC;
        Serial_Context.protocol.xmodem.state = EnetSerialXmodem_state_init;

        memcpy(&Serial_Context.cfg, scfg, sizeof(Serial_Context.cfg));
        memset(&Serial_Context.protocol.xmodem.stats, 0, sizeof(Serial_Context.protocol.xmodem.stats));
    }
    else
    {
        return(NULL);
    }

    return((void *)&Serial_Context);
}

/*   The physical port is configured */
int32_t EnetSerialXmodem_open(void *scxt)
{
    EnetSerial_ContextObj *cxt = (EnetSerial_ContextObj *)scxt;

    /* Sanity check the context magic number */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Sanity check the protocol */
    if (cxt->cfg.protocol != ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerial_bad_protocol);
    }

    /* Sanity check the current driver state */
    if (cxt->protocol.xmodem.state != EnetSerialXmodem_state_init)
    {
        return((int32_t)EnetSerial_bad_state);
    }

    /* Change the driver state to open */
    cxt->protocol.xmodem.state = EnetSerialXmodem_state_open;

    /*  Configure the xmodem state machine, initial timeout setup for the ping */
    cxt->protocol.xmodem.sm.xmodemState      = ENET_SERIAL_XMODEM_STATE_PINGS;
    cxt->protocol.xmodem.sm.currentXferBytes = 0;
    cxt->protocol.xmodem.sm.blockNum         = 1;
    cxt->protocol.xmodem.sm.returnCode       = ENET_SERIAL_XMODEM_RET_NONE;
    cxt->protocol.xmodem.sm.buffer           = (uint8_t *)0;
    cxt->protocol.xmodem.sm.timeoutStart     = TimerP_getTimeInUsecs() / 125;
    /* ping timeout in msec Q.3 */
    cxt->protocol.xmodem.sm.timeout      = (cxt->cfg.protocolConfig.xmodem.ackTimeoutSec * 1000) << 3;
    cxt->protocol.xmodem.sm.requestAbort = BFALSE;

    return(EnetSerial_success);
}

/*   Xmodem transfer is started by sending a ping character */
int32_t EnetSerialXmodem_start(void *scxt)
{
    EnetSerial_ContextObj *cxt = (EnetSerial_ContextObj *)scxt;

    /* Sanity check the context magic number */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Sanity check the protocol */
    if (cxt->cfg.protocol != ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerial_bad_protocol);
    }

    /*  Send the initial ping character */
    UART_putc(ENET_SERIAL_XMODEM_CHAR_PING);
    return(EnetSerial_success);
}

/*   A debug character is sent out the serial port */
int32_t EnetSerialXmodem_sendDebugInfo(void *scxt,
                                       uint8_t c)
{
    UART_putc(c);
    return(EnetSerial_success);
}

/*! \brief The CRC is computed over a data set
 * *! \param[in] buf      The data buffer
 * *! \param[in] lenBytes The size of the data buffer in bytes
 * *! \return             The CRC over the data
 */

static uint16_t EnetSerialXmodem_computeCrc(uint8_t *buf,
                                            uint32_t lenBytes)
{
    uint32_t i;
    uint16_t v;
    uint16_t crc = 0;

    for (i = 0; i < lenBytes; i++)
    {
        v   = EnetSerialXmodemCrc[((crc >> 8) & 0xff)];
        crc = v ^ (crc << 8) ^ buf[i];
    }

    /* Flush through the remaining 16 bits */

    v   = EnetSerialXmodemCrc[((crc >> 8) & 0xff)];
    crc = v ^ (crc << 8) ^ 0;

    v   = EnetSerialXmodemCrc[((crc >> 8) & 0xff)];
    crc = v ^ (crc << 8) ^ 0;

    return(crc);
}

/*! \brief Received characters are processed through the xmodem state machine
 *  ! \param[in] cxt  The xmodem context
 *  ! \param[in] data The character received
 *  ! \return         always 0
 */
int32_t EnetSerialXmodem_rxData(void *scxt,
                                uint8_t data)
{
    uint64_t now;
    uint16_t crcCalc;
    uint8_t v8;

    EnetSerial_ContextObj *cxt = (EnetSerial_ContextObj *)scxt;

    now = TimerP_getTimeInUsecs() / 125;

    switch (cxt->protocol.xmodem.sm.xmodemState)
    {
        case ENET_SERIAL_XMODEM_STATE_PINGS:
        case ENET_SERIAL_XMODEM_STATE_WAIT_CMD:

            if (data == ENET_SERIAL_XMODEM_CHAR_SOH)
            {
                cxt->protocol.xmodem.sm.xmodemState  = ENET_SERIAL_XMODEM_STATE_STX_SOH_RCVD;
                cxt->protocol.xmodem.sm.timeoutStart = now;
                cxt->protocol.xmodem.sm.timeout      = cxt->cfg.protocolConfig.xmodem.charTimeoutMs << 3;
                cxt->protocol.xmodem.sm.xferLenBytes = 128;
            }
            else if (data == ENET_SERIAL_XMODEM_CHAR_STX)
            {
                cxt->protocol.xmodem.sm.xmodemState  = ENET_SERIAL_XMODEM_STATE_STX_SOH_RCVD;
                cxt->protocol.xmodem.sm.timeoutStart = now;
                cxt->protocol.xmodem.sm.timeout      = cxt->cfg.protocolConfig.xmodem.charTimeoutMs << 3;
                cxt->protocol.xmodem.sm.xferLenBytes = 1024;
            }
            else if (data == ENET_SERIAL_XMODEM_CHAR_EOT)
            {
                cxt->protocol.xmodem.sm.xmodemState = ENET_SERIAL_XMODEM_STATE_DONE;
                cxt->protocol.xmodem.sm.returnCode  = ENET_SERIAL_XMODEM_RET_NONE;
                UART_putc(ENET_SERIAL_XMODEM_CHAR_ACK);
            }
            else
            {
                if (cxt->protocol.xmodem.sm.xmodemState == ENET_SERIAL_XMODEM_STATE_PINGS)
                {
                    v8                                       = ENET_SERIAL_XMODEM_CHAR_PING;
                    cxt->protocol.xmodem.stats.numPingsSent += 1;
                }
                else
                {
                    v8                                     = ENET_SERIAL_XMODEM_CHAR_NACK;
                    cxt->protocol.xmodem.stats.errorCount += 1;
                }

                UART_putc(v8);
                cxt->protocol.xmodem.sm.timeoutStart = now;
                cxt->protocol.xmodem.sm.timeout      = (cxt->cfg.protocolConfig.xmodem.ackTimeoutSec * 1000) << 3;
            }

            break;

        case ENET_SERIAL_XMODEM_STATE_STX_SOH_RCVD:

            if (data == cxt->protocol.xmodem.sm.blockNum)
            {
                cxt->protocol.xmodem.sm.xmodemState    = ENET_SERIAL_XMODEM_STATE_BLK_NUM_RCVD;
                cxt->protocol.xmodem.sm.timeoutStart   = now;
                cxt->protocol.xmodem.sm.timeout        = cxt->cfg.protocolConfig.xmodem.charTimeoutMs << 3;
                cxt->protocol.xmodem.sm.duplicateFrame = BFALSE;
            }
            else if (data == ((cxt->protocol.xmodem.sm.blockNum - 1) & 0xFF))
            {
                /* Receiving a block that was already received. This indicates that the
                 * ACK sent to the sender was lost. This block should be received
                 * normally (including sending the ack) but discarded. */
                cxt->protocol.xmodem.sm.blockNum               = data;
                cxt->protocol.xmodem.sm.xmodemState            = ENET_SERIAL_XMODEM_STATE_BLK_NUM_RCVD;
                cxt->protocol.xmodem.sm.timeoutStart           = now;
                cxt->protocol.xmodem.sm.timeout                = cxt->cfg.protocolConfig.xmodem.charTimeoutMs << 3;
                cxt->protocol.xmodem.sm.duplicateFrame         = BTRUE;
                cxt->protocol.xmodem.stats.numDuplicateFrames += 1;
            }
            else
            {
                cxt->protocol.xmodem.sm.xmodemState    = ENET_SERIAL_XMODEM_STATE_WAIT_CMD;
                cxt->protocol.xmodem.stats.errorCount += 1;

                UART_putc(ENET_SERIAL_XMODEM_CHAR_NACK);
            }

            break;

        case ENET_SERIAL_XMODEM_STATE_BLK_NUM_RCVD:

            if (data == (~cxt->protocol.xmodem.sm.blockNum & 0xFF))
            {
                cxt->protocol.xmodem.sm.xmodemState      = ENET_SERIAL_XMODEM_STATE_DATA;
                cxt->protocol.xmodem.sm.currentXferBytes = 0;

                /* The block number is updated only after the block was successfully received */
                cxt->protocol.xmodem.sm.timeoutStart = now;
                cxt->protocol.xmodem.sm.timeout      = cxt->cfg.protocolConfig.xmodem.charTimeoutMs << 3;
            }
            else
            {
                cxt->protocol.xmodem.sm.xmodemState    = ENET_SERIAL_XMODEM_STATE_WAIT_CMD;
                cxt->protocol.xmodem.stats.errorCount += 1;

                UART_putc(ENET_SERIAL_XMODEM_CHAR_NACK);
            }

            break;

        case ENET_SERIAL_XMODEM_STATE_DATA:

            if (cxt->protocol.xmodem.sm.currentXferBytes >= cxt->protocol.xmodem.sm.xferLenBytes)
            {
                cxt->protocol.xmodem.sm.crc         = (data << 8);
                cxt->protocol.xmodem.sm.xmodemState = ENET_SERIAL_XMODEM_STATE_CRC1_RCVD;
            }
            else
            {
                if (cxt->protocol.xmodem.sm.currentXferBytes < cxt->protocol.xmodem.sm.xferLenBytes)
                {
                    cxt->protocol.xmodem.sm.buffer[cxt->protocol.xmodem.sm.currentXferBytes++] = data;
                }
            }

            cxt->protocol.xmodem.sm.timeoutStart = now;
            cxt->protocol.xmodem.sm.timeout      = cxt->cfg.protocolConfig.xmodem.charTimeoutMs << 3;
            break;

        case ENET_SERIAL_XMODEM_STATE_CRC1_RCVD:

            cxt->protocol.xmodem.sm.crc = cxt->protocol.xmodem.sm.crc | (data & 0xFF);

            crcCalc = EnetSerialXmodem_computeCrc(cxt->protocol.xmodem.sm.buffer, cxt->protocol.xmodem.sm.xferLenBytes);

            if (crcCalc == cxt->protocol.xmodem.sm.crc)
            {
                cxt->protocol.xmodem.sm.xmodemState = ENET_SERIAL_XMODEM_STATE_BLOCK_DONE;
                cxt->protocol.xmodem.sm.returnCode  = ENET_SERIAL_XMODEM_RET_GOOD_BLOCK;
                cxt->protocol.xmodem.sm.blockNum    = (cxt->protocol.xmodem.sm.blockNum + 1) & 0xFF;

                if (cxt->protocol.xmodem.sm.requestAbort == BTRUE)
                {
                    UART_putc(ENET_SERIAL_XMODEM_CHAR_CAN);
                    UART_putc(ENET_SERIAL_XMODEM_CHAR_CAN);
                    cxt->protocol.xmodem.sm.xmodemState = ENET_SERIAL_XMODEM_STATE_DONE;
                }
                else
                {
                    UART_putc(ENET_SERIAL_XMODEM_CHAR_ACK);
                }

                if (cxt->protocol.xmodem.sm.duplicateFrame == BFALSE)
                {
                    cxt->protocol.xmodem.stats.bytesReceived += cxt->protocol.xmodem.sm.xferLenBytes;

                    if (cxt->protocol.xmodem.sm.xferLenBytes == 128)
                    {
                        cxt->protocol.xmodem.stats.frames128Received += 1;
                    }
                    else
                    {
                        cxt->protocol.xmodem.stats.frames1024Received += 1;
                    }
                }
            }
            else
            {
                cxt->protocol.xmodem.stats.numCrcErrs += 1;
                cxt->protocol.xmodem.stats.errorCount += 1;
                cxt->protocol.xmodem.sm.xmodemState    = ENET_SERIAL_XMODEM_STATE_WAIT_CMD;

                UART_putc(ENET_SERIAL_XMODEM_CHAR_NACK);
            }

            cxt->protocol.xmodem.sm.timeoutStart = now;
            cxt->protocol.xmodem.sm.timeout      = (cxt->cfg.protocolConfig.xmodem.ackTimeoutSec * 1000) << 3;

            break;

        case ENET_SERIAL_XMODEM_STATE_DONE:

            /* Nothing to do if currently in the done state */
            break;

        default:

            /* Mode fails if the state is unknown */
            cxt->protocol.xmodem.stats.errorCount += 1;
            cxt->protocol.xmodem.sm.xmodemState    = ENET_SERIAL_XMODEM_STATE_DONE;
            cxt->protocol.xmodem.sm.returnCode     = ENET_SERIAL_XMODEM_RET_INVALID_STATE;
            break;
    }

    return(0);
}

/*! \brief   Checks the current timer value and checks for any timeouts.
 * ! \details On timeout the state machine is updated to reflect the timeout,
 * !          the timeout action is handled on the next state machine operation
 * ! \param[in] cxt The serial context
 * ! \return        always 0.
 */
int32_t EnetSerialXmodem_checkTimer(void *scxt)
{
    uint64_t now;
    uint64_t delta;

    EnetSerial_ContextObj *cxt = (EnetSerial_ContextObj *)scxt;

    now = TimerP_getTimeInUsecs() / 125;

    /* Check for state machine timeouts */
    delta = now - cxt->protocol.xmodem.sm.timeoutStart;

    if (delta > cxt->protocol.xmodem.sm.timeout)
    {
        switch (cxt->protocol.xmodem.sm.xmodemState)
        {
            case ENET_SERIAL_XMODEM_STATE_PINGS:

                /* Remain in the ping state, just send another ping */
                cxt->protocol.xmodem.stats.numPingsSent += 1;
                cxt->protocol.xmodem.sm.timeoutStart     = now;
                cxt->protocol.xmodem.sm.timeout          = (cxt->cfg.protocolConfig.xmodem.ackTimeoutSec * 1000) << 3;
                UART_putc(ENET_SERIAL_XMODEM_CHAR_PING);
                break;

            case ENET_SERIAL_XMODEM_STATE_STX_SOH_RCVD:
            case ENET_SERIAL_XMODEM_STATE_BLK_NUM_RCVD:
            case ENET_SERIAL_XMODEM_STATE_DATA:
            case ENET_SERIAL_XMODEM_STATE_CRC1_RCVD:

                /* Timeout. Re-start the state machine */
                cxt->protocol.xmodem.stats.numCharTimeouts += 1;
                cxt->protocol.xmodem.sm.xmodemState         = ENET_SERIAL_XMODEM_STATE_PINGS;
                break;

            default:

                /* Timeout ignored for all other states */
                break;
        }
    }

    return(0);
}

/*   An xmodem transfer is initiated */
int32_t EnetSerialXmodem_read(void *scxt,
                              uint8_t *addr,
                              int32_t len)
{
    EnetSerial_ContextObj *cxt = (EnetSerial_ContextObj *)scxt;

    /* Require that the buffer be at least as large as the largest xmodem frame */
    if (len < 1024)
    {
        return(EnetSerial_invalid_read_len);
    }

    /* Sanity check the context magic number */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Sanity check the protocol field which identifies the entry */
    if (cxt->cfg.protocol != ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerial_bad_protocol);
    }

    /* Sanity check the current driver state */
    if (cxt->protocol.xmodem.state != EnetSerialXmodem_state_open)
    {
        return((int32_t)EnetSerial_bad_state);
    }

    /* Sanity check the magic value in the protocol configuration */
    if (cxt->cfg.magic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Store the receive buffer address */
    cxt->protocol.xmodem.sm.buffer = addr;

    return(0);
}

/*  Reads any characters from the serial port. Returns 0 while
 *  a frame reception is in progress. Returns the frame size when
 *  a frame is complete.
 */
int32_t EnetSerialXmodem_readStatus(void *scxt,
                                    uint8_t **dataAddr)
{
    int32_t spchar;
    bool processedChar = BFALSE;

    EnetSerial_ContextObj *cxt = (EnetSerial_ContextObj *)scxt;

    /* Sanity check the context magic number */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Sanity check the protocol field which identifies the entry */
    if (cxt->cfg.protocol != ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerial_bad_protocol);
    }

    /* Sanity check the current driver state */
    if (cxt->protocol.xmodem.state != EnetSerialXmodem_state_open)
    {
        return((int32_t)EnetSerial_bad_state);
    }

    /* Sanity check the magic value in the protocol configuration */
    if (cxt->cfg.magic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* If the transfer is done return */
    if (cxt->protocol.xmodem.sm.xmodemState == ENET_SERIAL_XMODEM_STATE_DONE)
    {
        return((int32_t)EnetSerial_read_past_end);
    }

    /*  Read any characters from the port */
    while ((spchar = UART_getc()) != 0)
    {
        EnetSerialXmodem_rxData(cxt, (uint8_t)(spchar & 0xFF));

        processedChar = BTRUE;

        /* Return any data, setup the state machine for the next entry */
        if ((cxt->protocol.xmodem.sm.returnCode == ENET_SERIAL_XMODEM_RET_GOOD_BLOCK) &&
            (cxt->protocol.xmodem.sm.xmodemState == ENET_SERIAL_XMODEM_STATE_BLOCK_DONE))
        {
            /*  A valid frame detected. Reset the state for the next frame */
            cxt->protocol.xmodem.sm.xmodemState = ENET_SERIAL_XMODEM_STATE_WAIT_CMD;

            if (cxt->protocol.xmodem.sm.duplicateFrame == BFALSE)
            {
                return(cxt->protocol.xmodem.sm.xferLenBytes);
            }
        }
    }

    /* If no characters were available look for a timeout */
    if (processedChar == BFALSE)
    {
        EnetSerialXmodem_checkTimer(cxt);
    }

    return(0);
}

/*  The X-modem connection is completed, the serial port is disabled
 *  At this point the receiver is expecting only an <EOT> character.
 *  However it is possible that some extra data is included in the transfer
 *  that the boot will not use. In that case accept the frames but discard
 *  them until either a timeout occurs or the <EOT> is found.
 */
int32_t EnetSerialXmodem_close(void *scxt,
                               uint32_t timeoutMsec)
{
    return(1);
}
