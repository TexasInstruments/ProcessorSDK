/*
 *
 * Copyright (c) 2017 Texas Instruments Incorporated
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
#include "dof_pyramid_module.h"



vx_status app_init_pyramid(vx_context context, PyramidObj *pyramidObj, char *objName,vx_int32 bufq_depth)
{
  vx_status status = VX_SUCCESS;
  vx_pyramid pyr_exemplar;
  uint32_t buf_id;

  // for  input
  for(buf_id=0; buf_id<bufq_depth; buf_id++)
  {
      pyramidObj->input_img[buf_id] = vxCreateImage(context, pyramidObj->width, pyramidObj->height, pyramidObj->in_vx_df_image);
      APP_ASSERT_VALID_REF(pyramidObj->input_img[buf_id]);
  }

  pyr_exemplar = vxCreatePyramid(context,
              pyramidObj->dof_levels, VX_SCALE_PYRAMID_HALF,
              pyramidObj->width, pyramidObj->height,
              pyramidObj->in_vx_df_image);
  APP_ASSERT_VALID_REF(pyr_exemplar);

  pyramidObj->pyr_delay = vxCreateDelay(context, (vx_reference)pyr_exemplar, 2);
  APP_ASSERT_VALID_REF(pyramidObj->pyr_delay);

  pyramidObj->pyr_ref = (vx_pyramid)vxGetReferenceFromDelay(pyramidObj->pyr_delay, -1);
  APP_ASSERT_VALID_REF(pyramidObj->pyr_ref);
  vxSetReferenceName((vx_reference)pyramidObj->pyr_ref, "PyramidReference");

  pyramidObj->pyr_cur = (vx_pyramid)vxGetReferenceFromDelay(pyramidObj->pyr_delay,  0);
  APP_ASSERT_VALID_REF(pyramidObj->pyr_cur);
  vxSetReferenceName((vx_reference)pyramidObj->pyr_cur, "PyramidCurrent");

  /* exemplar not needed any more */
  vxReleasePyramid(&pyr_exemplar);


  return status;
}

void app_deinit_pyramid(PyramidObj *pyramidObj,vx_int32 bufq_depth)
{
  uint32_t buf_id;

  vxReleaseDelay(&pyramidObj->pyr_delay);
  
  // for  input
  for(buf_id=0; buf_id<bufq_depth; buf_id++)
  {
      vxReleaseImage(&pyramidObj->input_img[buf_id]);
  }
}

void app_delete_pyramid(PyramidObj *pyramidObj)
{
  if(pyramidObj->node != NULL)
  {
    vxReleaseNode(&pyramidObj->node);
  }else{

  }
}

vx_status app_create_graph_pyramid(vx_graph graph, PyramidObj *pyramidObj, vx_image input_img, vx_pyramid output_pyramid )
{
    vx_status status = VX_SUCCESS;

    pyramidObj->node  = vxGaussianPyramidNode( graph, input_img , output_pyramid);
    APP_ASSERT_VALID_REF(pyramidObj->node);
    status = vxSetNodeTarget(pyramidObj->node, VX_TARGET_STRING, TIVX_TARGET_VPAC_MSC1);
    APP_ASSERT(status==VX_SUCCESS);
    vxSetReferenceName((vx_reference)pyramidObj->node, "GaussianPyramid");
    
    return status;
}