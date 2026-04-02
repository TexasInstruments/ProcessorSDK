/******************************************************************************
Copyright (c) [2012 - 2017] Texas Instruments Incorporated

All rights reserved not granted herein.

Limited License.

 Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 license under copyrights and patents it now or hereafter owns or controls to
 make,  have made, use, import, offer to sell and sell ("Utilize") this software
 subject to the terms herein.  With respect to the foregoing patent license,
 such license is granted  solely to the extent that any such patent is necessary
 to Utilize the software alone.  The patent license shall not apply to any
 combinations which include this software, other than combinations with devices
 manufactured by or for TI ("TI Devices").  No hardware patent is licensed
 hereunder.

 Redistributions must preserve existing copyright notices and reproduce this
 license (including the above copyright notice and the disclaimer and
 (if applicable) source code license limitations below) in the documentation
 and/or other materials provided with the distribution

 Redistribution and use in binary form, without modification, are permitted
 provided that the following conditions are met:

 * No reverse engineering, decompilation, or disassembly of this software
   is permitted with respect to any software provided in binary form.

 * Any redistribution and use are licensed by TI for use only with TI Devices.

 * Nothing shall obligate TI to provide you with source code for the software
   licensed and provided to you in object code.

 If software source code is provided to you, modification and redistribution of
 the source code are permitted provided that the following conditions are met:

 * Any redistribution and use of the source code, including any resulting
   derivative works, are licensed by TI for use only with TI Devices.

 * Any redistribution and use of any object code compiled from the source code
   and any resulting derivative works, are licensed by TI for use only with TI
   Devices.

 Neither the name of Texas Instruments Incorporated nor the names of its
 suppliers may be used to endorse or promote products derived from this software
 without specific prior written permission.

 DISCLAIMER.

 THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

/**
 *******************************************************************************
 *
 * \ingroup group_vision_apps_utils
 * \defgroup NETWORK_CTRL_IMPL NetworkCtrl  Implementation
 *
 * @{
 */

/**
 *******************************************************************************
 *
 * \file NetworkCtrl_priv.h NetworkCtrl  private API/Data structures
 *
 *******************************************************************************
 */

#ifndef NETWORK_CTRL_API_H_
#define NETWORK_CTRL_API_H_

#ifdef __cplusplus
extern "C" {
#endif


//#include <networkCtrl_api.h>
#include <network_api.h>


#define NETWORK_CTRL_MAX_CMDS        (64U)
#define NETWORK_CTRL_CMD_STRLEN_MAX     (64U)
#define NETWORK_CTRL_MAX_PARAMS     (32U)

/*******************************************************************************
 *  \brief Flag that is set in the 'flags' field of the header to indicate
 *         this packet is a ACK packet for a previously send command
 *******************************************************************************
 */
#define NETWORK_CTRL_FLAG_ACK            (0x00000001U)


/*******************************************************************************
 *  \brief Function callback to handle command received over network
 *
 *         The Network command handler should call
 *         NetworkCtrl_readParams() to read parameters into user supplied buffer
 *         if prmSize > 0
 *
 *         The Network command handler should call
 *         NetworkCtrl_writeParams() to send ACK and parameters, if any,
 *         to the sender.
 *
 *         If there are no return parameters, if should still call
 *         NetworkCtrl_writeParams(NULL, 0, 0) to send ACK to the sender
 *
 *  \param cmd  [IN] The command that is received
 *  \param prmSize [IN] Size of the parameters for the received command
 *
 *******************************************************************************
 */
typedef void (*NetworkCtrl_Handler)(char *cmd, uint32_t prmSize);



/*******************************************************************************
 *  Data structure's
 *******************************************************************************
 */
/*
 *  Note: These structures are shared between PC and EVM so dont use any platform
 *        dependent types. Also disable struct padding to avoid potential size
 *        mismatch between client and server
 */

typedef struct {

    uint32_t    header;
    /**< Header magic number NETWORK_CTRL_HEADER */

    char         cmd[NETWORK_CTRL_CMD_STRLEN_MAX];
    /**< Command, specified as a string of char's */

    uint32_t    returnValue;
    /**< Return value that is set by the command handler */

    uint32_t    flags;
    /**< command specified flags, see NETWORK_CTRL_FLAG_* */

    uint32_t    prmSize;
    /**< Size of input parameters in units of bytes.
     *   Can be 0 if no parameters need to sent for a command
     */

} NetworkCtrl_CmdHeader;



typedef struct {

    char cmd[NETWORK_CTRL_CMD_STRLEN_MAX];
    NetworkCtrl_Handler handler;

} NetworkCtrl_CmdHandler;


typedef struct {

    pthread_t thrHndl;

    Bool tskExit;
    /**< Flag to exit task */

    uint16_t serverPort;
    /**< Server port to use */

    NetworkCtrl_CmdHeader cmdBuf;
    /**< Buffer for recevied command header */

    Network_SockObj sockObj;
    /**< Networking socket */

    NetworkCtrl_CmdHandler cmdHandler[NETWORK_CTRL_MAX_CMDS];

} NetworkCtrl_Obj;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NETWORK_CTRL_API_H_ */

/* @} */


