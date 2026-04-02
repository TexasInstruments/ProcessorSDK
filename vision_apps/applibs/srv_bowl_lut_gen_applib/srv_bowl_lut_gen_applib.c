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

#include "TI/tivx_srv.h"
#include "srv_bowl_lut_gen_applib.h"

typedef struct
{
    vx_context           vxContext;

    vx_graph             vxGraph;

    /* Applib API */
    vx_user_data_object  in_config;

    vx_user_data_object  in_calmat;

    vx_user_data_object  in_offset;

    vx_user_data_object  in_ldclut;

    vx_array             out_gpulut3d;

    /* Intermediate params */
    vx_array             lut3dxyz;

    vx_user_data_object  calmat_scaled;

    /* Nodes */
    vx_node              bowl_gen_node;

    vx_node              gpu_lut_gen_node;

} srv_bowl_lut_gen_context;

static vx_status verify_create_params(srv_bowl_lut_gen_createParams *createParams)
{
    vx_status vxStatus = VX_SUCCESS;

    if (NULL == createParams->vxGraph)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create: vxGraph is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->vxContext)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create: vxContext is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->in_config)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create: in_config is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->in_calmat)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create: in_calmat is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->in_offset)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create: in_offset is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->in_ldclut)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create: in_ldclut is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->out_gpulut3d)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create: out_gpulut3d is NULL\n");
        vxStatus = VX_FAILURE;
    }

    return vxStatus;
}

static void srv_bowl_lut_gen_setParams(srv_bowl_lut_gen_handle handle, srv_bowl_lut_gen_createParams *createParams)
{
    srv_bowl_lut_gen_context *appCntxt;
    appCntxt = (srv_bowl_lut_gen_context *)handle;

    appCntxt->vxGraph      = createParams->vxGraph;
    appCntxt->vxContext    = createParams->vxContext;
    appCntxt->in_config    = createParams->in_config;
    appCntxt->in_calmat    = createParams->in_calmat;
    appCntxt->in_offset    = createParams->in_offset;
    appCntxt->in_ldclut    = createParams->in_ldclut;
    appCntxt->out_gpulut3d = createParams->out_gpulut3d;
}

static void srv_bowl_lut_gen_createIntermediateParams(srv_bowl_lut_gen_handle handle)
{
    srv_bowl_lut_gen_context *appCntxt;
    appCntxt = (srv_bowl_lut_gen_context *)handle;
    svGpuLutGen_t               in_params;

    vxCopyUserDataObject(appCntxt->in_config, 0, sizeof(svGpuLutGen_t), &in_params, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);

    appCntxt->lut3dxyz = vxCreateArray(appCntxt->vxContext, VX_TYPE_FLOAT32, (in_params.SVOutDisplayHeight/in_params.subsampleratio)*(in_params.SVOutDisplayWidth/in_params.subsampleratio)*3);

    appCntxt->calmat_scaled = vxCreateUserDataObject(appCntxt->vxContext, "svACCalmatStruct_t",
                                       sizeof(svACCalmatStruct_t), NULL);
}

static void srv_bowl_lut_gen_freeIntermediateParams(srv_bowl_lut_gen_handle handle)
{
    srv_bowl_lut_gen_context *appCntxt;
    appCntxt = (srv_bowl_lut_gen_context *)handle;

    vxReleaseArray(&appCntxt->lut3dxyz);
    vxReleaseUserDataObject(&appCntxt->calmat_scaled);
}

static void srv_bowl_lut_gen_createNodes(srv_bowl_lut_gen_handle handle)
{
    srv_bowl_lut_gen_context *appCntxt;
    appCntxt = (srv_bowl_lut_gen_context *)handle;

    appCntxt->bowl_gen_node = tivxGenerate3DbowlNode(appCntxt->vxGraph, appCntxt->in_config, appCntxt->in_calmat, appCntxt->in_offset, appCntxt->lut3dxyz, appCntxt->calmat_scaled);
    #if defined (SOC_J722S)
    vxSetNodeTarget(appCntxt->bowl_gen_node, VX_TARGET_STRING, TIVX_TARGET_DSP2);
    #endif
    appCntxt->gpu_lut_gen_node = tivxGenerateGpulutNode(appCntxt->vxGraph, appCntxt->in_config, appCntxt->in_ldclut, appCntxt->calmat_scaled, appCntxt->lut3dxyz, appCntxt->out_gpulut3d);
    #if defined (SOC_J722S)
    vxSetNodeTarget(appCntxt->gpu_lut_gen_node, VX_TARGET_STRING, TIVX_TARGET_DSP2);
    #endif
}

static void srv_bowl_lut_gen_freeNodes(srv_bowl_lut_gen_handle handle)
{
    srv_bowl_lut_gen_context *appCntxt;
    appCntxt = (srv_bowl_lut_gen_context *)handle;

    vxReleaseNode(&appCntxt->bowl_gen_node);
    vxReleaseNode(&appCntxt->gpu_lut_gen_node);
}

static void srv_bowl_lut_gen_free(srv_bowl_lut_gen_handle handle)
{
    if (handle)
    {
        tivxMemFree(handle, sizeof(srv_bowl_lut_gen_context), TIVX_MEM_EXTERNAL);
        handle = NULL;
    }

    return;
}

srv_bowl_lut_gen_handle srv_bowl_lut_gen_create(srv_bowl_lut_gen_createParams *createParams)
{
    srv_bowl_lut_gen_handle  handle = NULL;
    vx_status         vxStatus = VX_SUCCESS;

    vxStatus = verify_create_params(createParams);

    if (VX_SUCCESS == vxStatus)
    {
        handle = (srv_bowl_lut_gen_handle)tivxMemAlloc(sizeof(srv_bowl_lut_gen_context), TIVX_MEM_EXTERNAL);

        if (NULL == handle)
        {
            vxStatus = VX_FAILURE;
            return NULL;
        }

        /* Set applib-level create parameters */
        srv_bowl_lut_gen_setParams(handle, createParams);

        /* Create additional data objects */
        srv_bowl_lut_gen_createIntermediateParams(handle);

        /* Construct nodes */
        srv_bowl_lut_gen_createNodes(handle);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_bowl_lut_gen_create failed: returning NULL handle\n");
    }

    return handle;
} /* srv_bowl_lut_gen_create */

void srv_bowl_lut_gen_delete(srv_bowl_lut_gen_handle handle)
{
    if (handle)
    {
        srv_bowl_lut_gen_freeIntermediateParams(handle);

        srv_bowl_lut_gen_freeNodes(handle);

        srv_bowl_lut_gen_free(handle);
    }

    return;
} /* srv_bowl_lut_gen_delete */

