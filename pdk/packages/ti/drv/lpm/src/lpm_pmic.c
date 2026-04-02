/*
*
* Copyright (c) 2021-22 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
/**
 *  \file     lpm_pmic.c
 *
 *  \brief    This file implements switching the SoCs state from ACTIVE to
 *            MCU ONLY mode and then from MCU ONLY to ACTIVE mode.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/lpm/include/lpm_pmic.h>
#include <ti/drv/lpm/src/lpm_utils.h>
#include <ti/drv/pmic/pmic.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define WKUP_VTM_TMPSENS_CTRL_1        (0x42050320)
#define WKUP_VTM_TMPSENS_CTRL_2        (0x42050340)
#define WKUP_VTM_TMPSENS_CTRL_3        (0x42050360)
#define WKUP_VTM_TMPSENS_CTRL_4        (0x42050380)

#define MAXT_OUTRG_EN_SHIFT            (11U)

#define DDR_TEST_ADDRESS               (0xA0000000)
#define MSMC_TEST_ADDRESS              (0x70000000)
#define MCAN_REV_REG                   (0x02700000)

#define MSG_APP_NAME                   "MCU only app: "

/*!
 * \brief   CRC Status
 */
#define PMIC_STATUS_CRC_INIT_VAL            (0x0U)
#define PMIC_CFG_TO_ENABLE_CRC              (0x1U)
#define PMIC_STATUS_CRC_ENABLED             (0x2U)

/*!
 * \brief   Slave addresses of J721E LEO PMIC-A and PMIC-B devices
 */
#define J721E_LEO_PMICA_SLAVE_ADDR            (0x48U)
#define J721E_LEO_PMICA_PAGE1_SLAVE_ADDR      (0x49U)
#define J721E_LEO_PMICA_WDG_SLAVE_ADDR        (0x12U)

#define J721E_LEO_PMICB_SLAVE_ADDR            (0x4CU)
#define J721E_LEO_PMICB_PAGE1_SLAVE_ADDR      (0x4DU)
#define J721E_LEO_PMICB_WDG_SLAVE_ADDR        (0x13U)

/*!
 * \brief   Slave addresses of J7VCL HERA PMIC devices
 */
#define J7VCL_LEO_PMICA_SLAVE_ADDR            (0x48U)
#define J7VCL_LEO_PMICA_PAGE1_SLAVE_ADDR      (0x49U)
#define J7VCL_LEO_PMICA_WDG_SLAVE_ADDR        (0x12U)

#define J7VCL_HERA_PMIC_SLAVE_ADDR            (0x4CU)
#define J7VCL_HERA_PMIC_PAGE1_SLAVE_ADDR      (0x4DU)
#define J7VCL_HERA_PMIC_WDG_SLAVE_ADDR        (0x13U)

/*!
 * \brief   PMIC Startup type
 */
#define PMIC_ENABLE_STARTUP_TYPE        (0x01U)
#define PMIC_NPWRON_STARTUP_TYPE        (0x02U)
#define PMIC_FSD_STARTUP_TYPE           (0x03U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

struct bootApp_boardcfg_rm_resasg
{
    struct tisci_boardcfg_substructure_header subhdr;
    uint16_t                    resasg_entries_size;
    uint16_t                    reserved;
} __attribute__((__packed__));

struct bootApp_boardcfg_rm
{
    struct tisci_boardcfg_abi_rev     rev;
    struct tisci_boardcfg_rm_host_cfg host_cfg;
    struct bootApp_boardcfg_rm_resasg resasg;
} __attribute__((__packed__));

struct bootApp_local_rm_boardcfg
{
    struct bootApp_boardcfg_rm            rm_boardcfg;
    struct tisci_boardcfg_rm_resasg_entry resasg_entries[TISCI_RESASG_ENTRIES_MAX];
};

__attribute((section(".sysfw_data_cfg_board"))) struct tisci_boardcfg bootAppBoardCfg;
__attribute((section(".sysfw_data_cfg_board_rm"))) struct bootApp_local_rm_boardcfg bootAppBoardCfg_rm;
__attribute((section(".sysfw_data_cfg_board_sec"))) struct tisci_boardcfg_sec bootAppBoardCfg_sec;

/* ========================================================================== */
/*              Internal Function Declarations                                */
/* ========================================================================== */

static uint32_t Lpm_activeToMcuSwitch(void);
static uint32_t Lpm_mcuToActiveSwitch(void);

static void Lpm_vtmMaxOutrgAlertDisableForTmpSens1to4();
static void Lpm_swResetMainDomain(void);
static void Lpm_pmicStateChangeActiveToMCUOnly(void);
void Lpm_pmicStateChangeActiveToIORetention(void);

#if defined(PMIC_USE_DRV)
static int32_t Lpm_initPMIC(void);
#else
static int32_t Lpm_i2cInitPMIC(void);
#endif
static void Lpm_pmicStateChangeMCUOnlyToActive(void);
static void Lpm_bringBackMAINDomain(void);
static int32_t Lpm_enableMCU2MAINBridges(void);
static void Lpm_accessMainPeripherals(void);
static void Lpm_boardConfigurationForMainDomain(void);

#if defined(PMIC_USE_DRV)
/*!
 * \brief   Initialize PMIC Instance and corresponding Interface.
 */
static int32_t Lpm_pmicAppInit(Pmic_CoreHandle_t **pmicCoreHandle,
                          Pmic_CoreCfg_t     *pmicConfigData);
                          
static void Lpm_pmicAppDeInit(Pmic_CoreHandle_t *pmicCoreHandle);

/**
 * \brief  SPI stub function read operation to read PMIC registers
 *         using I2C interface
 */
static int32_t Lpm_pmicSpiStubRead(Pmic_CoreHandle_t  *pPmicCorehandle,
                           uint8_t            *pBuf,
                           uint8_t             bufLen);


/**
 * \brief  SPI stub function write operation to write PMIC registers
 *         using I2C interface
 */
static int32_t Lpm_pmicSpiWrite(Pmic_CoreHandle_t  *pPmicCorehandle,
                            uint8_t            *pBuf,
                            uint8_t             bufLen);


/*!
 * \brief   PMIC I2C read function.
 */
static int32_t Lpm_pmicRegRead(Pmic_CoreHandle_t  *pmicCorehandle,
                          uint8_t             instType,
                          uint16_t            regAddr,
                          uint8_t            *pBuf,
                          uint8_t             bufLen);

/*!
 * \brief   PMIC I2C write function.
 */
static int32_t Lpm_pmicRegWrite(Pmic_CoreHandle_t  *pmicCorehandle,
                           uint8_t             instType,
                           uint16_t            regAddr,
                           uint8_t            *pBuf,
                           uint8_t             bufLen);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#if defined(PMIC_USE_DRV)
/* Pointer holds the pPmicCoreHandle for I2C */
Pmic_CoreHandle_t *pPmicCoreHandleI2C = NULL;
/*!
 * \brief   OS specific Critical section locking Variable
 *          Should be OS specific locking varaible to
 *          use OS locking system for PMIC
 */
static SemaphoreP_Handle gLpmPmicSem = NULL;
uint8_t gLpmStartupType = 0U;
uint8_t gLpmEnableBenchMark = 0U;
uint8_t gLpmEnableFaultInjectionRead = 0U;
uint8_t gLpmEnableFaultInjectionWrite = 0U;
uint8_t gLpmReadCount = 0U;
uint8_t gLpmWriteCount = 0U;
uint8_t gLpmSkipReadCount = 0U;
uint8_t gLpmSkipWriteCount = 0U;
int32_t gCrcTestFlag_J721E = PMIC_STATUS_CRC_INIT_VAL;
int32_t gCrcTestFlag_J7VCL = PMIC_STATUS_CRC_INIT_VAL;
/* CRC8 Table with polynomial value:0x7 */
uint8_t gLpmCrc8Tlb[] =
{
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,
    0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
    0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
    0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
    0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5,
    0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
    0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85,
    0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
    0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
    0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
    0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2,
    0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
    0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32,
    0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
    0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
    0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
    0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c,
    0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
    0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec,
    0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
    0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
    0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
    0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c,
    0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
    0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b,
    0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
    0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
    0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
    0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb,
    0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
    0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb,
    0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3
};
#else
I2C_Handle gLpmPmicI2cHandle  = NULL;
#endif

/* ========================================================================== */
/*                            External Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if defined(PMIC_USE_DRV)
/*!
 * \brief   Function to release the I2C lld interface for PMIC
 */
static int32_t Lpm_pmicI2cLldIntfRelease(void **pCommHandle)
{
    if(NULL == (I2C_Handle)*pCommHandle)
    {
        return -1;
    }

    I2C_close((I2C_Handle)*pCommHandle);

    *pCommHandle = NULL;

    return 0;
}

/*!
 * \brief   DeInit Semaphore Handle
 */
static void Lpm_pmicOsalSemaphoreDeInit(void)
{
    if(gLpmPmicSem)
    {
        SemaphoreP_delete(gLpmPmicSem);
        gLpmPmicSem = NULL;
    }
}

/**
 * \brief    Deinitalize SPI stub function
 */
static int32_t Lpm_pmicSpiStubDeInit(void **pCommHandle)
{
    Lpm_pmicAppDeInit(pPmicCoreHandleI2C);

    if(NULL == (I2C_Handle)*pCommHandle)
    {
        return -1;
    }

    *pCommHandle = NULL;
    return 0;
}

/*!
 * \brief   Interface release function for PMIC to release the SPI Bus
 */
static int32_t Lpm_pmicSpiLldIntfRelease(void **pCommHandle)
{
    int32_t ret = 0;

    ret = Lpm_pmicSpiStubDeInit(pCommHandle);

    return ret;
}

static void Lpm_pmicAppDeInit(Pmic_CoreHandle_t *pmicCoreHandle)
{
    if(PMIC_INTF_SINGLE_I2C == pmicCoreHandle->commMode)
    {
        Lpm_pmicI2cLldIntfRelease(&pmicCoreHandle->pCommHandle);
    }
    else if(PMIC_INTF_DUAL_I2C == pmicCoreHandle->commMode)
    {
        Lpm_pmicI2cLldIntfRelease(&pmicCoreHandle->pCommHandle);
        Lpm_pmicI2cLldIntfRelease(&pmicCoreHandle->pQACommHandle);
    }
    else if(PMIC_INTF_SPI  == pmicCoreHandle->commMode)
    {
        Lpm_pmicSpiLldIntfRelease(&pmicCoreHandle->pCommHandle);
    }

    Pmic_deinit(pmicCoreHandle);

    free(pmicCoreHandle);

    /* PMIC Semaphore Clean-up */
    Lpm_pmicOsalSemaphoreDeInit();
}

/**
 * \brief    This API Set Config for TI HW I2C instances
 *
 * \param    instance [IN] I2C instance number
 * \param    baseAddr [IN] Register base address of the I2C instance
 */
static void Lpm_pmicSetConfigI2c(uint8_t instance, uint32_t baseAddr)
{
    I2C_HwAttrs i2cCfg;

    I2C_socGetInitCfg(instance, &i2cCfg);
    i2cCfg.baseAddr   = baseAddr;
    i2cCfg.enableIntr = 0U;
    I2C_socSetInitCfg(instance, &i2cCfg);
}

/**
 *
 * \brief    Initalize SPI stub function to access PMIC registers using
 *           I2C Communication Handle
 */
static int32_t Lpm_pmicSpiStubInit(Pmic_CoreCfg_t  *pPmicConfigData)
{
    int32_t pmicStatus = 0;
    Pmic_CoreCfg_t pmicConfigDataI2c = {0U};

    /* Fill parameters to pmicConfigDataI2C */
    pmicConfigDataI2c.pmicDeviceType      = pPmicConfigData->pmicDeviceType;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigDataI2c.commMode            = PMIC_INTF_DUAL_I2C;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigDataI2c.slaveAddr           = J721E_LEO_PMICA_SLAVE_ADDR;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigDataI2c.qaSlaveAddr         = J721E_LEO_PMICA_WDG_SLAVE_ADDR;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigDataI2c.nvmSlaveAddr        = J721E_LEO_PMICA_PAGE1_SLAVE_ADDR;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_NVMSLAVEADDR_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCommIoRead   = Lpm_pmicRegRead;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_COMM_IO_RD_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCommIoWrite  = Lpm_pmicRegWrite;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_COMM_IO_WR_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCritSecStart =
                                          pPmicConfigData->pFnPmicCritSecStart;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_CRITSEC_START_VALID_SHIFT;

    pmicConfigDataI2c.pFnPmicCritSecStop  = pPmicConfigData->pFnPmicCritSecStop;
    pmicConfigDataI2c.validParams        |= PMIC_CFG_CRITSEC_STOP_VALID_SHIFT;

    /*
     * Recalling appInit function to initialize I2C PMIC Core handle
     * for Stub Operations
     */
    pmicStatus = Lpm_pmicAppInit(&pPmicCoreHandleI2C, &pmicConfigDataI2c);

    if(0 == pmicStatus)
    {
        /*
         * Update Valid Communication Handle to make SPI stub
         * PMIC core handle Init Success
         */
        pPmicConfigData->pCommHandle  = pPmicCoreHandleI2C->pCommHandle;
        pPmicConfigData->validParams |= PMIC_CFG_COMM_HANDLE_VALID_SHIFT;
    }
    return pmicStatus;
}

/*!
 * \brief   Interface setup function for PMIC to create instance and
 *          initialise the SPI Bus for PMIC Communication
 */
static int32_t Lpm_pmicSpiLldIntfSetup(Pmic_CoreCfg_t *pPmicConfigData)
{
    int32_t ret = 0;

    ret = Lpm_pmicSpiStubInit(pPmicConfigData);

    return ret;
}

/*!
 * \brief   Function to setup the I2C lld interface for PMIC
 */
static int32_t Lpm_pmicI2cLldIntfSetup(Pmic_CoreCfg_t  *pPmicConfigData,
                                            uint8_t          instType)
{
    I2C_Params i2cParams;
    I2C_Handle i2cHandle  = NULL;
    uint8_t i2c_instance  = 0U;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"%s(): %d: %s I2C Setup...\n", __func__, __LINE__,
             (instType == PMIC_MAIN_INST)? "PMIC_MAIN_INST": "PMIC_QA_INST");

    /* Main I2c BUS */
    if(instType == PMIC_MAIN_INST)
    {
        /* Initialize i2c core instances */
        I2C_init();

        i2c_instance = 0U;
        Lpm_pmicSetConfigI2c(i2c_instance, CSL_WKUP_I2C0_CFG_BASE);
    }
    /* For WDG QA I2C BUS */
    else if(PMIC_QA_INST == instType)
    {
        i2c_instance = 1U;
        Lpm_pmicSetConfigI2c(i2c_instance, CSL_MCU_I2C0_CFG_BASE);
    }

    /* Configured i2cParams.bitRate with standard I2C_100kHz */
    I2C_Params_init(&i2cParams);

    i2cHandle = I2C_open(i2c_instance, &i2cParams);
    if(NULL == i2cHandle)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"I2C_open is failed!!!\n");
        return -1;
    }

    /* Main I2c BUS */
    if(instType == PMIC_MAIN_INST)
    {
        pPmicConfigData->pCommHandle = i2cHandle;
    }
    /* For WDOG QA I2C BUS */
    else if(PMIC_QA_INST == instType)
    {
        pPmicConfigData->pQACommHandle = i2cHandle;
    }

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"%s(): %d: done...\n", __func__, __LINE__);
    return 0;
}

/*!
 * \brief   Function to setup the QA I2c interface for LEO PMIC depending
 *          upon i2c mode
 */
static int32_t Lpm_pmicLeoDualI2cPinSetup(Pmic_CoreHandle_t *pPmicHandle)
{
    int32_t pmicStatus     = 0;
    Pmic_GpioCfg_t gpioCfg = {0U};

    gpioCfg.validParams      = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                               PMIC_GPIO_CFG_OD_VALID_SHIFT;

    pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                           PMIC_TPS6594X_GPIO1_PIN,
                                           &gpioCfg);

    if(PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
    {
       if(gpioCfg.pinFunc == PMIC_TPS6594X_GPIO_PINFUNC_GPIO1_SCL_I2C2_CS_SPI)
       {
           gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO;
       }
    }
    if(PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
    {
        gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
        gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO1_SCL_I2C2_CS_SPI;
    }

    pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                           PMIC_TPS6594X_GPIO1_PIN,
                                           gpioCfg);

    gpioCfg.validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                          PMIC_GPIO_CFG_OD_VALID_SHIFT;
    if(0 == pmicStatus)
    {
        pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                               PMIC_TPS6594X_GPIO2_PIN,
                                               &gpioCfg);
    }
    if(0 == pmicStatus)
    {
        if(PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
        {
            if(gpioCfg.pinFunc ==
               PMIC_TPS6594X_GPIO_PINFUNC_GPIO2_SDA_I2C2_SDO_SPI)
            {
                gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO;
            }
        }
        if(PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
        {
            gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
            gpioCfg.pinFunc = PMIC_TPS6594X_GPIO_PINFUNC_GPIO2_SDA_I2C2_SDO_SPI;
        }

        pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                               PMIC_TPS6594X_GPIO2_PIN,
                                               gpioCfg);
    }

    return pmicStatus;
}

/*!
 * \brief   Function to setup the QA I2c interface for HERA PMIC depending
 *          upon i2c mode
 */
static int32_t Lpm_pmicHeraDualI2cPinSetup(Pmic_CoreHandle_t *pPmicHandle)
{
    int32_t pmicStatus     = 0;
    Pmic_GpioCfg_t gpioCfg = {0U};

    gpioCfg.validParams      = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                               PMIC_GPIO_CFG_OD_VALID_SHIFT;

    pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                           PMIC_LP8764X_GPIO2_PIN,
                                           &gpioCfg);

    if(PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
    {
       if(gpioCfg.pinFunc == PMIC_LP8764X_GPIO_PINFUNC_GPIO2_SCL_I2C2)
       {
           gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO;
       }
    }
    if(PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
    {
        gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO2_SCL_I2C2;
        gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
    }

    pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                           PMIC_LP8764X_GPIO2_PIN,
                                           gpioCfg);

    gpioCfg.validParams = PMIC_GPIO_CFG_PINFUNC_VALID_SHIFT |
                          PMIC_GPIO_CFG_OD_VALID_SHIFT;
    if(0 == pmicStatus)
    {
        pmicStatus = Pmic_gpioGetConfiguration(pPmicHandle,
                                               PMIC_LP8764X_GPIO3_PIN,
                                               &gpioCfg);
    }
    if(0 == pmicStatus)
    {
        if(PMIC_INTF_SINGLE_I2C == pPmicHandle->commMode)
        {
            if(gpioCfg.pinFunc == PMIC_LP8764X_GPIO_PINFUNC_GPIO3_SDA_I2C2)
            {
                gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO;
            }
        }
        if(PMIC_INTF_DUAL_I2C == pPmicHandle->commMode)
        {
            gpioCfg.pinFunc = PMIC_LP8764X_GPIO_PINFUNC_GPIO3_SDA_I2C2;
            gpioCfg.outputSignalType = PMIC_GPIO_OPEN_DRAIN_OUTPUT;
        }

        pmicStatus = Pmic_gpioSetConfiguration(pPmicHandle,
                                               PMIC_LP8764X_GPIO3_PIN,
                                               gpioCfg);
    }

    return pmicStatus;
}

/*!
 * \brief   Initialize Semaphore Handle
 */
static void Lpm_pmicOsalSemaphoreInit(void)
{
    SemaphoreP_Params pmic_SemParams;

    /* Create call back semaphore */
    SemaphoreP_Params_init(&pmic_SemParams);
    pmic_SemParams.mode = SemaphoreP_Mode_BINARY;
    gLpmPmicSem = SemaphoreP_create(1U, &pmic_SemParams);
}


/**
 * \brief  Function to probe PMIC slave devices on I2C instance
 */
static int32_t Lpm_pmicI2cDevices(Pmic_CoreHandle_t  *pPmicCorehandle,
                                     uint32_t            instType)
{
    int32_t status = 0;
    uint16_t slaveAddr;
    I2C_Handle i2cHandle;

    if(PMIC_INTF_SINGLE_I2C == pPmicCorehandle->commMode)
    {
        i2cHandle = pPmicCorehandle->pCommHandle;
        /* For Main PAGE SLAVE ID */
        slaveAddr = pPmicCorehandle->slaveAddr;
        if(I2C_STATUS_SUCCESS ==
                      I2C_control(i2cHandle, I2C_CMD_PROBE, &slaveAddr))
        {
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"I2C%d: Passed for address 0x%X !!! \r\n",
                               instType, slaveAddr);
        }
        else
        {
            status = -1;
        }
        /* For WD PAGE SLAVE ID */
        slaveAddr = pPmicCorehandle->qaSlaveAddr;
        if(I2C_STATUS_SUCCESS ==
                      I2C_control(i2cHandle, I2C_CMD_PROBE, &slaveAddr))
        {
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"I2C%d: Passed for address 0x%X !!! \r\n",
                               instType, slaveAddr);
        }
        else
        {
            status = -1;
        }
    }
    if(PMIC_INTF_DUAL_I2C == pPmicCorehandle->commMode)
    {
        /* Main I2c BUS */
        if(PMIC_MAIN_INST == instType)
        {
            slaveAddr = pPmicCorehandle->slaveAddr;
            i2cHandle = pPmicCorehandle->pCommHandle;
        }
        /* For WDG QA I2C BUS */
        else if(PMIC_QA_INST == instType)
        {
            slaveAddr = pPmicCorehandle->qaSlaveAddr;
            i2cHandle = pPmicCorehandle->pQACommHandle;
        }
        if(I2C_STATUS_SUCCESS ==
                      I2C_control(i2cHandle, I2C_CMD_PROBE, &slaveAddr))
        {
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"I2C%d: Passed for address 0x%X !!! \r\n",
                               instType, slaveAddr);
        }
        else
        {
            status = -1;
        }
    }

    return status;
}

/*!
 * \brief   Get PMIC StartUp Interrupt Type.
 *          This function deciphers all interrupts and find startup type.
 */
static uint32_t Lpm_pmicGetStartupType(Pmic_CoreHandle_t *pPmicCoreHandle)
{
    Pmic_IrqStatus_t errStat  = {0U};
    int32_t pmicStatus        = 0;
    uint8_t type = 0U;

    pmicStatus = Pmic_irqGetErrStatus(pPmicCoreHandle, &errStat, false);
    switch(pPmicCoreHandle->pmicDeviceType)
    {
        case PMIC_DEV_LEO_TPS6594X:
            switch(gLpmStartupType)
            {
                case PMIC_ENABLE_STARTUP_TYPE:
                    if((0 == pmicStatus) &&
                       ((errStat.intStatus[PMIC_TPS6594X_ENABLE_INT/32U] &
                        (1U << (PMIC_TPS6594X_ENABLE_INT % 32U))) != 0U))
                    {
                        type = PMIC_ENABLE_STARTUP_TYPE;
                    }
                    break;
                case PMIC_NPWRON_STARTUP_TYPE:
                    if((0 == pmicStatus) &&
                       ((errStat.intStatus[PMIC_TPS6594X_NPWRON_START_INT/32U] &
                        (1U << (PMIC_TPS6594X_NPWRON_START_INT % 32U))) != 0U))
                    {
                        type = PMIC_NPWRON_STARTUP_TYPE;
                    }
                    break;
                case PMIC_FSD_STARTUP_TYPE:
                    if((0 == pmicStatus) &&
                       ((errStat.intStatus[PMIC_TPS6594X_FSD_INT/32U] &
                        (1U << (PMIC_TPS6594X_FSD_INT % 32U))) != 0U))
                    {
                        type = PMIC_FSD_STARTUP_TYPE;
                    }
                    break;
                default:
                        type = 0U;
                    break;
            }
            break;
        case PMIC_DEV_HERA_LP8764X:
            switch(gLpmStartupType)
            {
                case PMIC_ENABLE_STARTUP_TYPE:
                    if((0 == pmicStatus) &&
                       ((errStat.intStatus[PMIC_LP8764X_ENABLE_INT/32U] &
                        (1U << (PMIC_LP8764X_ENABLE_INT % 32U))) != 0U))
                    {
                        type = PMIC_ENABLE_STARTUP_TYPE;
                    }
                    break;
                case PMIC_FSD_STARTUP_TYPE:
                    if((0 == pmicStatus) &&
                       ((errStat.intStatus[PMIC_LP8764X_FSD_INT/32U] &
                        (1U << (PMIC_LP8764X_FSD_INT % 32U))) != 0U))
                    {
                        type = PMIC_FSD_STARTUP_TYPE;
                    }
                    break;
                default:
                    type = 0U;
                    break;
            }
            break;
    }

    gLpmStartupType = type;

    return pmicStatus;
}

/*!
 * \brief   PMIC Interrupt decipher and clear function
 *          This function deciphers all interrupts and clears the status
 */
static int32_t Lpm_pmiIntrClr(Pmic_CoreHandle_t *pmicHandle)
{
    int32_t pmicStatus = 0;
    Pmic_CoreHandle_t handle  = *(Pmic_CoreHandle_t *)pmicHandle;
    Pmic_IrqStatus_t errStat  = {0U};
    uint8_t irqNum;

    if(gLpmStartupType != 0U)
    {
        pmicStatus = Lpm_pmicGetStartupType(pmicHandle);
    }

    if(0 == pmicStatus)
    {
        pmicStatus = Pmic_irqGetErrStatus(&handle, &errStat, false);
        {
            int i = 0;
            for(i=0;i<4; i++)
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"INT STAT[%d]: 0x%08x\n", i, errStat.intStatus[i]);
            }
        }
    }

    if(0 == pmicStatus)
    {
        while(0 == pmicStatus)
        {
            pmicStatus = Pmic_getNextErrorStatus(&handle, &errStat, &irqNum);
            if(0 == pmicStatus)
            {
                pmicStatus = Pmic_irqClrErrStatus(&handle, irqNum);
            }

        }
    }

    if(-1 == pmicStatus)
    {
        pmicStatus = 0;
    }

    if(0 == pmicStatus)
    {
        pmicStatus = Pmic_irqGetErrStatus(&handle, &errStat, false);
        {
            int i = 0;
            for(i=0;i<4; i++)
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"INT STAT[%d]: 0x%08x\n", i, errStat.intStatus[i]);
            }
        }
    }


    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n");
    return pmicStatus;
}

/*!
 * \brief    : Prints time taken for a given Valid string and returns delta.
 */
static uint64_t Lpm_pmicPrintTimeTakenInUsecs(uint64_t t1, const char *str)
{
    uint64_t t2 = 0;
    uint64_t delta = 0;

    t2 = TimerP_getTimeInUsecs();
    delta = t2 - t1;

    if(NULL != str)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Time taken for %50s: %6d usec\n", str, (uint32_t)delta);
    }

    return delta;
}

/*!
 * \brief   Initialize PMIC Instance and corresponding Interface.
 *
 * \param   pmicCoreHandle    [OUT]     PMIC Core Handle.
 *
 * \retval  0 in case of success or appropriate error code.
 *          For valid values see \ref Pmic_ErrorCodes
 */
static int32_t Lpm_pmicAppInit(Pmic_CoreHandle_t **pmicCoreHandle,
                          Pmic_CoreCfg_t     *pmicConfigData)
{
    int32_t pmicStatus = 0;
    Pmic_CoreHandle_t *pmicHandle = NULL;
    uint8_t   i2c1SpiCrcStatus = 0xFF, i2c2CrcStatus = 0xFF;
    uint8_t i2c1Speed, i2c2Speed;

    /* Initialize Pmic Semaphore */
    Lpm_pmicOsalSemaphoreInit();

    if(pmicCoreHandle == NULL)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Invalid PMIC core Handle Reference\n");
        return -1;
    }

    if(pmicConfigData == NULL)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Invalid PMIC config Data - NULL \n");
        return -1;
    }


    /* Allocate memory for PMIC core Handle */
    pmicHandle = malloc(sizeof(Pmic_CoreHandle_t));
    if(pmicHandle == NULL)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Failed to allocate memory to pmicHandle\n");
        return -1;
    }

    memset(pmicHandle, 0, sizeof(Pmic_CoreHandle_t));

    /* For single I2C Instance */
    if(PMIC_INTF_SINGLE_I2C == pmicConfigData->commMode)
    {
        uint64_t delta = 0;
        /* Get PMIC valid Main I2C Instance */
        pmicStatus = Lpm_pmicI2cLldIntfSetup(pmicConfigData,
                                                  PMIC_MAIN_INST);
        if(0 == pmicStatus)
        {
            pmicConfigData->validParams |= PMIC_CFG_COMM_HANDLE_VALID_SHIFT;
            /* Update instance type to pmicConfigData */
            pmicConfigData->instType = PMIC_MAIN_INST;
            if(true == gLpmEnableBenchMark)
            {
                uint64_t t1 = 0;
                t1 = Lpm_pmicPrintTimeTakenInUsecs(0U, NULL);
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
                delta = Lpm_pmicPrintTimeTakenInUsecs(t1, NULL);
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"--------------------------------------\n");
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Time taken for %50s: %6d usec\n",
                            "Pmic_init API for single instance",
                            (uint32_t)delta);
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"--------------------------------------\n");
            }
            else
            {
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
            }

            /*
             * Check for Warning message due to Invalid Device ID.
             * And continue the application with WARNING message.
             */
            if(PMIC_ST_WARN_INV_DEVICE_ID == pmicStatus)
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\n*** WARNING: Found Invalid DEVICE ID ***\n\n");
                pmicStatus = 0;
            }
        }
#if defined(SOC_J721E)
        if((0 == pmicStatus) &&
           (PMIC_CFG_TO_ENABLE_CRC == gCrcTestFlag_J721E))
        {
            if(PMIC_SILICON_REV_ID_PG_2_0 == pmicHandle->pmicDevSiliconRev)
            {
                pmicStatus = Pmic_enableCRC(pmicHandle);
                if(0 == pmicStatus)
                {
                    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n enableCRC - pmicStatus %d crcEnable %d \r\n",pmicStatus, pmicHandle->crcEnable);
                    Osal_delay(10);
                    gCrcTestFlag_J721E = PMIC_STATUS_CRC_ENABLED;
                }
            }
        }
#endif

#if defined(SOC_J7200)
        if((0 == pmicStatus) &&
           (PMIC_CFG_TO_ENABLE_CRC == gCrcTestFlag_J7VCL))
        {
            if(PMIC_SILICON_REV_ID_PG_2_0 == pmicHandle->pmicDevSiliconRev)
            {
                pmicStatus = Pmic_enableCRC(pmicHandle);
                if(0 == pmicStatus)
                {
                    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n enableCRC - pmicStatus %d crcEnable %d \r\n",pmicStatus, pmicHandle->crcEnable);
                    Osal_delay(10);
                    gCrcTestFlag_J7VCL = PMIC_STATUS_CRC_ENABLED;
                }
            }
        }
#endif

        if(0 == pmicStatus)
        {
            pmicStatus = Pmic_getCrcStatus(pmicHandle,
                                           &i2c1SpiCrcStatus,
                                           &i2c2CrcStatus);
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n pmicStatus %d i2c1SpiCrcStatus %d i2c2CrcStatus %d \r\n",pmicStatus, i2c1SpiCrcStatus, i2c2CrcStatus);
        }

        if(0 == pmicStatus)
        {
            /* Probe connected PMIC device on given i2c Instance */
            Lpm_pmicI2cDevices(pmicHandle, PMIC_MAIN_INST);
        }
        if(PMIC_DEV_LEO_TPS6594X == pmicHandle->pmicDeviceType)
        {
            /* Check and De-select I2C2 PINFUNC for GPIO-1 and GPIO-2 */
            pmicStatus = Lpm_pmicLeoDualI2cPinSetup(pmicHandle);
        }
        if(PMIC_DEV_HERA_LP8764X == pmicHandle->pmicDeviceType)
        {
            /* Check and De-select I2C2 PINFUNC for GPIO-2 and GPIO-3 */
            pmicStatus = Lpm_pmicHeraDualI2cPinSetup(pmicHandle);
        }
        if(0 == pmicStatus)
        {
            /* Setup nSLEEP signals */
            pmicStatus = Pmic_fsmDeviceOnRequest(pmicHandle);
        }

        if(0 == pmicStatus)
        {
            pmicStatus = Lpm_pmiIntrClr(pmicHandle);
        }

        if(0 == pmicStatus)
        {
            /* Setup I2C1 Speed based on commMode */
            pmicStatus = Pmic_setI2CSpeedCfg(pmicHandle);
            if(0 == pmicStatus)
            {
                /* Get configured value for I2C1 Speed based on commMode */
                pmicStatus = Pmic_getI2CSpeed(pmicHandle, &i2c1Speed, 
                                              &i2c2Speed);
            }

            if(0 == pmicStatus)
            {
                if(PMIC_I2C_STANDARD_MODE == i2c1Speed)
                {
                    pmicStatus = 0;
                }
                else
                {
                    pmicStatus = -1;
                }
            }
        }

    }
    /* For DUAL I2C Instance */
    else if(PMIC_INTF_DUAL_I2C == pmicConfigData->commMode)
    {
        uint64_t delta = 0;
        /* Get PMIC valid Main I2C Instance */
        pmicStatus = Lpm_pmicI2cLldIntfSetup(pmicConfigData,
                                                  PMIC_MAIN_INST);
        if(0 == pmicStatus)
        {
            pmicConfigData->validParams |= PMIC_CFG_COMM_HANDLE_VALID_SHIFT;
            /* Update instance type to pmicConfigData */
            pmicConfigData->instType = PMIC_MAIN_INST;
            if(true == gLpmEnableBenchMark)
            {
                uint64_t t1 = 0;
                t1 = Lpm_pmicPrintTimeTakenInUsecs(0U, NULL);
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
                delta = Lpm_pmicPrintTimeTakenInUsecs(t1, NULL);
            }
            else
            {
                /* Get PMIC core Handle for Main Instance */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
            }
            /*
             * Check for Warning message due to Invalid Device ID.
             * And continue the application with WARNING message.
             */
            if(PMIC_ST_WARN_INV_DEVICE_ID == pmicStatus)
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\n*** WARNING: Found Invalid DEVICE ID ***\n\n");
                pmicStatus = 0;
            }
        }
#if defined(SOC_J721E)
        if((0 == pmicStatus) &&
           (PMIC_CFG_TO_ENABLE_CRC == gCrcTestFlag_J721E))
        {
            if(PMIC_SILICON_REV_ID_PG_2_0 == pmicHandle->pmicDevSiliconRev)
            {
                pmicStatus = Pmic_enableCRC(pmicHandle);
                if(0 == pmicStatus)
                {
                    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n enableCRC - pmicStatus %d crcEnable %d \r\n",pmicStatus, pmicHandle->crcEnable);
                    Osal_delay(10);
                    gCrcTestFlag_J721E = PMIC_STATUS_CRC_ENABLED;
                }
            }
        }
#endif

#if defined(SOC_J7200)
        if((0 == pmicStatus) &&
           (PMIC_CFG_TO_ENABLE_CRC == gCrcTestFlag_J7VCL))
        {
            if(PMIC_SILICON_REV_ID_PG_2_0 == pmicHandle->pmicDevSiliconRev)
            {
                pmicStatus = Pmic_enableCRC(pmicHandle);
                if(0 == pmicStatus)
                {
                    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n enableCRC - pmicStatus %d crcEnable %d \r\n",pmicStatus, pmicHandle->crcEnable);
                    Osal_delay(10);
                    gCrcTestFlag_J7VCL = PMIC_STATUS_CRC_ENABLED;
                }
            }
        }
#endif

        if(0 == pmicStatus)
        {
            pmicStatus = Pmic_getCrcStatus(pmicHandle,
                                           &i2c1SpiCrcStatus,
                                           &i2c2CrcStatus);
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n pmicStatus %d i2c1SpiCrcStatus %d i2c2CrcStatus %d \r\n",pmicStatus, i2c1SpiCrcStatus, i2c2CrcStatus);


        }

        if(0 == pmicStatus)
        {
            /* Probe connected PMIC device on given i2c Instance */
            Lpm_pmicI2cDevices(pmicHandle, PMIC_MAIN_INST);
        }
        if(0 == pmicStatus)
        {
            /* Setup nSLEEP signals */
            pmicStatus = Pmic_fsmDeviceOnRequest(pmicHandle);
        }

        if(0 == pmicStatus)
        {
            pmicStatus = Lpm_pmiIntrClr(pmicHandle);
        }

        if(0 == pmicStatus)
        {
            if(PMIC_DEV_LEO_TPS6594X == pmicHandle->pmicDeviceType)
            {
                /* Setup leo pmic Dual I2C functionality to GPIO-1 & GPIO-2 */
                pmicStatus = Lpm_pmicLeoDualI2cPinSetup(pmicHandle);
            }
            if(PMIC_DEV_HERA_LP8764X == pmicHandle->pmicDeviceType)
            {
                /* Setup hera pmic Dual I2C functionality to GPIO-2 & GPIO-3 */
                pmicStatus = Lpm_pmicHeraDualI2cPinSetup(pmicHandle);
            }
        }

        if(0 == pmicStatus)
        {
            /* Get PMIC valid QA I2C Instance */
            pmicStatus = Lpm_pmicI2cLldIntfSetup(pmicConfigData,
                                                      PMIC_QA_INST);
        }

        if(0 == pmicStatus)
        {
            pmicConfigData->validParams |= PMIC_CFG_QACOMM_HANDLE_VALID_SHIFT;
            /* Update instance type to pmicConfigData */
            pmicConfigData->instType = PMIC_QA_INST;
            if(true == gLpmEnableBenchMark)
            {
                uint64_t t1 = 0;
                t1 = Lpm_pmicPrintTimeTakenInUsecs(0U, NULL);
                /* Get PMIC core Handle for QA Instances */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
                delta += Lpm_pmicPrintTimeTakenInUsecs(t1, NULL);
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"--------------------------------------\n");
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Time taken for %50s: %6d usec\n",
                            "Pmic_init API for Dual instance",
                            (uint32_t)delta);
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"--------------------------------------\n");
            }
            else
            {
                /* Get PMIC core Handle for QA Instances */
                pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
            }
        }
        if(0 == pmicStatus)
        {
            /* Probe connected PMIC device on given i2c Instance */
            Lpm_pmicI2cDevices(pmicHandle, PMIC_QA_INST);
        }

        if(0 == pmicStatus)
        {
            /* Setup I2C1 Speed based on commMode */
            pmicStatus = Pmic_setI2CSpeedCfg(pmicHandle);
            if(0 == pmicStatus)
            {
                /* Get configured value for I2C1 Speed based on commMode */
                pmicStatus = Pmic_getI2CSpeed(pmicHandle, &i2c1Speed, 
                                              &i2c2Speed);
            }

            if(0 == pmicStatus)
            {
                if((PMIC_I2C_STANDARD_MODE == i2c1Speed) &&
                   (PMIC_I2C_STANDARD_MODE == i2c2Speed))
                {
                    pmicStatus = 0;
                }
                else
                {
                    pmicStatus = -1;
                }
            }
        }

    }
    /* For SPI Instance */
    else if(PMIC_INTF_SPI  == pmicConfigData->commMode)
    {
        /* Get PMIC valid Main SPI Communication Handle */
        pmicStatus = Lpm_pmicSpiLldIntfSetup(pmicConfigData);
        if(0 == pmicStatus)
        {
            /* Update MAIN instance type to pmicConfigData for SPI */
            pmicConfigData->instType = PMIC_MAIN_INST;
            pmicStatus = Pmic_init(pmicConfigData, pmicHandle);
            /*
             * Check for Warning message due to Invalid Device ID.
             * And continue the application with WARNING message.
             */
            if(PMIC_ST_WARN_INV_DEVICE_ID == pmicStatus)
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\n*** WARNING: Found Invalid DEVICE ID ***\n\n");
                pmicStatus = 0;
            }
        }
#if defined(SOC_J721E)
        if((0 == pmicStatus) &&
           (PMIC_CFG_TO_ENABLE_CRC == gCrcTestFlag_J721E))
        {
            if(PMIC_SILICON_REV_ID_PG_2_0 == pmicHandle->pmicDevSiliconRev)
            {
                pmicStatus = Pmic_enableCRC(pmicHandle);
                if(0 == pmicStatus)
                {
                    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n enableCRC - pmicStatus %d crcEnable %d \r\n",pmicStatus, pmicHandle->crcEnable);
                    Osal_delay(10);
                    gCrcTestFlag_J721E = PMIC_STATUS_CRC_ENABLED;
                }
            }
        }
#endif

#if defined(SOC_J7200)
        if((0 == pmicStatus) &&
           (PMIC_CFG_TO_ENABLE_CRC == gCrcTestFlag_J7VCL))
        {
            if(PMIC_SILICON_REV_ID_PG_2_0 == pmicHandle->pmicDevSiliconRev)
            {
                pmicStatus = Pmic_enableCRC(pmicHandle);
                if(0 == pmicStatus)
                {
                    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n enableCRC - pmicStatus %d crcEnable %d \r\n",pmicStatus, pmicHandle->crcEnable);
                    Osal_delay(10);
                    gCrcTestFlag_J7VCL = PMIC_STATUS_CRC_ENABLED;
                }
            }
        }
#endif

        if(0 == pmicStatus)
        {
            pmicStatus = Pmic_getCrcStatus(pmicHandle,
                                           &i2c1SpiCrcStatus,
                                           &i2c2CrcStatus);
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"\r\n pmicStatus %d i2c1SpiCrcStatus %d i2c2CrcStatus %d \r\n",pmicStatus, i2c1SpiCrcStatus, i2c2CrcStatus);


        }

        if(0 == pmicStatus)
        {
            /* Setup nSLEEP signals */
            pmicStatus = Pmic_fsmDeviceOnRequest(pmicHandle);
        }

        if(0 == pmicStatus)
        {
            pmicStatus = Lpm_pmiIntrClr(pmicHandle);
        }
    }

    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"%s(): %d: FAILED with status: %d\n",
                            __func__, __LINE__,  pmicStatus);
    }

    *pmicCoreHandle = pmicHandle;

    return pmicStatus;
}

/*!
 * \brief   PMIC Critical section Lock function
 *          This function should have OS specific locking setup and should
 *          assigned to 'pmicConfigData->pFnPmicCritSecStart'
 */
static void Lpm_pmicCriticalSectionStartFn(void)
{

    if(SemaphoreP_OK != SemaphoreP_pend(gLpmPmicSem,
                                        SemaphoreP_WAIT_FOREVER))
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"%s(): Invalid Semaphore Handle\n", __func__);
    }
}

/*!
 * \brief   PMIC Critical section Unlock function
 *          This function should have OS specific locking setup and should
 *          assigned to 'pmicConfigData->pFnPmicCritSecStop'
 */
static void Lpm_pmicCriticalSectionStopFn(void)
{
    if(SemaphoreP_OK != SemaphoreP_post(gLpmPmicSem))
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"%s(): Invalid Semaphore Handle\n", __func__);
    }
}

/*!
 * \brief   PMIC I2C read function.
 */
static int32_t Lpm_pmicRegRead(Pmic_CoreHandle_t  *pmicCorehandle,
                          uint8_t             instType,
                          uint16_t            regAddr,
                          uint8_t            *pBuf,
                          uint8_t             bufLen)
{
    int8_t ret     = 0U;

    if((PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode) ||
       (PMIC_INTF_DUAL_I2C   == pmicCorehandle->commMode))
    {
        I2C_Transaction transaction;
        I2C_transactionInit(&transaction);

        /* Set register offset for read and write */
        transaction.readBuf    = pBuf;
        transaction.readCount  = bufLen;
        transaction.writeBuf   = &regAddr;
        transaction.writeCount = 1U;

        /* Main I2c BUS */
        if(PMIC_MAIN_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->slaveAddr;
            ret = I2C_transfer((I2C_Handle)pmicCorehandle->pCommHandle,
                                &transaction);
            if(ret != I2C_STS_SUCCESS)
            {
                return -1;
            }
        }

        /* For WDOG QA I2C BUS */
        if(PMIC_QA_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->qaSlaveAddr;
            if(PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pCommHandle,
                                   &transaction);
            }
            if(PMIC_INTF_DUAL_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pQACommHandle,
                                   &transaction);
            }
            if(ret != I2C_STS_SUCCESS)
            {
                return -1;
            }
        }

        /* NVM Instance */
        if(PMIC_NVM_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->nvmSlaveAddr;
            ret = I2C_transfer((I2C_Handle)pmicCorehandle->pCommHandle,
                                &transaction);
            if(ret != I2C_STS_SUCCESS)
            {
                return -1;
            }
        }
    }

    if(PMIC_INTF_SPI == pmicCorehandle->commMode)
    {
        if(0 !=
                 Lpm_pmicSpiStubRead(pmicCorehandle, pBuf, bufLen))
        {
            /* For Fault Injection Tests */
            if(1U == gLpmEnableFaultInjectionRead)
            {
                return -1;
            }

            return -1;
        }
    }


    /* Added for Branch Coverage */
    gLpmReadCount++;
    if((1U == gLpmEnableFaultInjectionRead) && (gLpmSkipReadCount == gLpmReadCount))
    {
        return -1;
    }

    return 0;
}

/*!
 * \brief   PMIC I2C write function.
 */
static int32_t Lpm_pmicRegWrite(Pmic_CoreHandle_t  *pmicCorehandle,
                           uint8_t             instType,
                           uint16_t            regAddr,
                           uint8_t            *pBuf,
                           uint8_t             bufLen)
{
    int8_t  ret = 0U;

    if((PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode) ||
       (PMIC_INTF_DUAL_I2C   == pmicCorehandle->commMode))
    {
        I2C_Transaction transaction;

        /* regAddr, 8-bit-Data, 8-bit-CRC(optional) => max 3 bytes
         * Taking a databuff of size 3 only as it is enough
         */
        uint8_t dataBuff[16U] = {0U};

        /* Initializes the I2C transaction structure with default values */
        I2C_transactionInit(&transaction);

        if(PMIC_MAIN_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->slaveAddr;
        }
        if(PMIC_QA_INST == instType)
        {
            transaction.slaveAddress = pmicCorehandle->qaSlaveAddr;
        }

        dataBuff[0U] = regAddr;
        memcpy(&dataBuff[1U], pBuf, bufLen);

        /* Control Byte followed by write bit */
        transaction.writeBuf     = dataBuff;
        transaction.writeCount   = bufLen + 1U;
        transaction.readCount    = 0U;
        transaction.readBuf      = NULL;

        /* Main I2c BUS */
        if(PMIC_MAIN_INST == instType)
        {
            ret = I2C_transfer((I2C_Handle)pmicCorehandle->pCommHandle,
                                &transaction);
            if(ret != I2C_STS_SUCCESS)
            {
                return -1;
            }
        }

        /* For WDOG QA I2C BUS */
        if(PMIC_QA_INST == instType)
        {
            if(PMIC_INTF_SINGLE_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pCommHandle,
                                   &transaction);
            }
            if(PMIC_INTF_DUAL_I2C == pmicCorehandle->commMode)
            {
                ret = I2C_transfer((I2C_Handle)
                                   pmicCorehandle->pQACommHandle,
                                   &transaction);
            }
            if(ret != I2C_STS_SUCCESS)
            {
                return -1;
            }
        }
    }

    if(PMIC_INTF_SPI == pmicCorehandle->commMode)
    {
        if(0 !=
                   Lpm_pmicSpiWrite(pmicCorehandle, pBuf, bufLen))
        {
            /* For Fault Injection Tests */
            if(1U == gLpmEnableFaultInjectionWrite)
            {
                return -1;
            }

            return -1;
        }
    }

    /* Added for Branch Coverage */
    gLpmWriteCount++;
    if((1U == gLpmEnableFaultInjectionWrite) && (gLpmSkipWriteCount == gLpmWriteCount))
    {
        return -1;
    }


    return 0;
}

/**
 *
 * \brief    Function to calculate CRC8 for given data using below values
 *           CRC Polynomial value: 0x07, Initial Value: 0xFF, Final Value: 0x00
 */
static int32_t Lpm_pmicGetCrc8Val(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFFU;
    uint8_t i;

    for(i = 0; i < len; i++)
    {
        crc = gLpmCrc8Tlb[data[i] ^ crc];
    }

    return crc;
}

/**
 * \brief  SPI stub function read operation to read PMIC registers
 *         using I2C interface
 */
static int32_t Lpm_pmicSpiStubRead(Pmic_CoreHandle_t  *pPmicCorehandle,
                           uint8_t            *pBuf,
                           uint8_t             bufLen)
{
    int32_t pmicStatus = 0;
    uint8_t instType = 0U;
    uint16_t regAddr = 0U;
    bool wdgopn = 0, crcopn = 0;
    uint8_t rxBuf[4U] = {0U};

    /* Check for WatchDog Operation */
    if(0U != (pBuf[1U] & (0x04 << 5U)))
    {
        wdgopn = true;
    }

    /* Check for CRC Operation */
    if(0U != (pBuf[1U] & (0x01 << 5U)))
    {
        crcopn = true;
    }

    /* Update register Address from spi buffer */
    regAddr = (uint16_t)pBuf[0U];
    bufLen = 1U;

    /* Find Instance type from wdg or crc operation */
    if(true == wdgopn)
    {
        instType = PMIC_QA_INST;
    }
    else if(true == crcopn)
    {
        instType = PMIC_NVM_INST;
    }
    else
    {
        instType = PMIC_MAIN_INST;
    }

    /* Increase buffer lenth 1 more to get CRC, if CRC is Enabled */
    if(true == pPmicCorehandle->crcEnable)
    {
        bufLen++;
    }

    /* Call PMIC read with I2C Instance */
    pmicStatus = Lpm_pmicRegRead(pPmicCoreHandleI2C,
                                   instType,
                                   regAddr,
                                   rxBuf,
                                   bufLen);

    /* Updating the Recieved Reg Value to SPI Buffer */
    pBuf[2U] = rxBuf[0U];

    /* Updating the Recieved CRC to SPI Buffer */
    if(true == pPmicCorehandle->crcEnable)
    {
        pBuf[3U] = Lpm_pmicGetCrc8Val(pBuf, 3U);
    }

    return pmicStatus;
}

/**
 * \brief  SPI stub function write operation to write PMIC registers
 *         using I2C interface
 */
static int32_t Lpm_pmicSpiWrite(Pmic_CoreHandle_t  *pPmicCorehandle,
                            uint8_t            *pBuf,
                            uint8_t             bufLen)
{
    int32_t  pmicStatus = 0;
    uint8_t  instType = 0U;
    bool     wdgopn = 0;
    uint16_t regAddr = 0U;
    uint8_t  txBuf[4U] = {0U};

    /* Update register Address from spi buffer */
    regAddr = (uint16_t)pBuf[0U];

    /* Check for WatchDog Operation */
    if(0U != (pBuf[1U] & (0x04 << 5U)))
    {
        wdgopn = true;
    }

    /* Find Instance type from wdg operation */
    if(true == wdgopn)
    {
        instType = PMIC_QA_INST;
    }
    else
    {
        instType = PMIC_MAIN_INST;
    }

    /* Updating the SPI Buffer Reg Value to I2C Buffer */
    txBuf[0U] = pBuf[2U];
    bufLen = 1U;

    /* Updating the Recieved CRC to SPI Buffer */
    if(true == pPmicCorehandle->crcEnable)
    {
        uint8_t crcbuf[4U] = {0U};
        if(true == wdgopn)
        {
            crcbuf[0U] = pPmicCoreHandleI2C->qaSlaveAddr << 1U;
        }
        else
        {
            crcbuf[0U] = pPmicCoreHandleI2C->slaveAddr << 1U;
        }
        crcbuf[1U] = regAddr;
        crcbuf[2U] = pBuf[2U];
        txBuf[1U] = Lpm_pmicGetCrc8Val(crcbuf, 3U);
        bufLen++;
    }

    /* Call PMIC write with I2C Instance */
    pmicStatus = Lpm_pmicRegWrite(pPmicCoreHandleI2C,
                                    instType,
                                    regAddr,
                                    txBuf,
                                    bufLen);

    return pmicStatus;
}
#endif

volatile uint32_t ccs_halt = 0x0;
static void Lpm_boardConfigurationForMainDomain(void)
{
    int32_t retVal;
    Sciclient_DefaultBoardCfgInfo_t boardCfgInfo;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Configuring Sciclient_board for MAIN domain\n");

    retVal = Sciclient_getDefaultBoardCfgInfo(&boardCfgInfo);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_getDefaultBoardCfgInfo() failed.\n");
    }

    Sciclient_BoardCfgPrms_t bootAppBoardCfgPrms = {
                                                    .boardConfigLow = (uint32_t)&bootAppBoardCfg,
                                                    .boardConfigHigh = 0,
                                                    .boardConfigSize = sizeof(bootAppBoardCfg),
                                                    .devGrp = DEVGRP_01
                                                   };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgPmPrms = {
                                                      .boardConfigLow = (uint32_t)boardCfgInfo.boardCfgLowPm,
                                                      .boardConfigHigh = 0,
                                                      .boardConfigSize = boardCfgInfo.boardCfgLowPmSize,
                                                      .devGrp = DEVGRP_01
                                                     };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgRmPrms = {
                                                      .boardConfigLow = (uint32_t)&bootAppBoardCfg_rm,
                                                      .boardConfigHigh = 0,
                                                      .boardConfigSize = sizeof(bootAppBoardCfg_rm),
                                                      .devGrp = DEVGRP_01
                                                     };
    Sciclient_BoardCfgPrms_t bootAppBoardCfgSecPrms = {
                                                       .boardConfigLow = (uint32_t)&bootAppBoardCfg_sec,
                                                       .boardConfigHigh = 0,
                                                       .boardConfigSize = sizeof(bootAppBoardCfg_sec),
                                                       .devGrp = DEVGRP_01
                                                      };

    if(ccs_halt == 0xDEADBEEF)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Connect CCS and break out of ccs_halt loop!!\n");
	    while(ccs_halt);
    }

    retVal = Sciclient_boardCfg(&bootAppBoardCfgPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_boardCfg() failed.\n");
    }
    retVal = Sciclient_boardCfgPm(&bootAppBoardCfgPmPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME 
                         "Sciclient_boardCfgPm() failed.\n");
    }
    retVal = Sciclient_boardCfgSec(&bootAppBoardCfgSecPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_boardCfgSec() failed.\n");
    }
    retVal = Sciclient_boardCfgRm(&bootAppBoardCfgRmPrms);
    if (retVal != CSL_PASS)
    {
         AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                         "Sciclient_boardCfgRm() failed.\n");
    }

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Doing board init...\n");
    Board_uartDeInit();
    Board_initCfg cfg = BOARD_INIT_PINMUX_CONFIG | BOARD_INIT_PLL_MAIN | BOARD_INIT_MODULE_CLOCK_MAIN | BOARD_INIT_DDR | BOARD_INIT_UART_STDIO;
    Board_init(cfg);
}

static void Lpm_setupI2CTransfer(I2C_Handle handle,  uint32_t slaveAddr,
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
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "\n Data Transfer failed. \n");
    }
}

static int32_t Lpm_enableMCU2MAINBridges(void)
{
    int32_t status;
    /* Enable both bridges, needed to establish communication between MCU and
       MAIN domain */
    /* These are in the MCU devgrp and hence we can enable then w/o passing the
       board configuration for the MAIN devgrp */
    status = Sciclient_pmSetModuleState(TISCI_DEV_WKUPMCU2MAIN_VD,
                                        TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                        TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                        0xFFFFFFFFU);
    status += Sciclient_pmSetModuleState(TISCI_DEV_MAIN2WKUPMCU_VD,
                                            TISCI_MSG_VALUE_DEVICE_SW_STATE_ON,
                                            TISCI_MSG_FLAG_AOP | TISCI_MSG_FLAG_DEVICE_RESET_ISO,
                                            0xFFFFFFFFU);

    return status;
}

static void Lpm_accessMainPeripherals(void)
{
    uint32_t readValDDR;
    uint32_t writeValDDR = 0xDEADBEEF;
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Writing to DDR...\n");
    HW_WR_REG32(DDR_TEST_ADDRESS, writeValDDR);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Reading from DDR...\n");
    readValDDR = HW_RD_REG32(DDR_TEST_ADDRESS);
    if(writeValDDR == readValDDR)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Read value matches the value written for DDR!\n");
    }

    uint32_t readValMSMC;
    uint32_t writeValMSMC = 0xC0DEC0DE;
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Writing to MSMC...\n");
    HW_WR_REG32(MSMC_TEST_ADDRESS, writeValMSMC);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Reading from MSMC...\n");
    readValMSMC = HW_RD_REG32(MSMC_TEST_ADDRESS);
    if(writeValMSMC == readValMSMC)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Read value matches the value written for MSMC!\n");
    }

    uint32_t readValMCAN;
    readValMCAN = HW_RD_REG32(MCAN_REV_REG);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Reading MAIN MCAN0 Revision register...\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "MAIN MCAN0 MCANSS_PID = 0x%x\n", readValMCAN);

    return;
}

static void Lpm_bringBackMAINDomain(void)
{
    int32_t status;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Configure WKUPMCU2MAIN and MAIN2WKUPMCU Bridges...\n");
    /* Enable WKUPMCU2MAIN and MAIN2WKUPMCU bridges,
       this needs to be done before sending the RM, PM, Sec and common board cfg
       for DEVGRP01 */
    status = Lpm_enableMCU2MAINBridges();

    /* Pass board config for MAIN domain i.e. DEVGRP01 */
    if(status == CSL_PASS)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "WKUPMCU2MAIN and MAIN2WKUPMCU Bridges configured successfully!\n");
        Lpm_boardConfigurationForMainDomain();
    }

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Trying to access MAIN domain peripherals...\n");
    Lpm_accessMainPeripherals();

    return;
}

static void Lpm_vtmMaxOutrgAlertDisableForTmpSens1to4()
{
    uint32_t vtmRegVal;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Disabling MAXT_OUTRG_EN for TMPSENS1:4 in MAIN domain!\n");

    /* TMPSENS1 */
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_1);
    /* un-set 11th bit MAXT_OUTRG_EN */
    vtmRegVal &= (~(1 << MAXT_OUTRG_EN_SHIFT));
    HW_WR_REG32(WKUP_VTM_TMPSENS_CTRL_1, vtmRegVal);
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_1);

    /* TMPSENS2 */
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_2);
    /* un-set 11th bit MAXT_OUTRG_EN */
    vtmRegVal &= (~(1 << MAXT_OUTRG_EN_SHIFT));
    HW_WR_REG32(WKUP_VTM_TMPSENS_CTRL_2, vtmRegVal);
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_2);

    /* TMPSENS3 */
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_3);
    /* un-set 11th bit MAXT_OUTRG_EN */
    vtmRegVal &= (~(1 << MAXT_OUTRG_EN_SHIFT));
    HW_WR_REG32(WKUP_VTM_TMPSENS_CTRL_3, vtmRegVal);
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_3);

    /* TMPSENS4 */
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_4);
    /* un-set 11th bit MAXT_OUTRG_EN */
    vtmRegVal &= (~(1 << MAXT_OUTRG_EN_SHIFT));
    HW_WR_REG32(WKUP_VTM_TMPSENS_CTRL_4, vtmRegVal);
    vtmRegVal = HW_RD_REG32(WKUP_VTM_TMPSENS_CTRL_4);

    return;
}

/* Bring back main_domain */
static uint32_t Lpm_mcuToActiveSwitch(void)
{
    uint32_t MAIN_VDOM_CTRL = 0x43018070;

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: MCU ONLY -> ACTIVE...\n");
    Lpm_pmicStateChangeMCUOnlyToActive();
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: MCU ONLY -> ACTIVE...Done\n");

    /* Disabling MAIN domain deep sleep isolation */
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Disabling MAIN domain deep sleep isolation...\n");
    CSL_REG32_WR(MAIN_VDOM_CTRL, 0x0);

    /* Enable the MAIN domain, till now we have only changed the PMIC state
       This will enable the modules in the MAIN domain */
    Lpm_bringBackMAINDomain();

    return 0;
}

/* Refer TISCI_MSG_SYS_RESET in TISCI user guide for more details
   http://software-dl.ti.com/tisci/esd/latest/2_tisci_msgs/pm/sysreset.html */
volatile uint32_t loopSwResetMainDomain = 0;
static void Lpm_swResetMainDomain(void)
{
    struct tisci_msg_sys_reset_req request;
    struct tisci_msg_sys_reset_resp response = {0};

    Sciclient_ReqPrm_t reqParam;
    Sciclient_RespPrm_t respParam;

    memset(&request, 0, sizeof(request));
    request.domain = 0x2; /* 0x2 corresponds to the MAIN domain */

    reqParam.messageType    = (uint16_t) TISCI_MSG_SYS_RESET;
    reqParam.flags          = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload    = (const uint8_t *) &request;
    reqParam.reqPayloadSize = (uint32_t) sizeof (request);
    reqParam.timeout        = (uint32_t) SCICLIENT_SERVICE_WAIT_FOREVER;
    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t) sizeof (response);

    /* For debug purpose */
    if(loopSwResetMainDomain == 0xDEADBEEF)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Connect CCS and change loopSwResetMainDomain to 0!\n");
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "After that the MAIN domain will be reset!\n");
    }

    while(loopSwResetMainDomain == 0xDEADBEEF);

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Entering into SW Reset using Sciclient call\n");

    int32_t retVal;
    retVal = Sciclient_service(&reqParam, &respParam);

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Performed SW Reset using Sciclient call\n");

    if ((respParam.flags & TISCI_MSG_FLAG_ACK) == 0) {
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                            "retVal = %d\nGOT NACK! resp flag = 0x%08x\n",
                            retVal, respParam.flags);
    }
}

#if !defined(PMIC_USE_DRV)
static void Lpm_pmicStateChangeMCUOnlyToActive(void)
{
    uint8_t dataToSlave[2];
    uint8_t dataFromSlave[2] = {0U, 0U};

    /* Read INT_TOP */
    dataToSlave[0] = 0x5A;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "INT_TOP = 0x%x\n", dataFromSlave[0]);

    /* Mask NSLEEP2 and NSLEEP1 bits */
    dataToSlave[0] = 0x7D;
    dataToSlave[1] = 0xC0;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Write CONFIG_1 = 0x%x\n", dataToSlave[1]);

    /* Change FSM_NS-LEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    dataToSlave[1] = 0x03;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Write FSM_NSLEEP_TRIGGERS = 0x%x\n", dataToSlave[1]);

    /* Un-Mask NSLEEP2 and 1 bit */
    dataToSlave[0] = 0x7D;
    dataToSlave[1] = 0x00;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Write CONFIG_1 = 0x%x\n", dataToSlave[1]);

    /* Buffer time to change state */
    TaskP_sleep(100);

    /* Read FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Read FSM_NSLEEP_TRIGGERS = 0x%x\n", dataFromSlave[0]);

    /*************** You should now back in ACTIVE mode ****************/

    return;
}

static int32_t Lpm_i2cInitPMIC(void)
{
    int32_t retVal = 0;
    I2C_Params i2cParams;

    /* Initialize i2c core instances */
    I2C_init();
    uint8_t i2c_instance = 0U;
    uint32_t baseAddr = CSL_WKUP_I2C0_CFG_BASE;

    I2C_HwAttrs i2cCfg;
    I2C_socGetInitCfg(i2c_instance, &i2cCfg);
    i2cCfg.baseAddr   = baseAddr;
    i2cCfg.enableIntr = 0U;
    I2C_socSetInitCfg(i2c_instance, &i2cCfg);

    /* Configured i2cParams.bitRate with standard I2C_100kHz */
    I2C_Params_init(&i2cParams);
    gLpmPmicI2cHandle = I2C_open(i2c_instance, &i2cParams);
    if(NULL == gLpmPmicI2cHandle)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"ERROR: I2C_open failed!\n");
        retVal = -1;
    }

    return retVal;
}

volatile uint32_t loopPMICStateChangeActiveToMCUOnly = 0;
static void Lpm_pmicStateChangeActiveToMCUOnly(void)
{
    /* Write 0x02 to FSM_NSLEEP_TRIGGERS register 
       This should happen before clearing the interrupts */

    /* If you clear the interrupts before you write the NSLEEP bits,
     * it will transition to S2R state.
     * This is because as soon as you write NSLEEP2 to 0x0,
     * the trigger is present to move to S2R state.
     * By setting the NSLEEP bits before you clear the interrupts,
     * you can configure both NSLEEP bits before the PMIC reacts to the change.
     */

    uint8_t dataToSlave[2];
    uint8_t dataFromSlave[2];

    if(loopPMICStateChangeActiveToMCUOnly == 0xFEEDFACE)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Connect CCS and change the loopPMICStateChangeActiveToMCUOnly to 0x0!!!!\n");
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "This will disconnect the JTAG interface too and you can only see the MCU running from UART prints!!!!\n");
    }

    while(loopPMICStateChangeActiveToMCUOnly == 0xFEEDFACE);

    /* Read INT_TOP */
    dataToSlave[0] = 0x5A;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "INT_TOP = 0x%x\n", dataFromSlave[0]);

    /* Read INT_STARTUP */
    dataToSlave[0] = 0x65;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "INT_STARTUP = 0x%x\n", dataFromSlave[0]);

    /* Read INT_GPIO */
    dataToSlave[0] = 0x63;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "INT_GPIO = 0x%x\n", dataFromSlave[0]);

    /* Read INT_GPIO1_8 */
    dataToSlave[0] = 0x64;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "INT_GPIO1_8 = 0x%x\n", dataFromSlave[0]);

    /* Read FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Read FSM_NSLEEP_TRIGGERS = 0x%x\n", dataFromSlave[0]);

    /**** Start changing states ****/

    /* Change FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    dataToSlave[1] = 0x02;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write FSM_NSLEEP_TRIGGERS = 0x%x\n", dataToSlave[1]);

    /* Read FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Read FSM_NSLEEP_TRIGGERS = 0x%x\n", dataFromSlave[0]);

    /* Clear INT_STARTUP */
    dataToSlave[0] = 0x65;
    dataToSlave[1] = 0x02;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write INT_STARTUP = 0x%x\n", dataToSlave[1]);

    /* Read INT_TOP */
    dataToSlave[0] = 0x5A;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "INT_TOP = 0x%x\n", dataFromSlave[0]);

    /* Clear INT_GPIO1_8 */
    dataToSlave[0] = 0x64;
    dataToSlave[1] = 0xC8;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write INT_STARTUP = 0x%x\n", dataToSlave[1]);

    /* Read INT_GPIO */
    dataToSlave[0] = 0x63;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "INT_GPIO = 0x%x\n", dataFromSlave[0]);

    /* Read INT_TOP */
    dataToSlave[0] = 0x5A;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "INT_TOP = 0x%x\n", dataFromSlave[0]);

    /* Clear INT_GPIO */
    dataToSlave[0] = 0x63;
    dataToSlave[1] = 0x02;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write INT_GPIO = 0x%x\n", dataToSlave[1]);

    /* Read INT_TOP */
    dataToSlave[0] = 0x5A;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Final Read INT_TOP = 0x%x\n", dataFromSlave[0]);

    /* Read FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Final Read FSM_NSLEEP_TRIGGERS = 0x%x\n", dataFromSlave[0]);

    /*************** You should now be in MCU only mode ****************/

    return;
}

volatile uint32_t loopPMICStateChangeActiveToIORetention = 0;
void Lpm_mostlyConfigureIORetention(void)
{
    uint8_t dataToSlave[2];

    /* Change FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    dataToSlave[1] = 0x03;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write FSM_NSLEEP_TRIGGERS = 0x%x\n", dataToSlave[1]);

    /* Clear INT_STARTUP */
    dataToSlave[0] = 0x65;
    dataToSlave[1] = 0x02;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write INT_STARTUP = 0x%x\n", dataToSlave[1]);

    /* Configure GPIO4_CONF */
    dataToSlave[0] = 0x34;
    dataToSlave[1] = 0xc0;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write GPIO4_CONF = 0x%x\n", dataToSlave[1]);

    /* Configure INT_GPIO1_8 (enable GPIO4 interrupt) */
    dataToSlave[0] = 0x64;
    dataToSlave[1] = 0x08;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write INT_GPIO1_8 = 0x%x\n", dataToSlave[1]);

    /* Configure MASK_GPIO1_8_FALL (configure GPIO4 falling edge interrupt) */
    dataToSlave[0] = 0x4F;
    dataToSlave[1] = 0xF7;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write MASK_GPIO1_8_FALL = 0x%x\n", dataToSlave[1]);

    /* Change FSM_I2C_TRIGGERS */
    dataToSlave[0] = 0x85;
    dataToSlave[1] = 0x40;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write FSM_I2C_TRIGGERS = 0x%x\n", dataToSlave[1]);
}

void Lpm_pmicStateChangeActiveToIORetention(void)
{
    /* Write 0x02 to FSM_NSLEEP_TRIGGERS register 
       This should happen before clearing the interrupts */

    /* If you clear the interrupts before you write the NSLEEP bits,
     * it will transition to S2R state.
     * This is because as soon as you write NSLEEP2 to 0x0,
     * the trigger is present to move to S2R state.
     * By setting the NSLEEP bits before you clear the interrupts,
     * you can configure both NSLEEP bits before the PMIC reacts to the change.
     */

    uint8_t dataToSlave[2];
    uint8_t dataFromSlave[2] = {0U, 0U};

    if(loopPMICStateChangeActiveToIORetention == 0xFEEDFACE)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Connect CCS and change the loopPMICStateChangeActiveToIORetention to 0x0!!!!\n");
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "This will disconnect the JTAG interface too and you can only see the MCU running from UART prints!!!!\n");
    }

    while(loopPMICStateChangeActiveToIORetention == 0xFEEDFACE);

   /* Change FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    dataToSlave[1] = 0x03;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write FSM_NSLEEP_TRIGGERS = 0x%x\n", dataToSlave[1]);

    /* Clear INT_STARTUP */
    dataToSlave[0] = 0x65;
    dataToSlave[1] = 0x02;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write INT_STARTUP = 0x%x\n", dataToSlave[1]);

    /* Configure GPIO4_CONF */
    dataToSlave[0] = 0x34;
    dataToSlave[1] = 0xc0;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write GPIO4_CONF = 0x%x\n", dataToSlave[1]);

    /* Configure INT_GPIO1_8 (enable GPIO4 interrupt) */
    dataToSlave[0] = 0x64;
    dataToSlave[1] = 0x08;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write INT_GPIO1_8 = 0x%x\n", dataToSlave[1]);

    /* Configure MASK_GPIO1_8_FALL (configure GPIO4 falling edge interrupt) */
    dataToSlave[0] = 0x4F;
    dataToSlave[1] = 0xF7;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write MASK_GPIO1_8_FALL = 0x%x\n", dataToSlave[1]);
	
	/* Pause after unmask GPIO4 but before read 0x3F for nINT */
    {
        int c;
        AppUtils_Printf(MSG_NORMAL, "Lpm_pmicStateChangeActiveToIORetention: Press enter key to send I2C commands to PMIC to enter low power mode...before STEP4A ");
        /* Using UART_scanFmt control will wait till user enters key
         Unmask of GPIO4 can be checked */
        UART_scanFmt("%d", &c);
    }

    /* Read back Register 0x3F from PMIC A, check bit 3 if low then exit (to be implemented) */	
	dataToSlave[0] = 0x3F;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 1, dataFromSlave, 1);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "PMICA_REG_0x3F = 0x%x\n", dataFromSlave[0]);
	
    /* clear nINT to allow PMIC go to IORET */
    {
        int c;
        AppUtils_Printf(MSG_NORMAL, "Lpm_pmicStateChangeActiveToIORetention: Press enter key to clear nINT ");
        /*Check the nINT beofre clear at this point */
        UART_scanFmt("%d", &c);
        dataToSlave[0] = 0x64;
        dataToSlave[1] = 0x08;
        Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    }
 
    /* Change FSM_I2C_TRIGGERS */
    dataToSlave[0] = 0x85;
    dataToSlave[1] = 0x40;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write FSM_I2C_TRIGGERS = 0x%x\n", dataToSlave[1]);
	
    /* Change FSM_I2C_TRIGGERS - PMICB */
    dataToSlave[0] = 0x85;
    dataToSlave[1] = 0x40;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x4C, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write FSM_NSLEEP_TRIGGERS = 0x%x\n", dataToSlave[1]);

    /* Pause before send I2C commands to PMIC to enter low power mode. */
    {
        int c;
        AppUtils_Printf(MSG_NORMAL, "Lpm_pmicStateChangeActiveToIORetention: Press enter key to send I2C commands to PMIC to enter low power mode...immediately before STEP6 (gotoioret)");
        /* Wait till user enter key Memory values before sending I@C commands can be checked */
        UART_scanFmt("%d", &c);
    }

    /* Change FSM_NSLEEP_TRIGGERS */
    dataToSlave[0] = 0x86;
    dataToSlave[1] = 0x00;
    Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Write FSM_NSLEEP_TRIGGERS = 0x%x\n", dataToSlave[1]);

    /* JWTBD clear nINT to allow PMIC go to IORET, if there is pre-existing interrupt on GPIO4 */
    {
        int c;
        AppUtils_Printf(MSG_NORMAL, "GPIO4 interrupt PENDING, device can not go into IORET: Press any key to clear... ");
        UART_scanFmt("%d", &c);
        AppUtils_Printf(MSG_NORMAL, "Clearing GPIO4 pending interrupt to allow entering IORET: Good Night!");
        dataToSlave[0] = 0x64;
        dataToSlave[1] = 0x08;
        Lpm_setupI2CTransfer(gLpmPmicI2cHandle, 0x48, dataToSlave, 2, NULL, 0);
        AppUtils_Printf(MSG_NORMAL, "signing off!"); //this line should not execute
    }

    return;
}
#endif


#if defined(PMIC_USE_DRV)
Pmic_CoreHandle_t *pPmicCoreHandle = NULL;

/*!
 * \brief   GPIO Unity Test App wrapper Function for LEO PMIC-A
 */
static int32_t Lpm_initPMIC(void)
{
    int32_t status                = 0;
    Pmic_CoreCfg_t pmicConfigData = {0U};

    /* Fill parameters to pmicConfigData */
    pmicConfigData.pmicDeviceType      = PMIC_DEV_LEO_TPS6594X;
    pmicConfigData.validParams        |= PMIC_CFG_DEVICE_TYPE_VALID_SHIFT;

    pmicConfigData.commMode            = PMIC_INTF_DUAL_I2C;
    pmicConfigData.validParams        |= PMIC_CFG_COMM_MODE_VALID_SHIFT;

    pmicConfigData.i2c1Speed            = PMIC_I2C_STANDARD_MODE;
    pmicConfigData.validParams         |= PMIC_CFG_I2C1_SPEED_VALID_SHIFT;

    pmicConfigData.i2c2Speed            = PMIC_I2C_STANDARD_MODE;
    pmicConfigData.validParams         |= PMIC_CFG_I2C2_SPEED_VALID_SHIFT;

#if defined(SOC_J721E)
    pmicConfigData.slaveAddr           = J721E_LEO_PMICA_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData.qaSlaveAddr         = J721E_LEO_PMICA_WDG_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigData.nvmSlaveAddr        = J721E_LEO_PMICA_PAGE1_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_NVMSLAVEADDR_VALID_SHIFT;
#endif

#if defined(SOC_J7200)
    pmicConfigData.slaveAddr           = J7VCL_LEO_PMICA_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_SLAVEADDR_VALID_SHIFT;

    pmicConfigData.qaSlaveAddr         = J7VCL_LEO_PMICA_WDG_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_QASLAVEADDR_VALID_SHIFT;

    pmicConfigData.nvmSlaveAddr        = J7VCL_LEO_PMICA_PAGE1_SLAVE_ADDR;
    pmicConfigData.validParams        |= PMIC_CFG_NVMSLAVEADDR_VALID_SHIFT;
#endif

    pmicConfigData.pFnPmicCommIoRead    = Lpm_pmicRegRead;
    pmicConfigData.validParams         |= PMIC_CFG_COMM_IO_RD_VALID_SHIFT;

    pmicConfigData.pFnPmicCommIoWrite   = Lpm_pmicRegWrite;
    pmicConfigData.validParams         |= PMIC_CFG_COMM_IO_WR_VALID_SHIFT;

    pmicConfigData.pFnPmicCritSecStart  = Lpm_pmicCriticalSectionStartFn;
    pmicConfigData.validParams         |= PMIC_CFG_CRITSEC_START_VALID_SHIFT;

    pmicConfigData.pFnPmicCritSecStop   = Lpm_pmicCriticalSectionStopFn;
    pmicConfigData.validParams         |= PMIC_CFG_CRITSEC_STOP_VALID_SHIFT;

    status = Lpm_pmicAppInit(&pPmicCoreHandle, &pmicConfigData);

    return status;
}

static void Lpm_pmicStateChangeActiveToMCUOnly(void)
{
    int32_t pmicStatus = 0;
    uint8_t pmicState  = PMIC_FSM_MCU_ONLY_STATE;

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP1_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP2_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetMissionState(pPmicCoreHandle, pmicState);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetMissionState failed!\n");
    }

    pmicStatus = Pmic_irqClrErrStatus(pPmicCoreHandle, PMIC_IRQ_ALL);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_irqClrErrStatus failed!\n");
    }

    return;
}

static void Lpm_pmicStateChangeMCUOnlyToActive(void)
{
    int32_t pmicStatus = 0;
    uint8_t pmicState  = PMIC_FSM_ACTIVE_STATE;

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP1_SIGNAL,
                                         PMIC_NSLEEPX_MASK);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP2_SIGNAL,
                                         PMIC_NSLEEPX_MASK);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetMissionState(pPmicCoreHandle, pmicState);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetMissionState failed!\n");
    }
    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP1_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_fsmSetNsleepSignalMask(pPmicCoreHandle,
                                         PMIC_NSLEEP2_SIGNAL,
                                         PMIC_NSLEEPX_UNMASK);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_fsmSetNsleepSignalMask failed!\n");
    }

    pmicStatus = Pmic_irqClrErrStatus(pPmicCoreHandle, PMIC_IRQ_ALL);
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Pmic_irqClrErrStatus failed!\n");
    }

    return;
}

#endif

static uint32_t Lpm_activeToMcuSwitch()
{
    uint32_t MAIN_VDOM_CTRL = 0x43018070;
    /* Issue a SW reset to the MAIN domain */
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Issueing a SW reset to the MAIN domain...\n");
    Lpm_swResetMainDomain();

    /* Enabling MAIN domain deep sleep isolation */
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Enabling MAIN domain deep sleep isolation...\n");
    CSL_REG32_WR(MAIN_VDOM_CTRL, 0x1);

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: ACTIVE -> MCU ONLY...\n");

    /* Change PMIC state from ACTIVE to MCU ONLY */

    /* NOTE: JTAG power also compromises after the transition, so you will not
       be able to connect JTAG in MCU ONLY mode */
    Lpm_pmicStateChangeActiveToMCUOnly();
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: ACTIVE -> MCU ONLY...Done\n");

    return 0;
}


uint32_t Lpm_activeToIoRetSwitch()
{
    /* Change PMIC state from ACTIVE to IO Retention */
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "PMIC STATE CHANGE: ACTIVE -> IO Retention\n");
    Lpm_pmicStateChangeActiveToIORetention();

    return 0;
}

uint32_t Lpm_pmicInit()
{
    uint32_t pmicStatus = 0;
#if defined(PMIC_USE_DRV)
    pmicStatus = Lpm_initPMIC();
#else
    pmicStatus = Lpm_i2cInitPMIC();
#endif
    if(0 != pmicStatus)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME"Lpm_initPMIC failed!\n");
    }
    return pmicStatus;
}

/* MCU Only task */
uint32_t Lpm_pmicApp()
{
    uint32_t status = 0;
    uint8_t input=0;
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "Inside MCU ONLY task!\n");

    /* Before entering MCU_ONLY mode we need to disable all VTM temp sensors in
       the MAIN domain - VTM_TMPSENS1-4 */
    /* Disabling the VTM MAXT_OUTRG_ALERT_THR */
    Lpm_vtmMaxOutrgAlertDisableForTmpSens1to4();

    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "STATE INFO :: CURRENTLY IN ACTIVE MODE!\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "LED LD5 should be ON\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Please measure TP133 and TP134!\n");
    AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                    "Expected values in ACTIVE mode:\nTP133: HIGH\nTP134: HIGH\n");

    while(1)
    {
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                        "Kindly unload Remoteproc modules from Linux. Type these commands in Linux CMD:\n   modprobe -r ti_k3_r5_remoteproc\n   modprobe -r ti_k3_dsp_remoteproc\n");
        AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "After modules have been removed, press 1 and enter\n");
        UART_scanFmt("%d", &input);
        if (1U == input)
        {
            /* Change state from ACTIVE to MCU ONLY */
            if (0 == status)
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                                "############################ ACTIVE -> MCU ONLY MODE ############################\n");
                /* The status is dummy currently */
                status = Lpm_activeToMcuSwitch();
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                                "######################### ACTIVE -> MCU ONLY MODE DONE ##########################\n");
            }
            else
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME">> ERROR :: Status not correct!!!\n");
            }

            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME 
                            "STATE INFO :: NOW IN MCU ONLY MODE!\n");
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "LED LD5 should be OFF\n");
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                            "Sleeping for 10s, please measure TP133 and TP134!\n");
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                            "Expected values in MCU ONLY mode:\nTP133: HIGH\nTP134: LOW\n");
            TaskP_sleep(10000);

            /* Change state from MCU ONLY to ACTIVE */
            if (0 == status)
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                                "############################ MCU ONLY -> ACTIVE MODE ############################\n");
                /* The status is dummy currently */
                status = Lpm_mcuToActiveSwitch();
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                                "######################### MCU ONLY -> ACTIVE MODE DONE ##########################\n");
            }
            else
            {
                AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME">> ERROR :: Status not correct!!!\n");
            }

            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                            "STATE INFO :: CURRENTLY IN ACTIVE MODE!\n");
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME "LED LD5 should be ON\n");
            AppUtils_Printf(MSG_NORMAL, MSG_APP_NAME
                            "Expected values in ACTIVE mode:\nTP133: HIGH\nTP134: HIGH\n");
            break;
    
        }
        else 
        {
            AppUtils_Printf(MSG_NORMAL, "Invalid input: ");
        }
    }
    return status;
}
