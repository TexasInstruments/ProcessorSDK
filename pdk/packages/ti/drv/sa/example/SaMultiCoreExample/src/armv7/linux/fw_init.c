/**
 *  (C) Copyright 2012, Texas Instruments, Inc.
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
#include "fw_test.h"
#include "fw_mem_allocator.h"
#include "../../testconn.h"

#include <ti/drv/qmss/qmss_qm.h>
#if defined(DEVICE_K2K) || defined(SOC_K2K)
#include <ti/drv/qmss/device/k2h/src/qmss_device.c>
#include <ti/drv/cppi/device/k2h/src/cppi_device.c>
#elif defined(DEVICE_K2H) || defined(SOC_K2H)
#include <ti/drv/qmss/device/k2k/src/qmss_device.c>
#include <ti/drv/cppi/device/k2k/src/cppi_device.c>
#elif defined(DEVICE_K2E) || defined(SOC_K2E)
#include <ti/drv/qmss/device/k2e/src/qmss_device.c>
#include <ti/drv/cppi/device/k2e/src/cppi_device.c>
#elif defined(DEVICE_K2L) || defined(SOC_K2L)
#include <ti/drv/qmss/device/k2l/src/qmss_device.c>
#include <ti/drv/cppi/device/k2l/src/cppi_device.c>
#else /*Default k2h device is assumed */
#include <ti/drv/qmss/device/k2h/src/qmss_device.c>
#include <ti/drv/cppi/device/k2h/src/cppi_device.c>
#endif
/************************ GLOBAL VARIABLES ********************/

/* BOOT MODE CHECK */
extern uint32_t no_bootMode;

/* test framework instance */
tFramework_t tFramework;

/* shared memory block base address */
extern sa_Example_MemBlk_t* shared_ObjsBase;

/* Host Descriptor Region - [Size of descriptor * Number of descriptors]
 * MUST be 16 byte aligned.
 */
uint8_t *pHostDesc = 0;

/******************************************************************************
* Macro to convert to IP Register Virtual Address from a mapped base Virtual Address
* Input: virtBaseAddr: Virtual base address mapped using mmap for IP
*        phyBaseAddr: Physical base address for the IP
*        phyRegAddr:  Physical register address
******************************************************************************/
static inline void* FW_GET_REG_VADDR (void * virtBaseAddr, uint32_t phyBaseAddr, uint32_t phyRegAddr)
{
    return((void *)((uint8_t *)virtBaseAddr + (phyRegAddr - phyBaseAddr)));
}

/** ============================================================================
 *   @n@b initQmss
 *
 *   @b Description
 *   @n This API initializes the QMSS LLD on master processor only.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t initQmss (void)
{
    int32_t                     result;
    Qmss_MemRegInfo             memCfg;
    Qmss_InitCfg                qmssInitConfig;
    Cppi_DescCfg                cppiDescCfg;
    uint32_t                    numAllocated;
    Qmss_GlobalConfigParams     fw_qmssGblCfgParams;
    uint32_t                    count;

    /* Initialize QMSS */
    memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));

    /* Set up QMSS configuration */

    /* Use internal linking RAM */
    qmssInitConfig.linkingRAM0Base  = 0;
    qmssInitConfig.linkingRAM0Size  = 0;
    qmssInitConfig.linkingRAM1Base  = 0;
    qmssInitConfig.maxDescNum       =   TF_NUM_DESC;

    /* Bypass hardware initialization as it is done within Kernel */
    qmssInitConfig.qmssHwStatus     =   QMSS_HW_INIT_COMPLETE;

    fw_qmssGblCfgParams = qmssGblCfgParams;

    /* Convert address to Virtual address */
    for(count=0;count < fw_qmssGblCfgParams.maxQueMgrGroups;count++)
    {

        fw_qmssGblCfgParams.groupRegs[count].qmConfigReg =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.groupRegs[count].qmConfigReg);

        fw_qmssGblCfgParams.groupRegs[count].qmDescReg =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.groupRegs[count].qmDescReg);

        fw_qmssGblCfgParams.groupRegs[count].qmQueMgmtReg =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.groupRegs[count].qmQueMgmtReg);

        fw_qmssGblCfgParams.groupRegs[count].qmQueMgmtProxyReg =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.groupRegs[count].qmQueMgmtProxyReg);

        fw_qmssGblCfgParams.groupRegs[count].qmQueStatReg =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.groupRegs[count].qmQueStatReg);

        fw_qmssGblCfgParams.groupRegs[count].qmStatusRAM =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.groupRegs[count].qmStatusRAM);

        fw_qmssGblCfgParams.groupRegs[count].qmQueMgmtDataReg =
            FW_GET_REG_VADDR(fw_qmssDataVaddr,QMSS_DATA_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.groupRegs[count].qmQueMgmtDataReg);

        fw_qmssGblCfgParams.groupRegs[count].qmQueMgmtProxyDataReg = NULL; /* not supported on k2 hardware, and not used by lld */
    }

    for(count=0;count < QMSS_MAX_INTD;count++)
    {
        fw_qmssGblCfgParams.regs.qmQueIntdReg[count] =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.regs.qmQueIntdReg[count]);
    }

    for(count=0;count < QMSS_MAX_PDSP;count++)
    {
        fw_qmssGblCfgParams.regs.qmPdspCmdReg[count] =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.regs.qmPdspCmdReg[count]);

        fw_qmssGblCfgParams.regs.qmPdspCtrlReg[count] =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.regs.qmPdspCtrlReg[count]);

        fw_qmssGblCfgParams.regs.qmPdspIRamReg[count] =
            FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.regs.qmPdspIRamReg[count]);
    }

    fw_qmssGblCfgParams.regs.qmLinkingRAMReg =
        FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.regs.qmLinkingRAMReg);

    fw_qmssGblCfgParams.regs.qmBaseAddr =
        FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,(uint32_t)fw_qmssGblCfgParams.regs.qmBaseAddr);

#ifndef SIMULATOR_SUPPORT
	fw_qmssGblCfgParams.qmRmServiceHandle = rmClientServiceHandle;
#endif

    /* Initialize the Queue Manager */
    result = Qmss_init (&qmssInitConfig, &fw_qmssGblCfgParams);
    if (result != QMSS_SOK)
    {
        printf ("initQmss: Error initializing Queue Manager SubSystem, Error code : %d\n", result);
        return -1;
    }

    /* Start Queue manager on this core */
    result = Qmss_start ();

    /* Setup the descriptor memory regions.
     *
     * The Descriptor base addresses MUST be global addresses and
     * all memory regions MUST be setup in ascending order of the
     * descriptor base addresses.
     */
    if (coreNum == SYSINIT)
   	{
        pHostDesc = (uint8_t*)fw_memAlloc((TF_NUM_DESC * TF_SIZE_DESC), CACHE_LINESZ);
        if(pHostDesc == NULL) {
      	    printf ("initQmss: memAlloc failed for pHostDesc\n");
      	    return (-1);
        }

        /* Initialize and setup CPSW Host Descriptors required for example */
        memset (pHostDesc, 0, TF_SIZE_DESC * TF_NUM_DESC);
        memCfg.descBase             =   (uint32_t *) pHostDesc;
        memCfg.descSize             =   TF_SIZE_DESC;
        memCfg.descNum              =   TF_NUM_DESC;
        memCfg.manageDescFlag       =   Qmss_ManageDesc_MANAGE_DESCRIPTOR;
        memCfg.memRegion            =   Qmss_MemRegion_MEMORY_REGION0;
        memCfg.startIndex           =   0;

        /* Insert Host Descriptor memory region */
        result = Qmss_insertMemoryRegion(&memCfg);
        if (result == QMSS_MEMREGION_ALREADY_INITIALIZED)
        {
            printf ("initQmss: Memory Region %d already Initialized \n", memCfg.memRegion);
        }
        else if (result < QMSS_SOK)
        {
            printf ("initQmss: Error Inserting memory region %d, Error code : %d\n", memCfg.memRegion, result);
            return -1;
        }

        /* Initialize all the descriptors we just allocated on the
         * memory region above. Setup the descriptors with some well
         * known values before we use them for data transfers.
         */
        memset (&cppiDescCfg, 0, sizeof (cppiDescCfg));
        cppiDescCfg.queueGroup      =   0;
        cppiDescCfg.memRegion       =   Qmss_MemRegion_MEMORY_REGION0;
        cppiDescCfg.descNum         =   TF_NUM_DESC;
        cppiDescCfg.destQueueNum    =   TF_Q_FREE_DESC;
        cppiDescCfg.queueType       =   Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
        cppiDescCfg.initDesc        =   Cppi_InitDesc_INIT_DESCRIPTOR;
        cppiDescCfg.descType        =   Cppi_DescType_HOST;

        /* By default:
         *      (1) Return descriptors to tail of queue
         *      (2) Always return entire packet to this free queue
         *      (3) Set that PS Data is always present in start of SOP buffer
         *      (4) Configure free q num < 4K, hence qMgr = 0
         *      (5) Recycle back to the same Free queue by default.
         */
        cppiDescCfg.returnPushPolicy            =   Qmss_Location_TAIL;
        cppiDescCfg.cfg.host.returnPolicy       =   Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;
        cppiDescCfg.cfg.host.psLocation         =   Cppi_PSLoc_PS_IN_DESC;
        cppiDescCfg.returnQueue.qMgr            =   0;
        cppiDescCfg.returnQueue.qNum            =   QMSS_PARAM_NOT_SPECIFIED;
        cppiDescCfg.epibPresent                 =   Cppi_EPIB_EPIB_PRESENT;

        /* Initialize the descriptors, create a free queue and push descriptors to a global free queue */
        if ((tFramework.QfreeDesc = Cppi_initDescriptor (&cppiDescCfg, &numAllocated)) <= 0)
        {
            printf ("initQmss: Error Initializing Free Descriptors, Error: %d \n", tFramework.QfreeDesc);
            return -1;
        }
        else
        {
            printf ("initQmss: Initialized Free Descriptors. \n");
        }
    }

    /* Queue Manager Initialization Done */
    return 0;
}

/** ============================================================================
 *   @n@b initCppi
 *
 *   @b Description
 *   @n This API initializes the CPPI LLD, opens the PASS CPDMA and opens up
 *      the Tx, Rx channels required for data transfers.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t initCppi (void)
{
    int32_t                     result, i;
    Cppi_CpDmaInitCfg           cpdmaCfg;
    uint8_t                     isAllocated;
    Cppi_TxChInitCfg            txChCfg;
    Cppi_RxChInitCfg            rxChCfg;
    Cppi_GlobalConfigParams     fw_cppiGblCfgParams;
    sa_MCExample_entry_t val;

#ifndef SIMULATOR_SUPPORT
    Cppi_StartCfg               cppiStartCfg;
#endif

    uint32_t cppi_pa_tx_ch_disable_list[TF_PA_NUM_TX_CPDMA_CHANNELS];
    uint32_t cppi_pa_rx_ch_disable_list[TF_PA_NUM_RX_CPDMA_CHANNELS];
    uint32_t disable_list_flag = 0;

    /* Clear the list by default */
    memset (cppi_pa_tx_ch_disable_list, 0, sizeof (cppi_pa_tx_ch_disable_list));
    memset (cppi_pa_rx_ch_disable_list, 0, sizeof (cppi_pa_rx_ch_disable_list));

    fw_cppiGblCfgParams = cppiGblCfgParams;
    /* Convert Physical address to Virtual address for LLD access */

#if defined(DEVICE_K2K) || defined(DEVICE_K2H) || defined(SOC_K2K) || defined(SOC_K2H)
    /* SRIO CPDMA regs */
    if (fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_SRIO_CPDMA].maxRxCh ||
       fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_SRIO_CPDMA].maxTxCh)
    {
        fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_SRIO_CPDMA].gblCfgRegs =
            FW_GET_REG_VADDR(fw_srioCfgVaddr,CSL_SRIO_CFG_REGS,CSL_SRIO_CFG_PKTDMA_GLOBAL_CFG_REGS);

        fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_SRIO_CPDMA].txChRegs =
            FW_GET_REG_VADDR(fw_srioCfgVaddr,CSL_SRIO_CFG_REGS,CSL_SRIO_CFG_PKTDMA_TX_CFG_REGS);

        fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_SRIO_CPDMA].rxChRegs =
            FW_GET_REG_VADDR(fw_srioCfgVaddr,CSL_SRIO_CFG_REGS,CSL_SRIO_CFG_PKTDMA_RX_CFG_REGS);

        fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_SRIO_CPDMA].txSchedRegs =
            FW_GET_REG_VADDR(fw_srioCfgVaddr,CSL_SRIO_CFG_REGS,CSL_SRIO_CFG_PKTDMA_TX_SCHEDULER_CFG_REGS);

        fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_SRIO_CPDMA].rxFlowRegs =
            FW_GET_REG_VADDR(fw_srioCfgVaddr,CSL_SRIO_CFG_REGS,CSL_SRIO_CFG_PKTDMA_RX_FLOW_CFG_REGS);
    }
#endif

    /* PASS CPDMA regs */
    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].gblCfgRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_GLOBAL_CFG_REGS);

#if defined(DEVICE_K2L) || defined(DEVICE_K2E) || defined(SOC_K2L) || defined(SOC_K2E)

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].txChRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_GLOBAL_TX_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].rxChRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_GLOBAL_RX_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].txSchedRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_GLOBAL_TX_SCHEDULER_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].rxFlowRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_GLOBAL_RX_FLOW_CFG_REGS);

#else


    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].txChRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_TX_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].rxChRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_RX_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].txSchedRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_TX_SCHEDULER_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_PASS_CPDMA].rxFlowRegs =
        FW_GET_REG_VADDR(fw_passCfgVaddr,CSL_NETCP_CFG_REGS,CSL_NETCP_CFG_PKTDMA_RX_FLOW_CFG_REGS);

#endif

    /* QMSS CPDMA regs */
    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_QMSS_CPDMA].gblCfgRegs =
        FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,CSL_QMSS_CFG_PKTDMA_1_GLOBAL_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_QMSS_CPDMA].txChRegs =
        FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,CSL_QMSS_CFG_PKTDMA_1_TX_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_QMSS_CPDMA].rxChRegs =
        FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,CSL_QMSS_CFG_PKTDMA_1_RX_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_QMSS_CPDMA].txSchedRegs =
        FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,CSL_QMSS_CFG_PKTDMA_1_TX_SCHEDULER_CFG_REGS);

    fw_cppiGblCfgParams.cpDmaCfgs[Cppi_CpDma_QMSS_CPDMA].rxFlowRegs =
        FW_GET_REG_VADDR(fw_qmssCfgVaddr,QMSS_CFG_BASE_ADDR,CSL_QMSS_CFG_PKTDMA_1_RX_FLOW_CFG_REGS);

    /* Initialize CPPI LLD */
    result = Cppi_init (&fw_cppiGblCfgParams);
    if (result != CPPI_SOK)
    {
        printf ("initCppi: Error initializing CPPI LLD, Error code : %d\n", result);
        return -1;
    }

#ifndef SIMULATOR_SUPPORT
	if (rmClientServiceHandle) {
       memset(&cppiStartCfg, 0, sizeof(cppiStartCfg));
	   /* Register RM with CPPI */
	   cppiStartCfg.rmServiceHandle = rmClientServiceHandle;
	   Cppi_startCfg(&cppiStartCfg);
	}
#endif /* SIMULATOR_SUPPORT */

    /* Initialize PASS CPDMA */
    memset (&cpdmaCfg, 0, sizeof (Cppi_CpDmaInitCfg));
    cpdmaCfg.dmaNum     = Cppi_CpDma_PASS_CPDMA;

    if ((tFramework.tfPaCppiHandle = Cppi_open (&cpdmaCfg)) == NULL)
    {
        printf ("initCppi: Error initializing CPPI for PASS CPDMA %d \n", cpdmaCfg.dmaNum);
        return -1;
    }

    /* Initialize CPPI LLD */
	if (coreNum != SYSINIT)
	{
	    return (0);
	}

    /* Open all CPPI Tx Channels. These will be used to send data to PASS/CPSW */
    for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i ++)
    {
        txChCfg.channelNum      =   i;       /* CPPI channels are mapped one-one to the PA Tx queues */
        txChCfg.txEnable        =   Cppi_ChState_CHANNEL_DISABLE;  /* Disable the channel for now. */
        txChCfg.filterEPIB      =   0;
        txChCfg.filterPS        =   0;
        txChCfg.aifMonoMode     =   0;
        txChCfg.priority        =   2;
        if ((tFramework.tfPaTxChHnd[i] = Cppi_txChannelOpen (tFramework.tfPaCppiHandle, &txChCfg, &isAllocated)) == NULL)
        {
            if (no_bootMode == TRUE)
            {
        	   System_printf ("Error opening Tx channel %d\n", txChCfg.channelNum);
               return -1;
            }
            else
            {
            	cppi_pa_tx_ch_disable_list[i] = 1;
            	disable_list_flag = 1;
            	continue;
            }
        }
        Cppi_channelEnable (tFramework.tfPaTxChHnd[i]);
    }

    /* Print if there are any CPPI Tx channels that are not enabled by above code, presuming linux enabled it */
    if (disable_list_flag)
    {
    	System_printf ("Unable to open below cppi tx channels...presuming linux has already enabled it \n");
		for (i = 0; i<TF_PA_NUM_TX_CPDMA_CHANNELS; i++)
		{
			 if (cppi_pa_tx_ch_disable_list[i])
				 System_printf ("%d ", i);
		}
		System_printf (" \n ");
    }

    /* Open all CPPI Rx channels. These will be used by PA to stream data out. */
    for (i = 0; i < TF_PA_NUM_RX_CPDMA_CHANNELS; i++)
    {
        /* Open a CPPI Rx channel that will be used by PA to stream data out. */
        rxChCfg.channelNum  =   i;
        rxChCfg.rxEnable    =   Cppi_ChState_CHANNEL_DISABLE;
        if ((tFramework.tfPaRxChHnd[i] = Cppi_rxChannelOpen (tFramework.tfPaCppiHandle, &rxChCfg, &isAllocated)) == NULL)
        {
            if (no_bootMode == TRUE)
            {
            	System_printf ("Error opening Rx channel: %d \n", rxChCfg.channelNum);
                return -1;
            }
            else
            {
            	cppi_pa_rx_ch_disable_list[i] = 1;
            	disable_list_flag = 1;
            	continue;
            }
        }

        /* Also enable Rx Channel */
        Cppi_channelEnable (tFramework.tfPaRxChHnd[i]);
    }

    /* Print if there are any CPPI Rx channels that are not enabled by above code, presuming linux enabled it */
    if (disable_list_flag)
    {
    	System_printf ("Unable to open below cppi Rx channels...presuming linux has already enabled it \n");
		for (i = 0; i<TF_PA_NUM_RX_CPDMA_CHANNELS; i++)
		{
			 if (cppi_pa_rx_ch_disable_list[i])
				 System_printf ("%d ", i);
		}
		System_printf (" \n ");
    }

    /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
    Cppi_setCpdmaLoopback(tFramework.tfPaCppiHandle, 0);

    /* CPPI Init Done. Return success */
    return 0;
}

/* Setup all the queues used in the example */
int32_t setupQueues (void)
{
    uint8_t         isAlloc;
    Qmss_Queue      q;
    Cppi_HostDesc   *hd;
    uint8_t         *bPtr;
    int             i, j;

    /* The 10 PA transmit queues (corresponding to the 10 tx cdma channels */
	if (coreNum == SYSINIT)
	{
        for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i ++)
        {
            if ((tFramework.QPaTx[i] = Qmss_queueOpen (Qmss_QueueType_PASS_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAlloc)) < 0)
            {
            printf ("setupQueues: Qmss_queueOpen failed for PA transmit queue number %d\n", TF_PA_TX_QUEUE_BASE+i);
                return -1;
            }
            Qmss_setQueueThreshold (tFramework.QPaTx[i], 1, 1);
        }
	}
	else
	{
        for (i = 0, j = TF_PA_TX_QUEUE_BASE; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i ++, j ++)
        {
            if ((tFramework.QPaTx[i] = Qmss_queueOpen (Qmss_QueueType_PASS_QUEUE, j, &isAlloc)) < 0)
            {
                printf ("setupQueues: Qmss_queueOpen failed for PA transmit queue number %d\n", TF_PA_TX_QUEUE_BASE+i);
                return -1;
            }
        }

        /* The queues with attached buffers */
        tFramework.QfreeDesc = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_Q_FREE_DESC, &isAlloc);

        if (tFramework.QfreeDesc < 0)  {
        	System_printf ("setupQueues: Qmss_queueOpen failed for queue %d\n", TF_Q_FREE_DESC);
        	return (-1);
        }

        /* The queues with attached buffers */
        tFramework.QtDown = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_TEARDOWN_QUE_MGR, &isAlloc);

        if (tFramework.QtDown < 0)  {
        	System_printf ("setupQueues: Qmss_queueOpen failed for queue %d\n", TF_TEARDOWN_QUE_MGR);
        	return (-1);
        }
	}

    /* The queues with attached buffers */
    tFramework.QLinkedBuf1 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q1, &isAlloc);
    if (tFramework.QLinkedBuf1 < 0)  {
  	    printf ("setupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q1);
  	    return (-1);
    }

    tFramework.QLinkedBuf2 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q2, &isAlloc);
    if (tFramework.QLinkedBuf2 < 0)  {
  	    printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q2);
  	    return (-1);
    }

    tFramework.QLinkedBuf3 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q3, &isAlloc);
    if (tFramework.QLinkedBuf3 < 0)  {
  	    printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q3);
  	    return (-1);
    }

    tFramework.QLinkedBuf4 = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_LINKED_BUF_Q4, &isAlloc);
    if (tFramework.QLinkedBuf4 < 0)  {
  	    printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_LINKED_BUF_Q4);
  	    return (-1);
    }

    /* Attach buffers to the queues and push them onto the queue */
    if (coreNum == SYSINIT)
    {
        q.qMgr = 0;

        /* Allocate memory for the Packet buffers */
        q.qNum = TF_LINKED_BUF_Q1;
        bPtr = (uint8_t *) fw_memAlloc((TF_LINKED_BUF_Q1_NBUFS * TF_LINKED_BUF_Q1_BUF_SIZE), CACHE_LINESZ);
        if(bPtr == NULL) {
      	    printf ("SetupQueues: memAlloc failed for memQ1\n");
      	    return (-1);
        }

        for (i = 0; i < TF_LINKED_BUF_Q1_NBUFS; i++)  {

            hd = (Cppi_HostDesc *)((uint32_t)Qmss_queuePop (tFramework.QfreeDesc));

            /* The descriptor address returned from the hardware has the
             * descriptor size appended to the address in the last 4 bits.
             *
             * To get the true descriptor size, always mask off the last
             * 4 bits of the address.
             */
            hd = (Cppi_HostDesc *)((uint32_t)hd & 0xFFFFFFF0);

            if (hd == NULL)  {
                printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
                return (-1);
            }

            /* Populate the free descriptor with the buffer. */
            Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                          TF_LINKED_BUF_Q1_BUF_SIZE);
            /* Save original buffer information */
            Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                                     TF_LINKED_BUF_Q1_BUF_SIZE);

            hd->nextBDPtr = (uint32_t)NULL;
            Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
            Qmss_queuePushDesc (tFramework.QLinkedBuf1, (Ptr)hd);
            bPtr += TF_LINKED_BUF_Q1_BUF_SIZE;
        }

        bPtr = (uint8_t *) fw_memAlloc((TF_LINKED_BUF_Q2_NBUFS * TF_LINKED_BUF_Q2_BUF_SIZE), CACHE_LINESZ);
        if(bPtr == NULL) {
      	    printf ("SetupQueues: memAlloc failed for memQ2\n");
      	    return (-1);
        }

        q.qNum = tFramework.QLinkedBuf2;

        for (i = 0; i < TF_LINKED_BUF_Q2_NBUFS; i++)   {

            hd = (Cppi_HostDesc *)((uint32_t)Qmss_queuePop (tFramework.QfreeDesc));

            /* The descriptor address returned from the hardware has the
             * descriptor size appended to the address in the last 4 bits.
             *
             * To get the true descriptor size, always mask off the last
             * 4 bits of the address.
             */
            hd = (Cppi_HostDesc *)((uint32_t)hd & 0xFFFFFFF0);

            if (hd == NULL)  {
                printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
                return (-1);
            }

            /* Populate the free descriptor with the buffer. */
            Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                          TF_LINKED_BUF_Q2_BUF_SIZE);
            /* Save original buffer information */
            Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                                     TF_LINKED_BUF_Q2_BUF_SIZE);

            hd->nextBDPtr = (uint32_t)NULL;
            Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
            Qmss_queuePushDesc (tFramework.QLinkedBuf2, (Ptr)hd);
            bPtr += TF_LINKED_BUF_Q2_BUF_SIZE;
        }

        bPtr = (uint8_t *) fw_memAlloc((TF_LINKED_BUF_Q3_NBUFS * TF_LINKED_BUF_Q3_BUF_SIZE), CACHE_LINESZ);
        if(bPtr == NULL) {
      	    printf ("SetupQueues: memAlloc failed for memQ3\n");
      	    return (-1);
        }

        q.qNum = tFramework.QLinkedBuf3;

        for (i = 0; i < TF_LINKED_BUF_Q3_NBUFS; i++)   {

            hd = (Cppi_HostDesc *)((uint32_t)Qmss_queuePop (tFramework.QfreeDesc));

            /* The descriptor address returned from the hardware has the
             * descriptor size appended to the address in the last 4 bits.
             *
             * To get the true descriptor size, always mask off the last
             * 4 bits of the address.
             */
            hd = (Cppi_HostDesc *)((uint32_t)hd & 0xFFFFFFF0);

            if (hd == NULL)  {
                printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
                return (-1);
            }

            /* Populate the free descriptor with the buffer. */
            Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                          TF_LINKED_BUF_Q3_BUF_SIZE);
            /* Save original buffer information */
            Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                                     TF_LINKED_BUF_Q3_BUF_SIZE);

            hd->nextBDPtr = (uint32_t)NULL;
            Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
            Qmss_queuePushDesc (tFramework.QLinkedBuf3, (Ptr)hd);
            bPtr += TF_LINKED_BUF_Q3_BUF_SIZE;
        }

        bPtr = (uint8_t *) fw_memAlloc((TF_LINKED_BUF_Q4_NBUFS * TF_LINKED_BUF_Q4_BUF_SIZE), CACHE_LINESZ);
        if(bPtr == NULL) {
      	    printf ("SetupQueues: memAlloc failed for memQ4\n");
      	    return (-1);
        }

        q.qNum = tFramework.QLinkedBuf4;

        for (i = 0; i < TF_LINKED_BUF_Q4_NBUFS; i++)   {

            hd = (Cppi_HostDesc *)((uint32_t)Qmss_queuePop (tFramework.QfreeDesc));

            /* The descriptor address returned from the hardware has the
             * descriptor size appended to the address in the last 4 bits.
             *
             * To get the true descriptor size, always mask off the last
             * 4 bits of the address.
             */
            hd = (Cppi_HostDesc *)((uint32_t)hd & 0xFFFFFFF0);

            if (hd == NULL)  {
                printf ("setupQueues: Qmss_queuePop returned NULL on pop from queue number %d\n", tFramework.QfreeDesc);
                return (-1);
            }

            /* Populate the free descriptor with the buffer. */
            Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                          TF_LINKED_BUF_Q4_BUF_SIZE);
            /* Save original buffer information */
            Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, bPtr,
                                     TF_LINKED_BUF_Q4_BUF_SIZE);

            hd->nextBDPtr = (uint32_t)NULL;
            Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
            Qmss_queuePushDesc (tFramework.QLinkedBuf4, (Ptr)hd);
            bPtr += TF_LINKED_BUF_Q4_BUF_SIZE;
        }
    }

    /* General purpose queues */
    for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {

  	    tFramework.QGen[i] = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, TF_FIRST_GEN_QUEUE + i, &isAlloc);

 	    if (tFramework.QGen[i] < 0)  {
  	        printf ("SetupQueues: Qmss_queueOpen failed for queue %d\n", TF_FIRST_GEN_QUEUE + i);
  	        return (-1);
  	    }
    }

    /* All done return success. */
    return 0;
}

/* Configure flows */
int setupFlows (void)
{
  Cppi_RxFlowCfg  rxFlowCfg;
  uint8_t         isAlloc;
  int i;
  sa_MCExample_entry_t val;

  /* Configure Rx flow */
  rxFlowCfg.flowIdNum      = CPPI_PARAM_NOT_SPECIFIED;
  rxFlowCfg.rx_dest_qnum   = TF_FIRST_GEN_QUEUE + TF_NUM_GEN_QUEUES -1;   /* Override in PA */
  rxFlowCfg.rx_dest_qmgr   = 0;
  rxFlowCfg.rx_sop_offset  = 0;
  rxFlowCfg.rx_ps_location = Cppi_PSLoc_PS_IN_DESC;
  rxFlowCfg.rx_desc_type   = Cppi_DescType_HOST;
  rxFlowCfg.rx_error_handling = 1;
  rxFlowCfg.rx_psinfo_present = 1;
  rxFlowCfg.rx_einfo_present  = 1;

  rxFlowCfg.rx_dest_tag_lo = 0;
  rxFlowCfg.rx_dest_tag_hi = 0;
  rxFlowCfg.rx_src_tag_lo  = 0;
  rxFlowCfg.rx_src_tag_hi  = 0;

  rxFlowCfg.rx_size_thresh0_en = 1;
  rxFlowCfg.rx_size_thresh1_en = 1;
  rxFlowCfg.rx_size_thresh2_en = 1;

  rxFlowCfg.rx_dest_tag_lo_sel = 0;
  rxFlowCfg.rx_dest_tag_hi_sel = 0;
  rxFlowCfg.rx_src_tag_lo_sel  = 0;
  rxFlowCfg.rx_src_tag_hi_sel  = 0;

  rxFlowCfg.rx_fdq1_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq1_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz0_qnum = tFramework.QLinkedBuf1;
  rxFlowCfg.rx_fdq0_sz0_qmgr = 0;

  rxFlowCfg.rx_fdq3_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq3_qmgr = 0;
  rxFlowCfg.rx_fdq2_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq2_qmgr = 0;

  rxFlowCfg.rx_size_thresh1 = TF_LINKED_BUF_Q2_BUF_SIZE;
  rxFlowCfg.rx_size_thresh0 = TF_LINKED_BUF_Q1_BUF_SIZE;

  rxFlowCfg.rx_fdq0_sz1_qnum = tFramework.QLinkedBuf2;
  rxFlowCfg.rx_fdq0_sz1_qmgr = 0;
  rxFlowCfg.rx_size_thresh2  = TF_LINKED_BUF_Q3_BUF_SIZE;

  rxFlowCfg.rx_fdq0_sz3_qnum = tFramework.QLinkedBuf4;
  rxFlowCfg.rx_fdq0_sz3_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz2_qnum = tFramework.QLinkedBuf3;
  rxFlowCfg.rx_fdq0_sz2_qmgr = 0;

  tFramework.tfPaFlowHnd = Cppi_configureRxFlow (tFramework.tfPaCppiHandle, &rxFlowCfg, &isAlloc);
  if (tFramework.tfPaFlowHnd == NULL)  {
    printf ("setupFlows: cppi_ConfigureRxFlow returned NULL\n");
    return (-1);
  }
  tFramework.tfFlowNum = (uint8_t) Cppi_getFlowId(tFramework.tfPaFlowHnd);

  /* Publish the common flow for other cores */
  val.flowId = tFramework.tfFlowNum;
  fw_shmSetEntry(setGFlowId, val);

  return (0);
}

/* The QM/CPDMA are setup */
int initQm (void)
{
  if (initQmss())  {
  	printf ("initQm: initQmss failed\n");
    return (-1);
  }

  if (initCppi ())  {
  	printf ("initQm: initCppi failed\n");
    return (-1);
  }

  if (setupQueues ())  {
  	printf ("initQm: setupQueues failed\n");
    return (-1);
  }

  if (setupFlows ())  {
  	printf ("initQm: setupFlows failed\n");
    return (-1);
  }

  return (0);
}

/* The PA LLD instance is created, the PA firmware is
 * downloaded and started by the Linux Kernel*/
int initPa (void)
{
  paSizeInfo_t  paSize;
  paConfig_t    paCfg;
  paTimestampConfig_t tsCfg;
  int           ret;
  int           sizes[pa_N_BUFS];
  int           aligns[pa_N_BUFS];
  void*         bases[pa_N_BUFS];
  uint8_t*      memPaInst;
  uint8_t*      memL2Ram;
  uint8_t*      memL3Ram;
  sa_MCExample_entry_t val;

  /* The maximum number of handles that can exists are 32 for L2, and 64 for L3. */
  memset(&paSize, 0, sizeof(paSize));
  memset(sizes, 0, sizeof(sizes));
  memset(aligns, 0, sizeof(aligns));
  memset(bases, 0, sizeof(bases));
  memset(&paCfg, 0, sizeof(paCfg));
  paSize.nMaxL2 = TF_MAX_NUM_L2_HANDLES;
  paSize.nMaxL3 = TF_MAX_NUM_L3_HANDLES;
  paSize.nUsrStats = 0;

  ret = Pa_getBufferReq(&paSize, sizes, aligns);

  if (ret != pa_OK)  {
    printf ("initPa: Pa_getBufferReq() return with error code %d\n", ret);
    return (-1);
  }

  /* The first buffer is used as the instance buffer */
  val       = fw_shmGetEntry(getPaInstBufAddrCmd);
  memPaInst = (uint8_t *) val.paInstBufPtr;

  if ((uint32_t)memPaInst & (aligns[0] - 1))  {
    printf ("initPa: Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[0], (uint32_t)memPaInst);
    return (-1);
  }

  val = fw_shmGetEntry(getPaInstBufSizeCmd);
  if ((int) val.paInstSize < sizes[0])  {
    printf ("initPa: Pa_getBufferReq requires size %d for instance buffer, have only %d\n", sizes[0], sizeof(memPaInst));
    return (-1);
  }

  bases[0] = (void *)memPaInst;


  /* The second buffer is the L2 table */
  val      = fw_shmGetEntry(getMemL2RamBufAddrCmd);
  memL2Ram = (uint8_t *) val.paL2BufPtr;

  if ((uint32_t)memL2Ram & (aligns[1] - 1))  {
    printf ("initPa: Pa_getBufferReq requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[1], (uint32_t)memL2Ram);
    return (-1);
  }

  val = fw_shmGetEntry(getMemL2RamBufSizeCmd);
  if ((int) val.paL2MemSize <  sizes[1])  {
    printf ("initPa: Pa_getBufferReq requires %d bytes for buffer 1, have only %d\n", sizes[1], sizeof(memL2Ram));
    return (-1);
  }

  bases[1] = (void *)memL2Ram;

  /* The third buffer is the L3 table */
  val      = fw_shmGetEntry(getMemL3RamBufAddrCmd);
  memL3Ram = (uint8_t *) val.paL3BufPtr;

  if ((uint32_t)memL3Ram & (aligns[2] - 1))  {
    printf ("initPa: Pa_getBufferReq requires %d alignment for buffer 2, but address is 0x%08x\n", aligns[2], (uint32_t)memL3Ram);
    return (-1);
  }

  val = fw_shmGetEntry(getMemL3RamBufSizeCmd);
  if ((int) val.paL3MemSize <  sizes[2])  {
    printf ("initPa: Pa_getBufferReq requires %d bytes for buffer 2, have only %d\n", sizes[2], sizeof(memL3Ram));
    return (-1);
  }

  bases[2] = (void *)memL3Ram;

  paCfg.initTable = TRUE;
  paCfg.initDefaultRoute = TRUE;
  paCfg.baseAddr = (uint32_t)fw_passCfgVaddr;
  paCfg.sizeCfg = &paSize;
  paCfg.instPoolBaseAddr = (void *) shared_ObjsBase;

#ifndef SIMULATOR_SUPPORT
  paCfg.rmServiceHandle = rmClientServiceHandle;
#endif


  ret = Pa_create (&paCfg, bases, &tFramework.passHandle);
  if (ret != pa_OK)  {
    printf ("initPa: Pa_create returned with error code %d\n", ret);
    return (-1);
  }

  /* Publish pa Handle */
  val.paHandle = tFramework.passHandle;
  fw_shmSetEntry(setPAHndCmd, val);

   return (0);
}

/* The PA LLD instance is created, the PA firmware is
 * downloaded and started in Master Core and started in Local core */
int initPaLocal (void)
{
  sa_MCExample_entry_t val;
  paStartCfg_t				  paStartCfg;
  memset(&paStartCfg, 0, sizeof(paStartCfg_t));

  val       = fw_shmGetEntry(getPAHndCmd);
  tFramework.passHandle = val.paHandle;

  paStartCfg.baseAddr = (uint32_t) fw_passCfgVaddr;
  paStartCfg.instPoolBaseAddr = (void *) shared_ObjsBase;

  Pa_startCfg(tFramework.passHandle, &paStartCfg);
  return (0);

}


/* Initialize the test framework */
int setupTestFramework (void)
{
#ifndef SIMULATOR_SUPPORT
	if (setupRm ())
	{
	  System_printf ("Function setupRm failed\n");
	  System_flush();
	  return (-1);
	}
#endif

    /* Allocate memory for test packets & security context */
    if (fw_test_mem_init()) {
		printf ("setupTestFramework: fw_test_mem_init returned error, exiting\n");
		return (-1);
    }

	/* Create the PA driver instance */
	if (initPa())  {
		printf ("setupTestFramework: initPa returned error, exiting\n");
		return (-1);
	}

	/* Setup the QM with associated buffers and descriptors */
	if (initQm())  {
		printf ("setupTestFramework: initQm returned error, exiting\n");
		return (-1);
	}

    /* Initialize the SA unit test support and create the SA driver instance */
    salld_sim_initialize();

	return (0);
}

/* Initialize the test framework for the local cores (consumer cores)  */
int setupTestFrameworkLocal (void)
{
	/* Get the global flow */
	sa_MCExample_entry_t val = fw_shmGetEntry(getGFlowId);
	tFramework.tfFlowNum = (uint8_t) val.flowId;

    /* Allocate memory for test packets & security context */
    if (fw_test_mem_init_local()) {
		printf ("setupTestFramework: fw_test_mem_init_local returned error, exiting\n");
		return (-1);
    }

	/* Setup the QM with associated buffers and descriptors */
	if (initQm())  {
		System_printf ("setupTestFramework: initQm returned error, exiting\n");
		return (-1);
	}

	/* Create the PA driver instance */
	if (initPaLocal())  {
		System_printf ("setupTestFramework: initPa returned error, exiting\n");
		return (-1);
	}

    /* Initialize the SA unit test support and create the SA driver instance */
    salld_sim_initialize_local();

	return (0);

}

static int closeFlows(void)
{
    Cppi_Result cppi_result;
    {
		if ((cppi_result = Cppi_closeRxFlow (tFramework.tfPaFlowHnd)) != CPPI_SOK) {
			System_printf ("Cppi_closeRxFlow err: %d \n", cppi_result);
			return (-1);
		}
    }
    return cppi_result;
}

static int closeQueuesLocal(void)
{
	int i;

    /* The 10 PA transmit queues (corresponding to the 10 tx cdma channels */
 	for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)  {
 	 	 Qmss_queueClose (tFramework.QPaTx[i]);
 	}

	Qmss_queueClose(tFramework.QLinkedBuf1);

	Qmss_queueClose(tFramework.QLinkedBuf2);

	Qmss_queueClose(tFramework.QLinkedBuf3);

	Qmss_queueClose(tFramework.QLinkedBuf4);

	Qmss_queueClose(tFramework.QfreeDesc);

	/* General purpose queues */
	for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
		Qmss_queueClose(tFramework.QGen[i]);
	}

	return 0;
}


static int closeQueues(void)
{
	int i;

    /* The 10 PA transmit queues (corresponding to the 10 tx cdma channels */
 	for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)  {
 		 Qmss_queueEmpty (tFramework.QPaTx[i]);
 	 	 Qmss_queueClose (tFramework.QPaTx[i]);
 	}

	Qmss_queueEmpty(tFramework.QLinkedBuf1);
	Qmss_queueClose(tFramework.QLinkedBuf1);

	Qmss_queueEmpty(tFramework.QLinkedBuf2);
	Qmss_queueClose(tFramework.QLinkedBuf2);

	Qmss_queueEmpty(tFramework.QLinkedBuf3);
	Qmss_queueClose(tFramework.QLinkedBuf3);

	Qmss_queueEmpty(tFramework.QLinkedBuf4);
	Qmss_queueClose(tFramework.QLinkedBuf4);

	Qmss_queueEmpty(tFramework.QfreeDesc);
	Qmss_queueClose(tFramework.QfreeDesc);

	/* General purpose queues */
	for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
		Qmss_queueEmpty(tFramework.QGen[i]);
		Qmss_queueClose(tFramework.QGen[i]);
	}

	return 0;
}

static int closeQmMem(void)
{
	Qmss_Result qmss_result;

    if ((qmss_result = Qmss_removeMemoryRegion (Qmss_MemRegion_MEMORY_REGION0, 0)) != QMSS_SOK)
    {
         System_printf ("Error closeQmMem: Remove memory region error code : %d\n", qmss_result);
         return qmss_result;
    }

    if ((qmss_result = Qmss_exit ()))
    {
        System_printf ("Error closeQmMem: exit error code : %d\n", qmss_result);
        return qmss_result;
    }

    return qmss_result;
}


static int closeCpdma(void)
{
	int i;
    Cppi_Result cppi_result;

	for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)
	{
   		 /* close the channel only if it was opened by it */
		 if (tFramework.tfPaTxChHnd[i] != NULL) {
    	     if ((cppi_result = Cppi_channelClose (tFramework.tfPaTxChHnd[i])) != CPPI_SOK) {
    	        System_printf ("Cppi_channelClose for Tx err: %d \n", cppi_result);
    	 	    return (cppi_result);
    	     }
		 }
	}

	for (i = 0; i < TF_PA_NUM_RX_CPDMA_CHANNELS; i++)
	{
		/* close the channel only if it was opened by it */
		if (tFramework.tfPaRxChHnd[i] != NULL) {
    	    if ((cppi_result = Cppi_channelClose (tFramework.tfPaRxChHnd[i])) != CPPI_SOK) {
    	        System_printf ("Cppi_channelClose for Rx err: %d \n", cppi_result);
    	        return (cppi_result);
    	    }
		}
	}

	/* Close CPPI CPDMA instance */
	{
		if ((cppi_result = Cppi_close (tFramework.tfPaCppiHandle)) != CPPI_SOK)
		{
			System_printf ("Error: Closing CPPI CPDMA error code : %d\n", cppi_result);
			return (cppi_result);
		}


		/* Deinitialize CPPI LLD */
		if ((cppi_result = Cppi_exit ()) != CPPI_SOK)
		{
			System_printf ("Error : Exiting CPPI error code : %d\n", cppi_result);
			return (cppi_result);
		}
	}

 	return (cppi_result);
}

static int exitQm(void)
{
	  if (closeFlows ())  {
	  	System_printf ("exitQm: closeFlows failed\n");
	    return (-1);
	  }

	  if (closeQueues ())  {
	  	System_printf ("exitQm: closeQueues failed\n");
	    return (-1);
	  }

	  if (closeCpdma ())  {
	  	System_printf ("exitQm: closeCpdma failed\n");
	    return (-1);
	  }

	  if (closeQmMem())  {
		  	System_printf ("exitQm: closeQmMem failed\n");
		    return (-1);
	  }

	  return 0;
}

static int exitQmLocal(void)
{
	  if (closeQueuesLocal ())  {
	  	System_printf ("exitQm: closeQueues failed\n");
	    return (-1);
	  }

	  return 0;
}


/* clean up the test framework */
int exitTestFramework (void)
{
	/* exit the CPPI/QM with associated buffers and descriptors */
	if (exitQm())  {
		System_printf ("exitTestFramework: exitQm returned error, exiting\n");
		return (-1);
	}

	return (0);

}

/* clean up the test framework */
int exitTestFrameworkLocal (void)
{
	/* exit the CPPI/QM with associated buffers and descriptors */
	if (exitQmLocal())  {
		System_printf ("exitTestFrameworkLocal: exitQmLocal returned error, exiting\n");
		return (-1);
	}
	return (0);
}

/* Check that all the queues are setup correctly */
int verifyTestFramework (void)
{
	int i, j;
	int count;
	int returnVal = 0;
	Cppi_HostDesc *hd;
	uint8_t *bufp;
	uint32_t bufLen;
	int32_t linkedQ[4];
	int32_t nbufs[]  = { TF_LINKED_BUF_Q1_NBUFS,    TF_LINKED_BUF_Q2_NBUFS,    TF_LINKED_BUF_Q3_NBUFS,    TF_LINKED_BUF_Q4_NBUFS };
	int32_t bSize[]  = { TF_LINKED_BUF_Q1_BUF_SIZE, TF_LINKED_BUF_Q2_BUF_SIZE, TF_LINKED_BUF_Q3_BUF_SIZE, TF_LINKED_BUF_Q4_BUF_SIZE};

	linkedQ[0] = tFramework.QLinkedBuf1;
	linkedQ[1] = tFramework.QLinkedBuf2;
	linkedQ[2] = tFramework.QLinkedBuf3;
	linkedQ[3] = tFramework.QLinkedBuf4;

	/* Verify that all of the general purpose queues are empty */
	for (i = 0; i < TF_NUM_GEN_QUEUES; i++)  {
		if ((count = Qmss_getQueueEntryCount (tFramework.QGen[i])) != 0)  {
			printf ("verifyTestFramework: Expected 0 entry count for queue %d, found %d entries\n", tFramework.QGen[i], count);
			returnVal = -1;
		}
	}

	/* Verify that the number of descriptors in the free descriptor queue is correct */
	count = Qmss_getQueueEntryCount (tFramework.QfreeDesc);
	if (count != (TF_NUM_DESC - TF_LINKED_BUF_Q1_NBUFS - TF_LINKED_BUF_Q2_NBUFS - TF_LINKED_BUF_Q3_NBUFS  - TF_LINKED_BUF_Q4_NBUFS))  {
		printf ("verifyTestFramework: Expected %d entry count in the free descriptor queue (%d), found %d\n",
						TF_NUM_DESC - TF_LINKED_BUF_Q1_NBUFS - TF_LINKED_BUF_Q2_NBUFS - TF_LINKED_BUF_Q3_NBUFS - TF_LINKED_BUF_Q4_NBUFS,
						tFramework.QfreeDesc, count);
		returnVal = -1;
	}

	/* Verify the number and sizing of descriptors with linked buffers in the three queues */
	for (j = 0; j < 4; j++)  {

		count = Qmss_getQueueEntryCount (linkedQ[j]);
		if (count != nbufs[j])  {
		printf ("verifyTestFramework: Expected %d entry count in linked buffer queue 1 (%d), found %d\n",
						nbufs[j], linkedQ[j], count);
		returnVal = -1;
		}

		for (i = 0; i < count; i++)  {
			hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (linkedQ[j])) & ~15);
			Cppi_getOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, &bufp, &bufLen);
			Qmss_queuePush (linkedQ[j], (Ptr)hd, hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);

			if (bufLen != bSize[j])  {
				printf ("verifyTestFramework: Linked buffer queue %d (%d) expected orignal length of %d, found %d\n",
								j, linkedQ[j], bSize[j], bufLen);
				returnVal = -1;
				break;
			}
		}
	}


	return (returnVal);
}

