/*
 * Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   osal_extended_testapp_cache.c
 *
 *  \brief  OSAL Cache Sub Module testcases file.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "osal_extended_test.h"
#if defined (BUILD_C7X)
#include "Cache.h"
#elif defined (BUILD_C66X)
#include <ti/csl/csl_chipAux.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define OSAL_APP_CACHE_TEST_ARR_SIZE (64U)

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

uint8_t gOsalAppCacheTestArr[OSAL_APP_CACHE_TEST_ARR_SIZE]; 

/* ========================================================================== */
/*                           Function Declarations                            */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Internal Function Definitions                     */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t OsalApp_cacheTests(void)
{
    int32_t result = osal_OK;
#if defined (BUILD_C7X)
    Cache_Size size;

    Cache_getSize(&size);

    if(size.l1pSize != Cache_initSize.l1pSize ||
       size.l1dSize != Cache_initSize.l1dSize ||
       size.l2Size != Cache_initSize.l2Size) 
    {
        result = osal_FAILURE;
    }

    Cache_disable(Cache_Type_L1D);
    Cache_disable(Cache_Type_L2);
    Cache_disable(0xABCD);
    Cache_enable(Cache_Type_L1D);
    Cache_enable(Cache_Type_L2);
    Cache_enable(0xABCD);
    Cache_wbAll();
    Cache_wbL1dAll();
    Cache_wbInvAll();
    Cache_wbInvL1dAll();
#endif
    /*  Write back the cache */
    CacheP_wb(gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE);
#if defined (BUILD_MCU)
    CSL_armR5CacheWb(gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE, BFALSE);
#elif defined (BUILD_C7X)
    Cache_wb((void *)gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE, Cache_Type_ALL, BFALSE);
#endif

    /* Invalidate the cache */
    CacheP_Inv(gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE);
#if defined (BUILD_MCU)
    CSL_armR5CacheInv(gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE, BFALSE);
#elif defined (BUILD_C7X)
    Cache_inv((void *)gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE, Cache_Type_ALL, BFALSE);
#endif

    /*  Write back and invalidate the cache */
    CacheP_wbInv(gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE);
#if defined (BUILD_MCU)
    CSL_armR5CacheWbInv(gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE, BFALSE);
    CSL_armR5CacheWait();
#elif defined (BUILD_C7X)
    Cache_wbInv((void *)gOsalAppCacheTestArr, OSAL_APP_CACHE_TEST_ARR_SIZE, Cache_Type_ALL, BFALSE);
#endif

    /* To cover the branch coverage of for loop where it expects the size of the address more than zero */
#if defined (BUILD_C66X) && defined (SAFERTOS)
    CacheP_setMar((void *)configDDR_START, configDDR_START, CacheP_Mar_ENABLE);
    uint32_t marAddr = CacheP_getMar(configDDR_START);
    if(marAddr != CacheP_Mar_ENABLE)
    {
        result = osal_FAILURE;
    }
#endif

    OSAL_log("\n All Cache Tests have passed!!\n");

    return result;
}

