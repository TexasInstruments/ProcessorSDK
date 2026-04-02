/*
 *
 * Copyright (c) 2020 Texas Instruments Incorporated
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
#include "app_srv_module.h"
#include "app_display_module.h"
#if defined(LINUX) || defined(QNX)
#include <sys/stat.h>
#endif

static void read_calmat_file( svCalmat_t *calmat, const char*fileName);
vx_status app_create_graph_srv_write_output(vx_graph graph, SRVObj *srvObj);
static void read_lut_file(ldc_lensParameters *ldcParams, const char*fileName);
static LDC_status LDC_Init(LensDistortionCorrection* ldc,
                        dtype distCenterX, dtype distCenterY, dtype distFocalLength,
                        dtype *lut_d2u, vx_int32 lut_d2u_length, dtype lut_d2u_step,
                        dtype *lut_u2d, vx_int32 lut_u2d_length, dtype lut_u2d_step);
static char *get_test_file_path();

vx_status app_init_srv(vx_context context, SRVObj *srvObj, SensorObj *sensorObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    vx_user_data_object srv_views;

    srv_coords_t local_srv_coords;
    tivx_srv_params_t params;

    /* Applib data */
    svGpuLutGen_t               in_params;
    svACCalmatStruct_t          in_calmat;
    svGeometric_t               in_offset;
    ldc_lensParameters          lens_params;
    svLdcLut_t                  in_lens_params;
    svCalmat_t                  calmat_file;

    vx_uint32 num_cameras;
    vx_uint32 i;

    num_cameras = sensorObj->num_cameras_enabled;

    read_calmat_file(&calmat_file, "psdkra/srv/srv_app/CALMAT.BIN");

    memset(&params, 0, sizeof(tivx_srv_params_t));
    params.cam_bpp = 24;

    memset(&params, 0, sizeof(tivx_srv_params_t));
    params.cam_bpp = 24;

    /* Populating data objects */
    memset(&in_params, 0, sizeof(svGpuLutGen_t));
    srvObj->in_config = vxCreateUserDataObject(context, "svGpuLutGen_t",
                                       sizeof(svGpuLutGen_t), NULL);

    memset(&in_calmat, 0, sizeof(svACCalmatStruct_t));
    srvObj->in_calmat_object = vxCreateUserDataObject(context, "svACCalmatStruct_t",
                                       sizeof(svACCalmatStruct_t), NULL);

    memset(&in_offset, 0, sizeof(svGeometric_t));
    srvObj->in_offset_object = vxCreateUserDataObject(context, "svGeometric_t",
                                       sizeof(svGeometric_t), NULL);

    memset(&in_lens_params, 0, sizeof(svLdcLut_t));
    srvObj->in_lens_param_object = vxCreateUserDataObject(context, "svLdcLut_t",
                                       sizeof(svLdcLut_t), NULL);

    srvObj->out_gpulut_array = vxCreateArray(context, VX_TYPE_UINT16, SV_GPULUT_SIZE);

    srvObj->param_obj = vxCreateUserDataObject(context, "tivx_srv_params_t", sizeof(tivx_srv_params_t), &params);

    srv_views = vxCreateUserDataObject(context, "srv_coords_t", sizeof(srv_coords_t), &local_srv_coords);
    srvObj->srv_views_array = vxCreateObjectArray(context, (vx_reference)srv_views, srvObj->params.num_views);
    vxReleaseUserDataObject(&srv_views);

    for (i = 0; i < srvObj->params.num_views; i++)
    {
        local_srv_coords.camx =     srvObj->params.camx[i];
        local_srv_coords.camy =     srvObj->params.camy[i];
        local_srv_coords.camz =     srvObj->params.camz[i];
        local_srv_coords.targetx =  srvObj->params.targetx[i];
        local_srv_coords.targety =  srvObj->params.targety[i];
        local_srv_coords.targetz =  srvObj->params.targetz[i];
        local_srv_coords.anglex =   srvObj->params.anglex[i];
        local_srv_coords.angley =   srvObj->params.angley[i];
        local_srv_coords.anglez =   srvObj->params.anglez[i];

        srv_views = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)srvObj->srv_views_array, i);
        vxCopyUserDataObject(srv_views, 0, sizeof(srv_coords_t), &local_srv_coords, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
        vxReleaseUserDataObject(&srv_views);
    }

    /* Populate input data */
    in_params.SVInCamFrmHeight   = DISPLAY_WIDTH; /*sensorObj->sensorParams.sensorInfo.raw_params.height*/
    in_params.SVInCamFrmWidth    = DISPLAY_HEIGHT; /*sensorObj->sensorParams.sensorInfo.raw_params.width;*/
    in_params.SVOutDisplayHeight = SV_OUT_DISPLAY_HEIGHT;
    in_params.SVOutDisplayWidth  = SV_OUT_DISPLAY_WIDTH;
    in_params.numCameras         = num_cameras;
    in_params.subsampleratio     = SV_SUBSAMPLE;
    in_params.useWideBowl        = 1;

    in_offset.offsetXleft     = srvObj->params.offsetXleft;
    in_offset.offsetXright    = srvObj->params.offsetXright;
    in_offset.offsetYfront    = srvObj->params.offsetYfront;
    in_offset.offsetYback     = srvObj->params.offsetYback;

    for (int idx=0;idx<48;idx++) {
        in_calmat.outcalmat[idx] = calmat_file.calMatBuf[idx];
    }

    /* Read Lens file */
    read_lut_file(&lens_params,"psdkra/srv/srv_app/LENS.BIN" );

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
    vxCopyUserDataObject(srvObj->in_config, 0, sizeof(svGpuLutGen_t), &in_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    vxCopyUserDataObject(srvObj->in_calmat_object, 0, sizeof(svACCalmatStruct_t), &in_calmat, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    vxCopyUserDataObject(srvObj->in_offset_object, 0, sizeof(svGeometric_t), &in_offset, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    vxCopyUserDataObject(srvObj->in_lens_param_object, 0, sizeof(svLdcLut_t), &in_lens_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);

    /* Creating applib */
    srvObj->graph_gpu_lut = vxCreateGraph(context);

    srvObj->create_params.vxContext = context;
    srvObj->create_params.vxGraph   = srvObj->graph_gpu_lut;

    /* Data object */
    srvObj->create_params.in_config    = srvObj->in_config;
    srvObj->create_params.in_calmat    = srvObj->in_calmat_object;
    srvObj->create_params.in_offset    = srvObj->in_offset_object;
    srvObj->create_params.in_ldclut    = srvObj->in_lens_param_object;
    srvObj->create_params.out_gpulut3d = srvObj->out_gpulut_array;

    srvObj->srv_handle = srv_bowl_lut_gen_create(&srvObj->create_params);

    status = vxVerifyGraph(srvObj->graph_gpu_lut);
    if(VX_SUCCESS == status)
    {
        APP_PRINTF("GPU LUT Graph Verify successful!");
    }

    /* Creating objects for OpenGL node */
    srvObj->output_img = vxCreateImage(context, DISPLAY_WIDTH, DISPLAY_HEIGHT, VX_DF_IMAGE_RGBX);

    vx_uint32 in_width = sensorObj->sensorParams.sensorInfo.raw_params.width;
    vx_uint32 in_height = sensorObj->sensorParams.sensorInfo.raw_params.height;
    vx_image input_img = vxCreateImage(context, in_width, in_height, VX_DF_IMAGE_NV12);
    srvObj->input_img_arr = vxCreateObjectArray(context, (vx_reference)input_img, sensorObj->num_cameras_enabled);
    vxReleaseImage(&input_img);

    status = vxGetStatus((vx_reference)srvObj->input_img_arr);

    if(srvObj->en_out_srv_write == 1)
    {
        char file_path[TIVX_FILEIO_FILE_PATH_LENGTH];
        char file_prefix[TIVX_FILEIO_FILE_PREFIX_LENGTH];

        strcpy(file_path, srvObj->output_file_path);
        srvObj->file_path   = vxCreateArray(context, VX_TYPE_UINT8, TIVX_FILEIO_FILE_PATH_LENGTH);
        vxAddArrayItems(srvObj->file_path, TIVX_FILEIO_FILE_PATH_LENGTH, &file_path[0], 1);

        strcpy(file_prefix, "srv_output");
        srvObj->file_prefix = vxCreateArray(context, VX_TYPE_UINT8, TIVX_FILEIO_FILE_PREFIX_LENGTH);
        vxAddArrayItems(srvObj->file_prefix, TIVX_FILEIO_FILE_PREFIX_LENGTH, &file_prefix[0], 1);

        srvObj->write_cmd = vxCreateUserDataObject(context, "tivxFileIOWriteCmd", sizeof(tivxFileIOWriteCmd), NULL);
    }
    else
    {
        srvObj->file_path   = NULL;
        srvObj->file_prefix = NULL;
        srvObj->write_node  = NULL;
        srvObj->write_cmd   = NULL;
    }

  return status;
}

void app_deinit_srv(SRVObj *srvObj)
{
    /* Deleting applib */
    srv_bowl_lut_gen_delete(srvObj->srv_handle);

    vxReleaseUserDataObject(&srvObj->param_obj);
    vxReleaseObjectArray(&srvObj->srv_views_array);
    vxReleaseUserDataObject(&srvObj->in_config);
    vxReleaseUserDataObject(&srvObj->in_calmat_object);
    vxReleaseUserDataObject(&srvObj->in_offset_object);
    vxReleaseUserDataObject(&srvObj->in_lens_param_object);
    vxReleaseArray(&srvObj->out_gpulut_array);

    vxReleaseGraph(&srvObj->graph_gpu_lut);

    vxReleaseImage(&srvObj->output_img);

    if(srvObj->en_out_srv_write == 1)
    {
        vxReleaseArray(&srvObj->file_path);
        vxReleaseArray(&srvObj->file_prefix);
        vxReleaseUserDataObject(&srvObj->write_cmd);
    }

    vxReleaseObjectArray(&srvObj->input_img_arr);
}

void app_delete_srv(SRVObj *srvObj)
{
    if(srvObj->node != NULL)
    {
        vxReleaseNode(&srvObj->node);
    }
    if(srvObj->write_node != NULL)
    {
        vxReleaseNode(&srvObj->write_node);
    }
}

vx_status app_create_graph_srv(vx_context context, vx_graph graph, SRVObj *srvObj, vx_object_array input_img_arr)
{
    vx_status status = VX_SUCCESS;

    vx_object_array input_arr;

    if(input_img_arr != NULL)
    {
        input_arr = input_img_arr;
    }
    else
    {
        input_arr = srvObj->input_img_arr;
    }

    srvObj->node = tivxGlSrvNode(graph,
                                srvObj->param_obj,
                                input_arr,
                                srvObj->srv_views_array,
                                srvObj->out_gpulut_array,
                                srvObj->output_img);

    status =  vxGetStatus((vx_reference)srvObj->node);
    if(status != VX_SUCCESS)
    {
        APP_PRINTF("srv_node create failed\n");
    }
    vxSetNodeTarget(srvObj->node, VX_TARGET_STRING, TIVX_TARGET_MPU_0);
    vxSetReferenceName((vx_reference)srvObj->node, "OpenGL_SRV_Node");

    if(status == VX_SUCCESS)
    {
        if(srvObj->en_out_srv_write == 1)
        {
            APP_PRINTF("Adding SRV write node on graph .. \n");
            status = app_create_graph_srv_write_output(graph, srvObj);
            APP_PRINTF("SRV write node added! \n");
        }
    }

    return status;
}

vx_status app_create_graph_srv_write_output(vx_graph graph, SRVObj *srvObj)
{
    vx_status status = VX_SUCCESS;

    srvObj->write_node = tivxWriteImageNode(graph, srvObj->output_img, srvObj->file_path, srvObj->file_prefix);
    status = vxGetStatus((vx_reference)srvObj->write_node);
    vxSetNodeTarget(srvObj->write_node, VX_TARGET_STRING, TIVX_TARGET_MPU_0);
    vxSetReferenceName((vx_reference)srvObj->write_node, "srv_write_node");

    return (status);
}

vx_status app_run_graph_gpu_lut(SRVObj *srvObj)
{
    vx_status status = VX_SUCCESS;

    status = vxProcessGraph(srvObj->graph_gpu_lut);
    if(status != VX_SUCCESS)
    {
        APP_PRINTF("graph_gpu_lut process failed\n");
    }
    return status;
}

static void read_calmat_file( svCalmat_t *calmat, const char*fileName)
{
    char file[APP_MAX_FILE_PATH];
    uint32_t cnt;
    FILE* f = 0;
    size_t sz;
    uint32_t  read_size;
    char failsafe_test_data_path[3] = "./";
    char * test_data_path = get_test_file_path();

    printf ("Reading calmat file \n");

    if (!fileName)
    {
        printf("calmat file name not specified\n");
        return;
    }

    if(NULL == test_data_path)
    {
        printf("Test data path is NULL. Defaulting to current folder \n");
        test_data_path = failsafe_test_data_path;
    }

#if defined(LINUX) || defined(QNX)
    {
        struct stat s;
        if (stat(test_data_path, &s))
        {
            printf("Test data path %s does not exist. Defaulting to current folder \n", test_data_path);
            test_data_path = failsafe_test_data_path;
        }
    }
#endif

    sz = snprintf(file, APP_MAX_FILE_PATH, "%s/%s", test_data_path, fileName);
    if (sz > APP_MAX_FILE_PATH)
    {
        return;
    }

    f = fopen(file, "rb");
    if (!f)
    {
        printf("Cant open calmat file: %s\n", fileName);
        return;
    }

    read_size = fread((int8_t *)&calmat->numCameras,sizeof(uint8_t),4,f);
    if (read_size != 4)
    {
        printf("Incorrect Bytes read from calmat file: %s\n", fileName);
        fclose(f);
        return;
    }

    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        read_size = fread((int8_t *)&calmat->calMatSize[cnt],sizeof(uint8_t),4,f);
        if (read_size != 4)
        {
            printf("Incorrect Bytes read from calmat file: %s\n", fileName);
            fclose(f);
            return;
        }
    }

    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        APP_PRINTF("Calmat size for cnt %d = %d \n",cnt,calmat->calMatSize[cnt]);
    }

    /* Set Pointer ahead by 128 bytes to skip over metadata */
    fseek(f,128,SEEK_SET);

    /* Read calmat per camera */
    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        read_size = fread((int8_t *)&calmat->calMatBuf + 48*cnt,sizeof(uint8_t),calmat->calMatSize[cnt],f);
        if (read_size != calmat->calMatSize[cnt])
        {
            printf("Incorrect Bytes read from calmat file: %s\n", fileName);
            fclose(f);
            return;
        }
    }

    for (cnt=0;cnt<calmat->numCameras;cnt++) {
        APP_PRINTF("For Camera = %d Ref calmat[0] = %d Ref Calmat[11] = %d \n",cnt,calmat->calMatBuf[12*cnt+0],calmat->calMatBuf[12*cnt +11]);
    }

    printf ("file read completed \n");
    fclose(f);

}

static void read_lut_file(ldc_lensParameters *ldcParams, const char*fileName)
{
    char file[APP_MAX_FILE_PATH];
    uint32_t  read_size;
    FILE* f = 0;
    size_t sz;
    char failsafe_test_data_path[3] = "./";
    char * test_data_path = get_test_file_path();

    if (!fileName)
    {
        printf("Image file name not specified\n");
        return;
    }

    if(NULL == test_data_path)
    {
        printf("Test data path is NULL. Defaulting to current folder \n");
        test_data_path = failsafe_test_data_path;
    }

#if defined(LINUX) || defined(QNX)
    {
        struct stat s;
        if (stat(test_data_path, &s))
        {
            printf("Test data path %s does not exist. Defaulting to current folder \n", test_data_path);
            test_data_path = failsafe_test_data_path;
        }
    }
#endif

    sz = snprintf(file, APP_MAX_FILE_PATH, "%s/%s", test_data_path, fileName);
    if (sz > APP_MAX_FILE_PATH)
    {
        return;
    }

    f = fopen(file, "rb");
    if (!f)
    {
        printf("Can't open LUT file: %s\n", fileName);
        return;
    }

    read_size = fread((uint8_t *)ldcParams,sizeof(uint8_t),sizeof(ldc_lensParameters),f);
    if (read_size != sizeof(ldc_lensParameters))
    {
        printf("Incorrect Bytes read from  LUT file: %s\n", fileName);
        fclose(f);
        return;
    }

    fclose(f);
}

static LDC_status LDC_Init(LensDistortionCorrection* ldc,
                        dtype distCenterX, dtype distCenterY, dtype distFocalLength,
                        dtype *lut_d2u, vx_int32 lut_d2u_length, dtype lut_d2u_step,
                        dtype *lut_u2d, vx_int32 lut_u2d_length, dtype lut_u2d_step)
{
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

static char *get_test_file_path()
{
    char *tivxPlatformGetEnv(char *env_var);

    #if defined(SYSBIOS)
    return tivxPlatformGetEnv("VX_TEST_DATA_PATH");
    #else
    return getenv("VX_TEST_DATA_PATH");
    #endif
}
