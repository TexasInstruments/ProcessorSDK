/*
 *
 * Copyright (c) 2017-2018 Texas Instruments Incorporated
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
#ifdef BUILD_VPAC_NF

#include <VX/vx.h>
#include <TI/tivx.h>
#include <TI/hwa_kernels.h>
#include <TI/hwa_vpac_nf.h>
#include "test_engine/test.h"
#include <string.h>
#include "tivx_utils_checksum.h"
#include "test_hwa_common.h"
#include <inttypes.h>

#define TEST_NUM_NODE_INSTANCE 2
#define TEST_MAX_NUM_NODE_INSTANCE 4

/* #define TEST_NF_PERFORMANCE_LOGGING */
/* #define TEST_GET_ERROR_INTERRUPT_VALUES */
/* #define TEST_PRINT_TIME_STAMP_VALUES */
#define NF_NODE_ERROR_EVENT  (1U)
#define SET_TIME_STAMP_VALUE  (100U)

TESTCASE(tivxHwaVpacNfBilateral, CT_VXContext, ct_setup_vx_context, 0)
TESTCASE(tivxHwaVpacNfBilateralTarget, CT_VXContext, ct_setup_vx_context, 0)

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    char* target_string;

} SetTarget_Arg;

#if (VPAC_COUNT > 1)
#define ADD_SET_TARGET_PARAMETERS_MULTI_INST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF/TIVX_TARGET_VPAC2_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF, TIVX_TARGET_VPAC2_NF)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_NF", __VA_ARGS__, TIVX_TARGET_VPAC2_NF, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF, TIVX_TARGET_VPAC_NF)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_NF/TIVX_TARGET_VPAC2_NF", __VA_ARGS__, TIVX_TARGET_VPAC2_NF, TIVX_TARGET_VPAC2_NF))
#else
#define ADD_SET_TARGET_PARAMETERS_MULTI_INST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF/NULL", __VA_ARGS__, TIVX_TARGET_VPAC_NF, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF, TIVX_TARGET_VPAC_NF))
#endif

#if (VPAC_COUNT > 1)
#define ADD_SET_TARGET_PARAMETERS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_NF", __VA_ARGS__, TIVX_TARGET_VPAC2_NF))
#else
#define ADD_SET_TARGET_PARAMETERS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF))
#endif

#define SET_NODE_TARGET_PARAMETERS \
    CT_GENERATE_PARAMETERS("target", ADD_SET_TARGET_PARAMETERS, ARG, NULL)

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNodeCreation, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        tivx_vpac_nf_bilateral_params_init(&params);
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                            sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}


static CT_Image convolve_generate_random(const char* fileName, int width, int height)
{
    CT_Image image = NULL;

    image = ct_allocate_ct_image_random(width, height, VX_DF_IMAGE_U8, &CT()->seed_, 0, 256);
    if(NULL == image)
    {
        return NULL;
    }

    return image;
}

static CT_Image convolve_read_image(const char* fileName, int width, int height)
{
    CT_Image image = NULL;
    ASSERT_(return 0, width == 0 && height == 0);
    image = ct_read_image(fileName, 1);
    ASSERT_(return 0, image);
    ASSERT_(return 0, image->format == VX_DF_IMAGE_U8);
    return image;
}

typedef struct {
    const char* testName;
    CT_Image (*generator)(const char* fileName, int width, int height);
    const char* fileName;
    double sigma_s, sigma_r;
    vx_int32 numTables;
    vx_int32 shift;
    vx_df_image dst_format;
    vx_border_t border;
    int width, height;
    char *target_string, *target_string_2;
} Arg;
typedef struct {
    const char* testName;
    CT_Image (*generator)(const char* fileName, int width, int height);
    const char* fileName;
    double sigma_s, sigma_r;
    vx_int32 numTables;
    vx_int32 shift;
    vx_df_image dst_format;
    vx_border_t border;
    int width, height;
    int enable_safety_mechanism;
    char *target_string, *target_string_2;
} ArgSafety;

typedef struct {
    const char* testName;
    CT_Image (*generator)(const char* fileName, int width, int height);
    const char* fileName;
    double sigma_s, sigma_r;
    vx_int32 numTables;
    vx_int32 shift;
    vx_df_image dst_format;
    vx_border_t border;
    int width, height;
    int negative_test;
    int condition;
    char* target_string;
} ArgNegative;

static uint32_t nf_bilateral_checksums_ref[7*4] = {
    (uint32_t) 0xfba17165, (uint32_t) 0x98e85088, (uint32_t) 0xb912ec8e, (uint32_t) 0x9a9761de,
    (uint32_t) 0xe21b1231, (uint32_t) 0xe621c619, (uint32_t) 0xfe1fd400, (uint32_t) 0xcb17ed56,
    (uint32_t) 0xbf6d9f22, (uint32_t) 0x7ebd6360, (uint32_t) 0xa49d69ee, (uint32_t) 0xeb481747,
    (uint32_t) 0x2f3429bb, (uint32_t) 0xfe1fd3fe, (uint32_t) 0x20c52f33, (uint32_t) 0xc38e0a76,
    (uint32_t) 0x935d6a34, (uint32_t) 0x958e63e8, (uint32_t) 0x8b4e687b, (uint32_t) 0x7786a288,
    (uint32_t) 0xfe1fd3fb, (uint32_t) 0x8db682d9, (uint32_t) 0x4f60252,  (uint32_t) 0xfdcbe9d5,
    (uint32_t) 0x9e8b63d6, (uint32_t) 0x31bd5f38, (uint32_t) 0xef45f271, (uint32_t) 0xff09d3f0,
};

static uint32_t get_checksum(vx_int32 tables, vx_int32 shift)
{
    uint16_t a;
    uint16_t b;

    if (1 == tables)
    {
        a = 0U;
    }
    else if (2 == tables)
    {
        a = 1U;
    }
    else if (4 == tables)
    {
        a = 2U;
    }
    else
    {
        a = 3U;
    }

    if (0 == shift)
    {
        b = 0U;
    }
    else if (1 == shift)
    {
        b = 1U;
    }
    else if (2 == shift)
    {
        b = 2U;
    }
    else if (7 == shift)
    {
        b = 3U;
    }
    else if (-1 == shift)
    {
        b = 4U;
    }
    else if (-2 == shift)
    {
        b = 5U;
    }
    else
    {
        b = 6U;
    }

    return nf_bilateral_checksums_ref[(7U * a) + b];
}

#define ADD_SIGMAS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/sigma_s=1.0/sigma_r=128.0", __VA_ARGS__, 1.0, 128.0))

#define ADD_NUMTABLES(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/num_tables=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/num_tables=2", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/num_tables=4", __VA_ARGS__, 4)), \
    CT_EXPAND(nextmacro(testArgName "/num_tables=8", __VA_ARGS__, 8))

#define ADD_CONV_SHIFT(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=0", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=2", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=7", __VA_ARGS__, 7)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=-1", __VA_ARGS__, -1)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=-2", __VA_ARGS__, -2)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=-8", __VA_ARGS__, -8))

#define ADD_CONV_DST_FORMAT(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/dst8U", __VA_ARGS__, VX_DF_IMAGE_U8))
#if 0
, \
    CT_EXPAND(nextmacro(testArgName "/dst16S", __VA_ARGS__, VX_DF_IMAGE_S16))
#endif

#define ADD_SAFETY_MECHANISM(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/safety_mechanism=OFF", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/safety_mechanism=reconfigReinitReg", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/safety_mechanism=statusRegValidate", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/safety_mechanism=ConfigRegValidate", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "/safety_mechanism=all", __VA_ARGS__, 4))

#define PARAMETERS \
    CT_GENERATE_PARAMETERS("lena", ADD_SIGMAS, ADD_NUMTABLES, ADD_CONV_SHIFT, ADD_CONV_DST_FORMAT, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS_MULTI_INST, ARG, convolve_read_image, "lena.bmp")

#define PARAMETERS_SAFETY \
    CT_GENERATE_PARAMETERS("lena", ADD_SIGMAS, ADD_NUMTABLES, ADD_CONV_SHIFT, ADD_CONV_DST_FORMAT, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SAFETY_MECHANISM, ADD_SET_TARGET_PARAMETERS, ARG, convolve_read_image, "lena.bmp")

#define ADD_NUMTABLES_NEGATIVE(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/num_tables=1", __VA_ARGS__, 1))

#define ADD_CONV_SHIFT_NEGATIVE(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=0", __VA_ARGS__, 0))

#define ADD_NEGATIVE_TEST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/negative_test=input_interleaved", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_downshift", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_offset", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_pixel_skip", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_pixel_skip_odd", __VA_ARGS__, 4)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=kern_ln_offset", __VA_ARGS__, 5)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=kern_sz_height", __VA_ARGS__, 6)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=src_ln_inc_2", __VA_ARGS__, 7)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=adaptive_mode", __VA_ARGS__, 8)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=sub_table_select", __VA_ARGS__, 9)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=num_sigmas", __VA_ARGS__, 10))

#define ADD_NEGATIVE_CONDITION(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/condition=lower_positive", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/condition=upper_positive", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/condition=lower_negative", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/condition=upper_negative", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "/condition=middle_negative", __VA_ARGS__, 4))

#define PARAMETERS_NEGATIVE \
    CT_GENERATE_PARAMETERS("randomInput", ADD_SIGMAS, ADD_NUMTABLES_NEGATIVE, ADD_CONV_SHIFT_NEGATIVE, ADD_CONV_DST_FORMAT, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_64x64, ADD_NEGATIVE_TEST, ADD_NEGATIVE_CONDITION, ADD_SET_TARGET_PARAMETERS, ARG, convolve_generate_random, NULL)

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testGraphProcessing, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                checksum_expected = get_checksum(arg_->numTables, arg_->shift);
                checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                ASSERT(checksum_expected == checksum_actual);
            }
        }

        if ((NULL != arg_->target_string) &&
            (NULL != arg_->target_string_2) )
        {
            vx_perf_t perf_node[TEST_NUM_NODE_INSTANCE], perf_graph;

            for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
            {
                vxQueryNode(node[i], VX_NODE_PERFORMANCE, &perf_node[i], sizeof(perf_node[i]));
            }
            vxQueryGraph(graph, VX_GRAPH_PERFORMANCE, &perf_graph, sizeof(perf_graph));

            if (strncmp(arg_->target_string, arg_->target_string_2, TIVX_TARGET_MAX_NAME) == 0)
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are same\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg >= (perf_node[0].avg + perf_node[1].avg));
            }
            else
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are different\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg < (perf_node[0].avg + perf_node[1].avg));
            }
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }

        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

#if defined(VPAC3)
TEST_WITH_ARG(tivxHwaVpacNfBilateral, testGraphProcessingSafety, ArgSafety,
    PARAMETERS_SAFETY
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj = 0;
    vx_user_data_object sigma_obj = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    int j;
    uint32_t checksum_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;

    tivx_vpac_nf_safety_mechanism_params_t safety_mechanism_params;
    vx_user_data_object safety_mechanism_obj = NULL;
    vx_reference safety_ref[1] = {NULL};

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        tivx_vpac_nf_bilateral_params_init(&params);
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                            sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        tivx_vpac_nf_safety_mech_prms_init(&safety_mechanism_params);
        params.params.output_downshift = arg_->shift;

        sigmas.num_sigmas = arg_->numTables;
        for(j=0; j < arg_->numTables; j++)
        {
            sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
            sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
        }
        if(sigmas.num_sigmas > 1)
        {
            params.adaptive_mode = 1;
        }

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);


        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));
		
        VX_CALL(vxVerifyGraph(graph));
    
				
        if (0U != arg_->enable_safety_mechanism)
        {
            switch (arg_->enable_safety_mechanism)
            {
                case 1:
                    /* safety_mechanism=reconfigReinitReg */
                    safety_mechanism_params.enable_reconfig_and_reinit_Reg = 1U;
                    break;
                case 2:
                    /* safety_mechanism=statusRegValidate */
                    safety_mechanism_params.enable_status_reg_validation = 1U;
                    break;
                case 3:
                    /* safety_mechanism=ConfigRegValidate */
                    safety_mechanism_params.enable_readback_config_registers = 1U;
                    break;
                case 4:
                    /* safety_mechanism=all */
                    safety_mechanism_params.enable_reconfig_and_reinit_Reg = 1U;
                    safety_mechanism_params.enable_status_reg_validation = 1U;
                    safety_mechanism_params.enable_readback_config_registers = 1U;
                    break;
                default:
                /* Invalid value */
                break;
            }
            ASSERT_VX_OBJECT(safety_mechanism_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_safety_mechanism_params_t", sizeof(tivx_vpac_nf_safety_mechanism_params_t), &safety_mechanism_params), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
            safety_ref[0] = (vx_reference)safety_mechanism_obj;
            VX_CALL(tivxNodeSendCommand(node, 0, TIVX_VPAC_NF_CMD_ENABLE_VPAC_SAFETY_MECHANISM, safety_ref, 1));
        }

        VX_CALL(vxProcessGraph(graph));
        if(1U == arg_->enable_safety_mechanism)
        {
            /* safety_mechanism=reconfigReinitReg */
            safety_mechanism_params.enable_reconfig_and_reinit_Reg = 1U;
            VX_CALL(vxCopyUserDataObject(safety_mechanism_obj, 0, sizeof(tivx_vpac_nf_safety_mechanism_params_t), &safety_mechanism_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
            safety_ref[0] = (vx_reference)safety_mechanism_obj;
            VX_CALL(tivxNodeSendCommand(node, 0, TIVX_VPAC_NF_CMD_ENABLE_VPAC_SAFETY_MECHANISM, safety_ref, 1));
            VX_CALL(vxProcessGraph(graph));
        }
        if(0U != arg_->enable_safety_mechanism)
        {
            VX_CALL(vxReleaseUserDataObject(&safety_mechanism_obj));
        }

        checksum_expected = get_checksum(arg_->numTables, arg_->shift);
        checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);
        ASSERT(checksum_expected == checksum_actual);

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));
        ASSERT(node == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);
        ASSERT(safety_mechanism_obj == 0);

        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}
#endif

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeGraph, ArgNegative,
    PARAMETERS_NEGATIVE
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;
    int i;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                            sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_bilateral_params_init(&params);
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        params.params.output_downshift = arg_->shift;

        sigmas.num_sigmas = arg_->numTables;
        for(i=0; i < arg_->numTables; i++)
        {
            sigmas.sigma_space[i] = arg_->sigma_s + (0.2*i);
            sigmas.sigma_range[i] = arg_->sigma_r + (20*i);
        }
        if(sigmas.num_sigmas > 1)
        {
            params.adaptive_mode = 1;
        }

        switch (arg_->negative_test)
        {
            case 0:
            {
                if (0U == arg_->condition)
                {
                    params.params.input_interleaved = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.params.input_interleaved = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.params.input_interleaved = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.params.input_interleaved = 2;
                }
                else
                {
                    params.params.input_interleaved = 2;
                }
                break;
            }
            case 1:
            {
                if (0U == arg_->condition)
                {
                    params.params.output_downshift = -8;
                }
                else if (1U == arg_->condition)
                {
                    params.params.output_downshift = 7;
                }
                else if (2U == arg_->condition)
                {
                    params.params.output_downshift = -9;
                }
                else if (3U == arg_->condition)
                {
                    params.params.output_downshift = 8;
                }
                else
                {
                    params.params.output_downshift = 8;
                }
                break;
            }
            case 2:
            {
                if (0U == arg_->condition)
                {
                    params.params.output_offset = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.params.output_offset = 4095;
                }
                else if (2U == arg_->condition)
                {
                    params.params.output_offset = 4096;
                }
                else if (3U == arg_->condition)
                {
                    params.params.output_offset = 4096;
                }
                else
                {
                    params.params.output_offset = 4096;
                }
                break;
            }
            case 3:
            {
                if (0U == arg_->condition)
                {
                    params.params.output_pixel_skip = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.params.output_pixel_skip = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.params.output_pixel_skip = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.params.output_pixel_skip = 2;
                }
                else
                {
                    params.params.output_pixel_skip = 2;
                }
                break;
            }
            case 4:
            {
                if (0U == arg_->condition)
                {
                    params.params.output_pixel_skip_odd = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.params.output_pixel_skip_odd = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.params.output_pixel_skip_odd = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.params.output_pixel_skip_odd = 2;
                }
                else
                {
                    params.params.output_pixel_skip_odd = 2;
                }
                break;
            }
            case 5:
            {
                if (0U == arg_->condition)
                {
                    params.params.kern_ln_offset = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.params.kern_ln_offset = 4;
                }
                else if (2U == arg_->condition)
                {
                    params.params.kern_ln_offset = 5;
                }
                else if (3U == arg_->condition)
                {
                    params.params.kern_ln_offset = 5;
                }
                else
                {
                    params.params.kern_ln_offset = 5;
                }
                break;
            }
            case 6:
            {
                if (0U == arg_->condition)
                {
                    params.params.kern_sz_height = 1;
                }
                else if (1U == arg_->condition)
                {
                    params.params.kern_sz_height = 5;
                }
                else if (2U == arg_->condition)
                {
                    params.params.kern_sz_height = 0;
                }
                else if (3U == arg_->condition)
                {
                    params.params.kern_sz_height = 6;
                }
                else
                {
                    params.params.kern_sz_height = 6;
                }
                break;
            }
            case 7:
            {
                if (0U == arg_->condition)
                {
                    params.params.src_ln_inc_2 = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.params.src_ln_inc_2 = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.params.src_ln_inc_2 = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.params.src_ln_inc_2 = 2;
                }
                else
                {
                    params.params.src_ln_inc_2 = 2;
                }
                break;
            }
            case 8:
            {
                if (0U == arg_->condition)
                {
                    params.adaptive_mode = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.adaptive_mode = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.adaptive_mode = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.adaptive_mode = 2;
                }
                else
                {
                    params.adaptive_mode = 2;
                }
                break;
            }
            case 9:
            {
                if (0U == arg_->condition)
                {
                    params.adaptive_mode = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.adaptive_mode = 7;
                }
                else if (2U == arg_->condition)
                {
                    params.adaptive_mode = 8;
                }
                else if (3U == arg_->condition)
                {
                    params.adaptive_mode = 8;
                }
                else
                {
                    params.adaptive_mode = 8;
                }
                break;
            }
            case 10:
            {
                if (0U == arg_->condition)
                {
                    sigmas.num_sigmas = 1;
                }
                else if (1U == arg_->condition)
                {
                    sigmas.num_sigmas = 8;
                }
                else if (2U == arg_->condition)
                {
                    sigmas.num_sigmas = 0;
                }
                else if (3U == arg_->condition)
                {
                    sigmas.num_sigmas = 9;
                }
                else
                {
                    sigmas.num_sigmas = 5;
                }
                break;
            }
        }

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        if(2 > arg_->condition)
        {
            ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        }
        else
        {
            ASSERT_NE_VX_STATUS(VX_SUCCESS, vxVerifyGraph(graph));
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}
TEST_WITH_ARG(tivxHwaVpacNfBilateral, testErrorInterrupts, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj;
    vx_enum scalar_out_err_stat;
    vx_reference err_ref = 0;
    vx_reference param_ref = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Configure WDTimer with 128K cycle timeout */
                params.params.wdtimer_timeout_cycles = TIVX_VPAC_NF_WDTIMER_128K_CYCLES;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));

        /* Enable watchdog timer error events for all nodes */
        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                params.params.enable_error_events = TIVX_VPAC_NF_WDTIMER_ERR;
                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                vx_reference param_ref = (vx_reference)param_obj[i];
                tivxNodeSendCommand(node[i], 0u, TIVX_VPAC_NF_CMD_ENABLE_ERROR_EVENTS, &param_ref, 1u);
            }
        }

        /* Run first frame with watchdog timer error interrupts enabled */
        VX_CALL(vxProcessGraph(graph));

        /* Get error interrupt status values */
        ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
        VX_TYPE_UINT32 , &scalar_out_err_stat),
        (enum vx_type_e)VX_TYPE_SCALAR);

        err_ref = (vx_reference)scalar_err_stat_obj;
        VX_CALL(tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS, &err_ref, 1u));
        VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));
        #if defined TEST_GET_ERROR_INTERRUPT_VALUES
        printf("NF error status value is %d\n", scalar_out_err_stat);
        #endif
        /* Checks whether error interrupt was detected.
        *  No error is detected during normal operation. */
        ASSERT(scalar_out_err_stat == 0);
        checksum_expected = get_checksum(arg_->numTables, arg_->shift);
        checksum_actual = tivx_utils_simple_image_checksum(dst_image[0], 0, rect);
        ASSERT(checksum_expected == checksum_actual);

        /* Call the control command for maximum coverage */
        params.params.enable_error_events = 0;
        VX_CALL(vxCopyUserDataObject(param_obj[0], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params,
            VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        param_ref = (vx_reference)param_obj[0];
        VX_CALL(tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_ENABLE_ERROR_EVENTS, &param_ref, 1u));

        /* Run second frame with all error interrupts enabled, including the watchdog timer timeout errors
        *  which are disabled by default. */
        params.params.enable_error_events = TIVX_VPAC_NF_ALL_ERR_EVENTS;
        VX_CALL(vxCopyUserDataObject(param_obj[0], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params,
            VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        param_ref = (vx_reference)param_obj[0];
        VX_CALL(tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_ENABLE_ERROR_EVENTS, &param_ref, 1u));
        VX_CALL(vxProcessGraph(graph));

        VX_CALL(tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS, &err_ref, 1u));
        VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat,
                                VX_READ_ONLY, VX_MEMORY_TYPE_HOST));
        #if defined TEST_GET_ERROR_INTERRUPT_VALUES
        printf("NF error status value is %d\n", scalar_out_err_stat);
        #endif
        /* Checks whether error interrupt was detected.
        *  No error is detected during normal operation. */
        ASSERT(scalar_out_err_stat == 0);
        VX_CALL(vxReleaseScalar(&scalar_err_stat_obj));
        checksum_expected = get_checksum(arg_->numTables, arg_->shift);
        checksum_actual = tivx_utils_simple_image_checksum(dst_image[0], 0, rect);
        ASSERT(checksum_expected == checksum_actual);

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

static uint32_t nf_bilateral_psa_ref[7*4] = {
    (uint32_t) 0xcfdf673,  (uint32_t) 0x17d5c619, (uint32_t) 0xb9db3c8d, (uint32_t) 0x7c0c52f9,
    (uint32_t) 0xa3e1bb2d, (uint32_t) 0x339a9a20, (uint32_t) 0xec7984b2, (uint32_t) 0xfd699d30,
    (uint32_t) 0x8886c67,  (uint32_t) 0xe217e557, (uint32_t) 0xb02ff9bf, (uint32_t) 0xbcd9a3ca,
    (uint32_t) 0xa6520c55, (uint32_t) 0xe77e8b6a, (uint32_t) 0x7a9698d4, (uint32_t) 0xcd0de109,
    (uint32_t) 0x9d20465c, (uint32_t) 0xd8fcd87e, (uint32_t) 0x406ef454, (uint32_t) 0xf099485f,
    (uint32_t) 0x3f51463c, (uint32_t) 0xa4b5ebbf, (uint32_t) 0xe8e2c58d, (uint32_t) 0xff0a83c0,
    (uint32_t) 0x7cc3b21b, (uint32_t) 0x51b8c314, (uint32_t) 0x730e257c, (uint32_t) 0xdce80b3a
};

static uint32_t get_psa(vx_int32 tables, vx_int32 shift)
{
    uint16_t a;
    uint16_t b;

    if (1 == tables)
    {
        a = 0U;
    }
    else if (2 == tables)
    {
        a = 1U;
    }
    else if (4 == tables)
    {
        a = 2U;
    }
    else
    {
        a = 3U;
    }

    if (0 == shift)
    {
        b = 0U;
    }
    else if (1 == shift)
    {
        b = 1U;
    }
    else if (2 == shift)
    {
        b = 2U;
    }
    else if (7 == shift)
    {
        b = 3U;
    }
    else if (-1 == shift)
    {
        b = 4U;
    }
    else if (-2 == shift)
    {
        b = 5U;
    }
    else
    {
        b = 6U;
    }

    return nf_bilateral_psa_ref[(7U * a) + b];
}
TEST_WITH_ARG(tivxHwaVpacNfBilateral, testPsaSignValue, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);

        if(event.app_value == NF_NODE_ERROR_EVENT)
        {
            /* Get error interrupt status values */
            ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
            VX_TYPE_UINT32 , &scalar_out_err_stat),
            (enum vx_type_e)VX_TYPE_SCALAR);
        
            err_refs[0] = (vx_reference)scalar_err_stat_obj;
            ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
                err_refs, 1u));
            VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY,
                VX_MEMORY_TYPE_HOST));
            #if defined TEST_GET_ERROR_INTERRUPT_VALUES
            printf("NF error status value is %d\n", scalar_out_err_stat);
            #endif
            ASSERT(scalar_out_err_stat == 0);
            VX_CALL(vxReleaseScalar(&scalar_err_stat_obj)); 
        }
        else
        {
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", sizeof(tivx_vpac_nf_psa_timestamp_data_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
                psa_refs, 1u));
            
            VX_CALL(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));

            for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
            {
                if ( ((i==0) && (NULL != arg_->target_string)) ||
                    ((i==1) && (NULL != arg_->target_string_2)) )
                {
                    checksum_expected = get_checksum(arg_->numTables, arg_->shift);
                    checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                    ASSERT(checksum_expected == checksum_actual);
                    psa_expected = get_psa(arg_->numTables, arg_->shift);
                    ASSERT(psa_expected == psa_status.psa_values[0]);
                }
            }
            #if defined TEST_PRINT_TIME_STAMP_VALUES
            printf("PSA Timestamp = %" PRIu64 "\n", psa_status.timestamp);
            #endif

            if ((NULL != arg_->target_string) &&
                (NULL != arg_->target_string_2) )
            {
                vx_perf_t perf_node[TEST_NUM_NODE_INSTANCE], perf_graph;

                for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
                {
                    vxQueryNode(node[i], VX_NODE_PERFORMANCE, &perf_node[i], sizeof(perf_node[i]));
                }
                vxQueryGraph(graph, VX_GRAPH_PERFORMANCE, &perf_graph, sizeof(perf_graph));

                if (strncmp(arg_->target_string, arg_->target_string_2, TIVX_TARGET_MAX_NAME) == 0)
                {
                    #if defined(TEST_NF_PERFORMANCE_LOGGING)
                    printf("targets are same\n");
                    printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                    printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                    printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                    #endif
                    ASSERT(perf_graph.avg >= (perf_node[0].avg + perf_node[1].avg));
                }
                else
                {
                    #if defined(TEST_NF_PERFORMANCE_LOGGING)
                    printf("targets are different\n");
                    printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                    printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                    printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                    #endif
                    ASSERT(perf_graph.avg < (perf_node[0].avg + perf_node[1].avg));
                }
            }
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

#ifndef VPAC3L
TEST_WITH_ARG(tivxHwaVpacNfBilateral, testSl2Firewall, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_reference psa_refs[5] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;
    vx_status status;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        uint32_t cpu_id = APP_IPC_CPU_MCU2_0;  /* Default to MCU2_0 for VPAC0 */
        uint32_t vpacInstId = 0;  /* Default to VPAC0 */

#if defined(SOC_J784S4) || defined(SOC_J742S2)
        /* For J784S4/J742S2: VPAC1 (VPAC2_xxx targets) runs on MCU4_0 */
        if ((arg_->target_string != NULL) && (strstr(arg_->target_string, "VPAC2") != NULL))
        {
            cpu_id = APP_IPC_CPU_MCU4_0;
            vpacInstId = 1;  /* VPAC1 */
        }
#endif

		status = appRemoteServiceRun(cpu_id, APP_VHWA_VPAC_SERVICE_NAME,
                                     APP_VPAC_ENABLE_FIREWALL, &vpacInstId, sizeof(uint32_t), 0);
		ASSERT(status == 0);

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        ASSERT_VX_OBJECT(psa_obj =
            vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", sizeof(tivx_vpac_nf_psa_timestamp_data_t), NULL),
            VX_TYPE_USER_DATA_OBJECT);

        psa_refs[0] = (vx_reference) psa_obj;
        ASSERT_EQ_VX_STATUS(VX_SUCCESS,
            tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
            psa_refs, 1u));

        VX_CALL(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t),
            &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

        VX_CALL(vxReleaseUserDataObject(&psa_obj));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                ((i==1) && (NULL != arg_->target_string_2)) )
            {
                checksum_expected = get_checksum(arg_->numTables, arg_->shift);
                checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                ASSERT(checksum_expected == checksum_actual);
                psa_expected = get_psa(arg_->numTables, arg_->shift);
                ASSERT(psa_expected == psa_status.psa_values[0]);
            }
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        /* Disable firewall to restore default state */
        {
            uint32_t cpu_id = APP_IPC_CPU_MCU2_0;  /* Default to MCU2_0 for VPAC0 */
            uint32_t vpacInstId = 0;  /* Default to VPAC0 */

#if defined(SOC_J784S4) || defined(SOC_J742S2)
            /* For J784S4/J742S2: VPAC1 (VPAC2_xxx targets) runs on MCU4_0 */
            if ((arg_->target_string != NULL) && (strstr(arg_->target_string, "VPAC2") != NULL))
            {
                cpu_id = APP_IPC_CPU_MCU4_0;
                vpacInstId = 1;  /* VPAC1 */
            }
#endif

            status = appRemoteServiceRun(cpu_id, APP_VHWA_VPAC_SERVICE_NAME,
                                         APP_VPAC_DISABLE_FIREWALL, &vpacInstId, sizeof(uint32_t), 0);
            ASSERT(status == 0);
        }

        tivxHwaUnLoadKernels(context);
    }
}
#endif /* VPAC3L */

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    int condition;
    char* target_string;

} Arg_Negative_check;

#define ADD_CONDITION(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "condition_param_configuration_check=0", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "condition_input_format_check=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "condition_output_format_check=2", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "condition_sigma_configuration_check=3", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "condition_param_configuration_size_check=4", __VA_ARGS__, 4)), \
    CT_EXPAND(nextmacro(testArgName "condition_sigma_configuration_size_check=5", __VA_ARGS__, 5))

#define PARAMETERS_CONDITION \
    CT_GENERATE_PARAMETERS("testNegativeChecks", ADD_CONDITION, ADD_SET_TARGET_PARAMETERS, ARG, NULL) 

/* Negative case of PARAMETER CHECKING in tivxAddKernelVpacNfBilateralValidate function 
 * 0 : param configuration check 
 * 1 : input format check
 * 2 : output format check
 * 3 : sigma param configuration check
 * 4 : param cofiguration size check
 * 5 : sigma configutarion size check */

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeParameterChecks, Arg_Negative_check, PARAMETERS_CONDITION)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        if(arg_->condition == 1)
        {
            ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U32), VX_TYPE_IMAGE);
        }
        else
        {
            ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        }
        if(arg_->condition == 2)
        {
            ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U32), VX_TYPE_IMAGE);
        }
        else
        {
            ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        }

        memset(&params, 0, sizeof(tivx_vpac_nf_bilateral_params_t));
        
        if(arg_->condition == 0)
        {
            ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        }
        else
        {
            ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        }

        if(arg_->condition == 3)
        {
            ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }
        else
        {
            ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        }

        tivx_vpac_nf_bilateral_params_init(&params);
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        
        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

/* Negative case of PARAMETER CHECKING in tivxAddKernelVpacNfBilateralValidate function 
 * parameter size not equal to sizeof(tivx_vpac_nf_bilateral_params_t)*/

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeParameterSizeChecks, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        memset(&params, 0, sizeof(tivx_vpac_nf_bilateral_params_t));
        
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                250, NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);  
  
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        

        tivx_vpac_nf_bilateral_params_init(&params);
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        
        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

/* Negative case of PARAMETER CHECKING in tivxAddKernelVpacNfBilateralValidate function 
 * input and output format check for  VX_DF_IMAGE_U16*/

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeInputOutputFormatChecks, Arg_Negative_check, PARAMETERS_CONDITION)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        if(arg_->condition == 1)
        {
            ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        }
        else
        {
            ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        }
        if(arg_->condition == 2)
        {
            ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
        }
        else
        {
            ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        }

        tivx_vpac_nf_bilateral_params_init(&params);
        
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

/* Negative case of PARAMETER CHECKING in tivxAddKernelVpacNfBilateralValidate function 
 * sigma_size not equal to sizeof(tivx_vpac_nf_bilateral_sigmas_t) */

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeParameterChecksSigmaSize, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        tivx_vpac_nf_bilateral_params_init(&params);
        
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);

        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            250, NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT); 
        
        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}


/* Negative case of RELATIONSHIP CHECKING in tivxAddKernelVpacNfBilateralValidate function */

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeParameterRelationshipChecks, Arg_Negative_check, PARAMETERS_CONDITION)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 127, 127, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        tivx_vpac_nf_bilateral_params_init(&params);
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                            sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                    
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

/* Test case to verify NULL checks for the functions tivx_vpac_nf_bilateral_params_init and tivx_vpac_nf_bilateral_sigmas_init*/
TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeInitNullCheck, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        tivx_vpac_nf_bilateral_params_init(NULL);
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                            sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_bilateral_sigmas_init(NULL);
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                    
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}

/* Test case to cover negative case of parameter sub_table_select of tivxAddKernelVpacNfBilateralValidate function */

TEST_WITH_ARG(tivxHwaVpacNfBilateral, testNegativeParameterSubTableSelect, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj;
    vx_user_data_object sigma_obj;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        tivx_vpac_nf_bilateral_params_init(&params);
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                            sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
        ASSERT_VX_OBJECT(sigma_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                            sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        
        params.sub_table_select = 8;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        VX_CALL(vxCopyUserDataObject(sigma_obj, 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                    
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfBilateralNode(graph, param_obj, src_image, sigma_obj, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));
        VX_CALL(vxReleaseUserDataObject(&sigma_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(sigma_obj == 0);

        tivxHwaUnLoadKernels(context);
    }
}


#define ADD_SET_TARGET_PARAMETERS_MULTI_INST_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF, TIVX_TARGET_VPAC_NF))

#define ADD_SIGMAS_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/sigma_s=1.0/sigma_r=128.0", __VA_ARGS__, 1.0, 128.0))

#define ADD_NUMTABLES_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/num_tables=1", __VA_ARGS__, 1))

#define ADD_CONV_SHIFT_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=0", __VA_ARGS__, 0))

#define ADD_CONV_DST_FORMAT_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/dst8U", __VA_ARGS__, VX_DF_IMAGE_U8))
#if 0
, \
    CT_EXPAND(nextmacro(testArgName "/dst16S", __VA_ARGS__, VX_DF_IMAGE_S16))
#endif

#define ADD_NUMTABLES_ALT(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/num_tables=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/num_tables=2", __VA_ARGS__, 2))

#define PARAMETERS_CC_TEST \
    CT_GENERATE_PARAMETERS("lena", ADD_SIGMAS_CC, ADD_NUMTABLES_CC, ADD_CONV_SHIFT_CC, ADD_CONV_DST_FORMAT_CC, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS_MULTI_INST_CC, ARG, convolve_read_image, "lena.bmp")


TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testSetHtsLimitCmdNull, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference hts_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object hts_obj;
    tivx_vpac_nf_hts_bw_limit_params_t hts_bw_prms;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);

                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));

        ASSERT_VX_OBJECT(hts_obj = 
            vxCreateUserDataObject(context, "tivx_vpac_nf_hts_bw_limit_params_t", sizeof(tivx_vpac_nf_hts_bw_limit_params_t), NULL),
            VX_TYPE_USER_DATA_OBJECT);
        
        ASSERT_NO_FAILURE(vxCopyUserDataObject(hts_obj, 0, sizeof(tivx_vpac_nf_hts_bw_limit_params_t), 
            &hts_bw_prms, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
    
        hts_refs[0] = (vx_reference) NULL;
        ASSERT_NO_FAILURE(
            tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
            hts_refs, 1u));

        VX_CALL(vxReleaseUserDataObject(&hts_obj));

        VX_CALL(vxProcessGraph(graph));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}
TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testNegSetHtsLimitCmd, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference hts_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object hts_obj;
    tivx_vpac_nf_hts_bw_limit_params_t hts_bw_prms;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);

                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));

        hts_bw_prms.enable_hts_bw_limit = 1u;
        hts_bw_prms.cycle_cnt = 20000000u;
        hts_bw_prms.token_cnt = 20u;
        ASSERT_VX_OBJECT(hts_obj = 
            vxCreateUserDataObject(context, "tivx_vpac_nf_hts_bw_limit_params_t", sizeof(tivx_vpac_nf_hts_bw_limit_params_t), NULL),
            VX_TYPE_USER_DATA_OBJECT);
        
        ASSERT_NO_FAILURE(vxCopyUserDataObject(hts_obj, 0, sizeof(tivx_vpac_nf_hts_bw_limit_params_t), 
            &hts_bw_prms, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
    
        hts_refs[0] = (vx_reference) hts_obj;
        ASSERT_NO_FAILURE(
            tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
            hts_refs, 1u));

        VX_CALL(vxReleaseUserDataObject(&hts_obj));

        VX_CALL(vxProcessGraph(graph));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testSetHtsLimitCmdSize, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);
        {
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", sizeof(tivx_vpac_nf_psa_timestamp_data_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                psa_refs, 1u));
            
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testPsaSignValueSizeOf, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);

        if(event.app_value == NF_NODE_ERROR_EVENT)
        {
            /* Get error interrupt status values */
            ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
            VX_TYPE_UINT32 , &scalar_out_err_stat),
            (enum vx_type_e)VX_TYPE_SCALAR);
        
            err_refs[0] = (vx_reference)scalar_err_stat_obj;
            ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
                err_refs, 1u));
            VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY,
                VX_MEMORY_TYPE_HOST));
            #if defined TEST_GET_ERROR_INTERRUPT_VALUES
            printf("NF error status value is %d\n", scalar_out_err_stat);
            #endif
            ASSERT(scalar_out_err_stat == 0);
            VX_CALL(vxReleaseScalar(&scalar_err_stat_obj)); 
        }
        else
        {
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_hts_bw_limit_params_t", sizeof(tivx_vpac_nf_hts_bw_limit_params_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
                psa_refs, 1u));
            
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testPsaSignValueIsNull, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);

        if(event.app_value == NF_NODE_ERROR_EVENT)
        {
            /* Get error interrupt status values */
            ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
            VX_TYPE_UINT32 , &scalar_out_err_stat),
            (enum vx_type_e)VX_TYPE_SCALAR);
        
            err_refs[0] = (vx_reference)scalar_err_stat_obj;
            ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
                err_refs, 1u));
            VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY,
                VX_MEMORY_TYPE_HOST));
            #if defined TEST_GET_ERROR_INTERRUPT_VALUES
            printf("NF error status value is %d\n", scalar_out_err_stat);
            #endif
            ASSERT(scalar_out_err_stat == 0);
            VX_CALL(vxReleaseScalar(&scalar_err_stat_obj)); 
        }
        else
        {
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", sizeof(tivx_vpac_nf_psa_timestamp_data_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) NULL;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
                psa_refs, 1u));
            
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}
   
TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testSetCoeff, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);
        {
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", 5220, NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                psa_refs, 1u));
            
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}
#define PARAMETERS_CC_TEST_ALT \
    CT_GENERATE_PARAMETERS("lena", ADD_SIGMAS_CC, ADD_NUMTABLES_ALT, ADD_CONV_SHIFT_CC, ADD_CONV_DST_FORMAT_CC, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS_MULTI_INST_CC, ARG, convolve_read_image, "lena.bmp")

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testSetCoeffNull, Arg,
    PARAMETERS_CC_TEST_ALT
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);
        {
        if(arg_->numTables == 1)
        {
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", sizeof(tivx_vpac_nf_psa_timestamp_data_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                psa_refs, 1u));
            
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));
        }
        else if(arg_->numTables == 2)
        {
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", sizeof(tivx_vpac_nf_psa_timestamp_data_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) NULL;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                psa_refs, 1u));
            
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));
        }

            VX_CALL(vxReleaseUserDataObject(&psa_obj));

            for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
            {
                if ( ((i==0) && (NULL != arg_->target_string)) ||
                    ((i==1) && (NULL != arg_->target_string_2)) )
                {
                    checksum_expected = get_checksum(arg_->numTables, arg_->shift);
                    checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                    ASSERT(checksum_expected == checksum_actual);
                }
            }
            #if defined TEST_PRINT_TIME_STAMP_VALUES
            printf("PSA Timestamp = %" PRIu64 "\n", psa_status.timestamp);
            #endif

            if ((NULL != arg_->target_string) &&
                (NULL != arg_->target_string_2) )
            {
                vx_perf_t perf_node[TEST_NUM_NODE_INSTANCE], perf_graph;

                for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
                {
                    vxQueryNode(node[i], VX_NODE_PERFORMANCE, &perf_node[i], sizeof(perf_node[i]));
                }
                vxQueryGraph(graph, VX_GRAPH_PERFORMANCE, &perf_graph, sizeof(perf_graph));

                if (strncmp(arg_->target_string, arg_->target_string_2, TIVX_TARGET_MAX_NAME) == 0)
                {
                    #if defined(TEST_NF_PERFORMANCE_LOGGING)
                    printf("targets are same\n");
                    printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                    printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                    printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                    #endif
                    ASSERT(perf_graph.avg >= (perf_node[0].avg + perf_node[1].avg));
                }
                else
                {
                    #if defined(TEST_NF_PERFORMANCE_LOGGING)
                    printf("targets are different\n");
                    printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                    printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                    printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                    #endif
                    ASSERT(perf_graph.avg < (perf_node[0].avg + perf_node[1].avg));
                }
            }
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testGenerateLutCoeffs, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_actual;
    vx_rectangle_t rect;
    arg_->sigma_r = 0.0f;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                ASSERT(0X386011dd == checksum_actual);  
            }
        }

        if ((NULL != arg_->target_string) &&
            (NULL != arg_->target_string_2) )
        {
            vx_perf_t perf_node[TEST_NUM_NODE_INSTANCE], perf_graph;

            for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
            {
                vxQueryNode(node[i], VX_NODE_PERFORMANCE, &perf_node[i], sizeof(perf_node[i]));
            }
            vxQueryGraph(graph, VX_GRAPH_PERFORMANCE, &perf_graph, sizeof(perf_graph));

            if (strncmp(arg_->target_string, arg_->target_string_2, TIVX_TARGET_MAX_NAME) == 0)
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are same\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg >= (perf_node[0].avg + perf_node[1].avg));
            }
            else
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are different\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg < (perf_node[0].avg + perf_node[1].avg));
            }
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }

        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testGetErrStatus, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);

            /* Get error interrupt status values */
            ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
            VX_TYPE_UINT32 , &scalar_out_err_stat),
            (enum vx_type_e)VX_TYPE_SCALAR);
        
            err_refs[0] = (vx_reference)scalar_err_stat_obj;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
                err_refs, 1u));
            VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY,
                VX_MEMORY_TYPE_HOST));
            #if defined TEST_GET_ERROR_INTERRUPT_VALUES
            printf("NF error status value is %d\n", scalar_out_err_stat);
            #endif
            ASSERT(scalar_out_err_stat == 0);
            VX_CALL(vxReleaseScalar(&scalar_err_stat_obj)); 
        

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testGetErrStatusIsNull, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);

            /* Get error interrupt status values */
            ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
            VX_TYPE_UINT32 , &scalar_out_err_stat),
            (enum vx_type_e)VX_TYPE_SCALAR);
        
            err_refs[0] = (vx_reference)NULL;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
                err_refs, 1u));
            VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY,
                VX_MEMORY_TYPE_HOST));
            #if defined TEST_GET_ERROR_INTERRUPT_VALUES
            printf("NF error status value is %d\n", scalar_out_err_stat);
            #endif
            VX_CALL(vxReleaseScalar(&scalar_err_stat_obj)); 
        

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}
#define ADD_CONV_DST_FORMAT_CHECK(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/dst12P", __VA_ARGS__, TIVX_DF_IMAGE_P12))

#define PARAMETERS_CC_FORMAT \
    CT_GENERATE_PARAMETERS("lena", ADD_SIGMAS_CC, ADD_NUMTABLES_CC, ADD_CONV_SHIFT_CC, ADD_CONV_DST_FORMAT_CC, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS_MULTI_INST_CC, ARG, convolve_read_image, "lena.bmp")


TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testSetFmtTypes, Arg,
    PARAMETERS_CC_FORMAT
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height,TIVX_DF_IMAGE_P12), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                ASSERT(0Xd0c34480 == checksum_actual);   
            }
        }

        if ((NULL != arg_->target_string) &&
            (NULL != arg_->target_string_2) )
        {
            vx_perf_t perf_node[TEST_NUM_NODE_INSTANCE], perf_graph;

            for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
            {
                vxQueryNode(node[i], VX_NODE_PERFORMANCE, &perf_node[i], sizeof(perf_node[i]));
            }
            vxQueryGraph(graph, VX_GRAPH_PERFORMANCE, &perf_graph, sizeof(perf_graph));

            if (strncmp(arg_->target_string, arg_->target_string_2, TIVX_TARGET_MAX_NAME) == 0)
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are same\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg >= (perf_node[0].avg + perf_node[1].avg));
            }
            else
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are different\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg < (perf_node[0].avg + perf_node[1].avg));
            }
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }

        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testControlDefaultCase, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs[5] = {0};
    uint32_t psa_values[2] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);
                input_timestamp = SET_TIME_STAMP_VALUE;
                VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image[i], TIVX_REFERENCE_TIMESTAMP,
                &input_timestamp, sizeof(input_timestamp)));

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                /* Disable all events by default */
                params.params.enable_error_events = 0U;
                params.params.enable_psa = 1u;
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                /* enable events generation */
                vxEnableEvents(context);
                /* Register an error event for the node */
                ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node[0], VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        vxWaitEvent(context, &event, vx_true_e);

            /* Get error interrupt status values */
            ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
            VX_TYPE_UINT32 , &scalar_out_err_stat),
            (enum vx_type_e)VX_TYPE_SCALAR);
        
            err_refs[0] = (vx_reference)NULL;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node[0], 0u, 0,
                err_refs, 1u));
            VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY,
                VX_MEMORY_TYPE_HOST));
            #if defined TEST_GET_ERROR_INTERRUPT_VALUES
            printf("NF error status value is %d\n", scalar_out_err_stat);
            #endif
            VX_CALL(vxReleaseScalar(&scalar_err_stat_obj)); 
        

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }
        VX_CALL(vxDisableEvents(context));
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TEST_WITH_ARG(tivxHwaVpacNfBilateralTarget, testGenerateLutCoeffsSigmarZero, Arg,
    PARAMETERS_CC_TEST
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_NUM_NODE_INSTANCE] = {0};
    int i, j;
    arg_->sigma_s = 0;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                if (i==0)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                }
                else if (i==1)
                {
                    VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string_2));
                }

                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));


        if ((NULL != arg_->target_string) &&
            (NULL != arg_->target_string_2) )
        {
            vx_perf_t perf_node[TEST_NUM_NODE_INSTANCE], perf_graph;

            for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
            {
                vxQueryNode(node[i], VX_NODE_PERFORMANCE, &perf_node[i], sizeof(perf_node[i]));
            }
            vxQueryGraph(graph, VX_GRAPH_PERFORMANCE, &perf_graph, sizeof(perf_graph));

            if (strncmp(arg_->target_string, arg_->target_string_2, TIVX_TARGET_MAX_NAME) == 0)
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are same\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg >= (perf_node[0].avg + perf_node[1].avg));
            }
            else
            {
                #if defined(TEST_NF_PERFORMANCE_LOGGING)
                printf("targets are different\n");
                printf("Graph performance = %4.6f ms\n", perf_graph.avg/1000000.0);
                printf("First node performance = %4.6f ms\n", perf_node[0].avg/1000000.0);
                printf("Second node performance = %4.6f ms\n", perf_node[1].avg/1000000.0);
                #endif
                ASSERT(perf_graph.avg < (perf_node[0].avg + perf_node[1].avg));
            }
        }

        for (i = 0; i < TEST_NUM_NODE_INSTANCE; i++)
        {
            if ( ((i==0) && (NULL != arg_->target_string)) ||
                 ((i==1) && (NULL != arg_->target_string_2)) )
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }

        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}
TEST_WITH_ARG(tivxHwaVpacNfBilateral, testGraphProcessing_multi_node, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image[TEST_MAX_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    tivx_vpac_nf_bilateral_params_t params;
    tivx_vpac_nf_bilateral_sigmas_t sigmas;
    vx_user_data_object param_obj[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    vx_user_data_object sigma_obj[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    vx_graph graph = 0;
    vx_node node[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    int i, j;
    uint32_t checksum_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL;
    vx_border_t border = arg_->border;

    if (NULL != arg_->target_string)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));
    }
    if (NULL != arg_->target_string_2)
    {
        ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string_2));
    }

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));

        for (i = 0; i < TEST_MAX_NUM_NODE_INSTANCE; i++)
        {
            if ( (NULL != arg_->target_string) ||
                 (NULL != arg_->target_string_2))
            {
                ASSERT_VX_OBJECT(src_image[i] = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

                ASSERT_VX_OBJECT(dst_image[i] = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

                tivx_vpac_nf_bilateral_params_init(&params);
                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_params_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_bilateral_sigmas_init(&sigmas);
                ASSERT_VX_OBJECT(sigma_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_bilateral_sigmas_t",
                                                                    sizeof(tivx_vpac_nf_bilateral_sigmas_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

                params.params.output_downshift = arg_->shift;

                sigmas.num_sigmas = arg_->numTables;
                for(j=0; j < arg_->numTables; j++)
                {
                    sigmas.sigma_space[j] = arg_->sigma_s + (0.2*j);
                    sigmas.sigma_range[j] = arg_->sigma_r + (20*j);
                }
                if(sigmas.num_sigmas > 1)
                {
                    params.adaptive_mode = 1;
                }

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                VX_CALL(vxCopyUserDataObject(sigma_obj[i], 0, sizeof(tivx_vpac_nf_bilateral_sigmas_t), &sigmas, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

                ASSERT_VX_OBJECT(node[i] = tivxVpacNfBilateralNode(graph, param_obj[i], src_image[i], sigma_obj[i], dst_image[i]), VX_TYPE_NODE);

                
                VX_CALL(vxSetNodeTarget(node[i], VX_TARGET_STRING, arg_->target_string));
                VX_CALL(vxSetNodeAttribute(node[i], VX_NODE_BORDER, &border, sizeof(border)));
            }
        }

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        for (i = 0; i < TEST_MAX_NUM_NODE_INSTANCE; i++)
        {
            if ( (NULL != arg_->target_string) ||
                 (NULL != arg_->target_string_2))
            {
                checksum_expected = get_checksum(arg_->numTables, arg_->shift);
                checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);
                ASSERT(checksum_expected == checksum_actual);
            }
        }

        for (i = 0; i < TEST_MAX_NUM_NODE_INSTANCE; i++)
        {
            if ( (NULL != arg_->target_string) ||
                 (NULL != arg_->target_string_2))
            {
                VX_CALL(vxReleaseNode(&node[i]));
                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));
                VX_CALL(vxReleaseUserDataObject(&sigma_obj[i]));
                ASSERT(node[i] == 0);
                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);
                ASSERT(sigma_obj[i] == 0);
            }
        }

        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TESTCASE_TESTS(tivxHwaVpacNfBilateral,
               testNodeCreation,
               testGraphProcessing,
               testNegativeGraph
               #ifndef x86_64
               #if defined(VPAC3)
               ,
               testGraphProcessingSafety
               #endif
               ,
               testErrorInterrupts,
               testPsaSignValue
               #ifndef VPAC3L
               ,
               testSl2Firewall
               #endif
               #endif
               ,
               testGraphProcessing_multi_node,
               testNegativeParameterChecks,
               testNegativeParameterSizeChecks,
               testNegativeInputOutputFormatChecks,
               testNegativeParameterChecksSigmaSize,
               testNegativeParameterRelationshipChecks,
               testNegativeInitNullCheck,
               testNegativeParameterSubTableSelect
               )
TESTCASE_TESTS (tivxHwaVpacNfBilateralTarget,
                testNegSetHtsLimitCmd,
                #ifndef x86_64
                testSetHtsLimitCmdNull,
                #endif
                testSetHtsLimitCmdSize,
                testPsaSignValueSizeOf,
                testPsaSignValueIsNull,
                testSetCoeff,
                testSetCoeffNull,
                testGenerateLutCoeffs,
                #ifndef x86_64
                testGetErrStatus,
                testGetErrStatusIsNull,
                #endif
                testSetFmtTypes,
                testControlDefaultCase
                #ifndef x86_64
                ,
                testGenerateLutCoeffsSigmarZero
                #endif

)

#endif /* BUILD_VPAC_NF */
