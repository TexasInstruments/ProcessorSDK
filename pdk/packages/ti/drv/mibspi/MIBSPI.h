/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

/**
 *  \defgroup DRV_MIBSPI_MODULE MIBSPI Driver
 *
 *  The MIBSPI peripheral is a feature rich serial peripheral interface  
 *  in the ARxxxx/TPR12/AWR294X SoCs which provides a 16-bit configurable synchronous
 *  multi-buffered multi-pin serial peripheral interface (MibSPI). The MibSPI is 
 *  a programmable-length shift register used for high-speed communication between 
 *  external peripherals or other microcontrollers. The MibSPI peripheral can be 
 *  programmed at a high-level using the MIBSPI driver software that allows 
 *  hook-up with an operating system.
 *  The MIBSPI driver exposes programming of most features provided by the IP.
 *  The MIBSPI peripheral specification is available in TRM of ARxx devices:
 *  https://www.ti.com/lit/ug/swru520d/swru520d.pdf Chapter 23:MibSPI
 *
 *  Following is a high level description of the APIs:
 *
 *  - MIBSPI_init(): Initialize the MIBSPI module. MIBSPI_init() plugs in application specific 
 *    callbacks for   utilities such as printFxn/traceFxn/addressTranslation etc. 
 *    This is module level  software intiialization and does not program in MIBSPI registers
 *  - MIBSPI_open():Open a MIBSPI peripheral instance in master/slave mode with associated configuration
 *  - MIBSPI_transfer():Transfer data on the MIBSPI instance.The transaction structure contains info in the tx and rx data
 *  - MIBSPI_getStats():Statistics query from the driver
 *  - MIBSPI_control(): Runtime control
 *  - MIBSPI_close():Close an instance of MIBSPI
 *
 *
 *  \section mibspi_limit Limitations
 *     \subsection mibspi_hwlimit Unsupported Hardware features.
 *       Following features of the MIBSPI hardware are presently NOT supported (on TPR12, AWR294X):
 *       - Paralllel mode operation
 *       - Modulo mode operation
 *       - SPIENA pin configuration
 *       - Extended RAM mode support
 *
 *     \subsection mibspi_swlimit Unsupported Software features.
 *     - Compatibility mode operation
 *     - Support for all character length from 2 - 16. 
 *       -- Only 8bit and 16bit data length are supported 
 *     - Support for multiple simultaneous transactions
 *     - Support for multiple slaves in slave mode
 *     - Support for cancelling transaction
 *     - CSNR encoded mode support
 *     - Support for external trigger events for transfer group activation
 *     - Configuration of delay values like Chip select to transmit start delay etc in master mode
 *     - Support for power down mode
 *     - Support for MSS SPI instances on DSS c66x
 *     - Master Mode with external clock
 *     - Abort DMA transaction and cleanup on error
 *     - Abort and reset SPI peripheral state on  transaction error and cleanup
 *     - Categorization of interrupts in two levels
 *     - Support for mapping CS to transfer group priority
 *     - Support for interleaved transfer (priority based) with support for  disabling preemption
 *     - Configuration of slew rate for SPI pins
 *     - Configuration of SPI Pins in GPIO mode
 *     - Support for upto 4 different data formats for a single SPI instance
 *     - Support for dynamically changing format per Tx Data
 *     - Support for handling cache coherency inside driver
 *     - EDMA error handling
 *     - Multiple Slave support in slave mode
 *
 *  \section mibspi_api API
 *
 *
 *  The MIBSPI header file should be included in an application as follows:
 *  @code
 *  #include <ti/drv/mibspi/MIBSPI.h>
 *  @endcode
 *  The documentation for all APIs can be referred by clicking at above file or
 *  by going to @ref MIBSPI_DRIVER_EXTERNAL_FUNCTION. Example usage of the MIBSPI
 *  driver can be seen by referring to the MIBSPI unit test code located at
 *  @verbatim ti/drv/mibspi/test/src/main_mibspi_test.c @endverbatim

 *
 *  \section mibspi_operation Operation
 *  The MIBSPI driver in TI-RTOS/Baremetal is designed to serve a means to move data
 *  between SPI peripherals. This driver does not interpret any of the data sent
 *  to or received from this peripheral.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS/Baremetal
 *  application. The specific peripheral implementations are responsible to
 *  create all the OS specific primitives to allow for thread-safe
 *  operation.
 *
 *  The MIBSPI driver operates on some key definitions and assumptions:
 *  - The driver operates transparent from the chip select. Some SPI controllers
 *    feature a hardware chip select to assert SPI slave peripherals. See the
 *    specific peripheral implementations on chip select requirements.
 *
 *  - The SPI protocol does not account for a built-in handshaking mechanism and
 *    neither does this MIBSPI driver. Therefore, when operating in ::MIBSPI_SLAVE
 *    mode, the application must provide such a mechanism to ensure that the
 *    SPI slave is ready for the SPI master. The SPI slave must call
 *    MIBSPI_transfer() *before* the SPI master starts transmitting. Some example
 *    application mechanisms could include:
 *    - Timed delays on the SPI master to guarantee the SPI slave is be ready
 *      for a SPI transaction.
 *    - A form of GPIO flow control from the slave to the SPI master to notify
 *      the master when ready.
 *  - When SPI operates in ::MIBSPI_MASTER, the partition of TX/RX RAM should be 
 *    provided by application by providing proper values in ::MIBSPI_Params, such as 
 *    number of slaves and slave profiles. 
 *    The maximum number of supported slaves is platform specfic, please refer to 
 *    TRM for the information. The maximum number of slaves supported in the 
 *    driver is ::MIBSPI_SLAVE_MAX.
 *
 *  The platform specific MIBSPI file present in the ti/drivers/mibspi/soc
 *  directory. This file is built as a part of the SPI Library for the specific
 *  platform.
 *
 *    \subsection  mibspi_open Opening the driver
 *  
 *  Code examples for opening SPI driver in master and slave mode
 *
 *  Open MIBSPI driver as ::MIBSPI_MASTER
 *
 *  @code
 *  MIBSPI_Handle      handle;
 *  MIBSPI_Params      params;
 *  MIBSPI_Transaction spiTransaction;
 *
 *  MIBSPI_Params_init(&params);
 *  params.mode  = MIBSPI_MASTER;
 *  params.dmaEnable = 1;
 *  params.dmaHandle = edmaHandle;
 *  params.u.masterParams.bitRate = bitRate;
 *  params.u.masterParams.numSlaves = 1;
 *  params.u.masterParams.slaveProf[0].chipSelect = 0;
 *  params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
 *  params.u.masterParams.slaveProf[1].chipSelect = 1;
 *  params.u.masterParams.slaveProf[1].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
 *
 *  handle = MIBSPI_open(mibspiInstanceId, &params);
 *  if (!handle) {
 *      System_printf("SPI did not open");
 *  }
 *  @endcode
 *
 *
 *  Open MIBSPI driver as ::MIBSPI_SLAVE
 *
 *  @code
 *  MIBSPI_Handle      handle;
 *  MIBSPI_Params      params;
 *  MIBSPI_Transaction spiTransaction;
 *
 *  MIBSPI_Params_init(&params);
 *  params.mode = MIBSPI_SLAVE;
 *  params.frameFormat = MIBSPI_POL0_PHA0;
 *  params.pinMode = MIBSPI_PINMODE_4PIN_CS;
 *  params.shiftFormat = MIBSPI_MSB_FIRST;
 *  params.dmaEnable = 1;
 *  params.dmaHandle = edmaHandle;
 *  handle = MIBSPI_open(mibspiInstanceId, &params);
 *  if (!handle) {
 *      System_printf("SPI did not open");
 *  }
 *  @endcode
 *
 *    \subsection  mibspi_transfer Transferring data
 *
 *  Data transmitted and received by the MIBSPI peripheral is performed using
 *  MIBSPI_transfer(). MIBSPI_transfer() accepts a pointer to a MIBSPI_Transaction
 *  structure that dictates what quantity of data is sent and received.
 *
 *  @code
 *  MIBSPI_Transaction spiTransaction;
 *
 *  spiTransaction.count = someIntegerValue;
 *  spiTransaction.txBuf = transmitBufferPointer;
 *  spiTransaction.rxBuf = receiveBufferPointer;
 *  spiTransaction.slaveIndex = index1;  // For master mode only
 *
 *  ret = MIBSPI_transfer(handle, &spiTransaction);
 *  if (!ret) {
 *      System_printf("Unsuccessful SPI transfer");
 *  }
 *  @endcode
 *
 *  When MIBSPI driver is configured in ::MIBSPI_MASTER model, slave index need to be
 *  provided to indicate which slave the transfer is for.
 *
 *  ## Canceling a transaction #
 *  MIBSPI_transferCancel() is used to cancel a SPI transaction - not supported.
 *
 *  @code
 *  MIBSPI_transferCancel(handle);
 *  @endcode
 *
 *  \section mibspi_implementation Implementation
 *
 *  \subsection mibspi_multiicount MIBSPI transfer with multiple RAM buffer (icount)
 *  MIBSPI hardware has an internal RAM buffer that stores transmit/receive data 
 *  element in 8bits or 16bits. 
 *  The MIBSPI driver has a compile time option to transfer data bigger than RAM 
 *  buffer size. This is intended for high throughput transfers. But it has some 
 *  limitations and not supported in all SPI modes.
 *  
 *  SPI mode |  Pin Mode  | Supported?  | Limitations
 *  -------- | -------- |----------- |----------
 *  Master   |  4-pin  | Yes       |  Note1
 *  Master   |  3-pin  | Yes       |  Note1
 *  Slave    |  4-pin  | Yes       |  Note2
 *  Slave    |  3-pin  | No        |  None
 * 
 * 
 *  Note1: There will be gaps between every transfer of the RAM buffer size for 
 *        internal DMA copy of the data. During this time, Clock and CS will be 
 *        inactive.
 *  
 *  Note2: Slave needs time to do DMA copy of received data for every RAM buffer.
 *        Hence require the SPI master to deactivate  CS/ClK signal during this time. 
 *        When using another XWR1xxx device as master, it can be achieved by
 *        setting C2Tdelay/T2Cdelay/wdelay. 
 *  
 *  The maximum elements is defined as MIBSPI_RAM_MAX_ELEM. 
 *  For multiple slaves scenarios, this number is divided 
 *  among all slaves. This information is provided from application in 
 *  MIBSPI_MasterModeParams. The maximum icount value is 32. Please make sure 
 *  the transfer length is not exceed 32 * ramLen. For High clock rate, it is recommended
 *  to use small icount values.
 *
 *  \section mibspi_instrumentation Instrumentation
 *
 *  The MIBSPI driver interface supports configuration of trace level at build time
 *  Log statements will be passed to the application provided traceFxn 
 *
 *  Trace Level      | Log details |
 *  ---------------- | ----------- |
 *  ERROR            | Error conditions cause trace logging |
 *  WARNING          | Warning conditions cause trace logging |
 *  INFO             | Info trace logging enabled |
 *  ============================================================================
 *
 *  \section mibspi_caution Caution
 *
 *  TODO
 *
 *  \section mibspi_mem_footprint Memory Footprint
 *
 *  The memory footprint of the driver depends on how many APIs are referred to
 *  by an application (all are not necessary for basic operation). The unit test
 *  code's generated memory map file may be used to get an estimate of
 *  the memory foot-print when most APIs are used.
 *
 */

/** ============================================================================
 *  @file       MIBSPI.h
 *
 *  @brief      Multi-Buffer RAM SPI driver interface
 */



#ifndef MIBSPI_H
#define MIBSPI_H

#include <ti/drv/mibspi/soc/MIBSPI_soc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
@defgroup MIBSPI_DRIVER_EXTERNAL_FUNCTION            MIBSPI Driver External Functions
@ingroup DRV_MIBSPI_MODULE
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE     MIBSPI Driver External Data Structures
@ingroup DRV_MIBSPI_MODULE
@brief
*   The section has a list of all the data structures which are exposed to the application
*/

/** @defgroup MIBSPI_STATUS_DEFINES MIBSPI Status Definitions
 *  @ingroup DRV_MIBSPI_MODULE
 *
 *  @brief
 *  The section has a list of all the error codes returned by MIBSPI APIs like MIBSPI_control()
 *  module

 @{ */

/*!
 * \brief   Successful status code returned by MIBSPI_control().
 *
 * MIBSPI_control() returns MIBSPI_STATUS_SUCCESS if the control code was executed
 * successfully.
 */
#define MIBSPI_STATUS_SUCCESS         (int32_t)(0)

/*!
 * \brief   Generic error status code returned by MIBSPI_control().
 *
 * MIBSPI_control() returns MIBSPI_STATUS_ERROR if the control code was not executed
 * successfully.
 */
#define MIBSPI_STATUS_ERROR          (-((int32_t)1))

/*!
 * \brief   An error status code returned by MIBSPI_control() for undefined
 * command codes.
 *
 * MIBSPI_control() returns MIBSPI_STATUS_UNDEFINEDCMD if the control code is not
 * recognized by the driver implementation.
 */
#define MIBSPI_STATUS_UNDEFINEDCMD   (-((int32_t)2))
/** @}*/


/**
 *  @defgroup MIBSPI_CMD Command Codes
 *  MIBSPI_CMD_* macros are general command codes for MIBSPI_control().
 *  @{
 *  @ingroup MIBSPI_CONTROL
 */

/*!
 * Common MIBSPI_control command code reservation offset.
 * MIBSPI driver implementations should offset command codes with MIBSPI_CMD_RESERVED
 * growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define MIBSPIXYZ_COMMAND0         SPI_CMD_RESERVED + 0
 * #define MIBSPIXYZ_COMMAND1         SPI_CMD_RESERVED + 1
 * @endcode
 */
#define MIBSPI_CMD_RESERVED            (32U)

/*!
 * @brief Command used by SPI_control to enable loopback
 *
 */
#define MIBSPI_CMD_LOOPBACK_ENABLE                         (uint32_t)(MIBSPI_CMD_RESERVED + 0)

/*!
 * @brief Command used by SPI_control to set Clock phase and polarity
 *
 */
#define MIBSPI_CMD_SET_CLOCK_PHASEPOLARITY                 (uint32_t)(MIBSPI_CMD_RESERVED + 1)

/*!
 * @brief Command used by SPI_control to get SPI driver stats
 *
 */
#define MIBSPI_CMD_GET_STATS                               (uint32_t)(MIBSPI_CMD_RESERVED + 2)

/** @}*/

/*!
 *  @brief    Wait forever define
 */
#define MIBSPI_WAIT_FOREVER                                             (~(0U))

/*!
 *  @brief
 *  Definitions for MIBSPI Pins Operation Mode.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MIBSPI_PinMode_e
{
    MIBSPI_PINMODE_3PIN   = 0,        /*!< 3 Pin operation Mode */
    MIBSPI_PINMODE_4PIN_CS  = 1       /*!< 4 Pin operation Mode with CS signal */
} MIBSPI_PinMode;

/*!
 *  @brief
 *  Definitions for MIBSPI Data shift format.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MIBSPI_DataShiftFmt_e
{
    MIBSPI_MSB_FIRST = 0,    /*!< MSB shift out first */
    MIBSPI_LSB_FIRST  = 1    /*!< LSB shift out first */
} MIBSPI_DataShiftFmt;

/*!
 *  @brief
 *  Definitions for  MIBSPI loopback modes.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MibSpi_LoopBackType_e
{
    MIBSPI_LOOPBK_DIGITAL= 0,    /*!< MIBSPI digital loopback  mode */
    MIBSPI_LOOPBK_ANALOG = 1,    /*!< MIBSPI analog loopback  mode */
    MIBSPI_LOOPBK_NONE = 2,    /*!< MIBSPI loopback  disable */
} MibSpi_LoopBackType;

/*!
 *  @brief      Status codes that are set by the SPI driver.
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 */
typedef enum MIBSPI_Status_s {
    MIBSPI_TRANSFER_COMPLETED = 0,
    MIBSPI_TRANSFER_STARTED,
    MIBSPI_TRANSFER_CANCELED,
    MIBSPI_TRANSFER_FAILED,
    MIBSPI_TRANSFER_CSN_DEASSERT,
    MIBSPI_TRANSFER_TIMEOUT
} MIBSPI_Status;

/*!
 *  @brief
 *  Definitions for various SPI modes of operation.
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 */
typedef enum MIBSPI_Mode_s {
    MIBSPI_MASTER      = 0,    /*!< SPI in master mode */
    MIBSPI_SLAVE       = 1     /*!< SPI in slave mode */
} MIBSPI_Mode;

/*!
 *  @brief
 *  Definitions for various SPI data frame formats.
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 */
typedef enum MIBSPI_FrameFormat_s {
    MIBSPI_POL0_PHA0   = 0,    /*!< SPI mode Polarity 0 Phase 0 */
    MIBSPI_POL0_PHA1   = 1,    /*!< SPI mode Polarity 0 Phase 1 */
    MIBSPI_POL1_PHA0   = 2,    /*!< SPI mode Polarity 1 Phase 0 */
    MIBSPI_POL1_PHA1   = 3,    /*!< SPI mode Polarity 1 Phase 1 */
} MIBSPI_FrameFormat;

/*!
 *  @brief
 *
 *  MIBSPI transfer mode determines the whether the MIBSPI controller operates
 *  synchronously or asynchronously. In ::MIBSPI_MODE_BLOCKING mode MIBSPI_transfer()
 *  blocks code execution until the MIBSPI transaction has completed. In
 *  ::MIBSPI_MODE_CALLBACK MIBSPI_transfer() does not block code execution and instead
 *  calls a ::MIBSPI_CallbackFxn callback function when the transaction has
 *  completed.
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 */
typedef enum MIBSPI_TransferMode_s {
    /*!
     * MIBSPI_transfer() blocks execution. This mode can only be used when called
     * within a Task context
     */
    MIBSPI_MODE_BLOCKING,
    /*!
     * MIBSPI_transfer() does not block code execution and will call a
     * ::MIBSPI_CallbackFxn. This mode can be used in a Task, Swi, or Hwi context.
     */
    MIBSPI_MODE_CALLBACK
} MIBSPI_TransferMode;

/*!
 * @brief MIBSPI info/debug print function prototype
 * @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 * This function is used by the driver to print info/debug messages.
 *
 * @param str      [OUT] Info string to print
 */
typedef int (*Mibspi_PrintFxnCb)(const char *str, ...);

/*!
 * @brief MIBSPI log trace function prototype
 *
 * This function is used by the driver to trace log messages.
 *
 * @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 * @param str      [OUT] Info string to print
 */
typedef int (*Mibspi_TraceFxnCb)(const char *str, ...);

/*!
 * @brief MIBSPI virtual-to-physical address translation callback function.
 *
 * This function is used by the driver to convert virtual address to physical
 * address.
 *
 * @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 * @param virtAddr [IN] Virtual address
 * @param chNum    [IN] Channel number passed during channel open
 * @param appData  [IN] Callback pointer passed during channel open
 *
 * @return Corresponding physical address
 */
typedef uint32_t (*Mibspi_VirtToPhyFxn)(const void *virtAddr);




/*!
 *  @brief
 *  A ::MIBSPI_Transaction data structure is used with MIBSPI_transfer(). It indicates
 *  how many ::MIBSPI_FrameFormat frames are sent and received from the buffers
 *  pointed to txBuf and rxBuf.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MIBSPI_Transaction_t
{
    /* User input (write-only) fields */
    size_t     count;      /*!< Number of frames for this transaction */
    void      *txBuf;      /*!< void * to a buffer with data to be transmitted */
    void      *rxBuf;      /*!< void * to a buffer to receive data */
    void      *arg;        /*!< Argument to be passed to the callback function */

    /* User output (read-only) fields */
    MIBSPI_Status status;     /*!< Status code set by SPI_transfer */
    uint8_t   slaveIndex; /*!< Index of the slave enabled for this transfer */
} MIBSPI_Transaction;


/*!
 *  @brief
 *  A ::MIBSPI_Stats data structure is used with SPI_getStats() to get driver statistics. 
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MIBSPI_Stats_t
{
    uint32_t   dlenErr;     /*!<Number of data length error */
    uint32_t   timeout;     /*!<Number of timeout */
    uint32_t   parErr;      /*!<Number of Parity error */
    uint32_t   desync;      /*!<Number of De-synchronization of slave device - master only */
    uint32_t   bitErr;      /*!<Number of data mismatch transmit data error - master only */
    uint32_t   rxOvrnErr;   /*!<Number of RX Overrun Error */
} MIBSPI_Stats;


/*!
 *  @brief MIBSPI slave profile Parameters
 *
 *  When MIBSPI is configured as master mode, it may connect to multiple slaves. This data structure captures 
 *  the the configurations for remote slaves.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MIBSPI_SlaveProfile_t
{
    uint8_t             chipSelect;                /*!< CS0-CS7 signal number from 0 -7 */

    uint8_t             ramBufLen;                 /*!< RAM Length in bytes that used by the slave */

    uint32_t            dmaReqLine;                /*!< DMA request line to be used for slave */
} MIBSPI_SlaveProfile;

/*!
 *  @brief MIBSPI master Parameters
 *
 *  MIBSPI master Parameters are used  with the MIBSPI_open() call when mode is set to SPI_MASTER. 
 *  Default values for these parameters are set using MIBSPI_Params_init().
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         MIBSPI_Params_init()
 */
typedef struct MIBSPI_MasterModeParams_t
{
    uint32_t            bitRate;                    /*!< SPI bit rate in Hz */

    uint8_t             numSlaves;                  /*!< Number of slaves connected to SPI master */

    uint8_t             t2cDelay;                   /*!< Transmit end to chip select inactive delay */
    
    uint8_t             c2tDelay;                   /*!< Chip select active to transmit start delay */
    
    uint8_t             wDelay;                     /*!< Delay in between transmissions */
    
    MIBSPI_SlaveProfile    slaveProf[MIBSPI_SLAVE_MAX]; /*!< Slave profile for each slave */
} MIBSPI_MasterModeParams;

/*!
 *  @brief MIBSPI slave mode Parameters
 *
 *  MIBSPI slave Parameters are used with the MIBSPI_open() call when mode is set to MIBSPI_SLAVE. 
 *  Default values for these parameters are set using MIBSPI_Params_init().
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         MIBSPI_Params_init()
 */
typedef struct MIBSPI_SlaveModeParams_t
{
    uint32_t            dmaReqLine;                /*!< DMA request line to be used for Slave */

    uint8_t             chipSelect;                /*!< CS0-CS7 signal number from 0 -7 */
    
} MIBSPI_SlaveModeParams;


/*!
 *  @brief      The definition of a callback function used by the SPI driver
 *              when used in ::MIBSPI_MODE_CALLBACK
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @param      MIBSPI_Handle          MIBSPI_Handle
 *  @param      MIBSPI_Transaction*    MIBSPI_Transaction*
 */
typedef void        (*MIBSPI_CallbackFxn) (MIBSPI_Handle handle,
                                           MIBSPI_Transaction * transaction);


/*!
 *  @brief MIBSPI Parameters
 *
 *  MIBSPI Parameters are used with the MIBSPI_open() call. Default values for
 *  these parameters are set using MIBSPI_Params_init().
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         MIBSPI_Params_init()
 */
typedef struct MIBSPI_Params_t
{
    MIBSPI_TransferMode    transferMode;       /*!< Blocking or Callback mode */
    uint32_t               transferTimeout;    /*!< Transfer timeout in system
                                                 ticks (Not supported with all
                                                 implementations */
    MIBSPI_CallbackFxn     transferCallbackFxn;/*!< Callback function pointer */
    MIBSPI_Mode            mode;               /*!< Master or Slave mode */
    uint32_t               bitRate;            /*!< SPI bit rate in Hz */
    uint32_t               dataSize;           /*!< SPI data frame size in bits */
    MIBSPI_FrameFormat     frameFormat;        /*!< SPI frame format */
    union 
    {
        MIBSPI_MasterModeParams  masterParams;        /*!< Configuration dedicated to master mode, refer to ::MIBSPI_MasterModeParams */
    
        MIBSPI_SlaveModeParams   slaveParams;         /*!< Configuration dedicated to slave mode, refer to ::MIBSPI_SlaveModeParams */
    }u;
    
    MIBSPI_PinMode         pinMode;                   /*!< Pin operation Mode, refer to ::MIBSPI_PinMode */
                                                                                                    
    MIBSPI_DataShiftFmt    shiftFormat;                  /*!< SPI Data shift format, refer to ::MIBSPI_DataShiftFmt */ 
    
    uint8_t             dmaEnable;                 /*!< DMA mode enable. When Dma mode is disable, SPI driver
                                                        is operated in polling mode, there is while(1) loop waiting for
                                                        hardware to finish */
    uint8_t             eccEnable;                 /*!< ECC mode enable */
    
    uint8_t             csHold;                    /*!< CS Hold enable */

    uint16_t            txDummyValue;              /*!< 16 bit value transmitted when no TX data is provided for SPI_tranfer() */

    void               *dmaHandle;                 /*!< DMA handle */

    bool               compatibilityMode;          /*!< Enable Compatibility mode operation of MIBSPI. MIBSPI RAM will be disabled */

    uint16_t           edmaLinkParamId;            /*!< Link param id.
                                                    * @brief  EDMA link channel id
                                                    * Needs to be set only if compatibility mode is FALSE
                                                    * The MIBSPI requries an additional param to terminate Tx transfer.
                                                    * This should be from the pool of free param sets 
                                                    * As EDMA driver currently does not support dynamic allocation of
                                                    * params forced to statically configure this from application.
                                                    * TODO: Remove this once edma driver supports allocation of paramId
                                                    *       similar to edma3 drv
                                                    */
} MIBSPI_Params;

/*!
 *  @brief  This function initializes the MIBSPI module.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern void MIBSPI_init(void);

/*!
 *  @brief  Function to initialize the MIBSPI_Params struct to its defaults
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *  @param  params      An pointer to MIBSPI_Params structure for
 *                      initialization
 *
 *  Defaults values are:
 *      - mode               = MIBSPI_SLAVE,
 *      - dataSize           = 16U,
 *      - frameFormat        = MIBSPI_POL0_PHA0,
 *      - transferMode       = MIBSPI_MODE_BLOCKING,
 *      - transferTimeout    = MIBSPI_WAIT_FOREVER,
 *      - transferCallbackFxn  = NULL,
 *      - pinMode            = MIBSPI_PINMODE_4PIN_CS,
 *      - shiftFormat        = MIBSPI_MSB_FIRST,
 *      - dmaEnable          = (uint8_t)1U,
 *      - eccEnable          = (uint8_t)0U,
 *      - csHold             = (uint8_t)0U,
 *      - txDummyValue       = (uint16_t)0xFFFFU,
 *      - compatibilityMode  = FALSE,
 *      - dmaHandle          = NULL,
 *
 */
extern void MIBSPI_Params_init(MIBSPI_Params *params);

/*!
 *  @brief  This function opens a given instance of the MIBSPI peripheral.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *  @pre    MIBSPI controller has been initialized using MIBSPI_init()
 *
 *  @param  mibspiInstanceId  Id of the SPI instance.The SPI instances
 *                           available in an SOC are listed in the SOC specific MIBSPI_soc.h
 *                           file
 *
 *  @param  params        Pointer to an parameter block, if NULL it will use
 *                        default values. All the fields in this structure are
 *                        RO (read-only).
 *
 *  @return A MIBSPI_Handle on success or a NULL on an error or if it has been
 *          opened already.
 *
 *  @sa     MIBSPI_init()
 *  @sa     MIBSPI_close()
 */
extern MIBSPI_Handle MIBSPI_open(enum MibSpi_InstanceId mibspiInstanceId, MIBSPI_Params *params);

/*!
 *  @brief  Function to close a instance of a MIBSPI peripheral specified by the MIBSPI handle
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *  @pre    MIBSPI_open() has to be called first.
 *
 *  @param  handle A MIBSPI handle returned from MIBSPI_open()
 *
 *  @sa     MIBSPI_open()
 */
extern void MIBSPI_close(MIBSPI_Handle handle);

/*!
 *  @brief  Function to perform MIBSPI transactions on a instance of
 *          a MIBSPI peripheral specified by the MIBSPI handle for a specific slave.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *  If the MIBSPI is in ::MIBSPI_MASTER mode, it will immediately start the
 *  transaction. If the MIBSPI is in ::MIBSPI_SLAVE mode, it prepares itself for a
 *  transaction with a SPI master.
 *
 *  In ::MIBSPI_MODE_BLOCKING, MIBSPI_transfer will block task execution until the
 *  transaction has completed.
 *
 *  In ::MIBSPI_MODE_CALLBACK, MIBSPI_transfer() does not block task execution and
 *  calls a ::MIBSPI_CallbackFxn. This makes the MIBSPI_tranfer() safe to be used
 *  within a Task, Swi, or Hwi context. The ::MIBSPI_Transaction structure must
 *  stay persistent until the MIBSPI_transfer function has completed!
 *
 *  @param  handle      A MIBSPI_Handle
 *
 *  @param  spiTrans    A pointer to a MIBSPI_Transaction. All of the fields within
 *                      transaction except MIBSPI_Transaction.count and
 *                      MIBSPI_Transaction.status are WO (write-only) unless
 *                      otherwise noted in the driver implementations. If a
 *                      transaction timeout has occured, MIBSPI_Transaction.count
 *                      will contain the number of frames that were transferred.
 *
 *  @return true if started successfully; else false
 *
 *  @sa     MIBSPI_open
 *  @sa     MIBSPI_transferCancel
 */
extern bool MIBSPI_transfer(MIBSPI_Handle handle, MIBSPI_Transaction *spiTrans);

/*!
 *  @brief  Function to cancel MIBSPI transactions on instance of a
 *          MIBSPI peripheral specified by the MIBSPI handle.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  In ::MIBSPI_MODE_BLOCKING, MIBSPI_transferCancel has no effect.
 *
 *  In ::MIBSPI_MODE_CALLBACK, MIBSPI_transferCancel() will stop an SPI transfer if
 *  if one is in progress.
 *  If a transaction was in progress, its callback function will be called
 *  in context from which this API is called from. The ::MIBSPI_CallbackFxn
 *  function can determine if the transaction was successful or not by reading
 *  the ::MIBSPI_Status status value in the ::MIBSPI_Transaction structure.
 *
 *  @param  handle      A MIBSPI_Handle
 *
 *  @sa     MIBSPI_open
 *  @sa     MIBSPI_transfer
 */
extern void MIBSPI_transferCancel(MIBSPI_Handle handle);

/*!
 *  @brief  Function performs implementation specific features 
 *          of a MIBSPI peripheral specified by the MIBSPI handle.
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @pre    MIBSPI_open() has to be called first.
 *
 *  @param  handle      A MIBSPI handle returned from MIBSPI_open()
 *
 *  @param  cmd         A command value defined by the driver specific
 *                      implementation
 *
 *  @param  arg         An optional R/W (read/write) argument that is
 *                      accompanied with cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  @sa     MIBSPI_open()
 */
extern int32_t MIBSPI_control(MIBSPI_Handle handle, uint32_t cmd, void *arg);

/*!
 *  @brief  Function to get driver statistics
 *
 *  @ingroup MIBSPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @param  handle      A MIBSPI_Handle
 *  @param  ptrStats    Stats structure to be populated by driver
 *
 *  @sa     MIBSPI_open
 *  @sa     MIBSPI_transfer
 */
extern int32_t MIBSPI_getStats(MIBSPI_Handle handle, MIBSPI_Stats *ptrStats);

#ifdef __cplusplus
}
#endif

#endif /* _MIBSPI_H */

/* @} */
