#
# This file is the makefile for building CSL2 library.
#
#   Copyright (c) Texas Instruments Incorporated 2019
#

.PHONY : gensizes

include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = csl2

SRCDIR = .
INCDIR = . ../..

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk csl2

# List all the internal submodules whose interface header files
# need to be included for this module
INCLUDE_INTERNAL_INTERFACES += csl2

# Common source files and CFLAGS across all platforms and cores
#Include src files for only the modules defined under csl_COMP_LIST
SRCS_COMMON =
# Include all common files and also the top level makefiles as CSL is the base module
PACKAGE_SRCS_COMMON = makefile makefile_csl2.mk src_files_csl2.mk \
                      csl_component.mk csl.h cslr.h csl_error.h \
                      cslr_device.h soc.h csl2_mainpage.h arch/cslr64.h \
                      hw_types.h tistdtypes.h csl_types.h csl_utils.h \
                      docs/doxyfile_tdax docs/tifooter.htm docs/tiheader.htm docs/tilogo.gif \
                      docs/titagline.gif \
                      docs/CSL_Interaction_diagram.png  docs/CSL_system_interface_diagram.png

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

#Include src files for only the modules defined under csl_intc_COMP_LIST
SRCS_COMMON =
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

# csl2 modules
-include $(PDK_CSL2_COMP_PATH)/src_files_csl2.mk

# Include common make files
ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
