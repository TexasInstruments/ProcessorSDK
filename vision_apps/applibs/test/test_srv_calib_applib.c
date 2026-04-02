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

#include "test_engine/test.h"
#include "test_tiovx/test_tiovx.h"

#include <VX/vx.h>
#include <VX/vxu.h>
#include <VX/vx_khr_pipelining.h>
#include "math.h"
#include "tivx_utils_file_rd_wr.h"
#include <limits.h>
#include <TI/tivx_srv.h>
#include "srv_calibration_applib/srv_calibration_applib.h"
#include "lens_distortion_correction.h"
#include "srv_common.h"

TESTCASE(tivxSrvCalibApplib,  CT_VXContext, ct_setup_vx_context, 0)

static LDC_status LDC_Init(LensDistortionCorrection* ldc,
                        dtype distCenterX, dtype distCenterY, dtype distFocalLength,
                        dtype *lut_d2u, vx_int32 lut_d2u_length, dtype lut_d2u_step,
                        dtype *lut_u2d, vx_int32 lut_u2d_length, dtype lut_u2d_step)
{
#if LDC_LIB_DATA_TYPE!=0 && LDC_LIB_DATA_TYPE!=1
        "LDC_LIB_DATA_TYPE must be 0 (float) or 1 (double) in lens_distortion_correction.h"
#endif
        /*FLOATING TYPE*/
        /*distortion center*/
        ldc->distCenterX = distCenterX;
        ldc->distCenterY = distCenterY;
        ldc->distFocalLength = distFocalLength;
        ldc->distFocalLengthInv = 1/ldc->distFocalLength;
        /*ldc look-up table parameters*/
        ldc->lut_d2u_indMax = lut_d2u_length-1;
        ldc->lut_d2u_step = lut_d2u_step;
        ldc->lut_u2d_indMax = lut_u2d_length - 1;
        ldc->lut_u2d_step = lut_u2d_step;

        ldc->lut_d2u_stepInv = 1/ldc->lut_d2u_step;
        ldc->lut_u2d_stepInv = 1/ldc->lut_u2d_step;


        /*ldc look-up table pointers*/
        memcpy (ldc->lut_d2u, (uint8_t *)lut_d2u,(sizeof(dtype)*LDC_D2U_TABLE_MAX_LENGTH) );
        memcpy (ldc->lut_u2d, (uint8_t *)lut_u2d,(sizeof(dtype)*LDC_U2D_TABLE_MAX_LENGTH) );

        return LDC_STATUS_OK;
}

static void read_lut_file(ldc_lensParameters *ldcParams, const char*fileName)
{ 
    char file[MAXPATHLENGTH];  
    uint32_t  read_size;
    FILE* f = 0;
    size_t sz;

    if (!fileName)
    {
        printf("Image file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "rb");
    if (!f)
    {
        printf("Can't open LUT file: %s\n", fileName);
        return;
    }

    read_size = fread((uint8_t *)ldcParams,sizeof(uint8_t),sizeof(ldc_lensParameters),f);
    if (read_size != sizeof(ldc_lensParameters))
    {
        CT_ADD_FAILURE("Incorrect Bytes read from  LUT file: %s\n", fileName);
        fclose(f);
        return;
    }

    fclose(f);
}

static void ct_read_image2(vx_image image, const char* fileName, uint16_t file_byte_pack)
{
    FILE* f = 0;
    size_t sz;
    char* buf = 0;
    char file[MAXPATHLENGTH];

    if (!fileName)
    {
        CT_ADD_FAILURE("Image file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "rb");
    if (!f)
    {
        CT_ADD_FAILURE("Can't open image file: %s\n", fileName);
        return;
    }

    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    if( sz > 0 )
    {
        buf = (char*)ct_alloc_mem(sz);
        fseek(f, 0, SEEK_SET);
        if (NULL != buf)
        {
            if( fread(buf, 1, sz, f) == sz )
            {
                vx_uint32 width, height;
                vx_imagepatch_addressing_t image_addr;
                vx_rectangle_t rect;
                vx_map_id map_id;
                vx_df_image df;
                void *data_ptr;
                vx_uint32 num_bytes = 1;

                vxQueryImage(image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
                vxQueryImage(image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
                vxQueryImage(image, VX_IMAGE_FORMAT, &df, sizeof(vx_df_image));
                
                if( (df == VX_DF_IMAGE_U16) || (df == VX_DF_IMAGE_S16) )
                {
                    num_bytes = 2;
                }
                else if( (df == VX_DF_IMAGE_U32) || (df == VX_DF_IMAGE_S32) )
                {
                    num_bytes = 4;
                }

                rect.start_x = 0;
                rect.start_y = 0;
                rect.end_x = width;
                rect.end_y = height;

                vxMapImagePatch(image,
                    &rect,
                    0,
                    &map_id,
                    &image_addr,
                    &data_ptr,
                    VX_WRITE_ONLY,
                    VX_MEMORY_TYPE_HOST,
                    VX_NOGAP_X
                    );

                if(file_byte_pack == num_bytes)
                {
                    memcpy(data_ptr, buf, width*height*num_bytes);
                }
                else if((file_byte_pack == 2) && (num_bytes == 1))
                {
                    int i;
                    uint8_t *dst = data_ptr;
                    uint16_t *src = (uint16_t*)buf;
                    for(i = 0; i < width*height; i++)
                    {
                        dst[i] = src[i];
                    }
                }
                vxUnmapImagePatch(image, map_id);
            }
        }
        else
        {
            fclose(f);
        }
    }

    ct_free_mem(buf);
    fclose(f);
}


static void ct_write_image2(vx_image image, const char* fileName)
{
    FILE* f = 0;
    size_t sz;
    char file[MAXPATHLENGTH];

    if (!fileName)
    {
        CT_ADD_FAILURE("Image file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "wb");
    if (!f)
    {
        CT_ADD_FAILURE("Can't open image file: %s\n", fileName);
        return;
    }
    else
    {
        vx_uint32 width, height;
        vx_imagepatch_addressing_t image_addr;
        vx_rectangle_t rect;
        vx_map_id map_id;
        vx_df_image df;
        void *data_ptr;
        vx_uint32 num_bytes = 1;

        vxQueryImage(image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_FORMAT, &df, sizeof(vx_df_image));
        
        if( (df == VX_DF_IMAGE_U16) || (df == VX_DF_IMAGE_S16) )
        {
            num_bytes = 2;
        }
        else if( (df == VX_DF_IMAGE_U32) || (df == VX_DF_IMAGE_S32) )
        {
            num_bytes = 4;
        }

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = width;
        rect.end_y = height;

        vxMapImagePatch(image,
            &rect,
            0,
            &map_id,
            &image_addr,
            &data_ptr,
            VX_WRITE_ONLY,
            VX_MEMORY_TYPE_HOST,
            VX_NOGAP_X
            );

        fwrite(data_ptr, 1, width*height*num_bytes, f);

        vxUnmapImagePatch(image, map_id);
    }

    fclose(f);
}



static void point_detect_corners_check(svACDetectStructFinalCorner_t ref_corners, svACDetectStructFinalCorner_t out_corners)
/* ref corners are downshifted by 4, out corners need to be shifted down */
{
    int32_t   ref_corner;
    int32_t   out_corner;
    int32_t   i,j, difference;

    for (i=0;i<2;i++) 
    {
        for (j=0;j<8;j++) {
            ref_corner = ref_corners.finalCorners[i][j];
            out_corner = out_corners.finalCorners[i][j]>>4;
            difference = ref_corner - out_corner;
            ASSERT(abs(difference) <= 1);
        }
    }
    ASSERT(ref_corners.numFPDetected == out_corners.numFPDetected);

}

static void point_detect_binarize_check(vx_image img_out, vx_image img_ref)
{
    CT_Image ct_output_bw = NULL, ct_ref_bw = NULL;

    ASSERT(img_out && img_ref);

    //Convert vx to CT
    ASSERT_NO_FAILURE(ct_output_bw = ct_image_from_vx_image(img_out));
    ASSERT_NO_FAILURE(ct_ref_bw = ct_image_from_vx_image(img_ref));
    
    EXPECT_EQ_CTIMAGE(ct_ref_bw, ct_output_bw); 
}

static void read_chartpos_file(vx_int8 *inChartPos, const char*fileName)
{ 
    char file[MAXPATHLENGTH];  
    FILE* f = 0;
    size_t sz;
    uint32_t  read_size;

    #ifdef DEBUG_PRINT
    printf ("Reading chartpos file \n");
    #endif

    if (!fileName)
    {
        printf("chartpos file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "rb");
    if (!f)
    {
        CT_ADD_FAILURE("Cant open chartpos file: %s\n", fileName);
        return;
    }

    read_size = fread((int8_t *)inChartPos,sizeof(uint8_t),164,f);

    if (read_size != 164)
    {
        CT_ADD_FAILURE("Incorrect Bytes read from chartpos file: %s: %d should be 164\n", fileName, read_size);
        fclose(f);
        return;
    }

    #ifdef DEBUG_PRINT
    printf ("file read completed \n");
    #endif
    fclose(f);

}

static void read_calmat_file( svCalmat_t *calmat, const char*fileName)
{ 
    char file[MAXPATHLENGTH];  
    uint32_t cnt;
    FILE* f = 0;
    size_t sz;

    if (!fileName)
    {
        printf("calmat file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "rb");
    if (!f)
    {
        CT_ADD_FAILURE("Cant open calmat file: %s\n", fileName);
        return;
    }

    fread((int8_t *)&calmat->numCameras,sizeof(uint8_t),4,f);

    for (cnt=0;cnt<NUM_CAMERAS;cnt++) {
        fread((int8_t *)&calmat->calMatSize[cnt],sizeof(uint8_t),4,f);
    }

    /* Set Pointer ahead by 128 bytes to skip over metadata */
    fseek(f,128,SEEK_SET);

    /* Read calmat per camera */
    for (cnt=0;cnt<NUM_CAMERAS;cnt++) {
        fread((int8_t *)&calmat->calMatBuf + 48*cnt,sizeof(uint8_t),calmat->calMatSize[cnt],f);
    }

    fclose(f);

}

static void write_calmat_file( svCalmat_t *calmat, const char*fileName)
{
    char file[MAXPATHLENGTH];
    uint32_t cnt;
    FILE* f = 0;
    size_t sz;

    if (!fileName)
    {
        printf("calmat file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "wb");
    if (!f)
    {
        CT_ADD_FAILURE("Cant open calmat file: %s\n", fileName);
        return;
    }

    fwrite(&calmat->numCameras,sizeof(uint8_t),4,f);

    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        fwrite(&calmat->calMatSize[cnt],sizeof(uint8_t),4,f);
    }

    /* Set Pointer ahead by 128 bytes to skip over metadata */
    fseek(f,128,SEEK_SET);

    /* Read calmat per camera */
    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        fwrite((int8_t *)calmat->calMatBuf + 48*cnt,sizeof(uint8_t),calmat->calMatSize[cnt],f);
    }

    fclose(f);
}

static void pose_estimation_calmat_check(svCalmat_t ref_calmat, svCalmat_t out_calmat)
/* ref corners are downshifted by 4, out corners need to be shifted down */
{
    int32_t   i,j;
    int32_t  reference,out;

    for (i=0;i<4;i++) 
    {
        for (j=0;j<12;j++) {
            reference = ref_calmat.calMatBuf[12*i+j]; 
            out       = out_calmat.calMatBuf[12*i+j];
            ASSERT(reference == out);
        } //j
    } //i

}

typedef struct {
    const char* name;
    int width;
    int height;
} Arg;

#define PARAMETERS \
    CT_GENERATE_PARAMETERS("srv_calib_applib", ARG, 1280, 720), \
    CT_GENERATE_PARAMETERS("srv_calib_applib", ARG, 1920, 1080), \

TEST_WITH_ARG(tivxSrvCalibApplib, testApplib, Arg, PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_graph graph;
    vx_node node;
    vx_uint32 i, width, height;
    srv_calib_handle srv_handle = NULL;
    srv_calib_createParams create_params;
    vx_uint8 is2MP = 0;
    int idx, jdx;

    /* Structs for populating OpenVX objects */
    svPointDetect_t in_point_detect_params;
    svPoseEstimation_t    in_pose_estimation_params;
    svLdcLut_t      in_lens_params;
    svACCalmatStruct_t   out_calmat_params;
    ldc_lensParameters lens_params; /* This is only for reading from file */
    svACDetectStructFinalCorner_t out_params[NUM_CAMERAS];
    svACDetectStructFinalCorner_t ref_params[NUM_CAMERAS]; /* Pre-calculated ref output values */

    /* OpenVX objects as parameters to applib */
    vx_user_data_object point_detect_in_config;
    vx_user_data_object pose_estimation_in_config;
    vx_user_data_object in_lens_param_object;
    vx_image in, buf_bwluma_frame = 0;
    vx_object_array buf_bwluma_frame_array, in_array;
    vx_user_data_object  out_calmat;
    vx_object_array in_corners;

    /* Reference object */
    svCalmat_t            calmat_ref;
    svCalmat_t            calmat_out;

    width = arg_->width;
    height = arg_->height;

    if (width == 1920)
    {
        is2MP = 1;
    }

    ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

    tivxSrvLoadKernels(context);

    /* Loading lens params */
    if (1 == is2MP)
    {
        read_lut_file(&lens_params,"psdkra/srv/applibTC_2mpix/LENS.BIN" );

        ref_params[0].finalCorners[0][0] = 621;
        ref_params[0].finalCorners[0][1] = 465;
        ref_params[0].finalCorners[0][2] = 622;
        ref_params[0].finalCorners[0][3] = 717;
        ref_params[0].finalCorners[0][4] = 995;
        ref_params[0].finalCorners[0][5] = 548;
        ref_params[0].finalCorners[0][6] = 843;
        ref_params[0].finalCorners[0][7] = 300;

        ref_params[0].finalCorners[1][0] = 610;
        ref_params[0].finalCorners[1][1] = 1225;
        ref_params[0].finalCorners[1][2] = 596;
        ref_params[0].finalCorners[1][3] = 1481;
        ref_params[0].finalCorners[1][4] = 815;
        ref_params[0].finalCorners[1][5] = 1653;
        ref_params[0].finalCorners[1][6] = 977;
        ref_params[0].finalCorners[1][7] = 1411;
        ref_params[0].numFPDetected = 8;

        ref_params[1].finalCorners[0][0] = 655;
        ref_params[1].finalCorners[0][1] = 355;
        ref_params[1].finalCorners[0][2] = 659;
        ref_params[1].finalCorners[0][3] = 512;
        ref_params[1].finalCorners[0][4] = 969;
        ref_params[1].finalCorners[0][5] = 343;
        ref_params[1].finalCorners[0][6] = 850;
        ref_params[1].finalCorners[0][7] = 233;

        ref_params[1].finalCorners[1][0] = 625;
        ref_params[1].finalCorners[1][1] = 1427;
        ref_params[1].finalCorners[1][2] = 610;
        ref_params[1].finalCorners[1][3] = 1580;
        ref_params[1].finalCorners[1][4] = 788;
        ref_params[1].finalCorners[1][5] = 1718;
        ref_params[1].finalCorners[1][6] = 907;
        ref_params[1].finalCorners[1][7] = 1625;
        ref_params[1].numFPDetected = 8;

        ref_params[2].finalCorners[0][0] = 597;
        ref_params[2].finalCorners[0][1] = 500;
        ref_params[2].finalCorners[0][2] = 564;
        ref_params[2].finalCorners[0][3] = 742;
        ref_params[2].finalCorners[0][4] = 925;
        ref_params[2].finalCorners[0][5] = 591;
        ref_params[2].finalCorners[0][6] = 834;
        ref_params[2].finalCorners[0][7] = 352;

        ref_params[2].finalCorners[1][0] = 530;
        ref_params[2].finalCorners[1][1] = 1228;
        ref_params[2].finalCorners[1][2] = 532;
        ref_params[2].finalCorners[1][3] = 1487;
        ref_params[2].finalCorners[1][4] = 759;
        ref_params[2].finalCorners[1][5] = 1662;
        ref_params[2].finalCorners[1][6] = 886;
        ref_params[2].finalCorners[1][7] = 1415;
        ref_params[2].numFPDetected = 8;

        ref_params[3].finalCorners[0][0] = 479;
        ref_params[3].finalCorners[0][1] = 325;
        ref_params[3].finalCorners[0][2] = 475;
        ref_params[3].finalCorners[0][3] = 468;
        ref_params[3].finalCorners[0][4] = 790;
        ref_params[3].finalCorners[0][5] = 294;
        ref_params[3].finalCorners[0][6] = 682;
        ref_params[3].finalCorners[0][7] = 202;

        ref_params[3].finalCorners[1][0] = 483;
        ref_params[3].finalCorners[1][1] = 1412;
        ref_params[3].finalCorners[1][2] = 491;
        ref_params[3].finalCorners[1][3] = 1568;
        ref_params[3].finalCorners[1][4] = 711;
        ref_params[3].finalCorners[1][5] = 1690;
        ref_params[3].finalCorners[1][6] = 828;
        ref_params[3].finalCorners[1][7] = 1578;
        ref_params[3].numFPDetected = 8;
    }
    else
    {
        read_lut_file(&lens_params,"psdkra/srv/applibTC_1mpix/LENS.BIN" );

        ref_params[0].finalCorners[0][0] = 325;
        ref_params[0].finalCorners[0][1] = 322;
        ref_params[0].finalCorners[0][2] = 306;
        ref_params[0].finalCorners[0][3] = 491;
        ref_params[0].finalCorners[0][4] = 530;
        ref_params[0].finalCorners[0][5] = 369;
        ref_params[0].finalCorners[0][6] = 488;
        ref_params[0].finalCorners[0][7] = 182;

        ref_params[0].finalCorners[1][0] = 291;
        ref_params[0].finalCorners[1][1] = 756;
        ref_params[0].finalCorners[1][2] = 289;
        ref_params[0].finalCorners[1][3] = 925;
        ref_params[0].finalCorners[1][4] = 430;
        ref_params[0].finalCorners[1][5] = 1083;
        ref_params[0].finalCorners[1][6] = 494;
        ref_params[0].finalCorners[1][7] = 901;
        ref_params[0].numFPDetected = 8;

        ref_params[1].finalCorners[0][0] = 272;
        ref_params[1].finalCorners[0][1] = 213;
        ref_params[1].finalCorners[0][2] = 246;
        ref_params[1].finalCorners[0][3] = 324;
        ref_params[1].finalCorners[0][4] = 481;
        ref_params[1].finalCorners[0][5] = 186;
        ref_params[1].finalCorners[0][6] = 440;
        ref_params[1].finalCorners[0][7] = 114;

        ref_params[1].finalCorners[1][0] = 241;
        ref_params[1].finalCorners[1][1] = 920;
        ref_params[1].finalCorners[1][2] = 268;
        ref_params[1].finalCorners[1][3] = 1044;
        ref_params[1].finalCorners[1][4] = 439;
        ref_params[1].finalCorners[1][5] = 1148;
        ref_params[1].finalCorners[1][6] = 486;
        ref_params[1].finalCorners[1][7] = 1064;
        ref_params[1].numFPDetected = 8;

        ref_params[2].finalCorners[0][0] = 361;
        ref_params[2].finalCorners[0][1] = 337;
        ref_params[2].finalCorners[0][2] = 366;
        ref_params[2].finalCorners[0][3] = 506;
        ref_params[2].finalCorners[0][4] = 603;
        ref_params[2].finalCorners[0][5] = 336;
        ref_params[2].finalCorners[0][6] = 502;
        ref_params[2].finalCorners[0][7] = 171;

        ref_params[2].finalCorners[1][0] = 371;
        ref_params[2].finalCorners[1][1] = 784;
        ref_params[2].finalCorners[1][2] = 373;
        ref_params[2].finalCorners[1][3] = 966;
        ref_params[2].finalCorners[1][4] = 533;
        ref_params[2].finalCorners[1][5] = 1127;
        ref_params[2].finalCorners[1][6] = 631;
        ref_params[2].finalCorners[1][7] = 938;
        ref_params[2].numFPDetected = 8;

        ref_params[3].finalCorners[0][0] = 268;
        ref_params[3].finalCorners[0][1] = 224;
        ref_params[3].finalCorners[0][2] = 239;
        ref_params[3].finalCorners[0][3] = 342;
        ref_params[3].finalCorners[0][4] = 493;
        ref_params[3].finalCorners[0][5] = 208;
        ref_params[3].finalCorners[0][6] = 448;
        ref_params[3].finalCorners[0][7] = 128;

        ref_params[3].finalCorners[1][0] = 236;
        ref_params[3].finalCorners[1][1] = 958;
        ref_params[3].finalCorners[1][2] = 264;
        ref_params[3].finalCorners[1][3] = 1070;
        ref_params[3].finalCorners[1][4] = 436;
        ref_params[3].finalCorners[1][5] = 1166;
        ref_params[3].finalCorners[1][6] = 482;
        ref_params[3].finalCorners[1][7] = 1095;
        ref_params[3].numFPDetected = 8;
    }

    VX_CALL(vxDirective((vx_reference)context, VX_DIRECTIVE_ENABLE_PERFORMANCE));

    /* Clearing all params */
    memset(&in_point_detect_params, 0, sizeof(svPointDetect_t));
    ASSERT_VX_OBJECT(point_detect_in_config = vxCreateUserDataObject(context, "svPointDetect_t",
                                       sizeof(svPointDetect_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    memset(&in_pose_estimation_params, 0, sizeof(svPoseEstimation_t));
    ASSERT_VX_OBJECT(pose_estimation_in_config = vxCreateUserDataObject(context, "svPoseEstimation_t",
                                       sizeof(svPoseEstimation_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    memset(&in_lens_params, 0, sizeof(svLdcLut_t));
    ASSERT_VX_OBJECT(in_lens_param_object = vxCreateUserDataObject(context, "svLdcLut_t",
                                       sizeof(svLdcLut_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    memset(&out_calmat_params, 0, sizeof(svACCalmatStruct_t));
    ASSERT_VX_OBJECT(out_calmat = vxCreateUserDataObject(context, "svACCalmatStruct_t",
                                       sizeof(svACCalmatStruct_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    /* Setting Point Detect Params */
    if (1 == is2MP)
    {
        in_point_detect_params.thresholdMode = 0;
        in_point_detect_params.windowMode = 0;
        in_point_detect_params.Ransac = 0;
        in_point_detect_params.SVROIWidth = 1920;
        in_point_detect_params.SVROIHeight = 1080;
        in_point_detect_params.binarizeOffset = 80;
        in_point_detect_params.borderOffset = 80;
        in_point_detect_params.smallestCenter = 10;
        in_point_detect_params.largestCenter = 50;
        in_point_detect_params.maxWinWidth  = 400;
        in_point_detect_params.maxWinHeight = 400;
        in_point_detect_params.maxBandLen = 400;
        in_point_detect_params.minBandLen = 4;
        in_point_detect_params.minSampleInCluster = 16;
        in_point_detect_params.firstROITop = 300;
        in_point_detect_params.firstROIBottom = 1050;
        in_point_detect_params.firstROILeft = 50;
        in_point_detect_params.firstROIRight = 900;
        in_point_detect_params.secondROITop = 300;
        in_point_detect_params.secondROIBottom = 1050;
        in_point_detect_params.secondROILeft = 950;
        in_point_detect_params.secondROIRight = 1870;
        in_point_detect_params.camera_id     = 3;
    }
    else
    {
        in_point_detect_params.thresholdMode = 0;
        in_point_detect_params.windowMode = 0;
        in_point_detect_params.Ransac = 0;
        in_point_detect_params.SVROIWidth = 1280;
        in_point_detect_params.SVROIHeight = 720;
        in_point_detect_params.binarizeOffset = 75;
        in_point_detect_params.borderOffset = 50;
        in_point_detect_params.smallestCenter = 2;
        in_point_detect_params.largestCenter = 50;
        in_point_detect_params.maxWinWidth = 180;
        in_point_detect_params.maxWinHeight = 180;
        in_point_detect_params.maxBandLen = 160;
        in_point_detect_params.minBandLen = 2;
        in_point_detect_params.minSampleInCluster = 4;
        in_point_detect_params.firstROITop = 150;
        in_point_detect_params.firstROIBottom = 670;
        in_point_detect_params.firstROILeft = 100;
        in_point_detect_params.firstROIRight = 600;
        in_point_detect_params.secondROITop = 150;
        in_point_detect_params.secondROIBottom = 670;
        in_point_detect_params.secondROILeft = 700;
        in_point_detect_params.secondROIRight = 1200;
        in_point_detect_params.camera_id     = 0;
    }

    /* Setting Lens Params */
    for (i=0;i<NUM_CAMERAS; i++) {
        LDC_Init(&in_lens_params.ldc[i],
             lens_params.ldcLUT_distortionCenters[i*2],
             lens_params.ldcLUT_distortionCenters[i*2+1],
             lens_params.ldcLUT_focalLength,
             lens_params.ldcLUT_D2U_table,
             lens_params.ldcLUT_D2U_length,
             lens_params.ldcLUT_D2U_step,
             lens_params.ldcLUT_U2D_table,
             lens_params.ldcLUT_U2D_length,
             lens_params.ldcLUT_U2D_step);
    }

    /* Setting Pose Estimation Params */
    in_pose_estimation_params.Ransac = 0;
    in_pose_estimation_params.SingleChartPose = 0;
    in_pose_estimation_params.numCameras = NUM_CAMERAS;

    if (1 == is2MP)
    {
        read_chartpos_file(in_pose_estimation_params.inChartPos ,"psdkra/srv/applibTC_2mpix/CHARTPOS.BIN");
    }
    else
    {
        read_chartpos_file(in_pose_estimation_params.inChartPos ,"psdkra/srv/applibTC_1mpix/CHARTPOS.BIN");
    }

    /* Copy data to user data objects */
    VX_CALL(vxCopyUserDataObject(point_detect_in_config, 0, sizeof(svPointDetect_t),
                                 &in_point_detect_params, VX_WRITE_ONLY,VX_MEMORY_TYPE_HOST));

    VX_CALL(vxCopyUserDataObject(pose_estimation_in_config, 0, sizeof(svPoseEstimation_t),
                                 &in_pose_estimation_params, VX_WRITE_ONLY,VX_MEMORY_TYPE_HOST));

    VX_CALL(vxCopyUserDataObject(in_lens_param_object, 0, sizeof(svLdcLut_t),
                                 &in_lens_params, VX_WRITE_ONLY,VX_MEMORY_TYPE_HOST));

    ASSERT_VX_OBJECT(in = vxCreateImage(context, width, height, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

    ASSERT_VX_OBJECT(in_array = vxCreateObjectArray(context, (vx_reference)in, NUM_CAMERAS), VX_TYPE_OBJECT_ARRAY);

    VX_CALL(vxReleaseImage(&in));

    for (i = 0; i < NUM_CAMERAS; i++)
    {
        in = (vx_image)vxGetObjectArrayItem((vx_object_array)in_array, i);

        if (1 == is2MP)
        {
            if (0 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_2mpix/FRONT_0.YUV", 1);
            }
            else if (1 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_2mpix/RIGHT_0.YUV", 1);
            }
            else if (2 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_2mpix/BACK_0.YUV", 1);
            }
            else if (3 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_2mpix/LEFT_0.YUV", 1);
            }
        }
        else
        {
            if (0 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_1mpix/FRONT_0.YUV", 1);
            }
            else if (1 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_1mpix/RIGHT_0.YUV", 1);
            }
            else if (2 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_1mpix/BACK_0.YUV", 1);
            }
            else if (3 == i)
            {
                ct_read_image2(in, "psdkra/srv/applibTC_1mpix/LEFT_0.YUV", 1);
            }
        }

        VX_CALL(vxReleaseImage(&in));
    }

    ASSERT_VX_OBJECT(buf_bwluma_frame = vxCreateImage(context, width, height, VX_DF_IMAGE_U8), VX_TYPE_IMAGE);

    ASSERT_VX_OBJECT(buf_bwluma_frame_array = vxCreateObjectArray(context, (vx_reference)buf_bwluma_frame, NUM_CAMERAS), VX_TYPE_OBJECT_ARRAY);

    VX_CALL(vxReleaseImage(&buf_bwluma_frame));

    /* Creating applib */
    create_params.vxContext = context;
    create_params.vxGraph   = graph;

    /* Data object */
    create_params.point_detect_in_config    = point_detect_in_config;
    create_params.in_ldclut                 = in_lens_param_object;
    create_params.in_array                  = in_array;
    create_params.buf_bwluma_frame_array    = buf_bwluma_frame_array;
    create_params.pose_estimation_in_config = pose_estimation_in_config;
    create_params.out_calmat                = out_calmat;

    srv_handle = srv_calib_create(&create_params);

    /* Verifying applib handle is not NULL */
    ASSERT(srv_handle != NULL);

    VX_CALL(vxVerifyGraph(graph));
    VX_CALL(vxProcessGraph(graph));

    in_corners = srv_calib_get_corners(srv_handle);

    vx_user_data_object corner_element;

    for (i = 0; i < NUM_CAMERAS; i++)
    {
        corner_element = (vx_user_data_object)vxGetObjectArrayItem(in_corners, i);

        VX_CALL(vxCopyUserDataObject(corner_element, 0, sizeof(svACDetectStructFinalCorner_t), &out_params[i], VX_READ_ONLY, VX_MEMORY_TYPE_HOST));  

        #ifdef DEBUG_TEST_1
        printf("Test bench Final Corners 1 = %d %d %d %d %d %d %d %d\n",out_params[i].finalCorners[0][0]>>4,
        out_params[i].finalCorners[0][1]>>4,out_params[i].finalCorners[0][2]>>4, out_params[i].finalCorners[0][3]>>4,
        out_params[i].finalCorners[0][4]>>4,out_params[i].finalCorners[0][5]>>4, out_params[i].finalCorners[0][6]>>4,
        out_params[i].finalCorners[0][7]>>4);

        printf("Test bench Final Corners 2 = %d %d %d %d %d %d %d %d\n",out_params[i].finalCorners[1][0]>>4,
        out_params[i].finalCorners[1][1]>>4,out_params[i].finalCorners[1][2]>>4, out_params[i].finalCorners[1][3]>>4,
        out_params[i].finalCorners[1][4]>>4,out_params[i].finalCorners[1][5]>>4, out_params[i].finalCorners[1][6]>>4,
        out_params[i].finalCorners[1][7]>>4);
        printf("numFPDetected = %d \n",out_params[i].numFPDetected);

        #endif
        ASSERT_NO_FAILURE(point_detect_corners_check(ref_params[i], out_params[i]));
        VX_CALL(vxReleaseUserDataObject(&corner_element));
    }

    /* TODO: Error checking */
    #if 1
    if (is2MP == 1)
    {
        calmat_ref.calMatBuf[0] = 1070742123;
        calmat_ref.calMatBuf[1] = -32891438;
        calmat_ref.calMatBuf[2] = -72699208;
        calmat_ref.calMatBuf[3] = 31174890;
        calmat_ref.calMatBuf[4] = -539538345;
        calmat_ref.calMatBuf[5] = 927853288;
        calmat_ref.calMatBuf[6] = -64952709;
        calmat_ref.calMatBuf[7] = -927371895;
        calmat_ref.calMatBuf[8] = -537076076;
        calmat_ref.calMatBuf[9] = -429880;
        calmat_ref.calMatBuf[10] = 608630;
        calmat_ref.calMatBuf[11] = -604619;
        calmat_ref.calMatBuf[12] = -49294633;
        calmat_ref.calMatBuf[13] = -604632800;
        calmat_ref.calMatBuf[14] = 885834031;
        calmat_ref.calMatBuf[15] = -1072816806;
        calmat_ref.calMatBuf[16] = 38033270;
        calmat_ref.calMatBuf[17] = -27345079;
        calmat_ref.calMatBuf[18] = -15979104;
        calmat_ref.calMatBuf[19] = -886326285;
        calmat_ref.calMatBuf[20] = -605857993;
        calmat_ref.calMatBuf[21] = 608190;
        calmat_ref.calMatBuf[22] = 455655;
        calmat_ref.calMatBuf[23] = -346865;
        calmat_ref.calMatBuf[24] = -1070731579;
        calmat_ref.calMatBuf[25] = 69250563;
        calmat_ref.calMatBuf[26] = 40568559;
        calmat_ref.calMatBuf[27] = 13677241;
        calmat_ref.calMatBuf[28] = 668524427;
        calmat_ref.calMatBuf[29] = -840132975;
        calmat_ref.calMatBuf[30] = -79442518;
        calmat_ref.calMatBuf[31] = -837260896;
        calmat_ref.calMatBuf[32] = -667532321;
        calmat_ref.calMatBuf[33] = 414814;
        calmat_ref.calMatBuf[34] = -73208;
        calmat_ref.calMatBuf[35] = 356022;
        calmat_ref.calMatBuf[36] = -35290446;
        calmat_ref.calMatBuf[37] = 686879682;
        calmat_ref.calMatBuf[38] = -824662326;
        calmat_ref.calMatBuf[39] = 1073111241;
        calmat_ref.calMatBuf[40] = 7436550;
        calmat_ref.calMatBuf[41] = -33205151;
        calmat_ref.calMatBuf[42] = -15530084;
        calmat_ref.calMatBuf[43] = -825269369;
        calmat_ref.calMatBuf[44] = -686720710;
        calmat_ref.calMatBuf[45] = -583028;
        calmat_ref.calMatBuf[46] = -16943;
        calmat_ref.calMatBuf[47] = 326414;
    }
    else
    {
        calmat_ref.calMatBuf[0] = 1071273520;
        calmat_ref.calMatBuf[1] = -66350337;
        calmat_ref.calMatBuf[2] = 27993824;
        calmat_ref.calMatBuf[3]  = -66438059;
        calmat_ref.calMatBuf[4]  = -682853628;
        calmat_ref.calMatBuf[5]  = 826030782;
        calmat_ref.calMatBuf[6]  = -33240519;
        calmat_ref.calMatBuf[7]  = -825864039;
        calmat_ref.calMatBuf[8]  = -685389337;
        calmat_ref.calMatBuf[9]  = -361386;
        calmat_ref.calMatBuf[10]  = 681582;
        calmat_ref.calMatBuf[11]  = -540433;
        calmat_ref.calMatBuf[12]  = -6634208;
        calmat_ref.calMatBuf[13]  = -897625830;
        calmat_ref.calMatBuf[14]  = 589170481;
        calmat_ref.calMatBuf[15]  = -1073685111;
        calmat_ref.calMatBuf[16]  = 3999270;
        calmat_ref.calMatBuf[17]  = -11367770;
        calmat_ref.calMatBuf[18]  = 7308789;
        calmat_ref.calMatBuf[19]  = -589209599;
        calmat_ref.calMatBuf[20]  = -897603130;
        calmat_ref.calMatBuf[21]  = 553791;
        calmat_ref.calMatBuf[22]  = 568372;
        calmat_ref.calMatBuf[23]  = -184220;
        calmat_ref.calMatBuf[24]  = -1072871665;
        calmat_ref.calMatBuf[25]  = -22564608;
        calmat_ref.calMatBuf[26]  = 36950395;
        calmat_ref.calMatBuf[27]  = -45705011;
        calmat_ref.calMatBuf[28]  = 541639524;
        calmat_ref.calMatBuf[29]  = -925987324;
        calmat_ref.calMatBuf[30]  = 820258;
        calmat_ref.calMatBuf[31]  = -926809740;
        calmat_ref.calMatBuf[32]  = -542161068;
        calmat_ref.calMatBuf[33]  = 420342;
        calmat_ref.calMatBuf[34]  = 18407;
        calmat_ref.calMatBuf[35]  = 320590;
        calmat_ref.calMatBuf[36]  = 13903828;
        calmat_ref.calMatBuf[37]  = 921767319;
        calmat_ref.calMatBuf[38]  = -550379385;
        calmat_ref.calMatBuf[39]  = 1073744724;
        calmat_ref.calMatBuf[40]  = -8528633;
        calmat_ref.calMatBuf[41]  = -8689100;
        calmat_ref.calMatBuf[42]  = -11830881;
        calmat_ref.calMatBuf[43]  = -550268358;
        calmat_ref.calMatBuf[44]  = -921880247;
        calmat_ref.calMatBuf[45]  = -557518;
        calmat_ref.calMatBuf[46]  = -122494;
        calmat_ref.calMatBuf[47]  = 257398;
    }

    VX_CALL(vxCopyUserDataObject(out_calmat, 0, sizeof(svACCalmatStruct_t), &out_calmat_params, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));

    for (idx=0;idx<NUM_CAMERAS;idx++) {
        for (jdx=0;jdx<12;jdx++ ) {
          calmat_out.calMatBuf[12*idx + jdx] = out_calmat_params.outcalmat[12*idx + jdx];
        }
    }

    //write_calmat_file(&calmat_out, "output/CALMAT.BIN");

    ASSERT_NO_FAILURE(pose_estimation_calmat_check ( calmat_ref , calmat_out));
    #endif

    /* Deleting applib */
    srv_calib_delete(srv_handle);

    VX_CALL(vxReleaseObjectArray(&buf_bwluma_frame_array));
    VX_CALL(vxReleaseObjectArray(&in_array));
    VX_CALL(vxReleaseUserDataObject(&point_detect_in_config));
    VX_CALL(vxReleaseUserDataObject(&pose_estimation_in_config));
    VX_CALL(vxReleaseUserDataObject(&in_lens_param_object));
    VX_CALL(vxReleaseUserDataObject(&out_calmat));
    VX_CALL(vxReleaseGraph(&graph));

    ASSERT(in_array == 0);
    ASSERT(buf_bwluma_frame_array == 0);
    ASSERT(point_detect_in_config == 0);
    ASSERT(pose_estimation_in_config == 0);
    ASSERT(in_lens_param_object == 0);
    ASSERT(buf_bwluma_frame == 0);
    ASSERT(out_calmat == 0);
    ASSERT(in == 0);

    tivxSrvUnLoadKernels(context);
}

TESTCASE_TESTS(tivxSrvCalibApplib,
               testApplib)

