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

#include <TI/tivx.h>
#include <TI/tivx_task.h>
#include <TI/tivx_stereo.h>
#include <tivx_utils_file_rd_wr.h>
#include <tivx_utils_graph_perf.h>
#include <TI/tivx_img_proc.h>
#include <TI/hwa_dmpac_sde.h>
#include <TI/video_io_display.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#if defined(LINUX) || defined(QNX)
#include <sys/stat.h>
#endif

#if defined(PC)
#include <perception/perception.h>
#endif

#include <utils/perf_stats/include/app_perf_stats.h>
#include <utils/console_io/include/app_get.h>
#include <utils/draw2d/include/draw2d.h>
#include <utils/grpx/include/app_grpx.h>
#include <VX/vx_khr_pipelining.h>

//#define APP_DEBUG
#ifdef APP_DEBUG
#define APP_PRINTF(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define APP_PRINTF(f_, ...)
#endif

#define APP_MAX_FILE_PATH           (256u)
#define APP_ASSERT(x)               assert((x))
#define APP_ASSERT_VALID_REF(ref)   (APP_ASSERT(vxGetStatus((vx_reference)(ref))==VX_SUCCESS));

#define SDE_FILE_HEADER_LEN   80

#define HIST_IMG_WIDTH        1280
#define HIST_IMG_HEIGHT       960

#define OUTPUT_DISPLAY_WIDTH  1280/2
#define OUTPUT_DISPLAY_HEIGHT 960/2

#define INPUT_DISPLAY_WIDTH   1000
#define INPUT_DISPLAY_HEIGHT  960/4

#define MAX_NUM_BUF          (3u)

typedef struct {

    /* config options */
    char left_input_file_path[APP_MAX_FILE_PATH];
    char right_input_file_path[APP_MAX_FILE_PATH];
    char output_file_path[APP_MAX_FILE_PATH];
    char output_file_prefix[APP_MAX_FILE_PATH];
    char left_input_file_name[APP_MAX_FILE_PATH];
    char right_input_file_name[APP_MAX_FILE_PATH];
    int32_t start_fileno;
    int32_t end_fileno;
    uint32_t width;
    uint32_t height;
    uint8_t  vis_confidence;
    uint32_t mosaic_out_width;
    uint32_t mosaic_out_height;
    uint8_t  hist_output;
    uint8_t  sde_output;

    /* OpenVX references */
    vx_context context;
    vx_graph graph_sde;
    vx_node  node_sde;
    vx_node  node_sde_disparity_vis;
    vx_node  node_sde_histogram_vis;
    vx_image input_img_left[MAX_NUM_BUF];
    vx_image input_img_right[MAX_NUM_BUF];
    vx_object_array input_arr_left[MAX_NUM_BUF];
    vx_object_array input_arr_right[MAX_NUM_BUF];
    vx_object_array input_arr[2];

    vx_node mosaic_node;
    vx_image mosaic_output_image[MAX_NUM_BUF];
    vx_kernel mosaic_kernel;

    vx_image disparity;
    vx_distribution histogram;
    vx_image disparity_img[MAX_NUM_BUF];
    vx_image histogram_img[MAX_NUM_BUF];

    vx_scalar vis_confidence_threshold;

    vx_user_data_object mosaic_config;
    tivxImgMosaicParams mosaic_params;
    vx_user_data_object sde_config;
    vx_user_data_object sde_vis_config;
    tivx_dmpac_sde_params_t sde_params;
    tivx_sde_disparity_vis_params_t sde_vis_params;

    vx_node node_output_display;
    vx_user_data_object output_display_config;
    tivx_display_params_t output_display_params;
    vx_node node_input_display;
    vx_user_data_object input_display_config;
    tivx_display_params_t input_display_params;
    vx_uint32 display_option;
    vx_uint32 pipeline_option;
    vx_scalar confidence_threshold;

    vx_int32 input_arr_left_graph_parameter_index;
    vx_int32 input_arr_right_graph_parameter_index;
    vx_int32 disparity_output_parameter_index;
    vx_int32 mosaic_output_parameter_index;

    uint32_t num_buf;

    int32_t pipeline;
    int32_t enqueueCnt;

    vx_uint32 num_iterations;
    uint32_t is_interactive;
    uint32_t test_mode;
    uint8_t  bit_depth;
    tivx_task task;
    volatile uint32_t stop_task;
    volatile uint32_t stop_task_done;

    app_perf_point_t total_perf;
    app_perf_point_t fileio_perf;
    app_perf_point_t draw_perf;

} AppObj;

vx_status app_stereo_main(int argc, char* argv[]);
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

static vx_status app_save_sde_output(char* filename, vx_image disparity, AppObj * obj, vx_uint32 curFileNum);

#ifndef x86_64
#ifndef SOC_J722S
static void app_draw_graphics(Draw2D_Handle *handle, Draw2D_BufInfo *draw2dBufInfo, uint32_t update_type);
#endif
#endif
static vx_status add_graph_parameter_by_node_index(vx_graph graph, vx_node node,vx_uint32 node_parameter_index);
static void app_find_object_array_index(vx_object_array object_array[], vx_reference ref, vx_int32 array_size, vx_int32 *array_idx);
static vx_status app_run_graph_for_one_frame_pipeline(AppObj *obj, vx_int32 curFileNum);
static vx_status app_run_graph_for_one_frame_sequential(AppObj *obj, vx_int32 frame_id);
