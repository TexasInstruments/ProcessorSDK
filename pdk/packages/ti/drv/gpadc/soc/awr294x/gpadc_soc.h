/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *   \file  drv/gpadc/soc/awr294x/gpadc_soc.h
 *
 *   \brief
 *      This is the platform specific header file for the GPADC driver
 *      which specifies the GPADC register addresses and structures/ unions
 *      used in the GPADC driver
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
 #ifndef DRV_GPADC_SOC_H
 #define DRV_GPADC_SOC_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

 #include <ti/drv/gpadc/soc/awr294x/reg_gpadc.h>
 #include <ti/drv/gpadc/soc/awr294x/reg_gpadcpktram.h>
 #include <ti/csl/soc/awr294x/src/cslr_soc_baseaddress.h>
 #include <ti/csl/soc/awr294x/src/cslr_mss_rcm.h>
 #include <ti/csl/soc/awr294x/src/cslr_mss_toprcm.h>
 #include <ti/csl/soc/awr294x/src/cslr_mss_ctrl.h>
 #include <ti/drv/gpadc/gpadc_cfg.h>
/****************************************************************************************
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define M_GPADC_REGS_PTR            ((T_GPADC_REGS*)CSL_MSS_GPADC_REG_U_BASE)
#define M_GPADCPKTRAM_REGS_PTR      ((T_GPADCPKTRAM_REGS*)CSL_MSS_GPADC_PKT_RAM_U_BASE)
#define T_GPADCOUT_RAM_PTR          ((T_GPADCOUT_RAM*)CSL_MSS_GPADC_DATA_RAM_U_BASE)

#define MSS_GPADC_RST_CTRL_ADDR     (CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_GPADC_RST_CTRL)
#define MSS_GPADC_RST_CTRL_PTR      ((U_MSS_GPADC_RST_CTRL_REG*)MSS_GPADC_RST_CTRL_ADDR)

#define MSS_GPADC_CLK_DIV_VAL_ADDR  (CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_GPADC_CLK_DIV_VAL)
#define MSS_GPADC_CLK_DIV_VAL_PTR   ((U_MSS_GPADC_CLK_DIV_VAL_REG*)MSS_GPADC_CLK_DIV_VAL_ADDR)

#define MSS_GPADC_CLK_GATE_ADDR     (CSL_MSS_RCM_U_BASE + CSL_MSS_RCM_MSS_GPADC_CLK_GATE)
#define MSS_GPADC_CLK_GATE_PTR      ((U_MSS_GPADC_CLK_GATE_REG*)MSS_GPADC_CLK_GATE_ADDR)

#define MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_ADDR (CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV)
#define MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_PTR  ((U_MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_REG*)MSS_TOPRCM_ANA_REG_TW_CTRL_REG_LOWV_ADDR)

#define MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_ADDR (CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV)
#define MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_PTR  ((U_MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_REG*)MSS_TOPRCM_ANA_REG_TW_ANA_TMUX_CTRL_LOWV_ADDR)

#define MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_ADDR (CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV)
#define MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_PTR  ((U_MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_REG*)MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_ADDR)

#define MSS_CTRL_GPADC_CTRL_ADDR    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_GPADC_CTRL)
#define MSS_CTRL_GPADC_CTRL_PTR     ((U_MSS_CTRL_GPADC_CTRL_REG*)MSS_CTRL_GPADC_CTRL_ADDR)

#define MSS_CTRL_MSS_GPADC_MEM_INIT_ADDR    (CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_GPADC_MEM_INIT)
#define MSS_CTRL_MSS_GPADC_MEM_INIT_PTR     ((U_MSS_CTRL_MSS_GPADC_MEM_INIT_REG*)MSS_CTRL_MSS_GPADC_MEM_INIT_ADDR)

/*! \brief
 *  GPADC MODES
 */
#define M_GPADC_MODE_DISABLE        (0U)
#define M_GPADC_MODE_IFM            (1U)
#define M_GPADC_MODE_CTM            (2U)

/*! \brief
 *  GPADC MON CTM INSTRUCTION RAM Index
 */
#define M_GPADC_MON_INSTR_RAM_ST_IND    (224U)
#define M_GPADC_MAX_MON_INSTR_RAM       (32U)

/*! \brief
 *  GPADC macros
 */

#define M_GPADC_DISABLE               (0U)
#define M_GPADC_ENABLE                (1U)
#define M_GPADC_ASSERT_RESET          (1U)
#define M_GPADC_DEASSERT_RESET        (0U)
#define M_GPADC_FSM_ASSERT_RESET      (0x7U)
#define M_GPADC_FSM_DEASSERT_RESET    (0U)

/*! \brief
 *  Number of measurement parameters for GPADC in CTM mode
 */
#define M_MAX_CTM_GPADC_PARAMS        (1U)

/*! \brief
 *  GPADC TIMEOUT
 *  Max Number of Sample collect : 256
 *  Max Time for 256 samples collection : 256 * 16/10MHz = 409.6us
 *  Add 10 % margin : 450us
 *  Add 550us for skip samples
 *  Total timeout: 1ms
 *  Timout in terms of pmu count = 1ms/5ns = 200,000
 */
#define M_GPADC_TIMEOUT_MAX         (200000U)

#define AR_RFANACIO_TW_ANA_TMUX_CTRL_REG_ADDR                        (0xA32013CCU)
#define AR_RFANACIO_TW_ANA_TMUX_CTRL_CLK_TMUX_ESD_CTRL_MASK          (0x40000000U)
#define AR_RFANACIO_TW_ANA_TMUX_CTRL_CLK_TMUX_ESD_CTRL_SHIFT         (0x0000001EU)
#define CLK_TMUX_ESD_CTRL_ENABLE                                     (1U)

#define AR_RFANACIO_RX_REFSYS_TMUX_SPARE_CTRL_REG_ADDR                        (0xA3201144U)
#define AR_RFANACIO_RX_REFSYS_TMUX_SPARE_CTRL_ANA_TEST_ESD_MUX_EN_MASK        (0x80000000U)
#define AR_RFANACIO_RX_REFSYS_TMUX_SPARE_CTRL_ANA_TEST_ESD_MUX_EN_SHIFT       (0x0000001FU)
#define ANA_TEST_ESD_MUX_EN                                                   (1U)

/******************************************************************************
 * TYPE-DEFINE STRUCT/ENUM/UNION DEFINITIONS
 ******************************************************************************
 */

/*! \brief
 *  Configuration Parameters for GPADC in IFM mode
 *    ParamValue        : Value to be programmed in one hot reg
 *    CollectSamples    : Number of samples to collect @625KHz
 *    SkipSamples       : Number of samples to skip @10MHz
 *                        Time = Mantissa[3:0] * 2^(Exponent[6:4]) / 10M
 */
typedef union
{
    struct
    {
        uint32_t b8_ParamValue             : 8;    /*  bits  7:  0  */
        uint32_t b8_CollectSamples         : 8;    /*  bits 15:  8  */
        /* Skip samples = Mantissa[3:0] * 2^(Exponent[6:4]) */
        uint32_t b7_SkipSamples            : 7;    /*  bits 22: 16 - */
        uint32_t b9_Reserved               : 9;    /*  bits 31: 23 - */
    } bits;
    uint32_t b32_Val;
}GPADC_ParamInfoType;

typedef struct
{
    bool                  isChannelBufferedMode;
    uint32_t              channelConfigValue;
    GPADC_ParamInfoType   channelParamValue;
    bool                  isChannelConfigured;
}GPADC_DriverChannelConfigType;

typedef struct
{
    /**< Operation mode of the group */
	GPADC_ChannelConvModeType        convMode;
	GPADC_TriggerSourceType          triggSrc;
	GPADC_StatusType                 driverStatus;
	/**< Channel configurations */
	GPADC_DriverChannelConfigType   driverChannelConfig[MAX_GPADC_MEAS_SOURCES];
	uint16_t                        *ResultBufferPtr;
}GPADC_DriverObjectType;

/*! \brief
 *  Configuration Parameters for GPADC LUT in IFM mode
 *    32 bits:  ConfigValue
 *    32 bits:  ParamInfo
 */
typedef struct
{
    uint32_t             UnbuffConfigValue;
    uint32_t             BuffConfigValue;
    GPADC_ParamInfoType  ParamInfo;
    uint16_t             TotalTime; /* in unit of 100ns = CollectSamples*16 + SkipSamples */
}GPADC_CfgAndParamValuesType;

/*! \brief
 *  GPADC returning min, avg, max and sum
 */
typedef struct
{
    uint16_t Avg;
    uint16_t Min;
    uint16_t Max;
    uint32_t Sum;
}GPADC_ResultType;

/******************************************************************************
 * EXTERN GLOBAL VARIABLES/DATA-TYPES DECLARATIONS
 ******************************************************************************
 */

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */

void GPADC_DriverInit(const GPADC_ConfigType *CfgPtr);
void GPADC_DriverDeInit(void);
void GPADC_ResetRelease(void);
GPADC_ConvResultType GPADC_StartGroup(GPADC_channelsGroupSelectType channels);
GPADC_ConvResultType GPADC_StartSingleChannel(GPADC_MeasExtSrcType channelID, uint16_t *gpadcValue);
void GPADC_StopGroup(void);
void GPADC_Cfg(uint32_t cfgVal, uint32_t paramVal);
void GPADC_SWTrigger(void);
void GPADC_ReadSamplingResult(uint8_t c_numSamples, GPADC_ResultType *gpAdcResult);
void GPADC_Convert(uint32_t w_cfgVal, uint32_t paramVal, GPADC_ResultType *gpAdcResult);
uint8_t GPADC_ConvSkipTimeToCode(uint32_t skipSamples);

#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF DRV_GPADC_SOC_H
 */
