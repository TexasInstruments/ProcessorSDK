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
*  \file vhwa_nf_tirtos_cfg.h
*
*  \brief Configuration for TI RTOS
*/
#ifndef VHWA_NF_TIRTOS_CFG_
#define VHWA_NF_TIRTOS_CFG_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define VHWA_NF_TIRTOS_CFG                                                     \
{                                                                              \
    {                                                                          \
        "TC_NF_0",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[0U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_1",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[1U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_2",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[2U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_3",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[3U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_4",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        10u,                   /* Repeat Cnt */                               \
        UTRUE,                   /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[4U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_5",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[5U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_6",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[6U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_7",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[7U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_8",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[8U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_9",      /* Test Name */                                        \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[9U]                                                 \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_10",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[10U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_11",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[11U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_12",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[12U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_13",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[13U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_14",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[14U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_15",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[15U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_16",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[16U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_17",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[17U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_18",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[18U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_19",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[19U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_20",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[20U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_NF_21",      /* Test Name */                                       \
        UTRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        UFALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppNfTestCfg[21U]                                                \
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

#endif /*VHWA_NF_TIRTOS_CFG_*/




;
