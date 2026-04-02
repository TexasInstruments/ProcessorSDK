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
#include "srv_calibration_applib.h"

typedef struct
{
    vx_context           vxContext;

    vx_graph             vxGraph;

    /* Have the user provide just one then internally populate the camId */
    vx_user_data_object  point_detect_in_config;

    vx_user_data_object  in_ldclut;

    vx_object_array      in_array;

    vx_object_array      buf_bwluma_frame_array;

    vx_user_data_object  pose_estimation_in_config;

    vx_user_data_object  out_calmat;

    /* Intermediate params */
    vx_object_array      point_detect_in_config_array;

    vx_object_array      in_corner_points;

    vx_image             in_element;

    vx_user_data_object  point_detect_in_config_element;

    vx_user_data_object  out_configuration;

    vx_image             buf_bwluma_frame_element;

    /* Nodes */
    vx_node              point_detect_node;

    vx_node              pose_estimation_node;

} srv_calib_context;

static vx_status verify_create_params(srv_calib_createParams *createParams)
{
    vx_status vxStatus = VX_SUCCESS;

    if (NULL == createParams->vxGraph)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: vxGraph is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->vxContext)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: vxContext is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->point_detect_in_config)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: point_detect_in_config is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->in_ldclut)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: in_ldclut is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->buf_bwluma_frame_array)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: buf_bwluma_frame_array is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->pose_estimation_in_config)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: pose_estimation_in_config is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->out_calmat)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: out_calmat is NULL\n");
        vxStatus = VX_FAILURE;
    }

    if (NULL == createParams->in_array)
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create: in_array is NULL\n");
        vxStatus = VX_FAILURE;
    }

    return vxStatus;
}

static void srv_calib_setParams(srv_calib_handle handle, srv_calib_createParams *createParams)
{
    srv_calib_context *appCntxt;
    appCntxt = (srv_calib_context *)handle;

    appCntxt->vxGraph   = createParams->vxGraph;
    appCntxt->vxContext = createParams->vxContext;
    appCntxt->point_detect_in_config   = createParams->point_detect_in_config;
    appCntxt->in_ldclut = createParams->in_ldclut;
    appCntxt->buf_bwluma_frame_array = createParams->buf_bwluma_frame_array;
    appCntxt->pose_estimation_in_config   = createParams->pose_estimation_in_config;
    appCntxt->out_calmat = createParams->out_calmat;
    appCntxt->in_array = createParams->in_array;
}

static void srv_calib_createIntermediateParams(srv_calib_handle handle)
{
    vx_uint32 i;
    srv_calib_context *appCntxt;
    appCntxt = (srv_calib_context *)handle;
    vx_user_data_object exemplar;
    svPointDetect_t in_point_detect_params;

    exemplar = vxCreateUserDataObject(appCntxt->vxContext, "svACDetectStructFinalCorner_t",
                                      sizeof(svACDetectStructFinalCorner_t), NULL);

    appCntxt->in_corner_points = vxCreateObjectArray(appCntxt->vxContext, (vx_reference)exemplar, NUM_CAMERAS);

    vxReleaseUserDataObject(&exemplar);

    appCntxt->point_detect_in_config_array = vxCreateObjectArray(appCntxt->vxContext, (vx_reference)appCntxt->point_detect_in_config, NUM_CAMERAS);

    /* Load point_detect_in_config_array correctly */
    vxCopyUserDataObject(appCntxt->point_detect_in_config, 0, sizeof(svPointDetect_t),
                         &in_point_detect_params, VX_READ_ONLY,VX_MEMORY_TYPE_HOST);

    for (i = 0; i < NUM_CAMERAS; i++)
    {
        exemplar = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)appCntxt->point_detect_in_config_array, i);

        /* copying correct camera id to each user data object */
        in_point_detect_params.camera_id = i;

        vxCopyUserDataObject(exemplar, 0, sizeof(svPointDetect_t),
                             &in_point_detect_params, VX_WRITE_ONLY,VX_MEMORY_TYPE_HOST);

        vxReleaseUserDataObject(&exemplar);
    }
}

static void srv_calib_freeIntermediateParams(srv_calib_handle handle)
{
    srv_calib_context *appCntxt;
    appCntxt = (srv_calib_context *)handle;

    vxReleaseImage(&appCntxt->in_element);
    vxReleaseUserDataObject(&appCntxt->out_configuration);
    vxReleaseUserDataObject(&appCntxt->point_detect_in_config_element);
    vxReleaseImage(&appCntxt->buf_bwluma_frame_element);
    vxReleaseObjectArray(&appCntxt->in_corner_points);
    vxReleaseObjectArray(&appCntxt->point_detect_in_config_array);
}

static void srv_calib_createNodes(srv_calib_handle handle)
{
    srv_calib_context *appCntxt;
    appCntxt = (srv_calib_context *)handle;
    vx_bool replicate[] = { vx_true_e, vx_false_e, vx_true_e, vx_true_e, vx_true_e };

    /* Creating nodes and constructing applib using replicate node */
    appCntxt->out_configuration = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)appCntxt->in_corner_points, 0);

    appCntxt->point_detect_in_config_element = (vx_user_data_object)vxGetObjectArrayItem((vx_object_array)appCntxt->point_detect_in_config_array, 0);

    appCntxt->buf_bwluma_frame_element = (vx_image)vxGetObjectArrayItem((vx_object_array)appCntxt->buf_bwluma_frame_array, 0);

    appCntxt->in_element = (vx_image)vxGetObjectArrayItem((vx_object_array)appCntxt->in_array, 0);

    appCntxt->point_detect_node = tivxPointDetectNode(appCntxt->vxGraph, appCntxt->point_detect_in_config_element, appCntxt->in_ldclut, appCntxt->in_element, appCntxt->out_configuration, appCntxt->buf_bwluma_frame_element);
    #if defined (SOC_J722S)
    vxSetNodeTarget(appCntxt->point_detect_node, VX_TARGET_STRING, TIVX_TARGET_DSP2);
    #endif

    vxReplicateNode(appCntxt->vxGraph, appCntxt->point_detect_node, replicate, 5);

    appCntxt->pose_estimation_node = tivxPoseEstimationNode(appCntxt->vxGraph, appCntxt->pose_estimation_in_config, appCntxt->in_ldclut, appCntxt->in_corner_points, appCntxt->out_calmat);
    #if defined (SOC_J722S)
    vxSetNodeTarget(appCntxt->pose_estimation_node, VX_TARGET_STRING, TIVX_TARGET_DSP2);
    #endif
}

static void srv_calib_freeNodes(srv_calib_handle handle)
{
    srv_calib_context *appCntxt;
    appCntxt = (srv_calib_context *)handle;

    vxReleaseNode(&appCntxt->point_detect_node);
    vxReleaseNode(&appCntxt->pose_estimation_node);
}

static void srv_calib_free(srv_calib_handle handle)
{
    if (handle)
    {
        tivxMemFree(handle, sizeof(srv_calib_context), TIVX_MEM_EXTERNAL);
        handle = NULL;
    }

    return;
}

srv_calib_handle srv_calib_create(srv_calib_createParams *createParams)
{
    srv_calib_handle  handle = NULL;
    vx_status         vxStatus = VX_SUCCESS;

    vxStatus = verify_create_params(createParams);

    if (VX_SUCCESS == vxStatus)
    {
        handle = (srv_calib_handle)tivxMemAlloc(sizeof(srv_calib_context), TIVX_MEM_EXTERNAL);

        if (NULL == handle)
        {
            vxStatus = VX_FAILURE;
            return NULL;
        }

        /* Set applib-level create parameters */
        srv_calib_setParams(handle, createParams);

        /* Create additional data objects */
        srv_calib_createIntermediateParams(handle);

        /* Construct nodes */
        srv_calib_createNodes(handle);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "srv_calib_create failed: returning NULL handle\n");
    }

    return handle;
} /* srv_calib_create */

vx_object_array srv_calib_get_corners(srv_calib_handle handle)
{
    srv_calib_context *appCntxt;
    appCntxt = (srv_calib_context *)handle;

    vx_object_array return_array;

    return_array = appCntxt->in_corner_points;

    return return_array;
}

void srv_calib_delete(srv_calib_handle handle)
{
    if (handle)
    {
        srv_calib_freeIntermediateParams(handle);

        srv_calib_freeNodes(handle);

        srv_calib_free(handle);
    }

    return;
} /* srv_calib_delete */

