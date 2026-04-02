/**
 * @file   main_test.c
 *
 * @brief  This file tests the UART driver APIs in Blocking mode.
 */
/*
 * Copyright (c) 2014 - 2018, Texas Instruments Incorporated
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

#include <stdio.h>
#include <string.h>

/* CSL Header files */
#include <ti/csl/soc.h>
#include <ti/csl/csl_types.h>

/* OSAL Header files */
#include <ti/osal/osal.h>

#include <ti/osal/TaskP.h>

/* UART Header files */
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/uart/src/UART_osal.h>
#include <ti/drv/uart/soc/UART_soc.h>

#include "UART_board.h"

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/csl/soc.h>
#endif

#if defined(SOC_J721E) || defined(SOC_AM65XX) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/drv/sciclient/sciclient.h>
#endif

#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#include <ti/csl/arch/csl_arch.h>
#endif

#if defined (BUILD_C7X)
#include <ti/csl/arch/csl_arch.h>
#endif

//#define CIO_DRV_CONSOLE

#ifdef CIO_DRV_CONSOLE
#define UART_test_log            printf
#define UART_test_log1           printf
#define UART_test_log2           printf
#define UART_test_log3           printf
#else
#define UART_test_log(x)
#define UART_test_log1(x,y)
#define UART_test_log2(x,y,z)
#define UART_test_log3(x,y,z,l)
#endif


#if defined(SOC_TPR12) || defined (SOC_AWR294X)
#if defined(SIM_BUILD)
#define UART_RX_LOOPBACK_ONLY
#endif
#define UART_API2_NOT_SUPPORTED
#define UART_NO_FIFO
#endif


/* Define the UART test interface */
typedef struct UART_Tests_s
{
    bool     (*testFunc)(bool);
    bool     dmaMode;
    int16_t  testId;
    char     testDesc[80];

} UART_Tests;

/* UART test ID definitions */
#define UART_TEST_ID_DMA         0   /* UART DMA read write test in block mode */
#define UART_TEST_ID_INT         1   /* UART non-DMA read write test in block mode */
#define UART_TEST_ID_DMA_CB      2   /* UART DMA read write test in callback mode */
#define UART_TEST_ID_CB          3   /* UART non-DMA read write test in callback mode */
#define UART_TEST_ID_DMA_TO      4   /* UART DMA timeout test */
#define UART_TEST_ID_TO          5   /* UART non DMA timeout test */
#define UART_TEST_ID_DMA_RXERR   6   /* UART DMA RX error test */
#define UART_TEST_ID_RXERR       7   /* UART non-DMA RX error test */
#define UART_TEST_ID_DMA_CANCEL  8   /* UART DMA read write cancel test */
#define UART_TEST_ID_CANCEL      9   /* UART non-DMA read write cancel test */
#define UART_TEST_ID_DMA_RW      10  /* UART DMA simultaneous read write test */
#define UART_TEST_ID_RW          11  /* UART non-DMA simultaneous read write test */
#define UART_TEST_ID_DMA_TRGLVL  12  /* UART DMA TX/RX FIFO trigger level test */
#define UART_TEST_ID_PRINTF      13  /* UART stdio printf and scanf test */
#define UART_TEST_ID_TRGLVL      14  /* UART non-DMA TX/RX FIFO trigger level test */
#define UART_TEST_ID_POLL_TO     15  /* UART read write polling timeout test */
#define UART_TEST_ID_STDIOPARAMS 16  /* UART stdio printf and scanf test, with configurable params(Default params) */
#define UART_TEST_ID_INT_DISABLE 17  /* UART read write test with interrupt disabled */
#define UART_TEST_ID_RDVERIFY    18  /* UART non-DMA read API Test in loopback mode */
#define UART_TEST_ID_MULTI_INSTS 19  /* UART DMA multiple instances test in loopback mode */
#define UART_TEST_ID_API         20  /* UART API Test */
#define UART_TEST_ID_PROF_TX     21  /* UART non-DMA/DMA Blocking/Polling transmit polling */
#define UART_TEST_ID_DMA_LB_DATA 22  /* UART DMA data transfer test in loopback mode */
#define UART_TEST_ID_LB_DATA     23  /* UART non-DMA data transfer test in loopback mode */

/* Length of the input in number of characters */
#define UART_TEST_READ_LEN     (16U)
#define UART_RDVERIFY_READ_LEN (4U)

/* Timeout value of read and write */
#ifdef SIM_BUILD
#define UART_TEST_TIMEOUT      (50U)
#else
#define UART_TEST_TIMEOUT      (10000U)
#endif

/* Max number of instances to test in multiple instance test case */
#define UART_TEST_NUM_INSTS    (2U)
/* MAX Data Pattern Test Size for the Data Tests: */
#define MAX_TEST_BUFFER_SIZE   256

#if defined(CSL_CACHE_L2_LINESIZE)
#define UART_TEST_CACHE_LINE_SIZE CSL_CACHE_L2_LINESIZE
#elif defined(CSL_CACHE_L1D_LINESIZE)
#define UART_TEST_CACHE_LINE_SIZE CSL_CACHE_L1D_LINESIZE
#else
#define UART_TEST_CACHE_LINE_SIZE (128U)
#endif

volatile char scanPrompt[MAX_TEST_BUFFER_SIZE] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE)));
char echoPrompt[40] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) = "\n\r Data entered is as follows \r\n";
char dataPrint[40] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) = "\r\n enter the data of 16 character \r\n";
char readTimeoutPrompt[60] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) = "\r\n Read timed out \r\n";
char breakErrPrompt[60] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) = "\r\n Received a break condition error \r\n";
char rdCancelPrompt[60] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) = "\r\n Previous read canceled \r\n";
char wrCancelPrompt[60] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) = "\r\n Previous write canceled \r\n";
volatile char fifoTrgLvlData[MAX_TEST_BUFFER_SIZE] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) =
                        "0123456789112345678921234567893123456789412345678951234567896123456789712345678981234567899123456789"
                        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789abcedef12345"
                        "6789ABCDEF012345678901234567899876543210deadbeef89512345";
char stdioPrint[64] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE))) = "\r\n enter the data of 16 character and press ENTER \r\n";

UART_Transaction  callbackTransaction;
SemaphoreP_Handle callbackSem = NULL;

uint32_t uartTestInstance;

uint32_t verifyLoopback = UFALSE;

uint32_t verifyRS485 = UFALSE;

UART_PAR uartParity = UART_PAR_NONE;

/* Global Variable which holds the UART Handle */
UART_Handle     gUARTHandle = NULL;

#if defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#ifdef UART_DMA_ENABLE
/*
 * Ring parameters
 */
/** \brief Number of ring entries - we can prime this much memcpy operations */
#define UDMA_TEST_APP_RING_ENTRIES      (1U)
/** \brief Size (in bytes) of each ring entry (Size of pointer - 64-bit) */
#define UDMA_TEST_APP_RING_ENTRY_SIZE   (sizeof(uint64_t))
/** \brief Total ring memory */
#define UDMA_TEST_APP_RING_MEM_SIZE     (UDMA_TEST_APP_RING_ENTRIES * \
                                         UDMA_TEST_APP_RING_ENTRY_SIZE)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_RING_MEM_SIZE_ALIGN ((UDMA_TEST_APP_RING_MEM_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/**
 *  \brief UDMA host mode buffer descriptor memory size.
 *  Make it multiple of UART_TEST_CACHE_LINE_SIZE alignment
 */
/** \brief UDMA host mode buffer descriptor memory size. */
#define UDMA_TEST_APP_DESC_SIZE         (sizeof(CSL_UdmapCppi5HMPD))
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_DESC_SIZE_ALIGN   ((UDMA_TEST_APP_DESC_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/*
 * UDMA driver objects
 */
struct Udma_DrvObj      gUdmaDrvObj;
struct Udma_ChObj       gUdmaTxChObj;
struct Udma_ChObj       gUdmaRxChObj;
struct Udma_EventObj    gUdmaTxCqEventObj;
struct Udma_EventObj    gUdmaRxCqEventObj;

Udma_DrvHandle          gDrvHandle = NULL;
/*
 * UDMA Memories
 */
static uint8_t gTxRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gTxCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gTdTxCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmaTxHpdMem[UDMA_TEST_APP_DESC_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gRxRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gRxCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gTdRxCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static uint8_t gUdmaRxHpdMem[UDMA_TEST_APP_DESC_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));
static UART_dmaInfo gUdmaInfo;

Udma_DrvHandle UartApp_udmaInit(UART_HwAttrs *cfg)
{
    int32_t         retVal = UDMA_SOK;
    Udma_InitPrms   initPrms;
    uint32_t        instId;

    if (NULL == gDrvHandle)
    {
        /* UDMA driver init */
#if defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM65XX) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
        instId = UDMA_INST_ID_MCU_0;
#else
        instId = UDMA_INST_ID_MAIN_0;
#endif
#endif

#if defined(SOC_AM64X)
        instId = UDMA_INST_ID_PKTDMA_0;
#endif

        UdmaInitPrms_init(instId, &initPrms);

        retVal = Udma_init(&gUdmaDrvObj, &initPrms);
        if(UDMA_SOK == retVal)
        {
            gDrvHandle = &gUdmaDrvObj;
        }
    }

    if(NULL != gDrvHandle)
    {
        gDrvHandle = &gUdmaDrvObj;

        gUdmaInfo.txChHandle     = (void *)&gUdmaTxChObj;
        gUdmaInfo.rxChHandle     = (void *)&gUdmaRxChObj;
        gUdmaInfo.txRingMem      = (void *)&gTxRingMem[0];
        gUdmaInfo.cqTxRingMem    = (void *)&gTxCompRingMem[0];
        gUdmaInfo.tdCqTxRingMem  = (void *)&gTdTxCompRingMem[0];
        gUdmaInfo.rxRingMem      = (void *)&gRxRingMem[0];
        gUdmaInfo.cqRxRingMem    = (void *)&gRxCompRingMem[0];
        gUdmaInfo.tdCqRxRingMem  = (void *)&gTdRxCompRingMem[0];
        gUdmaInfo.txHpdMem       = (void *)&gUdmaTxHpdMem[0];
        gUdmaInfo.rxHpdMem       = (void *)&gUdmaRxHpdMem[0];
        gUdmaInfo.txEventHandle  = (void *)&gUdmaTxCqEventObj;
        gUdmaInfo.rxEventHandle  = (void *)&gUdmaRxCqEventObj;
        cfg->dmaInfo             = &gUdmaInfo;
    }

    return (gDrvHandle);
}

int32_t UART_udma_deinit(void)
{
    int32_t         retVal = UDMA_SOK;

    if (NULL != gDrvHandle)
    {
        retVal = Udma_deinit(gDrvHandle);
        if(UDMA_SOK == retVal)
        {
            gDrvHandle = NULL;
        }
    }

    return (retVal);
}
#endif
#endif /* #if defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2) */

#if defined(SOC_AM64X) && defined(BUILD_MCU)
int32_t UART_configClk(uint32_t freq)
{
    int32_t retVal = CSL_PASS;
    uint64_t uartClk;
    uint32_t parentID;
    uint32_t clkID[] = {
                           TISCI_DEV_UART0_FCLK_CLK,
                           TISCI_DEV_UART1_FCLK_CLK,
                           TISCI_DEV_UART2_FCLK_CLK,
                           TISCI_DEV_UART3_FCLK_CLK,
                           TISCI_DEV_UART4_FCLK_CLK,
                           TISCI_DEV_UART5_FCLK_CLK,
                           TISCI_DEV_UART6_FCLK_CLK,
                       };
    uint32_t modID[] = {
                           TISCI_DEV_UART0,
                           TISCI_DEV_UART1,
                           TISCI_DEV_UART2,
                           TISCI_DEV_UART3,
                           TISCI_DEV_UART4,
                           TISCI_DEV_UART5,
                           TISCI_DEV_UART6,
                       };

    retVal = Sciclient_pmModuleClkRequest(modID[uartTestInstance],
                                          clkID[uartTestInstance],
                                          TISCI_MSG_VALUE_CLOCK_SW_STATE_REQ,
                                          TISCI_MSG_FLAG_AOP,SCICLIENT_SERVICE_WAIT_FOREVER);
    if (CSL_PASS == retVal)
    {
        if (UART_MODULE_CLK_48M == freq)
        {
            parentID = TISCI_DEV_UART0_FCLK_CLK_PARENT_USART_PROGRAMMABLE_CLOCK_DIVIDER_OUT0;
        }
        else
        {
            parentID = TISCI_DEV_UART0_FCLK_CLK_PARENT_HSDIV4_16FFT_MAIN_1_HSDIVOUT1_CLK;
        }
        retVal = Sciclient_pmSetModuleClkParent(modID[uartTestInstance],
                                                clkID[uartTestInstance],
                                                parentID,
                                                SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if (CSL_PASS == retVal)
    {
        uartClk = (uint64_t)freq;
        retVal = Sciclient_pmSetModuleClkFreq(modID[uartTestInstance],
                                              clkID[uartTestInstance],
                                              uartClk,
                                              TISCI_MSG_FLAG_AOP,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if (CSL_PASS == retVal)
    {
        uartClk = 0;
        retVal = Sciclient_pmGetModuleClkFreq(modID[BOARD_OSPI_NOR_INSTANCE],
                                              clkID[BOARD_OSPI_NOR_INSTANCE],
                                              &uartClk,
                                              SCICLIENT_SERVICE_WAIT_FOREVER);
    }

    if (CSL_PASS == retVal)
    {
        if ((uint32_t)uartClk != freq)
        {
            retVal = CSL_EFAIL;
        }
    }

    return (retVal);
}
#endif
/*
 *  ======== UART init config ========
 */
static void UART_initConfig(bool dmaMode)
{
    UART_HwAttrs uart_cfg;

    /* Get the default UART init configurations */
    UART_socGetInitCfg(uartTestInstance, &uart_cfg);

#ifdef UART_DMA_ENABLE
    if (BTRUE == dmaMode)
    {
#if defined (SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
        uart_cfg.edmaHandle = UartApp_udmaInit(&uart_cfg);
#else
        uart_cfg.edmaHandle = UartApp_edmaInit();
    #if defined(SOC_TPR12) || defined (SOC_AWR294X)
        uart_cfg.paramSetId = EDMA_NUM_DMA_CHANNELS;
    #endif
#endif
        uart_cfg.dmaMode    = UTRUE;
    }
    else
#endif
    {
        uart_cfg.edmaHandle = NULL;
        uart_cfg.dmaMode    = UFALSE;
    }

    uart_cfg.loopback   = verifyLoopback;
#if defined(SOC_AM64X) && defined(BUILD_MCU)
    if (UTRUE == verifyRS485)
    {
        uart_cfg.frequency = UART_MODULE_CLK_160M;
    }
    else
    {
        uart_cfg.frequency = UART_MODULE_CLK_48M;
    }
    uart_cfg.dirEnable = verifyRS485;
    UART_configClk(uart_cfg.frequency);
#endif

    /* Set the DMA enabled UART init configurations */
    UART_socSetInitCfg(uartTestInstance, &uart_cfg);
}

void UART_getTestInstNum(uint32_t *instNum, bool *boardAM570x)
{
#if defined (idkAM571x)
    Board_STATUS  boardStatus;
    Board_IDInfo  id;
#endif

    *instNum = UART_INSTANCE;
    *boardAM570x = BFALSE;

#if defined (idkAM571x)
    boardStatus = Board_getIDInfo(&id);
    if (BOARD_SOK != boardStatus)
    {
        return;
    }

    /* Check if is DRA (AM570x) SoC */
    if (('D' == id.boardName[0]) &&
        ('R' == id.boardName[1]) &&
        ('A' == id.boardName[2]))
    {
        *boardAM570x = BTRUE;
        *instNum = 0;
    }
#endif
}

bool Board_initUART(void)
{
    Board_initCfg boardCfg;
    Board_STATUS  boardStatus;
    bool          boardAM570x;

#if defined(evmK2E) || defined(evmC6678)
    boardCfg = BOARD_INIT_MODULE_CLOCK;
#else
    boardCfg = BOARD_INIT_PINMUX_CONFIG |
        BOARD_INIT_MODULE_CLOCK;
#endif

    boardStatus = Board_init(boardCfg);
    if (BOARD_SOK != boardStatus)
    {
        return (BFALSE);
    }

    UART_getTestInstNum(&uartTestInstance, &boardAM570x);

#if defined (SOC_AM572x) || defined (SOC_AM571x) || defined (SOC_AM574x)
    CSL_l4per_cm_core_componentRegs *l4PerCmReg =
        (CSL_l4per_cm_core_componentRegs *) CSL_MPU_L4PER_CM_CORE_REGS;

    if (boardAM570x)
    {
#if defined (_TMS320C6X)
        UART_HwAttrs cfg;

        /*
         * AM5 DSP does not have a default Xbar connection for UART
         * interrupt, need the following Xbar interrupt configuration
         */

        /* Use reserved DSP1_IRQ_34 */
        CSL_xbarDspIrqConfigure(1,
                                CSL_XBAR_INST_DSP1_IRQ_34,
                                CSL_XBAR_UART1_IRQ);

        /* configure Xbar for UART2 instance */
        CSL_xbarDspIrqConfigure(1,
                                CSL_XBAR_INST_DSP1_IRQ_35,
                                CSL_XBAR_UART2_IRQ);
        UART_socGetInitCfg(uartTestInstance + 1, &cfg);
        cfg.eventId = 35;
        UART_socSetInitCfg(uartTestInstance + 1, &cfg);
#endif

#if ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'M') && defined(__ARM_FEATURE_SIMD32))
        UART_HwAttrs cfg;

        /*
         * AM571/AM570 IPU does not have a default Xbar connection for UART 1
         * interrupt, need to use a reserved IRQ Xbar instance for Xbar interrupt
         * configuration
         */

        /* Use reserved XBAR_INST_IPU1_IRQ_24 */
        CSL_xbarIpuIrqConfigure(1,
                                CSL_XBAR_INST_IPU1_IRQ_24,
                                CSL_XBAR_UART1_IRQ);

        /* configure Xbar for UART2 instance */
        CSL_xbarIpuIrqConfigure(1,
                                CSL_XBAR_INST_IPU1_IRQ_25,
                                CSL_XBAR_UART2_IRQ);
        UART_socGetInitCfg(uartTestInstance + 1, &cfg);
        cfg.intNum = 25;
        UART_socSetInitCfg(uartTestInstance + 1, &cfg);
#endif

        /* enable UART2 clock */
        CSL_FINST(l4PerCmReg->CM_L4PER_UART2_CLKCTRL_REG,
                  L4PER_CM_CORE_COMPONENT_CM_L4PER_UART2_CLKCTRL_REG_MODULEMODE,
                  ENABLE);

        while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_UART2_CLKCTRL_REG_IDLEST_FUNC !=
              CSL_FEXT(l4PerCmReg->CM_L4PER_UART2_CLKCTRL_REG,
              L4PER_CM_CORE_COMPONENT_CM_L4PER_UART2_CLKCTRL_REG_IDLEST));
    }
    else
    {
#if defined (_TMS320C6X)
        UART_HwAttrs cfg;

        CSL_xbarDspIrqConfigure(1,
                                CSL_XBAR_INST_DSP1_IRQ_34,
                                CSL_XBAR_UART3_IRQ);

        /* configure Xbar for UART4 instance */
        CSL_xbarDspIrqConfigure(1,
                                CSL_XBAR_INST_DSP1_IRQ_35,
                                CSL_XBAR_UART4_IRQ);
        UART_socGetInitCfg(uartTestInstance + 1, &cfg);
        cfg.eventId = 35;
        UART_socSetInitCfg(uartTestInstance + 1, &cfg);
#endif

#if ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'M') && defined(__ARM_FEATURE_SIMD32))
        /*
         * AM57x IPU does not have a default Xbar connection for UART 4
         * interrupt, need to use a reserved IRQ Xbar instance for Xbar interrupt
         * configuration
         */
        /* Use reserved XBAR_INST_IPU1_IRQ_24 */
        CSL_xbarIpuIrqConfigure(1,
                                CSL_XBAR_INST_IPU1_IRQ_24,
                                CSL_XBAR_UART4_IRQ);
#endif

        /* enable UART4 clock */
        CSL_FINST(l4PerCmReg->CM_L4PER_UART4_CLKCTRL_REG,
                  L4PER_CM_CORE_COMPONENT_CM_L4PER_UART4_CLKCTRL_REG_MODULEMODE,
                  ENABLE);

        while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_UART4_CLKCTRL_REG_IDLEST_FUNC !=
              CSL_FEXT(l4PerCmReg->CM_L4PER_UART4_CLKCTRL_REG,
              L4PER_CM_CORE_COMPONENT_CM_L4PER_UART4_CLKCTRL_REG_IDLEST));
    }
#endif

#if defined (SOC_AM335X) || defined (SOC_AM437X)
    /* enable UART clock */
    PRCMModuleEnable(42U, uartTestInstance + 1U, 0U);
#endif
    return (BTRUE);
}

/**
 *  @b Description
 *  @n
 *      Utility function which converts a local GEM L2 memory address
 *      to global memory address.
 *
 *  @param[in]  addr
 *      Local address to be converted
 *
 *  @retval
 *      Computed L2 global Address
 */
static uintptr_t l2_global_address (uintptr_t addr)
{
#if defined(SOC_K2H) || defined(SOC_K2K) || defined(SOC_K2L) || defined(SOC_K2E) || defined(SOC_K2G) || defined(SOC_C6678) || defined(SOC_C6657) || \
    defined(DEVICE_K2H) || defined(DEVICE_K2K) || defined(DEVICE_K2L) || defined(DEVICE_K2E) || defined(DEVICE_K2G) || defined(DEVICE_C6678) || defined(DEVICE_C6657)
#ifdef _TMS320C6X
    uint32_t corenum;

    /* Get the core number. */
    corenum = CSL_chipReadReg (CSL_CHIP_DNUM);

    /* Compute the global address. */
    return (addr + (0x10000000 + (corenum * 0x1000000)));
#else
    return addr;
#endif
#elif defined (SOC_TPR12) || defined (SOC_AWR294X)
    return ((uintptr_t)CSL_locToGlobAddr(addr));
#else
    return addr;
#endif

}

void UART_callback(UART_Handle handle, void *buf, size_t count)
{
    UART_osalPostLock(callbackSem);
}

void UART_callback2(UART_Handle handle, UART_Transaction *transaction)
{
    UART_osalPostLock(callbackSem);
}

#if !(defined(SOC_TPR12) || defined (SOC_AWR294X))

#if defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_J721E) || defined(SOC_J7200)
#define UART_NUM_TRIG_LVL           (2U)
#else
#define UART_NUM_TRIG_LVL           (4U)
#endif
/*
 *  ======== UART_initConfigTrgLvl ========
 */
static void UART_initConfigTrgLvl(uint32_t dmaMode,
                                  uint32_t txTrigLvl,
                                  uint32_t rxTrigLvl)
{
    UART_HwAttrs uart_cfg;

    /* Get the default UART init configurations */
    UART_socGetInitCfg(uartTestInstance, &uart_cfg);

#if defined(SOC_K2H) || defined(SOC_K2K) || defined(SOC_K2L) || defined(SOC_K2E) || defined(SOC_K2G) || defined(SOC_C6678) || defined(SOC_C6657) || defined(SOC_OMAPL137) || defined(SOC_OMAPL138) ||\
    defined(DEVICE_K2H) || defined(DEVICE_K2K) || defined(DEVICE_K2L) || defined(DEVICE_K2E) || defined(DEVICE_K2G) || defined(DEVICE_C6678) || defined(DEVICE_C6657) || defined(DEVICE_OMAPL137) || defined(DEVICE_OMAPL138)
    uart_cfg.txTrigLvl  = txTrigLvl;
#else
    uart_cfg.txTrigLvl  = (UART_TxTrigLvl)txTrigLvl;
#endif
    uart_cfg.rxTrigLvl  = (UART_RxTrigLvl)rxTrigLvl;
    uart_cfg.loopback   = UTRUE;
#ifdef UART_DMA_ENABLE
    if (BTRUE == dmaMode)
    {
#if defined (SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
        uart_cfg.edmaHandle = UartApp_udmaInit(&uart_cfg);
#else
        uart_cfg.edmaHandle = UartApp_edmaInit();
#endif
        uart_cfg.dmaMode    = UTRUE;
    }
    else
#endif
    {
        uart_cfg.edmaHandle = NULL;
        uart_cfg.dmaMode    = UFALSE;
    }

    /* Set the TX/RX FIFO trigger levels */
    UART_socSetInitCfg(uartTestInstance, &uart_cfg);
}

bool UART_test_trglvl_xfer(UART_Handle uart, uint32_t dmaMode, uint32_t xferSize)
{
    SemaphoreP_Params semParams;
    UART_Transaction  transaction;
    uintptr_t          addrFifoTrgLvlData, addrScanPrompt;
    uint32_t          i;
    bool              ret = BFALSE;

    if (0U == xferSize)
    {
        return BTRUE;
    }
    /* Create call back semaphore */
    UART_osalSemParamsInit(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    callbackSem = UART_osalCreateBlockingLock(0, &semParams);

    if (dmaMode)
    {
        addrFifoTrgLvlData = l2_global_address((uintptr_t)fifoTrgLvlData);
        addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
    }
    else
    {
        addrFifoTrgLvlData = (uintptr_t)fifoTrgLvlData;
        addrScanPrompt = (uintptr_t)scanPrompt;
    }

    /* Read in call back mode */
    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
        CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }

    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrScanPrompt;
    callbackTransaction.count = xferSize;
    if (UART_ERROR == UART_read2(uart, &callbackTransaction))
    {
        goto Err;
    }

    /* Write in blocking mode */
    if (dmaMode)
    {
        CacheP_wbInv((void *)(uintptr_t)addrFifoTrgLvlData, (int32_t)sizeof(addrFifoTrgLvlData));
    }
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrFifoTrgLvlData;
    transaction.count = xferSize;
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

    /* Wait for read callback */
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, callbackTransaction.timeout))
    {
        goto Err;
    }

    /* Check if read data matches with write data */
    for (i = 0; i < xferSize; i++)
    {
        if (scanPrompt[i] != fifoTrgLvlData[i])
        {
            goto Err;
        }
    }
    ret = BTRUE;

Err:
    if (callbackSem)
    {
	    UART_osalDeleteBlockingLock(callbackSem);
	    callbackSem = NULL;
    }
    return (ret);
}

bool UART_test_trglvl(uint32_t dmaMode,
                      int32_t txTrigLvl,
                      int32_t rxTrigLvl)
{
    UART_Handle       uart = NULL;
    UART_Params       uartParams;
    bool              ret = BFALSE;
    int32_t           i;

    /* UART SoC init configuration */
    UART_initConfigTrgLvl(dmaMode, (uint32_t)(uintptr_t)txTrigLvl, (uint32_t)(uintptr_t)rxTrigLvl);

    /* Read in callback mode and write in blocking mode */
    UART_Params_init(&uartParams);
    uartParams.readCallback = UART_callback;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.parityType = uartParity;

    for ( i = -1; i < 2; i++)
    {
        uart = UART_open(uartTestInstance, &uartParams);
        if (NULL == uart)
        {
            goto Err;
        }
        if (BFALSE == UART_test_trglvl_xfer(uart, dmaMode, (uint32_t)(uintptr_t)(txTrigLvl + i)))
        {
            goto Err;
        }
        if (uart)
        {
            UART_close(uart);
            uart = NULL;
        }

        uart = UART_open(uartTestInstance, &uartParams);
        if (NULL == uart)
        {
            goto Err;
        }

        if (BFALSE == UART_test_trglvl_xfer(uart, dmaMode, (uint32_t)(uintptr_t)(rxTrigLvl + i)))
        {
            goto Err;
        }

        if (uart)
        {
            UART_close(uart);
            uart = NULL;
        }
    }

    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }
    return (ret);
}

/*
 *  ======== UART DMA TX/RX FIFO trigger level test ========
 */
static bool UART_test_fifo_trglvl(bool dmaMode)
{
    bool     ret = BTRUE;
    uint32_t i;
#if defined(SOC_AM574x) || defined(SOC_AM572x)|| defined(SOC_AM571x) || defined (SOC_DRA72x)  || defined (SOC_DRA75x) || defined (SOC_DRA78x) || defined (SOC_AM335X) || defined (SOC_AM437X) || defined (SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
    UART_TxTrigLvl txTrgLvl[UART_NUM_TRIG_LVL] =
    {
        UART_TXTRIGLVL_8,
        UART_TXTRIGLVL_16,
    };

    UART_RxTrigLvl rxTrgLvl[UART_NUM_TRIG_LVL] =
    {
        UART_RXTRIGLVL_8,
        UART_RXTRIGLVL_16,
    };
#else
    uint32_t txTrgLvl[UART_NUM_TRIG_LVL] =
    {
        2,
        4,
        8,
        16
    };

    UART_RxTrigLvl rxTrgLvl[UART_NUM_TRIG_LVL] =
    {
        UART_RXTRIGLVL_1,
        UART_RXTRIGLVL_4,
        UART_RXTRIGLVL_8,
        UART_RXTRIGLVL_14
    };
#endif
    for (i = 0; i < UART_NUM_TRIG_LVL; i++)
    {
        if (BFALSE == UART_test_trglvl(dmaMode, (int32_t)txTrgLvl[i], (int32_t)rxTrgLvl[i]))
        {
            ret = BFALSE;
            break;
        }
    }

    return (ret);
}

#endif /* TPR12: no UART fifo */
/*
 *  ======== UART stdio printf/scanf test ========
 *
 *  The test function tests stdio driver printf/scanf APIs
 */
static bool UART_test_printf_scanf(bool dmaMode)
{
    bool    ret = BFALSE;

    if (UART_PAR_NONE == uartParity)
    {
        /* UART SoC init configuration */
        UART_initConfig(dmaMode);

        UART_stdioInit(uartTestInstance);

        UART_printf(stdioPrint);

#ifndef UART_RX_LOOPBACK_ONLY
        memset((void *)scanPrompt, 0, sizeof(scanPrompt));
        if (S_PASS != UART_scanFmt((const char *)scanPrompt))
        {
            goto Err;
        }
        ret = BTRUE;

Err:
#else
        ret = BTRUE;
#endif
        UART_stdioDeInit();
    }
    else
    {
        /*
         * bypass this test if the parity is not the default setting
         * (UART_PAR_NONE), since UART_stdioInit() only allows default
         * UART parameter settings.
         */
        ret = BTRUE;
    }

    return (ret);
}

/*
 *  ======== UART stdio printf/scanf test with param config (default) ========
 *
 *  The test function tests stdio driver printf/scanf APIs with default params config.
 */
static bool UART_test_printf_scanf_stdio_params(bool dmaMode)
{
    bool    ret = BFALSE;
    UART_Params params;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* UART params */
    UART_Params_init(&params);
    params.parityType = uartParity;

    UART_stdioInit2(uartTestInstance,&params);

    UART_printf(stdioPrint);

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
#ifndef UART_RX_LOOPBACK_ONLY
    if (S_PASS != UART_scanFmt((const char *)scanPrompt))
    {
        goto Err;
    }
    ret = BTRUE;

Err:
#else
    ret = BTRUE;
#endif
    UART_stdioDeInit();
    return (ret);
}

#if !defined(UART_API2_NOT_SUPPORTED)
#if !defined(BAREMETAL)
#if defined (BUILD_C7X)
#define APP_TSK_STACK_WRITE              (32U * 1024U)
#else
#define APP_TSK_STACK_WRITE              (8U * 1024U)
#endif /* #if defined (BUILD_C7X) */
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS) && defined (BUILD_MCU)
static uint8_t  gAppTskStackWrite[APP_TSK_STACK_WRITE] __attribute__((aligned(APP_TSK_STACK_WRITE)));
#else
static uint8_t  gAppTskStackWrite[APP_TSK_STACK_WRITE] __attribute__((aligned(0x2000)));
#endif
/* Use a global variable to sync the read task and the write task */
volatile bool taskSyncFlag;
void UART_simultaneous_rw_write(void *a0, void *a1)
{
	UART_Handle      uart = (UART_Handle)a0;
	bool             dmaMode = (bool)a1;
    uintptr_t         addrDataPrint;
    UART_Transaction transaction;

	if (dmaMode)
    {
    	addrDataPrint = l2_global_address((uintptr_t)dataPrint);
    }
    else
    {
    	addrDataPrint = (uintptr_t)dataPrint;
    }

    UART_transactionInit(&transaction);

    while (BTRUE == taskSyncFlag)
	{
        transaction.buf = (void *)(uintptr_t)addrDataPrint;
	    transaction.count = strlen(dataPrint);
	    UART_write2(uart, &transaction);
	    Osal_delay(100);
	}

    /* resume the read test task */
    taskSyncFlag = BTRUE;
}

/*
 *  ======== UART simultaneous read/write test ========
 *
 *  The read task creates a write task which will continuously
 *  writes the data out to the console, while at the same time
 *  the read task reads the data from the console input.
 *
 *  Note:
 *  In blocking/interrupt mode, if the write size is less than
 *  the TX FIFO size (64 bytes), the driver will just copy
 *  the write data to the TX FIFO and will not use interrupt
 *  and write semaphore. The read task should have a higher
 *  priority than the write task to resume the task by the
 *  scheduler once it gets the read semaphore.
 *
 */
static bool UART_test_simultaneous_rw(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    uintptr_t         addrScanPrompt, addrEchoPrompt;
    UART_Transaction transaction;
    TaskP_Handle      writeTask;
    TaskP_Params      writeTaskParams;
    bool              ret = BFALSE;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    /* run the write teas when task is created */
    taskSyncFlag = BTRUE;

    /* Initialize the task params */
    TaskP_Params_init(&writeTaskParams);
    writeTaskParams.arg0 = (void *)uart;
    writeTaskParams.arg1 = (void *)dmaMode;
    writeTaskParams.stack = gAppTskStackWrite;
    writeTaskParams.stacksize = sizeof (gAppTskStackWrite);
    /*
     * Set the write task priority to the default priority (1)
     * lower than the read task priority (2)
     */
    writeTaskParams.priority = 1;

    /* Create the UART write task */
    writeTask = TaskP_create(&UART_simultaneous_rw_write, &writeTaskParams);
    if (NULL == writeTask)
    {
        goto Err;
    }

    if (dmaMode)
    {
        addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
        addrEchoPrompt = l2_global_address((uintptr_t)echoPrompt);
    }
    else
    {
    	addrScanPrompt = (uintptr_t)scanPrompt;
        addrEchoPrompt = (uintptr_t)echoPrompt;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_read2(uart, &transaction))
    {
    	taskSyncFlag = BFALSE;
        goto Err;
    }
    if ((UART_TRANSFER_STATUS_SUCCESS != transaction.status) ||
        (UART_TEST_READ_LEN           != transaction.count))
    {
    	taskSyncFlag = BFALSE;
        goto Err;
    }

    /* stop the write test task */
    taskSyncFlag = BFALSE;

    /* Wait for the write task complete and exit */
    while (BFALSE == taskSyncFlag)
    {
        Osal_delay(100);
    }
    taskSyncFlag = BFALSE;
    /* Delete write task */
    TaskP_delete(&writeTask);

    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrEchoPrompt;
    transaction.count = strlen(echoPrompt);
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

    Osal_delay(5000);

    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }

    return (ret);
}
#endif /* #if !defined(BAREMETAL) */
#endif /* #if !defined(UART_API2_NOT_SUPPORTED) */

/*
 *  ======== UART read cancel test ========
 *
 *  The test function uses console intput to simulate/test the
 *  read cancel in callback mode. In a real use case, user can
 *  read a large file from the console and cancel the read before
 *  the read is complete.
 *
 */
static bool UART_test_read_write_cancel(bool dmaMode)
{
    UART_Handle       uart = NULL;
    UART_Params       uartParams;
    SemaphoreP_Params semParams;
    UART_Transaction  transaction;
    uintptr_t          addrRdCancelPrompt, addrWrCancelPrompt;
    uintptr_t          addrDataPrint, addrEchoPrompt, addrScanPrompt;
    bool              ret = BFALSE;

    /* Create call back semaphore */
    UART_osalSemParamsInit(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    callbackSem = UART_osalCreateBlockingLock(0, &semParams);

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Set callback mode for read */
    UART_Params_init(&uartParams);
    uartParams.readCallback = UART_callback;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.parityType = uartParity;

    if (dmaMode)
    {
    	addrRdCancelPrompt = l2_global_address((uintptr_t)rdCancelPrompt);
    	addrDataPrint = l2_global_address((uintptr_t)dataPrint);
    	addrEchoPrompt = l2_global_address((uintptr_t)echoPrompt);
    	addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
    }
    else
    {
    	addrRdCancelPrompt = (uintptr_t)rdCancelPrompt;
    	addrDataPrint = (uintptr_t)dataPrint;
    	addrEchoPrompt = (uintptr_t)echoPrompt;
    	addrScanPrompt = (uintptr_t)scanPrompt;
    }

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    /* Test receive error */

    /* Perform the first read, which will be cancelled before completion */
    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }

    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrScanPrompt;
    callbackTransaction.count = UART_TEST_READ_LEN;
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_read2(uart, &callbackTransaction))
#else
    if (UART_ERROR == UART_read(uart, callbackTransaction.buf, UART_TEST_READ_LEN))
#endif
    {
        goto Err;
    }
    /* Delay for 10 seconds to allow user to enter chars */
#if !defined(SIM_BUILD)
    Osal_delay(10000);
#else
    Osal_delay(100);
#endif
    /* Cancel the read before the read transfer is completed */
    UART_readCancel(uart);
    if (UART_osalPendLock(callbackSem, callbackTransaction.timeout) != SemaphoreP_OK)
    {
        goto Err;
    }

    /* Print read cancelled prompt */
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrRdCancelPrompt;
    transaction.count = strlen(rdCancelPrompt);
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_write2(uart, &transaction))
#else
    if (UART_ERROR == UART_write(uart, transaction.buf, transaction.count))
#endif
    {
        goto Err;
    }

    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrDataPrint;
    transaction.count = strlen(dataPrint);
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_write2(uart, &transaction))
#else
    if (UART_ERROR == UART_write(uart, transaction.buf, transaction.count))
#endif
    {
        goto Err;
    }

    /* Perform the 2nd read, which will be completed */
    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }

#if !defined(UART_RX_LOOPBACK_ONLY)
    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrScanPrompt;
    callbackTransaction.count = UART_TEST_READ_LEN;
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_read2(uart, &callbackTransaction))
#else
    if (UART_ERROR == UART_read(uart, callbackTransaction.buf, UART_TEST_READ_LEN))
#endif
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, callbackTransaction.timeout))
    {
        goto Err;
    }
#endif
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrEchoPrompt;
    transaction.count = strlen(echoPrompt);
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_write2(uart, &transaction))
#else
    if (UART_ERROR == UART_write(uart, transaction.buf, transaction.count))
#endif
    {
        goto Err;
    }

    /* Print the 2nd read chars, should NOT contain any chars
     * in the first cancelled read
     */
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = sizeof(scanPrompt);
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_write2(uart, &transaction))
#else
    if (UART_ERROR == UART_write(uart, transaction.buf, transaction.count))
#endif
    {
        goto Err;
    }

    UART_close(uart);

    /* write cancel test */

    /* Set callback mode for write */
    UART_Params_init(&uartParams);
    uartParams.writeCallback = UART_callback;
    uartParams.writeMode = UART_MODE_CALLBACK;
    uartParams.parityType = uartParity;

    if (dmaMode)
    {
        addrWrCancelPrompt = l2_global_address((uintptr_t)wrCancelPrompt);
    }
    else
    {
        addrWrCancelPrompt = (uintptr_t)wrCancelPrompt;
    }

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    /* Perform the 1st write */
    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrWrCancelPrompt;
    callbackTransaction.count = strlen(wrCancelPrompt);
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_write2(uart, &callbackTransaction))
#else
    if (UART_ERROR == UART_write(uart, callbackTransaction.buf, callbackTransaction.count))
#endif
    {
        goto Err;
    }
    /* Cancel the 1st write */
    UART_writeCancel(uart);
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, callbackTransaction.timeout))
    {
        goto Err;
    }

    /* Perform the 2nd write */
    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrWrCancelPrompt;
    callbackTransaction.count = strlen(wrCancelPrompt);
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_write2(uart, &callbackTransaction))
#else
    if (UART_ERROR == UART_write(uart, callbackTransaction.buf, callbackTransaction.count))
#endif
    {
        goto Err;
    }
    /* Cancel the write */
    UART_writeCancel(uart);
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, callbackTransaction.timeout))
    {
        goto Err;
    }
    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }

    if (callbackSem)
    {
    	UART_osalDeleteBlockingLock(callbackSem);
    	callbackSem = NULL;
    }

    return (ret);
}

#if !defined(UART_API2_NOT_SUPPORTED)
/*
 *  ======== UART receive error test ========
 *
 *  The test function tests receive error (e.g. break condition)
 */
static bool UART_test_rx_err(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    UART_Transaction transaction;
    uintptr_t         addrScanPrompt, addrBreakErrPrompt;
    bool             ret = BFALSE;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;

    if (dmaMode)
    {
    	addrBreakErrPrompt = l2_global_address((uintptr_t)breakErrPrompt);
    	addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
    }
    else
    {
    	addrBreakErrPrompt = (uintptr_t)breakErrPrompt;
    	addrScanPrompt = (uintptr_t)scanPrompt;
    }

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    /* Test receive error */
    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_read2(uart, &transaction))
    {
    	if (UART_TRANSFER_STATUS_ERROR_BI == transaction.status)
    	{
    	    UART_transactionInit(&transaction);
            transaction.buf = (void *)(uintptr_t)addrBreakErrPrompt;
            transaction.count = strlen(breakErrPrompt);
            if (UART_ERROR == UART_write2(uart, &transaction))
            {
                goto Err;
            }
    	}
    	else
        {
            goto Err;
        }
    }

    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }

    return (ret);
}

#endif

/*
 *  ======== UART timeout test ========
 *
 *  The test function tests read/write with OS timeout
 */
static bool UART_test_timeout(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    UART_Transaction transaction;
    uintptr_t         addrScanPrompt, addrReadTimeoutPrompt;
    bool             ret = BFALSE;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;
    uartParams.readTimeout = UART_TEST_TIMEOUT;

    if (dmaMode)
    {
    	addrReadTimeoutPrompt = l2_global_address((uintptr_t)readTimeoutPrompt);
    	addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
    }
    else
    {
    	addrReadTimeoutPrompt = (uintptr_t)readTimeoutPrompt;
    	addrScanPrompt = (uintptr_t)scanPrompt;
    }

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    /* Test read timeout */
    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)scanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    transaction.timeout = UART_TEST_TIMEOUT;
#if !defined(UART_API2_NOT_SUPPORTED)
    if (UART_ERROR == UART_read2(uart, &transaction))
#else
    if (UART_ERROR == UART_read(uart, transaction.buf, transaction.count))
#endif
    {
        UART_transactionInit(&transaction);
        transaction.buf = (void *)(uintptr_t)addrReadTimeoutPrompt;
        transaction.count = strlen(readTimeoutPrompt);
        transaction.timeout = UART_TEST_TIMEOUT;
        if (UART_ERROR == UART_write2(uart, &transaction))
        {
            goto Err;
        }
    }

    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }

    return (ret);
}

/*
 *  ======== UART polling timeout test ========
 *
 *  The test function tests read/write with OS timeout in polling mode
 */
static bool UART_test_polling_timeout(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    bool             ret = BFALSE;
    uint32_t         rdSize = UART_TEST_READ_LEN;
    uint32_t         wrSize = strlen(readTimeoutPrompt);

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;

    /* timeout is 0 for both read and write */
    uartParams.readTimeout  = UART_NO_WAIT;
    uartParams.writeTimeout = UART_NO_WAIT;
    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (UART_readPolling(uart, (void *)(uintptr_t)scanPrompt, rdSize) != rdSize)
    {
        if (UART_writePolling(uart, (const void *)readTimeoutPrompt, wrSize) != wrSize)
        {
            ret = BTRUE;
        }
    }

    if (BTRUE == ret)
    {
        ret = BFALSE;

        UART_close(uart);

        /* timeout is 5 seconds for both read and write */
        uartParams.readTimeout  = UART_TEST_TIMEOUT;
        uartParams.writeTimeout = UART_TEST_TIMEOUT;
        uart = UART_open(uartTestInstance, &uartParams);
        if (NULL == uart)
        {
            goto Err;
        }

        /* Test read timeout */
        memset((void *)scanPrompt, 0, sizeof(scanPrompt));
        if (UART_readPolling(uart, (void *)(uintptr_t)scanPrompt, rdSize) != rdSize)
        {
            UART_writePolling(uart, (const void *)readTimeoutPrompt, wrSize);
            ret = BTRUE;
        }
    }

    if (BTRUE == ret)
    {
        ret = BFALSE;

        UART_close(uart);

        /* timeout is 5 seconds for both read and write */
        uartParams.readTimeout  = UART_WAIT_FOREVER;
        uartParams.writeTimeout = UART_WAIT_FOREVER;
        uart = UART_open(uartTestInstance, &uartParams);
        if (NULL == uart)
        {
            goto Err;
        }

        wrSize = strlen(dataPrint);
        UART_writePolling(uart, (const void *)dataPrint, wrSize);

        memset((void *)scanPrompt, 0, sizeof(scanPrompt));

#if !defined(UART_RX_LOOPBACK_ONLY)
        if (UART_readPolling(uart, (void *)(uintptr_t)scanPrompt, rdSize) != rdSize)
        {
            goto Err;
        }
#endif
        wrSize = strlen(echoPrompt);
        UART_writePolling(uart, (const void *)echoPrompt, wrSize);
        UART_writePolling(uart, (const void *)scanPrompt, rdSize);

        ret = BTRUE;
    }

Err:
    if (uart)
    {
        UART_close(uart);
    }

    return (ret);
}

/*
 *  ======== UART callback test ========
 *
 *  The test function tests the read/write in callback mode
 */
static bool UART_test_callback(bool dmaMode)
{
    UART_Handle       uart = NULL;
    UART_Params       uartParams;
    SemaphoreP_Params semParams;
    uintptr_t          addrScanPrompt, addrDataPrint, addrEchoPrompt;
    bool              ret = BFALSE;

    /* Create call back semaphore */
    UART_osalSemParamsInit(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    callbackSem = UART_osalCreateBlockingLock(0, &semParams);

    /* Test read/write API's in callback mode */

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Set callback mode for both read and write */
    UART_Params_init(&uartParams);
    uartParams.readCallback = UART_callback;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.writeCallback = UART_callback;
    uartParams.writeMode = UART_MODE_CALLBACK;
    uartParams.parityType = uartParity;

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    if (dmaMode)
    {
    	addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
    	addrDataPrint = l2_global_address((uintptr_t)dataPrint);
    	addrEchoPrompt = l2_global_address((uintptr_t)echoPrompt);
    }
    else
    {
    	addrScanPrompt = (uintptr_t)scanPrompt;
    	addrDataPrint = (uintptr_t)dataPrint;
    	addrEchoPrompt = (uintptr_t)echoPrompt;
    }

    /* Write DMA or non-DMA test prompt */
    if (UART_ERROR == UART_write(uart, (void *)(uintptr_t)addrDataPrint, strlen(dataPrint)))
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, uartParams.writeTimeout))
    {
        goto Err;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }

#if !defined(UART_RX_LOOPBACK_ONLY)

    if (UART_ERROR == UART_read(uart, (void *)(uintptr_t)addrScanPrompt, UART_TEST_READ_LEN))
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, uartParams.readTimeout))
    {
        goto Err;
    }

#endif

    if (UART_ERROR == UART_write(uart, (void *)(uintptr_t)addrEchoPrompt, strlen(echoPrompt)))
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, uartParams.writeTimeout))
    {
        goto Err;
    }

    if (UART_ERROR == UART_write(uart, (void *)(uintptr_t)addrScanPrompt, UART_TEST_READ_LEN))
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, uartParams.writeTimeout))
    {
        goto Err;
    }

    UART_close(uart);
    uart = NULL;

    /* Test read2/write2 API's in callback mode */
#if !defined(UART_API2_NOT_SUPPORTED)
    uartParams.readCallback2 = UART_callback2;
    uartParams.writeCallback2 = UART_callback2;
    uartParams.readCallback = NULL;
    uartParams.writeCallback = NULL;

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrDataPrint;
    callbackTransaction.count = strlen(dataPrint);
    if (UART_ERROR == UART_write2(uart, &callbackTransaction))
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, callbackTransaction.timeout))
    {
        goto Err;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }

#if !defined(UART_RX_LOOPBACK_ONLY)
    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrScanPrompt;
    callbackTransaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_read2(uart, &callbackTransaction))
    {
        goto Err;
    }
    if (UART_osalPendLock(callbackSem, callbackTransaction.timeout) != SemaphoreP_OK)
    {
        goto Err;
    }
#endif

    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrEchoPrompt;
    callbackTransaction.count = strlen(echoPrompt);
    if (UART_ERROR == UART_write2(uart, &callbackTransaction))
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, callbackTransaction.timeout))
    {
        goto Err;
    }

    UART_transactionInit(&callbackTransaction);
    callbackTransaction.buf = (void *)(uintptr_t)addrScanPrompt;
    callbackTransaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_write2(uart, &callbackTransaction))
    {
        goto Err;
    }
    if (SemaphoreP_OK != UART_osalPendLock(callbackSem, callbackTransaction.timeout))
    {
        goto Err;
    }

#endif
    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }
    if (callbackSem)
    {
    	UART_osalDeleteBlockingLock(callbackSem);
    	callbackSem = NULL;
    }
    return (ret);
}

#if defined (SOC_AM64X) && defined(BUILD_MCU)
/* RS-485 Direction Enable loopback test */
static bool UART_test_rs485(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    int16_t          length = 0;
    bool             ret = BTRUE;
    uint8_t rBuff[UART_TEST_READ_LEN], tBuff[]="aaaabbbbccccddddeeee";

    /* UART SoC init configuration */
    verifyRS485    = UTRUE;
    verifyLoopback = UTRUE;
    UART_initConfig(dmaMode);

    UART_Params_init(&uartParams);
    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    if (UART_ERROR == UART_write(uart, (void *)&tBuff[0], UART_TEST_READ_LEN))
    {
        goto Err;
    }

    memset(rBuff, 0, UART_TEST_READ_LEN);
    UART_read(uart, (void *)&rBuff[0], UART_TEST_READ_LEN);
    for(length = 0; length < UART_TEST_READ_LEN; length++)
    {
        if(tBuff[length] != rBuff[length])
        {
            ret = BFALSE;
            break;
        }
    }
    UART_close(uart);

Err:
    if (NULL != uart)
    {
        UART_close(uart);
    }

    verifyRS485    = UFALSE;
    verifyLoopback = UFALSE;
    return (ret);
}
#endif

/*
 *  ========== UART read API test ==========
 *
 *  The test function for UART_read API
 *  in loopback mode
 */
static bool UART_test_read_verify(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    int16_t          length = 0;
    bool             ret = BFALSE;
    uint8_t rBuff[UART_TEST_READ_LEN], tBuff[]="aaaabbbbccccddddeeee";
#if defined(UART_NO_FIFO)
    int i;
#endif

    verifyLoopback = UTRUE;
    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;

    uart = UART_open(uartTestInstance, &uartParams);

    if (NULL == uart)
    {
        goto Err;
    }

#if !defined(UART_NO_FIFO)

    if (UART_ERROR == UART_write(uart, (void *)&tBuff[0], UART_TEST_READ_LEN))
    {
        goto Err;
    }

    length = UART_read(uart, (void *)&rBuff[0], UART_RDVERIFY_READ_LEN);

    if (UART_ERROR == UART_write(uart, (void *)&tBuff[15], UART_RDVERIFY_READ_LEN))
    {
        goto Err;
    }

    length = UART_read(uart, (void *)&rBuff[4], UART_RDVERIFY_READ_LEN);
    length = UART_read(uart, (void *)&rBuff[8], UART_RDVERIFY_READ_LEN);
    length = UART_read(uart, (void *)&rBuff[12], UART_RDVERIFY_READ_LEN);
#else
    memset(rBuff, 0, UART_TEST_READ_LEN);
    for (i = 0; i < UART_TEST_READ_LEN;i++)
    {
        if (UART_ERROR == UART_write(uart, (void *)&tBuff[i], 1))
        {
            goto Err;
        }

        length += UART_readPolling(uart, (void *)&rBuff[i], 1);
    }

#endif

    ret = BTRUE;
    for(length = 0; length < UART_TEST_READ_LEN; length++)
    {
        if(tBuff[length] != rBuff[length])
        {
            ret = BFALSE;
            break;
        }
    }
    UART_close(uart);

    /* stop bit loopback test */
    UART_Params_init(&uartParams);
    uartParams.stopBits = UART_STOP_TWO;
    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

#if !defined(UART_NO_FIFO)
    if (UART_ERROR == UART_write(uart, (void *)&tBuff[0], UART_TEST_READ_LEN))
    {
        goto Err;
    }

    memset(rBuff, 0, UART_TEST_READ_LEN);
    UART_read(uart, (void *)&rBuff[0], UART_TEST_READ_LEN);
#else
    memset(rBuff, 0, UART_TEST_READ_LEN);
    for (i = 0; i < UART_TEST_READ_LEN;i++)
    {
        if (UART_ERROR == UART_write(uart, (void *)&tBuff[i], 1))
        {
            goto Err;
        }

        length += UART_readPolling(uart, (void *)&rBuff[i], 1);
    }
#endif
    for(length = 0; length < UART_TEST_READ_LEN; length++)
    {
        if(tBuff[length] != rBuff[length])
        {
            ret = BFALSE;
            break;
        }
    }
    UART_close(uart);

    /* char length loopback test */
    UART_Params_init(&uartParams);
    uartParams.dataLength = UART_LEN_5;
    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

#if !defined(UART_NO_FIFO)
    if (UART_ERROR == UART_write(uart, (void *)&tBuff[0], UART_TEST_READ_LEN))
    {
        goto Err;
    }

    memset(rBuff, 0, UART_TEST_READ_LEN);
    UART_read(uart, (void *)&rBuff[0], UART_TEST_READ_LEN);
#else
    memset(rBuff, 0, UART_TEST_READ_LEN);
    for (i = 0; i < UART_TEST_READ_LEN;i++)
    {
        if (UART_ERROR == UART_write(uart, (void *)&tBuff[i], 1))
        {
            goto Err;
        }

        length += UART_readPolling(uart, (void *)&rBuff[i], 1);
    }
#endif
    for(length = 0; length < UART_TEST_READ_LEN; length++)
    {
        if ((tBuff[length] & ((1 << (uartParams.dataLength + 5)) - 1)) != rBuff[length])
        {
            ret = BFALSE;
            break;
        }
    }
    UART_close(uart);

    /* parity loopback test */
    UART_Params_init(&uartParams);
    uartParams.parityType = UART_PAR_EVEN;
    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }
#if !defined(UART_NO_FIFO)
    if (UART_ERROR == UART_write(uart, (void *)&tBuff[0], UART_TEST_READ_LEN))
    {
        goto Err;
    }

    memset(rBuff, 0, UART_TEST_READ_LEN);
    UART_read(uart, (void *)&rBuff[0], UART_TEST_READ_LEN);
#else
    memset(rBuff, 0, UART_TEST_READ_LEN);
    for (i = 0; i < UART_TEST_READ_LEN;i++)
    {
        if (UART_ERROR == UART_write(uart, (void *)&tBuff[i], 1))
        {
            goto Err;
        }

        length += UART_readPolling(uart, (void *)&rBuff[i], 1);
    }
#endif
    for(length = 0; length < UART_TEST_READ_LEN; length++)
    {
        if(tBuff[length] != rBuff[length])
        {
            ret = BFALSE;
            break;
        }
    }
    UART_close(uart);
    uart = NULL;

#if defined (SOC_AM64X) && defined(BUILD_MCU)
    if (BFALSE == UART_test_rs485(dmaMode))
    {
        ret = BFALSE;
    }
#endif
Err:
    if (NULL != uart)
    {
        UART_close(uart);
    }

    verifyLoopback = UFALSE;
    return (ret);
}

/*
 *  ======== UART read/write test ========
 *
 *  The test function tests read/write in blocking mode
 */
static bool UART_test_read_write(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    int              length = 0;
    uintptr_t        addrDataPrint, addrScanPrompt, addrEchoPrompt;
#if !defined(UART_API2_NOT_SUPPORTED)
    UART_Transaction transaction;
#endif
    bool             ret = BFALSE;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;

    uart = UART_open(uartTestInstance, &uartParams);

    if (NULL == uart)
    {
        goto Err;
    }

    if (dmaMode)
    {
    	addrDataPrint = l2_global_address((uintptr_t)dataPrint);
    	addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
    	addrEchoPrompt = l2_global_address((uintptr_t)echoPrompt);
    }
    else
    {
    	addrDataPrint = (uintptr_t)dataPrint;
    	addrScanPrompt = (uintptr_t)scanPrompt;
    	addrEchoPrompt = (uintptr_t)echoPrompt;
    }

    /* Test read/write API's in blocking mode */
    if (UART_ERROR == UART_write(uart, (void *)(uintptr_t)addrDataPrint, strlen(dataPrint)))
    {
        goto Err;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }
#ifndef UART_RX_LOOPBACK_ONLY
    length = UART_read(uart, (void *)(uintptr_t)addrScanPrompt, UART_TEST_READ_LEN);
    if (UART_TEST_READ_LEN != length)
    {
    	goto Err;
    }
#endif

    UART_write(uart, (void *)(uintptr_t)addrEchoPrompt, strlen(echoPrompt));

    UART_write(uart, (void *)(uintptr_t)addrScanPrompt, length);
    UART_close(uart);

#if !defined(UART_API2_NOT_SUPPORTED)
    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

    /* Test read2/write2 API's in blocking mode */
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrDataPrint;
    transaction.count = strlen(dataPrint);
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
    	CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_read2(uart, &transaction))
    {
        goto Err;
    }
    if ((UART_TRANSFER_STATUS_SUCCESS != transaction.status) ||
        (UART_TEST_READ_LEN           != transaction.count))
    {
        goto Err;
    }

    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrEchoPrompt;
    transaction.count = strlen(echoPrompt);
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

#else
    uart = NULL;
#endif

    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }

    return (ret);
}

/*
 *  ======== UART read/write test with interrupt disabled ========
 *
 *  The test function tests read/write with interrupt disabled
 */
static bool UART_test_read_write_int_disable(bool dmaMode)
{
    UART_Handle      uart = NULL;
    UART_Params      uartParams;
    int              length = 0;
    uintptr_t         addrDataPrint, addrScanPrompt, addrEchoPrompt;
    UART_Transaction transaction;
    bool             ret = BFALSE;
    UART_HwAttrs uart_cfg;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;

    /* Get the default UART init configurations */
    UART_socGetInitCfg(uartTestInstance, &uart_cfg);

    uart_cfg.enableInterrupt = UFALSE; /* Disabling interrupt forcefully */

    /* Get the default UART init configurations */
    UART_socSetInitCfg(uartTestInstance, &uart_cfg);

    uart = UART_open(uartTestInstance, &uartParams);

    if (NULL == uart)
    {
        goto Err;
    }

    if (dmaMode)
    {
        addrDataPrint = l2_global_address((uintptr_t)dataPrint);
        addrScanPrompt = l2_global_address((uintptr_t)scanPrompt);
        addrEchoPrompt = l2_global_address((uintptr_t)echoPrompt);
    }
    else
    {
        addrDataPrint = (uintptr_t)dataPrint;
        addrScanPrompt = (uintptr_t)scanPrompt;
        addrEchoPrompt = (uintptr_t)echoPrompt;
    }

    /* Test read/write API's in blocking mode */
    if (UART_ERROR == UART_write(uart, (void *)(uintptr_t)addrDataPrint, strlen(dataPrint)))
    {
        goto Err;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
        CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }

#if !defined(UART_RX_LOOPBACK_ONLY)
    length = UART_read(uart, (void *)(uintptr_t)addrScanPrompt, UART_TEST_READ_LEN);
    if (UART_TEST_READ_LEN != length)
    {
        goto Err;
    }
#endif
    UART_write(uart, (void *)(uintptr_t)addrEchoPrompt, strlen(echoPrompt));

    UART_write(uart, (void *)(uintptr_t)addrScanPrompt, length);
    UART_close(uart);

    uart = UART_open(uartTestInstance, &uartParams);
    if (NULL == uart)
    {
        goto Err;
    }

#if !defined(UART_API_NOT_SUPPORTED)
    /* Test read2/write2 API's in blocking mode */
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrDataPrint;
    transaction.count = strlen(dataPrint);
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

    memset((void *)scanPrompt, 0, sizeof(scanPrompt));
    if (dmaMode)
    {
        CacheP_wbInv((void *)(uintptr_t)addrScanPrompt, (int32_t)sizeof(scanPrompt));
    }
    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_read2(uart, &transaction))
    {
        goto Err;
    }
    if ((UART_TRANSFER_STATUS_SUCCESS != transaction.status) ||
        (UART_TEST_READ_LEN           != transaction.count))
    {
        goto Err;
    }

    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrEchoPrompt;
    transaction.count = strlen(echoPrompt);
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

    UART_transactionInit(&transaction);
    transaction.buf = (void *)(uintptr_t)addrScanPrompt;
    transaction.count = UART_TEST_READ_LEN;
    if (UART_ERROR == UART_write2(uart, &transaction))
    {
        goto Err;
    }

#endif
    ret = BTRUE;

Err:
    if (uart)
    {
        UART_close(uart);
    }

    /* Get the default UART init configurations */
    UART_socGetInitCfg(uartTestInstance, &uart_cfg);
    uart_cfg.enableInterrupt=1; /* Re-enabling interrupt for the remaining the tests */
    /* Get the default UART init configurations */
    UART_socSetInitCfg(uartTestInstance, &uart_cfg);

    return (ret);
}

#ifdef UART_DMA_ENABLE
#if !(defined(SOC_TPR12) || defined (SOC_AWR294X))
static uint32_t UART_getMaxNumInst(uint32_t numInst)
{
    uint32_t     i = 0U;
#if defined (SOC_AM572x) || defined (SOC_AM571x) || defined (SOC_AM574x) || defined (SOC_AM335X) || defined (SOC_AM437X)
    UART_HwAttrs uart_cfg;

    for (i = 0U; i < numInst; i++)
    {
        memset(&uart_cfg, 0, sizeof(UART_HwAttrs));
        UART_socGetInitCfg(i, &uart_cfg);

        if (0U == uart_cfg.baseAddr)
        {
            break;
        }
    }
#endif
    return (i);
}
SemaphoreP_Handle MiCbSem[UART_TEST_NUM_INSTS] = {NULL, };
UART_Handle       MiUartHandle[UART_TEST_NUM_INSTS] = {NULL, };
void UART_miCallback(UART_Handle handle, void *buf, size_t count)
{
    uint32_t i;

    for (i = 0U; i < UART_TEST_NUM_INSTS; i++)
    {
        if (MiUartHandle[i] == handle)
        {
            UART_osalPostLock(MiCbSem[i]);
            break;
        }
    }
}

/*
 *  ========== UART multiple instances test ==========
 *
 *  The test function for UART read/write on multiple instances
 *  in loopback mode
 */
#if (defined(_TMS320C6X) || ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'M') && defined(__ARM_FEATURE_SIMD32)))
#pragma DATA_ALIGN (MiRxBuf, UART_TEST_CACHE_LINE_SIZE)
char MiRxBuf[UART_TEST_NUM_INSTS][UART_TEST_CACHE_LINE_SIZE];
#pragma DATA_ALIGN (MiTxBuf, UART_TEST_CACHE_LINE_SIZE)
char MiTxBuf[UART_TEST_CACHE_LINE_SIZE];
#else
char MiRxBuf[UART_TEST_NUM_INSTS][UART_TEST_CACHE_LINE_SIZE] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE)));
char MiTxBuf[UART_TEST_CACHE_LINE_SIZE] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE)));
#endif

static bool UART_test_multiple_instances(bool dmaMode)
{
    UART_Params       uartParams;
    bool              ret = BFALSE;
    uint32_t          i, j;
    uintptr_t          bufAddr;
    uint32_t          uartTestStartInst = uartTestInstance;
    uint32_t          numUartTestInstances;
    SemaphoreP_Params semParams;

    /* enable the loopback */
    verifyLoopback = UTRUE;

    /* Get the max number of instances for testing */
    numUartTestInstances = UART_getMaxNumInst(UART_TEST_NUM_INSTS);

    for (i = 0U; i < numUartTestInstances; i++)
    {
        /* UART SoC init configuration */
        UART_initConfig(dmaMode);

        /* Create call back semaphores */
        UART_osalSemParamsInit(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        MiCbSem[i] = UART_osalCreateBlockingLock(0, &semParams);

        /* Set callback mode for read */
        UART_Params_init(&uartParams);
        uartParams.readCallback = UART_miCallback;
        uartParams.readMode = UART_MODE_CALLBACK;
        uartParams.parityType = uartParity;

        MiUartHandle[i] = UART_open(uartTestInstance, &uartParams);
        if (NULL == MiUartHandle[i])
        {
            goto Err;
        }
        uartTestInstance++;
    }

    for (i = 0U; i < numUartTestInstances; i++)
    {
        memset(MiRxBuf[i], 0, UART_TEST_CACHE_LINE_SIZE);
        if (dmaMode)
        {
            bufAddr = l2_global_address((uintptr_t)MiRxBuf[i]);
            CacheP_wbInv((void *)(uintptr_t)bufAddr, UART_TEST_CACHE_LINE_SIZE);
        }
        else
        {
            bufAddr = (uintptr_t)MiRxBuf[i];
        }

        UART_read(MiUartHandle[i], (void *)(uintptr_t)bufAddr, UART_TEST_READ_LEN);
    }

    for (i = 0U; i < numUartTestInstances; i++)
    {
        for (j = 0U; j < UART_TEST_READ_LEN; j++)
        {
            MiTxBuf[j] = fifoTrgLvlData[j] + (char)i;
        }
        if (dmaMode)
        {
            bufAddr = l2_global_address((uintptr_t)MiTxBuf);
            CacheP_wbInv((void *)(uintptr_t)bufAddr, UART_TEST_CACHE_LINE_SIZE);
        }
        else
        {
            bufAddr = (uintptr_t)MiTxBuf;
        }

        if (UART_ERROR == UART_write(MiUartHandle[i], (void *)(uintptr_t)bufAddr, UART_TEST_READ_LEN))
        {
            goto Err;
        }
    }

    /* add delay for the read semaphore calback */
    Osal_delay(100);

    for (i = 0U; i < numUartTestInstances; i++)
    {
        for (j = 0U; j < UART_TEST_READ_LEN; j++)
        {
            if ((fifoTrgLvlData[j] + (char)i) != MiRxBuf[i][j])
            {
                goto Err;
            }
        }
    }

    ret = BTRUE;

Err:
    for (i = 0U; i < numUartTestInstances; i++)
    {
        if (NULL != MiUartHandle[i])
        {
            UART_close(MiUartHandle[i]);
            MiUartHandle[i] = NULL;
        }
    }

    verifyLoopback = UFALSE;
    uartTestInstance = uartTestStartInst;
    return (ret);
}
#endif /* if SOC_TPR12 */
#endif /* if DMA */

/**
 *  @b Description
 *  @n
 *      The function is used to test the UART Driver *WRITE* functions with
 *      various invalid arguments.
 *
 *  @param[in]  handle
 *      UART Handle
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static bool UART_writeInvalidArgs(UART_Handle handle)
{
    uint8_t     buffer[4];
    int32_t     status;

    /**************************************************************************
     * Test: Write with invalid buffer
     **************************************************************************/
    status = UART_write(handle, NULL, 128);
    if (UART_SUCCESS <= status)
    {
        UART_test_log1("Error: UART Write with invalid buffer returned %d\n", status);
        return BFALSE;
    }

    /**************************************************************************
     * Test: Write with invalid size
     **************************************************************************/
    status = UART_write(handle, &buffer[0], 0);
    if (UART_SUCCESS <= status)
    {
        UART_test_log1("Error: UART Write with invalid size returned %d\n", status);
        return BFALSE;
    }

    /**************************************************************************
     * Test: Write Polling with invalid buffer
     **************************************************************************/
    status = UART_writePolling(handle, NULL, 128);
    if (UART_SUCCESS <= status)
    {
        UART_test_log1("Error: UART Write with invalid buffer returned %d\n", status);
        return BFALSE;
    }

    /**************************************************************************
     * Test: Write Polling with invalid size
     **************************************************************************/
    status = UART_writePolling(handle, &buffer[0], 0);
    if (UART_SUCCESS <= status)
    {
        UART_test_log1("Error: UART Write with invalid size returned %d\n", status);
        return BFALSE;
    }

    return BTRUE;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the UART Driver *READ* functions with
 *      various invalid arguments.
 *
 *  @param[in]  handle
 *      UART Handle
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static bool UART_readInvalidArgs(UART_Handle handle)
{
    uint8_t     buffer[4];
    int32_t     status;

    /**************************************************************************
     * Test: Read with invalid buffer
     **************************************************************************/
    status = UART_read(handle, NULL, 128);
    if (UART_EINVAL != status)
    {
        UART_test_log1("Error: UART Read with invalid buffer returned %d\n", status);
        return BFALSE;
    }

    /**************************************************************************
     * Test: Read with invalid size
     **************************************************************************/
    status = UART_read(handle, &buffer[0], 0);
    if (UART_EINVAL != status)
    {
        UART_test_log1("Error: UART Read with invalid size returned %d\n", status);
        return BFALSE;
    }

    /**************************************************************************
     * Test: Read Polling with invalid buffer
     **************************************************************************/
    status = UART_readPolling(handle, NULL, 128);
    if (UART_EINVAL != status)
    {
        UART_test_log1("Error: UART Read with invalid buffer returned %d\n", status);
        return BFALSE;
    }

    /**************************************************************************
     * Test: Read Polling with invalid size
     **************************************************************************/
    status = UART_readPolling(handle, &buffer[0], 0);
    if (UART_EINVAL != status)
    {
        UART_test_log1("Error: UART Read with invalid size returned %d\n", status);
        return BFALSE;
    }

    return BTRUE;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the exported UART API.
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
bool UART_test_api (bool dmaMode)
{
    UART_Params     params;
    UART_Handle     handle;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Setup the default UART Parameters */
    UART_Params_init(&params);

    /* Open the UART Instance */
    handle = UART_open(uartTestInstance, &params);
    if (NULL == handle)
    {
        UART_test_log("Error: Unable to open the UART Instance\n");
        return BFALSE;
    }
    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    UART_close(handle);

    /**************************************************************************
     * Test: Reopen the driver
     **************************************************************************/
    handle = UART_open(0, &params);
    if (NULL == handle)
    {
        UART_test_log("Error: Unable to open the UART Instance\n");
        return BFALSE;
    }

    /* Test: This will test the UART Write with Invalid arguments */
    if (!UART_writeInvalidArgs(handle))
        return BFALSE;

    /* Test: This will test the UART Read with Invalid arguments */
    if (!UART_readInvalidArgs(handle))
        return BFALSE;

    /* Close the driver: */
    UART_close(handle);
    return BTRUE;
}

#if (defined(SOC_TPR12) || defined (SOC_AWR294X)) && defined(SIM_BUILD)

/**
 *  @b Description
 *  @n
 *      The function is used to populate the *test* buffer with a predefined
 *      signature
 *
 *  @param[in]  ptrBuffer
 *      Buffer to be populated
 *  @param[in]  size
 *      Size of the buffer
 *
 *  @retval
 *      Not applicable
 */
void UART_populateBuffer (char* ptrBuffer, uint32_t size)
{
    uint32_t index;
    char     ch = 'A';

    /* Populate the data buffer: */
    for (index = 0U; index < size; index++)
    {
        *(ptrBuffer + index) = ch++;
        if (('Z' + 1) == ch)
            ch = 'A';
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to profile the UART Transmit API for
 *      comparison in DMA and Non-DMA mode
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
#if (defined(_TMS320C6X) || ((__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'M') && defined(__ARM_FEATURE_SIMD32)))
#pragma DATA_ALIGN (uartDataBuf, UART_TEST_CACHE_LINE_SIZE)
char uartDataBuf[0x2000];
#else
char uartDataBuf[0x2000] __attribute__ ((aligned (UART_TEST_CACHE_LINE_SIZE)));
#endif

bool UART_test_profile_tx(bool dmaMode)
{
    int32_t         status;
    UART_Params     params;
    UART_Handle     handle;
    int32_t         testIndex;
    int32_t         dataSizeIndex;
    uint32_t        dataSize[] = { 64, 256, 1024, 4096, 8192 };
    uint32_t        numDataSize = sizeof(dataSize)/sizeof(uint32_t);
    uint32_t        baudRate =  921600;
    uintptr_t       ptrTxData;
    uint32_t        txTimeTaken;
    UART_Stats      stats;
    char            testDescription[128];

#if defined(SIM_BUILD)
    numDataSize = 3U;
#endif

    CycleprofilerP_init();

    /********************************************************************************
     * Execute the test for all data sizes
     ********************************************************************************/
    for (dataSizeIndex = 0; dataSizeIndex < numDataSize; dataSizeIndex++)
    {
        /********************************************************************************
         * For each data size we will run through the following:-
         * - Test Index 0: Non-DMA + Blocking Mode
         * - Test Index 1: DMA + Blocking Mode
         * - Test Index 2: Non-DMA + Polling Mode
         * - Test Index 3: DMA + Polling Mode
         ********************************************************************************/
        for (testIndex = 0; testIndex < 4; testIndex++)
        {

#ifdef UART_DMA_ENABLE
            if(testIndex & 1)
            {
                dmaMode   = BTRUE;
                ptrTxData = l2_global_address((uintptr_t)uartDataBuf);
            }
            else
#endif
            {
                dmaMode = BFALSE;
                ptrTxData = (uintptr_t)uartDataBuf;
            }

            /* UART SoC init configuration */
            UART_initConfig(dmaMode);

            /*******************************************************************************
             * Initialization:
             *******************************************************************************/
            UART_Params_init(&params);
            params.readDataMode   = UART_DATA_BINARY;
            params.writeDataMode  = UART_DATA_BINARY;
            params.baudRate       = baudRate;
            params.readEcho       = UART_ECHO_OFF;

            /* Open the UART Instance: */
            handle = UART_open(uartTestInstance, &params);
            if (NULL == handle)
            {
                UART_test_log ("Error: Unable to open the UART Driver while profiling Transmit Data\n");
                return BFALSE;
            }

            /******************************************************************************
             * Debug Message:
             ******************************************************************************/
            switch (testIndex)
            {
                case 0:
                {
                    sprintf (testDescription, "UART %d Profile Test in Non-DMA Blocking Mode [Data Size %d bytes]",
                             uartTestInstance, dataSize[dataSizeIndex]);
                    break;
                }
                case 1:
                {
                    sprintf (testDescription, "UART %d Profile Test in DMA Blocking Mode [Data Size %d bytes]",
                             uartTestInstance, dataSize[dataSizeIndex]);
                    break;
                }
                case 2:
                {
                    sprintf (testDescription, "UART %d Profile Test in Non-DMA Polling Mode [Data Size %d bytes]",
                             uartTestInstance, dataSize[dataSizeIndex]);
                    break;
                }
                case 3:
                {
                    sprintf (testDescription, "UART %d Profile Test in DMA Polling Mode [Data Size %d bytes]",
                             uartTestInstance, dataSize[dataSizeIndex]);
                    break;
                }
                default:
                {
                    /* Control should never come here */
                    OSAL_Assert (!0);
                    return BFALSE;
                }
            }

            /* Initialize the memory to be used: */
            memset ((void *)uartDataBuf, 0, dataSize[dataSizeIndex]);

            /* Populate the transmit buffer: */
            UART_populateBuffer (uartDataBuf, dataSize[dataSizeIndex]);

            if (dmaMode)
            {
                CacheP_wbInv((void *)(uintptr_t)uartDataBuf, (int32_t)dataSize[dataSizeIndex]);
            }

            /****************************************************************
             * Profile the time taken to send out the data
             ****************************************************************/
            txTimeTaken = CycleprofilerP_getTimeStamp();

            if ((0 == testIndex) || (1 == testIndex))
                status = UART_write(handle, (void *)ptrTxData, dataSize[dataSizeIndex]);
            else
                status = UART_writePolling(handle, (void *)ptrTxData, dataSize[dataSizeIndex]);

            txTimeTaken = CycleprofilerP_getTimeStamp() - txTimeTaken;

            /* Were we able to send out the data */
            if (status != dataSize[dataSizeIndex])
            {
                UART_test_log1 ("Error: Unable to send out the data [Status %d]\n", status);
                UART_close(handle);
                return BFALSE;
            }

            /* Get the UART Statistics: */
            if (0 > UART_control (handle, UART_CMD_GET_STATS, &stats))
            {
                UART_close(handle);
                return BFALSE;
            }

            /* Debug Message: */
            UART_test_log ("Debug: Transmit Measurements\n");
            UART_test_log1 ("Debug: Baud Rate        : %d\n", baudRate);
            UART_test_log1 ("Debug: Data Size        : %d\n", dataSize[dataSizeIndex]);
            UART_test_log1 ("Debug: Transmit         : %d ticks\n", txTimeTaken);
            UART_test_log1 ("Debug: Tx Interrupt     : %d\n", stats.numTxInterrupts);
            UART_test_log1 ("Debug: Tx DMA Interrupt : %d\n", stats.numTxDMAInterrupts);

            /* Close the driver: */
            UART_close(handle);
        }
    }

    /********************************************************************************************
     * This is just a beautification; we have mucked around with the UART Console by dumping
     * all the data. So here we open a console and just put a banner to make it clean
     ********************************************************************************************/
    {
        char* ptrTestDelimitter = "\n\r*********************************************\n\r";

        UART_Params_init(&params);
        params.readDataMode   = UART_DATA_BINARY;
        params.writeDataMode  = UART_DATA_BINARY;
        params.baudRate       = baudRate;
        params.readEcho       = UART_ECHO_OFF;

        /* Open the UART Instance: */
        handle = UART_open(uartTestInstance, &params);
        if (!handle)
        {
            UART_test_log ("Error: Unable to open the UART Driver while printing the test banner\n");
            return BFALSE;
        }

        /* Write the banner on the console*/
        UART_writePolling (handle, (uint8_t*)ptrTestDelimitter, strlen(ptrTestDelimitter));
        UART_writePolling (handle, (uint8_t*)ptrTestDelimitter, strlen(ptrTestDelimitter));

        /* Close the driver: */
        UART_close(handle);
    }

    return BTRUE;
}
#endif

bool UART_test_loopback_data(bool dmaMode)
{
    bool              ret = BFALSE;
    UART_Handle       handle = NULL;
    UART_Params       params;
    SemaphoreP_Params semParams;
    int32_t           status;
    uintptr_t         pTxBuf;
    uintptr_t         pRxBuf;
    uint32_t          i;

    /* Create call back semaphore for read */
    UART_osalSemParamsInit(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    callbackSem = UART_osalCreateBlockingLock(0, &semParams);

    /* enable the loopback */
    verifyLoopback = UTRUE;

    /* UART SoC init configuration */
    UART_initConfig(dmaMode);

    /* Setup the read callback UART Parameters */
    UART_Params_init(&params);
    params.baudRate = 921600;
    params.readCallback = UART_callback;
    params.readMode = UART_MODE_CALLBACK;

    /* Open the UART Instance */
    handle = UART_open(uartTestInstance, &params);
    if (NULL == handle)
    {
        UART_test_log ("Error: Unable to open the UART Driver!\n");
        goto Err;
    }

    /* Reset the Rx buffer */
    memset((void *)scanPrompt, 0, MAX_TEST_BUFFER_SIZE);

    if (dmaMode)
    {
        pTxBuf = l2_global_address((uintptr_t)fifoTrgLvlData);
        pRxBuf = l2_global_address((uintptr_t)scanPrompt);
        CacheP_wbInv((void *)(uintptr_t)scanPrompt, MAX_TEST_BUFFER_SIZE);
    }
    else
    {
        pTxBuf = (uintptr_t)fifoTrgLvlData;
        pRxBuf = (uintptr_t)scanPrompt;
    }

    /* RX in callback mode */
    status = UART_read(handle, (void *)pRxBuf, MAX_TEST_BUFFER_SIZE);
    if (0 > status)
    {
        UART_test_log1 ("Error: UART_read() return %d!\n", status);
        goto Err;
    }

    /* Send out the data */
    status = UART_write(handle, (void *)pTxBuf, MAX_TEST_BUFFER_SIZE);
    if (0 > status)
    {
        UART_test_log1 ("Error: UART_write() return %d!\n", status);
        goto Err;
    }

    /* Wait for RX call back */
    if ((status = UART_osalPendLock(callbackSem, params.writeTimeout)) != SemaphoreP_OK)
    {
        UART_test_log1 ("Error: UART_osalPendLock() return %d!\n", status);
        goto Err;
    }

    /* Check if read data matches with write data */
    for (i = 0; i < MAX_TEST_BUFFER_SIZE; i++)
    {
        if (scanPrompt[i] != fifoTrgLvlData[i])
        {
            UART_test_log3 ("Error: UART_test_loopback_data: data fails at index %d (%c != %c)\n", i, scanPrompt[i], fifoTrgLvlData[i]);
            goto Err;
        }
    }
    ret = BTRUE;

Err:
    if (NULL != callbackSem)
    {
        UART_osalDeleteBlockingLock(callbackSem);
        callbackSem = NULL;
    }
    if (NULL != handle)
    {
        UART_close(handle);
    }

    verifyLoopback = UFALSE;
    return (ret);
}

UART_Tests Uart_tests[] =
{
#ifdef UART_DMA_ENABLE
    {UART_test_read_write, BTRUE,  UART_TEST_ID_DMA,    "\r\n UART DMA read write test in block mode"},
#endif
    {UART_test_read_write, BFALSE, UART_TEST_ID_INT,    "\r\n UART non-DMA read write test in block mode"},
#ifdef UART_DMA_ENABLE
    {UART_test_callback,   BTRUE,  UART_TEST_ID_DMA_CB, "\r\n UART DMA read write test in callback mode"},
#endif
    {UART_test_callback,   BFALSE, UART_TEST_ID_CB,     "\r\n UART non-DMA read write test in callback mode"},
#ifdef UART_DMA_ENABLE
    {UART_test_timeout,    BTRUE,  UART_TEST_ID_DMA_TO, "\r\n UART DMA timeout test, wait for 10 seconds to timeout read"},
#endif
    {UART_test_timeout,    BFALSE, UART_TEST_ID_TO,     "\r\n UART non-DMA timeout test, wait for 10 seconds to timeout read"},
#if !defined(UART_API2_NOT_SUPPORTED)
#ifdef UART_DMA_ENABLE
    {UART_test_rx_err,     BTRUE,  UART_TEST_ID_DMA_RXERR, "\r\n UART DMA RX error test, enter a break"},
#endif
    {UART_test_rx_err,     BFALSE, UART_TEST_ID_RXERR,     "\r\n UART non-DMA RX error test, enter a break"},
#endif
#ifdef UART_DMA_ENABLE
    {UART_test_read_write_cancel, BTRUE,  UART_TEST_ID_DMA_CANCEL, "\r\n UART DMA read write cancel test, enter less than 16 chars"},
#endif
    {UART_test_read_write_cancel, BFALSE, UART_TEST_ID_CANCEL,     "\r\n UART non-DMA read write cancel test, enter less than 16 chars"},
#if !defined(BAREMETAL)
#if !defined(UART_API2_NOT_SUPPORTED)
#ifdef UART_DMA_ENABLE
    {UART_test_simultaneous_rw, BTRUE,  UART_TEST_ID_DMA_RW, "\r\n UART DMA simultaneous read write test "},
#endif
    {UART_test_simultaneous_rw, BFALSE, UART_TEST_ID_RW,     "\r\n UART non-DMA simultaneous read write test "},
#endif
#endif
#if defined(UART_DMA_ENABLE)
#if !defined(UART_NO_FIFO)
    {UART_test_fifo_trglvl, BTRUE, UART_TEST_ID_DMA_TRGLVL, "\r\n UART DMA TX/RX FIFO trigger level test "},
#endif
#endif
    {UART_test_printf_scanf, BFALSE, UART_TEST_ID_PRINTF, "\r\n UART stdio printf and scanf test "},
#if !defined(UART_NO_FIFO)
    {UART_test_fifo_trglvl,  BFALSE, UART_TEST_ID_TRGLVL, "\r\n UART non-DMA TX/RX FIFO trigger level test "},
#endif
    {UART_test_polling_timeout,           BFALSE, UART_TEST_ID_POLL_TO,     "\r\n UART polling timeout test, wait for 10 seconds to timeout read"},
    {UART_test_printf_scanf_stdio_params, BFALSE, UART_TEST_ID_STDIOPARAMS, "\r\n UART stdio printf and scanf test with STDIO params(Default) "},
    {UART_test_read_write_int_disable,    BFALSE, UART_TEST_ID_INT_DISABLE, "\r\n UART read write test with interrupt disabled"},
    {UART_test_read_verify,               BFALSE, UART_TEST_ID_RDVERIFY,    "\r\n UART non-DMA read API test in loopback mode"},
#ifdef UART_DMA_ENABLE
#if !(defined(SOC_TPR12) || defined (SOC_AWR294X))
    {UART_test_multiple_instances, true, UART_TEST_ID_MULTI_INSTS, "\r\n UART DMA multiple instances loopback test "},
#endif
#endif
    {UART_test_api, BFALSE, UART_TEST_ID_API, "\r\n UART API Test"},
#if (defined(SOC_TPR12) || defined (SOC_AWR294X)) && defined(SIM_BUILD)
    {UART_test_profile_tx, false, UART_TEST_ID_PROF_TX, "\r\n UART non-DMA/DMA Blocking/Polling transmit profiling"},
#endif
#ifdef UART_DMA_ENABLE
#if !(defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_J721E) || defined(SOC_J7200)) /* disabling UT 22 till PDK-11221 errata is fixed */
    {UART_test_loopback_data, BTRUE, UART_TEST_ID_DMA_LB_DATA, "\r\n UART DMA read write test with loopback"},
#endif
#endif
    {UART_test_loopback_data, BFALSE, UART_TEST_ID_LB_DATA, "\r\n UART non-DMA read write test with loopback"},
    {NULL, }
};

void UART_test_print_test_desc(UART_Tests *test)
{

#if !defined(SIM_BUILD)
    UART_Handle uart = NULL;
    UART_Params uartParams;
    char        testIdPrompt[16] = "\r\n UART UT ";
	char        crPrompt[16] = "\r\n";
    char        testId[16] = {0, };

    /* UART SoC init configuration */
    UART_initConfig(BFALSE);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;
    uart = UART_open(uartTestInstance, &uartParams);

    /* Print unit test ID */
    sprintf(testId, "%d", test->testId);
    UART_write(uart, (void *)(uintptr_t)crPrompt, sizeof(crPrompt));
    UART_write(uart, (void *)(uintptr_t)testIdPrompt, sizeof(testIdPrompt));
    UART_write(uart, (void *)(uintptr_t)testId, sizeof(testId));
    UART_write(uart, (void *)(uintptr_t)crPrompt, sizeof(crPrompt));

    /* Print test description */
    UART_write(uart, (void *)(uintptr_t)test->testDesc, sizeof(test->testDesc));
    UART_write(uart, (void *)(uintptr_t)crPrompt, sizeof(crPrompt));

    UART_close(uart);
#else
    printf("UART UT %d: %s\n", test->testId, test->testDesc);
#endif
}

void UART_test_print_test_result(UART_Tests *test, bool pass)
{
#if !defined(SIM_BUILD)
    UART_Handle uart = NULL;
    UART_Params uartParams;
    char        testIdPrompt[16] = "\r\n UART UT ";
    char        crPrompt[16] = "\r\n";
    char        testId[16] = {0, };
    char        resultPass[16] = " PASSED";
    char        resultFail[16] = " FAILED";

    /* UART SoC init configuration */
    UART_initConfig(BFALSE);

    /* Initialize the default configuration params. */
    UART_Params_init(&uartParams);
    uartParams.parityType = uartParity;
    uart = UART_open(uartTestInstance, &uartParams);

    /* Print unit test ID */
    sprintf(testId, "%d", test->testId);
    UART_write(uart, (void *)(uintptr_t)crPrompt, strlen(crPrompt));
    UART_write(uart, (void *)(uintptr_t)testIdPrompt, strlen(testIdPrompt));
    UART_write(uart, (void *)(uintptr_t)testId, strlen(testId));
    if (BTRUE == pass)
    {
        UART_write(uart, (void *)(uintptr_t)resultPass, strlen(resultPass));
    }
    else
    {
        UART_write(uart, (void *)(uintptr_t)resultFail, strlen(resultFail));
    }

    UART_write(uart, (void *)(uintptr_t)crPrompt, strlen(crPrompt));

    UART_close(uart);
#else
    printf("UART UT %d: %s\n", test->testId, pass?"PASSED":"FAILED");
#endif
}

void UART_test_print_test_results(bool pass)
{
    const char  resultPass[32] = "\r\n All tests have passed. \r\n";
    const char  resultFail[32] = "\r\n Some tests have failed. \r\n";

#if !defined(SIM_BUILD)

    /* UART SoC init configuration */
    UART_initConfig(BFALSE);

    UART_stdioInit(uartTestInstance);

    if (BTRUE == pass)
    {
        UART_printStatus(resultPass);
    }
    else
    {
        UART_printStatus(resultFail);
    }
    UART_stdioDeInit();
#else
    printf("%s", pass?resultPass:resultFail);
#endif
}

#if !defined(BAREMETAL)
/*
 *  ======== taskFxn ========
 */
void taskFxn(void *a0, void *a1)
#else
int main(void)
#endif /* #if !defined(BAREMETAL) */
{
    bool testResult = BFALSE;
    uint32_t i;

#if defined(BAREMETAL) || defined(BUILD_MCU1_0)
    if (BFALSE == Board_initUART())
    {
#if !defined(BAREMETAL)
        return;
#else
        return (UART_ERROR);
#endif
    }
#endif

    UART_init();

    for (i = 0; ; i++)
    {
        if (NULL == Uart_tests[i].testFunc)
        {
            break;
        }
        UART_test_print_test_desc(&Uart_tests[i]);
        testResult = Uart_tests[i].testFunc(Uart_tests[i].dmaMode);
        UART_test_print_test_result(&Uart_tests[i], testResult);

        if (BFALSE == testResult)
        {
            break;
        }

    }

    UART_test_print_test_results(testResult);

    while (BTRUE)
    {
    }
}

#if !defined(BAREMETAL)
#if defined (BUILD_C7X)
#define APP_TSK_STACK_MAIN              (32U * 1024U)
#else
#define APP_TSK_STACK_MAIN              (16U * 1024U)
#endif
/* For SafeRTOS on R5F with FFI Support, task stack should be aligned to the stack size */
#if defined(SAFERTOS) && defined (BUILD_MCU) 
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(APP_TSK_STACK_MAIN)));
#else
static uint8_t  gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(64)));
#endif
/*
 *  ======== main ========
 */
Int main()
{
    TaskP_Params taskParams;

#if !defined (BUILD_MCU1_0)
    if (BFALSE == Board_initUART())
    {
        printf("\nBoard_initUART failed!\n");
        return(0);
    }
#endif

    /*  This should be called before any other OS calls (like Task creation, OS_start, etc..) */
    OS_init();

    /* Initialize the task params */
    TaskP_Params_init(&taskParams);
    /* Set the task priority higher than the default priority (1) */
    taskParams.priority     = 2;
    taskParams.stack        = gAppTskStackMain;
    taskParams.stacksize    = sizeof (gAppTskStackMain);

    TaskP_create(&taskFxn, &taskParams);
    /* Start the scheduler to start the tasks executing. */
    OS_start();
    return(0);
}
#endif /* #if !defined(BAREMETAL) */

#ifdef UART_DMA_ENABLE
#if !(defined (SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_AM64X) || defined(SOC_TPR12) || defined (SOC_AWR294X) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2))
EDMA3_RM_Handle gEdmaHandle = NULL;

/*
 * Initialize the edma driver and get the handle to the edma driver;
 */
static EDMA3_RM_Handle UartApp_edmaInit(void)
{
    EDMA3_DRV_Result edmaResult = EDMA3_DRV_E_INVALID_PARAM;
    uint32_t edma3Id;

#if defined(SOC_K2H) || defined(SOC_K2K) || defined(SOC_K2L) || defined(SOC_K2E) || defined(SOC_K2G) || defined(SOC_C6678) || defined(SOC_C6657) || defined(SOC_OMAPL137) || defined(SOC_OMAPL138) ||\
    defined(DEVICE_K2H) || defined(DEVICE_K2K) || defined(DEVICE_K2L) || defined(DEVICE_K2E) || defined(DEVICE_K2G) || defined(DEVICE_C6678) || defined(DEVICE_C6657) || defined(DEVICE_OMAPL137) || defined(DEVICE_OMAPL138)
    uint32_t edmaEvent[2], i, chnMapping, chnMappingIdx;

    /* For Keystone devices, edm3Id is UART instance and SoC specific */
    UART_getEdmaInfo(uartTestInstance, &edma3Id, edmaEvent);

    /* Set the RX/TX ownDmaChannels and dmaChannelHwEvtMap */
    for (i = 0; i < 2; i++)
    {
        chnMapping = edmaEvent[i];
        if (32 > chnMapping)
            chnMappingIdx = 0U;
        else
        {
            chnMapping -= 32;
            chnMappingIdx = 1U;
        }
        sampleInstInitConfig[edma3Id][0].ownDmaChannels[chnMappingIdx] |= (1 << chnMapping);
        sampleInstInitConfig[edma3Id][0].ownTccs[chnMappingIdx] |= (1 << chnMapping);
        sampleInstInitConfig[edma3Id][0].ownPaRAMSets[chnMappingIdx] |= (1 << chnMapping);
        sampleEdma3GblCfgParams[edma3Id].dmaChannelHwEvtMap[chnMappingIdx] |= (1 << chnMapping);
    }
#endif

    if (NULL != gEdmaHandle)
    {
        return(gEdmaHandle);
    }

#if defined (SOC_AM574x) || defined(SOC_AM572x)|| defined(SOC_AM571x) || defined (SOC_DRA72x)  || defined (SOC_DRA75x) || defined (SOC_DRA78x) || defined (SOC_AM335X) || defined (SOC_AM437X)
    edma3Id = 0;
#endif
    gEdmaHandle = (EDMA3_RM_Handle)edma3init(edma3Id, &edmaResult);
#ifdef USE_BIOS
    if (EDMA3_DRV_SOK != edmaResult)
    {
        /* Report EDMA Error */
        System_printf("\nEDMA driver initialization FAIL\n");
    }
    else
    {
        System_printf("\nEDMA driver initialization PASS.\n");
    }
#endif
    return(gEdmaHandle);
}
#endif
#if defined(SOC_TPR12) || defined (SOC_AWR294X)
EDMA_Handle gEdmaHandle = NULL;

#ifdef BUILD_MCU
#define UART_EDMA_INSTANCE_ID   EDMA_DRV_INST_MSS_A
#else
#define UART_EDMA_INSTANCE_ID   EDMA_DRV_INST_DSS_B
#endif

uint32_t UART_edmaErrCount = 0U;
uint32_t UART_edmaTCErrCount = 0U;

void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    //OSAL_Assert(BTRUE);
    UART_edmaErrCount++;
    #if 0
    printf("edmaError:isOutstandingTransferCompletionTransfersExceededLimit:%d\n",
            errorInfo->isOutstandingTransferCompletionTransfersExceededLimit);
    #endif
}

void Test_edmaTransferControllerErrorCallbackFxn
(
    EDMA_Handle                         handle,
    EDMA_transferControllerErrorInfo_t* errorInfo
)
{
    //OSAL_Assert(BTRUE);
    UART_edmaTCErrCount++;
    #if 0
    printf("edmaTransferControllerError:ReqErr:%d, RsvdMemErr:%d, BusErr:%d, errCode:%d, tcc:%d\n",
            errorInfo->isTransferRequestError,
            errorInfo->isWriteToReservedConfigMemoryMap,
            errorInfo->isBusError,
            errorInfo->busErrorInfo.errorCode,
            errorInfo->busErrorInfo.transferCompletionCode);
    #endif
}

/*
 * Initialize the edma driver and get the handle to the edma driver;
 */
static EDMA_Handle UartApp_edmaInit(void)
{
    EDMA_errorConfig_t  errorConfig;
    EDMA_instanceInfo_t instanceInfo;
    EDMA3CCInitParams 	initParam;
    uint32_t            edmaInstId = UART_EDMA_INSTANCE_ID;
    int32_t             errCode;

    if (NULL != gEdmaHandle)
    {
        return(gEdmaHandle);
    }

    if (2U == uartTestInstance)
    {
        /* UART Instance 2 uses DSS EDMA instance B */
        edmaInstId = EDMA_DRV_INST_DSS_B;
    }

    /* Initialize the UART edma instance */
    EDMA3CCInitParams_init(&initParam);
    initParam.initParamSet = UTRUE;
    errCode = EDMA_init(edmaInstId, &initParam);
    if (EDMA_NO_ERROR != errCode)
    {
        printf("Error: Unable to initialize EDMA, errorCode = %d\n", errCode);
        return NULL;
    }

    /* Open the UART edma Instance */
    gEdmaHandle = EDMA_open(edmaInstId, &errCode, &instanceInfo);
    if (NULL == gEdmaHandle)
    {
        printf("Error: Unable to open the edma Instance(%d), erorCode = %d\n",
                      UART_EDMA_INSTANCE_ID, errCode);
        return NULL;
    }

    /* Setup the EDMA Error Monitoring: */
    errorConfig.isConfigAllEventQueues              = BTRUE;
    errorConfig.isConfigAllTransferControllers      = BTRUE;
    errorConfig.isEventQueueThresholdingEnabled     = BTRUE;
    errorConfig.eventQueueThreshold                 = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = (uint8_t)BTRUE;
    errorConfig.callbackFxn                         = Test_edmaErrorCallbackFxn;
    errorConfig.transferControllerCallbackFxn       = Test_edmaTransferControllerErrorCallbackFxn;
    errCode = EDMA_configErrorMonitoring(gEdmaHandle, &errorConfig);
    if (EDMA_NO_ERROR != errCode)
    {
        printf("Debug: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errCode);
        //return NULL;
    }

    return(gEdmaHandle);
}
#endif /* SOC_TPR12 */
#endif

#if defined(BUILD_MPU) || defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif
