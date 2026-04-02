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
#include "app_dof_pyramid_module.h"
#include <TI/hwa_vpac_msc.h>


vx_status app_init_pyramid(vx_context context, PyramidObj *pyramidObj, char *objName, vx_uint32 num_ch)
{
    vx_status status = VX_SUCCESS;

    vx_pyramid pyramid_exemplar;
    vx_object_array pyramid_array;

    printf("Creating pyramid of level = %d, width =%d, height = %d, num_ch = %d\n", pyramidObj->dof_levels, pyramidObj->width, pyramidObj->height, num_ch);

    pyramid_exemplar = vxCreatePyramid(context,
                        pyramidObj->dof_levels, VX_SCALE_PYRAMID_HALF,
                        pyramidObj->width, pyramidObj->height,
                        pyramidObj->vx_df_pyramid);

    status  = vxGetStatus((vx_reference)pyramid_exemplar);

    if(status == VX_SUCCESS)
    {
        pyramid_array = vxCreateObjectArray(context, (vx_reference)pyramid_exemplar, num_ch);
        status  = vxGetStatus((vx_reference)pyramid_array);
    }
    else
    {
        printf("Unable to create pyramid examplar!\n");
    }

    if(status == VX_SUCCESS)
    {
        pyramidObj->pyramid_delay = vxCreateDelay(context, (vx_reference)pyramid_array, 2);
        status  = vxGetStatus((vx_reference)pyramidObj->pyramid_delay);
    }
    else
    {
        printf("Unable to create pyramid object array!\n");
    }

    if(status == VX_FAILURE)
    {
        printf("Unable to create pyramid delay!\n");
    }

    vxReleasePyramid(&pyramid_exemplar);
    vxReleaseObjectArray(&pyramid_array);

    /* Creating default input */
    vx_image input = vxCreateImage(context, pyramidObj->width, pyramidObj->height, VX_DF_IMAGE_NV12);
    pyramidObj->input_arr = vxCreateObjectArray(context, (vx_reference)input, num_ch);
    vxReleaseImage(&input);

    return status;
}

void app_deinit_pyramid(PyramidObj *pyramidObj)
{
    vxReleaseDelay(&pyramidObj->pyramid_delay);

    vxReleaseObjectArray(&pyramidObj->input_arr);
}

void app_delete_pyramid(PyramidObj *pyramidObj)
{
    if(pyramidObj->node != NULL)
    {
        vxReleaseNode(&pyramidObj->node);
    }
}

vx_status app_create_graph_pyramid(vx_graph graph, PyramidObj *pyramidObj, vx_object_array input_arr, vx_uint8 output_select)
{
    vx_status status = VX_SUCCESS;

    vx_pyramid output = NULL;
    vx_image input = NULL;

    if(input_arr == NULL)
    {
        input  = (vx_image)vxGetObjectArrayItem(pyramidObj->input_arr, 0);
    }
    else
    {
        input  = (vx_image)vxGetObjectArrayItem(input_arr, 0);
    }

    status = vxGetStatus((vx_reference)input);
   
    if(status == VX_SUCCESS)
    {
        //note: here the external_count is not incremented so no need to release this reference
        vx_object_array pyramid_array = NULL;

        if(output_select == DOF_PYRAMID_START_FROM_PREVIOUS)
            pyramid_array  = (vx_object_array)vxGetReferenceFromDelay(pyramidObj->pyramid_delay, -1);
        else if (output_select == DOF_PYRAMID_START_FROM_CURRENT)
            pyramid_array  = (vx_object_array)vxGetReferenceFromDelay(pyramidObj->pyramid_delay, 0);

        if (NULL != pyramid_array)
        {
            output = (vx_pyramid)vxGetObjectArrayItem((vx_object_array)pyramid_array, 0);
        }
        status = vxGetStatus((vx_reference)output);
    }
    else
    {
        printf("Unable to get input!\n");
    }

    if(status == VX_SUCCESS)
    {
        pyramidObj->node  = tivxVpacMscPyramidNode(graph, input, output);
        status = vxGetStatus((vx_reference)pyramidObj->node);
    }
    else
    {
        printf("Unable to get output!\n");
    }

    if(status == VX_SUCCESS)
    {
        vxSetReferenceName((vx_reference)pyramidObj->node, "GaussianPyramid");
        status = vxSetNodeTarget(pyramidObj->node, VX_TARGET_STRING, TIVX_TARGET_VPAC_MSC1);
    }
    else
    {
        printf("Unable to create pyramid node!\n");
    }

    if(status == VX_SUCCESS)
    {
        vx_bool replicate[] = { vx_true_e, vx_true_e };
        status = vxReplicateNode(graph, pyramidObj->node, replicate, 2);
    }
    else
    {
        printf("Unable to set target as VPAC_MSC1!\n");
    }

    if(status == VX_FAILURE)
    {
        printf("Unable to replicate node!\n");
    }

    vxReleaseImage(&input);
    vxReleasePyramid(&output);

    return status;
}
