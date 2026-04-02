/*
 *   Copyright (c) Texas Instruments Incorporated 2021-2022
 *   All rights reserved.
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
 *
 */

/**
 *  \ingroup  DRV_FC_MODULE
 *  \defgroup DRV_FC_MODULE_CFG Graph Configuration
 *            This is Graph Configuration file
 *
 *  @{
 */
/**
 *  \file fc_soc_cfg.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *          configure / control Graph of VPAC
 */

#ifndef FC_SOC_CFG_H_
#define FC_SOC_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <vhwa_vpac_abstraction_layer.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define VHWA_FC_NODE_STATUS_FREE         (0u)
#define VHWA_FC_NODE_STATUS_M2M          (1u)

/**
 *  \anchor FC_VhwaNodes
 *  \name   VHWA Flex-Connect Nodes
 *  \brief  Macro's to define the VHWA Glex-Connect Nodes
 *
 *  @{
 */
#define VHWA_FC_NODE_NONE                (0u)
#define VHWA_FC_NODE_VISS                (1u)
#define VHWA_FC_NODE_MSC0                (2u)
#define VHWA_FC_NODE_MSC1                (3u)
#define VHWA_FC_NODE_LDC                 (4u)
#define VHWA_FC_NODE_NF                  (5u)
#define VHWA_FC_NODE_L3                  (6u)
#define VHWA_FC_NODE_DDR                 (7u)
#define VHWA_FC_MAX_NODES                (8u)
/** @} */

/**
 *  \anchor Fc_VhwaNodeInputOutputPorts
 *  \name   VHWA Flex-Connect Node Input/Output Ports
 *  \brief  Macro's to define the input ports for the nodes in Flex-Connect
 *
 *  @{
 */
/* Common port */
#define VHWA_FC_PORT_DDR                         (uint64_t)(0x0)

/* Input port */
#define VHWA_FC_PORT_MSC0_IN_0                   (uint32_t)(1u)
#define VHWA_FC_PORT_MSC0_IN_1                  (uint32_t)(2u)
#define VHWA_FC_PORT_MSC1_IN_0                   (uint32_t)(3u)
#define VHWA_FC_PORT_MSC1_IN_1                  (uint32_t)(4u)
#define VHWA_FC_PORT_VISS_IN_0                   (uint32_t)(5u)
#define VHWA_FC_PORT_VISS_IN_1                   (uint32_t)(6u)
#define VHWA_FC_PORT_VISS_IN_2                   (uint32_t)(7u)

/* Output port */
#define VHWA_FC_PORT_VISS_OUT_Y12                (uint32_t)(65u)
#define VHWA_FC_PORT_VISS_OUT_UV12               (uint32_t)(66u)
#define VHWA_FC_PORT_VISS_OUT_Y8                 (uint32_t)(67u)
#define VHWA_FC_PORT_VISS_OUT_UV8                (uint32_t)(68u)
#define VHWA_FC_PORT_VISS_OUT_S8                 (uint32_t)(69u)
#define VHWA_FC_PORT_MSC0_OUT_0                  (uint32_t)(70u)
#define VHWA_FC_PORT_MSC0_OUT_1                  (uint32_t)(71u)
#define VHWA_FC_PORT_MSC0_OUT_2                  (uint32_t)(72u)
#define VHWA_FC_PORT_MSC0_OUT_3                  (uint32_t)(73u)
#define VHWA_FC_PORT_MSC0_OUT_4                  (uint32_t)(74u)
#define VHWA_FC_PORT_MSC0_OUT_5                  (uint32_t)(75u)
#define VHWA_FC_PORT_MSC0_OUT_6                  (uint32_t)(76u)
#define VHWA_FC_PORT_MSC0_OUT_7                  (uint32_t)(77u)
#define VHWA_FC_PORT_MSC0_OUT_8                  (uint32_t)(78u)
#define VHWA_FC_PORT_MSC0_OUT_9                  (uint32_t)(79u)
#define VHWA_FC_PORT_MSC1_OUT_0                  (uint32_t)(80u)
#define VHWA_FC_PORT_MSC1_OUT_1                  (uint32_t)(81u)
#define VHWA_FC_PORT_MSC1_OUT_2                  (uint32_t)(82u)
#define VHWA_FC_PORT_MSC1_OUT_3                  (uint32_t)(83u)
#define VHWA_FC_PORT_MSC1_OUT_4                  (uint32_t)(84u)
#define VHWA_FC_PORT_MSC1_OUT_5                  (uint32_t)(85u)
#define VHWA_FC_PORT_MSC1_OUT_6                  (uint32_t)(86u)
#define VHWA_FC_PORT_MSC1_OUT_7                  (uint32_t)(87u)
#define VHWA_FC_PORT_MSC1_OUT_8                  (uint32_t)(88u)
#define VHWA_FC_PORT_MSC1_OUT_9                  (uint32_t)(89u)

/** @} */



/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /** FC_SOC_CFG_H_ */

/** @} */
