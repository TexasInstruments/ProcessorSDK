/*
 *
 * Copyright (c) 2017 Texas Instruments Incorporated
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
#include "dof_display_module.h"
#include "dof_common.h"

vx_status app_init_display1(vx_context context, DisplayObj *displayObj, char *objName)
{
  vx_status status = VX_SUCCESS;

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (1 == displayObj->display_option))
    {
        memset(&displayObj->output_display_params, 0, sizeof(tivx_display_params_t));
        displayObj->output_display_config = vxCreateUserDataObject(context, "tivx_display_params_t",
             sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(displayObj->output_display_config);

        vxSetReferenceName((vx_reference)displayObj->output_display_config, "OutputDisplayConfiguration");

        displayObj->output_display_params.opMode=TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        #if defined(SOC_J722S)
        displayObj->output_display_params.pipeId = 1;
        displayObj->output_display_params.outWidth = displayObj->width;
        displayObj->output_display_params.outHeight = displayObj->height;
        displayObj->output_display_params.posX = (1920-displayObj->width);
        displayObj->output_display_params.posY = (1080-displayObj->height)/2 - 80;
        #else
        displayObj->output_display_params.pipeId = 0;
        displayObj->output_display_params.outWidth = OUTPUT_DISPLAY_WIDTH;
        displayObj->output_display_params.outHeight = OUTPUT_DISPLAY_HEIGHT;
        displayObj->output_display_params.posX = (1920-OUTPUT_DISPLAY_WIDTH);
        displayObj->output_display_params.posY = (1080-OUTPUT_DISPLAY_HEIGHT)/2 - 80;
        #endif

        status = vxCopyUserDataObject(displayObj->output_display_config, 0, sizeof(tivx_display_params_t), &displayObj->output_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(status==VX_SUCCESS);
    }

  return status;
}

vx_status app_init_display2(vx_context context, DisplayObj *displayObj, char *objName)
{
  vx_status status = VX_SUCCESS;

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2) && (1 == displayObj->display_option))
    {
        memset(&displayObj->input_display_params, 0, sizeof(tivx_display_params_t));
        displayObj->input_display_config = vxCreateUserDataObject(context, "tivx_display_params_t",
             sizeof(tivx_display_params_t), NULL);
        APP_ASSERT_VALID_REF(displayObj->input_display_config);

        vxSetReferenceName((vx_reference)displayObj->input_display_config, "InputDisplayConfiguration");

        displayObj->input_display_params.opMode=TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
        #if defined(SOC_J722S)
        displayObj->input_display_params.pipeId = 0;
        #else
        displayObj->input_display_params.pipeId = 2;
        #endif
        displayObj->input_display_params.outWidth = INPUT_DISPLAY_WIDTH;
        displayObj->input_display_params.outHeight = INPUT_DISPLAY_HEIGHT;
        displayObj->input_display_params.posX = 0;
        displayObj->input_display_params.posY = (1080-INPUT_DISPLAY_HEIGHT)/2 - 80;

        status = vxCopyUserDataObject(displayObj->input_display_config, 0, sizeof(tivx_display_params_t), &displayObj->input_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        APP_ASSERT(status==VX_SUCCESS);
    }
  return status;
}
void app_deinit_display1(DisplayObj *displayObj)
{
    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (displayObj->display_option == 1))
    {
        vxReleaseUserDataObject(&displayObj->output_display_config);
    }
}

void app_deinit_display2(DisplayObj *displayObj)
{
    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2) && (displayObj->display_option == 1))
    {
        vxReleaseUserDataObject(&displayObj->input_display_config);
    }

}

void app_delete_display1(DisplayObj *displayObj)
{

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (displayObj->display_option == 1))
    {
        vxReleaseNode(&displayObj->node1);
    }

}
void app_delete_display2(DisplayObj *displayObj)
{
    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2) && (displayObj->display_option == 1))
    {
        vxReleaseNode(&displayObj->node2);
    }
}

vx_status app_create_graph_display1(vx_graph graph, DisplayObj *displayObj, vx_image disp_image)
{
    vx_status status = VX_SUCCESS;

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (displayObj->display_option == 1))
    {
        displayObj->node1 = tivxDisplayNode(
            graph,
            displayObj->output_display_config,
            disp_image);
        status = vxGetStatus((vx_reference)displayObj->node1);
        if (VX_SUCCESS != status)
        {
            printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        APP_ASSERT(VX_SUCCESS == status);
        status = vxSetNodeTarget(displayObj->node1, VX_TARGET_STRING, TIVX_TARGET_DISPLAY1);
        APP_ASSERT(status==VX_SUCCESS);
        vxSetReferenceName((vx_reference)displayObj->node1, "Output Display");
    }

    return status;
}

vx_status app_create_graph_display2(vx_graph graph, DisplayObj *displayObj, vx_image disp_image)
{
    vx_status status = VX_SUCCESS;

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2) && (displayObj->display_option == 1))
    {
      displayObj->node2  = tivxDisplayNode(
            graph,
            displayObj->input_display_config,
            disp_image);
        status = vxGetStatus((vx_reference)displayObj->node2);
        if (VX_SUCCESS != status)
        {
            printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
        }
        APP_ASSERT(VX_SUCCESS == status);
        status = vxSetNodeTarget(displayObj->node2, VX_TARGET_STRING, TIVX_TARGET_DISPLAY2);
        APP_ASSERT(status==VX_SUCCESS);
        vxSetReferenceName((vx_reference)displayObj->node2, "Input Display");
    }
    return status;
}

