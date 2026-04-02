/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \file   ipc_config.h
 *
 *  \brief  This file has the configuration for specific SOC
 *
 *  \details
 *
 **/
#ifndef IPC_CONFIG_H_
#define IPC_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>

//#include <ti/csl/soc.h>
#include <ti/drv/sciclient/sciclient.h>

#if defined (SOC_AM64X)
#include <ti/csl/cslr_gtc.h>
#endif

/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */
#ifdef BUILD_MPU
#define MAILBOX_TEST_TIMER_FREQ_IN_HZ 200000000U
#endif

#ifdef BUILD_MCU
#define MAILBOX_TEST_TIMER_FREQ_IN_HZ 800000000U
#endif

#ifdef BUILD_M4F
#define MAILBOX_TEST_TIMER_FREQ_IN_HZ 200000000U
#endif

/* ========================================================================== */
/*                                 Structures                                 */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* MAILBOX_APP_H_ */
/********************************* End of file ******************************/
