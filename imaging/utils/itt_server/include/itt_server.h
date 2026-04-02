/*
 *
 * Copyright (c) 2020 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ITT_SERVER_H
#define ITT_SERVER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>

#include <utils/console_io/include/app_log.h>

#define NETWORK_CTRL_SERVER_PORT            (5000U)
/**
 * \defgroup group_vision_apps_itt_server (ISP Tuning Tool Server) APIs
 *
 * \brief This section contains APIs for ISP Tuning Tool Server
 *
 * \ingroup group_vision_apps_utils
 *
 * @{
 */


/**
 * \brief initialization routine for ITT server.
 *
 * This must be called from any application which needs to control ISP tuning at runtime.
 * Inputs
 * 1. appObj - Pointer to application object, compatible to the argument expected by appFileSaveCallback
 * 2. appFileSaveCallback - Callback funcion implemented in the calling application. This callback would take appObj as an argument. 
 *    If any of these arguments are NULL, live tuning will still work but RAW and YUV save features wil not be available.
 *
 */
int32_t itt_server_init(void * appObj, void * appFileSaveCallback, void * appDccUpdateCallback);

/**
 * \brief Handler for command "echo". Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 * 
 * Inputs
 * 1. cmd - Array of bytes
 * 2. prmSize - Number of valid bytes in cmd. 
 * 
 * Responds by sending the same command back to ITT Client (on PC)
 *
 */
void itt_ctrl_cmdHandlerEcho(char *cmd, uint32_t prmSize);

/**
 * \brief Handler for command "iss_read_2a_params". Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 *
 * Inputs
 * 1. cmd - Array of bytes in the format AlgItt_IssAewb2AParams
 * 2. prmSize - Number of valid bytes in cmd. 
 * 
 * Reads current state of AE and AWB and sends back to ITT Client (on PC)
 */
void itt_ctrl_cmdHandlerIssRead2AParams(char *cmd, uint32_t prmSize);

/**
 * \brief Handler for command "iss_write_2a_params". Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 *
 * Inputs
 * 1. cmd - Array of bytes in the format AlgItt_IssAewb2AParams
 * 2. prmSize - Number of valid bytes in cmd. 
 * 
 * Writes the data received from the client to 2A algorithms and sends back PASS/FAIL status to ITT Client (on PC)
 */
void itt_ctrl_cmdHandlerIssWrite2AParams(char *cmd, uint32_t prmSize);

/**
 * \brief Handler for commands "iss_raw_save" and "iss_yuv_save". Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 *
 * Inputs
 * 1. cmd - String value "iss_raw_save" OR "iss_yuv_save"
 * 2. prmSize - Number of valid bytes in cmd. 
 * 
 * Invokes appFileSaveCallback to capture RAW/YUV images. Send the image to ITT Client (on PC)
 */
void itt_ctrl_cmdHandlerIssFileSave(char *cmd, uint32_t prmSize);

/**
 * \brief Handler for command "iss_send_dcc_file". Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 * Reserved for future use
 * 
 */
void itt_ctrl_cmdHandlerIssDccSendFile(char *cmd, uint32_t prmSize);

/**
 * \brief Handler for command "iss_read_sensor_reg". Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 * Inputs
 * 1. cmd - 12-byte array containing
 * ----- 4-byte Channel ID
 * ----- 4-byte Sensor Register Address
 * ----- 4-byte Sensor Register Value. To be ignored
 * 2. prmSize - Number of valid bytes in cmd. 
 * 
 * Reads sensor register and sends back to ITT Client (on PC)
 */
void itt_ctrl_cmdHandlerIssReadSensorReg(char *cmd, uint32_t prmSize);

/**
 * \brief Handler for command "iss_write_sensor_reg". Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 * Inputs
 * 1. cmd - 12-byte array containing
 * ----- 4-byte Channel ID
 * ----- 4-byte Sensor Register Address
 * ----- 4-byte Sensor Register Value.
 * 2. prmSize - Number of valid bytes in cmd. 
 * 
 * Writes the value to specified sensor register and sends back status (PASS/FAIL) to ITT Client (on PC)
 */
void itt_ctrl_cmdHandlerIssWriteSensorReg(char *cmd, uint32_t prmSize);

/**
 * \brief Handler for command "dev_ctrl". Generic device control. Function callback passed to network_api utils. 
 * Must comply with the syntax of NetworkCtrl_Handler
 * 
 * Currently supports reading Deserializer, Serializer and Sensor registers in batch mode
 * Inputs
 * 1. cmd - Script specifying the commands to be executed
 * 2. prmSize - Number of valid bytes in cmd. 
 * 
 * Executes the script and sends the results (PASS/FAIL) to ITT Client (on PC) 
 */
void itt_ctrl_cmdHandlerIssDevCtrl(char *cmd, uint32_t prmSize);

/**
 * \brief Function used to register needed objects for EdgeAI ISP Live Tuning
 * 
 * Currenlty only two nodes are supported: VISS and LDC.
 * 
 */
int32_t itt_register_object(vx_context context,
                            vx_node *node,
                            tivx_raw_image *raw_image_handle,
                            vx_image *yuv_image_handle, 
                            uint8_t object_name);

/**
 * \brief initialization routine for ITT server on EdgeAI.
 *
 * This must be called from any application which needs to control ISP tuning at runtime.
 *
 */
int32_t itt_server_edge_ai_init(void);

/* @} */

#endif

