/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file pbist_defs.h
 *
 *  \brief PBIST header that defines SOC-specific structures and functions
 */

#ifndef PBIST_DEFS_H_
#define PBIST_DEFS_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdbool.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/csl_pbist.h>
#include <ti/csl/soc.h>

#include "bist_core_defs.h"

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define PBIST_REGION_LOCAL_BASE           (0x60000000u)

#define PBIST_REGION2_LOCAL_BASE          (0x68000000u)

#define PBIST_RAT_REGION_INDEX            0
#define PBIST_RAT_REGION2_INDEX           1

#define PBIST_REG_REGION_SIZE             (0x400u)
#define PBIST_REG_REGION2_SIZE            (0x10000u)

/* Firewall definitions */
#define FW_REGION_ENABLE                  (0xAU)
#define FW_MCU_R5F0_PRIVID                (96U)

typedef void (*PBIST_handlerPtr)(uint32_t instanceId);

/*
    InitRestore function : Initialize or Restore based on init flag
    init : TRUE  --> Initialize
    init : FALSE --> Restore
*/
typedef int32_t (*PBIST_auxInitRestoreFunctionPtr)(bool init);

typedef struct PBIST_TestHandle_s
{
  char               testName[PBIST_INSTANCE_NAME_MAX_LENGTH];
  SDL_PBIST_inst     pbistInst;
  uint32_t           tisciPBISTDeviceId;
  bool               procRstNeeded;
  bool               secondaryCoreNeeded;
  char               coreName[16];
  char               secCoreName[16];
  uint32_t           tisciProcId;
  uint32_t           tisciSecProcId;
  uint32_t           tisciDeviceId;
  uint32_t           tisciSecDeviceId;
  bool               coreCustPwrSeqNeeded;
  uint8_t            numPostPbistToCheck;
  uint32_t           numAuxDevices;
  uint32_t          *auxDeviceIdsP;
  PBIST_auxInitRestoreFunctionPtr auxInitRestoreFunction;
} PBIST_TestHandle_t;

extern PBIST_TestHandle_t PBIST_TestHandleArray[PBIST_MAX_INSTANCE+1];

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void PBIST_printPostStatus(SDL_PBIST_postResult *result);

#ifdef __cplusplus
}
#endif
#endif /* PBIST_DEFS_H_ */
