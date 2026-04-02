/*
 *
 * Copyright (c) 2024 Texas Instruments Incorporated
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

#ifndef ISS_SENSORS_OV2312_H_
#define ISS_SENSORS_OV2312_H_

#include <iss_sensors.h>
#include <iss_sensor_priv.h>
#include <iss_sensor_if.h>
#include <iss_sensor_serdes.h>
#include "ov2312_linear_1600x1300_setup.h"

/**< ISS AEWB plugin is included here to get the default AEWB configuration
     from each sensor */

#define OV2312_OUT_WIDTH   (1600)
#define OV2312_OUT_HEIGHT  (1300)

/*******************************************************************************
 *  Defines
 *******************************************************************************
 */
#define OV2312_CHIP_ID_REG_ADDR               (0x300A)
#define OV2312_CHIP_ID_REG_VAL                (0x23)

#define ISS_SENSOR_OV2312_FEATURES      (ISS_SENSOR_FEATURE_MANUAL_EXPOSURE|   \
                                         ISS_SENSOR_FEATURE_MANUAL_GAIN|       \
                                         ISS_SENSOR_FEATURE_LINEAR_MODE|       \
                                         ISS_SENSOR_FEATURE_DCC_SUPPORTED)

#define ISS_SENSOR_OV2312_DEFAULT_EXP_RATIO     (128U)
#define OV2312_AEC_PK_EXPO_HI	0x3501U
#define OV2312_AEC_PK_EXPO_LO	0x3502U
#define OV2312_AEC_PK_AGAIN_HI	0x3508U
#define OV2312_AEC_PK_AGAIN_LO	0x3509U
#define OV2312_AEC_PK_DGAIN_HI	0x350AU
#define OV2312_AEC_PK_DGAIN_LO	0x350BU
#define OV2312_TIMING_VFLIP	0x3820U
#define OV2312_TIMING_HFLIP	0x3821U
/*******************************************************************************
 *  Local Functions Declarations
 *******************************************************************************
 */

static int32_t OV2312_Probe(uint32_t chId, void *pSensorHdl);
static int32_t OV2312_Config(uint32_t chId, void *pSensorHdl, uint32_t sensor_features_requested);
static int32_t OV2312_StreamOn(uint32_t chId, void *pSensorHdl);
static int32_t OV2312_StreamOff(uint32_t chId, void *pSensorHdl);
static int32_t OV2312_PowerOn(uint32_t chMask, void *pSensorHdl);
static int32_t OV2312_PowerOff(uint32_t chId, void *pSensorHdl);
static int32_t OV2312_GetExpParams(uint32_t chId, void *pSensorHdl, IssSensor_ExposureParams *pExpPrms);
static int32_t OV2312_SetAeParams(void *pSensorHdl, uint32_t chId, IssSensor_ExposureParams *pExpPrms);
static int32_t OV2312_GetDccParams(uint32_t chId, void *pSensorHdl, IssSensor_DccParams *pDccPrms);
static void OV2312_InitAewbConfig(uint32_t chId, void *pSensorHdl);
static void OV2312_GetIspConfig (uint32_t chId, void *pSensorHdl);
static void OV2312_deinit (uint32_t chId, void *pSensorHdl);
static int32_t OV2312_ReadWriteReg (uint32_t chId, void *pSensorHdl, uint32_t readWriteFlag, I2cParams *pReg);
static int32_t OV2312_GetExpPrgFxn(uint32_t chId, void *pSensorHdl, IssAeDynamicParams *p_ae_dynPrms);
static int32_t OV2312_SetAwbParams(void *pSensorHdl, uint32_t chId, IssSensor_WhiteBalanceParams *pWbPrms);
static int32_t OV2312_GetWBPrgFxn(uint32_t chId, void *pSensorHdl, IssAwbDynamicParams *p_awb_dynPrms);

static int32_t OV2312_WriteReg(uint8_t  i2cInstId,
                             uint8_t    i2cAddr,
                             uint16_t   regAddr,
                             uint8_t    regValue,
                             uint32_t   numRegs);

static int32_t OV2312_ReadReg(uint8_t   i2cInstId,
                            uint8_t     i2cAddr,
                            uint16_t    regAddr,
                            uint8_t     *regVal,
                            uint32_t    numRegs);

#endif /* End of ISS_SENSORS_OV2312_H_*/