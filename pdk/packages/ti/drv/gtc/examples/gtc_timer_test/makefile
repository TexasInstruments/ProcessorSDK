#
# This file is the makefile for buiding GTC Timer Test application
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = GTC_Timer_TestApp_$(BUILD_OS_TYPE)

SRCDIR = . $(PDK_INSTALL_PATH)/ti/drv/gtc/examples/gtc_timer_test/src
INCDIR = . $(PDK_INSTALL_PATH)/ti/drv/gtc/examples/gtc_timer_test/src

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
COMP_LIST_COMMON  = $(PDK_COMMON_FREERTOS_COMP)
EXTERNAL_INTERFACES = freertos
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk gtc board uart $(EXTERNAL_INTERFACES)

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON = gtc_timer_test.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(CFLAGS_OS_DEFINES)
PACKAGE_SRCS_COMMON = . src/

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


