/*
 *  Copyright (c) Texas Instruments Incorporated 2020 - 2022
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
 *  \file udma_ospi_flash_test.c
 *
 *  \brief UDMA OSPI Flash sample application performs DAC DMA mode ospi write and read 
 *  at 133MHz RCLK and 166MHz RCLK with UDMA 3D transfer using SW trigger method as below.
 * 
 *  Performs OSPI write of "appTestObj->numBytes" bytes:
 *      - Loop L times (icnt1)
 *         - SW trigger UDMA Channel -> Triggers OSPI Write of K bytes from R5 TCM to OSPI Data Buffer
 *         - wait for transfer to complete
 *         - wait for ospi flash to get ready for next write
 *  > Eack inner loop writes K (inct0) bytes of data and wait for device to be ready.
 *  > R5 TCM and OSPI Data Buffer size is L x K bytes.
 * 
 *  Where,
 *      - K is icnt0 - "UDMA_TEST_WRITE_CHUNK_SIZE"
 *      - L is icnt1 - is chunkCnt: "appTestObj->numBytes/UDMA_TEST_WRITE_CHUNK_SIZE"
 * 
 *  Performs OSPI read of "appTestObj->numBytes" bytes, UDMA_TEST_XFER_REPEAT_CNT no. of times
 *     - Loop N times:
 *          [The following performs OSPI read of icnt0 bytes]
 *          - SW trigger UDMA Channel -> Triggers OSPI Read M bytes from OSPI Data Buffer to R5 TCM 
 *          - Wait for transfer to complete
 * 
 *  > Each loop transfers M (icnt0) bytes of data
 *  > R5 TCM and OSPI Data Buffer size is M bytes.
 *  > For each loop the tranfer restarts from the origin address.
 *
 *  Where,
 *      - M is icnt0 - "appTestObj->numBytes"  
 *          This refers to the no. of bytes transferred per OSPI Read operation.
 *      - N is UDMA_TEST_XFER_REPEAT_CNT
 *         This parameters can be used to configure the the no.of times to repeat the whole transfer starting from the origin address.
 * 
 *  This read illustrates TR Reload Feature in which TR Reload count is set as 0x1FFU for perpetual loop.
 *  In this example, after UDMA_TEST_XFER_REPEAT_CNT no.of read operations, we teardown the channel to exit from the loop.
 * 
 *  Note: This application has minimal DMA setup to configure OSPI for a particular DAC read.
 *  Users need to make changes to this app if they want to test different OSPI or DMA configurations,
 *  refer to OSPI and UDMA drivers for the same.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/hw_types.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/udma/examples/udma_apputils/udma_apputils.h>
#include <ti/csl/example/ospi/ospi_flash/common/ospi_flash_common.h>
#include <ti/csl/example/ospi/ospi_flash/common/ospi_flash_phy_tune.h>
#include <ti/csl/example/ospi/ospi_flash/common/ospi_flash_patterns.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
 * Flash type
 */
#if defined (SOC_J7200) || defined (SOC_J721S2) || defined (SOC_J784S4) || defined (SOC_J742S2)
#define FLASH_TYPE_XSPI
#else
#define FLASH_TYPE_OSPI
#endif

/*
 * Application test config parameters
 */
/** \brief Maximum Number of bytes to perform OSPI Read/Write per operation (Actual number based on test) */
#define UDMA_TEST_XFER_MAX_NUM_BYTES    (1024U)
/** \brief Number of times to repeat whole data tranfer */
#define UDMA_TEST_XFER_REPEAT_CNT       (10U)
/** \brief ChunkSize in bytes for each DMA mode OSPI Flash Write operation */
#define UDMA_TEST_WRITE_CHUNK_SIZE      (16U)
/** \brief Disable CacheOps in Realtime loop */
#define UDMA_TEST_DISABLE_RT_CACHEOPS

/*
 * Application other test parameters
 */

/** \brief Total number of bytes to copy and buffer allocation */
#define UDMA_TEST_APP_MAX_TOTAL_NUM_BYTES   (UDMA_TEST_XFER_MAX_NUM_BYTES)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_BUF_SIZE_ALIGN        ((UDMA_TEST_APP_MAX_TOTAL_NUM_BYTES + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

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
 *  \brief UDMA TR packet descriptor memory.
 *  This contains the CSL_UdmapCppi5TRPD + Padding to sizeof(CSL_UdmapTR15) +
 *  one Type_15 TR (CSL_UdmapTR15) + one TR response of 4 bytes.
 *  Since CSL_UdmapCppi5TRPD is less than CSL_UdmapTR15, size is just two times
 *  CSL_UdmapTR15 for alignment.
 */
#define UDMA_TEST_APP_TRPD_SIZE         ((sizeof(CSL_UdmapTR15) * 2U) + 4U)
/** \brief This ensures every channel memory is aligned */
#define UDMA_TEST_APP_TRPD_SIZE_ALIGN   ((UDMA_TEST_APP_TRPD_SIZE + UDMA_CACHELINE_ALIGNMENT) & ~(UDMA_CACHELINE_ALIGNMENT - 1U))

/*
 * UDMA OSPI Flash test ID definitions
 */
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 16 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_16B     (0U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 16 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_16B     (1U) 
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 32 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_32B     (2U) 
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 32 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_32B     (3U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 64 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_64B     (4U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 64 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_64B     (5U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 128 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_128B    (6U) 
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 128 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_128B    (7U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 256 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_256B    (8U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 256 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_256B    (9U)   
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 512 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_512B    (10U)  
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 512 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_512B    (11U)  
/** \brief OSPI flash test at 133MHz RCLK - Read/Write 1024 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_1024B   (12U) 
/** \brief OSPI flash test at 166MHz RCLK - Read/Write 1024 Bytes */
#define UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_1024B   (13U)
/** \brief OSPI flash test at 133MHz RCLK - Write PHY tuning data (For Cypress Flash)*/
#define UDMA_OSPI_FLASH_TEST_ID_WR_TUNING            (14U)

/** \brief Get GTC Timer Ticks */
#define App_getGTCTimerTicks() (*((uint64_t *)(CSL_GTC0_GTC_CFG1_BASE + 0x8U)))

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    struct Udma_EventObj    trEventObj;

    Udma_EventHandle        trEventHandle;
    Udma_EventPrms          trEventPrms;

    /**< TR Reload Count */
    uint32_t                reloadCnt;

    uint32_t                trigger;
    /**< Global0 or Global 1 Trigger - refer \ref CSL_UdmapTrFlagsTrigger. */
    uint32_t                eventSize;
    /**< Refer \ref CSL_UdmapTrFlagsEventSize. */
    uint32_t                triggerType;
    /**< Refer \ref CSL_UdmapTrFlagsTriggerType. */
    uint32_t                eolType;
    /**< Refer \ref CSL_UdmapTrFlagsEol. */

    /**< Refer TR Address and Size Attributes */
    uint16_t                icnt[4];    
    uint16_t                dicnt[4];    
    int32_t                 dim[3];
    int32_t                 ddim[3];    
    uint8_t                *addr;    
    uint8_t                *daddr;    

    Udma_DrvHandle          drvHandle;

} App_UdmaTrObj;

typedef struct
{
    struct Udma_ChObj       chObj;
    App_UdmaTrObj           appTrObj;   

#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    struct Udma_EventObj    tdCqEventObj;
    Udma_EventHandle        tdCqEventHandle;
    Udma_EventPrms          tdCqEventPrms;
#endif 

    Udma_ChHandle           chHandle;
    Udma_DrvHandle          drvHandle;

    uint8_t                 *txRingMem;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    uint8_t                 *txCompRingMem;
    uint8_t                 *txTdCompRingMem;
#endif
    uint8_t                 *trpdMem;

} App_UdmaChObj;

typedef struct 
{
    int32_t  (*testFunc)(void);
    int32_t  testId;    
    uint32_t clk;
    uint32_t numBytes;
    char     testDesc[80];

} App_UdmaTestObj;

typedef struct
{
    /**< GTC Timer ticks at start of profiling OSPI write. */
    volatile uint64_t       txStartTicks;
    /**< GTC Timer ticks at stop of profiling OSPI write. */
    volatile uint64_t       txStopTicks;
    /**< Measured total no. of GTC Timer ticks for OSPI write. */
    volatile uint64_t       txTotalTicks;
    /**< Elapsed time in nsec for OSPI write.. */
    volatile uint64_t       txElapsedTime;

    /**< GTC Timer ticks at start of profiling OSPI read. */
    volatile uint64_t       rxStartTicks[UDMA_TEST_XFER_REPEAT_CNT];
    /**< GTC Timer ticks at stop of profiling OSPI read. */
    volatile uint64_t       rxStopTicks[UDMA_TEST_XFER_REPEAT_CNT];
    /**< Measured total no. of GTC Timer ticks for OSPI read. */
    volatile uint64_t       rxTotalTicks[UDMA_TEST_XFER_REPEAT_CNT + 1U];
    /**< Elapsed time in nsec for OSPI read. */
    volatile uint64_t       rxElapsedTime[UDMA_TEST_XFER_REPEAT_CNT + 1U];

} App_UdmaCounterObj;

typedef struct
{
    struct Udma_DrvObj      drvObj;
    App_UdmaChObj           appChObj;
    App_UdmaTestObj         appTestObj;
    App_UdmaCounterObj      appCounterObj;    
    uint32_t                totalNumBytes;
} App_UdmaObj;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static int32_t Udma_ospiFlashTestRun(void);

static int32_t App_ospiFlashTest(App_UdmaObj *appObj, App_OspiObj *ospiObj);
static int32_t App_udmaOspiFlash(App_UdmaObj *appObj, App_OspiObj *ospiObj);
static int32_t App_udmaOspiFlashWrite(App_UdmaObj *appObj, App_OspiObj *ospiObj) __attribute__((section(".udma_critical_fxns")));
static int32_t App_udmaOspiFlashRead(App_UdmaObj *appObj) __attribute__((section(".udma_critical_fxns")));

#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static void App_udmaEventTdCb(Udma_EventHandle eventHandle,
                              uint32_t eventType,
                              void *appData);
#endif

static int32_t App_init(App_UdmaObj *appObj, App_OspiObj *ospiObj);
static int32_t App_deinit(App_UdmaObj *appObj, App_OspiObj *ospiObj);

static int32_t App_create(App_UdmaObj *appObj);
static int32_t App_delete(App_UdmaObj *appObj);

static void App_udmaTrpdInit(App_UdmaTrObj *appTrObj, App_UdmaChObj  *appChObj);
#if defined(FLASH_TYPE_OSPI)
static int32_t App_udmaTrpdSanityCheck(App_UdmaChObj *appChObj, uint64_t pDesc);
static inline void App_udmaTrObjInitWrite(App_UdmaTestObj *appTestObj, App_UdmaTrObj *appTrObj);
#endif
static inline void App_udmaTrObjInitRead(App_UdmaTestObj *appTestObj, App_UdmaTrObj *appTrObj);

static void App_printPerfResults(App_UdmaObj *appObj);

void App_print(const char *str);
static void App_printNum(const char *str, uint32_t num);
int32_t App_setGTCClk(uint32_t moduleId,
                      uint32_t clkId,
                      uint64_t clkRateHz);


static int32_t App_ospiFlashInit(App_OspiObj *ospiObj, uint32_t clk);
void App_ospiFlashConfigDacMode( App_OspiObj *ospiObj, bool dacMode);
#if defined(FLASH_TYPE_OSPI)
static int32_t App_ospiFlashStart(uint32_t numBytes) __attribute__((section(".udma_critical_fxns")));
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*
 * UDMA driver and channel objects
 */
App_UdmaObj gUdmaAppObj;
App_OspiObj gOspiAppObj;

/*
 * UDMA Memories
 */
static uint8_t gTxRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static uint8_t gTxCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
static uint8_t gTxTdCompRingMem[UDMA_TEST_APP_RING_MEM_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
#endif
static uint8_t gUdmaTrpdMem[UDMA_TEST_APP_TRPD_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT)));

/*
 * Application Buffers
 */
static uint8_t gUdmaTestTxBuf[UDMA_TEST_APP_BUF_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
static uint8_t gUdmaTestRxBuf[UDMA_TEST_APP_BUF_SIZE_ALIGN] __attribute__((aligned(UDMA_CACHELINE_ALIGNMENT), section(".udma_buffer_r5_tcm")));
static uint8_t * gUdmaTestOspiFlashDataAddr = (uint8_t *)(OSPI_FLASH_DATA_BASE_ADDR);

/* Global test pass/fail flag */
static volatile int32_t gUdmaTestResult = UDMA_SOK;
/* Global App pass/fail flag */
static volatile int32_t gUdmaAppResult = UDMA_SOK;

/* UDMA OSPI Flash Tests data structure */
App_UdmaTestObj gUdmaAppTestObj[] =
{
    /* testFunc, testID, clk, numBytes, testDesc */
#if defined(FLASH_TYPE_XSPI)
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_WR_TUNING,         OSPI_MODULE_CLK_133M, OSPI_FLASH_ATTACK_VECTOR_SIZE, "\r\n OSPI flash test at 133MHz RCLK - Write PHY tuning data"},
#endif
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_16B,  OSPI_MODULE_CLK_133M, 16U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 16 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_16B,  OSPI_MODULE_CLK_166M, 16U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 16 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_32B,  OSPI_MODULE_CLK_133M, 32U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 32 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_32B,  OSPI_MODULE_CLK_166M, 32U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 32 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_64B,  OSPI_MODULE_CLK_133M, 64U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 64 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_64B,  OSPI_MODULE_CLK_166M, 64U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 64 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_128B,  OSPI_MODULE_CLK_133M, 128U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 128 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_128B,  OSPI_MODULE_CLK_166M, 128U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 128 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_256B,  OSPI_MODULE_CLK_133M, 256U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 256 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_256B,  OSPI_MODULE_CLK_166M, 256U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 256 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_512B,  OSPI_MODULE_CLK_133M, 512U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 512 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_512B,  OSPI_MODULE_CLK_166M, 512U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 512 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_133M_1024B,  OSPI_MODULE_CLK_133M, 1024U, "\r\n OSPI flash test at 133MHz RCLK - Read/Write 1024 Bytes"},
    {Udma_ospiFlashTestRun, UDMA_OSPI_FLASH_TEST_ID_DAC_DMA_166M_1024B,  OSPI_MODULE_CLK_166M, 1024U, "\r\n OSPI flash test at 166MHz RCLK - Read/Write 1024 Bytes"},
    {NULL, }
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/*
 * UDMA OSPI Flash test
 */
int32_t Udma_ospiFlashTest(void)
{
    int32_t          retVal;
    uint32_t         i;
    App_UdmaTestObj *test;
    App_UdmaObj     *appObj = &gUdmaAppObj;

    for (i = 0; ; i++)
    {
        test = &gUdmaAppTestObj[i];
        if (test->testFunc == NULL)
        {
            break;
        }

        appObj->appTestObj = *test;
        appObj->totalNumBytes = test->numBytes;
        retVal = test->testFunc();
        if((UDMA_SOK == retVal) && (UDMA_SOK == gUdmaTestResult))
        {
            App_print(test->testDesc);
            App_print(" have passed\r\n");
        }
        else
        {
            App_print(test->testDesc);
            App_print(" have failed\r\n");
            gUdmaTestResult = UDMA_SOK;
            gUdmaAppResult = UDMA_EFAIL;
        }
    }

    #if defined LDRA_DYN_COVERAGE_EXIT
    App_print("\nLDRA ENTRY: UDMA OSPI FLASH!!\n");
    upload_execution_history();
    App_print("\nLDRA EXIT: UDMA OSPI FLASH!!\n");
    #endif
    
    if(UDMA_SOK != gUdmaAppResult)
    {
        App_print("\n Some tests have failed. \n");
    }
    else
    {
        App_print("\n All tests have passed. \n");
    }

    App_print("\n Done\n");

    return (0);
}

/*
 * UDMA OSPI Flash test run
 */
static int32_t Udma_ospiFlashTestRun(void)
{
    int32_t         retVal;
    App_UdmaObj    *appObj = &gUdmaAppObj;
    App_OspiObj    *ospiObj = &gOspiAppObj;

    retVal = App_init(appObj, ospiObj);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA App init failed!!\n");
    }

    App_print("UDMA OSPI Flash application started...\n");

    if(UDMA_SOK == retVal)
    {
        retVal = App_create(appObj);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA App create failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        retVal = App_ospiFlashTest(appObj, ospiObj);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA App OSPI Flash test failed!!\n");
        }
    }

    retVal += App_delete(appObj);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA App delete failed!!\n");
    }

    retVal += App_deinit(appObj, ospiObj);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA App deinit failed!!\n");
    }

    return (retVal);
}

static int32_t App_ospiFlashTest(App_UdmaObj *appObj, App_OspiObj *ospiObj)
{
    int32_t         retVal = UDMA_SOK;
    uint32_t        i;
    uint8_t        *rxBuf;

    /* Reset RX buffer */
    rxBuf  = &gUdmaTestRxBuf[0U];
    for(i = 0U; i < appObj->totalNumBytes; i++)
    {
        rxBuf[i] = 0U;
    }
    /* Writeback RX buffer */
    Udma_appUtilsCacheWb(rxBuf, appObj->totalNumBytes);

    /* Perform UDMA memcpy */
    retVal = App_udmaOspiFlash(appObj, ospiObj);
    if(UDMA_SOK == retVal)
    {
        /* Print performance results for OSPI Flash in DAC DMA mode */
        App_printPerfResults(appObj);
    }

    return (retVal);
}

static int32_t App_udmaOspiFlash(App_UdmaObj *appObj, App_OspiObj *ospiObj)
{
    int32_t         retVal = UDMA_SOK;
    App_UdmaTestObj *appTestObj = &appObj->appTestObj;
    uint32_t        i;
    uint8_t        *rxBuf, *txBuf;

#if defined(FLASH_TYPE_OSPI)
    retVal = App_ospiFlashStart(appObj->totalNumBytes);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] OSPI Start failed!!\n");
    }
#endif

    if(UDMA_SOK == retVal)
    {
        retVal = App_udmaOspiFlashWrite(appObj, ospiObj);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error]UDMA OSPI Write failed!!\n");
        }
    #if defined(FLASH_TYPE_XSPI)
        /* For XSPI Flash App_ospiFlashInit configures in INDAC mode for Write.
         * Now switching to DAC mode to perform DAC DMA read.
         */
        App_ospiFlashConfigDacMode(ospiObj, BTRUE);
    #endif
    }

    if(UDMA_SOK == retVal)
    {
        retVal = App_udmaOspiFlashRead(appObj);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error]UDMA OSPI Read failed!!\n");
        }
    }

    if(appTestObj->testId != UDMA_OSPI_FLASH_TEST_ID_WR_TUNING)
    {
        if(UDMA_SOK == retVal)
        {
            rxBuf  = &gUdmaTestRxBuf[0U];
            txBuf  = &gUdmaTestTxBuf[0U];

#ifdef UDMA_TEST_DISABLE_RT_CACHEOPS
            /* Invalidate destination buffer */
            Udma_appUtilsCacheInv(rxBuf, appObj->totalNumBytes);
#endif
            /* Compare data */
            for(i = 0U; i < appObj->totalNumBytes; i++)
            {
                if(rxBuf[i] != txBuf[i])
                {
                    App_printNum("\n [Error] Data mismatch at idx %d", i);
                    retVal = UDMA_EFAIL;
                    break;
                }
            }
        }
    }

    return (retVal);
}

/* Micron OSPI Flash supports writes in DAC Mode */
#if defined(FLASH_TYPE_OSPI)
static int32_t App_udmaOspiFlashWrite(App_UdmaObj *appObj, App_OspiObj *ospiObj)
{
    int32_t              retVal = UDMA_SOK;
    uint64_t             pDesc = 0;
    uint32_t             i;
    uint32_t             chunkCnt, cCnt;
    /* Use local variables in real-time loop for optimized performance */
    uint32_t             txStartTicks, txStopTicks;
    volatile uint32_t   *pSwTriggerReg;
    uint32_t             triggerMask;
    App_UdmaTestObj     *appTestObj       = &appObj->appTestObj;
    App_UdmaChObj       *appChObj         = &appObj->appChObj;
    App_UdmaTrObj       *appTrObj         = &appChObj->appTrObj;
    App_UdmaCounterObj  *appCounterObj    = &appObj->appCounterObj;
    Udma_ChHandle        chHandle         = appChObj->chHandle;
    uint8_t             *txBuf            = &gUdmaTestTxBuf[0U];
    const uint16_t       size             = appTestObj->numBytes;
    const uint16_t       totalSize        = appObj->totalNumBytes;
    volatile uint64_t   *pintrStatusReg   = appTrObj->trEventPrms.intrStatusReg;
    uint64_t             intrMask         = appTrObj->trEventPrms.intrMask;
    volatile uint64_t   *intrClearReg     = appTrObj->trEventPrms.intrClearReg;
    uint8_t             *trpdMem          = appChObj->trpdMem;

    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    /* Init TX buffers */
    for(i = 0U; i < totalSize; i++)
    {
        txBuf[i] = i;
    }
    
    App_udmaTrObjInitWrite(appTestObj, appTrObj);

    /* Get SW trigger register for easy access */
    triggerMask = ((uint32_t)1U << (appTrObj->trigger - 1U));
    pSwTriggerReg = (volatile uint32_t *) Udma_chGetSwTriggerRegister(chHandle);
    if(NULL == pSwTriggerReg)
    {
        App_print("\n [Error] Channel trigger register get failed!!\n");
    }

    /* Submit TRPD to channels */
    App_udmaTrpdInit(appTrObj, appChObj);
    retVal = Udma_ringQueueRaw(
                Udma_chGetFqRingHandle(chHandle),
                (uint64_t) Udma_appVirtToPhyFxn(trpdMem, 0U, NULL));
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] Channel queue failed!!\n");
    }

    if(UDMA_SOK == retVal)
    {

        /*****************************************************************************
         * OSPI Write "appTestObj->numBytes" (= appTrObj->icnt[0]* appTrObj->icnt[1] )
         *****************************************************************************/
        txStartTicks = App_getGTCTimerTicks();

        /* Do Cache write-back for "appTestObj->numBytes" chunk to be tranferred */
        CSL_armR5CacheWb(txBuf, size, BTRUE);
        
        /* Set number of times to trigger TX transfer */
        chunkCnt = appTrObj->icnt[1];
        for(cCnt = 0U; cCnt < chunkCnt; cCnt++)
        {
            /* Write UDMA_TEST_WRITE_CHUNK_SIZE(= appTrObj->icnt[0]) bytes and wait for device to be ready */

            /* Set channel trigger and wait for completion */
            CSL_REG32_WR(pSwTriggerReg, triggerMask);

            /* Wait for the transfer to complete in polling mode */
            while(1U)
            {
                volatile uint64_t   intrStatusReg;
                intrStatusReg = CSL_REG64_RD(pintrStatusReg);
                /* Check whether the interrupt status Reg is set - which indicates the
                * tranfser completion of appTestObj->numBytes */
                if(intrStatusReg & intrMask)
                {
                    /* Clear interrupt */
                    CSL_REG64_WR(intrClearReg, intrMask);
                    break;
                }
            }

            /* Wait device to be ready after write operation */
            uint32_t timeOutVal = OSPI_FLASH_WRITE_TIMEOUT;
            uint32_t retry = OSPI_FLASH_WRITE_TIMEOUT;
            volatile uint32_t delay = OSPI_FLASH_CHECK_IDLE_DELAY;
            uint8_t  status = 0xFF;
            uint32_t regVal;
            while (0U != timeOutVal)
            {
                (void)CSL_ospiCmdRead(baseAddr, OSPI_FLASH_CMD_RDSR, 1U);
                while(!CSL_ospiIsIdle(baseAddr));
                CSL_ospiFlashExecCmd(baseAddr);
                while(0U != retry)
                {
                    if(UTRUE == CSL_ospiFlashExecCmdComplete(baseAddr))
                    {
                        break;
                    }
                    while (0U < delay)
                    {  
                        delay = delay - 1U;
                    }
                    retry--;
                }
                while(!CSL_ospiIsIdle(baseAddr));
                regVal = CSL_REG32_RD(&baseAddr->FLASH_RD_DATA_LOWER_REG);
                (void)memcpy((void *)&status, (void *)(&regVal), 1U);
                if (0U == (status & 1U))
                {
                    break;
                }
                timeOutVal--;
                delay = OSPI_FLASH_CHECK_IDLE_DELAY;
                while (0U < delay)
                {  
                    delay = delay - 1U;
                }
            }
        } 
        txStopTicks = App_getGTCTimerTicks();

        appCounterObj->txStartTicks = txStartTicks;
        appCounterObj->txStopTicks = txStopTicks;
        
    }
    if(UDMA_SOK == retVal)
    {
        /* wait for response to be received in completion queue */
        while(1)
        {
            retVal =
                Udma_ringDequeueRaw(Udma_chGetCqRingHandle(chHandle), &pDesc);
            if(UDMA_SOK == retVal)
            {
                break;
            }
        }

        /* Sanity check - Check returned descriptor pointer & TR response status*/
        retVal = App_udmaTrpdSanityCheck(appChObj, pDesc);
    }

    return (retVal);
}
#else
/* Cypress XSPI Flash does not support writes in DAC mode */
static int32_t App_udmaOspiFlashWrite(App_UdmaObj *appObj, App_OspiObj *ospiObj)
{
    int32_t              retVal = UDMA_SOK;
    uint32_t             i;
    /* Use local variables in real-time loop for optimized performance */
    uint32_t             txStartTicks, txStopTicks;
    App_UdmaCounterObj  *appCounterObj    = &appObj->appCounterObj;
    App_UdmaTestObj     *appTestObj       = &appObj->appTestObj;
    uint8_t             *txBuf            = &gUdmaTestTxBuf[0U];
    uint16_t             totalSize        = appObj->totalNumBytes;
    uint32_t             offset;

    /* Init TX buffers */
    if(UDMA_OSPI_FLASH_TEST_ID_WR_TUNING == appTestObj->testId)
    {
        for(i = 0U; i < totalSize; i++)
        {
            txBuf[i] = ospi_flash_attack_vector[i];
        }
        offset = OSPI_FLASH_TUNING_DATA_OFFSET;
    }
    else
    {
        for(i = 0U; i < totalSize; i++)
        {
            txBuf[i] = i;
        }
        offset = 0;
    }

    /* Capture the time at the beginning of operation */
    txStartTicks = App_getGTCTimerTicks();

    /* Call the API to write in INDAC mode */
    OspiFlash_xspiIndacWrite(ospiObj, txBuf, totalSize, BFALSE, offset);

    /* Capture the time at the end of operation */
    txStopTicks = App_getGTCTimerTicks();

    /* Save the captured time to the app object */
    appCounterObj->txStartTicks = txStartTicks;
    appCounterObj->txStopTicks = txStopTicks;

    return (retVal);
}
#endif

static int32_t App_udmaOspiFlashRead(App_UdmaObj *appObj)
{
    int32_t              retVal = UDMA_SOK;
    uint32_t             triggerCnt, tCnt;
    /* Use local variables in real-time loop for optimized performance */
    volatile uint32_t   *pSwTriggerReg;
    uint32_t             triggerMask;
    uint32_t             rxStartTicks, rxStopTicks;
    App_UdmaTestObj     *appTestObj       = &appObj->appTestObj;
    App_UdmaChObj       *appChObj         = &appObj->appChObj;
    App_UdmaTrObj       *appTrObj         = &appChObj->appTrObj;
    App_UdmaCounterObj  *appCounterObj    = &appObj->appCounterObj;
    Udma_ChHandle        chHandle         = appChObj->chHandle;
#ifndef UDMA_TEST_DISABLE_RT_CACHEOPS
    uint8_t             *rxBuf            = &gUdmaTestRxBuf[0U];
    const uint16_t       size             = appTestObj->numBytes;
#endif
    volatile uint64_t   *pintrStatusReg   = appTrObj->trEventPrms.intrStatusReg;
    uint64_t             intrMask         = appTrObj->trEventPrms.intrMask;
    volatile uint64_t   *intrClearReg     = appTrObj->trEventPrms.intrClearReg;
    uint8_t             *trpdMem          = appChObj->trpdMem;

    if(UDMA_OSPI_FLASH_TEST_ID_WR_TUNING != appTestObj->testId)
    {
    #if defined(FLASH_TYPE_XSPI)
        OspiFlash_spiPhyTune(BTRUE, OSPI_FLASH_TUNING_DATA_OFFSET, appTestObj->clk);
    #endif

        App_udmaTrObjInitRead(appTestObj, appTrObj);

        /* Get SW trigger register for easy access */
        triggerMask = ((uint32_t)1U << (appTrObj->trigger - 1U));
        pSwTriggerReg = (volatile uint32_t *) Udma_chGetSwTriggerRegister(chHandle);
        if(NULL == pSwTriggerReg)
        {
            App_print("\n [Error] Channel trigger register get failed!!\n");
        }

        /* Submit TRPD to channels */
        App_udmaTrpdInit(appTrObj, appChObj);
        retVal = Udma_ringQueueRaw(
                    Udma_chGetFqRingHandle(chHandle),
                    (uint64_t) Udma_appVirtToPhyFxn(trpdMem, 0U, NULL));
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] Channel queue failed!!\n");
        }

        if(UDMA_SOK == retVal)
        {
            /* Set number of times to trigger RX transfer */
            triggerCnt = UDMA_TEST_XFER_REPEAT_CNT;
            for(tCnt = 0U; tCnt < triggerCnt; tCnt++)
            {      

                /********************************************************
                 * OSPI Read "appTestObj->numBytes" (= appTrObj->icnt[0])
                 ********************************************************/

                rxStartTicks = App_getGTCTimerTicks();

                /* Set channel trigger and wait for completion */
                CSL_REG32_WR(pSwTriggerReg, triggerMask);

                /* Wait for the transfer to complete in polling mode */
                while(1U)
                {
                    volatile uint64_t   intrStatusReg;
                    intrStatusReg = CSL_REG64_RD(pintrStatusReg);
                    /* Check whether the interrupt status Reg is set - which indicates the
                    * transfer completion of appTestObj->numBytes */
                    if(intrStatusReg & intrMask)
                    {
                        /* Clear interrupt */
                        CSL_REG64_WR(intrClearReg, intrMask);
                        break;
                    }
                }
#ifndef UDMA_TEST_DISABLE_RT_CACHEOPS
                /* Do Cache invalidate for the received chunk */
                CSL_armR5CacheInv(rxBuf, size, BTRUE);
#endif

                rxStopTicks = App_getGTCTimerTicks();

                appCounterObj->rxStartTicks[tCnt] = rxStartTicks;
                appCounterObj->rxStopTicks[tCnt] = rxStopTicks;
            }
        }
        /* Since TR Reload Count Set for perpetual loop, TRPD never completes and comes back to CQ.
        * To exit, teardown the channel using Udma_chDisable */
        retVal = Udma_chDisable(chHandle, UDMA_DEFAULT_CH_DISABLE_TIMEOUT);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA channel disable failed!!\n");
        }
        /* During channel forced teardown to break from the TR Reload Perpetual loop,
         * DMA will complete the already reloaded TR. This results in setting the 
         * interrupt status register after this transfer completion.
         * Hence clear the interrupt */
        CSL_REG64_WR(intrClearReg, intrMask);
    }
    return (retVal);
}

#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
static void App_udmaEventTdCb(Udma_EventHandle eventHandle,
                              uint32_t eventType,
                              void *appData)
{
    int32_t             retVal;
    CSL_UdmapTdResponse tdResp;
    App_UdmaChObj      *appChObj = (App_UdmaChObj *) appData;

    if(appChObj != NULL)
    {
        if(UDMA_EVENT_TYPE_TEARDOWN_PACKET == eventType)
        {
            /* Response received in Teardown completion queue */
            retVal = Udma_chDequeueTdResponse(appChObj->chHandle, &tdResp);
            if(UDMA_SOK != retVal)
            {
                /* [Error] No TD response after callback!! */
                gUdmaTestResult = UDMA_EFAIL;
            }
        }
        else
        {
            gUdmaTestResult = UDMA_EFAIL;
        }
    }
    else
    {
        gUdmaTestResult = UDMA_EFAIL;
    }

    return;
}
#endif

static int32_t App_init(App_UdmaObj *appObj, App_OspiObj *ospiObj)
{
    int32_t              retVal;
    Udma_InitPrms        initPrms;
    uint32_t             instId;
    App_UdmaChObj       *appChObj        = &appObj->appChObj;
    App_UdmaTrObj       *appTrObj       = &appChObj->appTrObj;
    App_UdmaTestObj     *appTestObj     = &appObj->appTestObj;
    App_UdmaCounterObj  *appCounterObj  = &appObj->appCounterObj;
    Udma_DrvHandle       drvHandle      = &appObj->drvObj;

#if defined (SOC_AM64X)
    /* Use Block Copy DMA for AM64x */
    instId = UDMA_INST_ID_BCDMA_0;
#else
    /* Use MCU NAVSS for MCU domain cores. Rest all cores uses Main NAVSS */
#if defined (BUILD_MCU1_0) || defined (BUILD_MCU1_1)
    instId = UDMA_INST_ID_MCU_0;
#else
    instId = UDMA_INST_ID_MAIN_0;
#endif
#endif
    /* UDMA driver init */
    UdmaInitPrms_init(instId, &initPrms);
    initPrms.virtToPhyFxn   = &Udma_appVirtToPhyFxn;
    initPrms.phyToVirtFxn   = &Udma_appPhyToVirtFxn;
    initPrms.printFxn       = &App_print;
    retVal = Udma_init(drvHandle, &initPrms);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA init failed!!\n");
    }

    /* Init channel parameters */
    appChObj->chHandle          = &appChObj->chObj;
    appChObj->drvHandle         = drvHandle;
    appChObj->txRingMem         = &gTxRingMem[0U];
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    appChObj->tdCqEventHandle   = NULL;
    appChObj->txCompRingMem     = &gTxCompRingMem[0U];
    appChObj->txTdCompRingMem   = &gTxTdCompRingMem[0U];
#endif
    appChObj->trpdMem           = &gUdmaTrpdMem[0U];
    
    /* Init TR parameters */
    appTrObj->trEventHandle      = NULL;
    appTrObj->drvHandle          = drvHandle;

    memset((void *)&appCounterObj->txStartTicks,  0, sizeof(appCounterObj->txStartTicks));
    memset((void *)&appCounterObj->txStopTicks,   0, sizeof(appCounterObj->txStopTicks));
    memset((void *)&appCounterObj->txTotalTicks,  0, sizeof(appCounterObj->txTotalTicks));
    memset((void *)&appCounterObj->txElapsedTime, 0, sizeof(appCounterObj->txElapsedTime));
    memset((void *)&appCounterObj->rxStartTicks,  0, sizeof(appCounterObj->rxStartTicks));
    memset((void *)&appCounterObj->rxStopTicks,   0, sizeof(appCounterObj->rxStopTicks));
    memset((void *)&appCounterObj->rxTotalTicks,  0, sizeof(appCounterObj->rxTotalTicks));
    memset((void *)&appCounterObj->rxElapsedTime, 0, sizeof(appCounterObj->rxElapsedTime));

    if(UDMA_SOK == retVal)
    {
        retVal += App_ospiFlashInit(ospiObj, appTestObj->clk);
    }

    return (retVal);
}


static int32_t App_deinit(App_UdmaObj *appObj, App_OspiObj *ospiObj)
{
    int32_t         retVal;
    Udma_DrvHandle  drvHandle = &appObj->drvObj;

    OspiFlash_ospiClose(ospiObj, BFALSE);

    retVal = Udma_deinit(drvHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA deinit failed!!\n");
    }

    return (retVal);
}
static int32_t App_create(App_UdmaObj *appObj)
{
    int32_t             retVal = UDMA_SOK;
    uint32_t            chType;
    Udma_ChPrms         chPrms;
    Udma_ChTxPrms       txPrms;
    Udma_ChRxPrms       rxPrms;
    Udma_EventHandle    eventHandle;
    App_UdmaChObj      *appChObj = &appObj->appChObj;
    App_UdmaTrObj      *appTrObj = &appChObj->appTrObj;
    Udma_ChHandle       chHandle = appChObj->chHandle;
    Udma_DrvHandle      drvHandle = &appObj->drvObj;

    if(UDMA_SOK == retVal)
    {
        /* Init channel parameters */
        chType = UDMA_CH_TYPE_TR_BLK_COPY;
        UdmaChPrms_init(&chPrms, chType);
        chPrms.fqRingPrms.ringMem       = appChObj->txRingMem;
        chPrms.fqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.fqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
        chPrms.cqRingPrms.ringMem       = appChObj->txCompRingMem;
        chPrms.cqRingPrms.ringMemSize   = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.cqRingPrms.elemCnt       = UDMA_TEST_APP_RING_ENTRIES;
        chPrms.tdCqRingPrms.ringMem     = appChObj->txTdCompRingMem;
        chPrms.tdCqRingPrms.ringMemSize = UDMA_TEST_APP_RING_MEM_SIZE;
        chPrms.tdCqRingPrms.elemCnt     = UDMA_TEST_APP_RING_ENTRIES;
#endif

        /* Open channel for block copy */
        retVal = Udma_chOpen(drvHandle, chHandle, chType, &chPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA channel open failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Config TX channel */
        UdmaChTxPrms_init(&txPrms, chType);
        retVal = Udma_chConfigTx(chHandle, &txPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA TX channel config failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Config RX channel - which is implicitly paired to TX channel in
            * block copy mode */
        UdmaChRxPrms_init(&rxPrms, chType);
        retVal = Udma_chConfigRx(chHandle, &rxPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA RX channel config failed!!\n");
        }
    }

    if(UDMA_SOK == retVal)
    {
        /* Register TR event */
        eventHandle = &appTrObj->trEventObj;
        UdmaEventPrms_init(&appTrObj->trEventPrms);
        appTrObj->trEventPrms.eventType         = UDMA_EVENT_TYPE_TR;
        appTrObj->trEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        appTrObj->trEventPrms.chHandle          = chHandle;
       /* For polling mode we can't use the existing master event as that is meant only for interrupt event - 
        *  we can't mix interrupt and poll mode in same master handle. Set the parameter to NULL 
        *  so that the driver creates a new master event. */
        appTrObj->trEventPrms.masterEventHandle = NULL;
        appTrObj->trEventPrms.eventCb           = NULL;
        appTrObj->trEventPrms.appData           = appChObj;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &appTrObj->trEventPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA TR event register failed!!\n");
        }
        else
        {
            appTrObj->trEventHandle = eventHandle;
        }
    }

#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    if(UDMA_SOK == retVal)
    {
        /* Register teardown ring completion callback */
        eventHandle = &appChObj->tdCqEventObj;
        UdmaEventPrms_init(&appChObj->tdCqEventPrms);
        appChObj->tdCqEventPrms.eventType         = UDMA_EVENT_TYPE_TEARDOWN_PACKET;
        appChObj->tdCqEventPrms.eventMode         = UDMA_EVENT_MODE_SHARED;
        appChObj->tdCqEventPrms.chHandle          = chHandle;
        appChObj->tdCqEventPrms.masterEventHandle = Udma_eventGetGlobalHandle(drvHandle);
        appChObj->tdCqEventPrms.eventCb           = &App_udmaEventTdCb;
        appChObj->tdCqEventPrms.appData           = appChObj;
        retVal = Udma_eventRegister(drvHandle, eventHandle, &appChObj->tdCqEventPrms);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA Teardown CQ event register failed!!\n");
        }
        else
        {
            appChObj->tdCqEventHandle = eventHandle;
        }
    }
#endif 

    if(UDMA_SOK == retVal)
    {
        /* Channel enable */
        retVal = Udma_chEnable(chHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA channel enable failed!!\n");
        }
    }

    return (retVal);
}

static int32_t App_delete(App_UdmaObj *appObj)
{
    int32_t         retVal = UDMA_SOK, tempRetVal;
    uint64_t        pDesc;
    App_UdmaChObj  *appChObj = &appObj->appChObj;
    App_UdmaTrObj  *appTrObj = &appChObj->appTrObj;
    Udma_ChHandle   chHandle = appChObj->chHandle;
    

    /* Flush any pending request from the free queue */
    while(1)
    {
        tempRetVal = Udma_ringFlushRaw(
                            Udma_chGetFqRingHandle(chHandle), &pDesc);
        if(UDMA_ETIMEOUT == tempRetVal)
        {
            break;
        }
    }

    /* Unregister all events */
    if(NULL != appTrObj->trEventHandle)
    {
        retVal = Udma_eventUnRegister(appTrObj->trEventHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("\n [Error] UDMA event unregister failed!!\n");
        }
        appTrObj->trEventHandle = NULL;
    }
#if (UDMA_SOC_CFG_RA_NORMAL_PRESENT == 1)
    if(NULL != appChObj->tdCqEventHandle)
    {
        retVal += Udma_eventUnRegister(appChObj->tdCqEventHandle);
        if(UDMA_SOK != retVal)
        {
            App_print("[Error] UDMA event unregister failed!!\n");
        }
        appChObj->tdCqEventHandle = NULL;
    }
#endif

    retVal += Udma_chClose(chHandle);
    if(UDMA_SOK != retVal)
    {
        App_print("\n [Error] UDMA channel close failed!!\n");
    }

    return (retVal);
}

static void App_udmaTrpdInit(App_UdmaTrObj *appTrObj, App_UdmaChObj  *appChObj)
{    
    CSL_UdmapCppi5TRPD *pTrpd = (CSL_UdmapCppi5TRPD *) appChObj->trpdMem;
    CSL_UdmapTR15 *pTr = (CSL_UdmapTR15 *)(appChObj->trpdMem + sizeof(CSL_UdmapTR15));
    uint32_t *pTrResp = (uint32_t *) (appChObj->trpdMem + (sizeof(CSL_UdmapTR15) * 2U));
    uint32_t cqRingNum = Udma_chGetCqRingNum(appChObj->chHandle);

    /* Make TRPD */
    UdmaUtils_makeTrpd(pTrpd, UDMA_TR_TYPE_15, 1U, cqRingNum);
    CSL_udmapCppi5TrSetReload((CSL_UdmapCppi5TRPD*)pTrpd, appTrObj->reloadCnt, 0U);

    /* Setup TR */
    pTr->flags  = CSL_FMK(UDMAP_TR_FLAGS_TYPE, CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING_INDIRECTION);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_STATIC, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EOL, appTrObj->eolType);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EVENT_SIZE, appTrObj->eventSize);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0, appTrObj->trigger);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1, CSL_UDMAP_TR_FLAGS_TRIGGER_NONE);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER0_TYPE, appTrObj->triggerType);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_TRIGGER1_TYPE, CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_CMD_ID, 0x25U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_SA_INDIRECT, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_DA_INDIRECT, 0U);
    pTr->flags |= CSL_FMK(UDMAP_TR_FLAGS_EOP, 1U);
    
    pTr->icnt0    = appTrObj->icnt[0];
    pTr->icnt1    = appTrObj->icnt[1];
    pTr->icnt2    = appTrObj->icnt[2];
    pTr->icnt3    = appTrObj->icnt[3];
    pTr->addr     = (uint64_t) Udma_appVirtToPhyFxn(appTrObj->addr, 0U, NULL);
    pTr->fmtflags = 0x00000000U;        /* Linear addressing, 1 byte per elem.
                                           Replace with CSL-FL API */

    pTr->dicnt0   = appTrObj->dicnt[0];
    pTr->dicnt1   = appTrObj->dicnt[1];
    pTr->dicnt2   = appTrObj->dicnt[2];
    pTr->dicnt3   = appTrObj->dicnt[3];
    pTr->daddr    = (uint64_t) Udma_appVirtToPhyFxn(appTrObj->daddr, 0U, NULL);

    pTr->dim1     = appTrObj->dim[0]; 
    pTr->dim2     = appTrObj->dim[1]; 
    pTr->dim3     = appTrObj->dim[2]; 
    pTr->ddim1    = appTrObj->ddim[0]; 
    pTr->ddim2    = appTrObj->ddim[1]; 
    pTr->ddim3    = appTrObj->ddim[2];

    /* Clear TR response memory */
    *pTrResp = 0xFFFFFFFFU;

    /* Writeback cache */
    Udma_appUtilsCacheWb(appChObj->trpdMem, UDMA_TEST_APP_TRPD_SIZE_ALIGN);

    return;
}

#if defined(FLASH_TYPE_OSPI)
static int32_t App_udmaTrpdSanityCheck(App_UdmaChObj *appChObj, uint64_t pDesc)
{
    int32_t         retVal  = UDMA_SOK;
    uint32_t       *pTrResp, trRespStatus;
    uint8_t        *trpdMem = appChObj->trpdMem;

    /* Check returned descriptor pointer */
    if(((uint64_t) Udma_appPhyToVirtFxn(pDesc, 0U, NULL)) != ((uint64_t) trpdMem))
    {
        App_print("\n [Error] TR descriptor pointer returned doesn't "
                "match the submitted address!!\n");
        retVal = UDMA_EFAIL;
    }

    if(UDMA_SOK == retVal)
    {
        /* Invalidate cache */
        Udma_appUtilsCacheInv(trpdMem, UDMA_TEST_APP_TRPD_SIZE_ALIGN);

        /* check TR response status */
        pTrResp = (uint32_t *) (trpdMem + (sizeof(CSL_UdmapTR15) * 2U));
        trRespStatus = CSL_FEXT(*pTrResp, UDMAP_TR_RESPONSE_STATUS_TYPE);
        if(trRespStatus != CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE)
        {
            App_print("\n [Error] TR Response not completed!!\n");
            retVal = UDMA_EFAIL;
        }
    }
    
    return (retVal);
}
#endif

static inline void App_udmaTrObjInitRead(App_UdmaTestObj *appTestObj, App_UdmaTrObj *appTrObj)
{
    /* TR Reload Count */
    appTrObj->reloadCnt    = 0x1FFU; /* Set to 0x1FFU for perpetual loop */
    appTrObj->trigger      = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL0;
    /* Set interrupt after transferring icnt0*icnt1*icnt2*icnt3 bytes - When TR is completed */
    appTrObj->eventSize    = CSL_UDMAP_TR_FLAGS_EVENT_SIZE_COMPLETION; 
    /* Transfer icnt0*icnt1*icnt2*icnt3 bytes(entire TR) after a trigger */
    appTrObj->triggerType  = CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL;
    /* EOL boundaries for each icnt0*icnt1*icnt2*icnt3 bytes */
    appTrObj->eolType      = CSL_UDMAP_TR_FLAGS_EOL_ICNT0_ICNT1_ICNT2_ICNT3; 

    /* No. of bytes to tansfer after getting interrupt (in this case)*/
    appTrObj->icnt[0] = appTestObj->numBytes; 
    /* No. of times to repeat the tansfer of inct0 bytes */
    appTrObj->icnt[1] = 1U;
    /* No. of times to repeat the tansfer of inct0*icnt1 bytes */
    appTrObj->icnt[2] = 1U;
    /* No. of times to repeat the tansfer of inct0*icnt1*inct2 bytes */
    appTrObj->icnt[3] = 1U;
    /* similar destination params */
    appTrObj->dicnt[0] = appTestObj->numBytes; 
    appTrObj->dicnt[1] = 1U;
    appTrObj->dicnt[2] = 1U;
    appTrObj->dicnt[3] = 1U;

    /* DIM1: Offset for source OSPI data address after transferring inct0 bytes */
    appTrObj->dim[0]  = appTrObj->icnt[0]; /* Use inct0 bytes so that successive triger tranfers the next icnt0 bytes */
    /* DIM2 - Offset for source OSPI data address after transferring inct0*inct1 bytes */
    appTrObj->dim[1]  = appTrObj->icnt[0]*appTrObj->icnt[1]; /* Use inct0*icnt1 bytes so that successive iteration tranfers the next icnt0*icnt1 bytes */
    /* DIM3 - Offset for source OSPI data address after transferring inct0*inct1*inct2 bytes */
    appTrObj->dim[2]  = appTrObj->icnt[0]*appTrObj->icnt[1]*appTrObj->icnt[2]; /* Use inct0*icnt1*icnt2 bytes so that successive iteration tranfers the next inct0*icnt1*icnt2 bytes */

    /* Similar offset for destination RX buffer address */
    appTrObj->ddim[0]  = appTrObj->dicnt[0]; 
    appTrObj->ddim[1]  = appTrObj->dicnt[0]*appTrObj->dicnt[1];
    appTrObj->ddim[2]  = appTrObj->dicnt[0]*appTrObj->dicnt[1]*appTrObj->dicnt[2];

    /* Source Address - OSPI Data address */
    appTrObj->addr  = gUdmaTestOspiFlashDataAddr;
    /* Destination Address - RX Buffer */
    appTrObj->daddr  = &gUdmaTestRxBuf[0U];;

    return;
}

#if defined(FLASH_TYPE_OSPI)
static inline void App_udmaTrObjInitWrite(App_UdmaTestObj *appTestObj, App_UdmaTrObj *appTrObj)
{
    /* TR Reload Count */
    appTrObj->reloadCnt    = 0U; 
    appTrObj->trigger      = CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL0;
    /* Set interrupt after transferring icnt0 bytes */
    appTrObj->eventSize    = CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT1_DEC; 
    /* Transfer icnt0 bytes after a trigger */
    appTrObj->triggerType  = CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC;
    /* EOL boundaries for each icnt0 bytes */
    appTrObj->eolType      = CSL_UDMAP_TR_FLAGS_EOL_ICNT0; 

    if(appTestObj->numBytes > UDMA_TEST_WRITE_CHUNK_SIZE)
    {
        /* For OSPI DMA write, write in chunks. No. of bytes to tansfer after getting interrupt(in this case) */
        appTrObj->icnt[0] = UDMA_TEST_WRITE_CHUNK_SIZE; 
        /* No. of times to repeat the tansfer of inct0 bytes (= No.of chunks in this case) */
        appTrObj->icnt[1] = appTestObj->numBytes/UDMA_TEST_WRITE_CHUNK_SIZE;

        appTrObj->dicnt[0] = UDMA_TEST_WRITE_CHUNK_SIZE; 
        appTrObj->dicnt[1] = appTestObj->numBytes/UDMA_TEST_WRITE_CHUNK_SIZE;
    }
    else
    {
        appTrObj->icnt[0] = appTestObj->numBytes;
        appTrObj->icnt[1] = 1U;
        
        appTrObj->dicnt[0] = appTestObj->numBytes;
        appTrObj->dicnt[1] = 1U;
    }
    
    appTrObj->icnt[2] = 1U;
    appTrObj->icnt[3] = 1U;
    
    appTrObj->dicnt[2] = 1U;
    appTrObj->dicnt[3] = 1U;

    /* DIM1- Offset for source TX Buffer address after transferring inct0 bytes */
    appTrObj->dim[0]  = appTrObj->icnt[0]; /* chunkSize - so that successive triger tranfers the next icnt0(chunkSize) bytes */
    /* DIM2 - Offset for source TX Buffer address after transferring inct0*inct1 bytes */
    appTrObj->dim[1]  = appTrObj->icnt[0]*appTrObj->icnt[1]; /* inct0*icnt1 bytes - so that successive iteration tranfers the next icnt0*icnt1 bytes */
    /* DIM3 - Offset for source TX Buffer address after transferring inct0*inct1*inct2 bytes */
    appTrObj->dim[2]  = appTrObj->icnt[0]*appTrObj->icnt[1]*appTrObj->icnt[2]; /* inct0*icnt1*icnt2 bytes - so that successive iteration tranfers the next inct0*icnt1*icnt2 bytes */

    /* DDIM1/DDIM2/DDIM3 - Similar offset for destination RX buffer address */
    appTrObj->ddim[0]  = appTrObj->dicnt[0]; 
    appTrObj->ddim[1]  = appTrObj->dicnt[0]*appTrObj->dicnt[1];
    appTrObj->ddim[2]  = appTrObj->dicnt[0]*appTrObj->dicnt[1]*appTrObj->dicnt[2];

    /* Source Address - TX Buffer */
    appTrObj->addr  = &gUdmaTestTxBuf[0U];
    /* Destination Address - OSPI Data address  */
    appTrObj->daddr = gUdmaTestOspiFlashDataAddr;

    return;
}
#endif

static void App_printPerfResults(App_UdmaObj *appObj)
{    
    App_UdmaCounterObj  *appCounterObj = &appObj->appCounterObj;
    App_UdmaTestObj     *appTestObj = &appObj->appTestObj;
    uint32_t             tCnt;
    uint32_t             triggerCnt;
    
    appCounterObj->txTotalTicks = appCounterObj->txStopTicks - appCounterObj->txStartTicks;
    appCounterObj->txElapsedTime = (appCounterObj->txTotalTicks*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

    App_printNum("\n OSPI Write %d", appTestObj->numBytes);
    App_printNum(" bytes in %d", (uint32_t)appCounterObj->txElapsedTime);
    App_print("ns.");

    if(appTestObj->testId != UDMA_OSPI_FLASH_TEST_ID_WR_TUNING)
    {
        triggerCnt = UDMA_TEST_XFER_REPEAT_CNT;
        for(tCnt = 0U; tCnt < triggerCnt; tCnt++)
        {
            appCounterObj->rxTotalTicks[tCnt] = appCounterObj->rxStopTicks[tCnt] - appCounterObj->rxStartTicks[tCnt];
            appCounterObj->rxTotalTicks[triggerCnt] += appCounterObj->rxTotalTicks[tCnt];
            appCounterObj->rxElapsedTime[tCnt] = (appCounterObj->rxTotalTicks[tCnt]*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;

            App_printNum("\n OSPI Read %d", appTestObj->numBytes);
            App_printNum(" bytes in %d", (uint32_t)appCounterObj->rxElapsedTime[tCnt]);
            App_print("ns.");
        }
        
        appCounterObj->rxTotalTicks[triggerCnt] = appCounterObj->rxTotalTicks[triggerCnt]/triggerCnt;
        appCounterObj->rxElapsedTime[triggerCnt] = (appCounterObj->rxTotalTicks[triggerCnt]*1000000000U)/(uint64_t)OSPI_FLASH_GTC_CLK_FREQ;
        App_printNum("\n\n Average time for OSPI Read %d", appTestObj->numBytes);
        App_printNum(" bytes = %d", (uint32_t)appCounterObj->rxElapsedTime[triggerCnt]);
        App_print("ns. \n\n");
    }

    return;
}

void App_print(const char *str)
{
    UART_printf("%s", str);
    if(UTRUE == Udma_appIsPrintSupported())
    {
        printf("%s", str);
    }

    return;
}

static void App_printNum(const char *str, uint32_t num)
{
    static char printBuf[200U];

    snprintf(printBuf, 200U, str, num);
    UART_printf("%s", printBuf);

    if(UTRUE == Udma_appIsPrintSupported())
    {
        printf("%s", printBuf);
    }

    return;
}

int32_t App_setGTCClk(uint32_t moduleId,
                      uint32_t clkId,
                      uint64_t clkRateHz)
{
    int32_t retVal;
    uint64_t currClkFreqHz;

    retVal = Sciclient_pmGetModuleClkFreq(moduleId,
                                          clkId,
                                          &currClkFreqHz,
                                          SCICLIENT_SERVICE_WAIT_FOREVER);
    if ((retVal == CSL_PASS) &&
        (currClkFreqHz != clkRateHz))
    {
        retVal = OspiFlash_ClkRateSet(moduleId, clkId, clkRateHz);
    }

    /* Enable GTC */
    HW_WR_REG32(CSL_GTC0_GTC_CFG1_BASE + 0x0U, 0x1);

    return (retVal);
}

static int32_t App_ospiFlashInit(App_OspiObj *ospiObj, uint32_t clk)
{
    int32_t status = UDMA_SOK;
    bool dacMode;

#if defined(FLASH_TYPE_OSPI)
    dacMode = BTRUE;
#else
    dacMode = BFALSE;
#endif

    OspiFlash_spiPhyTuneReset(BTRUE);

    status += OspiFlash_ospiConfigClk(clk);
    if(UDMA_SOK == status)
    {
        App_printNum("\n OSPI RCLK running at %d Hz. \n", clk);
    }

    ospiObj->clk = clk;
    status += OspiFlash_ospiOpen(ospiObj, OSPI_FLASH_WRITE_TIMEOUT, OSPI_FLASH_CHECK_IDLE_DELAY, dacMode, BFALSE);

    status += OspiFlash_ospiEnableDDR(dacMode, BFALSE);

    status += OspiFlash_ospiSetOpcode(dacMode);

#if defined(FLASH_TYPE_OSPI)
    status += OspiFlash_ospiConfigPHY(clk, BFALSE);
#endif

    return (status);    
}

void App_ospiFlashConfigDacMode( App_OspiObj *ospiObj, bool dacMode)
{
    OspiFlash_ospiClose(ospiObj, BFALSE);
    OspiFlash_ospiOpen(ospiObj, OSPI_FLASH_WRITE_TIMEOUT, OSPI_FLASH_CHECK_IDLE_DELAY, BTRUE, BTRUE);
    OspiFlash_ospiEnableDDR(dacMode, BFALSE);
    OspiFlash_ospiSetOpcode(dacMode);
}

#if defined(FLASH_TYPE_OSPI)
static int32_t App_ospiFlashStart(uint32_t numBytes)
{
    int32_t retVal = UDMA_SOK;
    const CSL_ospi_flash_cfgRegs *baseAddr = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    retVal = OspiFlash_ospiEraseBlk(0U, numBytes, BFALSE);

    OspiFlash_ospiXferIntrInit(BFALSE);

    /* Enable PHY pipeline mode */
    CSL_ospiPipelinePhyEnable(baseAddr, UTRUE);

    return (retVal);
}
#endif
