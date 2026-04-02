#
# Copyright (c) 2020, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# File: esm_component.mk
#       This file is component include makefile of ESM driver library.
# List of variables set in this file and their purpose:
# <mod>_RELPATH        - This is the relative path of the module, typically from
#                        top-level directory of the package
# <mod>_PATH           - This is the absolute path of the module. It derives from
#                        absolute path of the top-level directory (set in env.mk)
#                        and relative path set above
# <mod>_INCLUDE        - This is the path that has interface header files of the
#                        module. This can be multiple directories (space separated)
# <mod>_PKG_LIST       - Names of the modules (and sub-modules) that are a part
#                        part of this module, including itself.
# <mod>_BOARD_DEPENDENCY - "yes": means the code for this module depends on
#                             platform and the compiled obj/lib has to be kept
#                             under <platform> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no platform dependent code and hence
#                             the obj/libs are not kept under <platform> dir.
# <mod>_CORE_DEPENDENCY     - "yes": means the code for this module depends on
#                             core and the compiled obj/lib has to be kept
#                             under <core> directory
#                             "no" or "" or if this variable is not defined: means
#                             this module has no core dependent code and hence
#                             the obj/libs are not kept under <core> dir.
# <mod>_APP_STAGE_FILES     - List of source files that belongs to the module
#                             <mod>, but that needs to be compiled at application
#                             build stage (in the context of the app). This is
#                             primarily for link time configurations or if the
#                             source file is dependent on options/defines that are
#                             application dependent. This can be left blank or
#                             not defined at all, in which case, it means there
#                             no source files in the module <mod> that are required
#                             to be compiled in the application build stage.
#
ifeq ($(esm_component_make_include), )

drvesm_SOCLIST          = tpr12 awr294x
drvesm_tpr12_CORELIST   = $(DEFAULT_tpr12_CORELIST)
drvesm_awr294x_CORELIST   = $(DEFAULT_awr294x_CORELIST)
drvesm_BOARDLIST        = tpr12_evm tpr12_qt awr294x_evm

############################
# esm package
# List of components included under esm lib
# The components included here are built and will be part of esm lib
############################
esm_LIB_LIST = esm

############################
# esm examples
# List of examples under esm
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
esm_EXAMPLE_LIST =

#
# ESM Modules
#

# ESM library
export esm_COMP_LIST = esm
esm_RELPATH = ti/drv/esm
esm_PATH = $(PDK_ESM_COMP_PATH)
export esm_LIBNAME = esm
export esm_LIBPATH = $(esm_PATH)/lib
export esm_MAKEFILE = -f build/makefile.mk
export esm_BOARD_DEPENDENCY = no
export esm_CORE_DEPENDENCY = no
export esm_SOC_DEPENDENCY = yes
export esm_PKG_LIST = esm
esm_INCLUDE = $(esm_PATH)
export esm_SOCLIST = $(drvesm_SOCLIST)
export esm_$(SOC)_CORELIST = $(drvesm_$(SOC)_CORELIST)

export esm_LIB_LIST
export esm_EXAMPLE_LIST
export drvesm_LIB_LIST = $(esm_LIB_LIST)
export drvesm_EXAMPLE_LIST = $(esm_EXAMPLE_LIST)

export ESM_CFLAGS =

esm_component_make_include := 1
endif
