/*
 *  Copyright (C) 2020 Texas Instruments Incorporated.
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
 *
 */

/**
 *  \ingroup CSL_IP_MODULE
 *  \defgroup CSL_VTM VTM CSL-FL
 *
 */
/**
 *  \ingroup CSL_VTM
 *  \defgroup CSL_VTM_PVT_SENSOR VTM PVT Sensor
 *
 *  @{
 */

/**
 *  \file  csl_vtm_pvt_sensor.h
 *
 *  \brief
 *     Header file containing various enumerations, structure definitions and function
 *  declarations for the Voltage and Thermal Monitor (VTM) PVT Sensor Workaround.
 */


#ifndef CSL_VTM_PVT_SENSOR_H
#define CSL_VTM_PVT_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Macros */
#define CSL_VTM_NUM_OF_SENSOR_WA_COMP                       (1)
#define CSL_VTM_NUM_OF_ADC_CODES                            (1024)

/* Uninitialized value */
#define CSL_VTM_VALUES_ARE_UNINITIALIZED    (-1)

typedef int8_t CSL_vtm_err_id;
#define CSL_VTM_ERR_ID_N40                                  (0)
#define CSL_VTM_ERR_ID_P30                                  (1)
#define CSL_VTM_ERR_ID_P125                                 (2)
#define CSL_VTM_ERR_ID_P150                                 (3)

/* Delay for Reg Reads */
#define CSL_VTM_REG_READ_DELAY                              (5000)

/* Minimum and Maximum temperatures */
#define CSL_VTM_TEMPERATURE_MILLI_DEGREE_C_MIN              (-42000)
#define CSL_VTM_TEMPERATURE_MILLI_DEGREE_C_MAX              (158000)
#define CSL_VTM_NUM_EFUSE_REGS                              (4u)

/* Sensor 0 Shift and Mask values */
#define CSL_VTM_EFUSE0_S0_N40_MASK                           (0x0000003FU)
#define CSL_VTM_EFUSE0_S0_N40_SHIFT                          (0x00000000U)
#define CSL_VTM_EFUSE0_S0_N40_SIGN_BIT_MASK                  (0x00000020U)

#define CSL_VTM_EFUSE2_S0_P30_MASK                           (0x001FE000U)
#define CSL_VTM_EFUSE2_S0_P30_SHIFT                          (0x0000000DU)
#define CSL_VTM_EFUSE2_S0_P30_SIGN_BIT_MASK                  (0x00000080U)


#define CSL_VTM_EFUSE1_S0_P125_MASK                          (0x000001FFU)
#define CSL_VTM_EFUSE1_S0_P125_SHIFT                         (0x00000000U)
#define CSL_VTM_EFUSE1_S0_P125_SIGN_BIT_MASK                 (0x00000100U)


/* Sensor 1 Shift and Mask values */
#define CSL_VTM_EFUSE0_S1_N40_MASK                           (0x00000FC0U)
#define CSL_VTM_EFUSE0_S1_N40_SHIFT                          (0x000000006U)

/* Sensor 2 Shift and Mask values */
#define CSL_VTM_EFUSE0_S2_N40_MASK                           (0x0003F000U)
#define CSL_VTM_EFUSE0_S2_N40_SHIFT                          (0x0000000CU)

/* Sensor 3 Shift and Mask values */
#define CSL_VTM_EFUSE0_S3_N40_MASK                           (0x00FC0000U)
#define CSL_VTM_EFUSE0_S3_N40_SHIFT                          (0x00000012U)

/* Sensor 4 Shift and Mask values */
#define CSL_VTM_EFUSE0_S4_N40_MASK                           (0x3F000000U)
#define CSL_VTM_EFUSE0_S4_N40_SHIFT                          (0x00000018U)

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif  /* end of CSL_VTM_PVT_SENSOR_H definition */

 /** @} */
