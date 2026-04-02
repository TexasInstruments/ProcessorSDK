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
 *  \defgroup IP_FMA_RL7 RL7 static Register Check Wrappers
 */

/**
 *  \ingroup  IP_FMA_RL7
 *  \defgroup IP_FMA_RL7_INTERFACE Register Check Interface.
 *
 *  @{
 */

/**
 *  \file     ip_fma_rl7.h
 *
 *  \brief    Register readback interface for rate limit static registers.
 *
 */

#ifndef IP_FMA_RL7_
#define IP_FMA_RL7_

/*	==========================================================================	*/
/*	                            Include Files                                 	*/
/*	==========================================================================	*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ip_fma_common.h>

/*	==========================================================================	*/
/*	                        Macros & Typedefs                            	    */
/*	==========================================================================	*/

/* Offsets for the rate limiter’s read registers */
#define RD_CTRL                 ((uint32_t) 0x4U)
#define RD_BW_CIR               ((uint32_t) 0x100U)
#define RD_BW_PIR               ((uint32_t) 0x104U)
#define RD_BW_BURST_OFFSET      ((uint32_t) 0x108U)
#define RD_TXN                  ((uint32_t) 0x300U)

/* Offsets for the rate limiter’s write registers */
#define WR_CTRL                 ((uint32_t) 0x4U)
#define WR_BW_CIR               ((uint32_t) 0x200U)
#define WR_BW_PIR               ((uint32_t) 0x204U)
#define WR_BW_BURST_OFFSET      ((uint32_t) 0x208U)
#define WR_TXN                  ((uint32_t) 0x400U)

/* Default values for the rate limiter’s read registers */
#define RD_CTRL_RESET_VALUE                 ((uint32_t) 0x0U)
#define RD_BW_CIR_RESET_VALUE               ((uint32_t) 0x0U)
#define RD_BW_PIR_RESET_VALUE               ((uint32_t) 0x0U)
#define RD_BW_BURST_OFFSET_RESET_VALUE      ((uint32_t) 0x0U)
#define RD_TXN_RESET_VALUE                  ((uint32_t) 0x40U)

/* Default values for the rate limiter’s write registers */
#define WR_CTRL_RESET_VALUE                 ((uint32_t) 0x0U)
#define WR_BW_CIR_RESET_VALUE               ((uint32_t) 0x0U)
#define WR_BW_PIR_RESET_VALUE               ((uint32_t) 0x0U)
#define WR_BW_BURST_OFFSET_RESET_VALUE      ((uint32_t) 0x0U)
#define WR_TXN_RESET_VALUE                  ((uint32_t) 0x40U)

/**
 *  \brief Rate limiter static configuration registers for read/write operations.
 */
typedef struct
{
    uint32_t    CIR;            /**< Read/Write Bandwidth Committed Information Rate Register.          */
    uint32_t    PIR;            /**< Read/Write Bandwidth Peak Information Rate Register.               */
    uint32_t    BURST_OFFSET;   /**< Read/Write Bandwidth Burst Offset Register                         */
    uint32_t    CTRL;           /**< Read/Write Rate Limiter Control Register                           */
    uint32_t    TXN;            /**< Rate Limiter max number of outstanding read/write transactions.    */
} IpFma_Rl7Regs;

/*	==========================================================================	*/
/*	                        Structure Declarations                            	*/
/*	==========================================================================	*/

/*	==========================================================================	*/
/*	                         Function Declarations                            	*/
/*	==========================================================================	*/

/**
 * \brief   Gets rate limiter static configuration "read" registers from memory.
 *
 * \param   out     Structure containing rate limiter "read" static configuration registers states.
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 			\ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Rl7_GetReadRegs(IpFma_Rl7Regs* out);

/**
 * \brief   Gets rate limiter static configuration "write" registers from memory.
 *
 * \param   out     Structure containing rate limiter "write" static configuration registers states.
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 			\ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Rl7_GetWriteRegs(IpFma_Rl7Regs* out);

/**
 * \brief   Compares rate limiter static configuration registers.
 *
 * \param	expected	Previous status of r5f registers
 * \param	actual      Current status of r5f registers
 *
 * \retval 	\ref        IPFMA_OK 			Registers value has not been modified
 * 			\ref	    IPFMA_E_MISMATCH	Registers value modified
 */
IpFma_Status IpFma_Rl7_CompareRegs(IpFma_Rl7Regs* expected, IpFma_Rl7Regs* actual);

/*	==========================================================================	*/
/*	                      Static Function Definitions                         	*/
/*	==========================================================================	*/

/* None */

#ifdef __cplusplus
}
#endif

#endif /*	#ifndef IP_FMA_RL7_	*/

/** @} */
