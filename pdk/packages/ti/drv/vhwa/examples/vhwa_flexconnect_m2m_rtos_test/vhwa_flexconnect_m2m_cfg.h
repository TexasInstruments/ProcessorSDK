
#ifndef APP_FC_M2M_CFG_H_
#define APP_FC_M2M_CFG_H_

static App_MscTestCfg gAppMscTestCfg[] =
{
    #include <examples/include/vhwa_msc_test_cfg.h>
};

static AppLdc_TestConfig gAppLdcTestCfg[] =
{
    #include <examples/include/vhwa_ldc_test_cfg.h>
};

AppFcM2m_TestPrms gAppFcM2mTestPrms[] =
{
    /* 0: VISS+MSC0_1+LDC */
    {
        .testName                       = "TC_FC_M2M_000",        /* Test Name */
        .numHandles                     = 2,                      /* Num Handles */
        .repeatCnt                      = 10,                      /* Repeat Count */
        .isM2mVissEnabled               = UTRUE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UTRUE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UTRUE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UTRUE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UFALSE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {0},
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },
    /* 1: Flexconnect(VISS->MSC0(6)) + VISS */
    {
        .testName                       = "TC_FC_M2M_001",        /* Test Name */
        .numHandles                     = 2,                      /* Num Handles */
        .repeatCnt                      = 50,                      /* Repeat Count */
        .isM2mVissEnabled               = UTRUE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UFALSE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UFALSE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UFALSE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_112",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[112]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UFALSE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UFALSE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC020",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UFALSE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UFALSE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[20],
                        &gAppVissFcTestConfig[11],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UFALSE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_112",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[112]},     /* Test Config */
                    .isEnableTest       =  UFALSE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UFALSE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UFALSE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC020",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                            0
                        },
                        {
                        &gPathInfo[20],
                        &gAppVissFcTestConfig[11],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },
    /* 2: Flexconnect(VISS->MSC0(6)) + MSC0(4) */
    {
        .testName                       = "TC_FC_M2M_002",        /* Test Name */
        .numHandles                     = 2,                      /* Num Handles */
        .repeatCnt                      = 10,                      /* Repeat Count */
        .isM2mVissEnabled               = UFALSE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UTRUE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UFALSE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UFALSE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeat Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UFALSE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UFALSE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {0},
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },
    /* 3: Flexconnect + MSC1 */
    {
        .testName                       = "TC_FC_M2M_003",        /* Test Name */
        .numHandles                     = 2,                      /* Num Handles */
        .repeatCnt                      = 10,                      /* Repeat Count */
        .isM2mVissEnabled               = UFALSE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UFALSE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UTRUE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UFALSE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {0},
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },
    /* 4: Flexconnect + LDC */
    {
        .testName                       = "TC_FC_M2M_004",        /* Test Name */
        .numHandles                     = 2,                      /* Num Handles */
        .repeatCnt                      = 10,                      /* Repeat Count */
        .isM2mVissEnabled               = UFALSE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UFALSE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UFALSE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UTRUE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UFALSE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UFALSE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                            0
                        },
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },
    /* 5: Flexconnect + MSC + LDC */
    {
        .testName                       = "TC_FC_M2M_004",        /* Test Name */
        .numHandles                     = 2,                      /* Num Handles */
        .repeatCnt                      = 10,                      /* Repeat Count */
        .isM2mVissEnabled               = UFALSE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UFALSE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UTRUE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UTRUE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC31",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[31U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC019",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UTRUE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UFALSE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                            0
                        },
                        {
                        &gPathInfo[19],
                        &gAppVissFcTestConfig[9],
                        &gAppMscFcTestCfg[16],
                        NULL,
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },
    /* 6: Flexconnect(VISS->MSC1(6)) + MSC1(4) */
    {
        .testName                       = "TC_FC_M2M_006",        /* Test Name */
        .numHandles                     = 2,                      /* Num Handles */
        .repeatCnt                      = 10,                      /* Repeat Count */
        .isM2mVissEnabled               = UFALSE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UFALSE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UTRUE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UFALSE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC021",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeat Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UFALSE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UTRUE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[21],
                        &gAppVissFcTestConfig[9],
                        NULL,
                        &gAppMscFcTestCfg[17]
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UFALSE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UFALSE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC32",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[32U] },                                            \
                    .isEnableTest       = UFALSE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC021",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UFALSE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UTRUE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {0},
                        {
                        &gPathInfo[21],
                        &gAppVissFcTestConfig[9],
                        NULL,
                        &gAppMscFcTestCfg[17]
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },
    /* 7: Flexconnect(VISS->MSC1(10)) + MSC1(10) + VISS(5) */
    {
        .testName                       = "TC_FC_M2M_007",        /* Test Name */
        .numHandles                     = 3,                      /* Num Handles */
        .repeatCnt                      = 10,                      /* Repeat Count */
        .isM2mVissEnabled               = UTRUE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UFALSE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UTRUE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UFALSE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC23",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[23U] },                                            \
                    .isEnableTest       = UFALSE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC017",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeat Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UFALSE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UTRUE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[17],
                        &gAppVissFcTestConfig[9],
                        NULL,
                        &gAppMscFcTestCfg[15]
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UFALSE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UFALSE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UFALSE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC23",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[23U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC017",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UFALSE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UTRUE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {0},
                        {
                        &gPathInfo[17],
                        &gAppVissFcTestConfig[9],
                        NULL,
                        &gAppMscFcTestCfg[15]
                        }, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UFALSE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {0U, 0U, &gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UFALSE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, &gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UFALSE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC23",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {0U, 0U, &gAppMscTestCfg[23U] },                                            \
                    .isEnableTest       = UFALSE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {0U, &gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC017",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeate Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UFALSE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UTRUE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {0},{0},
                        {
                        &gPathInfo[17],
                        &gAppVissFcTestConfig[9],
                        NULL,
                        &gAppMscFcTestCfg[15]
                        },{0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },

    /* 8: Flexconnect with MSC0(ROI) */
    {
        .testName                       = "TC_FC_M2M_008",        /* Test Name */
        .numHandles                     = 1,                      /* Num Handles */
        .repeatCnt                      = 50,                      /* Repeat Count */
        .isM2mVissEnabled               = UFALSE,                 /* isM2mVissEnabled */
        .isM2mMsc0Enabled               = UFALSE,                  /* isM2mMsc0Enabled */
        .isM2mMsc1Enabled               = UFALSE,                 /* isM2mMsc1Enabled */
        .isM2mLdcEnabled                = UFALSE,                 /* isM2mLdcEnabled */
        .isFcEnabled                    = UTRUE,                  /* isFlexconnectEnabled */
        .isParallel                     = UFALSE,                 /* is Flexconnect and M2M running in parallel */
        .testCfg                        =                         /* Test Config */
        {
            {
                                                /* M2M VISS Config */
                {            
                    .testName           =  "TC_VISS_FUNC_000",                   /* Test Name */
                    .numHandles         =  1u,                          /* Num Handles */
                    .repeatCnt          =  1u,                        /* Repeate Count */
                    .isPerformanceTest  =  UFALSE,                       /* Is Performance */
                    .testCfg            =  {&gAppVissTestConfig[0]},     /* Test Config */
                    .isEnableTest       =  UTRUE,
                    .isSwitchGlbceCtx   =  UFALSE,
                    .chCfgOnEachIter    =  UFALSE
                },
                                                /* M2M MSC0 Config */
                {
                    .testName           = "TC_MSC0_FUNC_TC33",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[33U] },                                            \
                    .isEnableTest       = UTRUE,                                                              \
                },
                                                /* M2M MSC1 Config */
                {
                    .testName           = "TC_MSC1_FUNC_TC34",                                               \
                    .numHandles         = 1u,                     /* NumHandles */                           \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                           \
                    .isPerformanceTest  = UFALSE,                 /* Is Performance Test */                  \
                    .testCfg            = {&gAppMscTestCfg[34U] },                                            \
                    .isEnableTest       = UTRUE,             
                },
                                                /* M2M LDC Config */
                {
                    .testName           = "TC_LDC_FUNC_003",      /* Test Name */                                \
                    .numHandles         = 1u,                     /* Num Handles */                              \
                    .repeatCnt          = 1u,                     /* Repeat Cnt */                               \
                    .isPerformanceTest  = UFALSE,                  /* Is Performance Test */                      \
                    .testCfg            = {&gAppLdcTestCfg[3U]},                                                  \
                    .isEnableTest       = UTRUE            
                },                
                                                /* FlexConnect Config */
                {
                    .testName           = "TC_FC_FUNC_TC022",     /* Test Name */
                    .numHandles         = 1u,                      /* Num Handles */
                    .repeatCnt          = 1u,                      /* Repeat Count */
                    .isPerformanceTest  = UFALSE,                 /* Is Performance */
                    .isVissEnabled      = UTRUE,                  /* isVissEnabled */
                    .isMsc0Enabled      = UFALSE,                  /* Is Msc0Enabled */
                    .isMsc1Enabled      = UTRUE,                 /* isMsc1Enabled */
                    .testCfg =                                    /* Test Config */
                    {
                        {
                        &gPathInfo[22],
                        &gAppVissFcTestConfig[9],
                        NULL,
                        &gAppMscFcTestCfg[18]
                        }, {0}, {0}, {0}
                    },
                    /* Disabled for J721E */
                    #if defined (VHWA_VPAC_IP_REV_VPAC)
                    .isEnableTest       = UFALSE,                  /* isEnableTest */
                    #else
                    .isEnableTest       = UTRUE,                   /* isEnableTest */
                    #endif
                    .vissIsSwitchGlbceCtx   = UFALSE,             /* vissIsSwitchGlbceCtx */
                    .vissChCfgOnEachIter    = UFALSE,             /* vissChCfgOnEachIter  */
                },
            },
            {0},{0},{0}
        },
        /* Disabled for J721E */
        #if defined (VHWA_VPAC_IP_REV_VPAC)
        .isEnableTest = false,            /* isEnableTest */
        #else
        .isEnableTest = true,            /* isEnableTest */
        #endif
    },

    /* 9: Flexconnect + MSC0 + VISS + LDC */
};
#endif
