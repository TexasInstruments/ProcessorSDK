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

#ifndef _TI_2A_WRAPPER_H
#define _TI_2A_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <TI/tivx.h>
#include <TI/hwa_vpac_viss.h>
#include <TI/j7_kernels_imaging_aewb.h>

#include "idcc.h"
#include "TI_aaa_ae.h"
#include "TI_aaa_awb.h"
#include "TI/tivx_target_kernel.h"
#include <iss_sensors.h>

/*!
 * \brief Auto exposure and white balance run time parameters
 * \ingroup group_vision_function_imaging_ti_2a_wrapper
 */
typedef struct
{
    /* Input to DCC parser */
    dcc_parser_input_params_t * dcc_input_params;

    /* Output from DCC parser */
    dcc_parser_output_params_t * dcc_output_params;

    /* Pointer to AWB params */
    awbprm_t * p_awb_params;

    /* Pointer to AE params */
    tiae_prm_t * p_ae_params;

    /* Pointer to H3A merge buffer */
    h3a_aewb_paxel_data_t * p_h3a_merge;

    /* Pointer to updated H3A result data */
    h3a_aewb_paxel_data_t * awb_h3a_res;

    /* Pointer to AWB scratch buffer */
    uint8_t * scratch_memory;

    /* Pointer to previous AEWB result */
    tivx_ae_awb_params_t ae_awb_result_prev;

    /* Index of frame being currently processed */
    uint32_t  frame_count;

    /* Flag to indicate if sensor applies White Balance correction pre-HDR merge.
    1=True, 0=False */
    int32_t   sensor_pre_wb_gain;

    /* Flag to indicate if AWB was skipped by TI_2A_wrapper_process function
    1=YES, 0=NO */
    uint8_t skipAWB;

    /* Flag to indicate if AE was skipped by TI_2A_wrapper_process function
    1=YES, 0=NO */
    uint8_t skipAE;

} TI_2A_wrapper;

/*!
 * \brief Structure to get Exposure constraints from sensor
 * \ingroup group_vision_function_imaging_ti_2a_wrapper
 */
typedef struct {

    /* Sensor Specific Auto Exposure Dynamic Parameters */
    IssAeDynamicParams ae_dynPrms;

}sensor_config_get;

/*!
 * \brief Structure to set Exposure output to sensor
 * \ingroup group_vision_function_imaging_ti_2a_wrapper
 */
typedef struct {

    /* AutoExposure results data structure */
    IssSensor_ExposureParams aePrms;

}sensor_config_set;

/*!
 * \brief AE/AWB wrapper create function
 *
 * \param [in] obj Handle to TI_2A_wrapper structure
 *
 * \param [in] aewb_config Handle to tivx_aewb_config_t structure
 *
 * \param [in] dcc_buf Handle to dcc buffer
 *
 * \param [in] dcc_buf_size Size of dcc buffer
 *
 * \ingroup group_vision_function_imaging_ti_2a_wrapper
 */
int32_t TI_2A_wrapper_create(TI_2A_wrapper      *obj,
                             tivx_aewb_config_t *aewb_config,
                             uint8_t            *dcc_buf,
                             uint32_t            dcc_buf_size);

/*!
 * \brief AE/AWB wrapper process function
 *
 * \param [in|out] obj Handle to TI_2A_wrapper structure
 *
 * \param [in] aewb_config Handle to tivx_aewb_config_t structure
 *
 * \param [in] h3a_data Pointer to tivx_h3a_data_t which is output from VISS node
 *
 * \param [in] sensor_in_data Pointer to sensor_config_get which is input from sensor
 *
 * \param [out] ae_awb_result Pointer to tivx_ae_awb_params_t which is input to VISS node
 *
 * \param [out] sensor_out_data Pointer to sensor_config_set which is output to sensor
 *
 * \ingroup group_vision_function_imaging_ti_2a_wrapper
 */
int32_t TI_2A_wrapper_process(TI_2A_wrapper        *obj,
                              tivx_aewb_config_t   *aewb_config,
                              tivx_h3a_data_t      *h3a_data,
                              sensor_config_get    *sensor_in_data,
                              tivx_ae_awb_params_t *ae_awb_result,
                              sensor_config_set    *sensor_out_data);

/*!
 * \brief AE/AWB wrapper delete function.
 *
 * \param [in] obj Handle to TI_2A_wrapper structure
 *
 * \ingroup group_vision_function_imaging_ti_2a_wrapper
 */
int32_t TI_2A_wrapper_delete(TI_2A_wrapper *obj);

#ifdef __cplusplus
}
#endif

#endif /* _TI_2A_WRAPPER_H */

