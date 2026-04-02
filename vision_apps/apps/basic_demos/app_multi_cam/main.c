/*
 *
 * Copyright (c) 2020-2025 Texas Instruments Incorporated
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

#include <utils/draw2d/include/draw2d.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include <utils/console_io/include/app_get.h>
#include <utils/grpx/include/app_grpx.h>
#include <utils/hwa/include/app_hwa_hlos.h>
#include <VX/vx_khr_pipelining.h>
#include <TI/video_io_kernels.h>

#include "app_common.h"
#include "app_sensor_module.h"
#include "app_capture_module.h"
#include "app_obj_arr_split_module.h"
#include "app_viss_module.h"
#include "app_aewb_module.h"
#include "app_ldc_module.h"
#include "app_img_mosaic_module.h"
#include "app_display_module.h"
#include "app_test.h"
#if defined(SOC_AM62A) && defined(QNX)
#include <screen/screen.h>
#endif
#if defined (LDRA_COVERAGE)
#include <kernels/coverage_files/include/ldra_remote_core_coverage_main.h>
#endif

#define CAPTURE_BUFFER_Q_DEPTH  (4)
#define APP_BUFFER_Q_DEPTH      (4)
#define APP_PIPELINE_DEPTH      (7)

//#define APP_TIVX_LOG_RT_ENABLE

typedef struct {

    SensorObj     sensorObj;
    CaptureObj    captureObj;
    ObjArrSplitObj  objArrSplitObj;
    VISSObj       vissObj;
    AEWBObj       aewbObj;
    LDCObj        ldcObj;
    VISSObj       vissObj1;
    AEWBObj       aewbObj1;
    LDCObj        ldcObj1;
    ImgMosaicObj  imgMosaicObj;
#if !(defined(SOC_AM62A) && defined(QNX))
    DisplayObj    displayObj;
#endif

    vx_char output_file_path[APP_MAX_FILE_PATH];

    /* OpenVX references */
    vx_context context;
    vx_graph   graph;

    vx_int32 en_out_img_write;
    vx_int32 test_mode;

    vx_uint32 is_interactive;

    vx_uint32 num_frames_to_run;

    vx_uint32 num_frames_to_write;
    vx_uint32 num_frames_to_skip;

    tivx_task task;
    vx_uint32 stop_task;
    vx_uint32 stop_task_done;

    app_perf_point_t total_perf;
    app_perf_point_t fileio_perf;
    app_perf_point_t draw_perf;

#if defined(SOC_AM62A) && defined(QNX)
    tivx_task screen_task;
    uint32_t stop_screen_task;
    uint32_t stop_screen_task_done;
#endif

    int32_t enable_ldc;
    int32_t enable_viss;
    int32_t enable_split_graph;
    int32_t enable_aewb;
    int32_t enable_mosaic;

    int32_t pipeline;

    int32_t enqueueCnt;
    int32_t dequeueCnt;

    int32_t write_file;

    vx_uint32 enable_configure_hwa_freq;
    vx_uint32 hwa_freq_config;
    vx_uint32 bypass_split_graph;

    vx_image gDisplayInImage;

} AppObj;

AppObj gAppObj;

static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[]);
static vx_status app_init(AppObj *obj);
static void app_deinit(AppObj *obj);
static vx_status app_create_graph(AppObj *obj);
static vx_status app_verify_graph(AppObj *obj);
static vx_status app_run_graph(AppObj *obj);
static vx_status app_run_graph_interactive(AppObj *obj);
static void app_delete_graph(AppObj *obj);
static void app_default_param_set(AppObj *obj);
static void app_update_param_set(AppObj *obj);
static void app_pipeline_params_defaults(AppObj *obj);
static void add_graph_parameter_by_node_index(vx_graph graph, vx_node node, vx_uint32 node_parameter_index);
static vx_int32 calc_grid_size(vx_uint32 ch);
static void set_img_mosaic_params(ImgMosaicObj *imgMosaicObj, vx_uint32 in_width, vx_uint32 in_height, vx_int32 numCh, ObjArrSplitObj *objArrSplitObj, int32_t enable_split_graph);
#if !(defined(SOC_AM62A) && defined(QNX))
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type);
#endif
#if defined (LDRA_COVERAGE)
int32_t imaging_vpac_coverage_start(void);
int32_t imaging_vpac_coverage_end(void);
#endif

#if defined(SOC_AM62A) && defined(QNX)
/*AM62A: QNX to use screen package for displaying frames on A53*/
screen_context_t screen_ctx = NULL;
screen_window_t screen_win = NULL;
#endif

#if defined(SOC_AM62A) && defined(QNX)
int32_t app_run_screen(AppObj *obj)
{
    int32_t err = 0;
    int usage = SCREEN_USAGE_READ | SCREEN_USAGE_WRITE;
    int screenFormat = SCREEN_FORMAT_NV12;
    screen_context_t screen_ctx = NULL;
    screen_window_t screen_win = NULL;
    vx_uint32 width, height;
    vx_df_image df;
    vx_imagepatch_addressing_t image_addr;
    vx_rectangle_t rect;
    vx_map_id map_id1, map_id2;
    void *data_ptr1 = NULL, *data_ptr2 = NULL;
    vx_uint32 num_bytes_per_4pixels;
    vx_uint32 imgaddr_width, imgaddr_height, imgaddr_stride;
    uint32_t i;

    /* connect to screen */
    err = screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT);
    if(err != 0) {
        printf("Failed to create screen context\n");
    }

    /* create a window */
    err = screen_create_window(&screen_win, screen_ctx);
    if(err != 0) {
        printf("Failed to create screen window\n");
    }

    err = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);
    if(err != 0) {
        printf("Failed to set usage property\n");
    }
    err = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &screenFormat);
    if(err != 0) {
        printf("Failed to set format prpoerty\n");
    }

    /* create screen buffers */
    int nbuffers = 2;
    err = screen_create_window_buffers(screen_win, nbuffers);
    if(err != 0) {
        printf("Failed to create window buffer\n");
    }

    while(1)
    {
        int buffer_size[2];
        err = screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, buffer_size);
        if(err != 0) {
            printf("Failed to get window buffer size\n");
        }

        screen_buffer_t screen_buf[2];
        err = screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);
        if(err != 0) {
            printf("Failed to get window buffer\n");
        }

        /* obtain pointers to the buffers */
        void *ptr1 = NULL;
        err = screen_get_buffer_property_pv(screen_buf[0], SCREEN_PROPERTY_POINTER, (void **)&ptr1);
        if(err != 0) {
            printf("Failed to get buffer pointer\n");
        }

        int buf_stride1 = 0;
        err = screen_get_buffer_property_iv(screen_buf[0], SCREEN_PROPERTY_STRIDE, &buf_stride1);
        if(err != 0) {
           printf("Failed to get buffer stride1\n");
        }

        /* copy frames from OVX buffer to screen buffer*/
        vx_image display_image = obj->gDisplayInImage;
        vxQueryImage(display_image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
        vxQueryImage(display_image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
        vxQueryImage(display_image, VX_IMAGE_FORMAT, &df, sizeof(vx_df_image));

        if(VX_DF_IMAGE_NV12 == df)
        {
            num_bytes_per_4pixels = 4;
        }
        else if(TIVX_DF_IMAGE_NV12_P12 == df)
        {
            num_bytes_per_4pixels = 6;
        }
        else
        {
            num_bytes_per_4pixels = 8;
        }

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = width;
        rect.end_y = height;

        vxMapImagePatch(display_image,
            &rect,
            0,
            &map_id1,
            &image_addr,
            &data_ptr1,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            VX_NOGAP_X
            );

        if(!data_ptr1)
        {
            printf("data_ptr1 is NULL \n");
            return -1;
        }

        imgaddr_width  = image_addr.dim_x;
        imgaddr_height = image_addr.dim_y;
        imgaddr_stride = image_addr.stride_y;

        /*  The current implementation performs memcpy of frame from TIOVX buffer
         *  to screen buffer. This is not the optimal implementation and needs to
         *  be updated going ahead for better G2G latency figures
         */
        for(i=0;i<height;i++)
        {
            memcpy(ptr1, data_ptr1, imgaddr_width*num_bytes_per_4pixels/4);
            data_ptr1 += imgaddr_stride;
            ptr1 += (buf_stride1);
        }
        vxUnmapImagePatch(display_image, map_id1);

        if(VX_DF_IMAGE_NV12 == df || TIVX_DF_IMAGE_NV12_P12 == df)
        {
            vxMapImagePatch(display_image,
                &rect,
                1,
                &map_id2,
                &image_addr,
                &data_ptr2,
                VX_WRITE_ONLY,
                VX_MEMORY_TYPE_HOST,
                VX_NOGAP_X
                );

            if(!data_ptr2)
            {
                printf("data_ptr2 is NULL \n");
                return -1;
            }

            imgaddr_width  = image_addr.dim_x;
            imgaddr_height = image_addr.dim_y;
            imgaddr_stride = image_addr.stride_y;

            for(i=0;i<imgaddr_height/2;i++)
            {
                memcpy(ptr1, data_ptr2, imgaddr_width*num_bytes_per_4pixels/4);
                data_ptr2 += imgaddr_stride;
                ptr1 += buf_stride1;
            }
            vxUnmapImagePatch(display_image, map_id2);
        }

        err = screen_post_window(screen_win, screen_buf[0], 0, NULL, 0);
        if(err != 0) {
            printf("Failed to post window\n");
        }

        if(obj->stop_screen_task == 1)
        {
            break;
        }
    }

    return err;
}

static void app_run_screen_task(void *app_var)
{
    AppObj *obj = (AppObj *)app_var;

    app_run_screen(obj);

    obj->stop_screen_task_done = 1;
}

static int32_t app_screen_task_create(AppObj *obj)
{
    tivx_task_create_params_t params;
    int32_t status;

    tivxTaskSetDefaultCreateParams(&params);
    params.task_main = app_run_screen_task;
    params.app_var = obj;

    obj->stop_screen_task_done = 0;
    obj->stop_screen_task = 0;

    status = tivxTaskCreate(&obj->screen_task, &params);

    return status;
}

static void app_run_screen_task_delete(AppObj *obj)
{
    while(obj->stop_screen_task_done==0)
    {
         tivxTaskWaitMsecs(100);
    }

    screen_destroy_window(screen_win);
    screen_destroy_context(screen_ctx);
    tivxTaskDelete(&obj->screen_task);
}
#endif

static void app_show_usage(vx_int32 argc, vx_char* argv[])
{
    printf("\n");
    printf(" Camera Demo - (c) Texas Instruments 2020\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static const char menu[] = {
    "\n"
    "\n ========================="
    "\n Demo : Camera Demo"
    "\n ========================="
    "\n"
    "\n s: Save CSIx, VISS and LDC outputs"
    "\n"
    "\n p: Print performance statistics"
    "\n"
    "\n x: Exit"
    "\n"
    "\n Enter Choice: "
};


static void app_run_task(void *app_var)
{
    AppObj *obj = (AppObj *)app_var;
    vx_status status = VX_SUCCESS;
    while((!obj->stop_task) && (status == VX_SUCCESS))
    {
        status = app_run_graph(obj);
    }
    obj->stop_task_done = 1;
}

static int32_t app_run_task_create(AppObj *obj)
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

static vx_status app_run_graph_interactive(AppObj *obj)
{
    vx_status status;
    uint32_t done = 0;

    char ch;
    FILE *fp;
    app_perf_point_t *perf_arr[1];

    status = app_run_task_create(obj);
    if(status == VX_FAILURE)
    {
        printf("app_tidl: ERROR: Unable to create task\n");
    }
    else
    {
        appPerfStatsResetAll();
        while(!done)
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

                    vx_reference refs[1];
                    refs[0] = (vx_reference)obj->captureObj.raw_image_arr[0];
                    if (status == VX_SUCCESS)
                    {
                        status = tivxNodeSendCommand(obj->captureObj.node, 0u,
                                    TIVX_CAPTURE_PRINT_STATISTICS,
                                    refs, 1u);
                    }
                    break;
                case 'e':
                    perf_arr[0] = &obj->total_perf;
                    fp = appPerfStatsExportOpenFile(".", "basic_demos_app_multi_cam");
                    if (NULL != fp)
                    {
                        appPerfStatsExportAll(fp, perf_arr, 1);
                        if (status == VX_SUCCESS)
                        {
                            status = tivx_utils_graph_perf_export(fp, obj->graph);
                        }
                        appPerfStatsExportCloseFile(fp);
                        appPerfStatsResetAll();
                    }
                    else
                    {
                        printf("fp is null\n");
                    }
                    break;
                case 's':
                    obj->write_file = 1;
                    break;
                case 'x':
                    obj->stop_task = 1;
#if defined(SOC_AM62A) && defined(QNX)
                    obj->stop_screen_task = 1;
#endif
                    done = 1;
                    break;
            }
        }
        app_run_task_delete(obj);
#if defined(SOC_AM62A) && defined(QNX)
        app_run_screen_task_delete(obj);
#endif
    }
    return status;
}

static void app_set_cfg_default(AppObj *obj)
{
    snprintf(obj->captureObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->vissObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->vissObj1.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->ldcObj.output_file_path,APP_MAX_FILE_PATH, ".");

    obj->captureObj.en_out_capture_write = 0;
    obj->vissObj.en_out_viss_write = 0;
    obj->vissObj1.en_out_viss_write = 0;
    obj->ldcObj.en_out_ldc_write = 0;
    obj->ldcObj1.en_out_ldc_write = 0;

    obj->num_frames_to_write = 0;
    obj->num_frames_to_skip = 0;

    obj->objArrSplitObj.num_outputs = 2;
    obj->objArrSplitObj.output0_num_elements = 0;
    obj->objArrSplitObj.output1_num_elements = 0;
    obj->objArrSplitObj.output2_num_elements = 0;
    obj->objArrSplitObj.output3_num_elements = 0;
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
            if(strcmp(token, "sensor_index")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->sensorObj.sensor_index = atoi(token);
                }
            }
            else
            if(strcmp(token, "num_frames_to_run")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->num_frames_to_run = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_error_detection")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->captureObj.enable_error_detection = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_configure_hwa_freq")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_configure_hwa_freq = atoi(token);
                }
            }
            else
            if(strcmp(token, "hwa_freq_config")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->hwa_freq_config = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_ldc")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->sensorObj.enable_ldc = atoi(token);
                    if(obj->sensorObj.enable_ldc > 1)
                        obj->sensorObj.enable_ldc = 1;
                }
            }
            else
            if(strcmp(token, "en_out_img_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->en_out_img_write = atoi(token);
                    if(obj->en_out_img_write > 1)
                        obj->en_out_img_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_capture_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->captureObj.en_out_capture_write = atoi(token);
                    if(obj->captureObj.en_out_capture_write > 1)
                        obj->captureObj.en_out_capture_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_viss_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->vissObj.en_out_viss_write = atoi(token);
                    if(obj->vissObj.en_out_viss_write > 1)
                        obj->vissObj.en_out_viss_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_ldc_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->ldcObj.en_out_ldc_write = atoi(token);
                    if(obj->ldcObj.en_out_ldc_write > 1)
                        obj->ldcObj.en_out_ldc_write = 1;
                }
            }
            else
            if(strcmp(token, "output_file_path")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->captureObj.output_file_path, token);
                    strcpy(obj->vissObj.output_file_path, token);
                    strcpy(obj->vissObj1.output_file_path, token);
                    strcpy(obj->ldcObj.output_file_path, token);
                    strcpy(obj->ldcObj1.output_file_path, token);
                    strcpy(obj->output_file_path, token);
                }
            }
            else
            if(strcmp(token, "display_option")==0)
            {
#if !(defined(SOC_AM62A) && defined(QNX))
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->displayObj.display_option = atoi(token);
                    if(obj->displayObj.display_option > 1)
                        obj->displayObj.display_option = 1;
                }
#endif
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
                    {
                        obj->is_interactive = 1;
                    }
                }
                obj->sensorObj.is_interactive = obj->is_interactive;
            }
            else
            if(strcmp(token, "bypass_split_graph")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->bypass_split_graph = atoi(token);
                    if(obj->bypass_split_graph > 1)
                    {
                        obj->bypass_split_graph = 1;
                    }
                }
            }
            else

            /*
                num_cameras_enabled from cfg file is ignored
                Instead channel_mask is read. num_cameras_enabled = number of 1 bits in mask
                For e.g. channel_mask = 15 (0x0F) indicates that first 4 cameras are enabled
            */
            if(strcmp(token, "channel_mask")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->sensorObj.ch_mask = atoi(token);
                }
            }
            else
            if(strcmp(token, "usecase_option")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->sensorObj.usecase_option = atoi(token);
                }
            }
            else
            if(strcmp(token, "num_frames_to_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->num_frames_to_write = atoi(token);
                }
            }
            else
            if(strcmp(token, "num_frames_to_skip")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->num_frames_to_skip = atoi(token);
                }
            }
            else
            if(strcmp(token, "test_mode")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->test_mode = atoi(token);
                    obj->captureObj.test_mode = atoi(token);
                }
            }
        }
    }

    fclose(fp);

}

static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[])
{
    vx_int32 i;
    vx_int16 num_test_cams = 0xFF, sensor_override = 0xFF;
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
            // check to see if there is another argument following --test
            if (argc > i+1)
            {
                num_test_cams = atoi(argv[i+1]);
                // increment i again to avoid this arg
                i++;
            }
        }
        else
        if(strcmp(argv[i], "--sensor")==0)
        {
            // check to see if there is another argument following --sensor
            if (argc > i+1)
            {
                sensor_override = atoi(argv[i+1]);
                // increment i again to avoid this arg
                i++;
            }
        }
    }

    if (set_test_mode == vx_true_e)
    {
        obj->test_mode = 1;
        obj->captureObj.test_mode = 1;
        obj->is_interactive = 0;
        obj->bypass_split_graph = 0;
        obj->enable_configure_hwa_freq = 0;
        obj->hwa_freq_config = 0;
        obj->sensorObj.is_interactive = 0;
        // set the number of test cams from cmd line
        if (num_test_cams != 0xFF)
        {
            obj->sensorObj.num_cameras_enabled = num_test_cams;
        }
        if (sensor_override != 0xFF)
        {
            obj->sensorObj.sensor_index = sensor_override;
        }
    }

    return;
}

vx_int32 app_multi_cam_main(vx_int32 argc, vx_char* argv[])
{
    vx_status status = VX_SUCCESS;
#if defined (LDRA_COVERAGE)
    imaging_vpac_coverage_start();
#endif

    AppObj *obj = &gAppObj;

    /*Optional parameter setting*/
    app_default_param_set(obj);

    /*Config parameter reading*/
    app_parse_cmd_line_args(obj, argc, argv);

    /* Querry sensor parameters */
    status = app_querry_sensor(&obj->sensorObj);
    if(1 == obj->sensorObj.sensor_out_format)
    {
        printf("YUV Input selected. VISS, AEWB and Mosaic nodes will be bypassed. \n");
        obj->enable_viss = 0;
        obj->enable_split_graph = 0;
        obj->enable_aewb = 0;
        obj->enable_mosaic = 0;
    }
    else
    {
        obj->enable_viss = 1;
        obj->enable_split_graph = 1;
        obj->enable_aewb = 1;
        obj->enable_mosaic = 1;
    }
#if defined(SOC_AM62A) && defined(QNX)
        obj->stop_screen_task = 0;
        obj->stop_screen_task_done = 0;
#endif
    /*Update of parameters are config file read*/
    app_update_param_set(obj);

    if (status == VX_SUCCESS)
    {
        status = app_init(obj);
    }
    if(status == VX_SUCCESS)
    {
        APP_PRINTF("App Init Done! \n");

        status = app_create_graph(obj);

        if(status == VX_SUCCESS)
        {
            APP_PRINTF("App Create Graph Done! \n");

            status = app_verify_graph(obj);

            if(status == VX_SUCCESS)
            {
                APP_PRINTF("App Verify Graph Done! \n");

                if (status == VX_SUCCESS)
                {
                    APP_PRINTF("App Send Error Frame Done! \n");
                    if(obj->is_interactive)
                    {
                        status = app_run_graph_interactive(obj);
                    }
                    else
                    {
                        status = app_run_graph(obj);
                    }
                }
            }
        }

        APP_PRINTF("App Run Graph Done! \n");
    }

    app_delete_graph(obj);

    APP_PRINTF("App Delete Graph Done! \n");

    app_deinit(obj);

    APP_PRINTF("App De-init Done! \n");

    if(obj->test_mode == 1)
    {
        if((vx_false_e == test_result) || (status != VX_SUCCESS))
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
#if defined (LDRA_COVERAGE)
    imaging_vpac_coverage_end();
#endif
    return status;
}

static vx_status app_init(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
#if !(defined(SOC_AM62A) && defined(QNX))
    app_grpx_init_prms_t grpx_prms;
#endif

    if (1U == obj->enable_configure_hwa_freq)
    {
        APP_PRINTF("Configuring VPAC frequency!\n");
        if (0U == obj->hwa_freq_config)
        {
            APP_PRINTF("Configuring VPAC frequency to 650 MHz and DMPAC to 520 MHz!\n");
            status = appVhwaConfigureFreq(APP_HWA_CONFIGURE_FREQ_VPAC_650_DMPAC_520);
            APP_PRINTF("Configuring VPAC frequency done!\n");
        }
        else if (1U == obj->hwa_freq_config)
        {
            APP_PRINTF("Configuring VPAC frequency to 720 MHz and DMPAC to 480 MHz!\n");
            status = appVhwaConfigureFreq(APP_HWA_CONFIGURE_FREQ_VPAC_720_DMPAC_480);
            APP_PRINTF("Configuring VPAC frequency done!\n");
        }
        else
        {
            APP_PRINTF("Invalid option for VPAC frequency configuration!\n");
        }
    }

    if (status == VX_SUCCESS)
    {
        /* Create OpenVx Context */
        obj->context = vxCreateContext();
        status = vxGetStatus((vx_reference)obj->context);
        APP_PRINTF("Creating context done!\n");
    }

    if (status == VX_SUCCESS)
    {
        tivxHwaLoadKernels(obj->context);
        tivxExtLoadKernels(obj->context);
        tivxVideoIOLoadKernels(obj->context);
        tivxImagingLoadKernels(obj->context);
        tivxFileIOLoadKernels(obj->context);
        APP_PRINTF("Kernel loading done!\n");
    }

    /* Initialize modules */
    if (status == VX_SUCCESS)
    {
        app_init_sensor(&obj->sensorObj, "sensor_obj");
    }

    if (status == VX_SUCCESS)
    {
        APP_PRINTF("Sensor init done!\n");
        status = app_init_capture(obj->context, &obj->captureObj, &obj->sensorObj, "capture_obj", CAPTURE_BUFFER_Q_DEPTH);
    }

    if( (1 == obj->enable_split_graph) && (status == VX_SUCCESS) )
    {
        obj->objArrSplitObj.input_arr = obj->captureObj.raw_image_arr[0];
        APP_PRINTF("Obj arr splitter init done!\n");
        status = app_init_obj_arr_split(obj->context, &obj->objArrSplitObj, "objArrSplit_obj");
    }

    if((1 == obj->enable_viss) && (status == VX_SUCCESS))
    {
        status = app_init_viss(obj->context, &obj->vissObj, &obj->sensorObj, "viss_obj", obj->objArrSplitObj.output0_num_elements);
        APP_PRINTF("VISS init done!\n");
    }

    if((1 == obj->enable_aewb) && (status == VX_SUCCESS))
    {
        status = app_init_aewb(obj->context, &obj->aewbObj, &obj->sensorObj, "aewb_obj", 0, obj->objArrSplitObj.output0_num_elements);
        APP_PRINTF("AEWB init done!\n");
    }

    if((obj->sensorObj.enable_ldc == 1) && (status == VX_SUCCESS))
    {
        status = app_init_ldc(obj->context, &obj->ldcObj, &obj->sensorObj, "ldc_obj", obj->objArrSplitObj.output0_num_elements);
        APP_PRINTF("LDC init done!\n");
    }

    if((1 == obj->enable_split_graph) && (status == VX_SUCCESS))
    {
        status = app_init_viss(obj->context, &obj->vissObj1, &obj->sensorObj, "viss_obj1", obj->objArrSplitObj.output1_num_elements);
        APP_PRINTF("VISS init done!\n");
        if((1 == obj->enable_aewb) && (status == VX_SUCCESS))
        {
            uint32_t strt_ch = 0;
            uint32_t prev_ch = 0;
            uint32_t ch_mask = obj->sensorObj.ch_mask;

            while((ch_mask > 0) && (prev_ch < obj->objArrSplitObj.output0_num_elements))
            {
                if(ch_mask & 0x1)
                {
                    prev_ch++;
                }
                strt_ch++;
                ch_mask = ch_mask >> 1;
            }
            status = app_init_aewb(obj->context, &obj->aewbObj1, &obj->sensorObj, "aewb_obj", strt_ch, obj->objArrSplitObj.output1_num_elements);
            APP_PRINTF("AEWB init done!\n");
        }
        if((obj->sensorObj.enable_ldc == 1) && (status == VX_SUCCESS))
        {
            status = app_init_ldc(obj->context, &obj->ldcObj1, &obj->sensorObj, "ldc_obj",obj->objArrSplitObj.output1_num_elements);
            APP_PRINTF("LDC init done!\n");
        }
    }

    if((obj->enable_mosaic == 1) && (status == VX_SUCCESS))
    {
        status = app_init_img_mosaic(obj->context, &obj->imgMosaicObj, "img_mosaic_obj", APP_BUFFER_Q_DEPTH);
        APP_PRINTF("Img Mosaic init done!\n");
    }

#if !(defined(SOC_AM62A) && defined(QNX))
    if (status == VX_SUCCESS)
    {
        status = app_init_display(obj->context, &obj->displayObj, "display_obj");
        APP_PRINTF("Display init done!\n");
    }

    appGrpxInitParamsInit(&grpx_prms, obj->context);
    grpx_prms.draw_callback = app_draw_graphics;
    appGrpxInit(&grpx_prms);
#endif

    appPerfPointSetName(&obj->total_perf , "TOTAL");
    appPerfPointSetName(&obj->fileio_perf, "FILEIO");
    return status;
}

static void app_deinit(AppObj *obj)
{
    app_deinit_sensor(&obj->sensorObj);
    APP_PRINTF("Sensor deinit done!\n");

    app_deinit_capture(&obj->captureObj, CAPTURE_BUFFER_Q_DEPTH);
    APP_PRINTF("Capture deinit done!\n");

    if(1 == obj->enable_split_graph)
    {
        obj->objArrSplitObj.input_arr = obj->captureObj.raw_image_arr[0];
        app_deinit_obj_arr_split(&obj->objArrSplitObj);
        APP_PRINTF("Object array splitter deinit done!\n");
    }

    if(1 == obj->enable_viss)
    {
        app_deinit_viss(&obj->vissObj);
        APP_PRINTF("VISS deinit done!\n");
    }

    if(1 == obj->enable_aewb)
    {
        app_deinit_aewb(&obj->aewbObj);
        APP_PRINTF("AEWB deinit done!\n");
    }

    if(obj->sensorObj.enable_ldc == 1)
    {
        app_deinit_ldc(&obj->ldcObj);
        APP_PRINTF("LDC deinit done!\n");
    }

    if(1 == obj->enable_split_graph)
    {
        app_deinit_viss(&obj->vissObj1);
        APP_PRINTF("VISS deinit done!\n");
        if(1 == obj->enable_aewb)
        {
            app_deinit_aewb(&obj->aewbObj1);
            APP_PRINTF("AEWB deinit done!\n");
        }
        if(obj->sensorObj.enable_ldc == 1)
        {
            app_deinit_ldc(&obj->ldcObj1);
            APP_PRINTF("LDC deinit done!\n");
        }
    }

    if(obj->enable_mosaic == 1)
    {
        app_deinit_img_mosaic(&obj->imgMosaicObj, APP_BUFFER_Q_DEPTH);
        APP_PRINTF("Img Mosaic deinit done!\n");
    }

#if !(defined(SOC_AM62A) && defined(QNX))
    app_deinit_display(&obj->displayObj);
    APP_PRINTF("Display deinit done!\n");

    appGrpxDeInit();
#endif

    tivxHwaUnLoadKernels(obj->context);
    tivxExtUnLoadKernels(obj->context);
    tivxVideoIOUnLoadKernels(obj->context);
    tivxImagingUnLoadKernels(obj->context);
    tivxFileIOUnLoadKernels(obj->context);
    APP_PRINTF("Kernels unload done!\n");

    vxReleaseContext(&obj->context);
    APP_PRINTF("Release context done!\n");
}

static void app_delete_graph(AppObj *obj)
{

    #ifdef APP_TIVX_LOG_RT_ENABLE
    tivxLogRtTraceExportToFile("app_multicam.bin");
    tivxLogRtTraceDisable(obj->graph);
    #endif

    app_delete_capture(&obj->captureObj);
    APP_PRINTF("Capture delete done!\n");

    if(1 == obj->enable_split_graph)
    {
        app_delete_obj_arr_split(&obj->objArrSplitObj);
        APP_PRINTF("Object array splitter delete done!\n");
    }

    app_delete_viss(&obj->vissObj);
    APP_PRINTF("VISS delete done!\n");

    app_delete_aewb(&obj->aewbObj);
    APP_PRINTF("AEWB delete done!\n");

    if(1 == obj->enable_split_graph)
    {
        app_delete_viss(&obj->vissObj1);
        APP_PRINTF("VISS delete done!\n");

        app_delete_aewb(&obj->aewbObj1);
        APP_PRINTF("AEWB delete done!\n");
    }

    if(obj->sensorObj.enable_ldc == 1)
    {
        app_delete_ldc(&obj->ldcObj);
        APP_PRINTF("LDC delete done!\n");
        if(1 == obj->enable_split_graph)
        {
            app_delete_ldc(&obj->ldcObj1);
            APP_PRINTF("LDC delete done!\n");
        }
    }

    app_delete_img_mosaic(&obj->imgMosaicObj);
    APP_PRINTF("Img Mosaic delete done!\n");

#if !(defined(SOC_AM62A) && defined(QNX))
    app_delete_display(&obj->displayObj);
    APP_PRINTF("Display delete done!\n");
#endif

    vxReleaseGraph(&obj->graph);
    APP_PRINTF("Graph delete done!\n");
}

/* VISS module create helper function for IR frames */
vx_status app_create_graph_viss_ir(vx_graph graph, VISSObj *vissObj, vx_object_array raw_image_arr, const char *target)
{
    vx_status status = VX_SUCCESS;

    tivx_raw_image raw_image = (tivx_raw_image)vxGetObjectArrayItem(raw_image_arr, 0);
    vx_user_data_object h3a_stats = (vx_user_data_object)vxGetObjectArrayItem(vissObj->h3a_stats_arr, 0);
    vx_image output_img = (vx_image)vxGetObjectArrayItem(vissObj->output_arr, 0);

    vissObj->node = tivxVpacVissNode(
                                graph,
                                vissObj->config,
                                NULL,
                                vissObj->dcc_config,
                                raw_image, output_img, NULL,
                                NULL, NULL, NULL,
                                h3a_stats, NULL, NULL, NULL);

    tivxReleaseRawImage(&raw_image);
    vxReleaseImage(&output_img);
    vxReleaseUserDataObject(&h3a_stats);

    status = vxGetStatus((vx_reference)vissObj->node);
    if(status == VX_SUCCESS)
    {
        vxSetReferenceName((vx_reference)vissObj->node, "viss_node");
        vxSetNodeTarget(vissObj->node, VX_TARGET_STRING, target);

        vx_bool replicate[] = { vx_false_e, vx_false_e, vx_false_e, vx_true_e, vx_true_e, vx_true_e, vx_false_e, vx_false_e, vx_false_e, vx_true_e, vx_false_e, vx_false_e, vx_false_e};
        vxReplicateNode(graph, vissObj->node, replicate, 13);

        if(vissObj->en_out_viss_write == 1)
        {
            status = app_create_graph_viss_write_output(graph, vissObj);
        }
    }
    else
    {
        printf("[VISS-IR-MODULE] Unable to create VISS Node! \n");
    }

    return status;
}

static vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_graph_parameter_queue_params_t graph_parameters_queue_params_list[2] = {0};
    vx_int32 graph_parameter_index;

    obj->graph = vxCreateGraph(obj->context);
    status = vxGetStatus((vx_reference)obj->graph);
    if (status == VX_SUCCESS)
    {
        status = vxSetReferenceName((vx_reference)obj->graph, "app_multi_cam_graph");
        APP_PRINTF("Graph create done!\n");
    }

    if(status == VX_SUCCESS)
    {
        status = app_create_graph_capture(obj->graph, &obj->captureObj);
        APP_PRINTF("Capture graph done!\n");
    }

    if( (1 == obj->enable_split_graph) && (status == VX_SUCCESS) )
    {
        status = app_create_graph_obj_arr_split(obj->graph, &obj->objArrSplitObj);
        APP_PRINTF("Object array splitter graph done!\n");
    }

    if(1 == obj->enable_viss)
    {
        if(status == VX_SUCCESS)
        {
            if(1 == obj->enable_split_graph)
            {
                status = app_create_graph_viss(obj->graph, &obj->vissObj, obj->objArrSplitObj.output0_arr, TIVX_TARGET_VPAC_VISS1);
            }
            else
            {
                status = app_create_graph_viss(obj->graph, &obj->vissObj, obj->captureObj.raw_image_arr[0], TIVX_TARGET_VPAC_VISS1);
            }
            APP_PRINTF("VISS graph done!\n");
        }
    }

    if(1 == obj->enable_aewb)
    {
        if(status == VX_SUCCESS)
        {
            status = app_create_graph_aewb(obj->graph, &obj->aewbObj, obj->vissObj.h3a_stats_arr);

            APP_PRINTF("AEWB graph done!\n");
        }
    }

    vx_int32 idx = 0;
    if(obj->sensorObj.enable_ldc == 1)
    {
        vx_object_array ldc_in_arr;
        if(1 == obj->enable_viss)
        {
            ldc_in_arr = obj->vissObj.output_arr;
        }
        else
        {
            ldc_in_arr = obj->objArrSplitObj.output0_arr;
        }
        if (status == VX_SUCCESS)
        {
            status = app_create_graph_ldc(obj->graph, &obj->ldcObj, ldc_in_arr, TIVX_TARGET_VPAC_LDC1);
            APP_PRINTF("LDC graph done!\n");
        }
        obj->imgMosaicObj.input_arr[idx++] = obj->ldcObj.output_arr;
        APP_PRINTF("IDX = %i!\n",idx);
    }
    else
    {
        vx_object_array mosaic_in_arr;
        if(1 == obj->enable_viss)
        {
            mosaic_in_arr = obj->vissObj.output_arr;
        }
        else
        {
            mosaic_in_arr = obj->objArrSplitObj.output0_arr;
        }

        obj->imgMosaicObj.input_arr[idx++] = mosaic_in_arr;
    }

    if(1 == obj->enable_split_graph)
    {
        if(status == VX_SUCCESS)
        {
            #if defined(SOC_J784S4) || defined(SOC_J742S2)
            status = app_create_graph_viss(obj->graph, &obj->vissObj1, obj->objArrSplitObj.output1_arr, TIVX_TARGET_VPAC2_VISS1);
            #elif defined(SOC_AM62A) && defined(QNX)
            if (strcmp(obj->sensorObj.sensor_name,"OV2312-UB953_LI")==0) {
                /* Second instance of VISS to be configured for IR */
                status = app_create_graph_viss_ir(obj->graph, &obj->vissObj1, obj->objArrSplitObj.output1_arr, TIVX_TARGET_VPAC_VISS1);
            }
            else {
                status = app_create_graph_viss(obj->graph, &obj->vissObj1, obj->objArrSplitObj.output1_arr, TIVX_TARGET_VPAC_VISS1);
            }
            #else
            status = app_create_graph_viss(obj->graph, &obj->vissObj1, obj->objArrSplitObj.output1_arr, TIVX_TARGET_VPAC_VISS1);
            #endif
            vxSetReferenceName((vx_reference)obj->vissObj1.node, "viss_node2");
            APP_PRINTF("VISS graph done!\n");
        }
        if(1 == obj->enable_aewb)
        {
            if(status == VX_SUCCESS)
            {
                status = app_create_graph_aewb(obj->graph, &obj->aewbObj1, obj->vissObj1.h3a_stats_arr);
                vxSetReferenceName((vx_reference)obj->aewbObj1.node, "aewb_node2");
                APP_PRINTF("AEWB graph done!\n");
            }
        }
        if(obj->sensorObj.enable_ldc == 1)
        {
            vx_object_array ldc_in_arr;
            if(1 == obj->enable_split_graph)
            {
                ldc_in_arr = obj->vissObj1.output_arr;
            }
            else
            {
                ldc_in_arr = obj->objArrSplitObj.output1_arr;
            }
            if (status == VX_SUCCESS)
            {
                #if defined(SOC_J784S4) || defined(SOC_J742S2)
                status = app_create_graph_ldc(obj->graph, &obj->ldcObj1, ldc_in_arr, TIVX_TARGET_VPAC2_LDC1);
                #else
                status = app_create_graph_ldc(obj->graph, &obj->ldcObj1, ldc_in_arr, TIVX_TARGET_VPAC_LDC1);
                #endif
                vxSetReferenceName((vx_reference)obj->ldcObj1.node, "ldc_node2");
                APP_PRINTF("LDC graph done!\n");
            }
            obj->imgMosaicObj.input_arr[idx++] = obj->ldcObj1.output_arr;
            APP_PRINTF("IDX = %i!\n",idx);
        }
        else
        {
            vx_object_array mosaic_in_arr;
            if(1 == obj->enable_split_graph)
            {
                mosaic_in_arr = obj->vissObj1.output_arr;
            }
            else
            {
                mosaic_in_arr = obj->objArrSplitObj.output1_arr;
            }

            obj->imgMosaicObj.input_arr[idx++] = mosaic_in_arr;
        }
    }

    if(obj->enable_mosaic == 1)
    {
        obj->imgMosaicObj.num_inputs = idx;

        if(status == VX_SUCCESS)
        {
            status = app_create_graph_img_mosaic(obj->graph, &obj->imgMosaicObj, NULL);
            APP_PRINTF("Img Mosaic graph done!\n");
        }
        obj->gDisplayInImage = obj->imgMosaicObj.output_image[0];
    }
    else
    {
        obj->gDisplayInImage = (vx_image)vxGetObjectArrayItem(obj->captureObj.raw_image_arr[0], 0);
    }

#if !(defined(SOC_AM62A) && defined(QNX))
    if(status == VX_SUCCESS)
    {
        status = app_create_graph_display(obj->graph, &obj->displayObj, obj->gDisplayInImage);
        APP_PRINTF("Display graph done!\n");
    }
#endif

    if(status == VX_SUCCESS)
    {
        graph_parameter_index = 0;
        add_graph_parameter_by_node_index(obj->graph, obj->captureObj.node, 1);
        obj->captureObj.graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = CAPTURE_BUFFER_Q_DEPTH;
        graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->captureObj.raw_image_arr[0];
        graph_parameter_index++;

        if((obj->en_out_img_write == 1) || (obj->test_mode == 1))
        {
            add_graph_parameter_by_node_index(obj->graph, obj->imgMosaicObj.node, 1);
            obj->imgMosaicObj.graph_parameter_index = graph_parameter_index;
            graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
            graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = CAPTURE_BUFFER_Q_DEPTH;
            graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->imgMosaicObj.output_image[0];
            graph_parameter_index++;
        }

        status = vxSetGraphScheduleConfig(obj->graph,
                VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                graph_parameter_index,
                graph_parameters_queue_params_list);

        if (status == VX_SUCCESS)
        {
            status = tivxSetGraphPipelineDepth(obj->graph, APP_PIPELINE_DEPTH);
        }
        if((obj->enable_viss == 1) && (status == VX_SUCCESS))
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->vissObj.node, 6, APP_BUFFER_Q_DEPTH);

            if (status == VX_SUCCESS)
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->vissObj.node, 9, APP_BUFFER_Q_DEPTH);
            }
        }
        if((obj->enable_aewb == 1) && (status == VX_SUCCESS))
        {
            if (status == VX_SUCCESS)
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->aewbObj.node, 4, APP_BUFFER_Q_DEPTH);
            }
        }
        if((obj->sensorObj.enable_ldc == 1) && (status == VX_SUCCESS))
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->ldcObj.node, 7, APP_BUFFER_Q_DEPTH);
        }
        if((obj->enable_split_graph == 1) && (status == VX_SUCCESS))
        {
            status = tivxSetNodeParameterNumBufByIndex(obj->vissObj1.node, 6, APP_BUFFER_Q_DEPTH);

            if (status == VX_SUCCESS)
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->vissObj1.node, 9, APP_BUFFER_Q_DEPTH);
            }
            if((obj->enable_aewb == 1) && (status == VX_SUCCESS))
            {
                if (status == VX_SUCCESS)
                {
                    status = tivxSetNodeParameterNumBufByIndex(obj->aewbObj1.node, 4, APP_BUFFER_Q_DEPTH);
                }
            }
            if((obj->sensorObj.enable_ldc == 1) && (status == VX_SUCCESS))
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->ldcObj1.node, 7, APP_BUFFER_Q_DEPTH);
            }
        }

        if((obj->enable_mosaic == 1) && (status == VX_SUCCESS))
        {
            if(!((obj->en_out_img_write == 1) || (obj->test_mode == 1)))
            {
                status = tivxSetNodeParameterNumBufByIndex(obj->imgMosaicObj.node, 1, CAPTURE_BUFFER_Q_DEPTH);
                APP_PRINTF("Pipeline params setup done!\n");
            }
        }
    }

    return status;
}

static vx_status app_verify_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    status = vxVerifyGraph(obj->graph);

    if(status == VX_SUCCESS)
    {
        APP_PRINTF("Graph verify done!\n");
    }

    #if 1
    if(VX_SUCCESS == status)
    {
      status = tivxExportGraphToDot(obj->graph,".", "vx_app_multi_cam_ahp");
    }
    #endif

    if (((obj->captureObj.enable_error_detection) || (obj->test_mode)) && (status == VX_SUCCESS))
    {
        status = app_send_error_frame(&obj->captureObj);
    }

    #ifdef APP_TIVX_LOG_RT_ENABLE
    tivxLogRtTraceEnable(obj->graph);
    #endif

    /* wait a while for prints to flush */
    tivxTaskWaitMsecs(100);

    return status;
}

static vx_status app_run_graph_for_one_frame_pipeline(AppObj *obj, vx_int32 frame_id)
{
    vx_status status = VX_SUCCESS;

    APP_PRINTF("app_run_graph_for_one_pipeline: frame %d beginning\n", frame_id);
    appPerfPointBegin(&obj->total_perf);

    ImgMosaicObj *imgMosaicObj = &obj->imgMosaicObj;
    CaptureObj *captureObj = &obj->captureObj;

    /* checksum_actual is the checksum determined by the realtime test
        checksum_expected is the checksum that is expected to be the pipeline output */
    uint32_t checksum_actual = 0;

    /* This is the number of frames required for the pipeline AWB and AE algorithms to stabilize
        (note that 15 is only required for the 6-8 camera use cases - others converge quicker) */
    uint8_t stability_frame = 15;

    if(obj->pipeline < 0)
    {
        /* Enqueue outpus */
        if ((obj->en_out_img_write == 1) || (obj->test_mode == 1))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&imgMosaicObj->output_image[obj->enqueueCnt], 1);
        }

        /* Enqueue inputs during pipeup dont execute */
        if (status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&obj->captureObj.raw_image_arr[obj->enqueueCnt], 1);
        }
        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= CAPTURE_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }

    if((obj->pipeline == 0) && (status == VX_SUCCESS))
    {
        if((obj->en_out_img_write == 1) || (obj->test_mode == 1))
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&imgMosaicObj->output_image[obj->enqueueCnt], 1);
        }

        /* Execute 1st frame */
        if(status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&obj->captureObj.raw_image_arr[obj->enqueueCnt], 1);
        }
        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= CAPTURE_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }

    if((obj->pipeline > 0) && (status == VX_SUCCESS))
    {
        vx_object_array capture_input_arr;
        vx_image mosaic_output_image;
        uint32_t num_refs;

        /* Dequeue input */
        status = vxGraphParameterDequeueDoneRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&capture_input_arr, 1, &num_refs);
        if((obj->en_out_img_write == 1) || (obj->test_mode == 1))
        {
            vx_char output_file_name[APP_MAX_FILE_PATH];

            /* Dequeue output */
            if (status == VX_SUCCESS)
            {
                status = vxGraphParameterDequeueDoneRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&mosaic_output_image, 1, &num_refs);
            }
            if ((status == VX_SUCCESS) && (obj->test_mode == 1) && (frame_id > TEST_BUFFER))
            {
                /* calculate the checksum of the mosaic output */

                if ((app_test_check_image(mosaic_output_image, checksums_expected[obj->sensorObj.sensor_index][obj->sensorObj.num_cameras_enabled-1],
                                        &checksum_actual) != vx_true_e) && (frame_id > stability_frame))
                {
                    test_result = vx_false_e;
                    /* in case test fails and needs to change */
                    populate_gatherer(obj->sensorObj.sensor_index, obj->sensorObj.num_cameras_enabled-1, checksum_actual);
                }
            }

            if (obj->en_out_img_write == 1) {
                appPerfPointBegin(&obj->fileio_perf);
                snprintf(output_file_name, APP_MAX_FILE_PATH, "%s/mosaic_output_%010d_%dx%d.yuv", obj->output_file_path, (frame_id - CAPTURE_BUFFER_Q_DEPTH), imgMosaicObj->out_width, imgMosaicObj->out_height);
                if (status == VX_SUCCESS)
                {
                    status = writeMosaicOutput(output_file_name, mosaic_output_image);
                }
                appPerfPointEnd(&obj->fileio_perf);
            }
            /* Enqueue output */
            if (status == VX_SUCCESS)
            {
                status = vxGraphParameterEnqueueReadyRef(obj->graph, imgMosaicObj->graph_parameter_index, (vx_reference*)&mosaic_output_image, 1);
            }
        }

        /* Enqueue input - start execution */
        if (status == VX_SUCCESS)
        {
            status = vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&capture_input_arr, 1);
        }
        obj->enqueueCnt++;
        obj->dequeueCnt++;

        obj->enqueueCnt = (obj->enqueueCnt >= CAPTURE_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->dequeueCnt = (obj->dequeueCnt >= CAPTURE_BUFFER_Q_DEPTH)? 0 : obj->dequeueCnt;
    }

    appPerfPointEnd(&obj->total_perf);
    return status;
}

static vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    SensorObj *sensorObj = &obj->sensorObj;
    vx_int32 frame_id;
    int32_t ch_mask = obj->sensorObj.ch_mask;

    app_pipeline_params_defaults(obj);
    APP_PRINTF("app_pipeline_params_defaults returned\n");

    if('\0' == sensorObj->sensor_name[0])
    {
        printf("sensor name is NULL \n");
        return VX_FAILURE;
    }

    // if test_mode is enabled, don't fail the program if the sensor init fails
    if( (obj->test_mode) || (obj->captureObj.enable_error_detection) )
    {
        appStartImageSensor(sensorObj->sensor_name, ch_mask);
    }
    else
    {
        status = appStartImageSensor(sensorObj->sensor_name, ch_mask);
        APP_PRINTF("appStartImageSensor returned with status: %d\n", status);
    }

#if defined(SOC_AM62A) && defined(QNX)
    status = app_screen_task_create(obj);
    if(status!=0)
    {
        printf("ERROR: Unable to create screen task\n");
    }
#endif

    if(0 == obj->enable_viss)
    {
        obj->vissObj.en_out_viss_write = 0;
    }
    if(0 == obj->enable_split_graph)
    {
        obj->vissObj1.en_out_viss_write = 0;
    }

    if (obj->test_mode == 1) {
        // The buffer allows AWB/AE algos to converge before checksums are calculated
        obj->num_frames_to_run = TEST_BUFFER + 30;
    }

    for(frame_id = 0; frame_id < obj->num_frames_to_run; frame_id++)
    {
        if(obj->write_file == 1)
        {
            if((obj->captureObj.en_out_capture_write == 1) && (status == VX_SUCCESS))
            {
                status = app_send_cmd_capture_write_node(&obj->captureObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if((obj->vissObj.en_out_viss_write == 1) && (status == VX_SUCCESS))
            {
                status = app_send_cmd_viss_write_node(&obj->vissObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if((obj->vissObj1.en_out_viss_write == 1) && (status == VX_SUCCESS))
            {
                status = app_send_cmd_viss_write_node(&obj->vissObj1, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if((obj->ldcObj.en_out_ldc_write == 1) && (status == VX_SUCCESS))
            {
                status = app_send_cmd_ldc_write_node(&obj->ldcObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if((obj->ldcObj1.en_out_ldc_write == 1) && (status == VX_SUCCESS))
            {
                status = app_send_cmd_ldc_write_node(&obj->ldcObj1, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            obj->write_file = 0;
        }

        if (status == VX_SUCCESS)
        {
            status = app_run_graph_for_one_frame_pipeline(obj, frame_id);
        }

        /* user asked to stop processing */
        if(obj->stop_task)
          break;
    }

    if (status == VX_SUCCESS)
    {
        status = vxWaitGraph(obj->graph);
    }
    obj->stop_task = 1;

    if (status == VX_SUCCESS)
    {
        status = appStopImageSensor(obj->sensorObj.sensor_name, ch_mask);
    }

    return status;
}

#if !(defined(SOC_AM62A) && defined(QNX))
static void set_display_defaults(DisplayObj *displayObj)
{
    displayObj->display_option = 1;
}
#endif

static void app_pipeline_params_defaults(AppObj *obj)
{
    obj->pipeline       = -CAPTURE_BUFFER_Q_DEPTH + 1;
    obj->enqueueCnt     = 0;
    obj->dequeueCnt     = 0;
}

static void set_sensor_defaults(SensorObj *sensorObj)
{
    strcpy(sensorObj->sensor_name, SENSOR_SONY_IMX390_UB953_D3);

    sensorObj->num_sensors_found = 0;
    sensorObj->sensor_features_enabled = 0;
    sensorObj->sensor_features_supported = 0;
    sensorObj->sensor_dcc_enabled = 0;
    sensorObj->sensor_wdr_enabled = 0;
    sensorObj->sensor_exp_control_enabled = 0;
    sensorObj->sensor_gain_control_enabled = 0;
}

static void app_default_param_set(AppObj *obj)
{
    set_sensor_defaults(&obj->sensorObj);

#if !(defined(SOC_AM62A) && defined(QNX))
    set_display_defaults(&obj->displayObj);
#endif

    app_pipeline_params_defaults(obj);

    obj->is_interactive = 1;
    obj->stop_task = 0;
    obj->test_mode = 0;
    obj->write_file = 0;
    obj->bypass_split_graph = 0;

    obj->sensorObj.enable_ldc = 0;
    obj->sensorObj.num_cameras_enabled = 1;
    obj->sensorObj.ch_mask = 0x1;
    obj->sensorObj.usecase_option = APP_SENSOR_FEATURE_CFG_UC0;
}

static vx_int32 calc_grid_size(vx_uint32 ch)
{
    if(0==ch)
    {
        return -1;
    }
    else if(1==ch)
    {
        return 1;
    }
    else if(4>=ch)
    {
        return 2;
    }
    else if(9>=ch)
    {
        return 3;
    }
    else if(16>=ch)
    {
        return 4;
    }else
    {
        return -1;
    }
}

static void set_img_mosaic_params(ImgMosaicObj *imgMosaicObj, vx_uint32 in_width, vx_uint32 in_height, vx_int32 numCh, ObjArrSplitObj *objArrSplitObj, int32_t enable_split_graph)
{
    vx_int32 idx, ch;
    vx_int32 grid_size = calc_grid_size(numCh);

    imgMosaicObj->out_width    = DISPLAY_WIDTH;
    imgMosaicObj->out_height   = DISPLAY_HEIGHT;

    if (1 == enable_split_graph)
    {
        imgMosaicObj->num_inputs   = 2;
    }
    else
    {
        imgMosaicObj->num_inputs   = 1;
    }

    idx = 0;

    tivxImgMosaicParamsSetDefaults(&imgMosaicObj->params);

    for(ch = 0; ch < numCh; ch++)
    {
        vx_int32 winX = ch%grid_size;
        vx_int32 winY = ch/grid_size;

        imgMosaicObj->params.windows[idx].startX  = (winX * (in_width/grid_size));
        imgMosaicObj->params.windows[idx].startY  = (winY * (in_height/grid_size));
        imgMosaicObj->params.windows[idx].width   = in_width/grid_size;
        imgMosaicObj->params.windows[idx].height  = in_height/grid_size;
        imgMosaicObj->params.windows[idx].input_select   = 0;
        if (1 == enable_split_graph)
        {
            if(ch >= objArrSplitObj->output0_num_elements)
            {
                imgMosaicObj->params.windows[idx].input_select   = 1;
            }
            imgMosaicObj->params.windows[idx].channel_select = ch%objArrSplitObj->output0_num_elements;
        }
        else
        {
            imgMosaicObj->params.windows[idx].channel_select = ch;
        }
        idx++;
    }

    imgMosaicObj->params.num_windows  = idx;

    /* Number of time to clear the output buffer before it gets reused */
    imgMosaicObj->params.clear_count  = CAPTURE_BUFFER_Q_DEPTH;
}

static void app_update_param_set(AppObj *obj)
{
    vx_uint16 resized_width, resized_height;
    appIssGetResizeParams(obj->sensorObj.image_width, obj->sensorObj.image_height, DISPLAY_WIDTH, DISPLAY_HEIGHT, &resized_width, &resized_height);

    if ( obj->sensorObj.num_cameras_enabled == 1 )
    {
        obj->objArrSplitObj.output0_num_elements = 1;
        obj->objArrSplitObj.output1_num_elements = 0;
        obj->enable_split_graph = 0;
    }
    else if (1 == obj->bypass_split_graph)
    {
        obj->objArrSplitObj.output0_num_elements = obj->sensorObj.num_cameras_enabled;
        obj->objArrSplitObj.output1_num_elements = 0;
        obj->enable_split_graph = 0;
    }
    else
    {
        obj->objArrSplitObj.output0_num_elements = (obj->sensorObj.num_cameras_enabled + 1) / 2;
        obj->objArrSplitObj.output1_num_elements = obj->sensorObj.num_cameras_enabled - obj->objArrSplitObj.output0_num_elements;
    }
    set_img_mosaic_params(&obj->imgMosaicObj, resized_width, resized_height, obj->sensorObj.num_cameras_enabled, &obj->objArrSplitObj, obj->enable_split_graph);
}

/*
 * Utility API used to add a graph parameter from a node, node parameter index
 */
static void add_graph_parameter_by_node_index(vx_graph graph, vx_node node, vx_uint32 node_parameter_index)
{
    vx_parameter parameter = vxGetParameterByIndex(node, node_parameter_index);

    vxAddParameterToGraph(graph, parameter);
    vxReleaseParameter(&parameter);
}

#if !(defined(SOC_AM62A) && defined(QNX))
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    if(update_type == 0)
    {
        Draw2D_FontPrm sHeading;

        sHeading.fontIdx = 4;
        Draw2D_drawString(handle, 700, 5, "Multi Cam Demo", &sHeading);
    }

  return;
}
#endif
