/*
 *
 * Copyright (C) 2010-2013 Texas Instruments Incorporated - http://www.ti.com/
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

#include "unittest.h"
#include "testconn.h"
#include "salldsim/salldcfg.h"


/*
 * Declare protocol and connection related handles
 */
sauHdrHandle_t  testHdrHandles[SA_TEST_MAX_HDR_HANDLES];
sauConnHandle_t testConnHandles[SA_TEST_MAX_CONNS];
sauSrtp_t       testSrtpHandles[SA_TEST_MAX_SRTP_CHANS];
sauSrtcp_t      testSrtcpHandles[SA_TEST_MAX_SRTCP_CHANS];
sauIpsec_t      testIpsecHandles[SA_TEST_MAX_IPSEC_CHANS];
int             numHdrHandles, numConnHandles, numSrtpHandles, numSrtcpHandles, numIpsecHandles;

static uint8_t  testCmdLbBuf[sa_MAX_CMDLB_SIZE];
static uint8_t  testCmdLbBuf2[sa_MAX_CMDLB_SIZE];
static uint8_t sap_pkt_test_buf[0x1000];

static uint8_t testIpV4Template[20] =
    {
        0x45, 0x00, 0x00, 0x9c,   /* IP version, services, total length */
        0x00, 0x00, 0x00, 0x00,   /* IP ID, flags, fragment offset */
        0x05, 0x32, 0xff, 0xf3,   /* IP ttl, protocol (UDP), header checksum */
        0x9e, 0xda, 0x6d, 0x14,   /* Source IP address */
        0x14, 0x15, 0x16, 0x17,   /* Destination IP address*/
    };

static uint8_t testIpV6Template[40] =
    {
        0x60, 0x00, 0x00, 0x10,   /* IP version, DS (8-bit), Flow (20 bit) */
        0x00, 0x00, 0x17, 0x05,   /* IP length (16-bit), Next Hdr (8 bit), HOP Limit (8-bit) */
        0x00, 0x00, 0x00, 0x00,   /* Source IP Address */
        0x00, 0x00, 0x00, 0x00,   /* Source IP Address */
        0x00, 0x00, 0x00, 0x00,   /* Source IP Address */
        0x00, 0x00, 0x00, 0x00,   /* Source IP Address */
        0x00, 0x00, 0x00, 0x00,   /* Destination IP address */
        0x00, 0x00, 0x00, 0x00,   /* Destination IP address */
        0x00, 0x00, 0x00, 0x00,   /* Destination IP address */
        0x00, 0x00, 0x00, 0x00    /* Destination IP address */
    };

/* Protocol Base Addresses */
uint8_t testMacSrcAddr[6]  = {0x00, 0xe0, 0xa6, 0x66, 0x57, 0x04};
uint8_t testMacDestAddr[6] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
uint8_t testIpSrcAddr[16]  = {0x9e, 0xda, 0x6d, 0x14, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0};
uint8_t testIpDestAddr[16] = {0x14, 0x15, 0x16, 0x17, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0};
uint8_t testIpinSrcAddr[16] = {0x9e, 0xda, 0x6d, 0x20, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0};
uint8_t testIpinDestAddr[16] = {0x04, 0x05, 0x06, 0x07, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0};
                              
static uint8_t testAcIv[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                             0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};         

static uint8_t testEncIV[16] = {0x00, 0x00, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                              0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};    
                             
static uint8_t testAuthIV[16] = {0x00, 0x00, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
                               0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
                               
static uint8_t testAad[12] = {0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0, 0, 0, 0};  

#define SAU_TXPKT_TO_HOST   1                                                               
     
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/*******************************************************************************
 *  Function: Create a MAC channel  
 *******************************************************************************
 *  DESCRIPTION:  Create a MAC Channel
 *
 *  Note: we only support DIX format
 ******************************************************************************/
sauHdrHandle_t* sauCreateMac (uint8_t* srcAddr, uint8_t* destAddr, uint16_t ethType)
{
    sauMac_t        *pMacHdr;
    sauHdrHandle_t  *pHdrHandle;
    
    pHdrHandle = TEST_ALLOC_HDR_HANDLE();   
    if (pHdrHandle == NULL)
    {
        salld_sim_print("sauCreateMac: Hdr Handle is not available!\n");
        return (NULL);
    }    
    
    /* Fill the header information */
    pMacHdr = &pHdrHandle->u.mac;
    
    pMacHdr->t2Hdl = TEST_ALLOC_L2_HANDLE();
    
    if (pMacHdr->t2Hdl == NULL)
    {
        /* Free the protocol header handle */
        numHdrHandles--;
        salld_sim_print("sauCreateMac: L2 Handle is not available!\n");
        return (NULL);
    }
    
    memcpy(pMacHdr->dest, destAddr, 6);
    memcpy(pMacHdr->src, srcAddr, 6);
    pMacHdr->ethType = ethType;
    
    pHdrHandle->upHdl = NULL;   /* There is no upper layer for MAC */
    pHdrHandle->hdrLen = 14;
    pHdrHandle->hdrType = SAU_PROTO_MAC;
    memcpy(&pHdrHandle->hdr[0], destAddr, 6);
    memcpy(&pHdrHandle->hdr[6], srcAddr, 6);
    pktWrite16bits_m((uint8_t *)pHdrHandle->hdr, 12, ethType);
    
    return(pHdrHandle);
}

/*******************************************************************************
 *  Function: Create an IP channel  
 *******************************************************************************
 *  DESCRIPTION:  Create a IP Channel
 *
 ******************************************************************************/
sauHdrHandle_t* sauCreateIp (sauHdrHandle_t *pHdl, uint8_t* srcAddr, uint8_t* destAddr, 
                             uint8_t nextHdr, Bool ipv4)
{
    sauHdrHandle_t *pHdrHandle;
    sauIp_t         *pIpHdr;
    
    pHdrHandle = TEST_ALLOC_HDR_HANDLE();
    if (pHdrHandle == NULL)
    {
        salld_sim_print("sauCreateIp: Hdr Handle is not available!\n");
        return (NULL);
    }    
    
    /* clear the Handle */
    memset(pHdrHandle, 0, sizeof(sauHdrHandle_t));
    
    /* Fill the header information */
    pIpHdr = &pHdrHandle->u.ip;
    
    pIpHdr->t3Hdl = TEST_ALLOC_L3_HANDLE();
    
    if (pIpHdr->t3Hdl == NULL)
    {
        /* Free the protocol header handle */
        numHdrHandles--;
        salld_sim_print("sauCreateIp: L3 Handle is not available!\n");
        return (NULL);
    }
    
    pHdrHandle->hdrType = SAU_PROTO_IP;
    pHdrHandle->upHdl = pHdl;
    
    pIpHdr->nextHdr = nextHdr;
    
    if (ipv4)
    {
        memcpy(pIpHdr->dest, destAddr, 4);
        memcpy(pIpHdr->src, srcAddr, 4);
        pIpHdr->type = SAU_IP_TYPE_IP4;
        pHdrHandle->hdrLen = 20;
        memcpy(&pHdrHandle->hdr[0], testIpV4Template, 12);         
        memcpy(&pHdrHandle->hdr[12], srcAddr, 4);
        memcpy(&pHdrHandle->hdr[16], destAddr, 4);
        pHdrHandle->hdr[9] = nextHdr;
    }    
    else
    {
        memcpy(pIpHdr->dest, destAddr, 16);
        memcpy(pIpHdr->src, srcAddr, 16);
        pIpHdr->type = SAU_IP_TYPE_IP6;
        pHdrHandle->hdrLen = 40;
        memcpy(&pHdrHandle->hdr[0], testIpV6Template, 8);         
        memcpy(&pHdrHandle->hdr[8], srcAddr, 16);
        memcpy(&pHdrHandle->hdr[24], destAddr, 16);
        pHdrHandle->hdr[6] = nextHdr;
    }

    return(pHdrHandle);
}

/*******************************************************************************
 *  Function: Create an UDP channel  
 *******************************************************************************
 *  DESCRIPTION:  Create an UDP Channel
 *
 ******************************************************************************/
sauHdrHandle_t* sauCreateUdp (sauHdrHandle_t *pHdl, uint16_t srcPort, uint16_t destPort)
{
    sauHdrHandle_t *pHdrHandle = TEST_ALLOC_HDR_HANDLE();
    sauUdp_t       *pUdpHdr;
    
    if (pHdrHandle == NULL)
    {
        salld_sim_print("sauCreateUdp: Hdr Handle is not available!\n");
        return (NULL);
    }    
    
    /* Fill the header information */
    pUdpHdr = &pHdrHandle->u.udp;
    
    pUdpHdr->t4Hdl = TEST_ALLOC_L4_HANDLE();
    
    if (pUdpHdr->t4Hdl == NULL)
    {
        /* Free the protocol header handle */
        numHdrHandles--;
        salld_sim_print("sauCreateUdp: L4 Handle is not available!\n");
        return (NULL);
    }
    
    pUdpHdr->srcPort = srcPort;
    pUdpHdr->destPort = destPort;
    pUdpHdr->srtpHandle = NULL;
    pUdpHdr->srtcpHandle = NULL;
    
    
    pHdrHandle->upHdl = pHdl;   
    pHdrHandle->hdrLen = 8;
    pHdrHandle->hdrType = SAU_PROTO_UDP;
    pktWrite16bits_m((uint8_t *)pHdrHandle->hdr, 0, srcPort);
    pktWrite16bits_m((uint8_t *)pHdrHandle->hdr, 2, destPort);
    pktWrite16bits_m((uint8_t *)pHdrHandle->hdr, 6, 0);         /* checksum */
    
    return(pHdrHandle);
}

/*******************************************************************************
 *  Function: Create an IPSEC channel  
 *******************************************************************************
 *  DESCRIPTION:  Create an IPSEC Channel
 *
 *  Return: TRUE: IPSEC channel is created successfully
 *          FALSE: Otherwise
 *
 ******************************************************************************/
Bool sauCreateIpsec (sauHdrHandle_t *pIpHdl, uint32_t spi, uint8_t type, sauIpsecConfig_t* pCfg)
{
    salldSimCommConfig_t   commCfg;
    salldSimIpsecConfig_t  ipsecCfg;
    salldOpenConfig_t      openCfg;
    sauIp_t *pIpHdr = &pIpHdl->u.ip;
    uint16_t retCode;
    Bool esp = (type != SAU_IPSEC_TYPE_AH);
    
    sauIpsec_t* pIpsecHdl = TEST_ALLOC_IPSEC_HANDLE();    
    
    if (pIpsecHdl == NULL)
    {
        salld_sim_print("sauCreateEsp: Ipsec Handle is not available!\n");
        return (FALSE);
    }  
    
    if (type != SAU_IPSEC_TYPE_AH)
    {
        pIpsecHdl->type = type;
        pIpsecHdl->ahIvSize = 0;
    }                      
    else
    {
        pIpsecHdl->type = SAU_IPSEC_TYPE_AH;
        pIpsecHdl->ahIvSize = (pCfg->authMode == sa_AuthMode_GMAC)?8:0;
    
    }
    pIpsecHdl->macSize = pCfg->macSize;
    pIpsecHdl->spi = spi;  
    
    /* Prepare and create the SALLD channel */
    memset(&commCfg, 0, sizeof(commCfg));
    memset(&ipsecCfg, 0, sizeof(ipsecCfg));
    memset(&openCfg, 0, sizeof(openCfg));
    commCfg.protocolType  = (esp)?sa_PT_IPSEC_ESP:sa_PT_IPSEC_AH;
    commCfg.cipherMode    = pCfg->cipherMode;
    commCfg.authMode      = pCfg->authMode;
    commCfg.replayWinSize = pCfg->replayWindowSize;
    
    //commCfg.destInfo[0].ctrlBitfield = sa_DEST_INFO_CTRL_USE_LOC_DMA;
    commCfg.destInfo[0].swInfo0 = esp?0x11100000:0x22200000;
    commCfg.destInfo[0].swInfo1 = esp?0x1110:0x2220;
    commCfg.destInfo[0].flowID = 0;
    #if SA_GEN_TEST_VECTOR_TX
    commCfg.destInfo[0].queueID = DEST_QUEUE_PKT_TX;                   
    #else
    commCfg.destInfo[0].queueID = esp?DEST_QUEUE_PKT_TX:               /* For AH traffic, it goes to PA (644)*/
                                      (uint16_t)(nssGblCfgParams.layout.txQueueBase +      
                                                 nssGblCfgParams.layout.qPaTxCmdIndex);          
    #endif
    commCfg.destInfo[1].swInfo0 = esp?0x11110000:0x22220000;
    commCfg.destInfo[1].swInfo1 = esp?0x1111:0x2222;
    commCfg.destInfo[1].flowID = 0;
#if SA_GEN_TEST_VECTOR_RX
    commCfg.destInfo[1].queueID = DEST_QUEUE_PKT_RECV;                 /* Test Vector Collection */
#else
    if (nssGblCfgParams.layout.fNssGen2)
    {
         /* NSS Gen2: Inner IP firewall: UDP traffic: IP traffic */
    #ifndef NETSS_INTERNAL_PKTDMA 
        commCfg.destInfo[1].queueID = (uint16_t)(nssGblCfgParams.layout.txQueueBase +      
                                       nssGblCfgParams.layout.qPaFirewall2Index);
    #else
        /* Local PKTDMA test */
        commCfg.destInfo[1].queueID = (uint16_t)(nssGblCfgParams.layout.txLocQueueBase +
                                       nssGblCfgParams.layout.qPaFirewall2Index);
        commCfg.destInfo[1].ctrlBitfield |= sa_DEST_INFO_CTRL_USE_LOC_DMA;
    #endif
    }
    else
    {
        /* NSS Gen1: UDP Traffic: Inner IP traffic */
        commCfg.destInfo[1].queueID = (pIpHdr->nextHdr == 17)?(uint16_t)(nssGblCfgParams.layout.txQueueBase +      
                                                                         nssGblCfgParams.layout.qPaLut2Index):       
                                                              (uint16_t)(nssGblCfgParams.layout.txQueueBase +
                                                                         nssGblCfgParams.layout.qPaInnerIpIndex);       
    }
#endif
    
    ipsecCfg.spi          = spi;
    ipsecCfg.encKeySize   = pCfg->encKeySize;
    ipsecCfg.macKeySize   = pCfg->macKeySize;
    ipsecCfg.macSize      = pCfg->macSize;
    ipsecCfg.esn          = pCfg->esn;
    
    openCfg.ctrlBitMap    = sa_CONTROLINFO_CTRL_TX_ON | sa_CONTROLINFO_CTRL_RX_ON;
    
    pIpsecHdl->pChan = salldcfg_chan_init(&commCfg, (void *)&ipsecCfg);
    
    if (pIpsecHdl->pChan == NULL)
    {
        /* Free the protocol header handle */
        numIpsecHandles--;
        salld_sim_print("sauCreateIpsec: salldcfg_chan_init() failed!\n");
        return (FALSE);
    }
    
    if ((retCode = salldSim_init_chn(pIpsecHdl->pChan)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateIpsec: salldSim_init_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    if ((retCode = salldSim_open_chn(pIpsecHdl->pChan, &openCfg)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateIpsec: salldSim_open_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    pIpHdr->ipsecHandle = pIpsecHdl;
    
    return (TRUE);
}

/*******************************************************************************
 *  Function: Create a SRTP channel  
 *******************************************************************************
 *  DESCRIPTION:  Create a SRTP Channel
 *
 *  Return: TRUE: SRTP channel is created successfully
 *          FALSE: Otherwise
 *
 ******************************************************************************/
Bool sauCreateSrtp (sauHdrHandle_t *pUdpHdl, uint16_t initSeqNum, uint32_t ssrc, sauSrtpConfig_t* pCfg)
{
    salldSimCommConfig_t   commCfg;
    salldSimSrtpConfig_t   srtpCfg;
    salldOpenConfig_t      openCfg;
    sauUdp_t *pUdpHdr = &pUdpHdl->u.udp;
    uint16_t retCode;
    
    sauSrtp_t* pSrtpHdl = TEST_ALLOC_SRTP_HANDLE();    
    
    if (pSrtpHdl == NULL)
    {
        salld_sim_print("sauCreateSrtp: SRTP Handle is not available!\n");
        return (FALSE);
    }  
    
    pSrtpHdl->seqNum = initSeqNum;
    pSrtpHdl->ssrc = ssrc;  
    
    /* Prepare and create the SALLD channel */
    memset(&commCfg, 0, sizeof(commCfg));
    memset(&srtpCfg, 0, sizeof(srtpCfg));
    memset(&openCfg, 0, sizeof(openCfg));
    commCfg.protocolType  = sa_PT_SRTP;
    commCfg.cipherMode    = pCfg->cipherMode;
    commCfg.authMode      = pCfg->authMode;
    commCfg.replayWinSize = pCfg->replayWindowSize;
    
    commCfg.destInfo[0].swInfo0 = 0x33300000;
    commCfg.destInfo[0].swInfo1 = 0x3330;
    commCfg.destInfo[0].flowID = 0;
    #if SA_GEN_TEST_VECTOR_TX
    commCfg.destInfo[0].queueID = DEST_QUEUE_PKT_TX;   
    #else
    commCfg.destInfo[0].queueID = (uint16_t)(nssGblCfgParams.layout.txQueueBase +     /* PA for UDP checksum */ 
                                             nssGblCfgParams.layout.qPaTxCmdIndex);          
                     
    #endif
    
    commCfg.destInfo[1].swInfo0 = 0x33330000;
    commCfg.destInfo[1].swInfo1 = 0x3333;
    commCfg.destInfo[1].flowID = 0;
    commCfg.destInfo[1].queueID = DEST_QUEUE_PKT_RECV;   /* TBE: UDP traffic: IP traffic */
    
    srtpCfg.macSize      = pCfg->macSize;
    srtpCfg.mkiSize      = pCfg->mkiSize;
    srtpCfg.fromTo       = pCfg->fromTo;
    srtpCfg.derivRate    = pCfg->derivRate;
    srtpCfg.keyLifeTime  = pCfg->keyLifeTime;
    srtpCfg.fromEsn      = pCfg->fromEsn;
    srtpCfg.toEsn        = pCfg->toEsn;
    srtpCfg.index        = 0;                 /* set ROC to 0 */ 
    
    openCfg.ctrlBitMap    = sa_CONTROLINFO_CTRL_TX_ON | sa_CONTROLINFO_CTRL_RX_ON;
    
    pSrtpHdl->pChan = salldcfg_chan_init(&commCfg, (void *)&srtpCfg);
    
    if (pSrtpHdl->pChan == NULL)
    {
        /* Free the protocol header handle */
        numIpsecHandles--;
        salld_sim_print("sauCreateSrtp: salldcfg_chan_init() failed!\n");
        return (FALSE);
    }
    
    if ((retCode = salldSim_init_chn(pSrtpHdl->pChan)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateSrtp: salldSim_init_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    if ((retCode = salldSim_open_chn(pSrtpHdl->pChan, &openCfg)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateSrtp: salldSim_open_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    pUdpHdr->srtpHandle = pSrtpHdl;
    
    /* Append RTP header to the UDP header */
    pUdpHdl->hdrLen += 12;
    pktWrite16bits_m((uint8_t *)pUdpHdl->hdr, 8, 0x8064);              /* V|P|X|CC|M|PT */
    pktWrite16bits_m((uint8_t *)pUdpHdl->hdr, 10, pSrtpHdl->seqNum);
    pktWrite32bits_m((uint8_t *)pUdpHdl->hdr, 12, 0x12345678);        /* Timestamp */
    pktWrite32bits_m((uint8_t *)pUdpHdl->hdr, 16, pSrtpHdl->ssrc);    /* SSRC */
    
    return (TRUE);
}

/*******************************************************************************
 *  Function: Create a SRTCP channel  
 *******************************************************************************
 *  DESCRIPTION:  Create a SRTCP Channel
 *
 *  Return: TRUE: SRTP channel is created successfully
 *          FALSE: Otherwise
 *
 ******************************************************************************/
Bool sauCreateSrtcp (sauHdrHandle_t *pUdpHdl, uint32_t initSeqNum, uint32_t ssrc, sauSrtpConfig_t* pCfg)
{
    salldSimCommConfig_t   commCfg;
    salldSimSrtpConfig_t   srtpCfg;
    salldOpenConfig_t      openCfg;
    sauUdp_t *pUdpHdr = &pUdpHdl->u.udp;
    uint16_t retCode;
    
    sauSrtcp_t* pSrtcpHdl = TEST_ALLOC_SRTCP_HANDLE();    
    
    if (pSrtcpHdl == NULL)
    {
        salld_sim_print("sauCreateSrtp: SRTCP Handle is not available!\n");
        return (FALSE);
    }  
    
    pSrtcpHdl->ssrc  = ssrc;  
    
    /* Prepare and create the SALLD channel */
    memset(&commCfg, 0, sizeof(commCfg));
    memset(&srtpCfg, 0, sizeof(srtpCfg));
    memset(&openCfg, 0, sizeof(openCfg));
    commCfg.protocolType  = sa_PT_SRTCP;
    commCfg.cipherMode    = pCfg->cipherMode;
    commCfg.authMode      = pCfg->authMode;
    commCfg.replayWinSize = pCfg->replayWindowSize;
    
    srtpCfg.macSize      = pCfg->macSize;
    srtpCfg.mkiSize      = pCfg->mkiSize;
    srtpCfg.fromTo       = pCfg->fromTo;
    srtpCfg.derivRate    = pCfg->derivRate;
    srtpCfg.keyLifeTime  = pCfg->keyLifeTime;
    srtpCfg.fromEsn      = pCfg->fromEsn;
    srtpCfg.toEsn        = pCfg->toEsn;
    srtpCfg.index        = initSeqNum;
    
    openCfg.ctrlBitMap    = sa_CONTROLINFO_CTRL_TX_ON | sa_CONTROLINFO_CTRL_RX_ON;
    
    pSrtcpHdl->pChan = salldcfg_chan_init(&commCfg, (void *)&srtpCfg);
    
    if (pSrtcpHdl->pChan == NULL)
    {
        /* Free the protocol header handle */
        numIpsecHandles--;
        salld_sim_print("sauCreateSrtcp: salldcfg_chan_init() failed!\n");
        return (FALSE);
    }
    
    if ((retCode = salldSim_init_chn(pSrtcpHdl->pChan)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateSrtcp: salldSim_init_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    if ((retCode = salldSim_open_chn(pSrtcpHdl->pChan, &openCfg)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateSrtcp: salldSim_open_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    pUdpHdr->srtcpHandle = pSrtcpHdl;
    
    /* Append RTCP header to the UDP header */
    pUdpHdl->hdrLen += 8;
    pktWrite16bits_m((uint8_t *)pUdpHdl->hdr, 8, 0x8064);             /* V|P|RC|PT */
    pktWrite16bits_m((uint8_t *)pUdpHdl->hdr, 10, 0);                 /* Message Length */
    pktWrite32bits_m((uint8_t *)pUdpHdl->hdr, 12, pSrtcpHdl->ssrc);   /* SSRC */
    
    return (TRUE);
}


/*******************************************************************************
 *  Function: Update a SRTP channel  
 *******************************************************************************
 *  DESCRIPTION:  Update a SRTP Channel
 *
 ******************************************************************************/
void sauUpdateSrtp (sauHdrHandle_t *pUdpHdl, uint16_t seqNum, uint32_t ssrc)
{
    sauSrtp_t* pSrtpHdl = pUdpHdl->u.udp.srtpHandle;    
    
    if (pSrtpHdl == NULL)
    {
        salld_sim_print("sauUpdateSrtp: SRTP Handle does not exist!\n");
    }
    
    pSrtpHdl->seqNum = seqNum;
    pSrtpHdl->ssrc = ssrc;  
}

/*******************************************************************************
 *  Function: Create an Air Ciphering channel  
 *******************************************************************************
 *  DESCRIPTION:  Create an Air Ciphering Channel
 *
 ******************************************************************************/
sauHdrHandle_t* sauCreateAc (sauHdrHandle_t *pHdl, uint16_t initSeqNum, sauAcConfig_t* pCfg)
{
    sauHdrHandle_t *pHdrHandle = TEST_ALLOC_HDR_HANDLE();
    salldSimCommConfig_t   commCfg;
    salldSimAcConfig_t     acCfg;
    salldOpenConfig_t      openCfg;
    uint16_t retCode;
    sauAc_t   *pAcHdr = &pHdrHandle->u.ac;
    
    memset(pAcHdr, 0, sizeof(sauAc_t));
    
    if (pHdrHandle == NULL)
    {
        salld_sim_print("sauCreateAc: Hdr Handle is not available!\n");
        return (NULL);
    }    
    
    /* Prepare and create the SALLD channel */
    memset(&commCfg, 0, sizeof(commCfg));
    memset(&acCfg, 0, sizeof(acCfg));
    memset(&openCfg, 0, sizeof(openCfg));
    commCfg.protocolType  = sa_PT_3GPP_AC;
    commCfg.cipherMode    = pCfg->cipherMode;
    commCfg.authMode      = pCfg->authMode;
    commCfg.replayWinSize = 0;
    
    commCfg.destInfo[0].swInfo0 = 0x44400000;
    commCfg.destInfo[0].swInfo1 = 0x4440;
    commCfg.destInfo[0].flowID = 0;
    commCfg.destInfo[0].queueID = DEST_QUEUE_PKT_RECV;   /* Route to Host */
    commCfg.destInfo[1].swInfo0 = 0x44440000;
    commCfg.destInfo[1].swInfo1 = 0x4444;
    commCfg.destInfo[1].flowID = 0;
    commCfg.destInfo[1].queueID = DEST_QUEUE_PKT_TX;     /* Route to Host */
    
    acCfg.pduType = pCfg->pduType;
    acCfg.upLink = pCfg->upLink;
	acCfg.intKey = pCfg->intKey;
    acCfg.countCPresent = pCfg->countCPresent;
    
    openCfg.ctrlBitMap    = sa_CONTROLINFO_CTRL_TX_ON | sa_CONTROLINFO_CTRL_RX_ON;
    
    pAcHdr->pChan = salldcfg_chan_init(&commCfg, (void *)&acCfg);
    
    if (pAcHdr->pChan == NULL)
    {
        /* Free the protocol header handle */
        numHdrHandles--;
        salld_sim_print("sauCreateAc: salldcfg_chan_init() failed!\n");
        return (NULL);
    }
    
    if ((retCode = salldSim_init_chn(pAcHdr->pChan)) != salld_SIM_ERR_NONE)
    {
        numHdrHandles--;
        salld_sim_print("sauCreateAc: salldSim_init_chn() failed with retCode = 0x%0x!\n", retCode);
        return (NULL);
    }
    
    if ((retCode = salldSim_open_chn(pAcHdr->pChan, &openCfg)) != salld_SIM_ERR_NONE)
    {
        numHdrHandles--;
        salld_sim_print("sauCreateAc: salldSim_open_chn() failed with retCode = 0x%0x!\n", retCode);
        return (NULL);
    }

    /* Fill the header information */
    pAcHdr->seqNum = initSeqNum;
    pAcHdr->pduType = pCfg->pduType;
    pAcHdr->cipherMode = pCfg->cipherMode;
    pAcHdr->ctrlBitMap = ((pCfg->cipherMode == sa_CipherMode_NULL) || (pCfg->authMode == sa_AuthMode_NULL))?
                         SA_PROTO_AC_CTRL_FLAG_IV_TEST: 0; 
    pAcHdr->useIV = FALSE; 
    pAcHdr->ivSize = 0;
    pAcHdr->seqNumSize = 0;
    pAcHdr->authHdrSize = 0;
    pAcHdr->count32 = 0x43210000;
    pAcHdr->count32_2 = 0x56780000;
    
    switch (pAcHdr->pduType)
    {
        case sa_AcPduType_GSM:
            memset(pHdrHandle->hdr, 0, 16);
            pAcHdr->ivSize = 8;
            switch (pAcHdr->cipherMode)
            {
                case sa_CipherMode_GSM_A53:
                    pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, SAU_KGCORE_CA_OFFSET, SAU_KGCORE_CA_GSM_A53);
                    break;  
                    
                case sa_CipherMode_ECSD_A53:
                    pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, SAU_KGCORE_CA_OFFSET, SAU_KGCORE_CA_ECSD_A53);
                    break; 
                    
                case sa_CipherMode_GEA3:
                    pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, SAU_KGCORE_CA_OFFSET, SAU_KGCORE_CA_GEA3);
                    pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, SAU_KGCORE_CB_OFFSET, pCfg->upLink?0x00:0x04);
                    break;   
                      
                case sa_CipherMode_KASUMI_F8:    
                    pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, SAU_KGCORE_CA_OFFSET, SAU_KGCORE_CA_F8);
                    pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, SAU_KGCORE_CB_OFFSET, pCfg->upLink?0x00:0x04);
                    break; 
                    
                case sa_CipherMode_NULL:
                    /* Use iv for Authentication: CMAC and Kasumi-F9: ivSize = 1 */
                    pktWrite32bits_m((uint8_t *)pHdrHandle->hdr, 0, pAcHdr->count32);
                    pktWrite32bits_m((uint8_t *)pHdrHandle->hdr, 4, 0x12345678);
                    break; 
                    
                default:
                    pAcHdr->ivSize = 16; 
                    memcpy(pHdrHandle->hdr, testAcIv, pAcHdr->ivSize);
                    break;
            }
           
            pHdrHandle->hdrLen = pAcHdr->ivSize;
            break;
        
        case sa_AcPduType_WCDMA_UMD:
            pAcHdr->seqNumSize = 1;
            pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, 0, (uint8_t)(pAcHdr->seqNum << 1));
            pHdrHandle->hdrLen = 2;          
            break;
        
        case sa_AcPduType_WCDMA_AMD:
            pAcHdr->seqNumSize = 2;
            pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, 0, (uint8_t)(pAcHdr->seqNum << 3));
            pHdrHandle->hdrLen = 3;            
            break;
                   
        case sa_AcPduType_LTE_CP:
            pAcHdr->authHdrSize = 1;
            pktWrite8bits_m((uint8_t *)pHdrHandle->hdr, 0, (uint8_t)(pAcHdr->seqNum & 0x1F));
            pHdrHandle->hdrLen = pAcHdr->authHdrSize;      
            if ((pCfg->cipherMode == sa_CipherMode_KASUMI_F8) || (pCfg->authMode == sa_AuthMode_KASUMI_F9) ||  (pCfg->authMode == sa_AuthMode_CMAC)) 
            {
                pAcHdr->ivSize = 8;    
            }
            else
            {
                pAcHdr->ivSize = 16;    
                
            } 
            pAcHdr->iv[2] = pAcHdr->iv[0] = pAcHdr->count32_2;
            pAcHdr->iv[3] = pAcHdr->iv[1] = 0xbabeface;
            break;
            
        case sa_AcPduType_LTE:
            if ((pCfg->cipherMode == sa_CipherMode_KASUMI_F8) || (pCfg->authMode == sa_AuthMode_KASUMI_F9) ||  (pCfg->authMode == sa_AuthMode_CMAC)) 
            {
                pAcHdr->ivSize = 8;    
            }
            else
            {
                pAcHdr->ivSize = 16;    
                
            } 
            pAcHdr->iv[2] = pAcHdr->iv[0] = pAcHdr->count32_2;
            pAcHdr->iv[3] = pAcHdr->iv[1] = 0xbabeface;
            
            // pass through
        
        case sa_AcPduType_WCDMA_TMD:
            pAcHdr->ctrlBitMap |= SA_PROTO_AC_CTRL_FLAG_COUNTC_INSERT;
        default:
            pHdrHandle->hdrLen = 0;
            break;
    }
    
    pHdrHandle->upHdl = pHdl;   
    pHdrHandle->hdrType = SAU_PROTO_AC;
    
    return(pHdrHandle);
}

#endif

/*******************************************************************************
 *  Function: Create a Data Mode channel  
 *******************************************************************************
 *  DESCRIPTION:  Create a Data Mode Channel
 *
 ******************************************************************************/
sauHdrHandle_t* sauCreateDm (sauHdrHandle_t *pHdl, uint16_t hdrSize, sauDataModeConfig_t* pCfg)
{
    sauHdrHandle_t *pHdrHandle = TEST_ALLOC_HDR_HANDLE();
    salldSimCommConfig_t        commCfg;
    salldSimDataModeConfig_t    dmCfg;
    salldOpenConfig_t           openCfg;
    uint16_t retCode;
    sauDataMode_t  *pDmHdr = &pHdrHandle->u.data;
    int i;
    
    if (pHdrHandle == NULL)
    {
        salld_sim_print("sauCreateDm: Hdr Handle is not available!\n");
        return (NULL);
    }    
    
    /* Prepare and create the SALLD channel */
    memset(&commCfg, 0, sizeof(commCfg));
    memset(&dmCfg, 0, sizeof(dmCfg));
    memset(&openCfg, 0, sizeof(openCfg));
    commCfg.protocolType  = sa_PT_NULL;
    commCfg.cipherMode    = pCfg->cipherMode;
    commCfg.authMode      = pCfg->authMode;
    commCfg.replayWinSize = 0;
    
    commCfg.destInfo[0].swInfo0 = 0x55500000;
    commCfg.destInfo[0].swInfo1 = 0x5550;
    commCfg.destInfo[0].flowID = 0;
    commCfg.destInfo[0].queueID = DEST_QUEUE_PKT_TX;   /* Route to Host */
    commCfg.destInfo[1].swInfo0 = 0;
    commCfg.destInfo[1].flowID = 0;
    commCfg.destInfo[1].queueID = 0;     /* Note Used */
    
    dmCfg.encKeySize = pCfg->encKeySize;
    dmCfg.macKeySize = pCfg->macKeySize;
    dmCfg.macSize    = pCfg->macSize;
    dmCfg.aadSize    = pCfg->aadSize;
    dmCfg.enc        = pCfg->enc;
    dmCfg.enc1st     = pCfg->enc1st;
    dmCfg.ivSize     = pCfg->ivSize;
    dmCfg.saltSize   = pCfg->saltSize;
    dmCfg.ctrlBitMap = (uint16_t) 0U;
    if (pCfg->selACEng == TRUE)
    {
      dmCfg.ctrlBitMap |= sa_DM_CONFIG_SELECT_AIR_CIPHER_ENG;
    }

    openCfg.ctrlBitMap    = sa_CONTROLINFO_CTRL_TX_ON;
    pDmHdr->pChan = salldcfg_chan_init(&commCfg, (void *)&dmCfg);
    
    if (pDmHdr->pChan == NULL)
    {
        /* Free the protocol header handle */
        numHdrHandles--;
        salld_sim_print("sauCreateDm: salldcfg_chan_init() failed!\n");
        return (NULL);
    }
    
    if ((retCode = salldSim_init_chn(pDmHdr->pChan)) != salld_SIM_ERR_NONE)
    {
        numHdrHandles--;
        salld_sim_print("sauCreateDm: salldSim_init_chn() failed with retCode = 0x%0x!\n", retCode);
        return (NULL);
    }
    
    if ((retCode = salldSim_open_chn(pDmHdr->pChan, &openCfg)) != salld_SIM_ERR_NONE)
    {
        numHdrHandles--;
        salld_sim_print("sauCreateDm: salldSim_open_chn() failed with retCode = 0x%0x!\n", retCode);
        return (NULL);
    }

    /* Fill the header information */
    pDmHdr->seqNum = 0;
    pDmHdr->aadSize = pCfg->aadSize; 
    memcpy(pDmHdr->aad, testAad, pDmHdr->aadSize);
    
    pDmHdr->fCmdLbAvail = FALSE;
    
    pHdrHandle->hdrLen = hdrSize;
    
    switch (pCfg->cipherMode)
    {
        case sa_CipherMode_AES_CTR:
        case sa_CipherMode_AES_F8:
        case sa_CipherMode_AES_CBC:
        case sa_CipherMode_CCM:
        case sa_CipherMode_GCM:
        case sa_CipherMode_SNOW3G_F8:
            pDmHdr->encIvSize = 16;
            break;
        
        case sa_CipherMode_DES_CBC:
        case sa_CipherMode_3DES_CBC:
        case sa_CipherMode_KASUMI_F8:
            pDmHdr->encIvSize = 8;
            break;
            
        default:
            pDmHdr->encIvSize = 0;
            break;
    }
    
    memcpy(pDmHdr->encIV, testEncIV, pDmHdr->encIvSize);
    
    switch (pCfg->authMode)
    {
        case sa_AuthMode_GMAC:
        case sa_AuthMode_GMAC_AH:
            pDmHdr->authIvSize = 8;
            break;
        
        case sa_AuthMode_KASUMI_F9:
            pDmHdr->authIvSize = 8;
            break;
            
        default:
            pDmHdr->authIvSize = 0;
            break;
    }
    
    memcpy(pDmHdr->authIV, testAuthIV, pDmHdr->authIvSize);
    
    
    pHdrHandle->upHdl = pHdl;   
    pHdrHandle->hdrType = SAU_PROTO_DATAMODE;

    /* Check if the hdrSize exceeded the max size */
    if (hdrSize > SAU_MAX_HDR_SIZE)
    {
      /* Reset the hdrSize */
      hdrSize = 12;
    }

    /* Fill the default header info */
    for(i = 0; i < hdrSize; i++)
        pHdrHandle->hdr[i] = (uint8_t)i;
    
    return(pHdrHandle);
}
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
static paRouteInfo_t  sauContiRoute = {  pa_DEST_CONTINUE_PARSE_LUT1,/* Dest */
 								         0,					        /* Flow ID */
 								         0,					        /* queue */
 								         -1,					    /* Multi route */
 								         0,					        /* sw Info 0 */
 									     0,  				        /* sw Info 1 */
                                         0,                         /* customType : not used */         
                                         0,                         /* customIndex: not used */     
                                         0,                         /* pkyType: for SRIO only */    
                                         NULL};                     /* No commands */
                                         
static paRouteInfo_t  sauContiRouteLUT2 = {  pa_DEST_CONTINUE_PARSE_LUT2,/* Dest */
 								         0,					        /* Flow ID */
 								         0,					        /* queue */
 								         -1,					    /* Multi route */
 								         0,					        /* sw Info 0 */
 									     0,  				        /* sw Info 1 */
                                         0,                         /* customType : not used */         
                                         0,                         /* customIndex: not used */     
                                         0,                         /* pkyType: for SRIO only */    
                                         NULL};                     /* No commands */
                                         
                                    
 								    
static paRouteInfo_t   sauFailRoute = {  pa_DEST_DISCARD,           /* Dest */
 									     0,					        /* Flow ID */
 									     0,					        /* queue */
 									     -1,					    /* Multi route */
 									     0,					        /* sw Info 0 */
 									     0,  				        /* sw Info 1 */
                                         0,                         /* customType : not used */         
                                         0,                         /* customIndex: not used */     
                                         0,                         /* pkyType: for SRIO only */    
                                         NULL};                     /* No commands */
                                         
static paRouteInfo_t   sauSaRoute =    { 
                                         pa_DEST_SASS,      		/* Dest */
 								         0,					        /* Flow ID */
 								         0, 	                    /* queue */ /* 646 */
 								         -1,					    /* Multi route */
 								         0,				            /* sw Info 0 */
 									     0,  				        /* sw Info 1 */
                                         0,                         /* customType : not used */         
                                         0,                         /* customIndex: not used */     
                                         0,                         /* pkyType: for SRIO only */    
                                         NULL};                     /* No commands */
                                         
static paRouteInfo_t   sauHostRoute =  { pa_DEST_HOST,     		    /* Dest */
 								         0,					        /* Flow ID */
 								         DEST_QUEUE_PKT_RECV,       /* queue */ /* 950 */
 								         -1,					    /* Multi route */
 								         0x55550000,	            /* sw Info 0 */
 									     0,  				        /* sw Info 1 */
                                         0,                         /* customType : not used */         
                                         0,                         /* customIndex: not used */     
                                         0,                         /* pkyType: for SRIO only */    
                                         NULL};                     /* No commands */
                                         
static paRouteInfo_t   sauHostRouteTxPkt =  { pa_DEST_HOST,     		    /* Dest */
 								         0,					        /* Flow ID */
 								         DEST_QUEUE_PKT_TX,       /* queue */ /* 960 */
 								         -1,					    /* Multi route */
 								         0x55550000,	            /* sw Info 0 */
 									     0,  				        /* sw Info 1 */
                                         0,                         /* customType : not used */         
                                         0,                         /* customIndex: not used */     
                                         0,                         /* pkyType: for SRIO only */    
                                         NULL};                     /* No commands */
#endif

#ifndef SAU_TXPKT_TO_HOST                                         
static paRouteInfo_t   sauPaRouteTxPkt =  { pa_DEST_HOST,     		/* Dest */
 								            0,					    /* Flow ID */
 								            0,                      /* queue */ /*640 */
 								            -1,					    /* Multi route */
 								            0x55550000,	            /* sw Info 0 */
 									        0,  				    /* sw Info 1 */
                                            0,                      /* customType : not used */         
                                            0,                      /* customIndex: not used */     
                                            0,                      /* pkyType: for SRIO only */    
                                            NULL};                  /* No commands */
#endif                                            

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
static paCmdReply_t    sauCmdReply = {  pa_DEST_HOST,			    /* Dest */
 							            0,						    /* Reply ID (returned in swinfo0) */
 							            0,						    /* Queue */
 							            0 };						/* Flow ID */
#endif

/*******************************************************************************
 *  Function: Initialize the SA Test Connection module  
 *******************************************************************************
 *  DESCRIPTION:  Initialize SA test connection-related variables 
 *
 ******************************************************************************/
void sauConnInit(void)
{
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    if (nssGblCfgParams.layout.fNssGen2)
    {
        #ifndef NETSS_INTERNAL_PKTDMA 
        sauSaRoute.dest  = pa_DEST_SASS;   
        sauSaRoute.queue = (uint16_t) (nssGblCfgParams.layout.txQueueBase +
                                       nssGblCfgParams.layout.qSaIndex);
        #else
        sauSaRoute.dest  = pa_DEST_SASS_LOC_DMA;   
        sauSaRoute.queue = (uint16_t)(nssGblCfgParams.layout.txLocQueueBase +
                                      nssGblCfgParams.layout.qSaIndex);
        
        #endif                     
    }
    else
    {
        sauSaRoute.dest  = pa_DEST_SASS;   
        sauSaRoute.queue = nssGblCfgParams.layout.txQueueBase +
                           nssGblCfgParams.layout.qSaIndex;
    }
#endif
    
#ifndef SAU_TXPKT_TO_HOST             

    sauPaRouteTxPkt.queue = (uint16_t)(nssGblCfgParams.layout.txQueueBase +      
                                       nssGblCfgParams.layout.qPaInputIndex);
    
#endif    
 
}

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/*******************************************************************************
 *  Function: Register the MAC entry to PA  
 *******************************************************************************
 *  DESCRIPTION:  Register the MAC entry  to PA if not done before 
 *
 *  Note: Assume that there will be other entry after MAC
 ******************************************************************************/
static void sauRegisterMac(tFramework_t *tf, saTest_t *pat, sauHdrHandle_t* pMacHdl)
{
    sauMac_t* pMac = &pMacHdl->u.mac; 
    paEthInfo_t ethInfo;
    
    /* Verify whether we have already registered the MAC entry */
    if(pMac->t2Hdl->state == TF_L2_HANDLE_ACTIVE)
        return;
        
    memset(&ethInfo, 0, sizeof(paEthInfo_t));
    memcpy(ethInfo.dst, pMac->dest, 6);
    ethInfo.ethertype = pMac->ethType;  
    
 	if (!testCommonAddMac (tf, pat, (paEthInfo_t *)&ethInfo, &sauContiRoute, &sauFailRoute,
 	                       &pMac->t2Hdl->paHandle, tf->QLinkedBuf1, &sauCmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
 	pMac->t2Hdl->state = TF_L2_HANDLE_ACTIVE;
 	paTestExpectedStats.classify1.nPackets += 1;
}
 
/*******************************************************************************
 *  Function: Register the IP entry to PA  
 *******************************************************************************
 *  DESCRIPTION:  Register the IP entry  to PA if not done before 
 *
 *  Note: This function need to be enhanced
 *        Assume that the last entry is UDP (index = 0)
 *        index = 1 ==> next entry is UDP
 *        index = 2 ==> next entry is IP
 *
 *
 ******************************************************************************/
static void sauRegisterIp(tFramework_t *tf, saTest_t *pat, sauHdrHandle_t* pIpHdl, int index)
{
    sauIp_t* pIp = &pIpHdl->u.ip; 
    paIpInfo_t ipInfo, ipInfo2;
    paRouteInfo_t *pRouteInfo, *pRouteInfo2;
    paHandleL2L3_t  paHandle;
    
    /* Verify whether we have already registered the IP entry */
    if(pIp->t3Hdl->state == TF_L2_HANDLE_ACTIVE)
        return;
        
    pRouteInfo2 = NULL;    
    memset(&ipInfo, 0, sizeof(paIpInfo_t));
    memcpy(&ipInfo.dst, pIp->dest, 16);
    ipInfo.ipType = (pIp->type == SAU_IP_TYPE_IP4)?pa_IPV4:pa_IPV6;  
    
    if (pIp->ipsecHandle)
    {
        Sa_SWInfo_t*  pSwInfo = &pIp->ipsecHandle->pChan->regSwInfo;
    
        /* IPSEC channel */
        if (pIp->ipsecHandle->type == SAU_IPSEC_TYPE_ESP_NAT_T)
        {
            memset(&ipInfo2, 0, sizeof(paIpInfo_t));
            
            pIp->t3Hdl2 = TEST_ALLOC_L3_HANDLE();
    
            if (pIp->t3Hdl2 == NULL)
            {
                /* Free the protocol header handle */
                salld_sim_print("sauRegisterIp: L3 Handle2 is not available!\n");
            }

            ipInfo2.ipType = (pIp->type == SAU_IP_TYPE_IP4)?pa_IPV4:pa_IPV6;  
            ipInfo2.spi = pIp->ipsecHandle->spi;
        
            pRouteInfo = &sauContiRouteLUT2;
            pRouteInfo2 = &sauSaRoute;
            pRouteInfo2->swInfo0 = pSwInfo->swInfo[0];
            pRouteInfo2->swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);
        
            #if SA_GEN_TEST_VECTOR_RX
            /* Overwrite with Host routine */
            pRouteInfo2 = &sauHostRoute;
            #endif
        }
        else
        {
            ipInfo.spi = pIp->ipsecHandle->spi;
            ipInfo.proto = (pIp->ipsecHandle->type == SAU_IPSEC_TYPE_ESP)?0x32:0x33;
            pRouteInfo = &sauSaRoute;
            pRouteInfo->swInfo0 = pSwInfo->swInfo[0];
            pRouteInfo->swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);
        
            #if SA_GEN_TEST_VECTOR_RX
            /* Overwrite with Host routine */
            pRouteInfo = &sauHostRoute;
            #endif
        }
        
    }
    else if (index == 0)
    {
        /* It is the last entry */
        pRouteInfo = &sauHostRoute;
        
        /* Use swInfo0 as channel identifier */
    }
    else 
    {
        /* Continue Parsing */
        pRouteInfo = (index >= 2)?&sauContiRoute:&sauContiRouteLUT2;
    }
    
    
    if (pIpHdl->upHdl->hdrType == SAU_PROTO_MAC)
    {
        paHandle = pIpHdl->upHdl->u.mac.t2Hdl->paHandle;
    }
    else
    {
        if(pIpHdl->upHdl->u.ip.t3Hdl2 == NULL)
            paHandle = pIpHdl->upHdl->u.ip.t3Hdl->paHandle;
        else
            paHandle = pIpHdl->upHdl->u.ip.t3Hdl2->paHandle;
    }
    
 	if (!testCommonAddIp  (tf, pat, (paIpInfo_t *)&ipInfo, pRouteInfo, &sauFailRoute,
 	                       paHandle, &pIp->t3Hdl->paHandle, tf->QLinkedBuf2, 
 	                       &sauCmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }                    
      
 	pIp->t3Hdl->state = TF_L2_HANDLE_ACTIVE;
 	paTestExpectedStats.classify1.nPackets += 1;
    
    if (pRouteInfo2)
    {
        if (nssGblCfgParams.layout.fNssGen2)
        {
 	        if (!testCommonAddIp2  (tf, pat, pa_LUT1_INST_1, (paIpInfo_t *)&ipInfo2, pRouteInfo2, &sauFailRoute,
 	                                pIp->t3Hdl->paHandle, &pIp->t3Hdl2->paHandle, tf->QLinkedBuf1, 
 	                                &sauCmdReply))
            {
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }                    
                                    
                                    
        }
        else
        {                        
 	        if (!testCommonAddIp2  (tf, pat, pa_LUT1_INST_1, (paIpInfo_t *)&ipInfo2, pRouteInfo2, &sauFailRoute,
 	                                NULL, &pIp->t3Hdl2->paHandle, tf->QLinkedBuf1, 
 	                                &sauCmdReply))
            {
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }                    
                                    
        }                        
      
 	    pIp->t3Hdl2->state = TF_L2_HANDLE_ACTIVE;
 	    paTestExpectedStats.classify1.nPackets += 1;
    
    }
    
}

/*******************************************************************************
 *  Function: Register the UDP entry to PA  
 *******************************************************************************
 *  DESCRIPTION:  Register the UDP entry  to PA if not done before 
 *
 ******************************************************************************/
void sauRegisterUdp(tFramework_t *tf, saTest_t *pat, sauHdrHandle_t* pUdpHdl)
{
    sauUdp_t* pUdp = &pUdpHdl->u.udp; 
    paRouteInfo_t* pRouteInfo;
    paHandleL2L3_t  paHandle = pUdpHdl->upHdl->u.ip.t3Hdl->paHandle;
    
    /* Verify whether we have already registered the UDP entry */
    if(pUdp->t4Hdl->state == TF_L2_HANDLE_ACTIVE)
        return;
        
    if (pUdpHdl->upHdl->u.ip.t3Hdl2)
    {
        paHandle = pUdpHdl->upHdl->u.ip.t3Hdl2->paHandle;
    }
        
    if (pUdp->srtpHandle)
    {
        Sa_SWInfo_t*  pSwInfo = &pUdp->srtpHandle->pChan->regSwInfo;
            
        /* SRTP channel */
        pRouteInfo = &sauSaRoute;
        pRouteInfo->swInfo0 = pSwInfo->swInfo[0];
        pRouteInfo->swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);
        
        #if SA_GEN_TEST_VECTOR_RX
        /* Overwrite with Host routine */
        pRouteInfo = &sauHostRoute;
        #endif
    }
    else 
    {
        /* It is the last entry */
        pRouteInfo = &sauHostRoute;
        
        /* Use swInfo0 as channel identifier */
    }
    
 	if (!testCommonAddUdp  (tf, pat, pUdp->destPort, FALSE, pRouteInfo, 
 	                       paHandle, pUdp->t4Hdl->paHandle, tf->QLinkedBuf1, 
 	                       &sauCmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }                       
    
 	pUdp->t4Hdl->state = TF_L2_HANDLE_ACTIVE;
 	paTestExpectedStats.classify2.nPackets += 1;
    
}

/*******************************************************************************
 *  Function: Reregister the UDP entry to PA  
 *******************************************************************************
 *  DESCRIPTION:  Reregister the UDP entry  to PA for the new keys 
 *
 ******************************************************************************/
void sauReregegisterUdp(void* pHdl)
{
    sauConnHandle_t* pConnHdl = (sauConnHandle_t*)pHdl;
    sauHdrHandle_t* pUdpHdl;  
    sauUdp_t* pUdp; 
    paRouteInfo_t* pRouteInfo;
    paHandleL2L3_t  paHandle;
    Sa_SWInfo_t*  pSwInfo;
    
    if (pConnHdl->srtpIndex == SA_CONN_INDEX_NONE)
    {
        salld_sim_print("sauReregegisterUdp: SRTP channel does not exist in the connection!\n");
        return;
    }
    
    pUdpHdl =  pConnHdl->pHdrHdl[pConnHdl->srtpIndex];
    pUdp = &pUdpHdl->u.udp;
    paHandle = pUdpHdl->upHdl->u.ip.t3Hdl->paHandle;
    
    /* Verify whether we have already registered the UDP entry */
    if (pUdp->t4Hdl->state != TF_L2_HANDLE_ACTIVE)
    {
        salld_sim_print("sauReregegisterUdp: UDP has not been registered yet!\n");
        return;
    }    
        
    pSwInfo = &pUdp->srtpHandle->pChan->regSwInfo;
    pRouteInfo = &sauSaRoute;
    pRouteInfo->swInfo0 = pSwInfo->swInfo[0];
    pRouteInfo->swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);
    
 	if (!testCommonAddUdp  (&tFramework, tFramework.pTest, pUdp->destPort, TRUE, pRouteInfo, 
 	                       paHandle, pUdp->t4Hdl->paHandle, tFramework.QLinkedBuf1, 
 	                       &sauCmdReply))
    {
        salld_sim_print("sauReregegisterUdp: Add UDP failed!\n");
        return;
    }                       
    
}

#endif

/*******************************************************************************
 *  Function: Create connection  
 *******************************************************************************
 *  DESCRIPTION:  Link the related hdr Handles to create a connection 
 *
 ******************************************************************************/
sauConnHandle_t* sauCreateConnection(tFramework_t *tf, saTest_t *pat, sauHdrHandle_t *pTail)
{
    sauConnHandle_t* pConnHdl = TEST_ALLOC_CONN_HANDLE(); 
    sauHdrHandle_t* pHdrHdl = pTail;
    int index, ipIndex;
    
    if (pConnHdl == NULL)
    {
        salld_sim_print("sauCreateConnection: Connection Handle is not available!\n");
        return (NULL);
    }  
    
    memset(pConnHdl, 0, sizeof(sauConnHandle_t));
    memset(pConnHdl->ipOffset, SA_CONN_OFFSET_NONE, 3);
    pConnHdl->ID = (uint16_t)(numConnHandles - 1);
    pConnHdl->ipsecOffset = SA_CONN_OFFSET_NONE;
    pConnHdl->rtpOffset = SA_CONN_OFFSET_NONE;
    pConnHdl->udpOffset = SA_CONN_OFFSET_NONE;
    pConnHdl->ipsecIndex = SA_CONN_INDEX_NONE;
    pConnHdl->srtpIndex = SA_CONN_INDEX_NONE;
    pConnHdl->srtcpIndex = SA_CONN_INDEX_NONE;
    
    /* record the handles in the connection chain */
    do 
    {
        pConnHdl->pHdrHdl[pConnHdl->numHandles++] = pHdrHdl; 
        
        if (pConnHdl->numHandles > SA_MAX_HDR_HANDLES_PER_CONN)
        {
            salld_sim_print("sauCreateConnection: Too many entries (%d) in the chain\n", 
                              pConnHdl->numHandles);
            numConnHandles--;
            return(NULL);                  
        }
    } while ((pHdrHdl = pHdrHdl->upHdl));
    
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    /*
     * Perform the following processes in reverse order
     * - Extract Header information
     * - Extract protocol offset information
     * - Register the receive protocol information to PA if not done before
     */
    sauCmdReply.replyId = TF_CMD_SWINFO0_ADD_ID + 0;  /* TF_CMD_SWINFO0_ADD_ID identifies command, 16 LS bits identify the local handle number */
    sauCmdReply.queue = tf->QGen[Q_CMD_REPLY];
#endif

    ipIndex = 0; 
    for (index = pConnHdl->numHandles - 1; index >= 0; index--)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];
        
        switch (pHdrHdl->hdrType)
        {
            #if !defined(NSS_LITE) && !defined(NSS_LITE2)
            case SAU_PROTO_MAC:
                sauRegisterMac(tf, pat, pHdrHdl);
            break;
            
            case SAU_PROTO_IP:
                pHdrHdl->u.ip.index = ipIndex;
                pConnHdl->ipOffset[ipIndex++] = pConnHdl->hdrLen;
                if (pHdrHdl->u.ip.ipsecHandle)
                {
                    /* Note: only one IPSEC channel is supported per connection */
                    pConnHdl->ipsecIndex = index;  
                    pConnHdl->ipsecOffset = pConnHdl->hdrLen + pHdrHdl->hdrLen; 
                    pHdrHdl->u.ip.ipsecHandle->pChan->pConnHdl = (void *)pConnHdl; 
                    
                }    
                sauRegisterIp(tf, pat, pHdrHdl, index);   
            break;
        
            case SAU_PROTO_UDP:
                pConnHdl->udpOffset =  pConnHdl->hdrLen;
                if (pHdrHdl->u.udp.srtpHandle)
                {
                    pConnHdl->rtpOffset = pConnHdl->udpOffset + 8;
                    /* Note: only one SRTP channel is supported per connection */
                    pConnHdl->srtpIndex = index; 
                    pHdrHdl->u.udp.srtpHandle->pChan->pConnHdl = (void *)pConnHdl;   
                    
                }
                else if (pHdrHdl->u.udp.srtcpHandle)
                {
                    pConnHdl->rtpOffset = pConnHdl->udpOffset + 8;
                    /* Note: only one SRTCP channel is supported per connection */
                    pConnHdl->srtcpIndex = index; 
                    pHdrHdl->u.udp.srtcpHandle->pChan->pConnHdl = (void *)pConnHdl;   
                
                }    
                sauRegisterUdp(tf, pat, pHdrHdl);
            break;
            
            case SAU_PROTO_AC:
        #endif    
            case SAU_PROTO_DATAMODE:
            break;
            
            default:
                /* Should never enter here */
                salld_sim_print("sauCreateConnection: Invalid Hdr type (%d) in the chain\n", 
                                 pHdrHdl->hdrType);
                numConnHandles--;
                return(NULL);                  
        }
        
        /* copy the header and update hdr length */
        memcpy(&pConnHdl->hdr[pConnHdl->hdrLen], pHdrHdl->hdr, pHdrHdl->hdrLen);
        pConnHdl->hdrLen += pHdrHdl->hdrLen;
    }
    pConnHdl->numIp = (uint8_t)ipIndex; 
    
    return(pConnHdl);
} 

/*******************************************************************************
 *  Function: Generate payload  
 *******************************************************************************
 *  DESCRIPTION:  Fill the data buffer with the specified payload data  
 *
 *  Note: It is up to the caller to allocate buffer
 ******************************************************************************/
void sauGenPayload(sauPayloadType_e type, uint8_t initValue, uint16_t len, uint8_t* buf)
{
    uint8_t data = initValue;
    int i;
    
    switch (type)
    {
        case SAU_PAYLOAD_CONST:
            memset(buf, data, len);
            break;
            
        case SAU_PAYLOAD_INC8: 
            for(i = 0; i < len; i++) buf[i] = data++;       
            break;
            
        case SAU_PAYLOAD_DEC8: 
            for(i = 0; i < len; i++) buf[i] = data--;       
            break;
            
         case SAU_PAYLOAD_RANDOM:
            for(i = 0; i < len; i++) buf[i] = rand() & 0xFF;       
            break;
            
         default:
            printf("sauGenPayload: invalid paylaod type (%d)\n", type);
            break;   
    }
}

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/*******************************************************************************
 *  Function: Process the Tx packet for IP  
 *******************************************************************************
 *  DESCRIPTION:  This function perform IP operation of the Tx packet 
 *
 *  Return: TRUE: IPSEC operation
 *          FALSE: IP operation 
 *
 * Note: This function will be called in reverse order of the connection chain
 *
 ******************************************************************************/
Bool sauProcTxPktIp(sauConnHandle_t *pConnHdl, sauHdrHandle_t* pIpHdl, 
                    Sa_PktInfo_t* pPktInfo, Sa_PktDesc_t* pPktDesc)
{
    sauIp_t*    pIp    = &pIpHdl->u.ip;
    sauIpsec_t* pIpsec = pIp->ipsecHandle;
    uint8_t       ipOffset = pConnHdl->ipOffset[pIp->index];
    uint16_t      ipPayloadLen;
    
    /* fill in IP length before the call */
    ipPayloadLen = pPktDesc->size - ipOffset;
    
    if (pIp->type == SAU_IP_TYPE_IP6)
    {
        pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]), ipOffset + 4, ipPayloadLen - 40);
    }
    else
    {
        pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]), ipOffset + 2, ipPayloadLen);
    }

    if (pIpsec)
    {
        /* IPSEC operation */
        pPktDesc->payloadOffset = ipOffset;
        pPktDesc->payloadLen = ipPayloadLen;
        if (pIpsec->type == SAU_IPSEC_TYPE_ESP_NAT_T)
        {
            pPktInfo->validBitMap  =  sa_PKT_INFO_VALID_IPSEC_NAT_T_INFO;
            pPktInfo->natTInfo.dstPort = 4500;
            pPktInfo->natTInfo.srcPort = 4500;
            if (!nssGblCfgParams.layout.fNssGen2)
            {
                // It is already included at IPSEC match for NETCP 1.5 
 	            paTestExpectedStats.classify1.nPackets++;
                paTestExpectedStats.classify1.nTableMatch++;
            }
            paTestExpectedStats.classify2.nUdp++;
 	        paTestExpectedStats.classify2.nPackets++;
        }
        else
        {
            pPktInfo->validBitMap = 0;
        }
        Sa_chanSendData(pIpsec->pChan->salldInst, pPktInfo, FALSE);
        return (TRUE);
    }
    else
    {
        /* non-IPSEC operation */
        if (pIp->type == SAU_IP_TYPE_IP4)
        {
            utilSetIpv4ChkSum (((uint8_t *)pPktDesc->segments[0]) + ipOffset);
        }
        return (FALSE);
    }
}                     

Bool sauProcTxPktIpMultiSegment(sauConnHandle_t *pConnHdl, sauHdrHandle_t* pIpHdl,
                    Sa_PktInfo_t* pPktInfo, Sa_PktDesc_t* pPktDesc)
{
    sauIp_t*    pIp    = &pIpHdl->u.ip;
    sauIpsec_t* pIpsec = pIp->ipsecHandle;
    uint8_t       ipOffset = pConnHdl->ipOffset[pIp->index];
    uint16_t      ipPayloadLen;

    /* fill in IP length before the call */
    ipPayloadLen = pPktDesc->size - ipOffset;

    if (pIp->type == SAU_IP_TYPE_IP6)
    {
        pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]), ipOffset + 4, ipPayloadLen - 40);
    }
    else
    {
        pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]), ipOffset + 2, ipPayloadLen);
    }

    if (pIpsec)
    {
        /* IPSEC operation */
        pPktDesc->payloadOffset = ipOffset;
        pPktDesc->payloadLen = ipPayloadLen;
        pPktInfo->validBitMap = 0;
        Sa_chanSendData(pIpsec->pChan->salldInst, pPktInfo, FALSE);
        return (TRUE);
    }
    else
    {
        /* non-IPSEC operation */
        if (pIp->type == SAU_IP_TYPE_IP4)
        {
            utilSetIpv4ChkSum (((uint8_t *)pPktDesc->segments[0]) + ipOffset);
        }
        return (FALSE);
    }
}
/*******************************************************************************
 *  Function: Process the Tx packet for UDP  
 *******************************************************************************
 *  DESCRIPTION:  This function perform UDP operation of the Tx packet 
 *
 *  Return: TRUE: UDP/SRTP operation
 *          FALSE: UDP operation 
 ******************************************************************************/
Bool sauProcTxPktUdp(sauConnHandle_t *pConnHdl, sauHdrHandle_t* pUdpHdl, 
                     Sa_PktInfo_t* pPktInfo, Sa_PktDesc_t* pPktDesc,
                     paTxChksum_t* pUdpChksum)
{
    sauSrtp_t* pSrtp = pUdpHdl->u.udp.srtpHandle;
    sauSrtcp_t* pSrtcp = pUdpHdl->u.udp.srtcpHandle;
    sauIp_t*   pIp = &pUdpHdl->upHdl->u.ip;
    uint16_t udpPsudoChkSum, udpLen; 

    if (pSrtp)
    {
        /* SRTP operation */
        /* Insert RTP sequence number and etc */
        pktWrite16bits_m((uint8_t *)pPktDesc->segments[0], pConnHdl->rtpOffset + 2,
                       pSrtp->seqNum);
        pSrtp->seqNum++;   
                              
        /* Prepare and invoke SALLD SendData API */
        pPktDesc->payloadOffset = pConnHdl->rtpOffset;
        pPktDesc->payloadLen = (pPktDesc->size - pPktDesc->payloadOffset);
        pPktInfo->validBitMap = 0;
        /* 
         * It is just a simple hack to handle the MKI re-key test case.  
         * The salldSim_poll_chn() will always provide a new key since the request is made by the
         * by Sa_chanSendData().
         * Note: We need to pass some error code to the caller in general case.
         */
        while (Sa_chanSendData(pSrtp->pChan->salldInst, pPktInfo, FALSE) == sa_ERR_KEY_EXPIRED)
        {
            salldSim_poll_chn(pSrtp->pChan);
        }
        salldSim_poll_chn(pSrtp->pChan);
    }
    else if (pSrtcp)
    {
    
        /* SRTCP operation */
        
        /* Prepare and invoke SALLD SendData API */
        pPktDesc->payloadOffset = pConnHdl->rtpOffset;
        pPktDesc->payloadLen = (pPktDesc->size - pPktDesc->payloadOffset);
        pPktInfo->validBitMap = 0;
        
        /* Insert RTCP message length  and etc */
        pktWrite16bits_m((uint8_t *)pPktDesc->segments[0], pConnHdl->rtpOffset + 2,
                         pPktDesc->payloadLen >> 2);
        
        /* 
         * It is just a simple hack to handle the MKI re-key test case.  
         * The salldSim_poll_chn() will always provide a new key since the request is made by the
         * by Sa_chanSendData().
         * Note: We need to pass some error code to the caller in general case.
         */
        while (Sa_chanSendData(pSrtcp->pChan->salldInst, pPktInfo, FALSE) == sa_ERR_KEY_EXPIRED)
        {
            salldSim_poll_chn(pSrtcp->pChan);
        }
        salldSim_poll_chn(pSrtcp->pChan);
    }
    /* UDP operation */
    /* Assumption: UDP over the last IP in the connection */
    udpLen = pPktDesc->size - pConnHdl->udpOffset;
    pktWrite16bits_m(((uint8_t *)pPktDesc->segments[0]), pConnHdl->udpOffset + 4, udpLen);
    if (pIp->type == SAU_IP_TYPE_IP4)
    {
        udpPsudoChkSum = utilGetIpv4PsudoChkSum(((uint8_t *)pPktDesc->segments[0]) 
                                                + pConnHdl->ipOffset[pIp->index], 
                                                udpLen);
    }
    else
    {
        udpPsudoChkSum = utilGetIpv6PsudoChkSum(((uint8_t *)pPktDesc->segments[0]) 
                                                + pConnHdl->ipOffset[pIp->index], 
                                                udpLen);
    
    }                                          
                                              
    if (pSrtp)
    {
        pUdpChksum->lengthBytes = udpLen;
        pUdpChksum->initialSum  = udpPsudoChkSum;
        return TRUE;
    }
    else
    {
        /* Update the UDP checksum */
        utilSetUdpChkSum (((uint8_t *)pPktDesc->segments[0]) + pConnHdl->udpOffset, 
                          udpLen, udpPsudoChkSum);
        return FALSE;   
    
    }                                              
}                     

/*******************************************************************************
 *  Function: Generate Tx packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function prepares the tx packet ready to be send to PA/SA
 *      subsystem through the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Perform all the protocol-specific operations specified in the connection
 *        UDP: Update UDP length and calculate UDP checksum
 *        SRTP: Update sequence number, prepare and call salldSendData()
 *        IP: Update IP length, calculate IP checksum
 *        IPSEC: Prepare and call salldSendData()    
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the PA/SA commands 
 *        per connection 
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
Cppi_HostDesc* sauGenTxMultiSegmentPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen, uint8_t* payload, Bool corrupt, uint8_t mode)
{
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
    sauIpsec_t*     pIpsecHdl;
 	Cppi_HostDesc *hd, *hd2, *hd3;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    uint32_t      psInfo[16];
    int           psInfoIndex = 0;
    uint16_t      pktLen, hdrLen, rtpPayloadSize;
    uint32_t      origHdrLoc;
    uint16_t      offsetAdjust = 0;
    int           index;
    Bool          fSrtp = FALSE;
    Bool          fIpsec = FALSE;
	paReturn_t    paret;
    uint16_t      cmdSize;
    #if SA_GEN_TEST_VECTOR_TX
    static Bool   first[10] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
    #endif

    paTxChksum_t  pktUdpChksum = {
        0,  /* Start offset of UDP header */
        0,  /* Checksum length (UDP payload + UDP checksum */
        6,  /* Offset to checksum location RELATIVE TO THE START OF THE UDP HEADER */
        0,  /* Initial value is IPv4 pseudo header checksum value */
        1   /* computed value of 0 written as -0 */
    };

    paPatchInfo_t ahPatchInfo = {
		0,     /* Patch */
        0,     /* The number of bytes to be patched */
        16,    /* The number of patch bytes in the patch command, must be >= to nPatchBytes and a multiple of 4 bytes */
        0,     /* Offset from the start of the packet for the patch to begin */
        NULL   /* Pointer to the patch data */
    };

    paRouteInfo_t *pRouteInfo;

    if (connIndex >= SA_TEST_MAX_CONNS)
    {
        salld_sim_print("sauGenTxPkt: connIndex(%d) exceeds its range\n", connIndex);
        return NULL;
    }


    hdrLen = pConnHdl->hdrLen;
    pktLen = payloadLen + hdrLen;

    hd = testCommonGetBuffer(tf, (Int)(pktLen + SA_PKT_MARGIN));
	if (hd == NULL)  {
        salld_sim_print("sauGenTxPkt: no link buffer available\n");
        return NULL;
	}
	hd->nextBDPtr = (uint32_t)NULL;
	if(mode > ONE_SEGMENT)
	{
		hd2 = testCommonGetBuffer(tf, (Int)(payloadLen + SA_PKT_MARGIN));
		if (hd2 == NULL)  {
			salld_sim_print("sauGenTxPkt: no link buffer available\n");
			return NULL;
		}
		hd2->nextBDPtr = (uint32_t)NULL;
	}

	if(mode == THREE_SEGMENT)
		{
			hd3 = testCommonGetBuffer(tf, (Int)(payloadLen + SA_PKT_MARGIN));
			if (hd3 == NULL)  {
				salld_sim_print("sauGenTxPkt: no link buffer available\n");
				return NULL;
			}
			hd3->nextBDPtr = (uint32_t)NULL;
		}

    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, mode+1);
    pPktDesc->size = pktLen;
    pPktDesc->segments[0] = (void *)(hd->buffPtr + SA_PKT_HDR_MARGIN); /* reserve room for potential IPSEC Header insertion */


    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    origHdrLoc = (uint32_t)pPktDesc->segments[0];

	/* Construct the original packet and update the first 32-bit payload with connection ID */
    if(mode == ONE_SEGMENT)
	{
		pPktDesc->segUsedSizes[0] = pPktDesc->size;
		memcpy(pPktDesc->segments[0], pConnHdl->hdr,  pConnHdl->hdrLen);
		memcpy(((uint8_t *)pPktDesc->segments[0]) + pConnHdl->hdrLen, payload,  payloadLen);
		pktWrite32bits_m((uint8_t *)pPktDesc->segments[0], pConnHdl->hdrLen, (uint32_t)connIndex);

	}
	else if(mode == TWO_SEGMENT)
	{
		pPktDesc->segments[1] = (void *)(hd2->buffPtr + SA_PKT_HDR_MARGIN);
		pPktDesc->segUsedSizes[0] = hdrLen;
		pPktDesc->segUsedSizes[1] = payloadLen;
		pPktDesc->segAllocSizes[1] = hd2->origBufferLen;
		memcpy(pPktDesc->segments[0], pConnHdl->hdr,  hdrLen);
		memcpy(pPktDesc->segments[1], payload,  payloadLen);
		pktWrite32bits_m((uint8_t *)pPktDesc->segments[1], 0, (uint32_t)connIndex);
	}
	else if(mode == THREE_SEGMENT)
	{
		pPktDesc->segments[1] = (void *)(hd2->buffPtr + SA_PKT_HDR_MARGIN);
		pPktDesc->segments[2] = (void *)(hd3->buffPtr + SA_PKT_HDR_MARGIN);
		pPktDesc->segUsedSizes[0] = 14;
		pPktDesc->segUsedSizes[1] = hdrLen-14;
		pPktDesc->segUsedSizes[1] = payloadLen;
		pPktDesc->segAllocSizes[1] = hd2->origBufferLen;
		pPktDesc->segAllocSizes[2] = hd3->origBufferLen;
		memcpy(pPktDesc->segments[0], pConnHdl->hdr, 14);              /* MAC header */
		memcpy(pPktDesc->segments[1], (pConnHdl->hdr)+14, hdrLen-14);  /* Non-MAC header */
		memcpy(pPktDesc->segments[2], payload,  payloadLen);

		pktWrite32bits_m((uint8_t *)pPktDesc->segments[2], 0, (uint32_t)connIndex);
	}


    /* Perform protocol-specific operation in reverse order */
    for (index = 0; index < pConnHdl->numHandles; index++)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];

        switch (pHdrHdl->hdrType)
        {
            case SAU_PROTO_MAC:
 	            paTestExpectedStats.classify1.nPackets++;
                paTestExpectedStats.classify1.nTableMatch++;
            break;

            case SAU_PROTO_IP:
 	            paTestExpectedStats.classify1.nPackets++;
                paTestExpectedStats.classify1.nTableMatch++;
                if(pHdrHdl->u.ip.type == SAU_IP_TYPE_IP4)
                    paTestExpectedStats.classify1.nIpv4Packets++;
                else
                    paTestExpectedStats.classify1.nIpv6Packets++;

                if(mode == THREE_SEGMENT){
                	if (sauProcTxPktIpMultiSegment(pConnHdl, pHdrHdl, &pktInfo, pPktDesc))
                	{
                        if (nssGblCfgParams.layout.fNssGen2)
                        {
                            paTestExpectedStats.classify1.nTableMatch++;
 	                        paTestExpectedStats.classify1.nPackets++;
                        }
                   		fIpsec = TRUE;
                		pIpsecHdl = pHdrHdl->u.ip.ipsecHandle;
                		offsetAdjust = (uint16_t)(origHdrLoc - (uint32_t)pPktDesc->segments[0]);
                		pIpsecHdl->hdrSize = (uint8_t)offsetAdjust;

                		if (corrupt && (pIpsecHdl->type == SAU_IPSEC_TYPE_AH))
                		{
                			uint8_t offset = pConnHdl->ipOffset[pHdrHdl->u.ip.index];
                			uint8_t *pData = (uint8_t *)pPktDesc->segments[0];
                			uint8_t data;

                			/* Version number 4/5 ==> 5/7) */
                			data =  pktRead8bits_m((uint8_t*)pData, offset) | 0x10;
                			pktWrite8bits_m((uint8_t*)pData, offset, data);
                		}
                	}
                }
                else if (sauProcTxPktIp(pConnHdl, pHdrHdl, &pktInfo, pPktDesc))
                {
                    if (nssGblCfgParams.layout.fNssGen2)
                    {
                        paTestExpectedStats.classify1.nTableMatch++;
 	                    paTestExpectedStats.classify1.nPackets++;
                    }
                    fIpsec = TRUE;
                    pIpsecHdl = pHdrHdl->u.ip.ipsecHandle;
                    offsetAdjust = (uint16_t)(origHdrLoc - (uint32_t)pPktDesc->segments[0]);
                    pIpsecHdl->hdrSize = (uint8_t)offsetAdjust;

                    if (corrupt && (pIpsecHdl->type == SAU_IPSEC_TYPE_AH))
                    {
                        uint8_t offset = pConnHdl->ipOffset[pHdrHdl->u.ip.index];
                        uint8_t *pData = (uint8_t *)pPktDesc->segments[0];
                        uint8_t data;

                        /* Version number 4/5 ==> 5/7) */
                        data =  pktRead8bits_m((uint8_t*)pData, offset) | 0x10;
                        pktWrite8bits_m((uint8_t*)pData, offset, data);
                    }
                }
            break;

            case SAU_PROTO_UDP:
                paTestExpectedStats.classify2.nUdp++;
 	            paTestExpectedStats.classify2.nPackets++;
                if (sauProcTxPktUdp(pConnHdl, pHdrHdl, &pktInfo, pPktDesc, &pktUdpChksum))
                {
                    fSrtp = TRUE;
                    rtpPayloadSize = pPktDesc->payloadLen;

  	                Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);

                }
            break;

            default:
                /* Should never enter here */
                salld_sim_print("sauGenTxPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n",
                                 index, pHdrHdl->hdrType);
                return(NULL);
        }
    }

    /* Prepare to send pkt to PA/SA */
  	if(!mode)
	{
		Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	}
	else if(mode == TWO_SEGMENT)
	{
		Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->segUsedSizes[0]);
		Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd2, (Ptr)pPktDesc->segments[1], pPktDesc->segUsedSizes[1]);
		Cppi_linkNextBD(Cppi_DescType_HOST,(Cppi_Desc *)hd,(Cppi_Desc *)hd2);
	}
	else if (mode == THREE_SEGMENT)
	{
		Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->segUsedSizes[0]);
		Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd2, (Ptr)pPktDesc->segments[1], pPktDesc->segUsedSizes[1]);
		Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd3, (Ptr)pPktDesc->segments[2], pPktDesc->segUsedSizes[2]);
		Cppi_linkNextBD(Cppi_DescType_HOST,(Cppi_Desc *)hd,(Cppi_Desc *)hd2);
		Cppi_linkNextBD(Cppi_DescType_HOST,(Cppi_Desc *)hd2,(Cppi_Desc *)hd3);
	}
	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);

    /* Prepare for tx commands in the PS Info */
    /*
     * The following commands may be passed in the PS Info section
     * - Payload Info (short format) for SRTP
     * - UDP checksum
     * - Routing Info to SA (For IPSEC) or Host (SRTP only)
     * - Payload Info (short format) for IPSEC
     * - Routing Info + Patch command for IPSEC AH
     */
    if (fSrtp)
    {
        psInfo[psInfoIndex] = PASAHO_SINFO_FORMAT_CMD(pConnHdl->rtpOffset + offsetAdjust,
                                                      rtpPayloadSize);
        psInfoIndex += 2;
        pktUdpChksum.startOffset = pConnHdl->udpOffset + offsetAdjust;

        if (fIpsec)
        {
            /* SRTP over IPSEC */
            pRouteInfo = &sauSaRoute;
            pRouteInfo->swInfo0 = pktInfo.swInfo.swInfo[0];
            pRouteInfo->swInfo1 = pktInfo.swInfo.swInfo[1];
        }
        else
        {
            /* SRTP only */
            #if SAU_TXPKT_TO_HOST
            pRouteInfo = &sauHostRouteTxPkt;
            #else
            pRouteInfo = &sauPaRouteTxPkt;
            #endif
            pRouteInfo->swInfo0 = 0x55550000 | (uint16_t)connIndex;
        }

        cmdSize = sizeof(psInfo) - psInfoIndex * 4;

        paret = Pa_formatTxRoute (  &pktUdpChksum,    /* UDP checksum */
                                    NULL,             /* No second checksum */
                                    pRouteInfo,       /* Internal routing */
                                    (Ptr)&psInfo[psInfoIndex], /* Command buffer */
                                    &cmdSize);           /* Command size */

        if (paret != pa_OK)  {
            salld_sim_print ("sauGenTxPkt: Pa_formatTxRoute returned error code %d\n", paret);
	        testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
            return NULL;
        }
        psInfoIndex += (cmdSize/4);
    }

    if (fIpsec)
    {
        psInfo[psInfoIndex] = PASAHO_SINFO_FORMAT_CMD(pPktDesc->payloadOffset, pPktDesc->payloadLen);
        psInfoIndex += 2;
        if (!fSrtp)
        {
  	        Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
        }

        if (pIpsecHdl->type == SAU_IPSEC_TYPE_AH)
        {
            ahPatchInfo.nPatchBytes = pIpsecHdl->macSize;
            ahPatchInfo.offset = pConnHdl->ipsecOffset + 12 + pIpsecHdl->ahIvSize;
            #if SAU_TXPKT_TO_HOST
            pRouteInfo = &sauHostRouteTxPkt;
            #else
            pRouteInfo = &sauPaRouteTxPkt;
            #endif
            pRouteInfo->swInfo0 = 0x55550000 | (uint16_t)connIndex;
            cmdSize = sizeof(psInfo) - psInfoIndex * 4;

            paret = Pa_formatRoutePatch (pRouteInfo,       /* Internal routing     */
                                         &ahPatchInfo,     /* Patch Control */
                                        (Ptr)&psInfo[psInfoIndex], /* Command buffer       */
                                         &cmdSize);           /* Command size         */

            if (paret != pa_OK)  {
                salld_sim_print ("sauGenTxPkt: Pa_formatRoutePatch returned error code %d\n", paret);
	            testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
                return NULL;
            }
            psInfoIndex += (cmdSize/4);
        }
    }

    /* Attach the command in PS data */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, psInfoIndex * 4);

    return (hd);
}

/*******************************************************************************
 *  Function: Generate Tx packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function prepares the tx packet ready to be send to PA/SA
 *      subsystem through the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Perform all the protocol-specific operations specified in the connection
 *        UDP: Update UDP length and calculate UDP checksum
 *        SRTP: Update sequence number, prepare and call salldSendData()
 *        IP: Update IP length, calculate IP checksum
 *        IPSEC: Prepare and call salldSendData()    
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the PA/SA commands 
 *        per connection 
 *		- Mode: 0 - Single Segment
 *				1 - Two Segment, with Header in 1st Segment
 *				2 - Three Segment, with Mac and IP in separate segments
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
Cppi_HostDesc* sauGenTxPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen, uint8_t* payload, Bool corrupt)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
    sauIpsec_t*     pIpsecHdl;
 	Cppi_HostDesc *hd;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    uint32_t      psInfo[16];
    int           psInfoIndex = 0; 
    uint16_t      pktLen, rtpPayloadSize;
    uint32_t      origHdrLoc;
    uint16_t      offsetAdjust = 0;
    int           index;
    Bool          fSrtp = FALSE;
    Bool          fIpsec = FALSE;
	paReturn_t    paret;
    uint16_t      cmdSize;
    #if SA_GEN_TEST_VECTOR_TX
    static Bool   first[10] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
    #endif
    
    paTxChksum_t  pktUdpChksum = {
        0,  /* Start offset of UDP header */
        0,  /* Checksum length (UDP payload + UDP checksum */
        6,  /* Offset to checksum location RELATIVE TO THE START OF THE UDP HEADER */
        0,  /* Initial value is IPv4 pseudo header checksum value */
        1   /* computed value of 0 written as -0 */
    };
    
    paPatchInfo_t ahPatchInfo = {
        0,     /* Patch */
        0,     /* The number of bytes to be patched */
        16,    /* The number of patch bytes in the patch command, must be >= to nPatchBytes and a multiple of 4 bytes */
        0,     /* Offset from the start of the packet for the patch to begin */
        NULL   /* Pointer to the patch data */
    };

    paRouteInfo_t *pRouteInfo; 
    
    if (connIndex >= SA_TEST_MAX_CONNS)
    {
        salld_sim_print("sauGenTxPkt: connIndex(%d) exceeds its range\n", connIndex);
        return NULL;
    }
    
    pktLen = payloadLen + pConnHdl->hdrLen;

    hd = testCommonGetBuffer(tf, (Int)(pktLen + SA_PKT_MARGIN));
	if (hd == NULL)  {
        salld_sim_print("sauGenTxPkt: no link buffer available\n");
        return NULL;
	}
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    pPktDesc->size = pktLen;
    pPktDesc->segments[0] = (void *)(hd->buffPtr + SA_PKT_HDR_MARGIN); /* reserve room for potential IPSEC Header insertion */
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    origHdrLoc = (uint32_t)pPktDesc->segments[0];
    
    /* Construct the original packet and update the first 32-bit payload with connection ID */
    memcpy(pPktDesc->segments[0], pConnHdl->hdr,  pConnHdl->hdrLen);
    memcpy(((uint8_t *)pPktDesc->segments[0]) + pConnHdl->hdrLen, payload,  payloadLen);
    pktWrite32bits_m((uint8_t *)pPktDesc->segments[0], pConnHdl->hdrLen, (uint32_t)connIndex);
    
    /* Perform protocol-specific operation in reverse order */
    for (index = 0; index < pConnHdl->numHandles; index++)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];
        
        switch (pHdrHdl->hdrType)
        {
            case SAU_PROTO_MAC:
 	            paTestExpectedStats.classify1.nPackets++;
                paTestExpectedStats.classify1.nTableMatch++;
            break;
            
            case SAU_PROTO_IP:
 	            paTestExpectedStats.classify1.nPackets++;
                paTestExpectedStats.classify1.nTableMatch++;
                if(pHdrHdl->u.ip.type == SAU_IP_TYPE_IP4)
                    paTestExpectedStats.classify1.nIpv4Packets++;
                else
                    paTestExpectedStats.classify1.nIpv6Packets++;
                    
                if (sauProcTxPktIp(pConnHdl, pHdrHdl, &pktInfo, pPktDesc))
                {
                    if (nssGblCfgParams.layout.fNssGen2)
                    {
                        paTestExpectedStats.classify1.nTableMatch++;
 	                    paTestExpectedStats.classify1.nPackets++;
                    }
                    fIpsec = TRUE; 
                    pIpsecHdl = pHdrHdl->u.ip.ipsecHandle;   
                    offsetAdjust = (uint16_t)(origHdrLoc - (uint32_t)pPktDesc->segments[0]);
                    pIpsecHdl->hdrSize = (uint8_t)offsetAdjust;
                    
                    if (corrupt && (pIpsecHdl->type == SAU_IPSEC_TYPE_AH))
                    {
                        uint8_t offset = pConnHdl->ipOffset[pHdrHdl->u.ip.index];
                        uint8_t *pData = (uint8_t *)pPktDesc->segments[0];
                        uint8_t data;
                        
                        /* Version number 4/5 ==> 5/7) */
                        data =  pktRead8bits_m((uint8_t*)pData, offset) | 0x10;
                        pktWrite8bits_m((uint8_t*)pData, offset, data);
                    }
                }
            break;
        
            case SAU_PROTO_UDP:
                paTestExpectedStats.classify2.nUdp++;
 	            paTestExpectedStats.classify2.nPackets++;
                if (sauProcTxPktUdp(pConnHdl, pHdrHdl, &pktInfo, pPktDesc, &pktUdpChksum))
                {
                    fSrtp = TRUE;    
                    rtpPayloadSize = pPktDesc->payloadLen;
                    
  	                Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
                    
                }
            break;
            
            default:
                /* Should never enter here */
                salld_sim_print("sauGenTxPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n", 
                                 index, pHdrHdl->hdrType);
                return(NULL);                  
        }
    }
    
    /* Prepare to send pkt to PA/SA */
  	Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
    
    /* Prepare for tx commands in the PS Info */
    /*
     * The following commands may be passed in the PS Info section
     * - Payload Info (short format) for SRTP
     * - UDP checksum
     * - Routing Info to SA (For IPSEC) or Host (SRTP only)
     * - Payload Info (short format) for IPSEC
     * - Routing Info + Patch command for IPSEC AH
     */
    if (fSrtp)
    {
        psInfo[psInfoIndex] = PASAHO_SINFO_FORMAT_CMD(pConnHdl->rtpOffset + offsetAdjust, 
                                                      rtpPayloadSize);
        psInfoIndex += 2;                                                        
        pktUdpChksum.startOffset = pConnHdl->udpOffset + offsetAdjust;
        
        if (fIpsec)
        {
            /* SRTP over IPSEC */
            pRouteInfo = &sauSaRoute;
            pRouteInfo->swInfo0 = pktInfo.swInfo.swInfo[0];
            pRouteInfo->swInfo1 = pktInfo.swInfo.swInfo[1];
        }
        else
        {
            /* SRTP only */
            #if SAU_TXPKT_TO_HOST
            pRouteInfo = &sauHostRouteTxPkt;
            #else
            pRouteInfo = &sauPaRouteTxPkt;
            #endif
            pRouteInfo->swInfo0 = 0x55550000 | (uint16_t)connIndex;
        }
        
        cmdSize = sizeof(psInfo) - psInfoIndex * 4;
        
        paret = Pa_formatTxRoute (  &pktUdpChksum,    /* UDP checksum */
                                    NULL,             /* No second checksum   */
                                    pRouteInfo,       /* Internal routing     */
                                    (Ptr)&psInfo[psInfoIndex], /* Command buffer       */
                                    &cmdSize);           /* Command size         */

        if (paret != pa_OK)  {
            salld_sim_print ("sauGenTxPkt: Pa_formatTxRoute returned error code %d\n", paret);
	        testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
            return NULL;
        }
        psInfoIndex += (cmdSize/4);
    }
    
    if (fIpsec)
    {
        psInfo[psInfoIndex] = PASAHO_SINFO_FORMAT_CMD(pPktDesc->payloadOffset, pPktDesc->payloadLen);
        psInfoIndex += 2;                                                        
        if (!fSrtp)
        {
  	        Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
        }
        
        if (pIpsecHdl->type == SAU_IPSEC_TYPE_AH)
        {
            ahPatchInfo.nPatchBytes = pIpsecHdl->macSize;
            ahPatchInfo.offset = pConnHdl->ipsecOffset + 12 + pIpsecHdl->ahIvSize;    
            #if SAU_TXPKT_TO_HOST
            pRouteInfo = &sauHostRouteTxPkt;
            #else
            pRouteInfo = &sauPaRouteTxPkt;
            #endif
            pRouteInfo->swInfo0 = 0x55550000 | (uint16_t)connIndex;
            cmdSize = sizeof(psInfo) - psInfoIndex * 4;
            
            paret = Pa_formatRoutePatch (pRouteInfo,       /* Internal routing     */    
                                         &ahPatchInfo,     /* Patch Control */ 
                                        (Ptr)&psInfo[psInfoIndex], /* Command buffer       */
                                         &cmdSize);           /* Command size         */

            if (paret != pa_OK)  {
                salld_sim_print ("sauGenTxPkt: Pa_formatRoutePatch returned error code %d\n", paret);
	            testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
                return NULL;
            }
            psInfoIndex += (cmdSize/4);
        } 
    }
    
    /* Attach the command in PS data */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, psInfoIndex * 4);
    #if SA_GEN_TEST_VECTOR_TX
    if(first[connIndex])
    {
        utilOutputScFromSwInfo(&tFramework, pktInfo.swInfo.swInfo);
        first[connIndex] = FALSE;
    }
    
    utilOutputPkt(&tFramework, pktInfo.swInfo.swInfo, psInfoIndex*4, psInfo,
                   pPktDesc->size, (uint8_t*) pPktDesc->segments[0], TRUE);
    #endif
    
    return (hd);
}

/*******************************************************************************
 *  Function: Update Tx packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function corrupts the tx payload so that the packet will
 *      will be received by PA, but rejected by SA due to authentication failure
 *
 ******************************************************************************/
void sauUpdateTxPkt(int connIndex, Cppi_HostDesc *hd)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
    sauIpsec_t* pIpsecHdl;
    uint8_t *pkt = (uint8_t *)hd->buffPtr;
    uint16_t offset = 0; 
    int     index;
    #if 0
    uint32_t  *psInfo;
	uint32_t  infoLen;
    #endif
    
    if (connIndex >= SA_TEST_MAX_CONNS)
    {
        salld_sim_print("sauUpdateTxPkt: connIndex(%d) exceeds its range\n", connIndex);
    }
    
    /* Perform protocol-specific operation in reverse order */
    for (index = pConnHdl->numHandles - 1; index >= 0; index--)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];
        
        switch (pHdrHdl->hdrType)
        {
            case SAU_PROTO_MAC:
            break;
            
            case SAU_PROTO_IP:
                if ((pIpsecHdl = pHdrHdl->u.ip.ipsecHandle))
                {
                    offset = pIpsecHdl->hdrSize;
                }
            break;
        
            case SAU_PROTO_UDP:
                offset += (pConnHdl->udpOffset + 8); /* offset to UDP paylaod */
                if (pHdrHdl->u.udp.srtpHandle)
                {
                    offset += 12;  /* offset to SRTP payload */
                }
                else if (pHdrHdl->u.udp.srtcpHandle)
                {
                    offset += 8;  /* offset to SRTCP payload */
                }
 
                pkt[offset]++;  /* Corrupt the first encrypted payload byte */
                   
            break;
            
            default:
                /* Should never enter here */
                salld_sim_print("sauUpdateTxPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n", 
                                 index, pHdrHdl->hdrType);
            break;                     
        }
    }
    
}


/*******************************************************************************
 *  Function: Process Rx packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function process the rx packet from the network through
 *      PA/SA subsystem via the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Perform all the protocol-specific operations specified in the connection
 *        UDP: Update UDP length and calculate UDP checksum
 *        SRTP: Update sequence number, prepare and call salldSendData()
 *        IP: Update IP length, calculate IP checksum
 *        IPSEC: Prepare and call salldSendData()    
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the PA/SA commands 
 *        per connection 
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauProcRxPkt(int connIndex, Cppi_HostDesc* hd)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    pasahoLongInfo_t	*pLongInfo;
    uint32_t  origHdrLoc, infoLen;
    int16_t   dataLen;
    int16_t   errCode = sa_ERR_OK;
    uint8_t     *pData;
    uint32_t    rcvConnId;
    int i;
    
    Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&pLongInfo, &infoLen);
        
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    pPktDesc->size = hd->buffLen;
    pPktDesc->segments[0] = (void *)hd->buffPtr; 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    origHdrLoc = (uint32_t)pPktDesc->segments[0];
    pktInfo.validBitMap = sa_PKT_INFO_VALID_PKT_ERR_CODE;
    
    pktInfo.pktErrCode = Cppi_getDescError(Cppi_DescType_HOST, (Cppi_Desc *)hd);
    
    //if (pktInfo.pktErrCode != sa_PKT_ERR_OK)
    //{
    //    salld_sim_print("Receive Pkt of connection %d from PA/SA with pktErrCode = %d\n", connIndex, pktInfo.pktErrCode);
    //}
    
    if (pConnHdl->ipsecIndex != SA_CONN_INDEX_NONE)
    {
        sauIpsec_t* pIpsec;
        
        pHdrHdl = pConnHdl->pHdrHdl[pConnHdl->ipsecIndex]; 
        pIpsec = pHdrHdl->u.ip.ipsecHandle;
        
        /* Perform IPSEC related operation */
        /* TBE: need to get the extra IP offset */
        pPktDesc->payloadOffset = PASAHO_LINFO_READ_L3_OFFSET(pLongInfo);   /* Pointer to IP header */
        pPktDesc->payloadLen = (pPktDesc->size - pPktDesc->payloadOffset);
        
        /* Call SA LLD API to perform Protocol Specific Operation */
        pktInfo.pktErrCode = sa_PKT_ERR_OK;

        /* Set rx payload information */
        pktInfo.validBitMap |= sa_PKT_INFO_VALID_RX_PAYLOAD_INFO;
        pktInfo.rxPayloadInfo.ipOffset = pPktDesc->payloadOffset;
        if (PASAHO_LINFO_READ_IP_COUNT(pLongInfo) > 1)
        {
            pktInfo.rxPayloadInfo.ipOffset2 = PASAHO_LINFO_READ_INNER_IP_OFFSET(pLongInfo);
        }
        else
        {
            pktInfo.rxPayloadInfo.ipOffset2 = 0;
        }

        errCode = Sa_chanReceiveData(pIpsec->pChan->salldInst, &pktInfo);
        
    }
    
    if (pConnHdl->srtpIndex != SA_CONN_INDEX_NONE)
    {
        sauSrtp_t* pSrtp;
        
        pHdrHdl = pConnHdl->pHdrHdl[pConnHdl->srtpIndex]; 
        pSrtp = pHdrHdl->u.udp.srtpHandle;
        
        /* SRTP Rx processing */
        pPktDesc->payloadLen = PASAHO_LINFO_READ_END_OFFSET(pLongInfo) - 
                               PASAHO_LINFO_READ_START_OFFSET(pLongInfo);
                               
        pPktDesc->payloadOffset = PASAHO_LINFO_READ_START_OFFSET(pLongInfo) - 
                                  ((uint32_t)pPktDesc->segments[0] - origHdrLoc);                 
                        
        /* Call SA LLD API to perform Protocol Specific Operation */
        pktInfo.pktErrCode = Cppi_getDescError(Cppi_DescType_HOST, (Cppi_Desc *)hd);
        errCode = Sa_chanReceiveData(pSrtp->pChan->salldInst, &pktInfo);
    }
    else if (pConnHdl->srtcpIndex != SA_CONN_INDEX_NONE)
    {
        sauSrtcp_t* pSrtcp;
        
        pktInfo.validBitMap = 0;
        pktInfo.pktErrCode = 0;
        
        pHdrHdl = pConnHdl->pHdrHdl[pConnHdl->srtcpIndex]; 
        pSrtcp = pHdrHdl->u.udp.srtcpHandle;
        
        /* SRTP Rx processing */
        pPktDesc->payloadLen = PASAHO_LINFO_READ_END_OFFSET(pLongInfo) - 
                               PASAHO_LINFO_READ_START_OFFSET(pLongInfo);
                               
        pPktDesc->payloadOffset = PASAHO_LINFO_READ_START_OFFSET(pLongInfo) - 
                                  ((uint32_t)pPktDesc->segments[0] - origHdrLoc);                 
                        
        /* Call SA LLD API to perform Protocol Specific Operation */
        errCode = Sa_chanReceiveData(pSrtcp->pChan->salldInst, &pktInfo);
    }
    
    /*
     * Receive packet error check
     * 
     */
    
    salld_sim_disp_control(TRUE);    
    
    if ((errCode != sa_ERR_OK) && ((errCode > sa_ERR_KEY_EXPIRED) || (errCode < sa_ERR_AUTH_FAIL)))  
    {
        salld_sim_print("Receive Pkt of connection %d from PA/SA with errCode = %d\n", connIndex, errCode);
    }    
    
    if (errCode == sa_ERR_OK)
    {
        /* Verify the decoded packet */
        rcvConnId = pktRead32bits_m((uint8_t *)pPktDesc->segments[0], pConnHdl->hdrLen);
    
        if (rcvConnId != connIndex)
        {
            salld_sim_print("Receive Pkt of connection %d from PA/SA with incorrect connId = %d\n", connIndex, rcvConnId); 
        }
    
        pData = (uint8_t *)pPktDesc->segments[0] + pConnHdl->hdrLen + 4;
    
        dataLen = pPktDesc->size - pConnHdl->hdrLen - 4 - 1;
    
        for ( i = 0; i < dataLen; i++)
        {
            if ((uint8_t)(pData[i + 1] - pData[i]) != 1)
            {
                salld_sim_print("Receive Pkt of connection %d from PA/SA with incorrect data at location %d, data = 0x%02x\n", 
                                connIndex, pConnHdl->hdrLen + 4 + i, pData[i]); 
                break;
            } 
        }
    }
    salld_sim_disp_control(FALSE);    
    
    /* Verify payload only if there is no re-key Error (errCode) */
    testDispPkts(hd);
    
    /* free buffer */
	testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
}

void sauProcRxMultiSgmtPkt(int connIndex, Cppi_HostDesc* hd)
{
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    pasahoLongInfo_t	*pLongInfo;
    uint32_t  origHdrLoc, infoLen;
    int16_t   errCode = sa_ERR_OK;
    int buffOffset = 0;
    int fixedBuffLen = 256, buffRem, i;
    buffRem = hd->buffLen;
    int nSegments = buffRem / fixedBuffLen + (buffRem%fixedBuffLen? 1:0);
    
    if(nSegments > SALLD_UNIT_TEST_MAX_SEGMENTS)
    {
    	salld_sim_print("Test Code Error: Attempt to allocate more than %d buffers\n", SALLD_UNIT_TEST_MAX_SEGMENTS);
    }
    
    Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&pLongInfo, &infoLen);

    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, nSegments);

    for(i = 0; i < nSegments && buffRem > 0; i++)
    {
    	pPktDesc->segments[i] = (void*) (hd->buffPtr + buffOffset);
    	if(buffRem >= fixedBuffLen){
    		pPktDesc->segUsedSizes[i] = fixedBuffLen;
    		pPktDesc->segAllocSizes[i] = fixedBuffLen;
    	}
    	else
    	{
    		pPktDesc->segUsedSizes[i] = buffRem;
    		pPktDesc->segAllocSizes[i] = buffRem;
    	}
    	buffRem -= fixedBuffLen;
    	buffOffset += fixedBuffLen;
    }
    pPktDesc->size = hd->buffLen;
    origHdrLoc = (uint32_t)pPktDesc->segments[0];
    pktInfo.validBitMap = sa_PKT_INFO_VALID_PKT_ERR_CODE;
    pktInfo.pktErrCode = Cppi_getDescError(Cppi_DescType_HOST, (Cppi_Desc *)hd);

    if (pConnHdl->ipsecIndex != SA_CONN_INDEX_NONE)
    {
        sauIpsec_t* pIpsec;

        pHdrHdl = pConnHdl->pHdrHdl[pConnHdl->ipsecIndex];
        pIpsec = pHdrHdl->u.ip.ipsecHandle;

        /* Perform IPSEC related operation */
        /* TBE: need to get the extra IP offset */
        pPktDesc->payloadOffset = PASAHO_LINFO_READ_L3_OFFSET(pLongInfo);   /* Pointer to IP header */
        pPktDesc->payloadLen = (pPktDesc->size - pPktDesc->payloadOffset);

        /* Set rx payload information */
        pktInfo.validBitMap |= sa_PKT_INFO_VALID_RX_PAYLOAD_INFO;
        pktInfo.rxPayloadInfo.ipOffset = pPktDesc->payloadOffset;
        if (PASAHO_LINFO_READ_IP_COUNT(pLongInfo) > 1)
        {
            pktInfo.rxPayloadInfo.ipOffset2 = PASAHO_LINFO_READ_INNER_IP_OFFSET(pLongInfo);
        }
        else
        {
            pktInfo.rxPayloadInfo.ipOffset2 = 0;
        }

        /* Call SA LLD API to perform Protocol Specific Operation */
        Sa_chanReceiveData(pIpsec->pChan->salldInst, &pktInfo);
    }

    if (pConnHdl->srtpIndex != SA_CONN_INDEX_NONE)
    {
        sauSrtp_t* pSrtp;

        pHdrHdl = pConnHdl->pHdrHdl[pConnHdl->srtpIndex];
        pSrtp = pHdrHdl->u.udp.srtpHandle;

        /* SRTP Rx processing */
        pPktDesc->payloadLen = PASAHO_LINFO_READ_END_OFFSET(pLongInfo) -
                               PASAHO_LINFO_READ_START_OFFSET(pLongInfo);

        pPktDesc->payloadOffset = PASAHO_LINFO_READ_START_OFFSET(pLongInfo) -
                                  ((uint32_t)pPktDesc->segments[0] - origHdrLoc);

        /* Call SA LLD API to perform Protocol Specific Operation */
        errCode = Sa_chanReceiveData(pSrtp->pChan->salldInst, &pktInfo);
    }
    else if (pConnHdl->srtcpIndex != SA_CONN_INDEX_NONE)
    {
    
        sauSrtcp_t* pSrtcp;
        
        pktInfo.validBitMap = 0;
        pktInfo.pktErrCode = 0;
        
        pHdrHdl = pConnHdl->pHdrHdl[pConnHdl->srtcpIndex]; 
        pSrtcp = pHdrHdl->u.udp.srtcpHandle;
        
        /* SRTP Rx processing */
        pPktDesc->payloadLen = PASAHO_LINFO_READ_END_OFFSET(pLongInfo) - 
                               PASAHO_LINFO_READ_START_OFFSET(pLongInfo);
                               
        pPktDesc->payloadOffset = PASAHO_LINFO_READ_START_OFFSET(pLongInfo) - 
                                  ((uint32_t)pPktDesc->segments[0] - origHdrLoc);                 
                        
        /* Call SA LLD API to perform Protocol Specific Operation */
        errCode = Sa_chanReceiveData(pSrtcp->pChan->salldInst, &pktInfo);
    }

    salld_sim_print("Receive Pkt of connection %d from PA/SA with errCode = %d\n", connIndex, errCode);

    /* Verify payload only if there is no re-key Error (errCode) */
    testDispPkts(hd);

    /* free buffer */
	testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */

}

extern Bool sauDebug;
extern void gpioSet (unsigned int v);

/*******************************************************************************
 *  Function: Connection Verification
 *******************************************************************************
 *  DESCRIPTION:  This function perform multi-connection multi-packet verification
 *      as specified below:
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauConnMultiSegmentPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                    uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate, uint8_t mode)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;
    uint16_t errPeriod = 0;
    Bool fHashErr = FALSE;
    Bool fProtoErr = FALSE;
    
    if (errRate >= 50)
    {
        errPeriod = 2;  
    }
    else if (errRate > 0)
    {
        errPeriod = 100/errRate;
    }

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        /* Set error flags */
        if (errPeriod)
        {
            fHashErr = ((pktNum % errPeriod) == 1);
            fProtoErr = ((pktNum % errPeriod) == 2);
        }
        
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
            hd = sauGenTxMultiSegmentPkt(tf, connIndex, payloadLen, sap_pkt_test_buf, fProtoErr, mode);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            //if((connIndex == 4) && (pktNum == 1))
            //mdebugHaltSaPdsp (1);
            //mdebugHaltPdsp (4);
            //gpioSet(1);
#ifdef      NSS_LITE
            Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //if((connIndex == 4) && (pktNum == 1))
            //salld_sim_halt();
  	
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_TX */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (10000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX) > 0)
			        break;
	        }
	
	        if (i >= 200)  {
                if (!fProtoErr)
                {
                    salld_sim_halt();
		            SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_TX);
  	                System_flush ();
 		            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                }
                continue;
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_TX) & ~15));
                salld_sim_print("Conn %d: Receive TxPkt %d from PA/SA\n", connIndex, pktNum); 
                
                #if (!SA_GEN_TEST_VECTOR_RX)
                testDispPkts(hd);
                #endif
                
                #if SA_GEN_TEST_VECTOR_TX
                gpioSet(0);
                break;
                #endif
                
                if (fHashErr)
                {
                    sauUpdateTxPkt(connIndex, hd);
                }
                
                /* Push Tx packet to the PA receive Queue */
                //if ((pktNum == 9))
                //if(fHashErr)
                //mdebugHaltSaPdsp (0);
                //mdebugHaltPdsp (1);
#ifdef NSS_LITE
                Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	            Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                //if (sauDebug && (pktNum == 8))
                //if(pktNum == 9)
                //if(fHashErr)
                //salld_sim_halt();
            }
            
            #if SA_GEN_TEST_VECTOR_TX
            continue;
            #endif
            
            #if SA_GEN_TEST_VECTOR_RX
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }
            
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
            {
                Sa_SWInfo_t*  pSwInfo;
                uint32_t*      psInfo;
                int            psInfoSize; 
                uint8_t*       pData;
                uint32_t       psInfoPatch[6];
            
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                 
                Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&psInfo, &psInfoSize);
                memcpy(psInfoPatch, psInfo, psInfoSize);
                psInfoSize = ((psInfoSize + 7)>>3) << 3;
                psInfoPatch[0] &= 0xffffff;
                psInfoPatch[0] |= (psInfoSize << 24);
                Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfoPatch, psInfoSize);
            
                /*
                 * TBD: Enhanced to support multiple channel test vector collection
                 *      Enhanced to support multiple channel per connection
                 *
                 */
                pSwInfo = &tf->salldSimChn[connIndex].regSwInfo;
            
                /* Prepare to send pkt to SA */
  	            Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pSwInfo->swInfo);
            
                /* patch psInfo for display */
                utilOutputPkt(tf, pSwInfo->swInfo, psInfoSize, psInfoPatch, hd->buffLen, (uint8_t *)hd->buffPtr, TRUE); 
            
                //mdebugHaltSaPdsp (0);
#ifdef NSS_LITE
                Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	            Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                //salld_sim_halt();
  	
  	            /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	            for (i = 0; i < 200; i++)  {
		            utilCycleDelay (1000);
		            if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			            break;
	            }
	
	            if (i >= 200)  {
		            salld_sim_halt();
		            SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	            }
	
	            /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
                while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
                {
	                hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                    testDispPkts(hd);   
                    
                    
                    #if 1
                    //mdebugHaltPdsp (2);
#ifdef NSS_LITE
                    Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qPaInnerIpIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	                Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qPaInnerIpIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                    //salld_sim_halt();
                    
  	                /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	                for (i = 0; i < 200; i++)  {
		                utilCycleDelay (1000);
		                if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			                break;
	                }
	
	                if (i >= 200)  {
		                SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		                //saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	                }
                    
                    if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV))
                    {
	                    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                    }
                    
                    #endif
                                                             
                
	                if (testCommonRecycleLBDesc (tf, hd))  {
		                SALog ("%s (%s:%d): Failure in testCommonRecycleLBDesc\n", pat->name, __FILE__, __LINE__);
 		                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	                }
                }
            }
            
            continue;
            
            #endif
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }
            
            #if 0
	
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
            
            #endif
	
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                salld_sim_print("Conn %d: Receive RxPkt %d from PA/SA\n", connIndex, pktNum);
                //if(pktNum == 21)
                //salld_sim_halt();
                if(mode == ONE_SEGMENT)
                	sauProcRxPkt(connIndex, hd);
                else
                	sauProcRxMultiSgmtPkt(connIndex, hd);
            }
        } /* Connection Loop */
    } /* Packet Loop */
}

/*******************************************************************************
 *  Function: Connection Verification
 *******************************************************************************
 *  DESCRIPTION:  This function perform multi-connection multi-packet verification
 *      as specified below:
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauConnMultiSegmentSrtcpPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                                     uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate, uint8_t mode)
{
 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
            hd = sauGenTxMultiSegmentPkt(tf, connIndex, payloadLen, sap_pkt_test_buf, FALSE, mode);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            /* Push Tx packet to the PA receive Queue */
            //mdebugHaltPdsp (1);
#ifdef NSS_LITE
            Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //salld_sim_halt();
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }
            
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
	
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                salld_sim_print("Conn %d: Receive RxPkt %d from PA/SA\n", connIndex, pktNum);
                sauProcRxMultiSgmtPkt(connIndex, hd);
            }
        } /* Connection Loop */
    } /* Packet Loop */
}



/*******************************************************************************
 *  Function: Connection Verification
 *******************************************************************************
 *  DESCRIPTION:  This function perform multi-connection multi-packet verification
 *      as specified below:
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauConnPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                    uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;
    uint16_t errPeriod = 0;
    Bool fHashErr = FALSE;
    Bool fProtoErr = FALSE;
    
    if (errRate >= 50)
    {
        errPeriod = 2;  
    }
    else if (errRate > 0)
    {
        errPeriod = 100/errRate;
    }

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        /* Set error flags */
        if (errPeriod)
        {
            fHashErr = ((pktNum % errPeriod) == 1);
            fProtoErr = ((pktNum % errPeriod) == 2);
        }
        
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
            hd = sauGenTxPkt(tf, connIndex, payloadLen, sap_pkt_test_buf, fProtoErr);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            //if((connIndex == 4) && (pktNum == 1))
            //mdebugHaltSaPdsp (1);
            //mdebugHaltPdsp (4);
            //gpioSet(1);
#ifdef NSS_LITE
Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //if((connIndex == 4) && (pktNum == 1))
            //salld_sim_halt();
  	
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_TX */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (10000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX) > 0)
			        break;
	        }
	
	        if (i >= 200)  {
                if (!fProtoErr)
                {
                    salld_sim_halt();
		            SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_TX);
  	                System_flush ();
 		            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                }
                continue;
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_TX) & ~15));
                salld_sim_print("Conn %d: Receive TxPkt %d from PA/SA\n", connIndex, pktNum); 
                
                #if (!SA_GEN_TEST_VECTOR_RX)
                testDispPkts(hd);
                #endif
                
                #if SA_GEN_TEST_VECTOR_TX
                gpioSet(0);
                break;
                #endif
                
                if (fHashErr)
                {
                    sauUpdateTxPkt(connIndex, hd);
                }
                
                /* Push Tx packet to the PA receive Queue */
                //if ((pktNum == 9))
                //if(fHashErr)
                //mdebugHaltSaPdsp (0);
                //mdebugHaltPdsp (1);
#ifdef NSS_LITE
                Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	            Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                //if (sauDebug && (pktNum == 8))
                //if(pktNum == 9)
                //if(fHashErr)
                //salld_sim_halt();
            }
            
            #if SA_GEN_TEST_VECTOR_TX
            continue;
            #endif
            
            #if SA_GEN_TEST_VECTOR_RX
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }
            
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
            {
                Sa_SWInfo_t*  pSwInfo;
                uint32_t*      psInfo;
                int            psInfoSize; 
                uint8_t*       pData;
                uint32_t       psInfoPatch[6];
            
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                 
                Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&psInfo, &psInfoSize);
                memcpy(psInfoPatch, psInfo, psInfoSize);
                psInfoSize = ((psInfoSize + 7)>>3) << 3;
                psInfoPatch[0] &= 0xffffff;
                psInfoPatch[0] |= (psInfoSize << 24);
                Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfoPatch, psInfoSize);
            
                /*
                 * TBD: Enhanced to support multiple channel test vector collection
                 *      Enhanced to support multiple channel per connection
                 *
                 */
                pSwInfo = &tf->salldSimChn[connIndex].regSwInfo;
            
                /* Prepare to send pkt to SA */
  	            Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pSwInfo->swInfo);
            
                /* patch psInfo for display */
                utilOutputPkt(tf, pSwInfo->swInfo, psInfoSize, psInfoPatch, hd->buffLen, (uint8_t *)hd->buffPtr, TRUE); 
            
                //mdebugHaltSaPdsp (0);
#ifdef NSS_LITE
                Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	            Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                //salld_sim_halt();
  	
  	            /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	            for (i = 0; i < 200; i++)  {
		            utilCycleDelay (1000);
		            if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			            break;
	            }
	
	            if (i >= 200)  {
		            salld_sim_halt();
		            SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	            }
	
	            /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
                while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
                {
	                hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                    testDispPkts(hd);   
                    
                    
                    #if 1
                    //mdebugHaltPdsp (2);
#ifdef NSS_LITE
                    Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qPaInnerIpIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	                Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qPaInnerIpIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                    //salld_sim_halt();
                    
  	                /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	                for (i = 0; i < 200; i++)  {
		                utilCycleDelay (1000);
		                if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			                break;
	                }
	
	                if (i >= 200)  {
		                SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		                //saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	                }
                    
                    if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV))
                    {
	                    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                    }
                    
                    #endif
                                                             
                
	                if (testCommonRecycleLBDesc (tf, hd))  {
		                SALog ("%s (%s:%d): Failure in testCommonRecycleLBDesc\n", pat->name, __FILE__, __LINE__);
 		                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	                }
                }
            }
            
            continue;
            
            #endif
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }
            
            #if 0
	
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
            
            #endif
	
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                salld_sim_print("Conn %d: Receive RxPkt %d from PA/SA\n", connIndex, pktNum);
                //if(pktNum == 21)
                //salld_sim_halt();
                 
                sauProcRxPkt(connIndex, hd);
            }
        } /* Connection Loop */
    } /* Packet Loop */
}


/*******************************************************************************
 *  Function: Connection Verification
 *******************************************************************************
 *  DESCRIPTION:  This function perform multi-connection multi-packet verification
 *      as specified below:
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauConnSrtcpPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                        uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;
    uint16_t errPeriod = 0;
    Bool fHashErr = FALSE;
    
    if (errRate >= 50)
    {
        errPeriod = 2;  
    }
    else if (errRate > 0)
    {
        errPeriod = 100/errRate;
    }

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        /* Set error flags */
        if (errPeriod)
        {
            fHashErr = ((pktNum % errPeriod) == 1);
        }
        
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
            hd = sauGenTxPkt(tf, connIndex, payloadLen, sap_pkt_test_buf, FALSE);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
            
            /* Should we introduce Hash Error */
            if (fHashErr)
            {
                sauUpdateTxPkt(connIndex, hd);
            }
            
            // Software test: SA is not involved 
            /* Push Tx packet to the PA receive Queue */
            //mdebugHaltPdsp (1);
#ifdef  NSS_LITE
            Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //salld_sim_halt();
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }
            
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
	
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                salld_sim_print("Conn %d: Receive RxPkt %d from PA/SA\n", connIndex, pktNum);
                //if(pktNum == 21)
                //salld_sim_halt();
                 
                sauProcRxPkt(connIndex, hd);
            }
        } /* Connection Loop */
    } /* Packet Loop */
}


/*******************************************************************************
 *  Function: Connection Verification
 *******************************************************************************
 *  DESCRIPTION:  This function perform multi-connection multi-packet verification
 *      as specified below:
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauMultiPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                     uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint8_t errRate)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;
    uint16_t errPeriod = 0;
    Bool fProtoErr = FALSE;
    
    if (errRate >= 50)
    {
        errPeriod = 2;  
    }
    else if (errRate > 0)
    {
        errPeriod = 100/errRate;
    }

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        /* Set error flags */
        if (errPeriod)
        {
            fProtoErr = ((pktNum % errPeriod) == 2);
        }
        
        /* Generate and send out tx packets for all connection */
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
            hd = sauGenTxPkt(tf, connIndex, payloadLen, sap_pkt_test_buf, fProtoErr);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            //if((connIndex == 4) && (pktNum == 1))
            //mdebugHaltSaPdsp (0);
            //mdebugHaltPdsp (4);
#ifdef NSS_LITE
            Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //if((connIndex == 4) && (pktNum == 1))
            //salld_sim_halt();
  	
        }
        
            
  	    /* Wait for all packets appears at queue DEST_QUEUE_PKT_RECV */	
	    for (i = 0; i < 200; i++)  {
		    utilCycleDelay (1000);
		    if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) >= connIndex)
		        break;
	    }
            
        #if 0
	
	    if (i >= 200)  {
		    salld_sim_halt();
		    SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		    saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	    }
        
        #endif
        
        /* Receive packet processing */
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
	        hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
            salld_sim_print("Conn %d: Receive RxPkt %d from PA/SA\n", connIndex, pktNum);
            sauProcRxPkt(connIndex, hd);
        } /* Connection Loop */
    } /* Packet Loop */
}


/*******************************************************************************
 *  Function: Process the KeyStram packet for Air Ciphering  
 *******************************************************************************
 *  DESCRIPTION:  This function perform AC operation of the Key stream packet 
 *
 ******************************************************************************/
static void sauProcKeyStreamPktAc(sauConnHandle_t *pConnHdl, sauHdrHandle_t* pAcHdl, 
                                  Sa_PktInfo_t* pPktInfo, Sa_PktDesc_t* pPktDesc)
{
    sauAc_t*  pAcHdr = &pAcHdl->u.ac;
    uint32_t mask = 0xFFFFFFFF;
    
    if (pAcHdr->cipherMode == sa_CipherMode_GSM_A53)
    {
        mask = SAU_KGCORE_COUNT_MASK_GSM_A53;
    }
    else if (pAcHdr->cipherMode == sa_CipherMode_ECSD_A53)
    {
        mask = SAU_KGCORE_COUNT_MASK_ECSD_A53;
    }
    
    pktWrite32bits_m((uint8_t *)pPktDesc->segments[0], 0, (pAcHdr->count32 & mask));
    
    pAcHdr->count32++;
    
    pPktDesc->payloadOffset = 0;
    pPktDesc->payloadLen = pPktDesc->size;
    pPktInfo->validBitMap = sa_PKT_INFO_VALID_PKT_ERR_CODE;
    pPktInfo->pktErrCode = sa_PKT_ERR_OK;
    
    Sa_chanReceiveData(pAcHdr->pChan->salldInst, pPktInfo);
}                     

/*******************************************************************************
 *  Function: Process the To-Air packet for Air Ciphering  
 *******************************************************************************
 *  DESCRIPTION:  This function perform AC operation of the To-Air packet 
 *
 ******************************************************************************/
static void sauProcToAirPktAc(sauConnHandle_t *pConnHdl, sauHdrHandle_t* pAcHdl, 
                              Sa_PktInfo_t* pPktInfo, Sa_PktDesc_t* pPktDesc, uint16_t payloadLen)
{
    sauAc_t*  pAcHdr = &pAcHdl->u.ac;
    
    if (pAcHdr->seqNumSize)
    {
        if (pAcHdr->seqNumSize == 1)
        {
            pktWrite8bits_m((uint8_t *)pPktDesc->segments[0], 0, (uint8_t)(pAcHdr->seqNum << 1));
            
        }   
        else
        {
            pktWrite16bits_m((uint8_t *)pPktDesc->segments[0], 0, (pAcHdr->seqNum << 3));
        }  
        /* To be enhanced */
        pktWrite8bits_m((uint8_t *)pPktDesc->segments[0], pAcHdr->seqNumSize, (uint8_t)((payloadLen << 1) + 1));
    }
    else if (pAcHdr->ivSize && (pAcHdr->pduType == sa_AcPduType_GSM))
    {
        /* Update the first four bytes of IV as count-C*/
        pktWrite32bits_m((uint8_t *)pPktDesc->segments[0], 0, pAcHdr->seqNum);
    }
    else if (pAcHdr->authHdrSize)
    {
        pktWrite8bits_m((uint8_t *)pPktDesc->segments[0], 0, (uint8_t)(pAcHdr->seqNum & 0x1F));
    }
    
    pAcHdr->seqNum++;
    
    pPktDesc->payloadOffset = 0;
    pPktDesc->payloadLen = pPktDesc->size;
    pPktInfo->validBitMap = sa_PKT_INFO_VALID_PKT_ERR_CODE;
    pPktInfo->pktErrCode = sa_PKT_ERR_OK;
    
    Sa_chanReceiveData(pAcHdr->pChan->salldInst, pPktInfo);
}                     

#undef SAU_TEST_LONG_INFO

/*******************************************************************************
 *  Function: Generate Air Ciphering packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function prepares the Data Mode packet ready to be send to PA/SA
 *      subsystem through the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Prepare and call Sa_chanReceiveData()
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the SA commands 
 *        per connection 
 * 
 ******************************************************************************/
static Cppi_HostDesc* sauGenToAirPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen, uint8_t* payload)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
 	Cppi_HostDesc *hd;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    uint32_t      psInfo[10];
    int           psInfoIndex = 0; 
    uint16_t      pktLen;
    int           index;
    sauAc_t*      pAcHdr;
    Sa_SWInfo_t*  pSwInfo;
    
    if (connIndex >= SA_TEST_MAX_CONNS)
    {
        salld_sim_print("sauGenToAirPkt: connIndex(%d) exceeds its range\n", connIndex);
        return NULL;
    }
    
    memset(psInfo, 0, sizeof(psInfo));
    
    pktLen = payloadLen + pConnHdl->hdrLen;

    hd = testCommonGetBuffer(tf, (Int)(pktLen));
	if (hd == NULL)  {
        salld_sim_print("sauGenToAirPkt: no link buffer available\n");
        return NULL;
	}
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    pPktDesc->size = pktLen;
    pPktDesc->segments[0] = (void *)(hd->buffPtr); 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    
    /* Construct the original packet and update the first 32-bit payload with connection ID */
    memcpy(pPktDesc->segments[0], pConnHdl->hdr,  pConnHdl->hdrLen);
    memcpy(((uint8_t *)pPktDesc->segments[0]) + pConnHdl->hdrLen, payload,  payloadLen);
    pktWrite32bits_m((uint8_t *)pPktDesc->segments[0], pConnHdl->hdrLen, (uint32_t)connIndex);
    
    /* We only support one-layer at this moment */
    if ( pConnHdl->numHandles != 1)
    {
        salld_sim_print("sauGenToAirPkt: too many handles %d \n", pConnHdl->numHandles);
        return NULL;
    }
    
    /* Perform protocol-specific operation in reverse order */
    for (index = 0; index < pConnHdl->numHandles; index++)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];
        
        switch (pHdrHdl->hdrType)
        {
            case SAU_PROTO_AC:
                sauProcToAirPktAc(pConnHdl, pHdrHdl, &pktInfo, pPktDesc, payloadLen);
                pAcHdr = &pHdrHdl->u.ac;
                pSwInfo = &pAcHdr->pChan->regSwInfo;
  	            Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pSwInfo->swInfo);
            break;
        
            default:
                /* Should never enter here */
                salld_sim_print("sauGenToAirPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n", 
                                 index, pHdrHdl->hdrType);
                return(NULL);                  
        }
    }
    
    /* Prepare to send pkt to PA/SA */
  	Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
    
    /* Prepare for tx commands in the PS Info */
    /*
     * The following commands may be passed in the PS Info section
     * - Payload Info (short format) for Air Ciphering Packet
     */
    #ifndef SAU_TEST_LONG_INFO 
    /* Short Info */
    psInfo[0] = PASAHO_SINFO_FORMAT_CMD(pPktDesc->payloadOffset, pPktDesc->payloadLen);
    if (((pAcHdr->pduType == sa_AcPduType_LTE_CP) || (pAcHdr->pduType == sa_AcPduType_LTE)) && 
        (pAcHdr->ctrlBitMap & SA_PROTO_AC_CTRL_FLAG_IV_TEST)                                &&
        pAcHdr->useIV)
    {
        Sa_psInfo_t *psInfo2 = (Sa_psInfo_t *)&psInfo[0];
        psInfo[1] =  pAcHdr->iv[0] = pAcHdr->count32_2;
        sa_PSINFO_SET_IV(psInfo2, pAcHdr->iv, pAcHdr->ivSize);
        psInfoIndex = 2 + pAcHdr->ivSize/4;
    }
    else
    {
        psInfo[1] = pAcHdr->count32;
        psInfoIndex = 2; 
    }
    
    /* Potential private data at PS Info */
    psInfo[psInfoIndex]  = 0xbabeface;
    psInfo[psInfoIndex+1] = 0x87654321;    
    psInfoIndex += 2;
    #else
    /* Long info */
    psInfo[0] = (24 << 24) | pPktDesc->payloadOffset;
    psInfo[1] = (pPktDesc->payloadOffset + pPktDesc->payloadLen) << 16;
    psInfoIndex = 6;
    #endif                                                      
    
    /* Attach the command in PS data */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, psInfoIndex * 4);
    
    #if SA_GEN_TEST_VECTOR_RX
    
    utilOutputPkt(&tFramework, pSwInfo->swInfo, psInfoIndex*4, psInfo,
                   pPktDesc->size, (uint8_t*) pPktDesc->segments[0], TRUE);
    #endif
    
    
    return (hd);
}

/*******************************************************************************
 *  Function: Prepare and send From-Air packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function process the rx packet from the network through
 *      PA/SA subsystem via the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Perform all the protocol-specific operations specified in the connection
 *        Prepare and call Sa_chanSendData()
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the PA/SA commands 
 *        per connection 
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauGenFromAirPkt(int connIndex, Cppi_HostDesc* hd)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    sauAc_t*  pAcHdr;
    uint32_t      psInfo[10];
    int           psInfoIndex = 0; 
    #if SA_GEN_TEST_VECTOR_TX
    static Bool   first[10] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
    #endif   
    
    pHdrHdl = pConnHdl->pHdrHdl[0];
    pAcHdr = &pHdrHdl->u.ac;
    
    /* We only support one-layer at this moment */
    if ( pConnHdl->numHandles != 1)
    {
        salld_sim_print("sauProcFromAirPkt: too many handles %d \n", pConnHdl->numHandles);
        return;
    }
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    pPktDesc->size = hd->buffLen;
    
    /* Extract Count-C from the To-Air packet if required */
    if ( pAcHdr->ctrlBitMap & SA_PROTO_AC_CTRL_FLAG_COUNTC_INSERT)
    {
        /* overwrite count32 only if IV is not used */
        if(!pAcHdr->useIV)
            pAcHdr->count32 = pktRead32bits_m((uint8_t *)hd->buffPtr, 0);
        pPktDesc->size -= 4;
        hd->buffPtr += 4;
    }
    
    pPktDesc->segments[0] = (void *)hd->buffPtr; 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    pktInfo.validBitMap = 0;
    
    pPktDesc->payloadOffset = 0;
    pPktDesc->payloadLen = pPktDesc->size;
        
    /* Call SA LLD API to perform Protocol Specific Operation */
    Sa_chanSendData(pAcHdr->pChan->salldInst, &pktInfo, FALSE);
    
    /* Prepare to send pkt to PA/SA */
  	Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
    
    /* Prepare for tx commands in the PS Info */
    /*
     * The following commands may be passed in the PS Info section
     * - Payload Info (short format) for Air Ciphering Packet
     */
    psInfo[0] = PASAHO_SINFO_FORMAT_CMD(pPktDesc->payloadOffset, pPktDesc->payloadLen);
    if (((pAcHdr->pduType == sa_AcPduType_LTE_CP) || (pAcHdr->pduType == sa_AcPduType_LTE)) && 
        (pAcHdr->ctrlBitMap & SA_PROTO_AC_CTRL_FLAG_IV_TEST)                                &&
        pAcHdr->useIV)
    {
        Sa_psInfo_t *psInfo2 = (Sa_psInfo_t *)&psInfo[0];
        psInfo[1] = pAcHdr->count32_2++;
        sa_PSINFO_SET_IV(psInfo2, pAcHdr->iv, pAcHdr->ivSize);
        psInfoIndex = 2 + pAcHdr->ivSize/4;
        
        pAcHdr->useIV = FALSE;
    }
    else
    {
        psInfo[1] = pAcHdr->count32++;
        psInfoIndex = 2;
        if ((pAcHdr->pduType == sa_AcPduType_LTE_CP) || (pAcHdr->pduType == sa_AcPduType_LTE))pAcHdr->useIV = TRUE;
    }
    
    
    /* Potential private data at PS Info */
    psInfo[psInfoIndex]  = 0xdeadbeef;
    psInfo[psInfoIndex+1] = 0x12345678;    
    psInfoIndex += 2;
    
    /* Attach the command in PS data */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, psInfoIndex * 4);
    
    Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
    
    #if SA_GEN_TEST_VECTOR_TX
    if(first[connIndex])
    {
        utilOutputScFromSwInfo(&tFramework, pktInfo.swInfo.swInfo);
        first[connIndex] = FALSE;
    }
    
    utilOutputPkt(&tFramework, pktInfo.swInfo.swInfo, psInfoIndex*4, psInfo,
                   pPktDesc->size, (uint8_t*) pPktDesc->segments[0], TRUE);
    #endif
    
}

/*******************************************************************************
 *  Function: Generate Air Ciphering Key Stream packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function prepares the Data Mode packet ready to be send to PA/SA
 *      subsystem through the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Prepare and call salldSendData()
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the SA commands 
 *        per connection 
 * 
 ******************************************************************************/
static Cppi_HostDesc* sauGenKeyStreamPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl = pConnHdl->pHdrHdl[0];
 	Cppi_HostDesc *hd;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    uint32_t      psInfo[2];
    int           psInfoIndex = 0; 
    uint16_t      pktLen;
    int           index;
    sauAc_t*      pAcHdr = &pHdrHdl->u.ac;
    Sa_SWInfo_t*  pSwInfo;
    
    if (connIndex >= SA_TEST_MAX_CONNS)
    {
        salld_sim_print("sauGenKeyStreamPkt: connIndex(%d) exceeds its range\n", connIndex);
        return NULL;
    }
    
    /* We only support one-layer at this moment */
    if ( pConnHdl->numHandles != 1)
    {
        salld_sim_print("sauGenKeyStreamPkt: too many handles %d \n", pConnHdl->numHandles);
        return NULL;
    }
    
    /* 3GPP Air ciphering only */
    if (pHdrHdl->hdrType != SAU_PROTO_AC)
    {
        salld_sim_print("sauGenKeyStreamPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n", 
                         0, pHdrHdl->hdrType);
        return NULL;
    }
    
    /* Overwrite payloadlen as key stream size based on the air ciphering mode */
    if (pAcHdr->cipherMode == sa_CipherMode_GSM_A53)
    {
        payloadLen = SAU_KGCORE_KS_SIZE_GSM_A53;
    }
    else if (pAcHdr->cipherMode == sa_CipherMode_ECSD_A53)
    {
        payloadLen = SAU_KGCORE_KS_SIZE_ECSD_A53;
    }
    
    pktLen = payloadLen + pConnHdl->hdrLen;

    hd = testCommonGetBuffer(tf, (Int)(pktLen));
	if (hd == NULL)  {
        salld_sim_print("sauGenKeyStreamPkt: no link buffer available\n");
        return NULL;
	}
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    pPktDesc->size = pktLen;
    pPktDesc->segments[0] = (void *)(hd->buffPtr); 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    
    /* Construct the original packet and update the first 32-bit payload with connection ID */
    memcpy(pPktDesc->segments[0], pConnHdl->hdr,  pConnHdl->hdrLen);
    memset(((uint8_t *)pPktDesc->segments[0]) + pConnHdl->hdrLen, 0,  payloadLen);
    
    /* Perform protocol-specific operation in reverse order */
    for (index = 0; index < pConnHdl->numHandles; index++)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];
        
        switch (pHdrHdl->hdrType)
        {
            case SAU_PROTO_AC:
                sauProcKeyStreamPktAc(pConnHdl, pHdrHdl, &pktInfo, pPktDesc);
                pAcHdr = &pHdrHdl->u.ac;
                pSwInfo = &pAcHdr->pChan->regSwInfo;
  	            Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pSwInfo->swInfo);
            break;
        
            default:
                /* Should never enter here */
                salld_sim_print("sauGenToAirPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n", 
                                 index, pHdrHdl->hdrType);
                return(NULL);                  
        }
    }
    
    /* Prepare to send pkt to PA/SA */
  	Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
    
    /* Prepare for tx commands in the PS Info */
    /*
     * The following commands may be passed in the PS Info section
     * - Payload Info (short format) for Air Ciphering Packet
     */
    psInfo[psInfoIndex] = PASAHO_SINFO_FORMAT_CMD(pPktDesc->payloadOffset, pPktDesc->payloadLen);
    psInfoIndex += 2;                                                        
    
    /* Attach the command in PS data */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)psInfo, psInfoIndex * 4);
    
    #if SA_GEN_TEST_VECTOR_RX
    
    utilOutputPkt(&tFramework, pSwInfo->swInfo, psInfoIndex*4, psInfo,
                   pPktDesc->size, (uint8_t*) pPktDesc->segments[0], TRUE);
    #endif
    
    
    return (hd);
}


/*******************************************************************************
 *  Function: Process Rx packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function process the rx packet from the network through
 *      PA/SA subsystem via the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Perform all the protocol-specific operations specified in the connection
 *        UDP: Update UDP length and calculate UDP checksum
 *        SRTP: Update sequence number, prepare and call salldSendData()
 *        IP: Update IP length, calculate IP checksum
 *        IPSEC: Prepare and call salldSendData()    
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the PA/SA commands 
 *        per connection 
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauVerifyFromAirPkt(int connIndex, Cppi_HostDesc* hd)
{  
    
    int pktErrCode, dataLen, i;
    uint32_t rcvConnId;
    uint8_t  *pData;
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    
    pktErrCode = Cppi_getDescError(Cppi_DescType_HOST, (Cppi_Desc *)hd);
    
    if(pktErrCode)
        salld_sim_print("Conn %d: From-Air Pkt with error code %d from SA\n", connIndex, pktErrCode); 
    
    /* Verify payload */
    #if !SA_GEN_TEST_VECTOR_RX    
    testDispPkts(hd);
    #endif
    
    salld_sim_disp_control(TRUE);    
    
    /* Verify the decoded packet */
    rcvConnId = pktRead32bits_m((uint8_t *)hd->buffPtr, pConnHdl->hdrLen);
    
    if (rcvConnId != connIndex)
    {
        salld_sim_print("Receive Pkt of connection %d from PA/SA with incorrect connId = %d\n", connIndex, rcvConnId); 
    }
    
    pData = (uint8_t *)hd->buffPtr + pConnHdl->hdrLen + 4;
    
    dataLen = hd->buffLen - pConnHdl->hdrLen - 8 - 1;
    
    for ( i = 0; i < dataLen; i++)
    {
        if ((uint8_t)(pData[i + 1] - pData[i]) != 1)
        {
            salld_sim_print("Receive Pkt of connection %d from PA/SA with incorrect data at location %d, data = 0x%02x\n", 
                            connIndex, pConnHdl->hdrLen + 4 + i, pData[i]); 
            break;
        } 
    }
    
    salld_sim_disp_control(FALSE);    
    
    /* free buffer */
	testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
}

/*******************************************************************************
 *  Function: Display Air Ciphering Key Streams
 *******************************************************************************
 *  DESCRIPTION:  
 *
 ******************************************************************************/
void sauDispAcKeyStream(int connIndex, Cppi_HostDesc* hd)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl = pConnHdl->pHdrHdl[0];
    sauAc_t*  pAcHdr = &pHdrHdl->u.ac;
    uint8_t *pData = (uint8_t *)(hd->buffPtr);
    
    uint16_t keyBlockSize = 0;
    
    /* Overwrite payloadlen as key stream size based on the air ciphering mode */
    if (pAcHdr->cipherMode == sa_CipherMode_GSM_A53)
    {
        keyBlockSize = SAU_KGCORE_KB_SIZE_GSM_A53;
    }
    else if (pAcHdr->cipherMode == sa_CipherMode_ECSD_A53)
    {
        keyBlockSize = SAU_KGCORE_KB_SIZE_ECSD_A53;
    }
    
    
    utilDispKeyStream(&pData[pAcHdr->ivSize], hd->buffLen - pAcHdr->ivSize, keyBlockSize); 
    
    /* free buffer */
	testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
}


/*******************************************************************************
 *  Function: Connection Verification
 *******************************************************************************
 *  DESCRIPTION:  This function perform multi-connection multi-packet verification
 *      as specified below:
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
#ifdef _TMS320C6X
extern cregister volatile unsigned int TSCL;
#else
#define TSCL 0
#endif
#define SAUTEST_AC_PROFILE_SIZE 100
unsigned int acToAirProcCycle[SAUTEST_AC_PROFILE_SIZE];
unsigned int acFromAirProcCycle[SAUTEST_AC_PROFILE_SIZE];
unsigned int acToAirProcCycle2[SAUTEST_AC_PROFILE_SIZE];
unsigned int acFromAirProcCycle2[SAUTEST_AC_PROFILE_SIZE];
unsigned int acToAirAvgProcCycle;
unsigned int acFromAirAvgProcCycle;
Cppi_HostDesc *acHd, *acHd2;


void sauConnAcPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                      uint16_t initLen, uint16_t step, sauPayloadType_e payloadType)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        /* 
         * We should perform encryption/authentication verification for
         * a pair of connection
         *
         */
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
#ifdef _TMS320C6X
            if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
              acToAirProcCycle[pktNum] = TSCL;
            }
#else
            if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
               __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(acToAirProcCycle[pktNum]));
            }
#endif

            hd = sauGenToAirPkt(tf, connIndex, payloadLen, sap_pkt_test_buf);
            acHd = hd;

            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            //mdebugHaltSaPdsp (1);
#ifdef NSS_LITE
            Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //salld_sim_halt();

#ifdef _TMS320C6X
            if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
              acToAirProcCycle[pktNum] = TSCL - acToAirProcCycle[pktNum];
            }
#else
            if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                uint32_t temp;
                __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(temp));
                acToAirProcCycle[pktNum] = temp - acToAirProcCycle[pktNum];
            }
#endif
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_TX */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (10000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX) > 0)
			        break;
	        }
	
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_TX);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX)) 
            {
#ifdef _TMS320C6X
               if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                  acToAirProcCycle2[pktNum] = TSCL;
               }
#else
               if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(acToAirProcCycle2[pktNum]));
               }
#endif
                Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX);

#ifdef _TMS320C6X
               if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                  acToAirProcCycle2[pktNum] = TSCL - acToAirProcCycle2[pktNum];
               }
#else
               if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
            	   uint32_t temp;
                   __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(temp));
                   acToAirProcCycle2[pktNum] = temp - acToAirProcCycle2[pktNum];
               }
#endif
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_TX) & ~15));
                salld_sim_print("Conn %d: Receive To-Air Pkt %d from SA\n", connIndex, pktNum); 
                #if !SA_GEN_TEST_VECTOR_TX
                testDispPkts(hd);
                #endif

#ifdef _TMS320C6X
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                  acFromAirProcCycle[pktNum] = TSCL;
                }
#else
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(acFromAirProcCycle[pktNum]));
                }
#endif

                sauGenFromAirPkt(connIndex, hd);
                acHd2 = hd;
                /* Push From-Air packet to the SA receive Queue for decryption */
                //if(pktNum == 1)
                //    mdebugHaltSaPdsp (1);
#ifdef NSS_LITE
                Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	            Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                //if(pktNum == 1)
                //   salld_sim_halt();

#ifdef _TMS320C6X
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                  acFromAirProcCycle[pktNum] = TSCL - acFromAirProcCycle[pktNum];
                }
#else
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                  uint32_t temp;
                  __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(temp));
                  acFromAirProcCycle[pktNum] = temp - acFromAirProcCycle[pktNum];
                }
#endif

            }

  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV */
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }

	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }

	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV))
            {
#ifdef _TMS320C6X
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                    acFromAirProcCycle2[pktNum] = TSCL;
                }
#else
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(acFromAirProcCycle2[pktNum]));
                }
#endif
                Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV);
#ifdef _TMS320C6X
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                    acFromAirProcCycle2[pktNum] = TSCL - acFromAirProcCycle2[pktNum];
                }
#else
                if (pktNum < SAUTEST_AC_PROFILE_SIZE) {
                	uint32_t temp;
                    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(temp));
                    acFromAirProcCycle2[pktNum] = temp - acFromAirProcCycle2[pktNum];
                }
#endif
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                salld_sim_print("Conn %d: Receive From-Air Pkt %d from SA\n", connIndex, pktNum); 
                sauVerifyFromAirPkt(connIndex, hd);
            }
        } /* Connection Loop */
    } /* Packet Loop */
                                                       
    /* Calculate the average cycle */
    acToAirAvgProcCycle = 0;
    acFromAirAvgProcCycle = 0;
    for (i = 0; i < numPkts; i++)
    {
        acToAirAvgProcCycle += acToAirProcCycle[i];
        acFromAirAvgProcCycle += acFromAirProcCycle[i];
    }
    
    acToAirAvgProcCycle /= numPkts;
    acFromAirAvgProcCycle /= numPkts;
    
}

void sauConnAcKeyGenTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                         uint16_t initLen, uint16_t step)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        
        /* 
         * We should perform Key stream generation for each connection
         *
         */
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
            hd = sauGenKeyStreamPkt(tf, connIndex, payloadLen);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            //mdebugHaltSaPdsp (1);
#ifdef NSS_LITE
            Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //salld_sim_halt();
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_TX */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX) > 0)
			        break;
	        }
	
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_TX);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_TX) & ~15));
                salld_sim_print("Conn %d: Receive KeyGen Pkt %d from SA\n", connIndex, pktNum); 
                #if !SA_GEN_TEST_VECTOR_TX
                testDispPkts(hd);
                #endif
                
                sauDispAcKeyStream(connIndex, hd);
                
            }
            
        } /* Connection Loop */
    } /* Packet Loop */
    
}

#endif

uint32_t dmProcCycle[1000];
uint32_t dmProcCycleMeasCnt = 0;

uint32_t dmProcCycle2[1000];
uint32_t dmProcCycleMeasCnt2 = 0;

/*******************************************************************************
 *  Function: Prepare and format Data Mode packet 
 *******************************************************************************
 *  DESCRIPTION:  This function format data mode packet 
 *
 ******************************************************************************/
static void sauFormatDmPkt(sauConnHandle_t *pConnHdl, sauHdrHandle_t* pDmHdl, 
                          Sa_PktInfo_t* pPktInfo, Sa_PktDesc_t* pPktDesc, uint16_t payloadLen)
{
    Sa_PayloadInfo_t *pPayloadInfo = &pPktInfo->payloadInfo;
    Sa_CmdLabelInfo_t*  pCmdLbInfo = &pPktInfo->cmdlb;
    sauDataMode_t*  pDmHdr = &pDmHdl->u.data;
    
    
    /* Command Label Info */
    pCmdLbInfo->cmdLbBuf = testCmdLbBuf;
    
    /* fill the payload Info */
    pPktInfo->validBitMap = sa_PKT_INFO_VALID_PAYLOAD_INFO |
                            sa_PKT_INFO_VALID_CMDLB_INFO;
    pPayloadInfo->encOffset = pConnHdl->hdrLen;
    pPayloadInfo->authOffset = 0;
    pPayloadInfo->encSize  = payloadLen;
    pPayloadInfo->authSize = pPktDesc->size;
    pPayloadInfo->encIV    = pDmHdr->encIV;
    pPayloadInfo->authIV   = pDmHdr->authIV;
    pPayloadInfo->aad      = pDmHdr->aad;
    
    /* Update the IV and AAD with sequence Number */
    pktWrite32bits_m((uint8_t *)pDmHdr->encIV, 0, pDmHdr->seqNum);
    pktWrite32bits_m((uint8_t *)pDmHdr->authIV, 0, pDmHdr->seqNum);
    // if (pPayloadInfo->aad)
    if (pDmHdr->aadSize)
    {
        /* We only support AAD length of 8 and 12 */
        pktWrite32bits_m((uint8_t *)pDmHdr->aad, pDmHdr->aadSize - 4, pDmHdr->seqNum);
    }
    
    pDmHdr->seqNum++;
    
    pPktDesc->payloadOffset = 0;
    pPktDesc->payloadLen = pPktDesc->size;
    
    if (!pDmHdr->fCmdLbAvail)
    {
        /* first packet */
        pCmdLbInfo->cmdLbUpdateInfo = &pDmHdr->cmdLbUpdate;
        Sa_chanSendData(pDmHdr->pChan->salldInst, pPktInfo, FALSE);
        
        /* Copy and update command lable */
        pDmHdr->cmdLbSize = pCmdLbInfo->cmdLbSize;
        memcpy((void *)pDmHdr->cmdLbBuf, testCmdLbBuf, pDmHdr->cmdLbSize);
        
        {
            Sa_CmdLbUpdateInfo_t* updateInfo = &pDmHdr->cmdLbUpdate;
            uint32_t *cmdLb = pDmHdr->cmdLbBuf;
            
            sa_mDmResetCmdLb(updateInfo, cmdLb);
        
        }  
        
        pDmHdr->fCmdLbAvail = TRUE;      
            
    }
    else
    {
        pCmdLbInfo->cmdLbUpdateInfo = NULL;

        if(dmProcCycleMeasCnt < 1000)
#ifdef _TMS320C6X
            dmProcCycle[dmProcCycleMeasCnt] = CSL_chipReadTSCL();
#else
            __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(dmProcCycle[dmProcCycleMeasCnt]));
#endif
        Sa_chanSendData(pDmHdr->pChan->salldInst, pPktInfo, FALSE);
        if (dmProcCycleMeasCnt < 1000)
        {
#ifdef _TMS320C6X
            dmProcCycle[dmProcCycleMeasCnt] = TSCL - dmProcCycle[dmProcCycleMeasCnt];
#else
            uint32_t temp;
            __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(temp));
            dmProcCycle[dmProcCycleMeasCnt] = temp - dmProcCycle[dmProcCycleMeasCnt];
#endif
            dmProcCycleMeasCnt++;
        }

        /* Command update measurement */
        if(dmProcCycleMeasCnt2 < 1000)
#ifdef _TMS320C6X
            dmProcCycle2[dmProcCycleMeasCnt2] = TSCL;
#else
            __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(dmProcCycle2[dmProcCycleMeasCnt2]));
#endif

        {
            Sa_CmdLbUpdateInfo_t* updateInfo = &pDmHdr->cmdLbUpdate;
            uint32_t *cmdLb = (uint32_t *)testCmdLbBuf2;
            
            memcpy(cmdLb, pDmHdr->cmdLbBuf, pDmHdr->cmdLbSize);
            
            if (updateInfo->subMode == sa_DM_CCM_GEN)
            {
              sa_mDmUpdateCmdLb_ccm_gen(pConnHdl->hdrLen, payloadLen, pDmHdr->encIV, pDmHdr->aadSize,  pDmHdr->aad, updateInfo, cmdLb);
            }
            else 
            {
              sa_mDmUpdateCmdLb(pConnHdl->hdrLen, payloadLen, pDmHdr->encIV, 0, pPktDesc->size, pDmHdr->authIV,
                                pDmHdr->aadSize,  pDmHdr->aad,  (uint8_t *) pPktDesc->segments[0], updateInfo, cmdLb);
            }
            
            if (dmProcCycleMeasCnt2 < 1000)
            {
#ifdef _TMS320C6X
                dmProcCycle2[dmProcCycleMeasCnt2] = TSCL - dmProcCycle2[dmProcCycleMeasCnt2];
#else
                uint32_t temp;
                __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(temp));
                dmProcCycle2[dmProcCycleMeasCnt2] = temp - dmProcCycle2[dmProcCycleMeasCnt2];
#endif
                dmProcCycleMeasCnt2++;
            }


            if (memcmp(testCmdLbBuf, cmdLb, pDmHdr->cmdLbSize))
            {
                salld_sim_print("sauFormatDmPkt: command label mismatches!\n");
                salld_sim_halt ();

            }

        }



    }
}

/*******************************************************************************
 *  Function: Generate DM packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function prepares the Data Mode packet ready to be send to PA/SA
 *      subsystem through the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Prepare and call salldSendData()
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the SA commands
 *        per connection
 *
 ******************************************************************************/
static Cppi_HostDesc* sauGenDmPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen, uint8_t* payload)
{
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
 	Cppi_HostDesc *hd;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    Sa_CmdLabelInfo_t*  pCmdLbInfo = &pktInfo.cmdlb;
    uint16_t      pktLen;
    int           index;
    #if SA_GEN_TEST_VECTOR_TX
    static Bool   first[10] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
    #endif
    
    if (connIndex >= SA_TEST_MAX_CONNS)
    {
        salld_sim_print("sauGenToAirPkt: connIndex(%d) exceeds its range\n", connIndex);
        return NULL;
    }
    
    pktLen = payloadLen + pConnHdl->hdrLen;

    hd = testCommonGetBuffer(tf, (Int)(pktLen + SA_PKT_MARGIN));
	if (hd == NULL)  {
        salld_sim_print("sauGenTxPkt: no link buffer available\n");
        return NULL;
	}
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    pPktDesc->size = pktLen;
    pPktDesc->segments[0] = (void *)(hd->buffPtr); 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    
    /* Construct the original packet and update the first 32-bit payload with connection ID */
    memcpy(pPktDesc->segments[0], pConnHdl->hdr,  pConnHdl->hdrLen);
    memcpy(((uint8_t *)pPktDesc->segments[0]) + pConnHdl->hdrLen, payload,  payloadLen);
    pktWrite32bits_m((uint8_t *)pPktDesc->segments[0], pConnHdl->hdrLen, (uint32_t)connIndex);
    
    /* We only support one-layer at this moment */
    if ( pConnHdl->numHandles != 1)
    {
        salld_sim_print("sauGenToAirPkt: to many handles %d \n", pConnHdl->numHandles);
        return NULL;
    }
    
    /* Perform protocol-specific operation in reverse order */
    for (index = 0; index < pConnHdl->numHandles; index++)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];
        
        switch (pHdrHdl->hdrType)
        {
            case SAU_PROTO_DATAMODE:
                sauFormatDmPkt(pConnHdl, pHdrHdl, &pktInfo, pPktDesc, payloadLen);
                sa_SWINFO_UPDATE_DEST_INFO(pktInfo.swInfo.swInfo, DEST_QUEUE_PKT_TX, 0);
  	            Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
            break;
        
            default:
                /* Should never enter here */
                salld_sim_print("sauGenToAirPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n", 
                                 index, pHdrHdl->hdrType);
                return(NULL);                  
        }
    }
    
    /* Prepare to send pkt to PA/SA */
  	Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
    /* Attach the command label in PS data */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, pCmdLbInfo->cmdLbBuf, pCmdLbInfo->cmdLbSize);
    
    #if SA_GEN_TEST_VECTOR_TX
    if(first[connIndex])
    {
        utilOutputScFromSwInfo(&tFramework, pktInfo.swInfo.swInfo);
        first[connIndex] = FALSE;
    }
    
    utilOutputPkt(&tFramework, pktInfo.swInfo.swInfo, pCmdLbInfo->cmdLbSize, (uint32_t *)pCmdLbInfo->cmdLbBuf,
                   pPktDesc->size, (uint8_t*) pPktDesc->segments[0], TRUE);
    #endif
    
    
    return (hd);
}

/*******************************************************************************
 *  Function: Prepare and send From-Air packet per connection
 *******************************************************************************
 *  DESCRIPTION:  This function process the rx packet from the network through
 *      PA/SA subsystem via the CPPI queue. It performs the following actions
 *      - Allocate the buffer descriptor based on payalod and header size
 *      - Initialize the SA pktInfo structure
 *      - Perform all the protocol-specific operations specified in the connection
 *        UDP: Update UDP length and calculate UDP checksum
 *        SRTP: Update sequence number, prepare and call salldSendData()
 *        IP: Update IP length, calculate IP checksum
 *        IPSEC: Prepare and call salldSendData()    
 *      - Fill the swInfo words
 *      - Prepare and fill the PS Info words with a set of the PA/SA commands 
 *        per connection 
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
void sauRelayDmPkt(int connIndex, Cppi_HostDesc* hd)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauConnHandle_t *pPrevConnHdl = &testConnHandles[connIndex - 1];
    sauHdrHandle_t *pHdrHdl, *pPrevHdrHdl;
    Sa_PktInfo_t  pktInfo;
    Sa_PayloadInfo_t *pPayloadInfo = &pktInfo.payloadInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    Sa_CmdLabelInfo_t*  pCmdLbInfo = &pktInfo.cmdlb;
    #if SA_GEN_TEST_VECTOR_RX
    static Bool   first[10] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
    #endif
    
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
    pPktDesc->size = hd->buffLen;
    pPktDesc->segments[0] = (void *)hd->buffPtr; 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->origBufferLen;
    
    /* Call Post-Processing function to remove potential padding */
    pPrevHdrHdl = pPrevConnHdl->pHdrHdl[0];
    /*
     * Note: need to record the payload info at the output of Sa_chanSendData in general use case
     */
    pktInfo.validBitMap = sa_PKT_INFO_VALID_PAYLOAD_INFO;
    pPayloadInfo->encOffset = pPrevConnHdl->hdrLen;
    pPayloadInfo->authOffset = 0;
    pPayloadInfo->encSize  = pPktDesc->size - pPayloadInfo->encOffset;
    pPayloadInfo->authSize = pPktDesc->size;
    
    Sa_chanReceiveData(pPrevHdrHdl->u.data.pChan->salldInst, &pktInfo);
    
    /* Clear PS Info section */
    Cppi_setPSLen(Cppi_DescType_HOST, (Cppi_Desc *)hd,  0);
    
    /* We only support one-layer at this moment */
    if ( pConnHdl->numHandles != 1)
    {
        salld_sim_print("sauRelayDmPkt: too many handles %d \n", pConnHdl->numHandles);
        return;
    }
    
    pHdrHdl = pConnHdl->pHdrHdl[0];
    
    sauFormatDmPkt(pConnHdl, pHdrHdl, &pktInfo, pPktDesc, pPktDesc->size - pConnHdl->hdrLen);
  	Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)pktInfo.swInfo.swInfo);
    
    /* Prepare to send pkt to PA/SA */
  	Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (Ptr)pPktDesc->segments[0], pPktDesc->size);
  	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, pPktDesc->size);
    
    /* Attach the command label in PS data */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)hd, pCmdLbInfo->cmdLbBuf, pCmdLbInfo->cmdLbSize);
    
    #if SA_GEN_TEST_VECTOR_RX
    if(first[connIndex])
    {
        utilOutputScFromSwInfo(&tFramework, pktInfo.swInfo.swInfo);
        first[connIndex] = FALSE;
    }
    
    utilOutputPkt(&tFramework, pktInfo.swInfo.swInfo, pCmdLbInfo->cmdLbSize, (uint32_t *)pCmdLbInfo->cmdLbBuf,
                   pPktDesc->size, (uint8_t*) pPktDesc->segments[0], TRUE);
    #endif
    
    
}

/*******************************************************************************
 *  Function: Verify the decrypted payload data in Data Mode
 *******************************************************************************
 *  DESCRIPTION:  This function verifies the decrypted payload data in Data Mode
 *
 ******************************************************************************/
void sauVerifyDmPkt(int connIndex, Cppi_HostDesc* hd)
{  
    
    int dataLen, i;
    uint32_t rcvConnId;
    uint8_t  *pData;
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    
    /* Verify payload */
    #if !SA_GEN_TEST_VECTOR_TX
    testDispPkts(hd);
    #endif
    
    salld_sim_disp_control(TRUE);    
    
    /* Verify the decoded packet */
    rcvConnId = pktRead32bits_m((uint8_t *)hd->buffPtr, pConnHdl->hdrLen);
    
    if (rcvConnId != connIndex)
    {
        salld_sim_print("Receive Pkt of connection %d from PA/SA with incorrect connId = %d\n", connIndex, rcvConnId); 
    }
    
    pData = (uint8_t *)hd->buffPtr + pConnHdl->hdrLen + 4;
    
    /* For CBC mode, the last partial block will be incorrect */
    dataLen = hd->buffLen - pConnHdl->hdrLen - 4 - 1 - 15;
    
    for ( i = 0; i < dataLen; i++)
    {
        if ((uint8_t)(pData[i + 1] - pData[i]) != 1)
        {
            salld_sim_print("Receive Pkt of connection %d from PA/SA with incorrect data at location %d, data = 0x%02x\n", 
                            connIndex, pConnHdl->hdrLen + 4 + i, pData[i]); 
            break;
        } 
    }
    
    salld_sim_disp_control(FALSE);    
    
    /* free buffer */
	testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
}

Cppi_HostDesc *dmHd;

/*******************************************************************************
 *  Function: Connection Verification
 *******************************************************************************
 *  DESCRIPTION:  This function performs multi-connection multi-packet verification
 *      as specified below:
 *
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each pair of connections
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - Invoke salld_sendData() API for Data Mode channel
 *       - Prepare and send packet to SA for Encryption (and Authentication)
 *       - Receive encrypted packet from SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_sendData() API for the corresponding Data Mode channel
 *       - Forward the packet to SA for Decryption (and Authentication) 
 *       - Preceive the decrypted packet from the SA sub-system
 *       - Perform payload verification              
 *   - End of the test loop
 *
 ******************************************************************************/
void sauConnDmPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,
                      uint16_t initLen, uint16_t step, sauPayloadType_e payloadType)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t  payloadLen;
	uint32_t  infoLen;
    uint32_t  *psInfo;
    uint32_t  authTag[8];

    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        /* 
         * We should perform encryption/authentication verification for
         * a pair of connection
         *
         */
        for (connIndex = 0; connIndex < numConnHandles; connIndex += 2)
        {
            hd = sauGenDmPkt(tf, connIndex, payloadLen, sap_pkt_test_buf);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
        
            //mdebugHaltSaPdsp (1);
#ifdef NSS_LITE
            Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSa2Index], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSa2Index], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
            //salld_sim_halt();
  	
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_TX */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX) > 0)
			        break;
	        }
	
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_TX);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_TX) & ~15));
                /* Extract PS Info if exists */
                Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&psInfo, &infoLen);
                memcpy(authTag, psInfo, infoLen);
                
                salld_sim_print("Conn %d: Receive Data Mode Pkt %d from SA\n", connIndex, pktNum); 
                #if !SA_GEN_TEST_VECTOR_RX
                testDispPkts(hd);
                #endif
                
                {
                    Qmss_Queue  queue = Cppi_getReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd);
                    
                    if ((queue.qNum != tFramework.QLinkedBuf1) &&
                        (queue.qNum != tFramework.QLinkedBuf2) &&
                        (queue.qNum != tFramework.QLinkedBuf3) &&
                        (queue.qNum != tFramework.QLinkedBuf4))
                    {
                        salld_sim_print("sauConnDmPktTest: Conn %d Pkt %d Rx HD return queue %d corrupted\n",
                                         connIndex, pktNum, queue.qNum);  
                    }
                }
                
                sauRelayDmPkt(connIndex+1, hd);                
                /* Push From-Air packet to the SA receive Queue for decryption */
                //if(pktNum == 1)
                //    mdebugHaltSaPdsp (1);
                dmHd = hd;
#ifdef NSS_LITE
                Qmss_queuePushDescSize (tf->QPaTx[nssGblCfgParams.layout.qSa2Index], (Ptr)utilgAddr((uint32_t)hd), TF_SIZE_DESC);
#else
  	            Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSa2Index], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
#endif
                //if(pktNum == 1)
                //   salld_sim_halt();
            }
            
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_TX */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX) > 0)
			        break;
	        }
	
	        if (i >= 200)  {
		        salld_sim_halt();
		        SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_TX);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	        }
	
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_TX) & ~15));
                salld_sim_print("Conn %d: Receive Data Mode Pkt %d from SA\n", connIndex+1, pktNum); 
                /* Extract PS Info if exists */
                Cppi_getPSData (Cppi_DescType_HOST, Cppi_PSLoc_PS_IN_DESC, (Cppi_Desc *)hd, (uint8_t **)&psInfo, &infoLen);
                if (infoLen)
                {
                    if(memcmp(psInfo, authTag, infoLen))
                    {
                        SALog("Conn %d(Pkt %d): authentication tag does not match!\n", connIndex+1, pktNum); 
                        System_flush();
                    }
                }
                
                sauVerifyDmPkt(connIndex, hd);
            }
            
        } /* Connection Loop */
    } /* Packet Loop */
}

#ifndef NSS_LITE
/*******************************************************************************
 *  Function: Replay Verification
 *******************************************************************************
 *  DESCRIPTION:  This function perform multi-connection multi-packet verification
 *      as specified below:
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each connection
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - IPV4 checksum
 *         - IP length and IP psudo checksum computation
 *         - UDP length and checksum computation
 *         - Invoke salld_sendData() API for IPSEC channel
 *       - Prepare and send packet to SA for Tx operation
 *       - Receive packet from PA/SA sub-system
 *       - Forward the tx packet to PA for receive processing
 *       - Preceive the rx packet from the PA/SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_receiveData() API for IPSEC channel
 *       - Perform payload verification              
 *   - End of the test loop
 * 
 * Note: We do not support non-security connection at this moment
 *
 ******************************************************************************/
#ifdef _TMS320C6X
#pragma DATA_SECTION(sauTestPkt, ".scBufs")
#pragma DATA_ALIGN(sauTestPkt, 128)
//sauTestPkt_t  sauTestPkt[1100];
sauTestPkt_t  sauTestPkt[200];
#else
sauTestPkt_t  sauTestPkt[200] __attribute__ ((aligned (128)));
#endif
void sauConnReplayTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts,  uint16_t* numTxPkts,
                       uint16_t initLen, uint16_t step, sauPayloadType_e payloadType, uint32_t** ppSeqNum, int fSoftware)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen, index;
    uint32_t *connSeqNum;
	Qmss_Queue 	q;

    /* Packet Tests */
    for (connIndex = 0; connIndex < numConnHandles; connIndex++)
    {
        payloadLen = initLen + connIndex * step;
        connSeqNum = ppSeqNum[connIndex];
    
        /*
         * Generate test packets with sequence number  
         */
        for (pktNum = 0; pktNum < numTxPkts[connIndex]; pktNum++)
        {
            sauGenPayload(payloadType, 0, payloadLen, sap_pkt_test_buf);
        
            hd = sauGenTxPkt(tf, connIndex, payloadLen, sap_pkt_test_buf, FALSE);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }
            
            if (!fSoftware)
            {
                //if(pktNum == 4)
                //    mdebugHaltSaPdsp (1);
  	            Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qSaIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
                //if(pktNum == 4)
                //   salld_sim_halt();
  	
  	            /* The packet should loop back into queue DEST_QUEUE_PKT_TX */	
	            for (i = 0; i < 200; i++)  {
		            utilCycleDelay (1000);
		            if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX) > 0)
			            break;
	            }
	
	            if (i >= 200)  {
		            salld_sim_halt();
		            SALog ("%s (%s:%d): Did not find SA reply in queue %d \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_TX);
 		            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
	            }
            
	            /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
                while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_TX)) 
                {
	                hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_TX) & ~15));
                
                    /* Record the tx packet and free the buffer */
                    sauTestPkt[pktNum].pktSize = hd->buffLen;
                    memcpy(sauTestPkt[pktNum].pkt, (void *)hd->buffPtr, hd->buffLen);
                    salld_sim_print("Conn %d: Receive TxPkt %d from PA/SA\n", connIndex, pktNum); 
	                testCommonRecycleLBDesc (&tFramework, hd); /* No Error Check here */
                }
            }
            else
            {
                /* Record the tx packet and free the buffer */
                sauTestPkt[pktNum].pktSize = hd->buffLen;
                memcpy(sauTestPkt[pktNum].pkt, (void *)hd->buffPtr, hd->buffLen);
	            testCommonRecycleHostLBDesc (&tFramework, hd); /* No Error Check here */
            }

        } /* Packet Loop (Tx) */

#ifdef _TMS320C6X
        SYS_CACHE_WB (sauTestPkt, sizeof(sauTestPkt), CACHE_FENCE_WAIT);
#endif

        /*
         * Send test packets with specified sequence number 
         */
        for (pktNum = 0; pktNum < numPkts;  pktNum++)
        {
            /* Prepare and send the recorded packets */
		    hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (tf->QfreeDesc)) & ~15);
		
		    if (hd == NULL)  {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
		    }
	
		    /* Setup the return for the descriptor */
  		    q.qMgr = 0;
  		    q.qNum = tf->QfreeDesc;
  		    Cppi_setReturnQueue (Cppi_DescType_HOST, (Cppi_Desc *)hd, q);
            
            index = connSeqNum[pktNum]-1;
            
  	        /* Make sure there is no control info.  */
  	        Cppi_setPSLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, 0);
  	
  	        /* Attach the data and set the length */
            Cppi_setOriginalBufInfo(Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)(sauTestPkt[index].pkt)), sauTestPkt[index].pktSize);
  		    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)hd, (uint8_t *)utilgAddr((uint32_t)sauTestPkt[index].pkt), (uint32_t)sauTestPkt[index].pktSize);
  		    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)hd, sauTestPkt[index].pktSize);
            
            /* Push Tx packet to the PA receive Queue */
            //if(pktNum == 13)
            //   mdebugHaltSaPdsp (0);
            //mdebugHaltPdsp (0);
  	        Qmss_queuePush (tf->QPaTx[nssGblCfgParams.layout.qPaInputIndex], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
            //if(pktNum == 13)
            //    salld_sim_halt();
            
  	        /* We may not receive all the packets due to replay protection */	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV) > 0)
			        break;
	        }
	
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV) & ~15));
                salld_sim_print("Conn %d: Receive RxPkt %d from PA/SA\n", connIndex, pktNum); 
                sauProcRxPkt(connIndex, hd);
            }
        } 
        
    } /* Connection Loop */
}
#endif
