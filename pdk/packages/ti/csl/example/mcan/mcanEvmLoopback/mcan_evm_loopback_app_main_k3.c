/*
 *   Copyright (c) Texas Instruments Incorporated 2016-2022
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

/**
 *  \file     mcan_evm_loopback_app_main.c
 *
 *  \brief    This file contains MCAN sample code.
 *
 *  \details    This application is expected to be hosted on SoC/cores
 *                  J721E   / MCU 1_0 and MCU 2_1
 *                  J7200   / MCU 1_0 and MCU 2_1
 *                  J721S2  / MCU 1_0 and MCU 2_1
 *              This application can operate as either an transmitter, receiver
 *              CAN nodes. Also, supports digital-loop back mode.
 *
 *              This application with 2 board could be used to emulate 2 node
 *              CAN network. Where 1 node would be the transmitter and other
 *              would be receiver.
 *
 *              This application also provides an External looopback test, Where
 *              we configure one CAN instance as transmitter and other CAN
 *              instance as reciever, on the same board. For this, we need to externally
 *              connect the transciver pins of both instances.
 *
 *              In all modes MCAN operates in CAN-FD. Arbitration bit-rate and
 *              data phase bit-rate is set to 1Mbit/s and 5Mbit/s receptively.
 *
 *  Note on Available Instances on EVM, and Tested Instances with this example:
 *              For J721S2_EVM:
 *                  Available Instances    : MCU_MCAN0,MCU_MCAN1,MAIN_MCAN3,
 *                                           MAIN_MCAN4,MAIN_MCAN5,MAIN_MCAN16
 *                  Test support Instances : For mcu1_0
 *                                            1.Transmitter test : MCU_MCAN0
 *                                            2.Receiver test    : MCU_MCAN1
 *                                            3.Internal loopback: MCU_MCAN0
 *                                            4.External loopback: MCU_MCAN0,MCU_MCAN1
 *                                           For mcu2_1
 *                                            1.Transmitter test : MAIN_MCAN4
 *                                            2.Receiver test    : MAIN_MCAN4
 *                                            3.Internal loopback: MAIN_MCAN4
 *                                            4.External loopback: MAIN_MCAN4, MAIN_MCAN16
 *                                            NOTE: MAIN_MCAN4 is J6(MC9) on GESI Board for J721S2.
 *              For J7200_EVM:
 *                  Available Instances    : MCU_MCAN0, MCU_MCAN1, MAIN_MCAN0, MAIN_MCAN3
 *                                           MAIN_MCAN4, MAIN_MCAN5, MAIN_MCAN6, MAIN_MCAN7, 
 *                                           MAIN_MCAN8, MAIN_MCAN10
 *                  Test support Instances : For mcu1_0
 *                                            1.Transmitter test : MCU_MCAN0
 *                                            2.Receiver test    : MCU_MCAN1
 *                                            3.Internal loopback: MCU_MCAN0
 *                                            4.External loopback: MCU_MCAN0,MCU_MCAN1
 *                                           For mcu2_1
 *                                            1.Transmitter test : MAIN_MCAN4
 *                                            2.Receiver test    : MAIN_MCAN4
 *                                            3.Internal loopback: MAIN_MCAN4
 *                                            4.External loopback: MAIN_MCAN4, MAIN_MCAN5 
 *              For J721E_EVM:
 *                  Available Instances    : MCU_MCAN0,MCU_MCAN1,MAIN_MCAN0,MAIN_MCAN2
 *                                            MAIN_MCAN4,MAIN_MCAN5,MAIN_MCAN6,MAIN_MCAN7,
 *                                            MAIN_MCAN9,MAIN_MCAN11
 *                  Test support Instances : For mcu1_0
 *                                            1.Transmitter test : MCU_MCAN0
 *                                            2.Receiver test    : MCU_MCAN1
 *                                            3.Internal loopback: MCU_MCAN0
 *                                            4.External loopback: MCU_MCAN0,MCU_MCAN1
 *                                           For mcu2_1
 *                                            1.Transmitter test : MAIN_MCAN4
 *                                            2.Receiver test    : MAIN_MCAN4
 *                                            3.Internal loopback: MAIN_MCAN4
 *                                            4.External loopback: MAIN_MCAN4, MAIN_MCAN5
 *              For J784S4_EVM:
 *                  Available Instances    : MCU_MCAN0,MCU_MCAN1,MAIN_MCAN3,
 *                                            MAIN_MCAN4,MAIN_MCAN5,MAIN_MCAN16
 *                  Test support Instances : For mcu1_0
 *                                            1.Transmitter test : MCU_MCAN0
 *                                            2.Receiver test    : MCU_MCAN1
 *                                            3.Internal loopback: MCU_MCAN0
 *                                            4.External loopback: MCU_MCAN0,MCU_MCAN1
 *                                           For mcu2_1
 *                                            1.Transmitter test : MAIN_MCAN4
 *                                            2.Receiver test    : MAIN_MCAN4
 *                                            3.Internal loopback: MAIN_MCAN4
 *                                            4.External loopback: MAIN_MCAN4, MAIN_MCAN16
 *                                            
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/csl_mcan.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/osal/osal.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/csl/csl_gpio.h>
#include <ti/board/board.h>
#include <ti/drv/i2c/i2c.h>

#if defined (SOC_J721S2)
#include <ti/board/src/j721s2_evm/include/board_i2c_io_exp.h>
#include <ti/board/src/j721s2_evm/include/board_control.h>
#endif

#if defined (SOC_J7200)
#include <ti/board/src/j7200_evm/include/board_i2c_io_exp.h>
#include <ti/board/src/j7200_evm/include/board_control.h>
#endif

#if defined (SOC_J721E)
#include <ti/board/src/j721e_evm/include/board_control.h>
#endif

#if defined (SOC_J784S4)
#include <ti/board/src/j784s4_evm/include/board_i2c_io_exp.h>
#include <ti/board/src/j784s4_evm/include/board_control.h>
#endif

#if defined (SOC_J742S2)
#include <ti/board/src/j742s2_evm/include/board_i2c_io_exp.h>
#include <ti/board/src/j742s2_evm/include/board_control.h>
#endif


#include <ti/drv/sciclient/sciclient.h>
#ifdef UNITY_INCLUDE_CONFIG_H
#include <ti/build/unit-test/Unity/src/unity.h>
#include <ti/build/unit-test/config/unity_config.h>
#endif

/* ========================================================================== */
/*                                Macros                                      */
/* ========================================================================== */

#define APP_ENABLE_UART_PRINT                    (1U)

#define APP_MCAN_STD_ID_FILT_START_ADDR          (0U)
#define APP_MCAN_STD_ID_FILTER_NUM               (1U)
#define APP_MCAN_EXT_ID_FILT_START_ADDR          (48U)
#define APP_MCAN_EXT_ID_FILTER_NUM               (1U)
#define APP_MCAN_TX_EVENT_START_ADDR             (100U)
#define APP_MCAN_TX_EVENT_SIZE                   (5U)
#define APP_MCAN_TX_BUFF_START_ADDR              (148U)
#define APP_MCAN_TX_BUFF_SIZE                    (5U)
#define APP_MCAN_TX_FIFO_SIZE                    (5U)
#define APP_MCAN_FIFO_0_START_ADDR               (548U)
#define APP_MCAN_FIFO_0_NUM                      (5U)
#define APP_MCAN_FIFO_1_START_ADDR               (748U)
#define APP_MCAN_FIFO_1_NUM                      (5U)
#define APP_MCAN_RX_BUFF_START_ADDR              (948U)

#define APP_MCAN_EXT_ID_AND_MASK                 (0x1FFFFFFFU)

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#define APP_MCU_MCAN_0_INT0     (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN0_MCANSS_MCAN_LVL_INT_0)
#define APP_MCU_MCAN_0_INT1     (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN0_MCANSS_MCAN_LVL_INT_1)
#define APP_MCU_MCAN_0_TS_INT   (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN0_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#define APP_MCU_MCAN_1_INT0     (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN1_MCANSS_MCAN_LVL_INT_0)
#define APP_MCU_MCAN_1_INT1     (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN1_MCANSS_MCAN_LVL_INT_1)
#define APP_MCU_MCAN_1_TS_INT   (CSLR_MCU_R5FSS0_CORE0_INTR_MCU_MCAN1_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

/* Macros for MAIN MCAN Instances to use for testing */
#define APP_MAIN_MCAN_DEF_INST0_BASE_ADDRESS                (CSL_MCAN4_MSGMEM_RAM_BASE)
#if defined (SOC_J721E) || defined (SOC_J7200)
        #define APP_MAIN_MCAN_DEF_INST1_BASE_ADDRESS        (CSL_MCAN5_MSGMEM_RAM_BASE)
#elif defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
        #define APP_MAIN_MCAN_DEF_INST1_BASE_ADDRESS        (CSL_MCAN16_MSGMEM_RAM_BASE)
#endif

#if defined (SOC_J7200)
        #define APP_MAIN_MCAN_DEF_INST0_INT0                (CSLR_R5FSS0_CORE0_INTR_MCAN4_MCANSS_MCAN_LVL_INT_0)
        #define APP_MAIN_MCAN_DEF_INST0_INT1                (CSLR_R5FSS0_CORE0_INTR_MCAN4_MCANSS_MCAN_LVL_INT_1)
        #define APP_MAIN_MCAN_DEF_INST0_TS_INT              (CSLR_R5FSS0_CORE0_INTR_MCAN4_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
        #define APP_MAIN_MCAN_DEF_INST1_INT0                (CSLR_R5FSS0_CORE0_INTR_MCAN5_MCANSS_MCAN_LVL_INT_0)
        #define APP_MAIN_MCAN_DEF_INST1_INT1                (CSLR_R5FSS0_CORE0_INTR_MCAN5_MCANSS_MCAN_LVL_INT_1)
        #define APP_MAIN_MCAN_DEF_INST1_TS_INT              (CSLR_R5FSS0_CORE0_INTR_MCAN5_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
#endif

#if defined (SOC_J721E) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
        #define APP_MAIN_MCAN_DEF_INST0_INT0                (CSLR_R5FSS1_CORE0_INTR_MCAN4_MCANSS_MCAN_LVL_INT_0)
        #define APP_MAIN_MCAN_DEF_INST0_INT1                (CSLR_R5FSS1_CORE0_INTR_MCAN4_MCANSS_MCAN_LVL_INT_1)
        #define APP_MAIN_MCAN_DEF_INST0_TS_INT              (CSLR_R5FSS1_CORE0_INTR_MCAN4_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
    #if defined (SOC_J721E)
        #define APP_MAIN_MCAN_DEF_INST1_INT0                (CSLR_R5FSS1_CORE0_INTR_MCAN5_MCANSS_MCAN_LVL_INT_0)
        #define APP_MAIN_MCAN_DEF_INST1_INT1                (CSLR_R5FSS1_CORE0_INTR_MCAN5_MCANSS_MCAN_LVL_INT_1)
        #define APP_MAIN_MCAN_DEF_INST1_TS_INT              (CSLR_R5FSS1_CORE0_INTR_MCAN5_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
    #elif defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
        #define APP_MAIN_MCAN_DEF_INST1_INT0                (CSLR_R5FSS1_CORE0_INTR_MCAN16_MCANSS_MCAN_LVL_INT_0)
        #define APP_MAIN_MCAN_DEF_INST1_INT1                (CSLR_R5FSS1_CORE0_INTR_MCAN16_MCANSS_MCAN_LVL_INT_1)
        #define APP_MAIN_MCAN_DEF_INST1_TS_INT              (CSLR_R5FSS1_CORE0_INTR_MCAN16_MCANSS_EXT_TS_ROLLOVER_LVL_INT_0)
    #endif
#endif

/* Print buffer character limit for prints- UART or CCS Console */
#define APP_PRINT_BUFFER_SIZE           (4000U)

#if defined (SOC_J721E)
#define J721E_GPIO_GESI_CAN_STB_PIN     (0x003C)
#define J721E_GPIO_MAIN_MCAN2_STB_PIN   (0x007F)
#endif


#define MCU_MCAN1_STB_PIN               (0x2)               
#define MCU_MCAN0_ENABLE_PIN            (0x0)

#if defined (SOC_J721E)
#define MCU_MCAN0_STBZ_PIN              (0x36)
#elif defined (SOC_J7200)
#define MCU_MCAN0_STBZ_PIN              (0x3A)
#elif defined (SOC_J721S2)
#define MCU_MCAN0_STBZ_PIN              (0x45)
#endif

#if defined (SOC_J784S4) || defined (SOC_J742S2)
#define MCU_MCAN0_STB_PIN               (0x45)
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint32_t          gMcanAppdataSize[16] =
{0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
volatile uint32_t gMcanIsrIntr0Flag = 1U, gMcanIsrIntr0Flag_lpbk = 1U;
volatile uint32_t gMcanIsrIntr1Flag = 1U, gMcanIsrIntr1Flag_lpbk = 1U;
MCAN_ECCErrStatus gMcaneccErr;

uint32_t gMcanModAddr, gMcanModAddr_lpbk;

#if defined(SOC_J721E)
/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] = {
    /* Output pin : CAN STB */
    J721E_GPIO_GESI_CAN_STB_PIN   | GPIO_CFG_OUTPUT,
    J721E_GPIO_MAIN_MCAN2_STB_PIN | GPIO_CFG_OUTPUT,
};
#endif


/* GPIO Driver configuration structure */
GPIO_v0_Config GPIO_v0_config = {
#if defined (SOC_J721E)
        gpioPinConfigs,
        NULL,
        sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
        0,
        0,
#else
        NULL,
        NULL,
        0,
        0,
        0
#endif
    };

#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
/*GPIO expander Configuration array */
Board_IoExpCfg_t ioExpCfg[]= 
{
#if defined (SOC_J7200)
    /*Set CAN STB to LEVEL_LOW by Configuring GPIO expander*/
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_7,
       .signalLevel = GPIO_SIGNAL_LEVEL_LOW,
    },
    /*Set CANUART_MUX1_SEL0 to LEVEL_HIGH */
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_1,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
    /*Set CANUART_MUX2_SEL0 to LEVEL_HIGH */
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_2,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
    /*Set CANUART_MUX_SEL1 to LEVEL_HIGH*/
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_3,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
    /*Set TRC_D17 mux sel to low to select CAN5TX*/
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_5,
       .signalLevel = GPIO_SIGNAL_LEVEL_LOW,
    },
#elif defined (SOC_J721S2)
    { 
       .i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_7,
       .signalLevel = GPIO_SIGNAL_LEVEL_LOW,
    },
    /*Set CANUART_MUX1_SEL0 to LEVEL_HIGH */
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_1,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
    /*Set CANUART_MUX2_SEL0 to LEVEL_HIGH */
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_2,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
    /*Set CANUART_MUX_SEL1 to LEVEL_HIGH*/
    {
       .i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR,
       .enableIntr  = false,
       .ioExpType   = ONE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_3,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },

#elif defined (SOC_J784S4) || defined (SOC_J742S2)
    {
       .i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR,
       .enableIntr  = false,
       .ioExpType   = THREE_PORT_IOEXP,
       .portNum     = PORTNUM_0,
       .pinNum      = PIN_NUM_7,
       .signalLevel = GPIO_SIGNAL_LEVEL_LOW,
    },
    /*Set CANUART_MUX1_SEL0 to LEVEL_HIGH */
    {
       .i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR,
       .enableIntr  = false,
       .ioExpType   = THREE_PORT_IOEXP,
       .portNum     = PORTNUM_1,
       .pinNum      = PIN_NUM_5,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
    /*Set CANUART_MUX2_SEL0 to LEVEL_HIGH */
    {
       .i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR,
       .enableIntr  = false,
       .ioExpType   = THREE_PORT_IOEXP,
       .portNum     = PORTNUM_1,
       .pinNum      = PIN_NUM_6,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
    /*Set CANUART_MUX_SEL1 to LEVEL_HIGH*/
    {
       .i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE,
       .socDomain   = BOARD_SOC_DOMAIN_MAIN,
       .slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR,
       .enableIntr  = false,
       .ioExpType   = THREE_PORT_IOEXP,
       .portNum     = PORTNUM_1,
       .pinNum      = PIN_NUM_7,
       .signalLevel = GPIO_SIGNAL_LEVEL_HIGH,
    },
#endif
};
#endif

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

/**
 * \brief   This function will configure MCAN module
 *
 * \param   mcanInstAddr            MCAN Instance address
 *          enableInternalLpbk      Flag to enable/disable Internal loopback
 *
 * \retval  status      configuration status.
 */
static int32_t App_mcanConfig(uint32_t mcanInstAddr, bool enableInternalLpbk);

/**
 * \brief   This function will configure X-BAR for MCAN interrupts
 *
 * \param   MCAN Instance address
 *
 * \retval  status      configuration status.
 */
static int32_t App_mcanRegisterIsr(uint32_t mcanInstAddr);

/**
 * \brief   This is Interrupt Service Routine for MCAN interrupt 0.
 *
 * \param   none.
 *
 * \retval  none.
 */
static void App_mcanIntr0ISR(uintptr_t arg);

/**
 * \brief   This is Interrupt Service Routine for MCAN interrupt 1.
 *
 * \param   none.
 *
 * \retval  none.
 */
static void App_mcanIntr1ISR(uintptr_t arg);

/**
 * \brief   This is Interrupt Service Routine for MCAN TimeStamp interrupt.
 *
 * \param   none.
 *
 * \retval  none.
 */
static void App_mcanTSIntrISR(uintptr_t arg);

/**
 * \brief   This API will MCAN Rx Msg.
 *
 * \param   rxMsg           received message object.
 *
 * \retval  none.
 */
static void App_mcanPrintRxMsg(const MCAN_RxBufElement *rxMsg);

/**
 * \brief   This API will MCAN Tx Msg.
 *
 * \param   txMsg           message object to be transmitted.
 *
 * \retval  none.
 */
static void App_mcanPrintTxMsg(const MCAN_TxBufElement *txMsg);

/**
 * \brief   This API will load the register from ECC memory bank.
 *
 * \param   txMsg           message object to be transmitted.
 *
 * \return  None.
 */
static void APP_mcanTxTest(MCAN_TxBufElement *txMsg);

/**
 * \brief   This API will load the register from ECC memory bank.
 *
 * \param   txMsg           message object to be transmitted
 *                          (Needed for Message validation).
 *
 * \return  None.
 */
static void APP_mcanRxTest(const MCAN_TxBufElement *txMsg);

/**
 * \brief   This function will transmit fixed pattern and expects to receive
 *              the same
 *
 * \param   txMsg           message object to be transmitted.
 *
 * \return  None.
 */
static void APP_mcanLpbkTest(MCAN_TxBufElement *txMsg);

/**
 * \brief   This function will transmit fixed pattern from one CAN instance
 * and recieve the same pattern at another CAN instance, given that both 
 * are connected externally.
 * 
 *
 * \param   txMsg           message object to be transmitted.
 *
 * \return  None.
 */
static void APP_mcanExtLpbkTest(MCAN_TxBufElement *txMsg);
/**
 * \brief   This API will print on UART/CCS Console.
 *
 * \param   pcString        string to be printed.
 *
 * \return  None.
 */
void App_ConsolePrintf(const char *pcString, ...);

/**
 * \brief   This API will get a number from UART/CCS Console.
 *
 * \param   num        get the user input.
 *
 * \return  None.
 */
static void App_ConsoleGetNum(uint32_t *num);

static int32_t App_mcanRegisterInterrupt(uint32_t intNum, void f(uintptr_t));

#if defined (BUILD_MCU1_0)
static int32_t App_mcanCfgIrqRouterMain2Mcu(uint32_t devId, uint32_t offset, uint32_t intNum);
#endif

#if defined(UNITY_INCLUDE_CONFIG_H)
void test_csl_mcan_evm_loopback_app(void);
void test_csl_mcan_evm_loopback_app_runner(void);
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
#define TCA6424_CMD_AUTO_INC            ((uint8_t) 0x80U)

/* Input status register */
#define TCA6424_REG_INPUT0              ((UInt8) 0x00U)
#define TCA6424_REG_INPUT1              ((UInt8) 0x01U)
#define TCA6424_REG_INPUT2              ((UInt8) 0x02U)

/* Output register to change state of output BIT set to 1, output set HIGH */
#define TCA6424_REG_OUTPUT0             ((uint8_t) 0x04U)
#define TCA6424_REG_OUTPUT1             ((uint8_t) 0x05U)
#define TCA6424_REG_OUTPUT2             ((uint8_t) 0x06U)

/* Configuration register. BIT = '1' sets port to input, BIT = '0' sets
 * port to output */
#define TCA6424_REG_CONFIG0             ((uint8_t) 0x0CU)
#define TCA6424_REG_CONFIG1             ((uint8_t) 0x0DU)
#define TCA6424_REG_CONFIG2             ((uint8_t) 0x0EU)

void SetupI2CTransfer(I2C_Handle handle,  uint32_t slaveAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes)
{
    bool status;
    I2C_Transaction i2cTransaction;

    I2C_transactionInit(&i2cTransaction);
    i2cTransaction.slaveAddress = slaveAddr;
    i2cTransaction.writeBuf = (uint8_t *)&writeData[0];
    i2cTransaction.writeCount = numWriteBytes;
    i2cTransaction.readBuf = (uint8_t *)&readData[0];
    i2cTransaction.readCount = numReadBytes;
    status = I2C_transfer(handle, &i2cTransaction);
    if(FALSE == status)
    {
        printf("\n Data Transfer failed. \n");
    }
}

void padConfig_prcmEnable()
{
    /* UART Init */
    Board_initCfg   boardCfg;
    Board_STATUS    boardStatus;

#if defined(SOC_J721E)
    I2C_Params      i2cParams;
    I2C_Handle      handle = NULL;
    uint8_t         dataToSlave[4];
#endif

    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UNLOCK_MMR |
               BOARD_DEINIT_LOCK_MMR;
#if (APP_ENABLE_UART_PRINT == 1)
    boardCfg |= BOARD_INIT_UART_STDIO;
#endif
    boardStatus = Board_init(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        App_ConsolePrintf("[Error] Board init failed!!\n");
    }

#if defined (SOC_J721E)
    /* Pin mux for CAN STB used in GESI board */
    *(volatile unsigned int *)(0x0011c0f4) = 0x20007;
    /* Pinmux for MAIN_MCAN4 */
    *(volatile unsigned int *)(0x0011c020) = 0x60006;
    *(volatile unsigned int *)(0x0011c024) = 0x60006;
    /* Pinmux for MAIN_MCAN5 */
    *(volatile unsigned int *)(0x0011c04c) = 0x60006;
    *(volatile unsigned int *)(0x0011c050) = 0x60006;
    /* Pinmux for MAIN_MCAN6 */
    *(volatile unsigned int *)(0x0011c054) = 0x60006;
    *(volatile unsigned int *)(0x0011c06C) = 0x60006;
    /* Pinmux for MAIN_MCAN7 */
    *(volatile unsigned int *)(0x0011c074) = 0x60006;
    *(volatile unsigned int *)(0x0011c078) = 0x60006;
    /* Pinmux for MAIN_MCAN9 */
    *(volatile unsigned int *)(0x0011c0cc) = 0x60006;
    *(volatile unsigned int *)(0x0011c0d0) = 0x60006;
    /* Pinmux for MAIN_MCAN11 */
    *(volatile unsigned int *)(0x0011c11c) = 0x60006;
    *(volatile unsigned int *)(0x0011c120) = 0x60006;
#endif

    /* GPIO initialization */
    GPIO_init();
    /* Enable CAN transceivers by setting the STB pins */
    /* Enable the TCAN on GESI board.
     * Main Domain MCAN instances 4,5,6,7,9,11.
     */
    GPIO_write(0, GPIO_PIN_LOW);

    /* Set MCU_MCAN1_STB to LEVEL_LOW to exit CAN1 from Standby mode */
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN1_STB_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN1_STB_PIN, GPIO_PIN_LOW);

#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)
    /* Set MCU_MCAN0_EN Enable to LEVEL_HIGH to Enable MCU MCAN 0*/
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_ENABLE_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_ENABLE_PIN, GPIO_PIN_HIGH);

    /* Set MCU_MCAN0_STBz pin to LEVEL_High to exit CAN0 from Standby mode */
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_STBZ_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_STBZ_PIN, GPIO_PIN_HIGH);
#endif

#if defined (SOC_J784S4) || defined (SOC_J742S2)
    GPIOSetDirMode_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_STB_PIN, GPIO_DIRECTION_OUTPUT);
    GPIOPinWrite_v0(CSL_WKUP_GPIO0_BASE, MCU_MCAN0_STB_PIN, GPIO_PIN_LOW);
#endif
    /* Enable Main MCAN 2, GPIO0_127. */
    GPIO_write(1, GPIO_PIN_LOW);

#if defined (SOC_J721S2) || defined (SOC_J7200) || defined (SOC_J784S4) || defined (SOC_J742S2)
    for (int i=0; i< sizeof(ioExpCfg)/sizeof(ioExpCfg[0]); i++)
    {
        Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg[i]);
    }

#endif

#if defined (SOC_J721E)
    /*
     * Configuring TCA6424 IO Exp 2 with addr 0x22
     * This io expander is controlled by i2c0
     * For Main MCAN2 P13 and P14 should be set to 0, This should route the MCAN2 STB line to transciver.
     * For Main MCAN0 P06 and P07 should be set to 1.
     */
    /* I2C initialization */
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.bitRate = I2C_400kHz;
    i2cParams.transferCallbackFxn = NULL;

    handle = I2C_open(0U, &i2cParams);

    dataToSlave[0] = TCA6424_REG_CONFIG0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] = 0x0U;
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 2, NULL, 0);

    dataToSlave[0] = TCA6424_REG_INPUT0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] = 0x0U;
    dataToSlave[2] = 0x0U;
    dataToSlave[3] = 0x0U;
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 1, &dataToSlave[1], 3);

    /* Set P06 and P07 to 1.
     * Set P13 and P14 to 0.
     */
    dataToSlave[0] = TCA6424_REG_OUTPUT0 | TCA6424_CMD_AUTO_INC;
    dataToSlave[1] |= 0xC0;
    dataToSlave[2] &= ~(0x18);
    SetupI2CTransfer(handle, 0x22, &dataToSlave[0], 1, &dataToSlave[1], 3);
#endif

    boardStatus = Board_deinit(boardCfg);
    if (boardStatus != BOARD_SOK)
    {
        App_ConsolePrintf("[Error] Board de-init failed!!\n");
    }

}

void test_csl_mcan_evm_loopback_app(void)
{
    int32_t                    configStatus = CSL_PASS;
    uint32_t                   mode         = 0U, loopCnt = 0U;
    MCAN_TxBufElement          txMsg;

    /* Do Pad Config for UART and MCAN */
    padConfig_prcmEnable();

    App_ConsolePrintf("\n Starting Application...\n");
    App_ConsolePrintf("Available MCAN Module for usage:\n");

#if defined (BUILD_MCU1_0)
    gMcanModAddr = CSL_MCU_MCAN0_MSGMEM_RAM_BASE; 
#elif defined (BUILD_MCU2_1)
    gMcanModAddr = APP_MAIN_MCAN_DEF_INST0_BASE_ADDRESS;
#endif
    App_ConsolePrintf("MCAN Clock Configuration Successful.\n");

    App_ConsolePrintf("MCAN Node Type:\n");
    App_ConsolePrintf("1. Transmitter Side.\n");
    App_ConsolePrintf("2. Receiver Side.\n");
    App_ConsolePrintf("3. Loopback - no external transmission/reception \n");
    App_ConsolePrintf("4. Loopback - external transmission and reception \n");
    App_ConsolePrintf("Enter type of the node:\n");
    App_ConsoleGetNum(&mode);
    if(4 == mode)
    {
#if defined (BUILD_MCU1_0)
        gMcanModAddr_lpbk = CSL_MCU_MCAN1_MSGMEM_RAM_BASE; 
#elif defined (BUILD_MCU2_1)
        gMcanModAddr_lpbk = APP_MAIN_MCAN_DEF_INST1_BASE_ADDRESS;
#endif
    }
    /* CrossBar Configuration */
    configStatus = App_mcanRegisterIsr(gMcanModAddr);

    /* Initialize message to transmit */
    txMsg.id  = (uint32_t)((uint32_t)(0x4U) << 18U);
    txMsg.rtr = 0U;
    txMsg.xtd = 0U;
    txMsg.esi = 0U;
    txMsg.dlc = 0xFU;
    txMsg.brs = 1U;
    txMsg.fdf = 1U;
    txMsg.efc = 1U;
    txMsg.mm  = 0xAAU;
    for (loopCnt = 0; loopCnt < MCAN_MAX_PAYLOAD_BYTES; loopCnt++)
    {
        txMsg.data[loopCnt] = loopCnt;
    }

    if (3 != mode)
    {
        configStatus = App_mcanConfig(gMcanModAddr, FALSE);
        /* Check if GESI board is connected */
        #if defined (BUILD_MCU2_1) && !defined (SOC_J784S4) && !defined (SOC_J742S2)
            if(Board_detectBoard(BOARD_ID_GESI) == FALSE)
            {
                App_ConsolePrintf("\nError! GESI Board not connected\n");
            }
        #endif
    }
    else
    {
        configStatus = App_mcanConfig(gMcanModAddr, TRUE);
    }

    if(4 == mode)
    {
        configStatus = App_mcanRegisterIsr(gMcanModAddr_lpbk);
        configStatus = App_mcanConfig(gMcanModAddr_lpbk, FALSE);
    }


    if (CSL_PASS == configStatus)
    {
        switch (mode)
        {
            case 1:
                /* This is transmitter side application */
                App_ConsolePrintf("\nTransmitter Side application:\n");
                App_ConsolePrintf(
                    "This test will send 15  messages with various payload varying from 1byte to 64bytes.\n"
                    );
                App_ConsolePrintf("Message Object:\n");
                App_mcanPrintTxMsg(&txMsg);
                APP_mcanTxTest(&txMsg);
                break;

            case 2:
                /* This is receiver side application */
                App_ConsolePrintf("\nReceiver Side application:\n");
                App_ConsolePrintf(
                    "This test will receive 15  messages with various payload varying from 1byte to 64bytes.\n"
                    );
                APP_mcanRxTest(&txMsg);
                break;

            case 3:
                App_ConsolePrintf("\nLoop back application:\n");
                App_ConsolePrintf(
                    "This test will send 15  messages with various payload varying from 1byte to 64bytes.\n"
                    );
                App_ConsolePrintf(
                    "This test will attempt to receive 15  messages with various payload varying from 1byte to 64bytes.\n"
                    );
                App_ConsolePrintf("Message Object:\n");
                App_mcanPrintTxMsg(&txMsg);
                APP_mcanLpbkTest(&txMsg);
                break;

            case 4:
                App_ConsolePrintf("\nExternal Loop back application:\n");
                App_ConsolePrintf(
                    "This test will send 15  messages with various payload varying from 1byte to 64bytes.\n"
                    );
                App_ConsolePrintf(
                    "This test will attempt to receive 15  messages with various payload varying from 1byte to 64bytes.\n"
                    );
                App_ConsolePrintf("Message Object:\n");
                App_mcanPrintTxMsg(&txMsg);
                APP_mcanExtLpbkTest(&txMsg);
                break;
            default:
                App_ConsolePrintf("Wrong option...\n");
                break;
        }
    }
    App_ConsolePrintf("\n All tests have passed.\n");

#if defined (UNITY_INCLUDE_CONFIG_H)
    TEST_PASS();
#endif
    return;
}

/* ========================================================================== */
/*                 Internal Function Definitions                              */
/* ========================================================================== */
static int32_t App_mcanRegisterInterrupt(uint32_t intNum, void f(uintptr_t))
{
    int32_t configStatus = STW_SOK;
    OsalRegisterIntrParams_t    intrPrms;
    OsalInterruptRetCode_e      osalRetVal;
    HwiP_Handle                 hwiHandle = NULL;

    /* Enable CPU Interrupts and register ISR - MCAN0 Intr0 */
    Osal_RegisterInterrupt_initParams(&intrPrms);
    /* Populate the interrupt parameters */
    intrPrms.corepacConfig.arg              = (uintptr_t) NULL;
    intrPrms.corepacConfig.isrRoutine       = f;
    intrPrms.corepacConfig.priority         = 0U;
    intrPrms.corepacConfig.corepacEventNum  = 0U;
    intrPrms.corepacConfig.intVecNum        = intNum;

    /* Register interrupts */
    osalRetVal = Osal_RegisterInterrupt(&intrPrms, &hwiHandle);
    if(OSAL_INT_SUCCESS != osalRetVal)
    {
        configStatus = CSL_EFAIL;
    }
    return configStatus;
}

#if defined (BUILD_MCU1_0)
#if defined (SOC_J721E) || defined (SOC_J7200) || defined (SOC_J721S2)
static int32_t App_mcanCfgIrqRouterMain2Mcu(uint32_t devId, uint32_t offset, uint32_t intNum)
{
    int32_t retVal;
    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;

    rmIrqReq.valid_params           = TISCI_MSG_VALUE_RM_DST_ID_VALID;
    rmIrqReq.valid_params          |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;
    rmIrqReq.src_id                 = devId;
    rmIrqReq.global_event           = 0U;
    rmIrqReq.src_index              = offset;
    rmIrqReq.dst_id                 = TISCI_DEV_MCU_R5FSS0_CORE0;
    rmIrqReq.dst_host_irq           = intNum;
    rmIrqReq.ia_id                  = 0U;
    rmIrqReq.vint                   = 0U;
    rmIrqReq.vint_status_bit_index  = 0U;
    rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
    retVal = Sciclient_rmIrqSet(&rmIrqReq, &rmIrqResp, SCICLIENT_SERVICE_WAIT_FOREVER);
    if(CSL_PASS != retVal)
    {
        App_ConsolePrintf("Error in SciClient Interrupt Params Configuration!!!");
        App_ConsolePrintf("offset: %d \n", offset);
    }
    else
    {
        App_ConsolePrintf("SciClient Interrupt Params Configuration passed for offset: %d \n", offset);
    }
    return retVal;
}
#endif /* Platform specifc */
#endif /* MCU 10 only */

static int32_t App_mcanRegisterIsr(uint32_t mcanInstAddr)
{
    int32_t configStatus = STW_SOK;

#if defined (BUILD_MCU1_0)
    /* Running Code from Mcu R5 */
    /* MCU MCAN Inst 0 */
    configStatus =  App_mcanRegisterInterrupt(APP_MCU_MCAN_0_INT0, &App_mcanIntr0ISR);
    configStatus += App_mcanRegisterInterrupt(APP_MCU_MCAN_0_INT1, &App_mcanIntr1ISR);
    configStatus += App_mcanRegisterInterrupt(APP_MCU_MCAN_0_TS_INT, &App_mcanTSIntrISR);
    /* MCU MCAN Inst 1 */
    configStatus += App_mcanRegisterInterrupt(APP_MCU_MCAN_1_INT0, &App_mcanIntr0ISR);
    configStatus += App_mcanRegisterInterrupt(APP_MCU_MCAN_1_INT1, &App_mcanIntr1ISR);
    configStatus += App_mcanRegisterInterrupt(APP_MCU_MCAN_1_TS_INT, &App_mcanTSIntrISR);
#else
    /* Running Code from Main R5 */
        /* Main MCAN Default Tx Inst  */
        configStatus =  App_mcanRegisterInterrupt(APP_MAIN_MCAN_DEF_INST0_INT0, &App_mcanIntr0ISR);
        configStatus += App_mcanRegisterInterrupt(APP_MAIN_MCAN_DEF_INST0_INT1, &App_mcanIntr1ISR);
        configStatus += App_mcanRegisterInterrupt(APP_MAIN_MCAN_DEF_INST0_TS_INT, &App_mcanTSIntrISR);
        /* Main MCAN Default Rx Inst*/
        configStatus =  App_mcanRegisterInterrupt(APP_MAIN_MCAN_DEF_INST1_INT0, &App_mcanIntr0ISR);
        configStatus += App_mcanRegisterInterrupt(APP_MAIN_MCAN_DEF_INST1_INT1, &App_mcanIntr1ISR);
        configStatus += App_mcanRegisterInterrupt(APP_MAIN_MCAN_DEF_INST1_TS_INT, &App_mcanTSIntrISR);
#endif
    if(STW_SOK != configStatus)
    {
        App_ConsolePrintf("CrossBar/Interrupt Configuration failed.\n");
    }
    else
    {
        App_ConsolePrintf("CrossBar/Interrupt Configuration done.\n");
    }

    return configStatus;
}

static int32_t App_mcanConfig(uint32_t mcanInstAddr, bool enableInternalLpbk)
{
    uint32_t                   fdoe;
    int32_t                    configStatus = CSL_PASS;
    MCAN_RevisionId            revId;
    MCAN_InitParams            initParams;
    MCAN_ConfigParams          configParams;
    MCAN_MsgRAMConfigParams    msgRAMConfigParams;
    MCAN_StdMsgIDFilterElement stdFiltelem;
    MCAN_BitTimingParams       bitTimes;

    /* Initialize MCAN Init params */
    initParams.fdMode          = 0x1U;
    initParams.brsEnable       = 0x1U;
    initParams.txpEnable       = 0x0U;
    initParams.efbi            = 0x0U;
    initParams.pxhddisable     = 0x0U;
    /* To enable automatic retransmission of the packet,
     * program initParams.darEnable to "0" */
    initParams.darEnable       = 0x1U;
    initParams.wkupReqEnable   = 0x1U;
    initParams.autoWkupEnable  = 0x1U;
    initParams.emulationEnable = 0x1U;
    initParams.emulationFAck   = 0x0U;
    initParams.clkStopFAck     = 0x0U;
    initParams.wdcPreload      = 0xFFU;
    initParams.tdcEnable       = 0x1U;
    initParams.tdcConfig.tdcf  = 0xAU;
    initParams.tdcConfig.tdco  = 0x6U;
    /* Initialize MCAN Config params */
    configParams.monEnable         = 0x0U;
    configParams.asmEnable         = 0x0U;
    configParams.tsPrescalar       = 0xFU;
    configParams.tsSelect          = 0x0U;
    configParams.timeoutSelect     = MCAN_TIMEOUT_SELECT_CONT;
    configParams.timeoutPreload    = 0xFFFFU;
    configParams.timeoutCntEnable  = 0x0U;
    configParams.filterConfig.rrfs = 0x1U;
    configParams.filterConfig.rrfe = 0x1U;
    configParams.filterConfig.anfe = 0x1U;
    configParams.filterConfig.anfs = 0x1U;
    /* Initialize Message RAM Sections Configuration Parameters */
    msgRAMConfigParams.flssa                = APP_MCAN_STD_ID_FILT_START_ADDR;
    msgRAMConfigParams.lss                  = APP_MCAN_STD_ID_FILTER_NUM;
    msgRAMConfigParams.flesa                = APP_MCAN_EXT_ID_FILT_START_ADDR;
    msgRAMConfigParams.lse                  = APP_MCAN_EXT_ID_FILTER_NUM;
    msgRAMConfigParams.txStartAddr          = APP_MCAN_TX_BUFF_START_ADDR;
    msgRAMConfigParams.txBufNum             = APP_MCAN_TX_BUFF_SIZE;
    msgRAMConfigParams.txFIFOSize           = 0U;
    msgRAMConfigParams.txBufMode            = 0U;
    msgRAMConfigParams.txBufElemSize        = MCAN_ELEM_SIZE_64BYTES;
    msgRAMConfigParams.txEventFIFOStartAddr = APP_MCAN_TX_EVENT_START_ADDR;
    msgRAMConfigParams.txEventFIFOSize      = APP_MCAN_TX_BUFF_SIZE;
    msgRAMConfigParams.txEventFIFOWaterMark = 3U;
    msgRAMConfigParams.rxFIFO0startAddr     = APP_MCAN_FIFO_0_START_ADDR;
    msgRAMConfigParams.rxFIFO0size          = APP_MCAN_FIFO_0_NUM;
    msgRAMConfigParams.rxFIFO0waterMark     = 3U;
    msgRAMConfigParams.rxFIFO0OpMode        = 0U;
    msgRAMConfigParams.rxFIFO1startAddr     = APP_MCAN_FIFO_1_START_ADDR;
    msgRAMConfigParams.rxFIFO1size          = APP_MCAN_FIFO_1_NUM;
    msgRAMConfigParams.rxFIFO1waterMark     = 3U;
    msgRAMConfigParams.rxFIFO1OpMode        = 0U;
    msgRAMConfigParams.rxBufStartAddr       = APP_MCAN_RX_BUFF_START_ADDR;
    msgRAMConfigParams.rxBufElemSize        = MCAN_ELEM_SIZE_64BYTES;
    msgRAMConfigParams.rxFIFO0ElemSize      = MCAN_ELEM_SIZE_64BYTES;
    msgRAMConfigParams.rxFIFO1ElemSize      = MCAN_ELEM_SIZE_64BYTES;
    /* Initialize Tx Buffer Config params */
    stdFiltelem.sfid2 = 0x0U;
    stdFiltelem.sfid1 = 0x4U;
    stdFiltelem.sfec  = 0x7U;
    stdFiltelem.sft   = 0x0U;
    /* Initialize bit timings
     * Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively */
    bitTimes.nomRatePrescalar   = 0x7U;
    bitTimes.nomTimeSeg1        = 0x5U;
    bitTimes.nomTimeSeg2        = 0x2U;
    bitTimes.nomSynchJumpWidth  = 0x0U;
    bitTimes.dataRatePrescalar  = 0x1U;
    bitTimes.dataTimeSeg1       = 0x3U;
    bitTimes.dataTimeSeg2       = 0x2U;
    bitTimes.dataSynchJumpWidth = 0x0U;

    /* Get MCANSS Revision ID */
    MCAN_getRevisionId(mcanInstAddr, &revId);
    App_ConsolePrintf("MCANSS Revision ID:\n");
    App_ConsolePrintf("scheme:0x%x\n", revId.scheme);
    App_ConsolePrintf("Business Unit:0x%x\n", revId.bu);
    App_ConsolePrintf("Module ID:0x%x\n", revId.modId);
    App_ConsolePrintf("RTL Revision:0x%x\n", revId.rtlRev);
    App_ConsolePrintf("Major Revision:0x%x\n", revId.major);
    App_ConsolePrintf("Custom Revision:0x%x\n", revId.custom);
    App_ConsolePrintf("Minor Revision:0x%x\n", revId.minor);
    /* Enable Auto wakeup */
    fdoe = MCAN_isFDOpEnable(mcanInstAddr);
    if ((uint32_t)TRUE == fdoe)
    {
        App_ConsolePrintf("CAN-FD operation is enabled through E-Fuse.\n");
    }
    else
    {
        App_ConsolePrintf("CAN-FD operation is disabled through E-Fuse.\n");
    }
    /* wait for memory initialization to happen */
    while (FALSE == MCAN_isMemInitDone(mcanInstAddr))
    {}
    /* Get endianess value */
    App_ConsolePrintf("Endianess Value: 0x%x\n",
                       MCAN_getEndianVal(mcanInstAddr));
    /* Put MCAN in SW initialization mode */
    MCAN_setOpMode(mcanInstAddr, MCAN_OPERATION_MODE_SW_INIT);
    while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(mcanInstAddr))
    {}
    /* Initialize MCAN module */
    MCAN_init(mcanInstAddr, &initParams);
    /* Configure MCAN module */
    MCAN_config(mcanInstAddr, &configParams);
    /* Configure Bit timings */
    MCAN_setBitTime(mcanInstAddr, &bitTimes);
    /* Set Extended ID Mask */
    MCAN_setExtIDAndMask(mcanInstAddr, APP_MCAN_EXT_ID_AND_MASK);
    /* Configure Message RAM Sections */
    MCAN_msgRAMConfig(mcanInstAddr, &msgRAMConfigParams);
    /* Configure Standard ID filter element */
    MCAN_addStdMsgIDFilter(mcanInstAddr, 0U, &stdFiltelem);

    if (TRUE == enableInternalLpbk)
    {
        MCAN_lpbkModeEnable (mcanInstAddr, MCAN_LPBK_MODE_INTERNAL, TRUE);
    }

    /* Take MCAN out of the SW initialization mode */
    MCAN_setOpMode(mcanInstAddr, MCAN_OPERATION_MODE_NORMAL);

    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(mcanInstAddr))
    {}
    return configStatus;
}

static void App_mcanIntr0ISR(uintptr_t arg)
{
    uint32_t intrStatus;

    intrStatus = MCAN_getIntrStatus(gMcanModAddr);
    MCAN_clearIntrStatus(gMcanModAddr, intrStatus);
    if (MCAN_INTR_SRC_TRANS_COMPLETE ==
        (intrStatus & MCAN_INTR_SRC_TRANS_COMPLETE))
    {
        gMcanIsrIntr0Flag = 0U;
    }

    if (MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG ==
        (intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG))
    {
        gMcanIsrIntr1Flag = 0U;
    }

    intrStatus = MCAN_getIntrStatus(gMcanModAddr_lpbk);
    MCAN_clearIntrStatus(gMcanModAddr_lpbk, intrStatus);
    if (MCAN_INTR_SRC_TRANS_COMPLETE ==
        (intrStatus & MCAN_INTR_SRC_TRANS_COMPLETE))
    {
        gMcanIsrIntr0Flag_lpbk = 0U;
    }

    if (MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG ==
        (intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG))
    {
        gMcanIsrIntr1Flag_lpbk = 0U;
    }
}

static void App_mcanIntr1ISR(uintptr_t arg)
{
    uint32_t intrStatus;

    intrStatus = MCAN_getIntrStatus(gMcanModAddr);
    MCAN_clearIntrStatus(gMcanModAddr, intrStatus);

    if (MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG ==
        (intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG))
    {
        gMcanIsrIntr1Flag = 0U;
    }

    intrStatus = MCAN_getIntrStatus(gMcanModAddr_lpbk);
    MCAN_clearIntrStatus(gMcanModAddr_lpbk, intrStatus);

    if (MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG ==
        (intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG))
    {
        gMcanIsrIntr1Flag_lpbk = 0U;
    }
}

static void App_mcanTSIntrISR(uintptr_t arg)
{
    App_ConsolePrintf("Time Stamp overflow happened.\n");
}

static void App_mcanPrintTxMsg(const MCAN_TxBufElement *txMsg)
{
    uint32_t loopCnt;

    App_ConsolePrintf("\nMessage ID: 0x%x", txMsg->id);

    App_ConsolePrintf("\nMessage Remote Transmission Request: 0x%x",
                       txMsg->rtr);

    App_ConsolePrintf(
        "\nMessage Extended Frame ID(0:11Bit ID/1:29bit ID): 0x%x",
        txMsg->xtd);

    App_ConsolePrintf(
        "\nMessage Error State Indicator(0:Error Active/1:Error Passive): 0x%x",
        txMsg->esi);

    App_ConsolePrintf("\nMessage Data Length Code: 0x%x", txMsg->dlc);

    App_ConsolePrintf("\nMessage BRS: 0x%x", txMsg->brs);

    App_ConsolePrintf("\nMessage CAN FD format: 0x%x", txMsg->fdf);

    App_ConsolePrintf("\nMessage Store Tx Events: 0x%x", txMsg->efc);

    App_ConsolePrintf("\nMessage Marker: 0x%x", txMsg->mm);

    for (loopCnt = 0U; loopCnt < gMcanAppdataSize[txMsg->dlc]; loopCnt++)
    {
        App_ConsolePrintf("\nMessage DataByte%d", loopCnt);
        App_ConsolePrintf(": 0x%x", txMsg->data[loopCnt]);
    }
}

static void App_mcanPrintRxMsg(const MCAN_RxBufElement *rxMsg)
{
    uint32_t loopCnt;

    App_ConsolePrintf("\nMessage ID: 0x%x", rxMsg->id);
    App_ConsolePrintf("\nMessage Remote Transmission Request: 0x%x", rxMsg->rtr);
    App_ConsolePrintf(
        "\nMessage Extended Frame ID(0:11Bit ID/1:29bit ID): 0x%x",
        rxMsg->xtd);
    App_ConsolePrintf(
        "\nMessage Error State Indicator(0:Error Active/1:Error Passive): 0x%x",
        rxMsg->esi);
    App_ConsolePrintf("\nMessage TimeStamp: 0x%x", rxMsg->rxts);
    App_ConsolePrintf("\nMessage Data Length Code: 0x%x", rxMsg->dlc);
    App_ConsolePrintf("\nMessage BRS: 0x%x", rxMsg->brs);
    App_ConsolePrintf("\nMessage CAN FD format: 0x%x", rxMsg->fdf);
    App_ConsolePrintf("\nMessage Filter Index: 0x%x", rxMsg->fidx);
    App_ConsolePrintf("\nMessage Accept Non-matching Frame: 0x%x", rxMsg->anmf);
    for (loopCnt = 0U; loopCnt < gMcanAppdataSize[rxMsg->dlc]; loopCnt++)
    {
        App_ConsolePrintf("\nMessage DataByte%d", loopCnt);
        App_ConsolePrintf(": 0x%x", rxMsg->data[loopCnt]);
    }
}

static void APP_mcanTxTest(MCAN_TxBufElement *txMsg)
{
    int32_t  testStatus = CSL_PASS;
    uint32_t loopCnt      = 1U;
    MCAN_ProtocolStatus protStatus;

    /* Enable Interrupts */
    MCAN_enableIntr(gMcanModAddr, MCAN_INTR_MASK_ALL, (uint32_t)TRUE);
    MCAN_enableIntr(gMcanModAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
    /* Select Interrupt Line */
    MCAN_selectIntrLine(gMcanModAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_0);
    /* Enable Interrupt Line */
    MCAN_enableIntrLine(gMcanModAddr,
                        MCAN_INTR_LINE_NUM_0,
                        1U);
    /* Enable Transmission interrupt */
    testStatus = MCAN_txBufTransIntrEnable(gMcanModAddr,
                                           1U,
                                           (uint32_t)TRUE);
    if (CSL_PASS != testStatus)
    {
        App_ConsolePrintf("\nError in enabling buffer Transmit interrupt...\n");
    }
    else
    {
        for (loopCnt = 1U; loopCnt < 16U; loopCnt++)
        {
            txMsg->dlc = loopCnt;
            /* Write message to Msg RAM */
            MCAN_writeMsgRam(gMcanModAddr,
                             MCAN_MEM_TYPE_BUF,
                             1U,
                             txMsg);
            /* Add request for transmission */
            testStatus = MCAN_txBufAddReq(gMcanModAddr, 1U);
            if (CSL_PASS != testStatus)
            {
                App_ConsolePrintf("\nError in Adding Transmission Request...\n");
                break;
            }
            while (gMcanIsrIntr0Flag)
            {}
            gMcanIsrIntr0Flag = 1U;
            MCAN_getProtocolStatus(gMcanModAddr, &protStatus);
            /* Checking for Errors */
            if (((MCAN_ERR_CODE_NO_ERROR == protStatus.lastErrCode) ||
                 (MCAN_ERR_CODE_NO_CHANGE == protStatus.lastErrCode)) &&
                ((MCAN_ERR_CODE_NO_ERROR == protStatus.dlec) ||
                 (MCAN_ERR_CODE_NO_CHANGE == protStatus.dlec)) &&
                (0U == protStatus.pxe))
            {
                App_ConsolePrintf(
                    "\nMessage successfully transferred with payload Bytes:%d",
                    gMcanAppdataSize[txMsg->dlc]);
            }
            else
            {
                App_ConsolePrintf(
                "\nError in transmission with payload Bytes:%d",
                gMcanAppdataSize[txMsg->dlc]);
                testStatus = CSL_EFAIL;
                break;
            }
        }
    }
    if (CSL_EFAIL == testStatus)
    {
        App_ConsolePrintf("\nTx Test FAILED...\n");
    }
    else
    {
        App_ConsolePrintf("\nTx Test PASSED...\n");
    }
}

static void APP_mcanRxTest(const MCAN_TxBufElement *txMsg)
{
    uint32_t             loopCnt    = 1U, chkCnt = 0U, errFlag = 0U;
    int32_t              testStatus = CSL_PASS;
    MCAN_RxBufElement    rxMsg;
    MCAN_RxNewDataStatus newDataStatus;
    MCAN_ErrCntStatus    errCounter;

    /* Enable Interrupts */
    MCAN_enableIntr(gMcanModAddr, MCAN_INTR_MASK_ALL, (uint32_t)TRUE);
    MCAN_enableIntr(gMcanModAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
    /* Select Interrupt Line */
    MCAN_selectIntrLine(gMcanModAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_1);
    /* Enable Interrupt Line */
    MCAN_enableIntrLine(gMcanModAddr,
                        MCAN_INTR_LINE_NUM_1,
                        1U);
    for (loopCnt = 1U; loopCnt < 16U; loopCnt++)
    {
        while (gMcanIsrIntr1Flag)
        {}
        gMcanIsrIntr1Flag = 1U;
        /* Checking for Errors */
        MCAN_getErrCounters(gMcanModAddr, &errCounter);
        if ((0U == errCounter.recErrCnt) &&
            (0U == errCounter.canErrLogCnt))
        {
            MCAN_getNewDataStatus(gMcanModAddr, &newDataStatus);
            MCAN_clearNewDataStatus(gMcanModAddr, &newDataStatus);
            MCAN_readMsgRam(gMcanModAddr,
                            MCAN_MEM_TYPE_BUF,
                            0U,
                            0U,
                            &rxMsg);
            errFlag = 0U;
            for (chkCnt = 0U; chkCnt < gMcanAppdataSize[rxMsg.dlc]; chkCnt++)
            {
                if (txMsg->data[chkCnt] != rxMsg.data[chkCnt])
                {
                    errFlag = 1U;
                    break;
                }
            }
            if (0U == errFlag)
            {
                App_ConsolePrintf(
                    "\nMessage successfully received with payload Bytes: %d",
                    gMcanAppdataSize[rxMsg.dlc]);
            }
            else
            {
                App_ConsolePrintf(
                    "\nWrong data received in message with payload Bytes: ",
                    gMcanAppdataSize[rxMsg.dlc]);
                testStatus = CSL_EFAIL;
            }
        }
        else
        {
            App_ConsolePrintf(
            "\nError in reception with payload Bytes:%d",
            gMcanAppdataSize[txMsg->dlc]);
            testStatus = CSL_EFAIL;
        }
    }
    App_ConsolePrintf("\nReceived last message with following details:");
    App_mcanPrintRxMsg(&rxMsg);
    if (CSL_EFAIL == testStatus)
    {
        App_ConsolePrintf("\nRx Test FAILED...\n");
    }
    else
    {
        App_ConsolePrintf("\nRx Test PASSED...\n");
    }
}

static void APP_mcanExtLpbkTest(MCAN_TxBufElement *txMsg)
{
    int32_t  testStatus = CSL_PASS;
    MCAN_ProtocolStatus protStatus;
    MCAN_RxBufElement    rxMsg;
    MCAN_RxNewDataStatus newDataStatus;
    MCAN_ErrCntStatus    errCounter;
    uint32_t loopCnt, chkCnt, errFlag;

    /* Enable Transmission Interrupts for Tx CAN instance */
    MCAN_enableIntr(gMcanModAddr, MCAN_INTR_MASK_ALL, (uint32_t)TRUE);
    MCAN_enableIntr(gMcanModAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);

    /* Enable Transmission Interrupts for Rx CAN instance */
    MCAN_enableIntr(gMcanModAddr_lpbk, MCAN_INTR_MASK_ALL, (uint32_t)TRUE);
    MCAN_enableIntr(gMcanModAddr_lpbk,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);


    /* Select Interrupt Line for Tx CAN instance */
    MCAN_selectIntrLine(gMcanModAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_0);
    /* Select Interrupt Line for Rx CAN instance */
    MCAN_selectIntrLine(gMcanModAddr_lpbk,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_0);



    /* Enable Interrupt Line for Tx CAN instance */
    MCAN_enableIntrLine(gMcanModAddr,
                        MCAN_INTR_LINE_NUM_0,
                        1U);

    /* Enable Interrupt Line for Rx CAN instance*/
    MCAN_enableIntrLine(gMcanModAddr_lpbk,
                        MCAN_INTR_LINE_NUM_0,
                        1U);

    /* Enable Transmission interrupt */
    testStatus = MCAN_txBufTransIntrEnable(gMcanModAddr,
                                           1U,
                                           (uint32_t)TRUE);
#if defined(UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, testStatus);
#endif

    if (CSL_PASS != testStatus)
    {
        App_ConsolePrintf("\nError in enabling buffer Transmit interrupt...\n");
    }
    else
    {
        for (loopCnt = 1U; loopCnt < 16U; loopCnt++)
        {
            txMsg->dlc = loopCnt;
            /* Write message to Msg RAM */
            MCAN_writeMsgRam(gMcanModAddr,
                             MCAN_MEM_TYPE_BUF,
                             1U,
                             txMsg);
            /* Add request for transmission */
            testStatus = MCAN_txBufAddReq(gMcanModAddr, 1U);
#if defined(UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_INT32(CSL_PASS, testStatus);
#endif
            if (CSL_PASS != testStatus)
            {
                App_ConsolePrintf("\nError in Adding Transmission Request...\n");
                break;
            }
            while (gMcanIsrIntr0Flag)
            {}
            gMcanIsrIntr0Flag = 1U;
            MCAN_getProtocolStatus(gMcanModAddr, &protStatus);
            /* Checking for Errors */
            if (((MCAN_ERR_CODE_NO_ERROR == protStatus.lastErrCode) ||
                 (MCAN_ERR_CODE_NO_CHANGE == protStatus.lastErrCode)) &&
                ((MCAN_ERR_CODE_NO_ERROR == protStatus.dlec) ||
                 (MCAN_ERR_CODE_NO_CHANGE == protStatus.dlec)) &&
                (0U == protStatus.pxe))
            {
                App_ConsolePrintf(
                    "\nMessage successfully transferred with payload Bytes:%d",
                    gMcanAppdataSize[txMsg->dlc]);
            }
            else
            {
                App_ConsolePrintf(
                "\nError in transmission with payload Bytes:%d",
                gMcanAppdataSize[txMsg->dlc]);
                testStatus = CSL_EFAIL;
                break;
            }

            while (gMcanIsrIntr1Flag_lpbk)
            {}
            gMcanIsrIntr1Flag_lpbk = 1U;
            /* Checking for Errors */
            MCAN_getErrCounters(gMcanModAddr_lpbk, &errCounter);

#if defined(UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_UINT32(0U, errCounter.recErrCnt);
            TEST_ASSERT_EQUAL_UINT32(0U, errCounter.canErrLogCnt);
#endif
            if ((0U == errCounter.recErrCnt) &&
                (0U == errCounter.canErrLogCnt))
            {
                MCAN_getNewDataStatus(gMcanModAddr_lpbk, &newDataStatus);
                MCAN_clearNewDataStatus(gMcanModAddr_lpbk, &newDataStatus);
                MCAN_readMsgRam(gMcanModAddr_lpbk,
                                MCAN_MEM_TYPE_BUF,
                                0U,
                                0U,
                                &rxMsg);
                errFlag = 0U;
                for (chkCnt = 0U; chkCnt < gMcanAppdataSize[rxMsg.dlc]; chkCnt++)
                {
                    if (txMsg->data[chkCnt] != rxMsg.data[chkCnt])
                    {
                        errFlag = 1U;
                        break;
                    }
                }
#if defined(UNITY_INCLUDE_CONFIG_H)
                TEST_ASSERT_EQUAL_UINT32(0U, errFlag);
#endif
                if (0U == errFlag)
                {
                    App_ConsolePrintf(
                        "\nMessage successfully received with payload Bytes: %d",
                        gMcanAppdataSize[rxMsg.dlc]);
                }
                else
                {
                    App_ConsolePrintf(
                        "\nWrong data received in message with payload Bytes: ",
                        gMcanAppdataSize[rxMsg.dlc]);
                    testStatus = CSL_EFAIL;
                }
            }
            else
            {
                App_ConsolePrintf(
                "\nError in reception with payload Bytes:%d",
                gMcanAppdataSize[txMsg->dlc]);
                testStatus = CSL_EFAIL;
            }
        }
    }

#if defined(UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, testStatus);
#endif

    if (CSL_EFAIL == testStatus)
    {
        App_ConsolePrintf("\nLoopback Test FAILED...\n");
    }
    else
    {
        App_ConsolePrintf("\nLoopback Test PASSED...\n");
    }
}

static void APP_mcanLpbkTest(MCAN_TxBufElement *txMsg)
{
    int32_t  testStatus = CSL_PASS;
    MCAN_ProtocolStatus protStatus;
    MCAN_RxBufElement    rxMsg;
    MCAN_RxNewDataStatus newDataStatus;
    MCAN_ErrCntStatus    errCounter;
    uint32_t loopCnt, chkCnt, errFlag;

    /* Enable Transmission Interrupts */
    MCAN_enableIntr(gMcanModAddr, MCAN_INTR_MASK_ALL, (uint32_t)TRUE);
    MCAN_enableIntr(gMcanModAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
    /* Select Interrupt Line */
    MCAN_selectIntrLine(gMcanModAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_0);
    /* Enable Interrupt Line */
    MCAN_enableIntrLine(gMcanModAddr,
                        MCAN_INTR_LINE_NUM_0,
                        1U);

    /* Enable Transmission interrupt */
    testStatus = MCAN_txBufTransIntrEnable(gMcanModAddr,
                                           1U,
                                           (uint32_t)TRUE);
#if defined(UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, testStatus);
#endif

    if (CSL_PASS != testStatus)
    {
        App_ConsolePrintf("\nError in enabling buffer Transmit interrupt...\n");
    }
    else
    {
        for (loopCnt = 1U; loopCnt < 16U; loopCnt++)
        {
            txMsg->dlc = loopCnt;
            /* Write message to Msg RAM */
            MCAN_writeMsgRam(gMcanModAddr,
                             MCAN_MEM_TYPE_BUF,
                             1U,
                             txMsg);
            /* Add request for transmission */
            testStatus = MCAN_txBufAddReq(gMcanModAddr, 1U);
#if defined(UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_INT32(CSL_PASS, testStatus);
#endif
            if (CSL_PASS != testStatus)
            {
                App_ConsolePrintf("\nError in Adding Transmission Request...\n");
                break;
            }
            while (gMcanIsrIntr0Flag)
            {}
            gMcanIsrIntr0Flag = 1U;
            MCAN_getProtocolStatus(gMcanModAddr, &protStatus);
            /* Checking for Errors */
            if (((MCAN_ERR_CODE_NO_ERROR == protStatus.lastErrCode) ||
                 (MCAN_ERR_CODE_NO_CHANGE == protStatus.lastErrCode)) &&
                ((MCAN_ERR_CODE_NO_ERROR == protStatus.dlec) ||
                 (MCAN_ERR_CODE_NO_CHANGE == protStatus.dlec)) &&
                (0U == protStatus.pxe))
            {
                App_ConsolePrintf(
                    "\nMessage successfully transferred with payload Bytes:%d",
                    gMcanAppdataSize[txMsg->dlc]);
            }
            else
            {
                App_ConsolePrintf(
                "\nError in transmission with payload Bytes:%d",
                gMcanAppdataSize[txMsg->dlc]);
                testStatus = CSL_EFAIL;
                break;
            }

            while (gMcanIsrIntr1Flag)
            {}
            gMcanIsrIntr1Flag = 1U;
            /* Checking for Errors */
            MCAN_getErrCounters(gMcanModAddr, &errCounter);

#if defined(UNITY_INCLUDE_CONFIG_H)
            TEST_ASSERT_EQUAL_UINT32(0U, errCounter.recErrCnt);
            TEST_ASSERT_EQUAL_UINT32(0U, errCounter.canErrLogCnt);
#endif
            if ((0U == errCounter.recErrCnt) &&
                (0U == errCounter.canErrLogCnt))
            {
                MCAN_getNewDataStatus(gMcanModAddr, &newDataStatus);
                MCAN_clearNewDataStatus(gMcanModAddr, &newDataStatus);
                MCAN_readMsgRam(gMcanModAddr,
                                MCAN_MEM_TYPE_BUF,
                                0U,
                                0U,
                                &rxMsg);
                errFlag = 0U;
                for (chkCnt = 0U; chkCnt < gMcanAppdataSize[rxMsg.dlc]; chkCnt++)
                {
                    if (txMsg->data[chkCnt] != rxMsg.data[chkCnt])
                    {
                        errFlag = 1U;
                        break;
                    }
                }
#if defined(UNITY_INCLUDE_CONFIG_H)
                TEST_ASSERT_EQUAL_UINT32(0U, errFlag);
#endif
                if (0U == errFlag)
                {
                    App_ConsolePrintf(
                        "\nMessage successfully received with payload Bytes: %d",
                        gMcanAppdataSize[rxMsg.dlc]);
                }
                else
                {
                    App_ConsolePrintf(
                        "\nWrong data received in message with payload Bytes: ",
                        gMcanAppdataSize[rxMsg.dlc]);
                    testStatus = CSL_EFAIL;
                }
            }
            else
            {
                App_ConsolePrintf(
                "\nError in reception with payload Bytes:%d",
                gMcanAppdataSize[txMsg->dlc]);
                testStatus = CSL_EFAIL;
            }
        }
    }

#if defined(UNITY_INCLUDE_CONFIG_H)
    TEST_ASSERT_EQUAL_INT32(CSL_PASS, testStatus);
#endif

    if (CSL_EFAIL == testStatus)
    {
        App_ConsolePrintf("\nLoopback Test FAILED...\n");
    }
    else
    {
        App_ConsolePrintf("\nLoopback Test PASSED...\n");
    }
}


void App_ConsolePrintf(const char *pcString, ...)
{
    static char printBuffer[APP_PRINT_BUFFER_SIZE];
    va_list arguments;

    /* Start the varargs processing. */
    va_start(arguments, pcString);
    vsnprintf (printBuffer, sizeof(printBuffer), pcString, arguments);
#if (APP_ENABLE_UART_PRINT == 1)
    UART_printf(printBuffer);
#else
    printf(printBuffer);
#endif

    /* End the varargs processing. */
    va_end(arguments);
}

static void App_ConsoleGetNum(uint32_t *num)
{
#if (APP_ENABLE_UART_PRINT == 1)
    uint32_t  status;
    status = UART_scanFmt("%d", num);
    if (status != S_PASS)
    {
        App_ConsolePrintf("UART Scan failed \n");
    }
#else
    scanf("%d", (int32_t *)num);
#endif
}

#ifdef UNITY_INCLUDE_CONFIG_H
/*
 *  ======== Unity set up and tear down ========
 */
void setUp(void)
{
    /* Do nothing */
}

void tearDown(void)
{
    /* Do nothing */
}
#endif

void test_csl_mcan_evm_loopback_app_runner(void)
{
    /* @description:Test runner for mcan tests

       @requirements: PDK-1694

       @cores: mcu1_0 mcu2_1 */
#if defined(UNITY_INCLUDE_CONFIG_H)
    UNITY_BEGIN();
    RUN_TEST (test_csl_mcan_evm_loopback_app);
    UNITY_END();
    /* Function to print results defined in our unity_config.h file */
    print_unityOutputBuffer_usingUARTstdio();
#else
    test_csl_mcan_evm_loopback_app();
#endif
    return;
}

int main(void)
{
    (void) test_csl_mcan_evm_loopback_app_runner();

    while (true)
    {
    }
}
