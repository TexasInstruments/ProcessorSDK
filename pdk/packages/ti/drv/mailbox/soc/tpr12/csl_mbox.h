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

/** \file       drv/mailbox/soc/tpr12/csl_mbox.h
 *
 *  \brief      CSL Mailbox interface file.
 *
 */

#ifndef CSL_MAILBOX_H_
#define CSL_MAILBOX_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <ti/csl/csl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*!
 *  \brief Mailbox hardware register addresses
 *
 *  This Structure contains the register addresses of the mailbox.
 *  The registers are part of the control module.
 *  The SoC file will define the structure with actual register addresses.
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct CSL_mboxRegAddr_
{
    /**
     * \brief  Initialize mailbox memory.
     */
    uintptr_t memInit;
    /**
     * \brief  Mailbox memory initialization complete status.
     */
    uintptr_t memInitDone;
    /**
     * \brief  Mailbox memory initialization status.
     */
    uintptr_t memInitStatus;
    /**
     * \brief  Mailbox write done for the remote proc.
     *         This will generate an interrupt to the remote core.
     */
    uintptr_t mboxWriteDone;
    /**
     * \brief  Mailbox read request from the remote proc.
     */
    uintptr_t mboxReadReq;
    /**
     * \brief  Mailbox read done ack for the remote proc.
     *         This will generate an interrupt to the remote core.
     */
    uintptr_t mboxReadDoneAck;
    /**
     * \brief  Mailbox read Done from the remote proc.
     *         This will be corresponding to the mboxWriteDone generated previously.
     */
    uintptr_t mboxReadDone;

} CSL_mboxRegAddr;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
\defgroup MAILBOX_DRIVER_CSL_FUNCTION       Mailbox Driver IP specific CSL Functions
\ingroup DRV_MAILBOX_MODULE
\brief
*   The section has a list of all the CSL functions of the Mailbox IP.
 @{ */

/**
 * \brief   This API is used to initialize the mailbox.
 *
 * \param   pMboxRegAddr    Pointer to the mailbox SOC Registers
 *
 * \return  None.
 *
 * \ingroup MAILBOX_DRIVER_CSL_FUNCTION
 *
 */
void CSL_Mbox_memInit (CSL_mboxRegAddr* pMboxRegAddr);

/**
 * \brief   This API is used to check if the memory Initialization for mailbox
 *          has been completed or not.
 *
 * \param   pMboxRegAddr    Pointer to the mailbox SOC Registers
 *
 * \retval  status          Memory Init status
 *                          1:  MemInit Completed
 *                          0:  MemInit Not complete
 *
 * \ingroup MAILBOX_DRIVER_CSL_FUNCTION
 *
 */
uint32_t CSL_Mbox_isMeminitDone (const CSL_mboxRegAddr* pMboxRegAddr);

/**
 * \brief   This API triggers the interrupt to other processor notifying them
 *          the message is sent to them over mailbox
 *
 * \param   pMboxRegAddr    Pointer to the mailbox SOC Registers
 *
 * \param   processorId     Id specifying the processor to interrupt
 *
 * \return  None.
 *
 * \ingroup MAILBOX_DRIVER_CSL_FUNCTION
 *
 */
void CSL_Mbox_triggerTxInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId);

/**
 * \brief   This API returns the box empty interrupt status.
 *
 * \param   pMboxRegAddr    Pointer to the mailbox SOC Registers
 *
 * \return  Box empty interrupt status.
 *
 * \ingroup MAILBOX_DRIVER_CSL_FUNCTION
 *
 */
uint32_t CSL_Mbox_getBoxEmptyIntr (CSL_mboxRegAddr* pMboxRegAddr);

/**
 * \brief   This API Clears the Tx Acknowledge interrupt
 *
 * \param   pMboxRegAddr    Pointer to the mailbox SOC Registers
 *
 * \param   processorId     Id specifying the processor for which the interrupt will be cleared
 *
 * \return  None.
 *
 * \ingroup MAILBOX_DRIVER_CSL_FUNCTION
 *
 */
void CSL_Mbox_clearTxAckInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId);

/**
 * \brief   This API returns the box full interrupt status.
 *
 * \param   pMboxRegAddr    Pointer to the mailbox SOC Registers
 *
 * \return  Box full interrupt status.
 *
 * \ingroup MAILBOX_DRIVER_CSL_FUNCTION
 *
 */
uint32_t CSL_Mbox_getBoxFullIntr (CSL_mboxRegAddr* pMboxRegAddr);

void CSL_Mbox_clearBoxFullInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId);

/**
 * \brief   This API triggers the interrupt to other processor acknowledgin that
 *          the message is read by the processor
 *
 * \param   pMboxRegAddr    Pointer to the mailbox SOC Registers
 *
 * \param   processorId     Id specifying the processor for which the Ack interrupt will be triggered
 *
 * \return  None.
 *
 * \ingroup MAILBOX_DRIVER_CSL_FUNCTION
 *
 */
void CSL_Mbox_triggerAckInterrupt (CSL_mboxRegAddr* pMboxRegAddr, uint32_t processorId);

/** @} */ /* end defgroup MAILBOX_DRIVER_CSL_FUNCTION */

#ifdef __cplusplus
}
#endif

#endif /* CSL_MAILBOX_H_ */
