/*

 * Copyright (c) 2012-2017 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifdef BUILD_VPAC_MSC

#include <VX/vx.h>
#include <VX/vxu.h>
#include <TI/tivx.h>
#include <TI/hwa_kernels.h>
#include <TI/hwa_vpac_msc.h>
#include "test_engine/test.h"
#include <string.h>

#include <math.h> // floorf
#include "test_hwa_common.h"



#if (VPAC_COUNT > 1)
#define ADD_SET_TARGET_PARAMETERS_MULTI_INST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1/TIVX_TARGET_VPAC2_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1, TIVX_TARGET_VPAC2_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1/TIVX_TARGET_VPAC2_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1, TIVX_TARGET_VPAC2_MSC2)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2/TIVX_TARGET_VPAC2_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2, TIVX_TARGET_VPAC2_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2/TIVX_TARGET_VPAC2_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2, TIVX_TARGET_VPAC2_MSC2)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC2_MSC1, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC2_MSC2, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1/TIVX_TARGET_VPAC_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1, TIVX_TARGET_VPAC_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2/TIVX_TARGET_VPAC_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2, TIVX_TARGET_VPAC_MSC2)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_MSC1/TIVX_TARGET_VPAC2_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC2_MSC1, TIVX_TARGET_VPAC2_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_MSC2/TIVX_TARGET_VPAC2_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC2_MSC2, TIVX_TARGET_VPAC2_MSC2))
#else
#define ADD_SET_TARGET_PARAMETERS_MULTI_INST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1/NULL", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2, NULL)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1/TIVX_TARGET_VPAC_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1, TIVX_TARGET_VPAC_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2/TIVX_TARGET_VPAC_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2, TIVX_TARGET_VPAC_MSC2))
#endif

TESTCASE(tivxHwaVpacMscScale, CT_VXContext, ct_setup_vx_context, 0)

typedef struct {
    const char* testName;
    char* target_string;
    int dummy;
} ArgCreate;

#if (VPAC_COUNT > 1)
#define ADD_SET_TARGET_PARAMETERS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC2_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC2_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC2_MSC2))
#else
#define ADD_SET_TARGET_PARAMETERS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC1", __VA_ARGS__, TIVX_TARGET_VPAC_MSC1)), \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_MSC2", __VA_ARGS__, TIVX_TARGET_VPAC_MSC2))
#endif

#define ADD_DUMMY(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "", __VA_ARGS__, 0))

#define PARAMETERS_CREATE \
    CT_GENERATE_PARAMETERS("instance", ADD_SET_TARGET_PARAMETERS, ADD_DUMMY, ARG)


TEST_WITH_ARG(tivxHwaVpacMscScale, testNodeCreation, ArgCreate, PARAMETERS_CREATE)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = vxScaleImageNode(graph, src_image, dst_image, VX_INTERPOLATION_NEAREST_NEIGHBOR), VX_TYPE_NODE);
        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);

        tivxHwaUnLoadKernels(context);
    }
}

typedef struct {
    const char* testName;
    int dummy;
    vx_enum interpolation;
    CT_Image (*generator)(const char* fileName, int width, int height);
    const char* fileName;
    void (*dst_size_generator)(int width, int height, int* dst_width, int* dst_height);
    int exact_result;
    int width, height;
    char* target_string;
    vx_border_t border;
} Arg;

#define STR_VX_INTERPOLATION_NEAREST_NEIGHBOR "NN"
#define STR_VX_INTERPOLATION_BILINEAR "BILINEAR"
#define STR_VX_INTERPOLATION_AREA "AREA"

#define SCALE_TEST(interpolation, inputDataGenerator, inputDataFile, scale, exact, nextmacro, ...) \
    CT_EXPAND(nextmacro(STR_##interpolation "/" inputDataFile "/" #scale, __VA_ARGS__, \
            interpolation, inputDataGenerator, inputDataFile, img_dst_size_generator_ ## scale, exact))

#define ADD_DST_SIZE_NN(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/1_1", __VA_ARGS__, img_dst_size_generator_1_1)), \
    CT_EXPAND(nextmacro(testArgName "/1_2", __VA_ARGS__, img_dst_size_generator_1_2)), \
    CT_EXPAND(nextmacro(testArgName "/2_1", __VA_ARGS__, img_dst_size_generator_2_1)), \
    CT_EXPAND(nextmacro(testArgName "/3_1", __VA_ARGS__, img_dst_size_generator_3_1)), \
    CT_EXPAND(nextmacro(testArgName "/4_1", __VA_ARGS__, img_dst_size_generator_4_1))

#define ADD_DST_SIZE_BILINEAR(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/1:1", __VA_ARGS__, img_dst_size_generator_1_1)), \
    CT_EXPAND(nextmacro(testArgName "/1:2", __VA_ARGS__, img_dst_size_generator_1_2)), \
    CT_EXPAND(nextmacro(testArgName "/2:1", __VA_ARGS__, img_dst_size_generator_2_1)), \
    CT_EXPAND(nextmacro(testArgName "/3:1", __VA_ARGS__, img_dst_size_generator_3_1)), \
    CT_EXPAND(nextmacro(testArgName "/4:1", __VA_ARGS__, img_dst_size_generator_4_1))

#define ADD_DST_SIZE_AREA(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/1:1", __VA_ARGS__, img_dst_size_generator_1_1)), \
    CT_EXPAND(nextmacro(testArgName "/87:100", __VA_ARGS__, img_dst_size_generator_87_100)), \
    CT_EXPAND(nextmacro(testArgName "/4:1", __VA_ARGS__, img_dst_size_generator_4_1))

#define ADD_SIZE_96x96(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/sz=96x96", __VA_ARGS__, 96, 96))

#define ADD_SIZE_100x100(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/sz=100x100", __VA_ARGS__, 100, 100))

#define PARAMETERS \
    /* 1:1 scale */ \
    SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 1_1, 1, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \
    SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", 1_1, 1, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \
    SCALE_TEST(VX_INTERPOLATION_AREA,             scale_generate_random, "random", 1_1, 1, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \
    /* NN upscale with integer factor */ \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 1_2, 1, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 1_3, 1, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_read_image, "lena.bmp", 1_2, 1, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* NN downscale with odd integer factor */\
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 3_1, 1, ADD_SIZE_96x96, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */\
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 5_1, 1, ADD_SIZE_100x100, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_pattern3x3, "pattern3x3", 3_1, 1, ADD_SIZE_96x96, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_read_image, "lena.bmp", 3_1, 0, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */\
    /* other NN downscales */ \
    SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 2_1, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \
    SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 4_1, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", SCALE_PYRAMID_ORB, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* BILINEAR upscale with integer factor */ \
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", 1_2, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", 1_3, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* BILINEAR downscales */ \
    /*SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", 2_1, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", 3_1, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0),*/ \
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", 4_1, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", 5_1, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", SCALE_PYRAMID_ORB, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* AREA tests */ \
    SCALE_TEST(VX_INTERPOLATION_AREA,             scale_generate_gradient_16x16, "gradient16x16", 4_1, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \
    SCALE_TEST(VX_INTERPOLATION_AREA,             scale_read_image, "lena.bmp", 4_1, 0, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \
    /* AREA upscale */ \
    /* SCALE_TEST(VX_INTERPOLATION_AREA,             scale_generate_random, "random", 1_2, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_AREA,             scale_generate_random, "random", 1_3, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* other */ \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", SCALE_NEAR_UP, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", SCALE_NEAR_UP, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_AREA,             scale_generate_random, "random", SCALE_NEAR_UP, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */ \
    /* SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", SCALE_NEAR_DOWN, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */\
    /* SCALE_TEST(VX_INTERPOLATION_BILINEAR,         scale_generate_random, "random", SCALE_NEAR_DOWN, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */\
    /* SCALE_TEST(VX_INTERPOLATION_AREA,             scale_generate_random, "random", SCALE_NEAR_DOWN, 0, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), */\

#define PARAMETERS_CC_TEST \
    /* 1:1 scale */ \
    SCALE_TEST(VX_INTERPOLATION_NEAREST_NEIGHBOR, scale_generate_random, "random", 1_1, 1, ADD_SIZE_SMALL_SET, ADD_SET_TARGET_PARAMETERS, ADD_VX_BORDERS, ARG, 0), \

TEST_WITH_ARG(tivxHwaVpacMscScale, testGraphProcessing, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    int dst_width = 0, dst_height = 0;
    vx_image src_image = 0, dst_image = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    CT_Image src = NULL, dst = NULL;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_NO_FAILURE(arg_->dst_size_generator(src->width, src->height, &dst_width, &dst_height));

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, dst_width, dst_height, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = vxScaleImageNode(graph, src_image, dst_image, arg_->interpolation), VX_TYPE_NODE);

        ASSERT_NO_FAILURE(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &arg_->border, sizeof(arg_->border)));

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

        ASSERT_NO_FAILURE(scale_check(src, dst, arg_->interpolation, arg_->border, arg_->exact_result));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));

        tivxHwaUnLoadKernels(context);
    }

    ASSERT(dst_image == 0);
    ASSERT(src_image == 0);
}

TEST_WITH_ARG(tivxHwaVpacMscScale, testImmediateProcessing, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    int dst_width = 0, dst_height = 0;
    vx_image src_image = 0, dst_image = 0;

    CT_Image src = NULL, dst = NULL;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_NO_FAILURE(arg_->dst_size_generator(src->width, src->height, &dst_width, &dst_height));

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, dst_width, dst_height, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        VX_CALL(vxSetContextAttribute(context, VX_CONTEXT_IMMEDIATE_BORDER, &arg_->border, sizeof(arg_->border)));

        ASSERT_NO_FAILURE(vxSetImmediateModeTarget(context, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxuScaleImage(context, src_image, dst_image, arg_->interpolation));

        ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

        ASSERT_NO_FAILURE(scale_check(src, dst, arg_->interpolation, arg_->border, arg_->exact_result));

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));

        tivxHwaUnLoadKernels(context);
    }

    ASSERT(dst_image == 0);
    ASSERT(src_image == 0);
}

TESTCASE_TESTS(tivxHwaVpacMscScale, 
    testNodeCreation, 
    testGraphProcessing, 
    testImmediateProcessing
    )

#endif /* BUILD_VPAC_MSC */
