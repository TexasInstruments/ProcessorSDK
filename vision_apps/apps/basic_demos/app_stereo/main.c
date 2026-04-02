/*
 *
 * Copyright (c) 2019 Texas Instruments Incorporated
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
#include "app_stereo_main.h"
#include "tivx_utils_checksum.h"
#include "app_test.h"
#include <TI/hwa_kernels.h>
#include <TI/video_io_kernels.h>

AppObj gAppObj;

vx_status load_vximage_from_bin_16(vx_image image, char *filename)
{
    uint32_t stride;
    uint32_t img_width, img_height;
    vx_df_image img_df;
    vx_status status = VX_SUCCESS;
    uint16_t *data_ptr = NULL;
    void *dst_data_ptr = NULL;
    vx_map_id map_id;

    /** - Check if image object is valid
     *
     * \code
     */
    if (status == (vx_status)VX_SUCCESS)
    {
        status = vxGetStatus((vx_reference)image);
    }

    if(status == (vx_status)VX_SUCCESS)
    {
        FILE * pf;
        uint32_t bpp = 2;

        img_height = 0;
        img_width = 0;

        vxQueryImage(image, (vx_enum)VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
        vxQueryImage(image, (vx_enum)VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));
        vxQueryImage(image, (vx_enum)VX_IMAGE_FORMAT, &img_df, sizeof(vx_df_image));

        stride = img_width*bpp;

        // read 16-bit input file
        pf = (FILE *)fopen(filename, "rb");

        if (NULL != pf)
        {
            data_ptr = (uint16_t *) malloc(img_width*img_height*sizeof(uint16_t));

            if (NULL != data_ptr)
            {
                fread(data_ptr, sizeof(uint16_t), img_width*img_height, pf);
                fclose(pf);

                if(status == (vx_status)VX_SUCCESS)
                {
                    vx_imagepatch_addressing_t image_addr;
                    vx_rectangle_t rect;
                    uint16_t *data_ptr_local = data_ptr;

                    uint32_t y, x;

                    rect.start_x = 0;
                    rect.start_y = 0;
                    rect.end_x = img_width;
                    rect.end_y = img_height;

                    vxMapImagePatch(image,
                        &rect,
                        0,
                        &map_id,
                        &image_addr,
                        &dst_data_ptr,
                        (vx_enum)VX_WRITE_ONLY,
                        (vx_enum)VX_MEMORY_TYPE_HOST,
                        (vx_enum)VX_NOGAP_X
                    );

                    dst_data_ptr = (uint16_t*) dst_data_ptr;
                    for (y = 0; y < img_height; y++)
                    {
                        for (x = 0; x < img_width; x++)
                        {
                            ((uint16_t*)dst_data_ptr)[x] = (data_ptr_local[x] >> 4);
                        }

                        data_ptr_local = (uint16_t*)((uint8_t*)data_ptr_local + stride);
                        dst_data_ptr = (uint16_t*)((uint8_t*)dst_data_ptr + image_addr.stride_y);
                    }

                    vxUnmapImagePatch(image, map_id);
                }
                free(data_ptr);
            }
            else
            {
                printf("# ERROR: Unable to allocate memory for reading file [%s]\n", filename);
                status = VX_FAILURE;
                fclose(pf);
            }
        }
        else
        {
            printf("# ERROR: Unable to open file for reading [%s]\n", filename);
            status = VX_FAILURE;
        }
    }

    return status;
}

int app_stereo_main(int argc, char* argv[])
{
    AppObj *obj = &gAppObj;
    vx_status status = VX_SUCCESS;

    app_parse_cmd_line_args(obj, argc, argv);

    status = app_init(obj);
    if(status == VX_SUCCESS)
    {
        status = app_create_graph(obj);
    }

    if(status == VX_SUCCESS)
    {
        if(obj->is_interactive)
        {
            status = app_run_graph_interactive(obj);
        }
        else
        {
            status = app_run_graph(obj);
        }
    }

    app_delete_graph(obj);
    app_deinit(obj);

    if (obj->test_mode)
    {
        if((test_result == vx_false_e) || (status != VX_SUCCESS))
        {
            printf("\n\nTEST FAILED\n\n");

            /* in the case that checksums changed, print a new checksums_expected
                array */
            print_new_checksum_structs();
            status = (status == VX_SUCCESS) ? VX_FAILURE : status;
        }
        else
        {
            printf("\n\nTEST PASSED\n\n");
        }
    }
    return status;
}

static vx_status app_init(AppObj *obj)
{

    vx_status status = VX_SUCCESS;
#ifndef SOC_J722S
    app_grpx_init_prms_t grpx_prms;
#endif

    obj->context = vxCreateContext();
    status = vxGetStatus((vx_reference)obj->context);
    if(status == VX_SUCCESS)
    {
        tivxStereoLoadKernels(obj->context);
        tivxVideoIOLoadKernels(obj->context);
        tivxHwaLoadKernels(obj->context);
    }

#ifndef x86_64
#ifndef SOC_J722S
    if (1 == obj->display_option)
    {
        appGrpxInitParamsInit(&grpx_prms, obj->context);
        grpx_prms.draw_callback = app_draw_graphics;
        appGrpxInit(&grpx_prms);
    }
#endif
#endif

    if(obj->pipeline_option==1)
    {
        printf("Pipeline Flow Enabled\n");
    }
    else
    {
        printf("Pipeline Flow Disabled\n");
    }
    return status;
}

static void app_deinit(AppObj *obj)
{
    tivxStereoUnLoadKernels(obj->context);
    tivxVideoIOUnLoadKernels(obj->context);
    tivxHwaUnLoadKernels(obj->context);
#ifndef x86_64
#ifndef SOC_J722S
    if (1 == obj->display_option)
    {
        appGrpxDeInit();
    }
#endif
#endif
    vxReleaseContext(&obj->context);
}

/*
 * Utility API used to add a graph parameter from a node, node parameter index
 */
static vx_status add_graph_parameter_by_node_index(vx_graph graph, vx_node node,
    vx_uint32 node_parameter_index)
{
    vx_status status = VX_SUCCESS;
    vx_parameter parameter = vxGetParameterByIndex(node, node_parameter_index);

    status = vxAddParameterToGraph(graph, parameter);
    if(status == VX_SUCCESS)
    {
        status = vxReleaseParameter(&parameter);
    }
    return status;
}

/*
 * Graph,
 *                              sde_config
 *                                  |
 *                                  v
 * left_img  ---> DmpacSde --> dispairty_out -> SdeDisparityVisualize --- > disaprity_img
 *                 ^    |
 * right_img ------+    +----> histogram_out -> SdeHistogramVisualize --- > histogram_img
 *
 * Graph (target),
 *                             sde_config                                                                 |
 *                                 |                                                                      |
 *                                 v                                                                      |
 * left_img  ---> DmpacSde --> dispairty_out -> SdeDisparityVisualize --- > disaprity_img ---> Display1   | left_img  --->ImgMosaic --> mosaic_out_img ---> Display2
 *                 ^    |                                                                                 |                   ^
 * right_img ------+    +----> histogram_out -> SdeHistogramVisualize --- > histogram_img                 | right_img --------+
 */

static vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_df_image df;

    uint32_t pipeline_depth, buf_id, list_depth = 2;
    if (obj->test_mode == 1)
    {
        list_depth = 3;
    }
    vx_graph_parameter_queue_params_t graph_parameters_queue_params_list[list_depth];

#if defined(PC)
    vx_size pcSize;
#endif

    obj->num_buf = MAX_NUM_BUF;
    pipeline_depth = MAX_NUM_BUF;

    obj->graph_sde = vxCreateGraph(obj->context);
    status = vxGetStatus((vx_reference)obj->graph_sde);

    // Set image object type depending on input bit depth.
    if (obj->bit_depth == 8)
    {
        df = VX_DF_IMAGE_U8;
    } else
    {
        df = VX_DF_IMAGE_U16;
    }

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)  && (obj->display_option == 1) && (status == VX_SUCCESS))
    {
        vx_image sample_input_img_left;
        vx_image sample_input_img_right;

        sample_input_img_left  = vxCreateImage(obj->context, obj->width, obj->height, df);
        status = vxGetStatus((vx_reference)sample_input_img_left);
        if(status == VX_SUCCESS)
        {
            sample_input_img_right = vxCreateImage(obj->context, obj->width, obj->height, df);
            status = vxGetStatus((vx_reference)sample_input_img_right);
        }

        for(buf_id=0; buf_id<obj->num_buf; buf_id++)
        {
            if(status == VX_SUCCESS)
            {
                obj->input_arr_left[buf_id]= vxCreateObjectArray(obj->context, (vx_reference)sample_input_img_left, 1);
                obj->input_img_left[buf_id] = (vx_image)vxGetObjectArrayItem((vx_object_array)obj->input_arr_left[buf_id], 0);
                status = vxGetStatus((vx_reference)obj->input_img_left[buf_id]);
            }
            else
            {
                break;
            }
            if(status == VX_SUCCESS)
            {
                obj->input_arr_right[buf_id]= vxCreateObjectArray(obj->context, (vx_reference)sample_input_img_right, 1);
                obj->input_img_right[buf_id] = (vx_image)vxGetObjectArrayItem((vx_object_array)obj->input_arr_right[buf_id], 0);
                status = vxGetStatus((vx_reference)obj->input_img_right[buf_id]);
            }
            else
            {
                break;
            }
        }
        obj->input_arr[0]= obj->input_arr_left[0];
        obj->input_arr[1]= obj->input_arr_right[0];
        if(status == VX_SUCCESS)
        {
            status = vxReleaseImage(&sample_input_img_left);
        }
        if(status == VX_SUCCESS)
        {
            status = vxReleaseImage(&sample_input_img_right);
        }
    }
    else
    {
        for(buf_id=0; buf_id<obj->num_buf; buf_id++)
        {
            if(status == VX_SUCCESS)
            {
                obj->input_img_left[buf_id] = vxCreateImage(obj->context, obj->width, obj->height, df);
                status = vxGetStatus((vx_reference)obj->input_img_left[buf_id]);
            }
            if(status == VX_SUCCESS)
            {
                obj->input_img_right[buf_id] = vxCreateImage(obj->context, obj->width, obj->height, df);
                status = vxGetStatus((vx_reference)obj->input_img_right[buf_id]);
            }
        }
    }

    // set sde_config from sde_params
    if(status == VX_SUCCESS)
    {
        obj->sde_config = vxCreateUserDataObject(obj->context, "tivx_dmpac_sde_params_t", sizeof(tivx_dmpac_sde_params_t), &obj->sde_params);
        status = vxGetStatus((vx_reference)obj->sde_config);
    }
    if(status == VX_SUCCESS)
    {
        status = vxSetReferenceName((vx_reference)obj->sde_config, "Stereo_Config");
    }
    // output
    if(status == VX_SUCCESS)
    {
        obj->disparity = vxCreateImage(obj->context, obj->width, obj->height, VX_DF_IMAGE_S16);
        status = vxGetStatus((vx_reference)obj->disparity);
    }
    if(status == VX_SUCCESS)
    {
        status = vxSetReferenceName((vx_reference)obj->disparity, "Stereo_OutputDisparityImageS16");
    }

    {
        for(buf_id=0; buf_id<obj->num_buf; buf_id++)
        {
            if (status == VX_SUCCESS)
            {
                obj->disparity_img[buf_id] = vxCreateImage(obj->context, obj->width, obj->height, VX_DF_IMAGE_RGB);
                status = vxGetStatus((vx_reference)obj->disparity_img[buf_id]);
            }
        }

        if(status == VX_SUCCESS)
        {
            obj->vis_confidence_threshold = vxCreateScalar(obj->context, VX_TYPE_UINT8, &obj->vis_confidence);
            status = vxGetStatus((vx_reference)obj->vis_confidence_threshold);
        }
        if(status == VX_SUCCESS)
        {
            status = vxSetReferenceName((vx_reference)obj->vis_confidence_threshold, "Stereo_VisualizeConfidenceThreshold");
        }
        if(status == VX_SUCCESS)
        {
            obj->histogram = vxCreateDistribution(obj->context, 128, 0, 4096);
            status = vxGetStatus((vx_reference)obj->histogram);
        }
        if(status == VX_SUCCESS)
        {
            status = vxSetReferenceName((vx_reference)obj->histogram, "Stereo_OutputHistogramDistribution");
        }
        if (obj->hist_output)
        {
            for(buf_id=0; buf_id<obj->num_buf; buf_id++)
            {
                if(status == VX_SUCCESS)
                {
                    obj->histogram_img[buf_id] = vxCreateImage(obj->context, HIST_IMG_WIDTH, HIST_IMG_HEIGHT, VX_DF_IMAGE_U8);
                    status = vxGetStatus((vx_reference)obj->histogram_img[buf_id]);
                }
            }
        }
        if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (0 != obj->display_option))
        {
            if(status == VX_SUCCESS)
            {
                memset(&obj->output_display_params, 0, sizeof(tivx_display_params_t));
                obj->output_display_config = vxCreateUserDataObject(obj->context, "tivx_display_params_t",
                    sizeof(tivx_display_params_t), NULL);
                status = vxGetStatus((vx_reference)obj->output_display_config);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetReferenceName((vx_reference)obj->output_display_config, "OutputDisplayConfiguration");
            }
            obj->output_display_params.opMode=TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
            obj->output_display_params.pipeId = 0;
            obj->output_display_params.outWidth = OUTPUT_DISPLAY_WIDTH;
            obj->output_display_params.outHeight = OUTPUT_DISPLAY_HEIGHT;
            obj->output_display_params.posX = (1920-OUTPUT_DISPLAY_WIDTH)/2;
            obj->output_display_params.posY = 1080-OUTPUT_DISPLAY_HEIGHT-250;
            if (status == VX_SUCCESS)
            {
                status = vxCopyUserDataObject(obj->output_display_config, 0, sizeof(tivx_display_params_t), &obj->output_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
            }
        }
        if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)) && (0 != obj->display_option) && (status == VX_SUCCESS))
        {
            obj->mosaic_out_width    = INPUT_DISPLAY_WIDTH;
            obj->mosaic_out_height   = INPUT_DISPLAY_HEIGHT;

            tivxImgMosaicParamsSetDefaults(&obj->mosaic_params);

            obj->mosaic_params.windows[0].startX  = 0;
            obj->mosaic_params.windows[0].startY  = 0;
            obj->mosaic_params.windows[0].width   = OUTPUT_DISPLAY_WIDTH/2;
            obj->mosaic_params.windows[0].height  = INPUT_DISPLAY_HEIGHT;
            obj->mosaic_params.windows[0].input_select   = 0;
            obj->mosaic_params.windows[0].channel_select = 0;

            obj->mosaic_params.windows[1].startX  = 1000-OUTPUT_DISPLAY_WIDTH/2;
            obj->mosaic_params.windows[1].startY  = 0;
            obj->mosaic_params.windows[1].width   = OUTPUT_DISPLAY_WIDTH/2;
            obj->mosaic_params.windows[1].height  = INPUT_DISPLAY_HEIGHT;
            obj->mosaic_params.windows[1].input_select   = 1;
            obj->mosaic_params.windows[1].channel_select = 0;

            obj->mosaic_params.num_windows  = 2;

            /* Number of time to clear the output buffer before it gets reused */

            obj->mosaic_params.clear_count  = 10;
            if(status == VX_SUCCESS)
            {
                obj->mosaic_config = vxCreateUserDataObject(obj->context, "ImgMosaicConfig", sizeof(tivxImgMosaicParams), NULL);
                status = vxGetStatus((vx_reference)obj->mosaic_config);
            }
            if(status == VX_SUCCESS)
            {
                status = vxCopyUserDataObject(obj->mosaic_config, 0, sizeof(tivxImgMosaicParams),\
                    &obj->mosaic_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
            }
            for(int id=0; id<obj->num_buf; id++)
            {
                if(status == VX_SUCCESS)
                {
                    obj->mosaic_output_image[id] = vxCreateImage(obj->context, obj->mosaic_out_width, obj->mosaic_out_height, VX_DF_IMAGE_U8);
                    status = vxGetStatus((vx_reference)obj->mosaic_output_image[id]);
                }
            }
            if(status == VX_SUCCESS)
            {
                obj->mosaic_kernel = tivxAddKernelImgMosaic(obj->context, 2);
                status = vxGetStatus((vx_reference)obj->mosaic_kernel);
            }
            if(status == VX_SUCCESS)
            {
                memset(&obj->input_display_params, 0, sizeof(tivx_display_params_t));
                obj->input_display_config = vxCreateUserDataObject(obj->context, "tivx_display_params_t",
                    sizeof(tivx_display_params_t), NULL);
                status = vxGetStatus((vx_reference)obj->input_display_config);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetReferenceName((vx_reference)obj->input_display_config, "InputDisplayConfiguration");
            }
            obj->input_display_params.opMode=TIVX_KERNEL_DISPLAY_ZERO_BUFFER_COPY_MODE;
            #if defined(SOC_J722S)
            obj->input_display_params.pipeId = 1;
            #else
            obj->input_display_params.pipeId = 2;
            #endif
            obj->input_display_params.outWidth = INPUT_DISPLAY_WIDTH;
            obj->input_display_params.outHeight = INPUT_DISPLAY_HEIGHT;
            obj->input_display_params.posX = (1920-INPUT_DISPLAY_WIDTH)/2;
            obj->input_display_params.posY = 1080-OUTPUT_DISPLAY_HEIGHT-250-INPUT_DISPLAY_HEIGHT-25;
            if(status == VX_SUCCESS)
            {
                status = vxCopyUserDataObject(obj->input_display_config, 0, sizeof(tivx_display_params_t), &obj->input_display_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
            }
        }
    }

#if 0
    // debug - to see array params
    vx_map_id map_id;
    vx_size stride = sizeof(vx_size);
    void *ptr;
    tivx_dmpac_sde_params_t *temp;
    vx_size numPoints = 0;
    status = vxQueryArray(obj->sde_config, VX_ARRAY_NUMITEMS, &numPoints, sizeof(numPoints));
    assert(VX_SUCCESS == status);

    status = vxMapArrayRange(obj->sde_config, 0, numPoints, &map_id, &stride, &ptr, VX_READ_AND_WRITE, VX_MEMORY_TYPE_HOST, 0);
    assert(VX_SUCCESS == status);

    temp = (tivx_dmpac_sde_params_t *)ptr;
    printf("** median_filter_enable:%d\n", temp->median_filter_enable);
    printf("** reduced_range_search_enable:%d\n", temp->reduced_range_search_enable);
    printf("** disparity_min:%d\n", temp->disparity_min);
    printf("** disparity_max:%d\n", temp->disparity_max);
    printf("** threshold_left_right:%d\n", temp->threshold_left_right);
    printf("** texture_filter_enable:%d\n", temp->texture_filter_enable);
    printf("** threshold_texture:%d\n", temp->threshold_texture);
    printf("** aggregation_penalty_p1:%d\n", temp->aggregation_penalty_p1);
    printf("** aggregation_penalty_p2:%d\n", temp->aggregation_penalty_p2);

    status = vxUnmapArrayRange(obj->sde_config, map_id);
    assert(VX_SUCCESS == status);
#endif

    if(status == VX_SUCCESS)
    {
        obj->node_sde = tivxDmpacSdeNode(
                                obj->graph_sde,
                                obj->sde_config,
                                obj->input_img_left[0],
                                obj->input_img_right[0],
                                obj->disparity,
                                obj->histogram);
        status = vxGetStatus((vx_reference)obj->node_sde);
    }
    if(status == VX_SUCCESS)
    {
        status = vxSetNodeTarget(obj->node_sde, VX_TARGET_STRING, TIVX_TARGET_DMPAC_SDE);
    }
    if(status == VX_SUCCESS)
    {
        status = vxSetReferenceName((vx_reference)obj->node_sde, "Stereo_Processing");
    }

    if(status == VX_SUCCESS)
    {
        obj->sde_vis_config = vxCreateUserDataObject(obj->context, "tivx_sde_disparity_vis_params_t", sizeof(tivx_sde_disparity_vis_params_t), &obj->sde_vis_params);
        status = vxGetStatus((vx_reference)obj->sde_vis_config);
        if(status == VX_SUCCESS)
        {
            status = vxSetReferenceName((vx_reference)obj->sde_vis_config, "Stereo_Visualize_Config");
        }
        if(status == VX_SUCCESS)
        {
            obj->node_sde_disparity_vis = tivxSdeDisparityVisualizeNode(
                                    obj->graph_sde,
                                    obj->sde_vis_config,
                                    obj->disparity,
                                    obj->disparity_img[0]);

            status = vxGetStatus((vx_reference)obj->node_sde_disparity_vis);
        }
        if(status == VX_SUCCESS)
        {
            status = vxSetNodeTarget(obj->node_sde_disparity_vis, VX_TARGET_STRING, TIVX_TARGET_DSP1);
        }
        if(status == VX_SUCCESS)
        {
            status = vxSetReferenceName((vx_reference)obj->node_sde_disparity_vis, "Stereo_DisparityViz");
        }
        if (obj->hist_output)
        {
            if(status == VX_SUCCESS)
            {
                obj->node_sde_histogram_vis = tivxSdeHistogramVisualizeNode(
                                    obj->graph_sde,
                                    obj->histogram,
                                    obj->histogram_img[0]);

                status = vxGetStatus((vx_reference)obj->node_sde_histogram_vis);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetNodeTarget(obj->node_sde_histogram_vis, VX_TARGET_STRING, TIVX_TARGET_DSP2);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetReferenceName((vx_reference)obj->node_sde_histogram_vis, "Stereo_HistogramViz");
            }
        }

        if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (obj->display_option == 1))
        {
            if(status == VX_SUCCESS)
            {
                obj->node_output_display = tivxDisplayNode(
                    obj->graph_sde,
                    obj->output_display_config,
                    obj->disparity_img[0]);
                status = vxGetStatus((vx_reference)obj->node_output_display);
            }
            if (status != VX_SUCCESS)
            {
                printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetNodeTarget(obj->node_output_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY1);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetReferenceName((vx_reference)obj->node_output_display, "OutputDisplay");
            }
        }
        if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2) && (obj->display_option == 1) && (status == VX_SUCCESS))
        {
            if(status == VX_SUCCESS)
            {
                obj->mosaic_node = tivxImgMosaicNode(obj->graph_sde,
                                        obj->mosaic_kernel,
                                        obj->mosaic_config,
                                        obj->mosaic_output_image[0],
                                        NULL,
                                        obj->input_arr,
                                        2);

                status = vxGetStatus((vx_reference)obj->mosaic_node);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetNodeTarget(obj->mosaic_node, VX_TARGET_STRING, TIVX_TARGET_VPAC_MSC1);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetReferenceName((vx_reference)obj->mosaic_node, "InputMosaicNode");
            }
            if(status == VX_SUCCESS)
            {
                obj->node_input_display = tivxDisplayNode(
                    obj->graph_sde,
                    obj->input_display_config,
                    obj->mosaic_output_image[0]);
                status = vxGetStatus((vx_reference)obj->node_input_display);
            }
            if (status != VX_SUCCESS)
            {
                printf("# ERROR: Display is not enabled on this platform, please disable it in config \n");
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetNodeTarget(obj->node_input_display, VX_TARGET_STRING, TIVX_TARGET_DISPLAY2);
            }
            if(status == VX_SUCCESS)
            {
                status = vxSetReferenceName((vx_reference)obj->node_input_display, "InputDisplay");
            }
        }
    }
    if(obj->pipeline_option==1)
    {
        int graph_parameter_num = 0;
            /* Set graph schedule config such that graph parameter @ index 0 is
            * enqueuable */
        if(status == VX_SUCCESS)
        {
            status = add_graph_parameter_by_node_index(obj->graph_sde, obj->mosaic_node, 3); //input_arr_left
            obj->input_arr_left_graph_parameter_index = graph_parameter_num;
            graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
            graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = obj->num_buf;
            graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&obj->input_arr_left[0];
            graph_parameter_num++;
        }
        if(status == VX_SUCCESS)
        {
            status = add_graph_parameter_by_node_index(obj->graph_sde, obj->mosaic_node, 4); //input_arr_right
            obj->input_arr_right_graph_parameter_index = graph_parameter_num;
            graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
            graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = obj->num_buf;
            graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&obj->input_arr_right[0];
            graph_parameter_num++;
        }
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = add_graph_parameter_by_node_index(obj->graph_sde, obj->mosaic_node, 1);
            obj->mosaic_output_parameter_index = graph_parameter_num;
            graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
            graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = obj->num_buf;
            graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&obj->mosaic_output_image[0];
            graph_parameter_num++;
        }
        if(status == VX_SUCCESS)
        {
            status = vxSetGraphScheduleConfig(obj->graph_sde,
                            VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                            graph_parameter_num,
                            graph_parameters_queue_params_list);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetGraphPipelineDepth(obj->graph_sde, pipeline_depth);
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->node_sde, 3 , obj->num_buf); //disparity
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->node_sde, 4 , obj->num_buf); //histogram
        }
        if(status == VX_SUCCESS)
        {
            if (obj->test_mode != 1)
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->mosaic_node, 1 , 2*obj->num_buf); //mosaic_output_image
            }
        }
        if(status == VX_SUCCESS)
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->node_sde_disparity_vis, 2 , obj->num_buf); //disparity_img
        }
        if(obj->hist_output && (status == VX_SUCCESS))
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->node_sde_histogram_vis, 1 , obj->num_buf); // histogram_img
        }
    }
    if(status == VX_SUCCESS)
    {
        status = vxVerifyGraph(obj->graph_sde);
    }
    return status;
}

static void app_delete_graph(AppObj *obj)
{
    uint32_t buf_id;

    for(buf_id=0; buf_id<obj->num_buf; buf_id++)
    {
        if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2)  && (obj->display_option == 1))
        {
            vxReleaseObjectArray(&obj->input_arr_left[buf_id]);
            vxReleaseObjectArray(&obj->input_arr_right[buf_id]);
        }
        vxReleaseImage(&obj->input_img_left[buf_id]);
        vxReleaseImage(&obj->input_img_right[buf_id]);
    }

    vxReleaseImage(&obj->disparity);
    vxReleaseUserDataObject(&obj->sde_config);
    vxReleaseNode(&obj->node_sde);

    {
        for(buf_id=0; buf_id<obj->num_buf; buf_id++)
        {
            vxReleaseImage(&obj->disparity_img[buf_id]);
        }

        vxReleaseScalar(&obj->vis_confidence_threshold);
        vxReleaseUserDataObject(&obj->sde_vis_config);
        vxReleaseNode(&obj->node_sde_disparity_vis);
        vxReleaseDistribution(&obj->histogram);

        if(obj->hist_output)
        {
            for(buf_id=0; buf_id<obj->num_buf; buf_id++)
            {
                vxReleaseImage(&obj->histogram_img[buf_id]);
            }
            vxReleaseNode(&obj->node_sde_histogram_vis);
        }

        if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (obj->display_option == 1))
        {
            vxReleaseNode(&obj->node_output_display);
            vxReleaseUserDataObject(&obj->output_display_config);
        }
        if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY2) && (obj->display_option == 1))
        {
            vxReleaseUserDataObject(&obj->mosaic_config);
            vxRemoveKernel(obj->mosaic_kernel);
            vxReleaseNode(&obj->mosaic_node);
            for (int id=0; id<obj->num_buf; id++)
            {
                vxReleaseImage(&obj->mosaic_output_image[id]);
            }
            vxReleaseNode(&obj->node_input_display);
            vxReleaseUserDataObject(&obj->input_display_config);
        }
    }

    vxReleaseGraph(&obj->graph_sde);
}

static void app_run_task(void *app_var)
{
    AppObj *obj = (AppObj *)app_var;

    appPerfStatsCpuLoadResetAll();

    app_run_graph(obj);

    obj->stop_task_done = 1;
}

static vx_status app_run_task_create(AppObj *obj)
{
    tivx_task_create_params_t params;
    vx_status status;

    tivxTaskSetDefaultCreateParams(&params);
    params.task_main = app_run_task;
    params.app_var = obj;

    obj->stop_task_done = 0;
    obj->stop_task = 0;

    status = tivxTaskCreate(&obj->task, &params);

    return status;
}

static void app_run_task_delete(AppObj *obj)
{
    while(obj->stop_task_done==0)
    {
         tivxTaskWaitMsecs(100);
    }

    tivxTaskDelete(&obj->task);
}

static const char menu[] = {
    "\n"
    "\n ================================="
    "\n Demo : Stereo Disparity Example"
    "\n ================================="
    "\n"
    "\n p: Print performance statistics"
    "\n"
    "\n e: Export performance statistics"
    "\n"
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};

static vx_status app_run_graph_interactive(AppObj *obj)
{
    vx_status status;
    uint32_t done = 0;
    char ch;
    FILE *fp;
    app_perf_point_t *perf_arr[1];

    status = app_run_task_create(obj);
    if(status != VX_SUCCESS)
    {
        printf("# ERROR: Unable to create task\n");
    }
    else
    {
        appPerfStatsResetAll();
        while(!done && (status == VX_SUCCESS))
        {
            printf(menu);
            ch = appGetChar();
            printf("\n");

            switch(ch)
            {
                case 'p':
                    appPerfStatsPrintAll();
                    status = tivx_utils_graph_perf_print(obj->graph_sde);
                    appPerfPointPrint(&obj->fileio_perf);
                    appPerfPointPrint(&obj->total_perf);
                    printf("\n");
                    appPerfPointPrintFPS(&obj->total_perf);
                    printf("\n");
                    appPerfPointReset(&obj->total_perf);
                    break;
                case 'e':
                    perf_arr[0] = &obj->total_perf;
                    fp = appPerfStatsExportOpenFile(".", "basic_demos_app_stereo");
                    if (NULL != fp)
                    {
                        appPerfStatsExportAll(fp, perf_arr, 1);
                        status = tivx_utils_graph_perf_export(fp, obj->graph_sde);
                        appPerfStatsExportCloseFile(fp);
                        appPerfStatsResetAll();
                    }
                    else
                    {
                        printf("fp is null\n");
                    }
                    break;
                case 'x':
                    obj->stop_task = 1;
                    done = 1;
                    break;
                default:
                    break;
            }
        }
        app_run_task_delete(obj);
    }
    return status;
}


static vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    uint32_t curFileNum;
    uint32_t iterations;
    char     fileExt[10];

    if (obj->bit_depth == 8)
    {
        strncpy(fileExt, ".bmp", sizeof(fileExt));
    } else
    {
        strncpy(fileExt, ".bin", sizeof(fileExt));
    }

    if(obj->pipeline_option==1)
    {
        obj->pipeline = -MAX_NUM_BUF;
        obj->enqueueCnt = 0;
    }

    obj->stop_task = 0;

    /* create output directory is not already existing */
    mkdir(obj->output_file_path, S_IRWXU | S_IRWXG | S_IRWXO);
    for(iterations = 1; iterations <= obj->num_iterations; iterations++)
    {
        if(0 != obj->stop_task)
        {
            break;
        }
        for(curFileNum = obj->start_fileno; curFileNum <= obj->end_fileno; curFileNum++)
        {
            if(0 != obj->stop_task)
            {
                break;
            }

            appPerfPointBegin(&obj->total_perf);

            snprintf(obj->left_input_file_name, APP_MAX_FILE_PATH, "%s/%010d%s",
                obj->left_input_file_path,
                curFileNum,
                fileExt
                );
            snprintf(obj->right_input_file_name, APP_MAX_FILE_PATH, "%s/%010d%s",
                obj->right_input_file_path,
                curFileNum,
                fileExt
                );

            if(obj->pipeline_option==1 && (status == VX_SUCCESS))
            {
                status = app_run_graph_for_one_frame_pipeline(obj, curFileNum);
            }
            else
            if(status == VX_SUCCESS)
            {
                status = app_run_graph_for_one_frame_sequential(obj, curFileNum);
            }
            appPerfPointEnd(&obj->total_perf);

            if(0 == obj->is_interactive)
            {
                printf(" %d of %d: Done !!!\n", curFileNum, obj->end_fileno);
            }
        }
        if(0 == obj->is_interactive)
        {
            printf("Iteration %d of %d ... Done.\n", iterations, obj->num_iterations);
        }
    }
    if(0 == obj->is_interactive)
    {
        printf("Ran %d times successfully!\n", obj->num_iterations * (obj->end_fileno-obj->start_fileno+1));
    }

    if((obj->pipeline_option==1) && (status == VX_SUCCESS))
    {
        status = vxWaitGraph(obj->graph_sde);
    }

    return status;
}

static vx_status app_run_graph_for_one_frame_pipeline(AppObj *obj, vx_int32 curFileNum)
{
    vx_status status = VX_SUCCESS;

    vx_int32 left_img_obj_array_idx = -1;
    vx_int32 right_img_obj_array_idx = -1;

    if(obj->pipeline < 0)
    {
        /* Read Inputs */
        appPerfPointBegin(&obj->fileio_perf);
        if(0 == obj->is_interactive)
        {
            printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, obj->left_input_file_name);
        }

        if (obj->bit_depth == 8)
        {
            tivx_utils_load_vximage_from_bmpfile(obj->input_img_left[obj->enqueueCnt], obj->left_input_file_name, vx_true_e);
        } else
        {
            load_vximage_from_bin_16(obj->input_img_left[obj->enqueueCnt], obj->left_input_file_name);
        }

        if(0 == obj->is_interactive)
        {
            printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, obj->right_input_file_name);
        }

        if (obj->bit_depth == 8)
        {
            tivx_utils_load_vximage_from_bmpfile(obj->input_img_right[obj->enqueueCnt], obj->right_input_file_name, vx_true_e);
        } else
        {
            load_vximage_from_bin_16(obj->input_img_right[obj->enqueueCnt], obj->right_input_file_name);
        }

        appPerfPointEnd(&obj->fileio_perf);

        /* Enqueue input - start execution */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph_sde, obj->input_arr_left_graph_parameter_index, (vx_reference*)&obj->input_arr_left[obj->enqueueCnt], 1);
        }
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph_sde, obj->input_arr_right_graph_parameter_index, (vx_reference*)&obj->input_arr_right[obj->enqueueCnt], 1);
        }
        if((obj->test_mode == 1) && (status == VX_SUCCESS))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph_sde, obj->mosaic_output_parameter_index, (vx_reference*)&obj->mosaic_output_image[obj->enqueueCnt], 1);
        }
        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= obj->num_buf)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }
    if(obj->pipeline >= 0)
    {
        vx_object_array input_array_left;
        vx_object_array input_array_right;
        uint32_t num_refs;
        vx_image mosaic_test_output;
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph_sde, obj->input_arr_left_graph_parameter_index, (vx_reference*)&input_array_left, 1, &num_refs);
        }
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph_sde, obj->input_arr_right_graph_parameter_index, (vx_reference*)&input_array_right, 1, &num_refs);
        }
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph_sde, obj->mosaic_output_parameter_index, (vx_reference*)&mosaic_test_output, 1, &num_refs);
        }
        /* Read Inputs */
        appPerfPointBegin(&obj->fileio_perf);
        if(status == VX_SUCCESS)
        {
            app_find_object_array_index(obj->input_arr_left,(vx_reference)input_array_left, obj->num_buf, &left_img_obj_array_idx);
        }
        if(left_img_obj_array_idx != -1)
        {
            if(0 == obj->is_interactive)
            {
                printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, obj->left_input_file_name);
            }
            if(status == VX_SUCCESS)
            {
                if (obj->bit_depth == 8)
                {
                    status = tivx_utils_load_vximage_from_bmpfile(obj->input_img_left[left_img_obj_array_idx], obj->left_input_file_name, vx_true_e);
                } else
                {
                    status = load_vximage_from_bin_16(obj->input_img_left[left_img_obj_array_idx], obj->left_input_file_name);
                }

            }
        }

        if(status == VX_SUCCESS)
        {
            app_find_object_array_index(obj->input_arr_right,(vx_reference)input_array_right, obj->num_buf, &right_img_obj_array_idx);
        }
        if(right_img_obj_array_idx != -1)
        {
            if(0 == obj->is_interactive)
            {
                printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, obj->right_input_file_name);
            }
            if(status == VX_SUCCESS)
            {
                if (obj->bit_depth == 8)
                {
                    status = tivx_utils_load_vximage_from_bmpfile(obj->input_img_right[right_img_obj_array_idx], obj->right_input_file_name, vx_true_e);
                } else
                {
                    status = load_vximage_from_bin_16(obj->input_img_right[right_img_obj_array_idx], obj->right_input_file_name);
                }

            }
        }

        /* Check that you are within the first n frames, where n is the number
            of samples in the checksums_expected */
        vx_uint32 expected_idx = curFileNum - obj->start_fileno - MAX_NUM_BUF;
        if((obj->test_mode == 1) && (expected_idx < (sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0]))) &&
            (left_img_obj_array_idx != -1) && (right_img_obj_array_idx != -1))
        {
            vx_uint32 left_img_actual_checksum = 0, right_img_actual_checksum = 0, mosaic_actual_checksum = 0;
            /* Check left_img, right_img, and the disparity map */
            if (app_test_check_image(obj->input_img_left[left_img_obj_array_idx],
                                    checksums_expected[0][expected_idx], &left_img_actual_checksum) == vx_false_e)
            {
                test_result = vx_false_e;
            }
            if (app_test_check_image(obj->input_img_right[right_img_obj_array_idx],
                                    checksums_expected[1][expected_idx], &right_img_actual_checksum) == vx_false_e)
            {
                test_result = vx_false_e;
            }
            if (app_test_check_image(mosaic_test_output, checksums_expected[2][expected_idx],
                                     &mosaic_actual_checksum) == vx_false_e)
            {
                test_result = vx_false_e;
            }
            /* in case test fails and needs to change */
            populate_gatherer(0, expected_idx, left_img_actual_checksum);
            populate_gatherer(1, expected_idx, right_img_actual_checksum);
            populate_gatherer(2, expected_idx, mosaic_actual_checksum);
        }
        appPerfPointEnd(&obj->fileio_perf);

        /* Enqueue input - start execution */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph_sde, obj->input_arr_left_graph_parameter_index, (vx_reference*)&input_array_left, 1);
        }
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph_sde, obj->input_arr_right_graph_parameter_index, (vx_reference*)&input_array_right, 1);
        }
        if((status == VX_SUCCESS) && (obj->test_mode == 1))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph_sde, obj->mosaic_output_parameter_index, (vx_reference*)&mosaic_test_output, 1);
        }
    }
    return status;
}

static vx_status app_run_graph_for_one_frame_sequential(AppObj *obj, vx_int32 curFileNum)
{
    vx_status status = VX_SUCCESS;

    char output_file_name_disparity_img[APP_MAX_FILE_PATH];
    char output_file_name_histogram_img[APP_MAX_FILE_PATH];
    char output_file_name_sde[APP_MAX_FILE_PATH];

    snprintf(output_file_name_disparity_img, APP_MAX_FILE_PATH, "%s/disp_%010d.bmp",
        obj->output_file_path,
        curFileNum
        );

    if(obj->hist_output)
    {
        snprintf(output_file_name_histogram_img, APP_MAX_FILE_PATH, "%s/hist_%010d.bmp",
            obj->output_file_path,
            curFileNum
            );
    }

    if (obj->sde_output)
    {
        snprintf(output_file_name_sde, APP_MAX_FILE_PATH, "%s/disp_%010d.sde",
            obj->output_file_path,
            curFileNum
            );
    }

    appPerfPointBegin(&obj->fileio_perf);

    if(0 == obj->is_interactive)
    {
        printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, obj->left_input_file_name);
    }
    if(status == VX_SUCCESS)
    {
        if (obj->bit_depth == 8)
        {
            status = tivx_utils_load_vximage_from_bmpfile(obj->input_img_left[0], obj->left_input_file_name, vx_true_e);
        } else
        {
            status = load_vximage_from_bin_16(obj->input_img_left[0], obj->left_input_file_name);
        }

    }
    if(0 == obj->is_interactive)
    {
        printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, obj->right_input_file_name);
    }
    if(status == VX_SUCCESS)
    {
        if (obj->bit_depth == 8)
        {
            status = tivx_utils_load_vximage_from_bmpfile(obj->input_img_right[0], obj->right_input_file_name, vx_true_e);
        } else
        {
            status = load_vximage_from_bin_16(obj->input_img_right[0], obj->right_input_file_name);
        }

    }
    appPerfPointEnd(&obj->fileio_perf);

    if(0 == obj->is_interactive)
    {
        printf(" %d of %d: Running graph ...\n", curFileNum, obj->end_fileno);
    }
    if(status == VX_SUCCESS)
    {
        status = vxScheduleGraph(obj->graph_sde);
    }
    if(status == VX_SUCCESS)
    {
        status = vxWaitGraph(obj->graph_sde);
    }

    if (vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1) && (1 == obj->display_option))
    {
    }
    else
    {
        if(0 == obj->is_interactive)
        {
            printf(" %d of %d: Saving [%s] ...\n", curFileNum, obj->end_fileno, output_file_name_disparity_img);
        }
        if(status == VX_SUCCESS)
        {
            status = tivx_utils_save_vximage_to_bmpfile(output_file_name_disparity_img, obj->disparity_img[0]);
        }
        // store histogram image
        if(obj->hist_output)
        {
            if(0 == obj->is_interactive)
            {
                printf(" %d of %d: Saving [%s] ...\n", curFileNum, obj->end_fileno, output_file_name_histogram_img);
            }
            if(status == VX_SUCCESS)
            {
                status = tivx_utils_save_vximage_to_bmpfile(output_file_name_histogram_img, obj->histogram_img[0]);
            }
        }

        // store sde file
        if(obj->sde_output)
        {
            if(0 == obj->is_interactive)
            {
                printf(" %d of %d: Saving [%s] ...\n", curFileNum, obj->end_fileno, output_file_name_sde);
            }
            if(status == VX_SUCCESS)
            {
                status = app_save_sde_output(output_file_name_sde, obj->disparity, obj, curFileNum);
            }
        }
    }

    return status;
}

static vx_status app_save_sde_output(char* filename, vx_image disparity, AppObj * obj, vx_uint32 curFileNum)
{
    vx_uint32   i;
    vx_uint32   width;
    vx_uint32   height;
    vx_map_id   map_id;
    void      * data_ptr;

    vx_rectangle_t             rect;
    vx_imagepatch_addressing_t image_addr;

    vx_status status;

    status = vxGetStatus((vx_reference)disparity);

    if(status == VX_SUCCESS)
    {
        vxQueryImage(disparity, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
        vxQueryImage(disparity, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = width;
        rect.end_y = height;

        status = vxMapImagePatch(disparity,
            &rect,
            0,
            &map_id,
            &image_addr,
            &data_ptr,
            VX_READ_ONLY,
            VX_MEMORY_TYPE_HOST,
            VX_NOGAP_X
            );

        if(status==VX_SUCCESS)
        {
            vx_int32 header[SDE_FILE_HEADER_LEN];
            memset(header, 0, SDE_FILE_HEADER_LEN * sizeof(vx_int32));

            // only write image width and height
            header[3] = (vx_uint32)width;
            header[4] = (vx_uint32)height;

            FILE *fp = fopen(filename,"wb");

            if(fp != NULL)
            {
                size_t ret;

                fwrite((void *)header, 4, SDE_FILE_HEADER_LEN, fp);

                ret = 0;
                for (i = 0; i < height; i++)
                {
                    ret += fwrite(data_ptr, image_addr.stride_x, width, fp);
                    data_ptr += image_addr.stride_y;
                }

                if(ret != width * height)
                {
                    printf("# ERROR: Unable to write data to file [%s]\n", filename);
                }
                fclose(fp);
            }
            else
            {
                printf("# ERROR: Unable to open file for writing [%s]\n", filename);
                status = VX_FAILURE;
            }

            vxUnmapImagePatch(disparity, map_id);
        }
    }

    if (obj->test_mode == 1)
    {
        /* Check that you are within the first n frames, where n is the number
            of samples in the checksums_expected */
        /* TODO: This looks like only inputs are being checked,  need to check output, as of now, may be
         * false positives. */
        vx_uint32 expected_idx = curFileNum - obj->start_fileno - MAX_NUM_BUF;
        if((obj->test_mode == 1) && (expected_idx < (sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0]))))
        {
            vx_uint32 left_img_actual_checksum = 0, right_img_actual_checksum = 0;
            /* Check left_img, right_img, and the disparity map */
            if (app_test_check_image(obj->input_img_left[0],
                                    checksums_expected[0][expected_idx], &left_img_actual_checksum) == vx_false_e)
            {
                test_result = vx_false_e;
            }
            if (app_test_check_image(obj->input_img_right[0],
                                    checksums_expected[1][expected_idx], &right_img_actual_checksum) == vx_false_e)
            {
                test_result = vx_false_e;
            }
            /* in case test fails and needs to change */
            populate_gatherer(0, expected_idx, left_img_actual_checksum);
            populate_gatherer(1, expected_idx, right_img_actual_checksum);
        }
    }
    return status;
}

static void app_show_usage(int argc, char* argv[])
{
    printf("\n");
    printf(" Stereo Depth Engine HWA Demo - (c) Texas Instruments 2018\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static void app_set_cfg_default(AppObj *obj)
{
    snprintf(obj->left_input_file_path, APP_MAX_FILE_PATH, "/test_data/psdkra/app_stereo/left");
    snprintf(obj->right_input_file_path, APP_MAX_FILE_PATH, "/test_data/psdkra/app_stereo/right");
    snprintf(obj->output_file_path, APP_MAX_FILE_PATH, "/test_data/output");
    snprintf(obj->output_file_prefix, APP_MAX_FILE_PATH, "out");

    obj->start_fileno    = 1;
    obj->end_fileno      = 3;
    obj->width           = OUTPUT_DISPLAY_WIDTH;
    obj->height          = OUTPUT_DISPLAY_HEIGHT;
    obj->hist_output     = 0;
    obj->sde_output      = 0;
    obj->display_option  = 0;
    obj->pipeline_option = 0;
    obj->num_iterations  = 10;
    obj->is_interactive  = 0;
    obj->bit_depth       = 8;
    obj->test_mode       = 0;
    obj->vis_confidence  = 1;

    obj->sde_params.median_filter_enable        = 0;
    obj->sde_params.reduced_range_search_enable = 0;
    obj->sde_params.disparity_min               = 0;
    obj->sde_params.disparity_max               = 1;
    obj->sde_params.threshold_left_right        = 0;
    obj->sde_params.texture_filter_enable       = 0;
    obj->sde_params.threshold_texture           = 0;
    obj->sde_params.aggregation_penalty_p1      = 0;
    obj->sde_params.aggregation_penalty_p2      = 0;

    // TBD
#if 0
    int i;
    for(i = 0; i < 8; i++) {
        obj->sde_params.confidence_score_map[i] = i*8;
    }
#else

    obj->sde_params.confidence_score_map[0] = 0;
    obj->sde_params.confidence_score_map[1] = 4;
    obj->sde_params.confidence_score_map[2] = 9;
    obj->sde_params.confidence_score_map[3] = 18;
    obj->sde_params.confidence_score_map[4] = 28;
    obj->sde_params.confidence_score_map[5] = 43;
    obj->sde_params.confidence_score_map[6] = 109;
    obj->sde_params.confidence_score_map[7] = 127;

#endif
}

static void app_parse_cfg_file(AppObj *obj, char *cfg_file_name)
{
    FILE * fp = fopen(cfg_file_name, "r");
    char   line_str[1024];
    char * token;
    char * basePath;

    if(fp==NULL)
    {
        printf("# ERROR: Unable to open config file [%s]\n", cfg_file_name);
        exit(0);
    }

    basePath = getenv("APP_STEREO_DATA_PATH");
    if (basePath == NULL)
    {
        printf("Please define APP_STEREO_DATA_PATH environment variable.\n");
        exit(-1);
    }

    while(fgets(line_str, sizeof(line_str), fp)!=NULL)
    {
        char s[]=" \t";

        if (strchr(line_str, '#'))
        {
            continue;
        }

        /* get the first token */
        token = strtok(line_str, s);
        if (NULL != token)
        {
            if(strcmp(token, "left_img_file_path")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    snprintf(obj->left_input_file_path, APP_MAX_FILE_PATH,
                         "%s/%s", basePath, token);
                }
            }
            else
            if(strcmp(token, "right_img_file_path")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    snprintf(obj->right_input_file_path, APP_MAX_FILE_PATH,
                         "%s/%s", basePath, token);
                }
            }
            else
            if(strcmp(token, "output_file_path")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    snprintf(obj->output_file_path, APP_MAX_FILE_PATH,
                         "%s/%s", basePath, token);
                }
            }
            else
            if(strcmp(token, "output_file_prefix")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->output_file_prefix, token);
                }
            }
            else
            if(strcmp(token, "start_seq")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->start_fileno = atoi(token);
                }
            }
            else
            if(strcmp(token, "end_seq")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->end_fileno = atoi(token);
                }
            }
            else
            if(strcmp(token, "width")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->width = atoi(token);
                }
            }
            else
            if(strcmp(token, "height")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->height = atoi(token);
                }
            }
            else
            if(strcmp(token, "visualize_confidence")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->vis_confidence = atoi(token);
                }
            }
            else
            if(strcmp(token, "hist_output")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->hist_output = atoi(token);
                }
            }
            else
            if(strcmp(token, "sde_output")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_output = atoi(token);
                }
            }
            else
            if(strcmp(token, "median_filter_enable")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.median_filter_enable = atoi(token);
                }
            }
            else
            if(strcmp(token, "reduced_range_search_enable")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.reduced_range_search_enable = atoi(token);
                }
            }
            else
            if(strcmp(token, "disparity_min")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.disparity_min = atoi(token);
                }
            }
            else
            if(strcmp(token, "disparity_max")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.disparity_max = atoi(token);
                }
            }
            else
            if(strcmp(token, "threshold_left_right")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.threshold_left_right = atoi(token);
                }
            }
            else
            if(strcmp(token, "texture_filter_enable")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.texture_filter_enable = atoi(token);
                }
            }
            else
            if(strcmp(token, "threshold_texture")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.threshold_texture = atoi(token);
                }
            }
            else
            if(strcmp(token, "aggregation_penalty_p1")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.aggregation_penalty_p1 = atoi(token);
                }
            }
            else
            if(strcmp(token, "aggregation_penalty_p2")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.aggregation_penalty_p2 = atoi(token);
                }
            }
            else
            if(strcmp(token, "confidence_score_map_0")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.confidence_score_map[0] = atoi(token);
                }
            }
            else
            if(strcmp(token, "confidence_score_map_1")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.confidence_score_map[1] = atoi(token);
                }
            }
            else
            if(strcmp(token, "confidence_score_map_2")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.confidence_score_map[2] = atoi(token);
                }
            }
            else
            if(strcmp(token, "confidence_score_map_3")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.confidence_score_map[3] = atoi(token);
                }
            }
            else
            if(strcmp(token, "confidence_score_map_4")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.confidence_score_map[4] = atoi(token);
                }
            }
            else
            if(strcmp(token, "confidence_score_map_5")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.confidence_score_map[5] = atoi(token);
                }
            }
            else
            if(strcmp(token, "confidence_score_map_6")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->sde_params.confidence_score_map[6] = atoi(token);
                }
            }
            else
            if(strcmp(token, "display_option")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->display_option = atoi(token);
                }
            }
            else
            if(strcmp(token, "num_iterations")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->num_iterations = atoi(token);
                }
            }
            else
            if(strcmp(token, "is_interactive")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->is_interactive = atoi(token);
                }
            }
            else
            if(strcmp(token, "bit_depth")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->bit_depth = atoi(token);
                }
            }
        }
    }

    fclose(fp);

    if(obj->width<128)
    {
        obj->width = 128;
    }
    if(obj->height<128)
    {
        obj->height = 128;
    }
    if(obj->end_fileno < obj->start_fileno)
    {
        obj->end_fileno = obj->start_fileno;
    }
    if(obj->display_option==1)
    {
        obj->pipeline_option=1;
    }
    else
    {
        obj->pipeline_option=0;
    }
    obj->sde_vis_params.disparity_min  = obj->sde_params.disparity_min;
    obj->sde_vis_params.disparity_max  = obj->sde_params.disparity_max;
    obj->sde_vis_params.disparity_only = 0;
    obj->sde_vis_params.vis_confidence = obj->vis_confidence;
#if 0
    // debug - to see config params
    printf("median_filter_enable:%d\n", obj->sde_params.median_filter_enable);
    printf("reduced_range_search_enable:%d\n", obj->sde_params.reduced_range_search_enable);
    printf("disparity_min:%d\n", obj->sde_params.disparity_min);
    printf("disparity_max:%d\n", obj->sde_params.disparity_max);
    printf("threshold_left_right:%d\n", obj->sde_params.threshold_left_right);
    printf("texture_filter_enable:%d\n", obj->sde_params.texture_filter_enable);
    printf("threshold_texture:%d\n", obj->sde_params.threshold_texture);
    printf("aggregation_penalty_p1:%d\n", obj->sde_params.aggregation_penalty_p1);
    printf("aggregation_penalty_p2:%d\n", obj->sde_params.aggregation_penalty_p2);
    printf("display_option:%d\n", obj->display_option);

    printf("confidence:\n");
    for(int i = 0; i < 8; i++) {
       printf("%d ", obj->sde_params.confidence_score_map[i]);
    }
    printf("\n");
#endif
}

static void app_parse_cmd_line_args(AppObj *obj, int argc, char *argv[])
{
    int i;
    vx_bool set_test_mode = vx_false_e;

    app_set_cfg_default(obj);

    if(argc==1)
    {
            app_show_usage(argc, argv);
            exit(0);
    }

    for(i=0; i<argc; i++)
    {
        if(strcmp(argv[i], "--cfg")==0)
        {
            i++;
            if(i>=argc)
            {
                app_show_usage(argc, argv);
            }
            app_parse_cfg_file(obj, argv[i]);
        }
        else
        if(strcmp(argv[i], "--help")==0)
        {
            app_show_usage(argc, argv);
            exit(0);
        }
        else
        if(strcmp(argv[i], "--test")==0)
        {
            set_test_mode = vx_true_e;
        }
    }

    if (set_test_mode == vx_true_e)
    {
        uint32_t temp_end_fileno;
        obj->test_mode = 1;
        obj->is_interactive = 0;
        obj->num_iterations = 1;
        /* if testing, just run the number of frames that are found in the expected
            checksums + a BUFFER to maintain data integrity (-1 bc of the <= comparison
            in the loop) */
        temp_end_fileno = obj->start_fileno +
                            sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0])
                            + TEST_BUFFER - 1;
        if ( obj->end_fileno > temp_end_fileno)
        {
            obj->end_fileno = temp_end_fileno;
        }
    }
}
#ifndef x86_64
#ifndef SOC_J722S
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
  AppObj *obj = &gAppObj;

  appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

  if(update_type == 0)
  {
    Draw2D_FontPrm sHeading;
    Draw2D_FontPrm sAlgo1;
    Draw2D_FontPrm sAlgo2;
    Draw2D_FontPrm sAlgo3;
    Draw2D_FontPrm sAlgo4;
    Draw2D_FontPrm sAlgo5;
    Draw2D_FontPrm sAlgo6;
    Draw2D_FontPrm sAlgo7;

    Draw2D_BmpPrm bmp;

    bmp.bmpIdx = DRAW2D_BMP_IDX_SDE_COLOUR_MAP;
    Draw2D_drawBmp(handle, (1920-OUTPUT_DISPLAY_WIDTH)/2 - 50, 1080-OUTPUT_DISPLAY_HEIGHT-200, &bmp);

    sHeading.fontIdx = 5;
    Draw2D_drawString(handle, 625, 10, "Stereo Vision - Demo", &sHeading);

    sAlgo1.fontIdx = 1;
    Draw2D_drawString(handle, 100 , 1080-OUTPUT_DISPLAY_HEIGHT-250-INPUT_DISPLAY_HEIGHT-25 + INPUT_DISPLAY_HEIGHT/2 +10 , "Left Camera Input", &sAlgo1);

    sAlgo2.fontIdx = 1;
    Draw2D_drawString(handle, 1920-100-350 , 1080-OUTPUT_DISPLAY_HEIGHT-250-INPUT_DISPLAY_HEIGHT-25 + INPUT_DISPLAY_HEIGHT/2 +10, "Right Camera Input", &sAlgo2);

    char resolution[30];
    snprintf(resolution, 30, "%s%d%s%d%s","Resolution : ",obj->width,"px x ",obj->height,"px");
    sAlgo3.fontIdx = 3;
    Draw2D_drawString(handle, (1920-OUTPUT_DISPLAY_WIDTH)/2 + OUTPUT_DISPLAY_WIDTH/2 - 140 , 1080-OUTPUT_DISPLAY_HEIGHT-250 - 25, resolution , &sAlgo3);

    sAlgo4.fontIdx = 1;
    Draw2D_drawString(handle, (1920-OUTPUT_DISPLAY_WIDTH)/2 + 100 , 1080-OUTPUT_DISPLAY_HEIGHT-250 + OUTPUT_DISPLAY_HEIGHT +10 , "Stereo Vision Output", &sAlgo4);

    sAlgo5.fontIdx = 3;
    Draw2D_drawString(handle, 330 , 1080-OUTPUT_DISPLAY_HEIGHT-250 + OUTPUT_DISPLAY_HEIGHT/2 , "Stereo Vision Colour Map", &sAlgo5);

    sAlgo6.fontIdx = 3;
    Draw2D_drawString(handle, (1920-OUTPUT_DISPLAY_WIDTH)/2 - 100 , 1080-OUTPUT_DISPLAY_HEIGHT-200 , "Far", &sAlgo6);

    sAlgo7.fontIdx = 3;
    Draw2D_drawString(handle, (1920-OUTPUT_DISPLAY_WIDTH)/2 - 100 , 1080-OUTPUT_DISPLAY_HEIGHT-200 + 376 - 20 , "Near", &sAlgo7);
  }

  return;
}
#endif
#endif

static void app_find_object_array_index(vx_object_array object_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx)
{
  vx_int32 i;

  *array_idx = -1;
  for(i = 0; i < array_size; i++)
  {
    if(ref == (vx_reference)object_array[i])
    {
      *array_idx = i;

      break;
    }
  }
}



