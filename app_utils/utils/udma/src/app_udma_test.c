/*
 *
 * Copyright (c) 2019 Texas Instruments Incorporated
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

#include <stdint.h>
#include <utils/udma/include/app_udma.h>
#include <utils/mem/include/app_mem.h>
#include <utils/console_io/include/app_log.h>



/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Default heap ID to use */
#define APP_UDMA_HEAP_ID                (APP_MEM_HEAP_DDR)

/** \brief Alignment for memory allocation */
#define APP_UDMA_CACHELINE_ALIGNMENT    (64u)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

int32_t appUdmaTest1DCopy(void);
int32_t appUdmaTest2DCopy(void);
int32_t appUdmaTest2DFill(void);
int32_t appUdmaTestNDCopy(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t appUdmaTest(void)
{
    int32_t     ret_val = 0;

    appLogPrintf("UDMA: Test ... !!!\n");

    ret_val += appUdmaTestNDCopy();
    if(0 == ret_val)
    {
        appLogPrintf("UDMA: appUdmaTestNDCopy test passed!!\n");
    }
    #if 1
    ret_val += appUdmaTest1DCopy();
    if(0 == ret_val)
    {
        appLogPrintf("UDMA: appUdmaTest1DCopy test passed!!\n");
    }
    ret_val += appUdmaTest2DCopy();
    if(0 == ret_val)
    {
        appLogPrintf("UDMA: appUdmaTest2DCopy test passed!!\n");
    }
    ret_val += appUdmaTest2DFill();
    if(0 == ret_val)
    {
        appLogPrintf("UDMA: appUdmaTest2DFill test passed!!\n");
    }
    #endif

    if(0 == ret_val)
    {
        appLogPrintf("All tests have passed!!\n");
    }
    else
    {
        appLogPrintf("Some tests have failed!!\n");
    }
    appLogPrintf("UDMA: Test ... Done !!!\n");

    return (ret_val);
}

int32_t appUdmaTestNDCopy(void)
{
    int32_t                 ret_val = 0;
    uint8_t                *src_buf = NULL, *dest_buf = NULL, *tmp_buf = NULL;
    uint32_t                length, tmp_length, i;

    /* set test parameters */
    /* 0..7 UDMA CH, 8..15 DRU CH */
    uint32_t ch_idx_in = 0;
    uint32_t ch_idx_out = 1;
    uint32_t w_blk = 641;
    uint32_t h_blk = 7;
    uint32_t n_blk = 1;
    uint32_t tmp_blk = 1;
    uint32_t stride_blk = w_blk;
    uint32_t h = h_blk*n_blk;
    uint32_t copy_size = stride_blk * h;
    uint32_t offset_src = 0;
    uint32_t offset_dst = 1;

    length = copy_size + offset_src + offset_dst;
    tmp_length = stride_blk * h_blk * tmp_blk;

    /* Allocate buffer memory */
    src_buf = appMemAlloc(APP_UDMA_HEAP_ID, length, APP_UDMA_CACHELINE_ALIGNMENT);
    dest_buf = appMemAlloc(APP_UDMA_HEAP_ID, length, APP_UDMA_CACHELINE_ALIGNMENT);
    tmp_buf = appMemAlloc(APP_UDMA_HEAP_ID, tmp_length, APP_UDMA_CACHELINE_ALIGNMENT);
    if((NULL == src_buf) || (NULL == dest_buf) || (NULL == tmp_buf))
    {
        appLogPrintf("UDMA Test: ERROR: mem alloc failed !!!\n");
        ret_val = -1;
    }

    /* make test data */
    if(0 == ret_val)
    {
        /* Init source and dest buffer */
        for(i = 0; i < (copy_size); i++)
        {
            src_buf[offset_src+i] = i;
            dest_buf[offset_dst+i] = 0U;
        }
        /* Writeback cache */
        appMemCacheWb(src_buf, length);
        appMemCacheWb(dest_buf, length);
    }
    /* Do the N-dimentional DMA */
    if(0 == ret_val)
    {
        app_udma_copy_nd_prms_t prms_nd_in;
        app_udma_copy_nd_prms_t prms_nd_out;
        app_udma_ch_handle_t    udmaChIn;
        app_udma_ch_handle_t    udmaChOut;

        appUdmaCopyNDPrms_Init(&prms_nd_in);
        appUdmaCopyNDPrms_Init(&prms_nd_out);

        prms_nd_in.copy_mode    = 2;
        prms_nd_in.src_addr     = appMemGetVirt2PhyBufPtr((uint64_t) &src_buf[offset_src], APP_UDMA_HEAP_ID);
        prms_nd_in.dest_addr    = appMemGetVirt2PhyBufPtr((uint64_t) &tmp_buf[0], APP_UDMA_HEAP_ID);
        prms_nd_in.icnt0        = w_blk;
        prms_nd_in.icnt1        = h_blk;
        prms_nd_in.icnt2        = tmp_blk;
        prms_nd_in.icnt3        = n_blk / tmp_blk;
        prms_nd_in.dim1         = stride_blk;
        prms_nd_in.dim2         = (h_blk * stride_blk);
        prms_nd_in.dim3         = (h_blk * stride_blk * tmp_blk);

        prms_nd_in.dicnt0       = prms_nd_in.icnt0;
        prms_nd_in.dicnt1       = prms_nd_in.icnt1;
        prms_nd_in.dicnt2       = tmp_blk; /* Ping-pong */
        prms_nd_in.dicnt3       = n_blk / tmp_blk;
        prms_nd_in.ddim1        = stride_blk;
        prms_nd_in.ddim2        = (h_blk * stride_blk);
        prms_nd_in.ddim3        = 0;

        prms_nd_out.copy_mode   = 2;
        prms_nd_out.src_addr     = appMemGetVirt2PhyBufPtr((uint64_t) &tmp_buf[0], APP_UDMA_HEAP_ID);
        prms_nd_out.dest_addr    = appMemGetVirt2PhyBufPtr((uint64_t) &dest_buf[offset_dst], APP_UDMA_HEAP_ID);
        prms_nd_out.icnt0        = w_blk;
        prms_nd_out.icnt1        = h_blk;
        prms_nd_out.icnt2        = tmp_blk; /* Ping-pong */
        prms_nd_out.icnt3        = n_blk / tmp_blk;
        prms_nd_out.dim1         = stride_blk;
        prms_nd_out.dim2         = (h_blk * stride_blk);
        prms_nd_out.dim3         = 0;

        prms_nd_out.dicnt0       = prms_nd_out.icnt0;
        prms_nd_out.dicnt1       = prms_nd_out.icnt1;
        prms_nd_out.dicnt2       = tmp_blk;
        prms_nd_out.dicnt3       = n_blk / tmp_blk;
        prms_nd_out.ddim1        = stride_blk;
        prms_nd_out.ddim2        = (h_blk * stride_blk);
        prms_nd_out.ddim3        = (h_blk * stride_blk * tmp_blk);

        appUdmaCopyNDPrmsPrint(&prms_nd_in, "IN");
        appUdmaCopyNDPrmsPrint(&prms_nd_out, "OUT");

        udmaChIn = appUdmaCopyNDGetHandle(ch_idx_in);
        udmaChOut = appUdmaCopyNDGetHandle(ch_idx_out);

        appUdmaCopyNDInit(udmaChIn, &prms_nd_in);
        appUdmaCopyNDInit(udmaChOut, &prms_nd_out);

        for(i=0; i<n_blk; i++)
        {
            appUdmaCopyNDTrigger(udmaChIn);
            appUdmaCopyNDWait(udmaChIn);
            appUdmaCopyNDTrigger(udmaChOut);
            appUdmaCopyNDWait(udmaChOut);
        }

        appUdmaCopyNDDeinit(udmaChIn);
        appUdmaCopyNDDeinit(udmaChOut);
    }

    /* compare and check test data */
    if(0 == ret_val)
    {
        /* Invalidate cache */
        appMemCacheInv(dest_buf, length);

        for(i = 0; i < (copy_size); i++)
        {
            if(src_buf[offset_src+i] != dest_buf[offset_dst+i])
            {
                appLogPrintf("UDMA: ERROR: Data mismatch!!\n");
                ret_val = -1;
                break;
            }
        }
    }

    /* Free buffer memory */
    if(NULL != src_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, src_buf, length);
        src_buf = NULL;
    }
    if(NULL != dest_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, dest_buf, length);
        dest_buf = NULL;
    }
    if(NULL != tmp_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, tmp_buf, tmp_length);
        tmp_buf = NULL;
    }

    return (ret_val);
}

int32_t appUdmaTest1DCopy(void)
{
    int32_t                 ret_val = 0;
    uint8_t                *src_buf = NULL, *dest_buf = NULL;
    uint32_t                length = 100U, i;
    app_udma_copy_1d_prms_t prms_1d;

    /* Allocate buffer memory */
    src_buf = appMemAlloc(APP_UDMA_HEAP_ID, length, APP_UDMA_CACHELINE_ALIGNMENT);
    dest_buf = appMemAlloc(APP_UDMA_HEAP_ID, length, APP_UDMA_CACHELINE_ALIGNMENT);
    if((NULL == src_buf) || (NULL == dest_buf))
    {
        appLogPrintf("UDMA Test: ERROR: mem alloc failed !!!\n");
        ret_val = -1;
    }

    if(0 == ret_val)
    {
        /* Init source and dest buffer */
        for(i = 0U; i < length; i++)
        {
            src_buf[i] = i;
            dest_buf[i] = 0U;
        }
        /* Writeback cache */
        appMemCacheWb(src_buf, length);
        appMemCacheWb(dest_buf, length);

        /* Start transfer */
        appUdmaCopy1DPrms_Init(&prms_1d);
        prms_1d.dest_addr    = appMemGetVirt2PhyBufPtr((uint64_t) dest_buf, APP_UDMA_HEAP_ID);
        prms_1d.src_addr     = appMemGetVirt2PhyBufPtr((uint64_t) src_buf, APP_UDMA_HEAP_ID);
        prms_1d.length       = length;
        ret_val = appUdmaCopy1D(NULL, &prms_1d);
        if(0 != ret_val)
        {
            appLogPrintf("UDMA: 1D copy test failed... !!!\n");
        }
        else
        {
            /* Invalidate cache */
            appMemCacheInv(dest_buf, length);

            for(i = 0U; i < length; i++)
            {
                if(src_buf[i] != dest_buf[i])
                {
                    appLogPrintf("UDMA: ERROR: Data mismatch!!\n");
                    ret_val = -1;
                    break;
                }
            }
        }
    }

    /* Free buffer memory */
    if(NULL != src_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, src_buf, length);
        src_buf = NULL;
    }
    if(NULL != dest_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, dest_buf, length);
        dest_buf = NULL;
    }

    return (ret_val);
}

int32_t appUdmaTest2DCopy(void)
{
    int32_t                 ret_val = 0;
    uint8_t                *src_buf = NULL, *dest_buf = NULL;
    uint32_t                width, pitch, height, size, i, j;
    app_udma_copy_2d_prms_t prms_2d;

    width   = 64U;
    pitch   = 128U;
    height  = 100U;
    size    = pitch * height;

    /* Allocate buffer memory */
    src_buf = appMemAlloc(APP_UDMA_HEAP_ID, size, APP_UDMA_CACHELINE_ALIGNMENT);
    dest_buf = appMemAlloc(APP_UDMA_HEAP_ID, size, APP_UDMA_CACHELINE_ALIGNMENT);
    if((NULL == src_buf) || (NULL == dest_buf))
    {
        appLogPrintf("UDMA Test: ERROR: mem alloc failed !!!\n");
        ret_val = -1;
    }

    if(0 == ret_val)
    {
        /* Init source and dest buffer */
        for(i = 0U; i < height; i++)
        {
            for(j = 0U; j < width; j++)
            {
                src_buf[j + pitch*i] = j + pitch*i;
                dest_buf[j + pitch*i] = 0U;
            }
        }
        /* Writeback cache */
        appMemCacheWb(src_buf, size);
        appMemCacheWb(dest_buf, size);

        /* Start transfer */
        appUdmaCopy2DPrms_Init(&prms_2d);
        prms_2d.width        = width;
        prms_2d.height       = height;
        prms_2d.dest_pitch   = pitch;
        prms_2d.src_pitch    = pitch;
        prms_2d.dest_addr    = appMemGetVirt2PhyBufPtr((uint64_t) dest_buf, APP_UDMA_HEAP_ID);
        prms_2d.src_addr     = appMemGetVirt2PhyBufPtr((uint64_t) src_buf, APP_UDMA_HEAP_ID);
        ret_val = appUdmaCopy2D(NULL, &prms_2d, 1);
        if(0 != ret_val)
        {
            appLogPrintf("UDMA: 2D copy test failed... !!!\n");
        }
        else
        {
            /* Invalidate cache */
            appMemCacheInv(dest_buf, size);

            for(i = 0U; i < height; i++)
            {
                for(j = 0U; j < width; j++)
                {
                    if(src_buf[j + pitch*i] != dest_buf[j + pitch*i])
                    {
                        appLogPrintf("UDMA: ERROR: Data mismatch!!\n");
                        ret_val = -1;
                        break;
                    }
                }
            }
        }
    }

    /* Free buffer memory */
    if(NULL != src_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, src_buf, size);
        src_buf = NULL;
    }
    if(NULL != dest_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, dest_buf, size);
        dest_buf = NULL;
    }

    return (ret_val);
}

int32_t appUdmaTest2DFill(void)
{
    int32_t                 ret_val = 0;
    uint8_t                *src_buf = NULL, *dest_buf = NULL;
    uint32_t                width, pitch, height, size, i, j;
    app_udma_copy_2d_prms_t prms_2d;

    width   = 64U;
    pitch   = 128U;
    height  = 100U;
    size    = pitch * height;

    /* Allocate buffer memory */
    src_buf = appMemAlloc(APP_UDMA_HEAP_ID, width, APP_UDMA_CACHELINE_ALIGNMENT);
    dest_buf = appMemAlloc(APP_UDMA_HEAP_ID, size, APP_UDMA_CACHELINE_ALIGNMENT);
    if((NULL == src_buf) || (NULL == dest_buf))
    {
        appLogPrintf("UDMA Test: ERROR: mem alloc failed !!!\n");
        ret_val = -1;
    }

    if(0 == ret_val)
    {
        /* Init source and dest buffer */
        for(j = 0U; j < width; j++)
        {
            src_buf[j] = j;
        }
        for(i = 0U; i < height; i++)
        {
            for(j = 0U; j < width; j++)
            {
                dest_buf[j + pitch*i] = 0U;
            }
        }
        /* Writeback cache */
        appMemCacheWb(src_buf, width);
        appMemCacheWb(dest_buf, size);

        /* Start transfer */
        appUdmaCopy2DPrms_Init(&prms_2d);
        prms_2d.width        = width;
        prms_2d.height       = height;
        prms_2d.dest_pitch   = pitch;
        prms_2d.src_pitch    = width;
        prms_2d.dest_addr    = appMemGetVirt2PhyBufPtr((uint64_t) dest_buf, APP_UDMA_HEAP_ID);
        prms_2d.src_addr     = appMemGetVirt2PhyBufPtr((uint64_t) src_buf, APP_UDMA_HEAP_ID);
        ret_val = appUdmaFill2D(NULL, &prms_2d, 1);
        if(0 != ret_val)
        {
            appLogPrintf("UDMA: 2D fill test failed... !!!\n");
        }
        else
        {
            /* Invalidate cache */
            appMemCacheInv(dest_buf, size);

            for(i = 0U; i < height; i++)
            {
                for(j = 0U; j < width; j++)
                {
                    if(src_buf[j] != dest_buf[j + pitch*i])
                    {
                        appLogPrintf("UDMA: ERROR: Data mismatch!!\n");
                        ret_val = -1;
                        break;
                    }
                }
            }
        }
    }

    /* Free buffer memory */
    if(NULL != src_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, src_buf, width);
        src_buf = NULL;
    }
    if(NULL != dest_buf)
    {
        appMemFree(APP_UDMA_HEAP_ID, dest_buf, size);
        dest_buf = NULL;
    }

    return (ret_val);
}
