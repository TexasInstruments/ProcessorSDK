/**
* @file device.h
* @brief J721E device specific definitions
*
* Copyright (C) 2025 Texas Instruments Incorporated - https://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
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

/*  JTAGID: 0x2bb6402f (for 2nd version of silicon) */

#ifndef __COMMON_J721E_H
#define __COMMON_J721E_H

#include <stdlib.h>
#include <stdint.h>              // The library uses C99 exact-width integer types

#ifdef __cplusplus
extern "C" {
#endif


// CPTracer2 probe Offset address
#define CPT2_PROBEn_OFFSET(n)       (0x00020000 + n*0x0001000) // CPT2 probe N offset in Aggregator space

// Debug Cell sub-components offset address
#define TBR_OFFSET                  0x00005000 // TBR offset in Debug Cell space
#define ATBREP_OFFSET               0x00004000 // ATB Replicator offset in Debug Cell space

////////////////////////////////////////////////////////////////////
// Device Specific Definitions
////////////////////////////////////////////////////////////////////

////////// GTC definitions ///////////////////////////
// GTC base address in system memory view
#define GTC_BADDR       (0xa90000UL) //Global time counter GTC base address, this address is 32-bit so no need to be re-mapped for R5 or M3
#define GTC_TIMER_MAPPED_SIZE       (512)

////////// ETB definitions ///////////////////////////
#define NUM_ETB_INSTANCES   5 // Number of ETBs in device

///////// Debug Cell definitions ////////////////////
/*! \par eDebugCell_ID
    Debug Cell ID
*/
typedef enum
{
    eCC_DBGCELL_ID = 0,                  /*!< 0 - Compute Cluster Debug Cell */
    eSOC_DBGCELL_ID = 1,                 /*!< 1 - SoC Debug Cell */
    eMCU_DBGCELL_ID = 2,                 /*!< 2 - MCU Debug Cell */
    eC66xSS1_DBGCELL_ID = 3,             /*!< 3 - c66x SS1 Debug Cell */
    eC66xSS2_DBGCELL_ID = 4             /*!< 4 - c66x SS2 Debug Cell */
} eDebugCell_ID;


// Debug Cell Base address in system memory view
#define CC_DBGCELL_BADDR       (0x4C3C000000) // Compute Cluster Debug Cell base address
#define SOC_DBGCELL_BADDR      (0x4C3C010000) // SoC Debug Cell base address
#define MCU_DBGCELL_BADDR      (0x4C3C020000) // MCU Debug Cell base address
#define C66SS0_DBGCELL_BADDR   (0x4C3C030000) // C66x Sub-system0 Debug Cell base address
#define C66SS1_DBGCELL_BADDR   (0x4C3C040000) // C66x Sub-system1 Debug Cell base address

#define DBGCELL_MAPPED_SIZE       (0x10000) // 64KB

// Debug Cell base address in 48-bit system memory address space. This needs to be mapped to 32-bit address for R5 or M3 view.
#define _DBGCELL_BaseAddress_raw(dc_id) ((dc_id==eCC_DBGCELL_ID)? CC_DBGCELL_BADDR:( (dc_id==eSOC_DBGCELL_ID)? SOC_DBGCELL_BADDR : MCU_DBGCELL_BADDR))

#define _DBGCELL_BaseAddress(dc_id) _DBGCELL_BaseAddress_raw(dc_id)

// ETB base address
#define _ETB_BaseAddress(dc_id)		(_DBGCELL_BaseAddress(dc_id)+TBR_OFFSET)

// ATB Replicator base address
#define _ATBREP_BaseAddress(dc_id)	(_DBGCELL_BaseAddress(dc_id)+ATBREP_OFFSET)

////////// CPTracer2 definitions /////////////////////
//CPT2 Aggregators IDs
typedef enum
{
    eMSMC0_CPT2_ID  = 0,                 /*!< 0 - MSMC0 CPT2 Aggregator */
    eMSMC1_CPT2_ID  = 1,                 /*!< 1 - MSMC1 CPT2 Aggregator */
    eSOC_CPT2_ID    = 2,                 /*!< 2 - SOC CPT2 Aggregator   */
    eSOC_AC_CPT2_ID = 3,                 /*!< 3 - SOC AC CPT2 Aggregator   */
    eSOC_HC_CPT2_ID = 4,                 /*!< 4 - SOC HC CPT2 Aggregator */
    eMCU_CPT2_ID    = 5,                 /*!< 5 - MCU CPT2 Aggregator */
    eC7X_CPT2_ID    = 6                  /*!< 6 - C7X CPT2 Aggregator   */
} eCPT2Aggr_ID ;

// CPTracer2 Aggregator Base address in system memory view
#define MSMC0_CPT2_BADDR           (0x4C30140000)
#define MSMC1_CPT2_BADDR           (0x4C30180000)
#define C7X_CPT2_BADDR             (0x4C31440000)
#define MCU_CPT2_BADDR			   (0x4C3E000000)
#define SOC_CPT2_BADDR			   (0x4C3E100000)
#define SOC_AC_CPT2_BADDR          (0x4C3E140000)
#define SOC_HC_CPT2_BADDR          (0x4C3E180000)

#define CPT2_AGGR_MAPPED_SIZE      (0x40000) // 256KB

#define _CPT2_BaseAddress_raw(cpt2_id) ((cpt2_id==eMSMC0_CPT2_ID) ? MSMC0_CPT2_BADDR : \
                                       ((cpt2_id==eMSMC1_CPT2_ID) ? MSMC1_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_CPT2_ID) ? SOC_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_AC_CPT2_ID) ? SOC_AC_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_HC_CPT2_ID) ? SOC_HC_CPT2_BADDR : \
                                       ((cpt2_id==eMCU_CPT2_ID) ? MCU_CPT2_BADDR : C7X_CPT2_BADDR))))))

#define _CPT2_BaseAddress(cpt2_id)  _CPT2_BaseAddress_raw(cpt2_id)

// CPTracer2 probe base address
#define _CPT2Probe_BaseAddress(cpt2_id, pb_index)	(_CPT2_BaseAddress(cpt2_id) + CPT2_PROBEn_OFFSET(pb_index))

#define NUM_CPT2_PROBES     48 // Number of CPTracer2 probes

/*! \par cpt2pb_id_t
    CPTracer2 probe IDs
*/
typedef enum {
    /* MSMC CPTracer2 instance 0 probes */
    eCpTracer2_Probe_0,        /**< 0 - "DRU0_TARGET" */
	eCpTracer2_Probe_1,        /**< 1 - "DRU0_INITIATOR" */
	eCpTracer2_Probe_2,        /**< 2 - "DRU1_TARGET" */
	eCpTracer2_Probe_3,        /**< 3 - "DRU1_INITIATOR" */
	eCpTracer2_Probe_4,        /**< 4 - "CSI0_TARGET" */
	eCpTracer2_Probe_5,        /**< 5 - "CSI0_INITIATOR" */
	eCpTracer2_Probe_6,        /**< 6 - "CSI4_TARGET" */
	eCpTracer2_Probe_7,        /**< 7 - "CSI4_INITIATOR" */
	eCpTracer2_Probe_8,        /**< 8 - "CSI8_TARGET" */
	eCpTracer2_Probe_9,        /**< 9  -"CSI8_INITIATOR" */

	/* MSMC CPTracer2 instance 1 probes */
    eCpTracer2_Probe_10,       /**< 10 - "EMIF0_INITIATOR" */
    eCpTracer2_Probe_11,       /**< 11 - "CSI9_TARGET" */
	eCpTracer2_Probe_12,       /**< 12 - "CSI9_INITIATOR" */

    /* SoC CPTracer2 probes */
    eCpTracer2_Probe_13,       /**< 13 - "C66x0_MDMA" */
    eCpTracer2_Probe_14,       /**< 14 - "C66x1_MDMA"	*/
    eCpTracer2_Probe_15,       /**< 15 - "C66x0_SDMA" */
    eCpTracer2_Probe_16,       /**< 16 - "C66x1_SDMA" */
    eCpTracer2_Probe_17,       /**< 17 - "NAV_DDR_0"  */
    eCpTracer2_Probe_18,       /**< 18 - "NAV_DDR_1"  */
	eCpTracer2_Probe_19,       /**< 19 - "NAVSS_HMST" */
	eCpTracer2_Probe_20,       /**< 20 - "NAVSS_LMST" */
	eCpTracer2_Probe_21,       /**< 21 - "NAVSS_SRAM_0" */
	eCpTracer2_Probe_22,       /**< 22 - "NAVSS_SRAM_1" */
	eCpTracer2_Probe_23,       /**< 23 - "ONCHIP_SRAM"  */

    /* SoC AC CPTracer2 probes */
	eCpTracer2_Probe_24,       /**< 24 - "AC_CBASS_SLV_0" */
	eCpTracer2_Probe_25,       /**< 25 - "AC_CBASS_SLV_1" */
	eCpTracer2_Probe_26,       /**< 26 - "AC_CBASS_SLV_2" */
	eCpTracer2_Probe_27,       /**< 27 - "AC_CBASS_SLV_4" */
	eCpTracer2_Probe_28,       /**< 28 - "DMPAC_SRAM_SLV" */
	eCpTracer2_Probe_29,       /**< 29 - "VPAC_SRAM_SLV"  */

    /* SoC HC CPTracer2 probes */
	eCpTracer2_Probe_30,	  /**< 30 - "PCIE_HSLV_0" */
	eCpTracer2_Probe_31,	  /**< 31 - "PCIE_HSLV_1" */
	eCpTracer2_Probe_32,	  /**< 32 - "PCIE_HSLV_2" */
	eCpTracer2_Probe_33,	  /**< 33 - "PCIE_HSLV_3" */
	eCpTracer2_Probe_34,	  /**< 34 - "PCIE_LSLV_0" */
	eCpTracer2_Probe_35,	  /**< 35 - "PCIE_LSLV_1" */
	eCpTracer2_Probe_36,	  /**< 36 - "PCIE_LSLV_2" */
	eCpTracer2_Probe_37,	  /**< 37 - "PCIE_LSLV_3" */

    /* MCU CPTracer2 probes */
	eCpTracer2_Probe_38,	  /**< 38 - "MCU_EXPORT_SLV_0" */
	eCpTracer2_Probe_39,	  /**< 39 - "MCU_SRAM_SLV_1"   */
	eCpTracer2_Probe_40,	  /**< 40 - "MCU_FSS_S0_2"     */
	eCpTracer2_Probe_41,  	  /**< 41 - "MCU_FSS_S1_3"     */

    /* C7x CPTracer2 probes */
    eCpTracer2_Probe_42,      /**< 42 - "L1D_L2" */
    eCpTracer2_Probe_43,      /**< 43 - "L1P_L2" */
    eCpTracer2_Probe_44,      /**< 44 - "SE0_L2" */
    eCpTracer2_Probe_45,      /**< 45 - "SE1_L2" */
    eCpTracer2_Probe_46,      /**< 46 - "L2_L1D" */
    eCpTracer2_Probe_47       /**< 47 - "MMU_L2" */

} cpt2pb_id_t;

// DDR probe
#define eCpTracer2_DDR_Probe eCpTracer2_Probe_10

// CPTracer2 data type definitions
typedef struct {
    cpt2pb_id_t         probe_id; /* CPT2 probe ID */
    uint8_t             probe_port_index; /* Port number in CPT2 Aggregator */
    eDebugCell_ID       dbgcell_id; /* Debug Cell ID */
    uint64_t            dbgcell_baddr_raw; /* raw Debug Cell base address in 48-bit system memory address space */
    uint64_t            dbgcell_baddr; /* Debug Cell base address in the CPU view. For R5/M3, this would be the RAT mapped 32-bit address; For A53, this would be the same as dbgcell_baddr_raw */
    eCPT2Aggr_ID        aggr_id; /* CPT2 Aggregator ID */
    uint64_t            aggr_baddr_raw; /* CPT2 Aggregator base address in 48-bit system memory address space */
    uint64_t            aggr_baddr; /* CPT2 Aggregator base address in the CPU view. For R5/M3, this would be the RAT mapped 32-bit address; For A53, this would be the same as aggr_baddr_raw */
    uint8_t             master_id; /* Master ID for the CPT2 probe; this needs to match with the definition in Keystone3 platform database file in emupack */
} cpt2_access_t;

static cpt2_access_t g_cpt2_table[NUM_CPT2_PROBES] __attribute__((unused)) =
{
    /* MSMC CPTracer2 instance 0 probes */
    { eCpTracer2_Probe_0, 0, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 224 },
    { eCpTracer2_Probe_1, 1, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 225 },
    { eCpTracer2_Probe_2, 2, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 226 },
    { eCpTracer2_Probe_3, 3, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 227 },
    { eCpTracer2_Probe_4, 4, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 228 },
    { eCpTracer2_Probe_5, 5, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 229 },
    { eCpTracer2_Probe_6, 8, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 232 },
    { eCpTracer2_Probe_7, 9, eCC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 233 },
    { eCpTracer2_Probe_8, 12, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 236 },
    { eCpTracer2_Probe_9, 13, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 237 },

    /* MSMC CPTracer2 instance 1 probes */
    { eCpTracer2_Probe_10, 0,  eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 32 },
    { eCpTracer2_Probe_11, 10, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 42 },
    { eCpTracer2_Probe_12, 11, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 43 },

    /* SoC CPTracer2 probes */
    { eCpTracer2_Probe_13, 4, eSOC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 68 },
    { eCpTracer2_Probe_14, 5, eSOC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 69 },
    { eCpTracer2_Probe_15, 6, eSOC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 70 },
    { eCpTracer2_Probe_16, 7, eSOC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 71 },
    { eCpTracer2_Probe_17, 9, eSOC_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 105 },
    { eCpTracer2_Probe_18, 10, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 74 },
    { eCpTracer2_Probe_19, 11, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 75 },
    { eCpTracer2_Probe_20, 12, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 76 },
    { eCpTracer2_Probe_21, 13, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 77 },
    { eCpTracer2_Probe_22, 14, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 78 },
    { eCpTracer2_Probe_23, 15, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_CPT2_ID), _CPT2_BaseAddress(eSOC_CPT2_ID), 79 },

    /* SoC AC CPTracer2 probes */
    { eCpTracer2_Probe_24, 0, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 96 },
    { eCpTracer2_Probe_25, 1, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 97 },
    { eCpTracer2_Probe_26, 2, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 98 },
    { eCpTracer2_Probe_27, 3, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 99 },
    { eCpTracer2_Probe_28, 4, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 100 },
    { eCpTracer2_Probe_29, 5, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 101 },

    /* SoC HC CPTracer2 probes */
    { eCpTracer2_Probe_30, 0, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 128 },
    { eCpTracer2_Probe_31, 1, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 129 },
    { eCpTracer2_Probe_32, 2, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 130 },
    { eCpTracer2_Probe_33, 3, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 131 },
    { eCpTracer2_Probe_34, 4, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 132 },
    { eCpTracer2_Probe_35, 5, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 133 },
    { eCpTracer2_Probe_36, 6, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 134 },
    { eCpTracer2_Probe_37, 7, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eSOC_HC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_HC_CPT2_ID), _CPT2_BaseAddress(eSOC_HC_CPT2_ID), 135 },

    /* MCU CPTracer2 probes */
    { eCpTracer2_Probe_38, 0, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 160 },
    { eCpTracer2_Probe_39, 1, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 161 },
    { eCpTracer2_Probe_40, 2, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 162 },
    { eCpTracer2_Probe_41, 3, eMCU_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eMCU_DBGCELL_ID), _DBGCELL_BaseAddress(eMCU_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 163 },

    /* C7x CPTracer2 probes */
    { eCpTracer2_Probe_42, 0, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eC7X_CPT2_ID, _CPT2_BaseAddress_raw(eC7X_CPT2_ID), _CPT2_BaseAddress(eC7X_CPT2_ID), 192 },
    { eCpTracer2_Probe_43, 1, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eC7X_CPT2_ID, _CPT2_BaseAddress_raw(eC7X_CPT2_ID), _CPT2_BaseAddress(eC7X_CPT2_ID), 193 },
    { eCpTracer2_Probe_44, 2, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eC7X_CPT2_ID, _CPT2_BaseAddress_raw(eC7X_CPT2_ID), _CPT2_BaseAddress(eC7X_CPT2_ID), 194 },
    { eCpTracer2_Probe_45, 3, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eC7X_CPT2_ID, _CPT2_BaseAddress_raw(eC7X_CPT2_ID), _CPT2_BaseAddress(eC7X_CPT2_ID), 195 },
    { eCpTracer2_Probe_46, 4, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eC7X_CPT2_ID, _CPT2_BaseAddress_raw(eC7X_CPT2_ID), _CPT2_BaseAddress(eC7X_CPT2_ID), 196 },
    { eCpTracer2_Probe_47, 5, eCC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC_DBGCELL_ID), _DBGCELL_BaseAddress(eCC_DBGCELL_ID), eC7X_CPT2_ID, _CPT2_BaseAddress_raw(eC7X_CPT2_ID), _CPT2_BaseAddress(eC7X_CPT2_ID), 197 }


};


#ifdef __cplusplus
}
#endif

#endif
