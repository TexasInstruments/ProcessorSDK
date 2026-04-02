/******************************************************************************
 * FILE PURPOSE: SA Source module specification file.
 ******************************************************************************
 * FILE NAME: module.xs
 *
 * DESCRIPTION: 
 *  This file contains the module specification for the SA source directory.
 *
 * Copyright (C) 2009, Texas Instruments, Inc.
 *****************************************************************************/

/* Load the library utility. */
var libUtility = xdc.loadCapsule ("../build/buildlib.xs");

/* List of all the SA LLD Files */
var salldFile   =     libUtility.listAllFiles (".c", "src", false);
salldFile = salldFile.concat(libUtility.listAllFiles (".c", "src/auth"));
salldFile = salldFile.concat(libUtility.listAllFiles (".c", "src/cipher"));
salldFile = salldFile.concat(libUtility.listAllFiles (".c", "src/proto"));

/* List of all the SA LLD Files */

var sa =
{
    /* library name */
    libname: "ti.drv.sa",
    
    /* Souce File List */
    srcFile: ["src/salld.c", "src/salldcom.c", "src/salldctbl.c",  "src/salldmci.c", "src/salldpka.c", "src/salldsc.c", "src/salldtxrx.c",
              "src/auth/salldcmac.c", "src/auth/salldhmac_md5.c", "src/auth/salldhmac_sha1.c",  "src/auth/salldhmac_sha2.c",
              "src/auth/salldmd5.c", "src/auth/salldsha1.c", "src/auth/salldsha2.c", "src/auth/salldxcbc.c",
              "src/cipher/salldaes.c", "src/cipher/salldaes_ctr.c", "src/cipher/salldaes_f8.c", "src/cipher/salldgcm.c",
              "src/proto/airciph/salldac.c", "src/proto/airciph/salldacinit.c",
              "src/proto/datamode/sallddm.c", "src/proto/datamode/sallddminit.c", "src/proto/datamode/sallddmcmdl.c",
              "src/proto/ipsec/salldah.c", "src/proto/ipsec/salldesp.c", "src/proto/ipsec/salldipsec.c", "src/proto/ipsec/salldipsecinit.c", 
              "src/proto/srtp/salldsrtp.c", "src/proto/srtp/salldsrtpinit.c",  "src/proto/srtp/salldsrtp_ctr.c", "src/proto/srtp/salldsrtp_f8.c", "src/proto/srtp/salldsrtpsc.c",
              "src/proto/srtp/salldsrtcp.c", "src/proto/srtp/salldsrtcpinit.c",  "src/proto/srtp/salldsrtcp_ctr.c", "src/proto/srtp/salldsrtcp_f8.c"
             ],
              
    /* Library options */
    copts: " -DDEVICE_K2H",

    /* Obj Location */
    objloc: "sa"
};

var salite =
{
    /* library name */
    libname: "ti.drv.salite",
    
    /* Souce File List */
    srcFile: ["src/salld.c", "src/salldcom.c", "src/salldctbl.c",  "src/salldmci.c", "src/salldpka.c", "src/salldsc.c", "src/salldtxrx.c",
              "src/auth/salldcmac.c", "src/auth/salldhmac_md5.c", "src/auth/salldhmac_sha1.c",  "src/auth/salldhmac_sha2.c",
              "src/auth/salldmd5.c", "src/auth/salldsha1.c", "src/auth/salldsha2.c", "src/auth/salldxcbc.c",
              "src/cipher/salldaes.c", "src/cipher/salldgcm.c",
              "src/proto/datamode/sallddm.c", "src/proto/datamode/sallddminit.c", "src/proto/datamode/sallddmcmdl.c",
              "src/pkafw/firmware_eip29t2.c"
             ],
              
    /* Library options */
    copts: " -DDEVICE_K2G -DNSS_LITE -DNSS_PKA_GEN2",

   /* Object location */
   objloc: "salite"
};


var devices = [salite, sa];



/**************************************************************************
 * FUNCTION NAME : modBuild
 **************************************************************************
 * DESCRIPTION   :
 *  The function is used to build all the components of the SA library
 **************************************************************************/
function modBuild() 
{
    /* Build the libraries for all the targets specified. */
    for (var dev = 0; dev < devices.length; dev++)
    {
        for (var targets=0; targets < Build.targets.length; targets++)
        {
            var libOptions = {
                copts: devices[dev].copts,
                incs: salldIncPath, 
            };
        
            libUtility.buildLibrary (libOptions, devices[dev].libname, Build.targets[targets], devices[dev].srcFile, devices[dev].objloc);
        }
    }

    /* Add all the .c files to the release package. */
    var testFiles = libUtility.listAllFiles (".c", "src");
    for (var k = 0 ; k < testFiles.length; k++)
        Pkg.otherFiles[Pkg.otherFiles.length++] = testFiles[k];

    /* Add all the .h files to the release package. */
    var testFiles = libUtility.listAllFiles (".h", "src");
    for (var k = 0 ; k < testFiles.length; k++)
        Pkg.otherFiles[Pkg.otherFiles.length++] = testFiles[k];
    /* Add all the .mk files to the release package. */
    var testFiles = libUtility.listAllFiles (".mk", "src");
    for (var k = 0 ; k < testFiles.length; k++)
        Pkg.otherFiles[Pkg.otherFiles.length++] = testFiles[k];

    /* Add all the .txt files to the release package. */
    var testFiles = libUtility.listAllFiles (".txt", "src");
    for (var k = 0 ; k < testFiles.length; k++)
        Pkg.otherFiles[Pkg.otherFiles.length++] = testFiles[k];		
}

