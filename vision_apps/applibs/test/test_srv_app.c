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
#include "srv_bowl_lut_gen_applib/srv_bowl_lut_gen_applib.h"
#include "lens_distortion_correction.h"
#include "srv_common.h"
#include <TI/tivx_task.h>
#include <render.h>

#define LOG_RT_TRACE_ENABLE       (0u)

#define MAX_NUM_BUF               (8u)
#define MAX_IMAGE_PLANES          (3u)
#define MAX_NUM_OBJ_ARR_ELEMENTS  (4u)
#define MAX_ABS_FILENAME          (1024u)

#define SV_OUT_DISPLAY_HEIGHT   (1080)
#define SV_OUT_DISPLAY_WIDTH    (1080)
#define SV_SUBSAMPLE            (4)
#define SV_XYZLUT3D_SIZE        (SV_OUT_DISPLAY_HEIGHT/SV_SUBSAMPLE) * (SV_OUT_DISPLAY_WIDTH/SV_SUBSAMPLE) *3
#define SV_GPULUT_SIZE          ((2 + SV_OUT_DISPLAY_WIDTH/SV_SUBSAMPLE) * (2 + SV_OUT_DISPLAY_HEIGHT/SV_SUBSAMPLE)) *7

TESTCASE(tivxSrvApp,  CT_VXContext, ct_setup_vx_context, 0)

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

/* lut_size is the number of entries in the LUT */
static void read_gpulut_file(vx_uint16 *gpulut, const char*fileName, vx_int32 lut_size)
{
    char file[MAXPATHLENGTH];
    FILE* f = 0;
    size_t sz;
    uint32_t  read_size;

    printf ("Reading gpulut file \n");

    if (!fileName)
    {
        printf("gpulut file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "rb");
    if (!f)
    {
        CT_ADD_FAILURE("Cant open gpulut file: %s\n", fileName);
        return;
    }

    read_size = fread((int16_t *)gpulut,sizeof(int16_t),lut_size,f);
    if (read_size != lut_size)
    {
        CT_ADD_FAILURE("Incorrect Bytes read from gpulut file: %s\n", fileName);
        fclose(f);
        return;
    }

    printf ("file read completed \n");
    fclose(f);

}


static void gpulut_generation_lut_check(uint16_t *ref_lut, uint16_t *out_lut, int size)
{
    int32_t   i;
    int16_t  reference,out;
    for (i=0;i<size;i++)
    {
        reference = ref_lut[i];
        out       = out_lut[i];
        if (abs(reference -out ) >1) {
            printf ("Failure at location i = %d ref = %d out = %d \n",i,reference,out);
        }
        ASSERT(abs(reference -out)<2);
    } //i

}

static void read_calmat_file( svCalmat_t *calmat, const char*fileName)
{
    char file[MAXPATHLENGTH];
    uint32_t cnt;
    FILE* f = 0;
    size_t sz;
    uint32_t  read_size;

    printf ("Reading calmat file \n");

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

    read_size = fread((int8_t *)&calmat->numCameras,sizeof(uint8_t),4,f);
    if (read_size != 4)
    {
        CT_ADD_FAILURE("Incorrect Bytes read from calmat file: %s\n", fileName);
        fclose(f);
        return;
    }

    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        read_size = fread((int8_t *)&calmat->calMatSize[cnt],sizeof(uint8_t),4,f);
        if (read_size != 4)
        {
            CT_ADD_FAILURE("Incorrect Bytes read from calmat file: %s\n", fileName);
            fclose(f);
            return;
        }
    }

#if 0
    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        printf("Calmat size for cnt %d = %d \n",cnt,calmat->calMatSize[cnt]);
    }
#endif

    /* Set Pointer ahead by 128 bytes to skip over metadata */
    fseek(f,128,SEEK_SET);

    /* Read calmat per camera */
    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        read_size = fread((int8_t *)&calmat->calMatBuf + 48*cnt,sizeof(uint8_t),calmat->calMatSize[cnt],f);
        if (read_size != calmat->calMatSize[cnt])
        {
            CT_ADD_FAILURE("Incorrect Bytes read from calmat file: %s\n", fileName);
            fclose(f);
            return;
        }
    }

#if 0
    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        printf ("For Camera = %d Ref calmat[0] = %d Ref Calmat[11] = %d \n",cnt,calmat->calMatBuf[12*cnt+0],calmat->calMatBuf[12*cnt +11]);
    }
#endif
    printf ("file read completed \n");
    fclose(f);

}

/* lut_size is the number of entries in the LUT */
static void read_xyzlut3d_file(vx_float32 *xyzlut3d, const char*fileName, vx_int32 lut_size)
{
    char file[MAXPATHLENGTH];
    FILE* f = 0;
    size_t sz;

    printf ("Reading xyzlut3d file \n");

    if (!fileName)
    {
        printf("xyzlut3d file name not specified\n");
        return;
    }

    sz = snprintf(file, MAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), fileName);
    ASSERT_(return, (sz < MAXPATHLENGTH));

    f = fopen(file, "rb");
    if (!f)
    {
        CT_ADD_FAILURE("Cant open xyzlut3d file: %s\n", fileName);
        return;
    }

    fread((float *)xyzlut3d,sizeof(float),lut_size,f);

    fclose(f);

}

static void bowl_generation_calmat_check(svACCalmatStruct_t *ref_calmat, svACCalmatStruct_t *out_calmat)
{
    int32_t   i,j;
    float  reference,out;

    for (i=0;i<4;i++)
    {
        #ifdef DEBUG_TEST_2
        printf ("\nout_calmat for row  %d =",i);
        #endif
        for (j=0;j<12;j++) {
            reference = ref_calmat->scaled_outcalmat[12*i+j];
            out       = out_calmat->scaled_outcalmat[12*i+j];
            #ifdef DEBUG_TEST_2
            printf("%f  ",out);
            #endif
            if (fabs(reference -out)>0.0001) {
                printf("Failure @ camera = %d and location = %d  reference = %f out = %f \n",i,j,reference,out);
            }
            ASSERT(fabs(reference - out) < 0.0001);
        } //j
    } //i

}

static void bowl_generation_3dlut_check(float *ref_lut, float *out_lut, int size)
{
    int32_t   i;
    float  reference,out;
    for (i=0;i<size;i++)
    {
        reference = ref_lut[i];
        out       = out_lut[i];
        if (reference != out ) {
            printf ("Failure at location i = %d ref = %f out = %f \n",i,reference,out);
        }
        ASSERT(reference == out);
    } //i

}

static void make_filename(char *abs_filename, char *filename_prefix)
{
    snprintf(abs_filename, MAX_ABS_FILENAME, "%s/psdkra/srv/standalone/%s.bmp",
        ct_get_test_file_path(), filename_prefix);
}

static int load_from_raw_file(vx_image copy_image, int width, int height, const char* filename, int offset)
{
    void* data;
    FILE* fp;
    int dataread;
    int numbytes = 3 * width * height;
    char file[SRVMAXPATHLENGTH];
    vx_rectangle_t rect             = { 0, 0, width, height };
    vx_imagepatch_addressing_t addr = VX_IMAGEPATCH_ADDR_INIT;


    addr.dim_x = width;
    addr.dim_y = height;
    addr.stride_x = 3;
    addr.stride_y = width*3;

    snprintf(file, SRVMAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), filename);
    fp = fopen(file, "rb");

    if(!fp)
    {
        printf("Error opening file %s\n", filename);
        return -1;
    }

    data = ct_alloc_mem(numbytes);
    if (NULL != data)
    {
        memset(data, 0, numbytes);
        fseek(fp, offset, SEEK_CUR);
        dataread = fread(data, 1, numbytes, fp);
        fclose(fp);
        if(dataread != numbytes) {
            printf("Error in file size != width*height\n");
            return -1;
        }

        vxCopyImagePatch(copy_image, &rect, 0, &addr, data, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

        ct_free_mem(data);
    }
    else
    {
        fclose(fp);
    }

    return 0;
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

static int get_offset(const char *filename)
{
    FILE *fp;
    uint8_t header[54], inter;
    uint32_t offset;
    char file[SRVMAXPATHLENGTH];
    uint32_t  read_size;

    snprintf(file, SRVMAXPATHLENGTH, "%s/%s", ct_get_test_file_path(), filename);
    fp = fopen(file, "rb");

    if(!fp)
    {
        printf("Error opening file %s\n", filename);
        return -1;
    }

    read_size = fread(header, sizeof(uint8_t), 54, fp);
    if (read_size != 54)
    {
        CT_ADD_FAILURE("Incorrect Bytes read from file: %s\n", filename);
        fclose(fp);
        return -1;
    }

    inter = *(uint8_t *)(&header[10]);

    offset = (uint32_t)inter;

    fclose(fp);

    return offset;
}

/* Loading files to object array */
static vx_status load_input_images(vx_context context, vx_object_array input_array, int in_width, int in_height, int num_cameras)
{
    vx_status status = VX_SUCCESS;
    vx_image copy_image, input_image;
    CT_Image ct_read;
    int i, offset;
    char filename[MAX_ABS_FILENAME];

    for (i = 0; i < num_cameras; i++)
    {
        copy_image = vxCreateImage(context, in_width, in_height, VX_DF_IMAGE_RGB);

        input_image = (vx_image)vxGetObjectArrayItem((vx_object_array)input_array, i);

        if (i == 0)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_1mpix/FRONT_0.bmp");
        }
        else if (i == 1)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_1mpix/RIGHT_0.bmp");
        }
        else if (i == 2)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_1mpix/BACK_0.bmp");
        }
        else if (i == 3)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_1mpix/LEFT_0.bmp");
        }

        offset = get_offset(filename);
        if (offset < 0)
        {
            status = offset;
            return status;
        }

        status = load_from_raw_file(copy_image, in_width, in_height, filename, offset);
        if (VX_SUCCESS != status)
        {
            return status;
        }

        vxuColorConvert(context, copy_image, input_image);

        vxReleaseImage(&input_image);
        vxReleaseImage(&copy_image);
    }
    return status;
}

static vx_status load_input_images_2mp(vx_context context, vx_object_array input_array, int in_width, int in_height, int num_cameras)
{
    vx_status status = VX_SUCCESS;
    vx_image copy_image, input_image;
    CT_Image ct_read;
    int i, offset;
    char filename[MAX_ABS_FILENAME];

    for (i = 0; i < num_cameras; i++)
    {
        copy_image = vxCreateImage(context, in_width, in_height, VX_DF_IMAGE_RGB);

        input_image = (vx_image)vxGetObjectArrayItem((vx_object_array)input_array, i);

        if (i == 0)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_2mpix/FRONT_0.bmp");
        }
        else if (i == 1)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_2mpix/RIGHT_0.bmp");
        }
        else if (i == 2)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_2mpix/BACK_0.bmp");
        }
        else if (i == 3)
        {
            snprintf(filename, SRVMAXPATHLENGTH, "%s", "psdkra/srv/applibTC_2mpix/LEFT_0.bmp");
        }

        offset = get_offset(filename);
        status = load_from_raw_file(copy_image, in_width, in_height, filename, offset);
        if (VX_SUCCESS != status)
        {
            return status;
        }

        vxuColorConvert(context, copy_image, input_image);

        vxReleaseImage(&input_image);
        vxReleaseImage(&copy_image);
    }
    return status;
}

typedef struct {
    const char* name;
    int is2MP;
} Arg;

#define PARAMETERS \
    CT_GENERATE_PARAMETERS("srv_bowl_lut_gen", ARG, 0), \

TEST_WITH_ARG(tivxSrvApp, testApplib, Arg, PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_graph graph1, graph2;
    vx_uint32 i;
    srv_bowl_lut_gen_handle srv_handle = NULL;
    srv_bowl_lut_gen_createParams create_params;
    vx_uint8 is2MP = 0;

    /* Applib data */
    svGpuLutGen_t               in_params;
    svACCalmatStruct_t          in_calmat;
    svGeometric_t               in_offset;
    ldc_lensParameters          lens_params;
    svLdcLut_t                  in_lens_params;
    svCalmat_t                  calmat_file;
    uint16_t                    out_gpulut[SV_GPULUT_SIZE];

    vx_map_id map_id_2;
    vx_size item_size_2;
    uint32_t *data_ptr_2;
    vx_uint16 *returned_gpulut;
    vx_uint16 ref_gpulut[SV_GPULUT_SIZE];

    /* Applib API objects */
    vx_user_data_object         in_config;
    vx_user_data_object         in_calmat_object;
    vx_user_data_object         in_offset_object;
    vx_user_data_object         in_lens_param_object;
    vx_array                    out_gpulut_array;

    /* OpenGL node objects */
    vx_node node;
    vx_object_array input_array, srv_views_array;
    vx_image img_exemplar, output_image;
    vx_user_data_object param_obj, srv_views;
    vx_uint32 in_width, in_height, out_width, out_height, num_cameras, num_views;
    vx_status status;
    char filename[MAX_ABS_FILENAME];
    srv_coords_t local_srv_coords;
    tivx_srv_params_t params;
    CT_Image ct_output_ref, ct_output_tst;

    is2MP = arg_->is2MP;
    out_width = 1920;
    out_height = 1080;
    num_cameras = 4;
    num_views = 1;

    /* TODO: using 2mp */
    if (1 == is2MP)
    {
        read_calmat_file(&calmat_file, "psdkra/srv/applibTC_2mpix/CALMAT.BIN");
        read_gpulut_file(ref_gpulut,"psdkra/srv/GAlignLUT3D.bin",SV_GPULUT_SIZE);
        in_width = 1920;
        in_height = 1080;
    }
    else
    {
        read_calmat_file(&calmat_file, "psdkra/srv/applibTC_1mpix/CALMAT.BIN");
        in_width = 1280;
        in_height = 720;
    }

    memset(&params, 0, sizeof(tivx_srv_params_t));
    params.cam_bpp = 24;

    ASSERT_VX_OBJECT(graph1 = vxCreateGraph(context), VX_TYPE_GRAPH);

    ASSERT_VX_OBJECT(graph2 = vxCreateGraph(context), VX_TYPE_GRAPH);

    tivxSrvLoadKernels(context);

    /* Populating data objects */
    memset(&in_params, 0, sizeof(svGpuLutGen_t));
    ASSERT_VX_OBJECT(in_config = vxCreateUserDataObject(context, "svGpuLutGen_t",
                                       sizeof(svGpuLutGen_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    memset(&in_calmat, 0, sizeof(svACCalmatStruct_t));
    ASSERT_VX_OBJECT(in_calmat_object = vxCreateUserDataObject(context, "svACCalmatStruct_t",
                                       sizeof(svACCalmatStruct_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    memset(&in_offset, 0, sizeof(svGeometric_t));
    ASSERT_VX_OBJECT(in_offset_object = vxCreateUserDataObject(context, "svGeometric_t",
                                       sizeof(svGeometric_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    memset(&in_lens_params, 0, sizeof(svLdcLut_t));
    ASSERT_VX_OBJECT(in_lens_param_object = vxCreateUserDataObject(context, "svLdcLut_t",
                                       sizeof(svLdcLut_t), NULL), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    ASSERT_VX_OBJECT(out_gpulut_array = vxCreateArray(context, VX_TYPE_UINT16, SV_GPULUT_SIZE), VX_TYPE_ARRAY);

    VX_CALL(vxAddArrayItems(out_gpulut_array, 1, out_gpulut, sizeof(uint16_t)*SV_GPULUT_SIZE));

    /* Creating objects for OpenGL node */
    ASSERT_VX_OBJECT(output_image = vxCreateImage(context, out_width, out_height, VX_DF_IMAGE_RGBX), VX_TYPE_IMAGE);

    ASSERT_VX_OBJECT(param_obj = vxCreateUserDataObject(context, "tivx_srv_params_t", sizeof(tivx_srv_params_t), &params), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    ASSERT_VX_OBJECT(img_exemplar = vxCreateImage(context, in_width, in_height, VX_DF_IMAGE_NV12), VX_TYPE_IMAGE);

    ASSERT_VX_OBJECT(srv_views = vxCreateUserDataObject(context, "srv_coords_t", sizeof(srv_coords_t), &local_srv_coords), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

    ASSERT_VX_OBJECT(srv_views_array = vxCreateObjectArray(context, (vx_reference)srv_views, num_views), VX_TYPE_OBJECT_ARRAY);

    VX_CALL(vxReleaseUserDataObject(&srv_views));

    for (i = 0; i < num_views; i++)
    {
        local_srv_coords.camx =     0.0f;
        local_srv_coords.camy =     0.0f;
        local_srv_coords.camz =   240.0f;
        local_srv_coords.targetx =  0.0f;
        local_srv_coords.targety =  0.0f;
        local_srv_coords.targetz =  0.0f;
        local_srv_coords.anglex =   0.0f;
        local_srv_coords.angley =   0.0f;
        local_srv_coords.anglez =   0.0f;

        ASSERT_VX_OBJECT(srv_views = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)srv_views_array, i), (enum vx_type_e)VX_TYPE_USER_DATA_OBJECT);

        VX_CALL(vxCopyUserDataObject(srv_views, 0, sizeof(srv_coords_t), &local_srv_coords, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

        VX_CALL(vxReleaseUserDataObject(&srv_views));
    }

    ASSERT_VX_OBJECT(input_array = vxCreateObjectArray(context, (vx_reference)img_exemplar, num_cameras), VX_TYPE_OBJECT_ARRAY);

    /* Populate input data */
    in_params.SVInCamFrmHeight   = 1080;
    in_params.SVInCamFrmWidth    = 1920;
    in_params.SVOutDisplayHeight = SV_OUT_DISPLAY_HEIGHT;
    in_params.SVOutDisplayWidth  = SV_OUT_DISPLAY_WIDTH;
    in_params.numCameras         = 4;
    in_params.subsampleratio     = SV_SUBSAMPLE;
    in_params.useWideBowl        = 1;

    in_offset.offsetXleft     = -400;
    in_offset.offsetXright    = 400;
    in_offset.offsetYfront    = -450;
    in_offset.offsetYback     = 450;

    /* TODO: using 2mp */
    if (1 == is2MP)
    {
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, load_input_images_2mp(context, input_array, in_width, in_height, num_cameras));
    }
    else
    {
        ASSERT_EQ_VX_STATUS(VX_SUCCESS, load_input_images(context, input_array, in_width, in_height, num_cameras));
    }

    VX_CALL(vxReleaseImage(&img_exemplar));

    for (int idx=0;idx<48;idx++) {
        in_calmat.outcalmat[idx] = calmat_file.calMatBuf[idx];
        //printf("in_calmat for index %d = %d \n",idx,in_calmat.outcalmat[idx]);
    }

    /* Read Lens file */
    if (1 == is2MP)
    {
        read_lut_file(&lens_params,"psdkra/srv/applibTC_2mpix/LENS.BIN" );
    }
    else
    {
        read_lut_file(&lens_params,"psdkra/srv/applibTC_1mpix/LENS.BIN" );
    }

    /* Initialize all the 4 channel ldc luts within in_params    */

    for (int i=0;i<4; i++) {
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

    /* Copying to user data objects */
    VX_CALL(vxCopyUserDataObject(in_config, 0, sizeof(svGpuLutGen_t), &in_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

    VX_CALL(vxCopyUserDataObject(in_calmat_object, 0, sizeof(svACCalmatStruct_t), &in_calmat, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

    VX_CALL(vxCopyUserDataObject(in_offset_object, 0, sizeof(svGeometric_t), &in_offset, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

    VX_CALL(vxCopyUserDataObject(in_lens_param_object, 0, sizeof(svLdcLut_t), &in_lens_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));

    /* Creating applib */
    create_params.vxContext = context;
    create_params.vxGraph   = graph1;

    /* Data object */
    create_params.in_config    = in_config;
    create_params.in_calmat    = in_calmat_object;
    create_params.in_offset    = in_offset_object;
    create_params.in_ldclut    = in_lens_param_object;
    create_params.out_gpulut3d = out_gpulut_array;

    srv_handle = srv_bowl_lut_gen_create(&create_params);

    /* Verifying applib handle is not NULL */
    ASSERT(srv_handle != NULL);

    /* TODO: Replace NULL with GPULUT output */
    ASSERT_VX_OBJECT(node = tivxGlSrvNode(graph2, param_obj, input_array, srv_views_array, out_gpulut_array, output_image), VX_TYPE_NODE);
    vxSetNodeTarget(node, VX_TARGET_STRING, TIVX_TARGET_MPU_0);

    VX_CALL(vxVerifyGraph(graph1));
    VX_CALL(vxProcessGraph(graph1));

    VX_CALL(vxVerifyGraph(graph2));
    VX_CALL(vxProcessGraph(graph2));

    tivxTaskWaitMsecs(5000);

#if 0
    /* Map primary output-2 */
    vxMapArrayRange(out_gpulut_array, 0, 1, &map_id_2, &item_size_2,
                   (void *)&data_ptr_2,
                   VX_READ_ONLY,
                   VX_MEMORY_TYPE_HOST, 0);

    returned_gpulut = (vx_uint16 *)data_ptr_2;
    ASSERT_NO_FAILURE(gpulut_generation_lut_check (ref_gpulut, returned_gpulut,  SV_GPULUT_SIZE ));

    vxUnmapArrayRange(out_gpulut_array, map_id_2);
#endif
    /* Deleting applib */
    srv_bowl_lut_gen_delete(srv_handle);

    VX_CALL(vxReleaseUserDataObject(&param_obj));
    VX_CALL(vxReleaseObjectArray(&input_array));
    VX_CALL(vxReleaseObjectArray(&srv_views_array));
    VX_CALL(vxReleaseImage(&output_image));

    VX_CALL(vxReleaseUserDataObject(&in_config));
    VX_CALL(vxReleaseUserDataObject(&in_calmat_object));
    VX_CALL(vxReleaseUserDataObject(&in_offset_object));
    VX_CALL(vxReleaseUserDataObject(&in_lens_param_object));
    VX_CALL(vxReleaseArray(&out_gpulut_array));

    VX_CALL(vxReleaseNode(&node));
    VX_CALL(vxReleaseGraph(&graph1));
    VX_CALL(vxReleaseGraph(&graph2));

    ASSERT(in_config == 0);
    ASSERT(in_calmat_object == 0);
    ASSERT(in_offset_object == 0);
    ASSERT(in_lens_param_object == 0);
    ASSERT(out_gpulut_array == 0);

    tivxSrvUnLoadKernels(context);
}

TESTCASE_TESTS(tivxSrvApp,
               testApplib)

