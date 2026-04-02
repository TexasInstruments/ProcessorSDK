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

#include "app_pose_viz_module.h"

vx_status set_pose_viz_defaults(PoseVizObj *poseVizObj)
{
    vx_status status = VX_SUCCESS;

    memset(&poseVizObj->params, 0, sizeof(tivxPoseVizParams));

    /*Ego localization parametrs*/
    /* Image resolution for pose visualization and other parameter for visulization*/
    poseVizObj->params.img_width  = 2048;
    poseVizObj->params.img_height = 1024;
    poseVizObj->params.img_num_planes = 3;

    poseVizObj->params.projMat[0][0] = -0.999974081148704;
    poseVizObj->params.projMat[0][1] = 0.0;
    poseVizObj->params.projMat[0][2] = -0.007199793802923;
    poseVizObj->params.projMat[0][3] = -158.774279623468715;

    poseVizObj->params.projMat[1][0] = -0.007199793802923;
    poseVizObj->params.projMat[1][1] = 0.0;
    poseVizObj->params.projMat[1][2] = 0.999974081148704;
    poseVizObj->params.projMat[1][3] = 6.612800677299523;

    poseVizObj->params.projMat[2][0] = 0.0;
    poseVizObj->params.projMat[2][1] = 1.0;
    poseVizObj->params.projMat[2][2] = 0.0;
    poseVizObj->params.projMat[2][3] = 300.0;

    poseVizObj->params.fx = 1024.0;
    poseVizObj->params.fy = 1024.0;
    poseVizObj->params.cx = 1024.0;
    poseVizObj->params.cy = 512.0;

    poseVizObj->params.max_background_image_copy = APP_BUFFER_Q_DEPTH;

    return(status);
}

vx_status app_init_pose_viz(vx_context context, PoseVizObj *poseVizObj, char *objName, vx_int32 bufq_depth)
{
    vx_status status = VX_SUCCESS;
    vx_int32 q;

    vx_enum config_type = VX_TYPE_INVALID;
    config_type = vxRegisterUserStruct(context, sizeof(tivxPoseVizParams));
    APP_ASSERT(config_type >= VX_TYPE_USER_STRUCT_START && config_type <= VX_TYPE_USER_STRUCT_END);

    poseVizObj->config = vxCreateArray(context, config_type, 1);
    APP_ASSERT_VALID_REF(poseVizObj->config);

    vx_char ref_name[APP_MAX_FILE_PATH];
    vxAddArrayItems(poseVizObj->config, 1, &poseVizObj->params, sizeof(tivxPoseVizParams));
    snprintf(ref_name, APP_MAX_FILE_PATH, "%s_config", objName);
    vxSetReferenceName((vx_reference)poseVizObj->config, ref_name);

    vx_image output_image = vxCreateImage(context, poseVizObj->params.img_width, poseVizObj->params.img_height, VX_DF_IMAGE_NV12);
    for(q = 0; q < bufq_depth; q++)
    {
        poseVizObj->output_image_arr[q] = vxCreateObjectArray(context, (vx_reference)output_image, NUM_CH);
        poseVizObj->output_image[q] = (vx_image)vxGetObjectArrayItem((vx_object_array)poseVizObj->output_image_arr[q], 0);
    }
    vxReleaseImage(&output_image);

    poseVizObj->bg_image = vxCreateImage(context, poseVizObj->params.img_width, poseVizObj->params.img_height, VX_DF_IMAGE_NV12);

    return status;
}

vx_status app_update_pose_viz(vx_context context, PoseVizObj *poseVizObj)
{
    vx_status status = VX_SUCCESS;

    return status;
}

void app_deinit_pose_viz(PoseVizObj *poseVizObj, vx_int32 bufq_depth)
{
    vx_int32 q;
    vxReleaseArray(&poseVizObj->config);
    vxReleaseImage(&poseVizObj->bg_image);

    for(q = 0; q < bufq_depth; q++)
    {
        vxReleaseObjectArray(&poseVizObj->output_image_arr[q]);
        vxReleaseImage(&poseVizObj->output_image[q]);
    }
}

void app_delete_pose_viz(PoseVizObj *poseVizObj)
{
    if(poseVizObj->node != NULL)
    {
        vxReleaseNode(&poseVizObj->node);
    }
}

vx_status app_create_graph_pose_viz(vx_graph graph, PoseVizObj *poseVizObj, vx_object_array *input_tensor_arr)
{
    vx_status status = VX_SUCCESS;

    vx_matrix  pose = (vx_matrix)vxGetObjectArrayItem((vx_object_array)input_tensor_arr[0], 0);
    vx_image   output_image = (vx_image)vxGetObjectArrayItem((vx_object_array)poseVizObj->output_image_arr[0], 0);

    poseVizObj->node = tivxPoseVizNode(graph,
                                        poseVizObj->config,
                                        poseVizObj->bg_image,
                                        pose,
                                        output_image);

    APP_ASSERT_VALID_REF(poseVizObj->node);
    status = vxSetNodeTarget(poseVizObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP2);
    vxSetReferenceName((vx_reference)poseVizObj->node, "PoseVizNode");

    vx_bool replicate[] = {vx_false_e, vx_false_e, vx_true_e, vx_true_e};
    vxReplicateNode(graph, poseVizObj->node, replicate, 4);

    vxReleaseMatrix(&pose);
    vxReleaseImage(&output_image);

    return(status);
}

vx_status readPoseVizBackgroundImage(char* file_name, vx_image bg_img)
{
    vx_status status;

    status = vxGetStatus((vx_reference)bg_img);

    if(status == VX_SUCCESS)
    {
        FILE * fp = fopen(file_name,"rb");

        if(fp == NULL)
        {
            printf("Unable to open file %s \n", file_name);
            return (VX_FAILURE);
        }

        vx_rectangle_t rect;
        vx_imagepatch_addressing_t image_addr;
        vx_map_id map_id;
        void * data_ptr;
        vx_uint32  img_width;
        vx_uint32  img_height;
        vx_uint32  num_bytes = 0, j;

        vxQueryImage(bg_img, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
        vxQueryImage(bg_img, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = img_width;
        rect.end_y = img_height;
        status = vxMapImagePatch(bg_img,
                                &rect,
                                0,
                                &map_id,
                                &image_addr,
                                &data_ptr,
                                VX_WRITE_ONLY,
                                VX_MEMORY_TYPE_HOST,
                                VX_NOGAP_X);

        /* Copy Luma */
        for (j = 0; j < img_height; j++)
        {
            num_bytes += fread(data_ptr, 1, img_width, fp);
            data_ptr += image_addr.stride_y;
        }

        if(num_bytes != (img_width*img_height))
            printf("Luma bytes read = %d, expected = %d\n", num_bytes, img_width*img_height);

        vxUnmapImagePatch(bg_img, map_id);

        status = vxMapImagePatch(bg_img,
                                &rect,
                                1,
                                &map_id,
                                &image_addr,
                                &data_ptr,
                                VX_WRITE_ONLY,
                                VX_MEMORY_TYPE_HOST,
                                VX_NOGAP_X);

        /* Copy CbCr */
        num_bytes = 0;
        for (j = 0; j < img_height/2; j++)
        {
             num_bytes += fread(data_ptr, 1, img_width, fp);
             data_ptr += image_addr.stride_y;
        }

        if(num_bytes != (img_width*img_height/2))
            printf("CbCr bytes read = %d, expected = %d\n", num_bytes, img_width*img_height/2);

        vxUnmapImagePatch(bg_img, map_id);

        fclose(fp);
    }

    return(status);
}

vx_status writePoseVizOutput(char* file_name, vx_image out_img)
{
    vx_status status;

    status = vxGetStatus((vx_reference)out_img);

    if(status == VX_SUCCESS)
    {
        FILE * fp = fopen(file_name,"wb");

        if(fp == NULL)
        {
            printf("Unable to open file %s \n", file_name);
            return (VX_FAILURE);
        }

        vx_rectangle_t rect;
        vx_imagepatch_addressing_t image_addr;
        vx_map_id map_id;
        void * data_ptr;
        vx_uint32  img_width;
        vx_uint32  img_height;
        vx_uint32  num_bytes = 0, j;

        vxQueryImage(out_img, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
        vxQueryImage(out_img, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = img_width;
        rect.end_y = img_height;
        status = vxMapImagePatch(out_img,
                                &rect,
                                0,
                                &map_id,
                                &image_addr,
                                &data_ptr,
                                VX_READ_ONLY,
                                VX_MEMORY_TYPE_HOST,
                                VX_NOGAP_X);

        /* Copy Luma */
        for (j = 0; j < img_height; j++)
        {
            num_bytes += fwrite(data_ptr, 1, img_width, fp);
            data_ptr += image_addr.stride_y;
        }

        if(num_bytes != (img_width*img_height))
            printf("Luma bytes written = %d, expected = %d\n", num_bytes, img_width*img_height);

        vxUnmapImagePatch(out_img, map_id);

        status = vxMapImagePatch(out_img,
                                &rect,
                                1,
                                &map_id,
                                &image_addr,
                                &data_ptr,
                                VX_READ_ONLY,
                                VX_MEMORY_TYPE_HOST,
                                VX_NOGAP_X);


        /* Copy CbCr */
        num_bytes = 0;
        for (j = 0; j < img_height/2; j++)
        {
             num_bytes += fwrite(data_ptr, 1, img_width, fp);
             data_ptr += image_addr.stride_y;
        }

        if(num_bytes != (img_width*img_height/2))
            printf("CbCr bytes written = %d, expected = %d\n", num_bytes, img_width*img_height/2);

        vxUnmapImagePatch(out_img, map_id);

        fclose(fp);
    }

    return(status);
}
