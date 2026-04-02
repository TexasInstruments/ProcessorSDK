#
# This file is the makefile for building mailbox.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = mailbox

SRCDIR = src soc/$(SOC)
INCDIR = . src soc/$(SOC)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += mailbox.c mailbox_osal.c mailbox_soc.c
ifeq ($(SOC),$(filter $(SOC), tpr12 awr294x))
  SRCS_COMMON += csl_mbox.c
endif
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(MAILBOX_CFLAGS)
ifeq ($(SOC),$(filter $(SOC), am64x))
CFLAGS_LOCAL_COMMON += -DVIM_DIRECT_REGISTRATION
endif

PACKAGE_SRCS_COMMON = mailbox.h makefile mailbox_component.mk .gitignore src soc/mailbox_soc.h soc/$(SOC) include
PACKAGE_SRCS_COMMON += config_mk.bld package.bld package.xdc package.xs
PACKAGE_SRCS_COMMON += Settings.xdc

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
