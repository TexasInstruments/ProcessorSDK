/*
 *  Copyright (c) Texas Instruments Incorporated 2021
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <getopt.h>

#include <VX/vx.h>
#include <TI/tivx.h>

#include "app_common.h"

vx_reference App_Common_MemAllocObject(vx_context context, vx_enum type, uint32_t  aux)
{
    vx_reference    ref = NULL;

    if (type == (vx_enum)VX_TYPE_IMAGE)
    {
        vx_image    image;

        image = vxCreateImage(context, 64, 48, aux);

        if (image == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreateImage() failed.\n");
        }

        ref = (vx_reference)image;
    }
    else if (type == (vx_enum)VX_TYPE_TENSOR)
    {
        vx_tensor   tensor;
        vx_size     dims[TIVX_CONTEXT_MAX_TENSOR_DIMS];
        uint32_t    i;

        for (i = 0; i < aux; i++)
        {
            dims[i] = 100;
        }

        tensor = vxCreateTensor(context, aux, dims, VX_TYPE_UINT8, 0);

        if (tensor == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreateTensor() failed.\n");
        }

        ref = (vx_reference)tensor;
    }
    else if (type == (vx_enum)VX_TYPE_USER_DATA_OBJECT)
    {
        vx_user_data_object obj;

        obj = vxCreateUserDataObject(context, NULL, aux, NULL);

        if (obj == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreateUserDataObject() failed.\n");
        }

        ref = (vx_reference)obj;
    }
    else if (type == (vx_enum)VX_TYPE_ARRAY)
    {
        vx_array    array;

        array = vxCreateArray(context, VX_TYPE_COORDINATES3D, aux);

        if (array == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreateArray() failed.\n");
        }

        ref = (vx_reference)array;
    }
    else if (type == (vx_enum)VX_TYPE_CONVOLUTION)
    {
        vx_convolution  conv;

        conv = vxCreateConvolution(context, aux, aux);

        if (conv == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreateConvolution() failed.\n");
        }

        ref = (vx_reference)conv;
    }
    else if (type == (vx_enum)VX_TYPE_MATRIX)
    {
        vx_matrix   matrix;

        matrix = vxCreateMatrix(context, VX_TYPE_UINT32, aux, aux);

        if (matrix == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreateMatrix() failed.\n");
        }

        ref = (vx_reference)matrix;
    }
    else if (type == (vx_enum)VX_TYPE_DISTRIBUTION)
    {
        vx_distribution  dist;

        dist = vxCreateDistribution(context, aux, 5, 200);

        if (dist == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreateDistribution() failed.\n");
        }

        ref = (vx_reference)dist;
    }
    else if (type == (vx_enum)TIVX_TYPE_RAW_IMAGE)
    {
        tivx_raw_image                  rawImage;
        tivx_raw_image_create_params_t  params;

        params.width                     = 128;
        params.height                    = 128;
        params.num_exposures             = 3;
        params.line_interleaved          = vx_false_e;
        params.format[0].pixel_container = TIVX_RAW_IMAGE_16_BIT;
        params.format[0].msb             = 12;
        params.format[1].pixel_container = TIVX_RAW_IMAGE_8_BIT;
        params.format[1].msb             = 7;
        params.format[2].pixel_container = TIVX_RAW_IMAGE_P12_BIT;
        params.format[2].msb             = 11;
        params.meta_height_before        = 5;
        params.meta_height_after         = 0;

        rawImage = tivxCreateRawImage(context, &params);

        ref = (vx_reference)rawImage;
    }
    else if (type == (vx_enum)VX_TYPE_PYRAMID)
    {
        vx_pyramid  pyramid;
        vx_size     levels = 4;
        vx_uint32   width = 640;
        vx_uint32   height = 480;
        vx_float32  scale = VX_SCALE_PYRAMID_HALF;

        pyramid = vxCreatePyramid(context, levels, scale, width, height, aux);

        if (pyramid == NULL)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxCreatePyramid() failed.\n");
        }

        ref = (vx_reference)pyramid;
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Unsupported type [%d].\n", type);
    }

    return ref;
}

int32_t App_Common_DeAllocImageObjects(vx_reference  ref[], uint32_t numRefs)
{
    uint32_t    i;
    int32_t     status = 0;
    vx_status   vxStatus = VX_SUCCESS;

    for (i = 0; i < numRefs; i++)
    {
        vx_enum     refType;

        vxStatus = vxQueryReference(ref[i],
                                    VX_REFERENCE_TYPE,
                                    &refType,
                                    sizeof(vx_enum));

        if (vxStatus != (vx_status)VX_SUCCESS)
        {
            VX_PRINT(VX_ZONE_ERROR, "vxQueryReference() failed.\n");
            break;
        }

        if (refType == (vx_enum)VX_TYPE_IMAGE)
        {
            vx_image    image = (vx_image)ref[i];
            vxStatus = vxReleaseImage(&image);
        }
        else if (refType == (vx_enum)VX_TYPE_TENSOR)
        {
            vx_tensor   tensor = (vx_tensor)ref[i];
            vxStatus = vxReleaseTensor(&tensor);
        }
        else if (refType == (vx_enum)VX_TYPE_USER_DATA_OBJECT)
        {
            vx_user_data_object obj = (vx_user_data_object)ref[i];
            vxStatus = vxReleaseUserDataObject(&obj);
        }
        else if (refType == (vx_enum)VX_TYPE_ARRAY)
        {
            vx_array    array = (vx_array)ref[i];
            vxStatus = vxReleaseArray(&array);
        }
        else if (refType == (vx_enum)VX_TYPE_CONVOLUTION)
        {
            vx_convolution  conv = (vx_convolution)ref[i];
            vxStatus = vxReleaseConvolution(&conv);
        }
        else if (refType == (vx_enum)VX_TYPE_MATRIX)
        {
            vx_matrix   matrix = (vx_matrix)ref[i];
            vxStatus = vxReleaseMatrix(&matrix);
        }
        else if (refType == (vx_enum)VX_TYPE_DISTRIBUTION)
        {
            vx_distribution dist = (vx_distribution)ref[i];
            vxStatus = vxReleaseDistribution(&dist);
        }
        else if (refType == (vx_enum)TIVX_TYPE_RAW_IMAGE)
        {
            tivx_raw_image  rawImage = (tivx_raw_image)ref[i];
            vxStatus = tivxReleaseRawImage(&rawImage);
        }
        else if (refType == (vx_enum)VX_TYPE_PYRAMID)
        {
            vx_pyramid  pyramid = (vx_pyramid)ref[i];
            vxStatus = vxReleasePyramid(&pyramid);
        }
        else
        {
            VX_PRINT(VX_ZONE_ERROR, "Unsupported refType [%d].\n", refType);
            vxStatus = (vx_status)VX_FAILURE;
        }
    }

    if (vxStatus != (vx_status)VX_SUCCESS)
    {
        status = -1;
    }

    return status;
}

