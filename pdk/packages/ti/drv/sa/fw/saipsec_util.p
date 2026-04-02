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
//
//
// Note that the window is defined by a base and a size. The base is the
// lowest index value that can be considered for updating or checking,
// while the highest acceptable value for checking is win_base+win_size-1.
//
// On updating, if the index value is outside the window and "newer" than
// the window, then the value of win_base will be adjust so that the
// index will "just fit" within the window.
//
//
//                |                           |
//                |<------- win_size -------->|
//                |                           |
//   -------------+---------------------------+-------------------------
//                ^                          ^
//   << Older     |                          |                  Newer >>
//
//            win_base                  Highest seq idx
//                                      we will accept
//
//
// The window "packet present" bits are arranged as a 32-bit uint array.
// Each array element tracks 32 index values. The array is circular in
// nature so that the top wraps back to the bottom. It is one uint larger
// than the maximum window size, allowing for up to 32-bits of "spill-over"
// as new indices are added.
//
// The value of "win_base" is the lowest packet index value that is still
// in the window. Any packet coming before this base is not considered.
// The position of the bit corresponding to the window base in the physical
// bit mask array is determined by two variables; "win_mask_index" is the
// index of the 32-bit word in the mask array that hold the bit value,
// and "win_mask_bitoff" is the bit offset within that word.
//
// As the window slides, the flag bits are not moved. Instead, the view
// base into the uint array is altered. This is done by incrementing the
// value of "win_mask_bitoff" (this is always "win_base & 0x1F"), but it
// is tracked separately since it also controls when "win_mask_index" is
// incremented. Once the value of "win_mask_bitoff" is greater than 31,
// the old array word pointed to by "win_mask_index" is cleared and the
// value of "win_mask_index" is incremented. This puts the cleared value
// onto the head of the bit mask, and it is then used for new flags at the
// top of the window.
//
//
//  |    32 bits     |    32 bits     |    32 bits     |    32 bits     |
//  |     newest     |     oldest     |     older      |     newer      |
//  +----------------+----------------+----------------+----------------+
//                   ^         ^
//                   |         |
//             win_mask_index  |
//                        Window Base
//
//   win_mask_bitoff |<------->|
//
//  --------->|                |<------------------- Valid Window -------
//
//
//
// #include <stdio.h>

// typedef unsigned int uint;


//#define MAXWINDOW       128             // Max window size is 128 packets
//#define MAX_WORD_LEN    (((MAXWINDOW+31)/32)+1)

.struct struct_replayCheckLoc
    .u32    seqNum 
    .u32    seqNumDiff
    .u32    slideDelta
    .u8     bitNumber              // obsolete because it is too small to handle winSize >= 256
                                   // use r0.w0 instead
    .u8     indexOffset            // in bytes
    .u8     bitOffset              // up to 31
    .u8     rc                     // function return value
.ends

//
// Register Allocation
//
// Assume r0, r10 to r13 are scratch.
//

// The following are valid all the time
// Stratch R0, 
// Local variable: r10-R13
// replayCtrl: R14-R21
// Local: R6-R9  
// 
// R10-R13: Local variable
// R6-R9: Temporary storage for the second set of replay windows
// R29.b1: size of winMask in bytes
// R4-R5:Security context window move command
// mov     CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | ((SIZE(struct_ctxComm) + SIZE(replayCtrl))<< 8)
// xout    XID_SCVCTRL, CdeCmd, 4        
// 
// Input Parameter r29
// return value    r29.b0 
// return address  r30.w2

.enter  replayCheckScope
.assign struct_replayCheckLoc,  R10, R13, replayCheckLoc
.leave  replayCheckScope
                      
.using replayCheckScope   
.using replayCtrlScope

// Replay_Init
//
// This function is called to initialize the replay context, or
// to re-initialize it if an index it entirely out of range.
//
//void Replay_Init( REPLAY_CONTEXT *pReplay, uint win_base )
//{
//    // Initialize the mask
//    memset( pReplay->win_mask, 0, sizeof(pReplay->win_mask) );
//
//    // Setup the window to be based at the supplied sequence number
//    pReplay->win_mask_index = 0;
//    pReplay->win_base = win_base;
//
//    // For simplicity, we align the mask set on the same 32-bit
//    // alignment as the index value. This isn't really necessary,
//    // but it makes the array values a little easier to read, and
//    // doesn't have any affect on code size or performance.
//    pReplay->win_mask_bitoff = win_base & 0x1f;
//}

// Replace function with MACRO
// Replay_init(win_base)

Replay_Init:
        // Setup the window to be based at the supplied sequence number
        mov     replayCtrl.winMaskIndex, 0
        
        // Initialize winMaskBitoff
        and     replayCtrl.winMaskBitoff, replayCtrl.winBase, 0x1f
        
        //Clear the win_mask
        zero    &replayCtrl.winMask0, MAX_WINMASK_BYTE_LEN
        
#ifdef SASS_GEN2        
        qbge    Replay_Init_end,  replayCtrl.winSize, 128   
            // Chear the second half of win_masks: win_mask[5] ... win_mask[33] if winSize > 128
            mov  CdeCmd.Cmd0.w0, SCV_CMD_POSITION_WINDOW | (128 << 8)
            
Replay_Init_loop:            
            xout XID_SCVCTRL, CdeCmd, 4  
            
            // R6-R9 has been initialized to 0
            xout XID_SCVDATA, r6, 16
            
            // for next 16 bytes
            add  CdeCmd.Cmd0.b1, CdeCmd.Cmd0.b1, 16
            
            qbge Replay_Init_loop, CdeCmd.Cmd0.b1, 128 + (4*28)
                // Move Context window back to it original location
                mov CdeCmd.Cmd0.b1, SIZE(struct_ctxComm)
                xout XID_SCVCTRL, CdeCmd, 4
                // pass through
#endif            
Replay_Init_end:        
        ret     


// Replay_CheckAndUpdate
//
// This function is a combination of checking the index value and
// then updating the bit mask array.
//
// If the packet status is SA_REPLAY_RC_OK or SA_REPLAY_RC_NEW, then the mask array is
// updated. When the status is SA_REPLAY_RC_NEW, the window is also shifted
// so that the new value is just within the window.
//
// Return Value:
//    SA_REPLAY_RC_OLD - Packet is older than the window
//    SA_REPLAY_RC_DUP - Packet is inside the window, but has been seen before
//    SA_REPLAY_RC_OK  - Packet in inside the window and has not been seen
//    SA_REPLAY_RC_NEW - Packet is newer than the window
//

//int Replay_CheckAndUpdate(uint seq_num, uint win_size )
Replay_CheckAndUpdate:
//int Replay_CheckAndUpdate( REPLAY_CONTEXT *pReplay, uint seqNum, uint win_size )
//{
//    uint seqNumDiff,bit_number,index_offset,bit_offset;
//    int  rc = SA_REPLAY_RC_OK;
//
      mov     r30.w2, r30.w0          // Save RET addr to R30.w2
      
      mov     replayCheckLoc.seqNum, r29
      mov     r29.b0, SA_REPLAY_RC_OK
      mov     r29.b1, r1.b0           // r1.b0 is used as index of MVIx instruction
      
      // record winMask size

//    // Do the subract first so we are immune to 32-bit wrap
//    seqNumDiff = seqNum - pReplay->winBase;

      sub replayCheckLoc.seqNumDiff, replayCheckLoc.seqNum, replayCtrl.winBase
      
//
//    // If the delta is "negative", then this packet comes before our base.
//    if( seqNumDiff & (1<<31) )
//        return SA_REPLAY_RC_OLD;
//

      qbbc  Replay_CheckAndUpdate_L1, replayCheckLoc.seqNumDiff, 31
        mov   r29.b0, SA_REPLAY_RC_OLD
        jmp   Replay_CheckAndUpdate_End 
      
     
Replay_CheckAndUpdate_L1:
      
//    // See if the packet falls beyond our window
//    if( seqNumDiff >= win_size )
//    {
//        uint slide_delta;
//
//        slide_delta = seqNumDiff-win_size+1;
//
//        // If we have to slide more than the window size, then its
//        // faster to just re-init.
//        if( slide_delta >= win_size )
//            Replay_Init( pReplay, seqNum-win_size+1 );
//        else
//        {
//            pReplay->winBase += slide_delta;
//            pReplay->winMaskBitoff += slide_delta;
//
//            // If we're still in the same 32-bit word, then we're done, but if
//            // we moved out of the 32-bit word, we need to recover any empty
//            // word for re-use. This means moving the mask index and zeroing
//            // out the value.
//            while( pReplay->winMaskBitoff>=32 )
//            {
//                pReplay->winMaskBitoff -= 32;
//                pReplay->win_mask[pReplay->winMaskIndex] = 0;
//                if( ++pReplay->winMaskIndex == MAX_WORD_LEN )
//                    pReplay->winMaskIndex = 0;
//            }
//        }
//
//        // We know we're alway setting the bit at offset "win_size-1"
//        seqNumDiff = win_size - 1;
//
//        // The state here is SA_REPLAY_RC_NEW
//        rc = SA_REPLAY_RC_NEW;
//    }


    qbgt    Replay_CheckAndUpdate_L4, replayCheckLoc.seqNumDiff, replayCtrl.winSize  
        zero    &r6,  16                // Initialize r6-r9 to 0 for init or winMask clean up operation
        sub     replayCheckLoc.slideDelta, replayCheckLoc.seqNumDiff, replayCtrl.winSize
        add     replayCheckLoc.slideDelta, replayCheckLoc.slideDelta, 1
    
//        if( slideDelta >= winSize )
//            Replay_Init( pReplay, seqNum-winSize+1 );
        qbgt    Replay_CheckAndUpdate_L2, replayCheckLoc.slideDelta, replayCtrl.winSize
            sub     replayCtrl.winBase, replayCheckLoc.seqNum, replayCtrl.winSize
            add     replayCtrl.winBase, replayCtrl.winBase, 1
            call    Replay_Init
            jmp     Replay_CheckAndUpdate_L3 
    
//        else
//        {
//            pReplay->winBase += slideDelta;
//            pReplay->winMaskBitoff += slideDelta;
//
//            // If we're still in the same 32-bit word, then we're done, but if
//            // we moved out of the 32-bit word, we need to recover any empty
//            // word for re-use. This means moving the mask index and zeroing
//            // out the value.
//            while( pReplay->winMaskBitoff>=32 )
//            {
//                pReplay->winMaskBitoff -= 32;
//                pReplay->win_mask[pReplay->winMaskIndex] = 0;
//                if( ++pReplay->winMaskIndex == MAX_WORD_LEN )
//                    pReplay->winMaskIndex = 0;
//            }
//        }
    
Replay_CheckAndUpdate_L2:
    
            add     replayCtrl.winBase,  replayCtrl.winBase, replayCheckLoc.slideDelta
            add     replayCtrl.winMaskBitoff,  replayCtrl.winMaskBitoff, replayCheckLoc.slideDelta
            
#ifdef SASS_GEN2      
            // If winSize > 128, need to deal with winMask at both regions
            qblt    Replay_CheckAndUpdate_L2_LagreWindow,  replayCtrl.winSize, 128
#endif            
            //mov     r0, 0
Replay_CheckAndUpdate_LOOP1:
            qbgt    Replay_CheckAndUpdate_L3, replayCtrl.winMaskBitoff, 32
                sub     replayCtrl.winMaskBitoff, replayCtrl.winMaskBitoff, 32
                add     r1.b0,  replayCtrl.winMaskIndex, &replayCtrl.winMask0 
                mvid    *r1.b0, r6
                add     replayCtrl.winMaskIndex, replayCtrl.winMaskIndex, 4
                qbne    Replay_CheckAndUpdate_LOOP1,  replayCtrl.winMaskIndex, MAX_WINMASK_BYTE_LEN
                    ldi     replayCtrl.winMaskIndex, 0
                    jmp     Replay_CheckAndUpdate_LOOP1    
                    
#ifdef SASS_GEN2                    
Replay_CheckAndUpdate_L2_LagreWindow:
             // winMask clear in the case of large replay window
             // duplicate code to minimize the cycle impact for small window size
            // Chear the second half of win_masks: win_mask[5] ... win_mask[33] if winSize > 128
            mov  CdeCmd.Cmd0.b0, SCV_CMD_POSITION_WINDOW
             
Replay_CheckAndUpdate_LOOP2:
            qbgt    Replay_CheckAndUpdate_L2_LagreWindow_end, replayCtrl.winMaskBitoff, 32
                sub     replayCtrl.winMaskBitoff, replayCtrl.winMaskBitoff, 32
                
                // winMaskIndex in bytes, if >= 20, move to secnd half of window and set a flag
                qble    Replay_CheckAndUpdate_LOOP2_1, replayCtrl.winMaskIndex, 20 
                    //within replayCtrl
                    add     r1.b0,  replayCtrl.winMaskIndex, &replayCtrl.winMask0 
                    mvid    *r1.b0, r6
                    jmp     Replay_CheckAndUpdate_LOOP2_2
Replay_CheckAndUpdate_LOOP2_1:
                    // winMask at the second group of replay windows
                    add     CdeCmd.Cmd0.b1,  replayCtrl.winMaskIndex, 128 - 20
                    xout XID_SCVCTRL, CdeCmd, 4  
            
                    // R6 has been initialized to 0
                    xout XID_SCVDATA, r6, 4
                    // pass through
                
Replay_CheckAndUpdate_LOOP2_2:                
                add     replayCtrl.winMaskIndex, replayCtrl.winMaskIndex, 4
                qbne    Replay_CheckAndUpdate_LOOP2,  replayCtrl.winMaskIndex, MAX_WINMASK_BYTE_LEN2
                    ldi     replayCtrl.winMaskIndex, 0
                    jmp     Replay_CheckAndUpdate_LOOP2   
                    
Replay_CheckAndUpdate_L2_LagreWindow_end:
            // restore the window location
            mov CdeCmd.Cmd0.b1, SIZE(struct_ctxComm)
            xout XID_SCVCTRL, CdeCmd, 4
            
#endif            
            //pass through             

//        // We know we're always setting the bit at offset "winSize-1"
//        seqNumDiff = winSize - 1;
//
//        // The state here is SA_REPLAY_RC_NEW
//        rc = SA_REPLAY_RC_NEW;
//    }
Replay_CheckAndUpdate_L3:    
        sub     replayCheckLoc.seqNumDiff, replayCtrl.winSize, 1
        ldi     r29.b0, SA_REPLAY_RC_NEW

Replay_CheckAndUpdate_L4:

//
//    // We now know this packet is inside the window range
//    bit_number = seqNumDiff + pReplay->winMaskBitoff;
//
//    bit_offset   = bit_number & 0x1F;
//    index_offset = pReplay->winMaskIndex + (bit_number>>5);
//
//    // The bit mask can wrap
//    if( index_offset >= MAX_WORD_LEN )
//        index_offset -= MAX_WORD_LEN;
//
//    if( pReplay->win_mask[index_offset] & (1<<bit_offset) )
//        return SA_REPLAY_RC_DUP;
//
//    pReplay->win_mask[index_offset] |= (1<<bit_offset);
//    return rc;
//}

    // replace replayCheckLoc.bitNumber with r0.w0 to prevent overflow when winSize >= 256
    //add     replayCheckLoc.bitNumber,  replayCheckLoc.seqNumDiff, replayCtrl.winMaskBitoff
    //and     replayCheckLoc.bitOffset,  replayCheckLoc.bitNumber, 0x1f
    //lsr     replayCheckLoc.indexOffset,  replayCheckLoc.bitNumber, 5
    
    add     r0.w0,  replayCheckLoc.seqNumDiff, replayCtrl.winMaskBitoff
    and     replayCheckLoc.bitOffset,  r0.b0, 0x1f
    lsr     replayCheckLoc.indexOffset,  r0.w0, 5
    lsl     replayCheckLoc.indexOffset,  replayCheckLoc.indexOffset, 2   
    add     replayCheckLoc.indexOffset, replayCheckLoc.indexOffset, replayCtrl.winMaskIndex
    
#ifdef SASS_GEN2    
    // If winSize > 128, need to deal with winMask at both regions
    qblt    Replay_CheckAndUpdate_L7_LagreWindow,  replayCtrl.winSize, 128
#endif
    
    qbgt    Replay_CheckAndUpdate_L5, replayCheckLoc.indexOffset, MAX_WINMASK_BYTE_LEN
        sub     replayCheckLoc.indexOffset, replayCheckLoc.indexOffset, MAX_WINMASK_BYTE_LEN
         
Replay_CheckAndUpdate_L5:
    add     r1.b0,  replayCheckLoc.indexOffset, &replayCtrl.winMask0 
    mvid    r0, *r1.b0
    qbbc    Replay_CheckAndUpdate_L6, r0, replayCheckLoc.bitOffset
        ldi     r29.b0, SA_REPLAY_RC_DUP
        jmp     Replay_CheckAndUpdate_End 

Replay_CheckAndUpdate_L6:
        set     r0, r0, replayCheckLoc.bitOffset
        mvid    *r1.b0, r0

Replay_CheckAndUpdate_End:
    mov     r1.b0, r29.b1        // restore r1.b0
    jmp     r30.w2               // "return" 
    
#ifdef SASS_GEN2    
Replay_CheckAndUpdate_L7_LagreWindow:
    // Large window: receive sequence number check    
    qbgt    Replay_CheckAndUpdate_L8, replayCheckLoc.indexOffset, MAX_WINMASK_BYTE_LEN2
        sub     replayCheckLoc.indexOffset, replayCheckLoc.indexOffset, MAX_WINMASK_BYTE_LEN2
         
Replay_CheckAndUpdate_L8:

    // check indexOffset location for second half of window mask
    // winMaskIndex in bytes, if >= 20, move to secnd half of window and set a flag
    qbgt    Replay_CheckAndUpdate_L5, replayCheckLoc.indexOffset, 20 
        // Bit Mask is at the second half
        mov  CdeCmd.Cmd0.b0, SCV_CMD_POSITION_WINDOW
        add  CdeCmd.Cmd0.b1,  replayCheckLoc.indexOffset, 128 - 20
        xout XID_SCVCTRL, CdeCmd, 4  
        
        xin XID_SCVDATA, r6, 4
        qbbc    Replay_CheckAndUpdate_L9, r6, replayCheckLoc.bitOffset
            ldi     r29.b0, SA_REPLAY_RC_DUP
            jmp     Replay_CheckAndUpdate_End_LargeWindow 

Replay_CheckAndUpdate_L9:
        set   r6, r6, replayCheckLoc.bitOffset
        xout  XID_SCVDATA, r6, 4
        
Replay_CheckAndUpdate_End_LargeWindow:        
        // restore the window location
        mov  CdeCmd.Cmd0.b1, SIZE(struct_ctxComm)
        xout XID_SCVCTRL, CdeCmd, 4
        jmp  Replay_CheckAndUpdate_End

#endif
.leave  replayCheckScope   
.leave  replayCtrlScope


// IPSEC_ICVCheck
//
// This function is used to compare the hash value for both IPSEC
// ESP/AH
//
// Input:
//   R6-R9: ICV Value from the Authentication Routine
//   R10-R13: ICV value in the packet
//   R29.b0: icvSize
//
// Return Value:
//    R29.b0  0: No Match
//            icvSize:   
//    RC_DUP - Packet is inside the window, but has been seen before
//    RC_OK  - Packet in inside the window and has not been seen
//    RC_NEW - Packet is newer than the window
//
 IPSEC_ICVCheck:
    qbne    IPSEC_ICVCheck_Fail, r6, r10
    qbeq    IPSEC_ICVCheck_Match, r29.b0, 4
    qbne    IPSEC_ICVCheck_Fail, r7, r11
    qbeq    IPSEC_ICVCheck_Match, r29.b0, 8
    qbne    IPSEC_ICVCheck_Fail, r8, r12
    qbeq    IPSEC_ICVCheck_Match, r29.b0, 12
    qbne    IPSEC_ICVCheck_Fail, r9, r13
//qbeq    IPSEC_ICVCheck_Match, r29.b0, 16
//lbbo    r0, icv_address1, 16, 16 
//lbbo    r4, icv_address2, 16, 16 // skip
//qbne    IPSEC_ICVCheck_Fail, r0, r10
//qbeq    IPSEC_ICVCheck_Match, r29.b0, 20
//qbne    IPSEC_ICVCheck_Fail, r1, r11
//qbeq    IPSEC_ICVCheck_Match, r29.b0, 24
//qbne    IPSEC_ICVCheck_Fail, r2, r12
//qbeq    IPSEC_ICVCheck_Match, r29.b0, 28
//qbne    IPSEC_ICVCheck_Fail, r3, r13

 IPSEC_ICVCheck_Fail:
    mov r29.b0, 0

 IPSEC_ICVCheck_Match:
    ret     






