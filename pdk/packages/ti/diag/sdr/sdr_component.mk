# File: sdr_component.mk
#       This file is component include make file of Software Diagnostics Reference.
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
ifeq ($(sdr_component_make_include), )

############################
# Software Diagnostics Reference (SDR)
# List of components included under SDR
# The components included here are built and will be part of SDR
############################
sdr_LIB_LIST = sdr

compsdr_SOCLIST         = am65xx
compsdr_BOARDLIST       = am65xx_evm
compsdr_am65xx_CORELIST = mcu1_0
compsdr_DISABLE_PARALLEL_MAKE = yes

############################
# SDR examples / Test applications
# List of examples under sdr (+= is used at each example definition)
# All the tests mentioned in list are built when test target is called
# List below all examples for allowed values
############################
sdr_EXAMPLE_LIST =

# SDR dependent examples which should be built after all examples are completed
sdr_DUP_EXAMPLE_LIST =

#
# SDR Modules
#

# SDR (Software Diagnostics Reference)
sdr_COMP_LIST = sdr
sdr_RELPATH = ti/diag/sdr
sdr_PATH = $(PDK_SDR_COMP_PATH)
sdr_LIBNAME = sdr
sdr_LIBPATH = $(PDK_SDR_COMP_PATH)/lib
sdr_MAKEFILE = -fsrc/makefile
export sdr_MAKEFILE
export sdr_LIBNAME
export sdr_LIBPATH
sdr_BOARD_DEPENDENCY = no
sdr_CORE_DEPENDENCY = yes
export sdr_COMP_LIST
export sdr_BOARD_DEPENDENCY
export sdr_CORE_DEPENDENCY
sdr_PKG_LIST = sdr
sdr_INCLUDE = $(sdr_PATH)
sdr_SOCLIST = $(compsdr_SOCLIST)
export sdr_SOCLIST
sdr_$(SOC)_CORELIST = $(compsdr_$(SOC)_CORELIST)
export sdr_$(SOC)_CORELIST

#
# SDR Examples
#

# SDR test
sdr_test_COMP_LIST = sdr_test
sdr_test_RELPATH = ti/diag/sdr/test/sdtf-test
sdr_test_PATH = $(PDK_SDR_COMP_PATH)/test/sdtf-test
sdr_test_BOARD_DEPENDENCY = yes
sdr_test_CORE_DEPENDENCY = yes
sdr_test_XDC_CONFIGURO = no
export sdr_test_COMP_LIST
export sdr_test_BOARD_DEPENDENCY
export sdr_test_CORE_DEPENDENCY
export sdr_test_XDC_CONFIGURO
ifeq ($(SOC),$(filter $(SOC), am65xx))
sdr_test_APP_SMP_CONFIG = yes
export sdr_test_APP_SMP_CONFIG
endif
sdr_test_PKG_LIST = sdr_test
sdr_test_INCLUDE = $(sdr_test_PATH)
sdr_test_BOARDLIST = $(compsdr_BOARDLIST)
export sdr_test_BOARDLIST
sdr_test_$(SOC)_CORELIST = $(compsdr_$(SOC)_CORELIST)
export sdr_test_$(SOC)_CORELIST
sdr_EXAMPLE_LIST += sdr_test
export sdr_test_SBL_APPIMAGEGEN = yes

export sdr_LIB_LIST
export sdr_EXAMPLE_LIST
export sdr_DUP_EXAMPLE_LIST
export compsdr_LIB_LIST = $(sdr_LIB_LIST)
export compsdr_EXAMPLE_LIST  = $(sdr_EXAMPLE_LIST)
export compsdr_DUP_EXAMPLE_LIST  = $(sdr_DUP_EXAMPLE_LIST)

SDR_CFLAGS =

export SDR_CFLAGS

sdr_component_make_include := 1
endif
