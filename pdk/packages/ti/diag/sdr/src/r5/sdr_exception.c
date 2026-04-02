/*
 * SDR EXCEPTION
 *
 * Software Diagnostics Reference module for handling exceptions
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

#include <stddef.h>
#include <stdbool.h>

#include <sdr_esm.h>
#include <sdr_mpu.h>
#include <sdr_utils.h>

#include <sdr_exception.h>
#include <sdr_exception_priv.h>

/* Local defines */

#define SDR_EXCEPTION_SYNC_PARITY_OR_ECC_ERROR_MASK 0x409u
#define SDR_EXCEPTION_ASYNC_PARITY_OR_ECC_ERROR_MASK 0x408u
#define SDR_EXCEPTION_MPU_WRITE_PERMISSION_FAULT_MASK      (0x080DU)
#define SDR_EXCEPTION_MPU_READ_PERMISSION_FAULT_MASK       (0x000DU)

#ifdef mcu1_0
#define SDR_LOCAL_EXCEPTION_DED_ERROR SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR
#else
#define SDR_LOCAL_EXCEPTION_DED_ERROR SDR_ESM_ECC_PARAM_MCU_CPU1_DED_ERROR
#endif

void SDR_EXCEPTION_undefInstructionExptnHandler(void *param);
void SDR_EXCEPTION_swIntrExptnHandler(void *param);
void SDR_EXCEPTION_prefetchAbortExptnHandler(void *param);
void SDR_EXCEPTION_dataAbortExptnHandler(void *param);
void SDR_EXCEPTION_irqExptnHandler(void *param);
void SDR_EXCEPTION_fiqExptnHandler(void *param);
extern bool SDR_MPU_exceptionChkSelfTest(SDR_MPU_memAccessType faultType,
                                        uint32_t              faultAddr);

static SDR_EXCEPTION_Instance_t SDR_EXCEPTION_instance;

/*********************************************************************
 * @fn      SDR_exceptionInit
 *
 * @brief   Initialise Exception module
 *
 * @param1  callbackFunctions: Pointer to callback Functions
 *
 * @return  None
 */
void SDR_EXCEPTION_init(const SDR_EXCEPTION_CallbackFunctions_t *callbackFunctions)
{
    /* Store exception callback functions */
    SDR_EXCEPTION_instance.callbackFunctions = *callbackFunctions;
    /* Initialize ECC Callback Function */
    SDR_EXCEPTION_instance.ECCCallBackFunction = NULL;
    return;
}

/*********************************************************************
 * @fn      SDR_exceptionRegisterECCHandler
 *
 * @brief   Function to register ECC exception handler
 *
 * @param1  ECCCallBackFunctionPtr: callback function to register
 *
 * @return  None
 */
void SDR_EXCEPTION_registerECCHandler(SDR_EXCEPTION_ECCCallback_t ECCCallBackFunctionPtr)
{
    /* Store call back function */
    SDR_EXCEPTION_instance.ECCCallBackFunction = ECCCallBackFunctionPtr;

    return;
}


/*********************************************************************
 * @fn      SDR_undefInstructionExptnHandler
 *
 * @brief   SDR exception handler for undefined instructions
 *
 * @param1  param: parameter pointer
 *
 * @return  None
 */
void SDR_EXCEPTION_undefInstructionExptnHandler(void *param)
{

    /* Store param pointer */
    SDR_EXCEPTION_instance.paramPtr = param;

    /* Execute callback function */
    if ((void *)SDR_EXCEPTION_instance.callbackFunctions.udefExptnCallback != ((void *)0u)) {
        SDR_EXCEPTION_instance.callbackFunctions.udefExptnCallback();
    }

}

/*********************************************************************
 * @fn      SDR_swIntrExptnHandler
 *
 * @brief   SDR exception handler for Software interrupts
 *
 * @param1  param: parameter pointer
 *
 * @return  None
 */
void SDR_EXCEPTION_swIntrExptnHandler(void *param)
{
    /* Store param pointer */
    SDR_EXCEPTION_instance.paramPtr = param;


    /* Execute callback function */
    if ((void *)SDR_EXCEPTION_instance.callbackFunctions.swiExptnCallback != ((void *)0u)) {
        SDR_EXCEPTION_instance.callbackFunctions.swiExptnCallback();
    }
}

/*********************************************************************
 * @fn      SDR_prefetchAbortExptnHandler
 *
 * @brief   SDR exception handler for Prefetch abort
 *
 * @param1  param: parameter pointer
 *
 * @return  None
 */
void SDR_EXCEPTION_prefetchAbortExptnHandler(void *param)
{
    uint32_t ifsrValue;
    uint32_t ifarValue;

    /* Store param pointer */
    SDR_EXCEPTION_instance.paramPtr = param;

    /* Handle any instruction abort due to ECC errors */
    /* read IFSR value */
    ifsrValue = SDR_UTILS_getIFSR();
    if ((ifsrValue & SDR_EXCEPTION_SYNC_PARITY_OR_ECC_ERROR_MASK)
        == SDR_EXCEPTION_SYNC_PARITY_OR_ECC_ERROR_MASK) {
        ifarValue = SDR_UTILS_getIFAR();
        /* Execute callback function */
        if ((void *)SDR_EXCEPTION_instance.ECCCallBackFunction != ((void *)0u)) {
            SDR_EXCEPTION_instance.ECCCallBackFunction(
                SDR_LOCAL_EXCEPTION_DED_ERROR,
                ifarValue,
                SDR_ESM_ERRORRAMID_INVALID,
                SDR_ESM_ERRORBITOFFSET_INVALID,
                SDR_ESM_ERRORBITGROUP_INVALID);
        }
    } else if ((ifsrValue & SDR_EXCEPTION_ASYNC_PARITY_OR_ECC_ERROR_MASK)
            == SDR_EXCEPTION_ASYNC_PARITY_OR_ECC_ERROR_MASK) {
             /* Execute callback function */
            if ((void *)SDR_EXCEPTION_instance.ECCCallBackFunction != ((void *)0u)) {
                SDR_EXCEPTION_instance.ECCCallBackFunction(
                    SDR_LOCAL_EXCEPTION_DED_ERROR,
                    SDR_ESM_ERRORADDR_INVALID,
                    SDR_ESM_ERRORRAMID_INVALID,
                    SDR_ESM_ERRORBITOFFSET_INVALID,
                    SDR_ESM_ERRORBITGROUP_INVALID);
            }
    } else {
        /* Execute callback function */
        if ((void *)SDR_EXCEPTION_instance.callbackFunctions.pabtExptnCallback != ((void *)0u)) {
            SDR_EXCEPTION_instance.callbackFunctions.pabtExptnCallback();
        }
    }
}

/*********************************************************************
 * @fn      SDR_dataAbortExptnHandler
 *
 * @brief   SDR exception handler for Data abort
 *
 * @param1  param: parameter pointer
 *
 * @return  None
 */
void SDR_EXCEPTION_dataAbortExptnHandler(void *param)
{
   uint32_t dfsrValue;
   uint32_t dfarValue;
   bool     isMPUSelfTest;

   /* Store last param pointer */
   SDR_EXCEPTION_instance.paramPtr = param;

   /* Handle any data abort due to ECC errors */
   /* read DFSR value */
   dfsrValue = SDR_UTILS_getDFSR();
   if ((dfsrValue & SDR_EXCEPTION_SYNC_PARITY_OR_ECC_ERROR_MASK)
       == SDR_EXCEPTION_SYNC_PARITY_OR_ECC_ERROR_MASK) {
       dfarValue = SDR_UTILS_getDFAR();
       /* Execute callback function */
       if ((void *)SDR_EXCEPTION_instance.ECCCallBackFunction != ((void *)0u)) {
           SDR_EXCEPTION_instance.ECCCallBackFunction(
               SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR,
               dfarValue,
               SDR_ESM_ERRORRAMID_INVALID,
               SDR_ESM_ERRORBITOFFSET_INVALID,
               SDR_ESM_ERRORBITGROUP_INVALID);
       }
   } else if ((dfsrValue & SDR_EXCEPTION_ASYNC_PARITY_OR_ECC_ERROR_MASK)
           == SDR_EXCEPTION_ASYNC_PARITY_OR_ECC_ERROR_MASK) {
            /* Execute callback function */
           if ((void *)SDR_EXCEPTION_instance.ECCCallBackFunction != ((void *)0u)) {
               SDR_EXCEPTION_instance.ECCCallBackFunction(
                   SDR_ESM_ECC_PARAM_MCU_CPU0_DED_ERROR,
                   SDR_ESM_ERRORADDR_INVALID,
                   SDR_ESM_ERRORRAMID_INVALID,
                   SDR_ESM_ERRORBITOFFSET_INVALID,
                   SDR_ESM_ERRORBITGROUP_INVALID);
           }
   } else {
       /* Get the fault address */
       dfarValue = SDR_UTILS_getDFAR();
       if ((dfsrValue & SDR_EXCEPTION_MPU_WRITE_PERMISSION_FAULT_MASK) 
                  == SDR_EXCEPTION_MPU_WRITE_PERMISSION_FAULT_MASK) {
              /* Call the MPU exception check function to check if it is
               * due to write permission fault from self test, 
               * if yes, correct the fault to proceed the self test */
               isMPUSelfTest = SDR_MPU_exceptionChkSelfTest(SDR_MPU_DATA_WRITE_ACCESS,
                                                             dfarValue);
       }
       else {
              /* Call the MPU exception check function to check if it is
               * due to read permission fault from self test, 
               * if yes, correct the fault to proceed the self test */
               isMPUSelfTest = SDR_MPU_exceptionChkSelfTest(SDR_MPU_DATA_READ_ACCESS,
                                                             dfarValue);
       }

       /* Execute callback function */
       if ((!isMPUSelfTest) &&
           ((void *)SDR_EXCEPTION_instance.callbackFunctions.dabtExptnCallback != ((void *)0u))) {
           SDR_EXCEPTION_instance.callbackFunctions.dabtExptnCallback();
       }
   }
}

/*********************************************************************
 * @fn      SDR_irqExptnHandler
 *
 * @brief   SDR exception handler for IRQ
 *
 * @param1  param: parameter pointer
 *
 * @return  None
 */
void SDR_EXCEPTION_irqExptnHandler(void *param)
{
    /* Store param pointer */
    SDR_EXCEPTION_instance.paramPtr = param;

    /* Execute callback function */
    if ((void *)SDR_EXCEPTION_instance.callbackFunctions.irqExptnCallback != ((void *)0u)) {
        SDR_EXCEPTION_instance.callbackFunctions.irqExptnCallback();
    }
}

/*********************************************************************
 * @fn      SDR_fiqExptnHandler
 *
 * @brief   SDR exception handler for FIQ
 *
 * @param1  param: parameter pointer
 *
 * @return  None
 */
void SDR_EXCEPTION_fiqExptnHandler(void *param)
{
    /* Store param pointer */
    SDR_EXCEPTION_instance.paramPtr = param;

    /* Execute callback function */
    if ((void *)SDR_EXCEPTION_instance.callbackFunctions.fiqExptnCallback != ((void *)0u)) {
        SDR_EXCEPTION_instance.callbackFunctions.fiqExptnCallback();
    }
}
