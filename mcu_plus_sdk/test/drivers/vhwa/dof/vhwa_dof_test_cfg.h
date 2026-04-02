    /* 0 : 2MP 2048x1024 test case median enabled
       CS histogram output is diabled search range is 191, +- 48*/
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 1, 1,
        FALSE, 0x68CFA5BB90E741C8, /* CRC */
    },
    /* 1 : 1MP 1312x736 test case median enabled
       CS histogram output is disabled search range is 170, +- 62*/
    {
        1, 5,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED, FVID2_CCSF_BITS12_PACKED,
        FVID2_CCSF_BITS12_PACKED},
        {2048,1024,512,256,128,0,0},
        {2048,1024,512,256,128,0,0},
        1312*4, 0, 0, (1312 * 4),
        1/*Enable DOF*/, 1312, 736, 170, 62, 62,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 1, 0,
        FALSE, 0x4C8C4698567D7D34, /* CRC */
    },
    /* 2 : 2MP 2048x1024 unpacked test case median enabled
       CS histogram output is enabled search range is 191, +- 48*/
    {
        1, 6,
        {FVID2_CCSF_BITS12_UNPACKED16,FVID2_CCSF_BITS12_UNPACKED16,
         FVID2_CCSF_BITS12_UNPACKED16,FVID2_CCSF_BITS12_UNPACKED16,
         FVID2_CCSF_BITS12_UNPACKED16,FVID2_CCSF_BITS12_UNPACKED16},
        {4096,2048,1024,512,256,128,0},
        {4096,2048,1024,512,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 0, 0,
        FALSE, 0x68CFA5BB90E741C8, /* CRC */
        {0, 0, 0}
    },
    /* 3 : 2MP 2048x1024 8 bit test case median enabled, Temporal disabled
       CS histogram output is enabled search range is 191, +- 48*/
    {
        1, 6,
        {FVID2_CCSF_BITS8_PACKED, FVID2_CCSF_BITS8_PACKED,
         FVID2_CCSF_BITS8_PACKED, FVID2_CCSF_BITS8_PACKED,
        FVID2_CCSF_BITS8_PACKED,FVID2_CCSF_BITS8_PACKED},
        {2048,1024,512,256,128,64,0},
        {2048,1024,512,256,128,64,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        1, 1, 0, 0, 0, 0, 24, 0x66, 0, 0,
        FALSE, 0xD3FA98E745DD9C2E, /* CRC */
        {4, 0, 0}
    },
    /* 4 : 2MP SOF 2048x1024 test case median enabled
       search range is 191, +- 48*/
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 256, 1024, (125*4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 1, 0, 0, 100, 24, 0x66, 0, 0,
        FALSE,0x146A39DFDABA6BEB
    },
    /* 5 : 1MP SOF 1312x736 test case median enabled
       CS histogram output is disabled search range is 170, +- 62*/
    {
        1, 5,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED},
        {2048,1024,512,256,128,0,0},
        {2048,1024,512,256,128,0,0},
        1312*4, 164, 736, (384),
        1/*Enable DOF*/, 1312, 736, 170, 62, 62,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 1, 0, 0, 90, 24, 0x66, 0, 0,
        FALSE,0x4119955987285FD5
    },
    /* 6 : 1MP 1312x736 test case median enabled
       CS histogram output is disabled search range is 191, +- 48*/
    {
        1, 5,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED},
        {2048,1024,512,256,128,0,0},
        {2048,1024,512,256,128,0,0},
        1312*4, 0, 0, (1312 * 4),
        1/*Enable DOF*/, 1312, 736, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 1, 0,
        FALSE, 0xDDEA7222C1D10934, /* CRC */
    },
    /* 7 : 1MP 1312x736 unpacked test case median enabled
       CS histogram output is disabled search range is 191, +- 48*/
    {
        1, 5,
        {FVID2_CCSF_BITS12_UNPACKED16,FVID2_CCSF_BITS12_UNPACKED16,
         FVID2_CCSF_BITS12_UNPACKED16,FVID2_CCSF_BITS12_UNPACKED16,
         FVID2_CCSF_BITS12_UNPACKED16,FVID2_CCSF_BITS12_UNPACKED16},
        {2688,1408,768,384,256,0,0},
        {2688,1408,768,384,256,0,0},
        1312*4, 0, 0, (1312 * 4),
        1/*Enable DOF*/, 1312, 736, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 1, 0,
        FALSE, 0xDDEA7222C1D10934, /* CRC */
    },
    /* 8 : 1MP 1312x736 test case median enabled
       CS histogram output is Enabled search range is 63, +- 31*/
    {
        1, 5,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED},
        {2048,1024,512,256,128,0,0},
        {2048,1024,512,256,128,0,0},
        1312*4, 0, 0, (1312*4),
        1/*Enable DOF*/, 1312, 736, 63, 31, 31,
        DOF_PREDICTOR_NONE,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 0, 1,
        FALSE, 0x7B3507AE0765BA20, /* CRC */
    },
    /* 9 : 2MP 2048x1024 test case median enabled
       search range is 191, +- 48, No Pyr Colocated */
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 0, 0,
        FALSE, 0x71E2F51534D8A3E9, /* CRC */
    },
    /* 10 : 2MP 2048x1024 test case median enabled
       search range is 191, +- 48, No Pyr Left */
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_DELEY_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 0, 0,
        FALSE, 0x550E9E0283F70587, /* CRC */
    },
    /* 11 : 2MP 2048x1024 test case median enabled
       search range is 191, +- 48, Ref CT */
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 1, 0, 24, 0x66, 0, 0,
        FALSE, 0x587E658522753542, /* CRC */
    },
    /* 12 : 2MP 2048x1024 test case median enabled
       search range is 191, +- 48, Cur CT */
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 1, 0, 0, 24, 0x66, 0, 0,
        FALSE, 0x05F71FE1F20861E8, /* CRC */
    },
    /* 13 : 2MP 2048x1024 test case median enabled
       search range is 191, +- 48, Ref and Cur CT */
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 1, 1, 0, 24, 0x66, 0, 0,
        FALSE, 0x2F48CF14A1F32763, /* CRC */
    },
    /* 14 : 2MP 2048x1024 test case median disabled
       search range is 191, +- 48 */
    {
        1, 6,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED},
        {3072,1536,768,384,256,128,0},
        {3072,1536,768,384,256,128,0},
        2048*4, 0, 0, (2048 * 4),
        1/*Enable DOF*/, 2048, 1024, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 0, 0, 0, 0, 0, 24, 0x66, 0, 0,
        FALSE, 0x8985CFE8634EAF78, /* CRC */
    },
    /* 15 : 0.5MP 1024x512 test case median enabled
       search range is 191, +- 48, Performancae test case */
    {
        1, 5,
        {FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED,FVID2_CCSF_BITS12_PACKED,
         FVID2_CCSF_BITS12_PACKED},
        {1536,768,384,256,128,0,0},
        {1536,768,384,256,128,0,0},
        1024*4, 0, 0, (1024 * 4),
        1/*Enable DOF*/, 1024, 512, 191, 48, 48,
        DOF_PREDICTOR_DELEY_LEFT,
        DOF_PREDICTOR_PYR_LEFT, DOF_PREDICTOR_PYR_COLOCATED,
        DOF_PREDICTOR_PYR_COLOCATED, DOF_PREDICTOR_TEMPORAL,
        1, 1, 0, 0, 0, 0, 24, 0x66, 0, 0,
        FALSE, 0, /* CRC */
    },
