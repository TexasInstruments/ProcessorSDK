/**
 *  \file   main_led_blink.c
 *
 *  \brief  Example application main file. This application will toggle the led.
 *          The led toggling will be done inside an callback function, which
 *          will be called by Interrupt Service Routine. Interrupts are
 *          triggered manually and no external source is used to trigger
 *          interrupts.
 *
 */

/*
 * Copyright (C) 2014 - 2023 Texas Instruments Incorporated - http://www.ti.com/
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

#include <ti/csl/arch/csl_arch.h>

#include <ti/osal/osal.h>
#include <ti/osal/TaskP.h>

#include <stdio.h>
#include <string.h>

#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>

#include "GPIO_log.h"
#include "GPIO_board.h"

#include <ti/board/board.h>
#include <ti/csl/soc.h>
#include <ti/drv/sciclient/sciclient.h>

#if defined (BUILD_C7X)
#include <ti/csl/arch/csl_arch.h>
#endif

/**********************************************************************
 ************************** Macros ************************************
 **********************************************************************/

#if defined (SIM_BUILD)
#define DELAY_VALUE                     (1)
#else
#define DELAY_VALUE                     (500U)   /* 500 msec */
#endif

#define APP_TSK_STACK_MAIN              (0x8000U)

#if defined (SOC_J7200)
#define PADCONFIG                       (0x0011C000)    /* PADCONFIG_0 */
#elif defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#define PADCONFIG                       (0x0011C0C8)    /* PADCONFIG_11*/
#endif

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
volatile uint32_t wkup_gpio_intr_triggered = 0;
volatile uint32_t main_gpio_intr_triggered = 0;
uint32_t gpioBaseAddr[MAX_GPIOS_USED_IN_APP] =
{
    CSL_WKUP_GPIO0_BASE,
#if defined(SOC_J721E)
    CSL_GPIO1_BASE,
#elif defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
    /* J7200, J721S2 has no MAIN_GPIO_1 */
    CSL_GPIO0_BASE
#endif
};
uint32_t gpioPinNums[MAX_GPIOS_USED_IN_APP] =
{
    WKUP_GPIO_PIN_NUM,
    MAIN_GPIO_PIN_NUM
};

#if defined (FREERTOS)
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
#endif


/**********************************************************************
 ************************** Internal functions ************************
 **********************************************************************/

/* Delay function */
void AppDelay(unsigned int delayVal);

/* Callback function */
void AppMainGpioCallbackFxn(void);
void AppWkupGpioCallbackFxn(void);


/*
 *  ======== Board_initI2C ========
 */
static void Board_initGPIO(void)
{
    Board_initCfg boardCfg;
    GPIO_v0_HwAttrs gpio_cfg;
    uint32_t looper;

    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_MODULE_CLOCK |
        BOARD_INIT_UART_STDIO;
    Board_init(boardCfg);

    for (looper = 0; looper < MAX_GPIOS_USED_IN_APP; looper++)
    {
        GPIO_socGetInitCfg(looper, &gpio_cfg);
        gpio_cfg.baseAddr = gpioBaseAddr[looper];
        (gpio_cfg.intCfg[gpioPinNums[looper]]).intcMuxNum = INVALID_INTC_MUX_NUM;
        (gpio_cfg.intCfg[gpioPinNums[looper]]).intcMuxInEvent = 0;
        (gpio_cfg.intCfg[gpioPinNums[looper]]).intcMuxOutEvent = 0;
        GPIO_socSetInitCfg(looper, &gpio_cfg);
    }
}


/*
 *  ======== AppDelay ========
 */
void AppDelay(unsigned int delayVal)
{
    Osal_delay(delayVal);
}

/*
 *  ======== AppLoopDelay ========
 */
void AppLoopDelay(uint32_t delayVal)
{
    volatile uint32_t i;

    for (i = 0; i < (delayVal * 1000); i++);
}

/*
 *  ======== Callback function ========
 */
void AppWkupGpioCallbackFxn(void)
{
    wkup_gpio_intr_triggered = 1;
}

void AppMainGpioCallbackFxn(void)
{
    main_gpio_intr_triggered = 1;
}

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

void AppGPIOPadConfig()
{
#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
    Board_init(BOARD_INIT_UNLOCK_MMR);
    /* Set Ball number U6 to RX_ACTIVE on PADCONFIG0 in J7200 */
    /* Set Ball number AG24 to RX_ACTIVE on PADCONFIG0 in J721S2 */
    /* RX_ACTIVE=1, MUX_MODE=7 for GPIO */
    *((uint32_t *)(PADCONFIG)) = 0x00040007;
#endif
}

#if defined (RTOS_ENV)
void gpio_test(void* arg0, void* arg1)
{
#else
int main()
{
#endif

    /* Call board init functions */
    Board_initGPIO();

    /* GPIO initialization */
    GPIO_init();

    /* Set the callback function */
    GPIO_setCallback(WKUP_GPIO, AppWkupGpioCallbackFxn);
    GPIO_setCallback(MAIN_GPIO, AppMainGpioCallbackFxn);

    /* Enable GPIO interrupt on the specific gpio pin */
    GPIO_enableInt(WKUP_GPIO);
    GPIO_enableInt(MAIN_GPIO);

    AppGPIOPadConfig();
  
    GPIO_log("\n -------------GPIO Led Blink Application------------- \n");
    UART_printStatus("\n Awaiting interrupt occurrence ...\n");


    GPIO_toggle(WKUP_GPIO);
    GPIO_toggle(MAIN_GPIO);

    AppDelay(DELAY_VALUE);
    while (wkup_gpio_intr_triggered == 0U)
    {
        
    }
    UART_printStatus("\n WKUP GPIO toggled!! \n");
    while (main_gpio_intr_triggered == 0U)
    {
        
    }
    UART_printStatus("\n MAIN GPIO toggled!! \n");
    if(0U != wkup_gpio_intr_triggered && 0U != main_gpio_intr_triggered)
    {
        UART_printStatus("\n All tests have passed!! \n");
    }

}

#if defined (RTOS_ENV)
/*
 *  ======== main ========
 */
int main(void)
{

    TaskP_Params taskParams;
    
    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    TaskP_Params_init(&taskParams);
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&gpio_test, &taskParams);

    OS_start();

    return (0);
}
#endif


