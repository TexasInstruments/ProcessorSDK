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

#include "ti_2a_wrapper.h"
#include <math.h>

#include "AWB_config_hardware.h"
#include <itt_srvr_remote.h>

#include <app_remote_service.h>
#include <app_ipc.h>
#include <TI/hwa_viss_srvr_remote.h>


static int32_t AWB_TI_create(awbprm_t *p_awb_params, awb_calc_data_t* p_calib);
static int32_t AWB_TI_parse_H3a_buf(uint8_t * pH3a_out, awbprm_t *p_awb_params,
        h3a_aewb_paxel_data_t * awb_h3a_res, h3a_aewb_paxel_data_t * p_h3a_merge);
static int32_t AWB_TI_process(
        h3a_aewb_paxel_data_t * awb_h3a_res,
        awbprm_t              * p_awb_params,
        tivx_ae_awb_params_t  * aewb_prev,
        tivx_ae_awb_params_t  * aewb_result,
        uint8_t               * scratch_mem,
        uint8_t                 sensor_pre_gain);

static uint8_t * decode_h3a_header(uint8_t * h3a_buf, uint16_t *n_row,
                                    uint16_t *n_col, uint16_t * n_pix);

static uint8_t * decode_h3a_header_dcc (uint8_t * h3a_buf,
                                        iss_ipipe_h3a_aewb * h3a_dcc_cfg,
                                        uint16_t *n_row, uint16_t *n_col,
                                        uint16_t * n_pix);

static void map_sensor_ae_dynparams (IssAeDynamicParams * p_ae_dynPrms,
                                    tiae_exp_prog_t * p_ae_exp_prg);

static int32_t AE_TI_process (h3a_aewb_paxel_data_t *awb_h3a_res, int h3a_data_x,
                                int h3a_data_y, tiae_prm_t *h,
                                tivx_ae_awb_params_t *aewb_prev,
                                tivx_ae_awb_params_t *aewb_result);

extern AlgItt_IssAewb2AControlParams aewbCtrlPrms[ISS_SENSORS_MAX_SUPPORTED_SENSOR];
extern tivx_mutex                 itt_aewb_lock[ISS_SENSORS_MAX_SUPPORTED_SENSOR];

int32_t TI_2A_wrapper_create(TI_2A_wrapper      *obj,
                             tivx_aewb_config_t *aewb_config,
                             uint8_t            *dcc_buf,
                             uint32_t            dcc_buf_size)
{
    int32_t status = VX_SUCCESS;
    int32_t dcc_status = VX_SUCCESS;

    int32_t awbInitStatus;
    awb_calc_data_t * awb_calib;
    vx_bool ae_disable = vx_false_e;
    vx_bool awb_disable = vx_false_e;

    if(NULL == obj)
    {
        VX_PRINT(VX_ZONE_ERROR, "TI_2A_wrapper object is NULL \n");
        return VX_FAILURE;
    }

    if(NULL != aewb_config)
    {
        VX_PRINT(VX_ZONE_INFO, "DCC ID = %d \n", aewb_config->sensor_dcc_id);
        VX_PRINT(VX_ZONE_INFO, "Image Format = %d \n", aewb_config->sensor_img_format);
        VX_PRINT(VX_ZONE_INFO, "Phase = %d \n", aewb_config->sensor_img_phase);
        VX_PRINT(VX_ZONE_INFO, "awb_mode = %d \n", aewb_config->awb_mode);
        VX_PRINT(VX_ZONE_INFO, "ae_mode = %d \n", aewb_config->ae_mode);
        VX_PRINT(VX_ZONE_INFO, "awb_num_skip_frames = %d \n", aewb_config->awb_num_skip_frames);
        VX_PRINT(VX_ZONE_INFO, "ae_num_skip_frames = %d \n", aewb_config->ae_num_skip_frames);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "AEWB config is NULL \n");
        return VX_FAILURE;
    }

    obj->dcc_input_params = (dcc_parser_input_params_t *)tivxMemAlloc(sizeof(dcc_parser_input_params_t), TIVX_MEM_EXTERNAL);
    if(!obj->dcc_input_params)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate memory for dcc_input_params \n");
        return VX_ERROR_NO_MEMORY;
    }

    obj->dcc_output_params = (dcc_parser_output_params_t *)tivxMemAlloc(sizeof(dcc_parser_output_params_t), TIVX_MEM_EXTERNAL);
    if(!obj->dcc_output_params)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate memory for dcc_output_params \n");
        return VX_ERROR_NO_MEMORY;
    }

    if(NULL != dcc_buf)
    {

        obj->dcc_input_params->dcc_buf = dcc_buf;
        obj->dcc_input_params->dcc_buf_size = dcc_buf_size;
        obj->dcc_input_params->analog_gain = 1000;
        obj->dcc_input_params->cameraId = aewb_config->sensor_dcc_id;
        obj->dcc_input_params->color_temparature = 5000;
        obj->dcc_input_params->exposure_time = 33333;

        VX_PRINT(VX_ZONE_INFO, "Before dcc_update : awb_calb_data.radius = 0x%x \n", obj->dcc_output_params->awbCalbData.radius);

        dcc_status = Dcc_Create(obj->dcc_output_params, NULL);
        if(status != 0)
        {
            VX_PRINT(VX_ZONE_ERROR, "Dcc_Create returned %d \n", dcc_status);
        }
        else
        {
            dcc_status = dcc_update(obj->dcc_input_params, obj->dcc_output_params);
        }
        if(dcc_status != 0)
        {
            VX_PRINT(VX_ZONE_ERROR, "dcc_update returned %d \n", dcc_status);
        }

        VX_PRINT(VX_ZONE_INFO, "After dcc_update : awb_calb_data.radius = 0x%x \n", obj->dcc_output_params->awbCalbData.radius);
    }
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "DCC buffer is NULL \n");
        ae_disable = vx_true_e;
        awb_disable = vx_true_e;
    }

    /* Initialize frame count to 0 */
    obj->frame_count = 0;
    /* Initializing value to zero here but get this by querying the sensor */
    obj->sensor_pre_wb_gain = 0;

    obj->p_awb_params = tivxMemAlloc(sizeof(awbprm_t), TIVX_MEM_EXTERNAL);
    if(NULL == obj->p_awb_params)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate AWB Param\n");
        return VX_ERROR_NO_MEMORY;
    }
    else
    {
        if(awb_disable == vx_true_e)
        {
            obj->p_awb_params->mode = ALGORITHMS_ISS_AWB_DISABLED;
        }
        else
        {
            obj->p_awb_params->mode = aewb_config->awb_mode;
        }
    }

    obj->p_ae_params = tivxMemAlloc(sizeof(tiae_prm_t), TIVX_MEM_EXTERNAL);
    if(NULL == obj->p_ae_params)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate AE Param\n");
        return VX_ERROR_NO_MEMORY;
    }
    else
    {
        if(ae_disable == vx_true_e)
        {
            obj->p_ae_params->mode = ALGORITHMS_ISS_AE_DISABLED;
        }
        else
        {
            obj->p_ae_params->mode = aewb_config->ae_mode;
        }
    }

    obj->p_awb_params->sen_awb_calc_data =
        tivxMemAlloc(sizeof(awb_calc_data_t), TIVX_MEM_EXTERNAL);
    if(NULL == obj->p_awb_params->sen_awb_calc_data)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate AWB sensor calibration\n");
        return VX_ERROR_NO_MEMORY;
    }

    obj->p_h3a_merge =
    tivxMemAlloc(H3A_MAX_WINH*H3A_MAX_WINV*sizeof(h3a_aewb_paxel_data_t),
                    TIVX_MEM_EXTERNAL);
    if(NULL == obj->p_h3a_merge)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate AWB H3A merge data\n");
        return VX_ERROR_NO_MEMORY;
    }
    memset(obj->p_h3a_merge, 0x0,
            H3A_MAX_WINH*H3A_MAX_WINV*sizeof(h3a_aewb_paxel_data_t));

    obj->awb_h3a_res =
    tivxMemAlloc(H3A_MAX_WINH*H3A_MAX_WINV*sizeof(h3a_aewb_paxel_data_t),
                    TIVX_MEM_EXTERNAL);
    if(NULL == obj->awb_h3a_res)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate AWB H3A result data\n");
        return VX_ERROR_NO_MEMORY;
    }

    if (0 == dcc_status)
    {
        awb_calib = &(obj->dcc_output_params->awbCalbData);
        VX_PRINT(VX_ZONE_INFO, "DCC succeeded \n");
    }
    else
    {
        awb_calib = &ts1_5_awb_calc_data;
        VX_PRINT(VX_ZONE_INFO, "DCC failed. Using default AWB calibration \n");
    }

    TI_AE_init(obj->p_ae_params, NULL);

    awbInitStatus = AWB_TI_create(obj->p_awb_params, awb_calib);
    if(0 == awbInitStatus)
    {
        VX_PRINT(VX_ZONE_INFO, "AWB Initialization successful \n");
    }
    else
    {
        status = VX_ERROR_INVALID_PARAMETERS;
        printf("AWB Initialization returned %d \n", awbInitStatus);
    }

    obj->scratch_memory =
        tivxMemAlloc(sizeof(uint8_t) * AWB_SCRATCH_MEM_SIZE, TIVX_MEM_EXTERNAL);

    if (NULL == obj->scratch_memory)
    {
        VX_PRINT(VX_ZONE_ERROR, "Failed to allocate AWB scratch buffer \n");
        return VX_ERROR_NO_MEMORY;
    }
    else
    {
        VX_PRINT(VX_ZONE_INFO, "AWB_ScratchMemory size = %d\n",
                    AWB_SCRATCH_MEM_SIZE);
    }

    return status;
}

int32_t TI_2A_wrapper_process(TI_2A_wrapper        *obj,
                              tivx_aewb_config_t   *aewb_config,
                              tivx_h3a_data_t      *h3a_data,
                              sensor_config_get    *sensor_in_data,
                              tivx_ae_awb_params_t *ae_awb_result,
                              sensor_config_set    *sensor_out_data)
{
    int32_t status = 0;
    obj->skipAE = obj->skipAWB = 0;

    tivx_ae_awb_params_t *ae_awb_result_prev;
    ae_awb_result_prev = &(obj->ae_awb_result_prev);

    uint8_t rIndex  = (uint8_t)obj->p_awb_params->sen_awb_calc_data->red_index;
    uint8_t grIndex = (uint8_t)obj->p_awb_params->sen_awb_calc_data->green1_index;
    uint8_t gbIndex = (uint8_t)obj->p_awb_params->sen_awb_calc_data->green2_index;
    uint8_t bIndex  = (uint8_t)obj->p_awb_params->sen_awb_calc_data->blue_index;

    tivxMutexLock(itt_aewb_lock[aewb_config->channel_id]);
    obj->p_awb_params->mode = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.awbMode;
    obj->p_ae_params->mode = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.aeMode;

    if(ALGORITHMS_ISS_AWB_AUTO == obj->p_awb_params->mode)
    {
        if(0 != ((obj->frame_count + aewb_config->channel_id) % (aewb_config->awb_num_skip_frames+1)))
        {
            obj->skipAWB = 1;
            ae_awb_result->awb_valid = 0;

            ae_awb_result->wb_gains[0]       = ae_awb_result_prev->wb_gains[0];
            ae_awb_result->wb_gains[1]       = ae_awb_result_prev->wb_gains[1];
            ae_awb_result->wb_gains[2]       = ae_awb_result_prev->wb_gains[2];
            ae_awb_result->wb_gains[3]       = ae_awb_result_prev->wb_gains[3];
            ae_awb_result->wb_offsets[0]     = ae_awb_result_prev->wb_offsets[0];
            ae_awb_result->wb_offsets[1]     = ae_awb_result_prev->wb_offsets[1];
            ae_awb_result->wb_offsets[2]     = ae_awb_result_prev->wb_offsets[2];
            ae_awb_result->wb_offsets[3]     = ae_awb_result_prev->wb_offsets[3];
            ae_awb_result->color_temperature = ae_awb_result_prev->color_temperature;
        }
    }
    else if(ALGORITHMS_ISS_AWB_MANUAL == obj->p_awb_params->mode)
    {
        ae_awb_result->wb_gains[rIndex] = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.rGain;
        ae_awb_result->wb_gains[grIndex] = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.gGain;
        ae_awb_result->wb_gains[gbIndex] = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.gGain;
        ae_awb_result->wb_gains[bIndex] = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.bGain;
        ae_awb_result->color_temperature = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.colorTemp;
        ae_awb_result->awb_valid = 1;
    }

    if(ALGORITHMS_ISS_AE_AUTO == obj->p_ae_params->mode)
    {
        if(0 != ((obj->frame_count + aewb_config->channel_id) %
                    (aewb_config->ae_num_skip_frames+1)))
        {
            obj->skipAE = 1;
            ae_awb_result->ae_valid = 0;
            ae_awb_result->analog_gain       = ae_awb_result_prev->analog_gain;
            ae_awb_result->digital_gain      = ae_awb_result_prev->digital_gain;
            ae_awb_result->exposure_time     = ae_awb_result_prev->exposure_time;
            ae_awb_result->ae_converged      = ae_awb_result_prev->ae_converged;
        }
    }
    else if(ALGORITHMS_ISS_AE_MANUAL == obj->p_ae_params->mode)
    {
        int max_n = obj->p_ae_params->exposure_program.num_ranges - 1;
        int32_t max_analog_gain = obj->p_ae_params->exposure_program.analog_gain_range[max_n].max;
        int32_t min_analog_gain = obj->p_ae_params->exposure_program.analog_gain_range[0].min;
        int32_t max_exp_time = obj->p_ae_params->exposure_program.exposure_time_range[max_n].max;
        int32_t min_exp_time = obj->p_ae_params->exposure_program.exposure_time_range[0].min;
        int32_t max_digital_gain = obj->p_ae_params->exposure_program.digital_gain_range[max_n].max;
        int32_t min_digital_gain = obj->p_ae_params->exposure_program.digital_gain_range[0].min;

        if(aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.analogGain > max_analog_gain)
        {
            aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.analogGain = max_analog_gain;
        }else if(aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.analogGain < min_analog_gain)
        {
            aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.analogGain = min_analog_gain;
        }

        if(aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.expTime > max_exp_time)
        {
            aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.expTime = max_exp_time;
        }else if(aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.expTime < min_exp_time)
        {
            aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.expTime = min_exp_time;
        }

        if(aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.digitalGain > max_digital_gain)
        {
            aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.digitalGain = max_digital_gain;
        }else if(aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.digitalGain < min_digital_gain)
        {
            aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.digitalGain = min_digital_gain;
        }

        ae_awb_result->analog_gain   = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.analogGain;
        ae_awb_result->exposure_time = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.expTime;
        ae_awb_result->digital_gain = aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.digitalGain;
    }

    obj->frame_count++;

    if((1==obj->skipAE) && (1==obj->skipAWB))
    {
        return 0;
    }

    /* kernel processing function start */
    if(NULL != h3a_data)
    {
        /*
        H3A config should be read from DCC
        Header sent by VISS node is read but AWB params are overwritten by DCC values
        */
        uint8_t *h3a_buf = NULL;
        if(NULL == obj->dcc_output_params)
        {
            h3a_buf = decode_h3a_header((uint8_t *)h3a_data->data,
                &obj->p_awb_params->ti_awb_data_in.frame_data.h3a_data_y,
                &obj->p_awb_params->ti_awb_data_in.frame_data.h3a_data_x,
                &obj->p_awb_params->ti_awb_data_in.frame_data.pix_in_pax);
        }
        else
        {
            h3a_buf = decode_h3a_header_dcc((uint8_t *)h3a_data->data,
                            &obj->dcc_output_params->ipipeH3A_AEWBCfg,
                &obj->p_awb_params->ti_awb_data_in.frame_data.h3a_data_y,
                &obj->p_awb_params->ti_awb_data_in.frame_data.h3a_data_x,
                &obj->p_awb_params->ti_awb_data_in.frame_data.pix_in_pax);
        }

        AWB_TI_parse_H3a_buf(h3a_buf, obj->p_awb_params,
                    obj->awb_h3a_res, obj->p_h3a_merge);

        ae_awb_result->h3a_source_data = 0;

        if(1==obj->skipAE)
        {
            ae_awb_result->ae_valid = 0;
        }
        else
        {
            if(ALGORITHMS_ISS_AE_AUTO == obj->p_ae_params->mode)
            {
                if(0 == obj->p_ae_params->exposure_program.num_ranges)
                {
                    /*AE Exposure program not initialized. get it from sensor
                    driver and re-initialize AE*/
                    tiae_exp_prog_t ae_exp_prg;

                    map_sensor_ae_dynparams(&sensor_in_data->ae_dynPrms, &ae_exp_prg);
                    TI_AE_init(obj->p_ae_params, &ae_exp_prg);
                }
                status = AE_TI_process(obj->awb_h3a_res,
                    obj->p_awb_params->ti_awb_data_in.frame_data.h3a_data_x,
                    obj->p_awb_params->ti_awb_data_in.frame_data.h3a_data_y,
                    obj->p_ae_params,
                    ae_awb_result_prev,
                    ae_awb_result);

                if(0 != status)
                {
                    VX_PRINT(VX_ZONE_ERROR, "AE_TI_process returned error 0x%x \n",
                                status);
                    ae_awb_result->ae_valid = 0;
                }
            }

            if(ALGORITHMS_ISS_AE_DISABLED != obj->p_ae_params->mode)
            {
    #ifndef x86_64
                IssSensor_ExposureParams aePrms;
                if(1 == ae_awb_result->ae_valid)
                {
                    aePrms.chId = 0;
                    aePrms.analogGain[ISS_SENSOR_EXPOSURE_LONG] =
                        ae_awb_result->analog_gain;
                    aePrms.analogGain[ISS_SENSOR_EXPOSURE_SHORT] =
                        ae_awb_result->analog_gain;
                    aePrms.analogGain[ISS_SENSOR_EXPOSURE_VSHORT] =
                        ae_awb_result->analog_gain;
                    aePrms.exposureTime[ISS_SENSOR_EXPOSURE_LONG] =
                        ae_awb_result->exposure_time;
                    aePrms.exposureTime[ISS_SENSOR_EXPOSURE_SHORT] =
                        ae_awb_result->exposure_time;
                    aePrms.exposureTime[ISS_SENSOR_EXPOSURE_VSHORT] =
                        ae_awb_result->exposure_time;
                    aePrms.expRatio = 1U;/*Do not care*/

                    sensor_out_data->aePrms = aePrms;
                }
    #endif /*x86_64*/
            }
        }

        if(1==obj->skipAWB)
        {
            ae_awb_result->awb_valid = 0;
        }
        else
        {
            if(ALGORITHMS_ISS_AWB_AUTO == obj->p_awb_params->mode)
            {
                /* Is this memset required every frame? */
                memset(obj->scratch_memory, 0xAB, AWB_SCRATCH_MEM_SIZE);

                status = AWB_TI_process(obj->p_h3a_merge,
                                        obj->p_awb_params,
                                        ae_awb_result_prev,
                                        ae_awb_result,
                                        obj->scratch_memory,
                                        obj->sensor_pre_wb_gain);

                if(0 != status)
                {
                    VX_PRINT(VX_ZONE_ERROR, "AWB_TI_process returned error 0x%x \n",
                                status);
                    ae_awb_result->awb_valid = 0;
                }

                if (NULL != obj->dcc_output_params)
                {
                    if (100 == obj->dcc_output_params->awbCalbData.apply_rgb_adjust)
                    {
                        double power = obj->dcc_output_params->awbCalbData.R_adjust / 100.0;
                        double g0 = ae_awb_result->wb_gains[0];
                        double g1 = ae_awb_result->wb_gains[1];
                        double g2 = ae_awb_result->wb_gains[2];
                        double g3 = ae_awb_result->wb_gains[3];
                        ae_awb_result->wb_gains[0] = pow(g0 / 512.0, power) * 512 + 0.5;
                        ae_awb_result->wb_gains[1] = pow(g1 / 512.0, power) * 512 + 0.5;
                        ae_awb_result->wb_gains[2] = pow(g2 / 512.0, power) * 512 + 0.5;
                        ae_awb_result->wb_gains[3] = pow(g3 / 512.0, power) * 512 + 0.5;
                    }
                }

                VX_PRINT(VX_ZONE_INFO, "AWB Gain = (%d, %d, %d, %d) \n",
                        ae_awb_result->wb_gains[0],
                        ae_awb_result->wb_gains[1],
                        ae_awb_result->wb_gains[2],
                        ae_awb_result->wb_gains[3]);
            }
        }
    }

    /* kernel processing function complete */

    if(1 == ae_awb_result->ae_valid)
    {
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.digitalGain = ae_awb_result->digital_gain;
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.analogGain = ae_awb_result->analog_gain;
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.expTime = ae_awb_result->exposure_time;
    }
    if(1 == ae_awb_result->awb_valid)
    {
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.rGain = ae_awb_result->wb_gains[rIndex];
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.gGain =
            (ae_awb_result->wb_gains[grIndex] + ae_awb_result->wb_gains[gbIndex])/2;
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.bGain = ae_awb_result->wb_gains[bIndex];
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.colorTemp = ae_awb_result->color_temperature;
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.awb_valid = ae_awb_result->awb_valid;
        aewbCtrlPrms[aewb_config->channel_id].aewb2APrms.awb_converged = ae_awb_result->awb_converged;
    }

    tivxMutexUnlock(itt_aewb_lock[aewb_config->channel_id]);

    return status;
}

int32_t TI_2A_wrapper_delete(TI_2A_wrapper *obj)
{
    int32_t status = 0;

    if(NULL != obj->dcc_input_params)
    {
        tivxMemFree(obj->dcc_input_params, sizeof(dcc_parser_input_params_t), TIVX_MEM_EXTERNAL);
    }
    if(NULL != obj->dcc_output_params)
    {
        tivxMemFree(obj->dcc_output_params, sizeof(dcc_parser_output_params_t), TIVX_MEM_EXTERNAL);
    }
    if(NULL != obj->p_h3a_merge)
    {
        tivxMemFree(obj->p_h3a_merge, H3A_MAX_WINH*H3A_MAX_WINV*sizeof(h3a_aewb_paxel_data_t), TIVX_MEM_EXTERNAL);
    }
    if(NULL != obj->p_ae_params)
    {
        tivxMemFree(obj->p_ae_params, sizeof(tiae_prm_t), TIVX_MEM_EXTERNAL);
    }
    if(NULL != obj->p_awb_params)
    {
        if(NULL != obj->p_awb_params->sen_awb_calc_data)
        {
            tivxMemFree(obj->p_awb_params->sen_awb_calc_data, sizeof(awb_calc_data_t), TIVX_MEM_EXTERNAL);
        }
        tivxMemFree(obj->p_awb_params, sizeof(awbprm_t), TIVX_MEM_EXTERNAL);
    }
    if(NULL != obj->awb_h3a_res)
    {
        tivxMemFree(obj->awb_h3a_res, H3A_MAX_WINH*H3A_MAX_WINV*sizeof(h3a_aewb_paxel_data_t), TIVX_MEM_EXTERNAL);
    }
    if(NULL != obj->scratch_memory)
    {
        tivxMemFree(obj->scratch_memory, sizeof(uint8_t) * AWB_SCRATCH_MEM_SIZE, TIVX_MEM_EXTERNAL);
    }
    return status;
}

static uint8_t * decode_h3a_header(uint8_t * h3a_buf, uint16_t *n_row,
                                    uint16_t *n_col, uint16_t * n_pix)
{
    // the header is added by H3A c model
    uint16_t * header = (uint16_t*) h3a_buf;
    *n_row = header[2];
    *n_col = header[3];
    uint16_t winh = (header[1] + header[5] - 1) / header[5];
    uint16_t winv = (header[0] + header[4] - 1) / header[4];
    *n_pix = winh * winv;
    VX_PRINT(VX_ZONE_INFO, "H3A config: winH=%d, winW=%d, winVC=%d, winHC=%d, incV=%d, incH=%d\n",
            header[0], header[1], header[2], header[3], header[4], header[5]);

    return &h3a_buf[12];
}

static uint8_t * decode_h3a_header_dcc (uint8_t * h3a_buf,
                                        iss_ipipe_h3a_aewb * h3a_dcc_cfg,
                                        uint16_t *n_row, uint16_t *n_col,
                                        uint16_t * n_pix)
{
    uint16_t winh;
    uint16_t winv;

    if(0 == h3a_dcc_cfg->h_skip)
    {
        printf("Warning : h_skip = 0. Setting to 1 to avoid divide by zero error \n");
        h3a_dcc_cfg->h_skip = 1;
    }

    if(0 == h3a_dcc_cfg->v_skip)
    {
        printf("Warning : v_skip = 0. Setting to 1 to avoid divide by zero error \n");
        h3a_dcc_cfg->v_skip = 1;
    }

    winh = (h3a_dcc_cfg->h_size + h3a_dcc_cfg->h_skip - 1)/h3a_dcc_cfg->h_skip;
    winv = (h3a_dcc_cfg->v_size + h3a_dcc_cfg->v_skip - 1)/h3a_dcc_cfg->v_skip;

    *n_row = h3a_dcc_cfg->v_count;
    *n_col = h3a_dcc_cfg->h_count;
    *n_pix = winh * winv;
    VX_PRINT(VX_ZONE_INFO, "decode_h3a_header_dcc : num_rows = %d\n", *n_row);
    VX_PRINT(VX_ZONE_INFO, "decode_h3a_header_dcc : num_cols = %d\n", *n_col);
    VX_PRINT(VX_ZONE_INFO, "decode_h3a_header_dcc : num_pix = %d\n", *n_pix);

    return h3a_buf;
}

static void map_sensor_ae_dynparams (IssAeDynamicParams * p_ae_dynPrms,
                                    tiae_exp_prog_t * p_ae_exp_prg)
{
   uint8_t count;
   uint8_t num_dyn_params = p_ae_dynPrms->numAeDynParams;

   p_ae_exp_prg->num_ranges = num_dyn_params;
   p_ae_exp_prg->target_brightness = p_ae_dynPrms->targetBrightness;
   p_ae_exp_prg->target_brightness_range.min =
        p_ae_dynPrms->targetBrightnessRange.min;
   p_ae_exp_prg->target_brightness_range.max =
        p_ae_dynPrms->targetBrightnessRange.max;
   p_ae_exp_prg->exposure_time_step_size = p_ae_dynPrms->exposureTimeStepSize;
   p_ae_exp_prg->enableBLC = p_ae_dynPrms->enableBlc;

   for(count = 0; count < num_dyn_params; count++)
   {
       p_ae_exp_prg->exposure_time_range[count].min =
            p_ae_dynPrms->exposureTimeRange[count].min;
       p_ae_exp_prg->exposure_time_range[count].max =
            p_ae_dynPrms->exposureTimeRange[count].max;

       p_ae_exp_prg->analog_gain_range[count].min =
            p_ae_dynPrms->analogGainRange[count].min;
       p_ae_exp_prg->analog_gain_range[count].max =
            p_ae_dynPrms->analogGainRange[count].max;

       p_ae_exp_prg->digital_gain_range[count].min =
            p_ae_dynPrms->digitalGainRange[count].min;
       p_ae_exp_prg->digital_gain_range[count].max =
            p_ae_dynPrms->digitalGainRange[count].max;

       p_ae_exp_prg->aperture_size_range[count].min = 1;
       p_ae_exp_prg->aperture_size_range[count].max = 1;
   }
}

static int32_t AE_TI_process (h3a_aewb_paxel_data_t *awb_h3a_res, int h3a_data_x,
                                int h3a_data_y, tiae_prm_t *h,
                                tivx_ae_awb_params_t *aewb_prev,
                                tivx_ae_awb_params_t *aewb_result)
{
    if(!awb_h3a_res)
    {
        VX_PRINT(VX_ZONE_ERROR, "awb_h3a_res is NULL");
        return -1;
    }
    if(!(aewb_prev))
    {
        VX_PRINT(VX_ZONE_ERROR, "aewb_prev is NULL");
        return -1;
    }
    if(!aewb_result)
    {
        VX_PRINT(VX_ZONE_ERROR, "aewb_result is NULL");
        return -1;
    }

    //tiae_exp_t exp1 = h->prev_ae;
    tiae_exp_t exp2;
    aewb_result->ae_converged = TI_AE_do(h, awb_h3a_res, h3a_data_x, h3a_data_y,
                                            NULL, 256, 256, 256, &exp2);

    //float adj = (float)exp2.aperture_size / exp1.aperture_size
    //            * exp2.exposure_time  / exp1.exposure_time
    //            * exp2.analog_gain    / exp1.analog_gain
    //            * exp2.digital_gain   / exp1.digital_gain;
    //printf(">>> AE debug kernel: adj = %4f, locked = %d, lock_cnt = %d\n", adj, h->locked, h->lock_cnt);
    //printf(">>> AE debug kernel: T1 = %d, T2 = %d\n", exp1.exposure_time, exp2.exposure_time);

    aewb_result->analog_gain   = exp2.analog_gain;
    aewb_result->digital_gain  = exp2.digital_gain;
    aewb_result->exposure_time = exp2.exposure_time;
    aewb_result->ae_valid = 1;

    return 0;
}

static void parse_h3a_out(uint8_t h3a_buf[], int32_t n_col, int32_t n_row,
                          int32_t id_r, int32_t id_gr, int32_t id_gb, int32_t id_b,
                          h3a_aewb_paxel_data_t *h3a_data)
{
    uint8_t * cur_addr = h3a_buf;
    int n_win = 0;
    int j, i;

    for (j = 0; j < n_row; j++)
    {
        for (i = 0; i < n_col; i++)
        {
            uint16_t * pAewbWinData = (uint16_t *)cur_addr;

            uint32_t sum_gr = pAewbWinData[id_gr];
            uint32_t sum_gb = pAewbWinData[id_gb];
            uint16_t sum_g  = (uint16_t)((sum_gr + sum_gb + 1) >> 1);

            h3a_data[j * n_col + i].green = sum_g;
            h3a_data[j * n_col + i].red   = pAewbWinData[id_r];
            h3a_data[j * n_col + i].blue  = pAewbWinData[id_b];

            cur_addr += sizeof(uint16_t) * 8;
            n_win++;

            if (n_win % 8 == 0)
            {
                cur_addr += sizeof(uint16_t) * 8;
            }
        }

        if ((cur_addr - h3a_buf) % 32 == 16)
        {
            cur_addr += 16;
        }
    }
}

static void h3a_merge(
    h3a_aewb_paxel_data_t *p_in,
    int32_t sz_h,
    int32_t sz_v,
    int32_t pix_in_pax,
    int32_t T_low,
    int32_t T_high,
    h3a_aewb_paxel_data_t *p_out)
{
    int32_t th1 = pix_in_pax * T_low;
    int32_t th2 = pix_in_pax * T_high;
    int k;
    for (k = 0; k < sz_v * sz_h; k++)
    {
        if (p_in[k].green >= th1 && p_in[k].green <= th2)
        {
            p_out[k] = p_in[k];
        }
        else
        {
            p_out[k].red   = (p_out[k].red   * 7) >> 3;
            p_out[k].green = (p_out[k].green * 7) >> 3;
            p_out[k].blue  = (p_out[k].blue  * 7) >> 3;
        }
    }
}

static void h3a_normalize(h3a_aewb_paxel_data_t p_h3a[],
                          int32_t sz_h, int32_t sz_v, int32_t pix_in_pax)
{
    int k;
    for (k = 0; k < sz_v * sz_h; k++)
    {
        int32_t r = p_h3a[k].red;
        int32_t g = p_h3a[k].green;
        int32_t b = p_h3a[k].blue;
        p_h3a[k].red   = (uint16_t)((r + (pix_in_pax >> 1)) / pix_in_pax);
        p_h3a[k].green = (uint16_t)((g + (pix_in_pax >> 1)) / pix_in_pax);
        p_h3a[k].blue  = (uint16_t)((b + (pix_in_pax >> 1)) / pix_in_pax);
    }
}


static int32_t AWB_TI_parse_H3a_buf(uint8_t * pH3a_out, awbprm_t * p_awb_params,
        h3a_aewb_paxel_data_t * awb_h3a_res, h3a_aewb_paxel_data_t * p_h3a_merge)
{
    int32_t h3a_n_col = p_awb_params->ti_awb_data_in.frame_data.h3a_data_x;
    int32_t h3a_n_row = p_awb_params->ti_awb_data_in.frame_data.h3a_data_y;
    int32_t h3a_n_pix = p_awb_params->ti_awb_data_in.frame_data.pix_in_pax;
    int32_t rIndex    = p_awb_params->sen_awb_calc_data->red_index;
    int32_t grIndex   = p_awb_params->sen_awb_calc_data->green1_index;
    int32_t gbIndex   = p_awb_params->sen_awb_calc_data->green2_index;
    int32_t bIndex    = p_awb_params->sen_awb_calc_data->blue_index;
    VX_PRINT(VX_ZONE_INFO, "H3A color: r=%d, gr=%d, gb=%d, b=%d \n", rIndex, grIndex, gbIndex, bIndex);

    parse_h3a_out(pH3a_out, h3a_n_col, h3a_n_row, rIndex, grIndex, gbIndex, bIndex, awb_h3a_res);

    // "0, 1023" below: no merge for 10-bit H3A without H3A switching
    h3a_merge(awb_h3a_res, h3a_n_col, h3a_n_row, h3a_n_pix, 0, 1023, p_h3a_merge);

    h3a_normalize(awb_h3a_res, h3a_n_col, h3a_n_row, h3a_n_pix);

    return 0;
}

static int32_t AWB_TI_create(awbprm_t *p_awb_params, awb_calc_data_t* p_calib)
{
    int32_t status = 0;
    TI_AWB_ERROR awbInitStatus;

    if((NULL == p_calib) || (NULL == p_awb_params))
    {
        printf("AWB_TI_create Error : Invalid parameters \n");
        status = -10;
    }
    else
    {
        p_awb_params->init_done = 0;
        p_awb_params->mode = AWB_WB_MODE_AUTO;
        p_awb_params->AWB_ScratchMemory = NULL;
        p_awb_params->manl_tmpr = 0;
        p_awb_params->sb_total_exp = 999999;

        *p_awb_params->sen_awb_calc_data = *p_calib;

        awbInitStatus = TI_AWB_init(p_awb_params);
        if(TI_AWB_ERROR_OK == awbInitStatus)
        {
            status = 0;
        }
        else
        {
            printf("ERROR : AWB Initialization returned %d \n", awbInitStatus);
            status = -10;
        }
    }

    return status;
}

static int32_t AWB_TI_process(
        h3a_aewb_paxel_data_t * awb_h3a_res,
        awbprm_t              * p_awb_params,
        tivx_ae_awb_params_t  * aewb_prev,
        tivx_ae_awb_params_t  * aewb_result,
        uint8_t               * scratch_mem,
        uint8_t                 sensor_pre_gain)
{
    p_awb_params->ti_awb_data_in.frame_data.h3a_res = awb_h3a_res;
    p_awb_params->AWB_ScratchMemory = scratch_mem;
    p_awb_params->ti_awb_data_in.is_face.faces = NULL;      // Not supported
    int32_t status = -1;

    uint8_t rIndex  = (uint8_t)p_awb_params->sen_awb_calc_data->red_index;
    uint8_t grIndex = (uint8_t)p_awb_params->sen_awb_calc_data->green1_index;
    uint8_t gbIndex = (uint8_t)p_awb_params->sen_awb_calc_data->green2_index;
    uint8_t bIndex  = (uint8_t)p_awb_params->sen_awb_calc_data->blue_index;

    if (1==sensor_pre_gain)
    {
        uint16_t paxelIndex = 0;
        h3a_aewb_paxel_data_t h3a_paxel_data;
        awb_frame_data_t    * pFrame_data = &(p_awb_params->ti_awb_data_in.frame_data);
        int n_pax = pFrame_data->h3a_data_x * pFrame_data->h3a_data_y;
        uint32_t rGain_prev, bGain_prev, gGain_prev;

        rGain_prev = aewb_prev->wb_gains[rIndex];
        gGain_prev = (aewb_prev->wb_gains[grIndex]+aewb_prev->wb_gains[gbIndex])/2;
        bGain_prev = aewb_prev->wb_gains[bIndex];
        for (paxelIndex = 0; paxelIndex < n_pax; paxelIndex++)
        {
            h3a_paxel_data = pFrame_data->h3a_res[paxelIndex];
            h3a_paxel_data.red   /= rGain_prev;
            h3a_paxel_data.green /= gGain_prev;
            h3a_paxel_data.blue  /= bGain_prev;
        }
    }

    awb_data_out_t  awb_data_out;
    TI_AWB_ERROR awbStatus = TI_AWB_ERROR_OK;
    awbStatus = TI_AWB_do(p_awb_params, &awb_data_out);
    if(TI_AWB_ERROR_OK == awbStatus)
    {
        TI_AWB_stab(p_awb_params, &awb_data_out);

        aewb_result->wb_gains[rIndex] = awb_data_out.gain_R << 1;
        aewb_result->wb_gains[grIndex] = awb_data_out.gain_Gr << 1;
        aewb_result->wb_gains[gbIndex] = awb_data_out.gain_Gb << 1;
        aewb_result->wb_gains[bIndex] = awb_data_out.gain_B << 1;

        aewb_result->wb_offsets[rIndex] = 0;
        aewb_result->wb_offsets[grIndex] = 0;
        aewb_result->wb_offsets[gbIndex] = 0;
        aewb_result->wb_offsets[bIndex] = 0;

        aewb_result->color_temperature = awb_data_out.color_temperature_estim;
        aewb_result->awb_valid = 1;
        status = 0;
    }
    else
    {
        aewb_result->awb_valid = 0;
        VX_PRINT(VX_ZONE_ERROR, "TI_AWB_do Returned error %d \n", awbStatus);
        status = -1;
    }

    p_awb_params->AWB_ScratchMemory = NULL;
    return status;
}

