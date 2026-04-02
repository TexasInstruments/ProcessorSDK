/*
 *  Copyright (C) 2023 Texas Instruments Incorporated
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


        .text

/* FUNCTION DEF:    void PmuP_armR5SelectCntr( uint32_t cntrNum ) */

PmuP_armR5SelectCntr:
    AND     r0, r0, #0x1F
    MCR     p15, #0, r0, c9, c12, #5        // Write PMSELR Register
    BX      lr

/* FUNCTION DEF:    void PmuP_armR5Cfg( uint32_t cycleCntDiv, uint32_t exportEvents, uint32_t userEnable ) */

    .global PmuP_armR5Cfg
PmuP_armR5Cfg:
    MRC     p15, #0, r3, c9, c12, #0        // Read PMCR Register
    BIC     r3, r3, #((1<<3) | (1<<4))      // Clear D and X bits
    CMP     r0, #0
    BEQ     armR5Cfg_00
    ORR     r3, r3, #(1<<3)                 // Set D bit
armR5Cfg_00:
    CMP     r1, #0
    BEQ     armR5Cfg_01
    ORR     r3, r3, #(1<<4)                 // Set X bit
armR5Cfg_01:
    MCR     p15, #0, r3, c9, c12, #0        // Write PMCR Register
    EOR     r3, r3, r3                      // Clear r3
    CMP     r2, #0
    BEQ     armR5Cfg_02
    ORR     r3, r3, #1
armR5Cfg_02:
    MCR     p15, #0, r3, c9, c14, #0        // Write PMUSERENR Register
    BX      lr

/* FUNCTION DEF:    void PmuP_armR5EnableAllCntrs( uint32_t enable ) */

    .global PmuP_armR5EnableAllCntrs
PmuP_armR5EnableAllCntrs:
    MRC     p15, #0, r1, c9, c12, #0        // Read PMCR
    CMP     r0, #0
    BEQ     armR5EnableAllCntrs_disable
    ORR     r1, r1, #0x1                    // Set E bit to enable all counters
    B       armR5EnableAllCntrs_00
armR5EnableAllCntrs_disable:
    BIC     r1, r1, #0x1                    // Clr E bit to disable all counters
armR5EnableAllCntrs_00:
    MCR     p15, #0, r1, c9, c12, #0        // Write modified PMCR
    BX      lr

/* FUNCTION DEF:    uint32_t PmuP_armR5GetNumCntrs( void ) */

    .global PmuP_armR5GetNumCntrs
PmuP_armR5GetNumCntrs:
    MRC     p15, #0, r0, c9, c12, #0        // Read PMCR (Performance Monitor Control Register)
    LSR     r0, r0, #11                     // Shift and
    AND     r0, r0, #0x1F                   //   mask to get N
    BX      lr

/* FUNCTION DEF:    void PmuP_armR5CfgCntr( uint32_t cntrNum, PmuP_ArmR5EventType eventType ) */

    .global PmuP_armR5CfgCntr
PmuP_armR5CfgCntr:
    PUSH    {lr}
    BL      PmuP_armR5SelectCntr          // Select register
    AND     r1, r1, #0xFF
    MCR     p15, #0, r1, c9, c13, #1        // Write PMXEVTYPERx Register
    POP     {lr}
    BX      lr

/* FUNCTION DEF:    void PmuP_armR5EnableCntrOverflowIntr( uint32_t cntrNum, uint32_t enable ) */

    .global PmuP_armR5EnableCntrOverflowIntr
PmuP_armR5EnableCntrOverflowIntr:
    AND     r0, r0, #0x1F                   // cntrNum must be <= 31
    MOV     r2, #1
    LSL     r0, r2, r0                      // r0 is bit-mask corresponding to cntrNum
    CMP     r1, #0
    BEQ     armR5EnableCntrOverflowIntr_clear
    MCR     p15, #0, r0, c9, c14, #1        // Write PMINTENSET Register (writes of 0 have no effect)
    B       armR5EnableCntrOverflowIntr_00
armR5EnableCntrOverflowIntr_clear:
    MCR     p15, #0, r0, c9, c14, #2        // Write PMINTENCLR Register (writes of 0 have no effect)
armR5EnableCntrOverflowIntr_00:
    BX      lr

/* FUNCTION DEF:    void PmuP_armR5EnableCntr( uint32_t cntrNum, uint32_t enable ) */

    .global PmuP_armR5EnableCntr
PmuP_armR5EnableCntr:
    AND     r0, r0, #0x1F                   // cntrNum must be <= 31
    MOV     r2, #1
    LSL     r0, r2, r0                      // r0 is bit-mask corresponding to cntrNum
    CMP     r1, #0
    BEQ     armR5EnableCntrs_clear
    MCR     p15, #0, r0, c9, c12, #1        // Write PMCNTENSET Register (writes of 0 have no effect)
    B       armR5EnableCntrs_00
armR5EnableCntrs_clear:
    MCR     p15, #0, r0, c9, c12, #2        // Write PMCNTENCLR Register (writes of 0 have no effect)
armR5EnableCntrs_00:
    BX      lr


/* FUNCTION DEF:    uint32_t PmuP_armR5ReadCntr( uint32_t cntrNum ); */

    .global PmuP_armR5ReadCntr
PmuP_armR5ReadCntr:
    PUSH    {lr}
    CMP     r0, 0x1F
    BEQ     armR5ReadCntr_cycles
    BL      PmuP_armR5SelectCntr          // Select register
    MRC     p15, #0, r0, c9, c13, #2        // Read current PMNx Register
    B       armR5ReadCntr_00
armR5ReadCntr_cycles:
    MRC     p15, #0, r0, c9, c13, #0        // Read PMCCNTR Register
armR5ReadCntr_00:
    POP     {lr}
    BX      lr

/* FUNCTION DEF:    void  PmuP_armR5SetCntr( uint32_t cntrNum, uint32_t cntrVal ); */

    .global PmuP_armR5SetCntr
PmuP_armR5SetCntr:
    PUSH    {lr}
    CMP     r0, 0x1F
    BEQ     armR5SetCntr_cycles
    BL      PmuP_armR5SelectCntr          // Select register
    MCR     p15, #0, r1, c9, c13, #2        // Write current PMNx Register
    B       armR5SetCntr_00
armR5SetCntr_cycles:
    MCR     p15, #0, r1, c9, c13, #0        // Write PMCCNTR Register
armR5SetCntr_00:
    POP     {lr}
    BX      lr

/* FUNCTION DEF:    uint32_t PmuP_armR5ReadCntrOverflowStatus( void ) */

    .global PmuP_armR5ReadCntrOverflowStatus
PmuP_armR5ReadCntrOverflowStatus:
    MRC     p15, #0, r0, c9, c12, #3        // Read PMOVSR Register
    BX      lr


/* FUNCTION DEF:    void PmuP_armR5ClearCntrOverflowStatus( uint32_t cntrMask ) */


    .global PmuP_armR5ClearCntrOverflowStatus
PmuP_armR5ClearCntrOverflowStatus:
    MCR     p15, #0, r0, c9, c12, #3        // Write PMOVSR Register
    BX      lr

/* FUNCTION DEF:    void PmuP_armR5ResetCycleCnt( void ) */

    .global PmuP_armR5ResetCycleCnt
PmuP_armR5ResetCycleCnt:
    MRC     p15, #0, r0, c9, c12, #0        // Read PMCR
    ORR     r0, r0, #(1<<2)                 // Set C bit to reset the cycle counter, PMCCNTR, to zero
    MCR     p15, #0, r0, c9, c12, #0        // Write modified PMCR
    BX      lr


/* FUNCTION DEF:    void PmuP_armR5ResetCntrs( void ) */

    .global PmuP_armR5ResetCntrs
PmuP_armR5ResetCntrs:
    MRC     p15, #0, r0, c9, c12, #0        // Read PMCR
    ORR     r0, r0, #(1<<1)                 // Set P bit to reset all event counters to zero
    MCR     p15, #0, r0, c9, c12, #0        // Write modified PMCR
    BX      lr

/* FUNCTION DEF: uint32_t CycleCounterP_getCount32(void) */
        .global CycleCounterP_getCount32
        .type CycleCounterP_getCount32,%function
        .section ".text.pmu","ax",%progbits
        .arm
        .align 2
CycleCounterP_getCount32:
        MRC     p15, #0, r0, c9, c13, 0  // read PMCCNTR
        bx      LR

/* FUNCTION DEF: void PmuP_setup(uint32_t) */
        .global PmuP_setup
        .type PmuP_setup,%function
        .section ".text.pmu","ax",%progbits
        .arm
        .align 2
PmuP_setup:
        MCR     p15, #0, r0, c9, c12, 0 // write PMCR
        bx      LR

/* FUNCTION DEF: void PmuP_enableCounters(uint32_t) */
        .global PmuP_enableCounters
        .type PmuP_enableCounters,%function
        .section ".text.pmu","ax",%progbits
        .arm
        .align 2
PmuP_enableCounters:
        MCR     p15, #0, r0, c9, c12, 1  // write PMCNTENSET
        bx      LR

/* FUNCTION DEF: void PmuP_disableCounters(uint32_t) */
        .global PmuP_disableCounters
        .type PmuP_disableCounters,%function
        .section ".text.pmu","ax",%progbits
        .arm
        .align 2
PmuP_disableCounters:
        MCR     p15, #0, r0, c9, c12, 2  // write PMCNTENCLR
        bx      LR

/* FUNCTION DEF: uint32_t PmuP_getOverflowStatus(void) */
        .global PmuP_getOverflowStatus
        .type PmuP_getOverflowStatus,%function
        .section ".text.pmu","ax",%progbits
        .arm
        .align 2
PmuP_getOverflowStatus:
        MRC     p15, 0, r0, c9, c12, 3  // read PMOVSR
        bx      LR

/* FUNCTION DEF: uint32_t PmuP_clearOverflowStatus(void) */
        .global PmuP_clearOverflowStatus
        .type PmuP_clearOverflowStatus,%function
        .section ".text.pmu","ax",%progbits
        .arm
        .align 2
PmuP_clearOverflowStatus:
        MCR     p15, 0, r0, c9, c12, 3  // write PMOVSR
        bx      LR

	.end

