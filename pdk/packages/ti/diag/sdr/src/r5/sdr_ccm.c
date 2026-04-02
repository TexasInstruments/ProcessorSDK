/*
 * SDR CCM
 *
 * Software Diagnostics Reference module for CPU Comparator Module
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

#include <ti/csl/arch/r5/csl_mcu_armss_ccmr5.h>

#include <sdr_common.h>
#include <sdr_ccm.h>
#include <sdr_esm.h>


#include <sdr_ccm_soc.h>

/** ---------------------------------------------------------------------------
 * \enum SDR_CCM_ErrorFlag
 * \brief Defines the values for CCM test error flag used to track self test
 * ----------------------------------------------------------------------------
 */
typedef enum {
    SDR_CCM_ERROR_FLAG_NONE=0,
    /**< Error flag no action */
    SDR_CCM_ERROR_FLAG_INPROGRESS=1,
    /**< Error flag to indicate self test in progress */
    SDR_CCM_ERROR_FLAG_TRIGGERED=2,
    /**< Error flag to indicate error triggerred */
} SDR_CCM_ErrorFlag;

/** ---------------------------------------------------------------------------
 * \brief This structure defines the elements of CCM software instance
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_CCM_instance_s
{
   volatile SDR_CCM_ErrorFlag selfTestErrorFlag;
   /**< Flag to track self test */
}  SDR_CCM_Instance_t;


/* SDR CCM Instance */
static SDR_CCM_Instance_t SDR_CCM_instance;

#define ALL_CCM_STATUS_BITS (CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_STE1_MASK \
                                            | CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_STET1_MASK \
                                            | CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_STC1_MASK \
                                            | CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_CMPE1_MASK)

/* Function prototypes  */
static void SDR_CCM_callBackFunction (SDR_ESM_CCM_IntSrc intSrc);
static SDR_Result SDR_CCM_getMonitorTypeFromIntSrc (SDR_ESM_CCM_IntSrc intSrc,
                                          SDR_CCM_MonitorType *pMonitorType);
static SDR_Result SDR_CCM_getMonitorStatusRegister(SDR_CCM_MonitorType monitorType,
                                                   CSL_McuArmssCcmR5RegId *pCCMR5RegId);
static SDR_Result SDR_CCM_getMonitorKeyRegister(SDR_CCM_MonitorType monitorType,
                                                CSL_McuArmssCcmR5RegId *pCCMR5RegId);
SDR_Result  SDR_CCM_clearAllErrors (void);
SDR_Result SDR_CCM_CheckSelfTestErrorSource(SDR_CCM_MonitorType *monitorType);

/**
 *
 * \brief   Initializes CCM module and sets all three compare modules
 *          to compare active
 *
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 *          NOTE: In case of SDR_FAIL the peripheral may be configured partially
 */
SDR_Result SDR_CCM_init (void)
{
    int32_t cslResult;
    SDR_Result retVal = SDR_PASS;
    uint32_t keyValue;

    /* Enable Cpu output compare block active */
    cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                               CSL_MCU_ARMSS_CCMR5_CCMKEYR1_REGID,
                                               (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE,
                                               ((int32_t *)0u));
    if (cslResult != CSL_PASS) {
        retVal = SDR_FAIL;
    }

    if (retVal == SDR_PASS) {

        /* Read back Key register */
        cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                              CSL_MCU_ARMSS_CCMR5_CCMKEYR1_REGID,
                                              (uint32_t *)&keyValue,
                                              ((int32_t *)0u));
        if ((cslResult != CSL_PASS) || (keyValue != (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE)) {
            retVal = SDR_FAIL;
        }
    }


    if (retVal == SDR_PASS) {

        /* Enable Vim compare active */
        cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                                   CSL_MCU_ARMSS_CCMR5_CCMKEYR2_REGID,
                                                   (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE,
                                                   ((int32_t *)0u));
        if (cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {

        /* Read back Key register */
        cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                              CSL_MCU_ARMSS_CCMR5_CCMKEYR2_REGID,
                                              (uint32_t *)&keyValue,
                                              ((int32_t *)0u));
        if ((cslResult != CSL_PASS) || (keyValue != (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE)) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {
        /* Enable Inactivity monitor compare block active */
        cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                                   CSL_MCU_ARMSS_CCMR5_CCMKEYR3_REGID,
                                                   (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE,
                                                   ((int32_t *)0u));
        if (cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {

        /* Read back Key register */
        cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                              CSL_MCU_ARMSS_CCMR5_CCMKEYR3_REGID,
                                              (uint32_t *)&keyValue,
                                              ((int32_t *)0u));
        if ((cslResult != CSL_PASS) || (keyValue != (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE)) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {

        /* Clear any pending errors */
        retVal = SDR_CCM_clearAllErrors();
        if (retVal == SDR_PASS) {

            /* Register error interrupt call back function with MCU ESM handler */
            (void)SDR_ESM_registerCCMHandler(SDR_ESM_INSTANCE_MCU,
                                             &SDR_CCM_callBackFunction);
        }

    }
    /* Clear self test flag */
    SDR_CCM_instance.selfTestErrorFlag = SDR_CCM_ERROR_FLAG_NONE;

    return retVal;
}

/**
 *
 * \brief   Clear all CCM errors
 *
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 *          NOTE: In case of SDR_FAIL error may be cleared partially
 */
SDR_Result  SDR_CCM_clearAllErrors (void)
{
    SDR_Result retVal = SDR_PASS;
    uint32_t statusValue;
    int32_t cslResult;

    /* Clear any errors  in CPU output compare*/
    cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                        CSL_MCU_ARMSS_CCMR5_CCMSR1_REGID,
                                        ALL_CCM_STATUS_BITS,
                                        ((int32_t *)0u));
    if (cslResult != CSL_PASS) {
        retVal = SDR_FAIL;
    }

    if (retVal == SDR_PASS) {
        /* Read status register of CPU output compare block  */
        cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                    CSL_MCU_ARMSS_CCMR5_CCMSR1_REGID,
                                    (uint32_t *)&statusValue,
                                    ((int32_t *)0u));

        if ((cslResult != CSL_PASS) || (statusValue != 0u)) {
            retVal = SDR_FAIL;

        }
    }

    if (retVal == SDR_PASS) {

        /* Clear any errors in VIM compare */
        cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                            CSL_MCU_ARMSS_CCMR5_CCMSR2_REGID,
                                            ALL_CCM_STATUS_BITS,
                                            ((int32_t *)0u));
        if (cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {

        /* Read status register of CPU output compare block  */
        cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                       CSL_MCU_ARMSS_CCMR5_CCMSR2_REGID,
                                       (uint32_t *)&statusValue,
                                       ((int32_t *)0u));
        if ((cslResult != CSL_PASS) || (statusValue != 0u)) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {

        /* Clear any errors in Inactivity monitor */
        cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                             CSL_MCU_ARMSS_CCMR5_CCMSR3_REGID,
                                             ALL_CCM_STATUS_BITS,
                                             ((int32_t *)0u));

        if (cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    if (retVal == SDR_PASS) {

        /* Read status register of Inactivity monitor  */
        cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                        CSL_MCU_ARMSS_CCMR5_CCMSR3_REGID,
                                        (uint32_t *)&statusValue,
                                        ((int32_t *)0u));

        if ((cslResult != CSL_PASS) || (statusValue != 0u)) {
            retVal = SDR_FAIL;
        }
    }

    return retVal;

}

/** ========================================================================
 *
 * \brief   Injects forced error on CCM
 *
 * \param   monitorType: Type of monitor to inject error
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_CCM_injectError (SDR_CCM_MonitorType monitorType)
{

    int32_t cslResult;
    SDR_Result retVal = SDR_PASS;
    CSL_McuArmssCcmR5RegId monitorTypeKeyRegister;


    /* Get the key register address corresponding to the monitor type */
    retVal = SDR_CCM_getMonitorKeyRegister(monitorType, &monitorTypeKeyRegister);
    if (retVal == SDR_PASS) {

        /* Configure error forcing mode to inject error by writing to the key register */
        cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                                   monitorTypeKeyRegister,
                                                   (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_ERR_FORCE_MODE,
                                                   ((int32_t *)0u));

        if (cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    return retVal;
}

/** ========================================================================
 *
 * \brief   Self test error can happen due to any of the two events
 *          Compare unit or Inactivity detection unit. This routine
 *          checks the status bits to find the error source.
 *
 * \param   monitorType: Returned Monitor Type pointer
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_CCM_CheckSelfTestErrorSource(SDR_CCM_MonitorType *monitorType)
{
    int32_t cslResult;
    uint32_t statusValue;
    SDR_Result retVal = SDR_PASS;

    /* Read status register of CPU output compare block  */
    cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                           CSL_MCU_ARMSS_CCMR5_CCMSR1_REGID,
                                           &statusValue,
                                           ((int32_t *)0u));
    if (cslResult != CSL_PASS) {
        retVal = SDR_FAIL;
    } else {
        /* If status bit set, return output compare block */
        if ((statusValue & ALL_CCM_STATUS_BITS) != 0u) {
            *monitorType =  SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK;
        } else {

            /* Read status register of VIM compare block  */
            cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                                   CSL_MCU_ARMSS_CCMR5_CCMSR2_REGID,
                                                   &statusValue,
                                                   ((int32_t *)0u));
            /* Check result */
            if (cslResult != CSL_PASS) {
                retVal = SDR_FAIL;
            } else {
                /* If status bit set, return VIM monitor block */
                if ((statusValue & ALL_CCM_STATUS_BITS) != 0u) {
                     *monitorType =  SDR_CCM_MONITOR_TYPE_VIM;
                 } else {

                     /* Read status register of Inactivity monitor block  */
                     cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                                            CSL_MCU_ARMSS_CCMR5_CCMSR3_REGID,
                                                            &statusValue,
                                                            ((int32_t *)0u));
                     if (cslResult != CSL_PASS) {
                         retVal = SDR_FAIL;
                     } else {
                         /* If status bit set, return Inactivity monitor block */
                         if ((statusValue & ALL_CCM_STATUS_BITS) != 0u) {
                              *monitorType =  SDR_CCM_MONITOR_TYPE_INACTIVITY_MONITOR;
                          } else {
                              retVal = SDR_FAIL;
                          }
                     }
                 }
            }
        }
    }

    return retVal;
}

/** ========================================================================
 *
 * \brief   Execute Self test on CCM by inverting the polarity
 *
 * \param   polarityInversionMask: Mask used to invert polarity of
 *            selected signals. ( Meaning of signals refer to hardware
 *            manuals
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_CCM_selfTestPolarityInvert (uint32_t polarityInversionMask,
                             uint32_t maxCheckCount)
{
    int32_t cslResult;
    SDR_Result retVal = SDR_PASS;
    uint32_t checkCount;

    /* Initialise Error flag to be starting */
    SDR_CCM_instance.selfTestErrorFlag = SDR_CCM_ERROR_FLAG_INPROGRESS;

    /* Write polarity inversion register to force polarity flip
     * Which should cause a compare error
     */
    cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                               CSL_MCU_ARMSS_CCMR5_POLCNTRL_REGID,
                                               polarityInversionMask,
                                               ((int32_t *)0u));
    if ( cslResult != CSL_PASS) {
        retVal = SDR_FAIL;
    } else {
        /* Initialize check count */
        checkCount = ((uint32_t)0u);
        /* Wait for Error flag to be triggerred in the interrupt call back function */
        while((SDR_CCM_instance.selfTestErrorFlag != SDR_CCM_ERROR_FLAG_TRIGGERED)
              && ((checkCount < maxCheckCount) || (maxCheckCount == 0u))) {
             /* Increment timeout counter */
             checkCount++;
       }
    }

    /* if error flag is not set, then it is a timeout */
    if(SDR_CCM_instance.selfTestErrorFlag != SDR_CCM_ERROR_FLAG_TRIGGERED) {
        retVal = SDR_FAIL;
    }

    /* Reset error flag to be none */
    SDR_CCM_instance.selfTestErrorFlag = SDR_CCM_ERROR_FLAG_NONE;

    return retVal;
}

/** ========================================================================
 *
 * \brief   Execute Self test on CCM
 *          This initiates error injection and checks for the expected
 *          response event.
 *
 * \param   monitorType: Monitor Type
 * \param   selfTestType: Self test type
 * \param   maxCheckCount: max count to check for self test
 *                         completion ( 0 means indefinite)
 * @param   pErrorStatus: Pointer to Error status structure in case of error
 *                        or NULL if error status not required.
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
SDR_Result SDR_CCM_selfTest (SDR_CCM_MonitorType monitorType,
                             SDR_CCM_SelfTestType selfTestType,
                             uint32_t maxCheckCount,
                             SDR_CCM_ErrorStatus_t *pErrorStatus)
{
    int32_t cslResult;
    SDR_Result retVal = SDR_PASS;
    CSL_McuArmssCcmR5OpModeKey selfTestTypeValue;
    uint32_t statusValue;
    uint32_t timesCount=0;
    CSL_McuArmssCcmR5RegId monitorTypeKeyRegister;
    CSL_McuArmssCcmR5RegId monitorTypeStatusRegister;
    bool compareErrorFlag, selfTestErrorFlag, selfTestErrorTypeFlag;


    /* Get the Key register for the monitor type */
    retVal = SDR_CCM_getMonitorKeyRegister(monitorType, &monitorTypeKeyRegister);
    if ( retVal == SDR_PASS ) {

        switch(selfTestType) {

            case SDR_CCM_SELFTEST_TYPE_NORMAL:
                selfTestTypeValue = CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_MODE;
                break;

            case SDR_CCM_SELFTEST_TYPE_ERR_FORCING:
                selfTestTypeValue = CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_ERR_FORCE_MODE;
                break;

            default:
                retVal = SDR_FAIL;
                break;
        }
    }

    /* Check for error in earlier steps */
    if (retVal == SDR_PASS) {
        /* Get the status register for the monitor type */
        retVal = SDR_CCM_getMonitorStatusRegister(monitorType, &monitorTypeStatusRegister);
    }

    if (retVal == SDR_PASS) {
        /* Clear Status register */
        cslResult = CSL_armR5ConfigureCCMRegister (SOC_CCM_BASE,
                                                   monitorTypeStatusRegister,
                                                   ALL_CCM_STATUS_BITS,
                                                   ((int32_t *)0u));

        if ( cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    /* Check for error in earlier steps */
    if (retVal == SDR_PASS) {

        /* Initialise Error flag to be starting */
        SDR_CCM_instance.selfTestErrorFlag = SDR_CCM_ERROR_FLAG_INPROGRESS;

        /* Configure mode to initiate self test */
        cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                                   monitorTypeKeyRegister,
                                                   (uint32_t)selfTestTypeValue,
                                                   ((int32_t *)0u));

        if ( cslResult != CSL_PASS) {
            retVal = SDR_FAIL;
        }
    }

    /* Check for error in earlier steps */
    if (retVal == SDR_PASS) {
        /* Wait for error to take effect */
        while((maxCheckCount == 0u) || (timesCount < maxCheckCount)) {
            /* Check if the self test completed */
            cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                                   monitorTypeStatusRegister,
                                                   &statusValue,
                                                   ((int32_t *)0u));

            if (cslResult != CSL_PASS) {
                retVal = SDR_FAIL;
            }

            /* Check if the status bit is set to indicate completion of self test */
            if(((statusValue & ALL_CCM_STATUS_BITS) != 0u)
               || (SDR_CCM_instance.selfTestErrorFlag == SDR_CCM_ERROR_FLAG_TRIGGERED)
               || (retVal != SDR_PASS)) {
                break;
            }
            /* Increment timeout counter */
            timesCount++;

        }
    }

    if (retVal == SDR_PASS) {
        /* Switch it back to active mode */
        cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                                  monitorTypeKeyRegister,
                                                  (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE,
                                                  ((int32_t *)0u));

         if (cslResult != CSL_PASS) {
             retVal = SDR_FAIL;
         }

     }

    if (retVal == SDR_PASS) {
        /* Use the earlier recorded status value to check each error flag */
        compareErrorFlag = ((statusValue
                             & CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_CMPE1_MASK)
                            != 0u) ? ((bool)true): ((bool)false);
        selfTestErrorFlag = ((statusValue
                              & CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_STE1_MASK)
                             != 0u) ? ((bool)true): ((bool)false);
        selfTestErrorTypeFlag = ((statusValue
                                  & CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_STET1_MASK)
                                 != 0u) ? ((bool)true): ((bool)false);
        /* Check expected error occurred or timeout */
        if (timesCount >= maxCheckCount) {
            if (pErrorStatus != ((void *)0u)) {
                pErrorStatus->compareErrorFlag = compareErrorFlag;
                pErrorStatus->selfTestErrorFlag = selfTestErrorFlag;
                pErrorStatus->selfTestErrorTypeFlag = selfTestErrorTypeFlag;
            }

            if (statusValue != 0u) {
                /* Clear status register */
                (void) CSL_armR5ConfigureCCMRegister (SOC_CCM_BASE,
                                                      monitorTypeStatusRegister,
                                                      statusValue,
                                                      ((int32_t *)0u));
            }

            retVal = SDR_FAIL;
        }
    }

    /* Reset error flag to be none */
    SDR_CCM_instance.selfTestErrorFlag = SDR_CCM_ERROR_FLAG_NONE;

    return retVal;
}

/** ========================================================================
 *
 * \brief   CCM call back function registered with ESM handler
 *
 * \param  intSrc: Interrupt source
 *
 * \return  None
 */
static void SDR_CCM_callBackFunction (SDR_ESM_CCM_IntSrc intSrc)
{
    int32_t cslResult;
    uint32_t status;
    uint32_t polarityRegValue, keyRegValue;
    CSL_McuArmssCcmR5RegId monitorTypeStatusRegister = CSL_MCU_ARMSS_CCMR5_CCMSR1_REGID;
    CSL_McuArmssCcmR5RegId monitorTypeKeyRegister;
    SDR_CCM_MonitorType monitorType;
    SDR_CCM_ErrorStatus_t errorStatus;
    SDR_Result retVal;
    bool moduleIndependentEvent = (bool)false;

    /* Check if it is self test related interrupt */
    switch (intSrc) {
        case SDR_ESM_CCM_SELF_TEST_ERR_INT:
        case SDR_ESM_CCM_STAT_ERR_INT:
            /* These events can come for any of the CCM block. Read status register to see which self test */
            retVal = SDR_CCM_CheckSelfTestErrorSource(&monitorType);
            if ( retVal != SDR_PASS) {
                moduleIndependentEvent = (bool)true;
            }
            break;

        default:
            /* Get the monitor type based on interrupt source */
            retVal = SDR_CCM_getMonitorTypeFromIntSrc(intSrc, &monitorType);
            if (retVal != SDR_PASS ) {
                SDR_assertExternalFunction(SDR_CCM_INVALID_INTERRUPT_SOURCE);
            }
            break;
    }

    /* Read polarity convert register */
    /* Read status register 1 */
    cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                           CSL_MCU_ARMSS_CCMR5_POLCNTRL_REGID,
                                           &polarityRegValue,
                                           ((int32_t *)0u));
    if ( cslResult != CSL_PASS) {
        SDR_assertExternalFunction(SDR_CCM_READ_REG_FAILURE);
    } else {
        if(polarityRegValue != 0u) {
            /* If polarity reverted; switch back to 0 */
            cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                                       CSL_MCU_ARMSS_CCMR5_POLCNTRL_REGID,
                                                       0u,
                                                       ((int32_t *)0u));
            if ( cslResult != CSL_PASS) {
                SDR_assertExternalFunction(SDR_CCM_CONFIG_REG_FAILURE);
            }
            if (SDR_CCM_instance.selfTestErrorFlag == SDR_CCM_ERROR_FLAG_INPROGRESS ) {
                SDR_CCM_instance.selfTestErrorFlag = SDR_CCM_ERROR_FLAG_TRIGGERED;
            }
        }
        if (moduleIndependentEvent) {
            if (SDR_CCM_instance.selfTestErrorFlag == SDR_CCM_ERROR_FLAG_INPROGRESS ) {
                SDR_CCM_instance.selfTestErrorFlag = SDR_CCM_ERROR_FLAG_TRIGGERED;
            }
        } else {
            /* Get the status register and monitor type for the interrupt source */
            retVal = SDR_CCM_getMonitorStatusRegister(monitorType, &monitorTypeStatusRegister);

            if (retVal != SDR_PASS) {
                /* Unexpected result call assert */
                SDR_assertExternalFunction(SDR_CCM_READ_REG_FAILURE);
            }
        }
    }

    if (retVal == SDR_PASS) {
        /* Read status register 1 */
        cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                               monitorTypeStatusRegister,
                                               &status,
                                               ((int32_t *)0u));

        if (cslResult != CSL_PASS) {
            /* Unexpected result call assert */
            SDR_assertExternalFunction(SDR_CCM_READ_REG_FAILURE);
        }

        /* Get the Key register corresponding to the monitor type */
        retVal = SDR_CCM_getMonitorKeyRegister(monitorType, &monitorTypeKeyRegister);
    }

    if (retVal == SDR_PASS) {
        /* Read polarity convert register */
         /* Read status register 1 */
         cslResult = CSL_armR5ReadCCMRegister (SOC_CCM_BASE,
                                                monitorTypeKeyRegister,
                                                &keyRegValue,
                                                ((int32_t *)0u));
         if (cslResult != CSL_PASS) {
             /* Unexpected result call assert */
             SDR_assertExternalFunction(SDR_CCM_READ_REG_FAILURE);
         }
    }

    if (retVal == SDR_PASS) {
        if (keyRegValue == (uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_SELF_TEST_MODE) {
            /* Switch it back to active mode */
            cslResult = CSL_armR5ConfigureCCMRegister(SOC_CCM_BASE,
                                                       monitorTypeKeyRegister,
                                                       ((uint32_t)CSL_MCU_ARMSS_CCMR5_MKEY_CMP_MODE_ACTIVE),
                                                       ((int32_t *)0u));

            if (cslResult != CSL_PASS) {
                 SDR_assertExternalFunction(SDR_CCM_CONFIG_REG_FAILURE);
             }
        }

        /* Get compare error flag */
        errorStatus.compareErrorFlag = ((status
                                          & CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_CMPE1_MASK)
                                         != 0u) ? ((bool)true): ((bool)false);
        errorStatus.selfTestErrorFlag = ((status
                                          & CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_STE1_MASK)
                                         != 0u) ? ((bool)true): ((bool)false);
        errorStatus.selfTestErrorTypeFlag = ((status
                                          & CSL_MCU_ARMSS_CCMR5_COMPARE_WRAPPER_CFG_MMRS_CCMSR1_STET1_MASK)
                                         != 0u) ? ((bool)true): ((bool)false);

        /* Clear status register */
        cslResult = CSL_armR5ConfigureCCMRegister (SOC_CCM_BASE,
                                                   monitorTypeStatusRegister,
                                                   status,
                                                   ((int32_t *)0u));
        if (cslResult != CSL_PASS) {
             SDR_assertExternalFunction(SDR_CCM_CONFIG_REG_FAILURE);
        } else {
            if (SDR_CCM_instance.selfTestErrorFlag == SDR_CCM_ERROR_FLAG_NONE ) {
                /* Execute application call back */
                SDR_CCM_applicationCallbackFunction(monitorType, &errorStatus);
            }
        }
    }
}

/** ========================================================================
 * \fn      SDR_CCM_getMonitorTypeFromIntSrc
 *
 * \brief   Get Monitor type id from Interrupt source
 *
 * \param  intSrc: Interrupt source
 * \param   pCCMR5RegId: Pointer to Monitor type corresponding to the
 *                     interrupt source
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_CCM_getMonitorTypeFromIntSrc (SDR_ESM_CCM_IntSrc intSrc,
                                          SDR_CCM_MonitorType *pMonitorType)
{
    SDR_Result result = SDR_PASS;

    /* Set default value */
    *pMonitorType = SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK;

    /* This finds the monitor type based on the interrupt source */
    switch(intSrc) {
        case SDR_ESM_CCM_OUTPUT_COMPARE_BLOCK_INT:
            *pMonitorType = SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK;
            break;

        case SDR_ESM_CCM_VIM_ERR_INT:
            *pMonitorType = SDR_CCM_MONITOR_TYPE_VIM;
            break;

        case SDR_ESM_CCM_INACTIVITY_MONITOR_INT:
            *pMonitorType = SDR_CCM_MONITOR_TYPE_INACTIVITY_MONITOR;
            break;

        default:
            /* Invalid value return fail */
            result = SDR_FAIL;
            break;
    }
    return result;
}

/** ========================================================================
 * \fn      SDR_CCM_getMonitorKeyRegister
 *
 * \brief   Get Key Register Id for the given monitor type
 *
 * \param monitorType: Monitor type
 * \param pCCMR5RegId: Pointer to Key Register Id corresponding to
 *                     the monitor type
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_CCM_getMonitorKeyRegister(SDR_CCM_MonitorType monitorType,
                                                CSL_McuArmssCcmR5RegId *pCCMR5RegId)
{
    SDR_Result result = SDR_PASS;

    /* Set default value */
    *pCCMR5RegId = CSL_MCU_ARMSS_CCMR5_CCMKEYR1_REGID;

    /* This gets the key register based on monitorType*/
    switch(monitorType) {
        case SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK:
            *pCCMR5RegId = CSL_MCU_ARMSS_CCMR5_CCMKEYR1_REGID;
            break;

        case SDR_CCM_MONITOR_TYPE_VIM:
            *pCCMR5RegId = CSL_MCU_ARMSS_CCMR5_CCMKEYR2_REGID;
            break;

        case SDR_CCM_MONITOR_TYPE_INACTIVITY_MONITOR:
            *pCCMR5RegId = CSL_MCU_ARMSS_CCMR5_CCMKEYR3_REGID;
            break;

        default:
            /* Invalid value return fail */
            result = SDR_FAIL;
            break;
    }
    return result;
}

/** ========================================================================
 * \fn      SDR_CCM_getMonitorStatusRegister
 *
 * \brief   Get Status Register Id for the given monitor type
 *
 * \param monitorType: Monitor type
 * \param pCCMR5RegId: Pointer to Status Register Id corresponding to
 *                     the monitor type
 *
 * \return  SDR_PASS : Success; SDR_FAIL for failures
 */
static SDR_Result SDR_CCM_getMonitorStatusRegister(SDR_CCM_MonitorType monitorType,
                                                   CSL_McuArmssCcmR5RegId *pCCMR5RegId)
{
    SDR_Result result = SDR_PASS;

    /* Get the monitor type register based on monitor type */
    switch(monitorType) {
        case SDR_CCM_MONITOR_TYPE_OUTPUT_COMPARE_BLOCK:
            *pCCMR5RegId = CSL_MCU_ARMSS_CCMR5_CCMSR1_REGID;
            break;

        case SDR_CCM_MONITOR_TYPE_VIM:
            *pCCMR5RegId = CSL_MCU_ARMSS_CCMR5_CCMSR2_REGID;
            break;

        case SDR_CCM_MONITOR_TYPE_INACTIVITY_MONITOR:
            *pCCMR5RegId = CSL_MCU_ARMSS_CCMR5_CCMSR3_REGID;
            break;

        default:
            /* Invalid value return fail */
            result = SDR_FAIL;
            break;
    }
    return result;
}
