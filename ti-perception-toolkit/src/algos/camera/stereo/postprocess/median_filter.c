/*
*
* Copyright (c) 2020 Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include <math.h>
#include <string.h>
#include <stdio.h>

#include <perception/algos/stereo/postprocess/median_filter.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))

void PTK_Alg_StereoPP_MedianFilterSetParams(PTK_Alg_StereoPP_MedianFilter_configParams * cfg,
                                            PTK_Alg_StereoPP_MedianFilterObj           * cntxt)
{
    /* initialize parameters */
    cntxt->cfgParams.width       = cfg->width;
    cntxt->cfgParams.height      = cfg->height;

    cntxt->cfgParams.filterSize  = cfg->filterSize;
}


int32_t PTK_Alg_StereoPP_MedianFilterConfig(PTK_Alg_StereoPP_MedianFilter_configParams * cfg,
                                            PTK_Api_MemoryReq                          * memReq)
{
    uint32_t memSize;

    PTK_Alg_StereoPP_MedianFilterObj cntxt;
    cntxt.cfgParams = *cfg;

    memReq->numBlks = 2;

    /* Block 0 is for the internal context. */
    memReq->blks[0].size    = sizeof(PTK_Alg_StereoPP_MedianFilterObj);
    memReq->blks[0].memType = PTK_Api_MemoryType_LVL4;

    /* Block1 is for the memory needed by the algorithm */
    memSize = PTK_Alg_StereoPP_MedianFilterGetMem(&cntxt, 0);
    memReq->blks[1].size    = memSize;
    memReq->blks[1].memType = PTK_Api_MemoryType_LVL4;


    return PTK_ALG_RET_SUCCESS;
}


PTK_AlgHandle PTK_Alg_StereoPP_MedianFilterInit(PTK_Alg_StereoPP_MedianFilter_configParams * cfg,
                                                const PTK_Api_MemoryRsp                    * memRsp)
{
    PTK_Alg_StereoPP_MedianFilterObj  * cntxt;

    /* Validate the input parameters. */
    if (!cfg                  ||
        !memRsp               ||
        (memRsp->numBlks < 2) ||
        !memRsp->blks[0].mem  ||
        !memRsp->blks[1].mem)
    {
        return NULL;
    }

    /* Initialize the context. */
    cntxt = (PTK_Alg_StereoPP_MedianFilterObj *)memRsp->blks[0].mem;
    cntxt->cfgParams = *cfg;

    PTK_Alg_StereoPP_MedianFilterGetMem(cntxt, memRsp->blks[1].mem);

    return cntxt;
}


int32_t PTK_Alg_StereoPP_MedianFilterGetMem(PTK_Alg_StereoPP_MedianFilterObj * cntxt,  uint8_t *memPtr)
{
    int32_t  memSize = 0;

    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoPP_MedianFilter_configParams * cfg = &cntxt->cfgParams;

    if (pMem)
    {
        cntxt->buffer = (int16_t *)(pMem + memSize);
    }
    memSize += sizeof(int16_t)*(cfg->filterSize * cfg->filterSize);
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->tmpBuffer = (int16_t *)(pMem + memSize);
    }
    memSize += sizeof(int16_t)*(cfg->filterSize * cfg->filterSize);
    memSize = align(memSize, 8);


    return memSize;
}


void PTK_Alg_StereoPP_mergeSort(int16_t *a, int16_t *b, int16_t num)
{
    int16_t rght, rend;
    int16_t i, j, k, m, left;

    for (k = 1; k < num; k *= 2)
    {
        for (left = 0; left+k < num; left += k*2)
        {
            rght = left + k;
            rend = rght + k;

            if (rend > num) 
            { 
                rend = num;
            }

            m = left;
            i = left;
            j = rght;
            
            while (i < rght && j < rend) 
            {
                if (a[i] <= a[j])
                {
                    b[m] = a[i];
                    i++;
                } else 
                {
                    b[m] = a[j];
                    j++;
                }
                m++;
            }

            while (i < rght)
            {
                b[m]=a[i];
                i++; m++;
            }
            while (j < rend)
            {
                b[m]=a[j];
                j++; m++;
            }

            for (m = left; m < rend; m++)
            {
                a[m] = b[m];
            }
        }
    }
}

void PTK_Alg_StereoPP_runMedianFilter(PTK_Alg_StereoPP_MedianFilterObj * cntxt,
                                      int16_t                          * disparityBuffer,
                                      int16_t                          * outDisparityBuffer,
                                      int16_t                            stride) 
{
    int16_t i, j, k, l, x, u, v, w;

    int16_t numZeros;
    int16_t numValidD;

    int16_t medianValue;
    int16_t minSupport;

    int16_t horzStop;
    int16_t vertStop;

    int16_t width    = cntxt->cfgParams.width;
    int16_t height   = cntxt->cfgParams.height;

    int16_t n        = (int16_t)(cntxt->cfgParams.filterSize);
    int16_t n2       = n >> 1;

    int16_t * buffer = cntxt->buffer;
    int16_t * tmp    = cntxt->tmpBuffer;

    int16_t * disparity    = disparityBuffer;
    int16_t * outDisparity = outDisparityBuffer;

    horzStop   = width - n2*2;
    vertStop   = height - n2*2;
    minSupport = (n >> 1) * n;

    for (j = 0; j < vertStop; j++)
    {
        for (i = 0; i < horzStop; i++)
        {
            x = 0;
            numValidD = 0;
            numZeros  = 0;

            for (k = 0; k < n; k++)
            {
                for (l = 0; l < n; l++)
                {
                    buffer[x] = (disparity[(j + k)*stride + i + l] >> 3) & 0xFFF;

                    if (buffer[x] != 0)
                    {
                        numValidD++;
                    } else
                    {
                        numZeros++;
                    }

                    x++;
                }
            }

            if (numValidD >= minSupport)
            {
                w = n*n - numZeros;
                u = (n*n) - 1 - (w >> 1);
                v = u - !(w & 0x1);

                PTK_Alg_StereoPP_mergeSort(buffer, tmp, n*n);

                medianValue = (buffer[u] + buffer[v] + 1) / 2;
                outDisparity[(j + n2)*stride + i + n2] = (medianValue << 3) + 7;
            } else
            {
                outDisparity[(j + n2)*stride + i + n2] = disparity[(j + n2)*stride + i + n2];
            }
        }
    }

    return;
}
