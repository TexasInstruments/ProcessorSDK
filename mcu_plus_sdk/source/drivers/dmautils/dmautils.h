/*
 *  Copyright (c) Texas Instruments Incorporated 2019
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

//:TODO: Need to figure out def group and requirement Id here

/**
 *  \file dmautils.h
 *
 *  \brief DMA Utils API/interface file.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2494)
 */

#ifndef DMAUTILS_H_
#define DMAUTILS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#if !defined (MCU_PLUS_SDK)
#include "ti/csl/cslr.h"
#include "ti/csl/src/ip/udmap/V0/csl_udmap_tr.h"
#include <ti/drv/udma/dmautils/include/dmautils_autoincrement_3d.h>
#else
#include <drivers/hw_include/cslr.h>
#if defined(SOC_AM62AX) || defined(SOC_AM62DX) || defined(SOC_AM275X)
#include <drivers/udma.h>
#else
#include <drivers/dmautils/csl/udmap/v0/csl_udmap_tr.h>
#endif
#include <drivers/dmautils/include/dmautils_autoincrement_3d.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif



/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */



#ifdef __cplusplus
}
#endif

#endif /* #ifndef DMAUTILS_H_ */

/* @} */
