/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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
*  \file lpm_s2r.h
*
*  \brief File containing the Binary in a C array.
*
*/
#ifndef LPM_S2R_H_
#define LPM_S2R_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define LPM_SRAM_S2R_SIZE_IN_BYTES (2728U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#define LPM_SRAM_S2R { \
    0xe24dd010U,     0xe28d0004U,     0xe5802000U,     0xe59d0004U, \
    0xe3a00000U,     0xe3a01c02U,     0xe2511001U,     0xf57ff05fU, \
    0xee072f5aU,     0xee072f56U,     0xf57ff04fU,     0x1afffff9U, \
    0xe2800001U,     0xe3500004U,     0x1afffff5U,     0xe303e490U, \
    0xe30bcc5aU,     0xe346e8efU,     0xe34dc172U,     0xe3012008U, \
    0xe30c7010U,     0xe34420f0U,     0xe3407068U,     0xe3003210U, \
    0xe3a04801U,     0xe582e000U,     0xe3403299U,     0xe582c004U, \
    0xe3092008U,     0xe3442300U,     0xe3a01401U,     0xe582e000U, \
    0xe582c004U,     0xe3022030U,     0xe3402299U,     0xe587e000U, \
    0xe587c004U,     0xe59263dcU,     0xe59253dcU,     0xe59300f0U, \
    0xe00447a6U,     0xe0011325U,     0xe1800004U,     0xe1800001U, \
    0xe58300f0U,     0xe59300ecU,     0xe3800401U,     0xe58300ecU, \
    0xe59200d0U,     0xe3c00001U,     0xe58200d0U,     0xe5920320U, \
    0xe3c00c02U,     0xe5820320U,     0xe5926000U,     0xe3002040U, \
    0xe3402001U,     0xe3160002U,     0x0a000003U,     0xe59300e8U, \
    0xe28210c0U,     0xe1800001U,     0xe58300e8U,     0xe3160004U, \
    0x0a000003U,     0xe59300e8U,     0xe3800c01U,     0xe3800401U, \
    0xe58300e8U,     0xe3a00000U,     0xe3051008U,     0xe3401011U, \
    0xe3a05945U,     0xe581e000U,     0xe581c004U,     0xe5850000U, \
    0xe58d000cU,     0xe59d000cU,     0xe3500063U,     0x8a000005U, \
    0xe59d000cU,     0xe2800001U,     0xe58d000cU,     0xe59d000cU, \
    0xe3500064U,     0x3afffff9U,     0xe5950000U,     0xe3046004U, \
    0xe3406011U,     0xe3800c01U,     0xe5850000U,     0xe596007cU, \
    0xe3100080U,     0x0afffffcU,     0xe5970010U,     0xe3800102U, \
    0xe5870010U,     0xe59600bcU,     0xe3800001U,     0xe58600bcU, \
    0xe59600bcU,     0xe3100001U,     0x0afffffcU,     0xe5960000U, \
    0xe3100080U,     0x0afffffcU,     0xe3a00000U,     0xe3a01945U, \
    0xe30f4efcU,     0xe34f4fffU,     0xe58600bcU,     0xe5910000U, \
    0xe0000004U,     0xe5810000U,     0xe30f1fffU,     0xe5930024U, \
    0xe34f10f0U,     0xe0000001U,     0xe3a01051U,     0xe5830024U, \
    0xe5930000U,     0xe7de0c11U,     0xe5830000U,     0xe5930284U, \
    0xe3100b01U,     0x0afffffcU,     0xe3a00b01U,     0xe3077f7fU, \
    0xe3046f4fU,     0xe583028cU,     0xe5930024U,     0xe0000007U, \
    0xe1500006U,     0x1afffffbU,     0xe59d0004U,     0xe3500002U, \
    0x1a000061U,     0xe30806a0U,     0xe30c1124U,     0xe3400001U, \
    0xe3401011U,     0xe30f3ff0U,     0xe34f3ddfU,     0xe58d0008U, \
    0xe3090008U,     0xe3440301U,     0xe580e000U,     0xe580c004U, \
    0xe30d0008U,     0xe3440301U,     0xe580e000U,     0xe580c004U, \
    0xe5910000U,     0xe0000003U,     0xe3073cdaU,     0xe3483a6bU, \
    0xe5810000U,     0xe30a1ef9U,     0xe348123cU,     0xe3000020U, \
    0xe3440413U,     0xe5803000U,     0xe5801004U,     0xe5901068U, \
    0xe1812002U,     0xe3081300U,     0xe3441301U,     0xe5802068U, \
    0xe51121a0U,     0xe3822c01U,     0xe50121a0U,     0xe5902024U, \
    0xe3822002U,     0xe5802024U,     0xe5902068U,     0xe3822c01U, \
    0xe5802068U,     0xe5902068U,     0xe3120c02U,     0x0afffffcU, \
    0xe5902068U,     0xe3c22c01U,     0xe5802068U,     0xe5902068U, \
    0xe3120c02U,     0x1afffffcU,     0xe5902068U,     0xe3043004U, \
    0xe3443300U,     0xe3057554U,     0xe3457555U,     0xe3822401U, \
    0xe5802068U,     0xe5932000U,     0xe3c22801U,     0xe5832000U, \
    0xe5817000U,     0xe5912000U,     0xe3822001U,     0xe5812000U, \
    0xea000002U,     0xe59d2008U,     0xe2422001U,     0xe58d2008U, \
    0xe59d2008U,     0xe3520000U,     0x1591200cU,     0x13520001U, \
    0x1afffff7U,     0xe59d1008U,     0xe3510000U,     0x1a00000bU, \
    0xe3a0100aU,     0xe58d100cU,     0xe59d100cU,     0xe3510000U, \
    0x1afffffcU,     0xea000005U,     0xe5901068U,     0xe3110402U, \
    0x1a000005U,     0xe59d1008U,     0xe2411001U,     0xe58d1008U, \
    0xe59d1008U,     0xe3510000U,     0x1afffff6U,     0xe59d0008U, \
    0xe3500000U,     0x1a000004U,     0xe3a0000aU,     0xe58d000cU, \
    0xe59d000cU,     0xe3500000U,     0x1afffffcU,     0xe59d5004U, \
    0xe3a00048U,     0xeb000106U,     0xe3a00086U,     0xe3a01003U, \
    0xeb00014aU,     0xe3a00048U,     0xeb000101U,     0xe3a0005aU, \
    0xeb000167U,     0xe3500000U,     0x0a00007fU,     0xe6af6070U, \
    0xe6ef7076U,     0xe3160001U,     0x0a000017U,     0xe3a0005bU, \
    0xeb00015fU,     0xe1a04000U,     0xe3100001U,     0x0a000004U, \
    0xe3a0005cU,     0xeb00015aU,     0xe1a01000U,     0xe3a0005cU, \
    0xeb000136U,     0xe3140002U,     0x0a000004U,     0xe3a0005dU, \
    0xeb000153U,     0xe1a01000U,     0xe3a0005dU,     0xeb00012fU, \
    0xe3140004U,     0x0a000004U,     0xe3a0005eU,     0xeb00014cU, \
    0xe1a01000U,     0xe3a0005eU,     0xeb000128U,     0xe3170002U, \
    0x0a000017U,     0xe3a0005fU,     0xeb000145U,     0xe1a04000U, \
    0xe3100001U,     0x0a000004U,     0xe3a00060U,     0xeb000140U, \
    0xe1a01000U,     0xe3a00060U,     0xeb00011cU,     0xe3140002U, \
    0x0a000004U,     0xe3a00061U,     0xeb000139U,     0xe1a01000U, \
    0xe3a00061U,     0xeb000115U,     0xe3140004U,     0x0a000004U, \
    0xe3a00062U,     0xeb000132U,     0xe1a01000U,     0xe3a00062U, \
    0xeb00010eU,     0xe3170004U,     0x0a00000cU,     0xe3a00063U, \
    0xeb00012bU,     0xe1a04000U,     0xe3100008U,     0x0a000004U, \
    0xe3a00064U,     0xeb000126U,     0xe1a01000U,     0xe3a00064U, \
    0xeb000102U,     0xe3a00063U,     0xe1a01004U,     0xeb0000ffU, \
    0xe3170008U,     0x0a000004U,     0xe3a00065U,     0xeb00011cU, \
    0xe1a01000U,     0xe3a00065U,     0xeb0000f8U,     0xe3170010U, \
    0x0a000004U,     0xe3a00066U,     0xeb000115U,     0xe1a01000U, \
    0xe3a00066U,     0xeb0000f1U,     0xe3170020U,     0x0a000004U, \
    0xe3a00067U,     0xeb00010eU,     0xe1a01000U,     0xe3a00067U, \
    0xeb0000eaU,     0xe3170040U,     0x0a000004U,     0xe3a00068U, \
    0xeb000107U,     0xe1a01000U,     0xe3a00068U,     0xeb0000e3U, \
    0xe3760001U,     0xca00001aU,     0xe3a00069U,     0xeb000100U, \
    0xe1a04000U,     0xe3100010U,     0x0a000004U,     0xe3a0006aU, \
    0xeb0000fbU,     0xe1a01000U,     0xe3a0006aU,     0xeb0000d7U, \
    0xe3140020U,     0x0a000004U,     0xe3a0006bU,     0xeb0000f4U, \
    0xe1a01000U,     0xe3a0006bU,     0xeb0000d0U,     0xe3140040U, \
    0x0a000004U,     0xe3a0006cU,     0xeb0000edU,     0xe1a01000U, \
    0xe3a0006cU,     0xeb0000c9U,     0xe3a00069U,     0xe1a01004U, \
    0xeb0000c6U,     0xe3a0005aU,     0xeb0000e5U,     0xe3a0004cU, \
    0xeb00007bU,     0xe3a00065U,     0xe3a01002U,     0xeb0000bfU, \
    0xe3a04080U,     0xe3550005U,     0x0a000002U,     0xe3a040c0U, \
    0xe3550002U,     0x1a000009U,     0xe3a00048U,     0xeb000070U, \
    0xe3a00085U,     0xe1a01004U,     0xeb0000b4U,     0xe3a0004cU, \
    0xeb00006bU,     0xe3a00085U,     0xe1a01004U,     0xeb0000afU, \
    0xe3a00048U,     0xeb000066U,     0xe3a00034U,     0xe3a010caU, \
    0xeb0000aaU,     0xe3a00048U,     0xeb000061U,     0xe3a00064U, \
    0xe3a01008U,     0xeb0000a5U,     0xe3550005U,     0x1a00000bU, \
    0xe3a00048U,     0xeb00005aU,     0xe3a00050U,     0xe3a010f7U, \
    0xeb00009eU,     0xe3a00048U,     0xeb000055U,     0xe3a00051U, \
    0xe3a0103fU,     0xeb000099U,     0xe3a040ffU,     0xea000007U, \
    0xe3550002U,     0x1a00000aU,     0xe3a00048U,     0xeb00004cU, \
    0xe3a00050U,     0xe3a010ffU,     0xeb000090U,     0xe3a040f7U, \
    0xe3a00048U,     0xeb000046U,     0xe3a0004fU,     0xe1a01004U, \
    0xeb00008aU,     0xe3a0004cU,     0xeb000041U,     0xe3a00032U, \
    0xe3a01001U,     0xeb000085U,     0xe3a0004cU,     0xeb00003cU, \
    0xe3a00033U,     0xe3a01001U,     0xeb000080U,     0xe3a0004cU, \
    0xeb000037U,     0xe3a0003dU,     0xeb00009dU,     0xe1a04000U, \
    0xe3805002U,     0xe3a0004cU,     0xeb000031U,     0xe3a0003dU, \
    0xe1a01005U,     0xeb000075U,     0xe3a0004cU,     0xe3844006U, \
    0xeb00002bU,     0xe3a0003dU,     0xe1a01004U,     0xeb00006fU, \
    0xe3a0004cU,     0xeb000026U,     0xe20510fbU,     0xe3a0003dU, \
    0xeb00006aU,     0xe3a0004cU,     0xeb000021U,     0xe3a0003dU, \
    0xe1a01004U,     0xeb000065U,     0xe3a00048U,     0xeb00001cU, \
    0xe3a000cbU,     0xe3a010baU,     0xeb000060U,     0xe3a00048U, \
    0xeb000017U,     0xe3a00086U,     0xe3a01000U,     0xeb00005bU, \
    0xeafffffeU,     0xe1a01211U,     0xe3a02601U,     0xe7903002U, \
    0xe0033001U,     0xe1530001U,     0x1afffffbU,     0xe12fff1eU, \
    0xe92d4010U,     0xe3a0c601U,     0xe1a0e000U,     0xe7be400cU, \
    0xe1c42312U,     0xe1822311U,     0xe1a01311U,     0xe58e2000U, \
    0xe790200cU,     0xe0022001U,     0xe1520001U,     0x1afffffbU, \
    0xe8bd8010U,     0xe3011aa8U,     0xe3441101U,     0xe5d12000U, \
    0xe1520000U,     0x012fff1eU,     0xe30c30f8U,     0xe5c10000U, \
    0xe3443301U,     0xe3002010U,     0xe3442212U,     0xe5931000U, \
    0xe3811701U,     0xe5831000U,     0xe5931004U,     0xe3811701U, \
    0xe5831004U,     0xe5921000U,     0xe3811002U,     0xe5821000U, \
    0xe5921000U,     0xe3811008U,     0xe5821000U,     0xe5921094U, \
    0xe3811902U,     0xe5821094U,     0xe5921080U,     0xe3110001U, \
    0x0afffffcU,     0xe59210a0U,     0xe3a03007U,     0xe7c71013U, \
    0xe3a03009U,     0xe58210a0U,     0xe59210a4U,     0xe7c71013U, \
    0xe58210a4U,     0xe59210a8U,     0xe7c71013U,     0xe30330fcU, \
    0xe34f3fffU,     0xe58210a8U,     0xe5921094U,     0xe0011003U, \
    0xe3073f7fU,     0xe34f3fffU,     0xe5821094U,     0xe5921088U, \
    0xe7cf101fU,     0xe5821088U,     0xe5921084U,     0xe0011003U, \
    0xe3a030b4U,     0xe5821084U,     0xe5921098U,     0xe7c91013U, \
    0xe5821098U,     0xe5921094U,     0xe3811902U,     0xe5821094U, \
    0xe5921080U,     0xe3110001U,     0x0afffffcU,     0xe592109cU, \
    0xe304c040U,     0xe7c9101fU,     0xe1813000U,     0xe3a000a1U, \
    0xe3a0109bU,     0xe582309cU,     0xe582c084U,     0xeaffffffU, \
    0xe92d4800U,     0xe3002024U,     0xe3442212U,     0xe5923000U, \
    0xe3130a01U,     0x1afffffcU,     0xe3a03002U,     0xe3a0e001U, \
    0xe5823074U,     0xe5923080U,     0xe3833c06U,     0xe3c33002U, \
    0xe5823080U,     0xe5923080U,     0xe383c003U,     0xe582c080U, \
    0xe5923000U,     0xe3130010U,     0x0afffffcU,     0xe31e0001U, \
    0x0a000002U,     0xe3a0e000U,     0xe5820078U,     0xeafffff7U, \
    0xe3a00010U,     0xe5821078U,     0xe5820004U,     0xe5920000U, \
    0xe3100004U,     0x0afffffcU,     0xe5920000U,     0xe5820004U, \
    0xe8bd8800U,     0xe3001024U,     0xe3a02000U,     0xe3441212U, \
    0xe5913000U,     0xe3130a01U,     0x0a000002U,     0xe2522001U, \
    0x1afffffaU,     0xea000001U,     0xe3520001U,     0x0a00003fU, \
    0xe3a02001U,     0xe5812074U,     0xe5912080U,     0xe3822c06U, \
    0xe3c22002U,     0xe5812080U,     0xe5912080U,     0xe3823001U, \
    0xe3a02000U,     0xe5813080U,     0xe5913000U,     0xe3130010U, \
    0x1a000002U,     0xe2522001U,     0x1afffffaU,     0xea000001U, \
    0xe3520001U,     0x0a00002dU,     0xe3a03010U,     0xe3a02000U, \
    0xe5810078U,     0xe5813004U,     0xe5910000U,     0xe3100004U, \
    0x1a000002U,     0xe2522001U,     0x1afffffaU,     0xea000001U, \
    0xe3520001U,     0x0a000021U,     0xe5910000U,     0xe3a02002U, \
    0xe5810004U,     0xe5910080U,     0xe7ca0492U,     0xe3a02001U, \
    0xe5810080U,     0xe5812074U,     0xe5910080U,     0xe3802003U, \
    0xe3a00000U,     0xe5812080U,     0xe5912000U,     0xe3120008U, \
    0x1a000003U,     0xe2500001U,     0x1afffffaU,     0xe3a00000U, \
    0xea000002U,     0xe3500001U,     0x0a00000cU,     0xe2600000U, \
    0xe5912078U,     0xe2600000U,     0xe5913000U,     0xe3130004U, \
    0x1a000002U,     0xe2500001U,     0x1afffffaU,     0xea000001U, \
    0xe3500001U,     0x0a000001U,     0xe5910000U,     0xe5810004U, \
    0xe6ef0072U,     0xe12fff1eU\
} /* 2728 bytes */

#endif
