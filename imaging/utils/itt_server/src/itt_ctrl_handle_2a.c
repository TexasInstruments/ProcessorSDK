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
static uint8_t cmd_param_aewb[CMD_PARAM_SIZE];
extern uint8_t gEdgeAI;

void itt_ctrl_cmdHandlerIssRead2AParams(char *cmd, uint32_t prmSize)
{
    (void)cmd;
    int32_t status;
    uint8_t * cmd_ptr;
    AlgItt_IssAewb2AControlParams aewb2ACtrlPrms;
    uint8_t app_ipc_cpu;

    #ifdef ENABLE_DEBUG_ITTSERVER
    appLogPrintf("itt_ctrl_cmdHandlerIssRead2AParams\n");
    #endif

    (void)memset(&aewb2ACtrlPrms, 0, sizeof(AlgItt_IssAewb2AControlParams));

    if (prmSize == sizeof(AlgItt_IssAewb2AParams))
    {
        /* read parameters */
        (void)IttCtrl_readParams((uint8_t *)(&aewb2ACtrlPrms.aewb2APrms), prmSize);

        #ifdef ENABLE_DEBUG_ITTSERVER
        appLogPrintf("cmd = %d\n", aewb2ACtrlPrms.baseClassControl.controlCmd);
        appLogPrintf("size = %d\n", aewb2ACtrlPrms.baseClassControl.size);
        appLogPrintf("ae mode = %d\n", aewb2ACtrlPrms.aewb2APrms.aeMode);
        appLogPrintf("awb mode = %d\n", aewb2ACtrlPrms.aewb2APrms.awbMode);
        appLogPrintf("digitalGain = %d\n", aewb2ACtrlPrms.aewb2APrms.digitalGain);
        appLogPrintf("analogGain = %d\n", aewb2ACtrlPrms.aewb2APrms.analogGain);
        #endif

        (void)memset(cmd_param_aewb, 0xAB, CMD_PARAM_SIZE);
        cmd_ptr = (uint8_t *)cmd_param_aewb;
        *cmd_ptr = 0; /*channel ID*/

        app_ipc_cpu = (0U != gEdgeAI) ? APP_IPC_CPU_MPU1_0 : APP_IPC_RTOS_CPU;

        status = appRemoteServiceRun(
            app_ipc_cpu,
            AEWB_SERVER_REMOTE_SERVICE_NAME,
            (uint32_t)AEWB_CMD_GET_2A_PARAMS,
            (void*)cmd_param_aewb,
            CMD_PARAM_SIZE,
            0
        );

        if(status != 0)
        {
            appLogPrintf("Error : appRemoteServiceRun returned %d \n", status);
        }
        else
        {
            /*AEWB server copies 2A param struct to the beginning of the command buffer*/
            cmd_ptr = (uint8_t *)cmd_param_aewb;
            (void)memcpy((void*)&aewb2ACtrlPrms, (const void*)cmd_ptr, sizeof(AlgItt_IssAewb2AControlParams));
        }

        /* send response */
        (void)IttCtrl_writeParams((uint8_t *)&aewb2ACtrlPrms.aewb2APrms, (uint32_t)sizeof(AlgItt_IssAewb2AParams), 0);
    }
    else
    {
        appLogPrintf(" %s Error: Insufficient parameters (%d bytes) specified !!!\n", __FUNCTION__, prmSize);
    }
}

void itt_ctrl_cmdHandlerIssWrite2AParams(char *cmd, uint32_t prmSize)
{
    (void)cmd;
    int32_t status;
    uint8_t * cmd_ptr;
    AlgItt_IssAewb2AControlParams aewb2ACtrlPrms;
    uint8_t app_ipc_cpu;

    (void)memset(&aewb2ACtrlPrms, 0, sizeof(AlgItt_IssAewb2AControlParams));

    if (prmSize == sizeof(AlgItt_IssAewb2AParams))
    {
        /* read parameters */
        (void)IttCtrl_readParams((uint8_t *)(&aewb2ACtrlPrms.aewb2APrms), prmSize);

        aewb2ACtrlPrms.baseClassControl.controlCmd = (uint32_t)AEWB_CMD_SET_2A_PARAMS;
        aewb2ACtrlPrms.baseClassControl.size = (uint32_t)sizeof(aewb2ACtrlPrms);

        #ifdef ENABLE_DEBUG_ITTSERVER
        appLogPrintf("itt_ctrl_cmdHandlerIssWrite2AParams: cmd = %d\n", aewb2ACtrlPrms.baseClassControl.controlCmd);
        appLogPrintf("itt_ctrl_cmdHandlerIssWrite2AParams: size = %d\n", aewb2ACtrlPrms.baseClassControl.size);
        appLogPrintf("itt_ctrl_cmdHandlerIssWrite2AParams: ae mode = %d\n", aewb2ACtrlPrms.aewb2APrms.aeMode);
        appLogPrintf("itt_ctrl_cmdHandlerIssWrite2AParams: awb mode = %d\n", aewb2ACtrlPrms.aewb2APrms.awbMode);
        appLogPrintf("itt_ctrl_cmdHandlerIssWrite2AParams: digitalGain = %d\n", aewb2ACtrlPrms.aewb2APrms.digitalGain);
        appLogPrintf("itt_ctrl_cmdHandlerIssWrite2AParams: analogGain = %d\n", aewb2ACtrlPrms.aewb2APrms.analogGain);
        #endif

        (void)memset(cmd_param_aewb, 0xAB, CMD_PARAM_SIZE);
        cmd_ptr = (uint8_t *)cmd_param_aewb;
        *cmd_ptr = 0; /*channel ID*/
        cmd_ptr++;
        (void)memcpy((void*)cmd_ptr, (const void*)&aewb2ACtrlPrms, sizeof(AlgItt_IssAewb2AControlParams));

        app_ipc_cpu = (0U != gEdgeAI) ? APP_IPC_CPU_MPU1_0 : APP_IPC_RTOS_CPU;

        status = appRemoteServiceRun(
            app_ipc_cpu ,
            AEWB_SERVER_REMOTE_SERVICE_NAME,
            (uint32_t)AEWB_CMD_SET_2A_PARAMS,
            (void*)cmd_param_aewb,
            CMD_PARAM_SIZE,
            0
        );
        
        if(status != 0)
        {
            appLogPrintf("Error : appRemoteServiceRun returned %d \n", status);
        }
        /* send response */
        (void)IttCtrl_writeParams(NULL, 0, 0);
    }
    else
    {
        appLogPrintf(" %s: Insufficient parameters (%d bytes) specified !!!\n", __FUNCTION__, prmSize);
    }
}
