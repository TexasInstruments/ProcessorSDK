/*
 *  Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
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
 *  \ingroup CSL_RTI
 *  \defgroup CSL_RTI_TIMER RTI Timer
 *
 *  @{
 */
/**
 *  \file   rtiTmr.h
 *
 *  \brief  RTI Timer API prototypes and macros.
 *
 *   This file contains the RTI Timer API prototypes and macro definitions.
 **/

#ifndef RTI_TMR_H_
#define RTI_TMR_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <ti/csl/cslr_rti.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/**
 * \anchor CSL_RTIStallModes
 * \name   RTI Global Control Stall Modes
 */
/**
 * \brief  macro to select the stall mode in RTI Global Control.
 *
 *         The counters can be either stopped or running while in debug halting mode.
 */
#define    RTI_GC_STALL_MODE_ON     (RTI_RTIGCTRL_COS_STOPPED)
    /**< The counters will be stopped while in debug halting mode */
#define    RTI_GC_STALL_MODE_OFF    (RTI_RTIGCTRL_COS_RUNNING)
    /**< The counters will be running while in debug halting mode */

/**
 * \brief  type to select the stall mode in RTI global control.
 *         The counters can be either stopped or running while in halting debug mode.
 */
typedef uint32_t rtiGcStallMode_t;

/**
 * \anchor CSL_RTITmrCntBlkIndexes
 * \name RTI Timer Counter Block Indexes
 * \brief RTI Timer contains two counter blocks
 *  @{
**/
/*! RTI Timer counter block 0 */
#define RTI_TMR_CNT_BLK_INDEX_0        (0U)

/*! RTI Timer counter block 1 */
#define RTI_TMR_CNT_BLK_INDEX_1        (1U)

/*! RTI Timer counter block maximum */
#define RTI_TMR_CNT_BLK_INDEX_MAX      (RTI_TMR_CNT_BLK_INDEX_1)

/* @} */

/**
 * \anchor CSL_RTITmrCmpBlkIndexes
 * \name RTI Timer Comapre Block Indexes
 * \brief RTI Timer contains four compare blocks
 *  @{
**/
/*! RTI Timer compare block 0 */
#define RTI_TMR_CMP_BLK_INDEX_0        (0U)

/*! RTI Timer compare block 1 */
#define RTI_TMR_CMP_BLK_INDEX_1        (1U)

/*! RTI Timer compare block 2 */
#define RTI_TMR_CMP_BLK_INDEX_2        (2U)

/*! RTI Timer compare block 3 */
#define RTI_TMR_CMP_BLK_INDEX_3        (3U)

/*! RTI Timer compare block maximum */
#define RTI_TMR_CMP_BLK_INDEX_MAX      (RTI_TMR_CMP_BLK_INDEX_3)
/* @} */

/**
 * \anchor CSL_RTITmrPrescalerValues
 * \name   RTI Timer Prescaler Value Ranges
 *  @{
 */
/**
 * \brief  Minimum possible prescaler value for RTI Timer UP counter.
 */
#define RTI_TMR_MIN_PRESCALER_VAL           (1U)
/**
 * \brief  Maximum possible prescaler value for RTI Timer UP counter(2^32).
 */
#define RTI_TMR_MAX_PRESACLER_VAL           (0xFFFFFFFFU)
/* @} */

/**
 * \anchor CSL_RTITmrIntFlags
 * \name RTI Timer Interrupt Set/Indication Flags
 *  @{
**/
/* \brief Comapre 0 match interrupt */
#define RTI_TMR_INT_INT0_FLAG        (RTI_RTISETINT_SETINT0_MASK)

/* \brief Comapre 1 match interrupt */
#define RTI_TMR_INT_INT1_FLAG        (RTI_RTISETINT_SETINT1_MASK)

/* \brief Comapre 2 match interrupt */
#define RTI_TMR_INT_INT2_FLAG        (RTI_RTISETINT_SETINT2_MASK)

/* \brief Comapre 3 match interrupt */
#define RTI_TMR_INT_INT3_FLAG        (RTI_RTISETINT_SETINT3_MASK)

/* \brief Comapre 0 match DMA request */
#define RTI_TMR_INT_DMA0_FLAG        (RTI_RTISETINT_SETDMA0_MASK)

/* \brief Comapre 1 match DMA request */
#define RTI_TMR_INT_DMA1_FLAG        (RTI_RTISETINT_SETDMA1_MASK)

/* \brief Comapre 2 match DMA request */
#define RTI_TMR_INT_DMA2_FLAG        (RTI_RTISETINT_SETDMA2_MASK)

/* \brief Comapre 3 match DMA request */
#define RTI_TMR_INT_DMA3_FLAG        (RTI_RTISETINT_SETDMA3_MASK)

/* \brief Timebase interrupt: detection of a missing external clock edge */
#define RTI_TMR_INT_TB_FLAG          (RTI_RTISETINT_SETTBINT_MASK)

/* \brief Free running counter 0 overflow */
#define RTI_TMR_INT_OVL0_FLAG        (RTI_RTISETINT_SETOVL0INT_MASK)

/* \brief Free running counter 1 overflow */
#define RTI_TMR_INT_OVL1_FLAG        (RTI_RTISETINT_SETOVL1INT_MASK)

/* \brief All interrupt status mask */
#define RTI_TMR_INT_STATUS_ALL       (RTI_TMR_INT_INT0_FLAG |   \
                                      RTI_TMR_INT_INT1_FLAG |   \
                                      RTI_TMR_INT_INT2_FLAG |   \
                                      RTI_TMR_INT_INT3_FLAG |   \
                                      RTI_TMR_INT_TB_FLAG   |   \
                                      RTI_TMR_INT_OVL0_FLAG |   \
                                      RTI_TMR_INT_OVL1_FLAG)

/* \brief All interrupt request mask */
#define RTI_TMR_INT_REQ_ALL          (RTI_TMR_INT_INT0_FLAG |   \
                                      RTI_TMR_INT_INT1_FLAG |   \
                                      RTI_TMR_INT_INT2_FLAG |   \
                                      RTI_TMR_INT_INT3_FLAG |   \
                                      RTI_TMR_INT_DMA0_FLAG |   \
                                      RTI_TMR_INT_DMA1_FLAG |   \
                                      RTI_TMR_INT_DMA2_FLAG |   \
                                      RTI_TMR_INT_DMA3_FLAG |   \
                                      RTI_TMR_INT_TB_FLAG   |   \
                                      RTI_TMR_INT_OVL0_FLAG |   \
                                      RTI_TMR_INT_OVL1_FLAG)
/* @} */

/**
 * \anchor CSL_RTITmrNTUIds
 * \name RTI Timer NTU IDs
 * \brief RTI Timer Network Time (NTU) IDs
 *  @{
**/
/*! RTI Timer NTU 0 */
#define RTI_TMR_NTU_0        (0x0U)

/*! RTI Timer NTU 1 */
#define RTI_TMR_NTU_1        (0x5U)

/*! RTI Timer NTU 0 */
#define RTI_TMR_NTU_2        (0xAU)

/*! RTI Timer NTU 0 */
#define RTI_TMR_NTU_3        (0xFU)
/* @} */

/**
 * \anchor CSL_RTITmrCapEvent
 * \name RTI Timer Capture Event Indexes
 * \brief RTI Timer Capture Event source numbers
 *  @{
**/
/*! RTI Timer Capture Event source 0 */
#define RTI_TMR_CAPTURE_EVT_0        (0U)

/*! RTI Timer Capture Event source 0 */
#define RTI_TMR_CAPTURE_EVT_1        (1U)

/*! RTI Timer Capture Event source maximum */
#define RTI_TMR_CAPTURE_EVT_MAX      (RTI_TMR_CAPTURE_EVT_1)
/* @} */

/**
 * \anchor CSL_RTITmrClkSrc
 * \name RTI Timer Clock Source
 * \brief RTI Timer clock source of conter block 0
 *  @{
**/
/*! Use counter block 0 up counter as clock source of the free runnung counter */
#define RTI_TMR_CLK_SRC_COUNTER     (0U)

/*! Use one of NTU as clock source of the free runnung counter */
#define RTI_TMR_CLK_SRC_NTU         (1U)
/* @} */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */
/** @brief
 *
 *  RTI Timer configuration info.
 */
typedef struct rtiTmrConfig_s
{
    /**  Select Counter Block, see @ref CSL_RTITmrCntBlkIndexes */
    uint32_t        cntBlkIndex;

    /** Select Comapre Block, see @ref CSL_RTITmrCmpBlkIndexes */
    uint32_t        cmpBlkIndex;

    /** Select clock source of the free running counter of Counter Block 0
     *  0: Up counter 0
     *  1: One of the NTU clocks
     *  @note: this parameter is not applicable to counter block 1
     */
    uint32_t        clkSrc;

    /** Select the NTU clock, see @ref CSL_RTITmrNTUIds */
    uint32_t        ntu;

    /**  Specify the content of the compare register of the up counter, the actual period
     *   (prescaler) will be this parameter plus one.
     *   @note: the presacler will be 2**32 + 1 when the value is set to 0, although that
     *          is highly discourged.
     */
    uint32_t        prescaler;

    /**  Specify the period of the free running counter in terms of counts */
    uint32_t        period;

} rtiTmrConfig_t;

/** @brief
 *
 *  RTI Timer Compare Control info.
 */
typedef struct rtiTmrCmpCtrl_s
{
    /**  Select Counter Block, see @ref CSL_RTITmrCntBlkIndexes */
    uint32_t        cntBlkIndex;

    /**  initial compare value */
    uint32_t        cmpVal;

    /**  Specify the period of the free running counter in terms of counts */
    uint32_t        period;

} rtiTmrCmpCtrl_t;

/** @brief
 *
 *  RTI Timer Counter Capture Control info.
 */
typedef struct rtiTmrCapCtrl_s
{
    /** Capture triggered source for the counter block 0 */
    uint32_t        capSrcCnt0;

    /** Capture triggered source for the counter block 1 */
    uint32_t        capSrcCnt1;

} rtiTmrCapCtrl_t;

/** @brief
 *
 *  RTI Timer Counter values.
 */
typedef struct rtiTmrCnts_s
{
    /** vaue of the UP counter */
    uint32_t        up;

    /** value of the FRC counter */
    uint32_t        frc;

} rtiTmrCnts_t;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/**
 * \brief   Set the stall mode in RTI Global Control.
 *
 * \param   baseAddr       Base Address of the RTI instance.
 *
 * \param   gcStallMode    Stall mode in RTI global control
 *                         Values given by macro #rtiGcStallMode_t
 *
 * \return  status         Success of the stall mode configuration
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 */
int32_t RTIGcSetStallMode(uint32_t baseAddr, rtiGcStallMode_t gcStallMode);

/**
 * \brief   Start the timer.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   cntIndex        Conter Block index
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 * \note    The timer must be configured before it is started/enabled.
 *
 **/
int32_t RTITmrEnable(uint32_t baseAddr, uint32_t cntIndex);

/**
 * \brief   Stop the timer.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   cntIndex        Conter Block index
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 **/
int32_t RTITmrDisable(uint32_t baseAddr, uint32_t cntIndex);

/**
 * \brief   Configure Timer before enabling
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   pConfig        pointer to Configuration parameters
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 * \note    The timer will be stopped and cleared before the new confiuration.
 *
 **/
int32_t RTITmrConfigure(uint32_t baseAddr, rtiTmrConfig_t *pConfig);

/**
 * \brief   Clear Timer
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   cntIndex        Conter Block index
 * \param   cmpIndex        Comapre Block index
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 * \note    The timer will be stopped and then cleared. The function may be invoked
 *          at device initialization prior to configuration
 *
 **/
int32_t RTITmrClear(uint32_t baseAddr, uint32_t cntIndex, uint32_t cmpIndex);

/**
 * \brief   Get/Read the counter value from the counter registers.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   cntIndex        Conter Block index
 * \param   pCnts           Pointer to the counter values
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 * \note:   Value can be read from the counter register when the counter is
 *          stopped or when it is running.
 **/
int32_t RTITmrCounterGet(uint32_t baseAddr, uint32_t cntIndex, rtiTmrCnts_t *pCnts);

/**
 * \brief   Configure Capture operation
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   pConfig        pointer to Capture control parameters
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 **/
int32_t RTITmrCaptureConfig(uint32_t baseAddr, rtiTmrCapCtrl_t *pConfig);

/**
 * \brief   Get/Read the counter value from the capture registers.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   cntIndex        Conter Block index
 * \param   pCnts           Pointer to the counter values
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 **/
int32_t RTITmrCaptureCounterGet(uint32_t baseAddr, uint32_t cntIndex, rtiTmrCnts_t *pCnts);

/**
 * \brief   Confiure Compare operation
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   cmpIndex        Comapre Block index
 * \param   pConfig        pointer to Comapre control parameters
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 **/
int32_t RTITmrCompareConfig(uint32_t baseAddr, uint32_t cmpIndex, rtiTmrCmpCtrl_t *pConfig);

/**
 * \brief   Get the compare match register contents.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   cmpIndex        Comapre Block index
 *
 * \return  This API returns the match register contents.
 *
 **/
uint32_t RTITmrCompareGet(uint32_t baseAddr, uint32_t cmpIndex);

/**
 * \brief   Read the status of INTFLAG register.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 *
 * \return  This API returns the status of INTFLAG register.
 *
 **/
uint32_t RTITmrIntStatusGet(uint32_t baseAddr);

/**
 * \brief   Clear the status of interrupt events.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   intFlags        Variable used to clear the events.
 *
 * 'intFlags' can take any of @ref CSL_RTITmrIntFlags
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 **/
int32_t RTITmrIntStatusClear(uint32_t baseAddr, uint32_t intFlags);

/**
 * \brief   Enable the Timer interrupts.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   intFlags        Variable used to enable the interrupts.
 *
 * 'intFlags' can take @ref CSL_RTITmrIntFlags
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 *
 **/
int32_t RTITmrIntEnable(uint32_t baseAddr, uint32_t intFlags);

/**
 * \brief   Disable the Timer interrupts.
 *
 * \param   baseAddr        Base Address of the RTI instance.
 * \param   intFlags        Variable used to disable the interrupts.
 *
 * 'intFlags' can take @ref CSL_RTITmrIntFlags
 *
 * \return  status          Success of the operation
 *                                - Success: CSL_PASS
 *                                - Fail   : CSL_EFAIL
 **/
int32_t RTITmrIntDisable(uint32_t baseAddr, uint32_t intFlags);

#ifdef __cplusplus
}
#endif
#endif
 /** @} */
/********************************* End of file ******************************/
