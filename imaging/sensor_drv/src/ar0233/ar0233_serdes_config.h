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
#ifndef AR0233_SERDES_H
#define AR0233_SERDES_H

/**
7-bit Alias addresses for sensor and serializer
Physical addresses must be programmed in UB96x config
SoC will communicate with the devices using alias adresses 
*/

//#define AR0233_TEST_PATTERN_ENABLE


#ifdef AR0233_TEST_PATTERN_ENABLE
#define AR0233_SER_CFG_SIZE    (37U)
#else
#define AR0233_SER_CFG_SIZE    (7U)
#endif

#define AR0233_I2C_ADDR        (0x10U)

I2cParams ub953SerCfg_AR0233[AR0233_SER_CFG_SIZE] = {
    {0x1, 0x2, 1000},
    {0x2, 0x73, 0x1},
    {0x6, 0x41, 0x1},
    {0x7, 0x22, 0x1},
    {0xE, 0x3c, 0x1},
    {0xD, 0x1 , 0x1},

#ifdef AR0233_TEST_PATTERN_ENABLE
#define AR0233_WIDTH_HIBYTE  (((uint32_t)AR0233_OUT_WIDTH >> 8U) & 0xFFU)
#define AR0233_WIDTH_LOBYTE  ((uint32_t)AR0233_OUT_WIDTH & 0xFFU)
#define AR0233_HEIGHT_HIBYTE  (((uint32_t)AR0233_OUT_HEIGHT >> 8U) & 0xFFU)
#define AR0233_HEIGHT_LOBYTE  ((uint32_t)AR0233_OUT_HEIGHT & 0xFFU)

#define BAR_WIDTH_HIBYTE  ((((uint32_t)AR0233_OUT_WIDTH / 4U) >> 8U) & 0xFFU)
#define BAR_WIDTH_LOBYTE  (((uint32_t)AR0233_OUT_WIDTH / 4U) & 0xFFU)

    {0xB0, 0x00, 0x10}, // Indirect Pattern Gen Registers
    {0xB1, 0x01, 0x10}, // PGEN_CTL
    {0xB2, 0x01, 0x10}, //
    {0xB1, 0x02, 0x10}, // PGEN_CFG
    {0xB2, 0x33, 0x10}, //
    {0xB1, 0x03, 0x10}, // PGEN_CSI_DI
    {0xB2, 0x2C, 0x10}, //RAW12
    {0xB1, 0x04, 0x10}, // PGEN_LINE_SIZE1
    {0xB2, AR0233_WIDTH_HIBYTE, 0x10}, //
    {0xB1, 0x05, 0x10}, // PGEN_LINE_SIZE0
    {0xB2, AR0233_WIDTH_LOBYTE, 0x10}, //
    {0xB1, 0x06, 0x10}, // PGEN_BAR_SIZE1
    {0xB2, BAR_WIDTH_HIBYTE, 0x10}, //
    {0xB1, 0x07, 0x10}, // PGEN_BAR_SIZE0
    {0xB2, BAR_WIDTH_LOBYTE, 0x10}, //
    {0xB1, 0x08, 0x10}, // PGEN_ACT_LPF1
    {0xB2, AR0233_HEIGHT_HIBYTE, 0x10}, //
    {0xB1, 0x09, 0x10}, // PGEN_ACT_LPF0
    {0xB2, AR0233_HEIGHT_LOBYTE, 0x10}, //
    {0xB1, 0x0A, 0x10}, // PGEN_TOT_LPF1
    {0xB2, 0x04, 0x10}, //
    {0xB1, 0x0B, 0x10}, // PGEN_TOT_LPF0
    {0xB2, 0x1A, 0x10}, //
    {0xB1, 0x0C, 0x10}, // PGEN_LINE_PD1
    {0xB2, 0x0C, 0x10}, //
    {0xB1, 0x0D, 0x10}, // PGEN_LINE_PD0
    {0xB2, 0x67, 0x10}, //
    {0xB1, 0x0E, 0x10}, // PGEN_VBP
    {0xB2, 0x21, 0x10}, //
    {0xB1, 0x0F, 0x10}, // PGEN_VF
#endif

    {0xFFFF, 0x00, 0x0} //End of script
};

I2cParams ub960AR0233DesCSI2Enable[2u] = {
    {0x33, 0x03, 0x10},
    {0xFFFF, 0x00, 0x0} //End of script
};

I2cParams ub960AR0233DesCSI2Disable[2u] = {
    {0x33, 0x02, 0x10},
    {0xFFFF, 0x00, 0x0} //End of script
};

#endif /* AR0233_SERDES_H */


