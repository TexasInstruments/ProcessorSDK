/*
 *  Copyright (c) Texas Instruments Incorporated 2026
 *  All rights reserved.
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
 *  \ingroup  IP_FMA_RL7
 *  \defgroup IP_FMA_RL7_IMPLEMENTATION RL7 Register Check Implementation
 *
 *  @{
 */

/**
 *  \file     ip_fma_rl7.c
 *
 *  \brief    Rate limit L7 static registers specific wrappers for periodic readback implementation.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ip_fma_rl7.h>
#include <ti/csl/csl_types.h>

#if defined(SOC_J721S2)
#include <ti/csl/soc/j721s2/src/cslr_soc_baseaddress.h>
#elif defined(SOC_J784S4)
#include <ti/csl/soc/j784s4/src/cslr_soc_baseaddress.h>
#endif

/* ========================================================================== */
/*                           File Scope Variables                             */
/* ========================================================================== */

/*	==========================================================================	*/
/*	                          Macros & Typedefs                               	*/
/*	==========================================================================	*/

/* ========================================================================== */
/*                       Static Function Declarations                         */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

IpFma_Status IpFma_Rl7_GetReadRegs(IpFma_Rl7Regs* out)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == out))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc readRegs[] =
        {
            { RD_CTRL               , 0U, IPFMA_WIDTH_32 },
            { RD_BW_CIR             , 0U, IPFMA_WIDTH_32 },
            { RD_BW_PIR             , 0U, IPFMA_WIDTH_32 },
            { RD_BW_BURST_OFFSET    , 0U, IPFMA_WIDTH_32 },
            { RD_TXN                , 0U, IPFMA_WIDTH_32 }
        };

        status = IpFma_GetRegsValues(CSL_IGPU_MAIN_0_M0_VBUSM_R_ASYNC_BW_LIMITER0_REGS_BASE, readRegs, 5);

        if ((IPFMA_OK == status))
        {
            out->CTRL           = (uint32_t) readRegs[0].value;
            out->CIR            = (uint32_t) readRegs[1].value;
            out->PIR            = (uint32_t) readRegs[2].value;
            out->BURST_OFFSET   = (uint32_t) readRegs[3].value;
            out->TXN            = (uint32_t) readRegs[4].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Rl7_GetWriteRegs(IpFma_Rl7Regs* out)
{
    IpFma_Status status = IPFMA_OK;

    if ((NULL_PTR == out))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        IpFma_RegDesc writeRegs[] =
        {
            { WR_CTRL               , 0U, IPFMA_WIDTH_32 },
            { WR_BW_CIR             , 0U, IPFMA_WIDTH_32 },
            { WR_BW_PIR             , 0U, IPFMA_WIDTH_32 },
            { WR_BW_BURST_OFFSET    , 0U, IPFMA_WIDTH_32 },
            { WR_TXN                , 0U, IPFMA_WIDTH_32 }
        };

        status = IpFma_GetRegsValues(CSL_IGPU_MAIN_0_M0_VBUSM_W_ASYNC_BW_LIMITER0_REGS_BASE, writeRegs, 5);

        if ((IPFMA_OK == status))
        {
            out->CTRL           = (uint32_t) writeRegs[0].value;
            out->CIR            = (uint32_t) writeRegs[1].value;
            out->PIR            = (uint32_t) writeRegs[2].value;
            out->BURST_OFFSET   = (uint32_t) writeRegs[3].value;
            out->TXN            = (uint32_t) writeRegs[4].value;
        }
    }

    return status;
}

IpFma_Status IpFma_Rl7_CompareRegs(IpFma_Rl7Regs* expected, IpFma_Rl7Regs* actual)
{
    IpFma_Status status = IPFMA_OK;

    if (((NULL_PTR == expected)) || ((NULL_PTR == actual)))
    {
        status = IPFMA_E_PARAM;
    }
    else
    {
        if ((expected->CTRL != actual->CTRL))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->CIR != actual->CIR))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->PIR != actual->PIR))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->BURST_OFFSET != actual->BURST_OFFSET))
        {
            status = IPFMA_E_MISMATCH;
        }
        if ((IPFMA_OK == status) && (expected->TXN != actual->TXN))
        {
            status = IPFMA_E_MISMATCH;
        }
    }

    return status;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/** @} */