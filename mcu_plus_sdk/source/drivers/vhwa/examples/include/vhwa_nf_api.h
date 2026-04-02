/*
*  Copyright (c) Texas Instruments Incorporated 2018
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
*/

/**
*  \file vhwa_nf_api.h
*
*  \brief VHWA NF API and struct definitions
*/
#ifndef VPAC_EX_NF_API_H_
#define VPAC_EX_NF_API_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <include/vhwa_m2mNf.h>
#include <examples/include/vhwa_examples_common.h>
#include <examples/include/vhwa_common_crc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
* Application test parameters
*/
#define APP_NF_MAX_HANDLES           (4U)

#define APP_MAX_IMG_WIDTH            (8192U)
#define APP_INPUT_STORAGE_FORMAT     (FVID2_CCSF_BITS16_PACKED)
#define APP_MAX_SL2_IN_BUFF_DEPTH    (VHWA_M2M_NF_MIN_RD_BUFFER_DEPTH)
#define APP_MAX_SL2_OUT_BUFF_DEPTH   (VHWA_M2M_NF_MIN_WR_BUFFER_DEPTH)
#define APP_OUTPUT_STORAGE_FORMAT    (FVID2_CCSF_BITS16_PACKED)

#define APP_NF_CRC_CHANNEL           (CRC_CHANNEL_1)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct
{
    uint64_t refBuff;
    uint64_t currBuff;
    uint64_t tempBuff;
    uint64_t sofBuff;
    uint64_t outBuff;
    uint64_t interOddBuff;
    uint64_t interEvenBuff;
    uint32_t *refBuffIdx;
    uint32_t *currBuffIdx;
    uint32_t *tempBuffIdx;
    uint32_t *sofBuffIdx;
    uint32_t *outBuffIdx;
}Appnf_BuffAddr;

typedef struct
{
    /* FOCO params to be configured */
    uint32_t shiftM1;
    uint32_t dir;
    uint32_t round;
}Appnf_FocoCfgPrms;

typedef struct
{
    uint32_t dataFormat;

    /* Input Frame Parameter */
    uint32_t inWidth;
    uint32_t inHeight;
    uint32_t inStrFmt;
    uint32_t inPitch;

    /* output Frame Parameter */
    uint32_t outWidth;
    uint32_t outHeight;
    uint32_t outStrFmt;
    uint32_t outPitch;

    uint32_t    crc32;

    /* Noise Filter Parameters */
    uint32_t            filterMode;
    uint32_t            centralPixelWeight;
    uint32_t            tableMode;
    uint32_t            skipMode;
    uint32_t            outputShift;
    uint32_t            outputOffset;
    uint32_t            numSubTables;
    uint32_t            subTableIdx;
    uint32_t            isLsePsaAvail;
    uint32_t            psaSign[2];
    uint32_t            isCrcAvail;
    uint64_t            crcSign;
    /* WDTimer configuration */
    /* NF watchdog timer timeout cycles: 0=64K, 1=128K */
    uint32_t            wdTimeoutCycles;
    /* Bitmask of error events (both regular and watchdog timer errors) */
    uint32_t            enableErrorEvents;
} AppNf_TestConfig;

typedef struct
{
    uint32_t                enableBwLimit;
    uint32_t                cycleCnt, tokenCnt;

    Fvid2_Handle            handle;
    /**< FVID2 Driver Handle */

    Vhwa_M2mNfCreatePrms    createPrms;
    /**< FVID2 Create Arguments */

    Vhwa_M2mNfConfig        nfCfg;
    /**< NF Configuration */
    Fvid2_Frame             inFrm;
    /**< Input Frames */
    Fvid2_Frame             outFrm;
    /**< Output Frames */
    Fvid2_CbParams          cbPrms;
    /**< Callback params */
    Fvid2_FrameList         inFrmList;
    /**< Input Frame List */
    Fvid2_FrameList         outFrmList;
    /**< Output Frame List */

    uint32_t                crc;
    /**< CRC for the output should be set for the CRC to be generated */

    VhwaAl_SemaphoreP_Struct       waitForProcessCmpl;
    /**< Semaphore to wait for completion */
    uint32_t                    err_stat;
    /**<Stores error status value*/
    uint32_t                       wdTimerErrStatus;
    /**< Store watchdog timer error interrupt status*/
} AppNf_TestObject;

typedef struct
{
    char                    testName[50];
    /**< Name of the Test */
    uint32_t                isEnableTest;
    /**< Flag to Bypass Test */
    uint32_t                numHandles;
    /**< Max Handles in this Test */
    uint32_t                repeatCnt;
    /**< Number of times to repeat this test */
    uint32_t                isPerformanceTest;
    /**< Flag to indicate if this is performance test */
    AppNf_TestConfig      *testCfg[APP_NF_MAX_HANDLES];
    /**< Pointer to the Test Config */
    Appnf_BuffAddr         buffAddr;
    /**< Buffer address for NF input and output frames */
} NfApp_TestParams;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t AppNf_Init(Udma_DrvHandle udmaDrvHndl);
int32_t AppNf_SetParams(NfApp_TestParams *tObj, uint32_t hidx);

int32_t AppNf_AllocBuffers(NfApp_TestParams *tObj, uint32_t hidx ,
                           uint64_t srcBuf, uint32_t * srcFrameSize,
                           uint64_t dstBuf, uint32_t * dstFrameSize);
int32_t AppNf_SetCoeff(Nf_WgtTableConfig * wgtTbl, uint32_t hidx);

int32_t AppNf_Create(NfApp_TestParams *tObj, uint32_t hidx);
void AppNf_Delete(NfApp_TestParams *tObj, uint32_t hidx);

void AppNf_PrepareRequest(NfApp_TestParams *tObj, uint32_t hidx);
int32_t AppNf_SubmitRequest(NfApp_TestParams *tObj, uint32_t hidx);
int32_t AppNf_WaitForComplRequest(NfApp_TestParams *tObj, uint32_t hidx);

int32_t AppNf_CrcInit(Udma_DrvHandle udmaDrvHndl);
int32_t AppNf_CrcDeinit(Udma_DrvHandle udmaDrvHndl);

void AppNf_SyncStart(NfApp_TestParams *tObj, uint32_t hidx);

#endif /* VPAC_EX_NF_API_H_ */
