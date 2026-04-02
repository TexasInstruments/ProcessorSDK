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

#include <utils/draw2d/include/draw2d.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include <utils/console_io/include/app_get.h>
#include <utils/grpx/include/app_grpx.h>
#include <VX/vx_khr_pipelining.h>
#include <TI/video_io_kernels.h>

#include "app_common.h"
#include "app_sensor_module.h"
#include "app_capture_module.h"
#include "app_viss_module.h"
#include "app_aewb_module.h"
#include "app_ldc_module.h"
#include "app_scaler_module.h"
#include "app_pre_proc_module.h"
#include "app_tidl_module.h"
#include "app_post_proc_module.h"
#include "app_img_mosaic_module.h"
#include "app_display_module.h"
#include "app_srv_module.h"
#include "app_img_hist_module.h"
#include "app_dof_pyramid_module.h"
#include "app_dof_proc_module.h"

#define APP_BUFFER_Q_DEPTH   (4)
#define APP_PIPELINE_DEPTH   (10)

#define ENABLE_SRV
#define ENABLE_DISPLAY

typedef struct {

    SensorObj     sensorObj;
    CaptureObj    captureObj;
    VISSObj       vissObj;
    AEWBObj       aewbObj;
    LDCObj        ldcObj;
    ScalerObj     scalerObj;
    PreProcObj    preProcObj;
    TIDLObj       tidlObj;
    PostProcObj   postProcObj;
    ImgMosaicObj  imgMosaicObj;
    DisplayObj    displayObj;
    SRVObj        srvObj;
    ImgHistObj    imgHistObj;
    PyramidObj    pyramidObj;
    DofProcObj    dofProcObj;

    vx_char input_file_path[APP_MAX_FILE_PATH];
    vx_char output_file_path[APP_MAX_FILE_PATH];

    /* OpenVX references */
    vx_context context;
    vx_graph   graph;

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

    int32_t pipeline_exec;

    int32_t pipeline;
    int32_t enqueueCnt;
    int32_t dequeueCnt;

    int32_t enable_capture;
    int32_t enable_viss;
    int32_t enable_aewb;
    int32_t enable_ldc;
    int32_t enable_scaler;
    int32_t enable_pre_proc;
    int32_t enable_tidl;
    int32_t enable_post_proc;
    int32_t enable_mosaic;
    int32_t enable_display;
    int32_t enable_srv;
    int32_t enable_hist;
    int32_t enable_dof;

    int32_t write_file;

} AppObj;

AppObj gAppObj;

static const vx_uint8 color_map[5][3] = {{152,251,152},{0,130,180},{220,20,60},{3,3,251},{190,153,153}};

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

#ifndef x86_64
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type);
#endif

static void app_show_usage(vx_int32 argc, vx_char* argv[])
{
    printf("\n");
    printf(" TIDL - AVP4 Demo - (c) Texas Instruments 2020\n");
    printf(" ========================================================\n");
    printf("\n");
    printf(" Usage,\n");
    printf("  %s --cfg <config file>\n", argv[0]);
    printf("\n");
}

static const char menu[] = {
    "\n"
    "\n ========================="
    "\n Demo : TIDL - AVP4 Demo"
    "\n ========================="
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
    "\n"
    "\n s: Save intermediate outputs"
#endif
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

    while(!obj->stop_task)
    {
        app_run_graph(obj);
    }
    obj->stop_task_done = 1;
}

static int32_t app_run_task_create(AppObj *obj)
{
    tivx_task_create_params_t params;
    int32_t status;

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
    if(status!=0)
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
                    tivx_utils_graph_perf_print(obj->graph);
                    appPerfPointPrint(&obj->fileio_perf);
                    appPerfPointPrint(&obj->total_perf);
                    printf("\n");
                    appPerfPointPrintFPS(&obj->total_perf);
                    appPerfPointReset(&obj->total_perf);
                    printf("\n");

                    if(obj->enable_capture)
                    {
                        vx_reference refs[1];
                        refs[0] = (vx_reference)obj->captureObj.raw_image_arr[0];
                        status = tivxNodeSendCommand(obj->captureObj.node, 0u,
                                    TIVX_CAPTURE_PRINT_STATISTICS,
                                    refs, 1u);
                    }
                    break;
                case 'e':
                    perf_arr[0] = &obj->total_perf;
                    fp = appPerfStatsExportOpenFile(".", "dl_demos_app_tidl_avp4");
                    if (NULL != fp)
                    {
                        appPerfStatsExportAll(fp, perf_arr, 1);
                        tivx_utils_graph_perf_export(fp, obj->graph);
                        appPerfStatsExportCloseFile(fp);
                        appPerfStatsResetAll();
                    }
                    else
                    {
                        printf("fp is null\n");
                    }
                    break;
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
                case 's':
                    obj->write_file = 1;
                    break;
#endif
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

static void app_set_cfg_default(AppObj *obj)
{
    snprintf(obj->captureObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->vissObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->ldcObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->scalerObj.output_file_path,APP_MAX_FILE_PATH, ".");
    snprintf(obj->srvObj.output_file_path,APP_MAX_FILE_PATH, ".");

    obj->captureObj.en_out_capture_write = 0;
    obj->vissObj.en_out_viss_write = 0;
    obj->ldcObj.en_out_ldc_write = 0;
    obj->scalerObj.en_out_scaler_write = 0;
    obj->srvObj.en_out_srv_write = 0;

    obj->num_frames_to_write = 0;
    obj->num_frames_to_skip = 0;

}

static void app_parse_cfg_file(AppObj *obj, vx_char *cfg_file_name)
{
    FILE *fp = fopen(cfg_file_name, "r");
    vx_char line_str[1024];
    vx_char *token;

    uint32_t camx_idx = 0, camy_idx = 0, camz_idx = 0;
    uint32_t targetx_idx = 0, targety_idx = 0, targetz_idx = 0;
    uint32_t anglex_idx = 0, angley_idx = 0, anglez_idx = 0;

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
            if(strcmp(token, "enable_capture")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_capture = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_viss")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_viss = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_aewb")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_aewb = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_ldc")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_ldc = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_dof")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_dof = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_scaler")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_scaler = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_pre_proc")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_pre_proc = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_tidl")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_tidl = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_post_proc")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_post_proc = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_mosaic")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_mosaic = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_display")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_display = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_ldc")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_ldc = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_srv")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_srv = atoi(token);
                }
            }
            else
            if(strcmp(token, "enable_hist")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->enable_hist = atoi(token);
                }
            }
            else
            if(strcmp(token, "pipeline_exec")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->pipeline_exec = atoi(token);
                }
            }
            else
            if(strcmp(token, "num_cameras_enabled")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->sensorObj.num_cameras_enabled = atoi(token);
                }
            }
            else
            if(strcmp(token, "sensor_index")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->sensorObj.sensor_index = atoi(token);
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
                obj->sensorObj.is_interactive = obj->is_interactive;
            }
            else
            if(strcmp(token, "tidl_config")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->tidlObj.config_file_path, token);
                }
            }
            else
            if(strcmp(token, "tidl_network")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    strcpy(obj->tidlObj.network_file_path, token);
                }
            }
            else
            if(strcmp(token, "num_classes")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    obj->postProcObj.viz_params.num_classes[0] = atoi(token);
                }
            }
            else
            if(strcmp(token, "dl_size")==0)
            {
                vx_int32 width, height;

                token = strtok(NULL, s);
                if(token != NULL)
                {
                    width =  atoi(token);
                    obj->scalerObj.output[0].width  = width;
                    obj->preProcObj.in_width  = width;
                    obj->postProcObj.in_width  = width;
                    obj->imgHistObj.in_width = width;
                    obj->pyramidObj.width = width;
                    obj->dofProcObj.width = width;

                    token = strtok(NULL, s);
                    if(token != NULL)
                    {
                        if(token[strlen(token)-1] == '\n')
                        token[strlen(token)-1]=0;

                        height =  atoi(token);
                        obj->scalerObj.output[0].height = height;
                        obj->preProcObj.in_height  = height;
                        obj->postProcObj.in_height  = height;
                        obj->imgHistObj.in_height = height;
                        obj->pyramidObj.height = height;
                        obj->dofProcObj.height = height;

                    }
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
                    strcpy(obj->ldcObj.output_file_path, token);
                    strcpy(obj->scalerObj.output_file_path, token);
                    strcpy(obj->srvObj.output_file_path, token);
                }
            }
            else if(strcmp(token, "offsetXleft")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.offsetXleft = atoi(token);
                }
            }
            else if(strcmp(token, "offsetXright")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.offsetXright = atoi(token);
                }
            }
            else if(strcmp(token, "offsetYfront")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.offsetYfront = atoi(token);
                }
            }
            else if(strcmp(token, "offsetYback")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.offsetYback = atoi(token);
                }
            }
            else if(strcmp(token, "num_views")==0)
            {
                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.num_views = atoi(token);
                    if (obj->srvObj.params.num_views > MAX_SRV_VIEWS)
                    {
                        printf("Config file num_views argument exceeds MAX_SRV_VIEWS = %d\n", MAX_SRV_VIEWS);
                        exit(0);
                    }
                }
            }
            else if(strcmp(token, "camx")==0)
            {
                if (obj->srvObj.params.num_views < (camx_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.camx[camx_idx] = atof(token);
                    APP_PRINTF("obj->camx[%d] = %f\n", camx_idx, obj->srvObj.params.camx[camx_idx]);
                    camx_idx++;
                }
            }
            else if(strcmp(token, "camy")==0)
            {
                if (obj->srvObj.params.num_views < (camy_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.camy[camy_idx] = atof(token);
                    APP_PRINTF("obj->camy[%d] = %f\n", camy_idx, obj->srvObj.params.camy[camy_idx]);
                    camy_idx++;
                }
            }
            else if(strcmp(token, "camz")==0)
            {
                if (obj->srvObj.params.num_views < (camz_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.camz[camz_idx] = atof(token);
                    APP_PRINTF("obj->camz[%d] = %f\n", camz_idx, obj->srvObj.params.camz[camz_idx]);
                    camz_idx++;
                }
            }
            else if(strcmp(token, "targetx")==0)
            {
                if (obj->srvObj.params.num_views < (targetx_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.targetx[targetx_idx] = atof(token);
                    APP_PRINTF("obj->targetx[%d] = %f\n", targetx_idx, obj->srvObj.params.targetx[targetx_idx]);
                    targetx_idx++;
                }
            }
            else if(strcmp(token, "targety")==0)
            {
                if (obj->srvObj.params.num_views < (targety_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.targety[targety_idx] = atof(token);
                    APP_PRINTF("obj->targety[%d] = %f\n", targety_idx, obj->srvObj.params.targety[targety_idx]);
                    targety_idx++;
                }
            }
            else if(strcmp(token, "targetz")==0)
            {
                if (obj->srvObj.params.num_views < (targetz_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.targetz[targetz_idx] = atof(token);
                    APP_PRINTF("obj->targetz[%d] = %f\n", targetz_idx, obj->srvObj.params.targetz[targetz_idx]);
                    targetz_idx++;
                }
            }
            else if(strcmp(token, "anglex")==0)
            {
                if (obj->srvObj.params.num_views < (anglex_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.anglex[anglex_idx] = atof(token);
                    APP_PRINTF("obj->anglex[%d] = %f\n", anglex_idx, obj->srvObj.params.anglex[anglex_idx]);
                    anglex_idx++;
                }
            }
            else if(strcmp(token, "angley")==0)
            {
                if (obj->srvObj.params.num_views < (angley_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.angley[angley_idx] = atof(token);
                    APP_PRINTF("obj->angley[%d] = %f\n", angley_idx, obj->srvObj.params.angley[angley_idx]);
                    angley_idx++;
                }
            }
            else if(strcmp(token, "anglez")==0)
            {
                if (obj->srvObj.params.num_views < (anglez_idx+1))
                {
                    printf("Mismatch between number of views and view parameters!!!\n");
                    exit(0);
                }

                token = strtok(NULL, s);
                if (NULL != token)
                {
                    obj->srvObj.params.anglez[anglez_idx] = atof(token);
                    APP_PRINTF("obj->anglez[%d] = %f\n", anglez_idx, obj->srvObj.params.anglez[anglez_idx]);
                    anglez_idx++;
                }
            }
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
            else
            if(strcmp(token, "num_frames_to_run")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->num_frames_to_run = atoi(token);
                }
            }
            else
            if(strcmp(token, "en_out_capture_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
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
                    token[strlen(token)-1]=0;
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
                    token[strlen(token)-1]=0;
                    obj->ldcObj.en_out_ldc_write = atoi(token);
                    if(obj->ldcObj.en_out_ldc_write > 1)
                        obj->ldcObj.en_out_ldc_write = 1;
                }
            }
            else
            if(strcmp(token, "en_out_scaler_write")==0)
            {
                token = strtok(NULL, s);
                if(token != NULL)
                {
                    token[strlen(token)-1]=0;
                    obj->scalerObj.en_out_scaler_write = atoi(token);
                    if(obj->scalerObj.en_out_scaler_write > 1)
                        obj->scalerObj.en_out_scaler_write = 1;
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
#endif
        }
    }

    fclose(fp);

}

static void app_parse_cmd_line_args(AppObj *obj, vx_int32 argc, vx_char *argv[])
{
    vx_int32 i;

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
            break;
        }
        else
        if(strcmp(argv[i], "--help")==0)
        {
            app_show_usage(argc, argv);
            exit(0);
        }
    }

    return;
}

vx_int32 app_tidl_avp4_main(vx_int32 argc, vx_char* argv[])
{
    vx_status status = VX_SUCCESS;

    AppObj *obj = &gAppObj;

    /*Optional parameter setting*/
    app_default_param_set(obj);

    /*Config parameter reading*/
    app_parse_cmd_line_args(obj, argc, argv);

    /* Querry sensor parameters */
    app_querry_sensor(&obj->sensorObj);

    /*Update of parameters are config file read*/
    app_update_param_set(obj);

    status = app_init(obj);

    if(status == VX_SUCCESS)
    {
        APP_PRINTF("App Init Done! \n");

        status = app_create_graph(obj);

        if(status == VX_SUCCESS)
        {
            APP_PRINTF("App Create Graph Done! \n");

            status = app_verify_graph(obj);

            APP_PRINTF("App Verify Graph Done! \n");

            if((status == VX_SUCCESS) && (obj->enable_srv == 1))
            {
                status = app_run_graph_gpu_lut(&obj->srvObj);
                APP_PRINTF("App GPU LUT Graph Done! \n");
            }

            if(status == VX_SUCCESS)
            {
                if(obj->is_interactive)
                {
                    app_run_graph_interactive(obj);
                }
                else
                {
                    app_run_graph(obj);
                }
            }
        }

        APP_PRINTF("App Run Graph Done! \n");
    }

    app_delete_graph(obj);

    APP_PRINTF("App Delete Graph Done! \n");

    app_deinit(obj);

    APP_PRINTF("App De-init Done! \n");

    return 0;
}

static vx_status app_init(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    app_grpx_init_prms_t grpx_prms;

    /* Create OpenVx Context */
    obj->context = vxCreateContext();
    APP_ASSERT_VALID_REF(obj->context);
    APP_PRINTF("Creating context done!\n");

    tivxSrvLoadKernels(obj->context);
    tivxHwaLoadKernels(obj->context);
    tivxVideoIOLoadKernels(obj->context);
    tivxImagingLoadKernels(obj->context);
    tivxImgProcLoadKernels(obj->context);
    tivxFileIOLoadKernels(obj->context);
    APP_PRINTF("Kernel loading done!\n");

    /* Initialize modules */

    if(obj->enable_capture == 1)
    {
        app_init_sensor(&obj->sensorObj, "sensor_obj");
        APP_PRINTF("Sensor init done!\n");

        app_init_capture(obj->context, &obj->captureObj, &obj->sensorObj, "capture_obj", APP_BUFFER_Q_DEPTH);
        APP_PRINTF("Capture init done!\n");
    }

    if(obj->enable_viss == 1)
    {
        app_init_viss(obj->context, &obj->vissObj, &obj->sensorObj, "viss_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("VISS init done!\n");
    }

    if(obj->enable_aewb == 1)
    {
        app_init_aewb(obj->context, &obj->aewbObj, &obj->sensorObj, "aewb_obj", 0, obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("AEWB init done!\n");
    }

    if(obj->enable_srv == 1)
    {
        app_init_srv(obj->context, &obj->srvObj, &obj->sensorObj, "srv_obj");
        APP_PRINTF("SRV init done!\n");
    }

    if(obj->enable_ldc == 1)
    {
        app_init_ldc(obj->context, &obj->ldcObj, &obj->sensorObj, "ldc_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("LDC init done!\n");
    }

    if(obj->enable_scaler == 1)
    {
        app_init_scaler(obj->context, &obj->scalerObj, "scaler_obj", obj->sensorObj.num_cameras_enabled, 1);
        APP_PRINTF("Scaler init done!\n");
    }

    if(obj->enable_dof == 1)
    {
#ifdef ENABLE_DOF_PYRAMID
        app_init_pyramid(obj->context, &obj->pyramidObj, "pyramid_obj", obj->sensorObj.num_cameras_enabled);
#endif
        app_init_dof_proc(obj->context, &obj->dofProcObj, "dof_proc_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("DOF init done!\n");
    }

    if((obj->enable_tidl == 1) || (obj->enable_pre_proc == 1) || (obj->enable_post_proc == 1))
    {
        /* Initialize TIDL first to get tensor I/O information from network */
        app_init_tidl(obj->context, &obj->tidlObj, "tidl_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("TIDL Init Done! \n");
    }

    if(obj->enable_pre_proc == 1)
    {
        /* Update pre-proc parameters with TIDL config before calling init */
        app_update_pre_proc(obj->context, &obj->preProcObj, obj->tidlObj.config, obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("Pre Proc Update Done! \n");

        app_init_pre_proc(obj->context, &obj->preProcObj, "pre_proc_obj");
        APP_PRINTF("Pre Proc Init Done! \n");
    }

    if(obj->enable_post_proc == 1)
    {
        /* Update post-proc parameters with TIDL config before calling init */
        app_update_post_proc(obj->context, &obj->postProcObj, obj->tidlObj.config, obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("Post Proc Update Done! \n");

        app_init_post_proc(obj->context, &obj->postProcObj, "post_proc_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("Post Proc Init Done! \n");
    }

    if(obj->enable_mosaic == 1)
    {
        app_init_img_mosaic(obj->context, &obj->imgMosaicObj, "img_mosaic_obj", 1);
        APP_PRINTF("Img Mosaic init done!\n");
    }

    if(obj->enable_hist == 1)
    {
        app_init_img_hist(obj->context, &obj->imgHistObj, "img_hist_obj", obj->sensorObj.num_cameras_enabled);
        APP_PRINTF("Img Hist init done!\n");
    }

    if(obj->enable_display == 1)
    {
        app_init_display(obj->context, &obj->displayObj, "display_obj");
        APP_PRINTF("Display init done!\n");

        #ifndef x86_64
        if(obj->displayObj.display_option == 1)
        {
            appGrpxInitParamsInit(&grpx_prms, obj->context);
            grpx_prms.draw_callback = app_draw_graphics;
            appGrpxInit(&grpx_prms);
        }
        #endif
    }

    appPerfPointSetName(&obj->total_perf , "TOTAL");
    appPerfPointSetName(&obj->fileio_perf, "FILEIO");

    return status;
}

static void app_deinit(AppObj *obj)
{
    if(obj->enable_capture == 1)
    {
        app_deinit_sensor(&obj->sensorObj);
        APP_PRINTF("Sensor deinit done!\n");

        app_deinit_capture(&obj->captureObj, APP_BUFFER_Q_DEPTH);
        APP_PRINTF("Capture deinit done!\n");
    }

    if(obj->enable_viss == 1)
    {
        app_deinit_viss(&obj->vissObj);
        APP_PRINTF("VISS deinit done!\n");
    }

    if(obj->enable_aewb == 1)
    {
        app_deinit_aewb(&obj->aewbObj);
        APP_PRINTF("AEWB deinit done!\n");
    }

    if(obj->enable_srv == 1)
    {
        app_deinit_srv(&obj->srvObj);
        APP_PRINTF("SRV deinit done!\n");
    }

    if(obj->enable_ldc == 1)
    {
        app_deinit_ldc(&obj->ldcObj);
        APP_PRINTF("LDC deinit done!\n");
    }

    if(obj->enable_scaler == 1)
    {
        app_deinit_scaler(&obj->scalerObj);
        APP_PRINTF("Scaler deinit done!\n");
    }

    if(obj->enable_dof == 1)
    {
#ifdef ENABLE_DOF_PYRAMID
        app_deinit_pyramid(&obj->pyramidObj);
#endif
        app_deinit_dof_proc(&obj->dofProcObj);
        APP_PRINTF("DOF deinit done!\n");
    }

    if(obj->enable_pre_proc == 1)
    {
        app_deinit_pre_proc(&obj->preProcObj);
        APP_PRINTF("Pre proc deinit done!\n");
    }

    if((obj->enable_tidl == 1) || (obj->enable_pre_proc == 1) || (obj->enable_post_proc == 1))
    {
        app_deinit_tidl(&obj->tidlObj);
        APP_PRINTF("TIDL deinit done!\n");
    }

    if(obj->enable_post_proc == 1)
    {
        app_deinit_post_proc(&obj->postProcObj);
        APP_PRINTF("Post proc deinit done!\n");
    }

    if(obj->enable_mosaic == 1)
    {
        app_deinit_img_mosaic(&obj->imgMosaicObj, 1);
        APP_PRINTF("Img Mosaic deinit done!\n");
    }

    if(obj->enable_hist == 1)
    {
        app_deinit_img_hist(&obj->imgHistObj);
        APP_PRINTF("Img Hist deinit done!\n");
    }

    if(obj->enable_display == 1)
    {
        app_deinit_display(&obj->displayObj);
        APP_PRINTF("Display deinit done!\n");

        #ifndef x86_64
        if(obj->displayObj.display_option == 1)
        {
        appGrpxDeInit();
        }
        #endif
    }

    tivxSrvUnLoadKernels(obj->context);
    tivxHwaUnLoadKernels(obj->context);
    tivxVideoIOUnLoadKernels(obj->context);
    tivxImagingUnLoadKernels(obj->context);
    tivxImgProcUnLoadKernels(obj->context);
    tivxFileIOUnLoadKernels(obj->context);
    APP_PRINTF("Kernels unload done!\n");

    vxReleaseContext(&obj->context);
    APP_PRINTF("Release context done!\n");
}

static void app_delete_graph(AppObj *obj)
{
    if(obj->enable_capture == 1)
    {
        app_delete_capture(&obj->captureObj);
        APP_PRINTF("Capture delete done!\n");
    }
    if(obj->enable_viss == 1)
    {
        app_delete_viss(&obj->vissObj);
        APP_PRINTF("VISS delete done!\n");
    }

    if(obj->enable_aewb == 1)
    {
        app_delete_aewb(&obj->aewbObj);
        APP_PRINTF("AEWB delete done!\n");
    }

    if(obj->enable_srv == 1)
    {
        app_delete_srv(&obj->srvObj);
        APP_PRINTF("SRV delete done!\n");
    }

    if(obj->enable_ldc == 1)
    {
        app_delete_ldc(&obj->ldcObj);
        APP_PRINTF("LDC delete done!\n");
    }

    if(obj->enable_scaler == 1)
    {
        app_delete_scaler(&obj->scalerObj);
        APP_PRINTF("Scaler delete done!\n");
    }

    if(obj->enable_dof == 1)
    {
#ifdef ENABLE_DOF_PYRAMID
        app_delete_pyramid(&obj->pyramidObj);
#endif
        app_delete_dof_proc(&obj->dofProcObj);
        APP_PRINTF("DOF delete done!\n");
    }

    if(obj->enable_pre_proc == 1)
    {
        app_delete_pre_proc(&obj->preProcObj);
        APP_PRINTF("Pre Proc delete done!\n");
    }

    if(obj->enable_tidl == 1)
    {
        app_delete_tidl(&obj->tidlObj);
        APP_PRINTF("TIDL delete done!\n");
    }

    if(obj->enable_post_proc == 1)
    {
        app_delete_post_proc(&obj->postProcObj);
        APP_PRINTF("Post Proc delete done!\n");
    }

    if(obj->enable_mosaic == 1)
    {
        app_delete_img_mosaic(&obj->imgMosaicObj);
        APP_PRINTF("Img Mosaic delete done!\n");
    }

    if(obj->enable_hist == 1)
    {
        app_delete_img_hist(&obj->imgHistObj);
        APP_PRINTF("Img Hist delete done!\n");
    }

    if(obj->enable_display == 1)
    {
        app_delete_display(&obj->displayObj);
        APP_PRINTF("Display delete done!\n");
    }

    #ifdef APP_TIVX_LOG_RT_ENABLE
    tivxLogRtTraceExportToFile("app_tidl_avp4.bin");
    tivxLogRtTraceDisable(obj->graph);
    #endif

    vxReleaseGraph(&obj->graph);
    APP_PRINTF("Graph delete done!\n");
}

static vx_status app_create_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;
    vx_graph_parameter_queue_params_t graph_parameters_queue_params_list[2];
    vx_int32 graph_parameter_index;

    obj->graph = vxCreateGraph(obj->context);
    status = vxGetStatus((vx_reference)obj->graph);
    vxSetReferenceName((vx_reference)obj->graph, "app_tidl_avp4_graph");
    APP_PRINTF("Graph create done!\n");

    if((status == VX_SUCCESS) && (obj->enable_capture == 1))
    {
        status = app_create_graph_capture(obj->graph, &obj->captureObj);
        APP_PRINTF("Capture graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_viss == 1))
    {
        status = app_create_graph_viss(obj->graph, &obj->vissObj, obj->captureObj.raw_image_arr[0], TIVX_TARGET_VPAC_VISS1);
        APP_PRINTF("VISS graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_aewb == 1))
    {
        status = app_create_graph_aewb(obj->graph, &obj->aewbObj, obj->vissObj.h3a_stats_arr);
        APP_PRINTF("AEWB graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_srv == 1))
    {
        status = app_create_graph_srv(obj->context, obj->graph, &obj->srvObj, obj->vissObj.output_arr);
        APP_PRINTF("SRV graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_ldc == 1))
    {
        if(status == VX_SUCCESS)
        {
            status = app_create_graph_ldc(obj->graph, &obj->ldcObj, obj->vissObj.output_arr, TIVX_TARGET_VPAC_LDC1);
            APP_PRINTF("LDC graph done!\n");
        }
    }

    if((status == VX_SUCCESS) && (obj->enable_scaler == 1))
    {
        if(obj->enable_ldc == 1)
        {
            app_create_graph_scaler(obj->context, obj->graph, &obj->scalerObj, obj->ldcObj.output_arr);
        }
        else
        {
            app_create_graph_scaler(obj->context, obj->graph, &obj->scalerObj, obj->vissObj.output_arr);
        }
        APP_PRINTF("Scaler graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_hist == 1))
    {
        if(obj->enable_ldc == 1)
        {
            app_create_graph_img_hist(obj->graph, &obj->imgHistObj, obj->ldcObj.output_arr);
        }
        else
        {
            app_create_graph_img_hist(obj->graph, &obj->imgHistObj, obj->vissObj.output_arr);
        }
        APP_PRINTF("Img Hist graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_dof == 1))
    {
#ifdef ENABLE_DOF_PYRAMID
        app_create_graph_pyramid(obj->graph, &obj->pyramidObj, obj->scalerObj.output[0].arr, DOF_PYRAMID_START_FROM_PREVIOUS);

        vxRegisterAutoAging(obj->graph, obj->pyramidObj.pyramid_delay);
#endif
        if(obj->dofProcObj.enable_temporal_predicton_flow_vector == 1)
        {
            vxRegisterAutoAging(obj->graph, obj->dofProcObj.flow_vector_field_delay);
        }

        app_create_graph_dof_proc(obj->graph, &obj->dofProcObj, obj->pyramidObj.pyramid_delay);


        APP_PRINTF("DOF graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_pre_proc == 1))
    {
        app_create_graph_pre_proc(obj->graph, &obj->preProcObj, obj->scalerObj.output[0].arr);
        APP_PRINTF("Pre proc graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_tidl == 1))
    {
        app_create_graph_tidl(obj->context, obj->graph, &obj->tidlObj, obj->preProcObj.output_tensor_arr);
        APP_PRINTF("TIDL graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_post_proc == 1))
    {
        app_create_graph_post_proc(obj->graph, &obj->postProcObj, obj->scalerObj.output[0].arr, obj->tidlObj.out_args_arr, obj->tidlObj.output_tensor_arr[0]);
        APP_PRINTF("Post proc graph done!\n");
    }


    if((status == VX_SUCCESS) && (obj->enable_mosaic == 1))
    {
        vx_int32 idx = 0;

        if(obj->enable_post_proc == 1)
        {
            /* Provide post-proc output to mosaic */
            obj->imgMosaicObj.input_arr[idx++] = obj->postProcObj.output_image_arr;
            obj->imgMosaicObj.num_inputs = idx;
        }
        else if(obj->enable_scaler == 1)
        {
            /* Provide scaler node output[0] to mosaic */
            obj->imgMosaicObj.input_arr[idx++] = obj->scalerObj.output[0].arr;
            obj->imgMosaicObj.num_inputs = idx;
        }

        status = app_create_graph_img_mosaic(obj->graph, &obj->imgMosaicObj, NULL);
        APP_PRINTF("Img Mosaic graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->enable_display == 1))
    {
        status = app_create_graph_display(obj->graph, &obj->displayObj, obj->imgMosaicObj.output_image[0]);
        APP_PRINTF("Display graph done!\n");
    }

    if((status == VX_SUCCESS) && (obj->pipeline_exec == 1))
    {
        graph_parameter_index = 0;

        if(obj->enable_capture == 1)
        {
            add_graph_parameter_by_node_index(obj->graph, obj->captureObj.node, 1);
            obj->captureObj.graph_parameter_index = graph_parameter_index;
            graph_parameters_queue_params_list[graph_parameter_index].graph_parameter_index = graph_parameter_index;
            graph_parameters_queue_params_list[graph_parameter_index].refs_list_size = APP_BUFFER_Q_DEPTH;
            graph_parameters_queue_params_list[graph_parameter_index].refs_list = (vx_reference*)&obj->captureObj.raw_image_arr[0];
            graph_parameter_index++;
        }

        vxSetGraphScheduleConfig(obj->graph,
                VX_GRAPH_SCHEDULE_MODE_QUEUE_AUTO,
                graph_parameter_index,
                graph_parameters_queue_params_list);

        tivxSetGraphPipelineDepth(obj->graph, APP_PIPELINE_DEPTH);

        if(obj->enable_viss == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->vissObj.node, 6, APP_BUFFER_Q_DEPTH);
            tivxSetNodeParameterNumBufByIndex(obj->vissObj.node, 9, APP_BUFFER_Q_DEPTH);
        }

        if(obj->enable_aewb == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->aewbObj.node, 4, APP_BUFFER_Q_DEPTH);
        }

        if(obj->enable_ldc == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->ldcObj.node, 7, APP_BUFFER_Q_DEPTH);
        }

        /*This output is accessed slightly later in the pipeline by mosaic node so queue depth is larger */
        if(obj->enable_scaler == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->scalerObj.node, 1, 6);
        }

        if(obj->enable_dof == 1)
        {
#ifdef ENABLE_DOF_PYRAMID
            tivxSetNodeParameterNumBufByIndex(obj->pyramidObj.node, 1, 2);
#endif
        }

        if(obj->enable_pre_proc == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->preProcObj.node, 2, APP_BUFFER_Q_DEPTH);
        }

        if(obj->enable_tidl == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->tidlObj.node, 7, APP_BUFFER_Q_DEPTH);
        }

        if(obj->enable_post_proc == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->postProcObj.node, 4, APP_BUFFER_Q_DEPTH);
        }

        if(obj->enable_mosaic == 1)
        {
            tivxSetNodeParameterNumBufByIndex(obj->imgMosaicObj.node, 1, APP_BUFFER_Q_DEPTH);
        }

        APP_PRINTF("Pipeline params setup done!\n");
    }

    return status;
}

static vx_status app_verify_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    status = vxVerifyGraph(obj->graph);

    if(status == VX_SUCCESS)
    {
        APP_PRINTF("Grapy verify done!\n");
    }

    #if 1
    if(VX_SUCCESS == status)
    {
        status = tivxExportGraphToDot(obj->graph,".", "vx_app_tidl_avp4");
    }
    #endif

    #ifdef APP_TIVX_LOG_RT_ENABLE
    tivxLogRtTraceEnable(obj->graph);
    #endif

    if (obj->captureObj.enable_error_detection)
    {
        status = app_send_error_frame(&obj->captureObj);
        APP_PRINTF("App Send Error Frame Done! %d \n", obj->captureObj.enable_error_detection);
    }

    /* wait a while for prints to flush */
    tivxTaskWaitMsecs(100);

    return status;
}

static vx_status app_run_graph_for_one_frame_sequential(AppObj *obj, vx_int32 frame_id)
{
    vx_status status = VX_SUCCESS;
    vx_char input_file_name[APP_MAX_FILE_PATH];

    snprintf(input_file_name, APP_MAX_FILE_PATH, "%s/%010d.yuv", obj->input_file_path, frame_id);

    appPerfPointBegin(&obj->total_perf);

    appPerfPointBegin(&obj->fileio_perf);

    /* Read input files here */

    appPerfPointEnd(&obj->fileio_perf);

    status = vxProcessGraph(obj->graph);

    appPerfPointEnd(&obj->total_perf);

    return status;
}

static vx_status app_run_graph_for_one_frame_pipeline(AppObj *obj, vx_int32 frame_id)
{
    vx_status status = VX_SUCCESS;

    appPerfPointBegin(&obj->total_perf);

    CaptureObj *captureObj = &obj->captureObj;

    if(obj->pipeline < 0)
    {
        /* Enqueue outpus */

        /* Enqueue inputs during pipeup dont execute */
        if(obj->enable_capture == 1)
        {
            vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&captureObj->raw_image_arr[obj->enqueueCnt], 1);
        }

        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }

    if(obj->pipeline == 0)
    {
        /* Execute 1st frame */
        if(obj->enable_capture == 1)
        {
            vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&captureObj->raw_image_arr[obj->enqueueCnt], 1);
        }
        obj->enqueueCnt++;
        obj->enqueueCnt   = (obj->enqueueCnt  >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
        obj->pipeline++;
    }

    if(obj->pipeline > 0)
    {
        vx_reference raw_image;
        uint32_t num_refs;

        if(obj->enable_capture == 1)
        {
            /* Dequeue input */
            vxGraphParameterDequeueDoneRef(obj->graph, captureObj->graph_parameter_index, (vx_reference*)&raw_image, 1, &num_refs);
        }

    if(obj->enable_capture == 1)
    {
        /* Enqueue input - start execution */
        vxGraphParameterEnqueueReadyRef(obj->graph, captureObj->graph_parameter_index, &raw_image, 1);
    }

    obj->enqueueCnt++;
    obj->dequeueCnt++;

    obj->enqueueCnt = (obj->enqueueCnt >= APP_BUFFER_Q_DEPTH)? 0 : obj->enqueueCnt;
    obj->dequeueCnt = (obj->dequeueCnt >= APP_BUFFER_Q_DEPTH)? 0 : obj->dequeueCnt;

  }

  appPerfPointEnd(&obj->total_perf);

  return status;
}

static vx_status app_run_graph(AppObj *obj)
{
    vx_status status = VX_SUCCESS;

    SensorObj *sensorObj = &obj->sensorObj;
    vx_int32 frame_id;

    app_pipeline_params_defaults(obj);

    if(obj->enable_capture == 1)
    {
        if('\0' == sensorObj->sensor_name[0])
        {
            printf("sensor name is NULL \n");
            return VX_FAILURE;
        }
        status = appStartImageSensor(sensorObj->sensor_name, ((1 << sensorObj->num_cameras_enabled) - 1));
    }

    for(frame_id = 0; frame_id < obj->num_frames_to_run; frame_id++)
    {
#ifdef APP_WRITE_INTERMEDIATE_OUTPUTS
        if(obj->write_file == 1)
        {
            if(obj->captureObj.en_out_capture_write == 1)
            {
                app_send_cmd_capture_write_node(&obj->captureObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if(obj->vissObj.en_out_viss_write == 1)
            {
                app_send_cmd_viss_write_node(&obj->vissObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if(obj->ldcObj.en_out_ldc_write == 1)
            {
                app_send_cmd_ldc_write_node(&obj->ldcObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            if(obj->scalerObj.en_out_scaler_write == 1)
            {
                app_send_cmd_scaler_write_node(&obj->scalerObj, frame_id, obj->num_frames_to_write, obj->num_frames_to_skip);
            }
            obj->write_file = 0;
        }
#endif
        if(obj->pipeline_exec == 1)
        {
            app_run_graph_for_one_frame_pipeline(obj, frame_id);
        }
        else
        {
            app_run_graph_for_one_frame_sequential(obj, frame_id);
        }

        /* user asked to stop processing */
        if(obj->stop_task)
          break;
    }
    if(obj->pipeline_exec == 1)
    {
        vxWaitGraph(obj->graph);
    }

    obj->stop_task = 1;

    if(obj->enable_capture == 1)
    {
        status = appStopImageSensor(obj->sensorObj.sensor_name, ((1 << sensorObj->num_cameras_enabled) - 1));
    }

    return status;
}

static void set_display_defaults(DisplayObj *displayObj)
{
    displayObj->display_option = 1;
}

static void set_img_hist_defaults(ImgHistObj *imgHistObj)
{
    imgHistObj->num_bins = 256 * 256;
    imgHistObj->offset = 0;
    imgHistObj->range = 256 * 256;
}

static void app_pipeline_params_defaults(AppObj *obj)
{
    obj->pipeline       = -APP_BUFFER_Q_DEPTH + 1;
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

    sensorObj->enable_ldc = 1;
    sensorObj->num_cameras_enabled = 4;
    sensorObj->usecase_option = APP_SENSOR_FEATURE_CFG_UC0;
    sensorObj->is_interactive = 1;

}

static void set_dof_pyramid_defaults(PyramidObj *pyramidObj)
{
    pyramidObj->width  = 768;
    pyramidObj->height = 384;
    pyramidObj->dof_levels = 2;
    pyramidObj->vx_df_pyramid = VX_DF_IMAGE_U8;
}


static void set_dof_proc_defaults(DofProcObj *dofProcObj)
{
    dofProcObj->enable_temporal_predicton_flow_vector = 0;
    dofProcObj->width  = 768;
    dofProcObj->height = 384;
    dofProcObj->dof_levels = 2;
    dofProcObj->vx_df_pyramid = VX_DF_IMAGE_U8;
}

static void set_pre_proc_defaults(PreProcObj *preProcObj)
{
    vx_int32 i;
    for(i = 0; i< 3 ; i++)
    {
        preProcObj->params.scale_val[i] = 1.0;
        preProcObj->params.mean_pixel[i] = 0.0;
    }

    preProcObj->params.ip_rgb_or_yuv = 1;
    preProcObj->params.color_conv_flag = TIADALG_COLOR_CONV_YUV420_BGR;
}

static void set_post_proc_defaults(PostProcObj *postProcObj)
{
    int32_t i;

    postProcObj->viz_params.ip_rgb_or_yuv = 1;
    postProcObj->viz_params.op_rgb_or_yuv = 1;

    postProcObj->viz_params.num_input_tensors  = 1;
    postProcObj->viz_params.num_output_tensors = 1;

    postProcObj->viz_params.tidl_8bit_16bit_flag = 0;

    postProcObj->viz_params.num_classes[0] = 5; //5 classes for semantic segmentation
    postProcObj->viz_params.num_classes[1] = 0;
    postProcObj->viz_params.num_classes[2] = 0;

    for(i=0; i < TIVX_PIXEL_VIZ_MAX_TENSOR; i++)
    {
        postProcObj->viz_params.valid_region[i][0] = 0;
        postProcObj->viz_params.valid_region[i][1] = 0;
        postProcObj->viz_params.valid_region[i][2] = 0;
        postProcObj->viz_params.valid_region[i][3] = 0;
    }
}

static void update_post_proc_params(AppObj *obj, PostProcObj *postProcObj)
{
    vx_int32 i;

    postProcObj->out_width  = obj->scalerObj.output[0].width;
    postProcObj->out_height = obj->scalerObj.output[0].height;

    memcpy(postProcObj->viz_params.color_map[0], color_map, postProcObj->viz_params.num_classes[0]*3*sizeof(vx_uint8));
    postProcObj->viz_params.max_value[0] = postProcObj->viz_params.num_classes[0] - 1;

    for(i=0; i < TIVX_PIXEL_VIZ_MAX_TENSOR; i++)
    {
        postProcObj->viz_params.valid_region[i][0] = 0;
        postProcObj->viz_params.valid_region[i][1] = 0;
        postProcObj->viz_params.valid_region[i][2] = obj->scalerObj.output[0].width  - 1;
        postProcObj->viz_params.valid_region[i][3] = obj->scalerObj.output[0].height - 1;
    }
}

static void set_scaler_defaults(ScalerObj *scalerObj)
{

}

static void app_default_param_set(AppObj *obj)
{
    set_sensor_defaults(&obj->sensorObj);

    set_scaler_defaults(&obj->scalerObj);

    set_dof_pyramid_defaults(&obj->pyramidObj);

    set_dof_proc_defaults(&obj->dofProcObj);

    set_display_defaults(&obj->displayObj);

    set_pre_proc_defaults(&obj->preProcObj);

    set_post_proc_defaults(&obj->postProcObj);

    set_img_hist_defaults(&obj->imgHistObj);

    app_pipeline_params_defaults(obj);

    obj->captureObj.enable_error_detection = 1; /* enabled by default */
    obj->is_interactive = 1;
    obj->write_file = 0;
    obj->num_frames_to_run = 1000000000;

    obj->enable_capture   = 1;
    obj->enable_viss      = 1;
    obj->enable_aewb      = 1;
    obj->enable_ldc       = 1;
    obj->enable_scaler    = 1;
    obj->enable_pre_proc  = 1;
    obj->enable_tidl      = 1;
    obj->enable_post_proc = 1;
    obj->enable_mosaic    = 1;
    obj->enable_display   = 1;
    obj->enable_srv       = 1;
    obj->enable_hist      = 1;
    obj->enable_dof       = 1;

    obj->pipeline_exec    = 1;

}

static void set_img_mosaic_params(AppObj *obj, ImgMosaicObj *imgMosaicObj)
{
    vx_int32 idx;

    imgMosaicObj->out_width    = DISPLAY_WIDTH;
    imgMosaicObj->out_height   = DISPLAY_HEIGHT;
    imgMosaicObj->num_inputs   = 1;

    tivxImgMosaicParamsSetDefaults(&imgMosaicObj->params);

    idx = 0;

    imgMosaicObj->params.windows[idx].startX  = 190;
    imgMosaicObj->params.windows[idx].startY  = 110;
    imgMosaicObj->params.windows[idx].width   = obj->postProcObj.in_width;
    imgMosaicObj->params.windows[idx].height  = obj->postProcObj.in_height;
    imgMosaicObj->params.windows[idx].input_select   = 0;
    imgMosaicObj->params.windows[idx].channel_select = 0;
    idx++;

    imgMosaicObj->params.windows[idx].startX  = 190 + 770;
    imgMosaicObj->params.windows[idx].startY  = 110;
    imgMosaicObj->params.windows[idx].width   = obj->postProcObj.in_width;
    imgMosaicObj->params.windows[idx].height  = obj->postProcObj.in_height;
    imgMosaicObj->params.windows[idx].input_select   = 0;
    imgMosaicObj->params.windows[idx].channel_select = 1;
    idx++;

    imgMosaicObj->params.windows[idx].startX  = 190;
    imgMosaicObj->params.windows[idx].startY  = 110 + 386;
    imgMosaicObj->params.windows[idx].width   = obj->postProcObj.in_width;
    imgMosaicObj->params.windows[idx].height  = obj->postProcObj.in_height;
    imgMosaicObj->params.windows[idx].input_select   = 0;
    imgMosaicObj->params.windows[idx].channel_select = 2;
    idx++;

    imgMosaicObj->params.windows[idx].startX  = 190 + 770;
    imgMosaicObj->params.windows[idx].startY  = 110 + 386;
    imgMosaicObj->params.windows[idx].width   = obj->postProcObj.in_width;
    imgMosaicObj->params.windows[idx].height  = obj->postProcObj.in_height;
    imgMosaicObj->params.windows[idx].input_select   = 0;
    imgMosaicObj->params.windows[idx].channel_select = 3;
    idx++;

    imgMosaicObj->params.num_windows  = idx;

    /* Number of time to clear the output buffer before it gets reused */
    imgMosaicObj->params.clear_count  = APP_BUFFER_Q_DEPTH;
    imgMosaicObj->params.num_msc_instances = 1;
    imgMosaicObj->params.msc_instance = 1; /* Use MSC1 instance as MSC0 is used for scaler */
}

static void app_update_param_set(AppObj *obj)
{
    obj->sensorObj.sensor_index = 0; /* App works only for IMX390 2MP cameras */

    printf("Sensor width   = %d\n", obj->sensorObj.image_width);
    printf("Sensor height  = %d\n", obj->sensorObj.image_height);

    if(obj->enable_post_proc == 1)
    {
        update_post_proc_params(obj, &obj->postProcObj);
    }

    if(obj->enable_mosaic == 1)
    {
        set_img_mosaic_params(obj, &obj->imgMosaicObj);
    }
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

#ifndef x86_64
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type)
{
    appGrpxDrawDefault(handle, draw2dBufInfo, update_type);

    if(update_type == 0)
    {
            Draw2D_FontPrm sHeading;

            sHeading.fontIdx = 0;
            Draw2D_drawString(handle, 560, 5, "Analytics for Auto Valet Parking", &sHeading);
    }

    return;
}
#endif
