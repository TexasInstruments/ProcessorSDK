/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/*
 * CSL Mailbox
 *
 * This file implements the CSL Functional Layer for the Mailbox
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/drv/mailbox/soc/awr294x/csl_mbox.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/soc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void CSL_Mbox_memInit (CSL_mboxRegAddr* pMboxRegAddr)
{
    /* Initialize the memory for the mailbox */
    HW_WR_REG32(pMboxRegAddr->memInit, 1U);
}

uint32_t CSL_Mbox_isMeminitDone (const CSL_mboxRegAddr* pMboxRegAddr)
{
    uint32_t retVal = 0U;
    if (HW_RD_REG32(pMboxRegAddr->memInitDone) & 0x01U == 0x01U)
    {
        retVal = 1U;
        /* Clear init Done status. */
        HW_WR_REG32(pMboxRegAddr->memInitDone, 1U);
    }
    return (retVal);
}

void CSL_Mbox_triggerTxInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId)
{
    uint32_t     bits;
    bits = (processorId << 2U) ;
    /* raise interrupt to the processor */
    HW_WR_REG32(pMboxRegAddr->mboxWriteDone, (1U << bits));
}


uint32_t CSL_Mbox_getBoxFullIntr (CSL_mboxRegAddr* pMboxRegAddr)
{
    return (HW_RD_REG32(pMboxRegAddr->mboxReadReq));
}

void CSL_Mbox_clearBoxFullInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId)
{
    uint32_t     bits;
    bits = (processorId << 2U) ;
    /* raise interrupt to the processor */
    HW_WR_REG32(pMboxRegAddr->mboxReadReq, (1U << bits));
}

void CSL_Mbox_triggerAckInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId)
{
    /* raise interrupt to the processor */
#if defined (BUILD_MCU1_1)
    /* Read Done ack register for CR5A and CR5B are same.
       CR5A is from bit 0 to 7 and CR5B is from bit 8 to 15. */
    HW_WR_REG32(pMboxRegAddr->mboxReadDoneAck, ((1U << processorId) << 8));
#else
    HW_WR_REG32(pMboxRegAddr->mboxReadDoneAck, (1U << processorId));
#endif
}

uint32_t CSL_Mbox_getBoxEmptyIntr (CSL_mboxRegAddr* pMboxRegAddr)
{
    return (HW_RD_REG32(pMboxRegAddr->mboxReadDone));
}

void CSL_Mbox_clearTxAckInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId)
{
    uint32_t     bits;
    bits = (processorId << 2U) ;
    /* clear the interrupt */
    HW_WR_REG32(pMboxRegAddr->mboxReadDone, (1U << bits));
}
