/*
 * Copyright (c) 2016-2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== Mmu.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>

#include <ti/osal/DebugP.h>

#include "Hwi.h"
#include "Cache.h"


#include "Mmu.h"

#define Mmu_PADDR_MASK          0x0000FFFFFFFFFFFFU

#define Mmu_GRANULE_SIZE_4KB    0x000
#define Mmu_GRANULE_SIZE_16KB   0x100
#define Mmu_GRANULE_SIZE_64KB   0x080

#define Mmu_OUTER_SHAREABLE     0x0400U
#define Mmu_OUTER_CACHEABLE     0x2000U
#define Mmu_INNER_CACHEABLE     0x0800U

#define Mmu_MEMTYPE_CACHEABLE   0x08000U

#define Mmu_WALK_EN             0x1U
#define Mmu_tableArraySize      16U
#define Mmu_tableLength       ((uint32_t)0x200U)

extern uint64_t *Mmu_level1Table;

extern uint64_t  Mmu_tableArray[Mmu_tableLength*Mmu_tableArraySize];
extern uint64_t  Mmu_tableArraySlot;

#if Hwi_bootToNonSecure__D

#define Mmu_level1Table_NS    Mmu_level1Table_NS
#define Mmu_tableArray_NS     Mmu_tableArray_NS
#define Mmu_tableArraySlot_NS Mmu_tableArraySlot_NS

extern uint64_t *Mmu_level1Table_NS;
extern uint64_t  Mmu_tableArray_NS[Mmu_tableLength*Mmu_tableArraySize];
extern uint64_t  Mmu_tableArraySlot_NS;

#endif

/*
 * TODO Pending work
 * =================
 *  - Add custom memset() for BIOS (name it differently). Copy implementation
 *    from RTS version.
 */

/*
 *  ======== Mmu_addBlockEntry ========
 */
void Mmu_addBlockEntry(uint8_t level, uint64_t *tablePtr, uint16_t tableIdx,
    uint64_t paddr, Mmu_MapAttrs *mapAttrs)
{
    uint64_t desc;
    uint64_t nsVar, privExecute, userExecute, global;

    if (level == 3U) {
        desc = 0x3ULL;
    }
    else {
        desc = (uint64_t)Mmu_DescriptorType_BLOCK;
    }
    
    if(mapAttrs->ns) {
        nsVar = 1ULL;    
    }
    else {
        nsVar = 0ULL;        
    }
    if(!(mapAttrs->global)) {
        global = 1U;
    }
    else {
        global = 0U;
    }
    if(!(mapAttrs->privExecute)) {
       privExecute = 1U;
    }
    else {
       privExecute = 0U;
    }
    if(!(mapAttrs->userExecute)) {
      userExecute = 1U;
    }
    else {
      userExecute = 0U;
    }
    desc |= ((uint64_t)(mapAttrs->attrIndx & 0x7ULL) << 2ULL) |
            ((uint64_t) (nsVar << 5ULL)) |
            ((uint64_t)(mapAttrs->accessPerm & 0x3ULL) << 6ULL) |
            ((uint64_t)(mapAttrs->shareable & 0x3ULL) << 8ULL) |
            ((uint64_t)(0x1ULL) << 10ULL) |  /* access flag */
            ((uint64_t)(global & 0x1ULL) << 11ULL) |
            ((uint64_t)(paddr & ~((1ULL << Mmu_configInfo.tableOffset[level]) - 1ULL))) |
            ((uint64_t)(privExecute & 0x1U) << 53U) |
            ((uint64_t)(userExecute & 0x1U) << 54U);

    tablePtr[tableIdx] = desc;
}

/*
 *  ======== Mmu_addTableEntry ========
 */
uint64_t* Mmu_addTableEntry(uint64_t *tablePtr, uint16_t tableIdx,
    Mmu_MapAttrs *mapAttrs, bool secure)
{
    uint64_t desc, *newTable;

    /* Mmu_allocTable(secure); can never return NULL, as Hwi_bootToNonSecure__D is already defined. */
    newTable = Mmu_allocTable(secure);

    desc = ((uint64_t)Mmu_DescriptorType_TABLE & 0x3U) |
           ((uint64_t)newTable & ~(Mmu_granuleSize - 1U));
    tablePtr[tableIdx] = desc;

    return (newTable);
}

/*
 *  ======== Mmu_allocTable ========
 */
uint64_t* Mmu_allocTable(bool secure)
{
    uint64_t *table;
    unsigned int tableLen = (uint32_t)(Mmu_granuleSize >> 3U);
    uint64_t *slot;
    uint64_t i;

    if (secure) {
        table = &Mmu_tableArray[tableLen * Mmu_tableArraySlot];
        slot = &Mmu_tableArraySlot;
    }
#if Hwi_bootToNonSecure__D
    else {
        table = &Mmu_tableArray_NS[tableLen * Mmu_tableArraySlot_NS];
        slot = &Mmu_tableArraySlot_NS;
    }
#else
    else {
        return (NULL);
    }
#endif

#if Hwi_bootToNonSecure__D
    /* Slot can never be NULL as Mmu_tableArraySlot and Mmu_tableArraySlot_NS are global uin64_t */
    *slot = *table;
#endif
    /* Zero-out level 1 table */
    for (i = 0U; i < tableLen; i++) {
        /*
         * Use (i << 2) instead of 0 to init table, in order to force
         * compiler to not use memset() as an optimization
         */
        table[i] = (uint64_t)(i << 2U);
    }

    return (table);
}

/*
 *  ======== Mmu_disable ========
 *  Function to disable the MMU.
 */
void Mmu_disable(void)
{
    uint32_t key;
    bool     retval = BTRUE;

    /* if MMU is alreay disabled, just return */
    if (!Mmu_isEnabled()) {
        retval = BFALSE;
    }

    if (retval != BFALSE) {
        key = Hwi_disable();
        /* disable the L1 data cache */
        Cache_disable(Cache_Type_L1D);

        /* disables the MMU */
        Mmu_disableI();

        /* Invalidate entire TLB */
        Mmu_tlbInvAll(0U);

        /* set cache back to initial settings */
        Cache_enable(Cache_Type_L1D);

        Hwi_restore(key);
  }
  return;
}

/*
 *  ======== Mmu_disableI ========
 */
void Mmu_disableI(void)
{
    asm("\t mvk64.l1    0xfffffffffffffffeLL, a2 \n"
        "\t mvc.s1      SCR, a3                  \n"
        "\t nop                                  \n"
        "\t andd.l1     a2, a3, a3               \n"
        "\t mvc.s1      a3, SCR");
}

/*
 *  ======== Mmu_enable ========
 * This API should not be called from Secure Supervisor mode.
 */
void Mmu_enable(void)
{
    uint32_t key;
    bool     retval = BTRUE;

    /* if MMU is already enabled then just return */
    if (Mmu_isEnabled()) 
    {
        retval = BFALSE;
    }
    if(retval != BFALSE) 
    {
        key = Hwi_disable();

        Cache_disable((uint16_t)Cache_Type_L1D);

        /* Invalidate entire TLB */
        Mmu_tlbInvAll(0U);

        /* enables the MMU */
        Mmu_enableI();

        Cache_enable((uint16_t)Cache_Type_L1D);

        Hwi_restore(key);
    }
    return;  
  }

/*
 *  ======== Mmu_enableI_secure ========
 */
void Mmu_enableI_secure(void)
{
    asm("\t mvk64.l1    0x80000000000000C1LL, a2 \n"
        "\t mvc.s1      SCR, a3                  \n"
        "\t nop                                  \n"
        "\t ord.l1      a2, a3, a3               \n"
        "\t mvc.s1      a3, SCR                  \n"
        "\t mvc.s1      a3, SCR_S");
}

/*
 *  ======== Mmu_enableI ========
 */
void Mmu_enableI(void)
{
    asm("\t mvk64.l1    0x80000000000000C1LL, a2 \n"
        "\t mvc.s1      SCR, a3                \n"
        "\t nop                                  \n"
        "\t ord.l1      a2, a3, a3               \n"
        "\t mvc.s1      a3, SCR");
}

/*
 *  ======== Mmu_freeTable ========
 */
void Mmu_freeTable(uint64_t *table)
{
    *table = Mmu_tableArraySlot;
    Mmu_tableArraySlot =(uint64_t)(((uint64_t)table - (uint64_t)Mmu_tableArray) / ((uint64_t)Mmu_granuleSize >> 3U));
}

/*
 *  ======== Mmu_initMapAttrs ========
 */
void Mmu_initMapAttrs(Mmu_MapAttrs *attrs)
{
    /* Assert that attrs != NULL */
    DebugP_assert(attrs != NULL);

    attrs->ns = Mmu_defaultMapAttrs.ns;
    attrs->accessPerm = Mmu_defaultMapAttrs.accessPerm;
    attrs->privExecute = Mmu_defaultMapAttrs.privExecute;
    attrs->userExecute = Mmu_defaultMapAttrs.userExecute;
    attrs->shareable = Mmu_defaultMapAttrs.shareable;
    attrs->attrIndx = Mmu_defaultMapAttrs.attrIndx;
    attrs->global = Mmu_defaultMapAttrs.global;
}

/*
 *  ======== Mmu_map ========
 */
bool Mmu_map(uint64_t vaddr, uint64_t paddr, size_t size, Mmu_MapAttrs *mapAttrs, bool secure)
{
    unsigned int key;
    uint64_t     *tablePtr;
    bool         retStatus, enabled;
    uint64_t     vAddr     = vaddr;
    uint64_t     pAddr     = paddr;
    size_t       addrRange = size;

    /* Assert that mapAttrs != NULL */
    DebugP_assert(mapAttrs != NULL);

    /* Range check on vaddr and paddr */
    DebugP_assert(vaddr <= Mmu_PADDR_MASK);
    DebugP_assert(paddr <= Mmu_PADDR_MASK);

    /* Alignment check on vaddr, paddr & size */
    DebugP_assert((vaddr & (Mmu_granuleSize - 1U)) == 0U);
    DebugP_assert((paddr & (Mmu_granuleSize - 1U)) == 0U);
    DebugP_assert((size & (Mmu_granuleSize - 1U)) == 0U);

    key = Hwi_disable();

    /* determine the current state of the MMU */
    enabled = Mmu_isEnabled();

    /* disable the MMU (if already disabled, does nothing) */
    Mmu_disable();

    if (secure) {
        tablePtr = Mmu_level1Table;
    }
#if Hwi_bootToNonSecure__D
    else {
        tablePtr = Mmu_level1Table_NS;
    }
#else
    else {
        return (BFALSE);
    }
#endif

    if (Mmu_configInfo.noLevel0Table) {
        retStatus = Mmu_tableWalk(1, tablePtr, &vAddr, &pAddr,
            &addrRange, mapAttrs, secure);
    }
    else {
        retStatus = Mmu_tableWalk(0, tablePtr, &vAddr, &pAddr,
            &addrRange, mapAttrs, secure);
    }

    if (enabled) {
        /* if Mmu was enabled, then re-enable it */
        Mmu_enable();
    }

    Hwi_restore(key);

    return (retStatus);
}

/*
 *  ======== Mmu_readBlockEntry ========
 */
void Mmu_readBlockEntry(uint8_t level, uint64_t *tablePtr, uint16_t tableIdx,
    uint64_t *paddr, Mmu_MapAttrs *mapAttrs)
{
    uint64_t desc;

    desc = tablePtr[tableIdx];

    mapAttrs->attrIndx = (Mmu_AttrIndx)((desc >> 2U) & 0x7U);
    if(((desc >> 5U) & 0x1U) !=0ULL) {
        mapAttrs->ns = BTRUE;
    }
    else
    {
        mapAttrs->ns = BFALSE;
    }
    mapAttrs->accessPerm = (Mmu_AccessPerm)((desc >> 6U) & 0x3U);
    mapAttrs->shareable = (Mmu_Shareable)((desc >> 8U) & 0x3U);
    if(((desc >> 11U) & 0x1U) != 0U)
    {
        mapAttrs->global = BFALSE;
    }
    else
    {
        mapAttrs->global = BTRUE;
    }
    if(((desc >> 53U) & 0x1U)!= 0U)
    {
        mapAttrs->privExecute = BFALSE;
    }
    else
    {
        mapAttrs->privExecute = BTRUE;
    }
    if((((desc >> 54U) & 0x1U) != 0U))
    {
        mapAttrs->userExecute = BFALSE;
    }
    else
    {
        mapAttrs->userExecute = BTRUE;  
    }

    *paddr = desc & (uint64_t)Mmu_PADDR_MASK &
        ~((1U << Mmu_configInfo.tableOffset[level]) - 1U);
}

/*
 *  ======== Mmu_startup ========
 * Must only be called from Secure Supervisor context.
 * C7x at reset is in Secure Supervisor state, and the startup code
 * drops it down to root supervisor state.
 */
void Mmu_startup(void)
{
    uint64_t tcr = 0;
    unsigned int tableLen = Mmu_configInfo.tableLength;
    uint64_t  i;

    /* Initialize MAIR */
    Mmu_setMAIRAsm(0, Mmu_MAIR0);
    Mmu_setMAIRAsm(1, Mmu_MAIR1);
    Mmu_setMAIRAsm(2, Mmu_MAIR2);
    Mmu_setMAIRAsm(3, Mmu_MAIR3);
    Mmu_setMAIRAsm(4, Mmu_MAIR4);
    Mmu_setMAIRAsm(5, Mmu_MAIR5);
    Mmu_setMAIRAsm(6, Mmu_MAIR6);
    Mmu_setMAIRAsm(7, Mmu_MAIR7);

    if (Mmu_granuleSize == Mmu_GranuleSize_4KB) {
        tcr = Mmu_GRANULE_SIZE_4KB;
    }
    else if (Mmu_granuleSize == Mmu_GranuleSize_16KB) {
        tcr = Mmu_GRANULE_SIZE_16KB;
    }
    else if (Mmu_granuleSize == Mmu_GranuleSize_64KB) {
        tcr = Mmu_GRANULE_SIZE_64KB;
    }
    else {
       /* Do Nothing */
    }

    /*
     * TCR config:
     *  - MMU tables reside in outer and inner write-back write-allocate
     *    cacheable memory
     *  - Physical address size is 48-bits wide
     *  - TTBR0 is used for translating VA 0 to (2^48 - 1)
     */
    tcr = tcr | (uint64_t)Mmu_MEMTYPE_CACHEABLE | Mmu_OUTER_SHAREABLE |
          Mmu_OUTER_CACHEABLE | Mmu_INNER_CACHEABLE |
          ((64U - Mmu_PA_MAX_WIDTH) << 1U) | Mmu_WALK_EN;

    Mmu_setTCR(tcr, BTRUE);
    Mmu_setTCR(tcr, BFALSE);


    /* Initialize table array */
    for (i = 0U; i < Mmu_tableArrayLen; i++) {
        Mmu_tableArray[tableLen * i] = (uint64_t )(i + 1U);
#if Hwi_bootToNonSecure__D
        Mmu_tableArray_NS[tableLen * i] = (uint64_t)(i + 1U);
#endif
    }
    Mmu_tableArray[tableLen * (i - 1U)] = (~0U);
    Mmu_tableArraySlot = 0U;

    /* Allocate level1 Table */
    Mmu_level1Table = Mmu_allocTable(BTRUE);

#if Hwi_bootToNonSecure__D
    Mmu_tableArray_NS[tableLen * (i - 1U)] = (~0U);
    Mmu_tableArraySlot_NS = 0U;

    /* Allocate level1 Table */
    Mmu_level1Table_NS = Mmu_allocTable(BFALSE);
#endif

    /* Install MMU translation tables */
    Mmu_init(Mmu_level1Table, BTRUE);


#if Hwi_bootToNonSecure__D
    Mmu_init(Mmu_level1Table_NS, BFALSE);
#endif

    Mmu_initFunc();

    if (Mmu_enableMMU) {
        Mmu_enableI_secure();
    }

#if Hwi_bootToNonSecure__D
        Hwi_secureStart();
#endif
}

/*
 *  ======== Mmu_tableWalk ========
 */
bool Mmu_tableWalk(uint8_t level, uint64_t *tablePtr, uint64_t *vaddr, uint64_t *paddr,
    size_t *size, Mmu_MapAttrs *mapAttrs, bool secure)
{
    uint64_t desc;
    uint16_t tableIdx;
    size_t  blockSize;
    bool   retStatus;
    bool   blockTranslation;
    uint64_t *nextLevelTablePtr;
    bool doExit = BFALSE;

    blockTranslation = BTRUE;
    blockSize = (size_t)(1UL << Mmu_configInfo.tableOffset[level]);
    if ((level == 0U) ||
       ((level == 1U) && (Mmu_granuleSize != Mmu_GranuleSize_4KB))) {
        blockTranslation = BFALSE;
    }

    tableIdx = (uint16_t)((*vaddr >> Mmu_configInfo.tableOffset[level]) &
        Mmu_configInfo.indexMask);

    while ((*size != 0U) && (tableIdx < Mmu_configInfo.tableLength)) {

        desc = tablePtr[tableIdx];
        if (((desc & 0x3U) == Mmu_DescriptorType_TABLE) && (level != 3U)) {
            if ((blockTranslation == BTRUE) && (*size >= blockSize) &&
                ((*vaddr & (blockSize - 1U)) == 0U)) {
                Mmu_addBlockEntry(level, tablePtr, tableIdx, *paddr, mapAttrs);
                *size = *size - blockSize;
                *vaddr = *vaddr + blockSize;
                *paddr = *paddr + blockSize;
                Mmu_freeTable((uint64_t *)(Mmu_PADDR_MASK & desc &
                    ~(uint64_t)(Mmu_granuleSize - 1U)));
            }
            else {
                nextLevelTablePtr = (uint64_t *)(Mmu_PADDR_MASK & desc &
                    ~(uint64_t)(Mmu_granuleSize - 1U));
                retStatus = Mmu_tableWalk(level + 1U, nextLevelTablePtr,
                    vaddr, paddr, size, mapAttrs, secure);
                if (!retStatus) {
                    doExit = BTRUE;
                }
            }
        }
        else if (((desc & 0x3U) != Mmu_DescriptorType_TABLE) || (level == 3U)) {
            if ((level == 0U) || ((level < 3U) && (*size < blockSize)) ||
            ((*size >= blockSize) && ((*vaddr & (blockSize - 1U)) != 0U))) {
                uint64_t vaddrCopy = (*vaddr & (~(blockSize - 1U)));
                uint64_t paddrCopy;
                Mmu_MapAttrs mapAttrsCopy;
                size_t sizeCopy = blockSize;

                if ((desc & 0x3U) == Mmu_DescriptorType_BLOCK) {
                    Mmu_readBlockEntry(level, tablePtr, tableIdx, &paddrCopy,
                        &mapAttrsCopy);
                }

                nextLevelTablePtr =
                    Mmu_addTableEntry(tablePtr, tableIdx, mapAttrs, secure);
                if (nextLevelTablePtr == NULL) {
                    doExit = BTRUE;
                }

                if ((desc & 0x3U) == Mmu_DescriptorType_BLOCK) {
                    /*
                    * If old entry is a block entry, a new table entry is
                    * added and merged with the old block entry.
                    */
                    Mmu_tableWalk(level + 1U, nextLevelTablePtr, &vaddrCopy,
                        &paddrCopy, &sizeCopy, &mapAttrsCopy, secure);
                }

                retStatus = Mmu_tableWalk(level + 1U, nextLevelTablePtr,
                    vaddr, paddr, size, mapAttrs, secure);
                if (!retStatus) {
                    doExit = BTRUE;
                }
            }
            else if ((blockTranslation == BTRUE) && (*size >= blockSize)) {
                Mmu_addBlockEntry(level, tablePtr, tableIdx, *paddr, mapAttrs);
                *size = *size - blockSize;
                *vaddr = *vaddr + blockSize;
                *paddr = *paddr + blockSize;
            }
            else {
            /* Do Nothing */
            }
        }
        else {
        /* Do Nothing */
        }

        tableIdx++;

        if (BTRUE == doExit)
        {
            break;
        }
    }

    return !(doExit);
}

#if defined(__GNUC__) && !defined(__ti__)
#error "Unsupported compiler"
#else



#pragma DATA_SECTION(Mmu_tableArraySlot, ".data.Mmu_tableArraySlot");
#pragma DATA_ALIGN(Mmu_tableArraySlot, 4096);
uint64_t Mmu_tableArraySlot;
#pragma DATA_SECTION(Mmu_tableArraySlot_NS, ".data.Mmu_tableArraySlot_NS");
#pragma DATA_ALIGN(Mmu_tableArraySlot_NS, 4096);
uint64_t Mmu_tableArraySlot_NS;

#pragma DATA_SECTION(Mmu_level1Table, ".data.Mmu_level1Table");
#pragma DATA_ALIGN(Mmu_level1Table, 4096);
uint64_t* Mmu_level1Table;
#pragma DATA_SECTION(Mmu_level1Table_NS, ".data.Mmu_level1Table_NS");
#pragma DATA_ALIGN(Mmu_level1Table_NS, 4096);
uint64_t* Mmu_level1Table_NS;

#endif

/*
 * ======== ti.sysbios.family.c7x.Mmu INITIALIZERS ========
 */

/* defaultMapAttrs */
#pragma DATA_SECTION(Mmu_defaultMapAttrs, ".const:Mmu_defaultMapAttrs");
const Mmu_MapAttrs Mmu_defaultMapAttrs = {
    BTRUE,  /* ns */
    Mmu_AccessPerm_PRIV_RW_USER_NONE,  /* accessPerm */
    BTRUE,  /* privExecute */
    BFALSE,  /* userExecute */
    Mmu_Shareable_OUTER,  /* shareable */
    Mmu_AttrIndx_MAIR0,  /* attrIndx */
    BTRUE,  /* global */
};

/* enableMMU */
#pragma DATA_SECTION(Mmu_enableMMU, ".const:Mmu_enableMMU");
const bool Mmu_enableMMU = BTRUE;

/* granuleSize */
#pragma DATA_SECTION(Mmu_granuleSize, ".const:Mmu_granuleSize");
const Mmu_GranuleSize Mmu_granuleSize = Mmu_GranuleSize_4KB;

/* MAIR0 */
#pragma DATA_SECTION(Mmu_MAIR0, ".const:Mmu_MAIR0");
const uint8_t Mmu_MAIR0 = (uint8_t)0x0U;

/* MAIR1 */
#pragma DATA_SECTION(Mmu_MAIR1, ".const:Mmu_MAIR1");
const uint8_t Mmu_MAIR1 = (uint8_t)0x4U;

/* MAIR2 */
#pragma DATA_SECTION(Mmu_MAIR2, ".const:Mmu_MAIR2");
const uint8_t Mmu_MAIR2 = (uint8_t)0x8U;

/* MAIR3 */
#pragma DATA_SECTION(Mmu_MAIR3, ".const:Mmu_MAIR3");
const uint8_t Mmu_MAIR3 = (uint8_t)0xcU;

/* MAIR4 */
#pragma DATA_SECTION(Mmu_MAIR4, ".const:Mmu_MAIR4");
const uint8_t Mmu_MAIR4 = (uint8_t)0x44U;

/* MAIR5 */
#pragma DATA_SECTION(Mmu_MAIR5, ".const:Mmu_MAIR5");
const uint8_t Mmu_MAIR5 = (uint8_t)0x4fU;

/* MAIR6 */
#pragma DATA_SECTION(Mmu_MAIR6, ".const:Mmu_MAIR6");
const uint8_t Mmu_MAIR6 = (uint8_t)0xbbU;

/* MAIR7 */
#pragma DATA_SECTION(Mmu_MAIR7, ".const:Mmu_MAIR7");
const uint8_t Mmu_MAIR7 = (uint8_t)0x7dU;

/* tableArrayLen */
#pragma DATA_SECTION(Mmu_tableArrayLen, ".const:Mmu_tableArrayLen");
const uint32_t Mmu_tableArrayLen = 0x10U;

/* configInfo */
#pragma DATA_SECTION(Mmu_configInfo, ".const:Mmu_configInfo");
const Mmu_ConfigInfo Mmu_configInfo = {
    (uint64_t)0x1ffU,  /* indexMask */
    Mmu_tableLength,   /* tableLength */
    {
        (uint8_t)0x27U,  /* [0] */
        (uint8_t)0x1eU,  /* [1] */
        (uint8_t)0x15U,  /* [2] */
        (uint8_t)0xcU,  /* [3] */
    },  /* tableOffset */
    (uint8_t)0xcU,  /* granuleSizeBits */
    (uint8_t)0x9U,  /* indexBits */
    BFALSE,  /* noLevel0Table */
};

