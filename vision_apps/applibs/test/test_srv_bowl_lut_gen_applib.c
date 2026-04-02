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

#define SV_OUT_DISPLAY_HEIGHT   (1080)
#define SV_OUT_DISPLAY_WIDTH    (1080)
#define SV_SUBSAMPLE            (4)
#define SV_XYZLUT3D_SIZE        (SV_OUT_DISPLAY_HEIGHT/SV_SUBSAMPLE) * (SV_OUT_DISPLAY_WIDTH/SV_SUBSAMPLE) *3
#define SV_GPULUT_SIZE          ((2 + SV_OUT_DISPLAY_WIDTH/SV_SUBSAMPLE) * (2 + SV_OUT_DISPLAY_HEIGHT/SV_SUBSAMPLE)) *7

TESTCASE(tivxSrvBowlLutGenApplib,  CT_VXContext, ct_setup_vx_context, 0)

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

typedef struct {
    const char* name;
    int is2MP;
} Arg;

#define PARAMETERS \
    CT_GENERATE_PARAMETERS("srv_bowl_lut_gen", ARG, 1), \

TEST_WITH_ARG(tivxSrvBowlLutGenApplib, testApplib, Arg, PARAMETERS)
{
    vx_context context = context_->vx_context_;
    vx_graph graph;
    vx_node node;
    vx_uint32 i;
    srv_bowl_lut_gen_handle srv_handle = NULL;
    srv_bowl_lut_gen_createParams create_params;
    vx_uint8 is2MP = 0;
    int idx;

    /* Applib data */
    svGpuLutGen_t               in_params;
    svACCalmatStruct_t          in_calmat;
    svGeometric_t               in_offset;
    ldc_lensParameters          lens_params;
    svLdcLut_t                  in_lens_params;
    svCalmat_t                  calmat_file;
    uint16_t                    *out_gpulut = (uint16_t*)ct_alloc_mem(sizeof(uint16_t)*SV_GPULUT_SIZE);

    vx_map_id map_id_2;
    vx_size item_size_2;
    uint32_t *data_ptr_2;
    vx_uint16 *returned_gpulut;
    vx_uint16 *ref_gpulut = (vx_uint16*)ct_alloc_mem(sizeof(vx_uint16)*SV_GPULUT_SIZE);

    /* Applib API objects */
    vx_user_data_object         in_config;
    vx_user_data_object         in_calmat_object;
    vx_user_data_object         in_offset_object;
    vx_user_data_object         in_lens_param_object;
    vx_array                    out_gpulut_array;

    is2MP = arg_->is2MP;

    ASSERT_VX_OBJECT(graph = vxCreateGraph(context), VX_TYPE_GRAPH);

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
        read_calmat_file(&calmat_file, "psdkra/srv/calmat_imx390_test_3dbowl.bin");
        read_gpulut_file(ref_gpulut,"psdkra/srv/GAlignLUT3D.bin",SV_GPULUT_SIZE);
    }
    else
    {
        read_calmat_file(&calmat_file, "psdkra/srv/calmat_imx390_test_3dbowl.bin");
        read_gpulut_file(ref_gpulut,"psdkra/srv/GAlignLUT3D.bin",SV_GPULUT_SIZE);
    }

    for (idx=0;idx<48;idx++) {
        in_calmat.outcalmat[idx] = calmat_file.calMatBuf[idx];
        //printf("in_calmat for index %d = %d \n",idx,in_calmat.outcalmat[idx]); 
    }

    /* Read Lens file */
    if (1 == is2MP)
    {
        read_lut_file(&lens_params,"psdkra/srv/LENS_IMX390.bin" );
    }
    else
    {
        read_lut_file(&lens_params,"psdkra/srv/LENS_IMX390.bin" );
    }
    /* Initialize all the 4 channel ldc luts within in_params    */ 

    for (i=0;i<4; i++) {
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
    create_params.vxGraph   = graph;

    /* Data object */
    create_params.in_config    = in_config;
    create_params.in_calmat    = in_calmat_object;
    create_params.in_offset    = in_offset_object;
    create_params.in_ldclut    = in_lens_param_object;
    create_params.out_gpulut3d = out_gpulut_array;

    srv_handle = srv_bowl_lut_gen_create(&create_params);

    /* Verifying applib handle is not NULL */
    ASSERT(srv_handle != NULL);

    VX_CALL(vxVerifyGraph(graph));
    VX_CALL(vxProcessGraph(graph));

    /* Map primary output-2 */
    vxMapArrayRange(out_gpulut_array, 0, 1, &map_id_2, &item_size_2,
                   (void *)&data_ptr_2,
                   VX_READ_ONLY,
                   VX_MEMORY_TYPE_HOST, 0);

    returned_gpulut = (vx_uint16 *)data_ptr_2;
    if (NULL != ref_gpulut)
    {
        ASSERT_NO_FAILURE(gpulut_generation_lut_check (ref_gpulut, returned_gpulut,  SV_GPULUT_SIZE ));
    }

    vxUnmapArrayRange(out_gpulut_array, map_id_2);

    /* Deleting applib */
    srv_bowl_lut_gen_delete(srv_handle);

    VX_CALL(vxReleaseUserDataObject(&in_config));
    VX_CALL(vxReleaseUserDataObject(&in_calmat_object));
    VX_CALL(vxReleaseUserDataObject(&in_offset_object));
    VX_CALL(vxReleaseUserDataObject(&in_lens_param_object));
    VX_CALL(vxReleaseArray(&out_gpulut_array));

    VX_CALL(vxReleaseGraph(&graph));

    ct_free_mem(out_gpulut);
    ct_free_mem(ref_gpulut);
    ASSERT(in_config == 0);
    ASSERT(in_calmat_object == 0);
    ASSERT(in_offset_object == 0);
    ASSERT(in_lens_param_object == 0);
    ASSERT(out_gpulut_array == 0);

    tivxSrvUnLoadKernels(context);
}

TESTCASE_TESTS(tivxSrvBowlLutGenApplib,
               testApplib)

