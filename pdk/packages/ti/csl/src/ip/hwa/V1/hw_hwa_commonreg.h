/**
*  @file    hw_hwa_commonreg.h
*
*  @brief
*    This file gives register definitions of DSS_HW_ACC module.
*
*
*
*/

/*
*   (C) Copyright 2019 - 2020, Texas Instruments Incorporated. - TI web address www.ti.com
*---------------------------------------------------------------------------------------
*
*  Redistribution and use in source and binary forms, with or without modification,
*  are permitted provided that the following conditions are met:
*
*    Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of its
*    contributors may be used to endorse or promote products derived from this
*    software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
*  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
*  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR CONTRIBUTORS
*  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef HW_HWA_COMMONREG_V1_H
#define HW_HWA_COMMONREG_V1_H

/****************************************************************************************
* INCLUDE FILES
****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/* Definition for field PID_MSB16 in Register PID */
#define    PID_MSB16_START                                                   (16U)
#define    PID_MSB16_END                                                     (31U)

/* Definition for field PID_MISC in Register PID */
#define    PID_MISC_START                                                    (11U)
#define    PID_MISC_END                                                      (15U)

/* Definition for field PID_MAJOR in Register PID */
#define    PID_MAJOR_START                                                   (8U)
#define    PID_MAJOR_END                                                     (10U)

/* Definition for field PID_CUSTOM in Register PID */
#define    PID_CUSTOM_START                                                  (6U)
#define    PID_CUSTOM_END                                                    (7U)

/* Definition for field PID_MINOR in Register PID */
#define    PID_MINOR_START                                                   (0U)
#define    PID_MINOR_END                                                     (5U)

/* Definition for field PARAM_RAM_IDX_PARAM_END_IDX in Register PARAM_RAM_IDX */
#define    PARAM_RAM_IDX_PARAM_END_IDX_START                                 (16U)
#define    PARAM_RAM_IDX_PARAM_END_IDX_END                                   (25U)

/* Definition for field PARAM_RAM_IDX_PARAM_START_IDX in Register PARAM_RAM_IDX */
#define    PARAM_RAM_IDX_PARAM_START_IDX_START                               (0U)
#define    PARAM_RAM_IDX_PARAM_START_IDX_END                                 (9U)

/* Definition for field PARAM_RAM_LOOP_NUMLOOPS in Register PARAM_RAM_LOOP */
#define    PARAM_RAM_LOOP_NUMLOOPS_START                                     (0U)
#define    PARAM_RAM_LOOP_NUMLOOPS_END                                       (11U)

/* Definition for field PARAM_RAM_IDX_ALT_PARAM_END_IDX in Register PARAM_RAM_IDX_ALT */
#define    PARAM_RAM_IDX_ALT_PARAM_END_IDX_START                             (16U)
#define    PARAM_RAM_IDX_ALT_PARAM_END_IDX_END                               (25U)

/* Definition for field PARAM_RAM_IDX_ALT_PARAM_START_IDX in Register PARAM_RAM_IDX_ALT */
#define    PARAM_RAM_IDX_ALT_PARAM_START_IDX_START                           (0U)
#define    PARAM_RAM_IDX_ALT_PARAM_START_IDX_END                             (9U)

/* Definition for field PARAM_RAM_LOOP_ALT_NUMLOOPS in Register PARAM_RAM_LOOP_ALT */
#define    PARAM_RAM_LOOP_ALT_NUMLOOPS_START                                 (0U)
#define    PARAM_RAM_LOOP_ALT_NUMLOOPS_END                                   (11U)

/* Definition for field HWA_ENABLE_HWA_DYN_CLK_EN in Register HWA_ENABLE */
#define    HWA_ENABLE_HWA_DYN_CLK_EN_START                                   (24U)
#define    HWA_ENABLE_HWA_DYN_CLK_EN_END                                     (24U)

/* Definition for field HWA_ENABLE_HWA_RESET in Register HWA_ENABLE */
#define    HWA_ENABLE_HWA_RESET_START                                        (16U)
#define    HWA_ENABLE_HWA_RESET_END                                          (18U)

/* Definition for field HWA_ENABLE_HWA_CLK_EN in Register HWA_ENABLE */
#define    HWA_ENABLE_HWA_CLK_EN_START                                       (8U)
#define    HWA_ENABLE_HWA_CLK_EN_END                                         (10U)

/* Definition for field HWA_ENABLE_HWA_EN in Register HWA_ENABLE */
#define    HWA_ENABLE_HWA_EN_START                                           (0U)
#define    HWA_ENABLE_HWA_EN_END                                             (2U)

/* Definition for field CS_CONFIG_CS_TRGSRC in Register CS_CONFIG */
#define    CS_CONFIG_CS_TRGSRC_START                                         (16U)
#define    CS_CONFIG_CS_TRGSRC_END                                           (20U)

/* Definition for field CS_CONFIG_CS_TRIGMODE in Register CS_CONFIG */
#define    CS_CONFIG_CS_TRIGMODE_START                                       (8U)
#define    CS_CONFIG_CS_TRIGMODE_END                                         (11U)

/* Definition for field CS_CONFIG_CS_ENABLE in Register CS_CONFIG */
#define    CS_CONFIG_CS_ENABLE_START                                         (0U)
#define    CS_CONFIG_CS_ENABLE_END                                           (0U)

/* Definition for field FW2DMA_TRIG_FW2DMA_TRIGGER in Register FW2DMA_TRIG */
#define    FW2DMA_TRIG_FW2DMA_TRIGGER_START                                  (0U)
#define    FW2DMA_TRIG_FW2DMA_TRIGGER_END                                    (31U)

/* Definition for field DMA2HWA_TRIG_DMA2HWA_TRIGGER in Register DMA2HWA_TRIG */
#define    DMA2HWA_TRIG_DMA2HWA_TRIGGER_START                                (0U)
#define    DMA2HWA_TRIG_DMA2HWA_TRIGGER_END                                  (31U)

/* Definition for field SIGDMACH0DONE_SIGDMACH0DONE in Register SIGDMACH0DONE */
#define    SIGDMACH0DONE_SIGDMACH0DONE_START                                 (0U)
#define    SIGDMACH0DONE_SIGDMACH0DONE_END                                   (31U)

/* Definition for field SIGDMACH1DONE_SIGDMACH1DONE in Register SIGDMACH1DONE */
#define    SIGDMACH1DONE_SIGDMACH1DONE_START                                 (0U)
#define    SIGDMACH1DONE_SIGDMACH1DONE_END                                   (31U)

/* Definition for field SIGDMACH2DONE_SIGDMACH2DONE in Register SIGDMACH2DONE */
#define    SIGDMACH2DONE_SIGDMACH2DONE_START                                 (0U)
#define    SIGDMACH2DONE_SIGDMACH2DONE_END                                   (31U)

/* Definition for field SIGDMACH3DONE_SIGDMACH3DONE in Register SIGDMACH3DONE */
#define    SIGDMACH3DONE_SIGDMACH3DONE_START                                 (0U)
#define    SIGDMACH3DONE_SIGDMACH3DONE_END                                   (31U)

/* Definition for field SIGDMACH4DONE_SIGDMACH4DONE in Register SIGDMACH4DONE */
#define    SIGDMACH4DONE_SIGDMACH4DONE_START                                 (0U)
#define    SIGDMACH4DONE_SIGDMACH4DONE_END                                   (31U)

/* Definition for field SIGDMACH5DONE_SIGDMACH5DONE in Register SIGDMACH5DONE */
#define    SIGDMACH5DONE_SIGDMACH5DONE_START                                 (0U)
#define    SIGDMACH5DONE_SIGDMACH5DONE_END                                   (31U)

/* Definition for field SIGDMACH6DONE_SIGDMACH6DONE in Register SIGDMACH6DONE */
#define    SIGDMACH6DONE_SIGDMACH6DONE_START                                 (0U)
#define    SIGDMACH6DONE_SIGDMACH6DONE_END                                   (31U)

/* Definition for field SIGDMACH7DONE_SIGDMACH7DONE in Register SIGDMACH7DONE */
#define    SIGDMACH7DONE_SIGDMACH7DONE_START                                 (0U)
#define    SIGDMACH7DONE_SIGDMACH7DONE_END                                   (31U)

/* Definition for field SIGDMACH8DONE_SIGDMACH8DONE in Register SIGDMACH8DONE */
#define    SIGDMACH8DONE_SIGDMACH8DONE_START                                 (0U)
#define    SIGDMACH8DONE_SIGDMACH8DONE_END                                   (31U)

/* Definition for field SIGDMACH9DONE_SIGDMACH9DONE in Register SIGDMACH9DONE */
#define    SIGDMACH9DONE_SIGDMACH9DONE_START                                 (0U)
#define    SIGDMACH9DONE_SIGDMACH9DONE_END                                   (31U)

/* Definition for field SIGDMACH10DONE_SIGDMACH10DONE in Register SIGDMACH10DONE */
#define    SIGDMACH10DONE_SIGDMACH10DONE_START                               (0U)
#define    SIGDMACH10DONE_SIGDMACH10DONE_END                                 (31U)

/* Definition for field SIGDMACH11DONE_SIGDMACH11DONE in Register SIGDMACH11DONE */
#define    SIGDMACH11DONE_SIGDMACH11DONE_START                               (0U)
#define    SIGDMACH11DONE_SIGDMACH11DONE_END                                 (31U)

/* Definition for field SIGDMACH12DONE_SIGDMACH12DONE in Register SIGDMACH12DONE */
#define    SIGDMACH12DONE_SIGDMACH12DONE_START                               (0U)
#define    SIGDMACH12DONE_SIGDMACH12DONE_END                                 (31U)

/* Definition for field SIGDMACH13DONE_SIGDMACH13DONE in Register SIGDMACH13DONE */
#define    SIGDMACH13DONE_SIGDMACH13DONE_START                               (0U)
#define    SIGDMACH13DONE_SIGDMACH13DONE_END                                 (31U)

/* Definition for field SIGDMACH14DONE_SIGDMACH14DONE in Register SIGDMACH14DONE */
#define    SIGDMACH14DONE_SIGDMACH14DONE_START                               (0U)
#define    SIGDMACH14DONE_SIGDMACH14DONE_END                                 (31U)

/* Definition for field SIGDMACH15DONE_SIGDMACH15DONE in Register SIGDMACH15DONE */
#define    SIGDMACH15DONE_SIGDMACH15DONE_START                               (0U)
#define    SIGDMACH15DONE_SIGDMACH15DONE_END                                 (31U)

/* Definition for field SIGDMACH16DONE_SIGDMACH16DONE in Register SIGDMACH16DONE */
#define    SIGDMACH16DONE_SIGDMACH16DONE_START                               (0U)
#define    SIGDMACH16DONE_SIGDMACH16DONE_END                                 (31U)

/* Definition for field SIGDMACH17DONE_SIGDMACH17DONE in Register SIGDMACH17DONE */
#define    SIGDMACH17DONE_SIGDMACH17DONE_START                               (0U)
#define    SIGDMACH17DONE_SIGDMACH17DONE_END                                 (31U)

/* Definition for field SIGDMACH18DONE_SIGDMACH18DONE in Register SIGDMACH18DONE */
#define    SIGDMACH18DONE_SIGDMACH18DONE_START                               (0U)
#define    SIGDMACH18DONE_SIGDMACH18DONE_END                                 (31U)

/* Definition for field SIGDMACH19DONE_SIGDMACH19DONE in Register SIGDMACH19DONE */
#define    SIGDMACH19DONE_SIGDMACH19DONE_START                               (0U)
#define    SIGDMACH19DONE_SIGDMACH19DONE_END                                 (31U)

/* Definition for field SIGDMACH20DONE_SIGDMACH20DONE in Register SIGDMACH20DONE */
#define    SIGDMACH20DONE_SIGDMACH20DONE_START                               (0U)
#define    SIGDMACH20DONE_SIGDMACH20DONE_END                                 (31U)

/* Definition for field SIGDMACH21DONE_SIGDMACH21DONE in Register SIGDMACH21DONE */
#define    SIGDMACH21DONE_SIGDMACH21DONE_START                               (0U)
#define    SIGDMACH21DONE_SIGDMACH21DONE_END                                 (31U)

/* Definition for field SIGDMACH22DONE_SIGDMACH22DONE in Register SIGDMACH22DONE */
#define    SIGDMACH22DONE_SIGDMACH22DONE_START                               (0U)
#define    SIGDMACH22DONE_SIGDMACH22DONE_END                                 (31U)

/* Definition for field SIGDMACH23DONE_SIGDMACH23DONE in Register SIGDMACH23DONE */
#define    SIGDMACH23DONE_SIGDMACH23DONE_START                               (0U)
#define    SIGDMACH23DONE_SIGDMACH23DONE_END                                 (31U)

/* Definition for field SIGDMACH24DONE_SIGDMACH24DONE in Register SIGDMACH24DONE */
#define    SIGDMACH24DONE_SIGDMACH24DONE_START                               (0U)
#define    SIGDMACH24DONE_SIGDMACH24DONE_END                                 (31U)

/* Definition for field SIGDMACH25DONE_SIGDMACH25DONE in Register SIGDMACH25DONE */
#define    SIGDMACH25DONE_SIGDMACH25DONE_START                               (0U)
#define    SIGDMACH25DONE_SIGDMACH25DONE_END                                 (31U)

/* Definition for field SIGDMACH26DONE_SIGDMACH26DONE in Register SIGDMACH26DONE */
#define    SIGDMACH26DONE_SIGDMACH26DONE_START                               (0U)
#define    SIGDMACH26DONE_SIGDMACH26DONE_END                                 (31U)

/* Definition for field SIGDMACH27DONE_SIGDMACH27DONE in Register SIGDMACH27DONE */
#define    SIGDMACH27DONE_SIGDMACH27DONE_START                               (0U)
#define    SIGDMACH27DONE_SIGDMACH27DONE_END                                 (31U)

/* Definition for field SIGDMACH28DONE_SIGDMACH28DONE in Register SIGDMACH28DONE */
#define    SIGDMACH28DONE_SIGDMACH28DONE_START                               (0U)
#define    SIGDMACH28DONE_SIGDMACH28DONE_END                                 (31U)

/* Definition for field SIGDMACH29DONE_SIGDMACH29DONE in Register SIGDMACH29DONE */
#define    SIGDMACH29DONE_SIGDMACH29DONE_START                               (0U)
#define    SIGDMACH29DONE_SIGDMACH29DONE_END                                 (31U)

/* Definition for field SIGDMACH30DONE_SIGDMACH30DONE in Register SIGDMACH30DONE */
#define    SIGDMACH30DONE_SIGDMACH30DONE_START                               (0U)
#define    SIGDMACH30DONE_SIGDMACH30DONE_END                                 (31U)

/* Definition for field SIGDMACH31DONE_SIGDMACH31DONE in Register SIGDMACH31DONE */
#define    SIGDMACH31DONE_SIGDMACH31DONE_START                               (0U)
#define    SIGDMACH31DONE_SIGDMACH31DONE_END                                 (31U)

/* Definition for field FW2HWA_TRIG_0_FW2HWA_TRIGGER_0 in Register FW2HWA_TRIG_0 */
#define    FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START                              (0U)
#define    FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END                                (0U)

/* Definition for field FW2HWA_TRIG_1_FW2HWA_TRIGGER_1 in Register FW2HWA_TRIG_1 */
#define    FW2HWA_TRIG_1_FW2HWA_TRIGGER_1_START                              (0U)
#define    FW2HWA_TRIG_1_FW2HWA_TRIGGER_1_END                                (0U)

/* Definition for field CS_FW2ACC_TRIG_FW2HWA_TRIGGER_CS in Register CS_FW2ACC_TRIG */
#define    CS_FW2ACC_TRIG_FW2HWA_TRIGGER_CS_START                            (0U)
#define    CS_FW2ACC_TRIG_FW2HWA_TRIGGER_CS_END                              (0U)

/* Definition for field BPM_PATTERN_0_BPM_PATTERN_0 in Register BPM_PATTERN_0 */
#define    BPM_PATTERN_0_BPM_PATTERN_0_START                                 (0U)
#define    BPM_PATTERN_0_BPM_PATTERN_0_END                                   (31U)

/* Definition for field BPM_PATTERN_1_BPM_PATTERN_1 in Register BPM_PATTERN_1 */
#define    BPM_PATTERN_1_BPM_PATTERN_1_START                                 (0U)
#define    BPM_PATTERN_1_BPM_PATTERN_1_END                                   (31U)

/* Definition for field BPM_PATTERN_2_BPM_PATTERN_2 in Register BPM_PATTERN_2 */
#define    BPM_PATTERN_2_BPM_PATTERN_2_START                                 (0U)
#define    BPM_PATTERN_2_BPM_PATTERN_2_END                                   (31U)

/* Definition for field BPM_PATTERN_3_BPM_PATTERN_3 in Register BPM_PATTERN_3 */
#define    BPM_PATTERN_3_BPM_PATTERN_3_START                                 (0U)
#define    BPM_PATTERN_3_BPM_PATTERN_3_END                                   (31U)

/* Definition for field BPM_PATTERN_4_BPM_PATTERN_4 in Register BPM_PATTERN_4 */
#define    BPM_PATTERN_4_BPM_PATTERN_4_START                                 (0U)
#define    BPM_PATTERN_4_BPM_PATTERN_4_END                                   (31U)

/* Definition for field BPM_PATTERN_5_BPM_PATTERN_5 in Register BPM_PATTERN_5 */
#define    BPM_PATTERN_5_BPM_PATTERN_5_START                                 (0U)
#define    BPM_PATTERN_5_BPM_PATTERN_5_END                                   (31U)

/* Definition for field BPM_PATTERN_6_BPM_PATTERN_6 in Register BPM_PATTERN_6 */
#define    BPM_PATTERN_6_BPM_PATTERN_6_START                                 (0U)
#define    BPM_PATTERN_6_BPM_PATTERN_6_END                                   (31U)

/* Definition for field BPM_PATTERN_7_BPM_PATTERN_7 in Register BPM_PATTERN_7 */
#define    BPM_PATTERN_7_BPM_PATTERN_7_START                                 (0U)
#define    BPM_PATTERN_7_BPM_PATTERN_7_END                                   (31U)

/* Definition for field BPM_RATE_BPM_RATE in Register BPM_RATE */
#define    BPM_RATE_BPM_RATE_START                                           (0U)
#define    BPM_RATE_BPM_RATE_END                                             (9U)

/* Definition for field PARAM_DONE_SET_STATUS_0_PARAM_DONE_SET_STATUS_0 in Register PARAM_DONE_SET_STATUS_0 */
#define    PARAM_DONE_SET_STATUS_0_PARAM_DONE_SET_STATUS_0_START             (0U)
#define    PARAM_DONE_SET_STATUS_0_PARAM_DONE_SET_STATUS_0_END               (31U)

/* Definition for field PARAM_DONE_SET_STATUS_1_PARAM_DONE_SET_STATUS_1 in Register PARAM_DONE_SET_STATUS_1 */
#define    PARAM_DONE_SET_STATUS_1_PARAM_DONE_SET_STATUS_1_START             (0U)
#define    PARAM_DONE_SET_STATUS_1_PARAM_DONE_SET_STATUS_1_END               (31U)

/* Definition for field PARAM_DONE_CLR_0_PARAM_DONE_STATUS_CLR_0 in Register PARAM_DONE_CLR_0 */
#define    PARAM_DONE_CLR_0_PARAM_DONE_STATUS_CLR_0_START                    (0U)
#define    PARAM_DONE_CLR_0_PARAM_DONE_STATUS_CLR_0_END                      (31U)

/* Definition for field PARAM_DONE_CLR_1_PARAM_DONE_STATUS_CLR_1 in Register PARAM_DONE_CLR_1 */
#define    PARAM_DONE_CLR_1_PARAM_DONE_STATUS_CLR_1_START                    (0U)
#define    PARAM_DONE_CLR_1_PARAM_DONE_STATUS_CLR_1_END                      (31U)

/* Definition for field TRIGGER_SET_STATUS_0_TRIGGER_SET_STATUS_0 in Register TRIGGER_SET_STATUS_0 */
#define    TRIGGER_SET_STATUS_0_TRIGGER_SET_STATUS_0_START                   (0U)
#define    TRIGGER_SET_STATUS_0_TRIGGER_SET_STATUS_0_END                     (31U)

/* Definition for field TRIGGER_SET_STATUS_1_TRIGGER_SET_STATUS_1 in Register TRIGGER_SET_STATUS_1 */
#define    TRIGGER_SET_STATUS_1_TRIGGER_SET_STATUS_1_START                   (0U)
#define    TRIGGER_SET_STATUS_1_TRIGGER_SET_STATUS_1_END                     (31U)

/* Definition for field TRIGGER_SET_IN_CLR_0_TRIGGER_SET_IN_CLR_0 in Register TRIGGER_SET_IN_CLR_0 */
#define    TRIGGER_SET_IN_CLR_0_TRIGGER_SET_IN_CLR_0_START                   (0U)
#define    TRIGGER_SET_IN_CLR_0_TRIGGER_SET_IN_CLR_0_END                     (0U)

/* Definition for field TRIGGER_SET_IN_CLR_1_TRIGGER_SET_IN_CLR_1 in Register TRIGGER_SET_IN_CLR_1 */
#define    TRIGGER_SET_IN_CLR_1_TRIGGER_SET_IN_CLR_1_START                   (0U)
#define    TRIGGER_SET_IN_CLR_1_TRIGGER_SET_IN_CLR_1_END                     (0U)

/* Definition for field DC_EST_RESET_SW_DC_EST_RESET_SW in Register DC_EST_RESET_SW */
#define    DC_EST_RESET_SW_DC_EST_RESET_SW_START                             (0U)
#define    DC_EST_RESET_SW_DC_EST_RESET_SW_END                               (0U)

/* Definition for field DC_EST_CTRL_DC_EST_SHIFT in Register DC_EST_CTRL */
#define    DC_EST_CTRL_DC_EST_SHIFT_START                                    (16U)
#define    DC_EST_CTRL_DC_EST_SHIFT_END                                      (19U)

/* Definition for field DC_EST_CTRL_DC_EST_SCALE in Register DC_EST_CTRL */
#define    DC_EST_CTRL_DC_EST_SCALE_START                                    (0U)
#define    DC_EST_CTRL_DC_EST_SCALE_END                                      (8U)

/* Definition for field DC_EST_I_0_VAL_DC_EST_I_0_VAL in Register DC_EST_I_0_VAL */
#define    DC_EST_I_0_VAL_DC_EST_I_0_VAL_START                               (0U)
#define    DC_EST_I_0_VAL_DC_EST_I_0_VAL_END                                 (23U)

/* Definition for field DC_EST_I_1_VAL_DC_EST_I_1_VAL in Register DC_EST_I_1_VAL */
#define    DC_EST_I_1_VAL_DC_EST_I_1_VAL_START                               (0U)
#define    DC_EST_I_1_VAL_DC_EST_I_1_VAL_END                                 (23U)

/* Definition for field DC_EST_I_2_VAL_DC_EST_I_2_VAL in Register DC_EST_I_2_VAL */
#define    DC_EST_I_2_VAL_DC_EST_I_2_VAL_START                               (0U)
#define    DC_EST_I_2_VAL_DC_EST_I_2_VAL_END                                 (23U)

/* Definition for field DC_EST_I_3_VAL_DC_EST_I_3_VAL in Register DC_EST_I_3_VAL */
#define    DC_EST_I_3_VAL_DC_EST_I_3_VAL_START                               (0U)
#define    DC_EST_I_3_VAL_DC_EST_I_3_VAL_END                                 (23U)

/* Definition for field DC_EST_I_4_VAL_DC_EST_I_4_VAL in Register DC_EST_I_4_VAL */
#define    DC_EST_I_4_VAL_DC_EST_I_4_VAL_START                               (0U)
#define    DC_EST_I_4_VAL_DC_EST_I_4_VAL_END                                 (23U)

/* Definition for field DC_EST_I_5_VAL_DC_EST_I_5_VAL in Register DC_EST_I_5_VAL */
#define    DC_EST_I_5_VAL_DC_EST_I_5_VAL_START                               (0U)
#define    DC_EST_I_5_VAL_DC_EST_I_5_VAL_END                                 (23U)

/* Definition for field DC_EST_I_6_VAL_DC_EST_I_6_VAL in Register DC_EST_I_6_VAL */
#define    DC_EST_I_6_VAL_DC_EST_I_6_VAL_START                               (0U)
#define    DC_EST_I_6_VAL_DC_EST_I_6_VAL_END                                 (23U)

/* Definition for field DC_EST_I_7_VAL_DC_EST_I_7_VAL in Register DC_EST_I_7_VAL */
#define    DC_EST_I_7_VAL_DC_EST_I_7_VAL_START                               (0U)
#define    DC_EST_I_7_VAL_DC_EST_I_7_VAL_END                                 (23U)

/* Definition for field DC_EST_I_8_VAL_DC_EST_I_8_VAL in Register DC_EST_I_8_VAL */
#define    DC_EST_I_8_VAL_DC_EST_I_8_VAL_START                               (0U)
#define    DC_EST_I_8_VAL_DC_EST_I_8_VAL_END                                 (23U)

/* Definition for field DC_EST_I_9_VAL_DC_EST_I_9_VAL in Register DC_EST_I_9_VAL */
#define    DC_EST_I_9_VAL_DC_EST_I_9_VAL_START                               (0U)
#define    DC_EST_I_9_VAL_DC_EST_I_9_VAL_END                                 (23U)

/* Definition for field DC_EST_I_10_VAL_DC_EST_I_10_VAL in Register DC_EST_I_10_VAL */
#define    DC_EST_I_10_VAL_DC_EST_I_10_VAL_START                             (0U)
#define    DC_EST_I_10_VAL_DC_EST_I_10_VAL_END                               (23U)

/* Definition for field DC_EST_I_11_VAL_DC_EST_I_11_VAL in Register DC_EST_I_11_VAL */
#define    DC_EST_I_11_VAL_DC_EST_I_11_VAL_START                             (0U)
#define    DC_EST_I_11_VAL_DC_EST_I_11_VAL_END                               (23U)

/* Definition for field DC_EST_Q_0_VAL_DC_EST_Q_0_VAL in Register DC_EST_Q_0_VAL */
#define    DC_EST_Q_0_VAL_DC_EST_Q_0_VAL_START                               (0U)
#define    DC_EST_Q_0_VAL_DC_EST_Q_0_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_1_VAL_DC_EST_Q_1_VAL in Register DC_EST_Q_1_VAL */
#define    DC_EST_Q_1_VAL_DC_EST_Q_1_VAL_START                               (0U)
#define    DC_EST_Q_1_VAL_DC_EST_Q_1_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_2_VAL_DC_EST_Q_2_VAL in Register DC_EST_Q_2_VAL */
#define    DC_EST_Q_2_VAL_DC_EST_Q_2_VAL_START                               (0U)
#define    DC_EST_Q_2_VAL_DC_EST_Q_2_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_3_VAL_DC_EST_Q_3_VAL in Register DC_EST_Q_3_VAL */
#define    DC_EST_Q_3_VAL_DC_EST_Q_3_VAL_START                               (0U)
#define    DC_EST_Q_3_VAL_DC_EST_Q_3_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_4_VAL_DC_EST_Q_4_VAL in Register DC_EST_Q_4_VAL */
#define    DC_EST_Q_4_VAL_DC_EST_Q_4_VAL_START                               (0U)
#define    DC_EST_Q_4_VAL_DC_EST_Q_4_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_5_VAL_DC_EST_Q_5_VAL in Register DC_EST_Q_5_VAL */
#define    DC_EST_Q_5_VAL_DC_EST_Q_5_VAL_START                               (0U)
#define    DC_EST_Q_5_VAL_DC_EST_Q_5_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_6_VAL_DC_EST_Q_6_VAL in Register DC_EST_Q_6_VAL */
#define    DC_EST_Q_6_VAL_DC_EST_Q_6_VAL_START                               (0U)
#define    DC_EST_Q_6_VAL_DC_EST_Q_6_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_7_VAL_DC_EST_Q_7_VAL in Register DC_EST_Q_7_VAL */
#define    DC_EST_Q_7_VAL_DC_EST_Q_7_VAL_START                               (0U)
#define    DC_EST_Q_7_VAL_DC_EST_Q_7_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_8_VAL_DC_EST_Q_8_VAL in Register DC_EST_Q_8_VAL */
#define    DC_EST_Q_8_VAL_DC_EST_Q_8_VAL_START                               (0U)
#define    DC_EST_Q_8_VAL_DC_EST_Q_8_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_9_VAL_DC_EST_Q_9_VAL in Register DC_EST_Q_9_VAL */
#define    DC_EST_Q_9_VAL_DC_EST_Q_9_VAL_START                               (0U)
#define    DC_EST_Q_9_VAL_DC_EST_Q_9_VAL_END                                 (23U)

/* Definition for field DC_EST_Q_10_VAL_DC_EST_Q_10_VAL in Register DC_EST_Q_10_VAL */
#define    DC_EST_Q_10_VAL_DC_EST_Q_10_VAL_START                             (0U)
#define    DC_EST_Q_10_VAL_DC_EST_Q_10_VAL_END                               (23U)

/* Definition for field DC_EST_Q_11_VAL_DC_EST_Q_11_VAL in Register DC_EST_Q_11_VAL */
#define    DC_EST_Q_11_VAL_DC_EST_Q_11_VAL_START                             (0U)
#define    DC_EST_Q_11_VAL_DC_EST_Q_11_VAL_END                               (23U)

/* Definition for field DC_ACC_I_0_VAL_LSB_DC_ACC_I_0_VAL_LSB in Register DC_ACC_I_0_VAL_LSB */
#define    DC_ACC_I_0_VAL_LSB_DC_ACC_I_0_VAL_LSB_START                       (0U)
#define    DC_ACC_I_0_VAL_LSB_DC_ACC_I_0_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_0_VAL_MSB_DC_ACC_I_0_VAL_MSB in Register DC_ACC_I_0_VAL_MSB */
#define    DC_ACC_I_0_VAL_MSB_DC_ACC_I_0_VAL_MSB_START                       (0U)
#define    DC_ACC_I_0_VAL_MSB_DC_ACC_I_0_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_1_VAL_LSB_DC_ACC_I_1_VAL_LSB in Register DC_ACC_I_1_VAL_LSB */
#define    DC_ACC_I_1_VAL_LSB_DC_ACC_I_1_VAL_LSB_START                       (0U)
#define    DC_ACC_I_1_VAL_LSB_DC_ACC_I_1_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_1_VAL_MSB_DC_ACC_I_1_VAL_MSB in Register DC_ACC_I_1_VAL_MSB */
#define    DC_ACC_I_1_VAL_MSB_DC_ACC_I_1_VAL_MSB_START                       (0U)
#define    DC_ACC_I_1_VAL_MSB_DC_ACC_I_1_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_2_VAL_LSB_DC_ACC_I_2_VAL_LSB in Register DC_ACC_I_2_VAL_LSB */
#define    DC_ACC_I_2_VAL_LSB_DC_ACC_I_2_VAL_LSB_START                       (0U)
#define    DC_ACC_I_2_VAL_LSB_DC_ACC_I_2_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_2_VAL_MSB_DC_ACC_I_2_VAL_MSB in Register DC_ACC_I_2_VAL_MSB */
#define    DC_ACC_I_2_VAL_MSB_DC_ACC_I_2_VAL_MSB_START                       (0U)
#define    DC_ACC_I_2_VAL_MSB_DC_ACC_I_2_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_3_VAL_LSB_DC_ACC_I_3_VAL_LSB in Register DC_ACC_I_3_VAL_LSB */
#define    DC_ACC_I_3_VAL_LSB_DC_ACC_I_3_VAL_LSB_START                       (0U)
#define    DC_ACC_I_3_VAL_LSB_DC_ACC_I_3_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_3_VAL_MSB_DC_ACC_I_3_VAL_MSB in Register DC_ACC_I_3_VAL_MSB */
#define    DC_ACC_I_3_VAL_MSB_DC_ACC_I_3_VAL_MSB_START                       (0U)
#define    DC_ACC_I_3_VAL_MSB_DC_ACC_I_3_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_4_VAL_LSB_DC_ACC_I_4_VAL_LSB in Register DC_ACC_I_4_VAL_LSB */
#define    DC_ACC_I_4_VAL_LSB_DC_ACC_I_4_VAL_LSB_START                       (0U)
#define    DC_ACC_I_4_VAL_LSB_DC_ACC_I_4_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_4_VAL_MSB_DC_ACC_I_4_VAL_MSB in Register DC_ACC_I_4_VAL_MSB */
#define    DC_ACC_I_4_VAL_MSB_DC_ACC_I_4_VAL_MSB_START                       (0U)
#define    DC_ACC_I_4_VAL_MSB_DC_ACC_I_4_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_5_VAL_LSB_DC_ACC_I_5_VAL_LSB in Register DC_ACC_I_5_VAL_LSB */
#define    DC_ACC_I_5_VAL_LSB_DC_ACC_I_5_VAL_LSB_START                       (0U)
#define    DC_ACC_I_5_VAL_LSB_DC_ACC_I_5_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_5_VAL_MSB_DC_ACC_I_5_VAL_MSB in Register DC_ACC_I_5_VAL_MSB */
#define    DC_ACC_I_5_VAL_MSB_DC_ACC_I_5_VAL_MSB_START                       (0U)
#define    DC_ACC_I_5_VAL_MSB_DC_ACC_I_5_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_6_VAL_LSB_DC_ACC_I_6_VAL_LSB in Register DC_ACC_I_6_VAL_LSB */
#define    DC_ACC_I_6_VAL_LSB_DC_ACC_I_6_VAL_LSB_START                       (0U)
#define    DC_ACC_I_6_VAL_LSB_DC_ACC_I_6_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_6_VAL_MSB_DC_ACC_I_6_VAL_MSB in Register DC_ACC_I_6_VAL_MSB */
#define    DC_ACC_I_6_VAL_MSB_DC_ACC_I_6_VAL_MSB_START                       (0U)
#define    DC_ACC_I_6_VAL_MSB_DC_ACC_I_6_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_7_VAL_LSB_DC_ACC_I_7_VAL_LSB in Register DC_ACC_I_7_VAL_LSB */
#define    DC_ACC_I_7_VAL_LSB_DC_ACC_I_7_VAL_LSB_START                       (0U)
#define    DC_ACC_I_7_VAL_LSB_DC_ACC_I_7_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_7_VAL_MSB_DC_ACC_I_7_VAL_MSB in Register DC_ACC_I_7_VAL_MSB */
#define    DC_ACC_I_7_VAL_MSB_DC_ACC_I_7_VAL_MSB_START                       (0U)
#define    DC_ACC_I_7_VAL_MSB_DC_ACC_I_7_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_8_VAL_LSB_DC_ACC_I_8_VAL_LSB in Register DC_ACC_I_8_VAL_LSB */
#define    DC_ACC_I_8_VAL_LSB_DC_ACC_I_8_VAL_LSB_START                       (0U)
#define    DC_ACC_I_8_VAL_LSB_DC_ACC_I_8_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_8_VAL_MSB_DC_ACC_I_8_VAL_MSB in Register DC_ACC_I_8_VAL_MSB */
#define    DC_ACC_I_8_VAL_MSB_DC_ACC_I_8_VAL_MSB_START                       (0U)
#define    DC_ACC_I_8_VAL_MSB_DC_ACC_I_8_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_9_VAL_LSB_DC_ACC_I_9_VAL_LSB in Register DC_ACC_I_9_VAL_LSB */
#define    DC_ACC_I_9_VAL_LSB_DC_ACC_I_9_VAL_LSB_START                       (0U)
#define    DC_ACC_I_9_VAL_LSB_DC_ACC_I_9_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_I_9_VAL_MSB_DC_ACC_I_9_VAL_MSB in Register DC_ACC_I_9_VAL_MSB */
#define    DC_ACC_I_9_VAL_MSB_DC_ACC_I_9_VAL_MSB_START                       (0U)
#define    DC_ACC_I_9_VAL_MSB_DC_ACC_I_9_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_I_10_VAL_LSB_DC_ACC_I_10_VAL_LSB in Register DC_ACC_I_10_VAL_LSB */
#define    DC_ACC_I_10_VAL_LSB_DC_ACC_I_10_VAL_LSB_START                     (0U)
#define    DC_ACC_I_10_VAL_LSB_DC_ACC_I_10_VAL_LSB_END                       (31U)

/* Definition for field DC_ACC_I_10_VAL_MSB_DC_ACC_I_10_VAL_MSB in Register DC_ACC_I_10_VAL_MSB */
#define    DC_ACC_I_10_VAL_MSB_DC_ACC_I_10_VAL_MSB_START                     (0U)
#define    DC_ACC_I_10_VAL_MSB_DC_ACC_I_10_VAL_MSB_END                       (3U)

/* Definition for field DC_ACC_I_11_VAL_LSB_DC_ACC_I_11_VAL_LSB in Register DC_ACC_I_11_VAL_LSB */
#define    DC_ACC_I_11_VAL_LSB_DC_ACC_I_11_VAL_LSB_START                     (0U)
#define    DC_ACC_I_11_VAL_LSB_DC_ACC_I_11_VAL_LSB_END                       (31U)

/* Definition for field DC_ACC_I_11_VAL_MSB_DC_ACC_I_11_VAL_MSB in Register DC_ACC_I_11_VAL_MSB */
#define    DC_ACC_I_11_VAL_MSB_DC_ACC_I_11_VAL_MSB_START                     (0U)
#define    DC_ACC_I_11_VAL_MSB_DC_ACC_I_11_VAL_MSB_END                       (3U)

/* Definition for field DC_ACC_Q_0_VAL_LSB_DC_ACC_Q_0_VAL_LSB in Register DC_ACC_Q_0_VAL_LSB */
#define    DC_ACC_Q_0_VAL_LSB_DC_ACC_Q_0_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_0_VAL_LSB_DC_ACC_Q_0_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_0_VAL_MSB_DC_ACC_Q_0_VAL_MSB in Register DC_ACC_Q_0_VAL_MSB */
#define    DC_ACC_Q_0_VAL_MSB_DC_ACC_Q_0_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_0_VAL_MSB_DC_ACC_Q_0_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_1_VAL_LSB_DC_ACC_Q_1_VAL_LSB in Register DC_ACC_Q_1_VAL_LSB */
#define    DC_ACC_Q_1_VAL_LSB_DC_ACC_Q_1_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_1_VAL_LSB_DC_ACC_Q_1_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_1_VAL_MSB_DC_ACC_Q_1_VAL_MSB in Register DC_ACC_Q_1_VAL_MSB */
#define    DC_ACC_Q_1_VAL_MSB_DC_ACC_Q_1_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_1_VAL_MSB_DC_ACC_Q_1_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_2_VAL_LSB_DC_ACC_Q_2_VAL_LSB in Register DC_ACC_Q_2_VAL_LSB */
#define    DC_ACC_Q_2_VAL_LSB_DC_ACC_Q_2_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_2_VAL_LSB_DC_ACC_Q_2_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_2_VAL_MSB_DC_ACC_Q_2_VAL_MSB in Register DC_ACC_Q_2_VAL_MSB */
#define    DC_ACC_Q_2_VAL_MSB_DC_ACC_Q_2_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_2_VAL_MSB_DC_ACC_Q_2_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_3_VAL_LSB_DC_ACC_Q_3_VAL_LSB in Register DC_ACC_Q_3_VAL_LSB */
#define    DC_ACC_Q_3_VAL_LSB_DC_ACC_Q_3_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_3_VAL_LSB_DC_ACC_Q_3_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_3_VAL_MSB_DC_ACC_Q_3_VAL_MSB in Register DC_ACC_Q_3_VAL_MSB */
#define    DC_ACC_Q_3_VAL_MSB_DC_ACC_Q_3_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_3_VAL_MSB_DC_ACC_Q_3_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_4_VAL_LSB_DC_ACC_Q_4_VAL_LSB in Register DC_ACC_Q_4_VAL_LSB */
#define    DC_ACC_Q_4_VAL_LSB_DC_ACC_Q_4_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_4_VAL_LSB_DC_ACC_Q_4_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_4_VAL_MSB_DC_ACC_Q_4_VAL_MSB in Register DC_ACC_Q_4_VAL_MSB */
#define    DC_ACC_Q_4_VAL_MSB_DC_ACC_Q_4_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_4_VAL_MSB_DC_ACC_Q_4_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_5_VAL_LSB_DC_ACC_Q_5_VAL_LSB in Register DC_ACC_Q_5_VAL_LSB */
#define    DC_ACC_Q_5_VAL_LSB_DC_ACC_Q_5_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_5_VAL_LSB_DC_ACC_Q_5_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_5_VAL_MSB_DC_ACC_Q_5_VAL_MSB in Register DC_ACC_Q_5_VAL_MSB */
#define    DC_ACC_Q_5_VAL_MSB_DC_ACC_Q_5_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_5_VAL_MSB_DC_ACC_Q_5_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_6_VAL_LSB_DC_ACC_Q_6_VAL_LSB in Register DC_ACC_Q_6_VAL_LSB */
#define    DC_ACC_Q_6_VAL_LSB_DC_ACC_Q_6_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_6_VAL_LSB_DC_ACC_Q_6_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_6_VAL_MSB_DC_ACC_Q_6_VAL_MSB in Register DC_ACC_Q_6_VAL_MSB */
#define    DC_ACC_Q_6_VAL_MSB_DC_ACC_Q_6_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_6_VAL_MSB_DC_ACC_Q_6_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_7_VAL_LSB_DC_ACC_Q_7_VAL_LSB in Register DC_ACC_Q_7_VAL_LSB */
#define    DC_ACC_Q_7_VAL_LSB_DC_ACC_Q_7_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_7_VAL_LSB_DC_ACC_Q_7_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_7_VAL_MSB_DC_ACC_Q_7_VAL_MSB in Register DC_ACC_Q_7_VAL_MSB */
#define    DC_ACC_Q_7_VAL_MSB_DC_ACC_Q_7_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_7_VAL_MSB_DC_ACC_Q_7_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_8_VAL_LSB_DC_ACC_Q_8_VAL_LSB in Register DC_ACC_Q_8_VAL_LSB */
#define    DC_ACC_Q_8_VAL_LSB_DC_ACC_Q_8_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_8_VAL_LSB_DC_ACC_Q_8_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_8_VAL_MSB_DC_ACC_Q_8_VAL_MSB in Register DC_ACC_Q_8_VAL_MSB */
#define    DC_ACC_Q_8_VAL_MSB_DC_ACC_Q_8_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_8_VAL_MSB_DC_ACC_Q_8_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_9_VAL_LSB_DC_ACC_Q_9_VAL_LSB in Register DC_ACC_Q_9_VAL_LSB */
#define    DC_ACC_Q_9_VAL_LSB_DC_ACC_Q_9_VAL_LSB_START                       (0U)
#define    DC_ACC_Q_9_VAL_LSB_DC_ACC_Q_9_VAL_LSB_END                         (31U)

/* Definition for field DC_ACC_Q_9_VAL_MSB_DC_ACC_Q_9_VAL_MSB in Register DC_ACC_Q_9_VAL_MSB */
#define    DC_ACC_Q_9_VAL_MSB_DC_ACC_Q_9_VAL_MSB_START                       (0U)
#define    DC_ACC_Q_9_VAL_MSB_DC_ACC_Q_9_VAL_MSB_END                         (3U)

/* Definition for field DC_ACC_Q_10_VAL_LSB_DC_ACC_Q_10_VAL_LSB in Register DC_ACC_Q_10_VAL_LSB */
#define    DC_ACC_Q_10_VAL_LSB_DC_ACC_Q_10_VAL_LSB_START                     (0U)
#define    DC_ACC_Q_10_VAL_LSB_DC_ACC_Q_10_VAL_LSB_END                       (31U)

/* Definition for field DC_ACC_Q_10_VAL_MSB_DC_ACC_Q_10_VAL_MSB in Register DC_ACC_Q_10_VAL_MSB */
#define    DC_ACC_Q_10_VAL_MSB_DC_ACC_Q_10_VAL_MSB_START                     (0U)
#define    DC_ACC_Q_10_VAL_MSB_DC_ACC_Q_10_VAL_MSB_END                       (3U)

/* Definition for field DC_ACC_Q_11_VAL_LSB_DC_ACC_Q_11_VAL_LSB in Register DC_ACC_Q_11_VAL_LSB */
#define    DC_ACC_Q_11_VAL_LSB_DC_ACC_Q_11_VAL_LSB_START                     (0U)
#define    DC_ACC_Q_11_VAL_LSB_DC_ACC_Q_11_VAL_LSB_END                       (31U)

/* Definition for field DC_ACC_Q_11_VAL_MSB_DC_ACC_Q_11_VAL_MSB in Register DC_ACC_Q_11_VAL_MSB */
#define    DC_ACC_Q_11_VAL_MSB_DC_ACC_Q_11_VAL_MSB_START                     (0U)
#define    DC_ACC_Q_11_VAL_MSB_DC_ACC_Q_11_VAL_MSB_END                       (3U)

/* Definition for field DC_ACC_CLIP_STATUS_DC_ACC_CLIP_STATUS in Register DC_ACC_CLIP_STATUS */
#define    DC_ACC_CLIP_STATUS_DC_ACC_CLIP_STATUS_START                       (0U)
#define    DC_ACC_CLIP_STATUS_DC_ACC_CLIP_STATUS_END                         (11U)

/* Definition for field DC_EST_CLIP_STATUS_DC_EST_CLIP_STATUS in Register DC_EST_CLIP_STATUS */
#define    DC_EST_CLIP_STATUS_DC_EST_CLIP_STATUS_START                       (0U)
#define    DC_EST_CLIP_STATUS_DC_EST_CLIP_STATUS_END                         (11U)

/* Definition for field DC_I0_SW_DC_I0_SW in Register DC_I0_SW */
#define    DC_I0_SW_DC_I0_SW_START                                           (0U)
#define    DC_I0_SW_DC_I0_SW_END                                             (23U)

/* Definition for field DC_I1_SW_DC_I1_SW in Register DC_I1_SW */
#define    DC_I1_SW_DC_I1_SW_START                                           (0U)
#define    DC_I1_SW_DC_I1_SW_END                                             (23U)

/* Definition for field DC_I2_SW_DC_I2_SW in Register DC_I2_SW */
#define    DC_I2_SW_DC_I2_SW_START                                           (0U)
#define    DC_I2_SW_DC_I2_SW_END                                             (23U)

/* Definition for field DC_I3_SW_DC_I3_SW in Register DC_I3_SW */
#define    DC_I3_SW_DC_I3_SW_START                                           (0U)
#define    DC_I3_SW_DC_I3_SW_END                                             (23U)

/* Definition for field DC_I4_SW_DC_I4_SW in Register DC_I4_SW */
#define    DC_I4_SW_DC_I4_SW_START                                           (0U)
#define    DC_I4_SW_DC_I4_SW_END                                             (23U)

/* Definition for field DC_I5_SW_DC_I5_SW in Register DC_I5_SW */
#define    DC_I5_SW_DC_I5_SW_START                                           (0U)
#define    DC_I5_SW_DC_I5_SW_END                                             (23U)

/* Definition for field DC_I6_SW_DC_I6_SW in Register DC_I6_SW */
#define    DC_I6_SW_DC_I6_SW_START                                           (0U)
#define    DC_I6_SW_DC_I6_SW_END                                             (23U)

/* Definition for field DC_I7_SW_DC_I7_SW in Register DC_I7_SW */
#define    DC_I7_SW_DC_I7_SW_START                                           (0U)
#define    DC_I7_SW_DC_I7_SW_END                                             (23U)

/* Definition for field DC_I8_SW_DC_I8_SW in Register DC_I8_SW */
#define    DC_I8_SW_DC_I8_SW_START                                           (0U)
#define    DC_I8_SW_DC_I8_SW_END                                             (23U)

/* Definition for field DC_I9_SW_DC_I9_SW in Register DC_I9_SW */
#define    DC_I9_SW_DC_I9_SW_START                                           (0U)
#define    DC_I9_SW_DC_I9_SW_END                                             (23U)

/* Definition for field DC_I10_SW_DC_I10_SW in Register DC_I10_SW */
#define    DC_I10_SW_DC_I10_SW_START                                         (0U)
#define    DC_I10_SW_DC_I10_SW_END                                           (23U)

/* Definition for field DC_I11_SW_DC_I11_SW in Register DC_I11_SW */
#define    DC_I11_SW_DC_I11_SW_START                                         (0U)
#define    DC_I11_SW_DC_I11_SW_END                                           (23U)

/* Definition for field DC_Q0_SW_DC_Q0_SW in Register DC_Q0_SW */
#define    DC_Q0_SW_DC_Q0_SW_START                                           (0U)
#define    DC_Q0_SW_DC_Q0_SW_END                                             (23U)

/* Definition for field DC_Q1_SW_DC_Q1_SW in Register DC_Q1_SW */
#define    DC_Q1_SW_DC_Q1_SW_START                                           (0U)
#define    DC_Q1_SW_DC_Q1_SW_END                                             (23U)

/* Definition for field DC_Q2_SW_DC_Q2_SW in Register DC_Q2_SW */
#define    DC_Q2_SW_DC_Q2_SW_START                                           (0U)
#define    DC_Q2_SW_DC_Q2_SW_END                                             (23U)

/* Definition for field DC_Q3_SW_DC_Q3_SW in Register DC_Q3_SW */
#define    DC_Q3_SW_DC_Q3_SW_START                                           (0U)
#define    DC_Q3_SW_DC_Q3_SW_END                                             (23U)

/* Definition for field DC_Q4_SW_DC_Q4_SW in Register DC_Q4_SW */
#define    DC_Q4_SW_DC_Q4_SW_START                                           (0U)
#define    DC_Q4_SW_DC_Q4_SW_END                                             (23U)

/* Definition for field DC_Q5_SW_DC_Q5_SW in Register DC_Q5_SW */
#define    DC_Q5_SW_DC_Q5_SW_START                                           (0U)
#define    DC_Q5_SW_DC_Q5_SW_END                                             (23U)

/* Definition for field DC_Q6_SW_DC_Q6_SW in Register DC_Q6_SW */
#define    DC_Q6_SW_DC_Q6_SW_START                                           (0U)
#define    DC_Q6_SW_DC_Q6_SW_END                                             (23U)

/* Definition for field DC_Q7_SW_DC_Q7_SW in Register DC_Q7_SW */
#define    DC_Q7_SW_DC_Q7_SW_START                                           (0U)
#define    DC_Q7_SW_DC_Q7_SW_END                                             (23U)

/* Definition for field DC_Q8_SW_DC_Q8_SW in Register DC_Q8_SW */
#define    DC_Q8_SW_DC_Q8_SW_START                                           (0U)
#define    DC_Q8_SW_DC_Q8_SW_END                                             (23U)

/* Definition for field DC_Q9_SW_DC_Q9_SW in Register DC_Q9_SW */
#define    DC_Q9_SW_DC_Q9_SW_START                                           (0U)
#define    DC_Q9_SW_DC_Q9_SW_END                                             (23U)

/* Definition for field DC_Q10_SW_DC_Q10_SW in Register DC_Q10_SW */
#define    DC_Q10_SW_DC_Q10_SW_START                                         (0U)
#define    DC_Q10_SW_DC_Q10_SW_END                                           (23U)

/* Definition for field DC_Q11_SW_DC_Q11_SW in Register DC_Q11_SW */
#define    DC_Q11_SW_DC_Q11_SW_START                                         (0U)
#define    DC_Q11_SW_DC_Q11_SW_END                                           (23U)

/* Definition for field DC_SUB_CLIP_DC_SUB_CLIP in Register DC_SUB_CLIP */
#define    DC_SUB_CLIP_DC_SUB_CLIP_START                                     (0U)
#define    DC_SUB_CLIP_DC_SUB_CLIP_END                                       (0U)

/* Definition for field DC_RESERVED_2_DC_SUB_RESERVED_2 in Register DC_RESERVED_2 */
#define    DC_RESERVED_2_DC_SUB_RESERVED_2_START                             (0U)
#define    DC_RESERVED_2_DC_SUB_RESERVED_2_END                               (31U)

/* Definition for field DC_RESERVED_3_DC_SUB_RESERVED_3 in Register DC_RESERVED_3 */
#define    DC_RESERVED_3_DC_SUB_RESERVED_3_START                             (0U)
#define    DC_RESERVED_3_DC_SUB_RESERVED_3_END                               (31U)

/* Definition for field DC_RESERVED_4_DC_SUB_RESERVED_4 in Register DC_RESERVED_4 */
#define    DC_RESERVED_4_DC_SUB_RESERVED_4_START                             (0U)
#define    DC_RESERVED_4_DC_SUB_RESERVED_4_END                               (31U)

/* Definition for field DC_RESERVED_5_DC_SUB_RESERVED_5 in Register DC_RESERVED_5 */
#define    DC_RESERVED_5_DC_SUB_RESERVED_5_START                             (0U)
#define    DC_RESERVED_5_DC_SUB_RESERVED_5_END                               (31U)

/* Definition for field INTF_STATS_RESET_SW_INTF_STATS_RESET_SW in Register INTF_STATS_RESET_SW */
#define    INTF_STATS_RESET_SW_INTF_STATS_RESET_SW_START                     (0U)
#define    INTF_STATS_RESET_SW_INTF_STATS_RESET_SW_END                       (0U)

/* Definition for field INTF_STATS_CTRL_INTF_STATS_MAGDIFF_SCALE in Register INTF_STATS_CTRL */
#define    INTF_STATS_CTRL_INTF_STATS_MAGDIFF_SCALE_START                    (24U)
#define    INTF_STATS_CTRL_INTF_STATS_MAGDIFF_SCALE_END                      (31U)

/* Definition for field INTF_STATS_CTRL_INTF_STATS_MAG_SCALE in Register INTF_STATS_CTRL */
#define    INTF_STATS_CTRL_INTF_STATS_MAG_SCALE_START                        (16U)
#define    INTF_STATS_CTRL_INTF_STATS_MAG_SCALE_END                          (23U)

/* Definition for field INTF_STATS_CTRL_INTF_STATS_MAGDIFF_SHIFT in Register INTF_STATS_CTRL */
#define    INTF_STATS_CTRL_INTF_STATS_MAGDIFF_SHIFT_START                    (4U)
#define    INTF_STATS_CTRL_INTF_STATS_MAGDIFF_SHIFT_END                      (6U)

/* Definition for field INTF_STATS_CTRL_INTF_STATS_MAG_SHIFT in Register INTF_STATS_CTRL */
#define    INTF_STATS_CTRL_INTF_STATS_MAG_SHIFT_START                        (0U)
#define    INTF_STATS_CTRL_INTF_STATS_MAG_SHIFT_END                          (2U)

/* Definition for field INTF_LOC_THRESH_MAG0_VAL_INTF_LOC_THRESH_MAG0_VAL in Register INTF_LOC_THRESH_MAG0_VAL */
#define    INTF_LOC_THRESH_MAG0_VAL_INTF_LOC_THRESH_MAG0_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG0_VAL_INTF_LOC_THRESH_MAG0_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG1_VAL_INTF_LOC_THRESH_MAG1_VAL in Register INTF_LOC_THRESH_MAG1_VAL */
#define    INTF_LOC_THRESH_MAG1_VAL_INTF_LOC_THRESH_MAG1_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG1_VAL_INTF_LOC_THRESH_MAG1_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG2_VAL_INTF_LOC_THRESH_MAG2_VAL in Register INTF_LOC_THRESH_MAG2_VAL */
#define    INTF_LOC_THRESH_MAG2_VAL_INTF_LOC_THRESH_MAG2_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG2_VAL_INTF_LOC_THRESH_MAG2_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG3_VAL_INTF_LOC_THRESH_MAG3_VAL in Register INTF_LOC_THRESH_MAG3_VAL */
#define    INTF_LOC_THRESH_MAG3_VAL_INTF_LOC_THRESH_MAG3_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG3_VAL_INTF_LOC_THRESH_MAG3_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG4_VAL_INTF_LOC_THRESH_MAG4_VAL in Register INTF_LOC_THRESH_MAG4_VAL */
#define    INTF_LOC_THRESH_MAG4_VAL_INTF_LOC_THRESH_MAG4_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG4_VAL_INTF_LOC_THRESH_MAG4_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG5_VAL_INTF_LOC_THRESH_MAG5_VAL in Register INTF_LOC_THRESH_MAG5_VAL */
#define    INTF_LOC_THRESH_MAG5_VAL_INTF_LOC_THRESH_MAG5_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG5_VAL_INTF_LOC_THRESH_MAG5_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG6_VAL_INTF_LOC_THRESH_MAG6_VAL in Register INTF_LOC_THRESH_MAG6_VAL */
#define    INTF_LOC_THRESH_MAG6_VAL_INTF_LOC_THRESH_MAG6_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG6_VAL_INTF_LOC_THRESH_MAG6_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG7_VAL_INTF_LOC_THRESH_MAG7_VAL in Register INTF_LOC_THRESH_MAG7_VAL */
#define    INTF_LOC_THRESH_MAG7_VAL_INTF_LOC_THRESH_MAG7_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG7_VAL_INTF_LOC_THRESH_MAG7_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG8_VAL_INTF_LOC_THRESH_MAG8_VAL in Register INTF_LOC_THRESH_MAG8_VAL */
#define    INTF_LOC_THRESH_MAG8_VAL_INTF_LOC_THRESH_MAG8_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG8_VAL_INTF_LOC_THRESH_MAG8_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG9_VAL_INTF_LOC_THRESH_MAG9_VAL in Register INTF_LOC_THRESH_MAG9_VAL */
#define    INTF_LOC_THRESH_MAG9_VAL_INTF_LOC_THRESH_MAG9_VAL_START           (0U)
#define    INTF_LOC_THRESH_MAG9_VAL_INTF_LOC_THRESH_MAG9_VAL_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG10_VAL_INTF_LOC_THRESH_MAG10_VAL in Register INTF_LOC_THRESH_MAG10_VAL */
#define    INTF_LOC_THRESH_MAG10_VAL_INTF_LOC_THRESH_MAG10_VAL_START         (0U)
#define    INTF_LOC_THRESH_MAG10_VAL_INTF_LOC_THRESH_MAG10_VAL_END           (23U)

/* Definition for field INTF_LOC_THRESH_MAG11_VAL_INTF_LOC_THRESH_MAG11_VAL in Register INTF_LOC_THRESH_MAG11_VAL */
#define    INTF_LOC_THRESH_MAG11_VAL_INTF_LOC_THRESH_MAG11_VAL_START         (0U)
#define    INTF_LOC_THRESH_MAG11_VAL_INTF_LOC_THRESH_MAG11_VAL_END           (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF0_VAL_INTF_LOC_THRESH_MAGDIFF0_VAL in Register INTF_LOC_THRESH_MAGDIFF0_VAL */
#define    INTF_LOC_THRESH_MAGDIFF0_VAL_INTF_LOC_THRESH_MAGDIFF0_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF0_VAL_INTF_LOC_THRESH_MAGDIFF0_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF1_VAL_INTF_LOC_THRESH_MAGDIFF1_VAL in Register INTF_LOC_THRESH_MAGDIFF1_VAL */
#define    INTF_LOC_THRESH_MAGDIFF1_VAL_INTF_LOC_THRESH_MAGDIFF1_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF1_VAL_INTF_LOC_THRESH_MAGDIFF1_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF2_VAL_INTF_LOC_THRESH_MAGDIFF2_VAL in Register INTF_LOC_THRESH_MAGDIFF2_VAL */
#define    INTF_LOC_THRESH_MAGDIFF2_VAL_INTF_LOC_THRESH_MAGDIFF2_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF2_VAL_INTF_LOC_THRESH_MAGDIFF2_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF3_VAL_INTF_LOC_THRESH_MAGDIFF3_VAL in Register INTF_LOC_THRESH_MAGDIFF3_VAL */
#define    INTF_LOC_THRESH_MAGDIFF3_VAL_INTF_LOC_THRESH_MAGDIFF3_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF3_VAL_INTF_LOC_THRESH_MAGDIFF3_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF4_VAL_INTF_LOC_THRESH_MAGDIFF4_VAL in Register INTF_LOC_THRESH_MAGDIFF4_VAL */
#define    INTF_LOC_THRESH_MAGDIFF4_VAL_INTF_LOC_THRESH_MAGDIFF4_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF4_VAL_INTF_LOC_THRESH_MAGDIFF4_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF5_VAL_INTF_LOC_THRESH_MAGDIFF5_VAL in Register INTF_LOC_THRESH_MAGDIFF5_VAL */
#define    INTF_LOC_THRESH_MAGDIFF5_VAL_INTF_LOC_THRESH_MAGDIFF5_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF5_VAL_INTF_LOC_THRESH_MAGDIFF5_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF6_VAL_INTF_LOC_THRESH_MAGDIFF6_VAL in Register INTF_LOC_THRESH_MAGDIFF6_VAL */
#define    INTF_LOC_THRESH_MAGDIFF6_VAL_INTF_LOC_THRESH_MAGDIFF6_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF6_VAL_INTF_LOC_THRESH_MAGDIFF6_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF7_VAL_INTF_LOC_THRESH_MAGDIFF7_VAL in Register INTF_LOC_THRESH_MAGDIFF7_VAL */
#define    INTF_LOC_THRESH_MAGDIFF7_VAL_INTF_LOC_THRESH_MAGDIFF7_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF7_VAL_INTF_LOC_THRESH_MAGDIFF7_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF8_VAL_INTF_LOC_THRESH_MAGDIFF8_VAL in Register INTF_LOC_THRESH_MAGDIFF8_VAL */
#define    INTF_LOC_THRESH_MAGDIFF8_VAL_INTF_LOC_THRESH_MAGDIFF8_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF8_VAL_INTF_LOC_THRESH_MAGDIFF8_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF9_VAL_INTF_LOC_THRESH_MAGDIFF9_VAL in Register INTF_LOC_THRESH_MAGDIFF9_VAL */
#define    INTF_LOC_THRESH_MAGDIFF9_VAL_INTF_LOC_THRESH_MAGDIFF9_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF9_VAL_INTF_LOC_THRESH_MAGDIFF9_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF10_VAL_INTF_LOC_THRESH_MAGDIFF10_VAL in Register INTF_LOC_THRESH_MAGDIFF10_VAL */
#define    INTF_LOC_THRESH_MAGDIFF10_VAL_INTF_LOC_THRESH_MAGDIFF10_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF10_VAL_INTF_LOC_THRESH_MAGDIFF10_VAL_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF11_VAL_INTF_LOC_THRESH_MAGDIFF11_VAL in Register INTF_LOC_THRESH_MAGDIFF11_VAL */
#define    INTF_LOC_THRESH_MAGDIFF11_VAL_INTF_LOC_THRESH_MAGDIFF11_VAL_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF11_VAL_INTF_LOC_THRESH_MAGDIFF11_VAL_END      (23U)

/* Definition for field INTF_LOC_COUNT_ALL_CHIRP_INTF_LOC_COUNT_ALL_CHIRP in Register INTF_LOC_COUNT_ALL_CHIRP */
#define    INTF_LOC_COUNT_ALL_CHIRP_INTF_LOC_COUNT_ALL_CHIRP_START           (0U)
#define    INTF_LOC_COUNT_ALL_CHIRP_INTF_LOC_COUNT_ALL_CHIRP_END             (11U)

/* Definition for field INTF_LOC_COUNT_ALL_FRAME_INTF_LOC_COUNT_ALL_FRAME in Register INTF_LOC_COUNT_ALL_FRAME */
#define    INTF_LOC_COUNT_ALL_FRAME_INTF_LOC_COUNT_ALL_FRAME_START           (0U)
#define    INTF_LOC_COUNT_ALL_FRAME_INTF_LOC_COUNT_ALL_FRAME_END             (19U)

/* Definition for field INTF_STATS_MAG_ACC_0_LSB_INTF_STATS_MAG_ACC_0_LSB in Register INTF_STATS_MAG_ACC_0_LSB */
#define    INTF_STATS_MAG_ACC_0_LSB_INTF_STATS_MAG_ACC_0_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_0_LSB_INTF_STATS_MAG_ACC_0_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_0_MSB_INTF_STATS_MAG_ACC_0_MSB in Register INTF_STATS_MAG_ACC_0_MSB */
#define    INTF_STATS_MAG_ACC_0_MSB_INTF_STATS_MAG_ACC_0_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_0_MSB_INTF_STATS_MAG_ACC_0_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_1_LSB_INTF_STATS_MAG_ACC_1_LSB in Register INTF_STATS_MAG_ACC_1_LSB */
#define    INTF_STATS_MAG_ACC_1_LSB_INTF_STATS_MAG_ACC_1_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_1_LSB_INTF_STATS_MAG_ACC_1_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_1_MSB_INTF_STATS_MAG_ACC_1_MSB in Register INTF_STATS_MAG_ACC_1_MSB */
#define    INTF_STATS_MAG_ACC_1_MSB_INTF_STATS_MAG_ACC_1_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_1_MSB_INTF_STATS_MAG_ACC_1_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_2_LSB_INTF_STATS_MAG_ACC_2_LSB in Register INTF_STATS_MAG_ACC_2_LSB */
#define    INTF_STATS_MAG_ACC_2_LSB_INTF_STATS_MAG_ACC_2_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_2_LSB_INTF_STATS_MAG_ACC_2_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_2_MSB_INTF_STATS_MAG_ACC_2_MSB in Register INTF_STATS_MAG_ACC_2_MSB */
#define    INTF_STATS_MAG_ACC_2_MSB_INTF_STATS_MAG_ACC_2_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_2_MSB_INTF_STATS_MAG_ACC_2_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_3_LSB_INTF_STATS_MAG_ACC_3_LSB in Register INTF_STATS_MAG_ACC_3_LSB */
#define    INTF_STATS_MAG_ACC_3_LSB_INTF_STATS_MAG_ACC_3_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_3_LSB_INTF_STATS_MAG_ACC_3_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_3_MSB_INTF_STATS_MAG_ACC_3_MSB in Register INTF_STATS_MAG_ACC_3_MSB */
#define    INTF_STATS_MAG_ACC_3_MSB_INTF_STATS_MAG_ACC_3_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_3_MSB_INTF_STATS_MAG_ACC_3_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_4_LSB_INTF_STATS_MAG_ACC_4_LSB in Register INTF_STATS_MAG_ACC_4_LSB */
#define    INTF_STATS_MAG_ACC_4_LSB_INTF_STATS_MAG_ACC_4_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_4_LSB_INTF_STATS_MAG_ACC_4_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_4_MSB_INTF_STATS_MAG_ACC_4_MSB in Register INTF_STATS_MAG_ACC_4_MSB */
#define    INTF_STATS_MAG_ACC_4_MSB_INTF_STATS_MAG_ACC_4_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_4_MSB_INTF_STATS_MAG_ACC_4_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_5_LSB_INTF_STATS_MAG_ACC_5_LSB in Register INTF_STATS_MAG_ACC_5_LSB */
#define    INTF_STATS_MAG_ACC_5_LSB_INTF_STATS_MAG_ACC_5_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_5_LSB_INTF_STATS_MAG_ACC_5_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_5_MSB_INTF_STATS_MAG_ACC_5_MSB in Register INTF_STATS_MAG_ACC_5_MSB */
#define    INTF_STATS_MAG_ACC_5_MSB_INTF_STATS_MAG_ACC_5_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_5_MSB_INTF_STATS_MAG_ACC_5_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_6_LSB_INTF_STATS_MAG_ACC_6_LSB in Register INTF_STATS_MAG_ACC_6_LSB */
#define    INTF_STATS_MAG_ACC_6_LSB_INTF_STATS_MAG_ACC_6_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_6_LSB_INTF_STATS_MAG_ACC_6_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_6_MSB_INTF_STATS_MAG_ACC_6_MSB in Register INTF_STATS_MAG_ACC_6_MSB */
#define    INTF_STATS_MAG_ACC_6_MSB_INTF_STATS_MAG_ACC_6_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_6_MSB_INTF_STATS_MAG_ACC_6_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_7_LSB_INTF_STATS_MAG_ACC_7_LSB in Register INTF_STATS_MAG_ACC_7_LSB */
#define    INTF_STATS_MAG_ACC_7_LSB_INTF_STATS_MAG_ACC_7_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_7_LSB_INTF_STATS_MAG_ACC_7_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_7_MSB_INTF_STATS_MAG_ACC_7_MSB in Register INTF_STATS_MAG_ACC_7_MSB */
#define    INTF_STATS_MAG_ACC_7_MSB_INTF_STATS_MAG_ACC_7_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_7_MSB_INTF_STATS_MAG_ACC_7_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_8_LSB_INTF_STATS_MAG_ACC_8_LSB in Register INTF_STATS_MAG_ACC_8_LSB */
#define    INTF_STATS_MAG_ACC_8_LSB_INTF_STATS_MAG_ACC_8_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_8_LSB_INTF_STATS_MAG_ACC_8_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_8_MSB_INTF_STATS_MAG_ACC_8_MSB in Register INTF_STATS_MAG_ACC_8_MSB */
#define    INTF_STATS_MAG_ACC_8_MSB_INTF_STATS_MAG_ACC_8_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_8_MSB_INTF_STATS_MAG_ACC_8_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_9_LSB_INTF_STATS_MAG_ACC_9_LSB in Register INTF_STATS_MAG_ACC_9_LSB */
#define    INTF_STATS_MAG_ACC_9_LSB_INTF_STATS_MAG_ACC_9_LSB_START           (0U)
#define    INTF_STATS_MAG_ACC_9_LSB_INTF_STATS_MAG_ACC_9_LSB_END             (31U)

/* Definition for field INTF_STATS_MAG_ACC_9_MSB_INTF_STATS_MAG_ACC_9_MSB in Register INTF_STATS_MAG_ACC_9_MSB */
#define    INTF_STATS_MAG_ACC_9_MSB_INTF_STATS_MAG_ACC_9_MSB_START           (0U)
#define    INTF_STATS_MAG_ACC_9_MSB_INTF_STATS_MAG_ACC_9_MSB_END             (3U)

/* Definition for field INTF_STATS_MAG_ACC_10_LSB_INTF_STATS_MAG_ACC_10_LSB in Register INTF_STATS_MAG_ACC_10_LSB */
#define    INTF_STATS_MAG_ACC_10_LSB_INTF_STATS_MAG_ACC_10_LSB_START         (0U)
#define    INTF_STATS_MAG_ACC_10_LSB_INTF_STATS_MAG_ACC_10_LSB_END           (31U)

/* Definition for field INTF_STATS_MAG_ACC_10_MSB_INTF_STATS_MAG_ACC_10_MSB in Register INTF_STATS_MAG_ACC_10_MSB */
#define    INTF_STATS_MAG_ACC_10_MSB_INTF_STATS_MAG_ACC_10_MSB_START         (0U)
#define    INTF_STATS_MAG_ACC_10_MSB_INTF_STATS_MAG_ACC_10_MSB_END           (3U)

/* Definition for field INTF_STATS_MAG_ACC_11_LSB_INTF_STATS_MAG_ACC_11_LSB in Register INTF_STATS_MAG_ACC_11_LSB */
#define    INTF_STATS_MAG_ACC_11_LSB_INTF_STATS_MAG_ACC_11_LSB_START         (0U)
#define    INTF_STATS_MAG_ACC_11_LSB_INTF_STATS_MAG_ACC_11_LSB_END           (31U)

/* Definition for field INTF_STATS_MAG_ACC_11_MSB_INTF_STATS_MAG_ACC_11_MSB in Register INTF_STATS_MAG_ACC_11_MSB */
#define    INTF_STATS_MAG_ACC_11_MSB_INTF_STATS_MAG_ACC_11_MSB_START         (0U)
#define    INTF_STATS_MAG_ACC_11_MSB_INTF_STATS_MAG_ACC_11_MSB_END           (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_0_LSB_INTF_STATS_MAGDIFF_ACC_0_LSB in Register INTF_STATS_MAGDIFF_ACC_0_LSB */
#define    INTF_STATS_MAGDIFF_ACC_0_LSB_INTF_STATS_MAGDIFF_ACC_0_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_0_LSB_INTF_STATS_MAGDIFF_ACC_0_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_0_MSB_INTF_STATS_MAGDIFF_ACC_0_MSB in Register INTF_STATS_MAGDIFF_ACC_0_MSB */
#define    INTF_STATS_MAGDIFF_ACC_0_MSB_INTF_STATS_MAGDIFF_ACC_0_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_0_MSB_INTF_STATS_MAGDIFF_ACC_0_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_1_LSB_INTF_STATS_MAGDIFF_ACC_1_LSB in Register INTF_STATS_MAGDIFF_ACC_1_LSB */
#define    INTF_STATS_MAGDIFF_ACC_1_LSB_INTF_STATS_MAGDIFF_ACC_1_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_1_LSB_INTF_STATS_MAGDIFF_ACC_1_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_1_MSB_INTF_STATS_MAGDIFF_ACC_1_MSB in Register INTF_STATS_MAGDIFF_ACC_1_MSB */
#define    INTF_STATS_MAGDIFF_ACC_1_MSB_INTF_STATS_MAGDIFF_ACC_1_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_1_MSB_INTF_STATS_MAGDIFF_ACC_1_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_2_LSB_INTF_STATS_MAGDIFF_ACC_2_LSB in Register INTF_STATS_MAGDIFF_ACC_2_LSB */
#define    INTF_STATS_MAGDIFF_ACC_2_LSB_INTF_STATS_MAGDIFF_ACC_2_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_2_LSB_INTF_STATS_MAGDIFF_ACC_2_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_2_MSB_INTF_STATS_MAGDIFF_ACC_2_MSB in Register INTF_STATS_MAGDIFF_ACC_2_MSB */
#define    INTF_STATS_MAGDIFF_ACC_2_MSB_INTF_STATS_MAGDIFF_ACC_2_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_2_MSB_INTF_STATS_MAGDIFF_ACC_2_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_3_LSB_INTF_STATS_MAGDIFF_ACC_3_LSB in Register INTF_STATS_MAGDIFF_ACC_3_LSB */
#define    INTF_STATS_MAGDIFF_ACC_3_LSB_INTF_STATS_MAGDIFF_ACC_3_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_3_LSB_INTF_STATS_MAGDIFF_ACC_3_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_3_MSB_INTF_STATS_MAGDIFF_ACC_3_MSB in Register INTF_STATS_MAGDIFF_ACC_3_MSB */
#define    INTF_STATS_MAGDIFF_ACC_3_MSB_INTF_STATS_MAGDIFF_ACC_3_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_3_MSB_INTF_STATS_MAGDIFF_ACC_3_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_4_LSB_INTF_STATS_MAGDIFF_ACC_4_LSB in Register INTF_STATS_MAGDIFF_ACC_4_LSB */
#define    INTF_STATS_MAGDIFF_ACC_4_LSB_INTF_STATS_MAGDIFF_ACC_4_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_4_LSB_INTF_STATS_MAGDIFF_ACC_4_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_4_MSB_INTF_STATS_MAGDIFF_ACC_4_MSB in Register INTF_STATS_MAGDIFF_ACC_4_MSB */
#define    INTF_STATS_MAGDIFF_ACC_4_MSB_INTF_STATS_MAGDIFF_ACC_4_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_4_MSB_INTF_STATS_MAGDIFF_ACC_4_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_5_LSB_INTF_STATS_MAGDIFF_ACC_5_LSB in Register INTF_STATS_MAGDIFF_ACC_5_LSB */
#define    INTF_STATS_MAGDIFF_ACC_5_LSB_INTF_STATS_MAGDIFF_ACC_5_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_5_LSB_INTF_STATS_MAGDIFF_ACC_5_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_5_MSB_INTF_STATS_MAGDIFF_ACC_5_MSB in Register INTF_STATS_MAGDIFF_ACC_5_MSB */
#define    INTF_STATS_MAGDIFF_ACC_5_MSB_INTF_STATS_MAGDIFF_ACC_5_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_5_MSB_INTF_STATS_MAGDIFF_ACC_5_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_6_LSB_INTF_STATS_MAGDIFF_ACC_6_LSB in Register INTF_STATS_MAGDIFF_ACC_6_LSB */
#define    INTF_STATS_MAGDIFF_ACC_6_LSB_INTF_STATS_MAGDIFF_ACC_6_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_6_LSB_INTF_STATS_MAGDIFF_ACC_6_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_6_MSB_INTF_STATS_MAGDIFF_ACC_6_MSB in Register INTF_STATS_MAGDIFF_ACC_6_MSB */
#define    INTF_STATS_MAGDIFF_ACC_6_MSB_INTF_STATS_MAGDIFF_ACC_6_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_6_MSB_INTF_STATS_MAGDIFF_ACC_6_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_7_LSB_INTF_STATS_MAGDIFF_ACC_7_LSB in Register INTF_STATS_MAGDIFF_ACC_7_LSB */
#define    INTF_STATS_MAGDIFF_ACC_7_LSB_INTF_STATS_MAGDIFF_ACC_7_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_7_LSB_INTF_STATS_MAGDIFF_ACC_7_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_7_MSB_INTF_STATS_MAGDIFF_ACC_7_MSB in Register INTF_STATS_MAGDIFF_ACC_7_MSB */
#define    INTF_STATS_MAGDIFF_ACC_7_MSB_INTF_STATS_MAGDIFF_ACC_7_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_7_MSB_INTF_STATS_MAGDIFF_ACC_7_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_8_LSB_INTF_STATS_MAGDIFF_ACC_8_LSB in Register INTF_STATS_MAGDIFF_ACC_8_LSB */
#define    INTF_STATS_MAGDIFF_ACC_8_LSB_INTF_STATS_MAGDIFF_ACC_8_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_8_LSB_INTF_STATS_MAGDIFF_ACC_8_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_8_MSB_INTF_STATS_MAGDIFF_ACC_8_MSB in Register INTF_STATS_MAGDIFF_ACC_8_MSB */
#define    INTF_STATS_MAGDIFF_ACC_8_MSB_INTF_STATS_MAGDIFF_ACC_8_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_8_MSB_INTF_STATS_MAGDIFF_ACC_8_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_9_LSB_INTF_STATS_MAGDIFF_ACC_9_LSB in Register INTF_STATS_MAGDIFF_ACC_9_LSB */
#define    INTF_STATS_MAGDIFF_ACC_9_LSB_INTF_STATS_MAGDIFF_ACC_9_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_9_LSB_INTF_STATS_MAGDIFF_ACC_9_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_9_MSB_INTF_STATS_MAGDIFF_ACC_9_MSB in Register INTF_STATS_MAGDIFF_ACC_9_MSB */
#define    INTF_STATS_MAGDIFF_ACC_9_MSB_INTF_STATS_MAGDIFF_ACC_9_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_9_MSB_INTF_STATS_MAGDIFF_ACC_9_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_10_LSB_INTF_STATS_MAGDIFF_ACC_10_LSB in Register INTF_STATS_MAGDIFF_ACC_10_LSB */
#define    INTF_STATS_MAGDIFF_ACC_10_LSB_INTF_STATS_MAGDIFF_ACC_10_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_10_LSB_INTF_STATS_MAGDIFF_ACC_10_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_10_MSB_INTF_STATS_MAGDIFF_ACC_10_MSB in Register INTF_STATS_MAGDIFF_ACC_10_MSB */
#define    INTF_STATS_MAGDIFF_ACC_10_MSB_INTF_STATS_MAGDIFF_ACC_10_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_10_MSB_INTF_STATS_MAGDIFF_ACC_10_MSB_END      (3U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_11_LSB_INTF_STATS_MAGDIFF_ACC_11_LSB in Register INTF_STATS_MAGDIFF_ACC_11_LSB */
#define    INTF_STATS_MAGDIFF_ACC_11_LSB_INTF_STATS_MAGDIFF_ACC_11_LSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_11_LSB_INTF_STATS_MAGDIFF_ACC_11_LSB_END      (31U)

/* Definition for field INTF_STATS_MAGDIFF_ACC_11_MSB_INTF_STATS_MAGDIFF_ACC_11_MSB in Register INTF_STATS_MAGDIFF_ACC_11_MSB */
#define    INTF_STATS_MAGDIFF_ACC_11_MSB_INTF_STATS_MAGDIFF_ACC_11_MSB_START      (0U)
#define    INTF_STATS_MAGDIFF_ACC_11_MSB_INTF_STATS_MAGDIFF_ACC_11_MSB_END      (3U)

/* Definition for field INTF_LOC_THRESH_MAG0_SW_INTF_LOC_THRESH_MAG0_SW in Register INTF_LOC_THRESH_MAG0_SW */
#define    INTF_LOC_THRESH_MAG0_SW_INTF_LOC_THRESH_MAG0_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG0_SW_INTF_LOC_THRESH_MAG0_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG1_SW_INTF_LOC_THRESH_MAG1_SW in Register INTF_LOC_THRESH_MAG1_SW */
#define    INTF_LOC_THRESH_MAG1_SW_INTF_LOC_THRESH_MAG1_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG1_SW_INTF_LOC_THRESH_MAG1_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG2_SW_INTF_LOC_THRESH_MAG2_SW in Register INTF_LOC_THRESH_MAG2_SW */
#define    INTF_LOC_THRESH_MAG2_SW_INTF_LOC_THRESH_MAG2_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG2_SW_INTF_LOC_THRESH_MAG2_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG3_SW_INTF_LOC_THRESH_MAG3_SW in Register INTF_LOC_THRESH_MAG3_SW */
#define    INTF_LOC_THRESH_MAG3_SW_INTF_LOC_THRESH_MAG3_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG3_SW_INTF_LOC_THRESH_MAG3_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG4_SW_INTF_LOC_THRESH_MAG4_SW in Register INTF_LOC_THRESH_MAG4_SW */
#define    INTF_LOC_THRESH_MAG4_SW_INTF_LOC_THRESH_MAG4_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG4_SW_INTF_LOC_THRESH_MAG4_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG5_SW_INTF_LOC_THRESH_MAG5_SW in Register INTF_LOC_THRESH_MAG5_SW */
#define    INTF_LOC_THRESH_MAG5_SW_INTF_LOC_THRESH_MAG5_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG5_SW_INTF_LOC_THRESH_MAG5_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG6_SW_INTF_LOC_THRESH_MAG6_SW in Register INTF_LOC_THRESH_MAG6_SW */
#define    INTF_LOC_THRESH_MAG6_SW_INTF_LOC_THRESH_MAG6_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG6_SW_INTF_LOC_THRESH_MAG6_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG7_SW_INTF_LOC_THRESH_MAG7_SW in Register INTF_LOC_THRESH_MAG7_SW */
#define    INTF_LOC_THRESH_MAG7_SW_INTF_LOC_THRESH_MAG7_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG7_SW_INTF_LOC_THRESH_MAG7_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG8_SW_INTF_LOC_THRESH_MAG8_SW in Register INTF_LOC_THRESH_MAG8_SW */
#define    INTF_LOC_THRESH_MAG8_SW_INTF_LOC_THRESH_MAG8_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG8_SW_INTF_LOC_THRESH_MAG8_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG9_SW_INTF_LOC_THRESH_MAG9_SW in Register INTF_LOC_THRESH_MAG9_SW */
#define    INTF_LOC_THRESH_MAG9_SW_INTF_LOC_THRESH_MAG9_SW_START             (0U)
#define    INTF_LOC_THRESH_MAG9_SW_INTF_LOC_THRESH_MAG9_SW_END               (23U)

/* Definition for field INTF_LOC_THRESH_MAG10_SW_INTF_LOC_THRESH_MAG10_SW in Register INTF_LOC_THRESH_MAG10_SW */
#define    INTF_LOC_THRESH_MAG10_SW_INTF_LOC_THRESH_MAG10_SW_START           (0U)
#define    INTF_LOC_THRESH_MAG10_SW_INTF_LOC_THRESH_MAG10_SW_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAG11_SW_INTF_LOC_THRESH_MAG11_SW in Register INTF_LOC_THRESH_MAG11_SW */
#define    INTF_LOC_THRESH_MAG11_SW_INTF_LOC_THRESH_MAG11_SW_START           (0U)
#define    INTF_LOC_THRESH_MAG11_SW_INTF_LOC_THRESH_MAG11_SW_END             (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF0_SW_INTF_LOC_THRESH_MAGDIFF0_SW in Register INTF_LOC_THRESH_MAGDIFF0_SW */
#define    INTF_LOC_THRESH_MAGDIFF0_SW_INTF_LOC_THRESH_MAGDIFF0_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF0_SW_INTF_LOC_THRESH_MAGDIFF0_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF1_SW_INTF_LOC_THRESH_MAGDIFF1_SW in Register INTF_LOC_THRESH_MAGDIFF1_SW */
#define    INTF_LOC_THRESH_MAGDIFF1_SW_INTF_LOC_THRESH_MAGDIFF1_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF1_SW_INTF_LOC_THRESH_MAGDIFF1_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF2_SW_INTF_LOC_THRESH_MAGDIFF2_SW in Register INTF_LOC_THRESH_MAGDIFF2_SW */
#define    INTF_LOC_THRESH_MAGDIFF2_SW_INTF_LOC_THRESH_MAGDIFF2_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF2_SW_INTF_LOC_THRESH_MAGDIFF2_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF3_SW_INTF_LOC_THRESH_MAGDIFF3_SW in Register INTF_LOC_THRESH_MAGDIFF3_SW */
#define    INTF_LOC_THRESH_MAGDIFF3_SW_INTF_LOC_THRESH_MAGDIFF3_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF3_SW_INTF_LOC_THRESH_MAGDIFF3_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF4_SW_INTF_LOC_THRESH_MAGDIFF4_SW in Register INTF_LOC_THRESH_MAGDIFF4_SW */
#define    INTF_LOC_THRESH_MAGDIFF4_SW_INTF_LOC_THRESH_MAGDIFF4_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF4_SW_INTF_LOC_THRESH_MAGDIFF4_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF5_SW_INTF_LOC_THRESH_MAGDIFF5_SW in Register INTF_LOC_THRESH_MAGDIFF5_SW */
#define    INTF_LOC_THRESH_MAGDIFF5_SW_INTF_LOC_THRESH_MAGDIFF5_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF5_SW_INTF_LOC_THRESH_MAGDIFF5_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF6_SW_INTF_LOC_THRESH_MAGDIFF6_SW in Register INTF_LOC_THRESH_MAGDIFF6_SW */
#define    INTF_LOC_THRESH_MAGDIFF6_SW_INTF_LOC_THRESH_MAGDIFF6_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF6_SW_INTF_LOC_THRESH_MAGDIFF6_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF7_SW_INTF_LOC_THRESH_MAGDIFF7_SW in Register INTF_LOC_THRESH_MAGDIFF7_SW */
#define    INTF_LOC_THRESH_MAGDIFF7_SW_INTF_LOC_THRESH_MAGDIFF7_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF7_SW_INTF_LOC_THRESH_MAGDIFF7_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF8_SW_INTF_LOC_THRESH_MAGDIFF8_SW in Register INTF_LOC_THRESH_MAGDIFF8_SW */
#define    INTF_LOC_THRESH_MAGDIFF8_SW_INTF_LOC_THRESH_MAGDIFF8_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF8_SW_INTF_LOC_THRESH_MAGDIFF8_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF9_SW_INTF_LOC_THRESH_MAGDIFF9_SW in Register INTF_LOC_THRESH_MAGDIFF9_SW */
#define    INTF_LOC_THRESH_MAGDIFF9_SW_INTF_LOC_THRESH_MAGDIFF9_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF9_SW_INTF_LOC_THRESH_MAGDIFF9_SW_END       (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF10_SW_INTF_LOC_THRESH_MAGDIFF10_SW in Register INTF_LOC_THRESH_MAGDIFF10_SW */
#define    INTF_LOC_THRESH_MAGDIFF10_SW_INTF_LOC_THRESH_MAGDIFF10_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF10_SW_INTF_LOC_THRESH_MAGDIFF10_SW_END      (23U)

/* Definition for field INTF_LOC_THRESH_MAGDIFF11_SW_INTF_LOC_THRESH_MAGDIFF11_SW in Register INTF_LOC_THRESH_MAGDIFF11_SW */
#define    INTF_LOC_THRESH_MAGDIFF11_SW_INTF_LOC_THRESH_MAGDIFF11_SW_START      (0U)
#define    INTF_LOC_THRESH_MAGDIFF11_SW_INTF_LOC_THRESH_MAGDIFF11_SW_END      (23U)

/* Definition for field INTF_STATS_ACC_CLIP_STATUS_INTF_STATS_MAGDIFF_ACCUMULATOR_CLIP_STATUS in Register INTF_STATS_ACC_CLIP_STATUS */
#define    INTF_STATS_ACC_CLIP_STATUS_INTF_STATS_MAGDIFF_ACCUMULATOR_CLIP_STATUS_START      (16U)
#define    INTF_STATS_ACC_CLIP_STATUS_INTF_STATS_MAGDIFF_ACCUMULATOR_CLIP_STATUS_END      (27U)

/* Definition for field INTF_STATS_ACC_CLIP_STATUS_INTF_STATS_MAG_ACCUMULATOR_CLIP_STATUS in Register INTF_STATS_ACC_CLIP_STATUS */
#define    INTF_STATS_ACC_CLIP_STATUS_INTF_STATS_MAG_ACCUMULATOR_CLIP_STATUS_START      (0U)
#define    INTF_STATS_ACC_CLIP_STATUS_INTF_STATS_MAG_ACCUMULATOR_CLIP_STATUS_END      (11U)

/* Definition for field INTF_STATS_THRESH_CLIP_STATUS_INTF_STATS_THRESH_MAGDIFF_CLIP_STATUS in Register INTF_STATS_THRESH_CLIP_STATUS */
#define    INTF_STATS_THRESH_CLIP_STATUS_INTF_STATS_THRESH_MAGDIFF_CLIP_STATUS_START      (16U)
#define    INTF_STATS_THRESH_CLIP_STATUS_INTF_STATS_THRESH_MAGDIFF_CLIP_STATUS_END      (27U)

/* Definition for field INTF_STATS_THRESH_CLIP_STATUS_INTF_STATS_THRESH_MAG_CLIP_STATUS in Register INTF_STATS_THRESH_CLIP_STATUS */
#define    INTF_STATS_THRESH_CLIP_STATUS_INTF_STATS_THRESH_MAG_CLIP_STATUS_START      (0U)
#define    INTF_STATS_THRESH_CLIP_STATUS_INTF_STATS_THRESH_MAG_CLIP_STATUS_END      (11U)

/* Definition for field INTF_MITG_WINDOW_PARAM_0_INTF_MITG_WINDOW_PARAM_0 in Register INTF_MITG_WINDOW_PARAM_0 */
#define    INTF_MITG_WINDOW_PARAM_0_INTF_MITG_WINDOW_PARAM_0_START           (0U)
#define    INTF_MITG_WINDOW_PARAM_0_INTF_MITG_WINDOW_PARAM_0_END             (4U)

/* Definition for field INTF_MITG_WINDOW_PARAM_1_INTF_MITG_WINDOW_PARAM_1 in Register INTF_MITG_WINDOW_PARAM_1 */
#define    INTF_MITG_WINDOW_PARAM_1_INTF_MITG_WINDOW_PARAM_1_START           (0U)
#define    INTF_MITG_WINDOW_PARAM_1_INTF_MITG_WINDOW_PARAM_1_END             (4U)

/* Definition for field INTF_MITG_WINDOW_PARAM_2_INTF_MITG_WINDOW_PARAM_2 in Register INTF_MITG_WINDOW_PARAM_2 */
#define    INTF_MITG_WINDOW_PARAM_2_INTF_MITG_WINDOW_PARAM_2_START           (0U)
#define    INTF_MITG_WINDOW_PARAM_2_INTF_MITG_WINDOW_PARAM_2_END             (4U)

/* Definition for field INTF_MITG_WINDOW_PARAM_3_INTF_MITG_WINDOW_PARAM_3 in Register INTF_MITG_WINDOW_PARAM_3 */
#define    INTF_MITG_WINDOW_PARAM_3_INTF_MITG_WINDOW_PARAM_3_START           (0U)
#define    INTF_MITG_WINDOW_PARAM_3_INTF_MITG_WINDOW_PARAM_3_END             (4U)

/* Definition for field INTF_MITG_WINDOW_PARAM_4_INTF_MITG_WINDOW_PARAM_4 in Register INTF_MITG_WINDOW_PARAM_4 */
#define    INTF_MITG_WINDOW_PARAM_4_INTF_MITG_WINDOW_PARAM_4_START           (0U)
#define    INTF_MITG_WINDOW_PARAM_4_INTF_MITG_WINDOW_PARAM_4_END             (4U)

/* Definition for field INTF_STATS_SUM_MAG_VAL_INTF_STATS_SUM_MAG_VAL in Register INTF_STATS_SUM_MAG_VAL */
#define    INTF_STATS_SUM_MAG_VAL_INTF_STATS_SUM_MAG_VAL_START               (0U)
#define    INTF_STATS_SUM_MAG_VAL_INTF_STATS_SUM_MAG_VAL_END                 (23U)

/* Definition for field INTF_STATS_SUM_MAG_VAL_CLIP_STATUS_INTF_STATS_SUM_MAG_VAL_CLIP_STATUS in Register INTF_STATS_SUM_MAG_VAL_CLIP_STATUS */
#define    INTF_STATS_SUM_MAG_VAL_CLIP_STATUS_INTF_STATS_SUM_MAG_VAL_CLIP_STATUS_START      (0U)
#define    INTF_STATS_SUM_MAG_VAL_CLIP_STATUS_INTF_STATS_SUM_MAG_VAL_CLIP_STATUS_END      (0U)

/* Definition for field INTF_STATS_SUM_MAGDIFF_VAL_INTF_STATS_SUM_MAGDIFF_VAL in Register INTF_STATS_SUM_MAGDIFF_VAL */
#define    INTF_STATS_SUM_MAGDIFF_VAL_INTF_STATS_SUM_MAGDIFF_VAL_START       (0U)
#define    INTF_STATS_SUM_MAGDIFF_VAL_INTF_STATS_SUM_MAGDIFF_VAL_END         (23U)

/* Definition for field INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS_INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS in Register INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS */
#define    INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS_INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS_START      (0U)
#define    INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS_INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS_END      (0U)

/* Definition for field INTERF_RESERVED_5_INTERF_RESERVED_5 in Register INTERF_RESERVED_5 */
#define    INTERF_RESERVED_5_INTERF_RESERVED_5_START                         (0U)
#define    INTERF_RESERVED_5_INTERF_RESERVED_5_END                           (31U)

/* Definition for field ICMULT_SCALE0_ICMULT_SCALE0 in Register ICMULT_SCALE0 */
#define    ICMULT_SCALE0_ICMULT_SCALE0_START                                 (0U)
#define    ICMULT_SCALE0_ICMULT_SCALE0_END                                   (20U)

/* Definition for field ICMULT_SCALE1_ICMULT_SCALE1 in Register ICMULT_SCALE1 */
#define    ICMULT_SCALE1_ICMULT_SCALE1_START                                 (0U)
#define    ICMULT_SCALE1_ICMULT_SCALE1_END                                   (20U)

/* Definition for field ICMULT_SCALE2_ICMULT_SCALE2 in Register ICMULT_SCALE2 */
#define    ICMULT_SCALE2_ICMULT_SCALE2_START                                 (0U)
#define    ICMULT_SCALE2_ICMULT_SCALE2_END                                   (20U)

/* Definition for field ICMULT_SCALE3_ICMULT_SCALE3 in Register ICMULT_SCALE3 */
#define    ICMULT_SCALE3_ICMULT_SCALE3_START                                 (0U)
#define    ICMULT_SCALE3_ICMULT_SCALE3_END                                   (20U)

/* Definition for field ICMULT_SCALE4_ICMULT_SCALE4 in Register ICMULT_SCALE4 */
#define    ICMULT_SCALE4_ICMULT_SCALE4_START                                 (0U)
#define    ICMULT_SCALE4_ICMULT_SCALE4_END                                   (20U)

/* Definition for field ICMULT_SCALE5_ICMULT_SCALE5 in Register ICMULT_SCALE5 */
#define    ICMULT_SCALE5_ICMULT_SCALE5_START                                 (0U)
#define    ICMULT_SCALE5_ICMULT_SCALE5_END                                   (20U)

/* Definition for field ICMULT_SCALE6_ICMULT_SCALE6 in Register ICMULT_SCALE6 */
#define    ICMULT_SCALE6_ICMULT_SCALE6_START                                 (0U)
#define    ICMULT_SCALE6_ICMULT_SCALE6_END                                   (20U)

/* Definition for field ICMULT_SCALE7_ICMULT_SCALE7 in Register ICMULT_SCALE7 */
#define    ICMULT_SCALE7_ICMULT_SCALE7_START                                 (0U)
#define    ICMULT_SCALE7_ICMULT_SCALE7_END                                   (20U)

/* Definition for field ICMULT_SCALE8_ICMULT_SCALE8 in Register ICMULT_SCALE8 */
#define    ICMULT_SCALE8_ICMULT_SCALE8_START                                 (0U)
#define    ICMULT_SCALE8_ICMULT_SCALE8_END                                   (20U)

/* Definition for field ICMULT_SCALE9_ICMULT_SCALE9 in Register ICMULT_SCALE9 */
#define    ICMULT_SCALE9_ICMULT_SCALE9_START                                 (0U)
#define    ICMULT_SCALE9_ICMULT_SCALE9_END                                   (20U)

/* Definition for field ICMULT_SCALE10_ICMULT_SCALE10 in Register ICMULT_SCALE10 */
#define    ICMULT_SCALE10_ICMULT_SCALE10_START                               (0U)
#define    ICMULT_SCALE10_ICMULT_SCALE10_END                                 (20U)

/* Definition for field ICMULT_SCALE11_ICMULT_SCALE11 in Register ICMULT_SCALE11 */
#define    ICMULT_SCALE11_ICMULT_SCALE11_START                               (0U)
#define    ICMULT_SCALE11_ICMULT_SCALE11_END                                 (20U)

/* Definition for field QCMULT_SCALE0_QCMULT_SCALE0 in Register QCMULT_SCALE0 */
#define    QCMULT_SCALE0_QCMULT_SCALE0_START                                 (0U)
#define    QCMULT_SCALE0_QCMULT_SCALE0_END                                   (20U)

/* Definition for field QCMULT_SCALE1_QCMULT_SCALE1 in Register QCMULT_SCALE1 */
#define    QCMULT_SCALE1_QCMULT_SCALE1_START                                 (0U)
#define    QCMULT_SCALE1_QCMULT_SCALE1_END                                   (20U)

/* Definition for field QCMULT_SCALE2_QCMULT_SCALE2 in Register QCMULT_SCALE2 */
#define    QCMULT_SCALE2_QCMULT_SCALE2_START                                 (0U)
#define    QCMULT_SCALE2_QCMULT_SCALE2_END                                   (20U)

/* Definition for field QCMULT_SCALE3_QCMULT_SCALE3 in Register QCMULT_SCALE3 */
#define    QCMULT_SCALE3_QCMULT_SCALE3_START                                 (0U)
#define    QCMULT_SCALE3_QCMULT_SCALE3_END                                   (20U)

/* Definition for field QCMULT_SCALE4_QCMULT_SCALE4 in Register QCMULT_SCALE4 */
#define    QCMULT_SCALE4_QCMULT_SCALE4_START                                 (0U)
#define    QCMULT_SCALE4_QCMULT_SCALE4_END                                   (20U)

/* Definition for field QCMULT_SCALE5_QCMULT_SCALE5 in Register QCMULT_SCALE5 */
#define    QCMULT_SCALE5_QCMULT_SCALE5_START                                 (0U)
#define    QCMULT_SCALE5_QCMULT_SCALE5_END                                   (20U)

/* Definition for field QCMULT_SCALE6_QCMULT_SCALE6 in Register QCMULT_SCALE6 */
#define    QCMULT_SCALE6_QCMULT_SCALE6_START                                 (0U)
#define    QCMULT_SCALE6_QCMULT_SCALE6_END                                   (20U)

/* Definition for field QCMULT_SCALE7_QCMULT_SCALE7 in Register QCMULT_SCALE7 */
#define    QCMULT_SCALE7_QCMULT_SCALE7_START                                 (0U)
#define    QCMULT_SCALE7_QCMULT_SCALE7_END                                   (20U)

/* Definition for field QCMULT_SCALE8_QCMULT_SCALE8 in Register QCMULT_SCALE8 */
#define    QCMULT_SCALE8_QCMULT_SCALE8_START                                 (0U)
#define    QCMULT_SCALE8_QCMULT_SCALE8_END                                   (20U)

/* Definition for field QCMULT_SCALE9_QCMULT_SCALE9 in Register QCMULT_SCALE9 */
#define    QCMULT_SCALE9_QCMULT_SCALE9_START                                 (0U)
#define    QCMULT_SCALE9_QCMULT_SCALE9_END                                   (20U)

/* Definition for field QCMULT_SCALE10_QCMULT_SCALE10 in Register QCMULT_SCALE10 */
#define    QCMULT_SCALE10_QCMULT_SCALE10_START                               (0U)
#define    QCMULT_SCALE10_QCMULT_SCALE10_END                                 (20U)

/* Definition for field QCMULT_SCALE11_QCMULT_SCALE11 in Register QCMULT_SCALE11 */
#define    QCMULT_SCALE11_QCMULT_SCALE11_START                               (0U)
#define    QCMULT_SCALE11_QCMULT_SCALE11_END                                 (20U)

/* Definition for field TWID_INCR_DELTA_FRAC_TWID_INCR_DELTA_FRAC in Register TWID_INCR_DELTA_FRAC */
#define    TWID_INCR_DELTA_FRAC_TWID_INCR_DELTA_FRAC_START                   (0U)
#define    TWID_INCR_DELTA_FRAC_TWID_INCR_DELTA_FRAC_END                     (9U)

/* Definition for field RECWIN_RESET_SW_RECWIN_RESET_SW in Register RECWIN_RESET_SW */
#define    RECWIN_RESET_SW_RECWIN_RESET_SW_START                             (0U)
#define    RECWIN_RESET_SW_RECWIN_RESET_SW_END                               (0U)

/* Definition for field TWID_INCR_DELTA_FRAC_RESET_SW_TWID_INCR_DELTA_FRAC_RESET_SW in Register TWID_INCR_DELTA_FRAC_RESET_SW */
#define    TWID_INCR_DELTA_FRAC_RESET_SW_TWID_INCR_DELTA_FRAC_RESET_SW_START      (0U)
#define    TWID_INCR_DELTA_FRAC_RESET_SW_TWID_INCR_DELTA_FRAC_RESET_SW_END      (0U)

/* Definition for field TWID_INCR_DELTA_FRAC_CLIP_STATUS_TWID_INCR_DELTA_FRAC_CLIP_STATUS in Register TWID_INCR_DELTA_FRAC_CLIP_STATUS */
#define    TWID_INCR_DELTA_FRAC_CLIP_STATUS_TWID_INCR_DELTA_FRAC_CLIP_STATUS_START      (0U)
#define    TWID_INCR_DELTA_FRAC_CLIP_STATUS_TWID_INCR_DELTA_FRAC_CLIP_STATUS_END      (0U)

/* Definition for field RECWIN_INIT_KVAL_RECWIN_INIT_KVAL in Register RECWIN_INIT_KVAL */
#define    RECWIN_INIT_KVAL_RECWIN_INIT_KVAL_START                           (0U)
#define    RECWIN_INIT_KVAL_RECWIN_INIT_KVAL_END                             (11U)

/* Definition for field CMULT_RESERVED_2_CMULT_RESERVED_2 in Register CMULT_RESERVED_2 */
#define    CMULT_RESERVED_2_CMULT_RESERVED_2_START                           (0U)
#define    CMULT_RESERVED_2_CMULT_RESERVED_2_END                             (31U)

/* Definition for field CHAN_COMB_SIZE_CHAN_COMB_SIZE in Register CHAN_COMB_SIZE */
#define    CHAN_COMB_SIZE_CHAN_COMB_SIZE_START                               (0U)
#define    CHAN_COMB_SIZE_CHAN_COMB_SIZE_END                                 (7U)

/* Definition for field CHAN_COMB_VEC_0_CHAN_COMB_VEC_0 in Register CHAN_COMB_VEC_0 */
#define    CHAN_COMB_VEC_0_CHAN_COMB_VEC_0_START                             (0U)
#define    CHAN_COMB_VEC_0_CHAN_COMB_VEC_0_END                               (31U)

/* Definition for field CHAN_COMB_VEC_1_CHAN_COMB_VEC_1 in Register CHAN_COMB_VEC_1 */
#define    CHAN_COMB_VEC_1_CHAN_COMB_VEC_1_START                             (0U)
#define    CHAN_COMB_VEC_1_CHAN_COMB_VEC_1_END                               (31U)

/* Definition for field CHAN_COMB_VEC_2_CHAN_COMB_VEC_2 in Register CHAN_COMB_VEC_2 */
#define    CHAN_COMB_VEC_2_CHAN_COMB_VEC_2_START                             (0U)
#define    CHAN_COMB_VEC_2_CHAN_COMB_VEC_2_END                               (31U)

/* Definition for field CHAN_COMB_VEC_3_CHAN_COMB_VEC_3 in Register CHAN_COMB_VEC_3 */
#define    CHAN_COMB_VEC_3_CHAN_COMB_VEC_3_START                             (0U)
#define    CHAN_COMB_VEC_3_CHAN_COMB_VEC_3_END                               (31U)

/* Definition for field CHAN_COMB_VEC_4_CHAN_COMB_VEC_4 in Register CHAN_COMB_VEC_4 */
#define    CHAN_COMB_VEC_4_CHAN_COMB_VEC_4_START                             (0U)
#define    CHAN_COMB_VEC_4_CHAN_COMB_VEC_4_END                               (31U)

/* Definition for field CHAN_COMB_VEC_5_CHAN_COMB_VEC_5 in Register CHAN_COMB_VEC_5 */
#define    CHAN_COMB_VEC_5_CHAN_COMB_VEC_5_START                             (0U)
#define    CHAN_COMB_VEC_5_CHAN_COMB_VEC_5_END                               (31U)

/* Definition for field CHAN_COMB_VEC_6_CHAN_COMB_VEC_6 in Register CHAN_COMB_VEC_6 */
#define    CHAN_COMB_VEC_6_CHAN_COMB_VEC_6_START                             (0U)
#define    CHAN_COMB_VEC_6_CHAN_COMB_VEC_6_END                               (31U)

/* Definition for field CHAN_COMB_VEC_7_CHAN_COMB_VEC_7 in Register CHAN_COMB_VEC_7 */
#define    CHAN_COMB_VEC_7_CHAN_COMB_VEC_7_START                             (0U)
#define    CHAN_COMB_VEC_7_CHAN_COMB_VEC_7_END                               (31U)

/* Definition for field CHANNEL_COMB_CLIP_STATUS_CHANNEL_COMB_CLIP_STATUS in Register CHANNEL_COMB_CLIP_STATUS */
#define    CHANNEL_COMB_CLIP_STATUS_CHANNEL_COMB_CLIP_STATUS_START           (0U)
#define    CHANNEL_COMB_CLIP_STATUS_CHANNEL_COMB_CLIP_STATUS_END             (0U)

/* Definition for field ZERO_INSERT_NUM_ZERO_INSERT_NUM in Register ZERO_INSERT_NUM */
#define    ZERO_INSERT_NUM_ZERO_INSERT_NUM_START                             (0U)
#define    ZERO_INSERT_NUM_ZERO_INSERT_NUM_END                               (7U)

/* Definition for field ZERO_INSERT_MASK_0_ZERO_INSERT_MASK_0 in Register ZERO_INSERT_MASK_0 */
#define    ZERO_INSERT_MASK_0_ZERO_INSERT_MASK_0_START                       (0U)
#define    ZERO_INSERT_MASK_0_ZERO_INSERT_MASK_0_END                         (31U)

/* Definition for field ZERO_INSERT_MASK_1_ZERO_INSERT_MASK_1 in Register ZERO_INSERT_MASK_1 */
#define    ZERO_INSERT_MASK_1_ZERO_INSERT_MASK_1_START                       (0U)
#define    ZERO_INSERT_MASK_1_ZERO_INSERT_MASK_1_END                         (31U)

/* Definition for field ZERO_INSERT_MASK_2_ZERO_INSERT_MASK_2 in Register ZERO_INSERT_MASK_2 */
#define    ZERO_INSERT_MASK_2_ZERO_INSERT_MASK_2_START                       (0U)
#define    ZERO_INSERT_MASK_2_ZERO_INSERT_MASK_2_END                         (31U)

/* Definition for field ZERO_INSERT_MASK_3_ZERO_INSERT_MASK_3 in Register ZERO_INSERT_MASK_3 */
#define    ZERO_INSERT_MASK_3_ZERO_INSERT_MASK_3_START                       (0U)
#define    ZERO_INSERT_MASK_3_ZERO_INSERT_MASK_3_END                         (31U)

/* Definition for field ZERO_INSERT_MASK_4_ZERO_INSERT_MASK_4 in Register ZERO_INSERT_MASK_4 */
#define    ZERO_INSERT_MASK_4_ZERO_INSERT_MASK_4_START                       (0U)
#define    ZERO_INSERT_MASK_4_ZERO_INSERT_MASK_4_END                         (31U)

/* Definition for field ZERO_INSERT_MASK_5_ZERO_INSERT_MASK_5 in Register ZERO_INSERT_MASK_5 */
#define    ZERO_INSERT_MASK_5_ZERO_INSERT_MASK_5_START                       (0U)
#define    ZERO_INSERT_MASK_5_ZERO_INSERT_MASK_5_END                         (31U)

/* Definition for field ZERO_INSERT_MASK_6_ZERO_INSERT_MASK_6 in Register ZERO_INSERT_MASK_6 */
#define    ZERO_INSERT_MASK_6_ZERO_INSERT_MASK_6_START                       (0U)
#define    ZERO_INSERT_MASK_6_ZERO_INSERT_MASK_6_END                         (31U)

/* Definition for field ZERO_INSERT_MASK_7_ZERO_INSERT_MASK_7 in Register ZERO_INSERT_MASK_7 */
#define    ZERO_INSERT_MASK_7_ZERO_INSERT_MASK_7_START                       (0U)
#define    ZERO_INSERT_MASK_7_ZERO_INSERT_MASK_7_END                         (31U)

/* Definition for field ZERO_INSERT_RESERVED_1_ZERO_INSERT_RESERVED_1 in Register ZERO_INSERT_RESERVED_1 */
#define    ZERO_INSERT_RESERVED_1_ZERO_INSERT_RESERVED_1_START               (0U)
#define    ZERO_INSERT_RESERVED_1_ZERO_INSERT_RESERVED_1_END                 (31U)

/* Definition for field ZERO_INSERT_RESERVED_2_ZERO_INSERT_RESERVED_2 in Register ZERO_INSERT_RESERVED_2 */
#define    ZERO_INSERT_RESERVED_2_ZERO_INSERT_RESERVED_2_START               (0U)
#define    ZERO_INSERT_RESERVED_2_ZERO_INSERT_RESERVED_2_END                 (31U)

/* Definition for field ZERO_INSERT_RESERVED_3_ZERO_INSERT_RESERVED_3 in Register ZERO_INSERT_RESERVED_3 */
#define    ZERO_INSERT_RESERVED_3_ZERO_INSERT_RESERVED_3_START               (0U)
#define    ZERO_INSERT_RESERVED_3_ZERO_INSERT_RESERVED_3_END                 (31U)

/* Definition for field ZERO_INSERT_RESERVED_4_ZERO_INSERT_RESERVED_4 in Register ZERO_INSERT_RESERVED_4 */
#define    ZERO_INSERT_RESERVED_4_ZERO_INSERT_RESERVED_4_START               (0U)
#define    ZERO_INSERT_RESERVED_4_ZERO_INSERT_RESERVED_4_END                 (31U)

/* Definition for field LFSR_SEED_LFSR_SEED in Register LFSR_SEED */
#define    LFSR_SEED_LFSR_SEED_START                                         (0U)
#define    LFSR_SEED_LFSR_SEED_END                                           (28U)

/* Definition for field LFSR_LOAD_LFSR_LOAD in Register LFSR_LOAD */
#define    LFSR_LOAD_LFSR_LOAD_START                                         (0U)
#define    LFSR_LOAD_LFSR_LOAD_END                                           (0U)

/* Definition for field DITHER_TWID_EN_DITHER_TWID_EN in Register DITHER_TWID_EN */
#define    DITHER_TWID_EN_DITHER_TWID_EN_START                               (0U)
#define    DITHER_TWID_EN_DITHER_TWID_EN_END                                 (0U)

/* Definition for field FFT_CLIP_FFT_CLIP in Register FFT_CLIP */
#define    FFT_CLIP_FFT_CLIP_START                                           (0U)
#define    FFT_CLIP_FFT_CLIP_END                                             (12U)

/* Definition for field CLR_FFTCLIP_CLR_FFTCLIP in Register CLR_FFTCLIP */
#define    CLR_FFTCLIP_CLR_FFTCLIP_START                                     (0U)
#define    CLR_FFTCLIP_CLR_FFTCLIP_END                                       (0U)

/* Definition for field CLR_CLIP_MISC_CLR_CLIP_STATUS in Register CLR_CLIP_MISC */
#define    CLR_CLIP_MISC_CLR_CLIP_STATUS_START                               (0U)
#define    CLR_CLIP_MISC_CLR_CLIP_STATUS_END                                 (0U)

/* Definition for field IP_OP_FORMATTER_CLIP_STATUS_OP_FORMATTER_CLIP_STATUS in Register IP_OP_FORMATTER_CLIP_STATUS */
#define    IP_OP_FORMATTER_CLIP_STATUS_OP_FORMATTER_CLIP_STATUS_START        (16U)
#define    IP_OP_FORMATTER_CLIP_STATUS_OP_FORMATTER_CLIP_STATUS_END          (16U)

/* Definition for field IP_OP_FORMATTER_CLIP_STATUS_IP_FORMATTER_CLIP_STATUS in Register IP_OP_FORMATTER_CLIP_STATUS */
#define    IP_OP_FORMATTER_CLIP_STATUS_IP_FORMATTER_CLIP_STATUS_START        (0U)
#define    IP_OP_FORMATTER_CLIP_STATUS_IP_FORMATTER_CLIP_STATUS_END          (0U)

/* Definition for field FFT_RESERVED_1_FFT_RESERVED_1 in Register FFT_RESERVED_1 */
#define    FFT_RESERVED_1_FFT_RESERVED_1_START                               (0U)
#define    FFT_RESERVED_1_FFT_RESERVED_1_END                                 (31U)

/* Definition for field FFT_RESERVED_2_FFT_RESERVED_2 in Register FFT_RESERVED_2 */
#define    FFT_RESERVED_2_FFT_RESERVED_2_START                               (0U)
#define    FFT_RESERVED_2_FFT_RESERVED_2_END                                 (31U)

/* Definition for field FFT_RESERVED_3_FFT_RESERVED_3 in Register FFT_RESERVED_3 */
#define    FFT_RESERVED_3_FFT_RESERVED_3_START                               (0U)
#define    FFT_RESERVED_3_FFT_RESERVED_3_END                                 (31U)

/* Definition for field MAX1_VALUE_MAX1_VALUE in Register MAX1_VALUE */
#define    MAX1_VALUE_MAX1_VALUE_START                                       (0U)
#define    MAX1_VALUE_MAX1_VALUE_END                                         (23U)

/* Definition for field MAX2_VALUE_MAX2_VALUE in Register MAX2_VALUE */
#define    MAX2_VALUE_MAX2_VALUE_START                                       (0U)
#define    MAX2_VALUE_MAX2_VALUE_END                                         (23U)

/* Definition for field MAX3_VALUE_MAX3_VALUE in Register MAX3_VALUE */
#define    MAX3_VALUE_MAX3_VALUE_START                                       (0U)
#define    MAX3_VALUE_MAX3_VALUE_END                                         (23U)

/* Definition for field MAX4_VALUE_MAX4_VALUE in Register MAX4_VALUE */
#define    MAX4_VALUE_MAX4_VALUE_START                                       (0U)
#define    MAX4_VALUE_MAX4_VALUE_END                                         (23U)

/* Definition for field MAX1_INDEX_MAX1_INDEX in Register MAX1_INDEX */
#define    MAX1_INDEX_MAX1_INDEX_START                                       (0U)
#define    MAX1_INDEX_MAX1_INDEX_END                                         (11U)

/* Definition for field MAX2_INDEX_MAX2_INDEX in Register MAX2_INDEX */
#define    MAX2_INDEX_MAX2_INDEX_START                                       (0U)
#define    MAX2_INDEX_MAX2_INDEX_END                                         (11U)

/* Definition for field MAX3_INDEX_MAX3_INDEX in Register MAX3_INDEX */
#define    MAX3_INDEX_MAX3_INDEX_START                                       (0U)
#define    MAX3_INDEX_MAX3_INDEX_END                                         (11U)

/* Definition for field MAX4_INDEX_MAX4_INDEX in Register MAX4_INDEX */
#define    MAX4_INDEX_MAX4_INDEX_START                                       (0U)
#define    MAX4_INDEX_MAX4_INDEX_END                                         (11U)

/* Definition for field I_SUM1_LSB_I_SUM1_LSB in Register I_SUM1_LSB */
#define    I_SUM1_LSB_I_SUM1_LSB_START                                       (0U)
#define    I_SUM1_LSB_I_SUM1_LSB_END                                         (31U)

/* Definition for field I_SUM1_MSB_I_SUM1_MSB in Register I_SUM1_MSB */
#define    I_SUM1_MSB_I_SUM1_MSB_START                                       (0U)
#define    I_SUM1_MSB_I_SUM1_MSB_END                                         (3U)

/* Definition for field I_SUM2_LSB_I_SUM2_LSB in Register I_SUM2_LSB */
#define    I_SUM2_LSB_I_SUM2_LSB_START                                       (0U)
#define    I_SUM2_LSB_I_SUM2_LSB_END                                         (31U)

/* Definition for field I_SUM2_MSB_I_SUM2_MSB in Register I_SUM2_MSB */
#define    I_SUM2_MSB_I_SUM2_MSB_START                                       (0U)
#define    I_SUM2_MSB_I_SUM2_MSB_END                                         (3U)

/* Definition for field I_SUM3_LSB_I_SUM3_LSB in Register I_SUM3_LSB */
#define    I_SUM3_LSB_I_SUM3_LSB_START                                       (0U)
#define    I_SUM3_LSB_I_SUM3_LSB_END                                         (31U)

/* Definition for field I_SUM3_MSB_I_SUM3_MSB in Register I_SUM3_MSB */
#define    I_SUM3_MSB_I_SUM3_MSB_START                                       (0U)
#define    I_SUM3_MSB_I_SUM3_MSB_END                                         (3U)

/* Definition for field I_SUM4_LSB_I_SUM4_LSB in Register I_SUM4_LSB */
#define    I_SUM4_LSB_I_SUM4_LSB_START                                       (0U)
#define    I_SUM4_LSB_I_SUM4_LSB_END                                         (31U)

/* Definition for field I_SUM4_MSB_I_SUM4_MSB in Register I_SUM4_MSB */
#define    I_SUM4_MSB_I_SUM4_MSB_START                                       (0U)
#define    I_SUM4_MSB_I_SUM4_MSB_END                                         (3U)

/* Definition for field Q_SUM1_LSB_Q_SUM1_LSB in Register Q_SUM1_LSB */
#define    Q_SUM1_LSB_Q_SUM1_LSB_START                                       (0U)
#define    Q_SUM1_LSB_Q_SUM1_LSB_END                                         (31U)

/* Definition for field Q_SUM1_MSB_Q_SUM1_MSB in Register Q_SUM1_MSB */
#define    Q_SUM1_MSB_Q_SUM1_MSB_START                                       (0U)
#define    Q_SUM1_MSB_Q_SUM1_MSB_END                                         (3U)

/* Definition for field Q_SUM2_LSB_Q_SUM2_LSB in Register Q_SUM2_LSB */
#define    Q_SUM2_LSB_Q_SUM2_LSB_START                                       (0U)
#define    Q_SUM2_LSB_Q_SUM2_LSB_END                                         (31U)

/* Definition for field Q_SUM2_MSB_Q_SUM2_MSB in Register Q_SUM2_MSB */
#define    Q_SUM2_MSB_Q_SUM2_MSB_START                                       (0U)
#define    Q_SUM2_MSB_Q_SUM2_MSB_END                                         (3U)

/* Definition for field Q_SUM3_LSB_Q_SUM3_LSB in Register Q_SUM3_LSB */
#define    Q_SUM3_LSB_Q_SUM3_LSB_START                                       (0U)
#define    Q_SUM3_LSB_Q_SUM3_LSB_END                                         (31U)

/* Definition for field Q_SUM3_MSB_Q_SUM3_MSB in Register Q_SUM3_MSB */
#define    Q_SUM3_MSB_Q_SUM3_MSB_START                                       (0U)
#define    Q_SUM3_MSB_Q_SUM3_MSB_END                                         (3U)

/* Definition for field Q_SUM4_LSB_Q_SUM4_LSB in Register Q_SUM4_LSB */
#define    Q_SUM4_LSB_Q_SUM4_LSB_START                                       (0U)
#define    Q_SUM4_LSB_Q_SUM4_LSB_END                                         (31U)

/* Definition for field Q_SUM4_MSB_Q_SUM4_MSB in Register Q_SUM4_MSB */
#define    Q_SUM4_MSB_Q_SUM4_MSB_START                                       (0U)
#define    Q_SUM4_MSB_Q_SUM4_MSB_END                                         (3U)

/* Definition for field FFTSUMDIV_FFTSUMDIV in Register FFTSUMDIV */
#define    FFTSUMDIV_FFTSUMDIV_START                                         (0U)
#define    FFTSUMDIV_FFTSUMDIV_END                                           (4U)

/* Definition for field MAX2D_OFFSET_DIM1_MAX2D_OFFSET_DIM1 in Register MAX2D_OFFSET_DIM1 */
#define    MAX2D_OFFSET_DIM1_MAX2D_OFFSET_DIM1_START                         (0U)
#define    MAX2D_OFFSET_DIM1_MAX2D_OFFSET_DIM1_END                           (23U)

/* Definition for field MAX2D_OFFSET_DIM2_MAX2D_OFFSET_DIM2 in Register MAX2D_OFFSET_DIM2 */
#define    MAX2D_OFFSET_DIM2_MAX2D_OFFSET_DIM2_START                         (0U)
#define    MAX2D_OFFSET_DIM2_MAX2D_OFFSET_DIM2_END                           (23U)

/* Definition for field CDF_CNT_THRESH_CDF_CNT_THRESH in Register CDF_CNT_THRESH */
#define    CDF_CNT_THRESH_CDF_CNT_THRESH_START                               (0U)
#define    CDF_CNT_THRESH_CDF_CNT_THRESH_END                                 (11U)

/* Definition for field STATS_RESERVED_1_STATS_RESERVED_1 in Register STATS_RESERVED_1 */
#define    STATS_RESERVED_1_STATS_RESERVED_1_START                           (0U)
#define    STATS_RESERVED_1_STATS_RESERVED_1_END                             (31U)

/* Definition for field STATS_RESERVED_2_STATS_RESERVED_2 in Register STATS_RESERVED_2 */
#define    STATS_RESERVED_2_STATS_RESERVED_2_START                           (0U)
#define    STATS_RESERVED_2_STATS_RESERVED_2_END                             (31U)

/* Definition for field STATS_RESERVED_3_STATS_RESERVED_3 in Register STATS_RESERVED_3 */
#define    STATS_RESERVED_3_STATS_RESERVED_3_START                           (0U)
#define    STATS_RESERVED_3_STATS_RESERVED_3_END                             (31U)

/* Definition for field STATS_RESERVED_4_STATS_RESERVED_4 in Register STATS_RESERVED_4 */
#define    STATS_RESERVED_4_STATS_RESERVED_4_START                           (0U)
#define    STATS_RESERVED_4_STATS_RESERVED_4_END                             (31U)

/* Definition for field STATS_RESERVED_5_STATS_RESERVED_5 in Register STATS_RESERVED_5 */
#define    STATS_RESERVED_5_STATS_RESERVED_5_START                           (0U)
#define    STATS_RESERVED_5_STATS_RESERVED_5_END                             (31U)

/* Definition for field CFAR_PEAKCNT_CFAR_PEAKCNT in Register CFAR_PEAKCNT */
#define    CFAR_PEAKCNT_CFAR_PEAKCNT_START                                   (0U)
#define    CFAR_PEAKCNT_CFAR_PEAKCNT_END                                     (11U)

/* Definition for field CFAR_DET_THR_CFAR_DET_THR in Register CFAR_DET_THR */
#define    CFAR_DET_THR_CFAR_DET_THR_START                                   (0U)
#define    CFAR_DET_THR_CFAR_DET_THR_END                                     (23U)

/* Definition for field CFAR_TEST_REG_CFAR_TEST_REG in Register CFAR_TEST_REG */
#define    CFAR_TEST_REG_CFAR_TEST_REG_START                                 (0U)
#define    CFAR_TEST_REG_CFAR_TEST_REG_END                                   (23U)

/* Definition for field CFAR_THRESH_CFAR_THRESH in Register CFAR_THRESH */
#define    CFAR_THRESH_CFAR_THRESH_START                                     (0U)
#define    CFAR_THRESH_CFAR_THRESH_END                                       (17U)

/* Definition for field CFAR_RESERVED_1_CFAR_RESERVED_1 in Register CFAR_RESERVED_1 */
#define    CFAR_RESERVED_1_CFAR_RESERVED_1_START                             (0U)
#define    CFAR_RESERVED_1_CFAR_RESERVED_1_END                               (31U)

/* Definition for field CFAR_RESERVED_2_CFAR_RESERVED_2 in Register CFAR_RESERVED_2 */
#define    CFAR_RESERVED_2_CFAR_RESERVED_2_START                             (0U)
#define    CFAR_RESERVED_2_CFAR_RESERVED_2_END                               (31U)

/* Definition for field CFAR_RESERVED_3_CFAR_RESERVED_3 in Register CFAR_RESERVED_3 */
#define    CFAR_RESERVED_3_CFAR_RESERVED_3_START                             (0U)
#define    CFAR_RESERVED_3_CFAR_RESERVED_3_END                               (31U)

/* Definition for field CFAR_RESERVED_4_CFAR_RESERVED_4 in Register CFAR_RESERVED_4 */
#define    CFAR_RESERVED_4_CFAR_RESERVED_4_START                             (0U)
#define    CFAR_RESERVED_4_CFAR_RESERVED_4_END                               (31U)

/* Definition for field CMP_EGE_K0123_CMP_EGE_K3 in Register CMP_EGE_K0123 */
#define    CMP_EGE_K0123_CMP_EGE_K3_START                                    (24U)
#define    CMP_EGE_K0123_CMP_EGE_K3_END                                      (28U)

/* Definition for field CMP_EGE_K0123_CMP_EGE_K2 in Register CMP_EGE_K0123 */
#define    CMP_EGE_K0123_CMP_EGE_K2_START                                    (16U)
#define    CMP_EGE_K0123_CMP_EGE_K2_END                                      (20U)

/* Definition for field CMP_EGE_K0123_CMP_EGE_K1 in Register CMP_EGE_K0123 */
#define    CMP_EGE_K0123_CMP_EGE_K1_START                                    (8U)
#define    CMP_EGE_K0123_CMP_EGE_K1_END                                      (12U)

/* Definition for field CMP_EGE_K0123_CMP_EGE_K0 in Register CMP_EGE_K0123 */
#define    CMP_EGE_K0123_CMP_EGE_K0_START                                    (0U)
#define    CMP_EGE_K0123_CMP_EGE_K0_END                                      (4U)

/* Definition for field CMP_EGE_K4567_CMP_EGE_K7 in Register CMP_EGE_K4567 */
#define    CMP_EGE_K4567_CMP_EGE_K7_START                                    (24U)
#define    CMP_EGE_K4567_CMP_EGE_K7_END                                      (28U)

/* Definition for field CMP_EGE_K4567_CMP_EGE_K6 in Register CMP_EGE_K4567 */
#define    CMP_EGE_K4567_CMP_EGE_K6_START                                    (16U)
#define    CMP_EGE_K4567_CMP_EGE_K6_END                                      (20U)

/* Definition for field CMP_EGE_K4567_CMP_EGE_K5 in Register CMP_EGE_K4567 */
#define    CMP_EGE_K4567_CMP_EGE_K5_START                                    (8U)
#define    CMP_EGE_K4567_CMP_EGE_K5_END                                      (12U)

/* Definition for field CMP_EGE_K4567_CMP_EGE_K4 in Register CMP_EGE_K4567 */
#define    CMP_EGE_K4567_CMP_EGE_K4_START                                    (0U)
#define    CMP_EGE_K4567_CMP_EGE_K4_END                                      (4U)

/* Definition for field MEM_INIT_START_HIST_ODD_RAM in Register MEM_INIT_START */
#define    MEM_INIT_START_HIST_ODD_RAM_START                                 (14U)
#define    MEM_INIT_START_HIST_ODD_RAM_END                                   (14U)

/* Definition for field MEM_INIT_START_HIST_EVEN_RAM in Register MEM_INIT_START */
#define    MEM_INIT_START_HIST_EVEN_RAM_START                                (13U)
#define    MEM_INIT_START_HIST_EVEN_RAM_END                                  (13U)

/* Definition for field MEM_INIT_START_PER_ITER_MAX_VAL_RAM in Register MEM_INIT_START */
#define    MEM_INIT_START_PER_ITER_MAX_VAL_RAM_START                         (12U)
#define    MEM_INIT_START_PER_ITER_MAX_VAL_RAM_END                           (12U)

/* Definition for field MEM_INIT_START_PER_SAMPLE_MAX_VAL_ODD_RAM in Register MEM_INIT_START */
#define    MEM_INIT_START_PER_SAMPLE_MAX_VAL_ODD_RAM_START                   (11U)
#define    MEM_INIT_START_PER_SAMPLE_MAX_VAL_ODD_RAM_END                     (11U)

/* Definition for field MEM_INIT_START_PER_SAMPLE_MAX_VAL_EVEN_RAM in Register MEM_INIT_START */
#define    MEM_INIT_START_PER_SAMPLE_MAX_VAL_EVEN_RAM_START                  (10U)
#define    MEM_INIT_START_PER_SAMPLE_MAX_VAL_EVEN_RAM_END                    (10U)

/* Definition for field MEM_INIT_START_WINDOW_RAM in Register MEM_INIT_START */
#define    MEM_INIT_START_WINDOW_RAM_START                                   (9U)
#define    MEM_INIT_START_WINDOW_RAM_END                                     (9U)

/* Definition for field MEM_INIT_START_PARAM_RAM in Register MEM_INIT_START */
#define    MEM_INIT_START_PARAM_RAM_START                                    (8U)
#define    MEM_INIT_START_PARAM_RAM_END                                      (8U)

/* Definition for field MEM_INIT_START_DMEM7 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM7_START                                        (7U)
#define    MEM_INIT_START_DMEM7_END                                          (7U)

/* Definition for field MEM_INIT_START_DMEM6 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM6_START                                        (6U)
#define    MEM_INIT_START_DMEM6_END                                          (6U)

/* Definition for field MEM_INIT_START_DMEM5 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM5_START                                        (5U)
#define    MEM_INIT_START_DMEM5_END                                          (5U)

/* Definition for field MEM_INIT_START_DMEM4 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM4_START                                        (4U)
#define    MEM_INIT_START_DMEM4_END                                          (4U)

/* Definition for field MEM_INIT_START_DMEM3 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM3_START                                        (3U)
#define    MEM_INIT_START_DMEM3_END                                          (3U)

/* Definition for field MEM_INIT_START_DMEM2 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM2_START                                        (2U)
#define    MEM_INIT_START_DMEM2_END                                          (2U)

/* Definition for field MEM_INIT_START_DMEM1 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM1_START                                        (1U)
#define    MEM_INIT_START_DMEM1_END                                          (1U)

/* Definition for field MEM_INIT_START_DMEM0 in Register MEM_INIT_START */
#define    MEM_INIT_START_DMEM0_START                                        (0U)
#define    MEM_INIT_START_DMEM0_END                                          (0U)

/* Definition for field MEM_INIT_DONE_HIST_ODD_RAM in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_HIST_ODD_RAM_START                                  (14U)
#define    MEM_INIT_DONE_HIST_ODD_RAM_END                                    (14U)

/* Definition for field MEM_INIT_DONE_HIST_EVEN_RAM in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_HIST_EVEN_RAM_START                                 (13U)
#define    MEM_INIT_DONE_HIST_EVEN_RAM_END                                   (13U)

/* Definition for field MEM_INIT_DONE_PER_ITERATION_MAX_VAL_RAM in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_PER_ITERATION_MAX_VAL_RAM_START                     (12U)
#define    MEM_INIT_DONE_PER_ITERATION_MAX_VAL_RAM_END                       (12U)

/* Definition for field MEM_INIT_DONE_PER_SAMPLE_MAX_VAL_ODD_RAM in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_PER_SAMPLE_MAX_VAL_ODD_RAM_START                    (11U)
#define    MEM_INIT_DONE_PER_SAMPLE_MAX_VAL_ODD_RAM_END                      (11U)

/* Definition for field MEM_INIT_DONE_PER_SAMPLE_MAX_VAL_EVEN_RAM in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_PER_SAMPLE_MAX_VAL_EVEN_RAM_START                   (10U)
#define    MEM_INIT_DONE_PER_SAMPLE_MAX_VAL_EVEN_RAM_END                     (10U)

/* Definition for field MEM_INIT_DONE_WINDOW_RAM in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_WINDOW_RAM_START                                    (9U)
#define    MEM_INIT_DONE_WINDOW_RAM_END                                      (9U)

/* Definition for field MEM_INIT_DONE_PARAM_RAM in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_PARAM_RAM_START                                     (8U)
#define    MEM_INIT_DONE_PARAM_RAM_END                                       (8U)

/* Definition for field MEM_INIT_DONE_DMEM7 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM7_START                                         (7U)
#define    MEM_INIT_DONE_DMEM7_END                                           (7U)

/* Definition for field MEM_INIT_DONE_DMEM6 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM6_START                                         (6U)
#define    MEM_INIT_DONE_DMEM6_END                                           (6U)

/* Definition for field MEM_INIT_DONE_DMEM5 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM5_START                                         (5U)
#define    MEM_INIT_DONE_DMEM5_END                                           (5U)

/* Definition for field MEM_INIT_DONE_DMEM4 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM4_START                                         (4U)
#define    MEM_INIT_DONE_DMEM4_END                                           (4U)

/* Definition for field MEM_INIT_DONE_DMEM3 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM3_START                                         (3U)
#define    MEM_INIT_DONE_DMEM3_END                                           (3U)

/* Definition for field MEM_INIT_DONE_DMEM2 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM2_START                                         (2U)
#define    MEM_INIT_DONE_DMEM2_END                                           (2U)

/* Definition for field MEM_INIT_DONE_DMEM1 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM1_START                                         (1U)
#define    MEM_INIT_DONE_DMEM1_END                                           (1U)

/* Definition for field MEM_INIT_DONE_DMEM0 in Register MEM_INIT_DONE */
#define    MEM_INIT_DONE_DMEM0_START                                         (0U)
#define    MEM_INIT_DONE_DMEM0_END                                           (0U)

/* Definition for field MEM_INIT_STATUS_HIST_ODD_RAM in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_HIST_ODD_RAM_START                                (14U)
#define    MEM_INIT_STATUS_HIST_ODD_RAM_END                                  (14U)

/* Definition for field MEM_INIT_STATUS_HIST_EVEN_RAM in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_HIST_EVEN_RAM_START                               (13U)
#define    MEM_INIT_STATUS_HIST_EVEN_RAM_END                                 (13U)

/* Definition for field MEM_INIT_STATUS_PER_ITERATION_MAX_VAL_RAM in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_PER_ITERATION_MAX_VAL_RAM_START                   (12U)
#define    MEM_INIT_STATUS_PER_ITERATION_MAX_VAL_RAM_END                     (12U)

/* Definition for field MEM_INIT_STATUS_PER_SAMPLE_MAX_VAL_ODD_RAM in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_PER_SAMPLE_MAX_VAL_ODD_RAM_START                  (11U)
#define    MEM_INIT_STATUS_PER_SAMPLE_MAX_VAL_ODD_RAM_END                    (11U)

/* Definition for field MEM_INIT_STATUS_PER_SAMPLE_MAX_VAL_EVEN_RAM in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_PER_SAMPLE_MAX_VAL_EVEN_RAM_START                 (10U)
#define    MEM_INIT_STATUS_PER_SAMPLE_MAX_VAL_EVEN_RAM_END                   (10U)

/* Definition for field MEM_INIT_STATUS_WINDOW_RAM in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_WINDOW_RAM_START                                  (9U)
#define    MEM_INIT_STATUS_WINDOW_RAM_END                                    (9U)

/* Definition for field MEM_INIT_STATUS_PARAM_RAM in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_PARAM_RAM_START                                   (8U)
#define    MEM_INIT_STATUS_PARAM_RAM_END                                     (8U)

/* Definition for field MEM_INIT_STATUS_DMEM7 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM7_START                                       (7U)
#define    MEM_INIT_STATUS_DMEM7_END                                         (7U)

/* Definition for field MEM_INIT_STATUS_DMEM6 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM6_START                                       (6U)
#define    MEM_INIT_STATUS_DMEM6_END                                         (6U)

/* Definition for field MEM_INIT_STATUS_DMEM5 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM5_START                                       (5U)
#define    MEM_INIT_STATUS_DMEM5_END                                         (5U)

/* Definition for field MEM_INIT_STATUS_DMEM4 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM4_START                                       (4U)
#define    MEM_INIT_STATUS_DMEM4_END                                         (4U)

/* Definition for field MEM_INIT_STATUS_DMEM3 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM3_START                                       (3U)
#define    MEM_INIT_STATUS_DMEM3_END                                         (3U)

/* Definition for field MEM_INIT_STATUS_DMEM2 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM2_START                                       (2U)
#define    MEM_INIT_STATUS_DMEM2_END                                         (2U)

/* Definition for field MEM_INIT_STATUS_DMEM1 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM1_START                                       (1U)
#define    MEM_INIT_STATUS_DMEM1_END                                         (1U)

/* Definition for field MEM_INIT_STATUS_DMEM0 in Register MEM_INIT_STATUS */
#define    MEM_INIT_STATUS_DMEM0_START                                       (0U)
#define    MEM_INIT_STATUS_DMEM0_END                                         (0U)

/* Definition for field LM_THRESH_VAL_DIMC_THRESH_VAL in Register LM_THRESH_VAL */
#define    LM_THRESH_VAL_DIMC_THRESH_VAL_START                               (16U)
#define    LM_THRESH_VAL_DIMC_THRESH_VAL_END                                 (31U)

/* Definition for field LM_THRESH_VAL_DIMB_THRESH_VAL in Register LM_THRESH_VAL */
#define    LM_THRESH_VAL_DIMB_THRESH_VAL_START                               (0U)
#define    LM_THRESH_VAL_DIMB_THRESH_VAL_END                                 (15U)

/* Definition for field LM_2DSTATS_BASE_ADDR_BASE_ADDR_DIMC in Register LM_2DSTATS_BASE_ADDR */
#define    LM_2DSTATS_BASE_ADDR_BASE_ADDR_DIMC_START                         (16U)
#define    LM_2DSTATS_BASE_ADDR_BASE_ADDR_DIMC_END                           (27U)

/* Definition for field LM_2DSTATS_BASE_ADDR_BASE_ADDR_DIMB in Register LM_2DSTATS_BASE_ADDR */
#define    LM_2DSTATS_BASE_ADDR_BASE_ADDR_DIMB_START                         (0U)
#define    LM_2DSTATS_BASE_ADDR_BASE_ADDR_DIMB_END                           (11U)

/* Definition for field HWA_SAFETY_EN_CFG_DMEM_PARITY_EN in Register HWA_SAFETY_EN */
#define    HWA_SAFETY_EN_CFG_DMEM_PARITY_EN_START                            (3U)
#define    HWA_SAFETY_EN_CFG_DMEM_PARITY_EN_END                              (3U)

/* Definition for field HWA_SAFETY_EN_CFG_WINDOW_RAM_PARITY_EN in Register HWA_SAFETY_EN */
#define    HWA_SAFETY_EN_CFG_WINDOW_RAM_PARITY_EN_START                      (2U)
#define    HWA_SAFETY_EN_CFG_WINDOW_RAM_PARITY_EN_END                        (2U)

/* Definition for field HWA_SAFETY_EN_CFG_FSM_LOCKSTEP_INV_EN in Register HWA_SAFETY_EN */
#define    HWA_SAFETY_EN_CFG_FSM_LOCKSTEP_INV_EN_START                       (1U)
#define    HWA_SAFETY_EN_CFG_FSM_LOCKSTEP_INV_EN_END                         (1U)

/* Definition for field HWA_SAFETY_EN_CFG_FSM_LOCKSTEP_EN in Register HWA_SAFETY_EN */
#define    HWA_SAFETY_EN_CFG_FSM_LOCKSTEP_EN_START                           (0U)
#define    HWA_SAFETY_EN_CFG_FSM_LOCKSTEP_EN_END                             (0U)

/* Definition for field HWA_SAFETY_ERR_MASK_FSM_LOCKSTEP in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_FSM_LOCKSTEP_START                            (9U)
#define    HWA_SAFETY_ERR_MASK_FSM_LOCKSTEP_END                              (9U)

/* Definition for field HWA_SAFETY_ERR_MASK_WINDOW_RAM in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_WINDOW_RAM_START                              (8U)
#define    HWA_SAFETY_ERR_MASK_WINDOW_RAM_END                                (8U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM7 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM7_START                                   (7U)
#define    HWA_SAFETY_ERR_MASK_DMEM7_END                                     (7U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM6 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM6_START                                   (6U)
#define    HWA_SAFETY_ERR_MASK_DMEM6_END                                     (6U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM5 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM5_START                                   (5U)
#define    HWA_SAFETY_ERR_MASK_DMEM5_END                                     (5U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM4 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM4_START                                   (4U)
#define    HWA_SAFETY_ERR_MASK_DMEM4_END                                     (4U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM3 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM3_START                                   (3U)
#define    HWA_SAFETY_ERR_MASK_DMEM3_END                                     (3U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM2 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM2_START                                   (2U)
#define    HWA_SAFETY_ERR_MASK_DMEM2_END                                     (2U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM1 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM1_START                                   (1U)
#define    HWA_SAFETY_ERR_MASK_DMEM1_END                                     (1U)

/* Definition for field HWA_SAFETY_ERR_MASK_DMEM0 in Register HWA_SAFETY_ERR_MASK */
#define    HWA_SAFETY_ERR_MASK_DMEM0_START                                   (0U)
#define    HWA_SAFETY_ERR_MASK_DMEM0_END                                     (0U)

/* Definition for field HWA_SAFETY_ERR_STATUS_FSM_LOCKSTEP in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_FSM_LOCKSTEP_START                          (9U)
#define    HWA_SAFETY_ERR_STATUS_FSM_LOCKSTEP_END                            (9U)

/* Definition for field HWA_SAFETY_ERR_STATUS_WINDOW_RAM in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_WINDOW_RAM_START                            (8U)
#define    HWA_SAFETY_ERR_STATUS_WINDOW_RAM_END                              (8U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM7 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM7_START                                 (7U)
#define    HWA_SAFETY_ERR_STATUS_DMEM7_END                                   (7U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM6 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM6_START                                 (6U)
#define    HWA_SAFETY_ERR_STATUS_DMEM6_END                                   (6U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM5 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM5_START                                 (5U)
#define    HWA_SAFETY_ERR_STATUS_DMEM5_END                                   (5U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM4 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM4_START                                 (4U)
#define    HWA_SAFETY_ERR_STATUS_DMEM4_END                                   (4U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM3 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM3_START                                 (3U)
#define    HWA_SAFETY_ERR_STATUS_DMEM3_END                                   (3U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM2 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM2_START                                 (2U)
#define    HWA_SAFETY_ERR_STATUS_DMEM2_END                                   (2U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM1 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM1_START                                 (1U)
#define    HWA_SAFETY_ERR_STATUS_DMEM1_END                                   (1U)

/* Definition for field HWA_SAFETY_ERR_STATUS_DMEM0 in Register HWA_SAFETY_ERR_STATUS */
#define    HWA_SAFETY_ERR_STATUS_DMEM0_START                                 (0U)
#define    HWA_SAFETY_ERR_STATUS_DMEM0_END                                   (0U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_FSM_LOCKSTEP in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_FSM_LOCKSTEP_START                      (9U)
#define    HWA_SAFETY_ERR_STATUS_RAW_FSM_LOCKSTEP_END                        (9U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_WINDOW_RAM in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_WINDOW_RAM_START                        (8U)
#define    HWA_SAFETY_ERR_STATUS_RAW_WINDOW_RAM_END                          (8U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM7 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM7_START                             (7U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM7_END                               (7U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM6 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM6_START                             (6U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM6_END                               (6U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM5 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM5_START                             (5U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM5_END                               (5U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM4 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM4_START                             (4U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM4_END                               (4U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM3 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM3_START                             (3U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM3_END                               (3U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM2 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM2_START                             (2U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM2_END                               (2U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM1 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM1_START                             (1U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM1_END                               (1U)

/* Definition for field HWA_SAFETY_ERR_STATUS_RAW_DMEM0 in Register HWA_SAFETY_ERR_STATUS_RAW */
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM0_START                             (0U)
#define    HWA_SAFETY_ERR_STATUS_RAW_DMEM0_END                               (0U)

/* Definition for field HWA_SAFETY_DMEM0_ERR_ADDR_DMEM0_ERR_ADDR in Register HWA_SAFETY_DMEM0_ERR_ADDR */
#define    HWA_SAFETY_DMEM0_ERR_ADDR_DMEM0_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM0_ERR_ADDR_DMEM0_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_DMEM1_ERR_ADDR_DMEM1_ERR_ADDR in Register HWA_SAFETY_DMEM1_ERR_ADDR */
#define    HWA_SAFETY_DMEM1_ERR_ADDR_DMEM1_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM1_ERR_ADDR_DMEM1_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_DMEM2_ERR_ADDR_DMEM2_ERR_ADDR in Register HWA_SAFETY_DMEM2_ERR_ADDR */
#define    HWA_SAFETY_DMEM2_ERR_ADDR_DMEM2_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM2_ERR_ADDR_DMEM2_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_DMEM3_ERR_ADDR_DMEM3_ERR_ADDR in Register HWA_SAFETY_DMEM3_ERR_ADDR */
#define    HWA_SAFETY_DMEM3_ERR_ADDR_DMEM3_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM3_ERR_ADDR_DMEM3_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_DMEM4_ERR_ADDR_DMEM4_ERR_ADDR in Register HWA_SAFETY_DMEM4_ERR_ADDR */
#define    HWA_SAFETY_DMEM4_ERR_ADDR_DMEM4_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM4_ERR_ADDR_DMEM4_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_DMEM5_ERR_ADDR_DMEM5_ERR_ADDR in Register HWA_SAFETY_DMEM5_ERR_ADDR */
#define    HWA_SAFETY_DMEM5_ERR_ADDR_DMEM5_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM5_ERR_ADDR_DMEM5_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_DMEM6_ERR_ADDR_DMEM6_ERR_ADDR in Register HWA_SAFETY_DMEM6_ERR_ADDR */
#define    HWA_SAFETY_DMEM6_ERR_ADDR_DMEM6_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM6_ERR_ADDR_DMEM6_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_DMEM7_ERR_ADDR_DMEM7_ERR_ADDR in Register HWA_SAFETY_DMEM7_ERR_ADDR */
#define    HWA_SAFETY_DMEM7_ERR_ADDR_DMEM7_ERR_ADDR_START                    (0U)
#define    HWA_SAFETY_DMEM7_ERR_ADDR_DMEM7_ERR_ADDR_END                      (9U)

/* Definition for field HWA_SAFETY_WINDOW_RAM_ERR_ADDR_WINDOW_RAM_ERR_ADDR in Register HWA_SAFETY_WINDOW_RAM_ERR_ADDR */
#define    HWA_SAFETY_WINDOW_RAM_ERR_ADDR_WINDOW_RAM_ERR_ADDR_START          (0U)
#define    HWA_SAFETY_WINDOW_RAM_ERR_ADDR_WINDOW_RAM_ERR_ADDR_END            (10U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM7 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM7_START                                 (7U)
#define    MEM_ACCESS_ERR_STATUS_DMEM7_END                                   (7U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM6 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM6_START                                 (6U)
#define    MEM_ACCESS_ERR_STATUS_DMEM6_END                                   (6U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM5 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM5_START                                 (5U)
#define    MEM_ACCESS_ERR_STATUS_DMEM5_END                                   (5U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM4 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM4_START                                 (4U)
#define    MEM_ACCESS_ERR_STATUS_DMEM4_END                                   (4U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM3 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM3_START                                 (3U)
#define    MEM_ACCESS_ERR_STATUS_DMEM3_END                                   (3U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM2 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM2_START                                 (2U)
#define    MEM_ACCESS_ERR_STATUS_DMEM2_END                                   (2U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM1 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM1_START                                 (1U)
#define    MEM_ACCESS_ERR_STATUS_DMEM1_END                                   (1U)

/* Definition for field MEM_ACCESS_ERR_STATUS_DMEM0 in Register MEM_ACCESS_ERR_STATUS */
#define    MEM_ACCESS_ERR_STATUS_DMEM0_START                                 (0U)
#define    MEM_ACCESS_ERR_STATUS_DMEM0_END                                   (0U)

/* Definition for field LOOP_CNT_LOOP_CNT_ALT in Register LOOP_CNT */
#define    LOOP_CNT_LOOP_CNT_ALT_START                                       (16U)
#define    LOOP_CNT_LOOP_CNT_ALT_END                                         (27U)

/* Definition for field LOOP_CNT_LOOP_CNT in Register LOOP_CNT */
#define    LOOP_CNT_LOOP_CNT_START                                           (0U)
#define    LOOP_CNT_LOOP_CNT_END                                             (11U)

/* Definition for field PARAMADDR_PARAMADDR in Register PARAMADDR */
#define    PARAMADDR_PARAMADDR_START                                         (0U)
#define    PARAMADDR_PARAMADDR_END                                           (5U)

/* Definition for field PARAMADDR_CPUINTR0_PARAMADDR in Register PARAMADDR_CPUINTR0 */
#define    PARAMADDR_CPUINTR0_PARAMADDR_START                                (0U)
#define    PARAMADDR_CPUINTR0_PARAMADDR_END                                  (5U)

/* Definition for field PARAMADDR_CPUINTR1_PARAMADDR in Register PARAMADDR_CPUINTR1 */
#define    PARAMADDR_CPUINTR1_PARAMADDR_START                                (0U)
#define    PARAMADDR_CPUINTR1_PARAMADDR_END                                  (5U)

/* Definition for field FSM_STATE_FSM_STATE in Register FSM_STATE */
#define    FSM_STATE_FSM_STATE_START                                         (0U)
#define    FSM_STATE_FSM_STATE_END                                           (2U)

/* Definition for field SINGLE_STEP_EN_SINGLE_STEP_EN in Register SINGLE_STEP_EN */
#define    SINGLE_STEP_EN_SINGLE_STEP_EN_START                               (0U)
#define    SINGLE_STEP_EN_SINGLE_STEP_EN_END                                 (0U)

/* Definition for field SINGLE_STEP_TRIG_SINGLE_STEP_TRIG in Register SINGLE_STEP_TRIG */
#define    SINGLE_STEP_TRIG_SINGLE_STEP_TRIG_START                           (0U)
#define    SINGLE_STEP_TRIG_SINGLE_STEP_TRIG_END                             (0U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_CTRL_TYPE in Register HWA_DMEM_A_BUS_SAFETY_CTRL */
#define    HWA_DMEM_A_BUS_SAFETY_CTRL_TYPE_START                             (16U)
#define    HWA_DMEM_A_BUS_SAFETY_CTRL_TYPE_END                               (23U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_CTRL_ERR_CLEAR in Register HWA_DMEM_A_BUS_SAFETY_CTRL */
#define    HWA_DMEM_A_BUS_SAFETY_CTRL_ERR_CLEAR_START                        (8U)
#define    HWA_DMEM_A_BUS_SAFETY_CTRL_ERR_CLEAR_END                          (8U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_CTRL_ENABLE in Register HWA_DMEM_A_BUS_SAFETY_CTRL */
#define    HWA_DMEM_A_BUS_SAFETY_CTRL_ENABLE_START                           (0U)
#define    HWA_DMEM_A_BUS_SAFETY_CTRL_ENABLE_END                             (2U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_SAFE in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_SAFE_START                               (24U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_SAFE_END                                 (31U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_MAIN in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_MAIN_START                               (16U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_MAIN_END                                 (23U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_DATA in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_DATA_START                               (8U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_DATA_END                                 (15U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_DED in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_DED_START                                (5U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_DED_END                                  (5U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_SEC in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_SEC_START                                (4U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_SEC_END                                  (4U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_SAFE_REQ in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_SAFE_REQ_START                    (3U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_SAFE_REQ_END                      (3U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_MAIN_REQ in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_MAIN_REQ_START                    (2U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_MAIN_REQ_END                      (2U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_SAFE in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_SAFE_START                        (1U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_SAFE_END                          (1U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_MAIN in Register HWA_DMEM_A_BUS_SAFETY_FI */
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_MAIN_START                        (0U)
#define    HWA_DMEM_A_BUS_SAFETY_FI_GLOBAL_MAIN_END                          (0U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_ERR_DED in Register HWA_DMEM_A_BUS_SAFETY_ERR */
#define    HWA_DMEM_A_BUS_SAFETY_ERR_DED_START                               (24U)
#define    HWA_DMEM_A_BUS_SAFETY_ERR_DED_END                                 (31U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_ERR_SEC in Register HWA_DMEM_A_BUS_SAFETY_ERR */
#define    HWA_DMEM_A_BUS_SAFETY_ERR_SEC_START                               (16U)
#define    HWA_DMEM_A_BUS_SAFETY_ERR_SEC_END                                 (23U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_ERR_COMP_CHECK in Register HWA_DMEM_A_BUS_SAFETY_ERR */
#define    HWA_DMEM_A_BUS_SAFETY_ERR_COMP_CHECK_START                        (8U)
#define    HWA_DMEM_A_BUS_SAFETY_ERR_COMP_CHECK_END                          (15U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_ERR_COMP_ERR in Register HWA_DMEM_A_BUS_SAFETY_ERR */
#define    HWA_DMEM_A_BUS_SAFETY_ERR_COMP_ERR_START                          (0U)
#define    HWA_DMEM_A_BUS_SAFETY_ERR_COMP_ERR_END                            (7U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0_D1 in Register HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0 */
#define    HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0_D1_START                     (8U)
#define    HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0_D1_END                       (15U)

/* Definition for field HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0_D0 in Register HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0 */
#define    HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0_D0_START                     (0U)
#define    HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0_D0_END                       (7U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_CTRL_TYPE in Register HWA_DMEM_B_BUS_SAFETY_CTRL */
#define    HWA_DMEM_B_BUS_SAFETY_CTRL_TYPE_START                             (16U)
#define    HWA_DMEM_B_BUS_SAFETY_CTRL_TYPE_END                               (23U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_CTRL_ERR_CLEAR in Register HWA_DMEM_B_BUS_SAFETY_CTRL */
#define    HWA_DMEM_B_BUS_SAFETY_CTRL_ERR_CLEAR_START                        (8U)
#define    HWA_DMEM_B_BUS_SAFETY_CTRL_ERR_CLEAR_END                          (8U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_CTRL_ENABLE in Register HWA_DMEM_B_BUS_SAFETY_CTRL */
#define    HWA_DMEM_B_BUS_SAFETY_CTRL_ENABLE_START                           (0U)
#define    HWA_DMEM_B_BUS_SAFETY_CTRL_ENABLE_END                             (2U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_SAFE in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_SAFE_START                               (24U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_SAFE_END                                 (31U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_MAIN in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_MAIN_START                               (16U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_MAIN_END                                 (23U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_DATA in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_DATA_START                               (8U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_DATA_END                                 (15U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_DED in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_DED_START                                (5U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_DED_END                                  (5U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_SEC in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_SEC_START                                (4U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_SEC_END                                  (4U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_SAFE_REQ in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_SAFE_REQ_START                    (3U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_SAFE_REQ_END                      (3U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_MAIN_REQ in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_MAIN_REQ_START                    (2U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_MAIN_REQ_END                      (2U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_SAFE in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_SAFE_START                        (1U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_SAFE_END                          (1U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_MAIN in Register HWA_DMEM_B_BUS_SAFETY_FI */
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_MAIN_START                        (0U)
#define    HWA_DMEM_B_BUS_SAFETY_FI_GLOBAL_MAIN_END                          (0U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_ERR_DED in Register HWA_DMEM_B_BUS_SAFETY_ERR */
#define    HWA_DMEM_B_BUS_SAFETY_ERR_DED_START                               (24U)
#define    HWA_DMEM_B_BUS_SAFETY_ERR_DED_END                                 (31U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_ERR_SEC in Register HWA_DMEM_B_BUS_SAFETY_ERR */
#define    HWA_DMEM_B_BUS_SAFETY_ERR_SEC_START                               (16U)
#define    HWA_DMEM_B_BUS_SAFETY_ERR_SEC_END                                 (23U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_ERR_COMP_CHECK in Register HWA_DMEM_B_BUS_SAFETY_ERR */
#define    HWA_DMEM_B_BUS_SAFETY_ERR_COMP_CHECK_START                        (8U)
#define    HWA_DMEM_B_BUS_SAFETY_ERR_COMP_CHECK_END                          (15U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_ERR_COMP_ERR in Register HWA_DMEM_B_BUS_SAFETY_ERR */
#define    HWA_DMEM_B_BUS_SAFETY_ERR_COMP_ERR_START                          (0U)
#define    HWA_DMEM_B_BUS_SAFETY_ERR_COMP_ERR_END                            (7U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0_D1 in Register HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0 */
#define    HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0_D1_START                     (8U)
#define    HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0_D1_END                       (15U)

/* Definition for field HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0_D0 in Register HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0 */
#define    HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0_D0_START                     (0U)
#define    HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0_D0_END                       (7U)

/* Definition for field LOCK0_KICK0 in Register LOCK0_KICK0 */
#define    LOCK0_KICK0_START                                                 (0U)
#define    LOCK0_KICK0_END                                                   (31U)

/* Definition for field LOCK0_KICK1 in Register LOCK0_KICK1 */
#define    LOCK0_KICK1_START                                                 (0U)
#define    LOCK0_KICK1_END                                                   (31U)

/* Definition for field PROXY_ERR in Register INTR_RAW_STATUS */
#define    PROXY_ERR_START                                                   (3U)
#define    PROXY_ERR_END                                                     (3U)

/* Definition for field KICK_ERR in Register INTR_RAW_STATUS */
#define    KICK_ERR_START                                                    (2U)
#define    KICK_ERR_END                                                      (2U)

/* Definition for field ADDR_ERR in Register INTR_RAW_STATUS */
#define    ADDR_ERR_START                                                    (1U)
#define    ADDR_ERR_END                                                      (1U)

/* Definition for field PROT_ERR in Register INTR_RAW_STATUS */
#define    PROT_ERR_START                                                    (0U)
#define    PROT_ERR_END                                                      (0U)

/* Definition for field ENABLED_PROXY_ERR in Register INTR_ENABLED_STATUS_CLEAR */
#define    ENABLED_PROXY_ERR_START                                           (3U)
#define    ENABLED_PROXY_ERR_END                                             (3U)

/* Definition for field ENABLED_KICK_ERR in Register INTR_ENABLED_STATUS_CLEAR */
#define    ENABLED_KICK_ERR_START                                            (2U)
#define    ENABLED_KICK_ERR_END                                              (2U)

/* Definition for field ENABLED_ADDR_ERR in Register INTR_ENABLED_STATUS_CLEAR */
#define    ENABLED_ADDR_ERR_START                                            (1U)
#define    ENABLED_ADDR_ERR_END                                              (1U)

/* Definition for field ENABLED_PROT_ERR in Register INTR_ENABLED_STATUS_CLEAR */
#define    ENABLED_PROT_ERR_START                                            (0U)
#define    ENABLED_PROT_ERR_END                                              (0U)

/* Definition for field PROXY_ERR_EN in Register INTR_ENABLE */
#define    PROXY_ERR_EN_START                                                (3U)
#define    PROXY_ERR_EN_END                                                  (3U)

/* Definition for field KICK_ERR_EN in Register INTR_ENABLE */
#define    KICK_ERR_EN_START                                                 (2U)
#define    KICK_ERR_EN_END                                                   (2U)

/* Definition for field ADDR_ERR_EN in Register INTR_ENABLE */
#define    ADDR_ERR_EN_START                                                 (1U)
#define    ADDR_ERR_EN_END                                                   (1U)

/* Definition for field PROT_ERR_EN in Register INTR_ENABLE */
#define    PROT_ERR_EN_START                                                 (0U)
#define    PROT_ERR_EN_END                                                   (0U)

/* Definition for field PROXY_ERR_EN_CLR in Register INTR_ENABLE_CLEAR */
#define    PROXY_ERR_EN_CLR_START                                            (3U)
#define    PROXY_ERR_EN_CLR_END                                              (3U)

/* Definition for field KICK_ERR_EN_CLR in Register INTR_ENABLE_CLEAR */
#define    KICK_ERR_EN_CLR_START                                             (2U)
#define    KICK_ERR_EN_CLR_END                                               (2U)

/* Definition for field ADDR_ERR_EN_CLR in Register INTR_ENABLE_CLEAR */
#define    ADDR_ERR_EN_CLR_START                                             (1U)
#define    ADDR_ERR_EN_CLR_END                                               (1U)

/* Definition for field PROT_ERR_EN_CLR in Register INTR_ENABLE_CLEAR */
#define    PROT_ERR_EN_CLR_START                                             (0U)
#define    PROT_ERR_EN_CLR_END                                               (0U)

/* Definition for field EOI_VECTOR in Register EOI */
#define    EOI_VECTOR_START                                                  (0U)
#define    EOI_VECTOR_END                                                    (7U)

/* Definition for field FAULT_ADDR in Register FAULT_ADDRESS */
#define    FAULT_ADDR_START                                                  (0U)
#define    FAULT_ADDR_END                                                    (31U)

/* Definition for field FAULT_NS in Register FAULT_TYPE_STATUS */
#define    FAULT_NS_START                                                    (6U)
#define    FAULT_NS_END                                                      (6U)

/* Definition for field FAULT_TYPE in Register FAULT_TYPE_STATUS */
#define    FAULT_TYPE_START                                                  (0U)
#define    FAULT_TYPE_END                                                    (5U)

/* Definition for field FAULT_XID in Register FAULT_ATTR_STATUS */
#define    FAULT_XID_START                                                   (20U)
#define    FAULT_XID_END                                                     (31U)

/* Definition for field FAULT_ROUTEID in Register FAULT_ATTR_STATUS */
#define    FAULT_ROUTEID_START                                               (8U)
#define    FAULT_ROUTEID_END                                                 (19U)

/* Definition for field FAULT_PRIVID in Register FAULT_ATTR_STATUS */
#define    FAULT_PRIVID_START                                                (0U)
#define    FAULT_PRIVID_END                                                  (7U)

/* Definition for field FAULT_CLR in Register FAULT_CLEAR */
#define    FAULT_CLR_START                                                   (0U)
#define    FAULT_CLR_END                                                     (0U)


/* Definition for CDF_CNT_BINNUM_SAMPLE in RAM HWA_HIST_THRESH_RAM  */
#define    CDF_CNT_BINNUM_SAMPLE_START                                       (24U)
#define    CDF_CNT_BINNUM_SAMPLE_END                                          (29U)

/* Definition for CDF_CNT_CDFVAL_SAMPLE in RAM HWA_HIST_THRESH_RAM  */
#define    CDF_CNT_CDFVAL_SAMPLE_START                                       (12U)
#define    CDF_CNT_CDFVAL_SAMPLE_END                                          (23U)

/* Definition for CDF_CNT_HISTVAL_SAMPLE in RAM HWA_HIST_THRESH_RAM  */
#define    CDF_CNT_HISTVAL_SAMPLE_START                                       (0U)
#define    CDF_CNT_HISTVAL_SAMPLE_END                                          (11U)

/** @struct hwaAccVal
*   @brief  HWA accumulator register data structure
*
*/
typedef volatile struct hwaAccVal_t
{
    uint32_t    accValLSB;        /* AddrOffset = 0x000 */
    uint32_t    accValMSB;        /* AddrOffset = 0x004 */
} hwaAccVal;


/**
 * @struct DSSHWACCRegs_t
 * @brief
 *   Module DSS_HW_ACC Register Definition
 * @details
 *   This structure is used to access the DSS_HW_ACC module registers.
 */
typedef volatile struct DSSHWACCRegs_t
{

    uint32_t      PID                                         ;      /* offset = 0x0000 */
    uint32_t      PARAM_RAM_IDX                               ;      /* offset = 0x0004 */
    uint32_t      PARAM_RAM_LOOP                              ;      /* offset = 0x0008 */
    uint32_t      PARAM_RAM_IDX_ALT                           ;      /* offset = 0x000C */
    uint32_t      PARAM_RAM_LOOP_ALT                          ;      /* offset = 0x0010 */
    uint32_t      HWA_ENABLE                                  ;      /* offset = 0x0014 */
    uint32_t      CS_CONFIG                                   ;      /* offset = 0x0018 */
    uint32_t      FW2DMA_TRIG                                 ;      /* offset = 0x001C */
    uint32_t      DMA2HWA_TRIG                                ;      /* offset = 0x0020 */
#if 0
    uint32_t      SIGDMACH0DONE                               ;      /* offset = 0x0024 */
    uint32_t      SIGDMACH1DONE                               ;      /* offset = 0x0028 */
    uint32_t      SIGDMACH2DONE                               ;      /* offset = 0x002C */
    uint32_t      SIGDMACH3DONE                               ;      /* offset = 0x0030 */
    uint32_t      SIGDMACH4DONE                               ;      /* offset = 0x0034 */
    uint32_t      SIGDMACH5DONE                               ;      /* offset = 0x0038 */
    uint32_t      SIGDMACH6DONE                               ;      /* offset = 0x003C */
    uint32_t      SIGDMACH7DONE                               ;      /* offset = 0x0040 */
    uint32_t      SIGDMACH8DONE                               ;      /* offset = 0x0044 */
    uint32_t      SIGDMACH9DONE                               ;      /* offset = 0x0048 */
    uint32_t      SIGDMACH10DONE                              ;      /* offset = 0x004C */
    uint32_t      SIGDMACH11DONE                              ;      /* offset = 0x0050 */
    uint32_t      SIGDMACH12DONE                              ;      /* offset = 0x0054 */
    uint32_t      SIGDMACH13DONE                              ;      /* offset = 0x0058 */
    uint32_t      SIGDMACH14DONE                              ;      /* offset = 0x005C */
    uint32_t      SIGDMACH15DONE                              ;      /* offset = 0x0060 */
    uint32_t      SIGDMACH16DONE                              ;      /* offset = 0x0064 */
    uint32_t      SIGDMACH17DONE                              ;      /* offset = 0x0068 */
    uint32_t      SIGDMACH18DONE                              ;      /* offset = 0x006C */
    uint32_t      SIGDMACH19DONE                              ;      /* offset = 0x0070 */
    uint32_t      SIGDMACH20DONE                              ;      /* offset = 0x0074 */
    uint32_t      SIGDMACH21DONE                              ;      /* offset = 0x0078 */
    uint32_t      SIGDMACH22DONE                              ;      /* offset = 0x007C */
    uint32_t      SIGDMACH23DONE                              ;      /* offset = 0x0080 */
    uint32_t      SIGDMACH24DONE                              ;      /* offset = 0x0084 */
    uint32_t      SIGDMACH25DONE                              ;      /* offset = 0x0088 */
    uint32_t      SIGDMACH26DONE                              ;      /* offset = 0x008C */
    uint32_t      SIGDMACH27DONE                              ;      /* offset = 0x0090 */
    uint32_t      SIGDMACH28DONE                              ;      /* offset = 0x0094 */
    uint32_t      SIGDMACH29DONE                              ;      /* offset = 0x0098 */
    uint32_t      SIGDMACH30DONE                              ;      /* offset = 0x009C */
    uint32_t      SIGDMACH31DONE                              ;      /* offset = 0x00A0 */
#endif
    uint32_t      SIGDMACHDONE[32]                            ;      /* offset = 0x0024 - 0x00A0 */
    uint32_t      FW2HWA_TRIG_0                               ;      /* offset = 0x00A4 */
    uint32_t      FW2HWA_TRIG_1                               ;      /* offset = 0x00A8 */
    uint32_t      CS_FW2ACC_TRIG                              ;      /* offset = 0x00AC */
#if 0
    uint32_t      BPM_PATTERN_0                               ;      /* offset = 0x00B0 */
    uint32_t      BPM_PATTERN_1                               ;      /* offset = 0x00B4 */
    uint32_t      BPM_PATTERN_2                               ;      /* offset = 0x00B8 */
    uint32_t      BPM_PATTERN_3                               ;      /* offset = 0x00BC */
    uint32_t      BPM_PATTERN_4                               ;      /* offset = 0x00C0 */
    uint32_t      BPM_PATTERN_5                               ;      /* offset = 0x00C4 */
    uint32_t      BPM_PATTERN_6                               ;      /* offset = 0x00C8 */
    uint32_t      BPM_PATTERN_7                               ;      /* offset = 0x00CC */
#endif
    uint32_t      BPM_PATTERN[8]                              ;      /* offset = 0x00B0 - 0x00CC */
    uint32_t      BPM_RATE                                    ;      /* offset = 0x00D0 */
#if 0
    uint32_t      PARAM_DONE_SET_STATUS_0                     ;      /* offset = 0x00D4 */
    uint32_t      PARAM_DONE_SET_STATUS_1                     ;      /* offset = 0x00D8 */
#endif
    uint32_t      PARAM_DONE_SET_STATUS[2]                    ;      /* offset = 0x00D4 - 0x00D8 */
#if 0
    uint32_t      PARAM_DONE_CLR_0                            ;      /* offset = 0x00DC */
    uint32_t      PARAM_DONE_CLR_1                            ;      /* offset = 0x00E0 */
#endif
    uint32_t      PARAM_DONE_CLR[2]                           ;      /* offset = 0x00DC - 0x00E0 */
#if 0
    uint32_t      TRIGGER_SET_STATUS_0                        ;      /* offset = 0x00E4 */
    uint32_t      TRIGGER_SET_STATUS_1                        ;      /* offset = 0x00E8 */
#endif
    uint32_t      TRIGGER_SET_STATUS[2]                       ;      /* offset = 0x00E4 - 0x00E8 */
#if 0
    uint32_t      TRIGGER_SET_IN_CLR_0                        ;      /* offset = 0x00EC */
    uint32_t      TRIGGER_SET_IN_CLR_1                        ;      /* offset = 0x00F0 */
#endif
    uint32_t      TRIGGER_SET_IN_CLR[2]                       ;      /* offset = 0x00EC - 0x00F0 */
    uint32_t      DC_EST_RESET_SW                             ;      /* offset = 0x00F4 */
    uint32_t      DC_EST_CTRL                                 ;      /* offset = 0x00F8 */
#if 0
    uint32_t      DC_EST_I_0_VAL                              ;      /* offset = 0x00FC */
    uint32_t      DC_EST_I_1_VAL                              ;      /* offset = 0x0100 */
    uint32_t      DC_EST_I_2_VAL                              ;      /* offset = 0x0104 */
    uint32_t      DC_EST_I_3_VAL                              ;      /* offset = 0x0108 */
    uint32_t      DC_EST_I_4_VAL                              ;      /* offset = 0x010C */
    uint32_t      DC_EST_I_5_VAL                              ;      /* offset = 0x0110 */
    uint32_t      DC_EST_I_6_VAL                              ;      /* offset = 0x0114 */
    uint32_t      DC_EST_I_7_VAL                              ;      /* offset = 0x0118 */
    uint32_t      DC_EST_I_8_VAL                              ;      /* offset = 0x011C */
    uint32_t      DC_EST_I_9_VAL                              ;      /* offset = 0x0120 */
    uint32_t      DC_EST_I_10_VAL                             ;      /* offset = 0x0124 */
    uint32_t      DC_EST_I_11_VAL                             ;      /* offset = 0x0128 */
#endif
    uint32_t      DC_EST_I_VAL[12]                            ;      /* offset = 0x00FC - 0x0128 */
#if 0
    uint32_t      DC_EST_Q_0_VAL                              ;      /* offset = 0x012C */
    uint32_t      DC_EST_Q_1_VAL                              ;      /* offset = 0x0130 */
    uint32_t      DC_EST_Q_2_VAL                              ;      /* offset = 0x0134 */
    uint32_t      DC_EST_Q_3_VAL                              ;      /* offset = 0x0138 */
    uint32_t      DC_EST_Q_4_VAL                              ;      /* offset = 0x013C */
    uint32_t      DC_EST_Q_5_VAL                              ;      /* offset = 0x0140 */
    uint32_t      DC_EST_Q_6_VAL                              ;      /* offset = 0x0144 */
    uint32_t      DC_EST_Q_7_VAL                              ;      /* offset = 0x0148 */
    uint32_t      DC_EST_Q_8_VAL                              ;      /* offset = 0x014C */
    uint32_t      DC_EST_Q_9_VAL                              ;      /* offset = 0x0150 */
    uint32_t      DC_EST_Q_10_VAL                             ;      /* offset = 0x0154 */
    uint32_t      DC_EST_Q_11_VAL                             ;      /* offset = 0x0158 */
#endif
    uint32_t      DC_EST_Q_VAL[12]                            ;      /* offset = 0x012C - 0x0158 */
#if 0
    uint32_t      DC_ACC_I_0_VAL_LSB                          ;      /* offset = 0x015C */
    uint32_t      DC_ACC_I_0_VAL_MSB                          ;      /* offset = 0x0160 */
    uint32_t      DC_ACC_I_1_VAL_LSB                          ;      /* offset = 0x0164 */
    uint32_t      DC_ACC_I_1_VAL_MSB                          ;      /* offset = 0x0168 */
    uint32_t      DC_ACC_I_2_VAL_LSB                          ;      /* offset = 0x016C */
    uint32_t      DC_ACC_I_2_VAL_MSB                          ;      /* offset = 0x0170 */
    uint32_t      DC_ACC_I_3_VAL_LSB                          ;      /* offset = 0x0174 */
    uint32_t      DC_ACC_I_3_VAL_MSB                          ;      /* offset = 0x0178 */
    uint32_t      DC_ACC_I_4_VAL_LSB                          ;      /* offset = 0x017C */
    uint32_t      DC_ACC_I_4_VAL_MSB                          ;      /* offset = 0x0180 */
    uint32_t      DC_ACC_I_5_VAL_LSB                          ;      /* offset = 0x0184 */
    uint32_t      DC_ACC_I_5_VAL_MSB                          ;      /* offset = 0x0188 */
    uint32_t      DC_ACC_I_6_VAL_LSB                          ;      /* offset = 0x018C */
    uint32_t      DC_ACC_I_6_VAL_MSB                          ;      /* offset = 0x0190 */
    uint32_t      DC_ACC_I_7_VAL_LSB                          ;      /* offset = 0x0194 */
    uint32_t      DC_ACC_I_7_VAL_MSB                          ;      /* offset = 0x0198 */
    uint32_t      DC_ACC_I_8_VAL_LSB                          ;      /* offset = 0x019C */
    uint32_t      DC_ACC_I_8_VAL_MSB                          ;      /* offset = 0x01A0 */
    uint32_t      DC_ACC_I_9_VAL_LSB                          ;      /* offset = 0x01A4 */
    uint32_t      DC_ACC_I_9_VAL_MSB                          ;      /* offset = 0x01A8 */
    uint32_t      DC_ACC_I_10_VAL_LSB                         ;      /* offset = 0x01AC */
    uint32_t      DC_ACC_I_10_VAL_MSB                         ;      /* offset = 0x01B0 */
    uint32_t      DC_ACC_I_11_VAL_LSB                         ;      /* offset = 0x01B4 */
    uint32_t      DC_ACC_I_11_VAL_MSB                         ;      /* offset = 0x01B8 */
#endif
    hwaAccVal     DC_ACC_I_VAL[12]                ;      /* offset = 0x015C - 0x01B8 */
#if 0
    uint32_t      DC_ACC_Q_0_VAL_LSB                          ;      /* offset = 0x01BC */
    uint32_t      DC_ACC_Q_0_VAL_MSB                          ;      /* offset = 0x01C0 */
    uint32_t      DC_ACC_Q_1_VAL_LSB                          ;      /* offset = 0x01C4 */
    uint32_t      DC_ACC_Q_1_VAL_MSB                          ;      /* offset = 0x01C8 */
    uint32_t      DC_ACC_Q_2_VAL_LSB                          ;      /* offset = 0x01CC */
    uint32_t      DC_ACC_Q_2_VAL_MSB                          ;      /* offset = 0x01D0 */
    uint32_t      DC_ACC_Q_3_VAL_LSB                          ;      /* offset = 0x01D4 */
    uint32_t      DC_ACC_Q_3_VAL_MSB                          ;      /* offset = 0x01D8 */
    uint32_t      DC_ACC_Q_4_VAL_LSB                          ;      /* offset = 0x01DC */
    uint32_t      DC_ACC_Q_4_VAL_MSB                          ;      /* offset = 0x01E0 */
    uint32_t      DC_ACC_Q_5_VAL_LSB                          ;      /* offset = 0x01E4 */
    uint32_t      DC_ACC_Q_5_VAL_MSB                          ;      /* offset = 0x01E8 */
    uint32_t      DC_ACC_Q_6_VAL_LSB                          ;      /* offset = 0x01EC */
    uint32_t      DC_ACC_Q_6_VAL_MSB                          ;      /* offset = 0x01F0 */
    uint32_t      DC_ACC_Q_7_VAL_LSB                          ;      /* offset = 0x01F4 */
    uint32_t      DC_ACC_Q_7_VAL_MSB                          ;      /* offset = 0x01F8 */
    uint32_t      DC_ACC_Q_8_VAL_LSB                          ;      /* offset = 0x01FC */
    uint32_t      DC_ACC_Q_8_VAL_MSB                          ;      /* offset = 0x0200 */
    uint32_t      DC_ACC_Q_9_VAL_LSB                          ;      /* offset = 0x0204 */
    uint32_t      DC_ACC_Q_9_VAL_MSB                          ;      /* offset = 0x0208 */
    uint32_t      DC_ACC_Q_10_VAL_LSB                         ;      /* offset = 0x020C */
    uint32_t      DC_ACC_Q_10_VAL_MSB                         ;      /* offset = 0x0210 */
    uint32_t      DC_ACC_Q_11_VAL_LSB                         ;      /* offset = 0x0214 */
    uint32_t      DC_ACC_Q_11_VAL_MSB                         ;      /* offset = 0x0218 */
#endif
    hwaAccVal     DC_ACC_Q_VAL[12]                ;      /* offset = 0x01BC - 0x0218 */
    uint32_t      DC_ACC_CLIP_STATUS                          ;      /* offset = 0x021C */
    uint32_t      DC_EST_CLIP_STATUS                          ;      /* offset = 0x0220 */
#if 0
    uint32_t      DC_I0_SW                                    ;      /* offset = 0x0224 */
    uint32_t      DC_I1_SW                                    ;      /* offset = 0x0228 */
    uint32_t      DC_I2_SW                                    ;      /* offset = 0x022C */
    uint32_t      DC_I3_SW                                    ;      /* offset = 0x0230 */
    uint32_t      DC_I4_SW                                    ;      /* offset = 0x0234 */
    uint32_t      DC_I5_SW                                    ;      /* offset = 0x0238 */
    uint32_t      DC_I6_SW                                    ;      /* offset = 0x023C */
    uint32_t      DC_I7_SW                                    ;      /* offset = 0x0240 */
    uint32_t      DC_I8_SW                                    ;      /* offset = 0x0244 */
    uint32_t      DC_I9_SW                                    ;      /* offset = 0x0248 */
    uint32_t      DC_I10_SW                                   ;      /* offset = 0x024C */
    uint32_t      DC_I11_SW                                   ;      /* offset = 0x0250 */
#endif
    uint32_t      DC_I_SW[12]                                 ;      /* offset = 0x0224 - 0x0250 */
#if 0
    uint32_t      DC_Q0_SW                                    ;      /* offset = 0x0254 */
    uint32_t      DC_Q1_SW                                    ;      /* offset = 0x0258 */
    uint32_t      DC_Q2_SW                                    ;      /* offset = 0x025C */
    uint32_t      DC_Q3_SW                                    ;      /* offset = 0x0260 */
    uint32_t      DC_Q4_SW                                    ;      /* offset = 0x0264 */
    uint32_t      DC_Q5_SW                                    ;      /* offset = 0x0268 */
    uint32_t      DC_Q6_SW                                    ;      /* offset = 0x026C */
    uint32_t      DC_Q7_SW                                    ;      /* offset = 0x0270 */
    uint32_t      DC_Q8_SW                                    ;      /* offset = 0x0274 */
    uint32_t      DC_Q9_SW                                    ;      /* offset = 0x0278 */
    uint32_t      DC_Q10_SW                                   ;      /* offset = 0x027C */
    uint32_t      DC_Q11_SW                                   ;      /* offset = 0x0280 */
#endif
    uint32_t      DC_Q_SW[12]                                 ;      /* offset = 0x0254 - 0x0280 */
    uint32_t      DC_SUB_CLIP                                 ;      /* offset = 0x0284 */
    uint32_t      DC_RESERVED_2                               ;      /* offset = 0x0288 */
    uint32_t      DC_RESERVED_3                               ;      /* offset = 0x028C */
    uint32_t      DC_RESERVED_4                               ;      /* offset = 0x0290 */
    uint32_t      DC_RESERVED_5                               ;      /* offset = 0x0294 */
    uint32_t      INTF_STATS_RESET_SW                         ;      /* offset = 0x0298 */
    uint32_t      INTF_STATS_CTRL                             ;      /* offset = 0x029C */
#if 0
    uint32_t      INTF_LOC_THRESH_MAG0_VAL                    ;      /* offset = 0x02A0 */
    uint32_t      INTF_LOC_THRESH_MAG1_VAL                    ;      /* offset = 0x02A4 */
    uint32_t      INTF_LOC_THRESH_MAG2_VAL                    ;      /* offset = 0x02A8 */
    uint32_t      INTF_LOC_THRESH_MAG3_VAL                    ;      /* offset = 0x02AC */
    uint32_t      INTF_LOC_THRESH_MAG4_VAL                    ;      /* offset = 0x02B0 */
    uint32_t      INTF_LOC_THRESH_MAG5_VAL                    ;      /* offset = 0x02B4 */
    uint32_t      INTF_LOC_THRESH_MAG6_VAL                    ;      /* offset = 0x02B8 */
    uint32_t      INTF_LOC_THRESH_MAG7_VAL                    ;      /* offset = 0x02BC */
    uint32_t      INTF_LOC_THRESH_MAG8_VAL                    ;      /* offset = 0x02C0 */
    uint32_t      INTF_LOC_THRESH_MAG9_VAL                    ;      /* offset = 0x02C4 */
    uint32_t      INTF_LOC_THRESH_MAG10_VAL                   ;      /* offset = 0x02C8 */
    uint32_t      INTF_LOC_THRESH_MAG11_VAL                   ;      /* offset = 0x02CC */
#endif
    uint32_t      INTF_LOC_THRESH_MAG_VAL[12]                 ;      /* offset = 0x02A0 - 0x02CC */
#if 0
    uint32_t      INTF_LOC_THRESH_MAGDIFF0_VAL                ;      /* offset = 0x02D0 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF1_VAL                ;      /* offset = 0x02D4 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF2_VAL                ;      /* offset = 0x02D8 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF3_VAL                ;      /* offset = 0x02DC */
    uint32_t      INTF_LOC_THRESH_MAGDIFF4_VAL                ;      /* offset = 0x02E0 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF5_VAL                ;      /* offset = 0x02E4 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF6_VAL                ;      /* offset = 0x02E8 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF7_VAL                ;      /* offset = 0x02EC */
    uint32_t      INTF_LOC_THRESH_MAGDIFF8_VAL                ;      /* offset = 0x02F0 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF9_VAL                ;      /* offset = 0x02F4 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF10_VAL               ;      /* offset = 0x02F8 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF11_VAL               ;      /* offset = 0x02FC */
#endif
    uint32_t      INTF_LOC_THRESH_MAGDIFF_VAL[12]             ;      /* offset = 0x02D0 - 0x02FC */
    uint32_t      INTF_LOC_COUNT_ALL_CHIRP                    ;      /* offset = 0x0300 */
    uint32_t      INTF_LOC_COUNT_ALL_FRAME                    ;      /* offset = 0x0304 */
#if 0
    uint32_t      INTF_STATS_MAG_ACC_0_LSB                    ;      /* offset = 0x0308 */
    uint32_t      INTF_STATS_MAG_ACC_0_MSB                    ;      /* offset = 0x030C */
    uint32_t      INTF_STATS_MAG_ACC_1_LSB                    ;      /* offset = 0x0310 */
    uint32_t      INTF_STATS_MAG_ACC_1_MSB                    ;      /* offset = 0x0314 */
    uint32_t      INTF_STATS_MAG_ACC_2_LSB                    ;      /* offset = 0x0318 */
    uint32_t      INTF_STATS_MAG_ACC_2_MSB                    ;      /* offset = 0x031C */
    uint32_t      INTF_STATS_MAG_ACC_3_LSB                    ;      /* offset = 0x0320 */
    uint32_t      INTF_STATS_MAG_ACC_3_MSB                    ;      /* offset = 0x0324 */
    uint32_t      INTF_STATS_MAG_ACC_4_LSB                    ;      /* offset = 0x0328 */
    uint32_t      INTF_STATS_MAG_ACC_4_MSB                    ;      /* offset = 0x032C */
    uint32_t      INTF_STATS_MAG_ACC_5_LSB                    ;      /* offset = 0x0330 */
    uint32_t      INTF_STATS_MAG_ACC_5_MSB                    ;      /* offset = 0x0334 */
    uint32_t      INTF_STATS_MAG_ACC_6_LSB                    ;      /* offset = 0x0338 */
    uint32_t      INTF_STATS_MAG_ACC_6_MSB                    ;      /* offset = 0x033C */
    uint32_t      INTF_STATS_MAG_ACC_7_LSB                    ;      /* offset = 0x0340 */
    uint32_t      INTF_STATS_MAG_ACC_7_MSB                    ;      /* offset = 0x0344 */
    uint32_t      INTF_STATS_MAG_ACC_8_LSB                    ;      /* offset = 0x0348 */
    uint32_t      INTF_STATS_MAG_ACC_8_MSB                    ;      /* offset = 0x034C */
    uint32_t      INTF_STATS_MAG_ACC_9_LSB                    ;      /* offset = 0x0350 */
    uint32_t      INTF_STATS_MAG_ACC_9_MSB                    ;      /* offset = 0x0354 */
    uint32_t      INTF_STATS_MAG_ACC_10_LSB                   ;      /* offset = 0x0358 */
    uint32_t      INTF_STATS_MAG_ACC_10_MSB                   ;      /* offset = 0x035C */
    uint32_t      INTF_STATS_MAG_ACC_11_LSB                   ;      /* offset = 0x0360 */
    uint32_t      INTF_STATS_MAG_ACC_11_MSB                   ;      /* offset = 0x0364 */
#endif
    hwaAccVal     INTF_STATS_MAG_ACC[12]          ;      /* offset = 0x0308 - 0x0364 */
#if 0
    uint32_t      INTF_STATS_MAGDIFF_ACC_0_LSB                ;      /* offset = 0x0368 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_0_MSB                ;      /* offset = 0x036C */
    uint32_t      INTF_STATS_MAGDIFF_ACC_1_LSB                ;      /* offset = 0x0370 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_1_MSB                ;      /* offset = 0x0374 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_2_LSB                ;      /* offset = 0x0378 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_2_MSB                ;      /* offset = 0x037C */
    uint32_t      INTF_STATS_MAGDIFF_ACC_3_LSB                ;      /* offset = 0x0380 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_3_MSB                ;      /* offset = 0x0384 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_4_LSB                ;      /* offset = 0x0388 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_4_MSB                ;      /* offset = 0x038C */
    uint32_t      INTF_STATS_MAGDIFF_ACC_5_LSB                ;      /* offset = 0x0390 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_5_MSB                ;      /* offset = 0x0394 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_6_LSB                ;      /* offset = 0x0398 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_6_MSB                ;      /* offset = 0x039C */
    uint32_t      INTF_STATS_MAGDIFF_ACC_7_LSB                ;      /* offset = 0x03A0 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_7_MSB                ;      /* offset = 0x03A4 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_8_LSB                ;      /* offset = 0x03A8 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_8_MSB                ;      /* offset = 0x03AC */
    uint32_t      INTF_STATS_MAGDIFF_ACC_9_LSB                ;      /* offset = 0x03B0 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_9_MSB                ;      /* offset = 0x03B4 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_10_LSB               ;      /* offset = 0x03B8 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_10_MSB               ;      /* offset = 0x03BC */
    uint32_t      INTF_STATS_MAGDIFF_ACC_11_LSB               ;      /* offset = 0x03C0 */
    uint32_t      INTF_STATS_MAGDIFF_ACC_11_MSB               ;      /* offset = 0x03C4 */
#endif
    hwaAccVal     INTF_STATS_MAGDIFF_ACC[12]        ;      /* offset = 0x0368 - 0x03C4 */
#if 0
    uint32_t      INTF_LOC_THRESH_MAG0_SW                     ;      /* offset = 0x03C8 */
    uint32_t      INTF_LOC_THRESH_MAG1_SW                     ;      /* offset = 0x03CC */
    uint32_t      INTF_LOC_THRESH_MAG2_SW                     ;      /* offset = 0x03D0 */
    uint32_t      INTF_LOC_THRESH_MAG3_SW                     ;      /* offset = 0x03D4 */
    uint32_t      INTF_LOC_THRESH_MAG4_SW                     ;      /* offset = 0x03D8 */
    uint32_t      INTF_LOC_THRESH_MAG5_SW                     ;      /* offset = 0x03DC */
    uint32_t      INTF_LOC_THRESH_MAG6_SW                     ;      /* offset = 0x03E0 */
    uint32_t      INTF_LOC_THRESH_MAG7_SW                     ;      /* offset = 0x03E4 */
    uint32_t      INTF_LOC_THRESH_MAG8_SW                     ;      /* offset = 0x03E8 */
    uint32_t      INTF_LOC_THRESH_MAG9_SW                     ;      /* offset = 0x03EC */
    uint32_t      INTF_LOC_THRESH_MAG10_SW                    ;      /* offset = 0x03F0 */
    uint32_t      INTF_LOC_THRESH_MAG11_SW                    ;      /* offset = 0x03F4 */
#endif
    uint32_t      INTF_LOC_THRESH_MAG_SW[12]                  ;      /* offset = 0x03C8 - 0x03F4 */
#if 0
    uint32_t      INTF_LOC_THRESH_MAGDIFF0_SW                 ;      /* offset = 0x03F8 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF1_SW                 ;      /* offset = 0x03FC */
    uint32_t      INTF_LOC_THRESH_MAGDIFF2_SW                 ;      /* offset = 0x0400 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF3_SW                 ;      /* offset = 0x0404 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF4_SW                 ;      /* offset = 0x0408 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF5_SW                 ;      /* offset = 0x040C */
    uint32_t      INTF_LOC_THRESH_MAGDIFF6_SW                 ;      /* offset = 0x0410 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF7_SW                 ;      /* offset = 0x0414 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF8_SW                 ;      /* offset = 0x0418 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF9_SW                 ;      /* offset = 0x041C */
    uint32_t      INTF_LOC_THRESH_MAGDIFF10_SW                ;      /* offset = 0x0420 */
    uint32_t      INTF_LOC_THRESH_MAGDIFF11_SW                ;      /* offset = 0x0424 */
#endif
    uint32_t      INTF_LOC_THRESH_MAGDIFF_SW[12]              ;      /* offset = 0x03F8 - 0x0424 */
    uint32_t      INTF_STATS_ACC_CLIP_STATUS                  ;      /* offset = 0x0428 */
    uint32_t      INTF_STATS_THRESH_CLIP_STATUS               ;      /* offset = 0x042C */
#if 0
    uint32_t      INTF_MITG_WINDOW_PARAM_0                    ;      /* offset = 0x0430 */
    uint32_t      INTF_MITG_WINDOW_PARAM_1                    ;      /* offset = 0x0434 */
    uint32_t      INTF_MITG_WINDOW_PARAM_2                    ;      /* offset = 0x0438 */
    uint32_t      INTF_MITG_WINDOW_PARAM_3                    ;      /* offset = 0x043C */
    uint32_t      INTF_MITG_WINDOW_PARAM_4                    ;      /* offset = 0x0440 */
#endif
    uint32_t      INTF_MITG_WINDOW_PARAM[5]                   ;      /* offset = 0x0430 - 0x0440 */
    uint32_t      INTF_STATS_SUM_MAG_VAL                      ;      /* offset = 0x0444 */
    uint32_t      INTF_STATS_SUM_MAG_VAL_CLIP_STATUS          ;      /* offset = 0x0448 */
    uint32_t      INTF_STATS_SUM_MAGDIFF_VAL                  ;      /* offset = 0x044C */
    uint32_t      INTF_STATS_SUM_MAGDIFF_VAL_CLIP_STATUS        ;      /* offset = 0x0450 */
    uint32_t      INTERF_RESERVED_5                           ;      /* offset = 0x0454 */
#if 0
    uint32_t      ICMULT_SCALE0                               ;      /* offset = 0x0458 */
    uint32_t      ICMULT_SCALE1                               ;      /* offset = 0x045C */
    uint32_t      ICMULT_SCALE2                               ;      /* offset = 0x0460 */
    uint32_t      ICMULT_SCALE3                               ;      /* offset = 0x0464 */
    uint32_t      ICMULT_SCALE4                               ;      /* offset = 0x0468 */
    uint32_t      ICMULT_SCALE5                               ;      /* offset = 0x046C */
    uint32_t      ICMULT_SCALE6                               ;      /* offset = 0x0470 */
    uint32_t      ICMULT_SCALE7                               ;      /* offset = 0x0474 */
    uint32_t      ICMULT_SCALE8                               ;      /* offset = 0x0478 */
    uint32_t      ICMULT_SCALE9                               ;      /* offset = 0x047C */
    uint32_t      ICMULT_SCALE10                              ;      /* offset = 0x0480 */
    uint32_t      ICMULT_SCALE11                              ;      /* offset = 0x0484 */
#endif
    uint32_t      ICMULT_SCALE[12]                            ;      /* offset = 0x0458 - 0x0484 */
#if 0
    uint32_t      QCMULT_SCALE0                               ;      /* offset = 0x0488 */
    uint32_t      QCMULT_SCALE1                               ;      /* offset = 0x048C */
    uint32_t      QCMULT_SCALE2                               ;      /* offset = 0x0490 */
    uint32_t      QCMULT_SCALE3                               ;      /* offset = 0x0494 */
    uint32_t      QCMULT_SCALE4                               ;      /* offset = 0x0498 */
    uint32_t      QCMULT_SCALE5                               ;      /* offset = 0x049C */
    uint32_t      QCMULT_SCALE6                               ;      /* offset = 0x04A0 */
    uint32_t      QCMULT_SCALE7                               ;      /* offset = 0x04A4 */
    uint32_t      QCMULT_SCALE8                               ;      /* offset = 0x04A8 */
    uint32_t      QCMULT_SCALE9                               ;      /* offset = 0x04AC */
    uint32_t      QCMULT_SCALE10                              ;      /* offset = 0x04B0 */
    uint32_t      QCMULT_SCALE11                              ;      /* offset = 0x04B4 */
#endif
    uint32_t      QCMULT_SCALE[12]                            ;      /* offset = 0x0488 - 0x04B4 */
    uint32_t      TWID_INCR_DELTA_FRAC                        ;      /* offset = 0x04B8 */
    uint32_t      RECWIN_RESET_SW                             ;      /* offset = 0x04BC */
    uint32_t      TWID_INCR_DELTA_FRAC_RESET_SW               ;      /* offset = 0x04C0 */
    uint32_t      TWID_INCR_DELTA_FRAC_CLIP_STATUS            ;      /* offset = 0x04C4 */
    uint32_t      RECWIN_INIT_KVAL                            ;      /* offset = 0x04C8 */
    uint32_t      CMULT_RESERVED_2                            ;      /* offset = 0x04CC */
    uint32_t      CHAN_COMB_SIZE                              ;      /* offset = 0x04D0 */
#if 0
    uint32_t      CHAN_COMB_VEC_0                             ;      /* offset = 0x04D4 */
    uint32_t      CHAN_COMB_VEC_1                             ;      /* offset = 0x04D8 */
    uint32_t      CHAN_COMB_VEC_2                             ;      /* offset = 0x04DC */
    uint32_t      CHAN_COMB_VEC_3                             ;      /* offset = 0x04E0 */
    uint32_t      CHAN_COMB_VEC_4                             ;      /* offset = 0x04E4 */
    uint32_t      CHAN_COMB_VEC_5                             ;      /* offset = 0x04E8 */
    uint32_t      CHAN_COMB_VEC_6                             ;      /* offset = 0x04EC */
    uint32_t      CHAN_COMB_VEC_7                             ;      /* offset = 0x04F0 */
#endif
    uint32_t      CHAN_COMB_VEC[8]                            ;      /* offset = 0x04D4 - 0x04F0 */
    uint32_t      CHANNEL_COMB_CLIP_STATUS                    ;      /* offset = 0x04F4 */
    uint32_t      ZERO_INSERT_NUM                             ;      /* offset = 0x04F8 */
#if 0
    uint32_t      ZERO_INSERT_MASK_0                          ;      /* offset = 0x04FC */
    uint32_t      ZERO_INSERT_MASK_1                          ;      /* offset = 0x0500 */
    uint32_t      ZERO_INSERT_MASK_2                          ;      /* offset = 0x0504 */
    uint32_t      ZERO_INSERT_MASK_3                          ;      /* offset = 0x0508 */
    uint32_t      ZERO_INSERT_MASK_4                          ;      /* offset = 0x050C */
    uint32_t      ZERO_INSERT_MASK_5                          ;      /* offset = 0x0510 */
    uint32_t      ZERO_INSERT_MASK_6                          ;      /* offset = 0x0514 */
    uint32_t      ZERO_INSERT_MASK_7                          ;      /* offset = 0x0518 */
#endif
    uint32_t      ZERO_INSERT_MASK[8]                         ;      /* offset = 0x04FC - 0x0518 */
    uint32_t      ZERO_INSERT_RESERVED_1                      ;      /* offset = 0x051C */
    uint32_t      ZERO_INSERT_RESERVED_2                      ;      /* offset = 0x0520 */
    uint32_t      ZERO_INSERT_RESERVED_3                      ;      /* offset = 0x0524 */
    uint32_t      ZERO_INSERT_RESERVED_4                      ;      /* offset = 0x0528 */
    uint32_t      LFSR_SEED                                   ;      /* offset = 0x052C */
    uint32_t      LFSR_LOAD                                   ;      /* offset = 0x0530 */
    uint32_t      DITHER_TWID_EN                              ;      /* offset = 0x0534 */
    uint32_t      FFT_CLIP                                    ;      /* offset = 0x0538 */
    uint32_t      CLR_FFTCLIP                                 ;      /* offset = 0x053C */
    uint32_t      CLR_CLIP_MISC                               ;      /* offset = 0x0540 */
    uint32_t      IP_OP_FORMATTER_CLIP_STATUS                 ;      /* offset = 0x0544 */
    uint32_t      FFT_RESERVED_1                              ;      /* offset = 0x0548 */
    uint32_t      FFT_RESERVED_2                              ;      /* offset = 0x054C */
    uint32_t      FFT_RESERVED_3                              ;      /* offset = 0x0550 */
#if 0
    uint32_t      MAX1_VALUE                                  ;      /* offset = 0x0554 */
    uint32_t      MAX2_VALUE                                  ;      /* offset = 0x0558 */
    uint32_t      MAX3_VALUE                                  ;      /* offset = 0x055C */
    uint32_t      MAX4_VALUE                                  ;      /* offset = 0x0560 */
#endif
    uint32_t      MAX_VALUE[4]                                ;      /* offset = 0x0554 - 0x0560*/
#if 0
    uint32_t      MAX1_INDEX                                  ;      /* offset = 0x0564 */
    uint32_t      MAX2_INDEX                                  ;      /* offset = 0x0568 */
    uint32_t      MAX3_INDEX                                  ;      /* offset = 0x056C */
    uint32_t      MAX4_INDEX                                  ;      /* offset = 0x0570 */
#endif
    uint32_t      MAX_INDEX[4]                                ;      /* offset = 0x0564 - 0x0570*/
#if 0
    uint32_t      I_SUM1_LSB                                  ;      /* offset = 0x0574 */
    uint32_t      I_SUM1_MSB                                  ;      /* offset = 0x0578 */
    uint32_t      I_SUM2_LSB                                  ;      /* offset = 0x057C */
    uint32_t      I_SUM2_MSB                                  ;      /* offset = 0x0580 */
    uint32_t      I_SUM3_LSB                                  ;      /* offset = 0x0584 */
    uint32_t      I_SUM3_MSB                                  ;      /* offset = 0x0588 */
    uint32_t      I_SUM4_LSB                                  ;      /* offset = 0x058C */
    uint32_t      I_SUM4_MSB                                  ;      /* offset = 0x0590 */

#endif
    hwaAccVal     I_SUM_VALUE[4]                              ;      /* offset = 0x0574 - 0x0590*/
#if 0
    uint32_t      Q_SUM1_LSB                                  ;      /* offset = 0x0594 */
    uint32_t      Q_SUM1_MSB                                  ;      /* offset = 0x0598 */
    uint32_t      Q_SUM2_LSB                                  ;      /* offset = 0x059C */
    uint32_t      Q_SUM2_MSB                                  ;      /* offset = 0x05A0 */
    uint32_t      Q_SUM3_LSB                                  ;      /* offset = 0x05A4 */
    uint32_t      Q_SUM3_MSB                                  ;      /* offset = 0x05A8 */
    uint32_t      Q_SUM4_LSB                                  ;      /* offset = 0x05AC */
    uint32_t      Q_SUM4_MSB                                  ;      /* offset = 0x05B0 */
#endif
    hwaAccVal     Q_SUM_VALUE[4]                              ;      /* offset = 0x0594 - 0x05B0*/
    uint32_t      FFTSUMDIV                                   ;      /* offset = 0x05B4 */
    uint32_t      MAX2D_OFFSET_DIM1                           ;      /* offset = 0x05B8 */
    uint32_t      MAX2D_OFFSET_DIM2                           ;      /* offset = 0x05BC */
    uint32_t      CDF_CNT_THRESH                              ;      /* offset = 0x05C0 */
    uint32_t      STATS_RESERVED_1                            ;      /* offset = 0x05C4 */
    uint32_t      STATS_RESERVED_2                            ;      /* offset = 0x05C8 */
    uint32_t      STATS_RESERVED_3                            ;      /* offset = 0x05CC */
    uint32_t      STATS_RESERVED_4                            ;      /* offset = 0x05D0 */
    uint32_t      STATS_RESERVED_5                            ;      /* offset = 0x05D4 */
    uint32_t      CFAR_PEAKCNT                                ;      /* offset = 0x05D8 */
    uint32_t      CFAR_DET_THR                                ;      /* offset = 0x05DC */
    uint32_t      CFAR_TEST_REG                               ;      /* offset = 0x05E0 */
    uint32_t      CFAR_THRESH                                 ;      /* offset = 0x05E4 */
    uint32_t      CFAR_RESERVED_1                             ;      /* offset = 0x05E8 */
    uint32_t      CFAR_RESERVED_2                             ;      /* offset = 0x05EC */
    uint32_t      CFAR_RESERVED_3                             ;      /* offset = 0x05F0 */
    uint32_t      CFAR_RESERVED_4                             ;      /* offset = 0x05F4 */
    uint32_t      CMP_EGE_K0123                               ;      /* offset = 0x05F8 */
    uint32_t      CMP_EGE_K4567                               ;      /* offset = 0x05FC */
    uint32_t      MEM_INIT_START                              ;      /* offset = 0x0600 */
    uint32_t      MEM_INIT_DONE                               ;      /* offset = 0x0604 */
    uint32_t      MEM_INIT_STATUS                             ;      /* offset = 0x0608 */
    uint32_t      LM_THRESH_VAL                               ;      /* offset = 0x060C */
    uint32_t      LM_2DSTATS_BASE_ADDR                        ;      /* offset = 0x0610 */
    uint32_t      HWA_SAFETY_EN                               ;      /* offset = 0x0614 */
    uint32_t      HWA_SAFETY_ERR_MASK                         ;      /* offset = 0x0618 */
    uint32_t      HWA_SAFETY_ERR_STATUS                       ;      /* offset = 0x061C */
    uint32_t      HWA_SAFETY_ERR_STATUS_RAW                   ;      /* offset = 0x0620 */
#if 0
    uint32_t      HWA_SAFETY_DMEM0_ERR_ADDR                   ;      /* offset = 0x0624 */
    uint32_t      HWA_SAFETY_DMEM1_ERR_ADDR                   ;      /* offset = 0x0628 */
    uint32_t      HWA_SAFETY_DMEM2_ERR_ADDR                   ;      /* offset = 0x062C */
    uint32_t      HWA_SAFETY_DMEM3_ERR_ADDR                   ;      /* offset = 0x0630 */
    uint32_t      HWA_SAFETY_DMEM4_ERR_ADDR                   ;      /* offset = 0x0634 */
    uint32_t      HWA_SAFETY_DMEM5_ERR_ADDR                   ;      /* offset = 0x0638 */
    uint32_t      HWA_SAFETY_DMEM6_ERR_ADDR                   ;      /* offset = 0x063C */
    uint32_t      HWA_SAFETY_DMEM7_ERR_ADDR                   ;      /* offset = 0x0640 */
#endif
    uint32_t      HWA_SAFETY_DMEM_ERR_ADDR[8]                 ;      /* offset = 0x0624 - 0x0640 */
    uint32_t      HWA_SAFETY_WINDOW_RAM_ERR_ADDR              ;      /* offset = 0x0644 */
    uint32_t      MEM_ACCESS_ERR_STATUS                       ;      /* offset = 0x0648 */
    uint32_t      LOOP_CNT                                    ;      /* offset = 0x064C */
    uint32_t      PARAMADDR                                   ;      /* offset = 0x0650 */
#if 0
    uint32_t      PARAMADDR_CPUINTR0                          ;      /* offset = 0x0654 */
    uint32_t      PARAMADDR_CPUINTR1                          ;      /* offset = 0x0658 */
#endif
    uint32_t      PARAMADDR_CPUINTR[2]                        ;      /* offset = 0x0654 - 0x0658 */
    uint32_t      FSM_STATE                                   ;      /* offset = 0x065C */
    uint32_t      SINGLE_STEP_EN                              ;      /* offset = 0x0660 */
    uint32_t      SINGLE_STEP_TRIG                            ;      /* offset = 0x0664 */
    uint32_t      HWA_DMEM_A_BUS_SAFETY_CTRL                  ;      /* offset = 0x0668 */
    uint32_t      HWA_DMEM_A_BUS_SAFETY_FI                    ;      /* offset = 0x066C */
    uint32_t      HWA_DMEM_A_BUS_SAFETY_ERR                   ;      /* offset = 0x0670 */
    uint32_t      HWA_CFG_RESERVED_WORD1                      ;      /* offset = 0x0674 */
    uint32_t      HWA_DMEM_A_BUS_SAFETY_ERR_STAT_DATA0        ;      /* offset = 0x0678 */
    uint32_t      HWA_DMEM_B_BUS_SAFETY_CTRL                  ;      /* offset = 0x067C */
    uint32_t      HWA_DMEM_B_BUS_SAFETY_FI                    ;      /* offset = 0x0680 */
    uint32_t      HWA_DMEM_B_BUS_SAFETY_ERR                   ;      /* offset = 0x0684 */
    uint32_t      HWA_CFG_RESERVED_WORD2                      ;      /* offset = 0x0688 */
    uint32_t      HWA_DMEM_B_BUS_SAFETY_ERR_STAT_DATA0        ;      /* offset = 0x068C */
    uint32_t      HWA_CFG_RESERVED[606]                       ;      /* offset = 0x0690 - 0x1004 */
    uint32_t      LOCK0_KICK0                                 ;      /* offset = 0x1008 */
    uint32_t      LOCK0_KICK1                                 ;      /* offset = 0x100C */
    uint32_t      INTR_RAW_STATUS                             ;      /* offset = 0x1010 */
    uint32_t      INTR_ENABLED_STATUS_CLEAR                   ;      /* offset = 0x1014 */
    uint32_t      INTR_ENABLE                                 ;      /* offset = 0x1018 */
    uint32_t      INTR_ENABLE_CLEAR                           ;      /* offset = 0x101C */
    uint32_t      EOI                                         ;      /* offset = 0x1020 */
    uint32_t      FAULT_ADDRESS                               ;      /* offset = 0x1024 */
    uint32_t      FAULT_TYPE_STATUS                           ;      /* offset = 0x1028 */
    uint32_t      FAULT_ATTR_STATUS                           ;      /* offset = 0x102C */
    uint32_t      FAULT_CLEAR                                 ;      /* offset = 0x1030 */
}DSSHWACCRegs;


#ifdef __cplusplus
}
#endif

#endif /* HW_HWA_COMMONREG_V1_H */
/* END OF HW_HWA_COMMONREG_V1_H */
