/******************************************************************************
 * FILE PURPOSE: SALLD Automation commands.
 ******************************************************************************
 * FILE NAME:   salldcfg.c
 *
 * DESCRIPTION: Defines the SALLD module unit test configuration.
 *
 * (C) Copyright 2009-2013, Texas Instruments, Inc.
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
#include "../unittest.h"
#include <ti/drv/sa/salld.h>
#include "salldcfg.h"

/* Protocol Specific default configurations */
static Sa_SrtpConfigParams_t salld_gencfg_srtp[2] = {
    {
        16,         /* masterKeySize */
        14,         /* masterSaltSize */
        16,         /* sessionEncKeySize */
        20,         /* sessionMacKeySize */
        14,         /* sessionSaltSize */   
        10          /* macSize */
    },
    
    {
        16,         /* masterKeySize */
        14,         /* masterSaltSize */
        16,         /* sessionEncKeySize */
        20,         /* sessionMacKeySize */
        14,         /* sessionSaltSize */   
        10          /* macSize */
    }
};

static Sa_IpsecConfigParams_t salld_gencfg_ipsec_esp[2] = {
    {
        sa_IPSEC_TRANSPORT_TUNNEL,       /* transport Type */
        0,                               /* ctrlBitMap: sa_IPSEC_CONFIG_ESN */
        4,                               /* encryptionBlockSize */
        16,                              /* sessionEncKeySize */
        20,                              /* sessionMacKeySize */
        4,                               /* sessionSaltSize */
        8,                               /* ivSize */
        8,                               /* macSize */
        4,                               /* nextHdr */
        0x87654320,                      /* spi */
        0x12121212,                      /* esnLo */
        0x22222220                       /* esnHi */        
    },
    {
        sa_IPSEC_TRANSPORT_TUNNEL,       /* transport Type */
        0,                               /* ctrlBitMap:sa_IPSEC_CONFIG_ESN */
        4,                               /* encryptionBlockSize */
        16,                              /* sessionEncKeySize */
        20,                              /* sessionMacKeySize */
        4,                               /* sessionSaltSize */
        8,                               /* ivSize */
        8,                               /* macSize */
        4,                               /* nextHdr */
        0x44444444,                      /* spi */
        0x12121212,                      /* esnLo */
        0x11111110                       /* esnHi */        
    }
};

static Sa_IpsecConfigParams_t salld_gencfg_ipsec_ah[2] = {
    {
        sa_IPSEC_TRANSPORT_TUNNEL,       /* transport Type */
        sa_IPSEC_CONFIG_ESN,             /* ctrlBitMap */
        0,                               /* encryptionBlockSize */
        0,                               /* sessionEncKeySize */
        20,                              /* sessionMacKeySize */
        0,                               /* sessionSaltSize */
        0,                               /* ivSize */
        8,                               /* macSize */
        4,                               /* nextHdr */
        0x87654320,                      /* spi */
        0,                               /* esnLo */
        0x44444440                       /* esnHi */        
    },
    {
        sa_IPSEC_TRANSPORT_TUNNEL,       /* transport Type */
        0,                               /* ctrlBitMap */
        0,                               /* encryptionBlockSize */
        0,                               /* sessionEncKeySize */
        20,                              /* sessionMacKeySize */
        0,                               /* sessionSaltSize */
        0,                               /* ivSize */
        8,                               /* macSize */
        4,                               /* nextHdr */
        0x55555555,                      /* spi */
        0x12121212,                      /* esnLo */
        0x33333330                       /* esnHi */        
    }
};

static Sa_AcConfigParams_t salld_gencfg_3gpp_ac[2] = {
    {
        0x2000,         /* Count C */
        0x87654321,     /* Fresh */        
        0x1345600,      /* ivLow26 */
        sa_AcPduType_LTE,
        0x1A,           /* ctrlBitMap */
        16,             /* sessionEncKeySize */
        16,             /* sessionMacKeySize */
        16,             /* ivSize */
        0               /* macSize */
    },    

    {
        0x2000,         /* Count C */
        0x87654321,     /* Fresh */        
        0x1345600,      /* ivLow26 */
        sa_AcPduType_LTE,
        0x1A,           /* ctrlBitMap */
        16,             /* sessionEncKeySize */
        16,             /* sessionMacKeySize */
        16,             /* ivSize */
        0               /* macSize */
    }    
};

static Sa_DataModeConfigParams_t salld_gencfg_data_mode[2] = {
    {
        16,     /* sessionEncKeySize */
        16,     /* sessionMacKeySize */
         4,     /* sessionSaltSize */
        16,     /* ivSize */
         8,     /* macSize */
         0,     /* aadSize */
        TRUE,   /* enc */
        TRUE    /* enclst */
#if defined(NSS_LITE2)
        ,0      /* priv for the data channel */
        ,0xC3     /* PrivId for the channels */
#endif
    },

    {
        16,     /* sessionEncKeySize */
        16,     /* sessionMacKeySize */
         4,     /* sessionSaltSize */
        16,     /* ivSize */
         8,     /* macSize */
         0,     /* aadSize */
        TRUE,   /* enc */
        TRUE    /* enclst */    
#if defined(NSS_LITE2)
        ,0      /* priv for the data channel */
        ,0xC3     /* PrivId for the channels */
#endif
    }
};

static uint8_t salldsim_key1[32] = 
        {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
         0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
static uint8_t salldsim_key2[32] =
        {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 
         0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
static uint8_t salldsim_key3[32] = 
        {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x00,
         0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x00};
static uint8_t salldsim_key4[32] = 
        {0x1F, 0x1E, 0x1D, 0x1C, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00,
         0x1F, 0x1E, 0x1D, 0x1C, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x00};
        
        
static uint8_t salldsim_salt1[16] = 
        {0x0F, 0x0E, 0x0D, 0x0C, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x00, };
static uint8_t salldsim_salt2[4] = {0xde, 0xad, 0, 0};
static uint8_t salldsim_salt3[4] = {0xde, 0xad, 0xbe, 0xef};
        
static Sa_SrtpKeyParams_t salld_keycfg_srtp[2] = {
    {
        sa_SRTP_KEY_CTRL_MASTER_KEY         |
        sa_SRTP_KEY_CTRL_MASTER_SALT        |
        sa_SRTP_KEY_CTRL_KEY_DERIVE_RATE    |
        sa_SRTP_KEY_CTRL_KEY_LIFETIME       |
        sa_SRTP_KEY_CTRL_ROC,                
        
        salldsim_key1,
        salldsim_salt1,
        
        24,             /* derivRate */
        1,              /* keyLifeTimeMsw */
        0,              /* keyLifeTimeLsw */  
        0x66666600,     /* roc */
        0,              /* fromEsnMsw */
        0,              /* fromEsnLsw */
        0,              /* toEsnMsw */
        0,              /* toEsnLsw */
        0,              /* MKI length */
        0               /* MKI */
    }
    ,
    
    {
        sa_SRTP_KEY_CTRL_MASTER_KEY         |
        sa_SRTP_KEY_CTRL_MASTER_SALT        |
        sa_SRTP_KEY_CTRL_KEY_DERIVE_RATE    |
        sa_SRTP_KEY_CTRL_KEY_LIFETIME       |
        sa_SRTP_KEY_CTRL_ROC,                
        
        salldsim_key1,
        salldsim_salt1,
        
        24,             /* derivRate */
        1,              /* keyLifeTimeMsw */
        0,              /* keyLifeTimeLsw */  
        0x55555500,     /* roc */
        0,              /* fromEsnMsw */
        0,              /* fromEsnLsw */
        0,              /* toEsnMsw */
        0,              /* toEsnLsw */
        0,              /* MKI Length */
        0               /* MKI */
    }
};

static Sa_IpsecKeyParams_t salld_keycfg_ipsec_esp[2] = {
    {
         sa_IPSEC_KEY_CTRL_ENC_KEY |
         sa_IPSEC_KEY_CTRL_MAC_KEY |
         sa_IPSEC_KEY_CTRL_SALT,
         /* sessionEncKey */
         salldsim_key3,
         /* sessionAuthKey */
         salldsim_key4,
         /* sessionSalt */
         salldsim_salt2
    },

    {
         sa_IPSEC_KEY_CTRL_ENC_KEY |
         sa_IPSEC_KEY_CTRL_MAC_KEY |
         sa_IPSEC_KEY_CTRL_SALT,
         /* sessionEncKey */
         salldsim_key1,
         /* sessionAuthKey */
         salldsim_key2,
         /* sessionSalt */
         salldsim_salt3
    }
};

static Sa_IpsecKeyParams_t salld_keycfg_ipsec_ah[2] = {
    {
         sa_IPSEC_KEY_CTRL_MAC_KEY, 
         /* sessionEncKey */
         salldsim_key3,
         /* sessionAuthKey */
         salldsim_key4,
         /* sessionSalt */
         salldsim_salt2
    },

    {
         sa_IPSEC_KEY_CTRL_MAC_KEY,
         /* sessionEncKey */
         salldsim_key1,
         /* sessionAuthKey */
         salldsim_key2,
         /* sessionSalt */
         salldsim_salt3
    }
};


static Sa_AcKeyParams_t salld_keycfg_3gpp_ac[2] = {

    {
         sa_AC_KEY_CTRL_ENC_KEY | sa_AC_KEY_CTRL_MAC_KEY, 
         /* sessionEncKey */
         salldsim_key1,
         /* sessionAuthKey */
         salldsim_key2
    },

    {
         sa_AC_KEY_CTRL_ENC_KEY | sa_AC_KEY_CTRL_MAC_KEY,
         /* sessionEncKey */
         salldsim_key1,
         /* sessionAuthKey */
         salldsim_key2
    }
};

static Sa_DataModeKeyParams_t salld_keycfg_data_mode[2] = {
    {
         sa_DATA_MODE_KEY_CTRL_ENC_KEY |
         sa_DATA_MODE_KEY_CTRL_MAC_KEY |
         sa_DATA_MODE_KEY_CTRL_SALT,
         /* sessionEncKey */
         salldsim_key3,
         /* sessionAuthKey */
         salldsim_key4,
         /* sessionSalt */
         salldsim_salt2
    },

    {
         sa_DATA_MODE_KEY_CTRL_ENC_KEY |
         sa_DATA_MODE_KEY_CTRL_MAC_KEY |
         sa_DATA_MODE_KEY_CTRL_SALT,
         /* sessionEncKey */
         salldsim_key1,
         /* sessionAuthKey */
         salldsim_key2,
         /* sessionSalt */
         salldsim_salt3
    }
};


/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "GENCFG" handler function.
 ******************************************************************************
 * DESCRIPTION: UT configuration handler for "GENCFG"
 *
 *  void salldcfg_set_gencfg (
 *     Sa_SecProto_e protocolType,
 *     Sa_GenConfigParams_t *pGenCfg, 
 *     Bool tx)
 *
 *****************************************************************************/
void salldcfg_set_gencfg (Sa_SecProto_e protocolType, Sa_GenConfigParams_t *pGenCfg, Bool tx)
{
    int cfg_index = tx?SALLD_UT_CFG_INFO_INDEX_TX:SALLD_UT_CFG_INFO_INDEX_RX;

    switch (protocolType)
    {
        case sa_PT_NULL:
#if defined(NSS_LITE2)
            /* Wild card match for the incoming packets's for PrivId
             * User privillage for the channel
             */
             salld_gencfg_data_mode[cfg_index].priv              = 0;
             salld_gencfg_data_mode[cfg_index].privId            = 0xC3;
#endif
        pGenCfg->params.data = salld_gencfg_data_mode[cfg_index];
        break;
    
        case sa_PT_SRTP:
        case sa_PT_SRTCP:
            pGenCfg->params.srtp = salld_gencfg_srtp[cfg_index];
        break;
        
        case sa_PT_IPSEC_AH:
            pGenCfg->params.ipsec = salld_gencfg_ipsec_ah[cfg_index];
        break;
        
        case sa_PT_IPSEC_ESP:
            pGenCfg->params.ipsec = salld_gencfg_ipsec_esp[cfg_index];
        break;
        
        case sa_PT_3GPP_AC:
            pGenCfg->params.ac = salld_gencfg_3gpp_ac[cfg_index];
        
        break;
        
        default:
        
        break;
    }  
}

/******************************************************************************
 * FUNCTION PURPOSE: SALLD configuration "KEYCFG" handler function.
 ******************************************************************************
 * DESCRIPTION: UT configuration handler for "KEYCFG"
 *
 *  void salldcfg_set_keycfg (
 *     Sa_SecProto_e protocolType,
 *     Sa_ProtocolKeyParams_t *pKeyCfg, 
 *     int16_t  chnum,
 *     Bool tx)
 *
 *****************************************************************************/
void salldcfg_set_keycfg (Sa_SecProto_e protocolType, Sa_ProtocolKeyParams_t *pKeyCfg, 
                          int16_t  chnum, Bool tx)
{
    int cfg_index = tx?SALLD_UT_CFG_INFO_INDEX_TX:SALLD_UT_CFG_INFO_INDEX_RX;

    switch (protocolType)
    {
        case sa_PT_NULL:
            pKeyCfg->data = salld_keycfg_data_mode[cfg_index];
        break;
    
        case sa_PT_SRTP:
        case sa_PT_SRTCP:
            pKeyCfg->srtp = salld_keycfg_srtp[cfg_index];
        break;
        
        case sa_PT_IPSEC_AH:
            pKeyCfg->ipsec = salld_keycfg_ipsec_ah[cfg_index];
        break;
        
        case sa_PT_IPSEC_ESP:
            pKeyCfg->ipsec = salld_keycfg_ipsec_esp[cfg_index];
        break;
        
        case sa_PT_3GPP_AC:
            pKeyCfg->ac = salld_keycfg_3gpp_ac[cfg_index];
        
        break;
        
        default:
        
        break;
    }  
}

/******************************************************************************
 * FUNCTION PURPOSE: SALLD Test configuration function.
 ******************************************************************************
 * DESCRIPTION: Prepare the SALLD channel and associated resources for the
 *  specified unit test configuration
 *
 *  void salldcfg_test_init (
 *    salldTestConfig_t* pTestCfg,      - pointer to channel configuration)
 *
 *****************************************************************************/
void salldcfg_chan_init (salldTestConfig_t* pTestCfg)
{
    salldSimChannel_t *pSimChan = &tFramework.salldSimChn[pTestCfg->chnum];
    Sa_GenConfigParams_t *pTxGenCfg = &pSimChan->txInfo.genCfg;
    Sa_GenConfigParams_t *pRxGenCfg = &pSimChan->rxInfo.genCfg;
    Sa_ProtocolKeyParams_t *pTxKeyCfg = &pSimChan->txInfo.keyCfg;
    Sa_ProtocolKeyParams_t *pRxKeyCfg = &pSimChan->rxInfo.keyCfg;
    
    /* Initialize the channel */
    memset(pSimChan, 0, sizeof(salldSimChannel_t));
    pSimChan->ID = (salld_SIM_ID << 8) | (pTestCfg->chnum + 1);
    pSimChan->rxInfo.pgnIndex = SALLD_PGN_INDEX_NONE;
    pSimChan->txInfo.pgnIndex = SALLD_PGN_INDEX_NONE;
    
    /* Non-protocol specific channel configuration */
    pSimChan->protocolType = pTestCfg->protocolType;
    pSimChan->relayWinSize = pTestCfg->relayWinSize;
    
    pTxGenCfg->destInfo = pTestCfg->destInfo[SALLD_UT_CFG_INFO_INDEX_TX];
    pTxGenCfg->cipherMode = pTestCfg->cipherMode;
    pTxGenCfg->authMode = pTestCfg->authMode;
    
    pRxGenCfg->destInfo = pTestCfg->destInfo[SALLD_UT_CFG_INFO_INDEX_RX];
    pRxGenCfg->cipherMode = pTestCfg->cipherMode;
    pRxGenCfg->authMode = pTestCfg->authMode;
    
    /* Protocol specific channel configuration */
    salldcfg_set_gencfg(pSimChan->protocolType, pTxGenCfg, TRUE);
    salldcfg_set_gencfg(pSimChan->protocolType, pRxGenCfg, FALSE);
    salldcfg_set_keycfg(pSimChan->protocolType, pTxKeyCfg, pTestCfg->chnum, TRUE);
    salldcfg_set_keycfg(pSimChan->protocolType, pRxKeyCfg, pTestCfg->chnum, FALSE);
}

/* nothing past this point */
