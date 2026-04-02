/*
 *   @file  canfd_xwr18xx.c
 *
 *   @brief
 *      XWR18xx CANFD controller configuration. This is XWR18xx specific
 *      configuration and should *NOT* be modified by the customer.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/osal/DebugP.h>
#include <ti/drv/canfd/src/canfd_internal.h>
#include <ti/drv/canfd/soc/canfd_soc.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/


/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   Platform specific CANFD initializations. This is TPR12
 * specific configuration and should *NOT* be modified by the customer.
 */
CANFD_HwCfg gCanFdHwCfg[CANFD_MAX_NUM_INSTANCE] =
{
    /* MCAN Hardware configuration:
     * - instance Id 0
     * - CANFD register Base Address
     * - CANFD Interrupt 0
     * - CANFD Interrupt 1
     */
    {
        0U,
        CSL_MSS_MCANA_MSG_RAM_U_BASE,
        CSL_MSS_INTR_MSS_MCANA_INT0,
        CSL_MSS_INTR_MSS_MCANA_INT1
    },
    /* Only 1 instance of CANFD is supported.
     * Mark this entry invalid.
     */
    {
        1U,
        CSL_MSS_MCANB_MSG_RAM_U_BASE,
        CSL_MSS_INTR_MSS_MCANB_INT0,
        CSL_MSS_INTR_MSS_MCANB_INT1
    }
};

static CANFD_DriverMCB gCanfdMCB[CANFD_MAX_NUM_INSTANCE];

/**
 * \brief  This API gets the CANFD index  matching the CANFD instance id
 *
 * \param  instanceId [in] MIBSPI instance id
 * \param  idx[out]  MIBSPI instance index.
 *
 * \return           MIBSPI_SUCCESS success: MIBSPI_STATUS_ERROR : error
 *
 */
static int32_t CANFD_socGetInstIndex(uint8_t instanceId, uint32_t *idx)
{
    int32_t status = CANFD_EOK;
    uint32_t i;

    for (i = 0; i< CANFD_UTILS_ARRAYSIZE(gCanFdHwCfg); i++)
    {
        if (instanceId == gCanFdHwCfg[i].instanceId)
        {
            break;
        }
    }
    if (i < CANFD_UTILS_ARRAYSIZE(gCanFdHwCfg))
    {
        *idx   = i;
        status = CANFD_EOK;
    }
    else
    {
        *idx   = ~0U;
        status = CANFD_EINVAL;
    }
    return status;
}

CANFD_DriverMCB *CANFD_socGetInstHandle(uint8_t instanceId)
{
    CANFD_DriverMCB *hCanfd;
    uint32_t idx;
    int32_t status;

    status = CANFD_socGetInstIndex(instanceId, &idx);
    if (status == CANFD_EOK)
    {
        DebugP_assert(idx < CANFD_UTILS_ARRAYSIZE(gCanFdHwCfg));
        hCanfd = &gCanfdMCB[idx];
    
    }
    else
    {
        hCanfd = NULL;
    }
    return hCanfd;
}

/**
 * \brief  This API gets the SoC level of SPI intial configuration
 *
 * \param  idx       SPI instance index.
 * \param  cfg       Pointer to SPI SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
const CANFD_HwCfg * CANFD_socGetHwCfg(uint8_t instanceId)
{
    int32_t status = 0;
    uint32_t index;
    const CANFD_HwCfg *cfg;

    status = CANFD_socGetInstIndex(instanceId, &index);
    if (status == CANFD_EOK)
    {
        DebugP_assert(index < CANFD_UTILS_ARRAYSIZE(gCanFdHwCfg));
        cfg = &gCanFdHwCfg[index];
    }
    else
    {
        cfg = NULL;
    }
    return cfg;
}

