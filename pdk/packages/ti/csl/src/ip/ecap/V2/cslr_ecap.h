/********************************************************************
 * Copyright (C) 2017 Texas Instruments Incorporated.
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
 *  Name        : cslr_ecap.h
*/
#ifndef CSLR_ECAP_H_
#define CSLR_ECAP_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Module Base Offset Values
**************************************************************************/

#define CSL_ECAP_REGS_BASE                                             (0x00000000U)


/**************************************************************************
* Hardware Region  : ECAP Control and Status Registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t TSCNT;                     /* Time Stamp Counter Register */
    volatile uint32_t CNTPHS;                    /* Counter Phase Control Register */
    volatile uint32_t CAP1;                      /* Capture-1 Register */
    volatile uint32_t CAP2;                      /* Capture-2 Register */
    volatile uint32_t CAP3;                      /* Capture-3 Register */
    volatile uint32_t CAP4;                      /* Capture-4 Register */
    volatile uint8_t  Resv_40[16];
    volatile uint32_t ECCTL;                     /* ECAP Control Register */
    volatile uint32_t ECINT_EN_FLG;              /* ECAP Interrupt Enable and Flag Register */
    volatile uint32_t ECINT_CLR_FRC;             /* ECAP Interrupt Clear and Forcing Register */
    volatile uint8_t  Resv_92[40];
    volatile uint32_t PID;                       /* Peripheral ID Register */
} CSL_EcapRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_ECAP_TSCNT                                                 (0x00000000U)
#define CSL_ECAP_CNTPHS                                                (0x00000004U)
#define CSL_ECAP_CAP1                                                  (0x00000008U)
#define CSL_ECAP_CAP2                                                  (0x0000000CU)
#define CSL_ECAP_CAP3                                                  (0x00000010U)
#define CSL_ECAP_CAP4                                                  (0x00000014U)
#define CSL_ECAP_ECCTL                                                 (0x00000028U)
#define CSL_ECAP_ECINT_EN_FLG                                          (0x0000002CU)
#define CSL_ECAP_ECINT_CLR_FRC                                         (0x00000030U)
#define CSL_ECAP_PID                                                   (0x0000005CU)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* TSCNT */

#define CSL_ECAP_TSCNT_TSCNT_MASK                                      (0xFFFFFFFFU)
#define CSL_ECAP_TSCNT_TSCNT_SHIFT                                     (0x00000000U)
#define CSL_ECAP_TSCNT_TSCNT_MAX                                       (0xFFFFFFFFU)

/* CNTPHS */

#define CSL_ECAP_CNTPHS_CNTPHS_MASK                                    (0xFFFFFFFFU)
#define CSL_ECAP_CNTPHS_CNTPHS_SHIFT                                   (0x00000000U)
#define CSL_ECAP_CNTPHS_CNTPHS_MAX                                     (0xFFFFFFFFU)

/* CAP1 */

#define CSL_ECAP_CAP1_CAP1_MASK                                        (0xFFFFFFFFU)
#define CSL_ECAP_CAP1_CAP1_SHIFT                                       (0x00000000U)
#define CSL_ECAP_CAP1_CAP1_MAX                                         (0xFFFFFFFFU)

/* CAP2 */

#define CSL_ECAP_CAP2_CAP2_MASK                                        (0xFFFFFFFFU)
#define CSL_ECAP_CAP2_CAP2_SHIFT                                       (0x00000000U)
#define CSL_ECAP_CAP2_CAP2_MAX                                         (0xFFFFFFFFU)

/* CAP3 */

#define CSL_ECAP_CAP3_CAP3_MASK                                        (0xFFFFFFFFU)
#define CSL_ECAP_CAP3_CAP3_SHIFT                                       (0x00000000U)
#define CSL_ECAP_CAP3_CAP3_MAX                                         (0xFFFFFFFFU)

/* CAP4 */

#define CSL_ECAP_CAP4_CAP4_MASK                                        (0xFFFFFFFFU)
#define CSL_ECAP_CAP4_CAP4_SHIFT                                       (0x00000000U)
#define CSL_ECAP_CAP4_CAP4_MAX                                         (0xFFFFFFFFU)

/* ECCTL */

#define CSL_ECAP_ECCTL_CAP1POL_MASK                                    (0x00000001U)
#define CSL_ECAP_ECCTL_CAP1POL_SHIFT                                   (0x00000000U)
#define CSL_ECAP_ECCTL_CAP1POL_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CTRRST1_MASK                                    (0x00000002U)
#define CSL_ECAP_ECCTL_CTRRST1_SHIFT                                   (0x00000001U)
#define CSL_ECAP_ECCTL_CTRRST1_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CAP2POL_MASK                                    (0x00000004U)
#define CSL_ECAP_ECCTL_CAP2POL_SHIFT                                   (0x00000002U)
#define CSL_ECAP_ECCTL_CAP2POL_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CTRRST2_MASK                                    (0x00000008U)
#define CSL_ECAP_ECCTL_CTRRST2_SHIFT                                   (0x00000003U)
#define CSL_ECAP_ECCTL_CTRRST2_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CAP3POL_MASK                                    (0x00000010U)
#define CSL_ECAP_ECCTL_CAP3POL_SHIFT                                   (0x00000004U)
#define CSL_ECAP_ECCTL_CAP3POL_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CTRRST3_MASK                                    (0x00000020U)
#define CSL_ECAP_ECCTL_CTRRST3_SHIFT                                   (0x00000005U)
#define CSL_ECAP_ECCTL_CTRRST3_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CAP4POL_MASK                                    (0x00000040U)
#define CSL_ECAP_ECCTL_CAP4POL_SHIFT                                   (0x00000006U)
#define CSL_ECAP_ECCTL_CAP4POL_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CTRRST4_MASK                                    (0x00000080U)
#define CSL_ECAP_ECCTL_CTRRST4_SHIFT                                   (0x00000007U)
#define CSL_ECAP_ECCTL_CTRRST4_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_CAPLDEN_MASK                                    (0x00000100U)
#define CSL_ECAP_ECCTL_CAPLDEN_SHIFT                                   (0x00000008U)
#define CSL_ECAP_ECCTL_CAPLDEN_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_EVTFLTPS_MASK                                   (0x00003E00U)
#define CSL_ECAP_ECCTL_EVTFLTPS_SHIFT                                  (0x00000009U)
#define CSL_ECAP_ECCTL_EVTFLTPS_MAX                                    (0x0000001FU)

#define CSL_ECAP_ECCTL_FREE_SOFT_MASK                                  (0x0000C000U)
#define CSL_ECAP_ECCTL_FREE_SOFT_SHIFT                                 (0x0000000EU)
#define CSL_ECAP_ECCTL_FREE_SOFT_MAX                                   (0x00000003U)

#define CSL_ECAP_ECCTL_CONT_ONESHT_MASK                                (0x00010000U)
#define CSL_ECAP_ECCTL_CONT_ONESHT_SHIFT                               (0x00000010U)
#define CSL_ECAP_ECCTL_CONT_ONESHT_MAX                                 (0x00000001U)

#define CSL_ECAP_ECCTL_STOPVALUE_MASK                                  (0x00060000U)
#define CSL_ECAP_ECCTL_STOPVALUE_SHIFT                                 (0x00000011U)
#define CSL_ECAP_ECCTL_STOPVALUE_MAX                                   (0x00000003U)

#define CSL_ECAP_ECCTL_REARM_RESET_MASK                                (0x00080000U)
#define CSL_ECAP_ECCTL_REARM_RESET_SHIFT                               (0x00000013U)
#define CSL_ECAP_ECCTL_REARM_RESET_MAX                                 (0x00000001U)

#define CSL_ECAP_ECCTL_TSCNTSTP_MASK                                   (0x00100000U)
#define CSL_ECAP_ECCTL_TSCNTSTP_SHIFT                                  (0x00000014U)
#define CSL_ECAP_ECCTL_TSCNTSTP_MAX                                    (0x00000001U)

#define CSL_ECAP_ECCTL_SYNCI_EN_MASK                                   (0x00200000U)
#define CSL_ECAP_ECCTL_SYNCI_EN_SHIFT                                  (0x00000015U)
#define CSL_ECAP_ECCTL_SYNCI_EN_MAX                                    (0x00000001U)

#define CSL_ECAP_ECCTL_SYNCO_SEL_MASK                                  (0x00C00000U)
#define CSL_ECAP_ECCTL_SYNCO_SEL_SHIFT                                 (0x00000016U)
#define CSL_ECAP_ECCTL_SYNCO_SEL_MAX                                   (0x00000003U)

#define CSL_ECAP_ECCTL_SWSYNC_MASK                                     (0x01000000U)
#define CSL_ECAP_ECCTL_SWSYNC_SHIFT                                    (0x00000018U)
#define CSL_ECAP_ECCTL_SWSYNC_MAX                                      (0x00000001U)

#define CSL_ECAP_ECCTL_CAP_APWM_MASK                                   (0x02000000U)
#define CSL_ECAP_ECCTL_CAP_APWM_SHIFT                                  (0x00000019U)
#define CSL_ECAP_ECCTL_CAP_APWM_MAX                                    (0x00000001U)

#define CSL_ECAP_ECCTL_APWMPOL_MASK                                    (0x04000000U)
#define CSL_ECAP_ECCTL_APWMPOL_SHIFT                                   (0x0000001AU)
#define CSL_ECAP_ECCTL_APWMPOL_MAX                                     (0x00000001U)

#define CSL_ECAP_ECCTL_FILTER_MASK                                     (0xF8000000U)
#define CSL_ECAP_ECCTL_FILTER_SHIFT                                    (0x0000001BU)
#define CSL_ECAP_ECCTL_FILTER_MAX                                      (0x0000001FU)

/* ECINT_EN_FLG */

#define CSL_ECAP_ECINT_EN_FLG_CEVT1_EN_MASK                            (0x00000002U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT1_EN_SHIFT                           (0x00000001U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT1_EN_MAX                             (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CEVT2_EN_MASK                            (0x00000004U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT2_EN_SHIFT                           (0x00000002U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT2_EN_MAX                             (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CEVT3_EN_MASK                            (0x00000008U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT3_EN_SHIFT                           (0x00000003U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT3_EN_MAX                             (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CEVT4_EN_MASK                            (0x00000010U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT4_EN_SHIFT                           (0x00000004U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT4_EN_MAX                             (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CNTOVF_EN_MASK                           (0x00000020U)
#define CSL_ECAP_ECINT_EN_FLG_CNTOVF_EN_SHIFT                          (0x00000005U)
#define CSL_ECAP_ECINT_EN_FLG_CNTOVF_EN_MAX                            (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_PRDEQ_EN_MASK                            (0x00000040U)
#define CSL_ECAP_ECINT_EN_FLG_PRDEQ_EN_SHIFT                           (0x00000006U)
#define CSL_ECAP_ECINT_EN_FLG_PRDEQ_EN_MAX                             (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CMPEQ_EN_MASK                            (0x00000080U)
#define CSL_ECAP_ECINT_EN_FLG_CMPEQ_EN_SHIFT                           (0x00000007U)
#define CSL_ECAP_ECINT_EN_FLG_CMPEQ_EN_MAX                             (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_INT_FLG_MASK                             (0x00010000U)
#define CSL_ECAP_ECINT_EN_FLG_INT_FLG_SHIFT                            (0x00000010U)
#define CSL_ECAP_ECINT_EN_FLG_INT_FLG_MAX                              (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CEVT1_FLG_MASK                           (0x00020000U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT1_FLG_SHIFT                          (0x00000011U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT1_FLG_MAX                            (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CEVT2_FLG_MASK                           (0x00040000U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT2_FLG_SHIFT                          (0x00000012U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT2_FLG_MAX                            (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CEVT3_FLG_MASK                           (0x00080000U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT3_FLG_SHIFT                          (0x00000013U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT3_FLG_MAX                            (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CEVT4_FLG_MASK                           (0x00100000U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT4_FLG_SHIFT                          (0x00000014U)
#define CSL_ECAP_ECINT_EN_FLG_CEVT4_FLG_MAX                            (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CNTOVF_FLG_MASK                          (0x00200000U)
#define CSL_ECAP_ECINT_EN_FLG_CNTOVF_FLG_SHIFT                         (0x00000015U)
#define CSL_ECAP_ECINT_EN_FLG_CNTOVF_FLG_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_PRDEQ_FLG_MASK                           (0x00400000U)
#define CSL_ECAP_ECINT_EN_FLG_PRDEQ_FLG_SHIFT                          (0x00000016U)
#define CSL_ECAP_ECINT_EN_FLG_PRDEQ_FLG_MAX                            (0x00000001U)

#define CSL_ECAP_ECINT_EN_FLG_CMPEQ_FLG_MASK                           (0x00800000U)
#define CSL_ECAP_ECINT_EN_FLG_CMPEQ_FLG_SHIFT                          (0x00000017U)
#define CSL_ECAP_ECINT_EN_FLG_CMPEQ_FLG_MAX                            (0x00000001U)

/* ECINT_CLR_FRC */

#define CSL_ECAP_ECINT_CLR_FRC_INT_CLR_MASK                            (0x00000001U)
#define CSL_ECAP_ECINT_CLR_FRC_INT_CLR_SHIFT                           (0x00000000U)
#define CSL_ECAP_ECINT_CLR_FRC_INT_CLR_MAX                             (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT1_CLR_MASK                          (0x00000002U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT1_CLR_SHIFT                         (0x00000001U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT1_CLR_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT2_CLR_MASK                          (0x00000004U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT2_CLR_SHIFT                         (0x00000002U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT2_CLR_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT3_CLR_MASK                          (0x00000008U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT3_CLR_SHIFT                         (0x00000003U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT3_CLR_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT4_CLR_MASK                          (0x00000010U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT4_CLR_SHIFT                         (0x00000004U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT4_CLR_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CNTOVF_CLR_MASK                         (0x00000020U)
#define CSL_ECAP_ECINT_CLR_FRC_CNTOVF_CLR_SHIFT                        (0x00000005U)
#define CSL_ECAP_ECINT_CLR_FRC_CNTOVF_CLR_MAX                          (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_PRDEQ_CLR_MASK                          (0x00000040U)
#define CSL_ECAP_ECINT_CLR_FRC_PRDEQ_CLR_SHIFT                         (0x00000006U)
#define CSL_ECAP_ECINT_CLR_FRC_PRDEQ_CLR_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CMPEQ_CLR_MASK                          (0x00000080U)
#define CSL_ECAP_ECINT_CLR_FRC_CMPEQ_CLR_SHIFT                         (0x00000007U)
#define CSL_ECAP_ECINT_CLR_FRC_CMPEQ_CLR_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT1_FRC_MASK                          (0x00020000U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT1_FRC_SHIFT                         (0x00000011U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT1_FRC_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT2_FRC_MASK                          (0x00040000U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT2_FRC_SHIFT                         (0x00000012U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT2_FRC_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT3_FRC_MASK                          (0x00080000U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT3_FRC_SHIFT                         (0x00000013U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT3_FRC_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CEVT4_FRC_MASK                          (0x00100000U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT4_FRC_SHIFT                         (0x00000014U)
#define CSL_ECAP_ECINT_CLR_FRC_CEVT4_FRC_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CNTOVF_FRC_MASK                         (0x00200000U)
#define CSL_ECAP_ECINT_CLR_FRC_CNTOVF_FRC_SHIFT                        (0x00000015U)
#define CSL_ECAP_ECINT_CLR_FRC_CNTOVF_FRC_MAX                          (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_PRDEQ_FRC_MASK                          (0x00400000U)
#define CSL_ECAP_ECINT_CLR_FRC_PRDEQ_FRC_SHIFT                         (0x00000016U)
#define CSL_ECAP_ECINT_CLR_FRC_PRDEQ_FRC_MAX                           (0x00000001U)

#define CSL_ECAP_ECINT_CLR_FRC_CMPEQ_FRC_MASK                          (0x00800000U)
#define CSL_ECAP_ECINT_CLR_FRC_CMPEQ_FRC_SHIFT                         (0x00000017U)
#define CSL_ECAP_ECINT_CLR_FRC_CMPEQ_FRC_MAX                           (0x00000001U)

/* PID */

#define CSL_ECAP_PID_MINOR_MASK                                        (0x0000003FU)
#define CSL_ECAP_PID_MINOR_SHIFT                                       (0x00000000U)
#define CSL_ECAP_PID_MINOR_MAX                                         (0x0000003FU)

#define CSL_ECAP_PID_CUSTOM_MASK                                       (0x000000C0U)
#define CSL_ECAP_PID_CUSTOM_SHIFT                                      (0x00000006U)
#define CSL_ECAP_PID_CUSTOM_MAX                                        (0x00000003U)

#define CSL_ECAP_PID_MAJOR_MASK                                        (0x00000700U)
#define CSL_ECAP_PID_MAJOR_SHIFT                                       (0x00000008U)
#define CSL_ECAP_PID_MAJOR_MAX                                         (0x00000007U)

#define CSL_ECAP_PID_RTL_MASK                                          (0x0000F800U)
#define CSL_ECAP_PID_RTL_SHIFT                                         (0x0000000BU)
#define CSL_ECAP_PID_RTL_MAX                                           (0x0000001FU)

#define CSL_ECAP_PID_FUNCTION_MASK                                     (0x0FFF0000U)
#define CSL_ECAP_PID_FUNCTION_SHIFT                                    (0x00000010U)
#define CSL_ECAP_PID_FUNCTION_MAX                                      (0x00000FFFU)

#define CSL_ECAP_PID_RESERVED_MASK                                     (0x30000000U)
#define CSL_ECAP_PID_RESERVED_SHIFT                                    (0x0000001CU)
#define CSL_ECAP_PID_RESERVED_MAX                                      (0x00000003U)

#define CSL_ECAP_PID_SCHEME_MASK                                       (0xC0000000U)
#define CSL_ECAP_PID_SCHEME_SHIFT                                      (0x0000001EU)
#define CSL_ECAP_PID_SCHEME_MAX                                        (0x00000003U)

#ifdef __cplusplus
}
#endif
#endif
