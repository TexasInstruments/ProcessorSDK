
#ifndef _TESTCONN_H
#define _TESTCONN_H

/******************************************************************************
 * FILE PURPOSE: Definitions and prototypes for SA Unit Test.
 ******************************************************************************
 * FILE NAME:   testconn.h
 *
 * DESCRIPTION: Contains SA channel connection related definitions and function 
 *              prototypes for SA Unit Test.
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
#include "unittest.h" 
#include "salldsim/salldsim.h"
#if defined(SOC_AM64X)
#include "ti/csl/csl_pktdma.h"
#include "ti/csl/csl_lcdma_ringacc.h"
#endif

#define ETH_TYPE_IP             0x0800
#define ETH_TYPE_IPV6           0x86dd

/* Protocol field values (IPV4) / next header (IPV6) */
#define IP_PROTO_IPV6_HOP_BY_HOP    0   /* IPv6 extension header - hop by hop */
#define IP_PROTO_ICMP               1
#define IP_PROTO_IGMP               2 
#define IP_PROTO_GGP                3 
#define IP_PROTO_IP_IN_IP           4   /* IP tunneling */
#define IP_PROTO_TCP                6 
#define IP_PROTO_EGP                8 
#define IP_PROTO_UDP               17 
#define IP_PROTO_IPV6_IN_IP        41   /* IP tunneling */
#define IP_PROTO_IPV6_ROUTE        43   /* IPv6 extension header - route */
#define IP_PROTO_IPV6_FRAG         44   /* IPv6 extension header - fragmentation */
#define IP_PROTO_GRE               47
#define IP_PROTO_ESP               50   /* Encapsulating security payload */
#define IP_PROTO_AUTH              51   /* Authentication header (ipv4) */
#define IP_PROTO_IPV6_NO_NEXT      59   /* IPv6 extention header - no next header      */
#define IP_PROTO_IPV6_DEST_OPT     60   /* IPv6 extension header - destination options */
#define IP_PROTO_IPCOMP           108   /* IP Compression Protocol */


/* 
 * Protocol-specific Header information 
 */

/* MAC Header */ 
typedef struct sauMac_s 
{
    uint8_t     dest[6];        /* destination address */
    uint8_t     src[6];         /* source address */
    uint16_t    ethType;
    t2Handles_t *t2Hdl;         /* Pointer to the T2 handle */

} sauMac_t;

/* IPSEC Header */
typedef struct sauIpsec_t
{
    uint8_t     type;
    #define SAU_IPSEC_TYPE_ESP  0
    #define SAU_IPSEC_TYPE_AH   1
    #define SAU_IPSEC_TYPE_ESP_NAT_T    2
    uint8_t     macSize;
    uint8_t     ahIvSize;
    uint8_t     hdrSize;
    uint32_t    spi;
    salldSimChannel_t *pChan;   /* Pointer to the salld channel */ 
      
} sauIpsec_t;

/* IP Header */
typedef struct sauIp_s
{
    uint8_t   type;
    #define SAU_IP_TYPE_IP4     0
    #define SAU_IP_TYPE_IP6     1
    uint8_t   nextHdr;
    uint8_t   dest[16];
    uint8_t   src[16];
    uint8_t   index;
    sauIpsec_t* ipsecHandle;    /* Pointer to the IPSEC Handle */
                                /* NULL: No IPSEC */
    t2Handles_t *t3Hdl;         /* Pointer to the T3 handle */
    t2Handles_t *t3Hdl2;        /* Pointer to the second T3 handle for NAT-T entry only */
    
} sauIp_t;

/* SRTP Header */
typedef struct sauSrtp_s
{
    uint16_t      seqNum;         
    uint32_t      ssrc;
    salldSimChannel_t *pChan;   /* Pointer to the salld channel */   
} sauSrtp_t; 

/* SRTCP Header */
typedef struct sauSrtcp_s
{
    uint32_t      ssrc;
    salldSimChannel_t *pChan;   /* Pointer to the salld channel */   
} sauSrtcp_t; 

/* UDP header */
typedef struct sauUdp_s
{
    uint16_t    srcPort;
    uint16_t    destPort;
    sauSrtp_t   *srtpHandle;    /* Pointer to a SRTP channel NULl: No SRTP */ 
    sauSrtcp_t  *srtcpHandle;   /* Pointer to a SRTCP channel: NUll: No SRTCP */ 
    t4Handles_t *t4Hdl;         /* Pointer to the T4 handle */
} sauUdp_t; 

/* 3GPP Air Ciphering header */
typedef struct sauAc_s
{
    Sa_AcPduType_e pduType;
    Sa_CipherMode_e cipherMode;
    uint16_t    useIV;         /* For LTE CP, iv overwritten test only */
    uint16_t    ctrlBitMap;       
    uint16_t    seqNum;
    uint8_t     ivSize;        /* For GSM only */
    uint8_t     seqNumSize;    /* For WCDMA UMD and AMD only */
    uint8_t     authHdrSize;   /* FOr LTE Control packet only */
    uint32_t    count32;       /* For GSM Key generation and LTE CP only */
    uint32_t    count32_2;     /* For LTE CP, iv overwritten test only (simulate IV overwritten for control base channel */ 
    uint32_t    iv[4];         /* For LTE CP, iv overwritten test only */
    salldSimChannel_t *pChan;   /* Pointer to the salld channel */ 
      
} sauAc_t; 

#define SA_PROTO_AC_CTRL_FLAG_COUNTC_INSERT     0x0001
#define SA_PROTO_AC_CTRL_FLAG_IV_TEST           0x0002

/* GSM Key straem related definitions */
#define SAU_KGCORE_KS_SIZE_GSM_A53     29     /* Key stream size in bytes */
#define SAU_KGCORE_KB_SIZE_GSM_A53     114    /* Key block size in bits */
#define SAU_KGCORE_KS_SIZE_ECSD_A53    87     /* Key stream size in bytes */
#define SAU_KGCORE_KB_SIZE_ECSD_A53    348    /* Key block size in bits */
#define SAU_KGCORE_COUNT_MASK_GSM_A53  0x003FFFFF
#define SAU_KGCORE_COUNT_MASK_ECSD_A53 0x003FFFFF
#define SAU_KGCORE_CA_GSM_A53   0x0F
#define SAU_KGCORE_CA_ECSD_A53  0xF0
#define SAU_KGCORE_CA_GEA3      0xFF
#define SAU_KGCORE_CA_F8        0x00
#define SAU_KGCORE_CA_OFFSET    5
#define SAU_KGCORE_CB_OFFSET    4
#define SAU_KGCORE_CE_OFFSET    6


/* Data Mode header */
typedef struct sauDataMode_s
{
    uint8_t   aad[16];
    uint8_t   encIV[16];
    uint8_t   authIV[16];
    uint16_t  aadSize;
    uint16_t  encIvSize;
    uint16_t  authIvSize; 
    uint32_t  seqNum;           
    salldSimChannel_t *pChan;   /* Pointer to the salld channel */   
    /*command label related parameters */
    int     fCmdLbAvail;
    Sa_CmdLbUpdateInfo_t   cmdLbUpdate;
    uint16_t  cmdLbSize;
    uint32_t  cmdLbBuf[sa_MAX_CMDLB_SIZE/4]; 
} sauDataMode_t;  

/* Define Protocol Type */
enum  {
  SAU_PROTO_MAC,
  SAU_PROTO_IP,
  SAU_PROTO_UDP,
  SAU_PROTO_AC,
  SAU_PROTO_DATAMODE
};

#define SAU_MAX_HDR_SIZE       ((uint32_t) 40U)
typedef struct sauHdrHandle_s
{
    struct sauHdrHandle_s *upHdl;     /* pointer to the upper layer Handle */
    uint8_t   hdrLen;
    uint8_t   hdr[SAU_MAX_HDR_SIZE];
    int       hdrType;
    union   {
        sauMac_t        mac;
        sauIp_t         ip;
        sauUdp_t        udp;
        sauAc_t         ac;
        sauDataMode_t   data;
    } u;
    
} sauHdrHandle_t;

#define SA_PKT_HDR_MARGIN                 32            /* Reserved Margin for AH/ESP Header */
#define SA_PKT_TAIL_MARGIN                64            /* Reserved Margin for ESP Tail, ESP Hash and SRTP Hash */  
#define SA_PKT_MARGIN                     (SA_PKT_HDR_MARGIN + SA_PKT_TAIL_MARGIN)

#define SA_MAX_HDR_HANDLES_PER_CONN        5

#define SA_CONN_INDEX_NONE                 0xFF          /* SRTP/IPSEC channel does not exist */
#define SA_CONN_OFFSET_NONE                0xFF          /* Corresponding protocol does not exist */ 

#if defined(NSS_LITE2)
#define SA_CONN_PROMOTE_CHANNEL                      2
#define SA_CONN_DEMOTE_CHANNEL                       1
#define SA_CONN_NO_PROMOTE_DEMOTE_CHANNEL            0
#endif

typedef struct sauConnHandle_s
{
    uint16_t  ID;             /* Connection ID */
    uint8_t   hdrLen;
    uint8_t   hdr[256];
    uint8_t   ipOffset[3];    /* offset to each IPs -1: no IP */
    uint8_t   ipsecOffset;    /* offset to each IPs -1: no IP */
    uint8_t   udpOffset;      /* 0xFF: No UDP */
    uint8_t   rtpOffset;      /* 0xFF: no RTP */
    uint8_t   srtpIndex;      /* 0xFF: SRTP channel does not exist */
    uint8_t   srtcpIndex;     /* 0xFF: SRTCP channel does not exist */
    uint8_t   ipsecIndex;     /* 0xFF: IPSEC channel does not exist */
    uint8_t   numHandles;     /* number of header handles */
    uint8_t   numIp;          /* number of IPs */
    sauHdrHandle_t *pHdrHdl[SA_MAX_HDR_HANDLES_PER_CONN];
}   sauConnHandle_t;

#if defined(SAU_PRMOTE_DEMOTE_TEST)
#define SA_TEST_MAX_CONNS            4
#define SA_TEST_MAX_HDR_HANDLES      4
extern  sauHdrHandle_t  testHdrHandlesSecure[SA_TEST_MAX_HDR_HANDLES];
extern  sauConnHandle_t testConnHandlesSecure[SA_TEST_MAX_CONNS];
#else
#define SA_TEST_MAX_CONNS            100
#define SA_TEST_MAX_HDR_HANDLES      200
#endif
#define SA_TEST_MAX_IPSEC_CHANS       20
#define SA_TEST_MAX_SRTP_CHANS       SA_TEST_MAX_CONNS 
#define SA_TEST_MAX_SRTCP_CHANS      SA_TEST_MAX_CONNS 

extern  sauHdrHandle_t  testHdrHandles[SA_TEST_MAX_HDR_HANDLES];
extern  sauConnHandle_t testConnHandles[SA_TEST_MAX_CONNS];
extern  sauSrtp_t       testSrtpHandles[SA_TEST_MAX_SRTP_CHANS];
extern  sauSrtcp_t      testSrtcpHandles[SA_TEST_MAX_SRTCP_CHANS];
extern  sauIpsec_t      testIpsecHandles[SA_TEST_MAX_IPSEC_CHANS];
extern  int             numHdrHandles, numConnHandles, numSrtpHandles, numSrtcpHandles, numIpsecHandles;
/*
 * Assume that all protocol and connection related handles will be requested sequentially
 * and will last the entire test.
 * All the buffers will be initialized prior to each test.
 * Use simple MACROs for handle allocations
 */
#if defined(SAU_PRMOTE_DEMOTE_TEST)
#define TEST_ALLOC_HDR_HANDLE_SECURE()     (numHdrHandlesSecure< SA_TEST_MAX_HDR_HANDLES)?      \
                                     &testHdrHandlesSecure[numHdrHandlesSecure++]:NULL
#define TEST_ALLOC_CONN_HANDLE_SECURE()    (numConnHandlesSecure< SA_TEST_MAX_CONNS)?           \
                                     &testConnHandlesSecure[numConnHandlesSecure++]:NULL  
#endif
#define TEST_ALLOC_HDR_HANDLE()     (numHdrHandles < SA_TEST_MAX_HDR_HANDLES)?      \
                                     &testHdrHandles[numHdrHandles++]:NULL
#define TEST_ALLOC_CONN_HANDLE()    (numConnHandles < SA_TEST_MAX_CONNS)?           \
                                     &testConnHandles[numConnHandles++]:NULL  
#define TEST_ALLOC_IPSEC_HANDLE()   (numIpsecHandles < SA_TEST_MAX_IPSEC_CHANS)?    \
                                     &testIpsecHandles[numIpsecHandles++]:NULL  
#define TEST_ALLOC_SRTP_HANDLE()    (numIpsecHandles < SA_TEST_MAX_SRTP_CHANS)?     \
                                     &testSrtpHandles[numSrtpHandles++]:NULL  
#define TEST_ALLOC_SRTCP_HANDLE()   (numIpsecHandles < SA_TEST_MAX_SRTCP_CHANS)?     \
                                     &testSrtcpHandles[numSrtcpHandles++]:NULL  
                                     
/* Protocol Base Addresses */      
extern uint8_t testMacSrcAddr[6];
extern uint8_t testMacDestAddr[6];
extern uint8_t testIpSrcAddr[16];
extern uint8_t testIpDestAddr[16];
extern uint8_t testIpinSrcAddr[16];
extern uint8_t testIpinDestAddr[16];
                                
#define SA_TEST_UDP_PORT_BASE   0x8000
#define SA_TEST_IPSEC_SPI_BASE  0x44440000   

#define SA_TEST_GET_UDP_PORT(i)     (SA_TEST_UDP_PORT_BASE + (i)*2)
#define SA_TEST_GET_SPI(i)          (SA_TEST_IPSEC_SPI_BASE + (i)*16)    

/* Replay test sequence arrays */
extern uint32_t testSeqNum_64[40]; 
extern uint32_t testSeqNum_128[40]; 
extern uint32_t testSeqNum_160[40]; 
                                     
/******************************************************************************
 * DATA DEFINITION:  SA Unit Test Payload Type
 ******************************************************************************
 * DESCRIPTION:  Defines the payload types
 ******************************************************************************/
typedef enum sauPayloadType_s
{
    SAU_PAYLOAD_CONST,
    SAU_PAYLOAD_INC8,
    SAU_PAYLOAD_DEC8,
    SAU_PAYLOAD_RANDOM
}   sauPayloadType_e;                                     
                                     
                                       
/******************************************************************************
 * DATA DEFINITION:  SA Unit Test IPSEC Configuration
 ******************************************************************************
 * DESCRIPTION:  Defines the configuration parameters for an IPSEC channel
 *
 *  Note: 1. To support the further growth, there are more IPSEC channel 
 *           configuration parameters defined by the SA LLD. However, some of the 
 *           parameters will be fixed by the encryption mode and/or authentication 
 *           mode per RFCs
 *
 *        2. The SA LLD supports asymetric Tx/Rx configuration. In the unit test,
 *           we shall configure the tx/rx channel symetrically, so that we can
 *           generate the tx traffic and receive the same traffic to verify both
 *           Tx/Rx operation. 
 *
 ******************************************************************************/
typedef struct sauIpsecConfig_s
{
    Sa_CipherMode_e     cipherMode;  /* NULL, AES_CTR, AES_CBC, DES_CBC, GCM, CCM */
    Sa_AuthMode_e       authMode;    /* GMAC, CMAC, HMAC_MD5, HMAC_SHA1, HMAC_SHA2_224, HMAC_SHA2_256 */
    uint16_t            encKeySize;  /* (16/24/32) with some restrictions per RFCs */
    uint16_t            macKeySize;  /* (16/20/24/32) with some restrictions per RFCs */
    uint16_t            macSize;     /* (8/12/16) with some restrictions per RFCs */
    Bool                esn;         /* TRUE: ESN on
                                      * FALSE: ESN off
                                      * Should be set to TRUE for 50% test cases
                                      */
    uint16_t            replayWindowSize; /* 64/128 */                                           
 
} sauIpsecConfig_t;

/******************************************************************************
 * DATA DEFINITION:  SA Unit Test SRTP Configuration
 ******************************************************************************
 * DESCRIPTION:  Defines the configuration parameters for a SRTP channel
 *
 *  Note: 1. To support the further growth, there are more SRTP channel 
 *           configuration parameters defined by the SA LLD. However, some of the 
 *           parameters will be fixed by the encryption mode and/or authentication 
 *           mode per RFCs
 *
 *        2. The SA LLD supports asymetric Tx/Rx configuration. In the unit test,
 *           we shall configure the tx/rx channel symetrically, so that we can
 *           generate the tx traffic and receive the same traffic to verify both
 *           Tx/Rx operation. 
 *
 ******************************************************************************/
typedef struct sauSrtpConfig_s
{
    Sa_CipherMode_e     cipherMode;  /* NULL, AES_CTR, AES_F8 */
    Sa_AuthMode_e       authMode;    /* NULL, HMAC_MD5, HMAC_SHA1 */
    uint16_t              macSize;     /* (4/10) with some restrictions per RFCs */
    uint16_t              replayWindowSize; /* 64 */                                           
    uint16_t              derivRate;        /**< Specify the key derivation rate in n of 2^n format */
    uint16_t              mkiSize;          /* (0/2/4) */
    Bool                fromTo;           /* TRUE: From-To Key
                                           * FALSE: MKI Key
                                           * Should be set to TRUE for 50% test cases
                                           */
    uint32_t            keyLifeTime;      /**< Specify the maximum number of allowed for the master */
    uint32_t            fromEsn;          /**< Specify the starting 48-bit extended sequence number */
    uint32_t            toEsn;            /**< Specify the lastg 48-bit extended sequence number
                                               of the specified From-To keys */
    
} sauSrtpConfig_t;


/******************************************************************************
 * DATA DEFINITION:  SA Unit Test Air Ciphering Configuration
 ******************************************************************************
 * DESCRIPTION:  Defines the configuration parameters for a 3GPP Air Ciphering
 *               channel
 *
 *  Note: 1. To support the further growth, there are more 3GPP Air Ciphering channel 
 *           configuration parameters defined by the SA LLD. However, some of the 
 *           parameters will be fixed by the encryption mode and/or authentication 
 *           mode per RFCs
 *
 *        2. The SA LLD supports asymetric Tx/Rx configuration. In the unit test,
 *           we shall configure the tx/rx channel symetrically, so that we can
 *           generate the tx traffic and receive the same traffic to verify both
 *           Tx/Rx operation. 
 *
 ******************************************************************************/
typedef struct sauAcConfig_s
{
    Sa_CipherMode_e     cipherMode;  /* NULL, AES_CTR, KASUMI_F8, SNOW3G_F8 */
    Sa_AuthMode_e       authMode;    /* NULL, AES_CBC, KASUMI_F9 */
    Sa_AcPduType_e      pduType;
    /*
     * The Direction is actually fixed since the SA LLD is running at RNC
     * To Air Traffic (From Network): Dir: 1 (RNC to UE)
     * From Air Traffic (To Network): Dir: 0 (UE to RNC)
     *
     * Note: The configuration parameter upLink defined below is introduced so that
     *       we can verify the To-Air/From-Air traffic as a pair
     */
    Bool                upLink;      /* Dir 0:UE to RNC(uplink)
                                            1:RNC to UE(downlink) */
    /* Key to be kept in internal memory*/
    Bool                intKey;      /* 1: Key in internal memory 
                                        0: Key in external memory (DDR) */												
                                        												
    Bool                countCPresent;  /* CountC is maintained by application in stead of SASS */
                                        /* note: It is applicable to LTE to-air traffic only since
                                                 it is the only scenario that count-C is maintained
                                                 at SASS */                                        
} sauAcConfig_t;

#define SAU_DM_CFG_CTRL_NONE       (FALSE)
#define SAU_DM_CFG_SEL_AC_ENGINE    1
#define SAU_DM_CFG_PROMOTE          2
#define SAU_DM_CFG_DEMOTE           4
#define SAU_DM_CFG_NONSECURE_CRYPTO 8
/******************************************************************************
 * DATA DEFINITION:  SA Unit Test Data Mode Configuration
 ******************************************************************************
 * DESCRIPTION:  Defines the configuration parameters for a Data Mode
 *               channel
 *
 *  Note: 1. To support the further growth, there are more Data Mode channel 
 *           configuration parameters defined by the SA LLD. However, some of the 
 *           parameters will be fixed by the encryption mode and/or authentication 
 *           mode per RFCs
 *
 ******************************************************************************/
typedef struct sauDataModeConfig_s
{
    Sa_CipherMode_e     cipherMode;  /* All */
    Sa_AuthMode_e       authMode;    /* All */
    uint16_t            encKeySize;  /* (16/24/32) */
    uint16_t            macKeySize;  /* (16/20/24/32) */
    uint16_t            macSize;     /* (8/12/16) */
    uint16_t            aadSize;     /* (8/12) */  
    uint16_t            ivSize;      /* iv size */    
    uint16_t            saltSize;    /* salt size */    
    Bool                enc;         /* TRUE:encryption; FALSE: decryption */
    Bool                enc1st;      /* TRUE:encryption first; FALSE: authentication first */
#if defined (NSS_LITE2)
    uint32_t            ctrlBitMap;  /* Data mode channel configurations control for sa2ul */
#else
    uint32_t            selACEng;    /* Bit 0: 1: Select Air Cipher Engine for algorithms */
#endif
} sauDataModeConfig_t;

/******************************************************************************
 * DATA DEFINITION:  SA Unit Test Packet Description
 ******************************************************************************
 * DESCRIPTION:  Defines a test packet
 *
 ******************************************************************************/
#define SAU_MAX_TEST_PKT_SIZE   512 
typedef struct sauTestPkt_s
{
    uint32_t  pktSize;        
    uint8_t   pkt[SAU_MAX_TEST_PKT_SIZE];
} sauTestPkt_t;

#define ONE_SEGMENT 0
#define TWO_SEGMENT 1
#define THREE_SEGMENT 2

void sauConnInit(void);
sauHdrHandle_t* sauCreateMac (uint8_t* srcAddr, uint8_t* destAddr, uint16_t ethType);
sauHdrHandle_t* sauCreateIp (sauHdrHandle_t *pHdl, uint8_t* srcAddr, uint8_t* destAddr, 
                             uint8_t nextHdr, Bool ipv4);
sauHdrHandle_t* sauCreateUdp (sauHdrHandle_t *pHdl, uint16_t srcPort, uint16_t destPort);
sauHdrHandle_t* sauCreateAc (sauHdrHandle_t *pHdl, uint16_t initSeqNum, sauAcConfig_t* pCfg);
sauHdrHandle_t* sauCreateDm (sauHdrHandle_t *pHdl, uint16_t hdrSize, sauDataModeConfig_t* pCfg);
Bool sauCreateIpsec (sauHdrHandle_t *pIpHdl, uint32_t spi, uint8_t type, sauIpsecConfig_t* pCfg);
Bool sauCreateSrtp  (sauHdrHandle_t *pUdpHdl, uint16_t initSeqNum, uint32_t ssrc, sauSrtpConfig_t* pCfg);
Bool sauCreateSrtcp (sauHdrHandle_t *pUdpHdl, uint32_t initSeqNum, uint32_t ssrc, sauSrtpConfig_t* pCfg);
void sauUpdateSrtp (sauHdrHandle_t *pUdpHdl, uint16_t seqNum, uint32_t ssrc);
sauConnHandle_t* sauCreateConnection(tFramework_t *tf, saTest_t *pat, sauHdrHandle_t *pTail);
void sauGenPayload(sauPayloadType_e type, uint8_t initValue, uint16_t len, uint8_t* buf);

#ifndef NSS_LITE2
Cppi_HostDesc* sauGenTxPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen, uint8_t* payload, Bool corrupt);
Cppi_HostDesc* sauGenTxMultiSegmentPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen, uint8_t* payload, Bool corrupt, uint8_t mode);
void sauProcRxMultiSgmtPkt(int connIndex, Cppi_HostDesc* hd);
void sauProcRxPkt(int connIndex, Cppi_HostDesc* hd);
#endif

void sauConnPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                    uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate);
void sauConnSrtcpPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                        uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate);
void sauConnMultiSegmentPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                    uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate, uint8_t mode);
void sauConnMultiSegmentSrtcpPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                                     uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate, uint8_t mode);
void sauMultiPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                     uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate);
void sauConnAcPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                      uint16_t initLen, uint16_t step, sauPayloadType_e payloadType);
void sauConnDmPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                      uint16_t initLen, uint16_t step, sauPayloadType_e payloadType);
void sauConnTputDmPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts);
void sauConnPromoteDemoteDmPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts);
void sauConnReplayTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,  uint16_t* numTxPkts,
                       uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint32_t** ppSeqNum, int fSoftware);
void sauConnAcKeyGenTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                         uint16_t initLen, uint16_t step);
void sauReregegisterUdp(void* pHdl);
                      
#endif /* _TESTCONN_H */
