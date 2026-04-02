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

#include "app_common.h"
#include "app_scaler_module.h"
#include "app_pyramid_module.h"
#include "app_dof_module.h"
#include "app_sfm_module.h"
#include "app_img_mosaic_module.h"
#include "app_display_module.h"
#include "app_test.h"
#include <TI/video_io_kernels.h>

typedef struct {
    vx_object_array input_image_arr[APP_MAX_BUFQ_DEPTH];
    /* Array of 0th index reference of input_image_arr */
    vx_image input_images[APP_MAX_BUFQ_DEPTH];

    vx_int32 width;
    vx_int32 height;

} InputObj;

typedef struct {

    InputObj     inputObj;
    ScalerObj    scalerObj;
    PyramidObj   pyramidObj;
    ScalerObj    scalerObj_prime;
    PyramidObj   pyramidObj_prime;
    DOFObj       dofObj;
    SFMObj       sfmObj;
    DisplayObj   displayObj;
    ImgMosaicObj imgMosaicObj;

    vx_char input_file_path[APP_MAX_FILE_PATH];
    vx_char input_cam_proj_file[APP_MAX_FILE_PATH];
    vx_char output_file_path[APP_MAX_FILE_PATH];

    /* OpenVX references */
    vx_context context;
    vx_graph   graph;

    vx_int32 en_out_img_write;
    vx_int32 test_mode;

    vx_int32 start_frame;
    vx_int32 num_frames;
    vx_int32 cur_frame;
    vx_int32 in_img_color_fmt;
    vx_int32 out_img_color_fmt;

    vx_uint32 delay_in_msecs;
    vx_uint32 num_iterations;
    vx_uint32 is_interactive;

    tivx_task task;
    vx_uint32 stop_task;
    vx_uint32 stop_task_done;

    app_perf_point_t total_perf;
    app_perf_point_t fileio_perf;
    app_perf_point_t draw_perf;

    int32_t pipeline;

    int32_t enqueueCnt;
    int32_t dequeueCnt;

    FILE* cam_prm_file;

} AppObj;

AppObj gAppObj;

static vx_status readInputConfig(AppObj *obj, vx_object_array config_arr);
static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[]);
static int app_init(AppObj *obj);
static void app_deinit(AppObj *obj);
static vx_status app_prime_pyramid_output(AppObj *obj);
static vx_status app_create_graph(AppObj *obj);
static vx_status app_verify_graph(AppObj *obj);
static vx_status app_run_graph(AppObj *obj);
static vx_status app_run_graph_interactive(AppObj *obj);
static void app_delete_graph(AppObj *obj);
static void app_default_param_set(AppObj *obj);
static void app_update_param_set(AppObj *obj);
static vx_status add_graph_parameter_by_node_index(vx_graph graph, vx_node node, vx_uint32 node_parameter_index);
static void app_pipeline_params_defaults(AppObj *obj);
static void app_find_config_object_array_index(vx_object_array object_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx);
static void app_find_image_object_array_index(vx_object_array object_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx);
#ifndef x86_64
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type);
#endif
#ifdef APP_ENABLE_PIPELINE_FLOW
static vx_status app_run_graph_for_one_frame_pipeline(AppObj *obj, vx_int32 frame_id);
#else
static vx_status app_run_graph_for_one_frame_sequential(AppObj *obj, vx_int32 frame_id);
#endif
static FILE* app_read_cam_params(float * dstExt, float * dstInt, int32_t currFrameNum,
                                int32_t startFrameNum, char * fileName, int32_t maxFrameNum,
                                FILE* fp_in_cam);

static void app_show_usage(vx_int32 argc, vx_char* argv[])
{
    printf("\n");
    printf(" SFM Demo (c) Texas Instruments Inc. 2021\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static const char menu[] = {
    "\n"
    "\n ========================="
    "\n SFM Demo                 "
    "\n ========================="
    "\n"
    "\n p: Print performance statistics"
    "\n"
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};

static void app_run_task(void *app_var)
{
    vx_status status = VX_SUCCESS;
    AppObj *obj = (AppObj *)app_var;

    while((!obj->stop_task) && (status == VX_SUCCESS))
    {
        status = app_run_graph(obj);
    }
    obj->stop_task_done = 1;
    return;
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

static vx_status app_run_task_delete(AppObj *obj)
{
    while(obj->stop_task_done==0)
    {
         tivxTaskWaitMsecs(100);
    }

    return tivxTaskDelete(&obj->task);
}

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
        printf("app_sfm: ERROR: Unable to create task\n");
    }
    else
    {
        appPerfStatsResetAll();
        while(!done && (status == VX_SUCCESS))
        {
            printf(menu);
            ch = getchar();
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
                    fp = appPerfStatsExportOpenFile(".", "cv_demos_app_sfm");
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
        /* we should still try to delete the task
            even if prints or others fail */
        if (status == VX_SUCCESS)
        {
          status = app_run_task_delete(obj);
        }
        else
        {
          app_run_task_delete(obj);
        }
    }
    return status;
}

static void app_set_cfg_default(AppObj *obj)
{
    snprintf(obj->input_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->input_cam_proj_file,APP_MAX_FILE_PATH, ".");
    obj->cur_frame = 0;
}

static void app_parse_cfg_file(AppObj *obj, vx_char *cfg_file_name)
{
    FILE *fp = fopen(cfg_file_name, "r");
    vx_char line_str[1024];
    vx_char *token;

    if(fp==NULL)
    {
        printf("# ERROR: Unable to open config file [%s]\n", cfg_file_name);
        exit(0);
    }

    while(fgets(line_str, sizeof(line_str), fp)!=NULL)
    {
        vx_char s[]=" \t";

        if (strchr(line_str, '#'))
        {
            continue;
        }

        /* get the first token */
        token = strtok(line_str, s);
        if(token != NULL)
        {
            if(strcmp(token, "input_file_path")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->input_file_path, token);
                }
            }
            else
            if(strcmp(token, "input_cam_proj_file")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->input_cam_proj_file, token);
                }
            }
            else
            if(strcmp(token, "output_file_path")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->output_file_path, token);
                }
            }
            else
            if(strcmp(token, "start_frame")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->start_frame = atoi(token);
                }
            }
            else
            if(strcmp(token, "num_frames")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->num_frames = atoi(token);
                }
            }
            else
            if(strcmp(token, "input_size")==0)
            {
                vx_int32 width, height;
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    width =  atoi(token);
                    obj->inputObj.width  = width;
                    obj->sfmObj.width = width;
                    token = strtok(NULL, s);
                    if(token != NULL)
                    {
                        if (token[strlen(token)-1] == '\n')
                            token[strlen(token)-1]=0;

                        height =  atoi(token);
                        obj->inputObj.height = height;
                        obj->sfmObj.height   = height;
                    }
                }
            }
            else
            if(strcmp(token, "pyramid_base_size")==0)
            {
                vx_int32 width, height;

                token = strtok(NULL, s);
                if(token != NULL)
                {
                    width =  atoi(token);
                    obj->pyramidObj.base_width = width;
                    obj->scalerObj.output[0].width   = width;

                    token = strtok(NULL, s);
                    if(token != NULL)
                    {
                        if(token[strlen(token)-1] == '\n')
                            token[strlen(token)-1]=0;

                        height =  atoi(token);
                        obj->pyramidObj.base_height = height;
                        obj->scalerObj.output[0].height  = height;
                    }
                }
            }
            else
            if(strcmp(token, "pyramid_levels")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->pyramidObj.pyramid_levels = atoi(token);
                }
            }
            else
            if(strcmp(token, "en_out_img_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->en_out_img_write = atoi(token);
                }
            }
            else
            if(strcmp(token, "display_option")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->displayObj.display_option = atoi(token);
                }
            }
            else
            if(strcmp(token, "delay_in_msecs")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->delay_in_msecs = atoi(token);
                    if(obj->delay_in_msecs > 2000)
                        obj->delay_in_msecs = 2000;
                }
            }
            else
            if(strcmp(token, "num_iterations")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->num_iterations = atoi(token);
                    if(obj->num_iterations == 0)
                        obj->num_iterations = 1;
                }
            }
            else
            if(strcmp(token, "is_interactive")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->is_interactive = atoi(token);
                    if(obj->is_interactive > 1)
                        obj->is_interactive = 1;
                }
            }
            else
            if(strcmp(token, "maxNumTracks")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->sfmObj.maxNumTracks = atoi(token);
                }
            }
            else
            if(strcmp(token, "keyPointStep")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->sfmObj.keyPointStep = atoi(token);
                }
            }
            else
            if(strcmp(token, "flowConfThr")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->sfmObj.params.flowConfThr = atoi(token);
                }
            }
        }
    }

    fclose(fp);
}

static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[])
{
    vx_int32 i;
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
        obj->test_mode = 1;
        obj->delay_in_msecs = 0;
        obj->num_iterations = 1;
        obj->is_interactive = 0;
        /* display_option must be set to 1 in order for the checksums
            to come out correctly */
        obj->displayObj.display_option = 1;
        obj->num_frames = sizeof(checksums_expected[0])/sizeof(checksums_expected[0][0]) + TEST_BUFFER + 1;
    }

    #ifdef x86_64
    obj->displayObj.display_option = 0;
    obj->is_interactive = 0;
    #endif

    return;
}

vx_status app_sfm_main(vx_int32 argc, vx_char* argv[])
{
    AppObj *obj = &gAppObj;
    vx_status status = VX_SUCCESS;

    /*Optional parameter setting*/
    app_default_param_set(obj);
    APP_PRINTF("Default param set! \n");

    /*Config parameter reading*/
    app_parse_cmd_line_args(obj, argc, argv);
    APP_PRINTF("Parsed user params! \n");

    /*Update of parameters are config file read*/
    app_update_param_set(obj);
    APP_PRINTF("Updated user params! \n");

    status = app_init(obj);
    APP_PRINTF("App Init Done! \n");

    if(status == VX_SUCCESS)
    {
        status = app_create_graph(obj);
        APP_PRINTF("App Create Graph Done! \n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_verify_graph(obj);
        APP_PRINTF("App Verify Graph Done! \n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_prime_pyramid_output(obj);
        APP_PRINTF("App Pyramid Output Prime Done! \n");
    }

    if(obj->is_interactive && (status == VX_SUCCESS))
    {
        status = app_run_graph_interactive(obj);
    }
    else
    if (status == VX_SUCCESS)
    {
        status = app_run_graph(obj);
    }

    APP_PRINTF("App Run Graph Done! \n");

    app_delete_graph(obj);
    APP_PRINTF("App Delete Graph Done! \n");

    app_deinit(obj);
    APP_PRINTF("App De-init Done! \n");
    if (obj->test_mode == 1)
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
static vx_status add_graph_parameter_by_node_index(vx_graph graph, vx_node node, vx_uint32 node_parameter_index)
{
    vx_parameter parameter = vxGetParameterByIndex(node, node_parameter_index);
    vx_status status;
    status = vxAddParameterToGraph(graph, parameter);
    if(status == VX_SUCCESS)
    {
        status = vxReleaseParameter(&parameter);
    }
    return status;
}

static int app_init(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    app_grpx_init_prms_t grpx_prms;

    /* Create OpenVx Context */
    obj->context = vxCreateContext();
    status = vxGetStatus((vx_reference)obj->context);
    if(status == VX_SUCCESS)
    {
        tivxHwaLoadKernels(obj->context);
        tivxVideoIOLoadKernels(obj->context);
        tivxImgProcLoadKernels(obj->context);
    }

    /* Create Input OpenVx object */
    vx_image input  = vxCreateImage(obj->context, obj->inputObj.width, obj->inputObj.height, VX_DF_IMAGE_U8);
    status = vxGetStatus((vx_reference)input);
    if(status == VX_SUCCESS)
    {
        vx_int32 q;
        for(q = 0; q < APP_BUFFER_Q_DEPTH; q++)
        {
            obj->inputObj.input_image_arr[q]  = vxCreateObjectArray(obj->context, (vx_reference)input, NUM_CH);
            obj->inputObj.input_images[q]  = (vx_image)vxGetObjectArrayItem((vx_object_array)obj->inputObj.input_image_arr[q], 0);
        }
        vxReleaseImage(&input);
    }

    /* Initialize modules */

    /* Configure the prime copies of scaler and pyramid before initializing */
    memcpy(&obj->scalerObj_prime, &obj->scalerObj, sizeof(obj->scalerObj_prime));
    memcpy(&obj->pyramidObj_prime, &obj->pyramidObj, sizeof(obj->pyramidObj_prime));

    if(status == VX_SUCCESS)
    {
        status = app_init_scaler(obj->context, &obj->scalerObj, "scaler_obj", NUM_CH, 1);
        APP_PRINTF("Scaler Init Done! \n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_init_scaler(obj->context, &obj->scalerObj_prime, "scaler_obj_prime", NUM_CH, 1);
        APP_PRINTF("Scaler Prime Init Done! \n");
    }

    /* Initialize modules */
    if(status == VX_SUCCESS)
    {
        status = app_init_pyramid(obj->context, &obj->pyramidObj, "pyramidObj");
        APP_PRINTF("Pyramid Init Done! \n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_init_pyramid(obj->context, &obj->pyramidObj_prime, "pyramidObj_prime");
        APP_PRINTF("Pyramid Prime Init Done! \n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_init_dof(obj->context, &obj->dofObj, "dofObj");
        APP_PRINTF("DOF Init Done! \n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_init_sfm(obj->context, &obj->sfmObj, "sfmObj");
        APP_PRINTF("SFM Init Done! \n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_init_img_mosaic(obj->context, &obj->imgMosaicObj, "sw_mosaic_obj", APP_BUFFER_Q_DEPTH);
        APP_PRINTF("Img Mosaic Init Done! \n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_init_display(obj->context, &obj->displayObj, "display_obj");
        APP_PRINTF("Display Init Done! \n");
    }
    appPerfPointSetName(&obj->total_perf , "TOTAL");
    appPerfPointSetName(&obj->fileio_perf, "FILEIO");

    #ifndef x86_64
    if((obj->displayObj.display_option == 1) && (status == VX_SUCCESS))
    {
        appGrpxInitParamsInit(&grpx_prms, obj->context);
        grpx_prms.draw_callback = app_draw_graphics;
        appGrpxInit(&grpx_prms);
    }
    #endif

    return status;
}

static void app_deinit(AppObj *obj)
{
    vx_int32 q;

    for(q = 0; q < APP_BUFFER_Q_DEPTH; q++)
    {
        vxReleaseObjectArray(&obj->inputObj.input_image_arr[q]);
        vxReleaseImage(&obj->inputObj.input_images[q]);
    }
    APP_PRINTF("Input images de-initialized!\n");

    app_deinit_scaler(&obj->scalerObj);
    APP_PRINTF("Scaler module de-initialized!\n");

    app_deinit_pyramid(&obj->pyramidObj);
    APP_PRINTF("Pyramid module de-initialized!\n");

    app_deinit_scaler(&obj->scalerObj_prime);
    APP_PRINTF("Scaler prime module de-initialized!\n");

    app_deinit_pyramid(&obj->pyramidObj_prime);
    APP_PRINTF("Pyramid prime module de-initialized!\n");

    app_deinit_dof(&obj->dofObj);
    APP_PRINTF("DOF module de-initialized!\n");

    app_deinit_sfm(&obj->sfmObj);
    APP_PRINTF("SFM module de-initialized!\n");

    app_deinit_img_mosaic(&obj->imgMosaicObj, APP_BUFFER_Q_DEPTH);
    APP_PRINTF("Mosaic module de-initialized!\n");

    app_deinit_display(&obj->displayObj);
    APP_PRINTF("Display module de-initialized!\n");

    #ifndef x86_64
    if(obj->displayObj.display_option == 1)
    {
        appGrpxDeInit();
    }
    #endif

    tivxImgProcUnLoadKernels(obj->context);
    tivxVideoIOUnLoadKernels(obj->context);
    tivxHwaUnLoadKernels(obj->context);

    vxReleaseContext(&obj->context);
}

static void app_delete_graph(AppObj *obj)
{
    app_delete_scaler(&obj->scalerObj);
    APP_PRINTF("Scaler objects deleted!\n");

    app_delete_pyramid(&obj->pyramidObj);
    APP_PRINTF("Pyramid objects deleted!\n");

    app_delete_dof(&obj->dofObj);
    APP_PRINTF("DOF objects deleted!\n");

    app_delete_sfm(&obj->sfmObj);
    APP_PRINTF("SFM objects deleted!\n");

    app_delete_img_mosaic(&obj->imgMosaicObj);
    APP_PRINTF("Mosaic objects deleted!\n");

    app_delete_display(&obj->displayObj);
    APP_PRINTF("Display objects deleted!\n");

    vxReleaseGraph(&obj->graph);
}

static vx_status app_prime_pyramid_output(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_graph graph;

    graph = vxCreateGraph(obj->context);
    status = vxGetStatus((vx_reference)graph);

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_scaler(obj->context, graph, &obj->scalerObj_prime, obj->inputObj.input_image_arr[0]);
        APP_PRINTF("Scaler Node added!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_create_graph_pyramid(graph, &obj->pyramidObj_prime, obj->scalerObj_prime.output[0].arr);
        APP_PRINTF("Pyramid Node added!\n");
    }
    /* Prime the pyramid output to ensure that -1 delay has valid reference */
    if(status == VX_SUCCESS)
    {
        vx_char input_file_name[APP_MAX_FILE_PATH];
        vx_int32 frame_id = obj->start_frame;

        snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%010d.y", obj->input_file_path, frame_id);
        readScalerInput(input_file_name, obj->inputObj.input_image_arr[0], APP_MODULES_READ_FILE, 0);

        status = vxVerifyGraph(graph);

        if(status == VX_SUCCESS)
        {
            status = vxScheduleGraph(graph);
        }
        else
        {
            printf("Stage1: Graph verify fail!\n");
        }

        if(status == VX_SUCCESS)
        {
            status = vxWaitGraph(graph);
        }
        else
        {
            printf("Stage1: Graph execution fail!\n");
        }
    }

    if(status == VX_SUCCESS)
    {
        printf("Priming Pyramid output done!\n");
    }

    app_delete_scaler(&obj->scalerObj_prime);
    APP_PRINTF("Scaler prime objects deleted!\n");

    app_delete_pyramid(&obj->pyramidObj_prime);
    APP_PRINTF("Pyramid prime objects deleted!\n");

    vxReleaseGraph(&graph);

    return status;
}

static vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_graph_parameter_queue_params_t graph_parameters_queue_params_list[3] = {0};
    vx_int32 graph_parameter_index;

    obj->graph = vxCreateGraph(obj->context);
    status = vxGetStatus((vx_reference)obj->graph);

    if(status == VX_SUCCESS)
    {
        status = vxSetReferenceName((vx_reference)obj->graph, "OpenVxGraph");
    }
    if(status == VX_SUCCESS)
    {
        status = app_create_graph_scaler(obj->context, obj->graph, &obj->scalerObj, obj->inputObj.input_image_arr[0]);
        APP_PRINTF("Scaler Node added!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_create_graph_pyramid(obj->graph, &obj->pyramidObj, obj->scalerObj.output[0].arr);
        APP_PRINTF("Pyramid Node added!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_dof(obj->graph, &obj->dofObj, obj->pyramidObj.pyramid_delay);
        APP_PRINTF("DOF Node added!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = vxRegisterAutoAging(obj->graph, obj->pyramidObj.pyramid_delay);
    }
    if(status == VX_SUCCESS)
    {
        status = app_create_graph_sfm(obj->graph, &obj->sfmObj, obj->scalerObj.output[0].arr, obj->dofObj.flow_vector_field_array);
        APP_PRINTF("SFM Node added!\n");
    }
    if(status == VX_SUCCESS)
    {
        vx_int32 idx = 0;

        obj->imgMosaicObj.input_arr[idx++] = obj->sfmObj.output_ptcld_img_arr;
        obj->imgMosaicObj.input_arr[idx++] = obj->sfmObj.output_og_img_arr;

        obj->imgMosaicObj.num_inputs = idx;

        status = app_create_graph_img_mosaic(obj->graph, &obj->imgMosaicObj, NULL);
        APP_PRINTF("Mosaic Node added!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = app_create_graph_display(obj->graph, &obj->displayObj, obj->imgMosaicObj.output_image[0]);
        APP_PRINTF("Display Node added!\n");
    }

#ifdef APP_ENABLE_PIPELINE_FLOW
    /* Scalar Node - input is in Index 0 */
    if(status == VX_SUCCESS)
    {
        graph_parameter_index = 0;
        status = add_graph_parameter_by_node_index(obj->graph, obj->scalerObj.node, 0);
        obj->scalerObj.graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->inputObj.input_images[0];
        graph_parameter_index++;
    }
    if(status == VX_SUCCESS)
    {
        status = add_graph_parameter_by_node_index(obj->graph, obj->sfmObj.node, 0);
        obj->sfmObj.graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->sfmObj.config[0];
        graph_parameter_index++;
    }
    if(((obj->en_out_img_write == 1) || (obj->test_mode == 1)) && (status == VX_SUCCESS))
    {
        status = add_graph_parameter_by_node_index(obj->graph, obj->imgMosaicObj.node, 1);
        obj->imgMosaicObj.graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->imgMosaicObj.output_image[0];
        graph_parameter_index++;
    }

    if(status == VX_SUCCESS)
    {
        status = vxSetGraphScheduleConfig(obj->graph,
                    VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                    graph_parameter_index,
                    graph_parameters_queue_params_list);
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetGraphPipelineDepth(obj->graph, APP_PIPELINE_DEPTH);
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->scalerObj.node, 1, 8);
        APP_PRINTF("Scaler output buffer depth set!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->pyramidObj.node, 1, APP_MAX_BUFQ_DEPTH);
        APP_PRINTF("Pyramid output buffer depth set!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->dofObj.node, 8, APP_MAX_BUFQ_DEPTH);
        APP_PRINTF("DOF output buffer depth set!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->sfmObj.node, TIVX_KERNEL_SFM_OUT_ARGS_IDX, APP_MAX_BUFQ_DEPTH);
        APP_PRINTF("SFM output buffer depth set!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->sfmObj.node, TIVX_KERNEL_SFM_OUTPUT_PTCLD_IMAGE_IDX, APP_MAX_BUFQ_DEPTH);
        APP_PRINTF("SFM output buffer depth set!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->sfmObj.node, TIVX_KERNEL_SFM_OUTPUT_OCPGRD_IMAGE_IDX, APP_MAX_BUFQ_DEPTH);
        APP_PRINTF("SFM output buffer depth set!\n");
    }
    if(status == VX_SUCCESS)
    {
        status = tivxSetNodeParameterNumBufByIndex(obj->sfmObj.node, TIVX_KERNEL_SFM_OUTPUT_FEATURE_IDX, APP_MAX_BUFQ_DEPTH);
        APP_PRINTF("SFM output buffer depth set!\n");
    }
    if(status == VX_SUCCESS)
    {
        if(!((obj->en_out_img_write == 1) || (obj->test_mode == 1)))
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->imgMosaicObj.node, 1, 4);
        }
        APP_PRINTF("Mosaic output buffer depth set!\n");
    }
#endif
    return status;
}

static vx_status app_verify_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    ScalerObj *scalerObj;
    vx_reference refs[1];

    status = vxVerifyGraph(obj->graph);

    APP_PRINTF("App Verify Graph Done!\n");

    #if 1
    if(status == VX_SUCCESS)
    {
        status = tivxExportGraphToDot(obj->graph,".", "vx_app_sfm");
    }
    #endif

    scalerObj = &obj->scalerObj;

    refs[0] = (vx_reference)scalerObj->coeff_obj;

    if(status == VX_SUCCESS)
    {
        status = tivxNodeSendCommand(scalerObj->node, 0u,
                                    TIVX_VPAC_MSC_CMD_SET_COEFF,
                                    refs, 1u);
        APP_PRINTF("App Send MSC Command Done!\n");
    }
    if(status != VX_SUCCESS)
    {
        printf("MSC: Node send command failed!\n");
    }

    /* wait a while for prints to flush */
    tivxTaskWaitMsecs(100);

    return status;
}
#ifndef APP_ENABLE_PIPELINE_FLOW
static vx_status app_run_graph_for_one_frame_sequential(AppObj *obj, vx_int32 frame_id)
{
    vx_status status = VX_SUCCESS;

    vx_char input_file_name[APP_MAX_FILE_PATH];

    ScalerObj *scalerObj = &obj->scalerObj;

    snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%010d.y", obj->input_file_path, frame_id);

    appPerfPointBegin(&obj->fileio_perf);

    readScalerInput(input_file_name, obj->inputObj.input_image_arr[0], APP_MODULES_READ_FILE, NUM_CH);

    readInputConfig(obj, obj->sfmObj.config_arr[0]);

    appPerfPointEnd(&obj->fileio_perf);

    APP_PRINTF("App Reading Input Done!\n");

#ifdef x86_64
    printf("Processing file %s ...", input_file_name);
#endif

    status = vxProcessGraph(obj->graph);

#ifdef x86_64
    printf("Done!\n");
#endif

    if((VX_SUCCESS == status) && (obj->en_out_img_write == 1))
    {
        vx_char output_file_name[APP_MAX_FILE_PATH];

#ifdef APP_TIDL_TRACE_DUMP
        snprintf(output_file_name, APP_MAX_FILE_PATH, "%s/%08d_", obj->output_file_path, frame_id);
        vx_user_data_object trace_data = (vx_user_data_object)vxGetObjectArrayItem(obj->tidlObj.trace_data_arr, 0);
        tivx_utils_tidl_trace_write(trace_data, output_file_name);
        vxReleaseUserDataObject(&trace_data);
#endif
        snprintf(output_file_name, APP_MAX_FILE_PATH, "%s/mosaic_output_%010d_1920x1080.yuv", obj->output_file_path, frame_id);

        printf("Writing %s... ", output_file_name);
        writeMosaicOutput(output_file_name, obj->imgMosaicObj.output_image[0]);
        printf("Done!\n");

    }

    return status;
}
#else
static vx_status app_run_graph_for_one_frame_pipeline(AppObj *obj, vx_int32 frame_id)
{
    vx_status status = VX_SUCCESS;
    vx_uint32 actual_checksum = 0;

    vx_char input_file_name[APP_MAX_FILE_PATH];
    vx_int32 obj_array_idx = -1;

    ScalerObj    *scalerObj    = &obj->scalerObj;
    ImgMosaicObj *imgMosaicObj = &obj->imgMosaicObj;

    snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%010d.y", obj->input_file_path, frame_id);

    if(obj->pipeline < 0)
    {
        /* Enqueue output */
        if(((obj->en_out_img_write == 1) || (obj->test_mode == 1)) && (status == VX_SUCCESS))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&imgMosaicObj->output_image[obj->enqueueCnt], 1);
        }

        appPerfPointBegin(&obj->fileio_perf);
        /* Read input */
        if(status == VX_SUCCESS)
        {
            status = readInputConfig(obj, obj->sfmObj.config_arr[obj->enqueueCnt]);
        }

        if(status == VX_SUCCESS)
        {
            status = readScalerInput(input_file_name, obj->inputObj.input_image_arr[obj->enqueueCnt], APP_MODULES_READ_FILE, 0);
        }

        appPerfPointEnd(&obj->fileio_perf);

        APP_PRINTF("App Reading Input Done!\n");

        /* Enqueue input - start execution */

        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->sfmObj.graph_parameter_index, (vx_reference*)&obj->sfmObj.config[obj->enqueueCnt], 1);
        }

        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, scalerObj->graph_parameter_index, (vx_reference*)&obj->inputObj.input_images[obj->enqueueCnt], 1);
        }

        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }

    if(obj->pipeline >= 0)
    {
        vx_image scaler_input_image;
        vx_user_data_object config;
        vx_image mosaic_output_image;
        uint32_t num_refs;

        /* Dequeue input */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph, obj->sfmObj.graph_parameter_index, (vx_reference*)&config, 1, &num_refs);
        }

        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterDequeueDoneRef(obj->graph, scalerObj->graph_parameter_index, (vx_reference*)&scaler_input_image, 1, &num_refs);
        }

        if((obj->en_out_img_write == 1) || (obj->test_mode == 1))
        {
            vx_char output_file_name[APP_MAX_FILE_PATH];

            /* Dequeue output */
            if(status == VX_SUCCESS)
            {
                status = vxGraphParameterDequeueDoneRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&mosaic_output_image, 1, &num_refs);
            }
            /* Check that you are within the first n frames, where n is the number
                of samples in the checksums_expected */
            if(status == VX_SUCCESS)
            {
                if (obj->test_mode == 1 && (frame_id < (obj->start_frame + obj->num_frames - TEST_BUFFER)))
                {
                    vx_uint32 expected_idx = frame_id - obj->start_frame - 1;
                    if(app_test_check_image(mosaic_output_image, checksums_expected[0][expected_idx],
                                            &actual_checksum) == vx_false_e)
                    {
                        test_result = vx_false_e;
                    }
                    /* in case test fails and needs to change */
                    populate_gatherer(0 , expected_idx, actual_checksum);
                }
            }
            if ((obj->en_out_img_write == 1) && (status == VX_SUCCESS))
            {
                APP_PRINTF("App Writing Outputs Start...\n");
                snprintf(output_file_name, APP_MAX_FILE_PATH, "%s/mosaic_output_%08d_1920x1080.yuv", obj->output_file_path, (frame_id - APP_BUFFER_Q_DEPTH));
                status = writeMosaicOutput(output_file_name, mosaic_output_image);
                APP_PRINTF("App Writing Outputs Done!\n");
            }
            /* Enqueue output */
            if(status == VX_SUCCESS)
            {
                status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&mosaic_output_image, 1);
            }
        }

        appPerfPointBegin(&obj->fileio_perf);

        if(status == VX_SUCCESS)
        {
            app_find_config_object_array_index(obj->sfmObj.config_arr, (vx_reference)config, APP_BUFFER_Q_DEPTH, &obj_array_idx);
        }
        if((obj_array_idx != -1) && (status == VX_SUCCESS))
        {
            status = readInputConfig(obj, obj->sfmObj.config_arr[obj_array_idx]);
        }

        if(status == VX_SUCCESS)
        {
            app_find_image_object_array_index(obj->inputObj.input_image_arr, (vx_reference)scaler_input_image, APP_BUFFER_Q_DEPTH, &obj_array_idx);
        }
        if((obj_array_idx != -1) && (status == VX_SUCCESS))
        {
            status = readScalerInput(input_file_name, obj->inputObj.input_image_arr[obj_array_idx], APP_MODULES_READ_FILE, 0);
        }


        appPerfPointEnd(&obj->fileio_perf);

        APP_PRINTF("App Reading Input Done!\n");

        /* Enqueue input - start execution */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, obj->sfmObj.graph_parameter_index, (vx_reference*)&config, 1);
        }

        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, scalerObj->graph_parameter_index, (vx_reference*)&scaler_input_image, 1);
        }

        obj->enqueueCnt++;
        obj->dequeueCnt++;

        obj->enqueueCnt = (obj->enqueueCnt >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->dequeueCnt = (obj->dequeueCnt >= APP_BUFFER_Q_DEPTH)? 0 : obj->dequeueCnt;

    }

    APP_PRINTF("App Process Graph Done!\n");

    return status;
}
#endif
static vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    uint64_t cur_time;

    vx_int32 frame_id = obj->start_frame;
    vx_int32 x = 0;

#ifdef APP_ENABLE_PIPELINE_FLOW
    app_pipeline_params_defaults(obj);
#endif
    for(x = 0; x < obj->num_iterations; x++)
    {
        for(frame_id = obj->start_frame; frame_id < (obj->start_frame + obj->num_frames); frame_id++)
        {
            APP_PRINTF("Running frame %d\n", frame_id);

            appPerfPointBegin(&obj->total_perf);

            cur_time = tivxPlatformGetTimeInUsecs();

            #ifdef APP_ENABLE_PIPELINE_FLOW
                status = app_run_graph_for_one_frame_pipeline(obj, frame_id);
            #else
                status = app_run_graph_for_one_frame_sequential(obj, frame_id);
            #endif

            cur_time = tivxPlatformGetTimeInUsecs() - cur_time;
            /* convert to msecs */
            cur_time = cur_time/1000;

            if(cur_time < obj->delay_in_msecs)
            {
                tivxTaskWaitMsecs(obj->delay_in_msecs - cur_time);
            }

            appPerfPointEnd(&obj->total_perf);

            APP_PRINTF("app_sfm: Frame ID %d of %d ... Done.\n", frame_id, obj->start_frame + obj->num_frames);

            /* user asked to stop processing */
            if((obj->stop_task) || (status == VX_FAILURE))
            {
                break;
            }
        }

        APP_PRINTF("app_sfm: Iteration %d of %d ... Done.\n", x, obj->num_iterations);
        appPerfPointPrintFPS(&obj->total_perf);
        appPerfPointReset(&obj->total_perf);
        if(x==0)
        {
            /* after first iteration reset performance stats */
            appPerfStatsResetAll();
        }

        /* user asked to stop processing */
        if((obj->stop_task) || (status == VX_FAILURE))
        {
          break;
        }
    }

#ifdef APP_ENABLE_PIPELINE_FLOW
    vxWaitGraph(obj->graph);
#endif

    obj->stop_task = 1;

    return status;
}

static void update_params_pyramid( AppObj *obj, PyramidObj *pyramidObj)
{
    pyramidObj->base_width  = obj->scalerObj.output[0].width;
    pyramidObj->base_height = obj->scalerObj.output[0].height;
}

static void update_params_scalar( AppObj *obj, ScalerObj *scalarObj)
{
    scalarObj->color_format  = obj->in_img_color_fmt;
}

static void update_params_dof(AppObj *obj, DOFObj *dofObj)
{
    dofObj->width  = obj->scalerObj.output[0].width;
    dofObj->height = obj->scalerObj.output[0].height;
    dofObj->enable_temporal_prediction = 0; /* Forcing it to spatial only */
}

static void update_params_sfm(AppObj *obj, SFMObj *sfmObj)
{
    sfmObj->width  = obj->scalerObj.output[0].width;
    sfmObj->height = obj->scalerObj.output[0].height;
    sfmObj->params.skip_flag = 0;
}

static void update_img_mosaic_defaults(AppObj *obj, ImgMosaicObj *imgMosaicObj)
{
    vx_int32 idx = 0;
    vx_float32 resizeFactW = ((vx_float32)(DISPLAY_WIDTH >> 1))/obj->scalerObj.output[0].width; // two side by side image display has to happen
    vx_float32 resizeFactH = ((vx_float32)DISPLAY_HEIGHT)/obj->scalerObj.output[0].height;
    vx_float32 resizeFact  = resizeFactH < resizeFactW ? resizeFactH : resizeFactW;

    if(resizeFact > 1.0f)
    {
      resizeFact = 1.0f;
    }

    imgMosaicObj->out_width    = DISPLAY_WIDTH;
    imgMosaicObj->out_height   = DISPLAY_HEIGHT;
    imgMosaicObj->num_inputs   = 2;

    tivxImgMosaicParamsSetDefaults(&imgMosaicObj->params);

    imgMosaicObj->params.windows[idx].startX  = 0;
    imgMosaicObj->params.windows[idx].startY  = (DISPLAY_HEIGHT - (obj->scalerObj.output[0].height)*resizeFact)/2;
    imgMosaicObj->params.windows[idx].width   = obj->scalerObj.output[0].width*resizeFact;
    imgMosaicObj->params.windows[idx].height  = obj->scalerObj.output[0].height*resizeFact;
    imgMosaicObj->params.windows[idx].input_select   = 0;
    imgMosaicObj->params.windows[idx].channel_select = 0;
    idx++;

    imgMosaicObj->params.windows[idx].startX  = (DISPLAY_WIDTH - obj->scalerObj.output[0].width*resizeFact);
    /*Assuming that 2*obj->scalerObj.output[0].height is lesser than  DISPLAY_HEIGHT*/
    imgMosaicObj->params.windows[idx].startY  = (DISPLAY_HEIGHT - (obj->scalerObj.output[0].height)*resizeFact)/2;
    imgMosaicObj->params.windows[idx].width   = obj->scalerObj.output[0].width*resizeFact;
    imgMosaicObj->params.windows[idx].height  = obj->scalerObj.output[0].height*resizeFact;
    imgMosaicObj->params.windows[idx].input_select   = 1;
    imgMosaicObj->params.windows[idx].channel_select = 0;
    idx++;

    imgMosaicObj->params.num_windows  = idx;

    /* Number of time to clear the output buffer before it gets reused */
    imgMosaicObj->params.clear_count  = 4;
}

static void app_update_param_set(AppObj *obj)
{
    update_params_scalar(obj, &obj->scalerObj);
    update_params_pyramid(obj, &obj->pyramidObj);
    update_params_dof(obj, &obj->dofObj);
    update_params_sfm(obj, &obj->sfmObj);
    update_img_mosaic_defaults(obj, &obj->imgMosaicObj);

    if(obj->is_interactive)
    {
        obj->num_iterations = 1000000000;
    }

    app_read_cam_params(NULL, obj->sfmObj.camIntPrm, 0,
                                0,
                                obj->input_cam_proj_file,
                                0,
                                obj->cam_prm_file);

    obj->sfmObj.camIntPrm[0]    *= obj->sfmObj.scaleFact;
    obj->sfmObj.camIntPrm[2]    *= obj->sfmObj.scaleFact;
    obj->sfmObj.camIntPrm[4]    *= obj->sfmObj.scaleFact;
    obj->sfmObj.camIntPrm[5]    *= obj->sfmObj.scaleFact;
}

static void set_display_defaults(DisplayObj *displayObj)
{
    displayObj->display_option = 0;
}

static void app_pipeline_params_defaults(AppObj *obj)
{
    obj->pipeline       = -APP_BUFFER_Q_DEPTH;
    obj->enqueueCnt     = 0;
    obj->dequeueCnt     = 0;
}

static void app_default_param_set(AppObj *obj)
{
    set_display_defaults(&obj->displayObj);

    app_pipeline_params_defaults(obj);

    obj->delay_in_msecs = 0;
    obj->num_iterations = 1;
    obj->is_interactive = 0;
    obj->test_mode      = 0;
    obj->in_img_color_fmt     = VX_DF_IMAGE_U8;
    obj->out_img_color_fmt    = VX_DF_IMAGE_NV12;
    obj->sfmObj.scaleFact = 0.5f;

}

static void app_find_image_object_array_index(vx_object_array object_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx)
{
    vx_int32 i;

    *array_idx = -1;
    for(i = 0; i < array_size; i++)
    {
        vx_image img_ref = (vx_image)vxGetObjectArrayItem((vx_object_array)object_array[i], 0);
        if(ref == (vx_reference)img_ref)
        {
            *array_idx = i;
            vxReleaseImage(&img_ref);
            break;
        }
        vxReleaseImage(&img_ref);
    }
}

static void app_find_config_object_array_index(vx_object_array object_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx)
{
    vx_int32 i;

    *array_idx = -1;
    for(i = 0; i < array_size; i++)
    {
        vx_user_data_object config = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)object_array[i], 0);
        if(ref == (vx_reference)config)
        {
            *array_idx = i;
            vxReleaseUserDataObject(&config);
            break;
        }
        vxReleaseUserDataObject(&config);
    }
}

#ifndef x86_64
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    if(update_type == 0)
    {
        Draw2D_FontPrm sHeading;

        sHeading.fontIdx = 4;
        Draw2D_drawString(handle, 400, 10, "DOF HWA Based Structure From Motion Demo", &sHeading);
        sHeading.fontIdx = 2;
        Draw2D_drawString(handle, 260, 250, "SFM Output : 3D Point Cloud", &sHeading);
        Draw2D_drawString(handle, 1264, 250, "Ego Centric Occupancy Grid", &sHeading);
    }

  return;
}
#endif

static FILE* app_read_cam_params(float * dstExt,
                                float * dstInt,
                                int32_t currFrameNum,
                                int32_t startFrameNum,
                                char * fileName,
                                int32_t maxFrameNum,
                                FILE* fp_in_cam)
{
  int32_t i;
  char    tempStr[200];
  int32_t extPrmSizeInRows = 4;
  int32_t intPramStartRow  = 14;
  int32_t extPramStartRow  = 28;
  int32_t ymlFileStartNo   = 0;
  int32_t modStartNo       = startFrameNum - ymlFileStartNo;

  /* Specific format for the file is assumed. If that format is not maintained
  * in the camera parameters file, then this code will fail.
  */

  if(currFrameNum == startFrameNum) {
    fp_in_cam   = fopen(fileName, "rb");

    if(fp_in_cam == NULL)
    {
      printf("could not open the camera parameter file \n");
      exit(0);
    }


    for(i = 0; i < intPramStartRow; i++)
    {
      fgets(tempStr,200,fp_in_cam);
    }

    i=0;
    do{
      i++;
    }while(tempStr[i] != '[');

    if(dstInt != NULL){
      sscanf(&tempStr[i+1],"%f,%f,%f",&dstInt[0],&dstInt[1],&dstInt[2]);

      fgets(tempStr,200,fp_in_cam);
      sscanf(&tempStr[0],"%f,%f,%f",&dstInt[3],&dstInt[4],&dstInt[5]);

      fgets(tempStr,200,fp_in_cam);
      sscanf(&tempStr[0],"%f,%f,%f",&dstInt[6],&dstInt[7],&dstInt[8]);
    }
    else
    {
      fgets(tempStr,200,fp_in_cam);
      fgets(tempStr,200,fp_in_cam);
    }

    for(i=0;i< (extPramStartRow - intPramStartRow - 2 + extPrmSizeInRows * modStartNo);i++) {
      fgets(tempStr,200,fp_in_cam);
    }

    i=0;

    if(currFrameNum == ymlFileStartNo)
    {
      do{
        i++;
      }while(tempStr[i] != '[');
    }
  } else {
    i = -1;
    fgets(tempStr,200,fp_in_cam);
  }

  /* First 2 bytes represents number of feature points information. After wards
  * ONE_FEATURE_INFO_SIZE many feature points information is placed in file.
  */
  if(dstExt != NULL)
  {
    sscanf(&tempStr[i+1],"%f,%f,%f,%f,",&dstExt[0],&dstExt[1],&dstExt[2], &dstExt[3]);
    fgets(tempStr,200,fp_in_cam);
    sscanf(&tempStr[0],"%f,%f,%f,%f,",&dstExt[4],&dstExt[5],&dstExt[6], &dstExt[7]);
    fgets(tempStr,200,fp_in_cam);
    sscanf(&tempStr[0],"%f,%f,%f,%f,",&dstExt[8],&dstExt[9],&dstExt[10], &dstExt[11]);
    if(extPrmSizeInRows == 4){
      fgets(tempStr,200,fp_in_cam);
      sscanf(&tempStr[0],"%f,%f,%f,%f,",&dstExt[12],&dstExt[13],&dstExt[14], &dstExt[15]);
      if(dstExt[15] != 1.0){
        printf("\n Something wrong has happened");
        exit(0);
      }
    }
  }
  else
  {
    fgets(tempStr,200,fp_in_cam);
    fgets(tempStr,200,fp_in_cam);
    fgets(tempStr,200,fp_in_cam);
  }

  if(currFrameNum == (maxFrameNum - 1))
  {
    fclose(fp_in_cam);
  }

  return (fp_in_cam);
}

static vx_status readInputConfig(AppObj *obj, vx_object_array config_arr)
{
    vx_status status;
    vx_int32 i, arr_len;

    status = vxGetStatus((vx_reference)config_arr);

    if(status == VX_SUCCESS)
    {
        vxQueryObjectArray(config_arr, VX_OBJECT_ARRAY_NUMITEMS, &arr_len, sizeof(vx_size));

        for (i = 0; i < arr_len; i++)
        {
            vx_user_data_object   in_config;
            in_config = (vx_user_data_object)vxGetObjectArrayItem(config_arr, i);

            vx_map_id map_id_config;
            tivxSFMParams* params;

            vxMapUserDataObject(in_config, 0, sizeof(tivxSFMParams), &map_id_config,
                            (void **)&params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);


            obj->cam_prm_file = app_read_cam_params(params->camera_pose,
                                                    NULL,
                                                    obj->cur_frame,
                                                    0,
                                                    obj->input_cam_proj_file,
                                                    (obj->start_frame + obj->num_frames),
                                                    obj->cam_prm_file);

            if(obj->cur_frame == 0)
            {
              params->camera_pose[15] = 111.0f; // reset marker flag
            }

            vxUnmapUserDataObject(in_config, map_id_config);

            obj->cur_frame++;

            if(obj->cur_frame == obj->num_frames)
            {
                obj->cur_frame = 0;
            }

            vxReleaseUserDataObject(&in_config);
        }
    }
    return(status);
}
