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

int             numHdrHandles, numUdpHdrHandles, numConnHandles, numSrtpHandles, numSrtcpHandles, numIpsecHandles;
sauHdrHandle_t  testUdpHdrHandles[SA_TEST_MAX_UDPHDR_HANDLES];
sauConnHandle_t testConnHandles[SA_TEST_MAX_CONNS];
sauSrtp_t       testSrtpHandles[SA_TEST_MAX_SRTP_CHANS];
sauSrtcp_t      testSrtcpHandles[SA_TEST_MAX_SRTCP_CHANS];

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

/* Converts base address to offset for the shared memory for the test framework */
uint32_t sauConvBase2Offset(void* base_addr)
{
   return ((uint32_t) base_addr - (uint32_t) shared_ObjsBase);
}
 
/* Converts offset to base address for the shared memory for the test framework */
uint32_t sauConvOffset2Base(uint32_t offset)
{
   return ((uint32_t) offset + (uint32_t) shared_ObjsBase);
}

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
    t2Handles_t *t2Hdl;         /* Pointer to the T2 handle */
    
    pHdrHandle = TEST_ALLOC_HDR_HANDLE();   
    if (pHdrHandle == NULL)
    {
        salld_sim_print("sauCreateMac: Hdr Handle is not available!\n");
        return (NULL);
    }    
    
    /* Fill the header information */
    pMacHdr = &pHdrHandle->u.mac;
    
    //pMacHdr->t2Hdl = TEST_ALLOC_L2_HANDLE();
    fw_shmAllocL2Handles(&t2Hdl);
    pMacHdr->t2HdlOffset = sauConvBase2Offset(t2Hdl);
    
    if (t2Hdl == NULL)
    {
        /* Free the protocol header handle */
        numHdrHandles--;
        salld_sim_print("sauCreateMac: L2 Handle is not available!\n");
        return (NULL);
    }
    
    memcpy(pMacHdr->dest, destAddr, 6);
    memcpy(pMacHdr->src, srcAddr, 6);
    pMacHdr->ethType = ethType;
    
    pHdrHandle->upHdlOffset = 0xfaced;   /* There is no upper layer for MAC */
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
    t2Handles_t *t3Hdl;         /* Pointer to the T3 handle */
    
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
    
    //pIpHdr->t3Hdl = TEST_ALLOC_L3_HANDLE();
    fw_shmAllocL3Handles(&t3Hdl);
    pIpHdr->t3HdlOffset = sauConvBase2Offset(t3Hdl);
    
    if (t3Hdl == NULL)
    {
        /* Free the protocol header handle */
        numHdrHandles--;
        salld_sim_print("sauCreateIp: L3 Handle is not available!\n");
        return (NULL);
    }
    
    pHdrHandle->hdrType = SAU_PROTO_IP;
    pHdrHandle->upHdlOffset = sauConvBase2Offset(pHdl);
    
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
    sauHdrHandle_t *pHdrHandle = TEST_ALLOC_UDPHDR_HANDLE();
    sauUdp_t       *pUdpHdr;

//    APP_csEnter	();
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
    
    
    pHdrHandle->upHdlOffset = sauConvBase2Offset(pHdl);
    pHdrHandle->hdrLen = 8;
    pHdrHandle->hdrType = SAU_PROTO_UDP;
    pktWrite16bits_m((uint8_t *)pHdrHandle->hdr, 0, srcPort);
    pktWrite16bits_m((uint8_t *)pHdrHandle->hdr, 2, destPort);
    pktWrite16bits_m((uint8_t *)pHdrHandle->hdr, 6, 0);         /* checksum */

 //   APP_csExit	();
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
Bool sauCreateIpsec (sauHdrHandle_t *pIpHdl, uint8_t type, salldTestConfig_t* pTestCfg)
{
    salldOpenConfig_t      openCfg = pTestCfg->openCfg;
    sauIp_t *pIpHdr = &pIpHdl->u.ip;
    uint16_t retCode;
    salldSimChannel_t* pChan;
    
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
        pIpsecHdl->ahIvSize = (pTestCfg->authMode == sa_AuthMode_GMAC)?8:0;
    
    }	
    pIpsecHdl->macSize = salld_get_macSize(FALSE);
    pIpsecHdl->spi = salld_get_spi(FALSE);
    
    /* Prepare and create the SALLD channel */
    pChan = salldcfg_chan_init(pTestCfg);

    pIpsecHdl->pChanOffset = sauConvBase2Offset(pChan);
    
    if (pChan == NULL)
    {
        /* Free the protocol header handle */
        numIpsecHandles--;
        salld_sim_print("sauCreateIpsec: salldcfg_chan_init() failed!\n");
        return (FALSE);
    }
    
    if ((retCode = salldSim_init_chn(pChan)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateIpsec: salldSim_init_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    if ((retCode = salldSim_open_chn(pChan, &openCfg)) != salld_SIM_ERR_NONE)
    {
        salld_sim_print("sauCreateIpsec: salldSim_open_chn() failed with retCode = 0x%0x!\n", retCode);
        return (FALSE);
    }
    
    pIpHdr->ipsecHandleOffset = sauConvBase2Offset(pIpsecHdl);
    
    return (TRUE);
}

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
                                         
static paRouteInfo_t   sauSaRoute =    { pa_DEST_SASS,      		/* Dest */
 								         0,					        /* Flow ID */
 								         TF_SA_QUEUE_0, 	        /* queue */ /* 646 */
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
                                         
static paRouteInfo_t   sauPaRouteTxPkt =  { pa_DEST_HOST,     		/* Dest */
 								            0,					    /* Flow ID */
 								            TF_PA_QUEUE_INPUT,      /* queue */ /*640 */
 								            -1,					    /* Multi route */
 								            0x55550000,	            /* sw Info 0 */
 									        0,  				    /* sw Info 1 */
                                            0,                      /* customType : not used */         
                                            0,                      /* customIndex: not used */     
                                            0,                      /* pkyType: for SRIO only */    
                                            NULL};                  /* No commands */
static paCmdReply_t    sauCmdReply = {  pa_DEST_HOST,			    /* Dest */
 							            0,						    /* Reply ID (returned in swinfo0) */
 							            0,						    /* Queue */
 							            0 };						/* Flow ID */
                                         

void setsauHostRouteQ(void)
{
	sauHostRoute.queue = (DEST_QUEUE_PKT_RECV + coreNum);	

}
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
    t2Handles_t *t2Hdl = (t2Handles_t*) sauConvOffset2Base(pMacHdl->u.mac.t2HdlOffset);         /* Pointer to the T2 handle */
    
    /* Verify whether we have already registered the MAC entry */
    if(t2Hdl->state == TF_L2_HANDLE_ACTIVE)
        return;
        
    memset(&ethInfo, 0, sizeof(paEthInfo_t));
    memcpy(ethInfo.dst, pMac->dest, 6);
    ethInfo.ethertype = pMac->ethType;  
    
 	if (!testCommonAddMac (tf, pat, (paEthInfo_t *)&ethInfo, &sauContiRoute, &sauFailRoute,
 	                       &t2Hdl->paHandle, tf->QLinkedBuf1, &sauCmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
 	t2Hdl->state = TF_L2_HANDLE_ACTIVE;
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
	sauHdrHandle_t*  upHdl = (sauHdrHandle_t*) sauConvOffset2Base(pIpHdl->upHdlOffset);
    t2Handles_t *t2Hdl = (t2Handles_t*) sauConvOffset2Base(upHdl->u.mac.t2HdlOffset);         /* Pointer to the T2 handle */
    t2Handles_t *t3Hdl = (t2Handles_t *) sauConvOffset2Base(pIp->t3HdlOffset);         /* Pointer to the T3 handle */
    t2Handles_t *t3Hdl2 = (t2Handles_t *) sauConvOffset2Base(pIp->t3Hdl2Offset);        /* Pointer to the second T3 handle for NAT-T entry only */
    sauIpsec_t* ipsecHandle = NULL;    /* Pointer to the IPSEC Handle */
    salldSimChannel_t  *pChan = NULL;
    
    /* Get the ipsecHandle */
    if (pIp->ipsecHandleOffset != 0) /* assuming ipsecHandle offset is not the first element in the shmem */
    {
    	ipsecHandle =  (sauIpsec_t *) sauConvOffset2Base(pIp->ipsecHandleOffset);
        pChan = (salldSimChannel_t *) sauConvOffset2Base(ipsecHandle->pChanOffset);
    }

    /* Verify whether we have already registered the IP entry */
    if(t3Hdl->state == TF_L2_HANDLE_ACTIVE)
        return;
        
    pRouteInfo2 = NULL;    
    memset(&ipInfo, 0, sizeof(paIpInfo_t));
    memcpy(&ipInfo.dst, pIp->dest, 16);
    ipInfo.ipType = (pIp->type == SAU_IP_TYPE_IP4)?pa_IPV4:pa_IPV6;  
    
    if (ipsecHandle)
    {
        Sa_SWInfo_t*  pSwInfo = &pChan->regSwInfo;
    
        /* IPSEC channel */
        if (ipsecHandle->type == SAU_IPSEC_TYPE_ESP_NAT_T)
        {
            memset(&ipInfo2, 0, sizeof(paIpInfo_t));
            
            fw_shmAllocL3Handles(&t3Hdl2);
            pIp->t3Hdl2Offset = sauConvBase2Offset(t3Hdl2);
    
            if (t3Hdl2 == NULL)
            {
                /* Free the protocol header handle */
                salld_sim_print("sauRegisterIp: L3 Handle2 is not available!\n");
            }

            ipInfo2.ipType = (pIp->type == SAU_IP_TYPE_IP4)?pa_IPV4:pa_IPV6;  
            ipInfo2.spi = ipsecHandle->spi;
        
            pRouteInfo = &sauContiRouteLUT2;
            pRouteInfo2 = &sauSaRoute;
            pRouteInfo2->swInfo0 = pSwInfo->swInfo[0];
            pRouteInfo2->swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);
        }
        else
        {
            ipInfo.spi = ipsecHandle->spi;
            ipInfo.proto = (ipsecHandle->type == SAU_IPSEC_TYPE_ESP)?0x32:0x33;
        
            pRouteInfo = &sauSaRoute;
            pRouteInfo->swInfo0 = pSwInfo->swInfo[0];
            pRouteInfo->swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);
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
    
    
    if (upHdl->hdrType == SAU_PROTO_MAC)
    {
        paHandle = t2Hdl->paHandle;
    }
    else
    {
        if(upHdl->u.ip.t3Hdl2Offset == 0) /* assuming the offset is not the first element, hence '0' is valid - which means there is no t3Hdl2 */
        {
        	t2Handles_t *upt3Hdl = (t2Handles_t *) sauConvOffset2Base(upHdl->u.ip.t3HdlOffset);         /* Pointer to the T3 handle */
        	paHandle = upt3Hdl->paHandle;
        }
        else
        {
        	t2Handles_t *upt3Hdl2 = (t2Handles_t *) sauConvOffset2Base(upHdl->u.ip.t3Hdl2Offset);         /* Pointer to the T3 handle */
            paHandle = upt3Hdl2->paHandle;
        }
    }

 	if (!testCommonAddIp  (tf, pat, (paIpInfo_t *)&ipInfo, pRouteInfo, &sauFailRoute,
 	                       paHandle, &t3Hdl->paHandle, tf->QLinkedBuf2,
 	                       &sauCmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }                    
      
 	t3Hdl->state = TF_L2_HANDLE_ACTIVE;
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
	sauHdrHandle_t*  upHdl = (sauHdrHandle_t*)sauConvOffset2Base(pUdpHdl->upHdlOffset);
    t2Handles_t *t3Hdl = (t2Handles_t*) sauConvOffset2Base( upHdl->u.ip.t3HdlOffset);     /* Pointer to the T3 handle */
    t2Handles_t *t3Hdl2 = (t2Handles_t*) sauConvOffset2Base( upHdl->u.ip.t3Hdl2Offset);   /* Pointer to the second T3 handle for NAT-T entry only */
    paHandleL2L3_t  paHandle = t3Hdl->paHandle;

    /* Verify whether we have already registered the UDP entry */
    if(pUdp->t4Hdl->state == TF_L2_HANDLE_ACTIVE)
        return;

    if (upHdl->u.ip.t3Hdl2Offset) /* assuming offset is not the first element in the shared memory */
    {
        paHandle = t3Hdl2->paHandle;
    }
        
    if (pUdp->srtpHandle)
    {
        Sa_SWInfo_t*  pSwInfo = &pUdp->srtpHandle->pChan->regSwInfo;
            
        /* SRTP channel */
        pRouteInfo = &sauSaRoute;
        pRouteInfo->swInfo0 = pSwInfo->swInfo[0];
        pRouteInfo->swInfo1 = utilgAddr((uint32_t)pSwInfo->swInfo[1]);        
    }
    else 
    {
        /* It is the last entry */
        pRouteInfo = &sauHostRoute;
        
        /* Use swInfo0 as channel identifier */
    }
 	if (!testCommonAddUdp  (tf, pat, pUdp->destPort, pRouteInfo,
 	                       paHandle, pUdp->t4Hdl->paHandle, tf->QLinkedBuf1, 
 	                       &sauCmdReply))
    {
 		saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }                       
    
 	pUdp->t4Hdl->state = TF_L2_HANDLE_ACTIVE;
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
	sauHdrHandle_t*  upHdl;	
    t2Handles_t *t3Hdl;         /* Pointer to the T3 handle */
    
    if (pConnHdl->srtpIndex == SA_CONN_INDEX_NONE)
    {
        salld_sim_print("sauReregegisterUdp: SRTP channel does not exist in the connection!\n");
        return;
    }
    
    pUdpHdl =  pConnHdl->pHdrHdl[pConnHdl->srtpIndex];
    pUdp = &pUdpHdl->u.udp;
    upHdl   = (sauHdrHandle_t*)sauConvOffset2Base(pUdpHdl->upHdlOffset);
    t3Hdl = (t2Handles_t*) sauConvOffset2Base(upHdl->u.ip.t3HdlOffset);
    paHandle = t3Hdl->paHandle;
    
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
    
 	if (!testCommonAddUdp  (&tFramework, tFramework.pTest, pUdp->destPort, pRouteInfo,
 	                       paHandle, pUdp->t4Hdl->paHandle, tFramework.QLinkedBuf1, 
 	                       &sauCmdReply))
    {
        salld_sim_print("sauReregegisterUdp: Add UDP failed!\n");
        return;
    }                       
    
}


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
    sauIpsec_t* ipsecHandle;    /* Pointer to the IPSEC Handle */
    salldSimChannel_t* pChan;
    
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

	/* Update runtime values */
	sauContiRoute.flowId = tf->tfFlowNum;
	sauContiRouteLUT2.flowId = tf->tfFlowNum;
	sauFailRoute.flowId = tf->tfFlowNum;
	sauSaRoute.flowId = tf->tfFlowNum;
	sauHostRoute.flowId = tf->tfFlowNum;
	sauPaRouteTxPkt.flowId = tf->tfFlowNum;
	sauCmdReply.flowId = tf->tfFlowNum;
    
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

        if (pHdrHdl->upHdlOffset == 0xfaced)
        	break;
    } while (pHdrHdl = (sauHdrHandle_t*)sauConvOffset2Base(pHdrHdl->upHdlOffset));
    
    /*
     * Perform the following processes in reverse order
     * - Extract Header information
     * - Extract protocol offset information
     * - Register the receive protocol information to PA if not done before
     */
 	sauCmdReply.replyId = TF_CMD_SWINFO0_ADD_ID + 0;  /* TF_CMD_SWINFO0_ADD_ID identifies command, 16 LS bits identify the local handle number */
 	sauCmdReply.queue = tf->QGen[Q_CMD_REPLY + coreNum];
     
    ipIndex = 0; 
    for (index = pConnHdl->numHandles - 1; index >= 0; index--)
    {
        pHdrHdl = pConnHdl->pHdrHdl[index];
        
        switch (pHdrHdl->hdrType)
        {
            case SAU_PROTO_MAC:
                System_printf ("     Registering MAC (Core:%d) \n", coreNum);						
                sauRegisterMac(tf, pat, pHdrHdl);
            break;
            
            case SAU_PROTO_IP:
                pHdrHdl->u.ip.index = ipIndex;
                pConnHdl->ipOffset[ipIndex++] = pConnHdl->hdrLen;
                if (pHdrHdl->u.ip.ipsecHandleOffset != 0) /* assuming ipsecHandle offset is not the first element in the shmem */
                	ipsecHandle =  (sauIpsec_t *) sauConvOffset2Base(pHdrHdl->u.ip.ipsecHandleOffset);
                else
                	ipsecHandle = NULL;
                if (ipsecHandle)
                {
                    /* Note: only one IPSEC channel is supported per connection */
                    pConnHdl->ipsecIndex = index;  
                    pConnHdl->ipsecOffset = pConnHdl->hdrLen + pHdrHdl->hdrLen; 
                    pChan = (salldSimChannel_t*) sauConvOffset2Base(ipsecHandle->pChanOffset);
                    pChan->pConnHdl = (void *)pConnHdl;
                    
                }    
                System_printf ("     Registering Ip (Core:%d) \n", coreNum);						
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
                System_printf ("     Registering UDP (Core:%d) \n", coreNum);										
                sauRegisterUdp(tf, pat, pHdrHdl);
            break;
            
            case SAU_PROTO_AC:
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
    sauIpsec_t* pIpsec = (sauIpsec_t*)sauConvOffset2Base(pIp->ipsecHandleOffset);
    uint8_t       ipOffset = pConnHdl->ipOffset[pIp->index];
    uint16_t      ipPayloadLen;
    salldSimChannel_t* pChan = (salldSimChannel_t*) sauConvOffset2Base(pIpsec->pChanOffset);
    
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

    if (pIp->ipsecHandleOffset) /* assuming ipsecOffset is not at the beginning */
    {
        /* IPSEC operation */
        pPktDesc->payloadOffset = ipOffset;
        pPktDesc->payloadLen = ipPayloadLen;
        if (pIpsec->type == SAU_IPSEC_TYPE_ESP_NAT_T)
        {
            pPktInfo->validBitMap  =  sa_PKT_INFO_VALID_IPSEC_NAT_T_INFO;
            pPktInfo->natTInfo.dstPort = 4500;
            pPktInfo->natTInfo.srcPort = 4500;            
        }
        else
        {
            pPktInfo->validBitMap = 0;
        }
        Sa_chanSendData(pChan->salldInst, pPktInfo, FALSE);
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
	sauHdrHandle_t* upHdl = (sauHdrHandle_t*) sauConvOffset2Base(pUdpHdl->upHdlOffset);
    sauIp_t*   pIp = &upHdl->u.ip;
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
    uint32_t        psInfo[16];
    int           psInfoIndex = 0; 
    uint16_t        pktLen, rtpPayloadSize;
    uint32_t        origHdrLoc;
    uint16_t        offsetAdjust = 0;
    int           index;
    Bool          fSrtp = FALSE;
    Bool          fIpsec = FALSE;
	paReturn_t    paret;
    uint16_t        cmdSize;
    
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
            break;
            
            case SAU_PROTO_IP:
                if (sauProcTxPktIp(pConnHdl, pHdrHdl, &pktInfo, pPktDesc))
                {
                    fIpsec = TRUE; 
                    pIpsecHdl = (sauIpsec_t *) sauConvOffset2Base(pHdrHdl->u.ip.ipsecHandleOffset);
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
            pRouteInfo = &sauPaRouteTxPkt;
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
            pRouteInfo = &sauPaRouteTxPkt;
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
                if (pIpsecHdl = (sauIpsec_t*)sauConvOffset2Base(pHdrHdl->u.ip.ipsecHandleOffset))
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
    
    if (pConnHdl->ipsecIndex != SA_CONN_INDEX_NONE)
    {
        sauIpsec_t* pIpsec;
        salldSimChannel_t* pChan;

        pHdrHdl = pConnHdl->pHdrHdl[pConnHdl->ipsecIndex]; 
        pIpsec = (sauIpsec_t*) sauConvOffset2Base(pHdrHdl->u.ip.ipsecHandleOffset);
        
        /* Perform IPSEC related operation */
        /* TBE: need to get the extra IP offset */
        pPktDesc->payloadOffset = PASAHO_LINFO_READ_L3_OFFSET(pLongInfo);   /* Pointer to IP header */
        pPktDesc->payloadLen = (pPktDesc->size - pPktDesc->payloadOffset);
        
        /* Call SA LLD API to perform Protocol Specific Operation */
        pktInfo.pktErrCode = sa_PKT_ERR_OK;
        pChan = (salldSimChannel_t*) sauConvOffset2Base(pIpsec->pChanOffset);
        errCode = Sa_chanReceiveData(pChan->salldInst, &pktInfo);
        
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

static uint8_t sap_pkt_test_buf[0x1000];
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
                    uint16_t initLen, uint16_t step, sauPayloadType_e payloadType)
{

 	Cppi_HostDesc *hd;
 	Int i, pktNum, connIndex;
    uint16_t payloadLen;
	saTestStatus_t saTestStat = SA_TEST_PASSED;
    
    /* Packet Tests */
    for (pktNum = 0; pktNum < numPkts; pktNum++)
    {
        payloadLen = initLen + pktNum * step;
        sauGenPayload(payloadType, 0,  payloadLen, sap_pkt_test_buf);
        
        for (connIndex = 0; connIndex < numConnHandles; connIndex++)
        {
            hd = sauGenTxPkt(tf, connIndex, payloadLen, sap_pkt_test_buf, FALSE);
            
            if (hd == NULL)
            {
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                saTestStat = SA_TEST_FAILED;
            }
        
  	        Qmss_queuePush (tf->QPaTx[TF_PA_TX_QUEUE_INDEX_SA0], (Ptr)utilgAddr((uint32_t)hd), hd->buffLen, TF_SIZE_DESC, Qmss_Location_TAIL);
  	
  	        /* The packet should loop back into queue DEST_QUEUE_PKT_RECV + coreNum*/	
	        for (i = 0; i < 200; i++)  {
		        utilCycleDelay (1000);
		        if (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV + coreNum) > 0)
			        break;
	        }
            
	        if (i >= 200)  {
		        salld_sim_halt();
		        System_printf ("%s (%s:%d): Did not find SA reply in queue %d (CORE ID:%d) \n", pat->name, __FILE__, __LINE__, DEST_QUEUE_PKT_RECV + coreNum, coreNum);
 		        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                saTestStat = SA_TEST_FAILED;				
	        }
            
	        /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            while (Qmss_getQueueEntryCount (DEST_QUEUE_PKT_RECV + coreNum)) 
            {
	            hd = (Cppi_HostDesc *)(((uint32_t)Qmss_queuePop (DEST_QUEUE_PKT_RECV + coreNum) & ~15));
                salld_sim_print("Conn %d: Receive RxPkt %d from PA/SA (CORE ID:%d) \n", connIndex, pktNum, coreNum);                 
                sauProcRxPkt(connIndex, hd);
            }
        } /* Connection Loop */
    } /* Packet Loop */

    pat->testStatus = saTestStat;
}


