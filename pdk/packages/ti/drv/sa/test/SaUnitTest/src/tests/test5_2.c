/*
 *
 * Copyright (C) 2012-2013 Texas Instruments Incorporated - http://www.ti.com/ 
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

#include "../unittest.h"
#include "../salldsim/salldsim.h"

/* PKA (Public Key Accelerator) test.
 * This test is designed to verify the opertions of the Public Key Accelerator
 * (PKA) within SASS.
 *
 * Test Procedures:
 * 
 * - Enable PKA by invoking API Sa_pkaInit()
 * - Run through the PKA test vectors of all supported operations by invoking API Sa_pkaOperation() 
 * - Close PKA by invoking API Sa_pkaClose()
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */
#define SA_PKA_MAX_INT_PARAMS     3
#define SA_PAK_MAX_OUT_PARAMS     2
#define SA_PKA_MAX_PARAM_SIZE     sa_MAX_PKA_OPERAND_SIZE   /* In 32-bit words */

typedef struct saPkaParams_s
{
    uint16_t    size;       /* In bytes */
    uint32_t    data[SA_PKA_MAX_PARAM_SIZE];    

} saPkaParams_t;
 
typedef struct saPkaTestEntry_s
{
    int             operation;      /* PKA operation */
    int             cmpResult;      /* Expected compare result */
    uint16_t        numShiftBits;
    uint16_t        numInParams;    /* Number of input parameters */
    uint16_t        numOutParams;   /* Number of output parameters */
    saPkaParams_t   input[SA_PKA_MAX_INT_PARAMS];
    saPkaParams_t   outputExp[SA_PAK_MAX_OUT_PARAMS];  /* expected output */
} saPkaTestEntry_t; 
 
 
static saPkaTestEntry_t  saPkaTestEntry[] =
{

#if 1
    /* test entry 1 */
    {
        sa_PKA_OP_ADD,             /* operation */
        0,                         /* compare result */
        0,                         /* shiftBits */
        2,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x02010180, 0x81010102, 0x00000103, }
            },
            
            {
                3,
                {0x06050580, 0x85050506, 0x05050507,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                3,
                {0x08060700, 0x06060608, 0x0505060b, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
    /* test entry 2 */
    {
        sa_PKA_OP_SUB,             /* operation */
        0,                         /* compare result */
        0,                         /* shiftBits */
        2,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x67150600, 0x00080707, 0x12801603, }
            },
            
            {
                3,
                {0x060505ff, 0xFC050506, 0x00000502,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                3,
                {0x61100001, 0x04030201, 0x12801100, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
    
    /* test entry 3 */
    {
        sa_PKA_OP_MUL,             /* operation */
        0,                         /* compare result */
        0,                         /* shiftBits */
        2,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x08040201, 0x82402010, 0x88221100, }
            },
            
            {
                1,
                {0x00000102, 0x00000000, 0x000000000, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                4,
                {0x140a0502, 0x44a05028, 0x32552283, 0x00000089, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
#endif    
    
#if 1    
    /* test entry 4 */
    {
        sa_PKA_OP_DIV,             /* operation */
        0,                         /* compare result */
        0,                         /* shiftBits */
        2,                         /* Number of input parameters */  
        2,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x08040201, 0x82402010, 0x88221100, }
            },
            
            {
                2,
                {0x00000000, 0x00000001, 0x00000000, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                2,
                {0x82402010, 0x88221100, 0x00000000, }
            },
            
            {
                1,
                {0x08040201, 0x0, 0x0, }
            }
        }
    
    },
    
#endif

#if 1    

    /* test entry 5 */
    {
        sa_PKA_OP_RSHIFT,          /* operation */
        0,                         /* compare result */
        24,                        /* shiftBits */
        1,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x08040201, 0x82402010, 0x88221100, }
            },
            
            {
                0,
                {0, 0, 0, 0, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                3,
                {0x40201008, 0x22110082, 0x00000088,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
    /* test entry 6 */
    {
        sa_PKA_OP_LSHIFT,          /* operation */
        0,                         /* compare result */
        24,                        /* shiftBits */
        1,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x08040201, 0x82402010, 0x88221100, }
            },
            
            {
                0,
                {0, 0, 0, 0, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                4,
                {0x01000000, 0x10080402, 0x00824020, 0x00882211, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
    
    /* test entry 7 */
    {
        sa_PKA_OP_COPY,            /* operation */
        0,                         /* compare result */
        0,                         /* shiftBits */
        1,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x08040201, 0x82402010, 0x88221100, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                3,
                {0x08040201, 0x82402010, 0x88221100, }
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
    /* test entry 8 */
    {
        sa_PKA_OP_COMP,            /* operation */
        sa_PKA_COMP_ASUPB,         /* compare result */
        0,                        /* shiftBits */
        2,                         /* Number of input parameters */  
        0,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                3,
                {0x08040201, 0x82402010, 0x80221100, }
            },
            
            {
                3,
                {0x06050505, 0x05050506, 0x7FFF1207,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                0,
                {0, 0, 0, 0,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
    
    /* test entry 9 */
    {
        sa_PKA_OP_EXP2,            /* operation */
        0,                         /* compare result */
        0,                         /* shiftBits */
        3,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                1,
                {0x00000001, 0, 0,  }
            },
            
            {
                3,
                {0x000b0007,  0x000b0008, 0x000b0009,}
            },
            
            {
                3,
                {0x000b0004,  0x000b0005, 0x000b0006,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                3,
                {0x000b0004,  0x000b0005, 0x000b0006,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
    
    
    /* test entry 10 */
    {
        sa_PKA_OP_EXP4,            /* operation */
        0,                         /* compare result */
        0,                         /* shiftBits */
        3,                         /* Number of input parameters */  
        1,                         /* Number of output parameters */
        /* Input parameters */
        {
            {
                1,
                {0x00000001, 0, 0,  }
            },
            
            {
                3,
                {0x000b0007,  0x000b0008, 0x000b0009,}
            },
            
            {
                3,
                {0x000b0004,  0x000b0005, 0x000b0006,}
            }
            
        },
        
        /* Expected Output parameters */
        {
            {
                3,
                {0x000b0004,  0x000b0005, 0x000b0006,}
            },
            
            {
                0,
                {0, 0, 0, 0,}
            }
        }
    
    },
#endif    
    
#endif    

};

static saPkaParams_t   pkaOutput[SA_PAK_MAX_OUT_PARAMS];  /* actual output */

/* SA PKA test */
void saPkaTest (void* a0, void* a1)
{
    tFramework_t  *tf  = (tFramework_t *)(uintptr_t)a0;
 	saTest_t      *pat = (saTest_t *)(uintptr_t)a1;
 	saPkaTestEntry_t *pkaEntry;
    Sa_PkaReqInfo_t   pkaReq;
 	int  i;
    int16_t retCode;
    
    /* Enable Error logging */
    salld_sim_disp_control(TRUE);    
    
    /* Initialize the PKA module to be ready for large number arithmetic  */
    retCode = Sa_pkaInit(tf->salld_handle);
    
    if (retCode != sa_ERR_OK)
    {
        salld_sim_print("saPkaTest: Sa_pkaInit() returns error code = %d!\n", retCode);
        salld_sim_disp_control(FALSE);    
        
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    /* Wait for the Module to be ready */
	utilCycleDelay (1000);
    
    /* PKA Test */
    for (i = 0; i < sizeof(saPkaTestEntry)/sizeof(saPkaTestEntry_t); i++)
    {
        pkaEntry = &saPkaTestEntry[i];
        memset(&pkaReq, 0, sizeof(Sa_PkaReqInfo_t));
        
        /* Prepare the PKA request Info */
        /* Common actions */
        pkaReq.operation = (Sa_PkaOpTypes_t)pkaEntry->operation;
        pkaReq.oprandSize[0] = pkaEntry->input[0].size;
        pkaReq.oprandPtr[0] = pkaEntry->input[0].data;
        pkaReq.resultPtr = pkaOutput[0].data;
        
        /* Operation specific actions */
        switch (pkaEntry->operation)
        {
            case sa_PKA_OP_ADD:
            case sa_PKA_OP_SUB:
            case sa_PKA_OP_MUL:  
            case sa_PKA_OP_COMP:  
                pkaReq.oprandSize[1] = pkaEntry->input[1].size;
                pkaReq.oprandPtr[1] = pkaEntry->input[1].data;
                break;
                
            case sa_PKA_OP_DIV:
                pkaReq.oprandSize[1] = pkaEntry->input[1].size;
                pkaReq.oprandPtr[1] = pkaEntry->input[1].data;
                pkaReq.remPtr = pkaOutput[1].data;
                break; 
                
            case sa_PKA_OP_RSHIFT:
            case sa_PKA_OP_LSHIFT:
                pkaReq.numShiftBits = pkaEntry->numShiftBits;
                break;    
                
            case sa_PKA_OP_EXP2:  
            case sa_PKA_OP_EXP4:  
                pkaReq.oprandSize[1] = pkaEntry->input[1].size;
                pkaReq.oprandPtr[1]  = pkaEntry->input[1].data;
                pkaReq.oprandPtr[2]  = pkaEntry->input[2].data;
                break;
        
            default:
                break;
        }
        
        retCode = Sa_pkaOperation(tf->salld_handle, &pkaReq);
        
        if (retCode != sa_ERR_OK)
        {
            salld_sim_print("saPkaTest: Sa_pkaOperation() returns error code = %d!\n", retCode);
            salld_sim_disp_control(FALSE);    
        
            saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
        }
        
        /* Check operation results */
        switch (pkaEntry->operation)
        {
            case sa_PKA_OP_ADD:
            case sa_PKA_OP_SUB:
            case sa_PKA_OP_MUL:  
            case sa_PKA_OP_RSHIFT:
            case sa_PKA_OP_LSHIFT:
            case sa_PKA_OP_COPY:
            case sa_PKA_OP_EXP2:  
            case sa_PKA_OP_EXP4: 
            case sa_PKA_OP_DIV:  
                if (pkaReq.resultSize != pkaEntry->outputExp[0].size)
                {
                    salld_sim_print("saPkaTest: test %d fails! output size (%d) mismatches, expected size = %d \n", 
                                     i, pkaReq.resultSize, pkaEntry->outputExp[0].size);
                    salld_sim_disp_control(FALSE);    
 	                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                }
                else if (memcmp(pkaEntry->outputExp[0].data, pkaOutput[0].data, pkaReq.resultSize))
                {
                    salld_sim_print("saPkaTest: test %d fails! output data mismatches\n", i);
                    salld_sim_disp_control(FALSE);    
 	                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                }
                
                if (pkaEntry->operation == sa_PKA_OP_DIV)
                {
                
                    if (pkaReq.remSize != pkaEntry->outputExp[1].size)
                    {
                        salld_sim_print("saPkaTest: test %d fails! reminder size (%d) mismatches, expected size = %d \n", 
                                        i, pkaReq.remSize, pkaEntry->outputExp[1].size);
                        salld_sim_disp_control(FALSE);    
 	                    saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                    }
                    else if (memcmp(pkaEntry->outputExp[1].data, pkaOutput[1].data, pkaReq.remSize))
                    {
                        salld_sim_print("saPkaTest: test %d fails! reminder mismatches\n", i);
                        salld_sim_disp_control(FALSE);    
 	                    saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                    }
                }
                
                break;
                
            case sa_PKA_OP_COMP: 
                if (pkaReq.cmpResult != pkaEntry->cmpResult)
                {
                    salld_sim_print("saPkaTest: test %d (commpare operation)  fails! expected result = %d (actual result = %d)\n", 
                                    i, pkaEntry->cmpResult, pkaReq.cmpResult);
                    salld_sim_disp_control(FALSE);    
 	                saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
                }
                 
                break;
        
            default:
                break;
        }
    
        /* Delay a while before the next test */
		utilCycleDelay (100);
    }    
    
    /* Close the PKA module */
    retCode = Sa_pkaClose(tf->salld_handle);
    
    if (retCode != sa_ERR_OK)
    {
        salld_sim_print("saPkaTest: Sa_pkaClose() returns error code = %d!\n", retCode);
        salld_sim_disp_control(FALSE);    
        
        saTestRecoverAndExit (tf, pat, SA_TEST_FAILED);  /* no return */
    }
    
    salld_sim_print("saPkaTest: %d tests complete successfully!\n", sizeof(saPkaTestEntry)/sizeof(saPkaTestEntry_t));
    salld_sim_disp_control(FALSE);    
    
 	saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}
	
	
	
		
	


  		
