/*
 *  Copyright (c) Texas Instruments Incorporated 2018
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
/** ============================================================================
 *   @file  csl_lpddr.h
 *
 *   @path  $(CSLPATH)
 *
 *   @desc  This file contains the CSL API's for CSI-RX
 *  ============================================================================
 */
#ifndef CSL_LPDDR_TOP_H_
#define CSL_LPDDR_TOP_H_

#include <ti/csl/csl.h>
#include <ti/csl/tistdtypes.h>
#include <ti/csl/cslr_lpddr.h>

#if defined (SOC_J721E) || defined (SOC_J7200)
#include <stdbool.h>
#include <ti/csl/src/ip/lpddr/V0/cdn_errno.h>
#include <ti/csl/src/ip/lpddr/V0/lpddr4_if.h>
#include <ti/csl/src/ip/lpddr/V0/lpddr4_structs_if.h>
#include <ti/csl/src/ip/lpddr/V0/lpddr4_obj_if.h>
#include <ti/csl/src/ip/lpddr/V0/lpddr4_ctl_regs.h>
#endif

#if defined (SOC_AM64X) || defined (SOC_AM62X) || defined (SOC_AM62A) || defined (SOC_AM62PX) || defined (SOC_AM62LX) || defined (SOC_AM275X)
#include <stdbool.h>
#include <ti/csl/src/ip/lpddr/V1/cdn_errno.h>
#include <ti/csl/src/ip/lpddr/V1/lpddr4_if.h>
#include <ti/csl/src/ip/lpddr/V1/lpddr4_structs_if.h>
#include <ti/csl/src/ip/lpddr/V1/lpddr4_obj_if.h>
#endif

#if defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#include <stdbool.h>
#include <ti/csl/src/ip/lpddr/V2/cdn_errno.h>
#include <ti/csl/src/ip/lpddr/V2/lpddr4_if.h>
#include <ti/csl/src/ip/lpddr/V2/lpddr4_structs_if.h>
#include <ti/csl/src/ip/lpddr/V2/lpddr4_obj_if.h>
#include <ti/csl/src/ip/lpddr/V2/V2_1/lpddr4_ctl_regs.h>
#endif
#endif

