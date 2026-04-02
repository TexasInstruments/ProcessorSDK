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
*  \file vhwa_sde_tirtos_cfg.h
*
*  \brief Configuration for TI RTOS
*/
#ifndef VHWA_SDE_TIRTOS_CFG_
#define VHWA_SDE_TIRTOS_CFG_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define VHWA_SDE_TIRTOS_CFG                                                    \
{                                                                              \
    {                                                                          \
        "TC_SDE_TC0",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        100u,                     /* Repeat Cnt */                               \
        UTRUE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[0U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC1",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        100u,                     /* Repeat Cnt */                               \
        UTRUE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[1U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC2",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[2U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC3",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[3U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC4",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[4U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC5",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        100u,                     /* Repeat Cnt */                               \
        UTRUE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[5U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC6",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[6U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC7",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[7U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC8",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[8U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC9",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[9U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC10",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[10U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC11",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[11U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC12",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[12U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC13",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[13U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC14",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[14U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC15",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[15U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC16",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                             \
        UFALSE,                  /* Is Performance Test */                       \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[16U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC17",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                             \
        UFALSE,                  /* Is Performance Test */                       \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[9U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC18",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                   /* Repeat Cnt */                                 \
        UFALSE,                   /* Is Performance Test */                     \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[17U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                          \
    {                                                                          \
        "TC_SDE_TC19",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[18U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC20",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[19U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC21",      /* Test Name */                                     \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[20U]                                                \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC22",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        100u,                     /* Repeat Cnt */                               \
        UTRUE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[21U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                          \
    {                                                                          \
        "TC_SDE_TC23",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                   /* Repeat Cnt */                                 \
        UFALSE,                   /* Is Performance Test */                     \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[22U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC24",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                   /* Repeat Cnt */                                 \
        UFALSE,                   /* Is Performance Test */                     \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[23U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_SDE_TC25",      /* Test Name */                                    \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                   /* Repeat Cnt */                                 \
        UFALSE,                   /* Is Performance Test */                     \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppSdeTestCfg[24U]                                               \
        },                                                                     \
        {                                                                      \
            gConfScoreMap[0U]                                                  \
        },                                                                     \
    },                                                                         \
}


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None*/

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

#endif /*VHWA_SDE_TIRTOS_CFG_*/




;
