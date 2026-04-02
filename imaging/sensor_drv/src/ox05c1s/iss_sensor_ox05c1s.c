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
#include "iss_sensor_ox05c1s.h"
#include "ox05c1s_serdes_config.h"

static IssSensor_CreateParams  ox05c1sCreatePrms = {

    //TODO: must define this macro in "iss_sensors.h" for integrating the new sensor driver
    SENSOR_X5C_UB971_OV,             /* sensor name */
    0x6,                                /* i2cInstId: DON'T CARE */
    I2C_ADDR_SENSOR,                    /* i2cAddrSensor */
    I2C_ADDR_SER,                       /* i2cAddrSer */
    {                                   /* sensorInfo */
        {                               /* raw_params */
            OX05C1S_OUT_WIDTH,          /* width */
            OX05C1S_OUT_HEIGHT,         /* height */
            1,                          /* num_exposures */
            vx_false_e,                 /* line_interleaved */
            {                           /* tivx_raw_image_format_t */
                {TIVX_RAW_IMAGE_16_BIT, 9},    /*dataFormat and MSB */
            },
            OX05C1S_META_HEIGHT_BEFORE,  /* meta_height_before */
            OX05C1S_META_HEIGHT_AFTER,  /* meta_height_after */
        },
        ISS_SENSOR_OX05C1S_FEATURES,    /* features */
        ALGORITHMS_ISS_AEWB_MODE_AEWB,  /* aewbMode */
        30,                             /* fps: DON'T CARE */
        4,                              /* numDataLanes */
        {1, 2, 3, 4},                   /* dataLanesMap */
        {0, 0, 0, 0},                   /* dataLanesPolarity */
        CSIRX_LANE_BAND_SPEED_1350_TO_1500_MBPS, /* csi_laneBandSpeed */
    },
    1,                                  /* numChan */
    5,                                  /* dccId   */
};

static IssSensorFxns           ox05c1sSensorFxns = {
    OX05C1S_Probe,
    OX05C1S_Config,
    OX05C1S_StreamOn,
    OX05C1S_StreamOff,
    OX05C1S_PowerOn,
    OX05C1S_PowerOff,
    OX05C1S_GetExpParams,
    OX05C1S_SetAeParams,
    OX05C1S_GetDccParams,
    OX05C1S_InitAewbConfig,
    OX05C1S_GetIspConfig,
    OX05C1S_ReadWriteReg,
    OX05C1S_GetExpPrgFxn,
    OX05C1S_deinit,
    OX05C1S_GetWBPrgFxn,
    OX05C1S_SetAwbParams,
    NULL
};

static IssSensorIntfParams     ox05c1sSensorIntfPrms = {
    0,                      /* sensorBroadcast */
    0,                      /* enableFsin */
    0,                      /* numCamerasStreaming */
};

IssSensors_Handle ox05c1sSensorHandle = {
    1,                      /* isUsed */
    &ox05c1sCreatePrms,     /* CreatePrms */
    &ox05c1sSensorFxns,     /* SensorFxns */
    &ox05c1sSensorIntfPrms, /* SensorIntfPrms */
};

/*
 * \brief DCC Parameters of OX05C1S
 */
extern IssSensors_Handle *gIssSensorTable[ISS_SENSORS_MAX_SUPPORTED_SENSOR];
static uint16_t redGain_prev[ISS_SENSORS_MAX_CHANNEL];
static uint16_t greenGain_prev[ISS_SENSORS_MAX_CHANNEL];
static uint16_t blueGain_prev[ISS_SENSORS_MAX_CHANNEL];

int32_t IssSensor_OX05C1S_Init()
{
    int32_t status;
    int32_t chId;
    status = IssSensor_Register(&ox05c1sSensorHandle);
    if(0 != status)
    {
        appLogPrintf("IssSensor_OX05C1S_Init failed \n");
    }
	for(chId=0;chId<ISS_SENSORS_MAX_CHANNEL;chId++)
	{
        redGain_prev[chId] = greenGain_prev[chId] = blueGain_prev[chId] = 512;
    }

    return status;
}

/*******************************************************************************
 *  Local Functions Definition
 *******************************************************************************
 */

static int32_t OX05C1S_Probe(uint32_t chId, void *pSensorHdl)
{
    int32_t status = 0;

    /* TODO: add probe code here if necessary */
    status = -1;
    
    return (status);
}

static int32_t OX05C1S_Sensor_RegConfig(uint32_t i2cInstId, uint8_t sensorI2cAddr, I2cParams *sensorCfg, uint16_t sensor_cfg_script_len)
{
    int32_t status = 0;
    uint16_t regAddr;
    uint8_t regValue;
    uint16_t delayMilliSec;
    uint32_t regCnt;

    if(NULL != sensorCfg)
    {
        regCnt = 0;
        regAddr  = sensorCfg[regCnt].nRegAddr;
        regValue = sensorCfg[regCnt].nRegValue;
        delayMilliSec = sensorCfg[regCnt].nDelay;

        appLogPrintf(" Configuring OX05C1S imager 0x%.2x... Please wait till it finishes \n", sensorI2cAddr);
        while(regCnt<sensor_cfg_script_len)
        {
            status |= OX05C1S_WriteReg(sensorI2cAddr, regAddr, regValue, 1u);
            if (0 != status)
            {
                appLogPrintf("\nOX05C1S: Sensor Reg Write Failed for regAddr 0x%.2x\n", regAddr);
            }

            if(delayMilliSec > 0)
            {
               appLogWaitMsecs(delayMilliSec);
            }

            regCnt++;
            regAddr  = sensorCfg[regCnt].nRegAddr;
            regValue = sensorCfg[regCnt].nRegValue;
            delayMilliSec = sensorCfg[regCnt].nDelay;
        }
        /* Wait 100ms after the init is done */
        appLogWaitMsecs(100);
    }
    else
    {
        appLogPrintf(" OX05C1S config script is NULL \n");
    }
    return status;
}

static int32_t OX05C1S_VerifyRegConfig(uint32_t i2cInstId, uint8_t sensorI2cAddr, I2cParams *sensorCfg, uint16_t sensor_cfg_script_len)
{
    int32_t status = 0;
    uint16_t regAddr;
    uint8_t regValue;
    uint8_t readValue;
    uint32_t regCnt;
    uint32_t mismatchCount = 0;

    if(NULL != sensorCfg)
    {
        appLogPrintf(" Verifying OX05C1S imager 0x%.2x register configuration... \n", sensorI2cAddr);

        for(regCnt = 0; regCnt < sensor_cfg_script_len; regCnt++)
        {
            regAddr = sensorCfg[regCnt].nRegAddr;
            regValue = sensorCfg[regCnt].nRegValue;

            status = OX05C1S_ReadReg(sensorI2cAddr, regAddr, &readValue, 1u);
            if (0 != status)
            {
                appLogPrintf("OX05C1S: Failed to read register 0x%.4x\n", regAddr);
            }
            else
            {
                if(readValue != regValue)
                {
                    appLogPrintf("OX05C1S: Register mismatch at 0x%.4x - Expected: 0x%.2x, Read: 0x%.2x\n",
                           regAddr, regValue, readValue);
                    mismatchCount++;
                }
            }
        }

        if(mismatchCount == 0)
        {
            appLogPrintf(" OX05C1S register verification completed successfully - all %d registers match\n", sensor_cfg_script_len);
        }
        else
        {
            appLogPrintf(" OX05C1S register verification completed with %d mismatches out of %d registers\n",
                   mismatchCount, sensor_cfg_script_len);
            status = -1;
        }
    }
    else
    {
        appLogPrintf(" OX05C1S config script is NULL \n");
        status = -1;
    }

    return status;
}

static uint32_t ox05c1sFeaturesEnabled;
static int32_t OX05C1S_Config(uint32_t chId, void *pSensorHdl, uint32_t sensor_features_requested)
{
    int32_t status = 0;
    uint32_t i2cInstId;
    uint16_t sensor_cfg_script_len = 0;
    IssSensors_Handle * pSenHandle = (IssSensors_Handle*)pSensorHdl;
    IssSensor_CreateParams * pCreatePrms;

    uint8_t regVal = 0x00;

    assert(NULL != pSenHandle);
    pCreatePrms = pSenHandle->createPrms;
    assert(NULL != pCreatePrms);

    if(sensor_features_requested != (sensor_features_requested & ISS_SENSOR_OX05C1S_FEATURES))
    {
        appLogPrintf("OX05C1S_Config : Error. feature set 0x%x is not supported \n", sensor_features_requested);
        return -1;
    }

    ox05c1sFeaturesEnabled = sensor_features_requested;
    i2cInstId = pCreatePrms->i2cInstId;

    /* Configure serializer */
    status = ub953_cfgScript(i2cInstId, pCreatePrms->i2cAddrSer[chId], SerCfg_OX05C1S);
    if(0 != status)
    {
        appLogPrintf("OX05C1S_Config Error : UB953 config failed for camera # %d \n", chId);
    }
    else
    {
        OX05C1S_ReadReg(pCreatePrms->i2cAddrSensor[0], 0x300A, &regVal, 1);
        appLogPrintf("\n\n\n DEBUG X5B Addr: 0x%02x, regAddr: 0x300A = 0x%02x \n", pCreatePrms->i2cAddrSensor[0], regVal);



        sensor_cfg_script_len = sizeof(ox05c1sConfig_rggb) / sizeof(ox05c1sConfig_rggb[0]);
        status = OX05C1S_Sensor_RegConfig(i2cInstId, pCreatePrms->i2cAddrSensor[chId], ox05c1sConfig_rggb, sensor_cfg_script_len);
        if(0 != status)
        {
            appLogPrintf("OX05C1S_Config Error : OX05C1S_Sensor_RegConfig returned status = %d \n", status);
        }
        else
        {
            /* Verify the configuration was written correctly */
            status = OX05C1S_VerifyRegConfig(i2cInstId, pCreatePrms->i2cAddrSensor[chId], ox05c1sConfig_rggb, sensor_cfg_script_len);
            if(0 != status)
            {
                appLogPrintf("OX05C1S_Config Error : OX05C1S_VerifyRegConfig failed for camera # %d \n", chId);
            }
        }
    }

    return (status);
}

static int32_t OX05C1S_StreamOn(uint32_t chId, void *pSensorHdl)
{
    int32_t status = 0;

    IssSensors_Handle * pSenHandle = (IssSensors_Handle*)pSensorHdl;
    IssSensor_CreateParams * pCreatePrms;
    uint32_t i2cInstId;
    uint8_t sensorI2cAddr;
    uint8_t regVal;

    assert(NULL != pSenHandle);
    pCreatePrms = pSenHandle->createPrms;
    assert(NULL != pCreatePrms);

    i2cInstId = pCreatePrms->i2cInstId;
    sensorI2cAddr = pCreatePrms->i2cAddrSensor[chId];

    /* Stream on */
    status |= OX05C1S_WriteReg(sensorI2cAddr, 0x0100, 0x01, 1u);
#if 0
    status |= OX05C1S_WriteReg(sensorI2cAddr, 0x0100, 0x00, 1u);
#endif
    appLogWaitMsecs(10);

    OX05C1S_ReadReg(sensorI2cAddr, 0x0100, &regVal, 1u);
    appLogPrintf("\n\n StreasmOn Value after [0x0100] = 0x%02x\n", regVal);

    /* Enable deserializer CSI-TX output */
    status |= enableUB960Streaming(chId);
    return (status);
}

static int32_t OX05C1S_StreamOff(uint32_t chId, void *pSensorHdl)
{
    int32_t status = 0;

    IssSensors_Handle * pSenHandle = (IssSensors_Handle*)pSensorHdl;
    IssSensor_CreateParams * pCreatePrms;
    uint32_t i2cInstId;
    uint8_t sensorI2cAddr;

    assert(NULL != pSenHandle);
    pCreatePrms = pSenHandle->createPrms;
    assert(NULL != pCreatePrms);

    i2cInstId = pCreatePrms->i2cInstId;
    sensorI2cAddr = pCreatePrms->i2cAddrSensor[chId];

    /* Stream off */
    status |= OX05C1S_WriteReg(sensorI2cAddr, 0x0100, 0x00, 1u);
    appLogWaitMsecs(10);

    /* Disable deserializer CSI-TX output */
    status |= disableUB960Streaming(chId);
    return status;
}

static int32_t OX05C1S_PowerOn(uint32_t chId, void *pSensorHdl)
{
    int32_t status = 0;

    uint8_t serI2Caddr_7bit = OX05C1S_SER_I2C_ADDR >> 1;
    uint8_t senI2cAddr_7bit = OX05C1S_SEN_I2C_ADDR >> 1;

    IssSensors_Handle *pSenHandle = (IssSensors_Handle*)pSensorHdl;
    assert(pSenHandle != NULL);
    IssSensor_CreateParams *pCreatePrms = pSenHandle->createPrms;
    assert(pCreatePrms != NULL);

    uint32_t i2cInstId = pCreatePrms->i2cInstId;

#if 1
    /* Configure deserializer to FPD4 mode */
    for (int i = 0; i < 2; i++)
    {
        status = IssSensor_cfgDesScript(ub9702_971_DesCfg_Common, i);
        if(status!=0)
        {
            appLogPrintf("Error : IssSensor_cfgDesScript returned %d while configuring DES %d \n", status, i);
        }
    }
#endif

    /* Set serializer alias address */
    //Gang_TODO: is this refactored?
    status = UB960_SetAlias(chId, 0, serI2Caddr_7bit, pCreatePrms->i2cAddrSer[chId]);
    if(0 != status)
    {
        appLogPrintf("OX05C1S_PowerOn Error : UB960_SetAlias returned %d when setting serializer (0x%.2x 7-bit) to alias 0x%.2x 7-bit \n",
            status, serI2Caddr_7bit, pCreatePrms->i2cAddrSer[chId]);
    }

    /* Set sensor alias address */
    //Gang_TODO: is this refactored?
    status = UB960_SetAlias(chId, 1, senI2cAddr_7bit, pCreatePrms->i2cAddrSensor[chId]);
    if(0 != status)
    {
        appLogPrintf("OX05C1S_PowerOn Error : UB960_SetAlias returned %d when setting sensor (0x%.2x 7-bit) to alias 0x%.2x 7-bit \n",
            status, senI2cAddr_7bit, pCreatePrms->i2cAddrSer[chId]);
    }

    appLogWaitMsecs(20);

    return status;
}

static int32_t OX05C1S_PowerOff(uint32_t chId, void *pSensorHdl)
{
    int32_t status = 0;

    return (status);
}

static int32_t OX05C1S_SetAeParams(void *pSensorHdl, uint32_t chId, IssSensor_ExposureParams *pExpPrms)
{
    int32_t status = 0;

    /* TODO: Set exposure in terms of row time */

    /* TODO: Set analog gain */
    
    /* TODO: Set digital gain */

    return (status);
}

static int32_t OX05C1S_GetDccParams(uint32_t chId, void *pSensorHdl, IssSensor_DccParams *pDccPrms)
{
    int32_t status = 0;
    return (status);
}

static int32_t OX05C1S_GetExpParams(uint32_t chId, void *pSensorHdl, IssSensor_ExposureParams *pExpPrms)
{
    int32_t status = 0;
    return (status);
}

static void OX05C1S_InitAewbConfig(uint32_t chId, void *pSensorHdl)
{
    return;
}

static void OX05C1S_GetIspConfig(uint32_t chId, void *pSensorHdl)
{
    return;
}

static void OX05C1S_deinit(uint32_t chId, void *pSensorHdl)
{
    return;
}

static int32_t OX05C1S_ReadWriteReg(uint32_t chId, void *pSensorHdl, uint32_t readWriteFlag, I2cParams *pReg)
{
    int32_t status = 0;

    uint8_t regValue = 0;
    IssSensors_Handle * pSenHandle = (IssSensors_Handle*)pSensorHdl;
    IssSensor_CreateParams * pCreatePrms;

    assert(NULL != pSenHandle);
    pCreatePrms = pSenHandle->createPrms;
    assert(NULL != pReg);

    if (1u == readWriteFlag)
    {
        /* write */
        regValue = pReg->nRegValue;
        status = OX05C1S_WriteReg(pCreatePrms->i2cAddrSensor[chId], pReg->nRegAddr, regValue, 1u);
    }
    else
    {
        /* read */
        status = OX05C1S_ReadReg(pCreatePrms->i2cAddrSensor[chId], pReg->nRegAddr, &regValue, 1u);
        if (0 == status)
        {
            pReg->nRegValue = regValue;
        }
    }
    return (status);
}

static int32_t OX05C1S_ReadReg(uint8_t i2cAddr,
                                uint16_t regAddr,
                                uint8_t *regVal,
                                uint32_t numRegs)
{
    int32_t  status = -1;
    I2C_Handle sensorI2cHandle = NULL;
    static uint8_t sensorI2cByteOrder = 255U;

    getIssSensorI2cInfo(&sensorI2cByteOrder, &sensorI2cHandle);
    if(NULL == sensorI2cHandle)
    {
        appLogPrintf("Sensor I2C Handle is NULL \n");
        return -1;
    }

    // 16-bit address is used by most ADAS sensors from OVT/Sony/Onsemi
    status = Board_i2c16BitRegRd(sensorI2cHandle, i2cAddr, regAddr, regVal, numRegs, sensorI2cByteOrder, SENSOR_I2C_TIMEOUT);
    if(0 != status)
    {
        appLogPrintf("Error : I2C Timeout while reading from OX05C1S register 0x%x \n", regAddr);
    }

    return (status);
}

static int32_t OX05C1S_WriteReg(uint8_t i2cAddr,
                                uint16_t regAddr,
                                uint8_t regVal,
                                uint32_t numRegs)
{
    int32_t  status = -1;
    I2C_Handle sensorI2cHandle = NULL;
    static uint8_t sensorI2cByteOrder = 255U;

    getIssSensorI2cInfo(&sensorI2cByteOrder, &sensorI2cHandle);
    if(NULL == sensorI2cHandle)
    {
        appLogPrintf("Sensor I2C Handle is NULL \n");
        return -1;
    }

    // 16-bit address is used by most ADAS sensors from OVT/Sony/Onsemi
    status = Board_i2c16BitRegWr(sensorI2cHandle, i2cAddr, regAddr, &regVal, numRegs, sensorI2cByteOrder, SENSOR_I2C_TIMEOUT);
    if(0 != status)
    {
        appLogPrintf("Error : I2C Timeout while writing 0x%x to OX05C1S register 0x%x \n", regVal, regAddr);
    }

    return (status);
}

static int32_t OX05C1S_GetExpPrgFxn(uint32_t chId, void *pSensorHdl, IssAeDynamicParams *p_ae_dynPrms)
{
    int32_t status = 0;
    uint8_t count = 0;

    p_ae_dynPrms->targetBrightnessRange.min = 45;
    p_ae_dynPrms->targetBrightnessRange.max = 55;
    p_ae_dynPrms->targetBrightness = 50;
    p_ae_dynPrms->threshold = 5;
    p_ae_dynPrms->exposureTimeStepSize = 1;
    p_ae_dynPrms->enableBlc = 0;

    /* 16.666ms */
    p_ae_dynPrms->exposureTimeRange[count].min = 16666;
    p_ae_dynPrms->exposureTimeRange[count].max = 16666;

    /* sensor gain: 1x gain (1024) */
    p_ae_dynPrms->analogGainRange[count].min = 1024;
    p_ae_dynPrms->analogGainRange[count].max = 1024;

    /* digital 1x gain (256) */
    p_ae_dynPrms->digitalGainRange[count].min = 256;    /* not used */
    p_ae_dynPrms->digitalGainRange[count].max = 256;    /* not used */
    count++;

    p_ae_dynPrms->numAeDynParams = count;
    return (status);
}

static int32_t OX05C1S_GetWBPrgFxn(uint32_t chId, void *pSensorHdl, IssAwbDynamicParams *p_awb_dynPrms)
{
    int32_t  status = 0;

    p_awb_dynPrms->redGainRange.min = 512;
    p_awb_dynPrms->redGainRange.max = 512;

    p_awb_dynPrms->greenGainRange.min = 512;
    p_awb_dynPrms->greenGainRange.max = 512;

    p_awb_dynPrms->blueGainRange.min = 512;
    p_awb_dynPrms->blueGainRange.max = 512;

    p_awb_dynPrms->sensor_pre_gain = 0;

    appLogPrintf("OX05C1S_GetWBPrgFxn: sensor_pre_gain = %d \n", p_awb_dynPrms->sensor_pre_gain);
    return (status);
}

static int32_t OX05C1S_SetAwbParams(void *pSensorHdl, uint32_t chId, IssSensor_WhiteBalanceParams *pWbPrms)
{
    int32_t status = 0;

    /* TODO */

    return (status);
}

static void OX05C1S_print_frame_count(void *pSensorHdl)
{

    uint8_t     regVal = 0x00;
    uint32_t    frameCount = 0;

    IssSensors_Handle *pSenHandle = (IssSensors_Handle *)pSensorHdl;
    IssSensor_CreateParams *pCreatePrms;

    pCreatePrms = pSenHandle->createPrms;
    
    /*
        Address |   Description
        0x4610  |   Bit[7:0]: vfifo_fcnt[31:24]
        0x4611  |   Bit[7:0]: vfifo_fcnt[23:16]
        0x4612  |   Bit[7:0]: vfifo_fcnt[15:8]
        0x4613  |   Bit[7:0]: vfifo_fcnt[7:0]
    */
    uint16_t fCntAddrArr[] = {0x4610, 0x4611, 0x4612, 0x4613};

    appLogPrintf("\n\n");
    for(uint8_t i = 0; i<(sizeof(fCntAddrArr)/sizeof(fCntAddrArr[0])); i++)
    {
        OX05C1S_ReadReg(pCreatePrms->i2cAddrSensor[0], fCntAddrArr[i], &regVal, 1);
        appLogPrintf("%s: %s [0x%02x] = 0x%02x \n", __func__, SENSOR_X5C_UB971_OV, fCntAddrArr[i], regVal);

        frameCount = (frameCount << 8) | regVal;
    }

    appLogPrintf("\n%s: %s Frame count: %d \n", __func__, SENSOR_X5C_UB971_OV, frameCount);
}





