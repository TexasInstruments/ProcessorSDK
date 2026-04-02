/******************************************************************************
 * FILE PURPOSE: Defines libarary directory name using getLibs
 ******************************************************************************
 * FILE NAME: package.xs
 *
 * DESCRIPTION: This file defines the library directory name for proper build
 *              in case a different directory name for storing library files 
 *              other than "lib" is used. XDC by default assumes that the 
 *              library directory is "lib" is not sepcifically indicated by use
 *              the attributes in a file called package.xs  
 *
 * TABS: NONE
 *
 * Copyright (C) 2008 - 2015, Texas Instruments, Inc.
 *****************************************************************************/

/*
 *  ======== Package.getLibs ========
 *  This function is called when a program's configuration files are
 *  being generated and it returns the name of a library appropriate
 *  for the program's configuration.
 */

function getLibs(prog)
{
    var suffix = prog.build.target.suffix;

    var name = this.$name + ".a" + suffix;
    var libProfileSupport = 0;

    if (this.Settings.deviceType.equals("k2g"))
    {
        name = this.$name + "lite" + ".a" + suffix;
    }

    if (this.Settings.deviceType.equals("am65xx"))
    {
        name = this.$name + "lite2" + ".a" + suffix;
    }

    if (this.Settings.deviceType.equals("j721e"))
    {
        name = this.$name + "lite2" + ".a" + suffix;
    }

    /* Read LIBDIR variable */
    var lib = java.lang.System.getenv("LIBDIR");

    /* If NULL, default to "lib" folder */
    if (lib == null)
    {
        lib = "./lib";
    } else {
        print ("\tSystem environment LIBDIR variable defined : " + lib);
    }

    /* Get target folder, if applicable */
    if ( java.lang.String(suffix).contains('66') )
        lib = lib + "/c66";
    else if (java.lang.String(suffix).contains('a53'))
        lib = lib + "/a53";
    else if (java.lang.String(suffix).contains('a72'))
        lib = lib + "/a72";
    else if (java.lang.String(suffix).contains('r5f'))
        lib = lib + "/r5f";
    else
        lib = lib + "/armv7"; 

    if (this.Settings.deviceType.equals("am65xx"))
    {
         /* Release profile or debug profile *
          * Note: for makefile based builds, two profiles are created
          */
        libProfileSupport = 1;
    }

    if (this.Settings.deviceType.equals("j721e"))
    {
         /* Release profile or debug profile *
          * Note: for makefile based builds, two profiles are created
          */
        libProfileSupport = 1;
    }

    if (libProfileSupport == 1)
    {

         var libProfiles = ["debug", "release"];
         /* get the configured library profile */
         for each(var profile in libProfiles)
         {
             if (this.Settings.libProfile.equals(profile))
             {
                 lib = lib + "/" + profile;
                 break;
             }
         }
    }
    /* Get library name with path */
    lib = lib + "/" + name;
    if (java.io.File(this.packageBase + lib).exists()) {
       print ("\tLinking with library " + this.$name + ":" + lib);
       return lib;
    }

    /* Could not find any library, throw exception */
    throw new Error("\tLibrary not found: " + this.packageBase + lib);
}

/*
 *  ======== package.init ========
 */
function init() {
xdc.loadPackage("ti.osal");
xdc.loadPackage("ti.csl");
}

/*
 *  ======== package.close ========
 */
function close()
{    
    if (xdc.om.$name != 'cfg') {
        return;
    }
}
