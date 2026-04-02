/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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

/** ===========================================================
 *  @file       drv/adcbuf/adcbuf.h
 *
 *  @brief      ADCBuf driver interface
 *
 */

 /** \defgroup DRV_ADCBUF_MODULE ADCBuf Driver
 *
 *  The ADCBUF in the AWR29xx SoCs can be programmed at a high-level using
 *  the ADCBUF driver software that allows hook-up with an operating system.
 *  The ADCBUF driver exposes programming of most features provided by the IP.
 * 
 * 
 *  The ADCBuf header file should be included in an application as follows:
 *  @code
 *  #include <ti/drv/adcbuf/adcbuf.h>
 *  @endcode
 *
 *  # Operation #
 *  The ADCBuf driver samples an analog waveform at a specified frequency. 
 *  The resulting samples are transferred to a buffer provided by
 *  the application. The driver can either take n samples once, or continuously
 *  sample by double-buffering and providing a callback to process each finished
 *  buffer.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the SYS/BIOS specific primitives to allow for thread-safe
 *  operation.
 *
 *  The platform specific ADCBUF file present in the ti/drv/adcbuf/soc/
 *  directory. This file is built as a part of the ADCBUF Library for the specific
 *  platform.
 * 
 *  The documentation for all APIs can be referred by clicking at above file or
 *  by going to @ref ADCBUF_DRIVER_EXTERNAL_FUNCTION. Example usage of the EDMA
 *  driver can be seen by referring to the EDMA unit test code located at
 *  @verbatim ti/drv/adcbuf/test/example/main_mss.c @endverbatim
 *
 *  ## Opening the driver #
 *
 *  @code
 *      ADCBuf_Handle adcBufHandle;
 *      ADCBuf_Params adcBufParams;
 *
 *      ADCBuf_Params_init(&adcBufParams);
 *      adcBufHandle = ADCBuf_open(Board_ADCBuf0, &adcBufParams);
 *  @endcode
 *
 *
 *  # Implementation #
 *
 *  This module serves as the main interface for applications. Its
 *  purpose is to redirect the module's APIs to specific peripheral
 *  implementations which are specified using a pointer to an ADCBuf_FxnTable.
 *
 *  The ADCBuf driver interface module is joined (at link time) to a
 *  NULL-terminated array of ADCBuf_Config data structures named *ADCBuf_config*.
 *  *ADCBuf_config* is implemented in the application with each entry being an
 *  instance of an ADCBuf peripheral. Each entry in *ADCBuf_config* contains a:
 *  - (ADCBuf_FxnTable *) to a set of functions that implement an ADCBuf peripheral
 *  - (void *) data object that is associated with the ADCBuf_FxnTable
 *  - (void *) hardware attributes that are associated to the ADCBuf_FxnTable
 * 
 *  \section memory_footprint Memory Footprint
 *
 *  The memory footprint of the driver depends on how many APIs are referred to
 *  by an application (all are not necessary for basic operation). The unit test
 *  code's generated memory map file may be used to get an estimate of
 *  the memory foot-print when most APIs are used.
 *
 *
 *  ============================================================================
 */

/**
 *  \file       drv/adcbuf/adcbuf.h
 *
 *  \brief      ADCBUF driver interface
 *
 */
#ifndef ADCBUF_DRV_H
#define ADCBUF_DRV_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "ti/drv/adcbuf/soc/adcbuf_soc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** \brief ADCBUF driver error base */
#define MMWAVE_ERRNO_ADCBUF_BASE             (-2400)

/******************************************************************************/
/************************ Error code defines **********************************/
/******************************************************************************/
/** \defgroup ADCBUF_ERROR_CODES Error Codes
\ingroup DRV_ADCBUF_MODULE
 *
 @{ */

/*!
* \brief  Success status code returned by:
* ADCBuf_control()
*
* Functions return ADCBuf_STATUS_SUCCESS if the call was executed
* successfully.
*/
#define ADCBUF_STATUS_SUCCESS           (0U)

/*!
* \brief   Generic error status code returned by ADCBuf_control().
*
* ADCBuf_control() returns ADCBuf_STATUS_ERROR if the control code was not executed
* successfully.
*
*
*/
#define ADCBUF_STATUS_ERROR             (MMWAVE_ERRNO_ADCBUF_BASE - 1)

/*!
* \brief   An error status code returned by ADCBuf_control() for undefined
* command codes.
*
* ADCBuf_control() returns ADCBuf_STATUS_UNDEFINEDCMD if the control code is not
* recognized by the driver implementation.
*
*
*/
#define ADCBUF_STATUS_UNDEFINEDCMD      (MMWAVE_ERRNO_ADCBUF_BASE - 2)

/*!
* \brief   An error status code returned by ADCBuf_control() for feature not implemented.
*
* ADCBuf_control() returns ADCBuf_STATUS_NOT_IMPLEMENTED if the control command
* was not supported.
*
*
*/
#define ADCBUF_STATUS_NOT_IMPLEMENTED   (MMWAVE_ERRNO_ADCBUF_BASE - 3)

/*!
* \brief   An error status code returned by ADCBuf_control() for invalid input parameters.
*
* ADCBuf_control() returns ADCBuf_STATUS_INVALID_PARAMS if the control code gets
* invalid parameters.
*
*
*/
#define ADCBUF_STATUS_INVALID_PARAMS  (MMWAVE_ERRNO_ADCBUF_BASE - 4)

/*!
* \brief   An error status code returned by ADCBuf_control() for invalid
* command codes.
*
* ADCBuf_control() returns ADCBuf_STATUS_INVALID_CMD if the control code is not
* recognized by the driver implementation.
*
*
*/
#define ADCBUF_STATUS_INVALID_CMD      (MMWAVE_ERRNO_ADCBUF_BASE - 5)

/** @}*/ /* end defgroup ADCBUF_ERROR_CODES */


/*!
 *  @brief      A handle that is returned from an ADCBuf_open() call.
 */
typedef struct ADCBuf_Config_t              *ADCBuf_Handle;


/** @addtogroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE      ADCBUF Driver External Data Structures
\ingroup DRV_ADCBUF_MODULE
 @{ */

/**
* @brief
*  ADCBUF Source selection
*
* @details
*  The structure is used to define ADCBUF driver commands.
*
*/
typedef enum ADCBufSource_e
{
    /**
     * @brief  Select DFE as source.
     */
    ADCBUF_SOURCE_DFE        = 0,

    /**
     * @brief  Select HIL as source.
     */
    ADCBUF_SOURCE_HIL
}ADCBufSource;

/**
* @brief
*  ADC Buffer data format Parameters
*
* @details
*  The structure is used to define ADC Buffer data format.
*
*/
typedef struct ADCBuf_dataFormat_t
{
    /* ADC out Format
         0 - Complex Data mode
         1 - Real data mode
      */
    uint8_t     adcOutFormat;

    /* Sample interleave - IQswap
         0 - I is stored in LSB, Q is stored in MSB
         1 - Q is stored in LSB, I is stored in MSB
      */
    uint8_t     sampleInterleave;

    /* channel interleave
         0 - interleaved
         1 - non-interleaved
      */
    uint8_t     channelInterleave;
}ADCBuf_dataFormat;

/**
* @brief
*  ADC Buffer RX channel configuration Parameters
*
* @details
*  The structure is used to define ADC Buffer RX Channel configuration.
*
*
*/
typedef struct ADCBuf_RxChanConf_t
{
    /* RX channel id - 0~3 */
    uint8_t   channel;

    /* Address offset for the channel in Ping/Pong buffer
      Used only in non-interleaved mode, it must be 16 bytes aligned.
     */
    uint16_t  offset;
}ADCBuf_RxChanConf;

/**
* @brief
*  ADC Buffer test pattern configuration Parameters
*
* @details
*  The structure is used to define ADC Buffer test pattern configuration.
*
*/
typedef struct ADCBuf_rxTestPatternConf_t
{
    /* I sample offset */
    uint16_t rxIOffset;

    /* I sample incremental value */
    uint16_t rxIInc;

    /* Q sample offset */
    uint16_t rxQOffset;

    /* Q sample incremental value */
    uint16_t rxQInc;
}ADCBuf_rxTestPatternConf;

/**
* @brief
*  ADC Buffer test pattern configuration Parameters
*
* @details
*  The structure is used to define ADC Buffer test pattern configuration.
*
*/
typedef struct ADCBuf_TestPatternConf_t
{
    /* Test pattern configuration for 4 channels */
    ADCBuf_rxTestPatternConf rxConfig[NUM_RX_CHANNEL];

    /* Periodicity of the pattern */
    uint16_t               period;

    /* Sample count to store in ADC buffer */
    uint16_t               numSamples;
}ADCBuf_TestPatternConf;

/**
* @brief
*  Chirp Quality(CQ) configuration Parameters
*
* @details
*  The structure is used to define Chirp Quality configuration.
*
*
*/
typedef struct ADCBuf_CQConf_t
{
    /*! 0x0 or 0x1:Raw16, 0x2:Raw12, 0x3:Raw14 */
    uint8_t   cqDataWidth;

    /*! Set in case of 3 channel mode  */
    uint8_t   cq96BitPackEn;

    /*! CQ0 Address offset : 16bytes aligned address for Storing CQ0 */
    uint16_t  cq0AddrOffset;

    /*! CQ1 Address offset : 16 bytes aligned address for Storing CQ1 */
    uint16_t  cq1AddrOffset;

    /*! CQ2 Address offset : 16 bytes aligned address for Storing CQ2 */
    uint16_t  cq2AddrOffset;
}ADCBuf_CQConf;

/*!
*  @brief ADC Parameters
*
*  ADC Parameters are used to with the ADCBuf_open() call. Default values for
*  these parameters are set using ADCBuf_Params_init().
*
*/
typedef struct ADCBuf_Params_t
{

    /**
     * @brief   ADC buffer source, DFE or HIL 
     */
    ADCBufSource        source;

    /**
     * @brief   Continuous mode selection 
     */
    uint8_t             continousMode;

    /**
     * @brief   AWR2944: Ping buffer Chirp Threshold
                for non-continous operation 
     */
    uint8_t             chirpThresholdPing;

    /**
     * @brief   xwr16xx/xwr18xx/xwr68xx: Pong buffer Chirp Threshold 
                for non-continous operation 
     */
    uint8_t             chirpThresholdPong;

    /**
     * @brief   Custom configuration, Pointer to a device specific 
                extension of the ADCBuf_Params 
     */
    void                *custom;
} ADCBuf_Params;

/**
* @brief
*  ADCBUF Command
*
* @details
*  The structure is used to define ADCBUF driver commands.
*
*/
typedef enum ADCBufMMWave_CMD_e
{
    /**
     * @brief   set ADCBUF source, it can be DFE or VIN.
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_SRC    = 0,

    /**
     * @brief   Set number of Chirps to be stored in each Ping and Pong buffer.
     *          It should be programmed one less the actual number needed .
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD,

    /**
     * @brief   Set number of Chirps to be stored in each Ping buffer.
     *          It should be programmed one less the actual number needed .
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD,

    /**
     * @brief   Set number of Chirps to be stored in each  Pong buffer.
     *          It should be programmed one less the actual number needed .
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD,

    /**
     * @brief   Enables/Disables Continuous mode for ADCBUF.
     *             1 to enable continuous mode.
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_CONTINUOUS_MODE,

    /**
     * @brief   Starts Continuous mode for ADCBUF.
     *          Number of Samples to store in Ping/Pong buffer needs to be provided.
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_START_CONTINUOUS_MODE,

    /**
     * @brief   Stops Continuous mode for ADCBUF.
     *          The size of the argument size for this command is 0 bytes
     */
    ADCBufMMWave_CMD_STOP_CONTINUOUS_MODE,

    /**
     * @brief   Configures ADCBUF data format.
     *          The size of the argument size for this command is size of \ref ADCBuf_dataFormat
     */
    ADCBufMMWave_CMD_CONF_DATA_FORMAT,

    /**
     * @brief   Enable RX channels and configures the address offset in ADCBUF for the channel.
     *          The size of the argument size for this command is size of \ref ADCBuf_RxChanConf
     */
    ADCBufMMWave_CMD_CHANNEL_ENABLE,

    /**
     * @brief   Disable RX channels specified with RX channel bitmask
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_CHANNEL_DISABLE,

    /**
     * @brief   Test pattern configuration.
     *          The size of the argument size for this command is size of \ref ADCBuf_TestPatternConf
     */
    ADCBufMMWave_CMD_CONF_TEST_PATTERN,

    /**
     * @brief   Starts Test pattern generation. Reboot is required when switching from Test pattern mode
     *          to normal operation mode.
     *          The size of the argument size for this command is 0 bytes
     */
    ADCBufMMWave_CMD_START_TEST_PATTERN,

    /**
     * @brief   Stops Test pattern generation.
     *          The size of the argument size for this command is 0 bytes
     */
    ADCBufMMWave_CMD_STOP_TEST_PATTERN,

    /**
     * @brief   Chirp Quality configuration.
     *          The size of the argument size for this command is size of \ref ADCBuf_CQConf
     */
    ADCBufMMWave_CMD_CONF_CQ,

    /**
     * @brief  Last command.
     */
    ADCBufMMWave_CMD_LAST
}ADCBufMMWave_CMD;

/*!
*  @brief      A function pointer to a driver specific implementation of
*              ADCBuf_close().
*/
typedef void (*ADCBuf_CloseFxn)                     (ADCBuf_Handle handle);


/*!
*  @brief      A function pointer to a driver specific implementation of
*              ADCBuf_open().
*/
typedef ADCBuf_Handle (*ADCBuf_OpenFxn)             (ADCBuf_Handle handle,
                                                     const ADCBuf_Params *params);

/*!
*  @brief      A function pointer to a driver specific implementation of
*              ADCBuf_control().
*/
typedef int_fast16_t (*ADCBuf_ControlFxn)           (ADCBuf_Handle handle,
                                                     uint_fast8_t cmd,
                                                     void *arg);
/*!
*  @brief      A function pointer to a driver specific implementation of
*              ADCBuf_init().
*/
typedef void (*ADCBuf_InitFxn)                      (ADCBuf_Handle handle);

/*!
*  @brief      A function pointer to a driver specific implementation of
*              ADCBuf_getChanBufAddr().
*/
typedef uint32_t (*ADCBuf_GetChanBufAddrFxn)         (ADCBuf_Handle handle,
                                                     uint8_t channel,
                                                     int32_t *errCode);

/*!
*  @brief      The definition of an ADCBuf function table that contains the
*              required set of functions to control a specific ADC driver
*              implementation.
*/
typedef struct ADCBuf_FxnTable_t
{
    /*! Function to close the specified peripheral */
    ADCBuf_CloseFxn            closeFxn;
    /*! Function to driver implementation specific control function */
    ADCBuf_ControlFxn          controlFxn;
    /*! Function to initialize the given data object */
    ADCBuf_InitFxn             initFxn;
    /*! Function to open the specified peripheral */
    ADCBuf_OpenFxn             openFxn;
    /*! Function to get ADC buffer address for a given channel */
    ADCBuf_GetChanBufAddrFxn   getChanBufAddr;
} ADCBuf_FxnTable;

/*!
*  @brief ADCBuf Global configuration
*
*  The ADCBuf_Config structure contains a set of pointers used to characterise
*  the ADC driver implementation.
*
*  This structure needs to be defined before calling ADCBuf_init() and it must
*  not be changed thereafter.
*
*/
typedef struct ADCBuf_Config_t
{
    /*! Pointer to a table of driver-specific implementations of ADC APIs */
    const ADCBuf_FxnTable   *fxnTablePtr;

    /*! Pointer to a driver specific data object */
    void                    *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void const              *hwAttrs;
} ADCBuf_Config;


/**
 * @brief
 *  ADCBUF CQ monitoring type
 *
 * @details
 *  used to define CQ monitoring types.
 */
typedef uint32_t ADCBufMMWave_CQType;

#define ADCBufMMWave_CQType_CQ0          ((uint32_t) 0) /*! CQ type for CQ0. */
#define ADCBufMMWave_CQType_CQ1          ((uint32_t) 1) /*! CQ type for CQ1. */
#define ADCBufMMWave_CQType_CQ2          ((uint32_t) 2) /*! CQ type for CQ2. */
#define ADCBufMMWave_CQType_MAX_CQ       ((uint32_t) 3) /*! Maximum number of CQ which can be supported. */

/** @}*/ /* end defgroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
\defgroup ADCBUF_DRIVER_EXTERNAL_FUNCTION       ADCBUF Driver External Functions
\ingroup DRV_ADCBUF_MODULE
\brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
 @{ */

/**
*  @b Description
*  @n
*      Function to close an ADC peripheral specified by the ADC handle
*
*  @param[in]  handle
*      Handle to the ADCBUF instance obtained through call to @ref ADCBuf_open.
*/
extern void ADCBuf_close(ADCBuf_Handle handle);


/**
*  @b Description
*  @n
*      Function performs implementation specific features on a given
*      ADCBuf_Handle.
*
*  @param[in]  handle
*      Handle to the ADCBUF instance obtained through call to @ref ADCBuf_open.
*  @param[in] cmd
*      A command value defined by the driver specific implementation @ref ADCBufMMWave_CMD. \n
*  @param[in] arg
*      A pointer to an optional R/W (read/write) argument that is accompanied with cmd. 
*      arg should be 4 bytes aligned.
*  @retval
*      Success     - @ref ADCBUF_STATUS_SUCCESS
*  @retval
*      Error       - one of @ref ADCBUF_ERROR_CODES
*/
extern int_fast16_t ADCBuf_control(ADCBuf_Handle handle, uint_fast8_t cmd, void * arg);


/**
*  @b Description
*  @n
*      This function gets the physical address of ADCBuf for a given receive channel.
*
*  @param[in]  handle
*      Handle to the ADCBUF instance obtained through call to @ref ADCBuf_open.
*  @param[in] channel
*      Receive channel number.
*  @param[in] errCode
*      Pointer to an error code populated by the driver.
*  @retval
*      Success     - An ADCBuf physical address for the given channel.
*  @retval
*      Error       - 0 on an error.
*/
extern uint32_t ADCBuf_getChanBufAddr(ADCBuf_Handle handle, uint8_t channel, int32_t *errCode);


/**
*  @b Description
*  @n
*      This function gets the physical address of chirp info(CQ) buffer for a given CQ type.
*
*  @param[in]  handle
*      Handle to the ADCBUF instance obtained through call to @ref ADCBuf_open.
*  @param[in] cqType
*      Type of CQ that request the address.
*  @param[in] errCode
*      Pointer to an error code populated by the driver.
*  @retval
*      Success     - An CQ buffer physical address for the given cqType
*  @retval
*      Error       - 0 on an error.
*/
extern uint32_t ADCBUF_MMWave_getCQBufAddr
(
    ADCBuf_Handle handle,
    ADCBufMMWave_CQType cqType,
    int32_t *errCode
);

/**
*  @b Description
*  @n
*      This function initializes the ADC module. This function must be called
*      before any other functions are called.
*
*/
extern void ADCBuf_init(void);

/**
*  @b Description
*  @n
*      This function sets all fields of a specified ADCBuf_Params structure to their
*      default values.
*
*  @param[in]  params      A pointer to ADCBuf_Params structure for initialization
*
*  Default values are:
*                      source             = ADCBUF_SOURCE_DFE,
*                      continousMode      = 0;
*                      chirpThresholdPing = 1,
*                      chirpThresholdPong = 1,
*                      custom             = NULL
*
*/
extern void ADCBuf_Params_init(ADCBuf_Params *params);

/**
*  @b Description
*  @n
 *      This function opens a given ADCBuf peripheral.
 *
 *  @param[in]  index   Logical peripheral number for the ADCBuf indexed into
 *                      the ADCBuf_config table
 *
 *  @param[in]  params  Pointer to an parameter block, if NULL it will use
 *                      default values.
 *
 *  @return An ADCBuf_Handle on success or a NULL on an error or if it has been
 *          opened already. If NULL is returned further ADC API calls will
 *          result in undefined behaviour.
 *
 */
extern ADCBuf_Handle ADCBuf_open(uint_fast8_t index, ADCBuf_Params *params);

/** @}*/ /* end defgroup ADCBUF_DRIVER_EXTERNAL_FUNCTION */

#ifdef __cplusplus
}
#endif
#endif /* ADCBUF_DRV_H */
