/*
 *
 * Copyright (c) 2021 Texas Instruments Incorporated
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
#include "app_pyramid_module.h"

vx_status app_init_pyramid(vx_context context, PyramidObj *pyramidObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    vx_pyramid pyramid_exemplar;
    vx_object_array pyramid_array;

        // Creating pyamid delay
    if(status == VX_SUCCESS)
    {
        pyramid_exemplar = vxCreatePyramid(context,
                        pyramidObj->pyramid_levels, VX_SCALE_PYRAMID_HALF,
                        pyramidObj->base_width, pyramidObj->base_height,
                        VX_DF_IMAGE_U8);
        status  = vxGetStatus((vx_reference)pyramid_exemplar);
    }

    if(status == VX_SUCCESS)
    {
        pyramid_array = vxCreateObjectArray(context, (vx_reference)pyramid_exemplar, NUM_CH);
        status  = vxGetStatus((vx_reference)pyramid_array);
    }

    if(status == VX_SUCCESS)
    {
        pyramidObj->pyramid_delay = vxCreateDelay(context, (vx_reference)pyramid_array, 2);
        status  = vxGetStatus((vx_reference)pyramidObj->pyramid_delay);
    }

    vxReleasePyramid(&pyramid_exemplar);
    vxReleaseObjectArray(&pyramid_array);

    return status;
}

void app_deinit_pyramid(PyramidObj *pyramidObj)
{
    vxReleaseDelay(&pyramidObj->pyramid_delay);
}

void app_delete_pyramid(PyramidObj *pyramidObj)
{
    if(pyramidObj->node != NULL)
    {
        vxReleaseNode(&pyramidObj->node);
    }
}

vx_status app_create_graph_pyramid(vx_graph graph, PyramidObj *pyramidObj, vx_object_array input_image_arr)
{
    vx_status status = VX_SUCCESS;

    vx_image input    = (vx_image)vxGetObjectArrayItem(input_image_arr, 0);

    //note: here the external_count is not incremented so no need to release this reference
    vx_object_array pyramid_array  = (vx_object_array)vxGetReferenceFromDelay(pyramidObj->pyramid_delay, -1);
    vx_pyramid output = (vx_pyramid)vxGetObjectArrayItem((vx_object_array)pyramid_array, 0);

    pyramidObj->node  = vxGaussianPyramidNode(graph, input, output);
    status = vxGetStatus((vx_reference)pyramidObj->node);

    if(status == VX_SUCCESS)
    {
       vxSetReferenceName((vx_reference)pyramidObj->node, "GaussianPyramid");
       status = vxSetNodeTarget(pyramidObj->node, VX_TARGET_STRING, TIVX_TARGET_VPAC_MSC1);
    }

    if(status == VX_SUCCESS)
    {
      vx_bool replicate[] = { vx_true_e, vx_true_e };
      status = vxReplicateNode(graph, pyramidObj->node, replicate, 2);
    }

    vxReleaseImage(&input);
    //As external_count is not incremented, no need to release this.
    //vxReleaseObjectArray(&pyramid_array);
    vxReleasePyramid(&output);

    return status;
}
