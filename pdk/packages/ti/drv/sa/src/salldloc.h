#ifndef _SALLDLOC_H
#define _SALLDLOC_H
/******************************************************************************
 * FILE PURPOSE: Local Defines for the SA LLD 
 ******************************************************************************
 * FILE NAME:   salldloc.h
 *
 * DESCRIPTION: Define private data and function prototypes for the SA LLD  
 *
 * TABS: NONE
 *
 *
 * REVISION HISTORY:
 *
 *
 * (C) Copyright 2009-2013, Texas Instruments Inc.
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
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include "src/salldport.h"
#include "src/salldctx.h"
#include "src/salldpka.h"

/* 
 * Shut off: remark #880-D: parameter "descType" was never referenced
*
* This is better than removing the argument since removal would break
* backwards compatibility
*/
#ifdef _TMS320C6X
#pragma diag_suppress 880
#pragma diag_suppress 681
#elif defined(__GNUC__)
/* Same for GCC:
* warning: unused parameter descType [-Wunused-parameter]
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
 
#define SALLD_RTP_HEADER_SIZE_IN_BYTE       (12) /* RTP Header Size without CSRC */
#define SALLD_HMAC_MAX_DIGEST_LEN_IN_TUINT  (32) /* For SHA-512 */

/* Internal Function Return Codes */
#define SALLD_NOERROR     0
#define SALLD_ERROR       1

/* Define TRUE FALSE*/
#undef TRUE
#undef FALSE
#define TRUE (uint16_t)1
#define FALSE (uint16_t)0

/* Authentication Type definitions */
#define SALLD_MAC_GENERATION            1
#define SALLD_MAC_AUTHENTICATION        2

#define SALLD_AES_BLOCK_SIZE_IN_BYTE  (16)
#define SALLD_AES_BLOCK_SIZE_IN_TUINT SALLD_BYTE_TO_TUINT(SALLD_AES_BLOCK_SIZE_IN_BYTE)
#define SALLD_AES_BLOCK_SIZE_IN_WORD  SALLD_BYTE_TO_WORD(SALLD_AES_BLOCK_SIZE_IN_BYTE)
#define SALLD_16LSB_OFFSET_IN_BYTE (14)
#define SALLD_16LSB_OFFSET_IN_WORD SALLD_BYTE_TO_WORD(SALLD_16LSB_OFFSET_IN_BYTE)
#define SALLD_MAX_ROUND_KEY_SIZE_IN_TULONG (60)

/* 
 * Snow3G and ZUC single-pass mode
 */
#define  sa_CipherMode_SNOW3G_F8F9          100
#define  sa_CipherMode_ZUC_F8F9             101
 

/* Memory Buffer related definitions */
/* SA LLD Dynamic memory buffers */
/* NOTE: The assumption we are making is that all the memory */
/*       allocation is done from HEAP !!!                    */

#define SALLD_NBUF                sa_N_BUFS   /* # of buffers in SALLD instance*/
#define SALLD_BUFN                0           /* 1st buffer is SALLD object */

#define SALLD_CHAN_NBUF           sa_CHAN_N_BUFS /* # of buffers in SALLD channel */
#define SALLD_CHAN_BUFN           0              /* 1st buffer is SALLD channel instance*/

#define SALLD_MAX_NUM_CORES       8              /* Maxmium Number of DSP cores supported */
                                                 /* TBD: find CSL definitions */
#define SALLD_MAX_SEGMENTS 		  8

#define sa_CONV_ADDR_TO_OFFSET(poolbase, base)    ((uintptr_t)base     - (uintptr_t) poolbase )
#define sa_CONV_OFFSET_TO_ADDR(poolbase, offset)  ((uintptr_t)poolbase + (uintptr_t) offset   )

/* This macro generates compilier error if postulate is false, so 
 * allows 0 overhead compile time size check.  This "works" when
 * the expression contains sizeof() which otherwise doesn't work
 * with preprocessor */
#define SA_COMPILE_TIME_SIZE_CHECK(postulate)                           \
   do {                                                                 \
       typedef struct {                                                 \
         uint8_t NegativeSizeIfPostulateFalse[((int)(postulate))*2 - 1];\
       } PostulateCheck_t;                                              \
   }                                                                    \
   while (0)

typedef struct{
    tword *segments[SALLD_MAX_SEGMENTS];
    int16_t inputLen[SALLD_MAX_SEGMENTS];
    int16_t nSegments;
    int16_t pktLen;
} salldAesDesc_t;


typedef struct aesCtrInst_s{
    uint8_t key[32];
    tword iv[SALLD_AES_BLOCK_SIZE_IN_BYTE];
    tword block[SALLD_AES_BLOCK_SIZE_IN_WORD];
    int16_t keylen;
    uint16_t num;
    int16_t nr;
    uint16_t index;
} salldAesInst_t;
/*******************************************************************************
 * DATA DEFINITION:  SALLD Software Cipher and MAC functions
 *******************************************************************************
 * DESCRIPTION:  Defines the function prototypes of the common software Cipher
 *               and MAC processing routines .
 ******************************************************************************/
typedef int16_t (*salldCoreCipher)(uint32_t *key, int16_t keylen, int16_t nr, tword *iv,
								salldAesDesc_t *desc);

typedef int16_t (*salldCoreMac)(uint16_t *key, int16_t key_len, tword *tag, int16_t tag_len, 
								salldAesDesc_t *desc, uint16_t options, tword *pad);
                              
/******************************************************************************
 * DATA DEFINITION:  SALLD Local Object structure
 ******************************************************************************
 * DESCRIPTION:  Defines the Local Object structure for SALLD instance. This
 *               structure consists of parameters used by local core
 *               
 ******************************************************************************/
typedef struct salldLocObj_s {

  uint32_t            ctrlBitfield;         /* Local instance State */
/* 
 * Bit 0: local instance initialized
 * Bit 1-31: Reserved
 */   
#define SALLD_SET_LOC_INST_READY(a,b)     UTL_SET_BITFIELD((a)->ctrlBitfield, b, 0, 1)
#define SALLD_TEST_LOC_INST_READY(a)      UTL_GET_BITFIELD((a)->ctrlBitfield, 0, 1)
  Sa_CallOutFuncs_t  callOutFuncs;          /* Call-out function table */
  void*              intBuf;                 /* Pointer to an arbitrary internal buffer of at least  @ref sa_MAX_SC_SIZE bytes 
                                                for temporary key storage during air ciphering security 
                                                context construction */                                
  void*              baseAddr;              /* Specify the SASS base address */
  void*              instPoolBaseAddr;     /** < Base address of the global shared memory pool from which global
                                                 LLD instance & channel instance memory is allocated.*/ 
  void*              scPoolBaseAddr;       /** < Base address of the global shared memory pool from which SA
                                                 security context memory is allocated. This is a DMA’able memory */
} salldLocObj_t;

/* SA Local object */
extern salldLocObj_t    salldLObj;

#define SALLD_INST_MAGIC_WORD       0xbabeface
/******************************************************************************
 * DATA DEFINITION:  SALLD Object structure
 ******************************************************************************
 * DESCRIPTION:  Defines the Master Object structure for SALLD instance.
 ******************************************************************************/
typedef struct salldObj_s {
  
  uint32_t           memBaseOffsets[SALLD_NBUF];  /* record the buffer offset information for object close operation */
  uint32_t           magic;                 /* Magic word */
  uint32_t           ID;                    /* Instance ID */
  uint32_t           stateBitfield;         /* Instance State */
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  uint32_t           engCounter[2];         /* Keeps track of engine pair utilization */
  uint32_t			 scratchAllocBitmap[64];/* Scratch memory key allocation bitmap*/
#endif  
/* Bit 0: SA Enable
   Bit 1: RNG Enable
   Bit 2: PKA Enable
   Bit 3: Second Generation SASS
   Bit 4-6: IPSEC Engine select mode
   Bit 7: Enable Level 1 debug info 
   Bit 8: Enable Level 2 Debug Info
   Bit 9-31: Reserved
*/
#define SALLD_SET_STATE_ENABLE(a,b)     UTL_SET_BITFIELD((a)->stateBitfield, b, 0, 1)
#define SALLD_TEST_STATE_ENABLE(a)      UTL_GET_BITFIELD((a)->stateBitfield, 0, 1)
#define SALLD_SET_STATE_RNG(a,b)        UTL_SET_BITFIELD((a)->stateBitfield, b, 1, 1)
#define SALLD_TEST_STATE_RNG(a)         UTL_GET_BITFIELD((a)->stateBitfield, 1, 1)
#define SALLD_SET_STATE_PKA(a,b)        UTL_SET_BITFIELD((a)->stateBitfield, b, 2, 1)
#define SALLD_TEST_STATE_PKA(a)         UTL_GET_BITFIELD((a)->stateBitfield, 2, 1)
#define SALLD_SET_SASS_GEN2(a,b)        UTL_SET_BITFIELD((a)->stateBitfield, b, 3, 1)
#define SALLD_TEST_SASS_GEN2(a)         UTL_GET_BITFIELD((a)->stateBitfield, 3, 1)
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#define SALLD_SET_STATE_ENG_SEL_MODE(a,b)  UTL_SET_BITFIELD((a)->stateBitfield, b, 4, 3)
#define SALLD_GET_STATE_ENG_SEL_MODE(a)    UTL_GET_BITFIELD((a)->stateBitfield, 4, 3)
#define SALLD_SET_STATE_DBG_L1_COLLECT(a,b) UTL_SET_BITFIELD((a)->stateBitfield, b, 7, 1)
#define SALLD_TEST_STATE_DBG_L1_COLLECT(a)  UTL_GET_BITFIELD((a)->stateBitfield, 7, 1)
#define SALLD_SET_STATE_DBG_L2_COLLECT(a,b) UTL_SET_BITFIELD((a)->stateBitfield, b, 8, 1)
#define SALLD_TEST_STATE_DBG_L2_COLLECT(a)  UTL_GET_BITFIELD((a)->stateBitfield, 8, 1)

#define SALLD_FW_CTRL_LEVEL1_DEBUG_MASK  (1 << 15)
#define SALLD_FW_CTRL_LEVEL2_DEBUG_MASK  (1 << 14)
#else
#define SALLD_SET_PKA_ACTIVE(a,b)        UTL_SET_BITFIELD((a)->stateBitfield, b, 4, 1)
#define SALLD_TEST_PKA_ACTIVE(a)         UTL_GET_BITFIELD((a)->stateBitfield, 4, 1)
#define SALLD_SET_SASS_UL_GEN2(a,b)      UTL_SET_BITFIELD((a)->stateBitfield, b, 5, 1)
#define SALLD_TEST_SASS_UL_GEN2(a)       UTL_GET_BITFIELD((a)->stateBitfield, 5, 1)
#define SALLD_SET_LIMIT_ACCESS(a,b)      UTL_SET_BITFIELD((a)->stateBitfield, b, 6, 1)
#define SALLD_TEST_LIMIT_ACCESS(a)       UTL_GET_BITFIELD((a)->stateBitfield, 6, 1)
#endif
#define SALLD_ENG_SELECT_MODE_NA        7  


  uint32_t           shadowInstOffset;     /* Shadow instance offset relative to shadow base addr */
#if defined(NSS_LITE) || defined (NSS_LITE2)
  Sa_PkaReqInfo2_t   pkaReqInfo;           /* Record the active PKA request information */
  salldPkaInst_t     pkaInst;              /* Record the active PKA instance */
#endif  
} salldObj_t;

#define SA_DBG_COLLECT_UNSPECIFIED                 -1
#define SA_DBG_COLLECT_NUM_STATUS                  15
#define SA_DBG_COLLECT_MMR_SIZE                    16
#define SA_DBG_COLLECT_CMDLBL_TBUF_SIZE           256
#define SA_DBG_COLLECT_INT_BUF_SIZE              (256 * 2)
#define SA_DBG_COLLECT_DBGREGS_SIZE               128
#define SA_DBG_COLLECT_PKT_INFO_SIZE              1024
/******************************************************************************
 * DATA DEFINITION:  SALLD Debug Info Level 1 Layout
 ******************************************************************************
 * DESCRIPTION:  Defines the Debug Info layout
 ******************************************************************************/
typedef struct salldL1DbgInfo_s {
  uint32_t   ctrl_status[SA_DBG_COLLECT_NUM_STATUS + 1];    /* pdsp control and status */
  uint32_t   dbgregs[SA_DBG_COLLECT_DBGREGS_SIZE >> 2];     /* PDSP debug register dump */
  uint32_t   cmdLblTmpBuf[SA_DBG_COLLECT_CMDLBL_TBUF_SIZE >> 2]; /* Last command label/temp buf dump */
  uint32_t   intBuf[SA_DBG_COLLECT_INT_BUF_SIZE >> 2];           /* two Internal Buffer content dump */
} salldL1DbgInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD Debug Info Layout
 ******************************************************************************
 * DESCRIPTION:  Defines the Debug Info Level 2 layout
 ******************************************************************************/
typedef struct salldL2DbgInfo_s {
  uint32_t   swInfo[2];
  uint32_t   intBufIndex;
  uint32_t   pktSize;
  uint32_t   pktInfo[3];
  uint32_t   cmdLblInfo[2];
  uint8_t    reserved[28]; 
} salldL2DbgInfo_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD Debug Info Layout
 ******************************************************************************
 * DESCRIPTION:  Defines the Debug Info layout
 ******************************************************************************/

typedef struct salldCoreDumpInfo_s { 
  uint32_t          phpVer[3];           /* PHP versions for all PDSPs */
  uint16_t          numPdsps;            /* Number of Valid PDSPs */
  uint16_t          debug_level;         /* Debug Level set */
  Sa_SysStats_t     sysStats;            /* System stats dump */ 
  uint32_t          mmr[SA_DBG_COLLECT_MMR_SIZE >> 2]; /* mmr registers */  
  salldL1DbgInfo_t  dbgLevel1[3];        /* Level 1 debug information for all PDSPs */
  salldL2DbgInfo_t  dbgLevel2[16];       /* Level 2 debug information capturing
                                            16 packets information for Air Cipher PDSP */
} salldCoreDumpInfo_t;



/******************************************************************************
 * DATA DEFINITION:  SALLD Protocol Call Table
 ******************************************************************************
 * DESCRIPTION:  Defines the protocol-specific API call table.
 ******************************************************************************/
typedef struct Sa_ProtocolCallTbl_s {
  Sa_SecProto_e secProtocolType;                                          /* Specify the protocol type */
  int16_t  (*chanInit) (void *salldInst, void *cfg);                      /* Protocol-specific ChanInit API */
  int16_t  (*chanControl) (void *salldInst, void *ctrl);                  /* Protocol-specific ChanControl API */
  int16_t  (*chanGetStats) (void *salldInst, uint16_t flags, void *stats);/* Protocol-specific ChanGetStats API */
  int16_t  (*chanSendData) (void *salldInst, void *pkt, uint16_t clear);  /* Protocol-specific ChanSendData API */
  int16_t  (*chanReceiveData) (void *salldInst, void *pkt);               /* Protocol-specific ChanReceiveData API */
  int16_t  (*chanClose) (void *salldInst);                                /* Protocol-specific ChanClose API */
  int16_t  (*chanGetSwInfo) (void *salldInst, uint16_t dir, Sa_SWInfo_t* pChanSwInfo);  /* Protocol-specific ChanGetSwInfo API */
} Sa_ProtocolCallTbl_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD Common Statistics structure
 *****************************************************************************/
typedef struct salldStats_s {
  uint32_t     replayOld;     /* Number of Out of range Packets */
                            /* (options=-v3-h) */    
  uint32_t     replayDup;     /* Number of Duplicated Packets*/
                            /* (options=-v3-h) */    
  uint32_t     authFail;      /* Number of authentication failures */
                            /* (options=-v3-h) */    
} salldComStats_t;

/******************************************************************************
 * DATA DEFINITION:  SALLD Channel Instance structure
 ******************************************************************************
 * DESCRIPTION:  Defines the Common Instance structure for SALLD channel.
 ******************************************************************************/
typedef struct salldInst_s {
  uint32_t           memBaseOffsets[SALLD_CHAN_NBUF];  /* record the buffer offsetinformation for channel close operation */
  uint32_t           magic;             /* Magic word */
  uint32_t           ID;                /* Instance ID */
  uint16_t           instSize;          /* Instance Size */  
  uint16_t           stateBitfield;     /* Instance State */
/* Bit 0: TX On
   Bit 1: RX On
   Bit 2: Reset
   Bit 3: SW only
   Bit 4-7: reserved 
   Bit 8: TX SC valid
   Bit 9: RX SC valid 
   Bit 10: TX SC Transition
   Bit 11: Rx SC Transition
   Bit 15: Channel is shared among mutiple cores
*/   
#define SALLD_SET_STATE_TX_ON(a,b)           UTL_SET_BITFIELD((a)->stateBitfield, b, 0, 1)
#define SALLD_TEST_STATE_TX_ON(a)            UTL_GET_BITFIELD((a)->stateBitfield, 0, 1)
#define SALLD_SET_STATE_RX_ON(a,b)           UTL_SET_BITFIELD((a)->stateBitfield, b, 1, 1)
#define SALLD_TEST_STATE_RX_ON(a)            UTL_GET_BITFIELD((a)->stateBitfield, 1, 1)
#define SALLD_SET_STATE_SW_ONLY(a,b)         UTL_SET_BITFIELD((a)->stateBitfield, b, 3, 1)
#define SALLD_TEST_STATE_SW_ONLY(a)          UTL_GET_BITFIELD((a)->stateBitfield, 3, 1)
#define SALLD_SET_STATE_TX_SC_VALID(a,b)     UTL_SET_BITFIELD((a)->stateBitfield, b, 8, 1)
#define SALLD_TEST_STATE_TX_SC_VALID(a)      UTL_GET_BITFIELD((a)->stateBitfield, 8, 1)
#define SALLD_SET_STATE_RX_SC_VALID(a,b)     UTL_SET_BITFIELD((a)->stateBitfield, b, 9, 1)
#define SALLD_TEST_STATE_RX_SC_VALID(a)      UTL_GET_BITFIELD((a)->stateBitfield, 9, 1)
#define SALLD_SET_STATE_TX_SC_TRANS(a,b)     UTL_SET_BITFIELD((a)->stateBitfield, b, 10, 1)
#define SALLD_TEST_STATE_TX_SC_TRANS(a)      UTL_GET_BITFIELD((a)->stateBitfield, 10, 1)
#define SALLD_SET_STATE_RX_SC_TRANS(a,b)     UTL_SET_BITFIELD((a)->stateBitfield, b, 11, 1)
#define SALLD_TEST_STATE_RX_SC_TRANS(a)      UTL_GET_BITFIELD((a)->stateBitfield, 11, 1)
#define SALLD_SET_STATE_SHARED(a,b)          UTL_SET_BITFIELD((a)->stateBitfield, b, 15, 1)
#define SALLD_TEST_STATE_SHARED(a)           UTL_GET_BITFIELD((a)->stateBitfield, 15, 1)
  uint32_t              ownerInstOffset;     /* offset to the owner instance */
  
  uint32_t              protoTblIndex;       /* protocal table index */
  uint32_t              shadowInstOffset;    /* Shadow instance offset */
  uint16_t              updateCnt;           /* Number of times that channel has been re-configured */
#define SALLD_ENG_SELECT_NA                  0xFFFF  
  uint16_t              engSelect;           /* Index of the selected IPSEC engine (0, 1) */

  salldComStats_t       stats;      /* Current SALLD Stats; this is common to SRTP/IPSEC */
} salldInst_t;

/******************************************************************************
 * Type:  SALLD Sliding Window Structure         
 ******************************************************************************
 * Description: This structure specifies the parameters of the IPSEC/SRTP 
 *              replay control blocks
 *
 * Note: Some parameters may be initialized by SW
 *****************************************************************************/
#define SA_MAX_REPLAY_WINDOW_SIZE       128
#define SA_WIN_MASK_SIZE  SALLD_DIV_ROUND_UP(SA_MAX_REPLAY_WINDOW_SIZE, 32) + 1 

typedef struct salldWindow_s
{
    uint8_t   winMaskIndex;               /* Mask index of window base */
    uint8_t   winMaskBitoff;              /* Bit offset from "index" to the window base */
    uint8_t   winSize;                    /* replay window size */
    uint8_t   reserved1;                  /* for memory alignment only */
    uint32_t  winMask[SA_WIN_MASK_SIZE];  /* Bitmask Array  */
    uint32_t  winBaseHi;                  /* Upper 32-bit of the win_base when ESN is enabled */
    uint32_t  winBase;                    /* Packet ID of window base (lowest index) */
} salldWindow_t; 

typedef uint8_t  SALLD_REPLAY_RC_T;
#define SALLD_REPLAY_RC_OK     0     /* Packet is in window and not previously seen */
#define SALLD_REPLAY_RC_DUP    1     /* Packet is in window and previously seen */
#define SALLD_REPLAY_RC_OLD    2     /* Packet is older than window */
#define SALLD_REPLAY_RC_NEW    3     /* Packet is newer than window */

extern const Sa_ProtocolCallTbl_t* salld_callTblPtr[];

/* salld.c: replay related utility functions */
void salld_replay_init( salldWindow_t* pCtl, uint32_t winBase);
SALLD_REPLAY_RC_T salld_replay_check(salldWindow_t* pCtl, uint32_t seqNum);
void salld_replay_update(salldWindow_t* pCtl, uint32_t seqNum);
SALLD_REPLAY_RC_T salld_replay_check_and_update(salldWindow_t* pCtl, uint32_t seqNum);
uint16_t salld_replay_is_all_received(salldWindow_t* pCtl, uint32_t seqNum);
void salld_set_ipv4_chksum (tword *data);
void salld_update_shadow_scInfo(Sa_ScReqInfo_t *srcInfo, Sa_ScReqInfo_t *destInfo);
void salld_update_shadow_swInfo(Sa_SWInfo_t *srcInfo, Sa_SWInfo_t *destInfo);
void salld_update_shadow_destInfo(Sa_DestInfo_t *srcInfo, Sa_DestInfo_t *destInfo);

/* salld.c: pktutl ported functions */
/******************************************************************************
 * FUNCTION PURPOSE: Read 8 bit value from 16 bit word (macro version)
 ******************************************************************************
 * DESCRIPTION: Returns 8 bit value from 16 bit word.  Assumes nothing.
 * 
 * tuint pktRead8bits_m (
 *    tword *base,       - Base of byte array
 *    tuint byteOffset); - Byte offset to read
 * 
 *****************************************************************************/
static inline tuint pktRead8bits_m (tword *base, tuint byteOffset) 
{
  char *src = (char *)base;
  char wvalue = *(src + byteOffset);
  tuint readByte = (tuint)(wvalue & 0xFF);
  return readByte;
} /* pktRead8bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Write 8 bit value into 16 bit word (macro version)
 ******************************************************************************
 * DESCRIPTION: Writes 8 bit value into 16 bit word; nothing assumed.
 * 
 * void pktWrite8bits_m (
 *    tword *base,      - Base of byte array
 *    tuint byteOffset, - byte offset to write
 *    tuint val)        - Byte in low 8 bits of val
 * 
 *****************************************************************************/
static inline void pktWrite8bits_m (tword *base, tuint byteOffset, tuint val)
{
  char *wptr = ((char *)base + byteOffset);
  *wptr = (char)(val & 0xFF);
} /* pktWrite8bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Write 16 bit value into 16 bit word (macro version)
 ******************************************************************************
 * DESCRIPTION: Writes 16 bit value into 16 bit word.  No assumptions
 * 
 * void pktWrite16bits_m (
 *    tword *base,      - Base of byte array
 *    tuint byteOffset, - byte offset to write; assumed to be even
 *    tuint val)        - 16 bit val
 * 
 *****************************************************************************/
static inline void pktWrite16bits_m (tword *base, tuint byteOffset, tuint val) 
{
  char *wptr = ((char *)base + byteOffset);

  /* Shift/mask is endian-portable, but look out for stupid compilers */
  wptr[0] = (char)(val>>8);
  wptr[1] = (char)(val & 0xff);

} /* pktWrite16bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Read 16 bit value from 16 bit word (macro version)
 ******************************************************************************
 * DESCRIPTION: Returns 16 bit value from 16 bit word.  No assumptions.
 * 
 * tuint pktRead16bits_m (
 *    tword *base,       - Base of byte array
 *    tuint byteOffset); - Byte offset to read, assumed to be even
 * 
 *****************************************************************************/
static inline tuint pktRead16bits_m (tword *base, tuint byteOffset) 
{
  char *wptr = ((char *)base + byteOffset);
  tuint ret;

  /* Shift/mask is endian-portable, but look out for stupid compilers */
  ret = (((tuint)wptr[0]) << 8) | (wptr[1] & 0xFF);

  return ret;
} /* pktRead16bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Write 32 bit value into 16 bit words (macro version)
 ******************************************************************************
 * DESCRIPTION: Writes 32 bit value into 16 bit word; No
 *              alignment assumed
 * 
 * void pktWrite32bits_m (
 *    tword *base,      - Base of byte array
 *    tuint byteOffset, - byte offset to write; assumed to be even.
 *    tulong val)       - 32 bit val
 * 
 *****************************************************************************/
static inline void pktWrite32bits_m (tword *base, tuint byteOffset, tulong val) 
{
  /* Shift/mask is endian-portable, but look out for stupid compilers */
  pktWrite16bits_m (base, byteOffset, (tuint)(val>>16));
  pktWrite16bits_m (base, byteOffset+2, (tuint)(val&0xffff));

} /* pktWrite32bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Read 32 bit value from 16 bit words (macro version)
 ******************************************************************************
 * DESCRIPTION: Read 32 bit value from 16 bit words; No
 *              alignment assumed
 * 
 * tulong pktRead32bits_m (
 *    tword *base,      - Base of byte array
 *    tuint byteOffset) - byte offset to write; assumed to be even.
 * 
 *****************************************************************************/
static inline tulong pktRead32bits_m (tword *base, tuint byteOffset) 
{
  tulong ret;

  /* Shift/mask is endian-portable, but look out for stupid compilers */
  ret = (((tulong)pktRead16bits_m (base, byteOffset)) << 16);
  ret |= (tulong)pktRead16bits_m (base, byteOffset + 2);

  return ret;
} /* pktRead32bits_m */

 /******************************************************************************
 * FUNCTION PURPOSE: Pack bytes into words
 ******************************************************************************
 * DESCRIPTION: Packs bytes (which are stored one-byte-per word in the least
 *              significant 8 bits of the words) into words in big-endian
 *              format (meaning most-significant bytes are stored in the
 *              least significant bits of the words).
 *
 * void pktPackBytesIntoWords (
 *     tword *bytes,       # Source bytes (stored in LS Bits of words)
 *     tword *words,       # Destination packed words
 *     tint nbytes,        # Number of bytes to pack
 *     tint offset )       # # of bytes in 'words' to skip over
 *                         # In 24 bit word | 0 | 1 | 2 |
 *                         
 *****************************************************************************/
static inline void pktPackBytesIntoWords(tword *bytes, tword *words, tint nbytes, tint offset) 
{
   char *dst = (char *)words;
   /* Word is currently 16-bits.Hence bytes are stored into array of 16-bit
      values,in which only the lower 8 bits have the desired byte value */
   dst += offset;
   memcpy(dst,bytes,nbytes);
} /* pktPackBytesIntoWords */

/* salldsc.c */
void salld_set_swInfo(uint16_t engID, uint16_t ctrlFlags, Sa_DestInfo_t* pDestInfo,
                      Sa_ScReqInfo_t* pScInfo, Sa_SWInfo_t*  pSwInfo, uint8_t statusWordSize);
void salld_set_swInfo2(uint16_t engID, uint16_t ctrlFlags, Sa_DestInfo_t* pDestInfo,
                      Sa_ScReqInfo_t* pScInfo, Sa_SWInfo_t*  pSwInfo, uint8_t statusWordSize);
void salld_set_sc_phpCommom(saDMAReqInfo_t*  pDmaReqInfo,  Sa_DestInfo_t*  pDestInfo,
                            uint16_t pktInfoByte, uint16_t ctxId,  tword*  ctxBuf);
void salld_set_sc_scctl(saDMAReqInfo_t*  pDmaReqInfo,  Sa_DestInfo_t*  pDestInfo,
                            uint16_t pktInfoByte, uint16_t ctxId, tword*  ctxBuf, uint32_t ctxAttr, uint8_t priv, uint8_t privId);
void salld_sc_enc_get_info(int16_t mode, uint16_t ivSize, int16_t*  pCmdlSize, 
                           int16_t* pScSize, uint8_t* pEngId, uint16_t* pfRandowIV, int fSassGen2); 
void salld_sc_auth_get_info(int16_t mode, uint16_t* useEnc, int16_t*  pCmdlSize, 
                            int16_t* pScSize, uint8_t* pEngId); 
void salld_set_sc_enc(Sa_SecProto_e protoType, uint16_t mode,  uint16_t keySize, 
                        uint16_t* key, uint8_t aadLen, uint16_t enc, tword*  ctxBuf);
void salld_set_sc_auth(uint16_t mode,  uint16_t keySize,  uint16_t* key, tword*  ctxBuf);
void  salld_set_sc_acEnc(uint16_t mode,  uint16_t keySize,  uint16_t*  key, uint16_t*  key2,
                         uint16_t enc, tword* ctxBuf, int16_t* algorithm, int fSassGen2);
void salld_set_sc_acAuth(uint16_t mode,  uint16_t keySize,  uint16_t* key, 
                         tword* ctxBuf, int16_t* algorithm, int16_t dir, int fSassGen2);
uint16_t  salld_is_sc_updated (uint8_t* scBuf);
void  salld_sc_set_wait_update (uint8_t* scBuf);
void  salld_swiz_128 (uint8_t* in, uint8_t* out, uint16_t size);
                       
/* salldtxrx.c */
void salld_send_null_pkt (Sa_ChanHandle handle, Sa_DestInfo_t* pDestInfo, 
                          Sa_SWInfo_t* pSwInfo, uint8_t flags);
#endif /* _SALLDLOC_H */
/* nothing past this point */ 
