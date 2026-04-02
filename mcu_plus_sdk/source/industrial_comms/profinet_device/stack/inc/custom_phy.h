/*!
 *  \file pru_cust_phy.h
 *
 *  \brief
 *  Internal PRU interface for application based custom phy(s).
 *
 *  \author
 *  KUNBUS GmbH
 *
 *  \date
 *  2022-02-23
 *
 *  \copyright
 *  Copyright (c) 2022, KUNBUS GmbH<br /><br />
 *  All rights reserved.<br />
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:<br />
 *  <ol>
 *  <li>Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.</li>
 *  <li>Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.</li>
 *  <li>Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.</li>
 *  </ol>
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#if !(defined PROTECT_CUST_PHY_H)
#define PROTECT_CUST_PHY_H		1

#include <osal.h>

#if ((defined ECATSLAVE_SO) && (ECATSLAVE_SO==1) || ((defined ETHERNETIP_SO) && (ETHERNETIP_SO==1)) || ((defined PROFINETIO_SO) && (PROFINETIO_SO==1))) // defined if compiled as a DLL
#if (defined PRUAPI_EXPORTS) // defined if we are building the ECATSLAVE DLL (instead of using it)
#define PRU_API OSAL_DLL_EXPORT
#else
#define PRU_API OSAL_DLL_IMPORT
#endif // ECSLVAPI_EXPORTS
#define PRU_LOC OSAL_DLL_LOCAL
#else // not defined: this means is a static lib.
#define PRU_API
#define PRU_LOC
#endif

#define CUST_PHY_STATUS_ERROR_UNKNOWN_PHY   0x06

#define CUST_PHY_MDI_MANUAL_CONFIG          0x00
#define CUST_PHY_MDIX_MANUAL_CONFIG         0x01
#define CUST_PHY_MDIX_AUTO_CROSSOVER        0x02

/*! <!-- Description: -->
 *
 *  \brief Reset PHY
 *
 *  \details
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pCtxt_p     Callback context
 *  \param[in]  idx_p       Phy Index
 *  \param[in]  reset_p     true: Phy reset, false: Phy active
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void    (*CUST_PHY_CBreset_t)           (void*                          pCtxt_p
                                                ,uint8_t                        idx_p
                                                ,bool                           reset_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Issue PHY Reset by software (used if no Hard Reset is available)
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_softReset_t)            (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Issue PHY restart by software
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void  (*CUST_PHY_softRestart_t)         (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Enable Auto MDIX
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_enableAutoMDIX_t)       (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Set MII Mode
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_setMIIMode_t)           (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Configure Phy power mode state
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *  \param[in]  powerDown_p         False = Normal Operation True = Power-down mode
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_setPowerMode_t)         (void*                          pAppContext_p
                                                ,void*                          pStackContext_p
                                                ,bool                           powerDown_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Retrieve Phy power mode state
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  \return     bool                Power mode state.
 *  \retval     false               Normal operation.
 *  \retval     true                Power saving mode active.
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef bool    (*CUST_PHY_getPowerMode_t)      (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Configure PhyMLED to detect RxLink by MLED (e.g. TLK)
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_configMLED_t)           (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Set Ext Full Duplex enable
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_enableExtFD_t)          (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Set ODD Nibble detection enable
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_enableODDNibbleDet_t)   (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Set Rx Error Idle enable
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_enableRxErrIdle_t)      (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Configure PHY LEDs
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_configLed_t)            (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Configure PHY Blink LED mode
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_configLedBlink_t)       (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Set fast link down Detection enable
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_enableFastLinkDownDet_t)(void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Set Fast RX DV detection enable
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_enableFastRXDVDet_t)    (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Set SW Strap config done
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_configSwStrapDone_t)    (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Configures PHY for link connection.
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *  \param[in]  autoNeg_p           selected auto-negotiation mode (false - auto-negotiation inactive, true - auto-negotiation active)
 *  \param[in]  linkSpeed_p         required link speed (10, 100 or 1000 Mbps)
 *  \param[in]  fullDuplex_p        required duplex mode (false - half-duplex, true - full duplex)
 *  \param[out] pResult_p           pointer to variable where result of configuration needs to be stored (0 - Success, other - Error)
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void    (*CUST_PHY_setLinkConfig_t)     (void*                          pAppContext_p
                                                ,void*                          pStackContext_p
                                                ,bool                           autoNeg_p
                                                ,uint16_t                       linkSpeed_p
                                                ,bool                           fullDuplex_p
                                                ,uint32_t*                      pResult_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Provides auto-negotiation enabled flag.
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  \return     bool                auto-negotiation enabled flag (false - auto-negotiation disabled, true - auto-negotiation enabled)
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef bool (*CUST_PHY_getAutoNegotiation_t)   (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Configures PHY MDI crossover mode.
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *  \param[in]  mdixMode_p          required MDI crossover mode (0 - Manual MDI configuration, 1 - Manual MDI-X configuration, 2 - Enable automatic crossover)
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_setMdixMode_t)          (void*                          pAppContext_p
                                                ,void*                          pStackContext_p
                                                ,uint32_t                       mdixMode_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Provides actually used PHY MDI crossover mode.
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  \return     uint32_t            value of used MDI crossover mode (0 - Manual MDI configuration, 1 - Manual MDI-X configuration, 2 - Enable automatic crossover)
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef uint32_t (*CUST_PHY_getMdixMode_t)      (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Disables 1Gbit Advertisment during autonegotiation.
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_disable1GbAdver_t)      (void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  Enables low latency in RGMII mode
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_enableRgmiiLowLatency_t)(void*                          pAppContext_p
                                                ,void*                          pStackContext_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  RGMII_TX_HALF_FULL_THR[1:0] can be changed from the default of 0x2 to 0x1.
 *    This will reduce the latency by one clock period.
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *  \param[in]  threshold_p         Number of cycles for threshold (2, 1)
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_rgmiiTxHalfFullThreshold_t)
                                                (void*                          pAppContext_p
                                                ,void*                          pStackContext_p
                                                ,uint32_t                       threshold_p);

/*! <!-- Description: -->
 *
 *  \brief
 *  RGMII_RX_HALF_FULL_THR[1:0] can be changed from the default of 0x2 to 0x1.
 *    This will reduce the latency by one clock period.
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pAppContext_p       application context
 *  \param[in]  pStackContext_p     slave stack context
 *  \param[in]  threshold_p         Number of cycles for threshold (2, 1)
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef void (*CUST_PHY_rgmiiRxHalfFullThreshold_t)
                                                (void*                          pAppContext_p
                                                ,void*                          pStackContext_p
                                                ,uint32_t                       threshold_p);


typedef struct CUST_PHY_SPhyDescriptor
{
    CUST_PHY_softReset_t                softwareReset;
    CUST_PHY_softRestart_t              softwareRestart;
    CUST_PHY_enableAutoMDIX_t           enableAutoMDIX;
    CUST_PHY_setMIIMode_t               setMiiMode;
    CUST_PHY_setPowerMode_t             setPowerMode;
    CUST_PHY_getPowerMode_t             getPowerMode;
    CUST_PHY_configMLED_t               configMLED;
    CUST_PHY_enableExtFD_t              enableExtFD;
    CUST_PHY_enableODDNibbleDet_t       enableODDNibbleDet;
    CUST_PHY_enableRxErrIdle_t          enableRxErrIdle;
    CUST_PHY_configLed_t                configLed;
    CUST_PHY_configLedBlink_t           configLedBlink;
    CUST_PHY_enableFastLinkDownDet_t    enableFastLinkDownDet;
    CUST_PHY_enableFastRXDVDet_t        enableFastRXDVDet;
    CUST_PHY_configSwStrapDone_t        configSwStrapDone;
    CUST_PHY_setLinkConfig_t            setLinkConfig;
    CUST_PHY_getAutoNegotiation_t       getAutoNegotiation;
    CUST_PHY_setMdixMode_t              setMdixMode;
    CUST_PHY_getMdixMode_t              getMdixMode;
    CUST_PHY_disable1GbAdver_t          disable1GbAdver;
    CUST_PHY_enableRgmiiLowLatency_t    rgmiiLowLatencyEnable;
    CUST_PHY_rgmiiTxHalfFullThreshold_t rgmiiTxHalfFullThreshold;
    CUST_PHY_rgmiiRxHalfFullThreshold_t rgmiiRxHalfFullThreshold;
} CUST_PHY_SPhyDescriptor_t;

/*!
 *  <!-- Description: -->
 *
 *  \brief
 *  Phy Detection callback of external PhyLib
 *  \details
 *
 *  <!-- Parameters and return values: -->
 *
 *  \param[in]  pPhyLibCtxt_p       External PhyLib Context
 *  \param[in]  phyId_p             Phy ID read from hardware
 *  \param[in]  pPhyLibDesc_p       Customer PhyLib hook
 *  \return     0 on success and Phy detected, error code otherwise
 *
 *  <!-- Group: -->
 *
 *  \ingroup CUST_PHY
 *
 * */
typedef int16_t (*CUST_PHY_CBextPhyLibDetect_t)     (void*                          pPhyLibCtxt_p
                                                    ,uint32_t                       phyId_p
                                                    ,CUST_PHY_SPhyDescriptor_t*     pPhyLibDesc_p);

#if (defined __cplusplus)
extern "C" {
#endif

extern PRU_API void     CUST_PHY_CBregisterReset    (CUST_PHY_CBreset_t             cbFunc_p
                                                    ,void*                          pCtxt_p);

extern PRU_API void     CUST_PHY_CBregisterLibDetect(CUST_PHY_CBextPhyLibDetect_t   cbFunc_p
                                                    ,void*                          pCtxt_p);

extern PRU_API void     CUST_PHY_writeReg           (void*                          pStackCtxt_p
                                                    ,uint32_t                       regNum_p
                                                    ,uint16_t                       writeValue_p);
extern PRU_API uint32_t CUST_PHY_readReg            (void*                          pStackCtxt_p
                                                    ,uint32_t                       regNum_p
                                                    ,uint16_t*                      pData_p);

extern PRU_API uint32_t CUST_PHY_getPhyAddr         (void*                          pStackCtxt_p);

#if (defined __cplusplus)
}
#endif

#endif /* PROTECT_CUST_PHY_H */
