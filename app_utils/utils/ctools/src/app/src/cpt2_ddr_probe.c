/*
* CPT2 DDR Application
*
* Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
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

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <inttypes.h>
#include "cpt2.h"
#include "tbr.h"
#include "gtc.h"
#include "atbrep.h"
#include "td_error.h"
#include "trace_aggr.h"
#include "cpt2_ddr_probe.h"
#include "target_access.h"
#include "target_access_direct.h"
#include <utils/console_io/include/app_log.h>
#include <unistd.h>
#include <utils/mem/include/app_mem.h>
#include <utils/timer/include/app_timer.h>

/****************************************************************************/
// Global Variables
/****************************************************************************/
tbr_handle_t tbrHandle;
cpt2_handle_t cpt2Handle;
trace_aggr_handle_t traceAggrHandle;
atbrep_handle_t atbRepHandle;
gtc_handle_t gtcHandle;
cpt2pb_id_t cpt2_probe_id;
uint8_t cpt2_mst_id;

char pBinFileName[100];
char pDcmFileName[100];
ADDR_TYPE dbgcell_phys_baddr;
ADDR_TYPE cpt2_aggr_phys_baddr;

#define DEFAULT_ATB_ID      0x30 // This value can be changed to anything with 0-127 range. ATB ID (Advanced Trace Bus): Identifies the ATB channel/port used for routing trace data from the CPTracer2 probe to the TBR in ARM CoreSight systems.

uint8_t CPT2H_get_device_info(uint8_t *pdbgcell_id, ADDR_TYPE * pdbgcell_baddr, ADDR_TYPE * patbrep_baddr, ADDR_TYPE * pcpt2_aggr_baddr, ADDR_TYPE * pcpt2_probe_baddr, uint8_t *pcpt2_mst_id);
uint8_t trace_open(cpt2_options_t * p_options);
uint8_t trace_enable(cpt2_options_t * p_options);
uint8_t run_ddr(cpt2_options_t * p_options);
uint8_t trace_disable(void);
uint8_t trace_close(void);
const char* get_mode_str(cpt2_op_mode_t mode);
uint8_t CPT2TBR_PostProcess(cpt2_options_t * p_options, tbr_handle_t tbrHandle, uint32_t atb_id, uint32_t* pBuffer, uint32_t bufferWords);
uint8_t transferTBRData(tbr_handle_t tbrHandle, tbr_status_t tbrStatus, uint32_t wordsToRead, uint32_t* pBuffer);
uint8_t transferCPT2Config(cpt2_options_t * p_options, uint32_t atb_id);

int appCpt2EnableProbe(cpt2_options_t cpt2Options, cpt2pb_id_t probe_id)
{
    cpt2_probe_id = probe_id;

    const char* mode_str = get_mode_str(cpt2Options.mode);
    uint64_t time_usec = appLogGetGlobalTimeInUsec();
    snprintf(pBinFileName, sizeof(pBinFileName), "/tmp/CPT2_ddr_%s_%llu.bin", mode_str, (unsigned long long)time_usec);
    snprintf(pDcmFileName, sizeof(pDcmFileName), "/tmp/CPT2_ddr_%s_%llu.dcm", mode_str, (unsigned long long)time_usec);

    if (0 != run_ddr(&cpt2Options))
    {
        appLogPrintf("run_ddr failed!\n");
        return 1;
    }
    
    return 0;
}

/****************************************************************************
* get_mode_str()
* Returns a string representation of the CPTracer2 operation mode.
****************************************************************************/
const char* get_mode_str(cpt2_op_mode_t mode)
{
    switch (mode)
    {
        case eCPT2_MODE_THROUGHPUT:
            return "throughput";
        case eCPT2_MODE_LATENCY:
            return "latency";
        case eCPT2_MODE_TRANSACTION:
            return "transaction";
        default:
            return "unknown";
    }
}

/****************************************************************************
* run_ddr()
****************************************************************************/
uint8_t run_ddr(cpt2_options_t * p_options)
{
    uint8_t err = 0;

    err = trace_open(p_options);
    if (0!=err)
    {
        appLogPrintf("trace_open() failed\n");
        return 1;
    }

    err = trace_enable(p_options);
    if (0!=err)
    {
        appLogPrintf("trace_enable() failed\n");
        return 1;
    }

    appLogPrintf("[CTOOLS] CPT2 probe, Trace Aggregator, ATB Replicator, GTC, and TBR opened and enabled successfully.\n");
    return 0;
}

/****************************************************************************
* trace_open()
* Open the drivers for CPTracer2, Trace Aggregator, ATB replicator, TBR, and GTC
****************************************************************************/
uint8_t trace_open(cpt2_options_t * p_options) {

    td_error_t err = e_ERR_NONE;
    uint8_t dbgcell_id;
    ADDR_TYPE dbgcell_baddr;
    ADDR_TYPE atbrep_baddr;
    ADDR_TYPE cpt2_aggr_baddr;
    ADDR_TYPE cpt2_probe_baddr;
    ADDR_TYPE gtc_baddr = GTC_BADDR;

    if (0 != CPT2H_get_device_info(&dbgcell_id, &dbgcell_baddr, &atbrep_baddr, &cpt2_aggr_baddr, &cpt2_probe_baddr, &cpt2_mst_id)) {
        appLogPrintf("Failed to get device info for CPT2 probe %d\n", cpt2_probe_id);
        return 1;
    }

    if (APP_DEBUG_CTOOLS)
    {
        appLogPrintf("Before mmap:\n");
        appLogPrintf("[trace_open] dbgcell_baddr    = 0x%" PRIx64 "\n", (uint64_t)dbgcell_baddr);
        appLogPrintf("[trace_open] cpt2_aggr_baddr  = 0x%" PRIx64 "\n", (uint64_t)cpt2_aggr_baddr);
        appLogPrintf("[trace_open] cpt2_probe_baddr = 0x%" PRIx64 "\n", (uint64_t)cpt2_probe_baddr);
    }

    /*  map for debug IPs, 256KB for CPTracer2, 64KB for Debug Cell */
    dbgcell_phys_baddr = dbgcell_baddr;
    cpt2_aggr_phys_baddr = cpt2_aggr_baddr;
    ADDR_TYPE CPT_PROBE_OFFSET;
    CPT_PROBE_OFFSET = cpt2_probe_baddr - cpt2_aggr_baddr;

    dbgcell_baddr = (ADDR_TYPE)(uintptr_t)appMemMap((void*)(uintptr_t)dbgcell_baddr, DBGCELL_MAPPED_SIZE);
    if((void *)dbgcell_baddr == NULL)
    {
        appLogPrintf("APP_TIMER: ERROR: Unable to mmap dbgcell !!!\n");
    }
        
    cpt2_aggr_baddr = (ADDR_TYPE)(uintptr_t)appMemMap((void*)(uintptr_t)cpt2_aggr_baddr, CPT2_AGGR_MAPPED_SIZE);
    if((void *)cpt2_aggr_baddr == NULL)
    {
        appLogPrintf("APP_TIMER: ERROR: Unable to mmap cpt2_aggr !!!\n");
    }

    cpt2_probe_baddr = cpt2_aggr_baddr + CPT_PROBE_OFFSET;

    gtc_baddr = (ADDR_TYPE)(uintptr_t)appMemMap((void*)(uintptr_t)gtc_baddr, GTC_TIMER_MAPPED_SIZE);
    if((void *)gtc_baddr == NULL)
    {
        appLogPrintf("APP_TIMER: ERROR: Unable to mmap gtc !!!\n");
    }

    atbrep_baddr = dbgcell_baddr + ATBREP_OFFSET;

    if (APP_DEBUG_CTOOLS)
    {
        appLogPrintf("After mmap:\n");
        appLogPrintf("[trace_open] dbgcell_baddr    = 0x%" PRIx64 "\n", (uint64_t)dbgcell_baddr);
        appLogPrintf("[trace_open] cpt2_aggr_baddr  = 0x%" PRIx64 "\n", (uint64_t)cpt2_aggr_baddr);
        appLogPrintf("[trace_open] cpt2_probe_baddr = 0x%" PRIx64 "\n", (uint64_t)cpt2_probe_baddr);
        appLogPrintf("[trace_open] gtc_baddr       = 0x%" PRIx64 "\n", (uint64_t)gtc_baddr);
        appLogPrintf("[trace_open] atbrep_baddr    = 0x%" PRIx64 "\n", (uint64_t)atbrep_baddr);
    }

    /*
    * Update the master ID field
    * For the purpose of trace decoding, the probes are given unique 8 bit IDs across all aggregators.
    * The 3 MSBs of the 8 bit are used to indicate a specific aggregator. The 3 MSB set offset plus the probe port index within the aggregator gives the MASTER ID
    * IMPORTANT: This currently puts a restriction on the system that we can only support 7 aggregator domains at a time while decoding the output binary using CCS.
    */
    p_options->mast_id_3_msbs = cpt2_mst_id >> 5;

    // CPTracer2 Aggregator declarations
    target_access_t traceAggrTa = { 0, cpt2_aggr_baddr };
    trace_aggr_init_t traceAggrInit = { &traceAggrTa };

    // CPTracer2 Probe declarations
    target_access_t cpt2Ta = { 0, cpt2_probe_baddr };
    cpt2_init_t cpt2Init = { &cpt2Ta };

    // ATB Replicator declarations
    target_access_t atbRepTa = { 0, atbrep_baddr };
    atbrep_init_t atbRepInit = { &atbRepTa };

    // Global Timer Counter declarations
    target_access_t gtcTa = { 0, gtc_baddr };
    gtc_init_t gtcInit = { &gtcTa };

    //TBR declarations
    ADDR_TYPE tbr_baddr = dbgcell_baddr + TBR_OFFSET;

    target_access_t tbrTa = { 0, tbr_baddr };
    tbr_init_t tbrInit = { &tbrTa };

    // Open the IP drivers for above debug components
    err = CPT2_open(&cpt2Init, &cpt2Handle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to open CPT2 probe.\n");
        return 1;
    }

    err = TRACE_AGGR_open(&traceAggrInit, &traceAggrHandle);
    if (e_ERR_NONE != err) {
       appLogPrintf("Failed to open trace aggregator.\n");
        return 1;
    }

    err = ATBREP_open(&atbRepInit, &atbRepHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to open ATB replicator.\n");
        return 1;
    }

    err = GTC_open(&gtcInit, &gtcHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to open GTC.\n");
        return 1;
    }

    err = TBR_open(&tbrInit, &tbrHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to open TBR.\n");
        return 1;
    }

    return 0;
}


/****************************************************************************
* trace_enable()
* Configure and Enable CPTracer2, Trace Aggregator, ATB replicator, GTC and TBR
* Please follow the order in this function to enable the debug components!!!
****************************************************************************/
uint8_t trace_enable(cpt2_options_t * p_options)
{

    td_error_t err = e_ERR_NONE;

    // Configure and enable GTC
    err = GTC_configure(gtcHandle, false);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to configure GTC.\n");
        return 1;
    }

    err = GTC_enable(gtcHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to enable GTC.\n");
        return 1;
    }

    // Configure and enable TBR
    tbr_config_t config = cTBR_DEFAULT_CONFIG;
    err = TBR_configure(tbrHandle, &config);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to configure TBR.\n");
        return 1;
    }

    err = TBR_enable(tbrHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to enable TBR.\n");
        return 1;
    }

    // Configure and enable CPTracer2 aggregator
    trace_aggr_options_t traceAggrOptions;
    traceAggrOptions.atb_id = DEFAULT_ATB_ID;
    err = TRACE_AGGR_configure(traceAggrHandle, &traceAggrOptions);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to configure trace aggregator.\n");
        return 1;
    }

    err = TRACE_AGGR_enable(traceAggrHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to enable trace aggregator.\n");
        return 1;
    }

    // Configure and enable CPTracer2 probe
    err = CPT2_configure(cpt2Handle, p_options);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to configure CPTracer2 probe.\n");
        return 1;
    }

    err = CPT2_enable(cpt2Handle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to enable CPTracer2 probe.\n");
        return 1;
    }

    return 0;
}


/****************************************************************************
* CPT2H_get_device_info()
* Based on the CPTracer2 probe ID, get the corresponding debug cell ID,
* debug cell base address, ATB replicator base address, CPTracer2 Aggregator
* base address, CPTracer probe base address, and the master ID
* Here, pcpt2_probe_baddr is the base address of the CPTracer2 probe's register space in memory.
* It is not a single register, but the starting address of a memory-mapped region containing the control and status registers for the selected CPTracer2 probe.
* Software uses this base address to access and configure the probe by reading from or writing to specific offsets within this region.
****************************************************************************/
uint8_t CPT2H_get_device_info(uint8_t *pdbgcell_id, ADDR_TYPE * pdbgcell_baddr, ADDR_TYPE * patbrep_baddr, ADDR_TYPE * pcpt2_aggr_baddr, ADDR_TYPE * pcpt2_probe_baddr, uint8_t *pcpt2_mst_id)
{
    eDebugCell_ID     dbgcell_id = g_cpt2_table[cpt2_probe_id].dbgcell_id;
    uint8_t           port_index = g_cpt2_table[cpt2_probe_id].probe_port_index;
    eCPT2Aggr_ID      aggr_id    = g_cpt2_table[cpt2_probe_id].aggr_id;

    *pdbgcell_id        = dbgcell_id;
    *pdbgcell_baddr     = g_cpt2_table[cpt2_probe_id].dbgcell_baddr;
    *patbrep_baddr      = _ATBREP_BaseAddress(dbgcell_id);
    *pcpt2_aggr_baddr   = g_cpt2_table[cpt2_probe_id].aggr_baddr;
    *pcpt2_probe_baddr  = _CPT2Probe_BaseAddress(aggr_id, port_index);
    *pcpt2_mst_id       = g_cpt2_table[cpt2_probe_id].master_id;

    return 0;
}

/**************************************************************************************************/
/* Disable the CPT Probe ID */
/**************************************************************************************************/
int appCpt2DisableProbe(cpt2_options_t cpt2Options, uint32_t* pBuffer, uint32_t bufferWords)
{
    uint8_t err = 0;

    if (pBuffer == NULL) {
        appLogPrintf("Error: NULL buffer provided for TBR data\n");
        return 1;
    }

    err = trace_disable();
    if (0!=err)
    {
        appLogPrintf("trace_disable() failed\n");
        return 1;
    }

    /* 
    * Note: File I/O operations are supported only on the Cortex-A core. 
    * If the application is running on the Cortex-R5 core, file I/O is not permitted; 
    * therefore, CPT2TBR_PostProcess must not be invoked in this context.
    */
    // Post process TBR data and CPT2 configuration
    if (0 != CPT2TBR_PostProcess(&cpt2Options, tbrHandle, DEFAULT_ATB_ID, pBuffer, bufferWords))
    {
        appLogPrintf("CPT2TBR_PostProcess() call failed!\n");
        return 1;
    }

    // Close trace components
    err = trace_close();
    if (0!=err)
    {
        appLogPrintf("trace_close() failed\n");
        return 1;
    }

    appMemUnMap((void*)(uintptr_t)dbgcell_phys_baddr, DBGCELL_MAPPED_SIZE);
    appMemUnMap((void*)(uintptr_t)cpt2_aggr_phys_baddr, CPT2_AGGR_MAPPED_SIZE);
    appMemUnMap((void*)(uintptr_t)GTC_BADDR, GTC_TIMER_MAPPED_SIZE);

    appLogPrintf("[CTOOLS] CPT2 DDR probe is disabled successfully.\n");
    return 0;
}

/****************************************************************************
* trace_disable()
* Disable CPTracer2, Trace Aggregator,  TBR
* Please follow the order in this function to disable the debug components!!!
****************************************************************************/
uint8_t trace_disable() {

    td_error_t err = e_ERR_NONE;

    // Disable CPTracer2 probe
    err = CPT2_disable(cpt2Handle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to disable CPTracer2 probe.\n");
        return 1;
    }

    // Disable CPTracer2 aggregator
    err = TRACE_AGGR_disable(traceAggrHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to disable trace aggregator.\n");
        return 1;
    }

    /* Flush and Disable TBR to stop capturing trace. TBR must be disabled before reading contents */
    // Initiate a flush
    err = TBR_flush(tbrHandle, (tbr_flush_type_t)e_TBR_FLUSH_ACQUISTION);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to flush TBR.\n");
        return 1;
    }

    // Disable the TBR
    err = TBR_disable(tbrHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to disable TBR.\n");
        return 1;
    }

    return 0;
}

/****************************************************************************
* trace_close()
* close CPTracers/Aggregator/ATB replicator/TBR/GTC.
****************************************************************************/
uint8_t trace_close() {

    td_error_t err = e_ERR_NONE;

    // Close the trace drivers.
    err = CPT2_close(cpt2Handle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to close CPT2 probe.\n");
        return 1;
    }

    err = TRACE_AGGR_close(traceAggrHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to close trace aggregator.\n");
        return 1;
    }

    ATBREP_close(atbRepHandle);

    err = GTC_close(gtcHandle);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to close GTC.\n");
        return 1;
    }

    TBR_close(tbrHandle);

    return 0;
}

/****************************************************************************
* CPT2TBR_PostProcess()
* Read and Transfer TBR data to a Bin file, also record CPTracer2 configuration to a DCM file. We will then use both files for further decode and analysis
****************************************************************************/
uint8_t CPT2TBR_PostProcess(cpt2_options_t * p_options, tbr_handle_t tbrHandle, uint32_t atb_id, uint32_t* pBuffer, uint32_t bufferWords)
{
    /* Check the TBR status */
    uint32_t availableWords = 0;
    tbr_status_t tbrStatus;
    td_error_t err = e_ERR_NONE;

    err = TBR_status(tbrHandle, &tbrStatus, &availableWords);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to get TBR status.\n");
        return 1;
    }
    if (tbrStatus.b_enabled) {
        appLogPrintf("TBR needs to be disabled first before it can be read.\n");
        return 1;
    }

    uint32_t wordsToRead = (bufferWords < availableWords) ? bufferWords : availableWords;

    if (APP_DEBUG_CTOOLS)
    {
        appLogPrintf("Using user buffer of %d words to read TBR\n", bufferWords);
        appLogPrintf("Reading %d words from TBR (available: %d, buffer: %d)\n", wordsToRead, availableWords, bufferWords);
    }

    if (wordsToRead == 0) {
        appLogPrintf("No TBR data to read.\n");
        return 0;
    }

    /* Now read and transfer TBR data to a Bin file */
    if (0 != transferTBRData(tbrHandle, tbrStatus, wordsToRead, pBuffer))
    {
        return 1;
    }
    /* Now transfer the CPTracer2 configuration to a DCM file to help decode */
    if (0 != transferCPT2Config(p_options, atb_id)) {
        return 1;
    }

    return 0;
}


/****************************************************************************
* transferTBRData()
* Reads TBR (Trace Buffer RAM) data from the hardware, stores it in the provided buffer,
* and writes the data to a binary file on the host system for further decoding and analysis.
****************************************************************************/
uint8_t transferTBRData(tbr_handle_t tbrHandle, tbr_status_t tbrStatus, uint32_t wordsToRead, uint32_t* pBuffer)
{
    uint32_t retSize=0;
    td_error_t err = e_ERR_NONE;
    uint32_t tbrSize;
    tbr_property_t properties;

    err = TBR_get_property(tbrHandle, &properties);
    if (e_ERR_NONE != err) {
        appLogPrintf("Failed to get TBR property.\n");
        return 1;
    } else {
        tbrSize = properties.depth;
    }

    if(pBuffer && wordsToRead > 0)
    {
        err = TBR_read(tbrHandle, wordsToRead, pBuffer, &retSize);

        if (APP_DEBUG_CTOOLS)
        {
            appLogPrintf("TBR size = %d\n", tbrSize);
            if(tbrStatus.b_wrapped)
                appLogPrintf("TBR is wrapped; Requested words = %d\n", wordsToRead);
            else
                appLogPrintf("TBR is not wrapped; Requested words = %d\n", wordsToRead);
            appLogPrintf("Read %d words from TBR\n", retSize);
        }

        if (e_ERR_NONE != err) {
            appLogPrintf("Failed to read TBR data.\n");
            return 1;
        }

        if (retSize != wordsToRead) {
            appLogPrintf("Failed to read TBR data: Requested = %d, Actual = %d.\n", wordsToRead, retSize);
            return 1;
        }

        if (APP_DEBUG_CTOOLS)
        {
            appLogPrintf("Successfully read out TBR data into system memory\n");
        }
    }

    /* Transport TBR data */
    if(pBuffer)
    {
        FILE* fp = fopen(pBinFileName, "wb");
        if(fp)
        {
            size_t fret = 0;
            fret = fwrite((void*) pBuffer, sizeof(uint32_t), retSize, fp);

            if (fret < retSize)
            {
                appLogPrintf("Error writing data to  - %s \n", pBinFileName);
                return 1;
            }

            fclose(fp);
            appLogPrintf("Successfully transported TBR data - %s \n", pBinFileName);
        }
        else
        {
            appLogPrintf("Error opening file - %s \n", pBinFileName);
            return 1;
        }
    }

    return 0;
}

/****************************************************************************
* transferCPT2Config()
* Status of the CPTracer2 DCM data - this is required to decode the compressed data
****************************************************************************/
uint8_t transferCPT2Config(cpt2_options_t * p_options, uint32_t atb_id)
{
    char buf[1024];

    FILE* fp = fopen(pDcmFileName, "w");
    if(fp)
    {
        td_error_t err = e_ERR_NONE;

        err = CPT2_get_options_string(cpt2Handle, p_options, buf);
        if (err != e_ERR_NONE)
        {
            appLogPrintf("Error getting CPT2 option string");
            return 1;
        }

        fprintf(fp, "CPT2_%d=%s\n", cpt2_mst_id, buf);
        fprintf(fp, "STM_ATB_Id=%d\n", atb_id);
        fprintf(fp, "STM_Buffer_Wrapped=0\n");
        fprintf(fp, "STM_STP_Version=2\n");
        fprintf(fp, "TWP_Protocol=1\n");
        fclose(fp);
        appLogPrintf("Successfully transported CPT2 config - %s \n", pDcmFileName);
    }
    else
    {
        appLogPrintf("Error opening file - %s \n", pDcmFileName);
        return 1;
    }

    // Create symlink for latest .bin and .dcm file
    char symlinkBin[64];
    char symlinkDcm[64];
    snprintf(symlinkBin, sizeof(symlinkBin), "/tmp/CPT2_ddr_%s.bin", get_mode_str(p_options->mode));
    unlink(symlinkBin);
    if (symlink(pBinFileName, symlinkBin) != 0) {
        appLogPrintf("Failed to create symlink: %s -> %s\n", symlinkBin, pBinFileName);
    }

    snprintf(symlinkDcm, sizeof(symlinkDcm), "/tmp/CPT2_ddr_%s.dcm", get_mode_str(p_options->mode));
    unlink(symlinkDcm);
    if (symlink(pDcmFileName, symlinkDcm) != 0) {
        appLogPrintf("Failed to create symlink: %s -> %s\n", symlinkDcm, pDcmFileName);
    }
    return 0;
}
