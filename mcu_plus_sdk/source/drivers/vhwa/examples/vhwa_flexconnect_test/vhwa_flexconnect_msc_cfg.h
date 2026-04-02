#ifndef APP_MSC_FC_CFG_H_
#define APP_MSC_FC_CFG_H_

#if (VPAC_TEST_INSTANCE == 0)
    #define VPAC_MSC_TEST_INST_ID_0     VHWA_M2M_VPAC_0_MSC_DRV_INST_ID_0
    #define VPAC_MSC_TEST_INST_ID_1     VHWA_M2M_VPAC_0_MSC_DRV_INST_ID_1
#endif
#if (VPAC_TEST_INSTANCE == 1)
    #define VPAC_MSC_TEST_INST_ID_0     VHWA_M2M_VPAC_1_MSC_DRV_INST_ID_0
    #define VPAC_MSC_TEST_INST_ID_1     VHWA_M2M_VPAC_1_MSC_DRV_INST_ID_1
#endif

App_MscTestCfg gAppMscFcTestCfg[] =
{
   /* 0, THREAD 0, 1280x720, Luma only, 1 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360, 640,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 1, THREAD 0, 1280x720, Chroma only, 1 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720/2, 1280,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360/2, 640,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 2, THREAD 0, 1280x720, */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280, //TODO  check pitch
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360, 640, //TODO check Pitch
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 3, THREAD 1, 1280x720, */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720/2, 1280, //TODO  check pitch
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0},
            {
                640, 360/2, 640, //TODO check Pitch
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },   
    /* 4, THREAD 0, 1280x720, YUV420 only, 2 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360, 640,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640, 360/2, 640,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },            
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 5, THREAD 0, 1280x720, */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280, //TODO  check pitch
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360, 640, //TODO check Pitch
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, 360/2, 640/2, //TODO check Pitch
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 6, THREAD 1, 1280x720, */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720/2, 1280, //TODO  check pitch
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0},{0},
            {
                640, 360/2, 640, //TODO check Pitch
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, (180)/2, 640/2, //TODO check Pitch
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },            
        },
        /* MSC Configuration for each instance */
        {
            {
                UFALSE
            },
            {
                UFALSE
            },            
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },            
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },   
    /* 7, THREAD 0, 1280x720, YUV420 only, 4 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360, 640,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640, 360/2, 640,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, 360/2, 640/2,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, (360/2)/2, 640/2,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },            
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },      
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },                    
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 8, THREAD 0, 1280x720, Luma only, 2 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360, 640,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, 360/2, 640/2,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },            
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 9, THREAD 0, 1280x720, Chroma only, 2 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720/2, 1280,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 180, 640,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, 180/2, 640/2,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },            
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 10, THREAD 1, 1280x720, YUV420 only, 4 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                640, 360, 640,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640, 360/2, 640,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, 360/2, 640/2,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, (360/2)/2, 640/2,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },            
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },      
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },                    
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 11, THREAD 0, 1280x720, YUV420 only, 4 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                UFALSE,
            },
            {
                UFALSE,
            },
            {
                UFALSE,
            },
            {
                UFALSE,
            },                                    
            {
                640, 360, 640,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640, 360/2, 640,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, 360/2, 640/2,
                1280, 720,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640/2, (360/2)/2, 640/2,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },            
        },
        /* MSC Configuration for each instance */
        {
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},             
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },      
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },                    
            {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },

    /* 12, THREAD 0 TestCase 1-in 5-out 1920x1080 8bit packed input and Luma only 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },
    /* 13, THREAD 1 TestCase 1-in 5-out 1920x1080 8bit packed input and Chroma only 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080/2, 1920,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0}, {0}, {0}, {0}, {0},
            {
                1152, 648/2, 1152,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432/2, 768,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324/2, 576,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702/2, 1248,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486/2, 864,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },

    /* 14,Instance 0 TestCase 1-in 5-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432/2, 768,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },

        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
        },
        UFALSE,
        {
        }
    },

    /* 15,Instance 1 TestCase 1-in 5-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432/2, 768,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },

        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
        },
        UFALSE,
        {
        }
    },
    /* 16,Instance 0 TestCase 1-in 5-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },

        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
        },
        UFALSE,
        {
        }
    },
    /* 17,Instance 1 TestCase 1-in 5-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },

        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
        },
        UFALSE,
        {
        }
    },
    /* 18 (J721S2), Instance 1 TestCase 1-in 5-out 1920x1080 8b packed input and 8b packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                768, 432, 768,
                1920, 540,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432/2, 768,
                1920, 540/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
            {
                0
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },
            {
                UFALSE
            },            
        },
        UFALSE,
        {
        }
    },
};

#endif
