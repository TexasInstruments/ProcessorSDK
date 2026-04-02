/******************************************************************************
 * FILE PURPOSE:  MD5 Message Authentication Algorithm
 ******************************************************************************
 * FILE NAME:   salldMd5.c  
 *
 * DESCRIPTION: MD5 as per RFC1321. Implemented from RFC1321 The MD5 
 *                        Message-Digest Algorithm
 *
 * (C) Copyright 2009, Texas Instruments Inc.- http://www.ti.com
 *
 * The following source was taken from cryptographic software written by the below authors
 * and has been modified and adapted by TI
 ******************************************************************************
 * Copyright (C) 1995-1997 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 *****************************************************************************/

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include "src/salldport.h"
#include "salldmd5loc.h"

static uint32_t Md5MacPaddingWord[4] = {
    0x00000080,
    0x00008000,
    0x00800000,
    0x80000000
};

/* Implemented from RFC1321 The MD5 Message-Digest Algorithm
 */
#define INIT_DATA_A (uint32_t)0x67452301L
#define INIT_DATA_B (uint32_t)0xefcdab89L
#define INIT_DATA_C (uint32_t)0x98badcfeL
#define INIT_DATA_D (uint32_t)0x10325476L

/******************************************************************************
 * FUNCTION PURPOSE: MD5 Support Function 
 ******************************************************************************
 * DESCRIPTION: 
 *
 *    void salld_md5_init(         
 *      salldMd5Inst_t *md5Inst)  - MD5 instance
 *
 *
 *
 *
 *****************************************************************************/
void salld_md5_init(salldMd5Inst_t *md5Inst)
{
  md5Inst->A=INIT_DATA_A;
  md5Inst->B=INIT_DATA_B;
  md5Inst->C=INIT_DATA_C;
  md5Inst->D=INIT_DATA_D;
  md5Inst->Nl=0;
  md5Inst->Nh=0;
  md5Inst->num=0;
}

/******************************************************************************
 * FUNCTION PURPOSE: MD5 Core Digest 
 ******************************************************************************
 * DESCRIPTION: 
 *
 *    void md5_block(         
 *      salldMd5Inst_t *md5Inst,   - MD5 instance
 *      uint32_t       *p )        - 16 32bit registers/buffers
 *
 *
 *****************************************************************************/
static void md5_block(salldMd5Inst_t *md5Inst, uint32_t *X)
{
	uint32_t A,B,C,D;

	A=md5Inst->A;
	B=md5Inst->B;
	C=md5Inst->C;
	D=md5Inst->D;

	/* Round 0 */
	R0(A,B,C,D,X[ 0], 7,0xd76aa478L);
	R0(D,A,B,C,X[ 1],12,0xe8c7b756L);
	R0(C,D,A,B,X[ 2],17,0x242070dbL);
	R0(B,C,D,A,X[ 3],22,0xc1bdceeeL);
	R0(A,B,C,D,X[ 4], 7,0xf57c0fafL);
	R0(D,A,B,C,X[ 5],12,0x4787c62aL);
	R0(C,D,A,B,X[ 6],17,0xa8304613L);
	R0(B,C,D,A,X[ 7],22,0xfd469501L);
	R0(A,B,C,D,X[ 8], 7,0x698098d8L);
	R0(D,A,B,C,X[ 9],12,0x8b44f7afL);
	R0(C,D,A,B,X[10],17,0xffff5bb1L);
	R0(B,C,D,A,X[11],22,0x895cd7beL);
	R0(A,B,C,D,X[12], 7,0x6b901122L);
	R0(D,A,B,C,X[13],12,0xfd987193L);
	R0(C,D,A,B,X[14],17,0xa679438eL);
	R0(B,C,D,A,X[15],22,0x49b40821L);
	/* Round 1 */
	R1(A,B,C,D,X[ 1], 5,0xf61e2562L);
	R1(D,A,B,C,X[ 6], 9,0xc040b340L);
	R1(C,D,A,B,X[11],14,0x265e5a51L);
	R1(B,C,D,A,X[ 0],20,0xe9b6c7aaL);
	R1(A,B,C,D,X[ 5], 5,0xd62f105dL);
	R1(D,A,B,C,X[10], 9,0x02441453L);
	R1(C,D,A,B,X[15],14,0xd8a1e681L);
	R1(B,C,D,A,X[ 4],20,0xe7d3fbc8L);
	R1(A,B,C,D,X[ 9], 5,0x21e1cde6L);
	R1(D,A,B,C,X[14], 9,0xc33707d6L);
	R1(C,D,A,B,X[ 3],14,0xf4d50d87L);
	R1(B,C,D,A,X[ 8],20,0x455a14edL);
	R1(A,B,C,D,X[13], 5,0xa9e3e905L);
	R1(D,A,B,C,X[ 2], 9,0xfcefa3f8L);
	R1(C,D,A,B,X[ 7],14,0x676f02d9L);
	R1(B,C,D,A,X[12],20,0x8d2a4c8aL);
	/* Round 2 */
	R2(A,B,C,D,X[ 5], 4,0xfffa3942L);
	R2(D,A,B,C,X[ 8],11,0x8771f681L);
	R2(C,D,A,B,X[11],16,0x6d9d6122L);
	R2(B,C,D,A,X[14],23,0xfde5380cL);
	R2(A,B,C,D,X[ 1], 4,0xa4beea44L);
	R2(D,A,B,C,X[ 4],11,0x4bdecfa9L);
	R2(C,D,A,B,X[ 7],16,0xf6bb4b60L);
	R2(B,C,D,A,X[10],23,0xbebfbc70L);
	R2(A,B,C,D,X[13], 4,0x289b7ec6L);
	R2(D,A,B,C,X[ 0],11,0xeaa127faL);
	R2(C,D,A,B,X[ 3],16,0xd4ef3085L);
	R2(B,C,D,A,X[ 6],23,0x04881d05L);
	R2(A,B,C,D,X[ 9], 4,0xd9d4d039L);
	R2(D,A,B,C,X[12],11,0xe6db99e5L);
	R2(C,D,A,B,X[15],16,0x1fa27cf8L);
	R2(B,C,D,A,X[ 2],23,0xc4ac5665L);
	/* Round 3 */
	R3(A,B,C,D,X[ 0], 6,0xf4292244L);
	R3(D,A,B,C,X[ 7],10,0x432aff97L);
	R3(C,D,A,B,X[14],15,0xab9423a7L);
	R3(B,C,D,A,X[ 5],21,0xfc93a039L);
	R3(A,B,C,D,X[12], 6,0x655b59c3L);
	R3(D,A,B,C,X[ 3],10,0x8f0ccc92L);
	R3(C,D,A,B,X[10],15,0xffeff47dL);
	R3(B,C,D,A,X[ 1],21,0x85845dd1L);
	R3(A,B,C,D,X[ 8], 6,0x6fa87e4fL);
	R3(D,A,B,C,X[15],10,0xfe2ce6e0L);
	R3(C,D,A,B,X[ 6],15,0xa3014314L);
	R3(B,C,D,A,X[13],21,0x4e0811a1L);
	R3(A,B,C,D,X[ 4], 6,0xf7537e82L);
	R3(D,A,B,C,X[11],10,0xbd3af235L);
	R3(C,D,A,B,X[ 2],15,0x2ad7d2bbL);
	R3(B,C,D,A,X[ 9],21,0xeb86d391L);

	md5Inst->A+=(A&0xffffffffL);
	md5Inst->B+=(B&0xffffffffL);
	md5Inst->C+=(C&0xffffffffL);
	md5Inst->D+=(D&0xffffffffL);
}

/******************************************************************************
 * FUNCTION PURPOSE: MD5 Update Function
 ******************************************************************************
 * DESCRIPTION: MD5 function
 *
 *    void salld_md5_update(         
 *      salldMd5Inst_t    *md5Inst,  - MD5 instance
 *      tword             *data,     - message for digesting
 *      uint32_t            len)       - length of message in BYTE
 *
 *****************************************************************************/
void salld_md5_update(salldMd5Inst_t *md5Inst, tword *data, uint32_t len)
{
	uint32_t *p;
	int16_t   i, j, ew,ec,sw;
	uint32_t  l;
    uint32_t  offset=0;

	if (len == 0) return;

    /*  */
	l = (md5Inst->Nl + (len<<3)) & 0xffffffff;
	if (l < md5Inst->Nl) /* overflow */
    {
	    md5Inst->Nh++;
    }
	md5Inst->Nh += (len>>29);
	md5Inst->Nl = l;
    
    /* process the first partial block  */
    if ((j = md5Inst->num))
    {
        p = md5Inst->data;
	    i = j >> 2;
        j &= 0x03;
    
        while ((md5Inst->num < SALLD_MD5_BLOCK_SIZE_IN_BYTE) && len)
        {
            if (j)
            {
                /* Complete the first partial word */
                p[i] |= ((uint32_t)(data[0]) << ((j) * 8));
                if (j == 3)
                {
                    i++;
                    j = 0;
                }
                else
                {
                    j++;
                }
                data++;
                len--;
                md5Inst->num++;
            }
            else if (len >= 4)
            {
                /* Input one 32-bit word at a time */
                p[i] =  ((uint32_t)data[3] << 24) |
                        ((uint32_t)data[2] << 16) |
                        ((uint32_t)data[1] <<  8) |
                        ((uint32_t)data[0] <<  0); 
                i++;      
                data += 4;
                len -= 4;
                md5Inst->num += 4;
            }
            else
            {
                /* final partial word */
                int k;
            
                p[i] = 0;
                for (k = 0; k < (int)len; k++)
                {
                    p[i] |= ((uint32_t)data[k] << ((k)*8));   
                }
                md5Inst->num += (int16_t) len;
                len = 0;              
            }
        }
    
        if (md5Inst->num == SALLD_MD5_BLOCK_SIZE_IN_BYTE)
        {
            p = md5Inst->data;
            md5_block(md5Inst, p);
            md5Inst->num = 0;
        }
    
        if (len == 0)
        {
            return;
        }
    }

    /* we now can process the input data in blocks of MD5_CBLOCK
	 * chars and save the leftovers to md5Inst->data. */
	p=md5Inst->data;
	while (len >= SALLD_MD5_BLOCK_SIZE_IN_BYTE)
	{
		for (sw = (SALLD_MD5_BLOCK/4); sw; sw--, offset += 16)
		{
            *p++ = SALLD_SWAP_UINT32(pktRead32bits_m(data, offset));
            *p++ = SALLD_SWAP_UINT32(pktRead32bits_m(data, offset+4));
            *p++ = SALLD_SWAP_UINT32(pktRead32bits_m(data, offset+8));
            *p++ = SALLD_SWAP_UINT32(pktRead32bits_m(data, offset+12));
		}
		p = md5Inst->data;
		md5_block(md5Inst, p);
		len -= SALLD_MD5_BLOCK_SIZE_IN_BYTE;
	}
	ec = (int16_t)len;
	md5Inst->num = ec;
	ew = (ec >> 2);
	ec &= 0x03;

	for (sw = 0; sw < ew; sw++, offset += 4)
	{
      p[sw] = SALLD_SWAP_UINT32(pktRead32bits_m(data, offset));
    }
    
    if(ec)
    {
      p[sw] = SALLD_SWAP_UINT32((pktRead32bits_m(data, offset) & MacLastWordMask[ec-1]));
    }
}

/******************************************************************************
 * FUNCTION PURPOSE: MD5 Final Stage
 ******************************************************************************
 * DESCRIPTION: Initializes the md5 context
 *
 *    void salld_md5_final(         
 *      salldMd5Inst_t    *md5Inst   - MD5 instance
 *      tword             *md)       - mac location
 *
 *****************************************************************************/
void salld_md5_final(salldMd5Inst_t *md5Inst, tword *md)
{
	int    i,j;
	uint32_t *p;

	p = md5Inst->data;
	j = md5Inst->num;
	i = j >> 2;
    j &= 0x03;
    
    if (j)
    {
        p[i] |= Md5MacPaddingWord[j];
    }
    else
    {
        p[i] = Md5MacPaddingWord[0];
    }
    
	i++;
    
	if (md5Inst->num >= SALLD_MD5_LAST_BLOCK_OFFSET_IN_BYTE)
    {
		for (; i<SALLD_MD5_LBLOCK; i++)
			p[i] = 0;
		md5_block(md5Inst, p);
		i = 0;
	}
    
    /* Final Block */    
	for (; i<(SALLD_MD5_LBLOCK-2); i++)
		p[i]=0;
	p[SALLD_MD5_LBLOCK-2]=md5Inst->Nl;
	p[SALLD_MD5_LBLOCK-1]=md5Inst->Nh;
	md5_block(md5Inst,p);
    
    salld_md5_output(md5Inst, md);
	md5Inst->num=0;
}
