
#
# This file is the makefile for building I2C extended app
#

ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

#Application name
APP_NAME = i2c_extended_testapp

SRCDIR = . src
INCDIR = .
# Common source files across all platforms and cores
SRCS_COMMON += i2c_extended_testapp.c

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk

# Include common make files
PACKAGE_SRCS_COMMON = src makefile

COMP_LIST_COMMON = $(PDK_COMMON_BAREMETAL_COMP)

ifeq ($(MAKERULEDIR), )
#Makerule path not defined, define this and assume relative path from ROOTDIR
  MAKERULEDIR := $(ROOTDIR)/ti/build/makerules
  export MAKERULEDIR
endif
include $(MAKERULEDIR)/common.mk

# Nothing beyond this point
