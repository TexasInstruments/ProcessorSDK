#
# This file is the makefile for building DMA utils test app.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = dmautils_baremetal_autoincrement_testapp

SRCDIR = .
INCDIR = .

ifeq ($(DMA_UTILS_STANDALONE), 1)
DMAUTILS_COMP_LIST=csl
DMAUTILS_CFLAGS=-DDMA_UTILS_STANDALONE
INCDIR+=../../udma_standalone
else
DMAUTILS_COMP_LIST=$(PDK_COMMON_BAREMETAL_COMP)
endif
DMAUTILS_COMP_LIST += dmautils
# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk



# List all the components required by the application
COMP_LIST_COMMON = 
COMP_LIST_COMMON += $(DMAUTILS_COMP_LIST)

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON = dmautils_autoincrement_example.c dmautils_autoincrement_test.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
CFLAGS_LOCAL_COMMON += $(DMAUTILS_CFLAGS)

$(info CFLAGS_LOCAL_COMMON $(CFLAGS_LOCAL_COMMON))

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

# Include common make files
ifeq ($(SOC),$(filter $(SOC), j721e)) 
EXTERNAL_LNKCMD_FILE_LOCAL := c7x_vcop_lnk.cmd
endif
ifeq ($(SOC),$(filter $(SOC), am62a)) 
EXTERNAL_LNKCMD_FILE_LOCAL := c7x_vcop_lnk_am62a.cmd
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
