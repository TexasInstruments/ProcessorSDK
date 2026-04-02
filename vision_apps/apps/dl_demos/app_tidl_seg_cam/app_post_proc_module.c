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

#include "app_post_proc_module.h"

vx_status app_init_post_proc(vx_context context, PostProcObj *postProcObj, char *objName, vx_int32 num_cameras)
{
    vx_status status = VX_SUCCESS;
    vx_char ref_name[APP_MAX_FILE_PATH];

    postProcObj->viz_config = vxCreateUserDataObject(context, "", sizeof(tivxPixelVizParams), NULL);
    APP_ASSERT_VALID_REF(postProcObj->viz_config);

    status = vxCopyUserDataObject(postProcObj->viz_config, 0, sizeof(tivxPixelVizParams),\
                  &postProcObj->viz_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    if(status == VX_SUCCESS)
    {
        snprintf(ref_name, APP_MAX_FILE_PATH, "%s_viz_config", objName);
        status = vxSetReferenceName((vx_reference)postProcObj->viz_config, ref_name);
    }
    if(status == VX_SUCCESS)
    {
        vx_image output = vxCreateImage(context, postProcObj->out_width, postProcObj->out_height, VX_DF_IMAGE_NV12);
        postProcObj->output_image_arr  = vxCreateObjectArray(context, (vx_reference)output, num_cameras);
        status = vxReleaseImage(&output);
    }
    if(status == VX_SUCCESS)
    {
        postProcObj->viz_kernel = tivxAddKernelPixelViz(context, postProcObj->num_output_tensors);
        status = vxGetStatus((vx_reference)postProcObj->viz_kernel);
    }

    return status;
}

vx_status app_update_post_proc(PostProcObj *postProcObj, vx_user_data_object config)
{
    vx_status status = VX_SUCCESS;

    vx_map_id map_id_config;
    sTIDL_IOBufDesc_t *ioBufDesc;
    tivxTIDLJ7Params *tidlParams;
    vx_int32 id;

    status = vxMapUserDataObject(config, 0, sizeof(tivxTIDLJ7Params), &map_id_config,
                        (void **)&tidlParams, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0);

    if(status == VX_SUCCESS)
    {
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

        if((ioBufDesc->outElementType[0] == TIDL_UnsignedChar) || (ioBufDesc->outElementType[0] == TIDL_SignedChar))
        {
            postProcObj->viz_params.tidl_8bit_16bit_flag = 0;
        }
        else if((ioBufDesc->outElementType[0] == TIDL_UnsignedShort) || (ioBufDesc->outElementType[0] == TIDL_SignedShort))
        {
            postProcObj->viz_params.tidl_8bit_16bit_flag = 1;
        }
    }

    if(status == VX_SUCCESS)
    {
        status = vxUnmapUserDataObject(config, map_id_config);

        postProcObj->num_input_tensors = ioBufDesc->numInputBuf;
        postProcObj->num_output_tensors = ioBufDesc->numOutputBuf;
    }

    return status;
}
void app_deinit_post_proc(PostProcObj *postProcObj)
{
    vxReleaseUserDataObject(&postProcObj->viz_config);
    vxReleaseObjectArray(&postProcObj->output_image_arr);
}

void app_delete_post_proc(PostProcObj *postProcObj)
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

void app_create_graph_post_proc(vx_graph graph, PostProcObj *postProcObj, vx_object_array input_image_arr, vx_object_array out_args_arr, vx_object_array output_tensors_arr)
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
    vxSetReferenceName((vx_reference)postProcObj->node, "PostProcNode");

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

    status = vxQueryObjectArray((vx_object_array)output_arr, VX_OBJECT_ARRAY_NUMITEMS, &numCh, sizeof(vx_size));

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
        FILE *fp = NULL;
        vx_uint32  num_bytes = 0;

        vx_char new_name[APP_MAX_FILE_PATH];

        if(status == VX_SUCCESS)
        {
            output  = (vx_image)vxGetObjectArrayItem((vx_object_array)output_arr, ch);
            status = vxGetStatus((vx_reference) output);
        }
        if(status == VX_SUCCESS)
        {
            status = vxQueryImage(output, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
        }
        if(status == VX_SUCCESS)
        {
            status = vxQueryImage(output, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));
        }
        if(status == VX_SUCCESS)
        {
            snprintf(new_name, APP_MAX_FILE_PATH, "%s_%dx%d_ch%d.yuv", file_name, img_width, img_height, ch);
        }

        if(status == VX_SUCCESS)
        {
            fp = fopen(new_name, "wb");
            if(NULL == fp)
            {
                printf("Unable to open file %s for writing!\n", new_name);
                break;
            }
        }

        if(status == VX_SUCCESS)
        {
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
        }
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

            status = vxUnmapImagePatch(output, map_id_1);
        }

        if(status == VX_SUCCESS)
        {
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
        }

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

        if(fp != NULL)
        {
            fclose(fp);
        }
        vxReleaseImage(&output);
    }

    return status;
}
