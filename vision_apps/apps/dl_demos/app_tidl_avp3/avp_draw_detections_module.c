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

#include "avp_draw_detections_module.h"

vx_status app_init_draw_detections(vx_context context, DrawDetectionsObj *drawDetectionsObj, char *objName)
{
    vx_status status = VX_SUCCESS;
    vx_char ref_name[APP_MAX_FILE_PATH];

    drawDetectionsObj->config = vxCreateUserDataObject(context, "", sizeof(tivxDrawKeypointDetectionsParams), NULL);
    status = vxGetStatus((vx_reference)drawDetectionsObj->config);
    if(status == VX_SUCCESS)
    {
        status = vxCopyUserDataObject(drawDetectionsObj->config, 0, sizeof(tivxDrawKeypointDetectionsParams),\
                    &drawDetectionsObj->params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

        snprintf(ref_name, APP_MAX_FILE_PATH, "%s_config", objName);
        vxSetReferenceName((vx_reference)drawDetectionsObj->config, ref_name);
    }
    if(status == VX_SUCCESS)
    {
        vx_image output = vxCreateImage(context, drawDetectionsObj->params.dl_width, drawDetectionsObj->params.dl_height, VX_DF_IMAGE_NV12);
        drawDetectionsObj->output_image_arr = vxCreateObjectArray(context, (vx_reference)output, NUM_CH);
        vxReleaseImage(&output);
    }

    return status;
}

vx_status app_update_draw_detections(DrawDetectionsObj *drawDetectionsObj, vx_user_data_object config)
{
    vx_status status = VX_SUCCESS;

    vx_map_id map_id_config;
    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params *tidlParams;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                        (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;
    memcpy(&drawDetectionsObj->params.ioBufDesc, ioBufDesc, sizeof(sTIDL_IOBufDesc_t));

    vxUnmapUserDataObject(config, map_id_config);

    return status;
}

void app_deinit_draw_detections(DrawDetectionsObj *drawDetectionsObj)
{
    vxReleaseUserDataObject(&drawDetectionsObj->config);
    vxReleaseObjectArray(&drawDetectionsObj->output_image_arr);
}

void app_delete_draw_detections(DrawDetectionsObj *drawDetectionsObj)
{
    if(drawDetectionsObj->node != NULL)
    {
        vxReleaseNode(&drawDetectionsObj->node);
    }
}

vx_status app_create_graph_draw_detections(vx_graph graph,
                                           DrawDetectionsObj *drawDetectionsObj,
                                           vx_object_array    kp_tensor_arr,
                                           vx_object_array    kp_valid_arr,
                                           vx_object_array    input_tensor_arr,
                                           vx_object_array    input_image_arr)
{
    vx_status status = VX_SUCCESS;

    vx_tensor kp_tensor    = (vx_tensor)vxGetObjectArrayItem((vx_object_array)kp_tensor_arr, 0);
    vx_tensor kp_valid     = (vx_tensor)vxGetObjectArrayItem((vx_object_array)kp_valid_arr, 0);
    vx_tensor input_tensor = (vx_tensor)vxGetObjectArrayItem((vx_object_array)input_tensor_arr, 0);
    vx_image  input_image  = (vx_image)vxGetObjectArrayItem((vx_object_array)input_image_arr, 0);
    vx_image  output_image = (vx_image)vxGetObjectArrayItem((vx_object_array)drawDetectionsObj->output_image_arr, 0);

    drawDetectionsObj->node = tivxDrawKeypointDetectionsNode(graph,
                                                             drawDetectionsObj->config,
                                                             kp_tensor,
                                                             kp_valid,
                                                             input_tensor,
                                                             input_image,
                                                             output_image);

    APP_ASSERT_VALID_REF(drawDetectionsObj->node);
    status = vxSetNodeTarget(drawDetectionsObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP1);
    vxSetReferenceName((vx_reference)drawDetectionsObj->node, "DrawDetectionsNode");

    vx_bool replicate[] = {vx_false_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e};
    vxReplicateNode(graph, drawDetectionsObj->node, replicate, 6);

    vxReleaseTensor(&kp_tensor);
    vxReleaseTensor(&kp_valid);
    vxReleaseTensor(&input_tensor);
    vxReleaseImage(&input_image);
    vxReleaseImage(&output_image);

    return(status);
}
