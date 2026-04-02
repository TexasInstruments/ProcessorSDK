/*
 * Copyright (C) 2016-2019 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
#include "ti/osal/osal.h"
#include "ti/osal/TaskP.h"

#include <ti/board/board.h>

#if defined (idkAM572x) || defined (evmAM572x)
#include <ti/csl/soc/am572x/src/csl_device_xbar.h>
#endif

#include "ti/drv/usb/example/common/hardware.h"

#include <ti/drv/usb/usb_drv.h>
#include <ti/drv/usb/usb_osal.h>

#include "types.h"

#include "usb_drv.h"
#include "usblib.h"
#include "usbdaudio.h"
#include "usb_audio_structs.h"

#if defined (evmAM335x) || defined(evmOMAPL137) || defined(lcdkOMAPL138) \
        || defined(lcdkC6748)
#include "timer.h"
#endif

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

#define MUTE_VOLUME     (0)
#define NON_SECURE_MODE FALSE

#define SOC_CACHELINE_SIZE_MAX               (64)

#if (defined (evmAM335x)||defined(evmAM437x) || defined(evmOMAPL137)) || defined(lcdkOMAPL138) \
         || defined (lcdkC6748)
/* only USB#0 on AM437EVM can be USB DEV */
#define USB_DEV_INSTANCE                     0
#else
/* on idkAM572x, USB1 is attached to a USB device connector */
#define USB_DEV_INSTANCE                     1
#endif

/* Test application stack size */
#define APP_TSK_STACK_MAIN         (0x1f00U)
/* ========================================================================== */
/*                 Global Variables                                           */
/* ========================================================================== */
SemaphoreP_Handle hSemEventHandler = NULL;
uint32_t gs_Volume = 0;
Bool gusVolumeMute = TRUE;

/* Test application stack */
static uint8_t gTskStackMain[APP_TSK_STACK_MAIN];
/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */
void usbdIntrConfig(USB_Handle usbHandle, USB_Params* usbDevParams);
void consolePrintf(const char *pcString, ...);
void usbMiscIntrHandler(uint32_t* pUsbParam);
void usbCoreIntrHandler(uint32_t* pUsbParam);

void audio_buffer_init(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

extern void mcaspUpdateVolume(uint8_t volume);

//*****************************************************************************
// task function
//
//*****************************************************************************
void taskFxn(void * a0, void * a1)
{
    USB_Params  usb_dev_params;
    USB_Handle  usb_handle;

    ///consolePrintf("\nRTOS USB Dev AC example!!\n");
    printf("\nRTOS USB Device Audio Class Example!!\n");

    usb_dev_params.usbMode = USB_DEVICE_AC_MODE;
    usb_dev_params.instanceNo = USB_DEV_INSTANCE;  /* USB port # */
    usb_dev_params.usbClassData = (void*)&g_sAudioDevice;

    usb_handle = USB_open(usb_dev_params.instanceNo, &usb_dev_params);

    if (usb_handle == 0)
    {
        ///consolePrintf("Failed to open USB driver\n");
        printf("Failed to open USB driver\n");
        UART_printStatus("\n Some tests have failed.\n");
        while(1);
    }

    usbdIntrConfig(usb_handle, &usb_dev_params);

    ///consolePrintf("Done configuring USB and its interrupt. Example is ready!\n");
    printf("Done configuring USB and its interrupt. Example is ready!\n");

    while(1)
    {
        /* Main while loop. All USB dev events are handled in interrupt context */

        /* Pend forever until a volume change is requested by the HOST */
        SemaphoreP_pend(hSemEventHandler, BIOS_WAIT_FOREVER );

        /* Update Codec Volume */
        if(gusVolumeMute)
        {
            mcaspUpdateVolume(MUTE_VOLUME);
        }
        else
        {
            mcaspUpdateVolume(gs_Volume);
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                 Internal Function Definitions                              */
/* -------------------------------------------------------------------------r- */
void usbdIntrConfig(USB_Handle usbHandle, USB_Params* usbDevParams)
{
#if defined(_TMS320C6X)
    HwiP_Handle hwiHandle = NULL;
    OsalRegisterIntrParams_t interruptRegParams;

    /* Initialize with defaults */
    Osal_RegisterInterrupt_initParams(&interruptRegParams);

    /* Populate the interrupt parameters */
    interruptRegParams.corepacConfig.name=NULL;
    interruptRegParams.corepacConfig.corepacEventNum=SYS_INT_USB0; /* Event going in to CPU */
    interruptRegParams.corepacConfig.intVecNum= OSAL_REGINT_INTVEC_EVENT_COMBINER; /* Host Interrupt vector */
    interruptRegParams.corepacConfig.isrRoutine = (void (*)(uintptr_t))usbCoreIntrHandler;
    interruptRegParams.corepacConfig.arg = (uintptr_t)usbDevParams;

    Osal_RegisterInterrupt(&interruptRegParams,&hwiHandle);
#else
    HwiP_Params hwiInputParams;
    int32_t mainIntNo = 0;

    usb_osalHwiParamsInit(&hwiInputParams);

    /* Construct Hwi object for this USB interrupts. */
    hwiInputParams.name = NULL;
    hwiInputParams.arg  = (uintptr_t)usbDevParams;
    hwiInputParams.priority = 0x10U;
    hwiInputParams.evtId = 0;

    if (usbDevParams->instanceNo == 0)
    {
        mainIntNo = SYS_INT_USB0;
    }
#if !defined(SOC_OMAPL137) && !defined(SOC_OMAPL138)
    else
    {
        mainIntNo = SYS_INT_USB1;
    }
    hwiInputParams.triggerSensitivity = 0x3; /* interrupt edge triggered */
#endif

    /* Initialize the object ISR fucntion with application ISR */
    usb_osalRegisterInterrupt(mainIntNo,
                              (HwiP_Fxn)usbCoreIntrHandler,
                              &hwiInputParams);
#endif

    USB_irqConfig(usbHandle, usbDevParams);

}

/* main entry point for DWC Miscs interrupt handler  with USB Wrapper setup.
 * Matching interrupt call-back function API */
void usbMiscIntrHandler(uint32_t* pUsbParam)
{
    USB_miscIrqHandler(((USB_Params*)pUsbParam)->usbHandle, (USB_Params*)pUsbParam);
}


/* main entry point for DWC core interrupt handler with USB Wrapper setup
* Matching interrupt call-back function API */
void usbCoreIntrHandler(uint32_t* pUsbParam)
{
    USB_coreIrqHandler(((USB_Params*)pUsbParam)->usbHandle, (USB_Params*)pUsbParam);
}

uint32_t
usbDAudioEventCallback(struct usbGadgetObj *pGadgetObject, void *pvCBData, uint32_t ui32Event,
						   uint32_t ui32MsgParam,
                           void* pvMsgData)
{
	switch (ui32Event)
	{
            /* To handle set volume event from the HOST */
        case USBD_AUDIO_EVENT_VOLUME:
		 gs_Volume = (uint16_t)ui32MsgParam;
		 gusVolumeMute = FALSE;
		 break;
            /* To handle volume mute event from the HOST */
	    case USBD_AUDIO_EVENT_MUTE:
	         gusVolumeMute = ~gusVolumeMute;
		 break;
	}

	SemaphoreP_post(hSemEventHandler);
    return(0);
}

/*****************************************************************************
 *  usb_main
*****************************************************************************/
int usb_main(void)
{

    TaskP_Handle task;
    TaskP_Params taskParams;
    SemaphoreP_Params semParams;

    // initialize the audio buffers
    audio_buffer_init();

    /*Initialize the Semaphore params*/
    SemaphoreP_Params_init( &semParams );
    hSemEventHandler = ( SemaphoreP_Handle )SemaphoreP_create( 0, &semParams);

    OS_init();

    TaskP_Params_init(&taskParams);
    taskParams.stack = gTskStackMain;
    taskParams.stacksize = sizeof(gTskStackMain);
    taskParams.priority = 1;
    task = TaskP_create(&taskFxn, &taskParams);
    if (task == NULL) {
        printf("TaskP_create() failed!\n");
        OS_stop();
    }

#if defined (evmAM335x) || defined (evmOMAPL137) || defined (lcdkOMAPL138) \
        || defined (lcdkC6748)
    delayTimerSetup();
#endif

#if defined (idkAM572x)
    /* Cross bar configuration for USB interrupts - just use default values
     * Customized interrupt numbers can also be used if neccessary.
     * This code can be commented out since we are using default settings */
    /* USB 1 (1 base) */
    CSL_xbarMpuIrqConfigure(CSL_XBAR_INST_MPU_IRQ_76,  CSL_XBAR_USB1_IRQ_INTR0);  /* main irq */
    CSL_xbarMpuIrqConfigure(CSL_XBAR_INST_MPU_IRQ_77,  CSL_XBAR_USB1_IRQ_INTR1);  /* misc irq */

    /* USB 2 (1 base) */
    CSL_xbarMpuIrqConfigure(CSL_XBAR_INST_MPU_IRQ_78,  CSL_XBAR_USB2_IRQ_INTR0);  /* main irq */
    CSL_xbarMpuIrqConfigure(CSL_XBAR_INST_MPU_IRQ_92,  CSL_XBAR_USB2_IRQ_INTR1);  /* misc irq */
#endif

    OS_start();    /* does not return */

    return 0;
}
