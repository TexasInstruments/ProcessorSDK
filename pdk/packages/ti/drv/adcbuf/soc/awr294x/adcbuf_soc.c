/**
 *   @file  adcbuf_soc.c
 *
 *   @brief
 *      ADCBUF configuration: This file is provided as a sample
 *      file but it can be customized by application developers to
 *      meet their application requirements.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2021 Texas Instruments, Inc.
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
#include <ti/drv/adcbuf/adcbuf.h>
#include <ti/drv/adcbuf/src/adcbuf_mmwave.h>

#define RSS_ADCBUF_DFE_CQ_BASE  (0xA2060000U)

#if defined (__TI_ARM_V7R4__)
#define MSS_RSS_ADC_CAPTURE_COMPLETE  (159U)
#endif

#if defined (_TMS320C6X)
#define DSS_RSS_ADC_CAPTURE_COMPLETE  (57U)
#endif

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

const ADCBuf_FxnTable gADCBufFxnTable = {
    /*! Function to close the specified peripheral */
    &ADCBUF_MMWave_close,
    /*! Function to driver implementation specific control function */
    &ADCBUF_MMWave_control,
    /*! Function to initialize the given data object */
    &ADCBUF_MMWave_init,
    /*! Function to open the specified peripheral */
    &ADCBUF_MMWave_open,
    /*! Function to get ADCBuf channel address */
    &ADCBUF_MMWave_getChanBufAddr
};

/**
 * @brief   This is AWR2944 ADCBUF specific configuration for MSS/DSS Subsystem.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for MSS: */
    {
        ((CSL_rss_ctrlRegs *)CSL_RSS_CTRL_U_BASE),

        #if defined (__TI_ARM_V7R4__)
		MSS_RSS_ADC_CAPTURE_COMPLETE,
        #endif

        #if defined (_TMS320C6X)
		DSS_RSS_ADC_CAPTURE_COMPLETE,
        #endif
		CSL_RSS_ADCBUF_READ_U_BASE,
		RSS_ADCBUF_DFE_CQ_BASE
    }
};

/**
 * @brief   There is only one ADCBUF .
 */
ADCBuf_Config ADCBuf_config[2] =
{
    /* ADCBuf driver config */
    {
        &gADCBufFxnTable,                      /* ADCBUF Driver Function Table            */
        (void *)NULL,                          /* ADCBUF Driver Object:                   */
        (void const *)&gADCBufHwCfg[0]         /* ADCBUF Hw configuration:                */
    },

    /* Dummy ADCBuf driver config */
    {
        NULL,                                  /* ADCBUF Driver Function Table            */
        NULL,                                  /* ADCBUF Driver Object:                   */
        NULL                                   /* ADCBUF Hw configuration:                */
    }
};

