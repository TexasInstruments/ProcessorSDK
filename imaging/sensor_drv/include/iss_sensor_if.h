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
#ifndef ISS_SENSORS_IF_H_
#define ISS_SENSORS_IF_H_

#define SENSOR_0_I2C_ALIAS    (0x40U)
#define SENSOR_1_I2C_ALIAS    (0x42U)
#define SENSOR_2_I2C_ALIAS    (0x44U)
#define SENSOR_3_I2C_ALIAS    (0x46U)

#define SENSOR_4_I2C_ALIAS    (0x48U)
#define SENSOR_5_I2C_ALIAS    (0x4AU)
#define SENSOR_6_I2C_ALIAS    (0x4CU)
#define SENSOR_7_I2C_ALIAS    (0x4EU)

#define SENSOR_8_I2C_ALIAS    (0x50U)
#define SENSOR_9_I2C_ALIAS    (0x5CU)
#define SENSOR_10_I2C_ALIAS   (0x5EU)
#define SENSOR_11_I2C_ALIAS   (0x60U)

#define SER_0_I2C_ALIAS       (0x74U)
#define SER_1_I2C_ALIAS       (0x76U)
#define SER_2_I2C_ALIAS       (0x78U)
#define SER_3_I2C_ALIAS       (0x7AU)

#define SER_4_I2C_ALIAS       (0x6CU)
#define SER_5_I2C_ALIAS       (0x6EU)
#define SER_6_I2C_ALIAS       (0x6FU)
#define SER_7_I2C_ALIAS       (0x72U)

#define SER_8_I2C_ALIAS       (0x54U)
#define SER_9_I2C_ALIAS       (0x56U)
#define SER_10_I2C_ALIAS      (0x58U)
#define SER_11_I2C_ALIAS      (0x5AU)

#define MISC_0_I2C_ALIAS      (0x50U)
#define MISC_1_I2C_ALIAS      (0x51U)
#define MISC_2_I2C_ALIAS      (0x52U)
#define MISC_3_I2C_ALIAS      (0x53U)

#define MISC_4_I2C_ALIAS      (0x55U)
#define MISC_5_I2C_ALIAS      (0x57U)
#define MISC_6_I2C_ALIAS      (0x59U)
#define MISC_7_I2C_ALIAS      (0x5BU)

#define MISC_8_I2C_ALIAS      (0x5DU)
#define MISC_9_I2C_ALIAS      (0x5FU)
#define MISC_10_I2C_ALIAS     (0x61U)
#define MISC_11_I2C_ALIAS     (0x63U)


#if defined(SOC_J784S4) || defined(SOC_J742S2)
#define I2C_ADDR_SENSOR     \
    {                       \
        SENSOR_0_I2C_ALIAS, SENSOR_1_I2C_ALIAS, SENSOR_2_I2C_ALIAS, SENSOR_3_I2C_ALIAS,     \
        SENSOR_4_I2C_ALIAS, SENSOR_5_I2C_ALIAS, SENSOR_6_I2C_ALIAS, SENSOR_7_I2C_ALIAS,     \
        SENSOR_8_I2C_ALIAS, SENSOR_9_I2C_ALIAS, SENSOR_10_I2C_ALIAS, SENSOR_11_I2C_ALIAS    \
    }

#define I2C_ADDR_SER    \
    {                   \
        SER_0_I2C_ALIAS, SER_1_I2C_ALIAS, SER_2_I2C_ALIAS, SER_3_I2C_ALIAS,     \
        SER_4_I2C_ALIAS, SER_5_I2C_ALIAS, SER_6_I2C_ALIAS, SER_7_I2C_ALIAS,     \
        SER_8_I2C_ALIAS, SER_9_I2C_ALIAS, SER_10_I2C_ALIAS, SER_11_I2C_ALIAS    \
    }
#else
#define I2C_ADDR_SENSOR     \
    {                       \
        SENSOR_0_I2C_ALIAS, SENSOR_1_I2C_ALIAS, SENSOR_2_I2C_ALIAS, SENSOR_3_I2C_ALIAS,     \
        SENSOR_4_I2C_ALIAS, SENSOR_5_I2C_ALIAS, SENSOR_6_I2C_ALIAS, SENSOR_7_I2C_ALIAS,     \
    }

#define I2C_ADDR_SER    \
    {                   \
        SER_0_I2C_ALIAS, SER_1_I2C_ALIAS, SER_2_I2C_ALIAS, SER_3_I2C_ALIAS,     \
        SER_4_I2C_ALIAS, SER_5_I2C_ALIAS, SER_6_I2C_ALIAS, SER_7_I2C_ALIAS,     \
    }
#endif /* defined(SOC_J784S4) || defined(SOC_J742S2) */

#endif /* End of ISS_SENSORS_IF_H_*/


