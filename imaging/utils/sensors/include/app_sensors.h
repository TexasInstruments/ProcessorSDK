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

#ifndef APP_SENSORS_H_
#define APP_SENSORS_H_

/**
 * \defgroup group_vision_apps_utils_sensors Sensor Initialization API (RTOS only)
 *
 * \brief This section contains APIs for Sensor initialization
 *
 * \ingroup group_vision_apps_utils
 *
 * @{
 */

#include <stdint.h>


#define APP_REMOTE_SERVICE_SENSOR_NAME  "com.ti.remote_service_sensor"

#define APP_REMOTE_SERVICE_SENSOR_CMD_CONFIG_OV2775     (0x1000)

#define APP_REMOTE_SERVICE_SENSOR_CMD_CONFIG_IMX390     (0x1001)

#define APP_PORT_NUM_MAX                                (2U)

#define APP_MAX_SENSORS                                 (4u)

#if defined (MCU_PLUS_SDK)
/* Ported from pdk/packages/ti/drv/i2c/soc/i2c_soc.h */
#define I2C_HWIP_MAX_CNT         (7U)

/**
 *  \anchor BOARD_csiInst
 *  \name   CSIRX Module instance
 *  \brief  CSIRX Module instance used to get the I2C instance.
 *
 *  @{
 */
/** \brief CSIRX Instance: 0 */
#define BOARD_CSI_INST_0             ((uint32_t) 0x0U)
/** \brief CSIRX Instance: 1 */
#define BOARD_CSI_INST_1             ((uint32_t) 0x1U)
/** \brief CSIRX Instance: 2 */
#define BOARD_CSI_INST_2             ((uint32_t) 0x2U)
/* @} */

/** I2C transaction default timeout */
#define BOARD_I2C_TRANSACTION_TIMEOUT                       (2000U)

#define I2C_400kHz                                          I2C_400KHZ

/** To send the MSB of a register offset address first */
#define BOARD_I2C_REG_ADDR_MSB_FIRST                        (0x0U)

#else
#endif

typedef struct
{
    /*! Number of port to be configured  */
    uint32_t portNum;
    /*! Port ID mapping  */
    uint32_t portIdMap[APP_PORT_NUM_MAX];
    /*! Number of sensors to be configured  */
    uint32_t numSensors;
} AppSensorCmdParams;

int32_t appI2cInit(void);

int32_t appI2cDeInit(void);

int32_t appRemoteServiceSensorInit(void);

int32_t appRemoteServiceSensorDeInit(void);

/* @} */

#endif

