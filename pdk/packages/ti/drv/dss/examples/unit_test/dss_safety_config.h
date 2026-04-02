/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 *  \file dss_test_configs.h
 *
 *  \brief This file provides various test cases.
 *
 */

 #ifndef DSS_SAFETY_CONFIGS_H_
 #define DSS_SAFETY_CONFIGS_H_
 
 /* ========================================================================== */
 /*                             Include Files                                  */
 /* ========================================================================== */
 
 #include <ti/drv/dss/dss.h>
 #include "dss_test.h"
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /* ========================================================================== */
 /*                           Macros & Typedefs                                */
 /* ========================================================================== */
 

 /* ========================================================================== */
 /*                         Structure Declarations                             */
 /* ========================================================================== */
   

 /* ========================================================================== */
 /*                          Function Declarations                             */
 /* ========================================================================== */
 
 void DssTest_vpSafetyErrCallback(uint32_t vpID, Dss_DctrlVpSafetyCbData safetyCbData, void *appData);
 void DssTest_pipelineSafetyErrCallback(uint32_t pipeId, uint32_t capturedSign, void *appData);
  
 /* ========================================================================== */
 /*                      Static Function Declarations                          */
 /* ========================================================================== */
 
 /* None */
 
 /* ========================================================================== */
 /*                            Global Variables                                */
 /* ========================================================================== */

 
Dss_DctrlVpSafetyChkParams gDssVpSafetyParams[DSS_NUM_SAFETY_REGIONS] =
{
    {
        .vpId = CSL_DSS_VP_ID_1,
        .safetySignSeedVal = 0xFFFFFFFFU,
        .regionSafetyChkCfg = {
            .regionId = CSL_DSS_VP_SAFETY_REGION_0,
            .referenceSign = 0x0U,
            .safetyChkCfg = {
                .safetyChkEnable = TRUE,
                .safetyChkMode  = CSL_DSS_SAFETY_CHK_FRAME_FREEZE_DETECT,
                .seedSelectEnable = FALSE,
                .thresholdValue = 10U,
                .frameSkip = CSL_DSS_SAFETY_CHK_FRAME_SKIP_NO,
                .regionPos = {
                    .startX = 0U,
                    .startY = 0U,
                },
                .regionSize = {
                    .width = 40U,
                    .height = 30U,
                }
            },
        },
    },
    {
        .vpId = CSL_DSS_VP_ID_1,
        .safetySignSeedVal = 0xFFFFFFFFU,
        .regionSafetyChkCfg = {
            .regionId = CSL_DSS_VP_SAFETY_REGION_1,
            .referenceSign = 0x0U,
            .safetyChkCfg = {
                .safetyChkEnable = TRUE,
                .safetyChkMode  = CSL_DSS_SAFETY_CHK_FRAME_FREEZE_DETECT,
                .seedSelectEnable = FALSE,
                .thresholdValue = 30U,
                .frameSkip = CSL_DSS_SAFETY_CHK_FRAME_SKIP_NO,
                .regionPos = {
                    .startX = 100U,
                    .startY = 100U,
                },
                .regionSize = {
                    .width = 80U,
                    .height = 60U,
                }
            },
        },
    },
    {
        .vpId = CSL_DSS_VP_ID_1,
        .safetySignSeedVal = 0xFFFFFFFFU,
        .regionSafetyChkCfg = {
            .regionId = CSL_DSS_VP_SAFETY_REGION_2,
            .referenceSign = 0x1CFB0593U,
            .safetyChkCfg = {
                .safetyChkEnable = TRUE,
                .safetyChkMode  = CSL_DSS_SAFETY_CHK_DATA_INTEGRITY,
                .seedSelectEnable = FALSE,
                .thresholdValue = 0x0U,
                .frameSkip = CSL_DSS_SAFETY_CHK_FRAME_SKIP_NO,
                .regionPos = {
                    .startX = 1440U,
                    .startY = 840U,
                },
                .regionSize = {
                    .width = 40U,
                    .height = 20U,
                }
            },
        },
    },
    {
        .vpId = CSL_DSS_VP_ID_1,
        .safetySignSeedVal = 0xFFFFFFFFU,
        .regionSafetyChkCfg = {
            .regionId = CSL_DSS_VP_SAFETY_REGION_3,
            .referenceSign = 0xF7364289U,
            .safetyChkCfg = {
                .safetyChkEnable = TRUE,
                .safetyChkMode  = CSL_DSS_SAFETY_CHK_DATA_INTEGRITY,
                .seedSelectEnable = FALSE,
                .thresholdValue = 0x0U,
                .frameSkip = CSL_DSS_SAFETY_CHK_FRAME_SKIP_NO,
                .regionPos = {
                    .startX = 1640U,
                    .startY = 920U,
                },
                .regionSize = {
                    .width = 60U,
                    .height = 40U,
                }
            },
        },
    },
};


Dss_DispPipeSafetyChkParams gDssPipelineSafetyParams[CSL_DSS_VID_PIPE_ID_MAX] =
{
    {
        .safetySignSeedVal = 0xFFFFFFFFU,
        .referenceSign = 0x0U,
        .safetyChkCfg = {
            .safetyChkEnable = TRUE,
            .safetyChkMode  = CSL_DSS_SAFETY_CHK_FRAME_FREEZE_DETECT,
            .seedSelectEnable = FALSE,
            .thresholdValue = 50U,
            .frameSkip = CSL_DSS_SAFETY_CHK_FRAME_SKIP_NO,
            .regionPos = {
                .startX = 0U,
                .startY = 0U,
            },
            .regionSize = {
                .width = 240U,
                .height = 120U,
            }
        },
    },
    {
        .safetySignSeedVal = 0xFFFFFFFFU,
        .referenceSign = 0x2C066D70U,
        .safetyChkCfg = {
            .safetyChkEnable = TRUE,
            .safetyChkMode  = CSL_DSS_SAFETY_CHK_DATA_INTEGRITY,
            .seedSelectEnable = FALSE,
            .thresholdValue = 0x0U,
            .frameSkip = CSL_DSS_SAFETY_CHK_FRAME_SKIP_NO,
            .regionPos = {
                .startX = 0U,
                .startY = 0U,
            },
            .regionSize = {
                .width = 240U,
                .height = 120U,
            }
        },
    },
};


#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSS_SAFETY_CONFIGS_H_ */