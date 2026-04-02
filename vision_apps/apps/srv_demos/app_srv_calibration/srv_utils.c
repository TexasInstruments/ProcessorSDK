
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

#include "srv_utils.h"

/*
 * Utility API used to add a graph parameter from a node, node parameter index
 */
void add_graph_parameter_by_node_index(vx_graph graph, vx_node node,
    vx_uint32 node_parameter_index)
{
    vx_parameter parameter = vxGetParameterByIndex(node, node_parameter_index);

    vxAddParameterToGraph(graph, parameter);
    vxReleaseParameter(&parameter);
}

/* Provides file path for test files */
char *get_test_file_path()
{
    char *tivxPlatformGetEnv(char *env_var);

    #if defined(SYSBIOS)
    return tivxPlatformGetEnv("VX_TEST_DATA_PATH");
    #else
    return getenv("VX_TEST_DATA_PATH");
    #endif
}

vx_status app_load_vximage_from_bin_file(char *filename, vx_image image)
{
    vx_uint32 width, height, num_bytes;
    vx_imagepatch_addressing_t image_addr1, image_addr2;
    vx_rectangle_t rect;
    vx_map_id map_id1, map_id2;
    vx_df_image df;
    void *data_ptr1, *data_ptr2;
    vx_status status;

    status = vxGetStatus((vx_reference)image);
    if(status==VX_SUCCESS)
    {
        vxQueryImage(image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
        vxQueryImage(image, VX_IMAGE_FORMAT, &df, sizeof(vx_df_image));

        rect.start_x = 0;
        rect.start_y = 0;
        rect.end_x = width;
        rect.end_y = height;

        status = vxMapImagePatch(image,
            &rect,
            0,
            &map_id1,
            &image_addr1,
            &data_ptr1,
            VX_READ_ONLY,
            VX_MEMORY_TYPE_HOST,
            VX_NOGAP_X
            );
        status = vxMapImagePatch(image,
            &rect,
            1,
            &map_id2,
            &image_addr2,
            &data_ptr2,
            VX_READ_ONLY,
            VX_MEMORY_TYPE_HOST,
            VX_NOGAP_X
            );

        if(status==VX_SUCCESS)
        {
            FILE *fp = fopen(filename,"rb+a");

            if(fp!=NULL)
            {
                size_t ret;

                num_bytes = (width*height);
                ret = fread(data_ptr1, sizeof(uint8_t), num_bytes, fp);
                if(ret!=num_bytes)
                {
                    printf("# ERROR: Unable to read data from file [%s]\n", filename);
                }

                num_bytes = (width*height)/2;
                ret = fread(data_ptr2, sizeof(uint8_t), num_bytes, fp);
                if(ret!=num_bytes)
                {
                    printf("# ERROR: Unable to read data from file [%s]\n", filename);
                }
                fclose(fp);
            }
            else
            {
                 printf("# ERROR: Unable to open file for reading [%s]\n", filename);
            }
            vxUnmapImagePatch(image, map_id1);
            vxUnmapImagePatch(image, map_id2);
        }
    }
    else
    {
        printf("# ERROR: Invalid image specified for reading\n");
    }
    return status;
}

vx_int32 write_output_image_fp(FILE * fp, vx_image out_image)
{
    vx_uint32 width, height, i;
    vx_df_image df;
    vx_imagepatch_addressing_t image_addr;
    vx_rectangle_t rect;
    vx_map_id map_id1, map_id2;
    void *data_ptr1, *data_ptr2;
    vx_uint32 num_bytes_per_pixel = 1, index;
    vx_uint32 num_luma_bytes_written_to_file = 0, num_chroma_bytes_written_to_file = 0, num_bytes_written_to_file;

    vxQueryImage(out_image, VX_IMAGE_WIDTH, &width, sizeof(vx_uint32));
    vxQueryImage(out_image, VX_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
    vxQueryImage(out_image, VX_IMAGE_FORMAT, &df, sizeof(vx_df_image));

    APP_PRINTF("out width =  %d\n", width);
    APP_PRINTF("out height =  %d\n", height);
    APP_PRINTF("out format =  %d\n", df);

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = width;
    rect.end_y = height;

    vxMapImagePatch(out_image,
        &rect,
        0,
        &map_id1,
        &image_addr,
        &data_ptr1,
        VX_WRITE_ONLY,
        VX_MEMORY_TYPE_HOST,
        VX_NOGAP_X
        );

    if(!data_ptr1)
    {
        printf("data_ptr1 is NULL \n");
        fclose(fp);
        return -1;
    }

    for (i = 0; i < height; i++)
    {
        index = image_addr.stride_y * i;
        num_luma_bytes_written_to_file += fwrite((data_ptr1+index), 1, width*num_bytes_per_pixel, fp);
    }

    fflush(fp);
    vxMapImagePatch(out_image,
        &rect,
        1,
        &map_id2,
        &image_addr,
        &data_ptr2,
        VX_WRITE_ONLY,
        VX_MEMORY_TYPE_HOST,
        VX_NOGAP_X
        );

    if(!data_ptr2)
    {
        printf("data_ptr2 is NULL \n");
        fclose(fp);
        return -1;
    }

    for (i = 0; i < (height/2); i++)
    {
        index = image_addr.stride_y * i;
        num_chroma_bytes_written_to_file += fwrite((data_ptr2+index), 1, width*num_bytes_per_pixel, fp);
    }

    fflush(fp);

    num_bytes_written_to_file = num_luma_bytes_written_to_file + num_chroma_bytes_written_to_file;

    vxUnmapImagePatch(out_image, map_id1);
    vxUnmapImagePatch(out_image, map_id2);

    return num_bytes_written_to_file;

}

vx_int32 write_output_image_nv12_8bit(char * file_name, vx_image out_nv12)
{
    FILE * fp = fopen(file_name, "wb");
    if(!fp)
    {
        APP_PRINTF("Unable to open file %s\n", file_name);
        return -1;
    }
    vx_uint32 len1 = write_output_image_fp(fp, out_nv12);
    fclose(fp);
    printf("%d bytes written to %s\n", len1, file_name);
    return len1;
}

/* Reads LUT file using get_test_file_path */
void read_lut_file(ldc_lensParameters *ldcParams, const char*fileName)
{
    char file[APP_MAX_FILE_PATH];
    uint32_t  read_size;
    FILE* f = 0;
    size_t sz;
    char failsafe_test_data_path[3] = "./";
    char * test_data_path = get_test_file_path();

    if (!fileName)
    {
        printf("LUT file name not specified\n");
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

/* Initializes LDC parameters */
LDC_status LDC_Init(LensDistortionCorrection* ldc,
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

/* Reads CHARTPOS file */
void read_chartpos_file(vx_int8 *inChartPos, const char*fileName)
{
    char file[APP_MAX_FILE_PATH];
    FILE* f = 0;
    size_t sz;
    uint32_t  read_size;
    char failsafe_test_data_path[3] = "./";
    char * test_data_path = get_test_file_path();

    if (!fileName)
    {
        printf("chartpos file name not specified\n");
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
        return;
    }

    read_size = fread((int8_t *)inChartPos,sizeof(uint8_t),164,f);

    if (read_size != 164)
    {
        fclose(f);
        return;
    }

    fclose(f);

}

void read_calmat_file( svCalmat_t *calmat, const char*fileName)
{
    char file[APP_MAX_FILE_PATH];
    uint32_t cnt;
    FILE* f = 0;
    size_t sz;
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

void write_calmat_file( svCalmat_t *calmat, const char*fileName)
{
    char file[APP_MAX_FILE_PATH];
    uint32_t cnt;
    FILE* f = 0;
    size_t sz;
    char failsafe_test_data_path[3] = "./";
    char * test_data_path = get_test_file_path();

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

    f = fopen(file, "wb");
    if (!f)
    {
        printf("Error: could not open file path\n");
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

vx_int32 write_output_image_raw(char * file_name, tivx_raw_image raw_image)
{
    FILE * fp = fopen(file_name, "wb");
    vx_uint32 width, height, i, index;
    vx_imagepatch_addressing_t image_addr;
    vx_rectangle_t rect;
    vx_map_id map_id;
    void *data_ptr;
    vx_uint32 num_bytes_per_pixel = 2; /*Mayank : Hardcoded to 12b Unpacked format*/
    vx_uint32 num_bytes_written_to_file = 0;
    tivx_raw_image_format_t format;

    if(!fp)
    {
        APP_PRINTF("Unable to open file %s\n", file_name);
        return -1;
    }

    tivxQueryRawImage(raw_image, TIVX_RAW_IMAGE_WIDTH, &width, sizeof(vx_uint32));
    tivxQueryRawImage(raw_image, TIVX_RAW_IMAGE_HEIGHT, &height, sizeof(vx_uint32));
    tivxQueryRawImage(raw_image, TIVX_RAW_IMAGE_FORMAT, &format, sizeof(format));

    APP_PRINTF("in width =  %d\n", width);
    APP_PRINTF("in height =  %d\n", height);
    APP_PRINTF("in format =  %d\n", format.pixel_container);

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x = width;
    rect.end_y = height;

    tivxMapRawImagePatch(raw_image,
        &rect,
        0,
        &map_id,
        &image_addr,
        &data_ptr,
        VX_READ_ONLY,
        VX_MEMORY_TYPE_HOST,
        TIVX_RAW_IMAGE_PIXEL_BUFFER
        );

    if(!data_ptr)
    {
        APP_PRINTF("data_ptr is NULL \n");
        fclose(fp);
        return -1;
    }

    for (i = 0; i < height; i++)
    {
        index = image_addr.stride_y * i;
        num_bytes_written_to_file += fwrite((data_ptr+index), 1, width*num_bytes_per_pixel, fp);
    }

    fflush(fp);
    fclose(fp);
    printf("%d bytes written to %s\n", num_bytes_written_to_file, file_name);
    return num_bytes_written_to_file;
}


