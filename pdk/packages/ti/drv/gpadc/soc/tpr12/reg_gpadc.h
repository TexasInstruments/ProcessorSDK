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
 *  \file drv/gpadc/soc/tpr12/reg_gpadc.h
 *
 *  \brief
 *     The gpadc/soc/tpr12/reg_gpadc.h has the register layer definitons for the
 *     GPADC Module.
 *
 *
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
#ifndef REG_GPADC_H
#define REG_GPADC_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************
 */
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
 * MACROS
 ****************************************************************************************
 */

/*! \brief
 * Register Structure member write and compare macro
 */
#define M_REG_STRUCT_SWRITE(w_hwRegStruct, w_regVal, w_regWrSts)   \
                do { (w_hwRegStruct) = (w_regVal); \
                     M_REG32_SCOMPARE((w_hwRegStruct), (w_regVal), (w_regWrSts)); \
                   } while (0)

/*! \brief
 * 32 bit register compare macro
 */
#define M_REG32_SCOMPARE(w_hwVal, w_swVal, w_regWrSts)  \
                do { \
                     (w_regWrSts) |= (((uint32_t)(w_swVal)) ^ ((uint32_t)(w_hwVal))); \
                   } while (0)

/*! \brief
 * Register Structure member CLEAR and compare macro
 */
#define M_REG_STRUCT_SCLEAR(w_hwRegStruct, w_regVal, w_regWrSts)   \
                do { (w_hwRegStruct) = (w_regVal); \
                     (w_regWrSts) |= ((((uint32_t)(w_hwRegStruct)) & (uint32_t)(w_regVal))); \
                   } while (0)

/*! \brief
 * Register readback test result
 */
#define M_ASSERT_REGSWRITE_RESULT(w_regWrSts)   DebugP_assert(w_regWrSts == M_ZERO)

/*
 * Software assert macro, In debug mode which would display assert info in logger and
 * also send the LR info to MAC itrough MB.
 */
#define M_ASSERT(test)      DebugP_assert(test)

#define M_NULL                 (0x0U)
#define M_ZERO                 (0U)
/*---------------------------------------------------------------------------------------
 * REG0 -- Offset == 0x000
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b2_DcbistMode                :  2;           /*!< bits   1:  0 */
        uint32_t b6_Nu1                       :  6;           /*!< bits   7:  2 */
        uint32_t b1_GpadcFsmClkEnable         :  1;           /*!< bits   8:  8 */
        uint32_t b3_Gpadc2adcbufPathEn        :  3;           /*!< bits  11:  9 */
        uint32_t b4_Nu2                       :  4;           /*!< bits  15: 12 */
        uint32_t b1_GpadcDebugModeEnable      :  1;           /*!< bits  16: 16 */
        uint32_t b15_Nu3                      : 15;           /*!< bits  31: 17 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG0;

/*---------------------------------------------------------------------------------------
 * REG1 -- Offset == 0x004
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b1_GpadcTrigger              :  1;           /*!< bits   0:  0 */
        uint32_t b7_Nu1                       :  7;           /*!< bits   7:  1 */
        uint32_t b1_GpadcInit                 :  1;           /*!< bits   8:  8 */
        uint32_t b7_Nu2                       :  7;           /*!< bits  15:  9 */
        uint32_t b1_GpadcFsmBypass            :  1;           /*!< bits  16: 16 */
        uint32_t b7_Nu3                       :  7;           /*!< bits  23: 17 */
        uint32_t b1_GpadcStartBypVal          :  1;           /*!< bits  24: 24 */
        uint32_t b7_Nu4                       :  7;           /*!< bits  31: 25 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG1;

/*---------------------------------------------------------------------------------------
 * REG2 -- Offset == 0x008
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_ConfigValueIfm           : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG2;

/*---------------------------------------------------------------------------------------
 * REG3 -- Offset == 0x00C
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b8_ParamValIfm               :  8;           /*!< bits   7:  0 */
        uint32_t b8_CollectSamplesIfm         :  8;           /*!< bits  15:  8 */
        uint32_t b7_SkipSamplesIfm            :  7;           /*!< bits  22: 16 */
        uint32_t b9_Nu                        :  9;           /*!< bits  31: 23 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG3;

/*---------------------------------------------------------------------------------------
 * REG4 -- Offset == 0x010
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b8_PktRamBaseAddrCp0         :  8;           /*!< bits   7:  0 */
        uint32_t b8_PktRamBaseAddrCp1         :  8;           /*!< bits  15:  8 */
        uint32_t b8_PktRamBaseAddrCp2         :  8;           /*!< bits  23: 16 */
        uint32_t b8_PktRamBaseAddrCp3         :  8;           /*!< bits  31: 24 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG4;

/*---------------------------------------------------------------------------------------
 * REG5 -- Offset == 0x014
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b8_PktRamBaseAddrCp4         :  8;           /*!< bits   7:  0 */
        uint32_t b8_PktRamBaseAddrCp5         :  8;           /*!< bits  15:  8 */
        uint32_t b8_PktRamBaseAddrCp6         :  8;           /*!< bits  23: 16 */
        uint32_t b8_PktRamBaseAddrCp7         :  8;           /*!< bits  31: 24 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG5;

/*---------------------------------------------------------------------------------------
 * REG6 -- Offset == 0x018
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b8_PktRamBaseAddrCp8         :  8;           /*!< bits   7:  0 */
        uint32_t b8_PktRamBaseAddrCp9         :  8;           /*!< bits  15:  8 */
        uint32_t b8_PktRamBaseAddrCp10        :  8;           /*!< bits  23: 16 */
        uint32_t b8_PktRamBaseAddrCp11        :  8;           /*!< bits  31: 24 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG6;

/*---------------------------------------------------------------------------------------
 * REG7 -- Offset == 0x01C
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b8_PktRamBaseAddrCp12        :  8;           /*!< bits   7:  0 */
        uint32_t b8_PktRamBaseAddrCp13        :  8;           /*!< bits  15:  8 */
        uint32_t b8_PktRamBaseAddrCp14        :  8;           /*!< bits  23: 16 */
        uint32_t b8_PktRamBaseAddrCp15        :  8;           /*!< bits  31: 24 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG7;

/*---------------------------------------------------------------------------------------
 * REG8 -- Offset == 0x020
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b8_GpadcClkDiv               :  8;           /*!< bits   7:  0 */
        uint32_t b1_GpadcClkEnable            :  1;           /*!< bits   8:  8 */
        uint32_t b23_Nu                       : 23;           /*!< bits  31:  9 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG8;

/*---------------------------------------------------------------------------------------
 * REG9 -- Offset == 0x024
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_ParamNotUsedTxEna1Off    : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG9;

/*---------------------------------------------------------------------------------------
 * REG10 -- Offset == 0x028
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_ParamNotUsedTxEna2Off    : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG10;

/*---------------------------------------------------------------------------------------
 * REG11 -- Offset == 0x02C
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_ParamNotUsedTxEna3Off    : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG11;

/*---------------------------------------------------------------------------------------
 * REG12 -- Offset == 0x030
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b1_DramEccEnable             :  1;           /*!< bits   0:  0 */
        uint32_t b7_Nu1                       :  7;           /*!< bits   7:  1 */
        uint32_t b1_DramEccErrClr             :  1;           /*!< bits   8:  8 */
        uint32_t b7_Nu2                       :  7;           /*!< bits  15:  9 */
        uint32_t b8_DramEccErrAddr            :  8;           /*!< bits  23: 16 */
        uint32_t b8_DramRepairedBit           :  8;           /*!< bits  31: 24 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG12;

/*---------------------------------------------------------------------------------------
 * REG13 -- Offset == 0x034
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_SpareWr2                 : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG13;

/*---------------------------------------------------------------------------------------
 * REG14 -- Offset == 0x038
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b20_SumIfm                   : 20;           /*!< bits  19:  0 */
        uint32_t b12_Nu                       : 12;           /*!< bits  31: 20 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG14;

/*---------------------------------------------------------------------------------------
 * REG15 -- Offset == 0x03C
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
} U_GPADCREG_REG15;

/*---------------------------------------------------------------------------------------
 * REG16 -- Offset == 0x040
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b1_GpadcMemInitDoneStat      :  1;           /*!< bits   0:  0 */
        uint32_t b31_Nu                       : 31;           /*!< bits  31:  1 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG16;

/*---------------------------------------------------------------------------------------
 * REG17 -- Offset == 0x044
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b1_GpadcIfmDoneStatus        :  1;           /*!< bits   0:  0 */
        uint32_t b31_Nu                       : 31;           /*!< bits  31:  1 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG17;

/*---------------------------------------------------------------------------------------
 * REG18 -- Offset == 0x048
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b1_GpadcIfmDoneClr           :  1;           /*!< bits   0:  0 */
        uint32_t b31_Nu                       : 31;           /*!< bits  31:  1 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG18;

/*---------------------------------------------------------------------------------------
 * REG19 -- Offset == 0x04C
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b16_GpadcSamplesFrame        : 16;           /*!< bits  15:  0 */
        uint32_t b16_Nu                       : 16;           /*!< bits  31: 16 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG19;

/*---------------------------------------------------------------------------------------
 * REG20 -- Offset == 0x050
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_SpareRd1                 : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG20;

/*---------------------------------------------------------------------------------------
 * REG21 -- Offset == 0x054
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_SpareRd2                 : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG21;

/*---------------------------------------------------------------------------------------
 * REG22 -- Offset == 0x058
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b32_SpareWr1                 : 32;           /*!< bits  31:  0 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_GPADCREG_REG22;



/*---------------------------------------------------------------------------------------
 * MSS_GPADC_REG_REGS
 *---------------------------------------------------------------------------------------
 */
typedef volatile struct
{
    U_GPADCREG_REG0                    r_Reg0              ;        /* Offset = 0x000 */
    U_GPADCREG_REG1                    r_Reg1              ;        /* Offset = 0x004 */
    U_GPADCREG_REG2                    r_Reg2              ;        /* Offset = 0x008 */
    U_GPADCREG_REG3                    r_Reg3              ;        /* Offset = 0x00C */
    U_GPADCREG_REG4                    r_PacketRamAdd[4]   ;        /* Offset = 0x010 - 0x1C */
    U_GPADCREG_REG8                    r_Reg8              ;        /* Offset = 0x020 */
    U_GPADCREG_REG9                    r_Reg9              ;        /* Offset = 0x024 */
    U_GPADCREG_REG10                   r_Reg10             ;        /* Offset = 0x028 */
    U_GPADCREG_REG11                   r_Reg11             ;        /* Offset = 0x02C */
    U_GPADCREG_REG12                   r_Reg12             ;        /* Offset = 0x030 */
    U_GPADCREG_REG13                   r_Reg13             ;        /* Offset = 0x034 */
    U_GPADCREG_REG14                   r_Reg14             ;        /* Offset = 0x038 */
    U_GPADCREG_REG15                   r_Reg15             ;        /* Offset = 0x03C */
    U_GPADCREG_REG16                   r_Reg16             ;        /* Offset = 0x040 */
    U_GPADCREG_REG17                   r_Reg17             ;        /* Offset = 0x044 */
    U_GPADCREG_REG18                   r_Reg18             ;        /* Offset = 0x048 */
    U_GPADCREG_REG19                   r_Reg19             ;        /* Offset = 0x04C */
    U_GPADCREG_REG20                   r_Reg20             ;        /* Offset = 0x050 */
    U_GPADCREG_REG21                   r_Reg21             ;        /* Offset = 0x054 */
    U_GPADCREG_REG22                   r_Reg22             ;        /* Offset = 0x058 */
} T_GPADC_REGS;

/*---------------------------------------------------------------------------------------
 * MSS_GPADC_RST_CTRL_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b3_Assert                    :   3;           /*!< bits   2: 0 */
        uint32_t b29_Nu1                      :  29;         /*!< bits  31: 3 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_GPADC_RST_CTRL_REG;

/*---------------------------------------------------------------------------------------
 * MSS_GPADC_CLK_DIV_VAL_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b24_Clkdivr                    :  24;           /*!< bits   23: 0 */
        uint32_t b8_Nu1                         :  8;         /*!< bits  31: 24 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_GPADC_CLK_DIV_VAL_REG;

/*---------------------------------------------------------------------------------------
 * MSS_GPADC_CLK_GATE_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b3_Gated                     :   3;           /*!< bits   2: 0 */
        uint32_t b29_Nu1                      :  29;         /*!< bits  31: 3 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_GPADC_CLK_GATE_REG;


/*---------------------------------------------------------------------------------------
 * MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b30_Reserved1                     : 30;           /*!< bits    0: 29 */
    	uint32_t b1_ClkTmuxEsdCtrl                 :  1;           /*!< bits   30:  30 */
        uint32_t b1_AnaTestEn                      :  1;           /*!< bits   31:  31 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_REG;

/*---------------------------------------------------------------------------------------
 * MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b30_Reserved1                     : 31;           /*!< bits    0: 30 */
    	uint32_t b1_AnaogTestTmuxEsdCtrl           :  1;           /*!< bits   31:  31 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_REG;

/*---------------------------------------------------------------------------------------
 * MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
    	uint32_t b1_AdcEn                          :  1;           /*!< bits   0:  0 */
        uint32_t b1_AdcStartConv                   :  1;           /*!< bits   1:  1 */
        uint32_t b1_AdcReset                       :  1;           /*!< bits   2:  2 */
        uint32_t b1_AdcInpBufEn                    :  1;           /*!< bits   3:  3 */
        uint32_t b1_AdcRefBufEn                    :  1;           /*!< bits   4:  4 */
        uint32_t b3_AdcRefSel_2_0                  :  3;           /*!< bits   7:  5 */
        uint32_t b1_TsDiffInpBufEn                 :  1;           /*!< bits   8:  8 */
        uint32_t b1_TsSeInpBufEn                   :  1;           /*!< bits   9:  9 */
        uint32_t b1_IforceExtCtrl                  :  1;           /*!< bits  10: 10 */
        uint32_t b1_VrefExtCtrl                    :  1;           /*!< bits  11: 11 */
        uint32_t b1_VinExtCtrl                     :  1;           /*!< bits  12: 12 */
        uint32_t b1_AnaTmuxBufBypass               :  1;           /*!< bits  13: 13 */
        uint32_t b1_AnaTmuxBufEn                   :  1;           /*!< bits  14: 14 */
        uint32_t b5_RtrimTw_4_0                    :  5;           /*!< bits  19: 15 */
        uint32_t b12_Reserved1                     : 12;           /*!< bits  31: 20 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_REG;


/*---------------------------------------------------------------------------------------
 * MSS_CTRL_GPADC_CTRL_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b1_gpadc_sw_trig             :   1;         /*!< bits   0: 0 */
        uint32_t b7_Reserved1                 :   7;         /*!< bits   7: 1 */
        uint32_t b5_gpadc_trigin_sel          :   5;         /*!< bits  12: 8 */
        uint32_t b19_Reserved2                :  19;         /*!< bits  31: 13 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_CTRL_GPADC_CTRL_REG;

/*---------------------------------------------------------------------------------------
 * MSS_CTRL_MSS_GPADC_MEM_INIT_REG
 *---------------------------------------------------------------------------------------
 */
typedef union
{
    struct
    {
        uint32_t b1_mem0_init                 :   1;         /*!< bits   0: 0 */
        uint32_t b31_Reserved                 :  31;         /*!< bits  31: 1 */
    } bits;  /*!<Struct */
    uint32_t b32_Reg;  /*!<Union */
} U_MSS_CTRL_MSS_GPADC_MEM_INIT_REG;


#ifdef __cplusplus
}
#endif

#endif

/*
 * END OF REG_GPADC_H
 */

