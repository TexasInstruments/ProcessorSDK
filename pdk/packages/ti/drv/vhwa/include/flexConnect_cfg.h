/*
 *   Copyright (c) Texas Instruments Incorporated 2021
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
 *  \ingroup  DRV_FlexConnect_MODULE
 *  \defgroup DRV_FC_MODULE_CFG Graph Configuration
 *            This is Graph Configuration file
 *
 *  @{
 */
/**
 *  \file flexConnect_cfg.h
 *
 *  \brief  Defines the structures / control operations that could be used to
 *          configure / control Graph of VPAC
 */

#ifndef FC_CFG_H_
#define FC_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>
#include <soc/vhwa_fc_soc.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 * \brief Prototype for the Error Event for Flex-connect.
 *        The callback for the Flex-connect error events can be registered
 *        using #Fc_ErrEventParams. One of the parameter in this
 *        is call back. Driver calls this callback when error occurs.
 *
 * \param handle                FVID2 driver handle, for which error
 *                              has occurred.
 * \param errEvents             Error Events occured
 *
 * \return None.
 */
typedef void (*fc_ErrEventCbFxn)(Fvid2_Handle handle, uint32_t errEvents,
    void *appData);

/**
 *  struct Fc_ErrEventParams
 *  \brief Structure for error event parameters
 *         Used to register callback for the given set of events.
 */
typedef struct
{
    uint32_t                    errEvents[VHWA_FC_MAX_NODES];
    /**< bitmask of error events,
     *   Multiple error events can be registered with the same callback,
     *   Driver returns bitmask of events, which has occured valid only for
     *   MSC/VISS/LDC/NF nodes */
    fc_ErrEventCbFxn         cbFxn;
    /**< Callback function to be call on error events */
    void                        *appData;
    /**< private data */
} Fc_ErrEventParams;

#ifdef __cplusplus
}
#endif

#endif
/** @} */
