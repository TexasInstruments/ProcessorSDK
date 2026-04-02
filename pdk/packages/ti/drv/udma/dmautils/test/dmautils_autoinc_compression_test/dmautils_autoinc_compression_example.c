/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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

/*============================================================================*/
/*============================================================================*/

/**
----------------------------------------------------------------------------
@file    edma_utils_autoinc_compression_example.c
@brief   Demostrates a simple example of auto increment DMA to allow DSP to
operate a function on internal memory and transfer back the result with compression.
@version 0.0 (Jan 2020) : First version
----------------------------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "dmautils_autoinc_compression_example.h"
#include "ti/drv/udma/dmautils/src/dmautils_autoincrement_3d_priv.h"
#include "ti/drv/udma/dmautils/dmautils.h"
#include "ti/drv/udma/udma.h"


int32_t flipHorizontalKernel(
  uint8_t *inputData,
  uint8_t  *outputData,
  uint16_t width,
  uint16_t height,
  uint16_t inPitch,
  uint16_t outPitch)
{
  int32_t i, j;
  for(j = 0; j < height; j++)
  {
    for(i = 0; i < width; i++)
    {
      outputData[i] =  inputData[width - i - 1] ;
    }
    inputData  += inPitch ;
    outputData += outPitch ;
  }
  return 0;
}


int32_t blockCopyKernel(
  uint8_t *inputData,
  uint8_t  *outputData,
  uint16_t width,
  uint16_t height,
  uint16_t inPitch,
  uint16_t outPitch)
{
  int32_t i, j;

  for(j = 0; j < height; j++)
  {
    for(i = 0; i < width; i++)
    {
      outputData[i + (j * outPitch)] =
           inputData[i + (j * inPitch)] ;
    }
  }

  return 0;
}

static void testDmaAutoIncPrintf(const char *str)
{
#ifdef ENABLE_PRINT
  print(str);
#endif
}

#define TEST_DMAUTILS_ALIGN_CEIL(VAL, ALIGN) ((((VAL) + (ALIGN) - 1)/(ALIGN)) * (ALIGN) )

typedef enum{
  DMAUTILSTESTAUTOINC_CHANNEL_IN,
  DMAUTILSTESTAUTOINC_CHANNEL_OUT,
  DMAUTILSTESTAUTOINC_CHANNEL_MAX
}dmautilsTestAutoInc_Channel;

#define DMAUTILSTESTAUTOINC_MAX_NUM_TR  (32)
#define TEST_ALIGN_SIZE (128U)



int32_t tensorCopy( uint8_t* const pInput, 
            uint8_t* const pOutput, 
            uint8_t* const sectr, 
            uint8_t* const cdbTable,
            DmautilsAutoInc_AnalyticCompTest_config *testConfig, 
            uint8_t* pIntMemBase, 
            uint32_t intMemSize, 
            uint32_t compress)
{

  int32_t retVal = UDMA_SOK ;

  uint32_t intMemUsedSize = 0;
  uint8_t *dmautilsContext;
  uint8_t *TrMem;
  //uint8_t *tmp;
  uint32_t TrSize;
  uint32_t numTrReq;
  uint32_t dmaChannels;

  DmaUtilsAutoInc3d_InitParam        initParams;
  DmaUtilsAutoInc3d_ChannelInitParam chInitParams[DMAUTILSTESTAUTOINC_CHANNEL_MAX];
  DmaUtilsAutoInc3d_TrPrepareParam   trPrepParam;
  DmaUtilsAutoInc3d_TransferProp    *transferProp;


  Udma_InitPrms           initPrms;
  struct Udma_DrvObj      udmaDrvObj;
  uint32_t                instId;

  Udma_DrvHandle  drvHandle = &udmaDrvObj;

  instId = UDMA_INST_ID_MAIN_0;
  UdmaInitPrms_init(instId, &initPrms);
  initPrms.printFxn = &testDmaAutoIncPrintf;
  initPrms.skipGlobalEventReg = 1; //TO make it run on AVV  with this value set master event handle would not be created. 
    initPrms.printFxn = &testDmaAutoIncPrintf;
    retVal = Udma_init(drvHandle, &initPrms);
  if(UDMA_SOK != retVal)
  {
      testDmaAutoIncPrintf("[Error] UDMA init failed!!\n");
  }


  //dmaChannels = DMAUTILSTESTAUTOINC_CHANNEL_MAX; /* One for input and other for output */
  dmaChannels = 1;
  //Allocation/Assignment of buffers in internal memory
  dmautilsContext =  pIntMemBase + intMemUsedSize ;
  intMemUsedSize += TEST_DMAUTILS_ALIGN_CEIL(DmaUtilsAutoInc3d_getContextSize(dmaChannels), TEST_ALIGN_SIZE);

  transferProp    = (DmaUtilsAutoInc3d_TransferProp * ) (pIntMemBase + intMemUsedSize );
  intMemUsedSize += TEST_DMAUTILS_ALIGN_CEIL((sizeof(DmaUtilsAutoInc3d_TransferProp)), TEST_ALIGN_SIZE);

  initParams.contextSize = DmaUtilsAutoInc3d_getContextSize(dmaChannels);
  initParams.numChannels = dmaChannels;
  initParams.traceLogLevel    = 1;
  initParams.udmaDrvHandle = drvHandle;
  initParams.DmaUtilsVprintf = vprintf;

  numTrReq = 1; // one TR per compression

  chInitParams[DMAUTILSTESTAUTOINC_CHANNEL_IN].dmaQueNo  = 0;

  
  chInitParams[DMAUTILSTESTAUTOINC_CHANNEL_IN ].druOwner    = DMAUTILSAUTOINC3D_DRUOWNER_DIRECT_TR;
  retVal = DmaUtilsAutoInc3d_init(dmautilsContext, &initParams, chInitParams);
  
  const int32_t CIBLENGTH = 8;
  int32_t icnt0, icnt1, icnt2, icnt3;
  int32_t sdim0, sdim1, sdim2, sdim3;
  int32_t dicnt0, dicnt1, dicnt2, dicnt3;
  int32_t ddim0, ddim1, ddim2, ddim3;
  int32_t numSBinTensorWidth = testConfig->tensorWidth / testConfig->sbWidth;  
  int32_t triggers;
  int32_t i;
  
  if ( retVal != UDMA_SOK )
  {
    goto Exit;
  }

  TrSize = DmaUtilsAutoInc3d_getTrMemReq(numTrReq);

  TrMem = pIntMemBase + intMemUsedSize ;

  intMemUsedSize += TEST_DMAUTILS_ALIGN_CEIL(TrSize, TEST_ALIGN_SIZE);

  trPrepParam.channelId = DMAUTILSTESTAUTOINC_CHANNEL_IN;
  trPrepParam.numTRs    = numTrReq;
  trPrepParam.trMem     = TrMem;
  trPrepParam.trMemSize = TrSize;
  //setup requrest
  transferProp[0].syncType               = DMAUTILSAUTOINC3D_SYNC_2D;

  transferProp[0].dmaDfmt                = (compress == 1) ? DMAUTILSAUTOINC3D_DFMT_COMP : DMAUTILSAUTOINC3D_DFMT_DECOMP;
  
  if(compress ==  1)
  {
    
    if(testConfig->outDataFlowType == DMAUTILSAUTOINC_ROW_FLOW)
    { 
      icnt0 = testConfig->outProcSize / testConfig->sbWidth;
      sdim0 = testConfig->sbWidth;
      dicnt0 = icnt0;
      ddim0 = CIBLENGTH;
      
      icnt1 = testConfig->tensorHeight / testConfig->sbHeight;
      sdim1 = testConfig->tensorWidth * testConfig->sbHeight;
      dicnt1 =  icnt1;
      ddim1 = numSBinTensorWidth * CIBLENGTH;
      
      if(testConfig->tensorWidth > testConfig->outProcSize)
      {
        icnt2 = 2;
        sdim2 = testConfig->outProcSize;
        if(testConfig->tensorWidth % testConfig->outProcSize == 0)
        {
          dicnt2 = testConfig->tensorWidth / testConfig->outProcSize;
        }
        else
        {
          dicnt2 = testConfig->tensorWidth / testConfig->outProcSize + 1;
        }
        ddim2 = icnt0 * CIBLENGTH;
        
        if (testConfig->tensorWidth % (2*  testConfig->outProcSize) == 0)
        {
          icnt3 = testConfig->tensorWidth / (2*  testConfig->outProcSize);
        }
        else
        {
          icnt3 = testConfig->tensorWidth / (2*  testConfig->outProcSize) + 1;
        }
        sdim3 = 2 * testConfig->outProcSize;   
        
        dicnt3 =  1;
        ddim3 = 0;
      }   
    } 
    else
    {
      if(testConfig->tensorWidth == testConfig->outProcSize)
      {
        icnt0 = testConfig->tensorWidth / testConfig->sbWidth;
        sdim0 = testConfig->sbWidth;
        dicnt0 = icnt0;
        ddim0 = CIBLENGTH;  
        
        icnt1 = testConfig->Nco / testConfig->sbHeight;
        sdim1 = testConfig->tensorWidth * testConfig->sbHeight;
        dicnt1 =  icnt1;
        ddim1 = numSBinTensorWidth * CIBLENGTH;

        icnt2 = 2;
        sdim2 = testConfig->tensorWidth * testConfig->Nco;
        if(testConfig->tensorHeight % testConfig->Nco == 0)
        {
          dicnt2 = testConfig->tensorHeight / testConfig->Nco;
        }
        else
        {
          dicnt2 = testConfig->tensorHeight / testConfig->Nco + 1;
        }
        ddim2 = numSBinTensorWidth * icnt1 * CIBLENGTH;

        if (testConfig->tensorHeight % (2*  testConfig->Nco) == 0)
        {
          icnt3 = testConfig->tensorHeight / (2*  testConfig->Nco);
        }
        else
        {
          icnt3 = testConfig->tensorHeight / (2*  testConfig->Nco) + 1;
        }
        sdim3 = 2 * testConfig->tensorWidth * testConfig->Nco;   
        
        dicnt3 =  1;
        ddim3 = 0;
      }  
    }
  }
  else
  {
    if(testConfig->inDataFlowType == DMAUTILSAUTOINC_ROW_FLOW)
    { 
      icnt0 = testConfig->inProcSize / testConfig->sbWidth;
      sdim0 = CIBLENGTH;
      dicnt0 = icnt0;
      ddim0 = testConfig->sbWidth;
      
      icnt1 = testConfig->tensorHeight / testConfig->sbHeight;
      sdim1 = numSBinTensorWidth * CIBLENGTH;
      dicnt1 =  icnt1;
      ddim1 = testConfig->tensorWidth * testConfig->sbHeight;
      
      if(testConfig->tensorWidth > testConfig->inProcSize)
      {
        if (testConfig->tensorWidth % ( testConfig->inProcSize) == 0)
        {
          icnt2 = testConfig->tensorWidth / testConfig->inProcSize;
        }
        else
        {
          icnt2 = testConfig->tensorWidth / testConfig->inProcSize + 1;
        }
        sdim2 = icnt0 * CIBLENGTH;
        dicnt2 =  2;
        ddim2 = testConfig->inProcSize; 


        icnt3 = 1;
        sdim3 = 0; 
        if (testConfig->tensorWidth % (2*  testConfig->inProcSize) == 0)
        {
          dicnt3 = testConfig->tensorWidth / (2*  testConfig->inProcSize);
        }
        else
        {
          dicnt3 = testConfig->tensorWidth / (2*  testConfig->inProcSize) + 1;
        }
        ddim3 = 2 * testConfig->inProcSize;
      }  

    } 
    else
    {
      if(testConfig->tensorWidth == testConfig->inProcSize)
      {
        icnt0 = testConfig->tensorWidth / testConfig->sbWidth;
        sdim0 = CIBLENGTH;
        dicnt0 = icnt0;
        ddim0 = testConfig->sbWidth;  
        
        icnt1 = testConfig->Nci / testConfig->sbHeight;
        sdim1 = numSBinTensorWidth * CIBLENGTH; 
        dicnt1 =  icnt1;
        ddim1 = testConfig->tensorWidth * testConfig->sbHeight;
        
        if(testConfig->tensorHeight % testConfig->Nci == 0)
        {
          icnt2 = testConfig->tensorHeight / testConfig->Nci;
        }
        else
        {
          icnt2 = testConfig->tensorHeight / testConfig->Nci + 1;
        }
        sdim2 = numSBinTensorWidth * icnt1 * CIBLENGTH;
        icnt3 = 1;
        sdim3 = 0;   
        dicnt2 =  2;
        ddim2 =  testConfig->tensorWidth * testConfig->Nci;

        
        if (testConfig->tensorHeight % (2*  testConfig->Nci) == 0)
        {
          dicnt3 = testConfig->tensorHeight / (2*  testConfig->Nci);
        }
        else
        {
          dicnt3 = testConfig->tensorHeight / (2*  testConfig->Nci) + 1;
        }
        ddim3 = 2 * testConfig->tensorWidth * testConfig->Nci;
      }  
    }
  }

  triggers = ((icnt2 * icnt3) < (dicnt2 * dicnt3)) ? (icnt2 * icnt3) : (dicnt2 * dicnt3); 

  transferProp[0].transferDim.sicnt0     = icnt0; 
  transferProp[0].transferDim.sicnt1     = icnt1; 
  transferProp[0].transferDim.sicnt2     = icnt2; 
  transferProp[0].transferDim.sicnt3     = icnt3;

  transferProp[0].transferDim.dicnt0     = dicnt0;
  transferProp[0].transferDim.dicnt1     = dicnt1;
  transferProp[0].transferDim.dicnt2     = dicnt2;
  transferProp[0].transferDim.dicnt3     = dicnt3;

  transferProp[0].circProp.circSize1     = 0;
  transferProp[0].circProp.circSize2     = 0;
  transferProp[0].circProp.addrModeIcnt0 = 0;
  transferProp[0].circProp.addrModeIcnt1 = 0;
  transferProp[0].circProp.addrModeIcnt2 = 0;
  transferProp[0].circProp.addrModeIcnt3 = 0;
  transferProp[0].circProp.circDir       = 0;


  transferProp[0].cmpProp.cmpAlg         = testConfig->algorithm;
  transferProp[0].cmpProp.cmpBias        = testConfig->bias;
  transferProp[0].cmpProp.varKStartK     = 0; //generic not supported
  transferProp[0].cmpProp.varKUpdateK    = 0; //generic not supported
  transferProp[0].cmpProp.varKElemSize   = 0; //generic not supported
  transferProp[0].cmpProp.varKSubType    = 0; //generic not supported
  transferProp[0].cmpProp.varKSubSel0    = 0; //generic not supported
  transferProp[0].cmpProp.varKSubSel1    = 0; //generic not supported
  transferProp[0].cmpProp.varKSubSel2    = 0; //generic not supported
  transferProp[0].cmpProp.varKSubSel3    = 0; //generic not supported
  transferProp[0].cmpProp.sbIcnt0        = testConfig->sbWidth / 16;
  transferProp[0].cmpProp.sbIcnt1        = testConfig->sbHeight; //2 rows per superblock
  transferProp[0].cmpProp.sbDim1         = testConfig->tensorWidth;
  transferProp[0].cmpProp.sbAM0          = 0;
  transferProp[0].cmpProp.sbAM1          = 0;
  
  transferProp[0].transferDim.sdim1      = sdim1;
  transferProp[0].transferDim.sdim2      = sdim2; 
  transferProp[0].transferDim.sdim3      = sdim3;
  
  transferProp[0].transferDim.ddim1      = ddim1; // table dims
  transferProp[0].transferDim.ddim2      = ddim2;
  transferProp[0].transferDim.ddim3      = ddim3;
  
  transferProp[0].cmpProp.sDim0          = sdim0; 
  transferProp[0].cmpProp.dDim0          = ddim0; //2 rows per superblock
  

  transferProp[0].ioPointers.srcPtr      = pInput;
  transferProp[0].ioPointers.dstPtr      = pOutput;
  transferProp[0].ioPointers.strPtr      = sectr;
  transferProp[0].ioPointers.cdbPtr      = cdbTable;

  
  retVal = DmaUtilsAutoInc3d_prepareTr(&trPrepParam, &transferProp[0]);
  if ( retVal != UDMA_SOK )
  {
    goto Exit;
  }
  
  if(intMemUsedSize > intMemSize)
  {
    printf("insufficient memory, required is %d vs provided %d\n",intMemUsedSize, intMemSize);
    return -1 ;
  }

  //tmp = TrMem;
  //for (i = 0; i <= 63; i++ )
  //{
  //  printf("%d:         0x%X\n", i, *(tmp+i));
  //}

  retVal = DmaUtilsAutoInc3d_configure(dmautilsContext, DMAUTILSTESTAUTOINC_CHANNEL_IN, TrMem, numTrReq);
  

  if ( retVal != UDMA_SOK )
  {
    goto Exit;
  }
  
  for (i = 0; i < triggers; i++ )
  {
  	
    //DMA trigger for pipe-up, out transfer is dummy and handled inside DMA utility
    DmaUtilsAutoInc3d_trigger(dmautilsContext, DMAUTILSTESTAUTOINC_CHANNEL_IN);
    
	//Wait for previous transfer of in
    DmaUtilsAutoInc3d_wait(dmautilsContext, DMAUTILSTESTAUTOINC_CHANNEL_IN);
  }

  retVal = DmaUtilsAutoInc3d_deconfigure(dmautilsContext, DMAUTILSTESTAUTOINC_CHANNEL_IN, TrMem, 1);

  if ( retVal != UDMA_SOK )
  {
    goto Exit;
  }

  retVal = DmaUtilsAutoInc3d_deinit(dmautilsContext);

  if ( retVal != UDMA_SOK )
  {
    goto Exit;
  }


  retVal = Udma_deinit(drvHandle);
  if ( retVal != UDMA_SOK )
  {
    goto Exit;
  }

  Exit:
  {
      return retVal ;
  }

}


