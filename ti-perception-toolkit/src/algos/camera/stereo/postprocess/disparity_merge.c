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

#include <perception/algos/stereo/postprocess/disparity_merge.h>

#define align(x,b) ((x+(b-1)) & ~(b-1))
#define MAX(a,b) ((a > b) ? (a) : (b))

#define BOUNDARY_OFFSET 4

void PTK_Alg_StereoPP_DisparityMergeSetParams(PTK_Alg_StereoPP_DisparityMerge_configParams * cfg,
                                              PTK_Alg_StereoPP_DisparityMergeObj           * cntxt)
{
    /* initialize parameters */
    cntxt->cfgParams.loWidth      = cfg->loWidth;
    cntxt->cfgParams.loHeight     = cfg->loHeight;

    cntxt->cfgParams.hiWidth      = cfg->hiWidth;
    cntxt->cfgParams.hiHeight     = cfg->hiHeight;

    cntxt->cfgParams.diffDispTh   = cfg->diffDispTh;
}


int32_t PTK_Alg_StereoPP_DisparityMergeConfig(PTK_Alg_StereoPP_DisparityMerge_configParams * cfg,
                                              PTK_Api_MemoryReq                            * memReq)
{
    uint32_t memSize;

    PTK_Alg_StereoPP_DisparityMergeObj cntxt;
    cntxt.cfgParams = *cfg;

    memReq->numBlks = 2;

    /* Block 0 is for the internal context. */
    memReq->blks[0].size    = sizeof(PTK_Alg_StereoPP_DisparityMergeObj);
    memReq->blks[0].memType = PTK_Api_MemoryType_LVL4;

    /* Block1 is for the memory needed by the algorithm */
    memSize = PTK_Alg_StereoPP_DisparityMergeGetMem(&cntxt, 0);
    memReq->blks[1].size    = memSize;
    memReq->blks[1].memType = PTK_Api_MemoryType_LVL4;


    return PTK_ALG_RET_SUCCESS;
}


PTK_AlgHandle PTK_Alg_StereoPP_DisparityMergeInit(PTK_Alg_StereoPP_DisparityMerge_configParams * cfg,
                                                  const PTK_Api_MemoryRsp                      * memRsp)
{
	PTK_Alg_StereoPP_DisparityMergeObj  * cntxt;

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
    cntxt = (PTK_Alg_StereoPP_DisparityMergeObj *)memRsp->blks[0].mem;
    cntxt->cfgParams = *cfg;

    PTK_Alg_StereoPP_DisparityMergeGetMem(cntxt, memRsp->blks[1].mem);

    return cntxt;
}


int32_t PTK_Alg_StereoPP_DisparityMergeGetMem(PTK_Alg_StereoPP_DisparityMergeObj * cntxt,  uint8_t *memPtr)
{
    int32_t  memSize = 0;
    uint8_t *pMem = (uint8_t *) memPtr;

    PTK_Alg_StereoPP_DisparityMerge_configParams * cfg = &cntxt->cfgParams;


    if (pMem)
    {
        cntxt->upDisparity = (int16_t *)(pMem + memSize);
    }
    memSize += sizeof(int16_t)*(cfg->hiWidth * cfg->hiHeight);
    memSize = align(memSize, 8);

    /* initialize memory */
    if (pMem)
    {
        memset((void *)cntxt->upDisparity, 0, sizeof(int16_t)*cfg->hiWidth * cfg->hiHeight);
    }

    return memSize;
}


/* outDipsariyBuffer should be initialized to 0 */
void PTK_Alg_StereoPP_mergeDisparity(PTK_Alg_StereoPP_DisparityMergeObj * cntxt,
                                     int16_t * disparityLowBuffer,
                                     int16_t * disparityHighBuffer,
                                     int16_t * outDisparityBuffer,
                                     int16_t   lowBufferStride,
                                     int16_t   highBufferStride)
{
    int16_t   i, j;

    int16_t * highDisparity;
    int16_t * outDisparity;
    int16_t   outPixD;

    int16_t * upDisparity   = cntxt->upDisparity;
    int16_t   diffDispTh    = cntxt->cfgParams.diffDispTh;

    PTK_Alg_StereoPP_upsampleDisparity(disparityLowBuffer, cntxt->upDisparity, 
                                       cntxt->cfgParams.loWidth, cntxt->cfgParams.loHeight, lowBufferStride,
                                       cntxt->cfgParams.hiWidth, cntxt->cfgParams.hiHeight, highBufferStride,
                                       cntxt->cfgParams.loDispOnlyFlag);

    for (j = 0; j < cntxt->cfgParams.hiHeight; j++)
    {
        highDisparity = disparityHighBuffer + j * highBufferStride;
        upDisparity   = cntxt->upDisparity  + j * highBufferStride;
        outDisparity  = outDisparityBuffer  + j * highBufferStride;

        for (i = 0; i < cntxt->cfgParams.hiWidth; i++)
        {
            outPixD = (highDisparity[i] >> 3) & 0xFFF; // take only disparity values without confidence

            if (abs(outPixD - upDisparity[i]) > diffDispTh)
            {
                outDisparity[i] = 0;
            } else
            {
                outDisparity[i] = (highDisparity[i] & 0xFFF8) + 7; // highest confidence
            }
        }
    }


    return;
}

void    PTK_Alg_StereoPP_upsampleDisparity(int16_t * lowDisparity, 
                                           int16_t * upDisparity, 
                                           int16_t   loWidth, 
                                           int16_t   loHeight, 
                                           int16_t   loStride,
                                           int16_t   hiWidth, 
                                           int16_t   hiHeight,
                                           int16_t   hiStride,
                                           int8_t    loDispOnlyFlag)
{
    int16_t i, j;
    int16_t ofst = BOUNDARY_OFFSET;

    int16_t * lowDispRow;
    int16_t * upDispRow0;
    int16_t * upDispRow1;
    int16_t * upDispRow2;

    for (j = ofst; j < loHeight-ofst; j++)
    {
        lowDispRow = &lowDisparity[j*loStride];
        upDispRow2 = &upDisparity[j*2*hiStride];

        for (i = ofst; i < loWidth-ofst; i++)
        {
            upDispRow2[i<<1] = ((lowDispRow[i] >> 3) & 0xFFF) << 1;
        }

        for (i = ofst; i < loWidth-ofst; i++)
        {
            upDispRow2[(i<<1) + 1] = (upDispRow2[(i<<1)] + upDispRow2[(i<<1) + 2]) >> 1;
        }

        if (j!= ofst)
        {
            upDispRow0 = &upDisparity[(j-1)*2*hiStride];
            upDispRow1 = &upDisparity[j*2*hiStride - hiStride];

            for (i = ofst; i < hiWidth-ofst; i++)
            {
                upDispRow1[i] = (upDispRow0[i] + upDispRow2[i] + 1) >> 1;
            }
        }
    }
}
