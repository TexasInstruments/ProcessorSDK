/*
 *
 * Copyright (C) 2010-2015 Texas Instruments Incorporated - http://www.ti.com/
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

#ifndef __LINUX_USER_SPACE
#ifdef _TMS320C6X
extern cregister volatile unsigned int DNUM;
extern cregister volatile unsigned int TSCL;
#endif

#if !defined(NSS_LITE2)
/*****************************************************************************
 * Function: Utility function a cycle clock
 ****************************************************************************/
static uint32_t utilReadTime32(void)
{
    uint32_t timeVal;

#if defined (_TMS320C6X)
    timeVal = TSCL;
#else
    __asm__ __volatile__ ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(timeVal));
#endif
    return timeVal;
}
#endif

/*****************************************************************************
 * Function: Utility function to introduce delay 
 ****************************************************************************/
void utilCycleDelay (int32_t iCount)
{
#if defined (NSS_LITE2)
    Osal_delay(iCount);
#else
    uint32_t start = utilReadTime32();
    uint32_t count = (uint32_t)iCount;

    if (iCount <= 0)
        return;

    while ((utilReadTime32() - start) < count);
#endif
}

uint32_t utilgAddr(uint32_t x)
{
#ifdef _TMS320C6X
	if ((x >= 0x800000) && (x < 0x900000))
	  x = (1 << 28) | (DNUM << 24) | x;
#endif
  	return (x);
}
#else
void utilCycleDelay (int count)
{
    uint32_t sat;
    for (sat=0; sat<count; sat++);
    return;
}

uint32_t utilgAddr(uint32_t x)
{
  	return (x);
}
#endif

static int8_t utilName2Index(char *name, uint32_t *vecSize);
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
extern const uint8_t pkt_test1[];
extern const uint32_t pkt_test1_size;
extern const uint8_t pkt_test1_enc[];
extern const uint32_t pkt_test1_enc_size;
extern const uint8_t pkt_test2[];
extern const uint32_t pkt_test2_size;
extern const uint8_t pkt_test2_enc[];
extern const uint32_t pkt_test2_enc_size;
extern const uint8_t pkt_test3[];
extern const uint32_t pkt_test3_size;
extern const uint8_t pkt_test3_enc[];
extern const uint32_t pkt_test3_enc_size;
extern const uint8_t pkt_test4[];
extern const uint32_t pkt_test4_size;
extern const uint8_t pkt_test4_enc[];
extern const uint32_t pkt_test4_enc_size;
extern const uint8_t pkt_test5[];
extern const uint32_t pkt_test5_size;
extern const uint8_t pkt_test5_enc[];
extern const uint32_t pkt_test5_enc_size;
#endif

extern const uint8_t pkt_test7[];
extern const uint32_t pkt_test7_size;
/* Please do not change the order of the index, if there is a need to
 * change the order, then please update utilName2Index function also
 */
static uintptr_t testVectors[20] = {
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    (uintptr_t) &pkt_test1,      /* Index 0 */
    (uintptr_t) &pkt_test1_enc,  /* Index 1 */
    (uintptr_t) &pkt_test2,      /* Index 2 */
    (uintptr_t) &pkt_test2_enc,  /* Index 3 */
    (uintptr_t) &pkt_test3,      /* Index 4 */
    (uintptr_t) &pkt_test3_enc,  /* Index 5 */
    (uintptr_t) &pkt_test4,      /* Index 6 */
    (uintptr_t) &pkt_test4_enc,  /* Index 7 */
    (uintptr_t) &pkt_test5,      /* Index 8 */
    (uintptr_t) &pkt_test5_enc,  /* Index 9 */
#endif
    (uintptr_t) &pkt_test7,      /* Index 10 */
};


static int8_t utilName2Index(char *name, uint32_t *sz)
{
    int8_t index = 0;
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    if (strncmp(name, "pkt_test1.bin", 20) == 0)
    {
        index = 0;
        *sz = pkt_test1_size;
    }
    else if (strncmp(name, "pkt_test1_enc.bin", 20) == 0)
    {
        index = 1;
        *sz = pkt_test1_enc_size;
    }
    else if (strncmp(name, "pkt_test2.bin", 20) == 0)
    {
        index = 2;
        *sz = pkt_test2_size;
    }
    else if (strncmp(name, "pkt_test2_enc.bin",20) == 0)
    {
        index = 3;
        *sz = pkt_test2_enc_size;
    }
    else if (strncmp(name, "pkt_test3.bin",20) == 0)
    {
        index = 4;
        *sz = pkt_test3_size;
    }
    else if (strncmp(name, "pkt_test3_enc.bin",20) == 0)
    {
        index = 5;
        *sz = pkt_test3_enc_size;
    }
    else if (strncmp(name, "pkt_test4.bin",20) == 0)
    {
        index = 6;
        *sz = pkt_test4_size;
    }
    else if (strncmp(name, "pkt_test4_enc.bin",20) == 0)
    {
        index = 7;
        *sz = pkt_test4_enc_size;
    }
    else if (strncmp(name, "pkt_test5.bin",20) == 0)
    {
        index = 8;
        *sz = pkt_test5_size;
    }
    else if (strncmp(name, "pkt_test5_enc.bin",20) == 0)
    {
        index = 9;
        *sz = pkt_test5_enc_size;
    }
    else if (strncmp(name, "pkt_test7.bin",20) == 0)
    {
        index = 10;
        *sz = pkt_test7_size;
    }
    else
    {
        index = -1;
    }
#endif
    return (index);
}

/*
 * Convert MACRO to string.
 * Note: Need to run twice to satisfy pre-compiling
 */

#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)


/*******************************************************************************
 *  Function:   utilInputTestPkts
 *******************************************************************************
 *  DESCRIPTION:  Read the Input Packets from the file
 *
 ******************************************************************************/
void utilInputTestPkts (char* file_name)
{
    int8_t    index;
    uint32_t  sz;

    /* Clear the Pkt memory */
    memset(testPktRam, 0, TEST_PKT_RAM_SIZE);
    testPktOffset = 0;

    index = utilName2Index(file_name, &sz);

    if (index >= 0)
    {
        memcpy( (void *) testPktRam, (void *) testVectors[index], sz);
    }
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

/*******************************************************************************
 *  Function:   utilPrepFout
 *******************************************************************************
 *  DESCRIPTION:  Open and prepare output files
 *
 ******************************************************************************/
void utilPrepFout(tFramework_t  *tf)
{
    tf->fp_ctx = fopen (XSTRINGIFY(SA_ROOT_DIR) "/ti/drv/sa/example/SaBasicExample/outputs/sc_ctx.bin", "wb");
    tf->fp_pktIn = fopen (XSTRINGIFY(SA_ROOT_DIR) "/ti/drv/sa/example/SaBasicExample/outputs/pkt_in.bin", "wb");
    tf->fp_pktOut = fopen (XSTRINGIFY(SA_ROOT_DIR) "/ti/drv/sa/example/SaBasicExample/outputs/pkt_out.bin", "wb");
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

  /* Check if output is desired */
  if (fp == NULL) return;

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

  pktWrite32bits_m((uint8_t *)data, 0, (uint32_t)scBuf);
  fwrite ((void *) data, 1, 4, fp);

  fwrite ((void *) scBuf, 1, scSize, fp);
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

  /* Length for IP Checksum calculation  should be in terms of 8-bit uint8_t only */
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
  psudo_chksum = utilOnesCompAdd(psudo_chksum, (uint16_t) pktRead8bits_m(data, 9));
  psudo_chksum = utilOnesCompAdd(psudo_chksum, payloadLen);

  return (psudo_chksum);

} /* utilGetIpv4PsudoChkSum */

/**************************************************************************************
 * FUNCTION PURPOSE: Compute and insert the UDP checksum
 **************************************************************************************
 * DESCRIPTION: Compute and insert the UDP checksum
 **************************************************************************************/
void utilSetUdpChkSum (uint8_t *data, uint16_t len, uint16_t psudoChkSum)
{
  uint16_t chksum;

  pktWrite16bits_m(data, 6, psudoChkSum);

  /* Length for IP Checksum calculation  should be in terms of 8-bit uint8_t only */
  chksum = utilOnesCompChkSum ((uint16_t *)data, len >> 1);
  chksum = ~chksum;
  if(chksum == 0)
    chksum = 0xFFFF;

  pktWrite16bits_m(data, 6, chksum);

} /* utilSetUdpChkSum */






