/**
* @file device.h
* @brief J784S4 device specific definitions
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

#ifndef __DEVICE_J784S4_H
#define __DEVICE_J784S4_H

#include <stdlib.h>
#include <stdint.h>              // The library uses C99 exact-width integer types

#ifdef __cplusplus
extern "C" {
#endif

/*
* Note: This code is intended for use with J784S4 HS-FS devices only. 
* The CTOOLS library is not supported on GP or HS-FE device variants.
*
* The JTAGID (WKUP_CTRL_MMR0_JTAGID) is part of the Wakeup MMR and is required for converting
* binary trace data to CSV/TDF format using Code Composer Studio (CCS). For AHP devices, the JTAGID value is 0x0BB8002F.
*
* To convert binary trace output to CSV, use the following commands from the CCS emulation analysis tools directory:
*   1. Convert binary to TDF:
*      ./bin2tdf -bin /<user-path>/CPT2_ddr_throu.bin -rcvr ETB -output ddr_throughput.tdf -procid stm -devicekey 0x0BB8002F -dcmfile /<user-path>/CPT2_ddr_throu.dcm
*   2. Convert TDF to CSV:
*      ./td -bin ddr_throughput.tdf -rcvr ETB -output throughput.csv -procid stm -devicekey 0x0BB8002F -timestamp abs
*/

// CPTracer2 probe Offset address   (As, ProbeBase: AggBase+0x20000 + Prob#x0x1000)
#define CPT2_PROBEn_OFFSET(n)       (0x00020000 + n*0x0001000) // CPT2 probe N offset in Aggregator space

// Debug Cell sub-components offset address
#define TBR_OFFSET                  0x00005000 // TBR offset in Debug Cell space
#define ATBREP_OFFSET               0x00004000 // ATB Replicator offset in Debug Cell space

////////////////////////////////////////////////////////////////////
// Device Specific Definitions
////////////////////////////////////////////////////////////////////

////////// GTC definitions ///////////////////////////
// GTC base address in system memory view (GTC0_GTC_CFG1 in TRM)
#define GTC_BADDR       (0xa90000UL)   // Global time counter GTC base address, this address is 32-bit so no need to be re-mapped for R5
#define GTC_TIMER_MAPPED_SIZE       (512)

///////// Debug Cell definitions ////////////////////
/*! \par eDebugCell_ID
    Debug Cell ID
*/
// Debug Cells are present in Main Domain only
typedef enum
{
    eCC0_DBGCELL_ID = 0,                 /*!< 0 - Compute Cluster Debug Cell A72SS0*/
    eCC1_DBGCELL_ID = 1,                 /*!< 1 - C71SS MSMC2 Debug Cell */
    eSOC_DBGCELL_ID = 2,                 /*!< 2 - SoC Debug Cell */
    eCC2_DBGCELL_ID = 3                  /*!< 3 - Compute Cluster Debug Cell A72SS1*/
} eDebugCell_ID;

// Check Figure 13-2. SoC Trace Data Flow in TRM to understand better
// There are 4 debug cells in the device J784s4

// Debug Cell Base address in system memory view
// Processor access to all debug features (including JTAG scan) for full "self-hosted" debug
#define CC0_DBGCELL_BADDR      (0x4C3C000000) // Compute Cluster Debug Cell base address (CCDEBUGSS0_ROM). TBR Size: 65536 bytes
#define CC1_DBGCELL_BADDR      (0x4C3C010000) // C71SS MSMC2 Debug Cell base address (CCDEBUGSS1_ROM)
#define SOC_DBGCELL_BADDR      (0x4C3C020000) // SoC Debug Cell base address (DEBUGSS0_ROM). TBR Size: 32768 bytes
#define CC2_DBGCELL_BADDR      (0x4C3C030000) // A72 Sub-system1 Debug Cell base address (CCDEBUGSS2_ROM)

#define DBGCELL_MAPPED_SIZE       (0x10000) // 64KB

// Debug Cell base address in 48-bit system memory address space. This needs to be mapped to 32-bit address for R5 view.
#define _DBGCELL_BaseAddress_raw(dc_id) ((dc_id==eCC0_DBGCELL_ID)? CC0_DBGCELL_BADDR: \
                                        ((dc_id==eCC1_DBGCELL_ID)? CC1_DBGCELL_BADDR : \
                                        ((dc_id==eSOC_DBGCELL_ID)? SOC_DBGCELL_BADDR : CC2_DBGCELL_BADDR)))

#define _DBGCELL_BaseAddress(dc_id) _DBGCELL_BaseAddress_raw(dc_id)

// ETB base address
#define _ETB_BaseAddress(dc_id)		(_DBGCELL_BaseAddress(dc_id)+TBR_OFFSET)

// ATB Replicator base address
#define _ATBREP_BaseAddress(dc_id)	(_DBGCELL_BaseAddress(dc_id)+ATBREP_OFFSET)


////////// CPTracer2 definitions /////////////////////
//CPT2 Aggregators IDs (There are 17 aggregators which host probe groups.)
typedef enum
{
    eMSMC0_CPT2_ID  = 0,                 /*!< 0 - MSMC0 CPT2 Aggregator */
    eMSMC1_CPT2_ID  = 1,                 /*!< 1 - MSMC1 CPT2 Aggregator */
    eSOC_RC_CPT2_ID = 2,                 /*!< 2 - SOC-RC CPT2 Aggregator */
    eSOC_AC_CPT2_ID = 3,                 /*!< 3 - SOC AC CPT2 Aggregator */
    eSOC_HC_CPT2_ID = 4,                 /*!< 4 - SOC HC CPT2 Aggregator */
    eSOC_MV_CPT2_ID = 5,                 /*!< 5 - SOC MV CPT2 Aggregator */
    eSOC_MI_CPT2_ID = 6,                 /*!< 6 - SOC MI CPT2 Aggregator */
    eSOC_ACP_CPT2_ID= 7,                 /*!< 7 - SOC ACP CPT2 Aggregator */
    eMCU_CPT2_ID    = 8,                 /*!< 8 - MCU CPT2 Aggregator    */
    eC71_CPT2_ID    = 9,                 /*!< 9 - C7-1 CPT2 Aggregator   */
    eC72_CPT2_ID    = 10,                /*!< 10 - C7-2 CPT2 Aggregator   */
    eC73_CPT2_ID    = 11,                /*!< 11 - C7-3 CPT2 Aggregator   */
    eC74_CPT2_ID    = 12,                /*!< 12 - C7-4 CPT2 Aggregator   */
    eMSMC1_0_CPT2_ID = 13,               /*!< 13 - MSMC1_0 CPT2 Aggregator */
    eMSMC1_1_CPT2_ID = 14,               /*!< 14 - MSMC1_1 CPT2 Aggregator */
    eMSMC1_2_CPT2_ID = 15,               /*!< 15 - MSMC1_2 CPT2 Aggregator */
    eMSMC1_3_CPT2_ID = 16                /*!< 16 - MSMC1_3 CPT2 Aggregator */
} eCPT2Aggr_ID ;

// NOTE: C7x interacts with local MSMC which further engage with system MSMC. (Device System Interconnect in TRM)

// Debug Address (ABP) : 0x9xxx_xxxx --> System Address (AXI) 0x4C:0x3xxx_xxxx
// CPTracer2 Aggregator Base address in system memory view
#define MSMC0_CPT2_BADDR           (0x4C30140000)   // COMPUTE_CLUSTERHP0_VBUSP_DBG_AGR0_DBG_AGR0
#define MSMC1_CPT2_BADDR           (0x4C30180000)   // COMPUTE_CLUSTERHP0_VBUSP_DBG_AGR1_DBG_AGR1
#define SOC_RC_CPT2_BADDR          (0x4C3E100000)   // CPT2_AGGR0_MMR
#define SOC_AC_CPT2_BADDR          (0x4C3E180000)   // CPT2_AGGR1_MMR
#define SOC_HC_CPT2_BADDR          (0x4C3E200000)   // CPT2_AGGR2_MMR
#define SOC_MV_CPT2_BADDR          (0x4C3E1C0000)   // CPT2_AGGR3_MMR
#define SOC_MI_CPT2_BADDR          (0x4C3E140000)   // CPT2_AGGR4_MMR
#define SOC_ACP_CPT2_BADDR         (0x4C3E240000)   // CPT2_AGGR5_MMR
#define MCU_CPT2_BADDR			   (0x4C3E000000)   // MCU_CPT2_AGGR0_MMR
#define C71_CPT2_BADDR             (0x4C31440000)   // C7-1 CPT2 Aggregator
#define C72_CPT2_BADDR             (0x4C31840000)   // C7-2 CPT2 Aggregator
#define C73_CPT2_BADDR             (0x4C31C40000)   // C7-3 CPT2 Aggregator
#define C74_CPT2_BADDR             (0x4C32040000)   // C7-4 CPT2 Aggregator
#define MSMC1_0_CPT2_BADDR         (0x4C33040000)   // MSMC1_0 CPT2 Aggregator (COMPUTE_CLUSTERHP0_VBUSP_CFG_AN_4_VBUSP_DBG_AGR0_DBG_AGR0)
#define MSMC1_1_CPT2_BADDR         (0x4C33140000)   // MSMC1_1 CPT2 Aggregator (COMPUTE_CLUSTERHP0_VBUSP_CFG_AN_5_VBUSP_DBG_AGR0_DBG_AGR0)
#define MSMC1_2_CPT2_BADDR         (0x4C33240000)   // MSMC1_2 CPT2 Aggregator (COMPUTE_CLUSTERHP0_VBUSP_CFG_AN_6_VBUSP_DBG_AGR0_DBG_AGR0)
#define MSMC1_3_CPT2_BADDR         (0x4C33340000)   // MSMC1_3 CPT2 Aggregator (COMPUTE_CLUSTERHP0_VBUSP_CFG_AN_7_VBUSP_DBG_AGR0_DBG_AGR0)

#define CPT2_AGGR_MAPPED_SIZE      (0x40000) // 256KB


#define _CPT2_BaseAddress_raw(cpt2_id) ((cpt2_id==eMSMC0_CPT2_ID) ? MSMC0_CPT2_BADDR : \
                                       ((cpt2_id==eMSMC1_CPT2_ID) ? MSMC1_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_RC_CPT2_ID) ? SOC_RC_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_AC_CPT2_ID) ? SOC_AC_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_HC_CPT2_ID) ? SOC_HC_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_MV_CPT2_ID) ? SOC_MV_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_MI_CPT2_ID) ? SOC_MI_CPT2_BADDR : \
                                       ((cpt2_id==eSOC_ACP_CPT2_ID) ? SOC_ACP_CPT2_BADDR : \
                                       ((cpt2_id==eMCU_CPT2_ID) ? MCU_CPT2_BADDR : \
                                       ((cpt2_id==eC71_CPT2_ID) ? C71_CPT2_BADDR : \
                                       ((cpt2_id==eC72_CPT2_ID) ? C72_CPT2_BADDR : \
                                       ((cpt2_id==eC73_CPT2_ID) ? C73_CPT2_BADDR : \
                                       ((cpt2_id==eC74_CPT2_ID) ? C74_CPT2_BADDR : \
                                       ((cpt2_id==eMSMC1_0_CPT2_ID) ? MSMC1_0_CPT2_BADDR : \
                                       ((cpt2_id==eMSMC1_1_CPT2_ID) ? MSMC1_1_CPT2_BADDR : \
                                       ((cpt2_id==eMSMC1_2_CPT2_ID) ? MSMC1_2_CPT2_BADDR : MSMC1_3_CPT2_BADDR ))))))))))))))))

#define _CPT2_BaseAddress(cpt2_id)  _CPT2_BaseAddress_raw(cpt2_id)

// CPTracer2 probe base address
#define _CPT2Probe_BaseAddress(cpt2_id, pb_index)	(_CPT2_BaseAddress(cpt2_id) + CPT2_PROBEn_OFFSET(pb_index))

// Note: CFG mapping is the same of all K3, e.g. CSI8_TARGET is always #12 for all K3 devices

// For clock frequency, Run: k3conf dump clock
// It gives the clock frequency in Hz along with device id and clock id.
// For MSMC Probes, MSMC0_CPT2_BADDR and MSMC1_CPT2_BADDR, the frequency of clock is 1000000000 Hz = 1 GHz  (device id: 4, clock id: 1, clock name: DEV_A72SS0_MSMC_CLK)
// For MCU_CPT2_BADDR Probes, the frequency of clock is 333333333 Hz = 333.33 MHz (device id: 24, clock id: 0, clock name: DEV_MCU_CPT2_AGGR0_VCLK_CLK)
// For SOC_CPT2_BADDR Probes, the frequency of clock is 500000000 Hz = 500 MHz (device id: 20, clock id: 0, clock name: DEV_CPT2_AGGR0_VCLK_CLK)
// For SOC_AC_CPT2_BADDR Probes, the frequency of clock is 500000000 Hz = 500 MHz (device id: 21, clock id: 0, clock name: DEV_CPT2_AGGR1_VCLK_CLK)
// For SOC_HC_CPT2_BADDR Probes, the frequency of clock is 250000000 Hz = 250 MHz (device id: 23, clock id: 0, clock name: DEV_CPT2_AGGR2_VCLK_CLK)

/*
* ============================================================================
* CPTracer2 Probe Mappings: CPU, DSPx, MSMC1x, MSMC2, NAVSS <-> MSMC3 VBUSM.C
* ============================================================================
* - CSIx (Coherent Slave Interface) Probes:
*   - CSI0_T/I: A72 Cluster 1 (A72x4) <-> MSMC (Target/Initiator)
*   - CSI1_T/I: A72 Cluster 2 (A72x4) <-> MSMC (Target/Initiator)
*   - CSI4_T/I: MSMC2 (4x C7x DSPs connected) <-> MSMC (Target/Initiator)
*   - CSI8_T:   NAVSS North Bridge 1 (SoC <-> MSMC:DDR) (Target)
*   - CSI9_T/I: NAVSS North Bridge 0 (CPU:MSMC <-> SoC & MSMC:SRAM) (Target/Initiator)
*
* - MSMC-IP Probes:
*   - DRUxT/I: Data Routing Unit (DMA engines for L3, L2, DDR transfers; Target/Initiator)
*   - EMIF0/1/2/3: DDR endpoints at the top of MSMC (External Memory Interfaces)
*
* Probe roles:
*   - "Target" probes monitor incoming transactions to MSMC/DDR.
*   - "Initiator" probes monitor outgoing transactions from MSMC/DDR.
*
* These mappings enable helps trace analysis across CPU, DSP, MSMC, and NAVSS interconnects.
*/

#define NUM_CPT2_PROBES    37 // Number of CPTracer2 probes

/*! \par cpt2pb_id_t
    CPTracer2 probe IDs
*/
typedef enum {
    /* MSMC3 Probes are handles by Debug Cell Compute Cluster 1 (In TRM at Figure 13-2. SoC Trace Data Flow) */

    /* MSMC CPTracer2 instance 0 probes (Master Id: 224 + Probe Port Index) */
    eCpTracer2_Probe_0,        /**< 0 - "DRU0_TARGET (Data Routing Unit 0 (DRU0) Read Probe (Target) - VBUSM.C)" */
	eCpTracer2_Probe_1,        /**< 1 - "DRU0_INITIATOR (Data Routing Unit 0 (DRU0) Initiator Probe (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_2,        /**< 2 - "DRU1_TARGET (Data Routing Unit 1 (DRU1) Read Probe (Target) - VBUSM.C)" */
	eCpTracer2_Probe_3,        /**< 3 - "DRU1_INITIATOR (Data Routing Unit 1 (DRU1) Initiator Probe (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_4,        /**< 4 - "CSI0_TARGET ARM CLUST0 (Coherent Slave Interface: A72 CorePac 0 (Target) - VBUSM.C)" */
	eCpTracer2_Probe_5,        /**< 5 - "CSI0_INITIATOR ARM CLUST0 (Coherent Slave Interface: A72 CorePac 0 (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_6,        /**< 6 - "CSI4_TARGET (MSMC2 (Target) - VBUSM.C)" */
	eCpTracer2_Probe_7,        /**< 7 - "CSI4_INITIATOR (MSMC2 (Initiator) - VBUSM.C)" */
	eCpTracer2_Probe_8,        /**< 8 - "CSI8_TARGET (SoC 1 (NavSS Northbridge 1) (Target) - VBUSM.C)" */

	/* MSMC CPTracer2 instance 1 probes (Master Id: 32 + Probe Port Index) */
    eCpTracer2_Probe_9,       /**< 9 -  "EMIF0_INITIATOR (External Memory Interface (EMIF) 0 Probe (Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_10,      /**< 10 - "EMIF1_INITIATOR (External Memory Interface (EMIF) 1 Probe (Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_11,      /**< 11 - "CSI1_TARGET ARM CLUST1 (A72 CorePac 1 (Target) - VBUSM.C)" */
    eCpTracer2_Probe_12,      /**< 12 - "CSI1_INITIATOR ARM CLUST1 (A72 CorePac 1 (Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_13,      /**< 13 - "CSI9_TARGET (SoC 0 (NavSS Northbridge 0)(Target) - VBUSM.C)" */
	eCpTracer2_Probe_14,      /**< 14 - "CSI9_INITIATOR (SoC 0 (NavSS Northbridge 0)(Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_15,      /**< 15 - "EMIF3_INITIATOR (External Memory Interface (EMIF) 3 Probe (Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_16,      /**< 16 - "EMIF2_INITIATOR (External Memory Interface (EMIF) 2 Probe (Initiator) - VBUSM.C)" */

    /* For SOC RC, AC, HC, MI, ACP and MCU Probes, we use Debug Cell SOC i.e SOC_DBGCELL_BADDR */

    /* SoC-RC CPTracer2 probes (Master Id: 96 + Probe Port Index) */
    eCpTracer2_Probe_17,       /**< 17 - "NAV_DDR_0 - VBUSM"  */
    eCpTracer2_Probe_18,       /**< 18 - "NAV_DDR_1 - VBUSM"  */
	eCpTracer2_Probe_19,       /**< 19 - "NAVSS_HMAST (Hardware Multiple Address Translation and Stepping) - VBUSM" */
	eCpTracer2_Probe_20,       /**< 20 - "NAVSS_LMAST - VBUSM" */
	eCpTracer2_Probe_21,       /**< 21 - "NAVSS_SRAM_0 - VBUSM" */
	eCpTracer2_Probe_22,       /**< 22 - "NAVSS_SRAM_1 - VBUSM" */

    /* SoC AC CPTracer2 probes (Master Id: 192 + Probe Port Index) */
	eCpTracer2_Probe_23,       /**< 23 - "AC_CBASS_SLV_0 (AC NAV DDR SLV 0)" */
	eCpTracer2_Probe_24,       /**< 24 - "AC_CBASS_SLV_1 (AC NAV DDR SLV 1)" */
	eCpTracer2_Probe_25,       /**< 25 - "AC_CBASS_SLV_2 (AC NAV DDR SLV 2)" */
	eCpTracer2_Probe_26,       /**< 26 - "AC_CBASS_SLV_4 (AC NAV SRAM SLV 0)" */
    eCpTracer2_Probe_27,       /**< 27 - "DMPAC SRAM SLV (Clock Name: DEV_DMPAC0_CLK)" */
	eCpTracer2_Probe_28,       /**< 28 - "VPAC_SRAM_SLV0 (VPAC0 SRAM SLV) (Clock Name: DEV_VPAC0_MAIN_CLK)" */
	eCpTracer2_Probe_29,       /**< 29 - "VPAC_SRAM_SLV1 (VPAC1 SRAM SLV) (Clock Name: DEV_VPAC1_MAIN_CLK)"  */

    /* SOC MI Probes (Master Id: 64 + Probe Port Index) */
    eCpTracer2_Probe_30,	  /**< 30 - "ONCHIP_SRAM PULSAR0 - VBUSM" */

    /* SOC ACP Probes (Only present in J7AHP and J7AM devices) (Master Id: 160 + Probe Port Index) */
    eCpTracer2_Probe_31,	  /**< 31 - "ONCHIP_SRAM_PULSAR1 - VBUSM" */
    eCpTracer2_Probe_32,	  /**< 32 - "ONCHIP_SRAM_PULSAR2 - VBUSM" */  

    /* SOC MV Probes */
    // NAVSS Ring Accel (Custom Trace) whose Probe Buffer Size is not defined.

    /* MCU CPTracer2 probes (Master Id: 128 + Probe Port Index) */
	eCpTracer2_Probe_33,	  /**< 33 - "MCU_EXPORT_SLV_0 (Transactions from wakeup and MCU domains to the main domain)" */
	eCpTracer2_Probe_34,	  /**< 34 - "MCU_SRAM_SLV_1 (Transactions to the on-chip MSRAM in the MCU domain - VBUSM)"   */
	eCpTracer2_Probe_35,	  /**< 35 - "MCU_FSS_S0_2 (Transactions to the FSS slave port 0: first OSPI and Hyperbus - VBUSM) (Clock Name: DEV_MCU_FSS0_OSPI_0_OSPI_HCLK_CLK)" */
	eCpTracer2_Probe_36  	  /**< 36 - "MCU_FSS_S1_3 (Transactions to the second OSPI - VBUSM) (Clock Name: DEV_MCU_FSS0_OSPI_1_OSPI_HCLK_CLK)" */

    // The following aggregators are not supported by CCS as CCS supports only 7 aggregators at a time. We need another DPK JSONC file to use these aggregators.
    
    /* SoC HC CPTracer2 probes (Master Id: 160 + Probe Port Index) (Probe Index Respectively: 0, 1, 2, 3, 4, 5) */
    // SOC HC has following probes: 
	/**< "PCIE__SLV_1" */
	/**< "PCIE_VUSR_SLV_0" */
	/**< "PCIE_VUSR_SLV_1" */
	/**< "PCIE_SLV_0 - VBUSM" */
	/**< "PCIE_SLV_2 - VBUSM" */
	/**< "PCIE_SLV_3 - VBUSM" */
    
    /* For C7x and MSMC1 Aggregators, we use Debug Cell Compute Cluster 1 (Naming according to the picture: Figure 13-2. SoC Trace Data Flow in TRM) i.e CC0_DBGCELL_BADDR acc. to device.h */

    /* Per C7x core, L1 and SE Probes (Probe Index Respectively: 0, 1, 2, 3, 4, 5) */
    /* C7-1 CPTracer2 (Master Id: 64 + Probe Port Index) */
    /* C7-2 CPTracer2 (Master Id: 96 + Probe Port Index) */
    /**< "L1D_L2 (Probe on L1D initiator to L2) - VBUSM.C" */     
    /**< "L1P_L2 (Probe on L1P initiator to L2) - VBUSM.C" */     
    /**< "SE0_L2 (Probe on SE0 initiator to L2) - VBUSM.C" */     
    /**< "SE1_L2 (Probe on SE1 initiator to L2) - VBUSM.C" */    
    /**< "L2_L1D (Probe on L2 Initiator to L1D) - VBUSM.C" */     
    /**< "MMU_L2 (Probe on MMU-L2) - VBUSM.C" */                

    /*Per C7x, Local MSMC1 Probes (Probe Index Respectively: 0, 1, 2, 3, 4, 5, 6, 7)*/
    /* MSMC1_0 CPTracer2 probes (Master Id: 192 + Probe Port Index) */
    /**< "DRU0_TARGET" */    
    /**< "DRU0_INITIATOR" */
    /**< "DRU1_TARGET" */
    /**< "DRU1_INITIATOR" */
    /**< "CSI0_TARGET C7x (Coherent Slave Interface: C7x CorePac 0 (Target) - VBUSM.C)" */
    /**< "CSI0_INITIATOR C7x (Coherent Slave Interface: C7x CorePac 0 (Initiator) - VBUSM.C)" */
    /**< "CSI2_TARGET MSMC2 (Coherent Slave Interface 2 (Target) – MSMC2)" */
    /**< "CSI2_INITIATOR MSMC2 (Coherent Slave Interface 2 (Initiator) – MSMC2)" */

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
    /* MSMC3 CPTracer2 instance 0 probes */
    { eCpTracer2_Probe_0, 0, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 224 },
    { eCpTracer2_Probe_1, 1, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 225 },
    { eCpTracer2_Probe_2, 2, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 226 },
    { eCpTracer2_Probe_3, 3, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 227 },
    { eCpTracer2_Probe_4, 4, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 228 },
    { eCpTracer2_Probe_5, 5, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 229 },
    { eCpTracer2_Probe_6, 8, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 232 },
    { eCpTracer2_Probe_7, 9, eCC0_DBGCELL_ID,  _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 233 },
    { eCpTracer2_Probe_8, 12, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC0_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC0_CPT2_ID), _CPT2_BaseAddress(eMSMC0_CPT2_ID), 236 },

    /* MSMC3 CPTracer2 instance 1 probes */
    { eCpTracer2_Probe_9,  0, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 32 },
    { eCpTracer2_Probe_10, 1, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 33 },
    { eCpTracer2_Probe_11, 2, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 34 },
    { eCpTracer2_Probe_12, 3, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 35 },
    { eCpTracer2_Probe_13, 10, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 42 },
    { eCpTracer2_Probe_14, 11, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 43 },
    { eCpTracer2_Probe_15, 30, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 62 },
    { eCpTracer2_Probe_16, 31, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 63 },

    /* SoC RC CPTracer2 probes */
    { eCpTracer2_Probe_17, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 96 },
    { eCpTracer2_Probe_18, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 97 },
    { eCpTracer2_Probe_19, 2, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 98 },
    { eCpTracer2_Probe_20, 3, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 99 },
    { eCpTracer2_Probe_21, 4, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 100 },
    { eCpTracer2_Probe_22, 5, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_RC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_RC_CPT2_ID), _CPT2_BaseAddress(eSOC_RC_CPT2_ID), 101 },

    /* SoC AC CPTracer2 probes */
    { eCpTracer2_Probe_23, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 192 },
    { eCpTracer2_Probe_24, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 193 },
    { eCpTracer2_Probe_25, 2, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 194 },
    { eCpTracer2_Probe_26, 3, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 195 },
    { eCpTracer2_Probe_27, 4, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 196 },
    { eCpTracer2_Probe_28, 5, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 197 },
    { eCpTracer2_Probe_29, 6, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_AC_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_AC_CPT2_ID), _CPT2_BaseAddress(eSOC_AC_CPT2_ID), 198 },

    /* SoC MI CPTracer2 probes */
    { eCpTracer2_Probe_30, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_MI_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_MI_CPT2_ID), _CPT2_BaseAddress(eSOC_MI_CPT2_ID), 64 },

    /* SoC ACP CPTracer2 probes */
    { eCpTracer2_Probe_31, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_ACP_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_ACP_CPT2_ID), _CPT2_BaseAddress(eSOC_ACP_CPT2_ID), 160 },
    { eCpTracer2_Probe_32, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eSOC_ACP_CPT2_ID, _CPT2_BaseAddress_raw(eSOC_ACP_CPT2_ID), _CPT2_BaseAddress(eSOC_ACP_CPT2_ID), 161 },
    
    /* MCU CPTracer2 probes */
    { eCpTracer2_Probe_33, 0, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 128 },
    { eCpTracer2_Probe_34, 1, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 129 },
    { eCpTracer2_Probe_35, 2, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 130 },
    { eCpTracer2_Probe_36, 3, eSOC_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eSOC_DBGCELL_ID), _DBGCELL_BaseAddress(eSOC_DBGCELL_ID), eMCU_CPT2_ID, _CPT2_BaseAddress_raw(eMCU_CPT2_ID), _CPT2_BaseAddress(eMCU_CPT2_ID), 131 },

};

#ifdef __cplusplus
}
#endif

#endif 

// If we want to filter multiple RouteIDs, we can use the Route ID mask. 
// Say, If you wanna track all the transactions from C7x core, you can use the mask: 0xFF0
// If you wanna track all the transactions from only one of C7x core only, you can use the mask: 0xFFC

// ;;;;;;;;;;;;;;;;;;;;;;;;;;
// TDA4VH list of RouteIDs
// ;;;;;;;;;;;;;;;;;;;;;;;;;;
// A72-CORE0-C0	0    (0x000)
// A72-CORE1-C0	1    (0x001)
// A72-CORE2-C0	2    (0x002)
// A72-CORE3-C0	3    (0x003)
// A72-L2Cache-C0	4    (0x004)
// A72-CORE0-C1	8    (0x008)
// A72-CORE1-C1	9    (0x009)
// A72-CORE2-C1	10   (0x00A)
// A72-CORE3-C1	11   (0x00B)
// A72-L2Cache-C1	12   (0x00C)
// C70-CORE0	32    (0x020)
// C70-DRU0	33    (0x021)
// C70-DRU1	34    (0x022)
// C70-CMMU	35    (0x023)
// C71-CORE0	36    (0x024)
// C71-DRU0	37    (0x025)
// C71-DRU1	38    (0x026)
// C71-CMMU	39    (0x027)
// C72-CORE0	40    (0x028)
// C72-DRU0	41    (0x029)
// C72-DRU1	42    (0x02A)
// C72-CMMU	43    (0x02B)
// C73-CORE0	44    (0x02C)
// C73-DRU0	45    (0x02D)
// C73-DRU1	46    (0x02E)
// C73-CMMU	47    (0x02F)
// DRU00   104
// DRU01   105
// DRU10   106
// DRU11   107
// DRU20   108
// DRU21   109
// DRU30   110
// DRU31   111
// IOMMU	112
// MSMC3	127   (0x07F)
// EMMC8SS0_WR	256
// EMMC8SS0_RD	257
// EMMCSD4SS0_WR	258
// EMMCSD4SS0_RD	259
// UFS0_RD	274
// UFS0_WR	275
// SA2_UL	300
// COMPUTE_CLUSTER0_RD	302
// COMPUTE_CLUSTER0_WR	303
// USB0_RD	304
// USB0_WR	305
// LED	334
// DEBUGSS0_RD	350
// DEBUGSS0_WR	351
// PCIE0_RD_HP	352
// PCIE0_WR_HP	353
// PCIE0_RD 	354
// PCIE0_RD_LP	354
// PCIE0_WR 	355
// PCIE0_WR_LP	355
// PCIE1_RD_HP 	356
// PCIE1_WR_HP 	357
// PCIE1_RD	    358
// PCIE1_RD_LP 	358
// PCIE1_WR	    359
// PCIE1_WR_LP 	359
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
// PDMA_MCAN_MEMW0      622
// PDMA_MCAN_MEMR0	    623
// PDMA_DEBUG_CCSOC0_MEMR0	624
// PDMA_MCASP0_MEMW0	624
// PDMA_MCASP0_MEMR0	625
// PDMA_DEBUG_CCSOC1_MEMR0	626
// VUSR0	    640
// VUSR1	    641
// VPAC0_LDC0	2560
// VPAC0_DATA_MST0a	2564
// VPAC0_DATA_MST0b	2565
// VPAC0_DATA_MST1a	2566
// VPAC0_DATA_MST1b	2567
// DMPAC0_DATAa	2568
// DMPAC0_DATAb	2569
// VPAC1_LDC0 	2570
// VPAC1_DATA_MST0a	2572
// VPAC1_DATA_MST0b	2573
// VPAC1_DATA_MST1a	2574
// VPAC1_DATA_MST1b	2575
// ENCODER0_RD	    2576
// ENCODER0_WR	   2577
// WAVE521CL0_P1_RD	2578
// WAVE521CL 0_P1_WR	2579
// WAVE521CL1_P0_RD	2580
// WAVE521CL1_P0_WR	2581
// WAVE521CL1_P1_RD	2582
// WAVE521CL1_P1_WR	2583
// DSS_INST0_VBUSM_DMA	2592  (0xA20)
// DSS_INST0_VBUSM_FBDC	2593  (0xA21)
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
// MCU_SA3_UL	    3713
// DMSS_HSM_PKTDMA	4062
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
// NAVSS MAIN	0-255,2560-3071: 
// AC Master 0-5: 
// ac_mst_port_routeids[0]	2560-2687 2700-3071
// ac_mst_port_routeids[1]	2560-2687 2700-3071
// ac_mst_port_routeids[2]	2560-2687 2700-3071
// ac_mst_port_routeids[3]	2560-2687 2700-3071
// ac_mst_port_routeids[4]	2688-2691
// ac_mst_port_routeids[5]	2692-2699
// DMA Master 0-5: 
// dma_mst_port_routeids[0]	168 200
// dma_mst_port_routeids[1]	169
// dma_mst_port_routeids[2]	170
// dma_mst_port_routeids[3]	171
// dma_mst_port_routeids[4]	196
// dma_mst_port_routeids[5]	197
// MODSS Master 0-3: 
// mod_mst_port_routeids[0]	160 216 255-2559 3072-4095
// mod_mst_port_routeids[1]	160 216 255-2559 3072-4095
// mod_mst_port_routeids[2]	160 216 255-2559 3072-4095
// mod_mst_port_routeids[3]	160 216 255-2559 3072-4095
// Some other route IDs in NAVSS MAIN: 
// proxy0	        160
// sec_proxy0	    216
// udmap0.mem0	    168   (0x0A8)
// udmap0.mem1	    169   (0x0A9)  
// udmap0.umemw	    170   (0x0AA)  
// udmap0.umemr	    171   (0x0AB)  
// pvu_routeid[0]	208
// pvu_routeid[1]	209
// pvu_routeid[2]	210
// pvu_routeid[3]	211
// ringacc0.dst	    200   (0x0C8)
// NAVSS MCU	3584-3711: 
// proxy0	        3616
// sec_proxy0	    3640
// ringacc0.dst	    3624
// udmap0.mem0	    3584
// udmap0.mem1	    3585
// udmap0.umemw	    3586
// udmap0.umemr	    3587
// SPARE_MST_RSVD 	368-375
