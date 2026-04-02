/*
TEXAS INSTRUMENTS TEXT FILE LICENSE

Copyright (c) [2018] – [2019] Texas Instruments Incorporated

All rights reserved not granted herein.

Limited License.

Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive license under copyrights and patents it now or hereafter owns or controls to make, have made, use, import, offer to sell and sell ("Utilize") this software subject to the terms herein.  With respect to the foregoing patent license, such license is granted  solely to the extent that any such patent is necessary to Utilize the software alone.  The patent license shall not apply to any combinations which include this software, other than combinations with devices manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.

Redistributions must preserve existing copyright notices and reproduce this license (including the above copyright notice and the disclaimer and (if applicable) source code license limitations below) in the documentation and/or other materials provided with the distribution

Redistribution and use in binary form, without modification, are permitted provided that the following conditions are met:

*    No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any software provided in binary form.

*    any redistribution and use are licensed by TI for use only with TI Devices.

*    Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.

If software source code is provided to you, modification and redistribution of the source code are permitted provided that the following conditions are met:

*    any redistribution and use of the source code, including any resulting derivative works, are licensed by TI for use only with TI Devices.

*    any redistribution and use of any object code compiled from the source code and any resulting derivative works, are licensed by TI for use only with TI Devices.

Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or promote products derived from this software without specific prior written permission.

DISCLAIMER.

THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
* @file parse_data.c
*
*
* @version 1.00
* @author Mayank Mangla (Texas Instruments)
*
* @date 30.10.2018
*
* @version 2.00
*/

/* =============================================================================
*                                  INCLUDE FILES
* =========================================================================== */
#include <stdio.h>
#include <idcc.h>
#include "dcc_defs.h"

uint32_t calc_dcc_outbuf_size()
{
    uint32_t size = 0U;
    size += DCC_RGB2RGB1_MAX_PHOTO_SPACE_INST * sizeof(iss_ipipe_rgb2rgb);

    return size;
}

int32_t Dcc_Create(dcc_parser_output_params_t * p_output_params, uint8_t * out_dcc_buf)
{
/*
    When called from 2A node, out_dcc_buf is not needed and maybe NULL

    When called from VISS node, out_dcc_buf is needed to get the VISS parameters
    based on the photospace selected
*/
    p_output_params->useAwbCalbCfg = 0;
    p_output_params->useH3aCfg = 0;
    p_output_params->useNsf4Cfg = 0;
    p_output_params->useBlcCfg = 0;
    p_output_params->useCfaCfg = 0;
    p_output_params->useCcmCfg = 0;
    p_output_params->useH3aMuxCfg = 0;
    p_output_params->useRfeDcmpCfg = 0;
    p_output_params->useVpacLdcCfg = 0;
    p_output_params->useVissGlbceCfg = 0;
    p_output_params->useVissLscCfg = 0;
    p_output_params->useVissYeeCfg = 0;
    p_output_params->useVissDpcCfg = 0;
    p_output_params->useVissRawfeWb1VsCfg = 0;
    p_output_params->useVissGammaCfg = 0;
#if defined(VPAC3)
    p_output_params->useVissCacCfg = 0;
    p_output_params->useVissRawhistCfg = 0;
    p_output_params->useVissCfai3aCfg = 0;
    p_output_params->useVissCfai3bCfg = 0;
    p_output_params->useVissCcMvCfg = 0;
#endif
#if defined(VPAC3L)
    p_output_params->useVissCacCfg = 0;
    p_output_params->useVissRawhistCfg = 0;
    p_output_params->useVissCfai3aCfg = 0;
    p_output_params->useVissLscExtCfg = 0;
    p_output_params->useVissDpcExtCfg = 0;
    p_output_params->useVissPcidCfg = 0;
#endif
    return 0;
}

/* LDRA_JUSTIFY
<metric start> statement branch <metric end>
<function start> int32_t Dcc_Delete.* <function end>
<justification start> 
Rationale: The test framework and test apps cannot reach this portion.
The function is unused because of an un-implemented feature, this code base is not being used/enabled with the current kernel support.
Effect on this unit: None; Unused feature, cannot be enabled, control cannot reach to this condition.
<justification end> */
int32_t Dcc_Delete(dcc_parser_output_params_t * p_output_params)
{

    return 0;
}

