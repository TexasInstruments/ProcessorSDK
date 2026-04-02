/*
 *  Copyright (c) Texas Instruments Incorporated 2018-2024
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
 *  \file safety_checkers_app_csirx_sensor.h
 *
 *  \brief CSIRX safety checkers application sensor configuration header
 *
 */

#ifndef SAFETY_CHECKERS_APP_CSIRX_SENSOR_H_
#define SAFETY_CHECKERS_APP_CSIRX_SENSOR_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/*None*/

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define SAFETY_CHECKERS_APP_CSIRX_UB960_CFG_NUM_REGS              ((uint32_t)39U)
/**< UB960 de-serializer configuration registers count */
#define SAFETY_CHECKERS_APP_CSIRX_UB960_CFG_ARRAY_WIDTH           ((uint32_t)3U)
/**< UB960 de-serializer configuration array width for entries (address,data,timeout) */


/* ========================================================================== */
/*                         Global Variables                                   */
/* ========================================================================== */

/**< UB960 De-serializer configuration */
uint16_t gSafetyCheckersAppCsirxUb960SensorCfg[SAFETY_CHECKERS_APP_CSIRX_UB960_CFG_NUM_REGS][SAFETY_CHECKERS_APP_CSIRX_UB960_CFG_ARRAY_WIDTH] = {
    {0x32, 0x01, 0x50},
    {0x1F, 0x05, 0x1},
    {0xC9, 0x32, 0x1},
    {0xB0, 0x1C, 0x1},
    {0xB1, 0x92, 0x1},
    {0xB2, 0x40, 0x1},
    {0xB0, 0x01, 0x1},
    {0xB1, 0x01, 0x1},
    {0xB2, 0x01, 0x1},
    {0xB1, 0x02, 0x1},
    {0xB2, 0xF3, 0x1},
    {0xB1, 0x03, 0x1},
    {0xB2, 0x2C, 0x1},
    {0xB1, 0x04, 0x1},
    {0xB2, 0x0F, 0x1},
    {0xB1, 0x05, 0x1},
    {0xB2, 0x00, 0x1},
    {0xB1, 0x06, 0x1},
    {0xB2, 0x02, 0x1},
    {0xB1, 0x07, 0x1},
    {0xB2, 0x80, 0x1},/*D0*/
    {0xB1, 0x08, 0x1},
    {0xB2, 0x04, 0x1},
    {0xB1, 0x09, 0x1},
    {0xB2, 0x38, 0x1},
    {0xB1, 0x0A, 0x1},
    {0xB2, 0x08, 0x1},
    {0xB1, 0x0B, 0x1},
    {0xB2, 0x80, 0x1},
    {0xB1, 0x0C, 0x1},
    {0xB2, 0x04, 0x1},
    {0xB1, 0x0D, 0x1},
    {0xB2, 0x7D, 0x1},
    {0xB1, 0x0E, 0x1},
    {0xB2, 0x07, 0x1},
    {0xB1, 0x0F, 0x1},
    {0xB2, 0x08, 0x1},
    {0x33, 0x02, 0x1},
    {0xFFF,0x00, 0x100},
};

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t SafetyCheckersApp_csirxSensorConfig(SafetyCheckersApp_CsirxInstObj*
                                            appInstObj);

/* ========================================================================== */
/*      Internal Function Declarations (Needed for other static inlines)      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef SAFETY_CHECKERS_APP_CSIRX_SENSOR_H_ */
/* @} */
