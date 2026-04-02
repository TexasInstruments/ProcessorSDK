/*
 *
 * Copyright (C) 2010-2020 Texas Instruments Incorporated - http://www.ti.com/
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
#include "ti/osal/osal.h"
#if defined (NSS_LITE2)
#include "ti/csl/arch/csl_arch.h"
#include <inttypes.h>
#endif

/*
 * Declare protocol and connection related handles
 */
sauHdrHandle_t  testHdrHandles[SA_TEST_MAX_HDR_HANDLES];
sauConnHandle_t testConnHandles[SA_TEST_MAX_CONNS] __attribute__ ((aligned (16)));
int             numHdrHandles, numConnHandles;

static uint8_t  testCmdLbBuf[sa_MAX_CMDLB_SIZE];
static uint8_t  testCmdLbBuf2[sa_MAX_CMDLB_SIZE];
static uint8_t  sap_pkt_test_buf[TF_DESC_BUFSIZE];

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
                              
static uint8_t testEncIV[16] = {0x00, 0x00, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                              0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};    
                             
static uint8_t testAuthIV[16] = {0x00, 0x00, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
                               0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
                               
static uint8_t testAad[12] = {0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0, 0, 0, 0};  

#define SAU_TXPKT_TO_HOST   1                                                               

/* Internal local functions for throughput measurement */
#if defined (BUILD_MCU)
static void sau_cycle_cnt_init(void)
{
    uint32_t val;
    CSL_armR5PmuCfg(0, 0, 1);
    /* Clear the overflow */
    val = CSL_armR5PmuReadCntrOverflowStatus();
    val &= 0x80000007;
    CSL_armR5PmuClearCntrOverflowStatus(val);
    CSL_armR5PmuCfgCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM, CSL_ARM_R5_PMU_EVENT_TYPE_CYCLE_CNT);
    CSL_armR5PmuEnableAllCntrs(0);
    CSL_armR5PmuResetCycleCnt();
    /* Set PMCR C-bit */
    CSL_armR5PmuResetCntrs();
    CSL_armR5PmuEnableAllCntrs(1);
}
static uint64_t sau_read_cycle_cnt(void)
{
    uint32_t val;
    uint64_t cnt;
    val = CSL_armR5PmuReadCntr(CSL_ARM_R5_PMU_CYCLE_COUNTER_NUM);
    cnt = (uint64_t) val;
    return (cnt);
}
#elif defined (BUILD_MPU)
static void sau_cycle_cnt_init(void)
{
    CSL_initGTC();
}
static uint64_t sau_read_cycle_cnt(void)
{
    uint64_t cnt;
    cnt = CSL_getGTCcount();
    return (cnt);
}
#else
static void sau_cycle_cnt_init(void)
{
  /* stub function */
}
static uint64_t sau_read_cycle_cnt(void)
{
  /* stub function */
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
    commCfg.destInfo[0].flowID =  tFramework.tfFlowNum;
    commCfg.destInfo[0].queueID = tFramework.rxComplRingNum; /* Route to Host, using default flow */
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

    if(pCfg->ctrlBitMap & SAU_DM_CFG_PROMOTE)
    {
        dmCfg.ctrlBitMap |= sa_DM_CONFIG_PROMOTE_CHANNEL;
    }
    if(pCfg->ctrlBitMap & SAU_DM_CFG_DEMOTE)
    {
        dmCfg.ctrlBitMap |= sa_DM_CONFIG_DEMOTE_CHANNEL;
    }
    if(pCfg->ctrlBitMap & SAU_DM_CFG_NONSECURE_CRYPTO)
    {
        dmCfg.ctrlBitMap |= sa_DM_CONFIG_USE_SECURE_CTX_FOR_NON_SECURE_CHANNEL;
    }
#if defined(NSS_LITE2)
    /* Wild card match for the incoming packets's for PrivId
     * User privillage for the channel
     */
     dmCfg.secure            = 0;
     dmCfg.priv              = 0x1;
     dmCfg.privId            = 0xC3;
#endif

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

uint64_t dmProcCycle[1000];
uint32_t dmProcCycleMeasCnt = 0;

uint64_t dmProcCycle2[1000];
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
        {
            dmProcCycle[dmProcCycleMeasCnt] = TimerP_getTimeInUsecs();
        }
        Sa_chanSendData(pDmHdr->pChan->salldInst, pPktInfo, FALSE);
        if (dmProcCycleMeasCnt < 1000)
        {
            dmProcCycle[dmProcCycleMeasCnt] = TimerP_getTimeInUsecs() - dmProcCycle[dmProcCycleMeasCnt];
            dmProcCycleMeasCnt++;
        }

        /* Command update measurement */
        if(dmProcCycleMeasCnt2 < 1000)
        {
            dmProcCycle2[dmProcCycleMeasCnt2] = TimerP_getTimeInUsecs();
        }

        {
            Sa_CmdLbUpdateInfo_t* updateInfo = &pDmHdr->cmdLbUpdate;
            uint32_t *cmdLb  = (uint32_t *)testCmdLbBuf2;
            uint32_t *cmdLb2 = cmdLb;
            
            memcpy(cmdLb, pDmHdr->cmdLbBuf, pDmHdr->cmdLbSize);

            if (updateInfo->subMode == sa_DM_CCM_GEN)
            {
              sa_mDmUpdateCmdLb_ccm_gen(pConnHdl->hdrLen, payloadLen, pDmHdr->encIV, pDmHdr->aadSize,  pDmHdr->aad, updateInfo, cmdLb2);
            }
            else 
            {
              sa_mDmUpdateCmdLb(pConnHdl->hdrLen, payloadLen, pDmHdr->encIV, 0, pPktDesc->size, pDmHdr->authIV,
                                pDmHdr->aadSize,  pDmHdr->aad,  (uint8_t *) pPktDesc->segments[0], updateInfo, cmdLb2);
            }
            
            if (dmProcCycleMeasCnt2 < 1000)
            {
                dmProcCycle2[dmProcCycleMeasCnt2] = TimerP_getTimeInUsecs() - dmProcCycle2[dmProcCycleMeasCnt2];
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
static CSL_UdmapCppi5HMPD* sauGenDmPkt(tFramework_t *tf, int connIndex, uint16_t payloadLen, uint8_t* payload)
{
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauHdrHandle_t* pHdrHdl;
    CSL_UdmapCppi5HMPD *hd;
    Sa_PktInfo_t  pktInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    Sa_CmdLabelInfo_t*  pCmdLbInfo = &pktInfo.cmdlb;
    uint16_t      pktLen;
    int           index;
    uintptr_t     cmdLblAddr;
    uint32_t      pktId, flowId;
    uint32_t      pTsInfo = 0, pSwInfo0, pSwInfo1, pSwInfo2;

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
    pPktDesc->segments[0] =  (void *) (uintptr_t)(hd->bufPtr); 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->orgBufLen;
    
    /* Construct the original packet and update the first 32-bit payload with connection ID */
    memcpy((uint8_t *)pPktDesc->segments[0], pConnHdl->hdr,  pConnHdl->hdrLen);
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

                /* Update EPIB information */
                CSL_udmapCppi5SetEpiDataPresent((void*) hd, EPI_PRESENT);
                CSL_udmapCppi5RdEpiData((void *) hd, &pTsInfo, &pSwInfo0, &pSwInfo1, &pSwInfo2);
                /* UPdate swInfo */
                pSwInfo0 = pktInfo.swInfo.swInfo[0];
                pSwInfo1 = pktInfo.swInfo.swInfo[1];
                pSwInfo2 = pktInfo.swInfo.swInfo[2];
                CSL_udmapCppi5WrEpiData((void *) hd, pTsInfo, pSwInfo0, pSwInfo1, pSwInfo2);
                /* overwrite the flow id */
                CSL_udmapCppi5GetIds((void*) hd, &pktId, &flowId);
                /* Clear PS flags and any error */
                hd->pktInfo1 = 0;
#if defined (ACP_COHERENCY)
                hd->nextDescPtr = CSL_pktdmaMakeAselAddr((uint64_t) 0ULL, \
                                CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC);
#else
                /* Clear any next descriptor pointer */
                hd->nextDescPtr = 0;
#endif
                flowId = tFramework.tfFlowNum;
                CSL_udmapCppi5SetIds((void*) hd, SA2UL_TEST_CPPI5_DESCRIPTOR_TYPE_HOST, pktId, flowId);
            break;
        
            default:
                /* Should never enter here */
                salld_sim_print("sauGenToAirPkt: Hdr entry (%d) with invalid Hdr type (%d) in the chain \n", 
                                 index, pHdrHdl->hdrType);
                return(NULL);                  
        }
    }
    
    /* Prepare to send pkt to PA/SA */
    CSL_udmapCppi5SetBufferAddr((CSL_UdmapCppi5HMPD*) hd, (uint64_t)Osal_VirtToPhys(pPktDesc->segments[0]));

#if defined (ACP_COHERENCY)
    /* update the buffer for asel bits*/
    hd->bufPtr      = CSL_pktdmaMakeAselAddr ((uint64_t) hd->bufPtr, \
                             CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC);
#endif


#if defined(SOC_AM64X)
    /* For AM64X (LCDMA) bufferLen is set to max value during allocation */
#else
    CSL_udmapCppi5SetBufferLen ((CSL_UdmapCppi5HMPD*) hd, pPktDesc->size);
#endif
    CSL_udmapCppi5SetPktLen((void *) hd, SA2UL_TEST_CPPI5_DESCRIPTOR_TYPE_HOST, pPktDesc->size);

    /* Attach the command label in PS data */
    CSL_udmapCppi5SetPsDataLoc((void *) hd, CSL_UDMAP_CPPI5_PD_DESCINFO_PSINFO_VAL_IN_DESC);
    cmdLblAddr = CSL_udmapCppi5GetPsDataAddr((void*) hd, 0, EPI_PRESENT);
    /* First word of PS INFO */
    memcpy ((void *) cmdLblAddr, pCmdLbInfo->cmdLbBuf, pCmdLbInfo->cmdLbSize);
    CSL_udmapCppi5SetPsDataLen((void *) hd, pCmdLbInfo->cmdLbSize);

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
void sauRelayDmPkt(int connIndex, CSL_UdmapCppi5HMPD* hd)
{  
    sauConnHandle_t *pConnHdl = &testConnHandles[connIndex];
    sauConnHandle_t *pPrevConnHdl = &testConnHandles[connIndex - 1];
    sauHdrHandle_t *pHdrHdl, *pPrevHdrHdl;
    Sa_PktInfo_t  pktInfo;
    Sa_PayloadInfo_t *pPayloadInfo = &pktInfo.payloadInfo;
    Sa_PktDesc_t* pPktDesc = &pktInfo.pktDesc;
    Sa_CmdLabelInfo_t*  pCmdLbInfo = &pktInfo.cmdlb;
    uintptr_t      cmdLblAddr;
    #if SA_GEN_TEST_VECTOR_RX
    static Bool   first[10] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
    #endif
    uint32_t      pktId, flowId;
    uint32_t      pTsInfo, pSwInfo0, pSwInfo1, pSwInfo2;
    /* Initialize the packet descriptor */
    salld_sim_init_pktdesc(pPktDesc, 1);
#if defined (SOC_AM64X)
    pPktDesc->size= CSL_udmapCppi5GetPktLen((void*) hd);
#else
    pPktDesc->size = hd->bufInfo1 & 0x1FFFFF;
#endif
    pPktDesc->segments[0] = (void *) (uintptr_t) (hd->bufPtr); 
    pPktDesc->segUsedSizes[0] = pPktDesc->size;
    pPktDesc->segAllocSizes[0] = hd->orgBufLen;

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
    CSL_udmapCppi5SetPsDataLen((void *)hd,  0);
    
    /* We only support one-layer at this moment */
    if ( pConnHdl->numHandles != 1)
    {
        salld_sim_print("sauRelayDmPkt: too many handles %d \n", pConnHdl->numHandles);
        return;
    }
    
    pHdrHdl = pConnHdl->pHdrHdl[0];
    
    sauFormatDmPkt(pConnHdl, pHdrHdl, &pktInfo, pPktDesc, pPktDesc->size - pConnHdl->hdrLen);

    /* Update EPIB information */
    CSL_udmapCppi5SetEpiDataPresent((void*) hd, EPI_PRESENT);
    CSL_udmapCppi5RdEpiData((void *) hd, &pTsInfo, &pSwInfo0, &pSwInfo1, &pSwInfo2);
    /* UPdate swInfo */
    pSwInfo0 = pktInfo.swInfo.swInfo[0];
    pSwInfo1 = pktInfo.swInfo.swInfo[1];
    pSwInfo2 = pktInfo.swInfo.swInfo[2];
    CSL_udmapCppi5WrEpiData((void *) hd, pTsInfo, pSwInfo0, pSwInfo1, pSwInfo2);
    
    /* Prepare to send pkt to PA/SA */
    CSL_udmapCppi5SetBufferAddr((CSL_UdmapCppi5HMPD*) hd, (uint64_t) Osal_VirtToPhys(pPktDesc->segments[0]));
    CSL_udmapCppi5SetBufferLen ((CSL_UdmapCppi5HMPD*) hd, pPktDesc->size);
    CSL_udmapCppi5SetPktLen((void *) hd, SA2UL_TEST_CPPI5_DESCRIPTOR_TYPE_HOST, pPktDesc->size);

    /* overwrite the flow id */
    CSL_udmapCppi5GetIds((void*) hd, &pktId, &flowId);
    /* Clear PS flags and any error */
    hd->pktInfo1 = 0;
    /* Clear any next descriptor pointer */
    hd->nextDescPtr = 0;
    flowId = tFramework.tfFlowNum;
    CSL_udmapCppi5SetIds((void*) hd, SA2UL_TEST_CPPI5_DESCRIPTOR_TYPE_HOST, pktId, flowId);

    /* Attach the command label in PS data */
    CSL_udmapCppi5SetPsDataLoc((void *) hd, CSL_UDMAP_CPPI5_PD_DESCINFO_PSINFO_VAL_IN_DESC);
    cmdLblAddr = CSL_udmapCppi5GetPsDataAddr((void*) hd, 0, EPI_PRESENT);
    memcpy ((void *) cmdLblAddr, pCmdLbInfo->cmdLbBuf, pCmdLbInfo->cmdLbSize);
    CSL_udmapCppi5SetPsDataLen((void *) hd, pCmdLbInfo->cmdLbSize);


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
void sauVerifyDmPkt(int connIndex, CSL_UdmapCppi5HMPD* hd)
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
#if defined (BUILD_MCU1_0) || defined (_TMS320C6X)
    rcvConnId = pktRead32bits_m((uint8_t *)(uint32_t)hd->bufPtr, pConnHdl->hdrLen);
#else
    rcvConnId = pktRead32bits_m((uint8_t *)hd->bufPtr, pConnHdl->hdrLen);
#endif
    if (rcvConnId != connIndex)
    {
        salld_sim_print("Receive Pkt of connection %d from PA/SA with incorrect connId = %d\n", connIndex, rcvConnId); 
    }
    
#if defined (BUILD_MCU1_0) || defined (_TMS320C6X)
    pData = (uint8_t *)(uint32_t)hd->bufPtr + pConnHdl->hdrLen + 4;
#else
    pData = (uint8_t *)hd->bufPtr + pConnHdl->hdrLen + 4;
#endif
    /* For CBC mode, the last partial block will be incorrect */
    /* dataLen = CSL_cppi5GetBufLen(CSL_CPPI5_DESCRIPTOR_TYPE_HOST, (uintptr_t *)&hd) - pConnHdl->hdrLen - 4 - 1 - 15; */
#if defined (SOC_AM64X)
    dataLen    = CSL_udmapCppi5GetPktLen((void *) hd);
#else
    dataLen    = CSL_udmapCppi5GetBufferLen((CSL_UdmapCppi5HMPD*) hd);
#endif
    dataLen   -= (pConnHdl->hdrLen + 4 + 1 + 15);
    
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

volatile uint32_t bookKeepTxCount = 0;
void sauBookKeepTx(void)
{
  FW_CPPI_DESC_T *pCppiDesc;

  bookKeepTxCount++;

  /* return queue processing begin */
  do
  {
      /* Need to free up the FW_PKT_DESC_T back to the application */

      /* Go through the TX Return queue which is where the descriptor is re-cycled after it is transmitted on the wire
          get the buffer which is this case is a FW_PKT_DESC_T and return it to the application/stack */
      RingPop(tFramework.gTxComplRingHandle, &pCppiDesc);

      if (pCppiDesc)  {
        
          /* clear source and dst tags */
          CSL_udmapCppi5SetSrcTag(&pCppiDesc->hostDesc, 0);
          CSL_udmapCppi5SetDstTag(&pCppiDesc->hostDesc, 0);
#if defined (ACP_COHERENCY)
          pCppiDesc->hostDesc.nextDescPtr = CSL_pktdmaClrAselInAddr(pCppiDesc->hostDesc.nextDescPtr);
          /* update the buffer for asel bits*/
          pCppiDesc->hostDesc.bufPtr  = CSL_pktdmaClrAselInAddr ((uint64_t) pCppiDesc->hostDesc.bufPtr);
#endif

         pCppiDesc->nextPtr = (uint64_t) (uintptr_t) tFramework.txReadyDescs;
         tFramework.txReadyDescs = pCppiDesc;
      }
  } while (pCppiDesc != NULL);
  /* return queue processing end */
}

volatile uint32_t bookKeepRxCount = 0;
void sauBookKeepRx(void)
{
  FW_CPPI_DESC_T     *pCppiDesc;
  CSL_UdmapCppi5HMPD *hd;

  bookKeepRxCount++;

  /* return queue processing begin */
  do
  {
      /* Need to free up the FW_PKT_DESC_T back to the application */
      pCppiDesc = (FW_CPPI_DESC_T *) NULL;
      hd        = (CSL_UdmapCppi5HMPD *) NULL;

      /* Go through the TX Return queue which is where the descriptor is re-cycled after it is transmitted on the wire
          get the buffer which is this case is a FW_PKT_DESC_T and return it to the application/stack */
      RingPop(tFramework.gTxComplRingHandle, &pCppiDesc);

      /* push to receive fdq */
      if (pCppiDesc != (FW_CPPI_DESC_T *) NULL)
      {
          hd  = &pCppiDesc->hostDesc;
#if defined (ACP_COHERENCY)
         /* Clear ASEL tagging */
          hd->nextDescPtr = CSL_pktdmaClrAselInAddr(hd->nextDescPtr);
          hd->bufPtr      = CSL_pktdmaClrAselInAddr(hd->bufPtr);
#endif

#if defined (SOC_AM64X)
     /* There is no default return policy field for AM64x */
#else
          /* update back to original return queue for Rx */
          CSL_udmapCppi5SetReturnPolicy( hd,
                                         CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                         CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                         CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                         CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                         tFramework.rxFreeRingNum);
#endif
          /* clear source and dst tags */
          CSL_udmapCppi5SetSrcTag(hd, 0);
          CSL_udmapCppi5SetDstTag(hd, 0);

          hd  = (CSL_UdmapCppi5HMPD * )Osal_PhysToVirt (hd);
          
      }
    /* Push descriptor to Rx free descriptor queue */
    if (hd != NULL) {
      CSL_udmapCppi5SetBufferLen((CSL_UdmapCppi5HMPD *) hd, TF_DESC_BUFSIZE);
      RingPush (tFramework.gRxFreeRingHandle, TF_DESC_BUFSIZE, (physptr_t)hd);
    }

  } while (hd != NULL);


    /* return queue processing begin */
  do
  {
      /* Need to free up the FW_PKT_DESC_T back to the application */
      pCppiDesc = (FW_CPPI_DESC_T *) NULL;
      hd        = (CSL_UdmapCppi5HMPD *) NULL;

      /* Go through the RX Return queue which is where the descriptor is re-cycled after it is transmitted on the wire
          get the buffer which is this case is a FW_PKT_DESC_T and return it to the application/stack */
      RingPop(tFramework.gRxRingHandle, &pCppiDesc);

      /* push to receive fdq */
      if (pCppiDesc != (FW_CPPI_DESC_T *) NULL)
      {
          hd  = &pCppiDesc->hostDesc;
#if defined (ACP_COHERENCY)
          /* Clear ASEL tagging */
          hd->nextDescPtr = CSL_pktdmaClrAselInAddr(hd->nextDescPtr);
          hd->bufPtr      = CSL_pktdmaClrAselInAddr(hd->bufPtr);
#endif

#if defined (SOC_AM64X)
#else
          /* update back to original return queue for Rx */
          CSL_udmapCppi5SetReturnPolicy( hd,
                                         CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                         CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                         CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                         CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                         tFramework.rxFreeRingNum);
#endif
          /* clear source and dst tags */
          CSL_udmapCppi5SetSrcTag(hd, 0);
          CSL_udmapCppi5SetDstTag(hd, 0);

          hd  = (CSL_UdmapCppi5HMPD * )Osal_PhysToVirt (hd);
      }
    /* Push descriptor to Rx free descriptor queue */
    if (hd != NULL) {
      CSL_udmapCppi5SetBufferLen((CSL_UdmapCppi5HMPD *) hd, TF_DESC_BUFSIZE);
      RingPush (tFramework.gRxFreeRingHandle, TF_DESC_BUFSIZE, (physptr_t)hd);
    }

  } while (hd != NULL);
  /* return queue processing end */
}

void sauCheckDesc(CSL_UdmapCppi5HMPD *hd, uint32_t connIndex, uint32_t pktNum, uint32_t lineNum)
{
  uint32_t pktlen, bufflen;

  pktlen  = CSL_udmapCppi5GetPktLen((void*) hd);
  bufflen = CSL_udmapCppi5GetBufferLen(hd);

  if (pktlen != bufflen)
  {
      SALog("from Line num: %d Conn %d(Pkt %d) (pktLen %d) (buffLen %d): pktlen does not match bufflen!\n", \
             lineNum, connIndex, pktNum, pktlen, bufflen); 
      System_flush();
  }

}

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

    CSL_UdmapCppi5HMPD *hd;
    Int       pktNum, connIndex;
    uint16_t  payloadLen;
    uint32_t  infoLen, pktLen, infoLen1;
    uint32_t  authTag[8];
    uint32_t  psFlags;
    uintptr_t psDataAddr;
    Int       i;


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

            gRxPktCntInRing = 0;
            pktLen = CSL_udmapCppi5GetPktLen((void *) hd);
            RingPush(tFramework.gTxRingHandle, pktLen, (physptr_t) hd);

              /* The packet should loop back into queue TF_RING_RXQ */    
            for (i = 0; i < 2000; i++)  {
                utilCycleDelay (1000);
                if (gRxPktCntInRing != 0)
                    break;
            }

            if (i >= 2000)  {
                SALog ("%s (%s:%d): Did not find SA reply in rx ring %d \n", pat->name, __FILE__, __LINE__, tFramework.rxComplRingNum);
                System_flush();
                salld_sim_halt();
                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }

            /* Book Keep the Tx */
            sauBookKeepTx();

            /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            if (gRxPktCntInRing != 0) 
            {
                FW_CPPI_DESC_T *pAppDesc;
                pAppDesc = (FW_CPPI_DESC_T *) NULL;
                hd = (CSL_UdmapCppi5HMPD *)(NULL);
                RingPop(tFramework.gRxRingHandle, &pAppDesc);
                if (pAppDesc != (FW_CPPI_DESC_T *) NULL)
                {
                    hd = &pAppDesc->hostDesc;
                }
                else
                {
                   SALog(" The null descriptor in the ring, unexpected behavior \n");
                   System_flush();
                   salld_sim_halt();
                }

                /* Check if the descriptor has some errors, if yes - halt the test for inspection */
                psFlags = CSL_udmapCppi5GetPsFlags(hd);

                if (psFlags)
                {
                   SALog(" The SA2UL returned error code = %d \n", psFlags);
                   System_flush();
                   salld_sim_halt();
                }
                
                /* Extract PS Info if exists */
                psDataAddr = CSL_udmapCppi5GetPsDataAddr((void*) hd, 0, 1);
                infoLen    = CSL_udmapCppi5GetPsDataLen((void *) hd);

                /* Copy Auth tag if any */
                if (infoLen != 0) {
                  memcpy(authTag, (void *) psDataAddr, infoLen);
                }

#if defined (DMA_TYPE_LCDMA)
#else
                sauCheckDesc(hd, connIndex, pktNum, __LINE__);
#endif
                salld_sim_print("Conn %d: Receive Data Mode Pkt %d from SA\n", connIndex, pktNum); 
                #if !SA_GEN_TEST_VECTOR_RX
                testDispPkts(hd);
                #endif

                /* Relay the packet to sa2ul */
                sauRelayDmPkt(connIndex+1, hd);
                /* Set the interrupt flag to false to indicate it is processed now */
                gRxPktCntInRing = 0;
#if defined (ACP_COHERENCY)
                hd->nextDescPtr = CSL_pktdmaMakeAselAddr((uint64_t) 0ULL, \
                                    CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC);
                hd->bufPtr      = CSL_pktdmaMakeAselAddr(hd->bufPtr, \
                                    CSL_LCDMA_RINGACC_ASEL_ENDPOINT_ACP_WR_ALLOC);
#endif

#if defined (SOC_AM64X)
#else
                /* Update the return ring for the descriptor to be Tx Complete
                   this gets set back during rx recycle
                 */
                CSL_udmapCppi5SetReturnPolicy( hd,
                                               CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                               CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                               CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                               CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                               tFramework.txComplRingNum);
#endif
                pktLen = CSL_udmapCppi5GetPktLen((void *) hd);
                RingPush(tFramework.gTxRingHandle, pktLen, (physptr_t) hd);
            }

              /* The packet should loop back into queue DEST_QUEUE_PKT_TX */    
            for (i = 0; i < 2000; i++)  {
                utilCycleDelay (1000);
                if (gRxPktCntInRing != 0)
                    break;
            }

            if (i >= 2000)  {
                salld_sim_halt();
                SALog ("%s (%s:%d): Did not find SA reply in rx ring  %d \n", pat->name, __FILE__, __LINE__, tFramework.rxComplRingNum);
                 saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
            }

            /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            if (gRxPktCntInRing != 0)
            {

                FW_CPPI_DESC_T *pAppDesc;
                pAppDesc = (FW_CPPI_DESC_T *) NULL;
                hd = (CSL_UdmapCppi5HMPD *)(NULL);
                RingPop(tFramework.gRxRingHandle, &pAppDesc);
                if (pAppDesc != (FW_CPPI_DESC_T *) NULL)
                {
                    hd = &pAppDesc->hostDesc;
                }

                /* Indicate the Rx pkt is serviced now */
                gRxPktCntInRing = 0;

                /* Check if the descriptor has some errors, if yes - halt the test for inspection */
                psFlags = CSL_udmapCppi5GetPsFlags(hd);

                if (psFlags)
                {
                   SALog(" The SA2UL returned error code = %d \n", psFlags);
                   System_flush();
                   salld_sim_halt();
                }
#if defined (DMA_TYPE_LCDMA)
#else
                sauCheckDesc(hd, connIndex+1, pktNum, __LINE__);
#endif
                salld_sim_print("Conn %d: Receive Data Mode Pkt %d from SA\n", connIndex+1, pktNum); 
                /* Extract PS Info if exists */
                psDataAddr = CSL_udmapCppi5GetPsDataAddr((void*) hd, 0, 1);
                infoLen1    = CSL_udmapCppi5GetPsDataLen((void *) hd);
                if ((infoLen) || (infoLen1))
                {
                    if(memcmp((void*) psDataAddr, authTag, infoLen))
                    {
                        SALog("Conn %d(Pkt %d): authentication tag does not match!\n", connIndex+1, pktNum);
                        System_flush();
                     }
                }
                
                sauVerifyDmPkt(connIndex, hd);

                /* Book keep the Rx */
                sauBookKeepRx();
            }

        } /* Connection Loop */

    } /* Packet Loop */
}


extern volatile sauTest_tputData_t gSauTputData[SAU_TPUT_NUM_PKTSIZES][SAU_TPUT_MAX_PROFILE_CHANNELS];

#define SAU_TPUT_TEST_DESC_DUMP 0

#if SAU_TPUT_TEST_DESC_DUMP
volatile CSL_UdmapCppi5HMPD    gSauDescEncDumpMem[SAU_TPUT_NUM_PKTSIZES * SAU_TPUT_MAX_PROFILE_CHANNELS * SAU_TPUT_PKTBATCH_CNT];
volatile uint32_t    gSauDescDumpEncIndex;
volatile CSL_UdmapCppi5HMPD    gSauDescDecDumpMem[SAU_TPUT_NUM_PKTSIZES * SAU_TPUT_MAX_PROFILE_CHANNELS * SAU_TPUT_PKTBATCH_CNT];
volatile uint32_t    gSauDescDumpDecIndex;
#endif

/*******************************************************************************
 *  Function: Througput Test Data collection and Verification
 *******************************************************************************
 *  DESCRIPTION:  This function performs throughput packet test/verification
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
void sauConnTputDmPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts)
{
    CSL_UdmapCppi5HMPD *hd[SAU_TPUT_PKTBATCH_CNT+1];
    Int       connIndex, maxBatch;
#if   SAU_TPUT_TEST_DESC_DUMP
    uint32_t  descType;
#else
    Int       encFirstTime, decFirstTime;
    volatile uint64_t  encGtcCumSumVal = 0, decGtcCumSumVal = 0;
    volatile uint64_t  gtcTxStart, gtcTxEnd;
    volatile uint64_t  gtcRxStart, gtcRxEnd, overhead;
#endif
    uint16_t  pktNum,  nPkts, payloadLen;
    uint32_t  infoLen, numPktsRx, pLen;
    uint32_t  authTag[SAU_TPUT_PKTBATCH_CNT][8];
    uintptr_t psDataAddr;
    Int       i;
    Int       pIdx;
    Int       pktLen[SAU_TPUT_NUM_PKTSIZES] = { 64, 512, 1024, 2048 };
    uint32_t  psFlags;

    /* Initialize the cycle counter */
    sau_cycle_cnt_init();
    gtcTxStart = sau_read_cycle_cnt();
    gtcTxEnd = sau_read_cycle_cnt();
    overhead = gtcTxEnd - gtcTxStart;
    gtcTxStart = sau_read_cycle_cnt();
    /* 32 cycle NOP instructions, which can be used to compute the overhead */
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");
    asm (" NOP ");     asm (" NOP ");    asm (" NOP ");    asm (" NOP ");

    gtcTxEnd = sau_read_cycle_cnt();
    /* Total cycle should be 32 cycles + overhead */
    encGtcCumSumVal = gtcTxEnd - gtcTxStart - overhead;

#if defined(BUILD_MCU)
    SALog("Captured cycle is = 0x%" PRIx64 " \n", encGtcCumSumVal);
    SALog("overhead cycle is = 0x%" PRIx64 " \n", overhead);
#else
    SALog("Captured cycle is = 0x%" PRIx64 " \n", encGtcCumSumVal);
    SALog("overhead cycle is = 0x%" PRIx64 " \n", overhead);
#endif


#if SAU_TPUT_TEST_DESC_DUMP
    gSauDescDumpEncIndex =  gSauDescDumpDecIndex = 0;
#endif

    numPkts = SAU_TPUT_PKTBATCH_CNT;

    SALog("Running the SA Throughput Test to collect data ... (Please don't halt)");

    for (pIdx=0; pIdx < SAU_TPUT_NUM_PKTSIZES; pIdx++)
    {
        SALog("\n    For Packet Size %d: \n       Ch IDs: ", pktLen[pIdx]);
        for (connIndex = 0; connIndex < numConnHandles; connIndex += 2)
        {
            payloadLen = pktLen[pIdx];
            SALog ("%d  ", connIndex);

            nPkts = numPkts;
            /* Generate the payload for sa2ul */
            sauGenPayload(SAU_PAYLOAD_INC8, 0,  payloadLen, sap_pkt_test_buf);

#if   SAU_TPUT_TEST_DESC_DUMP
#else
            gSauTputData[pIdx][connIndex].connId    = connIndex;
            gSauTputData[pIdx][connIndex].pktLen    = pktLen[pIdx];
            gSauTputData[pIdx][connIndex].numPkts   = numPkts;

            encFirstTime = TRUE;
            decFirstTime = TRUE;
#endif

            pktNum = 0;
            while(pktNum < nPkts)
            {
                if (nPkts >= SAU_TPUT_PKTBATCH_CNT)
                {
                    maxBatch = SAU_TPUT_PKTBATCH_CNT;
                }
                else
                {
                    maxBatch = nPkts;
                }

                /* Pause the Tx UDMAP channel to prevent sa2ul to consume it */
                salld_test_controlTxDma(SA2UL_UDMAP_TX_PAUSE);

                /* Push in one shot to encrypt the packets */
                gRxPktCntInRing = 0;
                for (i = 0; i < maxBatch; i++)
                {
                    hd[i] = sauGenDmPkt(tf, connIndex, payloadLen, sap_pkt_test_buf);
                    if (hd[i] == NULL)
                    {
                        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                    }

#if   SAU_TPUT_TEST_DESC_DUMP
                    /* Check if the descriptor is valid host desc */
                    descType = CSL_udmapCppi5GetDescType((void *)hd[i]);
                    if (descType != CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST)
                    {
                        salld_sim_halt();
                    }
                    memcpy((void *)&gSauDescEncDumpMem[gSauDescDumpEncIndex++], (void *) hd[i], sizeof(CSL_UdmapCppi5HMPD));
#endif
                    pLen = CSL_udmapCppi5GetPktLen((void *) hd[i]);

                    /* Benchmarking Ring Push API */
                    if (i == 0)
                    {
                        gtcTxStart = sau_read_cycle_cnt();
                    }
                    RingPush(tFramework.gTxRingHandle, pLen, (physptr_t) hd[i]);
                    if (i == 0)
                    {
                        gtcTxEnd = sau_read_cycle_cnt();
                        gSauTputData[pIdx][connIndex].ringPushCycles = gtcTxEnd - gtcTxStart - overhead;
                    }
                }

                /*Make sure there is no packet in the Rx ring before enable of TX DMA */
                do
                {
                    utilCycleDelay(1000);
                } while (gRxPktCntInRing);

                /* Now start the profile */
                gtcTxStart = sau_read_cycle_cnt();
                /* The packets should loop back into queue TF_RING_RXQ after the
                 * udma is enabled
                 */
                 salld_test_controlTxDma(SA2UL_UDMAP_TX_UNPAUSE);
#if defined(SOC_AM64X)
                /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
                do
                {
                   numPktsRx =  CSL_lcdma_ringaccGetReverseRingOcc(&tFramework.gDrvHandle->lcdmaRaRegs, \
                                 tFramework.rxComplRingNum, CSL_LCDMA_RINGACC_RING_MODE_RX_RING);
                } while (numPktsRx < maxBatch);
#else
                /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
                do
                {
                   numPktsRx =  CSL_ringaccGetRingOcc(&tFramework.gDrvHandle->raRegs, tFramework.rxComplRingNum);
                } while (numPktsRx < maxBatch);
#endif

#if   SAU_TPUT_TEST_DESC_DUMP
#else
                /* Stop the profile */
                gtcTxEnd = sau_read_cycle_cnt();

                /* Book keep */
                sauBookKeepTx(); 
                encGtcCumSumVal = gtcTxEnd - gtcTxStart - overhead;

                /* Check */
                if (numPktsRx > maxBatch)
                {
                    /* The ring can not get more than pushed for sa2ul operations */
                    SALog("expected %d packets in the ring, got %d packets \n", maxBatch, numPktsRx);
                    salld_sim_halt();
                }

                /* Get the recived packets out of SA2UL */
                for (i = 0; i < numPktsRx; i++)
                {
                    FW_CPPI_DESC_T *pAppDesc;
                    pAppDesc = (FW_CPPI_DESC_T *) NULL;
                    hd[i] = (CSL_UdmapCppi5HMPD *)(NULL);
                                        /* Benchmarking Ring Push API */
                    if (i == 0)
                    {
                        gtcTxStart = sau_read_cycle_cnt();
                    }
                    RingPop(tFramework.gRxRingHandle, &pAppDesc);
                    if (i == 0)
                    {
                        gtcTxEnd = sau_read_cycle_cnt();
                        gSauTputData[pIdx][connIndex].ringPopCycles = gtcTxEnd - gtcTxStart - overhead;
                    }

                    if (pAppDesc != (FW_CPPI_DESC_T *) NULL)
                    {
                        hd[i] = &pAppDesc->hostDesc;
                    }
                    /* Check if the descriptor has some errors, if yes - halt the test for inspection */
                    psFlags = CSL_udmapCppi5GetPsFlags(hd[i]);

                    if (psFlags)
                    {
                       SALog(" The SA2UL returned error code = %d \n", psFlags);
                       System_flush();
                       salld_sim_halt();
                    }
                }

                gSauTputData[pIdx][connIndex].cycles = (uint32_t)encGtcCumSumVal;

                /* Collect the first batch encryption cycles */
                if (encFirstTime == TRUE)
                {
                    encFirstTime = FALSE;
                }

                gSauTputData[pIdx][connIndex + 1].connId    = connIndex+1;
                gSauTputData[pIdx][connIndex + 1].pktLen    = pktLen[pIdx];
                gSauTputData[pIdx][connIndex + 1].numPkts   = numPkts;
#endif

                /* Pause the DMA channels after all packets are sent to SA2UL */
                salld_test_controlTxDma(SA2UL_UDMAP_TX_PAUSE);

                /* Relay the packet to Decrypt the received packets */
                gRxPktCntInRing  = 0;
                for (i = 0; i < maxBatch; i++)
                {
                    psDataAddr = CSL_udmapCppi5GetPsDataAddr((void *) hd[i],false,true);
                    infoLen    = CSL_udmapCppi5GetPsDataLen((void*) hd[i]);
                    if (infoLen != 0)
                    {
                        memcpy(&authTag[i][0], (void *) psDataAddr, infoLen);
                    }

                    /* Relay the packet */
                    sauRelayDmPkt(connIndex + 1,hd[i]);
#if   SAU_TPUT_TEST_DESC_DUMP
                    /* Check if the descriptor is valid host desc */
                    descType = CSL_udmapCppi5GetDescType((void *)hd[i]);
                    if (descType != CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST)
                    {
                        salld_sim_halt();
                    }
                    memcpy((void *)&gSauDescDecDumpMem[gSauDescDumpDecIndex++], (void *) hd[i], sizeof(CSL_UdmapCppi5HMPD));
#endif
#if defined (SOC_AM64X)
#else
                    /* Update the return ring for the descriptor to be Tx Complete
                       this gets set back during rx recycle
                     */
                    CSL_udmapCppi5SetReturnPolicy( hd[i],
                                                   CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST,
                                                   CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPOLICY_VAL_ENTIRE_PKT,
                                                   CSL_UDMAP_CPPI5_PD_PKTINFO2_EARLYRET_VAL_NO,
                                                   CSL_UDMAP_CPPI5_PD_PKTINFO2_RETPUSHPOLICY_VAL_TO_TAIL,
                                                   tFramework.txComplRingNum);
#endif
                    pLen = CSL_udmapCppi5GetPktLen((void *) hd[i]);

                    /* Benchmarking Ring Push API */
                    if (i == 0)
                    {
                        gtcTxStart = sau_read_cycle_cnt();
                    }
                    RingPush(tFramework.gTxRingHandle, pLen, (physptr_t) hd[i]);

                    if (i == 0)
                    {
                        gtcTxEnd = sau_read_cycle_cnt();
                        gSauTputData[pIdx][connIndex+1].ringPushCycles = gtcTxEnd - gtcTxStart - overhead;
                    }
                }

                /* Make sre there are no Rx Pkts before enable of TX DMA */
                do
                {
                    utilCycleDelay(1000);
                } while (gRxPktCntInRing);

                /* The packets should loop back into queue TF_RING_RXQ after the 
                 * udma is unpaused
                 */

#if   SAU_TPUT_TEST_DESC_DUMP
#else
                 numPktsRx = 0;
#endif

                /* Now start the profile */
                 gtcRxStart = sau_read_cycle_cnt();
                 salld_test_controlTxDma(SA2UL_UDMAP_TX_UNPAUSE);

#if defined(SOC_AM64X)
                 while (numPktsRx < maxBatch)
                 {
                     numPktsRx =  CSL_lcdma_ringaccGetReverseRingOcc(&tFramework.gDrvHandle->lcdmaRaRegs, \
                                     tFramework.rxComplRingNum, CSL_LCDMA_RINGACC_RING_MODE_RX_RING);
                 }

#else
                 while (numPktsRx < maxBatch)
                 {
                     numPktsRx =  CSL_ringaccGetRingOcc(&tFramework.gDrvHandle->raRegs, tFramework.rxComplRingNum);
                 }
#endif

#if   SAU_TPUT_TEST_DESC_DUMP
#else
                 /* Stop the profile */
                 gtcRxEnd = sau_read_cycle_cnt();
                 decGtcCumSumVal = gtcRxEnd - gtcRxStart - overhead;

                 /* Pause the Tx UDMAP channel to prevent sa2ul to consume further packets */
                  salld_test_controlTxDma(SA2UL_UDMAP_TX_PAUSE); 

                 /* Check */
                 if (numPktsRx > maxBatch)
                 {
                     /* The ring can not get more than pushed for sa2ul operations */
                     SALog("expected %d packets in the ring, got %d packets \n", maxBatch, numPktsRx);
                     salld_sim_halt();
                 }
 
                 /* Get the recived packets out of SA2UL */
                 for (i = 0; i < numPktsRx; i++)
                 {
                     FW_CPPI_DESC_T *pAppDesc;
                     pAppDesc = (FW_CPPI_DESC_T *) NULL;
                     hd[i] = (CSL_UdmapCppi5HMPD *)(NULL);
                     /* Benchmarking Ring Pop API */
                     if (i == 0)
                     {
                         gtcTxStart = sau_read_cycle_cnt();
                     }
                     RingPop(tFramework.gRxRingHandle, &pAppDesc);
                     if (i == 0)
                     {
                         gtcTxEnd = sau_read_cycle_cnt();
                         gSauTputData[pIdx][connIndex+1].ringPopCycles = gtcTxEnd - gtcTxStart - overhead;
                     }

                     if (pAppDesc != (FW_CPPI_DESC_T *) NULL)
                     {
                         hd[i] = &pAppDesc->hostDesc;
                     }
                     /* Check if the descriptor has some errors, if yes - halt the test for inspection */
                     psFlags = CSL_udmapCppi5GetPsFlags(hd[i]);

                     if (psFlags)
                     {
                        SALog(" The SA2UL returned error code = %d \n", psFlags);
                        System_flush();
                        salld_sim_halt();
                     }

                 }
                gSauTputData[pIdx][connIndex+1].cycles = (uint32_t)decGtcCumSumVal;

#endif /* SAU_TPUT_TEST_DESC_DUMP */

                /* Book keep */
                sauBookKeepRx();

                /* compare the functionality */
                for (i = 0 ; i < maxBatch; i++)
                {
                    psDataAddr = CSL_udmapCppi5GetPsDataAddr((void *) hd[i],false,true);
                    infoLen    = CSL_udmapCppi5GetPsDataLen((void*) hd[i]);
                    if (infoLen != 0)
                    {
                        if (memcmp((void*) psDataAddr, &authTag[i][0],infoLen))
                        {
                            SALog("Conn %d(Pkt %d): authentication tag does not match!\n", connIndex+1, pktNum + i);
                            System_flush();
                        }
                    }
#if       SAU_TPUT_TEST_DESC_DUMP
                    /* for descriptor dump, don't verify data packets */
                    /* free buffer */
                    testCommonRecycleLBDesc (&tFramework, hd[i]); /* No Error Check here */
#else
                    /* verify */
                    sauVerifyDmPkt(connIndex, hd[i]);
#endif
                }

#if   SAU_TPUT_TEST_DESC_DUMP
#else
                /* Collect the first batch decryption cycles */
                if (decFirstTime == TRUE)
                {
                    decFirstTime = FALSE;
                }
#endif
                /* Indicate maxBatch number of packets are processed */
                pktNum += maxBatch;
            }
        }
    }

    SALog("\nDone ...\n");

    /* Re-enable the TX DMA back to always enable state */
    salld_test_controlTxDma(SA2UL_UDMAP_TX_UNPAUSE);
}

/* Nothing past this point */

