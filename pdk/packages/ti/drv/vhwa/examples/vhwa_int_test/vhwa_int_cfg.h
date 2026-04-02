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
*  \file vhwa_int_tirtos_cfg.h
*
*  \brief Configuration for TI RTOS
*/
#ifndef VHWA_INT_TIRTOS_CFG_
#define VHWA_INT_TIRTOS_CFG_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define VHWA_INT_TIRTOS_CFG                                                    \
{                                                                              \
    {                                                                          \
        "TC_INT_TC0",      /* Test Name */                                     \
        UTRUE,              /* Enable */                                        \
        1u,                /* Num Handles */                                   \
        10u,                /* Repeat Cnt */                                    \
        UFALSE,              /* LDC Enable */                                    \
        {                                                                      \
            "TC_LDC_FUNC_001",      /* Test Name */                            \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppLdcTestCfg[0U]                                            \
            },                                                                 \
            UTRUE                                                               \
        },                                                                     \
        UFALSE,              /* NF Enable */                                     \
        {                                                                      \
            "TC_NF_FUNC_001",      /* Test Name */                             \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppNfTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* MSC0 Enable */                                   \
        {                                                                      \
            "TC_MSC0_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[0U] },                                            \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* MSC1 Enable */                                  \
        {                                                                      \
            "TC_MSC1_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[1U] },                                           \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* SDE Enable */                                    \
        {                                                                      \
            "TC_SDE_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppSdeTestCfg[5U]                                            \
            },                                                                 \
            {                                                                  \
                gSdeConfScoreMap[0U]                                           \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* DOF Enable */                                    \
        {                                                                      \
            "TC_DOF_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppDofTestCfg[6U]                                            \
            },                                                                 \
        },                                                                     \
        UTRUE,              /* VISS Enable */                                   \
        {                                                                      \
            "TC_VISS_FUNC_001",                   /* Test Name */              \
            1,                          /* Num Handles */                      \
            1,                          /* Repeate Count */                    \
            UFALSE,                       /* Is Performance */                   \
            {&gAppVissTestConfig[0U]},     /* Test Config */                    \
            UTRUE,                                                              \
            UFALSE,                                                             \
            UFALSE                                                              \
        },                                                                     \
                                                                               \
    },                                                                                 \
    {                                                                          \
        "TC_INT_TC1",      /* Test Name */                                     \
        UTRUE,              /* Enable */                                        \
        1u,                /* Num Handles */                                   \
        10u,                /* Repeat Cnt */                                    \
        UTRUE,              /* LDC Enable */                                    \
        {                                                                      \
            "TC_LDC_FUNC_001",      /* Test Name */                            \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppLdcTestCfg[0U]                                            \
            },                                                                 \
            UTRUE                                                               \
        },                                                                     \
        UFALSE,              /* NF Enable */                                     \
        {                                                                      \
            "TC_NF_FUNC_001",      /* Test Name */                             \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppNfTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* MSC0 Enable */                                   \
        {                                                                      \
            "TC_MSC0_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[0U] },                                            \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* MSC1 Enable */                                  \
        {                                                                      \
            "TC_MSC1_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[1U] },                                           \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* SDE Enable */                                    \
        {                                                                      \
            "TC_SDE_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppSdeTestCfg[5U]                                            \
            },                                                                 \
            {                                                                  \
                gSdeConfScoreMap[0U]                                           \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* DOF Enable */                                    \
        {                                                                      \
            "TC_DOF_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppDofTestCfg[6U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* VISS Enable */                                   \
        {                                                                      \
            "TC_VISS_FUNC_001",                   /* Test Name */              \
            1,                          /* Num Handles */                      \
            1,                          /* Repeate Count */                    \
            UFALSE,                       /* Is Performance */                   \
            {&gAppVissTestConfig[0U]},     /* Test Config */                    \
            UTRUE,                                                              \
            UFALSE,                                                             \
            UFALSE                                                              \
        },                                                                     \
                                                                               \
    },                                                                 \
    {                                                                          \
        "TC_INT_TC2",      /* Test Name */                                     \
        UTRUE,              /* Enable */                                        \
        1u,                /* Num Handles */                                   \
        10u,                /* Repeat Cnt */                                    \
        UFALSE,              /* LDC Enable */                                    \
        {                                                                      \
            "TC_LDC_FUNC_001",      /* Test Name */                            \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppLdcTestCfg[0U]                                            \
            },                                                                 \
            UTRUE                                                               \
        },                                                                     \
        UFALSE,              /* NF Enable */                                     \
        {                                                                      \
            "TC_NF_FUNC_001",      /* Test Name */                             \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppNfTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UTRUE,              /* MSC0 Enable */                                   \
        {                                                                      \
            "TC_MSC0_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[0U] },                                            \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* MSC1 Enable */                                  \
        {                                                                      \
            "TC_MSC1_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[1U] },                                           \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* SDE Enable */                                    \
        {                                                                      \
            "TC_SDE_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppSdeTestCfg[5U]                                            \
            },                                                                 \
            {                                                                  \
                gSdeConfScoreMap[0U]                                           \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* DOF Enable */                                    \
        {                                                                      \
            "TC_DOF_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppDofTestCfg[6U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* VISS Enable */                                   \
        {                                                                      \
            "TC_VISS_FUNC_001",                   /* Test Name */              \
            1,                          /* Num Handles */                      \
            1,                          /* Repeate Count */                    \
            UFALSE,                       /* Is Performance */                   \
            {&gAppVissTestConfig[0U]},     /* Test Config */                    \
            UTRUE,                                                              \
            UFALSE,                                                             \
            UFALSE                                                              \
        },                                                                     \
                                                                               \
    },                                                                          \
    {                                                                          \
        "TC_INT_TC3",      /* Test Name */                                     \
        UTRUE,              /* Enable */                                        \
        1u,                /* Num Handles */                                   \
        10u,                /* Repeat Cnt */                                    \
        UFALSE,              /* LDC Enable */                                    \
        {                                                                      \
            "TC_LDC_FUNC_001",      /* Test Name */                            \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppLdcTestCfg[0U]                                            \
            },                                                                 \
            UTRUE                                                               \
        },                                                                     \
        UTRUE,              /* NF Enable */                                     \
        {                                                                      \
            "TC_NF_FUNC_001",      /* Test Name */                             \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppNfTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* MSC0 Enable */                                   \
        {                                                                      \
            "TC_MSC0_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[0U] },                                            \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* MSC1 Enable */                                  \
        {                                                                      \
            "TC_MSC1_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[1U] },                                           \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* SDE Enable */                                    \
        {                                                                      \
            "TC_SDE_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppSdeTestCfg[5U]                                            \
            },                                                                 \
            {                                                                  \
                gSdeConfScoreMap[0U]                                           \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* DOF Enable */                                    \
        {                                                                      \
            "TC_DOF_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppDofTestCfg[6U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* VISS Enable */                                   \
        {                                                                      \
            "TC_VISS_FUNC_001",                   /* Test Name */              \
            1,                          /* Num Handles */                      \
            1,                          /* Repeate Count */                    \
            UFALSE,                       /* Is Performance */                   \
            {&gAppVissTestConfig[0U]},     /* Test Config */                    \
            UTRUE,                                                              \
            UFALSE,                                                             \
            UFALSE                                                              \
        },                                                                     \
                                                                               \
    },                                                                          \
    {                                                                          \
        "TC_INT_TC4",      /* Test Name */                                     \
        UTRUE,              /* Enable */                                        \
        1u,                /* Num Handles */                                   \
        10u,                /* Repeat Cnt */                                    \
        UFALSE,              /* LDC Enable */                                    \
        {                                                                      \
            "TC_LDC_FUNC_001",      /* Test Name */                            \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppLdcTestCfg[0U]                                            \
            },                                                                 \
            UTRUE                                                               \
        },                                                                     \
        UFALSE,              /* NF Enable */                                     \
        {                                                                      \
            "TC_NF_FUNC_001",      /* Test Name */                             \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppNfTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* MSC0 Enable */                                   \
        {                                                                      \
            "TC_MSC0_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[0U] },                                            \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* MSC1 Enable */                                  \
        {                                                                      \
            "TC_MSC1_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[1U] },                                           \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* SDE Enable */                                    \
        {                                                                      \
            "TC_SDE_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppSdeTestCfg[5U]                                            \
            },                                                                 \
            {                                                                  \
                gSdeConfScoreMap[0U]                                           \
            },                                                                 \
        },                                                                     \
        UTRUE,              /* DOF Enable */                                    \
        {                                                                      \
            "TC_DOF_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppDofTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* VISS Enable */                                   \
        {                                                                      \
            "TC_VISS_FUNC_001",                   /* Test Name */              \
            1,                          /* Num Handles */                      \
            1,                          /* Repeate Count */                    \
            UFALSE,                       /* Is Performance */                   \
            {&gAppVissTestConfig[0U]},     /* Test Config */                    \
            UTRUE,                                                              \
            UFALSE,                                                             \
            UFALSE                                                              \
        },                                                                     \
                                                                               \
    },                                                                          \
    {                                                                          \
        "TC_INT_TC5",      /* Test Name */                                     \
        UTRUE,              /* Enable */                                        \
        1u,                /* Num Handles */                                   \
        10u,                /* Repeat Cnt */                                    \
        UFALSE,              /* LDC Enable */                                    \
        {                                                                      \
            "TC_LDC_FUNC_001",      /* Test Name */                            \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppLdcTestCfg[0U]                                            \
            },                                                                 \
            UTRUE                                                               \
        },                                                                     \
        UFALSE,              /* NF Enable */                                     \
        {                                                                      \
            "TC_NF_FUNC_001",      /* Test Name */                             \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppNfTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* MSC0 Enable */                                   \
        {                                                                      \
            "TC_MSC0_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[0U] },                                            \
            UTRUE,                                                              \
        },                                                                     \
        UFALSE,              /* MSC1 Enable */                                  \
        {                                                                      \
            "TC_MSC1_FUNC_TC01",                                               \
            1u,                     /* NumHandles */                           \
            1u,                     /* Repeat Cnt */                           \
            UFALSE,                  /* Is Performance Test */                  \
            {&gAppMscTestCfg[1U] },                                           \
            UTRUE,                                                              \
        },                                                                     \
        UTRUE,              /* SDE Enable */                                    \
        {                                                                      \
            "TC_SDE_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppSdeTestCfg[5U]                                            \
            },                                                                 \
            {                                                                  \
                gSdeConfScoreMap[0U]                                           \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* DOF Enable */                                    \
        {                                                                      \
            "TC_DOF_FUNC_001",      /* Test Name */                            \
            UTRUE,                                                              \
            1u,                     /* Num Handles */                          \
            1u,                     /* Repeat Cnt */                           \
            UTRUE,                  /* Is Performance Test */                  \
            /* Test Configuration */                                           \
            {                                                                  \
                &gAppDofTestCfg[0U]                                            \
            },                                                                 \
        },                                                                     \
        UFALSE,              /* VISS Enable */                                   \
        {                                                                      \
            "TC_VISS_FUNC_001",                   /* Test Name */              \
            1,                          /* Num Handles */                      \
            1,                          /* Repeate Count */                    \
            UFALSE,                       /* Is Performance */                   \
            {&gAppVissTestConfig[0U]},     /* Test Config */                    \
            UTRUE,                                                              \
            UFALSE,                                                             \
            UFALSE                                                              \
        },                                                                     \
                                                                               \
    },                                                                          \
}


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

#endif /*VHWA_INT_TIRTOS_CFG_*/




;
