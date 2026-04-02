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
 *  \defgroup CSL_POK POWER OK CSL-FL
 *
 *  @{
 */
/**
 *  \file  csl_pok.h
 *
 *  \brief
 *     Header file containing various enumerations, structure definitions and function
 *  declarations for the Power System modules such as POK/POR IP
 **/

#ifndef CSL_POK_V0_H
#define CSL_POK_V0_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <ti/csl/soc.h>

/**
@defgroup CSL_POK_DATASTRUCT  POK Data Structures
@ingroup CSL_POK_API
*/
/**
@defgroup CSL_POK_FUNCTION  POK Functions
@ingroup CSL_POK_API
*/
/**
@defgroup CSL_POK_ENUMS POK Enum Data defines
@ingroup CSL_POK_API
*/

/**
 *  \addtogroup CSL_POK_ENUMS
 *  @{
 */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the possible POK module types
 *
 *  \anchor CSL_pok_types
 *  \name POK types
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t                           CSL_pok_type;

    /** POK type Power System Module  */
#define CSL_TYPE_POK                          ((CSL_pok_type) 1U)
    /** POK_SA type Power System Module  */
#define CSL_TYPE_POK_SA                       ((CSL_pok_type) 2U)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the possible hysteresis control values for the
 *        Power Subsystem modules
 *
 *  \anchor CSL_PWRSS_hysteris_types
 *  \name POK/POR hysteresIs types
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t                           CSL_pwrss_hysteresis;

        /** Disable hysteresis for the module  */
#define CSL_PWRSS_SET_HYSTERESIS_DISABLE            ((CSL_pwrss_hysteresis) 0U)
    /** Enable hysteresis for the module  */
#define CSL_PWRSS_SET_HYSTERESIS_ENABLE             ((CSL_pwrss_hysteresis) 1U)
        /** Get hysteresis value for the module  */
#define CSL_PWRSS_GET_HYSTERESIS_VALUE              ((CSL_pwrss_hysteresis) 2U)
    /** No update on hysteresis for the module  */
#define CSL_PWRSS_HYSTERESIS_NO_ACTION              ((CSL_pwrss_hysteresis) 3U)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the possible trim value for
 *        POK/POR modules
 *        Any value between 0 through 127 is valid TRIM value
 *
 *  \verbatim
 *
 *     POK        | Under Voltage   | Over Voltage     |                 
 *                | Detection       | Detection        | Step Resolution 
 *    ----------- | --------------- | ---------------- | ----------------
 *     CORE_POK   | 475mV - 1.35V   |  725mV - 1.65V   | 0.0125V         
 *     POK1.8     | 1.432V - 2.168V |  1.432V - 2.168V | 0.02V           
 *     POK3.3     | 2.625V - 3.975V |  2.625V - 3.975V | 0.0375V         
 *
 *  \endverbatim
 *
 *  \anchor CSL_PWRSS_trim_value
 *  \name POK/POR TRIM values
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
typedef  uint8_t                          CSL_pwrss_trim;
    /** TRIM is 7 bit value, when the trim value is <= to the
      * MAX value, that value would be written to trim register
      * Any other values, would be treated as the command as described
      * below
      */
    /** TRIM is 7 bit value, and hence the maximum value is 127  */
#define CSL_PWRSS_MAX_TRIM_VALUE               ((CSL_pwrss_trim) 127U)

    /** No update on trim value read/write for the module  */
#define CSL_PWRSS_TRIM_NO_ACTION               ((CSL_pwrss_trim) 128U)

        /** Command to read the TRIM value  */
#define CSL_PWRSS_GET_TRIM_VALUE               ((CSL_pwrss_trim) 129U)

    /** Invalid TRIM value */
#define CSL_PWRSS_INVALID_TRIM_VALUE           ((CSL_pwrss_trim) 255U)


/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the possible values of Voltage Detection modes
 *
 *  \anchor CSL_PWRSS_vd_modes
 *  \name POK/POR Voltage detection modes
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef  uint8_t                           CSL_pwrss_vd_mode;

        /** Disable under voltage detection for the module */
#define CSL_PWRSS_SET_UNDER_VOLTAGE_DET_ENABLE           ((CSL_pwrss_vd_mode) 0U)
    /** Enable over voltage detection for the module  */
#define CSL_PWRSS_SET_OVER_VOLTAGE_DET_ENABLE            ((CSL_pwrss_vd_mode) 1U)
        /** Get voltage detection for the module */
#define CSL_PWRSS_GET_VOLTAGE_DET_MODE                   ((CSL_pwrss_vd_mode) 2U)
    /** No update on voltage detection mode update for the module  */
#define CSL_PWRSS_VOLTAGE_DET_NO_ACTION                  ((CSL_pwrss_vd_mode) 3U)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the POK Detection status values 
 *
 *  \anchor CSL_POK_detection_status_values
 *  \name POK detection status
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t                             CSL_POK_detection_status;
/** POK Detection disabled */
#define CSL_POK_DETECTION_DISABLED               ((CSL_POK_detection_status) 0U)
/** POK Detection Enabled */
#define CSL_POK_DETECTION_ENABLED                ((CSL_POK_detection_status) 1U)

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the POK Detection values 
 *
 *  \anchor CSL_POK_detection_values
 *  \name POK detection values
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t                             CSL_POK_detection;
/** POK Detection disabled */
#define CSL_POK_DETECTION_DISABLE               ((CSL_POK_detection) 0U)
/** POK Detection Enabled */
#define CSL_POK_DETECTION_ENABLE                ((CSL_POK_detection) 1U)
/** POK Detection No action */
#define CSL_POK_DETECTION_NO_ACTION             ((CSL_POK_detection) 2U)
/** POK Detection get value */
#define CSL_POK_GET_DETECTION_VALUE             ((CSL_POK_detection) 3U)


/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the POK Enable selection source
 *
 *  \anchor CSL_POK_Enable Selection source values
 *  \name POK Enable Selection source Values
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t                             CSL_POK_enSelSrc;
/** POK enables come from hardware tie offs */
#define CSL_POK_ENSEL_HWTIEOFFS                  ((CSL_POK_enSelSrc) 0U)
/** POK enables come from CTRLMMR_WKUP_PRG0_CTRL register*/
#define CSL_POK_ENSEL_PRG_CTRL                   ((CSL_POK_enSelSrc) 1U)
/** POK enable selection no action */
#define CSL_POK_ENSEL_NO_ACTION                  ((CSL_POK_enSelSrc) 2U)
/** POK enable selection Get value */
#define CSL_POK_GET_ENSEL_VALUE                  ((CSL_POK_enSelSrc) 3U)


/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the Voltage threshold status
 *
 *  \anchor CSL_POK_Voltage_Thr
 *  \name POK voltage threshold status
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t                             CSL_voltage_thr_status;
/* POK Volage above threshold detected */
#define CSL_VOLTAGE_ABOVE_THRESHOLD              ((CSL_voltage_thr_status) 0U)
/* POK Voltage normal/good */
#define CSL_VOLTAGE_GOOD                         ((CSL_voltage_thr_status) 1U)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the trim selection values
 *
 *  \anchor CSL_POK_trim_selection
 *  \name POK trim selection values from HHV default or CTRL registers
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef uint8_t         CSL_por_trim_sel;

/** Trim selections for Bandgap and PORs come from HHV defaults */
#define  CSL_POR_TRIM_SELECTION_FROM_HHV_DEFAULT      ((uint8_t) 0U)

/** Trim selections for Bandgap and POKs come from CTRLMMR_WKUP_POR_BANDGAP_CTRL and
    POR_POKxxx_CTRL registers */
#define  CSL_POR_TRIM_SELECTION_FROM_CTRL_REGS        ((uint8_t) 1U)

/** Trim selections for Bandgap and POKs No Change */
#define  CSL_POR_TRIM_SELECTION_NO_CHANGE             ((uint8_t) 2U)

/** Trim Read selections Bandgap and POKs  */
#define  CSL_POR_TRIM_SELECTION_GET_VALUE             ((uint8_t) 3U)


/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the override enable/set values
 *
 *  \anchor CSL_POR_override_enable/set
 *  \name POR POR Override enable and set
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef   uint8_t               CSL_por_override;

/**OVERRIDE IS NOT SET and DISABLED */
#define  CSL_POR_OVERRIDE_NOT_SET_DISABLE                   ((CSL_por_override) 0U)

/**OVERRIDE IS NOT SET and ENABLED */
#define  CSL_POR_OVERRIDE_NOT_SET_ENABLE                    ((CSL_por_override) 1U)

/**OVERRIDE IS SET and DISABLED */
#define  CSL_POR_OVERRIDE_SET_DISABLE                       ((CSL_por_override) 2U)

/**OVERRIDE IS SET and ENABLED */
#define  CSL_POR_OVERRIDE_SET_ENABLE                        ((CSL_por_override) 3U)

/** PORHV GET OVERRIDE VALUE */
#define   CSL_POR_GET_OVERRIDE_VALUE                        ((CSL_por_override) 4U)

/** PORHV SET OVERRIDE VALUE NO CHANGE */
#define   CSL_POR_SET_OVERRIDE_NO_CHANGE                    ((CSL_por_override) 5U)

/** OVRD and SET Values unknown */
#define   CSL_OVERRIDE_SET_UNKNOWN                          ((CSL_por_override) 0xFFU)


/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the override index values
 *
 *  \anchor CSL_POR_override_index
 *  \name POR POR Override index
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

/** PORHV Override index */
#define  CSL_PORHV_OVERRIDE_INDEX                           (0U)

/** Bandgap Override index */
#define  CSL_BGAP_OVERRIDE_INDEX                            (1U)

/** POKHV Override index */
#define  CSL_POKHV_OVERRIDE_INDEX                           (2U)

/** POKLVA Override index */
#define  CSL_POKLVA_OVERRIDE_INDEX                          (3U)

/** POKLVB Override index */
#define  CSL_POKLVB_OVERRIDE_INDEX                          (4U)

/** total Number of Override indeces */
#define  CSL_MAX_OVERRIDE_INDEX                             (5U)

/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the POR Module State
 *
 *  \anchor CSL_POR_Module_state
 *  \name POR Module state values
 *
 *  @{
 * ----------------------------------------------------------------------------
 */

typedef   uint8_t               CSL_por_module_status;

/** POR in functional mode */
#define   CSL_POR_MODULE_STATUS_FUNCTIONAL_MODE             ((CSL_por_module_status) 0U)
/** POR in Reset mode */
#define   CSL_POR_MODULE_STATUS_RESET_MODE                  ((CSL_por_module_status) 1U)
/* @} */


/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the Wake up control MMR register
 *
 *  \anchor CSL_POK_
 *  \name POK POK ID values
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
typedef CSL_wkup_ctrl_mmr_cfg0Regs CSL_wkupCtrlRegsBase_t;

/* @} */

/** ---------------------------------------------------------------------------
 * @brief This enumerator defines the possible POK module ID values on J721E
 *
 *  \anchor CSL_pok_id
 *  \name POK POK ID values
 *
 *  @{
 * ----------------------------------------------------------------------------
 */
typedef int8_t  CSL_pok_id;

/** Invalid POK/POR ID */
#define CSL_FIRST_POK_ID                              (0)
/* PMIC POK ID */
#define CSL_POK_VDDA_PMIC_IN_ID                       (CSL_FIRST_POK_ID)
/* CORE Under Voltage POK ID */
#define CSL_POK_VDD_CORE_UV_ID                        (1)
/* Wakeup General POK Under Voltage ID */
#define CSL_POK_VDDSHV_WKUP_GEN_UV_ID                 (2)
/* CPU under voltage POK ID */
#define CSL_POK_VDD_CPU_UV_ID                         (3)
/* MCU under voltage VDD POK ID */
#define CSL_POK_VDDR_MCU_UV_ID                        (4)
/* VMON under voltage POK ID */
#define CSL_POK_VMON_EXT_UV_ID                        (5)
/* MCU overvoltage POK ID */
#define CSL_POK_VDD_MCU_OV_ID                         (6)
/* VDD CORE POK ID */
#define CSL_POK_VDDR_CORE_UV_ID                       (7)
/* Wakeup General POK Over voltage ID */
#define CSL_POK_VDDSHV_WKUP_GEN_OV_ID                 (8)
/* CORE VDD Over Voltage POK ID */
#define CSL_POK_VDD_CORE_OV_ID                        (9)
/* MCU  over Voltage POK ID */
#define CSL_POK_VDDR_MCU_OV_ID                        (10)
/* CPU  over Voltage POK ID */
#define CSL_POK_VDD_CPU_OV_ID                         (11)
/* CORE VDDR over Voltage POK ID */
#define CSL_POK_VDDR_CORE_OV_ID                       (12)
/* VMON POK Over Voltage ID */
#define CSL_POK_VMON_EXT_OV_ID                        (13)
/* POKHV Under Voltage POK ID */
#define CSL_POR_POKHV_UV_ID                           (14)
/* POKLV Under Voltage POK ID */
#define CSL_POR_POKLV_UV_ID                           (15)
/* POKHV Over Voltage POK ID */
#define CSL_POR_POKHV_OV_ID                           (16)
/* LAST POK ID */
#define CSL_LAST_POK_ID                               (CSL_POR_POKHV_OV_ID)


/* @} */
    
/* @} */

/**
 *  \addtogroup CSL_POK_DATASTRUCT
 *  @{
 */

/** ---------------------------------------------------------------------------
 * \brief POK Configuration structure
 *
 * This structure contans the POK Control register Configuration 
 * ----------------------------------------------------------------------------
 */

typedef struct CSL_pokCfg
{
    /** hysteresis control */
    CSL_pwrss_hysteresis            hystCtrl;
    /** Voltage Detection Mode control */
    CSL_pwrss_vd_mode               voltDetMode;
    /** POK Trim bits 7 bits wide */
    CSL_pwrss_trim                  trim;
    /** POK Detection Enable */
    CSL_POK_detection               detectionCtrl;
    /** POK Enable Source control */
    CSL_POK_enSelSrc                pokEnSelSrcCtrl;
} CSL_pokCfg_t;

/** ---------------------------------------------------------------------------
 * \brief POK Configuration structure read value
 *
 * This structure contans the POK Control register Configuration 
 * ----------------------------------------------------------------------------
 */

typedef struct CSL_pokVal
{
    /** hysteresis control */
    CSL_pwrss_hysteresis            hystCtrl;
    /** Voltage Detection Mode control */
    CSL_pwrss_vd_mode               voltDetMode;
    /** POK Trim bits 7 bits wide */
    CSL_pwrss_trim                  trim;
    /** POK detection Status  */
    CSL_POK_detection_status        detectionStatus;
    /** POK Enable Source control */
    CSL_POK_enSelSrc                pokEnSelSrcCtrl;
    /** Voltage threshold status */
    CSL_voltage_thr_status          voltageThrStatus;
} CSL_pokVal_t;


/** ---------------------------------------------------------------------------
 * \brief POK functionality of POR Configuration structure
 *
 * This structure contans the configuration for POK functionality of 
 * POR Control register
 * ----------------------------------------------------------------------------
 */

typedef struct CSL_pokPorCfg
{
    /** override control */
    CSL_por_override              override[CSL_MAX_OVERRIDE_INDEX];
    /** Mask HHV/SOC_PORz outputs when applying new trim values */
    Bool                          maskHHVOutputEnable;
    /** POK Trim selection */
    CSL_por_trim_sel              trim_select;
} CSL_pokPorCfg_t;

/** ---------------------------------------------------------------------------
 * \brief POK functionality of POR Value structure
 *
 * This structure contans the value for POK functionality of 
 * POR Control register
 * ----------------------------------------------------------------------------
 */

typedef struct CSL_pokPorVal
{
    /** override control */
    CSL_por_override              override[CSL_MAX_OVERRIDE_INDEX];
    /** Mask HHV/SOC_PORz outputs when applying new trim values */
    Bool                          maskHHVOutputEnable;
    /** POK Trim selection */
    CSL_por_trim_sel              trim_select;
} CSL_pokPorVal_t;


/** ---------------------------------------------------------------------------
 * \brief POK functionality of POR Configuration structure
 *
 * This structure contans the configuration for POK functionality of 
 * POR Control register
 * ----------------------------------------------------------------------------
 */

typedef struct CSL_pokPorStat
{
    /** POR module status control */
    CSL_por_module_status           porModuleStatus;
    /** Band Gap OK Status  */
    Bool                            porBGapOK;
} CSL_pokPorStat_t;

/* @} */

/**
 *  \addtogroup CSL_POK_FUNCTION
 *  @{
 */

/**
 *  \brief write the POK configuration for the specified POK control register
 *
 *         This API supports the enable/disable of the POK hysterisis and
 *         Voltage Detection for a given POK control register
 *
 *  \param pBaseAddress      [IN]     Pointer to the Wakeup Control Register
 *  \param pPokCfg           [IN]     Pointer to the POK control register to be written
 *  \param pokId             [IN]     POK ID of which POK to be updated
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_pokSetControl   (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                             const CSL_pokCfg_t               *pPokCfg,
                             CSL_pok_id                        pokId);

/**
 *  \brief Read the POK configuration for the specified POK control register
 *
 *         This API supports the enable/disable of the POK hysterisis and
 *         Voltage Detection for a given POK control register
 *
 *  \param pBaseAddress      [IN]     Pointer to the Wakeup Control Register
 *  \param pPokCfg           [IN]     Pointer to the POK control register values to be read
 *  \param pPokVal           [OUT]    Pointer to the POK control register to be read
 *  \param pokId             [IN]     POK ID of which POK to be updated
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_pokGetControl   (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                             const CSL_pokCfg_t               *pPokCfg,
                             CSL_pokVal_t                     *pPokVal,
                             CSL_pok_id                        pokId);

/**
 *  \brief Read the POR STAT register
 *
 *         This API supports the read of the POR STAT register
 *
 *  \param baseAddress       [IN]     Pointer to the Wakeup Control Register
 *  \param pPorStat          [OUT]    Pointer to the POR STAT register
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */

int32_t CSL_porGetStat   (const CSL_wkupCtrlRegsBase_t     *baseAddress,
                          CSL_pokPorStat_t                 *pPorStat);



/**
 *  \brief write the POR configuration for the specified POK control register
 *
 *         This API supports the enable/disable of the POK hysterisis and
 *         Voltage Detection for a given POK control register
 *
 *  \param pBaseAddress      [IN]    Pointer to the Wakeup Control Register
 *  \param pPorCfg           [IN]    Pointer to the POR configuration
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_porSetControl (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                           const CSL_pokPorCfg_t            *pPorCfg);

/**
 *  \brief Read the POR configuration for the specified POR control register
 *
 *         This API supports the enable/disable of the POK hysterisis and
 *         Voltage Detection for a given POK control register
 *
 *  \param pBaseAddress      [IN]    Pointer to the Wakeup Control Register
 *  \param pPorCfg           [IN]    Pointer to the POR configuration
 *  \param pVal              [IN]    Pointer to POR control values
 *
 *  \return The CSL error code for the API.
 *                                 Success      : CSL_PASS
 *                                 Fail         : CSL_EFAIL
 *                                 Invalid Args : CSL_EBADARGS
 */
int32_t CSL_porGetControl (CSL_wkupCtrlRegsBase_t           *pBaseAddress,
                           const CSL_pokPorCfg_t            *pPorCfg,
                           CSL_pokPorVal_t                  *pVal);


/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif  /* end of CSL_POK_V0_H definition */
