/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *  \file fvid2_graph.h
 *
 *  \brief FVID2 Graph interface file.
 */

#ifndef VHWA_GRAPH_PRIV_H_
#define VHWA_GRAPH_PRIV_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>

#include "include/vhwa_m2mFlexConnect.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
 *  \brief Invalid Node Id
 */
#define VHWA_GRAPH_INVALID_NODE_ID  ((uint32_t) 0x0U)

/**
 *  \brief Maximum number of paths
 */
#define VHWA_GRAPH_MAX_NUM_PATHS    ((uint32_t) 5U)

/**
 *  \brief Maximum number of paths
 */
#define VHWA_GRAPH_MAX_NUM_CONNECTION    ((uint32_t) 10U)

/**
 *  \anchor Vhwa_GraphNodeMode
 *  \name   Mode of the node
 *
 *  @{
 */
/** \brief  Disable mode */
#define VHWA_FC_NODE_MODE_DISABLE            ((uint32_t) 0x0U)
/** \brief Enable mode */
#define VHWA_FC_NODE_MODE_ENABLE             ((uint32_t) 0x1U)
/** \brief Check Mode */
#define VHWA_FC_NODE_MODE_CHECK              ((uint32_t) 0x2U)
/* @} */

/**
 *  \anchor Vhwa_GraphNodeType
 *  \name   Node Type
 *
 *  @{
 */
/** \brief Node type is dummy */
#define VHWA_FC_NODE_TYPE_DUMMY            ((uint32_t) 0x0U)
/** \brief Node type VHWA HA */
#define VHWA_FC_NODE_TYPE_VHWA_HA          ((uint32_t) 0x1U)
/** \brief Node type VHWA HA */
#define VHWA_FC_NODE_TYPE_VHWA_MEM         ((uint32_t) 0x2U)
/** \brief Maximum Node types */
#define VHWA_FC_NODE_TYPE_MAX              ((uint32_t) 0x3U)
/* @} */


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/** \brief Forward declaration for Node Information. */
typedef struct Vhwa_M2mFcGraphNodeInfo_t Vhwa_M2mFcGraphNodeInfo;

/**
 *  \brief Structure containing Node Set Configuration.
 */
typedef struct
{
    uint32_t numNodes;
    /**< Number of input nodes */
    Vhwa_M2mFcGraphNodeInfo *node[VHWA_GRAPH_MAX_NUM_PATHS];
    /**< Pointer to the input node */
    Vhwa_M2mFcEdgeInfo *edgeInfo[VHWA_GRAPH_MAX_NUM_PATHS][VHWA_GRAPH_MAX_NUM_CONNECTION];
    /**< Edge connection info for this node */
    uint32_t isEnabled[VHWA_GRAPH_MAX_NUM_PATHS][VHWA_GRAPH_MAX_NUM_CONNECTION];
    /**< Flag to indicate whether input/output is enabled or not. */
} Vhwa_GraphInputNodeSet;

typedef struct
{
    uint32_t numNodes;
    /**< Number of output nodes */
    Vhwa_M2mFcGraphNodeInfo *node[VHWA_GRAPH_MAX_NUM_PATHS];
    /**< Pointer to the input node */
    Vhwa_M2mFcEdgeInfo *edgeInfo[VHWA_GRAPH_MAX_NUM_PATHS][VHWA_GRAPH_MAX_NUM_CONNECTION];
    /**< Edge connection info for this node */
    uint32_t isEnabled[VHWA_GRAPH_MAX_NUM_PATHS][VHWA_GRAPH_MAX_NUM_CONNECTION];
    /**< Flag to indicate whether input/output is enabled or not. */
} Vhwa_GraphOutputNodeSet;

/**
 *  \brief Structure containing Node Information.
 */
struct Vhwa_M2mFcGraphNodeInfo_t
{
    uint32_t nodeId;
    /**< Node Id */
    uint32_t nodeType;
    /**< Node type for the particular node. Refer \ref Vhwa_GraphNodeType
     *   for values */
    uint32_t inUse;
    /**< Variable defining whether node is used in the present context */
    Vhwa_GraphInputNodeSet inputNodeSet;
    /**< Input Node Set */
    Vhwa_GraphOutputNodeSet outputNodeSet;
    /**< Output Node Set */
};

/**
 *  \brief Structure containing Node List
 */
typedef struct
{
    uint32_t numNodes;
    /**< Number of nodes */
    Vhwa_M2mFcGraphNodeInfo *list;
    /**< Pointer to node list */
} Vhwa_GraphNodeList;

/**
 *  \brief Structure containing Edge List.
 */
typedef struct
{
    uint32_t numEdges;
    /**< Number of the edge */
    Vhwa_M2mFcEdgeInfo *list;
    /**< Edge list */
} Vhwa_GraphEdgeList;

/**
 *  \brief Structure containing Graph information.
 */
typedef struct
{
    Vhwa_GraphNodeList *nodeList;
    /**< Node list of the graph */
    Vhwa_GraphEdgeList *edgeList;
    /**< Edge list of the graph */
} Vhwa_GraphInfo;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

#endif /* VHWA_GRAPH_PRIV_H_ */
