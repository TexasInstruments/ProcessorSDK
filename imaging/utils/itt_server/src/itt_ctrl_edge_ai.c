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

#include "itt_priv.h"

#include <itt_server.h>
#include <itt_srvr_remote.h>
#include <sys/stat.h>
#include <app_iss.h>
#include <fcntl.h>
#include <unistd.h>

#define TIOVX_MODULES_MAX_FNAME (256u)

static ITTServerEdgeAIObj g_ITTobj;
static pthread_mutex_t lock;
uint8_t gEdgeAI = 0;

static vx_status writeRawImageEdgeAI(const char* file_name, tivx_raw_image image)
{
    vx_status status;

    status = vxGetStatus((vx_reference)image);

    if((vx_status)VX_SUCCESS == status)
    {
        int32_t fd = open(file_name, (uint32_t)O_WRONLY | (uint32_t)O_CREAT | (uint32_t)O_TRUNC, 
                        (uint32_t)S_IRUSR | (uint32_t)S_IWUSR | (uint32_t)S_IRGRP | (uint32_t)S_IROTH);
        vx_int32  j;

        if(fd < 0)
        {
            appLogPrintf("Unable to open file %s \n", file_name);
            status = (vx_status)VX_FAILURE;
        }
        else
        {
            vx_uint32 width, height;
            vx_imagepatch_addressing_t image_addr;
            vx_rectangle_t rect;
            vx_map_id map_id;
            void *data_ptr;
            vx_uint32 bpp = 1;
            vx_uint32 num_bytes;
            tivx_raw_image_format_t format[3];
            vx_int32 plane, num_planes, plane_size;
            vx_uint32 num_exposures;
            vx_bool line_interleaved = (vx_bool)vx_false_e;

            (void)tivxQueryRawImage(image, (vx_int32)TIVX_RAW_IMAGE_WIDTH, &width, (vx_int32)sizeof(vx_uint32));
            (void)tivxQueryRawImage(image, (vx_int32)TIVX_RAW_IMAGE_HEIGHT, &height, (vx_int32)sizeof(vx_uint32));
            (void)tivxQueryRawImage(image, (vx_int32)TIVX_RAW_IMAGE_FORMAT, &format, (vx_int32)sizeof(format));
            (void)tivxQueryRawImage(image, (vx_int32)TIVX_RAW_IMAGE_NUM_EXPOSURES, &num_exposures, (vx_int32)sizeof(num_exposures));
            (void)tivxQueryRawImage(image, (vx_int32)TIVX_RAW_IMAGE_LINE_INTERLEAVED, &line_interleaved, (vx_int32)sizeof(line_interleaved));

            if(line_interleaved == (vx_bool)vx_true_e)
            {
                num_planes = 1;
            }
            else
            {
                num_planes = (vx_int32)num_exposures;
            }

            if( format[0].pixel_container == (uint32_t)TIVX_RAW_IMAGE_16_BIT )
            {
                bpp = 2U;
            }
            else if( format[0].pixel_container == (uint32_t)TIVX_RAW_IMAGE_8_BIT )
            {
                bpp = 1U;
            }
            else if( format[0].pixel_container == (uint32_t)TIVX_RAW_IMAGE_P12_BIT )
            {
                bpp = 0U;
            }
            else
            {
                /* Default case */
            }

            rect.start_x = 0;
            rect.start_y = 0;
            rect.end_x = width;
            rect.end_y = height;

            for (plane = 0; plane < num_planes; plane++)
            {
                (void)tivxMapRawImagePatch(image,
                    &rect,
                    (uint32_t)plane,
                    &map_id,
                    &image_addr,
                    &data_ptr,
                    (vx_enum)VX_READ_ONLY,
                    (vx_enum)VX_MEMORY_TYPE_HOST,
                    (vx_enum)TIVX_RAW_IMAGE_PIXEL_BUFFER
                    );

                uint8_t *pIn = (uint8_t *)data_ptr;
                num_bytes = 0U;
                if(line_interleaved == (vx_bool)vx_true_e)
                {
                    uint32_t total_iterations = (vx_uint32)image_addr.dim_y * (vx_uint32)num_exposures;
                    vx_int32 limit = (vx_int32)total_iterations;
                    for (j = 0; j < limit; j++) 
                    {
                        ssize_t bytes_written = write(fd, pIn, (size_t)image_addr.dim_x * (size_t)bpp);
                        if(bytes_written > 0)
                        {
                            num_bytes += (vx_uint32)bytes_written;
                        }
                        pIn = &pIn[image_addr.stride_y];
                    }
                }
                else
                {
                    for (j = 0; j < (vx_int32)image_addr.dim_y; j++)
                    {
                        ssize_t bytes_written = write(fd, pIn, (size_t)image_addr.dim_x * (size_t)bpp);
                        if(bytes_written > 0)
                        {
                            num_bytes += (vx_uint32)bytes_written;
                        }
                        pIn = &pIn[image_addr.stride_y];
                    }
                }

                uint32_t total_size = (vx_uint32)image_addr.dim_y * 
                                      (vx_uint32)image_addr.dim_x * 
                                      (vx_uint32)bpp;
                plane_size = (vx_int32)total_size;

                if(num_bytes != (vx_uint32)plane_size)
                {
                    appLogPrintf("Plane [%d] bytes written = %d, expected = %d\n", (uint32_t)plane, num_bytes, plane_size);
                }

                (void)tivxUnmapRawImagePatch(image, map_id);
            }

            (void)close(fd);
        }
    }

    return(status);
}

static vx_status writeImageEdgeAI(const char* file_name, vx_image img)
{
    vx_status status;

    status = vxGetStatus((vx_reference)img);

    if((vx_status)VX_SUCCESS == status)
    {
        int32_t fd = open(file_name, (uint32_t)O_WRONLY | (uint32_t)O_CREAT | (uint32_t)O_TRUNC,
                        (uint32_t)S_IRUSR | (uint32_t)S_IWUSR | (uint32_t)S_IRGRP | (uint32_t)S_IROTH);
        vx_int32  j;

        if(fd < 0)
        {
            appLogPrintf("Unable to open file %s \n", file_name);
            status = (vx_status)VX_FAILURE;
        }
        else
        {
            vx_rectangle_t rect;
            vx_imagepatch_addressing_t image_addr;
            vx_map_id map_id;
            void * data_ptr;
            vx_uint32  img_width;
            vx_uint32  img_height;
            vx_uint32  num_bytes = 0;
            vx_size    num_planes;
            vx_uint32  plane;
            vx_uint32  plane_size;
            vx_df_image img_format;

            (void)vxQueryImage(img, (vx_int32)VX_IMAGE_WIDTH, &img_width, (vx_int32)sizeof(vx_uint32));
            (void)vxQueryImage(img, (vx_int32)VX_IMAGE_HEIGHT, &img_height, (vx_int32)sizeof(vx_uint32));
            (void)vxQueryImage(img, (vx_int32)VX_IMAGE_PLANES, &num_planes, (vx_int32)sizeof(vx_size));
            (void)vxQueryImage(img, (vx_int32)VX_IMAGE_FORMAT, &img_format, (vx_int32)sizeof(vx_df_image));

            for (plane = 0U; plane < (vx_uint32)num_planes; plane++)
            {
                rect.start_x = 0U;
                rect.start_y = 0U;
                rect.end_x = img_width;
                rect.end_y = img_height;
                status = vxMapImagePatch(img,
                                        &rect,
                                        plane,
                                        &map_id,
                                        &image_addr,
                                        &data_ptr,
                                        (vx_enum)VX_READ_ONLY,
                                        (vx_enum)VX_MEMORY_TYPE_HOST,
                                        (vx_enum)VX_NOGAP_X);

                #ifdef ENABLE_DEBUG_ITTSERVER
                appLogPrintf("image_addr.dim_x = %d\n ", image_addr.dim_x);
                appLogPrintf("image_addr.dim_y = %d\n ", image_addr.dim_y);
                appLogPrintf("image_addr.step_x = %d\n ", image_addr.step_x);
                appLogPrintf("image_addr.step_y = %d\n ", image_addr.step_y);
                appLogPrintf("image_addr.stride_y = %d\n ", image_addr.stride_y);
                appLogPrintf("image_addr.stride_x = %d\n ", image_addr.stride_x);
                appLogPrintf("\n");
                #endif

                num_bytes = 0U;

                uint32_t num_steps = (vx_uint32)image_addr.dim_y / (vx_uint32)image_addr.step_y;
                vx_int32 limit = (vx_int32)num_steps;
                for (j = 0; j < limit; j++)
                {
                    size_t bytes_to_write = (size_t)image_addr.stride_x * 
                                            ((size_t)image_addr.dim_x / (size_t)image_addr.step_x);
                    ssize_t bytes_written = write(fd, data_ptr, bytes_to_write);
                    if(bytes_written > 0)
                    {
                        num_bytes += (vx_uint32)bytes_written;
                    }
                    data_ptr = (void*)(&((uint8_t*)data_ptr)[image_addr.stride_y]);
                }

                plane_size = (vx_uint32)(((vx_uint32)image_addr.dim_y/(vx_uint32)image_addr.step_y) * (((vx_uint32)image_addr.dim_x * (vx_uint32)image_addr.stride_x)/(vx_uint32)image_addr.step_x));

                if(num_bytes != plane_size)
                {
                    appLogPrintf("Plane [%d] bytes written = %d, expected = %d\n", (uint32_t)plane, num_bytes, plane_size);
                }

                (void)vxUnmapImagePatch(img, map_id);
            }

            (void)close(fd);
        }
    }

    return(status);
}

static char *app_get_test_file_path(void)
{
    char *tivxPlatformGetEnv(char *env_var);

    #if defined(SYSBIOS)
    return tivxPlatformGetEnv("VX_TEST_DATA_PATH");
    #else
    return getenv("VX_TEST_DATA_PATH");
    #endif
}

static int32_t save_debug_images_edge_ai(const ITTServerEdgeAIObj *obj);

static int32_t save_debug_images_edge_ai(const ITTServerEdgeAIObj *obj)
{
    int32_t num_bytes_io = 0;
    static int32_t file_index = 0;
    char raw_image_fname[TIOVX_MODULES_MAX_FNAME];
    char yuv_image_fname[TIOVX_MODULES_MAX_FNAME];
    char failsafe_test_data_path[3] = "./";
    char * test_data_path = app_get_test_file_path();
    struct stat s;

    int32_t result;

    if(NULL == test_data_path)
    {
        appLogPrintf("Test data path is NULL. Defaulting to current folder \n");
        test_data_path = failsafe_test_data_path;
    }

    if (stat(test_data_path, &s) != 0)
    {
        appLogPrintf("Test data path %s does not exist. Defaulting to current folder \n", test_data_path);
        test_data_path = failsafe_test_data_path;
    }

    result = file_index;

    (void)snprintf(raw_image_fname, TIOVX_MODULES_MAX_FNAME, "%s/%s_%04d.raw", test_data_path, "img", file_index);
    appLogPrintf("RAW file name %s \n", raw_image_fname);
    if (*(obj->obj_viss.raw_image_handle) != NULL)
    {
        num_bytes_io = writeRawImageEdgeAI(raw_image_fname, *(obj->obj_viss.raw_image_handle));
        if(num_bytes_io < 0)
        {
            appLogPrintf("Error writing to RAW file \n");
            result = (vx_status)VX_FAILURE;
        }
    }
    else
    {
        int32_t fd = open(raw_image_fname, (int32_t)O_WRONLY | (int32_t)O_CREAT | (int32_t)O_TRUNC, 
                        (uint32_t)S_IRUSR | (uint32_t)S_IWUSR | (uint32_t)S_IRGRP | (uint32_t)S_IROTH);
        if(fd < 0)
        {
            appLogPrintf("Unable to open file %s \n", raw_image_fname);
            result = (vx_status)VX_FAILURE;
        }
        else
        {
            (void)close(fd);
        }
    }

    if(result >= 0)
    {
        (void)snprintf(yuv_image_fname, TIOVX_MODULES_MAX_FNAME, "%s/%s_%04d.yuv", test_data_path, "img_viss", file_index);
        appLogPrintf("YUV file name %s \n", yuv_image_fname);
        if (*(obj->obj_viss.yuv_image_handle) != NULL)
        {
            num_bytes_io = writeImageEdgeAI(yuv_image_fname, *(obj->obj_viss.yuv_image_handle));
            if(num_bytes_io < 0)
            {
                appLogPrintf("Error writing to VISS NV12 file \n");
                result = (vx_status)VX_FAILURE;
            }
        }
        else
        {
            int32_t fd = open(yuv_image_fname, (int32_t)O_WRONLY | (int32_t)O_CREAT | (int32_t)O_TRUNC, 
                        (uint32_t)S_IRUSR | (uint32_t)S_IWUSR | (uint32_t)S_IRGRP | (uint32_t)S_IROTH);
            if(fd < 0)
            {
                appLogPrintf("Unable to open file %s \n", yuv_image_fname);
                result = (vx_status)VX_FAILURE;
            }
            else
            {
                (void)close(fd);
            }
        }
    }

    if(result >= 0)
    {
        file_index++;
        result = file_index - 1;
    }

    return result;
}

int32_t itt_register_object(vx_context context,
                              vx_node *node,
                              tivx_raw_image *raw_image_handle,
                              vx_image *yuv_image_handle, 
                              uint8_t object_name)
{
    int32_t status = 0;

    switch(object_name)
    {
        case (uint8_t)VISS:
            g_ITTobj.obj_viss.context = context;
            g_ITTobj.obj_viss.node = node;
            g_ITTobj.obj_viss.raw_image_handle = raw_image_handle;
            g_ITTobj.obj_viss.yuv_image_handle = yuv_image_handle;
            status = (vx_status)VX_SUCCESS;
            break;
        case (uint8_t)LDC:
            g_ITTobj.obj_ldc.context = context;
            g_ITTobj.obj_ldc.node = node;
            g_ITTobj.obj_ldc.raw_image_handle = raw_image_handle;
            g_ITTobj.obj_ldc.yuv_image_handle = yuv_image_handle;
            status = (vx_status)VX_SUCCESS;
            break;
        default:
            status = (vx_status)VX_FAILURE;
            break;
    }

    return status;
}

static int32_t itt_handle_dcc(const ITTServerEdgeAIObj *obj, uint8_t* dcc_buf, uint32_t dcc_buf_size);

static int32_t itt_handle_dcc(const ITTServerEdgeAIObj *obj, uint8_t* dcc_buf, uint32_t dcc_buf_size)
{
    int32_t status = 0;

    (void)obj;

    dcc_component_header_type header_data;
    (void)memcpy((void *)&header_data, (const void *)dcc_buf, sizeof(dcc_component_header_type));

    if(1U == is_viss_plugin((uint32_t)header_data.dcc_descriptor_id))
    {

        if((g_ITTobj.obj_viss.context == NULL) && (g_ITTobj.obj_viss.node == NULL))
        {
            appLogPrintf("VISS context or node NULL!!\n");
        }
        else
        {
            /* Trigger VISS udpate dcc callback */
            status = appUpdateVpacDcc(dcc_buf, dcc_buf_size, g_ITTobj.obj_viss.context,
                *(g_ITTobj.obj_viss.node) /* node_viss */, 0,
                NULL /* node_aewb*/, 0,
                NULL /* node_ldc */, 0
             );
        }
    }
    else if(1U == is_ldc_plugin((uint32_t)header_data.dcc_descriptor_id))
    {
        if((g_ITTobj.obj_ldc.context == NULL) && (g_ITTobj.obj_ldc.node == NULL))
        {
            appLogPrintf("LDC context or node NULL!!\n");
        }
        else
        {
            /* Trigger LDC update dcc callback */
            status = appUpdateVpacDcc(dcc_buf, dcc_buf_size, g_ITTobj.obj_ldc.context,
                NULL /* node_viss */, 0,
                NULL /* node_aewb*/, 0,
                *(g_ITTobj.obj_ldc.node) /* node_ldc */, 0
             );
        }
    }
    else
    {
        appLogPrintf("Plugin not supported!");
        status = -1;
    }

    return status;
}

int32_t itt_server_edge_ai_init(void)
{
    (void)pthread_mutex_lock(&lock);

    int32_t status = 0;
    static uint8_t ITTinit = 0U;
    gEdgeAI = 1U;

    if(0U == ITTinit)
    {
        /* Initializes ITT server thread on A72/Linux */
        status = itt_server_init((void*)&g_ITTobj, save_debug_images_edge_ai, itt_handle_dcc);
        if(status != 0)
        {
            appLogPrintf("Warning : Failed to initialize ITT server. Live tuning will not work \n");
        }
        else
        {
            /* Initializes I2C bus for reading and writting of camera registers */
            status = i2cInit();
            if(status!=0)
            {
                appLogPrintf("Warning: Failed to initialize i2c bus. Register read/write will not work !!!\n");
            }

            /* Initializes remote server for 2A control */
            status = IttRemoteServer_Init();
            if(status!=0)
            {
                appLogPrintf("Warning: Failed to create remote ITT server failed. Live tuning will not work !!!\n");
            }
        }

        ITTinit = 1U;
    }

    (void)pthread_mutex_unlock(&lock);

    return (status);
}
