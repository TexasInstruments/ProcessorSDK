/*
 *   @file  mibspi_priv.c
 *
 *   @brief
 *      The file implements MIBSPI Driver with DMA.
 *
 *  The ti/drivers/mibspi/include/reg_mibspi.h has the register layer definitons for the
 *  MIBSPI Module.
 *
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 Texas Instruments, Inc.
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

/* Include Files */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ti/osal/osal.h>
#include <ti/osal/MemoryP.h>
#include <ti/drv/mibspi/MIBSPI.h>
#include "mibspi_priv.h"
#include "mibspi_trace_priv.h"
#include "mibspi_utils.h"

/**************************************************************************
 ************************** Local Definitions **********************************
 **************************************************************************/
 /* Flag to enalbe Params check */
#define SPI_PARAMS_CHECK

/* Support for multi icount in one transfer to achieve high throughput , this is only supported in blocking mode */
#define SPI_MULT_ICOUNT_SUPPORT

typedef enum Mibspi_DmaCtrlChType_tag
{
    MIBSPI_DMACTRL_CH_TX,
    MIBSPI_DMACTRL_CH_RX,
    MIBSPI_DMACTRL_CH_BOTH,
} Mibspi_DmaCtrlChType_e;

/**************************************************************************
 ************************* MibSPI Driver Functions Prototype ********************
 **************************************************************************/

/* Local Driver Functions */
#ifdef SPI_PARAMS_CHECK
static int32_t MIBSPI_validateParams(const MIBSPI_Params *params);
static int32_t MIBSPI_validateTransferParams(const MIBSPI_Transaction *transaction, const MibSpiDriver_Object *ptrMibSpiDriver);
#endif

static void MIBSPI_initSlave(MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params *params);
static void MIBSPI_initMaster(MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params *params);
static int32_t MIBSPI_openSlaveMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params *params);
static int32_t MIBSPI_openMasterMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params *params);
static void MIBSPI_enablePinSettings(CSL_mss_spiRegs  *ptrMibSpiReg, MIBSPI_PinMode pinMode, uint8_t chipSelectMask);
static void MIBSPI_setMasterClockRate(CSL_mss_spiRegs *ptrMibSpiReg, uint32_t clockSrcFreq, uint32_t desiredSpiClock);
static void MIBSPI_setResetMode(CSL_mss_spiRegs *ptrMibSpiReg, bool reset);
static void MIBSPI_writeDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem);
static uint32_t MIBSPI_readDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem);
static void MIBSPI_transferGroupEnable(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_transferGroupDisable(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_transferSetPStart(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group, uint8_t offset);
static uint32_t MIBSPI_checkTGComplete(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_enableLoopback(CSL_mss_spiRegs *ptrMibSpiReg, MibSpi_LoopBackType loopbacktype);
static void MIBSPI_disableLoopback(CSL_mss_spiRegs *ptrMibSpiReg);
static void MIBSPI_setClockPhasePolarity(volatile CSL_mss_spiRegs *ptrMibSpiReg, uint8_t clockFmt);
static void MIBSPI_SPIEnable(CSL_mss_spiRegs *ptrMibSpiReg);
static void MIBSPI_SPIDisable(CSL_mss_spiRegs *ptrMibSpiReg);
static void MIBSPI_dataTransfer
(
    MibSpiDriver_Object *ptrMibSpiDriver,
    uint8_t *srcData,
    uint8_t *dstData,
    uint16_t dataElemSize,
    uint8_t group
);

static void MIBSPI_ISR(uintptr_t arg);
static void MIBSPI_initTransactionState(Mibspi_transactionState_t *transactionState, 
                                        MIBSPI_Transaction     *transaction);
static void MIBSPI_resetTransactionState(Mibspi_transactionState_t *transactionState);
static uint32_t MIBSPI_getPolarity(MIBSPI_FrameFormat frameFormat);
static uint32_t MIBSPI_getPhase(MIBSPI_FrameFormat frameFormat);
static int32_t MIBSPI_getDrvStats(MIBSPI_Handle handle, MIBSPI_Stats *ptrStats);
static void MIBSPI_validateIPVersionInfo(const MibSpi_HwCfg* ptrHwCfg);
static void MIBSPI_getVersionInfo(const CSL_mss_spiRegs  *ptrMibSpiReg, MibSpi_VersionInfo *ver);
static void MIBSPI_enableErrorInterrupt(CSL_mss_spiRegs  *ptrMibSpiReg, uint32_t enableFlag);
static void MIBSPI_setErrorInterruptLevel(CSL_mss_spiRegs  *ptrMibSpiReg, uint32_t level);
#ifdef MIBSPI_DMA_ENABLE
static void MIBSPI_enableGroupInterrupt(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group, uint32_t intLine);
static void MIBSPI_disableGroupInterrupt(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group);
static void MIBSPI_dmaCtrlGroupConfig(CSL_mss_spiRegs *ptrMibSpiReg, uint16_t bufId, uint8_t iCount, uint8_t dmaCtrlGroup);
static void MIBSPI_dmaCtrlGroupStart(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t dmaCtrlGroup, Mibspi_DmaCtrlChType_e chType );
static void MIBSPI_dmaCtrlGroupDisable(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t dmaCtrlGroup);
#endif

/**************************************************************************
 ************************** Global Variables **********************************
 **************************************************************************/
/* NONE */

/**************************************************************************
 ************************* MibSPI Driver Local Functions ************************
 **************************************************************************/

#ifdef SPI_PARAMS_CHECK
/**
 *  @b Description
 *  @n
 *       SPI driver parameter validation
 *
 *   @param[in] params             SPI driver parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful          =0
 *             Failed              <0
 */
static int32_t MIBSPI_validateParams(const MIBSPI_Params *params)
{
    int32_t     retVal = (int32_t)0;
    uint8_t     index;
    uint8_t     ramBufOffset = 0U;


    /* Validate dataSize, only 8bits and 16bits are supported */
    if( (params->dataSize != 8U) && (params->dataSize != 16U))
    {
        retVal = MIBSPI_STATUS_ERROR;
        goto Exit;
    }


    /* Validate bitRate */
    if(params->mode == MIBSPI_MASTER)
    {
        if(params->u.masterParams.bitRate == 0U)
        {
            retVal = MIBSPI_STATUS_ERROR;
            goto Exit;

        }
        if( (params->u.masterParams.numSlaves == 0U) || (params->u.masterParams.numSlaves > MIBSPI_SLAVE_MAX) )
        {
            retVal = MIBSPI_STATUS_ERROR;
            goto Exit;
        }

        /* Validate slave profile configuraiton */
        for(index = 0; index < params->u.masterParams.numSlaves; index++)
        {
            const MIBSPI_SlaveProfile    *ptrSlaveProf;

            /* Get the pointer to the slave profile */
            ptrSlaveProf = &params->u.masterParams.slaveProf[index];

            /* Validate CS signal number */
            if(ptrSlaveProf->chipSelect >= MIBSPI_MAX_CS)
            {
                retVal = MIBSPI_STATUS_ERROR;
                goto Exit;
            }

            if(ptrSlaveProf->ramBufLen > MIBSPI_RAM_MAX_ELEM)
            {
                retVal = MIBSPI_STATUS_ERROR;
                goto Exit;
            }

            ramBufOffset += ptrSlaveProf->ramBufLen;

        }

        /* Validate total RAM Elements exceed the size of MibSPI RAM */
        if(ramBufOffset > MIBSPI_RAM_MAX_ELEM)
        {
            retVal = MIBSPI_STATUS_ERROR;
            goto Exit;
        }
    }
    else
    {
        /* Validate CS signal number */
        if(params->u.slaveParams.chipSelect >= MIBSPI_MAX_CS)
        {
            retVal = MIBSPI_STATUS_ERROR;
            goto Exit;
        }
    }

    /* Validate transfer mode. */
    if(params->transferMode == MIBSPI_MODE_CALLBACK)
    {
        if(params->transferCallbackFxn == NULL)
        {
            retVal = MIBSPI_STATUS_ERROR;
            goto Exit;
        }
    }

    /* Validate DMA driver handle */
    if(params->dmaEnable == (uint8_t)1U)
    {
        if(params->dmaHandle == NULL)
        {
            retVal = MIBSPI_STATUS_ERROR;
        }
    }

Exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *       SPI driver parameter validation
 *
 *   @param[in] params             SPI driver parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful          =0
 *             Failed              <0
 */
static int32_t MIBSPI_validateTransferParams(const MIBSPI_Transaction *transaction, const MibSpiDriver_Object *ptrMibSpiDriver)
{
    MibSpi_HwCfg    const  *hwAttrs;
    int32_t status = MIBSPI_STATUS_SUCCESS;
    uintptr_t  key;

    hwAttrs = ptrMibSpiDriver->ptrHwCfg;

    /* Check the transaction arguments */
    if (transaction->count == 0U)
    {
        status = MIBSPI_STATUS_ERROR;
    }

    /* Sanity check of parameters */
    /* Both Tx and Rx cannot be dummy transaction */
    if((transaction->txBuf == NULL) && (transaction->rxBuf == NULL))
    {
        status = MIBSPI_STATUS_ERROR;
    }

    if((transaction->slaveIndex >= MIBSPI_SLAVE_MAX)
       ||
       (transaction->slaveIndex >= hwAttrs->numDmaReqLines))
    {
        status = MIBSPI_STATUS_ERROR;
    }

    if(ptrMibSpiDriver->params.dataSize == 16U)
    {
        if(transaction->count % 2U != 0)
        {
            status = MIBSPI_STATUS_ERROR;
        }
    }

#ifndef SPI_MULT_ICOUNT_SUPPORT
    {
        uint16_t   dataLength;

        if(ptrMibSpiDriver->params.dataSize == 16U)
        {
            /* MIBSPI reads in 2 bytes format */
            dataLength = (uint16_t)transaction->count >> 1U;
        }
        else
        {
            dataLength = transaction->count;
        }

        /* Check data elements */
        if(dataLength > hwAttrs->mibspiRamSize)
        {
            status = MIBSPI_STATUS_ERROR;
        }
    }
#endif

    /* Check if a transfer is in progress */
    key = HwiP_disable();

    if ( ptrMibSpiDriver->transactionState.transaction != NULL)
    {
        status = MIBSPI_STATUS_ERROR;
    }

    /* Transfer is in progress */
    HwiP_restore(key);
    return status;
}

#endif

/**
 *  @b Description
 *  @n
 *       Enable pin settings for SPI driver
 *
 *   @param[in] ptrMibSpiReg             Pointer to the SPI driver Register Base Address
 *   @param[in] pinMode                  Pin mode Configuration
 *   @param[in] chipSelectMask           Bit Mask for enabled Chip select
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Not Applicable
 */
static void MIBSPI_enablePinSettings(CSL_mss_spiRegs  *ptrMibSpiReg, MIBSPI_PinMode pinMode, uint8_t chipSelectMask)
{
    uint32_t regVal = 0;

    switch(pinMode)
    {
        /* 3pin setting, enable CLK, SIMO, SOMI */
        case MIBSPI_PINMODE_3PIN:
            /* SPIPC0 Register: Set Port pins to functional */
            CSL_FINS(regVal, SPI_SPIPC0_CLKFUN, 1U);
            CSL_FINS(regVal, SPI_SPIPC0_SIMOFUN0, 1U);
            CSL_FINS(regVal, SPI_SPIPC0_SOMIFUN0, 1U);
            ptrMibSpiReg->SPIPC0 = regVal;
            break;

        /* 4pin with CS setting, enable CLK, SIMO, SOMI and CSx */
        case MIBSPI_PINMODE_4PIN_CS:
            /* SPIPC0 Register: Set Port pins to functional */
            CSL_FINS(regVal, SPI_SPIPC0_SCSFUN, (uint32_t)chipSelectMask);
            CSL_FINS(regVal, SPI_SPIPC0_CLKFUN, 1U);
            CSL_FINS(regVal, SPI_SPIPC0_SIMOFUN0, 1U);
            CSL_FINS(regVal, SPI_SPIPC0_SOMIFUN0, 1U);

            ptrMibSpiReg->SPIPC0 = regVal;         /* enable SOMI */
            break;

        default:
            Mibspi_assert(0);
            break;
    }

    /*  SPIPC7 Register: Set Port Pullup/Pulldown control: 0 to enable, 1 to disable  */
    regVal = ptrMibSpiReg->SPIPC7;
    CSL_FINS(regVal, SPI_SPIPC7_SOMIPDIS0, 0);
    CSL_FINS(regVal, SPI_SPIPC7_SIMOPDIS0, 0);
    CSL_FINS(regVal, SPI_SPIPC7_CLKPDIS, 0);
    CSL_FINS(regVal, SPI_SPIPC7_ENAPDIS, 0);
    CSL_FINS(regVal, SPI_SPIPC7_SCSPDIS, 0);
    ptrMibSpiReg->SPIPC7 = regVal;

    /*  SPIPC8 Register: Set Port Pullup/Pulldown value: 0 to pulldown, 1 to pullup  */
    regVal = ptrMibSpiReg->SPIPC8;
    CSL_FINS(regVal, SPI_SPIPC8_SCSPSEL, 0x1U);
    CSL_FINS(regVal, SPI_SPIPC8_ENAPSEL, 1U);
    CSL_FINS(regVal, SPI_SPIPC8_CLKPSEL, 1U);
    CSL_FINS(regVal, SPI_SPIPC8_SIMOPSEL0, 1U);
    CSL_FINS(regVal, SPI_SPIPC8_SOMIPSEL0, 1U);
    ptrMibSpiReg->SPIPC8 = regVal;

    return;
}

/**
 *  @b Description
 *  @n
 *       Change SPI master clock prescaler
 *
 *   @param[in] ptrMibSpiReg            Pointer to the SPI driver Register Base Address
 *   @param[in] clockSrcFreq            SPI Module source clock frequency
 *   @param[in] desiredSpiClock         Desired SPI clock rate on CLOCK pin
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_setMasterClockRate(CSL_mss_spiRegs *ptrMibSpiReg, uint32_t clockSrcFreq, uint32_t desiredSpiClock)
{
    uint8_t     clockDivisor;

    clockDivisor = (uint8_t)(clockSrcFreq / desiredSpiClock);

    /* Put MibSpi module in reset */
    CSL_FINS(ptrMibSpiReg->SPIGCR1, SPI_SPIGCR1_SPIEN, 0U);

    /* Set MibSpi clockDivisor */
    CSL_FINS(ptrMibSpiReg->SPIFMT[0],SPI_SPIFMT_PRESCALE, (uint32_t)((uint32_t)clockDivisor - 1U));

    /* Finally start MIBSPI1 */
    CSL_FINS(ptrMibSpiReg->SPIGCR1, SPI_SPIGCR1_SPIEN, 1U);

}

/**
 *  @b Description
 *  @n
 *       This function brings Mibspi module out of reset.
 *
 *   @param[in] ptrMibSpiReg    Pointer to the SPI driver Register Base Address
 *   @param[in] reset           true - mibspi module will be set in reset mode.
 *                              false - it brings MibSPI out of reset mode.
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_setResetMode(CSL_mss_spiRegs *ptrMibSpiReg, bool reset)
{
    if(reset == true)
    {
        /* Set MibSpi in Reset mode */
        CSL_FINS(ptrMibSpiReg->SPIGCR0, SPI_SPIGCR0_NRESET, 0U);
    }
    else
    {

        /* Bring MibSpi out of Reset mode */
        CSL_FINS(ptrMibSpiReg->SPIGCR0, SPI_SPIGCR0_NRESET, 1U);
    }
}

/**
 *  @b Description
 *  @n
 *      This function Enabe SPI and set SPI in MibSPI mode.
 *
 *   @param[in] ptrMibSpiReg        SPI module base address
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_SPIEnable(CSL_mss_spiRegs *ptrMibSpiReg)
{
    /* Acitvate SPI */
    CSL_FINS(ptrMibSpiReg->SPIGCR1, SPI_SPIGCR1_ENABLE, 1U);

    /* Enable MibSpi multibuffered mode and enable buffer RAM */
    CSL_FINS(ptrMibSpiReg->MIBSPIE, SPI_MIBSPIE_MSPIENA, 1U);
}

/**
 *  @b Description
 *  @n
 *      This function Disable SPI and set SPI in SPI mode.
 *
 *   @param[in] ptrMibSpiReg        SPI module base address
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_SPIDisable(CSL_mss_spiRegs *ptrMibSpiReg)
{
    /* Disable MibSpi multibuffered mode and enable buffer RAM */
    CSL_FINS(ptrMibSpiReg->MIBSPIE, SPI_MIBSPIE_MSPIENA, 0U);

    /* De-acitvate SPI */
    CSL_FINS(ptrMibSpiReg->SPIGCR1, SPI_SPIGCR1_SPIEN, 0U);
}

/**
 *  @b Description
 *  @n
 *      This function initiates a transfer for the specified transfer group.
 *
*   @param[in] ptrMibSpiReg        SPI module base address
*   @param[in] group               Transfer group (0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_transferGroupEnable(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group)
{
    /* Enable Transfer group */
    CSL_FINS(ptrMibSpiReg->TGCTRL[group],SPI_TGCTRL_TGENA, 1U);
}


/**
 *  @b Description
 *  @n
 *      This function initiates a transfer for the specified transfer group.
 *
 *   @param[in] ptrMibSpiReg        SPI module base address
 *   @param[in] group               Transfer group (0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_transferGroupDisable(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group)
{
    /* Disable Transfer group */
    CSL_FINS(ptrMibSpiReg->TGCTRL[group],SPI_TGCTRL_TGENA, 0U);

    /* Transfer is completed , disable SPI */
    MIBSPI_SPIDisable(ptrMibSpiReg);
}

/**
 *  @b Description
 *  @n
 *      This function sets the start offset of a transfer for the specified transfer group.
 *
 *   @param[in] ptrMibSpiReg       SPI module base address
 *   @param[in] group              Transfer group
 *   @param[in] offset             RAM offset For the TG group
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_transferSetPStart(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group, uint8_t offset)
{
    CSL_FINS(ptrMibSpiReg->TGCTRL[group],SPI_TGCTRL_PSTART, (uint32_t)offset);
}

/**
 *  @b Description
 *  @n
 *      This function checks to see if the transfer for the specified transfer group
 *   has finished.
 *
 *   @param[in] ptrMibSpiReg       SPI module base address
 *   @param[in] group              Transfer group
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    =1     Transfer is finished.
 *             =0     Transfer is not finished.
 */
static uint32_t MIBSPI_checkTGComplete(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group)
{
    uint32_t status = 0U;
    uint32_t groupMask;
    uint32_t intFlagReady = 0U;

    /* Get the bit mask of the group */
    groupMask = (uint32_t)1U << group;

    /* Read TGINT flag */
    intFlagReady = CSL_FEXT(ptrMibSpiReg->TGINTFLAG,SPI_TGINTFLAG_INTFLGRDY);
    if ( intFlagReady & groupMask)
    {
        /* Transfer finished, clear the corresponding flag */
        CSL_FINS(ptrMibSpiReg->TGINTFLAG,SPI_TGINTFLAG_INTFLGRDY, groupMask);
        status = 1U;
    }

    return (status);
}

/**
 *  @b Description
 *  @n
 *      This function enables the Loopback mode for self test. Loopback is SPI master only feature.
 *
 *   @param[in] ptrMibSpiReg            Mibspi module base address
 *   @param[in] loopbacktype            Digital or Analog
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_enableLoopback(CSL_mss_spiRegs *ptrMibSpiReg, MibSpi_LoopBackType loopbacktype)
{
    uint32_t regVal;

    /* Put MibSpi module in reset */
    CSL_FINS(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_SPIEN, 0U);

    regVal = ptrMibSpiReg->IOLPBKTSTCR;
    /* Set Loopback either in Analog or Digital Mode */
    CSL_FINS(regVal,SPI_IOLPBKTSTCR_LPBKTYPE, loopbacktype);

    /* For IOLPBK in SLave mode, set loopback enable in MIBSPI_transaction as it immediately trigger TG0 */
    if ((loopbacktype == MIBSPI_LOOPBK_ANALOG)
        &&
        (CSL_FEXT(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_MASTER) == CSL_SPI_SPIGCR1_MASTER_SLAVE))
    {
        CSL_FINS(regVal,SPI_IOLPBKTSTCR_IOLPBKTSTENA, 0x5U);
    }
    else
    {
        /* Enable Loopback  */
        CSL_FINS(regVal,SPI_IOLPBKTSTCR_IOLPBKTSTENA, 0xAU);
    }
    if (loopbacktype == MIBSPI_LOOPBK_ANALOG)
    {
        CSL_FINS(regVal,SPI_IOLPBKTSTCR_RXPENA, 0x1U);
    }

    ptrMibSpiReg->IOLPBKTSTCR = regVal;
    /* Restart MIBSPI1 */
    
    regVal = ptrMibSpiReg->SPIGCR1;
    CSL_FINS(regVal,SPI_SPIGCR1_SPIEN, 1U);

    if (CSL_FEXT(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_MASTER) == CSL_SPI_SPIGCR1_MASTER_MASTER)
    {
        CSL_FINS(regVal,SPI_SPIGCR1_LOOPBACK, 1U);
    }
    ptrMibSpiReg->SPIGCR1 = regVal;
}

/**
 *  @b Description
 *  @n
 *      This function disable the Loopback mode.
 *
 *   @param[in] ptrMibSpiReg        Mibspi module base address
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_disableLoopback(CSL_mss_spiRegs *ptrMibSpiReg)
{
    /* Put MibSpi module in reset */
    CSL_FINS(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_SPIEN, 0U);

    /* Disable Loopback either in Analog or Digital Mode */
    CSL_FINS(ptrMibSpiReg->IOLPBKTSTCR,SPI_IOLPBKTSTCR_IOLPBKTSTENA, 0x5U);

    /* Restart MIBSPI1 */
    CSL_FINS(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_SPIEN, 1U);
}

/**
 *  @b Description
 *  @n
 *      This function sets the Polarity and phase for MibSpi as requested.
 *
 *   @param[in] ptrMibSpiReg      MibSpi register base address
 *   @param[in] clockFmt          MibSpiSPI functional mode (clock/polarity)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_setClockPhasePolarity(volatile CSL_mss_spiRegs *ptrMibSpiReg, uint8_t clockFmt)
{
    uint32_t regVal;
    /* Put MibSpi module in reset */
    CSL_FINS(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_SPIEN, 0U);

    /* Set MibSpi Slave functional Mode Clock/polarity */
    regVal = ptrMibSpiReg->SPIFMT[0];
    CSL_FINS(regVal, SPI_SPIFMT_PHASE, MIBSPI_getPhase((MIBSPI_FrameFormat)clockFmt));   /* PHASE */
    CSL_FINS(regVal, SPI_SPIFMT_POLARITY, MIBSPI_getPolarity((MIBSPI_FrameFormat)clockFmt));   /* POLARITY */
    ptrMibSpiReg->SPIFMT[0] = regVal;

    /* Finally start MIBSPI1 */
    CSL_FINS(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_SPIEN, 1U);
}

#ifdef MIBSPI_DMA_ENABLE
/**
 *  @b Description
 *  @n
 *      This function enables the transfer group finished interrupt.
 *
 *   @param[in] ptrMibSpiReg   SPI module base address
 *   @param[in] group          Transfer group
 *   @param[in] intLine        Interrupt Line - 0->INT0, 1->INT1
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_enableGroupInterrupt(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group, uint32_t intLine)
{
    if(intLine == 0)
    {
        /* INT0 interrupt */
        CSL_FINS(ptrMibSpiReg->TGITLVCR,SPI_TGITLVCR_CLRINTLVLRDY, (uint32_t)1U << group);
    }
    else
    {
        /* INT1 interrupt */
        CSL_FINS(ptrMibSpiReg->TGITLVST,SPI_TGITLVST_SETINTLVLRDY, (uint32_t)1U << group);
    }

    /* Enable interrupt */
    CSL_FINS(ptrMibSpiReg->TGITENST,SPI_TGITENST_SETINTENRDY, (uint32_t)1U << group);
}

/**
 *  @b Description
 *  @n
 *      This function disables the transfer group finished interrupt.
 *
 *   @param[in] ptrMibSpiReg       SPI module base address
 *   @param[in] group              Transfer group
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void MIBSPI_disableGroupInterrupt(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t group)
{
    /* Disable interrupt */
    CSL_FINS(ptrMibSpiReg->TGITENCR,SPI_TGITENCR_CLRINTENRDY, (uint32_t)1U << group);
}

/**
 *  @b Description
 *  @n
 *      This function set the MIBSPI DMA channel Control Register depending on given
 *  input parameters
 *
 *   @param[in] ptrMibSpiReg      MibSpi Register base address
 *   @param[in] bufId             Buffer id
 *   @param[in] iCount            icount configuration
 *   @param[in] dmaCtrlGroup      MibSPI DMA Control group(0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval   None.
 */
static void MIBSPI_dmaCtrlGroupConfig(CSL_mss_spiRegs *ptrMibSpiReg, uint16_t bufId, uint8_t iCount, uint8_t dmaCtrlGroup)
{
    uint32_t regVal = 0;

    /* Setting Transmit channel DMA request */
    CSL_FINS(regVal,SPI_DMACTRL_TXDMA_MAP, (uint32_t)dmaCtrlGroup * 2U);
    CSL_FINS(regVal,SPI_DMACTRL_TXDMAENA, 0U);
    CSL_FINS(regVal,SPI_DMACTRL_BUFID, (uint32_t)bufId);
    CSL_FINS(regVal,SPI_DMACTRL_ICOUNT, (uint32_t)iCount);
    CSL_FINS(regVal,SPI_DMACTRL_ONESHOT, 1U);

    /* Setting Receive channel DMA request */
    CSL_FINS(regVal,SPI_DMACTRL_RXDMA_MAP, (uint32_t)dmaCtrlGroup * 2U + 1);
    CSL_FINS(regVal,SPI_DMACTRL_RXDMAENA, 0U);
    CSL_FINS(regVal,SPI_DMACTRL_BUFID, (uint32_t)bufId);
    CSL_FINS(regVal,SPI_DMACTRL_ICOUNT, (uint32_t)iCount);
    CSL_FINS(regVal,SPI_DMACTRL_ONESHOT, 1U);

    ptrMibSpiReg->DMACTRL[dmaCtrlGroup] = regVal;
    return;
}

static void MIBSPI_dmaCtrlGroupStart(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t dmaCtrlGroup, Mibspi_DmaCtrlChType_e chType )
{
    uint32_t regVal = ptrMibSpiReg->DMACTRL[dmaCtrlGroup];

    if ((chType == MIBSPI_DMACTRL_CH_TX)
        ||
        (chType == MIBSPI_DMACTRL_CH_BOTH))
    {
        /* Setting Transmit channel DMA request */
        CSL_FINS(regVal,SPI_DMACTRL_TXDMAENA, 1U);
    }
    if ((chType == MIBSPI_DMACTRL_CH_RX)
        ||
        (chType == MIBSPI_DMACTRL_CH_BOTH))
    {
        /* Setting Receive channel DMA request */
        CSL_FINS(regVal,SPI_DMACTRL_RXDMAENA, 1U);
    }
    ptrMibSpiReg->DMACTRL[dmaCtrlGroup] = regVal;
    return;
}

/**
 *  @b Description
 *  @n
 *      This function reset the MIBSPI TX and RX DMA channel Control Register
 *
 *   @param[in] ptrMibSpiReg      MibSpi Register base address
 *   @param[in] dmaCtrlGroup      MibSPI DMA Control group(0..7)
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval   None.
 */
static void MIBSPI_dmaCtrlGroupDisable(CSL_mss_spiRegs *ptrMibSpiReg, uint8_t dmaCtrlGroup)
{
    /* Get MibSpi Register & Ram Base address */
    uint32_t regVal = ptrMibSpiReg->DMACTRL[dmaCtrlGroup];

    CSL_FINS(regVal, SPI_DMACTRL_ONESHOT, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_BUFID, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_RXDMA_MAP, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_TXDMA_MAP, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_RXDMAENA, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_TXDMAENA, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_NOBRK, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_ICOUNT, 0U);
    CSL_FINS(regVal, SPI_DMACTRL_BUFID7, 0U);
    ptrMibSpiReg->DMACTRL[dmaCtrlGroup] = regVal;

}
#endif

 /**
 *  @b Description
 *  @n
 *       Initializes the MIBSPI as a SPI master
 *
 *   @param[in] ptrHwCfg           Pointer to the SPI driver hardward Configuration
 *   @param[in] params             SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_initMaster(MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params * params)
{
    CSL_mibspiRam           *ptrMibSpiRam;
    CSL_mss_spiRegs          *ptrMibSpiReg;
    uint32_t            flag;
    uint32_t            index;
    uint8_t             csnr = 0;
    uint8_t             chipSelectMask;
    uint8_t             ramBufOffset = 0;
    uint32_t            regVal;

    /* Get Register and RAM base */
    ptrMibSpiRam  = ptrHwCfg->ptrMibSpiRam;
    ptrMibSpiReg  = ptrHwCfg->ptrSpiRegBase;

    /* Bring MIBSPI out of reset */
    ptrMibSpiReg->SPIGCR0 = 0U;
    ptrMibSpiReg->SPIGCR0 = 1U;

    /* Enable MibSpi multibuffered mode and enable buffer RAM */
    CSL_FINS(ptrMibSpiReg->MIBSPIE,SPI_MIBSPIE_MSPIENA, 1U);

    /* Wait for buffer initialization complete before accessing MibSPI Ram */
    while(1)
    {
        flag = CSL_FEXT(ptrMibSpiReg->SPIFLG,SPI_SPIFLG_BUFINITACTIVE);
        if(flag == 0)
        {
            break;
        }
    }

    MIBSPI_socMemInit(ptrHwCfg->mibspiInstId);
    /* Set MibSpi master mode and clock configuration */
    regVal = ptrMibSpiReg->SPIGCR1;
    CSL_FINS(regVal,SPI_SPIGCR1_MASTER, CSL_SPI_SPIGCR1_MASTER_MASTER);
    CSL_FINS(regVal,SPI_SPIGCR1_CLKMOD, CSL_SPI_SPIGCR1_CLKMOD_INTERNAL);
    ptrMibSpiReg->SPIGCR1 = regVal;

    /* SPIENV pin pulled high when not active */
    CSL_FINS(ptrMibSpiReg->SPIINT0,SPI_SPIINT0_ENABLEHIGHZ, 0U);

    /* Delays Configuration: this master only configuraion, hence set it to all zeros */
    if( (params->u.masterParams.c2tDelay != 0U) ||
       (params->u.masterParams.t2cDelay != 0U) )
    {
        regVal = ptrMibSpiReg->SPIDELAY;
        CSL_FINS(regVal, SPI_SPIDELAY_C2TDELAY, (uint32_t)params->u.masterParams.c2tDelay);
        CSL_FINS(regVal, SPI_SPIDELAY_T2CDELAY, (uint32_t)params->u.masterParams.t2cDelay);
        ptrMibSpiReg->SPIDELAY = regVal;
    }
    else
    {
        ptrMibSpiReg->SPIDELAY = 0x0U;
    }

    /* Set Data Format 0 */
    regVal = ptrMibSpiReg->SPIFMT[0];
    CSL_FINS(regVal, SPI_SPIFMT_PHASE, MIBSPI_getPhase(params->frameFormat));   /* PHASE */
    CSL_FINS(regVal, SPI_SPIFMT_POLARITY, MIBSPI_getPolarity(params->frameFormat));   /* POLARITY */
    CSL_FINS(regVal, SPI_SPIFMT_WDELAY, (uint32_t) params->u.masterParams.wDelay);  /* WDELAY */
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE,  0U);                   /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_SHIFTDIR, (uint32_t) params->shiftFormat);  /* SHIFTDIR */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t) params->dataSize);        /* CHARlEN */
    ptrMibSpiReg->SPIFMT[0] = regVal;

    /* Set Data Format 1 */
    regVal = ptrMibSpiReg->SPIFMT[1];
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE, 18U);                 /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t) params->dataSize);  /* CHARlEN */
    ptrMibSpiReg->SPIFMT[1] = regVal;

    /* Set Data Format 2 */
    regVal = ptrMibSpiReg->SPIFMT[2];
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE, 18U);                 /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t)params->dataSize);        /* CHARlEN */
    ptrMibSpiReg->SPIFMT[2] = regVal;

    /* Set Data Format 3 */
    regVal = ptrMibSpiReg->SPIFMT[3];
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE, 18U);                 /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t)params->dataSize);        /* CHARlEN */
    ptrMibSpiReg->SPIFMT[3] = regVal;

    /* Set Default Chip Select pattern: 1- chip select is set to "1" when SPI is IDLE
       Debug Note: Only CS0 can be set to 1 */
    CSL_FINS(ptrMibSpiReg->SPIDEF,SPI_SPIDEF_CSDEF0, 0xFFU);

    /* Enable ECC if enabled */
    if(params->eccEnable)
    {
        /* Enable ECC detection and signal bit Error correction */
        regVal = ptrMibSpiReg->PAR_ECC_CTRL;
        CSL_FINS(regVal, SPI_PAR_ECC_CTRL_EDEN, 0xAU);
        CSL_FINS(regVal, SPI_PAR_ECC_CTRL_EDAC_MODE, 0xAU);
        CSL_FINS(regVal, SPI_PAR_ECC_CTRL_SBE_EVT_EN, 0xAU);
        ptrMibSpiReg->PAR_ECC_CTRL = regVal;
    }

    for (index = 0; index < params->u.masterParams.numSlaves; index++)
    {
        /* Initialize transfer groups for number of slaves connected to SPI master */
        regVal = ptrMibSpiReg->TGCTRL[index];
        CSL_FINS(regVal, SPI_TGCTRL_ONESHOT, 1U);  /* Oneshot trigger */
        CSL_FINS(regVal, SPI_TGCTRL_TRIGEVT, 7U);  /* Trigger event : Always */
        CSL_FINS(regVal, SPI_TGCTRL_TRIGSRC, 0U);  /* Trigger source : disabled */
        CSL_FINS(regVal, SPI_TGCTRL_PSTART, 0U);   /* TG start address : 0 */
        ptrMibSpiReg->TGCTRL[index] = regVal;
    }

    /* Initialize transfer groups end pointer */
    CSL_FINS(ptrMibSpiReg->LTGPEND,SPI_LTGPEND_LPEND, 0xFFU);

    /* Initialize TX Buffer Ram */
    for (index = 0; index < params->u.masterParams.numSlaves; index++)
    {
        uint8_t    ramBufIndex = 0;
        uint8_t    wDelayEnable = 0;

        /* Multibuffer RAM control:
         * buffer mode : 0x6
         * CSHOLD: 0x0
         */

        wDelayEnable = params->u.masterParams.wDelay? (uint8_t)1U : (uint8_t)0U;
        if(params->pinMode == MIBSPI_PINMODE_4PIN_CS)
        {
            chipSelectMask = (uint8_t)(0x1U << params->u.masterParams.slaveProf[index].chipSelect);
            csnr = ~chipSelectMask;
        }
        else
        {
            /* 3-pin mode, set CSNR to 0xFF */
            csnr = (uint8_t)MIBSPI_CS_NONE;
        }

        for (ramBufIndex = 0; ramBufIndex < params->u.masterParams.slaveProf[index].ramBufLen; ramBufIndex++)
        {
            uint32_t txControlWd = 0;
            uint16_t txCtrlWd16;
            volatile uint16_t *txCtrlWdPtr;
            
            CSL_FINS(txControlWd, MIBSPIRAM_TX_BUFMODE, MIBSPI_RAM_BUFFER_MODE);
            CSL_FINS(txControlWd, MIBSPIRAM_TX_CSHOLD, params->csHold);
            CSL_FINS(txControlWd, MIBSPIRAM_TX_WDEL, wDelayEnable);
            CSL_FINS(txControlWd, MIBSPIRAM_TX_CSNR, csnr);
            txCtrlWd16 = ((txControlWd & 0xFFFF0000) >> 16);
            txCtrlWdPtr = (uint16_t *)&ptrMibSpiRam->tx[ramBufOffset];
#ifdef __little_endian__
            txCtrlWdPtr++;
#elif defined(__big_endian__)
            //Do nothing. FIrst 16 bit word is the control word */
#else
#error "Unknown endianness"
#endif
            *txCtrlWdPtr = txCtrlWd16;
            ramBufOffset++;
        }

        if(ramBufOffset > MIBSPI_RAM_MAX_ELEM)
        {
            Mibspi_assert(0);
        }
    }

    /* Clear pending interrupts */
    ptrMibSpiReg->SPIFLG |= 0xFFFFU;

    /* Clear pending TG interrupts */
    ptrMibSpiReg->TGINTFLAG |= 0xFFFFFFFFU;

    /* Enable Error interrupts: Lower 8bits  */
    MIBSPI_enableErrorInterrupt(ptrMibSpiReg, 0x1U);

    /* Set Interrupt Levels - Interrupts are mapped to INT1 */
    MIBSPI_setErrorInterruptLevel(ptrMibSpiReg, 0x1U);

    /* Enable TG Interrupts to INT1 */
    ptrMibSpiReg->TGITENST|= 0xFFFFFFFFU;

    /* Pin settings for SPI signal */
    if(params->pinMode == MIBSPI_PINMODE_4PIN_CS)
    {
        chipSelectMask = (uint8_t)0U;
        for (index = 0; index < params->u.masterParams.numSlaves; index++)
        {
            chipSelectMask |= (uint8_t)(0x1U << params->u.masterParams.slaveProf[index].chipSelect);
        }
    }
    else
    {
        chipSelectMask = (uint8_t)0x0U;
    }
    MIBSPI_enablePinSettings(ptrMibSpiReg, params->pinMode, chipSelectMask);

    /* Finally start MIBSPI1 */
    CSL_FINS(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_SPIEN, 1U);
}

/**
 *  @b Description
 *  @n
 *       Initializes the MIBSPI as a SPI slave
 *
 *   @param[in] ptrHwCfg             Pointer to the SPI driver hardward Configuration
 *   @param[in] params               SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_initSlave(MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params *params)
{
    CSL_mibspiRam        *ptrMibSpiRam;
    CSL_mss_spiRegs      *ptrMibSpiReg;
    uint32_t        flag;
    uint32_t        index;
    uint32_t        regVal;

    /* Get Register and RAM base */
    ptrMibSpiRam = ptrHwCfg->ptrMibSpiRam;
    ptrMibSpiReg = ptrHwCfg->ptrSpiRegBase;

    /* Bring MIBSPI out of reset */
    ptrMibSpiReg->SPIGCR0 = 0U;
    ptrMibSpiReg->SPIGCR0 = 1U;

    /* Enable MIBSPI1 multibuffered mode and enable buffer RAM */
    CSL_FINS(ptrMibSpiReg->MIBSPIE,SPI_MIBSPIE_MSPIENA, 1U);

    /* Wait for buffer initialization complete before accessing MibSPI registers */
    while(1)
    {
        flag = CSL_FEXT(ptrMibSpiReg->SPIFLG,SPI_SPIFLG_BUFINITACTIVE);
        if(flag == 0)
        {
            break;
        }
    }

    MIBSPI_socMemInit(ptrHwCfg->mibspiInstId);

    regVal = ptrMibSpiReg->SPIGCR1;
    /* MIBSPI1 slave mode and clock configuration */
    CSL_FINS(regVal,SPI_SPIGCR1_MASTER, CSL_SPI_SPIGCR1_MASTER_SLAVE);
    CSL_FINS(regVal,SPI_SPIGCR1_CLKMOD, CSL_SPI_SPIGCR1_CLKMOD_EXTERNAL);
    ptrMibSpiReg->SPIGCR1 = regVal;

    /* SPIENA pin pulled high when not active */
    CSL_FINS(ptrMibSpiReg->SPIINT0,SPI_SPIINT0_ENABLEHIGHZ, 0U);

    /* Delays Configuration: this is only used by master, hence set it to all zeros */
    ptrMibSpiReg->SPIDELAY = 0x0U;

    /* Set Data Format 0 */
    regVal = ptrMibSpiReg->SPIFMT[0];
    CSL_FINS(regVal, SPI_SPIFMT_PHASE, MIBSPI_getPhase(params->frameFormat));         /* PHASE */
    CSL_FINS(regVal, SPI_SPIFMT_POLARITY, MIBSPI_getPolarity(params->frameFormat));   /* POLARITY */
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE, 4U);   /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_SHIFTDIR, (uint32_t)(params->shiftFormat)); /* SHIFTDIR */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t)params->dataSize);  /* CHARlEN */
    ptrMibSpiReg->SPIFMT[0] = regVal;

    /* Set Data Format 1,2,3. Used mulitple TG group transfer */
    regVal = ptrMibSpiReg->SPIFMT[1];
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE, 18U); /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t)params->dataSize); /* CHARlEN */
    ptrMibSpiReg->SPIFMT[1] = regVal;

    regVal = ptrMibSpiReg->SPIFMT[2];
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE, 18U); /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t)params->dataSize);    /* CHARlEN */
    ptrMibSpiReg->SPIFMT[2] = regVal;

    regVal = ptrMibSpiReg->SPIFMT[3];
    CSL_FINS(regVal, SPI_SPIFMT_PRESCALE, 18U); /* PRESCALE */
    CSL_FINS(regVal, SPI_SPIFMT_CHARLEN, (uint32_t)params->dataSize);    /* CHARlEN */
    ptrMibSpiReg->SPIFMT[3] = regVal;


    /* Enable ECC if enabled */
    if(params->eccEnable)
    {
        /* Enable ECC detection and signal bit Error correction */
        regVal = ptrMibSpiReg->PAR_ECC_CTRL;
        CSL_FINS(regVal,SPI_PAR_ECC_CTRL_EDEN, 0xAU);
        CSL_FINS(regVal,SPI_PAR_ECC_CTRL_EDAC_MODE, 0xAU);
        CSL_FINS(regVal,SPI_PAR_ECC_CTRL_SBE_EVT_EN, 0xAU);
        ptrMibSpiReg->PAR_ECC_CTRL = regVal;
    }

    for (index = 0; index < MIBSPI_NUM_TRANS_GROUP; index++)
    {
        /* Initialize transfer groups */
        regVal = ptrMibSpiReg->TGCTRL[index];
        CSL_FINS(regVal,SPI_TGCTRL_ONESHOT, 1U);  /* Oneshot trigger */
        CSL_FINS(regVal,SPI_TGCTRL_TRIGEVT, 7U);  /* Trigger event : Always */
        CSL_FINS(regVal,SPI_TGCTRL_TRIGSRC, 0U);  /* Trigger source : disabled */
        CSL_FINS(regVal,SPI_TGCTRL_PSTART,  0U);  /* TG start address : 0 */
        ptrMibSpiReg->TGCTRL[index] = regVal;
    }

    /* Initialize transfer groups end pointer */
    CSL_FINS(ptrMibSpiReg->LTGPEND,SPI_LTGPEND_LPEND, 0x0U);

    /* Initialize TX Buffer Ram, every element contains 16bits of data */
    for (index = 0; index < MIBSPI_RAM_MAX_ELEM; index++)
    {
            uint32_t txControlWd = 0;
            uint16_t txCtrlWd16;
            uint16_t *txCtrlWdPtr;
            
            CSL_FINS(txControlWd, MIBSPIRAM_TX_BUFMODE, MIBSPI_RAM_BUFFER_MODE);
            CSL_FINS(txControlWd, MIBSPIRAM_TX_CSHOLD, params->csHold);
            CSL_FINS(txControlWd, MIBSPIRAM_TX_CSNR, MIBSPI_CS_NONE);
            txCtrlWd16 = ((txControlWd & 0xFFFF0000) >> 16);
            txCtrlWdPtr = (uint16_t *)&ptrMibSpiRam->tx[index];
#ifdef __little_endian__
            txCtrlWdPtr++;
#elif defined(__big_endian__)
            //Do nothing. FIrst 16 bit word is the control word */
#else
#error "Unknown endianness"
#endif
            *txCtrlWdPtr = txCtrlWd16;
    }

    /* Clear pending interrupts */
    ptrMibSpiReg->SPIFLG |= 0xFFFFU;

    /* Clear pending TG interrupts */
    ptrMibSpiReg->TGINTFLAG |= 0xFFFFFFFFU;

    /* Enable Error interrupts: Lower 8bits  */
    MIBSPI_enableErrorInterrupt(ptrMibSpiReg, 0x1U);

    /* Set Interrupt Levels - Interrupts are mapped to INT0 */
    MIBSPI_setErrorInterruptLevel(ptrMibSpiReg, 0x1U);

    /* Enable TG Interrupts to INT1 */
    ptrMibSpiReg->TGITENST|= 0xFFFFFFFFU;

    /* Pin settings for SPI signal */
    MIBSPI_enablePinSettings(ptrMibSpiReg, params->pinMode, (uint8_t)(0x1U <<params->u.slaveParams.chipSelect));

    /* Finally start MIBSPI1 */
    CSL_FINS(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_SPIEN, 1U);
}

/**
 *  @b Description
 *  @n
 *     This function updates the data for the specified transfer group,
 *   the length of the data must match the length of the transfer group.
 *
 *   @param[in] ptrMibSpiDriver  MibSpi driver Handle
 *   @param[in] group            Transfer group (0..7)
 *   @param[in] data             Data buffer address
 *   @param[in] dataElem         Data element size
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void MIBSPI_writeDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem)
{
    CSL_mss_spiRegs      *ptrMibSpiReg;
    CSL_mibspiRam        *ptrMibSpiRam;
    uint32_t        start;
    uint32_t        index;

    /* Get MibSpi Register & RAM Base address */
    ptrMibSpiReg  = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;
    ptrMibSpiRam  = ptrMibSpiDriver->ptrHwCfg->ptrMibSpiRam;

    /* Fetch the start address from Register */
    start = CSL_FEXT(ptrMibSpiReg->TGCTRL[group],SPI_TGCTRL_PSTART);

    /* Write data in TX RAM */
    if(data == NULL)
    {
        for(index=start; index < (start + dataElem); index++)
        {
            CSL_MIBSPIRAM_SET_TX_TXDATA(ptrMibSpiRam, index, ptrMibSpiDriver->txScratchBuffer);
        }
    }
    else
    {
        if(ptrMibSpiDriver->params.dataSize == 16U)
        {
            for(index=start; index < (start + dataElem); index++)
            {
                CSL_MIBSPIRAM_SET_TX_TXDATA(ptrMibSpiRam, index, data[index-start]);
            }
        }
        else
        {
            uint8_t *pBuffer = (uint8_t *)data;

            for(index=start; index < (start + dataElem); index++)
            {
                CSL_MIBSPIRAM_SET_TX_TXDATA(ptrMibSpiRam, index, pBuffer[index-start]);
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      This function transfers the data from the specified transfer group receive
 *   buffers to the data array,  the length of the data must match the length
 *   of the transfer group.
 *
 *   @param[in]   ptrMibSpiDriver  MibSpi driver Handle
 *   @param[in]   group            Transfer group (0..7)
 *   @param[out]  data             Pointer to data array
 *   @param[in]   dataElem         Data element size
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval return error flags from data buffer, if there was a receive error on
 *         one of the buffers this will be reflected in the return value.
 */
static uint32_t MIBSPI_readDataRAM(MibSpiDriver_Object *ptrMibSpiDriver, uint8_t group, uint16_t *data, uint16_t dataElem)
{
    CSL_mss_spiRegs      *ptrMibSpiReg;
    volatile CSL_mibspiRam       *ptrMibSpiRam;
    uint16_t        mibspiFlags  = 0U;
    uint32_t        ret;
    uint32_t        start;
    uint32_t        index;

    /* Get MibSpi Register & Ram Base address */
    ptrMibSpiReg  = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;
    ptrMibSpiRam  = ptrMibSpiDriver->ptrHwCfg->ptrMibSpiRam;

    /* Fetch the start address from Register */
    start = CSL_FEXT(ptrMibSpiReg->TGCTRL[group],SPI_TGCTRL_PSTART);

    if(data == NULL)
    {
         /* Save data from RAM into scrach buffer */
         for(index=start; index < (start + dataElem); index++)
         {
             /* Wait until data is available */
             while(CSL_MIBSPIRAM_GET_RX_RXEMPTY(ptrMibSpiRam, index)  != 0U)
             {
             }

             mibspiFlags |= CSL_MIBSPIRAM_GET_RX_RXFLAGS(ptrMibSpiRam, index);

             ptrMibSpiDriver->rxScratchBuffer = CSL_MIBSPIRAM_GET_RX_RXDATA(ptrMibSpiRam, index);
         }
    }
    else
    {

         /* Save data from RAM */
         for(index=start; index < (start + dataElem); index++)
         {

             /* Wait until data is available */
             while(CSL_MIBSPIRAM_GET_RX_RXEMPTY(ptrMibSpiRam,index) != 0U)
             {
             }

             mibspiFlags |= CSL_MIBSPIRAM_GET_RX_RXFLAGS(ptrMibSpiRam, index);

             if(ptrMibSpiDriver->params.dataSize == 16U)
             {
                 data[index-start] = CSL_MIBSPIRAM_GET_RX_RXDATA(ptrMibSpiRam, index);
             }
             else
             {
                 uint8_t *pBuffer = (uint8_t *)data;
             
                 pBuffer[index-start] = (uint8_t)(CSL_MIBSPIRAM_GET_RX_RXDATA(ptrMibSpiRam, index) & 0xFFU);
             }
         }
    }

    /* Rx Flags */
    ret = ((uint32_t)mibspiFlags >> 8U) & 0x5FU;

    return ret;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the MIBSPI interrupt.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MIBSPI_ISR (uintptr_t arg)
{
    MibSpiDriver_Object    *ptrMibSpiDriver;
    CSL_mss_spiRegs             *ptrMibSpiReg;
    volatile uint32_t      intVector;
    uint8_t                group = 0U;

    /* Get the MibSpi driver handle */
    ptrMibSpiDriver = (MibSpiDriver_Object *)arg;

    if(ptrMibSpiDriver != (MibSpiDriver_Object *)NULL)
    {
        ptrMibSpiReg    = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;

        /* Handles the TG Complete Interrupt */
        intVector = ptrMibSpiReg->TGINTFLAG;
        intVector = intVector >> 16U;
        for(group = 0U ; group < MIBSPI_SLAVE_MAX; group++)
        {
            if(intVector & (0x1U << group))
            {
                ptrMibSpiDriver->hwStats.TGComplete[group]++;
                ptrMibSpiReg->TGINTFLAG = (uint32_t)0x10000U << group;

#ifdef SPI_MULT_ICOUNT_SUPPORT
                /* All transfer done ? - check icount status */
                if((ptrMibSpiReg->DMACTRL[group] & 0x3FU) != 0)
                {
                    MIBSPI_transferGroupEnable(ptrMibSpiReg, group);
              
 }
#endif
            }
        }

        /* Found out the interrupt source and increment the stats count */
        intVector = ptrMibSpiReg->SPIFLG;
        if (intVector & 0x1U)
        {
            ptrMibSpiDriver->hwStats.dlenErr++;
        }
        if (intVector & 0x2U)
        {
            ptrMibSpiDriver->hwStats.timeout++;
        }
        if (intVector & 0x4U)
        {
            ptrMibSpiDriver->hwStats.parErr++;
        }
        if (intVector & 0x8U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.desync++;
        }
        if (intVector & 0x10U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.bitErr++;
        }
        if (intVector & 0x40U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.rxOvrnErr++;
        }
        if (intVector & 0x100U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.rxFull++;
        }
        if (intVector & 0x200U)
        {
            /* Master only interrupt */
            ptrMibSpiDriver->hwStats.txEmpty++;
        }

        ptrMibSpiReg->SPIFLG = intVector;
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      This function takes source and destination address and invoke MibSPI to transfer it over DMA/CPU
 *
 *   @param[in] ptrMibSpiDriver   MibSpi driver handle
 *   @param[in] srcData           Input buffer pointer to send the Data from
 *   @param[in] dstData           Output buffer pointer to get the Data
 *   @param[in] dataElemSize      Data Elements size
 *   @param[in] group             MibSPI transfer group used for this transfer
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static void MIBSPI_dataTransfer
(
    MibSpiDriver_Object * ptrMibSpiDriver,
    uint8_t *srcData,
    uint8_t *dstData,
    uint16_t dataElemSize,
    uint8_t group
)
{
    MibSpi_HwCfg  const  *ptrHwCfg;
    CSL_mss_spiRegs      *ptrMibSpiReg;
    uint8_t         ramOffset = 0U;
    uint16_t        bufId = 0U;
    uint8_t         iCount = 0U;

    /* Get MibSpi driver hardware config */
    ptrHwCfg = ptrMibSpiDriver->ptrHwCfg;

    /* Get MibSpi Register & Ram Base address */
    ptrMibSpiReg  = ptrHwCfg->ptrSpiRegBase;

    /* Put SPI in active mode */
    MIBSPI_SPIEnable(ptrMibSpiReg);

    /* Find out bufId and RAM offset */
    if(ptrMibSpiDriver->params.mode == MIBSPI_SLAVE)
    {
        ramOffset = 0U;

        /* Find out iCount and bufid */
        if (dataElemSize > MIBSPI_RAM_MAX_ELEM)
        {
            /* (iCount + 1) transfer of size MIBSPI_RAM_MAX_ELEM */
            iCount = (uint8_t)(dataElemSize / MIBSPI_RAM_MAX_ELEM - 1U);
            bufId = (uint16_t)(MIBSPI_RAM_MAX_ELEM -1U);
        }
        else
        {
            /* One transfer of dataElemSize */
            iCount = (uint8_t)0U;
            bufId = (uint16_t)(dataElemSize -1U);
        }
    }
    else
    {
        uint8_t    ramLen = 0;

        Mibspi_assert(group < MIBSPI_UTILS_ARRAYSIZE(ptrMibSpiDriver->rambufStart));
        ramOffset = ptrMibSpiDriver->rambufStart[group];
        ramLen =  ptrMibSpiDriver->params.u.masterParams.slaveProf[group].ramBufLen;

        /* Find out iCound and bufid */
        if (dataElemSize > ramLen)
        {
            /* (iCount + 1) transfer of size ramLen */
            iCount = (uint8_t)(dataElemSize / ramLen - 1U);
            bufId = (uint16_t)ramOffset + (uint16_t)ramLen -(uint16_t)1U;
        }
        else
        {
            /* One transfer of dataElemSize */
            iCount = 0;
            bufId = (uint16_t)ramOffset + dataElemSize - (uint16_t)1U;
        }
    }

    /* Initialize transfer group start offset */
    MIBSPI_transferSetPStart(ptrMibSpiReg, group, ramOffset);

    /* This is only needed for XWR16xx/XWR18xx/XWR68xx device */
    MIBSPI_transferSetPStart(ptrMibSpiReg, (uint8_t)(group + 1U), (uint8_t)(bufId + 1U));

    /* Initialize transfer groups end pointer */
    CSL_FINS(ptrMibSpiReg->LTGPEND,SPI_LTGPEND_LPEND, (uint32_t)bufId);

#ifdef MIBSPI_DMA_ENABLE
     /* Configure DMA in the following 3 cases
         Case 1: SrcData=NULL, DstData!=NULL   => Read data from SPI with dummy write
         Case 2: SrcData!=NULL, DstData=NULL   => Write data to SPI with dummy read
         Case 3: SrcData!=NULL, DstData!=NULL  => duplex Read/Write
     */
    if(ptrMibSpiDriver->params.dmaEnable == (uint8_t)1U)
    {
        MibSpi_dmaXferInfo_t dmaXferInfo;
        uint32_t   txRAMAddr;
        uint32_t   rxRAMAddr;
        CSL_mibspiRam       *ptrMibSpiRam;

        /* Disable TG complete interrupt */
        MIBSPI_disableGroupInterrupt(ptrMibSpiReg, group);

        /* Disable SPI DMA */
        MIBSPI_dmaCtrlGroupDisable(ptrMibSpiReg, group);


        ptrMibSpiRam  = ptrHwCfg->ptrMibSpiRam;
        /* Get MibSPI RAM address */
        txRAMAddr = (uint32_t)(&(ptrMibSpiRam->tx[ramOffset]));
        rxRAMAddr = (uint32_t)(&(ptrMibSpiRam->rx[ramOffset]));


         /* Case 1: SrcData=NULL, DstData!=NULL  => Read data from SPI with dummy write */
        dmaXferInfo.dmaReqLine = group;
        if (srcData != NULL)
        {
            dmaXferInfo.tx.saddr = MibspiUtils_virtToPhy(srcData);
        }
        else
        {
            dmaXferInfo.tx.saddr = NULL;
        }
        dmaXferInfo.tx.daddr = MibspiUtils_virtToPhy((const void *)txRAMAddr);
        dmaXferInfo.rx.saddr = MibspiUtils_virtToPhy((const void *)rxRAMAddr);
        if (dstData != NULL)
        {
            dmaXferInfo.rx.daddr = MibspiUtils_virtToPhy(dstData);
        }
        else
        {
            dmaXferInfo.rx.daddr = NULL;
        }
        if(ptrMibSpiDriver->params.dataSize == 8U)
        {
            dmaXferInfo.size.elemSize = 1;
        }
        else
        {
            dmaXferInfo.size.elemSize =  2;
        }
        dmaXferInfo.size.elemCnt = bufId + 1U;
        dmaXferInfo.size.frameCnt = iCount + 1U;
        Mibspi_assert(MIBSPI_dmaTransfer(ptrMibSpiDriver->mibspiHandle, &dmaXferInfo) == MIBSPI_STATUS_SUCCESS);;

        /* Configuring the mibspi dmaCtrl for the channel */
        MIBSPI_dmaCtrlGroupConfig(ptrMibSpiReg, bufId, iCount, group);

        /* Setup TG interrupt */
        MIBSPI_enableGroupInterrupt(ptrMibSpiReg, group, MIBSPI_INT_LEVEL);

        if ((CSL_FEXT(ptrMibSpiReg->IOLPBKTSTCR,SPI_IOLPBKTSTCR_LPBKTYPE) == MIBSPI_LOOPBK_ANALOG)
            &&
            (CSL_FEXT(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_MASTER) == CSL_SPI_SPIGCR1_MASTER_SLAVE))
        {
            CSL_FINS(ptrMibSpiReg->IOLPBKTSTCR,SPI_IOLPBKTSTCR_IOLPBKTSTENA, 0xAU);
        }

        Mibspi_assert(MIBSPI_dmaStartTransfer(ptrMibSpiDriver->mibspiHandle, group) == MIBSPI_STATUS_SUCCESS);

        MIBSPI_dmaCtrlGroupStart(ptrMibSpiReg, group, MIBSPI_DMACTRL_CH_BOTH);

        /* Start TG group transfer */
        MIBSPI_transferGroupEnable(ptrMibSpiReg, group);


    }
    else
#endif
    {
        uint32_t  index;
        uint16_t  size;

        size = (uint32_t)bufId + 1U - (uint32_t)ramOffset;

        for(index = 0; index <= iCount; index++)
        {
            /* Read data with dummy write through CPU mode */
            if ((srcData == (uint8_t *)NULL) && (dstData != (uint8_t *)NULL))
            {
                MIBSPI_transferGroupEnable(ptrHwCfg->ptrSpiRegBase, group);

                while(MIBSPI_checkTGComplete(ptrMibSpiReg, group) == 0U)
                {
                }

                /* Read data from local buffer to MibSPI Rx RAM */
                MIBSPI_readDataRAM(ptrMibSpiDriver, group, (uint16_t *)dstData, size);

            }
            else if( (srcData != (uint8_t *)NULL) && (dstData == (uint8_t *)NULL))
            {
                /* Write data from MibSPI Rx RAM to local buffer */
                MIBSPI_writeDataRAM(ptrMibSpiDriver, group, (uint16_t *)srcData, size);

                MIBSPI_transferGroupEnable(ptrHwCfg->ptrSpiRegBase, group);

                while(MIBSPI_checkTGComplete(ptrMibSpiReg, group) == 0U)
                {
                }
                /* Read data from local buffer to MibSPI Rx RAM */
                MIBSPI_readDataRAM(ptrMibSpiDriver, group, (uint16_t *)NULL, size);

            }
            else
            {
                /* Write data from local buffer to MibSPI Tx RAM */
                MIBSPI_writeDataRAM(ptrMibSpiDriver, group, (uint16_t *)srcData, size);

                MIBSPI_transferGroupEnable(ptrHwCfg->ptrSpiRegBase, group);

                /* Wait for the transfer to complete */
                while(MIBSPI_checkTGComplete(ptrMibSpiReg, group) == 0U)
                {
                }

                /* Read the data from RX RAM */
                MIBSPI_readDataRAM(ptrMibSpiDriver, group, (uint16_t *)dstData, size);

            }

            srcData = (srcData == (uint8_t *)NULL)? (uint8_t *)NULL : (uint8_t *)(srcData + size * ptrMibSpiDriver->params.dataSize / 8U);
            dstData = (dstData == (uint8_t *)NULL)? (uint8_t *)NULL : (uint8_t *)(dstData + size * ptrMibSpiDriver->params.dataSize / 8U);

        }
        /* Transfer finished, unblock the thread */
        SemaphoreP_post(ptrMibSpiDriver->transferCompleteSem);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to open SPI device.
 *
 *   @param[in] ptrMibSpiDriver   MibSpi driver handle
 *   @param[in] ptrHwCfg          SPI device Hardware configuration
 *   @param[in] params            SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t MIBSPI_openSlaveMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params *params)
{
    int32_t                 retVal = 0;

    /* Initializing mibspi as a SPI slave */
    MIBSPI_initSlave(ptrHwCfg, params);

    if(params->dmaEnable)
    {
#ifdef MIBSPI_DMA_ENABLE

        retVal = MIBSPI_dmaConfig(ptrMibSpiDriver->mibspiHandle, MIBSPI_SLAVEMODE_TRANS_GROUP);
        if(retVal < 0)
        {
            retVal = MIBSPI_STATUS_ERROR;
            goto exit;
        }
#else
        retVal = MIBSPI_STATUS_ERROR;
        goto exit;
#endif
    }

    /* Save driver info for Slave mode */
    ptrMibSpiDriver->rambufStart [MIBSPI_SLAVEMODE_TRANS_GROUP]  = 0U;
    ptrMibSpiDriver->rambufEnd [MIBSPI_SLAVEMODE_TRANS_GROUP]    = MIBSPI_RAM_MAX_ELEM;

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to open SPI device.
 *
 *   @param[in] ptrMibSpiDriver     MibSpi driver handle
 *   @param[in] ptrHwCfg            SPI device Hardware configuration
 *   @param[in] params              SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Successful           =0
 *                 Failed           <0
 */
static int32_t MIBSPI_openMasterMode(MibSpiDriver_Object *ptrMibSpiDriver, MibSpi_HwCfg* ptrHwCfg, const MIBSPI_Params *params)
{
    int32_t                 retVal = 0;
    uint8_t                 index;
    uint8_t                 ramBufOffset = 0;

    /* Initializing mibspi as a SPI master */
    MIBSPI_initMaster(ptrHwCfg, params);

    /* Configure the Master clock prescaler */
    MIBSPI_setMasterClockRate(ptrHwCfg->ptrSpiRegBase, ptrHwCfg->clockSrcFreq, params->u.masterParams.bitRate);

    for(index = 0; index < params->u.masterParams.numSlaves; index++)
    {
        /***************************************
         ******** Save RAM offset information *******
         **************************************/
        ptrMibSpiDriver->rambufStart[index] = ramBufOffset;
        ptrMibSpiDriver->rambufEnd[index] = ramBufOffset + params->u.masterParams.slaveProf[index].ramBufLen;
        ramBufOffset =ptrMibSpiDriver->rambufEnd[index];

        /***************************************
         ****** DMA Configuration if enabled *******
         **************************************/
#ifdef MIBSPI_DMA_ENABLE
        if(params->dmaEnable)
        {
            retVal = MIBSPI_dmaConfig(ptrMibSpiDriver->mibspiHandle, index);

            if (retVal != MIBSPI_STATUS_SUCCESS)
            {
                break;
            }
        }
#endif

    }

    return (retVal);
}

/**************************************************************************
 ************************* MibSPI Exported Driver Functions *********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to open SPI device.
 *
 *   @param[in] handle     SPI device handle
 *   @param[in] params     SPI device configuration parameters
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    SPI device handle.
 */
MIBSPI_Handle MIBSPI_openCore(MIBSPI_Handle handle, const MIBSPI_Params *params)
{
    MIBSPI_Config*             ptrSPIConfig;
    MibSpi_HwCfg*           ptrHwCfg;
    SemaphoreP_Params       semParams;
    MIBSPI_Handle              retHandle = (MIBSPI_Handle)NULL;
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    int32_t                 retVal = MIBSPI_STATUS_SUCCESS;
    OsalRegisterIntrParams_t interruptRegParams;

    /* Get the SPI driver Configuration: */
    ptrSPIConfig = (MIBSPI_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (MibSpi_HwCfg*)ptrSPIConfig->hwAttrs;

    /* Validate hardware configuration */
    Mibspi_assert(ptrHwCfg != NULL);
    Mibspi_assert(ptrHwCfg->ptrSpiRegBase != NULL);
    Mibspi_assert(ptrHwCfg->ptrMibSpiRam != NULL);

#ifdef SPI_PARAMS_CHECK
    /* Validate params for SPI driver */
    retVal = MIBSPI_validateParams(params);
    MIBSPITRACE_ERROR_IF((retVal < 0), "SPI Driver (%p) failed params validation\n", ptrHwCfg->ptrSpiRegBase);
    if(retVal < 0)
    {
        /* Error: SPI Driver found incorrect parameters. */
        goto exit;
    }
    else
#endif
    {
        /* Ensure that the driver is NOT being used: */
        Mibspi_assert (ptrSPIConfig->object != NULL);
        ptrMibSpiDriver = (MibSpiDriver_Object *)ptrSPIConfig->object;

        /* Check instance was not previously opened */
        if (ptrMibSpiDriver->mibspiHandle == NULL)
        {
            /* Initialize the memory: */
            memset ((void *)ptrMibSpiDriver, 0, sizeof(MibSpiDriver_Object));

            /* Save parameters and hardware configurations */
            ptrMibSpiDriver->params = *params;

            /* Save the Driver handle and Hw config */
            ptrMibSpiDriver->ptrHwCfg = ptrHwCfg;
            MIBSPI_validateIPVersionInfo(ptrHwCfg);
            ptrMibSpiDriver->mibspiHandle = handle;
            ptrMibSpiDriver->txScratchBuffer = ptrMibSpiDriver->params.txDummyValue;

            MIBSPI_resetTransactionState(&ptrMibSpiDriver->transactionState);
            /* Call open function for the MibSPI operating mode */
            if(params->mode == MIBSPI_SLAVE)
            {
                retVal = MIBSPI_openSlaveMode(ptrMibSpiDriver, ptrHwCfg, params);
            }
            else
            {
                retVal = MIBSPI_openMasterMode(ptrMibSpiDriver, ptrHwCfg, params);
            }
            if(retVal < 0)
            {
                goto exit;
            }

            /* Create a binary semaphore which is used to handle the Blocking operation. */
            SemaphoreP_Params_init(&semParams);
            semParams.mode            = SemaphoreP_Mode_BINARY;
            ptrMibSpiDriver->transferCompleteSem = SemaphoreP_create(0U, &semParams);

            /* Register SPI Interrupt handling ISR */
            /* Initialize with defaults */
            Osal_RegisterInterrupt_initParams(&interruptRegParams);
            /* Populate the interrupt parameters */
            interruptRegParams.corepacConfig.arg=(uintptr_t)ptrMibSpiDriver;
            interruptRegParams.corepacConfig.name=(char *)"MibSpiInt";
            interruptRegParams.corepacConfig.isrRoutine=MIBSPI_ISR;
        #if defined(_TMS320C6X)
            interruptRegParams.corepacConfig.corepacEventNum=(int32_t)ptrHwCfg->interrupt1Num; /* Event going in to CPU */
            interruptRegParams.corepacConfig.intVecNum      = OSAL_REGINT_INTVEC_EVENT_COMBINER; /* Host Interrupt vector */
        #else
            interruptRegParams.corepacConfig.priority = 0x1U;
            interruptRegParams.corepacConfig.intVecNum=(int32_t)ptrHwCfg->interrupt1Num; /* Host Interrupt vector */
            interruptRegParams.corepacConfig.corepacEventNum = (int32_t)ptrHwCfg->interrupt1Num;
        #endif
            /* Register interrupts */
            Osal_RegisterInterrupt(&interruptRegParams, &ptrMibSpiDriver->hwiHandle);

            if (ptrMibSpiDriver->hwiHandle == NULL)
            {
                retVal = MIBSPI_STATUS_ERROR;
            }

            if (retVal == MIBSPI_STATUS_SUCCESS)
            {
                Osal_EnableInterrupt(interruptRegParams.corepacConfig.corepacEventNum, interruptRegParams.corepacConfig.intVecNum);

                /* Setup the return handle: */
                retHandle = handle;
            }
        }
        else
        {
            retVal = MIBSPI_STATUS_ERROR;
        }
    }
exit:
    if((retVal < 0) && (ptrMibSpiDriver != NULL) )
    {
        /* Cleanup in case of error */
        if(ptrMibSpiDriver->hwiHandle)
        {
            Osal_DeleteInterrupt(ptrMibSpiDriver->hwiHandle, ptrMibSpiDriver->ptrHwCfg->interrupt1Num);
        }
    }

    return (retHandle);
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to close SPI device.
 *
 *   @param[in] handle      SPI device handle
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
void MIBSPI_closeCore(MIBSPI_Handle handle)
{
    MibSpiDriver_Object*    ptrMibSpiDriver;
    CSL_mss_spiRegs              *ptrMibSpiReg;

    /* Sanity check handle */
    Mibspi_assert(handle != NULL);

    /* Get the Object from SPI Handle */
    ptrMibSpiDriver = (MibSpiDriver_Object *)handle->object;

    /* Sanity check driver handle */
    if(ptrMibSpiDriver != (MibSpiDriver_Object *)NULL)
    {
        /* Get the Register base address from SPI Handle */
        ptrMibSpiReg = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;

        Mibspi_assert(ptrMibSpiDriver->transactionState.transaction == NULL);

#ifdef MIBSPI_DMA_ENABLE
        if(ptrMibSpiDriver->params.dmaEnable)
        {

            if (ptrMibSpiDriver->params.mode == MIBSPI_SLAVE)
            {
                MIBSPI_dmaFreeChannel(ptrMibSpiDriver->mibspiHandle, MIBSPI_SLAVEMODE_TRANS_GROUP);
            }
            else
            {
                uint32_t index;

                for (index = 0; index < ptrMibSpiDriver->params.u.masterParams.numSlaves; index++)
                {
                    MIBSPI_dmaFreeChannel(ptrMibSpiDriver->mibspiHandle, index);
                }
            }
        }
#endif

        /* Does the SPI Driver in Blocking mode? */
        if (ptrMibSpiDriver->transferCompleteSem)
        {
            /* Delete the semaphore */
            SemaphoreP_delete (ptrMibSpiDriver->transferCompleteSem);
        }

        /* Disable MibSPI */
        MIBSPI_setResetMode(ptrMibSpiReg, true);

        /* Delete Hwi */
        if(ptrMibSpiDriver->hwiHandle)
        {
        

            Osal_DeleteInterrupt(ptrMibSpiDriver->hwiHandle, ptrMibSpiDriver->ptrHwCfg->interrupt1Num);
        }
        memset(ptrMibSpiDriver, 0, sizeof(*ptrMibSpiDriver));

    }
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to send command to SPI driver.
 *
 *   @param[in] handle      SPI device handle
 *   @param[in] cmd          Command to SPI device
 *   @param[in] arg           Command Argument to SPI device
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    Status of SPI device command.
 */
int32_t MIBSPI_controlCore(MIBSPI_Handle handle, uint32_t cmd, const void *arg)
{
    MibSpiDriver_Object     *ptrMibSpiDriver;
    CSL_mss_spiRegs              *ptrMibSpiReg;
    int32_t                 status = MIBSPI_STATUS_SUCCESS;
    MibSpi_LoopBackType     loopbacktype;

    /* Sanity check params */
    if( (handle == NULL) || (arg == NULL))
    {
        status = MIBSPI_STATUS_ERROR;
    }
    else
    {
        /* Get the Object from SPI Handle */
        ptrMibSpiDriver = (typeof (ptrMibSpiDriver))handle->object;

        /* Get the Register base address from SPI Handle */
        ptrMibSpiReg = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;

        switch (cmd)
        {
            case MIBSPI_CMD_LOOPBACK_ENABLE:
                {
                    loopbacktype = *(MibSpi_LoopBackType *)arg;

                    /* Sanity check the input parameters */
                    if ((loopbacktype == MIBSPI_LOOPBK_DIGITAL) && (ptrMibSpiDriver->params.mode != MIBSPI_MASTER))
                    {
                        /* Loopback is not supported in Slave mode */
                        status = MIBSPI_STATUS_ERROR;
                    }
                    else if ( (loopbacktype == MIBSPI_LOOPBK_DIGITAL) || (loopbacktype == MIBSPI_LOOPBK_ANALOG) )
                    {
                        /* Enable Loopback here in case Deice act as MibSPI Slave */
                        MIBSPI_enableLoopback(ptrMibSpiReg, loopbacktype);
                        status = MIBSPI_STATUS_SUCCESS;
                    }
                    else
                    {
                        /* Disable MibSPI LoopBack */
                        MIBSPI_disableLoopback(ptrMibSpiReg);

                        status = MIBSPI_STATUS_SUCCESS;
                    }
                }

                break;

            case MIBSPI_CMD_SET_CLOCK_PHASEPOLARITY:
                {
                    uint32_t frameFmt;
                    frameFmt = *(uint32_t *)arg;

                    /* 2 bit setting */
                    frameFmt = frameFmt & 0x3U;

                    /* Sanity check the input parameters */
                    if (ptrMibSpiDriver->params.mode != MIBSPI_SLAVE)
                    {
                        /* Loopback is not supported in Master mode */
                        status = MIBSPI_STATUS_ERROR;
                    }
                    else
                    {
                        MIBSPI_setClockPhasePolarity(ptrMibSpiReg, (uint8_t)frameFmt);
                        status = MIBSPI_STATUS_SUCCESS;
                    }
                }
                break;

            case MIBSPI_CMD_GET_STATS:
                {
                    MIBSPI_Stats *stats;
                    stats = (MIBSPI_Stats *)arg;
                    status = MIBSPI_getDrvStats(handle, stats);
                }
                break;

            default:
                /* Command is not supported */
                status = MIBSPI_STATUS_UNDEFINEDCMD;
                break;
        }
    }
    return (status);
}

static void MIBSPI_initTransactionState(Mibspi_transactionState_t *transactionState, MIBSPI_Transaction     *transaction)
{
    uintptr_t key;

    key = HwiP_disable();

    transactionState->edmaCbCheck = MIBSPI_NONE_EDMA_CALLBACK_OCCURED;
    transactionState->transferErr = MIBSPI_XFER_ERR_NONE;
    transactionState->transaction = transaction;
    transactionState->transaction->status = MIBSPI_TRANSFER_STARTED;
    transactionState->remainSize = 0U;
    transactionState->dataLength = 0U;
    transactionState->dataSizeInBytes = 0U;

    HwiP_restore(key);
}

static void MIBSPI_resetTransactionState(Mibspi_transactionState_t *transactionState)
{
    uintptr_t key;

    key = HwiP_disable();

    transactionState->edmaCbCheck = MIBSPI_NONE_EDMA_CALLBACK_OCCURED;
    transactionState->transferErr = MIBSPI_XFER_ERR_NONE;
    transactionState->transaction = NULL;
    transactionState->remainSize = 0U;
    transactionState->dataLength = 0U;
    transactionState->dataSizeInBytes = 0U;

    HwiP_restore(key);
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to initiate SPI transaction based on the transaction configuration.
 *
 *   @param[in] handle         SPI device handle
 *   @param[in] transaction    Transaction configuration
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    true          if transaction is successful
 *             false         if transaction is failed
 */
bool MIBSPI_transferCore(MIBSPI_Handle handle, MIBSPI_Transaction *transaction)
{
    uintptr_t               key;
    MibSpiDriver_Object     *ptrMibSpiDriver;
    SemaphoreP_Status       semaStatus;
    bool                    ret = false;
    uint16_t                dataLength;
    int32_t                status = MIBSPI_STATUS_SUCCESS;


    /* Get the MibSpi driver handle */
    ptrMibSpiDriver = (MibSpiDriver_Object *)handle->object;

#ifdef SPI_PARAMS_CHECK
    status = MIBSPI_validateTransferParams(transaction, ptrMibSpiDriver);
    if (status != MIBSPI_STATUS_SUCCESS)
    {
        /* Initialiaze transaction as failed transer */
        transaction->status = MIBSPI_TRANSFER_FAILED;
        ret = false;

    }
#endif
    if (status == MIBSPI_STATUS_SUCCESS)
    {
        if(ptrMibSpiDriver->params.dataSize == 16U)
        {
            /* MIBSPI reads in 2 bytes format */
            dataLength = (uint16_t)transaction->count >> 1U;
        }
        else
        {
            dataLength = transaction->count;
        }

        MIBSPI_initTransactionState(&ptrMibSpiDriver->transactionState, transaction);

#ifndef SPI_MULT_ICOUNT_SUPPORT
        if(ptrMibSpiDriver->params.mode == MIBSPI_SLAVE)
        {
            MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, MIBSPI_SLAVEMODE_TRANS_GROUP);
        }
        else
        {
            MIBSPI_dataTransfer(ptrMibSpiDriver, transaction->txBuf, transaction->rxBuf, dataLength, transaction->slaveIndex);
        }

        if (ptrMibSpiDriver->params.transferMode == MIBSPI_MODE_BLOCKING)
        {
            semaStatus = SemaphoreP_pend(ptrMibSpiDriver->transferCompleteSem, ptrMibSpiDriver->params.transferTimeout);
            if(semaStatus != SemaphoreP_OK)
            {
                /* Populate status code */
                transaction->status = MIBSPI_TRANSFER_TIMEOUT;
                ret = false;
            }
            else
            {
                /* Populate status code */
                transaction->status = MIBSPI_TRANSFER_COMPLETED;
                ret = true;
            }
        }
        else
        {
            /* Execution should not reach here */
            transaction->status = MIBSPI_TRANSFER_FAILED;
            ret = false;
        }
#else
        do {
            uint32_t    ramSize;
            uint16_t    remainSize;
            uint16_t    dataSizeInBytes = 2U;

            if(ptrMibSpiDriver->params.dataSize == 16U)
            {
                dataSizeInBytes = 2U;
            }
            else
            {
                dataSizeInBytes = 1U;
            }

            /* Find out rambuf size */
            if(ptrMibSpiDriver->params.mode == MIBSPI_SLAVE)
            {
                ramSize = MIBSPI_RAM_MAX_ELEM;
            }
            else
            {
                ramSize = ptrMibSpiDriver->params.u.masterParams.slaveProf[transaction->slaveIndex].ramBufLen;
            }

            /*
             *  If dataLength is bigger than ramSize, there will be two data transfers:
             *      1. multiple of ramSize
             *      2. remaining data after multiple of ramsize
             *  If dataLength is smaller or equal to ramSize, there will be one data transfer
             */
            remainSize = (dataLength > ramSize) ? dataLength%ramSize : 0U;

            if(remainSize > 0U)
            {
                /* Size of the first transfer */
                dataLength -= remainSize;
            }

            if (ptrMibSpiDriver->params.transferMode == MIBSPI_MODE_CALLBACK)
            {
                /* Tell ISR function how much data there is remaining after this transfer
                 * (if remainSize > 0 then this is the first of two transfers) */
                ptrMibSpiDriver->transactionState.remainSize = remainSize;
                ptrMibSpiDriver->transactionState.dataLength = dataLength;
                ptrMibSpiDriver->transactionState.dataSizeInBytes = dataSizeInBytes;
            }

            if(ptrMibSpiDriver->params.mode == MIBSPI_SLAVE)
            {
                MIBSPI_dataTransfer(ptrMibSpiDriver, (uint8_t *)transaction->txBuf, (uint8_t *)transaction->rxBuf, dataLength, MIBSPI_SLAVEMODE_TRANS_GROUP);
            }
            else
            {
                MIBSPI_dataTransfer(ptrMibSpiDriver, (uint8_t *)transaction->txBuf, (uint8_t *)transaction->rxBuf, dataLength, transaction->slaveIndex);
            }

            if (ptrMibSpiDriver->params.transferMode == MIBSPI_MODE_BLOCKING)
            {
                semaStatus = SemaphoreP_pend(ptrMibSpiDriver->transferCompleteSem, ptrMibSpiDriver->params.transferTimeout);
                if(semaStatus != SemaphoreP_OK)
                {
                    /* Populate status code */
                    transaction->status = MIBSPI_TRANSFER_TIMEOUT;
                    ret = false;
                    remainSize = 0;
                }
                else
                {
                    /* Populate status code */
                    transaction->status = MIBSPI_TRANSFER_COMPLETED;
                    ret = true;
                }
            }
            else
            {
                /* Return success status if non-blocking mode */
                ret = true;
                return ret;
            }

            /* Check if transfer finished */
            if(remainSize == 0)
            {
                break;
            }

            /* Change buffer pointer and data size for the second transfer */
            transaction->txBuf = (void *)((transaction->txBuf == NULL)? NULL: (uint8_t *)transaction->txBuf + dataLength * dataSizeInBytes);
            transaction->rxBuf = (void *)((transaction->rxBuf == NULL)? NULL: (uint8_t *)transaction->rxBuf + dataLength * dataSizeInBytes);
            dataLength = remainSize;

            /* Transfer the remaining size */
            if(ptrMibSpiDriver->params.mode == MIBSPI_SLAVE)
            {
                MIBSPI_dataTransfer(ptrMibSpiDriver, (uint8_t *)transaction->txBuf, (uint8_t *)transaction->rxBuf, dataLength, MIBSPI_SLAVEMODE_TRANS_GROUP);
            }
            else
            {
                MIBSPI_dataTransfer(ptrMibSpiDriver, (uint8_t *)transaction->txBuf, (uint8_t *)transaction->rxBuf, dataLength, transaction->slaveIndex);
            }

            if (ptrMibSpiDriver->params.transferMode == MIBSPI_MODE_BLOCKING)
            {
                semaStatus = SemaphoreP_pend(ptrMibSpiDriver->transferCompleteSem, ptrMibSpiDriver->params.transferTimeout);
                if(semaStatus != SemaphoreP_OK)
                {
                    /* Populate status code */
                    transaction->status = MIBSPI_TRANSFER_TIMEOUT;
                    ret = false;
                }
                else
                {
                    /* Populate status code */
                    transaction->status = MIBSPI_TRANSFER_COMPLETED;
                    ret = true;
                }
            }
            else
            {
                /* Execution should not reach here */
                transaction->status = MIBSPI_TRANSFER_FAILED;
                ret = false;
            }
        }while(0);

#endif
    /* For master blocking mode, if CS Hold is set clear it in SPIDAT1 reg here. */
    if ((ret == TRUE) && (ptrMibSpiDriver->params.mode == MIBSPI_MASTER) &&
        (ptrMibSpiDriver->params.transferMode == MIBSPI_MODE_BLOCKING) &&
        (ptrMibSpiDriver->params.csHold != 0))
        {
            CSL_mss_spiRegs *ptrMibSpiReg = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;
            CSL_FINS(ptrMibSpiReg->SPIDAT1,SPI_SPIDAT1_CSHOLD, 0U);
        }
    }

    /* Disable transfer group */
    MIBSPI_transferGroupDisable((ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase), transaction->slaveIndex);
    
    /* Clear transaction handle */
    key = HwiP_disable();

    ptrMibSpiDriver->transactionState.transaction = NULL;

    HwiP_restore(key);
    return ret;
}

/**
 *  @b Description
 *  @n
 *      SPI interface funtion to cancell an ongoing transaction.
 *
 *   @param[in] handle          SPI device handle
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None
 */
void MIBSPI_transferCancelCore(MIBSPI_Handle handle)
{
    /* Stop Transfer is in progress */
    MIBSPITRACE_ERROR("SPI:(%p) Error! Stop Transaction is  not supported\n", handle);

    /* This API is not supported, hence throw an assertion */
    Mibspi_assert(0);

    return;
}


/**
 *  @b Description
 *  @n
 *      Get SPI driver internal stats
 *
 *   @param[in] handle           SPI device handle
 *   @param[in] ptrStats         pointer to SPI driver stats structure
 *
 *  \ingroup SPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None
 */
static int32_t MIBSPI_getDrvStats(MIBSPI_Handle handle, MIBSPI_Stats *ptrStats)
{
    MibSpiDriver_Object     *ptrMibSpiDriver;
    int32_t                 retCode = 0;

    /* Validate input parameters */
    if((handle == NULL) || (ptrStats == NULL))
    {
        retCode = MIBSPI_STATUS_ERROR;
    }
    else
    {
        /* Get the MibSpi driver handle */
        ptrMibSpiDriver = (MibSpiDriver_Object *)handle->object;

        ptrStats->dlenErr = ptrMibSpiDriver->hwStats.dlenErr;
        ptrStats->timeout = ptrMibSpiDriver->hwStats.timeout;
        ptrStats->parErr = ptrMibSpiDriver->hwStats.parErr;
        ptrStats->desync = ptrMibSpiDriver->hwStats.desync;
        ptrStats->bitErr = ptrMibSpiDriver->hwStats.bitErr;
        ptrStats->rxOvrnErr =  ptrMibSpiDriver->hwStats.rxOvrnErr;
    }
    
    return retCode;
}



void MIBSPI_dmaDoneCb(MIBSPI_Handle mibspiHandle)
{
    MibSpiDriver_Object*    ptrMibSpiDriver = NULL;
    MIBSPI_Config*             ptrSPIConfig;
    CSL_mss_spiRegs  *ptrMibSpiReg;

    /* Get the SPI driver Configuration: */
    ptrSPIConfig = (MIBSPI_Config*)mibspiHandle;

    ptrMibSpiDriver = (MibSpiDriver_Object*)ptrSPIConfig->object;

    ptrMibSpiReg = ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase;
    if ((CSL_FEXT(ptrMibSpiReg->IOLPBKTSTCR,SPI_IOLPBKTSTCR_LPBKTYPE) == MIBSPI_LOOPBK_ANALOG)
        &&
        (CSL_FEXT(ptrMibSpiReg->SPIGCR1,SPI_SPIGCR1_MASTER) == CSL_SPI_SPIGCR1_MASTER_SLAVE)
        &&
        (CSL_FEXT(ptrMibSpiReg->IOLPBKTSTCR,SPI_IOLPBKTSTCR_IOLPBKTSTENA) ==  0xAU))
    {
        CSL_FINS(ptrMibSpiReg->IOLPBKTSTCR,SPI_IOLPBKTSTCR_IOLPBKTSTENA, 0x5U);
    }

    if ((ptrMibSpiDriver->params.transferMode == MIBSPI_MODE_BLOCKING) && (ptrMibSpiDriver->transferCompleteSem != NULL) )
    {
        /* MibSPI-RX DMA complete interrupt */
        SemaphoreP_post(ptrMibSpiDriver->transferCompleteSem);
    }
    else
    {
        if (ptrMibSpiDriver->transactionState.transaction != NULL)
        {
            /* Call the transfer completion callback function */
            if ((ptrMibSpiDriver->transactionState.transaction->status == MIBSPI_TRANSFER_STARTED) &&
                (ptrMibSpiDriver->transactionState.transferErr == MIBSPI_XFER_ERR_NONE) &&
                (ptrMibSpiDriver->transactionState.remainSize > 0U))
            {
                /* Change buffer pointer and data size for the second transfer */
                ptrMibSpiDriver->transactionState.transaction->txBuf = (void *)((ptrMibSpiDriver->transactionState.transaction->txBuf == NULL) ?
                    NULL : (uint8_t *)ptrMibSpiDriver->transactionState.transaction->txBuf +
                    ptrMibSpiDriver->transactionState.dataLength * (ptrMibSpiDriver->transactionState.dataSizeInBytes));
                ptrMibSpiDriver->transactionState.transaction->rxBuf = (void *)((ptrMibSpiDriver->transactionState.transaction->rxBuf == NULL) ?
                    NULL : (uint8_t *)ptrMibSpiDriver->transactionState.transaction->rxBuf +
                    ptrMibSpiDriver->transactionState.dataLength * (ptrMibSpiDriver->transactionState.dataSizeInBytes));
                ptrMibSpiDriver->transactionState.dataLength = ptrMibSpiDriver->transactionState.remainSize;
                ptrMibSpiDriver->transactionState.remainSize = 0U;

                /* Transfer the remaining size */
                if(ptrMibSpiDriver->params.mode == MIBSPI_SLAVE)
                {
                    MIBSPI_dataTransfer(ptrMibSpiDriver, (uint8_t *)ptrMibSpiDriver->transactionState.transaction->txBuf,
                        (uint8_t *)ptrMibSpiDriver->transactionState.transaction->rxBuf, ptrMibSpiDriver->transactionState.dataLength, MIBSPI_SLAVEMODE_TRANS_GROUP);
                }
                else
                {
                    MIBSPI_dataTransfer(ptrMibSpiDriver, (uint8_t *)ptrMibSpiDriver->transactionState.transaction->txBuf,
                        (uint8_t *)ptrMibSpiDriver->transactionState.transaction->rxBuf, ptrMibSpiDriver->transactionState.dataLength, ptrMibSpiDriver->transactionState.transaction->slaveIndex);
                }
            }
            else if ((ptrMibSpiDriver->transactionState.transaction->status == MIBSPI_TRANSFER_STARTED) &&
                (ptrMibSpiDriver->transactionState.transferErr == MIBSPI_XFER_ERR_NONE))
            {
                /* Update status*/
                ptrMibSpiDriver->transactionState.transaction->status = MIBSPI_TRANSFER_COMPLETED;
            }
            else
            {
                /* Update status */
                ptrMibSpiDriver->transactionState.transaction->status = MIBSPI_TRANSFER_FAILED;
            }

            /* For master blocking mode, if CS Hold is set clear it in SPIDAT1 reg here. */
            if ((ptrMibSpiDriver->transactionState.transaction->status == MIBSPI_TRANSFER_COMPLETED) &&
                (ptrMibSpiDriver->params.mode == MIBSPI_MASTER) &&
                (ptrMibSpiDriver->params.csHold != 0))
            {
                CSL_FINS(ptrMibSpiReg->SPIDAT1,SPI_SPIDAT1_CSHOLD, 0U);
            }

            if ((ptrMibSpiDriver->transactionState.transaction->status == MIBSPI_TRANSFER_COMPLETED) ||
                (ptrMibSpiDriver->transactionState.transaction->status == MIBSPI_TRANSFER_FAILED))
            {
                /* Call transferCallbackFxn */
                ptrMibSpiDriver->params.transferCallbackFxn(mibspiHandle, ptrMibSpiDriver->transactionState.transaction);

                /* Disable transfer group */
                MIBSPI_transferGroupDisable((ptrMibSpiDriver->ptrHwCfg->ptrSpiRegBase), ptrMibSpiDriver->transactionState.transaction->slaveIndex);

                /* Reset transaction state */
                MIBSPI_resetTransactionState(&ptrMibSpiDriver->transactionState);
            }
        }
    }
}

static uint32_t MIBSPI_getPhase(MIBSPI_FrameFormat frameFormat)
{
    uint32_t phase = 0;

    switch (frameFormat)
    {
        case MIBSPI_POL0_PHA0:    /*!< SPI mode Polarity 0 Phase 0 */
        case MIBSPI_POL1_PHA0:    /*!< SPI mode Polarity 1 Phase 0 */
            phase = 0;
            break;
        case MIBSPI_POL0_PHA1:    /*!< SPI mode Polarity 0 Phase 1 */
        case MIBSPI_POL1_PHA1:    /*!< SPI mode Polarity 1 Phase 1 */
            phase = 1;
            break;
        default:
            Mibspi_assert(FALSE);
    }
    return phase;
}

static uint32_t MIBSPI_getPolarity(MIBSPI_FrameFormat frameFormat)
{
    uint32_t polarity = 0;

    switch (frameFormat)
    {
        case MIBSPI_POL0_PHA0:    /*!< SPI mode Polarity 0 Phase 0 */
        case MIBSPI_POL0_PHA1:    /*!< SPI mode Polarity 0 Phase 1 */
            polarity = 0;
            break;
        case MIBSPI_POL1_PHA0:    /*!< SPI mode Polarity 1 Phase 0 */
        case MIBSPI_POL1_PHA1:    /*!< SPI mode Polarity 1 Phase 1 */
            polarity = 1;
            break;
        default:
            Mibspi_assert(FALSE);
    }
    return polarity;
}

static void MIBSPI_getVersionInfo(const CSL_mss_spiRegs  *ptrMibSpiReg, MibSpi_VersionInfo *ver)
{
    ver->minor = CSL_FEXT(ptrMibSpiReg->SPIREV,SPI_SPIREV_MINOR);
    ver->custom = CSL_FEXT(ptrMibSpiReg->SPIREV,SPI_SPIREV_CUSTOM);
    ver->major = CSL_FEXT(ptrMibSpiReg->SPIREV,SPI_SPIREV_MAJOR);
    ver->rtl = CSL_FEXT(ptrMibSpiReg->SPIREV,SPI_SPIREV_RTL);
    ver->functionality = CSL_FEXT(ptrMibSpiReg->SPIREV,SPI_SPIREV_FUNC);
    ver->scheme = CSL_FEXT(ptrMibSpiReg->SPIREV,SPI_SPIREV_SCHEME);

}

static void MIBSPI_validateIPVersionInfo(const MibSpi_HwCfg* ptrHwCfg)
{
    MibSpi_VersionInfo versionInfo;

    MIBSPI_getVersionInfo(ptrHwCfg->ptrSpiRegBase, &versionInfo);
    Mibspi_assert(versionInfo.custom == ptrHwCfg->versionInfo.custom);
    Mibspi_assert(versionInfo.minor  == ptrHwCfg->versionInfo.minor);
    Mibspi_assert(versionInfo.major  == ptrHwCfg->versionInfo.major);
    Mibspi_assert(versionInfo.rtl    == ptrHwCfg->versionInfo.rtl);
    Mibspi_assert(versionInfo.functionality == ptrHwCfg->versionInfo.functionality);
    Mibspi_assert(versionInfo.scheme == ptrHwCfg->versionInfo.scheme);
}

static void MIBSPI_enableErrorInterrupt(CSL_mss_spiRegs  *ptrMibSpiReg, uint32_t enableFlag)
{
    uint32_t regVal = ptrMibSpiReg->SPIINT0;

    CSL_FINS(regVal, SPI_SPIINT0_DLENERRENA, enableFlag);
    CSL_FINS(regVal, SPI_SPIINT0_TIMEOUTENA, enableFlag);
    CSL_FINS(regVal, SPI_SPIINT0_PARERRENA, enableFlag);
    CSL_FINS(regVal, SPI_SPIINT0_DESYNCENA, enableFlag);
    CSL_FINS(regVal, SPI_SPIINT0_BITERRENA, enableFlag);
    CSL_FINS(regVal, SPI_SPIINT0_OVRNINTENA, enableFlag);
    ptrMibSpiReg->SPIINT0 = regVal;
}

static void MIBSPI_setErrorInterruptLevel(CSL_mss_spiRegs  *ptrMibSpiReg, uint32_t level)
{
    uint32_t regVal = ptrMibSpiReg->SPILVL;

    CSL_FINS(regVal, SPI_SPILVL_DLENERRLVL, level);
    CSL_FINS(regVal, SPI_SPILVL_TIMEOUTLVL, level);
    CSL_FINS(regVal, SPI_SPILVL_PARERRLVL, level);
    CSL_FINS(regVal, SPI_SPILVL_DESYNCLVL, level);
    CSL_FINS(regVal, SPI_SPILVL_BITERRLVL, level);
    CSL_FINS(regVal, SPI_SPILVL_OVRNINTLVL, level);
    ptrMibSpiReg->SPILVL = regVal;
}


