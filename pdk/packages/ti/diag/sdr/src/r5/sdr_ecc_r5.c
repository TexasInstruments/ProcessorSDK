/*
 * SDR ECC
 *
 * Software Diagnostics Reference module for ECC
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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

#include <string.h>
#include <sdr_ecc.h>
#include <ti/csl/csl_ecc_aggr.h>
#include <ti/csl/soc.h>
#include <ti/csl/soc/cslr_soc_ecc_aggr.h>

#include <ti/csl/arch/r5/csl_arm_r5_pmu.h>
#include <ti/csl/arch/r5/csl_vim.h>

#include <sdr_utils.h>
#include <sdr_ecc_utils.h>

#include <sdr_ecc_priv.h>
#include <sdr_ecc_core.h>
#include <sdr_soc_r5.h>

/* Local defines */
#define SDR_ECC_R5_MONITOR_PMU_NUMBER (1u)
#define SDR_ECC_R5_CFLR_ATCM_DATA_ERROR_MASK (0x1000001u)
#define SDR_ECC_R5_CFLR_BTCM_DATA_ERROR_MASK (0x2000001u)

#define SDR_PMU_CTR_MAX_VALUE (0xffffffffu)

#define SDR_ECC_ATCM_SINGLE_BIT_ERROR_EVENT (0x67u)
#define SDR_ECC_B0TCM_SINGLE_BIT_ERROR_EVENT (0x68u)
#define SDR_ECC_B1TCM_SINGLE_BIT_ERROR_EVENT (0x69u)

/* Local functions */
static inline void SDR_ECC_R5EnablePmuForEccEvent(SDR_ECC_MemType eccMemType,
                              SDR_ECC_MemSubType memSubType,
                              SDR_ECC_InjectErrorType errorType);

static inline void SDR_ECC_R5DisablePmuForEccEvent(void);
/*********************************************************************
 *
 * /brief   Configure ECC for given ram Id for the CPU
 *
 * /param  ramId: Ram Id of memory section
 *
 * /return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_ECC_configECCRam(uint32_t ramId)
{
    SDR_Result retVal= SDR_PASS;

    switch(ramId) {

        case CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK0_RAM_ID:
        case CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_ATCM0_BANK1_RAM_ID:
            /* Enable ECC for ATCM */
            SDR_ECC_UTILS_enableECCATCM();

            break;

        case CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK0_RAM_ID:
        case CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B0TCM0_BANK1_RAM_ID:
            /* Enable ECC for B0TCM */
            SDR_ECC_UTILS_enableECCB0TCM();

            break;

        case CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK0_RAM_ID:
        case CSL_MCU_R5FSS0_CORE0_ECC_AGGR_PULSAR_SL_B1TCM0_BANK1_RAM_ID:
            /* Enable ECC for B1TCM */
            SDR_ECC_UTILS_enableECCB1TCM();

            break;

        default:
            if ( ramId > CSL_MCU_R5FSS0_CORE0_ECC_AGGR_CPU0_KS_VIM_RAMECC_RAM_ID) {
                retVal = SDR_FAIL;
            }
            break;
    }
    return retVal;

}

/*********************************************************************
 *
 * /brief   Poll ecc event
 *
 * \param  eccMemType: Memory type
 * \param  memSubType: Memory subtype
 * \param  errorType:  Error type to poll
 *
 * \return SDR_ECC_EVENT_FOUND or if no events found
 */
uint32_t SDR_ECC_pollErrorEvent(SDR_ECC_MemType eccMemType,
                               SDR_ECC_MemSubType memSubType,
                               SDR_ECC_InjectErrorType errorType)
{
    uint32_t retValue = 0u;
    uint32_t regValue;

    /* Polling only for R5F core self test */
    if ((eccMemType == SDR_ECC_MEMTYPE_MCU_R5F0_CORE)
        || (eccMemType == SDR_ECC_MEMTYPE_MCU_R5F1_CORE)) {
        switch(errorType) {
            case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                /* Only for single bit error do polling */
                switch (memSubType) {
                    case SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID:
                    case SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID:
                        /* Check PMOVSR register */
                        regValue =  SDR_UTILS_getPMOVSR();
                        if(regValue & (1u <<SDR_ECC_R5_MONITOR_PMU_NUMBER)) {
                            /* Check CFLR register */
                            regValue = SDR_UTILS_getCFLR();
                            if((regValue & SDR_ECC_R5_CFLR_ATCM_DATA_ERROR_MASK)
                               == SDR_ECC_R5_CFLR_ATCM_DATA_ERROR_MASK) {
                                retValue = (uint32_t)(SDR_ECC_EVENT_FOUND);
                            }
                        }
                        break;

                    case SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID:
                    case SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID:
                    case SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID:
                    case SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID:
                        /* Check PMOVSR register */
                        regValue =  SDR_UTILS_getPMOVSR();
                        if(regValue & (1u <<SDR_ECC_R5_MONITOR_PMU_NUMBER)) {
                            /* Check CFLR register */
                            regValue = SDR_UTILS_getCFLR();
                            if( (regValue & SDR_ECC_R5_CFLR_BTCM_DATA_ERROR_MASK)
                                == SDR_ECC_R5_CFLR_BTCM_DATA_ERROR_MASK) {
                                retValue = (uint32_t)(SDR_ECC_EVENT_FOUND);
                            }
                        }
                        break;

                    default:

                        break;
                }
                break;

            default:
                break;
        }
    }
    return retValue;
}

/*********************************************************************
 *
 * \brief   Configure PMU to monitor specific ECC event
 *
 * \param  eccMemType: Memory type
 * \param  memSubType: Memory subtype
 * \param  errorType:  Error type to poll
 *
 * \return  None
 */
static inline void SDR_ECC_R5EnablePmuForEccEvent(SDR_ECC_MemType eccMemType,
                              SDR_ECC_MemSubType memSubType,
                              SDR_ECC_InjectErrorType errorType)
{
    if ((eccMemType == SDR_ECC_MEMTYPE_MCU_R5F0_CORE)
        || (eccMemType == SDR_ECC_MEMTYPE_MCU_R5F1_CORE)) {
        switch (memSubType) {
            case SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK0_VECTOR_ID:
            case SDR_ECC_R5F_MEM_SUBTYPE_ATCM0_BANK1_VECTOR_ID:
                switch(errorType) {
                    case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                       /* enable PMU event monitoring of ATCM 1 bit*/
                       SDR_ECC_UTILS_configSecIntr(SDR_PMU_CTR_MAX_VALUE,
                                                 SDR_ECC_ATCM_SINGLE_BIT_ERROR_EVENT,
                                                 1);
                       break;

                    default:
                        break;
                }
                break;

            case SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK0_VECTOR_ID:
            case SDR_ECC_R5F_MEM_SUBTYPE_B0TCM0_BANK1_VECTOR_ID:
                switch(errorType) {
                    case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                       /* enable PMU event monitoring of ATCM 1 bit*/
                       SDR_ECC_UTILS_configSecIntr(SDR_PMU_CTR_MAX_VALUE,
                                                 SDR_ECC_B0TCM_SINGLE_BIT_ERROR_EVENT,
                                                 1);
                       break;

                    default:
                        break;
                }
                break;

            case SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK0_VECTOR_ID:
            case SDR_ECC_R5F_MEM_SUBTYPE_B1TCM0_BANK1_VECTOR_ID:
                switch(errorType) {
                    case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                       /* enable PMU event monitoring of ATCM 1 bit*/
                       SDR_ECC_UTILS_configSecIntr(SDR_PMU_CTR_MAX_VALUE,
                                                 SDR_ECC_B1TCM_SINGLE_BIT_ERROR_EVENT,
                                                 1);
                       break;

                    default:
                       break;
               }
               break;

            default:
                break;
        }
    }
}

/*********************************************************************
 *
 * \brief   Disable PMU from monitoring specific ECC event
 *
 * \param  eccMemType: Memory type
 * \param  memSubType: Memory subtype
 * \param  errorType:  Error type to monitor
 *
 * \return  None
 */
static inline void SDR_ECC_R5DisablePmuForEccEvent(void)
{
    /* PMU Disable counter */
    (void)CSL_armR5PmuEnableCntr(SDR_ECC_R5_MONITOR_PMU_NUMBER, 0u );
    /* Disable counter overflow interrupt */
    (void)CSL_armR5PmuEnableCntrOverflowIntr(SDR_ECC_R5_MONITOR_PMU_NUMBER, 0u);

}

/*********************************************************************
 *
 * \brief   Disable monitoring specific ECC event
 *
 * \param  eccMemType: Memory type
 * \param  memSubType: Memory subtype
 * \param  errorType:  Error type to monitor
 *
 * \return  None
 */
void SDR_ECC_enableECCEventCheck(SDR_ECC_MemType eccMemType,
                                SDR_ECC_MemSubType memSubType,
                                SDR_ECC_InjectErrorType errorType)
{
    switch(eccMemType)
    {
        case SDR_ECC_MEMTYPE_MCU_R5F0_CORE:
        case SDR_ECC_MEMTYPE_MCU_R5F1_CORE:

            switch(errorType)
            {
                case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE:
                case SDR_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT:
                case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT:
                    SDR_ECC_R5EnablePmuForEccEvent(eccMemType, memSubType, errorType);
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    return;
}

/*********************************************************************
 *
 * \brief   Disable monitoring specific ECC event
 *
 * \param  eccMemType: Memory type
 * \param  errorType:  Error type to monitor
 *
 * \return  None
 */
void SDR_ECC_disableECCEventCheck(SDR_ECC_MemType eccMemType,
                                 SDR_ECC_InjectErrorType errorType)
{
    switch(eccMemType)
     {
         case SDR_ECC_MEMTYPE_MCU_R5F0_CORE:
         case SDR_ECC_MEMTYPE_MCU_R5F1_CORE:

             switch(errorType)
             {
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_ONCE:
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_ONCE:
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_REPEAT:
                 case SDR_INJECT_ECC_ERROR_FORCING_1BIT_N_ROW_REPEAT:
                     SDR_ECC_R5DisablePmuForEccEvent();
                     break;

                 default:
                     break;
             }
             break;

         default:
             break;
     }

     return;

}

/** ============================================================================
 *
 * \brief   Register Handler for VIM DED ECC error
 *
 * \param  VIMDEDHandler: This is function to be called when an Double bit ECC
 *                        error happens on the VIM RAM. NOTE: That the regular
 *                        vector load will be replaced with this registered
 *                        function.
 *
 * \return  None
 */
void SDR_ECC_registerVIMDEDHandler(SDR_ECC_VIMDEDVector_t VIMDEDHandler)
{
    CSL_vimRegs     *pVimRegs;

    /* initialize the address */
    pVimRegs        = (CSL_vimRegs *)(SDR_MCU_VIC_CFG_BASE);

    CSL_vimSetDedVectorAddr(pVimRegs, (uint32_t)VIMDEDHandler);
}

