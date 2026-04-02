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
 *  Name        : cslr_dru.h
*/
#ifndef CSLR_DRU_H_
#define CSLR_DRU_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Module Base Offset Values
**************************************************************************/

#define CSL_DRU_ATOMIC_REGS_BASE                                               (0x00080000U)
#define CSL_DRU_CAUSE_REGS_BASE                                                (0x000E0000U)
#define CSL_DRU_CBASS_FW_REGS_BASE                                             (0x00000000U)
#define CSL_DRU_CBASS_GLB_REGS_BASE                                            (0x00000000U)
#define CSL_DRU_CHCORE_REGS_BASE                                               (0x000A0000U)
#define CSL_DRU_CHNRT_REGS_BASE                                                (0x00040000U)
#define CSL_DRU_CHRT_REGS_BASE                                                 (0x00060000U)
#define CSL_DRU_FW_MAIN_FW_REGS_BASE                                           (0x00000000U)
#define CSL_DRU_FW_MAIN_GLB_REGS_BASE                                          (0x00000000U)
#define CSL_DRU_MEM_ATT0_REGS_BASE                                             (0x00010000U)
#define CSL_DRU_MEM_ATT1_REGS_BASE                                             (0x00020000U)
#define CSL_DRU_MEM_ATT2_REGS_BASE                                             (0x00030000U)
#define CSL_DRU_MMU_REGS_BASE                                                  (0x0000A000U)
#define CSL_DRU_QUEUE_REGS_BASE                                                (0x00008000U)
#define CSL_DRU_REGS_BASE                                                      (0x00000000U)
#define CSL_DRU_SET_REGS_BASE                                                  (0x00004000U)
#define CSL_DRU_UTLB_REGS_BASE                                                 (0x0000C000U)


/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t DRU_PID;                   /* Peripheral ID Register */
    volatile uint64_t DRU_CAPABILITIES;          /* DRU Capabilities: Lists the capabilities of the channel for TR TYPE and formatting functions */
    volatile uint8_t  Resv_64[48];
    volatile uint64_t DRU_PRI_MASK0;             /* DRU Priority Mask */
} CSL_DRU_Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_PID                                                        (0x00000000U)
#define CSL_DRU_CAPABILITIES                                               (0x00000008U)
#define CSL_DRU_PRI_MASK0                                                  (0x00000040U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* DRU_PID */

#define CSL_DRU_PID_MINOR_MASK                                             (0x000000000000003FU)
#define CSL_DRU_PID_MINOR_SHIFT                                            (0x0000000000000000U)
#define CSL_DRU_PID_MINOR_MAX                                              (0x000000000000003FU)

#define CSL_DRU_PID_CUSTOM_MASK                                            (0x00000000000000C0U)
#define CSL_DRU_PID_CUSTOM_SHIFT                                           (0x0000000000000006U)
#define CSL_DRU_PID_CUSTOM_MAX                                             (0x0000000000000003U)

#define CSL_DRU_PID_MAJOR_MASK                                             (0x0000000000000700U)
#define CSL_DRU_PID_MAJOR_SHIFT                                            (0x0000000000000008U)
#define CSL_DRU_PID_MAJOR_MAX                                              (0x0000000000000007U)

#define CSL_DRU_PID_RTL_MASK                                               (0x000000000000F800U)
#define CSL_DRU_PID_RTL_SHIFT                                              (0x000000000000000BU)
#define CSL_DRU_PID_RTL_MAX                                                (0x000000000000001FU)

#define CSL_DRU_PID_FUNC_MASK                                              (0x000000000FFF0000U)
#define CSL_DRU_PID_FUNC_SHIFT                                             (0x0000000000000010U)
#define CSL_DRU_PID_FUNC_MAX                                               (0x0000000000000FFFU)

#define CSL_DRU_PID_BU_MASK                                                (0x0000000030000000U)
#define CSL_DRU_PID_BU_SHIFT                                               (0x000000000000001CU)
#define CSL_DRU_PID_BU_MAX                                                 (0x0000000000000003U)

#define CSL_DRU_PID_SCHEME_MASK                                            (0x00000000C0000000U)
#define CSL_DRU_PID_SCHEME_SHIFT                                           (0x000000000000001EU)
#define CSL_DRU_PID_SCHEME_MAX                                             (0x0000000000000003U)

#define CSL_DRU_PID_RSVD_MASK                                              (0xFFFFFFFF00000000U)
#define CSL_DRU_PID_RSVD_SHIFT                                             (0x0000000000000020U)
#define CSL_DRU_PID_RSVD_MAX                                               (0x00000000FFFFFFFFU)

/* DRU_CAPABILITIES */

#define CSL_DRU_CAPABILITIES_TYPE0_MASK                                    (0x0000000000000001U)
#define CSL_DRU_CAPABILITIES_TYPE0_SHIFT                                   (0x0000000000000000U)
#define CSL_DRU_CAPABILITIES_TYPE0_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE1_MASK                                    (0x0000000000000002U)
#define CSL_DRU_CAPABILITIES_TYPE1_SHIFT                                   (0x0000000000000001U)
#define CSL_DRU_CAPABILITIES_TYPE1_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE2_MASK                                    (0x0000000000000004U)
#define CSL_DRU_CAPABILITIES_TYPE2_SHIFT                                   (0x0000000000000002U)
#define CSL_DRU_CAPABILITIES_TYPE2_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE3_MASK                                    (0x0000000000000008U)
#define CSL_DRU_CAPABILITIES_TYPE3_SHIFT                                   (0x0000000000000003U)
#define CSL_DRU_CAPABILITIES_TYPE3_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE4_MASK                                    (0x0000000000000010U)
#define CSL_DRU_CAPABILITIES_TYPE4_SHIFT                                   (0x0000000000000004U)
#define CSL_DRU_CAPABILITIES_TYPE4_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE5_MASK                                    (0x0000000000000020U)
#define CSL_DRU_CAPABILITIES_TYPE5_SHIFT                                   (0x0000000000000005U)
#define CSL_DRU_CAPABILITIES_TYPE5_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE6_MASK                                    (0x0000000000000040U)
#define CSL_DRU_CAPABILITIES_TYPE6_SHIFT                                   (0x0000000000000006U)
#define CSL_DRU_CAPABILITIES_TYPE6_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE7_MASK                                    (0x0000000000000080U)
#define CSL_DRU_CAPABILITIES_TYPE7_SHIFT                                   (0x0000000000000007U)
#define CSL_DRU_CAPABILITIES_TYPE7_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE8_MASK                                    (0x0000000000000100U)
#define CSL_DRU_CAPABILITIES_TYPE8_SHIFT                                   (0x0000000000000008U)
#define CSL_DRU_CAPABILITIES_TYPE8_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE9_MASK                                    (0x0000000000000200U)
#define CSL_DRU_CAPABILITIES_TYPE9_SHIFT                                   (0x0000000000000009U)
#define CSL_DRU_CAPABILITIES_TYPE9_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE10_MASK                                   (0x0000000000000400U)
#define CSL_DRU_CAPABILITIES_TYPE10_SHIFT                                  (0x000000000000000AU)
#define CSL_DRU_CAPABILITIES_TYPE10_MAX                                    (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE11_MASK                                   (0x0000000000000800U)
#define CSL_DRU_CAPABILITIES_TYPE11_SHIFT                                  (0x000000000000000BU)
#define CSL_DRU_CAPABILITIES_TYPE11_MAX                                    (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE12_MASK                                   (0x0000000000001000U)
#define CSL_DRU_CAPABILITIES_TYPE12_SHIFT                                  (0x000000000000000CU)
#define CSL_DRU_CAPABILITIES_TYPE12_MAX                                    (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE13_MASK                                   (0x0000000000002000U)
#define CSL_DRU_CAPABILITIES_TYPE13_SHIFT                                  (0x000000000000000DU)
#define CSL_DRU_CAPABILITIES_TYPE13_MAX                                    (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE14_MASK                                   (0x0000000000004000U)
#define CSL_DRU_CAPABILITIES_TYPE14_SHIFT                                  (0x000000000000000EU)
#define CSL_DRU_CAPABILITIES_TYPE14_MAX                                    (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TYPE15_MASK                                   (0x0000000000008000U)
#define CSL_DRU_CAPABILITIES_TYPE15_SHIFT                                  (0x000000000000000FU)
#define CSL_DRU_CAPABILITIES_TYPE15_MAX                                    (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_TRSTATIC_MASK                                 (0x0000000000010000U)
#define CSL_DRU_CAPABILITIES_TRSTATIC_SHIFT                                (0x0000000000000010U)
#define CSL_DRU_CAPABILITIES_TRSTATIC_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_EOL_MASK                                      (0x0000000000020000U)
#define CSL_DRU_CAPABILITIES_EOL_SHIFT                                     (0x0000000000000011U)
#define CSL_DRU_CAPABILITIES_EOL_MAX                                       (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_LOCAL_TRIG_MASK                               (0x0000000000040000U)
#define CSL_DRU_CAPABILITIES_LOCAL_TRIG_SHIFT                              (0x0000000000000012U)
#define CSL_DRU_CAPABILITIES_LOCAL_TRIG_MAX                                (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_GLOBAL_TRIG_MASK                              (0x0000000000080000U)
#define CSL_DRU_CAPABILITIES_GLOBAL_TRIG_SHIFT                             (0x0000000000000013U)
#define CSL_DRU_CAPABILITIES_GLOBAL_TRIG_MAX                               (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_RSVD_CONF_SPEC_MASK                           (0x00000000FFF00000U)
#define CSL_DRU_CAPABILITIES_RSVD_CONF_SPEC_SHIFT                          (0x0000000000000014U)
#define CSL_DRU_CAPABILITIES_RSVD_CONF_SPEC_MAX                            (0x0000000000000FFFU)

#define CSL_DRU_CAPABILITIES_AMODE_MASK                                    (0x0000000700000000U)
#define CSL_DRU_CAPABILITIES_AMODE_SHIFT                                   (0x0000000000000020U)
#define CSL_DRU_CAPABILITIES_AMODE_MAX                                     (0x0000000000000007U)

#define CSL_DRU_CAPABILITIES_ELTYPE_MASK                                   (0x0000007800000000U)
#define CSL_DRU_CAPABILITIES_ELTYPE_SHIFT                                  (0x0000000000000023U)
#define CSL_DRU_CAPABILITIES_ELTYPE_MAX                                    (0x000000000000000FU)

#define CSL_DRU_CAPABILITIES_DFMT_MASK                                     (0x0000078000000000U)
#define CSL_DRU_CAPABILITIES_DFMT_SHIFT                                    (0x0000000000000027U)
#define CSL_DRU_CAPABILITIES_DFMT_MAX                                      (0x000000000000000FU)

#define CSL_DRU_CAPABILITIES_SECTR_MASK                                    (0x0000780000000000U)
#define CSL_DRU_CAPABILITIES_SECTR_SHIFT                                   (0x000000000000002BU)
#define CSL_DRU_CAPABILITIES_SECTR_MAX                                     (0x000000000000000FU)

#define CSL_DRU_CAPABILITIES_ACOMP_MASK                                    (0x0000800000000000U)
#define CSL_DRU_CAPABILITIES_ACOMP_SHIFT                                   (0x000000000000002FU)
#define CSL_DRU_CAPABILITIES_ACOMP_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_VCOMP_MASK                                    (0x0001000000000000U)
#define CSL_DRU_CAPABILITIES_VCOMP_SHIFT                                   (0x0000000000000030U)
#define CSL_DRU_CAPABILITIES_VCOMP_MAX                                     (0x0000000000000001U)

#define CSL_DRU_CAPABILITIES_RSVD_MASK                                     (0xFFFE000000000000U)
#define CSL_DRU_CAPABILITIES_RSVD_SHIFT                                    (0x0000000000000031U)
#define CSL_DRU_CAPABILITIES_RSVD_MAX                                      (0x0000000000007FFFU)

/* DRU_PRI_MASK0 */

#define CSL_DRU_PRI_MASK0_MASK0_MASK                                       (0x0000000000000001U)
#define CSL_DRU_PRI_MASK0_MASK0_SHIFT                                      (0x0000000000000000U)
#define CSL_DRU_PRI_MASK0_MASK0_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK1_MASK                                       (0x0000000000000002U)
#define CSL_DRU_PRI_MASK0_MASK1_SHIFT                                      (0x0000000000000001U)
#define CSL_DRU_PRI_MASK0_MASK1_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK2_MASK                                       (0x0000000000000004U)
#define CSL_DRU_PRI_MASK0_MASK2_SHIFT                                      (0x0000000000000002U)
#define CSL_DRU_PRI_MASK0_MASK2_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK3_MASK                                       (0x0000000000000008U)
#define CSL_DRU_PRI_MASK0_MASK3_SHIFT                                      (0x0000000000000003U)
#define CSL_DRU_PRI_MASK0_MASK3_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK4_MASK                                       (0x0000000000000010U)
#define CSL_DRU_PRI_MASK0_MASK4_SHIFT                                      (0x0000000000000004U)
#define CSL_DRU_PRI_MASK0_MASK4_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK5_MASK                                       (0x0000000000000020U)
#define CSL_DRU_PRI_MASK0_MASK5_SHIFT                                      (0x0000000000000005U)
#define CSL_DRU_PRI_MASK0_MASK5_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK6_MASK                                       (0x0000000000000040U)
#define CSL_DRU_PRI_MASK0_MASK6_SHIFT                                      (0x0000000000000006U)
#define CSL_DRU_PRI_MASK0_MASK6_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK7_MASK                                       (0x0000000000000080U)
#define CSL_DRU_PRI_MASK0_MASK7_SHIFT                                      (0x0000000000000007U)
#define CSL_DRU_PRI_MASK0_MASK7_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK8_MASK                                       (0x0000000000000100U)
#define CSL_DRU_PRI_MASK0_MASK8_SHIFT                                      (0x0000000000000008U)
#define CSL_DRU_PRI_MASK0_MASK8_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK9_MASK                                       (0x0000000000000200U)
#define CSL_DRU_PRI_MASK0_MASK9_SHIFT                                      (0x0000000000000009U)
#define CSL_DRU_PRI_MASK0_MASK9_MAX                                        (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK10_MASK                                      (0x0000000000000400U)
#define CSL_DRU_PRI_MASK0_MASK10_SHIFT                                     (0x000000000000000AU)
#define CSL_DRU_PRI_MASK0_MASK10_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK11_MASK                                      (0x0000000000000800U)
#define CSL_DRU_PRI_MASK0_MASK11_SHIFT                                     (0x000000000000000BU)
#define CSL_DRU_PRI_MASK0_MASK11_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK12_MASK                                      (0x0000000000001000U)
#define CSL_DRU_PRI_MASK0_MASK12_SHIFT                                     (0x000000000000000CU)
#define CSL_DRU_PRI_MASK0_MASK12_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK13_MASK                                      (0x0000000000002000U)
#define CSL_DRU_PRI_MASK0_MASK13_SHIFT                                     (0x000000000000000DU)
#define CSL_DRU_PRI_MASK0_MASK13_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK14_MASK                                      (0x0000000000004000U)
#define CSL_DRU_PRI_MASK0_MASK14_SHIFT                                     (0x000000000000000EU)
#define CSL_DRU_PRI_MASK0_MASK14_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK15_MASK                                      (0x0000000000008000U)
#define CSL_DRU_PRI_MASK0_MASK15_SHIFT                                     (0x000000000000000FU)
#define CSL_DRU_PRI_MASK0_MASK15_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK16_MASK                                      (0x0000000000010000U)
#define CSL_DRU_PRI_MASK0_MASK16_SHIFT                                     (0x0000000000000010U)
#define CSL_DRU_PRI_MASK0_MASK16_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK17_MASK                                      (0x0000000000020000U)
#define CSL_DRU_PRI_MASK0_MASK17_SHIFT                                     (0x0000000000000011U)
#define CSL_DRU_PRI_MASK0_MASK17_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK18_MASK                                      (0x0000000000040000U)
#define CSL_DRU_PRI_MASK0_MASK18_SHIFT                                     (0x0000000000000012U)
#define CSL_DRU_PRI_MASK0_MASK18_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK19_MASK                                      (0x0000000000080000U)
#define CSL_DRU_PRI_MASK0_MASK19_SHIFT                                     (0x0000000000000013U)
#define CSL_DRU_PRI_MASK0_MASK19_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK20_MASK                                      (0x0000000000100000U)
#define CSL_DRU_PRI_MASK0_MASK20_SHIFT                                     (0x0000000000000014U)
#define CSL_DRU_PRI_MASK0_MASK20_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK21_MASK                                      (0x0000000000200000U)
#define CSL_DRU_PRI_MASK0_MASK21_SHIFT                                     (0x0000000000000015U)
#define CSL_DRU_PRI_MASK0_MASK21_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK22_MASK                                      (0x0000000000400000U)
#define CSL_DRU_PRI_MASK0_MASK22_SHIFT                                     (0x0000000000000016U)
#define CSL_DRU_PRI_MASK0_MASK22_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK23_MASK                                      (0x0000000000800000U)
#define CSL_DRU_PRI_MASK0_MASK23_SHIFT                                     (0x0000000000000017U)
#define CSL_DRU_PRI_MASK0_MASK23_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK24_MASK                                      (0x0000000001000000U)
#define CSL_DRU_PRI_MASK0_MASK24_SHIFT                                     (0x0000000000000018U)
#define CSL_DRU_PRI_MASK0_MASK24_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK25_MASK                                      (0x0000000002000000U)
#define CSL_DRU_PRI_MASK0_MASK25_SHIFT                                     (0x0000000000000019U)
#define CSL_DRU_PRI_MASK0_MASK25_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK26_MASK                                      (0x0000000004000000U)
#define CSL_DRU_PRI_MASK0_MASK26_SHIFT                                     (0x000000000000001AU)
#define CSL_DRU_PRI_MASK0_MASK26_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK27_MASK                                      (0x0000000008000000U)
#define CSL_DRU_PRI_MASK0_MASK27_SHIFT                                     (0x000000000000001BU)
#define CSL_DRU_PRI_MASK0_MASK27_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK28_MASK                                      (0x0000000010000000U)
#define CSL_DRU_PRI_MASK0_MASK28_SHIFT                                     (0x000000000000001CU)
#define CSL_DRU_PRI_MASK0_MASK28_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK29_MASK                                      (0x0000000020000000U)
#define CSL_DRU_PRI_MASK0_MASK29_SHIFT                                     (0x000000000000001DU)
#define CSL_DRU_PRI_MASK0_MASK29_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK30_MASK                                      (0x0000000040000000U)
#define CSL_DRU_PRI_MASK0_MASK30_SHIFT                                     (0x000000000000001EU)
#define CSL_DRU_PRI_MASK0_MASK30_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK31_MASK                                      (0x0000000080000000U)
#define CSL_DRU_PRI_MASK0_MASK31_SHIFT                                     (0x000000000000001FU)
#define CSL_DRU_PRI_MASK0_MASK31_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK32_MASK                                      (0x0000000100000000U)
#define CSL_DRU_PRI_MASK0_MASK32_SHIFT                                     (0x0000000000000020U)
#define CSL_DRU_PRI_MASK0_MASK32_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK33_MASK                                      (0x0000000200000000U)
#define CSL_DRU_PRI_MASK0_MASK33_SHIFT                                     (0x0000000000000021U)
#define CSL_DRU_PRI_MASK0_MASK33_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK34_MASK                                      (0x0000000400000000U)
#define CSL_DRU_PRI_MASK0_MASK34_SHIFT                                     (0x0000000000000022U)
#define CSL_DRU_PRI_MASK0_MASK34_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK35_MASK                                      (0x0000000800000000U)
#define CSL_DRU_PRI_MASK0_MASK35_SHIFT                                     (0x0000000000000023U)
#define CSL_DRU_PRI_MASK0_MASK35_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK36_MASK                                      (0x0000001000000000U)
#define CSL_DRU_PRI_MASK0_MASK36_SHIFT                                     (0x0000000000000024U)
#define CSL_DRU_PRI_MASK0_MASK36_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK37_MASK                                      (0x0000002000000000U)
#define CSL_DRU_PRI_MASK0_MASK37_SHIFT                                     (0x0000000000000025U)
#define CSL_DRU_PRI_MASK0_MASK37_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK38_MASK                                      (0x0000004000000000U)
#define CSL_DRU_PRI_MASK0_MASK38_SHIFT                                     (0x0000000000000026U)
#define CSL_DRU_PRI_MASK0_MASK38_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK39_MASK                                      (0x0000008000000000U)
#define CSL_DRU_PRI_MASK0_MASK39_SHIFT                                     (0x0000000000000027U)
#define CSL_DRU_PRI_MASK0_MASK39_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK40_MASK                                      (0x0000010000000000U)
#define CSL_DRU_PRI_MASK0_MASK40_SHIFT                                     (0x0000000000000028U)
#define CSL_DRU_PRI_MASK0_MASK40_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK41_MASK                                      (0x0000020000000000U)
#define CSL_DRU_PRI_MASK0_MASK41_SHIFT                                     (0x0000000000000029U)
#define CSL_DRU_PRI_MASK0_MASK41_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK42_MASK                                      (0x0000040000000000U)
#define CSL_DRU_PRI_MASK0_MASK42_SHIFT                                     (0x000000000000002AU)
#define CSL_DRU_PRI_MASK0_MASK42_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK43_MASK                                      (0x0000080000000000U)
#define CSL_DRU_PRI_MASK0_MASK43_SHIFT                                     (0x000000000000002BU)
#define CSL_DRU_PRI_MASK0_MASK43_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK44_MASK                                      (0x0000100000000000U)
#define CSL_DRU_PRI_MASK0_MASK44_SHIFT                                     (0x000000000000002CU)
#define CSL_DRU_PRI_MASK0_MASK44_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK45_MASK                                      (0x0000200000000000U)
#define CSL_DRU_PRI_MASK0_MASK45_SHIFT                                     (0x000000000000002DU)
#define CSL_DRU_PRI_MASK0_MASK45_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK46_MASK                                      (0x0000400000000000U)
#define CSL_DRU_PRI_MASK0_MASK46_SHIFT                                     (0x000000000000002EU)
#define CSL_DRU_PRI_MASK0_MASK46_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK47_MASK                                      (0x0000800000000000U)
#define CSL_DRU_PRI_MASK0_MASK47_SHIFT                                     (0x000000000000002FU)
#define CSL_DRU_PRI_MASK0_MASK47_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK48_MASK                                      (0x0001000000000000U)
#define CSL_DRU_PRI_MASK0_MASK48_SHIFT                                     (0x0000000000000030U)
#define CSL_DRU_PRI_MASK0_MASK48_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK49_MASK                                      (0x0002000000000000U)
#define CSL_DRU_PRI_MASK0_MASK49_SHIFT                                     (0x0000000000000031U)
#define CSL_DRU_PRI_MASK0_MASK49_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK50_MASK                                      (0x0004000000000000U)
#define CSL_DRU_PRI_MASK0_MASK50_SHIFT                                     (0x0000000000000032U)
#define CSL_DRU_PRI_MASK0_MASK50_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK51_MASK                                      (0x0008000000000000U)
#define CSL_DRU_PRI_MASK0_MASK51_SHIFT                                     (0x0000000000000033U)
#define CSL_DRU_PRI_MASK0_MASK51_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK52_MASK                                      (0x0010000000000000U)
#define CSL_DRU_PRI_MASK0_MASK52_SHIFT                                     (0x0000000000000034U)
#define CSL_DRU_PRI_MASK0_MASK52_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK53_MASK                                      (0x0020000000000000U)
#define CSL_DRU_PRI_MASK0_MASK53_SHIFT                                     (0x0000000000000035U)
#define CSL_DRU_PRI_MASK0_MASK53_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK54_MASK                                      (0x0040000000000000U)
#define CSL_DRU_PRI_MASK0_MASK54_SHIFT                                     (0x0000000000000036U)
#define CSL_DRU_PRI_MASK0_MASK54_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK55_MASK                                      (0x0080000000000000U)
#define CSL_DRU_PRI_MASK0_MASK55_SHIFT                                     (0x0000000000000037U)
#define CSL_DRU_PRI_MASK0_MASK55_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK56_MASK                                      (0x0100000000000000U)
#define CSL_DRU_PRI_MASK0_MASK56_SHIFT                                     (0x0000000000000038U)
#define CSL_DRU_PRI_MASK0_MASK56_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK57_MASK                                      (0x0200000000000000U)
#define CSL_DRU_PRI_MASK0_MASK57_SHIFT                                     (0x0000000000000039U)
#define CSL_DRU_PRI_MASK0_MASK57_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK58_MASK                                      (0x0400000000000000U)
#define CSL_DRU_PRI_MASK0_MASK58_SHIFT                                     (0x000000000000003AU)
#define CSL_DRU_PRI_MASK0_MASK58_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK59_MASK                                      (0x0800000000000000U)
#define CSL_DRU_PRI_MASK0_MASK59_SHIFT                                     (0x000000000000003BU)
#define CSL_DRU_PRI_MASK0_MASK59_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK60_MASK                                      (0x1000000000000000U)
#define CSL_DRU_PRI_MASK0_MASK60_SHIFT                                     (0x000000000000003CU)
#define CSL_DRU_PRI_MASK0_MASK60_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK61_MASK                                      (0x2000000000000000U)
#define CSL_DRU_PRI_MASK0_MASK61_SHIFT                                     (0x000000000000003DU)
#define CSL_DRU_PRI_MASK0_MASK61_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK62_MASK                                      (0x4000000000000000U)
#define CSL_DRU_PRI_MASK0_MASK62_SHIFT                                     (0x000000000000003EU)
#define CSL_DRU_PRI_MASK0_MASK62_MAX                                       (0x0000000000000001U)

#define CSL_DRU_PRI_MASK0_MASK63_MASK                                      (0x8000000000000000U)
#define CSL_DRU_PRI_MASK0_MASK63_SHIFT                                     (0x000000000000003FU)
#define CSL_DRU_PRI_MASK0_MASK63_MAX                                       (0x0000000000000001U)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t MMU_PID;
    volatile uint8_t  Resv_256[248];
    volatile uint64_t TDRR;
    volatile uint8_t  Resv_320[56];
    volatile uint64_t TDFAR;
    volatile uint8_t  Resv_512[184];
    volatile uint64_t TLB_INV;
    volatile uint8_t  Resv_640[120];
    volatile uint64_t TLB_INVC;
    volatile uint8_t  Resv_704[56];
    volatile uint64_t TLB_DBG;
    volatile uint8_t  Resv_768[56];
    volatile uint64_t TLB_DBG_DATA0;
    volatile uint8_t  Resv_832[56];
    volatile uint64_t TLB_DBG_DATA1;
    volatile uint8_t  Resv_1024[184];
    volatile uint64_t SCR;
    volatile uint8_t  Resv_1088[56];
    volatile uint64_t TCR0;
    volatile uint8_t  Resv_1152[56];
    volatile uint64_t TCR1;
    volatile uint8_t  Resv_1216[56];
    volatile uint64_t TBR0;
    volatile uint8_t  Resv_1280[56];
    volatile uint64_t TBR1;
    volatile uint8_t  Resv_1344[56];
    volatile uint64_t MAR;
    volatile uint8_t  Resv_1408[56];
    volatile uint64_t TDAR;
    volatile uint8_t  Resv_2048[632];
    volatile uint64_t SCR_GS;
    volatile uint8_t  Resv_2112[56];
    volatile uint64_t TCR0_GS;
    volatile uint8_t  Resv_2176[56];
    volatile uint64_t TCR1_GS;
    volatile uint8_t  Resv_2240[56];
    volatile uint64_t TBR0_GS;
    volatile uint8_t  Resv_2304[56];
    volatile uint64_t TBR1_GS;
    volatile uint8_t  Resv_2368[56];
    volatile uint64_t MAR_GS;
    volatile uint8_t  Resv_2432[56];
    volatile uint64_t TDAR_GS;
    volatile uint8_t  Resv_3072[632];
    volatile uint64_t SCR_S;
    volatile uint8_t  Resv_3136[56];
    volatile uint64_t TCR0_S;
    volatile uint8_t  Resv_3200[56];
    volatile uint64_t TCR1_S;
    volatile uint8_t  Resv_3264[56];
    volatile uint64_t TBR0_S;
    volatile uint8_t  Resv_3328[56];
    volatile uint64_t TBR1_S;
    volatile uint8_t  Resv_3392[56];
    volatile uint64_t MAR_S;
    volatile uint8_t  Resv_3456[56];
    volatile uint64_t TDAR_S;
    volatile uint8_t  Resv_6144[2680];
    volatile uint64_t VCR;
    volatile uint8_t  Resv_6208[56];
    volatile uint64_t VTCR;
    volatile uint8_t  Resv_6272[56];
    volatile uint64_t VTBR;
    volatile uint8_t  Resv_6336[56];
    volatile uint64_t VTDAR;
} CSL_DRU_MMURegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_MMU_MMU_PID                                                    (0x00000000U)
#define CSL_DRU_MMU_TDRR                                                       (0x00000100U)
#define CSL_DRU_MMU_TDFAR                                                      (0x00000140U)
#define CSL_DRU_MMU_TLB_INV                                                    (0x00000200U)
#define CSL_DRU_MMU_TLB_INVC                                                   (0x00000280U)
#define CSL_DRU_MMU_TLB_DBG                                                    (0x000002C0U)
#define CSL_DRU_MMU_TLB_DBG_DATA0                                              (0x00000300U)
#define CSL_DRU_MMU_TLB_DBG_DATA1                                              (0x00000340U)
#define CSL_DRU_MMU_SCR                                                        (0x00000400U)
#define CSL_DRU_MMU_TCR0                                                       (0x00000440U)
#define CSL_DRU_MMU_TCR1                                                       (0x00000480U)
#define CSL_DRU_MMU_TBR0                                                       (0x000004C0U)
#define CSL_DRU_MMU_TBR1                                                       (0x00000500U)
#define CSL_DRU_MMU_MAR                                                        (0x00000540U)
#define CSL_DRU_MMU_TDAR                                                       (0x00000580U)
#define CSL_DRU_MMU_SCR_GS                                                     (0x00000800U)
#define CSL_DRU_MMU_TCR0_GS                                                    (0x00000840U)
#define CSL_DRU_MMU_TCR1_GS                                                    (0x00000880U)
#define CSL_DRU_MMU_TBR0_GS                                                    (0x000008C0U)
#define CSL_DRU_MMU_TBR1_GS                                                    (0x00000900U)
#define CSL_DRU_MMU_MAR_GS                                                     (0x00000940U)
#define CSL_DRU_MMU_TDAR_GS                                                    (0x00000980U)
#define CSL_DRU_MMU_SCR_S                                                      (0x00000C00U)
#define CSL_DRU_MMU_TCR0_S                                                     (0x00000C40U)
#define CSL_DRU_MMU_TCR1_S                                                     (0x00000C80U)
#define CSL_DRU_MMU_TBR0_S                                                     (0x00000CC0U)
#define CSL_DRU_MMU_TBR1_S                                                     (0x00000D00U)
#define CSL_DRU_MMU_MAR_S                                                      (0x00000D40U)
#define CSL_DRU_MMU_TDAR_S                                                     (0x00000D80U)
#define CSL_DRU_MMU_VCR                                                        (0x00001800U)
#define CSL_DRU_MMU_VTCR                                                       (0x00001840U)
#define CSL_DRU_MMU_VTBR                                                       (0x00001880U)
#define CSL_DRU_MMU_VTDAR                                                      (0x000018C0U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* MMU_PID */

#define CSL_DRU_MMU_MMU_PID_SCHEME_MASK                                        (0x00000000C0000000U)
#define CSL_DRU_MMU_MMU_PID_SCHEME_SHIFT                                       (0x000000000000001EU)
#define CSL_DRU_MMU_MMU_PID_SCHEME_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_MMU_PID_BU_MASK                                            (0x0000000030000000U)
#define CSL_DRU_MMU_MMU_PID_BU_SHIFT                                           (0x000000000000001CU)
#define CSL_DRU_MMU_MMU_PID_BU_MAX                                             (0x0000000000000003U)

#define CSL_DRU_MMU_MMU_PID_FUNC_MASK                                          (0x000000000FFF0000U)
#define CSL_DRU_MMU_MMU_PID_FUNC_SHIFT                                         (0x0000000000000010U)
#define CSL_DRU_MMU_MMU_PID_FUNC_MAX                                           (0x0000000000000FFFU)

#define CSL_DRU_MMU_MMU_PID_R_MASK                                             (0x000000000000F800U)
#define CSL_DRU_MMU_MMU_PID_R_SHIFT                                            (0x000000000000000BU)
#define CSL_DRU_MMU_MMU_PID_R_MAX                                              (0x000000000000001FU)

#define CSL_DRU_MMU_MMU_PID_X_MASK                                             (0x0000000000000700U)
#define CSL_DRU_MMU_MMU_PID_X_SHIFT                                            (0x0000000000000008U)
#define CSL_DRU_MMU_MMU_PID_X_MAX                                              (0x0000000000000007U)

#define CSL_DRU_MMU_MMU_PID_CUSTOM_MASK                                        (0x00000000000000C0U)
#define CSL_DRU_MMU_MMU_PID_CUSTOM_SHIFT                                       (0x0000000000000006U)
#define CSL_DRU_MMU_MMU_PID_CUSTOM_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_MMU_PID_Y_MASK                                             (0x000000000000003FU)
#define CSL_DRU_MMU_MMU_PID_Y_SHIFT                                            (0x0000000000000000U)
#define CSL_DRU_MMU_MMU_PID_Y_MAX                                              (0x000000000000003FU)

/* TDRR */

#define CSL_DRU_MMU_TDRR_RSVD0_MASK                                            (0x7E00000000000000U)
#define CSL_DRU_MMU_TDRR_RSVD0_SHIFT                                           (0x0000000000000039U)
#define CSL_DRU_MMU_TDRR_RSVD0_MAX                                             (0x000000000000003FU)

#define CSL_DRU_MMU_TDRR_PREF_MASK                                             (0x0100000000000000U)
#define CSL_DRU_MMU_TDRR_PREF_SHIFT                                            (0x0000000000000038U)
#define CSL_DRU_MMU_TDRR_PREF_MAX                                              (0x0000000000000001U)

#define CSL_DRU_MMU_TDRR_OUTER_MASK                                            (0x00C0000000000000U)
#define CSL_DRU_MMU_TDRR_OUTER_SHIFT                                           (0x0000000000000036U)
#define CSL_DRU_MMU_TDRR_OUTER_MAX                                             (0x0000000000000003U)

#define CSL_DRU_MMU_TDRR_INNER_MASK                                            (0x0030000000000000U)
#define CSL_DRU_MMU_TDRR_INNER_SHIFT                                           (0x0000000000000034U)
#define CSL_DRU_MMU_TDRR_INNER_MAX                                             (0x0000000000000003U)

#define CSL_DRU_MMU_TDRR_MEMTYPE_MASK                                          (0x000C000000000000U)
#define CSL_DRU_MMU_TDRR_MEMTYPE_SHIFT                                         (0x0000000000000032U)
#define CSL_DRU_MMU_TDRR_MEMTYPE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_TDRR_SHARE_MASK                                            (0x0003000000000000U)
#define CSL_DRU_MMU_TDRR_SHARE_SHIFT                                           (0x0000000000000030U)
#define CSL_DRU_MMU_TDRR_SHARE_MAX                                             (0x0000000000000003U)

#define CSL_DRU_MMU_TDRR_RSVD1_MASK                                            (0x0000FF0000000000U)
#define CSL_DRU_MMU_TDRR_RSVD1_SHIFT                                           (0x0000000000000028U)
#define CSL_DRU_MMU_TDRR_RSVD1_MAX                                             (0x00000000000000FFU)

#define CSL_DRU_MMU_TDRR_ADDR_MASK                                             (0x000000FFFFFFF000U)
#define CSL_DRU_MMU_TDRR_ADDR_SHIFT                                            (0x000000000000000CU)
#define CSL_DRU_MMU_TDRR_ADDR_MAX                                              (0x000000000FFFFFFFU)

#define CSL_DRU_MMU_TDRR_STATUS_MASK                                           (0x0000000000000FFFU)
#define CSL_DRU_MMU_TDRR_STATUS_SHIFT                                          (0x0000000000000000U)
#define CSL_DRU_MMU_TDRR_STATUS_MAX                                            (0x0000000000000FFFU)

/* TDFAR */


/* TLB_INV */

#define CSL_DRU_MMU_TLB_INV_ID_MASK                                            (0x00FF000000000000U)
#define CSL_DRU_MMU_TLB_INV_ID_SHIFT                                           (0x0000000000000030U)
#define CSL_DRU_MMU_TLB_INV_ID_MAX                                             (0x00000000000000FFU)

#define CSL_DRU_MMU_TLB_INV_RSVD1_MASK                                         (0x0000F80000000000U)
#define CSL_DRU_MMU_TLB_INV_RSVD1_SHIFT                                        (0x000000000000002BU)
#define CSL_DRU_MMU_TLB_INV_RSVD1_MAX                                          (0x000000000000001FU)

#define CSL_DRU_MMU_TLB_INV_INV_TYPE_MASK                                      (0x0000070000000000U)
#define CSL_DRU_MMU_TLB_INV_INV_TYPE_SHIFT                                     (0x0000000000000028U)
#define CSL_DRU_MMU_TLB_INV_INV_TYPE_MAX                                       (0x0000000000000007U)

#define CSL_DRU_MMU_TLB_INV_ASID_MASK                                          (0x0000008000000000U)
#define CSL_DRU_MMU_TLB_INV_ASID_SHIFT                                         (0x0000000000000027U)
#define CSL_DRU_MMU_TLB_INV_ASID_MAX                                           (0x0000000000000001U)

#define CSL_DRU_MMU_TLB_INV_VA_MASK                                            (0x0000004000000000U)
#define CSL_DRU_MMU_TLB_INV_VA_SHIFT                                           (0x0000000000000026U)
#define CSL_DRU_MMU_TLB_INV_VA_MAX                                             (0x0000000000000001U)

#define CSL_DRU_MMU_TLB_INV_LL_MASK                                            (0x0000002000000000U)
#define CSL_DRU_MMU_TLB_INV_LL_SHIFT                                           (0x0000000000000025U)
#define CSL_DRU_MMU_TLB_INV_LL_MAX                                             (0x0000000000000001U)

#define CSL_DRU_MMU_TLB_INV_ADDR_MASK                                          (0x0000001FFFFFFFFFU)
#define CSL_DRU_MMU_TLB_INV_ADDR_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_TLB_INV_ADDR_MAX                                           (0x0000001FFFFFFFFFU)

/* TLB_INVC */

#define CSL_DRU_MMU_TLB_INVC_COMP_MASK                                         (0x0000000000000001U)
#define CSL_DRU_MMU_TLB_INVC_COMP_SHIFT                                        (0x0000000000000000U)
#define CSL_DRU_MMU_TLB_INVC_COMP_MAX                                          (0x0000000000000001U)

/* TLB_DBG */

#define CSL_DRU_MMU_TLB_DBG_TLB_MASK                                           (0x0000000000030000U)
#define CSL_DRU_MMU_TLB_DBG_TLB_SHIFT                                          (0x0000000000000010U)
#define CSL_DRU_MMU_TLB_DBG_TLB_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_TLB_DBG_RSVD1_MASK                                         (0x000000000000C000U)
#define CSL_DRU_MMU_TLB_DBG_RSVD1_SHIFT                                        (0x000000000000000EU)
#define CSL_DRU_MMU_TLB_DBG_RSVD1_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TLB_DBG_WAY_MASK                                           (0x0000000000003000U)
#define CSL_DRU_MMU_TLB_DBG_WAY_SHIFT                                          (0x000000000000000CU)
#define CSL_DRU_MMU_TLB_DBG_WAY_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_TLB_DBG_RSVD2_MASK                                         (0x0000000000000F80U)
#define CSL_DRU_MMU_TLB_DBG_RSVD2_SHIFT                                        (0x0000000000000007U)
#define CSL_DRU_MMU_TLB_DBG_RSVD2_MAX                                          (0x000000000000001FU)

#define CSL_DRU_MMU_TLB_DBG_INDEX_MASK                                         (0x000000000000007FU)
#define CSL_DRU_MMU_TLB_DBG_INDEX_SHIFT                                        (0x0000000000000000U)
#define CSL_DRU_MMU_TLB_DBG_INDEX_MAX                                          (0x000000000000007FU)

/* TLB_DBG_DATA0 */

#define CSL_DRU_MMU_TLB_DBG_DATA0_DS_SIZE_MASK                                 (0x7800000000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_DS_SIZE_SHIFT                                (0x000000000000003BU)
#define CSL_DRU_MMU_TLB_DBG_DATA0_DS_SIZE_MAX                                  (0x000000000000000FU)

#define CSL_DRU_MMU_TLB_DBG_DATA0_DS_TYPE_MASK                                 (0x0400000000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_DS_TYPE_SHIFT                                (0x000000000000003AU)
#define CSL_DRU_MMU_TLB_DBG_DATA0_DS_TYPE_MAX                                  (0x0000000000000001U)

#define CSL_DRU_MMU_TLB_DBG_DATA0_IADDR_MASK                                   (0x03FFFFFFF0000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_IADDR_SHIFT                                  (0x000000000000001CU)
#define CSL_DRU_MMU_TLB_DBG_DATA0_IADDR_MAX                                    (0x000000003FFFFFFFU)

#define CSL_DRU_MMU_TLB_DBG_DATA0_VMID_MASK                                    (0x000000000FF00000U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_VMID_SHIFT                                   (0x0000000000000014U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_VMID_MAX                                     (0x00000000000000FFU)

#define CSL_DRU_MMU_TLB_DBG_DATA0_RSVD0_MASK                                   (0x00000000000FF000U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_RSVD0_SHIFT                                  (0x000000000000000CU)
#define CSL_DRU_MMU_TLB_DBG_DATA0_RSVD0_MAX                                    (0x00000000000000FFU)

#define CSL_DRU_MMU_TLB_DBG_DATA0_ASID_MASK                                    (0x0000000000000FF0U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_ASID_SHIFT                                   (0x0000000000000004U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_ASID_MAX                                     (0x00000000000000FFU)

#define CSL_DRU_MMU_TLB_DBG_DATA0_GBL_MASK                                     (0x0000000000000008U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_GBL_SHIFT                                    (0x0000000000000003U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_GBL_MAX                                      (0x0000000000000001U)

#define CSL_DRU_MMU_TLB_DBG_DATA0_ROOT_MASK                                    (0x0000000000000004U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_ROOT_SHIFT                                   (0x0000000000000002U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_ROOT_MAX                                     (0x0000000000000001U)

#define CSL_DRU_MMU_TLB_DBG_DATA0_SEC_MASK                                     (0x0000000000000002U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_SEC_SHIFT                                    (0x0000000000000001U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_SEC_MAX                                      (0x0000000000000001U)

#define CSL_DRU_MMU_TLB_DBG_DATA0_VALID_MASK                                   (0x0000000000000001U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_VALID_SHIFT                                  (0x0000000000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA0_VALID_MAX                                    (0x0000000000000001U)

/* TLB_DBG_DATA1 */

#define CSL_DRU_MMU_TLB_DBG_DATA1_SHARE_MASK                                   (0x0000C00000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_SHARE_SHIFT                                  (0x000000000000002EU)
#define CSL_DRU_MMU_TLB_DBG_DATA1_SHARE_MAX                                    (0x0000000000000003U)

#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_LVL_MASK                                  (0x0000300000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_LVL_SHIFT                                 (0x000000000000002CU)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_LVL_MAX                                   (0x0000000000000003U)

#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_MEM_TYPE_MASK                             (0x00000F0000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_MEM_TYPE_SHIFT                            (0x0000000000000028U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_MEM_TYPE_MAX                              (0x000000000000000FU)

#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_PERM_MASK                                 (0x000000F000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_PERM_SHIFT                                (0x0000000000000024U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S2_PERM_MAX                                  (0x000000000000000FU)

#define CSL_DRU_MMU_TLB_DBG_DATA1_S1_MEM_INDEX_MASK                            (0x0000000E00000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S1_MEM_INDEX_SHIFT                           (0x0000000000000021U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S1_MEM_INDEX_MAX                             (0x0000000000000007U)

#define CSL_DRU_MMU_TLB_DBG_DATA1_S1_PERM_MASK                                 (0x00000001F0000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S1_PERM_SHIFT                                (0x000000000000001CU)
#define CSL_DRU_MMU_TLB_DBG_DATA1_S1_PERM_MAX                                  (0x000000000000001FU)

#define CSL_DRU_MMU_TLB_DBG_DATA1_OADDR_MASK                                   (0x000000000FFFFFFFU)
#define CSL_DRU_MMU_TLB_DBG_DATA1_OADDR_SHIFT                                  (0x0000000000000000U)
#define CSL_DRU_MMU_TLB_DBG_DATA1_OADDR_MAX                                    (0x000000000FFFFFFFU)

/* SCR */

#define CSL_DRU_MMU_SCR_RSVD0_MASK                                             (0x7FFFFFFFFFFFFF00U)
#define CSL_DRU_MMU_SCR_RSVD0_SHIFT                                            (0x0000000000000008U)
#define CSL_DRU_MMU_SCR_RSVD0_MAX                                              (0x007FFFFFFFFFFFFFU)

#define CSL_DRU_MMU_SCR_INSTC_MASK                                             (0x0000000000000080U)
#define CSL_DRU_MMU_SCR_INSTC_SHIFT                                            (0x0000000000000007U)
#define CSL_DRU_MMU_SCR_INSTC_MAX                                              (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_DATAC_MASK                                             (0x0000000000000040U)
#define CSL_DRU_MMU_SCR_DATAC_SHIFT                                            (0x0000000000000006U)
#define CSL_DRU_MMU_SCR_DATAC_MAX                                              (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_FAULT_MASK                                             (0x0000000000000020U)
#define CSL_DRU_MMU_SCR_FAULT_SHIFT                                            (0x0000000000000005U)
#define CSL_DRU_MMU_SCR_FAULT_MAX                                              (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_ASEL_MASK                                              (0x0000000000000010U)
#define CSL_DRU_MMU_SCR_ASEL_SHIFT                                             (0x0000000000000004U)
#define CSL_DRU_MMU_SCR_ASEL_MAX                                               (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_WXN_MASK                                               (0x0000000000000008U)
#define CSL_DRU_MMU_SCR_WXN_SHIFT                                              (0x0000000000000003U)
#define CSL_DRU_MMU_SCR_WXN_MAX                                                (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_ENDIAN1_MASK                                           (0x0000000000000004U)
#define CSL_DRU_MMU_SCR_ENDIAN1_SHIFT                                          (0x0000000000000002U)
#define CSL_DRU_MMU_SCR_ENDIAN1_MAX                                            (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_ENDIAN0_MASK                                           (0x0000000000000002U)
#define CSL_DRU_MMU_SCR_ENDIAN0_SHIFT                                          (0x0000000000000001U)
#define CSL_DRU_MMU_SCR_ENDIAN0_MAX                                            (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S1_EN_MASK                                             (0x0000000000000001U)
#define CSL_DRU_MMU_SCR_S1_EN_SHIFT                                            (0x0000000000000000U)
#define CSL_DRU_MMU_SCR_S1_EN_MAX                                              (0x0000000000000001U)

/* TCR0 */

#define CSL_DRU_MMU_TCR0_MEMTYPE_MASK                                          (0x0000000000018000U)
#define CSL_DRU_MMU_TCR0_MEMTYPE_SHIFT                                         (0x000000000000000FU)
#define CSL_DRU_MMU_TCR0_MEMTYPE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_COUTER_MASK                                           (0x0000000000006000U)
#define CSL_DRU_MMU_TCR0_COUTER_SHIFT                                          (0x000000000000000DU)
#define CSL_DRU_MMU_TCR0_COUTER_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_CINNER_MASK                                           (0x0000000000001800U)
#define CSL_DRU_MMU_TCR0_CINNER_SHIFT                                          (0x000000000000000BU)
#define CSL_DRU_MMU_TCR0_CINNER_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_SHARE_MASK                                            (0x0000000000000600U)
#define CSL_DRU_MMU_TCR0_SHARE_SHIFT                                           (0x0000000000000009U)
#define CSL_DRU_MMU_TCR0_SHARE_MAX                                             (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_GRANULE_MASK                                          (0x0000000000000180U)
#define CSL_DRU_MMU_TCR0_GRANULE_SHIFT                                         (0x0000000000000007U)
#define CSL_DRU_MMU_TCR0_GRANULE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_TBL_SZ_MASK                                           (0x000000000000007EU)
#define CSL_DRU_MMU_TCR0_TBL_SZ_SHIFT                                          (0x0000000000000001U)
#define CSL_DRU_MMU_TCR0_TBL_SZ_MAX                                            (0x000000000000003FU)

#define CSL_DRU_MMU_TCR0_WALK_EN_MASK                                          (0x0000000000000001U)
#define CSL_DRU_MMU_TCR0_WALK_EN_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_TCR0_WALK_EN_MAX                                           (0x0000000000000001U)

/* TCR1 */

#define CSL_DRU_MMU_TCR1_MEMTYPE_MASK                                          (0x0000000000018000U)
#define CSL_DRU_MMU_TCR1_MEMTYPE_SHIFT                                         (0x000000000000000FU)
#define CSL_DRU_MMU_TCR1_MEMTYPE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_COUTER_MASK                                           (0x0000000000006000U)
#define CSL_DRU_MMU_TCR1_COUTER_SHIFT                                          (0x000000000000000DU)
#define CSL_DRU_MMU_TCR1_COUTER_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_CINNER_MASK                                           (0x0000000000001800U)
#define CSL_DRU_MMU_TCR1_CINNER_SHIFT                                          (0x000000000000000BU)
#define CSL_DRU_MMU_TCR1_CINNER_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_SHARE_MASK                                            (0x0000000000000600U)
#define CSL_DRU_MMU_TCR1_SHARE_SHIFT                                           (0x0000000000000009U)
#define CSL_DRU_MMU_TCR1_SHARE_MAX                                             (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_GRANULE_MASK                                          (0x0000000000000180U)
#define CSL_DRU_MMU_TCR1_GRANULE_SHIFT                                         (0x0000000000000007U)
#define CSL_DRU_MMU_TCR1_GRANULE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_TBL_SZ_MASK                                           (0x000000000000007EU)
#define CSL_DRU_MMU_TCR1_TBL_SZ_SHIFT                                          (0x0000000000000001U)
#define CSL_DRU_MMU_TCR1_TBL_SZ_MAX                                            (0x000000000000003FU)

#define CSL_DRU_MMU_TCR1_WALK_EN_MASK                                          (0x0000000000000001U)
#define CSL_DRU_MMU_TCR1_WALK_EN_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_TCR1_WALK_EN_MAX                                           (0x0000000000000001U)

/* TBR0 */

#define CSL_DRU_MMU_TBR0_ASID_MASK                                             (0x00FF000000000000U)
#define CSL_DRU_MMU_TBR0_ASID_SHIFT                                            (0x0000000000000030U)
#define CSL_DRU_MMU_TBR0_ASID_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR0_RSVD1_MASK                                            (0x0000FF0000000000U)
#define CSL_DRU_MMU_TBR0_RSVD1_SHIFT                                           (0x0000000000000028U)
#define CSL_DRU_MMU_TBR0_RSVD1_MAX                                             (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR0_BADDR_MASK                                            (0x000000FFFFFFFFFFU)
#define CSL_DRU_MMU_TBR0_BADDR_SHIFT                                           (0x0000000000000000U)
#define CSL_DRU_MMU_TBR0_BADDR_MAX                                             (0x000000FFFFFFFFFFU)

/* TBR1 */

#define CSL_DRU_MMU_TBR1_ASID_MASK                                             (0x00FF000000000000U)
#define CSL_DRU_MMU_TBR1_ASID_SHIFT                                            (0x0000000000000030U)
#define CSL_DRU_MMU_TBR1_ASID_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR1_RSVD1_MASK                                            (0x0000FF0000000000U)
#define CSL_DRU_MMU_TBR1_RSVD1_SHIFT                                           (0x0000000000000028U)
#define CSL_DRU_MMU_TBR1_RSVD1_MAX                                             (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR1_BADDR_MASK                                            (0x000000FFFFFFFFFFU)
#define CSL_DRU_MMU_TBR1_BADDR_SHIFT                                           (0x0000000000000000U)
#define CSL_DRU_MMU_TBR1_BADDR_MAX                                             (0x000000FFFFFFFFFFU)

/* MAR */

#define CSL_DRU_MMU_MAR_ATTR6_MASK                                             (0x00FF000000000000U)
#define CSL_DRU_MMU_MAR_ATTR6_SHIFT                                            (0x0000000000000030U)
#define CSL_DRU_MMU_MAR_ATTR6_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_ATTR5_MASK                                             (0x0000FF0000000000U)
#define CSL_DRU_MMU_MAR_ATTR5_SHIFT                                            (0x0000000000000028U)
#define CSL_DRU_MMU_MAR_ATTR5_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_ATTR4_MASK                                             (0x000000FF00000000U)
#define CSL_DRU_MMU_MAR_ATTR4_SHIFT                                            (0x0000000000000020U)
#define CSL_DRU_MMU_MAR_ATTR4_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_ATTR3_MASK                                             (0x00000000FF000000U)
#define CSL_DRU_MMU_MAR_ATTR3_SHIFT                                            (0x0000000000000018U)
#define CSL_DRU_MMU_MAR_ATTR3_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_ATTR2_MASK                                             (0x0000000000FF0000U)
#define CSL_DRU_MMU_MAR_ATTR2_SHIFT                                            (0x0000000000000010U)
#define CSL_DRU_MMU_MAR_ATTR2_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_ATTR1_MASK                                             (0x000000000000FF00U)
#define CSL_DRU_MMU_MAR_ATTR1_SHIFT                                            (0x0000000000000008U)
#define CSL_DRU_MMU_MAR_ATTR1_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_ATTR0_MASK                                             (0x00000000000000FFU)
#define CSL_DRU_MMU_MAR_ATTR0_SHIFT                                            (0x0000000000000000U)
#define CSL_DRU_MMU_MAR_ATTR0_MAX                                              (0x00000000000000FFU)

/* TDAR */

#define CSL_DRU_MMU_TDAR_INTEREST_MASK                                         (0x0000000000000008U)
#define CSL_DRU_MMU_TDAR_INTEREST_SHIFT                                        (0x0000000000000003U)
#define CSL_DRU_MMU_TDAR_INTEREST_MAX                                          (0x0000000000000001U)

#define CSL_DRU_MMU_TDAR_ACC_TYPE_MASK                                         (0x0000000000000006U)
#define CSL_DRU_MMU_TDAR_ACC_TYPE_SHIFT                                        (0x0000000000000001U)
#define CSL_DRU_MMU_TDAR_ACC_TYPE_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TDAR_PRIV_MASK                                             (0x0000000000000001U)
#define CSL_DRU_MMU_TDAR_PRIV_SHIFT                                            (0x0000000000000000U)
#define CSL_DRU_MMU_TDAR_PRIV_MAX                                              (0x0000000000000001U)

/* SCR_GS */

#define CSL_DRU_MMU_SCR_GS_RSVD0_MASK                                          (0x7FFFFFFFFFFFFF00U)
#define CSL_DRU_MMU_SCR_GS_RSVD0_SHIFT                                         (0x0000000000000008U)
#define CSL_DRU_MMU_SCR_GS_RSVD0_MAX                                           (0x007FFFFFFFFFFFFFU)

#define CSL_DRU_MMU_SCR_GS_INSTC_MASK                                          (0x0000000000000080U)
#define CSL_DRU_MMU_SCR_GS_INSTC_SHIFT                                         (0x0000000000000007U)
#define CSL_DRU_MMU_SCR_GS_INSTC_MAX                                           (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_GS_DATAC_MASK                                          (0x0000000000000040U)
#define CSL_DRU_MMU_SCR_GS_DATAC_SHIFT                                         (0x0000000000000006U)
#define CSL_DRU_MMU_SCR_GS_DATAC_MAX                                           (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_GS_FAULT_MASK                                          (0x0000000000000020U)
#define CSL_DRU_MMU_SCR_GS_FAULT_SHIFT                                         (0x0000000000000005U)
#define CSL_DRU_MMU_SCR_GS_FAULT_MAX                                           (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_GS_ASEL_MASK                                           (0x0000000000000010U)
#define CSL_DRU_MMU_SCR_GS_ASEL_SHIFT                                          (0x0000000000000004U)
#define CSL_DRU_MMU_SCR_GS_ASEL_MAX                                            (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_GS_WXN_MASK                                            (0x0000000000000008U)
#define CSL_DRU_MMU_SCR_GS_WXN_SHIFT                                           (0x0000000000000003U)
#define CSL_DRU_MMU_SCR_GS_WXN_MAX                                             (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_GS_ENDIAN1_MASK                                        (0x0000000000000004U)
#define CSL_DRU_MMU_SCR_GS_ENDIAN1_SHIFT                                       (0x0000000000000002U)
#define CSL_DRU_MMU_SCR_GS_ENDIAN1_MAX                                         (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_GS_ENDIAN0_MASK                                        (0x0000000000000002U)
#define CSL_DRU_MMU_SCR_GS_ENDIAN0_SHIFT                                       (0x0000000000000001U)
#define CSL_DRU_MMU_SCR_GS_ENDIAN0_MAX                                         (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_GS_S1_EN_MASK                                          (0x0000000000000001U)
#define CSL_DRU_MMU_SCR_GS_S1_EN_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_SCR_GS_S1_EN_MAX                                           (0x0000000000000001U)

/* TCR0_GS */

#define CSL_DRU_MMU_TCR0_GS_MEMTYPE_MASK                                       (0x0000000000018000U)
#define CSL_DRU_MMU_TCR0_GS_MEMTYPE_SHIFT                                      (0x000000000000000FU)
#define CSL_DRU_MMU_TCR0_GS_MEMTYPE_MAX                                        (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_GS_COUTER_MASK                                        (0x0000000000006000U)
#define CSL_DRU_MMU_TCR0_GS_COUTER_SHIFT                                       (0x000000000000000DU)
#define CSL_DRU_MMU_TCR0_GS_COUTER_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_GS_CINNER_MASK                                        (0x0000000000001800U)
#define CSL_DRU_MMU_TCR0_GS_CINNER_SHIFT                                       (0x000000000000000BU)
#define CSL_DRU_MMU_TCR0_GS_CINNER_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_GS_SHARE_MASK                                         (0x0000000000000600U)
#define CSL_DRU_MMU_TCR0_GS_SHARE_SHIFT                                        (0x0000000000000009U)
#define CSL_DRU_MMU_TCR0_GS_SHARE_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_GS_GRANULE_MASK                                       (0x0000000000000180U)
#define CSL_DRU_MMU_TCR0_GS_GRANULE_SHIFT                                      (0x0000000000000007U)
#define CSL_DRU_MMU_TCR0_GS_GRANULE_MAX                                        (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_GS_TBL_SZ_MASK                                        (0x000000000000007EU)
#define CSL_DRU_MMU_TCR0_GS_TBL_SZ_SHIFT                                       (0x0000000000000001U)
#define CSL_DRU_MMU_TCR0_GS_TBL_SZ_MAX                                         (0x000000000000003FU)

#define CSL_DRU_MMU_TCR0_GS_WALK_EN_MASK                                       (0x0000000000000001U)
#define CSL_DRU_MMU_TCR0_GS_WALK_EN_SHIFT                                      (0x0000000000000000U)
#define CSL_DRU_MMU_TCR0_GS_WALK_EN_MAX                                        (0x0000000000000001U)

/* TCR1_GS */

#define CSL_DRU_MMU_TCR1_GS_MEMTYPE_MASK                                       (0x0000000000018000U)
#define CSL_DRU_MMU_TCR1_GS_MEMTYPE_SHIFT                                      (0x000000000000000FU)
#define CSL_DRU_MMU_TCR1_GS_MEMTYPE_MAX                                        (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_GS_COUTER_MASK                                        (0x0000000000006000U)
#define CSL_DRU_MMU_TCR1_GS_COUTER_SHIFT                                       (0x000000000000000DU)
#define CSL_DRU_MMU_TCR1_GS_COUTER_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_GS_CINNER_MASK                                        (0x0000000000001800U)
#define CSL_DRU_MMU_TCR1_GS_CINNER_SHIFT                                       (0x000000000000000BU)
#define CSL_DRU_MMU_TCR1_GS_CINNER_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_GS_SHARE_MASK                                         (0x0000000000000600U)
#define CSL_DRU_MMU_TCR1_GS_SHARE_SHIFT                                        (0x0000000000000009U)
#define CSL_DRU_MMU_TCR1_GS_SHARE_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_GS_GRANULE_MASK                                       (0x0000000000000180U)
#define CSL_DRU_MMU_TCR1_GS_GRANULE_SHIFT                                      (0x0000000000000007U)
#define CSL_DRU_MMU_TCR1_GS_GRANULE_MAX                                        (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_GS_TBL_SZ_MASK                                        (0x000000000000007EU)
#define CSL_DRU_MMU_TCR1_GS_TBL_SZ_SHIFT                                       (0x0000000000000001U)
#define CSL_DRU_MMU_TCR1_GS_TBL_SZ_MAX                                         (0x000000000000003FU)

#define CSL_DRU_MMU_TCR1_GS_WALK_EN_MASK                                       (0x0000000000000001U)
#define CSL_DRU_MMU_TCR1_GS_WALK_EN_SHIFT                                      (0x0000000000000000U)
#define CSL_DRU_MMU_TCR1_GS_WALK_EN_MAX                                        (0x0000000000000001U)

/* TBR0_GS */

#define CSL_DRU_MMU_TBR0_GS_ASID_MASK                                          (0x00FF000000000000U)
#define CSL_DRU_MMU_TBR0_GS_ASID_SHIFT                                         (0x0000000000000030U)
#define CSL_DRU_MMU_TBR0_GS_ASID_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR0_GS_RSVD1_MASK                                         (0x0000FF0000000000U)
#define CSL_DRU_MMU_TBR0_GS_RSVD1_SHIFT                                        (0x0000000000000028U)
#define CSL_DRU_MMU_TBR0_GS_RSVD1_MAX                                          (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR0_GS_BADDR_MASK                                         (0x000000FFFFFFFFFFU)
#define CSL_DRU_MMU_TBR0_GS_BADDR_SHIFT                                        (0x0000000000000000U)
#define CSL_DRU_MMU_TBR0_GS_BADDR_MAX                                          (0x000000FFFFFFFFFFU)

/* TBR1_GS */

#define CSL_DRU_MMU_TBR1_GS_ASID_MASK                                          (0x00FF000000000000U)
#define CSL_DRU_MMU_TBR1_GS_ASID_SHIFT                                         (0x0000000000000030U)
#define CSL_DRU_MMU_TBR1_GS_ASID_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR1_GS_RSVD1_MASK                                         (0x0000FF0000000000U)
#define CSL_DRU_MMU_TBR1_GS_RSVD1_SHIFT                                        (0x0000000000000028U)
#define CSL_DRU_MMU_TBR1_GS_RSVD1_MAX                                          (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR1_GS_BADDR_MASK                                         (0x000000FFFFFFFFFFU)
#define CSL_DRU_MMU_TBR1_GS_BADDR_SHIFT                                        (0x0000000000000000U)
#define CSL_DRU_MMU_TBR1_GS_BADDR_MAX                                          (0x000000FFFFFFFFFFU)

/* MAR_GS */

#define CSL_DRU_MMU_MAR_GS_ATTR6_MASK                                          (0x00FF000000000000U)
#define CSL_DRU_MMU_MAR_GS_ATTR6_SHIFT                                         (0x0000000000000030U)
#define CSL_DRU_MMU_MAR_GS_ATTR6_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_GS_ATTR5_MASK                                          (0x0000FF0000000000U)
#define CSL_DRU_MMU_MAR_GS_ATTR5_SHIFT                                         (0x0000000000000028U)
#define CSL_DRU_MMU_MAR_GS_ATTR5_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_GS_ATTR4_MASK                                          (0x000000FF00000000U)
#define CSL_DRU_MMU_MAR_GS_ATTR4_SHIFT                                         (0x0000000000000020U)
#define CSL_DRU_MMU_MAR_GS_ATTR4_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_GS_ATTR3_MASK                                          (0x00000000FF000000U)
#define CSL_DRU_MMU_MAR_GS_ATTR3_SHIFT                                         (0x0000000000000018U)
#define CSL_DRU_MMU_MAR_GS_ATTR3_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_GS_ATTR2_MASK                                          (0x0000000000FF0000U)
#define CSL_DRU_MMU_MAR_GS_ATTR2_SHIFT                                         (0x0000000000000010U)
#define CSL_DRU_MMU_MAR_GS_ATTR2_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_GS_ATTR1_MASK                                          (0x000000000000FF00U)
#define CSL_DRU_MMU_MAR_GS_ATTR1_SHIFT                                         (0x0000000000000008U)
#define CSL_DRU_MMU_MAR_GS_ATTR1_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_GS_ATTR0_MASK                                          (0x00000000000000FFU)
#define CSL_DRU_MMU_MAR_GS_ATTR0_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_MAR_GS_ATTR0_MAX                                           (0x00000000000000FFU)

/* TDAR_GS */

#define CSL_DRU_MMU_TDAR_GS_INTEREST_MASK                                      (0x0000000000000008U)
#define CSL_DRU_MMU_TDAR_GS_INTEREST_SHIFT                                     (0x0000000000000003U)
#define CSL_DRU_MMU_TDAR_GS_INTEREST_MAX                                       (0x0000000000000001U)

#define CSL_DRU_MMU_TDAR_GS_ACC_TYPE_MASK                                      (0x0000000000000006U)
#define CSL_DRU_MMU_TDAR_GS_ACC_TYPE_SHIFT                                     (0x0000000000000001U)
#define CSL_DRU_MMU_TDAR_GS_ACC_TYPE_MAX                                       (0x0000000000000003U)

#define CSL_DRU_MMU_TDAR_GS_PRIV_MASK                                          (0x0000000000000001U)
#define CSL_DRU_MMU_TDAR_GS_PRIV_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_TDAR_GS_PRIV_MAX                                           (0x0000000000000001U)

/* SCR_S */

#define CSL_DRU_MMU_SCR_S_RSVD0_MASK                                           (0x7FFFFFFFFFFFFF00U)
#define CSL_DRU_MMU_SCR_S_RSVD0_SHIFT                                          (0x0000000000000008U)
#define CSL_DRU_MMU_SCR_S_RSVD0_MAX                                            (0x007FFFFFFFFFFFFFU)

#define CSL_DRU_MMU_SCR_S_INSTC_MASK                                           (0x0000000000000080U)
#define CSL_DRU_MMU_SCR_S_INSTC_SHIFT                                          (0x0000000000000007U)
#define CSL_DRU_MMU_SCR_S_INSTC_MAX                                            (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S_DATAC_MASK                                           (0x0000000000000040U)
#define CSL_DRU_MMU_SCR_S_DATAC_SHIFT                                          (0x0000000000000006U)
#define CSL_DRU_MMU_SCR_S_DATAC_MAX                                            (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S_FAULT_MASK                                           (0x0000000000000020U)
#define CSL_DRU_MMU_SCR_S_FAULT_SHIFT                                          (0x0000000000000005U)
#define CSL_DRU_MMU_SCR_S_FAULT_MAX                                            (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S_ASEL_MASK                                            (0x0000000000000010U)
#define CSL_DRU_MMU_SCR_S_ASEL_SHIFT                                           (0x0000000000000004U)
#define CSL_DRU_MMU_SCR_S_ASEL_MAX                                             (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S_WXN_MASK                                             (0x0000000000000008U)
#define CSL_DRU_MMU_SCR_S_WXN_SHIFT                                            (0x0000000000000003U)
#define CSL_DRU_MMU_SCR_S_WXN_MAX                                              (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S_ENDIAN1_MASK                                         (0x0000000000000004U)
#define CSL_DRU_MMU_SCR_S_ENDIAN1_SHIFT                                        (0x0000000000000002U)
#define CSL_DRU_MMU_SCR_S_ENDIAN1_MAX                                          (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S_ENDIAN0_MASK                                         (0x0000000000000002U)
#define CSL_DRU_MMU_SCR_S_ENDIAN0_SHIFT                                        (0x0000000000000001U)
#define CSL_DRU_MMU_SCR_S_ENDIAN0_MAX                                          (0x0000000000000001U)

#define CSL_DRU_MMU_SCR_S_S1_EN_MASK                                           (0x0000000000000001U)
#define CSL_DRU_MMU_SCR_S_S1_EN_SHIFT                                          (0x0000000000000000U)
#define CSL_DRU_MMU_SCR_S_S1_EN_MAX                                            (0x0000000000000001U)

/* TCR0_S */

#define CSL_DRU_MMU_TCR0_S_MEMTYPE_MASK                                        (0x0000000000018000U)
#define CSL_DRU_MMU_TCR0_S_MEMTYPE_SHIFT                                       (0x000000000000000FU)
#define CSL_DRU_MMU_TCR0_S_MEMTYPE_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_S_COUTER_MASK                                         (0x0000000000006000U)
#define CSL_DRU_MMU_TCR0_S_COUTER_SHIFT                                        (0x000000000000000DU)
#define CSL_DRU_MMU_TCR0_S_COUTER_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_S_CINNER_MASK                                         (0x0000000000001800U)
#define CSL_DRU_MMU_TCR0_S_CINNER_SHIFT                                        (0x000000000000000BU)
#define CSL_DRU_MMU_TCR0_S_CINNER_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_S_SHARE_MASK                                          (0x0000000000000600U)
#define CSL_DRU_MMU_TCR0_S_SHARE_SHIFT                                         (0x0000000000000009U)
#define CSL_DRU_MMU_TCR0_S_SHARE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_S_GRANULE_MASK                                        (0x0000000000000180U)
#define CSL_DRU_MMU_TCR0_S_GRANULE_SHIFT                                       (0x0000000000000007U)
#define CSL_DRU_MMU_TCR0_S_GRANULE_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR0_S_TBL_SZ_MASK                                         (0x000000000000007EU)
#define CSL_DRU_MMU_TCR0_S_TBL_SZ_SHIFT                                        (0x0000000000000001U)
#define CSL_DRU_MMU_TCR0_S_TBL_SZ_MAX                                          (0x000000000000003FU)

#define CSL_DRU_MMU_TCR0_S_WALK_EN_MASK                                        (0x0000000000000001U)
#define CSL_DRU_MMU_TCR0_S_WALK_EN_SHIFT                                       (0x0000000000000000U)
#define CSL_DRU_MMU_TCR0_S_WALK_EN_MAX                                         (0x0000000000000001U)

/* TCR1_S */

#define CSL_DRU_MMU_TCR1_S_MEMTYPE_MASK                                        (0x0000000000018000U)
#define CSL_DRU_MMU_TCR1_S_MEMTYPE_SHIFT                                       (0x000000000000000FU)
#define CSL_DRU_MMU_TCR1_S_MEMTYPE_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_S_COUTER_MASK                                         (0x0000000000006000U)
#define CSL_DRU_MMU_TCR1_S_COUTER_SHIFT                                        (0x000000000000000DU)
#define CSL_DRU_MMU_TCR1_S_COUTER_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_S_CINNER_MASK                                         (0x0000000000001800U)
#define CSL_DRU_MMU_TCR1_S_CINNER_SHIFT                                        (0x000000000000000BU)
#define CSL_DRU_MMU_TCR1_S_CINNER_MAX                                          (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_S_SHARE_MASK                                          (0x0000000000000600U)
#define CSL_DRU_MMU_TCR1_S_SHARE_SHIFT                                         (0x0000000000000009U)
#define CSL_DRU_MMU_TCR1_S_SHARE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_S_GRANULE_MASK                                        (0x0000000000000180U)
#define CSL_DRU_MMU_TCR1_S_GRANULE_SHIFT                                       (0x0000000000000007U)
#define CSL_DRU_MMU_TCR1_S_GRANULE_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_TCR1_S_TBL_SZ_MASK                                         (0x000000000000007EU)
#define CSL_DRU_MMU_TCR1_S_TBL_SZ_SHIFT                                        (0x0000000000000001U)
#define CSL_DRU_MMU_TCR1_S_TBL_SZ_MAX                                          (0x000000000000003FU)

#define CSL_DRU_MMU_TCR1_S_WALK_EN_MASK                                        (0x0000000000000001U)
#define CSL_DRU_MMU_TCR1_S_WALK_EN_SHIFT                                       (0x0000000000000000U)
#define CSL_DRU_MMU_TCR1_S_WALK_EN_MAX                                         (0x0000000000000001U)

/* TBR0_S */

#define CSL_DRU_MMU_TBR0_S_ASID_MASK                                           (0x00FF000000000000U)
#define CSL_DRU_MMU_TBR0_S_ASID_SHIFT                                          (0x0000000000000030U)
#define CSL_DRU_MMU_TBR0_S_ASID_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR0_S_RSVD1_MASK                                          (0x0000FF0000000000U)
#define CSL_DRU_MMU_TBR0_S_RSVD1_SHIFT                                         (0x0000000000000028U)
#define CSL_DRU_MMU_TBR0_S_RSVD1_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR0_S_BADDR_MASK                                          (0x000000FFFFFFFFFFU)
#define CSL_DRU_MMU_TBR0_S_BADDR_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_TBR0_S_BADDR_MAX                                           (0x000000FFFFFFFFFFU)

/* TBR1_S */

#define CSL_DRU_MMU_TBR1_S_ASID_MASK                                           (0x00FF000000000000U)
#define CSL_DRU_MMU_TBR1_S_ASID_SHIFT                                          (0x0000000000000030U)
#define CSL_DRU_MMU_TBR1_S_ASID_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR1_S_RSVD1_MASK                                          (0x0000FF0000000000U)
#define CSL_DRU_MMU_TBR1_S_RSVD1_SHIFT                                         (0x0000000000000028U)
#define CSL_DRU_MMU_TBR1_S_RSVD1_MAX                                           (0x00000000000000FFU)

#define CSL_DRU_MMU_TBR1_S_BADDR_MASK                                          (0x000000FFFFFFFFFFU)
#define CSL_DRU_MMU_TBR1_S_BADDR_SHIFT                                         (0x0000000000000000U)
#define CSL_DRU_MMU_TBR1_S_BADDR_MAX                                           (0x000000FFFFFFFFFFU)

/* MAR_S */

#define CSL_DRU_MMU_MAR_S_ATTR6_MASK                                           (0x00FF000000000000U)
#define CSL_DRU_MMU_MAR_S_ATTR6_SHIFT                                          (0x0000000000000030U)
#define CSL_DRU_MMU_MAR_S_ATTR6_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_S_ATTR5_MASK                                           (0x0000FF0000000000U)
#define CSL_DRU_MMU_MAR_S_ATTR5_SHIFT                                          (0x0000000000000028U)
#define CSL_DRU_MMU_MAR_S_ATTR5_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_S_ATTR4_MASK                                           (0x000000FF00000000U)
#define CSL_DRU_MMU_MAR_S_ATTR4_SHIFT                                          (0x0000000000000020U)
#define CSL_DRU_MMU_MAR_S_ATTR4_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_S_ATTR3_MASK                                           (0x00000000FF000000U)
#define CSL_DRU_MMU_MAR_S_ATTR3_SHIFT                                          (0x0000000000000018U)
#define CSL_DRU_MMU_MAR_S_ATTR3_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_S_ATTR2_MASK                                           (0x0000000000FF0000U)
#define CSL_DRU_MMU_MAR_S_ATTR2_SHIFT                                          (0x0000000000000010U)
#define CSL_DRU_MMU_MAR_S_ATTR2_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_S_ATTR1_MASK                                           (0x000000000000FF00U)
#define CSL_DRU_MMU_MAR_S_ATTR1_SHIFT                                          (0x0000000000000008U)
#define CSL_DRU_MMU_MAR_S_ATTR1_MAX                                            (0x00000000000000FFU)

#define CSL_DRU_MMU_MAR_S_ATTR0_MASK                                           (0x00000000000000FFU)
#define CSL_DRU_MMU_MAR_S_ATTR0_SHIFT                                          (0x0000000000000000U)
#define CSL_DRU_MMU_MAR_S_ATTR0_MAX                                            (0x00000000000000FFU)

/* TDAR_S */

#define CSL_DRU_MMU_TDAR_S_INTEREST_MASK                                       (0x0000000000000008U)
#define CSL_DRU_MMU_TDAR_S_INTEREST_SHIFT                                      (0x0000000000000003U)
#define CSL_DRU_MMU_TDAR_S_INTEREST_MAX                                        (0x0000000000000001U)

#define CSL_DRU_MMU_TDAR_S_ACC_TYPE_MASK                                       (0x0000000000000006U)
#define CSL_DRU_MMU_TDAR_S_ACC_TYPE_SHIFT                                      (0x0000000000000001U)
#define CSL_DRU_MMU_TDAR_S_ACC_TYPE_MAX                                        (0x0000000000000003U)

#define CSL_DRU_MMU_TDAR_S_PRIV_MASK                                           (0x0000000000000001U)
#define CSL_DRU_MMU_TDAR_S_PRIV_SHIFT                                          (0x0000000000000000U)
#define CSL_DRU_MMU_TDAR_S_PRIV_MAX                                            (0x0000000000000001U)

/* VCR */

#define CSL_DRU_MMU_VCR_ID_MASK                                                (0x0000000000000020U)
#define CSL_DRU_MMU_VCR_ID_SHIFT                                               (0x0000000000000005U)
#define CSL_DRU_MMU_VCR_ID_MAX                                                 (0x0000000000000001U)

#define CSL_DRU_MMU_VCR_CD_MASK                                                (0x0000000000000010U)
#define CSL_DRU_MMU_VCR_CD_SHIFT                                               (0x0000000000000004U)
#define CSL_DRU_MMU_VCR_CD_MAX                                                 (0x0000000000000001U)

#define CSL_DRU_MMU_VCR_DC_MASK                                                (0x0000000000000008U)
#define CSL_DRU_MMU_VCR_DC_SHIFT                                               (0x0000000000000003U)
#define CSL_DRU_MMU_VCR_DC_MAX                                                 (0x0000000000000001U)

#define CSL_DRU_MMU_VCR_PROT_MASK                                              (0x0000000000000004U)
#define CSL_DRU_MMU_VCR_PROT_SHIFT                                             (0x0000000000000002U)
#define CSL_DRU_MMU_VCR_PROT_MAX                                               (0x0000000000000001U)

#define CSL_DRU_MMU_VCR_ENDIAN_MASK                                            (0x0000000000000002U)
#define CSL_DRU_MMU_VCR_ENDIAN_SHIFT                                           (0x0000000000000001U)
#define CSL_DRU_MMU_VCR_ENDIAN_MAX                                             (0x0000000000000001U)

#define CSL_DRU_MMU_VCR_S2_EN_MASK                                             (0x0000000000000001U)
#define CSL_DRU_MMU_VCR_S2_EN_SHIFT                                            (0x0000000000000000U)
#define CSL_DRU_MMU_VCR_S2_EN_MAX                                              (0x0000000000000001U)

/* VTCR */

#define CSL_DRU_MMU_VTCR_SLEVEL_MASK                                           (0x0000000000060000U)
#define CSL_DRU_MMU_VTCR_SLEVEL_SHIFT                                          (0x0000000000000011U)
#define CSL_DRU_MMU_VTCR_SLEVEL_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_VTCR_MEMTYPE_MASK                                          (0x0000000000018000U)
#define CSL_DRU_MMU_VTCR_MEMTYPE_SHIFT                                         (0x000000000000000FU)
#define CSL_DRU_MMU_VTCR_MEMTYPE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_VTCR_COUTER_MASK                                           (0x0000000000006000U)
#define CSL_DRU_MMU_VTCR_COUTER_SHIFT                                          (0x000000000000000DU)
#define CSL_DRU_MMU_VTCR_COUTER_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_VTCR_CINNER_MASK                                           (0x0000000000001800U)
#define CSL_DRU_MMU_VTCR_CINNER_SHIFT                                          (0x000000000000000BU)
#define CSL_DRU_MMU_VTCR_CINNER_MAX                                            (0x0000000000000003U)

#define CSL_DRU_MMU_VTCR_SHARE_MASK                                            (0x0000000000000600U)
#define CSL_DRU_MMU_VTCR_SHARE_SHIFT                                           (0x0000000000000009U)
#define CSL_DRU_MMU_VTCR_SHARE_MAX                                             (0x0000000000000003U)

#define CSL_DRU_MMU_VTCR_GRANULE_MASK                                          (0x0000000000000180U)
#define CSL_DRU_MMU_VTCR_GRANULE_SHIFT                                         (0x0000000000000007U)
#define CSL_DRU_MMU_VTCR_GRANULE_MAX                                           (0x0000000000000003U)

#define CSL_DRU_MMU_VTCR_TBL_SZ_MASK                                           (0x000000000000007EU)
#define CSL_DRU_MMU_VTCR_TBL_SZ_SHIFT                                          (0x0000000000000001U)
#define CSL_DRU_MMU_VTCR_TBL_SZ_MAX                                            (0x000000000000003FU)

#define CSL_DRU_MMU_VTCR_RSVD1_MASK                                            (0x0000000000000001U)
#define CSL_DRU_MMU_VTCR_RSVD1_SHIFT                                           (0x0000000000000000U)
#define CSL_DRU_MMU_VTCR_RSVD1_MAX                                             (0x0000000000000001U)

/* VTBR */

#define CSL_DRU_MMU_VTBR_VMID_MASK                                             (0x00FF000000000000U)
#define CSL_DRU_MMU_VTBR_VMID_SHIFT                                            (0x0000000000000030U)
#define CSL_DRU_MMU_VTBR_VMID_MAX                                              (0x00000000000000FFU)

#define CSL_DRU_MMU_VTBR_RSVD1_MASK                                            (0x0000FF0000000000U)
#define CSL_DRU_MMU_VTBR_RSVD1_SHIFT                                           (0x0000000000000028U)
#define CSL_DRU_MMU_VTBR_RSVD1_MAX                                             (0x00000000000000FFU)

#define CSL_DRU_MMU_VTBR_BADDR_MASK                                            (0x000000FFFFFFFFFFU)
#define CSL_DRU_MMU_VTBR_BADDR_SHIFT                                           (0x0000000000000000U)
#define CSL_DRU_MMU_VTBR_BADDR_MAX                                             (0x000000FFFFFFFFFFU)

/* VTDAR */

#define CSL_DRU_MMU_VTDAR_INTEREST_MASK                                        (0x0000000000000008U)
#define CSL_DRU_MMU_VTDAR_INTEREST_SHIFT                                       (0x0000000000000003U)
#define CSL_DRU_MMU_VTDAR_INTEREST_MAX                                         (0x0000000000000001U)

#define CSL_DRU_MMU_VTDAR_ACC_TYPE_MASK                                        (0x0000000000000006U)
#define CSL_DRU_MMU_VTDAR_ACC_TYPE_SHIFT                                       (0x0000000000000001U)
#define CSL_DRU_MMU_VTDAR_ACC_TYPE_MAX                                         (0x0000000000000003U)

#define CSL_DRU_MMU_VTDAR_PRIV_MASK                                            (0x0000000000000001U)
#define CSL_DRU_MMU_VTDAR_PRIV_SHIFT                                           (0x0000000000000000U)
#define CSL_DRU_MMU_VTDAR_PRIV_MAX                                             (0x0000000000000001U)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t MATCH;
    volatile uint8_t  Resv_64[56];
} CSL_DRU_UTLBRegs_RD_UTLB_MATCH;


typedef struct {
    volatile uint64_t ATTR;
    volatile uint8_t  Resv_64[56];
} CSL_DRU_UTLBRegs_RD_UTLB_ATTR;


typedef struct {
    volatile uint64_t MATCH;
    volatile uint8_t  Resv_64[56];
} CSL_DRU_UTLBRegs_WR_UTLB_MATCH;


typedef struct {
    volatile uint64_t ATTR;
    volatile uint8_t  Resv_64[56];
} CSL_DRU_UTLBRegs_WR_UTLB_ATTR;


typedef struct {
    CSL_DRU_UTLBRegs_RD_UTLB_MATCH RD_UTLB_MATCH[8];
    volatile uint8_t  Resv_4096[3584];
    CSL_DRU_UTLBRegs_RD_UTLB_ATTR RD_UTLB_ATTR[8];
    volatile uint8_t  Resv_8192[3584];
    CSL_DRU_UTLBRegs_WR_UTLB_MATCH WR_UTLB_MATCH[8];
    volatile uint8_t  Resv_12288[3584];
    CSL_DRU_UTLBRegs_WR_UTLB_ATTR WR_UTLB_ATTR[8];
} CSL_DRU_UTLBRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH(RD_UTLB_MATCH)                        (0x00000000U+((RD_UTLB_MATCH)*0x40U))
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR(RD_UTLB_ATTR)                           (0x00001000U+((RD_UTLB_ATTR)*0x40U))
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH(WR_UTLB_MATCH)                        (0x00002000U+((WR_UTLB_MATCH)*0x40U))
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR(WR_UTLB_ATTR)                           (0x00003000U+((WR_UTLB_ATTR)*0x40U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* MATCH */

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_SECURE_MASK                           (0x4000000000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_SECURE_SHIFT                          (0x000000000000003EU)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_SECURE_MAX                            (0x0000000000000001U)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_ROOT_MASK                             (0x2000000000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_ROOT_SHIFT                            (0x000000000000003DU)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_ROOT_MAX                              (0x0000000000000001U)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_GBL_MASK                              (0x1000000000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_GBL_SHIFT                             (0x000000000000003CU)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_GBL_MAX                               (0x0000000000000001U)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_VMID_MASK                             (0x0FF0000000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_VMID_SHIFT                            (0x0000000000000034U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_VMID_MAX                              (0x00000000000000FFU)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_ASID_MASK                             (0x000FF00000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_ASID_SHIFT                            (0x000000000000002CU)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_ASID_MAX                              (0x00000000000000FFU)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_RSVD0_MASK                            (0x0000080000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_RSVD0_SHIFT                           (0x000000000000002BU)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_RSVD0_MAX                             (0x0000000000000001U)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_PG_SIZE_MASK                          (0x0000070000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_PG_SIZE_SHIFT                         (0x0000000000000028U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_PG_SIZE_MAX                           (0x0000000000000007U)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_RSVD1_MASK                            (0x000000E000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_RSVD1_SHIFT                           (0x0000000000000025U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_RSVD1_MAX                             (0x0000000000000007U)

#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_IADDR_MASK                            (0x0000001FFFFFFFFFU)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_IADDR_SHIFT                           (0x0000000000000000U)
#define CSL_DRU_UTLB_RD_UTLB_MATCH_MATCH_IADDR_MAX                             (0x0000001FFFFFFFFFU)

/* ATTR */

#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_STATUS_MASK                             (0x7FF8000000000000U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_STATUS_SHIFT                            (0x0000000000000033U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_STATUS_MAX                              (0x0000000000000FFFU)

#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_ACC_PERM_MASK                           (0x0007FC0000000000U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_ACC_PERM_SHIFT                          (0x000000000000002AU)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_ACC_PERM_MAX                            (0x00000000000001FFU)

#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_PG_NSEC_MASK                            (0x0000020000000000U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_PG_NSEC_SHIFT                           (0x0000000000000029U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_PG_NSEC_MAX                             (0x0000000000000001U)

#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_MEMTYPE_MASK                            (0x000001FF00000000U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_MEMTYPE_SHIFT                           (0x0000000000000020U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_MEMTYPE_MAX                             (0x00000000000001FFU)

#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_RSVD0_MASK                              (0x00000000F0000000U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_RSVD0_SHIFT                             (0x000000000000001CU)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_RSVD0_MAX                               (0x000000000000000FU)

#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_OADDR_MASK                              (0x000000000FFFFFFFU)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_OADDR_SHIFT                             (0x0000000000000000U)
#define CSL_DRU_UTLB_RD_UTLB_ATTR_ATTR_OADDR_MAX                               (0x000000000FFFFFFFU)

/* MATCH */

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_SECURE_MASK                           (0x4000000000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_SECURE_SHIFT                          (0x000000000000003EU)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_SECURE_MAX                            (0x0000000000000001U)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_ROOT_MASK                             (0x2000000000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_ROOT_SHIFT                            (0x000000000000003DU)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_ROOT_MAX                              (0x0000000000000001U)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_GBL_MASK                              (0x1000000000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_GBL_SHIFT                             (0x000000000000003CU)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_GBL_MAX                               (0x0000000000000001U)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_VMID_MASK                             (0x0FF0000000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_VMID_SHIFT                            (0x0000000000000034U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_VMID_MAX                              (0x00000000000000FFU)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_ASID_MASK                             (0x000FF00000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_ASID_SHIFT                            (0x000000000000002CU)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_ASID_MAX                              (0x00000000000000FFU)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_RSVD0_MASK                            (0x0000080000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_RSVD0_SHIFT                           (0x000000000000002BU)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_RSVD0_MAX                             (0x0000000000000001U)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_PG_SIZE_MASK                          (0x0000070000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_PG_SIZE_SHIFT                         (0x0000000000000028U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_PG_SIZE_MAX                           (0x0000000000000007U)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_RSVD1_MASK                            (0x000000E000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_RSVD1_SHIFT                           (0x0000000000000025U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_RSVD1_MAX                             (0x0000000000000007U)

#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_IADDR_MASK                            (0x0000001FFFFFFFFFU)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_IADDR_SHIFT                           (0x0000000000000000U)
#define CSL_DRU_UTLB_WR_UTLB_MATCH_MATCH_IADDR_MAX                             (0x0000001FFFFFFFFFU)

/* ATTR */

#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_STATUS_MASK                             (0x7FF8000000000000U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_STATUS_SHIFT                            (0x0000000000000033U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_STATUS_MAX                              (0x0000000000000FFFU)

#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_ACC_PERM_MASK                           (0x0007FC0000000000U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_ACC_PERM_SHIFT                          (0x000000000000002AU)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_ACC_PERM_MAX                            (0x00000000000001FFU)

#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_PG_NSEC_MASK                            (0x0000020000000000U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_PG_NSEC_SHIFT                           (0x0000000000000029U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_PG_NSEC_MAX                             (0x0000000000000001U)

#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_MEMTYPE_MASK                            (0x000001FF00000000U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_MEMTYPE_SHIFT                           (0x0000000000000020U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_MEMTYPE_MAX                             (0x00000000000001FFU)

#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_RSVD0_MASK                              (0x00000000F0000000U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_RSVD0_SHIFT                             (0x000000000000001CU)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_RSVD0_MAX                               (0x000000000000000FU)

#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_OADDR_MASK                              (0x000000000FFFFFFFU)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_OADDR_SHIFT                             (0x0000000000000000U)
#define CSL_DRU_UTLB_WR_UTLB_ATTR_ATTR_OADDR_MAX                               (0x000000000FFFFFFFU)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t CFG[7];                    /* Configuration Register for Queue 0 */
    volatile uint8_t  Resv_64[8];
    volatile uint64_t STATUS[7];                 /* Status Register for Queue 0 */
} CSL_DRU_QUEUERegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_QUEUE_CFG(CFG)                                                 (0x00000000U+((CFG)*0x8U))
#define CSL_DRU_QUEUE_STATUS(STATUS)                                           (0x00000040U+((STATUS)*0x8U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CFG */

#define CSL_DRU_QUEUE_CFG_CONSECUTIVE_TRANS_MASK                               (0x0000000000FF0000U)
#define CSL_DRU_QUEUE_CFG_CONSECUTIVE_TRANS_SHIFT                              (0x0000000000000010U)
#define CSL_DRU_QUEUE_CFG_CONSECUTIVE_TRANS_MAX                                (0x00000000000000FFU)

#define CSL_DRU_QUEUE_CFG_ORDERID_MASK                                         (0x00000000000000F0U)
#define CSL_DRU_QUEUE_CFG_ORDERID_SHIFT                                        (0x0000000000000004U)
#define CSL_DRU_QUEUE_CFG_ORDERID_MAX                                          (0x000000000000000FU)

#define CSL_DRU_QUEUE_CFG_PRI_MASK                                             (0x0000000000000007U)
#define CSL_DRU_QUEUE_CFG_PRI_SHIFT                                            (0x0000000000000000U)
#define CSL_DRU_QUEUE_CFG_PRI_MAX                                              (0x0000000000000007U)

#define CSL_DRU_QUEUE_CFG_QOS_MASK                                             (0x0000000000000700U)
#define CSL_DRU_QUEUE_CFG_QOS_SHIFT                                            (0x0000000000000008U)
#define CSL_DRU_QUEUE_CFG_QOS_MAX                                              (0x0000000000000007U)

#define CSL_DRU_QUEUE_CFG_REARB_WAIT_MASK                                      (0x00000000FF000000U)
#define CSL_DRU_QUEUE_CFG_REARB_WAIT_SHIFT                                     (0x0000000000000018U)
#define CSL_DRU_QUEUE_CFG_REARB_WAIT_MAX                                       (0x00000000000000FFU)

#define CSL_DRU_QUEUE_CFG_RSVD_MASK                                            (0xFFFFFFFF00000000U)
#define CSL_DRU_QUEUE_CFG_RSVD_SHIFT                                           (0x0000000000000020U)
#define CSL_DRU_QUEUE_CFG_RSVD_MAX                                             (0x00000000FFFFFFFFU)

/* STATUS */

#define CSL_DRU_QUEUE_STATUS_RD_TOP_MASK                                       (0x0000000007FC0000U)
#define CSL_DRU_QUEUE_STATUS_RD_TOP_SHIFT                                      (0x0000000000000012U)
#define CSL_DRU_QUEUE_STATUS_RD_TOP_MAX                                        (0x00000000000001FFU)

#define CSL_DRU_QUEUE_STATUS_RD_TOTAL_MASK                                     (0x0000000FF8000000U)
#define CSL_DRU_QUEUE_STATUS_RD_TOTAL_SHIFT                                    (0x000000000000001BU)
#define CSL_DRU_QUEUE_STATUS_RD_TOTAL_MAX                                      (0x00000000000001FFU)

#define CSL_DRU_QUEUE_STATUS_RSVD_MASK                                         (0xFFFFFFF000000000U)
#define CSL_DRU_QUEUE_STATUS_RSVD_SHIFT                                        (0x0000000000000024U)
#define CSL_DRU_QUEUE_STATUS_RSVD_MAX                                          (0x000000000FFFFFFFU)

#define CSL_DRU_QUEUE_STATUS_WR_TOP_MASK                                       (0x00000000000001FFU)
#define CSL_DRU_QUEUE_STATUS_WR_TOP_SHIFT                                      (0x0000000000000000U)
#define CSL_DRU_QUEUE_STATUS_WR_TOP_MAX                                        (0x00000000000001FFU)

#define CSL_DRU_QUEUE_STATUS_WR_TOTAL_MASK                                     (0x000000000003FE00U)
#define CSL_DRU_QUEUE_STATUS_WR_TOTAL_SHIFT                                    (0x0000000000000009U)
#define CSL_DRU_QUEUE_STATUS_WR_TOTAL_MAX                                      (0x00000000000001FFU)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t DRU_SHARED_EVT_SET;        /* DRU Shared Event Set Register */
    volatile uint8_t  Resv_64[56];
    volatile uint64_t DRU_COMP_EVT_SET0;         /* DRU Completion Event Set Register */
    volatile uint8_t  Resv_128[56];
    volatile uint64_t DRU_ERR_EVT_SET0;          /* DRU Error Event Set Register */
    volatile uint8_t  Resv_192[56];
    volatile uint64_t DRU_LOCAL_EVT_SET0;        /* DRU Local Event Set Register */
} CSL_DRU_SETRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_SET_DRU_SHARED_EVT_SET                                         (0x00000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0                                          (0x00000040U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0                                           (0x00000080U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0                                         (0x000000C0U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* DRU_SHARED_EVT_SET */

#define CSL_DRU_SET_DRU_SHARED_EVT_SET_PROT_ERR_MASK                           (0x0000000000000001U)
#define CSL_DRU_SET_DRU_SHARED_EVT_SET_PROT_ERR_SHIFT                          (0x0000000000000000U)
#define CSL_DRU_SET_DRU_SHARED_EVT_SET_PROT_ERR_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_SHARED_EVT_SET_RSVD_MASK                               (0xFFFFFFFFFFFFFFFEU)
#define CSL_DRU_SET_DRU_SHARED_EVT_SET_RSVD_SHIFT                              (0x0000000000000001U)
#define CSL_DRU_SET_DRU_SHARED_EVT_SET_RSVD_MAX                                (0x7FFFFFFFFFFFFFFFU)

/* DRU_COMP_EVT_SET0 */

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT0_MASK                           (0x0000000000000001U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT0_SHIFT                          (0x0000000000000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT0_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT1_MASK                           (0x0000000000000002U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT1_SHIFT                          (0x0000000000000001U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT1_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT2_MASK                           (0x0000000000000004U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT2_SHIFT                          (0x0000000000000002U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT2_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT3_MASK                           (0x0000000000000008U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT3_SHIFT                          (0x0000000000000003U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT3_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT4_MASK                           (0x0000000000000010U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT4_SHIFT                          (0x0000000000000004U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT4_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT5_MASK                           (0x0000000000000020U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT5_SHIFT                          (0x0000000000000005U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT5_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT6_MASK                           (0x0000000000000040U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT6_SHIFT                          (0x0000000000000006U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT6_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT7_MASK                           (0x0000000000000080U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT7_SHIFT                          (0x0000000000000007U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT7_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT8_MASK                           (0x0000000000000100U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT8_SHIFT                          (0x0000000000000008U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT8_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT9_MASK                           (0x0000000000000200U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT9_SHIFT                          (0x0000000000000009U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT9_MAX                            (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT10_MASK                          (0x0000000000000400U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT10_SHIFT                         (0x000000000000000AU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT10_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT11_MASK                          (0x0000000000000800U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT11_SHIFT                         (0x000000000000000BU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT11_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT12_MASK                          (0x0000000000001000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT12_SHIFT                         (0x000000000000000CU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT12_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT13_MASK                          (0x0000000000002000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT13_SHIFT                         (0x000000000000000DU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT13_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT14_MASK                          (0x0000000000004000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT14_SHIFT                         (0x000000000000000EU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT14_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT15_MASK                          (0x0000000000008000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT15_SHIFT                         (0x000000000000000FU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT15_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT16_MASK                          (0x0000000000010000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT16_SHIFT                         (0x0000000000000010U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT16_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT17_MASK                          (0x0000000000020000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT17_SHIFT                         (0x0000000000000011U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT17_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT18_MASK                          (0x0000000000040000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT18_SHIFT                         (0x0000000000000012U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT18_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT19_MASK                          (0x0000000000080000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT19_SHIFT                         (0x0000000000000013U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT19_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT20_MASK                          (0x0000000000100000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT20_SHIFT                         (0x0000000000000014U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT20_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT21_MASK                          (0x0000000000200000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT21_SHIFT                         (0x0000000000000015U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT21_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT22_MASK                          (0x0000000000400000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT22_SHIFT                         (0x0000000000000016U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT22_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT23_MASK                          (0x0000000000800000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT23_SHIFT                         (0x0000000000000017U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT23_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT24_MASK                          (0x0000000001000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT24_SHIFT                         (0x0000000000000018U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT24_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT25_MASK                          (0x0000000002000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT25_SHIFT                         (0x0000000000000019U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT25_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT26_MASK                          (0x0000000004000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT26_SHIFT                         (0x000000000000001AU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT26_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT27_MASK                          (0x0000000008000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT27_SHIFT                         (0x000000000000001BU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT27_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT28_MASK                          (0x0000000010000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT28_SHIFT                         (0x000000000000001CU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT28_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT29_MASK                          (0x0000000020000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT29_SHIFT                         (0x000000000000001DU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT29_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT30_MASK                          (0x0000000040000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT30_SHIFT                         (0x000000000000001EU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT30_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT31_MASK                          (0x0000000080000000U)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT31_SHIFT                         (0x000000000000001FU)
#define CSL_DRU_SET_DRU_COMP_EVT_SET0_COMP_EVT31_MAX                           (0x0000000000000001U)

/* DRU_ERR_EVT_SET0 */

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT0_MASK                             (0x0000000000000001U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT0_SHIFT                            (0x0000000000000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT0_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT1_MASK                             (0x0000000000000002U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT1_SHIFT                            (0x0000000000000001U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT1_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT2_MASK                             (0x0000000000000004U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT2_SHIFT                            (0x0000000000000002U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT2_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT3_MASK                             (0x0000000000000008U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT3_SHIFT                            (0x0000000000000003U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT3_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT4_MASK                             (0x0000000000000010U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT4_SHIFT                            (0x0000000000000004U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT4_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT5_MASK                             (0x0000000000000020U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT5_SHIFT                            (0x0000000000000005U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT5_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT6_MASK                             (0x0000000000000040U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT6_SHIFT                            (0x0000000000000006U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT6_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT7_MASK                             (0x0000000000000080U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT7_SHIFT                            (0x0000000000000007U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT7_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT8_MASK                             (0x0000000000000100U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT8_SHIFT                            (0x0000000000000008U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT8_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT9_MASK                             (0x0000000000000200U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT9_SHIFT                            (0x0000000000000009U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT9_MAX                              (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT10_MASK                            (0x0000000000000400U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT10_SHIFT                           (0x000000000000000AU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT10_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT11_MASK                            (0x0000000000000800U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT11_SHIFT                           (0x000000000000000BU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT11_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT12_MASK                            (0x0000000000001000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT12_SHIFT                           (0x000000000000000CU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT12_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT13_MASK                            (0x0000000000002000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT13_SHIFT                           (0x000000000000000DU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT13_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT14_MASK                            (0x0000000000004000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT14_SHIFT                           (0x000000000000000EU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT14_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT15_MASK                            (0x0000000000008000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT15_SHIFT                           (0x000000000000000FU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT15_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT16_MASK                            (0x0000000000010000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT16_SHIFT                           (0x0000000000000010U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT16_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT17_MASK                            (0x0000000000020000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT17_SHIFT                           (0x0000000000000011U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT17_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT18_MASK                            (0x0000000000040000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT18_SHIFT                           (0x0000000000000012U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT18_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT19_MASK                            (0x0000000000080000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT19_SHIFT                           (0x0000000000000013U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT19_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT20_MASK                            (0x0000000000100000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT20_SHIFT                           (0x0000000000000014U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT20_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT21_MASK                            (0x0000000000200000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT21_SHIFT                           (0x0000000000000015U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT21_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT22_MASK                            (0x0000000000400000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT22_SHIFT                           (0x0000000000000016U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT22_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT23_MASK                            (0x0000000000800000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT23_SHIFT                           (0x0000000000000017U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT23_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT24_MASK                            (0x0000000001000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT24_SHIFT                           (0x0000000000000018U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT24_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT25_MASK                            (0x0000000002000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT25_SHIFT                           (0x0000000000000019U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT25_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT26_MASK                            (0x0000000004000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT26_SHIFT                           (0x000000000000001AU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT26_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT27_MASK                            (0x0000000008000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT27_SHIFT                           (0x000000000000001BU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT27_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT28_MASK                            (0x0000000010000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT28_SHIFT                           (0x000000000000001CU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT28_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT29_MASK                            (0x0000000020000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT29_SHIFT                           (0x000000000000001DU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT29_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT30_MASK                            (0x0000000040000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT30_SHIFT                           (0x000000000000001EU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT30_MAX                             (0x0000000000000001U)

#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT31_MASK                            (0x0000000080000000U)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT31_SHIFT                           (0x000000000000001FU)
#define CSL_DRU_SET_DRU_ERR_EVT_SET0_ERR_EVT31_MAX                             (0x0000000000000001U)

/* DRU_LOCAL_EVT_SET0 */

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT0_MASK                          (0x0000000000000001U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT0_SHIFT                         (0x0000000000000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT0_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT1_MASK                          (0x0000000000000002U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT1_SHIFT                         (0x0000000000000001U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT1_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT2_MASK                          (0x0000000000000004U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT2_SHIFT                         (0x0000000000000002U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT2_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT3_MASK                          (0x0000000000000008U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT3_SHIFT                         (0x0000000000000003U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT3_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT4_MASK                          (0x0000000000000010U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT4_SHIFT                         (0x0000000000000004U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT4_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT5_MASK                          (0x0000000000000020U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT5_SHIFT                         (0x0000000000000005U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT5_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT6_MASK                          (0x0000000000000040U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT6_SHIFT                         (0x0000000000000006U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT6_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT7_MASK                          (0x0000000000000080U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT7_SHIFT                         (0x0000000000000007U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT7_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT8_MASK                          (0x0000000000000100U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT8_SHIFT                         (0x0000000000000008U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT8_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT9_MASK                          (0x0000000000000200U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT9_SHIFT                         (0x0000000000000009U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT9_MAX                           (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT10_MASK                         (0x0000000000000400U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT10_SHIFT                        (0x000000000000000AU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT10_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT11_MASK                         (0x0000000000000800U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT11_SHIFT                        (0x000000000000000BU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT11_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT12_MASK                         (0x0000000000001000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT12_SHIFT                        (0x000000000000000CU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT12_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT13_MASK                         (0x0000000000002000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT13_SHIFT                        (0x000000000000000DU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT13_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT14_MASK                         (0x0000000000004000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT14_SHIFT                        (0x000000000000000EU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT14_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT15_MASK                         (0x0000000000008000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT15_SHIFT                        (0x000000000000000FU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT15_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT16_MASK                         (0x0000000000010000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT16_SHIFT                        (0x0000000000000010U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT16_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT17_MASK                         (0x0000000000020000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT17_SHIFT                        (0x0000000000000011U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT17_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT18_MASK                         (0x0000000000040000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT18_SHIFT                        (0x0000000000000012U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT18_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT19_MASK                         (0x0000000000080000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT19_SHIFT                        (0x0000000000000013U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT19_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT20_MASK                         (0x0000000000100000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT20_SHIFT                        (0x0000000000000014U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT20_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT21_MASK                         (0x0000000000200000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT21_SHIFT                        (0x0000000000000015U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT21_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT22_MASK                         (0x0000000000400000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT22_SHIFT                        (0x0000000000000016U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT22_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT23_MASK                         (0x0000000000800000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT23_SHIFT                        (0x0000000000000017U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT23_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT24_MASK                         (0x0000000001000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT24_SHIFT                        (0x0000000000000018U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT24_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT25_MASK                         (0x0000000002000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT25_SHIFT                        (0x0000000000000019U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT25_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT26_MASK                         (0x0000000004000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT26_SHIFT                        (0x000000000000001AU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT26_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT27_MASK                         (0x0000000008000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT27_SHIFT                        (0x000000000000001BU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT27_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT28_MASK                         (0x0000000010000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT28_SHIFT                        (0x000000000000001CU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT28_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT29_MASK                         (0x0000000020000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT29_SHIFT                        (0x000000000000001DU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT29_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT30_MASK                         (0x0000000040000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT30_SHIFT                        (0x000000000000001EU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT30_MAX                          (0x0000000000000001U)

#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT31_MASK                         (0x0000000080000000U)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT31_SHIFT                        (0x000000000000001FU)
#define CSL_DRU_SET_DRU_LOCAL_EVT_SET0_COMP_EVT31_MAX                          (0x0000000000000001U)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t CFG;                       /* Channel Configuration Register. The Channel Configuration Register is used to initialize static mode settings for the DMA Channel. The register may only be written when the chanel is disable in the ch_enable field in the REALTIME CONTROL register is 0. */
    volatile uint8_t  Resv_32[24];
    volatile uint64_t CHOES0;                    /* The Output Event Steering Registers are used to specify a global event number to generate anytime the required event generation criteria specified in a TR are met. A single event with the event number set equal to the value in the corresponding register will be generated.  This register is provided in order to allow security SW to lock down which events in the global space any given channel/ thread is allowed to generate */
    volatile uint8_t  Resv_96[56];
    volatile uint64_t CHST_SCHED;                /* Channel Static Scheduler Config Register */
    volatile uint8_t  Resv_256[152];
} CSL_DRU_CHNRTRegs_CHNRT;


typedef struct {
    CSL_DRU_CHNRTRegs_CHNRT CHNRT[32];
} CSL_DRU_CHNRTRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_CHNRT_CFG(CHNRT)                                         (0x00000000U+((CHNRT)*0x100U))
#define CSL_DRU_CHNRT_CHOES0(CHNRT)                                      (0x00000020U+((CHNRT)*0x100U))
#define CSL_DRU_CHNRT_CHST_SCHED(CHNRT)                                  (0x00000060U+((CHNRT)*0x100U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CFG */

#define CSL_DRU_CHNRT_CFG_CHAN_TYPE_MASK                                 (0x0000000000070000U)
#define CSL_DRU_CHNRT_CFG_CHAN_TYPE_SHIFT                                (0x0000000000000010U)
#define CSL_DRU_CHNRT_CFG_CHAN_TYPE_MAX                                  (0x0000000000000007U)

#define CSL_DRU_CHNRT_CFG_CHAN_TYPE_OWNER_MASK                           (0x0000000000080000U)
#define CSL_DRU_CHNRT_CFG_CHAN_TYPE_OWNER_SHIFT                          (0x0000000000000013U)
#define CSL_DRU_CHNRT_CFG_CHAN_TYPE_OWNER_MAX                            (0x0000000000000001U)

#define CSL_DRU_CHNRT_CFG_ATYPE_MASK                                     (0x0000000002000000U)
#define CSL_DRU_CHNRT_CFG_ATYPE_SHIFT                                    (0x0000000000000019U)
#define CSL_DRU_CHNRT_CFG_ATYPE_MAX                                      (0x0000000000000001U)

#define CSL_DRU_CHNRT_CFG_PAUSE_ON_ERR_MASK                              (0x0000000080000000U)
#define CSL_DRU_CHNRT_CFG_PAUSE_ON_ERR_SHIFT                             (0x000000000000001FU)
#define CSL_DRU_CHNRT_CFG_PAUSE_ON_ERR_MAX                               (0x0000000000000001U)

#define CSL_DRU_CHNRT_CFG_RSVD_MASK                                      (0xFFFFFFFF00000000U)
#define CSL_DRU_CHNRT_CFG_RSVD_SHIFT                                     (0x0000000000000020U)
#define CSL_DRU_CHNRT_CFG_RSVD_MAX                                       (0x00000000FFFFFFFFU)

/* CHOES0 */

#define CSL_DRU_CHNRT_CHOES0_EVT_NUM_MASK                                (0x000000000000FFFFU)
#define CSL_DRU_CHNRT_CHOES0_EVT_NUM_SHIFT                               (0x0000000000000000U)
#define CSL_DRU_CHNRT_CHOES0_EVT_NUM_MAX                                 (0x000000000000FFFFU)

#define CSL_DRU_CHNRT_CHOES0_RSVD_MASK                                   (0xFFFFFFFFFFFF0000U)
#define CSL_DRU_CHNRT_CHOES0_RSVD_SHIFT                                  (0x0000000000000010U)
#define CSL_DRU_CHNRT_CHOES0_RSVD_MAX                                    (0x0000FFFFFFFFFFFFU)

/* CHST_SCHED */

#define CSL_DRU_CHNRT_CHST_SCHED_QUEUE_MASK                              (0x0000000000000007U)
#define CSL_DRU_CHNRT_CHST_SCHED_QUEUE_SHIFT                             (0x0000000000000000U)
#define CSL_DRU_CHNRT_CHST_SCHED_QUEUE_MAX                               (0x0000000000000007U)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t CHRT_CTL;                  /* The channel realtime control register contains real-time cotrol and status information for the DMA Channel. The fields in this regsiter can be changed while the channel is in operation. */
    volatile uint64_t CHRT_SWTRIG;               /* The Software Trigger Register provides a mechanism by which software can directly trigger the channel in a secure way. */
    volatile uint64_t CHRT_STATUS_DET;           /* The channel status details */
    volatile uint64_t CHRT_STATUS_CNT;           /* The channel count details */
    volatile uint8_t  Resv_256[224];
} CSL_DRU_CHRTRegs_CHRT;


typedef struct {
    CSL_DRU_CHRTRegs_CHRT CHRT[32];
} CSL_DRU_CHRTRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_CHRT_CTL(CHRT)                                       (0x00000000U+((CHRT)*0x100U))
#define CSL_DRU_CHRT_SWTRIG(CHRT)                                    (0x00000008U+((CHRT)*0x100U))
#define CSL_DRU_CHRT_STATUS_DET(CHRT)                                (0x00000010U+((CHRT)*0x100U))
#define CSL_DRU_CHRT_STATUS_CNT(CHRT)                                (0x00000018U+((CHRT)*0x100U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CHRT_CTL */

#define CSL_DRU_CHRT_CTL_ENABLE_MASK                                 (0x0000000080000000U)
#define CSL_DRU_CHRT_CTL_ENABLE_SHIFT                                (0x000000000000001FU)
#define CSL_DRU_CHRT_CTL_ENABLE_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CHRT_CTL_PAUSE_MASK                                  (0x0000000020000000U)
#define CSL_DRU_CHRT_CTL_PAUSE_SHIFT                                 (0x000000000000001DU)
#define CSL_DRU_CHRT_CTL_PAUSE_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CHRT_CTL_RSVD_MASK                                   (0xFFFFFFFF00000000U)
#define CSL_DRU_CHRT_CTL_RSVD_SHIFT                                  (0x0000000000000020U)
#define CSL_DRU_CHRT_CTL_RSVD_MAX                                    (0x00000000FFFFFFFFU)

#define CSL_DRU_CHRT_CTL_TEARDOWN_MASK                               (0x0000000040000000U)
#define CSL_DRU_CHRT_CTL_TEARDOWN_SHIFT                              (0x000000000000001EU)
#define CSL_DRU_CHRT_CTL_TEARDOWN_MAX                                (0x0000000000000001U)

#define CSL_DRU_CHRT_CTL_FORCED_TEARDOWN_MASK                        (0x0000000010000000U)
#define CSL_DRU_CHRT_CTL_FORCED_TEARDOWN_SHIFT                       (0x000000000000001CU)
#define CSL_DRU_CHRT_CTL_FORCED_TEARDOWN_MAX                         (0x0000000000000001U)

/* CHRT_SWTRIG */

#define CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER0_MASK                     (0x0000000000000001U)
#define CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER0_SHIFT                    (0x0000000000000000U)
#define CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER0_MAX                      (0x0000000000000001U)

#define CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER1_MASK                     (0x0000000000000002U)
#define CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER1_SHIFT                    (0x0000000000000001U)
#define CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER1_MAX                      (0x0000000000000001U)

#define CSL_DRU_CHRT_SWTRIG_LOCAL_TRIGGER0_MASK                      (0x0000000000000004U)
#define CSL_DRU_CHRT_SWTRIG_LOCAL_TRIGGER0_SHIFT                     (0x0000000000000002U)
#define CSL_DRU_CHRT_SWTRIG_LOCAL_TRIGGER0_MAX                       (0x0000000000000001U)

#define CSL_DRU_CHRT_SWTRIG_RSVD_MASK                                (0xFFFFFFFFFFFFFFF8U)
#define CSL_DRU_CHRT_SWTRIG_RSVD_SHIFT                               (0x0000000000000003U)
#define CSL_DRU_CHRT_SWTRIG_RSVD_MAX                                 (0x1FFFFFFFFFFFFFFFU)

/* CHRT_STATUS_DET */

#define CSL_DRU_CHRT_STATUS_DET_STATUS_TYPE_MASK                     (0x000000000000000FU)
#define CSL_DRU_CHRT_STATUS_DET_STATUS_TYPE_SHIFT                    (0x0000000000000000U)
#define CSL_DRU_CHRT_STATUS_DET_STATUS_TYPE_MAX                      (0x000000000000000FU)

#define CSL_DRU_CHRT_STATUS_DET_INFO_MASK                            (0x00000000000000F0U)
#define CSL_DRU_CHRT_STATUS_DET_INFO_SHIFT                           (0x0000000000000004U)
#define CSL_DRU_CHRT_STATUS_DET_INFO_MAX                             (0x000000000000000FU)

#define CSL_DRU_CHRT_STATUS_DET_CMD_ID_MASK                          (0x000000000000FF00U)
#define CSL_DRU_CHRT_STATUS_DET_CMD_ID_SHIFT                         (0x0000000000000008U)
#define CSL_DRU_CHRT_STATUS_DET_CMD_ID_MAX                           (0x00000000000000FFU)

#define CSL_DRU_CHRT_STATUS_DET_TR_CNT_MASK                          (0x0000000000FF0000U)
#define CSL_DRU_CHRT_STATUS_DET_TR_CNT_SHIFT                         (0x0000000000000010U)
#define CSL_DRU_CHRT_STATUS_DET_TR_CNT_MAX                           (0x00000000000000FFU)

#define CSL_DRU_CHRT_STATUS_DET_TR_IN_QUEUE_CNT_MASK                 (0x00000000FF000000U)
#define CSL_DRU_CHRT_STATUS_DET_TR_IN_QUEUE_CNT_SHIFT                (0x0000000000000018U)
#define CSL_DRU_CHRT_STATUS_DET_TR_IN_QUEUE_CNT_MAX                  (0x00000000000000FFU)

#define CSL_DRU_CHRT_STATUS_DET_RSVD_MASK                            (0x1FFFFFFF00000000U)
#define CSL_DRU_CHRT_STATUS_DET_RSVD_SHIFT                           (0x0000000000000020U)
#define CSL_DRU_CHRT_STATUS_DET_RSVD_MAX                             (0x000000001FFFFFFFU)

#define CSL_DRU_CHRT_STATUS_DET_RD_ACTIVE_MASK                       (0x2000000000000000U)
#define CSL_DRU_CHRT_STATUS_DET_RD_ACTIVE_SHIFT                      (0x000000000000003DU)
#define CSL_DRU_CHRT_STATUS_DET_RD_ACTIVE_MAX                        (0x0000000000000001U)

#define CSL_DRU_CHRT_STATUS_DET_WR_ACTIVE_MASK                       (0x4000000000000000U)
#define CSL_DRU_CHRT_STATUS_DET_WR_ACTIVE_SHIFT                      (0x000000000000003EU)
#define CSL_DRU_CHRT_STATUS_DET_WR_ACTIVE_MAX                        (0x0000000000000001U)

#define CSL_DRU_CHRT_STATUS_DET_CH_ACTIVE_MASK                       (0x8000000000000000U)
#define CSL_DRU_CHRT_STATUS_DET_CH_ACTIVE_SHIFT                      (0x000000000000003FU)
#define CSL_DRU_CHRT_STATUS_DET_CH_ACTIVE_MAX                        (0x0000000000000001U)

/* CHRT_STATUS_CNT */

#define CSL_DRU_CHRT_STATUS_CNT_ICNT0_MASK                           (0x000000000000FFFFU)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT0_SHIFT                          (0x0000000000000000U)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT0_MAX                            (0x000000000000FFFFU)

#define CSL_DRU_CHRT_STATUS_CNT_ICNT1_MASK                           (0x00000000FFFF0000U)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT1_SHIFT                          (0x0000000000000010U)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT1_MAX                            (0x000000000000FFFFU)

#define CSL_DRU_CHRT_STATUS_CNT_ICNT2_MASK                           (0x0000FFFF00000000U)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT2_SHIFT                          (0x0000000000000020U)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT2_MAX                            (0x000000000000FFFFU)

#define CSL_DRU_CHRT_STATUS_CNT_ICNT3_MASK                           (0xFFFF000000000000U)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT3_SHIFT                          (0x0000000000000030U)
#define CSL_DRU_CHRT_STATUS_CNT_ICNT3_MAX                            (0x000000000000FFFFU)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t NEXT_TR_WORD0_1;           /* The first TR submission word */
    volatile uint64_t NEXT_TR_WORD2_3;           /* The second TR submission word */
    volatile uint64_t NEXT_TR_WORD4_5;           /* The third TR submission word */
    volatile uint64_t NEXT_TR_WORD6_7;           /* The fourth TR submission word */
    volatile uint64_t NEXT_TR_WORD8_9;           /* The fifth TR submission word */
    volatile uint64_t NEXT_TR_WORD10_11;         /* The sixth TR submission word */
    volatile uint64_t NEXT_TR_WORD12_13;         /* The seventh TR submission word */
    volatile uint64_t NEXT_TR_WORD14_15;         /* The eight TR submission word */
} CSL_DRU_CHATOMIC_DEBUGRegs_CHATOMIC_DEBUG_DEBUG;


typedef struct {
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD0_1;   /* The first TR submission word */
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD2_3;   /* The second TR submission word */
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD4_5;   /* The third TR submission word */
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD6_7;   /* The fourth TR submission word */
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD8_9;   /* The fifth TR submission word */
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD10_11;   /* The sixth TR submission word */
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD12_13;   /* The seventh TR submission word */
    volatile uint64_t ATOMIC_SUBMIT_CURR_TR_WORD14_15;   /* The eight TR submission word */
    CSL_DRU_CHATOMIC_DEBUGRegs_CHATOMIC_DEBUG_DEBUG DEBUG[3];
} CSL_DRU_CHATOMIC_DEBUGRegs_CHATOMIC_DEBUG;


typedef struct {
    CSL_DRU_CHATOMIC_DEBUGRegs_CHATOMIC_DEBUG CHATOMIC_DEBUG[32];
} CSL_DRU_CHATOMIC_DEBUGRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1(CHATOMIC_DEBUG) (0x00000000U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD2_3(CHATOMIC_DEBUG) (0x00000008U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5(CHATOMIC_DEBUG) (0x00000010U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD6_7(CHATOMIC_DEBUG) (0x00000018U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD8_9(CHATOMIC_DEBUG) (0x00000020U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD10_11(CHATOMIC_DEBUG) (0x00000028U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD12_13(CHATOMIC_DEBUG) (0x00000030U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15(CHATOMIC_DEBUG) (0x00000038U+((CHATOMIC_DEBUG)*0x100U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1(CHATOMIC_DEBUG, DEBUG) (0x00000040U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD2_3(CHATOMIC_DEBUG, DEBUG) (0x00000048U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5(CHATOMIC_DEBUG, DEBUG) (0x00000050U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD6_7(CHATOMIC_DEBUG, DEBUG) (0x00000058U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD8_9(CHATOMIC_DEBUG, DEBUG) (0x00000060U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD10_11(CHATOMIC_DEBUG, DEBUG) (0x00000068U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD12_13(CHATOMIC_DEBUG, DEBUG) (0x00000070U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15(CHATOMIC_DEBUG, DEBUG) (0x00000078U+((CHATOMIC_DEBUG)*0x100U)+((DEBUG)*0x40U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* NEXT_TR_WORD0_1 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_FLAGS_MASK         (0x00000000FFFFFFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_FLAGS_SHIFT        (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_FLAGS_MAX          (0x00000000FFFFFFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_ICNT0_MASK         (0x0000FFFF00000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_ICNT0_SHIFT        (0x0000000000000020U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_ICNT0_MAX          (0x000000000000FFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_ICNT1_MASK         (0xFFFF000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_ICNT1_SHIFT        (0x0000000000000030U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD0_1_ICNT1_MAX          (0x000000000000FFFFU)

/* NEXT_TR_WORD2_3 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD2_3_RSVD_MASK          (0xFFFF000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD2_3_RSVD_SHIFT         (0x0000000000000030U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD2_3_RSVD_MAX           (0x000000000000FFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD2_3_SRC_ADDR_MASK      (0x0000FFFFFFFFFFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD2_3_SRC_ADDR_SHIFT     (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD2_3_SRC_ADDR_MAX       (0x0000FFFFFFFFFFFFU)

/* NEXT_TR_WORD4_5 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_DIM1_MASK          (0x00000000FFFFFFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_DIM1_SHIFT         (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_DIM1_MAX           (0x00000000FFFFFFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_ICNT2_MASK         (0x0000FFFF00000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_ICNT2_SHIFT        (0x0000000000000020U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_ICNT2_MAX          (0x000000000000FFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_ICNT3_MASK         (0xFFFF000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_ICNT3_SHIFT        (0x0000000000000030U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD4_5_ICNT3_MAX          (0x000000000000FFFFU)

/* NEXT_TR_WORD6_7 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD6_7_DIM2_MASK          (0x00000000FFFFFFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD6_7_DIM2_SHIFT         (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD6_7_DIM2_MAX           (0x00000000FFFFFFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD6_7_DIM3_MASK          (0xFFFFFFFF00000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD6_7_DIM3_SHIFT         (0x0000000000000020U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD6_7_DIM3_MAX           (0x00000000FFFFFFFFU)

/* NEXT_TR_WORD8_9 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD8_9_DDIM1_MASK         (0xFFFFFFFF00000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD8_9_DDIM1_SHIFT        (0x0000000000000020U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD8_9_DDIM1_MAX          (0x00000000FFFFFFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD8_9_FMTFLAGS_MASK      (0x00000000FFFFFFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD8_9_FMTFLAGS_SHIFT     (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD8_9_FMTFLAGS_MAX       (0x00000000FFFFFFFFU)

/* NEXT_TR_WORD10_11 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD10_11_DADDR_MASK       (0x0000FFFFFFFFFFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD10_11_DADDR_SHIFT      (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD10_11_DADDR_MAX        (0x0000FFFFFFFFFFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD10_11_RSVD_MASK        (0xFFFF000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD10_11_RSVD_SHIFT       (0x0000000000000030U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD10_11_RSVD_MAX         (0x000000000000FFFFU)

/* NEXT_TR_WORD12_13 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD12_13_DDIM2_MASK       (0x00000000FFFFFFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD12_13_DDIM2_SHIFT      (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD12_13_DDIM2_MAX        (0x00000000FFFFFFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD12_13_DDIM3_MASK       (0xFFFFFFFF00000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD12_13_DDIM3_SHIFT      (0x0000000000000020U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD12_13_DDIM3_MAX        (0x00000000FFFFFFFFU)

/* NEXT_TR_WORD14_15 */

#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT0_MASK      (0x000000000000FFFFU)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT0_SHIFT     (0x0000000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT0_MAX       (0x000000000000FFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT1_MASK      (0x00000000FFFF0000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT1_SHIFT     (0x0000000000000010U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT1_MAX       (0x000000000000FFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT2_MASK      (0x0000FFFF00000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT2_SHIFT     (0x0000000000000020U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT2_MAX       (0x000000000000FFFFU)

#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT3_MASK      (0xFFFF000000000000U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT3_SHIFT     (0x0000000000000030U)
#define CSL_DRU_DEBUG_NEXT_TR_WORD14_15_DICNT3_MAX       (0x000000000000FFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD0_1 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_FLAGS_MASK (0x00000000FFFFFFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_FLAGS_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_FLAGS_MAX  (0x00000000FFFFFFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_ICNT0_MASK (0x0000FFFF00000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_ICNT0_SHIFT (0x0000000000000020U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_ICNT0_MAX  (0x000000000000FFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_ICNT1_MASK (0xFFFF000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_ICNT1_SHIFT (0x0000000000000030U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD0_1_ICNT1_MAX  (0x000000000000FFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD2_3 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD2_3_RSVD_MASK  (0xFFFF000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD2_3_RSVD_SHIFT (0x0000000000000030U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD2_3_RSVD_MAX   (0x000000000000FFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD2_3_SRC_ADDR_MASK (0x0000FFFFFFFFFFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD2_3_SRC_ADDR_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD2_3_SRC_ADDR_MAX (0x0000FFFFFFFFFFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD4_5 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_DIM1_MASK  (0x00000000FFFFFFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_DIM1_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_DIM1_MAX   (0x00000000FFFFFFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_ICNT2_MASK (0x0000FFFF00000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_ICNT2_SHIFT (0x0000000000000020U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_ICNT2_MAX  (0x000000000000FFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_ICNT3_MASK (0xFFFF000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_ICNT3_SHIFT (0x0000000000000030U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD4_5_ICNT3_MAX  (0x000000000000FFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD6_7 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD6_7_DIM2_MASK  (0x00000000FFFFFFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD6_7_DIM2_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD6_7_DIM2_MAX   (0x00000000FFFFFFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD6_7_DIM3_MASK  (0xFFFFFFFF00000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD6_7_DIM3_SHIFT (0x0000000000000020U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD6_7_DIM3_MAX   (0x00000000FFFFFFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD8_9 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD8_9_DDIM1_MASK (0xFFFFFFFF00000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD8_9_DDIM1_SHIFT (0x0000000000000020U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD8_9_DDIM1_MAX  (0x00000000FFFFFFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD8_9_FMTFLAGS_MASK (0x00000000FFFFFFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD8_9_FMTFLAGS_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD8_9_FMTFLAGS_MAX (0x00000000FFFFFFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD10_11 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD10_11_DADDR_MASK (0x0000FFFFFFFFFFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD10_11_DADDR_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD10_11_DADDR_MAX (0x0000FFFFFFFFFFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD10_11_RSVD_MASK (0xFFFF000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD10_11_RSVD_SHIFT (0x0000000000000030U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD10_11_RSVD_MAX (0x000000000000FFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD12_13 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD12_13_DDIM2_MASK (0x00000000FFFFFFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD12_13_DDIM2_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD12_13_DDIM2_MAX (0x00000000FFFFFFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD12_13_DDIM3_MASK (0xFFFFFFFF00000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD12_13_DDIM3_SHIFT (0x0000000000000020U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD12_13_DDIM3_MAX (0x00000000FFFFFFFFU)

/* ATOMIC_SUBMIT_CURR_TR_WORD14_15 */

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT0_MASK (0x000000000000FFFFU)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT0_SHIFT (0x0000000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT0_MAX (0x000000000000FFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT1_MASK (0x00000000FFFF0000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT1_SHIFT (0x0000000000000010U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT1_MAX (0x000000000000FFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT2_MASK (0x0000FFFF00000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT2_SHIFT (0x0000000000000020U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT2_MAX (0x000000000000FFFFU)

#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT3_MASK (0xFFFF000000000000U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT3_SHIFT (0x0000000000000030U)
#define CSL_DRU_ATOMIC_SUBMIT_CURR_TR_WORD14_15_DICNT3_MAX (0x000000000000FFFFU)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t SUBMIT_WORD0_1;            /* The first TR submission word */
    volatile uint64_t SUBMIT_WORD2_3;            /* The second TR submission word */
    volatile uint64_t SUBMIT_WORD4_5;            /* The third TR submission word */
    volatile uint64_t SUBMIT_WORD6_7;            /* The fourth TR submission word */
    volatile uint64_t SUBMIT_WORD8_9;            /* The fifth TR submission word */
    volatile uint64_t SUBMIT_WORD10_11;          /* The sixth TR submission word */
    volatile uint64_t SUBMIT_WORD12_13;          /* The seventh TR submission word */
    volatile uint64_t SUBMIT_WORD14_15;          /* The eight TR submission word */
} CSL_DRU_CHCORERegs_CHCORE_CORE;


typedef struct {
    CSL_DRU_CHCORERegs_CHCORE_CORE CORE[3];
    volatile uint8_t  Resv_256[64];
} CSL_DRU_CHCORERegs_CHCORE;


typedef struct {
    CSL_DRU_CHCORERegs_CHCORE CHCORE[32];
} CSL_DRU_CHCORERegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_CORE_SUBMIT_WORD0_1(CHCORE, CORE)                (0x00000000U+((CHCORE)*0x100U)+((CORE)*0x40U))
#define CSL_DRU_CORE_SUBMIT_WORD2_3(CHCORE, CORE)                (0x00000008U+((CHCORE)*0x100U)+((CORE)*0x40U))
#define CSL_DRU_CORE_SUBMIT_WORD4_5(CHCORE, CORE)                (0x00000010U+((CHCORE)*0x100U)+((CORE)*0x40U))
#define CSL_DRU_CORE_SUBMIT_WORD6_7(CHCORE, CORE)                (0x00000018U+((CHCORE)*0x100U)+((CORE)*0x40U))
#define CSL_DRU_CORE_SUBMIT_WORD8_9(CHCORE, CORE)                (0x00000020U+((CHCORE)*0x100U)+((CORE)*0x40U))
#define CSL_DRU_CORE_SUBMIT_WORD10_11(CHCORE, CORE)              (0x00000028U+((CHCORE)*0x100U)+((CORE)*0x40U))
#define CSL_DRU_CORE_SUBMIT_WORD12_13(CHCORE, CORE)              (0x00000030U+((CHCORE)*0x100U)+((CORE)*0x40U))
#define CSL_DRU_CORE_SUBMIT_WORD14_15(CHCORE, CORE)              (0x00000038U+((CHCORE)*0x100U)+((CORE)*0x40U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* SUBMIT_WORD0_1 */

#define CSL_DRU_CORE_SUBMIT_WORD0_1_FLAGS_MASK                   (0x00000000FFFFFFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD0_1_FLAGS_SHIFT                  (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD0_1_FLAGS_MAX                    (0x00000000FFFFFFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD0_1_ICNT0_MASK                   (0x0000FFFF00000000U)
#define CSL_DRU_CORE_SUBMIT_WORD0_1_ICNT0_SHIFT                  (0x0000000000000020U)
#define CSL_DRU_CORE_SUBMIT_WORD0_1_ICNT0_MAX                    (0x000000000000FFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD0_1_ICNT1_MASK                   (0xFFFF000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD0_1_ICNT1_SHIFT                  (0x0000000000000030U)
#define CSL_DRU_CORE_SUBMIT_WORD0_1_ICNT1_MAX                    (0x000000000000FFFFU)

/* SUBMIT_WORD2_3 */

#define CSL_DRU_CORE_SUBMIT_WORD2_3_RSVD_MASK                    (0xFFFF000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD2_3_RSVD_SHIFT                   (0x0000000000000030U)
#define CSL_DRU_CORE_SUBMIT_WORD2_3_RSVD_MAX                     (0x000000000000FFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD2_3_SRC_ADDR_MASK                (0x0000FFFFFFFFFFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD2_3_SRC_ADDR_SHIFT               (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD2_3_SRC_ADDR_MAX                 (0x0000FFFFFFFFFFFFU)

/* SUBMIT_WORD4_5 */

#define CSL_DRU_CORE_SUBMIT_WORD4_5_DIM1_MASK                    (0x00000000FFFFFFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD4_5_DIM1_SHIFT                   (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD4_5_DIM1_MAX                     (0x00000000FFFFFFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD4_5_ICNT2_MASK                   (0x0000FFFF00000000U)
#define CSL_DRU_CORE_SUBMIT_WORD4_5_ICNT2_SHIFT                  (0x0000000000000020U)
#define CSL_DRU_CORE_SUBMIT_WORD4_5_ICNT2_MAX                    (0x000000000000FFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD4_5_ICNT3_MASK                   (0xFFFF000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD4_5_ICNT3_SHIFT                  (0x0000000000000030U)
#define CSL_DRU_CORE_SUBMIT_WORD4_5_ICNT3_MAX                    (0x000000000000FFFFU)

/* SUBMIT_WORD6_7 */

#define CSL_DRU_CORE_SUBMIT_WORD6_7_DIM2_MASK                    (0x00000000FFFFFFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD6_7_DIM2_SHIFT                   (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD6_7_DIM2_MAX                     (0x00000000FFFFFFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD6_7_DIM3_MASK                    (0xFFFFFFFF00000000U)
#define CSL_DRU_CORE_SUBMIT_WORD6_7_DIM3_SHIFT                   (0x0000000000000020U)
#define CSL_DRU_CORE_SUBMIT_WORD6_7_DIM3_MAX                     (0x00000000FFFFFFFFU)

/* SUBMIT_WORD8_9 */

#define CSL_DRU_CORE_SUBMIT_WORD8_9_DDIM1_MASK                   (0xFFFFFFFF00000000U)
#define CSL_DRU_CORE_SUBMIT_WORD8_9_DDIM1_SHIFT                  (0x0000000000000020U)
#define CSL_DRU_CORE_SUBMIT_WORD8_9_DDIM1_MAX                    (0x00000000FFFFFFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD8_9_FMTFLAGS_MASK                (0x00000000FFFFFFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD8_9_FMTFLAGS_SHIFT               (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD8_9_FMTFLAGS_MAX                 (0x00000000FFFFFFFFU)

/* SUBMIT_WORD10_11 */

#define CSL_DRU_CORE_SUBMIT_WORD10_11_DADDR_MASK                 (0x0000FFFFFFFFFFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD10_11_DADDR_SHIFT                (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD10_11_DADDR_MAX                  (0x0000FFFFFFFFFFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD10_11_RSVD_MASK                  (0xFFFF000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD10_11_RSVD_SHIFT                 (0x0000000000000030U)
#define CSL_DRU_CORE_SUBMIT_WORD10_11_RSVD_MAX                   (0x000000000000FFFFU)

/* SUBMIT_WORD12_13 */

#define CSL_DRU_CORE_SUBMIT_WORD12_13_DDIM2_MASK                 (0x00000000FFFFFFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD12_13_DDIM2_SHIFT                (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD12_13_DDIM2_MAX                  (0x00000000FFFFFFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD12_13_DDIM3_MASK                 (0xFFFFFFFF00000000U)
#define CSL_DRU_CORE_SUBMIT_WORD12_13_DDIM3_SHIFT                (0x0000000000000020U)
#define CSL_DRU_CORE_SUBMIT_WORD12_13_DDIM3_MAX                  (0x00000000FFFFFFFFU)

/* SUBMIT_WORD14_15 */

#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT0_MASK                (0x000000000000FFFFU)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT0_SHIFT               (0x0000000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT0_MAX                 (0x000000000000FFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT1_MASK                (0x00000000FFFF0000U)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT1_SHIFT               (0x0000000000000010U)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT1_MAX                 (0x000000000000FFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT2_MASK                (0x0000FFFF00000000U)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT2_SHIFT               (0x0000000000000020U)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT2_MAX                 (0x000000000000FFFFU)

#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT3_MASK                (0xFFFF000000000000U)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT3_SHIFT               (0x0000000000000030U)
#define CSL_DRU_CORE_SUBMIT_WORD14_15_DICNT3_MAX                 (0x000000000000FFFFU)

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint64_t CAUSE[2];                  /* Error Register cause for channels 0 to 15 */
    volatile uint8_t  Resv_256[240];
} CSL_DRU_CAUSE;


typedef struct {
    CSL_DRU_CAUSE CAUSE;
} CSL_DRU_CAUSERegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DRU_CAUSE(CAUSE)                                       (0x00000000U+((CAUSE)*0x8U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CAUSE */

#define CSL_DRU_CAUSE_R_ERR15_MASK                                 (0x8000000000000000U)
#define CSL_DRU_CAUSE_R_ERR15_SHIFT                                (0x000000000000003FU)
#define CSL_DRU_CAUSE_R_ERR15_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND15_MASK                                (0x4000000000000000U)
#define CSL_DRU_CAUSE_R_PEND15_SHIFT                               (0x000000000000003EU)
#define CSL_DRU_CAUSE_R_PEND15_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR15_MASK                                 (0x2000000000000000U)
#define CSL_DRU_CAUSE_T_ERR15_SHIFT                                (0x000000000000003DU)
#define CSL_DRU_CAUSE_T_ERR15_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND15_MASK                                (0x1000000000000000U)
#define CSL_DRU_CAUSE_T_PEND15_SHIFT                               (0x000000000000003CU)
#define CSL_DRU_CAUSE_T_PEND15_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR14_MASK                                 (0x0800000000000000U)
#define CSL_DRU_CAUSE_R_ERR14_SHIFT                                (0x000000000000003BU)
#define CSL_DRU_CAUSE_R_ERR14_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND14_MASK                                (0x0400000000000000U)
#define CSL_DRU_CAUSE_R_PEND14_SHIFT                               (0x000000000000003AU)
#define CSL_DRU_CAUSE_R_PEND14_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR14_MASK                                 (0x0200000000000000U)
#define CSL_DRU_CAUSE_T_ERR14_SHIFT                                (0x0000000000000039U)
#define CSL_DRU_CAUSE_T_ERR14_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND14_MASK                                (0x0100000000000000U)
#define CSL_DRU_CAUSE_T_PEND14_SHIFT                               (0x0000000000000038U)
#define CSL_DRU_CAUSE_T_PEND14_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR13_MASK                                 (0x0080000000000000U)
#define CSL_DRU_CAUSE_R_ERR13_SHIFT                                (0x0000000000000037U)
#define CSL_DRU_CAUSE_R_ERR13_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND13_MASK                                (0x0040000000000000U)
#define CSL_DRU_CAUSE_R_PEND13_SHIFT                               (0x0000000000000036U)
#define CSL_DRU_CAUSE_R_PEND13_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR13_MASK                                 (0x0020000000000000U)
#define CSL_DRU_CAUSE_T_ERR13_SHIFT                                (0x0000000000000035U)
#define CSL_DRU_CAUSE_T_ERR13_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND13_MASK                                (0x0010000000000000U)
#define CSL_DRU_CAUSE_T_PEND13_SHIFT                               (0x0000000000000034U)
#define CSL_DRU_CAUSE_T_PEND13_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR12_MASK                                 (0x0008000000000000U)
#define CSL_DRU_CAUSE_R_ERR12_SHIFT                                (0x0000000000000033U)
#define CSL_DRU_CAUSE_R_ERR12_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND12_MASK                                (0x0004000000000000U)
#define CSL_DRU_CAUSE_R_PEND12_SHIFT                               (0x0000000000000032U)
#define CSL_DRU_CAUSE_R_PEND12_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR12_MASK                                 (0x0002000000000000U)
#define CSL_DRU_CAUSE_T_ERR12_SHIFT                                (0x0000000000000031U)
#define CSL_DRU_CAUSE_T_ERR12_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND12_MASK                                (0x0001000000000000U)
#define CSL_DRU_CAUSE_T_PEND12_SHIFT                               (0x0000000000000030U)
#define CSL_DRU_CAUSE_T_PEND12_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR11_MASK                                 (0x0000800000000000U)
#define CSL_DRU_CAUSE_R_ERR11_SHIFT                                (0x000000000000002FU)
#define CSL_DRU_CAUSE_R_ERR11_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND11_MASK                                (0x0000400000000000U)
#define CSL_DRU_CAUSE_R_PEND11_SHIFT                               (0x000000000000002EU)
#define CSL_DRU_CAUSE_R_PEND11_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR11_MASK                                 (0x0000200000000000U)
#define CSL_DRU_CAUSE_T_ERR11_SHIFT                                (0x000000000000002DU)
#define CSL_DRU_CAUSE_T_ERR11_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND11_MASK                                (0x0000100000000000U)
#define CSL_DRU_CAUSE_T_PEND11_SHIFT                               (0x000000000000002CU)
#define CSL_DRU_CAUSE_T_PEND11_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR10_MASK                                 (0x0000080000000000U)
#define CSL_DRU_CAUSE_R_ERR10_SHIFT                                (0x000000000000002BU)
#define CSL_DRU_CAUSE_R_ERR10_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND10_MASK                                (0x0000040000000000U)
#define CSL_DRU_CAUSE_R_PEND10_SHIFT                               (0x000000000000002AU)
#define CSL_DRU_CAUSE_R_PEND10_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR10_MASK                                 (0x0000020000000000U)
#define CSL_DRU_CAUSE_T_ERR10_SHIFT                                (0x0000000000000029U)
#define CSL_DRU_CAUSE_T_ERR10_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND10_MASK                                (0x0000010000000000U)
#define CSL_DRU_CAUSE_T_PEND10_SHIFT                               (0x0000000000000028U)
#define CSL_DRU_CAUSE_T_PEND10_MAX                                 (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR9_MASK                                  (0x0000008000000000U)
#define CSL_DRU_CAUSE_R_ERR9_SHIFT                                 (0x0000000000000027U)
#define CSL_DRU_CAUSE_R_ERR9_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND9_MASK                                 (0x0000004000000000U)
#define CSL_DRU_CAUSE_R_PEND9_SHIFT                                (0x0000000000000026U)
#define CSL_DRU_CAUSE_R_PEND9_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR9_MASK                                  (0x0000002000000000U)
#define CSL_DRU_CAUSE_T_ERR9_SHIFT                                 (0x0000000000000025U)
#define CSL_DRU_CAUSE_T_ERR9_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND9_MASK                                 (0x0000001000000000U)
#define CSL_DRU_CAUSE_T_PEND9_SHIFT                                (0x0000000000000024U)
#define CSL_DRU_CAUSE_T_PEND9_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR8_MASK                                  (0x0000000800000000U)
#define CSL_DRU_CAUSE_R_ERR8_SHIFT                                 (0x0000000000000023U)
#define CSL_DRU_CAUSE_R_ERR8_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND8_MASK                                 (0x0000000400000000U)
#define CSL_DRU_CAUSE_R_PEND8_SHIFT                                (0x0000000000000022U)
#define CSL_DRU_CAUSE_R_PEND8_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR8_MASK                                  (0x0000000200000000U)
#define CSL_DRU_CAUSE_T_ERR8_SHIFT                                 (0x0000000000000021U)
#define CSL_DRU_CAUSE_T_ERR8_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND8_MASK                                 (0x0000000100000000U)
#define CSL_DRU_CAUSE_T_PEND8_SHIFT                                (0x0000000000000020U)
#define CSL_DRU_CAUSE_T_PEND8_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR7_MASK                                  (0x0000000080000000U)
#define CSL_DRU_CAUSE_R_ERR7_SHIFT                                 (0x000000000000001FU)
#define CSL_DRU_CAUSE_R_ERR7_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND7_MASK                                 (0x0000000040000000U)
#define CSL_DRU_CAUSE_R_PEND7_SHIFT                                (0x000000000000001EU)
#define CSL_DRU_CAUSE_R_PEND7_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR7_MASK                                  (0x0000000020000000U)
#define CSL_DRU_CAUSE_T_ERR7_SHIFT                                 (0x000000000000001DU)
#define CSL_DRU_CAUSE_T_ERR7_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND7_MASK                                 (0x0000000010000000U)
#define CSL_DRU_CAUSE_T_PEND7_SHIFT                                (0x000000000000001CU)
#define CSL_DRU_CAUSE_T_PEND7_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR6_MASK                                  (0x0000000008000000U)
#define CSL_DRU_CAUSE_R_ERR6_SHIFT                                 (0x000000000000001BU)
#define CSL_DRU_CAUSE_R_ERR6_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND6_MASK                                 (0x0000000004000000U)
#define CSL_DRU_CAUSE_R_PEND6_SHIFT                                (0x000000000000001AU)
#define CSL_DRU_CAUSE_R_PEND6_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR6_MASK                                  (0x0000000002000000U)
#define CSL_DRU_CAUSE_T_ERR6_SHIFT                                 (0x0000000000000019U)
#define CSL_DRU_CAUSE_T_ERR6_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND6_MASK                                 (0x0000000001000000U)
#define CSL_DRU_CAUSE_T_PEND6_SHIFT                                (0x0000000000000018U)
#define CSL_DRU_CAUSE_T_PEND6_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR5_MASK                                  (0x0000000000800000U)
#define CSL_DRU_CAUSE_R_ERR5_SHIFT                                 (0x0000000000000017U)
#define CSL_DRU_CAUSE_R_ERR5_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND5_MASK                                 (0x0000000000400000U)
#define CSL_DRU_CAUSE_R_PEND5_SHIFT                                (0x0000000000000016U)
#define CSL_DRU_CAUSE_R_PEND5_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR5_MASK                                  (0x0000000000200000U)
#define CSL_DRU_CAUSE_T_ERR5_SHIFT                                 (0x0000000000000015U)
#define CSL_DRU_CAUSE_T_ERR5_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND5_MASK                                 (0x0000000000100000U)
#define CSL_DRU_CAUSE_T_PEND5_SHIFT                                (0x0000000000000014U)
#define CSL_DRU_CAUSE_T_PEND5_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR4_MASK                                  (0x0000000000080000U)
#define CSL_DRU_CAUSE_R_ERR4_SHIFT                                 (0x0000000000000013U)
#define CSL_DRU_CAUSE_R_ERR4_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND4_MASK                                 (0x0000000000040000U)
#define CSL_DRU_CAUSE_R_PEND4_SHIFT                                (0x0000000000000012U)
#define CSL_DRU_CAUSE_R_PEND4_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR4_MASK                                  (0x0000000000020000U)
#define CSL_DRU_CAUSE_T_ERR4_SHIFT                                 (0x0000000000000011U)
#define CSL_DRU_CAUSE_T_ERR4_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND4_MASK                                 (0x0000000000010000U)
#define CSL_DRU_CAUSE_T_PEND4_SHIFT                                (0x0000000000000010U)
#define CSL_DRU_CAUSE_T_PEND4_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR3_MASK                                  (0x0000000000008000U)
#define CSL_DRU_CAUSE_R_ERR3_SHIFT                                 (0x000000000000000FU)
#define CSL_DRU_CAUSE_R_ERR3_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND3_MASK                                 (0x0000000000004000U)
#define CSL_DRU_CAUSE_R_PEND3_SHIFT                                (0x000000000000000EU)
#define CSL_DRU_CAUSE_R_PEND3_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR3_MASK                                  (0x0000000000002000U)
#define CSL_DRU_CAUSE_T_ERR3_SHIFT                                 (0x000000000000000DU)
#define CSL_DRU_CAUSE_T_ERR3_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND3_MASK                                 (0x0000000000001000U)
#define CSL_DRU_CAUSE_T_PEND3_SHIFT                                (0x000000000000000CU)
#define CSL_DRU_CAUSE_T_PEND3_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR2_MASK                                  (0x0000000000000800U)
#define CSL_DRU_CAUSE_R_ERR2_SHIFT                                 (0x000000000000000BU)
#define CSL_DRU_CAUSE_R_ERR2_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND2_MASK                                 (0x0000000000000400U)
#define CSL_DRU_CAUSE_R_PEND2_SHIFT                                (0x000000000000000AU)
#define CSL_DRU_CAUSE_R_PEND2_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR2_MASK                                  (0x0000000000000200U)
#define CSL_DRU_CAUSE_T_ERR2_SHIFT                                 (0x0000000000000009U)
#define CSL_DRU_CAUSE_T_ERR2_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND2_MASK                                 (0x0000000000000100U)
#define CSL_DRU_CAUSE_T_PEND2_SHIFT                                (0x0000000000000008U)
#define CSL_DRU_CAUSE_T_PEND2_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR1_MASK                                  (0x0000000000000080U)
#define CSL_DRU_CAUSE_R_ERR1_SHIFT                                 (0x0000000000000007U)
#define CSL_DRU_CAUSE_R_ERR1_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND1_MASK                                 (0x0000000000000040U)
#define CSL_DRU_CAUSE_R_PEND1_SHIFT                                (0x0000000000000006U)
#define CSL_DRU_CAUSE_R_PEND1_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR1_MASK                                  (0x0000000000000020U)
#define CSL_DRU_CAUSE_T_ERR1_SHIFT                                 (0x0000000000000005U)
#define CSL_DRU_CAUSE_T_ERR1_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND1_MASK                                 (0x0000000000000010U)
#define CSL_DRU_CAUSE_T_PEND1_SHIFT                                (0x0000000000000004U)
#define CSL_DRU_CAUSE_T_PEND1_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_ERR0_MASK                                  (0x0000000000000008U)
#define CSL_DRU_CAUSE_R_ERR0_SHIFT                                 (0x0000000000000003U)
#define CSL_DRU_CAUSE_R_ERR0_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_R_PEND0_MASK                                 (0x0000000000000004U)
#define CSL_DRU_CAUSE_R_PEND0_SHIFT                                (0x0000000000000002U)
#define CSL_DRU_CAUSE_R_PEND0_MAX                                  (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_ERR0_MASK                                  (0x0000000000000002U)
#define CSL_DRU_CAUSE_T_ERR0_SHIFT                                 (0x0000000000000001U)
#define CSL_DRU_CAUSE_T_ERR0_MAX                                   (0x0000000000000001U)

#define CSL_DRU_CAUSE_T_PEND0_MASK                                 (0x0000000000000001U)
#define CSL_DRU_CAUSE_T_PEND0_SHIFT                                (0x0000000000000000U)
#define CSL_DRU_CAUSE_T_PEND0_MAX                                  (0x0000000000000001U)

/**************************************************************************
 *  DRU TOP LEVEL Struct (hand created DLB)
 *************************************************************************/

typedef struct {
    CSL_DRU_Regs                               DRURegs;
    uint8_t                                    RSVD01[16368];
    CSL_DRU_SETRegs                            DRUEvtSet;
    uint8_t                                    RSVD02[16128];
    CSL_DRU_QUEUERegs                          DRUQueues;
    uint8_t                                    RSVD03[8072];
    CSL_DRU_MMURegs                            DRUMMURegs;
    uint8_t                                    RSVD04[1848];
    CSL_DRU_UTLBRegs                           DRUUTLBRegs;
    uint8_t                                    RSVD05[200192];
    CSL_DRU_CHNRTRegs_CHNRT                    CHNRT[512];
    CSL_DRU_CHRTRegs_CHRT                      CHRT[512];
    CSL_DRU_CHATOMIC_DEBUGRegs_CHATOMIC_DEBUG  CHATOMIC[512];
    CSL_DRU_CHCORERegs_CHCORE                  CHCORE[512];
    CSL_DRU_CHCORERegs_CHCORE                  CHCOREHI[512];
    CSL_DRU_CAUSE                              CAUSE;
} CSL_DRU_t;
#ifdef __cplusplus
}
#endif
#endif
