#
# This file is the makefile for building Sciclient rtos test application.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = sciclient_rtos_app_$(BUILD_OS_TYPE)

SRCDIR = . $(PDK_INSTALL_PATH)/ti/drv/sciclient/examples/common
INCDIR = . $(PDK_INSTALL_PATH)/ti/drv/sciclient/examples/common

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
# List all the components required by the application
COMP_LIST_COMMON  = $(PDK_COMMON_FREERTOS_COMP)
EXTERNAL_INTERFACES = freertos
endif

ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_OS_DEFINES = -DSAFERTOS
COMP_LIST_COMMON  = $(PDK_COMMON_SAFERTOS_COMP)
EXTERNAL_INTERFACES = safertos
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk sciclient board uart $(EXTERNAL_INTERFACES)

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON = sciclient_rtos_main.c \
              sci_app_common.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)
PACKAGE_SRCS_COMMON = . ../common

# Core/SoC/platform specific source files and CFLAGS
# Example:
#   SRCS_<coqre/SoC/platform-name> =
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
