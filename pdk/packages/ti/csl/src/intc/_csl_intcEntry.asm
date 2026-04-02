;/*****************************************************************************
; *  Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008   
; *  
; *  Use of this software is controlled by the terms and conditions found in the
; *  license agreement under which this software has been supplied.             
; *****************************************************************************/

;/** @file _csl_intcIsrDispatch.asm
; *
; *  @date 12th June, 2004
; *  @author Ruchika Kharwar
; */

TRUE                   .set   1
FALSE                  .set   0

; Set this value to FALSE to disable spinhere debug
CSL_SPINHERE_ENABLE    .set   FALSE

    .if CSL_SPINHERE_ENABLE
	.else
    .def _CSL_Entry
	.ref __c_int00
    .endif

    .sect ".text:csl_entry"
    .align 32          


    
   .if CSL_SPINHERE_ENABLE
_CSL_Entry:
wait_here:
      NOP 5
      B wait_here
      NOP 5
      NOP 5
      NOP 5
      NOP 5
      NOP 5
done:
   .else
_CSL_Entry:
    MVKL __c_int00,B0
    MVKH __c_int00,B0
    B B0
    NOP 5
    NOP 5
    NOP 5
    NOP 5
    NOP 5
   .endif

