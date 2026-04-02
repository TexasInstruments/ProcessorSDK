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

 #ifndef DSS_TEST_CONFIGS_H_
 #define DSS_TEST_CONFIGS_H_
 
 /* ========================================================================== */
 /*                             Include Files                                  */
 /* ========================================================================== */
 
 #include "dss_pipe_configs.h"
 
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
 
 /* All test ids are based of J721E test scenario Jira Ids */
 
 static DssTest_Params gDssTestCases[] =
 {
     /* DSI Tests */
     {
        .enableTest       = TRUE,
        .iterationCnt     = 1U,
        .numTests         = 17U,
        .testObj          =
        {
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3315,
                .testDesc = "DSS DSI 720P test\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_720P,
                .vpId = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams720p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = 
                {
                    &gPipeConfigC2,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3315,
                .testDesc = "DSS DSI 1080p test\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = 
                {
                    &gPipeConfigC2,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3542,
                .testDesc = "Dss crop test.\n\r",
                .feature = DSS_TEST_CROP,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigScalingTest,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3310,
                .testDesc = "Dss RGB24 with buffer needed.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_RGB24_888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3314,
                .testDesc = "Dss YUV422I_UYVY with buffer needed.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV422I_UYVY,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3313,
                .testDesc = "Dss YUV422I_YUYV with buffer needed.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV422I_YUYV,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3312,
                .testDesc = "Dss YUV420 Packed test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV420SP_UV,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigYUV420SP_Packed,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3886,
                /* Fvid2 format is same as 3312, pitch calculation is different on basis on unpacking */
                .testDesc = "Dss YUV420 unpacked test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV420SP_UV,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigYUV420SPUnpacked,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3311,
                .testDesc = "Dss BGR24 test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGR24_888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3309,
                .testDesc = "Dss ARGB32 test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_ARGB32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC2,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3544,
                .testDesc = "DSS Display RTOS run time scaling test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 2,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                    FVID2_DF_BGRA32_8888
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID2,
                    CSL_DSS_VID_PIPE_ID_VIDL2
                },
                .pipeConfig = {
                    &gPipeConfigScalingTest,
                    &gPipeConfigC3,
                }

            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
               .testId = 3308,
               .testDesc = "DSS Common Region 1 test\n\r",
               .feature = DSS_TEST_COMM_REG_1,
               .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

               /*
               * ---------------------DCTRL Configurations------------------
               */
               .lcdOutRes = DSS_LCD_OUT_RES_1080P,
               .vpId = CSL_DSS_VP_ID_3,
               .blankingParams = &gDsiBlankingParams1080p,
               /*
               * ------------------DSS Display Configurations---------------
               */
               .numPipes = 2,
               .inDataFmt = 
               {
                   FVID2_DF_BGRA32_8888,
                   FVID2_DF_BGRA32_8888
               },
               .pipeId = 
               {
                   CSL_DSS_VID_PIPE_ID_VID2,
                   CSL_DSS_VID_PIPE_ID_VIDL2
               },
               .pipeConfig = 
                {
                    &gPipeConfigC2,
                    &gPipeConfigC3,
                }
           
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3885,
                .testDesc = "Dss RGB16_565 test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,

                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_RGB16_565,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
               .testId = 3308,
               .testDesc = "DSS Display VID2/VIDL2 test\n\r",
               .feature = DSS_TEST_NONE,
               .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

               /*
               * ---------------------DCTRL Configurations------------------
               */
               .lcdOutRes = DSS_LCD_OUT_RES_1080P,
               .vpId = CSL_DSS_VP_ID_3,
               .blankingParams = &gDsiBlankingParams1080p,
               /*
               * ------------------DSS Display Configurations---------------
               */
               .numPipes = 2,
               .inDataFmt = 
               {
                   FVID2_DF_BGRA32_8888,
                   FVID2_DF_BGRA32_8888
               },
               .pipeId = 
               {
                   CSL_DSS_VID_PIPE_ID_VID2,
                   CSL_DSS_VID_PIPE_ID_VIDL2
               },
               .pipeConfig = 
                {
                    &gPipeConfigC2,
                    &gPipeConfigC3,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3884,
                .testDesc = "Dss BGR16_565 test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGR16_565,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3543,
                .testDesc = "Dss flip test.\n\r",
                .feature = DSS_TEST_FLIP,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC2,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 4734,
                .testDesc = "Dss flip test, with RGB24 and buffer needed.\n\r",
                .feature = DSS_TEST_FLIP,
                .bufferRequired = DSS_TEST_BUFFER_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_3,
                .blankingParams = &gDsiBlankingParams1080p,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_RGB24_888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
        },
     },
     
     /* EDP Tests */
     {
        .enableTest       = TRUE,
        .iterationCnt     = 1U,
        .numTests         = 18U,
        .testObj          =
        {
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3885,
                .testDesc = "Dss RGB16_565 test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId = CSL_DSS_VP_ID_1,

                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_RGB16_565,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
               .testId = 3308,
               .testDesc = "DSS Display VID2/VIDL2 test\n\r",
               .feature = DSS_TEST_NONE,
               .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

               /*
               * ---------------------DCTRL Configurations------------------
               */
               .lcdOutRes = DSS_LCD_OUT_RES_1080P,
               .vpId = CSL_DSS_VP_ID_1,
               /*
               * ------------------DSS Display Configurations---------------
               */
               .numPipes = 2,
               .inDataFmt = 
               {
                   FVID2_DF_BGRA32_8888,
                   FVID2_DF_BGRA32_8888
               },
               .pipeId = 
               {
                   CSL_DSS_VID_PIPE_ID_VID2,
                   CSL_DSS_VID_PIPE_ID_VIDL2
               },
               .pipeConfig = 
                {
                    &gPipeConfigC2,
                    &gPipeConfigC3,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3884,
                .testDesc = "Dss BGR16_565 test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGR16_565,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3543,
                .testDesc = "Dss flip test.\n\r",
                .feature = DSS_TEST_FLIP,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC2,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3542,
                .testDesc = "Dss crop test.\n\r",
                .feature = DSS_TEST_CROP,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigScalingTest,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3540,
                .testDesc = "Dss Overlay4 and VP4 test with BGRA32.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_4,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC2,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3310,
                .testDesc = "Dss RGB24 with buffer needed.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_RGB24_888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3314,
                .testDesc = "Dss YUV422I_UYVY with buffer needed.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV422I_UYVY,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3313,
                .testDesc = "Dss YUV422I_YUYV with buffer needed.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV422I_YUYV,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3312,
                .testDesc = "Dss YUV420 Packed test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV420SP_UV,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigYUV420SP_Packed,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3886,
                /* Fvid2 format is same as 3312, pitch calculation is different on basis on unpacking */
                .testDesc = "Dss YUV420 unpacked test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_YUV420SP_UV,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigYUV420SPUnpacked,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3311,
                .testDesc = "Dss BGR24 with buffer needed.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_BGR24_888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3309,
                .testDesc = "Dss ARGB32 test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_ARGB32_8888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC2,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 3544,
                .testDesc = "DSS Display RTOS run time scaling test.\n\r",
                .feature = DSS_TEST_NONE,
                .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 2,
                .inDataFmt = 
                {
                    FVID2_DF_BGRA32_8888,
                    FVID2_DF_BGRA32_8888
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID2,
                    CSL_DSS_VID_PIPE_ID_VIDL2
                },
                .pipeConfig = {
                    &gPipeConfigScalingTest,
                    &gPipeConfigC3,
                }

            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
                .testId = 4734,
                .testDesc = "Dss flip test, with RGB24 and buffer needed.\n\r",
                .feature = DSS_TEST_FLIP,
                .bufferRequired = DSS_TEST_BUFFER_REQURIED,
                /*
                * ---------------------DCTRL Configurations------------------
                */
                .lcdOutRes = DSS_LCD_OUT_RES_1080P,
                .vpId  = CSL_DSS_VP_ID_1,
                /*
                * ------------------DSS Display Configurations---------------
                */
                .numPipes = 1,
                .inDataFmt = 
                {
                    FVID2_DF_RGB24_888,
                },
                .pipeId = 
                {
                    CSL_DSS_VID_PIPE_ID_VID1,
                },
                .pipeConfig = {
                    &gPipeConfigC1,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
               .testId = 3308,
               .testDesc = "DSS Common Region 1 test\n\r",
               .feature = DSS_TEST_COMM_REG_1,
               .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

               /*
               * ---------------------DCTRL Configurations------------------
               */
               .lcdOutRes = DSS_LCD_OUT_RES_1080P,
               .vpId = CSL_DSS_VP_ID_1,
               /*
               * ------------------DSS Display Configurations---------------
               */
               .numPipes = 2,
               .inDataFmt = 
               {
                   FVID2_DF_BGRA32_8888,
                   FVID2_DF_BGRA32_8888
               },
               .pipeId = 
               {
                   CSL_DSS_VID_PIPE_ID_VID2,
                   CSL_DSS_VID_PIPE_ID_VIDL2
               },
               .pipeConfig = 
                {
                    &gPipeConfigC2,
                    &gPipeConfigC3,
                }
           
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
               .testId = 12844,
               .testDesc = "DSS DP to HDMI Convertor test\n\r",
               .feature = DSS_TEST_DP_TO_HDMI,
               .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

               /*
               * ---------------------DCTRL Configurations------------------
               */
               .lcdOutRes = DSS_LCD_OUT_RES_1080P,
               .vpId = CSL_DSS_VP_ID_1,
               /*
               * ------------------DSS Display Configurations---------------
               */
               .numPipes = 2,
               .inDataFmt = 
               {
                   FVID2_DF_BGRA32_8888,
                   FVID2_DF_BGRA32_8888
               },
               .pipeId = 
               {
                   CSL_DSS_VID_PIPE_ID_VID2,
                   CSL_DSS_VID_PIPE_ID_VIDL2
               },
               .pipeConfig = 
                {
                    &gPipeConfigC2,
                    &gPipeConfigC3,
                }
            },
            {
                /* 
                * --------------------Test defining Info------------------
                */
               .testId = 1234,
               .testDesc = "DSS Freeze Frame Test \n\r",
               .feature = DSS_TEST_FREEZE_FRAME,
               .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

               /*
               * ---------------------DCTRL Configurations------------------
               */
               .lcdOutRes = DSS_LCD_OUT_RES_1080P,
               .vpId = CSL_DSS_VP_ID_1,
               /*
               * ------------------DSS Display Configurations---------------
               */
               .numPipes = 2,
               .inDataFmt = 
               {
                   FVID2_DF_BGRA32_8888,
                   FVID2_DF_BGRA32_8888
               },
               .pipeId = 
               {
                   CSL_DSS_VID_PIPE_ID_VID1,
                   CSL_DSS_VID_PIPE_ID_VIDL1
               },
               .pipeConfig = 
                {
                    &gPipeConfigC2,
                    &gPipeConfigC3,
                }
            },
        }
     },
     /* HDMI Tests */
     {
         .enableTest       = TRUE,
         .iterationCnt     = 1U,
         .numTests         = 1U,
         .testObj          =
         {
             {
                /* 
                * --------------------Test defining Info------------------
                */
               .testId = 3308,
               .testDesc = "DSS Display VID2/VIDL2 test\n\r",
               .feature = DSS_TEST_NONE,
               .bufferRequired = DSS_TEST_BUFFER_NOT_REQURIED,

               /*
               * ---------------------DCTRL Configurations------------------
               */
               .lcdOutRes = DSS_LCD_OUT_RES_1080P,
               .vpId = CSL_DSS_VP_ID_1,
               /*
               * ------------------DSS Display Configurations---------------
               */
               .numPipes = 2,
               .inDataFmt = 
               {
                   FVID2_DF_BGRA32_8888,
                   FVID2_DF_BGRA32_8888
               },
               .pipeId = 
               {
                   CSL_DSS_VID_PIPE_ID_VID2,
                   CSL_DSS_VID_PIPE_ID_VIDL2
               },
               .pipeConfig = 
                {
                    &gPipeConfigC2,
                    &gPipeConfigC3,
                }
           
            },
            
         
         },
     },
     
 
 };
 
 DssTest_blankingParams gDsiBlankingParams720p = 
 {
    .hfp = 48U,
    .hbp = 80U,
    .hsync = 32U,
    .vfp = 4U,
    .vbp = 12U,
    .vsync = 5U,
    .pixelClk = 64000000ULL
 };

 DssTest_blankingParams gDsiBlankingParams1080p = 
 {
    .hfp = 60U,
    .hbp = 70U,
    .hsync = 62U,
    .vfp = 55U,
    .vbp = 60U,
    .vsync = 55U,
    .pixelClk = 158400000ULL
 };

 /* ========================================================================== */
 /*                       Static Function Definitions                          */
 /* ========================================================================== */
 
 /* None */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* #ifndef DSS_TEST_CONFIGS_H_ */