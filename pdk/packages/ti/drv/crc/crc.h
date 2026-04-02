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
 *  \defgroup DRV_CRC_MODULE CRC Driver
 *
 *  The CRC Driver allows the calculation of the CRC on a specified data.
 *
 *  The CRC header file should be included in an application as follows:
 *  \code
    #include <ti/drv/crc/crc.h>
    \endcode
 *
 *  The crc/include/reg_crc.h has the register layer definitons for the
 *  CRC Module.
 *
 * ## Opening the driver #
 * Once the CRC Driver has been initialized; the CRC Driver instance can be opened
 * using the #CRC_open. The helper function #CRC_initConfigParams can be used to
 * populate the default parameters
 *
 * ## Transaction Id #
 * Each driver requires access to a unique transaction identifier before it can
 * start using the driver. Transaction identifiers ensure that if the CRC Driver
 * instance is shared between multiple threads the usage from one thread does not
 * corrupt the usage from another thread.
 *
 * Use the #CRC_getTransactionId to get a unique transaction identifer. On success
 * the driver is marked as owned and belongs to the callee. The function can fail
 * with an error code CRC_EINUSE to indicate the driver belongs to another enrity
 * and cannot be used right now.
 *
 * Transaction identifiers are released on a call to #CRC_getSignature when the computed
 * CRC signature is passed back to the callee *OR* on the #CRC_cancelSignature when
 * the callee is no longer interested in calculating the CRC operation.
 *
 * ## Using the driver #
 *
 * As mentioned above a successful call to #CRC_getTransactionId will return a unique
 * transaction id. Applications can then invoke the #CRC_computeSignature API from 1 to N
 * times passing different data buffers over which the CRC is to be computed. The following
 * pseudo code reflects the API usage:-
 *
 * \code

    // Acquire a unique transaction identifer
    status = CRC_getTransactionId (crcDriverHandle, &transId, ...)
    if (status == 0)
    {
        // Successfully retreived a transaction id. We own the CRC
        // driver now!

        // Perform signature computation for Data Buffer-1
        sigGenCfg.transactionId = transId;
        sigGenCfg.ptrData       = &data_buffer1[0];
        sigGenCfg.dataLen       = 128;
        CRC_computeSignature (crcDriverHandle, )
        ...
        // Perform signature computation for Data Buffer-2
        sigGenCfg.transactionId = transId;
        sigGenCfg.ptrData       = &data_buffer2[0];
        sigGenCfg.dataLen       = 512;
        CRC_computeSignature (crcDriverHandle, )
        ...
        // Get the CRC Signature. This will also release the transaction identifer
        CRC_getSignature (crcDriverHandle, transId, &crc, ...)
    }

   \endcode
 *
 * The results of the CRC signature can be retreived using the #CRC_getSignature API.
 * This will also release the transaction identifier and another user of the CRC Driver
 * can start using the driver now.
 *
 * It is also possible to cancel requests for CRC signature generation by using the
 * #CRC_cancelSignature API. The API will also release the transaction identifer.
 *
 */

/**
 *   \file  drv/crc/crc.h
 *
 *   \brief
 *      This is the header file for the CRC driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the CRC driver.
 */

#ifndef CRC_DRIVER_H
#define CRC_DRIVER_H

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief CRC driver error base */
#define CRC_ERRNO_BASE                      (-2100)

/**
\defgroup CRC_DRIVER_EXTERNAL_FUNCTION            CRC Driver External Functions
\ingroup DRV_CRC_MODULE
\brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
\defgroup CRC_DRIVER_EXTERNAL_DATA_STRUCTURE      CRC Driver External Data Structures
\ingroup DRV_CRC_MODULE
\brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
\defgroup CRC_DRIVER_ERROR_CODE                   CRC Driver Error Codes
\ingroup DRV_CRC_MODULE
\brief
*   The section has a list of all the error codes which are generated by the CRC Driver
*   module
*/

/** \addtogroup CRC_DRIVER_ERROR_CODE
 *  Base error code for the CRC module
 @{ */

/**
 * \brief   Error Code: Invalid argument
 */
#define CRC_EINVAL                  (CRC_ERRNO_BASE-1)

/**
 * \brief   Error Code: Out of memory
 */
#define CRC_ENOMEM                  (CRC_ERRNO_BASE-2)

/**
 * \brief   Error Code: Operation cannot be implemented because the CRC driver
 * is in use
 */
#define CRC_EINUSE                  (CRC_ERRNO_BASE-3)

/**
@}
*/

/** \addtogroup CRC_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/**
 * \brief
 *  CRC Driver Handle
 */
typedef void*   CRC_Handle;

/**
 * \brief
 *  CRC Channel
 *
 * \details
 *  Enumeration which describes the CRC channel numbers which are available
 */
typedef enum CRC_Channel_e
{
    /**
     * \brief   Channel-1
     */
    CRC_Channel_CH1 = 0x1,

    /**
     * \brief   Channel-2
     */
    CRC_Channel_CH2
}CRC_Channel;

/**
 * \brief
 *  CRC Operational Mode
 *
 * \details
 *  Enumeration which describes the operational mode.
 */
typedef enum CRC_Operational_Mode_e
{
    /**
     * \brief   CRC channel is operating in FULL CPU Mode. The CPU is
     * responsible for the data pattern tranfers and signature verification
     */
    CRC_Operational_Mode_FULL_CPU = 0x1
}CRC_Operational_Mode;

/**
 * \brief
 *  CRC Type
 *
 * \details
 *  Enumeration which describes the different CRC polynomials which
 *  are supported by the CRC driver.
 */
typedef enum CRC_Type_e
{
    /**
     * \brief   CCITT
     */
    CRC_Type_16BIT  =   0x1,

    /**
     * \brief   Ethernet
     */
    CRC_Type_32BIT,

    /**
     * \brief   ISO 3309
     */
    CRC_Type_64BIT
}CRC_Type;

/**
 * \brief
 *  CRC Data Length
 *
 * \details
 *  Enumeration which describes the data lengths which are supported by
 *  the CRC Driver.
 */
typedef enum CRC_DataLen_e
{
    /**
     * \brief   16-bit
     */
    CRC_DataLen_16_BIT = 1,

    /**
     * \brief   32-bit
     */
    CRC_DataLen_32_BIT,

    /**
     * \brief   64-bit
     */
    CRC_DataLen_64_BIT
}CRC_DataLen;

/**
 * \brief
 *  Bit Swapping
 *
 * \details
 *  Enumeration which describes the supported bit swapping modes
 */
typedef enum CRC_BitSwap_t
{
    /**
     * \brief   Most Significant Bit is first
     */
    CRC_BitSwap_MSB     = 1,

    /**
     * \brief   Least Significant Bit is first
     */
    CRC_BitSwap_LSB
}CRC_BitSwap;

/**
 * \brief
 *  Byte Swapping
 *
 * \details
 *  Enumeration which describes the supported byte swapping modes
 */
typedef enum CRC_ByteSwap_t
{
    /**
     * \brief   Byte swap is disabled
     */
    CRC_ByteSwap_DISABLED     = 1,

    /**
     * \brief   Byte swap is enabled
     */
    CRC_ByteSwap_ENABLED
}CRC_ByteSwap;

/**
 * \brief
 *  CRC Configuration
 *
 * \details
 *  The structure describes the configuration information which is needed
 *  to open the handle to the CRC module.
 */
typedef struct CRC_Config_t
{
    /**
     * \brief   CRC Channel number
     */
    CRC_Channel                 channel;

    /**
     * \brief   Operational Mode
     */
    CRC_Operational_Mode        mode;

    /**
     * \brief   CRC Polynomial type
     */
    CRC_Type                    type;

    /**
     * \brief   CRC Bit Swapping mode
     */
    CRC_BitSwap                 bitSwap;

    /**
     * \brief   CRC Byte Swapping mode
     */
    CRC_ByteSwap                byteSwap;

    /**
     * \brief   Data Length
     */
    CRC_DataLen                 dataLen;
}CRC_Config;

/**
 * \brief
 *  CRC signature generation configuration
 *
 * \details
 *  The structure describes the configuration which needs to be provided
 *  to generate the signature.
 */
typedef struct CRC_SigGenCfg_t
{
    /**
     * \brief   Transaction identifier which has been allocated
     * #CRC_getTransactionId
     */
    uint32_t    transactionId;

    /**
     * \brief   Pointer to the data buffer for which the CRC is to be
     * computed
     */
    uint8_t*    ptrData;

    /**
     * \brief   Data buffer length (in bytes)
     */
    uint32_t    dataLen;
}CRC_SigGenCfg;

/**
@}
*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 *  @b Description
 *  @n
 *      The function is used to open the CRC Driver with the specific configuration.
 *
 *  \param[in]  ptrCRCCfg
 *      Pointer to the CRC configuration
 *  \param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *      Success     - CRC Driver Handle
 *  \retval
 *      Error       - NULL
 */
extern CRC_Handle CRC_open(const CRC_Config *ptrCRCCfg, int32_t *errCode);

/**
 *  @b Description
 *  @n
 *      The function is used to close a previously opened CRC Driver
 *
 *  \param[in]  crcHandle
 *      Handle to the CRC Driver to be closed
 *  \param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *      Success     - 0
 *  \retval
 *      Error       - <0
 */
extern int32_t CRC_close(CRC_Handle crcHandle, int32_t *errCode);

/**
 *  @b Description
 *  @n
 *      The function is used to allocate a unique transaction identifier for
 *      the CRC operation. Transaction identifiers are used to identify the CRC
 *      signature generation procedure since it is possible that the CRC compute
 *      signature API can be invoked multiple times. If there are multiple users
 *      of the same CRC Driver we should ensure that the User1 compute signature
 *      is not interrupted with the User2 request.
 *
 *      The transaction identifers are automatically released once the result
 *      of the signature is requested *OR* if the signature request is cancelled.
 *
 *  \param[in]  crcHandle
 *      CRC Driver Handle
 *  \param[out]  transactionId
 *      Unique Transaction Identifier allocated
 *  \param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *      Success     - 0
 *  \retval
 *      Error       - <0
 */
extern int32_t CRC_getTransactionId(CRC_Handle crcHandle, uint32_t *transactionId, int32_t *errCode);

/**
 *  @b Description
 *  @n
 *      The function is used to compute the CRC signature for the specified data.
 *      It is possible to invoke the API multiple times in which case the CRC
 *      signature is computed across multiple data buffers.
 *
 *  \param[in]  crcHandle
 *      CRC Driver Handle
 *  \param[in]  ptrSigGenCfg
 *      Pointer to the signature generation configuration
 *  \param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *      Success     - 0
 *  \retval
 *      Error       - <0
 */
extern int32_t CRC_computeSignature(CRC_Handle crcHandle, CRC_SigGenCfg *ptrSigGenCfg, int32_t *errCode);

/**
 *  @b Description
 *  @n
 *      The function is used to get the generated signature.
 *
 *  \param[in]  crcHandle
 *      CRC Driver Handle
 *  \param[out] transactionId
 *      Transaction Id for which the CRC signature is being retreived
 *  \param[out] ptrSignature
 *      Pointer to the generated signature
 *  \param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *      Success     - 0
 *  \retval
 *      Error       - <0
 */
extern int32_t CRC_getSignature(CRC_Handle crcHandle, uint32_t transactionId, uint64_t *ptrSignature, int32_t *errCode);

/**
 *  @b Description
 *  @n
 *      The function is used to cancel the signature generation.
 *
 *  \param[in]  crcHandle
 *      CRC Driver Handle
 *  \param[in]  transactionId
 *      Transaction Id for which the signature generation is being canceled
 *  \param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *      Success     - 0
 *  \retval
 *      Error       - <0
 */
extern int32_t CRC_cancelSignature(CRC_Handle crcHandle, uint32_t transactionId, int32_t *errCode);

/**
 *  @b Description
 *  @n
 *      The function is used to setup the default configuration parameters for
 *      the CRC Driver.
 *
 *  \param[out]  ptrCRCCfg
 *      Pointer to the default configuration which is populated by the API
 *
 *  \ingroup CRC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *      Not applicable
 */
extern void CRC_initConfigParams(CRC_Config *ptrCRCCfg);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* CRC_DRIVER_H */

