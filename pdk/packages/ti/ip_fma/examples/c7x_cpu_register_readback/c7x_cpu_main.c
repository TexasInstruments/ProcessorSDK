/*
 *  Copyright (C) 2026 Texas Instruments Incorporated
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
 *  \file c7x_cpu_main.c
 *
 *  \brief Main file for cpu illegal operations example build. 
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <c7x.h>
#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>
#include <ti/csl/soc.h>
#include <ti/board/board.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>

/* ========================================================================== */
/*                         Macros & Typedefs                                  */
/* ========================================================================== */

#define LOOP_CYCLES 10

typedef struct
{
    unsigned long ecr_L1DCFG;
    
    unsigned long ecr_L2CFG;
    unsigned long ecr_L2CC;
    unsigned long ecr_L2EDCFG;
    unsigned long ecr_L2MEMMAP;
    unsigned long ecr_L2MSWCTL;
    unsigned long ecr_L2MEMMAPIBUFA;
    unsigned long ecr_L2MEMMAPIBUFB;
    unsigned long ecr_L2MEMMAPWBUF;
    unsigned long ecr_L2EDTST;

    unsigned long ecr_MMU_REPL;
    unsigned long ecr_L1D_UTLB_REPL;
    unsigned long ecr_L1P_UTLB_REPL;
    unsigned long ecr_SE0_UTLB_REPL;
    unsigned long ecr_SE1_UTLB_REPL;

    unsigned long ecr_SCR;
    unsigned long ecr_TCR0;
    unsigned long ecr_TCR1;
    unsigned long ecr_TBR0;
    unsigned long ecr_TBR1;
    unsigned long ecr_MAR;
    
    unsigned long ecr_SCR_GS;
    unsigned long ecr_TCR0_GS;
    unsigned long ecr_TCR1_GS;
    unsigned long ecr_TBR0_GS;
    unsigned long ecr_TBR1_GS;
    unsigned long ecr_MAR_GS;

    unsigned long ecr_VCR;
    unsigned long ecr_VTCR;
    unsigned long ecr_VTBR;

} Static_Configuration_RegistersType;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

Static_Configuration_RegistersType gStatic_configuration_registers;

static uint16_t Test_status = 0U; 

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void C7x_static_registers_store(void)
{
    /* Store all the  static configuration registers 
     * in a global structure for comparison reference */
    gStatic_configuration_registers.ecr_L1DCFG = __get_indexed(__ECR_L1DCFG, 0);
    
    gStatic_configuration_registers.ecr_L2CFG = __get_indexed(__ECR_L2CFG, 0);
    gStatic_configuration_registers.ecr_L2CC = __get_indexed(__ECR_L2CC, 0);
    gStatic_configuration_registers.ecr_L2EDCFG = __get_indexed(__ECR_L2EDCFG, 0);
    gStatic_configuration_registers.ecr_L2MEMMAP = __get_indexed(__ECR_L2MEMMAP, 0);
    gStatic_configuration_registers.ecr_L2MSWCTL = __get_indexed(__ECR_L2MSWCTL, 0);
    gStatic_configuration_registers.ecr_L2MEMMAPIBUFA = __get_indexed(__ECR_L2MEMMAPIBUFA, 0);
    gStatic_configuration_registers.ecr_L2MEMMAPIBUFB = __get_indexed(__ECR_L2MEMMAPIBUFB, 0);
    gStatic_configuration_registers.ecr_L2MEMMAPWBUF = __get_indexed(__ECR_L2MEMMAPWBUF, 0);
    gStatic_configuration_registers.ecr_L2EDTST = __get_indexed(__ECR_L2EDTST, 0);

    gStatic_configuration_registers.ecr_MMU_REPL = __get_indexed(__ECR_MMU_REPL, 0);
    gStatic_configuration_registers.ecr_L1D_UTLB_REPL = __get_indexed(__ECR_L1D_UTLB_REPL, 0);
    gStatic_configuration_registers.ecr_L1P_UTLB_REPL = __get_indexed(__ECR_L1P_UTLB_REPL, 0);
    gStatic_configuration_registers.ecr_SE0_UTLB_REPL = __get_indexed(__ECR_SE0_UTLB_REPL, 0);
    gStatic_configuration_registers.ecr_SE1_UTLB_REPL = __get_indexed(__ECR_SE1_UTLB_REPL, 0);

    gStatic_configuration_registers.ecr_SCR = __get_indexed(__ECR_SCR, 0);
    gStatic_configuration_registers.ecr_TCR0 = __get_indexed(__ECR_TCR0, 0);
    gStatic_configuration_registers.ecr_TCR1 = __get_indexed(__ECR_TCR1, 0);
    gStatic_configuration_registers.ecr_TBR0 = __get_indexed(__ECR_TBR0, 0);
    gStatic_configuration_registers.ecr_TBR1 = __get_indexed(__ECR_TBR1, 0);
    gStatic_configuration_registers.ecr_MAR = __get_indexed(__ECR_MAR, 0);
    
    gStatic_configuration_registers.ecr_SCR_GS = __get_indexed(__ECR_SCR_GS, 0);
    gStatic_configuration_registers.ecr_TCR0_GS = __get_indexed(__ECR_TCR0_GS, 0);
    gStatic_configuration_registers.ecr_TCR1_GS = __get_indexed(__ECR_TCR1_GS, 0);
    gStatic_configuration_registers.ecr_TBR0_GS = __get_indexed(__ECR_TBR0_GS, 0);
    gStatic_configuration_registers.ecr_TBR1_GS = __get_indexed(__ECR_TBR1_GS, 0);
    gStatic_configuration_registers.ecr_MAR_GS = __get_indexed(__ECR_MAR_GS, 0);

    gStatic_configuration_registers.ecr_VCR = __get_indexed(__ECR_VCR, 0);
    gStatic_configuration_registers.ecr_VTCR = __get_indexed(__ECR_VTCR, 0);
    gStatic_configuration_registers.ecr_VTBR = __get_indexed(__ECR_VTBR, 0);
}

void C7x_registers_readback(void)
{
    /* Read all the static configuration registers */
    unsigned long ecr_L1DCFG = __get_indexed(__ECR_L1DCFG, 0);
    
    unsigned long ecr_L2CFG = __get_indexed(__ECR_L2CFG, 0);
    unsigned long ecr_L2CC = __get_indexed(__ECR_L2CC, 0);
    unsigned long ecr_L2EDCFG = __get_indexed(__ECR_L2EDCFG, 0);
    unsigned long ecr_L2MEMMAP = __get_indexed(__ECR_L2MEMMAP, 0);
    unsigned long ecr_L2MSWCTL = __get_indexed(__ECR_L2MSWCTL, 0);
    unsigned long ecr_L2MEMMAPIBUFA = __get_indexed(__ECR_L2MEMMAPIBUFA, 0);
    unsigned long ecr_L2MEMMAPIBUFB = __get_indexed(__ECR_L2MEMMAPIBUFB, 0);
    unsigned long ecr_L2MEMMAPWBUF = __get_indexed(__ECR_L2MEMMAPWBUF, 0);
    unsigned long ecr_L2EDTST = __get_indexed(__ECR_L2EDTST, 0);

    unsigned long ecr_MMU_REPL = __get_indexed(__ECR_MMU_REPL, 0);
    unsigned long ecr_L1D_UTLB_REPL = __get_indexed(__ECR_L1D_UTLB_REPL, 0);
    unsigned long ecr_L1P_UTLB_REPL = __get_indexed(__ECR_L1P_UTLB_REPL, 0);
    unsigned long ecr_SE0_UTLB_REPL = __get_indexed(__ECR_SE0_UTLB_REPL, 0);
    unsigned long ecr_SE1_UTLB_REPL = __get_indexed(__ECR_SE1_UTLB_REPL, 0);

    unsigned long ecr_SCR = __get_indexed(__ECR_SCR, 0);
    unsigned long ecr_TCR0 = __get_indexed(__ECR_TCR0, 0);
    unsigned long ecr_TCR1 = __get_indexed(__ECR_TCR1, 0);
    unsigned long ecr_TBR0 = __get_indexed(__ECR_TBR0, 0);
    unsigned long ecr_TBR1 = __get_indexed(__ECR_TBR1, 0);
    unsigned long ecr_MAR = __get_indexed(__ECR_MAR, 0);
    
    unsigned long ecr_SCR_GS = __get_indexed(__ECR_SCR_GS, 0);
    unsigned long ecr_TCR0_GS = __get_indexed(__ECR_TCR0_GS, 0);
    unsigned long ecr_TCR1_GS = __get_indexed(__ECR_TCR1_GS, 0);
    unsigned long ecr_TBR0_GS = __get_indexed(__ECR_TBR0_GS, 0);
    unsigned long ecr_TBR1_GS = __get_indexed(__ECR_TBR1_GS, 0);
    unsigned long ecr_MAR_GS = __get_indexed(__ECR_MAR_GS, 0);

    unsigned long ecr_VCR = __get_indexed(__ECR_VCR, 0);
    unsigned long ecr_VTCR = __get_indexed(__ECR_VTCR, 0);
    unsigned long ecr_VTBR = __get_indexed(__ECR_VTBR, 0);

    /* compare the read values in each iteration with the 
     * values stored at the start in the global structure */
    if (gStatic_configuration_registers.ecr_L1DCFG != ecr_L1DCFG || 
        gStatic_configuration_registers.ecr_L2CFG != ecr_L2CFG ||
        gStatic_configuration_registers.ecr_L2CC != ecr_L2CC ||
        gStatic_configuration_registers.ecr_L2EDCFG != ecr_L2EDCFG ||
        gStatic_configuration_registers.ecr_L2MEMMAP != ecr_L2MEMMAP ||
        gStatic_configuration_registers.ecr_L2MSWCTL != ecr_L2MSWCTL ||
        gStatic_configuration_registers.ecr_L2MEMMAPIBUFA != ecr_L2MEMMAPIBUFA ||
        gStatic_configuration_registers.ecr_L2MEMMAPIBUFB != ecr_L2MEMMAPIBUFB ||
        gStatic_configuration_registers.ecr_L2MEMMAPWBUF != ecr_L2MEMMAPWBUF ||
        gStatic_configuration_registers.ecr_L2EDTST != ecr_L2EDTST ||

        gStatic_configuration_registers.ecr_MMU_REPL != ecr_MMU_REPL ||
        gStatic_configuration_registers.ecr_L1D_UTLB_REPL != ecr_L1D_UTLB_REPL ||
        gStatic_configuration_registers.ecr_L1P_UTLB_REPL != ecr_L1P_UTLB_REPL ||
        gStatic_configuration_registers.ecr_SE0_UTLB_REPL != ecr_SE0_UTLB_REPL ||
        gStatic_configuration_registers.ecr_SE1_UTLB_REPL != ecr_SE1_UTLB_REPL ||

        gStatic_configuration_registers.ecr_SCR != ecr_SCR ||
        gStatic_configuration_registers.ecr_TCR0 != ecr_TCR0 ||
        gStatic_configuration_registers.ecr_TCR1 != ecr_TCR1 ||
        gStatic_configuration_registers.ecr_TBR0 != ecr_TBR0 ||
        gStatic_configuration_registers.ecr_TBR1 != ecr_TBR1 ||
        gStatic_configuration_registers.ecr_MAR != ecr_MAR ||
    
        gStatic_configuration_registers.ecr_SCR_GS != ecr_SCR_GS ||
        gStatic_configuration_registers.ecr_TCR0_GS != ecr_TCR0_GS ||
        gStatic_configuration_registers.ecr_TCR1_GS != ecr_TCR1_GS ||
        gStatic_configuration_registers.ecr_TBR0_GS != ecr_TBR0_GS ||
        gStatic_configuration_registers.ecr_TBR1_GS != ecr_TBR1_GS ||
        gStatic_configuration_registers.ecr_MAR_GS != ecr_MAR_GS ||

        gStatic_configuration_registers.ecr_VCR != ecr_VCR ||
        gStatic_configuration_registers.ecr_VTCR != ecr_VTCR ||
        gStatic_configuration_registers.ecr_VTBR != ecr_VTBR)
    {
        Test_status = 1U;
    } 

    /* Log all the values read */
    UART_printf("__L1DCFG           : 0x%x\n\n", ecr_L1DCFG);

    UART_printf("__L2CFG            : 0x%x\n", ecr_L2CFG);
    UART_printf("__L2CC             : 0x%x\n", ecr_L2CC);
    UART_printf("__L2EDCFG          : 0x%x\n", ecr_L2EDCFG);
    UART_printf("__L2MEMMAP         : 0x%x\n", ecr_L2MEMMAP);
    UART_printf("__L2MSWCTL         : 0x%x\n", ecr_L2MSWCTL);
    UART_printf("__L2MEMMAPIBUFA    : 0x%x\n", ecr_L2MEMMAPIBUFA);
    UART_printf("__L2MEMMAPIBUFB    : 0x%x\n", ecr_L2MEMMAPIBUFB);
    UART_printf("__L2MEMMAPWBUF     : 0x%x\n", ecr_L2MEMMAPWBUF);
    UART_printf("__L2EDTST          : 0x%x\n\n", ecr_L2EDTST);

    UART_printf("__MMU_REPL         : 0x%x\n", ecr_MMU_REPL);
    UART_printf("__L1D_UTLB_REPL    : 0x%x\n", ecr_L1D_UTLB_REPL);
    UART_printf("__L1P_UTLB_REPL    : 0x%x\n", ecr_L1P_UTLB_REPL);
    UART_printf("__SE0_UTLB_REPL    : 0x%x\n", ecr_SE0_UTLB_REPL);
    UART_printf("__SE1_UTLB_REPL    : 0x%x\n\n", ecr_SE1_UTLB_REPL);

    UART_printf("__SCR              : 0x%x\n", ecr_SCR);
    UART_printf("__TCR0             : 0x%x\n", ecr_TCR0);
    UART_printf("__TCR1             : 0x%x\n", ecr_TCR1);
    UART_printf("__TBR0             : 0x%x\n", ecr_TBR0);
    UART_printf("__TBR1             : 0x%x\n", ecr_TBR1);
    UART_printf("__MAR              : 0x%x\n\n", ecr_MAR);

    UART_printf("__SCR_GS           : 0x%x\n", ecr_SCR_GS);
    UART_printf("__TCR0_GS          : 0x%x\n", ecr_TCR0_GS);
    UART_printf("__TCR1_GS          : 0x%x\n", ecr_TCR1_GS);
    UART_printf("__TBR0_GS          : 0x%x\n", ecr_TBR0_GS);
    UART_printf("__TBR1_GS          : 0x%x\n", ecr_TBR1_GS);
    UART_printf("__MAR_GS           : 0x%x\n\n", ecr_MAR_GS);

    UART_printf("__VCR              : 0x%x\n", ecr_VCR);
    UART_printf("__VTCR             : 0x%x\n", ecr_VTCR);
    UART_printf("__VTBR             : 0x%x\n\n", ecr_VTBR);
}

void C7x_idle(void)
{
    UART_printf("Placing C7x in idle Condition\n");
   __asm(" IDLE");
   while (1)
   {
    /* code */
   }
}

/* ========================================================================== */
/*                          Main Function                                     */
/* ========================================================================== */

int main(void)
{
    /* Initialize OS */
    OS_init();

    /* Configure the Pin Mux and UART */
    Board_initCfg boardCfg;
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    /* Populate the global static configuration registers structure */
    C7x_static_registers_store();

    int loopcnt;
    for(loopcnt = 0; loopcnt < LOOP_CYCLES; loopcnt++)
    {
        UART_printf("\n Iteration Number: %d\n", loopcnt);
        
        C7x_registers_readback();

        /* Add delay to avoid rapid reading*/
        Osal_delay(loopcnt);
    }

    if(Test_status == 0U)
    {
        UART_printf("\n All tests have passed\n");
    }

    C7x_idle();
    return(0);
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
