/*
 *  Copyright (C) 2020 Texas Instruments Incorporated.
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
 *  \ingroup CSL_IP_MODULE
 *  \defgroup CSL_VTM VTM CSL-FL
 *
 *  @{
 */
/**
 *  \file  csl_vtm.h
 *
 *  \brief
 *     Header file containing various enumerations, structure definitions and function
 *  declarations for the Voltage and Thermal Monitor (VTM) IP.
 **/

#ifndef CSL_VTM_V1_H
#define CSL_VTM_V1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <ti/csl/cslr_vtm.h>

/**
@defgroup CSL_VTM_DATASTRUCT  VTM Data Structures
@ingroup CSL_VTM_API
*/
/**
@defgroup CSL_VTM_FUNCTION  VTM Functions
@ingroup CSL_VTM_API
*/
/**
@defgroup CSL_VTM_ENUMS VTM Enum Data defines
@ingroup CSL_VTM_API
*/

/**
 *  \addtogroup CSL_VTM_ENUMS
 *  @{
 */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the possible VID Codes to set various 
 *        voltage domain supply voltages
 *
 *  \anchor CSL_vtm_vidOpp
 *  \name VTM OPP VID Cpdes
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t                           CSL_vtm_vid_opp;
    /** Maximum number of OPP VID Codes */
#define CSL_VTM_VID_OPP_MAX_NUM                   ((uint8_t) 4U)
    /** VID OPP3 Code */
#define CSL_VTM_VID_OPP_3_CODE                    ((uint8_t) 3U)
    /** VID OPP2 Code */
#define CSL_VTM_VID_OPP_2_CODE                    ((uint8_t) 2U)
    /** VID OPP1 Code */
#define CSL_VTM_VID_OPP_1_CODE                    ((uint8_t) 1U)
    /** VID OPP0 Code */
#define CSL_VTM_VID_OPP_0_CODE                    ((uint8_t) 0U)


/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the core voltage domain mapping of VTM VD
 *
 *  \anchor CSL_vtm_ts_stat_vd_map
 *  \name VTM Core Voltage domain map
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
typedef  uint8_t CSL_vtm_ts_stat_vd_map;
    /** RTC Voltage Domain map */
#define CSL_VTM_TS_STAT_VD_MAP_RTC                       ((uint32) 0U)
    /** WKUP Voltage Domain map */
#define CSL_VTM_TS_STAT_VD_MAP_WKUP                      ((uint32) 1U)
    /** MCU Voltage Domain map */
#define CSL_VTM_TS_STAT_VD_MAP_MCU                       ((uint32) 2U)
    /** Core Voltage Domain map */
#define CSL_VTM_TS_STAT_VD_MAP_CORE                      ((uint32) 3U)
    /** Voltage Domain map not implemented  */
#define CSL_VTM_TSTAT_VD_MAP_NOT_IMPLEMENTED             ((uint32) 15U)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the VTM VD domain ID
 *
 *  \anchor CSL_vtm_vd_id
 *  \name VTM Core Voltage domain id
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t CSL_vtm_vd_id;
    /** Voltage Domain id 0 */
#define CSL_VTM_VD_DOMAIN_0                     ((uint32_t) 0U)
    /** Voltage Domain id 1 */
#define CSL_VTM_VD_DOMAIN_1                     ((uint32_t) 1U)
    /** Voltage Domain id 2 */
#define CSL_VTM_VD_DOMAIN_2                     ((uint32_t) 2U)
    /** Voltage Domain id 3 */
#define CSL_VTM_VD_DOMAIN_3                     ((uint32_t) 3U)
    /** Voltage Domain id 4 */
#define CSL_VTM_VD_DOMAIN_4                     ((uint32_t) 4U)
    /** Voltage Domain id 5 */
#define CSL_VTM_VD_DOMAIN_5                     ((uint32_t) 5U)
    /** Voltage Domain id 6 */
#define CSL_VTM_VD_DOMAIN_6                     ((uint32_t) 6U)
    /** Voltage Domain id 7 */
#define CSL_VTM_VD_DOMAIN_7                     ((uint32_t) 7U)
    /** Number of Voltage Domain id */
#define CSL_VTM_VD_DOMAIN_CNT                   ((uint32_t) 8U)
/* @} */



/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Voltage domain threshold interrupt control
 *
 *  \anchor CSL_vtm_vdThr_Interrupt_ctrl
 *  \name VTM Voltage domain threshold interrupt control
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint16_t CSL_vtm_vdThr_interrupt_ctrl;

#define CSL_VTM_VD_LT_THR0_INTR_RAW_SET                   (1u)
#define CSL_VTM_VD_GT_THR1_INTR_RAW_SET                   (2u)
#define CSL_VTM_VD_GT_THR2_INTR_RAW_SET                   (4u)
#define CSL_VTM_VD_LT_THR0_INTR_RAW_CLR                   (8u)
#define CSL_VTM_VD_GT_THR1_INTR_RAW_CLR                   (16u)
#define CSL_VTM_VD_GT_THR2_INTR_RAW_CLR                   (32u)
#define CSL_VTM_VD_LT_THR0_INTR_EN_SET                    (64u)
#define CSL_VTM_VD_GT_THR1_INTR_EN_SET                   (128u)
#define CSL_VTM_VD_GT_THR2_INTR_EN_SET                   (256u)
#define CSL_VTM_VD_LT_THR0_INTR_EN_CLR                   (512u)
#define CSL_VTM_VD_GT_THR1_INTR_EN_CLR                  (1024u)
#define CSL_VTM_VD_GT_THR2_INTR_EN_CLR                  (2048u)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Voltage domain event status
 *
 *  \anchor CSL_vtm_vdEvt_status
 *  \name VTM Voltage domain event status
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
  
typedef uint8_t CSL_vtm_vdEvt_status;

#define CSL_VTM_VD_EVT_STAT_THR_ALERTS_MASK               (7u)
#define CSL_VTM_VD_EVT_STAT_LT_TH0_ALERT                  (4u)
#define CSL_VTM_VD_EVT_STAT_GT_TH1_ALERT                  (1u)
#define CSL_VTM_VD_EVT_STAT_GT_TH2_ALERT                  (2u)
/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Voltage domain Event selection set
 *
 *  \anchor CSL_vtm_vdEvt_sel_set
 *  \name VTM Voltage domain Event selection set
 *
 *  @{
 * ----------------------------------------------------------------------------
 */


typedef uint16_t CSL_vtm_vdEvt_sel_set;

#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_0            (1u)
#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_1            (2u)
#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_2            (4u)
#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_3            (8u)
#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_4            (16u)
#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_5            (32u)
#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_6            (64u)
#define CSL_VTM_VD_EVT_SELECT_TEMP_SENSOR_7            (128u)
/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Voltage domain Event selection clear
 *
 *  \anchor CSL_vtm_vdEvt_sel_clr
 *  \name VTM Voltage domain Event selection clear
 *
 *  @{
 * ----------------------------------------------------------------------------
 */


typedef uint16_t CSL_vtm_vdEvt_sel_clr;

#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_0            (1u)
#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_1            (2u)
#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_2            (4u)
#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_3            (8u)
#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_4            (16u)
#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_5            (32u)
#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_6            (64u)
#define CSL_VTM_VD_EVT_CLEAR_TEMP_SENSOR_7            (128u)
/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Temperature sensor id
 *
 *  \anchor CSL_vtm_tmpSens_id
 *  \name VTM temperature sensor id
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t CSL_vtm_tmpSens_id;

#define CSL_VTM_TS_ID_0                                      (0U)
#define CSL_VTM_TS_ID_1                                      (1U)
#define CSL_VTM_TS_ID_2                                      (2U)
#define CSL_VTM_TS_ID_3                                      (3U)
#define CSL_VTM_TS_ID_4                                      (4U)
#define CSL_VTM_TS_ID_5                                      (5U)
#define CSL_VTM_TS_ID_6                                      (6U)
#define CSL_VTM_TS_ID_7                                      (7U)
#define CSL_VTM_TS_MAX_NUM                                   (8U)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines for VTM Temperature sensor id
 *         control update valid maps. This controls the selective
 *         update of the fields in the temperature sensor control field.
 *
 *  \anchor CSL_vtm_tsGlobal_ctrl_valid_map
 *  \name VTM temperature sensor id
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint32_t CSL_vtm_tsGlobal_ctrl_valid_map;

#define CSL_VTM_TSGLOBAL_CLK_SEL_VALID                    (1u)
#define CSL_VTM_TSGLOBAL_CLK_DIV_VALID                    (2u)
#define CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_EN_VALID    (4u)
#define CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR0_VALID      (8u)
#define CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR_VALID      (16u)
#define CSL_VTM_TSGLOBAL_SAMPLES_PER_CNT_VALID           (32u)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Temperature sensor global control Clock
 *        select options
 *
 *  \anchor CSL_vtm_tmpSens_global_control_clockSelect
 *  \name VTM Temperature Sensor global control clock select
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t CSL_vtm_tsGlobal_clkSel;

#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_SEL_FIX_REF_CLK         (1u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_SEL_FIX_REF2_CLK        (2u)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Temperature sensor global control Clock
 *        divide options
 *
 *  \anchor CSL_vtm_tsGlobal_clkDiv
 *  \name VTM Temperature Sensor global control clock divide
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t CSL_vtm_tsGlobal_clkDiv;

#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_1                   (0u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_2                   (1u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_3                   (2u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_4                   (3u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_5                   (4u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_6                   (5u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_7                   (6u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_8                   (7u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_9                   (8u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_10                  (9u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_11                  (10u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_12                   (11u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_13                   (12u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_14                   (13u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_15                   (14u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_16                   (15u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_17                   (16u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_18                   (17u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_19                   (18u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_20                  (19u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_21                  (20u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_22                   (21u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_23                   (22u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_24                   (23u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_25                   (24u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_26                   (25u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_27                   (26u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_28                   (27u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_29                   (28u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_30                  (29u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_31                  (30u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_32                   (31u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_33                   (32u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_34                   (33u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_35                   (34u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_36                   (35u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_37                   (36u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_38                   (37u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_39                   (38u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_40                  (39u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_41                  (40u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_42                   (41u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_43                   (42u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_44                   (43u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_45                   (44u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_46                   (45u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_47                   (46u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_48                   (47u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_49                   (48u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_50                  (49u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_51                  (50u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_52                   (51u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_53                   (52u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_54                   (53u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_55                   (54u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_56                   (55u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_57                   (56u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_58                   (57u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_59                   (58u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_60                  (59u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_61                  (60u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_62                   (61u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_63                   (62u)
#define CSL_VTM_TSGLOBAL_CLK_CTRL_CLK_DIV_BY_64                  (63u)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Temperature sensor global control any
 *        max temperature alert enable control
 *
 *  \anchor CSL_vtm_tsGlobal_any_maxt_outrg_alert_en
 *  \name VTM Temperature Sensor global control any
 *        maximum temperature alert enable
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t  CSL_vtm_tsGlobal_any_maxt_outrg_alert_en;

#define CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_ENABLE   (1u)
#define CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_DISABLE  (0u)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Temperature sensor global control
 *        samples per count
 *
 *  \anchor CSL_vtm_tsGlobal_samples_per_count
 *  \name VTM Temperature Sensor global control samples per count
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint16_t CSL_vtm_tsGlobal_samples_per_count;

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM Temperature sensor control valid map
 *
 *  \anchor CSL_vtm_tsCtrl_valid_map
 *  \name VTM Temperature Sensor control valid map
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
typedef uint8_t CSL_vtm_tsCtrl_valid_map;
#define CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID                (1u)
#define CSL_VTM_TS_CTRL_RESET_CTRL_VALID                     (2u)
#define CSL_VTM_TS_CTRL_SOC_VALID                            (4u)
#define CSL_VTM_TS_CTRL_MODE_VALID                           (8u)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for VTM temperature sensor band gap
 *        maximum temperature out of range alert control
 *
 *  \anchor CSL_vtm_tsBandGap_max_outrg_alert
 *  \name   VTM temperature sensor band gap maximum temperature 
 *          out of range alert control
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t  CSL_vtm_tsCtrl_max_outrg_alert;
#define CSL_VTM_TS_CTRL_MAXT_OUTRG_GEN_ALERT         (1u)
#define CSL_VTM_TS_CTRL_MAXT_OUTRG_NO_ALERT          (0u)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM temperature sensor band gap reset  control bits
 *
 *  \anchor CSL_vtm_tsGlobal_resetCtrl
 *  \name   VTM temperature sensor band gap reset  control bits
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t CSL_vtm_tsCtrl_resetCtrl;
#define CSL_VTM_TS_CTRL_SENSOR_RESET                     (0u)
#define CSL_VTM_TS_CTRL_SENSOR_NORM_OP                   (1u)
/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM temperature sensor mode control bits
 *
 *  \anchor CSL_vtm_tsGlobal_mode
 *  \name   VTM temperature sensor mode control bits
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t    CSL_vtm_tsCtrl_mode;
#define CSL_VTM_TS_CTRL_SINGLESHOT_MODE                 (0u)
#define CSL_VTM_TS_CTRL_CONTINUOUS_MODE                 (1u)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM temperature sensor band gap single shot mode start of conversion trigger
 *
 *  \anchor CSL_vtm_tsCtrl_soc
 *  \name   VTM temperature sensor band gap single shot mode start of conversion trigger
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
typedef  uint8_t CSL_vtm_tsCtrl_singleshot_conv_stat;
#define CSL_VTM_TS_CTRL_SINGLESHOT_ADC_CONV_IN_PROGRESS    (1u)
#define CSL_VTM_TS_CTRL_SINGLESHOT_ADC_CONV_COMPLETE       (0u)


/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM Temperature Sensor bandgap Alert Status
 *
 *  \anchor CSL_VTM_tsStat_alert_stat
 *  \name   VTM Temperature Sensor bandgap Alert Status
 *
 *  @{
 * ----------------------------------------------------------------------------
 */


typedef  uint8_t  CSL_VTM_tsStat_alert_stat;

#define CSL_VTM_TS_MAXT_OUTRG_ALERT              (1 << 15u)
#define CSL_VTM_TS_LT_TH0_ALERT                  (1 << 14u)
#define CSL_VTM_TS_GT_TH2_ALERT                  (1 << 13u)
#define CSL_VTM_TS_GT_TH1_ALERT                  (1 << 12u)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM Temperature Sensor bandgap first time end of conversion status
 *
 *  \anchor CSL_VTM_tsBandGap_first_eoc_stat
 *  \name   VTM Temperature Sensor bandgap first time end of conversion status
 *
 *  @{
 * ----------------------------------------------------------------------------
 */


typedef  uint8_t  CSL_VTM_tsBandGap_first_eoc_stat;

#define CSL_VTM_TS_FIRST_EOC_SET                (1u)
#define CSL_VTM_TS_FIRST_EOC_CLR                (0u)


/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM Temperature Sensor thresholds valid bit map
 *
 *  \anchor CSL_vtm_thr_valid_map
 *  \name   VTM Temperature Sensor thresholds valid bit map
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
typedef uint8_t  CSL_vtm_thr_valid_map;
#define CSL_VTM_GT_TH1_VALID                (1u)
#define CSL_VTM_GT_TH2_VALID                (2u)
#define CSL_VTM_LT_TH0_VALID                (4u)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM temperature sensor STAT read valid map
 *
 *  \anchor CSL_vtm_tsStat_read_valid_map
 *  \name   VTM temperature sensor STAT read valid map
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t CSL_vtm_tsStat_read_valid_map;
#define CSL_VTM_TS_READ_VD_MAP_VAL                (1U)
#define CSL_VTM_TS_READ_ALL_THRESHOLD_ALERTS      (2U)
#define CSL_VTM_TS_READ_FIRST_TIME_EOC_BIT        (4U)
#define CSL_VTM_TS_READ_DATA_VALID_BIT            (8U)
#define CSL_VTM_TS_READ_DATA_OUT_VAL             (16U)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM temperature sensor STAT read control map
 *
 *  \anchor CSL_vtm_tsStat_read_ctrl
 *  \name   VTM temperature sensor STAT read control map
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef CSL_vtm_tsStat_read_valid_map CSL_vtm_tsStat_read_ctrl;

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM temperature in milli Degree Celcius
 *
 *  \anchor CSL_vtm_mTemp_val
 *  \name   VTM temperature in milli Degree Celcius
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef int32_t CSL_vtm_milliDegreeTemp_val;

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator define for 
 *        VTM temperature sensor ADC code
 *        This is the data_out value of the temperature sensor stat register
 *
 *  \anchor CSL_vtm_adc_code
 *  \name   VTM temperature sensor ADC code
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  int16_t CSL_vtm_adc_code;

/* @} */
    
/* @} */

/**
 *  \addtogroup CSL_VTM_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief VTM Info structure
 *
 * This structure contans the VTM information
 * ----------------------------------------------------------------------------
 */

typedef struct {
    /** peripheral id */
    uint32_t           pid;
    /** Core voltage domain (cVD) global mapping 4-bit code, 
        in the context of this SOC.
        It shows in which cVD this VTM is instantiated/placed.
        This field indicates in which core voltage domain (cVD) has been
        physically placed this VTM.
        Valid values: 0x0 to 0xE where:
        0x0 = VDD_RTC (not present is some SOCs)
        0x1 = VDD_WKUP
        0x2 = VDD_MCU
        0x3 = VDD_CORE (not present is some SOCs)
        0x4-0xE = Mapping varies between SOCs. */
    uint8_t            vtm_vd_map;
     /** 0: There is NO VDD_RTC in this SOC.
         1: There is a VDD_RTC in this SOC. */
    uint8_t            vd_rtc;
    /** Number of temperature sensors associated with this VTM */
    uint8_t            temp_sensors_cnt;
    /** Number of core voltage domains in SOC. */
    uint8_t            cvd_cnt;
} CSL_vtm_info;

/** \brief VTM Global Configuration Registers
 *
 *  This structure contains VTM Global Configuration register.
 *
 */

typedef struct {
    /** valid bit map for temperature sensor global control */
    CSL_vtm_tsGlobal_ctrl_valid_map             validMap;
    /** Temperature sensor clock source selector.
         0 = fix_ref_clk as source.
         1 = fix_ref2_clk as source */
    CSL_vtm_tsGlobal_clkSel                     clkSel;
    /** Temperature sensor clock source divider selector.
        Default set by e-fuse or tie-off.
        Divider uses select reference clock as source.
        0 = 1x divide.  1 = 2x divide.
        ... 15 = 16x divide. ... 63 = 64x divide. */
    CSL_vtm_tsGlobal_clkDiv                     clkDiv;
    /** This bit in VTM_MISC_CTRL register, when enable will cause,
        the VTM’s output therm_maxtemp_outrange_alert to be driven high,
        if any of the sources for the MAXT_OUTRG_ALERT, is set high. */
    CSL_vtm_tsGlobal_any_maxt_outrg_alert_en    any_maxt_outrg_alert_en;
    /** ADC code programmed in VTM_MISC_CTRL2 for the global max temperature 
    out of range safe sample value. If the alert is enabled globally
    for the sensor, and the sensor reads a value <= this value, then the alert
    is cleared after being triggered */
    CSL_vtm_adc_code                            maxt_outrg_alert_thr0;
    /** ADC code programmed in VTM_MISC_CTRL2 for the global max temperature
    out of range sample value. If the alert is enabled globally for the
    sensor, and the sensor reads a value >= this value, then
    the alert is triggered */
    CSL_vtm_adc_code                            maxt_outrg_alert_thr; 
    /** Temperature sensor sample period count selector,
        programmed in VTM_SAMPLE_CTRL reg */
    CSL_vtm_tsGlobal_samples_per_count          samplesPerCnt;
} CSL_vtm_tsGlobal_cfg;

/** \brief VTM temperature sensor band gap control
 *
 *  This structure contains VTM temperature sensor control.
 *
 */

typedef struct {
    /** Valid control map for temperature sensor control parameters */
    CSL_vtm_tsCtrl_valid_map         valid_map;
    /** Enable out-of-range event. This bit enables generation of the
      alert in case the given temperature sensors generates a temp code
      above a programmed max */
    CSL_vtm_tsCtrl_max_outrg_alert   maxt_outrg_alert_en;
    /** Temp-Monitor control to reset all Temp-monitor digital outputs or
        allow operation of sensor */
    CSL_vtm_tsCtrl_resetCtrl         tsReset;
    /** Temp-Monitor control: ADC Start of Conversion.
    A transition from 0 to 1 starts a new ADC conversion cycle.
    The bit will automatically clear when the conversion has completed.
    This mode is not valid when already in continuous mode. */
    CSL_vtm_tsCtrl_singleshot_conv_stat adc_stat;
    /** Temp-Monitor control: ADC Continuous mode.
    Setting this mode enables the VTM to continuously monitor 
    the sensor automatically */
    CSL_vtm_tsCtrl_mode              mode;
} CSL_vtm_ts_ctrl_cfg;


/** \brief VTM Static Registers
 *
 *  This structure contains VTM static register for a given configuration
 *  The register values are not expected to change until a new configuration
 *  is done.
 *
 */

typedef struct {
    /** Voltage domain event selection control values */
    uint32_t                    vtm_vd_evt_sel_ctrl[CSL_VTM_VD_DOMAIN_CNT];
    /** VTM OPP voltages */
    uint32_t                    vtm_vd_opp_vid[CSL_VTM_VD_DOMAIN_CNT];
    /** VTM individual sensor cfg2 control information */
    uint32_t                    vtm_ts_ctrl2[CSL_VTM_TS_MAX_NUM];
    /** VTM individual sensor cfg1 control information */
    uint32_t                    vtm_ts_ctrl[CSL_VTM_TS_MAX_NUM];
    /** VTM individual sensor threshold information */
    uint32_t                    vtm_ts_th[CSL_VTM_TS_MAX_NUM];
    /** VTM individual sensor threshold2 information */
    uint32_t                    vtm_ts_th2[CSL_VTM_TS_MAX_NUM];     
    /** VTM global configuration configuration */
    CSL_vtm_tsGlobal_cfg        vtm_global_cfg;
} CSL_vtm_staticRegs;

/** \brief VTM temperature sensor threshold values
 *
 *  This structure contains VTM temperature sensor threshold values
 *
 */

typedef struct {
    /** Valid control bit map the threshold */
    CSL_vtm_thr_valid_map    thrValidMap;
    /** Over threshold value 1 */
    CSL_vtm_adc_code         gtTh1;
    /* 0: disable Th1, otherwise: enable Th1 */
    Bool                     gtTh1En;
    /** Over threshold value 2 */
    CSL_vtm_adc_code         gtTh2;
    /* 0: disable Th2, otherwise: enable Th2 */
    Bool                     gtTh2En;
    /** Under threshld value 0 */
    CSL_vtm_adc_code         ltTh0;
    /* 0: disable Th0, otherwise: enable Th0 */
    Bool                     ltTh0En;
} CSL_vtm_tsThrVal;

/** \brief VTM temperature sensor Stat values
 *
 *  This structure contains VTM temperature sensor Stat values
 *
 */

typedef struct {
    /** Indicates the core voltage domain placement of the temperature sensor.
       Device specific field. This field indicates in which core voltage domain,
       cVD, has been physically placed the temp-monitor.
       Valid values: 0x0 to 0xE where: 0x0 = VD_RTC, not present is some SOCs,
       0x1 = VD_WKUP, 0x2 = VD_MCU, 0x3 = VD_CORE etc */
    CSL_vtm_ts_stat_vd_map     vd_map;
    /** This bit will be driven to a level 1 for a given temperature monitor
       if it has  its corresponding bit maxt_outrg_en = 1, and the temperature
       reading is reporting to be outside the max temperature supported,
       temp > programmed value */
    uint8_t                    maxt_outrg_alert;
    /** reflects the status of the lt_th0_alert comparator result. */
    uint8_t                    lt_th0_alert;
    /** reflects the status of the gt_th1_alert comparator result */
    uint8_t                    gt_th1_alert;
    /** reflects the status of the gt_th2_alert comparator result */
    uint8_t                    gt_th2_alert;
    /** ADC conversion status */
    uint8_t                    soc_fc_update;
    /** Data valid bit after the adc is done */
    uint8_t                    data_valid;
    /** Data_out signal value from sensor: Temperature data from the ADC in monitor. */
    CSL_vtm_adc_code           data_out;
} CSL_vtm_tsStat_val; 

/* @} */

/**
 *  \addtogroup CSL_VTM_FUNCTION
 *  @{
 */

/**
 *  \brief set the VID OPP Code for VID OPP register
 *
 *            Reset defaults are sourced from efuse for each OPP. 
 *            The default reset values will not be necessarily overwritten. 
 *            The write capability in the MMR is for having the option to
 *            debug and have software driven adjustments if necessary
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param voltage_domain    [IN]    voltage domain id
 *  \param vid_opp           [IN]    which VID OPP to be updated
 *  \param vid_opp_val       [IN]    VID OPP Code value
 *  \param read_verify_flag  [IN]    value written would be verified after written
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_vtmVdSetOppVid (const CSL_vtm_cfg1Regs  *p_cfg,  
                            CSL_vtm_vd_id           voltage_domain,
                            CSL_vtm_vid_opp         vid_opp, 
                            uint8_t                 vid_opp_val,
                            Bool                    read_verify_flag);

/**
 *  \brief get VTM VID OPP Code from VID OPP register
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param voltage_domain    [IN]    voltage domain id
 *  \param vid_opp           [IN]    which VID OPP to be updated
 *  \param p_vid_opp_val     [OUT]   Pointer to VID OPP Code value
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_vtmVdGetOppVid (const CSL_vtm_cfg1Regs    *p_cfg,  
                            CSL_vtm_vd_id             voltage_domain,
                            CSL_vtm_vid_opp           vid_opp, 
                            uint8_t*                  p_vid_opp_val);

/**
 *  \brief set Voltage domain a event select and control set register.
 *         In this API, select which of the event contributions of the
 *         temp-monitors controlled by this VTM will contribute to generate the 
 *         merged event/alerts of this VD. Any combination of them could be selected
 *
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param voltage_domain    [IN]    voltage domain id
 *  \param vd_temp_evts      [IN]    Temperature events to be selected for VD
 *  \param read_verify_flag  [IN]    value written would be verified after written
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmVdEvtSelSet (const CSL_vtm_cfg1Regs  *p_cfg,
                            CSL_vtm_vd_id           voltage_domain,
                            CSL_vtm_vdEvt_sel_set   vd_temp_evts,
                            Bool                    read_verify_flag);



/**
 *  \brief clear Voltage domain a event select and control clear register.
 *         In this API, clear which of the event contributions of the
 *         temp-monitors controlled by this VTM will contribute to generate the 
 *         merged event/alerts of this VD. Any combination of them could be selected
 *
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param voltage_domain    [IN]    voltage domain id
 *  \param vd_temp_evts      [IN]    Temperature events to be cleared for VD
 *  \param read_verify_flag  [IN]    value written would be verified after written
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmVdEvtSelClr (const CSL_vtm_cfg1Regs    *p_cfg,
                            CSL_vtm_vd_id             voltage_domain,
                            CSL_vtm_vdEvt_sel_clr     vd_temp_evts,
                            Bool                      read_verify_flag);


/**
 *  \brief Read Voltage domain event Status register.
 *         Get the event threshold alert status for a given voltage domain
 *
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param voltage_domain    [IN]    voltage domain id
 *  \param p_evt_stat        [OUT]   Pointer to event stat
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmVdGetEvtStat (const CSL_vtm_cfg1Regs         *p_cfg,
                             CSL_vtm_vd_id                  voltage_domain,
                             CSL_vtm_vdEvt_status           *p_evt_stat);

/**
 *  \brief set Voltage domain threshold interrup control
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param voltage_domain    [IN]    voltage domain id
 *  \param ctrl              [IN]    Temperature threshold interrupt control (enable/disable controls).
 *  \param read_verify_flag  [IN]    value written would be verified after written
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmVdThrIntrCtrl (const CSL_vtm_cfg1Regs        *p_cfg,
                              CSL_vtm_vd_id                  voltage_domain,
                              CSL_vtm_vdThr_interrupt_ctrl   ctrl,
                              Bool                           read_verify_flag);

/**
 *  \brief VTM Temperature sensor set, clear threshold values and
 *         enable, disable threshold events
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param temp_sensor_id    [IN]    temperature sensor id
 *  \param p_thr_val         [IN]    Pointer to temperature threshold values
 *  \param read_verify_flag  [IN]    value written would be verified after written
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsSetThresholds (const CSL_vtm_cfg1Regs      *p_cfg,
                                CSL_vtm_tmpSens_id          temp_sensor_id,
                                const CSL_vtm_tsThrVal      *p_thr_val,
                                Bool                        read_verify_flag);
/**
 *  \brief VTM Temperature Sensor get threshold values and threshold enable/disable status
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param temp_sensor_id    [IN]    temperature sensor id
 *  \param p_thr_val         [OUT]   Pointer to temperature threshold values
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsGetThresholds (const CSL_vtm_cfg1Regs    *p_cfg,
                                CSL_vtm_tmpSens_id         temp_sensor_id,
                                CSL_vtm_tsThrVal          *p_thr_val);


/**
 *  \brief VTM Temperature Sensor Set Global configuration values
 *
 *  \param p_cfg2             [IN]    Pointer to the VTM configuration2 structure
 *  \param p_tsGlobal_cfg    [IN]    Pointer to temperature global configuration
 *  \param read_verify_flag  [IN]    Set this to enable register read verify after write

 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsSetGlobalCfg (const CSL_vtm_cfg2Regs       *p_cfg2,
                               const CSL_vtm_tsGlobal_cfg         *p_tsGlobal_cfg,
                               Bool                          read_verify_flag);


/**
 *  \brief VTM Temperature Sensor Control
 *
 *  \param p_cfg2            [IN]    Pointer to the VTM configuration2 structure
 *  \param temp_sensor_id    [IN]    Temperature sensor ID
 *  \param p_tsCtrl_cfg      [IN]    Pointer to temperature sensor control configuration
 *  \param read_verify_flag  [IN]    Set this to enable register read verify after write

 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsSetCtrl (const CSL_vtm_cfg2Regs        *p_cfg2,
                          CSL_vtm_tmpSens_id            temp_sensor_id, 
                          const CSL_vtm_ts_ctrl_cfg     *p_tsCtrl_cfg,
                          Bool                          read_verify_flag);


/**
 *  \brief Read VTM Temperature Sensor Control
 *
 *  \param p_cfg2            [IN]    Pointer to the VTM configuration2 structure
 *  \param temp_sensor_id    [IN]    Temperature sensor ID
 *  \param p_tsCtrl_cfg      [IN]    Pointer to temperature sensor control configuration

 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsGetCtrl (const CSL_vtm_cfg2Regs        *p_cfg2,
                          CSL_vtm_tmpSens_id             temp_sensor_id,
                          CSL_vtm_ts_ctrl_cfg           *p_tsCtrl_cfg);

/**
 *  \brief VTM Temperature Sensor Get Global configuration values
 *
 *  \param p_cfg2            [IN]    Pointer to the VTM configuration2 structure
 *  \param p_tsGlobal_cfg    [OUT]   Pointer to temperature global configuration
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsGetGlobalCfg (const CSL_vtm_cfg2Regs       *p_cfg2,
                               CSL_vtm_tsGlobal_cfg         *p_tsGlobal_cfg);

/**
 *  \brief VTM Temperature Sensor Get Global configuration values
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param p_vtm_info        [OUT]   Pointer to VTM information structure
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmGetVTMInfo (const CSL_vtm_cfg1Regs     *p_cfg,
                           CSL_vtm_info               *p_vtm_info);

/**
 *  \brief VTM Temperature ADC code to Temperature conversion
 *
 *  \param adc_code                 [IN]   10 Bit ADC code
 *  \param temp_sensor_id           [IN]   sensor id
 *  \param p_milli_degree_temp_val  [OUT]  Pointer to Temperature in milli degree celcius
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsConvADCToTemp (CSL_vtm_adc_code        adc_code,
                                CSL_vtm_tmpSens_id      temp_sensor_id,  
                                int32_t                 *p_milli_degree_temp_val);

/**
 *  \brief VTM Temperature to ADC conversion
 *
 *  \param milli_degree_temp_val        [IN] Temperature in milli degree celcius
 *  \param temp_sensor_id               [IN] sensor id
 *  \param p_adc_code                   [OUT] pointer to 10 Bit ADC code
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_vtmTsConvTempToAdc (int32_t                 milli_degree_temp_val,
                                CSL_vtm_tmpSens_id      temp_sensor_id,
                                CSL_vtm_adc_code*       p_adc_code);


/**
 *  \brief VTM Get Temperature Sensor stat
 *
 *  \param p_cfg             [IN]    Pointer to the VTM configuration1 structure
 *  \param p_ctrl            [IN]    Pointer to temperature Sensor Read control
 *  \param temp_sensor_id    [IN]    sensor id
 *  \param p_ts_stat_val     [OUT]   Pointer to temperature sensor stat values
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_vtmTsGetSensorStat (const CSL_vtm_cfg1Regs          *p_cfg,
                                const CSL_vtm_tsStat_read_ctrl  *p_ctrl,
                                CSL_vtm_tmpSens_id              temp_sensor_id,
                                CSL_vtm_tsStat_val              *p_ts_stat_val );


/**
 *  \brief VTM Temperature Sensor Maximum Temperature Out of Range Alert threshold
 *
 *  This function sets the "high temperature threshold" and "low temperature threshold"
 *  alert thresholds for the VTM hardware to use in determining when to apply the device
 *  reset (and when to release it). When the temperatures are above the high threshold,
 *  a SoC reset would be done and gets released after the temperature falls below
 *  the low temperature threshold. 
 *  There should not be any ISR (Interrupt Service Routine) need to program for
 *  maximum temperature out of range programming.
 *  The caller should have actively taken necessary cooling actions, prior to
 *  temperature reaching to this maximum value, with the help of GT_THR1 and/or
 *  GT_THR2 alert ISRs.
 *
 *  \param p_cfg2            [IN]    Pointer to the VTM configuration2 structure
 *  \param temp_sensor_id    [IN]    sensor id
 *  \param high_temp_in_milli_degree_celcius     [IN]   high temperature in milli degree celcius
 *  \param low_temp_in_milli_degree_celcius      [IN]   low temperature in milli degree celcius
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_vtmTsSetMaxTOutRgAlertThr(const CSL_vtm_cfg2Regs    *p_cfg2,
                                      CSL_vtm_tmpSens_id    temp_sensor_id, 
                                      int32_t               high_temp_in_milli_degree_celcius,
                                      int32_t               low_temp_in_milli_degree_celcius);

/**
 *  \brief VTM Temperature Sensor Static Registers Readback
 *
 *  This function reads back the static registers from VTM.
 *  This is helpful to readback the static registers after a new VTM configuration
 *  and the application can compare the registers peridically and compare
 *  with previous values read to make sure nothing changed between two static
 *  register reads.
 *
 *  \param p_cfg1            [IN]    Pointer to the VTM configuration1 structure
 *  \param p_cfg2            [IN]    Pointer to the VTM configuration2 structure
 *  \param p_static_regs     [OUT]   Pointer to static registers
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_vtmReadBackStaticRegisters( const CSL_vtm_cfg1Regs      *p_cfg1,
                                        const CSL_vtm_cfg2Regs      *p_cfg2,
                                        CSL_vtm_staticRegs          *p_static_regs);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif  /* end of CSL_VTM_V1_H definition */

/* @} */
