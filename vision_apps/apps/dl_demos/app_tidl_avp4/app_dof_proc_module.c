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
#include "app_dof_proc_module.h"

vx_status app_init_dof_proc(vx_context context, DofProcObj *dofProcObj, char *objName, vx_uint32 num_ch)
{
    vx_status status = VX_SUCCESS;
    vx_user_data_object dof_config;
    vx_int32 ch;

    vx_image flow_vector_field_exemplar;

    tivx_dmpac_dof_params_init(&dofProcObj->dof_params);

    if(dofProcObj->enable_temporal_predicton_flow_vector == 0)
    {
        dofProcObj->dof_params.base_predictor[0] = TIVX_DMPAC_DOF_PREDICTOR_DELAY_LEFT;
        dofProcObj->dof_params.base_predictor[1] = TIVX_DMPAC_DOF_PREDICTOR_PYR_COLOCATED;

        dofProcObj->dof_params.inter_predictor[0] = TIVX_DMPAC_DOF_PREDICTOR_DELAY_LEFT;
        dofProcObj->dof_params.inter_predictor[1] = TIVX_DMPAC_DOF_PREDICTOR_PYR_COLOCATED;
    }

    dof_config = vxCreateUserDataObject(context, "tivx_dmpac_dof_params_t",
                                             sizeof(tivx_dmpac_dof_params_t), NULL);
    status = vxGetStatus((vx_reference)dof_config);

    if(status == VX_SUCCESS)
    {
        dofProcObj->config_arr = vxCreateObjectArray(context, (vx_reference) dof_config, num_ch);
        vxReleaseUserDataObject(&dof_config);
        status = vxGetStatus((vx_reference)dofProcObj->config_arr);
    }

    if(status == VX_SUCCESS)
    {
        dofProcObj->dof_params.vertical_search_range[0] = 32;
        dofProcObj->dof_params.vertical_search_range[1] = 62;
        dofProcObj->dof_params.horizontal_search_range = 191;
        dofProcObj->dof_params.median_filter_enable = 1;
        dofProcObj->dof_params.motion_smoothness_factor = 24;
    }

    for(ch = 0; ch < num_ch; ch++)
    {
        dofProcObj->dof_params.motion_direction = 3; /* 0: for side camera */

        if(ch == 0) /* In the YUV input ch0 - front, ch1 - right, ch2 - left */
            dofProcObj->dof_params.motion_direction = 1; /* 1: for front camera */

        dof_config = (vx_user_data_object) vxGetObjectArrayItem( dofProcObj->config_arr, ch);
        status = vxCopyUserDataObject(dof_config, 0, sizeof(tivx_dmpac_dof_params_t), &dofProcObj->dof_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        vxReleaseUserDataObject(&dof_config);
    }

    //Creating flow vector delays
    if(status == VX_SUCCESS)
    {
        flow_vector_field_exemplar = vxCreateImage(context, dofProcObj->width, dofProcObj->height, VX_DF_IMAGE_U32);
        status  = vxGetStatus((vx_reference)flow_vector_field_exemplar);
    }

    if(status == VX_SUCCESS)
    {
        dofProcObj->flow_vector_field_array = vxCreateObjectArray(context, (vx_reference)flow_vector_field_exemplar, num_ch);
        status  = vxGetStatus((vx_reference)dofProcObj->flow_vector_field_array);
    }

    if(dofProcObj->enable_temporal_predicton_flow_vector == 1)
    {
        if(status == VX_SUCCESS)
        {
            dofProcObj->flow_vector_field_delay = vxCreateDelay(context, (vx_reference)dofProcObj->flow_vector_field_array, 2);
            status  = vxGetStatus((vx_reference)dofProcObj->flow_vector_field_delay);
        }
    }
    else
    {
        dofProcObj->flow_vector_field_delay = NULL;
    }

    vxReleaseImage(&flow_vector_field_exemplar);

    /*Creating default input */
    vx_pyramid pyramid_exemplar;
    vx_object_array pyramid_array;

    pyramid_exemplar = vxCreatePyramid(context,
                        dofProcObj->dof_levels, VX_SCALE_PYRAMID_HALF,
                        dofProcObj->width, dofProcObj->height,
                        dofProcObj->vx_df_pyramid);

    status  = vxGetStatus((vx_reference)pyramid_exemplar);

    if(status == VX_SUCCESS)
    {
        pyramid_array = vxCreateObjectArray(context, (vx_reference)pyramid_exemplar, num_ch);
        status  = vxGetStatus((vx_reference)pyramid_array);
    }
    else
    {
        printf("Unable to create pyramid examplar!\n");
    }

    if(status == VX_SUCCESS)
    {
        dofProcObj->pyramid_delay = vxCreateDelay(context, (vx_reference)pyramid_array, 2);
        status  = vxGetStatus((vx_reference)dofProcObj->pyramid_delay);
    }
    else
    {
        printf("Unable to create pyramid object array!\n");
    }

    if(status == VX_FAILURE)
    {
        printf("Unable to create pyramid delay!\n");
    }

    vxReleasePyramid(&pyramid_exemplar);
    vxReleaseObjectArray(&pyramid_array);


    return status;
}

void app_deinit_dof_proc(DofProcObj *dofProcObj)
{
    vxReleaseObjectArray(&dofProcObj->config_arr);
    vxReleaseObjectArray(&dofProcObj->flow_vector_field_array);

    if(dofProcObj->enable_temporal_predicton_flow_vector == 1)
    {
        vxReleaseDelay(&dofProcObj->flow_vector_field_delay);
    }

    vxReleaseDelay(&dofProcObj->pyramid_delay);
}


void app_delete_dof_proc(DofProcObj *dofProcObj)
{
    if(dofProcObj->node != NULL)
    {
        vxReleaseNode(&dofProcObj->node);
    }
}


vx_status app_create_graph_dof_proc(vx_graph graph, DofProcObj *dofProcObj, vx_delay pyramid_delay)
{
    vx_status status = VX_SUCCESS;

    vx_image flow_vector_field_in;
    vx_image flow_vector_field_out;

    vx_user_data_object dof_config = (vx_user_data_object) vxGetObjectArrayItem((vx_object_array)dofProcObj->config_arr,0);

    vx_object_array pyr_cur_array;
    vx_object_array pyr_ref_array;

    if(pyramid_delay == NULL)
    {
        pyr_cur_array = (vx_object_array)vxGetReferenceFromDelay(dofProcObj->pyramid_delay, 0);
        pyr_ref_array = (vx_object_array)vxGetReferenceFromDelay(dofProcObj->pyramid_delay, -1);
    }
    else
    {
        pyr_cur_array = (vx_object_array)vxGetReferenceFromDelay(pyramid_delay, 0);
        pyr_ref_array = (vx_object_array)vxGetReferenceFromDelay(pyramid_delay, -1);
    }

    vx_pyramid pyr_cur = (vx_pyramid)vxGetObjectArrayItem((vx_object_array)pyr_cur_array, 0);
    vx_pyramid pyr_ref = (vx_pyramid)vxGetObjectArrayItem((vx_object_array)pyr_ref_array, 0);

    if(dofProcObj->enable_temporal_predicton_flow_vector == 1)
    {
        vx_object_array flow_vector_field_in_array  = (vx_object_array)vxGetReferenceFromDelay(dofProcObj->flow_vector_field_delay, -1);
        flow_vector_field_in  = (vx_image)vxGetObjectArrayItem(flow_vector_field_in_array, 0);

        vx_object_array flow_vector_field_out_array = (vx_object_array)vxGetReferenceFromDelay(dofProcObj->flow_vector_field_delay,  0);
        flow_vector_field_out = (vx_image)vxGetObjectArrayItem(flow_vector_field_out_array, 0);
    }
    else
    {
        flow_vector_field_in  = NULL;
        flow_vector_field_out = (vx_image)vxGetObjectArrayItem(dofProcObj->flow_vector_field_array, 0);
    }

    dofProcObj->node = tivxDmpacDofNode(graph,
                                        dof_config,
                                        NULL,
                                        NULL,
                                        pyr_cur,
                                        pyr_ref,
                                        flow_vector_field_in,
                                        NULL,
                                        NULL,
                                        flow_vector_field_out,
                                        NULL);

    status = vxGetStatus((vx_reference)dofProcObj->node);

    if(status == VX_SUCCESS)
    {
        vxSetReferenceName((vx_reference)dofProcObj->node, "DenseOpticalFlow");
        status = vxSetNodeTarget(dofProcObj->node, VX_TARGET_STRING, TIVX_TARGET_DMPAC_DOF);
    }

    if(status == VX_SUCCESS)
    {
        vx_bool replicate[] = { vx_true_e, vx_false_e, vx_false_e, vx_true_e, vx_true_e, vx_true_e, vx_false_e, vx_false_e, vx_true_e, vx_false_e};
        vxReplicateNode(graph, dofProcObj->node, replicate, 10);
    }

    vxReleaseUserDataObject(&dof_config);
    vxReleasePyramid(&pyr_cur);
    vxReleasePyramid(&pyr_ref);

    if(dofProcObj->enable_temporal_predicton_flow_vector == 1)
    {
        vxReleaseImage(&flow_vector_field_in);
    }
    vxReleaseImage(&flow_vector_field_out);

    return status;
}
