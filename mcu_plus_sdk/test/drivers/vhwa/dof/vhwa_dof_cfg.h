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
*  \file vhwa_dof_tirtos_cfg.h
*
*  \brief Configuration for TI RTOS
*/
#ifndef VHWA_DOF_TIRTOS_CFG_
#define VHWA_DOF_TIRTOS_CFG_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define VHWA_DOF_TIRTOS_CFG                                                    \
{                                                                              \
    {                                                                          \
        "TC_DOF_FUNC_2MP_12b_Packed",      /* Test Name */                     \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[0U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_1MP_12b_Packed",      /* Test Name */                     \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        10u,                   /* Repeat Cnt */                               \
        TRUE,                   /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[1U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_2MP_12bUp",      /* Test Name */                          \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[2U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_2MP_8b",      /* Test Name */                             \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[3U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_04",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[4U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_05",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[5U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_06",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[6U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_07",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[7U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_08",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[8U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_09",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        10u,                   /* Repeat Cnt */                               \
        TRUE,                   /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[9U]                                                \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_10",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[10U]                                               \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_11",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[11U]                                               \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_12",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[12U]                                               \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_13",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[13U]                                               \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_14",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[14U]                                               \
        },                                                                     \
    },                                                                         \
    {                                                                          \
        "TC_DOF_FUNC_15",      /* Test Name */                                 \
        TRUE,                                                                  \
        1u,                     /* Num Handles */                              \
        1u,                     /* Repeat Cnt */                               \
        FALSE,                  /* Is Performance Test */                      \
        /* Test Configuration */                                               \
        {                                                                      \
            &gAppDofTestCfg[15U]                                               \
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

#endif /*VHWA_DOF_TIRTOS_CFG_*/




;
