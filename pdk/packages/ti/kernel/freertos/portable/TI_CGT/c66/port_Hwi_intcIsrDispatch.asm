;/*****************************************************************************
; *  Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008   
; *  
; *  Use of this software is controlled by the terms and conditions found in the
; *  license agreement under which this software has been supplied.             
; *****************************************************************************/

;/** @file Hwi_intcIsrDispatch.asm
; *
; *  @date 12th June, 2004
; *  @author Ruchika Kharwar
; */
    .def _Hwi_intcVectorTable


    .ref CSL_intcCpuIntrTable
    .ref ti_sysbios_family_c64p_Hwi_dispatchAlways
    .ref CSL_Entry


RESV    .macro num
    .loop num
    mvkh CSL_intcCpuIntrTable,a4
    .endloop
    .endm
_Hwi_intcpush .macro reg
    stw reg,*b15--[2]
    .endm
_Hwi_intcpop .macro reg
    ldw *++b15[2],reg
    .endm
CALLDISP .macro intr
    _Hwi_intcpush a0
    mvk  intr, a0
    stw  a0, *b15[1]
    mvkl ti_sysbios_family_c64p_Hwi_dispatchAlways, a0
    mvkh ti_sysbios_family_c64p_Hwi_dispatchAlways, a0
    b       a0
    _Hwi_intcpop a0
    nop 4
    .endm   

    .sect ".hwi_vect"
    .align 0x400
    .nocmp
_Hwi_intcVectorTable:
__Hwi_intcRsv0:
    NOP
    NOP
    NOP
    mvkl CSL_Entry,b0
    mvkh CSL_Entry,b0
    b       b0
    NOP 
    NOP 4

    .align 0x20
__Hwi_intcIsrNMI:   
    CALLDISP 1

    .align 0x20
__Hwi_intcRsv2:
    RESV 8
    .align 0x20
__Hwi_intcRsv3:
    RESV 8

    .align 0x20
__Hwi_intcIsr4:
    CALLDISP 4 

    .align 0x20
__Hwi_intcIsr5:
    CALLDISP 5

    .align 0x20
__Hwi_intcIsr6:
    CALLDISP 6

    .align 0x20
__Hwi_intcIsr7:
    CALLDISP 7

    .align 0x20
__Hwi_intcIsr8:
    CALLDISP 8

    .align 0x20
__Hwi_intcIsr9:
    CALLDISP 9

    .align 0x20
__Hwi_intcIsr10:
    CALLDISP 10

    .align 0x20
__Hwi_intcIsr11:
    CALLDISP 11

    .align 0x20
__Hwi_intcIsr12:
    CALLDISP 12

    .align 0x20
__Hwi_intcIsr13:
    CALLDISP 13

    .align 0x20
__Hwi_intcIsr14:
    CALLDISP 14

    .align 0x20
__Hwi_intcIsr15:
    CALLDISP 15 
    
    .sect  ".text:hwi_text"
    .global CSL_intcIvpSet
CSL_intcIvpSet:
    bnop b3,2
    mvkl _Hwi_intcVectorTable, b0
    mvkh _Hwi_intcVectorTable, b0
    mvc b0, istp

