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
 * \ingroup DRV_SCICLIENT_MODULE
 * \defgroup SCICLIENT_FMW_CC_IF Sciclient CC REGISTER API Interface
 *
 * TIFS provides the message IDs and messages for configuring
 * compute cluster registers on K3 devices, including MSMC (Multicore Shared Memory Controller)
 * timeout gasket registers and other compute cluster functionality.

 *
 * @{
 */
/**
 *  \file   sciclient_cc.h
 *
 *  \brief  This file contains the definition of all the message IDs, message
 *          formats to be able to interact with the System Controller firmware
 *          for configuration of compute cluster registers.
 */

#ifndef SCICLIENT_CC_H_
#define SCICLIENT_CC_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 *  \brief Request for configuring the compute cluster DMSC wrap register.
 *
 *  \n<b>Message</b>:    #TISCI_MSG_CONFIGURE_CC_REGISTER
 *  \n<b>Request</b>:    #tisci_msg_cc_configuration_req
 *  \n<b>Response</b>:   #tisci_msg_cc_configuration_resp
 *
 *  \param  req             Pointer to compute cluster configuration payload
 *
 *  \param  resp            Pointer to compute cluster response payload
 *
 *  \param  timeout         Gives a sense of how long to wait for the operation.
 *                          Refer \ref Sciclient_ServiceOperationTimeout.
 *
 *  \return CSL_PASS on success, else failure
 */
int32_t Sciclient_configureCC(const struct tisci_msg_cc_configuration_req *req,
							  struct  tisci_msg_cc_configuration_resp *resp,
							  uint32_t timeout);

#ifdef __cplusplus
}
#endif
#endif /* #ifndef SCICLIENT_CC_H_ */
/* @} */
