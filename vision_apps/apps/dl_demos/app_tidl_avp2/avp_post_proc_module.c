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

#include "avp_post_proc_module.h"

static void fillTableTensors(PostProcObj *postProcObj);
static void drawPoints(PostProcObj *postProcObj, vx_uint8 *data_ptr_1, vx_uint8 *data_ptr_2, vx_uint16* kp_ptr, vx_int32 num_points, vx_int32 label);

vx_status app_init_post_proc_od(vx_context context, PostProcObj *postProcObj, char *objName)
{
    vx_status status = VX_SUCCESS;
    vx_size temp_size[3];
    vx_char ref_name[APP_MAX_FILE_PATH];

    vx_enum config_type = VX_TYPE_INVALID;
    config_type = vxRegisterUserStruct(context, sizeof(tivxODPostProcParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);

    postProcObj->config = vxCreateArray(context, config_type, 1);
    APP_ASSERT_VALID_REF(postProcObj->config);

    vxAddArrayItems(postProcObj->config, 1, &postProcObj->params, sizeof(tivxODPostProcParams));
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_config", objName);
    vxSetReferenceName((vx_reference)postProcObj->config, ref_name);

    temp_size[0] = 2;
    temp_size[1] = ANGLE_TABLE_ROWS;
    postProcObj->fwd_table_tensor = vxCreateTensor(context, 2, temp_size, VX_TYPE_FLOAT32, 0);
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_FwdLDCTable", objName);
    vxSetReferenceName((vx_reference)postProcObj->fwd_table_tensor, ref_name);

    temp_size[0] = 2;
    temp_size[1] = ANGLE_TABLE_ROWS;
    postProcObj->rev_table_tensor = vxCreateTensor(context, 2, temp_size, VX_TYPE_FLOAT32, 0);
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_RevLDCTable", objName);
    vxSetReferenceName((vx_reference)postProcObj->rev_table_tensor, ref_name);

    temp_size[0] = postProcObj->params.num_max_det;
    temp_size[1] = postProcObj->params.points_per_line*postProcObj->params.num_keypoints*2;
    vx_tensor kp_tensor = vxCreateTensor(context, 2, temp_size, VX_TYPE_UINT16, 0);
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_OutDetectedObjs", objName);
    vxSetReferenceName((vx_reference)kp_tensor, ref_name);
    postProcObj->kp_tensor_arr  = vxCreateObjectArray(context, (vx_reference)kp_tensor, NUM_CH);
    vxReleaseTensor(&kp_tensor);

    temp_size[0] = postProcObj->params.num_max_det;
    vx_tensor kp_valid = vxCreateTensor(context, 1, temp_size, VX_TYPE_UINT8, 0);
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_OutObjsValidFlag", objName);
    vxSetReferenceName((vx_reference)kp_valid, ref_name);
    postProcObj->kp_valid_arr  = vxCreateObjectArray(context, (vx_reference)kp_valid, NUM_CH);
    vxReleaseTensor(&kp_valid);

    fillTableTensors(postProcObj);

    return status;
}

vx_status app_update_post_proc_od(PostProcObj *postProcObj, vx_user_data_object config)
{
    vx_status status = VX_SUCCESS;

    vx_map_id map_id_config;
    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params *tidlParams;
    vx_int32 id;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                        (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;
    memcpy(&postProcObj->ioBufDesc, ioBufDesc, sizeof(sTIDL_IOBufDesc_t));

    for(id = 0; id < ioBufDesc->numOutputBuf; id++)
    {
        if(id < TIVX_PIXEL_VIZ_MAX_TENSOR)
        {
            postProcObj->viz_params.output_width[id] = ioBufDesc->outWidth[id];
            postProcObj->viz_params.output_height[id] = ioBufDesc->outHeight[id];
            postProcObj->viz_params.output_buffer_pitch[id] = ioBufDesc->outWidth[id]  + ioBufDesc->outPadL[id] + ioBufDesc->outPadR[id];
            postProcObj->viz_params.output_buffer_offset[id] = postProcObj->viz_params.output_buffer_pitch[id]*ioBufDesc->outPadT[id] + ioBufDesc->outPadL[id];
        }
    }

    vxUnmapUserDataObject(config, map_id_config);

    postProcObj->num_input_tensors = ioBufDesc->numInputBuf;
    postProcObj->num_output_tensors = ioBufDesc->numOutputBuf;

    return status;
}

void app_deinit_post_proc_od(PostProcObj *postProcObj)
{
    vxReleaseArray(&postProcObj->config);
    vxReleaseTensor(&postProcObj->fwd_table_tensor);
    vxReleaseTensor(&postProcObj->rev_table_tensor);

    vxReleaseObjectArray(&postProcObj->kp_tensor_arr);
    vxReleaseObjectArray(&postProcObj->kp_valid_arr);
}

void app_delete_post_proc_od(PostProcObj *postProcObj)
{
    if(postProcObj->node != NULL)
    {
        vxReleaseNode(&postProcObj->node);
    }
}

void app_create_graph_post_proc_od(vx_graph graph, PostProcObj *postProcObj, vx_object_array output_arr)
{
    vx_tensor output_tensor[APP_MAX_TENSORS];
    vx_tensor kp_tensor;
    vx_tensor kp_valid;

    output_tensor[0] = (vx_tensor)vxGetObjectArrayItem((vx_object_array)output_arr, 0);
    kp_tensor = (vx_tensor)vxGetObjectArrayItem((vx_object_array)postProcObj->kp_tensor_arr, 0);
    kp_valid = (vx_tensor)vxGetObjectArrayItem((vx_object_array)postProcObj->kp_valid_arr, 0);

    postProcObj->node = tivxODPostProcNode(graph,
                                            postProcObj->config,
                                            output_tensor[0],
                                            postProcObj->fwd_table_tensor,
                                            postProcObj->rev_table_tensor,
                                            kp_tensor,
                                            kp_valid);

    APP_ASSERT_VALID_REF(postProcObj->node);
    vxSetNodeTarget(postProcObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP2);
    vxSetReferenceName((vx_reference)postProcObj->node, "ODPostProcNode");

    vx_bool replicate[] = {vx_false_e, vx_true_e, vx_false_e, vx_false_e, vx_true_e, vx_true_e};
    vxReplicateNode(graph, postProcObj->node, replicate, 6);

    vxReleaseTensor(&kp_tensor);
    vxReleaseTensor(&kp_valid);
    vxReleaseTensor(&output_tensor[0]);
}

vx_status app_init_post_proc_pc(vx_context context, PostProcObj *postProcObj, char *objName)
{
    vx_status status = VX_SUCCESS;
    vx_char ref_name[APP_MAX_FILE_PATH];

    postProcObj->viz_config = vxCreateUserDataObject(context, "", sizeof(tivxPixelVizParams), NULL);
    APP_ASSERT_VALID_REF(postProcObj->viz_config);

    status = vxCopyUserDataObject(postProcObj->viz_config, 0, sizeof(tivxPixelVizParams),\
                  &postProcObj->viz_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_viz_config", objName);
    vxSetReferenceName((vx_reference)postProcObj->viz_config, ref_name);

    vx_image output = vxCreateImage(context, postProcObj->params.dl_width, postProcObj->params.dl_height, VX_DF_IMAGE_NV12);
    postProcObj->output_image_arr  = vxCreateObjectArray(context, (vx_reference)output, NUM_CH);
    vxReleaseImage(&output);

    if(status == VX_SUCCESS)
    {
        postProcObj->viz_kernel = tivxAddKernelPixelViz(context, postProcObj->num_output_tensors);
        status = vxGetStatus((vx_reference)postProcObj->viz_kernel);
    }

    return status;
}

vx_status app_update_post_proc_pc(PostProcObj *postProcObj, vx_user_data_object config)
{
    vx_status status = VX_SUCCESS;

    vx_map_id map_id_config;
    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params *tidlParams;
    vx_int32 id;

    vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                        (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    ioBufDesc = (sTIDL_IOBufDesc_t *)&tidlParams->ioBufDesc;
    memcpy(&postProcObj->ioBufDesc, ioBufDesc, sizeof(sTIDL_IOBufDesc_t));

    for(id = 0; id < ioBufDesc->numOutputBuf; id++)
    {
        if(id < TIVX_PIXEL_VIZ_MAX_TENSOR)
        {
            postProcObj->viz_params.output_width[id] = ioBufDesc->outWidth[id];
            postProcObj->viz_params.output_height[id] = ioBufDesc->outHeight[id];
            postProcObj->viz_params.output_buffer_pitch[id] = ioBufDesc->outWidth[id]  + ioBufDesc->outPadL[id] + ioBufDesc->outPadR[id];
            postProcObj->viz_params.output_buffer_offset[id] = postProcObj->viz_params.output_buffer_pitch[id]*ioBufDesc->outPadT[id] + ioBufDesc->outPadL[id];
        }
    }

    vxUnmapUserDataObject(config, map_id_config);

    postProcObj->num_input_tensors = ioBufDesc->numInputBuf;
    postProcObj->num_output_tensors = ioBufDesc->numOutputBuf;

    return status;
}
void app_deinit_post_proc_pc(PostProcObj *postProcObj)
{
    vxReleaseUserDataObject(&postProcObj->viz_config);
    vxReleaseObjectArray(&postProcObj->output_image_arr);
}

void app_delete_post_proc_pc(PostProcObj *postProcObj)
{
    if(postProcObj->node != NULL)
    {
        vxReleaseNode(&postProcObj->node);
    }
    if(postProcObj->viz_kernel != NULL)
    {
        vxRemoveKernel(postProcObj->viz_kernel);
    }
}

void app_create_graph_post_proc_pc(vx_graph graph, PostProcObj *postProcObj, vx_object_array input_image_arr, vx_object_array out_args_arr, vx_object_array output_tensors_arr)
{
    vx_image input_image;
    vx_user_data_object out_args;
    vx_tensor output_tensors[APP_MAX_TENSORS];
    vx_image output_images[APP_MAX_TENSORS];
    vx_int32 i;

    input_image = (vx_image)vxGetObjectArrayItem((vx_object_array)input_image_arr, 0);
    out_args    = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)out_args_arr, 0);

    output_tensors[0] = (vx_tensor)vxGetObjectArrayItem((vx_object_array)output_tensors_arr, 0);
    output_images[0] = (vx_image)vxGetObjectArrayItem((vx_object_array)postProcObj->output_image_arr, 0);

    postProcObj->node = tivxPixelVizNode(graph,
                                        postProcObj->viz_kernel,
                                        postProcObj->viz_config,
                                        out_args,
                                        input_image,
                                        postProcObj->num_output_tensors,
                                        output_tensors,
                                        output_images);

    APP_ASSERT_VALID_REF(postProcObj->node);
    vxSetNodeTarget(postProcObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP2);
    vxSetReferenceName((vx_reference)postProcObj->node, "PCPostProcNode");

    vx_bool replicate[32];

    replicate[0] = vx_false_e;
    replicate[1] = vx_true_e;
    replicate[2] = vx_true_e;

    for(i = 0; i < (postProcObj->num_output_tensors * 2); i++)
    {
        replicate[3 + i] =  vx_true_e;
    }

    vxReplicateNode(graph, postProcObj->node, replicate, (3 + postProcObj->num_output_tensors*2));

    vxReleaseImage(&input_image);
    vxReleaseUserDataObject(&out_args);

    for(i = 0; i < postProcObj->num_output_tensors; i++)
    {
        vxReleaseTensor(&output_tensors[i]);
        vxReleaseImage(&output_images[i]);
    }
}


vx_status writePostProcOutput(char* file_name, vx_object_array output_arr)
{
    vx_status status = VX_SUCCESS;

    vx_image output;
    vx_size numCh;
    vx_int32 ch, j;

    vxQueryObjectArray((vx_object_array)output_arr, VX_OBJECT_ARRAY_NUMITEMS, &numCh, sizeof(vx_size));

    for(ch = 0; ch < numCh; ch++)
    {
        vx_rectangle_t rect;
        vx_imagepatch_addressing_t image_addr;
        vx_map_id map_id_1;
        vx_map_id map_id_2;
        void * data_ptr_1;
        void * data_ptr_2;
        vx_uint32  img_width;
        vx_uint32  img_height;
        vx_uint32  num_bytes = 0;

        vx_char new_name[APP_MAX_FILE_PATH];

        output  = (vx_image)vxGetObjectArrayItem((vx_object_array)output_arr, ch);

        vxQueryImage(output, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
        vxQueryImage(output, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

        snprintf(new_name, APP_MAX_FILE_PATH, "%s_%dx%d_ch%d.yuv", file_name, img_width, img_height, ch);

        FILE *fp = fopen(new_name, "wb");
        if(NULL == fp)
        {
            printf("Unable to open file %s for writing!\n", new_name);
            break;
        }

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = img_width;
        rect.end_y = img_height;
        status = vxMapImagePatch(output,
                                &rect,
                                0,
                                &map_id_1,
                                &image_addr,
                                &data_ptr_1,
                                VX_WRITE_ONLY,
                                VX_MEMORY_TYPE_HOST,
                                VX_NOGAP_X);

        if(VX_SUCCESS == status)
        {
            /* Copy Luma */
            for (j = 0; j < img_height; j++)
            {
                num_bytes += fwrite(data_ptr_1, 1, img_width, fp);
                data_ptr_1 += image_addr.stride_y;
            }
            if(num_bytes != (img_width*img_height))
                printf("Luma bytes written = %d, expected = %d\n", num_bytes, img_width*img_height);

            vxUnmapImagePatch(output, map_id_1);
        }

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = img_width;
        rect.end_y = img_height / 2;
        status = vxMapImagePatch(output,
                                &rect,
                                1,
                                &map_id_2,
                                &image_addr,
                                &data_ptr_2,
                                VX_WRITE_ONLY,
                                VX_MEMORY_TYPE_HOST,
                                VX_NOGAP_X);


        if(VX_SUCCESS == status)
        {
            vx_int32 i, j, k;
            vx_uint8 *pData;

            /*Deinterleave CbCr for display*/
            vx_uint8 *pCb = tivxMemAlloc((img_width * img_height)/4, TIVX_MEM_EXTERNAL);
            vx_uint8 *pCr = tivxMemAlloc((img_width * img_height)/4, TIVX_MEM_EXTERNAL);

            if((pCb !=NULL) && (pCr !=NULL))
            {
                k = 0;
                pData = (vx_uint8 *)data_ptr_2;
                for(i = 0; i < (img_height/2); i++)
                {
                    for(j = 0; j < img_width; j+=2)
                    {
                        pCb[k] = pData[i*image_addr.stride_y + j];
                        pCr[k] = pData[i*image_addr.stride_y + j + 1];
                        k++;
                    }
                }

                fwrite(pCb, 1, (img_width * img_height)/4, fp);
                fwrite(pCr, 1, (img_width * img_height)/4, fp);

                tivxMemFree(pCb, (img_width * img_height)/4, TIVX_MEM_EXTERNAL);
                tivxMemFree(pCr, (img_width * img_height)/4, TIVX_MEM_EXTERNAL);
            }
            vxUnmapImagePatch(output, map_id_2);
        }

        fclose(fp);
        vxReleaseImage(&output);
    }

    return(status);
}
void drawDetections(PostProcObj *postProcObj, vx_object_array output_tensor_arr, vx_object_array input_image_arr, vx_float32 viz_th)
{
    vx_status status = VX_SUCCESS;

    vx_size output_sizes[APP_MAX_TENSOR_DIMS];
    vx_size output_strides[APP_MAX_TENSOR_DIMS];
    vx_size start[APP_MAX_TENSOR_DIMS];

    vx_uint16* kp_ptr = NULL;
    vx_uint8* kp_valid_ptr = NULL;

    vx_map_id map_id_kp = 0;
    vx_map_id map_id_kp_valid = 0;
    vx_map_id map_id_output = 0;

    sTIDL_IOBufDesc_t *ioBufDesc;

    vx_int32 ch, i;

    ioBufDesc = &postProcObj->ioBufDesc;

    for(ch = 0; ch < NUM_CH; ch++)
    {
        vx_tensor kp_tensor, kp_valid, output_tensor;

        start[0] = start[1] = start[2] = start[3] = 0;

        output_sizes[0] = postProcObj->params.num_max_det;
        output_sizes[1] = postProcObj->params.points_per_line*\
                            postProcObj->params.num_keypoints*2;

        output_strides[0] = 1;
        output_strides[1] = postProcObj->params.points_per_line*\
                            postProcObj->params.num_keypoints*2;

        kp_tensor = (vx_tensor)vxGetObjectArrayItem((vx_object_array)postProcObj->kp_tensor_arr, ch);
        status = vxGetStatus((vx_reference)kp_tensor);
        if (VX_SUCCESS == status)
        {
            status = tivxMapTensorPatch(kp_tensor, 2, start, output_sizes, &map_id_kp, output_strides, (void**)&kp_ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
        }

        if (VX_SUCCESS == status)
        {
            kp_valid = (vx_tensor)vxGetObjectArrayItem((vx_object_array)postProcObj->kp_valid_arr, ch);
            status = vxGetStatus((vx_reference)kp_valid);

            if (VX_SUCCESS == status)
            {
                status = tivxMapTensorPatch(kp_valid, 1, start, output_sizes, &map_id_kp_valid, output_strides, (void**)&kp_valid_ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
            }
        }

        output_sizes[0] = ioBufDesc->outWidth[0]  + ioBufDesc->outPadL[0] + ioBufDesc->outPadR[0];
        output_sizes[1] = ioBufDesc->outHeight[0] + ioBufDesc->outPadT[0] + ioBufDesc->outPadB[0];
        output_sizes[2] = ioBufDesc->outNumChannels[0];

        if (VX_SUCCESS == status)
        {
            output_tensor = (vx_tensor)vxGetObjectArrayItem((vx_object_array)output_tensor_arr, ch);
            status = vxGetStatus((vx_reference)output_tensor);
        }

        if (VX_SUCCESS == status)
        {
            void *output_buffer;

            TIDL_ODLayerHeaderInfo *pHeader;
            TIDL_ODLayerObjInfo *pObjInfo;
            vx_float32*pOut;
            vx_uint32 numObjs;
            TIDL_ODLayerObjInfo * pPSpots;
            vx_int32 total_points_per_box;
            vx_int32 occupied, unoccupied, background;

            output_strides[0] = 1;
            output_strides[1] = output_sizes[0];
            output_strides[2] = output_sizes[1] * output_strides[1];

            tivxMapTensorPatch(output_tensor, 3, start, output_sizes, &map_id_output, output_strides, &output_buffer, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

            pOut     = (vx_float32*)output_buffer + (ioBufDesc->outPadT[0] * output_sizes[0]) + ioBufDesc->outPadL[0];
            pHeader  = (TIDL_ODLayerHeaderInfo *)pOut;
            pObjInfo = (TIDL_ODLayerObjInfo *)((uint8_t *)pOut + (vx_uint32)pHeader->objInfoOffset);
            numObjs  = (vx_uint32)pHeader->numDetObjects;

            total_points_per_box = postProcObj->params.num_keypoints * postProcObj->params.points_per_line;

            occupied = 0;
            unoccupied = 0;
            background = 0;
            for (i = 0; i < numObjs; i++)
            {
                pPSpots = (TIDL_ODLayerObjInfo *) ((uint8_t *)pObjInfo + (i * ((vx_uint32)pHeader->objInfoSize)));
                if(pPSpots->score >= viz_th)
                {
                    if(pPSpots->label == 2) occupied++;
                    else if (pPSpots->label == 1) unoccupied++;
                    else background++;
                }
            }

            tivxUnmapTensorPatch(output_tensor, map_id_output);

            vx_image input_image  = (vx_image)vxGetObjectArrayItem((vx_object_array)input_image_arr, ch);
            status = vxGetStatus((vx_reference)input_image);

            if(VX_SUCCESS == status)
            {
                vx_rectangle_t rect;
                vx_imagepatch_addressing_t image_addr;
                vx_map_id map_id_1 = 0;
                vx_map_id map_id_2 = 0;
                void * data_ptr_1;
                void * data_ptr_2;
                vx_uint32  img_width;
                vx_uint32  img_height;

                vxQueryImage(input_image, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
                vxQueryImage(input_image, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

                rect.start_x = 0;
                rect.start_y = 0;
                rect.end_x = img_width;
                rect.end_y = img_height;
                status = vxMapImagePatch(input_image,
                                        &rect,
                                        0,
                                        &map_id_1,
                                        &image_addr,
                                        &data_ptr_1,
                                        VX_READ_ONLY,
                                        VX_MEMORY_TYPE_HOST,
                                        VX_NOGAP_X);

                if(VX_SUCCESS == status)
                {

                    rect.start_x = 0;
                    rect.start_y = 0;
                    rect.end_x = img_width;
                    rect.end_y = img_height / 2;
                    status = vxMapImagePatch(input_image,
                                            &rect,
                                            1,
                                            &map_id_2,
                                            &image_addr,
                                            &data_ptr_2,
                                            VX_READ_ONLY,
                                            VX_MEMORY_TYPE_HOST,
                                            VX_NOGAP_X);
                }

                if(VX_SUCCESS == status)
                {
                    for (i = 0; i < numObjs; i++)
                    {
                        pPSpots = (TIDL_ODLayerObjInfo *) ((uint8_t *)pObjInfo + (i * ((vx_uint32)pHeader->objInfoSize)));

                        /*Drawing of object for display purpose should be done only when score is high and key point valid flag is 1*/
                        /*For drawing purpose interpolated and post processed key points are used*/
                        if (NULL != kp_valid_ptr)
                        {
                            if((pPSpots->score >= 0.5) && (kp_valid_ptr[i] == 1) && (data_ptr_1 != 0x0) && (data_ptr_2 != 0x0))
                            {
                                drawPoints(postProcObj, data_ptr_1, data_ptr_2, &kp_ptr[i*total_points_per_box*2],total_points_per_box, pPSpots->label);
                            }
                        }
                    }
                }

                if(VX_SUCCESS == status)
                {
                    vxUnmapImagePatch(input_image, map_id_1);
                    vxUnmapImagePatch(input_image, map_id_2);
                }
            }

            vxReleaseImage(&input_image);
        }

        if(VX_SUCCESS == status)
        {
            tivxUnmapTensorPatch(kp_tensor, map_id_kp);
            tivxUnmapTensorPatch(kp_valid, map_id_kp_valid);

            vxReleaseTensor(&output_tensor);
            vxReleaseTensor(&kp_tensor);
            vxReleaseTensor(&kp_valid);
        }
    }
}

static void drawPoints(PostProcObj *postProcObj, vx_uint8 *data_ptr_1, vx_uint8 *data_ptr_2, vx_uint16* kp_ptr, vx_int32 num_points, vx_int32 label)
{
    vx_uint8 *pData1;
    vx_uint8 *pData2;

    vx_int32 img_width;
    vx_int32 img_height;
    vx_int32 img_stride;
    vx_int32 kp_x,kp_y;
    vx_int32 i,j,k;
    vx_uint8 color[3];
    vx_int32 kp_disp_dim = 2;

    /*Currently key points are generated in original image resolution 1280x720, hence
      to display it back on 512x512 imge it has to be rescaled.
     */
    vx_float32 scale_fact_x = (vx_float32)postProcObj->params.dl_width/postProcObj->params.width;
    vx_float32 scale_fact_y = (vx_float32)postProcObj->params.dl_height/postProcObj->params.height;

    img_width  = postProcObj->params.dl_width;
    img_height = postProcObj->params.dl_height;
    img_stride = postProcObj->params.dl_width;

    if(label == 1)
    {
        color[0] = 128; //Y
        color[1] = 55;  //Cb
        color[2] = 35;  //Cr
    }
    else if(label == 2)
    {
        color[0] = 66;  //Y
        color[1] = 91;  //Cb
        color[2] = 240; //Cr
    }
    else
    {
        color[0] = 0;   //Y
        color[1] = 128; //Cb
        color[2] = 128; //Cr
    }

    for(i = 0; i < num_points; i++)
    {
        kp_x = (vx_int32)((vx_float32)kp_ptr[2*i]*scale_fact_x);
        kp_y = (vx_int32)((vx_float32)kp_ptr[2*i + 1]*scale_fact_y);

        if((kp_x >= (kp_disp_dim/2)) &&
            (kp_x < (img_width - (kp_disp_dim/2))) &&
            (kp_y > (kp_disp_dim/2)) &&
            (kp_y < (img_height - (kp_disp_dim/2))))
        {
            pData1 = (vx_uint8 *)data_ptr_1 + (kp_y * img_stride) + kp_x;
            for(j = -(kp_disp_dim/2); j < (kp_disp_dim/2); j++)
            {
                for(k = -(kp_disp_dim/2); k < (kp_disp_dim/2); k++)
                {
                    pData1[(j*img_stride) + k] = color[0];
                }
            }

            pData2 = (vx_uint8 *)data_ptr_2 + ((kp_y >> 1) * img_stride) + (((kp_x + 1)>> 1)<<1);
            for(j = -(kp_disp_dim/2); j < (kp_disp_dim/2); j+=2)
            {
                for(k = -(kp_disp_dim/2); k < (kp_disp_dim/2); k+=2)
                {
                    pData2[((j>>1)*img_stride) + (k << 1)] = color[1];
                    pData2[((j>>1)*img_stride) + (k << 1) + 1] = color[2];
                }
            }
        }
    }
}

static void fillTableTensors(PostProcObj *postProcObj)
{
    vx_status status = VX_SUCCESS;

    vx_size start[APP_MAX_TENSOR_DIMS];
    vx_size input_strides[APP_MAX_TENSOR_DIMS];
    vx_size input_sizes[APP_MAX_TENSOR_DIMS];
    vx_int32 i;

    vx_map_id map_id_fwd_table_tensor;
    vx_map_id map_id_rev_table_tensor;

    vx_float32* fwd_table_tensor_buffer;
    vx_float32* rev_table_tensor_buffer;

    input_sizes[0] = 2;
    input_sizes[1] = ANGLE_TABLE_ROWS;

    start[0] = start[1] = 0;

    input_strides[0] = 1;
    input_strides[1] = input_sizes[0];

    status = tivxMapTensorPatch(postProcObj->fwd_table_tensor, 2, start, input_sizes, &map_id_fwd_table_tensor, input_strides,(void**) &fwd_table_tensor_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    if (status == VX_SUCCESS)
    {
        for(i = 0; i < ANGLE_TABLE_ROWS; i++)
        {
            fwd_table_tensor_buffer[2*i + 0] = fisheye_angle_table[i][0];
            fwd_table_tensor_buffer[2*i + 1] = fisheye_angle_table[i][1];
        }
        tivxUnmapTensorPatch(postProcObj->fwd_table_tensor, map_id_fwd_table_tensor);
    }

    status = tivxMapTensorPatch(postProcObj->rev_table_tensor, 2, start, input_sizes, &map_id_rev_table_tensor, input_strides,(void**) &rev_table_tensor_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if (status == VX_SUCCESS)
    {
        for(i = 0; i < ANGLE_TABLE_ROWS; i++)
        {
            rev_table_tensor_buffer[2*i + 0] = fisheye_angle_table_rev[i][0];
            rev_table_tensor_buffer[2*i + 1] = fisheye_angle_table_rev[i][1];
        }
    }
    tivxUnmapTensorPatch(postProcObj->rev_table_tensor, map_id_rev_table_tensor);
}
