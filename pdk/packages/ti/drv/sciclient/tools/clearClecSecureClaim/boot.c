/*****************************************************************************/
/* Copyright (c) 2023 Texas Instruments Incorporated                         */
/* http://www.ti.com/                                                        */
/*                                                                           */
/*  Redistribution and  use in source  and binary forms, with  or without    */
/*  modification,  are permitted provided  that the  following conditions    */
/*  are met:                                                                 */
/*                                                                           */
/*     Redistributions  of source  code must  retain the  above copyright    */
/*     notice, this list of conditions and the following disclaimer.         */
/*                                                                           */
/*     Redistributions in binary form  must reproduce the above copyright    */
/*     notice, this  list of conditions  and the following  disclaimer in    */
/*     the  documentation  and/or   other  materials  provided  with  the    */
/*     distribution.                                                         */
/*                                                                           */
/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
/*     of its  contributors may  be used to  endorse or  promote products    */
/*     derived  from   this  software  without   specific  prior  written    */
/*     permission.                                                           */
/*                                                                           */
/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
/*                                                                           */
/*****************************************************************************/
#include <stdint.h>

#define  CLEC_NUM_EVT_IN            (2048U)

#define  CLEC_REGS_BASE             (0x78000000UL)
#define  CLEC_EVT_CFG_MRR(CFG)      (0x00001000U+((CFG)*0x10000U))

#define  CLEC_EVT_CFG_MRR_S_MASK    (0x80000000U)
#define  CLEC_EVT_CFG_MRR_S_SHIFT   (0x0000001FU)


/*****************************************************************************/
/* C_INT00() - C ENVIRONMENT ENTRY POINT                                     */
/*****************************************************************************/
#pragma CODE_SECTION(_c_int00_secure, ".text:_c_int00_secure")
void _c_int00_secure()
{
    volatile uint32_t *addr = (uint32_t *)CLEC_REGS_BASE;
    uint32_t evtNum         = 0U;
    uint32_t val            = 0U;

    for( evtNum = 0; evtNum < CLEC_NUM_EVT_IN; evtNum++ )
    {
        addr = (uint32_t *)(CLEC_REGS_BASE + CLEC_EVT_CFG_MRR(evtNum));
        val = *addr;
        /* Check if Secure Claim Bit is Set */
        if((val & CLEC_EVT_CFG_MRR_S_MASK) >> CLEC_EVT_CFG_MRR_S_SHIFT)
        {
            /* Clear Secure Claim Bit */
            *addr = val & ~(CLEC_EVT_CFG_MRR_S_MASK);
      }
   }

   asm("    IDLE");

}

