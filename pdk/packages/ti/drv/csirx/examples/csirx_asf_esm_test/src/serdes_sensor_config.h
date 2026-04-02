/*
 *  Copyright (c) Texas Instruments Incorporated 2025
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file serdes_sensor_config.h
 *
 *  \brief header for serdes sensor configuration
 *
 */
#ifndef SERDES_SENSOR_CONFIG_H_
#define SERDES_SENSOR_CONFIG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "imx390.h"
#include "csirx_asf_esm_test_main.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/**
 * @{
 * I2C Addresses for serialisers/Sensors attached to the UB960
 */
#define UB960_SERIALISER_ADDR                (0x18)
#define D3IMX390_SENSOR_ADDR_CM_MODULE       (0x21)
#define D3IMX390_SENSOR_ADDR_RCM_MODULE      (0x1A)
/** @} */

/**
 * @{
 * Generic Alias Addresses for serialisers attached to the UB960 and UB9702 Instance0
 */
#define D3IMX390_INST0_PORT_0_SER_ADDR       (0x74U)
#define D3IMX390_INST0_PORT_1_SER_ADDR       (0x76U)
#define D3IMX390_INST0_PORT_2_SER_ADDR       (0x78U)
#define D3IMX390_INST0_PORT_3_SER_ADDR       (0x7AU)
/** @} */

/**
 * @{
 * Generic Alias Addresses for sensors attached to the UB960 Instance0
 */
#define D3IMX390_INST0_PORT_0_SENSOR_ADDR    (0x40U)
#define D3IMX390_INST0_PORT_1_SENSOR_ADDR    (0x42U)
#define D3IMX390_INST0_PORT_2_SENSOR_ADDR    (0x44U)
#define D3IMX390_INST0_PORT_3_SENSOR_ADDR    (0x46U)
/** @} */

/**
 * @{
 * Generic Alias Addresses for serialisers attached to the UB960 Instance1
 */
#define D3IMX390_INST1_PORT_0_SER_ADDR       (0x6CU)
#define D3IMX390_INST1_PORT_1_SER_ADDR       (0x6EU)
#define D3IMX390_INST1_PORT_2_SER_ADDR       (0x70U)
#define D3IMX390_INST1_PORT_3_SER_ADDR       (0x72U)
/** @} */

/**
 * @{
 * Generic Alias Addresses for sensors attached to the UB960 Instance1
 */
#define D3IMX390_INST1_PORT_0_SENSOR_ADDR    (0x48U)
#define D3IMX390_INST1_PORT_1_SENSOR_ADDR    (0x4AU)
#define D3IMX390_INST1_PORT_2_SENSOR_ADDR    (0x4CU)
#define D3IMX390_INST1_PORT_3_SENSOR_ADDR    (0x4EU)

/** @} */

/**
 * @{
 * Generic Alias Addresses for serialisers attached to the UB960 Instance1
 */
#define D3IMX390_INST2_PORT_0_SER_ADDR       (0x54U)
#define D3IMX390_INST2_PORT_1_SER_ADDR       (0x56U)
#define D3IMX390_INST2_PORT_2_SER_ADDR       (0x58U)
#define D3IMX390_INST2_PORT_3_SER_ADDR       (0x5AU)
/** @} */

/**
 * @{
 * Generic Alias Addresses for sensors attached to the UB960 Instance1
 */
#define D3IMX390_INST2_PORT_0_SENSOR_ADDR    (0x50U)
#define D3IMX390_INST2_PORT_1_SENSOR_ADDR    (0x52U)
#define D3IMX390_INST2_PORT_2_SENSOR_ADDR    (0x5CU)
#define D3IMX390_INST2_PORT_3_SENSOR_ADDR    (0x5EU)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   App function to configure remote sensors.
 *
 * \param   appInstObj        CSI RX Capture Test Object
 *
 * \retval  Sensor configuration status.
 */
int32_t CsirxApp_sensorConfig(CsirxApp_CaptInstObj* appInstObj);

/**
 * \brief   App function to setup I2C instance to access Deserializer.
 *
 * \param   appInstObj        CSI RX Capture Test Object
 *
 * \retval  I2C instance setup status.
 */
int32_t CsirxApp_setupI2CInst(CsirxApp_CaptInstObj* appInstObj);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

#endif /* End of SERDES_SENSOR_CONFIG_H_ */
