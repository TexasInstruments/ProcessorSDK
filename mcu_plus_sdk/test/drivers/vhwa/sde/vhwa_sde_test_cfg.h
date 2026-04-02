    /* 0 : SDE 2048x1024 test  SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        3072, 3072,
        (2048 * 2),
        1, 1, 2048, 1024, 0, (uint32_t)SDE_SR_192,
        0xBD, 1, 0x18, 0x9, 0x26, 1, 1,
        FALSE, 0x98BEAB34FA0978E4, /* CRC */
    },
    /* 1 : SDE 1280x720 test SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_192,
        0xBD, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0x2AD39088BD4068E0, /* CRC */
    },
    /* 2 : SDE 1280x720 16 bit SR 192*/
    {
        FVID2_CCSF_BITS12_UNPACKED16,
        2560, 2560,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_192,
        0xBD, 1, 0x18, 0x9, 0x26, 0, 0,
        FALSE, 0x2AD39088BD4068E0, /* CRC */
        {0, 0, 0}
    },
    /* 3 : SDE 1280x720 8 bit SR 192*/
    {
        FVID2_CCSF_BITS8_PACKED,
        1280, 1280,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_192,
        0xBD, 1, 0x18, 0x9, 0x26, 0, 0,
        FALSE, 0x80EEF30E38943189, /* CRC */
        {0, 0, 0}
    },
    /* 4 : SDE 1280x720 SR 192 -ve Disparity test*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 1, 1280, 720, 1 /* min dis -3*/, (uint32_t)SDE_SR_192,
        0x10, 1, 0x18, 0x9, 0x26, 0, 0,
        FALSE, 0x0831E4AA3ED6E204, /* CRC */
    },
    /* 5 : SDE 1280x720 test SR 128*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_128,
        0x5D, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0xC323A770668299AC, /* CRC */
    },
    /* 6 : SDE 1280x720 Unpacked test SR 128*/
    {
        FVID2_CCSF_BITS12_UNPACKED16,
        2560, 2560,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_128,
        0x5D, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0xC323A770668299AC, /* CRC */
    },
    /* 7 : SDE 1280x720 test SR 192 Texture Filter Disabled*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_192,
        0x3D, 0, 0, 0x9, 0x26, 0, 0,
        FALSE, 0x39F0B36E97CF8EA5, /* CRC */
    },
    /* 8 : SDE 1280x720 test SR 192 Median Filter disabled*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 0, 1280, 720, 0, (uint32_t)SDE_SR_192,
        0x3D, 1, 0x18, 0x9, 0x26, 0, 0,
        FALSE, 0xC3C36D84972293EA, /* CRC */
    },
    /* 9 : SDE 1280x720 test SR 192 LR disabled*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_192,
        0xFF, 1, 0x18, 0x9, 0x26, 0, 0,
        FALSE, 0x2AD39088BD4068E0, /* CRC */
    },
    /* 10 : SDE 1280x720 test SR 64*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 1, 1280, 720, 0, (uint32_t)SDE_SR_64,
        0x3D, 1, 0x18, 0x9, 0x26, 0, 0,
        FALSE, 0xE4E380BD3260D92D, /* CRC */
    },
    /* 11 : SDE 1296x720 test SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1944, 1944,
        (1344 * 2),
        1, 1, 1296, 720, 0, (uint32_t)SDE_SR_192,
        0x3D, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0xD305BA147B50E2A4, /* CRC */
    },
    /* 12 : SDE 1312x720 test SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1968, 1968,
        (1344 * 2),
        1, 1, 1312, 720, 0, (uint32_t)SDE_SR_192,
        0x7D, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0x0142DBDA21CBBB21, /* CRC */
    },
    /* 13 : SDE 1328x720 test SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1992, 1992,
        (1344 * 2),
        1, 1, 1328, 720, 0, (uint32_t)SDE_SR_192,
        0x7D, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0x9AEFBA163232A1A4, /* CRC */
    },
    /* 14 : SDE 1280x704 test SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1920, 1920,
        (1280 * 2),
        1, 1, 1280, 704, 0, (uint32_t)SDE_SR_192,
        0x7D, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0xC5DDF02FEF6829AA, /* CRC */
    },
    /* 15 : SDE 1296x736 test SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        1944, 1944,
        (1344 * 2),
        1, 1, 1296, 736, 0, (uint32_t)SDE_SR_192,
        0x7D, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0xA0E3325A0F2AA139, /* CRC */
    },
    /* 16 : SDE 2048x1024 test  SR 192*/
    {
        FVID2_CCSF_BITS12_PACKED,
        3072, 3072,
        (2048 * 2),
        1, 1, 2048, 1024, 0, (uint32_t)SDE_SR_192,
        0xBD, 1, 0x18, 0x9, 0x26, 1, 0,
        FALSE, 0x98BEAB34FA0978E4, /* CRC */
    },
    /* 17 : SDE 128x64 test  SR 64*/
    {
        FVID2_CCSF_BITS12_PACKED,
        192, 192,
        (128 * 2),
        1, 1, 128, 64, 0, (uint32_t)SDE_SR_64,
        0x3D, 1, 0x18, 0x9, 0x26, 1, 1,
        FALSE, 0x0, /* CRC */
    },
