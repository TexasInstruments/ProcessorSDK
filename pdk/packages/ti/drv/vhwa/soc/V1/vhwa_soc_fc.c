/*
 *  Copyright (c) Texas Instruments Incorporated 2021
 *  All rights reserved.
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
 *  \file vhwa_soc_fc.c
 *
 *  \brief File containing the graph related configuration functions for Flex-Connect.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <src/drv/vhwa_m2mFcDrvPriv.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static const Vhwa_M2mFcGraphNodeInfo gFcNodeInfoDefaults[VHWA_FC_MAX_NODE_INST] =
{
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    },
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    },
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    },
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    },
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    },
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    },
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    },
    {
        VHWA_FC_NODE_NONE,
        VHWA_FC_NODE_TYPE_DUMMY,
        0U,
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        },
        {
            0U,
            {
                NULL, NULL, NULL, NULL, NULL
            },
            {
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
                {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
            },
            {0}
        }
    }
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

const Vhwa_M2mFcGraphNodeInfo* Vhwa_getDefaultNodeInfo(uint32_t *nodeMemSize)
{
    *nodeMemSize = sizeof (gFcNodeInfoDefaults);
    return &gFcNodeInfoDefaults[0U];
}
