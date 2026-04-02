/*----------------------------------------------------------------------------*/
/* File: xip_entry.lds                                                        */
/* Description:								      */
/*    Link command file to jump to mcu1_0 sbl xip application		      */
/*                                                                            */
/*    Platform: R5 Cores on AM64xx, J7200, J721e                              */
/* (c) Texas Instruments 2021, All rights reserved.                           */
/*----------------------------------------------------------------------------*/

_xipAppEntry=0x501c0000;
--entry_point=_xipAppEntry
--retain="_xipAppEntry"
--retain="_xip_stub"

-stack 0x100

MEMORY
{
    OCMRAM_SBL_SYSFW	        : origin=0x41C3E000 length=0x2000	/* 8KB */
}

SECTIONS
{
    .text 	        : {} palign(8) 		> OCMRAM_SBL_SYSFW
    .stack 	        : {} palign(8) 		> OCMRAM_SBL_SYSFW
}
