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

/* NOTE: NOT SUPPORTED for x86, hence all functions are blank */

#include <stdint.h>
#include <utils/perf_stats/include/app_perf_stats.h>

int32_t appPerfStatsCpuLoadReset(uint32_t app_cpu_id)
{
    return 0;
}

int32_t appPerfStatsCpuLoadGet(uint32_t app_cpu_id, app_perf_stats_cpu_load_t *cpu_load)
{
    return 0;
}

int32_t appPerfStatsCpuTaskStatsGet(uint32_t app_cpu_id, app_perf_stats_task_stats_t *cpu_stats)
{
    return 0;
}

int32_t appPerfStatsCpuMemStatsGet(uint32_t app_cpu_id, app_perf_stats_mem_stats_t *cpu_stats)
{
    return 0;
}

int32_t appPerfStatsCpuLoadPrintAll()
{
    return 0;
}

int32_t appPerfStatsCpuStatsPrintAll()
{
    return 0;
}

int32_t appPerfStatsCpuLoadResetAll()
{
    return 0;
}

int32_t appPerfStatsPrintAll()
{
    return 0;
}

int32_t appPerfStatsCpuTaskStatsPrint(uint32_t app_cpu_id, app_perf_stats_task_stats_t *cpu_stats)
{
    return 0;
}

int32_t appPerfStatsCpuMemStatsPrint(uint32_t app_cpu_id, app_perf_stats_mem_stats_t *cpu_stats)
{
    return 0;
}

int32_t appPerfStatsCpuLoadPrint(uint32_t app_cpu_id, app_perf_stats_cpu_load_t *cpu_load)
{
    return 0;
}

int32_t appPerfStatsRegisterTask(void *task_handle, const char *name)
{
    return 0;
}

void appPerfPointSetName(app_perf_point_t *prm, const char *name)
{
}

void appPerfPointReset(app_perf_point_t *prm)
{
}

void appPerfPointBegin(app_perf_point_t *prm)
{
}


void appPerfPointEnd(app_perf_point_t *prm)
{
}

void appPerfPointGet(app_perf_point_t *prm, app_perf_point_t *perf)
{
}

void appPerfPointPrint(app_perf_point_t *prm)
{
}

void appPerfPointPrintFPS(app_perf_point_t *prm)
{
}

int32_t appPerfStatsExportAll(FILE *fp, app_perf_point_t *perf_points[], uint32_t num_points)
{
    return 0;
}

FILE *appPerfStatsExportOpenFile(const char *output_file_path, const char *output_file_prefix)
{
    return NULL;
}

int32_t appPerfStatsExportCloseFile(FILE *fp)
{
    return 0;
}

void appPerfStatsResetAll()
{
}

void appPerfStatsHwaUpdateLoad(app_perf_hwa_id_t id, uint32_t active_time_in_usecs, uint32_t pixels_processed)
{
}
