/*
 *
 * Copyright (C) 2020 Texas Instruments Incorporated - http://www.ti.com/
 *
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

/*
 * Check resource availability for this device and configure accordingly.
 *
 * On High Secure (HS) variants, some SA2UL resources are reserved for use by
 * DMSC firmware. Only subset of resources are available for access by the unit
 * testi framework, and all others must be masked out. Test which are not
 * disabled may require additional firewall configuration to access the
 * submodules of the SA2UL subsystem.
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef USE_BIOS
    #include <xdc/std.h>
#endif

/* CSL includes */
#include <ti/csl/cslr.h>
#include <ti/csl/soc.h>

#include <ti/drv/sciclient/sciclient.h>


#include <unittest.h>
#include <saLog.h>

uint32_t TF_SA2UL_PEER_RXCHAN0;
uint32_t TF_SA2UL_PEER_RXCHAN1;
uint32_t TF_SA2UL_PEER_TXCHAN;
uint32_t SA_UTEST_SA2_UL0_BASE;
#define  TF_SA2UL_TX_THR0              (CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_OFFSET + 0U)
#define  TF_SA2UL_TX_THR1              (CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_OFFSET + 1U)

#define  TF_SA2UL_RX_THR0              (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET + 0U)
#define  TF_SA2UL_RX_THR1              (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET + 1U)
#define  TF_SA2UL_RX_THR2              (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET + 2U)
#define  TF_SA2UL_RX_THR3              (CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET + 3U)



/*
 * Configure which SA2UL resources are used for this test. This checks device
 * type (GP or HS) and silicon revision to determine which tests are available
 * and which various SoC resources (e.g. PSIL threads) to use.
 */
uint8_t configSaRes(void)
{
    uint8_t isHsDevice = false;
    /* Use MAIN SA2UL on this device */
    SA_UTEST_SA2_UL0_BASE = (CSL_SA2_UL0_BASE);

    /* Device is GP */
    TF_SA2UL_PEER_RXCHAN0 = TF_SA2UL_RX_THR3;
    TF_SA2UL_PEER_RXCHAN1 = TF_SA2UL_RX_THR2;
    TF_SA2UL_PEER_TXCHAN  = TF_SA2UL_TX_THR1;

    /* HS device implies that SA2UL access is limited on this device */
    return isHsDevice;
}


