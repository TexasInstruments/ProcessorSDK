/*
*  Copyright (c) Texas Instruments Incorporated 2019
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

/**
*  file vhwa_msc_tirtos_cfg.h
*
*  brief Configuration for TI RTOS
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

    /* 0 - Thread 0, 1-in 10-out 1920x1080 YUV 12bit Packed input/output */    
    {                                                                          
        "TC_MSC_FUNC_TC00",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[0U] },                                                
        UTRUE,                                                                  
    },                                                                         
    /* 1 - Thread 1, 1-in 10-out 1920x1080 YUV 12bit Packed input/output */    
    {                                                                          
        "TC_MSC_FUNC_TC01",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[1U] },                                                
        UTRUE,                                                                  
    },                                                                         
    /* 2 - Thread 0, 1-in 10-out 1920x1080 YUV 12bit Unpacked input/output */  
    {                                                                          
        "TC_MSC_FUNC_TC02",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[2U] },                                                
        UTRUE,                                                                  
    },                                                                         
    /* 3 - Thread 0, 1-in 10-out 1920x1080 12bit Packed input to 8b */         
    {                                                                          
        "TC_MSC_FUNC_TC03",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[3U] },                                                
        UTRUE,                                                                  
    },                                                                         
    /* 4 - Thread 0, 1-in 10-out 1920x1080 8b to 8b */                         
    {                                                                          
        "TC_MSC_FUNC_TC04",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[4U] },                                                
        UTRUE,                                                                  
    },                                                                         
    /* 5 - Thread 0, 1-in 10-out 1920x1080 8b to 12b packed */                 
    {                                                                          
        "TC_MSC_FUNC_TC05",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[5U] },                                                
        UTRUE,                                                                  
    },                                                                         
    /* 6 - Thread 0, 1-in 10-out 1920x1080 ROI 12bP */                         
    {                                                                          
        "TC_MSC_FUNC_TC06",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[8U] },                                                
        UTRUE,                                                                  
    },                                                                         
    /* 7 - Thread 0, 1-in 10-out 1920x1080  SP 12b P */                        
    {                                                                          
        "TC_MSC_FUNC_TC07",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[13U] },                                               
        UTRUE,                                                                  
    },                                                                         
    /* 8 - Thread 0, 1-in 10-out 1920x1080 64 Phase 12b P */                   
    {                                                                          
        "TC_MSC_FUNC_TC08",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[14U] },                                               
        UTRUE,                                                                  
    },
    /* 9 - Thread 0/1, 1-in 5-out 1920x1080 YUV 12bit Packed input */          
    {                                                                          
        "TC_MSC_FUNC_TC09",                                                    
        2u,                     /* NumHandles */                               
        2u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[6U],                                                  
         &gAppMscTestCfg[7U]},                                                 
        UTRUE,                                                                  
    },                                                                                                                                                    
    /* 10 - Thread 0, 1-in 5-out 1920x1080 YUV 12bit Packed input/output */    
    {                                                                          
        "TC_MSC_FUNC_TC10",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[22U] },
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },                                                                         
    /* 11 - Thread 1, 1-in 5-out 1920x1080 YUV 12bit Packed input/output */    
    {                                                                          
        "TC_MSC_FUNC_TC11",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[23U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },                                                                         
    /* 12 - Thread 0, 1-in 5-out 1920x1080 8bit Packed input/output */    
    {
        "TC_MSC_FUNC_TC12",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[24U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                 
    },                                                                         
    /* 13 - Thread 0, 1-in 5-out 1920x1080 Y12 UV8 */    
    {                                                                          
        "TC_MSC_FUNC_TC13",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[25U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },                                                                         
    /* 14 - Thread 0, 1-in 5-out 1920x1080 Y8 UV12 */    
    {
        "TC_MSC_FUNC_TC14",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[26U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },                                                                         
    /* 15 - Thread 0, 1-in 5-out 1920x1080 Y8 Y8 */    
    {
        "TC_MSC_FUNC_TC15",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[27U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },                                                                         
    /* 16 - Thread 0, 1-in 5-out 1920x1080 YUV422SP 12bit Unpacked input/output */    
    {
        "TC_MSC_FUNC_TC16",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[28U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },                                                                         
    /* 17 - Thread 0, 1-in 4-out 1920x1080 YUV422I 12bit unpacked input/output */    
    {
        "TC_MSC_FUNC_TC17",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[29U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },                                                                         
    /* 18 - Thread 0, 11-in 5-out 1920x1080 P1 +P2P3 Interleaved */    
    {
        "TC_MSC_FUNC_TC18",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[30U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 19 - THREAD 0, TestCase 1-in 5-out non-intereaved 1920x1080 12bit packed input and 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC19",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[31U] },                                                
        UTRUE,                                                                  
    },
    /* 20 - THREAD 1, TestCase 1-in 5-out non-intereaved 1920x1080 12bit packed input and 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC20",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[32U] },                                                
        UTRUE,                                                                  
    },
    /* 21 - THREAD 0, TestCase 1-in 5-out non-intereaved 1920x1080 8bit packed input and 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC21",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[33U] },                                                
        UTRUE,                                                                  
    },
    /* 22 - THREAD 1, TestCase 1-in 5-out non-intereaved 1920x1080 8bit packed input and 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC22",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[34U] },                                                
        UTRUE,                                                                  
    },
    /* 23 - THREAD 0, TestCase 1-in 5-out intereaved 1920x1080 12bit packed input and 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC23",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[35U] },                                                
        UTRUE,                                                                  
    },
    /* 24 - THREAD 1 TestCase 1-in 5-out interleaved 1920x1080 12bit packed input and 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC24",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[36U] },                                                
        UTRUE,                                                                  
    },
    /* 25 - THREAD 0, TestCase 1-in 5-out intereaved 1920x1080 8bit packed input and 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC25",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[37U] },                                                
        UTRUE,                                                                  
    },
    /* 26 - THREAD 1, TestCase 1-in 5-out intereaved 1920x1080 8bit packed input and 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC26",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[38U] },                                                
        UTRUE,                                                                  
    },
    /* 27 - THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel non interleaved 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC27",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[39U] },                                                
        UTRUE,                                                                  
    },
    /* 28 - THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel non interleaved 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC28",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[40U] },                                                
        UTRUE,                                                                  
    },
    /* 29 - THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel non interleaved 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC29",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[41U] },                                                
        UTRUE,                                                                  
    },
    /* 30 - THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel non interleaved 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC30",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[42U] },                                                
        UTRUE,                                                                  
    },
    /* 31 - THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel interleaved 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC31",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[43U] },                                                
        UTRUE,                                                                  
    },
    /* 32 - THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel interleaved 12bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC32",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[44U] },                                                
        UTRUE,                                                                  
    },
    /* 33 - THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel interleaved 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC33",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[45U] },                                                
        UTRUE,                                                                  
    },
    /* 34 - THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel interleaved 8bit packed output  */
    {                                                                          
        "TC_MSC_FUNC_TC34",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[46U] },                                                
        UTRUE,                                                                  
    },
    /* 35 - Thread 1, 1-in 5-out 1920x1080 8bit Packed input/output */    
    {
        "TC_MSC_FUNC_TC35",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[47U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 36 - Thread 0 TestCase 1-in 4-out 1920x1080 YUV422I 8bit packed input/output  */
    {
        "TC_MSC_FUNC_TC36",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[48U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                 
    },
    /* 37 - Thread 1 TestCase 1-in 4-out 1920x1080 YUV422I 8bit packed input/output  */
    {
        "TC_MSC_FUNC_TC37",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[49U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                 
    },
    /* 38 - Thread 0 TestCase 1-in 4-out 1920x1080 YUV422I 12bit packed input/output  */
    {
        "TC_MSC_FUNC_TC38",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[50U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 39 - Thread 1 TestCase 1-in 4-out 1920x1080 YUV422I 12bit packed input/output  */
    {
        "TC_MSC_FUNC_TC39",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[51U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 40 - Thread 1 TestCase 1-in 4-out 1920x1080 YUV422I 12bit unpacked input/output  */
    {
        "TC_MSC_FUNC_TC40",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[52U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                
    },
    /* 41 - Thread 1 TestCase 1-in 5-out 1920x1080 Y12 UV8  */
    {
        "TC_MSC_FUNC_TC41",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[53U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                 
    },
    /* 42 - Thread 1 TestCase 1-in 5-out 1920x1080 Y8 UV12  */
    {
        "TC_MSC_FUNC_TC42",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[54U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 43 - Thread 1 TestCase 1-in 5-out 1920x1080 Y8 Y8  */
    {
        "TC_MSC_FUNC_TC43",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[55U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 44 - Thread 0 TestCase 1-in 5-out 1920x1080 Y12 Y12  */
    {
        "TC_MSC_FUNC_TC44",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[56U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                
    },
    /* 45 - Thread 1 TestCase 1-in 5-out 1920x1080 Y12 Y12  */
    {
        "TC_MSC_FUNC_TC45",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[57U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 46 Thread 0, 1920x1080 YUV422SP 12bit packed input/output  */
    {
        "TC_MSC_FUNC_TC46",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[58U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    }, 
    /* 47 Thread 1, 1920x1080 YUV422SP 12bit packed input/output  */
    {
        "TC_MSC_FUNC_TC47",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[59U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    }, 
    /* 48 Thread 0, 1920x1080 YUV422SP 8bit packed input/output  */
    {
        "TC_MSC_FUNC_TC48",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[60U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 49 Thread 1, 1920x1080 YUV422SP 8bit packed input/output  */
    {
        "TC_MSC_FUNC_TC49",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[61U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    /* 50 THREAD 0 TestCase 1-in 10-out non intereaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC50",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[62U] },                                                
        UTRUE,                                                                 
    },
    /* 51 THREAD 1 TestCase 1-in 10-out non intereaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC51",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[63U] },                                                
        UTRUE,                                                                 
    },
    /* 52 THREAD 0 TestCase 1-in 10-out interleaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC52",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[64U] },                                                
        UTRUE,                                                                 
    },
    /* 53 THREAD 1 TestCase 1-in 10-out interleaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC53",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[65U] },                                                
        UTRUE,                                                                 
    },
    /* 54 THREAD 0 TestCase 1-in 10-out YUV420 1920x1080 12bit unpacked input and single channel non interleaved 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC54",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[66U] },                                                
        UTRUE,                                                                 
    },
    /* 55 THREAD 1 TestCase 1-in 10-out YUV420 1920x1080 12bit unpacked input and single channel non interleaved 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC55",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[67U] },                                                
        UTRUE,                                                                 
    },
    /* 56 THREAD 0 TestCase 1-in 10-out YUV420 1920x1080 12bit unpacked input and single channel interleaved 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC56",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[68U] },                                                
        UTRUE,                                                                 
    },
    /* 57 THREAD 1 TestCase 1-in 10-out YUV420 1920x1080 12bit unpacked input and single channel interleaved 12bit unpacked output  */
    {                                                                         
        "TC_MSC_FUNC_TC57",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[69U] },                                                
        UTRUE,                                                                 
    },
    /* 58 Thread 0 TestCase 1-in 5-out 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        "TC_MSC_FUNC_TC58",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[70U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                
    },
    /* 59 Thread 1 TestCase 1-in 5-out 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        "TC_MSC_FUNC_TC59",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[71U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                 
    },
    /* 60 Thread 0 TestCase 1-in 5-out 1920x1080 Y16 Y16  */
    {
        "TC_MSC_FUNC_TC60",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[72U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                
    },
    /* 61 Thread 1 TestCase 1-in 5-out 1920x1080 Y16 Y16  */
    {
        "TC_MSC_FUNC_TC61",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[73U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                
    },
    /* 62 - Thread 1, 1-in 5-out 1920x1080 YUV422SP 12bit Unpacked input/output */    
    {
        "TC_MSC_FUNC_TC62",                                                    
        1u,                     /* NumHandles */                               
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[74U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    }, 
    /* 63 - Thread 1, 1-in 10-out 1920x1080 8b to 8b */                         
    {                                                                          
        "TC_MSC_FUNC_TC63",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[75U] },                                                
        UTRUE,                                                                  
    },
    /* 64  */                         
    {                                                                          
        "TC_MSC_FUNC_TC64",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[76U] },                                                
        UTRUE,                                                                  
    },
    /* 65  */                         
    {                                                                          
        "TC_MSC_FUNC_TC65",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[77U] },                                                
        UTRUE,                                                                  
    }, 
    /* 66  */                         
    {                                                                          
        "TC_MSC_FUNC_TC66",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[78U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC67",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[79U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC68",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[80U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC69",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[81U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC70",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[82U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC71",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[83U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC72",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[84U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC73",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[85U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC74",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[86U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC75",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[87U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC76",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[88U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC77",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[89U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC78",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[90U] },                                                
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC79",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UTRUE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[16U] },                                                
        UTRUE,                                                                  
    },
    // {
    //     "TC_MSC_FUNC_TC80",                                                    
    //     1u,                     /* NumHandles */                               
    //     100u,                     /* Repeat Cnt */                               
    //     UFALSE,                  /* Is Performance Test */                      
    //     {&gAppMscTestCfg[92U] },                                                
    //     #ifndef VHWA_VPAC_IP_REV_VPAC                                                
    //     UTRUE,
    //     #else
    //     UFALSE,
    //     #endif                                                                  
    // },
    {
        "TC_MSC_FUNC_TC80",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[93U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    {
        "TC_MSC_FUNC_TC81",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[94U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    {
        "TC_MSC_FUNC_TC82",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[95U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    // {
    //     "TC_MSC_FUNC_TC84",                                                    
    //     1u,                     /* NumHandles */                               
    //     100u,                     /* Repeat Cnt */                               
    //     UFALSE,                  /* Is Performance Test */                      
    //     {&gAppMscTestCfg[96U] },                                                
    //     #ifndef VHWA_VPAC_IP_REV_VPAC                                                
    //     UTRUE,
    //     #else
    //     UFALSE,
    //     #endif                                                                  
    // },
    // {
    //     "TC_MSC_FUNC_TC85",                                                    
    //     1u,                     /* NumHandles */                               
    //     100u,                     /* Repeat Cnt */                               
    //     UFALSE,                  /* Is Performance Test */                      
    //     {&gAppMscTestCfg[97U] },                                                
    //     #ifndef VHWA_VPAC_IP_REV_VPAC                                                
    //     UTRUE,
    //     #else
    //     UFALSE,
    //     #endif                                                                  
    // },
    // {
    //     "TC_MSC_FUNC_TC86",                                                    
    //     1u,                     /* NumHandles */                               
    //     100u,                     /* Repeat Cnt */                               
    //     UFALSE,                  /* Is Performance Test */                      
    //     {&gAppMscTestCfg[98U] },                                                
    //     #ifndef VHWA_VPAC_IP_REV_VPAC                                                
    //     UTRUE,
    //     #else
    //     UFALSE,
    //     #endif                                                                  
    // },
    {
        "TC_MSC_FUNC_TC83",                                                    
        1u,                     /* NumHandles */                               
        100u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[100U] },                                                
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                
        UTRUE,
        #else
        UFALSE,
        #endif                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC84",                                                    
        4u,                     /* NumHandles */                               
        1000u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[101U],
         &gAppMscTestCfg[102U],
         &gAppMscTestCfg[101U],
         &gAppMscTestCfg[102U],},                                                 
        UTRUE,                                                                  
    },
    {                                                                          
        "TC_MSC_FUNC_TC85",                                                    
        4u,                     /* NumHandles */                               
        1000u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        {&gAppMscTestCfg[103U]},                                                 
        UTRUE,                                                                  
    }, 
   