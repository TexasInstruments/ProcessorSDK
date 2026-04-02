/*
*
* Copyright (c) {2015 - 2020} Texas Instruments Incorporated
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
#ifndef SINGLE_MMA_LOOP_H_
#define SINGLE_MMA_LOOP_H_
#include <stdio.h>
#include <stdlib.h>
#ifdef __C7X_HOSTEM__
#include <c7x.h>
#include <malloc.h>
#else
#include <c7x.h>
#include "enable_cache_mmu.h"
#endif
#include "tidl_conv2d_mma_i.h"

/*Debug support related header files*/
//#include <paramv.h>
//#include <printv.h>

#define TRACE_ON
//#define TRACE_L2_ON
#define MMA_MODELING
//#define STATIC_REF_CHECK

#ifdef STATIC_REF_CHECK
#include "staticRef.h"
#endif

#ifdef __C7X_HOSTEM__
#define restrict
#endif

//#define PROFILE_ON

/**************User configurable parameters**********************/
/****************************************************************/

#ifdef PROFILE_ON
uint64_t t1, t0;
#endif
/*****************************************************************/

#define L2_MEM_SIZE (256*1024)
#define INFEAT_L2_MEM_SIZE (128*1024) // this should be power of 2.
#define LEFT_L2_MEM_SIZE (L2_MEM_SIZE - INFEAT_L2_MEM_SIZE)

/* Needed buffer allocations*/
DSPRAGMA(scratchBuf, ".scratchBuf")
#ifndef __C7X_HOSTEM__
uint8_t scratchBuf[INFEAT_L2_MEM_SIZE];  // Here Fr + 1, Ni should be power of 2, otherwise it will fail for circularity SE access
#else
uint8_t * scratchBuf;  // Here Fr + 1, Ni should be power of 2, otherwise it will fail for circularity SE access
#endif

/*Profile related memory and defination*/
DSPRAGMA(profile, ".profile")
uint32_t profile[8];

#define PROFILE_READ   __TSC


/*****************Function definitions******************/
int32_t tidl_conv2d_mma();
static void hwaInit(tidlMmaConv2Params_t* convPrm);
static void calcMMAConv(tidlMmaConv2Params_t* convPrm);
static void initRandomCoeff(tidlMmaConv2Params_t* convPrm);
static void initRandomInput(tidlMmaConv2Params_t* convPrm);
void initParams(tidlMmaConv2Params_t* convPrm);
static void calcRefConv(tidlMmaConv2Params_t* convPrm);
static int32_t compareOutput(tidlMmaConv2Params_t* convPrm);


static void blockConvS08_ci(uint8_t* restrict cDest,
                            long biasLoadCond,
                            uchar64 biasPkd,
                            int32_t iterCount,
                            tidlMmaConv2Params_t* convPrm);

static void prefillBpanel_ci();
/*******************************************************/

/**************** streaming engine parameters ***********/
__STRM_TEMPLATE seTemplate0;
__STRM_TEMPLATE seTemplate1;
__STRM_TEMPLATE saTemplate0;
__STRM_TEMPLATE saTemplate1;
__STRM_TEMPLATE saTemplate2;
/********************************************************/
#define oneKB 1024

int32_t circularBlockSizeTbl[24] = {
  512, 1*oneKB, 2*oneKB, 4*oneKB, 8*oneKB, 16*oneKB, 32*oneKB,
  64*oneKB, 128*oneKB, 256*oneKB, 512*oneKB, oneKB*oneKB, 2*oneKB*oneKB, 4*oneKB*oneKB,
  8*oneKB*oneKB,16*oneKB*oneKB,32*oneKB*oneKB,64*oneKB*oneKB,128*oneKB*oneKB,256*oneKB*oneKB,
  512*oneKB*oneKB,oneKB*oneKB*oneKB*oneKB,2*oneKB*oneKB*oneKB*oneKB, 4*oneKB*oneKB*oneKB*oneKB
};

#ifndef MMA_MODELING
const __HWA_CONFIG_REG configRegisterStruct_i8s_i8s_o8s =
{
      .A_ATYPE  = A_CONFIG_ATYPE_UINT8,
      .A_RSVD1  = 0,
      .A_ALUTEN = A_CONFIG_NOLUT,
      .A_RSVD2  = 0,

      .B_BSWPER  = MMA_SIZE_8_BIT, // 32 bits // B bank switch period
      .B_BRSTPER = MMA_SIZE_8_BIT, // 8 bits // B offset reset period
      .B_BTYPE   = B_CONFIG_SIZE8, // 2 bits
      .B_RSVD1   = 0,
      .B_ORDER   = B_CONFIG_ROW,     // 1 bit // B matrix row major or col major
      .B_RSVD2   = 0,
      .B_BSTART  = 0,    // 1 bits // Initial B bank selection for writing B matrix data
      .B_RSVD3   = 0,
      .B_BOFFSET = 0,  // 8 bits //Global row or column offset
      .B_RSVD4   = 0,

      .C_ATYPE = C_CONFIG_ATYPE_SA, // Vector element is sgned or unsigned SA--> signed, UA --> unsigned
      .C_RSVD1 = 0,
      .C_BTYPE = C_CONFIG_BTYPE_UINT8, //
      .C_RSVD2 = 0,
      .C_OPERATION0 = C_CONFIG_MUL,
      .C_RSVD3 = 0,
      .C_OPERATION1 = C_CONFIG_MULPLUS,
      .C_RSVD4 = 0,
      .C_HWLDDST = C_CONFIG_HWLDDST_X1, // HWLD C data scaling or not. Not needed if bias loading is not done
      .C_RSVD5 = 0,
      .C_HWLDTYPE = C_CONFIG_HWLDTYPE_INT8, // HWLD C data type. Not needed if bias loading is not done
      .C_RSVD6 = 0,

      .C_OPSTART  = C_CONFIG_MUL, // No enum in MMA spec? Initial C operand selections
      .C_BSTART   = 0x0,  // Initial B bank selection for reading B matrix data
      .C_CRSTART  = 0x0, // Initial C bank selection for reading operands
      .C_CWSTART  = 0x0, // Initial C bank selection for writing computation results
      .C_CLSTART  = 0x0, // Initial C bank selection for writing operands from HWALD*
      .C_RSVD7    = 0,
      .C_CROFFSET = 0x0,       // 6-bits C row read offset
      .C_RSVD8    = 0,
      .C_CWOFFSET = 0x0,       // C row write offset for computations
      .C_RSVD9    = 0,
      .C_CLOFFSET = MMA_SIZE_8_BIT,  // C row write offset for HWALD* instructions
      .C_RSVD10   = 0,
      .C_CLSWPER  = MMA_SIZE_8_BIT,  // C bank switch period for HWALD* instruction writes
      .C_CLRSTPER = 0, // C write row offset reset period for HWALD*
      .C_OP1PER   = (K-1)*MMA_SIZE_8_BIT,  // Operation 1 period
      .C_OP0PER   = MMA_SIZE_8_BIT,  // Operation 0 period
      .C_BSWPER   = MMA_SIZE_8_BIT,  // B bank switch period
      .C_CRSWPER  = K*MMA_SIZE_8_BIT, // C bank switch period for read instructions
      .C_CWSWPER  = K*MMA_SIZE_8_BIT, // C bank switch period for computation writes
      .C_CRRSTPER = MMA_SIZE_8_BIT, // C read row offset reset period
      .C_CWRSTPER = MMA_SIZE_8_BIT, // C write row offset reset period for computations

      .X_ReLU  = 0x0, // Enable Rectified Linear Units non-linearity after optional saturation
      .X_RSVD1 = 0,
      .X_SAT   = 0x0,  // Enable saturation in the transfer buffer element type after optional rounding
      .X_RSVD2 = 0,
      .X_RE    = 0x0,   // Enable routing via 1/2 LSB addition after shifting
      .X_RSVD3 = 0,
      .X_SHIFT = OUT_SHIFT, // 7 bits Right shift amount, signed or unsigned depending on CTYPE field.
      .X_RSVD4 = 0,
      .X_XTYPE = X_CONFIG_XTYPE_UINT8, // Transfer buffer element type. Not all combinations of CTYPE and XTYPE are supported
      .X_RSVD5 = 0,
      .X_CTYPE = X_CONFIG_CTYPE_UINT32, // C matrix element type.  This must be consistent with the B FSM setting
      .X_RSVD6 = 0,
      .X_CSWPER   = K*MMA_SIZE_8_BIT, // C read bank switch period
      .X_CRRSTPER = MMA_SIZE_8_BIT,     // C read row offset reset period
      .X_COFFSET  = 0x0,          // C matrix row read address offset
      .X_CSTART   = 0x1,          // Initial C bank selection
      .X_RSVD7    = 0x0,          // Reserved

      .RSVD = 0,
      .PARITYCTRL = PNCM_NCK
};

const __HWA_OFFSET_REG offsetRegStruct =
{
    .A_LUT_VAL_0      = 0,
    .A_LUT_VAL_1      = 0,
    .A_LUT_VAL_2      = 0,
    .A_LUT_VAL_3      = 0,
    .A_LUT_VAL_4      = 0,
    .A_LUT_VAL_5      = 0,
    .A_LUT_VAL_6      = 0,
    .A_LUT_VAL_7      = 0,
    .A_LUT_VAL_8      = 0,
    .A_LUT_VAL_9      = 0,
    .A_LUT_VAL_10     = 0,
    .A_LUT_VAL_11     = 0,
    .A_LUT_VAL_12     = 0,
    .A_LUT_VAL_13     = 0,
    .A_LUT_VAL_14     = 0,
    .A_LUT_VAL_15     = 0,
    .offset0  = 0,
    .offset1  = 0,
    .offset2  = 0,
    .offset3  = 0,
    .offset4  = 0,
    .offset5  = 0,
    .offset6  = 0,
    .offset7  = 0,
    .offset8  = 0,
    .offset9  = 0,
    .offset10 = 0,
    .offset11 = 0,
    .offset12 = 0,
    .offset13 = 0,
    .offset14 = 0,
    .offset15 = 0,
    .offset16 = 0,
    .offset17 = 0,
    .offset18 = 0,
    .offset19 = 0,
    .offset20 = 0,
    .offset21 = 0,
    .offset22 = 0,
    .offset23 = 0,
    .offset24 = 0,
    .offset25 = 0,
    .offset26 = 0,
    .offset27 = 0,
    .offset28 = 0,
    .offset29 = 0,
    .offset30 = 0,
    .offset31 = 0,
    .offset32 = 0,
    .offset33 = 0,
    .offset34 = 0,
    .offset35 = 0,
    .offset36 = 0,
    .offset37 = 0,
    .offset38 = 0,
    .offset39 = 0,
    .offset40 = 0,
    .offset41 = 0,
    .offset42 = 0,
    .offset43 = 0,
    .offset44 = 0,
    .offset45 = 0,
    .offset46 = 0,
    .offset47 = 0,
    .offset48 = 0,
    .offset49 = 0,
    .offset50 = 0,
    .offset51 = 0,
    .offset52 = 0,
    .offset53 = 0,
    .offset54 = 0,
    .offset55 = 0,
    .offset56 = 0,
    .offset57 = 0,
    .offset58 = 0,
    .offset59 = 0,
    .offset60 = 0,
    .offset61 = 0,
    .offset62 = 0,
    .offset63 = 0
};
#endif

#ifdef MMA_MODELING

int32_t cPanel[2][MMA_SIZE_8_BIT*MMA_SIZE_8_BIT];
char64 bPanel[2][MMA_SIZE_8_BIT];
char64 bPanelT[MMA_SIZE_8_BIT];

int32_t cPanelReadId;
int32_t cPanelWriteId;
int32_t bPanelReadId;
int32_t bPanelWriteId;

static void prefillBpanel(long biasLoadCond,
                          char64 biasPkd);
static void transposeBPanel();
static void mmaOP(char64 vCoeff, int32_t i, int32_t j, tidlMmaConv2Params_t* convParams);

static void blockConvS08(uint8_t *cDest,
                            long biasLoadCond,
                            char64 biasPkd,
                            int32_t iterCount,
                            tidlMmaConv2Params_t* convPrm);

static void updateState(int32_t i, tidlMmaConv2Params_t* convParams);
#endif



#endif
