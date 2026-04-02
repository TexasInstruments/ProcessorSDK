/**
* @file device.h
* @brief J721S2 device specific definitions
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

#ifndef __DEVICE_J721S2_H
#define __DEVICE_J721S2_H

#include <stdlib.h>
#include <stdint.h>              // The library uses C99 exact-width integer types

#ifdef __cplusplus
extern "C" {
#endif

/*  JTAGID: 0x0bb7502f */

// CPTracer2 probe Offset address
#define CPT2_PROBEn_OFFSET(n)       (0x00020000 + n*0x0001000) // CPT2 probe N offset in Aggregator space

// Debug Cell sub-components offset address
#define TBR_OFFSET                  0x00005000 // TBR offset in Debug Cell space
#define ATBREP_OFFSET               0x00004000 // ATB Replicator offset in Debug Cell space
/*TBR size: 65536 bytes */

////////////////////////////////////////////////////////////////////
// Device Specific Definitions
////////////////////////////////////////////////////////////////////

////////// GTC definitions ///////////////////////////
// GTC base address in system memory view
#define GTC_BADDR       (0xa90000UL) // Global time counter GTC base address, this address is 32-bit so no need to be re-mapped for R5
#define GTC_TIMER_MAPPED_SIZE       (512)

///////// Debug Cell definitions ////////////////////
/*! \par eDebugCell_ID
    Debug Cell ID
*/
// Debug Cells are present in Main Domain only
typedef enum
{
    eCC0_DBGCELL_ID = 0,                 /*!< 0 - Compute Cluster Debug Cell */
    eCC1_DBGCELL_ID = 1,                 /*!< 1 - Compute Cluster Debug Cell */
    eSOC_DBGCELL_ID = 2,                 /*!< 2 - SoC Debug Cell */
} eDebugCell_ID;

// Debug Cell Base address in system memory view
#define CC0_DBGCELL_BADDR      (0x4C3C000000) 
#define CC1_DBGCELL_BADDR      (0x4C3C010000)
#define SOC_DBGCELL_BADDR      (0x4C3C020000)

#define DBGCELL_MAPPED_SIZE       (0x10000) // 64KB

// Debug Cell base address in 48-bit system memory address space. This needs to be mapped to 32-bit address for R5 view.
#define _DBGCELL_BaseAddress_raw(dc_id) ((dc_id==eCC0_DBGCELL_ID)? CC0_DBGCELL_BADDR: \
                                        ((dc_id==eCC1_DBGCELL_ID)? CC1_DBGCELL_BADDR : SOC_DBGCELL_BADDR ))

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
    eSOC_RC_CPT2_ID = 2,                 /*!< 2 - SOC-RC CPT2 Aggregator */
    eSOC_AC_CPT2_ID = 3,                 /*!< 3 - SOC AC CPT2 Aggregator */
    eSOC_MI_CPT2_ID = 4,                 /*!< 4 - SOC MI CPT2 Aggregator */
    eSOC_ACP_CPT2_ID= 5,                 /*!< 5 - SOC ACP CPT2 Aggregator */
    eMCU_CPT2_ID    = 6,                 /*!< 6 - MCU CPT2 Aggregator */
} eCPT2Aggr_ID ;

// Debug Address (ABP) : 0x9xxx_xxxx --> System Address (AXI) 0x4C:0x3xxx_xxxx
// CPTracer2 Aggregator Base address in system memory view
#define MSMC0_CPT2_BADDR           (0x4C30140000)  
#define MSMC1_CPT2_BADDR           (0x4C30180000)  
#define SOC_RC_CPT2_BADDR          (0x4C3E100000)
#define SOC_AC_CPT2_BADDR          (0x4C3E180000)
#define SOC_MI_CPT2_BADDR          (0x4C3E140000)
#define SOC_ACP_CPT2_BADDR         (0x4C3E240000)
#define MCU_CPT2_BADDR			   (0x4C3E000000)

#define CPT2_AGGR_MAPPED_SIZE      (0x40000) // 256KB

#define _CPT2_BaseAddress_raw(cpt2_id) ((cpt2_id==eMSMC0_CPT2_ID) ? MSMC0_CPT2_BADDR : \
                                       ((cpt2_id==eMSMC1_CPT2_ID) ? MSMC1_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_RC_CPT2_ID) ? SOC_RC_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_AC_CPT2_ID) ? SOC_AC_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_MI_CPT2_ID) ? SOC_MI_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_ACP_CPT2_ID) ? SOC_ACP_CPT2_BADDR : MCU_CPT2_BADDR ))))))

#define _CPT2_BaseAddress(cpt2_id)  _CPT2_BaseAddress_raw(cpt2_id)

// CPTracer2 probe base address
#define _CPT2Probe_BaseAddress(cpt2_id, pb_index)	(_CPT2_BaseAddress(cpt2_id) + CPT2_PROBEn_OFFSET(pb_index))


#define NUM_CPT2_PROBES   35 // Number of CPTracer2 probes

/*! \par cpt2pb_id_t
    CPTracer2 probe IDs
*/
typedef enum {
    /* MSMC3 Probes are handles by Compute Cluster Debug Cell 0 (eDebugCellTbr_0)*/

    /* MSMC CPTracer2 instance 0 probes */
    /* Master Id: 224 + Probe Port Index*/
    eCpTracer2_Probe_0,        /**< 0 - "DRU0_TARGET (Data Routing Unit 0 (DRU0) Read Probe (Target) - VBUSM.C)" */   
	eCpTracer2_Probe_1,        /**< 1 - "DRU0_INITIATOR (Data Routing Unit 0 (DRU0) Initiator Probe (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_2,        /**< 2 - "DRU1_TARGET (Data Routing Unit 1 (DRU1) Read Probe (Target) - VBUSM.C)" */
	eCpTracer2_Probe_3,        /**< 3 - "DRU1_INITIATOR (Data Routing Unit 1 (DRU1) Initiator Probe (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_4,        /**< 4 - "CSI0_TARGET ARM CLUST0 (Coherent Slave Interface: A72 CorePac 0 (Target) - VBUSM.C)" */
	eCpTracer2_Probe_5,        /**< 5 - "CSI0_INITIATOR ARM CLUST0 (Coherent Slave Interface: A72 CorePac 0 (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_6,        /**< 6 - "CSI4_TARGET (MSMC2 (Target) - VBUSM.C)" */
	eCpTracer2_Probe_7,        /**< 7 - "CSI4_INITIATOR (MSMC2 (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_8,        /**< 8 - "CSI8_TARGET (SoC 1 (NavSS Northbridge 1) (Target) - VBUSM.C)" */

	/* MSMC CPTracer2 instance 1 probes */
    /* Master Id: 32 + Probe Port Index */
    eCpTracer2_Probe_9,       /**< 9 -  "EMIF0_INITIATOR (External Memory Interface (EMIF) 0 Probe (Initiator) - VBUSM.C)" */  
    eCpTracer2_Probe_10,      /**< 10 - "EMIF1_INITIATOR (External Memory Interface (EMIF) 1 Probe (Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_11,      /**< 11 - "CSI5_TARGET ARM CLUST1 (A72 CorePac 1 (Target) - VBUSM.C)" */
    eCpTracer2_Probe_12,      /**< 12 - "CSI5_INITIATOR ARM CLUST1 (A72 CorePac 1 (Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_13,      /**< 13 - "CSI9_TARGET (SoC 0 (NavSS Northbridge 0)(Target) - VBUSM.C)" */
	eCpTracer2_Probe_14,      /**< 14 - "CSI9_INITIATOR (SoC 0 (NavSS Northbridge 0)(Initiator) - VBUSM.C)" */

    /* For SOC RC, AC, HC, MI, ACP Probes, we use SOC Debug Cell (eDebugCellTbr_2) */

    /* SoC-RC CPTracer2 probes */
    /* Master Id: 96 + Probe Port Index */
    eCpTracer2_Probe_15,       /**< 15 - "NAV_DDR_0 - VBUSM"  */
    eCpTracer2_Probe_16,       /**< 16 - "NAV_DDR_1 - VBUSM"  */
	eCpTracer2_Probe_17,       /**< 17 - "NAVSS_HMAST (Hardware Multiple Address Translation and Stepping) - VBUSM" */
	eCpTracer2_Probe_18,       /**< 18 - "NAVSS_LMAST - VBUSM" */
	eCpTracer2_Probe_19,       /**< 19 - "NAVSS_SRAM_0 - VBUSM" */
	eCpTracer2_Probe_20,       /**< 20 - "NAVSS_SRAM_1 - VBUSM" */

    /* SoC AC CPTracer2 probes */
    /* Master Id: 192 + Probe Port Index */
	eCpTracer2_Probe_21,       /**< 21 - "AC_CBASS_SLV_0 (AC NAV DDR SLV 0)" */
	eCpTracer2_Probe_22,       /**< 22 - "AC_CBASS_SLV_1 (AC NAV DDR SLV 1)" */
	eCpTracer2_Probe_23,       /**< 23 - "AC_CBASS_SLV_2 (AC NAV DDR SLV 2)" */
	eCpTracer2_Probe_24,       /**< 24 - "AC_CBASS_SLV_4 (AC NAV SRAM SLV 0)" */
    eCpTracer2_Probe_25,       /**< 25 - "DMPAC SRAM SLV" */
	eCpTracer2_Probe_26,       /**< 26 - "VPAC_SRAM_SLV0 (VPAC0 SRAM SLV)" */
	eCpTracer2_Probe_27,       /**< 27 - "VPAC_SRAM_SLV1 (VPAC1 SRAM SLV) " */

    /* SOC MI Probes */
    /* Master Id: 64 + Probe Port Index */
    eCpTracer2_Probe_28,	  /**< 28 - "ONCHIP_SRAM PULSAR0 - VBUSM" */

    /* SOC ACP Probes (Only present in J7AHP and J7AM devices) */
    /* Master Id: 160 + Probe Port Index */
    eCpTracer2_Probe_29,	  /**< 29 - "ONCHIP_SRAM_PULSAR1 - VBUSM" */
    eCpTracer2_Probe_30,	  /**< 30 - "ONCHIP_SRAM_PULSAR2 - VBUSM" */

    // /* For MCU Probes, we use SOC Debug Cell (eDebugCellTbr_2) */

    // /* MCU CPTracer2 probes (eTraceAggregator_6) */
    // /* Master Id: 128 + Probe Port Index */
	eCpTracer2_Probe_31,	  /**< 31 - "MCU_EXPORT_SLV_0 (Transactions from wakeup and MCU domains to the main domain)" */
	eCpTracer2_Probe_32,	  /**< 32 - "MCU_SRAM_SLV_1 (Transactions to the on-chip MSRAM in the MCU domain - VBUSM)"   */
	eCpTracer2_Probe_33,	  /**< 33 - "MCU_FSS_S0_2 (Transactions to the FSS slave port 0: first OSPI and Hyperbus - VBUSM)" */  
	eCpTracer2_Probe_34,  	  /**< 34 - "MCU_FSS_S1_3 (Transactions to the second OSPI - VBUSM)" */

} cpt2pb_id_t;

// DDR probe
#define eCpTracer2_DDR_Probe eCpTracer2_Probe_9

// CPTracer2 data type definitions
typedef struct {
    cpt2pb_id_t         probe_id; /* CPT2 probe ID */
    uint8_t             probe_port_index; /* Port number in CPT2 Aggregator */
    eDebugCell_ID       dbgcell_id; /* Debug Cell ID */
    uint64_t            dbgcell_baddr_raw; /* raw Debug Cell base address in 48-bit system memory address space */
    uint64_t            dbgcell_baddr; /* Debug Cell base address in the CPU view. For R5, this would be the RAT mapped 32-bit address; For A53, this would be the same as dbgcell_baddr_raw */
    eCPT2Aggr_ID        aggr_id; /* CPT2 Aggregator ID */
    uint64_t            aggr_baddr_raw; /* CPT2 Aggregator base address in 48-bit system memory address space */
    uint64_t            aggr_baddr; /* CPT2 Aggregator base address in the CPU view. For R5, this would be the RAT mapped 32-bit address; For A53, this would be the same as aggr_baddr_raw */
    uint8_t             master_id; /* Master ID for the CPT2 probe; this needs to match with the definition in Keystone3 platform database file in emupack */
} cpt2_access_t;

static cpt2_access_t g_cpt2_table[NUM_CPT2_PROBES] __attribute__((unused)) =
{
    /* MSMC3 CPTracer2 instance 0 probes (eTraceAggregator_0 and eDebugCell_0) */
    { eCpTracer2_Probe_0, 0, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 224 },
    { eCpTracer2_Probe_1, 1, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 225 },
    { eCpTracer2_Probe_2, 2, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 226 },
    { eCpTracer2_Probe_3, 3, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 227 },
    { eCpTracer2_Probe_4, 4, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 228 },
    { eCpTracer2_Probe_5, 5, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 229 },
    { eCpTracer2_Probe_6, 8, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 232 },
    { eCpTracer2_Probe_7, 9, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 233 },
    { eCpTracer2_Probe_8, 12, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 236 },

    /* MSMC3 CPTracer2 instance 1 probes (eTraceAggregator_1 and eDebugCell_0) */
    { eCpTracer2_Probe_9,  0, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 32 },
    { eCpTracer2_Probe_10, 1, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 33 },
    { eCpTracer2_Probe_11, 5, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 38 },
    { eCpTracer2_Probe_12, 6, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 39 },
    { eCpTracer2_Probe_13, 9, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 42 },
    { eCpTracer2_Probe_14, 10, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 43 },

    /* SoC RC CPTracer2 probes (eTraceAggregator_11 and SOC eDebugCell_2) */
    { eCpTracer2_Probe_15, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 96 },
    { eCpTracer2_Probe_16, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 97 },
    { eCpTracer2_Probe_17, 2, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 98 },
    { eCpTracer2_Probe_18, 3, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 99 },
    { eCpTracer2_Probe_19, 4, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 100 },
    { eCpTracer2_Probe_20, 5, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 101 },

    /* SoC AC CPTracer2 probes (eTraceAggregator_9 and SOC eDebugCell_2) */
    { eCpTracer2_Probe_21, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 192 },
    { eCpTracer2_Probe_22, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 193 },
    { eCpTracer2_Probe_23, 2, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 194 },
    { eCpTracer2_Probe_24, 3, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 195 },
    { eCpTracer2_Probe_25, 4, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 196 },
    { eCpTracer2_Probe_26, 5, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 197 },
    { eCpTracer2_Probe_27, 6, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 198 },

    /* SoC MI CPTracer2 probes (eTraceAggregator_8 and SOC eDebugCell_2) */
    { eCpTracer2_Probe_28, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_MI_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_MI_CPT2_ID), _CPT2_BaseAddress(eSOC_MI_CPT2_ID), 64 },

    /* SoC ACP CPTracer2 probes (eTraceAggregator_12 and SOC eDebugCell_2) */
    { eCpTracer2_Probe_29, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_ACP_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_ACP_CPT2_ID), _CPT2_BaseAddress(eSOC_ACP_CPT2_ID), 160 },
    { eCpTracer2_Probe_30, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_ACP_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_ACP_CPT2_ID), _CPT2_BaseAddress(eSOC_ACP_CPT2_ID), 161 },

    /* MCU CPTracer2 probes (eTraceAggregator_6 and SOC eDebugCell_2) */
    { eCpTracer2_Probe_31, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 128 },
    { eCpTracer2_Probe_32, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 129 },
    { eCpTracer2_Probe_33, 2, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 130 },
    { eCpTracer2_Probe_34, 3, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 131 }

};

#ifdef __cplusplus
}
#endif

#endif 

// ;;;;;;;;;;;;;;;;;;;;;;;;;;
// TDA4VL list of RouteIDs
// ;;;;;;;;;;;;;;;;;;;;;;;;;;
// A72-CORE0    0       (0x000)
// A72-CORE1    1       (0x001)
// A72-L2Cache  4       (0x004)
// C7x-0        12      (0x00C)
// C7x-1        21      (0x015)
// DRU0        104     (0x068)
// DRU1        105     (0x069)
// IOMMU       108
// MSMC3       127     (0x07F)
// EMMC8SS0_WR 256 (0x100)
// EMMC8SS0_RD 257 (0x101)
// EMMCSD4SS0_WR	258 (0x102)
// EMMCSD4SS0_RD	259 (0x103)
// UFS0_RD	    274
// UFS0_WR	    275
// SA2_UL	    300
// COMPUTE_CLUSTER0_RD	302
// COMPUTE_CLUSTER0_WR	303
// USB0_RD	    304
// USB0_WR	    305
// LED	        334
// DEBUGSS0_RD	350
// DEBUGSS0_WR	351
// PCIE0_RD_HP	352
// PCIE0_WR_HP	353
// PCIE0_RD 	354
// PCIE0_RD_LP	354
// PCIE0_WR 	355
// PCIE0_WR_LP	355
// PCIE1_RD_HP  356
// PCIE1_WR_HP  357
// PCIE1_RD	    358
// PCIE1_RD_LP  358
// PCIE1_WR	    359
// PCIE1_WR_LP  359
// PCIE2_RD_HP	360
// PCIE2_WR_HP	361
// PCIE2_RD_LP	362
// PCIE2_RD	    362
// PCIE2_WR_LP	363
// PCIE2_WR	    363
// PCIE3_RD_HP	364
// PCIE3_WR_HP	365
// PCIE3_RD_LP 	366
// PCIE3_RD 	366
// PCIE3_WR_LP 	367
// PCIE3_WR 	367
// R5FSS0_CORE0_PER1_RD	522
// R5FSS0_CORE0_PER1_WR	523
// R5FSS0_CORE1_PER1_RD	530
// R5FSS0_CORE1_PER1_WR	531
// R5FSS1_CORE0_PER1_RD	538
// R5FSS1_CORE0_PER1_WR	539
// R5FSS1_CORE1_PER1_RD	546
// R5FSS1_CORE1_PER1_WR	547
// R5FSS2_CORE0_PER1_RD	554
// R5FSS2_CORE0_PER1_WR	555
// R5FSS2_CORE1_PER1_RD	556
// R5FSS2_CORE1_PER1_WR	557
// PDMA_SPI_G00_MEMW0	608
// PDMA_SPI_G00_MEMR0	609
// PDMA_SPI_G10_MEMW0	612
// PDMA_SPI_G10_MEMR0	613
// PDMA_UART_G00_MEMW0	616
// PDMA_UART_G00_MEMR0	617
// PDMA_UART_G10_MEMW0	618
// PDMA_UART_G10_MEMR0	619
// PDMA_UART_G20_MEMW0	620
// PDMA_UART_G20_MEMR0	621
// PDMA_MCAN_MEMW0 	622
// PDMA_MCAN_MEMR0	623
// PDMA_DEBUG_CCSOC0_MEMR0	624
// PDMA_MCASP0_MEMW0	624
// PDMA_MCASP0_MEMR0	625
// PDMA_DEBUG_CCSOC1_MEMR0	626
// VUSR0	640
// VUSR1	641
// VPAC0_LDC0	    2560
// VPAC0_DATA_MST0a	2564
// VPAC0_DATA_MST0b	2565
// VPAC0_DATA_MST1a	2566
// VPAC0_DATA_MST1b	2567
// DMPAC0_DATAa	    2568
// DMPAC0_DATAb	    2569
// VPAC1_LDC0 	    2570
// VPAC1_DATA_MST0a	2572
// VPAC1_DATA_MST0b	2573
// VPAC1_DATA_MST1a	2574
// VPAC1_DATA_MST1b	2575
// ENCODER0_RD	    2576
// ENCODER0_WR	    2577
// WAVE521CL0_P1_RD	    2578
// WAVE521CL 0_P1_WR	2579
// WAVE521CL1_P0_RD	    2580
// WAVE521CL1_P0_WR	    2581
// WAVE521CL1_P1_RD	    2582
// WAVE521CL 1_P1_WR	2583
// DSS0_DMA	    2592
// DSS0_FBDC	2593
// GPU0_M1_RD	2600
// GPU0_M0_RD	2601
// GPU0_M0_WR	2602
// GPU0_M1_WR	2603
// GPU0_XPUa	2604
// GPU0_XPUb	2605
// R5FSS0_CORE0_MEM_RD	2688
// R5FSS0_CORE0_MEM_WR	2689
// R5FSS0_CORE1_MEM_RD	2690
// R5FSS0_CORE1_MEM_WR	2691
// R5FSS1_CORE0_MEM_RD	2692
// R5FSS1_CORE0_MEM_WR	2693
// R5FSS1_CORE1_MEM_RD	2694
// R5FSS1_CORE1_MEM_WR	2695
// R5FSS2_CORE0_MEM_RD	2696
// R5FSS2_CORE0_MEM_WR	2697
// R5FSS2_CORE1_MEM_RD	2698
// R5FSS2_CORE1_MEM_WR	2699
// MCU_SA3_UL	        3713
// DMSS_HSM_PKTDMA	    4062
// MCU_PDMA_MISC_G00_MEMW0	4063
// MCU_PDMA_MISC_G00_MEMR0	4064
// MCU_PDMA_MISC_G10_MEMW0	4065
// MCU_PDMA_MISC_G10_MEMR0	4066
// MCU_PDMA_MISC_G20_MEMW0	4067
// MCU_PDMA_MISC_G20_MEMR0	4068
// MCU_PDMA_ADC0_MEMR0	    4069
// MCU_R5FSS0_CORE1_MEM_WR	4072
// MCU_R5FSS0_CORE1_MEM_RD	4073
// MCU_R5FSS0_CORE0_MEM_WR	4076
// MCU_R5FSS0_CORE0_MEM_RD	4077
// MCU_R5FSS0_CORE0_PER0	4078
// MCU_R5FSS0_CORE1_PER0	4079
// SMS Base ROUTEID 	    4080
// NAVSS MAIN	0-255,2560-3071
// NAVSS MCU	3584-3711
// SPARE_MST_RSVD 	368-375