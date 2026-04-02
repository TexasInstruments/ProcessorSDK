#ifndef _UNITTEST_H
#define _UNITTEST_H

/******************************************************************************
 * FILE PURPOSE: Definitions and prototypes for SA Unit Test.
 ******************************************************************************
 * FILE NAME:   unittest.h
 *
 * DESCRIPTION: Contains definitions and function prototypes for SA
 *              Unit Test.
 *
 * (C) Copyright 2009-2020 Texas Instruments, Inc.
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
#ifdef USE_BIOS
  #include <xdc/std.h>
  #include <xdc/runtime/System.h>
  #include <xdc/runtime/Memory.h>
  #include <xdc/runtime/Error.h>
  
  #include <ti/sysbios/BIOS.h>
  #include <ti/sysbios/knl/Task.h>
  #include <ti/sysbios/knl/Semaphore.h>
  #include <ti/sysbios/gates/GateHwi.h>
  #include <ti/sysbios/knl/Clock.h>
  #include <ti/sysbios/hal/Hwi.h>
  #ifdef __ARM_ARCH_7A__
  #include <ti/sysbios/family/arm/a15/Mmu.h>
  #endif
#else
  #include <stdio.h>
  #include <stdarg.h>
  #include <ti/csl/soc.h>
  #include <errno.h>
  void Task_exit(void);
  #define BIOS_exit(x) 
  typedef unsigned int UArg;
#endif

#include <ti/drv/sa/test/SaUnitTest/src/saLog.h>

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#include <ti/drv/pa/pa.h>
#include <ti/drv/pa/pasahost.h>
#include <ti/drv/pa/nss_if.h>   
#include <ti/drv/pa/nss_cfg.h>
#elif defined(NSS_LITE2)
#else
#include <ti/drv/sa/test/SaUnitTest/k2g/nss/nss_if.h>
#include <ti/drv/sa/test/SaUnitTest/k2g/nss/nss_cfg.h>
#endif /* NSS_LITE */

#ifdef NSS_LITE2
#include <ti/csl/soc.h>
#include <ti/osal/osal.h>
/* UDMA Driver Header File. */
#include <ti/drv/udma/udma.h>
/***************************************************************************************
 This section is for internal information to help maintain this module or to explain
 key aspects or caution/ warnings that are not exposed via Doxygen.
 ***************************************************************************************/

/* general (i.e. for public APIs if this is a library) header files			*/


/* internal <package name (if appropriate)> header files 					*/
/* example #include "ipc_pv.h"												*/

/*	For each #DEFINE, include the following Doxygen information				*/
/**
 *  \def    <name of #define>
 *  \brief  <description>
 */
/* Tests for ZEBU platform other options: SVB_QT, SVB_Si, EVM, QT */
#define PLATFORM 	                    ZEBU 
extern void initialise_monitor_handles(void);
#else
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/cppi_desc.h>
#endif

#ifdef _TMS320C6X
#include <ti/csl/csl_chipAux.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_xmcAux.h>
#endif

#if defined(__aarch64__) && defined(SAU_PRMOTE_DEMOTE_TEST)
#include <kicktrigger/testKick2Secure.h>
#endif

#ifdef __ARM_ARCH_7A__
#include <ti/csl/cslr_msmc.h>
#endif

#include <string.h>
#include "salldsim/salldsim.h"
#include <ti/osal/osal.h>

/* Due to a bug in the simulator all packet lengths must be rounded up to 4 byte multiples */
#define PA_SIM_BUG_4BYTES

/* 
 * Compiler Switch for Test Vector Collections 
 *
 * Note:We only support test vector collection operation for single-channel single-mode operation
 */
#define SA_GEN_TEST_VECTOR_TX       0
#define SA_GEN_TEST_VECTOR_RX       0
#define SA_GEN_TEST_VECTOR   (SA_GEN_TEST_VECTOR_TX || SA_GEN_TEST_VECTOR_RX)

typedef enum  {
	SA_TEST_FAILED  = -1,
	SA_TEST_NOT_RUN,
	SA_TEST_SKIP_TEST,
	SA_TEST_PASSED
} saTestStatus_t;

/* Define the test interface */
typedef struct saTest_s
{
	void (*testFunction)(void*, void*);   /* The code that runs the test */
	char  name[80];							/* The test name */
	saTestStatus_t testStatus;			/* Test status */
	
} saTest_t;

extern saTest_t saTestList[];

int testCommonSetTestStatus(void (*fxn)(void*, void*), saTestStatus_t status);
saTestStatus_t testCommonGetTestStatus(void (*fxn)(void*, void*));


#ifdef SOC_C6678
#define SAUTEST_KEY_IN_INTMEM_NOT_SUPPORTED 1
#else
#define SAUTEST_KEY_IN_INTMEM_NOT_SUPPORTED 0
#endif

#define TF_NUM_GEN_QUEUES		   		  100
#ifdef NSS_GEN2
#define CONFIG_FIRST_GEN_QUEUE            1024

#elif defined(NSS_LITE)
#define CONFIG_FIRST_GEN_QUEUE            90
#else

#define CONFIG_FIRST_GEN_QUEUE            900

#endif

/* Link Buffer queue related definitions */
#define TF_Q_FREE_DESC					    (CONFIG_FIRST_GEN_QUEUE + 1)   /* Unassociated descriptor queue number */
#define TF_TEARDOWN_QUE_MGR  			    (CONFIG_FIRST_GEN_QUEUE + 2)   /* Teardown queue number */
#define TF_TEARDOWN_QUE_NUM  			    0


#define TF_LINKED_BUF_Q1           		    (CONFIG_FIRST_GEN_QUEUE + 3)    /* First queue with attached buffers (for PA commands as well) */
#define TF_LINKED_BUF_Q1_BUF_SIZE  		    128
#define TF_LINKED_BUF_Q1_NBUFS       	    32
extern unsigned char memQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE];

#define TF_LINKED_BUF_Q2		   			(CONFIG_FIRST_GEN_QUEUE + 4)	/* Second queue with attached buffers */
#define TF_LINKED_BUF_Q2_BUF_SIZE  			512
#define TF_LINKED_BUF_Q2_NBUFS				 32
extern unsigned char memQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE];

#define TF_LINKED_BUF_Q3		   			(CONFIG_FIRST_GEN_QUEUE + 5)
#define TF_LINKED_BUF_Q3_BUF_SIZE  			1600
#define TF_LINKED_BUF_Q3_NBUFS      		  8
extern unsigned char memQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE];

#define TF_LINKED_BUF_Q4		   			(CONFIG_FIRST_GEN_QUEUE + 6)
#define TF_LINKED_BUF_Q4_BUF_SIZE  		   4800
#define TF_LINKED_BUF_Q4_NBUFS      		  2
extern unsigned char memQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE];

#define TF_HOST_LINKED_BUF_Q1           	(CONFIG_FIRST_GEN_QUEUE + 7)     /* First queue with attached buffers (for PA commands, Stats and Host Tx initiated packets ) */
#define TF_HOST_LINKED_BUF_Q1_BUF_SIZE  	128
#define TF_HOST_LINKED_BUF_Q1_NBUFS       	 24
extern unsigned char memHostQ1[TF_HOST_LINKED_BUF_Q1_NBUFS][TF_HOST_LINKED_BUF_Q1_BUF_SIZE];

#define TF_HOST_LINKED_BUF_Q2		   		(CONFIG_FIRST_GEN_QUEUE + 8)		/* Second queue with attached buffers */
#define TF_HOST_LINKED_BUF_Q2_BUF_SIZE  	256
#define TF_HOST_LINKED_BUF_Q2_NBUFS			 12
extern unsigned char memHostQ2[TF_HOST_LINKED_BUF_Q2_NBUFS][TF_HOST_LINKED_BUF_Q2_BUF_SIZE];

#define TF_HOST_LINKED_BUF_Q3		   		(CONFIG_FIRST_GEN_QUEUE + 9)
#define TF_HOST_LINKED_BUF_Q3_BUF_SIZE  	512
#define TF_HOST_LINKED_BUF_Q3_NBUFS      	  6
extern unsigned char memHostQ3[TF_HOST_LINKED_BUF_Q3_NBUFS][TF_HOST_LINKED_BUF_Q3_BUF_SIZE];

#define TF_HOST_LINKED_BUF_Q4		   		(CONFIG_FIRST_GEN_QUEUE + 10)
#define TF_HOST_LINKED_BUF_Q4_BUF_SIZE     4800
#define TF_HOST_LINKED_BUF_Q4_NBUFS       	  2
extern unsigned char memHostQ4[TF_HOST_LINKED_BUF_Q4_NBUFS][TF_HOST_LINKED_BUF_Q4_BUF_SIZE];

#ifdef NSS_GEN2
/* PASS Local Queue definitions */
#define TF_Q_LOC_FREE_DESC 				     32   /* Unassociated descriptor queue number */

#define TF_LOC_LINKED_BUF_Q1           	     33   /* First queue with attached buffers */
extern unsigned char memLocQ1[TF_LINKED_BUF_Q1_NBUFS][TF_LINKED_BUF_Q1_BUF_SIZE];

#define TF_LOC_LINKED_BUF_Q2		   		 34   /* Second queue with attached buffers */
extern unsigned char memLocQ2[TF_LINKED_BUF_Q2_NBUFS][TF_LINKED_BUF_Q2_BUF_SIZE];

#define TF_LOC_LINKED_BUF_Q3		   		 35
extern unsigned char memLocQ3[TF_LINKED_BUF_Q3_NBUFS][TF_LINKED_BUF_Q3_BUF_SIZE];

#define TF_LOC_LINKED_BUF_Q4		   		 36
extern unsigned char memLocQ4[TF_LINKED_BUF_Q4_NBUFS][TF_LINKED_BUF_Q4_BUF_SIZE];
#endif


#define TF_FIRST_GEN_QUEUE		   			(CONFIG_FIRST_GEN_QUEUE + 11)		/* Queues available for general use */

 /* General purpose queue usage */
 #define Q_CMD_RECYCLE		  0		/* Command descriptors/buffers recycled here after sent to PA */
 #define Q_CMD_REPLY  		  1		/* Replies from PA routed here */
 #define Q_MATCH		  	  2		/* Packets from PA which match a lookup criteria */
 #define Q_NFAIL		      3		/* Packets from PA which matches a mac lookup, but failed an L3 lookup */
 #define Q_PARSE_ERR		  4		/* Packets which resulted in a parse error */
 #define Q_MULTI_0			  5		/* Multi route queue 0 */
 #define Q_MULTI_1			  6	    /* Multi route queue 1 */
 #define Q_MULTI_2			  7		/* Multi route queue 2 */
 #define Q_MULTI_3			  8		/* Multi route queue 3 */
 #define Q_MULTI_4			  9		/* Multi route queue 4 */
 #define Q_MULTI_5			 10		/* Multi route queue 5 */
 #define Q_MULTI_6           11		/* Multi route queue 6 */
 #define Q_MULTI_7			 12		/* Multi route queue 7 */
 
 #define Q_DATA_RECYCLE		 15	    /* DATA Buffer Recycle */
 
 #define Q_REQ_STATS_RECYCLE 16
 #define Q_STATS_REPLY		 17
 
 #define Q_PKT_RECV          18     /* The first Packet Receive Queue */
 #define Q_PKT_TX            19     /* The first Queue for Tx packet  */
 #define DEST_QUEUE_PKT_RECV        (TF_FIRST_GEN_QUEUE + Q_PKT_RECV)
 #define DEST_QUEUE_PKT_TX          (TF_FIRST_GEN_QUEUE + Q_PKT_TX)
 
/* The number of PA handles maintained by this test */
#define TF_NUM_LOCAL_HANDLES	64

 /* Commands to the PA are verified through the value in swinfo0.
  * The 16 ms bits are used as verification, the 16 lbs are for local handle id */
#define TF_CMD_SWINFO0_ADD_ID  		0x11110000  /* Identifies add mac command */
#define TF_CMD_SWINFO0_DEL_ID  		0x22220000  /* Identifies del mac command */
#define TF_CMD_SWINFO0_STATS_REQ_ID	0x33330000	/* Identifies the req stats command */
#define TF_CMD_SWINFO0_CONFIG_ID	0x44440000	/* Identifies the PA configuration command */
#define TF_CMD_SWINFO0_PKT_ID		0x55550000  /* Identifies the packet as a data packet */

#define TF_CACHE_LINESZ    128
#define TF_ROUND_UP(x, y)   (((x) + ((y)-1))/(y)*(y))

/* SA related definitions */
#define SALLDSIM_SC_BUF_SIZE    TF_ROUND_UP(sa_MAX_SC_SIZE, TF_CACHE_LINESZ)  /* 448 or 512 */
#if defined(SOC_AM65XX) || defined(SOC_J721E) || defined(SOC_AM64X)
#define SALLDSIM_NUM_SC_BUF     64      /* Multiple of 16 */
#else
#define SALLDSIM_NUM_SC_BUF     128     /* Multiple of 16 */ 
#endif

#if defined(SAU_PRMOTE_DEMOTE_TEST)
extern uintptr_t* salldsimScBufSecure[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE];
#else
extern uintptr_t* salldsimScBuf[SALLDSIM_NUM_SC_BUF][SALLDSIM_SC_BUF_SIZE]; 
#endif

#if defined (SA_GEN_TEST_VECTOR)
/* Memory to store test packets */
#define TEST_PKT_RAM_SIZE       0x20000
extern uint8_t  testPktRam[TEST_PKT_RAM_SIZE];
extern uint32_t testPktOffset;
#endif

#ifdef NSS_LITE2
typedef uintptr_t physptr_t;
#define  Osal_VirtToPhys(x)   (physptr_t)(x)
#define  Osal_PhysToVirt(x)   (void *)((uintptr_t)x)

#define  TF_RING_TRSIZE      (8U)  /*Size (in bytes) of each ring entry (for 48-bit packet descriptor ptr) */
#define  TF_RING_TRCNT       (32U) /* number of ring entries */
#define  TF_RING_MAX_CNT     (768U)/* Maximum number of rings */
#define  TF_RING_MAX_MONITOR (0U)
#define  TF_RING_MAX_MONITOR_INTR (0U)
#define  TF_RING_MAX_RING_MAPS (16U)
#define TF_NUM_DESC      (TF_RING_TRCNT)     /* 128 host descriptors managed by the Ring Acc */
#define TF_SIZE_DESC     256    /* Must be multiple of 16 */

#if defined(SAU_PRMOTE_DEMOTE_TEST)
#define  TF_DESC_BUFSIZE       (512U)
#else
#define  TF_DESC_BUFSIZE       (4096U)
#endif


/*  Channels */

/*
 * Check SA resource availability if running on HS device and configure what
 * resource parameters to use for the unit test
 */
uint8_t configSaRes(void);

#if defined (SOC_AM65XX) || defined (SOC_J721E) || defined (SOC_AM64X)
extern uint32_t TF_SA2UL_PEER_RXCHAN0;
extern uint32_t TF_SA2UL_PEER_RXCHAN1;
extern uint32_t TF_SA2UL_PEER_TXCHAN;
extern uint32_t SA_UTEST_SA2_UL0_BASE;
#endif

/*
 *  CPPI host descriptor plus bookkeeping pointers outside
 */
typedef struct FW_CPPI_DESC_S {
    /*! Host descriptor */
    CSL_UdmapCppi5HMPD    hostDesc;
    uint8_t               epib[16];
    uint8_t               psInfo[sa_MAX_CMDLB_SIZE]; /* max sa PSInfo */
    uint8_t               pad[84];
    /*! next ptr (to link unused descriptors) */
    uint64_t              nextPtr;
} FW_CPPI_DESC_T;

#define         SAU_TPUT_NUM_PKTSIZES             (4U)
#define         SAU_TPUT_PKTBATCH_CNT             (16U)
#define         SAU_TPUT_MAX_PROFILE_CHANNELS     (100U)
/*
 * Throughput data collection
 */
typedef struct sauTest_tputData_s {
    uint32_t         connId;
    uint32_t         pktLen;
    uint32_t         numPkts;
    uint32_t         cycles;
    uint32_t         ringPushCycles;
    uint32_t         ringPopCycles;
}sauTest_tputData_t;

extern FW_CPPI_DESC_T memDescRamTx[TF_NUM_DESC];
extern uint8_t memDescRamRx[TF_NUM_DESC][TF_SIZE_DESC];
extern uint8_t memBufRamTx[TF_RING_TRCNT][TF_DESC_BUFSIZE];
extern uint8_t memBufRamRx[TF_RING_TRCNT][TF_DESC_BUFSIZE];

/* Non secure Ring memories */
extern uint8_t  memTxRing[TF_RING_TRSIZE * TF_RING_TRCNT*2];
extern uint8_t  memRxFreeRing[TF_RING_TRSIZE * TF_RING_TRCNT*2];
extern uint8_t  memTxCompRing[TF_RING_TRSIZE * TF_RING_TRCNT*2];
extern uint8_t  memRxRing[TF_RING_TRSIZE * TF_RING_TRCNT*2];
extern volatile int32_t  gRxPktCntInRing;

/* Application Ring Push APIs */
extern void RingPush( Udma_RingHandle ringHandle, uint32_t pktSize, physptr_t ptr );
extern int32_t RingPop( Udma_RingHandle ringHandle, FW_CPPI_DESC_T **pAppDesc );

#define EPI_PRESENT (1)     /* Are Extended Packet Info words present? 0=false, 1=true */
#define PSI_LOCATION    (CSL_UDMAP_PS_LOC_DESC) /* PSI data location: CSL_UDMAP_PS_LOC_DESC or CSL_UDMAP_PS_LOC_PACKET */

#else
#define TF_NUM_DESC      128    /* 128 host descriptors managed by the QM */
#define TF_SIZE_DESC     128    /* Must be multiple of 16 */
#endif


/* Define the test framework */
typedef struct tFramework_s  {
	
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
	Pa_Handle   passHandle;	/* PA instance handle */
#endif

#ifdef NSS_LITE2
    uint32_t                txRingNum;
    uint32_t                txComplRingNum;
    uint32_t                rxComplRingNum;
    uint32_t                rxFreeRingNum;
    int32_t                 tfFlowNum;
    /* UDMA Driver handle */
    Udma_DrvHandle          gDrvHandle;

    /* UDMA Handles */
    Udma_ChHandle           gRxChHandle[2];
    Udma_ChHandle           gTxChHandle;
    Udma_FlowHandle         gRxFlowHandle;
    Udma_RingHandle         gRxFreeRingHandle;
    Udma_RingHandle         gRxRingHandle;
    Udma_RingHandle         gTxComplRingHandle;
    Udma_RingHandle         gTxRingHandle;
    Udma_EventHandle        gRxEvtHandle;

    /*! Reclaimed but not reused descriptors */
    FW_CPPI_DESC_T     *txReadyDescs;
#else
	Cppi_Handle tfPaCppiHandle;    /* PA CDMA handle */ 
	Cppi_Handle tfPaLocCppiHandle; /* PA Local CDMA handle */ 
	
	Cppi_ChHnd  tfPaTxChHnd[NSS_MAX_TX_PKTDMA_CHANNELS];
	Cppi_ChHnd  tfPaRxChHnd[NSS_MAX_RX_PKTDMA_CHANNELS];
    #ifdef NSS_GEN2
	Cppi_ChHnd  tfPaLocTxChHnd[NSS_MAX_TX_PKTDMA_CHANNELS];
	Cppi_ChHnd  tfPaLocRxChHnd[NSS_MAX_RX_PKTDMA_CHANNELS];
	
	/* Queues */
    Qmss_SubSysHnd  tfPaQmssHandle;             /* PA sub-system QMSS handle */
    #endif /* NSS_GEN2 */
	int32_t QPaTx[NSS_MAX_TX_QUEUES];

	int32_t QfreeDesc;		      				/* Unassociated descriptor queue handle */
	int32_t QtDown;								/* Tear down queue handle */
	int32_t QLinkedBuf1;						/* First Queue with descriptors and attached linked buffers */
	int32_t QLinkedBuf2;						/* Second Queue with descriptors and attached linked buffers */
	int32_t QLinkedBuf3;						/* Third Queue with descriptors and attached linked buffers */
	int32_t QLinkedBuf4;						/* Fourth Queue with descriptors and attached linked buffers */
	int32_t QHostLinkedBuf1;					/* First Queue with descriptors and attached linked buffers */
	int32_t QHostLinkedBuf2;					/* Second Queue with descriptors and attached linked buffers */
	int32_t QHostLinkedBuf3;					/* Third Queue with descriptors and attached linked buffers */
	int32_t QHostLinkedBuf4;					/* Fourth Queue with descriptors and attached linked buffers */
    #ifdef NSS_GEN2
	int32_t QLocfreeDesc;		      			/* Unassociated descriptor queue handle */
	int32_t QLocLinkedBuf1;						/* First Queue with descriptors and attached linked buffers */
	int32_t QLocLinkedBuf2;						/* Second Queue with descriptors and attached linked buffers */
	int32_t QLocLinkedBuf3;						/* Third Queue with descriptors and attached linked buffers */
	int32_t QLocLinkedBuf4;						/* Third Queue with descriptors and attached linked buffers */
    #endif
	int32_t QGen[TF_NUM_GEN_QUEUES];				/* General purpose queues */

	Cppi_FlowHnd tfPaFlowHnd;					/* Flow handle */
	int32_t		 tfFlowNum;						/* Physical flow number */
	
    #ifdef NSS_GEN2
	Cppi_FlowHnd tfPaLocFlowHnd0; 				/* Flow handle for local flow 0  */
	int32_t		 tfLocFlowNum; 					/* Physical local flow number */
    #endif
#endif  /* NSS_LITE2 */

    SemaphoreP_Handle tfPaTableL2Sem;			/* Semaphore for PA internal table for L2 */
    SemaphoreP_Handle tfPaTableL3Sem;			/* Semaphore for PA internal table for L3 */

    SemaphoreP_Handle tfQmSem;					/* Semaphore for queue manager */
    SemaphoreP_Handle tfSaSem;					/* Semaphore for SA LLD */
    
    /* SA test related variables */
    Sa_Handle   salld_handle;
    int         salld_buf_sizes[sa_N_BUFS];
    salldSimChannel_t salldSimChn[salld_SIM_MAX_CHANNELS];
    
    /* Output files */
    FILE* fp_ctx;       /* context output file */
    FILE* fp_pktIn;     /* record packets which enter PA/SA */
    FILE* fp_pktOut;    /* record packets from PA/SA */
    
    /* record the current test */
	saTest_t* pTest;
} tFramework_t;

extern tFramework_t tFramework;
#if defined(SAU_PRMOTE_DEMOTE_TEST)
extern tFramework_t tFrameworkSecure;
#endif

#ifndef NSS_LITE2
extern nssGlobalConfigParams_t nssGblCfgParams;
#endif

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/* PA memory */
#define TF_PA_INST_SIZE	  400                 /* Required size = 320 */
extern uint8_t memPaInst[TF_ROUND_UP(TF_PA_INST_SIZE, TF_CACHE_LINESZ)];

#define TF_MAX_NUM_L2_HANDLES  256
#define TF_L2_TABLE_SIZE	(TF_MAX_NUM_L2_HANDLES * 36)  /* Requires 36 bytes per entry */
extern uint8_t memL2Ram[TF_ROUND_UP(TF_L2_TABLE_SIZE, TF_CACHE_LINESZ)];

#define TF_MAX_NUM_L3_HANDLES  512
#define TF_L3_TABLE_SIZE	(TF_MAX_NUM_L3_HANDLES * 76)  /* Requires 76 bytes per entry */
extern uint8_t memL3Ram[TF_ROUND_UP(TF_L3_TABLE_SIZE, TF_CACHE_LINESZ)];

#define TF_MAX_NUM_ACL_HANDLES  512
#define TF_ACL_TABLE_SIZE	(TF_MAX_NUM_ACL_HANDLES * 104)  /* Requires 104 bytes per entry */
extern uint8_t memAclRam[TF_ROUND_UP(TF_ACL_TABLE_SIZE, TF_CACHE_LINESZ)];

#define TF_USR_STATS_LNK_TABLE_SIZE   (pa_USR_STATS_MAX_COUNTERS * 4)  /* Require 4 bytes per satistics */ 
extern uint8_t memUsrStatsLnkTbl[TF_USR_STATS_LNK_TABLE_SIZE];
#endif

/* QM memory */	
#define TF_NUM_RES_DESC             (TF_LINKED_BUF_Q1_NBUFS         +  \
                                     TF_LINKED_BUF_Q2_NBUFS         +  \
                                     TF_LINKED_BUF_Q3_NBUFS         +  \
                                     TF_LINKED_BUF_Q4_NBUFS         +  \
                                     TF_HOST_LINKED_BUF_Q1_NBUFS    +  \
                                     TF_HOST_LINKED_BUF_Q2_NBUFS    +  \
                                     TF_HOST_LINKED_BUF_Q3_NBUFS    +  \
                                     TF_HOST_LINKED_BUF_Q4_NBUFS)

/* Memory used for the linking RAM and descriptor RAM */
extern uint8_t  memDescRam[TF_NUM_DESC * TF_SIZE_DESC];	
extern uint8_t  *passDescRam;

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

/* 32 L2 handles are managed. This structure is used to track the handle and
 * the activation state state of the handle */
enum  {
  TF_L2_HANDLE_UNCONFIGURED = 0,
  TF_L2_HANDLE_PENDING_ACK,
  TF_L2_HANDLE_ACTIVE,
  TF_L2_HANDLE_DISABLED
};

typedef struct t2Handles_s  {

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  paHandleL2L3_t  paHandle;     /* The handle returned by the PA LLD */
#endif
  uint32_t		state;		    /* TF_L2_HANDLE_UNCONFIGURED = handle not configured
							     * TF_L2_HANDLE_PENDING_ACK = handle configured and sent to pa
							     * TF_L2_HANDLE_ACTIVE = handle creation acknowledged by pa */

} t2Handles_t;

typedef struct t4Handles_s  {

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  paHandleL4_t    paHandle;     /* The handle returned by the PA LLD */
#endif
  uint32_t		state;		    /* TF_L2_HANDLE_UNCONFIGURED = handle not configured
							     * TF_L2_HANDLE_PENDING_ACK = handle configured and sent to pa
							     * TF_L2_HANDLE_ACTIVE = handle creation acknowledged by pa */

} t4Handles_t;

typedef struct testPktDesc_s  {
    Bool      tx;                 /* 0: Tx Packet 1:Rx Packet */
    uint16_t  chnum;              /* Channel Index */
    uint16_t  size;               /* To be enhanced to include payload information */
    uint16_t  payloadOffset;
    uint16_t  payloadLen;
    uint8_t*  pkt;                /* Pointer to the packet */
} testPktDesc_t;

#define SA_TEST_MAX_L2_HANDLES    4     /* Max number of L2 Handles */
#define SA_TEST_MAX_L3_HANDLES   30     /* Max number of L3 Handles */
#define SA_TEST_MAX_L4_HANDLES   90     /* Max number of L4 Handles */

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
extern paSysStats_t paTestExpectedStats;  /* Expected stats results */
#endif
extern t2Handles_t     l2Handles[SA_TEST_MAX_L2_HANDLES];
extern t2Handles_t     l3Handles[SA_TEST_MAX_L3_HANDLES];
extern t4Handles_t     l4Handles[SA_TEST_MAX_L4_HANDLES];
extern int numL2Handles, numL3Handles, numL4Handles;

/* 
 * Enable/Disable Simulator:
 *  
 * TRUE: simulator is used 
 * FALSE: silicon is used
 *
 * Note: This varaible is used to determine some test parameters such as number of test packets,
 *       number of test channels to avoid the long test time at simulator.
 */
extern Bool saSimTest;
extern uint16_t numTestPkt1;
extern uint16_t numTestPkt2;
extern uint16_t numTestPkt3;

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/*
 * Assume that all L2/L3/L4 handles will be requested sequentially
 * and will last the entire test.
 * All the buffers will be initialized prior to each test.
 * Use simple MACROs for handle allocations
 */
#define TEST_ALLOC_L2_HANDLE()     (numL2Handles < SA_TEST_MAX_L2_HANDLES)?      \
                                   &l2Handles[numL2Handles++]:NULL
#define TEST_ALLOC_L3_HANDLE()     (numL3Handles < SA_TEST_MAX_L3_HANDLES)?      \
                                   &l3Handles[numL3Handles++]:NULL
#define TEST_ALLOC_L4_HANDLE()     (numL4Handles < SA_TEST_MAX_L4_HANDLES)?      \
                                   &l4Handles[numL4Handles++]:NULL
#endif

/* Miscellaneous Macros */
#define MK_UINT16(a, b)             (((a) << 8) | (b))
#define MK_UINT32(a, b, c, d)       (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
#define SYS_DIV_ROUND_UP(val,rnd)   (((val)+(rnd)-1)/(rnd))
#define SYS_ROUND_UP(val,rnd)       SALLD_DIV_ROUND_UP(val,rnd)*(rnd)

#ifndef USE_BIOS
#ifndef ARM11
/* Inline intrinsic, to save mips in data path */
#define thwCriticalBeginFast() _disable_interrupts()

/* Type to save from _disable_interrupts, and to pass to _restore_interrupts */
typedef uint32_t thwCriticalState_t;

/* Function, to save memory in control code */
void thwCriticalBegin(void);

/******************************************************************************
 * FUNCTION PURPOSE: End of critical section (up to 15 sections can be nested)
 *                   
 ******************************************************************************
 * DESCRIPTION: Restore interrupt enable/disable status encountered on entry to
 *              critical section. Bit ST1_INTM is poped from the global variable.
 *****************************************************************************/
#define thwCriticalEndFast(x) _restore_interrupts(x)
void thwCriticalEnd(void);
#endif /* ARM11 */
#endif /* USE_BIOS */

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
#ifndef SALLD_UNIT_TEST_MAX_SEGMENTS
#define SALLD_UNIT_TEST_MAX_SEGMENTS 10
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

int setupTestFramework (void);
int setupNavss(void);
int exitTestFramework(void);
int verifyTestFramework (void);
void saTestRecoverAndExit (tFramework_t *tf, saTest_t *pat, saTestStatus_t testResult);
void utilCycleDelay (int32_t count);
uintptr_t utilgAddr(uintptr_t x);
void utilInputTestPkts (char* file_name);
uint16_t utilOnesCompAdd (uint16_t value1, uint16_t value2);
uint16_t utilOnesCompChkSum ( uint16_t *p_data, uint16_t len );
Bool utilGetPkt(testPktDesc_t *pktDesc);  
void utilSetIpv4ChkSum (uint8_t *data);
uint16_t utilGetIpv4PsudoChkSum (uint8_t *data, uint16_t payloadLen);
uint16_t utilGetIpv6PsudoChkSum (uint8_t *data, uint16_t payloadLen);
void utilSetUdpChkSum (uint8_t *data, uint16_t len, uint16_t psudoChkSum);
void utilPrepFout(tFramework_t  *tf);
void utilCloseFout(tFramework_t  *tf);
void utilOutputPkt(tFramework_t  *tf, uint32_t *swInfo, uint32_t psInfoSize, uint32_t* psInfo,
                   uint32_t pktSize, uint8_t* pkt, Bool in);
void utlOutputSc (tFramework_t *tf,  uint16_t scSize, uint16_t scID, uint8_t* scBuf);
void utilOutputScFromSwInfo(tFramework_t *tf, uint32_t* pSwInfo);
void utilDispKeyStream(uint8_t* buf, uint16_t keySize, uint16_t blockSize);

#if defined(NSS_LITE2)
#define SA2UL_TEST_CPPI5_DESCRIPTOR_TYPE_HOST         (CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST)
#define SA2UL_UDMAP_TX_UNPAUSE                        (1U)
#define SA2UL_UDMAP_TX_PAUSE                          (2U)
CSL_UdmapCppi5HMPD*  testCommonGetBuffer(tFramework_t *tf, int32_t size);
int testCommonRecycleLBDesc (tFramework_t *tf, CSL_UdmapCppi5HMPD *hd);
int testCommonRecycleHostLBDesc (tFramework_t *tf, CSL_UdmapCppi5HMPD *hd);
void testDispPkts (CSL_UdmapCppi5HMPD *hd);

int  salld_test_controlTxDma(uint32_t enable);
#else
Cppi_HostDesc*  testCommonGetBuffer(tFramework_t *tf, int size);

int testCommonRecycleLBDesc (tFramework_t *tf, Cppi_HostDesc *hd);
int testCommonRecycleHostLBDesc (tFramework_t *tf, Cppi_HostDesc *hd);
void testDispPkts (Cppi_HostDesc *hd);
#endif

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
int testCommonRequestPaStats (char *fname, tFramework_t *tf, Bool reset, int32_t QSource, int32_t QRecycle,  paCmdReply_t *reply);
int testCommonCompareStats (char *fname, paSysStats_t *expected, paSysStats_t *actual);
int testCommonComparePktInfo (char *tfName, pasahoLongInfo_t *expected, pasahoLongInfo_t *actual);


void testCommonIncStats (paStatsBmap_t *map,  paSysStats_t *stats);
Bool testCommonReqAndChkPaStats (tFramework_t *tf, saTest_t *pat, paSysStats_t *expected);
Bool testCommonAddMac (tFramework_t *tf, saTest_t *pat, paEthInfo_t *ethInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                   paHandleL2L3_t *pl2handle, int32_t QCmdMem,  paCmdReply_t *repInfo);
Bool testCommonAddIp  (tFramework_t *tf, saTest_t *pat, paIpInfo_t *ipInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                   paHandleL2L3_t l2handle, paHandleL2L3_t *pl3handle, int32_t QCmdMem, paCmdReply_t *repInfo);
Bool testCommonAddIp2 (tFramework_t *tf, saTest_t *pat, int lutInst, paIpInfo_t *ipInfo, paRouteInfo_t *matchRoute, paRouteInfo_t *nfailRoute, 
 	                   paHandleL2L3_t l2handle, paHandleL2L3_t *pl3handle, int32_t QCmdMem, paCmdReply_t *repInfo);
Bool testCommonAddUdp (tFramework_t *tf, saTest_t *pat, uint16_t port, Bool replace, paRouteInfo_t *routeInfo,  
 	                   paHandleL2L3_t l3handle, paHandleL4_t l4handle, int32_t QCmdMem, paCmdReply_t *repInfo);
Bool testCommonConfigExceptionRoute (tFramework_t *tf, saTest_t *pat, int nRoute, int *routeTypes, paRouteInfo_t *eRoutes, 
 	                       		     int32_t QCmdMem, paCmdReply_t *repInfo);
Bool testCommonGlobalConfig (tFramework_t *tf, saTest_t *pat, paCtrlInfo_t *cfgInfo, int32_t QCmdMem, paCmdReply_t *repInfo);
Bool testCommonDelHandle (tFramework_t *tf, saTest_t *pat, paHandleL2L3_t *paHdl, int QCmdMem, paCmdReply_t *cmdReply);
Bool testCommonDelL4Handle (tFramework_t *tf, saTest_t *pat, paHandleL4_t paHdl, int QCmdMem, paCmdReply_t *cmdReply);
Bool testCommonDelL2L3Connection (tFramework_t *tf, saTest_t *pat, int numHandles, t2Handles_t *l2Handles, int *numConnet);
Bool testCommonDelL4Connection (tFramework_t *tf, saTest_t *pat, int numHandles, t4Handles_t *l4Handles, int *numConnet);
#endif
void testCommonSendNullPkt(tFramework_t *tf, uint32_t *swInfo);

void mdebugHaltPdsp (int pdspNum);

/* Tests */
void saESPTest (void* a0, void* a1);
void saAHTest (void* a0, void* a1);
void saESPNATTTest (void* a0, void* a1);
void saSRTPTest (void* a0, void* a1);
void saSRTCPTest (void* a0, void* a1);
void saESPMultiSgmtTest (void* a0, void* a1);
void saSRTPMultiSgmtTest (void* a0, void* a1);
void saSRTCPMultiSgmtTest (void* a0, void* a1);
void saAHMultiSgmtTest (void* a0, void* a1);
void saSrtpEspTest (void* a0, void* a1);
void saSrtpAhTest (void* a0, void* a1);
void saAirCipheringTest (void* a0, void* a1);
void saAirCipheringKeyGenTest (void* a0, void* a1);
void saDataModeTest (void* a0, void* a1);
void saDataModeTputTest (void* a0, void* a1);
void saESPTest2 (void* a0, void* a1);
void saESPReplayTest (void* a0, void* a1);
void saAHReplayTest (void* a0, void* a1);
void saSRTPReplayTest (void* a0, void* a1);
void saSRTCPReplayTest (void* a0, void* a1);
void saSRTPRekeyTest1 (void* a0, void* a1);
void saSRTPRekeyTest2 (void* a0, void* a1);
void saSRTPRekeyTest3 (void* a0, void* a1);
void saSRTPReKeyTest4 (void* a0, void* a1);
void saSRTCPRekeyTest1 (void* a0, void* a1);
void saSRTCPRekeyTest2 (void* a0, void* a1);
void saESPErrTest (void* a0, void* a1);
void saAHErrTest (void* a0, void* a1);
void saSRTPErrTest (void* a0, void* a1);
void saSRTCPErrTest (void* a0, void* a1);
void saRngTest (void* a0, void* a1);
void saPkaTest (void* a0, void* a1);
void saPkaTest2 (void* a0, void* a1);
void saDataModePromoteDemoteTest (void* a0, void* a1);
#endif /* _UNITTEST_H */
