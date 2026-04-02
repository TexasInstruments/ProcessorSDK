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

#include "app_pose_calc_module.h"

static void createInputOutputTensors(vx_context context, PoseCalcObj *poseCalcObj);
static vx_int32 fill1DTensor(vx_tensor in_tensor,vx_char* in_dof_file);
static vx_int32 fill1DTensorFrom2Bin(vx_tensor in_tensor, vx_char* in_file1, vx_char* in_file2);

vx_status set_pose_calc_defaults(PoseCalcObj *poseCalcObj)
{
    vx_status status = VX_SUCCESS;

    memset(&poseCalcObj->params, 0, sizeof(tivxVisualLocalizationParams));

    poseCalcObj->params.voxel_size[0] = 3;
    poseCalcObj->params.voxel_size[1] = 3;
    poseCalcObj->params.voxel_size[2] = 3;

    poseCalcObj->params.map_range[0][0] = -250;
    poseCalcObj->params.map_range[0][1] = 250;
    poseCalcObj->params.map_range[1][0] = -5;
    poseCalcObj->params.map_range[1][1] = 5;
    poseCalcObj->params.map_range[2][0] = -250;
    poseCalcObj->params.map_range[2][1] = 250;

    poseCalcObj->params.max_feat_match = 50;
    poseCalcObj->params.max_map_feat = 5000;

    /*Scale of last layer convolution weights in power of 2. This information is present in the file tidl_net_onnx_tiad_jdakaze_pw2.bin_paramDebug.csv
    this csv files gets gnerated while importing the onnx model through TIDL.
    */
    poseCalcObj->params.filter_scale_pw2 = 8; // 256

    /*This is the scale of descriptor in higher resolution. Data range can be found through onnx model, and float data range is [-256 to 256],
      that is why scale is 0.5 ( -1 in power of 2) to make it fixed point range in [-128 to 127] to fit in 8 signed bit
    */
    /*Acctual scale is 0.5, but wanted it to give in orignial scale. It is same to score buffer scale*/
    poseCalcObj->params.hi_res_desc_scale_pw2      = -1;

    poseCalcObj->params.lens_dist_table_size = 655;
    poseCalcObj->params.score_th = 128;

    poseCalcObj->params.dl_width    = 768;
    poseCalcObj->params.dl_height   = 384;
    poseCalcObj->params.img_width   = 1280;
    poseCalcObj->params.img_height  = 720;

    poseCalcObj->params.is_ip_fe    = 0;

    poseCalcObj->params.fx = 1024.0;
    poseCalcObj->params.fy = 1024.0;
    poseCalcObj->params.cx = 1024.0;
    poseCalcObj->params.cy = 512.0;

    return(status);
}

vx_status app_init_pose_calc(vx_context context, PoseCalcObj *poseCalcObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    vx_enum config_type = VX_TYPE_INVALID;
    config_type = vxRegisterUserStruct(context, sizeof(tivxVisualLocalizationParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);

    poseCalcObj->config = vxCreateArray(context, config_type, 1);
    APP_ASSERT_VALID_REF(poseCalcObj->config);

    vx_char ref_name[APP_MAX_FILE_PATH];
    vxAddArrayItems(poseCalcObj->config, 1, &poseCalcObj->params, sizeof(tivxVisualLocalizationParams));
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_config", objName);
    vxSetReferenceName((vx_reference)poseCalcObj->config, ref_name);

    createInputOutputTensors(context, poseCalcObj);

    return status;
}

vx_status app_update_pose_calc(vx_context context, PoseCalcObj *poseCalcObj)
{
    vx_status status = VX_SUCCESS;

    return status;
}

void app_deinit_pose_calc(PoseCalcObj *poseCalcObj)
{
    vxReleaseArray(&poseCalcObj->config);
    vxReleaseObjectArray(&poseCalcObj->output_tensor_arr[0]);
    vxReleaseObjectArray(&poseCalcObj->input_tensor_arr[0]);
    vxReleaseObjectArray(&poseCalcObj->input_tensor_arr[1]);
    vxReleaseObjectArray(&poseCalcObj->input_tensor_arr[2]);
    vxReleaseObjectArray(&poseCalcObj->input_tensor_arr[3]);
    vxReleaseObjectArray(&poseCalcObj->input_tensor_arr[4]);
}

void app_delete_pose_calc(PoseCalcObj *poseCalcObj)
{
    if(poseCalcObj->node != NULL)
    {
        vxReleaseNode(&poseCalcObj->node);
    }
}

vx_status app_create_graph_pose_calc(vx_graph graph, PoseCalcObj *poseCalcObj, vx_object_array* input_arr,
                                      vx_object_array* tidl_output_arr, vx_object_array* tidl_out_args)
{
    vx_status status = VX_SUCCESS;

    vx_tensor voxel_info = (vx_tensor)vxGetObjectArrayItem((vx_object_array)input_arr[0], 0);
    vx_tensor map_feat   = (vx_tensor)vxGetObjectArrayItem((vx_object_array)input_arr[1], 0);
    vx_tensor map_desc   = (vx_tensor)vxGetObjectArrayItem((vx_object_array)input_arr[2], 0);
    vx_tensor cur_feat;

    if(tidl_output_arr[poseCalcObj->params.score_lyr_id] != NULL)
    {
        cur_feat   = (vx_tensor)vxGetObjectArrayItem((vx_object_array)tidl_output_arr[poseCalcObj->params.score_lyr_id], 0);
    }
    else
    {
        /*This scenario comes in raw data dump. Mainly debugging. Just passing some valid tensor, not used anyway*/
        cur_feat = (vx_tensor)vxGetObjectArrayItem((vx_object_array)tidl_output_arr[0], 0);
    }

    vx_tensor cur_desc;

    if(tidl_output_arr[poseCalcObj->params.lo_res_desc_lyr_id] != NULL)
    {
        cur_desc   = (vx_tensor)vxGetObjectArrayItem((vx_object_array)tidl_output_arr[poseCalcObj->params.lo_res_desc_lyr_id], 0);
    }
    else
    {
        /*This scenario comes in raw data dump. Mainly debugging. Just passing some valid tensor, not used anyway*/
        cur_desc   = (vx_tensor)vxGetObjectArrayItem((vx_object_array)tidl_output_arr[0], 0);
    }

    vx_tensor wt_table   = (vx_tensor)vxGetObjectArrayItem((vx_object_array)input_arr[3], 0);
    vx_tensor lens_table = (vx_tensor)vxGetObjectArrayItem((vx_object_array)input_arr[4], 0);
    vx_user_data_object out_args   = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)tidl_out_args[0], 0);
    vx_matrix pose_mat   = (vx_matrix)vxGetObjectArrayItem((vx_object_array)poseCalcObj->output_tensor_arr[0], 0);

    poseCalcObj->node = tivxVisualLocalizationNode(graph,
                                                    poseCalcObj->config,
                                                    voxel_info, // voxelInfo
                                                    map_feat,   // map_3d_points,
                                                    map_desc,   // map_desc,
                                                    cur_feat,   // cur_frame_feat
                                                    cur_desc,   // cur_frame_desc
                                                    wt_table,   // up_samp_wt,
                                                    lens_table, // lens_dist_table,
                                                    out_args,   // tidl out args
                                                    pose_mat);  // poseMatrix
    APP_ASSERT_VALID_REF(poseCalcObj->node);

    status = vxSetNodeTarget(poseCalcObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP1);
    vxSetReferenceName((vx_reference)poseCalcObj->node, "VisualLocalizationNode");

    vx_bool replicate[] = {vx_false_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e};
    vxReplicateNode(graph, poseCalcObj->node, replicate, 10);

    vxReleaseTensor(&voxel_info);
    vxReleaseTensor(&map_feat);
    vxReleaseTensor(&map_desc);
    vxReleaseTensor(&cur_feat);
    vxReleaseTensor(&cur_desc);
    vxReleaseTensor(&wt_table);
    vxReleaseTensor(&lens_table);
    vxReleaseUserDataObject(&out_args);
    vxReleaseMatrix(&pose_mat);

    return(status);
}


static void createInputOutputTensors(vx_context context, PoseCalcObj* poseCalcObj)
{
    vx_size output_sizes[APP_MAX_TENSOR_DIMS];

    /*creating matrix for pose of size 4x4. Previous frame pose is used as estimated location for current frame*/
    vx_matrix pose_matrix = vxCreateMatrix(context, VX_TYPE_FLOAT32, 3, 4);
    APP_ASSERT_VALID_REF(pose_matrix);
    vxSetReferenceName((vx_reference)pose_matrix, "EstimatedPose");
    poseCalcObj->output_tensor_arr[0] = vxCreateObjectArray(context, (vx_reference)pose_matrix, NUM_CH);
    vxReleaseMatrix(&pose_matrix);

    /*Creating map related tensors*/
    output_sizes[0] = sizeof(tiadalg_voxel_info);
    output_sizes[1] = poseCalcObj->params.num_voxels;
    vx_tensor voxel_info  = vxCreateTensor(context, 2, output_sizes, VX_TYPE_UINT8, 0);
    vxSetReferenceName((vx_reference)voxel_info,"voxel_info");
    poseCalcObj->input_tensor_arr[0] = vxCreateObjectArray(context, (vx_reference)voxel_info, NUM_CH);
    vxReleaseTensor(&voxel_info);

    output_sizes[0] = sizeof(tiadalg_map_feat)>>2;
    output_sizes[1] = poseCalcObj->params.num_map_feat;
    vx_tensor map_feat = vxCreateTensor(context, 2, output_sizes, VX_TYPE_FLOAT32, 0);
    vxSetReferenceName((vx_reference)map_feat,"map_3d_points");
    poseCalcObj->input_tensor_arr[1] = vxCreateObjectArray(context, (vx_reference)map_feat, NUM_CH);
    vxReleaseTensor(&map_feat);

    output_sizes[0] = sizeof(tiadalg_feat_desc) >> 1;
    output_sizes[1] = poseCalcObj->params.num_map_feat;
    vx_tensor map_desc = vxCreateTensor(context, 2, output_sizes, VX_TYPE_INT16, 0);
    vxSetReferenceName((vx_reference)map_desc,"map_feat_desc");
    poseCalcObj->input_tensor_arr[2] = vxCreateObjectArray(context, (vx_reference)map_desc, NUM_CH);
    vxReleaseTensor(&map_desc);

    /* Even if upsampling weight is not needed in external feat flow, but passing it to avoid null reference //if(obj->is_feat_comp_ext == 0x0) */
    output_sizes[0] = 7*7*64+2*64; /*size in elements, each element is int8. 7x7 filter for 64 plane and 64 biases*/
    vx_tensor wt_table = vxCreateTensor(context, 1, output_sizes, VX_TYPE_INT8, 0);
    vxSetReferenceName((vx_reference)wt_table,"upsampling_weights");
    poseCalcObj->input_tensor_arr[3] = vxCreateObjectArray(context, (vx_reference)wt_table, NUM_CH);
    vxReleaseTensor(&wt_table);

    output_sizes[0] = poseCalcObj->params.lens_dist_table_size;
    vx_tensor lens_table = vxCreateTensor(context, 1, output_sizes, VX_TYPE_FLOAT32, 0);
    vxSetReferenceName((vx_reference)lens_table,"lens_dist_table");
    poseCalcObj->input_tensor_arr[4] = vxCreateObjectArray(context, (vx_reference)lens_table, NUM_CH);
    vxReleaseTensor(&lens_table);

    vx_tensor input_tensor;

    input_tensor = (vx_tensor)vxGetObjectArrayItem(poseCalcObj->input_tensor_arr[0], 0);
    fill1DTensor(input_tensor,poseCalcObj->input_voxel_info_file);
    vxReleaseTensor(&input_tensor);

    input_tensor = (vx_tensor)vxGetObjectArrayItem(poseCalcObj->input_tensor_arr[1], 0);
    fill1DTensor(input_tensor,poseCalcObj->input_map_feat_pt_file);
    vxReleaseTensor(&input_tensor);

    input_tensor = (vx_tensor)vxGetObjectArrayItem(poseCalcObj->input_tensor_arr[2], 0);
    fill1DTensor(input_tensor,poseCalcObj->input_map_feat_desc_file);
    vxReleaseTensor(&input_tensor);

    /*Format of weights tensor is 7x7*64 (8b filter coefficient) + 2*64 (16b bias)*/
    input_tensor = (vx_tensor)vxGetObjectArrayItem(poseCalcObj->input_tensor_arr[3], 0);
    fill1DTensorFrom2Bin(input_tensor,
                          poseCalcObj->input_upsampling_weight,
                          poseCalcObj->input_upsampling_bias);
    vxReleaseTensor(&input_tensor);

    if(poseCalcObj->params.is_ip_fe == 0x1)
    {
        input_tensor = (vx_tensor)vxGetObjectArrayItem(poseCalcObj->input_tensor_arr[4], 0);
        fill1DTensor(input_tensor,poseCalcObj->input_lens_dist_table_file);
        vxReleaseTensor(&input_tensor);
    }

    return;
}

static vx_int32 fill1DTensor(vx_tensor in_tensor, vx_char* in_file)
{
    vx_status status = VX_SUCCESS;
    vx_map_id map_id_tensor;
    vx_uint8* tensor_buffer;

    FILE* fp;
    vx_int32 bytes_read = 0;

    vx_size    start[APP_MAX_TENSOR_DIMS];

    vx_size num_dims;
    vx_size size[3];
    vx_enum data_type;

    vxQueryTensor(in_tensor, VX_TENSOR_NUMBER_OF_DIMS, &num_dims, sizeof(num_dims));
    vxQueryTensor(in_tensor, VX_TENSOR_DIMS, &size, sizeof(vx_size)*3);
    vxQueryTensor(in_tensor, VX_TENSOR_DATA_TYPE, &data_type, sizeof(data_type));

    start[0]         = 0;
    start[1]         = 0;
    start[2]         = 0;

    vx_int32 tensor_size=1;
    for(int32_t i=0; i <  num_dims; i++){
        tensor_size *= size[i];
    }

    if((data_type == VX_TYPE_FLOAT32) || (data_type == VX_TYPE_UINT32) || ((data_type == VX_TYPE_INT32)))
    {
        tensor_size *= sizeof(vx_int32);
    }

    if((data_type == VX_TYPE_UINT16) || ((data_type == VX_TYPE_INT16)))
    {
        tensor_size *= sizeof(vx_int16);
    }

    status = tivxMapTensorPatch(in_tensor, num_dims, start, size, &map_id_tensor,
                                size,(void**) &tensor_buffer,
                                VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    if (status == VX_SUCCESS)
    {
        fp = fopen(in_file,"rb");
        if(fp == NULL)
        {
            printf("input binary file %s could not be opened \n", in_file);
        }
        else
        {
            fseek(fp,0L,SEEK_END);
            size[0] = ftell(fp);
            fseek(fp,0L,SEEK_SET);

            if(size[0] != tensor_size)
            {
                printf("Size of the tensor is lesser than binary file size\n");
            }
            else
            {
                bytes_read = fread(tensor_buffer, 1, size[0], fp);
            }
            fclose(fp);
      }
      tivxUnmapTensorPatch(in_tensor, map_id_tensor);
    }
    return (bytes_read);
}

static vx_int32 fill1DTensorFrom2Bin(vx_tensor in_tensor, vx_char* in_file1, vx_char* in_file2)
{
    vx_status status = VX_SUCCESS;
    vx_map_id map_id_tensor;
    vx_uint8* tensor_buffer;

    FILE* fp1;
    FILE* fp2;

    vx_int32 bytes_read = 0;

    vx_size    start[APP_MAX_TENSOR_DIMS];

    vx_size num_dims;
    vx_size size[3];
    vx_enum data_type;

    vxQueryTensor(in_tensor, VX_TENSOR_NUMBER_OF_DIMS, &num_dims, sizeof(num_dims));
    vxQueryTensor(in_tensor, VX_TENSOR_DIMS, &size, sizeof(vx_size)*3);
    vxQueryTensor(in_tensor, VX_TENSOR_DATA_TYPE, &data_type, sizeof(data_type));

    start[0]  = 0;
    start[1]  = 0;
    start[2]  = 0;

    vx_int32 tensor_size=1;

    for(int32_t i=0; i <  num_dims; i++)
    {
        tensor_size *= size[i];
    }

    if((data_type == VX_TYPE_FLOAT32) || (data_type == VX_TYPE_UINT32) || ((data_type == VX_TYPE_INT32)))
    {
        tensor_size *= sizeof(vx_int32);
    }

    if((data_type == VX_TYPE_UINT16) || ((data_type == VX_TYPE_INT16)))
    {
        tensor_size *= sizeof(vx_int16);
    }

    status = tivxMapTensorPatch(in_tensor, num_dims, start, size, &map_id_tensor,
                                size,(void**) &tensor_buffer,
                                VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    if (status == VX_SUCCESS)
    {
        fp1 = fopen(in_file1,"rb");
        fp2 = fopen(in_file2,"rb");

        if((fp1 == NULL) && (fp2 == NULL))
        {
            printf("input binary file %s and %s could not be opened \n", in_file1, in_file2);
        }
        else if ((fp1 != NULL) && (fp2 == NULL))
        {
            printf("input binary file %s could not be opened \n", in_file1);
            fclose(fp1);
        }
        else if ((fp1 == NULL) && (fp2 != NULL))
        {
            printf("input binary file %s could not be opened \n", in_file2);
            fclose(fp2);
        }
        else
        {
            fseek(fp1,0L,SEEK_END);
            size[0] = ftell(fp1);
            fseek(fp1,0L,SEEK_SET);

            fseek(fp2,0L,SEEK_END);
            size[1] = ftell(fp2);
            fseek(fp2,0L,SEEK_SET);

            if((size[0] + size[1])  != tensor_size)
            {
                printf("Size of the tensor is lesser than binary file size\n");
            }
            else
            {
                bytes_read  = fread(tensor_buffer, 1, size[0], fp1);
                bytes_read += fread(tensor_buffer+bytes_read, 1, size[1], fp2);
            }

            fclose(fp1);
            fclose(fp2);
        }
        tivxUnmapTensorPatch(in_tensor, map_id_tensor);
    }
    return (bytes_read);
}
