/**********************************************************************
* Copyright (C) 2012-2024 Cadence Design Systems, Inc.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
**********************************************************************
* WARNING: This file is auto-generated using api-generator utility.
*          api-generator: 12.03.9e11b77(origin/DRV-3827_extract_sanity_to_c_file)
*          Do not edit it manually.
**********************************************************************
* Cadence Core Driver for the Cadence DP_SD0801 PHY for DisplayPort (DP)
* core. This header file lists the API providing a HAL (hardware
* abstraction layer) interface for the DP_SD0801 "Torrent" core.
**********************************************************************/

/* parasoft-begin-suppress METRICS-18-3 "Follow the Cyclomatic Complexity limit of 10" */
/* parasoft-begin-suppress METRICS-36-3 "A function should not be called from more than 5 different functions" */
/* parasoft-begin-suppress METRICS-39-3 "The value of VOCF metric for a function should not be higher than 4" */
/* parasoft-begin-suppress METRICS-41-3 "Number of blocks of comments per statement" */
/* parasoft-begin-suppress MISRA2012-RULE-8_7 "Functions and objects should not be defined with external linkage if they are referenced in only one translation unit" */

/**
 * This file contains sanity API functions. The purpose of sanity functions
 * is to check input parameters validity. They take the same parameters as
 * original API functions and return 0 on success or CDN_EINVAL on wrong parameter
 * value(s).
 */

#include "cdn_errno.h"
#include "cdn_stdtypes.h"
#include "dp_sd0801_priv.h"
#include "dp_sd0801_sanity.h"

/**
 * Function to validate struct VoltageCoefficients
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
uint32_t DP_SD0801_VoltageCoefficientSF(const DP_SD0801_VoltageCoefficients *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}

/**
 * Function to validate struct LinkState
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
uint32_t DP_SD0801_LinkStateSF(const DP_SD0801_LinkState *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        if (
            (obj->linkRate != DP_SD0801_LINK_RATE_1_62) &&
            (obj->linkRate != DP_SD0801_LINK_RATE_2_16) &&
            (obj->linkRate != DP_SD0801_LINK_RATE_2_43) &&
            (obj->linkRate != DP_SD0801_LINK_RATE_2_70) &&
            (obj->linkRate != DP_SD0801_LINK_RATE_3_24) &&
            (obj->linkRate != DP_SD0801_LINK_RATE_4_32) &&
            (obj->linkRate != DP_SD0801_LINK_RATE_5_40) &&
            (obj->linkRate != DP_SD0801_LINK_RATE_8_10)
            )
        {
            ret = CDN_EINVAL;
        }
    }

    return ret;
}

/**
 * Function to validate struct MlPhyInstance
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
uint32_t DP_SD0801_MlPhyInstanceSF(const DP_SD0801_MlPhyInstance *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        if (obj->mLane > (DP_SD0801_MAX_LANE_COUNT - 1U))
        {
            ret = CDN_EINVAL;
        }
        if (
            (obj->phyType != DP_SD0801_PHY_TYPE_NONE) &&
            (obj->phyType != DP_SD0801_PHY_TYPE_DP) &&
            (obj->phyType != DP_SD0801_PHY_TYPE_PCIE) &&
            (obj->phyType != DP_SD0801_PHY_TYPE_USB)
            )
        {
            ret = CDN_EINVAL;
        }
        if (
            (obj->ssc != DP_SD0801_NO_SSC) &&
            (obj->ssc != DP_SD0801_EXTERNAL_SSC) &&
            (obj->ssc != DP_SD0801_INTERNAL_SSC) &&
            (obj->ssc != DP_SD0801_ANY_SSC)
            )
        {
            ret = CDN_EINVAL;
        }
    }

    return ret;
}

/**
 * Function to validate struct Config
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
uint32_t DP_SD0801_ConfigSF(const DP_SD0801_Config *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}

/**
 * Function to validate struct Callbacks
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
uint32_t DP_SD0801_CallbacksSF(const DP_SD0801_Callbacks *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}

/**
 * Function to validate struct PrivateData
 *
 * @param[in] obj pointer to struct to be verified
 * @returns 0 for valid
 * @returns CDN_EINVAL for invalid
 */
uint32_t DP_SD0801_PrivateDataSF(const DP_SD0801_PrivateData *obj)
{
    uint32_t ret = 0;

    if (obj == NULL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        if (DP_SD0801_LinkStateSF(&obj->linkState) == CDN_EINVAL)
        {
            ret = CDN_EINVAL;
        }
        if (
            (obj->refClk != DP_SD0801_CLK_19_2_MHZ) &&
            (obj->refClk != DP_SD0801_CLK_20_MHZ) &&
            (obj->refClk != DP_SD0801_CLK_24_MHZ) &&
            (obj->refClk != DP_SD0801_CLK_25_MHZ) &&
            (obj->refClk != DP_SD0801_CLK_26_MHZ) &&
            (obj->refClk != DP_SD0801_CLK_27_MHZ) &&
            (obj->refClk != DP_SD0801_CLK_100_MHZ) &&
            (obj->refClk != DP_SD0801_CLK_ANY)
            )
        {
            ret = CDN_EINVAL;
        }
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] config Driver/hardware configuration.
 * @param[out] memReq Size of memory, that needs to be allocated (in bytes).
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction1(const DP_SD0801_Config* config, const uint32_t* memReq)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (memReq == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_ConfigSF(config) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in,out] pD Driver state info specific to this instance.
 * @param[in] config Specifies driver/hardware configuration.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction2(const DP_SD0801_PrivateData* pD, const DP_SD0801_Config* config)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (pD == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_ConfigSF(config) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction3(const DP_SD0801_PrivateData* pD)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @param[in] mLane Master lane number of a link.
 * @param[in] laneCount Number of lanes to initialize PHY with.
 * @param[in] linkRate Link rate to initialize PHY with.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction4(const DP_SD0801_PrivateData* pD, const uint8_t mLane, const uint8_t laneCount, const DP_SD0801_LinkRate linkRate)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (mLane > (DP_SD0801_MAX_LANE_COUNT - 1U))
    {
        ret = CDN_EINVAL;
    }
    else if ((laneCount < (1U)) || (laneCount > (DP_SD0801_MAX_LANE_COUNT)))
    {
        ret = CDN_EINVAL;
    }
    else if (
        (linkRate != DP_SD0801_LINK_RATE_1_62) &&
        (linkRate != DP_SD0801_LINK_RATE_2_16) &&
        (linkRate != DP_SD0801_LINK_RATE_2_43) &&
        (linkRate != DP_SD0801_LINK_RATE_2_70) &&
        (linkRate != DP_SD0801_LINK_RATE_3_24) &&
        (linkRate != DP_SD0801_LINK_RATE_4_32) &&
        (linkRate != DP_SD0801_LINK_RATE_5_40) &&
        (linkRate != DP_SD0801_LINK_RATE_8_10)
        )
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @param[in] linkState Pointer to structure with link state, containing lane settings to use.
 *
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction9(const DP_SD0801_PrivateData* pD, const DP_SD0801_LinkState* linkState)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_LinkStateSF(linkState) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @param[in] voltageSwing Voltage swing level to use to select register values.
 * @param[in] preEmphasis Pre-emphasis level to use to select register values.
 * @param[out] coefficients Pointer to struct to be filled with default values of coefficients.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction12(const DP_SD0801_PrivateData* pD, const uint8_t voltageSwing, const uint8_t preEmphasis, const DP_SD0801_VoltageCoefficients* coefficients)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (coefficients == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (voltageSwing > ((DP_SD0801_SWING_LEVEL_COUNT - 1U)))
    {
        ret = CDN_EINVAL;
    }
    else if (preEmphasis > ((DP_SD0801_EMPHASIS_LEVEL_COUNT - 1U)))
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @param[in] voltageSwing Voltage swing level to use to select register values.
 * @param[in] preEmphasis Pre-emphasis level to use to select register values.
 * @param[in] coefficients Pointer to struct containing values of coefficients to use.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction14(const DP_SD0801_PrivateData* pD, const uint8_t voltageSwing, const uint8_t preEmphasis, const DP_SD0801_VoltageCoefficients* coefficients)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (voltageSwing > ((DP_SD0801_SWING_LEVEL_COUNT - 1U)))
    {
        ret = CDN_EINVAL;
    }
    else if (preEmphasis > ((DP_SD0801_EMPHASIS_LEVEL_COUNT - 1U)))
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_VoltageCoefficientSF(coefficients) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @param[out] linkState Link State.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction15(const DP_SD0801_PrivateData* pD, const DP_SD0801_LinkState* linkState)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (linkState == NULL)
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @param[in] callbacks Structure with defined callbacks
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction16(const DP_SD0801_PrivateData* pD, const DP_SD0801_Callbacks* callbacks)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_CallbacksSF(callbacks) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/**
 * A common function to check the validity of API functions with
 * following parameter types
 * @param[in] pD Driver state info specific to this instance.
 * @param[in] dpPhyInst Configuration parameters for a DP link.
 * @param[in] linkRate Link rate to initialize PHY DP link with.
 * @param[in] otherPhyInst Configuration parameters for a second PHY link.
 * @return 0 success
 * @return CDN_EINVAL invalid parameters
 */
uint32_t DP_SD0801_SanityFunction17(const DP_SD0801_PrivateData* pD, const DP_SD0801_MlPhyInstance* dpPhyInst, const DP_SD0801_LinkRate linkRate, const DP_SD0801_MlPhyInstance* otherPhyInst)
{
    /* Declaring return variable */
    uint32_t ret = 0;

    if (DP_SD0801_PrivateDataSF(pD) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_MlPhyInstanceSF(dpPhyInst) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else if (
        (linkRate != DP_SD0801_LINK_RATE_1_62) &&
        (linkRate != DP_SD0801_LINK_RATE_2_16) &&
        (linkRate != DP_SD0801_LINK_RATE_2_43) &&
        (linkRate != DP_SD0801_LINK_RATE_2_70) &&
        (linkRate != DP_SD0801_LINK_RATE_3_24) &&
        (linkRate != DP_SD0801_LINK_RATE_4_32) &&
        (linkRate != DP_SD0801_LINK_RATE_5_40) &&
        (linkRate != DP_SD0801_LINK_RATE_8_10)
        )
    {
        ret = CDN_EINVAL;
    }
    else if (DP_SD0801_MlPhyInstanceSF(otherPhyInst) == CDN_EINVAL)
    {
        ret = CDN_EINVAL;
    }
    else
    {
        /*
         * All 'if ... else if' constructs shall be terminated with an 'else' statement
         * (MISRA2012-RULE-15_7-3)
         */
    }

    return ret;
}

/* parasoft-end-suppress MISRA2012-RULE-8_7 */
/* parasoft-end-suppress METRICS-41-3 */
/* parasoft-end-suppress METRICS-39-3 */
/* parasoft-end-suppress METRICS-36-3 */
/* parasoft-end-suppress METRICS-18-3 */
