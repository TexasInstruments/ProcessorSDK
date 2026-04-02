#ifndef _SALLDCFG_H
#define _SALLDCFG_H
/******************************************************************************
 * FILE PURPOSE: Internal definitions and prototypes for SALLD configuration.
 ******************************************************************************
 * FILE NAME:   salldcfg.h
 *
 * DESCRIPTION: Contains internal definitions and function prototypes for 
 *              SALLD test configuration
 *
 * (C) Copyright 2009-2013 Texas Instruments, Inc.
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
#include "salldsim.h"


#define SALLD_UT_CFG_INFO_INDEX_TX     0
#define SALLD_UT_CFG_INFO_INDEX_RX     1

/* Test configuration */
typedef struct {
    int16_t             chnum;         /* Specify the channel index */
    Sa_SecProto_e       protocolType;
    Sa_CipherMode_e     cipherMode;    /* Specify the cipher mode as defined at salldCipherModes_e */
    Sa_AuthMode_e       authMode;      /* Specify the authentication mode as defined at salldAuthModes_e */
    uint16_t            relayWinSize;
    salldOpenConfig_t   openCfg;
    Sa_DestInfo_t       destInfo[2];   /* Specify the post-SA destination informatio */
} salldTestConfig_t;

void salldcfg_chan_init (salldTestConfig_t* pTestCfg);

#endif  /* _SALLDCFG_H */
/* nothing past this point */
