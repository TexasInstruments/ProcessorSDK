/**
 *  \file   sbl_rprc.c
 *
 *  \brief  This file contains functions to parse the multi-core
 *          image file & loads into CPU internal memory & external memory.
 *
 */

/*
 * Copyright (C) 2015-2022 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string.h>

/* TI-RTOS Header files */
#include "sbl_rprc_parse.h"
#include "sbl_rprc.h"
#include "sbl_slave_core_boot.h"

/* ========================================================================== */
/*                             Global Variables                               */
/* ========================================================================== */
int32_t (*fp_readData)(void *dstAddr, void *srcAddr,
                        uint32_t length);

/* Function Pointer used while reading data from the storage. */
void  (*fp_seek)(void *srcAddr, uint32_t location);

/* Offset into app image that is being processed */
static uint32_t sblMemOffset = 0;

/* SBL scratch memory defined at compile time */
static uint8_t *sbl_scratch_mem_for_auth = ((uint8_t *)(SBL_SCRATCH_MEM_START));
static uint32_t sbl_scratch_sz = SBL_SCRATCH_MEM_SIZE;

/******************************************************************************
 ***                     SBL Multicore RPRC parse functions                 ***
*******************************************************************************/
#if defined (SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
#if defined(BUILD_MCU1_0)
extern void SBL_DCacheClean(void *addr, uint32_t size);
#endif
#endif

#if (SBL_USE_DMA && defined(BOOT_OSPI) && (defined (SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)))
extern int32_t SBL_OSPI_ReadSectors(void *dstAddr, void *srcOffsetAddr, uint32_t length);
#endif

extern uint32_t SBL_getAtcmSize(uint32_t CoreId);
extern uint32_t SBL_getBtcmSize(uint32_t CoreId);

/* read of block of data from buffer */
int32_t SBL_ReadMem(void       *buff,
                    void       *srcAddr,
                    uint32_t   size)
{
    memcpy(buff, (void*)(uintptr_t)((uint32_t)(uintptr_t)srcAddr + sblMemOffset), size);
    sblMemOffset += size;

    return E_PASS;
}

/* move the buffer pointer */
void SBL_SeekMem(void *srcAddr, uint32_t location)
{
    sblMemOffset = location;
}

void SBL_SetRsvdMem(uint8_t *scratch_mem, uint32_t scratch_sz)
{
    sbl_scratch_mem_for_auth = scratch_mem;
    sbl_scratch_sz = scratch_sz;
}

void SBL_SetMulticoreImageImgReadfxn(void *imge_read_fxn, void *image_seek_fxn)
{
    fp_readData = (int32_t (*)(void *, void *, uint32_t))imge_read_fxn;
    fp_seek = (void (*)(void *, uint32_t))image_seek_fxn;
}

__attribute__((weak)) int32_t SBL_VerifyMulticoreImage(void **img_handle,
                                                       uint32_t *ImageOffsetPtr,
                                                       uint8_t *scratch_mem,
                                                       uint32_t scratch_sz);
int32_t SBL_VerifyMulticoreImage(void **img_handle,
                                 uint32_t *ImageOffsetPtr,
                                 uint8_t *scratch_mem,
                                 uint32_t scratch_sz)
{
    int32_t retVal = E_FAIL;
    return retVal;
}

int32_t SBL_MulticoreImageParse(void *srcAddr,
                                uint32_t ImageOffset,
                                sblEntryPoint_t *pAppEntry,
                                uint32_t bootFlag)
{
    uint32_t            i;
    uint32_t            entryPoint = 0;
    meta_header_start_t mHdrStr;
    meta_header_core_t  mHdrCore[MAX_INPUT_FILES];
    meta_header_end_t   mHdrEnd;
    int32_t magic_str = META_HDR_MAGIC_STR;
    int32_t retVal    = E_PASS;

    if ((fp_readData == NULL) || (fp_seek == NULL))
    {
        retVal = E_FAIL;
    }
    else
    {
        /* Verify image if possible */
        SBL_VerifyMulticoreImage(&srcAddr, &ImageOffset, sbl_scratch_mem_for_auth, sbl_scratch_sz);

        /* Read Meta Header Start and get the Number of Input RPRC Files */
        fp_readData(&mHdrStr, srcAddr, sizeof (meta_header_start_t));
        if (mHdrStr.magic_str != (uint32_t) magic_str)
        {
            UART_printf("Invalid magic number in Single image header\r\n");
            retVal = E_FAIL;
        }
        else
        {
            if (mHdrStr.dev_id != GetDeviceId())
            {
                UART_printf("\nWARNING: Device Id Doesnot match\r\n");
            }

            if (mHdrStr.app_img_version != 1)
            {
                UART_printf("\nWARNING: You are using an app image generated with older version of SDk, please generate the app image again\r\n");
            }

            /* Read all the Core offset addresses */
            for (i = (0U); i < mHdrStr.num_files; i++)
            {
                fp_readData(&mHdrCore[i], srcAddr, sizeof (meta_header_core_t));
            }

            /* Add Base Offset address for All core Image start offset */
            for (i = (0U); i < mHdrStr.num_files; i++)
            {
                mHdrCore[i].image_offset += ImageOffset;
            }

            /* Read Meta Header End */
            fp_readData(&mHdrEnd, srcAddr, sizeof (meta_header_end_t));

            /* Now Parse Individual RPRC files */

            for (i = (0U); i < mHdrStr.num_files; i++)
            {
                if (mHdrCore[i].core_id != (0xFFFFFFFFU))
                {
                    fp_seek(srcAddr, mHdrCore[i].image_offset);
                    if (SBL_RprcImageParse(srcAddr, &entryPoint,
                                       (int32_t)(mHdrCore[i].core_id)) != E_PASS)
                    {
                        /* Error occurred parsing the RPRC file continue to
                         * parsing next
                         * image and skip booting the particular core */
                        UART_printf("RPRC parse error\n");
                        retVal = E_FAIL;
                    }
                    else
                    {
                        SBL_BootCore(entryPoint, mHdrCore[i].core_id, pAppEntry, bootFlag);
                    }
                }
            }
        }
    }
    return retVal;
}

void SBL_BootCore(uint32_t entry, uint32_t CoreID, sblEntryPoint_t *pAppEntry, uint32_t bootFlag)
{
    switch (CoreID)
    {
#if defined (SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
        case ONLY_LOAD_ID:
            /* Only loading, ignore entry point*/
            SBL_log(SBL_LOG_MAX, "Only load (not execute) image @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;
            break;

#if !defined(SBL_USE_MCU_DOMAIN_ONLY)
        case MPU1_CPU0_ID:
        case MPU1_CPU1_ID:
        case MPU1_CPU2_ID:
        case MPU1_CPU3_ID:
        case MPU2_CPU0_ID:
        case MPU2_CPU1_ID:
        case MPU2_CPU2_ID:
        case MPU2_CPU3_ID:
        case DSP1_C66X_ID:
        case DSP2_C66X_ID:
        case DSP1_C7X_ID:
        case DSP2_C7X_ID:
        case DSP3_C7X_ID:
        case DSP4_C7X_ID:
        case MCU2_CPU0_ID:
        case MCU2_CPU1_ID:
        case MCU3_CPU0_ID:
        case MCU3_CPU1_ID:
        case MCU4_CPU0_ID:
        case MCU4_CPU1_ID:
        case HSM_CPU_ID:
            /* All other non-bootloader cores*/
            SBL_log(SBL_LOG_MAX, "Setting entry point for core %d @0x%x\n", CoreID, entry);
            pAppEntry->CpuEntryPoint[CoreID] = entry;

            /* Immediately boot these cores */
            if (bootFlag == SBL_BOOT_AFTER_COPY)
            {
                SBL_SlaveCoreBoot(CoreID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
            }
            break;
#endif
        case MCU1_CPU0_ID:
        case MCU1_CPU1_ID:
            /* Bootloader cores */
            SBL_log(SBL_LOG_MAX, "Setting entry point for core %d @0x%x\n", CoreID, entry);
            pAppEntry->CpuEntryPoint[CoreID] = entry;

            /* DO NOT immediately boot these cores, SBL is still running! */

            break;
        case MPU1_SMP_ID:
            /* Cluster 1 SMP*/
            SBL_log(SBL_LOG_MAX, "Setting SMP entry point for MPU1 @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[MPU1_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MPU1_CPU1_ID] = entry;
            #if defined(SOC_J784S4) || defined(SOC_J742S2)
                pAppEntry->CpuEntryPoint[MPU1_CPU2_ID] = entry;
                pAppEntry->CpuEntryPoint[MPU1_CPU3_ID] = entry;
            #endif
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;

            /* Immediately boot these cores */
            if (bootFlag == SBL_BOOT_AFTER_COPY)
            {
                SBL_SlaveCoreBoot(MPU1_CPU0_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MPU1_CPU1_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                #if defined(SOC_J784S4) || defined(SOC_J742S2)
                    SBL_SlaveCoreBoot(MPU1_CPU2_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                    SBL_SlaveCoreBoot(MPU1_CPU3_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                #endif
            }

            break;
        case MPU2_SMP_ID:
            /* Cluster 2 SMP*/
            SBL_log(SBL_LOG_MAX, "Setting SMP entry point for MPU2 @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[MPU2_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MPU2_CPU1_ID] = entry;
            /* Not needed for J742S2 since mpu2_2, mpu2_3 is not present in the SOC */
            #if defined(SOC_J784S4)
                pAppEntry->CpuEntryPoint[MPU2_CPU2_ID] = entry;
                pAppEntry->CpuEntryPoint[MPU2_CPU3_ID] = entry;
            #endif
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;

            /* Immediately boot these cores */
            if (bootFlag == SBL_BOOT_AFTER_COPY)
            {
                SBL_SlaveCoreBoot(MPU2_CPU0_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MPU2_CPU1_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                /* Not needed for J742S2 since mpu2_2, mpu2_3 is not present in the SOC */
                #if defined(SOC_J784S4)
                    SBL_SlaveCoreBoot(MPU2_CPU2_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                    SBL_SlaveCoreBoot(MPU2_CPU3_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                #endif
            }

            break;
        case MPU_SMP_ID:
            /* SMP on both clusters*/
            SBL_log(SBL_LOG_MAX, "Setting SMP entry point for all MPUs @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[MPU1_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MPU1_CPU1_ID] = entry;
            pAppEntry->CpuEntryPoint[MPU2_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MPU2_CPU1_ID] = entry;
            /* Not needed for J742S2 since mpu2_2, mpu2_3 is not present in the SOC */
            #if defined(SOC_J784S4)
                pAppEntry->CpuEntryPoint[MPU1_CPU2_ID] = entry;
                pAppEntry->CpuEntryPoint[MPU1_CPU3_ID] = entry;
                pAppEntry->CpuEntryPoint[MPU2_CPU2_ID] = entry;
                pAppEntry->CpuEntryPoint[MPU2_CPU3_ID] = entry;
            #elif defined(SOC_J742S2)
                pAppEntry->CpuEntryPoint[MPU1_CPU2_ID] = entry;
                pAppEntry->CpuEntryPoint[MPU1_CPU3_ID] = entry;
            #endif
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;

            /* Immediately boot these cores */
            if (bootFlag == SBL_BOOT_AFTER_COPY)
            {
                SBL_SlaveCoreBoot(MPU1_CPU0_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MPU1_CPU1_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MPU2_CPU0_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MPU2_CPU1_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                /* Not needed for J742S2 since mpu2_2, mpu2_3 is not present in the SOC */
                #if defined(SOC_J784S4)
                    SBL_SlaveCoreBoot(MPU1_CPU2_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                    SBL_SlaveCoreBoot(MPU1_CPU3_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                    SBL_SlaveCoreBoot(MPU2_CPU2_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                    SBL_SlaveCoreBoot(MPU2_CPU3_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                #elif defined(SOC_J742S2)
                    SBL_SlaveCoreBoot(MPU1_CPU2_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                    SBL_SlaveCoreBoot(MPU1_CPU3_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                #endif
            }

            break;
        case MCU1_SMP_ID:
            /* Cluster 2 SMP*/
            SBL_log(SBL_LOG_MAX, "Setting Lockstep entry point for MCU1 @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[MCU1_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MCU1_CPU1_ID] = SBL_MCU_LOCKSTEP_ADDR;
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;

            /* DO NOT immediately boot these cores, SBL is still running! */

            break;
        case MCU2_SMP_ID:
            /* Cluster 2 SMP*/
            SBL_log(SBL_LOG_MAX, "Setting Lockstep entry point for MCU2 @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[MCU2_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MCU2_CPU1_ID] = SBL_MCU_LOCKSTEP_ADDR;
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;

            /* Immediately boot these cores */
            if (bootFlag == SBL_BOOT_AFTER_COPY)
            {
                SBL_SlaveCoreBoot(MCU2_CPU0_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MCU2_CPU1_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
            }

            break;
        case MCU3_SMP_ID:
            /* Cluster 3 SMP*/
            SBL_log(SBL_LOG_MAX, "Setting Lockstep entry point for MCU3 @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[MCU3_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MCU3_CPU1_ID] = SBL_MCU_LOCKSTEP_ADDR;
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;

            /* Immediately boot these cores */
            if (bootFlag == SBL_BOOT_AFTER_COPY)
            {
                SBL_SlaveCoreBoot(MCU3_CPU0_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MCU3_CPU1_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
            }

            break;
        case MCU4_SMP_ID:
            /* Cluster 4 SMP*/
            SBL_log(SBL_LOG_MAX, "Setting Lockstep entry point for MCU3 @0x%x\n", entry);
            pAppEntry->CpuEntryPoint[MCU4_CPU0_ID] = entry;
            pAppEntry->CpuEntryPoint[MCU4_CPU1_ID] = SBL_MCU_LOCKSTEP_ADDR;
            pAppEntry->CpuEntryPoint[CoreID] = SBL_INVALID_ENTRY_ADDR;

            /* Immediately boot these cores */
            if (bootFlag == SBL_BOOT_AFTER_COPY)
            {
                SBL_SlaveCoreBoot(MCU4_CPU0_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
                SBL_SlaveCoreBoot(MCU4_CPU1_ID, 0, pAppEntry, SBL_DONT_REQUEST_CORE);
            }

            break;
#endif
            default:
            /* Wrong CPU ID */
            break;
    }
}

#if defined (SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
/* */

__attribute__((weak)) void SBL_SetupCoreMem(uint32_t CoreID);
void SBL_SetupCoreMem(uint32_t core_id)
{
    return;
}

static int32_t SBL_RprcImageParse(void *srcAddr,
                                  uint32_t *entryPoint,
                                  int32_t CoreId)
{
    rprcFileHeader_t    header;
    rprcSectionHeader_t section;
    uint32_t sbl_rsvd_mem_start = (uint32_t)(SBL_SCRATCH_MEM_START);
    uint32_t sbl_rsvd_mem_end = (uint32_t)(((uint32_t)(SBL_SCRATCH_MEM_START)) + ((uint32_t)(SBL_SCRATCH_MEM_SIZE)));
    uint32_t i;
    int32_t retVal = E_PASS;
    uint32_t atcmSize;
    uint32_t btcmSize;

    const uint32_t SocAtcmAddr[] =
    {
#if (SBL_USE_DMA && (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)))
    /* Use SoC level address of MCU1_0 ATCM for non-CPU writes to this TCM. */
    SBL_MCU1_CPU0_ATCM_BASE_ADDR_SOC,
#else
    SBL_MCU_ATCM_BASE,
#endif
    SBL_MCU1_CPU1_ATCM_BASE_ADDR_SOC,
    SBL_MCU2_CPU0_ATCM_BASE_ADDR_SOC,
    SBL_MCU2_CPU1_ATCM_BASE_ADDR_SOC,
    SBL_MCU3_CPU0_ATCM_BASE_ADDR_SOC,
    SBL_MCU3_CPU1_ATCM_BASE_ADDR_SOC,
    SBL_MCU4_CPU0_ATCM_BASE_ADDR_SOC,
    SBL_MCU4_CPU1_ATCM_BASE_ADDR_SOC
    };

    const uint32_t SocBtcmAddr[] =
    {
#if (SBL_USE_DMA && (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)))
    /* Use SoC level address of MCU1_0 BTCM for non-CPU writes to this TCM. */
    SBL_MCU1_CPU0_BTCM_BASE_ADDR_SOC,
#else
    SBL_MCU_BTCM_BASE,
#endif
    SBL_MCU1_CPU1_BTCM_BASE_ADDR_SOC,
    SBL_MCU2_CPU0_BTCM_BASE_ADDR_SOC,
    SBL_MCU2_CPU1_BTCM_BASE_ADDR_SOC,
    SBL_MCU3_CPU0_BTCM_BASE_ADDR_SOC,
    SBL_MCU3_CPU1_BTCM_BASE_ADDR_SOC,
    SBL_MCU4_CPU0_BTCM_BASE_ADDR_SOC,
    SBL_MCU4_CPU1_BTCM_BASE_ADDR_SOC
    };

    const uint32_t SocC66xL2SramAddr[] =
    {
    SBL_C66X1_L2SRAM_BASE_ADDR_SOC,
    SBL_C66X2_L2SRAM_BASE_ADDR_SOC
    };

    const uint32_t SocC66xL1DmemAddr[] =
    {
    SBL_C66X1_L1DMEM_BASE_ADDR_SOC,
    SBL_C66X2_L1DMEM_BASE_ADDR_SOC
    };

    const uint32_t SocC7xL2SramAddr[] =
    {
    SBL_C7X1_L2SRAM_BASE_ADDR_SOC,
    SBL_C7X2_L2SRAM_BASE_ADDR_SOC
    };

    const uint32_t SocC7xL1DmemAddr[] =
    {
    SBL_C7X1_L1DMEM_BASE_ADDR_SOC,
    SBL_C7X2_L1DMEM_BASE_ADDR_SOC
    };

    const uint32_t SocHsmM4fDramAddr[] =
    {
    SBL_HSM_M4F_SRAM_BASE_ADDR_SOC
    };


    /*read application image header*/
    fp_readData(&header, srcAddr, sizeof (rprcFileHeader_t));

    /*check magic number*/
    if (header.magic != RPRC_MAGIC_NUMBER)
    {
        SBL_log(SBL_LOG_ERR, "Invalid magic number in boot image. Expected: %x, received: %x\n", RPRC_MAGIC_NUMBER, header.magic);
        retVal = E_FAIL;
    }
    else
    {
        /* Set the Entry Point */
        *entryPoint = header.entry;

        /* Setup CPUs internal memory before using it */
        SBL_SetupCoreMem(CoreId);
    
    /* Remap virtual core-ids if needed */
    switch (CoreId)
    {
        case MCU1_SMP_ID:
            CoreId = MCU1_CPU0_ID;
            break;
        case MCU2_SMP_ID:
            CoreId = MCU2_CPU0_ID;
            break;
        case MCU3_SMP_ID:
            CoreId = MCU3_CPU0_ID;
            break;
        case MCU4_SMP_ID:
            CoreId = MCU4_CPU0_ID;
            break;
        default:
            break;
    }

	/*read entrypoint and copy sections to memory*/
        for (i = (0U); i < header.SectionCount; i++)
        {
            fp_readData(&section, srcAddr, sizeof (rprcSectionHeader_t));

            switch (CoreId)
            {
#if (SBL_USE_DMA && defined(BOOT_OSPI) && (defined (SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)))
                /* Need address translation to SoC level addresses of MCU1_0 TCMs, when trying to copy to local addresses */
                case MCU1_CPU0_ID:
                    atcmSize = SBL_getAtcmSize(CoreId);
                    btcmSize = SBL_getBtcmSize(CoreId);
                    /* Only do TCM addr remapping for MCU1_0 if using UDMA for transfers from OSPI to local TCMs */
                    if (fp_readData == SBL_OSPI_ReadSectors)
                    {
                        /*Remap TCM address from R5 local to SoC memory map*/
                        if (section.addr < (SBL_MCU_ATCM_BASE + atcmSize))
                        {
                            /* Get offset into ATCM */
                            SBL_log(SBL_LOG_MAX, "Translating coreid %d local ATCM addr 0x%x to ", CoreId, section.addr);
                            section.addr = section.addr - SBL_MCU_ATCM_BASE;
                            section.addr = SocAtcmAddr[CoreId - MCU1_CPU0_ID] + section.addr;
                            SBL_log(SBL_LOG_MAX, "SoC MCU ATCM addr 0x%x\n", section.addr);
                        }
                        else if ((section.addr >= SBL_MCU_BTCM_BASE) &&
                                 (section.addr < (SBL_MCU_BTCM_BASE + btcmSize)))
                        {
                            /* Get offset into BTCM */
                            SBL_log(SBL_LOG_MAX, "Translating coreid %d local BTCM addr 0x%x to ", CoreId, section.addr);
                            section.addr = section.addr - SBL_MCU_BTCM_BASE;
                            section.addr = SocBtcmAddr[CoreId - MCU1_CPU0_ID] + section.addr;
                            SBL_log(SBL_LOG_MAX, "SoC MCU BTCM addr 0x%x\n", section.addr);
                        }
                        else
                        {
                            /* To remove MISRA C error */
                        }
                    }
                    break;
#endif
                case MCU1_CPU1_ID:
                case MCU2_CPU0_ID:
                case MCU2_CPU1_ID:
                case MCU3_CPU0_ID:
                case MCU3_CPU1_ID:
                case MCU4_CPU0_ID:
                case MCU4_CPU1_ID:
                    atcmSize = SBL_getAtcmSize(CoreId);
                    btcmSize = SBL_getBtcmSize(CoreId);
                    /*Remap TCM address from R5 local to SoC memory map*/
                    if (section.addr < (SBL_MCU_ATCM_BASE + atcmSize))
                    {
                        /* Get offset into ATCM */
                        SBL_log(SBL_LOG_MAX, "Translating coreid %d local ATCM addr 0x%x to ", CoreId, section.addr);
                        section.addr = section.addr - SBL_MCU_ATCM_BASE;
                        section.addr = SocAtcmAddr[CoreId - MCU1_CPU0_ID] + section.addr;
                        SBL_log(SBL_LOG_MAX, "SoC MCU ATCM addr 0x%x\n", section.addr);
                    }
                    else if ((section.addr >= SBL_MCU_BTCM_BASE) &&
                             (section.addr < (SBL_MCU_BTCM_BASE + btcmSize)))
                    {
                        /* Get offset into BTCM */
                        SBL_log(SBL_LOG_MAX, "Translating coreid %d local BTCM addr 0x%x to ", CoreId, section.addr);
                        section.addr = section.addr - SBL_MCU_BTCM_BASE;
                        section.addr = SocBtcmAddr[CoreId - MCU1_CPU0_ID] + section.addr;
                        SBL_log(SBL_LOG_MAX, "SoC MCU BTCM addr 0x%x\n", section.addr);
                    }
                    else
                    {
                        /* To remove MISRA C error */
                    }
                    break;
                case DSP1_C66X_ID:
                case DSP2_C66X_ID:
                    /*Remap L1 & L2 address from C66x local to SoC memory map*/
                    if ((section.addr >= SBL_C66X_L2SRAM_BASE) &&
                        (section.addr < (SBL_C66X_L2SRAM_BASE + SBL_C66X_L2SRAM_SIZE)))
                    {
                        /* Get offset into L2 SRAM */
                        SBL_log(SBL_LOG_MAX, "Translating C66x local L2SRAM addr 0x%x to ", section.addr);
                        section.addr = section.addr - SBL_C66X_L2SRAM_BASE;
                        section.addr = SocC66xL2SramAddr[CoreId - DSP1_C66X_ID] + section.addr;
                        SBL_log(SBL_LOG_MAX, "SoC C66x L2SRAM addr 0x%x\n", section.addr);
                    }
                    else if ((section.addr >= SBL_C66X_L1DMEM_BASE) &&
                             (section.addr < (SBL_C66X_L1DMEM_BASE + SBL_C66X_L1DMEM_SIZE)))
                    {
                        /* Get offset into L1 DRAM */
                        SBL_log(SBL_LOG_MAX, "Translating C66x local L1DMEM addr 0x%x to ", section.addr);
                        section.addr = section.addr - SBL_C66X_L1DMEM_BASE;
                        section.addr = SocC66xL1DmemAddr[CoreId - DSP1_C66X_ID] + section.addr;
                        SBL_log(SBL_LOG_MAX, "SoC C66x L2DMEM addr 0x%x\n", section.addr);
                    }
                    else
                    {
                        /* To remove MISRA C error */
                    }
                    break;
                case DSP1_C7X_ID:
                case DSP2_C7X_ID:
                case DSP3_C7X_ID:
                case DSP4_C7X_ID:
                    /*Remap L1 & L2 address from C7x local to SoC memory map*/
                    if ((section.addr >= SBL_C7X_L2SRAM_BASE) &&
                        (section.addr < (SBL_C7X_L2SRAM_BASE + SBL_C7X_L2SRAM_SIZE)))
                    {
                        /* Get offset into L2 SRAM */
                        SBL_log(SBL_LOG_MAX, "Translating C7x local L2SRAM addr 0x%x to ", section.addr);
                        section.addr = section.addr - SBL_C7X_L2SRAM_BASE;
                        section.addr = SocC7xL2SramAddr[CoreId - DSP1_C7X_ID] + section.addr;
                        SBL_log(SBL_LOG_MAX, "SoC C7x L2SRAM addr 0x%x\n", section.addr);
                    }
                    else if ((section.addr >= SBL_C7X_L1DMEM_BASE) &&
                             (section.addr < (SBL_C7X_L1DMEM_BASE + SBL_C7X_L1DMEM_SIZE)))
                    {
                        /* Get offset into L1 DRAM */
                        SBL_log(SBL_LOG_MAX, "Translating C7x local L1DMEM addr 0x%x to ", section.addr);
                        section.addr = section.addr - SBL_C7X_L1DMEM_BASE;
                        section.addr = SocC7xL1DmemAddr[CoreId - DSP1_C7X_ID] + section.addr;
                        SBL_log(SBL_LOG_MAX, "SoC C7x L2DMEM addr 0x%x\n", section.addr);
                    }
                    else
                    {
                        /* To remove MISRA C error */
                    }
                    break;
                case HSM_CPU_ID:
                    /*
                     * HSM entry is always specified as the local SRAM base
                     * (0x0000_0000)
                     */
                    *entryPoint = 0U;
                    /* Remap HSM local address to SoC address */
                    if ((section.addr >= 0U) &&
                        (section.addr < (SBL_HSM_M4F_SRAM_SIZE)))
                    {
                        SBL_log(SBL_LOG_MAX, "Translating HSM local SRAM addr 0x%x to ", section.addr);
                        section.addr = SocHsmM4fDramAddr[0U] + section.addr;
                        SBL_log(SBL_LOG_MAX, "SoC HSM SRAM addr 0x%x\n", section.addr);
                    }
                    else
                    {
                        /* To remove MISRA C error */
                    }
                    break;
                default:
                    break;
            }

            if ((fp_readData == SBL_ReadMem) &&
               (((section.addr > sbl_rsvd_mem_start) && (section.addr < sbl_rsvd_mem_end)) ||
                (((section.addr + section.size) > sbl_rsvd_mem_start) && ((section.addr + section.size) < sbl_rsvd_mem_end))))
            {
                SBL_log(SBL_LOG_ERR, "Warning!! Section overwrites SBL reserved memory\n");
                retVal = E_FAIL;
            }
            else
            {
                SBL_log(SBL_LOG_MAX, "Copying 0x%x bytes to 0x%x\n", section.size, section.addr);
                fp_readData((void *)(uintptr_t)(section.addr), srcAddr, section.size);
#if defined(BUILD_MCU1_0)
                SBL_DCacheClean((void *)(uintptr_t)(section.addr), section.size);
#endif
            }
        }
    }
    return retVal;
}
#endif

/**
 * \brief  Function to read the device ID
 *
 *
 * \param   None.
 *
 * \return   Return the device id
 *
 **/
uint32_t GetDeviceId(void)
{
    uint32_t deviceId = 55U;
    return (deviceId);
}
