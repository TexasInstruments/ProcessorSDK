/*
 *  Copyright (c) Texas Instruments Incorporated 2022-2023
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
 *  \file dmautils_autoinc_compression_test.c
 *
 *  \brief Simple application demonstrating 2D auto increment feature of dmautils along with handling
 *        of last block.
 *
 *  Requirement: DOX_REQ_TAG(PDK-2643:PDK-2649:PDK-2646)
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#if defined(HOST_EMULATION)
#include <malloc.h>
#else
#if !defined (MCU_PLUS_SDK)
#include <ti/csl/csl_clec.h>
#include <ti/csl/arch/c7x/cslr_C7X_CPU.h>
#else
#include <drivers/hw_include/csl_clec.h>
#endif
#endif

#include "dmautils_autoinc_compression_example.h"
#if !defined (MCU_PLUS_SDK)
#include "ti/drv/sciclient/sciclient.h"
#else
#include <drivers/sciclient.h>
#endif

#define TEST_malloc(heap, size)   malloc(size)
#define TEST_free(ptr)            free(ptr)
#define DRU_LOCAL_EVENT_START_DEFAULT  (192U)   // Default for J721E and J721S2
#define DRU_LOCAL_EVENT_START_J784S4   (664U)

#define ALIGN(ptr) (uint8_t*)((((uint64_t)(ptr)+127)/128)*128)

#if HOST_EMULATION
  #define L2SRAM_SIZE (512*1024)
#else
  #define L2SRAM_SIZE (64*1024)
#endif
#ifdef _MSC_VER
#ifndef __attribute__
#define __attribute__()
#endif
#endif
uint8_t L2SRAM[L2SRAM_SIZE] __attribute__((aligned(128)));

#if !defined(SOC_AM62A) && !defined(SOC_AM62DX)
#if !defined(HOST_EMULATION)
/*Configure CLEC*/
static void appC7xClecInitDru(void)
{
    CSL_ClecEventConfig   cfgClec;
    #if defined(SOC_AM62A) && !defined(SOC_AM62DX)
    CSL_CLEC_EVTRegs   *clecBaseAddr = (CSL_CLEC_EVTRegs*) CSL_C7X256V0_CLEC_BASE;
    #else
    CSL_CLEC_EVTRegs   *clecBaseAddr = (CSL_CLEC_EVTRegs*) CSL_COMPUTE_CLUSTER0_CLEC_REGS_BASE;
    #endif

    uint32_t i;
    uint32_t dru_input_start = 192;
    #if defined(SOC_J784S4)
    dru_input_start = DRU_LOCAL_EVENT_START_J784S4;
    #else
    dru_input_start = DRU_LOCAL_EVENT_START_DEFAULT;
    #endif
    uint32_t dru_input_num   = 16;
    /*Only configuring 16 channels*/
    for(i=dru_input_start; i<(dru_input_start+dru_input_num); i++)
    {
        /* Configure CLEC */
        cfgClec.secureClaimEnable = FALSE;
        cfgClec.evtSendEnable     = TRUE;

        /* cfgClec.rtMap value is different for each C7x */
        cfgClec.rtMap             = CSL_CLEC_RTMAP_CPU_4;

        cfgClec.extEvtNum         = 0;
        cfgClec.c7xEvtNum         = (i-dru_input_start)+32;
        CSL_clecConfigEvent(clecBaseAddr, i, &cfgClec);
    }
}
#endif
#endif

typedef struct
{
  uint32_t testcaseId;
  uint32_t requirementId;
  uint32_t imageWidth;
  uint32_t imageHeight;
  uint8_t  algorithm;
  uint8_t  bias;
}dmautilsAutoIncTest_config;


dmautilsAutoIncTest_config gTestConfig[] =
{
    {
        0,
        1,
        40,/*Image Width */
        32,/*Image Height */
        0,/*algorithm=zero*/
        5/*bias*/
    },
    {
        1,
        1,
        48,/*Image Width */
        32,/*Image Height */
        1,/*algorithm=ueg*/
        72/*bias*/
    },
    {
        2,
        1,
        64,/*Image Width */
        48,/*Image Height */
        2,/*algorithm=seg*/
        0/*bias*/
    },
    {
        3,
        1,
        128,/*Image Width */
        48,/*Image Height */
        2,/*algorithm=seg*/
        23/*bias*/
    }

};


DmautilsAutoInc_AnalyticCompTest_config gAnalyticCompTestConfig[] =
{
   {
     0, /*testcaseId*/
     0, /*requirementId*/
     32, /*featurePlaneWidth*/
     32, /*featurePlaneHeight*/
     1024, /*tensorWidth*/
     128, /*tensorHeight*/
     128, /*outProcSize*/
     1024, /*inProcSize*/
     128, /*Nco*/
     16, /*Nci*/
     DMAUTILSAUTOINC_ROW_FLOW, /*outDataFlowType*/
     DMAUTILSAUTOINC_COLUMN_FLOW, /*inDataFlowType*/
     0, /*circularity*/
     128, /*sbWidth*/
     8, /*sbHeight*/
     0, /*algorithm*/
     0, /*bias*/
   },

   {
     1, /*testcaseId*/
     0, /*requirementId*/
     32, /*featurePlaneWidth*/
     32, /*featurePlaneHeight*/
     1024, /*tensorWidth*/
     128, /*tensorHeight*/
     1024, /*outProcSize*/
     128, /*inProcSize*/
     16, /*Nco*/
     128, /*Nci*/
     DMAUTILSAUTOINC_COLUMN_FLOW, /*outDataFlowType*/
     DMAUTILSAUTOINC_ROW_FLOW, /*inDataFlowType*/
     0, /*circularity*/
     128, /*sbWidth*/
     8, /*sbHeight*/
     1, /*algorithm*/
     0, /*bias*/
   },

   {
     2, /*testcaseId*/
     0, /*requirementId*/
     32, /*featurePlaneWidth*/
     32, /*featurePlaneHeight*/
     1024, /*tensorWidth*/
     128, /*tensorHeight*/
     128, /*outProcSize*/
     64, /*inProcSize*/
     128, /*Nco*/
     128, /*Nci*/
     DMAUTILSAUTOINC_ROW_FLOW, /*outDataFlowType*/
     DMAUTILSAUTOINC_ROW_FLOW, /*inDataFlowType*/
     0, /*circularity*/
     64, /*sbWidth*/
     128, /*sbHeight*/
     1, /*algorithm*/
     0, /*bias*/
   },
   {
     3, /*testcaseId*/
     0, /*requirementId*/
     32, /*featurePlaneWidth*/
     32, /*featurePlaneHeight*/
     1024, /*tensorWidth*/
     128, /*tensorHeight*/
     1024, /*outProcSize*/
     1024, /*inProcSize*/
     16, /*Nco*/
     8, /*Nci*/
     DMAUTILSAUTOINC_COLUMN_FLOW, /*outDataFlowType*/
     DMAUTILSAUTOINC_COLUMN_FLOW, /*inDataFlowType*/
     0, /*circularity*/
     1024, /*sbWidth*/
     8, /*sbHeight*/
     2, /*algorithm*/
     0, /*bias*/
   },

   {
     4, /*testcaseId*/
     0, /*requirementId*/
     32, /*featurePlaneWidth*/
     32, /*featurePlaneHeight*/
     1024, /*tensorWidth*/
     128, /*tensorHeight*/
     1024, /*outProcSize*/
     1024, /*inProcSize*/
     1, /*Nco*/
     1, /*Nci*/
     DMAUTILSAUTOINC_COLUMN_FLOW, /*outDataFlowType*/
     DMAUTILSAUTOINC_COLUMN_FLOW, /*inDataFlowType*/
     0, /*circularity*/
     1024, /*sbWidth*/
     1, /*sbHeight*/
     2, /*algorithm*/
     0, /*bias*/
   },

   {
     0, /*testcaseId*/
     0, /*requirementId*/
     32, /*featurePlaneWidth*/
     32, /*featurePlaneHeight*/
     1024, /*tensorWidth*/
     128, /*tensorHeight*/
     128, /*outProcSize*/
     1024, /*inProcSize*/
     128, /*Nco*/
     16, /*Nci*/
     DMAUTILSAUTOINC_ROW_FLOW, /*outDataFlowType*/
     DMAUTILSAUTOINC_COLUMN_FLOW, /*inDataFlowType*/
     0, /*circularity*/
     128, /*sbWidth*/
     8, /*sbHeight*/
     0, /*algorithm*/
     39, /*bias*/
   },

};

uint32_t randomNumberGeneratorState = 258001;
uint8_t getRandomNumberLCG() {			//Linear congruential generator
  uint32_t    returnValue;
  uint32_t    largestValue = 16;
  uint32_t    randomNumberGeneratorStateLoc;
  randomNumberGeneratorStateLoc = ((randomNumberGeneratorState * (uint32_t)48271) + (uint32_t)0) >> 0x5U;	// c++11 minstd_rand
  returnValue = randomNumberGeneratorStateLoc % largestValue;
  randomNumberGeneratorState = randomNumberGeneratorStateLoc;
  return ((uint8_t )returnValue);
}


void fillBufferRandom(uint8_t* buffer, int32_t width, int32_t height, int32_t distribution)
{
  int32_t i,j, tmp1;
  for ( j = 0 ; j < height; j++)
  {
    tmp1 = j * width;
    for (i = 0; i < width; i++)
    {
      buffer[i + tmp1] = getRandomNumberLCG();
    }
  }
  return;
}

int32_t compareBuffers(uint8_t* buffer1, uint8_t* buffer2, int32_t width, int32_t height)
{
  int32_t i,j, tmp1,  tmp2;
  int32_t fail = -1;
  int32_t firstmismatchFound = 0;
  for ( j = 0 ; j < height; j++)
  {
    tmp1 = j * width;

    for (i = 0; i < width; i++)
    {
      tmp2 = tmp1 + i;
      if (*(buffer1 + tmp2) != *(buffer2 + tmp2) && firstmismatchFound == 0)
      {
        fail = tmp2;
	firstmismatchFound = 1;
	return fail;
      }

    }
  }
  return fail;
}

#if !defined(SOC_AM62A) && !defined(SOC_AM62DX)
int32_t test_sciclientDmscGetVersion(char *version_str, uint32_t version_str_size)
{
    int32_t retVal = 0;

    struct tisci_msg_version_req request;
    const Sciclient_ReqPrm_t      reqPrm =
    {
        TISCI_MSG_VERSION,
        TISCI_MSG_FLAG_AOP,
        (uint8_t *) &request,
        sizeof(request),
        SCICLIENT_SERVICE_WAIT_FOREVER
    };
    struct tisci_msg_version_resp response;
    Sciclient_RespPrm_t           respPrm =
    {
        0,
        (uint8_t *) &response,
        sizeof (response)
    };

    retVal = Sciclient_service(&reqPrm, &respPrm);
    if (0 == retVal)
    {
        if (respPrm.flags == TISCI_MSG_FLAG_ACK)
        {
            if(version_str == NULL)
            {
                printf("SCICLIENT: DMSC FW version [%s]\n", (char *) response.str);
                printf("SCICLIENT: DMSC FW revision 0x%x  \n", response.version);
                printf("SCICLIENT: DMSC FW ABI revision %d.%d\n",
                    response.abi_major, response.abi_minor);
            }
            else
            {
                snprintf(version_str, version_str_size, "version %s, revision 0x%x, ABI %d.%d",
                    (char *) response.str,
                    response.version,
                    response.abi_major, response.abi_minor
                    );
            }
        }
        else
        {
            retVal = -1;
        }
    }
    if(retVal!=0)
    {
        printf("SCICLIENT: ERROR: DMSC Firmware Get Version failed !!!\n");
    }

    return (retVal);
}
#endif

int32_t main()
{
  uint16_t   width;
  uint16_t   height;
  //uint8_t    algorithm;
  //uint8_t    bias;

  //int32_t i, j;
  uint8_t *input     = NULL;
  uint8_t *compressed = NULL;
  uint8_t *sectr      = NULL;
  uint8_t  *output    = NULL;
  uint8_t  *refOut    = NULL;

  uint8_t*    pIntMmeBase  = L2SRAM;
  uint32_t   intMemSize   = L2SRAM_SIZE;

  //uint8_t    useDMA      ;
  uint8_t    compress;

  int32_t status = -1;
  uint32_t testcaseIdx;

#ifdef HOST_EMULATION
#if defined(_MSC_VER)
    pIntMmeBase = (uint8_t*)_aligned_malloc(L2SRAM_SIZE, L2SRAM_SIZE);
#else
    pIntMmeBase = (uint8_t*)memalign(L2SRAM_SIZE, L2SRAM_SIZE);
#endif
#else

#if !defined(SOC_AM62A) && !defined(SOC_AM62DX)
    int32_t retVal = 0;

    Sciclient_ConfigPrms_t  sciClientCfg;
    Sciclient_configPrmsInit(&sciClientCfg);
    retVal = Sciclient_init(&sciClientCfg);
    if(retVal!=0)
    {
      printf("Sciclient Init Failed \n");
      goto Exit;
    }
    test_sciclientDmscGetVersion(NULL, 0 );
    appC7xClecInitDru();
#endif
#endif

  for (testcaseIdx = 0; testcaseIdx < sizeof(gAnalyticCompTestConfig)/ sizeof(DmautilsAutoInc_AnalyticCompTest_config); testcaseIdx++)
  //for (testcaseIdx = 0; testcaseIdx < 1; testcaseIdx++)
  {
      width    = gAnalyticCompTestConfig[testcaseIdx].tensorWidth;
      height   = gAnalyticCompTestConfig[testcaseIdx].tensorHeight;
      printf("width  : %d \n", width);
      printf("height : %d \n", height);

      int32_t tensorSize = ceil((float)(width * height) / (float)(gAnalyticCompTestConfig[testcaseIdx].sbWidth)) * gAnalyticCompTestConfig[testcaseIdx].sbWidth;
      int32_t CDBTableWidth = ceil((float)(width) / (float)(gAnalyticCompTestConfig[testcaseIdx].sbWidth));
      int32_t CDBTableHeight = height / gAnalyticCompTestConfig[testcaseIdx].sbHeight;
      int32_t sectrSize = CDBTableWidth * CDBTableHeight * 8 + 64;

      //algorithm    = gAnalyticCompTestConfig[testcaseIdx].algorithm;
      //bias         = gAnalyticCompTestConfig[testcaseIdx].bias;

      /* Buffer allocations for input, output and reference output  */
      #if(HOST_EMULATION)

        input      = (uint8_t *)malloc(width * height);
        output     = (uint8_t *)malloc(width * height);
        compressed = (uint8_t *)malloc(width * height * 3);
        sectr      = (uint8_t *)malloc(width * height + 64); // need two headers since compression and decompression have different fields and they will be interleaved...
        refOut     = input;
      #else
        //Malloc alignment to 128
        sectr      = (uint8_t *)malloc(width * height + 64 + 128); // need two headers since compression and decompression have different fields and they will be interleaved...
        input      = (uint8_t *)malloc(width * height + 128);
        output     = (uint8_t *)malloc(width * height + 128);
        compressed = (uint8_t *)malloc(width * height * 3 + 128);
        //Alignment to 128
        sectr   = ALIGN(sectr);
        input   = ALIGN(input);
        output  = ALIGN(output);
        compressed  = ALIGN(compressed);
        refOut     = input;

      #endif
      memset(output,     0, tensorSize);
      memset(compressed, 0, tensorSize * 3);
      memset(sectr,      0, sectrSize);

      fillBufferRandom(input, width, height, 0);
      printf("Tensor Filled\n");
      compress = 1;

#if (!HOST_EMULATION) && (CORE_DSP)
      tscStart = _TSC_read();
#endif

      tensorCopy( input,
                  compressed,
                  sectr, // leave room for decompression header
                  sectr + 32, // first CDB table entry is 32B after header
                  &gAnalyticCompTestConfig[testcaseIdx],
                  pIntMmeBase,
                  intMemSize,
                  compress);

#if (!HOST_EMULATION) && (CORE_DSP)
      tscEnd = _TSC_read();
      printf("Cycles - Compression = %llu\n",(tscEnd-tscStart));
#endif
      printf("Tensor Compressed\n");
      compress = 0;
#if (!HOST_EMULATION) && (CORE_DSP)
      tscStart = _TSC_read();
#endif
      // decompress data
      tensorCopy( compressed,
                  output,
                  sectr, // leave room for decompression header
                  sectr + 32, // first CDB table entry is 32B after header
                  &gAnalyticCompTestConfig[testcaseIdx],
                  pIntMmeBase,
                  intMemSize,
                  compress);

#if (!HOST_EMULATION) && (CORE_DSP)
      tscEnd = _TSC_read();
      printf("Cycles - De-compression = %llu\n",(tscEnd-tscStart));
#endif
      printf("Tensor Un-compressed\n");

      status = compareBuffers(refOut, output,  width, height);
      if(status == -1)
      {
        printf("DMAUtils Compression TestCase %d,        PASSED \n", gAnalyticCompTestConfig[testcaseIdx].testcaseId);
        //__asm( " MARK        31"  ) ;
      }
      else
      {
        printf("\nDMAUtils Compression %d,        FAILED!!!!!! \n", gAnalyticCompTestConfig[testcaseIdx].testcaseId);
        printf("\nFirst mismatch at               [%d, %d]     \n", status / width, status % width);
        //__asm( " MARK        30"  ) ;
      }
	  #if HOST_EMULATION
      free(input);
      free(output);
      free(compressed);
      free(sectr);
	  #endif
  }


#ifdef HOST_EMULATION
#if defined(_MSC_VER)
      _aligned_free(pIntMmeBase);
#else
      free(pIntMmeBase);
#endif
#endif
//Exit:
  return 0;
}

