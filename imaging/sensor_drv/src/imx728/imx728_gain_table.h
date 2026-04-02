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
#ifndef IMX728_GAIN_TABLE_H_
#define IMX728_GAIN_TABLE_H_

#define ISS_IMX728_GAIN_TBL_SIZE           (421U)
/*
* \brief Gain Table for IMX728
*/
static const uint32_t gIMX728GainsTable[ISS_IMX728_GAIN_TBL_SIZE][2U] =
{
    {1024, 0x0}, \
    {1036, 0x1}, \
    {1048, 0x2}, \
    {1060, 0x3}, \
    {1072, 0x4}, \
    {1085, 0x5}, \
    {1097, 0x6}, \
    {1110, 0x7}, \
    {1123, 0x8}, \
    {1136, 0x9}, \
    {1149, 0xA}, \
    {1162, 0xB}, \
    {1176, 0xC}, \
    {1189, 0xD}, \
    {1203, 0xE}, \
    {1217, 0xF}, \
    {1231, 0x10}, \
    {1245, 0x11}, \
    {1260, 0x12}, \
    {1274, 0x13}, \
    {1289, 0x14}, \
    {1304, 0x15}, \
    {1319, 0x16}, \
    {1334, 0x17}, \
    {1350, 0x18}, \
    {1366, 0x19}, \
    {1381, 0x1A}, \
    {1397, 0x1B}, \
    {1414, 0x1C}, \
    {1430, 0x1D}, \
    {1446, 0x1E}, \
    {1463, 0x1F}, \
    {1480, 0x20}, \
    {1497, 0x21}, \
    {1515, 0x22}, \
    {1532, 0x23}, \
    {1550, 0x24}, \
    {1568, 0x25}, \
    {1586, 0x26}, \
    {1604, 0x27}, \
    {1623, 0x28}, \
    {1642, 0x29}, \
    {1661, 0x2A}, \
    {1680, 0x2B}, \
    {1699, 0x2C}, \
    {1719, 0x2D}, \
    {1739, 0x2E}, \
    {1759, 0x2F}, \
    {1780, 0x30}, \
    {1800, 0x31}, \
    {1821, 0x32}, \
    {1842, 0x33}, \
    {1863, 0x34}, \
    {1885, 0x35}, \
    {1907, 0x36}, \
    {1929, 0x37}, \
    {1951, 0x38}, \
    {1974, 0x39}, \
    {1997, 0x3A}, \
    {2020, 0x3B}, \
    {2043, 0x3C}, \
    {2067, 0x3D}, \
    {2091, 0x3E}, \
    {2115, 0x3F}, \
    {2139, 0x40}, \
    {2164, 0x41}, \
    {2189, 0x42}, \
    {2215, 0x43}, \
    {2240, 0x44}, \
    {2266, 0x45}, \
    {2292, 0x46}, \
    {2319, 0x47}, \
    {2346, 0x48}, \
    {2373, 0x49}, \
    {2400, 0x4A}, \
    {2428, 0x4B}, \
    {2456, 0x4C}, \
    {2485, 0x4D}, \
    {2514, 0x4E}, \
    {2543, 0x4F}, \
    {2572, 0x50}, \
    {2602, 0x51}, \
    {2632, 0x52}, \
    {2663, 0x53}, \
    {2693, 0x54}, \
    {2725, 0x55}, \
    {2756, 0x56}, \
    {2788, 0x57}, \
    {2820, 0x58}, \
    {2853, 0x59}, \
    {2886, 0x5A}, \
    {2919, 0x5B}, \
    {2953, 0x5C}, \
    {2987, 0x5D}, \
    {3022, 0x5E}, \
    {3057, 0x5F}, \
    {3092, 0x60}, \
    {3128, 0x61}, \
    {3164, 0x62}, \
    {3201, 0x63}, \
    {3238, 0x64}, \
    {3276, 0x65}, \
    {3314, 0x66}, \
    {3352, 0x67}, \
    {3391, 0x68}, \
    {3430, 0x69}, \
    {3470, 0x6A}, \
    {3510, 0x6B}, \
    {3551, 0x6C}, \
    {3592, 0x6D}, \
    {3633, 0x6E}, \
    {3675, 0x6F}, \
    {3718, 0x70}, \
    {3761, 0x71}, \
    {3805, 0x72}, \
    {3849, 0x73}, \
    {3893, 0x74}, \
    {3938, 0x75}, \
    {3984, 0x76}, \
    {4030, 0x77}, \
    {4077, 0x78}, \
    {4124, 0x79}, \
    {4172, 0x7A}, \
    {4220, 0x7B}, \
    {4269, 0x7C}, \
    {4318, 0x7D}, \
    {4368, 0x7E}, \
    {4419, 0x7F}, \
    {4470, 0x80}, \
    {4522, 0x81}, \
    {4574, 0x82}, \
    {4627, 0x83}, \
    {4681, 0x84}, \
    {4735, 0x85}, \
    {4790, 0x86}, \
    {4845, 0x87}, \
    {4901, 0x88}, \
    {4958, 0x89}, \
    {5015, 0x8A}, \
    {5073, 0x8B}, \
    {5132, 0x8C}, \
    {5192, 0x8D}, \
    {5252, 0x8E}, \
    {5313, 0x8F}, \
    {5374, 0x90}, \
    {5436, 0x91}, \
    {5499, 0x92}, \
    {5563, 0x93}, \
    {5627, 0x94}, \
    {5692, 0x95}, \
    {5758, 0x96}, \
    {5825, 0x97}, \
    {5893, 0x98}, \
    {5961, 0x99}, \
    {6030, 0x9A}, \
    {6100, 0x9B}, \
    {6170, 0x9C}, \
    {6242, 0x9D}, \
    {6314, 0x9E}, \
    {6387, 0x9F}, \
    {6461, 0xA0}, \
    {6536, 0xA1}, \
    {6611, 0xA2}, \
    {6688, 0xA3}, \
    {6766, 0xA4}, \
    {6844, 0xA5}, \
    {6923, 0xA6}, \
    {7003, 0xA7}, \
    {7084, 0xA8}, \
    {7166, 0xA9}, \
    {7249, 0xAA}, \
    {7333, 0xAB}, \
    {7418, 0xAC}, \
    {7504, 0xAD}, \
    {7591, 0xAE}, \
    {7679, 0xAF}, \
    {7768, 0xB0}, \
    {7858, 0xB1}, \
    {7949, 0xB2}, \
    {8041, 0xB3}, \
    {8134, 0xB4}, \
    {8228, 0xB5}, \
    {8323, 0xB6}, \
    {8420, 0xB7}, \
    {8517, 0xB8}, \
    {8616, 0xB9}, \
    {8716, 0xBA}, \
    {8817, 0xBB}, \
    {8919, 0xBC}, \
    {9022, 0xBD}, \
    {9126, 0xBE}, \
    {9232, 0xBF}, \
    {9339, 0xC0}, \
    {9447, 0xC1}, \
    {9557, 0xC2}, \
    {9667, 0xC3}, \
    {9779, 0xC4}, \
    {9892, 0xC5}, \
    {10007, 0xC6}, \
    {10123, 0xC7}, \
    {10240, 0xC8}, \
    {10359, 0xC9}, \
    {10479, 0xCA}, \
    {10600, 0xCB}, \
    {10723, 0xCC}, \
    {10847, 0xCD}, \
    {10972, 0xCE}, \
    {11099, 0xCF}, \
    {11228, 0xD0}, \
    {11358, 0xD1}, \
    {11489, 0xD2}, \
    {11623, 0xD3}, \
    {11757, 0xD4}, \
    {11893, 0xD5}, \
    {12031, 0xD6}, \
    {12170, 0xD7}, \
    {12311, 0xD8}, \
    {12454, 0xD9}, \
    {12598, 0xDA}, \
    {12744, 0xDB}, \
    {12891, 0xDC}, \
    {13041, 0xDD}, \
    {13192, 0xDE}, \
    {13344, 0xDF}, \
    {13499, 0xE0}, \
    {13655, 0xE1}, \
    {13813, 0xE2}, \
    {13973, 0xE3}, \
    {14135, 0xE4}, \
    {14299, 0xE5}, \
    {14464, 0xE6}, \
    {14632, 0xE7}, \
    {14801, 0xE8}, \
    {14973, 0xE9}, \
    {15146, 0xEA}, \
    {15321, 0xEB}, \
    {15499, 0xEC}, \
    {15678, 0xED}, \
    {15860, 0xEE}, \
    {16044, 0xEF}, \
    {16229, 0xF0}, \
    {16417, 0xF1}, \
    {16607, 0xF2}, \
    {16800, 0xF3}, \
    {16994, 0xF4}, \
    {17191, 0xF5}, \
    {17390, 0xF6}, \
    {17591, 0xF7}, \
    {17795, 0xF8}, \
    {18001, 0xF9}, \
    {18210, 0xFA}, \
    {18420, 0xFB}, \
    {18634, 0xFC}, \
    {18850, 0xFD}, \
    {19068, 0xFE}, \
    {19289, 0xFF}, \
    {19512, 0x100}, \
    {19738, 0x101}, \
    {19966, 0x102}, \
    {20198, 0x103}, \
    {20431, 0x104}, \
    {20668, 0x105}, \
    {20907, 0x106}, \
    {21149, 0x107}, \
    {21394, 0x108}, \
    {21642, 0x109}, \
    {21893, 0x10A}, \
    {22146, 0x10B}, \
    {22403, 0x10C}, \
    {22662, 0x10D}, \
    {22925, 0x10E}, \
    {23190, 0x10F}, \
    {23458, 0x110}, \
    {23730, 0x111}, \
    {24005, 0x112}, \
    {24283, 0x113}, \
    {24564, 0x114}, \
    {24848, 0x115}, \
    {25136, 0x116}, \
    {25427, 0x117}, \
    {25722, 0x118}, \
    {26020, 0x119}, \
    {26321, 0x11A}, \
    {26626, 0x11B}, \
    {26934, 0x11C}, \
    {27246, 0x11D}, \
    {27561, 0x11E}, \
    {27880, 0x11F}, \
    {28203, 0x120}, \
    {28530, 0x121}, \
    {28860, 0x122}, \
    {29194, 0x123}, \
    {29532, 0x124}, \
    {29874, 0x125}, \
    {30220, 0x126}, \
    {30570, 0x127}, \
    {30924, 0x128}, \
    {31282, 0x129}, \
    {31645, 0x12A}, \
    {32011, 0x12B}, \
    {32382, 0x12C}, \
    {32757, 0x12D}, \
    {33136, 0x12E}, \
    {33520, 0x12F}, \
    {33908, 0x130}, \
    {34300, 0x131}, \
    {34698, 0x132}, \
    {35099, 0x133}, \
    {35506, 0x134}, \
    {35917, 0x135}, \
    {36333, 0x136}, \
    {36754, 0x137}, \
    {37179, 0x138}, \
    {37610, 0x139}, \
    {38045, 0x13A}, \
    {38486, 0x13B}, \
    {38931, 0x13C}, \
    {39382, 0x13D}, \
    {39838, 0x13E}, \
    {40300, 0x13F}, \
    {40766, 0x140}, \
    {41238, 0x141}, \
    {41716, 0x142}, \
    {42199, 0x143}, \
    {42687, 0x144}, \
    {43182, 0x145}, \
    {43682, 0x146}, \
    {44188, 0x147}, \
    {44699, 0x148}, \
    {45217, 0x149}, \
    {45740, 0x14A}, \
    {46270, 0x14B}, \
    {46806, 0x14C}, \
    {47348, 0x14D}, \
    {47896, 0x14E}, \
    {48451, 0x14F}, \
    {49012, 0x150}, \
    {49579, 0x151}, \
    {50153, 0x152}, \
    {50734, 0x153}, \
    {51322, 0x154}, \
    {51916, 0x155}, \
    {52517, 0x156}, \
    {53125, 0x157}, \
    {53740, 0x158}, \
    {54363, 0x159}, \
    {54992, 0x15A}, \
    {55629, 0x15B}, \
    {56273, 0x15C}, \
    {56925, 0x15D}, \
    {57584, 0x15E}, \
    {58251, 0x15F}, \
    {58925, 0x160}, \
    {59607, 0x161}, \
    {60298, 0x162}, \
    {60996, 0x163}, \
    {61702, 0x164}, \
    {62417, 0x165}, \
    {63139, 0x166}, \
    {63870, 0x167}, \
    {64610, 0x168}, \
    {65358, 0x169}, \
    {66115, 0x16A}, \
    {66881, 0x16B}, \
    {67655, 0x16C}, \
    {68438, 0x16D}, \
    {69231, 0x16E}, \
    {70033, 0x16F}, \
    {70843, 0x170}, \
    {71664, 0x171}, \
    {72494, 0x172}, \
    {73333, 0x173}, \
    {74182, 0x174}, \
    {75041, 0x175}, \
    {75910, 0x176}, \
    {76789, 0x177}, \
    {77678, 0x178}, \
    {78578, 0x179}, \
    {79488, 0x17A}, \
    {80408, 0x17B}, \
    {81339, 0x17C}, \
    {82281, 0x17D}, \
    {83234, 0x17E}, \
    {84198, 0x17F}, \
    {85173, 0x180}, \
    {86159, 0x181}, \
    {87157, 0x182}, \
    {88166, 0x183}, \
    {89187, 0x184}, \
    {90219, 0x185}, \
    {91264, 0x186}, \
    {92321, 0x187}, \
    {93390, 0x188}, \
    {94471, 0x189}, \
    {95565, 0x18A}, \
    {96672, 0x18B}, \
    {97791, 0x18C}, \
    {98924, 0x18D}, \
    {100069, 0x18E}, \
    {101228, 0x18F}, \
    {102400, 0x190}, \
    {103586, 0x191}, \
    {104785, 0x192}, \
    {105999, 0x193}, \
    {107226, 0x194}, \
    {108468, 0x195}, \
    {109724, 0x196}, \
    {110994, 0x197}, \
    {112279, 0x198}, \
    {113580, 0x199}, \
    {114895, 0x19A}, \
    {116225, 0x19B}, \
    {117571, 0x19C}, \
    {118932, 0x19D}, \
    {120310, 0x19E}, \
    {121703, 0x19F}, \
    {123112, 0x1A0}, \
    {124537, 0x1A1}, \
    {125980, 0x1A2}, \
    {127438, 0x1A3}, \
    {128914, 0x1A4}
};

#endif /* IMX623ES3_GAIN_TABLE_H_ */
