/*
 *
 * Copyright (C) 2010-2015 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
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

#ifndef _UNITTEST_H
#define _UNITTEST_H

#ifndef __LINUX_USER_SPACE
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/hal/Hwi.h>
#endif

#include <ti/drv/pa/pa.h>
#include <ti/drv/pa/pasahost.h>   
#include <ti/drv/pa/nss_if.h>   
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/cppi_desc.h>
#ifndef __LINUX_USER_SPACE
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_xmcAux.h>
#endif
#include <ti/csl/csl_qm_queue.h>

#include <string.h>
#include "salldsim/salldsim.h"

#ifdef __LINUX_USER_SPACE
#include <semaphore.h>
#include "fw_test.h"
#include "sockutils.h"
/* RM Includes */
#include <ti/drv/rm/rm_server_if.h>
#endif

extern uint32_t no_bootMode;

#ifndef SIMULATOR_SUPPORT
extern int setupRm (void);
#ifdef __LINUX_USER_SPACE
/* Linux Specific global variables per process */
extern sock_h                      rmClientSocket;
extern sem_t                       mutex;
extern Rm_ServiceHandle   *rmClientServiceHandle;
#else
extern Rm_Handle          rmHandle;
extern Rm_ServiceHandle  *rmServiceHandle;
#endif /* __LINUX_USER_SPACE */
#endif /* SIMULATOR_SUPPORT */

/* Due to a bug in the simulator all packet lengths must be rounded up to 4 byte multiples */
#define PA_SIM_BUG_4BYTES

typedef enum  {
	SA_TEST_FAILED  = -1,
	SA_TEST_NOT_RUN,
	SA_TEST_PASSED
} saTestStatus_t;


/* Define the test interface */
typedef struct saTest_s
{
#ifndef __LINUX_USER_SPACE
	void (*testFunction)(UArg a0, UArg a1);   /* The code that runs the test */
#else
	void* (*testFunction)(void *);   /* The code that runs the test */
#endif
	char *name;						/* The test name */
	saTestStatus_t testStatus;		/* Test status */
	
} saTest_t;

/* Cache Line Size */
#ifndef __LINUX_USER_SPACE
#define CACHE_LINESZ 128
#endif
#define SYS_ROUND_UP(x,y)   (((x) + ((y) -1))/(y)*(y))

/**********************************************************************
 ****************************** Defines *******************************
 **********************************************************************/
#define     MAX_NUM_CORES       sa_MC_EXAMPLE_NUM_CORES

/* Hardware Semaphore to synchronize access from
 * multiple applications (PA applications and non-PASS applications)
 * across different cores to the QMSS library.
 */
#define     QMSS_HW_SEM         3

/* Hardware Semaphore to synchronize access from
 * multiple applications (PASS applications and non-PASS applications)
 * across different cores to the CPPI library.
 */
#define     CPPI_HW_SEM         4

/* Hardware Semaphore to synchronize access from
 * multiple applications (PASS applications and non-PASS applications)
 * across different cores to the PA library.
 */
#define     PA_HW_SEM           5

/* Hardware Semaphore to synchronize access from
 * multiple applications (PASS/SASS applications and non-PASS/SASS applications)
 * across different cores in this application.
 */
#define     SA_HW_SEM           6


#define TF_NUM_PASS_PDSPS                 NSS_PA_NUM_PDSPS 
#define TF_NUM_SASS_PDSPS                 NSS_SA_NUM_PDSPS 

#define TF_PA_NUM_RX_CPDMA_CHANNELS       NSS_NUM_RX_PKTDMA_CHANNELS    
#define TF_PA_NUM_TX_CPDMA_CHANNELS       NSS_NUM_TX_PKTDMA_CHANNELS 

#define TF_PA_TX_QUEUE_BASE               QMSS_PASS_QUEUE_BASE

/* PA Tx Queues */
#define TF_PA_QUEUE_INPUT                 (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_INPUT_INDEX)
#define TF_PA_QUEUE_MAC                   (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_MAC_INDEX)
#define TF_PA_QUEUE_OUTER_IP              (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_OUTER_IP_INDEX)
#define TF_PA_QUEUE_INNER_IP              (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_INNER_IP_INDEX)
#define TF_PA_QUEUE_L4                    (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_LUT2_INDEX)
#define TF_PA_QUEUE_IPSEC                 (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_IPSEC_INDEX)
#define TF_PA_QUEUE_IPSEC2                (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_IPSEC2_INDEX)
#define TF_PA_QUEUE_POST                  (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_POST_INDEX)
#define TF_PA_QUEUE_TXCMD                 (TF_PA_TX_QUEUE_BASE + NSS_PA_QUEUE_TXCMD_INDEX)

#define TF_PA_QUEUE_SASS                  (TF_PA_TX_QUEUE_BASE + NSS_SA_QUEUE_SASS_INDEX)
#define TF_PA_QUEUE_SASS2                 (TF_PA_TX_QUEUE_BASE + NSS_SA_QUEUE_SASS2_INDEX)
#define TF_PA_QUEUE_ETH                   (TF_PA_TX_QUEUE_BASE + NSS_CPSW_QUEUE_ETH_INDEX)

/* SA (CP_ACE) queues */
#define TF_SA_QUEUE_0                     TF_PA_QUEUE_SASS
#define TF_SA_QUEUE_1                     TF_PA_QUEUE_SASS2

/* ENET queues */
#define TF_ENET_QUEUE_0                   TF_PA_QUEUE_ETH

/* PA Tx Queue Indexes */
#define TF_PA_Q_INPUT                     (NSS_PA_QUEUE_INPUT_INDEX)
#define TF_PA_Q_OUTER_IP                  (NSS_PA_QUEUE_OUTER_IP_INDEX)
#define TF_PA_Q_INNER_IP                  (NSS_PA_QUEUE_INNER_IP_INDEX)
#define TF_PA_Q_L4                        (NSS_PA_QUEUE_LUT2_INDEX)
#define TF_PA_Q_POST                      (NSS_PA_QUEUE_POST_INDEX)
#define TF_PA_Q_TXCMD                     (NSS_PA_QUEUE_TXCMD_INDEX)
#define TF_PA_Q_EMAC                      (NSS_CPSW_QUEUE_ETH_INDEX)

#define TF_PA_TX_QUEUE_INDEX_SA0          (NSS_SA_QUEUE_SASS_INDEX)
#define TF_PA_TX_QUEUE_INDEX_SA1          (NSS_SA_QUEUE_SASS2_INDEX)

#define TF_PA_Q_CONFIG_BASE               NSS_PA_QUEUE_INPUT_INDEX      /* To be deleted */

#ifndef NSS_GEN2 

/* TF init queue number */
#define TF_QUEUE_NUM_INIT                   900

#else  /* NSS_GEN2 */

/* TF init queue number */
#define TF_QUEUE_NUM_INIT                   1024

#endif

#define TF_Q_FREE_DESC						(TF_QUEUE_NUM_INIT)         /* Unassociated descriptor queue number */
#define TF_TEARDOWN_QUE_MGR  				(TF_QUEUE_NUM_INIT + 1)     /* Teardown queue number */
#define TF_TEARDOWN_QUE_NUM  				  0

#define TF_LINKED_BUF_Q1           			(TF_QUEUE_NUM_INIT + 2)     /* First queue with attached buffers (for PA commands as well) */
#define TF_LINKED_BUF_Q1_BUF_SIZE  			128
#define TF_LINKED_BUF_Q1_NBUFS       		 32
extern unsigned char memQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE];

#define TF_LINKED_BUF_Q2		   			(TF_QUEUE_NUM_INIT + 3)		/* Second queue with attached buffers */
#define TF_LINKED_BUF_Q2_BUF_SIZE  			256
#define TF_LINKED_BUF_Q2_NBUFS				 16
extern unsigned char memQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE];

#define TF_LINKED_BUF_Q3		   			(TF_QUEUE_NUM_INIT + 4)
#define TF_LINKED_BUF_Q3_BUF_SIZE  			512
#define TF_LINKED_BUF_Q3_NBUFS      		  8
extern unsigned char memQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE];

#define TF_LINKED_BUF_Q4		   			(TF_QUEUE_NUM_INIT + 5)
#define TF_LINKED_BUF_Q4_BUF_SIZE  		   1024
#define TF_LINKED_BUF_Q4_NBUFS      		  8
extern unsigned char memQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE];

#define TF_FIRST_GEN_QUEUE		   			(TF_QUEUE_NUM_INIT + 10)	/* Queues available for general use */
#define TF_NUM_GEN_QUEUES		   			100

 /* General purpose queue usage */
 #define Q_CMD_RECYCLE		  0		/* Command descriptors/buffers recycled here after sent to PA */
 #define Q_MATCH		  	  1		/* Packets from PA which match a lookup criteria */
 #define Q_NFAIL		      2		/* Packets from PA which matches a mac lookup, but failed an L3 lookup */
 #define Q_PARSE_ERR		  3		/* Packets which resulted in a parse error */
 #define Q_MULTI_0			  4		/* Multi route queue 0 */
 #define Q_MULTI_1			  5	    /* Multi route queue 1 */
 #define Q_MULTI_2			  6		/* Multi route queue 2 */
 #define Q_MULTI_3			  7		/* Multi route queue 3 */
 #define Q_MULTI_4			  8		/* Multi route queue 4 */
 #define Q_MULTI_5			  9		/* Multi route queue 5 */
 #define Q_MULTI_6           10		/* Multi route queue 6 */
 #define Q_MULTI_7			 11		/* Multi route queue 7 */

 #define TF_FIRST_CMD_REPLY_Q 12	   /* Replies from PA routed here */
 #define Q_CMD_REPLY  		 TF_FIRST_CMD_REPLY_Q		/* Replies from PA routed here */
 
 #define Q_DATA_RECYCLE		 20	    /* DATA Buffer Recycle */
 
 #define Q_REQ_STATS_RECYCLE 30
 #define Q_STATS_REPLY		 31
 
 #define Q_PKT_RECV          40     /* The first Packet Receive Queue */
 #define DEST_QUEUE_PKT_RECV        (TF_FIRST_GEN_QUEUE + Q_PKT_RECV)

/* The number of PA handles maintained by this test */
#define TF_NUM_LOCAL_HANDLES	64

 /* Commands to the PA are verified through the value in swinfo0.
  * The 16 ms bits are used as verification, the 16 lbs are for local handle id */
#define TF_CMD_SWINFO0_ADD_ID  		0x11110000  /* Identifies add mac command */
#define TF_CMD_SWINFO0_DEL_ID  		0x22220000  /* Identifies del mac command */
#define TF_CMD_SWINFO0_STATS_REQ_ID	0x33330000	/* Identifies the req stats command */
#define TF_CMD_SWINFO0_PKT_ID		0x55550000  /* Identifies the packet as a data packet */

#define TF_CACHE_LINESZ    128
#define TF_ROUND_UP(x, y)   (((x) + ((y)-1))/(y)*(y))

/* SA related definitions */
#define SALLDSIM_SC_BUF_SIZE    TF_ROUND_UP(sa_MAX_SC_SIZE, TF_CACHE_LINESZ)  /* 448 or 512 */
#define SALLDSIM_NUM_SC_BUF     256     /* Multiple of 16 */ 

extern uint8_t (*salldsimScBuf)[SALLDSIM_SC_BUF_SIZE];

/* Define the test framework */
typedef struct tFramework_s  {
	
	Pa_Handle       passHandle;	/* PA instance handle */
	Cppi_Handle tfPaCppiHandle; /* PA CDMA handle */
	Cppi_ChHnd  tfPaTxChHnd[TF_PA_NUM_TX_CPDMA_CHANNELS];
	Cppi_ChHnd  tfPaRxChHnd[TF_PA_NUM_RX_CPDMA_CHANNELS];
	
	/* Queues */
	int32_t QPaTx[TF_PA_NUM_TX_CPDMA_CHANNELS];
	int32_t QfreeDesc;		      				/* Unassociated descriptor queue handle */
	int32_t QtDown;								/* Tear down queue handle */
	int32_t QLinkedBuf1;							/* First Queue with descriptors and attached linked buffers */
	int32_t QLinkedBuf2;							/* Second Queue with descriptors and attached linked buffers */
	int32_t QLinkedBuf3;							/* Third Queue with descriptors and attached linked buffers */
	int32_t QLinkedBuf4;							/* Fourth Queue with descriptors and attached linked buffers */
	int32_t QGen[TF_NUM_GEN_QUEUES];				/* General purpose queues */
	
	Cppi_FlowHnd tfPaFlowHnd;					/* Flow handle */
	int32_t		 tfFlowNum;						/* Physical flow number */
	
    /* SA test related variables */
    Sa_Handle   salld_handle;
    int         salld_buf_sizes[sa_N_BUFS];
    
    /* Control flags */
    Bool  fNullEnc;     /* TRUE: Null encryption/authentication */
    
    /* Output files */
    FILE* fp_ctx;       /* context output file */
    FILE* fp_pktIn;     /* record packets which enter PA/SA */
    FILE* fp_pktOut;    /* record packets from PA/SA */

    /* record the current test */
	saTest_t* pTest;

}tFramework_t;

/* Number of cores */
#define  sa_MC_EXAMPLE_NUM_CORES   4

/* System Init Core
 */
#define SYSINIT       0

/* PA memory */
#define TF_PA_INST_SIZE	  400				/* Required size = 320 */
//extern uint8_t memPaInst[TF_PA_INST_SIZE];

#define TF_MAX_NUM_L2_HANDLES	64
#define TF_L2_TABLE_SIZE	(TF_MAX_NUM_L2_HANDLES * 36)	/* Requires 36 bytes per entry */
//extern uint8_t memL2Ram[TF_L2_TABLE_SIZE];

#define TF_MAX_NUM_L3_HANDLES	128
#define TF_L3_TABLE_SIZE	(TF_MAX_NUM_L3_HANDLES * 76)	/* Requires 76 bytes per entry */
//extern uint8_t memL3Ram[TF_L3_TABLE_SIZE];

extern tFramework_t tFramework;

/* Define the arguments to unit test functions */
typedef struct saTestArgs_s
{
    tFramework_t *tf;
    saTest_t     *pat;
} saTestArgs_t;
	
/* QM memory */	
#define TF_NUM_DESC		 128		/* 128 host descriptors managed by the QM */
#define TF_SIZE_DESC     128		/* Must be multiple of 16 */

/* Memory used for the descriptor RAM */
#ifndef __LINUX_USER_SPACE
extern uint8_t memDescRam[TF_NUM_DESC * TF_SIZE_DESC];	
#endif

typedef uint32_t  paStatsBmap_t; 

/* Stats numbers. Util functions are used to add these to the bit maps.
 * The values match the actual stats numbers */
#define TF_STATS_BM_C1_NUM_PACKETS		0
#define TF_STATS_BM_C1_TABLE_MATCH   	6
#define TF_STATS_BM_C1_NO_TABLE_MATCH   7
#define TF_STATS_BM_C1_VLAN_OVERFLOW    10
#define TF_STATS_BM_C1_NUM_MPLS			12
#define TF_STATS_BM_C1_PARSE_FAIL		13
#define TF_STATS_BM_C1_SILENT_DISCARD   17

/* Packet and associated structures */
typedef struct pktTestInfo_s  {
	uint8_t 		     *pkt;
	pasahoLongInfo_t *info;
	int			      pktLen;
	paStatsBmap_t     statsMap[3];  /* Bit map of which stats to increment. Some stats must be incremented 3 times */
	int    		      idx;		    /* Used to increment the test tracking - tells the test to look for 
						             * a packet with this index */
} pktTestInfo_t;			

/* 32 L2 handles are managed. This structure is used to track the handle and
 * the activation state state of the handle */
enum  {
  TF_L2_HANDLE_UNCONFIGURED = 0,
  TF_L2_HANDLE_PENDING_ACK,
  TF_L2_HANDLE_ACTIVE,
  TF_L2_HANDLE_DISABLED
};

typedef struct t2Handles_s  {

  paHandleL2L3_t  paHandle;     /* The handle returned by the PA LLD */

  uint32_t			state;		  /* TF_L2_HANDLE_UNCONFIGURED = handle not configured
							     * TF_L2_HANDLE_PENDING_ACK = handle configured and sent to pa
							     * TF_L2_HANDLE_ACTIVE = handle creation acknowledged by pa */

} t2Handles_t;

typedef struct t4Handles_s  {

  paHandleL4_t    paHandle;     /* The handle returned by the PA LLD */

  uint32_t			state;		  /* TF_L2_HANDLE_UNCONFIGURED = handle not configured
							     * TF_L2_HANDLE_PENDING_ACK = handle configured and sent to pa
							     * TF_L2_HANDLE_ACTIVE = handle creation acknowledged by pa */

} t4Handles_t;

typedef struct testPktDesc_s  {
    Bool    tx;                 /* 0: Tx Packet 1:Rx Packet */
    uint16_t  chnum;              /* Channel Index */
    uint16_t  size;               /* To be enhanced to include payload information */
    uint16_t  payloadOffset;
    uint16_t  payloadLen;
    uint8_t*  pkt;                /* Pointer to the packet */
} testPktDesc_t;

#define SA_TEST_MAX_L2_HANDLES   2     /* Max number of L2 Handles */
#define SA_TEST_MAX_L3_HANDLES   2     /* Max number of L3 Handles */
#define SA_TEST_MAX_L4_HANDLES   2     /* Max number of L4 Handles */

extern t4Handles_t     l4Handles[SA_TEST_MAX_L4_HANDLES];
extern int             numL4Handles;
#define TEST_ALLOC_L4_HANDLE()     (numL4Handles < SA_TEST_MAX_L4_HANDLES)?      \
                                   &l4Handles[numL4Handles++]:NULL

extern uint32_t                coreNum;

/* Miscellaneous Macros */
#define MK_UINT16(a, b)             (((a) << 8) | (b))

#ifndef __LINUX_USER_SPACE
#undef L2_CACHE
#ifdef L2_CACHE

#define SYS_CACHE_LINE_SIZE             128

    /* Invalidate L2 cache. This should invalidate L1D as well. 
     * Wait until operation is complete. */    
#define SYS_CACHE_INV(addr, size, code)    CACHE_invL2 (addr, (size) + ((uint32_t)(addr) & (SYS_CACHE_LINE_SIZE - 1)), code)

    /* Writeback L2 cache. This should Writeback L1D as well. 
     * Wait until operation is complete. */ 
#define SYS_CACHE_WB(addr, size, code)     CACHE_wbL2 (addr, (size) + ((uint32_t)(addr) & (SYS_CACHE_LINE_SIZE - 1)), code)


#else       

#define SYS_CACHE_LINE_SIZE              64

    /* Invalidate L1D cache and wait until operation is complete. 
     * Use this approach if L2 cache is not enabled */    
#define SYS_CACHE_INV(addr, size, code)    CACHE_invL1d (addr, (size) + ((uint32_t)(addr) & (SYS_CACHE_LINE_SIZE - 1)), code)
    /* Writeback L1D cache and wait until operation is complete. 
     * Use this approach if L2 cache is not enabled */    
#define SYS_CACHE_WB(addr, size, code)     CACHE_wbL1d (addr, (size) + ((uint32_t)(addr) & (SYS_CACHE_LINE_SIZE - 1)), code)

#define SYS_CACHE_INV1(addr, size, code)    CACHE_invL1d (addr, (size), code)
#define SYS_CACHE_WB1(addr, size, code)     CACHE_wbL1d (addr, (size), code)


#endif  /* L2_CACHE */
#endif
/* Prototypes */

/* salld.c: pktutl ported functions */
/******************************************************************************
 * FUNCTION PURPOSE: Read 8 bit value from 16 bit word (macro version)
 ******************************************************************************
 * DESCRIPTION: Returns 8 bit value from 16 bit word.  Assumes nothing.
 *
 * uint16_t pktRead8bits_m (
 *    uint8_t *base,       - Base of byte array
 *    uint16_t byteOffset); - Byte offset to read
 *
 *****************************************************************************/
static inline uint16_t pktRead8bits_m (uint8_t *base, uint16_t byteOffset)
{
  char *src = (char *)base;
  char wvalue = *(src + byteOffset);
  uint16_t readByte = (uint16_t)(wvalue & 0xFF);
  return readByte;
} /* pktRead8bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Write 8 bit value into 16 bit word (macro version)
 ******************************************************************************
 * DESCRIPTION: Writes 8 bit value into 16 bit word; nothing assumed.
 *
 * void pktWrite8bits_m (
 *    uint8_t *base,      - Base of byte array
 *    uint16_t byteOffset, - byte offset to write
 *    uint16_t val)        - Byte in low 8 bits of val
 *
 *****************************************************************************/
static inline void pktWrite8bits_m (uint8_t *base, uint16_t byteOffset, uint16_t val)
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
 *    uint8_t *base,      - Base of byte array
 *    uint16_t byteOffset, - byte offset to write; assumed to be even
 *    uint16_t val)        - 16 bit val
 *
 *****************************************************************************/
static inline void pktWrite16bits_m (uint8_t *base, uint16_t byteOffset, uint16_t val)
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
 * uint16_t pktRead16bits_m (
 *    uint8_t *base,       - Base of byte array
 *    uint16_t byteOffset); - Byte offset to read, assumed to be even
 *
 *****************************************************************************/
static inline uint16_t pktRead16bits_m (uint8_t *base, uint16_t byteOffset)
{
  char *wptr = ((char *)base + byteOffset);
  uint16_t ret;

  /* Shift/mask is endian-portable, but look out for stupid compilers */
  ret = (((uint16_t)wptr[0]) << 8) | (wptr[1] & 0xFF);

  return ret;
} /* pktRead16bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Write 32 bit value into 16 bit words (macro version)
 ******************************************************************************
 * DESCRIPTION: Writes 32 bit value into 16 bit word; No
 *              alignment assumed
 *
 * void pktWrite32bits_m (
 *    uint8_t *base,      - Base of byte array
 *    uint16_t byteOffset, - byte offset to write; assumed to be even.
 *    uint32_t val)       - 32 bit val
 *
 *****************************************************************************/
static inline void pktWrite32bits_m (uint8_t *base, uint16_t byteOffset, uint32_t val)
{
  /* Shift/mask is endian-portable, but look out for stupid compilers */
  pktWrite16bits_m (base, byteOffset, (uint16_t)(val>>16));
  pktWrite16bits_m (base, byteOffset+2, (uint16_t)(val&0xffff));

} /* pktWrite32bits_m */

/******************************************************************************
 * FUNCTION PURPOSE: Read 32 bit value from 16 bit words (macro version)
 ******************************************************************************
 * DESCRIPTION: Read 32 bit value from 16 bit words; No
 *              alignment assumed
 *
 * uint32_t pktRead32bits_m (
 *    uint8_t *base,      - Base of byte array
 *    uint16_t byteOffset) - byte offset to write; assumed to be even.
 *
 *****************************************************************************/
static inline uint32_t pktRead32bits_m (uint8_t *base, uint16_t byteOffset)
{
  uint32_t ret;

  /* Shift/mask is endian-portable, but look out for stupid compilers */
  ret = (((uint32_t)pktRead16bits_m (base, byteOffset)) << 16);
  ret |= (uint32_t)pktRead16bits_m (base, byteOffset + 2);

  return ret;
} /* pktRead32bits_m */

/* Other functions */
int setupTestFramework (void);
int setupTestFrameworkLocal (void);
int exitTestFramework(void);
int exitTestFrameworkLocal(void);
int verifyTestFramework (void);
void saTestRecoverAndExit (tFramework_t *tf, saTest_t *pat, saTestStatus_t testResult);
void utilCycleDelay (int32_t iCount);
uint32_t utilgAddr(uint32_t x);
uint16_t utilOnesCompAdd (uint16_t value1, uint16_t value2);
uint16_t utilOnesCompChkSum ( uint16_t *p_data, uint16_t len );
void utilSetIpv4ChkSum (uint8_t *data);
uint16_t utilGetIpv4PsudoChkSum (uint8_t *data, uint16_t payloadLen);
uint16_t utilGetIpv6PsudoChkSum (uint8_t *data, uint16_t payloadLen);
void utilSetUdpChkSum (uint8_t *data, uint16_t len, uint16_t psudoChkSum);
void utlOutputSc (tFramework_t *tf,  uint16_t scSize, uint16_t scID, uint8_t* scBuf);

Cppi_HostDesc*  testCommonGetBuffer(tFramework_t *tf, int size);
int testCommonRequestPaStats (char *fname, tFramework_t *tf, Bool reset, int32_t QSource, int32_t QRecycle,  paCmdReply_t *reply);
int testCommonCompareStats (char *fname, paSysStats_t *expected, paSysStats_t *actual);
int testCommonRecycleLBDesc (tFramework_t *tf, Cppi_HostDesc *hd);
int testCommonComparePktInfo (char *tfName, pasahoLongInfo_t *expected, pasahoLongInfo_t *actual);
void testCommonIncStats (paStatsBmap_t *map,  paSysStats_t *stats);
Bool testCommonReqAndChkPaStats (tFramework_t *tf, saTest_t *pat, paSysStats_t *pExpectedStats);

Bool testCommonAddMac (tFramework_t *tf, saTest_t *pat, paEthInfo_t *ethInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                   paHandleL2L3_t *pl2handle, int32_t QCmdMem,  paCmdReply_t *repInfo);
Bool testCommonAddIp  (tFramework_t *tf, saTest_t *pat, paIpInfo_t *ipInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                   paHandleL2L3_t l2handle, paHandleL2L3_t *pl3handle, int32_t QCmdMem, paCmdReply_t *repInfo);
Bool testCommonAddUdp (tFramework_t *tf, saTest_t *pat, uint16_t port, paRouteInfo_t *routeInfo,  
 	                   paHandleL2L3_t l3handle, paHandleL4_t l4handle, int32_t QCmdMem, paCmdReply_t *repInfo);
Bool testCommonDelHandle (tFramework_t *tf, saTest_t *pat, paHandleL2L3_t *paHdl, int QCmdMem, paCmdReply_t *cmdReply);
Bool testCommonDelL4Handle (tFramework_t *tf, saTest_t *pat, paHandleL4_t paHdl, int QCmdMem, paCmdReply_t *cmdReply);
Bool testCommonDelL2L3Connection (tFramework_t *tf, saTest_t *pat, int numHandles, t2Handles_t *l2Handles, int *numConnet);
Bool testCommonDelL4Connection (tFramework_t *tf, saTest_t *pat, int numHandles, t4Handles_t *l4Handles, int *numConnet);
void testCommonSendNullPkt(tFramework_t *tf, uint32_t *swInfo);

 
void testDispPkts (Cppi_HostDesc *hd);
 	
void mdebugHaltPdsp (int pdspNum);

/* Tests */
void saESPTest (void *arg);
void saAHTest (void *arg);
void saSRTPTest (void *arg);
void saSrtpEspTest (void *arg);
void saSrtpAhTest (void *arg);
void saLTETest (void *arg);
void saDataModeTest (void *arg);
void saESPTest2 (void *arg);

/* SA REG base address */
extern uint32_t saRegBaseAddr;

#endif /* _UNITTEST_H */
