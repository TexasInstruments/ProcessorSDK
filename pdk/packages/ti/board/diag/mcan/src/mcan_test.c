/******************************************************************************
 * Copyright (c) 2018-2022 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file     mcan_test.c
 *
 *  \brief    This file contains MCAN sample code.
 *
 *  Targeted Functionality: Verifying the transmit and receive functionality
 *    of MCAN module.
 *
 *  Operation: MCAN operational mode is set to CAN-FD. This test will need
 *  two MCAN ports.
 *
 *  Supported SoCs: AM65XX, J721E, J7200,AM64x, TPR12, AWR294X, J721S2
 *
 *  Supported Platforms: am65xx_idk, j721e_evm, j7200_evm, am64x_evm, am64x_svb, tpr12_evm, awr294x_evm, j721s2_evm, j784s4_evm
 *
 */

#include "mcan_test.h"

#if defined(SOC_AWR294X)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ti/drv/mibspi/MIBSPI.h>
#include <ti/drv/mibspi/soc/MIBSPI_soc.h>
#include <ti/drv/edma/edma.h>
#endif

MCAN_TxBufElement  txMsg;
MCAN_RxBufElement  rxMsg;

volatile uint32_t gMcanIsrIntr0Flag = UTRUE;
volatile uint32_t gMcanIsrIntr1Flag = UTRUE;

uint32_t expBoardDetect = UFALSE;
uint32_t mcanMaxPorts   = 0U;
uint32_t startPort      = 0U;

#if defined(am65xx_idk)
BoardDiag_McanPortInfo_t gMcanDiagPortInfo[MCAN_MAX_PORTS] = 
{{CSL_MCU_MCAN0_MSGMEM_RAM_BASE, 0, CSL_GIC0_INTR_MCU_MCAN0_BUS_MCANSS_MCAN_LVL_INT_0, CSL_GIC0_INTR_MCU_MCAN0_BUS_MCANSS_MCAN_LVL_INT_1, CSL_GIC0_INTR_MCU_MCAN0_BUS_MCANSS_EXT_TS_ROLLOVER_LVL_INT}, 
 {CSL_MCU_MCAN1_MSGMEM_RAM_BASE, 1, CSL_GIC0_INTR_MCU_MCAN1_BUS_MCANSS_MCAN_LVL_INT_0, CSL_GIC0_INTR_MCU_MCAN1_BUS_MCANSS_MCAN_LVL_INT_1, CSL_GIC0_INTR_MCU_MCAN1_BUS_MCANSS_EXT_TS_ROLLOVER_LVL_INT}
};
#elif defined(j7200_evm)
BoardDiag_McanPortInfo_t gMcanDiagPortInfo[MCAN_MAX_PORTS_EXP] =
{{CSL_MCU_MCAN0_MSGMEM_RAM_BASE, 0, MCU_MCAN0_TX_INT_NUM,  MCU_MCAN0_RX_INT_NUM,  MCU_MCAN0_TS_INT_NUM},
 {CSL_MCU_MCAN1_MSGMEM_RAM_BASE, 1, MCU_MCAN1_TX_INT_NUM,  MCU_MCAN1_RX_INT_NUM,  MCU_MCAN1_TS_INT_NUM},
 {CSL_MCAN0_MSGMEM_RAM_BASE,     0, MAIN_MCAN0_TX_INT_NUM, MAIN_MCAN0_RX_INT_NUM, MAIN_MCAN0_TS_INT_NUM},
 {CSL_MCAN3_MSGMEM_RAM_BASE,     3, MAIN_MCAN3_TX_INT_NUM, MAIN_MCAN3_RX_INT_NUM, MAIN_MCAN3_TS_INT_NUM},
 {CSL_MCAN4_MSGMEM_RAM_BASE,     4, MAIN_MCAN4_TX_INT_NUM, MAIN_MCAN4_RX_INT_NUM, MAIN_MCAN4_TS_INT_NUM},
 {CSL_MCAN5_MSGMEM_RAM_BASE,     5, MAIN_MCAN5_TX_INT_NUM, MAIN_MCAN5_RX_INT_NUM, MAIN_MCAN5_TS_INT_NUM},
 {CSL_MCAN6_MSGMEM_RAM_BASE,     6, MAIN_MCAN6_TX_INT_NUM, MAIN_MCAN6_RX_INT_NUM, MAIN_MCAN6_TS_INT_NUM},
 {CSL_MCAN7_MSGMEM_RAM_BASE,     7, MAIN_MCAN7_TX_INT_NUM, MAIN_MCAN7_RX_INT_NUM, MAIN_MCAN7_TS_INT_NUM},
 {CSL_MCAN8_MSGMEM_RAM_BASE,     8, MAIN_MCAN8_TX_INT_NUM, MAIN_MCAN8_RX_INT_NUM, MAIN_MCAN8_TS_INT_NUM},
 {CSL_MCAN10_MSGMEM_RAM_BASE,   10, MAIN_MCAN10_TX_INT_NUM, MAIN_MCAN10_RX_INT_NUM, MAIN_MCAN10_TS_INT_NUM}
};
#elif defined(j721s2_evm)
BoardDiag_McanPortInfo_t gMcanDiagPortInfo[MCAN_MAX_PORTS_CP] =
{{CSL_MCU_MCAN0_MSGMEM_RAM_BASE, 0, MCU_MCAN0_TX_INT_NUM,  MCU_MCAN0_RX_INT_NUM,  MCU_MCAN0_TS_INT_NUM},
 {CSL_MCU_MCAN1_MSGMEM_RAM_BASE, 1, MCU_MCAN1_TX_INT_NUM,  MCU_MCAN1_RX_INT_NUM,  MCU_MCAN1_TS_INT_NUM},
 {CSL_MCAN3_MSGMEM_RAM_BASE,     3, MAIN_MCAN3_TX_INT_NUM, MAIN_MCAN3_RX_INT_NUM, MAIN_MCAN3_TS_INT_NUM},
 {CSL_MCAN5_MSGMEM_RAM_BASE,     5, MAIN_MCAN5_TX_INT_NUM, MAIN_MCAN5_RX_INT_NUM, MAIN_MCAN5_TS_INT_NUM}
};
#elif defined(j784s4_evm)
BoardDiag_McanPortInfo_t gMcanDiagPortInfo[MCAN_MAX_PORTS_CP] =
{{CSL_MCU_MCAN0_MSGMEM_RAM_BASE, 0, MCU_MCAN0_TX_INT_NUM,  MCU_MCAN0_RX_INT_NUM,  MCU_MCAN0_TS_INT_NUM},
 {CSL_MCU_MCAN1_MSGMEM_RAM_BASE, 1, MCU_MCAN1_TX_INT_NUM,  MCU_MCAN1_RX_INT_NUM,  MCU_MCAN1_TS_INT_NUM},
 {CSL_MCAN3_MSGMEM_RAM_BASE,     3, MAIN_MCAN3_TX_INT_NUM, MAIN_MCAN3_RX_INT_NUM, MAIN_MCAN3_TS_INT_NUM},
 {CSL_MCAN5_MSGMEM_RAM_BASE,     5, MAIN_MCAN5_TX_INT_NUM, MAIN_MCAN5_RX_INT_NUM, MAIN_MCAN5_TS_INT_NUM},
 {CSL_MCAN16_MSGMEM_RAM_BASE,    16, MAIN_MCAN16_TX_INT_NUM, MAIN_MCAN16_RX_INT_NUM, MAIN_MCAN16_TS_INT_NUM},
 {CSL_MCAN4_MSGMEM_RAM_BASE,     4, MAIN_MCAN4_TX_INT_NUM, MAIN_MCAN4_RX_INT_NUM, MAIN_MCAN4_TS_INT_NUM}
};
#elif defined(SOC_AM64X)
BoardDiag_McanPortInfo_t  gMcanDiagPortInfo[MCAN_MAX_PORTS] =
{{CSL_MCAN0_MSGMEM_RAM_BASE,     0, MAIN_MCAN0_TX_INT_NUM, MAIN_MCAN0_RX_INT_NUM, MAIN_MCAN0_TS_INT_NUM},
 {CSL_MCAN1_MSGMEM_RAM_BASE,     1, MAIN_MCAN1_TX_INT_NUM, MAIN_MCAN1_RX_INT_NUM, MAIN_MCAN1_TS_INT_NUM},
};

#elif (defined(tpr12_evm) || defined(awr294x_evm))
BoardDiag_McanPortInfo_t  gMcanDiagPortInfo[MCAN_MAX_PORTS] = {{CSL_MSS_MCANA_MSG_RAM_U_BASE,     0, MAIN_MCAN0_TX_INT_NUM, MAIN_MCAN0_RX_INT_NUM, MAIN_MCAN0_TS_INT_NUM},
                                                       {CSL_MSS_MCANB_MSG_RAM_U_BASE,     1, MAIN_MCAN1_TX_INT_NUM, MAIN_MCAN1_RX_INT_NUM, MAIN_MCAN1_TS_INT_NUM},
                                                      };
#else
BoardDiag_McanPortInfo_t gMcanDiagPortInfo[MCAN_MAX_PORTS_EXP] =
{{CSL_MCU_MCAN0_MSGMEM_RAM_BASE, 0, MCU_MCAN0_TX_INT_NUM,  MCU_MCAN0_RX_INT_NUM,  MCU_MCAN0_TS_INT_NUM},
 {CSL_MCU_MCAN1_MSGMEM_RAM_BASE, 1, MCU_MCAN1_TX_INT_NUM,  MCU_MCAN1_RX_INT_NUM,  MCU_MCAN1_TS_INT_NUM},
 {CSL_MCAN0_MSGMEM_RAM_BASE,     0, MAIN_MCAN0_TX_INT_NUM, MAIN_MCAN0_RX_INT_NUM, MAIN_MCAN0_TS_INT_NUM},
 {CSL_MCAN2_MSGMEM_RAM_BASE,     2, MAIN_MCAN2_TX_INT_NUM, MAIN_MCAN2_RX_INT_NUM, MAIN_MCAN2_TS_INT_NUM},
 {CSL_MCAN4_MSGMEM_RAM_BASE,     4, MAIN_MCAN4_TX_INT_NUM, MAIN_MCAN4_RX_INT_NUM, MAIN_MCAN4_TS_INT_NUM},
 {CSL_MCAN5_MSGMEM_RAM_BASE,     5, MAIN_MCAN5_TX_INT_NUM, MAIN_MCAN5_RX_INT_NUM, MAIN_MCAN5_TS_INT_NUM},
 {CSL_MCAN6_MSGMEM_RAM_BASE,     6, MAIN_MCAN6_TX_INT_NUM, MAIN_MCAN6_RX_INT_NUM, MAIN_MCAN6_TS_INT_NUM},
 {CSL_MCAN7_MSGMEM_RAM_BASE,     7, MAIN_MCAN7_TX_INT_NUM, MAIN_MCAN7_RX_INT_NUM, MAIN_MCAN7_TS_INT_NUM},
 {CSL_MCAN9_MSGMEM_RAM_BASE,     9, MAIN_MCAN9_TX_INT_NUM, MAIN_MCAN9_RX_INT_NUM, MAIN_MCAN9_TS_INT_NUM},
 {CSL_MCAN11_MSGMEM_RAM_BASE,   11, MAIN_MCAN11_TX_INT_NUM, MAIN_MCAN11_RX_INT_NUM, MAIN_MCAN11_TS_INT_NUM}
};
#endif

#if defined(SOC_AWR294X)
extern GPIO_v2_Config GPIO_v2_config;
EDMA_Handle gDmaHandle;

/**
 * \brief   Function to read PMIC register
 *
 * \param   handle    [IN]  - SPI Handle
 * \param   regOffset [IN]  - Register offset
 *
 * \return  uint8_t Register Value
 *
 */
static uint8_t BoardDiag_mcanReadPmicReg(MIBSPI_Handle handle, uint8_t regOffset)
{
    uint8_t tx[4];
    uint8_t rx[4];
    MIBSPI_Transaction transaction;

    memset(&transaction, 0, sizeof(transaction));

    /* Configure Data Transfer */
    transaction.count = 3;
    transaction.txBuf = tx;
    transaction.rxBuf = rx;
    transaction.slaveIndex = 0;
    tx[0] = regOffset;
    tx[1] = 0x10;
    tx[2] = 0;
    /* Start Data Transfer */
    MIBSPI_transfer(handle, &transaction);

    return rx[2];
}

/**
 * \brief   Function to write PMIC register
 *
 * \param   handle  [IN]  - SPI Handle
 * \param   regAddr [IN]  - Register Address
 * \param   val     [IN]  - Register Value
 *
 */
static void BoardDiag_mcanWritePmicReg(MIBSPI_Handle handle, uint8_t regAddr, uint8_t val)
{
    uint8_t tx[4];
    MIBSPI_Transaction transaction;

    memset(&transaction, 0, sizeof(transaction));

    /* Configure Data Transfer */
    transaction.count = 3;
    transaction.txBuf = tx;
    transaction.rxBuf = NULL;
    transaction.slaveIndex = 0;
    tx[0] = regAddr;
    tx[1] = 0;
    tx[2] = val;

    CacheP_wbInv((void *) tx, 3);
    /* Start Data Transfer */
    MIBSPI_transfer(handle, &transaction);
}

/**
 * \brief   Function to open edma
 *
 * \param   devInstance  [IN]  - SPI Instance
 *
 */
static int32_t BoardDiag_mcanEdmaOpen(uint8_t devInstance)
{
    EDMA_instanceInfo_t instanceInfo;
    int32_t errorCode;
    MibSpi_HwCfg cfg;
    int32_t retVal = 0;

    retVal = MIBSPI_socGetInitCfg((enum MibSpi_InstanceId)devInstance, &cfg);

    gDmaHandle = EDMA_getHandle(cfg.edmaCCId, &instanceInfo);
    if(gDmaHandle == NULL)
    {
        EDMA3CCInitParams 	initParam;

        EDMA3CCInitParams_init(&initParam);
        initParam.initParamSet = UTRUE;
        if (EDMA_init(cfg.edmaCCId, &initParam) != EDMA_NO_ERROR)
        {
            printf("EDMA_init failed \n");
            return -1;
        }
        /* Open DMA driver instance 0 for SPI test */
        gDmaHandle = EDMA_open(cfg.edmaCCId, &errorCode, &instanceInfo);
    }

    if(gDmaHandle == NULL)
    {
        printf("Open DMA driver failed with error=%d\n", retVal);
        return -1;
    }

    return 0;
}

/**
 * \brief   Function to configure CAN STB pin through PMIC
 *
 * \param   devInstance  [IN]  - SPI Instance
 *
 */
int32_t BoardDiag_mcanConfigSTB(uint8_t devInstance)
{
    MIBSPI_Params     params;
    MIBSPI_Handle     handle;
    uint8_t           regAddr;
    uint8_t           regValue;

    if(BoardDiag_mcanEdmaOpen(devInstance))
    {
        UART_printf("DMA Open Failied\n");
        return NULL;
    }

    /* Initialize the SPI */
    MIBSPI_init();

    /* Setup the default SPI Parameters */
    MIBSPI_Params_init(&params);

    /* Disble DMA */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;

    /* Set SPI in master mode */
    params.mode = MIBSPI_MASTER;
    params.u.masterParams.bitRate = 1000000U;
    params.pinMode = MIBSPI_PINMODE_4PIN_CS;
    params.dataSize = 8U;
    params.frameFormat = MIBSPI_POL0_PHA1;
    params.csHold = 1;

    params.u.masterParams.numSlaves = 1U;
    params.u.masterParams.t2cDelay  = 0x5,                   /* t2cDelay */
    params.u.masterParams.c2tDelay  = 0x5,                /* c2tDelay */
    params.u.masterParams.wDelay    = 0,                   /* wDelay */
    params.u.masterParams.slaveProf[0].chipSelect = 0U;
    params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
    params.u.masterParams.slaveProf[0].dmaReqLine = 0U;

    /* Open the SPI Instance for MibSpi */
    handle = MIBSPI_open((enum MibSpi_InstanceId)devInstance, &params);

    regAddr = 0x3D;
    regValue = BoardDiag_mcanReadPmicReg(handle, regAddr);

    regValue = regValue | 0x1;
    BOARD_delay(10000);
    BoardDiag_mcanWritePmicReg(handle, regAddr, regValue);

    regValue = BoardDiag_mcanReadPmicReg(handle, regAddr);
    if((regValue & 0x1) != 0x1)
    {
        MIBSPI_close(handle);
        return -1;
    }

    MIBSPI_close(handle);
    return 0;
}

#endif

/**
 * \brief   This function will configure MCAN module
 *
 * \param   index  [IN]  - MCAN index number
 *
 * \return  int8_t
 *             0   - in case of success
 *            -1   - in case of failure
 *
 */
static int8_t BoardDiag_mcanConfig(uint8_t index)
{
    uint32_t                   fdoe;
    uint32_t                   baseAddr;
    MCAN_RevisionId            revId;
    MCAN_InitParams            initParams;
    MCAN_ConfigParams          configParams;
    MCAN_MsgRAMConfigParams    msgRAMConfigParams;
    MCAN_StdMsgIDFilterElement stdFiltelem;
    MCAN_BitTimingParams       bitTimes;
    
    /* Set base address */
    baseAddr = gMcanDiagPortInfo[index].mcanBaseAddr;

    /* Initialize MCAN Init params */
    initParams.fdMode          = 0x1U;
    initParams.brsEnable       = 0x1U;
    initParams.txpEnable       = 0x0U;
    initParams.efbi            = 0x0U;
    initParams.pxhddisable     = 0x0U;
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
    MCAN_getRevisionId(baseAddr, &revId);
    UART_printf("MCANSS Revision ID:\n");
    UART_printf("scheme:0x%x\n",revId.scheme);
    UART_printf("Business Unit:0x%x\n",revId.bu);
    UART_printf("Module ID:0x%x\n",revId.modId);
    UART_printf("RTL Revision:0x%x\n",revId.rtlRev);
    UART_printf("Major Revision:0x%x\n",revId.major);
    UART_printf("Custom Revision:0x%x\n",revId.custom);
    UART_printf("Minor Revision:0x%x\n",revId.minor);

    /* Enable Auto wakeup */
    fdoe = MCAN_isFDOpEnable(baseAddr);
    if (UTRUE == fdoe)
    {
        UART_printf("CAN-FD operation is enabled through E-Fuse.\n");
    }
    else
    {
        UART_printf("CAN-FD operation is disabled through E-Fuse.\n");
    }

    /* wait for memory initialization to happen */
    while (UFALSE == MCAN_isMemInitDone(baseAddr))
    {}

    /* Get endianess value */
    UART_printf("Endianess Value:0x%x\n",MCAN_getEndianVal(baseAddr));

    /* Put MCAN in SW initialization mode */
    MCAN_setOpMode(baseAddr, MCAN_OPERATION_MODE_SW_INIT);
    while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(baseAddr))
    {}

    /* Initialize MCAN module */
    MCAN_init(baseAddr, &initParams);

    /* Configure MCAN module */
    MCAN_config(baseAddr, &configParams);

    /* Configure Bit timings */
    MCAN_setBitTime(baseAddr, &bitTimes);

    /* Set Extended ID Mask */
    MCAN_setExtIDAndMask(baseAddr, APP_MCAN_EXT_ID_AND_MASK);

    /* Configure Message RAM Sections */
    MCAN_msgRAMConfig(baseAddr, &msgRAMConfigParams);

    /* Configure Standard ID filter element */
    MCAN_addStdMsgIDFilter(baseAddr, 0U, &stdFiltelem);

    /* Take MCAN out of the SW initialization mode */
    MCAN_setOpMode(baseAddr, MCAN_OPERATION_MODE_NORMAL);
    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(baseAddr))
    {}

    return 0;
}

#if defined(MCAN_DIAG_INTR_ENABLE)
/**
 * \brief   This API will disable the TX and RX interrupts
 *
 * \param   baseAddr  [IN]  MCAN base address
 *
 */
static void BoardDiag_mcanTxIntDisable(uint32_t baseAddr)
{
    /* Disable Interrupts */
    MCAN_enableIntr(baseAddr, MCAN_INTR_MASK_ALL, UFALSE);
    MCAN_enableIntr(baseAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, UFALSE);
    /* Select Interrupt Line */
    MCAN_selectIntrLine(baseAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_0);
    /* Disable Interrupt Line */
    MCAN_enableIntrLine(baseAddr,
                        MCAN_INTR_LINE_NUM_0,
                        0U);
}

/**
 * \brief   This API will disable the RX interrupts
 *
 * \param   baseAddr  [IN]  MCAN base address
 *
 */
static void BoardDiag_mcanRxIntDisable(uint32_t baseAddr)
{
    /* Disable Interrupts */
    MCAN_enableIntr(baseAddr, MCAN_INTR_MASK_ALL, UFALSE);
    MCAN_enableIntr(baseAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, UFALSE);
    /* Select Interrupt Line */
    MCAN_selectIntrLine(baseAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_1);
    /* Disable Interrupt Line */
    MCAN_enableIntrLine(baseAddr,
                        MCAN_INTR_LINE_NUM_1,
                        0U);
}
/**
 * \brief   This API will enables the RX interrupts
 *
 * \param   baseAddr  [IN]  MCAN base address
 *
 */
static void BoardDiag_mcanRxIntEnable(uint32_t baseAddr)
{
    /* Enable Interrupts */
    MCAN_enableIntr(baseAddr, MCAN_INTR_MASK_ALL, UTRUE);
    MCAN_enableIntr(baseAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, UFALSE);
    /* Select Interrupt Line */
    MCAN_selectIntrLine(baseAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_1);
    /* Enable Interrupt Line */
    MCAN_enableIntrLine(baseAddr,
                        MCAN_INTR_LINE_NUM_1,
                        1U);
}

/**
 * \brief   This API will enables the TX interrupts
 *
 * \param   baseAddr  [IN]  MCAN base address
 *
 */
static int32_t BoardDiag_mcanTxIntEnable(uint32_t baseAddr)
{
    int32_t  status = 0;
    MCAN_enableIntr(baseAddr, MCAN_INTR_MASK_ALL, UTRUE);
    MCAN_enableIntr(baseAddr,
                    MCAN_INTR_SRC_RES_ADDR_ACCESS, UFALSE);
    /* Select Interrupt Line */
    MCAN_selectIntrLine(baseAddr,
                        MCAN_INTR_MASK_ALL,
                        MCAN_INTR_LINE_NUM_0);
    /* Enable Interrupt Line */
    MCAN_enableIntrLine(baseAddr,
                        MCAN_INTR_LINE_NUM_0,
                        1U);
    /* Enable Transmission interrupt */
    status = MCAN_txBufTransIntrEnable(baseAddr,
                                       1U,
                                       UTRUE);
    return status;

}

/**
 * \brief   This is Interrupt Service Routine for MCAN Tx interrupt.
 *
 * \param   handle [IN/OUT] mcasp info structure
 *
 */
static void BoardDiag_mcanTxIntrISR(void *handle)
{
    uint32_t intrStatus;
    uint32_t baseAddrs;
    BoardDiag_McanPortInfo_t *intrInfo;

    intrInfo = (BoardDiag_McanPortInfo_t *)handle;
    baseAddrs = intrInfo -> mcanBaseAddr;

    intrStatus = MCAN_getIntrStatus(baseAddrs);
    MCAN_clearIntrStatus(baseAddrs, intrStatus);
    if (MCAN_INTR_SRC_TRANS_COMPLETE ==
        (intrStatus & MCAN_INTR_SRC_TRANS_COMPLETE))
    {
        gMcanIsrIntr0Flag = UFALSE;
    }

}

/**
 * \brief   This is Interrupt Service Routine for MCAN Rx interrupt.
 *
 * \param   handle [IN/OUT] mcasp info structure
 *
 */
static void BoardDiag_mcanRxIntrISR(void *handle)
{
    uint32_t intrStatus;
    uint32_t baseAddrs;
    BoardDiag_McanPortInfo_t *intrInfo;

    intrInfo = (BoardDiag_McanPortInfo_t *)handle;
    baseAddrs = intrInfo -> mcanBaseAddr;

    intrStatus = MCAN_getIntrStatus(baseAddrs);
    MCAN_clearIntrStatus(baseAddrs, intrStatus);
    if (MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG ==
        (intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG))
    {
        gMcanIsrIntr1Flag = UFALSE;
    }
}

/**
 * \brief   This function will configure MCAN Tx interrupts
 *
 * \param   index  [IN]  - MCAN index number
 *
 */
static void BoardDiag_mcanTxIntrConfig(uint32_t index)
{
#if !defined(SOC_AM64X)
#if defined (__aarch64__)
    Intc_Init(0);

    /* Enable CPU Interrupts and register ISR - MCAN Intr0 */
    Intc_IntRegister(gMcanDiagPortInfo[index].mcanTxIntNum,
                    (IntrFuncPtr) BoardDiag_mcanTxIntrISR, (void*)(&gMcanDiagPortInfo[index]));
    Intc_IntPrioritySet(gMcanDiagPortInfo[index].mcanTxIntNum, 1U, 0U);
    Intc_SystemEnable(gMcanDiagPortInfo[index].mcanTxIntNum);
    UART_printf("Tx Interrupt Configuration done.\n");
#endif
#else
#if !defined (__aarch64__)
    Intc_Init(0);

    /* Enable CPU Interrupts and register ISR - MCAN Intr0 */
    Intc_IntRegister(gMcanDiagPortInfo[index].mcanTxIntNum,
                    (IntrFuncPtr) BoardDiag_mcanTxIntrISR, (void*)(&gmcanDiagportInfo[index]));
    Intc_IntPrioritySet(gMcanDiagPortInfo[index].mcanTxIntNum, 1U, 0U);
    Intc_SystemEnable(gMcanDiagPortInfo[index].mcanTxIntNum);
    UART_printf("Tx Interrupt Configuration done.\n");
#endif
#endif
}

/**
 * \brief   This function will configure MCAN Rx interrupts
 *
 * \param   index  [IN]  - MCAN index number
 *
 */
static void BoardDiag_mcanRxIntrConfig(uint32_t index)
{
#if !defined(SOC_AM64X)
#if defined (__aarch64__)
    Intc_Init(0);

    /* Enable CPU Interrupts and register ISR - MCAN Intr0 */
    Intc_IntRegister(gMcanDiagPortInfo[index].mcanRxIntNum,
                    (IntrFuncPtr) BoardDiag_mcanRxIntrISR, (void*)(&gMcanDiagPortInfo[index]));
    Intc_IntPrioritySet(gMcanDiagPortInfo[index].mcanRxIntNum, 1U, 0U);
    Intc_SystemEnable(gMcanDiagPortInfo[index].mcanRxIntNum);
    UART_printf("Rx Interrupt Configuration done.\n");
#endif
#else
#if !defined (__aarch64__)
    Intc_Init(0);

    /* Enable CPU Interrupts and register ISR - MCAN Intr0 */
    Intc_IntRegister(gMcanDiagPortInfo[index].mcanRxIntNum,
                    (IntrFuncPtr) BoardDiag_mcanRxIntrISR, (void*)(&gmcanDiagportInfo[index]));
    Intc_IntPrioritySet(gMcanDiagPortInfo[index].mcanRxIntNum, 1U, 0U);
    Intc_SystemEnable(gMcanDiagPortInfo[index].mcanRxIntNum);
    UART_printf("Rx Interrupt Configuration done.\n");
#endif
#endif
}

#if defined(MCAN_DIAG_TS_INT_ENABLE)
/**
 * \brief   This is Interrupt Service Routine for MCAN TimeStamp interrupt.
 *
 * \param   handle [IN/OUT] mcasp info structure
 *
 */
static void BoardDiag_mcanTsIntrISR(void *handle)
{
    uint32_t baseAddrs;
    BoardDiag_McanPortInfo_t *intrInfo;

    intrInfo = (BoardDiag_McanPortInfo_t *)handle;
    baseAddrs = intrInfo -> mcanBaseAddr;
    if(MCAN_extTSIsIntrEnable(baseAddrs) == UTRUE)
    {
        UART_printf("MCAN Time Stamp overflow happened.\n");
    }

    MCAN_extTSClearRawStatus(baseAddrs);
    MCAN_extTSWriteEOI(baseAddrs);
}

/**
 * \brief   This function will configure MCAN Rx interrupts
 *
 * \param   index  [IN]  - MCAN index number
 *
 */
static void BoardDiag_mcanTsIntrConfig(uint32_t index)
{
#if !defined(SOC_AM64X)
#if defined (__aarch64__)
    Intc_Init(0);

    /* Enable CPU Interrupts and register ISR - MCAN Intr0 */
    Intc_IntRegister(gMcanDiagPortInfo[index].mcanTsIntNum,
                    (IntrFuncPtr) BoardDiag_mcanTsIntrISR, (void*)(&gMcanDiagPortInfo[index]));
    Intc_IntPrioritySet(gMcanDiagPortInfo[index].mcanTsIntNum, 1U, 0U);
    Intc_SystemEnable(gMcanDiagPortInfo[index].mcanTsIntNum);
    UART_printf("Tx Interrupt Configuration done.\n");
#endif
#else
#if !defined (__aarch64__)
    Intc_Init(0);

    /* Enable CPU Interrupts and register ISR - MCAN Intr0 */
    Intc_IntRegister(gMcanDiagPortInfo[index].mcanTsIntNum,
                    (IntrFuncPtr) BoardDiag_mcanTsIntrISR, (void*)(&gmcanDiagportInfo[index]));
    Intc_IntPrioritySet(gMcanDiagPortInfo[index].mcanTsIntNum, 1U, 0U);
    Intc_SystemEnable(gMcanDiagPortInfo[index].mcanTsIntNum);
    UART_printf("Tx Interrupt Configuration done.\n");
#endif
#endif
}
#endif  /* #if defined(MCAN_DIAG_TS_INT_ENABLE) */
#endif  /* #if defined(MCAN_DIAG_INTR_ENABLE) */

#if defined(BOARD_DIAG_MCAN_DEBUG)
/**
 * \brief   This API will print MCAN Tx Msg.
 *
 */
static void BoardDiag_mcanPrintTxMsg(void)
{
    uint32_t loopCnt;

    UART_printf("\nMessage ID:0x%x\n", txMsg.id);

    UART_printf("\nMessage Remote Transmission Request:0x%x\n",txMsg.rtr);

    UART_printf("\nMessage Extended Frame ID(0:11Bit ID/1:29bit ID):0x%x\n",
                                                              txMsg.xtd);

    UART_printf("\nMessage Error State Indicator(0:Error Active/1:Error Passive):0x%x\n",txMsg.esi);

    UART_printf("\nMessage Data Length Code:0x%x\n",txMsg.dlc);

    UART_printf("\nMessage BRS:0x%x\n",txMsg.brs);

    UART_printf("\nMessage CAN FD format:0x%x\n",txMsg.fdf);

    UART_printf("\nMessage Store Tx Events:0x%x\n",txMsg.efc);

    UART_printf("\nMessage Marker:0x%x\n",txMsg.mm);

    for (loopCnt = 0U; loopCnt < txMsg.dlc; loopCnt++)
    {
        UART_printf("\nMessage DataByte%d:0x%x\n",loopCnt,txMsg.data[loopCnt]);
    }
}

/**
 * \brief   This API will print MCAN Rx Msg.
 *
 */
static void BoardDiag_mcanPrintRxMsg(void)
{
    uint32_t loopCnt;

    UART_printf("\nReceived last message with following details:");

    UART_printf("\nMessage ID:0x%x\n",rxMsg.id);

    UART_printf("\nMessage Remote Transmission Request:0x%x\n",rxMsg.rtr);
    
    UART_printf("\nMessage Extended Frame ID(0:11Bit ID/1:29bit ID):0x%x\n",
                                                                   rxMsg.xtd);
    
    UART_printf("\nMessage Error State Indicator(0:Error Active/1:Error Passive):0x%x\n", rxMsg.esi);
    
    UART_printf("\nMessage TimeStamp:0x%x\n",rxMsg.rxts);

    UART_printf("\nMessage Data Length Code:0x%x\n",rxMsg.dlc);

    UART_printf("\nMessage BRS:0x%x\n",rxMsg.brs);

    UART_printf("\nMessage CAN FD format:0x%x\n",rxMsg.fdf);

    UART_printf("\nMessage Filter Index:0x%x\n",rxMsg.fidx);

    UART_printf("\nMessage Accept Non-matching Frame:0x%x\n",rxMsg.anmf);

    for (loopCnt = 0U; loopCnt < rxMsg.dlc; loopCnt++)
    {
        UART_printf("\nMessage DataByte%d:0x%x\n",loopCnt,rxMsg.data[loopCnt]);
    }
}
#endif  /* #if defined(BOARD_DIAG_MCAN_DEBUG) */

/**
 * \brief   This API will load the data to the message ram and checking
 *          the error status
 *
 * \param   instance  [IN]  - MCAN instance number
 *
 * \return  int8_t
 *             0   - in case of success
 *            -1   - in case of failure
 *
 */
static int8_t BoardDiag_mcanTxTest(uint8_t instance)
{
    int8_t   configStatus = 0;
    uint32_t baseAddr;
    MCAN_ProtocolStatus protStatus;
    
    /* Set base address */
    baseAddr = gMcanDiagPortInfo[instance].mcanBaseAddr;
  
#if defined(MCAN_DIAG_INTR_ENABLE)
    BoardDiag_mcanTxIntrConfig(instance);
    configStatus = BoardDiag_mcanTxIntEnable(baseAddr);
#endif

    /* Write message to Msg RAM */
    MCAN_writeMsgRam(baseAddr,
                     MCAN_MEM_TYPE_BUF,
                     APP_MCAN_TX_BUF_NUM,
                     &txMsg);

    /* Add request for transmission */
    configStatus += MCAN_txBufAddReq(baseAddr, APP_MCAN_TX_BUF_NUM);
    if (configStatus != 0)
    {
        UART_printf("\nError in Adding Transmission Request...\n");
        return -1;
    }


#if defined(MCAN_DIAG_INTR_ENABLE)
    while (gMcanIsrIntr0Flag)
    {}
    gMcanIsrIntr0Flag = UTRUE;
#else   
    /* Waiting for Transmission Complete */
    while(((MCAN_getTxBufReqPend(baseAddr) >>
            APP_MCAN_TX_BUF_NUM) & 0x1) == 0x1);
#endif

    MCAN_getProtocolStatus(baseAddr, &protStatus);
    /* Checking for Errors */
    if (((MCAN_ERR_CODE_NO_ERROR == protStatus.lastErrCode) ||
         (MCAN_ERR_CODE_NO_CHANGE == protStatus.lastErrCode)) &&
        ((MCAN_ERR_CODE_NO_ERROR == protStatus.dlec) ||
         (MCAN_ERR_CODE_NO_CHANGE == protStatus.dlec)) &&
        (0U == protStatus.pxe))
    {
        UART_printf("\nMessage successfully transferred with payload Bytes:0x%x\n",txMsg.dlc);
    }
    else
    {
        UART_printf("\nError in transmission with payload Bytes:0x%x\n", txMsg.dlc);
        configStatus = -1;
    }

    return configStatus;
}

/**
 * \brief   This API will receive the data and compares with the transmit data
 *
 * \param   instance  [IN]  - MCAN instance number
 *
 * \return  int8_t
 *             0   - in case of success
 *            -1   - in case of failure
 *
 */
static int8_t BoardDiag_mcanRxTest(uint8_t index)
{
    uint32_t chkCnt     = 0U;
    uint32_t errFlag    = 0U;
    uint32_t baseAddr;
    int8_t  testStatus  = 0;
    MCAN_RxNewDataStatus newDataStatus;
    MCAN_ErrCntStatus    errCounter;

    /* Set base address */
    baseAddr = gMcanDiagPortInfo[index].mcanBaseAddr;

#if defined(MCAN_DIAG_INTR_ENABLE)
    BoardDiag_mcanRxIntrConfig(index);
    BoardDiag_mcanRxIntEnable(baseAddr);
#endif

#if defined(MCAN_DIAG_INTR_ENABLE)
    while (gMcanIsrIntr1Flag)
    {}
    gMcanIsrIntr1Flag = UTRUE;
#else
    /* Waiting for Transmission Complete */
    while(MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG !=
        (MCAN_getIntrStatus(baseAddr) &
        MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG));
    
#endif

    /* Checking for Errors */
    MCAN_getErrCounters(baseAddr, &errCounter);
    if ((0U == errCounter.recErrCnt) &&
        (0U == errCounter.canErrLogCnt))
    {
        MCAN_getNewDataStatus(baseAddr, &newDataStatus);
        MCAN_clearNewDataStatus(baseAddr, &newDataStatus);
        MCAN_readMsgRam(baseAddr,
                        MCAN_MEM_TYPE_BUF,
                        0U,
                        0U,
                        &rxMsg);

        errFlag = 0U;

        for (chkCnt = 0U; chkCnt < rxMsg.dlc; chkCnt++)
        {
            if (txMsg.data[chkCnt] != rxMsg.data[chkCnt])
            {
                errFlag = 1U;
                break;
            }
        }

        if (0U == errFlag)
        {
            UART_printf("\nMessage successfully received with payload Bytes:0x%x\n",rxMsg.dlc);
        }
        else
        {
            UART_printf("\nWrong data received in message with payload Bytes:0x%x\n",rxMsg.dlc);
            testStatus = -1;
        }
    }
    else
    {
        UART_printf("\nError in reception with payload Bytes:0x%x\n", txMsg.dlc);
        testStatus = -1;
    }

#if defined(BOARD_DIAG_MCAN_DEBUG)
    BoardDiag_mcanPrintRxMsg();
#endif

    return testStatus;
}

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)
/**
 * \brief   This function enables the Main CAN module and transceiver by setting
 *          the Enable and STB Pins
 *
 */
static void BoardDiag_mcanMainconfigs(void)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status = BOARD_SOK;

#if !defined (j784s4_evm)
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
    ioExpCfg.enableIntr  = BFALSE;    
    ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_6;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
    
    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to enable the main MCAN0 \n");
    }
#endif
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
    ioExpCfg.enableIntr  = BFALSE;    
    ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_7;
#if defined(j784s4_evm)
    /* MCAN3_STB, MCAN5_STB, MCAN4_STB pins enabling*/
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

#else
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;
#endif

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to set the mcan0 stb pin to normal mode\n");
    }

#if defined(j721e_evm)
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE2_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE2_ADDR;
    ioExpCfg.enableIntr  = BFALSE;    
    ioExpCfg.ioExpType   = THREE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_4;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;
    
    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to enable the MCAN mux selection\n");
    }
#endif
}
#endif  /* #if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)*/

#if !defined(SOC_AM64X) && !defined(SOC_TPR12) && !defined(SOC_AWR294X)

/**
 * \brief   This API Initializes the GPIO module
 *
 * \param    gpioBaseAddrs [IN]  GPIO base address to configure
 * \param    port          [IN]  GPIO Port number
 *
 */
static void BoardDiag_mcanGpioConfig(uint32_t gpioBaseAddrs,uint32_t port)
{
    GPIO_v0_HwAttrs gpioCfg;
    GPIO_socGetInitCfg(port, &gpioCfg);
    gpioCfg.baseAddr = gpioBaseAddrs;
    GPIO_socSetInitCfg(port, &gpioCfg);
    /* GPIO initialization */
    GPIO_init();
}
#endif

#if defined(j7200_evm)
/**
 * \brief  UART Mux enable function
 *
 * This function is used to enable the uart mux selection
 *
 * \param  pinNum    -  I2C IO EXPANDER pin number
 * \param  pinVal    -  I2C IO EXPANDER pin value
 *
 */
void BoardDiag_McanMuxEnable(i2cIoExpPinNumber_t pinNum,
                             i2cIoExpSignalLevel_t pinVal)
{
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status = BOARD_SOK;

    /* Enable the UART1 and UART3 */
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = pinNum;
    ioExpCfg.signalLevel = pinVal;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to select the MCAN Mux\n");
    }
}
#endif

#if !defined(SOC_TPR12) && !defined(SOC_AWR294X)
/**
 * \brief   This API enables the CAN transceivers by setting the STB pins
 *
 */
static void BoardDiag_mcanEnable(void)
{

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4)
    Board_IoExpCfg_t ioExpCfg;
    Board_STATUS status = BOARD_SOK;
#endif

#if defined (am65xx_idk)
    BoardDiag_mcanGpioConfig(CSL_GPIO1_BASE, 1);
    GPIO_write(0, 1);
    GPIO_write(1, 1);
#elif defined(SOC_AM64X)
    Board_I2cInitCfg_t i2cCfg;

    i2cCfg.i2cInst   = BOARD_I2C_IOEXP_DEVICE1_INSTANCE;
    i2cCfg.socDomain = BOARD_SOC_DOMAIN_MAIN;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    Board_i2cIoExpInit();
    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                                  THREE_PORT_IOEXP,
                                  PORTNUM_1,
                                  PIN_NUM_0,
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the MCAN0_STB_EN pin to low for normal mode */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                              THREE_PORT_IOEXP,
                              PORTNUM_1,
                              PIN_NUM_0,
                              GPIO_SIGNAL_LEVEL_LOW);

    /* Setting the pin direction as output */
    Board_i2cIoExpSetPinDirection(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                                  THREE_PORT_IOEXP,
                                  PORTNUM_1,
                                  PIN_NUM_1,
                                  PIN_DIRECTION_OUTPUT);

    /* Pulling the MCAN1_STB_EN pin to low for normal mode */
    Board_i2cIoExpPinLevelSet(BOARD_I2C_IOEXP_DEVICE1_ADDR,
                              THREE_PORT_IOEXP,
                              PORTNUM_1,
                              PIN_NUM_1,
                              GPIO_SIGNAL_LEVEL_LOW);

    BOARD_delay(1000);
    Board_i2cIoExpDeInit();
#else
    BoardDiag_mcanGpioConfig(CSL_WKUP_GPIO0_BASE,0);
    /* Enable MCU CAN transceivers by setting the STB pins */
#if defined(j784s4_evm)
    GPIO_write(0, 0); /* MCU_CAN0_STB */
    GPIO_write(1, 0); /* MCU_CAN1_STB */
#else
    GPIO_write(0, 1); /* MCU_CAN0_STB */
    GPIO_write(1, 0); /* MCU_CAN1_STB */
    /* MCU_MCAN0_EN */
    GPIO_write(2, 1); /* WKUP_GPIO0_0 */
#endif

    /* Enable CP board MAIN CAN transceivers by setting the STB pins */
    BoardDiag_mcanMainconfigs();

#if defined(j721e_evm)
    /* MAIN CAN2 STB */
    BoardDiag_mcanGpioConfig(CSL_GPIO0_BASE, 0);
    GPIO_write(3, 0); /* GPIO0_127 */
#endif

#if defined(j721s2_evm)

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_SOM_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_SOM_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = ONE_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_0;
    ioExpCfg.pinNum      = PIN_NUM_7;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to set the CP board mcan stb pin to normal mode \n");
    }

    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = TWO_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_4;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);

    Board_control(BOARD_CTRL_CMD_SET_SOM_MUX_PORTB2, NULL);

#elif defined (j784s4_evm)
    /* Mapping MCAN3 and MCAN5 tx and rx pins */
    ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE1_INSTANCE;
    ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
    ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE1_ADDR;
    ioExpCfg.enableIntr  = BFALSE;
    ioExpCfg.ioExpType   = TWO_PORT_IOEXP;
    ioExpCfg.portNum     = PORTNUM_1;
    ioExpCfg.pinNum      = PIN_NUM_4;
    ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_HIGH;

    status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to set the mcan TX and RX pin \n");
    }

    /* Mapping MCAN4 tx and rx pins */
    status = Board_control (BOARD_CTRL_CMD_SET_IO_MUX_PORTB2, NULL);
    if(status != BOARD_SOK)
    {
        UART_printf("Failed to set the mux from A -> B2 \n");
    }
    /* MCU0, MCU1, MCAN16 tx and rx are directly mapped to SOC */
#endif
    if(expBoardDetect)
    {
        UART_printf("GESI board Detected\n");
        ioExpCfg.i2cInst     = BOARD_I2C_IOEXP_DEVICE1_INSTANCE;
        ioExpCfg.socDomain   = BOARD_SOC_DOMAIN_MAIN;
        ioExpCfg.slaveAddr   = BOARD_I2C_IOEXP_DEVICE1_ADDR;
        ioExpCfg.enableIntr  = BFALSE;    
        ioExpCfg.ioExpType   = TWO_PORT_IOEXP;
        ioExpCfg.portNum     = PORTNUM_1;
        ioExpCfg.pinNum      = PIN_NUM_4;
        ioExpCfg.signalLevel = GPIO_SIGNAL_LEVEL_LOW;
        
        status = Board_control(BOARD_CTRL_CMD_SET_IO_EXP_PIN_OUT, &ioExpCfg);
        if(status != BOARD_SOK)
        {
            UART_printf("Failed to set the GESI board mcan stb pin to normal mode \n");
        }

#if defined(j7200_evm)
        /* Enable GESI CAN STB pin to normal mode  */
        BoardDiag_McanMuxEnable(PIN_NUM_7, GPIO_SIGNAL_LEVEL_LOW);
        Board_control(BOARD_CTRL_CMD_SET_SOM_CAN_MUX, NULL);
#endif

#if defined(j721e_evm)
        /* GPIO0_60 */
        BoardDiag_mcanGpioConfig(CSL_GPIO0_BASE, 0);
        GPIO_write(4, 0);
#endif
    }
#endif
}
#endif /* #if !defined(SOC_TPR12) && !defined(SOC_AWR294X) */

/**
 * \brief  This function executes MCAN Diagnostic test
 *
 * \return  int8_t
 *           0  - in case of success
 *          -1  - in case of failure
 *
 */
int32_t BoardDiag_mcanTest(void)
{
    int8_t    ret     = 0;
    uint32_t  index;
    uint32_t  portNum;

#if defined(DIAG_STRESS_TEST) && (defined(am65xx_idk) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4))
    char rdBuf = 'y';
#endif

#if defined(DIAG_STRESS_TEST) && ((defined(am65xx_idk)) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4))
    UART_printf  ("***********************************************\n");
    UART_printf  ("*                MCAN Stress Test             *\n");
    UART_printf  ("***********************************************\n");
#else
    UART_printf  ("***********************************************\n");
    UART_printf  ("*                MCAN Test                    *\n");
    UART_printf  ("***********************************************\n");
#endif

#if !defined(SOC_TPR12) && !defined(SOC_AWR294X)
    BoardDiag_mcanEnable();
#endif
#if defined(am65xx_idk) || defined(SOC_AM64X) || defined(SOC_TPR12) || defined(SOC_AWR294X)
    mcanMaxPorts = MCAN_MAX_PORTS;
#else
    if(expBoardDetect)
    {
        mcanMaxPorts = MCAN_MAX_PORTS_EXP;
    }
    else
    {
        mcanMaxPorts = MCAN_MAX_PORTS_CP;
    }
#endif

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

    for(portNum = 0; portNum < mcanMaxPorts; portNum += 2)
    {
        /* Configure MCAN */
        ret = BoardDiag_mcanConfig(portNum);
        if(ret == 0)
        {
            UART_printf("Successfully configured MCAN%d\n",portNum);
        }

        ret = BoardDiag_mcanConfig(portNum+1);
        if(ret == 0)
        {
            UART_printf("Successfully configured MCAN%d\n",portNum+1);
        }

        UART_printf("\n\nTransmitting Data on MCAN Port%d and Receiving on MCAN port%d\n",portNum, portNum+1);

        for(index = 0; index < PKT_SEND_COUNT; index++)
        {
            UART_printf("\nSending Packet - %d\n", (index + 1));
            /* Fill the Tx buffer with random data */
            BoardDiag_genPattern(txMsg.data, MCAN_MAX_PAYLOAD_BYTES,
                                 BOARD_DIAG_TEST_PATTERN_RANDOM);

            /* Transmiting port */
            ret = BoardDiag_mcanTxTest(portNum);
            if(ret != 0)
            {
                UART_printf("Failed to transmit data on port%d\n",portNum);
                return (-1);
            }
#if defined(BOARD_DEBUG_MCAN)
            BoardDiag_mcanPrintTxMsg();
#endif
            /* Receiving port*/
            ret = BoardDiag_mcanRxTest(portNum+1);
            if(ret != 0)
            {
                UART_printf("Failed to receive data on port%d",portNum);
                return (-1);
            }

            UART_printf("\nReceived Packet - %d\n\n", (index + 1));
#if defined(DIAG_STRESS_TEST) && ((defined(am65xx_idk)) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4))
            /* Check if there a input from console to break the test */
            rdBuf = (char)BoardDiag_getUserInput(BOARD_UART_INSTANCE);
            if((rdBuf == 'b') || (rdBuf == 'B'))
            {
                UART_printf("Received Test Termination... Exiting the Test\n\n");
                UART_printf("MCAN Stress Test Status\n");
                UART_printf("============================================\n");
                UART_printf("Total Number of Packets sent: %d\nTotal Number of Packets Received: %d\n",index + 1,index + 1);
                break;
            }
#endif
#if defined(MCAN_DIAG_INTR_ENABLE)
            /* Disable the TX and RX interrupts */
            BoardDiag_mcanTxIntDisable(gMcanDiagPortInfo[portNum].mcanBaseAddr);
            BoardDiag_mcanRxIntDisable(gMcanDiagPortInfo[portNum+1].mcanBaseAddr);
#endif
        }

        UART_printf("\n\nTransmitting Data on MCAN Port%d and Receiving on MCAN port%d\n",portNum+1, portNum);
        for(index = 0; index < PKT_SEND_COUNT; index++)
        {
            UART_printf("\nSending Packet - %d\n", (index + 1));
            /* Fill the Tx buffer with random data */
            BoardDiag_genPattern(txMsg.data, MCAN_MAX_PAYLOAD_BYTES,
                                 BOARD_DIAG_TEST_PATTERN_RANDOM);

            /* Transmitting port*/
            ret = BoardDiag_mcanTxTest(portNum+1);
            if(ret != 0)
            {
                UART_printf("Failed to transmit data on port%d",portNum+1);
                return -1;
            }
#if defined(BOARD_DEBUG_MCAN)
            BoardDiag_mcanPrintTxMsg();
            UART_printf("Receiving data on port%d\n", portNum);
#endif

            /* Receiving port */
            ret = BoardDiag_mcanRxTest(portNum);
            if(ret != 0)
            {
                UART_printf("Failed to receive data on port%d\n",portNum);
                return (-1);
            }

            UART_printf("\nReceived Packet - %d\n\n", (index + 1));
#if defined(DIAG_STRESS_TEST) && ((defined(am65xx_idk)) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4))
            /* Check if there a input from console to break the test */
            rdBuf = (char)BoardDiag_getUserInput(BOARD_UART_INSTANCE);
            if((rdBuf == 'b') || (rdBuf == 'B'))
            {
                UART_printf("Received Test Termination... Exiting the Test\n\n");
                UART_printf("MCAN Stress Test Status\n");
                UART_printf("============================================\n");
                UART_printf("Total Number of Packets sent: %d\nTotal Number of Packets Received: %d\n",index + 1,index + 1);
                break;
            }
#endif
#if defined(MCAN_DIAG_INTR_ENABLE)
            /* Disable the TX and RX interrupts */
            BoardDiag_mcanTxIntDisable(gMcanDiagPortInfo[portNum+1].mcanBaseAddr);
            BoardDiag_mcanRxIntDisable(gMcanDiagPortInfo[portNum].mcanBaseAddr);
#endif
        }
    }
    UART_printf("\n MCAN diagnostic test completed.\n");

    return  0;
}


/**
 * \brief  main function
 *
 *  This function performs board initializations and calls MCAN test
 *
 * \return  int
 *           0  - in case of success
 *          -1  - in case of failure
 *
 */
int main(void)
{
    int32_t retVal;
    Board_STATUS status;
    Board_initCfg boardCfg;

#if defined(j721e_evm) || defined(j7200_evm)
    if(BTRUE == Board_detectBoard(BOARD_ID_GESI))
    {
        expBoardDetect = UTRUE;
    }
#endif

#if defined(j721e_evm)
    Board_PinmuxConfig_t gesiIcssgPinmux;
    if(expBoardDetect)
    {
        Board_pinmuxGetCfg(&gesiIcssgPinmux);
        gesiIcssgPinmux.autoCfg = BOARD_PINMUX_CUSTOM;
        gesiIcssgPinmux.gesiExp = BOARD_PINMUX_GESI_ICSSG;
        Board_pinmuxSetCfg(&gesiIcssgPinmux);
    }
#endif

#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_MODULE_CLOCK |
               BOARD_INIT_PINMUX_CONFIG |
               BOARD_INIT_UART_STDIO;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG ;
#endif

    status = Board_init(boardCfg);
    if(status != BOARD_SOK)
    {
        return -1;
    }

#if defined(SOC_AWR294X)
    /* Configure CAN Enable pin */
    GPIO_v2_updateConfig(&GPIO_v2_config);
    GPIO_init();
    GPIO_write(GPIO_v2_config.pinConfigs[0].pinIndex, 1);

    /* Configure CAN STB pin */
    retVal = BoardDiag_mcanConfigSTB(MIBSPI_INST_ID_MSS_SPIB);
    if(retVal != 0)
    {
        UART_printf("CAN STB Pin Configurations Failed!!\n");
        return retVal;
    }
    else
    {
        UART_printf("CAN STB Pin Configurations Successful!\n");
    }
#endif

    retVal = BoardDiag_mcanTest();
    if(retVal == 0)
    {
        UART_printf("\n MCAN Diagnostic Test Passed!\n");
    }
    else
    {
        UART_printf("\n MCAN Diagnostic Test Failed!!\n");
    }

    return retVal;
}
