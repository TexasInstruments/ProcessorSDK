#if (VPAC_TEST_INSTANCE == 0)
    #define VPAC_MSC_TEST_INST_ID_0     VHWA_M2M_VPAC_0_MSC_DRV_INST_ID_0
    #define VPAC_MSC_TEST_INST_ID_1     VHWA_M2M_VPAC_0_MSC_DRV_INST_ID_1
#endif
#if (VPAC_TEST_INSTANCE == 1)
    #define VPAC_MSC_TEST_INST_ID_0     VHWA_M2M_VPAC_1_MSC_DRV_INST_ID_0
    #define VPAC_MSC_TEST_INST_ID_1     VHWA_M2M_VPAC_1_MSC_DRV_INST_ID_1
#endif

    /* 0, Instance 0 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
        UTRUE,
        {
            {
                0x6b2879b5, 0x837004e8, 0x1f27aa63, 0x81cd6505, 0x60e53bbb,
                0x8bf84e75, 0x252ef6cf, 0x8e774c1e, 0x8ed21063, 0xdf4fef68
            },
            {
                0x816382c6, 0x2d236640, 0xdcb1a0e3, 0x5a9810b0, 0x3f9c45c2,
                0x2b135a8, 0x8cd80995, 0xa2ea8425, 0xc49b5642, 0xe9d4b85c
            }
        }
    },
    /* 1, Instance 1 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
        UTRUE,
        {
            {
                0x6b2879b5, 0x837004e8, 0x1f27aa63, 0x81cd6505, 0x60e53bbb,
                0x8bf84e75, 0x252ef6cf, 0x8e774c1e, 0x8ed21063, 0xdf4fef68
            },
            {
                0x816382c6, 0x2d236640, 0xdcb1a0e3, 0x5a9810b0, 0x3f9c45c2,
                0x2b135a8, 0x8cd80995, 0xa2ea8425, 0xc49b5642, 0xe9d4b85c
            }
        }
    },
    /* 2, THREAD 0 TestCase 1-in 10-out 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1344, 756, 1344*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1152, 648, 1152*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                576, 324, 576*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1248, 702, 1248*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                864, 486, 864*2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_UNPACKED16
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
        {{0},{0}}
    },
    /* 3, THREAD 0 TestCase 1-in 10-out 1920x1080 12bit packed input and 8bit output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /* 4, THREAD 0 TestCase 1-in 10-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
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
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /* 5, THREAD 0 TestCase 1-in 10-out 1920x1080 8bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
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
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
        {{0},{0}}
    },
    /* 6, THREAD 0 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
        UTRUE,
        {
            {0x6b2879b5, 0x837004e8, 0x1f27aa63, 0x81cd6505, 0x60e53bbb},
            {0x816382c6, 0x2d236640, 0xdcb1a0e3, 0x5a9810b0, 0x3f9c45c2}
        }
    },
    /* 7, THREAD 1 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0}, {0}, {0}, {0}, {0},
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
        UTRUE,
        {
            {0, 0, 0, 0, 0,
             0x8bf84e75, 0x252ef6cf, 0x8e774c1e, 0x8ed21063, 0xdf4fef68},
            {0, 0, 0, 0, 0,
             0x2b135a8, 0x8cd80995, 0xa2ea8425, 0xc49b5642, 0xe9d4b85c}
        }
    },
    /* 8, THREAD 0 TestCase 1-in 10-out 1280x720 12bit packed input and ROI 1024x640 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1280, 720, 1280*3/2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                1024, 640, 1024*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                512, 320, 512*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                256, 160, 256*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                920, 576, 920*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                818, 512, 818*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                716, 448, 716*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                614, 384, 614*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                408, 256, 408*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                308, 192, 308*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 480, 768*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {80,40},
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
                UTRUE, {80,40},
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
                UTRUE, {10,10},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
            {
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
        {{0},{0}}
    },
    /* 9, THREAD 0 TestCase 1-in 5-out intereaved 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
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
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UFALSE
            },
        },
        UTRUE,
        {
            {
                0x6B2879B5, 0, 0x489C28D5, 0, 0x654D2893,
                0, 0xEE126CD2, 0, 0x33EA88FF, 0
            },
            {
                0
            }
        }
    },
    /* 10, THREAD 1 TestCase 1-in 5-out interleaved 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080/2, 1920*3/2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0},
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1152, 648/2, 1152*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                576, 324/2, 576*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                864, 486/2, 864*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
        },
        UTRUE,
        {
            {
                0, 0x2D236640, 0, 0x71DB6E7C, 0,
                0x87531734, 0, 0xC121A445, 0, 0x219DC4CD
            },
            {
                0
            }
        }
    },
    /* 11, THREAD 0 TestCase 1-in 1-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
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
                1920, 1080, 1920,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UTRUE,
        {
            {0x006FA1F6}, //{0x2AFDF6B8},
            {0x38602E58} //{0x4D4B96B1}
        },
        UFALSE, /* MCRC */
        {0xE678FF159EE2C866}   /* MCRC */
    },
    /* 12, THREAD 1 TestCase 1-in 1-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0}, {0}, {0}, {0},
            {0}, {0}, {0}, {0}, {0},
            {
                1920, 1080, 1920,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            }
        },
        /* MSC Configuration for each instance */
        {
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
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
            }
        },
        UTRUE,
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0x006FA1F6}, //0x2AFDF6B8},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0x38602E58}, //0x4D4B96B1}
        },
        UFALSE, /* MCRC */
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0xE678FF159EE2C866}   /* MCRC */
    },
    /* 13, THREAD 0 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output Single Phase */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_SINGLE_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_SINGLE_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_1,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE},{UFALSE},{UFALSE},{UFALSE},{UFALSE},
            {UFALSE},{UFALSE},{UFALSE}
        },
        UFALSE,
        {{0},{0}},
        UFALSE, /* MCRC */
        {0x09526F94BB27382D, 0x7F50372CB943053F, 0, 0,
         0, 0, 0, 0, 0, 0}   /* MCRC */
    },
    /* 14, THREAD 0 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output 64 Phase */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_SINGLE_PHASE_SP_COEFF_0,
                MSC_MULTI_64PHASE_COEFF_SET_2,
                MSC_MULTI_64PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
        },
        UFALSE,
        {{0},{0}},
        UFALSE, /* MCRC */
        {0x7A42078E8215AD3D, 0xD8CC3853BD4A129B, 0x4C246255D00216C9,
         0x8A0E77F404DF687B, 0, 0, 0, 0, 0, 0}   /* MCRC */
    },
    /* 15, THREAD 0 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output 4tap filter */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_4TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
            {
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
        {{0},{0}},
        UFALSE, /* MCRC */
        {0x25286C09544A2D93, 0xFFC24F9C03195C62, 0x4B94FBEF0AAD152C, 0x9FC7F47762758E22,
         0, 0, 0, 0, 0, 0}   /* MCRC */
    },
    /* 16, THREAD 0 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output 3-Tap */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_3TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
            {
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
                UFALSE, {0,0},
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
        {{0},{0}},
        UFALSE, /* MCRC */
        {0xBBF67D83807C8B0D, 0xFDADE56B4E2FD659, 0x5785B3AC1820656C, 0xA53FCE1156CB2DED,
         0, 0, 0, 0, 0, 0}   /* MCRC */
    },
    /* 17, THREAD 0 TestCase 1-in 1-out 1920x1080 12bit packed input and 12bit packed output Line Skip */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UTRUE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}
        },
        UFALSE,
        {{0},{0}},
        UFALSE, /* MCRC */
        {0, 0, 0, 0,
         0, 0, 0, 0, 0, 0}   /* MCRC */
    },
    /* 18, THREAD 0 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        UTRUE,
        {
            {0x8DC9CDFA, 0xD95ABB46, 0x143129FE, 0x8BC2D516, 0x41264E29},
            {0x3E1DDF0B, 0x3D2555D9, 0x408B3CC1, 0x4C9BF8F2, 0xE74B4326}
        }
    },
    /* 19, THREAD 1 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0}, {0}, {0}, {0}, {0},
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        UTRUE,
        {
            {0, 0, 0, 0, 0,
             0x18435A84, 0x093D7805, 0xEEFCE657, 0x372ADD40, 0x83219C2A},
            {0, 0, 0, 0, 0,
             0xE3844B20, 0x91D3F804, 0x770502C8, 0x0F26FFEA, 0xA8E04955}
        }
    },
    /* 20, THREAD 0, 1280x720, Luma only, 2 output*/
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
                320, 180, 320,
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
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
    /* 21, THREAD 0, 1280x720, Chroma only, 2 output*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
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
            {0}, {0},
            {
                640, 360/2, 640,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                320, 180/2, 320,
                1280, 720/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {UFALSE}, {UFALSE},
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
            {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
        },
        UFALSE,
        {
            {0},
            {0}
        }
    },


    /* 22 (J721S2), Instance 0 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE, {0},
#if defined VHWA_VPAC_IP_REV_VPAC3 || defined VHWA_VPAC_IP_REV_VPAC3L
        {UFALSE, 0, 0},  /* inSecFrm */
#endif
        VHWA_WDTIMEOUT_64K_CYCLES, VHWA_MSC0_WDTIMER_ERR
        #endif
    },
    /* 23 (J721S2), Instance 1 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        #endif
    },
    /* 24 (J721S2), THREAD 0 TestCase 1-in 5-out 1920x1080 8bit input and 8bit output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
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
        {{0},{0}}
        #endif
    },
    /* 25 (J721S2), Instance 0 TestCase 1-in 5-out 1920x1080 Y12 UV8  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE,
        {
        },
        {
            UTRUE,
            1920,
            FVID2_CCSF_BITS8_PACKED,
        },
        #endif
    },
    /* 26 (J721S2), Instance 0 TestCase 1-in 5-out 1920x1080 Y8 UV12  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
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
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE,
        {
        },
        {
            UTRUE,
            1920*3/2,
            FVID2_CCSF_BITS12_PACKED,
        },
        #endif
    },
    /* 27 (J721S2), Instance 0 TestCase 1-in 5-out 1920x1080 Y8 Y8  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_2PLANES,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_PLANE_2,
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
        },
        UFALSE,
        {
        }
        #endif
    },
    /* 28 (J721S2), Instance 0, 1920x1080 YUV422SP 12bit unpacked input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV422SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
        #endif
    },
    /* 29 (J721S2) Instance 0 TestCase 1-in 4-out 1920x1080 YUV422I 12bit unpacked input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*4,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                960, 540, 960*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                1728, 972, 1728*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                1536, 864, 1536*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            { 0 },
            { 0 }
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
                UFALSE
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
        #endif
    },
    /* 30 (J721S2), Instance 0 TestCase 1-in 5-out 1920x1080 P1 +P2P3 Interleaved  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_R_GBI,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480*2, 270, 480*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960*2, 540, 960*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640, 360, 640,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640*2, 360, 640*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                384, 216, 384,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                384*2, 216, 384*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768*2, 432, 768*2,
                1920*2, 1080,
                FVID2_DF_GBI,
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
        },
        UFALSE,
        {
        },
        {
            UTRUE,
            1920*2,
            FVID2_CCSF_BITS8_PACKED,
        },
        #endif
    },

    /* 31, THREAD 0 TestCase 1-in 5-out non intereaved 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },
    /* 32, THREAD 1 TestCase 1-in 5-out non intereaved 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },

    /* 33, THREAD 0 TestCase 1-in 5-out non intereaved 1920x1080 8bit packed input and 8bit packed output  */
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
            {0},
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },
    /* 34, THREAD 1 TestCase 1-in 5-out non intereaved 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
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
            {0},
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },

    /* 35, THREAD 0 TestCase 1-in 5-out interleaved 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080/2, 1920*3/2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0},
            {
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
    /* 36, THREAD 1 TestCase 1-in 5-out interleaved 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080/2, 1920*3/2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0},
            {
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
    /* 37, THREAD 0 TestCase 1-in 5-out interleaved 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
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
            {0},
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
                UFALSE
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
    /* 38, THREAD 1 TestCase 1-in 5-out interleaved 1920x1080 8bit packed input and 8bit packed output  */
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
            {0},
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
                UFALSE
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
    /* 39, THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel non interleaved 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },
    /* 40, THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel non interleaved 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },

    /* 41, THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel non interleaved 8bit packed output  */
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
            {0},
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },
    /* 42, THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel non interleaved 8bit packed output  */
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
            {0},
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
        },
        UFALSE,
        {
        }
    },

    /* 43, THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel interleaved 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0},
            {
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
    /* 44, THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 12bit packed input and single channel interleaved 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {0},
            {
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
    /* 45, THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel interleaved 8bit packed output  */
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
            {0},
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
                UFALSE
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
    /* 46, THREAD 1 TestCase 1-in 5-out YUV420 1920x1080 8bit packed input and single channel interleaved 8bit packed output  */
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
            {0},
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {0},
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
                UFALSE
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
                UFALSE
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
                UFALSE
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
     /* 47, THREAD 1 TestCase 1-in 5-out 1920x1080 8bit input and 8bit output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
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
        {{0},{0}}
        #endif
    },
    /* 48 Thread 0 TestCase 1-in 4-out 1920x1080 YUV422I 8bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            { 0 },
            { 0 }
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
                UFALSE
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
        #endif
    },
    /* 49 Thread 1 TestCase 1-in 4-out 1920x1080 YUV422I 8bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS8_PACKED
            },
            { 0 },
            { 0 },
            { 0 }
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
                UFALSE
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
        #endif
    },
    /* 50 Thread 0 TestCase 1-in 4-out 1920x1080 YUV422I 12bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            {
                960, 540, 960*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            {
                1728, 972, 1728*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            {
                1536, 864, 1536*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            { 0 },
            { 0 }
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
                UFALSE
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
        #endif
    },
    /* 51 Thread 1 TestCase 1-in 4-out 1920x1080 YUV422I 12bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            {
                960, 540, 960*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            {
                1728, 972, 1728*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            {
                1536, 864, 1536*3,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_PACKED
            },
            { 0 },
            { 0 },
            { 0 }
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
                UFALSE
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
        #endif
    },
    /* 52 (J721S2) Instance 1 TestCase 1-in 4-out 1920x1080 YUV422I 12bit unpacked input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*4,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                960, 540, 960*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                1728, 972, 1728*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                1536, 864, 1536*4,
                1920, 1080,
                FVID2_DF_YUV422I_YUYV,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            { 0 },
            { 0 }
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
                UFALSE
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
        #endif
    },
    /* 53 Thread 1 TestCase 1-in 5-out 1920x1080 Y12 UV8  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270/2, 480,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540/2, 960,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972/2, 1728,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864/2, 1536,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE,
        {
        },
        {
            UTRUE,
            1920,
            FVID2_CCSF_BITS8_PACKED,
        },
        #endif
    },
    /* 54 Thread 1 TestCase 1-in 5-out 1920x1080 Y8 UV12  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
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
                480, 270/2, 480*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972/2, 1728*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432/2, 768*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE,
        {
        },
        {
            UTRUE,
            1920*3/2,
            FVID2_CCSF_BITS12_PACKED,
        },
        #endif
    },
    /* 55, Thread 1 TestCase 1-in 5-out 1920x1080 Y8 Y8  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_2PLANES,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_PLANE_2,
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
        },
        UFALSE,
        {
        }
        #endif
    },
    /* 56, Thread 0 TestCase 1-in 5-out 1920x1080 Y12 Y12  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_2PLANES,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE,
        {
        }
        #endif
    },
    /* 57, Thread 1 TestCase 1-in 5-out 1920x1080 Y12 Y12  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_2PLANES,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE,
        {
        }
        #endif
    },
    
    /* 58 Thread 0, 1920x1080 YUV422SP 12bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        #endif
    },
    /* 59 Thread 1, 1920x1080 YUV422SP 12bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
        #endif
    },
    /* 60 Thread 0, 1920x1080 YUV422SP 8bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV422SP_UV,
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
                480, 270, 480,
                1920, 1080,
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
                960, 540, 960,
                1920, 1080,
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
                1728, 972, 1728,
                1920, 1080,
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
                1536, 864, 1536,
                1920, 1080,
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
                768, 432, 768,
                1920, 1080,
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
        #endif
    },
    /* 61 Thread 1, 1920x1080 YUV422SP 8bit packed input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV422SP_UV,
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
                480, 270, 480,
                1920, 1080,
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
                960, 540, 960,
                1920, 1080,
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
                1728, 972, 1728,
                1920, 1080,
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
                1536, 864, 1536,
                1920, 1080,
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
                768, 432, 768,
                1920, 1080,
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
        #endif
    },
    /* 62, THREAD 0 TestCase 1-in 5-out non intereaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 63, THREAD 1 TestCase 1-in 5-out non intereaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 64, THREAD 0 TestCase 1-in 5-out interleaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080/2, 1920*2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 65, THREAD 0 TestCase 1-in 5-out interleaved 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080/2, 1920*2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 66, THREAD 0 TestCase 1-in 5-out YUV420 1920x1080 12bit unpacked input and single channel non interleaved 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 67, THREAD 1 TestCase 1-in 10-out YUV420 1920x1080 12bit unpacked input and single channel non interleaved 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 68, THREAD 0 TestCase 1-in 10-out YUV420 1920x1080 12bit unpacked input and single channel interleaved 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 69, THREAD 1 TestCase 1-in 10-out YUV420 1920x1080 12bit unpacked input and single channel interleaved 12bit unpacked output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
    /* 70, Thread 0 TestCase 1-in 5-out 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
        #endif
    },
    /* 71, Thread 1 TestCase 1-in 5-out 1920x1080 12bit unpacked input and 12bit unpacked output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
        #endif
    },
    /* 72, Thread 0 TestCase 1-in 5-out 1920x1080 Y16 Y16  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_2PLANES,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
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
        },
        UFALSE,
        {
        }
        #endif
    },
    /* 73, Thread 1 TestCase 1-in 5-out 1920x1080 Y16 Y16  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_2PLANES,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_UNPACKED16
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
        },
        UFALSE,
        {
        }
        #endif
    },
    /* 74 Instance 1, 1920x1080 YUV422SP 12bit unpacked input/output  */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*2,
            FVID2_DF_YUV422SP_UV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270, 480*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540, 960*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972, 1728*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864, 1536*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432, 768*2,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
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
        #endif
    },
    /* 75, THREAD 0 TestCase 1-in 10-out 1920x1080 8bit packed input and 8bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
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
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /*76.Vhwa_mscCheckMscCfg() cahnging input format to FVID2_DF_YUV420P and changing the output formats in Vhwa_mscCheckMscCfg()  */
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            { 960, 540, 960*3/2, 1920, 1080, FVID2_DF_YUV422SP_UV, FVID2_CCSF_BITS12_PACKED },
            { 960, 540/2, 960, 1920, 1080/2, FVID2_DF_YUV420SP_VU, FVID2_CCSF_BITS8_PACKED },
        },
        {
            { UTRUE, {0,0}, 0, 0, 4096, 4096, 1, MSC_SINGLE_PHASE_SP_COEFF_0, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_COEFF_SHIFT_8, UTRUE, UTRUE},
            { UTRUE, {0,0}, 0, 0, 4096, 4096, 1, MSC_SINGLE_PHASE_SP_COEFF_0, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_COEFF_SHIFT_8, UTRUE, UTRUE},

        },
        UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },
         },
     /*77.Vhwa_mscCheckMscCfg() cahnging input height and width*/
    {
    UFALSE,
    VPAC_MSC_TEST_INST_ID_0,
    UTRUE,
    { 
        8192, 8192, 1920*3/2,
        FVID2_DF_YUV420P,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS 
    },
    {
        { 960, 540, 960*3/2, 1920, 1080, FVID2_DF_YUV420P, FVID2_CCSF_BITS12_PACKED },
        { 960, 540/2, 960, 1920, 1080/2, FVID2_DF_YUV420P, FVID2_CCSF_BITS8_PACKED },
    },
    {
        { UTRUE, {0,0}, 0, 0, 4096, 4096, 1, MSC_SINGLE_PHASE_SP_COEFF_0, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_COEFF_SHIFT_8, UTRUE, UTRUE}
    },
    UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },
     
    },
     /*78.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_LUMA_ONLYformat*/
   {
    UFALSE,
    VPAC_MSC_TEST_INST_ID_0,
    UTRUE,
    { 
        1920, 1080, 1920*3/2,
        FVID2_DF_LUMA_ONLY,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS 
    },
    {
        { 960, 540, 960*3/2, 1920, 1080, FVID2_DF_YUV420P, FVID2_CCSF_BITS12_PACKED },
        { 960, 540/2, 960, 1920, 1080/2, FVID2_DF_YUV420P, FVID2_CCSF_BITS8_PACKED },
    },
    {
        { UTRUE, {0,0}, 0, 0, 4096, 4096, 1, MSC_SINGLE_PHASE_SP_COEFF_0, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_MULTI_32PHASE_COEFF_SET_1, MSC_COEFF_SHIFT_8, UTRUE, UTRUE}
    },
    UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },
    },
    /*79.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_CHROMA_ONLY*/
   {
    UFALSE,
    VPAC_MSC_TEST_INST_ID_0,
    UTRUE,
    { 
        1920, 1080, 1920*3/2,
        FVID2_DF_CHROMA_ONLY,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS 
    },
    /* output Frame Parameter */
        {
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                480, 270/2, 480*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                960, 540/2, 960*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1728, 972/2, 1728*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                1536, 864/2, 1536*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            {
                768, 432/2, 768*2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
        },
    {
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
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
            UTRUE, UTRUE
        },
    },
        UFALSE,
        {{0},{0}}     
},    
    /*80.Vhwa_mscCheckMscCfg() cahnging input _YUV422SP_VU*/
    {
    UFALSE,
    VPAC_MSC_TEST_INST_ID_0,
    UTRUE,
    { 
        1920, 1080, 1920*3/2,
        FVID2_DF_YUV420SP_VU,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS 
    },
    /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /*81.Vhwa_mscCheckMscCfg() cahnging input _DF_R_GBI*/
    {
    UFALSE,
    VPAC_MSC_TEST_INST_ID_0,
    UTRUE,
    { 
        1920, 1080, 1920*3/2,
        FVID2_DF_R_GBI,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS 
    },
     /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480*2, 270, 480*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960*2, 540, 960*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640, 360, 640,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640*2, 360, 640*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                384, 216, 384,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                384*2, 216, 384*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768*2, 432, 768*2,
                1920*2, 1080,
                FVID2_DF_GBI,
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
        },
        UFALSE,
        {
        },
        {
            UTRUE,
            1920*2,
            FVID2_CCSF_BITS8_PACKED,
        },
    },
    /*82.Vhwa_mscCheckMscCfg() cahnging input _DF_RGI_B*/
    {
    UFALSE,
    VPAC_MSC_TEST_INST_ID_0,
    UTRUE,
    { 
        1920, 1080, 1920*3/2,
        FVID2_DF_RGI_B,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS 
    },
     /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                480*2, 270, 480*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960*2, 540, 960*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640, 360, 640,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                640*2, 360, 640*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                384, 216, 384,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                384*2, 216, 384*2,
                1920*2, 1080,
                FVID2_DF_GBI,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_R,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768*2, 432, 768*2,
                1920*2, 1080,
                FVID2_DF_GBI,
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
        },
        UFALSE,
        {
        },
        {
            UTRUE,
            1920*2,
            FVID2_CCSF_BITS8_PACKED,
        },
    },
    /*83.Vhwa_mscCheckMscCfg() cahnging input _DF_2PLANES*/
    {
    UFALSE,
    VPAC_MSC_TEST_INST_ID_0,
    UTRUE,
    { 
        1920, 1080, 1920*3/2,
        FVID2_DF_2PLANES,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS 
    },
    /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_PLANE_1,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_PLANE_2,
                FVID2_CCSF_BITS12_PACKED
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
        },
        UFALSE,
        {
        }
     },
    /*84.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_YUV422SP_UV*/
    
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
    UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },        
    },
    /*85.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_YUV422SP_VU*/
    {      
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },
    },
    /*86.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_YUV422I_UYVY*/
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422I_UYVY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_3TAPS 
        },
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },
    },         
    /*87.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_YUV422I_YUYV*/
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    }, 
    /*89.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_YUV422SP_VU isEnablesimulprocessing is UFALSE*/
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422SP_VU,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
         UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },
    },  
    /*89.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_YUV422SP_UV isEnablesimulprocessing is UFALSE*/
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV422SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        UFALSE, {}, UFALSE, {}, { UTRUE, 1920, FVID2_CCSF_BITS8_PACKED },
    },
    /*89.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_LUMA_ONLY isEnablesimulprocessing is UFALSE*/
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            {
                1024, 640, 1024*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                512, 320, 512*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                256, 160, 256*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                920, 576, 920*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                818, 512, 818*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                716, 448, 716*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                614, 384, 614*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                408, 256, 408*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                308, 192, 308*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 480, 768*3/2,
                1024, 640,
                FVID2_DF_LUMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {80,40},
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
                UTRUE, {80,40},
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
                UTRUE, {10,10},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
            {
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
                UTRUE, {20,20},
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
        {{0},{0}}
    },   
    /*90.Vhwa_mscCheckMscCfg() cahnging input FVID2_DF_CHROMA_ONLY isEnablesimulprocessing is UFALSE*/
    {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS 
        },
        {
            {0},
            {
                960, 540/2, 960*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1536, 864/2, 1536*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                1152, 648/2, 1152*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                576, 324/2, 576*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
            },
            {0},
            {
                864, 486/2, 864*3/2,
                1920, 1080/2,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_PACKED
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
        },
        UFALSE,
        {
           
        }
    },
     /*92.Vhwa_mscSetHtsCfg() UTRUE == mscPrms -> enableLineSkip*/
     {
        UFALSE,
        VPAC_MSC_TEST_INST_ID_0,
        UFALSE,
        
        { 
            1920, 1080, 1920*3/2,
            FVID2_DF_CHROMA_ONLY,
            FVID2_CCSF_BITS12_PACKED,
            UTRUE, MSC_TAP_SEL_5TAPS 
        },
        {
            { 960, 540, 960*3/2, 1920, 1080, FVID2_DF_CHROMA_ONLY, FVID2_CCSF_BITS12_PACKED },
            { 960, 540/2, 960, 1920, 1080/2, FVID2_DF_LUMA_ONLY, FVID2_CCSF_BITS8_PACKED },
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_3,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_MULTI_32PHASE_COEFF_SET_2,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
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
        },
        UFALSE,
        {
            
        }
    },
    /*93.To cover FVID2_DF_R_GBI input format in function Vhwa_mscCalPrms()*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_R_GBI,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /*94.To cover FVID2_DF_2PLANES input format in function Vhwa_mscCalPrms()*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_2PLANES,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /*95.To cover FVID2_DF_YUV422SP_VU input format in function Vhwa_mscCalPrms()*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV422SP_VU,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /*96. To cover FVID2_DF_YUV420SP_VU input format in function Vhwa_mscCalPrms()*/
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920,
            FVID2_DF_YUV420SP_VU,
            FVID2_CCSF_BITS8_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /*97.To cover FVID2_DF_YUV420SP_VU input format in function Vhwa_mscCalPrms()*/
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
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
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
        {{0},{0}}
    },
    /* 98 output for mat chroma only for FVID2_DF_YUV422I_YUYV input */
    {
        #ifndef VHWA_VPAC_IP_REV_VPAC
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_0,
        UTRUE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*4,
            FVID2_DF_YUV422I_YUYV,
            FVID2_CCSF_BITS12_UNPACKED16,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*4,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                960, 540, 960*4,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                1728, 972, 1728*4,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            {
                1536, 864, 1536*4,
                1920, 1080,
                FVID2_DF_CHROMA_ONLY,
                FVID2_CCSF_BITS12_UNPACKED16
            },
            { 0 },
            { 0 },
            { 0 }
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_SINGLE_PHASE,
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
                UTRUE, UTRUE
            },
            {
                UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_MULTI_PHASE,
                MSC_PHASE_MODE_64PHASE,
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
        #endif
    },

     /* 99, THREAD 0 TestCase 1-in 10-out 1920x1080 8bit packed input and 8bit packed output  
     Vhwa_mscCheckMscCfg*/
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
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                960, 540, 960,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1728, 972, 1728,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1536, 864, 1536,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1344, 756, 1344,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1152, 648, 1152,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                768, 432, 768,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                576, 324, 576,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                1248, 702, 1248,
                1920, 1080,
                FVID2_DF_INVALID,
                FVID2_CCSF_BITS8_PACKED
            },
            {
                864, 486, 864,
                1920, 1080,
                FVID2_DF_INVALID,
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
        {{0},{0}}
    },
    /* 100, Instance 1 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output  */
    {
        UFALSE /*loopBack*/,
        VPAC_MSC_TEST_INST_ID_1,
        UFALSE,
        /* Input Frame Parameter */
        {
            1920, 1080, 1920*3/2,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED,
            UFALSE, MSC_TAP_SEL_5TAPS
        },
        /* output Frame Parameter */
        {
            {
                480, 270, 480*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                960, 540, 960*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1728, 972, 1728*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1536, 864, 1536*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1344, 756, 1344*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1152, 648, 1152*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                768, 432, 768*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                576, 324, 576*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                1248, 702, 1248*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
            {
                864, 486, 864*3/2,
                1920, 1080,
                FVID2_DF_YUV420SP_UV,
                FVID2_CCSF_BITS12_PACKED
            },
        },
        /* MSC Configuration for each instance */
        {
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_SINGLE_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_SP_COEFF_1,
                MSC_SINGLE_PHASE_SP_COEFF_1,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_MULTI_32PHASE_COEFF_SET_0,
                MSC_COEFF_SHIFT_8,
                UFALSE, UFALSE
            },
            {
                UTRUE, {0,0},
                0, 0,
                MSC_FILTER_MODE_SINGLE_PHASE,
                MSC_PHASE_MODE_32PHASE,
                MSC_SINGLE_PHASE_MP_COEFF0_0,
                MSC_SINGLE_PHASE_MP_COEFF0_0,
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
                UFALSE, UTRUE
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
                UTRUE, UFALSE
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
        UTRUE,
        {
            {
                0x6b2879b5, 0x837004e8, 0x1f27aa63, 0x81cd6505, 0x60e53bbb,
                0x8bf84e75, 0x252ef6cf, 0x8e774c1e, 0x8ed21063, 0xdf4fef68
            },
            {
                0x816382c6, 0x2d236640, 0xdcb1a0e3, 0x5a9810b0, 0x3f9c45c2,
                0x2b135a8, 0x8cd80995, 0xa2ea8425, 0xc49b5642, 0xe9d4b85c
            }
        }
    },
/* 101, THREAD 0 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
{
    UFALSE /*loopBack*/,
    VPAC_MSC_TEST_INST_ID_0,
    UFALSE,
    /* Input Frame Parameter */
    {
        8100, 8100, 8100*3/2,
        FVID2_DF_YUV420SP_UV,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS
    },
    /* output Frame Parameter */
    {
        {
            8100, 8100, 8100*3/2,
            8100, 8100,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            8100, 8100, 8100*3/2,
            8100, 8100,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            8100, 8100, 8100*3/2,
            8100, 8100,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            8100, 8100, 8100*3/2,
            8100, 8100,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            8100, 8100, 8100*3/2,
            8100, 8100,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
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
        {0x6b2879b5, 0x837004e8, 0x1f27aa63, 0x81cd6505, 0x60e53bbb},
        {0x816382c6, 0x2d236640, 0xdcb1a0e3, 0x5a9810b0, 0x3f9c45c2}
    }
},
/* 102, THREAD 1 TestCase 1-in 5-out 1920x1080 12bit packed input and 12bit packed output  */
{
    UFALSE /*loopBack*/,
    VPAC_MSC_TEST_INST_ID_1,
    UFALSE,
    /* Input Frame Parameter */
    {
        100, 100, 100*3/2,
        FVID2_DF_LUMA_ONLY,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS
    },
    /* output Frame Parameter */
    {
        {
            100, 100, 100*3/2,
            100, 100,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            100, 100, 100*3/2,
            100, 100,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            100, 100, 100*3/2,
            100, 100,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            100, 100, 100*3/2,
            100, 100,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            100, 100, 100*3/2,
            100, 100,
            FVID2_DF_LUMA_ONLY,
            FVID2_CCSF_BITS12_PACKED
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
        {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE}, {UFALSE},
    },
    UFALSE,
    {
        {0, 0, 0, 0, 0,
         0x8bf84e75, 0x252ef6cf, 0x8e774c1e, 0x8ed21063, 0xdf4fef68},
        {0, 0, 0, 0, 0,
         0x2b135a8, 0x8cd80995, 0xa2ea8425, 0xc49b5642, 0xe9d4b85c}
    }
},
/* 103, Instance 1 TestCase 1-in 10-out 1920x1080 12bit packed input and 12bit packed output  */
{
    UFALSE /*loopBack*/,
    VPAC_MSC_TEST_INST_ID_1,
    UFALSE,
    /* Input Frame Parameter */
    {
        1920, 1080, 1920*3/2,
        FVID2_DF_YUV420SP_UV,
        FVID2_CCSF_BITS12_PACKED,
        UFALSE, MSC_TAP_SEL_5TAPS
    },
    /* output Frame Parameter */
    {
        {
            480, 270, 480*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            960, 540, 960*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            480, 270, 1728*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            480, 864, 1536*3/2,
            480, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            1344, 756, 1344*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            1152, 648, 1152*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            768, 432, 768*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            576, 324, 576*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            1248, 702, 1248*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
        {
            864, 486, 864*3/2,
            1920, 1080,
            FVID2_DF_YUV420SP_UV,
            FVID2_CCSF_BITS12_PACKED
        },
    },
    /* MSC Configuration for each instance */
    {
        {
            UTRUE, {0,0},
            0, 0,
            MSC_FILTER_MODE_SINGLE_PHASE,
            MSC_PHASE_MODE_32PHASE,
            MSC_SINGLE_PHASE_SP_COEFF_1,
            MSC_SINGLE_PHASE_SP_COEFF_1,
            MSC_MULTI_32PHASE_COEFF_SET_0,
            MSC_MULTI_32PHASE_COEFF_SET_0,
            MSC_COEFF_SHIFT_8,
            UFALSE, UTRUE
        },
        {
            UTRUE, {0,0},
            0, 0,
            MSC_FILTER_MODE_MULTI_PHASE,
            MSC_PHASE_MODE_64PHASE,
            MSC_SINGLE_PHASE_MP_COEFF0_0,
            MSC_SINGLE_PHASE_MP_COEFF0_0,
            MSC_MULTI_32PHASE_COEFF_SET_3,
            MSC_MULTI_32PHASE_COEFF_SET_3,
            MSC_COEFF_SHIFT_8,
            UFALSE, UFALSE
        },
        {
            UTRUE, {0,0},
            0, 0,
            MSC_FILTER_MODE_SINGLE_PHASE,
            MSC_PHASE_MODE_32PHASE,
            MSC_SINGLE_PHASE_SP_COEFF_0,
            MSC_SINGLE_PHASE_SP_COEFF_0,
            MSC_MULTI_32PHASE_COEFF_SET_2,
            MSC_MULTI_32PHASE_COEFF_SET_0,
            MSC_COEFF_SHIFT_8,
            UFALSE, UTRUE
        },
        {
            UTRUE, {0,0},
            0, 0,
            MSC_FILTER_MODE_SINGLE_PHASE,
            MSC_PHASE_MODE_32PHASE,
            MSC_SINGLE_PHASE_SP_COEFF_0,
            MSC_SINGLE_PHASE_SP_COEFF_0,
            MSC_MULTI_32PHASE_COEFF_SET_1,
            MSC_MULTI_32PHASE_COEFF_SET_0,
            MSC_COEFF_SHIFT_8,
            UTRUE, UFALSE
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
            UFALSE, UTRUE
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
    UTRUE,
    {
        {
            0x6b2879b5, 0x837004e8, 0x1f27aa63, 0x81cd6505, 0x60e53bbb,
            0x8bf84e75, 0x252ef6cf, 0x8e774c1e, 0x8ed21063, 0xdf4fef68
        },
        {
            0x816382c6, 0x2d236640, 0xdcb1a0e3, 0x5a9810b0, 0x3f9c45c2,
            0x2b135a8, 0x8cd80995, 0xa2ea8425, 0xc49b5642, 0xe9d4b85c
        }
    }
},
