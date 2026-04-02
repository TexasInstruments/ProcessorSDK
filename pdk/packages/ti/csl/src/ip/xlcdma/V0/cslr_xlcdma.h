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
 *  Name        : cslr_xlcdma.h
 *  VPVERSION   : 3.0.368 - 2024.04.23.13.05.09
 *  VPREV       : 1.0.10
*/
#ifndef CSLR_XLCDMA_H_
#define CSLR_XLCDMA_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  : XLCDMA Control / Status Registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t CNTCTL;                    /* Debug Counter Control */
    volatile uint32_t EVTCTL;                    /* Debug Event Control */
    volatile uint32_t CNT;                       /* Debug Counter */
    volatile uint8_t  Resv_16[4];
} CSL_xlcdma_gcfg_v2Regs_dbgcnt;


typedef struct {
    volatile uint32_t REVISION;                  /* Revision Register */
    volatile uint8_t  Resv_32[28];
    volatile uint32_t CAP0;                      /* Capabilities Register 0 */
    volatile uint32_t CAP1;                      /* Capabilities Register 1 */
    volatile uint32_t CAP2;                      /* Capabilities Register 2 */
    volatile uint32_t CAP3;                      /* Capabilities Register 3 */
    volatile uint32_t CAP4;                      /* Capabilities Register 4 */
    volatile uint32_t CAP5;                      /* Capabilities Register 5 */
    volatile uint8_t  Resv_4096[4040];
    volatile uint32_t PERF_CTRL;                 /* Performance Control Register */
    volatile uint32_t EMU_CTRL;                  /* Emulation Control Register */
    volatile uint8_t  Resv_4160[56];
    volatile uint32_t DBGADDR;                   /* Debug Address Register */
    volatile uint32_t DBGDATA;                   /* Debug Data Register */
    volatile uint8_t  Resv_4352[184];
    CSL_xlcdma_gcfg_v2Regs_dbgcnt DBGCNT[4];
    volatile uint8_t  Resv_8192[3776];
    volatile uint32_t PM0;                       /* Power Management Register 0 */
    volatile uint32_t PM1;                       /* Power Management Register 1 */
} CSL_xlcdma_gcfg_v2Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_XLCDMA_GCFG_V2_REVISION                                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0                                                (0x00000020U)
#define CSL_XLCDMA_GCFG_V2_CAP1                                                (0x00000024U)
#define CSL_XLCDMA_GCFG_V2_CAP2                                                (0x00000028U)
#define CSL_XLCDMA_GCFG_V2_CAP3                                                (0x0000002CU)
#define CSL_XLCDMA_GCFG_V2_CAP4                                                (0x00000030U)
#define CSL_XLCDMA_GCFG_V2_CAP5                                                (0x00000034U)
#define CSL_XLCDMA_GCFG_V2_PERF_CTRL                                           (0x00001000U)
#define CSL_XLCDMA_GCFG_V2_EMU_CTRL                                            (0x00001004U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR                                             (0x00001040U)
#define CSL_XLCDMA_GCFG_V2_DBGDATA                                             (0x00001044U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL(DBGCNT)                               (0x00001100U+((DBGCNT)*0x10U))
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL(DBGCNT)                               (0x00001104U+((DBGCNT)*0x10U))
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNT(DBGCNT)                                  (0x00001108U+((DBGCNT)*0x10U))
#define CSL_XLCDMA_GCFG_V2_PM0                                                 (0x00002000U)
#define CSL_XLCDMA_GCFG_V2_PM1                                                 (0x00002004U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CNTCTL */

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_TYPE_MASK                             (0xC0000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_TYPE_SHIFT                            (0x0000001EU)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_TYPE_RESETVAL                         (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_TYPE_MAX                              (0x00000003U)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_TYPE_VAL_INCR                         (0x0U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_TYPE_VAL_ONESHOT                      (0x1U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_TYPE_VAL_CONT                         (0x2U)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_MASK                          (0x1E000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_SHIFT                         (0x00000019U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_MAX                           (0x0000000FU)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_SCHED                     (0x0U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_RD_CMD                    (0x1U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_WR_CMD                    (0x2U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_RD_RESP                   (0x3U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_WR_RESP                   (0x4U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_RD_RESP_EOP               (0x5U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_WR_RESP_EOP               (0x6U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPEVT_VAL_CNT_EVT                   (0x8U)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPFLOW_MASK                         (0x01FF0000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPFLOW_SHIFT                        (0x00000010U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPFLOW_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STOPFLOW_MAX                          (0x000001FFU)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_MASK                         (0x00001E00U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_SHIFT                        (0x00000009U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_MAX                          (0x0000000FU)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_SCHED                    (0x0U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_RD_CMD                   (0x1U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_WR_CMD                   (0x2U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_RD_RESP                  (0x3U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_WR_RESP                  (0x4U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_RD_RESP_EOP              (0x5U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_WR_RESP_EOP              (0x6U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTEVT_VAL_CNT_EVT                  (0x8U)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTFLOW_MASK                        (0x000001FFU)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTFLOW_SHIFT                       (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTFLOW_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_STARTFLOW_MAX                         (0x000001FFU)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNTCTL_RESETVAL                              (0x00000000U)

/* EVTCTL */

#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_EVTTYPE_MASK                          (0xC0000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_EVTTYPE_SHIFT                         (0x0000001EU)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_EVTTYPE_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_EVTTYPE_MAX                           (0x00000003U)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_EVTTYPE_VAL_NO_EVT                    (0x0U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_EVTTYPE_VAL_EXACT                     (0x1U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_EVTTYPE_VAL_AND_MASK                  (0x2U)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_CNTMATCH_MASK                         (0x0000FFFFU)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_CNTMATCH_SHIFT                        (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_CNTMATCH_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_CNTMATCH_MAX                          (0x0000FFFFU)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_EVTCTL_RESETVAL                              (0x00000000U)

/* CNT */

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNT_CNT_MASK                                 (0x0000FFFFU)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNT_CNT_SHIFT                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNT_CNT_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNT_CNT_MAX                                  (0x0000FFFFU)

#define CSL_XLCDMA_GCFG_V2_DBGCNT_CNT_RESETVAL                                 (0x00000000U)

/* REVISION */

#define CSL_XLCDMA_GCFG_V2_REVISION_MODID_MASK                                 (0xFFFF0000U)
#define CSL_XLCDMA_GCFG_V2_REVISION_MODID_SHIFT                                (0x00000010U)
#define CSL_XLCDMA_GCFG_V2_REVISION_MODID_RESETVAL                             (0x0000662AU)
#define CSL_XLCDMA_GCFG_V2_REVISION_MODID_MAX                                  (0x0000FFFFU)

#define CSL_XLCDMA_GCFG_V2_REVISION_REVRTL_MASK                                (0x0000F800U)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVRTL_SHIFT                               (0x0000000BU)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVRTL_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVRTL_MAX                                 (0x0000001FU)

#define CSL_XLCDMA_GCFG_V2_REVISION_REVMAJ_MASK                                (0x00000700U)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVMAJ_SHIFT                               (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVMAJ_RESETVAL                            (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVMAJ_MAX                                 (0x00000007U)

#define CSL_XLCDMA_GCFG_V2_REVISION_CUSTOM_MASK                                (0x000000C0U)
#define CSL_XLCDMA_GCFG_V2_REVISION_CUSTOM_SHIFT                               (0x00000006U)
#define CSL_XLCDMA_GCFG_V2_REVISION_CUSTOM_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_REVISION_CUSTOM_MAX                                 (0x00000003U)

#define CSL_XLCDMA_GCFG_V2_REVISION_REVMIN_MASK                                (0x0000003FU)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVMIN_SHIFT                               (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVMIN_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_REVISION_REVMIN_MAX                                 (0x0000003FU)

#define CSL_XLCDMA_GCFG_V2_REVISION_RESETVAL                                   (0x662A0200U)

/* CAP0 */

#define CSL_XLCDMA_GCFG_V2_CAP0_GLOBAL_TRIG_MASK                               (0x00080000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_GLOBAL_TRIG_SHIFT                              (0x00000013U)
#define CSL_XLCDMA_GCFG_V2_CAP0_GLOBAL_TRIG_RESETVAL                           (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_GLOBAL_TRIG_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_LOCAL_TRIG_MASK                                (0x00040000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_LOCAL_TRIG_SHIFT                               (0x00000012U)
#define CSL_XLCDMA_GCFG_V2_CAP0_LOCAL_TRIG_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_LOCAL_TRIG_MAX                                 (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_EOL_MASK                                       (0x00020000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_EOL_SHIFT                                      (0x00000011U)
#define CSL_XLCDMA_GCFG_V2_CAP0_EOL_RESETVAL                                   (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_EOL_MAX                                        (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_STATIC_MASK                                    (0x00010000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_STATIC_SHIFT                                   (0x00000010U)
#define CSL_XLCDMA_GCFG_V2_CAP0_STATIC_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_STATIC_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE15_MASK                                    (0x00008000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE15_SHIFT                                   (0x0000000FU)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE15_RESETVAL                                (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE15_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE14_MASK                                    (0x00004000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE14_SHIFT                                   (0x0000000EU)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE14_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE14_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE13_MASK                                    (0x00002000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE13_SHIFT                                   (0x0000000DU)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE13_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE13_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE12_MASK                                    (0x00001000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE12_SHIFT                                   (0x0000000CU)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE12_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE12_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE11_MASK                                    (0x00000800U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE11_SHIFT                                   (0x0000000BU)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE11_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE11_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE10_MASK                                    (0x00000400U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE10_SHIFT                                   (0x0000000AU)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE10_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE10_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE9_MASK                                     (0x00000200U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE9_SHIFT                                    (0x00000009U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE9_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE9_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE8_MASK                                     (0x00000100U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE8_SHIFT                                    (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE8_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE8_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE7_MASK                                     (0x00000080U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE7_SHIFT                                    (0x00000007U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE7_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE7_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE6_MASK                                     (0x00000040U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE6_SHIFT                                    (0x00000006U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE6_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE6_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE5_MASK                                     (0x00000020U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE5_SHIFT                                    (0x00000005U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE5_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE5_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE4_MASK                                     (0x00000010U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE4_SHIFT                                    (0x00000004U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE4_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE4_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE3_MASK                                     (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE3_SHIFT                                    (0x00000003U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE3_RESETVAL                                 (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE3_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE2_MASK                                     (0x00000004U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE2_SHIFT                                    (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE2_RESETVAL                                 (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE2_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE1_MASK                                     (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE1_SHIFT                                    (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE1_RESETVAL                                 (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE1_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE0_MASK                                     (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE0_SHIFT                                    (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE0_RESETVAL                                 (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP0_TYPE0_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP0_RESETVAL                                       (0x000A800FU)

/* CAP1 */

#define CSL_XLCDMA_GCFG_V2_CAP1_SECTR_MASK                                     (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_CAP1_SECTR_SHIFT                                    (0x00000003U)
#define CSL_XLCDMA_GCFG_V2_CAP1_SECTR_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP1_SECTR_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP1_DFMT_MASK                                      (0x00000004U)
#define CSL_XLCDMA_GCFG_V2_CAP1_DFMT_SHIFT                                     (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_CAP1_DFMT_RESETVAL                                  (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP1_DFMT_MAX                                       (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP1_ELTYPE_MASK                                    (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_CAP1_ELTYPE_SHIFT                                   (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP1_ELTYPE_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP1_ELTYPE_MAX                                     (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP1_AMODE_MASK                                     (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_CAP1_AMODE_SHIFT                                    (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP1_AMODE_RESETVAL                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP1_AMODE_MAX                                      (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_CAP1_RESETVAL                                       (0x00000000U)

/* CAP2 */

#define CSL_XLCDMA_GCFG_V2_CAP2_RCHAN_CNT_MASK                                 (0x07FC0000U)
#define CSL_XLCDMA_GCFG_V2_CAP2_RCHAN_CNT_SHIFT                                (0x00000012U)
#define CSL_XLCDMA_GCFG_V2_CAP2_RCHAN_CNT_RESETVAL                             (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_CAP2_RCHAN_CNT_MAX                                  (0x000001FFU)

#define CSL_XLCDMA_GCFG_V2_CAP2_TCHAN_CNT_MASK                                 (0x0003FE00U)
#define CSL_XLCDMA_GCFG_V2_CAP2_TCHAN_CNT_SHIFT                                (0x00000009U)
#define CSL_XLCDMA_GCFG_V2_CAP2_TCHAN_CNT_RESETVAL                             (0x00000004U)
#define CSL_XLCDMA_GCFG_V2_CAP2_TCHAN_CNT_MAX                                  (0x000001FFU)

#define CSL_XLCDMA_GCFG_V2_CAP2_CHAN_CNT_MASK                                  (0x000001FFU)
#define CSL_XLCDMA_GCFG_V2_CAP2_CHAN_CNT_SHIFT                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP2_CHAN_CNT_RESETVAL                              (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP2_CHAN_CNT_MAX                                   (0x000001FFU)

#define CSL_XLCDMA_GCFG_V2_CAP2_RESETVAL                                       (0x00080800U)

/* CAP3 */

#define CSL_XLCDMA_GCFG_V2_CAP3_UCHAN_CNT_MASK                                 (0xFF800000U)
#define CSL_XLCDMA_GCFG_V2_CAP3_UCHAN_CNT_SHIFT                                (0x00000017U)
#define CSL_XLCDMA_GCFG_V2_CAP3_UCHAN_CNT_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP3_UCHAN_CNT_MAX                                  (0x000001FFU)

#define CSL_XLCDMA_GCFG_V2_CAP3_HCHAN_CNT_MASK                                 (0x007FC000U)
#define CSL_XLCDMA_GCFG_V2_CAP3_HCHAN_CNT_SHIFT                                (0x0000000EU)
#define CSL_XLCDMA_GCFG_V2_CAP3_HCHAN_CNT_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP3_HCHAN_CNT_MAX                                  (0x000001FFU)

#define CSL_XLCDMA_GCFG_V2_CAP3_RESETVAL                                       (0x00000000U)

/* CAP4 */

#define CSL_XLCDMA_GCFG_V2_CAP4_TUCHAN_CNT_MASK                                (0xFF000000U)
#define CSL_XLCDMA_GCFG_V2_CAP4_TUCHAN_CNT_SHIFT                               (0x00000018U)
#define CSL_XLCDMA_GCFG_V2_CAP4_TUCHAN_CNT_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP4_TUCHAN_CNT_MAX                                 (0x000000FFU)

#define CSL_XLCDMA_GCFG_V2_CAP4_THCHAN_CNT_MASK                                (0x00FF0000U)
#define CSL_XLCDMA_GCFG_V2_CAP4_THCHAN_CNT_SHIFT                               (0x00000010U)
#define CSL_XLCDMA_GCFG_V2_CAP4_THCHAN_CNT_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP4_THCHAN_CNT_MAX                                 (0x000000FFU)

#define CSL_XLCDMA_GCFG_V2_CAP4_RUCHAN_CNT_MASK                                (0x0000FF00U)
#define CSL_XLCDMA_GCFG_V2_CAP4_RUCHAN_CNT_SHIFT                               (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_CAP4_RUCHAN_CNT_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP4_RUCHAN_CNT_MAX                                 (0x000000FFU)

#define CSL_XLCDMA_GCFG_V2_CAP4_RHCHAN_CNT_MASK                                (0x000000FFU)
#define CSL_XLCDMA_GCFG_V2_CAP4_RHCHAN_CNT_SHIFT                               (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP4_RHCHAN_CNT_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP4_RHCHAN_CNT_MAX                                 (0x000000FFU)

#define CSL_XLCDMA_GCFG_V2_CAP4_RESETVAL                                       (0x00000000U)

/* CAP5 */

#define CSL_XLCDMA_GCFG_V2_CAP5_RFLOW_CNT_MASK                                 (0x00003FFFU)
#define CSL_XLCDMA_GCFG_V2_CAP5_RFLOW_CNT_SHIFT                                (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_CAP5_RFLOW_CNT_RESETVAL                             (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_CAP5_RFLOW_CNT_MAX                                  (0x00003FFFU)

#define CSL_XLCDMA_GCFG_V2_CAP5_TFLOW_CNT_MASK                                 (0x3FFF0000U)
#define CSL_XLCDMA_GCFG_V2_CAP5_TFLOW_CNT_SHIFT                                (0x00000010U)
#define CSL_XLCDMA_GCFG_V2_CAP5_TFLOW_CNT_RESETVAL                             (0x00000004U)
#define CSL_XLCDMA_GCFG_V2_CAP5_TFLOW_CNT_MAX                                  (0x00003FFFU)

#define CSL_XLCDMA_GCFG_V2_CAP5_RESETVAL                                       (0x00040002U)

/* PERF_CTRL */

#define CSL_XLCDMA_GCFG_V2_PERF_CTRL_TIMEOUT_CNT_MASK                          (0x0000FFFFU)
#define CSL_XLCDMA_GCFG_V2_PERF_CTRL_TIMEOUT_CNT_SHIFT                         (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PERF_CTRL_TIMEOUT_CNT_RESETVAL                      (0x00000040U)
#define CSL_XLCDMA_GCFG_V2_PERF_CTRL_TIMEOUT_CNT_MAX                           (0x0000FFFFU)

#define CSL_XLCDMA_GCFG_V2_PERF_CTRL_RESETVAL                                  (0x00000040U)

/* EMU_CTRL */

#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_SOFT_MASK                                  (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_SOFT_SHIFT                                 (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_SOFT_RESETVAL                              (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_SOFT_MAX                                   (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_FREE_MASK                                  (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_FREE_SHIFT                                 (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_FREE_RESETVAL                              (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_FREE_MAX                                   (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_EMU_CTRL_RESETVAL                                   (0x00000000U)

/* DBGADDR */

#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_EN_MASK                                 (0x80000000U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_EN_SHIFT                                (0x0000001FU)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_EN_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_EN_MAX                                  (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_MASK                               (0x0000FF00U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_SHIFT                              (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_MAX                                (0x000000FFU)

#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_VAL_RDEC2                          (0x12U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_VAL_SDEC3                          (0x23U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_VAL_CFG                            (0x50U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_VAL_TSU0                           (0xB0U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_UNIT_VAL_RSU0                           (0xC0U)

#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_ADDR_MASK                               (0x000000FFU)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_ADDR_SHIFT                              (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_ADDR_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGADDR_DBG_ADDR_MAX                                (0x000000FFU)

#define CSL_XLCDMA_GCFG_V2_DBGADDR_RESETVAL                                    (0x00000000U)

/* DBGDATA */

#define CSL_XLCDMA_GCFG_V2_DBGDATA_DBG_DATA_MASK                               (0xFFFFFFFFU)
#define CSL_XLCDMA_GCFG_V2_DBGDATA_DBG_DATA_SHIFT                              (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGDATA_DBG_DATA_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_DBGDATA_DBG_DATA_MAX                                (0xFFFFFFFFU)

#define CSL_XLCDMA_GCFG_V2_DBGDATA_RESETVAL                                    (0x00000000U)

/* PM0 */

#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB2_MASK                               (0x00000004U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB2_SHIFT                              (0x00000002U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB2_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB2_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB3_MASK                               (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB3_SHIFT                              (0x00000003U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB3_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_CARB3_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_WARB3_MASK                               (0x00000080U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_WARB3_SHIFT                              (0x00000007U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_WARB3_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_WARB3_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_SDEC3_MASK                               (0x00000800U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_SDEC3_SHIFT                              (0x0000000BU)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_SDEC3_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_SDEC3_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_RDEC2_MASK                               (0x00004000U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_RDEC2_SHIFT                              (0x0000000EU)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_RDEC2_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM0_NOGATE_RDEC2_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM0_RESETVAL                                        (0x00000000U)

/* PM1 */

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRU0_MASK                                (0x00000001U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRU0_SHIFT                               (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRU0_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRU0_MAX                                 (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RWU0_MASK                                (0x00000010U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RWU0_SHIFT                               (0x00000004U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RWU0_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RWU0_MAX                                 (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EVTCU_MASK                               (0x00000100U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EVTCU_SHIFT                              (0x00000008U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EVTCU_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EVTCU_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRCU_MASK                                (0x00000400U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRCU_SHIFT                               (0x0000000AU)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRCU_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TRCU_MAX                                 (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_CFG_MASK                                 (0x00040000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_CFG_SHIFT                                (0x00000012U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_CFG_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_CFG_MAX                                  (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PCF_MASK                                 (0x00200000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PCF_SHIFT                                (0x00000015U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PCF_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PCF_MAX                                  (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TPCF_MASK                                (0x00400000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TPCF_SHIFT                               (0x00000016U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TPCF_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_TPCF_MAX                                 (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RPCF_MASK                                (0x00800000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RPCF_SHIFT                               (0x00000017U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RPCF_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RPCF_MAX                                 (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RINGOCC_MASK                             (0x01000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RINGOCC_SHIFT                            (0x00000018U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RINGOCC_RESETVAL                         (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_RINGOCC_MAX                              (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EHANDLER_MASK                            (0x02000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EHANDLER_SHIFT                           (0x00000019U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EHANDLER_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EHANDLER_MAX                             (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_P2P_MASK                                 (0x08000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_P2P_SHIFT                                (0x0000001BU)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_P2P_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_P2P_MAX                                  (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PSILIF_MASK                              (0x10000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PSILIF_SHIFT                             (0x0000001CU)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PSILIF_RESETVAL                          (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PSILIF_MAX                               (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PROXY_MASK                               (0x20000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PROXY_SHIFT                              (0x0000001DU)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PROXY_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_PROXY_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_STATS_MASK                               (0x40000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_STATS_SHIFT                              (0x0000001EU)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_STATS_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_STATS_MAX                                (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EDC_MASK                                 (0x80000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EDC_SHIFT                                (0x0000001FU)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EDC_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_GCFG_V2_PM1_NOGATE_EDC_MAX                                  (0x00000001U)

#define CSL_XLCDMA_GCFG_V2_PM1_RESETVAL                                        (0x00000000U)

/**************************************************************************
* Hardware Region  : Channel Configuration Registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t CFG;                       /* Channel Configuration Register */
    volatile uint8_t  Resv_8[4];
    volatile uint32_t RESRC;                     /* Resource Register */
    volatile uint8_t  Resv_100[88];
    volatile uint32_t PRI_CTRL;                  /* Channel Priority Control Register */
    volatile uint8_t  Resv_112[8];
    volatile uint32_t FIFO_DEPTH;                /* Tx Channel FIFO Depth Register */
    volatile uint8_t  Resv_128[12];
    volatile uint32_t TST_SCHED;                 /* Channel Static Scheduler Config Register */
    volatile uint8_t  Resv_256[124];
} CSL_xlcdma_chcfg_v2Regs_chan;


typedef struct {
    CSL_xlcdma_chcfg_v2Regs_chan CHAN[11];
} CSL_xlcdma_chcfg_v2Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG(CHAN)                                     (0x00000000U+((CHAN)*0x100U))
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC(CHAN)                                   (0x00000008U+((CHAN)*0x100U))
#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL(CHAN)                                (0x00000064U+((CHAN)*0x100U))
#define CSL_XLCDMA_CHCFG_V2_CHAN_FIFO_DEPTH(CHAN)                              (0x00000070U+((CHAN)*0x100U))
#define CSL_XLCDMA_CHCFG_V2_CHAN_TST_SCHED(CHAN)                               (0x00000080U+((CHAN)*0x100U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CFG */

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_PAUSE_ON_ERR_MASK                         (0x80000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_PAUSE_ON_ERR_SHIFT                        (0x0000001FU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_PAUSE_ON_ERR_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_PAUSE_ON_ERR_MAX                          (0x00000001U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_LB_SEND_MASK                              (0x00100000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_LB_SEND_SHIFT                             (0x00000014U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_LB_SEND_RESETVAL                          (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_LB_SEND_MAX                               (0x00000001U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_LB_SEND_VAL_LOOP_ONLY                     (0x0U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_LB_SEND_VAL_SEND_LOOP                     (0x1U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_CHAN_TYPE_MASK                            (0x000F0000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_CHAN_TYPE_SHIFT                           (0x00000010U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_CHAN_TYPE_RESETVAL                        (0x0000000FU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_CHAN_TYPE_MAX                             (0x0000000FU)

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_CHAN_TYPE_VAL_UNUSED                      (0x0U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_CHAN_TYPE_VAL_DTR                         (0xFU)

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_BURST_SIZE_MASK                           (0x00000C00U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_BURST_SIZE_SHIFT                          (0x0000000AU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_BURST_SIZE_RESETVAL                       (0x00000001U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_BURST_SIZE_MAX                            (0x00000003U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_CFG_RESETVAL                                  (0x000F0400U)

/* RESRC */

#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_CFG_ALIAS_MASK                          (0x80000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_CFG_ALIAS_SHIFT                         (0x0000001FU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_CFG_ALIAS_RESETVAL                      (0x00000001U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_CFG_ALIAS_MAX                           (0x00000001U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_PRI_CTRL_ALIAS_MASK                     (0x40000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_PRI_CTRL_ALIAS_SHIFT                    (0x0000001EU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_PRI_CTRL_ALIAS_RESETVAL                 (0x00000001U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_PRI_CTRL_ALIAS_MAX                      (0x00000001U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_FIFO_DEPTH_ALIAS_MASK                   (0x10000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_FIFO_DEPTH_ALIAS_SHIFT                  (0x0000001CU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_FIFO_DEPTH_ALIAS_RESETVAL               (0x00000001U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_FIFO_DEPTH_ALIAS_MAX                    (0x00000001U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_ST_SCHED_ALIAS_MASK                     (0x08000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_ST_SCHED_ALIAS_SHIFT                    (0x0000001BU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_ST_SCHED_ALIAS_RESETVAL                 (0x00000001U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_ST_SCHED_ALIAS_MAX                      (0x00000001U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_RESRC_RESETVAL                                (0xD8000000U)

/* PRI_CTRL */

#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_PRIORITY_MASK                        (0x70000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_PRIORITY_SHIFT                       (0x0000001CU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_PRIORITY_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_PRIORITY_MAX                         (0x00000007U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_ORDERID_MASK                         (0x0000000FU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_ORDERID_SHIFT                        (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_ORDERID_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_ORDERID_MAX                          (0x0000000FU)

#define CSL_XLCDMA_CHCFG_V2_CHAN_PRI_CTRL_RESETVAL                             (0x00000000U)

/* FIFO_DEPTH */

#define CSL_XLCDMA_CHCFG_V2_CHAN_FIFO_DEPTH_FDEPTH_MASK                        (0x000001FFU)
#define CSL_XLCDMA_CHCFG_V2_CHAN_FIFO_DEPTH_FDEPTH_SHIFT                       (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_FIFO_DEPTH_FDEPTH_RESETVAL                    (0x00000100U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_FIFO_DEPTH_FDEPTH_MAX                         (0x000001FFU)

#define CSL_XLCDMA_CHCFG_V2_CHAN_FIFO_DEPTH_RESETVAL                           (0x00000100U)

/* TST_SCHED */

#define CSL_XLCDMA_CHCFG_V2_CHAN_TST_SCHED_PRIORITY_MASK                       (0x00000003U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_TST_SCHED_PRIORITY_SHIFT                      (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_TST_SCHED_PRIORITY_RESETVAL                   (0x00000000U)
#define CSL_XLCDMA_CHCFG_V2_CHAN_TST_SCHED_PRIORITY_MAX                        (0x00000003U)

#define CSL_XLCDMA_CHCFG_V2_CHAN_TST_SCHED_RESETVAL                            (0x00000000U)

/**************************************************************************
* Hardware Region  : Channel Realtime Registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t CTL;                       /* Channel Realtime Control Register */
    volatile uint32_t CFG;                       /* Channel Configuration Register Aliased */
    volatile uint32_t SWTRIG;                    /* Channel Realtime Software Trigger Register */
    volatile uint8_t  Resv_64[52];
    volatile uint32_t STATUS0;                   /* Channel Realtime Status Register 0 */
    volatile uint32_t STATUS1;                   /* Channel Realtime Status Register 1 */
    volatile uint8_t  Resv_100[28];
    volatile uint32_t PRI_CTRL;                  /* Channel Priority Control Register Aliased */
    volatile uint8_t  Resv_112[8];
    volatile uint32_t FIFO_DEPTH;                /* Tx Channel FIFO Depth Register */
    volatile uint8_t  Resv_120[4];
    volatile uint32_t TST_SCHED;                 /* Channel Static Scheduler Config Register Aliased */
    volatile uint8_t  Resv_128[4];
    volatile uint32_t STDATA[32];                /* Channel Realtime Read State Data Register */
    volatile uint32_t STDATAW[32];               /* Channel Realtime Write State Data Register */
    volatile uint8_t  Resv_1024[640];
    volatile uint32_t PCNT;                      /* Channel Real-time Packet Count Statistics Register */
    volatile uint8_t  Resv_1032[4];
    volatile uint32_t BCNT;                      /* Channel Real-time Completed Byte Count Statistics Register */
    volatile uint8_t  Resv_1040[4];
    volatile uint32_t SBCNT;                     /* Channel Real-time Started Byte Count Statistics Register */
    volatile uint8_t  Resv_2048[1004];
    volatile uint32_t STATIC_TR_XY;              /* STATIC_TR_XY */
    volatile uint32_t STATIC_TR_Z;               /* STATIC_TR_Z */
    volatile uint32_t PDMA_LCNTS;                /* Peripheral DMA Loop Counts */
    volatile uint32_t PDMA_STATE;                /* Peripheral DMA State */
    volatile uint8_t  Resv_2108[44];
    volatile uint32_t PERIPH_FZCNT;              /* Peripheral Full Z Count */
    volatile uint8_t  Resv_4096[1984];
} CSL_xlcdma_chrt_v2Regs_chan;


typedef struct {
    CSL_xlcdma_chrt_v2Regs_chan CHAN[11];
} CSL_xlcdma_chrt_v2Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL(CHAN)                                      (0x00000000U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG(CHAN)                                      (0x00000004U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_SWTRIG(CHAN)                                   (0x00000008U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0(CHAN)                                  (0x00000040U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1(CHAN)                                  (0x00000044U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL(CHAN)                                 (0x00000064U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_FIFO_DEPTH(CHAN)                               (0x00000070U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_TST_SCHED(CHAN)                                (0x00000078U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATA(CHAN,STDATA)                            (0x00000080U+((CHAN)*0x1000U)+((STDATA)*0x4U))
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATAW(CHAN,STDATAW)                          (0x00000100U+((CHAN)*0x1000U)+((STDATAW)*0x4U))
#define CSL_XLCDMA_CHRT_V2_CHAN_PCNT(CHAN)                                     (0x00000400U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_BCNT(CHAN)                                     (0x00000408U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_SBCNT(CHAN)                                    (0x00000410U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY(CHAN)                             (0x00000800U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z(CHAN)                              (0x00000804U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS(CHAN)                               (0x00000808U+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE(CHAN)                               (0x0000080CU+((CHAN)*0x1000U))
#define CSL_XLCDMA_CHRT_V2_CHAN_PERIPH_FZCNT(CHAN)                             (0x0000083CU+((CHAN)*0x1000U))

/**************************************************************************
* 64K Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t CTL;                       /* Channel Realtime Control Register */
    volatile uint32_t CFG;                       /* Channel Configuration Register Aliased */
    volatile uint32_t SWTRIG;                    /* Channel Realtime Software Trigger Register */
    volatile uint8_t  Resv_64[52];
    volatile uint32_t STATUS0;                   /* Channel Realtime Status Register 0 */
    volatile uint32_t STATUS1;                   /* Channel Realtime Status Register 1 */
    volatile uint8_t  Resv_100[28];
    volatile uint32_t PRI_CTRL;                  /* Channel Priority Control Register Aliased */
    volatile uint8_t  Resv_112[8];
    volatile uint32_t FIFO_DEPTH;                /* Tx Channel FIFO Depth Register */
    volatile uint8_t  Resv_120[4];
    volatile uint32_t TST_SCHED;                 /* Channel Static Scheduler Config Register Aliased */
    volatile uint8_t  Resv_128[4];
    volatile uint32_t STDATA[32];                /* Channel Realtime Read State Data Register */
    volatile uint32_t STDATAW[32];               /* Channel Realtime Write State Data Register */
    volatile uint8_t  Resv_1024[640];
    volatile uint32_t PCNT;                      /* Channel Real-time Packet Count Statistics Register */
    volatile uint8_t  Resv_1032[4];
    volatile uint32_t BCNT;                      /* Channel Real-time Completed Byte Count Statistics Register */
    volatile uint8_t  Resv_1040[4];
    volatile uint32_t SBCNT;                     /* Channel Real-time Started Byte Count Statistics Register */
    volatile uint8_t  Resv_2048[1004];
    volatile uint32_t STATIC_TR_XY;              /* STATIC_TR_XY */
    volatile uint32_t STATIC_TR_Z;               /* STATIC_TR_Z */
    volatile uint32_t PDMA_LCNTS;                /* Peripheral DMA Loop Counts */
    volatile uint32_t PDMA_STATE;                /* Peripheral DMA State */
    volatile uint8_t  Resv_2108[44];
    volatile uint32_t PERIPH_FZCNT;              /* Peripheral Full Z Count */
    volatile uint8_t  Resv_65536[63424];
} CSL_xlcdma_chrt_v2Regs64_chan;


typedef struct {
    CSL_xlcdma_chrt_v2Regs64_chan CHAN[11];
} CSL_xlcdma_chrt_v2Regs64;


/**************************************************************************
* 64K Register Macros
**************************************************************************/

#define CSL_XLCDMA_CHRT_V2_64_CHAN_CTL(CHAN)                                   (0x00000000U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_CFG(CHAN)                                   (0x00000004U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_SWTRIG(CHAN)                                (0x00000008U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_STATUS0(CHAN)                               (0x00000040U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_STATUS1(CHAN)                               (0x00000044U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_PRI_CTRL(CHAN)                              (0x00000064U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_FIFO_DEPTH(CHAN)                            (0x00000070U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_TST_SCHED(CHAN)                             (0x00000078U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_STDATA(CHAN,STDATA)                         (0x00000080U+((CHAN)*0x10000U)+((STDATA)*0x4U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_STDATAW(CHAN,STDATAW)                       (0x00000100U+((CHAN)*0x10000U)+((STDATAW)*0x4U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_PCNT(CHAN)                                  (0x00000400U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_BCNT(CHAN)                                  (0x00000408U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_SBCNT(CHAN)                                 (0x00000410U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_STATIC_TR_XY(CHAN)                          (0x00000800U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_STATIC_TR_Z(CHAN)                           (0x00000804U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_PDMA_LCNTS(CHAN)                            (0x00000808U+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_PDMA_STATE(CHAN)                            (0x0000080CU+((CHAN)*0x10000U))
#define CSL_XLCDMA_CHRT_V2_64_CHAN_PERIPH_FZCNT(CHAN)                          (0x0000083CU+((CHAN)*0x10000U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CTL */

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_EN_MASK                                    (0x80000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_EN_SHIFT                                   (0x0000001FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_EN_RESETVAL                                (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_EN_MAX                                     (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_TDOWN_MASK                                 (0x40000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_TDOWN_SHIFT                                (0x0000001EU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_TDOWN_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_TDOWN_MAX                                  (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_MASK                                 (0x20000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_SHIFT                                (0x0000001DU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_MAX                                  (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_FTDOWN_MASK                                (0x10000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_FTDOWN_SHIFT                               (0x0000001CU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_FTDOWN_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_FTDOWN_MAX                                 (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_EOP_MASK                             (0x08000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_EOP_SHIFT                            (0x0000001BU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_EOP_RESETVAL                         (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_PAUSE_EOP_MAX                              (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_ERROR_MASK                                 (0x00000001U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_ERROR_SHIFT                                (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_ERROR_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_ERROR_MAX                                  (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CTL_RESETVAL                                   (0x00000000U)

/* CFG */

#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_PAUSE_ON_ERR_MASK                          (0x80000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_PAUSE_ON_ERR_SHIFT                         (0x0000001FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_PAUSE_ON_ERR_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_PAUSE_ON_ERR_MAX                           (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_LB_SEND_MASK                               (0x00100000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_LB_SEND_SHIFT                              (0x00000014U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_LB_SEND_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_LB_SEND_MAX                                (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_LB_SEND_VAL_LOOP_ONLY                      (0x0U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_LB_SEND_VAL_SEND_LOOP                      (0x1U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_CHAN_TYPE_MASK                             (0x000F0000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_CHAN_TYPE_SHIFT                            (0x00000010U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_CHAN_TYPE_RESETVAL                         (0x0000000FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_CHAN_TYPE_MAX                              (0x0000000FU)

#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_CHAN_TYPE_VAL_UNUSED                       (0x0U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_CHAN_TYPE_VAL_DTR                          (0xFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_BURST_SIZE_MASK                            (0x00000C00U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_BURST_SIZE_SHIFT                           (0x0000000AU)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_BURST_SIZE_RESETVAL                        (0x00000001U)
#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_BURST_SIZE_MAX                             (0x00000003U)

#define CSL_XLCDMA_CHRT_V2_CHAN_CFG_RESETVAL                                   (0x000F0400U)

/* SWTRIG */

#define CSL_XLCDMA_CHRT_V2_CHAN_SWTRIG_TRIGGER_MASK                            (0x00000001U)
#define CSL_XLCDMA_CHRT_V2_CHAN_SWTRIG_TRIGGER_SHIFT                           (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_SWTRIG_TRIGGER_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_SWTRIG_TRIGGER_MAX                             (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_SWTRIG_RESETVAL                                (0x00000000U)

/* STATUS0 */

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_RING_PEND_MASK                         (0x80000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_RING_PEND_SHIFT                        (0x0000001FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_RING_PEND_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_RING_PEND_MAX                          (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_Q_PEND_MASK                            (0x40000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_Q_PEND_SHIFT                           (0x0000001EU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_Q_PEND_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_Q_PEND_MAX                             (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_AVAIL_MASK                       (0x20000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_AVAIL_SHIFT                      (0x0000001DU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_AVAIL_RESETVAL                   (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_AVAIL_MAX                        (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_BUSY_MASK                        (0x10000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_BUSY_SHIFT                       (0x0000001CU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_BUSY_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_PKTID_BUSY_MAX                         (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_BUSY_MASK                              (0x02000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_BUSY_SHIFT                             (0x00000019U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_BUSY_RESETVAL                          (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_BUSY_MAX                               (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TRANSBUSY_MASK                         (0x01000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TRANSBUSY_SHIFT                        (0x00000018U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TRANSBUSY_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TRANSBUSY_MAX                          (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_IN_PACKET_MASK                         (0x00800000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_IN_PACKET_SHIFT                        (0x00000017U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_IN_PACKET_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_IN_PACKET_MAX                          (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_OK_MASK                                (0x00400000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_OK_SHIFT                               (0x00000016U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_OK_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_OK_MAX                                 (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_WAVAIL_MASK                            (0x00200000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_WAVAIL_SHIFT                           (0x00000015U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_WAVAIL_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_WAVAIL_MAX                             (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TDOWN_MSG_PEND_MASK                    (0x00040000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TDOWN_MSG_PEND_SHIFT                   (0x00000012U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TDOWN_MSG_PEND_RESETVAL                (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TDOWN_MSG_PEND_MAX                     (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TX_REQ_MASK                            (0x00020000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TX_REQ_SHIFT                           (0x00000011U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TX_REQ_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_TX_REQ_MAX                             (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_ERR_EVENT_REQS_MASK                    (0x00010000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_ERR_EVENT_REQS_SHIFT                   (0x00000010U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_ERR_EVENT_REQS_RESETVAL                (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_ERR_EVENT_REQS_MAX                     (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS0_RESETVAL                               (0x00000000U)

/* STATUS1 */

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_REQS_MASK                              (0x80000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_REQS_SHIFT                             (0x0000001FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_REQS_RESETVAL                          (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_REQS_MAX                               (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_SOP_WAVAVIL_MASK                       (0x04000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_SOP_WAVAVIL_SHIFT                      (0x0000001AU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_SOP_WAVAVIL_RESETVAL                   (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_SOP_WAVAVIL_MAX                        (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_PEND_MASK                         (0x02000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_PEND_SHIFT                        (0x00000019U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_PEND_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_PEND_MAX                          (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_BUSY_MASK                         (0x01000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_BUSY_SHIFT                        (0x00000018U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_BUSY_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_FIFO_BUSY_MAX                          (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_TDNULL_MASK                            (0x00000100U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_TDNULL_SHIFT                           (0x00000008U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_TDNULL_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_TDNULL_MAX                             (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_OKAY_MASK                      (0x00000080U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_OKAY_SHIFT                     (0x00000007U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_OKAY_RESETVAL                  (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_OKAY_MAX                       (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_BUSY_MASK                      (0x00000040U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_BUSY_SHIFT                     (0x00000006U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_BUSY_RESETVAL                  (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_CHANNEL_BUSY_MAX                       (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_IN_PACKET_ARRAY_MASK                   (0x00000008U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_IN_PACKET_ARRAY_SHIFT                  (0x00000003U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_IN_PACKET_ARRAY_RESETVAL               (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_IN_PACKET_ARRAY_MAX                    (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATUS1_RESETVAL                               (0x00000000U)

/* PRI_CTRL */

#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_PRIORITY_MASK                         (0x70000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_PRIORITY_SHIFT                        (0x0000001CU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_PRIORITY_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_PRIORITY_MAX                          (0x00000007U)

#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_ORDERID_MASK                          (0x0000000FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_ORDERID_SHIFT                         (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_ORDERID_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_ORDERID_MAX                           (0x0000000FU)

#define CSL_XLCDMA_CHRT_V2_CHAN_PRI_CTRL_RESETVAL                              (0x00000000U)

/* FIFO_DEPTH */

#define CSL_XLCDMA_CHRT_V2_CHAN_FIFO_DEPTH_FDEPTH_MASK                         (0x000001FFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_FIFO_DEPTH_FDEPTH_SHIFT                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_FIFO_DEPTH_FDEPTH_RESETVAL                     (0x00000100U)
#define CSL_XLCDMA_CHRT_V2_CHAN_FIFO_DEPTH_FDEPTH_MAX                          (0x000001FFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_FIFO_DEPTH_RESETVAL                            (0x00000100U)

/* TST_SCHED */

#define CSL_XLCDMA_CHRT_V2_CHAN_TST_SCHED_PRIORITY_MASK                        (0x00000003U)
#define CSL_XLCDMA_CHRT_V2_CHAN_TST_SCHED_PRIORITY_SHIFT                       (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_TST_SCHED_PRIORITY_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_TST_SCHED_PRIORITY_MAX                         (0x00000003U)

#define CSL_XLCDMA_CHRT_V2_CHAN_TST_SCHED_RESETVAL                             (0x00000000U)

/* STDATA */

#define CSL_XLCDMA_CHRT_V2_CHAN_STDATA_STATE_INFO_MASK                         (0xFFFFFFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATA_STATE_INFO_SHIFT                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATA_STATE_INFO_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATA_STATE_INFO_MAX                          (0xFFFFFFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_STDATA_RESETVAL                                (0x00000000U)

/* STDATAW */

#define CSL_XLCDMA_CHRT_V2_CHAN_STDATAW_STATE_INFO_MASK                        (0xFFFFFFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATAW_STATE_INFO_SHIFT                       (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATAW_STATE_INFO_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STDATAW_STATE_INFO_MAX                         (0xFFFFFFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_STDATAW_RESETVAL                               (0x00000000U)

/* PCNT */

#define CSL_XLCDMA_CHRT_V2_CHAN_PCNT_PCNT_MASK                                 (0xFFFFFFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PCNT_PCNT_SHIFT                                (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PCNT_PCNT_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PCNT_PCNT_MAX                                  (0xFFFFFFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_PCNT_RESETVAL                                  (0x00000000U)

/* BCNT */

#define CSL_XLCDMA_CHRT_V2_CHAN_BCNT_BCNT_MASK                                 (0xFFFFFFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_BCNT_BCNT_SHIFT                                (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_BCNT_BCNT_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_BCNT_BCNT_MAX                                  (0xFFFFFFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_BCNT_RESETVAL                                  (0x00000000U)

/* SBCNT */

#define CSL_XLCDMA_CHRT_V2_CHAN_SBCNT_SBCNT_MASK                               (0xFFFFFFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_SBCNT_SBCNT_SHIFT                              (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_SBCNT_SBCNT_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_SBCNT_SBCNT_MAX                                (0xFFFFFFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_SBCNT_RESETVAL                                 (0x00000000U)

/* STATIC_TR_XY */

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_BURST_MASK                        (0x80000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_BURST_SHIFT                       (0x0000001FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_BURST_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_BURST_MAX                         (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_ACC32_MASK                        (0x40000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_ACC32_SHIFT                       (0x0000001EU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_ACC32_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_ACC32_MAX                         (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_X_MASK                            (0x07000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_X_SHIFT                           (0x00000018U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_X_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_X_MAX                             (0x00000007U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_Y_MASK                            (0x00000FFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_Y_SHIFT                           (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_Y_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_Y_MAX                             (0x00000FFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_XY_RESETVAL                          (0x00000000U)

/* STATIC_TR_Z */

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_EOL_MASK                           (0x80000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_EOL_SHIFT                          (0x0000001FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_EOL_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_EOL_MAX                            (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_Z_MASK                             (0x00FFFFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_Z_SHIFT                            (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_Z_RESETVAL                         (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_Z_MAX                              (0x00FFFFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_STATIC_TR_Z_RESETVAL                           (0x00000000U)

/* PDMA_LCNTS */

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Z_MASK                              (0xFFFF0000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Z_SHIFT                             (0x00000010U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Z_RESETVAL                          (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Z_MAX                               (0x0000FFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Y_MASK                              (0x0000FFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Y_SHIFT                             (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Y_RESETVAL                          (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_Y_MAX                               (0x0000FFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_LCNTS_RESETVAL                            (0x00000000U)

/* PDMA_STATE */

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_INEVT_MASK                          (0x80000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_INEVT_SHIFT                         (0x0000001FU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_INEVT_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_INEVT_MAX                           (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_TDOWN_MASK                          (0x40000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_TDOWN_SHIFT                         (0x0000001EU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_TDOWN_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_TDOWN_MAX                           (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_PAUSE_MASK                          (0x20000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_PAUSE_SHIFT                         (0x0000001DU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_PAUSE_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_PAUSE_MAX                           (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_SPACE_MASK                          (0x10000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_SPACE_SHIFT                         (0x0000001CU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_SPACE_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_SPACE_MAX                           (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_XPSPACE_MASK                        (0x08000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_XPSPACE_SHIFT                       (0x0000001BU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_XPSPACE_RESETVAL                    (0x00000001U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_XPSPACE_MAX                         (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_BUF_MASK                            (0x04000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_BUF_SHIFT                           (0x0000001AU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_BUF_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_BUF_MAX                             (0x00000001U)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_STATE_MASK                          (0x00F00000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_STATE_SHIFT                         (0x00000014U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_STATE_RESETVAL                      (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_STATE_MAX                           (0x0000000FU)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_ECNT_MASK                           (0x000F0000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_ECNT_SHIFT                          (0x00000010U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_ECNT_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_ECNT_MAX                            (0x0000000FU)

#define CSL_XLCDMA_CHRT_V2_CHAN_PDMA_STATE_RESETVAL                            (0x08000000U)

/* PERIPH_FZCNT */

#define CSL_XLCDMA_CHRT_V2_CHAN_PERIPH_FZCNT_CURR_MASK                         (0xFFFFFFFFU)
#define CSL_XLCDMA_CHRT_V2_CHAN_PERIPH_FZCNT_CURR_SHIFT                        (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PERIPH_FZCNT_CURR_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_CHRT_V2_CHAN_PERIPH_FZCNT_CURR_MAX                          (0xFFFFFFFFU)

#define CSL_XLCDMA_CHRT_V2_CHAN_PERIPH_FZCNT_RESETVAL                          (0x00000000U)

/**************************************************************************
* Hardware Region  : Flow Realtime Registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint8_t  Resv_256[256];
    volatile uint32_t INTSRC;                    /* Interrupt Source */
    volatile uint8_t  Resv_320[60];
    volatile uint32_t INT_ENABLE_SET;            /* Interrupt Enable Set */
    volatile uint8_t  Resv_328[4];
    volatile uint32_t INT_ENABLE_CLR;            /* Interrupt Enable Clear */
    volatile uint8_t  Resv_336[4];
    volatile uint32_t INT_STATUS_SET;            /* Interrupt Status */
    volatile uint8_t  Resv_344[4];
    volatile uint32_t INT_STATUS;                /* Interrupt Status */
    volatile uint8_t  Resv_352[4];
    volatile uint32_t INT_STATUS_MSKD;           /* Interrupt Status Masked */
    volatile uint8_t  Resv_2048[1692];
    volatile uint32_t FLAG;                      /* Direct TR Flag Field */
    volatile uint32_t ICNT0_1;                   /* ICNT0_1 */
    volatile uint32_t SRC_ADDR_LO;               /* SRC ADDR LO */
    volatile uint32_t SRC_ADDR_HI;               /* SRC ADDR HI */
    volatile uint32_t SRCDIM1;                   /* SRCDIM1 */
    volatile uint8_t  Resv_2080[12];
    volatile uint32_t FMTFLAGS;                  /* FMTFLAGS */
    volatile uint32_t DSTDIM1;                   /* DSTDIM1 */
    volatile uint32_t DST_ADDR_LO;               /* DST ADDR LO */
    volatile uint32_t DST_ADDR_HI;               /* DST ADDR HI */
    volatile uint8_t  Resv_2104[8];
    volatile uint32_t DICNT0_1;                  /* DICNT0_1 */
    volatile uint8_t  Resv_2112[4];
    volatile uint32_t TR_SUBMIT;                 /* TR_SUBMIT */
    volatile uint8_t  Resv_8192[6076];
} CSL_xlcdma_flowrt_v2Regs_flow;


typedef struct {
    CSL_xlcdma_flowrt_v2Regs_flow FLOW[11];
} CSL_xlcdma_flowrt_v2Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC(FLOW)                                 (0x00000100U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET(FLOW)                         (0x00000140U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR(FLOW)                         (0x00000148U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET(FLOW)                         (0x00000150U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS(FLOW)                             (0x00000158U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD(FLOW)                        (0x00000160U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG(FLOW)                                   (0x00000800U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1(FLOW)                                (0x00000804U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_LO(FLOW)                            (0x00000808U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI(FLOW)                            (0x0000080CU+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRCDIM1(FLOW)                                (0x00000810U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FMTFLAGS(FLOW)                               (0x00000820U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DSTDIM1(FLOW)                                (0x00000824U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_LO(FLOW)                            (0x00000828U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI(FLOW)                            (0x0000082CU+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1(FLOW)                               (0x00000838U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT(FLOW)                              (0x00000840U+((FLOW)*0x2000U))

/**************************************************************************
* 64K Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint8_t  Resv_256[256];
    volatile uint32_t INTSRC;                    /* Interrupt Source */
    volatile uint8_t  Resv_320[60];
    volatile uint32_t INT_ENABLE_SET;            /* Interrupt Enable Set */
    volatile uint8_t  Resv_328[4];
    volatile uint32_t INT_ENABLE_CLR;            /* Interrupt Enable Clear */
    volatile uint8_t  Resv_336[4];
    volatile uint32_t INT_STATUS_SET;            /* Interrupt Status */
    volatile uint8_t  Resv_344[4];
    volatile uint32_t INT_STATUS;                /* Interrupt Status */
    volatile uint8_t  Resv_352[4];
    volatile uint32_t INT_STATUS_MSKD;           /* Interrupt Status Masked */
    volatile uint8_t  Resv_2048[1692];
    volatile uint32_t FLAG;                      /* Direct TR Flag Field */
    volatile uint32_t ICNT0_1;                   /* ICNT0_1 */
    volatile uint32_t SRC_ADDR_LO;               /* SRC ADDR LO */
    volatile uint32_t SRC_ADDR_HI;               /* SRC ADDR HI */
    volatile uint32_t SRCDIM1;                   /* SRCDIM1 */
    volatile uint8_t  Resv_2080[12];
    volatile uint32_t FMTFLAGS;                  /* FMTFLAGS */
    volatile uint32_t DSTDIM1;                   /* DSTDIM1 */
    volatile uint32_t DST_ADDR_LO;               /* DST ADDR LO */
    volatile uint32_t DST_ADDR_HI;               /* DST ADDR HI */
    volatile uint8_t  Resv_2104[8];
    volatile uint32_t DICNT0_1;                  /* DICNT0_1 */
    volatile uint8_t  Resv_2112[4];
    volatile uint32_t TR_SUBMIT;                 /* TR_SUBMIT */
    volatile uint8_t  Resv_8192[6076];
} CSL_xlcdma_flowrt_v2Regs64_flow;


typedef struct {
    CSL_xlcdma_flowrt_v2Regs64_flow FLOW[11];
} CSL_xlcdma_flowrt_v2Regs64;


/**************************************************************************
* 64K Register Macros
**************************************************************************/

#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_INTSRC(FLOW)                              (0x00000100U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_INT_ENABLE_SET(FLOW)                      (0x00000140U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_INT_ENABLE_CLR(FLOW)                      (0x00000148U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_INT_STATUS_SET(FLOW)                      (0x00000150U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_INT_STATUS(FLOW)                          (0x00000158U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_INT_STATUS_MSKD(FLOW)                     (0x00000160U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_FLAG(FLOW)                                (0x00000800U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_ICNT0_1(FLOW)                             (0x00000804U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_SRC_ADDR_LO(FLOW)                         (0x00000808U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_SRC_ADDR_HI(FLOW)                         (0x0000080CU+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_SRCDIM1(FLOW)                             (0x00000810U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_FMTFLAGS(FLOW)                            (0x00000820U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_DSTDIM1(FLOW)                             (0x00000824U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_DST_ADDR_LO(FLOW)                         (0x00000828U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_DST_ADDR_HI(FLOW)                         (0x0000082CU+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_DICNT0_1(FLOW)                            (0x00000838U+((FLOW)*0x2000U))
#define CSL_XLCDMA_FLOWRT_V2_64_FLOW_TR_SUBMIT(FLOW)                           (0x00000840U+((FLOW)*0x2000U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* INTSRC */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR0_MASK                            (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR0_SHIFT                           (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR0_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR0_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR1_MASK                            (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR1_SHIFT                           (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR1_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR1_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR2_MASK                            (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR2_SHIFT                           (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR2_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR2_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR3_MASK                            (0x00000008U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR3_SHIFT                           (0x00000003U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR3_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR3_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR4_MASK                            (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR4_SHIFT                           (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR4_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR4_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR5_MASK                            (0x00000020U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR5_SHIFT                           (0x00000005U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR5_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR5_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR6_MASK                            (0x00000040U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR6_SHIFT                           (0x00000006U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR6_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR6_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR7_MASK                            (0x00000080U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR7_SHIFT                           (0x00000007U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR7_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR7_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR8_MASK                            (0x00000100U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR8_SHIFT                           (0x00000008U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR8_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR8_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR9_MASK                            (0x00000200U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR9_SHIFT                           (0x00000009U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR9_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR9_MAX                             (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR10_MASK                           (0x00000400U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR10_SHIFT                          (0x0000000AU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR10_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR10_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR11_MASK                           (0x00000800U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR11_SHIFT                          (0x0000000BU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR11_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR11_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR12_MASK                           (0x00001000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR12_SHIFT                          (0x0000000CU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR12_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR12_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR13_MASK                           (0x00002000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR13_SHIFT                          (0x0000000DU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR13_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR13_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR14_MASK                           (0x00004000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR14_SHIFT                          (0x0000000EU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR14_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR14_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR15_MASK                           (0x00008000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR15_SHIFT                          (0x0000000FU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR15_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR15_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR16_MASK                           (0x00010000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR16_SHIFT                          (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR16_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR16_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR17_MASK                           (0x00020000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR17_SHIFT                          (0x00000011U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR17_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR17_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR18_MASK                           (0x00040000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR18_SHIFT                          (0x00000012U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR18_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR18_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR19_MASK                           (0x00080000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR19_SHIFT                          (0x00000013U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR19_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR19_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR20_MASK                           (0x00100000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR20_SHIFT                          (0x00000014U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR20_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR20_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR21_MASK                           (0x00200000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR21_SHIFT                          (0x00000015U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR21_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR21_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR22_MASK                           (0x00400000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR22_SHIFT                          (0x00000016U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR22_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR22_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR23_MASK                           (0x00800000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR23_SHIFT                          (0x00000017U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR23_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR23_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR24_MASK                           (0x01000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR24_SHIFT                          (0x00000018U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR24_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR24_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR25_MASK                           (0x02000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR25_SHIFT                          (0x00000019U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR25_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR25_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR26_MASK                           (0x04000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR26_SHIFT                          (0x0000001AU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR26_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR26_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR27_MASK                           (0x08000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR27_SHIFT                          (0x0000001BU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR27_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR27_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR28_MASK                           (0x10000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR28_SHIFT                          (0x0000001CU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR28_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR28_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR29_MASK                           (0x20000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR29_SHIFT                          (0x0000001DU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR29_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR29_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR30_MASK                           (0x40000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR30_SHIFT                          (0x0000001EU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR30_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR30_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR31_MASK                           (0x80000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR31_SHIFT                          (0x0000001FU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR31_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_INTR31_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INTSRC_RESETVAL                              (0x00000000U)

/* INT_ENABLE_SET */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_COMPLETE_MASK                 (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_COMPLETE_SHIFT                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_COMPLETE_RESETVAL             (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_COMPLETE_MAX                  (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_ERROR_MASK                    (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_ERROR_SHIFT                   (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_ERROR_RESETVAL                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_ERROR_MAX                     (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_TR_MASK                       (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_TR_SHIFT                      (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_TR_RESETVAL                   (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_TR_MAX                        (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_SET_RESETVAL                      (0x00000000U)

/* INT_ENABLE_CLR */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_COMPLETE_MASK                 (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_COMPLETE_SHIFT                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_COMPLETE_RESETVAL             (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_COMPLETE_MAX                  (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_ERROR_MASK                    (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_ERROR_SHIFT                   (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_ERROR_RESETVAL                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_ERROR_MAX                     (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_TR_MASK                       (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_TR_SHIFT                      (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_TR_RESETVAL                   (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_TR_MAX                        (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_ENABLE_CLR_RESETVAL                      (0x00000000U)

/* INT_STATUS_SET */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_COMPLETE_MASK                 (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_COMPLETE_SHIFT                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_COMPLETE_RESETVAL             (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_COMPLETE_MAX                  (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_ERROR_MASK                    (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_ERROR_SHIFT                   (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_ERROR_RESETVAL                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_ERROR_MAX                     (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_TR_MASK                       (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_TR_SHIFT                      (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_TR_RESETVAL                   (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_TR_MAX                        (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_SET_RESETVAL                      (0x00000000U)

/* INT_STATUS */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_COMPLETE_MASK                     (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_COMPLETE_SHIFT                    (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_COMPLETE_RESETVAL                 (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_COMPLETE_MAX                      (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_ERROR_MASK                        (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_ERROR_SHIFT                       (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_ERROR_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_ERROR_MAX                         (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_TR_MASK                           (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_TR_SHIFT                          (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_TR_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_TR_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_RESETVAL                          (0x00000000U)

/* INT_STATUS_MSKD */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_COMPLETE_MASK                (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_COMPLETE_SHIFT               (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_COMPLETE_RESETVAL            (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_COMPLETE_MAX                 (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_ERROR_MASK                   (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_ERROR_SHIFT                  (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_ERROR_RESETVAL               (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_ERROR_MAX                    (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_TR_MASK                      (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_TR_SHIFT                     (0x00000002U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_TR_RESETVAL                  (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_TR_MAX                       (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_INT_STATUS_MSKD_RESETVAL                     (0x00000000U)

/* FLAG */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_MASK                               (0x0000000FU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_SHIFT                              (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_MAX                                (0x0000000FU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_ONE_DIM                        (0x0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_TWO_DIM                        (0x1U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_THREE_DIM                      (0x2U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_FOUR_DIM                       (0x3U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_FOUR_DIM_FMT                   (0x4U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_CACHE_WARM                     (0x5U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_BM_FOUR_DIM                    (0x8U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_BM_FOUR_DIM_REPACK             (0x9U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_BM_TWO_DIM                     (0xAU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_BM_TWO_DIM_REPACK              (0xBU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TYPE_VAL_BM_FOUR_DIM_REPACK_IND         (0xFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_STATIC_MASK                             (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_STATIC_SHIFT                            (0x00000004U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_STATIC_RESETVAL                         (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_STATIC_MAX                              (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_WAIT_MASK                               (0x00000020U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_WAIT_SHIFT                              (0x00000005U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_WAIT_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_WAIT_MAX                                (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_WAIT_VAL_IMMEDIATE                      (0x0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_WAIT_VAL_WAIT_COMP                      (0x1U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_MASK                         (0x000000C0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_SHIFT                        (0x00000006U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_MAX                          (0x00000003U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_VAL_COMP                     (0x0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_VAL_ICNT1                    (0x1U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_VAL_ICNT2                    (0x2U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_EVENT_SIZE_VAL_ICNT3                    (0x3U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_MASK                           (0x00000300U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_SHIFT                          (0x00000008U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_MAX                            (0x00000003U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_VAL_NO_TRIG                    (0x0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_VAL_GLOB0                      (0x1U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_VAL_GLOB1                      (0x2U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_VAL_LOCAL                      (0x3U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_MASK                      (0x00000C00U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_SHIFT                     (0x0000000AU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_RESETVAL                  (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_MAX                       (0x00000003U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_VAL_ICNT1                 (0x0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_VAL_ICNT2                 (0x1U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_VAL_ICNT3                 (0x2U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER0_TYPE_VAL_COMP                  (0x3U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_MASK                           (0x00003000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_SHIFT                          (0x0000000CU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_MAX                            (0x00000003U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_VAL_NO_TRIG                    (0x0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_VAL_GLOB0                      (0x1U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_VAL_GLOB1                      (0x2U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_VAL_LOCAL                      (0x3U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_MASK                      (0x0000C000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_SHIFT                     (0x0000000EU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_RESETVAL                  (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_MAX                       (0x00000003U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_VAL_ICNT1                 (0x0U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_VAL_ICNT2                 (0x1U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_VAL_ICNT3                 (0x2U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_TRIGGER1_TYPE_VAL_COMP                  (0x3U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_CMD_ID_MASK                             (0x00FF0000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_CMD_ID_SHIFT                            (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_CMD_ID_RESETVAL                         (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_CMD_ID_MAX                              (0x000000FFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_ISA_MASK                                (0x01000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_ISA_SHIFT                               (0x00000018U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_ISA_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_ISA_MAX                                 (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_IDA_MASK                                (0x02000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_IDA_SHIFT                               (0x00000019U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_IDA_RESETVAL                            (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_IDA_MAX                                 (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_SUPR_EVT_MASK                           (0x04000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_SUPR_EVT_SHIFT                          (0x0000001AU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_SUPR_EVT_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_SUPR_EVT_MAX                            (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FLAG_RESETVAL                                (0x00000000U)

/* ICNT0_1 */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT0_MASK                           (0x0000FFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT0_SHIFT                          (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT0_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT0_MAX                            (0x0000FFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT1_MASK                           (0xFFFF0000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT1_SHIFT                          (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT1_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_ICNT1_MAX                            (0x0000FFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_ICNT0_1_RESETVAL                             (0x00000000U)

/* SRC_ADDR_LO */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_LO_SRC_ADDR_LO_MASK                 (0xFFFFFFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_LO_SRC_ADDR_LO_SHIFT                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_LO_SRC_ADDR_LO_RESETVAL             (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_LO_SRC_ADDR_LO_MAX                  (0xFFFFFFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_LO_RESETVAL                         (0x00000000U)

/* SRC_ADDR_HI */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_ASEL_MASK                        (0x000F0000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_ASEL_SHIFT                       (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_ASEL_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_ASEL_MAX                         (0x0000000FU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_SRC_ADDR_HI_MASK                 (0x0000FFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_SRC_ADDR_HI_SHIFT                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_SRC_ADDR_HI_RESETVAL             (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_SRC_ADDR_HI_MAX                  (0x0000FFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRC_ADDR_HI_RESETVAL                         (0x00000000U)

/* SRCDIM1 */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRCDIM1_DIM1_MASK                            (0xFFFFFFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRCDIM1_DIM1_SHIFT                           (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRCDIM1_DIM1_RESETVAL                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRCDIM1_DIM1_MAX                             (0xFFFFFFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_SRCDIM1_RESETVAL                             (0x00000000U)

/* FMTFLAGS */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FMTFLAGS_FMTFLAGS_MASK                       (0xFFFFFFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FMTFLAGS_FMTFLAGS_SHIFT                      (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FMTFLAGS_FMTFLAGS_RESETVAL                   (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_FMTFLAGS_FMTFLAGS_MAX                        (0xFFFFFFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_FMTFLAGS_RESETVAL                            (0x00000000U)

/* DSTDIM1 */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DSTDIM1_DDIM1_MASK                           (0xFFFFFFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DSTDIM1_DDIM1_SHIFT                          (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DSTDIM1_DDIM1_RESETVAL                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DSTDIM1_DDIM1_MAX                            (0xFFFFFFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DSTDIM1_RESETVAL                             (0x00000000U)

/* DST_ADDR_LO */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_LO_DST_ADDR_LO_MASK                 (0xFFFFFFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_LO_DST_ADDR_LO_SHIFT                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_LO_DST_ADDR_LO_RESETVAL             (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_LO_DST_ADDR_LO_MAX                  (0xFFFFFFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_LO_RESETVAL                         (0x00000000U)

/* DST_ADDR_HI */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_ASEL_MASK                        (0x000F0000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_ASEL_SHIFT                       (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_ASEL_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_ASEL_MAX                         (0x0000000FU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_DST_ADDR_HI_MASK                 (0x0000FFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_DST_ADDR_HI_SHIFT                (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_DST_ADDR_HI_RESETVAL             (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_DST_ADDR_HI_MAX                  (0x0000FFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DST_ADDR_HI_RESETVAL                         (0x00000000U)

/* DICNT0_1 */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT0_MASK                         (0x0000FFFFU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT0_SHIFT                        (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT0_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT0_MAX                          (0x0000FFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT1_MASK                         (0xFFFF0000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT1_SHIFT                        (0x00000010U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT1_RESETVAL                     (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_DICNT1_MAX                          (0x0000FFFFU)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_DICNT0_1_RESETVAL                            (0x00000000U)

/* TR_SUBMIT */

#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_SUBMIT_MASK                        (0x00000001U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_SUBMIT_SHIFT                       (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_SUBMIT_RESETVAL                    (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_SUBMIT_MAX                         (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_RESUBMIT_MASK                      (0x80000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_RESUBMIT_SHIFT                     (0x0000001FU)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_RESUBMIT_RESETVAL                  (0x00000000U)
#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_RESUBMIT_MAX                       (0x00000001U)

#define CSL_XLCDMA_FLOWRT_V2_FLOW_TR_SUBMIT_RESETVAL                           (0x00000000U)

/**************************************************************************
* Hardware Region  : Ring Credentials Registers
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t CRED;                      /* Credentials Register */
    volatile uint8_t  Resv_16[12];
} CSL_xlcdma_cred_v2Regs_ring;


typedef struct {
    CSL_xlcdma_cred_v2Regs_ring RING[11];
} CSL_xlcdma_cred_v2Regs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_XLCDMA_CRED_V2_RING_CRED(RING)                                     (0x00000000U+((RING)*0x10U))

/**************************************************************************
* 64K Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t CRED;                      /* Credentials Register */
    volatile uint8_t  Resv_16[12];
} CSL_xlcdma_cred_v2Regs64_ring;


typedef struct {
    CSL_xlcdma_cred_v2Regs64_ring RING[11];
} CSL_xlcdma_cred_v2Regs64;


/**************************************************************************
* 64K Register Macros
**************************************************************************/

#define CSL_XLCDMA_CRED_V2_64_RING_CRED(RING)                                  (0x00000000U+((RING)*0x10U))

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* CRED */

#define CSL_XLCDMA_CRED_V2_RING_CRED_SECURE_MASK                               (0x04000000U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_SECURE_SHIFT                              (0x0000001AU)
#define CSL_XLCDMA_CRED_V2_RING_CRED_SECURE_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_SECURE_MAX                                (0x00000001U)

#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIV_MASK                                 (0x03000000U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIV_SHIFT                                (0x00000018U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIV_RESETVAL                             (0x00000000U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIV_MAX                                  (0x00000003U)

#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIVID_MASK                               (0x00FF0000U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIVID_SHIFT                              (0x00000010U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIVID_RESETVAL                           (0x00000000U)
#define CSL_XLCDMA_CRED_V2_RING_CRED_PRIVID_MAX                                (0x000000FFU)

#define CSL_XLCDMA_CRED_V2_RING_CRED_RESETVAL                                  (0x00000000U)

#ifdef __cplusplus
}
#endif
#endif
