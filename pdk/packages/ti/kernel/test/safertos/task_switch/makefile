#
# This file is the makefile for building SafeRTOS Task Switch Example App
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif


APP_NAME = task_switch_safertos
BUILD_OS_TYPE=safertos
CFLAGS_TEST_DEFINES =


SRCDIR = . ../common
INCDIR += . ../common

# List all the external components/interfaces, whose interface header files
#  need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk
INCLUDE_EXTERNAL_INTERFACES += safertos
ifeq ($(BUILD_OS_TYPE), safertos)
COMP_LIST_COMMON = $(PDK_COMMON_SAFERTOS_COMP)
else
# List all the components required by the application
endif


# Common source files and CFLAGS across all platforms and cores
PACKAGE_SRCS_COMMON = .
SRCS_COMMON += main.c task_switch.c SafeRTOSConfig.c aborts.c timerHooks.c
SRCS_COMMON += SafeRTOS_startup_$(ISA).c SafeRTOS_Hwi_$(ISA).c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_TEST_DEFINES)

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
