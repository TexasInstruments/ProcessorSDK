/*
 *
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/ 
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

#include "../unittest.h"
#include "../testconn.h"
#include "../salldsim/salldcfg.h"
#include <testKick2Secure.h>
#include <inttypes.h>

/* Data Mode Promote/Demote test.
 * This test is designed to check the Promote/Demote opertions of the IP 
 * in Data Mode by creating multiple Data Mode channels with different
 * configurations and performing packet verification tests.
 * The goal is to cover all the normal program flows in SA LLD and the SA sub-system.
 *
 * Test Procedures:
 * 
 * - Create multiple Data Mode connections including pairs of SA LLD Data Mode
 *   channels with configurations specified below
 * - Perform multi-packet, multi-connection data verification test
 *   - For each pkt loop
 *     - Generate payload with specific data pattern and variable length
 *     - For each pair of connections
 *       - Format the raw packet with the connection header and payload
 *       - Perform the following protocol-specific operations 
 *         - Invoke salld_sendData() API for Data Mode channel
 *       - Prepare and send packet bursts (10pkts) to SA for Encryption (and Authentication)
 *       - Receive encrypted packet from SA sub-system
 *       - Perform the following protocol-specific operations
 *          - Invoke salld_sendData() API for the corresponding Data Mode channel
 *       - Forward the packet to SA for Decryption (and Authentication) 
 *       - Preceive the decrypted packet from the SA sub-system
 *       - Perform payload verification              
 *   - End of the test loop
 * - Query all the SALLD channel statistics
 * - Close all the SALLD channels
 * 
 * a0 is a pointer to the test framework structure
 * a1 is a pointer to the saTest_t structure for this test, as initialized in testMain.
 * 
 */

extern void sauRelayDmPkt(int connIndex, CSL_UdmapCppi5HMPD* hd);
extern void sauBookKeepRx(void);

volatile uint32_t           gPktNum, gPromoteDemoteConnIndex, gPromoteDemotePktLen;
void sauConnPromoteDemoteDmPktTest(tFramework_t *tf, saTest_t *pat, uint16_t numPkts)
{
    CSL_UdmapCppi5HMPD *hd;
    uintptr_t psDataAddr;
    uint32_t  infoLen;
    uint32_t  authTag[8];
    uint32_t  pktId, flowId;
    uint32_t  i;

    for (gPromoteDemoteConnIndex = 0; gPromoteDemoteConnIndex < numConnHandles; gPromoteDemoteConnIndex +=2 )
    {
        gPromoteDemotePktLen = 256;
        for (gPktNum=0; gPktNum < numPkts; gPktNum++)
        {
            /* Generate Plain Text Packet in Secure Side and push it for
             * Sa2UL encrypt operations to make the packet reach non secure 
             * side 
             * This will be Demote operation for the packet
             * Note that from the SMC handler, the descriptor has the packet
             * to be using secure flow and secure ring
             */
            smc_oem_service_fid(SAU_SMC_FID_ENCRYPT_AND_DEMOTE, (uint64_t)gPromoteDemoteConnIndex, (uint64_t)gPromoteDemotePktLen);

            /* Process the Encrypt packet in non secure side */
            /* The packet should loop back into queue TF_RING_RXQ */    
            for (i = 0; i < 200; i++)  {
                utilCycleDelay (1000);
                if (CSL_ringaccGetRingOcc(&tFramework.ringAccCfg, TF_RING_RXQ ) > 0)
                    break;
            }
            
            if (i >= 200)  {
                salld_sim_halt();
            }

            /* This descriptor has a buffer owned by the framework, so the common code can be used for recycle */
            if (CSL_ringaccGetRingOcc(&tFramework.ringAccCfg, TF_RING_RXQ )) 
            {
                uint32_t size  = TF_RING_TRSIZE;
                hd = (CSL_UdmapCppi5HMPD *)((uintptr_t)RingPop(TF_RING_RXQ, &size));
                /* Extract PS Info if exists */
                // CSL_cppi5GetPSData (CSL_CPPI5_DESCRIPTOR_TYPE_HOST, (uintptr_t *)hd, (uint8_t **)&psInfo, &infoLen);
                psDataAddr = CSL_udmapCppi5GetPsDataAddr((void*) hd, 0, 1);
                infoLen    = CSL_udmapCppi5GetPsDataLen((void *) hd);

                /* Copy Auth tag if any */
                if (infoLen != 0) {
                  memcpy(authTag, (void *) psDataAddr, infoLen);
                }

                salld_sim_print("Conn %d: Receive Data Mode Pkt %d from SA\n", gPromoteDemoteConnIndex, gPktNum); 

                sauRelayDmPkt(gPromoteDemoteConnIndex+1, hd);

                /* Update the flow to relay the packet to send packet to secure 
                 * side as a promote packet */
                CSL_udmapCppi5GetIds((void*) hd, &pktId, &flowId);
                flowId = TF_RXCHAN_FLOWID_SECURE;
                CSL_udmapCppi5SetIds((void*) hd, SA2UL_TEST_CPPI5_DESCRIPTOR_TYPE_HOST, pktId, flowId);

                RingPush(TF_RING_TXQ,hd->bufInfo1, (physptr_t) hd);
            }

            /* The packet should loop back into queue TF_RING_RXQ_SECURE */    
            smc_oem_service_fid(SAU_SMC_FID_DECRYPT_VERIFY_PROMOTE, (uint64_t)gPromoteDemoteConnIndex, (uint64_t)gPromoteDemotePktLen);

            /* Book keep the Rx from non secure side after packet pushed for decryption */
            sauBookKeepRx();

        }
    }
}


/* SA Data Mode Basic Functional test */
void saDataModePromoteDemoteTest (void* a0, void* a1)
{
    tFramework_t  *tf  = (tFramework_t *)a0;
    saTest_t      *pat = (saTest_t *)a1;

    /* Initialize protocol addresses */
    numConnHandles = 2;

    /* Kick to Secure side for Create test channels for demote and promote*/
    smc_oem_service_fid(SAU_SMC_FID_SETUP_PROMOTE_DEMOTE_CHANNELS, (uint64_t) a1, (uint64_t) NULL);

    /* Start Packet Tests */
    sauConnPromoteDemoteDmPktTest(tf, pat,
                     numTestPkt1      /* Number of packets per connection */
                    );

    /* Kick secure side to close all channels */
    salldSim_get_all_chan_stats();

    saTestRecoverAndExit (tf, pat, SA_TEST_PASSED);  /* no return */
}

/* Nothing past this point */

