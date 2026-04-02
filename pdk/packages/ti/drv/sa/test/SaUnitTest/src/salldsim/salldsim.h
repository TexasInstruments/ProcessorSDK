#ifndef _SALLDSIM_H
#define _SALLDSIM_H
/******************************************************************************
 * FILE PURPOSE: Definitions and prototypes for SALLD Simulation.
 ******************************************************************************
 * FILE NAME:   salldsim.h
 *
 * DESCRIPTION: Contains definitions and function prototypes for SALLD
 *              Simulation.
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

/* ANSI C headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* SALLD component and simulation headers */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>

#if defined (__aarch64__)
#include <ti/csl/arch/a53/csl_a53.h>
#endif

#ifndef USE_BIOS
#include <ti/csl/tistdtypes.h>
#endif

/* Simulation constraints */
#if defined(SAU_PRMOTE_DEMOTE_TEST)
#define salld_SIM_MAX_CHANNELS      16     /* Maximum SALLD Channels         */
#else
#define salld_SIM_MAX_CHANNELS      100    /* Maximum SALLD Channels         */
#endif
#define salld_SIM_MAX_PKTGENS       2     /* Maximum Number of Packet Gens  */
#define salld_SIM_MAX_ECOBUFS       1     /* Maximum module buffers       */
#define salld_SIM_MAX_STRING        256   /* Maximum string size          */
#define salld_SIM_MAX_NAME_STR      128   /* Maximum string size          */

#define salld_SIM_ID                0xa0
#define salld_MOD_ID                0xa1
#define SALLD_UNIT_TEST_MAX_SEGMENTS 10
/* Error identifier enumerations */
typedef enum {
  salld_SIM_ERR_NONE            = 0x0000, /* SALLD NO ERROR         */
  salld_SIM_ERR_SALLDGETNUMBUF  = 0x1000, /* Sa_chanGetNumBuf()   */  
  salld_SIM_ERR_SALLDGETBUFDEF,           /* Sa_chanGetBufDef()   */  
  salld_SIM_ERR_SALLDINIT,                /* Sa_chanInit()        */
  salld_SIM_ERR_SALLDOPEN,                /* salldChanOpen()        */
  salld_SIM_ERR_SALLDCLOSE,               /* Sa_chanClose()       */
  salld_SIM_ERR_SALLDCONTROL,             /* Sa_chanControl()     */
  salld_SIM_ERR_SALLDSENDDATA,            /* Sa_chanSendData()    */
  salld_SIM_ERR_SALLDRECEIVEDATA,         /* Sa_chanReceiveData() */
  salld_SIM_ERR_SALLDSTATS,               /* Sa_chanGetStats()    */ 
                                          /* SALLD SIM API ERRORS   */
  salld_SIM_ERR_INITCHN = 0x2000,         /* salldSim_init_chn()    */
  salld_SIM_ERR_OPENCHN,                  /* salldSim_open_chn()    */
  salld_SIM_ERR_CONFIGCHN,                /* salldSim_config_chn()  */
  salld_SIM_ERR_CLOSECHN,                 /* salldSim_close_chn()   */
  salld_SIM_ERR_NUMBUF,                   /* salld_SIM_MAX_ECOBUFS  */
  salld_SIM_ERR_GETVERSION,               /* salldSim_prnt_version()*/
  salld_SIM_ERR_SYSGETNUMBUF  = 0x3000,   /* Sa_numAlloc()          */  
  salld_SIM_ERR_SYSGETBUFDEF,             /* Sa_alloc()             */  
  salld_SIM_ERR_SYSINIT,                  /* Sa_init()              */
  salld_SIM_ERR_SYSCLOSE,                 /* Sa_free()              */
  salld_SIM_ERR_PDSPDNLD                  /* salld_download_pdsp_images()      */
} salldSimErr_e;

/* Simulation error indication  */
typedef enum {
  salld_SIM_NOERR,                        /* Simulation function pass         */
  salld_SIM_ERR                           /* Simulation function failure      */
} salldErr_e;

/* Simulation Channel Direction */
enum {
  salld_SIM_RX,                           /* Rx (From NetWork)       */
  salld_SIM_TX                            /* Tx (To Network)         */
};

/* Simulation Output file enumeration */
enum {
  salld_SIM_FILE_PKTIN,                   /* Packet In file            */
  salld_SIM_FILE_PKTOUT,                  /* Packet Out file           */
  salld_SIM_FILE_SASC,                    /* SA Security Context file  */
  salld_SIM_FILE_SAINFO,                  /* SA Info file              */
  salld_SIM_FILE_SACMDLB,                 /* SA Command Label file     */
  salld_SIM_NUM_OUTFILES                  /* Size of output            */            
};

/* SALLD file output structure */
typedef struct {       
  bool      output_data;
  FILE      *fptr;                         /* File pointer for read/write      */
  char      fname[salld_SIM_MAX_NAME_STR]; /* File name for output data        */
  void      (*out_fcn) (void *fout);       /* output fcn (READ ONLY)           */
} salldSimFout_t;  

extern salldSimFout_t outFile[];

#define SALLD_PGN_INDEX_NONE    0xFFFF

/*
 * Define the common configuraion and control information for Tx/Rx operation 
 */                     
typedef struct salldSimChanInfo_s
{
    
    Sa_GenConfigParams_t      genCfg;
    Sa_ProtocolKeyParams_t    keyCfg;
    Sa_KeyRequest_t           keyReq;
    uint16_t                  pgnIndex; 
}   salldSimChanInfo_t;

/* State BITMAP definitions */
#define salld_SIM_STATE_TX_ON         sa_CONTROLINFO_CTRL_TX_ON       /* 0x0001 */
#define salld_SIM_STATE_RX_ON         sa_CONTROLINFO_CTRL_RX_ON       /* 0x0002 */
#define salld_SIM_STATE_SW_ONLY       sa_CONTROLINFO_CTRL_SW_ONLY     /* 0x0008 */
#define salld_SIM_STATE_TX_REKEY      0x1000                          /* Tx Re-key Pending */   
#define salld_SIM_STATE_RX_REKEY      0x2000                          /* Rx Re-key Pending */ 

#define SALLD_STATE_IDLE        0
#define SALLD_STATE_ACTIVE      1
 
/* Global SALLD channel instance structure */
typedef struct {

    uint16_t          ID;
    uint16_t          state;
    
    /* SALLD elements */
    Sa_ChanHandle    salldInst;        /* SALLD instance pointer */
    int16_t          salld_heap_size;  /* Heap allocation for FIU */
    int              buf_sizes[sa_CHAN_N_BUFS];
    
    /* Other elements */
    int16_t          aux_heap_size;    /* Auxiliary heap allocation */
    
    /* record the Sc Info */
    int16_t          scInfoIndex;      /* the current index of the scInfo */
    Sa_ScReqInfo_t   scInfo[4];
    
    /* Channel Information */
    uint16_t            stateBitMap;
    Sa_SecProto_e       protocolType;
    uint16_t            relayWinSize;
    salldSimChanInfo_t  txInfo;        /* Record Tx Channel Information */
    salldSimChanInfo_t  rxInfo;        /* Record Rx Channel Information */
    Sa_SWInfo_t         regSwInfo; 
    
    /* Channel Statistics */
    Sa_Stats_t salldsimSalldStats;
    
    /* Connection related parameters */
    void*  pConnHdl;
    
} salldSimChannel_t;

extern int numSalldSimChans;

/*
 * Assume that all Salld channels be requested sequentially
 * and will last the entire test.
 * All the buffers will be initialized prior to each test.
 * Use simple MACROs for handle allocations
 */
#define SALLD_ALLOC_CHAN(tf)        (numSalldSimChans < salld_SIM_MAX_CHANNELS)?      \
                                     &(tf)->salldSimChn[numSalldSimChans++]:NULL



/* SALLD Open Channel configuration structure */
typedef struct {
    uint16_t  ctrlBitMap;           /* Control Bit Map as defined at SALLDCtrlInfoCtrlBit at salld.h */
} salldOpenConfig_t;


/* Simulation configuration structure */    
typedef struct {                            
  /* Automation configuration booleans */
  bool verbose_print;                      /* TRUE: Provide progress info       */
} simConfig_t;

extern void*     salldSimSegments[SALLD_UNIT_TEST_MAX_SEGMENTS];
extern uint16_t  salldSimSegUsedSizes[SALLD_UNIT_TEST_MAX_SEGMENTS];
extern uint16_t  salldSimSegAllocSizes[SALLD_UNIT_TEST_MAX_SEGMENTS];

/* Callout APIs */
void   salldsimFatalError (uint16_t id, char *str);
void   salldsim_init      (int16_t chnum);
void   salldsimDebugInfo  (void * moduleID, uint16_t msgType, uint16_t messageCode, 
                           uint16_t msgLength,  uint16_t *supportingData);
void   salldsimChanKeyRequest(Sa_ChanHandle handle, Sa_KeyRequest_t* keyReq);
void   salldsimScAlloc (Sa_ChanHandle handle, Sa_ScReqInfo_t* scReqInfo);
void   salldsimScFree (Sa_ChanHandle handle, uint16_t scID);
void   salldsimChanRegister (Sa_ChanHandle handle, Sa_SWInfo_t* chanSwInfo);
void   salldsimChanUnRegister (Sa_ChanHandle handle, Sa_SWInfo_t* chanSwInfo);
void   salldsimChanSendNullPkt (Sa_ChanHandle handle, Sa_PktInfo_t *pktInfo);

void mdebugHaltSaPdsp (int pdspNum);
void salld_sim_halt (void);

/* SALLD Channel Function Prototypes */
uint16_t salldSim_init_chn (salldSimChannel_t *pChan);
uint16_t salldSim_open_chn (salldSimChannel_t *pChan, salldOpenConfig_t *cfg);
uint16_t salldSim_close_chn (salldSimChannel_t *pChan);
uint16_t salldSim_get_stats (salldSimChannel_t *pChan);
void salldSim_get_sys_stats (void);
uint16_t salldSim_poll_chn (salldSimChannel_t *pChan);
void salldSim_close_all_chns (void);
void salldSim_get_all_chan_stats (void);


/* SALLD Simulation internal API functions ("salld_sim_") */
void salld_sim_set_defaults (void);
void salld_sim_init_sa      (uint8_t limitAccess);
void salld_sim_close_sa     (void);
void salld_sim_initialize   (uint8_t limitAccess);
void salld_sim_check_rc     (salldSimErr_e id, uint16_t rc, uint16_t pass_code);
void salld_sim_close_fout   (void);
void salld_sim_close_fpgn   (void);
void salld_sim_print        (const char* fmt, ...);
void salld_sim_sprint       (char *str, char *svar);
void salld_sim_iprint       (char *str, int32_t ivar);
void salld_sim_disp_system_stats(Sa_SysStats_t *pStats);
void salld_sim_disp_chan_stats(int16_t chanNum, Sa_SecProto_e protocolType, Sa_Stats_t *pStats);

/* SALLD Simulation Utility Functions */
bool salld_sim_alloc_sc(uint16_t scSize, uint16_t* p_scID, uint8_t** pp_scBuf);
void salld_sim_free_sc(uint16_t scID);
void salld_sim_init_sc(void);
bool salld_sim_htob(char* hex_in, uint8_t* bin_out, uint16_t *pSize);
void salld_sim_init_pktdesc(Sa_PktDesc_t *pktDesc, int32_t numSegments);
void salld_sim_disp_control (bool enable);
uintptr_t salld_sim_malloc (uint32_t num_bytes, int alignmnet);
void salld_sim_free (void* ptr, uint32_t size);

/*-----------------------------------------------------------------
 * Function:  salld_sim_banner_print
 *
 * Description: Utility to provide major progress information.
 *-----------------------------------------------------------------*/
static inline void salld_sim_banner_print (char *str)
{
  salld_sim_print ("\n");
  salld_sim_print ("------------------------------\n");
  salld_sim_print (str);
  salld_sim_print ("------------------------------\n");
  salld_sim_print ("\n");
}

/*-----------------------------------------------------------------
 * Function:  salld_sim_banner_iprint
 *
 * Description: Utility to provide major progress information.
 *-----------------------------------------------------------------*/
static inline void salld_sim_banner_iprint (char *str, int32_t ivar)
{
  salld_sim_print  ("\n");
  salld_sim_print  ("------------------------------\n");
  salld_sim_iprint (str, ivar);
  salld_sim_print  ("------------------------------\n");
  salld_sim_print  ("\n");
}


/*-----------------------------------------------------------------
 * Macro:  salld_SIM_CHECK_ENG
 *
 * Description: Utility to check Engine validity.
 *-----------------------------------------------------------------*/
#define salld_SIM_CHECK_NULL_PTR(p) {   \
  if ((p) == (void *) NULL) {         \
    return (salld_SIM_ERR);             \
  }                                   \
}

/*-----------------------------------------------------------------
 * Macro:  salld_SIM_PRINT_DONE
 *
 * Description: Utility to print completion of a task.
 *-----------------------------------------------------------------*/
#define salld_SIM_PRINT_DONE() { salld_sim_print (" DONE\n\n"); }


#endif /* _SALLDSIM_H */

/* Nothing past this point */
