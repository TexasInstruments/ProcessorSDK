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
#ifndef CSL_BCDMA_ALIAS_UDMAP_API_H_
#define CSL_BCDMA_ALIAS_UDMAP_API_H_

/*-----------------------------------------------------------------------------
 *  The following #define's are used to map bcdma CSL enumerations, structure
 *  definitions, and API functions to their udmap equivalents to make porting
 *  of existing code much easier.
 *
 *  Note that each user C file should only include
 *  csl_bcdma_alias_udmap_api.h or csl_pktdma_alias_udmap_api.h (never both)
 *  in order to avoid duplicate symbol definitions.
 *---------------------------------------------------------------------------*/
/* Enumerations */
#define CSL_UdmapChanDir                                             CSL_BcdmaChanDir
#define CSL_UDMAP_CHAN_DIR_TX                                        CSL_BCDMA_CHAN_DIR_TX
#define CSL_UDMAP_CHAN_DIR_RX                                        CSL_BCDMA_CHAN_DIR_RX
#define CSL_UdmapChanBurstSize                                       CSL_BcdmaChanBurstSize
#define CSL_UDMAP_CHAN_BURST_SIZE_64_BYTES                           CSL_BCDMA_CHAN_BURST_SIZE_64_BYTES
#define CSL_UDMAP_CHAN_BURST_SIZE_128_BYTES                          CSL_BCDMA_CHAN_BURST_SIZE_128_BYTES
#define CSL_UDMAP_CHAN_BURST_SIZE_256_BYTES                          CSL_BCDMA_CHAN_BURST_SIZE_256_BYTES
#define CSL_UdmapDescType                                            CSL_BcdmaDescType
#define CSL_UDMAP_DESC_TYPE_HOST                                     CSL_BCDMA_DESC_TYPE_HOST
#define CSL_UDMAP_DESC_TYPE_HOST_SB                                  CSL_BCDMA_DESC_TYPE_HOST_SB
#define CSL_UDMAP_DESC_TYPE_MONOLITHIC                               CSL_BCDMA_DESC_TYPE_MONOLITHIC
#define CSL_UDMAP_DESC_TYPE_RESERVED                                 CSL_BCDMA_DESC_TYPE_RESERVED
#define CSL_UdmapPsLoc                                               CSL_BcdmaPsLoc
#define CSL_UDMAP_PS_LOC_DESC                                        CSL_BCDMA_PS_LOC_DESC
#define CSL_UDMAP_PS_LOC_PACKET                                      CSL_BCDMA_PS_LOC_PACKET
#define CSL_UdmapAddrType                                            CSL_BcdmaAddrType
#define CSL_UDMAP_ADDR_TYPE_PHYS                                     CSL_BCDMA_ADDR_TYPE_PHYS
#define CSL_UDMAP_ADDR_TYPE_INTER                                    CSL_BCDMA_ADDR_TYPE_INTER
#define CSL_UDMAP_ADDR_TYPE_VIRT                                     CSL_BCDMA_ADDR_TYPE_VIRT
#define CSL_UdmapChanType                                            CSL_BcdmaChanType
#define CSL_UDMAP_CHAN_TYPE_REF_PKT_RING                             CSL_BCDMA_CHAN_TYPE_REF_PKT_RING
#define CSL_UDMAP_CHAN_TYPE_REF_PKTSB_QUEUE                          CSL_BCDMA_CHAN_TYPE_REF_PKTSB_QUEUE
#define CSL_UDMAP_CHAN_TYPE_REF_TR_RING                              CSL_BCDMA_CHAN_TYPE_REF_TR_RING
#define CSL_UDMAP_CHAN_TYPE_VAL_TR_RING                              CSL_BCDMA_CHAN_TYPE_VAL_TR_RING
#define CSL_UDMAP_CHAN_TYPE_COPY_REF_TR_RING                         CSL_BCDMA_CHAN_TYPE_COPY_REF_TR_RING
#define CSL_UDMAP_CHAN_TYPE_COPY_VAL_TR_RING                         CSL_BCDMA_CHAN_TYPE_COPY_VAL_TR_RING
#define CSL_UdmapTagSelect                                           CSL_BcdmaTagSelect
#define CSL_UDMAP_TAG_SELECT_NO_OVERWRITE                            CSL_BCDMA_TAG_SELECT_NO_OVERWRITE
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_VAL                      CSL_BCDMA_TAG_SELECT_OVERWRITE_WITH_VAL
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_FLOWID_7_0               CSL_BCDMA_TAG_SELECT_OVERWRITE_WITH_FLOWID_7_0
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_FLOWID_15_8              CSL_BCDMA_TAG_SELECT_OVERWRITE_WITH_FLOWID_15_8
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_TAG_7_0                  CSL_BCDMA_TAG_SELECT_OVERWRITE_WITH_TAG_7_0
#define CSL_UDMAP_TAG_SELECT_OVERWRITE_WITH_TAG_15_8                 CSL_BCDMA_TAG_SELECT_OVERWRITE_WITH_TAG_15_8
#define CSL_UDMAP_TAG_SELECT_INVALID                                 CSL_BCDMA_TAG_SELECT_INVALID
#define CSL_UdmapChanSchedPri                                        CSL_BcdmaChanSchedPri
#define CSL_UDMAP_CHAN_SCHED_PRI_HIGH                                CSL_BCDMA_CHAN_SCHED_PRI_HIGH
#define CSL_UDMAP_CHAN_SCHED_PRI_MED_HIGH                            CSL_BCDMA_CHAN_SCHED_PRI_MED_HIGH
#define CSL_UDMAP_CHAN_SCHED_PRI_MED_LOW                             CSL_BCDMA_CHAN_SCHED_PRI_MED_LOW
#define CSL_UDMAP_CHAN_SCHED_PRI_LOW                                 CSL_BCDMA_CHAN_SCHED_PRI_LOW
/* CSL-RL Structures */                                              
#define CSL_udmap_gcfgRegs                                           CSL_bcdma_gcfgRegs  
#define CSL_udmap_txccfgRegs                                         CSL_bcdma_txccfgRegs
#define CSL_udmap_rxccfgRegs                                         CSL_bcdma_rxccfgRegs
#define CSL_udmap_txcrtRegs                                          CSL_bcdma_txcrtRegs 
#define CSL_udmap_rxcrtRegs                                          CSL_bcdma_rxcrtRegs
/* CSL-FL Structures */                                              
#define CSL_UdmapRxFdqThresh                                         CSL_BcdmaRxFdqThresh
#define CSL_UdmapRouteTag                                            CSL_BcdmaRouteTag
#define CSL_UdmapRevision                                            CSL_BcdmaRevision
#define CSL_UdmapRxFlowCfg                                           CSL_BcdmaRxFlowCfg
#define CSL_UdmapTxChanCfg                                           CSL_BcdmaTxChanCfg
#define CSL_UdmapRxChanCfg                                           CSL_BcdmaRxChanCfg
#define CSL_UdmapRT                                                  CSL_BcdmaRT
#define CSL_UdmapCfg                                                 CSL_BcdmaCfg
#define CSL_UdmapRxFlowIdFirewallStatus                              CSL_BcdmaRxFlowIdFirewallStatus
#define CSL_UdmapChanStats                                           CSL_BcdmaChanStats
/* API functions */                                                  
#define CSL_udmapGetRevision                                         CSL_bcdmaGetRevision
#define CSL_udmapGetRevisionInfo                                     CSL_bcdmaGetRevisionInfo
#define CSL_udmapGetCfg                                              CSL_bcdmaGetCfg
#define CSL_udmapInitTxChanCfg                                       CSL_bcdmaInitTxChanCfg
#define CSL_udmapInitRxChanCfg                                       CSL_bcdmaInitRxChanCfg
#define CSL_udmapInitRxFlowCfg                                       CSL_bcdmaInitRxFlowCfg
#define CSL_udmapSetPerfCtrl                                         CSL_bcdmaSetPerfCtrl
#define CSL_udmapSetUtcCtrl                                          CSL_bcdmaSetUtcCtrl
#define CSL_udmapRxFlowCfg                                           CSL_bcdmaRxFlowCfg
#define CSL_udmapRxChanCfg                                           CSL_bcdmaRxChanCfg
#define CSL_udmapTxChanCfg                                           CSL_bcdmaTxChanCfg
#define CSL_udmapRxChanSetTrEvent                                    CSL_bcdmaRxChanSetTrEvent
#define CSL_udmapTxChanSetTrEvent                                    CSL_bcdmaTxChanSetTrEvent
#define CSL_udmapRxChanSetBurstSize                                  CSL_bcdmaRxChanSetBurstSize
#define CSL_udmapTxChanSetBurstSize                                  CSL_bcdmaTxChanSetBurstSize
#define CSL_udmapGetRxRT                                             CSL_bcdmaGetRxRT
#define CSL_udmapGetTxRT                                             CSL_bcdmaGetTxRT
#define CSL_udmapSetRxRT                                             CSL_bcdmaSetRxRT
#define CSL_udmapSetTxRT                                             CSL_bcdmaSetTxRT
#define CSL_udmapEnableTxChan                                        CSL_bcdmaEnableTxChan
#define CSL_udmapDisableTxChan                                       CSL_bcdmaDisableTxChan
#define CSL_udmapTeardownTxChan                                      CSL_bcdmaTeardownTxChan
#define CSL_udmapPauseTxChan                                         CSL_bcdmaPauseTxChan
#define CSL_udmapUnpauseTxChan                                       CSL_bcdmaUnpauseTxChan
#define CSL_udmapTriggerTxChan                                       CSL_bcdmaTriggerTxChan
#define CSL_udmapClearTxChanError                                    CSL_bcdmaClearTxChanError
#define CSL_udmapEnableRxChan                                        CSL_bcdmaEnableRxChan
#define CSL_udmapDisableRxChan                                       CSL_bcdmaDisableRxChan
#define CSL_udmapTeardownRxChan                                      CSL_bcdmaTeardownRxChan
#define CSL_udmapPauseRxChan                                         CSL_bcdmaPauseRxChan
#define CSL_udmapUnpauseRxChan                                       CSL_bcdmaUnpauseRxChan
#define CSL_udmapTriggerRxChan                                       CSL_bcdmaTriggerRxChan
#define CSL_udmapClearRxChanError                                    CSL_bcdmaClearRxChanError
#define CSL_udmapCfgRxFlowIdFirewall                                 CSL_bcdmaCfgRxFlowIdFirewall
#define CSL_udmapGetRxFlowIdFirewallStatus                           CSL_bcdmaGetRxFlowIdFirewallStatus
#define CSL_udmapGetChanStats                                        CSL_bcdmaGetChanStats
#define CSL_udmapDecChanStats                                        CSL_bcdmaDecChanStats
#define CSL_udmapGetChanPeerReg                                      CSL_bcdmaGetChanPeerReg
#define CSL_udmapSetChanPeerReg                                      CSL_bcdmaSetChanPeerReg
#define CSL_udmapEnableLink                                          CSL_bcdmaEnableLink
/* TR-related structures and defines */
#define CSL_UdmapTR                                                  CSL_BcdmaTR
#define CSL_UdmapTR0                                                 CSL_BcdmaTR0
#define CSL_UdmapTR1                                                 CSL_BcdmaTR1
#define CSL_UdmapTR2                                                 CSL_BcdmaTR2
#define CSL_UdmapTR3                                                 CSL_BcdmaTR3
#define CSL_UdmapTR15                                                CSL_BcdmaTR15
#define CSL_UDMAP_TR_FLAGS_TYPE_SHIFT                                CSL_BCDMA_TR_FLAGS_TYPE_SHIFT
#define CSL_UDMAP_TR_FLAGS_TYPE_MASK                                 CSL_BCDMA_TR_FLAGS_TYPE_MASK
#define CSL_UDMAP_TR_FLAGS_STATIC_SHIFT                              CSL_BCDMA_TR_FLAGS_STATIC_SHIFT
#define CSL_UDMAP_TR_FLAGS_STATIC_MASK                               CSL_BCDMA_TR_FLAGS_STATIC_MASK
#define CSL_UDMAP_TR_FLAGS_WAIT_SHIFT                                CSL_BCDMA_TR_FLAGS_WAIT_SHIFT
#define CSL_UDMAP_TR_FLAGS_WAIT_MASK                                 CSL_BCDMA_TR_FLAGS_WAIT_MASK
#define CSL_UDMAP_TR_FLAGS_EVENT_SIZE_SHIFT                          CSL_BCDMA_TR_FLAGS_EVENT_SIZE_SHIFT
#define CSL_UDMAP_TR_FLAGS_EVENT_SIZE_MASK                           CSL_BCDMA_TR_FLAGS_EVENT_SIZE_MASK
#define CSL_UDMAP_TR_FLAGS_TRIGGER0_SHIFT                            CSL_BCDMA_TR_FLAGS_TRIGGER0_SHIFT
#define CSL_UDMAP_TR_FLAGS_TRIGGER0_MASK                             CSL_BCDMA_TR_FLAGS_TRIGGER0_MASK
#define CSL_UDMAP_TR_FLAGS_TRIGGER0_TYPE_SHIFT                       CSL_BCDMA_TR_FLAGS_TRIGGER0_TYPE_SHIFT
#define CSL_UDMAP_TR_FLAGS_TRIGGER0_TYPE_MASK                        CSL_BCDMA_TR_FLAGS_TRIGGER0_TYPE_MASK
#define CSL_UDMAP_TR_FLAGS_TRIGGER1_SHIFT                            CSL_BCDMA_TR_FLAGS_TRIGGER1_SHIFT
#define CSL_UDMAP_TR_FLAGS_TRIGGER1_MASK                             CSL_BCDMA_TR_FLAGS_TRIGGER1_MASK
#define CSL_UDMAP_TR_FLAGS_TRIGGER1_TYPE_SHIFT                       CSL_BCDMA_TR_FLAGS_TRIGGER1_TYPE_SHIFT
#define CSL_UDMAP_TR_FLAGS_TRIGGER1_TYPE_MASK                        CSL_BCDMA_TR_FLAGS_TRIGGER1_TYPE_MASK
#define CSL_UDMAP_TR_FLAGS_CMD_ID_SHIFT                              CSL_BCDMA_TR_FLAGS_CMD_ID_SHIFT
#define CSL_UDMAP_TR_FLAGS_CMD_ID_MASK                               CSL_BCDMA_TR_FLAGS_CMD_ID_MASK
#define CSL_UDMAP_TR_FLAGS_CFG_FLAGS_SHIFT                           CSL_BCDMA_TR_FLAGS_CFG_FLAGS_SHIFT
#define CSL_UDMAP_TR_FLAGS_CFG_FLAGS_MASK                            CSL_BCDMA_TR_FLAGS_CFG_FLAGS_MASK
#define CSL_UDMAP_TR_FLAGS_SA_INDIRECT_SHIFT                         CSL_BCDMA_TR_FLAGS_SA_INDIRECT_SHIFT
#define CSL_UDMAP_TR_FLAGS_SA_INDIRECT_MASK                          CSL_BCDMA_TR_FLAGS_SA_INDIRECT_MASK
#define CSL_UDMAP_TR_FLAGS_DA_INDIRECT_SHIFT                         CSL_BCDMA_TR_FLAGS_DA_INDIRECT_SHIFT
#define CSL_UDMAP_TR_FLAGS_DA_INDIRECT_MASK                          CSL_BCDMA_TR_FLAGS_DA_INDIRECT_MASK
#define CSL_UDMAP_TR_FLAGS_SUPR_EVT_SHIFT                            CSL_BCDMA_TR_FLAGS_SUPR_EVT_SHIFT
#define CSL_UDMAP_TR_FLAGS_SUPR_EVT_MASK                             CSL_BCDMA_TR_FLAGS_SUPR_EVT_MASK
#define CSL_UDMAP_TR_FLAGS_EOL_SHIFT                                 CSL_BCDMA_TR_FLAGS_EOL_SHIFT
#define CSL_UDMAP_TR_FLAGS_EOL_MASK                                  CSL_BCDMA_TR_FLAGS_EOL_MASK
#define CSL_UDMAP_TR_FLAGS_EOP_SHIFT                                 CSL_BCDMA_TR_FLAGS_EOP_SHIFT
#define CSL_UDMAP_TR_FLAGS_EOP_MASK                                  CSL_BCDMA_TR_FLAGS_EOP_MASK
#define CSL_UdmapTrFlagsType                                         CSL_BcdmaTrFlagsType
#define CSL_UDMAP_TR_FLAGS_TYPE_1D_DATA_MOVE                         CSL_BCDMA_TR_FLAGS_TYPE_1D_DATA_MOVE
#define CSL_UDMAP_TR_FLAGS_TYPE_2D_DATA_MOVE                         CSL_BCDMA_TR_FLAGS_TYPE_2D_DATA_MOVE
#define CSL_UDMAP_TR_FLAGS_TYPE_3D_DATA_MOVE                         CSL_BCDMA_TR_FLAGS_TYPE_3D_DATA_MOVE
#define CSL_UDMAP_TR_FLAGS_TYPE_4D_DATA_MOVE                         CSL_BCDMA_TR_FLAGS_TYPE_4D_DATA_MOVE
#define CSL_UDMAP_TR_FLAGS_TYPE_4D_DATA_MOVE_FORMATTING              CSL_BCDMA_TR_FLAGS_TYPE_4D_DATA_MOVE_FORMATTING
#define CSL_UDMAP_TR_FLAGS_TYPE_4D_CACHE_WARM                        CSL_BCDMA_TR_FLAGS_TYPE_4D_CACHE_WARM
#define CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE                        CSL_BCDMA_TR_FLAGS_TYPE_4D_BLOCK_MOVE
#define CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING              CSL_BCDMA_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING
#define CSL_UDMAP_TR_FLAGS_TYPE_2D_BLOCK_MOVE                        CSL_BCDMA_TR_FLAGS_TYPE_2D_BLOCK_MOVE
#define CSL_UDMAP_TR_FLAGS_TYPE_2D_BLOCK_MOVE_REPACKING              CSL_BCDMA_TR_FLAGS_TYPE_2D_BLOCK_MOVE_REPACKING
#define CSL_UDMAP_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING_INDIRECTION  CSL_BCDMA_TR_FLAGS_TYPE_4D_BLOCK_MOVE_REPACKING_INDIRECTION
#define CSL_UdmapTrFlagsEventSize                                    CSL_BcdmaTrFlagsEventSize
#define CSL_UDMAP_TR_FLAGS_EVENT_SIZE_COMPLETION                     CSL_BCDMA_TR_FLAGS_EVENT_SIZE_COMPLETION
#define CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT1_DEC                      CSL_BCDMA_TR_FLAGS_EVENT_SIZE_ICNT1_DEC
#define CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT2_DEC                      CSL_BCDMA_TR_FLAGS_EVENT_SIZE_ICNT2_DEC
#define CSL_UDMAP_TR_FLAGS_EVENT_SIZE_ICNT3_DEC                      CSL_BCDMA_TR_FLAGS_EVENT_SIZE_ICNT3_DEC
#define CSL_UdmapTrFlagsTrigger                                      CSL_BcdmaTrFlagsTrigger
#define CSL_UDMAP_TR_FLAGS_TRIGGER_NONE                              CSL_BCDMA_TR_FLAGS_TRIGGER_NONE
#define CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL0                           CSL_BCDMA_TR_FLAGS_TRIGGER_GLOBAL0
#define CSL_UDMAP_TR_FLAGS_TRIGGER_GLOBAL1                           CSL_BCDMA_TR_FLAGS_TRIGGER_GLOBAL1
#define CSL_UDMAP_TR_FLAGS_TRIGGER_LOCAL_EVENT                       CSL_BCDMA_TR_FLAGS_TRIGGER_LOCAL_EVENT
#define CSL_UdmapTrFlagsTriggerType                                  CSL_BcdmaTrFlagsTriggerType
#define CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC                    CSL_BCDMA_TR_FLAGS_TRIGGER_TYPE_ICNT1_DEC
#define CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC                    CSL_BCDMA_TR_FLAGS_TRIGGER_TYPE_ICNT2_DEC
#define CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC                    CSL_BCDMA_TR_FLAGS_TRIGGER_TYPE_ICNT3_DEC
#define CSL_UDMAP_TR_FLAGS_TRIGGER_TYPE_ALL                          CSL_BCDMA_TR_FLAGS_TRIGGER_TYPE_ALL
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SHIFT                            CSL_BCDMA_TR_FMTFLAGS_AMODE_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_MASK                             CSL_BCDMA_TR_FMTFLAGS_AMODE_MASK
#define CSL_UDMAP_TR_FMTFLAGS_DIR_SHIFT                              CSL_BCDMA_TR_FMTFLAGS_DIR_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_DIR_MASK                               CSL_BCDMA_TR_FMTFLAGS_DIR_MASK
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_SHIFT                            CSL_BCDMA_TR_FMTFLAGS_ELYPE_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_MASK                             CSL_BCDMA_TR_FMTFLAGS_ELYPE_MASK
#define CSL_UDMAP_TR_FMTFLAGS_DFMT_SHIFT                             CSL_BCDMA_TR_FMTFLAGS_DFMT_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_DFMT_MASK                              CSL_BCDMA_TR_FMTFLAGS_DFMT_MASK
#define CSL_UDMAP_TR_FMTFLAGS_SECTR_SHIFT                            CSL_BCDMA_TR_FMTFLAGS_SECTR_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_SECTR_MASK                             CSL_BCDMA_TR_FMTFLAGS_SECTR_MASK
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK0_SHIFT              CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK0_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK0_MASK               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK0_MASK
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK1_SHIFT              CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK1_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK1_MASK               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK1_MASK
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM0_SHIFT               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM0_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM0_MASK                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM0_MASK
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM1_SHIFT               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM1_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM1_MASK                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM1_MASK
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM2_SHIFT               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM2_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM2_MASK                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM2_MASK
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM3_SHIFT               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM3_SHIFT
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AM3_MASK                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AM3_MASK
#define CSL_UDMAP_TR_CACHEFLAGS_CACHEID_SHIFT                        CSL_BCDMA_TR_CACHEFLAGS_CACHEID_SHIFT
#define CSL_UDMAP_TR_CACHEFLAGS_CACHEID_MASK                         CSL_BCDMA_TR_CACHEFLAGS_CACHEID_MASK
#define CSL_UDMAP_TR_CACHEFLAGS_CACHEOP_SHIFT                        CSL_BCDMA_TR_CACHEFLAGS_CACHEOP_SHIFT
#define CSL_UDMAP_TR_CACHEFLAGS_CACHEOP_MASK                         CSL_BCDMA_TR_CACHEFLAGS_CACHEOP_MASK
#define CSL_UdmapTrFmtflagsAmode                                     CSL_BcdmaTrFmtflagsAmode
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_LINEAR                           CSL_BCDMA_TR_FMTFLAGS_AMODE_LINEAR
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_CIRCULAR                         CSL_BCDMA_TR_FMTFLAGS_AMODE_CIRCULAR
#define CSL_UdmapTrFmtflagsAmodeSpecificCbk                          CSL_BcdmaTrFmtflagsAmodeSpecificCbk
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_512B                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_512B
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_1KB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_1KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_2KB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_2KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_4KB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_4KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_8KB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_8KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_16KB                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_16KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_32KB                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_32KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_64KB                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_64KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_128KB               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_128KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_256KB               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_256KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_512KB               CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_512KB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_1GB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_1GB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_2GB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_2GB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_4GB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_4GB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_8GB                 CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_8GB
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_16GB                CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_CBK_16GB
#define CSL_UdmapTrFmtflagsAmodeSpecificAmode                        CSL_BcdmaTrFmtflagsAmodeSpecificAmode
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AMODE_LINEAR            CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AMODE_LINEAR
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AMODE_CBK0              CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AMODE_CBK0
#define CSL_UDMAP_TR_FMTFLAGS_AMODE_SPECIFIC_AMODE_CBK1              CSL_BCDMA_TR_FMTFLAGS_AMODE_SPECIFIC_AMODE_CBK1
#define CSL_UdmapTrFmtflagsDir                                       CSL_BcdmaTrFmtflagsDir
#define CSL_UDMAP_TR_FMTFLAGS_DIR_SRC_USES_AMODE                     CSL_BCDMA_TR_FMTFLAGS_DIR_SRC_USES_AMODE
#define CSL_UDMAP_TR_FMTFLAGS_DIR_DST_USES_AMODE                     CSL_BCDMA_TR_FMTFLAGS_DIR_DST_USES_AMODE
#define CSL_UdmapTrFmtflagsEltype                                    CSL_BcdmaTrFmtflagsEltype
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_1                                CSL_BCDMA_TR_FMTFLAGS_ELYPE_1
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_1p5                              CSL_BCDMA_TR_FMTFLAGS_ELYPE_1p5
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_2                                CSL_BCDMA_TR_FMTFLAGS_ELYPE_2
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_3                                CSL_BCDMA_TR_FMTFLAGS_ELYPE_3
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_4                                CSL_BCDMA_TR_FMTFLAGS_ELYPE_4
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_5                                CSL_BCDMA_TR_FMTFLAGS_ELYPE_5
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_16                               CSL_BCDMA_TR_FMTFLAGS_ELYPE_16
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_32                               CSL_BCDMA_TR_FMTFLAGS_ELYPE_32
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_1_2                              CSL_BCDMA_TR_FMTFLAGS_ELYPE_1_2
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_1p5_2                            CSL_BCDMA_TR_FMTFLAGS_ELYPE_1p5_2
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1                              CSL_BCDMA_TR_FMTFLAGS_ELYPE_2_1
#define CSL_UDMAP_TR_FMTFLAGS_ELYPE_2_1p5                            CSL_BCDMA_TR_FMTFLAGS_ELYPE_2_1p5
#define CSL_UdmapTrFmtflagsDfmt                                      CSL_BcdmaTrFmtflagsDfmt
#define CSL_UDMAP_TR_FMTFLAGS_DFMT_NO_CHANGE                         CSL_BCDMA_TR_FMTFLAGS_DFMT_NO_CHANGE
#define CSL_UDMAP_TR_FMTFLAGS_DFMT_CONSTANT_COPY                     CSL_BCDMA_TR_FMTFLAGS_DFMT_CONSTANT_COPY
#define CSL_UDMAP_TR_FMTFLAGS_DFMT_TRANSPOSE                         CSL_BCDMA_TR_FMTFLAGS_DFMT_TRANSPOSE
#define CSL_UDMAP_TR_FMTFLAGS_DFMT_REVERSE                           CSL_BCDMA_TR_FMTFLAGS_DFMT_REVERSE
#define CSL_UDMAP_TR_FMTFLAGS_DFMT_REVERSE_TRANSPOSE                 CSL_BCDMA_TR_FMTFLAGS_DFMT_REVERSE_TRANSPOSE
#define CSL_UdmapTrCacheflagsOp                                      CSL_BcdmaTrCacheflagsOp
#define CSL_UDMAP_TR_CACHEFLAGS_OP_PREWARM_CACHE                     CSL_BCDMA_TR_CACHEFLAGS_OP_PREWARM_CACHE
#define CSL_UDMAP_TR_CACHEFLAGS_OP_PREWARM_MMU                       CSL_BCDMA_TR_CACHEFLAGS_OP_PREWARM_MMU
#define CSL_UdmapSecTR                                               CSL_BcdmaSecTR
#define CSL_UDMAP_SECTR_FLAGS_TYPE_SHIFT                             CSL_BCDMA_SECTR_FLAGS_TYPE_SHIFT
#define CSL_UDMAP_SECTR_FLAGS_TYPE_MASK                              CSL_BCDMA_SECTR_FLAGS_TYPE_MASK
#define CSL_UDMAP_SECTR_FLAGS_TYPE_SPECIFIC_SHIFT                    CSL_BCDMA_SECTR_FLAGS_TYPE_SPECIFIC_SHIFT
#define CSL_UDMAP_SECTR_FLAGS_TYPE_SPECIFIC_MASK                     CSL_BCDMA_SECTR_FLAGS_TYPE_SPECIFIC_MASK
#define CSL_UdmapTrFmtflagsSectr                                     CSL_BcdmaTrFmtflagsSectr
#define CSL_UDMAP_TR_FMTFLAGS_SECTR_NONE                             CSL_BCDMA_TR_FMTFLAGS_SECTR_NONE
#define CSL_UDMAP_TR_FMTFLAGS_SECTR_64                               CSL_BCDMA_TR_FMTFLAGS_SECTR_64
#define CSL_UDMAP_TR_FMTFLAGS_SECTR_128                              CSL_BCDMA_TR_FMTFLAGS_SECTR_128
#define CSL_UdmapTrResponse                                          CSL_BcdmaTrResponse
#define CSL_UDMAP_TR_RESPONSE_STATUS_TYPE_SHIFT                      CSL_BCDMA_TR_RESPONSE_STATUS_TYPE_SHIFT
#define CSL_UDMAP_TR_RESPONSE_STATUS_TYPE_MASK                       CSL_BCDMA_TR_RESPONSE_STATUS_TYPE_MASK
#define CSL_UDMAP_TR_RESPONSE_STATUS_INFO_SHIFT                      CSL_BCDMA_TR_RESPONSE_STATUS_INFO_SHIFT
#define CSL_UDMAP_TR_RESPONSE_STATUS_INFO_MASK                       CSL_BCDMA_TR_RESPONSE_STATUS_INFO_MASK
#define CSL_UDMAP_TR_RESPONSE_CMDID_SHIFT                            CSL_BCDMA_TR_RESPONSE_CMDID_SHIFT
#define CSL_UDMAP_TR_RESPONSE_CMDID_MASK                             CSL_BCDMA_TR_RESPONSE_CMDID_MASK
#define CSL_UDMAP_TR_RESPONSE_CFG_SPECIFIC_SHIFT                     CSL_BCDMA_TR_RESPONSE_CFG_SPECIFIC_SHIFT
#define CSL_UDMAP_TR_RESPONSE_CFG_SPECIFIC_MASK                      CSL_BCDMA_TR_RESPONSE_CFG_SPECIFIC_MASK
#define CSL_UdmapTrResponseStatus                                    CSL_BcdmaTrResponseStatus
#define CSL_UDMAP_TR_RESPONSE_STATUS_COMPLETE                        CSL_BCDMA_TR_RESPONSE_STATUS_COMPLETE
#define CSL_UDMAP_TR_RESPONSE_STATUS_TRANSFER_ERR                    CSL_BCDMA_TR_RESPONSE_STATUS_TRANSFER_ERR
#define CSL_UDMAP_TR_RESPONSE_STATUS_ABORTED_ERR                     CSL_BCDMA_TR_RESPONSE_STATUS_ABORTED_ERR
#define CSL_UDMAP_TR_RESPONSE_STATUS_SUBMISSION_ERR                  CSL_BCDMA_TR_RESPONSE_STATUS_SUBMISSION_ERR
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_ERR                 CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_ERR
#define CSL_UdmapTrResponseStatusSubmission                          CSL_BcdmaTrResponseStatusSubmission
#define CSL_UDMAP_TR_RESPONSE_STATUS_SUBMISSION_ICNT0                CSL_BCDMA_TR_RESPONSE_STATUS_SUBMISSION_ICNT0
#define CSL_UDMAP_TR_RESPONSE_STATUS_SUBMISSION_FIFO_FULL            CSL_BCDMA_TR_RESPONSE_STATUS_SUBMISSION_FIFO_FULL
#define CSL_UDMAP_TR_RESPONSE_STATUS_SUBMISSION_OWN                  CSL_BCDMA_TR_RESPONSE_STATUS_SUBMISSION_OWN
#define CSL_UdmapTrResponseStatusUnsupported                         CSL_BcdmaTrResponseStatusUnsupported
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_TR_TYPE             CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_TR_TYPE
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_STATIC              CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_STATIC
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_EOL                 CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_EOL
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_CFG_SPECIFIC        CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_CFG_SPECIFIC
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_AMODE               CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_AMODE
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_ELTYPE              CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_ELTYPE
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_DFMT                CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_DFMT
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_SECTR               CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_SECTR
#define CSL_UDMAP_TR_RESPONSE_STATUS_UNSUPPORTED_AMODE_SPECIFIC      CSL_BCDMA_TR_RESPONSE_STATUS_UNSUPPORTED_AMODE_SPECIFIC
#define CSL_udmapTrResponseGetStatusType                             CSL_bcdmaTrResponseGetStatusType
#define CSL_UdmapTdResponse                                          CSL_BcdmaTdResponse
#define CSL_UDMAP_TD_RESPONSE_TD_INDICATOR_SHIFT                     CSL_BCDMA_TD_RESPONSE_TD_INDICATOR_SHIFT
#define CSL_UDMAP_TD_RESPONSE_TD_INDICATOR_MASK                      CSL_BCDMA_TD_RESPONSE_TD_INDICATOR_MASK
#define CSL_UDMAP_TD_RESPONSE_CHAN_ID_SHIFT                          CSL_BCDMA_TD_RESPONSE_CHAN_ID_SHIFT
#define CSL_UDMAP_TD_RESPONSE_CHAN_ID_MASK                           CSL_BCDMA_TD_RESPONSE_CHAN_ID_MASK
#define CSL_UDMAP_TD_RESPONSE_FORCED_SHIFT                           CSL_BCDMA_TD_RESPONSE_FORCED_SHIFT
#define CSL_UDMAP_TD_RESPONSE_FORCED_MASK                            CSL_BCDMA_TD_RESPONSE_FORCED_MASK
#define CSL_udmapGetTdResponse                                       CSL_bcdmaGetTdResponse

/*-----------------------------------------------------------------------------
 * CPPI5-related TR defines and API functions
 *---------------------------------------------------------------------------*/
#define CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_SHIFT                      CSL_BCDMA_TRPD_DESCINFO_DTYPE_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_MASK                       CSL_BCDMA_TRPD_DESCINFO_DTYPE_MASK
#define   CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_HOST                   ((uint32_t) 1U)
#define   CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_MONO                   ((uint32_t) 2U)
#define   CSL_UDMAP_CPPI5_PD_DESCINFO_DTYPE_VAL_TR                     CSL_BCDMA_TRPD_DESCINFO_DTYPE_VAL_TR
#define CSL_UDMAP_CPPI5_PD_DESCINFO_RELOAD_SHIFT                     CSL_BCDMA_TRPD_DESCINFO_RELOAD_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_RELOAD_MASK                      CSL_BCDMA_TRPD_DESCINFO_RELOAD_MASK
#define CSL_UDMAP_CPPI5_PD_DESCINFO_RLDIDX_SHIFT                     CSL_BCDMA_TRPD_DESCINFO_RLDIDX_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_RLDIDX_MASK                      CSL_BCDMA_TRPD_DESCINFO_RLDIDX_MASK
#define CSL_UDMAP_CPPI5_PD_DESCINFO_LASTIDX_SHIFT                    CSL_BCDMA_TRPD_DESCINFO_LASTIDX_SHIFT
#define CSL_UDMAP_CPPI5_PD_DESCINFO_LASTIDX_MASK                     CSL_BCDMA_TRPD_DESCINFO_LASTIDX_MASK
         
#define CSL_UDMAP_CPPI5_PD_PKTINFO_PKTERROR_SHIFT                    ((uint32_t) 28U)
#define CSL_UDMAP_CPPI5_PD_PKTINFO_PKTERROR_MASK                     (((uint32_t) 0xFU) << CSL_UDMAP_CPPI5_TRPD_PKTINFO_PKTERROR_SHIFT)
#define CSL_UDMAP_CPPI5_PD_PKTINFO_RECSIZE_SHIFT                     CSL_BCDMA_TRPD_PKTINFO_RECSIZE_SHIFT
#define CSL_UDMAP_CPPI5_PD_PKTINFO_RECSIZE_MASK                      CSL_BCDMA_TRPD_PKTINFO_RECSIZE_MASK
#define   CSL_UDMAP_CPPI5_PD_PKTINFO_RECSIZE_VAL_16B                   CSL_BCDMA_TRPD_PKTINFO_RECSIZE_VAL_16B
#define   CSL_UDMAP_CPPI5_PD_PKTINFO_RECSIZE_VAL_32B                   CSL_BCDMA_TRPD_PKTINFO_RECSIZE_VAL_32B
#define   CSL_UDMAP_CPPI5_PD_PKTINFO_RECSIZE_VAL_64B                   CSL_BCDMA_TRPD_PKTINFO_RECSIZE_VAL_64B
#define   CSL_UDMAP_CPPI5_PD_PKTINFO_RECSIZE_VAL_128B                  CSL_BCDMA_TRPD_PKTINFO_RECSIZE_VAL_128B
#define CSL_UDMAP_CPPI5_PD_PKTINFO_PKTID_SHIFT                       ((uint32_t) 14U)
#define CSL_UDMAP_CPPI5_PD_PKTINFO_PKTID_MASK                        (((uint32_t) 0x3FFU) << CSL_UDMAP_CPPI5_TRPD_PKTINFO_PKTID_SHIFT)
#define CSL_UDMAP_CPPI5_PD_PKTINFO_FLOWID_SHIFT                      ((uint32_t) 0)
#define CSL_UDMAP_CPPI5_PD_PKTINFO_FLOWID_MASK                       (((uint32_t) 0x3FFFU) << CSL_UDMAP_CPPI5_TRPD_PKTINFO_FLOWID_SHIFT)
         
#define CSL_UDMAP_CPPI5_PD_RETINFO_RETPOLICY_SHIFT                   ((uint32_t) 16U)
#define CSL_UDMAP_CPPI5_PD_RETINFO_RETPOLICY_MASK                    (((uint32_t) 0x1U) << CSL_UDMAP_CPPI5_TRPD_RETINFO_RETPOLICY_SHIFT)
#define   CSL_UDMAP_CPPI5_PD_RETINFO_RETPOLICY_VAL_TO_TAIL               ((uint32_t) 0)
#define   CSL_UDMAP_CPPI5_PD_RETINFO_RETPOLICY_VAL_TO_HEAD               ((uint32_t) 1U)
#define CSL_UDMAP_CPPI5_PD_RETINFO_RETQ_SHIFT                        ((uint32_t) 0)
#define CSL_UDMAP_CPPI5_PD_RETINFO_RETQ_MASK                         (((uint32_t) 0xFFFFU) << CSL_UDMAP_CPPI5_TRPD_RETINFO_RETQ_SHIFT)
         
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_SRCTAG_SHIFT                    ((uint32_t) 16U)
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_SRCTAG_MASK                     (((uint32_t) 0xFFFFU) << CSL_UDMAP_CPPI5_TRPD_SRCDSTTAG_SRCTAG_SHIFT)
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_DSTTAG_SHIFT                    ((uint32_t) 0)
#define CSL_UDMAP_CPPI5_PD_SRCDSTTAG_DSTTAG_MASK                     (((uint32_t) 0xFFFFU) << CSL_UDMAP_CPPI5_TRPD_SRCDSTTAG_DSTTAG_SHIFT)

#define CSL_udmapCppi5TrGetReload                                    CSL_bcdmaCppi5TrGetReload
#define CSL_udmapCppi5TrSetReload                                    CSL_bcdmaCppi5TrSetReload
#define CSL_udmapCppi5TrGetEntryStride                               CSL_bcdmaCppi5TrGetEntryStride
#define CSL_udmapCppi5TrSetEntryStride                               CSL_bcdmaCppi5TrSetEntryStride
#define CSL_udmapCppi5TrSetPktLen                                    CSL_bcdmaCppi5TrSetPktLen
#define CSL_udmapCppi5GetPktLen                                      CSL_bcdmaCppi5GetPktLen
#define CSL_udmapCppi5SetPktLen                                      CSL_bcdmaCppi5SetPktLen

#endif
