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
 *  \file dss_pipe_configs.h
 *
 *  \brief This file provides various test cases.
 *
 */

 #ifndef DSS_PIPE_CONFIGS_H_
 #define DSS_PIPE_CONFIGS_H_
 
 /* ========================================================================== */
 /*                             Include Files                                  */
 /* ========================================================================== */
 
 #include <ti/drv/dss/dss.h>
 
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
 
 /* None */
 
 /* ========================================================================== */
 /*                      Static Function Declarations                          */
 /* ========================================================================== */
 
 /* None */
 
 /* ========================================================================== */
 /*                            Global Variables                                */
 /* ========================================================================== */
 
/* bpp and pitch to be updated based on the format while parsing */

static DssTest_PipeConfig gPipeConfigC1= 
{
    .inWidth = 1920U,
    .inHeight = 1080U,
    .pitch = {0U, 0U, 0U, 0U, 0U, 0U},
    .inScanFmt = FVID2_SF_PROGRESSIVE,
    .outWidth = 1920U,
    .outHeight = 1080U,
    .scEnable = TRUE,
    .globalAlpha = 0xFFU,
    .preMultiplyAlpha = FALSE,
    .posx = 0U,
    .posy = 0U,
};

static DssTest_PipeConfig gPipeConfigC2 = 
{
    .inWidth = 480U,
    .inHeight = 360U,
    .pitch = {0U, 0U, 0U, 0U, 0U, 0U},
    .inScanFmt = FVID2_SF_PROGRESSIVE,
    .outWidth = 480U,
    .outHeight = 360U,
    .scEnable = FALSE,
    .globalAlpha = 0xFFU,
    .preMultiplyAlpha = FALSE,
    .posx = 0U,
    .posy = 0U,
};

static DssTest_PipeConfig gPipeConfigC3 = 
{
    .inWidth = 480U,
    .inHeight = 360U,
    .pitch = {0U, 0U, 0U, 0U, 0U, 0U},
    .inScanFmt = FVID2_SF_PROGRESSIVE,
    .outWidth = 480U,
    .outHeight = 360U,
    .scEnable = FALSE,
    .globalAlpha = 0xFFU,
    .preMultiplyAlpha = FALSE,
    .posx = 1920U-480U,
    .posy = 1080U-360U,
};

static DssTest_PipeConfig gPipeConfigScalingTest = 
{
    .inWidth = 480U,
    .inHeight = 360U,
    .pitch = {0U, 0U, 0U, 0U, 0U, 0U},
    .inScanFmt = FVID2_SF_PROGRESSIVE,
    .outWidth = 720,
    .outHeight = 540U,
    .scEnable = TRUE,
    .globalAlpha = 0xFFU,
    .preMultiplyAlpha = FALSE,
    .posx = 0U,
    .posy = 0U,
};

static DssTest_PipeConfig gPipeConfigYUV420SP_Packed= 
{
    .inWidth = 1920U,
    .inHeight = 1080U,
    .pitch = {1920U, 1920U, 0U, 0U, 0U, 0U},
    .inScanFmt = FVID2_SF_PROGRESSIVE,
    .outWidth = 1920U,
    .outHeight = 1080U,
    .scEnable = TRUE,
    .globalAlpha = 0xFFU,
    .preMultiplyAlpha = FALSE,
    .posx = 0U,
    .posy = 0U,
    .ccsFormat = FVID2_CCSF_BITS8_PACKED
};

static DssTest_PipeConfig gPipeConfigYUV420SPUnpacked= 
{
    .inWidth = 1920U,
    .inHeight = 1080U,
    .pitch = {1920U * 2U, 1920U * 2U, 0U, 0U, 0U, 0U},
    .inScanFmt = FVID2_SF_PROGRESSIVE,
    .outWidth = 1920U,
    .outHeight = 1080U,
    .scEnable = TRUE,
    .globalAlpha = 0xFFU,
    .preMultiplyAlpha = FALSE,
    .posx = 0U,
    .posy = 0U,
    .ccsFormat = FVID2_CCSF_BITS12_UNPACKED16
};

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSS_TEST_CONFIGS_H_ */