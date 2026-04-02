/**
 * @file   emac_drv_v5.h
 */
/*
 * Copyright (c) 2018-2020, Texas Instruments Incorporated
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

#ifndef EMAC_DRV_V5_H
#define EMAC_DRV_V5_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/udma/udma.h>
#include <ti/drv/emac/emac_drv.h>
#include <ti/drv/emac/src/v5/emac_mdio.h>
#include <ti/drv/emac/src/emac_osal.h>
#include <ti/drv/emac/emac_ioctl.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define EMAC_MAX_NUM_TX_BUFF_POOL_SIZE  ((uint32_t)16U)
#define EMAC_MAC_PORTS_PER_ICSS         ((uint32_t)2U)
#if defined (SOC_AM65XX)
#define EMAC_MAX_ICSS                   ((uint32_t)3U)
#else
#define EMAC_MAX_ICSS                   ((uint32_t)2U)
#endif
#define EMAC_MAX_ICSSG_INSTANCES        EMAC_MAX_ICSS
#define EMAC_MAX_CPSW               ((uint32_t)1U)
#define EMAC_MAX_PORTS              ((EMAC_MAX_ICSS)*(EMAC_MAC_PORTS_PER_ICSS) +\
                                    (EMAC_MAX_CPSW))

#define EMAC_PORT_STATE_CLOSED      0
#define EMAC_PORT_STATE_OPEN        ((uint32_t)1U)

#if !defined (__aarch64__)
#define EMAC_INCOHERENT
#endif

#define EMAC_MAX_FREE_RINGS_PER_SUBCHAN ((uint32_t)9U)
#define EMAC_MAX_RX_SUBCHAN_PER_CHAN ((uint32_t)16U)
#define EMAC_TX_MAX_CHANNELS_PER_PORT ((uint32_t)4U)
#define EMAC_TX_MAX_CHANNELS_PER_SWITCH ((uint32_t)8U)

#define EMAC_AM65XX_PG1_0_VERSION (0x0BB5A02FU)

#define EMAC_CACHELINE_ALIGNMENT ((uint32_t)64U)

/* EMAC function table pointer */
extern const EMAC_FxnTable EMAC_FxnTable_v5;

/* For Dual MAC use case*/
#define EMAC_ICSSG0_PORT0 ((uint32_t)0U)
#define EMAC_ICSSG0_PORT1 ((uint32_t)1U)
#define EMAC_ICSSG1_PORT0 ((uint32_t)2U)
#define EMAC_ICSSG1_PORT1 ((uint32_t)3U)
#define EMAC_ICSSG2_PORT0 ((uint32_t)4U)
#define EMAC_ICSSG2_PORT1 ((uint32_t)5U)
#define EMAC_CPSW_PORT1 ((uint32_t)6U)

#define EMAC_ICSSG0_SWITCH_PORT  ((uint32_t)7U)     /* for emac open/close/certain IOCTL's, flooded packet */
#define EMAC_ICSSG0_SWITCH_PORT0 ((uint32_t)8U)     /* host port for certain IOCTL's */
#define EMAC_ICSSG0_SWITCH_PORT1 ((uint32_t)9U)    /* for directed send, stats, for certain IOCTL's */
#define EMAC_ICSSG0_SWITCH_PORT2 ((uint32_t)10U)    /* for directed send, stats, for certain IOCTL's */

#define EMAC_ICSSG1_SWITCH_PORT ((uint32_t)11U)      /* for emac open/close/certain IOCTL's, flooded packet */
#define EMAC_ICSSG1_SWITCH_PORT0 ((uint32_t)12U)     /* host port for certain IOCTL's */
#define EMAC_ICSSG1_SWITCH_PORT1 ((uint32_t)13U)    /* for directed send, stats, for certain IOCTL's */
#define EMAC_ICSSG1_SWITCH_PORT2 ((uint32_t)14U)    /* for directed send, stats, for certain IOCTL's */

#define EMAC_ICSSG2_SWITCH_PORT ((uint32_t)15U)      /* for emac open/close/certain IOCTL's, flooded packet */
#define EMAC_ICSSG2_SWITCH_PORT0 ((uint32_t)16U)     /* host port for certain IOCTL's */
#define EMAC_ICSSG2_SWITCH_PORT1 ((uint32_t)17U)    /* for directed send, stats, for certain IOCTL's */
#define EMAC_ICSSG2_SWITCH_PORT2 ((uint32_t)18U)    /* for directed send, stats, for certain IOCTL's */
#define EMAC_PORT_MAX            ((uint32_t)19U)    /* for directed send, stats, for certain IOCTL's */

/* Compatibility defines for code base using Interposer card specific macros */
#define EMAC_SWITCH_PORT0 EMAC_ICSSG0_SWITCH_PORT0  // host port
#define EMAC_SWITCH_PORT1 EMAC_ICSSG0_SWITCH_PORT1
#define EMAC_SWITCH_PORT2 EMAC_ICSSG0_SWITCH_PORT2
#define EMAC_SWITCH_PORT EMAC_ICSSG0_SWITCH_PORT  // used at time of emac_open to open/close software port 0,1,2,3, emac_send for undirected packet

#define EMAC_CPSW_PORT_NUM ((uint32_t)6U)
#define EMAC_MAX_NUM_EMAC_PORTS ((uint32_t)7U)
#define EMAC_ETHHDR_SIZE ((uint32_t)14U)

/* EMAC master control block (MCB) field macros */
#define EMAC_CORE_NUM                       emac_mcb.core_num
#define EMAC_MDIO(port_num)                 emac_mcb.port_cb[port_num].mdio_flag
#define EMAC_NUM_RX_PKTS(port_num)          emac_mcb.port_cb[port_num].num_of_rx_pkt_desc
#define EMAC_NUM_TX_PKTS(port_num)          emac_mcb.port_cb[port_num].num_of_tx_pkt_desc
#define EMAC_NUM_TXRX_PKTS(port_num)        emac_mcb.port_cb[port_num].num_of_rxtx_pkt_desc
#define EMAC_PKT_SIZE(port_num)             emac_mcb.port_cb[port_num].max_pkt_size
#define EMAC_MACADDR_CFG(port_num)          emac_mcb.port_cb[port_num].macaddr_cfg.addr
#define EMAC_RX_PKT(port_num, pkt)          emac_mcb.port_cb[port_num].rx_pkt_cb(port_num, pkt)
#define EMAC_ALLOC_PKT(port_num, size)      emac_mcb.port_cb[port_num].alloc_pkt_cb(port_num, size)
#define EMAC_FREE_PKT(port_num, pkt)        emac_mcb.port_cb[port_num].free_pkt_cb(port_num, pkt)
#define EMAC_COMM_PCB(port_num)             emac_mcb.comm_pcb[port_num]
#define EMAC_GLOBAL_RX_SEM_HANDLE(port_num, ringNum) emac_mcb.port_cb[port_num].rxSemaphoreHandle[ringNum]
#define EMAC_GLOBAL_RX_MGMT_SEM_HANDLE(port_num, ringNum) emac_mcb.port_cb[port_num].rxMgmtSemaphoreHandle[ringNum]

#define EMAC_GLOBAL_RX_HWI_HANDLE(port_num) emac_mcb.port_cb[port_num].hwi
#define EMAC_CFG_LOOPBACK(port_num)                 emac_mcb.port_cb[port_num].loop_back

/*!
 *  @brief    EMAC driver mode of operation
 */
#define EMAC_MODE_INTERRUPT     ((uint32_t)0U)
#define EMAC_MODE_POLL          ((uint32_t)1U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

#define  EMAC_CPPI_DESC_SIZE ((uint32_t)128U)
/** CPPI host descriptor plus bookkeeping pointers*/
typedef struct EMAC_CPPI_DESC_S {
    /* Host descriptor */
    CSL_UdmapCppi5HMPD    hostDesc;
    uint32_t psinfo[4];
    uint32_t reserved[(EMAC_CPPI_DESC_SIZE - sizeof(CSL_UdmapCppi5HMPD) - 2*sizeof(void *) - 
    4*sizeof(uint32_t))/sizeof(uint32_t)];
    /* App ptr */
    EMAC_PKT_DESC_T *appPtr;
    /* next ptr (to link unused descriptors) */
    struct EMAC_CPPI_DESC_S *nextPtr;
} EMAC_CPPI_DESC_T;

typedef struct EMAC_PRU_CFG_S {
    /**< Debug, PRU status */
    uint32_t status;
    /**< Base address (LO) of transmit buffer pools in MSMC */
    uint32_t addr_lo;
    /**< Base address (HI) of transmit buffer pools in MSMC, not currently used, MUST BE 0 */
    uint32_t addr_hi;
    /**< Array of transmit buffer pool sizes */
    uint32_t tx_bs[16];
    /**< Configure number of active Egress(TX) Threads/Channels */
    uint32_t act_thr_n;
    /**< Bit field to enable/disable rate limit for each egress thread */
    uint32_t egr_rate_lim_en;
    /**<The flow id for the first RX ring. 4 consecutive flow ids will be used. */
    uint32_t def_flow;
    /**<The flow id for the first management ring. Three consecutive ids will be used. */
    uint32_t mgr_flow;
    /**< TBD */
    uint32_t flags;
    uint32_t n_burst;
    /**< Debug, RTU status*/
    uint32_t rtu_status;
    /**< Rerserved */
    uint32_t info;
    uint32_t res;
    /**< Seed for random generator, used by FW to calculate backoff retransmission time for half duplex mode */
    uint32_t seed;
} EMAC_PRU_CFG_T;

typedef void EMAC_PROCESS_DESCRIPTOR_FN_T
(
    uint32_t port_num,
    uint32_t ringNum
);

typedef struct EMAC_POLLING_TABLE_ENTRY_S
{
    Udma_RingHandle freeRingHandle;
    Udma_RingHandle compRingHandle;
    EMAC_PROCESS_DESCRIPTOR_FN_T *ringPollFxn;
} EMAC_POLLING_TABLE_ENTRY_T;

typedef struct EMAC_POLLING_TABLE_S
{
   EMAC_POLLING_TABLE_ENTRY_T rxPkt[EMAC_MAX_RX_SUBCHAN_PER_CHAN];
   EMAC_POLLING_TABLE_ENTRY_T rxMgmt[EMAC_MAX_RX_SUBCHAN_PER_CHAN];
   EMAC_POLLING_TABLE_ENTRY_T txCompletion[EMAC_TX_MAX_CHANNELS_PER_PORT];
} EMAC_POLLING_TABLE_T;

typedef struct EMAC_RX_SUBCHAN_s
{
    /*! 1 typically, 4 max (to support a feature of flow) */
    uint32_t    nfreeRings;
    /*! UDMA  freeRing  handle */
    void         *freeRingHandle[EMAC_MAX_FREE_RINGS_PER_SUBCHAN];
    /*! UDMA  freee ring memory pointers */
    void        *freeRingMem[EMAC_MAX_FREE_RINGS_PER_SUBCHAN];
    /*! UDMA  completion ring handle */
    void            *compRingHandle;
    /*! UDMA  completion ring memory pointer */
    void        *compRingMem;
    /*! UDMA  packet data memory pointers for desciptors*/
    void        *hPdMem[EMAC_MAX_FREE_RINGS_PER_SUBCHAN];
    /*! UDMA  event handles */
    void        *eventHandle;
    /*! Element count, queue depth of the free ring */
    uint32_t     elementCountFree[EMAC_MAX_FREE_RINGS_PER_SUBCHAN];
    /*! Element count, queue depth of the completion ring */
    uint32_t     elementCountCompletion;
} EMAC_RX_SUBCHAN_T;

typedef struct EMAC_CHANNEL_RX_s{
    /**< UDMA  channel handle */
    Udma_ChHandle rxChHandle;
    /* UDMA flow handle */
    Udma_FlowHandle flowHandle;
    /*! Number of subchannels or flows */
    uint32_t        nsubChan;
    /*! sub channel/flow configuration, basically a flow */
    EMAC_RX_SUBCHAN_T subChan[EMAC_MAX_RX_SUBCHAN_PER_CHAN];
    /*! Thread ID  connect to/from emac psi to cpdma */
    uint32_t threadId;
} EMAC_CHANNEL_RX_T;

/**
 * @ingroup emaclld_api_structures
 * @brief       UDMA transmit channel configuration structure
 *
 * @details This is used to configure SOC specific configuration
 *          with the emac driver via hwAttrs field of EMAC_OPEN_CONFIG_INFO_T structure.
 */

typedef struct EMAC_PER_CHANNEL_CFG_TX_s{
    /*! UDMA  channel handle */
    void        *chHandle;
    /*! UDMA  freee ring memory pointers */
    void        *freeRingMem;
    /*! UDMA  completion  ring memory pointer */
    void        *compRingMem;
     /*! UDMA  PD memory pointers for desciptors*/
    void        *hPdMem;
    /*! UDMA  event handles */
    void        *eventHandle;
    /*!  Set to queue depth of the ring  */
    uint32_t    elementCount;
    /*! thread ID to connect cpdma to/from emac psi */
    uint32_t    threadId;
} EMAC_PER_CHANNEL_CFG_TX;


typedef struct EMAC_PER_CHANNEL_CFG_RX_s{
    /*! UDMA  channel handle */
    void            *chHandle;
    /*! UDMA  flow handle for additional non-default flows*/
    void            *flowHandle;
    /*! Number of subchannels or flows */
    uint32_t        nsubChan;
    /*! sub channel/flow configuration */
    EMAC_RX_SUBCHAN_T subChan[EMAC_MAX_RX_SUBCHAN_PER_CHAN];
    /*! Thread ID  connect to/from emac psi to cpdma */
    uint32_t threadId;
} EMAC_PER_CHANNEL_CFG_RX;

#define EMAC_ICSSG_MAX_NUM_BUFFER_POOLS ((uint32_t)24U)

typedef struct EMAC_FW_APP_CONFIG_S {
    uint32_t txPortQueueHighAddr;                           /* MSMC address high of transmit queues, only applicable PG1 */
    uint32_t txPortQueueLowAddr;                            /* MSMC address low of  transmit queues, only applicable PG1 */
    uint32_t txPortQueueSize;                               /* Total size required in bytes for all port queues, only applicable PG1  */
    uint32_t bufferPoolHighAddr;                            /* MSMC address high of buffer pools, only applicable PG2 */
    uint32_t bufferPoolLowAddr;                             /* MSMC address low of buffer pools, only applicable PG2 */
    uint32_t numBufferPool;                                 /* NUmber of buffer pools, only applicable PG2 */
    uint32_t bufferPoolSize[EMAC_ICSSG_MAX_NUM_BUFFER_POOLS]; /* Size in bytes of each buffer pool, only applicable PG2 */
}EMAC_FW_APP_CONFIG;

struct EMAC_FW_PORT_CFG;

#define EMAC_NUM_TRANSMIT_FW_QUEUES ((uint32_t)8U) /* this is the number of TX port queues and number of TX host queues */
/**
 * @brief ICSSG Switch firmware configuration structure 
 */
typedef struct EMAC_ICSSG_SWITCH_FW_CFG_S {
uint32_t txPortQueueSize[EMAC_NUM_TRANSMIT_FW_QUEUES];  /* Size of each TX port queue */
uint32_t txHostQueueSize[EMAC_NUM_TRANSMIT_FW_QUEUES];  /* Size of each TX host queue */
uint32_t descQueueOffset;                              /* SHAREM MEM offset of actual descriptor queues */
uint32_t mtuSize;                                       /* MAX MTU size in bytes */
uint32_t defaultVlanTblOffset;                          /* SHARED MEM offset tto default vlan table */
uint32_t pktFlowIdOffset;                               /* SHARED MEM offset to packet flow ID start*/
uint32_t mgmtFlowIdOffset;                              /* SHARED MEM offset to managment flow ID start */
uint32_t switchPort0DefaultVlanOffset;                  /* SHARED MEM OFFSET to switch port 0/host port default VLAN entry */
uint32_t switchPort1DefaultVlanOffset;                  /* SHARED MEM OFFSET to switch port 1 default VLAN entry */
uint32_t switchPort2DefaultVlanOffset;                  /* SHARED MEM OFFSET to switch port 2 default VLAN entry */
uint32_t prioRegenTableOffset;                          /* DEM0 offset to priority regen tab */
uint32_t txPortQueueDescSize;                           /* Size of each TX port descriptor queue */
uint32_t txHostQueueDescSize;                           /* Size of each TX host descriptor queue */
uint32_t expressPremptiveQueueOffset;                   /* DEM0 offset to priority regen tab */
uint32_t splPacketDefaultPrioOffset;                    /* SHARED MEM OFFSET offset to special pkt default priority */
uint64_t fdbAgeingTimeoutOffset;                        /* DMEM1 offset for storing FDB ageing timeout interval */
uint32_t premptionTxEnabledStatusOffset;                /* DEM0 offset for status of premption on Tx enabled */
uint32_t premptionTxActiveStatusOffset;                 /* DEM0 offset for status of premption on Tx active */
uint32_t premptionVerifyStateStatusOffset;              /* DEM0 offset for status of premption verify state machine*/
uint32_t premptionVerifyStateValueOffset;               /* DEM0 offset for value of premption verify state machine*/
uint32_t premptionMinFragSizeOffset;                    /* DEM0 offset for min fragment size supported by firmware */
uint32_t premptionMinFragAddOffset;                     /* DEM0 offset to configure min fragment size during premption */
uint64_t defaultAgeingTimeout;                          /* Default ageing timeout value */
uint32_t numFdbBucketEntries;                           /* Number of entries in FDB bucket */
uint32_t fdbSize;                                       /* FDB size */
uint32_t prioMappingTableOffset;                         /* DEM0 offset to priority mapping table */
uint32_t switchPort1UntaggedQueue;                       /* SHARED MEM OFFSET to switch port 1 untagged packet queue number */
uint32_t switchPort2UntaggedQueue;                       /* SHARED MEM OFFSET to switch port 2 untagged packet queue number */
uint32_t expressPremptiveQueueMaskOffset;                /* DMEM0 offset for express queue mask */
} EMAC_ICSSG_SWITCH_FW_CFG;

/**
 * @brief ICSSG Dual MAC firmware configuration structure
 */
typedef struct EMAC_ICSSG_DUALMAC_FW_CFG_S {
uint32_t txHostQueueSize[EMAC_NUM_TRANSMIT_FW_QUEUES];  /* Size of each TX host queue */
} EMAC_ICSSG_DUALMAC_FW_CFG;

/**
 * @brief ICSSG firmware configuration structure for Maxwell PG2.0/J7
 */
typedef struct EMAC_ICSSG_FW_CFG_PG2_S {
    uintptr_t icssgBaseAddr;
    uint32_t rxMgmtFlowStart;
    uint32_t rxPktFlowStart;
    uint32_t bufferPoolHighAddr;                           /* MSMC address high of buffer pool */
    uint32_t bufferPoolLowAddr;                            /* MSMC address low of  buffer pool */
    uint32_t numBufferPool;                                /* NUmber of buffer pools */
    uint32_t bufferPoolSize[EMAC_ICSSG_MAX_NUM_BUFFER_POOLS];
} EMAC_ICSSG_FW_CFG_PG2;

typedef void EMAC_ICSSG_FW_FXN_T(uint32_t portNum, EMAC_ICSSG_FW_CFG_PG2 *pIcssgFwCfg);

typedef struct EMAC_PER_PORT_ICSSG_FW_CFG_S {
    EMAC_FW_APP_CONFIG fwAppCfg;
    struct EMAC_FW_PORT_CFG *pFwPortCfg;
    EMAC_ICSSG_FW_FXN_T *pIcssgFwCfgFxn;
}EMAC_PER_PORT_ICSSG_FW_CFG;


/**
 * @brief definition for get fw config function
 */
typedef int32_t (*EMAC_getFwCfg)(uint32_t port_num, EMAC_PER_PORT_ICSSG_FW_CFG **pFwCfg);

/**
 * @ingroup emaclld_api_structures
 * @brief       SOC specific port configuration structure
 *
 * @details This is used to configure SOC specific configuration
 *          with the emac driver via hwAttrs field of EMAC_OPEN_CONFIG_INFO_T structure.
 */
typedef struct EMAC_PER_PORT_CFG_s{
    int32_t phyAddr;
    /* PHY address */
    int32_t nTxChans;
    uintptr_t                    cpswSSRegsBaseAddr;
    uintptr_t                    mdioRegsBaseAddr;
     /**< base address of MDIO IP subsystem */
    uintptr_t                    icssSharedRamBaseAddr;
    /**< base address of ICSSG shared memory, N/A for CPSW2G */
    uintptr_t                    icssgCfgRegBaseAddr;
    /* base address of ICSSG data ram 0 registers, N/A for CPSW2G */
    uintptr_t                    icssDram0BaseAddr;
    /* base address of ICSSG data ram 1 registers, N/A for CPSW2G */
    uintptr_t                    icssDram1BaseAddr;
    /**< base address of ICSSG shared memory, N/A for CPSW2G */
    EMAC_PER_CHANNEL_CFG_TX     txChannel[EMAC_TX_MAX_CHANNELS_PER_PORT];
    EMAC_PER_CHANNEL_CFG_RX     rxChannel;
    EMAC_PER_CHANNEL_CFG_RX     rxChannelCfgOverPSI;
    EMAC_PER_CHANNEL_CFG_RX     rxChannel2CfgOverPSI;
    EMAC_getFwCfg               getFwCfg;
} EMAC_PER_PORT_CFG;

typedef struct EMAC_HwAttrs_V5_s {
    /*! Per Port configuration */
    EMAC_PER_PORT_CFG portCfg[EMAC_MAX_PORTS];
} EMAC_HwAttrs_V5;

typedef struct EMAC_TX_TS_RESPONSE_s{
    uint32_t lo_txts;
    uint32_t hi_txts;
    uint32_t ts_valid;
    uint32_t ts_id;
} EMAC_TX_TS_RESPONSE;

typedef struct EMAC_TX_QUEUE_CONTEXT_S
{
    uint32_t start_addr;        /* start address of MSMC/descriptor queue */
    uint32_t rd_ptr;            /* Initial read pointer of port/host desciptor queue queue  */
    uint32_t wr_ptr;            /* Initial write pointer of port/host desciptor queue queue  */
    uint32_t end_addr;          /* end address of MSMC/descriptor queue */
    
} EMAC_TX_QUEUE_CONTEXT;


/* Port specficic control block */
typedef struct EMAC_PORT_CB_V5_S
{
    Bool                            master_core_flag;
    /**< True: master core configuration */
    Bool                mdio_flag;
    /**< True: configure MDIO                                               */
    void*                   hwi;                /* Hwi object */
    uint32_t                          num_of_rx_pkt_desc;
    /**< Total number of packet descriptors initialized in the RX queues    */
    uint32_t                          num_of_tx_pkt_desc;
    /**< Total number of packet descriptors initialized in the TX queues    */
    uint32_t                          num_of_rxtx_pkt_desc;
    /**< Total number of packet descriptors initialized in both TX and RX queues    */
    uint32_t                          max_pkt_size;
    /**< MAC address for the port     */
    EMAC_MAC_ADDR_T               macaddr_cfg;
    /**<  MAC address configuration */
    Bool                            loop_back;
    /**< True: packets loop back                                            */
    /**< RX packet call back function */
    EMAC_RX_PKT_CALLBACK_FN_T*      rx_pkt_cb;

    /**< Allocate packet call back function */
    EMAC_ALLOC_PKT_CALLBACK_FN_T*   alloc_pkt_cb;
    /**< Free packet call back function */
    EMAC_FREE_PKT_CALLBACK_FN_T*    free_pkt_cb;
    /**< FW config response callback function */
    EMAC_RX_MGMT_CALLBACK_FN_T*      rx_mgmt_response_cb;
    /**< timestamp response callback function */
    EMAC_TX_TS_CALLBACK_FN_T*       tx_ts_cb;
    EMAC_getFwCfg                   getFwCfg;
    EMAC_CPPI_DESC_T         *txReadyDescs[EMAC_TX_MAX_CHANNELS_PER_PORT];
    /*Rx Semaphore Handle for the emac instance.Required for receiving packets */
    void                      *rxSemaphoreHandle[EMAC_MAX_RX_SUBCHAN_PER_CHAN];
    /*Rx Semaphore Handle for the emac instance.Required for receiving config responses from ICSSGFW */
    void                      *rxMgmtSemaphoreHandle[EMAC_MAX_RX_SUBCHAN_PER_CHAN];
    /*Rx Semaphore Handle for the emac instance.Required for receiving config responses from ICSSGFW */
    HwiP_Handle rxCmplHwi[EMAC_MAX_RX_SUBCHAN_PER_CHAN];
    int32_t                 emacState;
    uint32_t               mode_of_operation;
    EMAC_MDIO_DEVICE_T     MdioDev;
    Udma_DrvHandle udmaHandle;

    EMAC_CHANNEL_RX_T rxPktCh;
    EMAC_CHANNEL_RX_T rxMgmtCh;
    EMAC_CHANNEL_RX_T rxMgmtCh2;
    Udma_ChHandle txChHandle[EMAC_TX_MAX_CHANNELS_PER_PORT];
    uint32_t numTxChans;
    uintptr_t icssSharedRamBaseAddr;
    uintptr_t icssgCfgRegBaseAddr;
    uintptr_t icssDram0BaseAddr;
    uintptr_t icssDram1BaseAddr;
    int32_t     ioctlInProgress;
    EMAC_POLLING_TABLE_T pollTable;
    CSL_Xge_cpswRegs *hCpswRegs;
    CSL_AleRegs *hCpswAleRegs;
    uint32_t pgVersion;
    uint32_t            ioctlType;
} EMAC_PORT_CB_V5_T;

#define EMAC_ICSSG_IOCTL_TYPE_R30_OVER_DMEM ((uint32_t)1U)

/* Port specficic control block for IOCTL interface*/
typedef struct EMAC_IOCTL_CB_V5_S
{
    volatile int32_t    ioctlCount;
    EMAC_IOCTL_CMD_T    *pCmd1Icssg;
    uint8_t             sequenceNumber;
    bool                internalIoctl;
    bool                ioctlInProgress;
} EMAC_IOCTL_CB_V5_T;

/* icssg instance macros */
#define EMAC_ICSSG0_INSTANCE ((uint32_t)0U)
#define EMAC_ICSSG1_INSTANCE ((uint32_t)1U)
#define EMAC_ICSSG2_INSTANCE ((uint32_t)2U)

typedef struct EMAC_PORT_INFO_S {
    int32_t icssgInst;
    int32_t swithPortNum;
    int32_t portNum1;
    int32_t portNum2;
    int32_t pollPortNum;
    int32_t ioctlPortNum;
    int32_t sendPortNum;
    const EMAC_FxnTable *fxnTable;
} EMAC_PORT_INFO_T;

typedef struct EMAC_MCB_V5_S {
    /* Port specific control block */
    EMAC_PORT_CB_V5_T          port_cb[EMAC_MAX_NUM_EMAC_PORTS];
    /* Switch specific control block */
    EMAC_IOCTL_CB_V5_T   ioctl_cb;
    UTIL_TRACE_CB_T* drv_trace_cb;
} EMAC_MCB_V5_T;

#ifdef __cplusplus
}
#endif

#endif  /* EMAC_DRV_V5_H */

