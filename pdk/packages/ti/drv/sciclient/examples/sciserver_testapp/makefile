#
# This file is the makefile for building Sciserver Test application.
#
ifeq ($(RULES_MAKE), )
include $(PDK_INSTALL_PATH)/ti/build/Rules.make
else
include $(RULES_MAKE)
endif

APP_NAME = sciserver_testapp_$(BUILD_OS_TYPE)

SRCDIR = . ../common
INCDIR = . ../common

# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON = sciserver_testapp_main.c \
              sci_app_common.c

CFLAGS_LOCAL_COMMON = $(PDK_CFLAGS)
PACKAGE_SRCS_COMMON = . ../common

ifeq ($(BUILD_OS_TYPE),freertos)
CFLAGS_OS_DEFINES = -DFREERTOS
# List all the components required by the application
COMP_LIST_COMMON  = $(PDK_COMMON_FREERTOS_COMP)
EXTERNAL_INTERFACES = freertos
ifeq ($(ENABLE_DM_TRACE),yes)
CONFIG_BLD_LNK_r5f = $(PDK_INSTALL_PATH)/ti/drv/sciclient/examples/sciserver_testapp/linker_r5_debug_trace_freertos.lds
else
CONFIG_BLD_LNK_r5f = $(PDK_INSTALL_PATH)/ti/drv/sciclient/examples/sciserver_testapp/linker_r5_freertos_$(BUILD_PROFILE).lds
endif
endif

ifeq ($(BUILD_OS_TYPE),safertos)
CFLAGS_OS_DEFINES = -DSAFERTOS
# List all the components required by the application
COMP_LIST_COMMON  = $(PDK_COMMON_SAFERTOS_COMP)
EXTERNAL_INTERFACES = safertos
CONFIG_BLD_LNK_r5f = $(PDK_INSTALL_PATH)/ti/drv/sciclient/examples/sciserver_testapp/safertos/$(SOC)/linker_r5_safertos.lds
CFLAGS_LOCAL_COMMON += -DSAFERTOS
endif

ifeq ($(ENABLE_DM_TRACE),yes)
CFLAGS_LOCAL_COMMON += -DENABLE_DM_TRACE
endif

COMP_LIST_COMMON += sciserver_tirtos

# Replace sciclient_direct with sciclient_merged if we want to use sciclient_merged library
ifeq ($(SCI_BUILD_INTERNAL), yes)
COMP_LIST_COMMON := $(filter-out sciclient_direct$(HS_SUFFIX), $(COMP_LIST_COMMON))
COMP_LIST_COMMON += sciclient_merged$(HS_SUFFIX)
endif

# List all the external components/interfaces, whose interface header files
# need to be included for this component
INCLUDE_EXTERNAL_INTERFACES = pdk sciclient board uart $(EXTERNAL_INTERFACES)

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

