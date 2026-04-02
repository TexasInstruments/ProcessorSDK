/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/

/**
 *  \file   gpio_test_header_config.c
 *
 *  \brief  GPIO test header configuration file.
 *
 */
#include <stdio.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/csl/soc.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>

/*
   Port and pin number mask for GPIO.
   Bits 7-0: Pin number  and Bits 15-8: Port number */

#define PIN1       (0x0128)   /* GPIO1_40/GPIO1_51 */
#define PIN2       (0x0133)

#define PIN3       (0x0129)  /* GPIO1_41/GPIO1_50 */
#define PIN4       (0x0132)

#define PIN5       (0x0108)  /* GPIO1_8/GPIO1_28 */
#define PIN6       (0x011C)

#define PIN7       (0x0102)  /* GPIO1_2/GPIO1_21 */
#define PIN8       (0x0115)

#define PIN9       (0x0103)  /* GPIO1_3/GPIO1_20 */
#define PIN10      (0x0114)

#define PIN11      (0x0104)  /* GPIO1_4/GPIO1_7 */
#define PIN12      (0x0107)

#define PIN13      (0x010C)  /* GPIO1_12/GPIO1_17 */
#define PIN14      (0x0111)

#define PIN15      (0x0124)  /* GPIO1_36/GPIO1_18 */
#define PIN16      (0x0112)

#define PIN17      (0x001E)  /* GPIO0_30/GPIO1_19 */
#define PIN18      (0x0113)

#define PIN19      (0x0024)  /* GPIO0_36/GPIO1_0 */
#define PIN20      (0x0100)

#define PIN21      (0x0018)  /* GPIO0_24/GPIO1_24 */
#define PIN22      (0x0118)

#define PIN23      (0x0017)  /* GPIO0_23/GPIO1_11 */
#define PIN24      (0x010B)

#define PIN25      (0x0120)  /* GPIO1_32/GPIO0_29 */
#define PIN26      (0x001D)

#define PIN27      (0x0019)  /* GPIO0_25/GPIO0_35 */
#define PIN28      (0x0023)

#define PIN29      (0x001F)  /* GPIO0_31/GPIO1_10 */
#define PIN30      (0x010A)

#define PIN31      (0x0131)  /* GPIO1_49/GPIO1_13 */
#define PIN32      (0x010D)

#define PIN33      (0x0105)  /* GPIO1_5/GPIO1_23 */
#define PIN34      (0x0117)

#define PIN35      (0x010E)  /* GPIO1_14/GPIO1_6 */
#define PIN36      (0x0106)

#define PIN37      (0x0123)  /* GPIO1_35/GPIO1_22 */
#define PIN38      (0x0116)

#define PIN39      (0x0054)  /* GPIO0_84/GPIO0_17 */
#define PIN40      (0x0011)

#define PIN41      (0x0053)  /* GPIO0_83/GPIO0_27 */
#define PIN42      (0x001B)

#define PIN43      (0x0020)  /* GPIO0_32/GPIO0_22 */
#define PIN44      (0x0016)

#define PIN45      (0x0022)  /* GPIO0_34/GPIO0_43 */
#define PIN46      (0x002B)

#define PIN47      (0x0025)  /* GPIO0_37/GPIO0_44 */
#define PIN48      (0x002C)

#define PIN49      (0x0027)  /* GPIO0_39/GPIO1_37 */
#define PIN50      (0x0125)

#define PIN51      (0x0127)  /* GPIO1_39/GPIO1_63 */
#define PIN52      (0x013F)

#define PIN53      (0x012F)  /* GPIO1_47/GPIO1_33 */
#define PIN54      (0x0121)

#define PIN55      (0x0130)  /* GPIO1_48/GPIO1_25 */
#define PIN56      (0x0119)

#define PIN57      (0x011F)  /* GPIO1_31/GPIO0_16 */
#define PIN58      (0x0010)

#define PIN59      (0x010F)  /* GPIO1_15/GPIO0_82 */
#define PIN60      (0x0052)

#define PIN61      (0x001C)  /* GPIO0_28/GPIO0_40 */
#define PIN62      (0x0028)

#define PIN63      (0x0021)  /* GPIO0_33/GPIO1_60 */
#define PIN64      (0x013C)

#define PIN65      (0x0126)  /* GPIO1_38/GPIO1_62 */
#define PIN66      (0x013E)

#define PIN67      (0x0101)  /* GPIO1_1/GPIO1_26 */
#define PIN68      (0x011A)

#define PIN69      (0x0110)  /* GPIO1_16/GPIO1_34 */
#define PIN70      (0x0122)

#define PIN71      (0x0014)  /* GPIO0_20/GPIO0_18 */
#define PIN72      (0x0012)

#define PIN73      (0x002A)  /* GPIO0_42/GPIO0_26 */
#define PIN74      (0x001A)

#define PIN75      (0x011D)  /* GPIO1_29/GPIO0_38 */
#define PIN76      (0x0026)

#define PIN77      (0x013D)  /* GPIO1_61/GPIO1_30 */
#define PIN78      (0x011E)

#define PIN79      (0x000F)  /* GPIO0_15/GPIO0_21 */
#define PIN80      (0x0015)

#define PIN81      (0x0140)  /* GPIO1_64/GPIO1_65 */
#define PIN82      (0x0141)

#define PIN83      (0x011B)  /* GPIO1_27/GPIO0_41/GPIO1_9 */
#define PIN84      (0x0029)
#define PIN85      (0x0109)
/* Safety Connector Pins  */
#define PIN86      (0x0204)  /* MCU_GPIO0_4/MCU_GPIO0_13 */
#define PIN87      (0x020D)

#define PIN88      (0x0210)  /* MCU_GPIO0_16/MCU_GPIO0_21 */
#define PIN89      (0x0215)

#define PIN90      (0x0209)  /* MCU_GPIO0_9/MCU_GPIO0_11 */
#define PIN91      (0x020B)

#define PIN92      (0x0214)  /* MCU_GPIO0_20/MCU_GPIO0_15 */
#define PIN93      (0x020F)

#define PIN94      (0x020C)  /* MCU_GPIO0_12/MCU_GPIO0_7 */
#define PIN95      (0x0207)

#define PIN96      (0x0208)  /* MCU_GPIO0_8/MCU_GPIO0_14 */
#define PIN97      (0x020D)

#define PIN98      (0x0211)  /* MCU_GPIO0_17/MCU_GPIO0_22 */
#define PIN99      (0x0216)

#define PIN100      (0x0212)  /* MCU_GPIO0_18/MCU_GPIO0_19 */
#define PIN101      (0x0213)

/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
    /* pin Mux Line PROFI_UART_SEL */
    PIN1 | GPIO_CFG_OUTPUT,
    PIN2 | GPIO_CFG_INPUT,
    PIN3 | GPIO_CFG_OUTPUT,
    PIN4 | GPIO_CFG_INPUT,
    PIN5 | GPIO_CFG_OUTPUT,
    PIN6 | GPIO_CFG_INPUT,
    PIN7 | GPIO_CFG_OUTPUT,
    PIN8 | GPIO_CFG_INPUT,
    PIN9 | GPIO_CFG_OUTPUT,
    PIN10 | GPIO_CFG_INPUT,
    PIN11 | GPIO_CFG_OUTPUT,
    PIN12 | GPIO_CFG_INPUT,
    PIN13 | GPIO_CFG_OUTPUT,
    PIN14 | GPIO_CFG_INPUT,
    PIN15 | GPIO_CFG_OUTPUT,
    PIN16 | GPIO_CFG_INPUT,
    PIN17 | GPIO_CFG_OUTPUT,
    PIN18 | GPIO_CFG_INPUT,
    PIN19 | GPIO_CFG_OUTPUT,
    PIN20 | GPIO_CFG_INPUT,
    PIN21 | GPIO_CFG_OUTPUT,
    PIN22 | GPIO_CFG_INPUT,
    PIN23 | GPIO_CFG_OUTPUT,
    PIN24 | GPIO_CFG_INPUT,
    PIN25 | GPIO_CFG_OUTPUT,
    PIN26 | GPIO_CFG_INPUT,
    PIN27 | GPIO_CFG_OUTPUT,
    PIN28 | GPIO_CFG_INPUT,
    PIN29 | GPIO_CFG_OUTPUT,
    PIN30 | GPIO_CFG_INPUT,
    PIN31 | GPIO_CFG_OUTPUT,
    PIN32 | GPIO_CFG_INPUT,
    PIN33 | GPIO_CFG_OUTPUT,
    PIN34 | GPIO_CFG_INPUT,
    PIN35 | GPIO_CFG_OUTPUT,
    PIN36 | GPIO_CFG_INPUT,
    PIN37 | GPIO_CFG_OUTPUT,
    PIN38 | GPIO_CFG_INPUT,
    PIN39 | GPIO_CFG_OUTPUT,
    PIN40 | GPIO_CFG_INPUT,
    PIN41 | GPIO_CFG_OUTPUT,
    PIN42 | GPIO_CFG_INPUT,
    PIN43 | GPIO_CFG_OUTPUT,
    PIN44 | GPIO_CFG_INPUT,
    PIN45 | GPIO_CFG_OUTPUT,
    PIN46 | GPIO_CFG_INPUT,
    PIN47 | GPIO_CFG_OUTPUT,
    PIN48 | GPIO_CFG_INPUT,
    PIN49 | GPIO_CFG_OUTPUT,
    PIN50 | GPIO_CFG_INPUT,
    PIN51 | GPIO_CFG_OUTPUT,
    PIN52 | GPIO_CFG_INPUT,
    PIN53 | GPIO_CFG_OUTPUT,
    PIN54 | GPIO_CFG_INPUT,
    PIN55 | GPIO_CFG_OUTPUT,
    PIN56 | GPIO_CFG_INPUT,
    PIN57 | GPIO_CFG_OUTPUT,
    PIN58 | GPIO_CFG_INPUT,
    PIN59 | GPIO_CFG_OUTPUT,
    PIN60 | GPIO_CFG_INPUT,
    PIN61 | GPIO_CFG_OUTPUT,
    PIN62 | GPIO_CFG_INPUT,
    PIN63 | GPIO_CFG_OUTPUT,
    PIN64 | GPIO_CFG_INPUT,
    PIN65 | GPIO_CFG_OUTPUT,
    PIN66 | GPIO_CFG_INPUT,
    PIN67 | GPIO_CFG_OUTPUT,
    PIN68 | GPIO_CFG_INPUT,
    PIN69 | GPIO_CFG_OUTPUT,
    PIN70 | GPIO_CFG_INPUT,
    PIN71 | GPIO_CFG_OUTPUT,
    PIN72 | GPIO_CFG_INPUT,
    PIN73 | GPIO_CFG_OUTPUT,
    PIN74 | GPIO_CFG_INPUT,
    PIN75 | GPIO_CFG_OUTPUT,
    PIN76 | GPIO_CFG_INPUT,
    PIN77 | GPIO_CFG_OUTPUT,
    PIN78 | GPIO_CFG_INPUT,
    PIN79 | GPIO_CFG_OUTPUT,
    PIN80 | GPIO_CFG_INPUT,
    PIN81 | GPIO_CFG_OUTPUT,
    PIN82 | GPIO_CFG_INPUT,
    PIN83 | GPIO_CFG_OUTPUT,
    PIN84 | GPIO_CFG_INPUT,
    PIN85 | GPIO_CFG_INPUT,
/* Safety Connector Pins  */
    PIN86 | GPIO_CFG_OUTPUT,
    PIN87 | GPIO_CFG_INPUT,
    PIN88 | GPIO_CFG_OUTPUT,
    PIN89 | GPIO_CFG_INPUT,
    PIN90 | GPIO_CFG_OUTPUT,
    PIN91 | GPIO_CFG_INPUT,
    PIN92 | GPIO_CFG_OUTPUT,
    PIN93 | GPIO_CFG_INPUT,
    PIN94 | GPIO_CFG_OUTPUT,
    PIN95 | GPIO_CFG_INPUT,
    PIN96 | GPIO_CFG_OUTPUT,
    PIN97 | GPIO_CFG_INPUT,
    PIN98 | GPIO_CFG_OUTPUT,
    PIN99 | GPIO_CFG_INPUT,
    PIN100 | GPIO_CFG_OUTPUT,
    PIN101 | GPIO_CFG_INPUT,
};

/* GPIO Driver call back functions */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,
};

/* GPIO Driver configuration structure */
GPIO_v0_Config GPIO_v0_config = {
    gpioPinConfigs,
    gpioCallbackFunctions,
    sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
    sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
    0,
};
