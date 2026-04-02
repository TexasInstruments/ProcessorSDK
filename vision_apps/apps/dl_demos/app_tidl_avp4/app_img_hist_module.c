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

#include "app_img_hist_module.h"

vx_status app_init_img_hist(vx_context context, ImgHistObj *imgHistObj, char *objName, vx_uint32 num_ch)
{
    vx_status status = VX_SUCCESS;

    vx_distribution histogram = vxCreateDistribution(context, imgHistObj->num_bins, imgHistObj->offset, imgHistObj->range);
    imgHistObj->output_hist_arr  = vxCreateObjectArray(context, (vx_reference)histogram, num_ch);
    vxReleaseDistribution(&histogram);

    vx_image input  = vxCreateImage(context, imgHistObj->in_width, imgHistObj->in_height, VX_DF_IMAGE_NV12);
    imgHistObj->input_img_arr  = vxCreateObjectArray(context, (vx_reference)input, num_ch);
    status = vxGetStatus((vx_reference)imgHistObj->input_img_arr);
    vxReleaseImage(&input);

    return status;
}

void app_deinit_img_hist(ImgHistObj *imgHistObj)
{
    vxReleaseObjectArray(&imgHistObj->output_hist_arr);
    vxReleaseObjectArray(&imgHistObj->input_img_arr);
}

void app_delete_img_hist(ImgHistObj *imgHistObj)
{
    if(imgHistObj->node != NULL)
    {
        vxReleaseNode(&imgHistObj->node);
    }
}

vx_status app_create_graph_img_hist(vx_graph graph, ImgHistObj *imgHistObj, vx_object_array input_arr)
{
    vx_status status = VX_SUCCESS;

    vx_image  input;
    vx_distribution histogram  = (vx_distribution)vxGetObjectArrayItem((vx_object_array)imgHistObj->output_hist_arr, 0);

    if(input_arr != NULL)
    {
        input   = (vx_image)vxGetObjectArrayItem((vx_object_array)input_arr, 0);
    }
    else
    {
        input   = (vx_image)vxGetObjectArrayItem((vx_object_array)imgHistObj->input_img_arr, 0);
    }

    imgHistObj->node = tivxImgHistNode(graph,
                                       input,
                                       histogram);

    status = vxSetNodeTarget(imgHistObj->node, VX_TARGET_STRING, TIVX_TARGET_MPU_1);
    vxSetReferenceName((vx_reference)imgHistObj->node, "ImgHistNode");

    vx_bool replicate[] = {vx_true_e, vx_true_e};
    vxReplicateNode(graph, imgHistObj->node, replicate, 2);

    vxReleaseImage(&input);
    vxReleaseDistribution(&histogram);

    return(status);
}

