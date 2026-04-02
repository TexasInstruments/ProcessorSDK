//  ***************************************************************************
//  * FILE PURPOSE: Define PA/SA/Host structures 
//  ***************************************************************************
//  * FILE NAME: pdsp_psInfo2.h
//  *
//  * DESCRIPTION: Defines structures used among NetCP sub-modules  (PASS/SASS Gen2)
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
// Protocol Information
.struct struct_protoInfo
    .u16    offset
    .u16    len
.ends   

// Protocol related Payload Info long version (from PA)
.struct struct_payloadInfoLong
    .u8     recordLen       // The size of this record 
    .u16    reserved
    .u8     startOffset     // Start Offset relative to packet start
    .u16    eopOffset       // EOP Offset relative to packet start
                            // It should be changed to remove the ESP hash data
    .u8     reserved2       // Not used by SA
    .u8     nextHdrType
#define SA_PAYLOLD_INFO_NEXT_HDR_TYPE_MASK              0x3F
#define SA_PAYLOAD_INFO_NEXT_HDR_TYPE_ESP               10
#define SA_PAYLOAD_INFO_NEXT_HDR_TYPE_DECODED_ESP       11
#define SA_PAYLOAD_INFO_NEXT_HDR_TYPE_AH                12
    .u8     l3Offset
    .u8     l4Offset
    .u8     l5Offset
    .u8     ipsecOffset     // ESP/AH offset
    .u16    reserved3       // not used by SA
    .u16    reserved4       // not used by SA
    .u32    reserved5       // not used by SA
.ends 

// Protocol related Payload Info Short version (from Host)
.struct struct_payloadInfoShort
    .u8     cmdId           // should be 0x20
    .u8     offset          // payload offset
    .u16    len             // payload length
    .u32    countC          // count-C for Air-Ciphering mode (optional)
.ends 

.struct struct_payloadInfoShort2
    .u8     cmdId           // should be 0x20
    .u8     offset          // payload offset
    .u16    len             // payload length
    .u32    countC          // count-C for Air-Ciphering mode (optional)
    .u32    iv1             // optional 8/16-byte IV
    .u32    iv2
    .u32    iv3
    .u32    iv4
.ends    
   
