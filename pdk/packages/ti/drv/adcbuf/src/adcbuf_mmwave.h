/**
 *   @file  adcbuf_mmwave.h
 *
 *   @brief
 *      This is the internal Header for mmWave ADCBUF Driver. This header
 *      file should *NOT* be directly included by applications.
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

#ifndef ADCBUF_MMWAVE_H
#define ADCBUF_MMWAVE_H

#include <ti/osal/HwiP.h>
#include <ti/osal/DebugP.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/csl/soc.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    ADCBUF Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef const struct ADCBuf_HwCfg_t
{
    /**
     * @brief   Base address of the DSS_REG registers.
     */
	CSL_rss_ctrlRegs*  ptrRSSCtrlRegBase;

    /**
     * @brief   Interrupt Number
     */
    uint32_t           interruptNum;

    /**
     * @brief   ADC Buffer base address
     */
    uint32_t           ptrADCBUFBaseAddr;

    /**
     * @brief   ADC Buffer base address
     */
    uint32_t           ptrCQBUFBaseAddr;
}ADCBuf_HwCfg;

/**
 *  @brief      ADCBufMMWave Object
 *
 *  The application must not access any member variables of this structure!
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBufMMWave_Object_t
{
    /**
      * @brief   Status of ADCBUF driver.
     */
    bool                   isOpen;

     /**
      * @brief   Base address of DSS Register Base.
     */
    CSL_rss_ctrlRegs       *ptrRssCtrlRegBase;

} ADCBufMMWave_Object, *ADCBufMMWave_Handle;

/* External Functions */
void ADCBUF_MMWave_init(ADCBuf_Handle handle);
ADCBuf_Handle ADCBUF_MMWave_open(ADCBuf_Handle handle, const ADCBuf_Params *params);
void ADCBUF_MMWave_close(ADCBuf_Handle handle);
int_fast16_t ADCBUF_MMWave_control(ADCBuf_Handle handle, uint_fast8_t cmd, void * arg);
uint32_t ADCBUF_MMWave_getChanBufAddr(ADCBuf_Handle handle, uint8_t channel, int32_t *errCode);

#ifdef __cplusplus
}
#endif

#endif /* ADCBUF_MMWAVE_H */
