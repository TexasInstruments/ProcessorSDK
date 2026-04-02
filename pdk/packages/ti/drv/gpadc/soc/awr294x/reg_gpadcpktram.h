/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *
 *  \file drv/gpadc/soc/awr294x/reg_gpadc.h
 *
 *  \brief
 *     The gpadc/soc/awr294x/reg_gpadc.h has the register layer definitons for the
 *     GPADC Module.
 *
 *
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
#ifndef REG_GPADCPKTRAM_H
#define REG_GPADCPKTRAM_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************
 */
#ifdef __cplusplus
extern "C" {
#endif

#define M_TWO_HUNDRED_FIFTY_SIX     (256U)

/*---------------------------------------------------------------------------------------
 * INST0_0 -- Offset == 0x000
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_ConfigValue              : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCPKTRAM_INST0;

/*---------------------------------------------------------------------------------------
 * INST0_1 -- Offset == 0x004
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b8_Param                     :  8;           /*!< bits   7:  0 */
        uint32_t b8_CollectSamples            :  8;           /*!< bits  15:  8 */
        uint32_t b7_SkipSamples               :  7;           /*!< bits  22: 16 */
        uint32_t b1_ChirpBrk                  :  1;           /*!< bits  23: 23 */
        uint32_t b1_Nu1                       :  1;           /*!< bits  24: 24 */
        uint32_t b7_Nu2                       :  7;           /*!< bits  31: 25 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCPKTRAM_INST1;

/*---------------------------------------------------------------------------------------
 * REG_GROUPS
 *---------------------------------------------------------------------------------------
 */
typedef struct
{
    U_GPADCPKTRAM_INST0                          r_Inst0        ;        /* Offset = 0x000 */
    U_GPADCPKTRAM_INST1                          r_Inst1        ;        /* Offset = 0x004 */
} T_GPADCPKTRAM_INST;

/*---------------------------------------------------------------------------------------
 * BSS_GPADC_PKT_RAM_REGS
 *---------------------------------------------------------------------------------------
 */
typedef volatile struct
{
    T_GPADCPKTRAM_INST                 r_Inst[M_TWO_HUNDRED_FIFTY_SIX];        /* Offset = 0x000 */
} T_GPADCPKTRAM_REGS;

/*---------------------------------------------------------------------------------------
 * BSS_GPADC_OUTPUT_RAM
 *---------------------------------------------------------------------------------------
 */
/*---------------------------------------------------------------------------------------
 * REG0 -- Offset == 0x0
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b10_MinGpadc                 : 10;           /*!< bits   9:  0 */
        uint32_t b6_Nu1                       :  6;           /*!< bits  15: 10 */
        uint32_t b10_MaxGpadc                 : 10;           /*!< bits  25: 16 */
        uint32_t b6_Nu2                       :  6;           /*!< bits  31: 26 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCRAM_REG0;

/*---------------------------------------------------------------------------------------
 * REG1 -- Offset == 0x4
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b20_SumCtm                   : 20;           /*!< bits  19:  0 */
        uint32_t b12_Nu                       : 12;           /*!< bits  31: 20 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCRAM_REG1;

/*---------------------------------------------------------------------------------------
 * REG_GROUPS
 *---------------------------------------------------------------------------------------
 */
typedef struct
{
    U_GPADCRAM_REG0                          r_MinMax        ;        /* Offset = 0x000 */
    U_GPADCRAM_REG1                          r_Sum           ;        /* Offset = 0x004 */
} T_GPADCOUT_REG;

/*---------------------------------------------------------------------------------------
 * BSS_GPADC_OUT_RAM_REGS
 *---------------------------------------------------------------------------------------
 */
typedef volatile struct
{
    T_GPADCOUT_REG                 r_Result[M_TWO_HUNDRED_FIFTY_SIX];  /* Offset = 0x000 */
} T_GPADCOUT_RAM;


#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF REG_GPADCPKTRAM_H
 */

