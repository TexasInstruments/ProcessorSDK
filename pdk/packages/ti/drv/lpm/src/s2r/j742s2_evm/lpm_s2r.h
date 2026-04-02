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

#define LPM_SRAM_S2R_SIZE_IN_BYTES (2940U)

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
    0xe24dd008U,     0xe1a0000dU,     0xe5802000U,     0xe59d0000U, \
    0xe3a00000U,     0xe3a01c02U,     0xe2511001U,     0xf57ff05fU, \
    0xee072f5aU,     0xee072f56U,     0xf57ff04fU,     0x1afffff9U, \
    0xe2800001U,     0xe3500004U,     0x1afffff5U,     0xe3033490U, \
    0xe30b7c5aU,     0xe34638efU,     0xe34d7172U,     0xe3010008U, \
    0xe3002000U,     0xe34400f0U,     0xe3402299U,     0xe30c1010U, \
    0xe3059008U,     0xe5803000U,     0xe3401068U,     0xe5807004U, \
    0xe3090008U,     0xe3440300U,     0xe301ab04U,     0xe30fbefcU, \
    0xe3409011U,     0xe344a101U,     0xe34fbfffU,     0xe5803000U, \
    0xe3a04000U,     0xe5807004U,     0xe3a06601U,     0xe5813000U, \
    0xe1a03002U,     0xe3a0c000U,     0xe5817004U,     0xe1a01003U, \
    0xe302040cU,     0xe7b17000U,     0xe5911000U,     0xe3a00801U, \
    0xe5935300U,     0xe00077a7U,     0xe3a00401U,     0xe0001321U, \
    0xe1857007U,     0xe3a00c21U,     0xe1871001U,     0xe1a07003U, \
    0xe5831300U,     0xe59312fcU,     0xe3811401U,     0xe58312fcU, \
    0xe7b71000U,     0xe1a00003U,     0xe3c11001U,     0xe5871000U, \
    0xe3021350U,     0xe7b01001U,     0xe3c11c02U,     0xe5801000U, \
    0xe5101320U,     0xe3110002U,     0x0a000004U,     0xe3007100U, \
    0xe59302f8U,     0xe3407001U,     0xe1800007U,     0xe58302f8U, \
    0xe3110004U,     0x0a000004U,     0xe3001100U,     0xe59302f8U, \
    0xe3401100U,     0xe1800001U,     0xe58302f8U,     0xe08c010cU, \
    0xe08a8180U,     0xe3030490U,     0xe34608efU,     0xe5890000U, \
    0xe30b0c5aU,     0xe34d0172U,     0xe5890004U,     0xe598e004U, \
    0xe78e4006U,     0xe58d4004U,     0xe59d0004U,     0xe3500063U, \
    0x8a000005U,     0xe59d0004U,     0xe2800001U,     0xe58d0004U, \
    0xe59d0004U,     0xe3500064U,     0x3afffff9U,     0xe1a0000eU, \
    0xe7b01006U,     0xe3811c01U,     0xe5801000U,     0xe598000cU, \
    0xe7901006U,     0xe3110080U,     0x0afffffcU,     0xe598001cU, \
    0xe3091de0U,     0xe34011a3U,     0xe1500001U,     0x8a000003U, \
    0xe5980014U,     0xe5901000U,     0xe3811102U,     0xea000027U, \
    0xe3061b40U,     0xe34017edU,     0xe1500001U,     0x8a00000dU, \
    0xe30f1f40U,     0xe5983018U,     0xe3471f22U,     0xe730f011U, \
    0xe5981014U,     0xe5935000U,     0xe3c5507fU,     0xe2400001U, \
    0xe1850000U,     0xe5830000U,     0xe5910000U,     0xe3c00102U, \
    0xe5810000U,     0xea000016U,     0xe30717a0U,     0xe3401fe2U, \
    0xe1500001U,     0x8a000002U,     0xe1c801d4U,     0xe3a05007U, \
    0xea000009U,     0xe30e1f40U,     0xe3411fc4U,     0xe1500001U, \
    0x9a000003U,     0xe3001800U,     0xe3421fafU,     0xe1500001U, \
    0x8a00001dU,     0xe1c801d4U,     0xe3a05003U,     0xe5913000U, \
    0xe7c63015U,     0xe5813000U,     0xe5901000U,     0xe3c11102U, \
    0xe5801000U,     0xe5980010U,     0xe1a01000U,     0xe7b13006U, \
    0xe3833001U,     0xe5813000U,     0xe7901006U,     0xe3110001U, \
    0x0afffffcU,     0xe5981008U,     0xe7913006U,     0xe3130080U, \
    0x0afffffcU,     0xe7804006U,     0xe28cc001U,     0xe7be0006U, \
    0xe35c0002U,     0xe08c110cU,     0xe000000bU,     0xe58e0000U, \
    0xe79a3181U,     0x1affff74U,     0xea000006U,     0xe3071fa0U, \
    0xe3431f91U,     0xe1500001U,     0x8affffe5U,     0xe1c801d4U, \
    0xe3a05001U,     0xeaffffdcU,     0xe30f4fffU,     0xe3a00000U, \
    0xe34f40f0U,     0xe3a01051U,     0xe3a07b01U,     0xe3076f7fU, \
    0xe3045f4fU,     0xe5923234U,     0xe0033004U,     0xe5823234U, \
    0xe5923210U,     0xe7de3c11U,     0xe5823210U,     0xe5923494U, \
    0xe3130b01U,     0x0afffffcU,     0xe582749cU,     0xe5923234U, \
    0xe0033006U,     0xe1530005U,     0x1afffffbU,     0xe2800001U, \
    0xe3500002U,     0xe0802100U,     0xe79a2182U,     0x1affffecU, \
    0xe59d0000U,     0xe3500002U,     0x1a000053U,     0xe3032490U, \
    0xe30b1c5aU,     0xe34628efU,     0xe34d1172U,     0xe30d0008U, \
    0xe3043084U,     0xe3440301U,     0xe3443301U,     0xe5802000U, \
    0xe5801004U,     0xe3050008U,     0xe3440301U,     0xe5802000U, \
    0xe30c2100U,     0xe5801004U,     0xe3a01721U,     0xe5930000U, \
    0xe3442301U,     0xe3800040U,     0xe5830000U,     0xe3a00502U, \
    0xe5821000U,     0xe5821004U,     0xe582007cU,     0xe5820080U, \
    0xe5820084U,     0xe3a02000U,     0xe5130040U,     0xe3801001U, \
    0xe3080310U,     0xe3440301U,     0xe5031040U,     0xe51011b0U, \
    0xe3811001U,     0xe50011b0U,     0xe5931000U,     0xe3811801U, \
    0xe5831000U,     0xe5931000U,     0xe3817401U,     0xe3021710U, \
    0xe5837000U,     0xe5937000U,     0xe3170402U,     0x1a00000aU, \
    0xe2511001U,     0xe2c22000U,     0xe1917002U,     0x1afffff8U, \
    0xe3a0100aU,     0xe58d1004U,     0xe59d1004U,     0xe3510000U, \
    0x1afffffcU,     0xe3a01000U,     0xe3a02000U,     0xe3037490U, \
    0xe3093008U,     0xe34678efU,     0xe3443301U,     0xe30b6c5aU, \
    0xe34d6172U,     0xe5837000U,     0xe3057554U,     0xe3457555U, \
    0xe5836004U,     0xe5807000U,     0xe1917002U,     0xe5903000U, \
    0xe3833001U,     0xe5803000U,     0x0a000006U,     0xe590300cU, \
    0xe3530001U,     0x0a000008U,     0xe2511001U,     0xe2c22000U, \
    0xe1913002U,     0x1afffff8U,     0xe3a0000aU,     0xe58d0004U, \
    0xe59d0004U,     0xe3500000U,     0x1afffffcU,     0xe59d5000U, \
    0xeb0000d9U,     0xe3a00086U,     0xe3a01003U,     0xeb00011eU, \
    0xeb0000d5U,     0xe3a0005aU,     0xeb00013cU,     0xe3500000U, \
    0x0a00007fU,     0xe6af6070U,     0xe6ef7076U,     0xe3160001U, \
    0x0a000017U,     0xe3a0005bU,     0xeb000134U,     0xe1a04000U, \
    0xe3100001U,     0x0a000004U,     0xe3a0005cU,     0xeb00012fU, \
    0xe1a01000U,     0xe3a0005cU,     0xeb00010bU,     0xe3140002U, \
    0x0a000004U,     0xe3a0005dU,     0xeb000128U,     0xe1a01000U, \
    0xe3a0005dU,     0xeb000104U,     0xe3140004U,     0x0a000004U, \
    0xe3a0005eU,     0xeb000121U,     0xe1a01000U,     0xe3a0005eU, \
    0xeb0000fdU,     0xe3170002U,     0x0a000017U,     0xe3a0005fU, \
    0xeb00011aU,     0xe1a04000U,     0xe3100001U,     0x0a000004U, \
    0xe3a00060U,     0xeb000115U,     0xe1a01000U,     0xe3a00060U, \
    0xeb0000f1U,     0xe3140002U,     0x0a000004U,     0xe3a00061U, \
    0xeb00010eU,     0xe1a01000U,     0xe3a00061U,     0xeb0000eaU, \
    0xe3140004U,     0x0a000004U,     0xe3a00062U,     0xeb000107U, \
    0xe1a01000U,     0xe3a00062U,     0xeb0000e3U,     0xe3170004U, \
    0x0a00000cU,     0xe3a00063U,     0xeb000100U,     0xe1a04000U, \
    0xe3100008U,     0x0a000004U,     0xe3a00064U,     0xeb0000fbU, \
    0xe1a01000U,     0xe3a00064U,     0xeb0000d7U,     0xe3a00063U, \
    0xe1a01004U,     0xeb0000d4U,     0xe3170008U,     0x0a000004U, \
    0xe3a00065U,     0xeb0000f1U,     0xe1a01000U,     0xe3a00065U, \
    0xeb0000cdU,     0xe3170010U,     0x0a000004U,     0xe3a00066U, \
    0xeb0000eaU,     0xe1a01000U,     0xe3a00066U,     0xeb0000c6U, \
    0xe3170020U,     0x0a000004U,     0xe3a00067U,     0xeb0000e3U, \
    0xe1a01000U,     0xe3a00067U,     0xeb0000bfU,     0xe3170040U, \
    0x0a000004U,     0xe3a00068U,     0xeb0000dcU,     0xe1a01000U, \
    0xe3a00068U,     0xeb0000b8U,     0xe3760001U,     0xca00001aU, \
    0xe3a00069U,     0xeb0000d5U,     0xe1a04000U,     0xe3100010U, \
    0x0a000004U,     0xe3a0006aU,     0xeb0000d0U,     0xe1a01000U, \
    0xe3a0006aU,     0xeb0000acU,     0xe3140020U,     0x0a000004U, \
    0xe3a0006bU,     0xeb0000c9U,     0xe1a01000U,     0xe3a0006bU, \
    0xeb0000a5U,     0xe3140040U,     0x0a000004U,     0xe3a0006cU, \
    0xeb0000c2U,     0xe1a01000U,     0xe3a0006cU,     0xeb00009eU, \
    0xe3a00069U,     0xe1a01004U,     0xeb00009bU,     0xe3a0005aU, \
    0xeb0000baU,     0xe3a04080U,     0xe3550005U,     0x0a000002U, \
    0xe3a040a0U,     0xe3550002U,     0x1a000003U,     0xeb00004aU, \
    0xe3a00085U,     0xe1a01004U,     0xeb00008fU,     0xeb000046U, \
    0xe3a00034U,     0xe3a010caU,     0xeb00008bU,     0xeb000042U, \
    0xe3a00064U,     0xe3a01008U,     0xeb000087U,     0xe3550005U, \
    0x1a000009U,     0xeb00003cU,     0xe3a00050U,     0xe3a010f7U, \
    0xeb000081U,     0xeb000038U,     0xe3a00051U,     0xe3a0103fU, \
    0xeb00007dU,     0xe3a040ffU,     0xea000006U,     0xe3550002U, \
    0x1a000008U,     0xeb000030U,     0xe3a00050U,     0xe3a010ffU, \
    0xeb000075U,     0xe3a040f7U,     0xeb00002bU,     0xe3a0004fU, \
    0xe1a01004U,     0xeb000070U,     0xeb000027U,     0xe3a00036U, \
    0xe3a01001U,     0xeb00006cU,     0xeb000023U,     0xe3a0003dU, \
    0xeb00008aU,     0xe3804020U,     0xeb00001fU,     0xe3a0003dU, \
    0xe1a01004U,     0xeb000064U,     0xeb00001bU,     0xe3a000cbU, \
    0xe3a010baU,     0xeb000060U,     0xeb000017U,     0xe3a00086U, \
    0xe3a01000U,     0xeb00005cU,     0xeafffffeU,     0xe1a01211U, \
    0xe3a02601U,     0xe7903002U,     0xe0033001U,     0xe1530001U, \
    0x1afffffbU,     0xe12fff1eU,     0xe92d4010U,     0xe3a0c601U, \
    0xe1a0e000U,     0xe7be400cU,     0xe1c42312U,     0xe1822311U, \
    0xe1a01311U,     0xe58e2000U,     0xe790200cU,     0xe0022001U, \
    0xe1520001U,     0x1afffffbU,     0xe8bd8010U,     0xe3010b7cU, \
    0xe3440101U,     0xe5d01000U,     0xe3510000U,     0x112fff1eU, \
    0xe30c30f8U,     0xe3a01001U,     0xe3443301U,     0xe3002010U, \
    0xe3442212U,     0xe5c01000U,     0xe5930000U,     0xe3800701U, \
    0xe5830000U,     0xe5930004U,     0xe3800701U,     0xe5830004U, \
    0xe5920000U,     0xe3800002U,     0xe5820000U,     0xe5920000U, \
    0xe3800008U,     0xe5820000U,     0xe5920094U,     0xe3800902U, \
    0xe5820094U,     0xe5920080U,     0xe3100001U,     0x0afffffcU, \
    0xe59200a0U,     0xe3a01007U,     0xe7c70011U,     0xe3a01009U, \
    0xe58200a0U,     0xe59200a4U,     0xe7c70011U,     0xe58200a4U, \
    0xe59200a8U,     0xe7c70011U,     0xe30310fcU,     0xe34f1fffU, \
    0xe58200a8U,     0xe5920094U,     0xe0000001U,     0xe3071f7fU, \
    0xe34f1fffU,     0xe5820094U,     0xe5920088U,     0xe7cf001fU, \
    0xe5820088U,     0xe5920084U,     0xe0000001U,     0xe3a010b4U, \
    0xe5820084U,     0xe5920098U,     0xe7c90011U,     0xe5820098U, \
    0xe5920094U,     0xe3800902U,     0xe5820094U,     0xe5920080U, \
    0xe3100001U,     0x0afffffcU,     0xe592309cU,     0xe3a00048U, \
    0xe304c040U,     0xe3a0109bU,     0xe7c93010U,     0xe3a000a1U, \
    0xe582309cU,     0xe582c084U,     0xeaffffffU,     0xe92d4800U, \
    0xe3002024U,     0xe3442212U,     0xe5923000U,     0xe3130a01U, \
    0x1afffffcU,     0xe3a03002U,     0xe3a0e001U,     0xe5823074U, \
    0xe5923080U,     0xe3833c06U,     0xe3c33002U,     0xe5823080U, \
    0xe5923080U,     0xe383c003U,     0xe582c080U,     0xe5923000U, \
    0xe3130010U,     0x0afffffcU,     0xe31e0001U,     0x0a000002U, \
    0xe3a0e000U,     0xe5820078U,     0xeafffff7U,     0xe3a00010U, \
    0xe5821078U,     0xe5820004U,     0xe5920000U,     0xe3100004U, \
    0x0afffffcU,     0xe5920000U,     0xe5820004U,     0xe8bd8800U, \
    0xe3001024U,     0xe3a02000U,     0xe3441212U,     0xe5913000U, \
    0xe3130a01U,     0x0a000002U,     0xe2522001U,     0x1afffffaU, \
    0xea000001U,     0xe3520001U,     0x0a00003fU,     0xe3a02001U, \
    0xe5812074U,     0xe5912080U,     0xe3822c06U,     0xe3c22002U, \
    0xe5812080U,     0xe5912080U,     0xe3823001U,     0xe3a02000U, \
    0xe5813080U,     0xe5913000U,     0xe3130010U,     0x1a000002U, \
    0xe2522001U,     0x1afffffaU,     0xea000001U,     0xe3520001U, \
    0x0a00002dU,     0xe3a03010U,     0xe3a02000U,     0xe5810078U, \
    0xe5813004U,     0xe5910000U,     0xe3100004U,     0x1a000002U, \
    0xe2522001U,     0x1afffffaU,     0xea000001U,     0xe3520001U, \
    0x0a000021U,     0xe5910000U,     0xe3a02002U,     0xe5810004U, \
    0xe5910080U,     0xe7ca0492U,     0xe3a02001U,     0xe5810080U, \
    0xe5812074U,     0xe5910080U,     0xe3802003U,     0xe3a00000U, \
    0xe5812080U,     0xe5912000U,     0xe3120008U,     0x1a000003U, \
    0xe2500001U,     0x1afffffaU,     0xe3a00000U,     0xea000002U, \
    0xe3500001U,     0x0a00000cU,     0xe2600000U,     0xe5912078U, \
    0xe2600000U,     0xe5913000U,     0xe3130004U,     0x1a000002U, \
    0xe2500001U,     0x1afffffaU,     0xea000001U,     0xe3500001U, \
    0x0a000001U,     0xe5910000U,     0xe5810004U,     0xe6ef0072U, \
    0xe12fff1eU,     0x02990000U,     0x00014000U,     0x00014004U, \
    0x00014080U,     0x000140c0U,     0x0068c020U,     0x0068c080U, \
    0x0124f800U,     0x3f917fa0U,     0x3f917fa0U,     0x029b0000U, \
    0x00014010U,     0x00014014U,     0x00014090U,     0x000140d0U, \
    0x0069a020U,     0x0069a080U,     0x0124f800U,     0x3f917fa0U, \
    0x3f917fa0U,     0x00000000U,     0x00000000U,     0x00000000U, \
    0x00000000U,     0x00000000U,     0x00000000U,     0x00000000U, \
    0x00000000U,     0x00000000U,     0x00000000U\
} /* 2940 bytes */

#endif
