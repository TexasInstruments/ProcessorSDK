/*
 *
 * Copyright (c) 2025 Texas Instruments Incorporated
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

#ifndef VX_VPAC_FC_TARGET_FVID2_PRIV_H_
#define VX_VPAC_FC_TARGET_FVID2_PRIV_H_

#include "TI/tivx.h"
#include "TI/hwa_vpac_fc.h"
#include "tivx_hwa_kernels.h"
#include "tivx_kernel_vpac_fc.h"
#include "TI/hwa_vpac_viss.h"
#include "tivx_kernel_vpac_viss.h"
#include "TI/hwa_vpac_msc.h"
#include "tivx_kernel_vpac_msc.h"
#include "TI/tivx_target_kernel.h"
#include "tivx_kernels_target_utils.h"
#include "TI/tivx_event.h"
#include "TI/tivx_mutex.h"
#include "idcc.h"

#include "vhwa/include/vhwa_m2mFlexConnect.h"
#include "vhwa/include/vhwa_m2mViss.h"
#include "vhwa/include/vhwa_m2mMsc.h"

#include "utils/perf_stats/include/app_perf_stats.h"

#include "../vpac_viss/vx_vpac_viss_target_priv.h"
#include "../vpac_msc/vx_vpac_multi_scale_output_target_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************
 *      VPAC_VISS STRUCTURES
 *********************************/

typedef struct
{
    /*! Flag to indicate if this object is free or not */
    uint32_t                            isAlloc;

    /*! FC M2M Driver Create Parameter */
    Vhwa_M2mFcCreatePrms              createArgs;

    /*! VISS M2M Driver Handle */
    Fvid2_Handle                        handle;
    /*! Mutex used for waiting for process completion */
    tivx_event                          waitForProcessCmpl;
    /*! Mutex used for waiting for protecting config */
    tivx_mutex                          config_lock;

    int32_t                     firstNode;
    /**< First Node in Graph */
    int32_t                     lastNode;
    /**< Last Node in Graph */

    Vhwa_M2mFcGraphPathInfo     pathInfo;

    void                        *fc_prms_target_ptr;

    /*! FC M2M Driver Input Frame List, used for providing
     *  an array of input frames */
    Fvid2_FrameList                     inFrmList;
    /*! FC M2M Driver Output Frame List, used for providing
     *  an array of output frames */
    Fvid2_FrameList                     outFrmList;
    /*! FC M2M Driver Input Frames */
    Fvid2_Frame                         inFrm[VHWA_M2M_VISS_MAX_INPUTS];
    /*! FC M2M Driver Output Frames */
    Fvid2_Frame                         outFrm[(2U * TIVX_KERNEL_VPAC_FC_MAX_IMAGE_OUTPUT) + VHWA_M2M_VISS_MAX_OUTPUTS + 2U];
    /*! FC M2M Driver Callback parameters */
    Fvid2_CbParams                      cbPrms;

    /*! Number of input buffer pointers */
    uint32_t                            num_in_buf;
    /*! Number of output buffer pointers for each output */
    uint32_t                            num_out_buf_addr[(2U * TIVX_KERNEL_VPAC_FC_MAX_IMAGE_OUTPUT) + VHWA_M2M_VISS_MAX_OUTPUTS + 2U];

    /*! Instance ID of the FC driver */
    uint32_t                            fc_drv_inst_id;
    /*! HWA Performance ID */
    app_perf_hwa_id_t                   hwa_perf_id;

    tivxVpacVissObj vissObj;

    tivxVpacMscScaleObj msc_obj;
} tivxVpacFcObj;

typedef struct
{
    uint32_t mscInstId;

    Ptr mscArgs;
} tivxVpacMscArgs;

typedef struct
{
    /*! Protects allocation of fc objects */
    tivx_mutex       lock;

    tivxVpacFcObj fcObj[VHWA_FC_DRV_MAX_HANDLES];
} tivxVpacFcInstObj;

/*********************************
 *      Function Prototypes
 *********************************/
vx_status tivxVpacFcVissSetConfigInDrv(tivxVpacFcObj *fcObj);

vx_status tivxVpacFcVissSetConfigBuffer(tivxVpacFcObj *fcObj);
void tivxVpacFcVissDeleteConfigBuffer(tivxVpacFcObj *fcObj);

vx_status tivxVpacFcMscScaleSetCoeffsCmd(tivxVpacFcObj *fcObj, const tivx_obj_desc_user_data_object_t *usr_data_obj, const tivx_obj_desc_user_data_object_t *usr_data_obj2);
vx_status tivxVpacFcMscScaleSetOutputParamsCmd(tivxVpacFcObj *fcObj, tivx_obj_desc_user_data_object_t *usr_data_obj[], uint16_t num_params);
vx_status tivxVpacFcMscScaleSetInputParamsCmd(tivxVpacFcObj *fcObj, const tivx_obj_desc_user_data_object_t *usr_data_obj, const tivx_obj_desc_user_data_object_t *usr_data_obj2);
vx_status tivxVpacFcMscScaleSetCropParamsCmd(tivxVpacFcObj *fcObj, tivx_obj_desc_user_data_object_t *usr_data_obj[], uint16_t num_params);

#ifdef __cplusplus
}
#endif

#endif /* VX_VPAC_FC_TARGET_FVID2_PRIV_H_ */

