#
# This file is the makefile for building adcbuff.
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

MODULE_NAME = adcbuf

SRCDIR = src soc/$(SOC)
INCDIR = . src soc/$(SOC)

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += adcbuf.c adcbuf_mmwave.c
ifeq ($(SOC),$(filter $(SOC), awr294x))
  SRCS_COMMON += adcbuf_soc.c
endif
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(ADCBUF_CFLAGS)

PACKAGE_SRCS_COMMON = adcbuf.h adcbuf_component.mk makefile src soc/$(SOC) soc/adcbuf_soc.h
PACKAGE_SRCS_COMMON += config_mk.bld package.bld package.xdc package.xs
PACKAGE_SRCS_COMMON += Settings.xdc

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
