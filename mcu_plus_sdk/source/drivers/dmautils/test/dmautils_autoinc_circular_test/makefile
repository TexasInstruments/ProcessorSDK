#
# This file is the makefile for building DMA utils test app.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = dmautils_baremetal_autoinc_circular_testapp

SRCDIR = .
INCDIR = .

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# List all the components required by the application
COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP) 
COMP_LIST_COMMON += dmautils

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON = dmautils_autoinc_circular_example.c dmautils_autoinc_circular_test.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(SOC),$(filter $(SOC), am62a)) 
EXTERNAL_LNKCMD_FILE_LOCAL := c7x_vcop_lnk_am62a.cmd
else
EXTERNAL_LNKCMD_FILE_LOCAL := c7x_lnk.cmd
endif

ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# OBJs and libraries are built by using rule defined in rules_<target>.mk
#     and need not be explicitly specified here

# Nothing beyond this point
