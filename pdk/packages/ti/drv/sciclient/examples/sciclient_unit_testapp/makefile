#
# This file is the makefile for building Sciclient Unit Test application.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = sciclient_unit_testapp_$(BUILD_OS_TYPE)

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
# List all the components required by the application
COMP_LIST_COMMON  = $(PDK_COMMON_SAFERTOS_COMP)
EXTERNAL_INTERFACES = safertos
endif

# SoCs which use split DM / TIFS can enable tests that use msg forwarding (mcu1_0 UT only)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  ifeq ($(CORE), mcu1_0)
    SCICLIENT_UT_CFLAGS = -DENABLE_MSG_FWD
  endif
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk sciclient board uart $(EXTERNAL_INTERFACES)

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON = sciclient_ut_main.c \
              sci_app_common.c uart_utils.c

# SoCs which use firewall exception notification tests (mcu1_0 UT only)
ifeq ($(SOC),$(filter $(SOC), j721e j7200 j721s2 j784s4 j742s2))
  ifeq ($(CORE), mcu1_0)
    ifeq ($(BUILD_OS_TYPE), freertos)
      SCICLIENT_UT_CFLAGS += -DENABLE_FW_NOTIFICATION
      SRCS_COMMON +=  sciclient_fw_notify.c
    endif
  endif
endif

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS) $(SCICLIENT_UT_CFLAGS) $(CFLAGS_OS_DEFINES)
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
