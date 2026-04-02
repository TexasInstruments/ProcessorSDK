/*
 *  Copyright (C) 2020 Texas Instruments Incorporated.
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
#ifndef CSL_PKTDMA_ALIAS_UDMAP_API_H_
#define CSL_PKTDMA_ALIAS_UDMAP_API_H_

/*-----------------------------------------------------------------------------
 *  The following #define's are used to map pktdma CSL enumerations, structure
 *  definitions, and API functions to their udmap equivalents to make porting
 *  of existing code much easier.
 *
 *  Note that each user C file should only include
 *  csl_bcdma_alias_udmap_api.h or csl_pktdma_alias_udmap_api.h (never both)
 *  in order to avoid duplicate symbol definitions.
 *---------------------------------------------------------------------------*/
/* Enumerations */
#define CSL_UdmapChanDir                                            CSL_PktdmaChanDir
#define CSL_UDMAP_CHAN_DIR_TX                                       CSL_PKTDMA_CHAN_DIR_TX
#define CSL_UDMAP_CHAN_DIR_RX                                       CSL_PKTDMA_CHAN_DIR_RX
#define CSL_UdmapChanBurstSize                                      CSL_PktdmaChanBurstSize
#define CSL_UDMAP_CHAN_BURST_SIZE_32_BYTES                          CSL_PKTDMA_CHAN_BURST_SIZE_32_BYTES
#define CSL_UDMAP_CHAN_BURST_SIZE_64_BYTES                          CSL_PKTDMA_CHAN_BURST_SIZE_64_BYTES
#define CSL_UDMAP_CHAN_BURST_SIZE_128_BYTES                         CSL_PKTDMA_CHAN_BURST_SIZE_128_BYTES
#define CSL_UDMAP_CHAN_BURST_SIZE_256_BYTES                         CSL_PKTDMA_CHAN_BURST_SIZE_256_BYTES
#define CSL_UdmapDescType                                           CSL_PktdmaDescType
#define CSL_UDMAP_DESC_TYPE_HOST                                    CSL_PKTDMA_DESC_TYPE_HOST
#define CSL_UDMAP_DESC_TYPE_HOST_SB                                 CSL_PKTDMA_DESC_TYPE_HOST_SB
#define CSL_UDMAP_DESC_TYPE_MONOLITHIC                              CSL_PKTDMA_DESC_TYPE_MONOLITHIC
#define CSL_UDMAP_DESC_TYPE_RESERVED                                CSL_PKTDMA_DESC_TYPE_RESERVED
#define CSL_UdmapPsLoc                                              CSL_PktdmaPsLoc
#define CSL_UDMAP_PS_LOC_DESC                                       CSL_PKTDMA_PS_LOC_DESC
#define CSL_UDMAP_PS_LOC_PACKET                                     CSL_PKTDMA_PS_LOC_PACKET
#define CSL_UdmapAddrType                                           CSL_PktdmaAddrType
#define CSL_UDMAP_ADDR_TYPE_PHYS                                    CSL_PKTDMA_ADDR_TYPE_PHYS
#define CSL_UDMAP_ADDR_TYPE_INTER                                   CSL_PKTDMA_ADDR_TYPE_INTER
#define CSL_UDMAP_ADDR_TYPE_VIRT                                    CSL_PKTDMA_ADDR_TYPE_VIRT
#define CSL_UdmapChanType                                           CSL_PktdmaChanType
#define CSL_UDMAP_CHAN_TYPE_REF_PKT_RING                            CSL_PKTDMA_CHAN_TYPE_REF_PKT_RING
#define CSL_UDMAP_CHAN_TYPE_REF_PKTSB_QUEUE                         CSL_PKTDMA_CHAN_TYPE_REF_PKTSB_QUEUE
#define CSL_UdmapTagSelect                                          CSL_PktdmaTagSelect
#define CSL_UDMAP_TAG_SELECT_NO_OVERWRITE                           CSL_PKTDMA_TAG_SELECT_NO_OVERWRITE
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_VAL                     CSL_PKTDMA_TAG_SELECT_OVERWRITE_WITH_VAL
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_FLOWID_7_0              CSL_PKTDMA_TAG_SELECT_OVERWRITE_WITH_FLOWID_7_0
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_FLOWID_15_8             CSL_PKTDMA_TAG_SELECT_OVERWRITE_WITH_FLOWID_15_8
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_TAG_7_0                 CSL_PKTDMA_TAG_SELECT_OVERWRITE_WITH_TAG_7_0
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_TAG_15_8                CSL_PKTDMA_TAG_SELECT_OVERWRITE_WITH_TAG_15_8
#define CSL_UDMAP_TAG_SELECT_INVALID                                CSL_PKTDMA_TAG_SELECT_INVALID
#define CSL_UdmapChanSchedPri                                       CSL_PktdmaChanSchedPri
#define CSL_UDMAP_CHAN_SCHED_PRI_HIGH                               CSL_PKTDMA_CHAN_SCHED_PRI_HIGH
#define CSL_UDMAP_CHAN_SCHED_PRI_MED_HIGH                           CSL_PKTDMA_CHAN_SCHED_PRI_MED_HIGH
#define CSL_UDMAP_CHAN_SCHED_PRI_MED_LOW                            CSL_PKTDMA_CHAN_SCHED_PRI_MED_LOW
#define CSL_UDMAP_CHAN_SCHED_PRI_LOW                                CSL_PKTDMA_CHAN_SCHED_PRI_LOW
#define CSL_UDMAP_TXCRT_CHAN_CTL_EN_MASK                            CSL_PKTDMA_TXCRT_CHAN_CTL_EN_MASK
#define CSL_UDMAP_TXCRT_CHAN_CTL_EN_SHIFT                           CSL_PKTDMA_TXCRT_CHAN_CTL_EN_SHIFT
#define CSL_UDMAP_RXCRT_CHAN_CTL_EN_MASK                            CSL_PKTDMA_RXCRT_CHAN_CTL_EN_MASK
#define CSL_UDMAP_RXCRT_CHAN_CTL_EN_SHIFT                           CSL_PKTDMA_RXCRT_CHAN_CTL_EN_SHIFT
#define CSL_UDMAP_TXCRT_CHAN_CTL_TDOWN_MASK                         CSL_PKTDMA_TXCRT_CHAN_CTL_TDOWN_MASK
#define CSL_UDMAP_TXCRT_CHAN_CTL_TDOWN_SHIFT                        CSL_PKTDMA_TXCRT_CHAN_CTL_TDOWN_SHIFT
/* CSL-RL Structures */
#define CSL_udmap_gcfgRegs                                          CSL_pktdma_gcfgRegs
#define CSL_udmap_rxfcfgRegs                                        CSL_pktdma_rxfcfgRegs
#define CSL_udmap_txccfgRegs                                        CSL_pktdma_txccfgRegs
#define CSL_udmap_rxccfgRegs                                        CSL_pktdma_rxccfgRegs
#define CSL_udmap_txcrtRegs                                         CSL_pktdma_txcrtRegs
#define CSL_udmap_rxcrtRegs                                         CSL_pktdma_rxcrtRegs
/* CSL-FL Structures */
#define CSL_UdmapRouteTag                                           CSL_PktdmaRouteTag
#define CSL_UdmapRevision                                           CSL_PktdmaRevision
#define CSL_UdmapRing                                               CSL_PktdmaRing
#define CSL_UdmapRxFlowCfg                                          CSL_PktdmaRxFlowCfg
#define CSL_UdmapTxChanCfg                                          CSL_PktdmaTxChanCfg
#define CSL_UdmapRxChanCfg                                          CSL_PktdmaRxChanCfg
#define CSL_UdmapRT                                                 CSL_PktdmaRT
#define CSL_UdmapCfg                                                CSL_PktdmaCfg
#define CSL_UdmaRxFlowIdFirewallStatus                              CSL_PktdmaRxFlowIdFirewallStatus
#define CSL_UdmapChanStats                                          CSL_PktdmaChanStats
/* API functions */
#define CSL_udmapGetRevision                                        CSL_pktdmaGetRevision
#define CSL_udmapGetRevisionInfo                                    CSL_pktdmaGetRevisionInfo
#define CSL_udmapGetCfg                                             CSL_pktdmaGetCfg
#define CSL_udmapInitTxChanCfg                                      CSL_pktdmaInitTxChanCfg
#define CSL_udmapInitRxChanCfg                                      CSL_pktdmaInitRxChanCfg
#define CSL_udmapInitRxFlowCfg                                      CSL_pktdmaInitRxFlowCfg
#define CSL_udmapSetPerfCtrl                                        CSL_pktdmaSetPerfCtrl
#define CSL_udmapSetUtcCtrl                                         CSL_pktdmaSetUtcCtrl
#define CSL_udmapRxFlowCfg                                          CSL_pktdmaRxFlowCfg
#define CSL_udmapRxChanCfg                                          CSL_pktdmaRxChanCfg
#define CSL_udmapTxChanCfg                                          CSL_pktdmaTxChanCfg
#define CSL_udmapRxChanSetTrEvent                                   CSL_pktdmaRxChanSetTrEvent
#define CSL_udmapTxChanSetTrEvent                                   CSL_pktdmaTxChanSetTrEvent
#define CSL_udmapRxChanSetBurstSize                                 CSL_pktdmaRxChanSetBurstSize
#define CSL_udmapTxChanSetBurstSize                                 CSL_pktdmaTxChanSetBurstSize
#define CSL_udmapGetRxRT                                            CSL_pktdmaGetRxRT
#define CSL_udmapGetTxRT                                            CSL_pktdmaGetTxRT
#define CSL_udmapSetRxRT                                            CSL_pktdmaSetRxRT
#define CSL_udmapSetTxRT                                            CSL_pktdmaSetTxRT
#define CSL_udmapEnableTxChan                                       CSL_pktdmaEnableTxChan
#define CSL_udmapDisableTxChan                                      CSL_pktdmaDisableTxChan
#define CSL_udmapTeardownTxChan                                     CSL_pktdmaTeardownTxChan
#define CSL_udmapPauseTxChan                                        CSL_pktdmaPauseTxChan
#define CSL_udmapUnpauseTxChan                                      CSL_pktdmaUnpauseTxChan
#define CSL_udmapTriggerTxChan                                      CSL_pktdmaTriggerTxChan
#define CSL_udmapClearTxChanError                                   CSL_pktdmaClearTxChanError
#define CSL_udmapEnableRxChan                                       CSL_pktdmaEnableRxChan
#define CSL_udmapDisableRxChan                                      CSL_pktdmaDisableRxChan
#define CSL_udmapTeardownRxChan                                     CSL_pktdmaTeardownRxChan
#define CSL_udmapPauseRxChan                                        CSL_pktdmaPauseRxChan
#define CSL_udmapUnpauseRxChan                                      CSL_pktdmaUnpauseRxChan
#define CSL_udmapTriggerRxChan                                      CSL_pktdmaTriggerRxChan
#define CSL_udmapClearRxChanError                                   CSL_pktdmaClearRxChanError
#define CSL_udmapCfgRxFlowIdFirewall                                CSL_pktdmaCfgRxFlowIdFirewall
#define CSL_udmapGetRxFlowIdFirewallStatus                          CSL_pktdmaGetRxFlowIdFirewallStatus
#define CSL_udmapGetChanStats                                       CSL_pktdmaGetChanStats
#define CSL_udmapDecChanStats                                       CSL_pktdmaDecChanStats
#define CSL_udmapGetChanPeerReg                                     CSL_pktdmaGetChanPeerReg
#define CSL_udmapSetChanPeerReg                                     CSL_pktdmaSetChanPeerReg
#define CSL_udmapEnableLink                                         CSL_pktdmaEnableLink
/* cppi5 related structures, macros, and functions */
#define CSL_UdmapCppi5Epi                                           CSL_PktdmaCppi5Epi
#define CSL_UdmapCppi5HMPD                                          CSL_PktdmaCppi5HMPD
#define CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_SHIFT                     CSL_PKTDMA_CPPI5_PD_DESCINFO_DTYPE_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_MASK                      CSL_PKTDMA_CPPI5_PD_DESCINFO_DTYPE_MASK
#define CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST                  CSL_PKTDMA_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST
#define CSL_UDMAP_CPPI5_PD_DESCINFO_EINFO_SHIFT                     CSL_PKTDMA_CPPI5_PD_DESCINFO_EINFO_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_EINFO_MASK                      CSL_PKTDMA_CPPI5_PD_DESCINFO_EINFO_MASK
#define CSL_UDMAP_CPPI5_PD_DESCINFO_EINFO_VAL_NOT_PRESENT           CSL_PKTDMA_CPPI5_PD_DESCINFO_EINFO_VAL_NOT_PRESENT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_EINFO_VAL_IS_PRESENT            CSL_PKTDMA_CPPI5_PD_DESCINFO_EINFO_VAL_IS_PRESENT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_PSWCNT_SHIFT                    CSL_PKTDMA_CPPI5_PD_DESCINFO_PSWCNT_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_PSWCNT_MASK                     CSL_PKTDMA_CPPI5_PD_DESCINFO_PSWCNT_MASK
#define CSL_UDMAP_CPPI5_PD_DESCINFO_PKTLEN_SHIFT                    CSL_PKTDMA_CPPI5_PD_DESCINFO_PKTLEN_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_PKTLEN_MASK                     CSL_PKTDMA_CPPI5_PD_DESCINFO_PKTLEN_MASK
#define CSL_UDMAP_CPPI5_PD_PKTINFO1_PKTERROR_SHIFT                  CSL_PKTDMA_CPPI5_PD_PKTINFO1_PKTERROR_SHIFT
#define CSL_UDMAP_CPPI5_PD_PKTINFO1_PKTERROR_MASK                   CSL_PKTDMA_CPPI5_PD_PKTINFO1_PKTERROR_MASK
#define CSL_UDMAP_CPPI5_PD_PKTINFO1_PSFLGS_SHIFT                    CSL_PKTDMA_CPPI5_PD_PKTINFO1_PSFLGS_SHIFT
#define CSL_UDMAP_CPPI5_PD_PKTINFO1_PSFLGS_MASK                     CSL_PKTDMA_CPPI5_PD_PKTINFO1_PSFLGS_MASK
#define CSL_UDMAP_CPPI5_PD_PKTINFO1_FLOWID_SHIFT                    CSL_PKTDMA_CPPI5_PD_PKTINFO1_FLOWID_SHIFT
#define CSL_UDMAP_CPPI5_PD_PKTINFO1_FLOWID_MASK                     CSL_PKTDMA_CPPI5_PD_PKTINFO1_FLOWID_MASK
#define CSL_UDMAP_CPPI5_PD_PKTINFO2_PKTTYPE_SHIFT                   CSL_PKTDMA_CPPI5_PD_PKTINFO2_PKTTYPE_SHIFT
#define CSL_UDMAP_CPPI5_PD_PKTINFO2_PKTTYPE_MASK                    CSL_PKTDMA_CPPI5_PD_PKTINFO2_PKTTYPE_MASK
#define CSL_UDMAP_CPPI5_PD_PKTINFO2_RETQ_SHIFT                      CSL_PKTDMA_CPPI5_PD_PKTINFO2_RETQ_SHIFT
#define CSL_UDMAP_CPPI5_PD_PKTINFO2_RETQ_MASK                       CSL_PKTDMA_CPPI5_PD_PKTINFO2_RETQ_MASK
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_SRCTAG_SHIFT                   CSL_PKTDMA_CPPI5_PD_SRCDSTTAG_SRCTAG_SHIFT
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_SRCTAG_MASK                    CSL_PKTDMA_CPPI5_PD_SRCDSTTAG_SRCTAG_MASK
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_DSTTAG_SHIFT                   CSL_PKTDMA_CPPI5_PD_SRCDSTTAG_DSTTAG_SHIFT
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_DSTTAG_MASK                    CSL_PKTDMA_CPPI5_PD_SRCDSTTAG_DSTTAG_MASK
#define CSL_UDMAP_CPPI5_PD_NEXTDESCPTR_ASPACE_SHIFT                 CSL_PKTDMA_CPPI5_PD_NEXTDESCPTR_ASPACE_SHIFT
#define CSL_UDMAP_CPPI5_PD_NEXTDESCPTR_ASPACE_MASK                  CSL_PKTDMA_CPPI5_PD_NEXTDESCPTR_ASPACE_MASK
#define CSL_UDMAP_CPPI5_PD_NEXTDESCPTR_ADDR_SHIFT                   CSL_PKTDMA_CPPI5_PD_NEXTDESCPTR_ADDR_SHIFT
#define CSL_UDMAP_CPPI5_PD_NEXTDESCPTR_ADDR_MASK                    CSL_PKTDMA_CPPI5_PD_NEXTDESCPTR_ADDR_MASK
#define CSL_UDMAP_CPPI5_PD_BUFPTR_ASPACE_SHIFT                      CSL_PKTDMA_CPPI5_PD_BUFPTR_ASPACE_SHIFT
#define CSL_UDMAP_CPPI5_PD_BUFPTR_ASPACE_MASK                       CSL_PKTDMA_CPPI5_PD_BUFPTR_ASPACE_MASK
#define CSL_UDMAP_CPPI5_PD_BUFPTR_ADDR_SHIFT                        CSL_PKTDMA_CPPI5_PD_BUFPTR_ADDR_SHIFT
#define CSL_UDMAP_CPPI5_PD_BUFPTR_ADDR_MASK                         CSL_PKTDMA_CPPI5_PD_BUFPTR_ADDR_MASK
#define CSL_UDMAP_CPPI5_PD_BUFINFO1_LEN_SHIFT                       CSL_PKTDMA_CPPI5_PD_BUFINFO1_LEN_SHIFT
#define CSL_UDMAP_CPPI5_PD_BUFINFO1_LEN_MASK                        CSL_PKTDMA_CPPI5_PD_BUFINFO1_LEN_MASK
#define CSL_UDMAP_CPPI5_PD_ORGBUFLEN_LEN_SHIFT                      CSL_PKTDMA_CPPI5_PD_ORGBUFLEN_LEN_SHIFT
#define CSL_UDMAP_CPPI5_PD_ORGBUFLEN_LEN_MASK                       CSL_PKTDMA_CPPI5_PD_ORGBUFLEN_LEN_MASK
#define CSL_UDMAP_CPPI5_PD_ORGBUFPTR_ASPACE_SHIFT                   CSL_PKTDMA_CPPI5_PD_ORGBUFPTR_ASPACE_SHIFT
#define CSL_UDMAP_CPPI5_PD_ORGBUFPTR_ASPACE_MASK                    CSL_PKTDMA_CPPI5_PD_ORGBUFPTR_ASPACE_MASK
#define CSL_UDMAP_CPPI5_PD_ORGBUFPTR_ADDR_SHIFT                     CSL_PKTDMA_CPPI5_PD_ORGBUFPTR_ADDR_SHIFT
#define CSL_UDMAP_CPPI5_PD_ORGBUFPTR_ADDR_MASK                      CSL_PKTDMA_CPPI5_PD_ORGBUFPTR_ADDR_MASK
#define CSL_udmapCppi5GetDescType                                   CSL_pktdmaCppi5GetDescType
#define CSL_udmapCppi5SetDescType                                   CSL_pktdmaCppi5SetDescType
#define CSL_udmapCppi5GetPktLen                                     CSL_pktdmaCppi5GetPktLen
#define CSL_udmapCppi5SetPktLen                                     CSL_pktdmaCppi5SetPktLen
#define CSL_udmapCppi5HostSetPktLen                                 CSL_pktdmaCppi5HostSetPktLen
#define CSL_udmapCppi5GetBufferAddr                                 CSL_pktdmaCppi5GetBufferAddr
#define CSL_udmapCppi5GetBufferLen                                  CSL_pktdmaCppi5GetBufferLen
#define CSL_udmapCppi5SetBufferAddr                                 CSL_pktdmaCppi5SetBufferAddr
#define CSL_udmapCppi5SetOrgBufferAddr                              CSL_pktdmaCppi5SetOrgBufferAddr
#define CSL_udmapCppi5SetBufferLen                                  CSL_pktdmaCppi5SetBufferLen
#define CSL_udmapCppi5SetOrgBufferLen                               CSL_pktdmaCppi5SetOrgBufferLen
#define CSL_udmapCppi5LinkDesc                                      CSL_pktdmaCppi5LinkDesc
#define CSL_udmapCppi5IsEpiDataPresent                              CSL_pktdmaCppi5IsEpiDataPresent
#define CSL_udmapCppi5SetEpiDataPresent                             CSL_pktdmaCppi5SetEpiDataPresent
#define CSL_udmapCppi5GetEpiDataPtr                                 CSL_pktdmaCppi5GetEpiDataPtr
#define CSL_udmapCppi5RdEpiData                                     CSL_pktdmaCppi5RdEpiData
#define CSL_udmapCppi5WrEpiData                                     CSL_pktdmaCppi5WrEpiData
#define CSL_udmapCppi5GetPsDataLoc                                  CSL_pktdmaCppi5GetPsDataLoc
#define CSL_udmapCppi5SetPsDataLoc                                  CSL_pktdmaCppi5SetPsDataLoc
#define CSL_udmapCppi5GetPsDataLen                                  CSL_pktdmaCppi5GetPsDataLen
#define CSL_udmapCppi5SetPsDataLen                                  CSL_pktdmaCppi5SetPsDataLen
#define CSL_udmapCppi5GetPsDataAddr                                 CSL_pktdmaCppi5GetPsDataAddr
#define CSL_udmapCppi5GetPsDataPtr                                  CSL_pktdmaCppi5GetPsDataPtr
#define CSL_udmapCppi5GetSrcTag                                     CSL_pktdmaCppi5GetSrcTag
#define CSL_udmapCppi5GetDstTag                                     CSL_pktdmaCppi5GetDstTag
#define CSL_udmapCppi5SetSrcTag                                     CSL_pktdmaCppi5SetSrcTag
#define CSL_udmapCppi5SetDstTag                                     CSL_pktdmaCppi5SetDstTag
#define CSL_udmapCppi5GetErrorFlags                                 CSL_pktdmaCppi5GetErrorFlags
#define CSL_udmapCppi5GetPsFlags                                    CSL_pktdmaCppi5GetPsFlags
#define CSL_udmapCppi5SetPsFlags                                    CSL_pktdmaCppi5SetPsFlags
#define CSL_udmapCppi5GetPktType                                    CSL_pktdmaCppi5GetPktType
#define CSL_udmapCppi5SetPktType                                    CSL_pktdmaCppi5SetPktType
#define CSL_udmapCppi5GetIds                                        CSL_pktdmaCppi5GetIds
#define CSL_udmapCppi5SetIds                                        CSL_pktdmaCppi5SetIds
#define CSL_udmapCppi5GetReturnPolicy                               CSL_pktdmaCppi5GetReturnPolicy
#define CSL_udmapCppi5SetReturnPolicy                               CSL_pktdmaCppi5SetReturnPolicy

#endif
