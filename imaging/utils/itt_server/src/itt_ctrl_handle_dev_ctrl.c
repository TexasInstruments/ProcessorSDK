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
#include "itt_priv.h"

static int32_t get_next_line(const char* buf, char* line_str, uint8_t max_line_len)
{
    int32_t i=0;
    int32_t result;

    result = 0;

    while(buf[i] != '#')
    {
        line_str[i] = buf[i];
        i++;

        if(i == (int32_t)max_line_len)
        {
            result = -1;
            break;
        }
    }

    if(result == 0)
    {
        line_str[i] = '\0';

        while(buf[i] != '\n')
        {
            i++;
            if(i == (int32_t)max_line_len)
            {
                result = -2;
                break;
            }
        }

        if(result == 0)
        {
            result = i+1;/*Including new line char*/
        }
    }

    return result;
}
static uint8_t cmd_param_dev_ctrl[CMD_PARAM_SIZE];
#define MAX_LINE_SIZE (128)
#define MAX_DEV_CTRL_BUF_SIZE (16U * 1024U)
static char g_line_str[MAX_LINE_SIZE];
static char g_line_str_out[MAX_LINE_SIZE];

void itt_ctrl_cmdHandlerIssDevCtrl(char *cmd, uint32_t prmSize)
{
    /*Generic device control*/
    /*Currently programmed to receive text and send the same text back*/
    /*This maybe enhanced in future to read/write a list of register values*/
    int32_t status;

    (void)cmd;
    char *prm;
    char *prm_out;
    uint32_t prmOutSize = 0;
    uint8_t * cmd_ptr;
    uint32_t * ptr32;
    char *ptr8;
    uint16_t chId;
    uint16_t devType;
    uint16_t i2cSlaveAddr = 0x32;
    uint16_t regAddr;
    int32_t count = 0;
    char *token;
    char s[]=" \t \n";
    IMAGE_SENSOR_CTRLCMD ctrlCmd = IMAGE_SENSOR_CTRLCMD_DEBUG;
    uint16_t bytesAvailable = 0;
    uint8_t max_line_size = MAX_LINE_SIZE;
    char dummy_name[ISS_SENSORS_MAX_NAME] = "dummy";
    int32_t should_continue = 1;
    static char prm_buf[MAX_DEV_CTRL_BUF_SIZE];
    static char prm_out_buf[MAX_DEV_CTRL_BUF_SIZE];

    if(prmSize > MAX_DEV_CTRL_BUF_SIZE)
    {
        appLogPrintf("Failed: Device control buffer size %u exceeds maximum %u bytes\n", prmSize, (uint32_t)MAX_DEV_CTRL_BUF_SIZE);
        should_continue = 0;
        /* send error response */
        (void)IttCtrl_writeParams(NULL, 0, (uint32_t)-1);
    }

    if(should_continue != 0)
    {
    prm = prm_buf;
    prm_out = prm_out_buf;
    ptr8 = prm;
    bytesAvailable = (uint16_t)prmSize;

    (void)IttCtrl_readParams((uint8_t *)prm, prmSize);

    //# Command (RD/WR)  CH_ID      DEVICE   REG_ADDR REG_VAL

    while( count >= 0)
    {
        count = get_next_line(ptr8, g_line_str, max_line_size);
        if (count > 0)
        {
            bytesAvailable -= (uint16_t)count;

            if(max_line_size > bytesAvailable)
            {
                max_line_size = (uint8_t)bytesAvailable;
            }
            token = strtok(g_line_str, s);
            if(NULL != token)
            {
                uint32_t rw_flag = 0xFF;

                if(strcmp(token, "WR")==0)
                {
                    rw_flag = 1;
                }
                else if(strcmp(token, "RD")==0)
                {
                    rw_flag = 0;
                }
                else
                {
                    rw_flag = 0; /*Unsupported command. Forcing to read*/
                    appLogPrintf("Illegal token %s \n", token);
                }

                token = strtok(NULL, s);
                if(NULL == token)
                {
                    appLogPrintf("Failed to read chId \n");
                    count = -1;
                }
                else
                {
                    chId = (uint16_t)strtol(token, NULL, 16);
                    appLogPrintf("chId = [0x%x]  ", chId);

                    token = strtok(NULL, s);
                    if(NULL == token)
                    {
                        appLogPrintf("Failed to read devType \n");
                        count = -1;
                    }
                    else
                    {
                        devType= (uint16_t)strtol(token, NULL, 16);
                        appLogPrintf("devType = [0x%x]  ", devType);

                        token = strtok(NULL, s);
                        if(NULL == token)
                        {
                            appLogPrintf("Failed to read regAddr \n");
                            count = -1;
                        }
                        else
                        {
                            regAddr = (uint16_t)strtol(token, NULL, 16);
                            appLogPrintf("register address = [0x%x]  ", regAddr);

                            token = strtok(NULL, s);
                            if(NULL == token)
                            {
                                appLogPrintf("Failed to read regValue \n");
                                count = -1;
                            }
                            else
                            {
                                (void)memset(cmd_param_dev_ctrl, 0xAB, CMD_PARAM_SIZE);
                                cmd_ptr = (uint8_t *)cmd_param_dev_ctrl;

                                (void)memcpy((void*)cmd_ptr, (const void*)dummy_name, strlen(dummy_name)+1U);
                                cmd_ptr = &cmd_ptr[ISS_SENSORS_MAX_NAME];

                                ptr32 = (uint32_t *)cmd_ptr;
                                *ptr32 = chId; /*channel ID */
                                cmd_ptr = &cmd_ptr[sizeof(uint32_t)];

                                (void)memcpy((void*)cmd_ptr, (const void*)&ctrlCmd, sizeof(IMAGE_SENSOR_CTRLCMD));
                                cmd_ptr = &cmd_ptr[sizeof(IMAGE_SENSOR_CTRLCMD)];

                                ptr32 = (uint32_t *)cmd_ptr;
                                if(1U == rw_flag)
                                {
                                    *ptr32 = 1U; /*ReadWrite Flag = 1 for Write*/
                                }
                                else
                                {
                                    *ptr32 = 0U; /*ReadWrite Flag = 0 for Read*/
                                }
                                cmd_ptr = &cmd_ptr[sizeof(uint32_t)];

                                ptr32 = (uint32_t *)cmd_ptr;
                                *ptr32 = devType; /*Device Type*/
                                cmd_ptr = &cmd_ptr[sizeof(uint32_t)];

                                ptr32 = (uint32_t *)cmd_ptr;
                                *ptr32 = regAddr; /*Register Address*/

                                status = appRemoteServiceRun(
                                    APP_IPC_RTOS_CPU ,
                                    IMAGE_SENSOR_REMOTE_SERVICE_NAME,
                                    (uint32_t)IM_SENSOR_CMD_CTL,
                                    (void*)cmd_param_dev_ctrl,
                                    CMD_PARAM_SIZE,
                                    0);

                                if(status != 0)
                                {
                                    appLogPrintf("Error : appRemoteServiceRun returned %d \n", status);
                                }
                                if(0U == rw_flag)
                                {
                                    ptr32 = (uint32_t *)cmd_param_dev_ctrl;
                                    (void)sprintf(g_line_str_out, "%x %x %x\n", i2cSlaveAddr, regAddr, *ptr32);
                                    (void)memcpy(&prm_out[prmOutSize], g_line_str_out, strlen(g_line_str_out));
                                    prmOutSize += (uint32_t)strlen(g_line_str_out);
                                }
                            }
                        }
                    }
                }
            }
            if(count > 0)
            {
                ptr8 = &ptr8[count];
            }
        }

        }

        /* send response */
        (void)IttCtrl_writeParams((uint8_t *)prm_out, prmOutSize, 0);
    }
}
