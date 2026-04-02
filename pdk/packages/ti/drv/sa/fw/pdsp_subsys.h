//  ***************************************************************************
//  * FILE PURPOSE: Define structures common to SA subsystems
//  ***************************************************************************
//  * FILE NAME: pdsp_subsys.h
//  *
//  * DESCRIPTION: Defines structures used in the classify pdsps
//  *
//  ***************************************************************************/
//
//  TEXAS INSTRUMENTS TEXT FILE LICENSE
// 
//   Copyright (c) 2016 Texas Instruments Incorporated
// 
//  All rights reserved not granted herein.
//  
//  Limited License.  
// 
//  Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive 
//  license under copyrights and patents it now or hereafter owns or controls to 
//  make, have made, use, import, offer to sell and sell ("Utilize") this software 
//  subject to the terms herein.  With respect to the foregoing patent license, 
//  such license is granted  solely to the extent that any such patent is necessary 
//  to Utilize the software alone.  The patent license shall not apply to any 
//  combinations which include this software, other than combinations with devices 
//  manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.
// 
//  Redistributions must preserve existing copyright notices and reproduce this license 
//  (including the above copyright notice and the disclaimer and (if applicable) source 
//  code license limitations below) in the documentation and/or other materials provided 
//  with the distribution.
//  
//  Redistribution and use in binary form, without modification, are permitted provided 
//  that the following conditions are met:
// 	No reverse engineering, decompilation, or disassembly of this software is 
//   permitted with respect to any software provided in binary form.
// 	Any redistribution and use are licensed by TI for use only with TI Devices.
// 	Nothing shall obligate TI to provide you with source code for the software 
//   licensed and provided to you in object code.
//  
//  If software source code is provided to you, modification and redistribution of the 
//  source code are permitted provided that the following conditions are met:
// 	Any redistribution and use of the source code, including any resulting derivative 
//   works, are licensed by TI for use only with TI Devices.
// 	Any redistribution and use of any object code compiled from the source code
//   and any resulting derivative works, are licensed by TI for use only with TI Devices.
// 
//  Neither the name of Texas Instruments Incorporated nor the names of its suppliers 
//  may be used to endorse or promote products derived from this software without 
//  specific prior written permission.
// 
//  DISCLAIMER.
// 
//  THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED 
//  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
//  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S 
//  LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
//  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// 
//
//===========================================================
//
// Macros
//

// mov32 : Move a 32bit value to a register
.macro  mov32
.mparam arg1, arg2
        mov     arg1.w0, arg2 & 0xFFFF
        mov     arg1.w2, arg2 >> 16
.endm

// mov16n : Move a 16bit value in network format
.macro  mov16n
.mparam arg1, arg2
#ifdef _BE_
        mov     arg1.w0, arg2.w0
#else
        mov     arg1.b0, arg2.b1
        mov     arg1.b1, arg2.b0
#endif
.endm

// ipsec_icv_check
//
// This macro is used to compare the hash value for both IPSEC
// ESP/AH
//
// Input: fail_label, success_label, icv_size
//   R6-R9: ICV Value from the Authentication Routine
//   R10-R13: ICV value in the packet
//
//
.macro ipsec_icv_check
.mparam fail_label, success_label, icv_size
    qbne    fail_label, r6, r10
    qbeq    success_label, icv_size, 4
    qbne    fail_label, r7, r11
    qbeq    success_label, icv_size, 8
    qbne    fail_label, r8, r12
    qbeq    success_label, icv_size, 12
    qbne    fail_label, r9, r13
.endm 

// srtp_icv_check
//
// This macro is used to compare the hash value for both IPSEC
// ESP/AH
//
// Input: fail_label, success_label, icv_size
//   R6-R9: ICV Value from the Authentication Routine
//   R10-R13: ICV value in the packet
//
//
.macro srtp_icv_check
.mparam fail_label, success_label, icv_size
    qbne    fail_label, r6, r10
    qbeq    success_label, icv_size, 4
    qbne    fail_label, r7, r11
    qbne    fail_label, r8.w2, r12.w2
.endm    
   

//  Scratch memory allocation (0xC000 - 0xCFFF)
//                            (0xD000 - 0xDFFF)
//                            (0xE000 - 0xEFFF)

//  Random number FIFO control block */
//  Refer to the RN FIFO description at ipsec_util.h 
#define phpRnFifoCtrl       0x20

// Function call table 0x30-0xb0
#define phpCallTable        0x30

// IV storage Area     0xb0-0xd0
#define phpIVBase           0xb0
#define phpMaxIVSize        32

// Aux Storage Area    0xe0-0xf0
#define phpAuxBase          0xe0
#define phpMaxAuxSize       16

// Command Label storage area 0x100 - 0x180
#define phpCmdLblBase       0x100

// Temporary buffer for IPv6 routing header reordering
#define phpTempBufBase      0x180

// System statistics storage area 0x200 - 0x300
#define phpStatsBase        0x200

// Version storage area 0x2f0 - 0x300
#define phpVersionBase      0x2f0
#define phpVersionOffset    0x0f0   // offset from phpStatsBase

// IPSEC Command Label Processing function tables
#define phpEspTxCmdlCallTable   0x300
#define phpEspRxCmdlCallTable   0x340
#define phpAhTxCmdlCallTable    0x380
#define phpAhRxCmdlCallTable    0x3c0

// Random Number FIFO data area 0x400 - 0x800
// Refer to the RN FIFO description at ipsec_util.h 
#define phpRnFifoBase           0x400
#define phpRnFifoSize           0x200

// Pkt Instance   0x800 - 0x1000 (0x100 * 8 = 0x800)
// Pkt Instance (index) = 0x800 + 0x100 * (index - 1)
#define pktInstBase                 0x800  
#define pktInstSize                 0x100
// Pkt Instance2   0x800 - 0x1000 (0x80 * 16 = 0x800)
// Pkt Instance2 (index) = 0x800 + 0x80 * (index - 1)
#define pktInst2Size                0x080
// Store the ICV data from/to packet
#define pktInstICVDataOffset        0x20
#define pktInstICVDataOffset2       0x30
// Temporay big buffer: IPSec AH: IP buffer; SRTP: MKI + hash data
#define pktInstTempBufOffset        0x40

// PDSP System Statistics
// Error Statistics
#define PDSP_ERR_NOMEM_OFFSET       0       // PDSP Instance Buffers not available 
#define PDSP_ERR_CTX_OFFSET         4       // Context Cache related errors 
#define PDSP_ERR_ENGINE_OFFSET      8       // SA processing engine related error
#define PDSP_ERR_PROTO_OFFSET       12      // Packet Procotol related error such as
                                            // Invalid IP types
                                            
// IPSEC reltaed system statistics (PDSP1/PDSP3)
#define PDSP_STATS_ESP_OFFSET       0x40
#define PDSP_ESP_REPLAY_OLD_OFFSET  (PDSP_STATS_ESP_OFFSET + 0)
#define PDSP_ESP_REPLAY_DUP_OFFSET  (PDSP_STATS_ESP_OFFSET + 4)
#define PDSP_ESP_AUTH_FAIL_OFFSET   (PDSP_STATS_ESP_OFFSET + 8)
#define PDSP_ESP_ENC_HI_OFFSET      (PDSP_STATS_ESP_OFFSET + 12)
#define PDSP_ESP_ENC_LO_OFFSET      (PDSP_STATS_ESP_OFFSET + 16)
#define PDSP_ESP_DEC_HI_OFFSET      (PDSP_STATS_ESP_OFFSET + 20)
#define PDSP_ESP_DEC_LO_OFFSET      (PDSP_STATS_ESP_OFFSET + 24)

#define PDSP_STATS_AH_OFFSET        0x80
#define PDSP_AH_REPLAY_OLD_OFFSET   (PDSP_STATS_AH_OFFSET + 0)
#define PDSP_AH_REPLAY_DUP_OFFSET   (PDSP_STATS_AH_OFFSET + 4)
#define PDSP_AH_AUTH_FAIL_OFFSET    (PDSP_STATS_AH_OFFSET + 8)
#define PDSP_AH_ENC_HI_OFFSET       (PDSP_STATS_AH_OFFSET + 12)
#define PDSP_AH_ENC_LO_OFFSET       (PDSP_STATS_AH_OFFSET + 16)
#define PDSP_AH_DEC_HI_OFFSET       (PDSP_STATS_AH_OFFSET + 20)
#define PDSP_AH_DEC_LO_OFFSET       (PDSP_STATS_AH_OFFSET + 24)

// SRTP related system statistics (PDSP2)
#define PDSP_STATS_SRTP_OFFSET      0x40
#define PDSP_SRTP_REPLAY_OLD_OFFSET (PDSP_STATS_SRTP_OFFSET + 0)
#define PDSP_SRTP_REPLAY_DUP_OFFSET (PDSP_STATS_SRTP_OFFSET + 4)
#define PDSP_SRTP_AUTH_FAIL_OFFSET  (PDSP_STATS_SRTP_OFFSET + 8)
#define PDSP_SRTP_ENC_HI_OFFSET     (PDSP_STATS_SRTP_OFFSET + 12)
#define PDSP_SRTP_ENC_LO_OFFSET     (PDSP_STATS_SRTP_OFFSET + 16)
#define PDSP_SRTP_DEC_HI_OFFSET     (PDSP_STATS_SRTP_OFFSET + 20)
#define PDSP_SRTP_DEC_LO_OFFSET     (PDSP_STATS_SRTP_OFFSET + 24)

// Air Ciphering related system statistics
#define PDSP_STATS_AC_OFFSET        0x80
#define PDSP_AC_AUTH_FAIL_OFFSET    (PDSP_STATS_AC_OFFSET + 0)
#define PDSP_AC_ENC_HI_OFFSET       (PDSP_STATS_AC_OFFSET + 4)
#define PDSP_AC_ENC_LO_OFFSET       (PDSP_STATS_AC_OFFSET + 8)
#define PDSP_AC_DEC_HI_OFFSET       (PDSP_STATS_AC_OFFSET + 12)
#define PDSP_AC_DEC_LO_OFFSET       (PDSP_STATS_AC_OFFSET + 16)

// Packet Instance Memory Alloc/Free Counters
#define PDSP_STATS_MEM_LEAK_DBG_OFFSET 0xE0
#define PDSP_MEM_ALLOC_CNT               (PDSP_STATS_MEM_LEAK_DBG_OFFSET + 0)
#define PDSP_MEM_EOP_FREE_CNT            (PDSP_MEM_ALLOC_CNT             + 4)
#define PDSP_MEM_ERR_ENGINE_FREE_CNT     (PDSP_MEM_EOP_FREE_CNT          + 4)
#define PDSP_MEM_ERR_INV_PKT_TYPE_CNT    (PDSP_MEM_ERR_ENGINE_FREE_CNT   + 4)

                                            
// PDSP Const
#define PDSP_IRAM1_BASE             0xc000
#define PDSP_IRAM2_BASE             0xd000
#define PDSP_IRAM3_BASE             0xe000
#define PDSP_IRAM4_BASE             0xf000
#define PDSP_ACDBG_BASE             0xf400

#define PHP1_CMDL_BUF_ADDR         (PDSP_IRAM1_BASE + phpCmdLblBase) 
#define PHP1_AUX_BUF_ADDR          (PDSP_IRAM1_BASE + phpAuxBase) 
#define PHP1_INST_BASE_ADDR        (PDSP_IRAM1_BASE + pktInstBase) 
#define PHP1_RN_CTRL_ADDR          (PDSP_IRAM1_BASE + phpRnFifoCtrl)
#define PHP1_RN_FIFO_ADDR          (PDSP_IRAM1_BASE + phpRnFifoBase)
#define PHP1_TEMP_BUF_ADDR         (PDSP_IRAM1_BASE + phpTempBufBase) 


#define PHP2_CMDL_BUF_ADDR         (PDSP_IRAM2_BASE + phpCmdLblBase) 
#define PHP2_AUX_BUF_ADDR          (PDSP_IRAM2_BASE + phpAuxBase) 
#define PHP2_INST_BASE_ADDR        (PDSP_IRAM2_BASE + pktInstBase) 
#define PHP2_RN_CTRL_ADDR          (PDSP_IRAM2_BASE + phpRnFifoCtrl)
#define PHP2_RN_FIFO_ADDR          (PDSP_IRAM2_BASE + phpRnFifoBase)
#define PHP2_TEMP_BUF_ADDR         (PDSP_IRAM2_BASE + phpTempBufBase) 


#define PHP3_CMDL_BUF_ADDR         (PDSP_IRAM3_BASE + phpCmdLblBase) 
#define PHP3_AUX_BUF_ADDR          (PDSP_IRAM3_BASE + phpAuxBase) 
#define PHP3_INST_BASE_ADDR        (PDSP_IRAM3_BASE + pktInstBase) 
#define PHP3_RN_CTRL_ADDR          (PDSP_IRAM3_BASE + phpRnFifoCtrl)
#define PHP3_RN_FIFO_ADDR          (PDSP_IRAM3_BASE + phpRnFifoBase)
#define PHP3_TEMP_BUF_ADDR         (PDSP_IRAM3_BASE + phpTempBufBase) 

//
// PDSP Constant Register definitions
//
#define cRam1Base                   c0      // Scratch RAM1 base
#define cRam2Base                   c1      // Scratch RAM2 base
#define cKeyRamBase                 c2      // Scratch Key Ram base
#define cKeyRamBase2                c4      // Scratch Key Ram base2
// #define cAcDbgBase               c9      // Scratch storage Air Cipher Debug Base
#define cRnFifoCtrl                 c10     // Random Number FIFO control Block 
#define cInstBase                   c11     // Packet Instance Base Address
#define cAuxBuf                     c12     // Temporary storage of Aux (ICV) Data
#define cCmdlBuf                    c13     // Temporary storage of Command Label Table
#define cStats                      c14     // Global Statistics
#define cRnFifoBuf                  c15     // Random Number FIFO base address
#define cEspTxTbl                   c16     // IPSEC ESP Tx Command Label Processing Table
#define cEspRxTbl                   c17     // IPSEC ESP Rx Command Label Processing Table
#define cAhTxTbl                    c18     // IPSEC AH Tx Command Label Processing Table
#define cAhRxTbl                    c19     // IPSEC AH Rx Command Label Processing Table
#define cRngBase                    c20     // RNG Base Address
#define cPkaBase                    c21     // PKA Base Address

//===========================================================
//
// Packet Descriptor
//
.struct struct_PktDesc
    .u8     ThreadId        // Thread ID
    .u8     SA_StatusLen    // CPPI Engress Status Length (Output Hash Length)
    .u16    SA_FullSize     // Full Size in SA
    .u8     SA_NextEngId    // Next Engine ID in SA
    .u8     SA_CmdLblInfo   // Command Label Info in SA
#define   flagCmdLblPresent t7   
    .u8     PsValidSize     // Byte size of valid data in PsInfo
    .u8     PsPhysSize      // Physical byte size of PsInfo
    .u8     ChunkInfo       // Chunk Flags and Type Fields
#define   chunkFlag_discard t2    
#define   chunkFlag_sop     t1
#define   chunkFlag_eop     t0
#define   chunkFlag_pktIndMask  0x03 
    .u8     ChunkInfoPS     // Protocol specific Chunk Info
// Protocol-specific packet error code
#define SA_PKT_ERR_REPLAY_OLD        1    // out of replay window range
#define SA_PKT_ERR_REPLAY_DUP        2    //  duplicated packets
#define SA_PKT_ERR_INVALID_KEY       3    // SRTP Key is out of date
#define SA_PKT_ERR_INVALID_MKI       4    //  MKI out of date, no decryption performed
#define SA_PKT_ERR_AUTH_FAIL         5    // Authentication failure
#define SA_PKT_ERR_INVALID_MKI2      6    //  MKI out of date, invalid decryption performed
    .u8     SrcID           // Source ID
    .u8     FlowIndex       // Flow Index
    .u16    ControlSize     // Control Data Size
    .u16    ChunkSize       // Data Chunk Size
    .u16    PacketId        // Unique ID for this packet
    .u16    DestQueue       // Destination queue
    .u32    Timestamp       // Packet Timestamp
    .u32    SwInfo0         // Software Info Word 0   
    .u8     PktId           // Pkt ID to identify the local instance index
    .u8     reserved
    .u16    SwInfo1_ctxPtr_w0  // Software Info Word 1 (ctx pointer)
.ends

#define  PKT_DESC_SWINFO_CMD_DATA_BIT  t0
#define  PKT_DESC_SWINFO_RE_ENTER_BIT  t1
#define  PAS_PKT_SRC_RIO                4  // Bits 31:27 in pktInfo. Need to get the real value! (not yet in spec) 

.struct struct_PktDesc2
    .u8     ThreadId        // Thread ID
    .u8     SA_StatusLen    // CPPI Engress Status Length (Output Hash Length)
    .u16    SA_FullSize     // Full Size in SA
    .u8     SA_NextEngId    // Next Engine ID in SA
    .u8     SA_CmdLblInfo   // Command Label Info in SA
    .u8     PsValidSize     // Byte size of valid data in PsInfo
    .u8     PsPhysSize      // Physical byte size of PsInfo
    .u8     ChunkInfo       // Chunk Flags and Type Fields
    .u8     ChunkInfoPS     // Protocol specific Chunk Info
    .u8     SrcID           // Source ID
    .u8     FlowIndex       // Flow Index
    .u16    ControlSize     // Control Data Size
    .u16    ChunkSize       // Data Chunk Size
    .u16    PacketId        // Unique ID for this packet
    .u16    DestQueue       // Destination queue
    .u32    Timestamp       // Packet Timestamp
    .u32    SwInfo0         // Software Info Word 0   
    .u32    SwInfo1         // Software Info Word 1   
.ends

//===========================================================
//
// CDE and SCV commands
//
.struct struct_CdeCmd
    .u32    Cmd0
    .u32    Cmd1
.ends

// The function call table. The ordering of the table must
// precisely match the pkt_type, dir, pass ordering in pdsp_sa.h (PA_HDR_xxx)
// index = pkt_type + (dir << 2) + (pass << 3))
.struct struct_phpCalltable
    .u16    acTxProc_1
    .u16    srtpTxProc_1
    .u16    ahTxProc_1
    .u16    espTxProc_1
    .u16    acRxProc_1
    .u16    srtpRxProc_1
    .u16    ahRxProc_1
    .u16    espRxProc_1
    .u16    acTxProc_2
    .u16    srtpTxProc_2
    .u16    ahTxProc_2
    .u16    espTxProc_2
    .u16    acRxProc_2
    .u16    srtpRxProc_2
    .u16    ahRxProc_2
    .u16    espRxProc_2
    .u16    acTxProc_3
    .u16    srtpTxProc_3
    .u16    ahTxProc_3
    .u16    espTxProc_3
    .u16    acRxProc_3
    .u16    srtpRxProc_3
    .u16    ahRxProc_3
    .u16    espRxProc_3
.ends

//
// General Command Label Processing function
//
.struct struct_cmdlFuncTable
    .u16    func1
    .u16    func2
    .u16    func3
    .u16    func4
    .u16    func5
    .u16    func6
    .u16    func7
    .u16    func8
.ends

// argument passing and return values
.struct  struct_arg
    .u16   arg1
    .u16   arg0
.ends


// PHP System Context
.struct struct_phpContext
    .u8     instMask            // instance mask 0: free 1: busy
                                // reduce number of instance buffers from 16 to 8
                                // since we need to increase the instance buffer size
                                // from 0x80 to 0x100 to handle IPv6 extension header
    .u8     rsvd
    .u8     ctrlInfo            // control Info: see definitions below
                                // Bit 0:4: Pke Inst index for Pass 1
                                // Store pktId for Pass 1
#define   PHP_INST_INDEX_MASK   0x1f
#define   PHP_INST_INDEX_MASK_NOT 0xE0
    .u8     chunkInfo           // Record Chunk Info of the data chunk
    // global data per data chunk processing  
    .u16    instOffset          // Instance offset from the instance array base
    .u16    chunkSize           // Data Chunk Size
.ends

// PHP System Context2
.struct struct_phpContext2
    .u16    instMask            // instance mask 0: free 1: busy
    .u8     ctrlInfo            // control Info: see definitions below
                                // Bit 0:4: Pke Inst index for Pass 1
                                // Store pktId for Pass 1
// below defines are already defined as above, should be used for readability
// #define   PHP_INST_INDEX_MASK   0x1f
// #define   PHP_INST_INDEX_MASK_NOT 0xE0
    .u8     chunkInfo           // Record Chunk Info of the data chunk
    // global data per data chunk processing
    .u16    instOffset          // Instance offset from the instance array base
    .u16    chunkSize           // Data Chunk Size
.ends

#define     t_php_dbg_halt       t7
#define     t_php_pkt_dbg_info   t6

.struct struct_acDbgCxt
     .u16   offset
     .u8    index    // Air cipher debug index (0-15)
.ends     

// There is no register layout to hold this, so the elements are written in the order
// they are written to scratch memory for debug
.struct struct_acDbgInfo
     .u32   swInfo0
     .u32   swInfo1
     .u16   intBufIndex
     .u16   pktInstMask
     .u32   pktSize
     .u32   pktInfo0
     .u32   pktInfo1
     .u32   pktInfo2
     .u32   cmdLbl0
     .u32   cmdLbl1
.ends


#define SA_CMDL_BASIC_SIZE      8    

// SA Command Label
.struct struct_saCmdL
    .u8     nextEngId
    .u8     cmdlLen
    .u16    procLen
    .u8     sopOffset
    .u8     optctrl1
    .u8     optctrl2
    .u8     optctrl3
.ends 

// SA Command Label Register View to save copy cycles
.struct struct_saCmdLReg
    .u32    reg1
    .u32    reg2
.ends 

// SA Command Label Option data
.struct struct_saCmdlOpt
    .u32    data1
    .u32    data2
    .u32    data3
    .u32    data4
.ends 

// SA Random Number FIFO control block
// Refer to the RN FIFO description at ipsec_util.h 
// This structure will be maintained at phpRnFifoCtrl 
// Simple FIFO with in/out pointer
// FIFO will be filled with random bytes at stratup
// PDSP2 will insrt randon bytes and update in pointer
// PDSP1 will consume the data and update out pointer
// The FIFO is considered full when in == out
//.struct struct_saRnFifoCb
//    .u16    in          
//    .u16    cnt
//    .u16    out
//.ends  
//
//.struct struct_saRnFifoPushCb
//    .u16    in          
//    .u16    cnt
//.ends 
//
//.struct struct_saRnFifoPopCb
//    .u16    cnt
//    .u16    out
//.ends  
//
//

.struct struct_saRnFifoCb
    .u16    in          
    .u16    out
.ends  
  

//******************************************************************************
// * Type:         SA_IPSEC_ESP_PKT_INST_T        
// ******************************************************************************
// * Description: This structure specifies the PKT_INST for IPSEC ESP processing 
// *
// *****************************************************************************/
//typedef struct SA_IPSEC_ESP_PKT_INST_tag
//{
//    UINT8 ctrl_bitfield;       /* various IPSEC ESP control information */            
//                                    
//    /*
//     *  Bit 0:   Hash Value Available in the data chunk
//     *  Bit 1:   Remaining Hash value pending to be processed
//     *  Bit 2:   Hash value processing is complete
//     *  Bit 3-7: Reserved 
//     */ 
//       
//    #define SA_IPSEC_ESP_SET_FLAG_HASH_AVAIL(a,b)     UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 1)
//    #define SA_IPSEC_ESP_CHECK_FLAG_HASH_AVAIL(a)     UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 1)
//    #define SA_IPSEC_ESP_SET_FLAG_HASH_PENDING(a,b)   UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 1, 1)
//    #define SA_IPSEC_ESP_CHECK_FLAG_HASH_PENDING(a)   UTL_GET_BITFIELD((a)->ctrl_bitfield, 1, 1)
//    #define SA_IPSEC_ESP_SET_FLAG_HASH_DONE(a,b)      UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_IPSEC_ESP_CHECK_FLAG_HASH_DONE(a)      UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//    
//    UINT8  next_eng_id;
//    UINT8  next_eng_id_pass2;
//    
//    UINT8  esp_hdr_offset;   /* TBD: offset to the ESP header */
//    UINT16 icv_offset;       /* offset to the ICV (integrity check value or also name as Authentication tag */
//    UINT16 icv_offset_pass2; /* offset to the ICV for pass 2 */
//    UINT8  hash_recorded;    /* record number of hash bytes which has been recored (Rx only)*/
//    UINT8  esn_inserted;     /* number of bytes upper-32 bit ESN which has been inserted */
//    
//    SA_IPSEC_ESN_T  esn;     /* extended sequence number of the received packet (Rx only) */    
//    
//    UINT8  iv[SA_IPSEC_MAX_IV_SIZE]; /* store the IV data TBD: can be local */
//    
//    UINT8  aux_data[SA_IPSEC_MAX_AUX_DATA_SIZE]; /* store the Auxiliary data (AAD)  
//                                                  * TBD: write to command label directly
//                                                  */
//                                                  
//    UINT8  hash_data[SA_IPSEC_MAX_HASH_DATA_SIZE]; /* Extracted hash value (Rx only) */
//    
//} SA_IPSEC_ESP_PKT_INST_T;

.struct struct_espPktInst
    .u16 icvOffset
    .u16 icvOffset_pass2
    .u32 esnHi
    .u32 esnLo
    .u8  ctrlFlag                // various control flags
    #define espHashAvail        t0      // Hash is available for PHP Pass1
    #define espHashPending      t1      // More hash data needs to be extracted from the following data chunk
    #define espHashComplete     t2      // Hash operation is complete for PHP Pass1
    #define espPktError         t3
    #define espHashAvail2       t4      // Hash is avialable for PHP Pass 2
    #define espHashComplete2    t5      // Hash operation is complete for PHP Pass2
    #define espLeftoverBytes    t6      // extra two bytes letftover from the previous data chunk
    .u8  nextEngId
    .u8  nextEngId_pass2
    .u8  hashRecorded
    .u16 leftoverBytes                  // bytes leftover from the previous data chunk per alignment requirement
.ends

.struct struct_espTxPktInst
    .u16 icvOffset
    .u8  encryptedOffset
    .u8  reserved1
    .u16 icvOffset_pass2
    .u8  ctrlFlag               // various control flags as defined at truct_espPktInst.ctrlFlag
    .u8  nextEngId
    .u8  nextEngId_pass2
    .u8  hashRecorded
    .u16 leftoverBytes          // bytes leftover from the previous data chunk per alignment requirement
.ends


#define ESP_PKT_INST_HASH_DATA_OFFSET     pktInstICVDataOffset
#define ESP_PKT_INST_HASH_DATA_OFFSET2    pktInstICVDataOffset + 16


///******************************************************************************
// * Type:        SA_IPSEC_AH_PKT_INST_T        
// ******************************************************************************
// * Description: This structure specifies the PKT_INST for IPSEC AH processing
// *
// *****************************************************************************/
//typedef struct SA_IPSEC_AH_PKT_INST_tag
//{
//
//    UINT8 ctrl_bitfield;       /* various IPSEC AH control information */            
//
//    /*
//     *  Bit 0:   IPV6 flag
//     *  Bit 1:   ESN flag 
//     *  Bit 2:   HASH Received
//     *  Bit 3:   HASH Inserted
//     *  Bit 4-7: Reserved 
//     */   
//    #define SA_IPSEC_AH_SET_IPV6(a,b)               UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 1)
//    #define SA_IPSEC_AH_IS_IPV6(a)                  UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 1)
//    #define SA_IPSEC_AH_SET_FLAG_ESN_INSERTED(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 1, 1)
//    #define SA_IPSEC_AH_CHECK_FLAG_ESN_INSERTED(a)  UTL_GET_BITFIELD((a)->ctrl_bitfield, 1, 1)
//    #define SA_IPSEC_AH_SET_FLAG_HASH_AVAIL(a,b)    UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_IPSEC_AH_CHECK_FLAG_HASH_AVAIL(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//    #define SA_IPSEC_AH_SET_FLAG_HASH_DONE(a,b)     UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 3, 1)
//    #define SA_IPSEC_AH_CHECK_FLAG_HASH_DONE(a)     UTL_GET_BITFIELD((a)->ctrl_bitfield, 3, 1)
//    
//    
//    UINT8  next_eng_id;
//    UINT8  next_eng_id_pass2;
//    
//    UINT8  ip_offset;      /* offset to the outer IP header */
//    UINT8  ah_offset;      /* offset to the ah header (TBD: can be moved to loacl) */ 
//    UINT8  icv_offset;     /* offset to the hash value; may be derived from the ah_offset */
//    UINT8  ip_hdr_size;    /* total size of ip header and options */
//    
//    UINT16 esn_offset;     /* offset to the ESN insrted location */
//    UINT16 esn_offset_pass2; /* offset to the ESN insrted location */
//    
//    SA_IPSEC_ESN_T  esn;   /* extended sequence number of the received packet (Rx only) */    
//    
//    
//    UINT8  ip_hdr_buf[SA_IPSEC_MAX_IPHDR_OPT_SIZE]; /* store the original IP header and options */
//    UINT8  hash_data[SA_IPSEC_MAX_HASH_DATA_SIZE]; /* Extracted hash value */
//    UINT8  iv[SA_IPSEC_MAX_IV_SIZE]; /* ??? store the IV data TBD: can be local */
//    
// } SA_IPSEC_AH_PKT_INST_T;

.struct struct_ahPktInst
    .u16 esnOffset
    .u8  ipOffset
    .u8  icvOffset
    .u8  ipHdrSize
    .u8  ctrlFlag                // various control flags
    #define ahIPV6             t0
    #define ahESNInserted      t1 
    #define ahHashAvail        t2
    #define ahHashComplete     t3
    #define ahErr              t4 
    #define ahLeftoverBytes    t5      // extra two bytes letftover from the previous data chunk
    .u16 esnOffset_pass2
    .u32 esnHi
    .u32 esnLo
    .u8  nextEngId
    .u8  nextEngId_pass2
    .u16 leftoverBytes          // bytes leftover from the previous data chunk per alignment requirement 
.ends

#define AH_PKT_INST_HASH_DATA_OFFSET     pktInstICVDataOffset 
#define AH_PKT_INST_IPBUF_OFFSET         pktInstTempBufOffset    
    
///******************************************************************************
// * Type:         SA_SRTP_PKT_INST_T        
// ******************************************************************************
// * Description: This structure specifies the PKT_INST for SRTP processing 
// *
// *****************************************************************************/
//typedef struct SA_SRTP_PKT_INST_tag
//{
//    UINT8  ctrl_bitfield;       /* various SRTP control information */            
//                                    
//    /*
//     *  Bit 0:   MKI and Hash Value Available in the PS Info section
//     *  Bit 1:   Remaining MKI and Hash value pending to be recorded
//     *  Bit 2:   MKI and Hash value processing is complete
//     *  Bit 3-7: Reserved 
//     */ 
//       
//    #define SA_SRTP_SET_FLAG_MKI_HASH_AVAIL(a,b)    UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 1)
//    #define SA_SRTP_CHECK_FLAG_MKI_HASH_AVAIL(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 1)
//    #define SA_SRTP_SET_FLAG_MKI_HASH_PENDING(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 1, 1)
//    #define SA_SRTP_CHECK_FLAG_MKI_HASH_PENDING(a)  UTL_GET_BITFIELD((a)->ctrl_bitfield, 1, 1)
//    #define SA_SRTP_SET_FLAG_MKI_HASH_DONE(a,b)     UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_SRTP_CHECK_FLAG_MKI_HASH_DONE(a)     UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//    
//    UINT8  next_eng_id;
//    UINT8  next_eng_id_pass2;
//    
//    UINT8  roc_status;
//    
//    
//    UINT8  rtp_offset;          /* Offset to the RTP header */
//    UINT8  payload_offset;      /* Offset to the RTP payload */     
//    UINT16 mki_offset;
//    UINT16 mki_offset_pass2;
//    
//    UINT32 temp_roc;            /* v: Local stored roc before the update is complete (may be removed) */
//    UINT16 seq_num;             /* stored the seqence number in the receive packet */
//    UINT8  mki_hash_recorded;
//    UINT8  reserved;
//    
//    UINT8  mki_hash_data[SA_SRTP_MAX_AUTH_TAG_SIZE + SA_SRTP_MAX_MKI_SIZE]; 
//    UINT8  cmdl_buf[SA_SRTP_MAX_CMDL_SIZE]; /* store the command labels to be copied to 
//                                             * control info area */
//    
//
//} SA_SRTP_PKT_INST_T;

#define SA_SRTP_ROC_NOCHANGE    0
#define SA_SRTP_ROC_MINUS_1     1
#define SA_SRTP_ROC_PLUS_1      2

.struct struct_srtpPktInst
    .u8  mkiOffset_sc            // mkioffset for single_chunk packet 
    .u8  rocStatus 
    .u16 seqNum
    .u32 tempROC
    .u8  ctrlFlag                // various control flags
    #define srtpMkiHashAvail        t0      // Hash is available for PHP Pass1
    #define srtpMkiHashPending      t1      // More hash data needs to be extracted from the following data chunk
    #define srtpMkiHashComplete     t2      // Hash operation is complete for PHP Pass1
    #define srtpPktError            t3
    #define srtpReKeyFailure        t4
    #define srtpMkiVerified         t5      // MKI has been verified at PHP Pass 1
    #define srtpMkiHashAvail2       t6      // Hash is available for PHP Pass2
    #define srtpMkiHashComplete2    t7      // Hash operation is complete for PHP Pass2
    .u8  nextEngId
    .u8  nextEngId_pass2
    .u8  mkiHashRecorded
    .u16 mkiOffset
    .u16 mkiOffset_pass2        
    .u8  ctrlFlag2              // various control flags
    #define srtpLeftoverBytes       t0      // extra two bytes letftover from the previous data chunk
    #define srtpNullMode            t1      // Both encryption mode and authentication mode are null
    .u8  rsvd1  
    .u16 leftoverBytes          // bytes leftover from the previous data chunk per alignment requirement 
    
.ends

#define SRTP_PKT_INST_MKI_HASH_DATA_OFFSET  pktInstICVDataOffset
#define SRTP_PKT_INST_HASH_DATA_OFFSET2     pktInstICVDataOffset + 16
 

///******************************************************************************
// * Type:         SA_AC_PKT_INST_T        
// ******************************************************************************
// * Description: This structure specifies the PKT_INST for Air Cipher processing 
// *
// *****************************************************************************/
//typedef struct SA_AC_PKT_INST_tag
//{
//    UINT8  ctrl_bitfield;       /* various SRTP control information */            
//                                    
//    /*
//     *  Bit 0:   Hash Value Available in the PS Info section
//     *  Bit 1:   Remaining  Hash value pending to be recorded
//     *  Bit 2:   Hash value processing is complete
//     *  Bit 3-7: Reserved 
//     */ 
//       
//    #define SA_AC_SET_FLAG_HASH_AVAIL(a,b)    UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 0, 1)
//    #define SA_AC_CHECK_FLAG_HASH_AVAIL(a)    UTL_GET_BITFIELD((a)->ctrl_bitfield, 0, 1)
//    #define SA_AC_SET_FLAG_HASH_PENDING(a,b)  UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 1, 1)
//    #define SA_AC_CHECK_FLAG_HASH_PENDING(a)  UTL_GET_BITFIELD((a)->ctrl_bitfield, 1, 1)
//    #define SA_AC_SET_FLAG_HASH_DONE(a,b)     UTL_SET_BITFIELD((a)->ctrl_bitfield, b, 2, 1)
//    #define SA_AC_CHECK_FLAG_HASH_DONE(a)     UTL_GET_BITFIELD((a)->ctrl_bitfield, 2, 1)
//    
//    
//    UINT8  next_eng_id_pass2;
//    UINT8  payload_offset;      /* Offset to the RTP payload */
//    UINT16 icv_offset;
//    UINT8  hash_recorded;       /* record number of hash bytes which has been recored (From Air only)*/
//    
//    UINT8  hash_data[SA_AC_MAX_HASH_DATA_SIZE]; /* Extracted hash value (Rx only) */
//    UINT8  cmdl_buf[SA_AC_MAX_CMDL_SIZE]; /* store the command labels to be copied to 
//                                             * control info area */
//} SA_AC_PKT_INST_T;

//
// There is only one special case where PHP PASS3 is required
// Kasumi-F8/F9 with Padding in the from-air direction  
// Encryption command: next pass = PHP PASS2, nextEngId_pass2 = Air-Cipher 2  
// Authentication command: next pass: PHP PASS3
// PHP Pass1: Command processing
// PHP Pass2: Insert Padding
// PHP Pass3: Delete Padding and compare checksum


.struct struct_acPktInst
    .u16 ctrlFlag                // various control flags
    #define acHashAvail          t0
    #define acHashPending        t1
    #define acHashComplete       t2
    #define acPktError           t3
    #define acIvInserted         t4
    #define acPaddingInserted    t5
    #define acPaddingComplete    t6
    #define acPaddingInserted2   t7           // Insert Padding at the second PASS
    #define acIvCopied           t8    // IV has been copied to either the command or packet buffer
    .u8  nextEngId
    .u8  nextEngId_pass2
    .u8  hashRecorded
    .u8  hdrOffset               // Use to insert and delete CMAC IV
    .u16 icvOffset
    .u16 icvOffset_pass2
    .u16 icvOffset_pass3         // only if acPaddingInserted2 is set
    .u32 countC
.ends

// Data Mode packet instance for alignmnet workaround
.struct struct_dmPktInst
    .u16 leftover_w2
    .u8  leftover_b1
    .u8  numLeftoverBytes
.ends

#define AC_PKT_INST_HASH_DATA_OFFSET   pktInstICVDataOffset 
#define AC_PKT_INST_HASH_DATA_OFFSET2  (pktInstICVDataOffset + 16)
     
