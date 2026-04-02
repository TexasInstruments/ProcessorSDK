/*
 *
 * Copyright (c) 2021 Texas Instruments Incorporated
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
#include "app_sfm_module.h"

static vx_status setCreateParams(vx_context context, SFMObj *sfmObj, vx_user_data_object createParams);
static vx_status setInArgs(vx_context context, vx_user_data_object inArgs);
static vx_status setOutArgs(vx_context context, vx_user_data_object outArgs);

vx_status app_init_sfm(vx_context context, SFMObj *sfmObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    vx_uint32 capacity;
    vx_int32 i;

    vx_int32 q;

    vx_user_data_object config = vxCreateUserDataObject(context, "tivxSFMParams",
                                                    sizeof(tivxSFMParams), NULL);
    for(q = 0; q < APP_BUFFER_Q_DEPTH; q++)
    {
      sfmObj->config_arr[q] = vxCreateObjectArray(context, (vx_reference)config, NUM_CH);
      sfmObj->config[q]  = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)sfmObj->config_arr[q], 0);
    }

    vxReleaseUserDataObject(&config);

    if(status == VX_SUCCESS)
    {
        capacity = sizeof(SFM_TI_CreateParams);
        sfmObj->createParams = vxCreateUserDataObject(context, "SFM_TI_CreateParams", capacity, NULL );
        status = setCreateParams(context, sfmObj, sfmObj->createParams);
    }

    if(status == VX_SUCCESS)
    {
        vx_user_data_object inArgs;

        capacity = sizeof(SFM_TI_InArgs);
        inArgs = vxCreateUserDataObject(context, "SFM_TI_InArgs", capacity, NULL );
        sfmObj->in_args_arr  = vxCreateObjectArray(context, (vx_reference)inArgs, NUM_CH);
        vxReleaseUserDataObject(&inArgs);

        for(i = 0; i < NUM_CH; i++)
        {
            vx_user_data_object inArgs;

            inArgs = (vx_user_data_object)vxGetObjectArrayItem(sfmObj->in_args_arr, i);
            setInArgs(context, inArgs);
            vxReleaseUserDataObject(&inArgs);
        }
    }

    if(status == VX_SUCCESS)
    {
        vx_user_data_object outArgs;

        capacity = sizeof(SFM_TI_OutArgs);
        outArgs = vxCreateUserDataObject(context, "SFM_TI_OutArgs", capacity, NULL );
        sfmObj->out_args_arr  = vxCreateObjectArray(context, (vx_reference)outArgs, NUM_CH);
        vxReleaseUserDataObject(&outArgs);

        for(i = 0; i < NUM_CH; i++)
        {
            vx_user_data_object outArgs;

            outArgs = (vx_user_data_object)vxGetObjectArrayItem(sfmObj->out_args_arr, i);
            setOutArgs(context, outArgs);
            vxReleaseUserDataObject(&outArgs);
        }
    }

    if(status == VX_SUCCESS)
    {
        vx_image out_pt_cld_img = vxCreateImage(context, sfmObj->width, sfmObj->height, VX_DF_IMAGE_NV12);
        status = vxGetStatus((vx_reference)out_pt_cld_img);
        if(status == VX_SUCCESS)
        {
            sfmObj->output_ptcld_img_arr = vxCreateObjectArray(context, (vx_reference)out_pt_cld_img, NUM_CH);
            status = vxGetStatus((vx_reference)sfmObj->output_ptcld_img_arr);
            vxReleaseImage(&out_pt_cld_img);
            if(status != VX_SUCCESS)
            {
                printf("Unable to create SFM output point cloud image array!\n");
            }
        }
        else
        {
            printf("Unable to create SFM output point cloud image template!\n");
        }
    }

    if(status == VX_SUCCESS)
    {
        vx_image out_og_img = vxCreateImage(context, sfmObj->width, sfmObj->height, VX_DF_IMAGE_NV12);
        status = vxGetStatus((vx_reference)out_og_img);
        if(status == VX_SUCCESS)
        {
            sfmObj->output_og_img_arr = vxCreateObjectArray(context, (vx_reference)out_og_img, NUM_CH);
            status = vxGetStatus((vx_reference)sfmObj->output_og_img_arr);
            vxReleaseImage(&out_og_img);
            if(status != VX_SUCCESS)
            {
                printf("Unable to create SFM output point cloud image array!\n");
            }
        }
        else
        {
            printf("Unable to create SFM output point cloud image template!\n");
        }
    }

    if(status == VX_SUCCESS)
    {
        vx_user_data_object outFeat;

        capacity = sizeof(SFM_TI_output)*sfmObj->maxNumTracks;
        outFeat = vxCreateUserDataObject(context, "SFM_TI_OutFeat", capacity, NULL );
        sfmObj->out_feat_arr  = vxCreateObjectArray(context, (vx_reference)outFeat, NUM_CH);
        vxReleaseUserDataObject(&outFeat);
    }

    return status;
}

void app_deinit_sfm(SFMObj *sfmObj)
{
    vx_int32 q;

    for(q = 0; q < APP_BUFFER_Q_DEPTH; q++)
    {
        vxReleaseObjectArray(&sfmObj->config_arr[q]);
        vxReleaseUserDataObject(&sfmObj->config[q]);
    }
    APP_PRINTF("Config deinit done!\n");

    vxReleaseObjectArray(&sfmObj->in_args_arr);
    APP_PRINTF("in-args deinit done!\n");

    vxReleaseObjectArray(&sfmObj->out_args_arr);
    APP_PRINTF("out-args deinit done!\n");

    vxReleaseObjectArray(&sfmObj->output_ptcld_img_arr);
    APP_PRINTF("out-ptcld-img deinit done!\n");

    vxReleaseObjectArray(&sfmObj->output_og_img_arr);
    APP_PRINTF("out-og-img deinit done!\n");

    vxReleaseObjectArray(&sfmObj->out_feat_arr);
    APP_PRINTF("out-feat deinit done!\n");

    vxReleaseUserDataObject(&sfmObj->createParams);
    APP_PRINTF("params released!\n");
}

void app_delete_sfm(SFMObj *sfmObj)
{
    if(sfmObj->node != NULL)
    {
        vxReleaseNode(&sfmObj->node);
    }
}

vx_status app_create_graph_sfm(vx_graph graph, SFMObj *sfmObj, vx_object_array input_img_arr, vx_object_array flow_vec_arr)
{
    vx_status status = VX_SUCCESS;

    vx_image input_img  = (vx_image)vxGetObjectArrayItem(input_img_arr, 0);
    vx_image output_ptcld_img = (vx_image)vxGetObjectArrayItem(sfmObj->output_ptcld_img_arr, 0);
    vx_image output_og_img = (vx_image)vxGetObjectArrayItem(sfmObj->output_og_img_arr, 0);
    vx_image flow_vec = (vx_image)vxGetObjectArrayItem(flow_vec_arr, 0);

    vx_user_data_object config   = (vx_user_data_object)vxGetObjectArrayItem(sfmObj->config_arr[0], 0);
    vx_user_data_object in_args  = (vx_user_data_object)vxGetObjectArrayItem(sfmObj->in_args_arr, 0);
    vx_user_data_object out_args = (vx_user_data_object)vxGetObjectArrayItem(sfmObj->out_args_arr, 0);
    vx_user_data_object out_feat = (vx_user_data_object)vxGetObjectArrayItem(sfmObj->out_feat_arr, 0);

    sfmObj->node = tivxSFMNode(graph,
                               config,
                               sfmObj->createParams,
                               in_args,
                               out_args,
                               input_img,
                               flow_vec,
                               output_ptcld_img,
                               output_og_img,
                               out_feat);

    status = vxGetStatus((vx_reference)sfmObj->node);

    if(status == VX_SUCCESS)
    {
        vxSetReferenceName((vx_reference)sfmObj->node, "SFM_Node");
        status = vxSetNodeTarget(sfmObj->node, VX_TARGET_STRING, TIVX_TARGET_DSP_C7_1);
    }

    if(status == VX_SUCCESS)
    {
        vx_bool replicate[] = { vx_true_e, vx_false_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e, vx_true_e};
        vxReplicateNode(graph, sfmObj->node, replicate, TIVX_KERNEL_SFM_MAX_PARAMS);
    }

    vxReleaseImage(&input_img);
    vxReleaseImage(&flow_vec);
    vxReleaseImage(&output_ptcld_img);
    vxReleaseImage(&output_og_img);

    vxReleaseUserDataObject(&config);
    vxReleaseUserDataObject(&in_args);
    vxReleaseUserDataObject(&out_args);
    vxReleaseUserDataObject(&out_feat);

    return status;
}

static vx_status setCreateParams(vx_context context, SFMObj *sfmObj, vx_user_data_object createParams)
{
    vx_status status = VX_SUCCESS;
    vx_map_id  map_id;
    vx_uint32  capacity;
    void *createParams_buffer = NULL;

    status = vxGetStatus((vx_reference)createParams);

    if(VX_SUCCESS == status)
    {
        capacity = sizeof(SFM_TI_CreateParams);
        vxMapUserDataObject(createParams, 0, capacity, &map_id,
              (void **)&createParams_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if(createParams_buffer)
        {
            /* Fill all required create params here */

            SFM_TI_CreateParams *prms = createParams_buffer;

            prms->imgWidth      = sfmObj->width;
            prms->imgHeight     = sfmObj->height;
            prms->flowCtrl      = 0; //intrinsic flow
            prms->enAlgPtCldVis = 1;
            prms->enAlgOcpGrdVis= 1;
            prms->imgColorFmt   = 0; // 0--> yuvnv12, 1 BGR
            prms->maxNumTracks  = sfmObj->maxNumTracks;
            prms->keyPointStep  = sfmObj->keyPointStep;
            prms->ocpGrdSizeInPixels = 500;
            prms->rsvd1         = 0;// disable the profiling
            memcpy(prms->camIntPrm, sfmObj->camIntPrm, 9*sizeof(vx_float32));
        }
        else
        {
            printf("Unable to allocate memory for create time params! %d bytes\n", capacity);
        }

        vxUnmapUserDataObject(createParams, map_id);
    }

    return status;
}

static vx_status setInArgs(vx_context context, vx_user_data_object inArgs)
{
    vx_status status = VX_SUCCESS;

    vx_map_id  map_id;
    vx_uint32  capacity;
    void *inArgs_buffer = NULL;

    status = vxGetStatus((vx_reference)inArgs);

    if(VX_SUCCESS == status)
    {
        capacity = sizeof(SFM_TI_InArgs);
        vxMapUserDataObject(inArgs, 0, capacity, &map_id,
                    (void **)&inArgs_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if(inArgs_buffer)
        {
            SFM_TI_InArgs *prms = inArgs_buffer;
            prms->iVisionInArgs.size         = sizeof(SFM_TI_InArgs);
            prms->iVisionInArgs.subFrameInfo = 0;
        }
        else
        {
            printf("Unable to allocate memory for inArgs! %d bytes\n", capacity);
        }

        vxUnmapUserDataObject(inArgs, map_id);
    }

    return status;
}

static vx_status setOutArgs(vx_context context, vx_user_data_object outArgs)
{
    vx_status status = VX_SUCCESS;

    vx_map_id  map_id;
    vx_uint32  capacity;
    void *outArgs_buffer = NULL;

    status = vxGetStatus((vx_reference)outArgs);

    if(VX_SUCCESS == status)
    {
        capacity = sizeof(SFM_TI_OutArgs);
        vxMapUserDataObject(outArgs, 0, capacity, &map_id,
                            (void **)&outArgs_buffer, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);

        if(outArgs_buffer)
        {
            SFM_TI_OutArgs *prms = outArgs_buffer;
            prms->iVisionOutArgs.size  = sizeof(SFM_TI_OutArgs);
        }
        else
        {
            printf("Unable to allocate memory for outArgs! %d bytes\n", capacity);
        }

        vxUnmapUserDataObject(outArgs, map_id);
    }

    return status;
}