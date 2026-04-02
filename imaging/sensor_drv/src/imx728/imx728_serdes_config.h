/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sony Semiconductor Solutions Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
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
#ifndef IMX728_SERDES_H
#define IMX728_SERDES_H
    
#include "iss_sensor_imx728.h"

#define IMX728_D3

/*
7-bit Alias addresses for sensor and serializer
Physical addresses must be programmed in UB96x config
SoC will communicate with the devices using alias adresses 
*/
    
//#define FUSION_BOARD_VER 0 //RevB and older
#define FUSION_BOARD_VER 1 //RevC
    
#define IMX728_SONY_I2C_ADDR 0x1AU

#ifdef IMX728_D3
#define IMX728_PCAL_7BIT_ADDR 0x70U
#endif
    
//#define ENABLE_IMX728_SER_TEST_PATTERN
    
/*RAW12*/
#define IMX728_RAWTESTPAT_OUT_BYTES_PER_LINE (((uint32_t)IMX728_OUT_WIDTH * 3U) >> 1U)
#define IMX728_RAWTESTPAT_BPLN_HIGH          (((uint32_t)IMX728_RAWTESTPAT_OUT_BYTES_PER_LINE & 0xFF00U) >> 8U)
#define IMX728_RAWTESTPAT_BPLN_LOW           ((uint32_t)IMX728_RAWTESTPAT_OUT_BYTES_PER_LINE & 0x00FFU)
#define IMX728_RAWTESTPAT_OUT_HEIGHT_HIGH    (((uint32_t)IMX728_OUT_HEIGHT & 0xFF00U) >> 8U)
#define IMX728_RAWTESTPAT_OUT_HEIGHT_LOW     ((uint32_t)IMX728_OUT_HEIGHT & 0x00FFU)
#define IMX728_RAWTESTPAT_BAR_SIZE           ((uint32_t)IMX728_RAWTESTPAT_OUT_BYTES_PER_LINE / 8U)
    
#define IMX728_SONY_SER_CFG_SIZE           (10U)
#define IMX728_SONY_DES_FSYNC_CFG_SIZE     (15U)
#define IMX728_RAWTESTPAT_SER_CFG_SIZE     (23U)
    
I2cParams ub953SerCfg_SonyIMX728[IMX728_SONY_SER_CFG_SIZE] = {
    {0x01, 0x05, 0x20},
    {0x02, 0x72, 0x10},
    
#if (FUSION_BOARD_VER == 0)
    {0x06, 0x21, 0x1F},
#elif (FUSION_BOARD_VER == 1)
    {0x06, 0x41, 0x1F},
#else
Unsuppprted version
#endif
    
    {0x07, 0x25, 0x1F},
    {0x0D, 0x18, 0x10},
    {0x0E, 0xF0, 0x10},
    {0xB0, 0x04, 0x10},
    {0xB1, 0x08, 0x10},
    {0xB2, 0x07, 0x10},
    
    {0xFFFF, 0x00, 0x0} // End of script
};

I2cParams powerOff_SonyIMX728[2] = {
    {0x0D, 0x00, 0x10}, // Turn off sensor
    {0xFFFF, 0x00, 0x00},
};

I2cParams ub960DesFsyncCfg_SonyIMX728[IMX728_SONY_DES_FSYNC_CFG_SIZE] = {
    {0x4c, 0x01, 0x00}, // Select port 0
    {0x6E, 0x8A, 0x00}, // BC_GPIO_CTL0: FrameSync signal to GPIO0
    {0x4c, 0x12, 0x00}, // Select port 1
    {0x6E, 0x8A, 0x00}, // BC_GPIO_CTL0: FrameSync signal to GPIO0
    {0x4c, 0x24, 0x00}, // Select port 2
    {0x6E, 0x8A, 0x00}, // BC_GPIO_CTL0: FrameSync signal to GPIO0
    {0x4c, 0x38, 0x00}, // Select port 3
    {0x6E, 0x8A, 0x00}, // BC_GPIO_CTL0: FrameSync signal to GPIO0
    {0x10, 0x91, 0x00}, // GPIO0 FrameSync signal; Output Source Select: Device Status; Output: Enabled
    {0x58, 0x5E, 0x00}, // Back Channel FREQ SELECT: 50 Mbps
    {0x19, 0x15, 0x00}, // FS_HIGH_TIME_1
    {0x1A, 0xB3, 0x00}, // FS_HIGH_TIME_0
    {0x1B, 0xC3, 0x00}, // FS_LOW_TIME_1
    {0x1C, 0x50, 0x00}, // FS_LOW_TIME_0

    {0xFFFF, 0x00, 0x0} // End of script
};

I2cParams ub960EnableFsync_SonyIMX728[2] = {
    {0x18, 0x01, 0x00}, // Enable Frame Sync
    {0xFFFF, 0x00, 0x0} // End of script
};

I2cParams ub960DisableFsync_SonyIMX728[2] = {
    {0x18, 0x00, 0x00}, // Enable Frame Sync
    {0xFFFF, 0x00, 0x0} // End of script
};

#ifdef ENABLE_IMX728_SER_TEST_PATTERN
I2cParams ub953SerTestPattern_SonyIMX728[IMX728_RAWTESTPAT_SER_CFG_SIZE] =
{
    {0x1F, 0x10, 0x1},  /* CSI_PLL_CTL Register (reserved) */
    {0xC9, 0x32, 0x1},  /* RESERVED */
    {0xB0, 0x1C, 0x1},  /* CSI-2 TX Reserved Registers */
    {0xB1, 0x92, 0x1},
    {0xB2, 0x40, 0x1},
    {0xB0, 0x03, 0x1},  /* Digital Page 0 Indirect Registers, IA_AUTO_INC=1 */
    {0xB1, 0x01, 0x1},  /* PGEN_CTL */
    {0xB2, 0x01, 0x1},  /* Enable Pattern Generator */
    {0xB2, 0xF3, 0x1},  /* PGEN_CFG: Send Fixed Color Pattern, 8 Color Bars, BLOCK_SIZE=3Bytes */
    {0xB2, 0x2C, 0x1},  /* PGEN_CSI_DI */
    {0xB2, IMX728_RAWTESTPAT_BPLN_HIGH, 0x1},                  /* PGEN_LINE_SIZE1 */
    {0xB2, IMX728_RAWTESTPAT_BPLN_LOW,  0x1},                  /* PGEN_LINE_SIZE0 */
    {0xB2, (((uint32_t)IMX728_RAWTESTPAT_BAR_SIZE & 0xFF00U) >> 8U), 0x1}, /* PGEN_BAR_SIZE1 */
    {0xB2, ((uint32_t)IMX728_RAWTESTPAT_BAR_SIZE & 0x00FFU),        0x1}, /* PGEN_BAR_SIZE0 */
    {0xB2, IMX728_RAWTESTPAT_OUT_HEIGHT_HIGH, 0x1},            /* PGEN_ACT_LPF1 */
    {0xB2, IMX728_RAWTESTPAT_OUT_HEIGHT_LOW,  0x1},            /* PGEN_ACT_LPF0 */
    {0xB2, 0x08, 0x1},  /* PGEN_TOT_LPF1 */
    {0xB2, 0x80, 0x1},  /* PGEN_TOT_LPF0 */
    {0xB2, 0x04, 0x1},  /* PGEN_LINE_PD1 */
    {0xB2, 0x7D, 0x1},  /* PGEN_LINE_PD0 */
    {0xB2, 0x07, 0x1},  /* PGEN_VBP */
    {0xB2, 0x08, 0x1},  /* PGEN_VFP */
    
    {0xFFFF, 0x00, 0x0} // End of script
};
#endif /* ENABLE_IMX728_SER_TEST_PATTERN */
#endif /* IMX728_SERDES_H */

