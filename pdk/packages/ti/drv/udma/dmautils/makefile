#
# This file is the makefile for building DMA Utils library.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = dmautils

SRCDIR = src
INCDIR = . src inc

ifeq ($(DMA_UTILS_STANDALONE), 1)
$(info Building in standalone mode with only csl depdendency)
DMAUTILS_CFLAGS=-DDMA_UTILS_STANDALONE
SRCDIR += udma_standalone/src
SRCS_COMMON+= udma.c
INCDIR+=udma_standalone
else
INCDIR+=../
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES=pdk


# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += dmautils_autoincrement_3d.c
SRCS_COMMON += dmautils_autoincrement_3d_hostemu.c
PACKAGE_SRCS_COMMON = .
ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
DMAUTILS_CFLAGS += --disable_inlining
endif
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(UDMA_CFLAGS) $(DMAUTILS_CFLAGS) 



# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<core/SoC/platform-name> =
#   CFLAGS_LOCAL_<core/SoC/platform-name> =

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
