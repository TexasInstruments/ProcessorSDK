/**
 *   @file  uartsci.c
 *
 *   @brief
 *      The file implements the UART Serial Communication Interface Driver
 *      for the XWR14xx and XWR16xx. The file implements the driver to conform
 *      to the MCPI standards.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016-2020 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/src/UART_osal.h>
#include <ti/drv/uart/src/UART_drv_log.h>
#include <ti/drv/uart/src/v3/uartsci.h>
#include <ti/csl/cslr_sci.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/* Registered callback functions: */
static void         UartSci_close(UART_Handle handle);
static void         UartSci_init(UART_Handle handle);
static UART_Handle  UartSci_open(UART_Handle handle, const UART_Params *params);
static int32_t      UartSci_read(UART_Handle handle, void *buffer, size_t size);
static int32_t      UartSci_readPolling(UART_Handle handle, void *buffer, size_t size);
static void         UartSci_readCancel(UART_Handle handle);
static int32_t      UartSci_write(UART_Handle handle, const void *buffer, size_t size);
static int32_t      UartSci_writePolling(UART_Handle handle, const void* buffer, size_t size);
static void         UartSci_writeCancel(UART_Handle handle);
static int32_t      UartSci_control(UART_Handle handle, uint32_t cmd, void *arg);
static int32_t      UartSci_read2(UART_Handle handle, UART_Transaction *transaction);
static int32_t      UartSci_write2(UART_Handle handle, UART_Transaction *transaction);

/* Read/Write SCI Registers API: */
static void     UartSci_enableTxInterrupt (CSL_sciRegs* ptrSCIRegs);
static void     UartSci_disableTxInterrupt (CSL_sciRegs* ptrSCIRegs);
static uint32_t UartSci_isTxInterruptEnabled (const CSL_sciRegs* ptrSCIRegs);
static void     UartSci_enableRxInterrupt (CSL_sciRegs* ptrSCIRegs);
static void     UartSci_disableRxInterrupt (CSL_sciRegs* ptrSCIRegs);
static uint32_t UartSci_isRxInterruptEnabled (const CSL_sciRegs* ptrSCIRegs);
static uint32_t UartSci_isRxFree (const CSL_sciRegs* ptrSCIRegs);
static uint32_t UartSci_isTxFree (const CSL_sciRegs* ptrSCIRegs);
static uint32_t UartSci_isRxOverrun (const CSL_sciRegs* ptrSCIRegs);
static void     UartSci_clearRxOverrun (CSL_sciRegs* ptrSCIRegs);
static uint32_t UartSci_isParityError (const CSL_sciRegs* ptrSCIRegs);
static void     UartSci_clearParityError (CSL_sciRegs* ptrSCIRegs);
static uint32_t UartSci_isFramingError (const CSL_sciRegs* ptrSCIRegs);
static void     UartSci_clearFramingError (CSL_sciRegs* ptrSCIRegs);
static uint8_t  UartSci_getCh (const CSL_sciRegs* ptrSCIRegs);
static void     UartSci_putCh (CSL_sciRegs* ptrSCIRegs, uint8_t ch);

/* SCI Driver API: */
static void    UartSci_ISR (uintptr_t arg);
static int32_t UartSci_validateParams (const UART_Params* params, UartSci_HwCfg* ptrHwCfg);
static void    UartSci_writeChar(UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg);
static void    UartSci_readChar(UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg);

/**************************************************************************
 ************************** Local Declarations ****************************
 **************************************************************************/

/**
 * @brief   Global Constant CR
 */
static const uint8_t   CONST_CR =  (uint8_t)'\r';

/**
 * @brief   Global Constant LF
 */
static const uint8_t   CONST_LF =  (uint8_t)'\n';

/** @brief
 *
 *  Defines SCI frame format: number of stop bits
 */
typedef uint32_t UARTSCI_FORMAT_STOPBIT;
#define UARTSCI_FORMAT_STOPBIT_ONE           ((uint32_t) 0U)
#define UARTSCI_FORMAT_STOPBIT_TWO           ((uint32_t) 1U)

/** @brief
 *
 *  Defines SCI frame format: partty type
 */
typedef uint32_t UARTSCI_FORMAT_PARITY_TYPE;
#define UARTSCI_FORMAT_PARITY_NONE           ((uint32_t) 0U)
#define UARTSCI_FORMAT_PARITY_EVEN           ((uint32_t) 1U)
#define UARTSCI_FORMAT_PARITY_ODD            ((uint32_t) 2U)

/** @brief
 *
 *  Defines SCI frame format: character length in bits
 */
typedef uint32_t UARTSCI_FORMAT_CHAR_LEN;
#define UARTSCI_FORMAT_CHAR_LEN_1           ((uint32_t) 0U)
#define UARTSCI_FORMAT_CHAR_LEN_2           ((uint32_t) 1U)
#define UARTSCI_FORMAT_CHAR_LEN_3           ((uint32_t) 2U)
#define UARTSCI_FORMAT_CHAR_LEN_4           ((uint32_t) 3U)
#define UARTSCI_FORMAT_CHAR_LEN_5           ((uint32_t) 4U)
#define UARTSCI_FORMAT_CHAR_LEN_6           ((uint32_t) 5U)
#define UARTSCI_FORMAT_CHAR_LEN_7           ((uint32_t) 6U)
#define UARTSCI_FORMAT_CHAR_LEN_8           ((uint32_t) 7U)
#define UARTSCI_FORMAT_CHAR_LEN_MAX         UARTSCI_FORMAT_CHAR_LEN_8

/** @brief
 *
 *  Defines SCI Timing mode
 */
typedef uint32_t UARTSCI_TIMING_MODE;
#define UARTSCI_TIMING_MODE_SYNC            ((uint32_t) 0U)
#define UARTSCI_TIMING_MODE_ASYNC           ((uint32_t) 1U)

/** @brief
 *
 *  Defines SCI Clock mode
 */
typedef uint32_t UARTSCI_CLOCK_MODE;
#define UARTSCI_CLOCK_MODE_EXTERNAL         ((uint32_t) 0U)
#define UARTSCI_CLOCK_MODE_INTERNAL         ((uint32_t) 1U)

/** @brief
 *
 *  Defines SCI Communication mode
 */
typedef uint32_t UARTSCI_COMM_MODE;
#define UARTSCI_COMM_MODE_IDLE              ((uint32_t) 0U)
#define UARTSCI_COMM_MODE_ADDRESS           ((uint32_t) 1U)

/** @brief
 *
 *  Defines SCI interrupt indication
 */
typedef uint32_t UARTSCI_INT_IND;
#define UARTSCI_INT_IND_BREAK               ((uint32_t) 1U)
#define UARTSCI_INT_IND_PE                  ((uint32_t) 2U)
#define UARTSCI_INT_IND_FE                  ((uint32_t) 3U)
#define UARTSCI_INT_IND_BREAK_DET_ERR       ((uint32_t) 4U)
#define UARTSCI_INT_IND_OE                  ((uint32_t) 5U)
#define UARTSCI_INT_IND_RX                  ((uint32_t) 6U)
#define UARTSCI_INT_IND_TX                  ((uint32_t) 7U)

/** @brief
 *
 *  Defines SCI interrupt contrl bitmap
 */
#define UARTSCI_INT_BITMAP_BREAK           ((uint32_t) 0x01U)
#define UARTSCI_INT_BITMAP_WAKEUP          ((uint32_t) 0x02U)
#define UARTSCI_INT_BITMAP_TX              ((uint32_t) 0x04U)
#define UARTSCI_INT_BITMAP_RX              ((uint32_t) 0x08U)
#define UARTSCI_INT_BITMAP_PE              ((uint32_t) 0x10U)
#define UARTSCI_INT_BITMAP_OE              ((uint32_t) 0x20U)
#define UARTSCI_INT_BITMAP_FE              ((uint32_t) 0x30U)

/** @brief
 *
 *  Holds the SCI UART configuration info.
 */
typedef struct UART_SCI_CONFI_s {
    /**  Receive Enable */
    uint32_t                    rxEn;

    /**  Transmit Enable */
    uint32_t                    txEn;

    /**  Clock Mode internal or external
     * @note: If an external clock source is selected, the internal baud rate
     *        generator is bypassed. The maximum frequency allowed for external
     *        clock is VCLK/16
     */
    UARTSCI_CLOCK_MODE          clkMode;

    /** Timing mode: Asynchronous or Synchronous mode
     */
    UARTSCI_TIMING_MODE         timingMode;

    /** Communication mode: idle-line or address-bit for multiprocessor operation
     *  only
     */
    UARTSCI_COMM_MODE           commMode;

    /** Number of stop bits: one or two */
    UARTSCI_FORMAT_STOPBIT      stopBits;

    /** Parity: none, even or odd */
    UARTSCI_FORMAT_PARITY_TYPE  parityType;

    /** Data length in bits */
    UARTSCI_FORMAT_CHAR_LEN     charLen;

    /** Baud rate */
    uint32_t                    baudRate;

    /** Input Clock Frequency */
    uint32_t                    clockFrequency;

    /** flag to indicate in loopback mode  */
    uint32_t                    loopback;

} UART_SCI_CONFIG;

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   UART-SCI Driver Function Table
 */
UART_FxnTable gUartSciFxnTable =
{
    &UartSci_close,
    &UartSci_control,
    &UartSci_init,
    &UartSci_open,
    &UartSci_read,
    &UartSci_readPolling,
    &UartSci_readCancel,
    &UartSci_write,
    &UartSci_writePolling,
    &UartSci_writeCancel,
    &UartSci_read2,
    &UartSci_write2,
};

/**************************************************************************
 ************************* UART SCI Driver Functions **********************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the specific SCI
 *      Instance by taking it out of reset
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_enable (CSL_sciRegs* ptrSCIRegs)
{
    CSL_FINS(ptrSCIRegs->SCIGCR0, SCI_SCIGCR0_RESET, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to check if the specific SCI
 *      Instance is enabled or not
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   SCI is enabled
 *  @retval
 *      0   -   SCI is disabled
 */
uint32_t UartSci_isEnabled (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCIGCR0, SCI_SCIGCR0_RESET);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the specific SCI
 *      Instance by putting it into reset state
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disable (CSL_sciRegs* ptrSCIRegs)
{
    CSL_FINS(ptrSCIRegs->SCIGCR0, SCI_SCIGCR0_RESET, 0U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to put the specific SCI
 *      Instance into software reset for configuration
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_enableSwReset (CSL_sciRegs* ptrSCIRegs)
{
    CSL_FINS(ptrSCIRegs->SCIGCR1, SCI_SCIGCR1_SW_NRESET, 0U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to check if the specific SCI
 *      Instance is at Software Reset state
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   SCI is at software reset
 *  @retval
 *      0   -   SCI is operational
 */
uint32_t UartSci_isSwReset (const CSL_sciRegs* ptrSCIRegs)
{
    return (CSL_FEXT(ptrSCIRegs->SCIGCR1, SCI_SCIGCR1_SW_NRESET)?0U:1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to take the specific SCI
 *      instance out of software reset for normal operation
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disableSwReset (CSL_sciRegs* ptrSCIRegs)
{
    CSL_FINS(ptrSCIRegs->SCIGCR1, SCI_SCIGCR1_SW_NRESET, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable all interrupts
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disableAllInterrupts (CSL_sciRegs* ptrSCIRegs)
{
    CSL_REG_WR(&ptrSCIRegs->SCICLEARINT, 0xFFFFFFFFU);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to configure the specific SCI
 *      Instance to its desired operation modes
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *  @param[in]  ptrSCIConfig
 *      Point to the Configuartion structure
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *   <b> Return Value </b>
 *   @n   0 = success
 *   @n  -1 = bad state or bad parameters
 *
 *   <b> Pre Condition </b>
 *   @n  SCI should be at software reset state.
 */
int32_t UartSci_config (CSL_sciRegs* ptrSCIRegs, UART_SCI_CONFIG* ptrSCIConfig)
{
    uint32_t resetVal;
    uint32_t val32 = 0;
    int32_t retVal = 0;
    uint32_t divisionFactor = 1U;

    resetVal = UartSci_isSwReset(ptrSCIRegs);
    if (resetVal == 0)
    {
        retVal = -1;
    }
    else
    {
        if (ptrSCIConfig->txEn)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_TXENA, 1U);
            CSL_FINS(ptrSCIRegs->SCIPIO0, SCI_SCIPIO0_TX_FUNC, 1U);
        }

        if (ptrSCIConfig->rxEn)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_RXENA, 1U);
            CSL_FINS(ptrSCIRegs->SCIPIO0, SCI_SCIPIO0_RX_FUNC, 1U);
        }

        if (ptrSCIConfig->clkMode == UARTSCI_CLOCK_MODE_INTERNAL)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_CLOCK, 1U);
            CSL_FINS(ptrSCIRegs->SCIPIO0, SCI_SCIPIO0_CLK_FUNC, 1U);
        }

        if (ptrSCIConfig->timingMode == UARTSCI_TIMING_MODE_ASYNC)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_TIMING_MODE, 1U);
            divisionFactor = 16U;
        }

        if (ptrSCIConfig->commMode == UARTSCI_COMM_MODE_ADDRESS)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_COMM_MODE, 1U);
        }

        if (ptrSCIConfig->stopBits == UARTSCI_FORMAT_STOPBIT_TWO)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_STOP, 1U);
        }

        if (ptrSCIConfig->parityType == UARTSCI_FORMAT_PARITY_EVEN)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_PARITY_ENA, 1U) |
                     CSL_FMK (SCI_SCIGCR1_PARITY, 1U);
        }
        else if (ptrSCIConfig->parityType == UARTSCI_FORMAT_PARITY_ODD)
        {
            val32 |= CSL_FMK (SCI_SCIGCR1_PARITY_ENA, 1U);
        }

        CSL_REG_WR(&ptrSCIRegs->SCIGCR1, val32);

        if(ptrSCIConfig->charLen > UARTSCI_FORMAT_CHAR_LEN_MAX)
        {
            retVal = -1;
        }
        else
        {
            CSL_REG_WR(&ptrSCIRegs->SCICHAR, CSL_FMK(SCI_SCICHAR_CHAR, ptrSCIConfig->charLen));
        }

        /* Calculate the baud value to generate the desired baud rate */
        if (ptrSCIConfig->baudRate == 0U)
        {
            val32 = ptrSCIConfig->clockFrequency/32U;
        }
        else
        {
            val32 = ptrSCIConfig->clockFrequency/(ptrSCIConfig->baudRate * divisionFactor) - 1U;
        }
        CSL_REG_WR(&ptrSCIRegs->SCIBAUD, val32);

        if (ptrSCIConfig->loopback)
        {
            /* Enable Loopback: */
            CSL_FINS(ptrSCIRegs->SCIGCR1, SCI_SCIGCR1_LOOP_BACK , 1U);
        }
    }

    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable multiple interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *  @param[in]  intBitMap
 *      Intrrupts to be enabled
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_enableInterrupts (CSL_sciRegs* ptrSCIRegs, uint32_t intBitMap)
{
    uint32_t val32 = 0;

    if(intBitMap & UARTSCI_INT_BITMAP_BREAK)
    {
        val32 |= CSL_FMK(SCI_SCISETINT_SET_BRKDT_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_WAKEUP)
    {
        val32 |= CSL_FMK(SCI_SCISETINT_SET_WAKEUP_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_TX)
    {
        val32 |= CSL_FMK(SCI_SCISETINT_SET_TX_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_RX)
    {
        val32 |= CSL_FMK(SCI_SCISETINT_SET_RX_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_PE)
    {
        val32 |= CSL_FMK(SCI_SCISETINT_SET_PE_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_OE)
    {
        val32 |= CSL_FMK(SCI_SCISETINT_SET_OE_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_FE)
    {
        val32 |= CSL_FMK(SCI_SCISETINT_SET_FE_INT, 1U);
    }

    CSL_REG_WR(&ptrSCIRegs->SCISETINT, val32);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable multiple interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *  @param[in]  intBitMap
 *      Intrrupts to be enabled
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disableInterrupts (CSL_sciRegs* ptrSCIRegs, uint32_t intBitMap)
{
    uint32_t val32 = 0;

    if(intBitMap & UARTSCI_INT_BITMAP_BREAK)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINT_CLR_BRKDT_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_WAKEUP)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINT_CLR_WAKEUP_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_TX)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINT_CLR_TX_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_RX)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINT_CLR_RX_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_PE)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINT_CLR_PE_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_OE)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINT_CLR_OE_INT, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_FE)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINT_CLR_FE_INT, 1U);
    }

    CSL_REG_WR(&ptrSCIRegs->SCICLEARINT, val32);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to set multiple interrupt
 *      levels from int0 to int1 for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *  @param[in]  intBitMap
 *      Intrrupts to be enabled
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_setInterruptLevels (CSL_sciRegs* ptrSCIRegs, uint32_t intBitMap)
{
    uint32_t val32 = 0;

    if(intBitMap & UARTSCI_INT_BITMAP_BREAK)
    {
        val32 |= CSL_FMK(SCI_SCISETINTLVL_SET_BRKDT_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_WAKEUP)
    {
        val32 |= CSL_FMK(SCI_SCISETINTLVL_SET_WAKEUP_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_TX)
    {
        val32 |= CSL_FMK(SCI_SCISETINTLVL_SET_TX_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_RX)
    {
        val32 |= CSL_FMK(SCI_SCISETINTLVL_SET_RX_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_PE)
    {
        val32 |= CSL_FMK(SCI_SCISETINTLVL_SET_PE_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_OE)
    {
        val32 |= CSL_FMK(SCI_SCISETINTLVL_SET_OE_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_FE)
    {
        val32 |= CSL_FMK(SCI_SCISETINTLVL_SET_FE_INT_LVL, 1U);
    }

    CSL_REG_WR(&ptrSCIRegs->SCISETINTLVL, val32);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear multiple interrupt
 *      levels back from int1 to int0 for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *  @param[in]  intBitMap
 *      Intrrupts to be enabled
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_clearInterruptLevels (CSL_sciRegs* ptrSCIRegs, uint32_t intBitMap)
{
    uint32_t val32 = 0;

    if(intBitMap & UARTSCI_INT_BITMAP_BREAK)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINTLVL_CLR_BRKDT_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_WAKEUP)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINTLVL_CLR_WAKEUP_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_TX)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINTLVL_CLR_TX_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_RX)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINTLVL_CLR_RX_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_PE)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINTLVL_CLR_PE_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_OE)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINTLVL_CLR_OE_INT_LVL, 1U);
    }

    if(intBitMap & UARTSCI_INT_BITMAP_FE)
    {
        val32 |= CSL_FMK(SCI_SCICLEARINTLVL_CLR_FE_INT_LVL, 1U);
    }

    CSL_REG_WR(&ptrSCIRegs->SCICLEARINTLVL, val32);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear all interrupt
 *      levels back to int0 for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_clearAllInterruptLevels (CSL_sciRegs* ptrSCIRegs)
{
    CSL_REG_WR(&ptrSCIRegs->SCICLEARINTLVL, 0xFFFFFFFFU);
}
/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable/disable loopback mode
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *  @param[in]  enable
 *      0/1: disable/enable loopback
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_loopbackControl (CSL_sciRegs* ptrSCIRegs, uint32_t enable)
{
    uint32_t resetVal = UartSci_isSwReset(ptrSCIRegs);

    if (resetVal == 1U)
    {
        UartSci_enableSwReset(ptrSCIRegs);
    }

    if (enable == 1U)
    {
        CSL_FINS(ptrSCIRegs->SCIGCR1, SCI_SCIGCR1_LOOP_BACK , 1U);
    }
    else
    {
        CSL_FINS(ptrSCIRegs->SCIGCR1, SCI_SCIGCR1_LOOP_BACK , 0U);
    }

    if (resetVal == 1U)
    {
        UartSci_disableSwReset(ptrSCIRegs);
    }
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the transmit interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_enableTxInterrupt (CSL_sciRegs* ptrSCIRegs)
{
    CSL_FINS(ptrSCIRegs->SCISETINT, SCI_SCISETINT_SET_TX_INT, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to check if the transmit interrupt
 *      for the specific SCI Instance is enabled or not
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Interrupt is enabled
 *  @retval
 *      0   -   Interrupt is disabled
 */
static uint32_t UartSci_isTxInterruptEnabled (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCISETINT, SCI_SCISETINT_SET_TX_INT);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the transmit DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_enableTxDMA (CSL_sciRegs* ptrSCIRegs)
{
    /* Enable Transmit DMA */
    CSL_FINS(ptrSCIRegs->SCISETINT, SCI_SCISETINT_SET_TX_DMA, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the transmit DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disableTxDMA (CSL_sciRegs* ptrSCIRegs)
{
    /* Disable the Transmit DMA */
    CSL_REG_WR(&ptrSCIRegs->SCICLEARINT, CSL_FMK(SCI_SCICLEARINT_CLR_TX_DMA, 1U));
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the transmit interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_disableTxInterrupt (CSL_sciRegs* ptrSCIRegs)
{
    CSL_REG_WR(&ptrSCIRegs->SCICLEARINT, CSL_FMK(SCI_SCICLEARINT_CLR_TX_INT, 1U));
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the receive interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_enableRxInterrupt (CSL_sciRegs* ptrSCIRegs)
{
    CSL_FINS(ptrSCIRegs->SCISETINT, SCI_SCISETINT_SET_RX_INT, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to check if the transmit interrupt
 *      for the specific SCI Instance is enabled or not
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Interrupt is enabled
 *  @retval
 *      0   -   Interrupt is disabled
 */
static uint32_t UartSci_isRxInterruptEnabled (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCISETINT, SCI_SCISETINT_SET_RX_INT);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the receive interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_disableRxInterrupt (CSL_sciRegs* ptrSCIRegs)
{
    CSL_REG_WR(&ptrSCIRegs->SCICLEARINT,
               CSL_FMK(SCI_SCICLEARINT_CLR_RX_INT, 1U));
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the receive DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_enableRxDMA (CSL_sciRegs* ptrSCIRegs)
{
    /* Enable the Rx DMA All and Rx DMA */
    CSL_FINS(ptrSCIRegs->SCISETINT, SCI_SCISETINT_SET_RX_DMA_ALL, 1U);
    CSL_FINS(ptrSCIRegs->SCISETINT, SCI_SCISETINT_SET_RX_DMA, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the receive DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disableRxDMA (CSL_sciRegs* ptrSCIRegs)
{
    /* Disable the Rx DMA All and Rx DMA */
    CSL_REG_WR(&ptrSCIRegs->SCICLEARINT, CSL_FMK(SCI_SCICLEARINT_CLR_RX_DMA_ALL, 1U));
    CSL_REG_WR(&ptrSCIRegs->SCICLEARINT, CSL_FMK(SCI_SCICLEARINT_CLR_RX_DMA, 1U));
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if the SCI receiver
 *      has been overrun or not?
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Receiver overrun has been detected
 *  @retval
 *      0   -   Receiver overrun not detected
 */
static uint32_t UartSci_isRxOverrun (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCIFLR, SCI_SCIFLR_OE);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear the SCI receiver.
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_clearRxOverrun (CSL_sciRegs* ptrSCIRegs)
{
    /* Write a 1 to the Overrun bit will clear the status of the overrun */
    CSL_REG_WR(&ptrSCIRegs->SCIFLR, CSL_FMK(SCI_SCIFLR_OE, 1U));
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if there is a
 *      framing error or not?
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Framing error has been detected
 *  @retval
 *      0   -   Framing error has not been detected
 */
static uint32_t UartSci_isFramingError (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCIFLR, SCI_SCIFLR_FE);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear the framing error
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_clearFramingError (CSL_sciRegs* ptrSCIRegs)
{
    /* Write a 1 to the Overrun bit will clear the status of the overrun */
    CSL_REG_WR(&ptrSCIRegs->SCIFLR, CSL_FMK(SCI_SCIFLR_FE, 1U));
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if there is a
 *      parity error or not?
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Parity error has been detected
 *  @retval
 *      0   -   Parity error has not been detected
 */
static uint32_t UartSci_isParityError (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCIFLR, SCI_SCIFLR_PE);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear the parity error
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_clearParityError (CSL_sciRegs* ptrSCIRegs)
{
    /* Write a 1 to the Parity Error bit will clear the status */
    CSL_REG_WR(&ptrSCIRegs->SCIFLR, CSL_FMK(SCI_SCIFLR_PE, 1U));
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if the SCI receiver
 *      is free or not.
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Receiver is free
 *  @retval
 *      0   -   Receiver is not free
 */
static uint32_t UartSci_isRxFree (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCIFLR, SCI_SCIFLR_RXRDY);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if the SCI transmitter
 *      is free or not.
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Transmitter is free
 *  @retval
 *      0   -   Transmitter is not free
 */
static uint32_t UartSci_isTxFree (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCIFLR, SCI_SCIFLR_TXRDY);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if the SCI transmitter
 *      is empty, i.e. there is no data transmission in progress
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Transmitter is empty
 *  @retval
 *      0   -   Transmitter is not empty
 */
static uint32_t UartSci_isTxEmpty (const CSL_sciRegs* ptrSCIRegs)
{
    return CSL_FEXT(ptrSCIRegs->SCIFLR, SCI_SCIFLR_TX_EMPTY);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to put a character
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *  @param[in]  ch
 *      Character to be placed
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */

static void UartSci_putCh (CSL_sciRegs* ptrSCIRegs, uint8_t ch)
{
    CSL_FINS(ptrSCIRegs->SCITD, SCI_SCITD_TD, ch);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to get a character
 *
 *  @param[in]  ptrSCIRegs
 *      Point to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Character which is read from the SCI Register
 */
static uint8_t UartSci_getCh (const CSL_sciRegs* ptrSCIRegs)
{
    return (uint8_t)CSL_FEXT(ptrSCIRegs->SCIRD, SCI_SCIRD_RD);
}

extern uint64_t TimerP_getTimeInUsecs(void);

/**
 *  @b Description
 *  @n
 *      Utility function of delay in micro-second
 *
 *  @param[in]  nTicks
 *      dealy ticks in us
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      None
 */
static void Uartsci_usDelay(uint64_t nTicks)
{
    uint64_t start, end;

    start = TimerP_getTimeInUsecs();

    /* Every 32 cycle increment is 1ms */
    do {
        end = TimerP_getTimeInUsecs();
    } while ((end - start) <= nTicks);
}

/**
 *  @b Description
 *  @n
 *      The function is used to write the character from the write buffer
 *      on the UART.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  isRx
 *      true: receive operation
 *      false: transmit operation
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_callback (UART_Handle handle, bool isRx)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Call back to application */
    if (isRx)
    {
        if (UART_MODE_CALLBACK == ptrUartSciDriver->params.readMode)
        {
             ptrUartSciDriver->params.readCallback(handle,
                                                   (void *)ptrUartSciDriver->ptrReadBuffer,
                                                   ptrUartSciDriver->readCount);
        }
        else
        {
            (void)UART_osalPostLock(ptrUartSciDriver->readSem);
        }
    }
    else
    {
        if (UART_MODE_CALLBACK == ptrUartSciDriver->params.writeMode)
        {
             ptrUartSciDriver->params.writeCallback(handle,
                                                    (void *)(ptrUartSciDriver->ptrWriteBuffer),
                                                    ptrUartSciDriver->writeCount);
        }
        else
        {
            (void)UART_osalPostLock( ptrUartSciDriver->writeSem);
        }
    }
}


/**
 *  @b Description
 *  @n
 *      The function is used to write the character from the write buffer
 *      on the UART.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART Driver Hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_writeChar
(
    UartSci_Driver*   ptrUartSciDriver,
    UartSci_HwCfg*    ptrHwCfg
)
{
    /* Is the UART Driver operating in TEXT/BINARY Mode? */
    if (ptrUartSciDriver->params.writeDataMode == UART_DATA_TEXT)
    {
        /* TEXT Mode: */
        if (ptrUartSciDriver->writeCR)
        {
            /* Write the return character */
            UartSci_putCh (ptrHwCfg->ptrSCIRegs, CONST_CR);

            /* Write size is incremented an additional time on the detection of '\n'
             * So we decrement it back again here. */
            ptrUartSciDriver->writeSize--;
            ptrUartSciDriver->writeCount++;
            ptrUartSciDriver->writeCR = 0;
        }
        else
        {
            /* Add a return if next character is a newline. */
            if (*(char *)ptrUartSciDriver->ptrWriteBuffer == CONST_LF)
            {
               ptrUartSciDriver->writeSize++;
               ptrUartSciDriver->writeCR = 1;
            }

            /* Write the character to the transmit buffer: */
            UartSci_putCh (ptrHwCfg->ptrSCIRegs, *(uint8_t*)ptrUartSciDriver->ptrWriteBuffer);

            /* Move to the next character */
            ptrUartSciDriver->ptrWriteBuffer = (uint8_t *)ptrUartSciDriver->ptrWriteBuffer + 1;
            ptrUartSciDriver->writeSize--;
            ptrUartSciDriver->writeCount++;
        }
    }
    else
    {
        /* Binary Mode: */
        UartSci_putCh (ptrHwCfg->ptrSCIRegs, *(uint8_t *)ptrUartSciDriver->ptrWriteBuffer);

        /* Move to the next character */
        ptrUartSciDriver->ptrWriteBuffer = (uint8_t *)ptrUartSciDriver->ptrWriteBuffer + 1;
        ptrUartSciDriver->writeSize--;
        ptrUartSciDriver->writeCount++;
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to read the character and place this into the
 *      read buffer.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART Driver Hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_readChar
(
    UartSci_Driver* ptrUartSciDriver,
    UartSci_HwCfg*  ptrHwCfg
)
{
    uint8_t readIn;

    /* Read the char: */
    readIn = UartSci_getCh (ptrHwCfg->ptrSCIRegs) >> ptrUartSciDriver->shiftJustification;

    /* Is the UART Driver operating in TEXT/BINARY Mode? */
    if (ptrUartSciDriver->params.readDataMode == UART_DATA_TEXT)
    {
        /* TEXT Mode: */
        if (readIn == CONST_CR)
        {
            /* Do we need to echo character? */
            if (ptrUartSciDriver->params.readEcho)
            {
                /* YES: Loop around and wait until TX is ready */
                while (1)
                {
                    if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
                    {
                        UartSci_putCh (ptrHwCfg->ptrSCIRegs, CONST_CR);
                        break;
                    }
                }
            }
            readIn = CONST_LF;
        }
    }

    /* Do we need to echo character? */
    if (ptrUartSciDriver->params.readEcho)
    {
        /* YES: Loop around and wait until TX is ready */
        while (1)
        {
            if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
            {
                UartSci_putCh (ptrHwCfg->ptrSCIRegs, readIn);
                break;
            }
        }
    }

    /* Place the character into the receive buffer and increment the various counters */
    *(uint8_t *)ptrUartSciDriver->ptrReadBuffer = readIn;
    ptrUartSciDriver->ptrReadBuffer = (uint8_t *)ptrUartSciDriver->ptrReadBuffer + 1;
    ptrUartSciDriver->readCount++;
    ptrUartSciDriver->readSize--;

    /* Is the read complete? */
    if ((ptrUartSciDriver->params.readReturnMode == UART_RETURN_NEWLINE) && (readIn == CONST_LF))
    {
        /* YES: In return mode; NEWLINE we can stop the reception operation when we receive
         * a new line character. */
        ptrUartSciDriver->readSize = 0;
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the UART SCI Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_ISR (uintptr_t arg)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)arg;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Increment the total number of interrupts received. */
    ptrUartSciDriver->stats.totalInterrupts++;

    /* Are receive interrupts enabled? Only then do we handle them in the ISR context. */
    if (UartSci_isRxInterruptEnabled (ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the total number of receive interrupts: */
        ptrUartSciDriver->stats.numRxInterrupts++;

        /* Is there a Rx Interrupt? */
        if (UartSci_isRxFree(ptrHwCfg->ptrSCIRegs) == 1U)
        {
            /* YES: Do we have a valid data buffer where we need to place the data? */
            if (ptrUartSciDriver->readSize > 0)
            {
                /* Read the character: */
                UartSci_readChar (ptrUartSciDriver, ptrHwCfg);

                /* Sanity Check: The read size can never be negative */
                UART_osalAssert (ptrUartSciDriver->readSize >= 0);

                /* Are we done with the read buffer ? */
                if (ptrUartSciDriver->readSize == 0)
                {
                    /* Disable RX interrupt until we do a new read */
                    UartSci_disableRxInterrupt(ptrHwCfg->ptrSCIRegs);
                    UartSci_callback((UART_Handle)arg, BTRUE);
                }
            }
            else
            {
                /* We received a character but there was no application provided buffer. We
                 * cannot keep this. */
                ptrUartSciDriver->stats.numDummyRead++;

                /* Dummy read and drop the received character */
                UartSci_getCh (ptrHwCfg->ptrSCIRegs);
            }
        }
    }

    /* Are transmit interrupts enabled? Only then do we handle them in the ISR context. */
    if (UartSci_isTxInterruptEnabled (ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the total number of transmit interrupts: */
        ptrUartSciDriver->stats.numTxInterrupts++;

        /* Is there a Tx Interrupt? */
        if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
        {
            /* YES: Is there any data which needs to be written? */
            if (ptrUartSciDriver->writeSize > 0)
            {
                /* Write the character: */
                UartSci_writeChar (ptrUartSciDriver, ptrHwCfg);

                /* Sanity Check: The write size can never be negative */
                UART_osalAssert (ptrUartSciDriver->writeSize >= 0);

                /* Are we done with the write buffer ? */
                if (ptrUartSciDriver->writeSize <= 0)
                {
                    /* Disable TX interrupt until we do a new write */
                    UartSci_disableTxInterrupt(ptrHwCfg->ptrSCIRegs);
                    UartSci_callback((UART_Handle)arg, BFALSE);
                }
            }
        }
    }

    /* Is there an overrun error? */
    if (UartSci_isRxOverrun(ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the number of overrun counter: */
        ptrUartSciDriver->stats.numRxOverrunInterrupts++;

        /* Clear the overrun status: */
        UartSci_clearRxOverrun (ptrHwCfg->ptrSCIRegs);
    }

    /* Is there a framing error? */
    if (UartSci_isFramingError(ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the stats: */
        ptrUartSciDriver->stats.numFramingErrors++;

        /* Clear the framing error: */
        UartSci_clearFramingError (ptrHwCfg->ptrSCIRegs);
    }

    /* Is there a parity error? */
    if (UartSci_isParityError(ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the stats: */
        ptrUartSciDriver->stats.numParityError++;

        /* Clear the parity error: */
        UartSci_clearParityError (ptrHwCfg->ptrSCIRegs);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be read from the UART Driver. This function implements
 *      a semaphore blocking operation if the UART driver instance is configured
 *      to operate in BLOCKING mode.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data which needs to be read
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been read
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_read(UART_Handle handle, void *buffer, size_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;
    SemaphoreP_Status   status;
    int32_t             retVal = 0;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL) || (ptrHwCfg->duplexity == UartSci_Duplexity_TX_ONLY))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    if(ptrHwCfg->swCfg.enableInterrupt)
    {
        /* Disable preemption while checking if the uart is in use. */
        key = UART_osalHardwareIntDisable();

        /* Is the UART in use? */
        if (ptrUartSciDriver->readSize != 0U)
        {
            /* YES: Restore the interrupts and report the error to the application. */
            UART_osalHardwareIntRestore(key);
            UART_drv_log1 ("UART:(%p) Could not read data, UART in use\n", ptrHwCfg->ptrSCIRegs);
            retVal = UART_EINUSE;
            goto exit;
        }

        /* Save the data to be read */
        ptrUartSciDriver->ptrReadBuffer = (uint8_t *)buffer;
        ptrUartSciDriver->readSize      = (int32_t)size;
        ptrUartSciDriver->readCount     = 0;

        /* Restore the interrupts: */
        UART_osalHardwareIntRestore(key);

#ifdef UART_DMA_ENABLE
        /* Determine the DMA Mode for the Driver: */
        if (BTRUE == ptrUartSciDriver->isDMAEnabled)
        {
            /****************************************************************
            * DMA Mode: Initiate the Receive DMA
            ****************************************************************/
            retVal = ptrHwCfg->initiateRxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, (uint32_t)size, false);
            if (retVal < 0)
            {
                /* Error: Unable to initiate the receive DMA */
                goto exit;
            }
        }
        else
#endif
        {
            /****************************************************************
            * Normal Mode: Enable the receive interrupt
            ****************************************************************/
            UartSci_enableRxInterrupt (ptrHwCfg->ptrSCIRegs);
        }

        if (ptrUartSciDriver->params.readMode == UART_MODE_BLOCKING)
        {
            /* Block the callee; till the UART reads are complete */
            status = UART_osalPendLock (ptrUartSciDriver->readSem, ptrUartSciDriver->params.readTimeout);
            if (status == SemaphoreP_TIMEOUT)
            {
                /* Reset the read size */
                ptrUartSciDriver->readSize = 0;

                /* Report the error condition: */
                UART_drv_log2 ("UART:(%p) Read timed out %d bytes read\n",
                            ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->readCount);
            }

            /* Setup the number of bytes which have been read */
            retVal = ptrUartSciDriver->readCount;
        }
        else
        {
            /*
             * for callback mode, immediately return SUCCESS,
             * once the transaction is done, callback function
             * will return the transaction status and actual
             * read count
             */
            retVal = UART_SUCCESS;
        }
    }
    else
    {
        retVal = UartSci_readPolling(handle, buffer, size);
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be read in polling mode from the UART Driver. The function
 *      will loop around till all the buffer is completely filled up or if the new
 *      line character is received.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data which needs to be read
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been read
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_readPolling(UART_Handle handle, void *buffer, size_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;
    int32_t             retVal;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL) || (ptrHwCfg->duplexity == UartSci_Duplexity_TX_ONLY))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    /* Disable preemption while checking if the uart is in use. */
    key = UART_osalHardwareIntDisable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->readSize != 0)
    {
        /* YES: Restore the interrupts */
        UART_osalHardwareIntRestore(key);

        /* Report the error to the application. */
        UART_drv_log1 ("UART:(%p) Could not read data, UART in use\n", ptrHwCfg->ptrSCIRegs);

        /* Setup the error code: */
        retVal = UART_EINUSE;
        goto exit;
    }

    /* Save the data to be read */
    ptrUartSciDriver->ptrReadBuffer = (uint8_t *)buffer;
    ptrUartSciDriver->readSize      = (int32_t)size;
    ptrUartSciDriver->readCount     = 0;

    /* Restore the interrupts: */
    UART_osalHardwareIntRestore(key);

#ifdef UART_DMA_ENABLE
    /* Determine the DMA Mode for the Driver: */
    if (BTRUE == ptrUartSciDriver->isDMAEnabled)
    {
        /****************************************************************
         * DMA Mode: Initiate the Receive DMA
         ****************************************************************/
        retVal = ptrHwCfg->initiateRxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, size, true);
        if (retVal < 0)
        {
            /* Error: Unable to initiate the receive DMA */
            goto exit;
        }
    }
    else
#endif
    {
        /****************************************************************
         * Normal Mode: Read out all the data
         ****************************************************************/
        bool isWaitForever = (ptrUartSciDriver->params.readTimeout == UART_WAIT_FOREVER);
        uint64_t timeout = ptrUartSciDriver->params.readTimeout * 1000;

        while (ptrUartSciDriver->readSize > 0)
        {
            /* Is the receiver free? */
            if (UartSci_isRxFree(ptrHwCfg->ptrSCIRegs) == 1U)
            {
                /* YES: Read out a character from the buffer. */
                UartSci_readChar (ptrUartSciDriver, ptrHwCfg);
            }
            else if (timeout < 10)
            {
                /* Debug Message: */
                UART_drv_log2 ("Debug: UART(%p) readPolling timeout %d bytes have been read\n",
                                ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->readCount);
                break;
            }
            else if (!isWaitForever)
            {
                Uartsci_usDelay(10);
                timeout -= 10;
            }
        }
    }

    /* Setup the number of bytes which have been read */
    retVal = ptrUartSciDriver->readCount;

exit:
    return retVal;
}

/**
 *  @b Diption
 *  @n
 *      function is the registered callback function which is invoked when
 *      a previous data read operation has to be cancelled.
 *
 *  @parn]  handle
 *      le to the UART Driver
 *
 *  \ing UART_SCI_INTERNAL_FUNCTION
 *
 *  @ret
 *      applicable
 */
static void UartSci_readCancel(UART_Handle handle)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = UART_osalHardwareIntDisable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->readSize == 0)
    {
        /* No: There is nothing to cancel */
        UART_osalHardwareIntRestore(key);
    }
    else
    {
        /* YES: Reset the read size to 0; this will stop the read operations */
        ptrUartSciDriver->readSize = 0;

        /* Determine the DMA Mode for the Driver: */
        if (BTRUE == ptrUartSciDriver->isDMAEnabled)
        {
            /****************************************************************
             * DMA Mode: Disable the Rx DMA
             ****************************************************************/
            UartSci_disableRxDMA (ptrHwCfg->ptrSCIRegs);
        }
        else
        {
            /****************************************************************
             * Normal Mode: Disable the Rx interrupt
             ****************************************************************/
            UartSci_disableRxInterrupt (ptrHwCfg->ptrSCIRegs);
        }

        /* Restore the interrupts: */
        UART_osalHardwareIntRestore(key);

        UartSci_callback(handle, BTRUE);

        /* Debug Message: */
        UART_drv_log2 ("Debug: UART(%p) read canceled %d bytes have been read\n",
                        ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->readCount);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be written to the UART Driver. This function implements
 *      a semaphore blocking operation if the UART driver instance is configured
 *      to operate in BLOCKING mode.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data to be written
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been transferred
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_write(UART_Handle handle, const void *buffer, size_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    SemaphoreP_Status   status;
    uintptr_t           key;
    int32_t             retVal = 0;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    if(ptrHwCfg->swCfg.enableInterrupt)
    {
        /* Disable preemption while checking if the uart is in use. */
        key = UART_osalHardwareIntDisable();

        /* Is the UART in use? */
        if (ptrUartSciDriver->writeSize != 0)
        {
            /* YES: Restore the interrupts */
            UART_osalHardwareIntRestore(key);

            /* Log the error to the application: */
            UART_drv_log1 ("UART:(%p) Could not write data, UART in use\n", (void *)ptrHwCfg->ptrSCIRegs);

            /* Setup the error code: */
            retVal = UART_EINUSE;
            goto exit;
        }

        /* NO: Save the data to be written */
        ptrUartSciDriver->ptrWriteBuffer = (uint8_t *)buffer;
        ptrUartSciDriver->writeSize      = (int32_t)size;
        ptrUartSciDriver->writeCount     = 0;

        /* Restore the interrupts: */
        UART_osalHardwareIntRestore(key);

#ifdef UART_DMA_ENABLE
        /* Determine the DMA Mode for the Driver: */
        if (BTRUE == ptrUartSciDriver->isDMAEnabled)
        {
            /****************************************************************
            * DMA Mode: Initiate the transmit DMA
            ****************************************************************/
            retVal = ptrHwCfg->initiateTxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, (uint32_t)size, false);
            if (retVal < 0)
            {
                /* Error: Unable to initiate the transmit DMA */
                goto exit;
            }
        }
        else
#endif
        {
            /****************************************************************
            * Normal Mode: We always need to send out the first character
            * because the Transmit interrupt is only generated after the
            * first transfer from the TD to the TXSHF
            ****************************************************************/
	    /* Wait for Tx Ready */
            while (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 0U) {};
            UartSci_writeChar(ptrUartSciDriver, ptrHwCfg);

            /* Do we have more data to send? */
            if (ptrUartSciDriver->writeSize == 0U)
            {
                /* NO: This is the case where there is only 1 byte of data to be
                * sent out. Setup the return value and we are done. */
                retVal = ptrUartSciDriver->writeCount;
                goto exit;
            }

            /* Enable the Transmit Interrupt: */
            UartSci_enableTxInterrupt (ptrHwCfg->ptrSCIRegs);
        }

        if (ptrUartSciDriver->params.writeMode == UART_MODE_BLOCKING)
        {
            /* Block the callee; till the UART writes are complete */
            status = UART_osalPendLock (ptrUartSciDriver->writeSem, ptrUartSciDriver->params.writeTimeout);
            if (status == SemaphoreP_TIMEOUT)
            {
                /* Time out: Write has not been completed in the specified duration
                * Disable the transmit interrupt */
                UartSci_disableTxInterrupt (ptrHwCfg->ptrSCIRegs);

                /* Reset the write size */
                ptrUartSciDriver->writeSize = 0;

                /* Report the error condition: */
                UART_drv_log2 ("UART:(%p) Write timed out %d bytes written\n",
                               (void *)ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->writeCount);
            }

            /* Return the number of bytes which have been sent out. */
            retVal = ptrUartSciDriver->writeCount;
        }
        else
        {
            /*
             * for callback mode, immediately return SUCCESS,
             * once the transaction is done, callback function
             * will return the transaction status and actual
             * write count
             */
            retVal = UART_SUCCESS;
        }
    }
    else
    {
        retVal = UartSci_writePolling(handle, buffer, size);
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be written to the UART Driver but this will poll the UART
 *      driver in a while loop and will only return once all the data has been
 *      sent out.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data to be written
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been transferred
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_writePolling(UART_Handle handle, const void* buffer, size_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;
    int32_t             retVal = 0;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = UART_osalHardwareIntDisable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->writeSize != 0)
    {
        /* YES: Restore the interrupts and report the error to the application. */
        UART_osalHardwareIntRestore(key);
        UART_drv_log1 ("UART:(%p) Could not write data, UART in use\n", (void *)ptrHwCfg->ptrSCIRegs);

        /* Setup the error code: */
        retVal = UART_EINUSE;
        goto exit;
    }

    /* NO: Save the data to be written */
    ptrUartSciDriver->ptrWriteBuffer = (uint8_t*)buffer;
    ptrUartSciDriver->writeSize      = (int32_t)size;
    ptrUartSciDriver->writeCount     = 0U;

    /* Restore the interrupts: */
    UART_osalHardwareIntRestore(key);

#ifdef UART_DMA_ENABLE
    /* Determine the DMA Mode for the Driver: */
    if (BTRUE == ptrUartSciDriver->isDMAEnabled)
    {
        /****************************************************************
         * DMA Mode: Initiate the transmit DMA
         ****************************************************************/
        retVal = ptrHwCfg->initiateTxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, (uint32_t)size, true);
        if (retVal < 0)
        {
            /* Error: Unable to initiate the transmit DMA. The return value
             * is already setup with the error code. */
            goto exit;
        }
    }
    else
#endif
    {
        /****************************************************************
         * Normal Mode: Write the data out one by one
         ****************************************************************/
        bool isWaitForever = (ptrUartSciDriver->params.writeTimeout == UART_WAIT_FOREVER);
        uint64_t timeout = ptrUartSciDriver->params.writeTimeout * 1000;

        /* Send out all the data: */
        while (ptrUartSciDriver->writeSize > 0)
        {
            /* Is the transmitter free? */
            if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
            {
                /* YES: Write out a character from the buffer. */
                UartSci_writeChar (ptrUartSciDriver, ptrHwCfg);
            }
            else if (timeout < 10)
            {
                /* Debug Message: */
                UART_drv_log2 ("Debug: UART(%p) writePolling timeout %d bytes have been read\n",
                                ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->writeCount);
                break;
            }
            else if (!isWaitForever)
            {
                Uartsci_usDelay(10);
                timeout -= 10;
            }
        }
    }

    /* Setup the number of bytes which have been written */
    retVal = ptrUartSciDriver->writeCount;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked
 *      when the UART Driver write operations are ben canceled.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_writeCancel(UART_Handle handle)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = UART_osalHardwareIntDisable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->writeSize == 0)
    {
        /* NO: Restore the interrupts and report the error to the application. */
        UART_osalHardwareIntRestore(key);
    }
    else
    {
        /* Set the write size to 0; this will prevent any more characters from being
         * transmitted. */
        ptrUartSciDriver->writeSize = 0;

        /* Determine the DMA Mode for the Driver: */
        if (BTRUE == ptrUartSciDriver->isDMAEnabled)
        {
            /* DMA Mode: Disable the Tx DMA */
            UartSci_disableTxDMA (ptrHwCfg->ptrSCIRegs);
        }
        else
        {
            /* Normal Mode: Disable the Tx interrupt: */
            UartSci_disableTxInterrupt (ptrHwCfg->ptrSCIRegs);
        }

        /* Restore the interrupts: */
        UART_osalHardwareIntRestore(key);

        /* We have cancelled the write operation. Invoke post-write operation */
        UartSci_callback(handle, BFALSE);

        /* Debug Message: */
        UART_drv_log2 ("Debug: UART(%p) write canceled %d bytes have been written\n",
                        (void *)ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->writeCount);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART Driver instance is being passed a control command.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  cmd
 *      UART command
 *  @param[in]  arg
 *      Opaque handle to the argument
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   UART Error code
 */
static int32_t UartSci_control(UART_Handle handle, uint32_t cmd, void *arg)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    int32_t             errCode;

    /* Initialize and setup the error code: */
    errCode = 0;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Processing is done on the basis of the command: */
    switch (cmd)
    {
        case UART_CMD_LOOPBACK:
        {
            uint32_t*   loopback;

            /* Sanity Check: We need to ensure that a valid argument was passed */
            if (arg == NULL)
            {
                /* Error: No valid argument was passed. */
                errCode = UART_EINVAL;
            }
            else
            {
                /* Get the loopback status from the argument */
                loopback = (uint32_t*)arg;
                UartSci_loopbackControl(ptrHwCfg->ptrSCIRegs, *loopback);

                /*
                 * Silicon workaround: It has been identified that it will take a while
                 *                     for the loopback mode to take effect, therefore,
                 *                     introduce some delay here to avoid tx operation
                 *                     to be triggered prior to that.
                 */
                if (*loopback)
                {
                    UART_osalDelay(2);
                }
            }
            break;
        }
        case UART_CMD_GET_STATS:
        {
            UART_Stats* ptrUARTStats;

            /* Sanity Check: We need to ensure that a valid argument was passed */
            if (arg == NULL)
            {
                /* Error: No valid argument was passed. */
                errCode = UART_EINVAL;
            }
            else
            {
                /* Get the pointer to the UART statistics */
                ptrUARTStats = (UART_Stats*)arg;

                /* Copy over the UART statistics */
                memcpy ((void*)ptrUARTStats, (void*)&ptrUartSciDriver->stats, sizeof(UART_Stats));
            }
            break;
        }
        default:
        {
            /* Error: Unsuported/Invalid command specified */
            errCode = UART_EINVAL;
            break;
        }
    }
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the arguments
 *
 *  @param[in]  params
 *      UART Parameters to be validated
 *  @param[in]  ptrHwCfg
 *      Pointer to the hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t UartSci_validateParams (const UART_Params* params, UartSci_HwCfg* ptrHwCfg)
{
    int32_t retVal = 0;

    /* Sanity Check: The current implementation of the UART Driver currently only supports
     * NONE, ODD and EVEN Parity.  */
    if (params->parityType >= UART_PAR_ZERO)
    {
        retVal = -1;
    }

    /* Sanity Check: The current implementation of the UART Driver currently only supports
     * 5/6/7/8-bit data length.  */
    if (params->dataLength > UART_LEN_8)
    {
        retVal = -1;
    }

    /* Sanity Check: Ensure that the clock frequency is NON-Zero. */
    if ((params->baudRate == 0) || (ptrHwCfg->swCfg.clockFrequency == 0))
    {
        retVal = -1;
    }

    /* Sanity Check: Ensure the callback function is provided at callback mode. */
    if (((params->readMode == UART_MODE_CALLBACK) && (params->readCallback == NULL)) ||
        ((params->writeMode == UART_MODE_CALLBACK) && (params->writeCallback == NULL)))
    {
        retVal = -1;
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  params
 *      UART Parameters with which the driver is being opened
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
static UART_Handle UartSci_open(UART_Handle handle, const UART_Params* params)
{
    UART_Config*             ptrUARTConfig;
    UartSci_Driver*          ptrUartSciDriver;
    SemaphoreP_Params        semParams;
    OsalRegisterIntrParams_t interruptRegParams;
    UartSci_HwCfg*           ptrHwCfg;
    UART_Handle              retHandle = NULL;
    CSL_sciRegs*             ptrSCIRegs;
    UART_SCI_CONFIG          sciConfig;
    uint32_t                 val32;
    int32_t                  ret;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Sanity Check: Validate the arguments */
    if (UartSci_validateParams(params, ptrHwCfg) < 0)
    {
        /* Error: Invalid arguments have been passed to the driver */
        UART_drv_log1 ("Debug: UART Driver (%p) Invalid arguments\n", (void *)ptrHwCfg->ptrSCIRegs);
        goto exit;
    }

    /* Initialize the memory: */
    memset ((void *)ptrUartSciDriver, 0, sizeof(UartSci_Driver));

    /* Copy over the UART Parameters */
    memcpy ((void*)&ptrUartSciDriver->params, (void *)params, sizeof(UART_Params));
    ptrUartSciDriver->handle = handle;

#ifdef UART_DMA_ENABLE
    /* Open the DMA Block associated with the UART Instance */
    if (ptrHwCfg->openDMAFxn (ptrUartSciDriver, ptrHwCfg) < 0)
    {
        /* Error: Unable to open the DMA Block. */
        UART_drv_log1 ("Error: UART Driver (%p) Unable to open the DMA Block\n", (void *)ptrHwCfg->ptrSCIRegs);
        goto exit;
    }
#endif

    /* Create a binary semaphore which is used to handle the Blocking operation. */
    UART_osalSemParamsInit(&semParams);
    semParams.mode             = SemaphoreP_Mode_BINARY;
    ptrUartSciDriver->writeSem = UART_osalCreateBlockingLock(0, &semParams);

    /* Create a binary semaphore which is used to handle the Blocking operation. */
    UART_osalSemParamsInit(&semParams);
    semParams.mode            = SemaphoreP_Mode_BINARY;
    ptrUartSciDriver->readSem = UART_osalCreateBlockingLock(0, &semParams);

    ptrSCIRegs = ptrHwCfg->ptrSCIRegs;
    /* Enable the SCI Module: */
    UartSci_enable(ptrSCIRegs);

    /* Put the SCI Module in Software Reset State: */
    UartSci_enableSwReset(ptrSCIRegs);

    /* Disable the interrupts: */
    UartSci_disableAllInterrupts (ptrSCIRegs);
    UartSci_clearAllInterruptLevels (ptrSCIRegs);

    /* Setup the SCI Global Control Register:
     *  - Receiver Enabled
     *  - Transmit Enabled
     *  - Internal Clock
     *  - Asynchronous Timing Mode
     *  - Idle-line Communication mode
     */
    sciConfig.rxEn = 1U;
    sciConfig.txEn = 1U;
    sciConfig.clkMode = UARTSCI_CLOCK_MODE_INTERNAL;
    sciConfig.timingMode = UARTSCI_TIMING_MODE_ASYNC;
    sciConfig.commMode = UARTSCI_COMM_MODE_IDLE;

    /* Setup the Stop Bits: */
    if (params->stopBits == UART_STOP_ONE)
    {
        sciConfig.stopBits = UARTSCI_FORMAT_STOPBIT_ONE;
    }
    else
    {
        sciConfig.stopBits = UARTSCI_FORMAT_STOPBIT_ONE;
    }

    /* Setup the Parity: */
    switch (params->parityType)
    {
        case UART_PAR_NONE:
        {
            sciConfig.parityType = UARTSCI_FORMAT_PARITY_NONE;
            break;
        }
        case UART_PAR_EVEN:
        {
            sciConfig.parityType = UARTSCI_FORMAT_PARITY_EVEN;
            break;
        }
        case UART_PAR_ODD:
        {
            sciConfig.parityType = UARTSCI_FORMAT_PARITY_ODD;
            break;
        }
        default:
        {
            /* Error: We have already verified the parameters. Control should never come here. */
            UART_osalAssert (0);
            break;
        }
    }

    /* Setup the Baud Rate: */
    sciConfig.clockFrequency = ptrHwCfg->swCfg.clockFrequency;
    sciConfig.baudRate = params->baudRate;

    /* Setup the data length control: */
    switch (params->dataLength)
    {
        case UART_LEN_5:
        {
            /* Data Length is 5 */
            sciConfig.charLen = UARTSCI_FORMAT_CHAR_LEN_5;
            ptrUartSciDriver->shiftJustification = 3U;
            break;
        }
        case UART_LEN_6:
        {
            /* Data Length is 6 */
            sciConfig.charLen = UARTSCI_FORMAT_CHAR_LEN_6;
            ptrUartSciDriver->shiftJustification = 2U;
            break;
        }
        case UART_LEN_7:
        {
            /* Data Length is 7 */
            sciConfig.charLen = UARTSCI_FORMAT_CHAR_LEN_7; 
            ptrUartSciDriver->shiftJustification = 1U;
            break;
        }
        case UART_LEN_8:
        {
            /* Data Length is 8 */
            sciConfig.charLen = UARTSCI_FORMAT_CHAR_LEN_8;
            ptrUartSciDriver->shiftJustification = 0U;
            break;
        }
        default:
        {
            /* Error: We have already verified the parameters. Control should never come here. */
            UART_osalAssert (0);
            break;
        }
    }

    if (ptrHwCfg->swCfg.loopback)
    {
        /* Enable Loopback: */
        sciConfig.loopback = 1U;
    }
    else
    {
        /* Disable Loopback: */
        sciConfig.loopback = 0U;
    }

    ret = UartSci_config (ptrSCIRegs, &sciConfig);
    if (ret < 0)
    {
        UART_drv_log1 ("Error: UART Driver configure error with ret = %d\n", ret);
        goto exit;
    }

    /* Enable the Interrupts:
     * - Framing Error
     * - Overrun Error
     * - Parity Error */
    val32 = UARTSCI_INT_BITMAP_FE |
            UARTSCI_INT_BITMAP_OE |
            UARTSCI_INT_BITMAP_PE;
    UartSci_enableInterrupts(ptrSCIRegs, val32);

    /* Register the Interrupt Handler: */
    if(ptrHwCfg->swCfg.enableInterrupt)
    {
        /* Construct Hwi object for this UART peripheral. */
        /* Initialize with defaults */
        Osal_RegisterInterrupt_initParams(&interruptRegParams);

         /* Populate the interrupt parameters */
        interruptRegParams.corepacConfig.arg=(uintptr_t)handle;
        interruptRegParams.corepacConfig.name=(char *)"UART";
        interruptRegParams.corepacConfig.isrRoutine=UartSci_ISR;
#ifdef __TI_ARM_V7R4__
        interruptRegParams.corepacConfig.priority=0x8U;
        interruptRegParams.corepacConfig.intVecNum = (int32_t)(ptrHwCfg->interruptNum); /* Host Interrupt vector */
#elif  defined(_TMS320C6X)
        interruptRegParams.corepacConfig.priority=0x20U;
        interruptRegParams.corepacConfig.corepacEventNum = (int32_t)(ptrHwCfg->interruptNum); /* Event going to INTC */
        interruptRegParams.corepacConfig.intVecNum = (int32_t)(OSAL_REGINT_INTVEC_EVENT_COMBINER); /* Host Interrupt vector */
#endif

        /* Register interrupts */
        (void)UART_osalRegisterInterrupt(&interruptRegParams,&(ptrUartSciDriver->hwiHandle));

        if(ptrUartSciDriver->hwiHandle)
        {
            UART_drv_log2 ("Debug: UART Driver Registering HWI ISR [%p] for Interrupt %d\n",
                           ptrUartSciDriver->hwiHandle, ptrHwCfg->interruptNum);
        }
        else
        {
            goto exit;
        }
    }

    /* Start the SCI: */
    UartSci_disableSwReset (ptrSCIRegs);

    /*
     * Silicon workaround: It has been identified that it will take a while for the loopback mode
     *                     to take effect, therefore, introduce some delay here to avoid tx operation
     *                     to be triggered prior to that.
     */
    if (ptrHwCfg->swCfg.loopback)
    {
        UART_osalDelay(2);
    }

    /* Mark the driver to be operational */
    ptrUartSciDriver->status = UartSci_DriverStatus_OPERATIONAL;

    /* Debug Message: */
    UART_drv_log1 ("Debug: UART Driver %p opened\n", (void *)ptrHwCfg->ptrSCIRegs);

    /* Setup the return handle */
    retHandle = (UART_Handle)handle;

exit:
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART Driver instance is being closed
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_close(UART_Handle handle)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Is the driver in use? */
    if (ptrUartSciDriver->status == UartSci_DriverStatus_UNINITIALIZED)
    {
        /* Error: UART has alreay been closed */
        UART_drv_log1 ("Error: UART Driver has already been closed\n", (void *)ptrHwCfg->ptrSCIRegs);
        goto exit;
    }

    /* Disable the interrupts: */
    UartSci_disableAllInterrupts(ptrHwCfg->ptrSCIRegs);

    /* Was the HWI registered?  */
    if (ptrUartSciDriver->hwiHandle)
    {
        /* YES: Delete and unregister the interrupt handler. */
        (void)UART_osalHardwareIntDestruct(ptrUartSciDriver->hwiHandle, (int32_t)(ptrHwCfg->interruptNum));
    }

    /*
     * Wait for the ongoing transmission to be completed.
     */
    while(!UartSci_isTxEmpty(ptrHwCfg->ptrSCIRegs)){};

    /* Was the UART Driver operating in Write Blocking mode? */
    if (ptrUartSciDriver->writeSem)
    {
        /* YES: Delete the write semaphore */
        UART_osalDeleteBlockingLock (ptrUartSciDriver->writeSem);
    }

    /* Was the UART Driver operating in Read Blocking mode? */
    if (ptrUartSciDriver->readSem)
    {
        /* YES: Delete the read semaphore */
        UART_osalDeleteBlockingLock (ptrUartSciDriver->readSem);
    }

#ifdef UART_DMA_ENABLE
    /* Close the DMA block associated with the UART Instance */
    ptrHwCfg->closeDMAFxn (ptrUartSciDriver);
#endif
    /* The driver is not in use: */
    ptrUartSciDriver->status = UartSci_DriverStatus_UNINITIALIZED;

exit:
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART drivers are initialized.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver which is to be initialized.
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void
UartSci_init(UART_Handle handle)
{
    UART_socInitHwCfg(handle);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART driver invokes UART_read2() to receive data .
 *
 *  @param[in]  handle
 *      Handle to the UART Driver which is to be initialized.
 *
 *  @param[in/out] uartTrans
 *      A pointer to a UART_Transaction
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @return Returns UART_SUCCESS or UART_ERROR on an error.
 */
static int32_t UartSci_read2(UART_Handle handle, UART_Transaction *transaction)
{
    return (UART_ENOTIMPL);
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART driver invokes UART_write2() to transmit data .
 *
 *  @param[in]  handle
 *      Handle to the UART Driver which is to be initialized.
 *
 *  @param[in/out] uartTrans
 *      A pointer to a UART_Transaction
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @return Returns UART_SUCCESS or UART_ERROR on an error.
 */
static int32_t UartSci_write2(UART_Handle handle, UART_Transaction *transaction)
{
    return (UART_ENOTIMPL);
}
