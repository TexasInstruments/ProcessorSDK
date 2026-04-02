/**
* @file device.h
* @brief J742S2 device specific definitions
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

#ifndef __DEVICE_J742S2_H
#define __DEVICE_J742S2_H

#include <stdlib.h>
#include <stdint.h>              // The library uses C99 exact-width integer types

#ifdef __cplusplus
extern "C" {
#endif

/*
* Note: We don't currently have DPK enabled for J742S2 device. So, to convert the binary to human readable format, use the J784S4 JTAGID value (0x0BB8002F).
* As 2 EMIF probes of J742S2 are overlapping with J784S4 device.
*/

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
//CPT2 Aggregators IDs
typedef enum
{
    eMSMC0_CPT2_ID  = 0,                 /*!< 0 - MSMC0 CPT2 Aggregator */
    eMSMC1_CPT2_ID  = 1,                 /*!< 1 - MSMC1 CPT2 Aggregator */
} eCPT2Aggr_ID ;

// Debug Address (ABP) : 0x9xxx_xxxx --> System Address (AXI) 0x4C:0x3xxx_xxxx
// CPTracer2 Aggregator Base address in system memory view
#define MSMC0_CPT2_BADDR           (0x4C30140000)
#define MSMC1_CPT2_BADDR           (0x4C30180000)

#define CPT2_AGGR_MAPPED_SIZE      (0x40000) // 256KB

#define _CPT2_BaseAddress_raw(cpt2_id) ((cpt2_id==eMSMC0_CPT2_ID) ? MSMC0_CPT2_BADDR : MSMC1_CPT2_BADDR )

#define _CPT2_BaseAddress(cpt2_id)  _CPT2_BaseAddress_raw(cpt2_id)

// CPTracer2 probe base address
#define _CPT2Probe_BaseAddress(cpt2_id, pb_index)	(_CPT2_BaseAddress(cpt2_id) + CPT2_PROBEn_OFFSET(pb_index))

#define NUM_CPT2_PROBES    4 // Number of CPTracer2 probes

/*! \par cpt2pb_id_t
    CPTracer2 probe IDs
*/
typedef enum {

    /* MSMC CPTracer2 instance 0 probes (Master Id: 224 + Probe Port Index) */
    eCpTracer2_Probe_0,        /**< 0 - "DRU0_TARGET (Data Routing Unit 0 (DRU0) Read Probe (Target) - VBUSM.C)" */
	eCpTracer2_Probe_1,        /**< 1 - "DRU0_INITIATOR (Data Routing Unit 0 (DRU0) Initiator Probe (Initiator) - VBUSM.C)" */

	/* MSMC CPTracer2 instance 1 probes (Master Id: 32 + Probe Port Index) */
    eCpTracer2_Probe_2,       /**< 2 -  "EMIF0_INITIATOR (External Memory Interface (EMIF) 0 Probe (Initiator) - VBUSM.C)" */
    eCpTracer2_Probe_3,      /**< 3 - "EMIF1_INITIATOR (External Memory Interface (EMIF) 1 Probe (Initiator) - VBUSM.C)" */

} cpt2pb_id_t;

// DDR probe
#define eCpTracer2_DDR_Probe eCpTracer2_Probe_2

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

    /* MSMC3 CPTracer2 instance 1 probes */
    { eCpTracer2_Probe_2,  0, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 32 },
    { eCpTracer2_Probe_3, 1, eCC0_DBGCELL_ID, _DBGCELL_BaseAddress_raw(eCC0_DBGCELL_ID), _DBGCELL_BaseAddress(eCC0_DBGCELL_ID), eMSMC1_CPT2_ID, _CPT2_BaseAddress_raw(eMSMC1_CPT2_ID), _CPT2_BaseAddress(eMSMC1_CPT2_ID), 33 },
};

#ifdef __cplusplus
}
#endif

#endif 
