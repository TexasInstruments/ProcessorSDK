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
#include "app_dof_module.h"

vx_status app_init_dof(vx_context context, DOFObj *dofObj, char *objName)
{
    vx_status status = VX_SUCCESS;
    vx_user_data_object dof_config;
    vx_int32 ch;

    vx_image flow_vector_field_exemplar;

    tivx_dmpac_dof_params_init(&dofObj->dof_params);

    if(dofObj->enable_temporal_prediction == 0)
    {
      dofObj->dof_params.base_predictor[0] = TIVX_DMPAC_DOF_PREDICTOR_DELAY_LEFT;
      dofObj->dof_params.base_predictor[1] = TIVX_DMPAC_DOF_PREDICTOR_PYR_COLOCATED;

      dofObj->dof_params.inter_predictor[0] = TIVX_DMPAC_DOF_PREDICTOR_DELAY_LEFT;
      dofObj->dof_params.inter_predictor[1] = TIVX_DMPAC_DOF_PREDICTOR_PYR_COLOCATED;
    }

    dof_config = vxCreateUserDataObject(context, "tivx_dmpac_dof_params_t",
                                             sizeof(tivx_dmpac_dof_params_t), NULL);
    status = vxGetStatus((vx_reference)dof_config);

    if(status == VX_SUCCESS)
    {
      dofObj->config_arr = vxCreateObjectArray(context, (vx_reference) dof_config, NUM_CH);
      vxReleaseUserDataObject(&dof_config);
      status = vxGetStatus((vx_reference)dofObj->config_arr);
    }

    if(status == VX_SUCCESS)
    {
      dofObj->dof_params.vertical_search_range[0] = 32;
      dofObj->dof_params.vertical_search_range[1] = 62;
      dofObj->dof_params.horizontal_search_range = 191;
      dofObj->dof_params.median_filter_enable = 1;
      dofObj->dof_params.motion_smoothness_factor = 12;
      dofObj->dof_params.motion_direction = 0; /* 0: for side camera */
    }

    for(ch = 0; ch < NUM_CH; ch++)
    {
      dofObj->dof_params.motion_direction = 0; /* 0: for side camera */

      if(ch == 0) /* In the YUV input ch0 - front, ch1 - right, ch2 - left */
        dofObj->dof_params.motion_direction = 1; /* 1: for front camera */

      dof_config = (vx_user_data_object) vxGetObjectArrayItem( dofObj->config_arr, ch);
      status = vxCopyUserDataObject(dof_config, 0, sizeof(tivx_dmpac_dof_params_t), &dofObj->dof_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
      vxReleaseUserDataObject(&dof_config);
    }

    //Creating flow vector delays
    if(status == VX_SUCCESS)
    {
      flow_vector_field_exemplar = vxCreateImage(context, dofObj->width, dofObj->height, VX_DF_IMAGE_U32);
      status  = vxGetStatus((vx_reference)flow_vector_field_exemplar);
    }

    if(status == VX_SUCCESS)
    {
      dofObj->flow_vector_field_array = vxCreateObjectArray(context, (vx_reference)flow_vector_field_exemplar, NUM_CH);
      status  = vxGetStatus((vx_reference)dofObj->flow_vector_field_array);
    }

    if(dofObj->enable_temporal_prediction == 1)
    {
      if(status == VX_SUCCESS)
      {
        dofObj->flow_vector_field_delay = vxCreateDelay(context, (vx_reference)dofObj->flow_vector_field_array, 2);
        status  = vxGetStatus((vx_reference)dofObj->flow_vector_field_delay);
      }
    }
    else
    {
      dofObj->flow_vector_field_delay = NULL;
    }

    vxReleaseImage(&flow_vector_field_exemplar);

    return status;
}

void app_deinit_dof(DOFObj *dofObj)
{
    vxReleaseObjectArray(&dofObj->config_arr);
    APP_PRINTF("DOF: config deinit done!\n");
    vxReleaseObjectArray(&dofObj->flow_vector_field_array);
    APP_PRINTF("DOF: flow vector field deinit done!\n");
    if(dofObj->enable_temporal_prediction == 1)
    {
        vxReleaseDelay(&dofObj->flow_vector_field_delay);
        APP_PRINTF("DOF: flow vector delay deinit done!\n");
    }
}


void app_delete_dof(DOFObj *dofObj)
{
  if(dofObj->node != NULL)
  {
    vxReleaseNode(&dofObj->node);
  }
}


vx_status app_create_graph_dof(vx_graph graph, DOFObj *dofObj, vx_delay pyramid_delay)
{
  vx_status status = VX_SUCCESS;

  vx_image flow_vector_field_in;
  vx_image flow_vector_field_out;

  vx_user_data_object dof_config = (vx_user_data_object) vxGetObjectArrayItem((vx_object_array)dofObj->config_arr,0);

  vx_object_array pyr_cur_array = (vx_object_array)vxGetReferenceFromDelay(pyramid_delay, 0);
  vx_object_array pyr_ref_array = (vx_object_array)vxGetReferenceFromDelay(pyramid_delay, -1);

  vx_pyramid pyr_cur = (vx_pyramid)vxGetObjectArrayItem((vx_object_array)pyr_cur_array, 0);
  vx_pyramid pyr_ref = (vx_pyramid)vxGetObjectArrayItem((vx_object_array)pyr_ref_array, 0);

  if(dofObj->enable_temporal_prediction == 1)
  {
    vx_object_array flow_vector_field_in_array  = (vx_object_array)vxGetReferenceFromDelay(dofObj->flow_vector_field_delay, -1);
    flow_vector_field_in  = (vx_image)vxGetObjectArrayItem(flow_vector_field_in_array, 0);

    vx_object_array flow_vector_field_out_array = (vx_object_array)vxGetReferenceFromDelay(dofObj->flow_vector_field_delay,  0);
    flow_vector_field_out = (vx_image)vxGetObjectArrayItem(flow_vector_field_out_array, 0);
  }
  else
  {
    flow_vector_field_in  = NULL;
    flow_vector_field_out = (vx_image)vxGetObjectArrayItem(dofObj->flow_vector_field_array, 0);
  }

  dofObj->node = tivxDmpacDofNode(
                            graph,
                            dof_config,
                            NULL,
                            NULL,
                            pyr_cur,
                            pyr_ref,
                            flow_vector_field_in,
                            NULL,
                            NULL,
                            flow_vector_field_out,
                            NULL);

  status = vxGetStatus((vx_reference)dofObj->node);

  if(status == VX_SUCCESS)
  {
    vxSetReferenceName((vx_reference)dofObj->node, "DenseOpticalFlow");
    status = vxSetNodeTarget(dofObj->node, VX_TARGET_STRING, TIVX_TARGET_DMPAC_DOF);
  }

  if(status == VX_SUCCESS)
  {
    vx_bool replicate[] = { vx_true_e, vx_false_e, vx_false_e, vx_true_e, vx_true_e, vx_true_e, vx_false_e, vx_false_e, vx_true_e, vx_false_e};
    vxReplicateNode(graph, dofObj->node, replicate, 10);
  }

  vxReleaseUserDataObject(&dof_config);
  vxReleasePyramid(&pyr_cur);
  vxReleasePyramid(&pyr_ref);
  if(dofObj->enable_temporal_prediction == 1)
  {
    vxReleaseImage(&flow_vector_field_in);
  }
  vxReleaseImage(&flow_vector_field_out);

  return status;
}
