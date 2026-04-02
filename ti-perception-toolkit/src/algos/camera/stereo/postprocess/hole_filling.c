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

#include <perception/algos/stereo/postprocess/hole_filling.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))
#define MAX(a,b) ((a > b) ? (a) : (b))


void PTK_Alg_StereoPP_HoleFillingSetParams(PTK_Alg_StereoPP_HoleFilling_configParams * cfg,
                                           PTK_Alg_StereoPP_HoleFillingObj           * cntxt)
{
    /* initialize parameters */
    cntxt->cfgParams.width        = cfg->width;
    cntxt->cfgParams.height       = cfg->height;

    cntxt->cfgParams.minDisparity = cfg->minDisparity;
    cntxt->cfgParams.maxDisparity = cfg->maxDisparity;

    cntxt->cfgParams.deltaDispTh  = cfg->deltaDispTh;
    cntxt->cfgParams.gapLengthTh  = cfg->gapLengthTh;
}


int32_t PTK_Alg_StereoPP_HoleFillingConfig(PTK_Alg_StereoPP_HoleFilling_configParams * cfg,
                                           PTK_Api_MemoryReq                         * memReq)
{
    uint32_t memSize;

    PTK_Alg_StereoPP_HoleFillingObj cntxt;
    cntxt.cfgParams = *cfg;

    memReq->numBlks = 2;

    /* Block 0 is for the internal context. */
    memReq->blks[0].size    = sizeof(PTK_Alg_StereoPP_HoleFillingObj);
    memReq->blks[0].memType = PTK_Api_MemoryType_LVL4;

    /* Block1 is for the memory needed by the algorithm */
    memSize = PTK_Alg_StereoPP_HoleFillingGetMem(&cntxt, 0);
    memReq->blks[1].size    = memSize;
    memReq->blks[1].memType = PTK_Api_MemoryType_LVL4;


    return PTK_ALG_RET_SUCCESS;
}


PTK_AlgHandle PTK_Alg_StereoPP_HoleFillingInit(PTK_Alg_StereoPP_HoleFilling_configParams * cfg,
                                               const PTK_Api_MemoryRsp                   * memRsp)
{
	PTK_Alg_StereoPP_HoleFillingObj  * cntxt;

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
    cntxt = (PTK_Alg_StereoPP_HoleFillingObj *)memRsp->blks[0].mem;
    cntxt->cfgParams = *cfg;

    PTK_Alg_StereoPP_HoleFillingGetMem(cntxt, memRsp->blks[1].mem);

    return cntxt;
}


int32_t PTK_Alg_StereoPP_HoleFillingGetMem(PTK_Alg_StereoPP_HoleFillingObj * cntxt,  uint8_t *memPtr)
{
    int32_t  memSize = 0;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoPP_HoleFilling_configParams * cfg = &cntxt->cfgParams;

    if (pMem)
    {
        cntxt->segStart = (int16_t *)(pMem + memSize);
    }
    memSize += sizeof(int16_t)*(cfg->width);
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->segLength = (int16_t *)(pMem + memSize);
    }
    memSize += sizeof(int16_t)*(cfg->width);
    memSize = align(memSize, 8);

    if (pMem)
    {
        cntxt->segAvg = (int16_t *)(pMem + memSize);
    }
    memSize += sizeof(int16_t)*(cfg->width);
    memSize = align(memSize, 8);

    return memSize;
}



void PTK_Alg_StereoPP_runHoleFilling(PTK_Alg_StereoPP_HoleFillingObj * cntxt,
                                     int16_t                         * disparityBuffer,
                                     int16_t                           stride) 
{
    int16_t * segStart    = cntxt->segStart; 
    int16_t * segLength   = cntxt->segLength;
    int16_t * segAvg      = cntxt->segAvg;

    int16_t   width       = cntxt->cfgParams.width;
    int16_t   height      = cntxt->cfgParams.height;
    int16_t   deltaDispTh = cntxt->cfgParams.deltaDispTh;
    int16_t   gapLengthTh = cntxt->cfgParams.gapLengthTh;

    int16_t * disparity;
    int16_t * outDisparity;
    
    int16_t   i, j, k;
    int16_t   numSegments;
    int16_t   segLen, gapLen;
    int16_t   start, end;
    int16_t   leftDisp, rightDisp;
    int16_t   delta;
    int16_t   linearInterpolatedValue;
    int16_t   pixDisp;


    for (j = 0; j < height; j++)
    {
        // initialize
        numSegments = 0;
        segLen      = 0;

        disparity    = disparityBuffer + j * stride;
        outDisparity = disparityBuffer + j * stride;

        memset(segStart,  0, sizeof(int16_t) * width);
        memset(segLength, 0, sizeof(int16_t) * width);
        memset(segAvg,    0, sizeof(int16_t) * width);

        for (i = 0; i < width; i++)
        {
            // get disparity only
            pixDisp = (disparity[i] >> 3) & 0xFFF;

            if ((pixDisp == 0) || (i == width -1))
            {
                if (segLen)
                {
                    segLen                   = i - segStart[numSegments];
                    segLength[numSegments]   = segLen;
                    segAvg[numSegments]     /= segLen;
                    numSegments++;
                } 
                segLen = 0;
                continue;
            }

            if (pixDisp > 0)
            {
                if (segLen == 0)
                {
                    segStart[numSegments] = i;
                    segAvg[numSegments]   = pixDisp;
                    segLen                = 1;
                } else
                {
                    segLen               += 1;
                    segAvg[numSegments]  += pixDisp;
                }
            }
        }

        for (i = 1; i < numSegments; i++)
        {
            gapLen    = segStart[i] - (segStart[i-1] + segLength[i-1]);
            start     = segStart[i-1] + segLength[i-1];
            end       = segStart[i];
            leftDisp  = (disparity[start] >> 3) & 0xFFF; //disparity[start];
            rightDisp = (disparity[end] >> 3) & 0xFFF; //disparity[end];
            delta     = rightDisp - leftDisp;

            if ( (abs(delta) < deltaDispTh) && gapLen < gapLengthTh)
            {
                linearInterpolatedValue = 0;
                if (gapLen) {
                    delta /= gapLen;
                }

                for (k = start; k < end; k++)
                {
                    linearInterpolatedValue += delta;
                    outDisparity[k] = MAX(leftDisp + linearInterpolatedValue, 0);
                    outDisparity[k] = (outDisparity[k] << 3) + 7;  // highest confidence
                }
            }
        }
    }

    return;
}
