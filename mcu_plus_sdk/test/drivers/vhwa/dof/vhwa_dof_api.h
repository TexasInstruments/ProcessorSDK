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
*  \file vhwa_dof_api.h
*
*  \brief VHWA DOF API and struct definitions
*/
#ifndef VPAC_EX_DOF_API_H_
#define VPAC_EX_DOF_API_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <drivers/vhwa/include/vhwa_m2mDof.h>

//#include vhwa_examples_common.h
//#include <ti/drv/vhwa/examples/include/vhwa_common_crc.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*
* Application test parameters
*/
#define APP_DOF_MAX_HANDLES                  (4U)

/* Default Initialization paraments for Memory allocatio */
#define APP_DOF_DEFAULT_IMG_WIDTH       (2048U)
#define APP_DOF_DEFAULT_CCSF            (FVID2_CCSF_BITS12_UNPACKED16)
#define APP_DOF_DEFAULT_TOP_SR          (48U)
#define APP_DOF_DEFAULT_BOT_SR          (48U)

#define APP_DOF_CRC_CHANNEL         (CRC_CHANNEL_1)

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
}AppDof_BuffAddr;

typedef struct
{
    /* FOCO params to be configured */
    uint32_t shiftM1;
    uint32_t dir;
    uint32_t round;
}AppDof_FocoCfgPrms;

typedef struct
{
    uint32_t skipTempPredLine;

    /* Total Pyramid Levels */
    uint32_t tPyrLvl;
    /* Input Frame Parameter */
    uint32_t strFmt[DOF_MAX_PYR_LVL_SUPPORTED];
    /* array of pitch starting from the base frame
       index 0 is level 0 (base frame)
       index 1 is level 1 so on */
    uint32_t curPitch[DOF_MAX_PYR_LVL_SUPPORTED];
    uint32_t refPitch[DOF_MAX_PYR_LVL_SUPPORTED];
    uint32_t temPredPitch;
    uint32_t sofPitch;
    uint32_t fvHeight;
    /* This parameters represents the total hieght of output data
       when SOF is enabled. If a output for pixel is enabled in a paxel
       then output height for that paxel should be added otherwise ignoured */

    /* output buffer Parameter */
    uint32_t outPitch;


    /* DOF Parameters */
    uint32_t            enableDOF;
    uint32_t            width;
    uint32_t            height;
    uint32_t            HorizontalSearchRange;
    uint32_t            TopSearchRange;
    uint32_t            BottomSearchRange;
    /* Top layer predictor */
    uint32_t            tPredictor;
    /* Middle layer predictors */
    uint32_t            mPredictor1;
    uint32_t            mPredictor2;
    /* BAse layer predictors */
    uint32_t            bPredictor1;
    uint32_t            bPredictor2;
    uint32_t            LkConfidanceScore;
    uint32_t            medianFilter;
    uint32_t            enableSof;
    uint32_t            currentCensusTransform;
    uint32_t            referenceCensusTransform;
    uint32_t            maxMVsInSof;
    uint32_t            motionSmoothnessFactor;
    uint32_t            iirFilterAlpha;
    uint32_t            isPsa;
    uint32_t            isHistogram;
    uint32_t            isCrcAvail;
    uint64_t            crcSign;

     /* valid only when the Image storage format is not 12 bit packed */
    AppDof_FocoCfgPrms  focoCfg;
} AppDof_TestConfig;

typedef struct
{
    uint32_t                enableBwLimit;
    uint32_t                cycleCnt, tokenCnt;

    Fvid2_Handle            handle;
    /**< FVID2 Driver Handle */

    Vhwa_M2mDofCreateArgs   createArgs;
    /**< FVID2 Create Arguments */

    Vhwa_M2mDofPrms         dofPrms;
    /**< DOF Configuration */
    Fvid2_Frame             inFrm[DOF_MAX_PYR_LVL_SUPPORTED]
                                 [VHWA_M2M_DOF_MAX_IN_BUFFER];
    /**< Input Frames */
    Fvid2_Frame             outFrm[DOF_MAX_PYR_LVL_SUPPORTED];
    /**< Output Frames */
    Fvid2_CbParams          cbPrms;
    /**< Callback params */
    Fvid2_FrameList         inFrmList[DOF_MAX_PYR_LVL_SUPPORTED];
    /**< Input Frame List */
    Fvid2_FrameList         outFrmList[DOF_MAX_PYR_LVL_SUPPORTED];
    /**< Output Frame List */

    uint32_t                crc;
    /**< CRC for the out flow vector enableCrc in Dof_Config should be
         set for the CRC to be generated */
    uint32_t                csHistogram[16];
    /**< CS histraom value will be filled for base frame only when
         enableCsHistogram in Vhwa_m2mDofConfig is set */

    uint32_t                curPyrLvl;
    /**< current pyramid handle being processed */

    SemaphoreP_Object       waitForProcessCmpl;
    /**< Semaphore to wait for completion */
    uint32_t                    errStat;
    /**<Stores error status value*/
    uint32_t                       wdTimerErrStatus;
    /**< Store watchdog timer error interrupt status*/
} AppDof_TestObject;

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
    AppDof_TestConfig      *testCfg[APP_DOF_MAX_HANDLES];
    /**< Pointer to the Test Config */
    Dof_ConfScoreParam     dofConfScore[APP_DOF_MAX_HANDLES];
    /**< Pointer to the Confidence score paramters */
    AppDof_BuffAddr         buffAddr;
    /**< Buffer address for DOF input and output frames */
} DofApp_TestParams;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
int32_t AppDof_Init(Udma_DrvHandle udmaDrvHndl);
int32_t AppDof_SetParams(DofApp_TestParams *tObj, uint32_t hidx);
int32_t AppDof_SetConfScoreParam(DofApp_TestParams *tObj, uint32_t hidx);

int32_t AppDof_AllocBuffers(DofApp_TestParams *tObj, uint32_t hidx,
            AppDof_BuffAddr *buffAddr);

int32_t AppDof_Create(DofApp_TestParams *tObj, uint32_t hidx);
void AppDof_Delete(DofApp_TestParams *tObj, uint32_t hidx);

void AppDof_PrepareRequest(DofApp_TestParams *tObj, uint32_t hidx);
int32_t AppDof_SubmitRequest(DofApp_TestParams *tObj, uint32_t hidx);
int32_t AppDof_WaitForComplRequest(DofApp_TestParams *tObj, uint32_t hidx);

int32_t AppDof_CrcInit(Udma_DrvHandle udmaDrvHndl);
int32_t AppDof_CrcDeinit(Udma_DrvHandle udmaDrvHndl);

void AppDof_SyncStart(DofApp_TestParams *tObj, uint32_t hidx);
void AppDofMain(void *args);
#endif /*VPAC_EX_DOF_API_H_*/
