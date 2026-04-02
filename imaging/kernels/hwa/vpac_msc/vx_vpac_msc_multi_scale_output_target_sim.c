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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "TI/tivx.h"
#include "TI/hwa_vpac_msc.h"
#include <tivx_openvx_core_kernels.h>
#include <tivx_kernel_scale.h>
#include "TI/tivx_target_kernel.h"
#include "tivx_kernels_target_utils.h"
#include "vx_kernels_hwa_target.h"
#include "scaler_core.h"
#include "tivx_kernel_vpac_msc.h"
#include "tivx_hwa_vpac_msc_priv.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    uint16_t *src16_0, *src16_1;
    uint16_t *dst16[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t buffer_size_in0, buffer_size_in1;
    uint32_t buffer_size_out[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];

    /* To support NV12 format */
    uint16_t *src16_cbcr_0, *src16_cbcr_1;
    uint16_t *dst16_cbcr[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t buffer_size_in_cbcr_0, buffer_size_in_cbcr_1;
    uint32_t buffer_size_out_cbcr[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];

    msc_config config;

    uint32_t crop_width[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t crop_height[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];

    /* State from user commands to override auto mode or not */
    uint32_t user_init_phase_x[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t user_init_phase_y[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t user_offset_x[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t user_offset_y[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t user_crop_start_x[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];
    uint32_t user_crop_start_y[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS];

    vx_df_image             in_img_format;
    vx_df_image              out_img_format;
    uint32_t                 in0_height,in1_height;
    tivx_obj_desc_image_t    *imgIn0, *imgIn1;
    uint32_t                in_img0_yc_mode;
    uint32_t                in_img1_yc_mode;

} tivxMscScaleParams;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


static vx_status VX_CALLBACK tivxKernelMscScaleProcess(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxKernelMscScaleCreate(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxKernelMscScaleDelete(
       tivx_target_kernel_instance kernel,
       tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);
static vx_status VX_CALLBACK tivxKernelMscScaleControl(
       tivx_target_kernel_instance kernel,
       uint32_t node_cmd_id, tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg);

static void tivxVpacMscScaleFreeMem(tivxMscScaleParams *prms);
static void tivxVpacMscScaleInitParams(Scaler_Config *settings);

static vx_status tivxVpacMscScaleSetCoeffsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj);
static vx_status tivxVpacMscScaleSetInputParamsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj);
static vx_status tivxVpacMscScaleSetOutputParamsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj[], uint16_t num_params);
static vx_status tivxVpacMscScaleSetCropParamsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj[], uint16_t num_params);
static vx_status tivxVpacMscScaleUpdateOutputSettings(tivxMscScaleParams *prms,
    uint32_t ow, uint32_t oh, uint32_t cnt, uint32_t h_divider, vx_df_image format);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Given that for J7AHP, there are multiple VPAC's, there needs to be separate
 * target kernels in the PC emulation mode kernel file given how this is
 * registered */
static tivx_target_kernel vx_vpac_msc_multi_scale_target_kernel[4] = {NULL};
static tivx_target_kernel vx_vpac_msc_multi_scale2_target_kernel[4] = {NULL};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void tivxAddTargetKernelVpacMscMultiScale(void)
{
    char target_name[TIVX_TARGET_MAX_NAME];
    vx_enum self_cpu;

    self_cpu = tivxGetSelfCpuId();

    if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC1)
    {
        strncpy(target_name, TIVX_TARGET_VPAC_MSC1, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale_target_kernel[0] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);

        strncpy(target_name, TIVX_TARGET_VPAC_MSC2, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale_target_kernel[1] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);
    }
    #if (VPAC_COUNT > 1)
    else if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC2)
    {
        strncpy(target_name, TIVX_TARGET_VPAC2_MSC1, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale_target_kernel[2] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);

        strncpy(target_name, TIVX_TARGET_VPAC2_MSC2, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale_target_kernel[3] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);
    }
    #endif
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid CPU ID\n");
    }
}


void tivxRemoveTargetKernelVpacMscMultiScale(void)
{
    vx_enum self_cpu;

    self_cpu = tivxGetSelfCpuId();

    if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC1)
    {
        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale_target_kernel[0]);
        vx_vpac_msc_multi_scale_target_kernel[0] = NULL;

        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale_target_kernel[1]);
        vx_vpac_msc_multi_scale_target_kernel[1] = NULL;
    }
    #if (VPAC_COUNT > 1)
    else if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC2)
    {
        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale_target_kernel[2]);
        vx_vpac_msc_multi_scale_target_kernel[2] = NULL;

        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale_target_kernel[3]);
        vx_vpac_msc_multi_scale_target_kernel[3] = NULL;
    }
    #endif
}

void tivxAddTargetKernelVpacMscMultiOut(void)
{
    char target_name[TIVX_TARGET_MAX_NAME];
    vx_enum self_cpu;

    self_cpu = tivxGetSelfCpuId();

    if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC1)
    {
        strncpy(target_name, TIVX_TARGET_VPAC_MSC1, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale2_target_kernel[0] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE2_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);

        strncpy(target_name, TIVX_TARGET_VPAC_MSC2, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale2_target_kernel[1] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE2_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);
    }
    #if (VPAC_COUNT > 1)
    else if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC2)
    {
        strncpy(target_name, TIVX_TARGET_VPAC2_MSC1, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale2_target_kernel[2] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE2_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);

        strncpy(target_name, TIVX_TARGET_VPAC2_MSC2, TIVX_TARGET_MAX_NAME);
        vx_vpac_msc_multi_scale2_target_kernel[3] = tivxAddTargetKernelByName(
            TIVX_KERNEL_VPAC_MSC_MULTI_SCALE2_NAME,
            target_name,
            tivxKernelMscScaleProcess,
            tivxKernelMscScaleCreate,
            tivxKernelMscScaleDelete,
            tivxKernelMscScaleControl,
            NULL);
    }
    #endif
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid CPU ID\n");
    }
}


void tivxRemoveTargetKernelVpacMscMultiOut(void)
{
    vx_enum self_cpu;

    self_cpu = tivxGetSelfCpuId();

    if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC1)
    {
        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale2_target_kernel[0]);
        vx_vpac_msc_multi_scale2_target_kernel[0] = NULL;

        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale2_target_kernel[1]);
        vx_vpac_msc_multi_scale2_target_kernel[1] = NULL;
    }
    #if (VPAC_COUNT > 1)
    else if (self_cpu == (vx_enum)TIVX_CPU_ID_VPAC2)
    {
        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale2_target_kernel[2]);
        vx_vpac_msc_multi_scale2_target_kernel[2] = NULL;

        tivxRemoveTargetKernel(vx_vpac_msc_multi_scale2_target_kernel[3]);
        vx_vpac_msc_multi_scale2_target_kernel[3] = NULL;
    }
    #endif
}

/* ========================================================================== */
/*                              OPENVX Callbacks                              */
/* ========================================================================== */

static vx_status VX_CALLBACK tivxKernelMscScaleCreate(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg)
{
    uint32_t cnt;
    vx_status status = (vx_status)VX_SUCCESS;
    tivx_obj_desc_image_t *imgIn0 = NULL, *imgIn1 = NULL, *imgOut[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT] = {NULL};
    tivxMscScaleParams *prms = NULL;

    if (((num_params != TIVX_KERNEL_VPAC_MSC_SCALE_MAX_PARAMS) ||
        ((NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_IN0_IMG_IDX]) ||
        (NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_OUT0_IMG_IDX]))) &&
        ((num_params != TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS) ||
        ((NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_IN0_IMG_IDX]) ||
        (NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_OUT0_IMG_IDX]))))
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Parameters\n");
        status = (vx_status)VX_FAILURE;
    }

    if ((vx_status)VX_SUCCESS == status)
    {
        imgIn0 = (tivx_obj_desc_image_t *)
            obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_IN0_IMG_IDX];
        if(num_params == TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS)
        {
            imgIn0 = (tivx_obj_desc_image_t *)
                obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_IN0_IMG_IDX];
            imgIn1 = (tivx_obj_desc_image_t *)
                obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_IN1_IMG_IDX];
        }

        if(NULL == imgIn1)
        {
            for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE_MAX_OUTPUT; cnt ++)
            {
                imgOut[cnt] = (tivx_obj_desc_image_t *)
                        obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_OUT0_IMG_IDX + cnt];
            }
        }
        else
        {
            for (cnt = 0u; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT/2; cnt++)
            {
                imgOut[2*cnt] = (tivx_obj_desc_image_t *)
                    obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_OUT0_IMG_IDX + cnt];
                imgOut[2*cnt+1] = (tivx_obj_desc_image_t *)
                    obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_OUT5_IMG_IDX + cnt];
            }
        }
        prms = tivxMemAlloc(sizeof(tivxMscScaleParams), (vx_enum)TIVX_MEM_EXTERNAL);
        if (NULL == prms)
        {
            VX_PRINT(VX_ZONE_ERROR, "Params allocation error\n");
            status = (vx_status)VX_ERROR_NO_MEMORY;
        }
        else
        {
            memset(prms, 0x0, sizeof(tivxMscScaleParams));
        }

        prms->in_img_format = imgIn0->format;
        prms->out_img_format = imgOut[0]->format;
        prms->in0_height = imgIn0->imagepatch_addr[0].dim_y;
        if(imgIn1 != NULL)
        {
            prms->in1_height = imgIn1->imagepatch_addr[0].dim_y;
        }
        prms->in_img0_yc_mode = TIVX_VPAC_MSC_MODE_LUMA_ONLY;
        prms->in_img1_yc_mode = TIVX_VPAC_MSC_MODE_LUMA_ONLY;
        prms->imgIn0 = imgIn0;
        prms->imgIn1 = imgIn1;
        if ((vx_status)VX_SUCCESS == status)
        {
            prms->buffer_size_in0 = imgIn0->imagepatch_addr[0].dim_x *
                                   imgIn0->imagepatch_addr[0].dim_y * 2;

            prms->src16_0 = tivxMemAlloc(prms->buffer_size_in0, (vx_enum)TIVX_MEM_EXTERNAL);
            if (NULL == prms->src16_0)
            {
                VX_PRINT(VX_ZONE_ERROR, "Input Buffer Alloc Error\n");
                status = (vx_status)VX_ERROR_NO_MEMORY;
            }
            if(imgIn1 != NULL)
            {
                prms->buffer_size_in1 = imgIn1->imagepatch_addr[0].dim_x *
                                    imgIn1->imagepatch_addr[0].dim_y * 2;
                
                prms->src16_1 = tivxMemAlloc(prms->buffer_size_in1, (vx_enum)TIVX_MEM_EXTERNAL);
                if (NULL == prms->src16_1)
                {
                    VX_PRINT(VX_ZONE_ERROR, "Input Buffer Alloc Error\n");
                    status = (vx_status)VX_ERROR_NO_MEMORY;
                }
            }
            if(((vx_status)VX_SUCCESS == status) && (imgIn0->format == (vx_df_image)VX_DF_IMAGE_NV12))
            {
                prms->buffer_size_in_cbcr_0 = imgIn0->imagepatch_addr[1].dim_x *
                                            (imgIn0->imagepatch_addr[1].dim_y / imgIn0->imagepatch_addr[1].step_y) * 2;

                prms->src16_cbcr_0 = tivxMemAlloc(prms->buffer_size_in_cbcr_0, (vx_enum)TIVX_MEM_EXTERNAL);
                if (NULL == prms->src16_cbcr_0)
                {
                    VX_PRINT(VX_ZONE_ERROR, "Input Buffer Alloc Error\n");
                    status = (vx_status)VX_ERROR_NO_MEMORY;
                }
            }
            if(((vx_status)VX_SUCCESS == status) && ((imgIn0->format == (vx_df_image)VX_DF_IMAGE_UYVY) || (imgIn0->format == (vx_df_image)VX_DF_IMAGE_YUYV)))
            {
                prms->buffer_size_in_cbcr_0 = imgIn0->imagepatch_addr[0].dim_x *
                                            imgIn0->imagepatch_addr[0].dim_y * 2;

                prms->src16_cbcr_0 = tivxMemAlloc(prms->buffer_size_in_cbcr_0, (vx_enum)TIVX_MEM_EXTERNAL);
                if (NULL == prms->src16_cbcr_0)
                {
                    VX_PRINT(VX_ZONE_ERROR, "Input Buffer Alloc Error\n");
                    status = (vx_status)VX_ERROR_NO_MEMORY;
                }
            }
        }

        if ((vx_status)VX_SUCCESS == status)
        {
            for (cnt = 0u; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
            {
                if (NULL != imgOut[cnt])
                {
                    prms->user_init_phase_x[cnt] = TIVX_VPAC_MSC_AUTOCOMPUTE;
                    prms->user_init_phase_y[cnt] = TIVX_VPAC_MSC_AUTOCOMPUTE;
                    prms->user_offset_x[cnt] =     TIVX_VPAC_MSC_AUTOCOMPUTE;
                    prms->user_offset_y[cnt] =     TIVX_VPAC_MSC_AUTOCOMPUTE;
                    prms->user_crop_start_x[cnt] = 0;
                    prms->user_crop_start_y[cnt] = 0;

                    prms->crop_width[cnt] = imgIn0->imagepatch_addr[0].dim_x;
                    prms->crop_height[cnt] = imgIn0->imagepatch_addr[0].dim_y;

                    prms->buffer_size_out[cnt] =
                        imgOut[cnt]->imagepatch_addr[0].dim_x *
                        imgOut[cnt]->imagepatch_addr[0].dim_y * 2;

                    prms->dst16[cnt] = tivxMemAlloc(prms->buffer_size_out[cnt],
                        (vx_enum)TIVX_MEM_EXTERNAL);
                    if (NULL == prms->dst16[cnt])
                    {
                        VX_PRINT(VX_ZONE_ERROR, "Output%d Buffer Alloc Error\n",
                            cnt);
                        status = (vx_status)VX_ERROR_NO_MEMORY;
                    }

                    if(((vx_status)VX_SUCCESS == status) && (imgOut[cnt]->format == (vx_df_image)VX_DF_IMAGE_NV12))
                    {
                        prms->buffer_size_out_cbcr[cnt] =
                            imgOut[cnt]->imagepatch_addr[1].dim_x *
                            (imgOut[cnt]->imagepatch_addr[1].dim_y / imgOut[cnt]->imagepatch_addr[1].step_y) * 2;

                        prms->dst16_cbcr[cnt] = tivxMemAlloc(prms->buffer_size_out_cbcr[cnt],
                            (vx_enum)TIVX_MEM_EXTERNAL);
                        if (NULL == prms->dst16_cbcr[cnt])
                        {
                            VX_PRINT(VX_ZONE_ERROR, "Output%d Buffer Alloc Error\n",
                                cnt);
                            status = (vx_status)VX_ERROR_NO_MEMORY;
                        }
                    }
                    if(((vx_status)VX_SUCCESS == status) && ((imgOut[cnt]->format == (vx_df_image)VX_DF_IMAGE_UYVY) || (imgOut[cnt]->format == (vx_df_image)VX_DF_IMAGE_YUYV)))
                    {
                        prms->buffer_size_out_cbcr[cnt] =
                            imgOut[cnt]->imagepatch_addr[0].dim_x *
                            imgOut[cnt]->imagepatch_addr[0].dim_y * 2;

                        prms->dst16_cbcr[cnt] = tivxMemAlloc(prms->buffer_size_out_cbcr[cnt],
                            (vx_enum)TIVX_MEM_EXTERNAL);
                        if (NULL == prms->dst16_cbcr[cnt])
                        {
                            VX_PRINT(VX_ZONE_ERROR, "Output%d Buffer Alloc Error\n",
                                cnt);
                            status = (vx_status)VX_ERROR_NO_MEMORY;
                        }
                    }
                }
            }
        }

        if ((vx_status)VX_SUCCESS == status)
        {
            tivxVpacMscScaleInitParams(&prms->config.settings);

            tivxSetTargetKernelInstanceContext(kernel, prms,
                sizeof(tivxMscScaleParams));
        }
        else
        {
            tivxVpacMscScaleFreeMem(prms);
        }
    }

    return status;
}

static vx_status VX_CALLBACK tivxKernelMscScaleProcess(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg)
{
    uint32_t cnt;
    vx_status status = (vx_status)VX_SUCCESS;
    tivxMscScaleParams *prms = NULL;
    tivx_obj_desc_image_t *src0= NULL, *src1 = NULL;
    tivx_obj_desc_image_t *dst[TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT] = {NULL};
    uint32_t size;

    if (((num_params != TIVX_KERNEL_VPAC_MSC_SCALE_MAX_PARAMS) ||
        ((NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_IN0_IMG_IDX]) ||
        (NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_OUT0_IMG_IDX]))) &&
        ((num_params != TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS) ||
        ((NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_IN0_IMG_IDX]) ||
        (NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_OUT0_IMG_IDX]))))
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Descriptor\n");
        status = (vx_status)VX_FAILURE;
    }

    if ((vx_status)VX_SUCCESS == status)
    {
        src0 = (tivx_obj_desc_image_t *)obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_IN0_IMG_IDX];
        if(num_params == TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS)
        {
            src0 = (tivx_obj_desc_image_t *)
                obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_IN0_IMG_IDX];
            src1 = (tivx_obj_desc_image_t *)
                obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_IN1_IMG_IDX];
        }
        
        if(NULL == src1)
        {
            for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE_MAX_OUTPUT; cnt ++)
            {
                dst[cnt] = (tivx_obj_desc_image_t *)
                        obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_OUT0_IMG_IDX + cnt];
            }
        }
        else
        {
            for (cnt = 0u; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT/2; cnt++)
            {
                dst[2*cnt] = (tivx_obj_desc_image_t *)
                    obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_OUT0_IMG_IDX + cnt];
                dst[2*cnt+1] = (tivx_obj_desc_image_t *)
                    obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_OUT5_IMG_IDX + cnt];
            }
        }

        status = tivxGetTargetKernelInstanceContext(kernel,
            (void **)&prms, &size);

        if ((vx_status)VX_SUCCESS == status)
        {
            if ((NULL == prms) || (size != sizeof(tivxMscScaleParams)))
            {
                status = (vx_status)VX_FAILURE;
            }
        }
    }

    if ((vx_status)VX_SUCCESS == status)
    {
        void *src_target_ptr0;
        /* Cmodel doesn't support UYVY on same plane, first separate */
        if ((vx_df_image)VX_DF_IMAGE_UYVY == src0->format)
        {
            src_target_ptr0 = tivxMemShared2TargetPtr(&src0->mem_ptr[0]);
            tivxCheckStatus(&status, tivxMemBufferMap(src_target_ptr0,
                src0->mem_size[0], (vx_enum)VX_MEMORY_TYPE_HOST,
                (vx_enum)VX_READ_ONLY));

            lse_deinterleave_422(src0, src_target_ptr0, prms->src16_0, prms->src16_cbcr_0, 8);
        }
        else
        {
            src_target_ptr0 = tivxMemShared2TargetPtr(&src0->mem_ptr[0]);
            tivxCheckStatus(&status, tivxMemBufferMap(src_target_ptr0, src0->mem_size[0],
                (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

            /* C-model supports only 12-bit in uint16_t container
            * So we may need to translate.  In HW, VPAC_LSE does this
            */
            lse_reformat_in(src0, src_target_ptr0, prms->src16_0, 0, 0);

            tivxCheckStatus(&status, tivxMemBufferUnmap(src_target_ptr0, src0->mem_size[0],
                (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

            if(src0->format == (vx_df_image)VX_DF_IMAGE_NV12)
            {
                src_target_ptr0 = tivxMemShared2TargetPtr(&src0->mem_ptr[1]);
                tivxCheckStatus(&status, tivxMemBufferMap(src_target_ptr0, src0->mem_size[1],
                    (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

                /* C-model supports only 12-bit in uint16_t container
                * So we may need to translate.  In HW, VPAC_LSE does this
                */
                lse_reformat_in(src0, src_target_ptr0, prms->src16_cbcr_0, 1, 0);

                tivxCheckStatus(&status, tivxMemBufferUnmap(src_target_ptr0, src0->mem_size[1],
                    (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));
            }
            if(src1 != NULL)
            {
                void *src_target_ptr1;

                src_target_ptr1 = tivxMemShared2TargetPtr(&src1->mem_ptr[0]);
                tivxCheckStatus(&status, tivxMemBufferMap(src_target_ptr1, src1->mem_size[0],
                    (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

                /* C-model supports only 12-bit in uint16_t container
                * So we may need to translate.  In HW, VPAC_LSE does this
                */
                lse_reformat_in(src1, src_target_ptr1, prms->src16_1, 0, 0);

                tivxCheckStatus(&status, tivxMemBufferUnmap(src_target_ptr1, src1->mem_size[0],
                    (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));
            }
        }
    }

    if ((vx_status)VX_SUCCESS == status)
    {
        prms->config.settings.G_inWidth[0] = src0->imagepatch_addr[0].dim_x;
        prms->config.settings.G_inHeight[0] = src0->imagepatch_addr[0].dim_y;
        
        if((prms->in_img_format != (vx_df_image)VX_DF_IMAGE_NV12) || (prms->out_img_format != (vx_df_image)VX_DF_IMAGE_U8))
        {
            /* Is it enough to set for just 1 pipe in host-emulation mode? */
            prms->config.settings.unitParams[0].uvMode = false;
        }
        if((prms->out_img_format == (vx_df_image)VX_DF_IMAGE_U8)&&(prms->config.settings.unitParams[0].uvMode == true))
        {
            if(NULL == src1)
            {
                prms->config.settings.G_inHeight[0] = src0->imagepatch_addr[0].dim_y/2;
            }
        }

        for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
        {
            if ((NULL != dst[cnt]) && ((vx_status)VX_SUCCESS == status))
            {
                uint32_t ow = dst[cnt]->imagepatch_addr[0].dim_x;
                uint32_t oh = dst[cnt]->imagepatch_addr[0].dim_y;
                status = tivxVpacMscScaleUpdateOutputSettings(prms, ow, oh, cnt, 1, dst[cnt]->format);
            }
            else
            {
                break;
            }
        }
    }

    if ((vx_status)VX_SUCCESS == status)
    {

#ifdef VLAB_HWA

        prms->config.magic = 0xC0DEFACE;
        prms->config.buffer[0]  = prms->src16_0;
        for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
        {
            if (NULL != dst[cnt])
            {
                prms->config.buffer[4 + (cnt * 2)]  = prms->dst16[cnt];
            }
            else
            {
                break;
            }
        }

        status = vlab_hwa_process(VPAC_MSC_BASE_ADDRESS, "VPAC_MSC_SCALE", sizeof(msc_config), &prms->config);

#else
        {
            unsigned short *imgInput[2];
            unsigned short *imgOutput[SCALER_NUM_PIPES] = {0};

            imgInput[0] = prms->src16_0;

            if(prms->in_img_format == (vx_df_image)VX_DF_IMAGE_NV12)
            {
                if(prms->out_img_format == (vx_df_image)VX_DF_IMAGE_U8)
                {
                    if(prms->config.settings.unitParams[0].uvMode == false)
                    {
                        imgInput[0] = prms->src16_0;
                    }
                    else if(prms->config.settings.unitParams[0].uvMode == true)
                    {
                        imgInput[0] = prms->src16_cbcr_0;
                    }
                    else
                    {
                        imgInput[0] = prms->src16_0;
                    }
                }
            }
            if(src0 != NULL && src1 != NULL)
            {
                for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
                {
                    if(cnt % 2 == 0)
                    {
                        if(dst[cnt] != NULL)
                        {
                            imgInput[0] = prms->src16_0;
                            imgOutput[cnt] = prms->dst16[cnt];
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        imgOutput[cnt] = NULL;
                    }
                }
            }
            else
            {
                for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE_MAX_OUTPUT; cnt ++)
                {
                    if (NULL != dst[cnt])
                    {
                        imgOutput[cnt] = prms->dst16[cnt];
                    }
                    else
                    {
                        break;
                    }
                }
            }

            scaler_top_processing(imgInput, imgOutput, &prms->config.settings);
        }
#endif
    }
if ((vx_status)VX_SUCCESS == status && src0 != NULL && src1 != NULL)
{

#ifdef VLAB_HWA

    prms->config.magic = 0xC0DEFACE;
    prms->config.buffer[0]  = prms->src16_0;
    for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE_MAX_OUTPUT; cnt ++)
    {
        if (NULL != dst[cnt])
        {
            prms->config.buffer[4 + (cnt * 2)]  = prms->dst16[cnt];
        }
        else
        {
            break;
        }
    }

    status = vlab_hwa_process(VPAC_MSC_BASE_ADDRESS, "VPAC_MSC_SCALE", sizeof(msc_config), &prms->config);

#else
    {
        unsigned short *imgInput[2];
        unsigned short *imgOutput[SCALER_NUM_PIPES] = {0};

        imgInput[0] = prms->src16_1;
        if(prms->in_img0_yc_mode == TIVX_VPAC_MSC_MODE_LUMA_ONLY && prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_CHROMA_ONLY)
        {
            prms->config.settings.G_inHeight[0] = src1->imagepatch_addr[0].dim_y;
        }
        for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
        {
            if(cnt % 2 == 1)
            {
                if(dst[cnt] != NULL)
                {
                    imgInput[0] = prms->src16_1;
                    imgOutput[cnt] = prms->dst16[cnt];
                    if(prms->in_img0_yc_mode == TIVX_VPAC_MSC_MODE_LUMA_ONLY)
                    {
                        if(prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_LUMA_ONLY)
                        {
                            prms->config.settings.unitParams[cnt].uvMode = false;
                        }
                        else
                        {
                            prms->config.settings.unitParams[cnt].uvMode = true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                imgOutput[cnt] = NULL;
            }
        }

        scaler_top_processing(imgInput, imgOutput, &prms->config.settings);
    }
#endif
}
    if ((vx_status)VX_SUCCESS == status)
    {
        if(((src0->format == (vx_df_image)VX_DF_IMAGE_NV12) && (dst[0]->format == (vx_df_image)VX_DF_IMAGE_NV12))||
            (dst[0]->format == (vx_df_image)VX_DF_IMAGE_UYVY) || (dst[0]->format == (vx_df_image)VX_DF_IMAGE_YUYV))
        {
            if((src0->format == (vx_df_image)VX_DF_IMAGE_NV12) && (dst[0]->format == (vx_df_image)VX_DF_IMAGE_NV12))
            {
                prms->config.settings.G_inWidth[0] = src0->imagepatch_addr[1].dim_x;
                prms->config.settings.G_inHeight[0] = src0->imagepatch_addr[1].dim_y / src0->imagepatch_addr[1].step_y;
            }
            else
            {
                prms->config.settings.G_inWidth[0] = src0->imagepatch_addr[0].dim_x;
                prms->config.settings.G_inHeight[0] = src0->imagepatch_addr[0].dim_y / src0->imagepatch_addr[0].step_y;
            }
            prms->config.settings.unitParams[0].uvMode = true;

            for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE_MAX_OUTPUT; cnt ++)
            {
                if ((NULL != dst[cnt]) && ((vx_status)VX_SUCCESS == status))
                {
                    if(dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_NV12)
                    {
                        uint32_t ow = dst[cnt]->imagepatch_addr[1].dim_x;
                        uint32_t oh = dst[cnt]->imagepatch_addr[1].dim_y / dst[cnt]->imagepatch_addr[1].step_y;
                        status = tivxVpacMscScaleUpdateOutputSettings(prms, ow, oh, cnt, 2, dst[cnt]->format);
                    }
                    else if((dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_UYVY) || (dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_YUYV))
                    {
                        uint32_t ow = dst[cnt]->imagepatch_addr[0].dim_x;
                        uint32_t oh = dst[cnt]->imagepatch_addr[0].dim_y / dst[cnt]->imagepatch_addr[0].step_y;
                        status = tivxVpacMscScaleUpdateOutputSettings(prms, ow, oh, cnt, 1, dst[cnt]->format);
                    }
                }
                else
                {
                    break;
                }
            }
        

#ifdef VLAB_HWA

            prms->config.magic = 0xC0DEFACE;
            prms->config.buffer[0]  = prms->src16_cbcr;
            for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE_MAX_OUTPUT; cnt ++)
            {
                if (NULL != dst[cnt])
                {
                    prms->config.buffer[4 + (cnt * 2)]  = prms->dst16_cbcr[cnt];
                }
                else
                {
                    break;
                }
            }

            status = vlab_hwa_process(VPAC_MSC_BASE_ADDRESS, "VPAC_MSC_SCALE", sizeof(msc_config), &prms->config);

#else
            {
                unsigned short *imgInput[2];
                unsigned short *imgOutput[SCALER_NUM_PIPES] = {0};

                imgInput[0] = prms->src16_cbcr_0;

                for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
                {
                    if (NULL != dst[cnt] && ((dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_NV12) || 
                        (dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_YUYV) ||
                        (dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_UYVY)))
                    {
                        prms->config.settings.unitParams[cnt].uvMode = true;
                        imgOutput[cnt] = prms->dst16_cbcr[cnt];
                    }
                    else
                    {
                        break;
                    }
                }

                scaler_top_processing(imgInput, imgOutput, &prms->config.settings);
            }
        }
#endif
    }

    if ((vx_status)VX_SUCCESS == status)
    {
        tivx_obj_desc_image_t stub;
        void *dst_target_ptr;

        stub.valid_roi.start_x = 0;
        stub.valid_roi.start_y = 0;

        for (cnt = 0; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
        {
            if (NULL != dst[cnt])
            {
                /* Reformat output */
                if((src0->format == (vx_df_image)VX_DF_IMAGE_YUYV) || (src0->format == (vx_df_image)VX_DF_IMAGE_UYVY))
                {
                    if(dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_U8)
                    {
                        dst_target_ptr = tivxMemShared2TargetPtr(&dst[cnt]->mem_ptr[0]);
                        tivxCheckStatus(&status, tivxMemBufferMap(dst_target_ptr,
                        dst[cnt]->mem_size[0], (vx_enum)VX_MEMORY_TYPE_HOST,
                        (vx_enum)VX_WRITE_ONLY));
                        lse_reformat_out(&stub, dst[cnt], dst_target_ptr,
                        prms->dst16[cnt], 8, 0);

                    }
                    else
                    {
                        dst_target_ptr = tivxMemShared2TargetPtr(&dst[cnt]->mem_ptr[0]);
                        tivxCheckStatus(&status, tivxMemBufferMap(dst_target_ptr,
                            dst[cnt]->mem_size[0], (vx_enum)VX_MEMORY_TYPE_HOST,
                            (vx_enum)VX_WRITE_ONLY));

                        lse_interleave_422(&stub, dst[cnt], dst_target_ptr, prms->dst16[cnt], prms->dst16_cbcr[cnt], 8);
                    }

                }
                else
                {
                    dst_target_ptr = tivxMemShared2TargetPtr(&dst[cnt]->mem_ptr[0]);
                    tivxCheckStatus(&status, tivxMemBufferMap(dst_target_ptr, dst[cnt]->mem_size[0],
                        (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_WRITE_ONLY));

                    lse_reformat_out(&stub, dst[cnt], dst_target_ptr,
                        prms->dst16[cnt], 12, 0);

                    tivxCheckStatus(&status, tivxMemBufferUnmap(dst_target_ptr, dst[cnt]->mem_size[0],
                        (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_WRITE_ONLY));

                    if(dst[cnt]->format == (vx_df_image)VX_DF_IMAGE_NV12)
                    {
                        dst_target_ptr = tivxMemShared2TargetPtr(&dst[cnt]->mem_ptr[1]);
                        tivxCheckStatus(&status, tivxMemBufferMap(dst_target_ptr, dst[cnt]->mem_size[1],
                            (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_WRITE_ONLY));

                        lse_reformat_out(&stub, dst[cnt], dst_target_ptr,
                            prms->dst16_cbcr[cnt], 12, 1);

                        tivxCheckStatus(&status, tivxMemBufferUnmap(dst_target_ptr, dst[cnt]->mem_size[1],
                            (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_WRITE_ONLY));
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    return status;
}

static vx_status VX_CALLBACK tivxKernelMscScaleDelete(
    tivx_target_kernel_instance kernel, tivx_obj_desc_t *obj_desc[],
    uint16_t num_params, void *priv_arg)
{
    vx_status status = (vx_status)VX_SUCCESS;

    if (((num_params != TIVX_KERNEL_VPAC_MSC_SCALE_MAX_PARAMS) ||
        ((NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_IN0_IMG_IDX]) ||
        (NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE_OUT0_IMG_IDX]))) &&
        ((num_params != TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_PARAMS) ||
        ((NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_IN0_IMG_IDX]) ||
        (NULL == obj_desc[TIVX_KERNEL_VPAC_MSC_SCALE2_OUT0_IMG_IDX]))))
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid Parameters\n");
        status = (vx_status)VX_FAILURE;
    }

    if ((vx_status)VX_SUCCESS == status)
    {
        uint32_t size;
        tivxMscScaleParams *prms = NULL;

        status = tivxGetTargetKernelInstanceContext(kernel,
            (void **)&prms, &size);

        if (((vx_status)VX_SUCCESS == status) && (NULL != prms) &&
            (sizeof(tivxMscScaleParams) == size))
        {
            tivxVpacMscScaleFreeMem(prms);
        }
    }

    return status;
}

static vx_status VX_CALLBACK tivxKernelMscScaleControl(
       tivx_target_kernel_instance kernel,
       uint32_t node_cmd_id, tivx_obj_desc_t *obj_desc[],
       uint16_t num_params, void *priv_arg)
{
    vx_status                     status = (vx_status)VX_SUCCESS;
    uint32_t                      size;
    tivxMscScaleParams           *prms = NULL;

    if ((vx_status)VX_SUCCESS == status)
    {
        status = tivxGetTargetKernelInstanceContext(kernel,
            (void **)&prms, &size);

        if (((vx_status)VX_SUCCESS == status) && (NULL != prms) &&
            (sizeof(tivxMscScaleParams) == size))
        {
            status = (vx_status)VX_SUCCESS;
        }
        else
        {
            status = (vx_status)VX_FAILURE;
        }
    }

    if ((vx_status)VX_SUCCESS == status)
    {
        switch (node_cmd_id)
        {
            case TIVX_VPAC_MSC_CMD_SET_COEFF:
            {
                status = tivxVpacMscScaleSetCoeffsCmd(prms,
                    (tivx_obj_desc_user_data_object_t *)obj_desc[0U]);
                break;
            }
            case TIVX_VPAC_MSC_CMD_SET_INPUT_PARAMS:
            {
                status = tivxVpacMscScaleSetInputParamsCmd(prms,
                    (tivx_obj_desc_user_data_object_t *)obj_desc[0U]);
                break;
            }
            case TIVX_VPAC_MSC_CMD_SET_OUTPUT_PARAMS:
            {
                status = tivxVpacMscScaleSetOutputParamsCmd(prms,
                    (tivx_obj_desc_user_data_object_t **)&obj_desc[0U], num_params);
                break;
            }
            case TIVX_VPAC_MSC_CMD_SET_CROP_PARAMS:
            {
                status = tivxVpacMscScaleSetCropParamsCmd(prms,
                    (tivx_obj_desc_user_data_object_t **)&obj_desc[0U], num_params);
                break;
            }
            default:
            {
                status = (vx_status)VX_FAILURE;
                break;
            }
        }
    }

    return (status);
}

/* ========================================================================== */
/*                          Local Functions                                   */
/* ========================================================================== */

static void tivxVpacMscScaleFreeMem(tivxMscScaleParams *prms)
{
    uint32_t cnt;

    if (NULL != prms)
    {
        if (NULL != prms->src16_0)
        {
            tivxMemFree(prms->src16_0, prms->buffer_size_in0, (vx_enum)TIVX_MEM_EXTERNAL);
            prms->src16_0 = NULL;
        }
        if (NULL != prms->src16_1)
        {
            tivxMemFree(prms->src16_1, prms->buffer_size_in1, (vx_enum)TIVX_MEM_EXTERNAL);
            prms->src16_1 = NULL;
        }
        if (NULL != prms->src16_cbcr_0)
        {
            tivxMemFree(prms->src16_cbcr_0, prms->buffer_size_in_cbcr_0, (vx_enum)TIVX_MEM_EXTERNAL);
            prms->src16_cbcr_0 = NULL;
        }

        for (cnt = 0u; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt ++)
        {
            if ((NULL != prms->dst16[cnt]) &&
                (0U != prms->buffer_size_out[cnt]))
            {
                tivxMemFree(prms->dst16[cnt], prms->buffer_size_out[cnt],
                    (vx_enum)TIVX_MEM_EXTERNAL);
                prms->dst16[cnt] = NULL;
            }
            if ((NULL != prms->dst16_cbcr[cnt]) &&
                (0U != prms->buffer_size_out_cbcr[cnt]))
            {
                tivxMemFree(prms->dst16_cbcr[cnt], prms->buffer_size_out_cbcr[cnt],
                    (vx_enum)TIVX_MEM_EXTERNAL);
                prms->dst16_cbcr[cnt] = NULL;
            }
        }

        tivxMemFree(prms, sizeof(tivxMscScaleParams), (vx_enum)TIVX_MEM_EXTERNAL);
    }
}

static void tivxVpacMscScaleInitParams(Scaler_Config *settings)
{
    uint32_t i;
    uint32_t weight;
    /* The precision (64 phases) of the bilinear interpolation on the random
     * conformance tests for ORB, 3_1, and DOWN_NEAR is not enough
     * to pass these conformance tests.  It is expected that real images
     * would have better results */

    /* Coefficients for Bilinear Interpolation */
    for(i=0; i<32; i++)
    {
        weight = i<<2;
        settings->coef_mp[0].matrix[i][0] = 0;
        settings->coef_mp[0].matrix[i][1] = 0;
        settings->coef_mp[0].matrix[i][2] = 256-weight;
        settings->coef_mp[0].matrix[i][3] = weight;
        settings->coef_mp[0].matrix[i][4] = 0;
    }
    for(i=0; i<32; i++)
    {
        weight = (i+32)<<2;
        settings->coef_mp[1].matrix[i][0] = 0;
        settings->coef_mp[1].matrix[i][1] = 0;
        settings->coef_mp[1].matrix[i][2] = 256-weight;
        settings->coef_mp[1].matrix[i][3] = weight;
        settings->coef_mp[1].matrix[i][4] = 0;
    }
    /* Coefficients for Nearest Neighbor */
    for(i=0; i<32; i++)
    {
        settings->coef_mp[2].matrix[i][0] = 0;
        settings->coef_mp[2].matrix[i][1] = 0;
        settings->coef_mp[2].matrix[i][2] = 256;
        settings->coef_mp[2].matrix[i][3] = 0;
        settings->coef_mp[2].matrix[i][4] = 0;
    }
    for(i=0; i<32; i++)
    {
        settings->coef_mp[3].matrix[i][0] = 0;
        settings->coef_mp[3].matrix[i][1] = 0;
        settings->coef_mp[3].matrix[i][2] = 0;
        settings->coef_mp[3].matrix[i][3] = 256;
        settings->coef_mp[3].matrix[i][4] = 0;
    }

    /* Be default, 5-tap filter */
    settings->cfg_Kernel[0].Sz_height = 5;
    settings->cfg_Kernel[0].Tpad_sz = 2;
    settings->cfg_Kernel[0].Bpad_sz = 2;

    /* Initializing all scaler outputs to defaults */
    for (i = 0u; i < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; i ++)
    {
        settings->unitParams[i].threadMap = 0;
        settings->unitParams[i].coefShift = 8;

        settings->unitParams[i].filter_mode = 1;

        settings->unitParams[i].phase_mode = 0;
        settings->unitParams[i].hs_coef_sel = 0;
        settings->unitParams[i].vs_coef_sel = 0;

        settings->unitParams[i].x_offset = 0;
        settings->unitParams[i].y_offset = 0;
    }
}

static vx_status tivxVpacMscScaleSetCoeffsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj)
{
    vx_status                     status = (vx_status)VX_SUCCESS;
    void                         *target_ptr;
    tivx_vpac_msc_coefficients_t *coeffs;
    uint32_t                      i, j, idx;

    if (NULL != usr_data_obj)
    {
        target_ptr = tivxMemShared2TargetPtr(&usr_data_obj->mem_ptr);

        tivxCheckStatus(&status, tivxMemBufferMap(target_ptr, usr_data_obj->mem_size,
            (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

        if (sizeof(tivx_vpac_msc_coefficients_t) ==
                usr_data_obj->mem_size)
        {
            coeffs = (tivx_vpac_msc_coefficients_t *)target_ptr;
            for (i = 0; i < 4; i ++)
            {
                idx = 0;
                for (j = 0; j < 32; j ++)
                {
                    prms->config.settings.coef_mp[i].matrix[j][0] =
                        coeffs->multi_phase[i][idx ++];
                    prms->config.settings.coef_mp[i].matrix[j][1] =
                        coeffs->multi_phase[i][idx ++];
                    prms->config.settings.coef_mp[i].matrix[j][2] =
                        coeffs->multi_phase[i][idx ++];
                    prms->config.settings.coef_mp[i].matrix[j][3] =
                        coeffs->multi_phase[i][idx ++];
                    prms->config.settings.coef_mp[i].matrix[j][4] =
                        coeffs->multi_phase[i][idx ++];
                }
            }

            for (i = 0; i < 2; i ++)
            {
                for (j = 0; j < 5; j ++)
                {
                    prms->config.settings.coef_sp[i][j] =
                        coeffs->single_phase[i][j];
                }
            }
        }
    }
    else
    {
        status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
    }

    return (status);
}

static vx_status tivxVpacMscScaleSetInputParamsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj)
{
    vx_status                         status = (vx_status)VX_SUCCESS;
    tivx_vpac_msc_input_params_t     *in_prms = NULL;
    void                             *target_ptr;
    uint32_t                          cnt;

    if (NULL != usr_data_obj)
    {
        target_ptr = tivxMemShared2TargetPtr(&usr_data_obj->mem_ptr);

        tivxCheckStatus(&status, tivxMemBufferMap(target_ptr, usr_data_obj->mem_size,
            (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

        if (sizeof(tivx_vpac_msc_input_params_t) ==
                usr_data_obj->mem_size)
        {
            in_prms = (tivx_vpac_msc_input_params_t *)target_ptr;
            switch (in_prms->kern_sz)
            {
                case 3:
                    prms->config.settings.cfg_Kernel[0].Sz_height = 3;
                    prms->config.settings.cfg_Kernel[0].Tpad_sz = 1;
                    prms->config.settings.cfg_Kernel[0].Bpad_sz = 1;
                    break;
                case 4:
                    prms->config.settings.cfg_Kernel[0].Sz_height = 4;
                    prms->config.settings.cfg_Kernel[0].Tpad_sz = 1;
                    prms->config.settings.cfg_Kernel[0].Bpad_sz = 2;
                    break;
                case 5:
                    prms->config.settings.cfg_Kernel[0].Sz_height = 5;
                    prms->config.settings.cfg_Kernel[0].Tpad_sz = 2;
                    prms->config.settings.cfg_Kernel[0].Bpad_sz = 2;
                    break;
                default:
                    VX_PRINT(VX_ZONE_ERROR, "Invalid Kernel Size\n");
                    status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
                    break;
            }
            if(1 < in_prms->yc_mode)
            {
                VX_PRINT(VX_ZONE_ERROR, "yc_mode value should be either 0 or 1\n");
                status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
            }
            if((vx_status)VX_SUCCESS == status)
            {
                for(cnt = 0U; cnt < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT; cnt++)
                {
                    if ((vx_df_image)VX_DF_IMAGE_NV12 == prms->in_img_format)
                    {
                        if((vx_df_image)VX_DF_IMAGE_U8 == prms->out_img_format)
                        {
                            if(TIVX_VPAC_MSC_MODE_LUMA_ONLY == in_prms->yc_mode)
                            {
                                prms->config.settings.unitParams[cnt].uvMode = false;
                            }
                            else if(TIVX_VPAC_MSC_MODE_CHROMA_ONLY == in_prms->yc_mode)
                            {
                                prms->config.settings.unitParams[cnt].uvMode = true;
                                prms->crop_height[cnt] = prms->in0_height>>1;
                            }
                        }
                    }
                    #if defined(VPAC3) || defined(VPAC3L)
                    if(prms->imgIn0 != NULL && prms->imgIn1 != NULL)
                    {
                        if(cnt % 2 == 0)
                        {
                            prms->crop_width[cnt] = prms->imgIn0->imagepatch_addr[0].dim_x;
                            prms->crop_height[cnt] = prms->imgIn0->imagepatch_addr[0].dim_y;
                        }
                        else
                        {
                            prms->crop_width[cnt] = prms->imgIn1->imagepatch_addr[0].dim_x;
                            prms->crop_height[cnt] = prms->imgIn1->imagepatch_addr[0].dim_y;
                            if(prms->in_img0_yc_mode == TIVX_VPAC_MSC_MODE_LUMA_ONLY && prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_CHROMA_ONLY)
                            {
                                prms->config.settings.unitParams[cnt].uvMode = true;
                            }
                        }
                    }
                    #endif
                }
                #if defined(VPAC3) || defined(VPAC3L)
                prms->in_img0_yc_mode = in_prms->in_img0_yc_mode;
                prms->in_img1_yc_mode = in_prms->in_img1_yc_mode;
                if(in_prms->in_img0_yc_mode == TIVX_VPAC_MSC_MODE_CHROMA_ONLY)
                {
                    VX_PRINT(VX_ZONE_ERROR, "Input0 should be luma only\n");
                    status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
                }
                if(NULL != prms->imgIn1)
                {
                    if(in_prms->in_img0_yc_mode != TIVX_VPAC_MSC_MODE_LUMA_ONLY)
                    {
                        VX_PRINT(VX_ZONE_ERROR, "Input0 should be luma only \n");
                        status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
                    }
                    if(in_prms->in_img0_yc_mode == TIVX_VPAC_MSC_MODE_LUMA_ONLY)
                    {
                        if(in_prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_LUMA_ONLY)
                        {
                            if(prms->in0_height != (prms->in1_height))
                            {
                                VX_PRINT(VX_ZONE_ERROR, "Incase of luma-luma plane processing height of both planes should be same\n");
                                status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
                            }
                        }
                        else if((in_prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_420SP_UV_CHROMA_ONLY) ||
                                (in_prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_420SP_VU_CHROMA_ONLY))
                        {
                            if(prms->in0_height != (prms->in1_height * 2))
                            {
                                VX_PRINT(VX_ZONE_ERROR, "Incase of YUV420SP processing Chroma plane height should be half of luma plane height\n");
                                status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
                            }
                        }
                        else if((in_prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_422SP_UV_CHROMA_ONLY) ||
                                (in_prms->in_img1_yc_mode == TIVX_VPAC_MSC_MODE_422SP_VU_CHROMA_ONLY))
                        {
                            if(prms->in0_height != (prms->in1_height))
                            {
                                VX_PRINT(VX_ZONE_ERROR, "Incase of YUV422SP processing Chroma plane height should be same as luma plane height\n");
                                status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
                            }
                        }
                        else
                        {
                            /*Do nothing*/
                        }
                    }
                }
                #endif
            }

        }
        else
        {
            VX_PRINT(VX_ZONE_ERROR, "Invalid Size \n");
            status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
        }

        tivxCheckStatus(&status, tivxMemBufferUnmap(target_ptr, usr_data_obj->mem_size,
            (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "User Data Object is NULL \n");
        status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
    }

    return (status);
}

static vx_status tivxVpacMscScaleSetOutputParamsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj[], uint16_t num_params)
{
    vx_status                         status = (vx_status)VX_SUCCESS;
    uint32_t                          cnt;
    tivx_vpac_msc_output_params_t    *out_prms = NULL;
    void                             *target_ptr;

    uint32_t loop_params = (num_params < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT) ? \
                            num_params : TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT;

    for (cnt = 0u; cnt < loop_params; cnt ++)
    {
        if (NULL != usr_data_obj[cnt])
        {
            target_ptr = tivxMemShared2TargetPtr(&usr_data_obj[cnt]->mem_ptr);

            tivxCheckStatus(&status, tivxMemBufferMap(target_ptr, usr_data_obj[cnt]->mem_size,
                (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

            if (sizeof(tivx_vpac_msc_output_params_t) ==
                    usr_data_obj[cnt]->mem_size)
            {
                out_prms = (tivx_vpac_msc_output_params_t *)target_ptr;

                prms->config.settings.unitParams[cnt].signedData = out_prms->signed_data;
                prms->config.settings.unitParams[cnt].filter_mode = out_prms->filter_mode;
                prms->config.settings.unitParams[cnt].coefShift = out_prms->coef_shift;
                prms->config.settings.unitParams[cnt].satMode = out_prms->saturation_mode;
                prms->config.settings.unitParams[cnt].sp_hs_coef_src = out_prms->single_phase.horz_coef_src;
                prms->config.settings.unitParams[cnt].sp_hs_coef_sel = out_prms->single_phase.horz_coef_sel;
                prms->config.settings.unitParams[cnt].sp_vs_coef_src = out_prms->single_phase.vert_coef_src;
                prms->config.settings.unitParams[cnt].sp_vs_coef_sel = out_prms->single_phase.vert_coef_sel;

                prms->config.settings.unitParams[cnt].phase_mode = out_prms->multi_phase.phase_mode;
                prms->config.settings.unitParams[cnt].hs_coef_sel = out_prms->multi_phase.horz_coef_sel;
                prms->config.settings.unitParams[cnt].vs_coef_sel = out_prms->multi_phase.vert_coef_sel;

                /* These will be applied or autocalculated during process callback (after all user commands have been called) */
                prms->user_init_phase_x[cnt] = out_prms->multi_phase.init_phase_x;
                prms->user_init_phase_y[cnt] = out_prms->multi_phase.init_phase_y;
                prms->user_offset_x[cnt] =     out_prms->offset_x;
                prms->user_offset_y[cnt] =     out_prms->offset_y;
            }
            else
            {
                VX_PRINT(VX_ZONE_ERROR,
                    "Invalid Mem Size for tivx_vpac_msc_output_params_t\n");
                status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
            }

            tivxCheckStatus(&status, tivxMemBufferUnmap(target_ptr, usr_data_obj[cnt]->mem_size,
                (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));
        }

        if ((vx_status)VX_SUCCESS != status)
        {
            break;
        }
    }

    return (status);
}

static vx_status tivxVpacMscScaleSetCropParamsCmd(tivxMscScaleParams *prms,
    tivx_obj_desc_user_data_object_t *usr_data_obj[], uint16_t num_params)
{
    vx_status                         status = (vx_status)VX_SUCCESS;
    uint32_t                          cnt;
    tivx_vpac_msc_crop_params_t      *out_prms = NULL;
    void                             *target_ptr;

    uint32_t loop_params = (num_params < TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT) ? \
                            num_params : TIVX_KERNEL_VPAC_MSC_SCALE2_MAX_OUTPUT;

    for (cnt = 0u; cnt < loop_params; cnt ++)
    {
        if (NULL != usr_data_obj[cnt])
        {
            target_ptr = tivxMemShared2TargetPtr(&usr_data_obj[cnt]->mem_ptr);

            tivxCheckStatus(&status, tivxMemBufferMap(target_ptr, usr_data_obj[cnt]->mem_size,
                (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));

            if (sizeof(tivx_vpac_msc_crop_params_t) ==
                    usr_data_obj[cnt]->mem_size)
            {
                out_prms = (tivx_vpac_msc_crop_params_t *)target_ptr;

                prms->crop_width[cnt] = out_prms->crop_width;
                prms->crop_height[cnt] = out_prms->crop_height;

                /* These will be applied or autocalculated during process callback (after all user commands have been called) */
                prms->user_crop_start_x[cnt] = out_prms->crop_start_x;
                prms->user_crop_start_y[cnt] = out_prms->crop_start_y;
            }
            else
            {
                VX_PRINT(VX_ZONE_ERROR, "Invalid Mem Size for Crop Params\n");
                status = (vx_status)VX_ERROR_INVALID_PARAMETERS;
            }

            tivxCheckStatus(&status, tivxMemBufferUnmap(target_ptr, usr_data_obj[cnt]->mem_size,
                (vx_enum)VX_MEMORY_TYPE_HOST, (vx_enum)VX_READ_ONLY));
        }

        if ((vx_status)VX_SUCCESS != status)
        {
            break;
        }
    }

    return (status);
}

static vx_status tivxVpacMscScaleUpdateOutputSettings(tivxMscScaleParams *prms, uint32_t ow, uint32_t oh, uint32_t cnt, uint32_t h_divider, vx_df_image format)
{

    vx_status status = (vx_status)VX_SUCCESS;
    float temp_horzAccInit, temp_vertAccInit;
    uint32_t int_horzAccInit, int_vertAccInit;
    uint32_t temp_cropStartX, temp_cropStartY;
    uint32_t iw = prms->crop_width[cnt];
    uint32_t ih = prms->crop_height[cnt]/h_divider;
    uint32_t hzScale = (4096*iw+(ow>>1))/ow;
    uint32_t vtScale = (4096*ih+(oh>>1))/oh;

    if(hzScale > 16384U)
    {
        VX_PRINT(VX_ZONE_ERROR,
            "Output %d: max horizontal downscale exceeded, limit is 1/4\n", cnt);
        status = (vx_status)VX_FAILURE;
    }

    if(vtScale > 16384U)
    {
        VX_PRINT(VX_ZONE_ERROR,
            "Output %d: max vertical downscale exceeded, limit is 1/4\n", cnt);
        status = (vx_status)VX_FAILURE;
    }

    prms->config.settings.unitParams[cnt].outWidth = ow;
    prms->config.settings.unitParams[cnt].outHeight = oh;
    prms->config.settings.unitParams[cnt].hzScale = hzScale;
    prms->config.settings.unitParams[cnt].vtScale = vtScale;

    if((TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_offset_x[cnt]) ||
       (TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_init_phase_x[cnt]))
    {
        temp_horzAccInit = (((((float)iw/(float)ow) * 0.5f) - 0.5f) * 4096.0f) + 0.5f;
        int_horzAccInit = (uint32_t)temp_horzAccInit;
        temp_cropStartX = 0;
        if(int_horzAccInit > 4095U)
        {
            int_horzAccInit -= 4096U;
            temp_cropStartX = 1U;
        }

        if(TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_init_phase_x[cnt])
        {
            prms->config.settings.unitParams[cnt].initPhaseX = int_horzAccInit;
        }
        else
        {
            prms->config.settings.unitParams[cnt].initPhaseX = prms->user_init_phase_x[cnt];
        }

        if(TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_offset_x[cnt])
        {
            prms->config.settings.unitParams[cnt].x_offset = prms->user_crop_start_x[cnt] + temp_cropStartX;
        }
        else
        {
            prms->config.settings.unitParams[cnt].x_offset = prms->user_crop_start_x[cnt] + prms->user_offset_x[cnt];
        }

        /* TIOVX-1129: If NV12, x_offset should be an even number to not flip the chroma channels */
        if ((format == (vx_df_image)VX_DF_IMAGE_NV12) || (format == (vx_df_image)VX_DF_IMAGE_UYVY) || (format == (vx_df_image)VX_DF_IMAGE_YUYV))
        {
            if((prms->config.settings.unitParams[cnt].x_offset & 1U) == 1U)
            {
                prms->config.settings.unitParams[cnt].x_offset--;
                prms->config.settings.unitParams[cnt].initPhaseX = 4095U;
            }
        }
    }
    else
    {
        prms->config.settings.unitParams[cnt].initPhaseX = prms->user_init_phase_x[cnt];
        prms->config.settings.unitParams[cnt].x_offset = prms->user_crop_start_x[cnt] + prms->user_offset_x[cnt];
    }

    if((TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_offset_y[cnt]) ||
       (TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_init_phase_y[cnt]))
    {
        temp_vertAccInit = (((((float)ih/(float)oh) * 0.5f) - 0.5f) * 4096.0f) + 0.5f;
        int_vertAccInit = (uint32_t)temp_vertAccInit;
        temp_cropStartY = 0;
        if(int_vertAccInit > 4095U)
        {
            int_vertAccInit -= 4096U;
            temp_cropStartY = 1U;
        }

        if(TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_init_phase_y[cnt])
        {
            prms->config.settings.unitParams[cnt].initPhaseY = int_vertAccInit;
        }
        else
        {
            prms->config.settings.unitParams[cnt].initPhaseY = prms->user_init_phase_y[cnt];
        }

        if(TIVX_VPAC_MSC_AUTOCOMPUTE == prms->user_offset_y[cnt])
        {
            prms->config.settings.unitParams[cnt].y_offset = prms->user_crop_start_y[cnt] + temp_cropStartY;
        }
        else
        {
            prms->config.settings.unitParams[cnt].y_offset = prms->user_crop_start_y[cnt] + prms->user_offset_y[cnt];
        }
        if(h_divider == 2)
        {
            prms->config.settings.unitParams[cnt].y_offset = prms->config.settings.unitParams[cnt].y_offset/2;
        }
    }
    else
    {
        prms->config.settings.unitParams[cnt].initPhaseY = prms->user_init_phase_y[cnt];
        prms->config.settings.unitParams[cnt].y_offset = prms->user_crop_start_y[cnt] + prms->user_offset_y[cnt];
        if(h_divider == 2)
        {
            prms->config.settings.unitParams[cnt].y_offset = prms->config.settings.unitParams[cnt].y_offset/2;
        }
    }

    return status;
}
