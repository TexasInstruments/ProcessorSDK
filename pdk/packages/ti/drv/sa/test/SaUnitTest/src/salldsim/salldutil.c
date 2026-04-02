/**
 *   @file  salldutil.c
 *
 *   @brief   
 *      Common Utility functions used by SA Low Level Driver test file.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009-2020, Texas Instruments, Inc.
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
/*******************************************************************************
* FILE PURPOSE:	Security Accelerator (SA) LLD Simulator Utility Functions
*
********************************************************************************
* FILE NAME: 	salldutil.c
*
* DESCRIPTION: 	
*   This file contains utility routines.
*
* REVISION HISTORY:
*
* Note: 54x and 55x family are not supported
*
*******************************************************************************/
/* System level header files */
#include <stdint.h>
#include <stdio.h>
#include <string.h> 
/* SALLD component and simulation headers */
#include <ti/drv/sa/salld.h>
#include "salldsim.h"
#include "../unittest.h"
/******************************************************************************
 * Global Variable and Constant Definitions
 *****************************************************************************/
/*
 * Security Context ID and Buffer Management related definition
 *
 * Description: A simple Security Context Management scheme is provided here
 *      to support the SA LLD Unit test. 
 *      - Security Context Buffers are implemented as fixed-size double array
 *      - Security Context ID is equal to a fixed base number plus the buffer
 *        index
 *      - Free buffer indexes are managered by a simple FIFO 
 *        (TBD: changed to a simple bit mask search)  
 *
 */ 
#define SALLD_FORCE_SC_EVICT
#define SALLDSIM_SC_ID_BASE     0x4000
#define SALLDSIM_SC_ID_MASK     0x0FFF

#ifdef  SALLD_FORCE_SC_EVICT
    #define SALLDSIM_MK_SC_ID(index)        (index*16) | SALLDSIM_SC_ID_BASE
    #define SALLDSIM_EXTRACT_INDEX(id)      ((id) & SALLDSIM_SC_ID_MASK)>>4
#else
    #define SALLDSIM_MK_SC_ID(index)        (index) | SALLDSIM_SC_ID_BASE
    #define SALLDSIM_EXTRACT_INDEX(id)      ((id) & SALLDSIM_SC_ID_MASK)
#endif    

static uint16_t salldsimScBusyIndexMask[SALLDSIM_NUM_SC_BUF/16]; 

/* 
 * Packet Gen related definitions 
 */
#define SALLDSIM_MAX_PKT_SIZE   1024
#define SALLDSIM_PKT_GARD_SIZE  50 
uint16_t  salldPktBuf[(SALLDSIM_MAX_PKT_SIZE + SALLDSIM_PKT_GARD_SIZE*2)/2];

/*******************************************************************************
 *******************************************************************************
 *	Private Prototypes
 *******************************************************************************
 *******************************************************************************/
 
/*******************************************************************************
 *******************************************************************************
 *	Global Utility functions
 *******************************************************************************
 *******************************************************************************/
 
/*******************************************************************************
 *  Function:   salldsim_init_sc
 *******************************************************************************
 *  DESCRIPTION:
 *      Initailze the packet instance FIFO
 *  
 ******************************************************************************/
void salld_sim_init_sc(void)
{
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    memset(salldsimScBufSecure, 0, SALLDSIM_NUM_SC_BUF*SALLDSIM_SC_BUF_SIZE);
#else
    memset(salldsimScBuf, 0, SALLDSIM_NUM_SC_BUF*SALLDSIM_SC_BUF_SIZE);
#endif
    memset(salldsimScBusyIndexMask, 0, SALLDSIM_NUM_SC_BUF/8);
} 

/*******************************************************************************
 *  Function:   salldsim_free_sc
 *******************************************************************************
 *  DESCRIPTION:  Free the specific SC ID
 *  
 ******************************************************************************/
void salld_sim_free_sc(uint16_t scID)
{
    uint16_t index = SALLDSIM_EXTRACT_INDEX(scID);
    int indexOffset, bitOffset;    
    
    indexOffset = index/16;
    bitOffset=index%16;
    
    salldsimScBusyIndexMask[indexOffset] &= ~(1 << bitOffset);
} 

/*******************************************************************************
 *  Function:   salldsim_alloc_sc
 *******************************************************************************
 *  DESCRIPTION:  Allocate an SC ID and SC Buffer
 *
 *  Returns:
 *      TRUE: SC ID and Buffer is allocated successfully
 *      FALSE: SC ID or Buffer are not available
 *  
 ******************************************************************************/
#ifndef NSS_LITE2
int dbgScBufStuckCnt = 0;
uint8_t*  dbgScBufStucked[100];
#endif

bool salld_sim_alloc_sc(uint16_t scSize, uint16_t* p_scID, uint8_t** pp_scBuf)
{
    uint16_t indexOffset, bitOffset, value, index;
#ifdef NSS_LITE2
    uint64_t* pScBuf;
#else
    uint32_t* pScBuf;
#endif
    if (scSize > SALLDSIM_SC_BUF_SIZE)
    {
        SALog(" salld_sim_alloc_sc: scSize (%d)is greater than Max SC Buf Size \n", scSize);
        return FALSE;
    }
    
    for (indexOffset = 0; indexOffset < (SALLDSIM_NUM_SC_BUF/16); indexOffset++)
    {
        if ((value = salldsimScBusyIndexMask[indexOffset]) != 0xFFFF)
        {
            for (bitOffset = 0; bitOffset < 16; bitOffset++)
            {
                if (!(value & (1 << bitOffset)))
                {
                    index = bitOffset + indexOffset * 16;
#if defined(SAU_PRMOTE_DEMOTE_TEST)
                    pScBuf =  (uint64_t *)utilgAddr((uintptr_t)&salldsimScBufSecure[index][0]);
#else
                    pScBuf =  (uint64_t *)utilgAddr((uintptr_t)&salldsimScBuf[index][0]);
#endif
                    if (Sa_isScBufFree((uint8_t*) pScBuf))
                    {
                        break;
                    }
                    #ifndef NSS_LITE2
                    else
                    {
                        if(dbgScBufStuckCnt < 100)
                            dbgScBufStucked[dbgScBufStuckCnt++] = (uint8_t*) pScBuf;    
                    }    
                    #endif
                }
            } 
        
            if(bitOffset < 16)
                break;
        }
    }
    
    if(indexOffset >= (SALLDSIM_NUM_SC_BUF/16))
        return FALSE;
    
    *p_scID = SALLDSIM_MK_SC_ID(index);
    *pp_scBuf = (uint8_t *) pScBuf;
#if defined(SAU_PRMOTE_DEMOTE_TEST)
    memset(&salldsimScBufSecure[index][0], 0, SALLDSIM_SC_BUF_SIZE);
#else
    memset(&salldsimScBuf[index][0], 0, SALLDSIM_SC_BUF_SIZE);
#endif
    /* Mark the busy bit */
    salldsimScBusyIndexMask[indexOffset] |= (1 << bitOffset);
    
    return TRUE;
} 
 
/*******************************************************************************
 *  Function:   Convert Hex String to Binary Array
 *******************************************************************************
 *  DESCRIPTION:  Convert Hex String to Binary Array
 *
 *  Returns:
 *      TRUE:  Conversion succeeds
 *      FALSE: Error Occur
 *
 *  TBD: Enhancements for C55x/C54x device
 *  
 ******************************************************************************/
bool salld_sim_htob(char* hex_in, uint8_t* bin_out, uint16_t *pSize)
{
    uint8_t value;
    int   odd = 0, size = 0;
    
    while (*hex_in)
    {
        if (*hex_in >= '0' && *hex_in <= '9')
        {
            value = *hex_in - '0';
        }
        else if (*hex_in >= 'A' && *hex_in <= 'F') 
        {
            value = *hex_in - 'A' + 10;
        }
        else if (*hex_in >= 'a' && *hex_in <= 'f') 
        {
            value = *hex_in - 'a' + 10;
        }
        else
        {
            return FALSE;
        }
    
        if (odd)
        {
            *bin_out = (*bin_out << 4) + value;
            bin_out++;
            size++;
            odd = 0;
        }
        else
        {
            *bin_out = value;
            odd = 1;
        }   
        hex_in++;
    }
    
    if (odd)
    {
        *bin_out = (*bin_out << 4);
        size++;
    }
    
    *pSize = size;
    return TRUE;    
}

/*******************************************************************************
 *  Function:   salld_sim_init_pktdesc
 *******************************************************************************
 *  DESCRIPTION:  Initialize the packet descriptor
 *  
 ******************************************************************************/
void salld_sim_init_pktdesc(Sa_PktDesc_t *pktDesc, int32_t numSegments)
{
    pktDesc->nSegments = numSegments;
    pktDesc->segments = (void **)salldSimSegments;
    pktDesc->segUsedSizes = salldSimSegUsedSizes;
    pktDesc->segAllocSizes = salldSimSegAllocSizes;
}


