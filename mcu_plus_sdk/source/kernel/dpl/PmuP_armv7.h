    /**
 * @file  PmuP_armv7.h
 *
 * @brief
 *  Header file containing various enumerations, structure definitions and function
 *  declarations for the ARM R5 PMU IP.
 *  ============================================================================
 *  @n   (C) Copyright 2023, Texas Instruments, Inc.
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
#ifndef PMUP_ARM_V7_H
#define PMUP_ARM_V7_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @defgroup PmuP_ARM_R5_PMU_API R5 PMU API
 *
 * Provides the APIs to configure/read Performance Monitor Unit counters
 *
 */

/**
@defgroup PmuP_ARM_R5_PMU_FUNCTION  R5 PMU Functions
@ingroup PmuP_ARM_R5_PMU_API
*/
/**
@defgroup PmuP_ARM_R5_PMU_ENUM R5 PMU Enumerated Data Types
@ingroup PmuP_ARM_R5_PMU_API
*/

/**
@defgroup PmuP_ARM_R5_PMU_MACROS R5 PMU Macros
@ingroup PmuP_ARM_R5_PMU_API
*/

/**
 *  @addtogroup PmuP_ARM_R5_PMU_ENUM
    @{
 *
 */


/**
 * @brief This enumerator defines PMU event types
 *
 *
 */
typedef enum
{
    PmuP_ARM_R5_PMU_EVENT_TYPE_SWINC                     = 0,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ICACHE_MISS               = 0x01U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_MISS               = 0x03U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_ACCESS             = 0x04U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_D_RD                      = 0x06U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_D_WR                      = 0x07U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_I_X                       = 0x08U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_PI_X                      = 0x5EU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_EXCEPTION                 = 0x09U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_EXCEPTION_RET             = 0x0AU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_CID_CHANGE                = 0x0BU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_SW_PC                     = 0x0CU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_B_IMMEDIATE               = 0x0DU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_PROC_RET                  = 0x0EU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_UNALIGNED_ACCESS          = 0x0FU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_BRANCH_TAKEN              = 0x10U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_BRANCH_PRED               = 0x12U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ICACHE_STALL              = 0x40U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_STALL              = 0x41U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_WB                 = 0x42U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_MEM_REQ                   = 0x43U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_LSU_BUSY_STALL            = 0x44U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_SB_DRAIN                  = 0x45U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_FIQ_DISABLED_CYCLES       = 0x46U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_IRQ_DISABLED_CYCLES       = 0x47U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ETMEXTOUTM0               = 0x48U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ETMEXTOUTM1               = 0x49U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ICACHE_TAG_CECC           = 0x4AU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ICACHE_DATA_CECC          = 0x4BU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_TAG_CECC           = 0x4CU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_DATA_CECC          = 0x4DU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_TCM_FECC_PF               = 0x4EU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_TCM_FECC_LS               = 0x4FU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_SB_MERGE                  = 0x50U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_LSU_SB_STALL              = 0x51U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_LSU_QF_STALL              = 0x52U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_INT_DIV                   = 0x53U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_INT_DIV_STALL             = 0x54U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_PLD_LINEFILL              = 0x55U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_PLD_NO_LINEFILL           = 0x56U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_NONCACHEABLE_ACCESS       = 0x57U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ICACHE_ACCESS             = 0x58U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_SB_ATTR                   = 0x59U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DUAL_ISSUE_CASE_A         = 0x5AU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DUAL_ISSUE_CASE_B         = 0x5BU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DUAL_ISSUE_CASE_OTHER     = 0x5CU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DOUBLE_FP                 = 0x5DU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_DATA_FECC          = 0x60U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_DCACHE_TAG_FECC           = 0x61U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_LIVELOCK                  = 0x62U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ATCM_MB_ECC               = 0x64U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_B0TCM_MB_ECC              = 0x65U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_B1TCM_MB_ECC              = 0x66U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ATCM_SB_ECC               = 0x67U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_B0TCM_SB_ECC              = 0x68U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_B1TCM_SB_ECC              = 0x69U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_TCM_CECC_LS               = 0x6AU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_TCM_CECC_PF               = 0x6BU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_TCM_FECC_AXI              = 0x6CU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_TCM_CECC_AXI              = 0x6DU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_CORRECTABLE_EVENTS        = 0x6EU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_FATAL_EVENTS              = 0x6FU,
    PmuP_ARM_R5_PMU_EVENT_TYPE_CORRECTABLE_BUS_FAULTS    = 0x70U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_FATAL_BUS_FAULTS          = 0x71U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ACP_DCACHE_ACCESS         = 0x72U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_ACP_DCACHE_INVALIDATE     = 0x73U,
    PmuP_ARM_R5_PMU_EVENT_TYPE_CYCLE_CNT                 = 0xFFU
} PmuP_ArmR5PmuEventType;


/* @} */

/**
 *  \addtogroup PmuP_ARM_R5_PMU_MACROS
 *  @{
 */
/** PMU Cycle count number */
#define PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM                                           (31U)

/** PMU_CFG_CNTR_EVENT_TYPE */
#define PmuP_ARM_R5_PMU_CFG_CNTR_EVENT_TYPE_MASK                                    (0x000000FFU)
#define PmuP_ARM_R5_PMU_CFG_CNTR_EVENT_TYPE_SHIFT                                   (0x00000000U)
#define PmuP_ARM_R5_PMU_CFG_CNTR_EVENT_TYPE_RESETVAL                                (0x00000000U)
#define PmuP_ARM_R5_PMU_CFG_CNTR_EVENT_TYPE_MAX                                     (0x000000FFU)

/* @} */

/**
 *  \addtogroup PmuP_ARM_R5_PMU_FUNCTION
 *  @{
 */


/**
 *  \brief Configure the Performance Management Unit (PMU)
 *  This function is used to configure the PMU.
 *
 *  The value arguments provided to this function are directly written into
 *  the corresponding R5 system register. As a result, the values must be
 *  constructed per the expected register format.
 *
 *  See the "MPU memory region programming registers" section of the ARM Cortex
 *  R5 TRM for more information.
 *
 *  \param cycleCntDiv      [IN]    Cycle count divider: 0=Counts every
 *                                  processor clock cycle, otherwise counts
 *                                  every 64th processor clock cycle
 *  \param exportEvents     [IN]    Export of the events to the event bus for an
 *                                  external monitoring block: 0=Export disabled,
 *                                  otherwise export is enabled.
 *  \param userEnable       [IN]    User mode access to performance monitor and
 *                                  validation registers: 0=disabled, otherwise
 *                                  enabled.
 *
 *  \return None
 */
extern void PmuP_armR5Cfg( uint32_t cycleCntDiv, uint32_t exportEvents, uint32_t userEnable );

/**
 *  \brief Enable/disable all PMU counters
 *  This function is used to enable or disable all of the PMU counters.
 *  Note that to enable a specific counter, both this function and the
 *  #PmuP_armR5PmuEnableCntr function must be called.
 *
 *  \param enable   [IN]    0=All counters are disabled, otherwise
 *                          they are enabled
 *
 *  \return None
 */
extern void PmuP_armR5EnableAllCntrs( uint32_t enable );

/**
 *  \brief Get the number of PMU counters supported
 *  This function is used to get the number of PMU counters supported.
 *
 *
 *  \return The number of PMU counters supported
 */
extern uint32_t PmuP_armR5GetNumCntrs( void );

/**
 *  \brief Configure a PMU counter
 *  This function is used to configure a PMU counter.
 *
 *  \param cntrNum      [IN]    Counter number (0..(#PmuP_armR5PmuGetNumCntrs()-1))
 *  \param eventType    [IN]    Event type to count. See #PmuP_ArmR5PmuEventType
 *                              for available event types.
 *
 *  \return None
 */
extern void PmuP_armR5CfgCntr( uint32_t cntrNum, PmuP_ArmR5PmuEventType eventType );

/**
 *  \brief Enable/disable overflow interrupt generation for a PMU counter
 *  This function is used to enable or disable overflow interrupt generation for the specified PMU counter.
 *
 *  \param cntrNum      [IN]    Counter number (0..(#PmuP_armR5PmuGetNumCntrs()-1))
 *                              or PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM)
 *  \param enable       [IN]    0=Overflow interrupt generation is disabled,
 *                              otherwise it is enabled
 *
 *  \return None
 */
extern void PmuP_armR5EnableCntrOverflowIntr( uint32_t cntrNum, uint32_t enable );

/**
 *  \brief Enable/disable a PMU counter
 *  This function is used to enable or disable the specified PMU counter.
 *  Note that to enable a specific counter, both this function and the
 *  #PmuP_armR5PmuEnableAllCntrs function must be called.
 *
 *  \param cntrNum      [IN]    Counter number (0..(PmuP_armR5PmuGetNumCntrs()-1))
 *                              or PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM)
 *  \param enable       [IN]    0=Disable counter, otherwise it is enabled
 *
 *  \return None
 */
extern void PmuP_armR5EnableCntr( uint32_t cntrNum, uint32_t enable );

/**
 *  \brief Read a PMU counter
 *  This function is used to read the specified PMU counter.
 *
 *  \param cntrNum      [IN]    Counter number (0..(PmuP_armR5PmuGetNumCntrs()-1))
 *                              or PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM)
 *
 *  \return The current count associated with the specified PMU counter
 */
extern uint32_t PmuP_armR5ReadCntr( uint32_t cntrNum );

/**
 *  \brief Set a PMU counter
 *  This function is used to set the specified PMU counter.
 *
 *  \param cntrNum      [IN]    Counter number (0..(PmuP_armR5PmuGetNumCntrs()-1))
 *                              or PmuP_ARM_R5_PMU_CYCLE_COUNTER_NUM)
 *  \param cntrVal      [IN]    Counter Value to be set
 *
 *  \return None
 */
extern uint32_t PmuP_armR5SetCntr( uint32_t cntrNum, uint32_t cntrVal);

/**
 *  \brief Clear the overflow flag for the specified counter(s)
 *  This function is used to clear the overflow flag for the specified
 *  counter(s). The cntrMask argument is a bit-mask where bits set to '1'
 *  indicate which counter(s)' overflow flag to clear. See the 'Overflow
 *  Flag Status Register' description in the ARM R5 TRM for the format of
 *  this bitmask.
 *
 *  \param cntrMask     [IN]    Bit-mask indicating which counter(s)' overflow
 *                      flag to clear
 *
 *  \return None
 */
extern void PmuP_armR5ClearCntrOverflowStatus( uint32_t cntrMask );

/**
 *  \brief Reset the cycle counter to zero
 *  This function resets the cycle counter to zero.
 *
 *
 *  \return None
 */
extern void PmuP_armR5ResetCycleCnt( void );

/**
 *  \brief Reset all counters to zero
 *  This function resets all event counters to zero.
 *
 *
 *  \return None
 */
extern void PmuP_armR5ResetCntrs( void );

/**
 *  \brief Selects the PMU counter
 *  This function selects the PMU counter by writing into the PMSELR register.
 *
 *  \param cntrNum      [IN]    Counter number (0..(PmuP_armR5PmuGetNumCntrs()-1))
 *
 *  \return None
 */
void PmuP_armR5SelectCntr( uint32_t cntrNum );

/**
 *  \brief Setup PMU
 *  This function writes the PMCR register.
 *
 *  \return None
 */
void PmuP_setup(uint32_t);

/**
 *  \brief Enables PMU counter
 *  This function writes PMCNTENSET register to enable the counter.
 *
 *  \return None
 */
void PmuP_enableCounters(uint32_t);

/**
 *  \brief Disables PMU counter
 *  This function writes PMCNTENCLR register to disable the counter.
 *
 *  \return None
 */
void PmuP_disableCounters(uint32_t);

/**
 *  \brief Read the overflow status for all of the counters
 *  This function is used to read the overflow status for all of the counters.
 *  A bit-mask is returned where bits set to '1' indicate overflow occurred for
 *  the corresponding counter. See the 'Overflow Flag Status Register'
 *  description in the ARM R5 TRM for the format of this bitmask.
 *
 *  \return the value of PMOVSR register
 */
uint32_t PmuP_getOverflowStatus(void);

/**
 *  \brief Clear the overflow flag for all the counters
 *
 *  \return None
 */
uint32_t PmuP_clearOverflowStatus(void);

/* @} */

#ifdef __cplusplus
}
#endif  /* extern "C" */

#endif  /* PMUP_ARM_V7_H */
