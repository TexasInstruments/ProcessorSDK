/**
 * @file  csl_vtm.c
 *
 * @brief
 *  C implementation file for the VTM module CSL-FL.
 *
 *  Contains the different control command and status query functions definitions
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
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#include <string.h>
#include <stdbool.h>
#include <ti/csl/csl_vtm.h>

/*=============================================================================
 *  Internal definitions and functions
 *===========================================================================*/

#if defined (SOC_J721E)
static CSL_vtm_adc_code CSL_vtmGetBestValue(CSL_vtm_adc_code c0, CSL_vtm_adc_code c1, CSL_vtm_adc_code c2);
static CSL_vtm_adc_code CSL_vtm_abs(CSL_vtm_adc_code val);
#endif
static CSL_vtm_adc_code CSL_vtmGetAdcCode(const CSL_vtm_cfg1Regs_TMPSENS*    p_sensor);
static void CSL_vtmGetSensorVDCount(const CSL_vtm_cfg1Regs       *p_cfg1);

/*=============================================================================
 *  internal macros
 *===========================================================================*/

#define CSL_VTM_VALUES_ARE_UNINITIALIZED    (-1)
/* Delay for Reg Reads */
#define CSL_VTM_DOUT_REG_READ_DELAY         (100)

/*=============================================================================
 *  static global variables
 *===========================================================================*/

/* Uninitialized number of temperature sensors, will be initialized later */
static int32_t gNumTempSensors        = CSL_VTM_VALUES_ARE_UNINITIALIZED;
/* Uninitialized number of core voltage domain, will be initialized later */
static int32_t gNumCoreVoltageDomains = CSL_VTM_VALUES_ARE_UNINITIALIZED;

/*=============================================================================
 *  Internal functions
 *===========================================================================*/

#if defined (SOC_J721E)
static CSL_vtm_adc_code CSL_vtm_abs(CSL_vtm_adc_code val)
{
    CSL_vtm_adc_code retVal;
    if (val < 0)
    {
        retVal = -val;
    }
    else
    {
        retVal = val;
    }
    return (retVal);
}

static CSL_vtm_adc_code CSL_vtmGetBestValue(CSL_vtm_adc_code c0,
                                            CSL_vtm_adc_code c1,
                                            CSL_vtm_adc_code c2)
{ 
   CSL_vtm_adc_code d01 = CSL_vtm_abs(c0 - c1);
   CSL_vtm_adc_code d02 = CSL_vtm_abs(c0 - c2);
   CSL_vtm_adc_code d12 = CSL_vtm_abs(c1 - c2);
   CSL_vtm_adc_code result;
                    
   /* if delta 01 is least, take 0 and 1 */
   if ((d01 <= d02) && (d01 <=d12))
   {
           result = (c0+c1)/2; 
   } 
   /* if delta 02 is least, take 0 and 2 */
   else if ((d02 <= d01) && (d02 <=d12))
   {
           result = (c0+c2)/2; 
   } 
   else
   {
       /* in all other cases, take 1 and 2 */ 
       result = (c1+c2)/2;
   }
   return (result); 
}
#endif

static void CSL_vtmGetSensorVDCount(const CSL_vtm_cfg1Regs       *p_cfg1)
{
    gNumTempSensors         = (int32_t)CSL_REG32_FEXT(&p_cfg1->DEVINFO_PWR0, \
                                     VTM_CFG1_DEVINFO_PWR0_TMPSENS_CT);
    gNumCoreVoltageDomains  = (int32_t)CSL_REG32_FEXT(&p_cfg1->DEVINFO_PWR0, \
                                     VTM_CFG1_DEVINFO_PWR0_CVD_CT);
}
  
static CSL_vtm_adc_code CSL_vtmGetAdcCode(const CSL_vtm_cfg1Regs_TMPSENS    *p_sensor) 
{ 
#if defined(SOC_J721E)
    CSL_vtm_adc_code s0,s1,s2;
    volatile        int32_t  i;

    /* have some delay before read */
    for (i = 0; i < CSL_VTM_DOUT_REG_READ_DELAY;)
    {
        i = i + 1;
    }
    s0 = (CSL_vtm_adc_code)CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_DATA_OUT);

    /* have some delay before read */
    for (i = 0; i < CSL_VTM_DOUT_REG_READ_DELAY;)
    {
        i = i + 1;
    }
    s1 = (CSL_vtm_adc_code)CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_DATA_OUT);

    /* have some delay before read */
    for (i = 0; i < CSL_VTM_DOUT_REG_READ_DELAY;)
    {
        i = i + 1;
    }
    s2 = (CSL_vtm_adc_code)CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_DATA_OUT);

    /* Return the best of 3 values */
    return CSL_vtmGetBestValue(s0,s1,s2); 
#else
    CSL_vtm_adc_code s0;
    s0 = (CSL_vtm_adc_code)CSL_REG32_FEXT(&p_sensor->STAT, \
            VTM_CFG1_TMPSENS_STAT_DATA_OUT);
    return s0;
#endif
}


/*=============================================================================
 *  Interface functions
 *===========================================================================*/

/**  
* Requirement: REQ_TAG(PDK-5862)
* Design: did_csl_vtm_cfg_vd did_csl_vtm_interfaces
*/

int32_t CSL_vtmVdSetOppVid (const CSL_vtm_cfg1Regs  *p_cfg,
                            CSL_vtm_vd_id           voltage_domain,
                            CSL_vtm_vid_opp         vid_opp, 
                            uint8_t                 vid_opp_val,
                            Bool                    read_verify_flag)
{
    int32_t  retVal = CSL_PASS;
    uint32_t vid_opp_val_read;
    uint32_t vid_opp_val_prog = (uint32_t) vid_opp_val;

    /* Argument check for VD, temperature sensor */
    if (gNumCoreVoltageDomains == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((voltage_domain      >= (CSL_vtm_vd_id) gNumCoreVoltageDomains)  || 
       (vid_opp             >= CSL_VTM_VID_OPP_MAX_NUM)                 ||
       (p_cfg               == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);

        switch (vid_opp)
        {
            case CSL_VTM_VID_OPP_0_CODE:
                CSL_REG32_FINS(&vtmVDRegs->OPPVID,              \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_0,    \
                               vid_opp_val);
                break;
            case CSL_VTM_VID_OPP_1_CODE:
                CSL_REG32_FINS(&vtmVDRegs->OPPVID,              \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_1,    \
                               vid_opp_val);
                break;
            case CSL_VTM_VID_OPP_2_CODE:
                CSL_REG32_FINS(&vtmVDRegs->OPPVID,              \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_2,    \
                               vid_opp_val);
                break;
            case CSL_VTM_VID_OPP_3_CODE:
                CSL_REG32_FINS(&vtmVDRegs->OPPVID,              \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_3,    \
                               vid_opp_val);
                break;
            default:
                retVal = CSL_EBADARGS;
                break;
        }        

    }

    /* read verify check to be implemented for safety */
    if ((read_verify_flag == TRUE) &&
        (retVal == CSL_PASS))
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);

        switch(vid_opp)
        {
            case CSL_VTM_VID_OPP_0_CODE:
                vid_opp_val_read = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                                                  VTM_CFG1_VTM_VD_OPPVID_OPP_0);
                break;
            case CSL_VTM_VID_OPP_1_CODE:
                vid_opp_val_read = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                                                  VTM_CFG1_VTM_VD_OPPVID_OPP_1);
                break;
            case CSL_VTM_VID_OPP_2_CODE:
                vid_opp_val_read = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                                                  VTM_CFG1_VTM_VD_OPPVID_OPP_2);
                break;
            case CSL_VTM_VID_OPP_3_CODE:
                vid_opp_val_read = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                                                  VTM_CFG1_VTM_VD_OPPVID_OPP_3);
                break;
            default:
                vid_opp_val_read = (uint32_t)CSL_VTM_VALUES_ARE_UNINITIALIZED;
                retVal           = CSL_EBADARGS;
                break;
        }

        /* Fail the API if the value read is not same as the value programmed */
        if (vid_opp_val_read != vid_opp_val_prog)
        {
            retVal = CSL_EFAIL;
        }
    }

    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5865)
* Design: did_csl_vtm_wr_read_back did_csl_vtm_interfaces
*/
int32_t CSL_vtmVdGetOppVid (const CSL_vtm_cfg1Regs  *p_cfg,  
                            CSL_vtm_vd_id           voltage_domain,
                            CSL_vtm_vid_opp         vid_opp, 
                            uint8_t                 *p_vid_opp_val)

{
    int32_t     retVal = CSL_PASS;
    uint32_t    vid_opp_val;

    /* Argument check for VD, temperature sensor */
    if (gNumCoreVoltageDomains == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((voltage_domain  >= (CSL_vtm_vd_id)gNumCoreVoltageDomains)       || 
       (vid_opp         >= CSL_VTM_VID_OPP_MAX_NUM)                     ||
       (p_cfg           == NULL_PTR)                                    ||
       (p_vid_opp_val   == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);

        switch (vid_opp)
        {
            case CSL_VTM_VID_OPP_0_CODE:
                vid_opp_val = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_0);
                break;
            case CSL_VTM_VID_OPP_1_CODE:
                vid_opp_val = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_1);
                break;
            case CSL_VTM_VID_OPP_2_CODE:
                vid_opp_val = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_2);
                break;
            case CSL_VTM_VID_OPP_3_CODE:
                vid_opp_val = CSL_REG32_FEXT(&vtmVDRegs->OPPVID, \
                               VTM_CFG1_VTM_VD_OPPVID_OPP_3);
                break;
            default:
                retVal = CSL_EBADARGS;
                break;
        }        

    }

    if ( (p_vid_opp_val != NULL_PTR) &&
         (retVal == CSL_PASS) )
    {
        *p_vid_opp_val = (uint8_t) vid_opp_val;
    }
    return (retVal);
}


/**  
* Requirement: REQ_TAG(PDK-5862)
* Design: did_csl_vtm_cfg_vd did_csl_vtm_interfaces
*/
int32_t CSL_vtmVdEvtSelSet (const CSL_vtm_cfg1Regs  *p_cfg,
                            CSL_vtm_vd_id           voltage_domain,
                            CSL_vtm_vdEvt_sel_set   vd_temp_evts,
                            Bool                    read_verify_flag)

{
    int32_t retVal = CSL_PASS;
    uint32_t reg_val_read, reg_val_prog = (uint32_t)vd_temp_evts;

    /* Argument check for VD, temperature sensor */
    if (gNumCoreVoltageDomains == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((voltage_domain  >= (CSL_vtm_vd_id) gNumCoreVoltageDomains)      || 
       (p_cfg           == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);
        CSL_REG32_FINS(&vtmVDRegs->EVT_SEL_SET,              \
               VTM_CFG1_EVT_SET_TSENS_EVT_SEL,               \
               vd_temp_evts);
    }

    if ((read_verify_flag == TRUE) &&
        (retVal == CSL_PASS))
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);
        reg_val_read = CSL_REG32_FEXT(&vtmVDRegs->EVT_SEL_SET, \
                                          VTM_CFG1_EVT_SET_TSENS_EVT_SEL);
        if (reg_val_prog != reg_val_read)
        {
            retVal = CSL_EFAIL;
        }
    }
   
    return (retVal);
}


/**  
* Requirement: REQ_TAG(PDK-5862)
* Design: did_csl_vtm_cfg_vd did_csl_vtm_interfaces
*/
int32_t CSL_vtmVdEvtSelClr (const CSL_vtm_cfg1Regs  *p_cfg,
                            CSL_vtm_vd_id           voltage_domain,
                            CSL_vtm_vdEvt_sel_clr   vd_temp_evts,
                            Bool                    read_verify_flag)

{
    int32_t retVal = CSL_PASS;
    uint32_t reg_val_read, reg_val_prog = (uint32_t)vd_temp_evts;

    /* Argument check for VD, temperature sensor */
    if (gNumCoreVoltageDomains == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((voltage_domain  >= (CSL_vtm_vd_id) gNumCoreVoltageDomains)          || 
       (p_cfg           == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);
        CSL_REG32_FINS(&vtmVDRegs->EVT_SEL_CLR,                \
                       VTM_CFG1_EVT_CLR_TSENS_EVT_SEL,         \
                       vd_temp_evts);
    }

    if ((read_verify_flag == TRUE) &&
        (retVal           == CSL_PASS))
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);
        reg_val_read = CSL_REG32_FEXT(&vtmVDRegs->EVT_SEL_CLR, \
                                      VTM_CFG1_EVT_CLR_TSENS_EVT_SEL);
        reg_val_read &= reg_val_prog;
        if (0U != reg_val_read)
        {
            retVal = CSL_EFAIL;
        }
    }   

    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5865)
* Design: did_csl_vtm_wr_read_back did_csl_vtm_interfaces
*/

int32_t CSL_vtmVdGetEvtStat ( const CSL_vtm_cfg1Regs     *p_cfg,
                              CSL_vtm_vd_id               voltage_domain,    
                              CSL_vtm_vdEvt_status       *p_evt_stat)
{
    int32_t retVal = CSL_PASS;
    uint32_t mask = CSL_VTM_VD_EVT_STAT_THR_ALERTS_MASK;

    /* Argument check for VD, temperature sensor */
    if (gNumCoreVoltageDomains == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((voltage_domain  >= (CSL_vtm_vd_id) gNumCoreVoltageDomains)  || 
       (p_evt_stat      ==  NULL_PTR)                               ||
       (p_cfg           ==  NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        const CSL_vtm_cfg1Regs_VD * vtmVDRegs = &(p_cfg->VD[voltage_domain]);
        *p_evt_stat = (CSL_vtm_vdEvt_status)(vtmVDRegs->EVT_STAT & mask);
    }

    return (retVal);
}



/**  
* Requirement: REQ_TAG(PDK-5862)
* Design: did_csl_vtm_cfg_vd did_csl_vtm_interfaces
*/
int32_t CSL_vtmVdThrIntrCtrl (const CSL_vtm_cfg1Regs         *p_cfg,
                              CSL_vtm_vd_id                  voltage_domain,
                              CSL_vtm_vdThr_interrupt_ctrl   ctrl,
                              Bool                           read_verify_flag)
{
    int32_t                         retVal = CSL_PASS;
    uint32_t                        vd;
    uint32_t                        ctrl_read, ctrl_prog = (uint32_t) ctrl;

    /* Argument check for VD, temperature sensor */
    if (gNumCoreVoltageDomains == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((voltage_domain  >= (CSL_vtm_vd_id) gNumCoreVoltageDomains)  || 
       (p_cfg           ==  NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }

    /* Arg check for the control, it can't have orthogonal settings */
    if ((retVal                                   == CSL_PASS) &&
        ((ctrl & CSL_VTM_VD_LT_THR0_INTR_RAW_SET) == CSL_VTM_VD_LT_THR0_INTR_RAW_SET) &&
        ((ctrl & CSL_VTM_VD_LT_THR0_INTR_RAW_CLR) == CSL_VTM_VD_LT_THR0_INTR_RAW_CLR))
    {
        retVal = CSL_EBADARGS;
    }

    /* Arg check for the control, it can't have orthogonal settings */
    if ((retVal                                   == CSL_PASS) &&
        ((ctrl & CSL_VTM_VD_GT_THR1_INTR_RAW_SET) == CSL_VTM_VD_GT_THR1_INTR_RAW_SET) &&
        ((ctrl & CSL_VTM_VD_GT_THR1_INTR_RAW_CLR) == CSL_VTM_VD_GT_THR1_INTR_RAW_CLR))
    {
        retVal = CSL_EBADARGS;
    }

    /* Arg check for the control, it can't have orthogonal settings */
    if ((retVal                                   == CSL_PASS) &&
        ((ctrl & CSL_VTM_VD_GT_THR2_INTR_RAW_SET) == CSL_VTM_VD_GT_THR2_INTR_RAW_SET) &&
        ((ctrl & CSL_VTM_VD_GT_THR2_INTR_RAW_CLR) == CSL_VTM_VD_GT_THR2_INTR_RAW_CLR))
    {
        retVal = CSL_EBADARGS;
    }

    /* Arg check for the control, it can't have orthogonal settings */
    if ((retVal                                  == CSL_PASS) &&
        ((ctrl & CSL_VTM_VD_LT_THR0_INTR_EN_SET) == CSL_VTM_VD_LT_THR0_INTR_EN_SET) &&
        ((ctrl & CSL_VTM_VD_LT_THR0_INTR_EN_CLR) == CSL_VTM_VD_LT_THR0_INTR_EN_CLR))
    {
        retVal = CSL_EBADARGS;
    }

    /* Arg check for the control, it can't have orthogonal settings */
    if ((retVal                                  == CSL_PASS) &&
        ((ctrl & CSL_VTM_VD_GT_THR1_INTR_EN_SET) == CSL_VTM_VD_GT_THR1_INTR_EN_SET) &&
        ((ctrl & CSL_VTM_VD_GT_THR1_INTR_EN_CLR) == CSL_VTM_VD_GT_THR1_INTR_EN_CLR))
    {
        retVal = CSL_EBADARGS;
    }

    /* Arg check for the control, it can't have orthogonal settings */
    if ((retVal                                  == CSL_PASS) &&
        ((ctrl & CSL_VTM_VD_GT_THR2_INTR_EN_SET) == CSL_VTM_VD_GT_THR2_INTR_EN_SET) &&
        ((ctrl & CSL_VTM_VD_GT_THR2_INTR_EN_CLR) == CSL_VTM_VD_GT_THR2_INTR_EN_CLR))
    {
        retVal = CSL_EBADARGS;
    }

    if (retVal == CSL_PASS)
    {
        vd = (uint32_t)((uint32_t)1u << voltage_domain);
        if ((ctrl & CSL_VTM_VD_LT_THR0_INTR_RAW_SET) == CSL_VTM_VD_LT_THR0_INTR_RAW_SET)
        {
            CSL_REG32_FINS(&p_cfg->LT_TH0_INT_RAW_STAT_SET,     \
               VTM_CFG1_LT_TH0_INT_RAW_STAT_SET_INT_VD, vd);
        }
        if ((ctrl & CSL_VTM_VD_GT_THR1_INTR_RAW_SET) == CSL_VTM_VD_GT_THR1_INTR_RAW_SET)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH1_INT_RAW_STAT_SET,     \
               VTM_CFG1_GT_TH1_INT_RAW_STAT_SET_INT_VD, vd);
        }

        if ((ctrl & CSL_VTM_VD_GT_THR2_INTR_RAW_SET) == CSL_VTM_VD_GT_THR2_INTR_RAW_SET)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH2_INT_RAW_STAT_SET,     \
               VTM_CFG1_GT_TH2_INT_RAW_STAT_SET_INT_VD, vd);
        }

        if ((ctrl & CSL_VTM_VD_LT_THR0_INTR_RAW_CLR) == CSL_VTM_VD_LT_THR0_INTR_RAW_CLR)
        {
            CSL_REG32_FINS(&p_cfg->LT_TH0_INT_EN_STAT_CLR,     \
               VTM_CFG1_LT_TH0_INT_EN_STAT_CLR_INT_VD, vd);
        }
        if ((ctrl & CSL_VTM_VD_GT_THR1_INTR_RAW_CLR) == CSL_VTM_VD_GT_THR1_INTR_RAW_CLR)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH1_INT_EN_STAT_CLR,     \
               VTM_CFG1_GT_TH1_INT_EN_STAT_CLR_INT_VD, vd);
        }

        if ((ctrl & CSL_VTM_VD_GT_THR2_INTR_RAW_CLR) == CSL_VTM_VD_GT_THR2_INTR_RAW_CLR)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH2_INT_EN_STAT_CLR,     \
               VTM_CFG1_GT_TH2_INT_EN_STAT_CLR_INT_VD, vd);
        }        

        if ((ctrl & CSL_VTM_VD_LT_THR0_INTR_EN_SET) == CSL_VTM_VD_LT_THR0_INTR_EN_SET)
        {
            CSL_REG32_FINS(&p_cfg->LT_TH0_INT_EN_SET,     \
               VTM_CFG1_LT_TH0_INT_EN_SET_INT_VD, vd);
        }
        if ((ctrl & CSL_VTM_VD_GT_THR1_INTR_EN_SET) == CSL_VTM_VD_GT_THR1_INTR_EN_SET)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH1_INT_EN_SET,     \
               VTM_CFG1_GT_TH1_INT_EN_SET_INT_VD, vd);
        }
        
        if ((ctrl & CSL_VTM_VD_GT_THR2_INTR_EN_SET) == CSL_VTM_VD_GT_THR2_INTR_EN_SET)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH2_INT_EN_SET,     \
               VTM_CFG1_GT_TH2_INT_EN_SET_INT_VD, vd);
        }

        if ((ctrl & CSL_VTM_VD_LT_THR0_INTR_EN_CLR) == CSL_VTM_VD_LT_THR0_INTR_EN_CLR)
        {
            CSL_REG32_FINS(&p_cfg->LT_TH0_INT_EN_CLR,     \
               VTM_CFG1_LT_TH0_INT_EN_CLR_INT_VD, vd);
        }
        if ((ctrl & CSL_VTM_VD_GT_THR1_INTR_EN_CLR) == CSL_VTM_VD_GT_THR1_INTR_EN_CLR)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH1_INT_EN_CLR,     \
               VTM_CFG1_GT_TH1_INT_EN_CLR_INT_VD, vd);
        }
        
        if ((ctrl & CSL_VTM_VD_GT_THR2_INTR_EN_CLR) == CSL_VTM_VD_GT_THR2_INTR_EN_CLR)
        {
            CSL_REG32_FINS(&p_cfg->GT_TH2_INT_EN_CLR,     \
               VTM_CFG1_GT_TH2_INT_EN_CLR_INT_VD, vd);
        }
    }

    if ((read_verify_flag == TRUE) &&
        (retVal           == CSL_PASS))
    {
        uint32_t vd_rd;

        ctrl_read = (uint32_t)0u;
        
        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->LT_TH0_INT_RAW_STAT_SET,     \
                               VTM_CFG1_LT_TH0_INT_RAW_STAT_SET_INT_VD);

        /* indicate in ctrl_read */
        if ((vd_rd & vd) == vd)
        {
            ctrl_read += CSL_VTM_VD_LT_THR0_INTR_RAW_SET;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH1_INT_RAW_STAT_SET,     \
                               VTM_CFG1_GT_TH1_INT_RAW_STAT_SET_INT_VD);

        /* indicate in ctrl_read */
        if ((vd_rd & vd) == vd)
        {
            ctrl_read += CSL_VTM_VD_GT_THR1_INTR_RAW_SET;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH2_INT_RAW_STAT_SET,     \
                               VTM_CFG1_GT_TH2_INT_RAW_STAT_SET_INT_VD);

        /* indicate in ctrl_read */
        if ((vd_rd & vd) == vd)
        {
            ctrl_read += CSL_VTM_VD_GT_THR2_INTR_RAW_SET;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->LT_TH0_INT_EN_STAT_CLR,     \
                               VTM_CFG1_LT_TH0_INT_EN_STAT_CLR_INT_VD);

        /* indicate in ctrl_read */
        vd_rd &= vd;
        if (vd_rd == 0U)
        {
            ctrl_read += CSL_VTM_VD_LT_THR0_INTR_RAW_CLR;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH1_INT_EN_STAT_CLR,     \
                                VTM_CFG1_GT_TH1_INT_EN_STAT_CLR_INT_VD);
        /* indicate in ctrl_read */
        vd_rd &= vd;
        if (vd_rd == 0U)
        {
            ctrl_read += CSL_VTM_VD_GT_THR1_INTR_RAW_CLR;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH2_INT_EN_STAT_CLR,     \
                VTM_CFG1_GT_TH2_INT_EN_STAT_CLR_INT_VD);
        /* indicate in ctrl_read */
        vd_rd &= vd;
        if (vd_rd == 0U)
        {
            ctrl_read += CSL_VTM_VD_GT_THR2_INTR_RAW_CLR;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->LT_TH0_INT_EN_SET,     \
                                VTM_CFG1_LT_TH0_INT_EN_SET_INT_VD);
        /* indicate in ctrl_read */
        if ((vd_rd & vd) == vd)
        {
            ctrl_read += CSL_VTM_VD_LT_THR0_INTR_EN_SET;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH1_INT_EN_SET,     \
                                VTM_CFG1_GT_TH1_INT_EN_SET_INT_VD);
        /* indicate in ctrl_read */
        if ((vd_rd & vd) == vd)
        {
            ctrl_read += CSL_VTM_VD_GT_THR1_INTR_EN_SET;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH2_INT_EN_SET,     \
                                VTM_CFG1_GT_TH2_INT_EN_SET_INT_VD);
        /* indicate in ctrl_read */
        if ((vd_rd & vd) == vd)
        {
            ctrl_read += CSL_VTM_VD_GT_THR2_INTR_EN_SET;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->LT_TH0_INT_EN_CLR,     \
                                VTM_CFG1_LT_TH0_INT_EN_CLR_INT_VD);
        /* indicate in ctrl_read */
        vd_rd &= vd;
        if (vd_rd == 0U)
        {
            ctrl_read += CSL_VTM_VD_LT_THR0_INTR_EN_CLR;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH1_INT_EN_CLR,     \
                                VTM_CFG1_GT_TH1_INT_EN_CLR_INT_VD);
        /* indicate in ctrl_read */
        vd_rd &= vd;
        if (vd_rd == 0U)
        {
            ctrl_read += CSL_VTM_VD_GT_THR1_INTR_EN_CLR;
        }

        /* Read the VD for this threshold */
        vd_rd = CSL_REG32_FEXT(&p_cfg->GT_TH2_INT_EN_CLR,     \
                                VTM_CFG1_GT_TH2_INT_EN_CLR_INT_VD);

        /* indicate in ctrl_read */
        vd_rd &= vd;
        if (vd_rd == 0U)
        {
            ctrl_read += CSL_VTM_VD_GT_THR2_INTR_EN_CLR;
        }

        /* Mask other values, that are read */
        ctrl_read &= ctrl_prog;

        if (ctrl_prog != ctrl_read)
        {
            retVal = CSL_EFAIL;
        }
    }

    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5866)
* Design: did_csl_vtm_cfg_temp_warning did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsSetThresholds (const CSL_vtm_cfg1Regs   *p_cfg,
                                CSL_vtm_tmpSens_id        temp_sensor_id,
                                const CSL_vtm_tsThrVal   *p_thr_val,
                                Bool                      read_verify_flag)
{
    int32_t                             retVal = CSL_PASS;
    uint32_t                            enable;
    const CSL_vtm_cfg1Regs_TMPSENS      *pVtmTSRegs;
    CSL_vtm_tsThrVal                    tsThrValRd;

    /* Argument check for VD, temperature sensor */
    if (gNumTempSensors == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((temp_sensor_id  >= (CSL_vtm_tmpSens_id) gNumTempSensors)      || \
       (p_thr_val       == NULL_PTR)                                  || \
       (p_cfg           == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        pVtmTSRegs = &p_cfg->TMPSENS[temp_sensor_id];

        if ((p_thr_val->thrValidMap & CSL_VTM_LT_TH0_VALID) == CSL_VTM_LT_TH0_VALID)
        {
            CSL_REG32_FINS(&pVtmTSRegs->TH, VTM_CFG1_TMPSENS_TH_TH0_VAL, p_thr_val->ltTh0);
            if (p_thr_val->ltTh0En == FALSE)
            {
                enable = 0u;
            }
            else
            {
                enable = 1u;
            }
            CSL_REG32_FINS(&pVtmTSRegs->CTRL, VTM_CFG1_TMPSENS_CTRL_LT_TH0_EN, enable);
        }
        if ((p_thr_val->thrValidMap & CSL_VTM_GT_TH1_VALID) != 0u)
        {
            CSL_REG32_FINS(&pVtmTSRegs->TH, VTM_CFG1_TMPSENS_TH_TH1_VAL, p_thr_val->gtTh1);

            if (p_thr_val->gtTh1En == FALSE)
            {
                enable = 0u;
            }
            else
            {
                enable = 1u;
            }

            CSL_REG32_FINS(&pVtmTSRegs->CTRL, VTM_CFG1_TMPSENS_CTRL_GT_TH1_EN, enable);
        }
        if ((p_thr_val->thrValidMap & CSL_VTM_GT_TH2_VALID) != 0u)
        {
            CSL_REG32_FINS(&pVtmTSRegs->TH2, VTM_CFG1_TMPSENS_TH2_TH2_VAL, p_thr_val->gtTh2);

            if (p_thr_val->gtTh2En == FALSE)
            {
                enable = 0u;
            }
            else
            {
                enable = 1u;
            }

            CSL_REG32_FINS(&pVtmTSRegs->CTRL, VTM_CFG1_TMPSENS_CTRL_GT_TH2_EN, enable);
        }      
    }

    if ((retVal == CSL_PASS) &&
        (read_verify_flag == TRUE))
    {
        tsThrValRd.thrValidMap  = p_thr_val->thrValidMap;

        /* Read the thresholds */
        retVal = CSL_vtmTsGetThresholds(p_cfg, temp_sensor_id, &tsThrValRd);

        if (retVal == CSL_PASS)
        {
            /* set the threshold values same as passed value for invalid maps */
            if ((tsThrValRd.thrValidMap & CSL_VTM_LT_TH0_VALID) != CSL_VTM_LT_TH0_VALID)
            {
                tsThrValRd.ltTh0    = p_thr_val->ltTh0;
                tsThrValRd.ltTh0En  = p_thr_val->ltTh0En;
            }
            /* set the threshold values same as passed value for invalid maps */
            if ((tsThrValRd.thrValidMap & CSL_VTM_GT_TH1_VALID) != CSL_VTM_GT_TH1_VALID)
            {
                tsThrValRd.gtTh1    = p_thr_val->gtTh1;
                tsThrValRd.gtTh1En  = p_thr_val->gtTh1En;
            }
            /* set the threshold values same as passed value for invalid maps */
            if ((tsThrValRd.thrValidMap & CSL_VTM_GT_TH2_VALID) != CSL_VTM_GT_TH2_VALID)
            {
                tsThrValRd.gtTh2    = p_thr_val->gtTh2;
                tsThrValRd.gtTh2En  = p_thr_val->gtTh2En;
            }
        }

        if ((tsThrValRd.ltTh0    != p_thr_val->ltTh0)       &&
            (tsThrValRd.ltTh0En  != p_thr_val->ltTh0En)     &&
            (tsThrValRd.gtTh1    != p_thr_val->gtTh1)       &&
            (tsThrValRd.gtTh1En  != p_thr_val->gtTh1En)     &&
            (tsThrValRd.gtTh2    != p_thr_val->gtTh2)       &&
            (tsThrValRd.gtTh2En  != p_thr_val->gtTh2En))
        {
            retVal = CSL_EFAIL;
        }
    }
    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5865)
* Design: did_csl_vtm_wr_read_back did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsGetThresholds (const CSL_vtm_cfg1Regs   *p_cfg,
                                CSL_vtm_tmpSens_id        temp_sensor_id,
                                CSL_vtm_tsThrVal         *p_thr_val)

{
    int32_t retVal = CSL_PASS;
    const CSL_vtm_cfg1Regs_TMPSENS    *pVtmTSRegs;

    /* Argument check for VD, temperature sensor */
    if (gNumTempSensors == CSL_VTM_VALUES_ARE_UNINITIALIZED)
    {
        CSL_vtmGetSensorVDCount(p_cfg);
    }

    /* argument checks */
    if((temp_sensor_id  >= (CSL_vtm_tmpSens_id)gNumTempSensors)   ||
        (p_thr_val      == NULL_PTR)                              ||
        (p_cfg          == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        pVtmTSRegs = &p_cfg->TMPSENS[temp_sensor_id];

        if ((p_thr_val->thrValidMap & CSL_VTM_LT_TH0_VALID) == CSL_VTM_LT_TH0_VALID)
        {
            p_thr_val->ltTh0   = (CSL_vtm_adc_code)CSL_REG32_FEXT(&pVtmTSRegs->TH, VTM_CFG1_TMPSENS_TH_TH0_VAL);
            p_thr_val->ltTh0En = (Bool) CSL_REG32_FEXT(&pVtmTSRegs->CTRL, VTM_CFG1_TMPSENS_CTRL_LT_TH0_EN);
        }
        if ((p_thr_val->thrValidMap & CSL_VTM_GT_TH1_VALID) == CSL_VTM_GT_TH1_VALID)
        {
            p_thr_val->gtTh1   = (CSL_vtm_adc_code) CSL_REG32_FEXT(&pVtmTSRegs->TH, VTM_CFG1_TMPSENS_TH_TH1_VAL);
            p_thr_val->gtTh1En = (Bool) CSL_REG32_FEXT(&pVtmTSRegs->CTRL, VTM_CFG1_TMPSENS_CTRL_GT_TH1_EN);            
        }
        if ((p_thr_val->thrValidMap & CSL_VTM_GT_TH2_VALID) == CSL_VTM_GT_TH2_VALID)
        {
            p_thr_val->gtTh2   = (CSL_vtm_adc_code)CSL_REG32_FEXT(&pVtmTSRegs->TH2, VTM_CFG1_TMPSENS_TH2_TH2_VAL);
            p_thr_val->gtTh2En = (Bool) CSL_REG32_FEXT(&pVtmTSRegs->CTRL, VTM_CFG1_TMPSENS_CTRL_GT_TH2_EN);
        }      
   }

   return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5866)
* Design: did_csl_vtm_cfg_temp_warning did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsSetGlobalCfg (const CSL_vtm_cfg2Regs       *p_cfg2,
                               const CSL_vtm_tsGlobal_cfg         *p_tsGlobal_cfg,
                               Bool                          read_verify_flag)

{
    int32_t                             retVal = CSL_PASS;
    CSL_vtm_tsGlobal_ctrl_valid_map     valid_map;
    CSL_vtm_tsGlobal_cfg                tsGlobalCfgRd;

    /* argument checks */
    if( (p_tsGlobal_cfg  == NULL_PTR)    ||
        (p_cfg2          ==  NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        valid_map = p_tsGlobal_cfg->validMap;
        if ((valid_map & CSL_VTM_TSGLOBAL_CLK_SEL_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_cfg2->CLK_CTRL, VTM_CFG2_CLK_CTRL_TSENS_CLK_SEL, \
                p_tsGlobal_cfg->clkSel);
        }
        if ((valid_map & CSL_VTM_TSGLOBAL_CLK_DIV_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_cfg2->CLK_CTRL, VTM_CFG2_CLK_CTRL_TSENS_CLK_DIV, \
                p_tsGlobal_cfg->clkDiv);
        }        

        if ((valid_map & CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_EN_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_cfg2->MISC_CTRL, VTM_CFG2_MISC_CTRL_ANY_MAXT_OUTRG_ALERT_EN, \
                p_tsGlobal_cfg->any_maxt_outrg_alert_en);
        }    

        if ((valid_map & CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_cfg2->MISC_CTRL2, VTM_CFG2_MISC_CTRL2_MAXT_OUTRG_ALERT_THR, \
                p_tsGlobal_cfg->maxt_outrg_alert_thr);
        }

        if ((valid_map & CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR0_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_cfg2->MISC_CTRL2, VTM_CFG2_MISC_CTRL2_MAXT_OUTRG_ALERT_THR0, \
                p_tsGlobal_cfg->maxt_outrg_alert_thr0);
        }

        if ((valid_map & CSL_VTM_TSGLOBAL_SAMPLES_PER_CNT_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_cfg2->SAMPLE_CTRL, VTM_CFG2_SAMPLE_CTRL_SAMPLE_PER_CNT, \
                p_tsGlobal_cfg->samplesPerCnt);
        }
    }

    if ((retVal           == CSL_PASS) &&
        (read_verify_flag == TRUE))
    {
        tsGlobalCfgRd.validMap  = valid_map;

        /* Get the global config */
        retVal = CSL_vtmTsGetGlobalCfg(p_cfg2, &tsGlobalCfgRd);

        /* Keep the same value for unconfigured values, for compare */
        if (retVal == CSL_PASS)
        {
            if ((valid_map & CSL_VTM_TSGLOBAL_CLK_SEL_VALID) == 0u)
            {
                tsGlobalCfgRd.clkSel = p_tsGlobal_cfg->clkSel;
            }
            if ((valid_map & CSL_VTM_TSGLOBAL_CLK_DIV_VALID) == 0u)
            {
                tsGlobalCfgRd.clkDiv = p_tsGlobal_cfg->clkDiv;
            }        

            if ((valid_map & CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_EN_VALID) == 0u)
            {
                tsGlobalCfgRd.any_maxt_outrg_alert_en = p_tsGlobal_cfg->any_maxt_outrg_alert_en;
            }    

            if ((valid_map & CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR_VALID) == 0u)
            {
                tsGlobalCfgRd.maxt_outrg_alert_thr = p_tsGlobal_cfg->maxt_outrg_alert_thr;
            }

            if ((valid_map & CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR0_VALID) == 0u)
            {
                tsGlobalCfgRd.maxt_outrg_alert_thr0 = p_tsGlobal_cfg->maxt_outrg_alert_thr0;
            }

            if ((valid_map & CSL_VTM_TSGLOBAL_SAMPLES_PER_CNT_VALID) == 0u)
            {
                tsGlobalCfgRd.samplesPerCnt = p_tsGlobal_cfg->samplesPerCnt;
            }
        }

        if ((tsGlobalCfgRd.clkSel                   != p_tsGlobal_cfg->clkSel)     &&
            (tsGlobalCfgRd.clkDiv                   != p_tsGlobal_cfg->clkDiv)     &&             
            (tsGlobalCfgRd.any_maxt_outrg_alert_en  != p_tsGlobal_cfg->any_maxt_outrg_alert_en)     &&             
            (tsGlobalCfgRd.maxt_outrg_alert_thr     != p_tsGlobal_cfg->maxt_outrg_alert_thr)     &&             
            (tsGlobalCfgRd.maxt_outrg_alert_thr0    != p_tsGlobal_cfg->maxt_outrg_alert_thr0)     && 
            (tsGlobalCfgRd.samplesPerCnt            != p_tsGlobal_cfg->samplesPerCnt))
        {
            retVal = CSL_EFAIL;
        }

    }
    
    return (retVal);
}


/**
* Requirement: REQ_TAG(PDK-5865)
* Design: did_csl_vtm_wr_read_back did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsGetGlobalCfg (const CSL_vtm_cfg2Regs       *p_cfg2,
                               CSL_vtm_tsGlobal_cfg         *p_tsGlobal_cfg)

{
    int32_t retVal = CSL_PASS;
    CSL_vtm_tsGlobal_ctrl_valid_map valid_map;

    /* argument checks */
    if( (p_tsGlobal_cfg  == NULL_PTR)    ||
        (p_cfg2          == NULL_PTR))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        valid_map = p_tsGlobal_cfg->validMap;
        if ((valid_map & CSL_VTM_TSGLOBAL_CLK_SEL_VALID) !=0u)
        {
            p_tsGlobal_cfg->clkSel = (CSL_vtm_tsGlobal_clkSel) \
                CSL_REG32_FEXT(&p_cfg2->CLK_CTRL, VTM_CFG2_CLK_CTRL_TSENS_CLK_SEL);
        }
        if ((valid_map & CSL_VTM_TSGLOBAL_CLK_DIV_VALID) !=0u)
        {
            p_tsGlobal_cfg->clkDiv = (CSL_vtm_tsGlobal_clkDiv) \
            CSL_REG32_FEXT(&p_cfg2->CLK_CTRL, VTM_CFG2_CLK_CTRL_TSENS_CLK_DIV);
        }        

        if ((valid_map & CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_EN_VALID) !=0u)
        {
            p_tsGlobal_cfg->any_maxt_outrg_alert_en = (CSL_vtm_tsGlobal_any_maxt_outrg_alert_en) \
            CSL_REG32_FEXT(&p_cfg2->MISC_CTRL, VTM_CFG2_MISC_CTRL_ANY_MAXT_OUTRG_ALERT_EN);
        }    

        if ((valid_map & CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR_VALID) !=0u)
        {
            p_tsGlobal_cfg->maxt_outrg_alert_thr = (CSL_vtm_adc_code) \
            CSL_REG32_FEXT(&p_cfg2->MISC_CTRL2, VTM_CFG2_MISC_CTRL2_MAXT_OUTRG_ALERT_THR);
        }

        if ((valid_map & CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR0_VALID) !=0u)
        {
            p_tsGlobal_cfg->maxt_outrg_alert_thr0 = (CSL_vtm_adc_code) \
            CSL_REG32_FEXT(&p_cfg2->MISC_CTRL2, VTM_CFG2_MISC_CTRL2_MAXT_OUTRG_ALERT_THR0);
        }

        if ((valid_map & CSL_VTM_TSGLOBAL_SAMPLES_PER_CNT_VALID) !=0u)
        {
            p_tsGlobal_cfg->samplesPerCnt = (CSL_vtm_tsGlobal_samples_per_count)\
            CSL_REG32_FEXT(&p_cfg2->SAMPLE_CTRL, VTM_CFG2_SAMPLE_CTRL_SAMPLE_PER_CNT);
        }
    }
    return (retVal);
}




/**
* Requirement: REQ_TAG(PDK-5866)
* Design: did_csl_vtm_cfg_temp_warning did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsSetCtrl (const CSL_vtm_cfg2Regs        *p_cfg2,
                          CSL_vtm_tmpSens_id            temp_sensor_id,
                          const CSL_vtm_ts_ctrl_cfg     *p_tsCtrl_cfg,
                          Bool                          read_verify_flag)

{
    int32_t retVal = CSL_PASS;
    CSL_vtm_tsCtrl_valid_map                valid_map;
    CSL_vtm_tsCtrl_max_outrg_alert          maxt_outrg_alert_en;
    CSL_vtm_tsCtrl_resetCtrl                tsReset;
    CSL_vtm_tsCtrl_singleshot_conv_stat     adc_trigger;
    CSL_vtm_tsCtrl_mode                     mode;
    const CSL_vtm_cfg2Regs_TMPSENS         *p_sensor;
    CSL_vtm_ts_ctrl_cfg                     tsCtrlCfg;

    /* argument checks */
    if( (p_tsCtrl_cfg    == NULL_PTR)    ||
        (p_cfg2          == NULL_PTR)   ||
        (temp_sensor_id  > (CSL_vtm_tmpSens_id)CSL_VTM_TS_MAX_NUM))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        valid_map = p_tsCtrl_cfg->valid_map;
        p_sensor  = &p_cfg2->TMPSENS[temp_sensor_id];

        /* get the maxt_outrg_en filed */
        if (p_tsCtrl_cfg->maxt_outrg_alert_en == CSL_VTM_TS_CTRL_MAXT_OUTRG_GEN_ALERT)
        {
            maxt_outrg_alert_en = CSL_VTM_TS_CTRL_MAXT_OUTRG_GEN_ALERT;
        }
        else
        {
            maxt_outrg_alert_en = CSL_VTM_TS_CTRL_MAXT_OUTRG_NO_ALERT;
        }

        /* Get the clrz filed */
        if (p_tsCtrl_cfg->tsReset == CSL_VTM_TS_CTRL_SENSOR_RESET)
        {
            tsReset   = CSL_VTM_TS_CTRL_SENSOR_RESET;
        }
        else
        {
            tsReset  = CSL_VTM_TS_CTRL_SENSOR_NORM_OP;
        }

        /* Get the start of conversion trigger field */
        if (p_tsCtrl_cfg->adc_stat == CSL_VTM_TS_CTRL_SINGLESHOT_ADC_CONV_IN_PROGRESS)
        {
            adc_trigger = CSL_VTM_TS_CTRL_SINGLESHOT_ADC_CONV_IN_PROGRESS;
        }
        else
        {
            adc_trigger = CSL_VTM_TS_CTRL_SINGLESHOT_ADC_CONV_COMPLETE;
        }

        /* Get the temperature sensor mode of operation */
        if (p_tsCtrl_cfg->mode == CSL_VTM_TS_CTRL_CONTINUOUS_MODE)
        {
            mode = CSL_VTM_TS_CTRL_CONTINUOUS_MODE;
        }
        else
        {
            mode = CSL_VTM_TS_CTRL_SINGLESHOT_MODE;
        }

        /* update the fileds that are valid */
        if ((valid_map & CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_MAXT_OUTRG_EN, maxt_outrg_alert_en);
        }

        if ((valid_map & CSL_VTM_TS_CTRL_RESET_CTRL_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_CLRZ, tsReset);
        }

        if ((valid_map & CSL_VTM_TS_CTRL_SOC_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_SOC, adc_trigger);
        }

        if ((valid_map & CSL_VTM_TS_CTRL_MODE_VALID) !=0u)
        {
            CSL_REG32_FINS(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_CONT, mode);        
        }
    }

    if ((read_verify_flag == TRUE) &&
        (retVal == CSL_PASS))
    {
        tsCtrlCfg.valid_map = valid_map;

        /* read the Control */
        retVal = CSL_vtmTsGetCtrl(p_cfg2, temp_sensor_id, &tsCtrlCfg);

        if (retVal == CSL_PASS)
        {
            /* Get the values for those which are not valid */
            if ((valid_map & CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID) ==0u)
            {
                tsCtrlCfg.maxt_outrg_alert_en = p_tsCtrl_cfg->maxt_outrg_alert_en;
            }
            
            if ((valid_map & CSL_VTM_TS_CTRL_RESET_CTRL_VALID) ==0u)
            {
                tsCtrlCfg.tsReset = p_tsCtrl_cfg->tsReset;
            }

            /* Always set the ADC_STAT to configured value as it can change
             * for a given read */
            tsCtrlCfg.adc_stat = p_tsCtrl_cfg->adc_stat;
            
            if ((valid_map & CSL_VTM_TS_CTRL_MODE_VALID) ==0u)
            {
                tsCtrlCfg.mode = p_tsCtrl_cfg->mode;
            }

            if ((tsCtrlCfg.maxt_outrg_alert_en  != p_tsCtrl_cfg->maxt_outrg_alert_en)  &&
                (tsCtrlCfg.tsReset              != p_tsCtrl_cfg->tsReset) &&
                (tsCtrlCfg.mode                 != p_tsCtrl_cfg->mode))
            {
                retVal = CSL_EFAIL;
            }
        }
    }
    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5865)
* Design: did_csl_vtm_wr_read_back did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsGetCtrl (const CSL_vtm_cfg2Regs      *p_cfg2,
                          CSL_vtm_tmpSens_id          temp_sensor_id,      
                          CSL_vtm_ts_ctrl_cfg         *p_tsCtrl_cfg)

{
    int32_t retVal = CSL_PASS;
    CSL_vtm_tsCtrl_valid_map           valid_map;
    const CSL_vtm_cfg2Regs_TMPSENS     *p_sensor;

    /* argument checks */
    if( (p_tsCtrl_cfg    == NULL_PTR)   ||
        (p_cfg2          == NULL_PTR)   ||
        (temp_sensor_id  > (CSL_vtm_tmpSens_id) CSL_VTM_TS_MAX_NUM))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        valid_map = p_tsCtrl_cfg->valid_map;
        p_sensor  = &p_cfg2->TMPSENS[temp_sensor_id];

        /* Get the fileds that are valid */
        if ((valid_map & CSL_VTM_TS_CTRL_MAXT_OUTG_ALERT_VALID) !=0u)
        {
            p_tsCtrl_cfg->maxt_outrg_alert_en = (CSL_vtm_tsCtrl_max_outrg_alert) \
                    CSL_REG32_FEXT(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_MAXT_OUTRG_EN);
        }

        if ((valid_map & CSL_VTM_TS_CTRL_RESET_CTRL_VALID) !=0u)
        {
             p_tsCtrl_cfg->tsReset = (CSL_vtm_tsCtrl_resetCtrl) \
                    CSL_REG32_FEXT(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_CLRZ);
        }

        if ((valid_map & CSL_VTM_TS_CTRL_SOC_VALID) !=0u)
        {
             p_tsCtrl_cfg->adc_stat = (CSL_vtm_tsCtrl_singleshot_conv_stat) \
                    CSL_REG32_FEXT(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_SOC);
        }

        if ((valid_map & CSL_VTM_TS_CTRL_MODE_VALID) !=0u)
        {
             p_tsCtrl_cfg->mode = (CSL_vtm_tsCtrl_mode) \
                    CSL_REG32_FEXT(&p_sensor->CTRL, VTM_CFG2_TMPSENS_CTRL_CONT);        
        }
    }
    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5865)
* Design: did_csl_vtm_wr_read_back did_csl_vtm_interfaces
*/
int32_t CSL_vtmGetVTMInfo (const CSL_vtm_cfg1Regs     *p_cfg,
                           CSL_vtm_info         *p_vtm_info)

{
    int32_t retVal = CSL_EBADARGS;

    /* argument checks */
    if((p_cfg != NULL_PTR) &&
       (p_vtm_info != NULL_PTR))
    {
        p_vtm_info->pid                 = p_cfg->PID;
        p_vtm_info->vtm_vd_map          = (uint8_t) CSL_REG32_FEXT(&p_cfg->DEVINFO_PWR0, \
                                              VTM_CFG1_DEVINFO_PWR0_VTM_VD_MAP);
        p_vtm_info->vd_rtc              = (uint8_t) CSL_REG32_FEXT(&p_cfg->DEVINFO_PWR0, \
                                              VTM_CFG1_DEVINFO_PWR0_VDD_RTC);
        p_vtm_info->temp_sensors_cnt    = (uint8_t) CSL_REG32_FEXT(&p_cfg->DEVINFO_PWR0, \
                                              VTM_CFG1_DEVINFO_PWR0_TMPSENS_CT);
        p_vtm_info->cvd_cnt             = (uint8_t) CSL_REG32_FEXT(&p_cfg->DEVINFO_PWR0, \
                                              VTM_CFG1_DEVINFO_PWR0_CVD_CT);
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5865)
* Design: did_csl_vtm_wr_read_back did_csl_vtm_interfaces
*/
int32_t CSL_vtmTsGetSensorStat (const CSL_vtm_cfg1Regs             *p_cfg,
                                const CSL_vtm_tsStat_read_ctrl     *p_ctrl,
                                CSL_vtm_tmpSens_id                  temp_sensor_id,
                                CSL_vtm_tsStat_val                 *p_ts_stat_val )

{
    int32_t                                 retVal = CSL_PASS;
    CSL_vtm_tsStat_read_ctrl                ctrl;
    const CSL_vtm_cfg1Regs_TMPSENS          *p_sensor;

    /* argument checks */
    if( (p_ctrl          == NULL_PTR)                ||
        (p_cfg           == NULL_PTR)                ||
        (p_ts_stat_val   == NULL_PTR)                ||
        (temp_sensor_id  > (CSL_vtm_tmpSens_id)CSL_VTM_TS_MAX_NUM))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        p_sensor = &p_cfg->TMPSENS[temp_sensor_id];
        ctrl     = *p_ctrl;
        if ((ctrl & CSL_VTM_TS_READ_VD_MAP_VAL) != 0u)
        {
            p_ts_stat_val->vd_map = (CSL_vtm_ts_stat_vd_map) \
                                    CSL_REG32_FEXT(&p_sensor->STAT, \
                                                   VTM_CFG1_TMPSENS_STAT_VD_MAP);
        }

        if ((ctrl & CSL_VTM_TS_READ_ALL_THRESHOLD_ALERTS) != 0u)
        {
            p_ts_stat_val->lt_th0_alert = (uint8_t) CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_LT_TH0_ALERT);
            p_ts_stat_val->gt_th1_alert = (uint8_t) CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_GT_TH1_ALERT);
            p_ts_stat_val->gt_th2_alert = (uint8_t) CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_GT_TH2_ALERT);
            p_ts_stat_val->maxt_outrg_alert = (uint8_t) CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_MAXT_OUTRG_ALERT);
        }

        if ((ctrl & CSL_VTM_TS_READ_FIRST_TIME_EOC_BIT) != 0u)
        {
            p_ts_stat_val->soc_fc_update = (uint8_t) CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_EOC_FC_UPDATE);
        }

        if ((ctrl & CSL_VTM_TS_READ_DATA_VALID_BIT) != 0u)
        {
            p_ts_stat_val->data_valid = (uint8_t) CSL_REG32_FEXT(&p_sensor->STAT, \
                VTM_CFG1_TMPSENS_STAT_DATA_VALID);
        }

        if ((ctrl & CSL_VTM_TS_READ_DATA_OUT_VAL) != 0u)
        {
            p_ts_stat_val->data_out = CSL_vtmGetAdcCode(p_sensor);
        }

    }

    return (retVal);
}

/**
* Requirement: REQ_TAG(PDK-5863)
* Design: did_csl_vtm_static_read_back did_csl_vtm_interfaces
*/

int32_t CSL_vtmReadBackStaticRegisters( const CSL_vtm_cfg1Regs      *p_cfg1,
                                        const CSL_vtm_cfg2Regs      *p_cfg2,
                                        CSL_vtm_staticRegs          *p_static_regs)
{
    int32_t                      retVal = CSL_EBADARGS;
    CSL_vtm_tmpSens_id           ts_id;
    CSL_vtm_vd_id                vd_id;

    /* arg checked */
    if ((p_cfg1         != NULL_PTR) &&
        (p_cfg2         != NULL_PTR) &&
        (p_static_regs  != NULL_PTR))
    {
        retVal = CSL_PASS;
    }

    /* if good args are passed */
    if(retVal == CSL_PASS)
    {
        /* get number of VD, temperature sensors */
        if (gNumTempSensors == CSL_VTM_VALUES_ARE_UNINITIALIZED)
        {
            CSL_vtmGetSensorVDCount(p_cfg1);
        }

        /* Read all elements */
        p_static_regs->vtm_global_cfg.validMap =  \
                                 (CSL_VTM_TSGLOBAL_CLK_SEL_VALID                 |
                                  CSL_VTM_TSGLOBAL_CLK_DIV_VALID                 |
                                  CSL_VTM_TSGLOBAL_ANY_MAXT_OUTRG_ALERT_EN_VALID |
                                  CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR0_VALID   |
                                  CSL_VTM_TSGLOBAL_MAXT_OUTRG_ALERT_THR_VALID    |
                                  CSL_VTM_TSGLOBAL_SAMPLES_PER_CNT_VALID );

        /* did not check p_cfg2 arg as it would be checked in below call */
        retVal = CSL_vtmTsGetGlobalCfg(p_cfg2, &p_static_regs->vtm_global_cfg);
    }

    /* proceed if previous operation is good */
    if (retVal == CSL_PASS)
    {
        /* Read the individual temperature sensor control values */
        for (ts_id = ((CSL_vtm_tmpSens_id)CSL_VTM_TS_ID_0); ts_id < (CSL_vtm_tmpSens_id)gNumTempSensors; ts_id++)
        {
            p_static_regs->vtm_ts_ctrl2[ts_id] = p_cfg2->TMPSENS[ts_id].CTRL;
            p_static_regs->vtm_ts_ctrl[ts_id]  = p_cfg1->TMPSENS[ts_id].CTRL;
            p_static_regs->vtm_ts_th[ts_id]    = p_cfg1->TMPSENS[ts_id].TH;
            p_static_regs->vtm_ts_th2[ts_id]   = p_cfg1->TMPSENS[ts_id].TH2;            
        }

        /* remaining temperature sensor control values are zeroed */
        for (ts_id = (CSL_vtm_tmpSens_id)gNumTempSensors; ts_id < ((CSL_vtm_tmpSens_id)CSL_VTM_TS_MAX_NUM); ts_id++)
        {
            p_static_regs->vtm_ts_ctrl2[ts_id] = 0U;
            p_static_regs->vtm_ts_ctrl[ts_id]  = 0U;
            p_static_regs->vtm_ts_th[ts_id]    = 0U;
            p_static_regs->vtm_ts_th2[ts_id]   = 0U; 
        }

        /* Read the individual voltage domain event selection and opp vid values */
        for (vd_id = ((CSL_vtm_vd_id)CSL_VTM_VD_DOMAIN_0); vd_id < (CSL_vtm_vd_id)gNumCoreVoltageDomains; vd_id++)
        {
            p_static_regs->vtm_vd_evt_sel_ctrl[vd_id] = p_cfg1->VD[vd_id].EVT_SEL_SET;
            p_static_regs->vtm_vd_opp_vid[vd_id]      = p_cfg1->VD[vd_id].OPPVID;
        }

        /* remaining VD event selection and OPP VID values are zeroed */
        for (vd_id = (CSL_vtm_vd_id)gNumCoreVoltageDomains; vd_id < ((CSL_vtm_vd_id)CSL_VTM_VD_DOMAIN_CNT); vd_id++)
        {
            p_static_regs->vtm_vd_evt_sel_ctrl[vd_id] = 0U;
            p_static_regs->vtm_vd_opp_vid[vd_id]      = 0U;
        }        
    }

    return (retVal);
}


/* Nothing past this point */
 
