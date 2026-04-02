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

/* Following is not defined in CSL SoC */
#define CSL_WKUP_DMSC0_SECMGR_BASE  (0x44234000UL)

#include <unittest.h>
#include <saLog.h>

#if defined BUILD_MCU1_0
    #define HOST_ID     TISCI_HOST_ID_R5_0
    #define PRIV_ID     0x60U
#elif defined BUILD_MPU1_0
    #define HOST_ID     TISCI_HOST_ID_A53_2
    #define PRIV_ID     0x1U
#endif

uint32_t TF_SA2UL_PEER_RXCHAN0;
uint32_t TF_SA2UL_PEER_RXCHAN1;
uint32_t TF_SA2UL_PEER_TXCHAN;
uint32_t SA_UTEST_SA2_UL0_BASE;

/*
 * Configure which SA2UL resources are used for this test. This checks device
 * type (GP or HS) and silicon revision to determine which tests are available
 * and which various SoC resources (e.g. PSIL threads) to use.
 */
uint8_t configSaRes(void)
{
    struct tisci_msg_fwl_change_owner_info_req chown_req;
    struct tisci_msg_fwl_change_owner_info_resp chown_resp = {0};
    struct tisci_msg_fwl_set_firewall_region_req set_reg_req;
    struct tisci_msg_fwl_set_firewall_region_resp set_reg_resp = {0};
    uint32_t DEV_ID, SYS_STATUS;
    uint32_t timeout = 0xFFFFFFFFU;
    int32_t ret = CSL_EFAIL;
    uint8_t isHsDevice = false;

    SYS_STATUS = CSL_REG32_RD(CSL_WKUP_DMSC0_SECMGR_BASE + 0x100);
    DEV_ID = CSL_REG32_RD(CSL_WKUP_CTRL_MMR0_CFG0_BASE + CSL_WKUP_CTRL_MMR_CFG0_JTAGID);

    /* Use MAIN SA2UL on this device */
    SA_UTEST_SA2_UL0_BASE = (CSL_SA2_UL0_BASE);

    switch (SYS_STATUS & 0xFU)
    {
        /* Device is GP */
        case 0x3U:
        {
            TF_SA2UL_PEER_RXCHAN0 = (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET + 0);
            TF_SA2UL_PEER_RXCHAN1 = (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET + 1);
            TF_SA2UL_PEER_TXCHAN  = (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_OFFSET + 0);
            break;
        }

        /* Device is HS */
        case 0x9U:
        case 0xAU:
        default:
        {
            isHsDevice = true;
            /*
             * Use PSIL threads available (egress thread 0/1 and ingress thread
             * 0 are reserved).
             */
            TF_SA2UL_PEER_RXCHAN0 = (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET + 2);
            TF_SA2UL_PEER_RXCHAN1 = (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET + 3);
            TF_SA2UL_PEER_TXCHAN  = (CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_OFFSET + 1);

            /* PKA is not available in MCU SA2UL on HS devices; skip these tests */
            testCommonSetTestStatus(saPkaTest, SA_TEST_SKIP_TEST);
            testCommonSetTestStatus(saPkaTest2, SA_TEST_SKIP_TEST);

            /* Claim firewall ownership for TRNG */
            chown_req.fwl_id = 2112U;
            chown_req.region = 3U;
            chown_req.owner_index = HOST_ID;

            ret = Sciclient_firewallChangeOwnerInfo(&chown_req, &chown_resp, timeout);
            if (ret == CSL_PASS)
            {
                set_reg_req.fwl_id = 2112U;
                set_reg_req.region = 3U;
                set_reg_req.n_permission_regs = 1U;
                set_reg_req.control = 0xAU;
                set_reg_req.permissions[0] = (PRIV_ID << 16) | 0xFFFF;
                set_reg_req.start_address = 0x04E10000;
                set_reg_req.end_address = 0x04E10FFF;

                ret = Sciclient_firewallSetRegion(&set_reg_req, &set_reg_resp, timeout);
            }

            if (ret == CSL_EFAIL)
            {
                /* Could not access TRNG firewall control. Fail this test */
                SALog("Failed to configure TRNG firewall for test\n");
                testCommonSetTestStatus(saRngTest, SA_TEST_FAILED);
            }
            break;
        }
    }

    switch (DEV_ID)
    {
        /* SR 1.0 */
        case 0x0BB5A02F:
        {
            if (isHsDevice == true)
            {
                /*
                 * Unable to access any PSIL threads to SA2UL on this device
                 * revision.
                 */
                testCommonSetTestStatus(saDataModeTest, SA_TEST_SKIP_TEST);
            }
            break;
        }

        /* SR 2.0 */
        case 0x1BB5A02F:
        default:
        {
            break;
        }
    }

    /* HS device implies that SA2UL access is limited on this device */
    return isHsDevice;
}


