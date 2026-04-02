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
*  \file vhwa_ldc_tirtos_cfg.h
*
*  \brief Configuration for TI RTOS
*/

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */                                                                           
    
    {                                                                          
        "TC_LDC_FUNC_000",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[0U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_001",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[1U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_002",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[2U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_003",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        100u,                     /* Repeat Cnt */                             
        UTRUE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[3U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_004",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[4U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_005",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[5U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_006",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[6U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_007",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[7U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_008",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[8U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_009",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[9U]                                                
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_010",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[10U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_011",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[11U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_012",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[12U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_013",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[13U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_014",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[14U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_015",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[15U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_016",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[16U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_017",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[17U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_018",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[18U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_019",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[19U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_020",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[20U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_021",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[21U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_022",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[22U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_023",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[23U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_024",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[24U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_025",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[25U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_026",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[26U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_027",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[27U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_028",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[28U]                                               
        },                                                                     
        UFALSE                                                                 
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_029",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[29U]                                               
        },                                                                     
        UFALSE                                                                 
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_030",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[30U]                                               
        },                                                                     
        UFALSE                                                                 
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_031",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[31U]                                               
        },                                                                     
        UFALSE                                                                 
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_032",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[32U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_033",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[33U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_034",      /* Test Name */                                
        4u,                     /* Num Handles */                              
        1000u,                  /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[0U],                                               
            &gAppLdcTestCfg[14U],                                              
            &gAppLdcTestCfg[16U],                                              
            &gAppLdcTestCfg[17U]                                               
        },                                                                     
        UTRUE                                                                  
    },                                                                                                                                               
    {                                                                          
        "TC_LDC_FUNC_035",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[3U]                                                
        },
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                  
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_036",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[35U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_037",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[36U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_038",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        100u,                     /* Repeat Cnt */                             
        UTRUE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[37U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_039",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[38U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_040",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[39U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_041",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[40U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_042",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[41U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_043",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[42U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_044",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[43U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_045",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[44U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_046",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[45U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_047",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[46U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                 
        "TC_LDC_FUNC_048",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[47U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_049",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[48U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_050",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[49U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_051",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[50U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_052",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[51U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_053",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[52U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },                                                                         
    {                                                                          
        "TC_LDC_FUNC_054",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[53U]                                               
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                   
    },
    {                                                                          
        "TC_LDC_FUNC_055",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[54U]                                                
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                    
    },
    {                                                                          
        "TC_LDC_FUNC_056",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[55U]                                                
        },                                                                     
        UTRUE                                                                    
    },
    {                                                                          
        "TC_LDC_FUNC_057",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[56U]                                                
        },                                                                     
        UTRUE                                                                    
    },
    {                                                                          
        "TC_LDC_FUNC_058",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[57U]                                                
        },                                                                     
        UTRUE                                                                    
    },
    {                                                                          
        "TC_LDC_FUNC_059",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[58U]                                                
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                    
    },
    {                                                                          
        "TC_LDC_FUNC_060",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        FALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[59U]                                                
        },                                                                     
        TRUE                                                                   
    },
    {                                                                          
        "TC_LDC_FUNC_061",      /* Test Name */                                
        4u,                     /* Num Handles */                              
        1000u,                  /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[60U],                                               
            &gAppLdcTestCfg[61U],                                              
            &gAppLdcTestCfg[61U],                                              
            &gAppLdcTestCfg[61U]                                          
        },                                                                     
        UTRUE                                                                  
    },
    {                                                                          
        "TC_LDC_FUNC_062",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        FALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[62U]                                                
        },                                                                     
        TRUE                                                                   
    },
    {                                                                          
        "TC_LDC_FUNC_063",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                      
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[63U]                                                
        },                                                                     
        #ifndef VHWA_VPAC_IP_REV_VPAC                                                                    
        UTRUE
        #else
        UFALSE
        #endif                                                                    
    },
    {                                                                          
        "TC_LDC_FUNC_064",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[64U]                                               
        },                                                                     
        UTRUE                                                                  
    },
    {                                                                          
        "TC_LDC_FUNC_065",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[65U]                                               
        },                                                                     
        UTRUE                                                                  
    },
    {                                                                          
        "TC_LDC_FUNC_066",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[66U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_067",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[67U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_068",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[68U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_069",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[69U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_070",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[70U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_071",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[71U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_072",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[72U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_073",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[73U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_074",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[74U]                                                
        },                                                                     
        UTRUE                                                                  
    },
    {                                                                          
        "TC_LDC_FUNC_075",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[75U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_076",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[76U]                                                
        },                                                                     
        UTRUE                                                                  
    }, 
    {                                                                          
        "TC_LDC_FUNC_077",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[77U]                                                
        },                                                                     
        UTRUE                                                                  
    },
    {                                                                          
        "TC_LDC_FUNC_078",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[78U]                                                
        },                                                                     
        UTRUE                                                                  
    },
    {                                                                          
        "TC_LDC_FUNC_079",      /* Test Name */                                
        1u,                     /* Num Handles */                              
        1u,                     /* Repeat Cnt */                               
        UFALSE,                  /* Is Performance Test */                     
        /* Test Configuration */                                               
        {                                                                      
            &gAppLdcTestCfg[79U]                                                
        },                                                                     
        UTRUE                                                                  
    },
    