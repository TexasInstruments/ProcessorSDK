/*
 *  Copyright (c) Texas Instruments Incorporated 2026
 *  All rights reserved.
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
 *  \defgroup IP_FMA_DSS Register readback for dss static registers
 */

/**
 *  \ingroup  IP_FMA_DSS
 *  \defgroup IP_FMA_DSS_INTERFACE Register readback interface.
 *
 *  @{
 */

/**
 *  \file     ip_fma_dss.h
 *
 *  \brief    Register readback interface for dss registers.
 *
 */

#ifndef IP_FMA_DSS
#define IP_FMA_DSS

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ip_fma_common.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define IPFMA_DSS_VIDEO_PIPE_FIR_COEF_H0_REG_NUM            (9U)

/**
 *  \brief Dss Common M region registers.
 */
typedef struct
{
    uint32_t    dssRevision;        /**< Dss revision register. */
    uint32_t    dssCbaCfg;          /**< Dss cba cfg register.  */
} IpFma_DssCommonMRegs;

/**
 *  \brief Dss Common S region registers.
 */
typedef struct
{
    uint32_t    dssDispcIrqStatus;  /**< Dss dispc irq status register. */
    uint32_t    dssWbIrqStatus;     /**< Dss wb irq status register.    */
} IpFma_DssCommonSRegs;

/**
 *  \brief Video pipe region registers.
 */
typedef struct
{
    uint32_t    dssBufSizeStatus;                                           /**< Dss buf size status register. */
    uint32_t    dssFirCoefH0[IPFMA_DSS_VIDEO_PIPE_FIR_COEF_H0_REG_NUM];     /**< Dss fir coef h0 register.     */
} IpFma_DssVideoPipeRegs;

/**
 *  \brief Video pipe layer region registers.
 */
typedef struct
{
    uint32_t    dssBufSizeStatus;       /**< Dss buf size status register. */
    uint32_t    dssSafetyAttributes;    /**< Dss safety attributes.        */
} IpFma_DssVideoPipeLayerRegs;

/**
 *  \brief Dss overlay region registers.
 */
typedef struct
{
    uint32_t    dssConfig;		    /**< Dss config register.     */
    uint32_t    dssVirtualVp;       /**< Dss virtual vp register. */
} IpFma_DssOverlayRegs;

/**
 *  \brief Dss video port region registers.
 */
typedef struct
{
    uint32_t    dssConfig;		    /**< Dss config register.  */
    uint32_t    dssControl;    	    /**< Dss control register. */
} IpFma_DssVideoPortRegs;

/**
 *  \brief Dss write-back pipe region registers.
 */
typedef struct
{
    uint32_t    dssAttributes;    	/**< Dss attributes register. */
    uint32_t    dssPosition;        /**< Dss position register.   */
} IpFma_DssWriteBackPipeRegs;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   This function gets dss common M region registers.
 *
 * \param   out		        Structure containing dss common M region registers states
 * \param   instanceId      Id of the instance's MMR region
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 		    \ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Dss_GetCommonMRegs(IpFma_DssCommonMRegs* out, uint32_t instanceId);

/**
 * \brief   This function gets dss common S region registers.
 *
 * \param   out		        Structure containing dss common S region registers states
 * \param   instanceId      Id of the instance's MMR region
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 		    \ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Dss_GetCommonSRegs(IpFma_DssCommonSRegs* out, uint32_t instanceId);

/**
 * \brief   This function gets video pipe region registers.
 *
 * \param   out             Structure containing video pipe region registers
 * \param   instanceId      Id of the instance's MMR region
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 		    \ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Dss_GetVideoPipeRegs(IpFma_DssVideoPipeRegs* out, uint32_t instanceId);

/**
 * \brief   This function gets video pipe layer region registers.
 *
 * \param   out             Structure containing video pipe layer region registers
 * \param   instanceId      Id of the instance's MMR region
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 		    \ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Dss_GetVideoPipeLayerRegs(IpFma_DssVideoPipeLayerRegs* out, uint32_t instanceId);

/**
 * \brief   This function gets dss overlay region registers.
 *
 * \param   out             Structure containing dss overlay region registers states
 * \param   instanceId      Id of the instance's MMR region
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 		    \ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Dss_GetOverlayRegs(IpFma_DssOverlayRegs* out, uint32_t instanceId);

/**
 * \brief   This function gets dss video port region registers.
 *
 * \param   out             Structure containing dss video port region registers states
 * \param   instanceId      Id of the instance's MMR region
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 		    \ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Dss_GetVideoPortRegs(IpFma_DssVideoPortRegs* out, uint32_t instanceId);

/**
 * \brief   This function gets dss write-back pipe region registers.
 *
 * \param   out             Structure containing dss write-back pipe region registers states
 * \param   instanceId      Id of the instance's MMR region
 *
 * \retval  \ref    IPFMA_OK 		    Registers states read successfully
 * 		    \ref    IPFMA_E_PARAM       Registers states read unsuccessfully - Invalid parameter(s)
 */
IpFma_Status IpFma_Dss_GetWriteBackPipeRegs(IpFma_DssWriteBackPipeRegs* out, uint32_t instanceId);

/**
 * \brief   This function is used to compare dss common M region registers
 *
 * \param   expected    Previous status of dss registers
 * \param   actual      Current status of dss registers
 *
 * \retval  \ref        IPFMA_OK 		    Registers value has not been modified
 * 		    \ref        IPFMA_E_MISMATCH    Registers value modified
 */
IpFma_Status IpFma_Dss_CompareCommonMRegs(const IpFma_DssCommonMRegs* expected, const IpFma_DssCommonMRegs* actual);

/**
 * \brief   This function is used to compare dss common S region registers
 *
 * \param   expected    Previous status of dss registers
 * \param   actual      Current status of dss registers
 *
 * \retval  \ref        IPFMA_OK 		    Registers value has not been modified
 * 		    \ref        IPFMA_E_MISMATCH    Registers value modified
 */
IpFma_Status IpFma_Dss_CompareCommonSRegs(const IpFma_DssCommonSRegs* expected, const IpFma_DssCommonSRegs* actual);

/**
 * \brief   This function is used to compare video pipe region registers.
 *
 * \param   expected    Previous status of dss registers
 * \param   actual      Current status of dss registers
 *
 * \retval  \ref        IPFMA_OK 		    Registers value has not been modified
 * 		    \ref        IPFMA_E_MISMATCH    Registers value modified
 */
IpFma_Status IpFma_Dss_CompareVideoPipeRegs(const IpFma_DssVideoPipeRegs* expected, const IpFma_DssVideoPipeRegs* actual);

/**
 * \brief   This function is used to compare video pipe layer region registers.
 *
 * \param   expected    Previous status of dss registers
 * \param   actual      Current status of dss registers
 *
 * \retval  \ref        IPFMA_OK 		    Registers value has not been modified
 * 		    \ref        IPFMA_E_MISMATCH    Registers value modified
 */
IpFma_Status IpFma_Dss_CompareVideoPipeLayerRegs(const IpFma_DssVideoPipeLayerRegs* expected, const IpFma_DssVideoPipeLayerRegs* actual);

/**
 * \brief   This function is used to compare dss overlay region registers.
 *
 * \param   expected    Previous status of dss registers
 * \param   actual      Current status of dss registers
 *
 * \retval  \ref        IPFMA_OK 		    Registers value has not been modified
 * 		    \ref        IPFMA_E_MISMATCH    Registers value modified
 */
IpFma_Status IpFma_Dss_CompareOverlayRegs(const IpFma_DssOverlayRegs* expected, const IpFma_DssOverlayRegs* actual);

/**
 * \brief   This function is used to compare dss video port region registers.
 *
 * \param   expected    Previous status of dss registers
 * \param   actual      Current status of dss registers
 *
 * \retval  \ref        IPFMA_OK 		    Registers value has not been modified
 * 		    \ref        IPFMA_E_MISMATCH    Registers value modified
 */
IpFma_Status IpFma_Dss_CompareVideoPortRegs(const IpFma_DssVideoPortRegs* expected, const IpFma_DssVideoPortRegs* actual);

/**
 * \brief   This function is used to compare dss write-back pipe region registers.
 *
 * \param   expected    Previous status of dss registers
 * \param   actual      Current status of dss registers
 *
 * \retval  \ref        IPFMA_OK 		    Registers value has not been modified
 * 		    \ref        IPFMA_E_MISMATCH    Registers value modified
 */
IpFma_Status IpFma_Dss_CompareWriteBackPipeRegs(const IpFma_DssWriteBackPipeRegs* expected, const IpFma_DssWriteBackPipeRegs* actual);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef IP_FMA_DSS */

/** @} */
