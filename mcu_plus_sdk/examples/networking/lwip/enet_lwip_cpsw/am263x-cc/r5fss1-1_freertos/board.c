/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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

#include <stdlib.h>
#include <drivers/hw_include/cslr_soc.h>
#include <stdbool.h>
#include "ti_drivers_config.h"
#include <kernel/dpl/AddrTranslateP.h>
#include <drivers/gpio.h>

#include "ti_board_config.h"
#include <networking/enet/core/include/phy/dp83869.h>

#include <drivers/pinmux.h>

static Pinmux_PerCfg_t MDIOPinMuxMainDomainCfg[] = {
    /* MDIO pin config */
    /* MDIO_MDIO -> MDIO_MDIO (N16) */
    {
        PIN_MDIO_MDIO,
        ( PIN_MODE(0) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* MDIO_MDC -> MDIO_MDC (M17) */
    {
        PIN_MDIO_MDC,
        ( PIN_MODE(0) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
	
	{PINMUX_END, PINMUX_END}
};

/*
 * Board info
 */
void Board_cpswMuxSel(void)
{
    /* MDIO0 pin config */
    Pinmux_config(MDIOPinMuxMainDomainCfg, PINMUX_DOMAIN_ID_MAIN);
    return;
}

/*
 * Tx and Rx Delay set
 */
void Board_TxRxDelaySet(const EnetBoard_PhyCfg *boardPhyCfg)
{
    return;
}

/*
 * Get ethernet board id
 */
uint32_t Board_getEthBoardId(void)
{
    return ENETBOARD_AM263X_EVM;
}

/*
 * Get ethernet type
 */
uint32_t Board_getEthType(void)
{
    return ENET_CPSW_3G;
}
