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
 *  \file enet_serial.c
 *
 *  \brief Top level serial protocol layer
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "inc/enet_serial.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/*   A global context is used. Only one transfer can be active at a time */
EnetSerial_ContextObj Serial_Context __attribute__ ((section(".serialContext")));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*   Validates the configuration, then passes through to the specific protocol
 *   initialization
 */
void *EnetSerial_init(EnetSerial_ConfigObj *cfg)
{
    /* Sanity check on the input structure */
    if (cfg->magic != ENET_SERIAL_MODULE_MAGIC)
    {
        return(NULL);
    }

    if (cfg->protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerialXmodem_init((void *)cfg));
    }
    else
    {
        return(NULL);
    }
}

/* Dispatches to the correct protocol handler */
int32_t EnetSerial_open(EnetSerial_ContextObj *cxt)
{
    /* Sanity check on the context */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    if (cxt->cfg.protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerialXmodem_open((void *)cxt));
    }
    else
    {
        return(EnetSerial_bad_protocol);
    }
}

/* The reception protocol is initiated */
int32_t EnetSerial_start(EnetSerial_ContextObj *cxt)
{
    /* Sanity check on the context */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    if (cxt->cfg.protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerialXmodem_start((void *)cxt));
    }
    else
    {
        return(EnetSerial_bad_protocol);
    }
}

/* Debug info is passed through to the protocol handler for output */
int32_t EnetSerial_sendDebugInfo(EnetSerial_ContextObj *cxt,
                                 uint8_t c)
{
    /* Sanity check on the context */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    if (cxt->cfg.protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerialXmodem_sendDebugInfo((void *)cxt, c));
    }
    else
    {
        return(EnetSerial_bad_protocol);
    }
}

/* Dispatches to the correct protocol handler */
int32_t EnetSerial_read(EnetSerial_ContextObj *cxt,
                        uint8_t *addr,
                        int32_t len)
{
    /* Context sanity check */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Fall through to the protocol specific handler */
    if (cxt->cfg.protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        int32_t length = EnetSerial_invalid_read_len;
        char buff[ENET_SERIAL_XMODEM_DATA_MAX_LEN] = {0};
        char *pRecvBuff = (char *)addr;

        EnetSerialXmodem_read((void *)cxt,
                              (uint8_t *)buff,
                              ENET_SERIAL_XMODEM_DATA_MAX_LEN);
        while (length != EnetSerial_read_past_end)
        {
            length = EnetSerial_readStatus(cxt, (uint8_t **)&buff);

            if (length > 0U)
            {
                /* concatenate  buff to recvBuff */
                sprintf(pRecvBuff, "%.*s", length, buff);
                pRecvBuff += length;

                /* memset tempBuff and re-submit for next packet*/
                memset(&buff[0U], 0U, ENET_SERIAL_XMODEM_DATA_MAX_LEN);
                EnetSerialXmodem_read((void *)cxt,
                                      (uint8_t *)buff,
                                      ENET_SERIAL_XMODEM_DATA_MAX_LEN);
            }
        }

        return(EnetSerial_success);
    }
    else
    {
        return(EnetSerial_bad_protocol);
    }
}

/* Dispatches to the correct protocol handler */
int32_t EnetSerial_readStatus(EnetSerial_ContextObj *cxt,
                              uint8_t **dataAddr)
{
    /* Context sanity check */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Fall through to the protocol specific handler */
    if (cxt->cfg.protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerialXmodem_readStatus((void *)cxt, dataAddr));
    }
    else
    {
        return(EnetSerial_bad_protocol);
    }
}

/* Dispatches to the correct protocol handler */
int32_t EnetSerial_close(EnetSerial_ContextObj *cxt,
                         uint32_t timeoutMsec)
{
    /* Context sanity check */
    if (cxt->serialMagic != ENET_SERIAL_MODULE_MAGIC)
    {
        return((int32_t)EnetSerial_bad_magic);
    }

    /* Fall through to the protocol specific handler */
    if (cxt->cfg.protocol == ENET_SERIAL_PROTOCOL_XMODEM)
    {
        return(EnetSerialXmodem_close((void *)cxt, timeoutMsec));
    }
    else
    {
        return(EnetSerial_bad_protocol);
    }
}
