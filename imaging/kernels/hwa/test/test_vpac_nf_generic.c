/*

 * Copyright (c) 2017 The Khronos Group Inc.
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

#define MAX_CONV_SIZE 5
/* #define TEST_GET_ERROR_INTERRUPT_VALUES */
/* #define TEST_PRINT_TIME_STAMP_VALUES */
#define NF_NODE_ERROR_EVENT  (1U)
#define SET_TIME_STAMP_VALUE  (100U)
#define TEST_MAX_NUM_NODE_INSTANCE 4


TESTCASE(tivxHwaVpacNfGeneric, CT_VXContext, ct_setup_vx_context, 0)
TESTCASE(tivxHwaVpacNfGenericTarget, CT_VXContext, ct_setup_vx_context, 0)

static vx_convolution convolution_create(vx_context context, int cols, int rows, vx_int16* data, vx_uint32 scale)
{
    vx_convolution convolution = vxCreateConvolution(context, cols, rows);
    vx_size size = 0;

    ASSERT_VX_OBJECT_(return 0, convolution, VX_TYPE_CONVOLUTION);

    VX_CALL_(return 0, vxQueryConvolution(convolution, VX_CONVOLUTION_SIZE, &size, sizeof(size)));

    VX_CALL_(return 0, vxCopyConvolutionCoefficients(convolution, data, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

    VX_CALL_(return 0, vxSetConvolutionAttribute(convolution, VX_CONVOLUTION_SCALE, &scale, sizeof(scale)));

    return convolution;
}

static void convolution_data_fill_identity(int cols, int rows, vx_int16* data)
{
    int x = cols / 2, y = rows / 2;
    ct_memset(data, 0, sizeof(vx_int16) * cols * rows);
    data[y * cols + x] = 1;
}

static void convolution_data_fill_random_128(int cols, int rows, vx_int16* data)
{
    uint64_t* seed = &CT()->seed_;
    int i;

    for (i = 0; i < cols * rows; i++)
        data[i] = (vx_uint8)CT_RNG_NEXT_INT(*seed, (uint32_t)-128, 128);
}

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    char* target_string;

} SetTarget_Arg;

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

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNodeCreation, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

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

static int32_t convolve_get(CT_Image src, int32_t x, int32_t y, vx_border_t border,
        int cols, int rows, vx_int16* data, vx_int32 shift, vx_df_image dst_format)
{
    int i;
    int32_t sum = 0, datasum = 0, value = 0;
    int32_t src_data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };

    ASSERT_(return 0, cols <= MAX_CONV_SIZE);
    ASSERT_(return 0, rows <= MAX_CONV_SIZE);

    ASSERT_NO_FAILURE_(return 0,
            ct_image_read_rect_S32(src, src_data, x - cols / 2, y - rows / 2, x + cols / 2, y + rows / 2, border));

    for (i = 0; i < cols * rows; ++i) {
        sum += src_data[i] * data[cols * rows - 1 - i];
        datasum += data[cols * rows - 1 - i];
    }

    if ( shift > 0 ) {
        value = ((sum >> shift) / datasum) ;
    } else if ( shift < 0 ) {
        value = ((sum << (-1*shift)) / datasum) ;
    } else {
        value = (sum / datasum);
    }

    if (dst_format == VX_DF_IMAGE_U8)
    {
        if (value < 0) value = 0;
        else if (value > UINT8_MAX) value = UINT8_MAX;
    }
    else if (dst_format == VX_DF_IMAGE_S16)
    {
        if (value < INT16_MIN) value = INT16_MIN;
        else if (value > INT16_MAX) value = INT16_MAX;
    }

    return value;
}


static CT_Image convolve_create_reference_image(CT_Image src, vx_border_t border,
        int cols, int rows, vx_int16* data, vx_int32 shift, vx_df_image dst_format)
{
    CT_Image dst;

    CT_ASSERT_(return NULL, src->format == VX_DF_IMAGE_U8);

    dst = ct_allocate_image(src->width, src->height, dst_format);

    if (dst_format == VX_DF_IMAGE_U8)
    {
        CT_FILL_IMAGE_8U(return 0, dst,
                {
                    int32_t res = convolve_get(src, x, y, border, cols, rows, data, shift, dst_format);
                    *dst_data = (vx_uint8)res;
                });
    }
    else if (dst_format == VX_DF_IMAGE_S16)
    {
        CT_FILL_IMAGE_16S(return 0, dst,
                {
                    int32_t res = convolve_get(src, x, y, border, cols, rows, data, shift, dst_format);
                    *dst_data = (vx_int16)res;
                });
    }
    else
    {
        CT_FAIL_(return 0, "NOT IMPLEMENTED");
    }
    return dst;
}


static void convolve_check(CT_Image src, CT_Image dst, vx_border_t border,
        int cols, int rows, vx_int16* data, vx_int32 shift, vx_df_image dst_format)
{
    CT_Image dst_ref = NULL;

    ASSERT(src && dst);

    ASSERT_NO_FAILURE(dst_ref = convolve_create_reference_image(src, border, cols, rows, data, shift, dst_format));

    ASSERT_NO_FAILURE(
        if (border.mode == VX_BORDER_UNDEFINED)
        {
            ct_adjust_roi(dst, cols / 2, rows / 2, cols / 2, rows / 2);
            ct_adjust_roi(dst_ref, cols / 2, rows / 2, cols / 2, rows / 2);
        }
    );

    EXPECT_CTIMAGE_NEAR(dst_ref, dst, 1);
#if 0
    if (CT_HasFailure())
    {
        printf("=== SRC ===\n");
        ct_dump_image_info_ex(src, 16, 4);
        printf("=== DST ===\n");
        ct_dump_image_info_ex(dst, 16, 4);
        printf("=== EXPECTED ===\n");
        ct_dump_image_info_ex(dst_ref, 16, 4);
    }
#endif
}

typedef struct {
    const char* testName;
    CT_Image (*generator)(const char* fileName, int width, int height);
    const char* fileName;
    int cols, rows;
    vx_int32 shift;
    void (*convolution_data_generator)(int cols, int rows, vx_int16* data);
    vx_df_image dst_format;
    vx_border_t border;
    int width, height;
    char* target_string, *target_string_2;
} Arg;
typedef struct {
    const char* testName;
    CT_Image (*generator)(const char* fileName, int width, int height);
    const char* fileName;
    int cols, rows;
    vx_int32 shift;
    void (*convolution_data_generator)(int cols, int rows, vx_int16* data);
    vx_df_image dst_format;
    vx_border_t border;
    int width, height;
    int enable_safety_mechanism;
    char* target_string, *target_string_2;
} ArgSafety;

typedef struct {
    const char* testName;
    CT_Image (*generator)(const char* fileName, int width, int height);
    const char* fileName;
    int cols, rows;
    vx_int32 shift;
    void (*convolution_data_generator)(int cols, int rows, vx_int16* data);
    vx_df_image dst_format;
    vx_border_t border;
    int width, height;
    int negative_test;
    int condition;
    char* target_string;
} ArgNegative;

static uint32_t nf_generic_checksums_ref[4*7*2] = {
    (uint32_t) 0x41dd742d, (uint32_t) 0x710babf1, (uint32_t) 0x681bb594, (uint32_t) 0x7411b1b8,
    (uint32_t) 0xf1876e3a, (uint32_t) 0xe27dc06c, (uint32_t) 0xe8d09420, (uint32_t) 0x73735ed8,
    (uint32_t) 0x6cb023a,  (uint32_t) 0xbe5e681d, (uint32_t) 0xc26f1bb8, (uint32_t) 0x93343528,
    (uint32_t) 0x4fedc2e,  (uint32_t) 0xfffed400, (uint32_t) 0x41dd742d, (uint32_t) 0xb33d7506,
    (uint32_t) 0x681bb594, (uint32_t) 0xf3c46a44, (uint32_t) 0xf1876e3a, (uint32_t) 0xcc99df7e,
    (uint32_t) 0xe8d09420, (uint32_t) 0x477b3f8c, (uint32_t) 0x6cb023a,  (uint32_t) 0x27a0a3b2,
    (uint32_t) 0xc26f1bb8, (uint32_t) 0xa42111cf, (uint32_t) 0x4fedc2e,  (uint32_t) 0xfffed400,
    (uint32_t) 0x41dd742d, (uint32_t) 0x7f9344c2, (uint32_t) 0x681bb594, (uint32_t) 0xceeccc5f,
    (uint32_t) 0xf1876e3a, (uint32_t) 0x8f57d6fd, (uint32_t) 0xe8d09420, (uint32_t) 0x637011a3,
    (uint32_t) 0x6cb023a,  (uint32_t) 0x8f4ec3cc, (uint32_t) 0xc26f1bb8, (uint32_t) 0x6234152d,
    (uint32_t) 0x4fedc2e,  (uint32_t) 0xfffed400, (uint32_t) 0x41dd742d, (uint32_t) 0x3482f1e5,
    (uint32_t) 0x681bb594, (uint32_t) 0xd3664389, (uint32_t) 0xf1876e3a, (uint32_t) 0xa8dcc146,
    (uint32_t) 0xe8d09420, (uint32_t) 0x41552d94, (uint32_t) 0x6cb023a,  (uint32_t) 0x798ec272,
    (uint32_t) 0xc26f1bb8, (uint32_t) 0x639d96ed, (uint32_t) 0x4fedc2e,  (uint32_t) 0xfffed400
};

static uint32_t get_checksum(vx_int32 cols, vx_int32 rows, vx_int32 shift, void (*convolution_data_generator)(int cols, int rows, vx_int16* data))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;

    if ((3 == cols) && (3 == rows))
    {
        a = 0U;
    }
    else if ((5 == cols) && (3 == rows))
    {
        a = 1U;
    }
    else if ((3 == cols) && (5 == rows))
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

    if (convolution_data_fill_identity == convolution_data_generator)
    {
        c = 0u;
    }
    else
    {
        c = 1u;
    }

    return nf_generic_checksums_ref[7*2*a+2*b+c];
}

#define ADD_CONV_SIZE(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv=3x3", __VA_ARGS__, 3, 3)), \
    CT_EXPAND(nextmacro(testArgName "/conv=5x3", __VA_ARGS__, 5, 3)), \
    CT_EXPAND(nextmacro(testArgName "/conv=3x5", __VA_ARGS__, 3, 5)), \
    CT_EXPAND(nextmacro(testArgName "/conv=5x5", __VA_ARGS__, 5, 5))

#define ADD_CONV_SHIFT(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=0", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=2", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=7", __VA_ARGS__, 7)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=-1", __VA_ARGS__, -1)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=-2", __VA_ARGS__, -2)), \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=-8", __VA_ARGS__, -8))

#define ADD_CONV_GENERATORS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_fill=identity", __VA_ARGS__, convolution_data_fill_identity)), \
    CT_EXPAND(nextmacro(testArgName "/conv_fill=random128", __VA_ARGS__, convolution_data_fill_random_128))

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
    CT_GENERATE_PARAMETERS("lena", ADD_CONV_SIZE, ADD_CONV_SHIFT, ADD_CONV_GENERATORS, ADD_CONV_DST_FORMAT, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS, ARG, convolve_read_image, "lena.bmp")

#define PARAMETERS_SAFETY \
    CT_GENERATE_PARAMETERS("lena", ADD_CONV_SIZE, ADD_CONV_SHIFT, ADD_CONV_GENERATORS, ADD_CONV_DST_FORMAT, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SAFETY_MECHANISM, ADD_SET_TARGET_PARAMETERS, ARG, convolve_read_image, "lena.bmp")

#define ADD_CONV_SIZE_NEGATIVE(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv=5x5", __VA_ARGS__, 5, 5))

#define ADD_CONV_GENERATORS_NEGATIVE(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_fill=identity", __VA_ARGS__, convolution_data_fill_identity))

#define ADD_NEGATIVE_TEST(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/negative_test=input_interleaved", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_downshift", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_offset", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_pixel_skip", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=output_pixel_skip_odd", __VA_ARGS__, 4)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=kern_ln_offset", __VA_ARGS__, 5)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=kern_sz_height", __VA_ARGS__, 6)), \
    CT_EXPAND(nextmacro(testArgName "/negative_test=src_ln_inc_2", __VA_ARGS__, 7))

#define ADD_NEGATIVE_CONDITION(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/condition=lower_positive", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/condition=upper_positive", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/condition=lower_negative", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/condition=upper_negative", __VA_ARGS__, 3)), \
    CT_EXPAND(nextmacro(testArgName "/condition=middle_negative", __VA_ARGS__, 4))

#define PARAMETERS_NEGATIVE \
    CT_GENERATE_PARAMETERS("testNegative", ADD_CONV_SIZE_NEGATIVE, ADD_CONV_SHIFT, ADD_CONV_GENERATORS_NEGATIVE, ADD_CONV_DST_FORMAT, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_64x64, ADD_NEGATIVE_TEST, ADD_NEGATIVE_CONDITION, ADD_SET_TARGET_PARAMETERS, ARG, convolve_generate_random, NULL)

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testGraphProcessing, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

        ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

        if (arg_->convolution_data_generator == convolution_data_fill_identity)
        {
            checksum_expected = get_checksum(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
            checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

            ASSERT(checksum_expected == checksum_actual);
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

#if defined(VPAC3)
TEST_WITH_ARG(tivxHwaVpacNfGeneric, testGraphProcessingSafety, ArgSafety,
    PARAMETERS_SAFETY
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;

    tivx_vpac_nf_safety_mechanism_params_t safety_mechanism_params;
    vx_user_data_object safety_mechanism_obj = NULL;
    vx_reference safety_ref[1] = { NULL };

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        tivx_vpac_nf_safety_mech_prms_init(&safety_mechanism_params);

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

        ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

        ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

        if (arg_->convolution_data_generator == convolution_data_fill_identity)
        {
            checksum_expected = get_checksum(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
            checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

            ASSERT(checksum_expected == checksum_actual);
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);
        ASSERT(safety_mechanism_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}
#endif

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNegativeGraph, ArgNegative,
    PARAMETERS_NEGATIVE
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;

        switch (arg_->negative_test)
        {
            case 0:
            {
                if (0U == arg_->condition)
                {
                    params.input_interleaved = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.input_interleaved = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.input_interleaved = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.input_interleaved = 2;
                }
                else
                {
                    params.input_interleaved = 2;
                }
                break;
            }
            case 1:
            {
                if (0U == arg_->condition)
                {
                    params.output_downshift = -8;
                }
                else if (1U == arg_->condition)
                {
                    params.output_downshift = 7;
                }
                else if (2U == arg_->condition)
                {
                    params.output_downshift = -9;
                }
                else if (3U == arg_->condition)
                {
                    params.output_downshift = 8;
                }
                else
                {
                    params.output_downshift = 8;
                }
                break;
            }
            case 2:
            {
                if (0U == arg_->condition)
                {
                    params.output_offset = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.output_offset = 4095;
                }
                else if (2U == arg_->condition)
                {
                    params.output_offset = 4096;
                }
                else if (3U == arg_->condition)
                {
                    params.output_offset = 4096;
                }
                else
                {
                    params.output_offset = 4096;
                }
                break;
            }
            case 3:
            {
                if (0U == arg_->condition)
                {
                    params.output_pixel_skip = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.output_pixel_skip = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.output_pixel_skip = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.output_pixel_skip = 2;
                }
                else
                {
                    params.output_pixel_skip = 2;
                }
                break;
            }
            case 4:
            {
                if (0U == arg_->condition)
                {
                    params.output_pixel_skip_odd = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.output_pixel_skip_odd = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.output_pixel_skip_odd = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.output_pixel_skip_odd = 2;
                }
                else
                {
                    params.output_pixel_skip_odd = 2;
                }
                break;
            }
            case 5:
            {
                if (0U == arg_->condition)
                {
                    params.kern_ln_offset = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.kern_ln_offset = 4;
                }
                else if (2U == arg_->condition)
                {
                    params.kern_ln_offset = 5;
                }
                else if (3U == arg_->condition)
                {
                    params.kern_ln_offset = 5;
                }
                else
                {
                    params.kern_ln_offset = 5;
                }
                break;
            }
            case 6:
            {
                if (0U == arg_->condition)
                {
                    params.kern_sz_height = 1;
                }
                else if (1U == arg_->condition)
                {
                    params.kern_sz_height = 5;
                }
                else if (2U == arg_->condition)
                {
                    params.kern_sz_height = 0;
                }
                else if (3U == arg_->condition)
                {
                    params.kern_sz_height = 6;
                }
                else
                {
                    params.kern_sz_height = 6;
                }
                break;
            }
            case 7:
            {
                if (0U == arg_->condition)
                {
                    params.src_ln_inc_2 = 0;
                }
                else if (1U == arg_->condition)
                {
                    params.src_ln_inc_2 = 1;
                }
                else if (2U == arg_->condition)
                {
                    params.src_ln_inc_2 = 2;
                }
                else if (3U == arg_->condition)
                {
                    params.src_ln_inc_2 = 2;
                }
                else
                {
                    params.src_ln_inc_2 = 2;
                }
                break;
            }
        }

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

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

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}
TEST_WITH_ARG(tivxHwaVpacNfGeneric, testErrorInterrupts, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        /* Configure WDTimer with 128K cycle timeout */
        params.wdtimer_timeout_cycles = TIVX_VPAC_NF_WDTIMER_128K_CYCLES;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        VX_CALL(vxVerifyGraph(graph));

        /* Enable watchdog timer error events */
        params.enable_error_events = TIVX_VPAC_NF_WDTIMER_ERR;
        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        vx_reference param_ref = (vx_reference)param_obj;
        tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_ENABLE_ERROR_EVENTS, &param_ref, 1u);

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
            ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

            ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

            if (arg_->convolution_data_generator == convolution_data_fill_identity)
            {
                checksum_expected = get_checksum(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
                checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

                ASSERT(checksum_expected == checksum_actual);
            }
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

static uint32_t nf_generic_psa_ref[4*7*2*2] = {
    (uint32_t) 0x4e11f495, (uint32_t) 0x4e11f495, (uint32_t) 0x7a4ae16b, (uint32_t) 0x192c2985,
    (uint32_t) 0x2df3ea1,  (uint32_t) 0x2df3ea1,  (uint32_t) 0xce76a6b7, (uint32_t) 0x9c179cb4,
    (uint32_t) 0x1523f330, (uint32_t) 0x1523f330, (uint32_t) 0x68d478f7, (uint32_t) 0xcf5b4ee4,
    (uint32_t) 0x4c288d1b, (uint32_t) 0x4c288d1b, (uint32_t) 0x8bb922ac, (uint32_t) 0x5495cccd,
    (uint32_t) 0xb2679ba,  (uint32_t) 0xb2679ba,  (uint32_t) 0x7506433b, (uint32_t) 0xb213e116,
    (uint32_t) 0x3e67ec24, (uint32_t) 0x3e67ec24, (uint32_t) 0x901625da, (uint32_t) 0x1c504343,
    (uint32_t) 0xaeb6bd39, (uint32_t) 0xaeb6bd39, (uint32_t) 0x34afc358, (uint32_t) 0x34afc358,
    (uint32_t) 0x4e11f495, (uint32_t) 0x4e11f495, (uint32_t) 0x3a50640f, (uint32_t) 0x3ce58798,
    (uint32_t) 0x2df3ea1,  (uint32_t) 0x2df3ea1,  (uint32_t) 0x1b5a539e, (uint32_t) 0x57e39472,
    (uint32_t) 0x1523f330, (uint32_t) 0x1523f330, (uint32_t) 0x6e4764c1, (uint32_t) 0x89dfd2b1,
    (uint32_t) 0x4c288d1b, (uint32_t) 0x4c288d1b, (uint32_t) 0x49b02785, (uint32_t) 0x8b0c4260,
    (uint32_t) 0xb2679ba,  (uint32_t) 0xb2679ba,  (uint32_t) 0x325b28a8, (uint32_t) 0x443b4d61,
    (uint32_t) 0x3e67ec24, (uint32_t) 0x3e67ec24, (uint32_t) 0xc564bbc5, (uint32_t) 0x846054d4,
    (uint32_t) 0xaeb6bd39, (uint32_t) 0xaeb6bd39, (uint32_t) 0x34afc358, (uint32_t) 0x34afc358,
    (uint32_t) 0x4e11f495, (uint32_t) 0x4e11f495, (uint32_t) 0x5a457536, (uint32_t) 0x67c1c46c,
    (uint32_t) 0x2df3ea1,  (uint32_t) 0x2df3ea1,  (uint32_t) 0x7a088fe7, (uint32_t) 0xab4909c, 
    (uint32_t) 0x1523f330, (uint32_t) 0x1523f330, (uint32_t) 0x9ac38765, (uint32_t) 0x8133c52d,
    (uint32_t) 0x4c288d1b, (uint32_t) 0x4c288d1b, (uint32_t) 0x6f82f54b, (uint32_t) 0x3bd65fd8, 
    (uint32_t) 0xb2679ba,  (uint32_t) 0xb2679ba,  (uint32_t) 0xff2626e5, (uint32_t) 0xc535ef6, 
    (uint32_t) 0x3e67ec24, (uint32_t) 0x3e67ec24, (uint32_t) 0x1f2124d0, (uint32_t) 0x5690f9f2, 
    (uint32_t) 0xaeb6bd39, (uint32_t) 0xaeb6bd39, (uint32_t) 0x34afc358, (uint32_t) 0x34afc358, 
    (uint32_t) 0x4e11f495, (uint32_t) 0x4e11f495, (uint32_t) 0x52d2769c, (uint32_t) 0x3a08a3c7, 
    (uint32_t) 0x2df3ea1,  (uint32_t) 0x2df3ea1,  (uint32_t) 0x9e964440, (uint32_t) 0x14b45d73, 
    (uint32_t) 0x1523f330, (uint32_t) 0x1523f330, (uint32_t) 0x47fe93f8, (uint32_t) 0x52d40e75,
    (uint32_t) 0x4c288d1b, (uint32_t) 0x4c288d1b, (uint32_t) 0x84c89a22, (uint32_t) 0xa4a8bf88,
    (uint32_t) 0xb2679ba,  (uint32_t) 0xb2679ba,  (uint32_t) 0x8b4dccd0, (uint32_t) 0xbbbd3c17, 
    (uint32_t) 0x3e67ec24, (uint32_t) 0x3e67ec24, (uint32_t) 0xc4d183e2, (uint32_t) 0x32993d0d, 
    (uint32_t) 0xaeb6bd39, (uint32_t) 0xaeb6bd39, (uint32_t) 0x34afc358, (uint32_t) 0x34afc358
};

static uint32_t get_psa(vx_int32 cols, vx_int32 rows, vx_int32 shift, void (*convolution_data_generator)(int cols, int rows, vx_int16* data))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;

    if ((3 == cols) && (3 == rows))
    {
        a = 0U;
    }
    else if ((5 == cols) && (3 == rows))
    {
        a = 1U;
    }
    else if ((3 == cols) && (5 == rows))
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

    if (convolution_data_fill_identity == convolution_data_generator)
    {
        c = 0u;
    }
    else
    {
        c = 1u;
    }
    return nf_generic_psa_ref[7*2*2*a+2*2*b+2*c];
}

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testPsaSignValue, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        input_timestamp = SET_TIME_STAMP_VALUE;
        VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image, TIVX_REFERENCE_TIMESTAMP,
        &input_timestamp, sizeof(input_timestamp)));

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
                psa_refs, 1u));
            
            VX_CALL(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
            ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

            ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

            if (arg_->convolution_data_generator == convolution_data_fill_identity)
            {
                checksum_expected = get_checksum(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
                checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

                ASSERT(checksum_expected == checksum_actual);
                psa_expected = get_psa(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
                ASSERT(psa_expected == psa_status.psa_values[0]);
            }
            #if defined TEST_PRINT_TIME_STAMP_VALUES
            printf("PSA Timestamp = %" PRIu64 "\n", psa_status.timestamp);
            #endif
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

#ifndef VPAC3L
/*
 * Test name: testSl2Firewall
 *
 * Purpose: Test NF with SL2 firewall protection enabled
 *
 * Test Details: This test is identical to testPsaSignValue but with SL2 firewall
 *               protection enabled. Since firewall is transparent to output data,
 *               it reuses the same PSA signature values and checksums for validation.
 *
 * Pass Criteria:
 *    - PSA signature values match expected values
 *    - Checksums match expected values
 *    - No error events occur
 *    - Firewall protection is successfully configured and doesn't affect output
 */
TEST_WITH_ARG(tivxHwaVpacNfGeneric, testSl2Firewall, Arg,
    PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;
    vx_status status;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

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

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        input_timestamp = SET_TIME_STAMP_VALUE;
        VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image, TIVX_REFERENCE_TIMESTAMP,
        &input_timestamp, sizeof(input_timestamp)));

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
                psa_refs, 1u));

            VX_CALL(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t),
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
            ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

            ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

            if (arg_->convolution_data_generator == convolution_data_fill_identity)
            {
                checksum_expected = get_checksum(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
                checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

                ASSERT(checksum_expected == checksum_actual);
                psa_expected = get_psa(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
                ASSERT(psa_expected == psa_status.psa_values[0]);
            }
            #if defined TEST_PRINT_TIME_STAMP_VALUES
            printf("PSA Timestamp = %" PRIu64 "\n", psa_status.timestamp);
            #endif
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

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
    CT_EXPAND(nextmacro(testArgName "condition_configuration_check=0", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "condition_input_format_check=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "condition_output_format_check=2", __VA_ARGS__, 2))

#define PARAMETERS_CONDITION \
    CT_GENERATE_PARAMETERS("testNegativeChecks", ADD_CONDITION, ADD_SET_TARGET_PARAMETERS, ARG, NULL) 

/* Negative case of PARAMETER CHECKING in tivxAddKernelVpacNfGenericValidate function 
 * 0 : configuration check 
 * 1 : input format check
 * 2 : output format check */

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNegativeParameterChecks, Arg_Negative_check,
    PARAMETERS_CONDITION
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
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

        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));
        if(arg_->condition == 0)
        {
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        }
        else
        {
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        }
        tivx_vpac_nf_common_params_init(&params);  
        params.output_downshift = 0;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                  
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

/* Negative case of RELATIONSHIP CHECKING in tivxAddKernelVpacNfGenericValidate function */

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNegativeParameterRelationshipChecks, Arg_Negative_check,
    PARAMETERS_CONDITION
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 127, 127, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        tivx_vpac_nf_common_params_init(&params);  
        params.output_downshift = 0;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                  
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

/* Negative case of PARAMETER CHECKING in tivxAddKernelVpacNfGenericValidate function 
 * input and output format check for  VX_DF_IMAGE_U16*/

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNegativeInputOutputFormatChecks, Arg_Negative_check, PARAMETERS_CONDITION)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    tivx_vpac_nf_hts_bw_limit_params_t prms;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
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
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        tivx_vpac_nf_common_params_init(&params);  
        tivx_vpac_nf_hts_bw_limit_prms_init(&prms);
        params.output_downshift = 0;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                  
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

/* Negative case of PARAMETER CHECKING in tivxAddKernelVpacNfGenericValidate function 
 * parameter size not equal to sizeof(tivx_vpac_nf_common_params_t)*/

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNegativeParameterSizeChecks, SetTarget_Arg, SET_NODE_TARGET_PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    tivx_vpac_nf_hts_bw_limit_params_t prms;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                250, NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        tivx_vpac_nf_common_params_init(&params);  
        tivx_vpac_nf_hts_bw_limit_prms_init(&prms);
        params.output_downshift = 0;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                  
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

/* Test case to cover NULL checks for tivx_vpac_nf_hts_bw_limit_prms_init and tivx_vpac_nf_common_params_init*/

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNegativeInitNullCheck, Arg_Negative_check,
    PARAMETERS_CONDITION
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    tivx_vpac_nf_hts_bw_limit_params_t prms;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        tivx_vpac_nf_common_params_init(NULL);  
        tivx_vpac_nf_hts_bw_limit_prms_init(NULL);
        params.output_downshift = 0;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                  
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

/* Test case to check tivx_vpac_nf_hts_bw_limit_prms_init function */
TEST_WITH_ARG(tivxHwaVpacNfGeneric, testInitChecksHtsbwlimit, SetTarget_Arg,
    SET_NODE_TARGET_PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    tivx_vpac_nf_hts_bw_limit_params_t prms;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        tivx_vpac_nf_common_params_init(&params);  
        tivx_vpac_nf_hts_bw_limit_prms_init(&prms);
        params.output_downshift = 0;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                  
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

/* Test case to cover error case for border.mode */

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    vx_border_t border;
    char* target_string;

} Arg_border;

#define ADD_BORDER(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/VX_BORDER_UNDEFINED", __VA_ARGS__, { VX_BORDER_UNDEFINED, {{ 0 }} })), \
    CT_EXPAND(nextmacro(testArgName "/VX_BORDER_REPLICATE", __VA_ARGS__, { VX_BORDER_REPLICATE, {{ 0 }} })), \
    CT_EXPAND(nextmacro(testArgName "/VX_BORDER_CONSTANT=0", __VA_ARGS__, { VX_BORDER_CONSTANT, {{ 0 }} })), \
    CT_EXPAND(nextmacro(testArgName "/VX_BORDER_CONSTANT=1", __VA_ARGS__, { VX_BORDER_CONSTANT, {{ 1 }} })), \
    CT_EXPAND(nextmacro(testArgName "/VX_BORDER_CONSTANT=127", __VA_ARGS__, { VX_BORDER_CONSTANT, {{ 127 }} })), \
    CT_EXPAND(nextmacro(testArgName "/VX_BORDER_CONSTANT=255", __VA_ARGS__, { VX_BORDER_CONSTANT, {{ 255 }} }))
#define PARAMETERS_BORDER \
    CT_GENERATE_PARAMETERS("testborder", ADD_BORDER, ADD_SET_TARGET_PARAMETERS, ARG, NULL) 


TEST_WITH_ARG(tivxHwaVpacNfGeneric, testNegativeBorder, Arg_border, PARAMETERS_BORDER)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    vx_border_t border = arg_->border;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));
        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        params.kern_sz_height = 3;
        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

/* Test case for Targer files Coverage Gaps */

typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    int node_cmd_id;
    char* target_string;
} Argnode_cmd_id;

#define ADD_NODE(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=TIVX_VPAC_NF_CMD_SET_HTS_LIMIT", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=TIVX_VPAC_NF_CMD_SET_COEFF", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=TIVX_VPAC_NF_CMD_GET_ERR_STATUS", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=3", __VA_ARGS__, 3))

    #define PARAMETERS_NODE_ID \
    CT_GENERATE_PARAMETERS("testControlNodeCmdId", ADD_NODE, ADD_SET_TARGET_PARAMETERS, ARG, NULL)

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testControlNodeCmdId, Argnode_cmd_id,
    PARAMETERS_NODE_ID
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL, dst = NULL;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = 0;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        //VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));
        
        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        //vxWaitEvent(context, &event, vx_true_e);
 
            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_psa_timestamp_data_t", sizeof(tivx_vpac_nf_psa_timestamp_data_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            switch (arg_->node_cmd_id)
            {
            case 0:
                ASSERT_NO_FAILURE(
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                    psa_refs, 1u));
                break;
            case 1:
                ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                psa_refs, 1u));
                break;
            case 3:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                tivxNodeSendCommand(node, 0u, 0,
                psa_refs, 1u));
                break;
            default:
                break;
            }
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}


/* Testcase to cover Image format for tivxVpacNfSetFmt Function */
typedef struct
{
    const char* testName;
    CT_Image(*generator)(const char* fileName, int width, int height);
    int image_format;
    char* target_string;

} Arg_ImageFormat_check;

#define ADD_IMAGE_FORMAT(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "ImageFormat=0", __VA_ARGS__, 0)), \
    CT_EXPAND(nextmacro(testArgName "ImageFormat=1", __VA_ARGS__, 1)), \
    CT_EXPAND(nextmacro(testArgName "ImageFormat=2", __VA_ARGS__, 2)), \
    CT_EXPAND(nextmacro(testArgName "ImageFormat=3", __VA_ARGS__, 3))

#define PARAMETERS_IMAGE_FORMAT \
    CT_GENERATE_PARAMETERS("testNegativeChecks", ADD_IMAGE_FORMAT, ADD_SET_TARGET_PARAMETERS, ARG, NULL) 

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testImageFormat, Arg_ImageFormat_check, PARAMETERS_IMAGE_FORMAT)
{
   vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    tivx_vpac_nf_common_params_t params;
    tivx_vpac_nf_hts_bw_limit_params_t prms;
    vx_user_data_object param_obj;
    vx_int16 data[3 * 3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
    vx_convolution convolution = 0;
    vx_graph graph = 0;
    vx_node node = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);
        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
	    switch (arg_->image_format)
        {
            case 0:
            {
            	ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
            	break;
            }
            case 1:
            {
            	ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U16), VX_TYPE_IMAGE);
            	break;
            }
            case 2:
            {
            	ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_S16), VX_TYPE_IMAGE);
            	break;
            }
            case 3:
            {
            	ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, TIVX_DF_IMAGE_P12), VX_TYPE_IMAGE);
            	break;
            }
            default:
                    break;
        }
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        tivx_vpac_nf_common_params_init(&params);  
        tivx_vpac_nf_hts_bw_limit_prms_init(&prms);
        params.output_downshift = 0;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));
                                                  
        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        ASSERT_NO_FAILURE(vxVerifyGraph(graph));
        ASSERT_NO_FAILURE(vxProcessGraph(graph));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(node == 0);
        ASSERT(graph == 0);
        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}
#if (VPAC_COUNT > 1)
#if defined IMAGING_COVERAGE_ENABLED
#define ADD_SET_TARGET_PARAMETERS_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF))
#else
#define ADD_SET_TARGET_PARAMETERS_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF))
#endif
#else
#define ADD_SET_TARGET_PARAMETERS_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/TIVX_TARGET_VPAC_NF", __VA_ARGS__, TIVX_TARGET_VPAC_NF))
#endif

#define ADD_NODE_CC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=TIVX_VPAC_NF_CMD_GET_ERR_STATUS", __VA_ARGS__, 2))

#define PARAMETERS_NODE_ID_CC \
    CT_GENERATE_PARAMETERS("testControlNodeCmdId", ADD_NODE_CC, ADD_SET_TARGET_PARAMETERS_CC,ARG, NULL)

// Negative TC to cover Null check in tivxVpacNfGenericGetErrStatusCmd 
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testNullCmdGetErrStatus, Argnode_cmd_id,
    PARAMETERS_NODE_ID_CC
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL, dst = NULL;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = 0;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));


        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));
        
        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */

        /* Get error interrupt status values */
        ASSERT_VX_OBJECT(scalar_err_stat_obj = vxCreateScalar(context,
        VX_TYPE_UINT32 , &scalar_out_err_stat),
        (enum vx_type_e)VX_TYPE_SCALAR);
    
        err_refs[0] = (vx_reference)NULL;
        ASSERT_NO_FAILURE(
            tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
            err_refs, 1u));
        VX_CALL(vxCopyScalar(scalar_err_stat_obj, &scalar_out_err_stat, VX_READ_ONLY,
            VX_MEMORY_TYPE_HOST));
        #if defined TEST_GET_ERROR_INTERRUPT_VALUES
        printf("NF error status value is %d\n", scalar_out_err_stat);
        #endif
        VX_CALL(vxReleaseScalar(&scalar_err_stat_obj));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

// Negative TC for TIVX_VPAC_NF_CMD_GET_PSA_STATUS size check in tivxVpacNfGenericGetPsaStatusCmd
#define ADD_NODE_PS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=TIVX_VPAC_NF_CMD_GET_PSA_STATUS", __VA_ARGS__, 3))

#define PARAMETERS_NODE_ID_PS \
CT_GENERATE_PARAMETERS("testControlNodeCmdIdGetPSA", ADD_NODE_PS, ADD_SET_TARGET_PARAMETERS, ARG, NULL)

TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testControlNodePSAStatusSizeCheck, Argnode_cmd_id,
    PARAMETERS_NODE_ID_PS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL, dst = NULL;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = 0;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        //VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                vxCreateUserDataObject(context, "tivx_vpac_nf_get_psa_params_t", 4, NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            switch (arg_->node_cmd_id)
            {
            case 0:
                ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                    psa_refs, 1u));
                break;
            case 1:
                ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                    psa_refs, 1u));
                break;
            case 3:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
                    psa_refs, 1u));
                break;
            default:
                break;
            }
        ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
            &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

        VX_CALL(vxReleaseUserDataObject(&psa_obj));
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}


#define ADD_NODE_HTS(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=TIVX_VPAC_NF_CMD_SET_HTS_LIMIT", __VA_ARGS__, 0))

    #define PARAMETERS_NODE_ID_HTS \
    CT_GENERATE_PARAMETERS("testControlNodeCmdIdSetHTSLimit", ADD_NODE_HTS, ADD_SET_TARGET_PARAMETERS, ARG, NULL)

// Negative TC for NULL check in tivxVpacNfGenericSetHtsLimitCmd
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testControlNodeSetHTSLimitNullCheck, Argnode_cmd_id,
    PARAMETERS_NODE_ID_HTS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL, dst = NULL;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = 0;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        //VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));
        
        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        //vxWaitEvent(context, &event, vx_true_e);


            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_hts_bw_limit_params_t", sizeof(tivx_vpac_nf_hts_bw_limit_params_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) NULL;
            switch (arg_->node_cmd_id)
            {
            case 0:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                    psa_refs, 1u));
                break;
            case 1:
                ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                psa_refs, 1u));
                break;
            case 3:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                tivxNodeSendCommand(node, 0u, 0,
                psa_refs, 1u));
                break;
            default:
                break;
            }
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

//Negative TC for Size check in tivxVpacNfGenericSetHtsLimitCmd
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testControlNodeHTSLimitSizeCheck, Argnode_cmd_id,
    PARAMETERS_NODE_ID_HTS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL, dst = NULL;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = 0;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        //VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

        VX_CALL(vxVerifyGraph(graph));
        VX_CALL(vxProcessGraph(graph));
        
        /* Note: since we are not blocking and there are no events, this returns VX_FAILURE per spec
         * and prints an error message saying that there are no events in the queue */
        //vxWaitEvent(context, &event, vx_true_e);

            ASSERT_VX_OBJECT(psa_obj = 
                vxCreateUserDataObject(context, "tivx_vpac_nf_hts_bw_limit_params_t", sizeof(tivx_vpac_nf_hts_bw_limit_params_t)-5, NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) psa_obj;
            switch (arg_->node_cmd_id)
            {
            case 0:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                    psa_refs, 1u));
                break;
            case 1:
                ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                psa_refs, 1u));
                break;
            case 3:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                tivxNodeSendCommand(node, 0u, 0,
                psa_refs, 1u));
                break;
            default:
                break;
            }
            ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
        

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

#define ADD_NODE_SC(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/node_cmd_id=TIVX_VPAC_NF_CMD_SET_COEFF", __VA_ARGS__, 1))

#define PARAMETERS_NODE_ID_SC \
CT_GENERATE_PARAMETERS("testControlNodeCmdIdFilterCoe", ADD_NODE_SC, ADD_SET_TARGET_PARAMETERS, ARG, NULL)

//Negative TC for NULL check in tivxVpacNfGenericSetCoeff
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testControlFilterCoeNullCheck, Argnode_cmd_id,
    PARAMETERS_NODE_ID_SC
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL, dst = NULL;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = 0;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        //VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                vxCreateUserDataObject(context, "tivx_vpac_nf_set_coe_params_t", 5220, NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) NULL;
            switch (arg_->node_cmd_id)
            {
            case 0:
                ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                    psa_refs, 1u));
                break;
            case 1:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                    psa_refs, 1u));
                break;
            case 3:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                tivxNodeSendCommand(node, 0u, 0,
                psa_refs, 1u));
                break;
            default:
                break;
            }
        ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
            &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

        VX_CALL(vxReleaseUserDataObject(&psa_obj));
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

//Negative TC for NULL check in tivxVpacNfGenericGetPsaStatusCmd
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testControlNodePSAStatusNullCheck, Argnode_cmd_id,
    PARAMETERS_NODE_ID_PS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    int cols = 3, rows = 3;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_expected, psa_expected;
    uint32_t checksum_actual;

    CT_Image src = NULL, dst = NULL;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_VX_OBJECT(src_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, 128, 128, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);
        
        ASSERT_VX_OBJECT(convolution = convolution_create(context, cols, rows, data, 1), VX_TYPE_CONVOLUTION);

        memset(&params, 0, sizeof(tivx_vpac_nf_common_params_t));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = 0;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        //VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t", sizeof(tivx_vpac_nf_common_params_t), NULL),
                VX_TYPE_USER_DATA_OBJECT);
        
            psa_refs[0] = (vx_reference) NULL;
            switch (arg_->node_cmd_id)
            {
            case 0:
                ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                    psa_refs, 1u));
                break;
            case 1:
                ASSERT_EQ_VX_STATUS(VX_SUCCESS,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                    psa_refs, 1u));
                break;
            case 3:
                ASSERT_EQ_VX_STATUS(VX_FAILURE,
                    tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_PSA_STATUS,
                    psa_refs, 1u));
                break;
            default:
                break;
            }
        ASSERT_NO_FAILURE(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_psa_timestamp_data_t), 
            &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

        VX_CALL(vxReleaseUserDataObject(&psa_obj));
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}
#define ADD_CONV_SIZE_CH(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv=3x3", __VA_ARGS__, 3, 3))

#define ADD_CONV_SHIFT_CH(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_shift=0", __VA_ARGS__, 0))

#define ADD_CONV_GENERATORS_CH(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/conv_fill=identity", __VA_ARGS__, convolution_data_fill_identity))

#define ADD_CONV_DST_FORMAT_CH(testArgName, nextmacro, ...) \
    CT_EXPAND(nextmacro(testArgName "/dst8U", __VA_ARGS__, VX_DF_IMAGE_U8))
#if 0
, \
    CT_EXPAND(nextmacro(testArgName "/dst16S", __VA_ARGS__, VX_DF_IMAGE_S16))
#endif

#define PARAMETERS_CCN \
    CT_GENERATE_PARAMETERS("lena", ADD_CONV_SIZE_CH, ADD_CONV_SHIFT_CH, ADD_CONV_GENERATORS_CH, ADD_CONV_DST_FORMAT_CH, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS, ARG, convolve_read_image, "lena.bmp")


// TC to cover TIVX_VPAC_NF_CMD_SET_HTS_LIMIT
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testControlNodeCmdIdSetHTSLimit, Arg,
    PARAMETERS_CCN
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        input_timestamp = SET_TIME_STAMP_VALUE;
        VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image, TIVX_REFERENCE_TIMESTAMP,
        &input_timestamp, sizeof(input_timestamp)));

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_HTS_LIMIT,
                psa_refs, 1u));
            
            VX_CALL(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_hts_bw_limit_params_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
            ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

            ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

            if (arg_->convolution_data_generator == convolution_data_fill_identity)
            {
                checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

                ASSERT(0x41dd742d == checksum_actual);
            }
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

//TC to cover TIVX_VPAC_NF_CMD_GET_ERR_STATUS
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testTivxVpac_NF_CmdGetErrStatus, Arg,
    PARAMETERS_CCN
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        input_timestamp = SET_TIME_STAMP_VALUE;
        VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image, TIVX_REFERENCE_TIMESTAMP,
        &input_timestamp, sizeof(input_timestamp)));

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
                psa_refs, 1u));
            
            VX_CALL(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_hts_bw_limit_params_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
            ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

            ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

            if (arg_->convolution_data_generator == convolution_data_fill_identity)
            {
                checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

                ASSERT(0x41dd742d == checksum_actual);
            }
        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));
        ASSERT(convolution == NULL);

        tivxHwaUnLoadKernels(context);
    }
}

//TC to cover TIVX_VPAC_NF_CMD_SET_COEFF
TEST_WITH_ARG(tivxHwaVpacNfGenericTarget, testControlNodeCmdIdFilterCoe, Arg,
    PARAMETERS_CCN
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image = 0, dst_image = 0;
    vx_convolution convolution = 0;
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj;
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node = 0;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
    vx_border_t border = arg_->border;
    vx_scalar scalar_obj;
    vx_enum scalar_out;
    vx_reference refs[5] = {0};
    vx_scalar scalar_err_stat_obj, scalar_wdtimer_err_stat_obj;
    vx_enum scalar_out_err_stat, scalar_out_wdtimer_err_stat;
    vx_reference err_refs[5] = {0};
    vx_reference wd_err_refs[5] = {0};
    vx_event_t event = { 0 };
    vx_reference psa_refs [5] = {0};
    uint32_t psa_values[4] = {0};
    vx_user_data_object psa_obj;
    tivx_vpac_nf_psa_timestamp_data_t psa_status;
    uint64_t input_timestamp = 0;

    ASSERT(vx_true_e == tivxIsTargetEnabled(arg_->target_string));

    {
        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = 640;
        rect.end_y = 480;
        tivxHwaLoadKernels(context);
        CT_RegisterForGarbageCollection(context, ct_teardown_hwa_kernels, CT_GC_OBJECT);

        ASSERT_NO_FAILURE(src = arg_->generator(arg_->fileName, arg_->width, arg_->height));
        ASSERT_VX_OBJECT(src_image = ct_image_to_vx_image(src, context), VX_TYPE_IMAGE);

        input_timestamp = SET_TIME_STAMP_VALUE;
        VX_CALL(tivxSetReferenceAttribute((vx_reference)src_image, TIVX_REFERENCE_TIMESTAMP,
        &input_timestamp, sizeof(input_timestamp)));

        ASSERT_VX_OBJECT(dst_image = vxCreateImage(context, src->width, src->height, arg_->dst_format), VX_TYPE_IMAGE);

        VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

        if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
        {
            printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
            return;
        }

        ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
        ASSERT_NO_FAILURE(convolution = convolution_create(context, arg_->cols, arg_->rows, data, 1));

        ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                            sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
        tivx_vpac_nf_common_params_init(&params);

        params.output_downshift = arg_->shift;
        /* Disable all events by default */
        params.enable_error_events = 0U;
        params.enable_psa = 1u;

        VX_CALL(vxCopyUserDataObject(param_obj, 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

        ASSERT_VX_OBJECT(node = tivxVpacNfGenericNode(graph, param_obj, src_image, convolution, dst_image), VX_TYPE_NODE);

        /* enable events generation */
        vxEnableEvents(context);
        /* Register an error event for the node */
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, vxRegisterEvent((vx_reference)node, VX_EVENT_NODE_ERROR, 0, NF_NODE_ERROR_EVENT));

        VX_CALL(vxSetNodeTarget(node, VX_TARGET_STRING, arg_->target_string));

        VX_CALL(vxSetNodeAttribute(node, VX_NODE_BORDER, &border, sizeof(border)));

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
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_GET_ERR_STATUS,
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
                vxCreateUserDataObject(context, "tivx_vpac_nf_hts_bw_limit_params_t", 5220, NULL),
                VX_TYPE_USER_DATA_OBJECT);
            psa_refs[0] = (vx_reference) psa_obj;
            ASSERT_NO_FAILURE(
                tivxNodeSendCommand(node, 0u, TIVX_VPAC_NF_CMD_SET_COEFF,
                psa_refs, 1u));
            
            VX_CALL(vxCopyUserDataObject(psa_obj, 0, sizeof(tivx_vpac_nf_hts_bw_limit_params_t), 
                &psa_status, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

            VX_CALL(vxReleaseUserDataObject(&psa_obj));
            ASSERT_NO_FAILURE(dst = ct_image_from_vx_image(dst_image));

            ASSERT_NO_FAILURE(convolve_check(src, dst, border, arg_->cols, arg_->rows, data, arg_->shift, arg_->dst_format));

            if (arg_->convolution_data_generator == convolution_data_fill_identity)
            {
                checksum_actual = tivx_utils_simple_image_checksum(dst_image, 0, rect);

                ASSERT(0x41dd742d == checksum_actual);
            }

        }

        VX_CALL(vxReleaseNode(&node));
        VX_CALL(vxReleaseGraph(&graph));
        VX_CALL(vxDisableEvents(context));

        ASSERT(node == 0);
        ASSERT(graph == 0);

        VX_CALL(vxReleaseImage(&dst_image));
        VX_CALL(vxReleaseImage(&src_image));
        VX_CALL(vxReleaseUserDataObject(&param_obj));

        ASSERT(dst_image == 0);
        ASSERT(src_image == 0);
        ASSERT(param_obj == 0);

        VX_CALL(vxReleaseConvolution(&convolution));

        tivxHwaUnLoadKernels(context);
    }
}

#define MULTI_NODE_PARAMETERS \
    CT_GENERATE_PARAMETERS("lena", ADD_CONV_SIZE, ADD_CONV_SHIFT, ADD_CONV_GENERATORS, ADD_CONV_DST_FORMAT, ADD_VX_BORDERS_REQUIRE_UNDEFINED_ONLY, ADD_SIZE_NONE, ADD_SET_TARGET_PARAMETERS_MULTI_INST, ARG, convolve_read_image, "lena.bmp")

TEST_WITH_ARG(tivxHwaVpacNfGeneric, testGraphProcessing_multi_node, Arg,
    MULTI_NODE_PARAMETERS
)
{
    vx_context context = context_->vx_context_;
    vx_image src_image [TEST_MAX_NUM_NODE_INSTANCE] = {0}, dst_image[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    vx_convolution convolution[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    vx_int16 data[MAX_CONV_SIZE * MAX_CONV_SIZE] = { 0 };
    tivx_vpac_nf_common_params_t params;
    vx_user_data_object param_obj[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    vx_size conv_max_dim = 0;
    vx_graph graph = 0;
    vx_node node[TEST_MAX_NUM_NODE_INSTANCE] = {0};
    uint32_t checksum_expected,i;
    uint32_t checksum_actual;
    vx_rectangle_t rect;

    CT_Image src = NULL, dst = NULL;
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

                VX_CALL(vxQueryContext(context, VX_CONTEXT_CONVOLUTION_MAX_DIMENSION, &conv_max_dim, sizeof(conv_max_dim)));

                if ((vx_size)arg_->cols > conv_max_dim || (vx_size)arg_->rows > conv_max_dim)
                {
                    printf("%dx%d convolution is not supported. Skip test\n", (int)arg_->cols, (int)arg_->rows);
                    return;
                }

                ASSERT_NO_FAILURE(arg_->convolution_data_generator(arg_->cols, arg_->rows, data));
                ASSERT_NO_FAILURE(convolution[i] = convolution_create(context, arg_->cols, arg_->rows, data, 1));

                ASSERT_VX_OBJECT(param_obj[i] = vxCreateUserDataObject(context, "tivx_vpac_nf_common_params_t",
                                                                    sizeof(tivx_vpac_nf_common_params_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);
                tivx_vpac_nf_common_params_init(&params);

                params.output_downshift = arg_->shift;

                VX_CALL(vxCopyUserDataObject(param_obj[i], 0, sizeof(tivx_vpac_nf_common_params_t), &params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));


                ASSERT_VX_OBJECT(node[i] = tivxVpacNfGenericNode(graph, param_obj[i], src_image[i], convolution[i], dst_image[i]), VX_TYPE_NODE);

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
                if (arg_->convolution_data_generator == convolution_data_fill_identity)
                {
                    checksum_expected = get_checksum(arg_->cols, arg_->rows, arg_->shift, arg_->convolution_data_generator);
                    checksum_actual = tivx_utils_simple_image_checksum(dst_image[i], 0, rect);

                    ASSERT(checksum_expected == checksum_actual);
                }
            }
        }

        for (i = 0; i < TEST_MAX_NUM_NODE_INSTANCE; i++)
        {
            if ( (NULL != arg_->target_string) ||
                 (NULL != arg_->target_string_2))
            {
                VX_CALL(vxReleaseNode(&node[i]));

                ASSERT(node[i] == 0);

                VX_CALL(vxReleaseImage(&dst_image[i]));
                VX_CALL(vxReleaseImage(&src_image[i]));
                VX_CALL(vxReleaseUserDataObject(&param_obj[i]));

                ASSERT(dst_image[i] == 0);
                ASSERT(src_image[i] == 0);
                ASSERT(param_obj[i] == 0);

                VX_CALL(vxReleaseConvolution(&convolution[i]));
                ASSERT(convolution[i] == NULL);
            }
        }
        VX_CALL(vxReleaseGraph(&graph));
        ASSERT(graph == 0);

        tivxHwaUnLoadKernels(context);
    }
}

TESTCASE_TESTS(tivxHwaVpacNfGeneric,
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
    testNegativeParameterRelationshipChecks,
    testNegativeInputOutputFormatChecks,
    testNegativeParameterSizeChecks,
    testNegativeInitNullCheck,
    testInitChecksHtsbwlimit,
    testNegativeBorder,
    testControlNodeCmdId,
    testImageFormat
    )
TESTCASE_TESTS(tivxHwaVpacNfGenericTarget,
    #ifndef x86_64
    testNullCmdGetErrStatus,
    #endif
    testControlNodePSAStatusSizeCheck,
    testControlNodeSetHTSLimitNullCheck,
    testControlNodeHTSLimitSizeCheck,
    testControlFilterCoeNullCheck,
    testControlNodePSAStatusNullCheck,
    testControlNodeCmdIdSetHTSLimit,
    #ifndef x86_64
    testTivxVpac_NF_CmdGetErrStatus,
    #endif
    testControlNodeCmdIdFilterCoe
    )

#endif /* BUILD_VPAC_NF */
