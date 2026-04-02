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






/*
 * Convert MACRO to string.
 * Note: Need to run twice to satisfy pre-compiling
 */

#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)


#ifndef ARM11
#ifdef _TMS320C6X
extern cregister volatile unsigned int DNUM;
extern cregister volatile unsigned int TSCL;
#endif

#ifndef DISABLE_CYCLE_DELAY
/*****************************************************************************
 * Function: Utility function a cycle clock
 ****************************************************************************/
static uint32_t utilReadTime32(void)
{
    uint32_t timeVal =0;

#if defined (_TMS320C6X)
    timeVal = TSCL;
#else
#ifndef NSS_LITE2
    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(timeVal));
#endif
#endif
    return timeVal;
}
#endif

/*****************************************************************************
 * Function: Utility function to introduce delay 
 ****************************************************************************/
void utilCycleDelay (int32_t iCount)
{
#ifndef DISABLE_CYCLE_DELAY
  uint32_t start = utilReadTime32();
#endif
  uint32_t count;

  if (iCount <= 100)
    iCount = 100;

  count = (uint32_t)iCount;
#ifndef DISABLE_CYCLE_DELAY
  while ((utilReadTime32() - start) < count);
#else
  do {
    count--;
  } while (count > 0);
#endif
}

uintptr_t utilgAddr(uintptr_t x)
{
#ifdef _TMS320C6X
	if ((x >= 0x800000) && (x < 0x900000))
	  x = (1 << 28) | (DNUM << 24) | x;
#endif
  	return (x);
}

#else

/* Delay number of cycles */
#define ARM11_CPU_CYCLE_ADJUST_FACTOR   4
void utilCycleDelay (int count)
{
  if (count <= 0)
    return;

  count /= ARM11_CPU_CYCLE_ADJUST_FACTOR;

  while (count)
  {
      count--;
  }
}

uint32_t utilgAddr(uint32_t x)
{
    #if ARM11SIM_ON_NYSH /* Only for ARM11 testing on the simulator */

	if ((x >= 0x800000) && (x < 0x900000))
	  x = (1 << 28) | (DNUM << 24) | x;
    #endif

  	return (x);
}

#endif

#if defined (SA_GEN_TEST_VECTOR)
/*******************************************************************************
 *  Function:   utilInputTestPkts
 *******************************************************************************
 *  DESCRIPTION:  Read the Input Packets from the file
 *
 ******************************************************************************/
void utilInputTestPkts (char* file_name)
{
    FILE *fp;
    char msg[80] = "Unable to open file: %s\n";

    /* Clear the Pkt memory */
    memset(testPktRam, 0, TEST_PKT_RAM_SIZE);
    testPktOffset = 0;

    if((fp = fopen (file_name, "rb")) == (FILE *)NULL)
    {
        salld_sim_sprint (msg, file_name);
        return;
    }

    /* Read the file content to the test Pkt Memory */
    memset(testPktRam, 0, TEST_PKT_RAM_SIZE);

    /*
     * Assume that the file size is smaller than TEST_PKT_RAM_SIZE
     * and it is 0 terminated
     *
     * To be enhanced.
     */
    fread ((void *) testPktRam, 1, TEST_PKT_RAM_SIZE, fp);

    fclose(fp);
}


/*******************************************************************************
 *  Function:   utilGetPkt
 *******************************************************************************
 *  DESCRIPTION:  Input a new test packet
 *
 *  Return TRUE:    packet available
 *         FALSE:   packet is not available
 *
 ******************************************************************************/
Bool utilGetPkt(testPktDesc_t *pktDesc)
{
    uint8_t *pData = &testPktRam[testPktOffset];
    uint16_t offset = 0;

    if(testPktOffset >= TEST_PKT_RAM_SIZE)
        return(FALSE);

    /* Read Packet Size */
    pktDesc->size = MK_UINT16(pData[offset], pData[offset+1]);

    if (pktDesc->size == 0)
    {
        return (FALSE);
    }
    offset += 2;

    /* Read the payload information */
    pktDesc->payloadOffset = MK_UINT16(pData[offset], pData[offset+1]);
    offset += 2;

    pktDesc->payloadLen = MK_UINT16(pData[offset], pData[offset+1]);
    offset += 2;

    pktDesc->pkt = &pData[offset];

    testPktOffset += (offset + pktDesc->size);

    if(testPktOffset & 0x01)
        testPktOffset++;


    return (TRUE);

}
#endif


/*******************************************************************************
 *  Function:   utilPrepFout
 *******************************************************************************
 *  DESCRIPTION:  Open and prepare output files
 *
 ******************************************************************************/
void utilPrepFout(tFramework_t  *tf)
{
   #if  SA_GEN_TEST_VECTOR
    tf->fp_ctx = fopen (XSTRINGIFY(SA_ROOT_DIR) "/ti/drv/sa/test/SaUnitTest/outputs/sc_ctx.bin", "wb");
    tf->fp_pktIn = fopen (XSTRINGIFY(SA_ROOT_DIR) "/ti/drv/sa/test/SaUnitTest/outputs/pkt_in.bin", "wb");
    tf->fp_pktOut = fopen (XSTRINGIFY(SA_ROOT_DIR) "/ti/drv/sa/test/SaUnitTest/outputs/pkt_out.bin", "wb");
   #else
    tf->fp_ctx    = NULL;
    tf->fp_pktIn  = NULL;
    tf->fp_pktOut = NULL;
   #endif
}

/*******************************************************************************
 *  Function:   utilCloseFout
 *******************************************************************************
 *  DESCRIPTION:  Clean and close output files
 *
 ******************************************************************************/
void utilCloseFout(tFramework_t  *tf)
{
    if(tf->fp_ctx)
        fclose(tf->fp_ctx);

    if(tf->fp_pktIn)
        fclose(tf->fp_pktIn);

    if(tf->fp_pktOut)
        fclose(tf->fp_pktOut);
}

/*******************************************************************************
 *  Function:   utilOutputPkt
 *******************************************************************************
 *  DESCRIPTION:  Copy the packet related information to the output file
 ******************************************************************************/
void utilOutputPkt(tFramework_t  *tf, uint32_t *swInfo, uint32_t psInfoSize, uint32_t* psInfo,
                   uint32_t pktSize, uint8_t* pkt, Bool in)
{
    int i;
    uint16_t data[2];
    FILE* fp = (in)?tf->fp_pktIn:tf->fp_pktOut;

    if (fp == NULL)
        return;

    /* Output Software Info */
    for (i = 0; i < 3; i++)
    {
        pktWrite32bits_m((uint8_t *)data, 0, swInfo[i]);
        fwrite ((void *) data, 1, 4, fp);
    }

    /* Output the PS Info related operation */
    /* Ps Info is always 4-byte alignment */
    pktWrite32bits_m((uint8_t *)data, 0, (uint32_t)psInfoSize);
    fwrite ((void *) data, 1, 4, fp);

    psInfoSize >>= 2;
    for (i = 0; i < psInfoSize; i++)
    {
        pktWrite32bits_m((uint8_t *)data, 0, psInfo[i]);
        fwrite ((void *) data, 1, 4, fp);
    }

    /* Output the packet */
    pktWrite32bits_m((uint8_t *)data, 0, (uint32_t)pktSize);
    fwrite ((void *) data, 1, 4, fp);
    pktSize = (pktSize + 3)/4*4;
    fwrite ((void *) pkt, 1, pktSize, fp);
}

/*******************************************************************************
 *  Function:   utilOutputSc
 *******************************************************************************
 *  DESCRIPTION:  Copy the security context related information to the output file
 ******************************************************************************/
void utlOutputSc (tFramework_t *tf,  uint16_t scSize, uint16_t scID, uint8_t* scBuf)
{
  uint16_t data[2];
  FILE* fp =  tf->fp_ctx;
  extern void  salld_swiz_128 (uint8_t* in, uint8_t* out, uint16_t size);
  uint8_t   *temp;

  /* Check if output is desired */
  if (fp == NULL) return;

  /* allocate memory */
  temp = (uint8_t*) salld_sim_malloc(scSize, 16);
  /* Swizzle the arrary */
  salld_swiz_128(scBuf, temp, scSize);

  /*
   * Output the SC buffer in the following format
   *    Sc Size
   *    Sc Id
   *    Sc Address
   *    Sc Content
   */
  pktWrite16bits_m((uint8_t *)data, 0, scSize);
  fwrite ((void *) data, 1, 2, fp);

  pktWrite16bits_m((uint8_t *)data, 0, scID);
  fwrite ((void *) data, 1, 2, fp);

  pktWrite32bits_m((uint8_t *)data, 0, (uint32_t)(uintptr_t)scBuf);
  fwrite ((void *) data, 1, 4, fp);

  fwrite ((void *) temp, 1, scSize, fp);

  /* Free the memory */
  salld_sim_free(temp, scSize);
}

static uint16_t utilpScSize[4] = {0, 64, 96, 128};

/*******************************************************************************
 *  Function:   utilOutputScFromSwInfo
 *******************************************************************************
 *  DESCRIPTION:  Copy the security context related information to the output file
 ******************************************************************************/
void  utilOutputScFromSwInfo(tFramework_t *tf, uint32_t* pSwInfo)
{
    uint16_t scID = pSwInfo[0] & 0xFFFF ;
    uint8_t* scBuf = (uint8_t *)(uintptr_t)pSwInfo[1];
    uint8_t   sizeCfg;
    uint16_t  scSize;

#if defined( _BIG_ENDIAN )
    sizeCfg = scBuf[1];
#else
    sizeCfg = scBuf[14];
#endif

    scSize = utilpScSize[sizeCfg & 0x03] +
             utilpScSize[(sizeCfg >> 2) & 0x03] +
             utilpScSize[(sizeCfg >> 4) & 0x03];

    utlOutputSc(tf, scSize, scID, scBuf);
}


/******************************************************************************
 * Function:    util_ones_complement_add
 ******************************************************************************
 * Description: Calculate an Internet style one's complement checksum
 *
 ******************************************************************************/
uint16_t utilOnesCompAdd (uint16_t value1, uint16_t value2)
{
  uint32_t result;

  result = (uint32_t)value1 + (uint32_t)value2;

  result = (result >> 16) + (result & 0xFFFF); /* add in carry   */
  result += (result >> 16);                    /* maybe one more */
  return (uint16_t)result;
} /* utilOnesCompAdd */

/******************************************************************************
 * Function:    utilOnesCompChkSum
 ******************************************************************************
 * Description: Calculate an Internet style one's complement checksum
 *
 ******************************************************************************/
uint16_t utilOnesCompChkSum ( uint16_t *p_data, uint16_t len )
{
  uint32_t  chksum = 0;

  while (len > 0)
  {
    chksum += (uint32_t)pktRead16bits_m ((uint8_t *)p_data,0);
    p_data++;
    len--;
  }
  chksum = (chksum >> 16) + (chksum & 0xFFFF); /* add in carry   */
  chksum += (chksum >> 16);                    /* maybe one more */
  return (uint16_t)chksum;

} /* end of utilOnesCompChkSum() */

/**************************************************************************************
 * FUNCTION PURPOSE: Compute and insert the ipv4 checksum
 **************************************************************************************
 * DESCRIPTION: Compute and insert the ipv4 checksum
 **************************************************************************************/
void utilSetIpv4ChkSum (uint8_t *data)
{
  uint16_t hdr_len; /* Hdr Length in 16-bit word */
  uint16_t ip_hdr_chksum;

  /* calculate IP header length */
  hdr_len =  (pktRead8bits_m(data, 0) & 0x0F) << 1;

  pktWrite16bits_m(data, 10, 0);

  /* Length for IP Checksum calculation  should be in terms of 16-bit twords only */
  ip_hdr_chksum = utilOnesCompChkSum ((uint16_t *)data, hdr_len);

  pktWrite16bits_m(data, 10, ~ip_hdr_chksum);

} /* utilSetIpv4ChkSum */

/**************************************************************************************
 * FUNCTION PURPOSE: Compute ipv4 psudo checksum
 **************************************************************************************
 * DESCRIPTION: Compute ipv4 psudo checksum
 **************************************************************************************/
uint16_t utilGetIpv4PsudoChkSum (uint8_t *data, uint16_t payloadLen)
{
  uint16_t psudo_chksum;

  psudo_chksum = utilOnesCompChkSum ((uint16_t *)&data[12], 4);
  psudo_chksum = utilOnesCompAdd(psudo_chksum, (uint16_t) pktRead8bits_m((uint8_t *)data, 9));
  psudo_chksum = utilOnesCompAdd(psudo_chksum, payloadLen);

  return (psudo_chksum);

} /* utilGetIpv4PsudoChkSum */

/**************************************************************************************
 * FUNCTION PURPOSE: Compute ipv6 psudo checksum
 **************************************************************************************
 * DESCRIPTION: Compute ipv6 psudo checksum
 **************************************************************************************/
uint16_t utilGetIpv6PsudoChkSum (uint8_t *data, uint16_t payloadLen)
{
  uint16_t psudo_chksum;

  psudo_chksum = utilOnesCompChkSum ((uint16_t *)&data[8], 16);
  psudo_chksum = utilOnesCompAdd(psudo_chksum, (uint16_t) pktRead8bits_m((uint8_t *)data, 6));
  psudo_chksum = utilOnesCompAdd(psudo_chksum, payloadLen);

  return (psudo_chksum);

} /* utilGetIpv6PsudoChkSum */


/**************************************************************************************
 * FUNCTION PURPOSE: Compute and insert the UDP checksum
 **************************************************************************************
 * DESCRIPTION: Compute and insert the UDP checksum
 **************************************************************************************/
void utilSetUdpChkSum (uint8_t *data, uint16_t len, uint16_t psudoChkSum)
{
  uint16_t chksum;

  pktWrite16bits_m(data, 6, psudoChkSum);

  /* Length for IP Checksum calculation  should be in terms of 16-bit twords only */
  chksum = utilOnesCompChkSum ((uint16_t *)data, len >> 1);
  chksum = ~chksum;
  if(chksum == 0)
    chksum = 0xFFFF;

  pktWrite16bits_m(data, 6, chksum);

} /* utilSetUdpChkSum */

static uint8_t mask1[8] = { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
static uint8_t mask2[8] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };

#define SAU_COMBINE_BYTE(index, bitOffset)   ((buf[index] & mask1[bitOffset]) << bitOffset) |       \
                                             ((buf[index+1] & mask2[bitOffset]) >> (8 - bitOffset))


/*******************************************************************************
 *  Function:   utilDispKeyStream
 *******************************************************************************
 *  DESCRIPTION:  Display Key Streams
 *
 ******************************************************************************/
 void utilDispKeyStream(uint8_t* buf, uint16_t keySize, uint16_t blockSize)
 {
    int i, j;
    int numBlk, blkNum, endIndex;
    int bitOffset, bitOffset2;


    if (!blockSize)
    {
        /* single block */
        salld_sim_print ("Key stream (size = %d):\n", keySize);
        salld_sim_print ("---------------------------------------------------------\n");

        for(i = 0, j = 0; j < keySize/8 ; i += 8, j++)
            salld_sim_print("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
                              buf[i], buf[i+1], buf[i+2], buf[i+3],
                              buf[i+4], buf[i+5], buf[i+6], buf[i+7]);
        for(;i < keySize; i++)
            salld_sim_print("0x%02x ", buf[i]);

        salld_sim_print("\n\n");

    }
    else
    {
        /* multiple blocks */
        numBlk = keySize * 8/blockSize;
        bitOffset = 0;

        blkNum = 0;

        while (blkNum < numBlk)
        {
            endIndex = (blkNum + 1) * blockSize/8;
            salld_sim_print ("Key stream block %d:\n", blkNum);

            for(i = blkNum * blockSize/8, j = 0; j < blockSize/64; i+=8, j++)
                salld_sim_print("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
                                 SAU_COMBINE_BYTE(i, bitOffset), SAU_COMBINE_BYTE(i+ 1, bitOffset),
                                 SAU_COMBINE_BYTE(i+2, bitOffset), SAU_COMBINE_BYTE(i+ 3, bitOffset),
                                 SAU_COMBINE_BYTE(i+4, bitOffset), SAU_COMBINE_BYTE(i+ 5, bitOffset),
                                 SAU_COMBINE_BYTE(i+6, bitOffset), SAU_COMBINE_BYTE(i+ 7, bitOffset));

            for(;i < endIndex; i++)
                salld_sim_print("0x%02x ", SAU_COMBINE_BYTE(i, bitOffset));

            bitOffset2 = ((blkNum + 1) * blockSize) & 0x7;

            if(bitOffset2)
                salld_sim_print("0x%02x ", (buf[i] & mask1[bitOffset] & mask2[bitOffset2]) << bitOffset);

            salld_sim_print("\n\n");

            bitOffset = bitOffset2;

            blkNum++;

        }
    }

 }




#define GPIO_REG_DIR01      0x10
#define GPIO_REG_OUT01      0x14

void gpioSet (unsigned int v)
{
  /* Configure the GPIO as all output */
  *((unsigned int *)(0x2320000 + GPIO_REG_OUT01)) = v;
  *((unsigned int *)(0x2320000 + GPIO_REG_DIR01)) = 0;
}






