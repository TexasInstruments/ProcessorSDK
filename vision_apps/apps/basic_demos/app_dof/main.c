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

#include "dof_common.h"
#include "dof_pyramid_module.h"
#include "dof_proc_module.h"
#include "dof_viz_module.h"
#include "dof_display_module.h"
#include "dof_test.h"
#include <TI/hwa_kernels.h>
#include <TI/video_io_kernels.h>

typedef struct {

    /* config options */
    char input_file_path[APP_MAX_FILE_PATH];
    char output_file_path[APP_MAX_FILE_PATH];
    char input_file_prefix[APP_MAX_FILE_PATH];
    char input_file_postfix[APP_MAX_FILE_PATH];
    char output_file_prefix[APP_MAX_FILE_PATH];
    uint16_t in_file_format;
    vx_df_image in_vx_df_image;
    char in_file_ext[8];
    uint16_t out_file_format;
    char out_file_ext[8];
    int32_t start_fileno;
    int32_t end_fileno;
    uint32_t width;
    uint32_t height;
    uint32_t dof_levels;
    vx_bool save_intermediate_output;
    uint32_t enable_temporal_predicton_flow_vector;

    /* OpenVX references */
    vx_context context;
    vx_graph graph;

    PyramidObj pyramidObj;
    DofProcObj dofprocObj;
    DofVizObj  dofvizObj;
    DisplayObj displayObj;


    vx_uint32 num_iterations;
    vx_int32 input_img_graph_parameter_index;
    vx_int32 flow_vector_field_img_graph_parameter_index;
    vx_int32 confidence_img_graph_parameter_index;

    #ifndef SOC_J722S
    Draw2D_Handle  pHndl;
    #endif

    uint32_t is_interactive;
    uint32_t test_mode;

    tivx_task task;
    uint32_t stop_task;
    uint32_t stop_task_done;

    app_perf_point_t total_perf;
    app_perf_point_t fileio_perf;

} AppObj;

int app_dof_main(int argc, char* argv[]);
static void app_parse_cmd_line_args(AppObj *obj, int argc, char *argv[]);
static vx_status app_init(AppObj *obj);
static void app_deinit(AppObj *obj);
static vx_status app_create_graph(AppObj *obj);
static vx_status app_run_graph(AppObj *obj);
static vx_status app_run_graph_interactive(AppObj *obj);
static void app_delete_graph(AppObj *obj);
static void app_run_task(void *app_var);
static vx_status app_run_task_create(AppObj *obj);
static void app_run_task_delete(AppObj *obj);
static void app_update_param_set(AppObj *obj);
static void update_pre_proc_params_pyramid( AppObj *obj, PyramidObj *pyramidObj);
static void update_pre_proc_params_dofviz( AppObj *obj, DofVizObj *dofvizObj);
static void update_pre_proc_params_display( AppObj *obj, DisplayObj *displayObj);
static vx_status app_save_vximage_to_bin_file(char *filename, vx_image image);
static vx_status app_run_pyramid_for_first_frame(AppObj *obj);
static vx_status app_load_vximage_from_bin_or_yuv_file(char *filename, vx_image image, uint16_t file_format);
static vx_status add_graph_parameter_by_node_index(vx_graph graph, vx_node node,vx_uint32 node_parameter_index);
static void app_find_image_array_index(vx_image image_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx);
#ifndef SOC_J722S
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type);
#endif

AppObj gAppObj;

/* checksums for the confidence image and the flow vector image
    confidence image (at least on plane 0) remains constant through
    the video whereas field vectore changes */


int app_dof_main(int argc, char* argv[])
{
    AppObj *obj = &gAppObj;
    vx_status status = VX_SUCCESS;

    /*Config parameter reading*/
    app_parse_cmd_line_args(obj, argc, argv);

    /*Update of parameters are config file read*/
    app_update_param_set(obj);

    status = app_init(obj);
    if(status == VX_SUCCESS)
    {
        status = app_create_graph(obj);
    }
    if((obj->is_interactive) && (status == VX_SUCCESS))
    {
        status = app_run_graph_interactive(obj);
    }
    else
    if(status == VX_SUCCESS)
    {
        status = app_run_graph(obj);
    }
    app_delete_graph(obj);
    app_deinit(obj);
    if (obj->test_mode)
    {
        if((test_result == vx_false_e) || (status != VX_SUCCESS))
        {
            printf("\n\nTEST FAILED\n\n");
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


/*
 * Utility API used to add a graph parameter from a node, node parameter index
 */
static vx_status add_graph_parameter_by_node_index(vx_graph graph, vx_node node,
    vx_uint32 node_parameter_index)
{
    vx_status status = VX_SUCCESS;
    vx_parameter parameter = vxGetParameterByIndex(node, node_parameter_index);
    if (status == VX_SUCCESS)
    {
        status = vxAddParameterToGraph(graph, parameter);
    }
    if (status == VX_SUCCESS)
    {
        status = vxReleaseParameter(&parameter);
    }
    return status;
}

static void app_get_file_format_ext(uint16_t file_format, char *file_ext)
{
    strcpy(file_ext, "bmp");
    if(file_format==APP_FILE_FORMAT_BMP)
    {
        strcpy(file_ext, "bmp");
    }
    else
    if(file_format==APP_FILE_FORMAT_BIN12B_UNPACKED)
    {
        strcpy(file_ext, "bin");
    }
    else
    if(file_format==APP_FILE_FORMAT_YUV)
    {
        strcpy(file_ext, "yuv");
    }
}

static vx_status app_load_vximage_from_file(vx_image img, char *filename, uint16_t file_format)
{
    vx_status status = VX_SUCCESS;

    if((file_format==APP_FILE_FORMAT_BMP) && (status == VX_SUCCESS))
    {
        status = tivx_utils_load_vximage_from_bmpfile(img, filename, vx_true_e);
    }
    else
    if(((file_format==APP_FILE_FORMAT_BIN12B_UNPACKED)||(file_format==APP_FILE_FORMAT_YUV)) && (status == VX_SUCCESS))
    {
        status = app_load_vximage_from_bin_or_yuv_file(filename, img, file_format);
    }
    return status;
}

static vx_status app_save_vximage_to_file(char *filename, vx_image img, uint16_t file_format)
{
    vx_status status = VX_SUCCESS;

    if((file_format==APP_FILE_FORMAT_BMP) && (status == VX_SUCCESS))
    {
        status = tivx_utils_save_vximage_to_bmpfile(filename, img);
    }
    else
    if((file_format==APP_FILE_FORMAT_BIN12B_UNPACKED) && (status == VX_SUCCESS))
    {
        status = app_save_vximage_to_bin_file(filename, img);
    }
    return status;
}

static vx_status app_init(AppObj *obj)
{
    #ifndef SOC_J722S
    app_grpx_init_prms_t grpx_prms;
    #endif
    vx_status status = VX_SUCCESS;

    obj->context = vxCreateContext();
    if(status == VX_SUCCESS)
    {
        status = vxGetStatus((vx_reference) obj->context);
    }

    tivxHwaLoadKernels(obj->context);

    tivxVideoIOLoadKernels(obj->context);

    /* Initialize modules */
    if(status == VX_SUCCESS)
    {
        status = app_init_pyramid(obj->context, &obj->pyramidObj, "pyramidObj", MAX_NUM_BUF );
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_dofproc(obj->context, &obj->dofprocObj, "dofprocObj");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_dofviz( obj->context, &obj->dofvizObj , "dofvizObj");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_display1( obj->context, &obj->displayObj , "display1Obj");
    }
    if(status == VX_SUCCESS)
    {
        status = app_init_display2( obj->context, &obj->displayObj , "display2Obj");
    }

    if (1 == obj->displayObj.display_option)
    {
        #ifndef SOC_J722S
        appGrpxInitParamsInit(&grpx_prms, obj->context);
        grpx_prms.draw_callback = app_draw_graphics;
        appGrpxInit(&grpx_prms);
        #endif
    }

    return status;
}

static void app_deinit(AppObj *obj)
{
    app_deinit_pyramid(&obj->pyramidObj, MAX_NUM_BUF);
    app_deinit_dofproc(&obj->dofprocObj);
    app_deinit_dofviz(&obj->dofvizObj);

    app_deinit_display1(&obj->displayObj);
    app_deinit_display2(&obj->displayObj);

    if (1 == obj->displayObj.display_option)
    {
        #ifndef SOC_J722S
        appGrpxDeInit();
        #endif
    }

    tivxVideoIOUnLoadKernels(obj->context);
    tivxHwaUnLoadKernels(obj->context);
    vxReleaseContext(&obj->context);
}

/*
 * Graph (APP_DISPLAY_MODE 0)
 *                                                dof_config
 *                                                   |
 *                                                   v
 * input_img -> GaussianPyramid -----> pyr_cur ---> DmpacDof -> flow_vector_field_out -> DofVisualize --- > flow_vector_field_img
 *                               |                   ^  ^                |                   |
 *                              pyr_delay            |  |       flow_vector_field_delay      +------------> confidence_img
 *                               |                   |  |                |
 *                               +----> pyr_ref -----+  +------- flow_vector_field_in
 *
 * Graph (APP_DISPLAY_MODE 1)
 *                                                dof_config
 *      +--> Display2                                             |
 *      |                                            v
 * input_img -> GaussianPyramid -----> pyr_cur ---> DmpacDof -> flow_vector_field_out -> DofVisualize --- > flow_vector_field_img -> Display1
 *                               |                   ^  ^                |                   |
 *                              pyr_delay            |  |       flow_vector_field_delay      +------------> confidence_img
 *                               |                   |  |                |
 *                               +----> pyr_ref -----+  +------- flow_vector_field_in
 */
static vx_status app_create_graph(AppObj *obj)
{
    uint32_t num_buf, pipeline_depth;
    vx_graph_parameter_queue_params_t graph_parameters_queue_params_list[3] = {0};
    vx_status status = VX_SUCCESS;


    num_buf = MAX_NUM_BUF;
    pipeline_depth = MAX_NUM_BUF;

    obj->graph = vxCreateGraph(obj->context);

    status = vxGetStatus((vx_reference) obj->graph);
    if(status == VX_SUCCESS)
    {
        status = app_create_graph_display2(obj->graph,
                                            &obj->displayObj,
                                            obj->pyramidObj.input_img[0]);
    }
    if (status == VX_SUCCESS)
    {
        status = app_create_graph_pyramid(obj->graph,
                                            &obj->pyramidObj,
                                            obj->pyramidObj.input_img[0],
                                            obj->pyramidObj.pyr_ref );
    }
    if (status == VX_SUCCESS)
    {
        status = app_create_graph_dofproc(obj->graph,
                                            &obj->dofprocObj,
                                            obj->dofprocObj.dof_config,
                                            obj->pyramidObj.pyr_cur,
                                            obj->pyramidObj.pyr_ref,
                                            obj->dofprocObj.flow_vector_field_in,
                                            obj->dofprocObj.flow_vector_field_out );
    }
    if (status == VX_SUCCESS)
    {
        status = app_create_graph_dofviz(obj->graph,
                                            &obj->dofvizObj,
                                            obj->dofprocObj.flow_vector_field_out,
                                            obj->dofvizObj.confidence_threshold,
                                            obj->dofvizObj.flow_vector_field_img[0],
                                            obj->dofvizObj.confidence_img[0]);
    }
    if (status == VX_SUCCESS)
    {
        status = app_create_graph_display1(obj->graph, &obj->displayObj, obj->dofvizObj.flow_vector_field_img[0]);
    }
    if (status == VX_SUCCESS)
    {
        status = vxRegisterAutoAging(obj->graph, obj->pyramidObj.pyr_delay);
    }

    if(obj->dofprocObj.enable_temporal_predicton_flow_vector && (status == VX_SUCCESS))
    {
        status = vxRegisterAutoAging(obj->graph,obj->dofprocObj.flow_vector_field_delay);
    }

    int graph_parameter_num = 0;
        /* Set graph schedule config such that graph parameter @ index 0 is
         * enqueuable */

    if(status == VX_SUCCESS)
    {
        status = add_graph_parameter_by_node_index(obj->graph, obj->pyramidObj.node, 0);
        obj->input_img_graph_parameter_index = graph_parameter_num;
        graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = num_buf;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&obj->pyramidObj.input_img[0];
        graph_parameter_num++;
    }

    if(status == VX_SUCCESS)
    {
        status = add_graph_parameter_by_node_index(obj->graph, obj->dofvizObj.node, 2);
        obj->flow_vector_field_img_graph_parameter_index = graph_parameter_num;
        graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = num_buf;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&obj->dofvizObj.flow_vector_field_img[0];
        graph_parameter_num++;
    }

    if(status == VX_SUCCESS)
    {
        status = add_graph_parameter_by_node_index(obj->graph, obj->dofvizObj.node, 3);
        obj->confidence_img_graph_parameter_index = graph_parameter_num;
        graph_parameters_queue_params_list[graph_parameter_num].graph_parameter_index = graph_parameter_num;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list_size = num_buf;
        graph_parameters_queue_params_list[graph_parameter_num].refs_list = (vx_reference*)&obj->dofvizObj.confidence_img[0];
        graph_parameter_num++;
    }

    if(status == VX_SUCCESS)
    {
        status = vxSetGraphScheduleConfig(obj->graph,
                            VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                            graph_parameter_num,
                            graph_parameters_queue_params_list);
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetGraphPipelineDepth(obj->graph, pipeline_depth);
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->pyramidObj.node, 1, num_buf);
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->dofprocObj.node, 8, num_buf);
    }
    if(status == VX_SUCCESS)
    {
        status = vxVerifyGraph(obj->graph);
    }

    appPerfPointSetName(&obj->total_perf , "TOTAL");
    appPerfPointSetName(&obj->fileio_perf, "FILEIO");

    return status;
}

static void app_delete_graph(AppObj *obj)
{
    app_delete_pyramid(&obj->pyramidObj);
    app_delete_dofproc(&obj->dofprocObj);
    app_delete_dofviz(&obj->dofvizObj);
    app_delete_display1(&obj->displayObj);
    app_delete_display2(&obj->displayObj);
    vxReleaseGraph(&obj->graph);
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
    "\n Demo : Dense Optical Flow Example 1"
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
    vx_status status = VX_SUCCESS;
    uint32_t done = 0;
    char ch;
    FILE *fp;
    app_perf_point_t *perf_arr[1];

    status = app_run_task_create(obj);
    if(status!=0)
    {
        printf("app_dof_example: ERROR: Unable to create task\n");
    }
    else
    {
        appPerfStatsResetAll();
        while((!done) && (status == VX_SUCCESS))
        {
            printf(menu);
            ch = appGetChar();
            printf("\n");

            switch(ch)
            {
                case 'p':
                    appPerfStatsPrintAll();
                    status = tivx_utils_graph_perf_print(obj->graph);
                    appPerfPointPrint(&obj->fileio_perf);
                    appPerfPointPrint(&obj->total_perf);
                    printf("\n");
                    appPerfPointPrintFPS(&obj->total_perf);
                    appPerfPointReset(&obj->total_perf);
                    printf("\n");
                    break;
                case 'e':
                    perf_arr[0] = &obj->total_perf;
                    fp = appPerfStatsExportOpenFile(".", "basic_demos_app_dense_optical_flow");
                    if (NULL != fp)
                    {
                        appPerfStatsExportAll(fp, perf_arr, 1);
                        status = tivx_utils_graph_perf_export(fp, obj->graph);
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
            }
        }
        app_run_task_delete(obj);
    }
    return status;
}

static vx_status app_run_graph(AppObj *obj)
{
    char input_file_name[APP_MAX_FILE_PATH];
    char output_file_name_flow_img[APP_MAX_FILE_PATH];
    char output_file_name_conf_img[APP_MAX_FILE_PATH];
    uint32_t curFileNum;
    int32_t outputFileNum;
    uint32_t iterations = 1;
    vx_status status = VX_SUCCESS;

    uint32_t num_buf;
    num_buf = MAX_NUM_BUF;

    int32_t pipeline = -num_buf;
    int32_t enqueueCnt = 0;

    vx_int32 img_array_idx = -1;

    /* create output directory is not already existing */
    mkdir(obj->output_file_path, S_IRWXU | S_IRWXG | S_IRWXO);

    /* run pyramid only for 1st frame */
    status = app_run_pyramid_for_first_frame(obj);

    /* run DOF for 2nd frame onwards */

    vx_uint32 test_counter = 0;
    for(curFileNum = obj->start_fileno+1; curFileNum <= obj->end_fileno; curFileNum++)
    {
        if(obj->stop_task)
        {
            break;
        }

        appPerfPointBegin(&obj->total_perf);

        snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%s%05d%s.%s",
            obj->input_file_path,
            obj->input_file_prefix,
            curFileNum,
            obj->input_file_postfix,
            obj->in_file_ext
            );
        if(pipeline < 0)
        {
            /* Enqueue outpus */
            if (status == VX_SUCCESS)
            {
                status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->flow_vector_field_img_graph_parameter_index, (vx_reference*)&obj->dofvizObj.flow_vector_field_img[enqueueCnt], 1);
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->confidence_img_graph_parameter_index, (vx_reference*)&obj->dofvizObj.confidence_img[enqueueCnt], 1);
                }
            }

            if(0 == obj->is_interactive)
            {
                printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, input_file_name);
            }
            appPerfPointBegin(&obj->fileio_perf);
            /* Read input */
            if (status == VX_SUCCESS)
            {
                status = app_load_vximage_from_file(obj->pyramidObj.input_img[enqueueCnt], input_file_name, obj->in_file_format);
            }

            appPerfPointEnd(&obj->fileio_perf);

            /* Enqueue input - start execution */
            if (status == VX_SUCCESS)
            {
                status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->input_img_graph_parameter_index, (vx_reference*)&obj->pyramidObj.input_img[enqueueCnt], 1);
            }

            enqueueCnt++;
            enqueueCnt   = (enqueueCnt  >= num_buf)? 0 : enqueueCnt;
            pipeline++;
        }
        else if(pipeline >= 0)
        {
            vx_image input_image;
            vx_image flow_vector_field_image;
            vx_image confidence_image;
            uint32_t num_refs;
            vx_uint32 actual_checksum = 0;

            /* Dequeue input */
            if (status == VX_SUCCESS)
            {
                status = vxGraphParameterDequeueDoneRef(obj->graph, obj->input_img_graph_parameter_index, (vx_reference*)&input_image, 1, &num_refs);
            }

            if ((vx_true_e == tivxIsTargetEnabled(TIVX_TARGET_DISPLAY1)) && (1 == obj->displayObj.display_option) && (!obj->test_mode))
            {
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterDequeueDoneRef(obj->graph, obj->flow_vector_field_img_graph_parameter_index, (vx_reference*)&flow_vector_field_image, 1, &num_refs);
                }
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterDequeueDoneRef(obj->graph, obj->confidence_img_graph_parameter_index, (vx_reference*)&confidence_image, 1, &num_refs);
                }
                /* Enqueue output */
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->flow_vector_field_img_graph_parameter_index, (vx_reference*)&flow_vector_field_image, 1);
                }
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->confidence_img_graph_parameter_index, (vx_reference*)&confidence_image, 1);
                }
            }
            else
            {
                /* Dequeue & Save output */
                outputFileNum = curFileNum - num_buf;
                outputFileNum = (outputFileNum <=obj->start_fileno) ? (obj->end_fileno-obj->start_fileno+outputFileNum+1) : outputFileNum;

                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterDequeueDoneRef(obj->graph, obj->flow_vector_field_img_graph_parameter_index, (vx_reference*)&flow_vector_field_image, 1, &num_refs);
                    snprintf(output_file_name_flow_img, APP_MAX_FILE_PATH, "%s/%sflov_%05d.%s",
                                obj->output_file_path,
                                obj->output_file_prefix,
                                outputFileNum,
                                obj->out_file_ext
                                );
                }

                if ((status == VX_SUCCESS) && (!obj->test_mode))
                {
                    if(0 == obj->is_interactive)
                    {
                        printf(" %d of %d: Saving [%s] ...\n", outputFileNum, obj->end_fileno, output_file_name_flow_img);
                    }
                    status = app_save_vximage_to_file(output_file_name_flow_img, flow_vector_field_image, obj->out_file_format);
                }
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterDequeueDoneRef(obj->graph, obj->confidence_img_graph_parameter_index, (vx_reference*)&confidence_image, 1, &num_refs);
                    snprintf(output_file_name_conf_img, APP_MAX_FILE_PATH, "%s/%sconf_%05d.%s",
                        obj->output_file_path,
                        obj->output_file_prefix,
                        outputFileNum,
                        obj->out_file_ext
                        );
                }
                if ((status == VX_SUCCESS) && (!obj->test_mode))
                {
                    if(0 == obj->is_interactive)
                    {
                        printf(" %d of %d: Saving [%s] ...\n", outputFileNum, obj->end_fileno, output_file_name_conf_img);
                    }
                    status = app_save_vximage_to_file(output_file_name_conf_img, confidence_image, obj->out_file_format);
                }

                if((status == VX_SUCCESS) && (obj->test_mode == 1))
                {
                    if (app_test_check_image(confidence_image, checksums_expected[0][test_counter], &actual_checksum) == vx_false_e)
                    {
                        test_result = vx_false_e;
                        populate_gatherer(0, test_counter, actual_checksum);
                    }
                    if (app_test_check_image(flow_vector_field_image, checksums_expected[1][test_counter], &actual_checksum) == vx_false_e)
                    {
                        test_result = vx_false_e;
                        populate_gatherer(1, test_counter, actual_checksum);
                    }
                }
                /* Enqueue output */
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->flow_vector_field_img_graph_parameter_index, (vx_reference*)&flow_vector_field_image, 1);
                }
                if (status == VX_SUCCESS)
                {
                    status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->confidence_img_graph_parameter_index, (vx_reference*)&confidence_image, 1);
                }
            }
            if (status == VX_SUCCESS)
            {
                app_find_image_array_index(obj->pyramidObj.input_img,(vx_reference)input_image, num_buf, &img_array_idx);
            }
            if(img_array_idx != -1)
            {
                if(0 == obj->is_interactive)
                {
                    printf(" %d of %d: Loading [%s] ...\n", curFileNum, obj->end_fileno, input_file_name);
                }
                appPerfPointBegin(&obj->fileio_perf);
                if (status == VX_SUCCESS)
                {
                    status = app_load_vximage_from_file(obj->pyramidObj.input_img[img_array_idx], input_file_name, obj->in_file_format);
                }
                appPerfPointEnd(&obj->fileio_perf);
            }

            /* Enqueue input - start execution */
            if (status == VX_SUCCESS)
            {
                status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->input_img_graph_parameter_index, (vx_reference*)&input_image, 1);
            }
            test_counter++;
        }
       appPerfPointEnd(&obj->total_perf);

       /* Run for num_iterations - loop to first image if we're on the last image */
       if ((curFileNum == obj->end_fileno) && (iterations < obj->num_iterations))
       {
            test_counter = 0;
            if(0 == obj->is_interactive)
            {
                printf("Iteration %d of %d: Done !!!\n", iterations, obj->num_iterations);
            }

            #if 1 //def APP_DEBUG
            appPerfPointPrintFPS(&obj->total_perf);
            appPerfPointReset(&obj->total_perf);
            if(iterations==1)
            {
                /* after first iteration reset performance stats */
                appPerfStatsResetAll();
            }
            #endif

            curFileNum = obj->start_fileno - 1u;
            iterations++;
       }
    }

    if(0 == obj->is_interactive)
    {
        printf("Ran %d times successfully!\n", obj->num_iterations * (obj->end_fileno-obj->start_fileno));
    }
    if (status == VX_SUCCESS)
    {
        status = vxWaitGraph(obj->graph);
    }
    obj->stop_task = 1;

    return status;
}

/*
 * For 1st frame, DOF should not be run, only pyramid is run
 * For 2nd frame onwards, DOF is run with 1st frame pyramid as reference
 */
static vx_status app_run_pyramid_for_first_frame(AppObj *obj)
{
    vx_graph graph;
    vx_image input_img;
    vx_pyramid pyr;
    vx_node node_pyr;
    vx_status status = VX_SUCCESS;
    char input_file_name[APP_MAX_FILE_PATH];


    graph = vxCreateGraph(obj->context);
    APP_ASSERT_VALID_REF(graph);

    input_img = vxCreateImage(obj->context, obj->width, obj->height, obj->in_vx_df_image);
    APP_ASSERT_VALID_REF(input_img);

    pyr = (vx_pyramid)vxGetReferenceFromDelay(obj->pyramidObj.pyr_delay, 0);
    APP_ASSERT_VALID_REF(pyr);

    node_pyr = vxGaussianPyramidNode(
                            graph,
                            input_img,
                            pyr);
    APP_ASSERT_VALID_REF(node_pyr);
    status = vxSetNodeTarget(node_pyr, VX_TARGET_STRING, TIVX_TARGET_VPAC_MSC1);
    APP_ASSERT(status==VX_SUCCESS);

    snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%s%05d%s.%s",
        obj->input_file_path,
        obj->input_file_prefix,
        obj->start_fileno,
        obj->input_file_postfix,
        obj->in_file_ext
        );

    if(0 == obj->is_interactive)
    {
        printf(" %d of %d: Loading [%s] ...\n", obj->start_fileno, obj->end_fileno, input_file_name);
    }
    app_load_vximage_from_file(input_img, input_file_name, obj->in_file_format);

    if(0 == obj->is_interactive)
    {
        printf(" %d of %d: Running pyramid graph ...\n", obj->start_fileno, obj->end_fileno);
    }
    status = vxScheduleGraph(graph);
    APP_ASSERT(status==VX_SUCCESS);
    status = vxWaitGraph(graph);
    APP_ASSERT(status==VX_SUCCESS);

    vxReleaseNode(&node_pyr);
    vxReleaseGraph(&graph);
    vxReleaseImage(&input_img);

    return status;
}

static vx_status app_save_vximage_to_bin_file(char *filename, vx_image image)
{
    vx_uint32 width, height;
    vx_imagepatch_addressing_t image_addr;
    vx_rectangle_t rect;
    vx_map_id map_id;
    vx_df_image df;
    void *data_ptr;
    vx_status status;

    status = vxGetStatus((vx_reference)image);

    if(status==VX_SUCCESS)
    {
        vxQueryImage(image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_FORMAT, &df, sizeof(vx_df_image));

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = width;
        rect.end_y = height;

        status = vxMapImagePatch(image,
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
            FILE *fp = fopen(filename,"wb");

            if(fp!=NULL)
            {
                vx_int32 num_bytes = 0;
                vx_int32 j;

                for (j = 0; j < height; j++)
                {
                    num_bytes += fwrite(data_ptr, image_addr.stride_x, width, fp);
                    data_ptr += image_addr.stride_y;
                }

                if(num_bytes != (width*image_addr.stride_x*height))
                    printf("Luma bytes written = %d, expected = %d\n", num_bytes, width*image_addr.stride_x*height);

                fclose(fp);
            }
            else
            {
                printf("# ERROR: Unable to open file for writing [%s]\n", filename);
                status = VX_FAILURE;
            }
            vxUnmapImagePatch(image, map_id);
        }
    }
    return status;
}

static vx_status app_load_vximage_from_bin_or_yuv_file(char *filename, vx_image image, uint16_t file_format)
{
    vx_uint32 width, height;
    vx_imagepatch_addressing_t image_addr;
    vx_rectangle_t rect;
    vx_map_id map_id;
    void *data_ptr;
    vx_status status;

    status = vxGetStatus((vx_reference)image);
    if(status==VX_SUCCESS)
    {
        vxQueryImage(image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = width;
        rect.end_y = height;

        status = vxMapImagePatch(image,
            &rect,
            0,
            &map_id,
            &image_addr,
            &data_ptr,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            VX_NOGAP_X
            );

        if(status==VX_SUCCESS)
        {
            FILE *fp = fopen(filename,"rb");
            vx_int32 j;
            vx_int32 num_bytes = 0;

            if(fp!=NULL)
            {
                if(file_format==APP_FILE_FORMAT_BIN12B_UNPACKED)
                {
                    for (j = 0; j < height; j++)
                    {
                        num_bytes += fread(data_ptr, image_addr.stride_x, width, fp);
                        data_ptr += image_addr.stride_y;
                    }

                    if(num_bytes != (width*height))
                        printf("Bytes read = %d, expected = %d", num_bytes, width*height);
                }
                else if(file_format==APP_FILE_FORMAT_YUV)
                {
                    /* Copy Luma */
                    for (j = 0; j < height; j++)
                    {
                        num_bytes += fread(data_ptr, image_addr.stride_x, width, fp);
                        data_ptr += image_addr.stride_y;
                    }

                    if(num_bytes != (width*height))
                      printf("Luma bytes read = %d, expected = %d", num_bytes, width*height);
                }

                fclose(fp);
            }
            else
            {
                printf("# ERROR: Unable to open file for reading [%s]\n", filename);
                status = VX_FAILURE;
            }
            vxUnmapImagePatch(image, map_id);
        }
    }
    return status;
}

static void app_show_usage(int argc, char* argv[])
{
    printf("\n");
    printf(" Dense Optical Flow HWA Demo - (c) Texas Instruments 2018\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s [--cfg <config file>]\n", argv[0]);
    printf("\n");
}

static void app_set_cfg_default(AppObj *obj)
{
    snprintf(obj->input_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/psdkra/tidl_demo_images");
    snprintf(obj->output_file_path,APP_MAX_FILE_PATH, "/opt/vision_apps/test_data/output");
    snprintf(obj->input_file_prefix,APP_MAX_FILE_PATH, "00000");
    obj->input_file_postfix[0] = 0;
    snprintf(obj->output_file_prefix,APP_MAX_FILE_PATH, "00000");
    obj->start_fileno = 100;
    obj->end_fileno = 500;

    /* obj read */
    obj->width = 1280;
    obj->height = 720;
    obj->dof_levels = 4;
    obj->in_file_format = APP_FILE_FORMAT_YUV;
    obj->in_vx_df_image = VX_DF_IMAGE_U8;
    obj->out_file_format = APP_FILE_FORMAT_BMP;
    app_get_file_format_ext(obj->in_file_format, obj->in_file_ext);
    app_get_file_format_ext(obj->out_file_format, obj->out_file_ext);
    obj->save_intermediate_output = vx_false_e;
    obj->enable_temporal_predicton_flow_vector = 1;
    obj->displayObj.display_option = 0;
    obj->num_iterations = 10;
    obj->is_interactive = 0;
    obj->test_mode = 0;
}

static void app_parse_cfg_file(AppObj *obj, char *cfg_file_name)
{
    FILE *fp = fopen(cfg_file_name, "r");
    char line_str[1024];
    char *token;

    if(fp==NULL)
    {
        printf("# ERROR: Unable to open config file [%s]\n", cfg_file_name);
        exit(0);
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
            if(strcmp(token, "input_file_path")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->input_file_path, token);
                }
            }
            else
            if(strcmp(token, "output_file_path")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->output_file_path, token);
                }
            }
            else
            if(strcmp(token, "input_file_prefix")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->input_file_prefix, token);
                }
            }
            else
            if(strcmp(token, "input_file_postfix")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->input_file_postfix, token);
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
            if(strcmp(token, "in_file_format")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    if(strcmp(token, "bmp")==0)
                    {
                        obj->in_file_format = APP_FILE_FORMAT_BMP;
                        obj->in_vx_df_image = VX_DF_IMAGE_U8;
                    }
                    if(strcmp(token, "bin12b")==0)
                    {
                        obj->in_file_format = APP_FILE_FORMAT_BIN12B_UNPACKED;
                        obj->in_vx_df_image = VX_DF_IMAGE_U16;
                    }
                    if(strcmp(token, "yuv")==0)
                    {
                        obj->in_file_format = APP_FILE_FORMAT_YUV;
                        obj->in_vx_df_image = VX_DF_IMAGE_U8;
                    }
                    app_get_file_format_ext(obj->in_file_format, obj->in_file_ext);
                }
            }
            else
            if(strcmp(token, "out_file_format")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    token[strlen(token)-1]=0;
                    if(strcmp(token, "bmp")==0)
                    {
                        obj->out_file_format = APP_FILE_FORMAT_BMP;
                    }
                    app_get_file_format_ext(obj->out_file_format, obj->out_file_ext);
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
            if(strcmp(token, "dof_levels")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->dof_levels = atoi(token);
                }
            }
            else
            if(strcmp(token, "save_intermediate_output")==0)
            {
                obj->save_intermediate_output=vx_true_e;
            }
            else
            if(strcmp(token, "enable_temporal_predicton_flow_vector")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->enable_temporal_predicton_flow_vector = atoi(token);
                }
            }
            else
            if(strcmp(token, "display_option")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->displayObj.display_option = atoi(token);
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
            if(strcmp(token, "test_mode")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->test_mode = atoi(token);
                }
            }
        }
    }

    /* When running a test, the user should not be prompted */
    if (obj->test_mode)
    {
        obj->is_interactive = 0;
    }

    fclose(fp);

    if(obj->dof_levels<2)
    {
        obj->dof_levels = 2;
    }
    if(obj->width<128)
    {
        obj->width = 128;
    }
    if(obj->height<128)
    {
        obj->height = 128;
    }
    /* at least two files need to given as input */
    if(obj->end_fileno <= obj->start_fileno)
    {
        obj->end_fileno = obj->start_fileno+1;
    }
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
        else
        if (i > 0)
        {
            printf("ERROR: argument %s not recognized\n\n", argv[i]);
            app_show_usage(argc, argv);
            exit(0);
        }
    }

    if (set_test_mode == vx_true_e)
    {
        obj->test_mode = 1;
        obj->is_interactive = 0;
#ifndef x86_64
        obj->displayObj.display_option = 1;
#else
        obj->displayObj.display_option = 0;
#endif
        /* starting file number + 2 frames required to queue up dof
            + the number of checksums availble */
        obj->end_fileno = (obj->start_fileno) + 2 +
                sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0]);
        obj->num_iterations = 1;
    }
}

static void app_find_image_array_index(vx_image image_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx)
{
  vx_int32 i;

  *array_idx = -1;
  for(i = 0; i < array_size; i++)
  {
    if(ref == (vx_reference)image_array[i])
    {
      *array_idx = i;
      break;
    }
  }
}

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

    Draw2D_BmpPrm bmp;

    bmp.bmpIdx = DRAW2D_BMP_IDX_DOF_COLOUR_MAP;
    Draw2D_drawBmp(handle, 20, 1080-256-40, &bmp);

    sHeading.fontIdx = 5;
    Draw2D_drawString(handle, 620, 40, "Dense Optical Flow - Demo", &sHeading);

    sAlgo1.fontIdx = 1;
    Draw2D_drawString(handle, INPUT_DISPLAY_WIDTH/2 - 120 , (1080 + INPUT_DISPLAY_HEIGHT)/2 - 75, "Camera Input", &sAlgo1);

    char resolution[30];
    snprintf(resolution, 30, "%s%d%s%d%s","Resolution : ",obj->width,"px x ",obj->height,"px");
    sAlgo2.fontIdx = 3;
    Draw2D_drawString(handle, INPUT_DISPLAY_WIDTH/2 - 130 , (1080 + INPUT_DISPLAY_HEIGHT)/2 - 35, resolution , &sAlgo2);

    sAlgo3.fontIdx = 1;
    Draw2D_drawString(handle, 1920 - OUTPUT_DISPLAY_WIDTH/2 - 65 , (1080 + OUTPUT_DISPLAY_HEIGHT)/2 - 72, "DOF Output", &sAlgo3);

    sAlgo4.fontIdx = 1;
    Draw2D_drawString(handle, 10 , 1080-256-40-40 , "DOF Colour Map", &sAlgo4);

    sAlgo5.fontIdx = 3;
    Draw2D_drawString(handle, 0 , 1080-40 , "Smaller vectors are lighter.", &sAlgo5);

    sAlgo6.fontIdx = 3;
    Draw2D_drawString(handle, 0 , 1080-20 , "Colour represents the direction", &sAlgo6);
  }

  return;
}
#endif

static void update_pre_proc_params_pyramid( AppObj *obj, PyramidObj *pyramidObj)
{

    pyramidObj->width = obj->width;
    pyramidObj->height = obj->height;
    pyramidObj->dof_levels = obj->dof_levels;
    pyramidObj->in_vx_df_image = obj->in_vx_df_image;

}
static void update_pre_proc_params_dofproc( AppObj *obj, DofProcObj *dofprocObj)
{
  dofprocObj->width = obj->width;
  dofprocObj->height = obj->height;
  dofprocObj->enable_temporal_predicton_flow_vector = obj->enable_temporal_predicton_flow_vector;
}

static void update_pre_proc_params_dofviz( AppObj *obj, DofVizObj *dofvizObj)
{

    dofvizObj->width = obj->width;
    dofvizObj->height = obj->height;

}

static void update_pre_proc_params_display(AppObj *obj, DisplayObj *displayObj)
{
    displayObj->width = obj->width;
    displayObj->height = obj->height;
}

static void app_update_param_set(AppObj *obj)
{

    update_pre_proc_params_pyramid( obj, &obj->pyramidObj);
    update_pre_proc_params_dofproc( obj, &obj->dofprocObj);
    update_pre_proc_params_dofviz( obj, &obj->dofvizObj);
    update_pre_proc_params_display( obj, &obj->displayObj);

}
