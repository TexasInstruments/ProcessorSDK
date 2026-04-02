/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2019
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
 *  \file  pmlib_vtm_vd.c
 *
 *  \brief This file contains the interface definition for the VTM VD.
 *         API provided
 *         - To read the voltage value in a given OPP and VD.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stddef.h>
#include <ti/csl/soc.h>
#include <ti/csl/cslr.h>
#include <ti/drv/pm/include/dmsc/pmlib_vtm_vd.h>
#include <ti/csl/cslr_vtm.h>
#include "pm_types.h"
#include "pm_utils.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define PMLIB_VTM_VD_MIN_VID                              (uint32_t)(0x1EU)
#define PMLIB_VTM_VD_MAX_VID                              (uint32_t)(0x91U)
#define PMLIB_VTM_VD_MIN_VAL                              (uint32_t)(600U)
#define PMLIB_VTM_VD_MAX_VAL                              (uint32_t)(1250U)
#define PMLIB_VTM_VD_CONV_FACT                            (uint32_t)(5U)

#define NUM_OPP                                           (uint32_t)(4U)
#define NUM_VOLTAGE_DOMAIN                                (uint32_t)(8U)

#if defined(SOC_AM65XX)
#define VTM_BASE                                          CSL_WKUP_VTM0_CFG0_BASE
#elif defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#define VTM_BASE                                          CSL_WKUP_VTM0_MMR_VBUSP_CFG1_BASE
#endif
/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief This API is used to convert a given VID byte into voltage value in mV
 *
 * \param  VID_byte VID byte read from the oppVidReg.
 *
 * \return voltageInMillivolt Voltage in milli degrees.
 */
static int32_t PmlibVID2mV(uint32_t VID_byte);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t PMLIBReadVoltageVD(uint32_t voltageDomain, uint32_t opp, int32_t *val)
{
    int32_t retVal;
    uint32_t vid = 0;
    int32_t temp = 0;

    if((voltageDomain >= NUM_VOLTAGE_DOMAIN) || (opp >= NUM_OPP))
    {
        retVal = (int32_t)PM_BADARGS;
    }
    else
    {
        CSL_vtm_cfg1Regs * vtmCfgRegs =
            (CSL_vtm_cfg1Regs *)VTM_BASE;

        CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(vtmCfgRegs->VD[voltageDomain]);

        switch (opp)
        {
            case 0:
                vid = CSL_FEXT(vtmVDRegs->OPPVID, VTM_CFG1_OPPVID_OPP_LOW_DFLT);
                temp = PmlibVID2mV(vid);
                break;
            case 1:
                vid = CSL_FEXT(vtmVDRegs->OPPVID, VTM_CFG1_OPPVID_OPP_NOM_DFLT);
                temp = PmlibVID2mV(vid);
                break;
            case 2:
                vid = CSL_FEXT(vtmVDRegs->OPPVID, VTM_CFG1_OPPVID_OPP_ODR_DFLT);
                temp = PmlibVID2mV(vid);
                break;
            case 3:
                vid = CSL_FEXT(vtmVDRegs->OPPVID, VTM_CFG1_OPPVID_OPP_TRB_DFLT);
                temp = PmlibVID2mV(vid);
                break;
            default:
                retVal = (int32_t)PM_BADARGS;
                temp = -1;
                break;
        }
        if(temp > 0)
        {
            *val = temp;
            retVal = PM_SUCCESS;
        }
        /* if the efuse is not programmed, the vid value is 0, nominal voltage can be used */
        else if(temp == PM_VOLTAGE_INVALID)
        {
            retVal = temp;
        }
        else
        {
            retVal = PM_FAIL;
        }
    }
    return retVal;
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------- */
static int32_t PmlibVID2mV(uint32_t VID_byte)
{
    int32_t voltageInMillivolt = 0;

    if((VID_byte > PMLIB_VTM_VD_MAX_VID) || (VID_byte < PMLIB_VTM_VD_MIN_VID))
    {
        /* if the efuse is not programmed, the vid value is 0 */
        if(VID_byte == 0)
        {
            voltageInMillivolt = (int32_t)PM_VOLTAGE_INVALID;
        }
        else
        {
            voltageInMillivolt = (int32_t)PM_BADARGS;
        }
    }
    else
    {
        uint32_t tempVal   = PMLIB_VTM_VD_MIN_VAL + (PMLIB_VTM_VD_CONV_FACT*(VID_byte - PMLIB_VTM_VD_MIN_VID));
        voltageInMillivolt = (int32_t) (tempVal);
    }

    return voltageInMillivolt;
}


