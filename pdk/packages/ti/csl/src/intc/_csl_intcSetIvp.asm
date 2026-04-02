;/*****************************************************************************
; *  Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008   
; *  
; *  Use of this software is controlled by the terms and conditions found in the
; *  license agreement under which this software has been supplied.             
; *****************************************************************************/

;/** @file _csl_intcSetIvp.asm
; *
; *  @date 12th June, 2004
; *  @author Ruchika Kharwar
; */
    ; A0 contains the Register to be read 
    
    .ref __CSL_intcVectorTable
    .sect  ".text:csl_section:intc"
    .global CSL_intcIvpSet
    .weak   CSL_intcIvpSet
CSL_intcIvpSet:
    bnop b3,2
    mvkl __CSL_intcVectorTable, b0
    mvkh __CSL_intcVectorTable, b0
    mvc b0, istp
 
