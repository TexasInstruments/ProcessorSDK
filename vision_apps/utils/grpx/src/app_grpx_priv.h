/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
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

#ifndef APP_GRPX_PRIV_H
#define APP_GRPX_PRIV_H

#include <utils/grpx/include/app_grpx.h>
#include <utils/ipc/include/app_ipc.h>
#include <utils/perf_stats/include/app_perf_stats.h>
#include <stdio.h>
#include <TI/tivx_task.h>

#define APP_GRPX_STRING_MAX (32u)


#define APP_GRPX_LOAD_BAR_HEIGHT      (100u)
#define APP_GRPX_LOAD_PAD_Y           (5u)
#define APP_GRPX_LOAD_BAR_NUM_CHAR    (4u)

#if defined(SOC_J784S4) || defined(SOC_J742S2)
#define APP_GRPX_LOAD_BAR_WIDTH_FACTOR    (6u)
#else
#define APP_GRPX_LOAD_BAR_WIDTH_FACTOR    (8u)
#endif

typedef struct
{
    app_grpx_init_prms_t prm;
    vx_graph disp_graph;
    vx_node disp_node;
    vx_user_data_object disp_config;
    vx_image disp_image;
    Draw2D_Handle draw2d_obj;
    Draw2D_BufInfo draw2d_buf_info;
    uint16_t is_task_created;
    tivx_task task;
    uint16_t task_stop;
    uint16_t task_stop_done;
    uint32_t logo_bmp_idx;
    uint8_t  cpu_load[APP_IPC_CPU_MAX]; /* 0..100 */
    uint8_t  hwa_load[APP_PERF_HWA_MAX]; /* 0..100 */
    uint16_t hwa_perf[APP_PERF_HWA_MAX]; /* in MP/s */
    uint32_t read_bw_avg; /* in MB/s */
    uint32_t write_bw_avg; /* in MB/s */
    uint32_t total_available_bw; /* in MB/s */
} app_grpx_obj_t;

extern app_grpx_obj_t g_app_grpx_obj;

void appGrpxDrawLoadBar(
                    app_grpx_obj_t *obj,
                    uint32_t cpuLoad, /* 0 .. 100 */
                    uint32_t startX,
                    uint32_t startY,
                    uint32_t width,
                    uint32_t height
                );

void appGrpxDrawLoadBar2(
                    app_grpx_obj_t *obj,
                    uint32_t load1, /* 0 .. 100 */
                    uint32_t load2, /* 0 .. 100 */
                    uint32_t startX,
                    uint32_t startY,
                    uint32_t width,
                    uint32_t height,
                    char *label1,
                    char *label2
                );


#endif
