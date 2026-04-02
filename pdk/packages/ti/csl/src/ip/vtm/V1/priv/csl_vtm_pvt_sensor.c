/**
 * @file  csl_vtm_pvt_sensor.c
 *
 * @brief
 *  C implementation of the workaround computed temperature array.
 *
 *  Contains the look up table and control command and status query
 *  function definitions
 *
 * @details
 * The VTM Temperature Monitors (TEMPSENSORs) are trimmed during production
 * with resulting values stored in software readable registers.
 *
 * Software should use these register values when translating the
 * Temperature Monitor output codes to temperature values.
 *
 * A bug in the VTM-IP has zero-out the trim bits of the
 * Temperature Monitor IP (PVT). The end result is an increased error
 * in the temperature reading, which is estimated up to +/-20c.
 * The temperature monitoring feature is not usable with such
 * large error.
 *
 * This HW issues is workaround by soft trim by passing the desired trim
 * via the GP eFUSEs.
 *
 * - Use as golden reference the PVT J721e-PVT-Code values, produced by the
 *   J721e-PVTPolynomial for -40c, 30c and 125c and with it, de-compress the
 *   e-fuse AMTV values for a given sensor for the same 3 temperature points.
 * - Now using the AMTV 10-bit values vs the golden reference J7es-PVT-Code
 *   values
 *   create 2 error lines using linear interpolation of the errors.
 * - First using AMTV(-40c) and AMTV(30c) create the error line for that
 *   segment vs J721e-PVT-Code. We call that error line L_err_a1.
 * - Now to find out all the interpolation points In the look-up table for this
 *   segment we simply add L_err_a1 function values to
 *   J721e-PVT-Code values in this segment.
 * - Followed by creating the error line for the segment between AMTV(30c) and
 *   AMTV(125c) . We call that error line L_err_a2.
 * - Now to find out all the interpolation points In the look-up table for this segment we
 *   simply add L_err_a2 function values to J721e-PVT-Code values in this segment.
 * - Finally we extrapolate for the segment between 125c and 150c using the same
 *   method. Simply add L_err_a2 function values to J721e-PVT-Code values in this
 *   segment.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2020, Texas Instruments, Inc.
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
 *  DATA, OR PROFITS; OR BUSINESS int32_tERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include <string.h>
#include <stdbool.h>
#include <ti/csl/csl_vtm.h>
#include <ti/csl/soc.h>
#include <ti/csl/src/ip/vtm/V1/csl_vtm_pvt_sensor.h>
#include <ti/csl/src/ip/vtm/V1/csl_pvt_sensor_lut.h>

/* Global variables */
int32_t              gCSL_pvt_poly_work_around[CSL_VTM_NUM_OF_SENSOR_WA_COMP][CSL_VTM_NUM_OF_ADC_CODES];
int32_t              gCSL_vtm_pvt_error[CSL_VTM_NUM_EFUSE_REGS];

/* Static globals */
#if defined (SOC_J721E)
/* ADC codes for standard temperatures */
static int32_t minus40cref = 5;
static int32_t plus30cref = 253;
static int32_t plus125cref = 730;
static int32_t plus150cref = 940;
#endif

/* lut_computation done */
Bool gCSL_vtm_lut_done[CSL_VTM_NUM_OF_SENSOR_WA_COMP];
int32_t gCSL_lut_computation_init_done = CSL_VTM_VALUES_ARE_UNINITIALIZED;

/* Internal functions */
static void     CSL_vtmPrepLookupTable(void);

#if defined (SOC_J721E)
static int32_t  CSL_vtm2sComplement(uint32_t value, uint32_t mask);
static void     CSL_vtmEfuseOffsetAndMasks(uint32_t id, int32_t *pErr);


static void CSL_vtmGetErr(int32_t   *p_minus40cerr, \
                          int32_t   *p_plus30cerr,  \
                          int32_t   *p_plus125cerr, \
                          int32_t   *p_plus150cerr, \
                          uint32_t   id);

static int32_t CSL_vtm2sComplement(uint32_t value, uint32_t mask)
{
    uint32_t tmp;
    tmp = ~(value);
    tmp &= mask;
    tmp ++;

    /* Return negative value */
    return (0 - (int32_t)tmp);
}

static void CSL_vtmEfuseOffsetAndMasks(uint32_t id, int32_t *pErr)
{
    uint32_t    *p_efuse0;
    uint32_t    v0, mask0;
    uint32_t    *p_efuse1;
    uint32_t    v1, mask1;
    uint32_t    *p_efuse2;
    uint32_t    v2, mask2;


    p_efuse0 = (uint32_t *)(uintptr_t)(CSL_VTM_EFUSE_BASE_ADDR + 0x0u);
    p_efuse1 = (uint32_t *)(uintptr_t)(CSL_VTM_EFUSE_BASE_ADDR + 0x4u);
    p_efuse2 = (uint32_t *)(uintptr_t)(CSL_VTM_EFUSE_BASE_ADDR + 0x8u);

    /* Populate efuse reg offsets & Bit masks for -40C, 30C, 125C */
    switch (id) {
    case CSL_VTM_TS_ID_0:
        v0 = CSL_REG32_FEXT(p_efuse0, VTM_EFUSE0_S0_N40);
        v1 = CSL_REG32_FEXT(p_efuse2, VTM_EFUSE2_S0_P30);
        v2 = CSL_REG32_FEXT(p_efuse1, VTM_EFUSE1_S0_P125);

        mask0 = (CSL_VTM_EFUSE0_S0_N40_MASK >> CSL_VTM_EFUSE0_S0_N40_SHIFT);
        mask1 = (CSL_VTM_EFUSE2_S0_P30_MASK >> CSL_VTM_EFUSE2_S0_P30_SHIFT);
        mask2 = (CSL_VTM_EFUSE1_S0_P125_MASK >> CSL_VTM_EFUSE1_S0_P125_SHIFT);

        /* Store the 2's complement value in 32 bit representation */
        if ((v0 & CSL_VTM_EFUSE0_S0_N40_SIGN_BIT_MASK) == CSL_VTM_EFUSE0_S0_N40_SIGN_BIT_MASK)
        {
            pErr[0] = CSL_vtm2sComplement(v0, mask0);
        }
        else
        {
            pErr[0] = (int32_t)v0;
        }

        if ((v1 & CSL_VTM_EFUSE2_S0_P30_SIGN_BIT_MASK) == CSL_VTM_EFUSE2_S0_P30_SIGN_BIT_MASK)
        {
            pErr[1] = CSL_vtm2sComplement(v1, mask1);
        }
        else
        {
            pErr[1] = (int32_t)v1;
        }
        if ((v2 & CSL_VTM_EFUSE1_S0_P125_SIGN_BIT_MASK) == CSL_VTM_EFUSE1_S0_P125_SIGN_BIT_MASK)
        {
            pErr[2] = CSL_vtm2sComplement(v2, mask2);
        }
        else
        {
            pErr[2] = (int32_t)v2;
        }

        break;
    default:
        break;
    }

}

static void CSL_vtmGetErr(int32_t   *p_minus40cerr, \
                          int32_t   *p_plus30cerr,  \
                          int32_t   *p_plus125cerr, \
                          int32_t   *p_plus150cerr, \
                          uint32_t   id)
{
    /* construct the error values */
    CSL_vtmEfuseOffsetAndMasks(id, &gCSL_vtm_pvt_error[0]);
    *p_minus40cerr = gCSL_vtm_pvt_error[0];
    *p_plus30cerr  = gCSL_vtm_pvt_error[1];
    *p_plus125cerr = gCSL_vtm_pvt_error[2];
    *p_plus150cerr = gCSL_vtm_pvt_error[3];

}

bool i2128_sw_workaround_needed(void)
{
    uint32_t    *p_efuse0;
    uint32_t    v0;
    bool        retVal = true;

    p_efuse0 = (uint32_t *)(uintptr_t)(CSL_VTM_EFUSE_BASE_ADDR + 0x0u);

    v0 = CSL_REG32_RD_OFF(p_efuse0, 0);

    /* Workaround not needed if bit30/bit31 is set */
    if (0xc0000000 == (v0 & 0xc0000000))
        retVal = false;
    else
        retVal = true;

    return retVal;
}
#endif

static void CSL_vtmPrepLookupTable(void)
{
#if defined (SOC_J721E)
    int32_t minus40cerr, plus30cerr, plus125cerr, plus150cerr;
    int32_t m1, m2, m3, c1, c2, c3;
    int32_t num, den, inc;
    int32_t err;
#endif
    uint32_t sens_id = 0u;
    int32_t  i;
    int32_t *derived_array = &gCSL_pvt_poly_work_around[0][0];

    if (gCSL_lut_computation_init_done == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        gCSL_vtm_lut_done[sens_id] = FALSE;
    }

#if defined (SOC_J721E)
    /* Work around NOT needed */
    /* If not computed earlier, computation needed only once */
    if ((gCSL_vtm_lut_done[sens_id] == FALSE) && (false == i2128_sw_workaround_needed()))
    {
        for ( i = 0; i < CSL_VTM_NUM_OF_ADC_CODES; i++)
        {
            derived_array[i] = gCSL_pvt_poly_golden[i];
        }
    }

     /* Work around needed */
    /* If not computed earlier, computation needed only once */
    else if (gCSL_vtm_lut_done[sens_id] == FALSE)
    {
        for (i = 0; i < CSL_VTM_NUM_OF_ADC_CODES; i++)
        {
            derived_array[i] = 0;
        }
        CSL_vtmGetErr(&minus40cerr, &plus30cerr, &plus125cerr, &plus150cerr, sens_id);

        /*
         * Calculate the slope with adc values read from the register
         * as the y-axis param and err in adc value as x-axis param
         * gCSL_pvt_poly[5] represnts -40C with adc value 6 in polynomial equation
         * This is region -40C to 30C
         */
        if (minus40cerr != plus30cerr) {
            num = plus30cref - minus40cref;
            den = plus30cerr - minus40cerr;
            m1 = num / den;
            c1 = plus30cref - (m1 * plus30cerr);
            for (i = 0; i <= plus30cref; i++) {
                err = (i - c1) / m1;
                if ((i + err) < 0)
                {
                    continue;
                }
                derived_array[i] = gCSL_pvt_poly[i + err];
            }
        } else { /* Constant error take care of divide by zero */
            for (i = 0; i < plus30cref; i++) {
                if ((i + plus30cerr) < 0)
                {
                    continue;
                }
                derived_array[i] = gCSL_pvt_poly[i + plus30cerr];
            }
        }

        /*
         * Calculate the slope with adc values
         * as the y-axis param and err in adc value as x-axis param
         * gCSL_pvt_poly[5] represnts -40C with adc value 6 in polynomial equation
         * This is region 30C to 125C
         */
        if (plus125cerr != plus30cerr) {
            num = plus125cref - plus30cref;
            den = plus125cerr - plus30cerr;
            m2 = num / den;
            c2 = plus125cref - (m2 * plus125cerr);

            for (i = plus30cref; i < plus125cref; i++) {
                err = (i - c2) / m2;
                derived_array[i] = gCSL_pvt_poly[i + err];
            }
        } else { /* Constant error take care of divide by zero */

            for (i = plus30cref; i < plus125cref; i++) {
                if ((i + plus30cerr) < 0)
                {
                    continue;
                }
                derived_array[i] = gCSL_pvt_poly[i + plus30cerr];
            }
        }

        /*
         * Calculate the slope with adc values
         * as the y-axis param and err in adc value as x-axis param
         * gCSL_pvt_poly[5] represnts -40C with adc value 6 in polynomial equation
         * This is region 125C to 150C
         */
        if (plus150cerr != plus125cerr) {
            num = plus150cref - plus125cref;
            den = plus150cerr - plus125cerr;
            m3 = num / den;
            c3 = plus125cref - (m3 * plus125cerr);

            for (i = plus125cref; i < CSL_VTM_NUM_OF_ADC_CODES; i++) {
                err = (i - c3) / m3;
                if ((i + err) > (CSL_VTM_NUM_OF_ADC_CODES-1))
                {
                    continue;
                }
                derived_array[i] = gCSL_pvt_poly[i +err];
            }
        } else { /* Constant error take care of divide by zero */

            for (i = plus125cref; i < CSL_VTM_NUM_OF_ADC_CODES; i++) {
                if ((i + plus125cerr) < 0)
                {
                    continue;
                }
                derived_array[i] = gCSL_pvt_poly[i + plus125cerr];
            }
        }

        i = 0;
        /* Get to the first valid temperature */
        while (derived_array[i] == 0)
        {
            if ( i < (CSL_VTM_NUM_OF_ADC_CODES-2))
            {
                i++;
            }
            else
            {
                break;
            }
        }

        /* Get to the last zero index and back fill the temperature just for sake of continuity */
        if (i > 0)
        {
            /* 300 milli celsius steps */
            while(i > 0)
            {
                derived_array[i] = derived_array[i + 1] - 300;
                i--;
            }
            /* case 0 */
            derived_array[0] = derived_array[1] - 300;
        }

        /* Fill the last trailing 0s which are unfilled with increments of 100 milli celsius till (CSL_VTM_NUM_OF_ADC_CODES-1u) code */
        i = (CSL_VTM_NUM_OF_ADC_CODES-1);
        while (derived_array[i] == 0)
        {
            i--;
        }
        i++;
        inc = (int32_t)1;
        while (i < CSL_VTM_NUM_OF_ADC_CODES) {
            derived_array[i] = derived_array[i - 1] + (inc * 100);
            inc++;
            i++;
        }
    }
#else
    if (gCSL_vtm_lut_done[sens_id] == FALSE)
    {
        for ( i = 0; i < CSL_VTM_NUM_OF_ADC_CODES; i++)
        {
            derived_array[i] = gCSL_pvt_poly_golden[i];
        }
    }
#endif

    gCSL_vtm_lut_done[sens_id] = TRUE;
    return;
}



/**
* Requirement: REQ_TAG(PDK-5866) REQ_TAG(PDK-5864)
* Design: did_csl_vtm_cfg_temp_warning did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsConvADCToTemp (CSL_vtm_adc_code       adc_code,
                                CSL_vtm_tmpSens_id     temp_sensor_id,
                                int32_t                *p_milli_degree_temp_val)
{
    int32_t retVal = CSL_PASS;

    if (temp_sensor_id < (CSL_vtm_tmpSens_id)CSL_VTM_TS_MAX_NUM)
    {
        CSL_vtmPrepLookupTable();
    }

    if ((adc_code < (CSL_vtm_adc_code)0) ||
        (adc_code > (CSL_vtm_adc_code)(CSL_VTM_NUM_OF_ADC_CODES-1)))
    {
        retVal = CSL_EBADARGS;
    }

    if ((p_milli_degree_temp_val != NULL_PTR) &&
        (retVal                  == CSL_PASS))
    {
        /* for all temp sensors, use the sensor 0 table */
        *p_milli_degree_temp_val = gCSL_pvt_poly_work_around[0][adc_code];
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);
}


/**
* Requirement: REQ_TAG(PDK-5866) REQ_TAG(PDK-5864)
* Design: did_csl_vtm_cfg_temp_warning did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsConvTempToAdc (int32_t             milli_degree_temp_val,
                                CSL_vtm_tmpSens_id  temp_sensor_id,
                                CSL_vtm_adc_code    *p_adc_code)

{
    int32_t             retVal;
    CSL_vtm_adc_code    low  = (CSL_vtm_adc_code)(0);
    CSL_vtm_adc_code    high = (CSL_vtm_adc_code)(CSL_VTM_NUM_OF_ADC_CODES-1);
    CSL_vtm_adc_code    mid;
    CSL_vtm_tmpSens_id  ts_id;


    /* since pvt sensor 0 is used for all sensors, the input arg is not used */
    ts_id = CSL_VTM_TS_ID_0;

    if ((milli_degree_temp_val > CSL_VTM_TEMPERATURE_MILLI_DEGREE_C_MAX) ||
        (milli_degree_temp_val < CSL_VTM_TEMPERATURE_MILLI_DEGREE_C_MIN))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        retVal = CSL_PASS;
    }

    /* Check the temperature sensor ID out of range values */
    if (temp_sensor_id > (CSL_vtm_tmpSens_id)CSL_VTM_TS_MAX_NUM)
    {
        retVal = CSL_EBADARGS;
    }

    if ( (p_adc_code     != NULL_PTR) &&
         (retVal         == CSL_PASS))
    {

        CSL_vtmPrepLookupTable();

        /* Binary search to find the adc code */
        while (low < (high - (CSL_vtm_adc_code)1)) {
            mid = (low + high) / 2;
            if (milli_degree_temp_val <= gCSL_pvt_poly_work_around[ts_id][mid])
            {
                high = mid;
                if (milli_degree_temp_val == gCSL_pvt_poly_work_around[ts_id][mid])
                {
                    break;
                }
            }
            else
            {
                low = mid;
            }
        }

        *p_adc_code =  mid;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5866)
* Design: did_csl_vtm_cfg_temp_warning did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsSetMaxTOutRgAlertThr( const CSL_vtm_cfg2Regs  *p_cfg2,
                                       CSL_vtm_tmpSens_id      temp_sensor_id,
                                       int32_t                 high_temp_in_milli_degree_celsius,
                                       int32_t                 low_temp_in_milli_degree_celsius)
{
    int32_t                 retVal = CSL_EBADARGS;
    volatile                int32_t i;
    CSL_vtm_adc_code        adc_code_h, adc_code_l;
    uint32_t                value;
    CSL_vtm_ts_ctrl_cfg     ts_ctrl_cfg;

    if ((p_cfg2 != NULL_PTR))
    {
        retVal = CSL_vtmTsConvTempToAdc(high_temp_in_milli_degree_celsius, 0, &adc_code_h);
    }

    if (retVal == CSL_PASS)
    {
        retVal = CSL_vtmTsConvTempToAdc(low_temp_in_milli_degree_celsius, 0, &adc_code_l);
    }

    if (retVal == CSL_PASS)
    {
        /*
         * Program maximum temperature out of range thresholds
         * Step 1: set the thresholds to ~123C (sample value for high) and
         * 105C (sample value for low) WKUP_VTM_MISC_CTRL2
         * Step 2: WKUP_VTM_TMPSENS_CTRL_j set the MAXT_OUTRG_EN  bit This is already taken care as per of init
         * Step 3: WKUP_VTM_MISC_CTRL set the ANYMAXT_OUTRG_ALERT_EN  bit
         */

        /* Step 1 */
         ts_ctrl_cfg.valid_map = CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID | \
                                 CSL_VTM_TS_CTRL_RESET_CTRL_VALID      | \
                                 CSL_VTM_TS_CTRL_SOC_VALID             | \
                                 CSL_VTM_TS_CTRL_MODE_VALID;
         value =  0u;
         CSL_REG32_FINS(&value, VTM_CFG2_MISC_CTRL2_MAXT_OUTRG_ALERT_THR0, adc_code_l);
         CSL_REG32_FINS(&value, VTM_CFG2_MISC_CTRL2_MAXT_OUTRG_ALERT_THR, adc_code_h);
         CSL_REG32_WR(&p_cfg2->MISC_CTRL2,value);

         /* Step 2 */
         ts_ctrl_cfg.valid_map = CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID;
         ts_ctrl_cfg.maxt_outrg_alert_en = CSL_VTM_TS_CTRL_MAXT_OUTRG_GEN_ALERT;

         retVal = CSL_vtmTsSetCtrl (p_cfg2,
                         temp_sensor_id,
                         &ts_ctrl_cfg, TRUE);

         if (retVal == CSL_PASS)
         {
            /* have some delay before write */
            for (i = 0; i < CSL_VTM_REG_READ_DELAY;)
            {
                i = i + 1;
            }

            /* Step 3 */
            CSL_REG32_FINS(&p_cfg2->MISC_CTRL, \
                           VTM_CFG2_MISC_CTRL_ANY_MAXT_OUTRG_ALERT_EN, \
                           CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_ENABLE);
         }
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);

}

/* Nothing past this point */

