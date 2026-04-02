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
extern cregister volatile unsigned int DNUM;
extern cregister volatile unsigned int TSCL;
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

/*****************************************************************************
 * Function: Utility function to introduce delay 
 ****************************************************************************/
void utilCycleDelay (int32_t iCount)
{
  uint32_t start = utilReadTime32();
  uint32_t count = (uint32_t)iCount;

  if (iCount <= 0)
    return;

  while ((utilReadTime32() - start) < count);
}

#if defined (_TMS320C6X)
uint32_t utilgAddr(uint32_t x)
{
	if ((x >= 0x800000) && (x < 0x900000))
	  x = (1 << 28) | (DNUM << 24) | x;
	  
  	return (x);
}
#endif
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

/*
 * Convert MACRO to string.
 * Note: Need to run twice to satisfy pre-compiling 
 */

#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)

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

  /* Length for IP Checksum calculation  should be in terms of 8-bit uint8_t only */
  chksum = utilOnesCompChkSum ((uint16_t *)data, len >> 1);
  chksum = ~chksum;
  if(chksum == 0)
    chksum = 0xFFFF;
  
  pktWrite16bits_m(data, 6, chksum);

} /* utilSetUdpChkSum */






