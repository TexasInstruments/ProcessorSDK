#ifndef _SALLD_H
#define _SALLD_H

/* ========================================================================== */
/**
 *  @file   salld.h
 *
 *  path    ti/drv/sa/salld.h
 *
 *  @brief  SA LLD Interface Unit API and Data Definitions
 *
 *  ============================================================================
 *  Copyright (c) Texas Instruments Incorporated 2009-2018
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
*/

/* Define SALLD Module as a master group in Doxygen format and add all SA LLD API
   definitions to this group. */
/** @defgroup salld_module SA LLD Module API
 *  @{
 *
 *  Security Accelerator Low Level Driver
 *
 *   @section intro  Introduction
 *
 *   The Security Accelerator (SA) also known as cp_ace (Adaptive Cryptographic Engine) is designed to 
 *   provide packet security as part of IPSEC, SRTP, and 3GPP industry standards. The security accelerator 
 *   low level driver (referred to as the module) provides APIs for the configuration and control of 
 *   the security accelerator sub-system. The SA low level driver provides an abstraction layer between 
 *   the application and the Security Accelerator Sub System (SASS). It provides both the system level 
 *   interface and the channel-level interface with a set of APIs defined here.
 *
 *   The common interface maintains the system-level resources and needs to be coordinated among multiple 
 *   CGEM cores. All the data access provided by the common interface will invoke the SA CSL layer. 
 *   The common interface performs the following tasks:
 *    -	Reset, download and update the SA PDSP images.
 *    -	Query SA states and statistics.
 *    - Read a 64-bit true random number.
 *    -	Perform the large integer arithmetic through the PKA module.
 *    -	Monitor and report SA system errors.
 *
 *   The channel interface performs protocol-specific pre-processing for all the packets to be passed to 
 *   the SA and protocol-specific post-processing of all the packets received from the SA. The channel interface 
 *   performs the following tasks:
 *   - Convert the channel configuration information into the security contexts defined by the SA.
 *   - Perform protocol-specific packet operations such as insertion of the ESP header, padding and ESP tail.
 *   - Decrypt and authenticate the received SRTP packet when the SA is not able to perform the operations 
 *     due to the key validation failure.
 *   - Generate the SA operation control command labels in data mode operation.
 *   - Maintain the protocol-specific channel statistics.
 *
 *   With the exception of some initial setup functions, the module does not communicate directly with
 *   the sub-system. The driver does not contain a transport layer and is always non-blocking.  The 
 *   software layers above the SA LLD must call the appropriate SA LLD APIs, and then call the appropriate 
 *   PA and/or CPPI and QMSS APIs to actually send the data to the SA sub-system.
 *
 */
/* @} */
 
#ifdef __cplusplus
extern "C" {
#endif

/* System level header files */
#include <stdint.h>
#include <stdlib.h> 
#include <string.h>    

#include <ti/drv/sa/saver.h>

/* 
 * Shut off: remark #880-D: parameter "descType" was never referenced
 *
 * This is better than removing the argument since removal would break
 * backwards compatibility
 */
#ifdef _TMS320C6X
#pragma diag_suppress 880
#pragma diag_suppress 681
#elif defined(__GNUC__)  && !defined(__ti__)
/* Same for GCC:
 * warning: unused parameter descType [-Wunused-parameter]
 * expectation is all these catch up with some other intelligent 
 * tools like coverity, Klocwork etc, instead of dump GNU 
 * warnings
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#else
/* Same for TI ARM CLANG:
 * warning: unused parameter descType [-Wunused] [-Wsometimes-uninitialized]
 * expectation is all these catch up with some other intelligent 
 * tools like coverity, Klocwork etc, instead of dump GNU 
 * warnings
 */ 
#pragma clang diagnostic ignored "-Wunused"
#pragma clang diagnostic ignored "-Wsometimes-uninitialized"
#endif


/** @defgroup salld_api_functions SA LLD Functions
 *  @ingroup salld_module
 */
 
/** @defgroup salld_api_macros SA LLD Macros
 *  @ingroup salld_module
 */

/** @defgroup salld_api_structures SA LLD Data Structures
 *  @ingroup salld_module
 */

/** @defgroup salld_api_constants SA LLD Constants (enums and defines)
 *  @ingroup salld_module
 */
 

/**
 *  @defgroup salldDebugMessageTypes SALLD Debug message types
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD Debug message types
 *
 *  Debug message types used by debugInfo().
 */
/*@{*/
/**
 *  @def  sa_DBG_INFORMATIONAL
 *        SALLD Debug Message Type -- SA LLD general debug information message.
 */
#define sa_DBG_INFORMATIONAL   0
/**
 *  @def  sa_DBG_WARNING
 *        SALLD Debug Message Type -- SA LLD warning debug information message.
 */
#define sa_DBG_WARNING         1 

/**
 *  @def  sa_DBG_FATAL_ERROR
 *        SALLD Debug Message Type -- SA LLD critical debug information message.
 */
#define sa_DBG_FATAL_ERROR     2
/*@}*/
/** @} */

/**
 *  @defgroup salldDebugMessageCodes SALLD Debug message codes
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD Debug message codes
 *
 *  Debug message codes used by debugInfo().
 */
/*@{*/
/**
 *  @def  sa_MSG_NO_MKI_LEN_CHANGE_ON_FLY
 *        SALLD Debug Message Code -- One-the-fly MKI change is not supported.
 */
#define sa_MSG_NO_MKI_LEN_CHANGE_ON_FLY    1
/**
 *  @def  sa_MSG_MKI_LEN_IS_ZERO
 *        SALLD Debug Message Code -- MKI length is zero.
 */
#define sa_MSG_MKI_LEN_IS_ZERO             2 
/*@}*/
/** @} */

/**
 *  @defgroup  salldSubSysStates SALLD Sub-System Queries and States
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD Sub-System Queries and States
 *
 *  SA Sub-System reset state and query arguments used by API function @ref Sa_resetControl
 */
/*@{*/
/**
 *  @def  sa_STATE_RESET  
 *        The Sub-System is in reset.
 */
#define sa_STATE_RESET            0  /**< Sub-System state reset */

/**
 *  @def  sa_STATE_ENABLE
 *        The Sub-System state is enabled.
 */
#define sa_STATE_ENABLE           1  /**< Sub-System state enable  */

/**
 *  @def  sa_STATE_QUERY
 *        Query the Sub-System state.
 */
#define sa_STATE_QUERY            2  /**< Query the Sub-System state */

/**
 *  @def  sa_STATE_INCONSISTENT
 *        The Sub-System state is partially enabled.
 */
#define sa_STATE_INCONSISTENT     3  /**< Sub-System is partially enabled */

/**
 *  @def  sa_STATE_INVALID_REQUEST
 *        Invalid state command to the Sub-System.
 */
#define sa_STATE_INVALID_REQUEST  4  /**< Invalid state command to the Sub-System */

/**
 *  @def  sa_STATE_ENABLE_FAILED
 *        The Sub-System do not respond after restart.
 */
#define sa_STATE_ENABLE_FAILED    5   /**<  The Sub-System did not respond after restart */
/**
 *  @def  sa_STATE_INVALID
 *        The Sub-System is at an invalid state for requested operation.
 */
#define sa_STATE_INVALID          6   /**<  The Sub-System  at an invalid state for requested operation */

/*@}*/
/** @} */

/**
 *  @ingroup salld_api_structures
 *  @brief  Sa_State_t defines the operating state of the security accelerator sub-system
 *
 *  @details  The values in @ref salldSubSysStates are used both to set the state of the security accelerator
 *            sub-system (sa_STATE_RESET and sa_STATE_ENABLE) as well as show the current state
 *            of the system (all values).
 */
typedef int Sa_State_t;

/**
 *  @defgroup salldRetCodes SALLD Function Return Codes
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD Function Return Codes
 *
 *  Error codes returned by SALLD API functions.
 */
/*@{*/
/**
 *  @def  sa_ERR_OK
 *        SALLD Return Codes -- No Error.
 */
#define sa_ERR_OK                0  
/**
 *  @def  sa_ERR_GEN
 *        SALLD Return Codes -- General Error.
 */
#define sa_ERR_GEN              -1 
/**
 *  @def  sa_ERR_PARAMS
 *        SALLD Return Codes -- Incorrect Input parameters.
 */
#define sa_ERR_PARAMS           -2   
/**
 *  @def  sa_ERR_NOMEM
 *        SALLD Return Codes -- Memory buffers not avialable.
 */
#define sa_ERR_NOMEM            -3   

/**
 *  @def  sa_ERR_INV_BUF (deprecated)
 *        SALLD Return Codes -- Invalid buffers.
 */                                                         
#define sa_ERR_INV_BUF          -4   
/**
 *  @def  sa_ERR_INV_PROTO_TYPE
 *        SALLD Return Codes -- Unsupported security protocol type.
 */
#define sa_ERR_INV_PROTO_TYPE   -5
/**
 *  @def  sa_ERR_NO_CTX_BUF
 *        SALLD Return Codes -- Required Context Buffer not available.
 */
#define sa_ERR_NO_CTX_BUF       -6 
/**
 *  @def  sa_ERR_KEY_EXPIRED
 *        SALLD Return Codes -- Security Key is expired.
 */
#define sa_ERR_KEY_EXPIRED      -7 
/**
 *  @def  sa_ERR_REPLAY_OLD
 *        SALLD Return Codes -- Receive Packet is out of replay window range.
 */
#define sa_ERR_REPLAY_OLD       -8 
/**
 *  @def  sa_ERR_REPLAY_DUP
 *        SALLD Return Codes -- Duplicated Packet is received.
 */
#define sa_ERR_REPLAY_DUP       -9 
/**
 *  @def  sa_ERR_AUTH_FAIL
 *        SALLD Return Codes -- Authentication Failure.
 */
#define sa_ERR_AUTH_FAIL        -10 
/**
 *  @def  sa_ERR_PADDING_FAIL
 *        SALLD Return Codes -- ESP Padding Verification Failure.
 */
#define sa_ERR_PADDING_FAIL     -11 
/**
 *  @def  sa_ERR_CLOSE_PENDING
 *        SALLD Return Codes -- Close still pending, wait for Security context to be freed.
 */
#define sa_ERR_CLOSE_PENDING    -12 
/**
 *  @def  sa_ERR_UNSUPPORTED
 *        SALLD Return Codes -- API is not supported.
 */
#define sa_ERR_UNSUPPORTED      -13 
/**
 *  @def  sa_ERR_STATS_UNAVAIL
 *        SALLD Return Codes -- Statistics is not available yet.
 */
#define sa_ERR_STATS_UNAVAIL    -14 
/**
 *  @def  sa_ERR_MODULE_BUSY
 *        SALLD Return Codes -- Module (RNG or PKA) is busy serving pending request.
 */
#define sa_ERR_MODULE_BUSY      -15
/**
 *  @def  sa_ERR_MODULE_UNAVAIL
 *        SALLD Return Codes -- Module (RNG or PKA) is not available (initialized) yet.
 */
#define sa_ERR_MODULE_UNAVAIL   -16 
/**
 *  @def  sa_ERR_PKA_TIMEOUT
 *        SALLD Return Codes -- PKA operation timeout.
 */
#define sa_ERR_PKA_TIMEOUT      -17 
/**
 *  @def  sa_ERR_SWINFO_UNAVAIL
 *        SALLD Return Codes -- Software Information is not available yet.
 */
#define sa_ERR_SWINFO_UNAVAIL   -18
/**
 *  @def  sa_ERR_INV_HANDLE
 *        SALLD Return Codes -- Invalid system or channel handles.
 */                                                         
#define sa_ERR_INV_HANDLE       -19
/**
 *  @def  sa_ERR_INV_ENDIAN_MODE
 *        SALLD Return Codes -- System or Channel instance using the different endian mode than the processor.
 */                                                         
#define sa_ERR_INV_ENDIAN_MODE  -20   
/**
 *  @def  sa_ERR_SCRATCH_MEMORY_FULL
 *        SALLD Return Codes -- No avail scratch memory for key storage
 */
#define sa_ERR_SCRATCH_MEMORY_FULL      -21
/**
 *  @def  sa_ERR_PACKET
 *        SALLD Return Codes -- Error in the input packet such that SALLD is not able to process this packet.
 */
#define sa_ERR_PACKET                   -22   

/**
 *  @def  sa_ERR_INV_INT_MEM
 *        SALLD Return Codes -- Invalid internal memory buffer for key storage
 */
#define sa_ERR_INV_INT_MEM              -23

/** 
  *  @def sa_ERR_API_UNSUPPORTED
  *       The API is not supported by this generation of SASS
  */
#define sa_ERR_API_UNSUPPORTED          -24

/** 
  *  @def sa_ERR_PKA_OP_UNSUPPORTED
  *       The required PKA operation  is not supported by this generation of PKA module
  */
#define sa_ERR_PKA_OP_UNSUPPORTED       -25

/**
 *  @def  sa_ERR_PKA_NOT_READY
 *        Some PKA operation in in progress and it is not ready for new operation.
 */
#define sa_ERR_PKA_NOT_READY            -26 

/**
 *  @def  sa_ERR_PKA_OP_ERROR
 *        The PKA operation return error.
 */
#define sa_ERR_PKA_OP_ERROR             -27

/**
 *  @def  sa_ERR_PKA_INVALID_STATE
 *        PKA API is invoked at inavlid state.
 */
#define sa_ERR_PKA_INVALID_STATE        -28 

/**
 *  @def  sa_ERR_PKA_WORKSPACE_ERROR
 *        There is no enough workspace to execute the PKA complex operaion.
 */
#define sa_ERR_PKA_WORKSPACE_ERROR      -29

/**
 *  @def  sa_ERR_PKA_DOWNLOAD_FAIL
 *        The PKA Firmware download failure.
 */
#define sa_ERR_PKA_DOWNLOAD_FAIL        -30

/**
 *  @def  sa_ERR_ENGINE_STATE
 *        The SA2UL engines are in an unexpected state
 */
#define sa_ERR_ENGINE_STATE             -31

/**
 *  @def  sa_PKA_OP_IN_PROGRESS
 *        The current PKA operation is still in progress.
 */
#define sa_PKA_OP_IN_PROGRESS           1
 
/**
 *  @def  sa_PKA_OP_CONTINUE
 *        The current PKA operation is completed and it has started the next operation in sequence.
 */
#define sa_PKA_OP_CONTINUE              2

/**
 *  @def  sa_PKA_OP_COMPLETE
 *        All PKA operations in sequence are completed and the final results are ready.
 */
#define sa_PKA_OP_COMPLETE              sa_ERR_OK

/**
 *  @def  sa_MAX_PS_AI_WORD_COUNT
 *        upto 12 32-bit words of 'personalizatoin string' and 'additional input' used for
 *        SP-800-90A AES-256 DRBG
 */

#define sa_MAX_PS_AI_WORD_COUNT         12


/*@}*/
/** @} */

/**
 *  @defgroup SecurityProtocolTypes Security Protocol Types
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Security Protocol Types
 *
 *  Definition of Security Protocol Types of the packets to
 *  be processed in the SALLD channel
 *  
 */ 
/*@{*/
typedef enum {
  sa_PT_NULL = 0,        /**< No protocol: Data mode */
  sa_PT_SRTP,            /**< Security RTP */
  sa_PT_SRTCP,           /**< Security RTCP */
  sa_PT_IPSEC_AH,        /**< IPSEC AH mode */
  sa_PT_IPSEC_ESP,       /**< IPSEC ESP mode */
  sa_PT_3GPP_AC          /**< 3GPP Air Ciphering */
} Sa_SecProto_e;
/*@}*/
/** @} */

/**
 *  @defgroup CipherModes Cipher Modes
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Cipher Modes
 *
 *  Definition of Cipher Modes used by the SA sub-engines 
 *  
 */ 
/*@{*/
typedef enum {
  sa_CipherMode_NULL = 0,        /**< No encryption */
  sa_CipherMode_AES_CTR,         /**< AES Counter mode */
  sa_CipherMode_AES_F8,          /**< AES F8 mode */
  sa_CipherMode_AES_CBC,         /**< AES CBC mode */
  sa_CipherMode_DES_CBC,         /**< DES CBC mode */
  sa_CipherMode_3DES_CBC,        /**< 3DES CBC mode */
  sa_CipherMode_CCM,             /**< Counter with CBC-MAC mode */
  sa_CipherMode_GCM,             /**< Galois Counter mode */
  sa_CipherMode_GSM_A53,         /**< 3GPP GSM A5/3 encryption: Key stream generation */
  sa_CipherMode_ECSD_A53,        /**< 3GPP ECSD A5/3 encryption: Key stream generation */
  sa_CipherMode_GEA3,            /**< 3GPP GPRA encryption: Key stream generation */
  sa_CipherMode_KASUMI_F8,       /**< 3GPP Kasumi F8 mode */
  sa_CipherMode_SNOW3G_F8,       /**< 3GPP Snow3G F8 mode */
  sa_CipherMode_ZUC_F8,          /**< 3GPP ZUC F8 mode (SASS_GEN2 only)*/
  sa_CipherMode_LAST
} Sa_CipherMode_e;
/*@}*/
/** @} */

/**
 *  @defgroup AuthModes Authentication Modes
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Authentication Modes
 *  Definition of Authentication Modes used by the SA sub-engines 
 */ 
/*@{*/
typedef enum {
  sa_AuthMode_NULL = 0,          /**< No idviudal Authentication  */
  sa_AuthMode_MD5 = sa_CipherMode_LAST, /**< MD5 mode */
  sa_AuthMode_SHA1,              /**< SHA1 mode */
  sa_AuthMode_SHA2_224,          /**< 224-bit SHA2 mode */
  sa_AuthMode_SHA2_256,          /**< 256-bit SHA2 mode */
  sa_AuthMode_SHA2_384,          /**< 384-bit SHA2 mode
                                      @note: This mode is used at Data Mode only for SA2_UL */
  sa_AuthMode_SHA2_512,          /**< 512-bit SHA2 mode
                                      @note: This mode is used at Data Mode only for SA2_UL */
  sa_AuthMode_HMAC_MD5,          /**< HMAC with MD5 mode */
  sa_AuthMode_HMAC_SHA1,         /**< HMAC with SHA1 mode */
  sa_AuthMode_HMAC_SHA2_224,     /**< HMAC with 224-bit SHA2 mode */
  sa_AuthMode_HMAC_SHA2_256,     /**< HMAC with 256-bit SHA2 mode */
  sa_AuthMode_HMAC_SHA2_384,      /**< HMAC with 224-bit SHA mode
                                      @note: This mode is used at Data Mode only for SA2_UL */
  sa_AuthMode_HMAC_SHA2_512,      /**< HMAC with 256-bit SHA mode
                                      @note: This mode is used at Data Mode only for SA2_UL */
  sa_AuthMode_GMAC,              /**< Galois Message Authentication Code mode */
  sa_AuthMode_GMAC_AH,           /**< Galois Message Authentication Code mode for IPSEC AH operation 
                                      @note: This mode is used at Data Mode only  */
  sa_AuthMode_CMAC,              /**< Cipher-based Message Authentication Code mode */
  sa_AuthMode_CBC_MAC,           /**< Cipher Block Chaining - Message Authentication Code mode */
  sa_AuthMode_AES_XCBC,          /**< AES Extended Cipher Block Chaining - Message Authentication Code mode */
  sa_AuthMode_KASUMI_F9,         /**< 3GPP Kasumi F9 mode */
  sa_AuthMode_SNOW3G_F9,         /**< 3GPP Snow3G F9 mode (SASS_GEN2 only) */
  sa_AuthMode_ZUC_F9             /**< 3GPP ZUC F9 mode (SASS_GEN2 only) */
} Sa_AuthMode_e;
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief Specification of Sa_ChanHandle 
 * The Sa_ChanHandle is used to identify a SALLD channel instance
 */
typedef void*   Sa_ChanHandle;

/**
 *  @defgroup saSizeConfigCtrlBit SA Size Configuration Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SA Size Configuration Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in Sa_ChanSizeCfg_t and Sa_SizeCfg_t. 
 *  
 */ 
/*@{*/
/**
 *  @def  sa_SIZE_CONFIG_CREATE_SHADOW_INST
 *        Control Info -- 0:Do not create shadow instance (refer to @ref appendix2)
 *                        1:Create Shadow instance for mixed-endianness operation  
 */

#define  sa_SIZE_CONFIG_CREATE_SHADOW_INST     0x0001


/**
 *  @def  sa_SIZE_CONFIG_SASS_GEN2
 *        Control Info -- 0:First generation Security Accelerator Sub-System (SASS)
 *                        1:Second generattion SASS at more advanced keystone2 devices such as Lamarr and Edison 
 *                          (TBD: to be replaced with part number) 
 */

#define  sa_SIZE_CONFIG_SASS_GEN2              0x0002

/**
 *  @def  sa_SIZE_CONFIG_SASS_UL_GEN2
 *        Control Info -- 0:First generation Security Accelerator Ultra Lite Sub-System (SAUL)
 *                        1:Second generattion Security Aaccelerator Ultra Lite Sub-System (SA2_UL)
 *                          at more advanced keystone3 devices such as Maxwell
 */

#define  sa_SIZE_CONFIG_SASS_UL_GEN2              0x0003


/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SALLD channel size configuration structure
 *
 * DESCRIPTION: Data structure that defines memory requirements for SALLD channel instance
 *
 */
typedef struct {
  Sa_SecProto_e protocolType;   /**< Security protocol type as defined by @ref SecurityProtocolTypes */
  int           cacheLineSize;  /**< Specify the size of cache line of the memory where the channel buffers will reside.
                                     Set this variable to 0 if this channel is not shared among multiple cores 
                                     @note cacheLineSize should be the larger of cache line sizes amoung mixed processors */
  uint16_t      ctrlBitMap;     /**< Various configuration information as specified at @ref saSizeConfigCtrlBit */                                   
} Sa_ChanSizeCfg_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD Channel Configuration structure
 *
 * Data structure providing configuration information in Sa_chanInit()
 *
 */
typedef struct {
  uint32_t          ID;             /**< User specified channel ID */
  Sa_ChanSizeCfg_t  sizeConfig;     /**< SALLD memory allocation requirement structure */
  uint16_t          engSelect;      /**< User selected encryption/authentication engine set number, 
										 valid only when Sa_Config_t.engSelMode is sa_EngSelMode_USERSPECIFIED */
} Sa_ChanConfig_t;

/**
 * @ingroup salld_api_structures
 * @brief SRTP Configuration Parameters structure
 *
 * Data structure defines the SRTP specific configuration parameters excluding the keys
 *
 */
typedef struct {
  uint16_t    masterKeySize;      /**< Specify the size of the master key in bytes */  
  uint16_t    masterSaltSize;     /**< Specify the size of the master salt in bytes */  
  uint16_t    sessionEncKeySize;  /**< Specify the size of the session encryption key in bytes */  
  uint16_t    sessionMacKeySize;  /**< Specify the size of the session mac key in bytes */  
  uint16_t    sessionSaltSize;    /**< Specify the size of the session salt in bytes */  
  uint16_t    macSize;            /**< Specify the size of the authentication tag in bytes */  
} Sa_SrtpConfigParams_t;

/**
 *  @defgroup saIpsecTransportTypes IPSEC Transport Types
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name IPSEC Transport Types
 *
 *  Parameter transportType of Sa_IpsecConfigParams_t should be set to
 *  one of these transport types.
 */ 
/*@{*/
typedef enum {
  sa_IPSEC_TRANSPORT_TRANSPORT = 0,  /**< Trasport mode */
  sa_IPSEC_TRANSPORT_TUNNEL          /**< Tunnel mode */
} Sa_IpsecTransportType_e;
/*@}*/
/** @} */

/**
 *  @defgroup saIpsecConfigCtrlBit  IPSEC Configuration Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name IPSEC Configuration Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in salldInpsecConfigParams_t. 
 *  
 */ 
/*@{*/
/**
 *  @def  sa_IPSEC_CONFIG_ESN
 *        Control Info -- 0:Disable Extended Sequence Number
 *                        1:Enable Extended Sequence Number  
 */
#define sa_IPSEC_CONFIG_ESN                  0x0001 
/**
 *  @def  sa_IPSEC_CONFIG_PERMANENT_SC
 *        Control Info -- 0:The corresponding security context is marked as second tier
 *                        1:The corresponding security context ia marked as first tier so that it will
 *                          not be evicted automatically. 
 */
#define sa_IPSEC_CONFIG_PERMANENT_SC         0x0002 
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief IPSEC Configuration Parameters structure
 *
 * Data structure defines the IPSEC specific configuration parameters excluding the keys
 *
 */
typedef struct {
  uint16_t    transportType;       /**< Specify the transport Type as defined at @ref saIpsecTransportTypes */
  uint16_t    ctrlBitMap;          /**< Various control information as specified at @ref saIpsecConfigCtrlBit */                                                                       
  uint16_t    encryptionBlockSize; /**< Specify the encryption block size 
                                      1: Stream encryption and no alignment requirement
                                      4: AES-CTR or other stream-like encryption with 4-byte 
                                         alignment
                                      block size: block encryption algorithm */
  uint16_t    sessionEncKeySize;   /**< Specify the size of the session encryption key in bytes */  
  uint16_t    sessionMacKeySize;   /**< Specify the size of the session mac key in bytes */ 
  uint16_t    sessionSaltSize;     /**< Specify the size of the session salt in bytes */  
  uint16_t    ivSize;              /**< Specify the size of the initialization vector in bytes */  
  uint16_t    macSize;             /**< Specify the size of the authentication tag in bytes */ 
  uint16_t    nextHdr;             /**< Specify the next protocol header */
                                   /**< @note: This parameter is no longer used. */ 
  uint32_t    spi;                 /**< Specify the SPI (Security Parameter Index) */
  uint32_t    esnLo;               /**< Specify the initial value of the (extended) sequence Number (lower 32-bit) */
  uint32_t    esnHi;               /**< Specify the initial value of the extended sequence Number (upper 32-bit) */
} Sa_IpsecConfigParams_t;

/**
 *  @defgroup AcPduTypes Air Ciphering PDU Types
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Air Ciphering PDU Types
 *
 *  Definition of Air Ciphering PDU Types supported in SA 
 *  Parameter pduType of Sa_AcConfigParams_t should be set to
 *  one of these types. Refer to @ref appendix1 for detailed 
 *  description 
 *  
 */ 
/*@{*/
typedef enum {
  sa_AcPduType_GSM = 0,         /**< All GSM PDUs */
  sa_AcPduType_WCDMA_TMD,       /**< WCDMA MAC TMD */
  sa_AcPduType_WCDMA_UMD,       /**< WCDMA RLC UMD */
  sa_AcPduType_WCDMA_AMD,       /**< WCDMA RLC AMD */
  sa_AcPduType_LTE,             /**< LTE PDCP PDUs */
  sa_AcPduType_LTE_CP           /**< PDCP Control Plane Data Uint for SRBs */
} Sa_AcPduType_e;
/*@}*/
/** @} */

/**
 *  @defgroup SALLDAcConfigCtrlBit   AC Configuration Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name AC Configuration Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in Sa_AcConfigParams_t. 
 *  
 */ 
/*@{*/
/**
 *  @def  sa_AC_CONFIG_BEARER_MASK
 *        Control Info -- 5-bit or 8-bit Bearer mask
 */
#define sa_AC_CONFIG_BEARER_MASK            0x00FF 
/**
 *  @def  sa_AC_CONFIG_DIR
 *        Control Info -- 0:UE to RNC(uplink)
 *                        1:RNC to UE(downlink)  
 */
#define sa_AC_CONFIG_DIR                    0x0100 
/**
 *  @def  sa_AC_CONFIG_COPY_COUNTC
 *        Control Info -- 1: Copy Count-C into the timestamp field within CPPI Descriptor for reference
 *                        0: Otherwise  
 */
#define sa_AC_CONFIG_COPY_COUNTC            0x0200 

/**
 *  @def  sa_AC_CONFIG_COUNTC_BY_APP
 *        Control Info -- 1: Count-C is provided by application.
 *                        0: Count-C is incremented and stored at SASS
 *
 * @note: This flag is only applicable for the to-air traffic when PDU type is set to LTE 
 *        where the count-C is maintained by SASS by default.
 *        This flag may be used to inform SASS that the count-C will be provided by application 
 *        for LTE data traffic in slow path.  
 */
#define sa_AC_CONFIG_COUNTC_BY_APP          0x0400 

/**
 *  @def  sa_AC_CONFIG_KEY_IN_SCRATCH
 *        Control Info -- 0:key is copied into security context by lld
 *                        1:key is copied into scratch memory by lld 
 *  @note: This feature is not supported for KeyStone devices such as C6678
 */
#define sa_AC_CONFIG_KEY_IN_SCRATCH         0x0800 

/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief AC (Air Ciphering) Configuration Parameters structure
 *
 * Data structure defines the AC specific configuration parameters excluding the keys
 *
 */
 
typedef struct {
  uint32_t    countC;              /**< Specify the high bits, HFN, for the frame Counter 
                                      WCDMA RLC AM: the high 20 bits are used
                                      WCDMA RLC UM: the high 25 bits are used
                                      WCDMA RLC TM: the high 25 bits are used
                                      LTE: All 32-bit is used as Count-C
                                      GSM: Not used */
  uint32_t    fresh;               /**< Specify the 32-bit random number (fresh) required
                                      for some integrity check algorithms */   
  uint32_t    ivLow26;             /**< Specify the low 26-bit value of the initialization vector */   
  uint16_t    pduType;             /**< Specify the Air Ciphering PDU Type as defined at @ref Sa_AcPduType_e */
  uint16_t    ctrlBitMap;          /**< Various control information as specified at @ref SALLDAcConfigCtrlBit */                                                                       
  uint16_t    sessionEncKeySize;   /**< Specify the size of the session encryption key (Kc) in bytes 
                                        Note: The LLD may expand the key stream based on the encryption mode */  
  uint16_t    sessionMacKeySize;   /**< Specify the size of the session mac key in bytes */ 
  uint16_t    ivSize;              /**< Specify the size of the initialization vector in bytes */  
  uint16_t    macSize;             /**< Specify the size of the authentication tag in bytes */ 
} Sa_AcConfigParams_t;

/**
 *  @defgroup SALLDDmConfigCtrlBit   Data Mode Configuration Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Data Mode Configuration Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in Sa_DmConfigParams_t. 
 *  
 */ 
/*@{*/
/**
 *  @def  sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG
 *        Control Info -- 1: indicate selection of Air Cipher engine in data mode
 *                        0: use Encryption engine (default)
 *  @note: There are Encryption Engine and Air Cipher Engine in SASS. There are
 *  certain types of algorithms such as AES_CTR which both of these engines are
 *  capable of executing. By default the data mode selects to use the encryption
 *  engine for the algorithms such as AES_CTR. To seperate the Air Cipher traffic
 *  from the IPSec traffic, application may select the air cipher engine
 *  for the encryption by setting this bit.
 *
 *  For devices such as K2G (NSS_LITE) which do not have air cipher engine
 *  setting this bit would cause error during datamode channel control configuration.
 *
 *  This bit is supported for SA LLD releases version 3.0.0.15 or higher
 */
#define sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG  ((uint16_t) (0x0001U))

/**
 *  @def  sa_DM_CONFIG_PROMOTE_CHANNEL
 *        Control Info -- 1: promotion of normal world packet to secure packet
 *                           intended to land in the secure memory (only for sa2ul)
 *                        0: no promotion
 *  @note: Promotion of non-secure packet to be secure packet also always requires
 *         the 48-bit SCPTR that comes with the packet to be within the range
 *         of 'SCPTR Promote Range' registers.
 *
 *  For devices that do not have SA2UL setting this bit would cause no action
 *
 */
#define sa_DM_CONFIG_PROMOTE_CHANNEL  ((uint16_t) (0x0002U))

/**
 *  @def  sa_DM_CONFIG_DEMOTE_CHANNEL
 *        Control Info -- 1: demotion of secure packet to normal world packet
 *                           intended to land in the secure memory (only for sa2ul)
 *                        0: no demotion
 * 
 *  For devices that do not have SA2UL setting this bit would cause no action
 *
 */
#define sa_DM_CONFIG_DEMOTE_CHANNEL  ((uint16_t) (0x0004U))

/**
 *  @def  sa_DM_CONFIG_USE_SECURE_CTX_FOR_NON_SECURE_CHANNEL
 *        Control Info -- 1: a normal world packet may use secure context
 *                        0: non-secure packet for this context can't use secure context
 * 
 *  For devices that do not have SA2UL setting this bit would cause no action
 *
 */
#define sa_DM_CONFIG_USE_SECURE_CTX_FOR_NON_SECURE_CHANNEL  ((uint16_t) (0x0008U))

/**
 *  @def  sa_DM_CONFIG_USE_DKEK
 *        Control Info -- 1: Set the USE_DKEK flag in the security context so
 *                           that DKEK programmed by DMSC is loaded in-band
 *                           instead of user-supplied key
 *                        0: Do not set USE_DKEK flag. User supplies a key
 *                           directly.
 *
 *  For devices that do not have SA2UL setting this bit would cause no action
 *
 */
#define sa_DM_CONFIG_USE_DKEK  ((uint16_t) (0x0010U))


/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief Data Mode Configuration Parameters structure
 *
 * Data structure defines the Data Mode specific configuration parameters excluding the keys.
 *
 * Due to hardware limiations in IP, below restrictions apply :
 * 
 @verbatim
  ---------------------------------------------------------------------------
  | Cipher Mode              | Restriction                                  |
  ---------------------------------------------------------------------------
  | - sa_CipherMode_CCM      | - (IV Size + Salt Size) should not exceed 13 | 
  |                          | - AAD size should not exceed 14              |
  |                          | - authentication mode should be null     |
  ---------------------------------------------------------------------------
  | - sa_CipherMode_GCM      | - (IV Size + Salt Size) should be 12         | 
  |                          | - AAD size should not exceed 16              |
  |                          | - authentication mode should be null     |
  ---------------------------------------------------------------------------
  | - sa_CipherMode_DES_CBC  | - (IV Size should be 8                       |
  |                          | - Salt size should be 0                      | 
  ---------------------------------------------------------------------------  
  | - sa_CipherMode_3DES_CBC | - (IV Size should be 8                       |
  |                          | - Salt size should be 0                      | 
  ---------------------------------------------------------------------------  
  | - sa_CipherMode_KASUMI_F8| - (IV Size should be 8                       |
  |                          |                                              |
  ---------------------------------------------------------------------------  
  | - sa_CipherMode_SNOW3G_F8| - (IV Size should be 16                      |
  |                          |                                              |
  --------------------------------------------------------------------------- 

  ---------------------------------------------------------------------------
  | Auth Mode                | Restriction                                  |
  ---------------------------------------------------------------------------
  | - sa_AuthMode_GMAC       | - (IV Size + Salt Size) should be 12         | 
  |                          | - AAD size should not exceed 16              |
  |                          | - cipher mode should be null             |
  ---------------------------------------------------------------------------
  | - sa_AuthMode_GMAC_AH    | - (IV Size should be 8                       |
  |                          | - Salt size should be 4                      | 
  |                          | - cipher mode should be null             |
  ---------------------------------------------------------------------------  
  | - sa_AuthMode_KASUMI_F9  | - (IV Size should be 8                       |
  |                          |                                              | 
  ---------------------------------------------------------------------------
 @endverbatim
 */
typedef struct {
  uint16_t    ctrlBitMap;         /**< Various control information as specified at @ref SALLDDmConfigCtrlBit */
  uint16_t    sessionEncKeySize;  /**< Specify the size of the session encryption key in bytes */  
  uint16_t    sessionMacKeySize;  /**< Specify the size of the session mac key in bytes */  
  uint16_t    sessionSaltSize;    /**< Specify the size of the session salt used in the GCM/CCM operation in bytes */  
  uint16_t    ivSize;             /**< Specify the size of the initialization vector in bytes */  
  uint16_t    macSize;            /**< Specify the size of the authentication tag in bytes */  
  uint16_t    aadSize;            /**< Specify the size of the additional authenticated data in bytes used in CCM and GCM modes */
  uint16_t    enc;                /**< TRUE: Encryption(To-Air); FALSE: Decryption (From-Air)*/
  uint16_t    enc1st;             /**< TRUE: Perform encryption first; FALSE: Perform authentication first */
#if defined(NSS_LITE2)
  uint8_t     priv;               /**< Specify the priv for the security context creation */
  uint8_t     privId;             /**< Specify the priv ID for the security context to checking with incoming packets to match */
#endif
} Sa_DataModeConfigParams_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD Protocol-specfic Configuration Parameters structure
 *
 * Data structure defines the security protocol specific configuration parameters
 *
 */
typedef union {
  Sa_SrtpConfigParams_t      srtp;       /**< Specify the configuration parameters for SRTP */
  Sa_IpsecConfigParams_t     ipsec;      /**< Specify the configuration parameters for IPSEC */
  Sa_AcConfigParams_t        ac;         /**< Specify the configuration parameters for Air Ciphering */
  Sa_DataModeConfigParams_t  data;       /**< Specify the configuration parameters for Data Mode */
} Sa_ProtocolConfigParams_t;   

/**
 *  @defgroup SALLDDestInfoCtrlBit  SALLD Destination Info Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD Destination Info Control Bit Definitions
 *  Bitmap definition of the ctrlBitfield in Sa_DestInfo_t
 *  It allows selective enabling/disabling of specific SALLD operations
 *  
 */ 
/*@{*/
/**
 *  @def  sa_DEST_INFO_CTRL_USE_LOC_DMA
 *        Control Info -- Set: Use Local DMA
 *                        Clear: Use Global DMA (defAult)
 *
 *  There are Network Sub-System (NSS) internal DMA resources including CPPI and QMSS which can be used to transfer packets within 
 *  the NSS such as	from PASS to SASS and vica versa. This control bit is used to enable local DMA tranfer from SASS to PASS. 
 *
 *  @note: This feature is only supported at the second generation NSS devices.
 */
#define sa_DEST_INFO_CTRL_USE_LOC_DMA        0x01 
/*@}*/
/** @} */


/**
 * @ingroup salld_api_structures
 * @brief SALLD Destination Info structure
 *
 * Data structure defines the destination parameters which are used
 * by SASS to deliver the processed packets to the desired destination
 *
 */
typedef struct {
  uint8_t     ctrlBitfield; /**< Control Bit Map to specify destination related operations such as local DMA 
                                         as defined at @ref SALLDDestInfoCtrlBit */ 

  uint16_t    flowID;     /**< Specify the CPPI Flow ID */
  uint16_t    queueID;    /**< Specify the 16-bit destination Queue ID (OR Ring ID for SA2UL) */
  uint32_t    swInfo0;    /**< User-defined channel-specific parameter which will be placed in SwInfo0 for packets from SA */
  uint32_t    swInfo1;    /**< User-defined channel-specific parameter which will be placed in SwInfo1 for packets from SA */
} Sa_DestInfo_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD General Configuration Parameters structure
 *
 * Data structure defines the general configuration parameters excluding the keys and related 
 * re-key parameters
 *
 */
typedef struct {
  Sa_CipherMode_e cipherMode;         /**< Specify the cipher mode as defined at @ref CipherModes */
  Sa_AuthMode_e   authMode;           /**< Specify the authentication mode as defined at @ref AuthModes */
  Sa_DestInfo_t   destInfo;           /**< Specify the post-SA destination information */
  Sa_ProtocolConfigParams_t   params; /**< Specify the protocol-specific configuration parameters */
} Sa_GenConfigParams_t;

/**
 *  @defgroup SALLDCtrlInfoValidBit  SALLD General Control Info Valid Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD General Control Info Valid Bit Definitions
 *  Bitmap definition of the validBitfield in Sa_GenCtrlInfo_t. 
 *  It allows selective control parameters
 */ 
/*@{*/
/**
 *  @def  sa_CONTROLINFO_VALID_CTRL_BITMAP
 *        Control Info -- ctrlBitfield 
 */
#define sa_CONTROLINFO_VALID_CTRL_BITMAP     0x0001 
/**
 *  @def  sa_CONTROLINFO_VALID_TX_CTRL
 *        Control Info -- txCtrl
 */
#define sa_CONTROLINFO_VALID_TX_CTRL         0x0002 
/**
 *  @def  sa_CONTROLINFO_VALID_RX_CTRL
 *        Control Info -- rtxCtrl
 */
#define sa_CONTROLINFO_VALID_RX_CTRL         0x0004 
/**
 *  @def  sa_CONTROLINFO_VALID_REPLAY_WIN
 *        Control Info -- replayWindowSize 
 */
#define sa_CONTROLINFO_VALID_REPLAY_WIN      0x0008 
/*@}*/
/** @} */

/**
 *  @defgroup SALLDCtrlInfoCtrlBit  SALLD General Control Info Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD General Control Info Control Bit Definitions
 *  Bitmap definition of the ctrlBitfield in Sa_GenCtrlInfo_t. 
 *  It allows selective enabling/disabling of specific SALLD operations
 *  
 */ 
/*@{*/
/**
 *  @def  sa_CONTROLINFO_CTRL_TX_ON
 *        Control Info -- Enable Tx
 */
#define sa_CONTROLINFO_CTRL_TX_ON            0x0001 
/**
 *  @def  sa_CONTROLINFO_CTRL_RX_ON
 *        Control Info -- Enable Rx
 */
#define sa_CONTROLINFO_CTRL_RX_ON            0x0002 
/**
 *  @def  sa_CONTROLINFO_CTRL_TX_RX_MASK
 *        Control Info -- Tx/Rx Control 
 *                        Bit Mask 
 */
#define sa_CONTROLINFO_CTRL_TX_RX_MASK       0x0003 
/**
 *  @def  sa_CONTROLINFO_CTRL_SW_ONLY
 *        Control Info -- Software support only.
 *        It is used for the protocols which are not supported by the SA 
 *        firmware such as SRTCP.                
 */
#define sa_CONTROLINFO_CTRL_SW_ONLY          0x0008 
/**
 *  @def  sa_CONTROLINFO_CTRL_OP_MASK
 *        Control Info -- Operation Control 
 *                        Bit Mask 
 */
#define sa_CONTROLINFO_CTRL_OP_MASK          0x000b 
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SALLD General Control Information structure
 *
 * Data structure defines the general control information of the security channel excluding the keys 
 * and related re-key parameters
 *
 */
typedef struct {
  uint16_t validBitfield;           /**< Specify valid parameters as defined at @ref SALLDCtrlInfoValidBit */
  uint16_t ctrlBitfield;            /**< Control Bit Map to specify tx/rx and other operations 
                                         as defined at @ref SALLDCtrlInfoCtrlBit */ 
  Sa_GenConfigParams_t txCtrl;      /**< Specify the general configuration parameters in Tx
                                         (to-network) direction */  
  Sa_GenConfigParams_t rxCtrl;      /**< Specify the general configuration parameters in Rx
                                         (from-network) direction */
  uint16_t replayWindowSize;        /**< Specify the size of the replay window
                                         (SRTP 64 IPSEC 64:128) */  
} Sa_GenCtrlInfo_t;

/**
 *  @defgroup SALLDSrtpKeyCtrlInfo  SALLD SRTP Key Control Info Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SRTP Key Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in Sa_SrtpKeyParams_t. 
 */ 
/*@{*/
/**
 *  @def  sa_SRTP_KEY_CTRL_MASTER_KEY
 *        Control Info -- Set: MASTER_KEY available
 */
#define sa_SRTP_KEY_CTRL_MASTER_KEY        0x0001 
/**
 *  @def  sa_SRTP_KEY_CTRL_MASTER_SALT
 *        Control Info -- Set: MASTER_SALT available
 */
#define sa_SRTP_KEY_CTRL_MASTER_SALT       0x0002 
/**
 *  @def  sa_SRTP_KEY_CTRL_KEY_DERIVE_RATE
 *        Control Info -- Set: key derivation rate available
 */
#define sa_SRTP_KEY_CTRL_KEY_DERIVE_RATE   0x0004 
/**
 *  @def  sa_SRTP_KEY_CTRL_KEY_LIFETIME
 *        Control Info -- Set: key lifetime available
 */
#define sa_SRTP_KEY_CTRL_KEY_LIFETIME      0x0008 
/**
 *  @def  sa_SRTP_KEY_CTRL_ROC
 *        Control Info -- Set: Initial ROC is available
 */
#define sa_SRTP_KEY_CTRL_ROC               0x0010 

/**
 *  @def  sa_SRTP_KEY_CTRL_MKI
 *        Control Info -- Set: MKI available
 */
#define sa_SRTP_KEY_CTRL_MKI               0x0020 
/**
 *  @def  sa_SRTP_KEY_CTRL_KEY_TYPE_FROM_TO
 *        Control Info -- Set: From To Key 
 *                        Clear: MKI Key
 */
#define sa_SRTP_KEY_CTRL_KEY_TYPE_FROM_TO  0x0040 
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SRTP Key Information structure
 *
 * Data structure defines the SRTP Key related parameters
 *
 */
typedef struct {
  uint16_t    ctrlBitfield;    /**< Specify the Key types and other control information as defined at @ref SALLDSrtpKeyCtrlInfo */
  uint8_t*    masterKey;       /**< Specify the master key */
  uint8_t*    masterSalt;      /**< Specify the master Salt */
  uint16_t    derivRate;       /**< Specify the key derivation rate in n of 2^n format (where 0 <= n <= 24) 
                                    if not set then use default value, i.e no key derivation */
  uint16_t    keyLifeTimeMsw;  /**< Specify the maximum number of packets allowed for the master
                                  key combination */ 
  uint32_t    keyLifeTimeLsw;  /**< Specify the maximum number of packets allowed for the master
                                key combination */ 
  uint32_t    roc;             /**< SRTP: Specify the initial value of the rollover counter 
                                    SRTCP: Specify the initial index at the tx packet */
  uint32_t    fromEsnMsw;      /**< Specify the starting 48-bit extended sequence number
                                of the specified From-To keys */
  uint16_t    fromEsnLsw;      /**< Specify the starting 48-bit extended sequence number
                                of the specified From-To keys */
  uint32_t    toEsnMsw;        /**< Specify the last 48-bit extended sequence number
                                of the specified From-To keys */
  uint16_t    toEsnLsw;        /**< Specify the last 48-bit extended sequence number
                                of the specified From-To keys */
  uint16_t    mkiSize;         /**< Specify the size of the MKI in bytes */
  uint32_t    mki;             /**< Specify the MKI value */                              
} Sa_SrtpKeyParams_t;

/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maximum key size supported by SASS 
 */
#if defined(NSS_LITE2)
#define SALLD_MAX_KEY_SIZE      64
#else
#define SALLD_MAX_KEY_SIZE      32
#endif

/**
 *  @defgroup SALLDIpsecKeyCtrlInfo  SALLD IPSEC Key Control Info Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name IPSEC Key Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in Sa_IpsecKeyParams_t. 
 */ 
/*@{*/
/**
 *  @def  sa_IPSEC_KEY_CTRL_ENC_KEY
 *        Control Info -- Set: ENC_KEY available
 */
#define sa_IPSEC_KEY_CTRL_ENC_KEY          0x0001 
/**
 *  @def  sa_IPSEC_KEY_CTRL_MAC_KEY
 *        Control Info -- Set: MAC_KEY available
 */
#define sa_IPSEC_KEY_CTRL_MAC_KEY          0x0002 
/**
 *  @def  sa_IPSEC_KEY_CTRL_SALT
 *        Control Info -- Set: Session Salt available
 */
#define sa_IPSEC_KEY_CTRL_SALT             0x0004 
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief IPSEC Key Information structure
 *
 * Data structure defines the IPSEC Key related parameters
 *
 */
typedef struct {
  uint16_t       ctrlBitfield;    /**< Specify the Key types and other control Information as defined at @ref SALLDIpsecKeyCtrlInfo */
  uint8_t*       sessionEncKey;   /**< Specify the session encryption key */
  uint8_t*       sessionAuthKey;  /**< Specify the session authentication key */
  uint8_t*       sessionSalt;     /**< Specify the session salt */
} Sa_IpsecKeyParams_t;

/**
 *  @defgroup SALLDAcKeyCtrlInfo  SALLD AC Key Control Info Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Air Ciphering Key Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in Sa_AcKeyParams_t. 
 */ 
/*@{*/
/**
 *  @def  sa_AC_KEY_CTRL_ENC_KEY
 *        Control Info -- Set: ENC_KEY available
 */
#define sa_AC_KEY_CTRL_ENC_KEY          0x0001 
/**
 *  @def  sa_AC_KEY_CTRL_MAC_KEY
 *        Control Info -- Set: MAC_KEY available
 */
#define sa_AC_KEY_CTRL_MAC_KEY          0x0002 
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief Air Ciphering Key Information structure
 *
 * Data structure defines the Air Ciphering Key related parameters
 *
 */
typedef struct {
  uint16_t     ctrlBitfield;    /**< Specify the Key types and other control Information as defined at @ref SALLDAcKeyCtrlInfo */
  uint8_t*     sessionEncKey;   /**< Specify the session encryption key */
  uint8_t*     sessionAuthKey;  /**< Specify the session authentication key */
} Sa_AcKeyParams_t;

/**
 *  @defgroup SALLDDataModeKeyCtrlInfo  SALLD Data Mode Key Control Info Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Data Mode Key Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in Sa_DataModeKeyParams_t. 
 */ 
/*@{*/
/**
 *  @def  sa_DATA_MODE_KEY_CTRL_ENC_KEY
 *        Control Info -- Set: ENC_KEY available
 */
#define sa_DATA_MODE_KEY_CTRL_ENC_KEY          0x0001 
/**
 *  @def  sa_DATA_MODE_KEY_CTRL_MAC_KEY
 *        Control Info -- Set: MAC_KEY available
 */
#define sa_DATA_MODE_KEY_CTRL_MAC_KEY          0x0002 
/**
 *  @def  sa_DATA_MODE_KEY_CTRL_SALT
 *        Control Info -- Set: SALT available
 */
#define sa_DATA_MODE_KEY_CTRL_SALT             0x0004 
/**
 *  @def  sa_DATA_MODE_KEY_USE_DKEK
 *        Control Info -- Set: USE_DKEK field in security context
 */
#define sa_DATA_MODE_KEY_USE_DKEK              0x0008
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief Data Mode Key Information structure
 *
 * Data structure defines the Data Mode Key related parameters
 *
 */
typedef struct {
  uint16_t     ctrlBitfield;    /**< Specify the Key types and other control Information as defined at @ref SALLDDataModeKeyCtrlInfo */
  uint8_t*     sessionEncKey;   /**< Specify the session encryption key */
  uint8_t*     sessionAuthKey;  /**< Specify the session authentication key */
  uint8_t*     sessionSalt;     /**< Specify the session salt in GCM/CCM only */
} Sa_DataModeKeyParams_t;

/**
 * @ingroup salld_api_structures
 * @brief Protocol-Specific Key Information structure
 *
 * Data structure defines the protocol-specific Key related parameters
 *
 */
typedef union {
 Sa_SrtpKeyParams_t       srtp;    /**< Specify the key related parameters for SRTP/SRTCP */
 Sa_IpsecKeyParams_t      ipsec;   /**< Specify the key related parameters for IPSEC */
 Sa_AcKeyParams_t         ac;      /**< Specify the key related parameters for 3GPP Air Ciphering */
 Sa_DataModeKeyParams_t   data;    /**< Specify the key related parameters in Data Mode */
} Sa_ProtocolKeyParams_t; 

/**
 *  @defgroup SALLDKeyCtrlInfo SALLD Key Control Info Control Bit Definitions
 *  @ingroup salld_api_constants

 *  @{
 *
 *  @name SALLD Key Control Info Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in Sa_KeyCtrlInfo_t. 
 */ 
/*@{*/
/**
 *  @def  sa_KEY_CONTROL_TX_KEY_VALID
 *        Control Info -- TX key is valid
 */
#define sa_KEY_CONTROL_TX_KEY_VALID           0x0001 
/**
 *  @def  sa_KEY_CONTROL_RX_KEY_VALID
 *        Control Info -- RX key is valid
 */
#define sa_KEY_CONTROL_RX_KEY_VALID           0x0002 
/* @} */ /* ingroup */
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SALLD Key Control Information structure
 *
 * Data structure defines the Key related information of the security channel.
 * It is used for initial channel configuration and re-key operation.
 *
 */
typedef struct {
  uint16_t ctrlBitfield;           /**< Bit Map to specify tx/rx and other operations as 
                                        defined at @ref SALLDKeyCtrlInfo */ 
  Sa_ProtocolKeyParams_t txKey;    /**< Specify the security key parameters in Tx
                                       (to-network) direction */  
  Sa_ProtocolKeyParams_t rxKey;    /**< Specify the security key parameters in Rx
                                       (from-network) direction */
} Sa_KeyCtrlInfo_t;

/**
 *  @defgroup salldChanCtrlType SALLD Channel Control Type
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Channel Control Type
 *  Definition of channel control types supported by SA LLD 
 *  Parameter ctrlType of Sa_ChanCtrlInfo_t should be set to
 *  one of these types.
 *
 */
/*@{*/
typedef enum {
  sa_CHAN_CTRL_GEN_CONFIG = 1,   /**<  Security Channel General Configuration */
  sa_CHAN_CTRL_KEY_CONFIG = 2    /**<  Security Channel Key Configuration */ 
} Sa_ChanControlCode_e; 
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief Channel Control Information structure
 *
 * Data structure providing control information in Sa_chanControl()
 *
 */
typedef struct {
  uint16_t    ctrlType;    /**< Specify the channel control type as defined at @ref Sa_ChanControlCode_e */
  union {
    Sa_GenCtrlInfo_t  gen; /**< Specify the general control information */
    Sa_KeyCtrlInfo_t  key; /**< Specify the key control information */ 
  }ctrlInfo;                 /**< Contain the control-type specific control information */

} Sa_ChanCtrlInfo_t;

/**
 *  @defgroup PktErrorCodes SALLD PKT Error Codes 
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD PKT Error Codes
 *
 *  Definitions of the error codes for the packets received from SA
 */
/*@{*/
/**
 *  @def  sa_PKT_ERR_OK
 *        SALLD Packet Error code -- No Error.
 */
#define sa_PKT_ERR_OK                0 
/**
 *  @def  sa_PKT_ERR_REPLAY_OLD
 *        SALLD Packet Error code -- Packet rejected by SA because it is out of replay window range.
 */
#define sa_PKT_ERR_REPLAY_OLD        1
/**
 *  @def  sa_PKT_ERR_REPLAY_DUP
 *        SALLD Packet Error code -- Packet rejected by SA because it is duplicated.
 */
#define sa_PKT_ERR_REPLAY_DUP        2  
/**
 *  @def  sa_PKT_ERR_INVALID_KEY
 *        SALLD Packet Error code -- Packet rejected by SA because the key in the security
 *                                   context is out of date. 
 */
#define sa_PKT_ERR_INVALID_KEY       3  
/**
 *  @def  sa_PKT_ERR_INVALID_MKI
 *        SALLD Packet Error code -- Packet rejected by SA because the MKI in the security
 *                                   context does not match the one in the packet. 
 */
#define sa_PKT_ERR_INVALID_MKI       4  
/**
 *  @def  sa_PKT_ERR_AUTH_FAIL
 *        SALLD Packet Error code -- Packet rejected by SA because authentication failed.
 */
#define sa_PKT_ERR_AUTH_FAIL         5
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SA Packet Error data type
 *
 * Data type defines the packet error type of all packets to be delivered to SA. 
 *
 */
typedef uint16_t  Sa_PktErr_t;

/**
 *  @defgroup SaPktDir SA Packet Directions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SA Packet Directions
 *
 *  Definition of SA Packet Directions used at API @ref Sa_chanGetSwInfo
 */ 
/*@{*/
typedef enum {
  sa_PKT_DIR_FROM_NETWORK = 0,   /**< Form-network (To-air) packet */
  sa_PKT_DIR_TO_NETWORK         /**< To-network (From-air) packet */
} Sa_PktDir_t;
/*@}*/
/** @} */

/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maxmium number of software info parameters at @ref Sa_SWInfo_t. 
 */
#define sa_MAX_SW_INFO_SIZE          3
 
/**
 * @ingroup salld_api_structures
 * @brief SA Software Information structure
 *
 * Data structure defines the SA-specific software information required for all packets to 
 * be delivered to SA. It will be provided by the SA LLD based on the channel configuration 
 * parameters and the security protocols.The software information words should be copied
 * to the CPPI Software words area as provided through the CPPI LLD or equivalent component.
 *
 * for SA2_UL (Ultra Lite Generation 2) devices, size is always 4 and the swInfo[3] would 
 * hold the first 32 bits of Protocol Specific data of CPPI
 */
typedef struct {
  uint16_t  size;   /**< Specify the software info size in 32-bit words */
  uint32_t  swInfo[sa_MAX_SW_INFO_SIZE]; /**< Specify the software information for SA */
} Sa_SWInfo_t;


/**
 *  @ingroup salld_api_macros
 *  @brief  sa_SWINFO_UPDATE_DEST_INFO is used to update the destination information within swInfo[2] at @ref Sa_SWInfo_t
 *
 *  @note   this macro is not applicable for socs such as AM65XX or J721E that have second generation SA Ultra Lite (SA2_UL)
 *
 *  @details  The application may want to deliver output packets to different queues for load balance.
 *            This macro is used to update the destination queue and CPPI flow number in the Sa_SWInfo_t data structure 
 *            provided by Sa_chanSendData()
 *
 */
#define sa_SWINFO_UPDATE_DEST_INFO(info, queueID, flowIndex)                               \
{                                                                                          \
    (info[0]) |= 0x40000000L;                                                              \
    (info[2])  = ((queueID)) | (((flowIndex) & 0xFF) << 16) | ((info[2]) & 0xFF000000L);   \
}                                                                                          \

/**
 * @ingroup salld_api_structures
 * @brief SA Command Label Parameter Information structure
 *
 * Data structure defines the updating information of parameters which may be updated within the 
 * command label per packet. 
 *
 */

typedef struct {
  uint8_t index;   /**< Specify the index of the starting 32-bit command word */
  uint8_t offset;  /**< specify the offset to the parameter within the command word */  
  uint8_t size;    /**< Specify the size of parameter in bytes */

} Sa_CmdLbParamInfo_t;

/**
 *  @defgroup DataSubModes Data Sub Operation Modes
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Data Sub Operation Modes
 *
 *  Definition of the sub operation modes in Data mode.
 *  Enhanced the sub modes to more general types of protocols such as WiMax.
 *  Pleae refer to @ref Sa_DataModeConfigParams_t restrictions table
 *  on supported IV, Salt and AAD sizes
 *  
 */ 
/*@{*/
typedef enum {
  sa_DM_GEN = 0,     /**< General operation mode */
  sa_DM_CCM,         /**< CCM IPSEC */
  sa_DM_CCM_GEN,     /**< CCM non IPSEC */
  sa_DM_GCM,         /**< GCM IPSEC */
  sa_DM_GCM_GEN,     /**< GCM non IPSEC */
  sa_DM_GMAC,        /**< GMAC IPSec */
  sa_DM_GMAC_GEN,    /**< GMAC non IPSec*/
  sa_DM_GMAC_AH      /**< GMAC for IPSEC AH */
} Sa_DM_subMode_e;
/*@}*/
/** @} */


/**
 *  @defgroup SALLDCmdLbUpdateVaildInfo  SALLD Command Label Update Vaild Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Command Label Update Valid Bit Definitions
 *
 *  Bitmap definition of the validBitfield in Sa_CmdLbUpdateInfo_t. 
 */ 
/*@{*/
/**
 *  @def  sa_CMDL_UPDATE_VALID_ENC
 *        Control Info -- Encryption related information is valid
 */
#define sa_CMDL_UPDATE_VALID_ENC          0x0001 
/**
 *  @def  sa_CMDL_UPDATE_VALID_AUTH
 *        Control Info -- Authentication related information is valid
 */
#define sa_CMDL_UPDATE_VALID_AUTH         0x0002 
/**
 *  @def  sa_CMDL_UPDATE_VALID_ENC_IV
 *        Control Info -- Encryption IV is valid
 */
#define sa_CMDL_UPDATE_VALID_ENC_IV       0x0004 
/**
 *  @def  sa_CMDL_UPDATE_VALID_AUTH_IV
 *        Control Info -- Authentication IV valid
 */
#define sa_CMDL_UPDATE_VALID_AUTH_IV      0x0008 
/**
 *  @def  sa_CMDL_UPDATE_VALID_AAD
 *        Control Info -- AAD is valid
 */
#define sa_CMDL_UPDATE_VALID_AAD          0x0010 
/**
 *  @def  sa_CMDL_UPDATE_VALID_AUX_KEY
 *        Control Info -- CMAC-type Auxiliary key is valid
 */
#define sa_CMDL_UPDATE_VALID_AUX_KEY      0x0020 
/**
 *  @def  sa_CMDL_UPDATE_VALID_PAYLOAD
 *        Control Info -- Payload pending is valid such as GMAC mode
 */
#define sa_CMDL_UPDATE_VALID_PAYLOAD      0x0040 
/**
 *  @def  sa_CMDL_UPDATE_VALID_ENC_SIZE
 *        Control Info -- Second encryption size update is required, such as CCM mode
 */
#define sa_CMDL_UPDATE_VALID_ENC_SIZE     0x0080 
/**
 *  @def  sa_CMDL_UPDATE_VALID_ENC_IV2
 *        Control Info -- Second encryption IV update is required, such as CCM mode
 */
#define sa_CMDL_UPDATE_VALID_ENC_IV2      0x0100 
/**
 *  @def  sa_CMDL_UPDATE_VALID_AUTH_SIZE
 *        Control Info -- Second authentication size update is required, such as GMAC mode
 */
#define sa_CMDL_UPDATE_VALID_AUTH_SIZE    0x0200 

/*@}*/
/** @} */


/**
 * @ingroup salld_api_structures
 * @brief SA Command Label Updating Information structure
 *
 * Data structure provides the information how to update the command label. The application can
 * use this information to update the command label per packet without invoking Sa_chanSendData() API.
 *
 */

typedef struct {
  uint16_t  validBitfield;    /**< Specify the parameter valid Information as defined at @ref SALLDCmdLbUpdateVaildInfo */
  uint16_t  subMode;          /**< Specify the sub operation modes as defined at @ref DataSubModes */
  Sa_CmdLbParamInfo_t  encSizeInfo;    /**< Information used to update encryption size */
  Sa_CmdLbParamInfo_t  encSizeInfo2;   /**< Information used to update 2nd encryption size */
  Sa_CmdLbParamInfo_t  encOffsetInfo;  /**< Information used to update encryption offset */
  Sa_CmdLbParamInfo_t  encIvInfo;      /**< Information used to update encryption initialization vector */
  Sa_CmdLbParamInfo_t  encIvInfo2;     /**< Information used to update 2nd encryption initialization vector */
  Sa_CmdLbParamInfo_t  aadInfo;        /**< Information used to update AAD  */
  Sa_CmdLbParamInfo_t  payloadInfo;    /**< Information used to update payload padding information */
  Sa_CmdLbParamInfo_t  authSizeInfo;   /**< Information used to update authentication size */
  Sa_CmdLbParamInfo_t  authSizeInfo2;  /**< Information used to update 2nd authentication size */
  Sa_CmdLbParamInfo_t  authOffsetInfo; /**< Information used to update authentication offset */
  Sa_CmdLbParamInfo_t  authIvInfo;     /**< Information used to update authentication initialization vector */
  Sa_CmdLbParamInfo_t  auxKeyInfo;     /**< Information used to update auxiliary Key information */
  uint32_t             auxKey[8];      /**< Contain auxiliary key1/key2 */ 
} Sa_CmdLbUpdateInfo_t;

/**
 *  @ingroup salld_api_constants
 *   Note: For NSS_LITE2 the effective command label size is 4 bytes less than the maximum size
 *  @brief   Define the maxmium size of the command label stored at cmdLbBuf of @ref Sa_CmdLabelInfo_t.
 */
#if defined (NSS_LITE2)
#define sa_MAX_CMDLB_SIZE           ((uint32_t)(100U))
#else
#define sa_MAX_CMDLB_SIZE           ((uint32_t)(96U))
#endif
/**
 * @ingroup salld_api_structures
 * @brief SA Command Label Information structure
 *
 * Data structure defines the SA-specific command label information used by SA to route packet 
 * to sub-engines within SA. The command label information will be formatted by the SA LLD
 * based on the channel configuration parameters and the payload information as defined at 
 * @ref Sa_PayloadInfo_t. The command label should be copied to the protocol-specific section 
 * at the CPPI packet descriptor as provided through the CPPI LLD or equivalent component. 
 *
 * @note: This structure is only used in Data Mode and the cmdLbBuf should be provided by the
 *       caller.
 */
typedef struct {
  uint16_t  cmdLbSize;   /**< Specify the command label size in bytes */
  uint8_t*  cmdLbBuf;    /**< Pointer to the buffer which contains the command labels 
                              The buffer should be allocated by the caller in Data
                              Mode opeation and 4-byte alignment is required*/
  Sa_CmdLbUpdateInfo_t* cmdLbUpdateInfo; /**< Pointer to command label updating data structure, the command 
                                              label updating information will be provided upon return if not 
                                              NLL pointer */                            
} Sa_CmdLabelInfo_t;

/**
 * @ingroup salld_api_structures
 * @brief SA Payload Information structure
 *
 * Data structure providing the payload related information used to construct the 
 * SA-specific command labels in data mode
 */
typedef struct {
  uint8_t   encOffset;   /**< Specify the offset to the encrypted/decrypted data in the packet in bytes */
  uint8_t   authOffset;  /**< Specify the offset to the authenticated data in the packet in bytes */
  uint16_t  encSize;     /**< Specify the total number of bytes to be encrypted or decrypted */ 
  uint16_t  authSize;    /**< Specify the total number of bytes to be authenticated */ 
  uint8_t*  encIV;       /**< Contain the initialization vectors used in certain encryption modes. 
                            @note: IV should be specified here in GCM/GMAC/CCM mode */
  uint8_t*  authIV;      /**< Contain the initialization vectors used in certain authentication modes.*/
  uint8_t*  aad;         /**< Contain the additional authenticated data in GCM/GMAC/CCM modes */ 
} Sa_PayloadInfo_t;

/**
 * @ingroup salld_api_structures
 * @brief SA Rx Payload Information structure
 *
 * Data structure providing the miscellaneous header information of the received packets which is 
 * required for the IPSEC post-processing function to patch the outer IP header when the inner IP 
 * fragments have been reassembled by the hardware IP reassembly engine (RA) on the NSS Gen2 devices.
 *
 * @note: This structure should be provided at the Sa_chanReceiveData() call when the RA engine is enabled
 *        for inner IP. If it is not provided, the IPSEC post-processing function will not be able to handle
 *        inner IP reassembled packets correctly. 
 */
typedef struct {
  uint8_t   ipOffset;    /**< Specify the offset to the outer IP in the packet in bytes */
  uint8_t   ipOffset2;   /**< Specify the offset to the inner IP in the packet in bytes 
                              (0: indicates there is no inner IP) */
} Sa_RxPayloadInfo_t;


/**
 * @ingroup salld_api_structures
 * @brief SA IPSEC NAT-T structure
 *
 * Data structure defines the IPSEC NAT-T parameters used by @ref Sa_chanSendData() API to
 * construct the IPSEC NAT-T (UDP) header which resides in front of IPSEC ESP header.
 */
typedef struct {
  uint16_t  dstPort;   /**< Specify the destination UDP port number */
  uint16_t  srcPort;   /**< Specify the source UDP port number */
} Sa_ipsecNatTInfo_t;

/**
 *  @defgroup SALLDPktInfoValidBits  SALLD Packet Info Valid Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD Packet Info Valid Bit Definitions
 *
 *  Bitmap definition of the validBitMap in Sa_PktInfo_t. 
 */ 
/*@{*/
/**
 *  @def  sa_PKT_INFO_VALID_PKT_ERR_CODE
 *        - pktErrorCode is present
 */
#define sa_PKT_INFO_VALID_PKT_ERR_CODE           0x0001 
/**
 *  @def  sa_PKT_INFO_VALID_SW_INFO
 *        - swInfo is present
 */
#define sa_PKT_INFO_VALID_SW_INFO                0x0002 
/**
 *  @def  sa_PKT_INFO_VALID_CMDLB_INFO
 *        - cmdlb is present
 */
#define sa_PKT_INFO_VALID_CMDLB_INFO             0x0004 
/**
 *  @def  sa_PKT_INFO_VALID_PAYLOAD_INFO
 *        - payloadInfo is present
 */
#define sa_PKT_INFO_VALID_PAYLOAD_INFO           0x0008 
/**
 *  @def  sa_PKT_INFO_VALID_IPSEC_NAT_T_INFO
 *        - natTInfo is present
 */
#define sa_PKT_INFO_VALID_IPSEC_NAT_T_INFO       0x0010 
/**
 *  @def  sa_PKT_INFO_VALID_RX_PAYLOAD_INFO
 *        - rxPayloadInfo is present
 */
#define sa_PKT_INFO_VALID_RX_PAYLOAD_INFO        0x0020 


/*@}*/
/** @} */

/**
*  @defgroup SALLDEspInfoTxRxValidBits SALLD ESP TxRx Info Valid Bit Definitions
*  @ingroup salld_api_constants
*  @{
*
*  @name SALLD ESP TxRx Valid Bit Definitions
*
*  Bitmap definition of the validBitMap in salldIpsecEspTxRxInfo_t. 
 */ 
/*@{*/
/**
*  @def  sa_ESP_TXRX_VALID_IPSEC_NAT_T_INFO      
 *        - espTxRx_natTInfo is present
*/
#define sa_ESP_TXRX_VALID_IPSEC_NAT_T_INFO      sa_PKT_INFO_VALID_IPSEC_NAT_T_INFO
/**
 *  @def sa_ESP_TXRX_VALID_RX_PAYLOAD_INFO
 *        - rxPayloadInfo is present
 */
#define sa_ESP_TXRX_VALID_RX_PAYLOAD_INFO       sa_PKT_INFO_VALID_RX_PAYLOAD_INFO 

/*@}*/
/** @} */


/**
 * @ingroup salld_api_structures
 * @brief IpSec ESP Data send/receive info structure
 *
 */
typedef struct salldIpsecEspTxRxInfo_s {
  uint16_t    validBitMap;                /**< Specify which parameters are vaild as defined at @ref SALLDEspInfoTxRxValidBits */
 uint16_t     ivSize;              /**< Specify the size of the initialization vector in bytes */  
  uint16_t    macSize;             /**< Specify the size of the authentication tag in bytes */
  uint16_t    encryptionBlockSize; /**< Specify the encryption block size 
                                      1: Stream encryption and no alignment requirement
                                      4: AES-CTR or other stream-like encryption with 4-byte 
                                         alignment
                                      block size: block encryption algorithm */
  Sa_ipsecNatTInfo_t natTInfo;     /**< Specify the IPSEC NAT-T parameters, if natTInfo is present*/
  Sa_RxPayloadInfo_t rxPayloadInfo;/**< Specify the received payload information in IPSEC modes */
} salldIpsecEspTxRxInfo_t;

/**
 * @ingroup salld_api_structures
 * @brief Packet Descriptor structure
 *
 * The packet may consist of one or more segments.
 * Several assumptions regarding the data organization among these segment(s) were made, 
 * in compliance with requirements from the supported protocols. Below are a
 * list of assumptions imposed upon each packet with regard to their corresponding protocol. 
 *
 *  - IPSec
 *      -# [Transmit] Segment containing IP Header must reserve approx 32 bytes of buffer space at the 
 *          beginning of the segment, to allow insertion of ESP/AH header after the IP header. 
 *      -# [Transmit] Final segment containing IP Datagram must reserve approx 32 bytes of buffer space at the
 *          end of the segment, to allow insertion of ESP padding, ESP trailer and authentication tag. 
 *      -# [General] IP Header must be contiguous and in one segment. 
 *  - SRTP
 *      -# [Transmit] Final segment containing SRTP payload must reserve approx 16 bytes of buffer space at the
 *          end of the segment, to allow insertion of MKI and authentication tag. 
 *      -# [General] RTP Header must be contiguous in one segment. 
 *      -# [Receive] Total number of segments used to carry SRTP payload may not exceed 10.
 *  - Air Cipher
 *      -# No assumptions were made as data segments are untouched by the processing function. 
 *  - Data Mode
 *      -# No assumptions were made as data segments are untouched by the processing function. 
 *
 *  @note Elements in the packet descriptor structure may change, after each protocol specific processing. 
 */
typedef struct {
  int32_t   size;             /**< packet length */
  uint16_t  payloadOffset;    /**< offset from base of the packet to the header
                                   of protocol per the following list:
                                   IPSEC ESP/AH: IP header
                                   IPSEC ESP(Output): ESP Header
                                   SRTP: RTP header
                                   SRTCP: RTCP header
                                   3GPP Air Ciphering: PDU header */
  uint16_t  payloadLen;       /**< length of the payload starting from payloadOffset to the end of the protocol */
  uint16_t  nSegments;        /**< number of segments */
  void**    segments;         /**< data segments */
  uint16_t *segUsedSizes;     /**< pointer to segment used size array */ 
  uint16_t *segAllocSizes;    /**< pointer to segment allocated size array */
} Sa_PktDesc_t;               /* base class */

/**
 * @ingroup salld_api_structures
 * @brief Packet Information structure
 *
 * Data structure defines the SALLD input/output packet formats  
 *
 */
typedef struct {
  Sa_PktDesc_t pktDesc;         /**< Specify packet descriptor */
  uint16_t  validBitMap;        /**< Specify which parameters are vaild as defined at @ref SALLDPktInfoValidBits */
  uint16_t          pktErrCode; /**< Specify the error code of the received packet as defined at Sa_PktErr_t */
  Sa_SWInfo_t       swInfo;     /**< Specify the software information required by SA */
  Sa_CmdLabelInfo_t cmdlb;      /**< Specify the command label in data mode */
  Sa_PayloadInfo_t  payloadInfo;/**< Specify the payload information in data mode */
  Sa_ipsecNatTInfo_t natTInfo;  /**< Specify the IPSEC NAT-T parameters */
  Sa_RxPayloadInfo_t rxPayloadInfo;/**< Specify the received payload information in IPSEC modes */
} Sa_PktInfo_t;

/**
 *  @defgroup SALLDSrtpKeyReqInfo  SALLD SRTP Key Request Info Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SRTP Key Request Info Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in Sa_SrtpKeyRequest_t. 
 */ 
/*@{*/
/**
 *  @def  sa_SRTP_KEY_REQUEST_KEY_TYPE_MKI
 *        Control Info -- Set: MKI Key
 *                        Clear: From-To Key
 */
#define sa_SRTP_KEY_REQUEST_KEY_TYPE_MKI      0x0001 
/**
 *  @def  sa_SRTP_KEY_REQUEST_TX_KEY
 *        Control Info -- Request Tx key
 */
#define sa_SRTP_KEY_REQUEST_TX_KEY            0x0002 
/**
 *  @def  sa_SRTP_KEY_REQUEST_RX_KEY
 *        Control Info -- Request Rx key
 */
#define sa_SRTP_KEY_REQUEST_RX_KEY            0x0004 
/**
 *  @def  sa_SRTP_KEY_REQUEST_MKI_VALID
 *        Control Info -- MKI VALUE included
 */
#define sa_SRTP_KEY_REQUEST_MKI_VALID         0x0008 

/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SRTP Key Request structure
 *
 * Data structure defines the key request information for SRTP  
 *
 */
typedef struct {
  uint16_t    ctrlBitfield;    /**< Specify the Key types and other control Information 
                                    as defined at @ref SALLDSrtpKeyReqInfo */
  uint32_t    mki;             /**< Specify the MKI value of the MKI keys */
} Sa_SrtpKeyRequest_t;

/**
 * @ingroup salld_api_structures
 * @brief Key Request structure
 *
 * Data structure defines the key request information for all supported protocols  
 *
 */
typedef struct {
  union {
    Sa_SrtpKeyRequest_t   srtp;  /**< Specify the key request parameters for SRTP */
  } params;                      /**< Contain the protocol-specific key request information */
} Sa_KeyRequest_t; 

/**
 * @ingroup salld_api_structures
 * @brief Security Context Request Information structure
 *
 * SASS is equipped with context cache module to auto-fetch security context 
 * from external memory. This module is essential to allow any number of simultaneous 
 * security connections by caching only limited (64) contexts on-chip and fetching other 
 * contexts as and when required for processing.
 *
 * In order to facilitate fast retrieval for performance critical connections such as IPSEC channels, 
 * context cache module allows two tier of security connections. First tier has permanent 
 * residence within Context cache RAM for fast retrieval and is never evicted automatically 
 * by context cache module. Second tier connections are kept as long as space is available within 
 * context cache RAM; new fetch request may automatically evict the context of the second tier connections  
 * into external memory to allow free space.
 *
 * The following data structure defines the parameters for the security context request.
 * The 15-bit security context ID, of which LSB 4-bits act as cache set select, and 
 * its corresponding context buffer, which contains the protocol-specific security parameters 
 * used by the LLD and sub-engines within SA, should be maintained across multiple DSP cores. 
 * They are multi-core resources and should be managed by the application.   
 *
 * The security context IDs and buffers can be managed across mult-cores or be pre-allocated for 
 * each core or each security channel. The security context buffer should be 16-byte aligned at least 
 * and it should be cache-line aligned if it is allocated at cacheable memory. It is recommended to 
 * maintain the security context buffers in the following three lists:
 *
 *  @verbatim 
    Allocated: The buffer and its corresponding ID have been allocated to the SA
               LLD through *ScAlloc API and they are owned by SA.
    Pending Free: The buffer and its corresponding ID has been released from the 
                  SA LLD through *ScFree API. However, they are still owned by 
                  SA until the security context tear-down procedure is complete.
                  The API Sa_isScBufFree should be used to check 
                  whether the buffer is free
    Free: The buffer and its corresonding ID are owned by the application.
    @endverbatim
 *
 *  @note There are only up to three first tier contexts allowed for each cache set.
 * 
 *  @note It is highly recommended to place the security context buffers at non-cacheable memory 
 *        since they are accessed and maintained by the SA sub-system primarily
 *
 *  @note The application can manage the security context buffers and IDs as
 *        two seperate resources or a single resource pair. The only
 *        requirement is that the security context ID cannot be re-used until
 *        its corresponding security context buffer is free.
 *        The application may perform periodic checks to move buffers from the
 *        Pending Free list to the Free list or perform checks only when new
 *        buffers are requested by the SA LLD.    
 */
typedef struct {
  uint16_t    scSize; /**< Specify the size of the required security context */                        
  uint16_t    scID;   /**< Security Context ID specified by the application */
  uintptr_t   scBuf;  /**< Security Context Buffer provided by the application
                          (16-byte alignment required) */
} Sa_ScReqInfo_t;

/**
 *  @def  sa_SC_ID_MASK
 *        Define security context ID mask.
 */
#define sa_SC_ID_MASK                  0x7FFF

/**
 *  @def  sa_SC_IND_PERMANENT
 *        Indicate that the corresponding security context is permanent
 */
#define sa_SC_IND_PERMANENT            0x8000


/**
 *  @defgroup  saScMaxSize Security Context maximum size requirements
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name Security Context maximum size
 *
 *  Define Security Context maximum size requirements.
 */
/* @{ */

/**
 *  @def  sa_IPSEC_TX_MAX_SC_SIZE
 *        The maximum security context size required for IPSEC Tx channel
 */
#define sa_IPSEC_TX_MAX_SC_SIZE         288


/**
 *  @def  sa_IPSEC_RX_MAX_SC_SIZE
 *        The maximum security context size required for IPSEC Rx channel
 *
 *  Note: The maxmium IPSEC Rx channel security context includes the extra 128 bytes used by 
 *        large replay window when the replay window size exceeds 128.
 */
#define sa_IPSEC_RX_MAX_SC_SIZE         448
#define sa_IPSEC_RX_MAX_SC_SIZE_SM_REPLAY_WINDOW         320

/**
 *  @def  sa_SRTP_TX_MAX_SC_SIZE
 *        The maximum security context size required for SRTP Tx channel
 */
#define sa_SRTP_TX_MAX_SC_SIZE          224


/**
 *  @def  sa_SRTP_RX_MAX_SC_SIZE
 *        The maximum security context size required for SRTP Rx channel
 */
#define sa_SRTP_RX_MAX_SC_SIZE          288

/**
 *  @def  sa_AC_MAX_SC_SIZE
 *        The maximum security context size required for Air Ciphering channel
 */
#define sa_AC_MAX_SC_SIZE               288

/**
 *  @def  sa_DATA_MODE_MAX_SC_SIZE
 *        The maximum security context size required for Data Mode channel
 */
#define sa_DATA_MODE_MAX_SC_SIZE        256

/**
 *  @def  sa_MAX_SC_SIZE
 *        Define the maxmium size of the security context buffer defined at @ref Sa_ScReqInfo_t.
 */
#define sa_MAX_SC_SIZE                  448   

/* @} */  
/** @} */

/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maxmium replay window size supported by SASS. 
 */
#define sa_MAX_REPLAY_WINDOW_SIZE       1024
#define sa_MAX_REPLAY_WINDOW_SIZE_GEN1  128 	 /* first generation SASS */
#define sa_MAX_REPLAY_WINDOW_SIZE_GEN2  1024	 /* second generation SASS */

/**
 * @ingroup salld_api_structures
 * @brief SALLD Replay Window Context
 *
 * This structure defines the replay window context which reprenets the snapshot of
 * SASS replay windows.
 */

typedef struct Sa_Replay_Cxt_tag
{
    uint32_t  winMask[sa_MAX_REPLAY_WINDOW_SIZE/32];    /**< Bitmask Array:w0.b0, w0.b1, ... w1.b0, w0.b2 ...
                                                             1:packet received  
                                                             0:packet not received*/
    uint32_t  winBaseHi;                                /**< Upper 32-bit of win_base when ESN is enabled */
    uint32_t  winBase;                                  /**< Lower 32-bit of win_base which represents the sequence number of
                                                             the oldest packet received */
} Sa_Replay_Cxt_t; 

/**
 *  @defgroup SALLDStatsCtrlBit  SALLD Statistics Query Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD Statistics Query Control Bit Definitions
 *  Bitmap definition of the control flags in Sa_chanGetStats() API. 
 */ 
/*@{*/
/**
 *  @def  sa_STATS_QUERY_FLAG_CLEAR
 *        Control Info -- Clear some statistics after they are reported
 */
#define sa_STATS_QUERY_FLAG_CLEAR           0x0001 
/**
 *  @def  sa_STATS_QUERY_FLAG_TRIG
 *        Control Info -- Inform the SA Sub-system to provide statistics
 */
#define sa_STATS_QUERY_FLAG_TRIG            0x0002 
/**
 *  @def  sa_STATS_QUERY_FLAG_NOW
 *        Control Info -- Request the SA LLD to provide the latest available statistics
 *        If the bit is not set, the SA LLD will return sa_ERR_STATS_UNAVAIL if the 
 *        latest statistics is not available
 */
#define sa_STATS_QUERY_FLAG_NOW             0x0004 
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SALLD SRTP Statistics Structure
 *
 * This structure defines the SRTP-specific statistics provided upon request
 * with salldChanStats().
 */
typedef struct {
  uint32_t   replayOld;     /**< Number of replay failures with old packets */
  uint32_t   replayDup;     /**< Number of replay failures with duplicated packets */
  uint32_t   authFail;      /**< Number of Authentication failures */
  uint32_t   txROC;         /**< Tx Roll-over Counter (32-bits) */
  uint32_t   rxROC;         /**< RX Roll-Over Counter (32 bits) */
  uint16_t   txRekey;       /**< Number of times re-keying happened in Tx */
  uint16_t   rxRekey;       /**< Number of times re-keying happened in Rx */
  uint16_t   pktEncHi;      /**< Total Number of Packets encrypted with this key (48-bits) Upper 16 bits */
  uint32_t   pktEncLo;      /**< Total Number of Packets encrypted with this key (48-bits) Lower 32 bits */
  uint16_t   pktDecHi;      /**< Total Number of Packets decrypted with this key (48-bits) Upper 16 bits */
  uint32_t   pktDecLo;      /**< Total Number of Packets decrypted with this key (48-bits) Lower 32 bits */
} Sa_SrtpStats_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD SRTCP Statistics Structure
 *
 * This structure defines the SRTCP-specific statistics provided upon request
 * with salldChanStats().
 */
typedef struct {
  uint32_t   replayOld;     /**< Number of replay failures with old packets */
  uint32_t   replayDup;     /**< Number of replay failures with duplicated packets */
  uint32_t   authFail;      /**< Number of Authentication failures */
  uint16_t   txRekey;       /**< Number of times re-keying happened in Tx */
  uint16_t   rxRekey;       /**< Number of times re-keying happened in Rx */
  uint32_t   pktEnc;        /**< Total Number of Packets encrypted with this key (32-bits) */
  uint32_t   pktDec;        /**< Total Number of Packets decrypted with this key (32-bits) */
} Sa_SrtcpStats_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD IPSEC Statistics Structure
 *
 * This structure defines the IPSEC-specific statistics provided upon request
 * with salldChanStats().
 */
typedef struct {
  Sa_Replay_Cxt_t replayCxt;   /**< Snapshot of the replay context */ 
  uint32_t   replayOld;     /**< Number of replay failures with old packets */
  uint32_t   replayDup;     /**< Number of replay failures with duplicated packets */
  uint32_t   authFail;      /**< Number of Authentication failures */
  uint32_t   txRollover;    /**< Number of Tx packets dropped due to rollover error, i.e. the sequence number exceed its limit (0xFFFFFFFF) */
  uint32_t   txESN;         /**< Tx ESN (32-bits) */
  uint32_t   txSN;          /**< Sequence Number (32-bits) of the last transmitted packet */
  uint32_t   rxESN;         /**< RX ESN (32 bits) */
  uint32_t   pktEncHi;      /**< Total Number of Packets encrypted with this key (64-bits) Upper 32 bits */
  uint32_t   pktEncLo;      /**< Total Number of Packets encrypted with this key (64-bits) Lower 32 bits */
  uint32_t   pktDecHi;      /**< Total Number of Packets decrypted with this key (64-bits) Upper 32 bits */
  uint32_t   pktDecLo;      /**< Total Number of Packets decrypted with this key (64-bits) Lower 32 bits */
  uint32_t   txByteCountHi; /**< Total bytes processed by SA on TX (64-bits) Upper 32 bits */
  uint32_t   txByteCountLo; /**< Total bytes processed by SA on TX (64-bits) Lower 32 bits */
  uint32_t   rxByteCountHi; /**< Total bytes processed by SA on RX (64-bits) Upper 32 bits */
  uint32_t   rxByteCountLo; /**< Total bytes processed by SA on RX (64-bits) Lower 32 bits */
} Sa_IpsecStats_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD Air Ciphering Statistics Structure
 *
 * This structure defines the Air Ciphering-specific statistics provided upon request
 * with salldChanStats().
 */
typedef struct {
  uint32_t   authFail;      /**< Number of Authentication failures */
  uint32_t   toAirCountC;   /**< To-Air Count-C (32-bits) */
  uint32_t   fromAirCountC; /**< From-Air Count-C (32-bits) */ 
  uint32_t   pktToAirHi;    /**< Total Number of To-Air Packets (64-bits) Upper 32 bits */
  uint32_t   pktToAirLo;    /**< Total Number of To-Air Packets (64-bits) Lower 32 bits */
  uint32_t   pktFromAirHi;  /**< Total Number of From-Air Packets (64-bits) Upper 32 bits */
  uint32_t   pktFromAirLo;  /**< Total Number of From-Air Packets (64-bits) Lower 32 bits */
} Sa_AcStats_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD Data Mode Statistics Structure
 *
 * This structure defines the Data Mode statistics provided upon request
 * with salldChanStats().
 */
typedef struct {
  uint32_t   pktHi;         /**< Total Number of processed Packets (64-bits) Upper 32 bits */
  uint32_t   pktLo;         /**< Total Number of processed Packets (64-bits) Lower 32 bits */
} Sa_DataModeStats_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD Statistics Structure
 *
 * This structure defines the protocol-specific statistics provided upon request
 * with salld_chanGetStats().
 */
typedef union {
  Sa_SrtpStats_t      srtp;   /**< SRTP-specific channel statistics */
  Sa_SrtcpStats_t     srtcp;  /**< SRTCP-specific channel statistics */
  Sa_IpsecStats_t     ipsec;  /**< IPSEC-specific channel statistics */
  Sa_AcStats_t        ac;     /**< 3GPP Air Ciphering-specific channel statistics */
  Sa_DataModeStats_t  data;   /**< Data Mode channel statistics */
} Sa_Stats_t; 

/**
 * @ingroup salld_api_structures
 * @brief Specification of Sa_Handle 
 * The Sa_Handle is used to identify a SALLD system instance
 */
typedef void* Sa_Handle;

/**
 *  @defgroup saConfigCtrlBit SA Configuration Control Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SA Configuration Control Bit Definitions
 *
 *  Bitmap definition of the ctrlBitMap in Sa_Config_t. 
 *  
 */ 
/*@{*/
/**
 *  @def  sa_CONFIG_CTRL_BITMAP_TRIGGER_SYS_ERR_HALT
 *        Control Info -- 0:Disable trigger PDSP halt during system errors  (refer to @ref appendix3)
 *                        1:Enables trigger PDSP halt during system errors  (for Debug Purpose only).  
 */

#define  sa_CONFIG_CTRL_BITMAP_TRIGGER_SYS_ERR_HALT            0x0001

/**
 *  @def  sa_CONFIG_CTRL_BITMAP_TRIGGER_PKT_INFO_LOG
 *        Control Info -- 0:Disable internal internal packet information data collection (refer to @ref appendix3) 
 *                        1:Enables internal internal packet information data collection (for Debug Purpose only)
 *
 *  @note This control bit is applicable to Air Cipher channels only. This fetaure should not be enabled if there might be
 *        both Air Cipher and SRTP channels. This feature bit is not supported for C6678 device.
 */

#define  sa_CONFIG_CTRL_BITMAP_TRIGGER_PKT_INFO_LOG           0x0002

/**
 *  @def  sa_CONFIG_CTRL_BITMAP_SET_SCPTR_RANGE
 *        Control Info -- 0:Disable setting of security context pointer range limit set
 *                        1:Enables security context pointer range limit set
 *
 *  @note This control bit is applicable to SA2UL only (for other generations it is don't care). 
 *        For SA2UL promote (packet from non secure to secure operations), the security context range needs to be
 *        set in SCPTR_Promote_Range registers
 */

#define  sa_CONFIG_CTRL_BITMAP_SET_SCPTR_RANGE                0x0004

/**
 *  @def  sa_CONFIG_CTRL_BITMAP_LIMIT_ACCESS
 *        Control Info -- 0: All SA2UL registers may be accessed
 *                        1: Limit access to SA2UL registers which are reserved
 *                           for use by DMSC firmware
 *
 *  @note This control bit is applicable to SA2UL only (for other generations it is don't care).
 *        It is furthermore only necessary on High Secure (HS) device variants
 *        when an application wishes to share access to the SA2UL instance which
 *        is owned by DMSC firmware. DMSC prohibits read/write access to MMRA
 *        region on this instance, so this bit has the following effect:
 *
 *        * Bypasses programming ENGINE_ENABLE register. DMSC sets all engines
 *          to enabled at device boot. The driver can confirm engine status
 *          through the ENGINE_STATUS register.
 *
 *        * Ignores the SET_SCPTR_RANGE control flag and bypasses any attempt to
 *          access the SCPTR promote registers
 */

#define  sa_CONFIG_CTRL_BITMAP_LIMIT_ACCESS                   0x0008



/*@}*/
/** @} */

/**
 *  @ingroup salld_api_structures
 *  @brief   The SALLD Call-out function table
 *
 *  The SALLD software module requires a set of call-out functions to report debug information,
 *  request and free system resources, and etc. All the call-out functions should be implemented
 *  by the application and provided to the SALLD at @ref Sa_create
 *          
 */
typedef struct Sa_CallOutFuns_s {

/**
 *  @brief  A callout to the system code's debug and exception handling function.  This is a function
 *  pointer and must point to a valid function which meets the API requirements.    
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   msgType      Specify how serious the debug message is as defined at @ref salldDebugMessageTypes.
 *  @param[in]   msgCode      Specify the message code as defined at @ref salldDebugMessageCodes.
 *  @param[in]   msgLength    Specify the length of the message supporting data.
 *  @param[in]   msgData      Pointer to the message supporting data.
 */
   void  (*DebugTrace) (Sa_ChanHandle handle, uint16_t msgType, uint16_t msgCode, 
                        uint16_t msgLength, uint16_t *msgData);
/**                      
 *  @brief  Callout to externally supplied system to request a new security key. This function may be triggered
 *  by either the Sa_chanSendData() or Sa_chanReceiveData() APIs. The application should call the 
 *  Sa_chanControl() API to pass the new key when it is available.
 *  This is a function pointer and must point to a valid function which meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   keyReq       Pointer to SALLD key Request structure.
 *
 *  @sa Sa_KeyRequest_t
 *
 */
   void (*ChanKeyRequest) (Sa_ChanHandle handle, Sa_KeyRequest_t* keyReq);
   
/**
 *  @brief  Callout to externally supplied system to allocate the security context with the specified size.
 *  This function must be implemented as a simple non-blocking function.
 *  This is a function pointer and must point to a valid function which meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   scReqInfo    Pointer to SALLD security context Request Information structure.
 *
 *  @sa Sa_ScReqInfo_t
 *
 *  @note:Both the base address and size of the security context buffer should be cache-line and csche-line size 
 *        aligned if it is allocated at cacheable memory, otherwise, the base address should be 16-byte aligned.
 *        It is highly recommended to place the security context buffers at non-cacheable memory 
 *        since they are accessed and maintained by the SA sub-system primarily
 */
   void (*ScAlloc) (Sa_ChanHandle handle, Sa_ScReqInfo_t* scReqInfo);
   
/**
 *  @brief  Callout to externally supplied system to release the security context with the specified ID.
 *  This function must be implemented as a simple non-blocking function.
 *  This is a function pointer and must point to a valid function which meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   scID         Security Context ID
 *
 *  @note The security context buffer is only released from the SA LLD and is still owned by SA.
 *        It should be maintained at the Pending Free list until it is freed by SA.
 *        Use API @ref Sa_isScBufFree() to check its status.
 *
 */
   void (*ScFree) (Sa_ChanHandle handle, uint16_t scID);
   
/**
 *  @brief  Callout to externally supplied system to register the security channel with its software
 *  routing information to be programmed into the PASS lookup table in the from-Network direction. 
 *  It may be triggered by the Sa_chanControl(), Sa_chanSendData() and Sa_chanReceiveData() APIs.
 *  This is a function pointer and must point to a valid function which meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   chanSwInfo   Pointer to SALLD software routing information structure.
 *
 *  @sa Sa_SWInfo_t
 *
 */
   void (*ChanRegister) (Sa_ChanHandle handle, Sa_SWInfo_t* chanSwInfo);

/**
 *  @brief  Callout to externally supplied system to un-register the security channel with its software
 *  routing information to be removed from the PASS lookup tables. It may be triggered by 
 *  the sSa_chanClose(), Sa_chanSendData() and Sa_chanReceiveData() APIs.
 *  This is a function pointer and must point to a valid function which meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   chanSwInfo   Pointer to SALLD software routing information structure.
 *
 *  @sa Sa_SWInfo_t
 *
 */
   void (*ChanUnRegister) (Sa_ChanHandle handle, Sa_SWInfo_t* chanSwInfo);
   
/**
 *  @brief  Callout to externally supplied system to send an Null packet to the SA sub-system.
 *  The null packet is used to evict and/or tear down the security context associated with the channel. 
 *  It may be triggered by the Sa_chanClose(), Sa_chanSendData() and Sa_chanReceiveData() APIs.
 *  This is a function pointer and must point to a valid function which meets the API requirements.
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   pktInfo      Pointer to the packet info structure.
 *
 *  @sa Sa_PktInfo_t
 *
 */
   void (*ChanSendNullPkt) (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo);
   
} Sa_CallOutFuncs_t;

/**
 * @ingroup salld_api_structures
 * @brief  SA Size Configuration Structure
 *
 * @details The module is configured at run time with a maximum number of channels supported. 
 */
typedef struct  {
  uint16_t nMaxChan;       /**< Maximum number of channels supported */
  int      cacheLineSize;  /**< Specify the size of cache line of the memory where the system buffer will reside 
                                @note cacheLineSize should be the larger of cache line sizes amoung mixed processors */
  uint16_t ctrlBitMap;     /**< Various configuration information as specified at @ref saSizeConfigCtrlBit */                                   
} Sa_SizeCfg_t;

/**
 * @ingroup salld_api_structures
 * @brief  SA Security Context Range Structure for promote operations
 *
 * @details for SA2UL promote operations, the range register needs to be set with correct address ranges 
 */
typedef struct  {
  uint32_t           scPtrPromoteLowRangeL; /**< The lower 32-bits of SCPTR lower limit (must be 4KB aligned) */
  uint32_t           scPtrPromoteLowRangeH; /**< The upper 16-bits of SCPTR lower limit */
  uint32_t           scPtrPromoteHighRangeL; /**< The lower 32-bits of SCPTR upper limit (must be 4KB aligned) */
  uint32_t           scPtrPromoteHighRangeH; /**< The upper 16-bits of SCPTR upper limit */
} Sa_ScPtrRangeCfg_t;

/**
 * 	@defgroup saEngSelMode SA Engine Selector Algorithms
 *	@ingroup	salld_api_constants
 *	@{
 *
 *	@name	SA Engine Selector Algorithms
 *	
 * 	There are two sets of IPSEC processing engines at the second generation SASS. 
 *  During SA initialization, user may configure IPSEC processing engine set
 * 	selection algorithm to be used during channel creation by setting the variable 
 *	engSelMode to the following values:
 *
 * - 0 (Load balanced)
 *  -# SA will select the set of engines with lower channel utility
 * - 1 (Channel-based Round Robin selection)
 *  -# SA will use each set of engines interleaved in order for each channel created 
 * - 2 (User Select)
 *  -# User will specify engine set to use when creating new channel
 */
 /*@{*/

typedef enum {
    sa_EngSelMode_LOADBALANCED = 0,
    sa_EngSelMode_ROUNDROBIN,
    sa_EngSelMode_USERSPECIFIED
} Sa_EngSelMode_e;
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SALLD Configuration structure
 *
 * Data structure providing configuration information in @ref Sa_create()
 * 
 */
typedef struct {
  uint32_t           ID;          /**< User specified module ID */
  uint32_t           baseAddr;    /**< Specify the SASS base address */
  Sa_CallOutFuncs_t  *callTable;  /**< Pointer to call-out function Table */
  Sa_EngSelMode_e    engSelMode;  /**< User specified authentication/encryption 
                                       engine set selection mode */
  void*              intBuf;      /**< Pointer to an arbitrary internal buffer of at least @ref sa_MAX_SC_SIZE bytes 
                                       for temporary key storage during air ciphering security 
                                       context construction, The buffer needs an alignment of 4 bytes */
  Sa_SizeCfg_t      *sizeConfig;  /**< SALLD memory allocation requirement structure */
  void              *instPoolBaseAddr; /**< Base address of the global shared memory pool from which global 
                                            LLD instance & channel instance memory is allocated.*/ 
  void              *scPoolBaseAddr;   /**< Base address of the global shared memory pool from which SA
                                             security context memory is allocated. This is a DMA-able memory */
  Sa_ScPtrRangeCfg_t scPtrRange;      /**< Security Context Buffer Range low and high address configuration */
  uint32_t           ctrlBitMap;      /**< Various configuration information as specified at @ref saConfigCtrlBit */                                                
} Sa_Config_t;

/** 
* @ingroup salld_api_structures 
* @brief SALLD SRTP System Statistics Structure 
* 
* This structure defines the SRTP-specific system statistics provided upon request 
* with Sa_getSysStats(). 
*/ 
typedef struct { 
  uint32_t  replayOld;    	/**< Number of replay failures with old packets */ 
  uint32_t  replayDup;    	/**< Number of replay failures with duplicated packets */ 
  uint32_t  authFail;     	/**< Number of Authentication failures */ 
  uint32_t  pktEncHi;     	/**< Total Number of Packets encrypted Upper 32 bits */ 
  uint32_t  pktEncLo;     	/**< Total Number of Packets encrypted Lower 32 bits */ 
  uint32_t  pktDecHi;     	/**< Total Number of Packets decrypted Upper 32 bits */ 
  uint32_t  pktDecLo;     	/**< Total Number of Packets decrypted Lower 32 bits */ 
} Sa_SrtpSysStats_t; 

/** 
* @ingroup salld_api_structures 
* @brief SALLD IPSEC System Statistics Structure 
* 
* This structure defines the IPSEC-specific system statistics provided upon request 
* with Sa_getSysStats(). 
*/ 
typedef struct { 
  uint32_t  replayOld;    	/**< Number of replay failures with old packets */ 
  uint32_t  replayDup;    	/**< Number of replay failures with duplicated packets */ 
  uint32_t  authFail;     	/**< Number of Authentication failures */ 
  uint32_t  pktEncHi;     	/**< Total Number of Packets encrypted Upper 32 bits */ 
  uint32_t  pktEncLo;     	/**< Total Number of Packets encrypted Lower 32 bits */ 
  uint32_t  pktDecHi;     	/**< Total Number of Packets decrypted Upper 32 bits */ 
  uint32_t  pktDecLo;     	/**< Total Number of Packets decrypted Lower 32 bits */ 
} Sa_IpsecSysStats_t; 

/** 
* @ingroup salld_api_structures 
* @brief SALLD Air Ciphering System Statistics Structure 
* 
* This structure defines the Air Ciphering-specific systrem statistics provided upon request 
* with Sa_getSysStats(). 
*/ 
typedef struct { 
  uint32_t  authFail;      	/**< Number of Authentication failures */ 
  uint32_t  pktToAirHi;    	/**< Total Number of To-Air Packets Upper 32 bits */ 
  uint32_t  pktToAirLo;    	/**< Total Number of To-Air Packets Lower 32 bits */ 
  uint32_t  pktFromAirHi;  	/**< Total Number of From-Air Packets Upper 32 bits */ 
  uint32_t  pktFromAirLo;  	/**< Total Number of From-Air Packets Lower 32 bits */ 
} Sa_AcSysStats_t; 

/** 
* @ingroup salld_api_structures 
* @brief SALLD Error System Statistics Structure 
* 
* This structure defines the SA error system statistics provided upon request 
* with Sa_getSysStats(). 
 */
typedef struct {
  uint32_t   errNoMem;      /**< Number of packets dropped due to lack of PDSP instance memory */
  uint32_t   errCtx;        /**< Number of packets dropped due to security context error */
  uint32_t   errEngine;     /**< Number of packets dropped due to SA processing engine error */
  uint32_t   errProto;      /**< Number of packets dropped due to protocol error such as inavlid IP version */
} Sa_ErrorSysStats_t; 

/** 
* @ingroup salld_api_structures 
* @brief SALLD Statistics Structure 
* 
* This structure defines the system statistics provided upon request 
* with @ref Sa_getSysStats(). 
*/ 
typedef struct Sa_SysStats_s { 
  Sa_ErrorSysStats_t    err;    /**< System Error statistics */ 
  Sa_IpsecSysStats_t    esp;    /**< IPSEC(ESP)-specific system statistics */ 
  Sa_IpsecSysStats_t    ah;     /**< IPSEC(AH)-specific system statistics */ 
  Sa_SrtpSysStats_t     srtp;   /**< SRTP-specific system statistics */ 
  Sa_AcSysStats_t       ac;     /**< 3GPP Air Ciphering-specific system statistics */ 
} Sa_SysStats_t; 

/**
 *  @defgroup PkaOpTypes PKA Arithmetic Operation Types
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name PKA Arithmetic Operation Types
 *
 *  Definition of PKA Arithmetic Operation Types
 *  Parameter operation of @ref Sa_PkaReqInfo_t and @ref Sa_PkaReqInfo2_t should be set 
 *  to one of these types.
 */ 
/*@{*/
typedef enum {
  /**
   * List of PKA basic operations supported by both NSS PKA Gen1 and Gen2 devices.
   */
  sa_PKA_OP_ADD = 0,     /**< Large Vector Addition (A+B)*/
  sa_PKA_OP_SUB,         /**< Large Vector Subtraction (A-B)*/
  sa_PKA_OP_MUL,         /**< Large Vector Mutiplication (A*B)*/
  sa_PKA_OP_DIV,         /**< Large Vector Division (A/B)*/
  sa_PKA_OP_RSHIFT,      /**< Large Vector Shift Right (A>>shiftval) */
  sa_PKA_OP_LSHIFT,      /**< Large Vector Shift Left (A<<shiftval)*/
  sa_PKA_OP_COMP,        /**< Large Vector Comparison (A?B)*/
  sa_PKA_OP_COPY,        /**< Large Vector Copy (A->C)*/
  sa_PKA_OP_EXP2,        /**< Large Vector Exponentiation (Cexp(A)mod(B))(2-bit ACT) (NSS PKA Gen1 only) */
  sa_PKA_OP_EXP4,        /**< Large Vector Exponentiation (Cexp(A)mod(B))(4-bit ACT) (NSS PKA Gen1 only) */
  sa_PKA_OP_ADD_SUB,     /**< Large Vector Addition plus Substraction (A+C-B) (NSS PKA Gen2 only) */
  sa_PKA_OP_MOD,         /**< Large Vector Modulo (A%B) (NSS PKA Gen2 only) */
  /**
   * List of PKA complex operations supported by NSS PKA Gen2 devices only.
   */
  sa_PKA_OP_MODEXP,         /**< Large Vector Modular Exponentiation: M**E(mod N) */
  sa_PKA_OP_MODEXP_CRT,     /**< Large Vector Modular Exponentiation: CRT CRTpq(Mp**Dp (mod p), Mq**Dq (mod q), qInv) */
  sa_PKA_OP_MODINVp,        /**< Large Vector Modular Inversion (prime): (1/Z) (mod N) */
  sa_PKA_OP_MODINV2m,       /**< Large Vector Modular Inversion (binary): (1/Z) (mod N) */
  sa_PKA_OP_ECp_ADD,        /**< Large Vector Point Add on a Prime Field Curve: 
                             *   P1_xyx + P2_xyx ==> P0_xyz, on prime curve y**2=x**3+ax+b (mod p) */ 
  sa_PKA_OP_ECp_MUL,        /**< Large Vector Point Multiply on a Prime Field Curve: 
                             *   k*P1_xyx ==> P0_xyz  with P1_z = 1, on prime curve y**2=x**3+ax+b (mod p) */
  sa_PKA_OP_ECp_SCALE,      /**< Large Vector Point Scale on a Prime Field Curve: 
                             *   P1_xyx ==> P0_xyz with P0_z = 1, on prime curve y**2=x**3+ax+b (mod p) */
  sa_PKA_OP_ECp_MUL_SACLE,  /**< Large Vector Point Multiply following by Scale on a Prime Field Curve: 
                             *   k*P1_xyx ==> P0_xyz  with P1_z = 1 and P0_z = 1, on prime curve y**2=x**3 + ax + b (mod p) */
  sa_PKA_OP_EC2m_ADD,       /**< Large Vector Point Add on a Binary Field Curve: 
                             *   P1_xyx + P2_xyx ==> P0_xyz, on binary curve y**2=x**3 + ax + b (mod p) */ 
  sa_PKA_OP_EC2m_MUL,       /**< Large Vector Point Multiply on a Binary Field Curve: 
                             *   k*P1_xyx ==> P0_xyz  with P1_z = 1, on binary curve y**2+xy =x**3+ax**2+b (mod p) */
  sa_PKA_OP_EC2m_SCALE,     /**< Large Vector Point Scale on a Binary Field Curve: 
                             *   P1_xyx ==> P0_xyz with P0_z = 1, on binary curve y**2+xy =x**3+ax**2+b (mod p) */
  sa_PKA_OP_EC2m_MUL_SACLE, /**< Large Vector Point Multiply following by Scale on a Binary Field Curve: 
                             *   k*P1_xyx ==> P0_xyz  with P1_z = 1 and P0_z = 1, on binary curve y**2+xy =x**3+ax**2+b (mod p) */
  sa_PKA_OP_ECp_DSA_SIGN,   /**< Large Vector ECDSA sign operation on a Prime Field Curve: 
                             *   k*P1_xyx ==> P0_xyz  with P1_z = 1 and P0_z = 1, on prime curve y**2=x**3 + ax + b (mod p)
                             *   r = P0x mod N, s = 1/k * (h + rd) mod N where h = hash(m)n, d = private key 
                             */
  sa_PKA_OP_ECp_DSA_VERIFY   /**< Large Vector ECDSA verify operation on a Prime Field Curve:
                             *   u1*P1_xyx + u2 * Y ==> P0_xyz  with P1_z = 1 and P0_z = 1, on prime curve y**2=x**3 + ax + b (mod p)
                             *   u1 = h * w(mod N) and u2 = r * w mod N where where w = 1/s * mod N, h = hash(m)n, Y = public key
                             */
} Sa_PkaOpTypes_t;
/*@}*/
/** @} */

/**
 *  @defgroup PkaOpStatusCodes PKA Complex Operation Status Codes
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name PKA Complex Operation Status Codes
 *
 *  Status codes returned by PKA module for complex operation.
 */
/*@{*/
/**
 *  @def  sa_PKA_OP_STATUS_SUCCESS
 *        PKA Complex Operation executed successfully
 */
#define sa_PKA_OP_STATUS_SUCCESS                0x01  
/**
 *  @def  sa_PKA_OP_STATUS_PARAM_EVEN
 *        Error: Modulus or Prime polynomial is even
 */
#define sa_PKA_OP_STATUS_PARAM_EVEN             0x03 
/**
 *  @def  sa_PKA_OP_STATUS_PARAM_ZERO
 *        Error: Exponent or Multiplier k is zero
 */
#define sa_PKA_OP_STATUS_PARAM_ZERO             0x05 
/**
 *  @def  sa_PKA_OP_STATUS_PARAM_SHORT
 *        Error: Modulus is too short
 */
#define sa_PKA_OP_STATUS_PARAM_SHORT            0x07
/**
 *  @def  sa_PKA_OP_STATUS_PARAM_ONE
 *        Error: Exponent or Multiplier k is one
 */
#define sa_PKA_OP_STATUS_PARAM_ONE              0x09 
/**
 *  @def  sa_PKA_OP_STATUS_INFINITY
 *        Result is "at-infinity" (Z=0, not an error if 
 *         the scalar 'k' was the curve's order)
 */
#define sa_PKA_OP_STATUS_INFINITY               0x0D 
/**
 *  @def  sa_PKA_OP_STATUS_ERR_INFINITY
 *        Error: An intermediate result of ECpMUL was "at-infinity"
 */
#define sa_PKA_OP_STATUS_ERR_INFINITY           0x0F 
/**
 *  @def  sa_PKA_OP_STATUS_NO_INVERSE
 *        Error: No inverse exist
 */
#define sa_PKA_OP_STATUS_NO_INVERSE             0x17

/*@}*/
/** @} */


/**
 *  @defgroup PkaCompResults PKA Comparison Results
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name PKA Comparison Results
 *
 *  Definition of PKA Comparsion Result from SALLD API @ref Sa_pkaOperation
 */ 
/*@{*/
typedef enum {
  sa_PKA_COMP_AEQB  = 1,     /**< A=B */
  sa_PKA_COMP_AINFB = 2,     /**< A<B */
  sa_PKA_COMP_ASUPB = 4      /**< A>B */
} Sa_PkaCompResults_t;
/*@}*/
/** @} */

/*
 *  @ingroup salld_api_constants
 *  @brief   Define the maxmium sizes of the PKA operands in 32-bit words.
 */
#define sa_MAX_PKA_OPERAND_SIZE_GEN1      128  /**< General Operations 4k-bits or 128 32-bit words */ 
#define sa_MAX_PKA_OPERAND_SIZE_EXP4       64  /**< EXP4 operation: 2k-bits or 64 32-bit words */ 

#define sa_MAX_PKA_OPERAND_SIZE_GEN2      130  /**< General operations 4160 bits or 130 32-bit words */
#define sa_MAX_PKA_OPERAND_SIZE_ECp        24  /**< Prime Curve related operations: 24 32-bit words */ 
#define sa_MAX_PKA_OPERAND_SIZE_EC2m       18  /**< Binary Curve related operations:18 32-bit words */ 

#ifdef NSS_PKA_GEN2
#define sa_MAX_PKA_OPERAND_SIZE     sa_MAX_PKA_OPERAND_SIZE_GEN2
#else
#define sa_MAX_PKA_OPERAND_SIZE     sa_MAX_PKA_OPERAND_SIZE_GEN1
#endif
 
/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Operation Request Structure
 *
 * This structure defines PKA request information which is used to perform the large 
 * vector arithmetic operations.
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - Add: 0 < Alength, Blength <= Max Len (128)
 *  - Subtract:0 < Alength, Blength <= Max Len (128); Aoperand >= Boperand
 *  - Multiply: 0 < Alength, Blength <= Max Len (128)
 *  - Divide:
 *    - Alength, Blength > 1
 *    - Alength >= Blength
 *    - Most significant word of Boperand can not be zero
 *  - Right/Left Shift: 0 < Alength <= Max Len (128)
 *  - Compare:  0 < Alength, Blength <= Max Len (128); Alength = Blength
 *  - Copy:  0 < Alength <= Max Len (128)
 *  - Exponentitation (2-bit/4-bit ACT):
 *    - Alength > 0
 *    - Blength > 1
 *    - Blength >= Alength
 *    - Aoperand cannot be zero
 *    - Coperand cannot be zero
 *    - Boperand is odd (least significant bit set to 1)
 *    - Most significant word of Boperand can not be zero
 *    - Boperand > Coperand
 */
typedef struct {
  Sa_PkaOpTypes_t     operation;     /**< Specify the arithmetic operation as defined at salldPkaOpTypes_t */
  Sa_PkaCompResults_t cmpResult;     /**< Store the comparsion result  */
  uint16_t            numShiftBits;  /**< Specify number of bits in shift operation */
  uint16_t            oprandSize[2]; /**< Specify the size of operands (A, B(C))in 32-bit words */
  uint16_t            resultSize;    /**< Specify the size of result in 32-bit words */
  uint16_t            remSize;       /**< Specify the size of reminder in 32-bit words */
  uint32_t*           oprandPtr[3];  /**< Pointers to up to 3 operand (A,B and C) buffers*/
  uint32_t*           remPtr;        /**< Pointer to the buffer to store the reminder */
  uint32_t*           resultPtr;     /**< Pointer to the buffer to store the operation result */
} Sa_PkaReqInfo_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Basic Operation Parameters Structure
 *
 * This structure defines the input and output parameters for PKA basic operations
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - Add: 0 < ALen, BLen <= Max Len (130)
 *  - Subtract: 0 < ALen, BLen <= Max Len (130); A >= B
 *  - AddSub: 0 < ALen <= Max Len (130); (A+C) >= B; A, B, C all use ALen and BLen is ignored.
 *  - Multiply: 0 < ALen, BLen <= Max Len (130)
 *  - Divide and Modulo:
 *    - ALen, BLen > 1
 *    - ALen >= BLen
 *    - Most significant 32-bit word of B operand cannot be zero
 *  - Right/Left Shift: 0 < ALen <= Max Len (130)
 *  - Compare: 0 < ALen <= Max Len (130); Both A and B use ALen and BLen is ignored.
 *  - Copy: 0 < ALen <= Max Len (130)
 */
typedef struct {
  uint32_t*           pA;            /**< Pointers to Aoperand in 32-bit word array */
  uint32_t*           pB;            /**< Pointers to Boperand in 32-bit word array */
  uint32_t*           pC;            /**< Pointers to Coperand in 32-bit word array */
  uint32_t            numShiftBits;  /**< Specify number of bits in shift operation */
  uint32_t*           pRem;          /**< Pointer to the 32-bit word buffer to store the reminder */
  uint32_t*           pResult;       /**< Pointer to the 32-bit word buffer to store the operation result */
  uint16_t            resultSize;    /**< Store the size of operation result in 32-bit words */
  uint16_t            remSize;       /**< Store the size of reminder in 32-bit words of Divide/Modulo operations*/
  Sa_PkaCompResults_t cmpResult;     /**< Store the comparison result  */
} Sa_PkaBasicOpParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Modular Exponentiation Operation Parameters Structure
 *
 * This structure defines the input and output parameters of PKA Modular Exponentiation operation as
 * described with the following formula:
 *
 *      M**E(mod N) ==> R
 *      
 *      E: ALen
 *      M, N, R: BLen
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - 0 < ALen <= Max Len (130)
 *  - 1 < BLen <= Max Len (130)
 *  - Modulus N must be odd (i.e. the least significant bit must be ONE)
 *  - Modulus N > 2**32
 *  - Base M < Modulus N
 *  - Number of pre-calculated odd powers to use >= 1
 *
 */
typedef struct {
  uint32_t*           pE;            /**< Pointer to Exponent in 32-bit word array */
  uint32_t*           pN;            /**< Pointer to Modulus N in 32-bit word array */
  uint32_t*           pM;            /**< Pointer to Message (Base) M in 32-bit word array */
  uint32_t            numOddPowers;  /**< Specify number of pre-calculated odd powers to use */
  uint32_t*           pResult;       /**< Pointer to the 32-bit word buffer to store the operation result */
} Sa_PkaModExpParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Modular Exponentiation CRT Operation Parameters Structure
 *
 * This structure defines the input and output parameters of PKA Modular Exponentiation CRT operation as
 * described with the following formula:
 *
 *      CRTpq(Mp**Dp mod p, Mq**Dq mod q, qInv) ==> R
 *      
 *      where Mp = M mod p, Mq = M mod q and CRTpq(a, b, qInv) = ((a-b)*qInv mod p)*q + b
 *      
 *      Dp, Dq: ALen
 *      p, q, qInv: BLen
 *      M, R: 2*Blen
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - 0 < ALen <= Max Len (130)
 *  - 1 < BLen <= Max Len (130)
 *  - Modulus p and Modulus q must be odd (i.e. the least significant bit must be ONE)
 *  - Modulus p > Modulus q > 2**32
 *  - Modulus p and Modulus q must be co-prime (GCD(p,q) = 1)
 *  - TBD: 0 < Exp P < (Mod P - 1)
 *  - TBD: 0 < Exp Q < (Mod Q - 1)
 *  - (qInv * q) = 1 (mod p)
 *  - Base M < p*q
 *  - Number of pre-calculated odd powers to use >= 1
 *
 */
typedef struct {
  uint32_t*           pDp;           /**< Pointer to Exponent mod p in 32-bit word array */
  uint32_t*           pDq;           /**< Pointer to Exponent mod q in 32-bit word array */
  uint32_t*           pModP;         /**< Pointer to Modulus p in 32-bit word array */
  uint32_t*           pModQ;         /**< Pointer to Modulus q in 32-bit word array */
  uint32_t*           pQInv;         /**< Pointer to Modulus qInv in 32-bit word array */
  uint32_t*           pM;            /**< Pointer to Message (Base) M in 32-bit word array */
  uint32_t            numOddPowers;  /**< Specify number of pre-calculated odd powers to use */
  uint32_t*           pResult;       /**< Pointer to the 32-bit word buffer to store the operation result */
} Sa_PkaModExpCRTParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Modular Inversion Operation Parameters Structure
 *
 * This structure defines the input and output parameters of PKA Modular Inversion operation as
 * described with the following formula:
 *
 *      1/Z (mod N) ==> R
 *      
 *      Z: ALen
 *      N, R: BLen
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - ModINVp: 0 < ALen <= Max Len (130)  
 *  - ModINVp: 0 < BLen <= Max Len (130)
 *  - ModINV2m: 0 < ALen <= 18
 *  - ModINV2m: 0 < BLen <= 18
 *  - ModINV2m: Z < Modulus N
 *  - Modulus N must be odd (i.e. the least significant bit must be ONE)
 *  - Modulus N should not be 1
 *  - The highest word of the modulus vector, as indicated by BLen, may not be zero.
 *
 */
typedef struct {
  uint32_t*           pN;            /**< Pointer to Modulus N in 32-bit word array */
  uint32_t*           pZ;            /**< Pointer to operand Z in 32-bit word array */
  uint32_t*           pResult;       /**< Pointer to the 32-bit word buffer to store the operation result */
} Sa_PkaModInvParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Field Curve Point Structure
 *
 * This structure specifies a point in projective format (X, Y, Z) on the Prime or Binary Field Curve used by 
 * ECp or EC2m related operations 
 *
 * Where the EC (Elliptic Curve) is represented as y**2=x**3 + ax + b (mod p) and 
 * point in affine format (x,y) = (X/Z, Y/Z).
 *       
 */
typedef struct {
  uint32_t*           pX;       /**< Pointer to X in 32-bit word array */
  uint32_t*           pY;       /**< Pointer to Y in 32-bit word array */
  uint32_t*           pZ;       /**< Pointer to Z in 32-bit word array */
} Sa_PkaECPoint_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Elliptic Curve (EC) Point Add Operation Parameters Structure
 *
 * This structure defines the input and output parameters of Point Add operation on a 
 * Prime (Binary) Field Curve as described below
 *
 *      P1_xyz + P2_xyz ==> P0_xyz, on prime  curve y**2=x**3+ax+b (mod p) or
 *                                     binary curve y**2+xy=x**3+ax**2+b (mod p)
 *      
 *      ALen: not used
 *      p, a, b, P1_x, P1_y, P1_z, P2_x, P2_y, P2_z, P0_x, P0_y, P0_z: BLen
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - Prime Curve: 1 < BLen <= 24 (maximum vector length is 768 bits)
 *  - Binary Curve: 1 < BLen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: Modulus p must be a prime
 *  - Prime Curve: Modulus p must be a prime > 2**63
 *  - The highest word of the modulus vector, as indicated by BLen, may not be zero.
 *  - a < p and b < p
 *  - P1 and P2 must be on the curve
 */
typedef struct {
  uint32_t*           pModP;    /**< Pointer to Modulus p in 32-bit word array */
  uint32_t*           pEcA;     /**< Pointer to EC curve parameter a in 32-bit word array */
  uint32_t*           pEcB;     /**< Pointer to EC curve parameter b in 32-bit word array */
  Sa_PkaECPoint_t     point1;   /**< Pointers to input point 1 (P1_xyz) */
  Sa_PkaECPoint_t     point2;   /**< Pointers to input point 2 (P2_xyz) */
  Sa_PkaECPoint_t     point0;   /**< Pointers to output point 0 (P0_xyz) */
} Sa_PkaECAddParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Elliptic Curve (EC) Point Multiply Operation Parameters Structure
 *
 * This structure defines the input and output parameters of Point Multiply operation on a 
 * Prime (Binary) Field Curve as described below
 *
 *      k*P1_xyz ==> P0_xyz, on prime  curve y**2=x**3+ax+b (mod p) or     where P1_z = 1
 *                              binary curve y**2+xy=x**3+ax**2+b (mod p)
 *      k: Alen
 *      p, a, b (c), P1_x, P1_y, P1_z, P0_x, P0_y, P0_z: BLen
 *
 *      c**2 = b (md p) on the binary curve
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - Prime Curve: 1 < ALen <= 24 (maximum vector length is 768 bits)
 *  - Prime Curve: 1 < BLen <= 24 (maximum vector length is 768 bits)
 *  - Binary Curve: 1 < ALen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: 1 < BLen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: Modulus p must be a prime
 *  - Prime Curve: Modulus p must be a prime > 2**63
 *  - The highest word of the modulus vector, as indicated by BLen, may not be zero.
 *  - a < p and b < p
 *  - P1 must be on the curve
 *  - 1 < k <= n, where n is the curve's order.
 */
typedef struct {
  uint32_t*           pK;       /**< Pointer to parameter k in 32-bit word array */
  uint32_t*           pModP;    /**< Pointer to Modulus p in 32-bit word array */
  uint32_t*           pEcA;     /**< Pointer to EC curve parameter a in 32-bit word array */
  uint32_t*           pEcBC;    /**< Pointer to EC curve parameter b (or c) in 32-bit word array */
  Sa_PkaECPoint_t     point1;   /**< Pointers to input point 1 (P1_xyz) */
  Sa_PkaECPoint_t     point0;   /**< Pointers to output point 0 (P0_xyz) */
} Sa_PkaECMulParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Elliptic Curve (EC) Point Scale Operation Parameters Structure
 *
 * This structure defines the input and output parameters of Point Scale operation on a 
 * Prime (Binary) Field Curve as described below
 *
 *      P1_xyz ==> P0_xyz, on prime  curve y**2=x**3+ax+b (mod p) or     where P0_z = 1
 *                            binary curve y**2+xy=x**3+ax**2+b (mod p)
 *      Alen (not used)
 *      p, a, b, P1_x, P1_y, P1_z, P0_x, P0_y, P0_z: BLen
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - Prime Curve: 1 < BLen <= 24 (maximum vector length is 768 bits)
 *  - Binary Curve: 1 < BLen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: Modulus p must be a prime
 *  - Prime Curve: Modulus p must be a prime > 2**63
 *  - The highest word of the modulus vector, as indicated by BLen, may not be zero.
 *  - a < p and b < p
 *  - P1 must be on the curve
 */
typedef struct {
  uint32_t*           pModP;        /**< Pointer to Modulus p in 32-bit word array */
  uint32_t*           pEcA;         /**< Pointer to equation parameter a in 32-bit word array */
  uint32_t*           pEcB;         /**< Pointer to equation parameter b in 32-bit word array */
  Sa_PkaECPoint_t     point1;       /**< Pointers to input point 1 (P1_xyz) */
  Sa_PkaECPoint_t     point0;       /**< Pointers to output point 0 (P0_xyz) */
} Sa_PkaECScaleParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Elliptic Curve (EC) DSA Sign Operation Parameters Structure
 *
 * This structure defines the input and output parameters of DSA Sign operation on a 
 * Prime (Binary) Field Curve as described below
 *
 *      k*P1_xyz ==> P0_xyz, on prime  curve y**2=x**3+ax+b (mod p) or     where P1_z = 1
 *                              binary curve y**2+xy=x**3+ax**2+b (mod p)
 *      k: Alen
 *      p, a, b (c), P1_x, P1_y, P1_z, P0_x, P0_y, P0_z: BLen
 *
 *      c**2 = b (md p) on the binary curve
 *
 *      N: multiplicative (integer) order of the point P1 means that n times P1 = O
 *      h: messgae hash truncated to n bit Hash(m)n
 *      d: private key
 *
 *      Output:
 *      r = P0x mod n;
 *      s = (1/k)(h + dr) mod N
 *
 * @par  
 *        
 * Restrictions of input parameters:
 *  - Prime Curve: 1 < ALen <= 24 (maximum vector length is 768 bits)
 *  - Prime Curve: 1 < BLen <= 24 (maximum vector length is 768 bits)
 *  - Binary Curve: 1 < ALen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: 1 < BLen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: Modulus p must be a prime
 *  - Prime Curve: Modulus p must be a prime > 2**63
 *  - The highest word of the modulus vector, as indicated by BLen, may not be zero.
 *  - a < p and b < p
 *  - P1 must be on the curve
 *  - 1 < k <= n, where n is the curve's order.
 */
typedef struct {
  uint32_t*           pK;       /**< Pointer to parameter k in 32-bit word array */
  uint32_t*           pModP;    /**< Pointer to Modulus p in 32-bit word array */
  uint32_t*           pEcA;     /**< Pointer to EC curve parameter a in 32-bit word array */
  uint32_t*           pEcBC;    /**< Pointer to EC curve parameter b (or c) in 32-bit word array */
  uint32_t*           pN;       /**< Pointer to multiplicative (integer) order N in 32-bit word array */
  uint32_t*           pH;       /**< Pointer to hash data (h) in 32-bit word array */
  uint32_t*           pD;       /**< Pointer to private key (d) in 32-bit word array */
  Sa_PkaECPoint_t     point1;   /**< Pointers to input point 1 (P1_xyz) */
  uint32_t*           pR;       /**< Pointer to output parameter r in 32-bit word array */
  uint32_t*           pS;       /**< Pointer to output parameter s in 32-bit word array */
} Sa_PkaECDSASignParams_t; 

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Elliptic Curve (EC) DSA Verify Operation Parameters Structure
 *
 * This structure defines the input and output parameters of ECDSA Verify operation on a
 * Prime (Binary) Field Curve as described below
 *
 *      k*P1_xyz ==> P0_xyz, on prime  curve y**2=x**3+ax+b (mod p) or     where P1_z = 1
 *                              binary curve y**2+xy=x**3+ax**2+b (mod p)
 *      k: Alen
 *      p, a, b (c), P1_x, P1_y, P1_z, P0_x, P0_y, P0_z: BLen
 *
 *      c**2 = b (md p) on the binary curve
 *
 *      N: multiplicative (integer) order of the point P1 means that n times P1 = O
 *      h: messgae hash truncated to n bit Hash(m)n
 *      d: private key
 *
 *      Output:
 *      r = P0x mod n;
 *      s = (1/k)(h + dr) mod N
 *
 * @par
 *
 * Restrictions of input parameters:
 *  - Prime Curve: 1 < ALen <= 24 (maximum vector length is 768 bits)
 *  - Prime Curve: 1 < BLen <= 24 (maximum vector length is 768 bits)
 *  - Binary Curve: 1 < ALen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: 1 < BLen <= 18 (maximum vector length is 571 bits)
 *  - Binary Curve: Modulus p must be a prime
 *  - Prime Curve: Modulus p must be a prime > 2**63
 *  - The highest word of the modulus vector, as indicated by BLen, may not be zero.
 *  - a < p and b < p
 *  - P1 must be on the curve
 *  - 1 < k <= n, where n is the curve's order.
 */
typedef struct {
  uint32_t*           pModP;    /**< Pointer to Modulus p in 32-bit word array */
  uint32_t*           pEcA;     /**< Pointer to EC curve parameter a in 32-bit word array */
  uint32_t*           pEcBC;    /**< Pointer to EC curve parameter b (or c) in 32-bit word array */
  uint32_t*           pN;       /**< Pointer to multiplicative (integer) order N in 32-bit word array */
  uint32_t*           pH;       /**< Pointer to hash data (h) in 32-bit word array */
  Sa_PkaECPoint_t     pY;       /**< Pointer to public key (y) in 32-bit word array */
  Sa_PkaECPoint_t     point1;   /**< Pointers to input point 1 (P1_xyz) */
  uint32_t*           pR;       /**< Pointer to signature parameter r in 32-bit word array */
  uint32_t*           pS;       /**< Pointer to signature parameter s in 32-bit word array */
  uint32_t*           pRes;     /**< Pointer to output parameter Result (res) in 32-bit word array */
} Sa_PkaECDSAVerifyParams_t;

/**
 *  @defgroup PkaOpModes PKA Operation Modes
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name PKA Operation Modes
 *
 *  Definition of PKA Operation Modes
 *  Parameter mode of @ref Sa_PkaReqInfo2_t should be set to one of these modes.
 */ 
/*@{*/
typedef enum {
  /**
   *  The API Sa_pkaOperation2() operates in blocking mode. The API call will not return until all specified operations
   *  are completed or some error occurs.
   */
  sa_PKA_MODE_WAIT = 0,  
  
  /**
   *  The API Sa_pkaOperation2() operates in non-blocking mode. The API call will return immediately when the first PKA
   *  operation is triggered or some input error occurs. The application should invoke API @ref Sa_pkaPoll() periodically 
   *  until all specified PKA operations are completed or PKA error occurs. 
   * 
   */
  sa_PKA_MODE_POLL,      
  
  /**
   *  The API Sa_pkaOperation2 operates in non-blocking mode. The API call will return immediately when the first PKA
   *  operation is triggered or some input error occurs. The application should invoke API @ref Sa_pkaPoll() when
   *  PKA interrupt occurs until all specified PKA operations are completed or PKA error occurs. 
   * 
   */
  sa_PKA_MODE_INT      
  
} Sa_PkaOpModes_t;
/*@}*/
/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SALLD PKA Operation Request Structure for second generation PKA module
 *
 * This structure defines PKA request information which is used to perform the basic 
 * and complex large vector arithmetic operations on the second generation PKA.
 *
 */
typedef struct {
  Sa_PkaOpTypes_t     operation;        /**< Specify the arithmetic operation as defined at salldPkaOpTypes_t */
  Sa_PkaOpModes_t     mode;             /**< Specify the operation mode as defined at salldPkaOpModes_t */
  uint16_t            aLen;             /**< Specify the parameter ALen in 32-bit words */
  uint16_t            bLen;             /**< Specify the parameter BLen in 32-bit words */
  int                 statusCode;       /**< Status code of the complex operation as defined at PkaOpStatusCodes */
  union {
    Sa_PkaBasicOpParams_t   basicOp;    /**< Specify the input/output parameters for all basic PKA operations */
    Sa_PkaModExpParams_t    modExp;     /**< Specify the input/output parameters for modular exponentitation operation */
    Sa_PkaModExpCRTParams_t modExpCRT;  /**< Specify the input/output parameters for modular exponentitation CRT operation */
    Sa_PkaModInvParams_t    modInv;     /**< Specify the input/output parameters for modular inversion operation */
    Sa_PkaECAddParams_t     ecAdd;      /**< Specify the input/output parameters for EC point add operation */
    Sa_PkaECMulParams_t     ecMul;      /**< Specify the input/output parameters for EC point multiply operation */
    Sa_PkaECScaleParams_t   ecScale;    /**< Specify the input/output parameters for EC point scale operation */
    Sa_PkaECDSASignParams_t ecDSASign;  /**< Specify the input/output parameters for ECDSA Sign operation */
    Sa_PkaECDSAVerifyParams_t ecDSAVerify; /**< Specify the input/output parameters for ECDSA Sign operation */
  } params;                             /**< Specify the operation specific input/output parameters */
} Sa_PkaReqInfo2_t; 

/**
 *  @defgroup SALLDRngConfigCtrlInfo  SALLD RNG Configuration Control Info Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name RNG Configuration Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in @ref Sa_RngConfigParams_t. 
 */ 
/*@{*/
/**
 *  @def  sa_RNG_CTRL_REINIT
 *        Control Info -- Set: Force re-initialization
 *                        Clear: Perform initialization only if not actived
 */
#define sa_RNG_CTRL_REINIT        0x0001 
/**
 *  @def  sa_RNG_CTRL_ENABLE_INT
 *        Control Info -- Set: Interrupt mode
 *                        Clear: Poll mode 
 */
#define sa_RNG_CTRL_ENABLE_INT    0x0002 
/**
 *  @def  sa_RNG_CTRL_RESET
 *        Control Info -- Set: RNG reset
 */
#define sa_RNG_CTRL_RESET         0x0004 

/*@}*/


/**
 *  @defgroup SALLDRng2ConfigCtrlInfo  SALLD RNG2 Configuration Control Info Bit Definitions
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name RNG2 Configuration Control Info Bit Definitions
 *
 *  Bitmap definition of the ctrlBitfield in @ref Sa_Rng2ConfigParams_t. 
 */ 
/*@{*/
/**
 *  @def  sa_RNG2_CTRL_REINIT
 *        Control Info -- Set: Force re-initialization
 *                        Clear: Perform initialization only if not actived
 */
#define sa_RNG2_CTRL_REINIT        0x0001

/**
 *  @def  sa_RNG2_CTRL_RESET
 *        Control Info -- Set: RNG2 reset
 */
#define sa_RNG2_CTRL_RESET         0x0002

/**
 *  @def  sa_RNG2_CTRL_DRBG_USE
 *        Control Info -- Set: DRBG use for SA2UL
 *                        Clear: No DRBG for SA2UL
 */
#define sa_RNG2_CTRL_DRBG_USE      0x0004

/**
 *  @def  sa_RNG2_CTRL_DRBG_KNOWN_TESTS
 *        Control Info -- Set: DRBG use for SA2UL
 *                        Clear: No DRBG for SA2UL
 */
#define sa_RNG2_CTRL_DRBG_KNOWN_TESTS      0x0008


/*@}*/

/** @} */

/**
 * @ingroup salld_api_structures
 * @brief SALLD RNG Configuration Structure
 *
 * Data structure defines the RNG configuration related parameters
 *
 */
typedef struct {
  uint16_t    ctrlBitfield;    /**< Specify the initialization mode and other control information as defined 
                                    at @ref SALLDRngConfigCtrlInfo */
  uint16_t    clockDiv;        /**< Specify the clock divider (1, 16) 0: default */                                  
  uint32_t    startupCycles;   /**< Specify the number of clock cycles (2**8, 2**24) to start up
                                    the random number generator initially(0:default) */ 
  uint32_t    minRefillCycles; /**< Specify the minimum number of clock cycles (2**6, 2**14) to generate
                                    the next random number (0:default) */ 
  uint32_t    maxRefillCycles; /**< Specify the maximum number of clock cycles (2**8, 2**24) to generate
                                    the next random number (0:default) */
} Sa_RngConfigParams_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD RNG2 Configuration Structure
 *
 * Data structure defines the RNG2 configuration related parameters
 *
 */
typedef struct {
    uint16_t    ctrlBitfield;    /**< Specify the initialization mode and other control information as defined 
                                      at @ref SALLDRng2ConfigCtrlInfo */
    uint16_t    clockDiv;        /**< Specify the clock divider (1, 16) 0: default */
    uint32_t    sampleCycles;    /**< Sets the number of FRO samples that are XOR-ed together into one bit 
                                      to be shifted into the main shift register.
                                      This value must be such that there is at least one bit of entropy (in total)
                                      in each 8 bits that are shifted. 
                                      Note: Value 0 in this field selects 65536 FRO samples to be XOR-ed together */
    uint32_t   pStringLen;            /** < Personalization string length in 32-bit words (valid values 1 thorugh 12) */
    uint32_t   pStringData[sa_MAX_PS_AI_WORD_COUNT]; /** < Personalization String and Additional Input for SP-800-90A
                                                     AES-256 DRBG (valid when DRBG is enabled */

} Sa_Rng2ConfigParams_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD RNG Output Structure
 *
 * This structure defines the random number output provided upon request
 * with @ref Sa_getRandomNum().
 */
typedef struct {
  uint32_t   hi;         /**< Upper 32 bits of the lower 64-bit Random Number output */
  uint32_t   lo;         /**< Lower 32 bits of the lower 64-bit Random Number output */
} Sa_RngData_t;

/**
 * @ingroup salld_api_structures
 * @brief SALLD RNG2 Output Structure
 *
 * This structure defines the random number output provided upon request
 * with @ref Sa_getRandomNum2().
 */
typedef struct {
  uint32_t   hihi;       /**< Upper 32 bits of the higher 64-bit Random Number output */
  uint32_t   hilo;       /**< Lower 32 bits of the higher 64-bit Random Number output */
  uint32_t   hi;         /**< Upper 32 bits of the lower 64-bit Random Number output */
  uint32_t   lo;         /**< Lower 32 bits of the lower 64-bit Random Number output */
} Sa_Rng2Data_t;


/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maximum number of buffers the module (SALLD) can request
 *
 */
#define sa_N_BUFS           1 
                          
/**
 *  @defgroup  saBufIndex SA System Memory Buffer Index
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name   SA System Memory Buffer Index
 *  @brief  Define the buffer index of the SA system memory blocks.
 *
 */
/* @{ */
/**
 *  @def  sa_SYS_BUF_INST
 *        SA LLD system instance buffer
 */
#define sa_SYS_BUF_INST         0

/*  @}  */  
/** @} */

/** @name COMMON (Common Interface) APIs
 *  
 */
/*@{*/
                           
/**
 *  @ingroup salld_api_functions
 *  @brief Sa_getBufferReq returns the memory requirements for the SALLD instance.
 *
 *  @details This function returns the memory buffer requirements in terms of the size and 
 *           alignment array. The SA LLD requires only one memory block as described below:
 *           - SA Instance: SA instance data
 *
 *  @param[in]   sizeCfg     Size configuration information
 *  @param[out]  sizes       Array of size requirements
 *  @param[out]  aligns      Array of alignment requirements
 *  @retval                  Value @ref salldRetCodes
 *
 *  @sa Sa_SizeCfg_t
 */
int16_t  Sa_getBufferReq (Sa_SizeCfg_t *sizeCfg, int sizes[], int aligns[]);

/**
 *  @ingroup salld_api_functions
 *  @brief  Sa_create creates the SA LLD instance. It initializes the SALLD instance 
 *          and its corresponding instance structure based on channel configuration data
 *          such as the call-out table, and etc.
 *
 *  @param[in]  cfg     Configuration information
 *  @param[in]  bases   Array of the memory buffer base addresses 
 *  @param[out] pHandle Instance handle. This is a pointer to an initialized
 *                      instance structure. 
 *  @retval             Value @ref salldRetCodes
 *
 *  @sa Sa_Config_t
 *
 *  @note The system buffers should be allocated from global accessible memory blocks and global
 *        addresses should be used if the LLD instance is expected to be shared among multiple cores. 
 *
 */
int16_t Sa_create (Sa_Config_t *cfg, void* bases[], Sa_Handle *pHandle);

/**
 *  @ingroup salld_api_functions
 *  @brief Sa_start activates the SA LLD instance 
 *
 *  @details This function activates the SA LLD instance. This function should
 *           be called once at all other DSP cores which share the same SALLD
 *           instance. 
 *         
 *  @param[in]  handle  The PA LLD instance identifier
 *  @param[in]  cfg     Configuration information
 *  @retval             Value @ref salldRetCodes
 *
 *  @note: It is optional to call the function at the DSP core where Sa_create() is
 *         invoked.
 */
int16_t  Sa_start  (Sa_Handle handle, Sa_Config_t *cfg);

/**
 *  @ingroup salld_api_functions
 *  @brief Sa_getShadowHandle queries the shadow system handle. 
 *
 *  @details This function returns the shadow system handle. The shadow system handle
 *           is set to NULL if it does not exist. Refer to @ref appendix2 for details
 *         
 *  @param[in]  handle  The PA LLD instance identifier
 *  @param[in]  shandle Pointer to the shadow system handle
 *  @retval             Value @ref salldRetCodes
 *
 */
int16_t  Sa_getShadowHandle  (Sa_Handle handle, Sa_Handle *shandle);


/**
 *  @ingroup salld_api_functions
 *  @brief Sa_close deactivates the SA LLD instance 
 *
 *  @details This function deactivates the SA LLD instance, all the associated
 *           memory buffers can be freed after this call. 
 *         
 *  @param[in]  handle  The PA LLD instance identifier
 *  @param[out] bases   Array of the memory buffer base addresses 
 *  @retval             Value @ref salldRetCodes
 */
int16_t  Sa_close  (Sa_Handle handle, void* bases[]);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function obtains SALLD system statistics. 
 *
 *  @param[in]      handle    SALLD instance identifier.
 *  @param[in, out] stats	  pointer to system statistics 
 *  @retval                   Value @ref salldRetCodes         
 */
int16_t  Sa_getSysStats (Sa_Handle handle, Sa_SysStats_t *stats);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function controls the reset state of the SA Sub-System
 *
 *  @details  This function is used to assert or release reset for the sub-system. Asserting reset does not
 *            reset any of the sub-system internal data, but only the packet processing modules. To acheive 
 *            a complete system reset the system-level reset must be asserted through the power controller.
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  newState    Value @ref salldSubSysStates
 *  @retval                 Value @ref salldSubSysStates
 *  @pre                    None
 */
Sa_State_t Sa_resetControl (Sa_Handle handle, Sa_State_t newState);
                             
/**
 *  @ingroup salld_api_functions
 *  @brief  This function downloads a PDSP image to a PDSP core within the SA sub-system.
 *
 *  @details This function is used to download an executable image to a packet processing module.
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  modId       The PDSP number (0-1)
 *  @param[in]  image       The image to download
 *  @param[in]  sizeBytes   The size of the image
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    The packet processing modules must be in reset. See @ref Sa_resetControl.
 */
int16_t Sa_downloadImage (Sa_Handle handle, int modId, void* image, int sizeBytes);

/**
 *  @ingroup salld_api_functions
 *  @brief  The function is called to initialize and configure the RNG (Random Number Generator) module 
 *          inside SA
 *
 *  @remark   For a multi-core device, it is up to the upper-layer application to make sure that only 
 *            the master core performs the RNG hardware initialization. 
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  cfg         Pointer the RNG configuration parameters as defined at @ref Sa_RngConfigParams_t
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    None
 */
int16_t Sa_rngInit (Sa_Handle handle, Sa_RngConfigParams_t* cfg);

/**
 *  @ingroup salld_api_functions
 *  @brief  The function is called to initialize and configure the RNG (Random Number Generator) module 
 *          inside SA2UL
 *
 *  @remark   For a multi-core device, it is up to the upper-layer application to make sure that only 
 *            the master core performs the RNG hardware initialization. 
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  cfg         Pointer the RNG configuration parameters as defined at @ref Sa_RngConfigParams_t
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    None
 */
int16_t Sa_rng2Init (Sa_Handle handle, Sa_Rng2ConfigParams_t* cfg);


/**
 *  @ingroup salld_api_functions
 *  @brief  This function returns a 64-bit true random number
 *
 *  @details  This function is called to get a 64-bit true random number. It is a non-blocking function 
 *            call which indicates the random number is not available if the RNG is still in the 
 *            process of generating the next random number.  
 *  @remark   For a multi-core device, it is up to the application to prevent this function from being invoked 
 *            by multiple CGEM cores simultaneously.
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  f_isr       Flag to indicate whether it is called from interrupt srevice routine
 *  @param[in]  rnd         Pointer to the 64-bit random number
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    RNG is initialized
 */
int16_t Sa_getRandomNum (Sa_Handle handle, uint16_t f_isr, Sa_RngData_t* rnd);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function returns a 128-bit true random number
 *
 *  @details  This function is called to get a 64-bit true random number. It is a non-blocking function 
 *            call which indicates the random number is not available if the RNG is still in the 
 *            process of generating the next random number.  
 *  @remark   For a multi-core device, it is up to the application to prevent this function from being invoked 
 *            by multiple CGEM cores simultaneously.
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  f_isr       Flag to indicate whether it is called from interrupt srevice routine
 *  @param[in]  rnd         Pointer to the 128-bit random number
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    RNG is initialized
 */
int16_t Sa_getRandomNum2 (Sa_Handle handle, uint16_t f_isr, Sa_Rng2Data_t* rnd);

/**
 *  @ingroup salld_api_functions
 *  @brief Sa_rngClose decativates the SA RNG module 
 *
 *  @details This function deactivates the SA RNG module and clears LLD internal state. 
 *         
 *  @param[in]  handle  The PA LLD instance identifier
 *  @retval             Value @ref salldRetCodes
 */
int16_t  Sa_rngClose  (Sa_Handle handle);

/**
 *  @ingroup salld_api_functions
 *  @brief Sa_rngClose decativates the SA RNG module 
 *
 *  @details This function deactivates the SA RNG module and clears LLD internal state. 
 *         
 *  @param[in]  handle  The PA LLD instance identifier
 *  @retval             Value @ref salldRetCodes
 */
int16_t  Sa_rng2Close  (Sa_Handle handle);


/**
 *  @ingroup salld_api_functions
 *  @brief  This function initializes the PKA (Public Key Accelerator) module inside SA
 *
 *  @details  The function is called to initialize the PKA module inside SA. 
 *  @remark   For a multi-core device, it is up to the upper-layer application to make sure that only 
 *            the master core performs the PKA hardware initialization. 
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    None
 */
int16_t Sa_pkaInit (Sa_Handle handle);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function downloads the PKA firmware image to the second generation PKA module.
 *
 *  @details This function is used to download an executable firmware image to the second
 *           generation PKA module, triggers the firmware to run and verify its operation. 
 *           The PKA firmware is required for PKA to perform complex operations such as 
 *           modular exponentiation and etc.
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  image       The image to download
 *  @param[in]  sizeWords   The size of the image in 32-bit words 
 *  @param[out] pVersion    The pointer to PKA firmware reversion number
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    The PKA module has been initialized through API @ref Sa_pkaInit.
 */
int16_t Sa_pkaDownloadImage (Sa_Handle handle, void* image, int sizeWords, uint32_t* pVersion);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function triggers a large vector arithmetic operation through the PKA module
 *
 *  @details  The function is called to perform a large vector arithmetic operation through the PKA module. 
 *            It is considered as a blocking function call since it will wait for the PKA module to complete 
 *            the arithmetic operation. However, it only takes a few cycles for PKA to complete any operation. 
 *            This function also returns with error code immediately if the PKA is still in the process to 
 *            perform the previous operation or when timeout occurs.
 *  @remark   For a multi-core device, it is up to the application to prevent this function from being invoked 
 *            by multiple CGEM cores simultaneously.
 *  @note     This API does not support the second generation PKA module
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  pkaReqInfo  Pointer to the PKA operation request information structure
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    PKA is initialized
 */
int16_t Sa_pkaOperation (Sa_Handle handle, Sa_PkaReqInfo_t* pkaReqInfo);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function triggers a large vector arithmetic operation through the PKA module
 *
 *  @details  Sa_pkaOperation2 is the next generation of PKA API to replace @ref Sa_pkaOperation eventually. 
 *            This new API supports both the first generation and second generation of PKA module.
 *            The function is called to perform basic or complex large vector arithmetic operations through 
 *            the PKA module. It can operate in either blocking or non-blocking mode. In blocking mode, this API 
 *            will not return until the PKA module completes all requested large vector arithmetic operations.
 *            In non-blocking mode, this API will returns immediately after it sets up and starts the first
 *            requested PKA operation. And then the API @ref Sa_pkaPoll() should be invoked periodically or 
 *            per PKA interrupt until all PKA operations are completed and the final results are available. 
 *            This function also returns with error code immediately if the PKA is still in the process to 
 *            perform the previous operation or when timeout occurs.
 *
 *  @note     All simple operations should be done in blocking mode since it will only take a few cycles to
 *            complete those operaion. 
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  pPkaReqInfo Pointer to the PKA operation request information structure
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    PKA is initialized. The complex operation will not work until the firmware image
 *                          is downloaded through API @ref Sa_pkaDownloadImage.
 */
int16_t Sa_pkaOperation2 (Sa_Handle handle, Sa_PkaReqInfo2_t* pPkaReqInfo);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function checks the status of the current large vector arithmetic operation and continue 
 *          the operation accordingly..
 *
 *  @details  Sa_pkaPoll is used to support non-blocking PKA operations. It should be called periodically 
 *            in Poll mode or per PKA interrupt in Interrupt mode. When it is invoked, this function checks
 *            the status of the current PKA operation. If the operation is still in progress, it just 
 *            returns sa_PKA_OP_IN_PROGRESS. If the operation is complete and more operation is required, 
 *            it will prepare and trigger the next (complex) operation and then return sa_PKA_OP_CONTINUE, 
 *            otherwise, it copies operation results from the PKA registers and vector RAM to the result 
 *            locations specified by the PKA request information data structure and return sa_PKA_OP_COMPLETE. 
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[out] pPkaReqInfo Pointer to the local copy of PKA operation request information structure
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    PKA is initialized and Sa_pkaOperation2 has been invoked in non-block mode. 
 */
int16_t Sa_pkaPoll (Sa_Handle handle, Sa_PkaReqInfo2_t* pPkaReqInfo);

/**
 *  @ingroup salld_api_functions
 *  @brief Sa_pkaClose decativates the SA PKA module 
 *
 *  @details This function deactivates the SA PKA module and clears LLD internal state. 
 *         
 *  @param[in]  handle  The PA LLD instance identifier
 *  @retval             Value @ref salldRetCodes
 */
int16_t  Sa_pkaClose  (Sa_Handle handle);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function returns the SA system ID associated with the specified handle
 *
 *  @details  The function is called to request the SA system ID corresponding to the handle. 
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @retval                 SA system ID
 *  @pre                    Sa_create function should be called before calling this function
 */
uint32_t Sa_getID (Sa_Handle handle);

/**
 *  @ingroup salld_api_functions
 *  @brief  Sa_getVersion returns the SA LLD version information
 *
 *  @details This function is used to get the version information of the SA LLD in 0xAABBCCDD format.
 *           where Arch (AA); API Changes (BB); Major (CC); Minor (DD)
 *
 *  @retval                32-bit version information
 */
uint32_t Sa_getVersion (void);


/**
 *  @ingroup salld_api_functions
 *  @brief  Sa_getVersionStr returns the SA LLD version string
 *
 *  @details This function is used to get the version string of the SA LLD.
 *
 *  @retval                Version string
 */
const char* Sa_getVersionStr (void);

/**
 *  @ingroup salld_api_functions
 *  @brief  Sa_getPDSPVersion returns the SA PDSP version information.
 *
 *  @details This function is used to get the SA PDSP version information in 0xAABBCCDD format.
 *           where Arch (AA); API Changes (BB); Major (CC); Minor (DD
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  modId       The PDSP number (0-1)
 *  @param[out] pVersion    The pointer to PDSP version number
 *  @retval                 Value @ref salldRetCodes
 *  @pre                    The PDSP image should be downloaded successfully.
 *
 */
int16_t Sa_getPDSPVersion (Sa_Handle handle, int modId, uint32_t *pVersion);

/*@}*/ /* @name COMMON APIs */

/**
 *  @ingroup salld_api_constants
 *  @brief   Define the maximum number of buffers each SA LLD channel can request
 *
 */
#define sa_CHAN_N_BUFS           1 

/**
 *  @defgroup  saChanBufIndex SA Channel Memory Buffer Index
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name   SA Channel Memory Buffer Index
 *  @brief  Define the buffer index of the SA channel memory blocks.
 *
 */
/* @{ */
/**
 *  @def  sa_CHAN_BUF_INST
 *        SA LLD channel instance buffer
 */
#define sa_CHAN_BUF_INST         0

/*  @}  */  
/** @} */


/** @name CHANNEL (Channel-Specific) APIs
 *  
 */
/*@{*/

/**
 *  @ingroup salld_api_functions
 *  @brief Sa_chanGetBufferReq returns the memory requirements for an SALLD channel
 *
 *  @details This function returns the memory buffer requirements in terms of the size and 
 *           alignment array. The SA Channel requires only one memory block as described below:
 *           - SA Channel Instance: SA channel instance data
 *
 *  @param[in]   sizeCfg     Size configuration information
 *  @param[out]  sizes       Array of size requirements
 *  @param[out]  aligns      Array of alignment requirements
 *  @retval                  Value @ref salldRetCodes
 */
int16_t Sa_chanGetBufferReq (Sa_ChanSizeCfg_t *sizeCfg, int sizes[], int aligns[]);

/**
 *  @ingroup salld_api_functions
 *  @brief  Sa_chanCreate creates the SALLD channel. It initializes an instance of SALLD 
 *          channel and its corresponding instance structure based on channel
 *          configuration data such as the security protocol, and etc. 
 *  
 *  @param[in]  handle      SALLD instance identifier.
 *  @param[in]  cfg         Configuration information
 *  @param[in]  bases       Array of the memory buffer base addresses 
 *  @param[out] pChanHdl    Channel handle. This is a pointer to an initialized
 *                          instance structure. 
 *  @retval                 Value @ref salldRetCodes
 *
 *  @note The channel buffers should be allocated from global accessible memory blocks and global
 *        addresses should be used if this channel is expected to be shared among multiple cores. 
 */
int16_t Sa_chanCreate (Sa_Handle handle, Sa_ChanConfig_t *cfg, void* bases[], 
                       Sa_ChanHandle *pChanHdl);
                       
/**
 *  @ingroup salld_api_functions
 *  @brief Sa_chanClose decativates the SALLD channel. It clears the SALLD channel instance. 
 *         All the associated memory buffers can be freed after this call. 
 *
 *  @param[in]  handle  SALLD channel instance identifier
 *  @param[out] bases   Array of the memory buffer base addresses 
 *  @retval             Value @ref salldRetCodes
 */
int16_t Sa_chanClose (Sa_ChanHandle handle, void* bases[]);

 /**
 *  @ingroup salld_api_functions
 *  @brief This function controls the operations of a channel instance of SALLD. It is
 *         used to configure and/or re-configure the SALLD channel with various control
 *         information. This function should be called multiple times to configure and 
 *         activate the SALLD channel during the call setup period. Reconfiguration is
 *         applicable for SRTP for re-key operations and there is no reconfiguration 
 *         supported for IPSec and Air Ciphering modes.  
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   chanCtrlInfo Pointer to SALLD channel control config structure.
 *  @retval                   Value @ref salldRetCodes         
 *
 *  @sa Sa_ChanCtrlInfo_t
 */
int16_t  Sa_chanControl (Sa_ChanHandle handle, Sa_ChanCtrlInfo_t *chanCtrlInfo);

/**
 *  @ingroup salld_api_functions
 *  @brief This function activates the local channel instance at other DSP cores. It should 
 *         be called once at any other DSP cores which invoke the same SALLD channel that
 *         is created and configured at the master core. It is up to the master core
 *         to make the channel handle public and accessible by other DSP cores after
 *         channel creation and configuration.
 *         The following APIs are supported at slave cores.
 *          - Sa_chanReceiveData
 *          - Sa_chanSendData
 *          - Sa_chanGetStats
 *          - Sa_chanGetID 
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @retval                   Value @ref salldRetCodes         
 *
 *  @note: This function should be called prior to other channel APIs
 *         such as Sa_chanSendData() and Sa_chanReceiveData() are invoked
 *  
 */
int16_t  Sa_chanStart (Sa_ChanHandle handle);

/**
 *  @ingroup salld_api_functions
 *  @brief Sa_chanGetShadowHandle queries the shadow channel handle. 
 *
 *  @details This function returns the shadow channel handle. The shadow channel handle
 *           is set to NULL if it does not exist. Refer to @ref appendix2 for details
 *         
 *  @param[in]  handle  The PA LLD channel instance identifier
 *  @param[in]  shandle Pointer to the shadow channel handle
 *  @retval             Value @ref salldRetCodes
 *
 */
int16_t  Sa_chanGetShadowHandle  (Sa_ChanHandle handle, Sa_ChanHandle *shandle);

/**
 *  @ingroup salld_api_functions
 *  @brief This utility function processes packets received from the network. It performs IPSec ESP specific
 *         post-SA operations on the decrypted and/or integrity-verified data packet. It also
 *         performs the actual decryption/authentication operation in SW-only mode. 
 *
 *  @param[in]   info         ESP Tx/Rx information pointer.
 *  @param[in] 	 pktDesc      Pointer to the packet descriptor structure.
 *  @retval                   Value @ref salldRetCodes          
 *
 *  @sa Sa_PktDesc_t
 */
int16_t salld_esp_post_proc_util (salldIpsecEspTxRxInfo_t *info,   Sa_PktDesc_t* pktDesc);

/**
 *  @ingroup salld_api_functions
 *  @brief This function processes packets received from the network. It performs protocol-specific
 *         post-SA operations on the decrypted and/or integrity-verified data packet. It also
 *         performs the actual decryption/authentication operation in SW-only mode. 
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in] 	 pktInfo      Pointer to the packet info structure.
 *  @retval                   Value @ref salldRetCodes         
 *
 *  @sa Sa_PktInfo_t
 */
int16_t  Sa_chanReceiveData (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo);


/**
 *  @ingroup salld_api_functions
 *  @brief  This utility function processes the data packet to the networks. It performs IPSec ESP specific
 *          operations to prepare the data packets to be encrypted and/or authenticated by the SA.
 *          It also performs the actual encryption and/or authentication in the SW-only mode
 *  
 *  @param[in]   info         ESP Tx/Rx information pointer.
 *  @param[in] 	 pktDesc      Pointer to the packet descriptor structure.
 *  @retval                   Value @ref salldRetCodes         
 *
 *  @sa Sa_PktDesc_t
 *
 */
int16_t salld_esp_pre_proc_util (salldIpsecEspTxRxInfo_t *info,   Sa_PktDesc_t* pktDesc);

/**
 *  @ingroup salld_api_functions
 *  @brief  This function processes the data packet to the networks. It performs protocol-specific
 *          operations to prepare the data packets to be encrypted and/or authenticated by the SA.
 *          It also performs the actual encryption and/or authentication in the SW-only mode
 *  
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in] 	 pktInfo      Pointer to the packet info structure.
 *  @param[in] 	 clear        Flag indicating whether to force non-encryption
 *  @retval                   Value @ref salldRetCodes         
 *
 *  @sa Sa_PktInfo_t
 *
 */
int16_t  Sa_chanSendData (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo, uint16_t clear);
  

/**
 *  @ingroup salld_api_functions
 *  @brief  This function obtains SALLD channel protocol-specific statistics. 
 *
 *  @param[in]      handle    SALLD channel instance identifier.
 *  @param[in]      flags     Various control flags @ref SALLDStatsCtrlBit 
 *  @param[in, out] stats	  Pointer to protocol-specific statistics 
 *  @retval                   Value @ref salldRetCodes         
 */
int16_t  Sa_chanGetStats (Sa_ChanHandle handle, uint16_t flags, Sa_Stats_t *stats);

/**
 *  @ingroup salld_api_functions
 *  @brief  Sa_chanGetID returns the SA channel ID associated with the specified handle
 *
 *  @details  The function is called to request the SA channel ID corresponding to the handle. 
 *
 *  @param[in]  handle      SALLD instance identifier.
 *  @retval                 Channel ID
 *  @pre                    Sa_chanInit function should be called before calling this function
 */
uint32_t Sa_chanGetID (Sa_ChanHandle handle);

/**
 *  @ingroup salld_api_functions
 *  @brief  Sa_chanGetSwInfo returns the SA-specific software information required for all packets 
 *          to be delivered to SA.
 *
 *  @details  It is an optional utility function to query the SA-specific software information in 
 *            both directions in case Sa_chanSendData() is not used and/or the callout function
 *            ChanRegister() is not implemented.
 *
 *            In normal operation, the from-network SWInfo will be passed to module user via callout
 *            function ChanRegister() and the to-network SWInfo will be passed to module user via API
 *            Sa_chanSendData().
 *
 *  @param[in]   handle       SALLD channel instance identifier.
 *  @param[in]   dir          Packet direction as specified at @ref Sa_PktDir_t
 *  @param[in]   pChanSwInfo  Pointer to SALLD software routing information structure.
 *  @retval                   Value @ref salldRetCodes         
 *
 *  @sa Sa_SWInfo_t
 *
 */
int16_t Sa_chanGetSwInfo (Sa_ChanHandle handle, uint16_t dir, Sa_SWInfo_t* pChanSwInfo);


/*@}*/ /* @name Channel APIs */

/** @name Utility APIs
 *  
 */
/*@{*/

/**
 *  @ingroup salld_api_functions
 *  @brief This function verifies whether the security context buffer has been freed
 *         by SA. 
 *
 *  @param[in] scBuf       Pointer to the Security Context buffer.
 *  @retval                TRUE if buffer is free; FALSE if otherwise         
 *
 */
uint16_t  Sa_isScBufFree (uint8_t* scBuf);

/**
 *  @ingroup salld_api_constants
 *  @brief   Define the core dump collect buffer size in words (32 bit) supported by SASS. 
 */
#define sa_CORE_DUMP_BUF_SIZE                2048

/**
 *  @ingroup salld_api_functions
 *  @brief This function snap shots the internal registers and scratch memory locations 
 *         for furt
 *
 *  @param[in] handle      SA system handle.
 *  @param[in] dbgInfoBuf  Debug information collection buffer pointer
 *  @param[in] bufSize     Debug information collection buffer size 
 *  @retval                TRUE if collection is successful; FALSE if otherwise         
 *
 *  @warning Note that system is halted after @ref Sa_coreDump is initiated. After the call to this API,
 *  it is up to the application software to redownload the firmware and restart SASS.
 *
 *  @note This API is used to provide dump of internal data when requested by TI. 
 *        These can be submitted via e2e.ti.com or your contracted support vehicle.
 *        When there is a JTAG connected to the system for collecting the data,
 *        Please use the fprintf() code provided in the SaUnitTest application to save 
 *        the coredump, to avoid formatting issues.
 *        There is also user space utility saCoreDumpUtil.out provided to collect 
 *        the debug information when requested from linux user space.
 */
int16_t Sa_coreDump(Sa_Handle handle, uint32_t *dbgInfoBuf, uint16_t bufSize);

                           
/*@}*/ /* @name Utility APIs */

/** @name SA LLD Macros
 *  
 */
/*@{*/

/**
 *  @ingroup salld_api_macros
 *  @brief  sa_MK_UINT32 is a simple utility macro which constructs a 32-bit word from 4 input bytes
 * 
 *  @param[in]  a       Input byte 1 
 *  @param[in]  b       Input byte 2
 *  @param[in]  c       Input byte 3
 *  @param[in]  d       Input byte 4
 *            
 */
#define sa_MK_UINT32(a, b, c, d)       (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

/**
 *  
 *  @ingroup salld_api_macros
 *  @brief sa_mDmResetCmdLb: Reset the Command Label for Data Mode
 *
 *  @details Inline macro API can be used to reset the Command Label 
 *           for Data Mode after first time initialization
 *
 *  @param[in]  updateInfo     Command label updating control structure returned from SA LLD
 *  @param[in]  cmdLb          Command Label Buffer
 *
 *  @note: It is just a sample implemenation which should be optimized per use case
 */
static inline
void sa_mDmResetCmdLb(Sa_CmdLbUpdateInfo_t*   updateInfo,
                      uint32_t*               cmdLb)
{

#ifdef NSS_LITE2
    cmdLb++;
#endif

    if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_ENC)
    {
        /* Clear the encSize and encOffset field to simplify the operation in command label updating Macros */
        cmdLb[updateInfo->encSizeInfo.index] &= 0xffff0000UL;
        cmdLb[updateInfo->encOffsetInfo.index] &= 0x00ffffffUL;
    }
    
    if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_AUTH)
    {
        /* Clear the authSize and authOffset field to simplify the operation in command label updating Macros */
        cmdLb[updateInfo->authSizeInfo.index] &= 0xffff0000UL;
        cmdLb[updateInfo->authOffsetInfo.index] &= 0x00ffffffUL;
    }
    
    if ( (updateInfo->subMode == sa_DM_CCM) ||
         (updateInfo->subMode == sa_DM_CCM_GEN) )
    {
        /* Clear AAD length field */
        cmdLb[2] &= 0xffff0000UL;
    
    }
}

/**
 *  
 *  @ingroup salld_api_macros
 *  @brief salld_form_32bits_util: form 32 bits command label from a byte array
 *
 *  @details Inline macro API can be used to generate the 32 bits word array from
 *           byte array as b[0]<<24 | b[1]<<16 | b[2] << 8 | b[3].
 *
 *  @param[in]  cmdLb           Command Label Buffer
 *  @param[in]  index           start index of cmdLb
 *  @param[in]  offset          start offset in the cmdLb 32-bit word 
 *  @param[in]  bPtr            byte array pointer
 *  @param[in]  bSize           byte array size
 *
 */
static inline 
void salld_form_32bits_util(uint32_t* cmdLb, uint8_t index, uint8_t offset, uint8_t* bPtr, uint8_t bSize)
{
  uint32_t temp   = 0;
  int      bIndex = 0;
  int8_t   iShift = 8 *( 3 - offset);
  int      w32Size, bytesInlast32bWord;

  /* number of bytes to be formed in last 32-bit word */
  bytesInlast32bWord = (bSize + offset ) & 3;

  /* number of bytes to be formed into full 32-bit words */
  w32Size = bSize - offset - bytesInlast32bWord;
  
  /* Handle the un-aligned first 32 word that needs update specially */
  if (offset != 0)
  {    
    while (iShift >= 0) {
      temp |= bPtr[bIndex ++] << iShift;
      iShift -= 8;
    }
    cmdLb[index ++] |= temp;
  }

  /* the loop runs for all full 32-bit words */
  while (bIndex < w32Size)
  {
    cmdLb[index ++] = bPtr[bIndex] << 24 | bPtr[bIndex+1] << 16 | \
                      bPtr[bIndex + 2] <<  8 | bPtr[bIndex + 3];
    bIndex += 4;
  }

  /* Handle any un-handled (remaining) bytes towards the end */
  if (bytesInlast32bWord)
  {
    iShift = 24;
    temp = 0;
    while (bytesInlast32bWord) {
      temp |= bPtr[bIndex ++] << iShift;
      iShift -= 8;
      bytesInlast32bWord --;
    }
    cmdLb[index] = temp;
  }
}

/**
 *  
 *  @ingroup salld_api_macros
 *  @brief sa_mDmUpdateCmdLb_ccm_gen: Update the command label for Data Mode in CCM general case
 *
 *  @details Inline macro API can be used to update the command label per packet 
 *           for Data Mode during Encryption/Decryption request to SA.
 *
 *  @param[in]  encOffset       Offset to the encrypted/decrypted data in the 
 *                              packet in bytes 
 *  @param[in]  encSize         Number of bytes to be encrypted or decrypted 
 *  @param[in]  pEncIV          Initialization vectors if applicable for 
 *                              encryption mode. 
 *  @param[in]  aadSize         Number of additional data to be authenticated in bytes
 *  @param[in]  aad             Pointer to the additional authenticated data 
 *  @param[in]  pCmdlUpdate     Command label updating control structure returned from SA LLD
 *  @param[in]  cmdLb           Command Label Buffer
 *
 *  @note: It is just a sample implemenation for generic CCM use cases
 *
 */
static inline
void sa_mDmUpdateCmdLb_ccm_gen(uint8_t                 encOffset,
                               uint16_t                encSize,
                               uint8_t*                pEncIV,
                               uint8_t                 aadSize,
                               uint8_t*                aad,
                               Sa_CmdLbUpdateInfo_t*   pCmdlUpdate,                               
                               uint32_t*               cmdLb)
{
    uint8_t *iv  = pEncIV;

#ifdef NSS_LITE2
    cmdLb++;
#endif

    /* Handle Option 1, (Option 2 & Option 3) in two different groups */
    /* Update the command label header (8 byte) */
    cmdLb[0] |= encSize;
    cmdLb[1] |= ((uint32_t)encOffset << 24);

    /* Option 1 Update: if AAD size is 0, no updates to Option 1 */
    if ( (aadSize >   0) &&
         (aadSize <= 14) )
    {
       /* Option 1: B1 (aadlen | AAD ) (16 byte) */
       /* update AAD */
       salld_form_32bits_util(&cmdLb[0], pCmdlUpdate->aadInfo.index, \
                          pCmdlUpdate->aadInfo.offset, &aad[0], aadSize);        
    }
    
    /* Option 2 update IV: B0 (flag | salt | IV | encypted data len) (16 byte) */
    salld_form_32bits_util(&cmdLb[0], pCmdlUpdate->encIvInfo.index, \
                        pCmdlUpdate->encIvInfo.offset, iv, pCmdlUpdate->encIvInfo.size);
    
    cmdLb[9] &= 0xFFFF0000UL; /* Clear the lower 16 bits */
    cmdLb[9] |= encSize;      /* Set with encoder size */

    /* Option 3: AES-CTR IV (salt| IV | 0)  (16 byte) */
    /* Insert IV and encrypted data len */
    /* IV1 & 2*/
    cmdLb[10] = cmdLb[6] & 0x07FFFFFFUL;
    cmdLb[11] = cmdLb[7];    
    cmdLb[12] = cmdLb[8];
    cmdLb[13] = cmdLb[9] & 0xFFFF0000UL; 
}

/**
 *  
 *  @ingroup salld_api_macros
 *  @brief sa_mDmUpdateCmdLb_ccm_wimax: Update the command label for CCM-WiMax
 *
 *  @details Inline macro API can be used to update the command label per packet 
 *           for Data Mode during Encryption/Decryption request to SA.
 *
 *  @param[in]  encOffset       Offset to the encrypted/decrypted data in the 
 *                              packet in bytes 
 *  @param[in]  encSize         Number of bytes to be encrypted or decrypted 
 *  @param[in]  gmh             generic mac header pointer. 
 *  @param[in]  sn              sequence number pointer
 *  @param[in]  cmdLb           Command Label Buffer
 *
 *  @note: It is just a sample implemenation for CCM-WiMax (optimal implementation)
 *
 */
static inline
void sa_mDmUpdateCmdLb_ccm_wimax (
                       uint8_t                 encOffset,
                       uint16_t                encSize,
                       uint8_t*                gmh,        /* 5 bytes (excluding HCS (header check sequence) */
                       uint8_t*                sn,         /* 4 bytes of sequence number */
                       uint32_t*               cmdLb)
{

#ifdef NSS_LITE2
    cmdLb++;
#endif

    cmdLb[0] |= encSize;
    cmdLb[1] |= ((uint32_t)encOffset << 24);
                
    /* AAD (none) */
               
    /* Option 2: B0 (flag | nonce | encypted data len) (16 byte) */
    /* Insert IV and encrypted data len */                
    cmdLb[6] |= ((gmh[0] << 16) | \
                  (gmh[1] << 8)  | \
                  (gmh[2]) );    
    cmdLb[7] = ((gmh[3] << 24) | (gmh[4] << 16));
    cmdLb[8] = ((sn[0] << 8) | (sn[1]));
    cmdLb[9] = ((sn[2] << 24) | (sn[3] << 16) | encSize);

    /* Option 3: AES-CTR IV (flag | nonce | 0)  (16 byte) */                
    cmdLb[10] = cmdLb[6] & 0x07FFFFFFUL;
    cmdLb[11] = cmdLb[7];                
    cmdLb[12] = cmdLb[8];
    cmdLb[13] = cmdLb[9] & 0xFFFF0000UL;
}

/**
 *  
 *  @ingroup salld_api_macros
 *  @brief sa_mDmUpdateCmdLb: Update the command label for Data Mode for IPSEC,SRTP
 *
 *  @details Inline macro API can be used to update the command label per packet 
 *           for Data Mode during Encryption/Decryption request to SA.
 *
 *  @param[in]  encOffset       Offset to the encrypted/decrypted data in the 
 *                              packet in bytes 
 *  @param[in]  encSize         Number of bytes to be encrypted or decrypted 
 *  @param[in]  pEncIV          Initialization vectors if applicable for 
 *                              encryption mode. 
 *  @param[in]  authOffset      Offset to the (To be) authenticated data in the 
 *                              packet in bytes 
 *  @param[in]  authSize        Number of bytes to be authenticated
 *  @param[in]  pAuthIV         Initialization vectors if applicable for 
 *                              authentication modes. 
 *  @param[in]  aadSize         Number of additional data to be authenticated in bytes
 *  @param[in]  aad             Pointer to the additional authenticated data 
 *  @param[in]  pDataBuf        Pointer to data buffer
 *  @param[in]  updateInfo      Command label updating control structure returned from SA LLD
 *  @param[in]  cmdLb           Command Label Buffer
 *
 *  @note: It is just a sample implemenation for below use cases:
 *         IPSEC
 *         SRTP
 *
 */
static inline
void sa_mDmUpdateCmdLb(uint8_t                 encOffset,
                       uint16_t                encSize,
                       uint8_t*                pEncIV,
                       uint8_t                 authOffset,
                       uint16_t                authSize,
                       uint8_t*                pAuthIV,
                       uint8_t                 aadSize,
                       uint8_t*                aad,
                       uint8_t*                pDataBuf,
                       Sa_CmdLbUpdateInfo_t*   updateInfo,
                       uint32_t*               cmdLb)
{
#ifdef NSS_LITE2
   cmdLb++;
#endif

    switch (updateInfo->subMode)
    {
        case sa_DM_GEN:  /* General Mode operation */
            if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_ENC)
            {
                cmdLb[updateInfo->encSizeInfo.index] |= encSize;
                cmdLb[updateInfo->encOffsetInfo.index] |= ((uint32_t)encOffset << 24);
        
                if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_ENC_IV)
                {
                    uint32_t *data = &cmdLb[updateInfo->encIvInfo.index];
                    uint8_t *iv = pEncIV;
            
                    data[0] = sa_MK_UINT32(iv[0], iv[1], iv[2], iv[3]);
                    data[1] = sa_MK_UINT32(iv[4], iv[5], iv[6], iv[7]);
            
                    if (updateInfo->encIvInfo.size > 8)
                    {
                        data[2] = sa_MK_UINT32(iv[8], iv[9], iv[10], iv[11]);
                        data[3] = sa_MK_UINT32(iv[12], iv[13], iv[14], iv[15]);
                    }
                }
            }
    
            if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_AUTH)
            {
                cmdLb[updateInfo->authSizeInfo.index] |= authSize;
                cmdLb[updateInfo->authOffsetInfo.index] |= (authOffset << 24);
        
                if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_AUTH_IV)
                {
                    uint32_t *data = &cmdLb[updateInfo->authIvInfo.index];
                    uint8_t *iv = pAuthIV;
            
                    data[0] = sa_MK_UINT32(iv[0], iv[1], iv[2], iv[3]);
                    data[1] = sa_MK_UINT32(iv[4], iv[5], iv[6], iv[7]);
            
                    if (updateInfo->authIvInfo.size > 8)
                    {
                        data[2] = sa_MK_UINT32(iv[8], iv[9], iv[10], iv[11]);
                        data[3] = sa_MK_UINT32(iv[12], iv[13], iv[14], iv[15]);
                    }
                }
        
                if (updateInfo->validBitfield & sa_CMDL_UPDATE_VALID_AUX_KEY)
                {
        
                    int offset = (authSize & 0xF)?4:0;
                    memcpy(&cmdLb[updateInfo->auxKeyInfo.index],  &updateInfo->auxKey[offset], 16);
                }
            }
            break;
            
            
        case sa_DM_CCM:
            { 
                uint8_t *iv  = pEncIV;
                
                cmdLb[0] |= encSize;
                cmdLb[1] |= ((uint32_t)encOffset << 24);
                
                /* AAD */
                cmdLb[2] |= ((aad[0] << 8) + aad[1]);
                cmdLb[3] = sa_MK_UINT32(aad[2], aad[3], aad[4], aad[5]);
                if (aadSize == 8)
                {
                    cmdLb[4] = (aad[6] << 24) + (aad[7] << 16);
                }
                else
                {
                    cmdLb[4] = sa_MK_UINT32(aad[6], aad[7], aad[8], aad[9]);
                    cmdLb[5] = (aad[10] << 24) + (aad[11] << 16);
                }
                
                /* IV1 & 2*/
                cmdLb[11] = cmdLb[7] = sa_MK_UINT32(iv[0], iv[1], iv[2], iv[3]);
                cmdLb[12] = cmdLb[8] = sa_MK_UINT32(iv[4], iv[5], iv[6], iv[7]);
                
                /* data length 2 */
                cmdLb[9] = encSize;
        
            }
            break;
            
        case sa_DM_GCM:
            { 
        
                uint8_t *iv  = pEncIV;
                
                cmdLb[0] |= encSize;
                cmdLb[1] |= ((uint32_t)encOffset << 24);
                
                /* AAD */
                cmdLb[4] = sa_MK_UINT32(aad[0], aad[1], aad[2], aad[3]);
                cmdLb[5] = sa_MK_UINT32(aad[4], aad[5], aad[6], aad[7]);
                if (aadSize == 12)
                {
                    cmdLb[6] = sa_MK_UINT32(aad[8], aad[9], aad[10], aad[11]);
                }
                
                /* IV */
                cmdLb[9]  = sa_MK_UINT32(iv[0], iv[1], iv[2], iv[3]);
                cmdLb[10] = sa_MK_UINT32(iv[4], iv[5], iv[6], iv[7]);
                
                /* data length 2 */
                cmdLb[3] = encSize << 3;
        
            }
            break;

        case sa_DM_GMAC:
            { 
                uint8_t *iv  = pAuthIV;
                uint8_t *payload =  pDataBuf + authOffset;
                uint8_t dataAppended = 16 - aadSize; /* payload appended to AAD in bytes */
                
                cmdLb[0] |=   authSize  - dataAppended;
                cmdLb[1] |=  ((authOffset + dataAppended) << 24);
                
                /* AAD */
                cmdLb[4] = sa_MK_UINT32(aad[0], aad[1], aad[2], aad[3]);
                cmdLb[5] = sa_MK_UINT32(aad[4], aad[5], aad[6], aad[7]);
                if (aadSize == 8)
                {
                    cmdLb[6] = sa_MK_UINT32(payload[0], payload[1], payload[2], payload[3]);
                    cmdLb[7] = sa_MK_UINT32(payload[4], payload[5], payload[6], payload[7]);
                }
                else
                {
                    cmdLb[6] = sa_MK_UINT32(aad[8], aad[9], aad[10], aad[11]);
                    cmdLb[7] = sa_MK_UINT32(payload[0], payload[1], payload[2], payload[3]);
                }
                
                /* IV */
                cmdLb[9]  = sa_MK_UINT32(iv[0], iv[1], iv[2], iv[3]);
                cmdLb[10] = sa_MK_UINT32(iv[4], iv[5], iv[6], iv[7]);
                
                /* data length 2 */
                cmdLb[3] = (authSize + aadSize) << 3;
        
            }
            break;
            
        case sa_DM_GMAC_AH:
            { 
                uint8_t *iv  = pAuthIV;
                
                cmdLb[0] |=   authSize;
                cmdLb[1] |=   (authOffset << 24);
                
                /* data length 2 */
                cmdLb[3] = (authSize) << 3;
                
                /* IV */
                cmdLb[5]  = sa_MK_UINT32(iv[0], iv[1], iv[2], iv[3]);
                cmdLb[6] = sa_MK_UINT32(iv[4], iv[5], iv[6], iv[7]);
        
            }
            break;
            
            
        default:
           /* Error handling */ 
           break; 
    }
} 

/**
 *  @ingroup salld_api_structures
 *  @brief   Sa_psInfo_t defines the PS Info data used by SASS
 *
 *  @details Sa_psInfo_t defines the Protocol-specific information required by SASS
 *           - word[0] contains command, payload offset and payload length described below:
 *              -SRTP/AC: offset to the RTP header; RTP payload length including ICV
 *              -IPSEC AH: offset to the Outer IP; IP payload length
 *              -IPSEC ESP: offset to the ESP header; ESP papload length including ICV
 *           - word[1] contains 32-bit count-C for certain 3GPP ciphering and encryption modes
 *           - word[2:5] contain up to 16-byte IV in array of 32-bit words for certain 3GPP 
 *                       ciphering and encryption modes
 *  @note: Use PS Info constructing Macros to format the PS Info data
 */

typedef struct  {
    uint32_t  word[6]; /**< PS Info control block word 0-5 */
} Sa_psInfo_t;

/** 
 *  @ingroup salld_api_macros
 *  @{
 *  @name SA PS Info Constructing Macros
 *  Macros used by the SA PS Info Command
 *  
 */
/*@{*/
#define sa_PSINFO_FORMAT_CMD(x, offset, len)   ((x)->word[0] = (0x20000000UL | ((offset) << 16) | (len)))   /**< Format the PS Info header */
#define sa_PSINFO_SET_COUNTC(x, countC)        ((x)->word[1] = (countC))                                    /**< Constructing Count-C */
/*@}*/
/** @} */

/* @} */ /* ingroup */

/*@}*/ /* @name SA LLD Macros */

/**
 *
 *  @ingroup salld_api_macros
 *  @brief sa_PSINFO_SET_IV: Constructing IV
 *
 *  @param[in]  x               pointer to PS Info data used by SASS
 *  @param[in]  iv              pointer to IV
 *  @param[in]  ivSize          size of the IV
 *
 */
static inline void sa_PSINFO_SET_IV(Sa_psInfo_t *x, uint32_t *iv, int ivSize)
{
    int i;
    x->word[0] |= ((ivSize + 8) << 24);
    for( i = 0; i < ivSize/4 ; i++)
        x->word[i+ 2] = iv[i];
}

/**   
 *  @page appendix1 3GPP Opeartion and PDU formats 
 *
 *  There are many variation of PDU formats specified by the various 3GPP Protocols. 
 *  All supported 3GPP operations and data formats are described below:
 *
 *  @par GSM:
 *     Supported PDU types:
 * 
 *  @verbatim 
 

                      0 1 2 3 4 5 6 7 
                     +-+-+-+-+-+-+-+-+
                     |               |
                     + IV (8/16byte) |
                     |               |          
                   +>+-+-+-+-+-+-+-+-+
                   | |               | 
                   | |               |
                   | |     Data      |               
                   | |               | 
                   | |               | 
                   | +-+-+-+-+-+-+-+-+
                   |
                   |-> Ciphering Unit
 
                   Figure 1.  Ciphering Unit of a GSM PDU.      
    @endverbatim
 *
 * @par          
 *      Ciphering algorithm: GSM A5/3, ECSD A5/3, GAE3, Kasumi-F8
 * @par          
 *      Key Stream Generation:
 *      - Input to SA: 64-bit IV plus all zero payload
 *      - Output from SA: 64-bit IV plus key stream (228 bit for GSM A5/3, 696 bit for ECSD A5/3, varialbe length for GAE3)
 * @par
 *      Full Encryption/Decryption:
 *      - Input to SA: 64/128-bit IV plus payload
 *      - Output from SA: 64/128-bit IV plus encryped/decrypted paylaod
 *       
 *  @par WCDMA:
 *     Supported PDU types:
 * 
 *  @verbatim 
 
  
                      0 1 2 3 4 5 6 7 
                     +-+-+-+-+-+-+-+-+
                     |DC|  Seq Num   |
                     +-+-+-+-+-+-+-+-+
                     |Seq Num  |P| HE|                  
                   +>+-+-+-+-+-+-+-+-+
                   | | Length Ind. |E|          
                   | +-+-+-+-+-+-+-+-+
                   | |               | 
                   | |               |
                   | |     Data      |               
                   | |               | 
                   | |               | 
                   | +-+-+-+-+-+-+-+-+
                   | | PAD or piggy-backed STATUS PDU                
                   | +-+-+-+-+-+-+-+-+
                   |
                   |-> Ciphering Unit
 
 
                   Figure 2.  Ciphering Unit of a WCDMA AMD PDU.
 
 
                      0 1 2 3 4 5 6 7 
                     +-+-+-+-+-+-+-+-+
                     | Seq Num     |E|
                   +>+-+-+-+-+-+-+-+-+
                   | | Length Ind. |E|          
                   | +-+-+-+-+-+-+-+-+
                   | |               | 
                   | |               |
                   | |     Data      |               
                   | |               | 
                   | |               | 
                   | +-+-+-+-+-+-+-+-+
                   |
                   |-> Ciphering Unit
 
 
                   Figure 3.  Ciphering Unit of a WCDMA UMD PDU.
 
 
                      0 1 2 3 4 5 6 7 
                     +-+-+-+-+-+-+-+-+
                     |               |
                     |   Count-C     |
                     |   (4-byte)    |
                     |               |          
                   +>+-+-+-+-+-+-+-+-+
                   | |               | 
                   | |               |
                   | |     Data      |               
                   | |               | 
                   | |               | 
                   | +-+-+-+-+-+-+-+-+
                   |
                   |-> Ciphering Unit
 
                   Figure 4.  Ciphering Unit of a WCDMA TMD PDU.
    @endverbatim
 
 *
 * @par          
 *      Ciphering algorithm: Kasumi/F8,Snow 3G/F8, AES-CTR
 *
 * @par          
 *      From Air Traffic (Decryption) :
 *          - Input to SA: 
 *            - PDU header plus payload for RLC AMD/UMD PDU
 *            - Payload with 32-bit Count-C in the CPPI header for TMD MAC PDU
 *          - Output from SA: 
 *            - PDU header plus decrypted payload for RLC AMD/UMD PDU
 *            - Decrypted payload for TMD MAC PDU
 *
 * @par          
 *      To Air Traffic (Encryption) :
 *          - Input to SA: 
 *            - PDU header plus payload for RLC AMD/UMD PDU
 *            - Payload for TMD MAC PDU
 *          - Output from SA: 
 *            - PDU header plus encrypted payload for RLC AMD/UMD PDU
 *            - 32-bit Count-C plus encrypted payload for TMD MAC PDU
 *      
 *                     
 * @par LTE:
 *     Supported PDU types:
 * 
 *  @verbatim 
 
                      0 1 2 3 4 5 6 7 
                     +-+-+-+-+-+-+-+-+
                     |               |
                     |   Count-C     |
                     |   (4-byte)    |
                     |               |          
                   +>+-+-+-+-+-+-+-+-+
                   | |               | 
                   | |               |
                   | |     Data      |               
                   | |               | 
                   | |               | 
                   | +-+-+-+-+-+-+-+-+
                   |
                   |-> Ciphering Unit
 
                   Figure 5.  Ciphering Unit of a PDCP Data PDU.
 
 
                      0 1 2 3 4 5 6 7 
                     +-+-+-+-+-+-+-+-+ <+
                     |R R R| PDCP SN |  |
                   +>+-+-+-+-+-+-+-+-+  |
                   | |               |  |
                   | |               |  |
                   | |     Data      |  |             
                   | |               |  |
                   | |               |  |
                   | +-+-+-+-+-+-+-+-+  |-> Authentication Unit
                   | |   MAC-I       |
                   | |   (4 byte)    |
                   | +-+-+-+-+-+-+-+-+
                   | 
                   |-> Ciphering Unit
 
                   Figure 6.  PDCP Control Plane Data Uint for SRBs
    @endverbatim
 *
 * @par          
 *      Ciphering algorithm: Snow 3G/F8, AES-CTR
 *      Authentication algorithm: CMAC
 *
 * @par          
 *      From Air Traffic (Decryption and/or Authentication verification) :
 *          - Input to SA: 
 *            - PDCP Data with 32-bit Count-C in the CPPI header for the Data Plane PDU
 *            - PDCP Header plus Data and MAC-I with 32-bit Count-C in the CPPI header for the Control Plane PDU
 *          - Output from SA: 32-bit Count-C plus decrypted PDCP PDU Data
 *            - PDCP Data for the Data Plane PDU
 *            - PDCP Header plus Data and MAC-I for the Control Plane PDU
 *
 * @par          
 *      To Air Traffic (Encryption and/or Authentication) :
 *          - Input to SA: 
 *            - PDCP Data for the Data Plane PDU
 *            - PDCP Header plus Data and MAC-I with 32-bit Count-C in the CPPI header for the Control Plane PDU
 *          - Output from SA: 32-bit Count-C plus encrypted PDCP PDU Data
 *            - 32-bit Count-C plus PDCP Data for the Data Plane PDU
 *            - PDCP Header plus Data and MAC-I for the Control Plane PDU
 */
 
/**   
 *  @page appendix2 Shadow Instance for mixed-Endian operation
 *
 *  The SASS supports mixed-endianess operating mode such that the channel is created and configured by the master 
 *  processor, (e.g. ARM which is outside of SoC), while the data processing is handled by slave processor(s) 
 *  (e.g. DSPs). When in this mode, SA LLD must create and maintain a shadow instance, which is a copy of 
 *  the master instance with opposite endianess. New APIs are created to query the shadow instances, which should be
 *  passed to and used by the slave processor(s). 
 *
 *  @note: Only the following use case is supported at this moment, further enhancements may be provided:
 *         - IPSEC ESP/AH mode
 *         - Control Path at ARM and Data Path at DSPs 
 *
 */

/**   
 *  @page appendix3 SA2_UL  Requirements for packet to pass Security Attributes Checkes
 * 
 * In addition to the first-level check that the context fetch must pass is to satisfy
 * the external firewall module's check at the soc level configured by DMSC, the second-level check 
 * that SA2_UL does is to compare the incoming packet\92s security attributes 
 * against the attributes that are stored as part of the SCCTL inside the security context.
 * The following table summarizes the evaluation that will occur before a context is used for the packet
 * processing. The rightmost column shows the possible change in 'secure' attribute of the packet 
 * if and only if the check passes. If packet check fails, the original packet attribute is untouched.
 *
 *  @verbatim
 * |----------------------------------------------------|-------------------------------------------------|----------------|
 * | Incoming Packet Attributes                         | Attributes in Security Context                  |                |
 * |--------|---------|--------|--------|------|--------|--------|---------|--------|-----|------|--------| Impact         |
 * | Secure | Promote | Demote | NS     | Priv | PrivID |  Ctx   |   Ctx   |  Ctx   | Ctx | Priv | PrivID | on out         |
 * |        |         |        | Crypto |      |        | Secure | Promote | Demote | NS  |      |        | pkt attributes |
 * |--------|---------|--------|--------|------|--------|--------|---------|--------|-----|------|--------|----------------|
 * |    0   |    0    |   0    |    0   |  b   |   c    |   0    |    0    |   0    |  0  |  b   |   c    |  No Change     |
 * |--------|---------|--------|--------|------|--------|--------|---------|--------|-----|------|--------|----------------|
 * |    1   |    0    |   0    |    0   |  b   |   c    |   1    |    0    |   0    |  0  |  b   |   c    |  No Change     |
 * |--------|---------|--------|--------|------|--------|--------|---------|--------|-----|------|--------|----------------|
 * |    0   |    1    |   0    |    0   |  b   |   c    |   1    |    1    |   0    |  0  |  b   |   c    | Secure=1(note2)|
 * |--------|---------|--------|--------|------|--------|--------|---------|--------|-----|------|--------|----------------|
 * |    1   |    0    |   1    |    0   |  b   |   c    |   1    |    0    |   1    |  0  |  b   |   c    | Secure=0(note3)|
 * |--------|---------|--------|--------|------|--------|--------|---------|--------|-----|------|--------|----------------|
 * |    0   |    0    |   0    |    1   |  b   |   c    |   1    |    0    |   0    |  1  |  b   |   c    |No Change(note4)|
 * |--------|---------|--------|--------|------|--------|--------|---------|--------|-----|------|--------|----------------|
 *
 * @endverbatim
 * Legend:
 * The 'b' and 'c' can be any legal value of 2-bit priv and 8-bit privid respectively. The priv attribute on the
 * input packet must match the priv attribute in the context. Similarly the privid attribute on the input packet
 * must match the privid attribute in the context, except when the privid in the context is using the wildcard
 * value (0xC3) which will bypass the privid check.
 * Note 1: Promotion of non-secure packet to be secure packet also always requires the 48-bit SCPTR that
 * comes with the packet to be within the range of 'SCPTR Promote Range' registers.
 * Note 2: Output packet (with secure attribute = 1) is intended to land in a secure memory.
 * Note 3: Output packet (with secure attribute = 0) is intended to land in a non-secure memory.
 * Note 4: When the input packet has NS_crypto = 1 and the context has AllowNS = 1, a non-secure packet
 * is allowed to use a secure context without knowing the key, but the output packet is intended to land in a
 * non-secure memory (secure attribute on the packet stays at 0).
 * Any other attributes combinations from the table above will result in security exceptions.
 *
 */
 
#ifdef __cplusplus
}
#endif

#endif /* _SALLD_H */

/* nothing past this point */
