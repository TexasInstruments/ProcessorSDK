/*
 * SDR ESM
 *
 * Software Diagnostics Reference module for ESM
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

#ifndef INCLUDE_SDR_ESM_PRIV_H_
#define INCLUDE_SDR_ESM_PRIV_H_

#include <stddef.h>
#include <stdbool.h>

#include <sdr_esm.h>

/** ---------------------------------------------------------------------------
 * @brief This structure defines the elements of ESM software instance
 * ----------------------------------------------------------------------------
 */
typedef struct SDR_ESM_Instance_s
{
    SDR_ESM_InitConfig_t esmInitConfig;
    /**< Store esm Init configuration */
    SDR_ESM_ECCCallback_t eccCallBackFunction;
    /**< Store ECC callback function */
    SDR_ESM_CCMCallback_t CCMCallBackFunction;
    /**< Store CCM callback function */
    SDR_ESM_WDTCallback_t WDTCallBackFunction;
    /**< Store WDT callback function */
    volatile bool selfTestFlag;
    /**< selfTest Flag */
}  SDR_ESM_Instance_t;

/** ============================================================================
 *
 * \brief   Handle any event that needs to be handled locally before
 *          reporting to application
 *
 * \param   pInstance: Pointer to ESM instance
 * \param   intSrc: Source interrupt number
 *
 * \return  true: if event handled; false if not handled
 */
bool SDR_ESM_handleIntSrc(const SDR_ESM_Instance_t *pInstance, uint32_t intSrc);

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
bool SDR_ESM_getBaseAddr(const SDR_ESM_InstanceType esmInstType, uint32_t *esmBaseAddr);

/** ================================================================================
 *
 * \brief        Check that ESM instance type is valid for this device, and fill the
 *               SDR_ESM instance
 *
 * \param [in]   esmInstType: ESM instance type
 * \param [out]  pEsmInstancePtr: Pointer to location of ESM instance structure
 *
 * \return       true: if valid instance type; false if not valid instance type
 */
bool SDR_ESM_selectEsmInst(const SDR_ESM_InstanceType esmInstType,
                           SDR_ESM_Instance_t **pEsmInstancePtr);

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
                                   SDR_ESM_Instance_t **pEsmInstancePtr);

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
                             uint32_t *esmMaxNumEvents);

#endif /* INCLUDE_SDR_ESM_PRIV_H_ */
