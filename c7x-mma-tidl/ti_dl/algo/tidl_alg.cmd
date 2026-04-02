/*
* module name       :TIDL 
*
* module descripton :TI Deep learning Library module is TIs CNN/DNN 
*                    acceleration on EVE and DSP 
*
* Copyright (C) 2016-2017 Texas Instruments Incorporated - http://www.ti.com/ 
*
* ALL RIGHTS RESERVED 
*
*
*/

/*===========================================================================*/
/* Partial Linker Command File for DMVAL on C64xplus platform               */
/* Purpose: To have hassel free integration of algorithm in system, system   */
/* integrator should not worry about managing multiple sections of code and  */
/* data.                                                                     */
/* Define code section(s) with naming convention:  .text:DMVAL_TI_cSectx     */
/* Define data section(s) with naming convention:  .const:DMVAL_TI_dSectx    */
/* Define const section(s) with naming convention: .far:DMVAL_TI_uSectx      */
/* - Hide all symbols, export XDAIS functions, create namespaced sections    */
/* - Create sections with optimised cache relative placement                 */
/*===========================================================================*/

/* Make relocatable object */
-r

/* Hide all symbols in this partial link */
/*-h*/

/* Make XDAIS Functions and Tables Symbols Globally Visible */
/*-g SFM_TI_VISION_FXNS*/

SECTIONS
{  
  /* ------------------------------------------ */
  /* IMPORTANT NOTE: Splitting external memory  */
  /* into sections shown, grouping of functions */
  /* and ordering inside groups is a crucial    */
  /* requirement for performance. Alteration    */
  /* in the above will lead to severe cache     */
  /* penalties and hence results in performace  */
  /* degradation.                               */
  /* ------------------------------------------ */

}                                                                     


