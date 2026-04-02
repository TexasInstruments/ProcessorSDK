/********************************************************************
 * Copyright (C) 2024 Texas Instruments Incorporated.
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
 *  Name        : cslr_dthe_v2_crc.h
 *  VPVERSION   : 3.0.367 - 2024.03.02.21.42.22
 *  VPREV       : 1.0.8
*/
#ifndef CSLR_DTHE_V2_CRC_H_
#define CSLR_DTHE_V2_CRC_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  : dthe_v2 CRC registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t S_CRC_CTRL;
    volatile uint8_t  Resv_32[28];
    volatile uint32_t S_CRC_SEED;
    volatile uint8_t  Resv_64[28];
    volatile uint32_t S_CRC_DIN;
    volatile uint8_t  Resv_96[28];
    volatile uint32_t S_CRC_RSLT_PP;
} CSL_dthe_v2_crc_sRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL                                           (0x00000000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_SEED                                           (0x00000020U)
#define CSL_DTHE_V2_CRC_S_S_CRC_DIN                                            (0x00000040U)
#define CSL_DTHE_V2_CRC_S_S_CRC_RSLT_PP                                        (0x00000060U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* S_CRC_CTRL */

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_TYPE_MASK                                 (0x0000000FU)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_TYPE_SHIFT                                (0x00000000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_TYPE_MAX                                  (0x0000000FU)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_ENDIAN_MASK                               (0x00000030U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_ENDIAN_SHIFT                              (0x00000004U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_ENDIAN_MAX                                (0x00000003U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES1_MASK                                 (0x00000040U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES1_SHIFT                                (0x00000006U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES1_MAX                                  (0x00000001U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_IBR_MASK                                  (0x00000080U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_IBR_SHIFT                                 (0x00000007U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_IBR_MAX                                   (0x00000001U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_OBR_MASK                                  (0x00000100U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_OBR_SHIFT                                 (0x00000008U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_OBR_MAX                                   (0x00000001U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_OINV_MASK                                 (0x00000200U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_OINV_SHIFT                                (0x00000009U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_OINV_MAX                                  (0x00000001U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES2_MASK                                 (0x00000C00U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES2_SHIFT                                (0x0000000AU)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES2_MAX                                  (0x00000003U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_BYTE_MASK                                 (0x00001000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_BYTE_SHIFT                                (0x0000000CU)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_BYTE_MAX                                  (0x00000001U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_INIT_MASK                                 (0x00006000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_INIT_SHIFT                                (0x0000000DU)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_INIT_MAX                                  (0x00000003U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_FLASH_ACC2CRC_MASK                        (0x00008000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_FLASH_ACC2CRC_SHIFT                       (0x0000000FU)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_FLASH_ACC2CRC_MAX                         (0x00000001U)

#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES3_MASK                                 (0xFFFF0000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES3_SHIFT                                (0x00000010U)
#define CSL_DTHE_V2_CRC_S_S_CRC_CTRL_RES3_MAX                                  (0x0000FFFFU)


/* S_CRC_SEED */

#define CSL_DTHE_V2_CRC_S_S_CRC_SEED_SEED_MASK                                 (0xFFFFFFFFU)
#define CSL_DTHE_V2_CRC_S_S_CRC_SEED_SEED_SHIFT                                (0x00000000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_SEED_SEED_MAX                                  (0xFFFFFFFFU)


/* S_CRC_DIN */

#define CSL_DTHE_V2_CRC_S_S_CRC_DIN_DATA_IN_MASK                               (0xFFFFFFFFU)
#define CSL_DTHE_V2_CRC_S_S_CRC_DIN_DATA_IN_SHIFT                              (0x00000000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_DIN_DATA_IN_MAX                                (0xFFFFFFFFU)


/* S_CRC_RSLT_PP */

#define CSL_DTHE_V2_CRC_S_S_CRC_RSLT_PP_RSLT_PP_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_CRC_S_S_CRC_RSLT_PP_RSLT_PP_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_CRC_S_S_CRC_RSLT_PP_RSLT_PP_MAX                            (0xFFFFFFFFU)


/**************************************************************************
* Hardware Region  : dthe_v2 CRC registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t P_CRC_CTRL;
    volatile uint8_t  Resv_32[28];
    volatile uint32_t P_CRC_SEED;
    volatile uint8_t  Resv_64[28];
    volatile uint32_t P_CRC_DIN;
    volatile uint8_t  Resv_96[28];
    volatile uint32_t P_CRC_RSLT_PP;
} CSL_dthe_v2_crc_pRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL                                           (0x00000000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_SEED                                           (0x00000020U)
#define CSL_DTHE_V2_CRC_P_P_CRC_DIN                                            (0x00000040U)
#define CSL_DTHE_V2_CRC_P_P_CRC_RSLT_PP                                        (0x00000060U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* P_CRC_CTRL */

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_TYPE_MASK                                 (0x0000000FU)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_TYPE_SHIFT                                (0x00000000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_TYPE_MAX                                  (0x0000000FU)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_ENDIAN_MASK                               (0x00000030U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_ENDIAN_SHIFT                              (0x00000004U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_ENDIAN_MAX                                (0x00000003U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES1_MASK                                 (0x00000040U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES1_SHIFT                                (0x00000006U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES1_MAX                                  (0x00000001U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_IBR_MASK                                  (0x00000080U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_IBR_SHIFT                                 (0x00000007U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_IBR_MAX                                   (0x00000001U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_OBR_MASK                                  (0x00000100U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_OBR_SHIFT                                 (0x00000008U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_OBR_MAX                                   (0x00000001U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_OINV_MASK                                 (0x00000200U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_OINV_SHIFT                                (0x00000009U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_OINV_MAX                                  (0x00000001U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES2_MASK                                 (0x00000C00U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES2_SHIFT                                (0x0000000AU)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES2_MAX                                  (0x00000003U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_BYTE_MASK                                 (0x00001000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_BYTE_SHIFT                                (0x0000000CU)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_BYTE_MAX                                  (0x00000001U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_INIT_MASK                                 (0x00006000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_INIT_SHIFT                                (0x0000000DU)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_INIT_MAX                                  (0x00000003U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_FLASH_ACC2CRC_MASK                        (0x00008000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_FLASH_ACC2CRC_SHIFT                       (0x0000000FU)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_FLASH_ACC2CRC_MAX                         (0x00000001U)

#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES3_MASK                                 (0xFFFF0000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES3_SHIFT                                (0x00000010U)
#define CSL_DTHE_V2_CRC_P_P_CRC_CTRL_RES3_MAX                                  (0x0000FFFFU)


/* P_CRC_SEED */

#define CSL_DTHE_V2_CRC_P_P_CRC_SEED_SEED_MASK                                 (0xFFFFFFFFU)
#define CSL_DTHE_V2_CRC_P_P_CRC_SEED_SEED_SHIFT                                (0x00000000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_SEED_SEED_MAX                                  (0xFFFFFFFFU)


/* P_CRC_DIN */

#define CSL_DTHE_V2_CRC_P_P_CRC_DIN_DATA_IN_MASK                               (0xFFFFFFFFU)
#define CSL_DTHE_V2_CRC_P_P_CRC_DIN_DATA_IN_SHIFT                              (0x00000000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_DIN_DATA_IN_MAX                                (0xFFFFFFFFU)


/* P_CRC_RSLT_PP */

#define CSL_DTHE_V2_CRC_P_P_CRC_RSLT_PP_RSLT_PP_MASK                           (0xFFFFFFFFU)
#define CSL_DTHE_V2_CRC_P_P_CRC_RSLT_PP_RSLT_PP_SHIFT                          (0x00000000U)
#define CSL_DTHE_V2_CRC_P_P_CRC_RSLT_PP_RSLT_PP_MAX                            (0xFFFFFFFFU)


#ifdef __cplusplus
}
#endif
#endif
