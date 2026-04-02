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
#include <app_remote_service.h>
#include <app_ipc.h>
#include <TI/hwa_viss_srvr_remote.h>
#include <vhwa/include/vhwa_m2mViss.h>

tivx_mutex             viss_aewb_lock[VHWA_M2M_VISS_MAX_HANDLES];
tivx_ae_awb_params_t   viss_aewb_results[VHWA_M2M_VISS_MAX_HANDLES];
uint32_t               viss_aewb_channel[VHWA_M2M_VISS_MAX_HANDLES];

int32_t VissServer_RemoteServiceHandler(char *service_name, uint32_t cmd,
    void *prm, uint32_t prm_size, uint32_t flags)
{
    int32_t status = 0;
    uint8_t * cmd_param = (uint8_t * )prm;
    uint8_t * ptr8 = (uint8_t * )prm;
    uint32_t chId = *ptr8;
    ptr8 += sizeof(uint32_t);
    tivx_ae_awb_params_t * pAewbPrms = (tivx_ae_awb_params_t * )ptr8;

    /* LDRA_JUSTIFY_START
    <metric start> branch <metric end>
    <justification start> 
    Rationale: The component level negative test framework and test applications cannot reach this portion.
    The test framework does not support the configuration required to trigger this error scenario.
    Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
    This behaviour is part of the application design. An error print statement can be added in a future release if required.
    <justification end> */
    switch(cmd)
    {
    /* LDRA_JUSTIFY_END */
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications CAN reach this portion.
                    No existing test case can reach this portion. A test case will be added to cover this gap in a future release.
        Effect on this unit: If the control reaches here, our code base is expected to perform clean-up operations.
                However, due to the stated rationale, this is not tested.
        <justification end> */
        case (uint32_t)VISS_CMD_GET_2A_PARAMS:
            memcpy((void *)cmd_param, (const void *)&viss_aewb_results[chId], sizeof(tivx_ae_awb_params_t));
            break;
         /* LDRA_JUSTIFY_END */
        case (uint32_t)VISS_CMD_SET_2A_PARAMS:
            status = tivxMutexLock(viss_aewb_lock[chId]);
	    /* LDRA_JUSTIFY_START
	    <metric start> branch <metric end>
	    <justification start>
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
	    <justification end> */
            if ((vx_status)VX_SUCCESS == status)
	    /* LDRA_JUSTIFY_END */
            {
                memcpy((void *)&viss_aewb_results[chId], (const void *)pAewbPrms, sizeof(tivx_ae_awb_params_t));
                status = tivxMutexUnlock(viss_aewb_lock[chId]);
            }
            break;
        /* LDRA_JUSTIFY_START
        <metric start> statement branch <metric end>
        <justification start> 
        Rationale: The component level negative test framework and test applications cannot reach this portion.
        The test framework does not support the configuration required to trigger this error scenario.
        Effect on this unit: The unit is NOT expected to result in an error because the branch statement is pre-validated by the application.
        This behaviour is part of the application design. An error print statement can be added in a future release if required.
        <justification end> */
        default:
            VX_PRINT(VX_ZONE_ERROR, "VissServer_RemoteServiceHandler : Invalid command %d\n", cmd);
            break;
        /* LDRA_JUSTIFY_END */
    }

    return status;
}

int32_t VissRemoteServer_Init(void)
{
    int32_t status = 0;
    int32_t i = 0;

    status = appRemoteServiceRegister(
        VISS_SERVER_REMOTE_SERVICE_NAME,
        VissServer_RemoteServiceHandler
    );

/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start>
Rationale: The component level negative test framework and test applications cannot reach this portion.
This failure case is out of scope for the imaging test framework.
Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
However, due to the stated rationale, this is not tested.
<justification end> */
    if(status!=0)
    {
        VX_PRINT(VX_ZONE_ERROR, " VissRemoteServer_Init: ERROR: Unable to register VISS remote service handler\n");
    }
    else
/* LDRA_JUSTIFY_END */
    {
        for(i=0;i<(int32_t)VHWA_M2M_VISS_MAX_HANDLES;i++)
        {
            memset(&viss_aewb_results[i], 0x0, sizeof(tivx_ae_awb_params_t));
            memset(&viss_aewb_channel[i], 0x0, sizeof(uint32_t));

            status = tivxMutexCreate(&(viss_aewb_lock[i]));
/* LDRA_JUSTIFY_START
<metric start> statement branch <metric end>
<justification start>
Rationale: The component level negative test framework and test applications cannot reach this portion.
This failure case is out of scope for the imaging test framework.
Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
However, due to the stated rationale, this is not tested.
<justification end> */
            if(status!=0)
            {
                VX_PRINT(VX_ZONE_ERROR, " VissRemoteServer_Init: ERROR: Unable to create mutex\n");
                break;
            }
/* LDRA_JUSTIFY_END */
        }
    }

    return status;
}

/* LDRA_JUSTIFY
<metric start> statement branch <metric end>
<function start> int32_t VissRemoteServer_DeInit.* <function end>
<justification start>
Rationale: The component level negative test framework and test applications cannot reach this portion.
This failure case is out of scope for the imaging test framework.
Effect on this unit: If the control reaches here, our code base is expected to accumulate the error status and return the same to the application.
However, due to the stated rationale, this is not tested.
<justification end> */
int32_t VissRemoteServer_DeInit(void)
{
    int32_t status = 0;
    int32_t i = 0;

    status = appRemoteServiceUnRegister(VISS_SERVER_REMOTE_SERVICE_NAME);

    if(status!=0)
    {
        VX_PRINT(VX_ZONE_ERROR, " VissRemoteServer_DeInit: Warning: Unable to unregister VISS remote service handler\n");
    }
    else
    {
        for(i=0;i<(int32_t)VHWA_M2M_VISS_MAX_HANDLES;i++)
        {
            status = tivxMutexDelete(&(viss_aewb_lock[i]));
            if(status!=0)
            {
                VX_PRINT(VX_ZONE_ERROR, " VissRemoteServer_DeInit: Warning: Unable to delete mutex\n");
            }
        }
    }

    return status;
}
