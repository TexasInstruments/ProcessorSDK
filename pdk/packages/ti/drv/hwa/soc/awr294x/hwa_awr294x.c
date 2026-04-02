/*
 * Copyright (c) 2019 - 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 /*
  *  ======== hwa_awr294x.c ========
  */

#include <stdint.h>

#include <ti/osal/osal.h>
#include <ti/drv/hwa/hwa.h>
#include <ti/drv/hwa/src/V1/hwa_internal.h>
#include <ti/drv/hwa/soc/hwa_soc.h>



#define NUM_HWA_PARAMSETS_PER_INSTANCE          SOC_HWA_NUM_PARAM_SETS
#define NUM_HWA_DMACHANNELS_PER_INSTANCE        SOC_HWA_NUM_DAM_CHANNEL

/* storage for HWA driver object handles */
HWA_Driver *gHWADriverPtr[HWA_NUM_INSTANCES] = { NULL };

HWA_RAMAttrs gHWARAMCfg[HWA_NUM_RAMS] = {
    { CSL_DSS_HWA_WINDOW_RAM_U_BASE, CSL_DSS_HWA_WINDOW_RAM_U_SIZE },
    { CSL_DSS_HWA_MULT_RAM_U_BASE, CSL_DSS_HWA_MULT_RAM_U_SIZE },
    { CSL_DSS_HWA_DEROT_RAM_U_BASE, CSL_DSS_HWA_DEROT_RAM_U_SIZE } ,
    { CSL_DSS_HWA_SHUFFLE_RAM_U_BASE,CSL_DSS_HWA_SHUFFLE_RAM_U_SIZE } ,
    { CSL_DSS_HWA_HIST_THRESH_RAM_U_BASE, CSL_DSS_HWA_HIST_THRESH_RAM_U_SIZE } ,
    { CSL_DSS_HWA_2DSTAT_ITER_VAL_RAM_U_BASE, CSL_DSS_HWA_2DSTAT_ITER_VAL_RAM_U_SIZE } ,
    { CSL_DSS_HWA_2DSTAT_ITER_IDX_RAM_U_BASE, CSL_DSS_HWA_2DSTAT_ITER_IDX_RAM_U_SIZE },
    { CSL_DSS_HWA_2DSTAT_SMPL_VAL_RAM_U_BASE, CSL_DSS_HWA_2DSTAT_SMPL_VAL_RAM_U_SIZE } ,
    { CSL_DSS_HWA_2DSTAT_SMPL_IDX_RAM_U_BASE, CSL_DSS_HWA_2DSTAT_SMPL_IDX_RAM_U_SIZE } ,
    { CSL_DSS_HWA_HIST_RAM_U_BASE, CSL_DSS_HWA_HIST_RAM_U_SIZE }

};
/* AWR294X DSS/MSS specific HWA hardware attributes */
HWA_HWAttrs gHWAHwCfg[HWA_NUM_INSTANCES] =
{
    /* HWA1 Hardware configuration:
     */
    {
        0U,
        CSL_DSS_HWA_CFG_U_BASE,                               /* HWA base address for control/common register */
        CSL_DSS_HWA_PARAM_U_BASE,                          /* HWA base address for the paramset */
        CSL_DSS_HWA_WINDOW_RAM_U_BASE,            /* HWA base address for window ram */
        CSL_DSS_CTRL_U_BASE,                                       /* DSS_CTRL base address */
        NUM_HWA_PARAMSETS_PER_INSTANCE,       /* number of HWA paramsets */
#if defined (_TMS320C6X)
        CSL_DSS_INTR_DSS_HWA_PARAM_DONE_INTR1,    /* intNumParamSet HWA Peripheral's interrupt vector 1 for individual paramset completion */
        CSL_DSS_INTR_DSS_HWA_PARAM_DONE_INTR2,    /* intNumParamSet HWA Peripheral's interrupt vector 2 for individual paramset completion */
        CSL_DSS_INTR_DSS_HWA_LOOP_INTR1,                  /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset */
        CSL_DSS_INTR_DSS_HWA_LOOP_INTR2,                  /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset in ALT thread */
#endif
#if defined (__TI_ARM_V7R4__)
        CSL_MSS_INTR_DSS_HWA_PARAM_DONE_INTR1,    /* intNumParamSet HWA Peripheral's interrupt vector 1 for individual paramset completion */
        CSL_MSS_INTR_DSS_HWA_PARAM_DONE_INTR2,    /* intNumParamSet HWA Peripheral's interrupt vector 2 for individual paramset completion */
        CSL_MSS_INTR_DSS_HWA_LOOP_INTR1,           /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset */
        CSL_MSS_INTR_DSS_HWA_LOOP_INTR2,       /* intNumDone HWA Peripheral's interrupt vector for completion of all programmed paramset in ALT thread  */
#endif
        NUM_HWA_DMACHANNELS_PER_INSTANCE,          /* number of DMA channels available for HWA */
        CSL_DSS_HWA_DMA0_U_BASE,                     /* HWA Accelerator processing memory base address */
        SOC_HWA_MEM_SIZE,                                /* HWA Accelerator processing memory size in bytes */
        true
    }
};
