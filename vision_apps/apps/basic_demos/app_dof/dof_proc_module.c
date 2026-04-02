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
#include "dof_proc_module.h"



vx_status app_init_dofproc(vx_context context, DofProcObj *dofprocObj, char *objName)
{
    vx_status status = VX_SUCCESS;

    tivx_dmpac_dof_params_init(&dofprocObj->dof_params);

    if(dofprocObj->enable_temporal_predicton_flow_vector == 0)
    {
      dofprocObj->dof_params.base_predictor[0] = TIVX_DMPAC_DOF_PREDICTOR_DELAY_LEFT;
      dofprocObj->dof_params.base_predictor[1] = TIVX_DMPAC_DOF_PREDICTOR_PYR_COLOCATED;

      dofprocObj->dof_params.inter_predictor[0] = TIVX_DMPAC_DOF_PREDICTOR_DELAY_LEFT;
      dofprocObj->dof_params.inter_predictor[1] = TIVX_DMPAC_DOF_PREDICTOR_PYR_COLOCATED;
    }

    dofprocObj->dof_config = vxCreateUserDataObject(context, "tivx_dmpac_dof_params_t",
                                             sizeof(tivx_dmpac_dof_params_t), NULL);
    APP_ASSERT_VALID_REF(dofprocObj->dof_config);
    vxSetReferenceName((vx_reference)dofprocObj->dof_config, "DofConfiguration");

    dofprocObj->dof_params.vertical_search_range[0] = 48;
    dofprocObj->dof_params.vertical_search_range[1] = 48;
    dofprocObj->dof_params.horizontal_search_range = 191;
    dofprocObj->dof_params.median_filter_enable = 1;
    dofprocObj->dof_params.motion_smoothness_factor = 12;
    dofprocObj->dof_params.motion_direction = 1; /* 0: for side camera */

    status = vxCopyUserDataObject(dofprocObj->dof_config, 0, sizeof(tivx_dmpac_dof_params_t), &dofprocObj->dof_params, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    APP_ASSERT(status==VX_SUCCESS);

    if(dofprocObj->enable_temporal_predicton_flow_vector)
    {
        vx_image flow_vector_field_exemplar;

        flow_vector_field_exemplar = vxCreateImage(context, dofprocObj->width, dofprocObj->height, VX_DF_IMAGE_U32);
        APP_ASSERT_VALID_REF(flow_vector_field_exemplar);
        
        dofprocObj->flow_vector_field_delay = vxCreateDelay(context, (vx_reference)flow_vector_field_exemplar, 2);
        APP_ASSERT_VALID_REF(dofprocObj->flow_vector_field_delay);

        dofprocObj->flow_vector_field_in = (vx_image)vxGetReferenceFromDelay(dofprocObj->flow_vector_field_delay, -1);
        APP_ASSERT_VALID_REF(dofprocObj->flow_vector_field_in);
        vxSetReferenceName((vx_reference)dofprocObj->flow_vector_field_in, "FlowVectorIn");

        dofprocObj->flow_vector_field_out = (vx_image)vxGetReferenceFromDelay(dofprocObj->flow_vector_field_delay,  0);
        APP_ASSERT_VALID_REF(dofprocObj->flow_vector_field_out);
        vxSetReferenceName((vx_reference)dofprocObj->flow_vector_field_out, "FlowVectorOut");

        vxReleaseImage(&flow_vector_field_exemplar);
    }
    else
    {
        dofprocObj->flow_vector_field_delay =  NULL;
        dofprocObj->flow_vector_field_in = NULL;
        dofprocObj->flow_vector_field_out = vxCreateImage(context, dofprocObj->width, dofprocObj->height, VX_DF_IMAGE_U32);
        APP_ASSERT_VALID_REF(dofprocObj->flow_vector_field_out);
        vxSetReferenceName((vx_reference)dofprocObj->flow_vector_field_out, "FlowVectorOut");

    }

    return status;
}

void app_deinit_dofproc(DofProcObj *dofprocObj)
{

  vxReleaseUserDataObject(&dofprocObj->dof_config);
  if(dofprocObj->enable_temporal_predicton_flow_vector)
  {
    vxReleaseDelay(&dofprocObj->flow_vector_field_delay); 
  }
  else
  {
    vxReleaseImage(&dofprocObj->flow_vector_field_out);
  }

}

void app_delete_dofproc(DofProcObj *dofprocObj)
{

  if(dofprocObj->node != NULL)
  {
    vxReleaseNode(&dofprocObj->node);
  }else{

  }
 
}

vx_status app_create_graph_dofproc(vx_graph graph, DofProcObj *dofprocObj,
                                    vx_user_data_object  configuration,
                                    vx_pyramid           input_current_base,
                                    vx_pyramid           input_reference_base,
                                    vx_image             flow_vector_in,
                                    vx_image             flow_vector_out )
{
  vx_status status = VX_SUCCESS;
                                      
  dofprocObj->node = tivxDmpacDofNode(
                            graph,
                            configuration,
                            NULL,
                            NULL,
                            input_current_base,
                            input_reference_base,
                            flow_vector_in,
                            NULL,
                            NULL,
                            flow_vector_out,
                            NULL);
  APP_ASSERT_VALID_REF(dofprocObj->node);
  status = vxSetNodeTarget(dofprocObj->node, VX_TARGET_STRING, TIVX_TARGET_DMPAC_DOF);
  APP_ASSERT(status==VX_SUCCESS);
  vxSetReferenceName((vx_reference)dofprocObj->node, "DenseOpticalFlow");

  return status;
}
