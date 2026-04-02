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
#ifndef OX05C1S_GAIN_TABLE_H_
#define OX05C1S_GAIN_TABLE_H_

#define ISS_OX05C1S_GAIN_TBL_SIZE ((sizeof gOX05C1SGainsTable) / (sizeof gOX05C1SGainsTable[0]))

typedef struct {
    uint16_t gain;
    uint8_t intVal;
    uint8_t decVal;
} OX05C1S_gain_table_t;

/*
 * \brief Gain Table for OX05C1S
 */
static const OX05C1S_gain_table_t gOX05C1SGainsTable[] =
{
    /* example gain table for X3F; must be updated for your own sensor */
    /* 1.0x00 - 1.9375 */
    {1055, 0x01, 0x00},
    {1119, 0x01, 0x01},
    {1183, 0x01, 0x02},
    {1247, 0x01, 0x03},
    {1311, 0x01, 0x04},
    {1375, 0x01, 0x05},
    {1439, 0x01, 0x06},
    {1503, 0x01, 0x07},
    {1567, 0x01, 0x08},
    {1631, 0x01, 0x09},
    {1695, 0x01, 0x0A},
    {1759, 0x01, 0x0B},
    {1823, 0x01, 0x0C},
    {1887, 0x01, 0x0D},
    {1951, 0x01, 0x0E},
    {2015, 0x01, 0x0F},
    /* 2.000 - 3.875 */
    {2011, 0x02, 0x00},
    {2239, 0x02, 0x02},
    {2367, 0x02, 0x04},
    {2495, 0x02, 0x06},
    {2623, 0x02, 0x08},
    {2751, 0x02, 0x0A},
    {2879, 0x02, 0x0C},
    {3007, 0x02, 0x0E},
    {3135, 0x03, 0x00},
    {3263, 0x03, 0x02},
    {3391, 0x03, 0x04},
    {3519, 0x03, 0x06},
    {3647, 0x03, 0x08},
    {3775, 0x03, 0x0A},
    {3903, 0x03, 0x0C},
    {4031, 0x03, 0x0E},
    /* 4.00 - 7.75 */
    {4223, 0x04, 0x00},
    {4479, 0x04, 0x04},
    {4735, 0x04, 0x08},
    {4991, 0x04, 0x0C},
    {5247, 0x05, 0x00},
    {5503, 0x05, 0x04},
    {5759, 0x05, 0x08},
    {6015, 0x05, 0x0C},
    {6271, 0x06, 0x00},
    {6527, 0x06, 0x04},
    {6783, 0x06, 0x08},
    {7039, 0x06, 0x0C},
    {7295, 0x07, 0x00},
    {7551, 0x07, 0x04},
    {7807, 0x07, 0x08},
    {8063, 0x07, 0x0C},
    /* 8.0 - 15.5 */
    {8447, 0x08, 0x00},
    {8959, 0x08, 0x08},
    {9471, 0x09, 0x00},
    {9983, 0x09, 0x08},
    {10495, 0x0A, 0x00},
    {11007, 0x0A, 0x08},
    {11519, 0x0B, 0x00},
    {12031, 0x0B, 0x08},
    {12543, 0x0C, 0x00},
    {13055, 0x0C, 0x08},
    {13567, 0x0D, 0x00},
    {14079, 0x0D, 0x08},
    {14591, 0x0E, 0x00},
    {15103, 0x0E, 0x08},
    {15615, 0x0F, 0x00},
    {15872, 0x0F, 0x08}
};

#endif /* OX05C1S_GAIN_TABLE_H_ */
