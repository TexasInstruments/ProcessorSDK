/*
 * SDR ESM
 *
 * Software Diagnostics Reference module for Error Signaling Module
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

#include <stdbool.h>
#include <stddef.h>

#include <sdr_esm.h>
#include <sdr_esm_soc.h>
#include <sdr_esm_core.h>
#include <sdr_esm_priv.h>

static SDR_ESM_Instance_t SDR_ESM_instance_MCU;
static SDR_ESM_Instance_t SDR_ESM_instance_WKUP;
static SDR_ESM_Instance_t SDR_ESM_instance_MAIN;

/** ================================================================================
 *
 * \brief        Check that ESM instance type is valid for this device, and fill the
 *               ESM base address
 *
 * \param [in]   esmInstType: ESM instance type
 * \param [out]  esmBaseAddr: Base address for ESM instance's registers
 *
 * \return       true: if valid instance type; false if not valid instance type
 */
bool SDR_ESM_getBaseAddr(const SDR_ESM_InstanceType esmInstType, uint32_t *esmBaseAddr)
{
    bool instValid = ((bool)false);

    if (esmBaseAddr != NULL)
    {
        switch(esmInstType)
        {
            case SDR_ESM_INSTANCE_MCU:
                instValid = ((bool)true);
                *esmBaseAddr = SOC_MCU_ESM_BASE;
                break;

            case SDR_ESM_INSTANCE_WKUP:
                instValid = ((bool)true);
                *esmBaseAddr = SOC_WKUP_ESM_BASE;
                break;

            case SDR_ESM_INSTANCE_MAIN:
                instValid = ((bool)true);
                *esmBaseAddr = SOC_MAIN_ESM_BASE;
                break;

            default:
                break;
        }
    }

    return (instValid);
}

/** ================================================================================
 *
 * \brief        Get the max number of ESM events supported for a given ESM instance
 *
 * \param [in]   esmInstType: ESM instance type
 * \param [out]  esmMaxNumEvents: Maximum number of ESM events supported
 *
 * \return       true: if valid instance type; false if not valid instance type
 */
bool SDR_ESM_getMaxNumEvents(const SDR_ESM_InstanceType esmInstType,
                             uint32_t *esmMaxNumEvents)
{
    bool instValid = ((bool)false);

    if (esmMaxNumEvents != NULL)
    {
        switch(esmInstType)
        {
            case SDR_ESM_INSTANCE_MCU:
                instValid = ((bool)true);
                *esmMaxNumEvents = SOC_MCU_ESM_MAX_NUM_EVENTS;
                break;

            case SDR_ESM_INSTANCE_WKUP:
                instValid = ((bool)true);
                *esmMaxNumEvents = SOC_MCU_ESM_MAX_NUM_EVENTS;
                break;

            case SDR_ESM_INSTANCE_MAIN:
                instValid = ((bool)true);
                *esmMaxNumEvents = SOC_MCU_ESM_MAX_NUM_EVENTS;
                break;

            default:
                break;
        }
    }

    return (instValid);
}

/** ================================================================================
 *
 * \brief        Check that ESM instance type or ESM base address is valid for this
 *               device, and fill the SDR_ESM instance
 *
 * \param [in]   esmInstType:     ESM instance type
 * \param [out]  pEsmInstancePtr: Pointer to location of ESM instance structure
 *
 * \return       true: if valid instance type; false if not valid instance type
 */
bool SDR_ESM_selectEsmInst(const SDR_ESM_InstanceType esmInstType,
                           SDR_ESM_Instance_t **pEsmInstancePtr)
{
    bool instValid = ((bool)true);

    switch(esmInstType)
    {
        case SDR_ESM_INSTANCE_MCU:
            *pEsmInstancePtr = (SDR_ESM_Instance_t *)(&SDR_ESM_instance_MCU);
            break;

        case SDR_ESM_INSTANCE_WKUP:
            *pEsmInstancePtr = (SDR_ESM_Instance_t *)(&SDR_ESM_instance_WKUP);
            break;

        case SDR_ESM_INSTANCE_MAIN:
            *pEsmInstancePtr = (SDR_ESM_Instance_t *)(&SDR_ESM_instance_MAIN);
            break;

        default:
            /* Invalid instance input parameter */
            instValid = ((bool)false);
            break;
    }

    return (instValid);
}

/** ================================================================================
 *
 * \brief        Check that ESM instance type or ESM base address is valid for this
 *               device, and fill the SDR_ESM instance
 *
 * \param [in]   esmInstBaseAddr: Base address for ESM instance's registers.
 *                                Function fills pointer to instance for this
 *                                set of registers
 * \param [out]  pEsmInstType:    Pointer to ESM instance type
 * \param [out]  pEsmInstancePtr: Pointer to location of ESM instance structure
 *
 * \return       true: if valid base address; false if not valid base address
 */
bool SDR_ESM_selectEsmInstFromAddr(uint32_t esmInstBaseAddr,
                                   SDR_ESM_InstanceType *pEsmInstType,
                                   SDR_ESM_Instance_t **pEsmInstancePtr)
{
    bool instValid = ((bool)false);

    switch(esmInstBaseAddr)
    {
        case SOC_MCU_ESM_BASE:
            instValid = ((bool)true);
            *pEsmInstancePtr = &SDR_ESM_instance_MCU;
            *pEsmInstType = SDR_ESM_INSTANCE_MCU;
            break;

        case SOC_WKUP_ESM_BASE:
            instValid = ((bool)true);
            *pEsmInstancePtr = &SDR_ESM_instance_WKUP;
            *pEsmInstType = SDR_ESM_INSTANCE_WKUP;
            break;

        case SOC_MAIN_ESM_BASE:
            instValid = ((bool)true);
            *pEsmInstancePtr = &SDR_ESM_instance_MAIN;
            *pEsmInstType = SDR_ESM_INSTANCE_MAIN;
            break;

        default:
            break;
    }

    return (instValid);
}

/** ============================================================================
 *
 * \brief   Handle any event that needs to be handled locally before
 *          reporting to application
 *
 * \param  pInstance: Pointer to ESM instance
 * \param  intSrc: Source interrupt number
 *
 * \return  true: if event handled; false if not handled
 */
bool SDR_ESM_handleIntSrc(const SDR_ESM_Instance_t *pInstance, uint32_t intSrc)
{
    bool handledFlag = ((bool)false);

    /* For each error bit */
    switch (intSrc) {
        case SDR_ESM_MCU_R5_CORE0_SEC_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Single bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MCU_CPU0_SEC_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_CORE0_DED_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Double bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }

            break;

        case SDR_ESM_MCU_R5_CPU_BUS_CMP_ERR:
            if ((void *)pInstance->CCMCallBackFunction != ((void *)0u)) {
            /* Call back function for CCM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_OUTPUT_COMPARE_BLOCK_INT);
                handledFlag = ((bool)true);
            }
            break;


        case SDR_ESM_MCU_R5_INACTIVITY_ERR_INT:
            if ((void *)pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM inactivity monitor error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_INACTIVITY_MONITOR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_VIM_BUS_CMP_ERR_INT:
            if ((void *)pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM VIM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_VIM_ERR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_SELFTEST_ERR_INT:
            if ((void *)pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM VIM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_SELF_TEST_ERR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_R5_CCM_STAT_ERR_INT:
            if ((void *)pInstance->CCMCallBackFunction != ((void *)0u)) {
                /* Call back function for CCM VIM compare error */
                pInstance->CCMCallBackFunction(SDR_ESM_CCM_STAT_ERR_INT);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_RTI0_WWD_INT:
            if ((void *)pInstance->WDTCallBackFunction != ((void *)0u)) {
                /* Call back function for Windowed watchdog timer 0 */
                handledFlag = pInstance->WDTCallBackFunction(SDR_ESM_TIMER_ID_0);
            }
            break;

        case SDR_ESM_MCU_RTI1_WWD_INT:
            if ((void *)pInstance->WDTCallBackFunction != ((void *)0u)) {
                /* Call back function for Windowed watchdog timer 1 */
                handledFlag = pInstance->WDTCallBackFunction(SDR_ESM_TIMER_ID_1);
            }
            break;
#ifdef SOC_J721E
        case SDR_ESM_MAIN_MSMC_ECC_AGGR0_SEC_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Single bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_SEC_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MAIN_MSMC_ECC_AGGR0_DED_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Double bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MAIN_MSMC_AGGR0_DED_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MAIN_A72_ECC_AGGR0_SEC_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Single bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_SEC_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MAIN_A72_ECC_AGGR0_DED_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Double bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MAIN_A72_AGGR0_DED_ERROR ,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_CBASS_ECC_AGGR_SEC_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Single bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MCU_CBASS_SEC_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;

        case SDR_ESM_MCU_CBASS_ECC_AGGR_DED_INT:
            if ((void *)pInstance->eccCallBackFunction != ((void *)0u)) {
                /* Call back function for Single bit ECC error */
                pInstance->eccCallBackFunction(SDR_ESM_ECC_PARAM_MCU_CBASS_DED_ERROR,
                                                    SDR_ESM_ERRORADDR_INVALID);
                handledFlag = ((bool)true);
            }
            break;
#endif
        default:
            /* No actions for other ESM Events */
            break;
    }
    return handledFlag;
}

