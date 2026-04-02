/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/**
 *  \file enet_test.h
 *
 *  \brief This file contains all the structures, macros, enums
 *  used by the CPSW test application.
 *
 */

#ifndef ENET_TEST_H_
#define ENET_TEST_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ti/csl/tistdtypes.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/per/cpsw.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils_cfg.h>
#include <ti/drv/enet/examples/utils/include/enet_appmemutils.h>
#include <ti/drv/enet/examples/utils/include/enet_apputils.h>
#include <ti/drv/enet/examples/utils/include/enet_ethutils.h>

#include <ti/osal/osal.h>
#include <ti/osal/SemaphoreP.h>
#include <ti/osal/TaskP.h>

#include "enet_test_utils_mem.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/board/board.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* UART read timeout in msec                        */
#define ENET_TEST_UART_TIMEOUT_MSEC                (10000U)

#define ENET_TEST_PRINT_BUFSIZE                    (0x4000U)

/* Application name string used in print statements */
#define APP_NAME                        "ENET_TEST"
#define ENET_TEST_MAX_TASKS             (1U)
#define ENET_INSTANCE_ID_MAX            (1U)
#define ENET_TEST_MAX_INST              (ENET_INSTANCE_ID_MAX)
#define ENET_TEST_CFG_MAX_NUM_RX_FLOWS  (4U)
#define ENET_TEST_CFG_MAX_NUM_TX_CH     (4U)
/* Default values */
#define ENET_TEST_USE_DEF               (0xFAFAU)
#define ENET_TEST_DEF_ITERATION_CNT          (1U)

/* Defined the following so that it is easy to understand a particular config */
#define DEF_HEAP_ID                     (UTILS_MEM_HEAP_ID_DDR)
#define TEST_ENABLE                     (BTRUE)
#define TEST_DISABLE                    (BFALSE)
#define PRF_ENABLE                      (BTRUE)
#define PRF_DISABLE                     (BFALSE)
#define PRINT_ENABLE                    (BTRUE)
#define PRINT_DISABLE                   (BFALSE)
#define TIMEST_ENABLE                   (BTRUE)
#define TIMEST_DISABLE                  (BFALSE)
#define DATA_CHECK_ENABLE               (BTRUE)
#define DATA_CHECK_DISABLE              (BFALSE)
#define PACING_NONE                     (0U)
#define DEF_PACING                      (20U)

#define ENET_TEST_RF_SOC_J721E          ((uint64_t)0x00000001U)
#define ENET_TEST_RF_SOC_ALL            ((uint64_t)0xFFFFFFFFU)

#define ENET_TEST_RF_CORE_MPU1_0  ((uint64_t)(((uint64_t)0x0001U) << 32U))
#define ENET_TEST_RF_CORE_MPU2_0  ((uint64_t)(((uint64_t)0x0002U) << 32U))
#define ENET_TEST_RF_CORE_MCU1_0  ((uint64_t)(((uint64_t)0x0004U) << 32U))
#define ENET_TEST_RF_CORE_MCU1_1  ((uint64_t)(((uint64_t)0x0008U) << 32U))
#define ENET_TEST_RF_CORE_MCU2_0  ((uint64_t)(((uint64_t)0x0010U) << 32U))
#define ENET_TEST_RF_CORE_MCU2_1  ((uint64_t)(((uint64_t)0x0020U) << 32U))
#define ENET_TEST_RF_CORE_MCU3_0  ((uint64_t)(((uint64_t)0x0040U) << 32U))
#define ENET_TEST_RF_CORE_MCU3_1  ((uint64_t)(((uint64_t)0x0080U) << 32U))
#define ENET_TEST_RF_CORE_C7X_1   ((uint64_t)(((uint64_t)0x0100U) << 32U))
#define ENET_TEST_RF_CORE_C66X_1  ((uint64_t)(((uint64_t)0x0200U) << 32U))
#define ENET_TEST_RF_CORE_C66X_2  ((uint64_t)(((uint64_t)0x0400U) << 32U))
#define ENET_TEST_RF_CORE_ALL     ((uint64_t)(((uint64_t)0xFFFFU) << 32U))
#define ENET_TEST_RF_CORE_MCU_ALL       (ENET_TEST_RF_CORE_MCU1_0 | \
                                         ENET_TEST_RF_CORE_MCU1_1 | \
                                         ENET_TEST_RF_CORE_MCU2_0 | \
                                         ENET_TEST_RF_CORE_MCU2_1 | \
                                         ENET_TEST_RF_CORE_MCU3_0 | \
                                         ENET_TEST_RF_CORE_MCU3_1)

/* For future when we have dynamic coverage testcases */
#define ENET_TEST_RF_CFG_ALL      ((uint64_t)(((uint64_t)0xFFFFU) << 48U))

#define ENET_TEST_VINTR_MAX             (16U)

/**
 *  \brief Testcase types.
 */
typedef enum
{
    /* Category */
    ENET_TCT_SANITY      = 0x01U,
    ENET_TCT_REGRESSION  = 0x02U,
    ENET_TCT_FULL        = 0x04U,
    /* Adequacy */
    ENET_TCT_FUNCTIONAL  = 0x08U,
    ENET_TCT_STRESS      = 0x10U,
    ENET_TCT_NEGATIVE    = 0x20U,
    ENET_TCT_PERFORMANCE = 0x40U,
    ENET_TCT_MISC        = 0x80U,
    ENET_TCT_API         = 0x100U,
    /* Used for Test parser dont use in test case */
    ENET_TCT_ALL         = 0x1FFU
} EnetTestCaseType;

/**
 *  \brief Test types.
 */
typedef enum
{
    ENET_TEST_TYPE_J721E    = 0U,

    /**< Test type: Test only J721E EVM test cases
     *   from which it can be viewed off line */
    ENET_TEST_TYPE_MAXWELL = 1U,

    /**< Test type: Test only MAXWELL EVM test cases
     *   DSS */
} EnetTestTypes;

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* Forward declaration. */
typedef struct EnetTestTxChCfgInfo_t EnetTestTxChCfgInfo;
typedef struct EnetTestRxFlowCfgInfo_t EnetTestRxFlowCfgInfo;
typedef struct EnetTestTxChObj_t EnetTestTxChObj;
typedef struct EnetTestRxFlowObj_t EnetTestRxFlowObj;
typedef struct EnetTestStateObj_t EnetTestStateObj;
typedef struct EnetTestTaskObj_t EnetTestTaskObj;
typedef struct EnetTestTaskCfg_t EnetTestTaskCfg;
typedef struct EnetTestParams_t EnetTestParams;

/** \brief Typedef for test case type function pointer */
typedef void (*EnetTest_OpenPrmsUpdateFxn)(EnetTestTaskObj *taskObj,
                                           Cpsw_Cfg *pCpswCfg,
                                           EnetUtils_Cfg *pUtilsPrms);
typedef int32_t (*EnetTest_SetTxChPrmsFxn)(EnetUdma_OpenTxChPrms *pTxChPrms,
                                           EnetTestTaskObj *taskObj,
                                           uint32_t txPSILThreadId,
                                           uint32_t txChCfgIndex);
typedef int32_t (*EnetTest_SetRxFlowPrmsFxn)(EnetUdma_OpenRxFlowPrms *pRxFlowPrms,
                                             EnetTestTaskObj *taskObj,
                                             uint32_t rxFlowCfgIdx);
typedef void (*EnetTest_SetPortLinkPrmsFxn)(EnetPer_PortLinkCfg *pLinkArgs,
                                            Enet_MacPort portNum);
typedef int32_t (*EnetTest_RunFxn)(EnetTestTaskObj *taskObj);
typedef int32_t (*EnetTest_DeInitFxn)(EnetTestTaskObj *taskObj);
typedef int32_t (*EnetTest_SetTxPktInfoFxn)(EnetTestTaskObj *taskObj,
                                            uint32_t txChIndex,
                                            uint32_t pktNum, EnetDma_Pkt *pktInfo,
                                            bool *testComplete);
typedef int32_t (*EnetTest_PreTxSendFxn)(EnetTestTaskObj *taskObj, uint32_t txChIndex);
typedef int32_t (*EnetTest_PostTxSendFxn)(EnetTestTaskObj *taskObj,
                                          uint32_t txChIndex);

typedef int32_t (*EnetTest_ProcessRxPktFxn)(EnetTestTaskObj *taskObj, uint32_t rxFlowId,
                                            uint32_t pktNum, EnetDma_Pkt *pktInfo,
                                            bool *testComplete);
typedef int32_t (*EnetTest_PreProcessRxPktFxn)(EnetTestTaskObj *taskObj,
                                               uint32_t rxCfgIndex);
typedef int32_t (*EnetTest_PostProcessRxPktFxn)(EnetTestTaskObj *taskObj,
                                                uint32_t rxCfgIndex);

struct EnetTestRxFlowObj_t
{
    EnetDma_RxChHandle hRxFlow;
    EnetDma_PktQ rxFreeQ;
    EnetDma_PktQ rxReadyQ;
    uint32_t rxFlowStartIdx;
    uint32_t rxFlowIdx;
    TaskP_Params rxTaskParams;
    TaskP_Handle hRxTask;
    SemaphoreP_Handle hRxCQSem;
    MailboxP_Handle hRxTestStatusMbx;
    int32_t rxTaskStatus;
    uint32_t receivedBitRate;
    uint32_t processCount;
    uint32_t stackUsed;
};

struct EnetTestTxChObj_t
{
    EnetDma_TxChHandle hTxCh;
    EnetDma_PktQ txFreePktInfoQ;
    TaskP_Params txTaskParams;
    TaskP_Handle hTxTask;
    SemaphoreP_Handle hTxCQSem;
    MailboxP_Handle hTxTestStatusMbx;
    int32_t txTaskStatus;
    uint32_t transmitBitRate;
    uint32_t processCount;
    uint32_t retrieveCnt;
    uint32_t stackUsed;
    uint32_t txPSILThreadId;
};

struct EnetTestStateObj_t
{
    /* CPSW driver handle */
    Enet_Handle hEnet;
    Udma_DrvHandle hUdmaDrv;
    EnetTestTxChObj txChObj[ENET_TEST_CFG_MAX_NUM_TX_CH];
    EnetTestRxFlowObj rxFlowObj[ENET_TEST_CFG_MAX_NUM_RX_FLOWS];
    uint8_t hostMacAddr[ENET_MAC_ADDR_LEN];
    void *testCtx;
    EnetTestTxChCfgInfo *txChCfgInfo[ENET_TEST_CFG_MAX_NUM_TX_CH];
    EnetTestRxFlowCfgInfo *rxFlowCfgInfo[ENET_TEST_CFG_MAX_NUM_RX_FLOWS];
    uint32_t curIteration;
    ClockP_Handle hTimer;
    TaskP_Handle task_periodicTick;
    SemaphoreP_Handle timerSem;
    EventP_Handle phyEvent;
    EventP_Handle portEvent;
    uint32_t coreId;
    uint32_t coreKey;
    uint32_t noPhyPortMask;
};

struct EnetTestTxChCfgInfo_t
{
    const uint32_t txChCfgId;

    const EnetTest_SetTxChPrmsFxn updateTxChPrms;
    /**< Tx DMA channel to use */
    const uint32_t maxTxPktLen;
    /**< Max Tx Packet Length */
    const EnetTest_SetTxPktInfoFxn setTxPktInfo;
    const EnetTest_PreTxSendFxn preTxSend;
    const EnetTest_PostTxSendFxn postTxSend;
    const uint32_t pktSendCount;
    const uint32_t pktSendLoopCount;
};

struct EnetTestRxFlowCfgInfo_t
{
    const uint32_t rxFlowCfgId;
    const EnetTest_SetRxFlowPrmsFxn updateRxFlowPrms;
    const uint32_t rxFlowMtu;
    const uint32_t pktRecvCount;
    const EnetTest_ProcessRxPktFxn processRxPkt;
    const EnetTest_PreProcessRxPktFxn preRxProcess;
    const EnetTest_PostProcessRxPktFxn postRxProcess;
    const bool useDefaultFlow;
};

/**
 *  \brief Structure used for CPSW UT task configuration parameters.
 */
struct EnetTestTaskCfg_t
{
    const uint32_t taskCfgId;
    /**< Id of the test task obj */
    const uint32_t txChCfgId[ENET_TEST_CFG_MAX_NUM_TX_CH];
    /**< Type of test case to run. */
    const uint32_t rxFlowCfgId[ENET_TEST_CFG_MAX_NUM_RX_FLOWS];
    /**< Type of test case to run. */

    const EnetTest_OpenPrmsUpdateFxn openPrmsUpdate;
    const EnetTest_SetPortLinkPrmsFxn portLinkPrmsUpdate;
    /**< Type of test case to run. */
    const EnetTest_RunFxn runTest;
    /**< Test case deinit hook function - can be used to do additional test specific cleanup . */
    const EnetTest_DeInitFxn deInitTest;
    /**< Type of test case to run. */
    const uint32_t taskId;
    /**< task Id */
    const Enet_Type enetType;
    /**< Enet Type for the test. */
    const uint32_t instId;
    /**< Enet instance Id for selected Enet Type for the test. */
    const uint32_t macPortMask;
    /**< Bitmask of Mac port number enabled for this test */
    uint32_t numTxCh;
    uint32_t numRxFlow;
};

/**
 *  \brief Structure used for CPSW UT task object parameters.
 */
struct EnetTestTaskObj_t
{
    EnetTestTaskCfg *taskCfg;
    /**< Pointer to CPSW DRV instance configuration. */
    EnetTestStateObj stateObj;
    /**< CPSW Test instance object. Holds all state info for the test */
    int32_t testResult;
    /**< test result. */
    uint32_t durationMs;
    /**< Time taken in ms */
    TaskP_Handle taskHandle;
    /**< task handle */
    SemaphoreP_Handle syncSem;
    /**< Semaphore to synch task start */
    EnetTestParams *testParams;
    /**< Reference to corresponding test parameters */
};

/**
 *  \brief Test case parameter structure.
 */
struct EnetTestParams_t
{
    const bool enableTest;
    /**< Whether test case should be executed or not. */
    const uint32_t tcId;
    /**< Test case ID. */
    const char *tcName;
    /**< Test case name. */
    const char *tcRunInfo;
    /**< Test case running instructions. */
    const char *reqMapping;
    /**< Requirement mapping. */
    const char *disableInfo;
    /**< Reason string for disabling a test case. */
    const bool printEnable;
    /**< Enable/disable print statements, used for stress testing. */
    const bool profilingEnable;
    /**< Enable Profiling prints. */
    const uint32_t tcType;
    /**< Type of testcase  - like BFT, stress etc... */
    const uint32_t dcEnable;
    /**< Enable/disable data check - used for performance. */
    const uint32_t iterationCnt;
    /**< Count for iteration, how many times this test should be run. */
    const uint32_t numTasks;
    /**< Number of tasks to test */
    const uint32_t testType[ENET_TEST_MAX_TASKS];
    /**< Type of test, Refer enum #EnetTestTypes */
    const uint32_t taskCfgId[ENET_TEST_MAX_TASKS];
    /**< Id of the test task cfg */
    SemaphoreP_Handle taskCompleteSem;
    /**< Semaphore to indicate task completion */
    EnetTestTaskObj taskObj[ENET_TEST_MAX_TASKS];

    /**< CPSW Test Task Object. This object is filled runtime.
     *   This is not a input parameter */

    /*
     * Below variables are initialized in code and not in table!!
     */
    bool isRun;
    /**< Flag to indicate whether the test case is run or not. */
    int32_t testResult;
    /**< Test result. */
};
/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*
 * Parser functions
 */
int32_t enetTestParser(void);

void EnetUT_testPrint(const char *str,
                      ...);

void udmaDrvPrint(const char *str);

/*
 * Parser functions
 */
void enetTestLogTestResult(const EnetTestParams *testParams,
                           int32_t testResult,
                           uint32_t testCaseId,
                           char *testcaseInfo);

void enetTestPrintTestResult(const EnetTestParams *testParams,
                             uint32_t skipCount,
                             uint32_t disableCount);

void enetTestResetTestResult(void);

/*
 * CPSW common functions
 */

void EnetUT_testResetTestResult(void);

void EnetUT_testCalcPerformance(EnetTestTaskObj *taskObj,
                                uint32_t durationMs);

int32_t AppUtils_getCharTimeout(char *ch,
                                uint32_t msec);

uint32_t AppUtils_getCurTimeInMsec(void);

uint32_t AppUtils_getElapsedTimeInMsec(uint32_t startTime);

/* ========================================================================== */
/*      Internal Function Declarations (Needed for other static inlines)      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ENET_TEST_H_ */
