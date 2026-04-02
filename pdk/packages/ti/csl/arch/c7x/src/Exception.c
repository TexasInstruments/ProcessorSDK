/*
 * Copyright (c) 2014-2021 Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== Exception.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

#include "Hwi.h"

#include "Exception.h"
#pragma FUNC_EXT_CALLED(Exception_handler);

#include <c7x.h>

#include <ti/osal/DebugP.h>

/* Object__sizingError */
typedef char Exception_Context__sizingError[(sizeof(Exception_Context) > Exception_sizeContextBuf) ? -1 : 1];

/* Exception hooks init */
Exception_Hooks gExceptionHooks = {NULL};

/*
 *  ======== Exception_registerHooks ========
 *  Exception_registerHooks must be called to register custom hooks during exception 
 *  handling including a custom return hook
 */
void Exception_registerHooks(const Exception_Hooks *hooks)
{
    /* Copying exception hooks */
    gExceptionHooks.exceptionHook = hooks->exceptionHook;
    gExceptionHooks.internalHook = hooks->internalHook;
    Exception_Module_state.returnHook = hooks->returnHook;
}

/*
 *  ======== Exception_Module_startup ========
 *  Exception_Module_startup must be called to enable events to cause
 *  an exception.
 */
void Exception_Module_startup(void)
{
    /*
     *  Exception uses Hwi, needs to wait for its startup.
     */
    Exception_Module_state.excPtr = Hwi_getIsrStackAddress();

    return;
}

/*
 *  ======== Exception_handler ========
 *  This handler is called by the dispatch function.  It calls the various
 *  exception hook functions.
 */
void Exception_handler(bool abortFlag, int32_t vectorType)
{
    uint64_t *contextStack;
    uint64_t nrp;
    uint64_t savedTSR;
    Exception_Context *excp;
    uint32_t ncnt;
    int32_t i;
    /* set exception context */
    excp = Exception_Module_state.excContext;

    /* Force MAIN threadtype So we can safely call System_printf */
    DebugP_exceptionLog("A0 =0x%x A1 =0x%x\n", (uint32_t)(uintptr_t)excp->A0,(uint32_t) (uintptr_t)excp->A1);
    DebugP_exceptionLog("A2 =0x%x A3 =0x%x\n", (uint32_t)(uintptr_t)excp->A2, (uint32_t)(uintptr_t)excp->A3);
    DebugP_exceptionLog("A4 =0x%x A5 =0x%x\n", (uint32_t)(uintptr_t)excp->A4, (uint32_t)(uintptr_t)excp->A5);
    DebugP_exceptionLog("A6 =0x%x A7 =0x%x\n", (uint32_t)(uintptr_t)excp->A6, (uint32_t)(uintptr_t)excp->A7);
    DebugP_exceptionLog("A8 =0x%x A9 =0x%x\n", (uint32_t)(uintptr_t)excp->A8, (uint32_t)(uintptr_t)excp->A9);
    DebugP_exceptionLog("A10=0x%x A11=0x%x\n", (uint32_t)(uintptr_t)excp->A10, (uint32_t)(uintptr_t)excp->A11);
    DebugP_exceptionLog("A12=0x%x A13=0x%x\n", (uint32_t)(uintptr_t)excp->A12, (uint32_t)(uintptr_t)excp->A13);
    DebugP_exceptionLog("A14=0x%x A15=0x%x\n", (uint32_t)(uintptr_t)excp->A14, (uint32_t)(uintptr_t)excp->A15);
    DebugP_exceptionLog("D0 =0x%x D1 =0x%x\n", (uint32_t)((uintptr_t)excp->D0), (uint32_t)((uintptr_t)excp->D1));
    DebugP_exceptionLog("D2 =0x%x D3 =0x%x\n", (uint32_t)((uintptr_t)excp->D2), (uint32_t)((uintptr_t)excp->D3));
    DebugP_exceptionLog("D4 =0x%x D5 =0x%x\n", (uint32_t)((uintptr_t)excp->D4), (uint32_t)((uintptr_t)excp->D5));
    DebugP_exceptionLog("D6 =0x%x D7 =0x%x\n", (uint32_t)((uintptr_t)excp->D6), (uint32_t)((uintptr_t)excp->D7));
    DebugP_exceptionLog("D8 =0x%x D9 =0x%x\n", (uint32_t)((uintptr_t)excp->D8), (uint32_t)((uintptr_t)excp->D9));
    DebugP_exceptionLog("D10=0x%x D11=0x%x\n", (uint32_t)((uintptr_t)excp->D10),(uint32_t)((uintptr_t)excp->D11));
    DebugP_exceptionLog("D12=0x%x D13=0x%x\n", (uint32_t)((uintptr_t)excp->D12), (uint32_t)((uintptr_t)excp->D13));
    DebugP_exceptionLog("D14=0x%x D15=0x%x\n", (uint32_t)((uintptr_t)excp->D14),(uint32_t)((uintptr_t)excp->D15));
    DebugP_exceptionLog("AM0=0x%x AM1=0x%x\n", (uint32_t)((uintptr_t)excp->AM0), (uint32_t)((uintptr_t)excp->AM1));
    DebugP_exceptionLog("AM2=0x%x AM3=0x%x\n", (uint32_t)((uintptr_t)excp->AM2), (uint32_t)((uintptr_t)excp->AM3));
    DebugP_exceptionLog("AM4=0x%x AM5=0x%x\n", (uint32_t)((uintptr_t)excp->AM4), (uint32_t)((uintptr_t)excp->AM5));
    DebugP_exceptionLog("AM6=0x%x AM7=0x%x\n", (uint32_t)((uintptr_t)excp->AM6),(uint32_t)((uintptr_t)excp->AM7));
    DebugP_exceptionLog("AL0=0x%x AL1=0x%x\n", (uint32_t)((uintptr_t)excp->AL0), (uint32_t)((uintptr_t)excp->AL1));
    DebugP_exceptionLog("AL2=0x%x AL3=0x%x\n", (uint32_t)((uintptr_t)excp->AL2), (uint32_t)((uintptr_t)excp->AL3));
    DebugP_exceptionLog("AL4=0x%x AL5=0x%x\n", (uint32_t)((uintptr_t)excp->AL4), (uint32_t)((uintptr_t)excp->AL5));
    DebugP_exceptionLog("AL6=0x%x AL7=0x%x\n", (uint32_t)((uintptr_t)excp->AL6), (uint32_t)((uintptr_t)excp->AL7));
    DebugP_exceptionLog("P0=0x%x P1=0x%x\n", (uint32_t)((uintptr_t)excp->P0), (uint32_t)((uintptr_t)excp->P1));
    DebugP_exceptionLog("P2=0x%x P3=0x%x\n", (uint32_t)((uintptr_t)excp->P2), (uint32_t)((uintptr_t)excp->P3));
    DebugP_exceptionLog("P4=0x%x P5=0x%x\n", (uint32_t)((uintptr_t)excp->P4), (uint32_t)((uintptr_t)excp->P5));
    DebugP_exceptionLog("P6=0x%x P7=0x%x\n", (uint32_t)((uintptr_t)excp->P6), (uint32_t)((uintptr_t)excp->P7));
    DebugP_exceptionLog("FPCR=0x%x FSR=0x%x\n", (uint32_t)((uintptr_t)excp->FPCR), (uint32_t)((uintptr_t)excp->FSR));
    DebugP_exceptionLog("GFPGFR=0x%x GPLY=0x%x\n", (uint32_t)((uintptr_t)excp->GFPGFR), (uint32_t)((uintptr_t)excp->GPLY));

    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM1[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM1[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM2[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM2[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM3[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM3[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM4[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM4[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM5[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM5[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM6[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM6[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBM7[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBM7[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL1[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL1[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL2[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL2[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL3[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL3[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL4[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL4[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL5[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL5[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL6[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL6[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VBL7[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VBL7[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB1[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB1[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB2[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB2[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB3[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB3[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB4[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB4[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB5[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB5[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB6[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB6[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB7[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB7[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB8[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB8[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB9[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB9[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB10[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB10[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB11[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB11[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB12[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB12[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB13[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB13[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB14[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB14[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("VB15[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->VB15[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("CUCR0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->CUCR0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("CUCR1[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->CUCR1[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("CUCR2[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->CUCR2[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("CUCR3[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->CUCR3[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE0_0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE0_0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE0_1[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE0_1[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE0_2[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE0_2[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE0_3[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE0_3[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE1_0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE1_0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE1_1[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE1_1[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE1_2[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE1_2[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SE1_3[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SE1_3[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA0CR[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA0CR[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA1CR[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA1CR[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA2CR[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA2CR[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA3CR[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA3CR[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA0CNTR0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA0CNTR0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA1CNTR0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA1CNTR0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA2CNTR0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA2CNTR0[i]));
    }
    for (i = 0; i < 8; i++) {
        DebugP_exceptionLog("SA3CNTR0[%x]=0x%x\n", i, (uint32_t)((uintptr_t)excp->SA3CNTR0[i]));
    }
    DebugP_log0("\n");

    ncnt = (uint32_t)((__ECSP_S & 0xe000U) >> 13U);
    if (ncnt != 0U) {
        contextStack = (uint64_t *)(((uint64_t)__ECSP_S) + ((((uint64_t)ncnt - 1ULL) * 0x2000ULL)));
    }
    else  {
        contextStack = (uint64_t *)__TCSP;
    }

    nrp = contextStack[0];
    savedTSR = contextStack[1];

    Exception_Module_state.nrp = nrp;
    Exception_Module_state.ntsr = savedTSR;

    /* print general exception info */
        DebugP_log1("Exception at 0x%016lx\n", (uintptr_t)Exception_Module_state.nrp);
        DebugP_log1("TSR at time of exception: 0x%016lx\n",
                      (uintptr_t)Exception_Module_state.ntsr);
    if(gExceptionHooks.exceptionHook != (Exception_FuncPtr)NULL){
        gExceptionHooks.exceptionHook();
    }

    /* process all possible causes of exception */
    if (vectorType == 0) {
        /* internal exception */
        DebugP_log0("Internal exception:\n");
        Exception_internalHandler();
    }
    if (vectorType == 1) {
        /* page fault */
        DebugP_log0("Page fault:\n");
        Exception_internalHandler();
    }

    if (abortFlag) {
      DebugP_log2("Exception_E_exceptionMax", (uintptr_t)nrp, (uintptr_t)excp->D15);
    }
}

/*
 *  ======== Exception_internalHandler ========
 *  This function is called only if an internal exception has occurred.
 *  It checks the EFR register to determine what type of exceptions occurred.
 */
void Exception_internalHandler(void)
{
    uint32_t ierr;
    uint32_t iesr19_16;
    uint32_t iesr15_0;

    /* record IERR in Exc_module field */
    Exception_Module_state.ierr = __IERR;
    Exception_Module_state.iear = __IEAR;
    Exception_Module_state.iesr = __IESR;


    DebugP_log3("  IERR=0x%016lx\n"
                  "  IEAR=0x%016lx\n"
                  "  IESR=0x%016lx\n",
                  (uintptr_t)Exception_Module_state.ierr,
                  (uintptr_t)Exception_Module_state.iear,
                  (uintptr_t)Exception_Module_state.iesr);

    iesr19_16 = (uint32_t)(Exception_Module_state.iesr & 0xF0000U) >> 16U;
    iesr15_0 = (uint32_t)Exception_Module_state.iesr & 0xFFFFU;
    ierr = (uint32_t)Exception_Module_state.ierr;

    if (0U != (ierr & Exception_IERRPFX)) {
        DebugP_log0("  Page fault exception:\n");
        switch (iesr19_16) {
          case 0:
            DebugP_log1("    uTLB Fault, cpu_pmc_rstatus[10:0]=0x%x\n",
                          (iesr15_0 & 0x7FFUL));
            break;
          case 1:
            DebugP_log0("    .D1 or .D2 uTLB lookup fault, Non-speculative load\n");
            DebugP_log1("    utlb_rstatus=0x%x\n", (uintptr_t)iesr15_0);
            break;
          case 2:
            DebugP_log0("    .D1 or .D2 uTLB lookup fault, Speculative load\n");
            DebugP_log1("    utlb_rstatus=0x%x\n", (uintptr_t)iesr15_0);
            break;
          case 3:
            DebugP_log1("    uTLB Fault, cpu_se_N_rstatus[10:0]=0x%x\n",
                          ((uintptr_t)iesr15_0 & 0x7FFUL));
            break;
          default:
            DebugP_log1("    unknown sub-type, iesr[19:16]=0x%x\n",
                          (uintptr_t)iesr19_16);
            break;
        }
    }
    if (0U != (ierr & Exception_IERRIFX)) {
        DebugP_log1("  Instruction fetch exception, cpu_pmc_rstatus[2:0]=0x%x\n",
                      ((uintptr_t)iesr15_0 & 0x7UL));
    }
    if (0U != (ierr & Exception_IERRFPX)) {
        DebugP_log0("  Fetch packet exception\n");
    }
    if (0U != (ierr & Exception_IERREPX)) {
        DebugP_log0("  Execute packet exception\n");
    }
    if (0U != (ierr & Exception_IERROPX)) {
        DebugP_log0("  Illegal opcode exception\n");
    }
    if (0U != (ierr & Exception_IERRRCX)) {
        DebugP_log0("  Resource conflict exception\n");
    }
    if (0U != (ierr & Exception_IERRRAX)) {
        DebugP_log0("  Resource access exception\n");
        switch (iesr19_16) {
          case 0:
            DebugP_log0("    *SE access when stream is not open\n");
            break;
          case 1:
            DebugP_log0("    SESAVE or SERSTR while stream is open\n");
            break;
          case 2:
            DebugP_log0("    LUT ,HIST when the access bit = 0\n");
            DebugP_log1("    Histogram Base CR #: %d\n", (uintptr_t)iesr15_0);
            break;
          case 3:
            DebugP_log0("    ECR Non-zero Rstatus except for rstatus == 2 (permission violation)\n");
            DebugP_log1("    cpu_to_ecr_status=0x%x\n", (uintptr_t)iesr15_0);
            break;
          case 4:
            DebugP_log0("    DSWBP when SWBP en == 0\n");
            break;
          default:
            DebugP_log1("    unknown sub-type, iesr[19:16]=0x%x\n",
                          iesr19_16);
            break;
        }
    }
    if (0U != (ierr & Exception_IERRPRX)) {
        DebugP_log0("  Privilege exception\n");
        switch (iesr19_16) {
          case 0:
            DebugP_log0("    Internal MVC permission violation\n");
            break;
          case 1:
            DebugP_log0("    External MVC permission violation\n");
            DebugP_log1("    cpu_to_ecr_status=0x%x\n", iesr15_0);
            break;
          case 2:
            DebugP_log0("    Invalid Permissions for given opcode\n");
            break;
          case 3:
            DebugP_log0("    Illegal RETE/RETS execution\n");
            break;
          case 4:
            DebugP_log0("    ECSP addressing mode attempted as a user\n");
            break;
          default:
            DebugP_log1("    unknown sub-type, iesr[19:16]=0x%x\n",
                          iesr19_16);
            break;
        }
    }
    if (0U != (ierr & Exception_IERRLBX)) {
        DebugP_log0("  Loop buffer exception\n");
    }
    if (0U != (ierr & Exception_IERRMSX)) {
        DebugP_log0("  Missed stall exception\n");
    }
    if (0U != (ierr & Exception_IERRDFX)) {
        DebugP_log0("  Data fetch exception\n");
        DebugP_log1("  cpu_dmc_X_rstatus[2:0]=0x%x\n", ((uintptr_t)iesr15_0 & 0x7UL));
    }
    if (0U != (ierr & Exception_IERRSEX)) {
        DebugP_log0("  Streaming engine exception\n");
        switch (iesr19_16) {
          case 0:
            DebugP_log1("    L2 Return Error, cpu_se_N_rstatus[2:0]=0x%x\n",
                          ((uintptr_t)iesr15_0 & 0x7UL));
            break;
          case 1:
            DebugP_log1("    SE Internal Error, cpu_se_N_rstatus[2:0]=0x%x\n",
                          ((uintptr_t)iesr15_0 & 0x7UL));
            break;
          default:
            DebugP_log1("    unknown sub-type, iesr[19:16]=0x%x\n",
                          iesr19_16);
            break;
        }
    }
    if (0U != (ierr & Exception_IERREXX)) {
        DebugP_log0("  Execution exception\n");
        switch (iesr19_16) {
          case 0:
            DebugP_log0("    Executing PROT-mode only instructions in UNPROT mode\n");
            break;
          case 1:
            DebugP_log0("    Division by zero\n");
            break;
          default:
            DebugP_log1("    unknown sub-type, iesr[19:16]=0x%x\n",
                          iesr19_16);
            break;
        }
    }
    if (0U != (ierr & Exception_IERRADX)) {
        DebugP_log0("  Address exception\n");
        switch (iesr19_16) {
          case 0:
            DebugP_log0("    Out-of-range address on DMC memory address\n");
            break;
          case 1:
            DebugP_log0("    Out-of-range address on CR memory address\n");
            break;
          default:
            DebugP_log1("    unknown sub-type, iesr[19:16]=0x%x\n",
                          iesr19_16);
            break;
        }
    }
    if (0U != (ierr & Exception_IERRMMX)) {
        DebugP_log0("  MMA exception\n");
        switch (iesr19_16) {
          case 0:
            DebugP_log0("    MMA instruction when MMA isn't present\n");
            break;
          case 1:
            DebugP_log0("    MMA instruction when MMA isn't active\n");
            break;
          case 2:
            DebugP_log0("    MMA status error\n");
      DebugP_log1("    mma_rstatus=0x%x\n", iesr15_0);
            break;
          default:
            DebugP_log1("    unknown sub-type, iesr[19:16]=0x%x\n",
                          iesr19_16);
            break;
        }
    }

    if(gExceptionHooks.internalHook != (Exception_FuncPtr)NULL){
        gExceptionHooks.internalHook();
    }

    /* clear internal exceptions to allow them to be recognized again */
    __IERR = 0;
}

/* Module_state */
#ifdef __ti__
struct Exception_Module_State Exception_Module_state __attribute__ ((section(".data:Exception_Module_state")));
#elif !(defined(__MACH__) && defined(__APPLE__))
struct Exception_Module_State Exception_Module_state __attribute__ ((section(".data:Exception_Module_state")));
#endif
struct Exception_Module_State Exception_Module_state = {
    (uint64_t)0x0U,  /* nrp */
    (uint64_t)0x0U,  /* ntsr */
    (uint64_t)0x0U,  /* ierr */
    (uint64_t)0x0U,  /* iear */
    (uint64_t)0x0U,  /* iesr */
    ((void(*)(void))NULL),  /* returnHook */
    ((Exception_Context*)NULL),  /* excContext */
    ((char*)NULL),  /* excPtr */
    {0},  /* contextBuf */
};


