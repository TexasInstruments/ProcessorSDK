#
# This file is the makefile for building Software Diagnostics ESM Example app.
#
#   Copyright (c) Texas Instruments Incorporated 2020
#
include $(PDK_INSTALL_PATH)/ti/build/Rules.make

APP_NAME = diag_ex_esm_example_app

BUILD_OS_TYPE=baremetal

SRCDIR = . ../common
INCDIR =

INCDIR += $(SOC)/. ../common

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk sdr

# List all the components required by the application
COMP_LIST_COMMON = csl_utils_common csl_uart_console
COMP_LIST_COMMON += sdr

ifeq ($(SOC),$(filter $(SOC), j721e am64x))
  COMP_LIST_COMMON += $(PDK_COMMON_BAREMETAL_COMP)
endif

ifeq ($(CSL_BUILD),$(filter $(CSL_BUILD), CSL))
  ifneq ($(SOC),$(filter $(SOC), j721e am64x))
    COMP_LIST_COMMON = csl
  endif
else
  COMP_LIST_COMMON := $(filter-out csl,$(COMP_LIST_COMMON))
  COMP_LIST_COMMON = csl2 csl_trim
endif

# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = . ../common ../diag_ex_component.mk

ifeq ($(SOC),$(filter $(SOC), j721e am64x))
  SRCS_COMMON = main.c esm.c event_trig.c diag_utils.c
endif
LNKFLAGS_LOCAL_mpu1_0 += --entry Entry
CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)

CFLAGS_LOCAL_COMMON += -DUART_PRINTF

# To print events to UART during the flow of the Use Case itself,
# uncomment this line
# By default, tests are done with pin triggering with 100usec minimum
# interval time, i.e. this macro is disabled
#CFLAGS_LOCAL_COMMON += -DPRINT_EVENTS
# To print more verbose prints, uncomment this build flag
#CFLAGS_LOCAL_COMMON += -DDEBUG_PRINT

ifeq ($(SOC),$(filter $(SOC), j721e am64x))
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 SRCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/Unity/src
 INCDIR += $(PDK_INSTALL_PATH)/ti/build/unit-test/config
 SRCS_COMMON += unity_config.c unity.c
 CFLAGS_LOCAL_COMMON += -DUNITY_INCLUDE_CONFIG_H
endif


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
