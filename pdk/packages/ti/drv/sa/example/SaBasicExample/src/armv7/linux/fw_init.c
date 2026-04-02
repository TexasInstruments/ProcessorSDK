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
#include "../../unittest.h"

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

/* Host Descriptor Region - [Size of descriptor * Number of descriptors]
 * MUST be 16 byte aligned.
 */
uint8_t *pHostDesc = 0;

/* Static memory allocation for test framework */
uint8_t memPaInst[TF_PA_INST_SIZE]ALIGN(CACHE_LINESZ);
uint8_t memL2Ram[TF_L2_TABLE_SIZE]ALIGN(CACHE_LINESZ);
uint8_t memL3Ram[TF_L3_TABLE_SIZE]ALIGN(CACHE_LINESZ);

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

/*
 * Netss Local PKTDMA related convert functions
 */

void* fw_Netss_qmssVirtToPhy (void *ptr)
{
    uint32_t addr = (uint32_t) ptr;

    {
        if (addr >= (uint32_t)fw_passCfgVaddr)
        {
            addr = ((addr - (uint32_t)fw_passCfgVaddr) & 0x00FFFFFFUL) | 0xFF000000UL;
        }
    }

    return ((void *)addr);
}

void* fw_Netss_qmssPhyToVirt (void *ptr)
{
    uint32_t addr = (uint32_t) ptr;

    {
        if ((addr & 0xFF000000UL) == 0xFF000000UL)
        {
            addr = (addr & 0x00FFFFFFUL) + (uint32_t)fw_passCfgVaddr;
        }
    }

    return ((void *)addr);
}

void* fw_Netss_qmssConvertDescVirtToPhy(uint32_t QID, void *descAddr)
{
    uint32_t addr = (uint32_t) descAddr;

    {
        if (addr >= (uint32_t)fw_passCfgVaddr)
        {
            addr = ((addr - (uint32_t)fw_passCfgVaddr) & 0x00FFFFFFUL) | 0xFF000000UL;
        }
    }

    return ((void *)addr);
}

void* fw_Netss_qmssConvertDescPhyToVirt(uint32_t QID, void *descAddr)
{
    uint32_t addr = (uint32_t) descAddr;

    {
        if ((addr & 0xFF000000UL) == 0xFF000000UL)
        {
            addr = (addr & 0x00FFFFFFUL) + (uint32_t)fw_passCfgVaddr;
        }

    }

    return ((void *)addr);
}

/* Function to clear the command set use index and exception index use for PA */
void clearPaInfo(void)
{
    tFramework_t *tf = &tFramework;
    memset(tf, 0, sizeof(tFramework_t));
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

    /* Queue Manager Initialization Done */
    return 0;
}

/** ============================================================================
 *   @n@b initPassQmss
 *
 *   @b Description
 *   @n This API initializes the PASS internal QMSS on master processor only.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t initPassQmss (void)
{

#ifdef NETSS_INTERNAL_PKTDMA

    int32_t                     result;
    Qmss_MemRegInfo             memCfg;
    Qmss_InitCfg                qmssInitConfig;
    Qmss_StartCfg               qmssStartConfig;
    Cppi_DescCfg                cppiDescCfg;
    uint32_t                    numAllocated;
    Qmss_GlobalConfigParams     fw_qmssNetssGblCfgParams;
    uint32_t                    count;

    /* Initialize QMSS */
    memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));
    memset (&qmssStartConfig, 0, sizeof (Qmss_StartCfg));

    /* Set up QMSS configuration */

    /* Use internal linking RAM */
    qmssInitConfig.linkingRAM0Base  =   0;
    qmssInitConfig.linkingRAM0Size  =   TF_NUM_DESC;
    qmssInitConfig.linkingRAM1Base  =   0x0;
    qmssInitConfig.maxDescNum       =   TF_NUM_DESC;

    /* Bypass hardware initialization as it is done within Kernel */
    qmssInitConfig.qmssHwStatus     =   QMSS_HW_INIT_COMPLETE;

    fw_qmssNetssGblCfgParams = qmssNetssGblCfgParams;

    /* Convert address to Virtual address */
    for(count=0;count < fw_qmssNetssGblCfgParams.maxQueMgrGroups;count++)
    {

        fw_qmssNetssGblCfgParams.groupRegs[count].qmConfigReg =
            FW_GET_REG_VADDR(fw_passCfgVaddr,PASS_CFG_BASE_ADDR,(uint32_t)fw_qmssNetssGblCfgParams.groupRegs[count].qmConfigReg);

        fw_qmssNetssGblCfgParams.groupRegs[count].qmDescReg =
            FW_GET_REG_VADDR(fw_passCfgVaddr,PASS_CFG_BASE_ADDR,(uint32_t)fw_qmssNetssGblCfgParams.groupRegs[count].qmDescReg);

        fw_qmssNetssGblCfgParams.groupRegs[count].qmQueMgmtReg =
            FW_GET_REG_VADDR(fw_passCfgVaddr,PASS_CFG_BASE_ADDR,(uint32_t)fw_qmssNetssGblCfgParams.groupRegs[count].qmQueMgmtReg);

        fw_qmssNetssGblCfgParams.groupRegs[count].qmQueMgmtProxyReg = NULL;

        fw_qmssNetssGblCfgParams.groupRegs[count].qmQueStatReg =
            FW_GET_REG_VADDR(fw_passCfgVaddr,PASS_CFG_BASE_ADDR,(uint32_t)fw_qmssNetssGblCfgParams.groupRegs[count].qmQueStatReg);

        fw_qmssNetssGblCfgParams.groupRegs[count].qmStatusRAM = NULL;

        fw_qmssNetssGblCfgParams.groupRegs[count].qmQueMgmtDataReg =
            FW_GET_REG_VADDR(fw_passDataVaddr,PASS_CFG_BASE_ADDR,(uint32_t)fw_qmssNetssGblCfgParams.groupRegs[count].qmQueMgmtDataReg);

        fw_qmssNetssGblCfgParams.groupRegs[count].qmQueMgmtProxyDataReg = NULL; /* not supported on k2 hardware, and not used by lld */
    }

    fw_qmssNetssGblCfgParams.regs.qmLinkingRAMReg =
        FW_GET_REG_VADDR(fw_passCfgVaddr,PASS_CFG_BASE_ADDR,(uint32_t)fw_qmssNetssGblCfgParams.regs.qmLinkingRAMReg);

    fw_qmssNetssGblCfgParams.regs.qmBaseAddr =
        FW_GET_REG_VADDR(fw_passCfgVaddr,PASS_CFG_BASE_ADDR,(uint32_t)fw_qmssNetssGblCfgParams.regs.qmBaseAddr);

    // Supply virtual-2-physical conversion functions
    fw_qmssNetssGblCfgParams.virt2Phy     = fw_Netss_qmssVirtToPhy;
    fw_qmssNetssGblCfgParams.phy2Virt     = fw_Netss_qmssPhyToVirt;
    fw_qmssNetssGblCfgParams.virt2PhyDesc = fw_Netss_qmssConvertDescVirtToPhy;
    fw_qmssNetssGblCfgParams.phy2VirtDesc = fw_Netss_qmssConvertDescPhyToVirt;

    /* Initialize the Queue Manager */
    result = Qmss_initSubSys (&tFramework.tfPaQmssHandle, Qmss_SubSys_NETSS, &qmssInitConfig, &fw_qmssNetssGblCfgParams);
    if (result != QMSS_SOK)
    {
        printf ("initPassQmss: Error initializing Queue Manager SubSystem, Error code : %d\n", result);
        return -1;
    }

    /* Start Queue manager on this core */
    result = Qmss_startSubSysCfg(&tFramework.tfPaQmssHandle, Qmss_SubSys_NETSS, &qmssStartConfig);
    if (result != QMSS_SOK)  {
        printf ("initPassQmMem: Qmss_start failed with error code %d\n", result);
        return (-1);
    }

    /* Initialize and setup CPSW Host Descriptors required for example */
    memset (pHostDesc, 0, TF_SIZE_DESC * TF_NUM_DESC);
    memCfg.descBase             =   (uint32_t *) FW_GET_REG_VADDR(fw_qmssCfgVaddr,PASS_CFG_BASE_ADDR,(uint32_t)(CSL_NETCP_CFG_REGS + 0x001c0000));
    memCfg.descSize             =   TF_SIZE_DESC;
    memCfg.descNum              =   TF_NUM_DESC;
    memCfg.manageDescFlag       =   Qmss_ManageDesc_MANAGE_DESCRIPTOR;
    memCfg.memRegion            =   Qmss_MemRegion_MEMORY_REGION0;
    memCfg.startIndex           =   0;

    /* Insert Host Descriptor memory region */
    result = Qmss_insertMemoryRegionSubSys (tFramework.tfPaQmssHandle, &memCfg);
    if (result == QMSS_MEMREGION_ALREADY_INITIALIZED)
    {
        printf ("initPassQmss: Memory Region %d already Initialized \n", memCfg.memRegion);
    }
    else if (result < QMSS_SOK)
    {
        printf ("initPassQmss: Error Inserting memory region %d, Error code : %d\n", memCfg.memRegion, result);
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
     *      (2) Always return (entire packet) individual buffers to this free queue
     *      (3) Set that PS Data is always present in start of SOP buffer
     *      (4) Configure free q num < 4K, hence qMgr = 0
     *      (5) Recycle back to the same Free queue by default.
     */
    cppiDescCfg.returnPushPolicy            =   Qmss_Location_TAIL;
    //cppiDescCfg.cfg.host.returnPolicy       =   Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;
    cppiDescCfg.cfg.host.returnPolicy       =   Cppi_ReturnPolicy_RETURN_BUFFER;
    cppiDescCfg.cfg.host.psLocation         =   Cppi_PSLoc_PS_IN_DESC;
    cppiDescCfg.returnQueue.qMgr            =   0;
    cppiDescCfg.returnQueue.qNum            =   QMSS_PARAM_NOT_SPECIFIED;
    cppiDescCfg.epibPresent                 =   Cppi_EPIB_EPIB_PRESENT;

    /* Initialize the descriptors, create a free queue and push descriptors to a global free queue */
    if ((tFramework.QLocfreeDesc = Cppi_initDescriptorSubSys (tFramework.tfPaQmssHandle, &cppiDescCfg, &numAllocated)) <= 0)
    {
        printf ("initQmss: Error Initializing Free Descriptors, Error: %d \n", tFramework.QLocfreeDesc);
        return -1;
    }
    else
    {
        printf ("initQmss: Initialized Free Descriptors. \n");
    }

#endif

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

#if defined(DEVICE_K2K) || defined(DEVICE_K2H)  || defined(SOC_K2K)  || defined(SOC_K2H)
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

#if defined(DEVICE_K2L) || defined(DEVICE_K2E)  || defined(SOC_K2L)  || defined(SOC_K2E)

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
    int             i;

    /* The 10 PA transmit queues (corresponding to the 10 tx cdma channels */
    for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i ++)
    {
        if ((tFramework.QPaTx[i] = Qmss_queueOpen (Qmss_QueueType_PASS_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAlloc)) < 0)
        {
            printf ("setupQueues: Qmss_queueOpen failed for PA transmit queue number %d\n", TF_PA_TX_QUEUE_BASE+i);
            return -1;
        }
        Qmss_setQueueThreshold (tFramework.QPaTx[i], 1, 1);
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

    q.qNum = TF_LINKED_BUF_Q2;
    bPtr = (uint8_t *) fw_memAlloc((TF_LINKED_BUF_Q2_NBUFS * TF_LINKED_BUF_Q2_BUF_SIZE), CACHE_LINESZ);
    if(bPtr == NULL) {
  	    printf ("SetupQueues: memAlloc failed for memQ2\n");
  	    return (-1);
    }

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

    q.qNum = TF_LINKED_BUF_Q3;
    bPtr = (uint8_t *) fw_memAlloc((TF_LINKED_BUF_Q3_NBUFS * TF_LINKED_BUF_Q3_BUF_SIZE), CACHE_LINESZ);
    if(bPtr == NULL) {
  	    printf ("SetupQueues: memAlloc failed for memQ3\n");
  	    return (-1);
    }

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
    q.qNum = TF_LINKED_BUF_Q4;
    if(bPtr == NULL) {
  	    printf ("SetupQueues: memAlloc failed for memQ4\n");
  	    return (-1);
    }

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

  rxFlowCfg.rx_fdq0_sz3_qnum = tFramework.QLinkedBuf3;
  rxFlowCfg.rx_fdq0_sz3_qmgr = 0;
  rxFlowCfg.rx_fdq0_sz2_qnum = tFramework.QLinkedBuf3;
  rxFlowCfg.rx_fdq0_sz2_qmgr = 0;

  tFramework.tfPaFlowHnd = Cppi_configureRxFlow (tFramework.tfPaCppiHandle, &rxFlowCfg, &isAlloc);
  if (tFramework.tfPaFlowHnd == NULL)  {
    printf ("setupFlows: cppi_ConfigureRxFlow returned NULL\n");
    return (-1);
  }
  tFramework.tfFlowNum = (uint8_t) Cppi_getFlowId(tFramework.tfPaFlowHnd);

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
  if ((uint32_t)memPaInst & (aligns[pa_BUF_INST] - 1))  {
    printf ("initPa: Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[0], (uint32_t)memPaInst);
    return (-1);
  }

  if (sizeof(memPaInst) < sizes[pa_BUF_INST])  {
    printf ("initPa: Pa_getBufferReq requires size %d for instance buffer, have only %d\n", sizes[pa_BUF_INST], sizeof(memPaInst));
    return (-1);
  }

  bases[pa_BUF_INST] = (void *)memPaInst;


  /* The second buffer is the L2 table */
  if ((uint32_t)memL2Ram & (aligns[pa_BUF_L2_TABLE] - 1))  {
    printf ("initPa: Pa_getBufferReq requires %d alignment for L2 buffer, but address is 0x%08x\n", aligns[pa_BUF_L2_TABLE], (uint32_t)memL2Ram);
    return (-1);
  }

  if (sizeof(memL2Ram) <  sizes[pa_BUF_L2_TABLE])  {
    printf ("initPa: Pa_getBufferReq requires %d bytes for L2 buffer, have only %d\n", sizes[pa_BUF_L2_TABLE], sizeof(memL2Ram));
    return (-1);
  }

  bases[pa_BUF_L2_TABLE] = (void *)memL2Ram;

  /* The third buffer is the L3 table */
  if ((uint32_t)memL3Ram & (aligns[pa_BUF_L3_TABLE] - 1))  {
    printf ("initPa: Pa_getBufferReq requires %d alignment for L3 buffer, but address is 0x%08x\n", aligns[pa_BUF_L3_TABLE], (uint32_t)memL3Ram);
    return (-1);
  }

  if (sizeof(memL3Ram) <  sizes[pa_BUF_L3_TABLE])  {
    printf ("initPa: Pa_getBufferReq requires %d bytes for L3 buffer, have only %d\n", sizes[pa_BUF_L3_TABLE], sizeof(memL3Ram));
    return (-1);
  }

  bases[pa_BUF_L3_TABLE] = (void *)memL3Ram;

  paCfg.initTable = TRUE;
  paCfg.initDefaultRoute = TRUE;
  paCfg.baseAddr = (uint32_t)fw_passCfgVaddr;
  paCfg.sizeCfg = &paSize;

#ifndef SIMULATOR_SUPPORT
  paCfg.rmServiceHandle = rmClientServiceHandle;
#endif

  ret = Pa_create (&paCfg, bases, &tFramework.passHandle);
  if (ret != pa_OK)  {
    printf ("initPa: Pa_create returned with error code %d\n", ret);
    return (-1);
  }

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

static int closeFlows(void)
{
    Cppi_Result cppi_result;
    if ((cppi_result = Cppi_closeRxFlow (tFramework.tfPaFlowHnd)) != CPPI_SOK) {
       	return (-1);
    }
    return cppi_result;
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

static int closeCpdma(void)
{
	int i;
    Cppi_Result cppi_result;

	for (i = 0; i < TF_PA_NUM_TX_CPDMA_CHANNELS; i++)
	{
   		 /* close the channel only if it was opened by it */
		 if (tFramework.tfPaTxChHnd[i] != NULL) {
    	     if ((cppi_result = Cppi_channelClose (tFramework.tfPaTxChHnd[i])) != CPPI_SOK) {
    	 	    return (cppi_result);
    	     }
		 }
	}

	for (i = 0; i < TF_PA_NUM_RX_CPDMA_CHANNELS; i++)
	{
		/* close the channel only if it was opened by it */
		if (tFramework.tfPaRxChHnd[i] != NULL) {
    	    if ((cppi_result = Cppi_channelClose (tFramework.tfPaRxChHnd[i])) != CPPI_SOK) {
    	        return (cppi_result);
    	    }
		}
	}

	/* Close CPPI CPDMA instance */
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

 	return (cppi_result);
}

static int closeQmMem(void)
{
	Qmss_Result qmss_result;
	int         i;

    if ((qmss_result = Qmss_removeMemoryRegion (Qmss_MemRegion_MEMORY_REGION0, 0)) != QMSS_SOK)
    {
         System_printf ("Error : Remove memory region error code : %d\n", qmss_result);
         return qmss_result;
    }

    while ((qmss_result = Qmss_exit()) != QMSS_SOK)
    {
        for (i=0; i<100; i++); /* Wait for QM Exit */
    }

    return qmss_result;
}

/* clean up for QM/CPPI */
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


int exitTestFramework(void)
{
    salld_sim_close_sa();

	/* Clean up CPPI/QM entries */
	if (exitQm()) {
		System_printf ("exitTestFramework: exitQm returned error, exiting\n");
		return (-1);
	}

	return 0;
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

