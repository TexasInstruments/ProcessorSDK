/*
 *
 * Copyright (c) 2025 Texas Instruments Incorporated
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

#ifdef BUILD_VPAC_FC


#include <VX/vx.h>
#include <VX/vxu.h>
#include <TI/tivx.h>
#include <TI/hwa_kernels.h>
#include <TI/hwa_vpac_viss.h>
#include <TI/hwa_vpac_msc.h>
#include <TI/hwa_vpac_fc.h>
#include "test_engine/test.h"
#include "tivx_utils_file_rd_wr.h"
#include <string.h>
#include <utils/iss/include/app_iss.h>
#include "test_hwa_common.h"
#include "tivx_utils_checksum.h"
#include <stdio.h>

#define TEST_NUM_NODE_INSTANCE 2


#define APP_MAX_FILE_PATH           (512u)
// #define TEST_FC_CHECKSUM_LOGGING

#ifndef x86_64

TESTCASE(tivxHwaVpacFc, CT_VXContext, ct_setup_vx_context, 0)
TESTCASE(tivxHwaVpacFcNegative, CT_VXContext, ct_setup_vx_context, 0)

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    char *target_string;

} SetTarget_Arg;

#if (VPAC_COUNT > 1)
#define ADD_SET_TARGET_PARAMETERS_MULTI_INST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_FC/TIVX_TARGET_VPAC2_FC", __VA_ARGS__, TIVX_TARGET_VPAC_FC, TIVX_TARGET_VPAC2_FC)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_FC", __VA_ARGS__, TIVX_TARGET_VPAC_FC, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_FC", __VA_ARGS__, TIVX_TARGET_VPAC2_FC, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_FC/TIVX_TARGET_VPAC_FC", __VA_ARGS__, TIVX_TARGET_VPAC_FC, TIVX_TARGET_VPAC_FC)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_FC/TIVX_TARGET_VPAC2_FC", __VA_ARGS__, TIVX_TARGET_VPAC2_FC, TIVX_TARGET_VPAC2_FC))
#else
#define ADD_SET_TARGET_PARAMETERS_MULTI_INST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_FC/NULL", __VA_ARGS__, TIVX_TARGET_VPAC_FC, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_FC/TIVX_TARGET_VPAC_FC", __VA_ARGS__, TIVX_TARGET_VPAC_FC, TIVX_TARGET_VPAC_FC))
#endif

#if (VPAC_COUNT > 1)
#define ADD_SET_TARGET_PARAMETERS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_FC", __VA_ARGS__, TIVX_TARGET_VPAC_FC)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_FC", __VA_ARGS__, TIVX_TARGET_VPAC2_FC))
#else
#define ADD_SET_TARGET_PARAMETERS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_FC", __VA_ARGS__, TIVX_TARGET_VPAC_FC))
#endif

#define SET_NODE_TARGET_PARAMETERS \
    CT_GENERATE_PARAMETERS("target", ADD_SET_TARGET_PARAMETERS, ARG, NULL)

TEST_WITH_ARG(tivxHwaVpacFc, testNodeCreation, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;
    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_image y12 = NULL, uv12_c1 = NULL, y8_r8_c2 = NULL, uv8_g8_c3 = NULL;
    vx_user_data_object h3a_aew_af = NULL;
    vx_image dst_image[10] = {0};
    vx_uint32 dst_width = 1920/2;
    vx_uint32 dst_height = 1080/2;

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;

    vx_graph graph = 0;
    vx_node node = 0;

    tivx_raw_image_create_params_t raw_params;
    raw_params.width = 1920;
    raw_params.height = 1080;
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.format[1].pixel_container = TIVX_RAW_IMAGE_8_BIT;
    raw_params.format[1].msb = 7;
    raw_params.format[2].pixel_container = TIVX_RAW_IMAGE_P12_BIT;
    raw_params.format[2].msb = 11;
    raw_params.meta_height_before = 5;
    raw_params.meta_height_after = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);
        ASSERT_VX_OBJECT(y12 = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(uv12_c1 = vxCreateImage(context, 1920, 1080/2, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(y8_r8_c2 = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(uv8_g8_c3 = vxCreateImage(context, 1920, 1080/2, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        for(int i = 0; i < 10; i++)
        {
            ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width, dst_height, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        }
        memset(&params, 0, sizeof(tivx_vpac_fc_viss_msc_params_t));
        ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        memset(&ae_awb_params, 0, sizeof(tivx_ae_awb_params_t));
        ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, NULL,
                                                raw, y12, uv12_c1, y8_r8_c2, uv8_g8_c3,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&uv8_g8_c3));
        VX_CALL(vxReleaseImage(&y8_r8_c2));
        VX_CALL(vxReleaseImage(&uv12_c1));
        VX_CALL(vxReleaseImage(&y12));
        VX_CALL(tivxReleaseRawImage(&raw));
        for (int i = 0; i < 10; i++)
        {
            VX_CALL(vxReleaseImage(&dst_image[i]));
            ASSERT(dst_image[i] == 0);
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseUserDataObject(&configuration));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(uv8_g8_c3 == 0);
        ASSERT(y8_r8_c2 == 0);
        ASSERT(uv12_c1 == 0);
        ASSERT(y12 == 0);
        ASSERT(raw == 0);
        ASSERT(ae_awb_result == 0);
        ASSERT(configuration == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacFc, testGraphProcessing_ten_outputs, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;

    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_user_data_object h3a_aew_af = NULL;

    int dst_width[10], dst_height[10];
    vx_image dst_image[10] = {0};

    vx_user_data_object output_obj[10];
    vx_user_data_object sc_input_params_obj;

    tivx_vpac_msc_coefficients_t coeffs;
    tivx_vpac_msc_input_params_t sc_input_params;
    tivx_vpac_msc_output_params_t output_params[10];
    vx_enum interpolation = VX_INTERPOLATION_BILINEAR;

    vx_reference refs[1] = {0};
    vx_reference  output_refs[10] = {0};

    vx_pixel_value_t pixel;
    vx_uint32 inp_width = 1920;
    vx_uint32 inp_height = 1080;

    char file[MAXPATHLENGTH];

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;

    vx_graph graph = 0;
    vx_node node = 0;

    uint32_t cnt;

    tivx_raw_image_create_params_t raw_params;

    /* FC input params */
    raw_params.width    = inp_width;
    raw_params.height   = inp_height;
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.format[1].pixel_container = TIVX_RAW_IMAGE_8_BIT;
    raw_params.format[1].msb = 7;
    raw_params.format[2].pixel_container = TIVX_RAW_IMAGE_P12_BIT;
    raw_params.format[2].msb = 11;
    raw_params.meta_height_before = 0;
    raw_params.meta_height_after = 0;

    /* FC output params */
    float scale_factor[10] = {0.25, 0.25, 0.5, 0.5, 0.9, 0.9, 0.8, 0.8, 0.4, 0.4};

    /* FC output params */
    for(int i = 0; i < 10; i++)
    {
        dst_width[i] = (uint32_t)(inp_width * scale_factor[i]);

        if(i%2 == 0)
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]);
        }
        else
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]) / 2U;
        }
    }

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        /* Input Image configurations */
        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);

        /* Output Image Configurations */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            #else
                if (i % 2 == 0)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                    tivx_vpac_msc_output_params_init(&output_params[i]);
                    scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
                }
            #endif
        }        

        /* Create/Configure configuration input structure */
        tivx_vpac_fc_params_init(&(params));
        tivx_vpac_viss_params_init(&(params.tivxVissPrms));
        ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        memset(&ae_awb_params, 0, sizeof(tivx_ae_awb_params_t));
        ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        /* VISS params for FC */
        params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;
        /* YUV422 or YUV420 to be decided from chroma mode */
        params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;

        params.tivxVissPrms.h3a_in = TIVX_VPAC_VISS_H3A_IN_LSC;        
        params.tivxVissPrms.h3a_aewb_af_mode = 0;
        params.tivxVissPrms.bypass_glbce = 0;
        params.tivxVissPrms.bypass_nsf4 = 0;
        params.tivxVissPrms.bypass_cac = 1;

        /* MSC params for FC */
        /* Using control commands below */


        /* FC params */
        #if defined (VPAC3) || defined (VPAC3L)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #else
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #endif        

        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
            #else
                if (i % 2 == 0)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
            #endif
        }

        VX_CALL(vxCopyUserDataObject(configuration, 0, sizeof(tivx_vpac_fc_viss_msc_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(ae_awb_result, 0, sizeof(tivx_ae_awb_params_t), &ae_awb_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT(graph != NULL);
        ASSERT(configuration != NULL);
        ASSERT(ae_awb_result != NULL);
        ASSERT(raw != NULL);
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] != NULL);
                }
            #else
                ASSERT(dst_image[i] != NULL);
            #endif
        }

        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, NULL, NULL,
                                                raw, NULL, NULL, NULL, NULL,
                                                NULL, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        img_scale_set_coeff(&coeffs, interpolation);

        VX_CALL(vxVerifyGraph(graph));

        /* Process Graph */

        VX_CALL(vxProcessGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Delete the Flexconnect graph using the control command */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(tivxReleaseRawImage(&raw));
        VX_CALL(vxReleaseUserDataObject(&configuration));
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
            {
                VX_CALL(vxReleaseImage(&dst_image[i]));
            }
            #else
                VX_CALL(vxReleaseImage(&dst_image[i]));
            #endif
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(graph == 0);
        ASSERT(node == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(raw == 0);
        ASSERT(configuration == 0);
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] == 0);
                }
            #else
                ASSERT(dst_image[i] == 0);
            #endif
        }
        ASSERT(ae_awb_result == 0);

        tivxHwaUnLoadKernels(context);
    }
}

uint32_t expected_checksums[10] = {
    0x28051073,
    0x23ad4b3d,
    0xc3ed46c8,
    0xd04acf2e,
    0x2817ed59,
    0x100892b8,
    0xb9721eb8,
    0x004f9be9,
    0xb31604a4,
    0x559b86a9
};

TEST_WITH_ARG(tivxHwaVpacFc, testGraphProcessing_ten_outputs_dcc, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;

    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_user_data_object h3a_aew_af = NULL;

    int dst_width[10], dst_height[10];
    vx_image dst_image[10] = {0};
    for (int i = 0; i < 10; i++)
    {
        dst_image[i] = NULL;
    }
    vx_user_data_object coeff_obj, msc_inst_id_obj, output_obj[10];
    vx_user_data_object sc_input_params_obj;

    tivx_vpac_msc_coefficients_t coeffs;
    uint32_t msc_inst_id;
    tivx_vpac_msc_input_params_t sc_input_params;
    tivx_vpac_msc_output_params_t output_params[10];
    vx_enum interpolation = VX_INTERPOLATION_BILINEAR;

    vx_reference refs[2] = {0};
    vx_reference  output_refs[11] = {0};

    vx_pixel_value_t pixel;
    vx_uint32 inp_width = 1936;
    vx_uint32 inp_height = 1096;

    char file[MAXPATHLENGTH];

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;
    tivx_h3a_data_t h3a_data;

    vx_graph graph = 0;
    vx_node node = 0;

    uint32_t cnt;

    tivx_raw_image_create_params_t raw_params;

    /* Dcc objects */
    vx_user_data_object dcc_param_viss = NULL;
    const vx_char dcc_viss_user_data_object_name[] = "dcc_viss";
    vx_size dcc_buff_size = 1;
    vx_map_id dcc_viss_buf_map_id;
    uint8_t * dcc_viss_buf;
    int32_t dcc_status;
    uint32_t checksum_actual = 0;
    vx_rectangle_t rect;
    uint32_t sensor_dcc_id;
    uint32_t sensor_dcc_mode;
    char *sensor_name = NULL;
    char *file_name = NULL;
    uint16_t downshift_bits;

    raw_params.width = inp_width;
    raw_params.height = inp_height;
    raw_params.meta_height_after = 4;
    sensor_dcc_id = 390;
    sensor_name = SENSOR_SONY_IMX390_UB953_D3;
    sensor_dcc_mode = 0;
    file_name = "psdkra/app_single_cam/IMX390_001/input2.raw";
    downshift_bits = 0;

    /* FC input params */
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.meta_height_before = 0;

    float scale_factor[10] = {0.25, 0.25, 0.5, 0.5, 0.9, 0.9, 0.8, 0.8, 0.4, 0.4};

    /* FC output params */
    for(int i = 0; i < 10; i++)
    {
        dst_width[i] = (uint32_t)(inp_width * scale_factor[i]);

        if(i%2 == 0)
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]);
        }
        else
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]) / 2U;
        }
    }

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        /* Input Image configurations */
        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);

        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_WIDTH, &inp_width, sizeof(inp_width)));
        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_HEIGHT, &inp_height, sizeof(inp_height)));

        /* Output Image Configurations */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            #else
                if (i % 2 == 0)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                    tivx_vpac_msc_output_params_init(&output_params[i]);
                    scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
                }
            #endif
        }        

        /* Create/Configure configuration input structure */
        tivx_vpac_fc_params_init(&(params));

        tivx_vpac_viss_params_init(&(params.tivxVissPrms));

        ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        /* VISS params for FC */
        params.tivxVissPrms.sensor_dcc_id = sensor_dcc_id;
        params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;
        /* YUV422 or YUV420 to be decided from chroma mode */
        params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;

        params.tivxVissPrms.h3a_in = TIVX_VPAC_VISS_H3A_IN_LSC;        
        params.tivxVissPrms.h3a_aewb_af_mode = TIVX_VPAC_VISS_H3A_MODE_AEWB;
        params.tivxVissPrms.bypass_glbce = 1;
        params.tivxVissPrms.bypass_nsf4 = 0;
        params.tivxVissPrms.bypass_cac = 1;

        /* MSC params for FC */
        /* Using control commands below */

        /* FC params */
        /* If MSC0 is used then msc_in_thread_viss_out_map[0] and msc_in_thread_viss_out_map[1] must be populated  */
        /* If MSC1 is used then msc_in_thread_viss_out_map[2] and msc_in_thread_viss_out_map[3] must be populated */
        #if defined (VPAC3)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_VISS_OUT3;
        #elif defined (VPAC3L)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #else
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #endif

        /* Mixed usage of MSC0 and MSC1 is not supported as of now */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC1;
            #elif defined(VPAC3L)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
            #else
                if (i % 2 == 0)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
            #endif
        }

        VX_CALL(vxCopyUserDataObject(configuration, 0, sizeof(tivx_vpac_fc_viss_msc_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        /* Create/Configure ae_awb_params input structure */
        tivx_ae_awb_params_init(&ae_awb_params);
        ae_awb_params.ae_valid = 1;
        ae_awb_params.exposure_time = 16666;
        ae_awb_params.analog_gain = 1030;
        ae_awb_params.awb_valid = 1;
        ae_awb_params.color_temperature = 3000;
        for (int j=0; j<4; j++)
        {
            ae_awb_params.wb_gains[j] = 525;
            ae_awb_params.wb_offsets[j] = 2;
        }

        VX_CALL(vxCopyUserDataObject(ae_awb_result, 0, sizeof(tivx_ae_awb_params_t), &ae_awb_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        /* Creating DCC */
        dcc_buff_size = appIssGetDCCSizeVISS(sensor_name, sensor_dcc_mode);

        ASSERT_VX_OBJECT(dcc_param_viss = vxCreateUserDataObject( context, (const vx_char*)&dcc_viss_user_data_object_name,
            dcc_buff_size, NULL),(enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxMapUserDataObject(
            dcc_param_viss,
            0,
            dcc_buff_size,
            &dcc_viss_buf_map_id,
            (void **)&dcc_viss_buf,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            0
        ));
        memset(dcc_viss_buf, 0xAB, dcc_buff_size);

        dcc_status = appIssGetDCCBuffVISS(sensor_name, sensor_dcc_mode, dcc_viss_buf, dcc_buff_size);
        ASSERT(dcc_status == 0);

        VX_CALL(vxUnmapUserDataObject(dcc_param_viss, dcc_viss_buf_map_id));
        /* Done w/ DCC */

        /* Creating H3A output */
        tivx_h3a_data_init(&h3a_data);
        ASSERT_VX_OBJECT(h3a_aew_af = vxCreateUserDataObject(context, "tivx_h3a_data_t", sizeof(tivx_h3a_data_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        if(NULL != h3a_aew_af)
        {
            VX_CALL(vxMapUserDataObject(h3a_aew_af,
                0,
                sizeof(tivx_h3a_data_t),
                &dcc_viss_buf_map_id,
                (void **)&dcc_viss_buf,
                (vx_enum)VX_WRITE_ONLY,
                (vx_enum)VX_MEMORY_TYPE_HOST,
                0
                ));

            memset(dcc_viss_buf, 0, sizeof(tivx_h3a_data_t));

            VX_CALL(vxUnmapUserDataObject(h3a_aew_af, dcc_viss_buf_map_id));
        }

        ASSERT(graph != NULL);
        ASSERT(configuration != NULL);
        ASSERT(ae_awb_result != NULL);
        ASSERT(raw != NULL);
        ASSERT(dcc_param_viss != NULL);
        ASSERT(h3a_aew_af != NULL);

        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] != NULL);
                }
            #else
                ASSERT(dst_image[i] != NULL);
            #endif
        }

        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ct_read_raw_image(raw, file_name, 2, downshift_bits);

        img_scale_set_coeff(&coeffs, interpolation);

        VX_CALL(vxVerifyGraph(graph));

        /* Set MSC Coefficients */

        ASSERT_VX_OBJECT(coeff_obj = vxCreateUserDataObject(context,
            "tivx_vpac_msc_coefficients_t",
            sizeof(tivx_vpac_msc_coefficients_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(coeff_obj, 0,
            sizeof(tivx_vpac_msc_coefficients_t), &coeffs, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        #if defined(VPAC3)
            msc_inst_id = TIVX_VPAC_FC_MSC1;
        #else
            msc_inst_id = TIVX_VPAC_FC_MSC0;
        #endif
        ASSERT_VX_OBJECT(msc_inst_id_obj = vxCreateUserDataObject(context,
            "uint32_t",
            sizeof(uint32_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(msc_inst_id_obj, 0,
            sizeof(uint32_t), &msc_inst_id, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        refs[0] = (vx_reference)coeff_obj;
        refs[1] = (vx_reference)msc_inst_id_obj;
        ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
            refs, 2u));

        /* Process Graph */

        VX_CALL(vxProcessGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        rect.start_x = 0;
        rect.start_y = 0;
        for (int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
                    rect.end_x = dst_width[i];
                    rect.end_y = dst_height[i];
                    checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                    if(checksum_actual == expected_checksums[i])
                    {
                        printf("Checksum for output %d is correct\n", i);
                    }
                    else
                    {
                        if (i != 6 && i != 7)
                        {
                            printf("Checksum for output %d is incorrect\n", i);
                        }
                    }
                    #if defined(TEST_FC_CHECKSUM_LOGGING)
                    vx_char temp[MAXPATHLENGTH];
                    printf("0x%08x\n", checksum_actual);
                    snprintf(temp, MAXPATHLENGTH, "%s/%s%d", ct_get_test_file_path(), "output/u8_fc_", i);
                    write_output_ir_image(temp, dst_image[i], 0);
                    #endif
            #else
                    if (i % 2 == 0)
                    {
                    rect.end_x = dst_width[i];
                    rect.end_y = dst_height[i];
                    checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                    if(checksum_actual == expected_checksums[i])
                    {
                        printf("Checksum for output %d is correct\n", i);
                    }
                    else
                    {
                        if (i == 0)
                        {
                            printf("Checksum for output %d is incorrect\n", i);
                        }
                    }
                    #if defined(TEST_FC_CHECKSUM_LOGGING)
                    vx_char temp[MAXPATHLENGTH];
                    printf("0x%08x\n", checksum_actual);
                    snprintf(temp, MAXPATHLENGTH, "%s/%s%d", ct_get_test_file_path(), "output/u8_fc_", i);
                    write_output_ir_image(temp, dst_image[i], 0);
                    #endif
                    }
            #endif
        }

        /* Delete the Flexconnect graph using the control command */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(tivxReleaseRawImage(&raw));
        VX_CALL(vxReleaseUserDataObject(&configuration));
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
            {
                VX_CALL(vxReleaseImage(&dst_image[i]));
            }
            #else
                VX_CALL(vxReleaseImage(&dst_image[i]));
            #endif
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseUserDataObject(&h3a_aew_af));
        VX_CALL(vxReleaseUserDataObject(&dcc_param_viss));
        VX_CALL(vxReleaseUserDataObject(&coeff_obj));
        VX_CALL(vxReleaseUserDataObject(&msc_inst_id_obj));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(graph == 0);
        ASSERT(node == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(raw == 0);
        ASSERT(configuration == 0);
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] == 0);
                }
            #else
                ASSERT(dst_image[i] == 0);
            #endif
        }
        ASSERT(ae_awb_result == 0);
        ASSERT(dcc_param_viss == 0);
        ASSERT(coeff_obj == 0);
        ASSERT(msc_inst_id_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

#define TIVX_VPAC_FC_MSC_CMD_INVALID            (0x11111110u)

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    char *target_string;
    int var_tar;

} Set_Variable_Target_Arg;

#define ADD_TEST_PARAMETERS_TARGET(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/var_tar=0", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=2", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=3", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=4", __VA_ARGS__, 4)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=5", __VA_ARGS__, 5)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=6", __VA_ARGS__, 6)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=7", __VA_ARGS__, 7)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=8", __VA_ARGS__, 8)), \
    CT_EXPAND(nextmacro(testArgName "/var_tar=9", __VA_ARGS__, 9))
#define PARAMETERS_SET_VARIABLE_TARGET_ARG \
    CT_GENERATE_PARAMETERS("target", ADD_SET_TARGET_PARAMETERS,ADD_TEST_PARAMETERS_TARGET, ARG, NULL)

TEST_WITH_ARG(tivxHwaVpacFcNegative, testTargetDrivfcNegative,Set_Variable_Target_Arg, PARAMETERS_SET_VARIABLE_TARGET_ARG)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;

    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_user_data_object h3a_aew_af = NULL;

    int dst_width[10], dst_height[10];
    vx_image dst_image[10] = {0};
    for (int i = 0; i < 10; i++)
    {
        dst_image[i] = NULL;
    }
    vx_user_data_object coeff_obj = NULL;
    vx_user_data_object msc_inst_id_obj = NULL;

    vx_user_data_object sc_input_params_obj;

    tivx_vpac_msc_coefficients_t coeffs;
    uint32_t msc_inst_id;
    tivx_vpac_msc_input_params_t sc_input_params;
    tivx_vpac_msc_output_params_t output_params[10];
    vx_enum interpolation = VX_INTERPOLATION_BILINEAR;

    vx_reference refs[2] = {0};
    vx_reference  output_refs[11] = {0};

    vx_pixel_value_t pixel;
    vx_uint32 inp_width = 1936;
    vx_uint32 inp_height = 1096;

    char file[MAXPATHLENGTH];

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;
    tivx_h3a_data_t h3a_data;

    vx_graph graph = 0;
    vx_node node = 0;

    uint32_t cnt;

    tivx_raw_image_create_params_t raw_params;

    /* Dcc objects */
    vx_user_data_object dcc_param_viss = NULL;
    const vx_char dcc_viss_user_data_object_name[] = "dcc_viss";
    vx_size dcc_buff_size = 1;
    vx_map_id dcc_viss_buf_map_id;
    uint8_t * dcc_viss_buf;
    int32_t dcc_status;
    uint32_t checksum_actual = 0;
    uint32_t sensor_dcc_id;
    uint32_t sensor_dcc_mode;
    char *sensor_name = NULL;
    char *file_name = NULL;
    uint16_t downshift_bits;

    raw_params.width = inp_width;
    raw_params.height = inp_height;
    raw_params.meta_height_after = 4;
    sensor_dcc_id = 390;
    sensor_name = SENSOR_SONY_IMX390_UB953_D3;
    sensor_dcc_mode = 0;
    file_name = "psdkra/app_single_cam/IMX390_001/input2.raw";
    downshift_bits = 0;

    /* FC input params */
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.meta_height_before = 0;

    float scale_factor[10] = {0.25, 0.25, 0.5, 0.5, 0.9, 0.9, 0.8, 0.8, 0.4, 0.4};

    /* FC output params */
    for(int i = 0; i < 10; i++)
    {
        dst_width[i] = (uint32_t)(inp_width * scale_factor[i]);

        if(i%2 == 0)
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]);
        }
        else
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]) / 2U;
        }
    }

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        /* Input Image configurations */
        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);

        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_WIDTH, &inp_width, sizeof(inp_width)));
        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_HEIGHT, &inp_height, sizeof(inp_height)));

        /* Output Image Configurations */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            #else
                if (i % 2 == 0)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                    tivx_vpac_msc_output_params_init(&output_params[i]);
                    scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
                }
            #endif
        }        

        /* Create/Configure configuration input structure */
        tivx_vpac_fc_params_init(&(params));
        
            tivx_vpac_viss_params_init(&(params.tivxVissPrms));

            ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                                sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

            /* VISS params for FC */
            params.tivxVissPrms.sensor_dcc_id = sensor_dcc_id;
            params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;
            /* YUV422 or YUV420 to be decided from chroma mode */
            params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;
            params.tivxVissPrms.h3a_aewb_af_mode = TIVX_VPAC_VISS_H3A_MODE_AEWB;
            params.tivxVissPrms.h3a_in = TIVX_VPAC_VISS_H3A_IN_LSC;        
            params.tivxVissPrms.h3a_aewb_af_mode = TIVX_VPAC_VISS_H3A_MODE_AEWB;
            params.tivxVissPrms.bypass_glbce = 1;
            params.tivxVissPrms.bypass_nsf4 = 0;
            /*
            @brief Negative test case coverage for bypass_cac
            Coverage: tivxAddKernelVpacVissValidate - (To set bypass_cac = 0)
            */
            if(arg_->var_tar != 4)
            {
                params.tivxVissPrms.bypass_cac = 1;
            }
            else
            {
                params.tivxVissPrms.bypass_cac = 0;
            }
        /* MSC params for FC */
        /* Using control commands below */

        /* FC params */
        /* If MSC0 is used then msc_in_thread_viss_out_map[0] and msc_in_thread_viss_out_map[1] must be populated  */
        /* If MSC1 is used then msc_in_thread_viss_out_map[2] and msc_in_thread_viss_out_map[3] must be populated */
        #if defined (VPAC3)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_VISS_OUT3;
        #elif defined (VPAC3L)
        if(arg_->var_tar == 1)
        {
            params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_MSC_CH_INVALID;
            params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
            params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
            params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
            
        }
        else if(arg_->var_tar == 3)
        {
            params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
            params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
            params.msc_in_thread_viss_out_map[2] = 5;
            params.msc_in_thread_viss_out_map[3] = 5;
        }
    
        else
        {
            params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
            params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
            params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
            params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        }
        #else
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #endif

        /* Mixed usage of MSC0 and MSC1 is not supported as of now */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC1;
            #elif defined(VPAC3L)
                params.msc_out_msc_in_map[i]= TIVX_VPAC_FC_MSC0;
            #else
                if (i % 2 == 0)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
            #endif
        }

        VX_CALL(vxCopyUserDataObject(configuration, 0, sizeof(tivx_vpac_fc_viss_msc_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        /* Create/Configure ae_awb_params input structure */
        tivx_ae_awb_params_init(&ae_awb_params);
        ae_awb_params.ae_valid = 1;
        ae_awb_params.exposure_time = 16666;
        ae_awb_params.analog_gain = 1030;
        ae_awb_params.awb_valid = 1;
        ae_awb_params.color_temperature = 3000;
        for (int j=0; j<4; j++)
        {
            ae_awb_params.wb_gains[j] = 525;
            ae_awb_params.wb_offsets[j] = 2;
        }

        VX_CALL(vxCopyUserDataObject(ae_awb_result, 0, sizeof(tivx_ae_awb_params_t), &ae_awb_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        /* Creating DCC */
        dcc_buff_size = appIssGetDCCSizeVISS(sensor_name, sensor_dcc_mode);

        ASSERT_VX_OBJECT(dcc_param_viss = vxCreateUserDataObject( context, (const vx_char*)&dcc_viss_user_data_object_name,
            dcc_buff_size, NULL),(enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxMapUserDataObject(
            dcc_param_viss,
            0,
            dcc_buff_size,
            &dcc_viss_buf_map_id,
            (void **)&dcc_viss_buf,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            0
        ));
        memset(dcc_viss_buf, 0xAB, dcc_buff_size);

        dcc_status = appIssGetDCCBuffVISS(sensor_name, sensor_dcc_mode, dcc_viss_buf, dcc_buff_size);
        ASSERT(dcc_status == 0);

        VX_CALL(vxUnmapUserDataObject(dcc_param_viss, dcc_viss_buf_map_id));
        /* Done w/ DCC */

        /* Creating H3A output */
        tivx_h3a_data_init(&h3a_data);
        ASSERT_VX_OBJECT(h3a_aew_af = vxCreateUserDataObject(context, "tivx_h3a_data_t", sizeof(tivx_h3a_data_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        if(NULL != h3a_aew_af)
        {
            VX_CALL(vxMapUserDataObject(h3a_aew_af,
                0,
                sizeof(tivx_h3a_data_t),
                &dcc_viss_buf_map_id,
                (void **)&dcc_viss_buf,
                (vx_enum)VX_WRITE_ONLY,
                (vx_enum)VX_MEMORY_TYPE_HOST,
                0
                ));

            memset(dcc_viss_buf, 0, sizeof(tivx_h3a_data_t));

            VX_CALL(vxUnmapUserDataObject(h3a_aew_af, dcc_viss_buf_map_id));
        }

        ASSERT(graph != NULL);
        ASSERT(configuration != NULL);
        ASSERT(ae_awb_result != NULL);
        ASSERT(raw != NULL);
        ASSERT(dcc_param_viss != NULL);
        ASSERT(h3a_aew_af != NULL);

        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] != NULL);
                }
            #else
                ASSERT(dst_image[i] != NULL);
            #endif
        }

        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ct_read_raw_image(raw, file_name, 2, downshift_bits);

        img_scale_set_coeff(&coeffs, interpolation);

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));

        /* Set MSC Coefficients */
        /*
        @brief Negative test case coverage for coeff object size
        Coverage: tivxAddKernelVpacVissValidate - For Incorrect Data Object Size
        */
        if(arg_->var_tar != 5)
        {
            ASSERT_VX_OBJECT(coeff_obj = vxCreateUserDataObject(context,
                "tivx_vpac_msc_coefficients_t",
                sizeof(tivx_vpac_msc_coefficients_t), NULL),
                (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

            VX_CALL(vxCopyUserDataObject(coeff_obj, 0,
                sizeof(tivx_vpac_msc_coefficients_t), &coeffs, VX_WRITE_ONLY,
                VX_MEMORY_TYPE_HOST));
        }
        else
        {
            ASSERT_VX_OBJECT(coeff_obj = vxCreateUserDataObject(context,
                "tivx_vpac_msc_coefficients_t",
                sizeof(tivx_vpac_msc_coefficients_t)+4, NULL),
                (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

            VX_CALL(vxCopyUserDataObject(coeff_obj, 0,
                sizeof(tivx_vpac_msc_coefficients_t)+4, &coeffs, VX_WRITE_ONLY,
                VX_MEMORY_TYPE_HOST));
        }

        #if defined(VPAC3)
            msc_inst_id = TIVX_VPAC_FC_MSC1;
        #else
            /*
            @brief Negative test case coverage 
            Coverage: tivxAddKernelVpacVissValidate - For invalid msc_inst_id
            */
            if(arg_->var_tar == 2)
            {
                msc_inst_id = TIVX_VPAC_FC_MSC_CH_INVALID;
            }
            else
            {
                msc_inst_id = TIVX_VPAC_FC_MSC0;
            }
        #endif
        /*
        @brief Negative test case coverage 
        Coverage: tivxAddKernelVpacVissValidate - For Incorrect Data Object2 Size
        */
        if(arg_->var_tar == 6)
        {
            ASSERT_VX_OBJECT(msc_inst_id_obj = vxCreateUserDataObject(context,
                "uint32_t",
                sizeof(uint16_t), NULL),
                (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

            VX_CALL(vxCopyUserDataObject(msc_inst_id_obj, 0,
                sizeof(uint16_t), &msc_inst_id, VX_WRITE_ONLY,
                VX_MEMORY_TYPE_HOST));
        }
        else
        {
            ASSERT_VX_OBJECT(msc_inst_id_obj = vxCreateUserDataObject(context,
                "uint32_t",
                sizeof(uint32_t), NULL),
                (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

            VX_CALL(vxCopyUserDataObject(msc_inst_id_obj, 0,
                sizeof(uint32_t), &msc_inst_id, VX_WRITE_ONLY,
                VX_MEMORY_TYPE_HOST));
        }

        refs[0] = (vx_reference)coeff_obj;
        refs[1] = (vx_reference)msc_inst_id_obj;
        /*
        @brief Negative test case coverage 
        Coverage: tivxAddKernelVpacVissValidate - For Invalid command id
        */
        if(arg_->var_tar == 0)
        {
            ASSERT_EQ_VX_STATUS(VX_FAILURE,
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_INVALID,
                refs, 2u));
        }
        else if((arg_->var_tar == 4) || (arg_->var_tar > 6))
        {
            ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
                refs, 2u));
        }
        else
        {
            ASSERT_EQ_VX_STATUS(VX_FAILURE,
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
                refs, 2u));
        }

        /* Process Graph */
        if((arg_->var_tar == 1) || (arg_->var_tar == 3))
        {
            ASSERT_NO_FAILURE(vxProcessGraph(graph));
            ASSERT_NO_FAILURE(vxProcessGraph(graph));
        }
        else
        {
            VX_CALL(vxProcessGraph(graph));
            VX_CALL(vxProcessGraph(graph));
        }

        /* Delete the Flexconnect graph using the control command */
        if((arg_->var_tar == 1) || (arg_->var_tar == 3))
        {
            ASSERT_EQ_VX_STATUS(VX_FAILURE,
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
                NULL, 0U));
        }
        else
        {
            ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
                NULL, 0U));
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(tivxReleaseRawImage(&raw));
        VX_CALL(vxReleaseUserDataObject(&configuration));
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
            {
                VX_CALL(vxReleaseImage(&dst_image[i]));
            }
            #else
                VX_CALL(vxReleaseImage(&dst_image[i]));
            #endif
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseUserDataObject(&h3a_aew_af));
        VX_CALL(vxReleaseUserDataObject(&dcc_param_viss));
        VX_CALL(vxReleaseUserDataObject(&coeff_obj));
        VX_CALL(vxReleaseUserDataObject(&msc_inst_id_obj));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(graph == 0);
        ASSERT(node == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(raw == 0);
        ASSERT(configuration == 0);
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] == 0);
                }
            #else
                ASSERT(dst_image[i] == 0);
            #endif
        }
        ASSERT(ae_awb_result == 0);
        ASSERT(dcc_param_viss == 0);
        ASSERT(coeff_obj == 0);
        ASSERT(msc_inst_id_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    char *target_string;
    int var;

} Set_Variable_Arg;

#define ADD_TEST_PARAMETERS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/var=0", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/var=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/var=2", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/var=3", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "/var=4", __VA_ARGS__, 4)), \
    CT_EXPAND(nextmacro(testArgName "/var=5", __VA_ARGS__, 5)), \
    CT_EXPAND(nextmacro(testArgName "/var=6", __VA_ARGS__, 6)), \
    CT_EXPAND(nextmacro(testArgName "/var=7", __VA_ARGS__, 7)), \
    CT_EXPAND(nextmacro(testArgName "/var=8", __VA_ARGS__, 8)), \
    CT_EXPAND(nextmacro(testArgName "/var=9", __VA_ARGS__, 9))
#define PARAMETERS_SET_VARIABLE_ARG \
    CT_GENERATE_PARAMETERS("target", ADD_SET_TARGET_PARAMETERS,ADD_TEST_PARAMETERS, ARG, NULL)


TEST_WITH_ARG(tivxHwaVpacFcNegative, testKernelValidate, Set_Variable_Arg, PARAMETERS_SET_VARIABLE_ARG)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;
    vx_distribution histogram=0;

    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_user_data_object h3a_aew_af = NULL;
    vx_image y12 = NULL, uv12_c1 = NULL, y8_r8_c2 = NULL, uv8_g8_c3 = NULL;
    int dst_width[10], dst_height[10];
    vx_image dst_image[10] = {0};
    for (int i = 0; i < 10; i++)
    {

        dst_image[i] = NULL;
    }
    vx_user_data_object coeff_obj = NULL;
    vx_user_data_object msc_inst_id_obj = NULL;
   
    vx_user_data_object sc_input_params_obj;

    tivx_vpac_msc_coefficients_t coeffs;
    uint32_t msc_inst_id;
    tivx_vpac_msc_input_params_t sc_input_params;
    tivx_vpac_msc_output_params_t output_params[10];
    vx_enum interpolation = VX_INTERPOLATION_BILINEAR;

    vx_reference refs[2] = {0};
    vx_reference  output_refs[11] = {0};

    vx_pixel_value_t pixel;
    vx_uint32 inp_width = 1936;
    vx_uint32 inp_height = 1096;

    char file[MAXPATHLENGTH];

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;
    tivx_h3a_data_t h3a_data;

    vx_graph graph = 0;
    vx_node node = 0;

    uint32_t cnt;

    tivx_raw_image_create_params_t raw_params;

    /* Dcc objects */
    vx_user_data_object dcc_param_viss = NULL;
    const vx_char dcc_viss_user_data_object_name[] = "dcc_viss";
    vx_size dcc_buff_size = 1;
    vx_map_id dcc_viss_buf_map_id;
    uint8_t * dcc_viss_buf;
    int32_t dcc_status;
    uint32_t checksum_actual = 0;
    uint32_t sensor_dcc_id;
    uint32_t sensor_dcc_mode;
    char *sensor_name = NULL;
    char *file_name = NULL;
    uint16_t downshift_bits;

    raw_params.width = inp_width;
    raw_params.height = inp_height;
    raw_params.meta_height_after = 4;
    sensor_dcc_id = 390;
    sensor_name = SENSOR_SONY_IMX390_UB953_D3;
    sensor_dcc_mode = 0;
    file_name = "psdkra/app_single_cam/IMX390_001/input2.raw";
    downshift_bits = 0;

    /* FC input params */
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.meta_height_before = 0;

    float scale_factor[10] = {0.25, 0.25, 0.5, 0.5, 0.9, 0.9, 0.8, 0.8, 0.4, 0.4};

    /* FC output params */
    for(int i = 0; i < 10; i++)
    {
        dst_width[i] = (uint32_t)(inp_width * scale_factor[i]);

        if(i%2 == 0)
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]);
        }
        else
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]) / 2U;
        }
    }

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        /* Input Image configurations */
        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);
        ASSERT_VX_OBJECT(y12 = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(uv12_c1 = vxCreateImage(context, 1920, 1080/2, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(y8_r8_c2 = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(uv8_g8_c3 = vxCreateImage(context, 1920, 1080/2, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        if(arg_->var == 8)
        ASSERT_VX_OBJECT(histogram = vxCreateDistribution(context, 256, 0, 256), VX_TYPE_DISTRIBUTION);

        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_WIDTH, &inp_width, sizeof(inp_width)));
        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_HEIGHT, &inp_height, sizeof(inp_height)));

        /* Output Image Configurations */
        for(int i = 0; i < 10; i++)
        {
#if defined(VPAC3) || defined(VPAC3L)
            /*
            @brief Negative test case coverage for msc_out
            Coverage: tivxAddKernelVpacVissValidate - 'msc_out0' should be an image of type and
            All MSC outputs should have the same format)
            */
            if((arg_->var != 6) && (arg_->var != 9))
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            else if(arg_->var != 9)
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_S16), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            else
            {
                if (i == 0)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_YUYV), VX_TYPE_IMAGE);
                }
                else if (i != 9)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                }
                else
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
                }
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
#else
            if (i % 2 == 0)
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
#endif
        }

        /* Create/Configure configuration input structure */
        tivx_vpac_fc_params_init(&(params));

        tivx_vpac_viss_params_init(&(params.tivxVissPrms));
        /*
        @brief Negative test case coverage for configuration
        Coverage: tivxAddKernelVpacVissValidate - configaration should be user data)
        */
        if(arg_->var != 1)
        {
            ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }
        else
        {
            ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }
        /* VISS params for FC */
        params.tivxVissPrms.sensor_dcc_id = sensor_dcc_id;
        params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;
        /* YUV422 or YUV420 to be decided from chroma mode */
        params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;

        params.tivxVissPrms.h3a_in = TIVX_VPAC_VISS_H3A_IN_LSC;        
        params.tivxVissPrms.h3a_aewb_af_mode = TIVX_VPAC_VISS_H3A_MODE_AEWB;
        params.tivxVissPrms.bypass_glbce = 1;
        params.tivxVissPrms.bypass_nsf4 = 0;
        params.tivxVissPrms.bypass_cac = 1;

        /* MSC params for FC */
        /* Using control commands below */

        /* FC params */
        /* If MSC0 is used then msc_in_thread_viss_out_map[0] and msc_in_thread_viss_out_map[1] must be populated  */
        /* If MSC1 is used then msc_in_thread_viss_out_map[2] and msc_in_thread_viss_out_map[3] must be populated */
        #if defined (VPAC3)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_VISS_OUT3;
        #elif defined (VPAC3L)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #else
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #endif

        /* Mixed usage of MSC0 and MSC1 is not supported as of now */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC1;
            #elif defined(VPAC3L)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
            #else
                if (i % 2 == 0)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
            #endif
        }
        VX_CALL(vxCopyUserDataObject(configuration, 0, sizeof(tivx_vpac_fc_viss_msc_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        /*
        @brief Negative test case coverage for ae_awb_result
        Coverage: tivxAddKernelVpacVissValidate - ae_awb_result should be user data)
        */
        if(arg_->var != 2)
        {
            ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }
        else
        {
            ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }                                                    
        /* Create/Configure ae_awb_params input structure */
        tivx_ae_awb_params_init(&ae_awb_params);
        ae_awb_params.ae_valid = 1;
        ae_awb_params.exposure_time = 16666;
        ae_awb_params.analog_gain = 1030;
        ae_awb_params.awb_valid = 1;
        ae_awb_params.color_temperature = 3000;
        for (int j=0; j<4; j++)
        {
            ae_awb_params.wb_gains[j] = 525;
            ae_awb_params.wb_offsets[j] = 2;
        }
        VX_CALL(vxCopyUserDataObject(ae_awb_result, 0, sizeof(tivx_ae_awb_params_t), &ae_awb_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST)); 

        /* Creating DCC */
        dcc_buff_size = appIssGetDCCSizeVISS(sensor_name, sensor_dcc_mode);

        ASSERT_VX_OBJECT(dcc_param_viss = vxCreateUserDataObject( context, (const vx_char*)&dcc_viss_user_data_object_name,
            dcc_buff_size, NULL),(enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxMapUserDataObject(
            dcc_param_viss,
            0,
            dcc_buff_size,
            &dcc_viss_buf_map_id,
            (void **)&dcc_viss_buf,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            0
        ));
        memset(dcc_viss_buf, 0xAB, dcc_buff_size);

        dcc_status = appIssGetDCCBuffVISS(sensor_name, sensor_dcc_mode, dcc_viss_buf, dcc_buff_size);
        ASSERT(dcc_status == 0);

        VX_CALL(vxUnmapUserDataObject(dcc_param_viss, dcc_viss_buf_map_id));
        /* Done w/ DCC */

        /* Creating H3A output */
        tivx_h3a_data_init(&h3a_data);
        /*
        @brief Negative test case coverage for h3a_out
        Coverage: tivxAddKernelVpacVissValidate - h3a_out should be user data)
        */
        if(arg_->var != 3)
        {
            ASSERT_VX_OBJECT(h3a_aew_af = vxCreateUserDataObject(context, "tivx_h3a_data_t", sizeof(tivx_h3a_data_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }
        else
        {
            ASSERT_VX_OBJECT(h3a_aew_af = vxCreateUserDataObject(context, "tivx_h3a_data_t", sizeof(tivx_h3a_data_t)+2, NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }

        if(NULL != h3a_aew_af)
        {
            VX_CALL(vxMapUserDataObject(h3a_aew_af,
                0,
                sizeof(tivx_h3a_data_t),
                &dcc_viss_buf_map_id,
                (void **)&dcc_viss_buf,
                (vx_enum)VX_WRITE_ONLY,
                (vx_enum)VX_MEMORY_TYPE_HOST,
                0
                ));

            memset(dcc_viss_buf, 0, sizeof(tivx_h3a_data_t));
            VX_CALL(vxUnmapUserDataObject(h3a_aew_af, dcc_viss_buf_map_id));
        }

        ASSERT(graph != NULL);
        ASSERT(configuration != NULL);
        ASSERT(ae_awb_result != NULL);
        ASSERT(raw != NULL);
        ASSERT(dcc_param_viss != NULL);
        ASSERT(h3a_aew_af != NULL);
        if(arg_->var == 8)
        ASSERT(histogram != NULL);

        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] != NULL);
                }
            #else

               ASSERT(dst_image[i] != NULL);

            #endif
        }
        /*
        @brief Negative test case coverage for viss_out, histogram
        Coverage: tivxAddKernelVpacVissValidate - 'viss_out' are not supported and should be NULL
        'hist1_out', and 'raw_hist_out' should be NUL)
        */
        if((arg_->var != 5)&&(arg_->var != 8))//histogram
        {
            ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }
        else if(arg_->var != 8)//viss_out
        {
            ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, y12, uv12_c1, y8_r8_c2, uv8_g8_c3,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }
        else
        {
            ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, histogram,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ct_read_raw_image(raw, file_name, 2, downshift_bits);

        img_scale_set_coeff(&coeffs, interpolation);

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));

        /* Set MSC Coefficients */

        ASSERT_VX_OBJECT(coeff_obj = vxCreateUserDataObject(context,
            "tivx_vpac_msc_coefficients_t",
            sizeof(tivx_vpac_msc_coefficients_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(coeff_obj, 0,
            sizeof(tivx_vpac_msc_coefficients_t), &coeffs, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        #if defined(VPAC3)
            msc_inst_id = TIVX_VPAC_FC_MSC1;
        #else
            msc_inst_id = TIVX_VPAC_FC_MSC0;
        #endif
        ASSERT_VX_OBJECT(msc_inst_id_obj = vxCreateUserDataObject(context,
            "uint32_t",
            sizeof(uint32_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(msc_inst_id_obj, 0,
            sizeof(uint32_t), &msc_inst_id, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        refs[0] = (vx_reference)coeff_obj;
        refs[1] = (vx_reference)msc_inst_id_obj;
        if(((arg_->var != 0)&&arg_->var != 4)&&(arg_->var != 7))
        {
            ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
            refs, 2u));
        }
        else
        {
        ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
            refs, 2u));
        }
        /* Process Graph */

        ASSERT_NO_FAILURE(vxProcessGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        
        /* Delete the Flexconnect graph using the control command */
        if(((arg_->var != 0)&&arg_->var != 4)&&(arg_->var != 7))
        {
            ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));
        }
        else
        {
        ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));
        }

        VX_CALL(tivxReleaseRawImage(&raw));
        ASSERT_NO_FAILURE(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&uv8_g8_c3));
        VX_CALL(vxReleaseImage(&y8_r8_c2));
        VX_CALL(vxReleaseImage(&uv12_c1));
        VX_CALL(vxReleaseImage(&y12));
        if(arg_->var == 8)
        {
            VX_CALL(vxReleaseDistribution(&histogram));
        }
        VX_CALL(vxReleaseUserDataObject(&configuration));
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
            {
                VX_CALL(vxReleaseImage(&dst_image[i]));
            }
            #else
                VX_CALL(vxReleaseImage(&dst_image[i]));
            #endif
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseUserDataObject(&h3a_aew_af));
        VX_CALL(vxReleaseUserDataObject(&dcc_param_viss));
        VX_CALL(vxReleaseUserDataObject(&coeff_obj));
        VX_CALL(vxReleaseUserDataObject(&msc_inst_id_obj));

        ASSERT(graph == 0);
        ASSERT(node == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(uv8_g8_c3 == 0);
        ASSERT(y8_r8_c2 == 0);
        ASSERT(uv12_c1 == 0);
        ASSERT(y12 == 0);
        ASSERT(raw == 0);
        ASSERT(configuration == 0);
        if(arg_->var == 8)
        {
            ASSERT(histogram==0);
        }
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] == 0);
                }
            #else
                ASSERT(dst_image[i] == 0);
            #endif
        }
        ASSERT(ae_awb_result == 0);
        ASSERT(dcc_param_viss == 0);
        ASSERT(coeff_obj == 0);
        ASSERT(msc_inst_id_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacFcNegative, testKernelValidateNegativeImg, Set_Variable_Arg, PARAMETERS_SET_VARIABLE_ARG)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;

    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_image y12 = NULL, uv12_c1 = NULL, y8_r8_c2 = NULL, uv8_g8_c3 = NULL;
    vx_user_data_object h3a_aew_af = NULL;

    int dst_width[10], dst_height[10];
    vx_image dst_image[10] = {0};
    for (int i = 0; i < 10; i++)
    {
        dst_image[i] = NULL;
    }
    vx_user_data_object coeff_obj = NULL;
    vx_user_data_object msc_inst_id_obj = NULL;
    
    vx_user_data_object sc_input_params_obj;

    tivx_vpac_msc_coefficients_t coeffs;
    uint32_t msc_inst_id;
    tivx_vpac_msc_input_params_t sc_input_params;
    tivx_vpac_msc_output_params_t output_params[10];
    vx_enum interpolation = VX_INTERPOLATION_BILINEAR;

    vx_reference refs[2] = {0};
    vx_reference  output_refs[11] = {0};

    vx_pixel_value_t pixel;
    vx_uint32 inp_width = 1936;
    vx_uint32 inp_height = 1096;

    char file[MAXPATHLENGTH];

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;
    tivx_h3a_data_t h3a_data;

    vx_graph graph = 0;
    vx_node node = 0;

    uint32_t cnt;

    tivx_raw_image_create_params_t raw_params;

    /* Dcc objects */
    vx_user_data_object dcc_param_viss = NULL;
    const vx_char dcc_viss_user_data_object_name[] = "dcc_viss";
    vx_size dcc_buff_size = 1;
    vx_map_id dcc_viss_buf_map_id;
    uint8_t * dcc_viss_buf;
    int32_t dcc_status;
    uint32_t checksum_actual = 0;
    vx_rectangle_t rect;
    uint32_t sensor_dcc_id;
    uint32_t sensor_dcc_mode;
    char *sensor_name = NULL;
    char *file_name = NULL;
    uint16_t downshift_bits;

    raw_params.width = inp_width;
    raw_params.height = inp_height;
    raw_params.meta_height_after = 4;
    sensor_dcc_id = 390;
    sensor_name = SENSOR_SONY_IMX390_UB953_D3;
    sensor_dcc_mode = 0;
    file_name = "psdkra/app_single_cam/IMX390_001/input2.raw";
    downshift_bits = 0;

    /* FC input params */
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.meta_height_before = 0;

    float scale_factor[10] = {0.25, 0.25, 0.5, 0.5, 0.9, 0.9, 0.8, 0.8, 0.4, 0.4};

    /* FC output params */
    for(int i = 0; i < 10; i++)
    {
        dst_width[i] = (uint32_t)(inp_width * scale_factor[i]);

        if(i%2 == 0)
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]);
        }
        else
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]) / 2U;
        }
    }

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        /* Input Image configurations */
        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);
        ASSERT_VX_OBJECT(y12 = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(uv12_c1 = vxCreateImage(context, 1920, 1080/2, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(y8_r8_c2 = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(uv8_g8_c3 = vxCreateImage(context, 1920, 1080/2, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_WIDTH, &inp_width, sizeof(inp_width)));
        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_HEIGHT, &inp_height, sizeof(inp_height)));

        /* Output Image Configurations */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
            /*
            @brief Negative test case coverage for MSC outputs
            Coverage: tivxAddKernelVpacVissValidate - 'MSC outputs should be disabled for formats and branch coverage')
            */
            if((arg_->var != 1)&&(arg_->var != 2)&&(arg_->var != 5))
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            else if((arg_->var != 2)&&(arg_->var != 5))
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], TIVX_DF_IMAGE_NV12_P12), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            else if(arg_->var != 5)
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_UYVY), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            else
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_NV12), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            #else
                if (i % 2 == 0)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                    tivx_vpac_msc_output_params_init(&output_params[i]);
                    scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
                }
            #endif
        }        

        /* Create/Configure configuration input structure */
        tivx_vpac_fc_params_init(&(params));

        tivx_vpac_viss_params_init(&(params.tivxVissPrms));

        ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        /* VISS params for FC */
        params.tivxVissPrms.sensor_dcc_id = sensor_dcc_id;
        params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;
        /* YUV422 or YUV420 to be decided from chroma mode */
        params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;

        params.tivxVissPrms.h3a_in = TIVX_VPAC_VISS_H3A_IN_LSC;        
        params.tivxVissPrms.h3a_aewb_af_mode = TIVX_VPAC_VISS_H3A_MODE_AEWB;
        params.tivxVissPrms.bypass_glbce = 1;
        params.tivxVissPrms.bypass_nsf4 = 0;
        params.tivxVissPrms.bypass_cac = 1;

        /* MSC params for FC */
        /* Using control commands below */

        /* FC params */
        /* If MSC0 is used then msc_in_thread_viss_out_map[0] and msc_in_thread_viss_out_map[1] must be populated  */
        /* If MSC1 is used then msc_in_thread_viss_out_map[2] and msc_in_thread_viss_out_map[3] must be populated */
        #if defined (VPAC3)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_VISS_OUT3;
        #elif defined (VPAC3L)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #else
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #endif

        /* Mixed usage of MSC0 and MSC1 is not supported as of now */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC1;
            #elif defined(VPAC3L)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                /*if(arg_->var == 3)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
                    else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;*/
            #else
                if (i % 2 == 0)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
            #endif
        }

        VX_CALL(vxCopyUserDataObject(configuration, 0, sizeof(tivx_vpac_fc_viss_msc_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        /* Create/Configure ae_awb_params input structure */
        tivx_ae_awb_params_init(&ae_awb_params);
        ae_awb_params.ae_valid = 1;
        ae_awb_params.exposure_time = 16666;
        ae_awb_params.analog_gain = 1030;
        ae_awb_params.awb_valid = 1;
        ae_awb_params.color_temperature = 3000;
        for (int j=0; j<4; j++)
        {
            ae_awb_params.wb_gains[j] = 525;
            ae_awb_params.wb_offsets[j] = 2;
        }

        VX_CALL(vxCopyUserDataObject(ae_awb_result, 0, sizeof(tivx_ae_awb_params_t), &ae_awb_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        /* Creating DCC */
        dcc_buff_size = appIssGetDCCSizeVISS(sensor_name, sensor_dcc_mode);

        ASSERT_VX_OBJECT(dcc_param_viss = vxCreateUserDataObject( context, (const vx_char*)&dcc_viss_user_data_object_name,
            dcc_buff_size, NULL),(enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxMapUserDataObject(
            dcc_param_viss,
            0,
            dcc_buff_size,
            &dcc_viss_buf_map_id,
            (void **)&dcc_viss_buf,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            0
        ));
        memset(dcc_viss_buf, 0xAB, dcc_buff_size);

        dcc_status = appIssGetDCCBuffVISS(sensor_name, sensor_dcc_mode, dcc_viss_buf, dcc_buff_size);
        ASSERT(dcc_status == 0);

        VX_CALL(vxUnmapUserDataObject(dcc_param_viss, dcc_viss_buf_map_id));
        /* Done w/ DCC */

        /* Creating H3A output */
        tivx_h3a_data_init(&h3a_data);
        ASSERT_VX_OBJECT(h3a_aew_af = vxCreateUserDataObject(context, "tivx_h3a_data_t", sizeof(tivx_h3a_data_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        if(NULL != h3a_aew_af)
        {
            VX_CALL(vxMapUserDataObject(h3a_aew_af,
                0,
                sizeof(tivx_h3a_data_t),
                &dcc_viss_buf_map_id,
                (void **)&dcc_viss_buf,
                (vx_enum)VX_WRITE_ONLY,
                (vx_enum)VX_MEMORY_TYPE_HOST,
                0
                ));

            memset(dcc_viss_buf, 0, sizeof(tivx_h3a_data_t));

            VX_CALL(vxUnmapUserDataObject(h3a_aew_af, dcc_viss_buf_map_id));
        }

        ASSERT(graph != NULL);
        ASSERT(configuration != NULL);
        ASSERT(ae_awb_result != NULL);
        ASSERT(raw != NULL);
        ASSERT(dcc_param_viss != NULL);
        ASSERT(h3a_aew_af != NULL);

        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] != NULL);
                }
            #else
                ASSERT(dst_image[i] != NULL);
            #endif
        }
        /*
        @brief Negative test case coverage for VISS outputs
        Coverage: tivxAddKernelVpacVissValidate - 'viss out branch coverage')
        */
        if((arg_->var != 6)&&(arg_->var != 7))
        {
        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }
        else if(arg_->var != 7)
        {
        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, uv12_c1, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }
        else
        {
            ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, y8_r8_c2, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }
        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ct_read_raw_image(raw, file_name, 2, downshift_bits);

        img_scale_set_coeff(&coeffs, interpolation);

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));

        /* Set MSC Coefficients */

        ASSERT_VX_OBJECT(coeff_obj = vxCreateUserDataObject(context,
            "tivx_vpac_msc_coefficients_t",
            sizeof(tivx_vpac_msc_coefficients_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(coeff_obj, 0,
            sizeof(tivx_vpac_msc_coefficients_t), &coeffs, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        #if defined(VPAC3)
            msc_inst_id = TIVX_VPAC_FC_MSC1;
        #else
            msc_inst_id = TIVX_VPAC_FC_MSC0;
        #endif
        ASSERT_VX_OBJECT(msc_inst_id_obj = vxCreateUserDataObject(context,
            "uint32_t",
            sizeof(uint32_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(msc_inst_id_obj, 0,
            sizeof(uint32_t), &msc_inst_id, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        refs[0] = (vx_reference)coeff_obj;
        refs[1] = (vx_reference)msc_inst_id_obj;
        ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
            refs, 2u));

        /* Process Graph */

        ASSERT_NO_FAILURE(vxProcessGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));


        /* Delete the Flexconnect graph using the control command */
        ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseImage(&uv8_g8_c3));
        VX_CALL(vxReleaseImage(&y8_r8_c2));
        VX_CALL(vxReleaseImage(&uv12_c1));
        VX_CALL(vxReleaseImage(&y12));
        VX_CALL(tivxReleaseRawImage(&raw));
        VX_CALL(vxReleaseUserDataObject(&configuration));
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
            {
                VX_CALL(vxReleaseImage(&dst_image[i]));
            }
            #else
                VX_CALL(vxReleaseImage(&dst_image[i]));
            #endif
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseUserDataObject(&h3a_aew_af));
        VX_CALL(vxReleaseUserDataObject(&dcc_param_viss));
        VX_CALL(vxReleaseUserDataObject(&coeff_obj));
        VX_CALL(vxReleaseUserDataObject(&msc_inst_id_obj));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(graph == 0);
        ASSERT(node == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(uv8_g8_c3 == 0);
        ASSERT(y8_r8_c2 == 0);
        ASSERT(uv12_c1 == 0);
        ASSERT(y12 == 0);
        ASSERT(raw == 0);
        ASSERT(configuration == 0);
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] == 0);
                }
            #else
                ASSERT(dst_image[i] == 0);
            #endif
        }
        ASSERT(ae_awb_result == 0);
        ASSERT(dcc_param_viss == 0);
        ASSERT(coeff_obj == 0);
        ASSERT(msc_inst_id_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}
/*
@brief Negative test case coverage for dcc_buffer
Coverage: tivxAddKernelVpacVissValidate - 'dcc_buffer' should be a user_data_object of name:)
*/
TEST_WITH_ARG(tivxHwaVpacFcNegative, testKernelValidateNegativeDcc, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;

    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_user_data_object h3a_aew_af = NULL;

    int dst_width[10], dst_height[10];
    vx_image dst_image[10] = {0};
    for (int i = 0; i < 10; i++)
    {
        dst_image[i] = NULL;
    }
    vx_user_data_object coeff_obj = NULL;
    vx_user_data_object msc_inst_id_obj = NULL;
    
    vx_user_data_object sc_input_params_obj;

    tivx_vpac_msc_coefficients_t coeffs;
    uint32_t msc_inst_id;
    tivx_vpac_msc_input_params_t sc_input_params;
    tivx_vpac_msc_output_params_t output_params[10];
    vx_enum interpolation = VX_INTERPOLATION_BILINEAR;

    vx_reference refs[2] = {0};
    vx_reference  output_refs[11] = {0};

    vx_pixel_value_t pixel;
    vx_uint32 inp_width = 1936;
    vx_uint32 inp_height = 1096;

    char file[MAXPATHLENGTH];

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;
    tivx_h3a_data_t h3a_data;

    vx_graph graph = 0;
    vx_node node = 0;

    uint32_t cnt;

    tivx_raw_image_create_params_t raw_params;

    /* Dcc objects */
    vx_user_data_object dcc_param_viss = NULL;
    const vx_char dcc_viss_user_data_object_name[] = "dcc_2a_viss";//dcc_buffer should be user data
    vx_size dcc_buff_size = 1;
    vx_map_id dcc_viss_buf_map_id;
    uint8_t * dcc_viss_buf;
    int32_t dcc_status;
    uint32_t checksum_actual = 0;
    vx_rectangle_t rect;
    uint32_t sensor_dcc_id;
    uint32_t sensor_dcc_mode;
    char *sensor_name = NULL;
    char *file_name = NULL;
    uint16_t downshift_bits;

    raw_params.width = inp_width;
    raw_params.height = inp_height;
    raw_params.meta_height_after = 4;
    sensor_dcc_id = 390;
    sensor_name = SENSOR_SONY_IMX390_UB953_D3;
    sensor_dcc_mode = 0;
    file_name = "psdkra/app_single_cam/IMX390_001/input2.raw";
    downshift_bits = 0;

    /* FC input params */
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.meta_height_before = 0;

    float scale_factor[10] = {0.25, 0.25, 0.5, 0.5, 0.9, 0.9, 0.8, 0.8, 0.4, 0.4};

    /* FC output params */
    for(int i = 0; i < 10; i++)
    {
        dst_width[i] = (uint32_t)(inp_width * scale_factor[i]);

        if(i%2 == 0)
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]);
        }
        else
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]) / 2U;
        }
    }

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        /* Input Image configurations */
        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);

        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_WIDTH, &inp_width, sizeof(inp_width)));
        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_HEIGHT, &inp_height, sizeof(inp_height)));

        /* Output Image Configurations */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            #else
                if (i % 2 == 0)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                    tivx_vpac_msc_output_params_init(&output_params[i]);
                    scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
                }
            #endif
        }        

        /* Create/Configure configuration input structure */
        tivx_vpac_fc_params_init(&(params));

        tivx_vpac_viss_params_init(&(params.tivxVissPrms));

        ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        /* VISS params for FC */
        params.tivxVissPrms.sensor_dcc_id = sensor_dcc_id;
        params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;
        /* YUV422 or YUV420 to be decided from chroma mode */
        params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;

        params.tivxVissPrms.h3a_in = TIVX_VPAC_VISS_H3A_IN_LSC;        
        params.tivxVissPrms.h3a_aewb_af_mode = TIVX_VPAC_VISS_H3A_MODE_AEWB;
        params.tivxVissPrms.bypass_glbce = 1;
        params.tivxVissPrms.bypass_nsf4 = 0;
        params.tivxVissPrms.bypass_cac = 1;

        /* MSC params for FC */
        /* Using control commands below */

        /* FC params */
        /* If MSC0 is used then msc_in_thread_viss_out_map[0] and msc_in_thread_viss_out_map[1] must be populated  */
        /* If MSC1 is used then msc_in_thread_viss_out_map[2] and msc_in_thread_viss_out_map[3] must be populated */
        #if defined (VPAC3)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_VISS_OUT3;
        #elif defined (VPAC3L)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #else
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #endif

        /* Mixed usage of MSC0 and MSC1 is not supported as of now */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC1;
            #elif defined(VPAC3L)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
            #else
                if (i % 2 == 0)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
            #endif
        }

        VX_CALL(vxCopyUserDataObject(configuration, 0, sizeof(tivx_vpac_fc_viss_msc_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        /* Create/Configure ae_awb_params input structure */
        tivx_ae_awb_params_init(&ae_awb_params);
        ae_awb_params.ae_valid = 1;
        ae_awb_params.exposure_time = 16666;
        ae_awb_params.analog_gain = 1030;
        ae_awb_params.awb_valid = 1;
        ae_awb_params.color_temperature = 3000;
        for (int j=0; j<4; j++)
        {
            ae_awb_params.wb_gains[j] = 525;
            ae_awb_params.wb_offsets[j] = 2;
        }

        VX_CALL(vxCopyUserDataObject(ae_awb_result, 0, sizeof(tivx_ae_awb_params_t), &ae_awb_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        /* Creating DCC */
        dcc_buff_size = appIssGetDCCSizeVISS(sensor_name, sensor_dcc_mode);

        ASSERT_VX_OBJECT(dcc_param_viss = vxCreateUserDataObject( context, (const vx_char*)&dcc_viss_user_data_object_name,
            dcc_buff_size, NULL),(enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxMapUserDataObject(
            dcc_param_viss,
            0,
            dcc_buff_size,
            &dcc_viss_buf_map_id,
            (void **)&dcc_viss_buf,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            0
        ));
        memset(dcc_viss_buf, 0xAB, dcc_buff_size);

        dcc_status = appIssGetDCCBuffVISS(sensor_name, sensor_dcc_mode, dcc_viss_buf, dcc_buff_size);
        ASSERT(dcc_status == 0);

        VX_CALL(vxUnmapUserDataObject(dcc_param_viss, dcc_viss_buf_map_id));
        /* Done w/ DCC */

        /* Creating H3A output */
        tivx_h3a_data_init(&h3a_data);
        ASSERT_VX_OBJECT(h3a_aew_af = vxCreateUserDataObject(context, "tivx_h3a_data_t", sizeof(tivx_h3a_data_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        if(NULL != h3a_aew_af)
        {
            VX_CALL(vxMapUserDataObject(h3a_aew_af,
                0,
                sizeof(tivx_h3a_data_t),
                &dcc_viss_buf_map_id,
                (void **)&dcc_viss_buf,
                (vx_enum)VX_WRITE_ONLY,
                (vx_enum)VX_MEMORY_TYPE_HOST,
                0
                ));

            memset(dcc_viss_buf, 0, sizeof(tivx_h3a_data_t));

            VX_CALL(vxUnmapUserDataObject(h3a_aew_af, dcc_viss_buf_map_id));
        }

        ASSERT(graph != NULL);
        ASSERT(configuration != NULL);
        ASSERT(ae_awb_result != NULL);
        ASSERT(raw != NULL);
        ASSERT(dcc_param_viss != NULL);
        ASSERT(h3a_aew_af != NULL);

        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] != NULL);
                }
            #else
                ASSERT(dst_image[i] != NULL);
            #endif
        }

        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ct_read_raw_image(raw, file_name, 2, downshift_bits);

        img_scale_set_coeff(&coeffs, interpolation);

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));

        /* Set MSC Coefficients */

        ASSERT_VX_OBJECT(coeff_obj = vxCreateUserDataObject(context,
            "tivx_vpac_msc_coefficients_t",
            sizeof(tivx_vpac_msc_coefficients_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(coeff_obj, 0,
            sizeof(tivx_vpac_msc_coefficients_t), &coeffs, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        #if defined(VPAC3)
            msc_inst_id = TIVX_VPAC_FC_MSC1;
        #else
            msc_inst_id = TIVX_VPAC_FC_MSC0;
        #endif
        ASSERT_VX_OBJECT(msc_inst_id_obj = vxCreateUserDataObject(context,
            "uint32_t",
            sizeof(uint32_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(msc_inst_id_obj, 0,
            sizeof(uint32_t), &msc_inst_id, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        refs[0] = (vx_reference)coeff_obj;
        refs[1] = (vx_reference)msc_inst_id_obj;
        ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
            refs, 2u));

        /* Process Graph */

        ASSERT_NO_FAILURE(vxProcessGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        /* Delete the Flexconnect graph using the control command */
        ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(tivxReleaseRawImage(&raw));
        VX_CALL(vxReleaseUserDataObject(&configuration));
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
            {
                VX_CALL(vxReleaseImage(&dst_image[i]));
            }
            #else
                VX_CALL(vxReleaseImage(&dst_image[i]));
            #endif
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseUserDataObject(&h3a_aew_af));
        VX_CALL(vxReleaseUserDataObject(&dcc_param_viss));
        VX_CALL(vxReleaseUserDataObject(&coeff_obj));
        VX_CALL(vxReleaseUserDataObject(&msc_inst_id_obj));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(graph == 0);
        ASSERT(node == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(raw == 0);
        ASSERT(configuration == 0);
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] == 0);
                }
            #else
                ASSERT(dst_image[i] == 0);
            #endif
        }
        ASSERT(ae_awb_result == 0);
        ASSERT(dcc_param_viss == 0);
        ASSERT(coeff_obj == 0);
        ASSERT(msc_inst_id_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacFcNegative, testNegative, Set_Variable_Arg, PARAMETERS_SET_VARIABLE_ARG)
{
    vx_context context = context_->vx_context_;
    vx_user_data_object configuration = NULL;

    vx_user_data_object ae_awb_result = NULL;
    tivx_raw_image raw = NULL;
    vx_user_data_object h3a_aew_af = NULL;

    int dst_width[10], dst_height[10];
    vx_image dst_image[10] = {0};
    for (int i = 0; i < 10; i++)
    {
        dst_image[i] = NULL;
    }
    vx_user_data_object coeff_obj = NULL;
    vx_user_data_object msc_inst_id_obj = NULL;
    
    vx_user_data_object sc_input_params_obj;

    tivx_vpac_msc_coefficients_t coeffs;
    uint32_t msc_inst_id;
    tivx_vpac_msc_input_params_t sc_input_params;
    tivx_vpac_msc_output_params_t output_params[10];
    vx_enum interpolation = VX_INTERPOLATION_BILINEAR;

    vx_reference refs[2] = {0};
    vx_reference  output_refs[11] = {0};

    vx_pixel_value_t pixel;
    vx_uint32 inp_width = 1936;
    vx_uint32 inp_height = 1096;

    char file[MAXPATHLENGTH];

    tivx_vpac_fc_viss_msc_params_t params;
    tivx_ae_awb_params_t ae_awb_params;
    tivx_h3a_data_t h3a_data;

    vx_graph graph = 0;
    vx_node node = 0;

    uint32_t cnt;

    tivx_raw_image_create_params_t raw_params;

    /* Dcc objects */
    vx_user_data_object dcc_param_viss = NULL;
    const vx_char dcc_viss_user_data_object_name[] = "dcc_viss";
    vx_size dcc_buff_size = 1;
    vx_map_id dcc_viss_buf_map_id;
    uint8_t * dcc_viss_buf;
    int32_t dcc_status;
    uint32_t checksum_actual = 0;
    vx_rectangle_t rect;
    uint32_t sensor_dcc_id;
    uint32_t sensor_dcc_mode;
    char *sensor_name = NULL;
    char *file_name = NULL;
    uint16_t downshift_bits;
    vx_distribution histogram = NULL;
    vx_distribution histogram1 = NULL;
    vx_distribution raw_hist = NULL;

    raw_params.width = inp_width;
    raw_params.height = inp_height;
    raw_params.meta_height_after = 4;
    sensor_dcc_id = 390;
    sensor_name = SENSOR_SONY_IMX390_UB953_D3;
    sensor_dcc_mode = 0;
    file_name = "psdkra/app_single_cam/IMX390_001/input2.raw";
    downshift_bits = 0;

    /* FC input params */
    raw_params.num_exposures = 1;
    raw_params.line_interleaved = vx_false_e;
    raw_params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
    raw_params.format[0].msb = 11;
    raw_params.meta_height_before = 0;

    float scale_factor[10] = {0.25, 0.25, 0.5, 0.5, 0.9, 0.9, 0.8, 0.8, 0.4, 0.4};

    /* FC output params */
    for(int i = 0; i < 10; i++)
    {
        dst_width[i] = (uint32_t)(inp_width * scale_factor[i]);

        if(i%2 == 0)
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]);
        }
        else
        {
            dst_height[i] = (uint32_t)(inp_height * scale_factor[i]) / 2U;
        }
    }

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        /* Input Image configurations */
        ASSERT_VX_OBJECT(raw = tivxCreateRawImage(context, &raw_params), (enum vx_type_e)TIVX_TYPE_RAW_IMAGE);
        ASSERT_VX_OBJECT(histogram = vxCreateDistribution(context, 256, 0, 256), VX_TYPE_DISTRIBUTION);
        ASSERT_VX_OBJECT(histogram1 = vxCreateDistribution(context, 256, 0, 256), VX_TYPE_DISTRIBUTION);
        ASSERT_VX_OBJECT(raw_hist = vxCreateDistribution(context, 127, 0, 128), VX_TYPE_DISTRIBUTION);

        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_WIDTH, &inp_width, sizeof(inp_width)));
        VX_CALL(tivxQueryRawImage(raw, TIVX_RAW_IMAGE_HEIGHT, &inp_height, sizeof(inp_height)));

        /* Output Image Configurations */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3) || defined(VPAC3L)
        /*
        @brief Negative test case coverage for MCU
        Coverage: tivxAddKernelVpacVissValidate - MSC output negative branch)
        */
            if((arg_->var != 2)&&(arg_->var != 3))
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            else if(arg_->var != 3)
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, 1920, 1080, VX_DF_IMAGE_NV12), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            else
            {
                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, 1920, 1080, TIVX_DF_IMAGE_NV12_P12), VX_TYPE_IMAGE);
                tivx_vpac_msc_output_params_init(&output_params[i]);
                scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
            }
            #else
                if (i % 2 == 0)
                {
                    ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, dst_width[i], dst_height[i], VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
                    tivx_vpac_msc_output_params_init(&output_params[i]);
                    scale_set_output_params(&output_params[i], interpolation, inp_width, inp_height, dst_width[i], dst_height[i]);
                }
            #endif
        }        

        /* Create/Configure configuration input structure */
        tivx_vpac_fc_params_init(&(params));

        tivx_vpac_viss_params_init(&(params.tivxVissPrms));
        /*
        @brief Negative test case coverage for configuration
        Coverage: tivxAddKernelVpacVissValidate - configuration negative branch)
        */
        if(arg_->var != 1)
        {
            ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }
        else
        {
            ASSERT_VX_OBJECT(configuration = vxCreateUserDataObject(context, "tivx_vpac_fc_viss_msc_params_t",
                                                            sizeof(tivx_vpac_fc_viss_msc_params_t)+2, NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }

        /* VISS params for FC */
        params.tivxVissPrms.sensor_dcc_id = sensor_dcc_id;
        params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;
            /* YUV422 or YUV420 to be decided from chroma mode */
            params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;
        /*
        if(arg_->var != 9)
        {
           params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_OFF;          
           params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_420;
        }
        else
        {
            params.tivxVissPrms.fcp[0].ee_mode = TIVX_VPAC_VISS_EE_MODE_FCP0_Y12;
            params.tivxVissPrms.fcp[0].chroma_mode = TIVX_VPAC_VISS_CHROMA_MODE_422;
        }
        */

        params.tivxVissPrms.h3a_in = TIVX_VPAC_VISS_H3A_IN_LSC;        
        params.tivxVissPrms.h3a_aewb_af_mode = TIVX_VPAC_VISS_H3A_MODE_AEWB;
        params.tivxVissPrms.bypass_glbce = 1;
        params.tivxVissPrms.bypass_nsf4 = 0;
        params.tivxVissPrms.bypass_cac = 1;

        /* MSC params for FC */
        /* Using control commands below */

        /* FC params */
        /* If MSC0 is used then msc_in_thread_viss_out_map[0] and msc_in_thread_viss_out_map[1] must be populated  */
        /* If MSC1 is used then msc_in_thread_viss_out_map[2] and msc_in_thread_viss_out_map[3] must be populated */
        #if defined (VPAC3)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_VISS_OUT3;
        #elif defined (VPAC3L)
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_VISS_OUT3;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #else
        params.msc_in_thread_viss_out_map[0] = TIVX_VPAC_FC_VISS_OUT2;
        params.msc_in_thread_viss_out_map[1] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[2] = TIVX_VPAC_FC_MSC_CH_INVALID;
        params.msc_in_thread_viss_out_map[3] = TIVX_VPAC_FC_MSC_CH_INVALID;
        #endif

        /* Mixed usage of MSC0 and MSC1 is not supported as of now */
        for(int i = 0; i < 10; i++)
        {
            #if defined(VPAC3)
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC1;
            #elif defined(VPAC3L)
            params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
            /*
            if (arg_->var != 9)
            {
                params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
            }
            else
            {
                if (i < 2)
                {
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                }
                else
                {
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
                }
            }
            */
            #else
                if (i % 2 == 0)
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC0;
                else
                    params.msc_out_msc_in_map[i] = TIVX_VPAC_FC_MSC_TH_INVALID;
            #endif
        }

        VX_CALL(vxCopyUserDataObject(configuration, 0, sizeof(tivx_vpac_fc_viss_msc_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(ae_awb_result = vxCreateUserDataObject(context, "tivx_ae_awb_params_t",
                                                            sizeof(tivx_ae_awb_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        /* Create/Configure ae_awb_params input structure */
        tivx_ae_awb_params_init(&ae_awb_params);
        ae_awb_params.ae_valid = 1;
        ae_awb_params.exposure_time = 16666;
        ae_awb_params.analog_gain = 1030;
        ae_awb_params.awb_valid = 1;
        ae_awb_params.color_temperature = 3000;
        for (int j=0; j<4; j++)
        {
            ae_awb_params.wb_gains[j] = 525;
            ae_awb_params.wb_offsets[j] = 2;
        }

        VX_CALL(vxCopyUserDataObject(ae_awb_result, 0, sizeof(tivx_ae_awb_params_t), &ae_awb_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        /* Creating DCC */
        dcc_buff_size = appIssGetDCCSizeVISS(sensor_name, sensor_dcc_mode);

        ASSERT_VX_OBJECT(dcc_param_viss = vxCreateUserDataObject( context, (const vx_char*)&dcc_viss_user_data_object_name,
            dcc_buff_size, NULL),(enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxMapUserDataObject(
            dcc_param_viss,
            0,
            dcc_buff_size,
            &dcc_viss_buf_map_id,
            (void **)&dcc_viss_buf,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            0
        ));
        memset(dcc_viss_buf, 0xAB, dcc_buff_size);

        dcc_status = appIssGetDCCBuffVISS(sensor_name, sensor_dcc_mode, dcc_viss_buf, dcc_buff_size);
        ASSERT(dcc_status == 0);

        VX_CALL(vxUnmapUserDataObject(dcc_param_viss, dcc_viss_buf_map_id));
        /* Done w/ DCC */

        /* Creating H3A output */
        tivx_h3a_data_init(&h3a_data);
        ASSERT_VX_OBJECT(h3a_aew_af = vxCreateUserDataObject(context, "tivx_h3a_data_t", sizeof(tivx_h3a_data_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        if(NULL != h3a_aew_af)
        {
            VX_CALL(vxMapUserDataObject(h3a_aew_af,
                0,
                sizeof(tivx_h3a_data_t),
                &dcc_viss_buf_map_id,
                (void **)&dcc_viss_buf,
                (vx_enum)VX_WRITE_ONLY,
                (vx_enum)VX_MEMORY_TYPE_HOST,
                0
                ));

            memset(dcc_viss_buf, 0, sizeof(tivx_h3a_data_t));

            VX_CALL(vxUnmapUserDataObject(h3a_aew_af, dcc_viss_buf_map_id));
        }

        ASSERT(graph != NULL);
        ASSERT(configuration != NULL);
        ASSERT(ae_awb_result != NULL);
        ASSERT(raw != NULL);
        ASSERT(dcc_param_viss != NULL);
        ASSERT(h3a_aew_af != NULL);

        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] != NULL);
                }
            #else
                ASSERT(dst_image[i] != NULL);
            #endif
        }
        /*
        @brief Negative test case coverage for histogram
        Coverage: tivxAddKernelVpacVissValidate - histogram negative branch)
        */
        if((arg_->var != 5)&&(arg_->var != 6))
        {
        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, NULL, NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }
        else if(arg_->var != 6)
        {

        ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, NULL, raw_hist,NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        } 
        else
        {
            ASSERT_VX_OBJECT(node = tivxVpacFcVissMscNode(graph, configuration, ae_awb_result, dcc_param_viss,
                                                raw, NULL, NULL, NULL, NULL,
                                                h3a_aew_af, histogram1, NULL,NULL,
                                               dst_image[0], dst_image[1], dst_image[2], dst_image[3],
                                               dst_image[4], dst_image[5], dst_image[6], dst_image[7],
                                               dst_image[8], dst_image[9]), VX_TYPE_NODE);
        }       
        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ct_read_raw_image(raw, file_name, 2, downshift_bits);

        img_scale_set_coeff(&coeffs, interpolation);

        if((arg_->var != 0)&&(arg_->var != 4)&&(arg_->var != 7)&&(arg_->var != 8)&&(arg_->var != 9))
        {
        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        }
        else
        {
        VX_CALL(vxVerifyGraph(graph));
        }

        /* Set MSC Coefficients */

        ASSERT_VX_OBJECT(coeff_obj = vxCreateUserDataObject(context,
            "tivx_vpac_msc_coefficients_t",
            sizeof(tivx_vpac_msc_coefficients_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(coeff_obj, 0,
            sizeof(tivx_vpac_msc_coefficients_t), &coeffs, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        #if defined(VPAC3)
            msc_inst_id = TIVX_VPAC_FC_MSC1;
        #else
            msc_inst_id = TIVX_VPAC_FC_MSC0;
        #endif
        ASSERT_VX_OBJECT(msc_inst_id_obj = vxCreateUserDataObject(context,
            "uint32_t",
            sizeof(uint32_t), NULL),
            (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(msc_inst_id_obj, 0,
            sizeof(uint32_t), &msc_inst_id, VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST));

        refs[0] = (vx_reference)coeff_obj;
        refs[1] = (vx_reference)msc_inst_id_obj;
        if((arg_->var != 0)&&(arg_->var != 4)&&(arg_->var != 7)&&(arg_->var != 8)&&(arg_->var != 9))
        {
        ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
            refs, 2u));
        }
        else
        {
        ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_MSC_CMD_SET_COEFF,
            refs, 2u));
        }

        /* Process Graph */

        if((arg_->var != 0)&&(arg_->var != 4)&&(arg_->var != 7)&&(arg_->var != 8)&&(arg_->var != 9))
        {
            ASSERT_NO_FAILURE(vxProcessGraph(graph));
            ASSERT_NO_FAILURE(vxProcessGraph(graph));
        }
        else
        {
            VX_CALL(vxProcessGraph(graph));
            VX_CALL(vxProcessGraph(graph));
        }
        
        

        /* Delete the Flexconnect graph using the control command */
        if((arg_->var != 0)&&(arg_->var != 4)&&(arg_->var != 7)&&(arg_->var != 8)&&(arg_->var != 9))
        {
            ASSERT_EQ_VX_STATUS(VX_FAILURE,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));
        }
        else
        {
            ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_FC_DELETE_GRAPH,
            NULL, 0U));
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseDistribution(&raw_hist));
        VX_CALL(vxReleaseDistribution(&histogram1));
        VX_CALL(vxReleaseDistribution(&histogram));
        VX_CALL(tivxReleaseRawImage(&raw));
        VX_CALL(vxReleaseUserDataObject(&configuration));
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
            if (i % 2 == 0)
            {
                VX_CALL(vxReleaseImage(&dst_image[i]));
            }
            #else
                VX_CALL(vxReleaseImage(&dst_image[i]));
            #endif
        }
        VX_CALL(vxReleaseUserDataObject(&ae_awb_result));
        VX_CALL(vxReleaseUserDataObject(&h3a_aew_af));
        VX_CALL(vxReleaseUserDataObject(&dcc_param_viss));
        VX_CALL(vxReleaseUserDataObject(&coeff_obj));
        VX_CALL(vxReleaseUserDataObject(&msc_inst_id_obj));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(graph == 0);
        ASSERT(node == 0);
        ASSERT(h3a_aew_af == 0);
        ASSERT(raw == 0);
        ASSERT(configuration == 0);
        ASSERT(raw_hist == 0);
        ASSERT(histogram1 == 0);
        ASSERT(histogram == 0);
        for (int i = 0; i < 10; i++)
        {
            #if !defined(VPAC3) && !defined(VPAC3L)
                if (i % 2 == 0)
                {
                    ASSERT(dst_image[i] == 0);
                }
            #else
                ASSERT(dst_image[i] == 0);
            #endif
        }
        ASSERT(ae_awb_result == 0);
        ASSERT(dcc_param_viss == 0);
        ASSERT(coeff_obj == 0);
        ASSERT(msc_inst_id_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}



TESTCASE_TESTS(tivxHwaVpacFc,
               testNodeCreation,
               testGraphProcessing_ten_outputs,
               testGraphProcessing_ten_outputs_dcc
               )

TESTCASE_TESTS(tivxHwaVpacFcNegative,
               testTargetDrivfcNegative,           
               testKernelValidate,
               testKernelValidateNegativeImg,
               testKernelValidateNegativeDcc,
               testNegative
               )
#endif /* Not defined for x86_64*/
#endif /* BUILD_VPAC_FC */
