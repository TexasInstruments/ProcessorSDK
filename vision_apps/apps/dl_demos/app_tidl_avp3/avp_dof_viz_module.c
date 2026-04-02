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
#include "avp_dof_viz_module.h"
#include "TI/hwa_dmpac_dof.h"

vx_status app_init_dof_viz(vx_context context, DofVizObj *dofVizObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    uint32_t ch;

    vx_image flow_vector_field_image;
    vx_image confidence_image;
    vx_scalar confidence_threshold;

    confidence_threshold = vxCreateScalar(context, VX_TYPE_UINT32, &dofVizObj->confidence_threshold_value);
    APP_ASSERT_VALID_REF(confidence_threshold);
    vxSetReferenceName((vx_reference)confidence_threshold, "ConfidenceThreshold");
    dofVizObj->confidence_threshold_array  = vxCreateObjectArray(context, (vx_reference)confidence_threshold, NUM_CH);

    /* We need to copy the scalar value again here as creating the object array does not copy the contents of scalar exemplar */
    for(ch = 0; ch < NUM_CH; ch++)
    {
        vx_scalar tmp_scalar = (vx_scalar)vxGetObjectArrayItem(dofVizObj->confidence_threshold_array, ch);
        vxCopyScalar(tmp_scalar, &dofVizObj->confidence_threshold_value, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        vxReleaseScalar(&tmp_scalar);
    }

    if(dofVizObj->file_format==APP_FILE_FORMAT_YUV)
    {
        flow_vector_field_image = vxCreateImage(context, dofVizObj->width, dofVizObj->height, VX_DF_IMAGE_NV12);
    }
    else
    {
        flow_vector_field_image = vxCreateImage(context, dofVizObj->width, dofVizObj->height, VX_DF_IMAGE_RGB);
    }
    confidence_image = vxCreateImage(context, dofVizObj->width, dofVizObj->height, VX_DF_IMAGE_U8);

    dofVizObj->flow_vector_field_image_array  = vxCreateObjectArray(context, (vx_reference)flow_vector_field_image, NUM_CH);
    APP_ASSERT_VALID_REF(dofVizObj->flow_vector_field_image_array);

    dofVizObj->confidence_image_array  = vxCreateObjectArray(context, (vx_reference)confidence_image, NUM_CH);
    APP_ASSERT_VALID_REF(dofVizObj->confidence_image_array);

    vxReleaseScalar(&confidence_threshold);
    vxReleaseImage(&flow_vector_field_image);
    vxReleaseImage(&confidence_image);

    return status;
}

void app_deinit_dof_viz(DofVizObj *dofVizObj)
{
    vxReleaseObjectArray(&dofVizObj->confidence_threshold_array);
    vxReleaseObjectArray(&dofVizObj->flow_vector_field_image_array);
    vxReleaseObjectArray(&dofVizObj->confidence_image_array);
}

void app_delete_dof_viz(DofVizObj *dofVizObj)
{
    if(dofVizObj->node != NULL)
    {
        vxReleaseNode(&dofVizObj->node);
    }else{

    }
}

vx_status app_create_graph_dof_viz(vx_graph graph, DofVizObj *dofVizObj, vx_delay flow_vector_field_delay, vx_object_array flow_vector_field_array)
{
    vx_status status = VX_SUCCESS;

    vx_scalar confidence_threshold = (vx_scalar)vxGetObjectArrayItem(dofVizObj->confidence_threshold_array, 0);
    vx_image flow_vector_field = NULL;

    if((flow_vector_field_delay == NULL) && (flow_vector_field_array != NULL))
    {
        flow_vector_field = (vx_image)vxGetObjectArrayItem(flow_vector_field_array, 0);
    }
    else if (flow_vector_field_delay != NULL)
    {
        vx_object_array tmp_array = (vx_object_array)vxGetReferenceFromDelay(flow_vector_field_delay, 0);
        flow_vector_field = (vx_image)vxGetObjectArrayItem(tmp_array, 0);
    }

    vx_image flow_vector_field_image = (vx_image)vxGetObjectArrayItem(dofVizObj->flow_vector_field_image_array, 0);
    vx_image confidence_image  = (vx_image)vxGetObjectArrayItem(dofVizObj->confidence_image_array, 0);

    dofVizObj->node = tivxDofVisualizeNode(graph,
                                           flow_vector_field,
                                           confidence_threshold,
                                           flow_vector_field_image,
                                           confidence_image);

    status = vxGetStatus((vx_reference)dofVizObj->node);

    if(status == VX_SUCCESS)
    {
        vxSetReferenceName((vx_reference)dofVizObj->node , "DofVisualize");
        status = vxSetNodeTarget(dofVizObj->node , VX_TARGET_STRING, TIVX_TARGET_DSP1);
    }

    if(status == VX_SUCCESS)
    {
        vx_bool replicate[] = { vx_true_e, vx_true_e, vx_true_e, vx_true_e };
        vxReplicateNode(graph, dofVizObj->node, replicate, 4);
    }

    vxReleaseScalar(&confidence_threshold);
    vxReleaseImage(&flow_vector_field);
    vxReleaseImage(&flow_vector_field_image);
    vxReleaseImage(&confidence_image);

    return status;
}

vx_status writeDofVizOutput(char* file_name, vx_object_array img_arr)
{
    vx_status status;

    status = vxGetStatus((vx_reference)img_arr);

    if(status == VX_SUCCESS)
    {
        vx_size arr_len;
        vx_int32 ch;

        vxQueryObjectArray(img_arr, VX_OBJECT_ARRAY_NUMITEMS, &arr_len, sizeof(vx_size));

        for (ch = 0; ch < arr_len; ch++)
        {
            vx_rectangle_t rect;
            vx_imagepatch_addressing_t image_addr;
            vx_map_id map_id;
            void * data_ptr;
            vx_uint32  img_width;
            vx_uint32  img_height;
            vx_image   out_img;
            vx_uint32  num_bytes;

            vx_char new_name[APP_MAX_FILE_PATH];
            FILE *fp = NULL;

            out_img = (vx_image)vxGetObjectArrayItem(img_arr, ch);

            vxQueryImage(out_img, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
            vxQueryImage(out_img, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));

            snprintf(new_name, APP_MAX_FILE_PATH, "%s_%dx%d_ch%d.yuv", file_name, (uint32_t)img_width, (uint32_t)img_height, ch);

            fp = fopen(new_name,"wb");

            if(fp == NULL)
            {
                printf("Unable to open file %s \n", new_name);
                return (VX_FAILURE);
            }

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

            //Copy Luma
            num_bytes = fwrite(data_ptr,1,img_width*img_height, fp);

            if(num_bytes != (img_width*img_height))
                printf("Luma bytes written = %d, expected = %d", num_bytes, img_width*img_height);

            vxUnmapImagePatch(out_img, map_id);

            rect.start_x = 0;
            rect.start_y = 0;
            rect.end_x = img_width;
            rect.end_y = img_height / 2;
            status = vxMapImagePatch(out_img,
                                    &rect,
                                    1,
                                    &map_id,
                                    &image_addr,
                                    &data_ptr,
                                    VX_READ_ONLY,
                                    VX_MEMORY_TYPE_HOST,
                                    VX_NOGAP_X);


            //Copy CbCr
            num_bytes = fwrite(data_ptr,1,img_width*img_height/2, fp);

            if(num_bytes != (img_width*img_height/2))
                printf("CbCr bytes written = %d, expected = %d", num_bytes, img_width*img_height/2);

            vxUnmapImagePatch(out_img, map_id);

            vxReleaseImage(&out_img);

            fclose(fp);
        }
    }

    return(status);
}
