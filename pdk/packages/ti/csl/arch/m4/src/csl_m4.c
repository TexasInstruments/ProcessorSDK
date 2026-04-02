/*
 *  Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   csl_r5.c
 *
 *  \brief  Pulsar/R5 SS related functions.
 **/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stddef.h>
#include <csl_arm_m4.h>

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */
#define M4_CPUID_REG_ADDR  (0xE000ED00U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
void CSL_armM4GetCpuID( CSL_ArmM4CPUInfo *cpuInfo )
{
    uint32_t regVal;

    regVal = HW_RD_REG32(M4_CPUID_REG_ADDR);
    cpuInfo->varient  = (uint32_t)((regVal & CSL_M4_CPUID_VARIENT_MASK) >>
                                          CSL_M4_CPUID_VARIENT_SHIFT);
    cpuInfo->partno   = (uint32_t)((regVal & CSL_M4_CPUID_PARTNO_MASK) >>
                                          CSL_M4_CPUID_PARTNO_SHIFT);
    cpuInfo->revision = (uint32_t)(
                                (regVal & CSL_M4_CPUID_REVISION_MASK) >>
                                          CSL_M4_CPUID_REVISION_SHIFT);
}

/********************************* End of file ******************************/
