/*
 * Copyright (C) 2016-2022 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   current_monitor_test.c
 *
 *  \brief  Current Monitor diagnostic test file
 *
 *  Targeted Functionality: Verification of basic functionality of
 *  Current Monitor interface.
 *
 *  Operation: This test is used for detection of each INA device connected,
 *  verification of INA device configuration by measuring the shunt voltage,
 *  bus voltage, power, current readings and displaying it on to the serial
 *  console.
 *
 *  Supported SoCs: K2G, AM65XX, J721E, J7200, TPR12, AM64x, AWR294x, J721S2
 *
 *  Supported Platforms: iceK2G, am65xx_evm, am65xx_idk, j721e_evm, j7200_evm,
 *                       tpr12_evm, am64x_evm, am64x_svb, awr294x_evm, j721s2_evm
 *
 */

#include "current_monitor_test.h"

extern I2C_config_list I2C_config;

#if defined(am65xx_evm) || defined(am65xx_idk)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_CORE",      0x40, {0.002, 0.0025, 1.25, 305.17, 0.000152, 16777}},
    {"VDD_MCU",       0x41, {0.01, 0.0025, 1.25,  38.14,  0.0000305, 16777}},
    {"VDD_MPU",       0x42, {0.002, 0.0025, 1.25, 167.84,  0.0000915, 27962}},
    {"SoC_DVDD3V3",   0x43, {0.002, 0.0025, 1.25,  95.36,  0.0000915, 27962}},
    {"SoC_DVDD1V8",   0x44, {0.01, 0.0025, 1.25, 152.58,  0.0000915, 5592}},
    {"SoC_AVDD1V8",   0x45, {0.01, 0.0025, 1.25,  38.14,  0.0000122, 41943}},
    {"SoC_VDDS_DDR",  0x46, {0.01, 0.0025, 1.25,  26.70,  0.0000610, 8388}},
    {"VDD_DDR",       0x47, {0.01, 0.0025, 1.25,  86.21,  0.0000610, 8388}}
};
#elif defined(SOC_J721E)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_MCU_0V85",        0x40U, {0.01, 0.0025, 1.25, 0.000839, 0.0000336, 15252}},
    {"VDD_MCU_RAM_0V85",    0x41U, {0.01, 0.0025, 1.25,  0.000198,  0.0000079, 64528}},
    {"VDA_MCU_1V8",         0x42U, {0.01, 0.0025, 1.25, 0.000198,  0.0000079, 64528}},
    {"VDD_MCUIO_3V3",       0x43U, {0.01, 0.0025, 1.25,  0.000198,  0.0000079, 64528}},
    {"VDD_MCUIO_1V8",       0x44U, {0.01, 0.0025, 1.25, 0.000763,  0.0000305, 16777}},
    {"VDD_CORE_0V8",        0x45U, {0.01, 0.0025, 1.25,  0.008392,  0.0003357, 1525}},
    {"VDD_CORE_RAM_0V85",   0x46U, {0.01, 0.0025, 1.25,  0.000381,  0.0000153, 33554}},
    {"VDD_CPU_RAM_0V85",    0x47U, {0.01, 0.0025, 1.25,  0.000305,  0.0000122, 41943}},
    {"VDD_CPU_AVS",         0x48U, {0.01, 0.0025, 1.25, 0.00534, 0.0002136, 2397}},
    {"VDDR_BIAS_1V1",       0x49U, {0.01, 0.0025, 1.25,  0.000198,  0.0000079, 64528}},
    {"VDDR_IO_DV",          0x4AU, {0.01, 0.0025, 1.25, 0.000267,  0.0000107, 47935}},
    {"VDD_PHYCORE_0V8",     0x4BU, {0.01, 0.0025, 1.25,  0.000198,  0.0000079, 64528}},
    {"VDA_PLL_1V8",         0x4CU, {0.01, 0.0025, 1.25, 0.000198,  0.0000079, 64528}},
    {"VDD_PHYIO_1V8",       0x4DU, {0.01, 0.0025, 1.25,  0.000687,  0.0000275, 18641}},
    {"VDA_USB_3V3",         0x4EU, {0.01, 0.0025, 1.25,  0.000198,  0.0000079, 64528}},
    {"VDD_IO_1V8",          0x40U, {0.01, 0.0025, 1.25,  0.000229,  0.0000092, 55924}},
    {"VDD_IO_3V3",          0x41U, {0.01, 0.0025, 1.25, 0.000763, 0.0000305, 16777}},
    {"VDD_SD_DV",           0x42U, {0.01, 0.0025, 1.25,  0.000198,  0.0000079, 64528}},
    {"VDD1",                0x43U, {0.01, 0.0025, 1.25, 0.000381,  0.0000153, 33554}},
    {"VDD2",                0x44U, {0.01, 0.0025, 1.25,  0.000381,  0.0000153, 33554}},
    {"VDDQ_LPDDR4_DV",      0x45U, {0.01, 0.0025, 1.25, 0.000198,  0.0000079, 64527}},
    {"VSYS_MCUIO_1V8",      0x46U, {0.01, 0.0025, 1.25,  0.000381,  0.0000153, 33554}},
    {"VSYS_MCUIO_3V3",      0x47U, {0.01, 0.0025, 1.25,  0.000198,  0.0000079, 64528}},
    {"VSYS_IO_1V8",         0x48U, {0.01, 0.0025, 1.25,  0.000229,  0.0000092, 55924}},
    {"VSYS_IO_3V3",         0x49U, {0.01, 0.0025, 1.25, 0.000381, 0.0000153, 33554}},
    {"VCC_12V0",            0x4AU, {0.01, 0.0025, 1.25,  0.006104,  0.0002441, 2097}},
    {"VSYS_5V0",            0x4BU, {0.01, 0.0025, 1.25, 0.003815,  0.0001526, 3355}},
    {"VSYS_3V3",            0x4CU, {0.01, 0.0025, 1.25,  0.0015259,  0.0000610, 8389}},
    {"VSYS_3V3_SOM",        0x4DU, {0.01, 0.0025, 1.25, 0.006104,  0.0002441, 2097}},
    {"VDDA_DLL_0V8",        0x4EU, {0.01, 0.0025, 1.25,  0.0001983,  0.0000079, 64528}},
    {"EXP_3V3",             0x4FU, {0.01, 0.0025, 1.25,  0.007629,  0.0003052, 1677}}
};
#elif defined(SOC_J721S2)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_MCU_0V85",        0x40U, {0.01,  0.0025, 1.25, 0.001028442, 0.0000411, 12446}},
    {"VDD_MCU_RAM_0V85",    0x41U, {0.01,  0.0025, 1.25, 0.000190735, 0.0000076, 64528}},
    {"VDA_MCU_1V8",         0x42U, {0.01,  0.0025, 1.25, 4.42505E-05, 0.0000018, 64528}},
    {"VDD_MCUIO_3V3",       0x43U, {0.01,  0.0025, 1.25, 3.96729E-05, 0.0000016, 64528}},
    {"VDD_MCUIO_1V8",       0x44U, {0.01,  0.0025, 1.25, 0.0002388,   0.0000096, 53601}},
    {"VDD_CORE_0V8",        0x45U, {0.005, 0.0025, 1.25, 0.003561401, 0.0001425, 7188}},
    {"VDD_CORE_RAM_0V85",   0x46U, {0.01,  0.0025, 1.25, 0.000244904, 0.0000098, 52265}},
    {"VDD_WK_0V8",          0x47U, {0.01,  0.0025, 1.25, 2.28882E-05, 0.0000009, 64528}},
    {"VDD_CPU_AVS",         0x48U, {0.01,  0.0025, 1.25, 0.003318787, 0.0001328, 3856}},
    {"VDDR_BIAS_1V1",       0x49U, {0.01,  0.0025, 1.25, 0.000117493, 0.0000047, 64528}},
    {"VDDR_IO_DV",          0x4AU, {0.01,  0.0025, 1.25, 0.000117493, 0.0000047, 47935}},
    {"VDD_PHYCORE_0V8",     0x4BU, {0.01,  0.0025, 1.25, 0.00021286,  0.0000085, 60133}},
    {"VDA_PLL_1V8",         0x4CU, {0.01,  0.0025, 1.25, 1.60217E-05, 0.0000006, 64528}},
    {"VDD_PHYIO_1V8",       0x4DU, {0.01,  0.0025, 1.25, 6.63757E-05, 0.0000027, 18641}},
    {"VDA_USB_3V3",         0x4EU, {0.01,  0.0025, 1.25, 7.62939E-06, 0.0000003, 64528}},
    {"VDD_GPIORET_3V3",     0x4FU, {0.01,  0.0025, 1.25, 0.000122833, 0.0000049, 64528}},
    {"VDD_IO_1V8",          0x40U, {0.01,  0.0025, 1.25, 9.91821E-05, 0.0000040, 55924}},
    {"VDD_IO_3V3",          0x41U, {0.01,  0.0025, 1.25, 0.000127411, 0.0000051, 16777}},
    {"VDD_SD_DV",           0x42U, {0.01,  0.0025, 1.25, 2.28882E-05, 0.0000009, 64528}},
    {"VDD1",                0x43U, {0.01,  0.0025, 1.25, 4.27246E-05, 0.0000017, 33554}},
    {"VDD2",                0x44U, {0.01,  0.0025, 1.25, 0.000976563, 0.0000391, 13107}},
    {"VDDQ_LPDDR4_DV",      0x45U, {0.01,  0.0025, 1.25, 0.000190735, 0.0000076, 64527}},
    {"VSYS_MCUIO_1V8",      0x46U, {0.01,  0.0025, 1.25, 0.00038147,  0.0000153, 33554}},
    {"VSYS_MCUIO_3V3",      0x47U, {0.01,  0.0025, 1.25, 0.000198364, 0.0000079, 64527}},
    {"VSYS_IO_1V8",         0x48U, {0.01,  0.0025, 1.25, 0.000686646, 0.0000275, 18641}},
    {"VSYS_IO_3V3",         0x49U, {0.01,  0.0025, 1.25, 0.001144409, 0.0000458, 11184}},
    {"VCC_12V0",            0x4AU, {0.01,  0.0025, 1.25, 0.006103516, 0.0002441, 2097}},
    {"VSYS_5V0",            0x4BU, {0.01,  0.0025, 1.25, 0.003814697, 0.0001526, 3355}},
    {"VSYS_3V3",            0x4CU, {0.005, 0.0025, 1.25, 0.004959106, 0.0001984, 5162}},
    {"VSYS_3V3_SOM",        0x4DU, {0.001, 0.0025, 1.25, 0.006103516, 0.0002441, 20972}},
    {"VDDA_DLL_0V8",        0x4EU, {0.01,  0.0025, 1.25, 1.2207E-05,  0.0000005, 64528}},
    {"EXP_3V3",             0x4FU, {0.01,  0.0025, 1.25, 0.007629395, 0.0003052, 1677}}
};
#elif defined(SOC_J784S4)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_MCU_0V85",        0x40U, {0.01,  0.0025, 1.25, 0.001028442, 0.0000411, 12446}},
    {"VDD_MCU_RAM_0V85",    0x41U, {0.01,  0.0025, 1.25, 9.91821E-05, 0.0000040, 55924}},
    {"VDA_MCU_1V8",         0x42U, {0.01,  0.0025, 1.25, 4.42505E-05, 0.0000018, 64528}},
    {"VDD_MCUIO_3V3",       0x43U, {0.01,  0.0025, 1.25, 3.96729E-05, 0.0000016, 64528}},
    {"VDD_MCUIO_1V8",       0x44U, {0.01,  0.0025, 1.25, 0.0002388,   0.0000096, 53601}},
    {"VDD_RAM_0V85",        0x46U, {0.01,  0.0025, 1.25, 0.000244904, 0.0000098, 52265}},
    {"VDD_GPIORET_WK_0V8",  0x47U, {0.01,  0.0025, 1.25, 2.28882E-05, 0.0000009, 64528}},
    {"VSYS_GPIORET_IO_3V3", 0x49U, {0.01,  0.0025, 1.25, 0.000117493, 0.0000047, 64528}},
/*
    {"VDD_CORE_0V8",        0x45U, {0.005, 0.0025, 1.25, 0.003561401, 0.0001425, 7188}},
    {"VDD_CPU_AVS",         0x48U, {0.01,  0.0025, 1.25, 0.003318787, 0.0001328, 3856}},
    {"VDD_DDR_1V1",         0x4AU, {0.01,  0.0025, 1.25, 0.000117493, 0.0000047, 47935}},
*/ //Reserved for future use. Not enabled in current design
    {"VDD_PHYCORE_0V8",     0x4BU, {0.01,  0.0025, 1.25, 0.00021286,  0.0000085, 60133}},
    {"VDA_PLL_1V8",         0x4CU, {0.01,  0.0025, 1.25, 1.60217E-05, 0.0000006, 64528}},
    {"VDA_PHY_1V8",         0x4DU, {0.01,  0.0025, 1.25, 6.63757E-05, 0.0000027, 18641}},
    {"VDA_USB_3V3",         0x4EU, {0.01,  0.0025, 1.25, 7.62939E-06, 0.0000003, 64528}},
    {"VDD_GPIORET_IO_3V3",  0x4FU, {0.01,  0.0025, 1.25, 0.000122833, 0.0000049, 64528}},
    {"VDD_IO_1V8",          0x40U, {0.01,  0.0025, 1.25, 0.000127411, 0.0000051, 16777}},
    {"VDD_IO_3V3",          0x41U, {0.01,  0.0025, 1.25, 2.28882E-05, 0.0000009, 64528}},
    {"VDD_SD_DV",           0x42U, {0.01,  0.0025, 1.25, 4.27246E-05, 0.0000017, 33554}},
    {"VDD1_DDR_1V8",        0x43U, {0.01,  0.0025, 1.25, 0.000976563, 0.0000391, 13107}},
    {"VCCA_3V3_CORE",       0x45U, {0.01,  0.0025, 1.25, 0.00038147,  0.0000153, 33554}},
    {"VSYS_MCUIO_1V8",      0x46U, {0.01,  0.0025, 1.25, 0.000198364, 0.0000079, 64527}},
    {"VSYS_MCUIO_3V3",      0x47U, {0.01,  0.0025, 1.25, 0.000686646, 0.0000275, 18641}},
    {"VSYS_IO_1V8",         0x48U, {0.01,  0.0025, 1.25, 0.001144409, 0.0000458, 11184}},
    {"VSYS_IO_3V3",         0x49U, {0.01,  0.0025, 1.25, 0.006103516, 0.0002441, 2097}},
    {"VCC_12V0",            0x4AU, {0.01,  0.0025, 1.25, 0.003814697, 0.0001526, 3355}},
    {"VSYS_5V0",            0x4BU, {0.005, 0.0025, 1.25, 0.004959106, 0.0001984, 5162}},
    {"VSYS_3V3",            0x4CU, {0.001, 0.0025, 1.25, 0.006103516, 0.0002441, 20972}},
    {"VCCA_3V3_DDR",        0x4DU, {0.01,  0.0025, 1.25, 0.007629395, 0.0003052, 1677}},
    {"VDA_DLL_0V8",         0x4EU, {0.01,  0.0025, 1.25, 1.2207E-05,  0.0000005, 64528}},
    {"VCCA_3V3_CPU_AVS",    0x4FU, {0.01,  0.0025, 1.25, 0.007629395, 0.0003052, 1677}}
//    {"VDD_DDR_SOC_1V1",     0x44U, {0.01,  0.0025, 1.25, 0.000190735, 0.0000076, 64527}}, //Reserved for future use. Not enabled in current design
};
#elif defined(SOC_J7200)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_MCU_0V85",        0x40U, {0.01,  0.0025, 1.25, 0.001028442, 0.0000411, 12446}},
    {"VDD_MCU_RAM_0V85",    0x41U, {0.01,  0.0025, 1.25, 0.000190735, 0.0000076, 64528}},
    {"VDA_MCU_1V8",         0x42U, {0.01,  0.0025, 1.25, 4.42505E-05, 0.0000018, 64528}},
    {"VDD_MCUIO_3V3",       0x43U, {0.01,  0.0025, 1.25, 3.96729E-05, 0.0000016, 64528}},
    {"VDD_MCUIO_1V8",       0x44U, {0.01,  0.0025, 1.25, 0.0002388,   0.0000096, 53601}},
    {"VDD_CORE_0V8",        0x45U, {0.005, 0.0025, 1.25, 0.003561401, 0.0001425, 7188}},
    {"VDD_CORE_RAM_0V85",   0x46U, {0.01,  0.0025, 1.25, 0.000244904, 0.0000098, 52265}},
    {"VDD_WK_0V8",          0x47U, {0.01,  0.0025, 1.25, 2.28882E-05, 0.0000009, 64528}},
    {"VDD_CPU_AVS",         0x48U, {0.01,  0.0025, 1.25, 0.003318787, 0.0001328, 3856}},
    {"VDDR_BIAS_1V1",       0x49U, {0.01,  0.0025, 1.25, 0.000117493, 0.0000047, 64528}},
    {"VDDR_IO_DV",          0x4AU, {0.01,  0.0025, 1.25, 0.000117493, 0.0000047, 47935}},
    {"VDD_PHYCORE_0V8",     0x4BU, {0.01,  0.0025, 1.25, 0.00021286,  0.0000085, 60133}},
    {"VDA_PLL_1V8",         0x4CU, {0.01,  0.0025, 1.25, 1.60217E-05, 0.0000006, 64528}},
    {"VDD_PHYIO_1V8",       0x4DU, {0.01,  0.0025, 1.25, 6.63757E-05, 0.0000027, 18641}},
    {"VDA_USB_3V3",         0x4EU, {0.01,  0.0025, 1.25, 7.62939E-06, 0.0000003, 64528}},
    {"VDD_GPIORET_3V3",     0x4FU, {0.01,  0.0025, 1.25, 0.000122833, 0.0000049, 64528}},
    {"VDD_IO_1V8",          0x40U, {0.01,  0.0025, 1.25, 9.91821E-05, 0.0000040, 55924}},
    {"VDD_IO_3V3",          0x41U, {0.01,  0.0025, 1.25, 0.000127411, 0.0000051, 16777}},
    {"VDD_SD_DV",           0x42U, {0.01,  0.0025, 1.25, 2.28882E-05, 0.0000009, 64528}},
    {"VDD1",                0x43U, {0.01,  0.0025, 1.25, 4.27246E-05, 0.0000017, 33554}},
    {"VDD2",                0x44U, {0.01,  0.0025, 1.25, 0.000976563, 0.0000391, 13107}},
    {"VDDQ_LPDDR4_DV",      0x45U, {0.01,  0.0025, 1.25, 0.000190735, 0.0000076, 64527}},
    {"VSYS_MCUIO_1V8",      0x46U, {0.01,  0.0025, 1.25, 0.00038147,  0.0000153, 33554}},
    {"VSYS_MCUIO_3V3",      0x47U, {0.01,  0.0025, 1.25, 0.000198364, 0.0000079, 64527}},
    {"VSYS_IO_1V8",         0x48U, {0.01,  0.0025, 1.25, 0.000686646, 0.0000275, 18641}},
    {"VSYS_IO_3V3",         0x49U, {0.01,  0.0025, 1.25, 0.001144409, 0.0000458, 11184}},
    {"VCC_12V0",            0x4AU, {0.01,  0.0025, 1.25, 0.006103516, 0.0002441, 2097}},
    {"VSYS_5V0",            0x4BU, {0.01,  0.0025, 1.25, 0.003814697, 0.0001526, 3355}},
    {"VSYS_3V3",            0x4CU, {0.005, 0.0025, 1.25, 0.004959106, 0.0001984, 5162}},
    {"VSYS_3V3_SOM",        0x4DU, {0.001, 0.0025, 1.25, 0.006103516, 0.0002441, 20972}},
    {"VDDA_DLL_0V8",        0x4EU, {0.01,  0.0025, 1.25, 1.2207E-05,  0.0000005, 64528}},
    {"EXP_3V3",             0x4FU, {0.01,  0.0025, 1.25, 0.007629395, 0.0003052, 1677}}
};
#elif defined(SOC_TPR12)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_1V2",      0x40, {0.002, 0.0025, 1.25, 0.001373291, 0.0000549, 46603}},
    {"VIOIN_1V8", 	 0x41, {0.002, 0.0025, 1.25, 0.000991821, 0.0000397, 64528}},
    {"VIOIN_3V3",    0x44, {0.002, 0.0025, 1.25, 0.001068115, 0.0000427, 59919}},
    {"VDD_SRAM_1V2", 0x45, {0.002, 0.0025, 1.25, 0.000991821, 0.0000397, 64528}}
};
#elif defined(SOC_AWR294X)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"INA_1V2",      0x40, {0.001, 0.0025, 1.25, 0.002746582, 0.0001099, 46603}},
    {"INA_1V8", 	 0x41, {0.001, 0.0025, 1.25, 0.001983643, 0.0000793, 64528}},
    {"INA_1V0RF1",   0x42, {0.001, 0.0025, 1.25, 0.00213623, 0.0000854, 59919}},
    {"INA_1V0RF2",   0x43, {0.001, 0.0025, 1.25, 0.00213623, 0.0000854, 59919}},
    {"INA_3V3",      0x44, {0.001, 0.0025, 1.25, 0.002212524, 0.0000885, 57852}}

};
#elif defined(am64x_evm) 
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_CORE",      0x40, {0.002, 0.0025, 1.25, 0.001804352, 0.0000722, 35469}},
    {"VDDR_CORE",     0x41, {0.01,  0.0025, 1.25, 0.000198364, 0.0000079, 64527}},
    {"VDDS_DDR",      0x46, {0.01,  0.0025, 1.25, 0.000196838, 0.0000079, 65027}},
    {"SoC_DVDD1V8",   0x4B, {0.01,  0.0025, 1.25, 0.000268555, 0.0000107, 47662}},
    {"SoC_DVDD3V3",   0x4C, {0.01,  0.0025, 1.25, 0.0002388,   0.0000096, 53601}},
    {"SoC_AVDD1V8",   0x4E, {0.01,  0.0025, 1.25, 0.001266479, 0.0000507, 10106}}
};
#elif defined(am64x_svb)
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_CORE",         0x40, {0.02, 0.0025, 1.25, 0.001879119873, 0.0000752, 3547}},
    {"VDDR_CORE",        0x41, {0.1, 0.0025, 1.25, 0.0001724243164, 0.0000069, 7424}},
    {"MCU_WKUP_1V8",     0x45, {0.1, 0.0025, 1.25, 0.0003814697266, 0.0000153, 3355}},
    {"VDDS_DDR",         0x46, {0.1, 0.0025, 1.25, 0.0001525878906, 0.0000061, 8389}},
    {"VDD_DIMM",         0x47, {0.1, 0.0025, 1.25, 0.00008850097656, 0.0000035, 14463}},
    {"MCU_WKUP_3V3",     0x4A, {0.1, 0.0025, 1.25, 0.0003814697266,	0.0000153, 3355}},
    {"SOC_1V8",          0x4B, {0.05, 0.0025, 1.25, 0.0004577636719, 0.0000183, 5592}},
    {"SOC_3V3",          0x4C, {0.05, 0.0025, 1.25, 0.0003967285156, 0.0000159, 6453}},
    {"MCU_WKUP_1V8_LDO", 0x4D, {0.1, 0.0025, 1.25, 0.00001907348633, 0.0000008, 41943}}, //AM64X_SVB_TODO: Need to update this Value
    {"SOC_1V8_LDO",      0x4E, {0.1, 0.0025, 1.25, 0.00005340576172, 0.0000021, 23968}},
    {"SOC_3V3_LDO",      0x4F, {0.1, 0.0025, 1.25, 0.00003051757813, 0.0000012, 41943}}
};
#else
/* TODO: Need to update the values for iceK2G */
inaCfgObj_t inaDevice[NUM_OF_INA_DEVICES] = {
    {"VDD_CORE",      0x40, {25, 0.0025, 1.25, 305.17, 122.07}},
    {"VDD_MCU",       0x41, {25, 0.0025, 1.25,  38.14,  15.25}},
    {"VDD_MPU",       0x43, {25, 0.0025, 1.25, 167.84,  67.13}},
    {"VDD_DDR",       0x44, {25, 0.0025, 1.25,  86.21,  34.48}}
};
#endif

/**
 *  \brief    Reads the specified current monitor register of the given
 *            INA Slave Device.
 *
 *  \param    handle       [IN]  I2C Handler
 *            slaveAddress [IN]  INA Device Slave Address
 *            regAddr      [IN]  Register address of a INA slave device
 *            *rdRegval    [OUT] Pointer to store the read register value.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_read_register(I2C_Handle handle,
                                      uint8_t slaveAddress,
                                      uint8_t regAddr,
                                      uint16_t *rdRegval)
{
    int8_t  ret = 0;
    uint8_t  rx[2];
    I2C_Transaction transaction;

    /* Initializes the I2C transaction structure with default values */
    I2C_transactionInit(&transaction);

    transaction.slaveAddress = slaveAddress;
    transaction.writeBuf     = &regAddr;
    transaction.writeCount   = 1;
    transaction.readBuf      = NULL;
    transaction.readCount    = 0;

    ret = I2C_transfer(handle, &transaction);
    if(I2C_STS_SUCCESS != ret)
    {
        ret = -1;
        return ret;
    }

    BOARD_delay(1000);

    transaction.writeBuf     = NULL;
    transaction.writeCount   = 0;
    transaction.readBuf      = rx;
    transaction.readCount    = 2;

    ret = I2C_transfer(handle, &transaction);
    if(I2C_STS_SUCCESS != ret)
    {
        ret = -1;
        return ret;
    }
    else
    {
        ret = 0;
    }

    /* Note:- Slave device responds with MSB first for the read sequence sent */
    *rdRegval = ((((uint16_t)rx[0]) << 8) | ((uint16_t)rx[1]));

    return ret;
}

/**
 *  \brief    Writes the specified current monitor register of the given
 *            INA Slave Device.
 *
 *  \param    handle       [IN] I2C Handler
 *            slaveAddress [IN] INA Device Slave Address
 *            regAddr      [IN] Register address of a INA slave device
 *            regData      [IN] Data to be configured into the register.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_write_register(I2C_Handle handle,
                                       uint8_t slaveAddress,
                                       uint8_t regAddr,
                                       uint16_t regData)
{
    int8_t ret = 0;
    uint8_t tx[3];
    I2C_Transaction transaction;

    /* Initializes the I2C transaction structure with default values */
    I2C_transactionInit(&transaction);

    transaction.slaveAddress = slaveAddress;
    transaction.writeBuf     = &tx[0];
    transaction.writeCount   = 3;
    transaction.readBuf      = NULL;
    transaction.readCount    = 0;

    tx[0] = regAddr;

    /* MSB of 16-bit data should be sent first followed by the LSB */
    tx[1] = (uint8_t)((regData & 0xFF00) >> 8);
    tx[2] = (uint8_t)(regData & 0x00FF);

    BOARD_delay(1000);

    ret = I2C_transfer(handle, &transaction);
    if(I2C_STS_SUCCESS != ret)
    {
        ret = -1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

/**
 *  \brief    Used to calibrate the specified INA slave device by writing to
 *            INA calibration register.
 *
 *  \param    handle     [IN] I2C Handler
 *            slaveAddr  [IN] INA Device Slave Address
 *            currentLSB [IN] Current LSB value
 *            maxRShunt  [IN] The RShunt is the resistance of the
 *                            external shunt used to develop the
 *                            differential voltage across the input pins.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_set_calibration(I2C_Handle handle,
                                        uint8_t slaveAddr,
                                        float currentLSB,
                                        uint8_t maxRShunt,
                                        uint16_t calibration)
{
    int8_t ret = 0;

    ret = BoardDiag_write_register(handle,
                                   slaveAddr,
                                   CALIBRATION_REG_ADDR_OFFSET,
                                   calibration);

    return ret;
}

/**
 *  \brief    Used to configure the specified INA slave device by writing
 *            to configuration register.
 *
 *  \param    handle              [IN]   I2C Handler
 *            slaveAddr           [IN]   INA Device Slave Address.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_set_configuration(I2C_Handle handle,
                                          uint8_t slaveAddr)
{
    int8_t ret = 0;

    ret = BoardDiag_write_register(handle,
                                   slaveAddr,
                                   CONFIGURATION_REG_ADDR_OFFSET,
                                   (uint16_t)DEFAULT_CONFIG_REG_VAL);

    return ret;
}

/**
 *  \brief    Used to read shunt voltage register and converts it into actual
 *            shunt voltage reading.
 *
 *  \param    handle              [IN]      I2C Handler
 *            slaveAddr           [IN]      INA Device Slave Address
 *            shuntVolLsb         [IN]      Shunt voltage LSB value
 *            *shuntVolMeasured   [OUT]     Pointer to store the shunt voltage
 *                                          measured.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_read_shunt_voltage(I2C_Handle handle,
                                           uint8_t slaveAddr,
                                           float shuntVolLsb,
                                           uint16_t *shuntVolMeasured)
{
    int8_t ret = 0;
    uint16_t rdShuntRegVal = 0;

    ret = BoardDiag_read_register(handle,
                                  slaveAddr,
                                  SHUNT_VOLTAGE_REG_ADDR_OFFSET,
                                  &rdShuntRegVal);
    if(0 != ret)
    {
        return ret;
    }
    else
    {
        *shuntVolMeasured = ((rdShuntRegVal & SHUNT_VOLTAGE_REG_MASK) * shuntVolLsb);
    }

    return ret;
}

/**
 *  \brief    Used to read bus voltage register and converts it into actual
 *            bus voltage reading.
 *
 *  \param    handle              [IN]      I2C Handler
 *            slaveAddr           [IN]      INA Device Slave Address
 *            busVolLsb           [IN]      Bus voltage LSB value
 *            *busVolMeasured     [OUT]     Pointer to store the bus voltage
 *                                          measured.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_read_bus_voltage(I2C_Handle handle,
                                         uint8_t slaveAddr,
                                         float busVolLsb,
                                         uint16_t *busVolMeasured)
{
    int8_t ret = 0;
    uint16_t rdBusVolRegVal = 0;

    ret = BoardDiag_read_register(handle,
                                  slaveAddr,
                                  BUS_VOLTAGE_REG_ADDR_OFFSET,
                                  &rdBusVolRegVal);
    if(0 != ret)
    {
        return ret;
    }
    else
    {
        *busVolMeasured = ((rdBusVolRegVal & BUS_VOLTAGE_REG_MASK) * busVolLsb);
    }

    return ret;
}

/**
 *  \brief    Used to read power register and converts it into actual
 *            power reading.
 *
 *  \param    handle              [IN]     I2C Handler
 *            slaveAddr           [IN]     INA Device Slave Address
 *            powerLsb            [IN]     Power LSB value
 *            *powerMeasured      [OUT]    Pointer to store the power
 *                                         measured.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_read_power(I2C_Handle handle,
                                   uint8_t slaveAddr,
                                   float powerLsb,
                                   uint16_t *powerMeasured)
{
    int8_t ret = 0;
    uint16_t rdPowerRegVal = 0;

    ret = BoardDiag_read_register(handle,
                                  slaveAddr,
                                  POWER_REG_ADDR_OFFSET,
                                  &rdPowerRegVal);
    if(0 != ret)
    {
        return ret;
    }
    else
    {
        *powerMeasured = ((rdPowerRegVal & POWER_REG_MASK) * powerLsb);
    }

    return ret;
}

/**
 *  \brief    Used to read current register and converts it into actual
 *            current reading.
 *
 *  \param    handle              [IN]      I2C Handler
 *            slaveAddr           [IN]      INA Device Slave Address
 *            currentLsb          [IN]      Current LSB value
 *            *currentMeasured    [OUT]     Pointer to store the current
 *                                          measured.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_read_current(I2C_Handle handle,
                                     uint8_t slaveAddr,
                                     float currentLsb,
                                     float *currentMeasured)
{
    int8_t ret = 0;
    uint16_t rdCurrentRegVal = 0;

    ret = BoardDiag_read_register(handle,
                                  slaveAddr,
                                  CURRENT_REG_ADDR_OFFSET,
                                  &rdCurrentRegVal);
    if(0 != ret)
    {
        return ret;
    }
    else
    {
        *currentMeasured = ((rdCurrentRegVal & CURRENT_REG_MASK) * currentLsb);
    }

    return ret;
}

#if defined(j721s2_evm)
/**
 * \brief   This function enables the Mux for I2C instance controlling
 *          current monitor devices
 *
 */
static void BoardDiag_configI2CMux(void)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status = BOARD_SOK;

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_1;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(BOARD_SOK != status)
    {
        UART_printf("Failed to enable the I2C mux selection\n");
    }

    ioExpCfg.pinNum      = PIN_NUM_2;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(BOARD_SOK != status)
    {
        UART_printf("Failed to enable the I2C mux selection\n");
    }

    ioExpCfg.pinNum      = PIN_NUM_3;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(BOARD_SOK != status)
    {
        UART_printf("Failed to enable the I2C mux selection\n");
    }
}

#elif defined(j784s4_evm)

static void BoardDiag_configI2CMux(void)
{

    Board_STATUS status = BOARD_SOK;
    status = Board_control(BOARD_CTRL_CMD_SET_IO_MUX_PORTB2, NULL);
    if(BOARD_SOK != status)
    {
        UART_printf("Failed to enable the port2  mux selection\n");
    }
}


#endif  /* #if defined(j721e_evm) */

/**
 *  \brief    This function is used to run the Current Monitor diagnostic test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
static int8_t BoardDiag_run_current_monitor_test(void)
{
    int8_t ret = 0;
    uint8_t index;
    uint16_t rdData = 0;
    float current;

    I2C_Params i2cParams;
    I2C_HwAttrs i2cConfig;
    I2C_Handle handle = NULL;

    for(index = 0; index < I2C_HWIP_MAX_CNT; index++)
    {
        I2C_socGetInitCfg(index, &i2cConfig);
        i2cConfig.enableIntr = BFALSE;
        I2C_socSetInitCfg(index, &i2cConfig);
    }

    /* Initializes the I2C */
    I2C_init();

    /* Initializes the I2C Parameters */
    I2C_Params_init(&i2cParams);
#if (defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_TPR12) || defined(SOC_AWR294X) || defined(SOC_J721S2) || defined(SOC_J784S4))
    i2cParams.bitRate = I2C_400kHz;
#endif
    /* Configures the I2C instance with the passed parameters*/
    handle = I2C_open(BOARD_I2C_CURRENT_MONITOR_INSTANCE,
                      &i2cParams);
    if(NULL == handle)
    {
        UART_printf("\nI2C Open failed!\n");
        ret = -1;
        return ret;
    }

    for(index = 0; index < NUM_OF_INA_DEVICES; index++)
    {
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)
        if (TOT_INA_IN_PM1 == index)
        {
             /* Selecting PM2 I2C */
		    GPIO_write(0U, SIGNAL_LEVEL_HIGH);
        }
#endif
        UART_printf("\n\nVerifying Device %s at Address - 0x%x\n",
                     inaDevice[index].deviceID,
                     inaDevice[index].slaveAddr);

        UART_printf("Setting the configuration register...\n\r");
        ret = BoardDiag_set_configuration(handle,
                                          inaDevice[index].slaveAddr);
        if(0 != ret)
        {
            UART_printf("Unable to set the configuration register\n\r");
            break;
        }

        UART_printf("Setting the calibration register...\n\r");
        ret = BoardDiag_set_calibration(handle,
                                        inaDevice[index].slaveAddr,
                                        inaDevice[index].inaCalParams.currentLsb,
                                        (uint8_t)inaDevice[index].inaCalParams.maxRShunt,
                                        inaDevice[index].inaCalParams.calibrartion);
        if(0 != ret)
        {
            UART_printf("Unable to set the calibration register\n\r");
            break;
        }

        UART_printf("Reading the Shunt Voltage register...\n\r");
        ret = BoardDiag_read_shunt_voltage(handle,
                                           inaDevice[index].slaveAddr,
                                           inaDevice[index].inaCalParams.shuntVolLsb,
                                           &rdData);
        if(0 != ret)
        {
            UART_printf("Failed to read the Shunt Voltage register\n\r");
            break;
        }
        else
        {
            UART_printf("Shunt Voltage = %dmV\n", rdData);
        }

        UART_printf("Reading the Bus Voltage register...\n\r");
        ret = BoardDiag_read_bus_voltage(handle,
                                         inaDevice[index].slaveAddr,
                                         inaDevice[index].inaCalParams.busVolLsb,
                                         &rdData);
        if(0 != ret)
        {
            UART_printf("Failed to read the Bus Voltage register\n\r");
            break;
        }
        else
        {
            UART_printf("Bus Voltage = %dmV\n", rdData);
        }

        UART_printf("Reading the Power register...\n\r");
        ret = BoardDiag_read_power(handle,
                                   inaDevice[index].slaveAddr,
                                   inaDevice[index].inaCalParams.powerLsb,
                                   &rdData);
        if(0 != ret)
        {
            UART_printf("Failed to read the Power register\n\r");
            break;
        }
        else
        {
            UART_printf("Power = %dmW\n", rdData);
        }

        UART_printf("Reading the Current register...\n\r");
        ret = BoardDiag_read_current(handle,
                                     inaDevice[index].slaveAddr,
                                     inaDevice[index].inaCalParams.currentLsb,
                                     &current);
        if(0 != ret)
        {
            UART_printf("Failed to read the Current register\n\r");
            break;
        }
        else
        {
            rdData = current * 1000;
            UART_printf("Current = %dmA\n", rdData);
        }
    }

    /* Closing the current monitor i2c instance */
    I2C_close(handle);

    return ret;
}

int8_t BoardDiag_currentMonitorFuctionalTest(void)
{
    int8_t ret = 0;
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2)  || defined(SOC_J784S4)

    Board_I2cInitCfg_t i2cCfg;
#if!defined(j784s4_evm)
    i2cIoExpSignalLevel_t signalLev = GPIO_SIGNAL_LEVEL_LOW;
#endif
#endif

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)

#if !defined (__aarch64__)
	enableI2C(CSL_I2C0_CFG_BASE);
#endif

    i2cCfg.i2cInst   = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    i2cCfg.socDomain = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);
	
    /* I2C Mux select */
    Board_i2cIoExpInit();

    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  THREE_PORT_IOEXP,
                                  PORTNUM_1,
                                  PIN_NUM_0, /* CTRL_PM_I2C_OE# */
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling PM bus switch to high for accessing the INA devices */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              THREE_PORT_IOEXP,
                              PORTNUM_1,
                              PIN_NUM_0,
                              (i2cIoExpSignalLevel_t) SIGNAL_LEVEL_HIGH);

#if !defined(j784s4_evm)
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  THREE_PORT_IOEXP,
                                  PORTNUM_1,
                                  PIN_NUM_3, /* MLB_MUX_SEL */
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the select input line to low to select port 1 */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              THREE_PORT_IOEXP,
                              PORTNUM_1,
                              PIN_NUM_3,
                              signalLev);

    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                                  THREE_PORT_IOEXP,
                                  PORTNUM_1,
                                  PIN_NUM_4,
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the select input line to low to select port 1 */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE2_ADDR,
                              THREE_PORT_IOEXP,
                              PORTNUM_1,
                              PIN_NUM_4,
                              signalLev);
#endif
    Board_i2cIoExpDeInit();


#if defined(j784s4_evm)
    GPIO_v0_HwAttrs gpioCfg;
    GPIO_socGetInitCfg(0, &gpioCfg);
    gpioCfg.baseAddr = CSL_WKUP_GPIO0_BASE;
    GPIO_socSetInitCfg(0, &gpioCfg);
#endif

    GPIO_init();
    /* Selecting PM1 I2C */
    GPIO_write(0U, SIGNAL_LEVEL_LOW);
#endif
    ret = BoardDiag_run_current_monitor_test();

    return ret;
}

#ifdef DIAG_STRESS_TEST
/**
 *  \brief    The function performs the Current Monitor Diagnostic
 *            stress test.
 *
 *  \return   int8_t
 *               0 - in case of success
 *              -1 - in case of failure.
 *
 */
int8_t BoardDiag_currentMonitorStressTest(void)
{
    int8_t ret = 0;
    char rdBuf = 'y';
    uint32_t iteration;
    uint32_t passCount = 0;
    uint32_t failCount = 0;

    UART_printf("\n\nRunning Current Monitor Test in Stress Mode for %d Number of Times...\n", DIAG_STRESS_TEST_ITERATIONS);
    UART_printf("Enter 'b' in Serial Console to Terminate the Test\n\n");

    for(iteration = 1; iteration <= DIAG_STRESS_TEST_ITERATIONS; iteration++)
    {
        ret = BoardDiag_currentMonitorFuctionalTest();
        if (0 == ret)
        {
            UART_printf("\n\n\n\nIteration : %d Current Monitor Test Passed\n",iteration);
            passCount++;
        }
        else
        {
            UART_printf("\n\n\n\nCurrent Monitor Test Failed at iteration - %d\n",iteration);
            failCount++;
        }

        /* Check if there a input from console to break the test */
        rdBuf = (char)BoardDiag_getUserInput(BOARD_UART_INSTANCE);
        if(('b' == rdBuf) || ('B' == rdBuf))
        {
            UART_printf("Received Test Termination... Exiting the Test\n");
            iteration++;
            break;
        }
    }

    UART_printf("\n\nCurrent Monitor Stress Test Status\n");
    UART_printf("===================================\n");
    UART_printf("Number of Times Executed - %d\n", (iteration - 1));
    UART_printf("Pass Count - %d\n", passCount);
    UART_printf("Fail Count - %d\n", failCount);

    if((0 != iteration) && (0 == failCount))
    {
        UART_printf("Overall Status - PASS\n\n\n" );
    }
    else
    {
        UART_printf("Overall Status - FAIL\n\n\n" );
        ret = -1;
    }

    return ret;
}
#endif

/**
 *  \brief   Current Monitor Diagnostic test main function
 *
 *  \return  int - Current Monitor Diagnostic test status.
 *             0 - in case of success
 *            -1 - in case of failure.
 *
 */
#if (defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_TPR12) || defined(SOC_AM64X) || defined(SOC_AWR294X) || defined(SOC_J721S2) || defined(SOC_J784S4))
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;
    int8_t ret = 0;

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return -1;
    }

#if defined(SOC_AM65XX) && !defined (__aarch64__)
    /* MCU I2C instance will be active by default for R5 core.
     * Need update HW attrs to enable MAIN I2C instance.
     */
	enableMAINI2C(2, CSL_I2C2_CFG_BASE);
#endif

#if (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2)) && !defined (__aarch64__)
    /* MCU I2C instance will be active by default for R5 core.
     * Need update HW attrs to enable MAIN I2C instance.
     */
	enableMAINI2C(BOARD_I2C_CURRENT_MONITOR_INSTANCE, CSL_I2C2_CFG_BASE);
#endif

#if defined(SOC_J784S4) && !defined (__aarch64__)
    enableMAINI2C(BOARD_I2C_CURRENT_MONITOR_INSTANCE, CSL_I2C1_CFG_BASE);
#endif

    UART_printf("\n**********************************************\n");
    UART_printf  ("*            Current Monitor Test            *\n");
    UART_printf  ("**********************************************\n");

#if defined(j721s2_evm) || defined(j784s4_evm)
    /* Enable the Mux for routing I2C lines */
    BoardDiag_configI2CMux();
#endif

#ifdef DIAG_STRESS_TEST
    ret = BoardDiag_currentMonitorStressTest();
#else
    ret = BoardDiag_currentMonitorFuctionalTest();
#endif
    if(0 == ret)
    {
        UART_printf("\nCurrent Monitor Test Passed\n");
    }
    else
    {
        UART_printf("\nCurrent Monitor Test Failed\n");
    }

    return ret;
}
#else
int main(void)
{
    Board_STATUS status;
    Board_initCfg boardCfg;

#ifdef SOC_K2G
    DIAG_IntrInit();
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
                BOARD_INIT_PINMUX_CONFIG |
                BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO;
#endif
    status = Board_init(boardCfg);
    if(BOARD_SOK != status)
    {
        return -1;
    }

    UART_printf("\n**********************************************\n");
    UART_printf  ("*            Current Monitor Test            *\n");
    UART_printf  ("**********************************************\n");

/* Current monitor is available only on Alpha ICE board */
#ifdef defined (ICE_K2G) && defined (K2G_ICE_ALPHA)
    UART_printf("Unsupported Test!!\n");
    return 1;
#else
    return BoardDiag_currentMonitorFuctionalTest();
#endif
}
#endif
