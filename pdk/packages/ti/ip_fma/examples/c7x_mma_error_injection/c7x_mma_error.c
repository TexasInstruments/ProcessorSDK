/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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
 *  \file c7x_mma_error.c
 *
 *  \brief MMA error functions should not be optimized for recovery
 *         Except for C Matrix Read/Write
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "c7x_mma_error.h"
#include <c7x.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define C_MATRIX_REPEAT 5
#define FSM_ENABLE      1
/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

__HWA_CONFIG_REG_v1 mma_config_reg;
__HWA_OFFSET_REG offset_reg;
__HWA_OFFSET_REG offset_reg2;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void HWA_NOP(uint8_t delay_count)
{
  int init_delay = 0;
  for(;init_delay<delay_count;init_delay++)
  {
    __HWAADV();
  }
}

#pragma FUNCTION_OPTIONS(MMA_underflowError_injection,"--opt_level=off")
void MMA_underflowError_injection(void)
{
  mma_config_reg = __gen_HWA_CONFIG_REG_v1();
  offset_reg = __gen_HWA_OFFSET_REG();
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET);
  HWA_NOP(3);
  __HWARCV(0);
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(MMA_overflowError_injection,"--opt_level=off")
void MMA_overflowError_injection(void)
{
  mma_config_reg = __gen_HWA_CONFIG_REG_v1();
  offset_reg = __gen_HWA_OFFSET_REG();
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET);
  int loop_begin = 1;
  int loop_end = 30;
  for(;loop_begin<=loop_end;loop_begin++)
  {
    __HWAXFER(__MMA_XFER_SRC_HWA_STATUS);
    __HWAADV();
    __HWAADV();
    __HWAADV();
    __HWAADV();
  }
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(MMA_cMatrixWrite_collision,"--opt_level=off")
void MMA_cMatrixWrite_collision(void)
{
    
  int switch_period=0; 
  int repeat_op_count = 0;
  for( ;switch_period<C_MATRIX_REPEAT;switch_period++)
  {
    __HWA_CONFIG_REG_v1 config_reg = __gen_HWA_CONFIG_REG_v1();
    config_reg.C_OPERATION0 = __MMA_C_CONFIG_MULPLUS;
    config_reg.C_OPERATION1 = __MMA_C_CONFIG_MULPLUS;
    config_reg.C_CRSWPER = switch_period; //different switching period
    config_reg.C_CWSWPER  = switch_period+10000;
    offset_reg = __gen_HWA_OFFSET_REG(); //This generates the offset register.  
    __HWAOPEN(config_reg,offset_reg,__MMA_OPEN_FSM_RESET); //This opens theHWAOPEN
    repeat_op_count = 0;
    for(;repeat_op_count<C_MATRIX_REPEAT;repeat_op_count++)
    {
      __asm(" HWALDC .L2 VB0 \n"
            "||HWAOP.S1 SA0");      
      __HWAADV();
      __HWAADV();
      __HWAADV();
      __HWAADV();
    }
  }
    __HWARCV(0);  //Calls the error
    __asm(" NOP");
    __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(MMA_cMatrixRead_collision,"--opt_level=3")
void MMA_cMatrixRead_collision(void)
{
  int switch_period=0;
  int repeat_op_count = 0;

  for( ;switch_period<C_MATRIX_REPEAT;switch_period++)
  {
    __HWA_CONFIG_REG_v1 config_reg = __gen_HWA_CONFIG_REG_v1();
    config_reg.C_OPERATION0 = __MMA_C_CONFIG_MULPLUS;
    config_reg.C_OPERATION1 = __MMA_C_CONFIG_MULPLUS;
    config_reg.C_CRSWPER = switch_period; //different switching period
    config_reg.C_CWSWPER  = switch_period+10000;
    offset_reg = __gen_HWA_OFFSET_REG(); //This generates the offset register.  
    __HWAOPEN(config_reg,offset_reg,__MMA_OPEN_FSM_RESET); //This opens theHWAOPEN
    repeat_op_count = 0;

    for(;repeat_op_count<C_MATRIX_REPEAT;repeat_op_count++)
      {
        __asm(" HWAOP.S1 SA0 \n"
              "||HWAXFER .L1 CMAT"); //This should run them in parallel
        __HWAADV();
        __HWAADV();
        __HWAADV();
      }
  }
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(Get_AFSM_Corrupt_config,"--opt_level=off")
__HWA_CONFIG_REG_v1 Get_AFSM_Corrupt_config(void)
{
  __HWA_CONFIG_REG_v1 corrupted_config = __gen_HWA_CONFIG_REG_v1(); 
  corrupted_config.A_ATYPE = __MMA_A_CONFIG_ATYPE_UINT32;
  corrupted_config.A_ALUTEN = __MMA_A_LUTEN_LAST;
  return corrupted_config;
}

#pragma FUNCTION_OPTIONS(Get_BFSM_Corrupt_config,"--opt_level=off")
__HWA_CONFIG_REG_v1 Get_BFSM_Corrupt_config(void)
{
  __HWA_CONFIG_REG_v1 corrupted_config = __gen_HWA_CONFIG_REG_v1();   
  corrupted_config.B_BSWPER = 100; 
  corrupted_config.B_BTYPE = __MMA_B_CONFIG_SIZE32;
  return corrupted_config; 
}

#pragma FUNCTION_OPTIONS(Get_CFSM_Corrupt_config,"--opt_level=off")
__HWA_CONFIG_REG_v1 Get_CFSM_Corrupt_config(void)
{
  __HWA_CONFIG_REG_v1 corrupted_config = __gen_HWA_CONFIG_REG_v1(); 
  corrupted_config.C_ATYPE = __MMA_C_CONFIG_ATYPE_UA; 
  corrupted_config.C_BTYPE = __MMA_C_CONFIG_BTYPE_UINT16;
  return corrupted_config;
}

#pragma FUNCTION_OPTIONS(Get_XFSM_Corrupt_config,"--opt_level=off")
__HWA_CONFIG_REG_v1 Get_XFSM_Corrupt_config(void)
{
  __HWA_CONFIG_REG_v1 corrupted_config = __gen_HWA_CONFIG_REG_v1(); 
  corrupted_config.X_ReLU = 1;
  return corrupted_config; 
}

#pragma FUNCTION_OPTIONS(Get_MMAconfig,"--opt_level=off")
__HWA_CONFIG_REG_v1 Get_MMAconfig(void)
{
  __HWA_CONFIG_REG_v1 mma_config_reg;
  mma_config_reg = __gen_HWA_CONFIG_REG_v1();
  mma_config_reg.A_ATYPE = __MMA_A_CONFIG_ATYPE_UINT16;
  mma_config_reg.A_ALUTEN = __MMA_A_LUTEN_LAST;
  mma_config_reg.B_BSWPER = 1000;
  mma_config_reg.B_BTYPE = __MMA_B_CONFIG_SIZE16;
  mma_config_reg.B_BSTART = 1;
  mma_config_reg.C_ATYPE = __MMA_C_CONFIG_ATYPE_SA;
  mma_config_reg.C_BTYPE = __MMA_C_CONFIG_BTYPE_UINT16;
  mma_config_reg.PARITYCTRL = __MMA_NORMAL;
  mma_config_reg.C_BSTART = 1; /* Initial B bank selection for reading B matrix data for the matrix computations */
  mma_config_reg.C_CRSTART = 1; /* Initial C bank selection for reading operands */
  mma_config_reg.C_CWSTART = 1; /* Initial C bank selection for writing computation results */
  mma_config_reg.C_CLSTART = 1; 
  return mma_config_reg;
}

#pragma FUNCTION_OPTIONS(MMA_AFSM_configParityError_injection,"--opt_level=off")
void MMA_AFSM_configParityError_injection(void)
{
  __HWA_CONFIG_REG_v1 mma_config_reg = Get_MMAconfig();
  __HWA_OFFSET_REG offset_reg;
  offset_reg = __gen_HWA_OFFSET_REG(); 
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET);
  HWA_NOP(3);
  __asm(" HWALDA .L2 VB1");
  __HWAXFER(__MMA_XFER_SRC_C);
  __HWA_CONFIG_REG_v1 mma_config_corrupt = Get_AFSM_Corrupt_config();
  mma_config_corrupt.PARITYCTRL = __MMA_PNCM_CK;
  __HWA_OFFSET_REG offset_reg2 = __gen_HWA_OFFSET_REG(); 
  HWA_NOP(3);
  __HWAOPEN(mma_config_corrupt,offset_reg2,__MMA_OPEN_FSM_RESET);
  HWA_NOP(3);
  __asm(" HWALDA .L2 VB0"); //Load to A matrix
  __HWAXFER(__MMA_XFER_SRC_HWA_CONFIG);
  HWA_NOP(3);
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(MMA_BFSM_configParityError_injection,"--opt_level=off")
void MMA_BFSM_configParityError_injection(void)
{
__HWA_CONFIG_REG_v1 mma_config_reg = Get_MMAconfig();
  __HWA_OFFSET_REG offset_reg;
  offset_reg = __gen_HWA_OFFSET_REG(); 
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET);
  HWA_NOP(4);
  __asm(" HWALDB .L2 VB1");
  __HWAXFER(__MMA_XFER_SRC_C);
  __HWA_CONFIG_REG_v1 mma_config_corrupt = Get_BFSM_Corrupt_config();
  mma_config_corrupt.PARITYCTRL = __MMA_PNCM_CK;
  __HWA_OFFSET_REG offset_reg2 = __gen_HWA_OFFSET_REG(); 
  HWA_NOP(3);
  __HWAOPEN(mma_config_corrupt,offset_reg2,__MMA_OPEN_FSM_RESET);
  HWA_NOP(3);
  __asm(" HWALDB .L2 VB0"); //Load to B matrix
  __HWAXFER(__MMA_XFER_SRC_HWA_CONFIG);
  HWA_NOP(3);
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");

}

#pragma FUNCTION_OPTIONS(MMA_CFSM_configParityError_injection,"--opt_level=off")
void MMA_CFSM_configParityError_injection(void)
{
  __HWA_CONFIG_REG_v1 mma_config_reg = Get_MMAconfig();
  __HWA_OFFSET_REG offset_reg;
  offset_reg = __gen_HWA_OFFSET_REG(); 
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET);
  HWA_NOP(4);
  __asm(" HWALDC .L2 VB1");
  __HWAXFER(__MMA_XFER_SRC_C);
  __HWA_CONFIG_REG_v1 mma_config_corrupt = Get_CFSM_Corrupt_config();
  mma_config_corrupt.PARITYCTRL = __MMA_PNCM_CK;
  __HWA_OFFSET_REG offset_reg2 = __gen_HWA_OFFSET_REG(); 
  HWA_NOP(3);
  __HWAOPEN(mma_config_corrupt,offset_reg2,__MMA_OPEN_FSM_RESET);
  HWA_NOP(3);
  __asm(" HWALDC .L2 VB0"); //Load to C Matrix
  __HWAXFER(__MMA_XFER_SRC_HWA_CONFIG);
  HWA_NOP(3);
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(MMA_XFSM_configParityError_injection,"--opt_level=off")
void MMA_XFSM_configParityError_injection(void)
{
  __HWA_CONFIG_REG_v1 mma_config_reg = Get_MMAconfig();
  __HWA_OFFSET_REG offset_reg;
  offset_reg = __gen_HWA_OFFSET_REG(); 
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET); // Initialises Config offset
  HWA_NOP(4);
 __HWAXFER(__MMA_XFER_SRC_HWA_CONFIG);

  __HWA_CONFIG_REG_v1 mma_config_corrupt = Get_XFSM_Corrupt_config(); //Receives X FSM corrupt configuration
  mma_config_corrupt.PARITYCTRL = __MMA_PNCM_CK; //Parity checking enabled calculation disabled
  __HWA_OFFSET_REG offset_reg2 = __gen_HWA_OFFSET_REG(); 
  HWA_NOP(3);
  __HWAOPEN(mma_config_corrupt,offset_reg2,__MMA_OPEN_FSM_RESET);
  HWA_NOP(3);
  __HWAXFER(__MMA_XFER_SRC_HWA_CONFIG); 
  HWA_NOP(3);
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(get_corrupted_offset,"--opt_level=off")
__HWA_OFFSET_REG get_corrupted_offset(void)
{
  __HWA_OFFSET_REG mma_offset_reg = __gen_HWA_OFFSET_REG();
  mma_offset_reg.A_LUT_VAL_1 = 0x01;
  return mma_offset_reg; //return the corrupted offset
}

#pragma FUNCTION_OPTIONS(MMA_offsetPartiyError_injection,"--opt_level=off")
void MMA_offsetPartiyError_injection(void)
{
  __HWA_CONFIG_REG_v1 mma_config_reg;
  mma_config_reg = __gen_HWA_CONFIG_REG_v1();
  mma_config_reg.A_ALUTEN = __MMA_A_LUTEN_LAST; //A matrix LUT2 Enabled
  mma_config_reg.PARITYCTRL = __MMA_NORMAL; //Parity computation enabled and parity checking enabled.
  __HWA_OFFSET_REG offset_reg;
  offset_reg = __gen_HWA_OFFSET_REG();
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET);//Initialises Config,offset
  HWA_NOP(4);
  __HWA_CONFIG_REG_v1 mma_config_1;
  mma_config_1 = __gen_HWA_CONFIG_REG_v1();
  mma_config_1.A_ALUTEN = __MMA_A_LUTEN_LAST;
  mma_config_1.PARITYCTRL = __MMA_PNCM_CK;//Parity computation disabled and parity checking enabled.
  __HWA_OFFSET_REG offset_reg2 = get_corrupted_offset(); //This gets the corrupted offset
  HWA_NOP(3);
  __HWAOPEN(mma_config_1,offset_reg2,__MMA_OPEN_FSM_RESET);
  HWA_NOP(3);
  __HWAXFER(__MMA_XFER_SRC_HWA_OFFSET);
  HWA_NOP(3);
  __HWARCV(0);
  __asm(" NOP");
  __asm(" NOP");
}

#pragma FUNCTION_OPTIONS(ClearMMA,"--opt_level=off")
void ClearMMA(void)
{
  mma_config_reg = __gen_HWA_CONFIG_REG_v1(); 
  mma_config_reg.PARITYCTRL = __MMA_NORMAL;
  offset_reg = __gen_HWA_OFFSET_REG();
  __HWAOPEN(mma_config_reg,offset_reg,__MMA_OPEN_FSM_RESET); //Initialises Config,offset
  __HWAADV();
  __HWACLOSE(0); //Clearing the first error code
}

