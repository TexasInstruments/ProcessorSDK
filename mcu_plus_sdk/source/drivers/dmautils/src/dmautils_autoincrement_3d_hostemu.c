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


/*
 *******************************************************************************
 *
 *  DMA utility functions
 *
 *******************************************************************************
*/

/**
 *****************************************************************************
 * \file  dmautils_autoincrement_3d_hostemu.c
 *
 * \brief*  This file contains host emulation function implementation for dma util functions for
 *  configuring DMA on J7 device for autoincrement 3D usecase.
 *
 *
 *****************************************************************************
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "stdlib.h"

#if !defined (MCU_PLUS_SDK)
#include "ti/drv/udma/dmautils/src/dmautils_autoincrement_3d_priv.h"
#include "ti/drv/udma/dmautils/include/dmautils_autoincrement_3d.h"
#else
#include "drivers/dmautils/src/dmautils_autoincrement_3d_priv.h"
#include "drivers/dmautils/include/dmautils_autoincrement_3d.h"
#endif

#if defined(__C7100__) || defined(__C7120__) || defined(__C7504__)
#include <c7x.h>
#endif

#define USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR 0

/* If its a Loki Build then force the mode to be in hostemulation as Loki doesnt support DRU */
#if defined (LOKI_BUILD)
#define HOST_EMULATION (1U)
#endif

#define DMAUTILS_ALIGN_CEIL(VAL, ALIGN) ((((VAL) + (ALIGN) - 1)/(ALIGN)) * (ALIGN) )


/**
 *
 *  \brief This function Decode Zero based elements
 *
 *  \param srcPtr [IN]
 *
 * \param decodedElem [IN]
 *
 * \param curBitPos [IN]
 *
 *  \return   Zero Value (isEOB)
 *
 */
static int32_t decodeZeroBasedElem(const uint8_t * srcPtr, uint8_t * decodedElem, int32_t * curBitPos);

/*Common Functions:*/
uint64_t hostEmulation_addressUpdate( uint64_t base, int32_t offset, uint64_t addrMask )
{
  uint64_t newAddr;

  newAddr = (uint64_t)(base + offset);

  if ( addrMask != 0U )
  {
    newAddr = ( base & addrMask ) | ( newAddr & ~addrMask );
  }

  return newAddr;
}

void hostEmulation_circMask( uint32_t cbk0, uint32_t cbk1, uint64_t * circMask0, uint64_t * circMask1  )
{
  uint64_t blockSize0 = (uint64_t)cbk0 + (uint64_t)9; /* power-of-2 of block size in bytes */
  uint64_t blockSize1 = (uint64_t)cbk0 + (uint64_t)cbk1 + (uint64_t)10; /* power-of-2 of block size in bytes */

  if ( blockSize1 > 31U )
  {
    blockSize1 = 32U; /* clamp to 4GB maximum size */
  }
  *circMask0 = ((uint64_t)~0ULL << blockSize0);
  *circMask1 = ((uint64_t)~0ULL << blockSize1);
}


//////////////////////////////////
// Compression helper functions //
//////////////////////////////////

/*
   secondaryTR : compression secondary TR
   symbolIn    : 8/16/32 bit symbol to compress
   symbolOut   : compressed symbol
   return      : length of compressed symbol in bits
*/
static uint8_t DmaUtilsAutoInc3d_compressElem
( const CSL_UdmapSecTR * secondaryTR
, uint32_t         symbolIn
, uint64_t*        symbolOut
)
{
  uint32_t elem       = symbolIn;
  uint64_t compressed = 0;
  uint8_t  length     = 8; /* elements are 8-bit unless variable-k*/
  uint32_t temp       = elem;
  uint8_t  kVal       = (uint8_t)( ((uint32_t)secondaryTR->data[0]) >> (uint8_t)8) & (uint8_t)0x07;  /*TODO add dynamic k-updating...*/
  uint8_t  bias       = (uint8_t)( ((uint32_t)secondaryTR->data[0]) >> (uint8_t)8) & (uint8_t)0xFF; /*TODO CSL doesn't exist for compression fields...*/
  uint8_t  algorithm  = ( ((uint8_t)secondaryTR->data[0])      ) & (uint8_t)0x0F;
  uint16_t i;
  if ( algorithm != (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K )
  {
    temp = elem - bias;

    /*prevent underflowing*/
    temp &= ((uint32_t)1 << ((uint32_t)length)) - (uint32_t)1;
  }



  switch( algorithm ) /*TODO CSL doesn't exist for compression fields...*/
  {
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO : /*zero algorithm*/
    {
      /*python code (no bias)
        compressed = 0 ;
        if ( num != 0 ) :
          compressed = 1 ;
          #invert the 8 bit number
          for i in range (0,8) :
            if ( num & ( 1 << i ) ) :
              compressed |= ( 1 << (8 - i) ) ;
      */

      if ( temp != 0U )
      {
        compressed = 1;
        /*invert the 8 bit number*/
        for ( i = 0; i < length; ++i )
        {
          if ( (temp & ((uint64_t)1<<(uint64_t)i) )!= (uint64_t)0)
          {
            compressed |= ( (uint64_t)1 << ((uint64_t)length - (uint64_t)i) );
          }
        }
        length = length + (uint8_t)1;
      }
      else
      {
        compressed = 0;
        length     = 1;
      }
      break;
    }
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_UEG : /*unsigned exponential golomb*/
    {
      /*python code (no bias)
        compressed = 0 ;
        j = num + 1 ;
        z = -1 ;
        #invert j
        inverted = 0 ;
        for k in range(0,9) :
          if ( (j & (1<<k) ) ) :
            z = k;
        for k in range(0,z+1) :
          if ( j & (1<<k) ) :
            inverted |= 1<<(z-k) ;
        compressed = inverted << z;
      */
      temp++;

      uint8_t lengthDiv2 = 0xff;
      /*Find how many zeroes to append*/
      for ( i = 0U; i < (length+1U); ++i )
      {
        if ( (temp & ((uint64_t)1<<(uint64_t)i))!= (uint64_t)0 )
        {
          lengthDiv2 = (uint8_t)i;
        }
      }
      /*invert temp*/
      uint32_t inverted = 0;
      for ( i = (uint16_t)0; i < ((uint16_t)lengthDiv2+(uint16_t)2); ++i )
      {
        if ( (temp &  ((uint64_t)1<<(uint64_t)i) )!= (uint64_t)0 )
        {
            inverted |= ((uint32_t)1<<((uint32_t)lengthDiv2-(uint32_t)i));
        }
      }
      compressed = ((uint64_t)inverted << ((uint64_t)lengthDiv2));
      length = ( lengthDiv2 << (uint8_t)1 ) + (uint8_t)1;
      break;
    }
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K : /*signed exponential golomb*/
    {
      length = 0; /*TODO: variable K not yet supported...*/
      break;
    }
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG :
    {
      /*python code (no bias)
        compressed = 0 ;
        #need to sign extend 1 bit
        if ( num & ( 1 << (elsize-1) ) ) :
          temp = num | ( 1 << elsize ) ;
        else :
          temp = num ;

        #add in variable-k
        temp += (1 << my_k) - 1

        #mask out top bit in case of rollover
        temp &= ( (1 << elsize+1) - 1 )

        #alternate positive negative
        if ( temp < ( 1 << elsize ) ) :
          j = ( temp << 1 ) + 1 ;
        else :
          j = ( ( 1<<(elsize+1) ) - temp ) << 1 ;

        #invert j
        inverted =  0 ;
        z        = -1 ;
        for k in range(0,elsize+1) :
          if ( (j & (1<<k) ) ) :
            z = k;
        for k in range(0,z+1) :
          if ( j & (1<<k) ) :
            inverted |= 1<<(z-k) ;
        compressed = inverted << z;
      */

      /* sign extend by 1 bit*/
      if (( temp & ( (uint32_t)1 << ((uint32_t)length-(uint32_t)1) ) )!= 0U)
      {
        temp |= ( (uint32_t)1 << (uint32_t)length );
      }

      if ( algorithm == (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K )
      {
        temp   = elem + ((uint32_t)1 << (uint32_t)kVal) - (uint32_t)1;
        temp  &= (((uint32_t)1 << ((uint32_t)length+(uint32_t)1)) - (uint32_t)1); /*mask out top bit in case of rollover*/
      }

      /*alternate output mapping between positive and negative inputs*/
      if (( temp & ( (uint32_t)1 << (uint32_t)length ) )!= 0U)
      {
        temp = ( ( (uint32_t)1<<((uint32_t)length+(uint32_t)1) ) - temp ) << (uint32_t)1;
      }
      else
      {
          temp = ( temp << 1U ) + 1U;
      }

      /*Find how many zeroes to append*/
      uint8_t lengthDiv2 = 0xff;
      for ( i = (uint16_t)0; (i < ((uint16_t)length+(uint16_t)1)); ++i )
      {
        if ( (temp &((uint64_t)1<<(uint64_t)i))!= (uint64_t)0)
        {
          lengthDiv2 = (uint8_t)i;
        }
      }

      /*invert temp*/
      uint64_t inverted = 0;
      for ( i = (uint16_t)0; i < ((uint16_t)lengthDiv2+(uint16_t)1); ++i )
      {
        if ( (temp &  ((uint64_t)1<<(uint64_t)i))!= (uint64_t)0 )
        {
          inverted |= (uint64_t)1<<((uint64_t)lengthDiv2-(uint64_t)i);
        }
      }
      compressed = inverted << lengthDiv2;
      length = ( lengthDiv2 << (uint8_t)1 ) + (uint8_t)1;
      break;
    }
    default:
    {
      break;
    }
  }
  *symbolOut = compressed;
  return length;
}


/*
 secondaryTR : compression secondary TR
 symbolIn    : next 64-bits of compressed data
 symbolOut   : uncompressed symbol
 return      : length of compressed symbol
*/
static uint8_t DmaUtilsAutoInc3d_decompressElem (const CSL_UdmapSecTR * secondaryTR,
                                                       uint64_t           symbolIn,
                                                       uint32_t*          symbolOut)
{
  uint64_t elem         = symbolIn;
  uint32_t uncompressed = 0;
  uint8_t  length       = 8; /*elements are 8-bit unless variable-k*/
  uint8_t  elsize       = length;
  uint64_t temp         = elem;
  uint8_t  kVal       = (uint8_t)( ((uint32_t)(secondaryTR->data[0])) >> (uint8_t)8 ) & (uint8_t)0x07;  /*TODO add dynamic k-updating...*/
  uint8_t  bias       = (uint8_t)( ((uint32_t)secondaryTR->data[0]) >> (uint8_t)8 ) & (uint8_t)0xFF; /*TODO CSL doesn't exist for compression fields...*/
  uint8_t  algorithm  = ( ((uint8_t)secondaryTR->data[0])      ) & (uint8_t)0x0F;
  uint8_t i ;
  switch( algorithm ) /*TODO CSL doesn't exist for compression fields...*/
  {
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO :  /*zero algorithm*/
    {
      /*python code
        uncompressed = 0 ;
        if ( compressed & 1 ) :
          #invert the remaining 8 bit number
          for i in range (1,9) :
            if ( compressed & ( 1 << i ) ) :
              uncompressed |= ( 1 << (8 - i) ) ;
      */
      if (( temp & (uint64_t)1 )!= (uint64_t)0)
      {
        length = elsize + (uint8_t)1;
        for (i = 1; i < length; i++ )
        {
          if ( (temp & ((uint64_t)1<<(uint64_t)i)  )!= (uint64_t)0 )
          {
            uncompressed |= ( (uint32_t)1 << ((uint32_t)8-(uint32_t)i) );
          }
        }
      }
      else
      {
        length = 1;
      }
      break;
    }
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_UEG : /*unsigned exponential golomb*/
    {
      /*python code
        uncompressed = 0 ;
        z = -1 ;
        for k in range(0,9) :
          if ( compressed & (1<<k) ) :
            z = k ;
            break ;
        length = 2*z + 1 ;
        for k in range(0,z+1) :
          if ( compressed & (1<<(length-k-1)) ) :
            uncompressed |= (1<<k) ;
        uncompressed -= 1 ;
      */
      int8_t zeros = -1;
      for ( i = 0; i <= elsize; i++ )
      {
        if ( (temp &  ((uint64_t)1<<(uint64_t)i)  )!= (uint64_t)0)
        {
          zeros = (int8_t)i;
          i=elsize;
        }
      }

      if ( zeros == -1 )
      {
        length = 0;
      }
      else
      {
        length = ((uint8_t)zeros<<(uint8_t)1) + (uint8_t)1;

        for ( i = (uint8_t)0; i < ((uint8_t)zeros+(uint8_t)1); i++ )
        {
          if (( temp & ((uint64_t)1<<((uint64_t)length-(uint64_t)i-(uint64_t)1)) )!= (uint64_t)0)
          {
            uncompressed |= ((uint32_t)1<<(uint32_t)i);
          }
        }
        uncompressed--;
      }
      break;
    }
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K : /*signed exponential golomb*/
    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG :
    {
      /*uncompressed = 0 ;
        z = -1 ;
        for k in range(0,elsize+1) :
          if ( compressed & (1<<k) ) :
            z = k ;
            break ;
        length = 2*z + 1 ;
        for k in range(0,z+1) :
          if ( compressed & (1<<(length-k-1)) ) :
            uncompressed |= (1<<k) ;
        if ( uncompressed & 1 ) :
          uncompressed = ( uncompressed - 1 ) >> 1 ;
        else :
          uncompressed = ( 1 << (elsize+1) ) - ( uncompressed >> 1 ) ;

        #remove variable-k
        if ( uncompressed >= (1 << my_k) - 1 ) :
          uncompressed -= (1 << my_k) - 1
        else :
          uncompressed += (1 << elsize) - (1 << my_k) + 1

        #mask out top bit if set
        uncompressed &= (1<<elsize)-1
      */
      int8_t zeros = -1;
      for ( i = 0; i <= elsize; i++ )
      {
        if (( temp & (((uint64_t)1<<(uint64_t)i)) )!= (uint64_t)0)
        {
          zeros = (int8_t)i;
          i=elsize;
        }
      }

      if ( zeros == -1 )
      {
        length = 0;
      }
      else
      {

              length =(((uint8_t)zeros<<(uint8_t)1) + (uint8_t)1);

              for (i = (uint8_t)0; i < ((uint8_t)zeros+(uint8_t)1); i++ )
              {
                if (( temp & ((uint64_t)1<<((uint64_t)length-(uint64_t)i-(uint64_t)1)) )!= (uint64_t)0)
                {
                  uncompressed |= ((uint32_t)1<<(uint32_t)i);
                }
              }

              if (( uncompressed & 1U )!= 0U)
              {
                uncompressed = uncompressed >> 1U;
              }
              else
              {
                uncompressed = ( (uint32_t)1 << ((uint32_t)elsize+(uint32_t)1) ) - ( uncompressed >> (uint32_t)1 );
              }

      /*remove variable-k*/
      if ( algorithm == (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K )
      {
        if ( uncompressed >= ((1U << (uint32_t)kVal) - 1U ))
        {
          uncompressed -= (((uint32_t)1 << (uint32_t)kVal) - (uint32_t)1);
        }
        else
        {
          uncompressed += (((uint32_t)1 << (uint32_t)elsize) - ((uint32_t)1 << (uint32_t)kVal) + (uint32_t)1);
        }
      }
   }
      break;
    }
    default:
    {
      length = 0;
    }
    break;
  }

  if ( algorithm != (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K )
  {
    /*add bias back in*/
    uncompressed += bias;

    /*prevent overflow*/
    uncompressed &= (((uint32_t)1<<(uint32_t)elsize)-(uint32_t)1);
  }

  *symbolOut = uncompressed;
  return length;
}

/*Start of updated decompression routines*/
Bool checkZeroBasedEOB(int32_t curBitPos, int32_t numDecodeBits)
{
  Bool isEOB = TRUE;
  if(((uint32_t)curBitPos + (uint32_t)numDecodeBits) > 128U)
  {
    isEOB = TRUE;
  }
  else
  {
    isEOB = FALSE;
  }
  return isEOB;
}

static int32_t decodeZeroBasedElem(const uint8_t* srcPtr, uint8_t* decodedElem, int32_t* curBitPos)
{
  int32_t localBitPos  = (*curBitPos) % (int32_t)8;
  int32_t localBytePos = (*curBitPos) / (int32_t)8;
  int32_t isEOB = (int32_t)TRUE;
  int32_t i = 0;
  int32_t numBitsRead = (int32_t)0;
  //Reset decoded value:
  (*decodedElem) = 0U;
  if(((uint32_t)srcPtr[localBytePos] & ((uint32_t)1 << (uint32_t)localBitPos))!= 0U)
  {
    //Non Zero Value:
    isEOB = checkZeroBasedEOB(*curBitPos, (int32_t)9);
    (*curBitPos)++;
    if( (int32_t)FALSE == isEOB)
    {
      localBitPos  = (*curBitPos) % (int32_t)8;
      localBytePos = (*curBitPos) / (int32_t)8;
      //Retrieve symbol bits and invert to decode symbol
      for(i = localBitPos; (i < (int32_t)8) && (numBitsRead < (int32_t)8); i++)
      {
        numBitsRead++;
        if(((uint32_t)srcPtr[localBytePos] & ((uint32_t)1 << (uint32_t)i)) != 0U)
        {
          (*decodedElem) |= ((uint8_t)1 << ((uint8_t)8 - (uint8_t)numBitsRead));
        }
      }
      localBitPos = (int32_t)0;
      localBytePos++;
      for(i = localBitPos; (i < (int32_t)8U) && (numBitsRead < (int32_t)8U); i++)
      {
        numBitsRead++;
        if(((uint32_t)srcPtr[localBytePos] & ((uint32_t)1 << (uint32_t)i))!= 0U)
        {
          (*decodedElem) |= ((uint8_t)1 << ((uint8_t)8 - (uint8_t)numBitsRead));
        }
      }
      (*curBitPos) += (int32_t)8U;
    }
  }
  else
  {
    //Zero Value:
    isEOB = checkZeroBasedEOB(*curBitPos, (int32_t)1);
    if( (int32_t)FALSE == isEOB)
    {
      *decodedElem = 0;
      (*curBitPos)++;
    }

  }
  return isEOB;
}

/*
 secondaryTR  : compression secondary TR
 srcPtr       : Compressed bytestream
 dstPtr       : Uncompressed bytestream
 bitPosition  : Current bit position in CDB for a given byte
 bytePosition : Current byte position within the CDB
 return       : Number of bytes decoded from a given CDB
*/

static uint32_t DmaUtilsAutoInc3d_decompressZeroBasedCDB(uint8_t* srcPtr, uint8_t** dstPtr, uint64_t sbDstAM0, uint32_t* sbIcnt0, uint32_t sbWidth1, uint32_t* curBitPosition, uint8_t bias)
{
  int32_t curBitPos = (int32_t)*curBitPosition;
  int32_t isEOB = (int32_t)FALSE;
  uint8_t* localDstPtr = *dstPtr;
  uint8_t decodedElem = 0;
  int32_t numDecodedBytes = 0;
  uint32_t srcJump = 16U;
  uint32_t biasAdjustedValue = 0;
  while((isEOB == (int32_t)FALSE) && ((((int32_t)numDecodedBytes + (int32_t)(*sbIcnt0))) < (int64_t)(sbWidth1)))
  {
    isEOB = decodeZeroBasedElem(srcPtr, &decodedElem, &curBitPos);
    if(isEOB == (int32_t)FALSE)
    {
      //Adjust for bias:
      biasAdjustedValue = ((uint32_t)decodedElem + (uint32_t)bias);
      /*prevent overflow, assumption of 8-bit decompressed element being made here*/
      biasAdjustedValue &= ((uint32_t)1<<8U)-(uint32_t)1;
      decodedElem = (uint8_t) biasAdjustedValue;
      *(localDstPtr) = decodedElem;
      localDstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)localDstPtr , 1 , sbDstAM0);
      numDecodedBytes++;
    }
  }
  if(!(((uint32_t)numDecodedBytes + (*sbIcnt0)) < (sbWidth1)))
  {
    //Implicit
    srcJump = 0U;
  }
  else
  {
    srcJump = 16U;
  }
  *curBitPosition = (uint32_t)curBitPos;
  *dstPtr         = localDstPtr;
  *sbIcnt0       += (uint32_t)numDecodedBytes;
  return srcJump;
}
/*End of updated decompression routines*/

//////////////////////////////////////
// END Compression helper functions //
//////////////////////////////////////

/*Software Compression Routine*/
/*SW Compression Logic*/
int32_t DmaUitlsAutoInc3d_CompressSW(void* trMem)
{
  uint32_t chId;
  for ( chId = 0U; chId <= 1U; chId++)/*:TODO: Remove hard coded value of 32*/
  {
      uint8_t *srcPtr;
      uint8_t *dstPtr;
      uint8_t *cdbPtr;
      CSL_UdmapSecTR * sectrPtr;
      uint8_t  eobSize;
      //uint8_t  eobSizeLoc;
      uint16_t *srcPtr16;
      uint16_t *dstPtr16;
      uint32_t triggerType;
      uint8_t  cmprsType;
      uint32_t circDir;
      uint32_t icnt0, sbIcnt0, sbIcnt1,sbDim1t;
      int32_t  sbDim1, sDim0, dDim0;
      uint32_t CBK0, CBK1;
      uint32_t AM0, AM1, AM2, AM3, sbAM0, sbAM1;
      uint64_t srcAM0, srcAM1, srcAM2, srcAM3;
      uint64_t dstAM0, dstAM1, dstAM2, dstAM3;

      uint64_t sbSrcAM0, sbSrcAM1;
      uint64_t sbDstAM0, sbDstAM1;

      uint64_t circMask0;
      uint64_t circMask1;
      uint32_t AMODE, DFMT, ELYPE;
      uint32_t loopCnt1Reset, loopCnt2Reset;
      uint8_t * interimBuffer = NULL;
      uint32_t srcLoopExitCondition = 0;
      //uint32_t dstLoopExitCondition;
      uint32_t totalSrcCnt, totalDstCnt;//, totalSBCnt;
      int32_t nextSBStartOffset = 0; /*Stores offset of the SB compressed bitstream form the base of the compressed bitsteram. Used in DFMT == 5*/
      /* Clear the sw trigger so that next trigger can happen */
      uint16_t i;
      CSL_UdmapTR   trArr[4];
      trArr[0] = *((CSL_UdmapTR*) trMem);
      trArr[1] = *((CSL_UdmapTR*) trMem);
      trArr[2] = *((CSL_UdmapTR*) trMem);
      trArr[3] = *((CSL_UdmapTR*) trMem);
      CSL_UdmapTR * origTransferRecord  = (CSL_UdmapTR *)(void *) &trArr[0];
      CSL_UdmapTR * nextTransferRecord  = (CSL_UdmapTR *)(void *) &trArr[1];
      CSL_UdmapTR * nextTransferRecord1 = (CSL_UdmapTR *)(void *) &trArr[2];
      CSL_UdmapTR * nextTransferRecord2 = (CSL_UdmapTR *)(void *) &trArr[3];


      triggerType = CSL_FEXT(origTransferRecord->flags, UDMAP_TR_FLAGS_TRIGGER0_TYPE);
      AMODE = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE);
      DFMT  = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_DFMT );
      //Update for compression: (Fails for other modes):
      nextTransferRecord->daddr += (uint64_t)DMAUTILS_COMP_SECTR_PROG_SIZE;
      nextTransferRecord1->daddr += (uint64_t)DMAUTILS_COMP_SECTR_PROG_SIZE;
      nextTransferRecord2->daddr += (uint64_t)DMAUTILS_COMP_SECTR_PROG_SIZE;
      if ( AMODE == CSL_UDMAP_TR_FMTFLAGS_AMODE_CIRCULAR)
      {
        circDir        = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_DIR);
        CBK0         = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK0);
        CBK1         = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK1);
        AM0           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM0);
        AM1           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM1);
        AM2           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM2);
        AM3           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM3);
        hostEmulation_circMask(CBK0, CBK1, &circMask0, &circMask1);
      }
      else
      {
        /* Linear Addressing */
        circMask0 = (~0ULL);
        circMask1 = (~0ULL);
        AM0 = 0;
        AM1 = 0;
        AM2 = 0;
        AM3 = 0;
        circDir = 0;
      }
      /*Decoding secondary TR*/
      if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP ) /*TODO compression CSL definitions don't exist yet...*/
      {
        sectrPtr   = (CSL_UdmapSecTR *)(origTransferRecord->daddr);
        sDim0      = (int32_t)(sectrPtr->data[2]);
        dDim0      = (int32_t)(sectrPtr->data[3]);
        sbDim1t     = (sectrPtr->flags & 0xFFFFFFF0U);
        cmprsType  = (uint8_t)(sectrPtr->data[0] & 0xFU);
        sbAM0      = (sectrPtr->data[0] >> 4U) & 0x3U;
        sbAM1      = (sectrPtr->data[0] >> 6U) & 0x3U;
        sbDim1     =  (int32_t) sbDim1t;
        switch( cmprsType )
        {
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO :
          {
            eobSize = 1;
            break;
          }
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_UEG :
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG :
          {
            eobSize = 9;
            break;
          }
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K :
          {
            eobSize = 9; /*TODO eobSize = elsize + 1 (9,13,17)*/
            break;
          }
          default:
          {
            break;
          }
        }
      }
      else if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP ) /*TODO decompression CSL definition doesn't exist yet...*/
      {
        sectrPtr   = (CSL_UdmapSecTR *)origTransferRecord->addr;
        sbDim1t     = (sectrPtr->flags & 0xFFFFFFF0U);
        sDim0      = (int32_t)(sectrPtr->data[2]);
        dDim0      = (int32_t)(sectrPtr->data[3]);
        cmprsType  = (uint8_t)(sectrPtr->data[0] & 0xFU);
        sbAM0      = (sectrPtr->data[0] >> 4U) & 0x3U;
        sbAM1      = (sectrPtr->data[0] >> 6U) & 0x3U;
        sbDim1     =  (int32_t) sbDim1t;

        switch( cmprsType )
        {
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO :
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_UEG :
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG :
          {
            eobSize = 9; /*EOB for zero compression is 1 bit to write and 9 to read*/
            break;
          }
          case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K :
          {
            eobSize = 9; /*TODO eobSize = elsize + 1*/
            break;
          }
          default:
          {
            break;
          }
        }
      }
      else
   {
     /*Do Nothing*/
   }

      /*Circularity masks and flags setup*/
      if ( circDir == CSL_UDMAP_TR_FMTFLAGS_DIR_DST_USES_AMODE)
      {
        dstAM0 = (AM0 == 0U ) ? 0U : ( (AM0 == 1U) ? circMask0 : circMask1  );/* Circular update */
        srcAM0 = 0;/* Linear Update */

        dstAM1 = (AM1 == 0U ) ? 0U : ( (AM1 == 1U) ? circMask0 : circMask1  );/* Circular update */
        srcAM1 = 0;/* Linear Update */

        dstAM2 = (AM2 == 0U ) ? 0U : ( (AM2 == 1U) ? circMask0 : circMask1  );/* Circular update */
        srcAM2 = 0;/* Linear Update */

        dstAM3 = (AM3 == 0U ) ? 0U : ( (AM3 == 1U) ? circMask0 : circMask1  );/* Circular update */
        srcAM3 = 0;/* Linear Update */

        if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP) /* TODO decompression CSL definition doesn't exist yet...*/
        {
          sbDstAM0 = (sbAM0 == 0U ) ? 0U : ( (sbAM0 == 1U) ? circMask0 : circMask1  );/* Circular update */
          sbSrcAM0 = 0; /* Linear Update */

          sbDstAM1 = (sbAM1 == 0U ) ? 0U : ( (sbAM1 == 1U) ? circMask0 : circMask1  );/* Circular update */
          sbSrcAM1 = 0; /* Linear Update */
        }
        else
        {
          sbDstAM0 = 0;
          sbSrcAM0 = 0;
          sbDstAM1 = 0;
          sbSrcAM1 = 0;
        }
      }
      else
      {
        dstAM0 = 0;/* Linear Update */
        srcAM0 = (AM0 == 0U ) ? 0U : ( (AM0 == 1U) ? circMask0 : circMask1  );

        dstAM1 = 0;/* Linear Update */
        srcAM1 = (AM1 == 0U ) ? 0U : ( (AM1 == 1U) ? circMask0 : circMask1  );

        dstAM2 = 0;/* Linear Update */
        srcAM2 = (AM2 == 0U ) ? 0U : ( (AM2 == 1U) ? circMask0 : circMask1  );

        dstAM3 = 0;/* Linear Update */
        srcAM3 = (AM3 == 0U ) ? 0U : ( (AM3 == 1U) ? circMask0 : circMask1  );


        if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP) /* TODO decompression CSL definition doesn't exist yet...*/
        {
          sbDstAM0 = 0; /* Linear Update */
          sbSrcAM0 = (sbAM0 == 0U ) ? 0U : ( (sbAM0 == 1U) ? circMask0 : circMask1  );/* Circular update */

          sbDstAM1 = 0; /* Linear Update */
          sbSrcAM1 = (sbAM1 == 0U ) ? 0U : ( (sbAM1 == 1U) ? circMask0 : circMask1  );/* Circular update */
        }
        else
        {
          sbDstAM0 = 0;
          sbSrcAM0 = 0;
          sbDstAM1 = 0;
          sbSrcAM1 = 0;
        }
      }

      loopCnt1Reset = 0;    /*These are set as per source ICNTx*/
      loopCnt2Reset = 0;
      /*Set up interim buffers for non-compression/decompression flow. Direct copy for  compression/decompression flow.
         if we are compressing/decompressing then we don't need an interim buffer. Just write compressed data to the destination.
         Setup of dstPtr moved inside the while loop
      */
      switch ( DFMT )
      {
        case (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
        {
          break;
        }
        case (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
        {
          break;
        }
        default:
        {
          /* allocate worst case, actual buffer used will depend on trugerType */
          ELYPE = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_ELYPE);
          #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            uint32_t interimBufSize = (origTransferRecord->icnt0 * origTransferRecord->icnt1 * origTransferRecord->icnt2 *
                                        origTransferRecord->icnt3);
      if (( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1) ||
          ( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2))
      {
        interimBufSize *= 2; // As icnt0 is in elements in case of 16 bit to 8 bit conversion
      }
      interimBuffer = (uint8_t *)malloc(interimBufSize);
      dstPtr = interimBuffer;
      dstPtr16 = (uint16_t *)interimBuffer;
          #endif
          break;
        }
      }

      totalSrcCnt = ((uint32_t)nextTransferRecord->icnt0 * (uint32_t)nextTransferRecord->icnt1 * (uint32_t)nextTransferRecord->icnt2 * (uint32_t)nextTransferRecord->icnt3);
      totalDstCnt = ((uint32_t)nextTransferRecord->dicnt0 * (uint32_t)nextTransferRecord->dicnt1 * (uint32_t)nextTransferRecord->dicnt2 * (uint32_t)nextTransferRecord->dicnt3);

      srcLoopExitCondition  = ( totalSrcCnt < totalDstCnt )? totalSrcCnt : totalDstCnt;
      //dstLoopExitCondition  = srcLoopExitCondition;
      /* Set the minimum value of icnt3 for both src and dst as TR completes whenever anyone of them  is exauhsted */
      /* Transfer source data to a intermediate linear buffer */
      uint8_t  ret= 0U;
      bool cond = (bool)1;
      while (cond == (bool)1)
      {
        uint8_t* sbCdbPtr;  /*stores the pointer to cdb table entry corresponding to a SB being encoded or decoded.*/
        uint8_t* sbAddr;    /*stores the pointer to to a uncomressed SB data being encoded or decoded. */
        if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP )    /*TODO decompression CSL definitions don't exist yet...*/
        {
          srcPtr = (uint8_t *)nextTransferRecord->addr;
          cdbPtr = (uint8_t *)nextTransferRecord->daddr;
          dstPtr = (uint8_t *)hostEmulation_addressUpdate(sectrPtr->addr , ((int32_t *)cdbPtr)[1], 0);

          sbCdbPtr    = cdbPtr;
          sbAddr      = srcPtr;    /*Stores the base pointer for a SB row*/
        }
        else if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP )  /*TODO decompression CSL definitions don't exist yet...*/
        {
          cdbPtr = (uint8_t *)  nextTransferRecord->addr;
          sbCdbPtr    = cdbPtr;
          dstPtr = (uint8_t *)  nextTransferRecord->daddr;
          sbAddr = dstPtr;
        }
        else
        {
          srcPtr = (uint8_t *)nextTransferRecord->addr;
          if ( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
          {
            srcPtr16 = (uint16_t *)srcPtr;
          }
          #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            dstPtr = (uint8_t *)nextTransferRecord->daddr;
            if ( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
            {
              dstPtr16 = (uint16_t *)dstPtr;
            }
          #endif
        }

        for (icnt0 = 0; icnt0 < nextTransferRecord->icnt0; icnt0++)
        {
          if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP )/* TODO compression CSL definition doesn't exist yet...*/
          {
            uint64_t cmprs       = 0;
            uint64_t dstPtrcom  = ((uint64_t)dstPtr - sectrPtr->addr);
            uint8_t  len         = 0;
            uint16_t cdbCnt      = 0;
            uint8_t* sb1Addr     = sbAddr;    /*Stores the base pointer for a SB row being encoded/decoded*/
            int32_t  startOffset = (int32_t)dstPtrcom;
            /*Implicit EOB for both cases, filling buffers based on
              H/W behaviour:*/
            if ( cmprsType == (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO )
            {
              memset(dstPtr, 0x0, 16);
            }
            else
            {
              memset(dstPtr, 0, 16);
            }
            uint32_t  sbHeight = ((sectrPtr->data[1] >> 16U) & 0xFFFFU);
            uint32_t  sbWidth  = 16U * ((sectrPtr->data[1] & 0xFFFFU));
            for (sbIcnt1 = 0; sbIcnt1 < sbHeight; sbIcnt1++)
            {
              srcPtr = sb1Addr;
              for (sbIcnt0 = 0; sbIcnt0 < sbWidth; sbIcnt0++)
              {
                uint8_t newLen = DmaUtilsAutoInc3d_compressElem( sectrPtr , (uint32_t)(*srcPtr) , &cmprs );
                /*merge compressed data into output*/

                if ( (len + newLen) > (uint8_t)128 ) /*This won't fit in the current CDB*/
                {
                  if ( cmprsType == (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO )
                  {
                    *(dstPtr + (len >> 3U)) |= ( 1U << (len & 0x7U) );
                  }
                  dstPtr += 16;
                  /*Implicit EOB for both cases, filling buffers based on
                    H/W behaviour:*/
                  if ( cmprsType == (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO )
                  {
                    memset(dstPtr, 0, 16);
                  }
                  else
                  {
                    memset(dstPtr, 0, 16);
                  }
                  len = 0;
                  cdbCnt++;
                }

                /*align compressed data starting bit with the output data*/
                cmprs <<=  len & 0x7U;

                /*deal with compressed data endianness agnostically*/
                uint8_t touchedBytes = (newLen + (len & 0x7U)) >> 3U;
                for ( i = 0; i <= touchedBytes; i++ )
                {
                  uint8_t off  = (len >> (uint8_t)3) + (uint8_t)i;
                  uint8_t elem = ((uint8_t)cmprs >> ((uint8_t)i << (uint8_t)3)) & (uint8_t)0xFF;
                  *(dstPtr + off) |= elem;
                }
                len   += newLen;
                srcPtr = (uint8_t*)hostEmulation_addressUpdate((uint64_t)srcPtr , 1 , sbSrcAM0);
              }
              sb1Addr = (uint8_t*)hostEmulation_addressUpdate((uint64_t)sb1Addr , sbDim1 , sbSrcAM1);
            }
            /*
              finish last CDB in superblock
              eob only has to be written for zero compression. Otherwise it's zero
            */
            if ( cmprsType == (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO )
            {
              *(dstPtr + (len >> 3U)) |= ( 1U << (len & 0x7U) );
            }
            dstPtr += 16;
            /*zero out next CDB for writing*/
            memset(dstPtr, 0, 16);
            /* MISRAC fix for VA_UNUSED.GEN */
            //len = 0;
            cdbCnt++;

            /*Write CDB info to table*/
            ( (uint32_t*)sbCdbPtr )[0] = cdbCnt;
            ( (int32_t *)sbCdbPtr )[1] = startOffset;
            nextSBStartOffset = startOffset + ((int32_t)cdbCnt * (int32_t)16);
            sbCdbPtr += dDim0;
            sbAddr  = (uint8_t*)hostEmulation_addressUpdate((uint64_t)sbAddr, sDim0, srcAM0);
          }
          else if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP ) /*TODO decompression CSL definitions don't exist yet...*/
          {
            uint64_t cmprs         = 0;
            uint32_t decmprs       = 0;
            uint8_t  cdbLen        = 128;
            uint8_t  maxSymbolSize = 17; /*default for UEG/SEG*/
            uint8_t* sb1Addr       = sbAddr;
            int32_t srcJump       = 0;
            uint32_t curBitPosition = 0;

            switch( cmprsType )
            {
              case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO :
              {
                maxSymbolSize = 16; /*actual = 9*/
                break;
              }
              case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K :
              {
                  maxSymbolSize = 17; /*TODO maxSymbolSize = (elemsize * 2) + 1*/
                  break;
              }
              case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_UEG :
              case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG :
              {
                maxSymbolSize = (uint8_t)((uint8_t)(maxSymbolSize + (uint8_t)7) & ~(uint8_t)0x7); /*round up to nearest multiple of 8*/
                break;
              }
              default:
              {
                break;
              }
            }
            /*The actual max size is always 1 more than a multiple of 8, so when we drop up to 7 bits during alignment we will still have enough data*/

            /*get start of superblock*/
            srcPtr = (uint8_t*)hostEmulation_addressUpdate(sectrPtr->addr, ((int32_t*)sbCdbPtr )[1], 0); /*srcAM0 ==> 0?*/

            uint32_t  sbHeight = ((sectrPtr->data[1] >> 16U) & 0xFFFFU);
            uint32_t  sbWidth  = 16U * ((sectrPtr->data[1] & 0xFFFFU));
            uint8_t   bias      =  (uint8_t)(( (uint16_t)((uint8_t)sectrPtr->data[0]) >>(uint8_t) 8 ) & (uint16_t)0xFF);

            for (sbIcnt1 = 0; sbIcnt1 < sbHeight; sbIcnt1++)
            {
              dstPtr = sb1Addr;
              for (sbIcnt0 = 0; sbIcnt0 < sbWidth; )
              {

                if(cmprsType == (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO)
                {
                  /*CDB Decode, and dstPtr update happens within this function:*/
                  srcJump = (int32_t)DmaUtilsAutoInc3d_decompressZeroBasedCDB(srcPtr, &dstPtr, sbDstAM0, &sbIcnt0, sbWidth, &curBitPosition, bias);
                  //Move to the next CDB:
                  srcPtr += srcJump;
                  if(srcJump != 0)
                  {
                    //Move to next CDB:
                    curBitPosition = 0;
                  }
                  //sbIcnt0 += dstBytes;
                }
                else
                {
                  /*get at least one symbol of data in this CDB and be endianess agnostic*/
                  cmprs = 0;
                  uint8_t len = (cdbLen > maxSymbolSize) ? maxSymbolSize  : cdbLen;		// COpy
                  len         = ( len & (uint8_t)0x7 ) ? ((len >> (uint8_t)3) + (uint8_t)1) : (len >> (uint8_t)3);
                  uint8_t off = ((uint8_t)128 - cdbLen) >> (uint8_t)3;

                  /*printf("maxSymbolSize = %d : cdbLen = %d : len = %d : off = %d :\n",maxSymbolSize,cdbLen,len,off);*/
                  for ( i = 0; i < len; i++ )
                  {
                    cmprs |= (uint64_t)*(srcPtr + (uint64_t)off + (uint64_t)i) << ((uint64_t)i << (uint64_t)3);
                  }

                  /*align data*/
                  if ((cdbLen & (uint8_t)0x7)!= (uint8_t)0)
                  {
                    cmprs = cmprs >> ((uint64_t)8 - ((uint64_t)cdbLen & (uint64_t)0x7));
                  }

                  /*check for eob symbol*/
                  bool eobFound = (bool)false;
                  switch( cmprsType )
                  {
                    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_ZERO :
                    {
                      eobFound = ((uint32_t)cmprs & (((uint32_t)1<<(uint32_t)eobSize)-(uint32_t)1)) == 1U;
                      break;
                    }
                    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_UEG :
                    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG :
                    case (uint8_t)DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K : /*TODO varK symbol is (element bits + 1) consecutive zeros*/
                    {
                      eobFound = ((uint32_t)cmprs & (((uint32_t)1<<(uint32_t)eobSize)-(uint32_t)1)) == 0U;
                      break;
                    }
                    default :
                    {
                      break;
                    }
                  }

                  if (eobFound)
                  {
                    /*printf("EOB found!\n");*/
                    srcPtr += 16; /*move to next CDB*/
                    cdbLen  = 128;
                  }
                  else
                  {
                    cdbLen -= DmaUtilsAutoInc3d_decompressElem( sectrPtr , cmprs , &decmprs );
                    /*TODO varK can have non-1B elements...*/
                    *dstPtr = (uint8_t)decmprs & (uint8_t)0xFF;
                    dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr , 1 , sbDstAM0);
                    sbIcnt0++; /*only move on if we decompressed an actual element*/
                  }
                }

              }

              sb1Addr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)sb1Addr , sbDim1 , sbDstAM1);
            }

            sbAddr  = (uint8_t *)hostEmulation_addressUpdate((uint64_t)sbAddr , dDim0 , dstAM0); /*Move outside?*/
            sbCdbPtr += sDim0;
          }
          else
          {

            if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
            {  *dstPtr16 = *srcPtr;
			        srcPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)srcPtr, 1, srcAM0);
			        #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
  	              dstPtr16++;
			        #else
			  	      dstPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)dstPtr16, 2, dstAM0);
			        #endif
            }
            else if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
            {
              #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
              *dstPtr16 = *srcPtr16;
              dstPtr16++;
              #else
                *dstPtr = (uint8_t)*srcPtr16;
                dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
              #endif
              srcPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)srcPtr16, 2, srcAM0);
            }
            else
            {
              *dstPtr = *srcPtr;
              srcPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)srcPtr, 1, srcAM0);
              #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
                dstPtr++;
              #else
                dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
              #endif
            }
          }
        }
        nextTransferRecord->icnt1--;
        if( (DFMT != (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP) && (DFMT != (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP))
        {
          #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            nextTransferRecord->dicnt1--;
          #endif
        }
        else
        {
          nextTransferRecord->dicnt1--;
        }

        switch( DFMT )
        {
          case (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
          {
            /*update CDB table address*/
            nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord->daddr , nextTransferRecord->ddim1, 0);
            /*move source pointer*/
            nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord->addr  , nextTransferRecord->dim1,   srcAM1);
            break;
          }
          case (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
          {
            /*update CDB table address*/
            nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord->addr  , nextTransferRecord->dim1, 0);
            /*move destination pointer*/
            nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord->daddr , nextTransferRecord->ddim1,   dstAM1);
            break;
          }
          default :
          {
            nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord->addr  , nextTransferRecord->dim1,   srcAM1);
            #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            	nextTransferRecord->daddr   = hostEmulation_addressUpdate(nextTransferRecord->daddr  , nextTransferRecord->ddim1,   dstAM1);
            #endif
            break;
          }
        }
        /*For compression/decompression type 9 transfer currently we assume that icnt0 = dicnt0 and icnt1 = dicnt1*/

        if ( nextTransferRecord->icnt1 == 0U)
        {
          loopCnt1Reset = 1;
          nextTransferRecord->icnt2--;
          nextTransferRecord->icnt1 = origTransferRecord->icnt1;
		      if( (DFMT != (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP) && (DFMT != (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP))
          {
         	 #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            	nextTransferRecord->dicnt2--;
				      nextTransferRecord->dicnt1 = origTransferRecord->dicnt1;
          	 #endif
          }
          else
          {
          	nextTransferRecord->dicnt2--;
			      nextTransferRecord->dicnt1 = origTransferRecord->dicnt1;
          }
		      switch( DFMT )
          {
            case (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP :
            {
              /*update CDB table address*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord1->daddr , nextTransferRecord->ddim2, 0);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              /*move source pointer*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord1->addr  , nextTransferRecord->dim2,   srcAM2);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              break;
            }
            case (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
            {
              /*update CDB table address*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord1->addr  , nextTransferRecord->dim2, 0);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              /*move destination pointer*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord1->daddr , nextTransferRecord->ddim2,   dstAM2);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              break;
            }
            default :
            {
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord1->addr  , nextTransferRecord->dim2,   srcAM2);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
                nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord1->daddr , nextTransferRecord->ddim2,   dstAM2);
                nextTransferRecord1->daddr = nextTransferRecord->daddr;
              #endif
              break;
            }
          }
        }

        if ( nextTransferRecord->icnt2 == 0U)
        {
          loopCnt2Reset= 1;
          nextTransferRecord->icnt3--;
          nextTransferRecord->icnt2 = origTransferRecord->icnt2;

          switch( DFMT )
          {
            case (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
            {
              /*update CDB table address*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord2->addr  , nextTransferRecord->dim3,   srcAM3);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              nextTransferRecord2->addr  = nextTransferRecord->addr;
              break;
            }
            case (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
            {
              /*update CDB table address*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord2->addr  , nextTransferRecord->dim3, 0);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              nextTransferRecord2->addr  = nextTransferRecord->addr;
              break;
            }
            default :
            {
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord2->addr  , nextTransferRecord->dim3,   srcAM3);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              nextTransferRecord2->addr  = nextTransferRecord->addr;
              break;
            }
          }
        }

        if ( nextTransferRecord->dicnt2 == 0U)
        {

		      if( (DFMT != (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP) && (DFMT != (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP))
          {
         	  #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
			        nextTransferRecord->dicnt3--;
              nextTransferRecord->dicnt2 = origTransferRecord->dicnt2;
          	#endif
          }
          else
          {
            nextTransferRecord->dicnt3--;
            nextTransferRecord->dicnt2 = origTransferRecord->dicnt2;
          }
          switch( DFMT )
          {
            case (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
            {
              /*update CDB table address*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord2->daddr , nextTransferRecord->ddim3, 0);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              nextTransferRecord2->daddr = nextTransferRecord->daddr;
              break;
            }
            case (uint32_t)DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
            {
              /*move destination pointer*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord2->daddr , nextTransferRecord->ddim3,   dstAM3);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              nextTransferRecord2->daddr = nextTransferRecord->daddr;
              break;
            }
            default :
            {
              #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
                nextTransferRecord->daddr   = hostEmulation_addressUpdate(nextTransferRecord2->daddr  , nextTransferRecord->ddim3,   dstAM3); //Validate
                nextTransferRecord1->daddr  = nextTransferRecord->daddr;
                nextTransferRecord2->daddr  = nextTransferRecord->daddr;
              #endif
              break;
            }
          }
        }

        /*Updating the start offset for next SB to be read in next iteration of the while loop or next trigger*/
        if ( (DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP) && (nextTransferRecord->icnt3 != 0U) && (nextTransferRecord->dicnt3 != 0U))
        {
          ((int32_t *)nextTransferRecord->daddr)[1] = nextSBStartOffset;
        }

        if ( (nextTransferRecord->icnt3 == (uint16_t)0) || (nextTransferRecord->dicnt3 == (uint16_t)0))
        {
          if ( DFMT == (uint32_t)DMAUTILSAUTOINC3D_DFMT_COMP)
          {
            /* fix for MISRAC VA_UNUSED.GEN */
            //totalSrcCnt = ((uint32_t)origTransferRecord->icnt0 * (uint32_t)origTransferRecord->icnt1 * (uint32_t)origTransferRecord->icnt2 * (uint32_t)origTransferRecord->icnt3);
            //totalDstCnt = ((uint32_t)origTransferRecord->dicnt0 * (uint32_t)origTransferRecord->dicnt1 * (uint32_t)origTransferRecord->dicnt2 * (uint32_t)origTransferRecord->dicnt3);
            nextSBStartOffset = 0;
          }
          ret = 1U;
        }


        if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC )
        {
          ret = 1U;
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC )
        {
          if ( loopCnt1Reset == 1U)
          {
            nextTransferRecord1->addr   = nextTransferRecord->addr;
            ret = 1U;
          }
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC )
        {
          if ( loopCnt2Reset == 1U)
          {
            nextTransferRecord2->addr   = nextTransferRecord->addr;
          ret = 1U;
          }
        }
        else
        {
          /*Indicates 4D sync,  Just continue */
        }
        srcLoopExitCondition--;
        if ( srcLoopExitCondition == 0U)
        {
          nextTransferRecord->icnt3 = 0;
          ret = 1U;
        }
if (ret == 1U)
{
  break;
}
      }
    #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
      if (DFMT != DMAUTILSAUTOINC3D_DFMT_COMP && DFMT != DMAUTILSAUTOINC3D_DFMT_DECOMP)
      {
        loopCnt1Reset = 0;
        loopCnt2Reset = 0;
        srcPtr = interimBuffer;
        srcPtr16 = (uint16_t *)interimBuffer;
         /* Now copy the intermediate data to the destination buffer */
        while (1)
         {
        if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
        {
          dstPtr16 = (uint16_t *)nextTransferRecord->daddr;
          for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
          {
            *dstPtr16 = (uint16_t)(*srcPtr16);
            srcPtr16++;
            dstPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)dstPtr16, 2, dstAM0);
          }
        }
        else if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
           {
             dstPtr = (uint8_t *)nextTransferRecord->daddr;
             for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
             {
            *dstPtr = (uint8_t)(*srcPtr16);
              srcPtr16++;
                 dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
             }
           }
           else
          {
            dstPtr = (uint8_t *)nextTransferRecord->daddr;
            for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
            {
              *dstPtr = *srcPtr;
              *srcPtr++;
              dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
            }
          }
          nextTransferRecord->dicnt1--;

          nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord->daddr, nextTransferRecord->ddim1, dstAM1);

          if ( nextTransferRecord->dicnt1 == 0)
          {
            loopCnt1Reset = 1;
            nextTransferRecord->dicnt2--;
            nextTransferRecord->dicnt1 = origTransferRecord->dicnt1;

            nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord1->daddr, nextTransferRecord->ddim2, dstAM2);
            nextTransferRecord1->daddr = nextTransferRecord->daddr;
          }

          if ( nextTransferRecord->dicnt2 == 0)
          {
            loopCnt2Reset= 1;
            nextTransferRecord->dicnt3--;
            nextTransferRecord->dicnt2 = origTransferRecord->dicnt2;

            nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord2->daddr, nextTransferRecord->ddim3, dstAM3);

            nextTransferRecord1->daddr = nextTransferRecord->daddr;
            nextTransferRecord2->daddr = nextTransferRecord->daddr;
          }

          if ( nextTransferRecord->dicnt3 == 0)
          {
            break;
          }

          if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC )
          {
            break;
          }
          else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC )
          {
            if ( loopCnt1Reset == 1)
            {
              break;
            }
          }
          else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC )
          {
            if ( loopCnt2Reset == 1)
            {
              break;
            }
          }

          dstLoopExitCondition--;
          if ( dstLoopExitCondition == 0)
          {
            nextTransferRecord->dicnt3 = 0;
            break;
          }
        } /*while(1)*/
      } /*if (not compression)*/
    #endif
      if (( nextTransferRecord->icnt3 == 0U ) || ( nextTransferRecord->dicnt3 == 0U ))
      {
        /* this indicates that TR is received from PSIL */
        /*Not handled*/
      }
      if ( interimBuffer != NULL)
      {
        free(interimBuffer);
      }
  }
  return 0;
}
/*End of Software Compression Routine*/

#ifdef HOST_EMULATION

void hostEmulation_updateTriggerCount(struct Udma_DrvObj * udmaDrvHandle,
      volatile uint64_t *pSwTrigReg, uint32_t utcId)
{
  uint32_t i;
  CSL_DRU_t *druRegs = udmaDrvHandle->utcInfo[utcId].druRegs;
  uintptr_t origPtr = (uintptr_t)pSwTrigReg;
  uintptr_t currPtr;

  for ( i = 0; i < udmaDrvHandle->utcInfo[utcId].numCh; i++)
  {
    currPtr = (uintptr_t)&druRegs->CHRT[i].CHRT_SWTRIG;
    if ( currPtr == origPtr)
    {
      /* Use reserved space for tracking number of triggers submitted for a given channel */
      druRegs->CHRT[i].Resv_256[0]++;
      break;
    }
  }
}

void hostEmulation_druChSubmitAtomicTr(CSL_DRU_t *pRegs,
                                           uint32_t chId,
                                           void *  vdata)
{
  CSL_UdmapTR * tr = (CSL_UdmapTR *)vdata;
  CSL_UdmapTR * origTransferRecord   = ( CSL_UdmapTR *)&pRegs->CHATOMIC[chId];
  CSL_UdmapTR * nextTransferRecord  = (CSL_UdmapTR *)&pRegs->CHATOMIC[chId].DEBUG[0];
  CSL_UdmapTR * nextTransferRecord1 = (CSL_UdmapTR *)&pRegs->CHATOMIC[chId].DEBUG[1];
  CSL_UdmapTR * nextTransferRecord2 = (CSL_UdmapTR *)&pRegs->CHATOMIC[chId].DEBUG[2];

  /* Use reserved space for tracking number of triggers submitted for a given channel */
  pRegs->CHRT[chId].Resv_256[0] = 0;

  *origTransferRecord  = *tr;
  *nextTransferRecord = *tr;
  *nextTransferRecord1 = *tr;
  *nextTransferRecord2 = *tr;

  /*nextTransferRecord 0,1,2 need to track the CDB table address if in compression*/
  uint32_t DFMT  = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_DFMT);
  if ( DFMT == DMAUTILSAUTOINC3D_DFMT_COMP ) /*TODO CSL definitions for compression don't exist yet*/
  {
    uint64_t cdbPtr            = origTransferRecord->daddr + DMAUTILS_COMP_SECTR_PROG_SIZE;
    nextTransferRecord ->daddr = cdbPtr; /*compression doesn't need destination addr tracking*/
    nextTransferRecord1->daddr = cdbPtr;
    nextTransferRecord2->daddr = cdbPtr;
  }
  else if ( DFMT == DMAUTILSAUTOINC3D_DFMT_DECOMP ) /*TODO CSL definitions for decompression don't exist yet*/
  {
    CSL_UdmapSecTR* sectrPtr  = (CSL_UdmapSecTR *)  origTransferRecord->addr;
    int32_t cdbOffset         = (int32_t)(sectrPtr->data[4]);
    uint64_t cdbPtr           = origTransferRecord->addr + cdbOffset;
    nextTransferRecord ->addr = cdbPtr; /*decompression doesn't need source addr tracking*/
    nextTransferRecord1->addr = cdbPtr;
    nextTransferRecord2->addr = cdbPtr;
  }
}

void hostEmulation_triggerDMA(struct Udma_DrvObj * udmaDrvHandle, uint32_t utcId)
{
  uint32_t chId;
  CSL_DRU_t * druRegs;

#if ( DMA_UTILS_STANDALONE == 0 )
  CSL_UdmapCfg  * udmapRegs;
  CSL_ringacc_cfgRegs * ringAccCfgRegs;

  druRegs         = udmaDrvHandle->utcInfo[utcId].druRegs;
  ringAccCfgRegs  = udmaDrvHandle->raRegs.pCfgRegs;
  udmapRegs        = &udmaDrvHandle->udmapRegs;
#endif

  druRegs             = udmaDrvHandle->utcInfo[utcId].druRegs;



  for ( chId = 0; chId < 32; chId++)//:TODO: Remove hard coded value of 32
  {
    if ( (druRegs->CHRT[chId].CHRT_SWTRIG & CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER0_MASK) == 1U)
    {
      uint8_t *srcPtr;
      uint8_t *dstPtr;
      uint8_t *cdbPtr;
      CSL_UdmapSecTR * sectrPtr;
      uint8_t  eobSize;
      uint16_t *srcPtr16;
      uint16_t *dstPtr16;
      uint32_t triggerType;
      uint8_t  cmprsType;
      uint32_t circDir;
      uint32_t icnt0, sbIcnt0, sbIcnt1;
      int32_t  sbDim1, sDim0, dDim0;
      uint32_t CBK0, CBK1;
      uint32_t AM0, AM1, AM2, AM3, sbAM0, sbAM1;
      uint64_t srcAM0, srcAM1, srcAM2, srcAM3;
      uint64_t dstAM0, dstAM1, dstAM2, dstAM3;

      uint64_t sbSrcAM0, sbSrcAM1;
      uint64_t sbDstAM0, sbDstAM1;

      uint64_t circMask0;
      uint64_t circMask1;
      uint32_t AMODE, DFMT, ELYPE;
      uint32_t loopCnt1Reset, loopCnt2Reset;
      uint8_t * interimBuffer = NULL;
      uint32_t srcLoopExitCondition = 0;
      uint32_t dstLoopExitCondition = 0;
      uint32_t totalSrcCnt, totalDstCnt;
      int32_t nextSBStartOffset = 0; /*Stores offset of the SB compressed bitstream form the base of the compressed bitsteram. Used in DFMT == 5*/
      /* Clear the sw trigger so that next trigger can happen */
      uint16_t i;

      druRegs->CHRT[chId].Resv_256[0]--;
      /* Use reserved space for tracking number of triggers submitted for a given channel */
      if ( druRegs->CHRT[chId].Resv_256[0] == 0 )
      {
        druRegs->CHRT[chId].CHRT_SWTRIG  = druRegs->CHRT[chId].CHRT_SWTRIG & (uint64_t)(~CSL_DRU_CHRT_SWTRIG_GLOBAL_TRIGGER0_MASK);
      }

      CSL_UdmapTR * origTransferRecord  = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId];
      CSL_UdmapTR * nextTransferRecord = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId].DEBUG[0];
      CSL_UdmapTR * nextTransferRecord1 = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId].DEBUG[1];
      CSL_UdmapTR * nextTransferRecord2 = (CSL_UdmapTR *)(void *) &druRegs->CHATOMIC[chId].DEBUG[2];

      triggerType = CSL_FEXT(origTransferRecord->flags, UDMAP_TR_FLAGS_TRIGGER0_TYPE);
      AMODE = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE);
      DFMT  = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_DFMT);

      if ( AMODE == CSL_UDMAP_TR_FMTFLAGS_AMODE_CIRCULAR)
      {
        circDir        = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_DIR);
        CBK0         = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK0);
        CBK1         = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK1);
        AM0           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM0);
        AM1           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM1);
        AM2           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM2);
        AM3           = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM3);
        hostEmulation_circMask(CBK0, CBK1, &circMask0, &circMask1);
      }
      else
      {
        /* Linear Addressing */
        circMask0 = (~0ULL);
        circMask1 = (~0ULL);
        AM0 = 0;
        AM1 = 0;
        AM2 = 0;
        AM3 = 0;
        circDir = 0;
      }
      /*Decoding secondary TR*/
      if ( DFMT == DMAUTILSAUTOINC3D_DFMT_COMP ) /*TODO compression CSL definitions don't exist yet...*/
      {
        sectrPtr   = (CSL_UdmapSecTR *)(origTransferRecord->daddr);
        sDim0      = (int32_t)(sectrPtr->data[2]);
        dDim0      = (int32_t)(sectrPtr->data[3]);
        sbDim1     = (int32_t)(sectrPtr->flags & 0xFFFFFFF0);
        cmprsType  = sectrPtr->data[0] & 0xF;
        sbAM0      = (sectrPtr->data[0] >> 4) & 0x3;
        sbAM1      = (sectrPtr->data[0] >> 6) & 0x3;

        switch( cmprsType )
        {
          case DMAUTILSAUTOINC3D_CMPRS_ZERO :
          {
            eobSize = 1;
            break;
          }
          case DMAUTILSAUTOINC3D_CMPRS_UEG :
          case DMAUTILSAUTOINC3D_CMPRS_SEG :
          {
            eobSize = 9;
            break;
          }
          case DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K :
          {
            eobSize = 9; /*TODO eobSize = elsize + 1 (9,13,17)*/
            break;
          }
          default:
          {
            break;
          }
        }
      }
      else if ( DFMT == DMAUTILSAUTOINC3D_DFMT_DECOMP ) /*TODO decompression CSL definition doesn't exist yet...*/
      {
        sectrPtr   = (CSL_UdmapSecTR *)origTransferRecord->addr;
        sbDim1     = (int32_t)(sectrPtr->flags & 0xFFFFFFF0);
        sDim0      = (int32_t)(sectrPtr->data[2]);
        dDim0      = (int32_t)(sectrPtr->data[3]);
        cmprsType  = sectrPtr->data[0] & 0xF;
        sbAM0      = (sectrPtr->data[0] >> 4) & 0x3;
        sbAM1      = (sectrPtr->data[0] >> 6) & 0x3;

        switch( cmprsType )
        {
          case DMAUTILSAUTOINC3D_CMPRS_ZERO :
          case DMAUTILSAUTOINC3D_CMPRS_UEG :
          case DMAUTILSAUTOINC3D_CMPRS_SEG :
          {
            eobSize = 16; /*EOB for zero compression is 1 bit to write and 9 to read*/
            break;
          }
          case DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K :
          {
            eobSize = 9; /*TODO eobSize = elsize + 1*/
            break;
          }
          default:
          {
            break;
          }
        }
      }

      /*Circularity masks and flags setup*/
      if ( circDir == CSL_UDMAP_TR_FMTFLAGS_DIR_DST_USES_AMODE)
      {
        dstAM0 = (AM0 == 0 ) ? 0 : ( (AM0 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM0 = 0;/* Linear Update */

        dstAM1 = (AM1 == 0 ) ? 0 : ( (AM1 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM1 = 0;/* Linear Update */

        dstAM2 = (AM2 == 0 ) ? 0 : ( (AM2 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM2 = 0;/* Linear Update */

        dstAM3 = (AM3 == 0 ) ? 0 : ( (AM3 == 1) ? circMask0 : circMask1  );/* Circular update */
        srcAM3 = 0;/* Linear Update */

        if ( DFMT == DMAUTILSAUTOINC3D_DFMT_DECOMP) /* TODO decompression CSL definition doesn't exist yet...*/
        {
          sbDstAM0 = (sbAM0 == 0 ) ? 0 : ( (sbAM0 == 1) ? circMask0 : circMask1  );/* Circular update */
          sbSrcAM0 = 0; /* Linear Update */

          sbDstAM1 = (sbAM1 == 0 ) ? 0 : ( (sbAM1 == 1) ? circMask0 : circMask1  );/* Circular update */
          sbSrcAM1 = 0; /* Linear Update */
        }
        else
        {
          sbDstAM0 = 0;
          sbSrcAM0 = 0;
          sbDstAM1 = 0;
          sbSrcAM1 = 0;
        }
      }
      else
      {
        dstAM0 = 0;/* Linear Update */
        srcAM0 = (AM0 == 0 ) ? 0 : ( (AM0 == 1) ? circMask0 : circMask1  );

        dstAM1 = 0;/* Linear Update */
        srcAM1 = (AM1 == 0 ) ? 0 : ( (AM1 == 1) ? circMask0 : circMask1  );

        dstAM2 = 0;/* Linear Update */
        srcAM2 = (AM2 == 0 ) ? 0 : ( (AM2 == 1) ? circMask0 : circMask1  );

        dstAM3 = 0;/* Linear Update */
        srcAM3 = (AM3 == 0 ) ? 0 : ( (AM3 == 1) ? circMask0 : circMask1  );


        if ( DFMT == DMAUTILSAUTOINC3D_DFMT_COMP) /* TODO decompression CSL definition doesn't exist yet...*/
        {
          sbDstAM0 = 0; /* Linear Update */
          sbSrcAM0 = (sbAM0 == 0 ) ? 0 : ( (sbAM0 == 1) ? circMask0 : circMask1  );/* Circular update */

          sbDstAM1 = 0; /* Linear Update */
          sbSrcAM1 = (sbAM1 == 0 ) ? 0 : ( (sbAM1 == 1) ? circMask0 : circMask1  );/* Circular update */
        }
        else
        {
          sbDstAM0 = 0;
          sbSrcAM0 = 0;
          sbDstAM1 = 0;
          sbSrcAM1 = 0;
        }
      }

      loopCnt1Reset = 0;    /*These are set as per source ICNTx*/
      loopCnt2Reset = 0;
      /*Set up interim buffers for non-compression/decompression flow. Direct copy for  compression/decompression flow.
         if we are compressing/decompressing then we don't need an interim buffer. Just write compressed data to the destination.
         Setup of dstPtr moved inside the while loop
      */
      switch ( DFMT )
      {
        case DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
        {
          break;
        }
        case DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
        {
          break;
        }
        default:
        {
          /* allocate worst case, actual buffer used will depend on trugerType */
          ELYPE = CSL_FEXT(origTransferRecord->fmtflags, UDMAP_TR_FMTFLAGS_ELYPE);
          #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            uint32_t interimBufSize = (origTransferRecord->icnt0 * origTransferRecord->icnt1 * origTransferRecord->icnt2 *
                                        origTransferRecord->icnt3);
            if (( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1) ||
                ( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2))
            {
              interimBufSize *= 2; // As icnt0 is in elements in case of 16 bit to 8 bit conversion
            }
            interimBuffer = (uint8_t *)malloc(interimBufSize);
            dstPtr = interimBuffer;
            dstPtr16 = (uint16_t *)interimBuffer;
          #endif
          break;
        }
      }

      totalSrcCnt = nextTransferRecord->icnt0 * nextTransferRecord->icnt1 * nextTransferRecord->icnt2 * nextTransferRecord->icnt3;
      totalDstCnt = nextTransferRecord->dicnt0 * nextTransferRecord->dicnt1 * nextTransferRecord->dicnt2 * nextTransferRecord->dicnt3;

      srcLoopExitCondition  = ( totalSrcCnt < totalDstCnt )? totalSrcCnt : totalDstCnt;
      dstLoopExitCondition  = srcLoopExitCondition;
      /* Set the minimum value of icnt3 for both src and dst as TR completes whenever anyone of them  is exauhsted */
      /* Transfer source data to a intermediate linear buffer */

      while (1)
      {
        uint8_t* sbCdbPtr;  /*stores the pointer to cdb table entry corresponding to a SB being encoded or decoded.*/
        uint8_t* sbAddr;    /*stores the pointer to to a uncomressed SB data being encoded or decoded. */
        if ( DFMT == DMAUTILSAUTOINC3D_DFMT_COMP )    /*TODO decompression CSL definitions don't exist yet...*/
        {
          srcPtr = (uint8_t *)nextTransferRecord->addr;
          cdbPtr = (uint8_t *)nextTransferRecord->daddr;
          dstPtr = (uint8_t *)hostEmulation_addressUpdate(sectrPtr->addr , ((int32_t *)cdbPtr)[1], 0);

          sbCdbPtr    = cdbPtr;
          sbAddr      = srcPtr;    /*Stores the base pointer for a SB row*/
        }
        else if ( DFMT == DMAUTILSAUTOINC3D_DFMT_DECOMP )  /*TODO decompression CSL definitions don't exist yet...*/
        {
          cdbPtr = (uint8_t *)  nextTransferRecord->addr;
          sbCdbPtr    = cdbPtr;
          dstPtr = (uint8_t *)  nextTransferRecord->daddr;
          sbAddr = dstPtr;
        }
        else
        {
          srcPtr = (uint8_t *)nextTransferRecord->addr;
          if ( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
            srcPtr16 = (uint16_t *)srcPtr;
          #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            dstPtr = (uint8_t *)nextTransferRecord->daddr;
            if ( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
               dstPtr16 = (uint16_t *)dstPtr;
          #endif
        }

        for (icnt0 = 0; icnt0 < nextTransferRecord->icnt0; icnt0++)
        {
          if ( DFMT == DMAUTILSAUTOINC3D_DFMT_COMP )/* TODO compression CSL definition doesn't exist yet...*/
          {
            uint64_t cmprs       = 0;
            uint8_t  len         = 0;
            uint16_t cdbCnt      = 0;
            uint8_t* sb1Addr     = sbAddr;    /*Stores the base pointer for a SB row being encoded/decoded*/
            int32_t  startOffset = (int32_t)((uint64_t)dstPtr - sectrPtr->addr);
            /*Implicit EOB for both cases, filling buffers based on
              H/W behaviour:*/
            if ( cmprsType == DMAUTILSAUTOINC3D_CMPRS_ZERO )
            {
              memset(dstPtr, 0x0, 16);
            }
            else
            {
              memset(dstPtr, 0, 16);
            }
            uint32_t  sbHeight = ((sectrPtr->data[1] >> 16) & 0xFFFF);
            uint32_t  sbWidth  = 16 * ((sectrPtr->data[1] & 0xFFFF));
            for (sbIcnt1 = 0; sbIcnt1 < sbHeight; sbIcnt1++)
            {
              srcPtr = sb1Addr;
              for (sbIcnt0 = 0; sbIcnt0 < sbWidth; sbIcnt0++)
              {
                uint8_t newLen = DmaUtilsAutoInc3d_compressElem( sectrPtr , (uint32_t)(*srcPtr) , &cmprs );
                /*merge compressed data into output*/

                if ( len + newLen > 128 ) /*This won't fit in the current CDB*/
                {
                  if ( cmprsType == DMAUTILSAUTOINC3D_CMPRS_ZERO )
                  {
                    *(dstPtr + (len >> 3)) |= ( 1 << (len & 0x7) );
                  }
                  dstPtr += 16;
                  /*Implicit EOB for both cases, filling buffers based on
                    H/W behaviour:*/
                  if ( cmprsType == DMAUTILSAUTOINC3D_CMPRS_ZERO )
                  {
                    memset(dstPtr, 0, 16);
                  }
                  else
                  {
                    memset(dstPtr, 0, 16);
                  }
                  len = 0;
                  cdbCnt++;
                }

                /*align compressed data starting bit with the output data*/
                cmprs <<=  len & 0x7;

                /*deal with compressed data endianness agnostically*/
                uint8_t touchedBytes = (newLen + (len & 0x7)) >> 3;
                for ( i = 0; i <= touchedBytes; i++ )
                {
                  uint8_t off  = (len >> 3) + i;
                  uint8_t elem = (cmprs >> (i << 3)) & 0xFF;
                  *(dstPtr + off) |= elem;
                }
                len   += newLen;
                srcPtr = (uint8_t*)hostEmulation_addressUpdate((uint64_t)srcPtr , 1 , sbSrcAM0);
              }
              sb1Addr = (uint8_t*)hostEmulation_addressUpdate((uint64_t)sb1Addr , sbDim1 , sbSrcAM1);
            }
            /*
              finish last CDB in superblock
              eob only has to be written for zero compression. Otherwise it's zero
            */
            if ( cmprsType == DMAUTILSAUTOINC3D_CMPRS_ZERO )
            {
              *(dstPtr + (len >> 3)) |= ( 1 << (len & 0x7) );
            }
            dstPtr += 16;
            /*zero out next CDB for writing*/
            memset(dstPtr, 0, 16);
            len = 0;
            cdbCnt++;

            /*Write CDB info to table*/
            ( (uint32_t*)sbCdbPtr )[0] = cdbCnt;
            ( (int32_t *)sbCdbPtr )[1] = startOffset;
            nextSBStartOffset = startOffset + (cdbCnt * 16);
            sbCdbPtr += dDim0;
            sbAddr  = (uint8_t*)hostEmulation_addressUpdate((uint64_t)sbAddr, sDim0, srcAM0);
          }
          else if ( DFMT == DMAUTILSAUTOINC3D_DFMT_DECOMP ) /*TODO decompression CSL definitions don't exist yet...*/
          {
            uint64_t cmprs         = 0;
            uint32_t decmprs       = 0;
            uint8_t  cdbLen        = 128;
            uint8_t  maxSymbolSize = 17; /*default for UEG/SEG*/
            uint8_t* sb1Addr       = sbAddr;
            int32_t srcJump       = 0;
            uint32_t curBitPosition  = 0;

            switch( cmprsType )
            {
              case DMAUTILSAUTOINC3D_CMPRS_ZERO :
              {
                maxSymbolSize = 16; /*actual = 9*/
                break;
              }
              case DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K :
                maxSymbolSize = 17; /*TODO maxSymbolSize = (elemsize * 2) + 1*/
              case DMAUTILSAUTOINC3D_CMPRS_UEG :
              case DMAUTILSAUTOINC3D_CMPRS_SEG :
              {
                maxSymbolSize = (maxSymbolSize + 7) & ~0x7; /*round up to nearest multiple of 8*/
                break;
              }
              default:
              {
                break;
              }
            }
            /*The actual max size is always 1 more than a multiple of 8, so when we drop up to 7 bits during alignment we will still have enough data*/

            /*get start of superblock*/
            srcPtr = (uint8_t*)hostEmulation_addressUpdate(sectrPtr->addr, ((int32_t*)sbCdbPtr )[1], 0); /*srcAM0 ==> 0?*/

            uint32_t  sbHeight = ((sectrPtr->data[1] >> 16) & 0xFFFF);
            uint32_t  sbWidth  = 16 * ((sectrPtr->data[1] & 0xFFFF));
            uint8_t   bias      =  ( (sectrPtr->data[0]) >> 8 ) & 0xFF;

            for (sbIcnt1 = 0; sbIcnt1 < sbHeight; sbIcnt1++)
            {
              dstPtr = sb1Addr;
              for (sbIcnt0 = 0; sbIcnt0 < sbWidth; )
              {

                if(cmprsType == DMAUTILSAUTOINC3D_CMPRS_ZERO)
                {
                  /*CDB Decode, and dstPtr update happens within this function:*/
                  srcJump = DmaUtilsAutoInc3d_decompressZeroBasedCDB(srcPtr, &dstPtr, sbDstAM0, &sbIcnt0, sbWidth, &curBitPosition, bias);
                  //Move to the next CDB:
                  srcPtr += srcJump;
                  if(srcJump != 0)
                  {
                    //Move to next CDB:
                    curBitPosition = 0;
                  }
                }
                else
                {
                  /*get at least one symbol of data in this CDB and be endianess agnostic*/
                  cmprs = 0;
                  uint8_t len = (cdbLen > maxSymbolSize) ? maxSymbolSize  : cdbLen;		// COpy
                  len         = ( len & 0x7 )            ? (len >> 3) + 1 : (len >> 3);
                  uint8_t off = (128 - cdbLen) >> 3;

                  /*printf("maxSymbolSize = %d : cdbLen = %d : len = %d : off = %d :\n",maxSymbolSize,cdbLen,len,off);*/
                  for ( i = 0; i < len; i++ )
                  {
                    cmprs |= *(srcPtr + off + i) << (i << 3);
                  }

                  /*align data*/
                  if (cdbLen & 0x7)
                  {
                    cmprs = cmprs >> (8 - (cdbLen & 0x7));
                  }

                  /*check for eob symbol*/
                  bool eobFound = false;
                  switch( cmprsType )
                  {
                    case DMAUTILSAUTOINC3D_CMPRS_ZERO :
                    {
                      eobFound = (cmprs & ((1<<eobSize)-1)) == 1;
                      break;
                    }
                    case DMAUTILSAUTOINC3D_CMPRS_UEG :
                    case DMAUTILSAUTOINC3D_CMPRS_SEG :
                    case DMAUTILSAUTOINC3D_CMPRS_SEG_VAR_K : /*TODO varK symbol is (element bits + 1) consecutive zeros*/
                    {
                      eobFound = (cmprs & ((1<<eobSize)-1)) == 0;
                      break;
                    }
                    default :
                    {
                      break;
                    }
                  }

                  if (eobFound)
                  {
                    /*printf("EOB found!\n");*/
                    srcPtr += 16; /*move to next CDB*/
                    cdbLen  = 128;
                  }
                  else
                  {
                    cdbLen -= DmaUtilsAutoInc3d_decompressElem( sectrPtr , cmprs , &decmprs );
                    /*TODO varK can have non-1B elements...*/
                    *dstPtr = decmprs & 0xFF;
                    dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr , 1 , sbDstAM0);
                    sbIcnt0++; /*only move on if we decompressed an actual element*/
                  }
                }

              }

              sb1Addr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)sb1Addr , sbDim1 , sbDstAM1);
            }

            sbAddr  = (uint8_t *)hostEmulation_addressUpdate((uint64_t)sbAddr , dDim0 , dstAM0); /*Move outside?*/
            sbCdbPtr += sDim0;
          }
          else
          {

            if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
            {  *dstPtr16 = *srcPtr;
			        srcPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)srcPtr, 1, srcAM0);
			        #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
  	              dstPtr16++;
			        #else
			  	      dstPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)dstPtr16, 2, dstAM0);
			        #endif
            }
            else if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
            {
              #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
              *dstPtr16 = *srcPtr16;
              dstPtr16++;
              #else
                *dstPtr = (uint8_t)*srcPtr16;
                dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
              #endif
              srcPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)srcPtr16, 2, srcAM0);
            }
            else
            {
              *dstPtr = *srcPtr;
              srcPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)srcPtr, 1, srcAM0);
              #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
                dstPtr++;
              #else
                dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
              #endif
            }
          }
        }
        nextTransferRecord->icnt1--;
        if( DFMT != DMAUTILSAUTOINC3D_DFMT_COMP && DFMT != DMAUTILSAUTOINC3D_DFMT_DECOMP)
        {
          #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            nextTransferRecord->dicnt1--;
          #endif
        }
        else
        {
          nextTransferRecord->dicnt1--;
        }

        switch( DFMT )
        {
          case DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
          {
            /*update CDB table address*/
            nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord->daddr , nextTransferRecord->ddim1, 0);
            /*move source pointer*/
            nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord->addr  , nextTransferRecord->dim1,   srcAM1);
            break;
          }
          case DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
          {
            /*update CDB table address*/
            nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord->addr  , nextTransferRecord->dim1, 0);
            /*move destination pointer*/
            nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord->daddr , nextTransferRecord->ddim1,   dstAM1);
            break;
          }
          default :
          {
            nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord->addr  , nextTransferRecord->dim1,   srcAM1);
            #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            	nextTransferRecord->daddr   = hostEmulation_addressUpdate(nextTransferRecord->daddr  , nextTransferRecord->ddim1,   dstAM1);
            #endif
            break;
          }
        }
        /*For compression/decompression type 9 transfer currently we assume that icnt0 = dicnt0*/

        if ( nextTransferRecord->icnt1 == 0)
        {
          loopCnt1Reset = 1;
          nextTransferRecord->icnt2--;
          nextTransferRecord->icnt1 = origTransferRecord->icnt1;
		      switch( DFMT )
          {
            case DMAUTILSAUTOINC3D_DFMT_COMP :
            {
              /*move source pointer*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord1->addr  , nextTransferRecord->dim2,   srcAM2);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              break;
            }
            case DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
            {
              /*update CDB table address*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord1->addr  , nextTransferRecord->dim2, 0);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              break;
            }
            default :
            {
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord1->addr  , nextTransferRecord->dim2,   srcAM2);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              break;
            }
          }
        }

        if ( nextTransferRecord->dicnt1 == 0)
        {
	        if( DFMT != DMAUTILSAUTOINC3D_DFMT_COMP && DFMT != DMAUTILSAUTOINC3D_DFMT_DECOMP)
          {
         	 #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
            	nextTransferRecord->dicnt2--;
				      nextTransferRecord->dicnt1 = origTransferRecord->dicnt1;
          	 #endif
          }
          else
          {
          	nextTransferRecord->dicnt2--;
			      nextTransferRecord->dicnt1 = origTransferRecord->dicnt1;
          }
		      switch( DFMT )
          {
            case DMAUTILSAUTOINC3D_DFMT_COMP :
            {
              /*update CDB table address*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord1->daddr , nextTransferRecord->ddim2, 0);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              break;
            }
            case DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
            {
              /*move destination pointer*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord1->daddr , nextTransferRecord->ddim2,   dstAM2);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              break;
            }
            default :
            {
              #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
                nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord1->daddr , nextTransferRecord->ddim2,   dstAM2);
                nextTransferRecord1->daddr = nextTransferRecord->daddr;
              #endif
              break;
            }
          }
        }

        if ( nextTransferRecord->icnt2 == 0)
        {
          loopCnt2Reset= 1;
          nextTransferRecord->icnt3--;
          nextTransferRecord->icnt2 = origTransferRecord->icnt2;

          switch( DFMT )
          {
            case DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
            {
              /*update CDB table address*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord2->addr  , nextTransferRecord->dim3,   srcAM3);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              nextTransferRecord2->addr  = nextTransferRecord->addr;
              break;
            }
            case DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
            {
              /*update CDB table address*/
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord2->addr  , nextTransferRecord->dim3, 0);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              nextTransferRecord2->addr  = nextTransferRecord->addr;
              break;
            }
            default :
            {
              nextTransferRecord->addr   = hostEmulation_addressUpdate(nextTransferRecord2->addr  , nextTransferRecord->dim3,   srcAM3);
              nextTransferRecord1->addr  = nextTransferRecord->addr;
              nextTransferRecord2->addr  = nextTransferRecord->addr;
              break;
            }
          }
        }

        if ( nextTransferRecord->dicnt2 == 0)
        {

	  if( DFMT != DMAUTILSAUTOINC3D_DFMT_COMP && DFMT != DMAUTILSAUTOINC3D_DFMT_DECOMP)
          {
         	  #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
			        nextTransferRecord->dicnt3--;
              nextTransferRecord->dicnt2 = origTransferRecord->dicnt2;
          	#endif
          }
          else
          {
            nextTransferRecord->dicnt3--;
            nextTransferRecord->dicnt2 = origTransferRecord->dicnt2;
          }
          switch( DFMT )
          {
            case DMAUTILSAUTOINC3D_DFMT_COMP : /*compression*/
            {
              /*update CDB table address*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord2->daddr , nextTransferRecord->ddim3, 0);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              nextTransferRecord2->daddr = nextTransferRecord->daddr;
              break;
            }
            case DMAUTILSAUTOINC3D_DFMT_DECOMP : /*decompression*/
            {
              /*move destination pointer*/
              nextTransferRecord->daddr  = hostEmulation_addressUpdate(nextTransferRecord2->daddr , nextTransferRecord->ddim3,   dstAM3);
              nextTransferRecord1->daddr = nextTransferRecord->daddr;
              nextTransferRecord2->daddr = nextTransferRecord->daddr;
              break;
            }
            default :
            {
              #if !USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
                nextTransferRecord->daddr   = hostEmulation_addressUpdate(nextTransferRecord2->daddr  , nextTransferRecord->ddim3,   dstAM3); //Validate
                nextTransferRecord1->daddr  = nextTransferRecord->daddr;
                nextTransferRecord2->daddr  = nextTransferRecord->daddr;
              #endif
              break;
            }
          }
        }

        /*Updating the start offset for next SB to be read in next iteration of the while loop or next trigger*/
        if ( DFMT == DMAUTILSAUTOINC3D_DFMT_COMP && nextTransferRecord->icnt3 != 0 && nextTransferRecord->dicnt3 != 0)
        {
          ((int32_t *)nextTransferRecord->daddr)[1] = nextSBStartOffset;
        }

        if ( nextTransferRecord->icnt3 == 0 || nextTransferRecord->dicnt3 == 0)
        {
          CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
          if ( DFMT == DMAUTILSAUTOINC3D_DFMT_COMP)
          {
            totalSrcCnt = origTransferRecord->icnt0 * origTransferRecord->icnt1 * origTransferRecord->icnt2 * origTransferRecord->icnt3;
            totalDstCnt = origTransferRecord->dicnt0 * origTransferRecord->dicnt1 * origTransferRecord->dicnt2 * origTransferRecord->dicnt3;
            nextSBStartOffset = 0;
          }
          break;
        }


        if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC )
        {
          break;
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC )
        {
          if ( loopCnt1Reset == 1)
          {
            nextTransferRecord1->addr   = nextTransferRecord->addr;
            break;
          }
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC )
        {
          if ( loopCnt2Reset == 1)
          {
            nextTransferRecord2->addr   = nextTransferRecord->addr;
            break;
          }
        }
        else
        {
          /*Indicates 4D sync,  Just continue */
        }
        srcLoopExitCondition--;
        if ( srcLoopExitCondition == 0)
        {
          nextTransferRecord->icnt3 = 0;
          CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
          break;
        }
      }
    #if USE_INTERIMBUFFER_FOR_NONCOMPRESSED_TR
      if (DFMT != DMAUTILSAUTOINC3D_DFMT_COMP && DFMT != DMAUTILSAUTOINC3D_DFMT_DECOMP)
      {
        loopCnt1Reset = 0;
        loopCnt2Reset = 0;
        srcPtr = interimBuffer;
        srcPtr16 = (uint16_t *)interimBuffer;
         /* Now copy the intermediate data to the destination buffer */
        while (1)
         {
        if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2)
        {
          dstPtr16 = (uint16_t *)nextTransferRecord->daddr;
          for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
          {
            *dstPtr16 = (uint16_t)(*srcPtr16);
            srcPtr16++;
            dstPtr16 = (uint16_t *)hostEmulation_addressUpdate((uint64_t)dstPtr16, 2, dstAM0);
          }
        }
        else if( ELYPE == CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1)
        {
          dstPtr = (uint8_t *)nextTransferRecord->daddr;
          for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
          {
            *dstPtr = (uint8_t)(*srcPtr16);
              srcPtr16++;
              dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
          }
        }
        else
        {
          dstPtr = (uint8_t *)nextTransferRecord->daddr;

          for (icnt0 = 0; icnt0 < nextTransferRecord->dicnt0; icnt0++)
          {
            *dstPtr = *srcPtr;
             *srcPtr++;
              dstPtr = (uint8_t *)hostEmulation_addressUpdate((uint64_t)dstPtr, 1, dstAM0);
          }
        }
        nextTransferRecord->dicnt1--;

        nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord->daddr, nextTransferRecord->ddim1, dstAM1);

        if ( nextTransferRecord->dicnt1 == 0)
        {
          loopCnt1Reset = 1;
          nextTransferRecord->dicnt2--;
          nextTransferRecord->dicnt1 = origTransferRecord->dicnt1;

          nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord1->daddr, nextTransferRecord->ddim2, dstAM2);
          nextTransferRecord1->daddr = nextTransferRecord->daddr;
        }

        if ( nextTransferRecord->dicnt2 == 0)
        {
          loopCnt2Reset= 1;
          nextTransferRecord->dicnt3--;
          nextTransferRecord->dicnt2 = origTransferRecord->dicnt2;

          nextTransferRecord->daddr = hostEmulation_addressUpdate(nextTransferRecord2->daddr, nextTransferRecord->ddim3, dstAM3);

          nextTransferRecord1->daddr = nextTransferRecord->daddr;
          nextTransferRecord2->daddr = nextTransferRecord->daddr;
        }

        if ( nextTransferRecord->dicnt3 == 0)
        {
          CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
          break;
        }

        if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC )
        {
          break;
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC )
        {
          if ( loopCnt1Reset == 1)
          {
            break;
          }
        }
        else if ( triggerType == CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC )
        {
          if ( loopCnt2Reset == 1)
          {
            break;
          }
        }

          dstLoopExitCondition--;
          if ( dstLoopExitCondition == 0)
          {
            nextTransferRecord->dicnt3 = 0;
            CSL_REG64_FINS(&druRegs->CHRT[chId].CHRT_CTL, DRU_CHRT_CTL_ENABLE, 0);
            break;
          }
        } /*while(1)*/
      } /*if (not compression)*/
    #endif
      if (( nextTransferRecord->icnt3 == 0 ) || ( nextTransferRecord->dicnt3 == 0 ))
      {
#if ( DMA_UTILS_STANDALONE == 0 )
        /* this indicates that TR is received from PSIL */
        if ( (druRegs->CHNRT[chId].CFG & CSL_DRU_CHNRT_CFG_CHAN_TYPE_OWNER_MASK) != 0 )
        {
           CSL_UdmapCppi5TRPD * trDescriptor;
           uint64_t * ringPtr;
           uint64_t currTr;
           uint64_t numTr;
           CSL_UdmapTR           *pTr;
           /* Get Ring Memory Pointer */
          ringPtr = (uint64_t *) ((uint64_t)((uint64_t)ringAccCfgRegs->RING[udmapRegs->txChanCnt + chId].BA_HI << 32) |
                                            ringAccCfgRegs->RING[udmapRegs->txChanCnt + chId].BA_LO);

           trDescriptor = (CSL_UdmapCppi5TRPD *) (*ringPtr);

           numTr = CSL_udmapCppi5GetPktLen(trDescriptor);
           /* Use this field to track the TR, For the target build this would be handled by hardware */
           /* In real hardware this will not be like this it is done just for host emulation*/
           currTr =  druRegs->CHATOMIC[chId].DEBUG[1].NEXT_TR_WORD0_1;

            if ( currTr < numTr)
            {
               currTr++;
               pTr = (CSL_UdmapTR *)( (uint8_t *)trDescriptor + sizeof(CSL_UdmapTR) * currTr);

               /* Update both original and next transfer record by reading the TR from the TR descriptor */
              hostEmulation_druChSubmitAtomicTr(druRegs, chId, (void *) pTr);

               druRegs->CHATOMIC[chId].DEBUG[1].NEXT_TR_WORD0_1 = currTr;

            }
        }
#endif
      }
      if ( interimBuffer != NULL)
      {
        free(interimBuffer);
      }
    }
  }
}
#endif

