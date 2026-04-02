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
#ifndef _APP_SRV_MODULE
#define _APP_SRV_MODULE

#include "app_common.h"
#include "app_sensor_module.h"
#include <TI/tivx_srv.h>
#include "srv_bowl_lut_gen_applib/srv_bowl_lut_gen_applib.h"
#include "lens_distortion_correction.h"
#include "srv_common.h"
#include <render.h>
#define SV_OUT_DISPLAY_HEIGHT   (1080)
#define SV_OUT_DISPLAY_WIDTH    (1080)
#define SV_SUBSAMPLE            (4)
#define SV_XYZLUT3D_SIZE        (SV_OUT_DISPLAY_HEIGHT/SV_SUBSAMPLE) * (SV_OUT_DISPLAY_WIDTH/SV_SUBSAMPLE) *3
#define SV_GPULUT_SIZE          ((2 + SV_OUT_DISPLAY_WIDTH/SV_SUBSAMPLE) * (2 + SV_OUT_DISPLAY_HEIGHT/SV_SUBSAMPLE)) *7

typedef struct {
    vx_uint32 inWidth;
    vx_uint32 inHeight;
    vx_uint32 outWidth;
    vx_uint32 outHeight;
    uint32_t num_views;

    int32_t offsetXleft;
    int32_t offsetXright;
    int32_t offsetYfront;
    int32_t offsetYback;

    float camx[MAX_SRV_VIEWS];
    float camy[MAX_SRV_VIEWS];
    float camz[MAX_SRV_VIEWS];
    float targetx[MAX_SRV_VIEWS];
    float targety[MAX_SRV_VIEWS];
    float targetz[MAX_SRV_VIEWS];
    float anglex[MAX_SRV_VIEWS];
    float angley[MAX_SRV_VIEWS];
    float anglez[MAX_SRV_VIEWS];

} SRVParams;

typedef struct {
    vx_node node;
    vx_graph graph_gpu_lut;

    /* GPU LUT Applib API objects */
    vx_user_data_object         in_config;
    vx_user_data_object         in_calmat_object;
    vx_user_data_object         in_offset_object;
    vx_user_data_object         in_lens_param_object;
    vx_array                    out_gpulut_array;

    /* GPU LUT Applib params */
    srv_bowl_lut_gen_handle srv_handle;
    srv_bowl_lut_gen_createParams create_params;

    /* OpenGL SRV node objects */
    vx_object_array srv_views_array;
    vx_image output_img;
    vx_user_data_object param_obj;

    SRVParams params;

    /* Default input */
    vx_object_array input_img_arr;
    
    vx_int32 graph_parameter_index;

    /* These params are needed only for writing intermediate output */
    vx_int32 en_out_srv_write;
    vx_node write_node;
    vx_array file_path;
    vx_array file_prefix;
    vx_user_data_object write_cmd;

    vx_char output_file_path[TIVX_FILEIO_FILE_PATH_LENGTH];

    vx_char objName[APP_MAX_FILE_PATH];

} SRVObj;

vx_status app_init_srv(vx_context context, SRVObj *srvObj, SensorObj *sensorObj, char *objName);
void app_deinit_srv(SRVObj *srvObj);
void app_delete_srv(SRVObj *srvObj);
vx_status app_create_graph_srv(vx_context context, vx_graph graph, SRVObj *srvObj, vx_object_array input_arr);

vx_status app_run_graph_gpu_lut(SRVObj *srvObj);

#endif
