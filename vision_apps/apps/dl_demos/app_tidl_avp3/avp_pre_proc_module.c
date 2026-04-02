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

#include "avp_pre_proc_module.h"

vx_status app_init_pre_proc(vx_context context, PreProcObj *preProcObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    vx_enum config_type = VX_TYPE_INVALID;
    config_type = vxRegisterUserStruct(context, sizeof(tivxImgPreProcParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);

    preProcObj->config = vxCreateArray(context, config_type, 1);
    APP_ASSERT_VALID_REF(preProcObj->config);

    vx_char ref_name[APP_MAX_FILE_PATH];
    vxAddArrayItems(preProcObj->config, 1, &preProcObj->params, sizeof(tivxImgPreProcParams));
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_config", objName);
    vxSetReferenceName((vx_reference)preProcObj->config, ref_name);

    return status;
}

vx_status app_update_pre_proc(vx_context context, PreProcObj *preProcObj, vx_user_data_object config, vx_int32 tensor_idx)
{
    vx_status status = VX_SUCCESS;

    vx_size   input_sizes[APP_MAX_TENSOR_DIMS];
    vx_map_id map_id_config;
    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params *tidlParams;
    vx_tensor output_tensor;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                        (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;
    memcpy(&preProcObj->ioBufDesc, ioBufDesc, sizeof(sTIDL_IOBufDesc_t));

    vxUnmapUserDataObject(config, map_id_config);

    if((ioBufDesc->inElementType[tensor_idx] == TIDL_UnsignedChar) || (ioBufDesc->inElementType[tensor_idx] == TIDL_SignedChar))
    {
        preProcObj->params.tidl_8bit_16bit_flag = 0;
    }
    else if((ioBufDesc->inElementType[tensor_idx] == TIDL_UnsignedShort) || (ioBufDesc->inElementType[tensor_idx] == TIDL_SignedShort))
    {
        preProcObj->params.tidl_8bit_16bit_flag = 1;
    }

    preProcObj->params.pad_pixel[0] = ioBufDesc->inPadL[tensor_idx]; //Left
    preProcObj->params.pad_pixel[1] = ioBufDesc->inPadT[tensor_idx]; //Top
    preProcObj->params.pad_pixel[2] = ioBufDesc->inPadR[tensor_idx]; //Right
    preProcObj->params.pad_pixel[3] = ioBufDesc->inPadB[tensor_idx]; //Bottom

    preProcObj->num_input_tensors = ioBufDesc->numInputBuf;
    preProcObj->num_output_tensors = ioBufDesc->numOutputBuf;

    input_sizes[0] = ioBufDesc->inWidth[tensor_idx]  + ioBufDesc->inPadL[tensor_idx] + ioBufDesc->inPadR[tensor_idx];
    input_sizes[1] = ioBufDesc->inHeight[tensor_idx] + ioBufDesc->inPadT[tensor_idx] + ioBufDesc->inPadB[tensor_idx];
    input_sizes[2] = ioBufDesc->inNumChannels[tensor_idx];

    vx_enum data_type = get_vx_tensor_datatype(ioBufDesc->inElementType[tensor_idx]);
    output_tensor = vxCreateTensor(context, 3, input_sizes, data_type, 0);
    preProcObj->output_tensor_arr = vxCreateObjectArray(context, (vx_reference)output_tensor, NUM_CH);
    vxReleaseTensor(&output_tensor);

    return status;
}

void app_deinit_pre_proc(PreProcObj *preProcObj)
{
    vxReleaseArray(&preProcObj->config);
    vxReleaseObjectArray(&preProcObj->output_tensor_arr);
}

void app_delete_pre_proc(PreProcObj *preProcObj)
{
    if(preProcObj->node != NULL)
    {
        vxReleaseNode(&preProcObj->node);
    }
}

vx_status app_create_graph_pre_proc(vx_graph graph, PreProcObj *preProcObj, vx_object_array input_arr)
{
    vx_status status = VX_SUCCESS;

    vx_image  input   = (vx_image)vxGetObjectArrayItem((vx_object_array)input_arr, 0);
    vx_tensor output  = (vx_tensor)vxGetObjectArrayItem((vx_object_array)preProcObj->output_tensor_arr, 0);

    preProcObj->node = tivxImgPreProcNode(graph,
                                          preProcObj->config,
                                          input,
                                          output);

    APP_ASSERT_VALID_REF(preProcObj->node);

    status = vxSetNodeTarget(preProcObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP1);
    vxSetReferenceName((vx_reference)preProcObj->node, "PreProcNode");

    vx_bool replicate[] = {vx_false_e, vx_true_e, vx_true_e};
    vxReplicateNode(graph, preProcObj->node, replicate, 3);

    vxReleaseImage(&input);
    vxReleaseTensor(&output);

    return(status);
}

vx_status writePreProcOutput(char* file_name, vx_object_array output_arr)
{
    vx_status status = VX_SUCCESS;

    vx_tensor output;
    vx_size numCh;
    vx_int32 ch;

    vxQueryObjectArray((vx_object_array)output_arr, VX_OBJECT_ARRAY_NUMITEMS, &numCh, sizeof(vx_size));

    for(ch = 0; ch < numCh; ch++)
    {
        vx_size num_dims;
        void *data_ptr;
        vx_map_id map_id;

        vx_size    start[APP_MAX_TENSOR_DIMS];
        vx_size    tensor_strides[APP_MAX_TENSOR_DIMS];
        vx_size    tensor_sizes[APP_MAX_TENSOR_DIMS];

        output  = (vx_tensor)vxGetObjectArrayItem((vx_object_array)output_arr, ch);

        vxQueryTensor(output, VX_TENSOR_NUMBER_OF_DIMS, &num_dims, sizeof(vx_size));

        if(num_dims != 3)
        {
            printf("Number of dims are != 3! exiting.. \n");
            break;
        }

        vxQueryTensor(output, VX_TENSOR_DIMS, tensor_sizes, 3 * sizeof(vx_size));

        start[0] = start[1] = start[2] = 0;

        tensor_strides[0] = 1;
        tensor_strides[1] = tensor_strides[0];
        tensor_strides[2] = tensor_strides[1] * tensor_strides[1];

        status = tivxMapTensorPatch(output, num_dims, start, tensor_sizes, &map_id, tensor_strides, &data_ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

        if(VX_SUCCESS == status)
        {
            vx_char new_name[APP_MAX_FILE_PATH];
            snprintf(new_name, APP_MAX_FILE_PATH, "%s_%dx%d_ch%d.rgb", file_name, (uint32_t)tensor_sizes[0], (uint32_t)tensor_sizes[1], ch);

            FILE *fp = fopen(new_name, "wb");
            if(NULL == fp)
            {
                printf("Unable to open file %s for writing!\n", new_name);
                break;
            }

            fwrite(data_ptr, 1, tensor_sizes[0] * tensor_sizes[1] * tensor_sizes[2], fp);
            fclose(fp);

            tivxUnmapTensorPatch(output, map_id);
        }

        vxReleaseTensor(&output);
    }

    return(status);
}

vx_status app_update_dof_pre_proc(vx_context context, DofPreProcObj *dofPreProcObj, vx_user_data_object config, vx_int32 tensor_idx)
{
    vx_status status = VX_SUCCESS;

    vx_size   input_sizes[APP_MAX_TENSOR_DIMS];
    vx_map_id map_id_config;
    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params *tidlParams;
    vx_tensor output_tensor;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                        (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;
    memcpy(&dofPreProcObj->ioBufDesc, ioBufDesc, sizeof(sTIDL_IOBufDesc_t));

    vxUnmapUserDataObject(config, map_id_config);

    dofPreProcObj->params.pad_pixel[0] = ioBufDesc->inPadL[tensor_idx]; //Left
    dofPreProcObj->params.pad_pixel[1] = ioBufDesc->inPadT[tensor_idx]; //Top
    dofPreProcObj->params.pad_pixel[2] = ioBufDesc->inPadR[tensor_idx]; //Right
    dofPreProcObj->params.pad_pixel[3] = ioBufDesc->inPadB[tensor_idx]; //Bottom

    dofPreProcObj->num_input_tensors = ioBufDesc->numInputBuf;
    dofPreProcObj->num_output_tensors = ioBufDesc->numOutputBuf;

    input_sizes[0] = ioBufDesc->inWidth[tensor_idx]  + ioBufDesc->inPadL[tensor_idx] + ioBufDesc->inPadR[tensor_idx];
    input_sizes[1] = ioBufDesc->inHeight[tensor_idx] + ioBufDesc->inPadT[tensor_idx] + ioBufDesc->inPadB[tensor_idx];
    input_sizes[2] = ioBufDesc->inNumChannels[tensor_idx];

    vx_enum data_type = get_vx_tensor_datatype(ioBufDesc->inElementType[tensor_idx]);
    output_tensor = vxCreateTensor(context, 3, input_sizes, data_type, 0);
    dofPreProcObj->output_tensor_arr = vxCreateObjectArray(context, (vx_reference)output_tensor, NUM_CH);
    vxReleaseTensor(&output_tensor);

    return status;
}

vx_status app_init_dof_pre_proc(vx_context context, DofPreProcObj *dofPreProcObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    if(status == VX_SUCCESS)
    {
        vx_user_data_object config = vxCreateUserDataObject(context, "", sizeof(tivxDofPlaneSepParams), NULL);
        dofPreProcObj->config_arr = vxCreateObjectArray(context, (vx_reference)config, NUM_CH);
        status = vxGetStatus((vx_reference)dofPreProcObj->config_arr);
        vxReleaseUserDataObject(&config);
    }

    if(status == VX_SUCCESS)
    {
        vx_user_data_object config = (vx_user_data_object)vxGetObjectArrayItem(dofPreProcObj->config_arr, 0);
        dofPreProcObj->params.skip_flag = 0;
        status = vxCopyUserDataObject(config, 0, sizeof(tivxDofPlaneSepParams),\
                &dofPreProcObj->params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        vxReleaseUserDataObject(&config);
    }
    if(status == VX_SUCCESS)
    {
        vx_user_data_object config = (vx_user_data_object)vxGetObjectArrayItem(dofPreProcObj->config_arr, 1);
        dofPreProcObj->params.skip_flag = 0;
        status = vxCopyUserDataObject(config, 0, sizeof(tivxDofPlaneSepParams),\
                &dofPreProcObj->params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        vxReleaseUserDataObject(&config);
    }
    if(status == VX_SUCCESS)
    {
        vx_user_data_object config = (vx_user_data_object)vxGetObjectArrayItem(dofPreProcObj->config_arr, 2);
        dofPreProcObj->params.skip_flag = 0;
        status = vxCopyUserDataObject(config, 0, sizeof(tivxDofPlaneSepParams),\
                &dofPreProcObj->params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        vxReleaseUserDataObject(&config);
    }

    return status;
}

void app_deinit_dof_pre_proc(DofPreProcObj *dofPreProcObj)
{
    vxReleaseObjectArray(&dofPreProcObj->config_arr);
    vxReleaseObjectArray(&dofPreProcObj->output_tensor_arr);
}

void app_delete_dof_pre_proc(DofPreProcObj *dofPreProcObj)
{
    if(dofPreProcObj->node != NULL)
    {
        vxReleaseNode(&dofPreProcObj->node);
    }
}

vx_status app_create_graph_dof_pre_proc(vx_graph graph, DofPreProcObj *dofPreProcObj, vx_object_array input_arr)
{
    vx_status status = VX_SUCCESS;

    vx_user_data_object config = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)dofPreProcObj->config_arr, 0);
    vx_image  input   = (vx_image)vxGetObjectArrayItem((vx_object_array)input_arr, 0);
    vx_tensor output  = (vx_tensor)vxGetObjectArrayItem((vx_object_array)dofPreProcObj->output_tensor_arr, 0);

    dofPreProcObj->node = tivxDofPlaneSepNode(graph,
                                              config,
                                              input,
                                              output);

    APP_ASSERT_VALID_REF(dofPreProcObj->node);

    status = vxSetNodeTarget(dofPreProcObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP1);
    vxSetReferenceName((vx_reference)dofPreProcObj->node, "DofPreProcNode");

    vx_bool replicate[] = {vx_true_e, vx_true_e, vx_true_e, vx_false_e};
    vxReplicateNode(graph, dofPreProcObj->node, replicate, 4);

    vxReleaseUserDataObject(&config);
    vxReleaseImage(&input);
    vxReleaseTensor(&output);

    return status;
}
