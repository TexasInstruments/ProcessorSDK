[TOC]

# Copyright modification introduction {#cpyright_intro}

This script provides a mechanism to introduce or update a source file with required copyright license.

__IMPORTANT NOTES:__

1) __Override capability:__ In some cases, it may be preferable to skip the copyright modification and hand
encode. Set the environment variable `TI_COPYRIGHT_SKIP` variable to 1 to make
the script skip copyright modifications.
2) __Expect Manual modification:__ Since makefile and assembly files have different formats for adding comments, the licence file may need variations correspondingly.
3) __Limited license replacement:__ The python script only replaces first copyright text within a file. In case if there are significant files with multiple copyright text then the script or source may need to be manually modified.
4) __Limitations due to patterns:__ The python script also looks only for some fixed patterns. If there are variants then again the script or source may need to be manually modified.
5) __Mandatory Review:__ Updated code must follow the standard review and inspection process dictated by the project to ensure there are no violations or invalid modifications.

# Steps to insert/modify the copyright {#cpyright_steps}

- If not already done, download the relevant copyright header template from [OSRB website](https://sps05.itg.ti.com/sites/TSPARepository/Open%20Source%20Wiki/OSLibraryHeaders.aspx). Standard header for SYSFW project is TI_Header.txt.
- Current script can handle .[ch], .p[yl], [mM]akefile, Kconfig, .sh , .asm files
	* .[ch] follows /* */ C comment style
	* .p[yl], [mM]akefile, Kconfig, .sh follow # comment style
	* .asm follow the ; comment style
	- __NOTE__: '.S' files are skipped and need manual fixups given the variations of formats followed (/* */ and ; comments are followed).
- Script modifies all License copyright contents following the first comment indicating Copyright.
	* Notice that all contents following the first copyright to the end of comments is basically considered as a Copyright and is replaced with the header content.
	* The script is also smart enough to pick up existing copyright's start year and update till end the end year. Example:
	* Example change:
~~~~~~~~~~~~~~~{.patch}
@@ -1,11 +1,6 @@
-# Copyright (C) 2017 Texas Instruments Incorporated
+# Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 # ALL RIGHTS RESERVED
 #
-# For a description of the syntax of this configuration file,
-# see https://www.kernel.org/doc/Documentation/kbuild/kconfig-language.txt
-# NOTE: We ONLY support "bool"
-#
-#
~~~~~~~~~~~~~~~
Notice:
~~~~~~~~~~~~~~~{.sh}
# Copyright (C) 2013 Texas Instruments Incorporated
~~~~~~~~~~~~~~~
gets modified to:
~~~~~~~~~~~~~~~{.sh}
# Copyright (C) 2013-2017 Texas Instruments Incorporated - http://www.ti.com/
~~~~~~~~~~~~~~~

This saves effort in making sure that copyright years are properly maintained.


## Fix copyrights on a listing of files {#cpyright_flist}
- Run command to list all the files of particular file types with or without. Example below indicates how to do on a linux OS:
~~~~~~~~~~~~~~~{.sh}
  find .  -iname '*.[chs]' -o -iname '*.asm' -o -iname "[mM]akefile" -o -iname Kconfig |tee filelist.txt
~~~~~~~~~~~~~~~
  Note: file types may need to be modified/added according to the requirements


- Run copyright_fix.py to add or modify license header

  Example:
~~~~~~~~~~~~~~~{.sh}
  python ./copyright_fix.py -l TI_HEADER.txt -f filelist.txt -s `pwd`
~~~~~~~~~~~~~~~
  Note: Since Makefile and assembly files may have different formats for adding comments, the copyright file would need variations

## Fix copyrights on a specific list of files {#cpyright_flist}


- Run copyright_fix.py to add or modify license header

~~~~~~~~~~~~~~~{.sh}
  python ./copyright_fix.py -l TI_HEADER.txt -s `pwd` -F fileA.c Makefile Kconfig FileB.c
~~~~~~~~~~~~~~~
or:
~~~~~~~~~~~~~~~{.sh}
  find .  -iname '*.[chs]' -o -iname '*.asm' -o -iname "[mM]akefile" -o -iname Kconfig | xargs python ./copyright_fix.py -l TI_HEADER.txt -s `pwd` -F
~~~~~~~~~~~~~~~

## Script usage {#cpyright_usage}
~~~~~~~~~~~~~~~{.sh}
	Usage: ./copyright_fix.py [-h] -l LICENSE
				  (-f FILELIST | -F [FILES [FILES ...]])
				  [-d DIRECTORY_LICENSE] [-s SOURCEDIR] [-D]

	Script that updates Copyright of files to TI Copyright

	required arguments:
	  -l LICENSE, --license LICENSE
				TI Copyright license template file from:
				https://sps05.itg.ti.com/sites/TSPARepository/Open
				Source Wiki/OSLibraryHeaders.aspx (default: None)

	optional arguments:
	  -d DIRECTORY_LICENSE, --directory_license DIRECTORY_LICENSE
				Location of License directory path (default:
				system-firmware/scripts/git-hooks/copyright)
	  -s SOURCEDIR, --sourcedir SOURCEDIR
				Source code directory base (default:
				system-firmware/scripts/git-hooks/copyright)
	  -D, --dummy_placeholder
				Use a dummy placeholder to fill up later (default:
				False)

	Required: (mutually exclusive) ways to select files to check/modify:
	  -f FILELIST, --filelist FILELIST
				Consolidated file containing files for which Copyright
				text needs to be checked (default: None)
	  -F [FILES [FILES ...]], --Files [FILES [FILES ...]]
				Explicit list of files to check to check for copyright
				(default: None)

~~~~~~~~~~~~~~~

__NOTE__: A placeholder is detected from license file input by using
{...} pattern with the exception of {YEAR} which is pre-parsed to
indicate year.

Example: {module description} is detected from license file as a
placeholder for inserting the description of module. using '-D' will
introduce a "TBD/FIXME" entry to the comment section if the file does
not contain a copyright. However an existing copyright in file will
assume existance of desired description. The following is a sample
code generated using -D and no copyrights ever applied on file.

~~~~~~~~~~~~~~~{.c}
/*
 * { TBD/FIXME: module name}
 *
 * { TBD/FIXME: module description}
 *
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */
~~~~~~~~~~~~~~~
