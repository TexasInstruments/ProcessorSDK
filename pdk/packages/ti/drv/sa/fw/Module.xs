/******************************************************************************
 * FILE PURPOSE: PA FW module specification file.
 ******************************************************************************
 * FILE NAME: module.xs
 *
 * DESCRIPTION: 
 *  This file contains the module specification for the PA fw directory.
 *
 * Copyright (C) 2009, Texas Instruments, Inc.
 *****************************************************************************/

/* Load the library utility. */
var libUtility = xdc.loadCapsule ("../build/buildlib.xs");

/**************************************************************************
 * FUNCTION NAME : modBuild
 **************************************************************************
 * DESCRIPTION   :
 *  The function is used to build all the components of the PA library
 **************************************************************************/
function modBuild() 
{
    /* Assemble the firmware */
 /*
    Pkg.makePrologue += ".PHONY: firmware\n\n";
    Pkg.makePrologue += "release: firmware\n";
    Pkg.makePrologue += "firmware: \n";
    Pkg.makePrologue += "\t $(ECHO) making the firmware for SASS version 0\n";
    Pkg.makePrologue += "\t cd fw/v0; pasm_dos -EBldXcm -CSa_php1 php1.p\n";
    Pkg.makePrologue += "\t cd fw/v0; pasm_dos -EBldXcm -CSa_php2 php2.p\n";
 */
    /* format the firmware assembler output */
 /*
    Pkg.makePrologue += "\t cd fw/v0; cat ../saphp1_desc.c > saphp1_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v0; cat php1_bin.h | sed -e s/\"unsigned int\"/\"uint32_t\"/g >> saphp1_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v0; $(ECHO) \"const int Sa_php1Size = sizeof(Sa_php1);\" >> saphp1_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v0; cp saphp1_bin.c ../. \n";
    
    Pkg.makePrologue += "\t cd fw/v0; cat ../saphp2_desc.c > saphp2_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v0; cat php2_bin.h | sed -e s/\"unsigned int\"/\"uint32_t\"/g >> saphp2_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v0; $(ECHO) \"const int Sa_php2Size = sizeof(Sa_php2);\" >> saphp2_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v0; cp saphp2_bin.c ../. \n";
 */   
     
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/safw.h";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v0/saphp1_bin.c";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v0/saphp2_bin.c";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v0/safw.h";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v0/php1.bib";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v0/php2.bib";
    
    /* add for backward compatibility */
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/saphp1_bin.c";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/saphp2_bin.c";
    
    /* Delete temporary files */
 /*  
    Pkg.makePrologue += "\t cd fw/v0; rm -f php1_bin.h php2_bin.h\n";
    
    Pkg.makePrologue += "\t $(ECHO) making the firmware for SASS version 1\n";
    Pkg.makePrologue += "\t cd fw/v1; pasm_dos -EBldXcm -CSa2_php1 php1.p\n";
    Pkg.makePrologue += "\t cd fw/v1; pasm_dos -EBldXcm -CSa2_php2 php2.p\n";
    Pkg.makePrologue += "\t cd fw/v1; pasm_dos -EBldXcm -CSa2_php3 php3.p\n";
  */
    /* format the firmware assembler output */
 /*  
    Pkg.makePrologue += "\t cd fw/v1; cat ../saphp1_desc.c > saphp1_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v1; cat php1_bin.h | sed -e s/\"unsigned int\"/\"uint32_t\"/g >> saphp1_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v1; $(ECHO) \"const int Sa2_php1Size = sizeof(Sa2_php1);\" >> saphp1_bin.c\n";
    
    Pkg.makePrologue += "\t cd fw/v1; cat ../saphp2_desc.c > saphp2_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v1; cat php2_bin.h | sed -e s/\"unsigned int\"/\"uint32_t\"/g >> saphp2_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v1; $(ECHO) \"const int Sa2_php2Size = sizeof(Sa2_php2);\" >> saphp2_bin.c\n";
    
    Pkg.makePrologue += "\t cd fw/v1; cat ../saphp3_desc.c > saphp3_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v1; cat php3_bin.h | sed -e s/\"unsigned int\"/\"uint32_t\"/g >> saphp3_bin.c\n";
    Pkg.makePrologue += "\t cd fw/v1; $(ECHO) \"const int Sa2_php3Size = sizeof(Sa2_php3);\" >> saphp3_bin.c\n";
 */
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v1/saphp1_bin.c";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v1/saphp2_bin.c";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v1/saphp3_bin.c";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v1/safw.h";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v1/php1.bib";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v1/php2.bib";
    Pkg.otherFiles[Pkg.otherFiles.length++] = "fw/v1/php3.bib";
    
    /* Delete temporary files */
   /*  Pkg.makePrologue += "\t cd fw/v1; rm -f php1_bin.h php2_bin.h php3_bin.h\n";
   */
}

